// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _EVENTSTORE
#define _EVENTSTORE

#include "../Meta.hpp"
#include "DCArray.h"
#include "Set.h"
#include "HandleObjectInfo.h"
#include "List.h"
#include "ObjectSpaceIntrinsics.h"

struct EventLoggerEvent;

//.SAVE FILES
struct SaveGame
{

	struct AgentInfo {
		Symbol mAgentName;
		Symbol mSceneName;
		Vector3 mPosition;
		__declspec(align(8)) Quaternion mQuaternion;
		bool mbAttached;
		Symbol mAttachedToAgent;
		Symbol mAttachedToNode;
	};

	String mLuaDoFile;
	DCArray<SaveGame::AgentInfo> mAgentInfo;
	Set<Symbol, std::less<Symbol> > mRuntimePropNames;
	Set<Symbol, std::less<Symbol> > mEnabledDynamicSets;
};

//.EPAGE FILES
struct EventStoragePage {
	long mVersion;
	u64 mSessionID;
	String mFlushedNameOnDisk;
	List<EventLoggerEvent*> mEvents;

	EventStoragePage(int pagesize) {}
	EventStoragePage() = default;
	~EventStoragePage();

	static METAOP_FUNC_DEF(SerializeAsync);

};

enum EventDataType {
	eEventData_Symbol = 0,
	eEventData_Int = 1,
	eEventData_Double = 2
};

struct EventData {

	EventDataType mDataType;//as char
	union {
		Symbol mDataSymbolMemory;
		long double mDataDouble;
		u64 mDataInt;
	};
	char mSeverity;

	EventData& operator=(const EventData& in) {
		mDataType = in.mDataType;
		mSeverity = in.mSeverity;
		mDataInt = in.mDataInt;
		return *this;
	}

};

struct EventLoggerEvent {

	long mEventID;
	long mMaxSeverity;

	struct TypeHeader {
		Symbol mType;
		List<EventData> mData;

		u64 ComputeCRC(u64 crc) {
			u64 crc64 = mType.GetCRC();
			u64 result = CRC64(crc, reinterpret_cast<const char*>(&crc64), 8u);
			for (int i = 0; i < mData.GetSize(); i++) {
				EventData d = mData[i];
				result = CRC64(result, reinterpret_cast<const char*>(&d.mDataType), 4u);
				result = CRC64(result, reinterpret_cast<const char*>(&d.mSeverity), 1u);
				result = CRC64(result, reinterpret_cast<const char*>(&d.mDataInt), 8u);
			}
			return result;
		}

	};

	DCArray<TypeHeader> mEventData;

	void Serialize(EventStoragePage* storage, MetaStream* stream) {
		Meta::MetaOperation_SerializeAsync(this,
			GetMetaClassDescription<EventLoggerEvent>(), NULL, stream);
		stream->BeginBlock();
		u64 datacount = mEventData.GetSize();
		stream->serialize_uint64(&datacount);
		bool isread = stream->mMode == MetaStreamMode::eMetaStream_Read;
		if (datacount > 0xFFFFFFFF) {
			TelltaleToolLib_RaiseError("Event logger event data corrupt", ErrorSeverity::ERR);
			stream->SkipToEndOfCurrentBlock();
			return;
		}
		u32 count = 0;
		char** datatypesbuffer = NULL;
		if (isread)
			datatypesbuffer = new char* [datacount];
		int* sizesbuf = isread ? new int[datacount] : NULL;
		mEventData.Resize((int)datacount);
		for (u64 i = 0; i < datacount; i++) {
			if (isread) {
				TypeHeader hdr{};
				stream->serialize_Symbol(&hdr.mType);
				stream->serialize_uint32(&count);
				datatypesbuffer[(int)i] = (char*)malloc(count);
				for (int x = 0; x < count; x++) {
					stream->serialize_int8(datatypesbuffer[(int)i] + x);
				}
				sizesbuf[(int)i] = count;
				mEventData.AddElement(0, NULL, &hdr);
			}
			else {
				TypeHeader hdr = mEventData[i];
				count = hdr.mData.GetSize();
				stream->serialize_Symbol(&hdr.mType);
				stream->serialize_uint32(&count);
				for (int x = 0; x < count; x++) {
					EventData data = hdr.mData[x];
					stream->serialize_int8(reinterpret_cast<char*>(&data.mDataType));
				}
			}
		}

		for (u64 i = 0; i < datacount; i++) {
			TypeHeader* hdr = mEventData.mpStorage + i;
			stream->BeginObject(&hdr->mType, true);
			count = sizesbuf[(int)i];
			for (int x = 0; x < count; x++) {
				EventData data{};
				data.mDataType =(EventDataType)(datatypesbuffer[(int)i][x]);
				stream->BeginObject("Event Data", false);
				if (data.mDataType == EventDataType::eEventData_Symbol) {
					stream->serialize_Symbol(&data.mDataSymbolMemory);
				}
				else if (data.mDataType == EventDataType::eEventData_Double) {
					stream->serialize_double(&data.mDataDouble);
				}
				else {
					stream->serialize_uint64(&data.mDataInt);
				}
				stream->EndObject("Event Data");
				stream->BeginObject("Event Severity", false);
				stream->serialize_int8(&data.mSeverity);
				stream->EndObject("Event Severity");
				hdr->mData.push_back(data);
			}
			stream->EndObject(&hdr->mType);
		}
		if (isread) {
			for (int i = 0; i < datacount; i++)
				free(datatypesbuffer[i]);
			delete[] datatypesbuffer;
			delete[] sizesbuf;
		}
		stream->EndBlock();
	}

};

//.ESTORE FILES
struct EventStorage {

	struct PageEntry {
		Handle<EventStoragePage> mhPage;
		u32 mMaxEventID;
	};

	long mVersion;
	u64 mSessionID;
	DCArray<PageEntry> mPages;
	String mName;
	u32 mLastEventID;
	u32 mEventStoragePageSize;
	EventStoragePage* mpCurrentPage;

	EventStorage() {
		mpCurrentPage = NULL;
	}

	~EventStorage() {
		if (mpCurrentPage)
			delete mpCurrentPage;
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription,
			pContextDescription, pUserData);
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		EventStorage* estore = static_cast<EventStorage*>(pObj);
		if (r == eMetaOp_Succeed) {
			bool hasPage = estore->mpCurrentPage != NULL;
			meta->serialize_bool(&hasPage);
			if (hasPage) {
				if (estore->mpCurrentPage == NULL)
					estore->mpCurrentPage = new EventStoragePage(0x2000);
				r=PerformMetaSerializeAsync<EventStoragePage>
					(meta, estore->mpCurrentPage);
			}
		}
		return r;
	}

};

#endif