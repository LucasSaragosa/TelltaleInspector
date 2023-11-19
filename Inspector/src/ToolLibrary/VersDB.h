#ifndef _VERS_DB_H
#define _VERS_DB_H

#include "Meta.hpp"
#include "Types/DCArray.h"
#include "DataStream/DataStream.h"
#include "TelltaleToolLibrary.h"

/*Updated */
#define MINIMUM_VERSDB_GAME "Borderlands"

//flatfile db, tables are 'entry' lists each struct is a row.
class TelltaleVersionDatabase {
public:

	static constexpr char MAGIC[9] = "TTVERSDB";

	enum VersFlags {
		eVersionCRCS_Outdated = 1,
	};

	struct EnumEntry {
		unsigned __int64 mpEnumName;
		int mEnumValue;
		int mFlags;
		int mpNext = -1;
	};

	struct FlagEntry {
		unsigned __int64 mpFlagName;
		int mFlagValue;
		int mpNext = -1;
	};

	struct ClassEntry {
		unsigned __int64 mpTypeInfoName;
		unsigned __int64 mpExt;
		unsigned __int64 mHash;
		//if the actual one is not -1, then it is correct in the game (should equal versioncrc, if not then use actual)
		unsigned __int32 mFlags, mClassSize, mVersionCRC, mActualVersionCRC = -1;

		int mpFirstMember = -1;

		inline bool is_crc_definite() {
			return mActualVersionCRC != -1;
		}

		inline unsigned __int32 get_best_version_crc_estimate() {
			return mActualVersionCRC != -1 ? mActualVersionCRC : mVersionCRC;
		}

	};

	struct MemberEntry {
		unsigned __int64 mpName;
		unsigned __int32 mOffset, mFlags;
		int mpHostClass;
		int mpNextMember = -1;
		int mpEnumDesc = -1, mpFlagDesc = -1;
		/*its a normal pointer index into the classes array, stored as a 64 bit value for backend purposes in serialization*/
		unsigned __int64 mpMemberDesc;
	};

	int mVersion;
	int mDBFlags;
	DCArray<ClassEntry> mClasses;
	DCArray<FlagEntry> mFlags;
	DCArray<EnumEntry> mEnums;
	DCArray<MemberEntry> mMembers;
	char* mpNamingBuffer;

	inline ClassEntry* PushClass() {
		ClassEntry t{};
		mClasses.AddElement(0, 0, &t);
		return mClasses.mpStorage + (mClasses.mSize - 1);
	}

	inline MemberEntry* PushMember() {
		MemberEntry t{};
		mMembers.AddElement(0, 0, &t);
		return mMembers.mpStorage + (mMembers.mSize - 1);
	}

	inline EnumEntry* PushEnum() {
		EnumEntry t{};
		mEnums.AddElement(0, 0, &t);
		return mEnums.mpStorage + (mEnums.mSize - 1);
	}

	inline FlagEntry* PushFlag() {
		FlagEntry t{};
		mFlags.AddElement(0, 0, &t);
		return mFlags.mpStorage + (mFlags.mSize - 1);
	}

	inline bool AreVersionCRCsOutdated() const {
		return (mDBFlags & eVersionCRCS_Outdated) != 0;
	}

	inline ClassEntry* FindClass(unsigned __int64 crc) {
		for (int i = 0; i < mClasses.GetSize(); i++) {
			if (mClasses[i].mHash == crc)
				return mClasses.mpStorage + i;
		}
		return NULL;
	}

	inline TelltaleVersionDatabase() {
		mpNamingBuffer = nullptr;
		mDBFlags = 0;
		mVersion = 0;
	}

	inline ~TelltaleVersionDatabase() {
		if (mpNamingBuffer)
			delete[] mpNamingBuffer;
	}

	//leaves this database emptry
	inline void SerializeOut(DataStream* pOut) {
		mVersion = 3;
		/*gen names buffer and format classes for write*/
		char* names_temp = new char[0x800000];//8MB
		u64 names_off = 0;
		for (int i = 0; i < mClasses.GetSize(); i++) {
			ClassEntry* pClass = mClasses.mpStorage + i;
			int len = strlen((const char*)pClass->mpTypeInfoName) + 1;
			memcpy(names_temp + names_off, (const char*)pClass->mpTypeInfoName, len);
			pClass->mpTypeInfoName = names_off;
			names_off += len;
			if (pClass->mpExt) {
				len = strlen((const char*)pClass->mpExt) + 1;
				memcpy(names_temp + names_off, (const char*)pClass->mpExt, len);
				pClass->mpExt = names_off;
				names_off += len;
			}
		}
		for (int i = 0; i < mMembers.GetSize(); i++) {
			MemberEntry* pEntry = mMembers.mpStorage + i;
			int len = strlen((const char*)pEntry->mpName) + 1;
			memcpy(names_temp + names_off, (const char*)pEntry->mpName, len);
			pEntry->mpName = names_off;
			names_off += len;
		}
		for (int i = 0; i < mFlags.GetSize(); i++) {
			FlagEntry* pEntry = mFlags.mpStorage + i;
			int len = strlen((const char*)pEntry->mpFlagName) + 1;
			memcpy(names_temp + names_off, (const char*)pEntry->mpFlagName, len);
			pEntry->mpFlagName = names_off;
			names_off += len;
		}
		for (int i = 0; i < mEnums.GetSize(); i++) {
			EnumEntry* pEntry = mEnums.mpStorage + i;
			int len = strlen((const char*)pEntry->mpEnumName) + 1;
			memcpy(names_temp + names_off, (const char*)pEntry->mpEnumName, len);
			pEntry->mpEnumName = names_off;
			names_off += len;
		}

		/*write header*/
		pOut->SerializeWrite(MAGIC, 8);
		pOut->Serialize((char*)&mVersion, 4);
		pOut->Serialize((char*)&names_off, 8);
		pOut->Serialize((char*)&mDBFlags, 4);
		pOut->Serialize((char*)&mClasses.mSize, 4);
		pOut->Serialize((char*)&mMembers.mSize, 4);
		pOut->Serialize((char*)&mFlags.mSize, 4);
		pOut->Serialize((char*)&mEnums.mSize, 4);

		/*write everything*/
		pOut->Serialize((char*)mClasses.mpStorage, sizeof(ClassEntry) * mClasses.mSize);
		pOut->Serialize((char*)mMembers.mpStorage, sizeof(MemberEntry) * mMembers.mSize);
		pOut->Serialize((char*)mEnums.mpStorage, sizeof(EnumEntry) * mEnums.mSize);
		pOut->Serialize((char*)mFlags.mpStorage, sizeof(FlagEntry) * mFlags.mSize);
		pOut->Serialize((char*)names_temp, names_off);

		/*cleanup*/
		mClasses.Clear(0);
		mMembers.Clear(0);
		mFlags.Clear(0);
		mEnums.Clear(0);
		mDBFlags = 0;
		delete[] names_temp;
	}

	//does not own pIn, you delete it af ter
	inline bool SerializeIn(DataStream* pIn) {

		/*read header*/
		char temp[8];
		pIn->Serialize(temp, 8);
		if (memcmp(temp, MAGIC, 8)) {
			TelltaleToolLib_RaiseError("could not load vers db: not a ttversdb", ErrorSeverity::ERR);
			return false;
		}
		int nClasses, nMembers, nFlags, nEnums;
		u64 namesSize{};
		pIn->Serialize((char*)&mVersion, 4);
		if (!(mVersion == 2 || mVersion == 3)) {
			TelltaleToolLib_RaiseError("could not load vers db: unknown version or old", ErrorSeverity::ERR);
			return false;
		}
		pIn->Serialize((char*)&namesSize, 8);
		pIn->Serialize((char*)&mDBFlags, 4);
		pIn->Serialize((char*)&nClasses, 4);
		pIn->Serialize((char*)&nMembers, 4);
		pIn->Serialize((char*)&nFlags, 4);
		pIn->Serialize((char*)&nEnums, 4);
		if (nClasses > 10000 || nMembers > 50000 || nFlags > 20000 || nEnums > 20000 || namesSize > 0x800000) {
			TelltaleToolLib_RaiseError("could not load vers db : corrupt", ErrorSeverity::ERR);
			return false;
		}

		/*do alloc*/
		if (mpNamingBuffer)
			delete[] mpNamingBuffer;
		mpNamingBuffer = new char[namesSize];
		mClasses.Clear(0);
		mMembers.Clear(0);
		mFlags.Clear(0);
		mEnums.Clear(0);
		mClasses.ReserveAndResize(nClasses);
		mMembers.ReserveAndResize(nMembers);
		mFlags.ReserveAndResize(nFlags);
		mEnums.ReserveAndResize(nEnums);

		/*read classes*/
		pIn->Serialize((char*)mClasses.mpStorage, nClasses * sizeof(ClassEntry));
		for (int i = 0; i < nClasses; i++) {
			ClassEntry* pClass = mClasses.mpStorage + i;
			pClass->mpTypeInfoName = ((u64)pClass->mpTypeInfoName + (u64)mpNamingBuffer);
			if (pClass->mpExt)
				pClass->mpExt = ((u64)pClass->mpExt + (u64)mpNamingBuffer);
		}

		/*read members*/
		pIn->Serialize((char*)mMembers.mpStorage, nMembers * sizeof(MemberEntry));
		for (int i = 0; i < nMembers; i++) {
			MemberEntry* pMem = mMembers.mpStorage + i;
			pMem->mpName = ((u64)pMem->mpName + (u64)mpNamingBuffer);
		}

		/*read enums*/
		pIn->Serialize((char*)mEnums.mpStorage, nEnums * sizeof(EnumEntry));
		for (int i = 0; i < nEnums; i++) {
			EnumEntry* pEnum = mEnums.mpStorage + i;
			pEnum->mpEnumName = ((u64)pEnum->mpEnumName + (u64)mpNamingBuffer);
		}

		/*read flags*/
		pIn->Serialize((char*)mFlags.mpStorage, nFlags * sizeof(FlagEntry));
		for (int i = 0; i < nFlags; i++) {
			FlagEntry* pFlag = mFlags.mpStorage + i;
			pFlag->mpFlagName = ((u64)pFlag->mpFlagName + (u64)mpNamingBuffer);
		}

		/*read names*/
		pIn->Serialize(mpNamingBuffer, namesSize);

		if (mVersion == 2) {
			mVersion = 3;
			//fix member desc fix
			for (int i = 0; i < mMembers.mSize; i++) {
				MemberEntry* pMem = mMembers.mpStorage + i;
				pMem->mpMemberDesc <<= 3;
				pMem->mpMemberDesc /= sizeof(ClassEntry);
			}
		}

		return true;
	}

};

#endif