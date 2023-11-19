//Taken from my old Telltale lib, LibTelltale!

#ifndef _HASHDB
#define _HASHDB

#include "../TelltaleToolLibrary.h"

#include <vector>

#define HASHDB_EXT HashDB

const extern u32 MAGIC;

/*
* A database of hashes which the library uses to search for its CRCs. Similar to RTBs, except more complex with pages of entries to make quicker searching
* Use this to find a CRC to string mapping
*/
class HashDatabase {
public:
	struct Page {
		char* pageName;
		u16 flags;
		u32 count;
		u64 offset;
		u64 size;
	};

	~HashDatabase();
	//TAKES OWNERSHIP OF STREAM!
	HashDatabase(DataStream* db);
	bool Open();
	u32 NumPages();
	u32 NumEntries();
	u32 Flags();
	Page* PageAt(int index);
	Page** Pages();
	Page* FindPage(const char* name);
	void FindEntry(Page* page, u64 crc,String* result);
	void FindEntry(Page* page, u64 crc, char* result);

	void DumpPage(Page* page, std::vector<String>& dest);

protected:
	DataStream* db_stream;
	Page** db_pages;
	int numPages;
	int flags;
	char* bufferedPage;
	Page* cached_page;
};

typedef HashDatabase::Page Page;

#define SEP ,

#define DB_FN(_NAME, _RETURN, _ARGS) _TTToolLib_Exp _RETURN TelltaleToolLib_HashDB_ ## _NAME(HashDatabase* pDatabase _ARGS)

DB_FN(NumPages, int);
DB_FN(NumEntries, int);
DB_FN(Flags, int);
DB_FN(PageAt, Page*, SEP int index);
DB_FN(FindPage, Page*, SEP const char* pageName);
/*result must be a char[1024]*/
DB_FN(FindEntry, void, SEP Page* pPage SEP u64 crc SEP char* result);

#endif