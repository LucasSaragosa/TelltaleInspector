#pragma warning(disable C4267 C4244 C4554 C4477)
#include "HashDB.h"

const u32 MAGIC = 0x54544c42;

HashDatabase::HashDatabase(DataStream* stream) {
	if (!stream)throw "bad stream";
	this->db_stream = stream;
	this->db_pages = NULL;
	this->flags = 0;
	this->numPages = 0;
	this->cached_page = NULL;
	this->bufferedPage = NULL;
}

HashDatabase::Page* HashDatabase::PageAt(int index) {
	if (index >= this->NumPages())return NULL;
	return this->db_pages ? this->db_pages[index] : NULL;
}

HashDatabase::Page** HashDatabase::Pages() {
	return this->db_pages;
}

DB_FN(NumPages, int) {
	return pDatabase->NumPages();
}

DB_FN(NumEntries, int) {
	return pDatabase->NumEntries();
}

DB_FN(Flags, int) {
	return pDatabase->Flags();
}

DB_FN(PageAt, HashDatabase::Page*, SEP int index) {
	return pDatabase->PageAt(index);
}

DB_FN(FindPage, HashDatabase::Page*, SEP const char* pageName) {
	return pDatabase->FindPage(pageName);
}

DB_FN(FindEntry, void, SEP HashDatabase::Page* pPage SEP u64 crc SEP char* r) {
	return pDatabase->FindEntry(pPage, crc, r);
}

void HashDatabase::FindEntry(HashDatabase::Page* page, u64 crc, String* result) {
	char res[1024];
	res[0] = 0;
	FindEntry(page, crc, res);
	if (res[0] == 0)
		return;
	result->operator=(res);
}

void HashDatabase::FindEntry(HashDatabase::Page* page, u64 crc, char* result) {
	if (!result || !crc)
		return;
	result[0] = 0;
	if (page) {
		if (!(cached_page && cached_page == page)) {
			this->db_stream->SetPosition(page->offset, DataStreamSeekType::eSeekType_Begin);
			if (bufferedPage)free(bufferedPage);
			bufferedPage = (char*)malloc(page->count * 0xC);
			this->db_stream->Serialize(bufferedPage, page->count * 0xC);
			cached_page = page;
		}
		char* cp;
		u32 c;
		for (int i = 0; i < page->count; i++) {
			cp = (bufferedPage + 0xC * i);
			if (*((u64*)(cp)) == crc) {
				cp = (bufferedPage + 0xC * i + 8);
				c = *((u32*)(cp));
				db_stream->SetPosition((page->count * 0xC) + page->offset + c, DataStreamSeekType::eSeekType_Begin);
				int len = 0;
				if (i + 1 != page->count) {
					cp = (bufferedPage + (0xC * (i+1)) + 8);
					len = *((u32*)(cp));
					len -= c;
				}
				else {
					len = page->size - (0xC * page->count) - c;
				}
				if (len >= 1024) {
					TelltaleToolLib_RaiseError("hash database entry too large (>=1024)", ErrorSeverity::ERR);
					return;
				}
				db_stream->Serialize(result, len);
				return;
			}
		}
		return;
	}
	else {
		for (int i = 0; i < NumPages(); i++) {
			FindEntry(PageAt(i), crc, result);
			if ((*result))
				return;
		}
		return;
	}
}


void HashDatabase::DumpPage(Page* page, std::vector<String>& dest)
{
	char result[1024];
	if (page) {
		if (!(cached_page && cached_page == page)) {
			this->db_stream->SetPosition(page->offset, DataStreamSeekType::eSeekType_Begin);
			if (bufferedPage)free(bufferedPage);
			bufferedPage = (char*)malloc(page->count * 0xC);
			this->db_stream->Serialize(bufferedPage, page->count * 0xC);
			cached_page = page;
		}
		dest.reserve(page->count);
		char* cp;
		u32 c;
		for (int i = 0; i < page->count; i++) {
			cp = (bufferedPage + 0xC * i + 8);
			c = *((u32*)(cp));
			db_stream->SetPosition((page->count * 0xC) + page->offset + c, DataStreamSeekType::eSeekType_Begin);
			int len = 0;
			if (i + 1 != page->count) {
				cp = (bufferedPage + (0xC * (i + 1)) + 8);
				len = *((u32*)(cp));
				len -= c;
			}
			else {
				len = page->size - (0xC * page->count) - c;
			}
			if (len >= 1024) {
				TelltaleToolLib_RaiseError("hash database entry too large (>=1024)", ErrorSeverity::ERR);
				return;
			}
			db_stream->Serialize(result, len);
			bool cont = false;
			if (page->count < 5000) {
				for (auto it = dest.begin(); it != dest.end(); it++)
					if (*it == result) {
						cont = true;
						break;
					}
				if (cont)
					continue;
			}
			dest.push_back(String{ result });
		}
	}
}

bool HashDatabase::Open() {
	if (!this->db_stream)return false;
	DataStream* stream = this->db_stream;
	int magic;
	stream->Serialize((char*)&magic, 4);
	if (magic != MAGIC)return false;
	stream->Serialize((char*)&flags, 1);
	stream->Serialize((char*)&this->numPages, 4);
	if (this->db_pages)return false;//cannot open another
	this->db_pages = (HashDatabase::Page**)calloc(sizeof(HashDatabase::Page*), this->numPages);
	if (!this->db_pages)return false;
	for (int i = 0; i < this->numPages; i++) {
		HashDatabase::Page* page = new Page();
		stream->Serialize((char*)&page->count, 4);
		stream->Serialize((char*)&page->flags, 1);
		int len;
		stream->Serialize((char*)&len, 4);
		char* tmp = new char[len + 1];
		tmp[len] = 0;
		stream->Serialize(tmp, len);
		page->pageName = tmp;
		stream->Serialize((char*)&page->size, 4);
		page->offset = stream->GetPosition();
		stream->SetPosition(page->size + stream->GetPosition(), DataStreamSeekType::eSeekType_Begin);
		this->db_pages[i] = page;
	}
	return true;
}

u32 HashDatabase::Flags() {
	return this->flags;
}

HashDatabase::Page* HashDatabase::FindPage(const char* n) {
	if (!this->db_pages)return NULL;
	int pages = this->NumPages();
	for (int i = 0; i < pages; i++) {
		if (!_stricmp(PageAt(i)->pageName, n)) return PageAt(i);
	}
	return NULL;
}

u32 HashDatabase::NumEntries() {
	u32 ret = 0;
	int pages = NumPages();
	for (int i = 0; i < pages; i++) {
		ret += PageAt(i)->count;
	}
	return ret;
}

u32 HashDatabase::NumPages() {
	return this->numPages;
}

HashDatabase::~HashDatabase() {
	if (this->db_stream)
		delete db_stream;
	for (int i = 0; i < NumPages(); i++) {
		delete[] PageAt(i)->pageName;
		delete PageAt(i);
	}
	if (this->db_pages)
		free(this->db_pages);
	if (this->bufferedPage)
		free(bufferedPage);
}