#pragma warning(disable C4267 C4244 C4554 C4477)
#include "TTArchive2.hpp"

bool TTArchive2::GetResourceInfo(const Symbol& s, ResourceInfo* i) {
	if (!i)return false;
	TTArchive2::ResourceEntry* e = _FindResource(s);
	if (!e)return false;
	i->mOffset = e->mOffset;
	i->mSize = e->mSize;
	return true;
}

void TTArchive2::Activate(DataStream* inArchiveStream) {
#define getint(dest,size)inArchiveStream->Serialize((char*)dest, size);
	if (mbActive)Deactivate();
	DataStreamContainerParams params;
	params.mpSrcStream = inArchiveStream;
	DataStreamContainer* c;
	inArchiveStream = c = new DataStreamContainer(params);
	mpInStream = inArchiveStream;
	u64 version{ 0 };
	c->Read(params.mpSrcStream->GetPosition(), &version);
	version = 0;
	inArchiveStream->Serialize((char*)&version, 4);
	if (version == 1414807858) {//TTA2: Telltale Archive 2
		version = 0;
		goto v3;
	}
	else if (version == 1414807859) {//TTA3
		version = 1;
	v3:
		u32 i;
		getint(&i, 4);
		if (i > 15)return;
	}
	else if (version != 1414807860) {//if not TTA4
		return;
	}
	else version = 2;
	u32 namesize, resources;
	getint(&namesize, 4);
	if (namesize > 0x10000000)return;
	this->mNamePageCount = namesize / 0x10000;
	getint(&resources, 4);
	if (resources > 0xFFFFF)return;
	mResources.reserve(resources);
	u32 u;
	//int* buffer = version < 1 ? new int[resources] : NULL;
	for (int i = 0; i < resources; i++) {
		ResourceEntry entry;
		getint(&entry.mNameCRC, 8);
		getint(&entry.mOffset, 8);
		if (version < 1) {//if v2
			//getint(buffer + i, 4);
			getint(&u, 4);//skipped member
		}
		getint(&entry.mSize, 4);
		getint(&u, 4);//skipped member
		getint(&entry.mNamePageIndex, 2);
		getint(&entry.mNamePageOffset, 2);
		mResources.push_back(entry);
	}
	u32 startoff = inArchiveStream->GetPosition() + namesize;
	mpNameStream = inArchiveStream->GetSubStream(startoff - namesize, namesize);
	mpResourceStream = inArchiveStream->GetSubStream(startoff,
		inArchiveStream->GetSize() - startoff);
	//delete[] buffer;
	mbActive = true;
	mVersion = version;
}

DataStream* TTArchive2::GetResourceStream(TTArchive2::ResourceEntry* entry) {
	return new DataStreamSubStream(mpResourceStream, (unsigned __int64)entry->mSize, entry->mOffset);
}

bool TTArchive2::Create(ProgressFunc func, DataStream* pDst, TTArchive2::ResourceCreateEntry* pFiles, int pNumFiles,
	bool pEncrypt, bool pCompress, Compression::Library
	pCompressionLibrary, u32 pVersion) {
#define writeint(i,size) out.Serialize((char*)i,size);
	if (!pDst || pVersion > 2)return false;
	char buf1[MAX_PATH];
	char buf2[MAX_PATH];
	GetTempPathA(MAX_PATH, buf1);
	GetTempFileNameA(buf1, "create_ttarch2_ttlib", 0, buf2);
	DataStreamFileDisc out = DataStreamFileDisc(PlatformSpecOpenFile(buf2, WRITE), DataStreamMode::eMode_Write);
	u32 vh;
	if (pVersion == 0) {
		vh = 1414807858;//TTA2
	}
	else if (pVersion == 1) {
		vh = 1414807859;//TTA3
	}
	else {
		vh = 1414807860;//TTA4
	}
	writeint(&vh, 4);
	u32 i = 2;
	if (pVersion <= 1) {
		writeint(&i, 4);
	}
	i = 0;
	for (int x = 0; x < pNumFiles; x++) {
		i += (u32)(pFiles + x)->name.length() + 1;
	}
	u32 ntz = i;
	i += 0x10000 - (i % 0x10000);
	writeint(&i, 4);
	writeint(&pNumFiles, 4);
	if(func)
		func("Writing Headers", 10);
	u64 curroff = 0;
	u32 curnameoff = 0;
	for (int i = 0; i < pNumFiles; i++) {
		ResourceCreateEntry* entry = pFiles + i;
		u64 crc = CRC64_CaseInsensitive(0, entry->name.c_str());
		writeint(&crc, 8);
		writeint(&curroff, 8);
		crc = 0;
		if(pVersion < 1)
			writeint(&crc, 4);
		crc = entry->mpStream->GetSize();
		writeint(&crc, 4);
		crc = 0;
		writeint(&crc, 4);
		crc = curnameoff / 0x10000;
		writeint(&crc, 2);
		crc = curnameoff % 0x10000;
		writeint(&crc, 2);
		curnameoff += (u32)entry->name.length() + 1;
		curroff += entry->mpStream->GetSize();
	}
	float pr = 10;
	float incr = 10 / pNumFiles;
	for (int x = 0; x < pNumFiles; x++) {
		out.SerializeWrite((pFiles + x)->name.c_str(), (u32)(pFiles + x)->name.length() + 1);
		//func(NULL, pr);
		pr += incr;
	}
	int rem = 0x10000 - (ntz%0x10000);
	char* temp = (char*)calloc(1, rem);
	out.Serialize(temp, rem);
	free(temp);
	incr = 60 / pNumFiles;
	for (int x = 0; x < pNumFiles; x++) {
		(pFiles + x)->mpStream->Copy(&out, out.GetPosition(), 0, (pFiles + x)->mpStream->GetSize());
		if (func)
			func((pFiles+x)->name.c_str(), pr);
		pr += incr;
	}
	pr = 80;//in case
	out.SetPosition(0, DataStreamSeekType::eSeekType_Begin);
	out.SetMode(DataStreamMode::eMode_Read);
	DataStreamContainerParams p;
	p.mpSrcStream = &out;
	p.mpDstStream = pDst;
	p.mDstOffset = pDst->GetPosition();
	p.mbEncrypt = pEncrypt;
	p.mbCompress = pCompress;
	p.mCompressionLibrary = pCompressionLibrary;
	pr = 85;
	DataStreamContainer::Create(func,p, out.GetSize());
	pr = 100;
	if (func)
		func("Created container", pr);
	return true;
}

String* TTArchive2::GetResourceName(const Symbol& name, String* result) {
	if (!mbActive)return NULL;
	TTArchive2::ResourceEntry* e = _FindResource(name);
	if (!e)return NULL;
	if (!mpNamePageCache)
		mpNamePageCache = (char*)malloc(0x10000);
	if (!mpNamePageCache)
		return NULL;
	if (mNamePageCacheIndex != e->mNamePageIndex) {
		mpNameStream->SetPosition(e->mNamePageIndex << 16,
			DataStreamSeekType::eSeekType_Begin);
		mpNameStream->Serialize(mpNamePageCache, 0x10000);
		mNamePageCacheIndex = e->mNamePageIndex;
	}
	result->assign(mpNamePageCache + e->mNamePageOffset);
	return result;
}

bool TTArchive2::HasResource(const Symbol& sym) {
	return _FindResource(sym) != NULL;
}

TTArchive2::ResourceEntry* TTArchive2::_FindResource(const Symbol& sym) {
	if (mbActive) {
		int i = 0;
		for (auto it = mResources.begin(); it != mResources.end(); it++, i++) {
			if (sym.GetCRC() == it->mNameCRC)
				return mResources.data() + i;
		}
	}
	return NULL;
}

void TTArchive2::Deactivate() {
	if (mbActive) {
		mNamePageCacheIndex = -1;
		if (mpResourceStream)
			delete mpResourceStream;
		if (mpNameStream)
			delete mpNameStream;
		if (mResources.size())
			mResources.clear();
		if (mpInStream)
			delete mpInStream;
		if (mpNamePageCache)
			free(mpNamePageCache);
		mbActive = false;
	}
}