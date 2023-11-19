// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "EventStorage.h"
#include "Procedural.h"
#include "Chore.h"

MetaOpResult ChoreResource::MetaOperation_SerializeAsync(void* pObj,
	MetaClassDescription* pObjDescription,
	MetaMemberDescription* pContextDescription,
	void* pUserData) {
	MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
	MetaStream* meta = static_cast<MetaStream*>(pUserData);
	ChoreResource* choreres = static_cast<ChoreResource*>(pObj);
	if (r == eMetaOp_Succeed) {
		if (meta->IsWrite() && choreres->mbEmbedded) {
			MetaClassDescription* desc = choreres->mhObjectDesc;
			meta->serialize_uint64(&desc->mHash);
			meta->serialize_uint64(&desc->mHash);
			r = PerformMetaSerializeFull(meta, choreres->mhObjectEmbedded, choreres->mhObjectDesc);
			if (r != eMetaOp_Succeed)
				return r;
		}
		u64 hash = 0;
		MetaClassDescription* embedt = NULL;
		if (!meta->IsRead())
			return r;//done writing
		if (!choreres->mbEmbedded)
			return r;//no embed, we are done here
		meta->serialize_uint64(&hash);
		meta->serialize_uint64(&hash);
		if (choreres->mVersion) {
			embedt = TelltaleToolLib_FindMetaClassDescription_ByHash(hash);
		}
		else {
			String s;
			meta->serialize_String(&s);
			embedt = TelltaleToolLib_FindMetaClassDescription_ByHash(CRC64_CaseInsensitive(0, s.c_str()));
		}
		if (!embedt) {
			TelltaleToolLib_RaiseError("Could not locate meta class description from CRC in embedded chore data", ErrorSeverity::ERR);
			return eMetaOp_Invalid;
		}
		void* obj = embedt->New();
		if (!obj) {
			return eMetaOp_OutOfMemory;
		}
		if (choreres->mVersion >= 2 || embedt != GetMetaClassDescription<Procedural_LookAt>()) {
			PerformMetaSerializeFull(meta, obj, embedt);
		}
		else {
			Animation temp;
			MetaClassDescription* anmdesc = GetMetaClassDescription<Animation>();
			PerformMetaSerializeFull(meta, &temp, anmdesc);
			meta->mRuntimeFlags.mFlags |= 1u;
		}
		choreres->SetResourceHandle(obj, embedt, true);
	}
	return r;
}

EventStoragePage::~EventStoragePage() {
	for (int i = 0; i < this->mEvents.GetSize(); i++)
		delete mEvents[i];
}

MetaOpResult EventStoragePage::MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDescription, 
	MetaMemberDescription* pContextDescription, void* pUserData) {
	MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription,
		pContextDescription, pUserData);
	MetaStream* meta = static_cast<MetaStream*>(pUserData);
	EventStoragePage* epage = static_cast<EventStoragePage*>(pObj);
	bool iswrite = meta->mMode == MetaStreamMode::eMetaStream_Write;
	if (r == eMetaOp_Succeed) {
		u32 numevents = epage->mEvents.size();
		meta->serialize_uint32(&numevents);
		meta->BeginObject("Events", true);
		if (iswrite) {
			std::list<EventLoggerEvent*>::iterator it;
			for (it = epage->mEvents.begin(); it != epage->mEvents.end(); it++) {
				meta->BeginObject("Event Page",false);
				(*it)->Serialize(epage, meta);
				meta->EndObject("Event Page");
			}
		}
		else {
			for (int i = 0; i < numevents; i++) {
				EventLoggerEvent* event = new EventLoggerEvent();
				event->Serialize(epage, meta);
				epage->mEvents.push_back(event);
			}
		}
		meta->EndObject("Events");
	}
	return r;
}