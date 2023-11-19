// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _HOI
#define _HOI

#include "../TelltaleToolLibrary.h"
#include "../Meta.hpp"
#include <algorithm>
#include "../HashDB/HashDB.h"

struct HandleObjectInfo {
	Symbol mObjectName;
	Flags mFlags;
};

struct HandleBase {

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDescription, MetaMemberDescription* pCtx,
		void* pUserData) {
		MetaStream* stream = static_cast<MetaStream*>(pUserData);
		HandleBase* handle = static_cast<HandleBase*>(pObj);
		if (stream->mMode == MetaStreamMode::eMetaStream_Read) {
			if (stream->mStreamVersion >= 5) {
				stream->serialize_Symbol(&handle->mHandleObjectInfo.mObjectName);
			}
			else {
				String str;
				stream->serialize_String(&str);
				handle->mHandleObjectInfo.mObjectName = str.c_str();
			}
		}
		else {
			stream->serialize_Symbol(&handle->mHandleObjectInfo.mObjectName);
		}
		return eMetaOp_Succeed;
	}

	//HandleObjectInfo* mpHandleObjectInfo; //normally a pointer in the engine
	//maybe to do with a buffer of them? but anyway
	HandleObjectInfo mHandleObjectInfo;

	INLINE Symbol GetObjectName() const {
		return mHandleObjectInfo.mObjectName;
	}

	INLINE void SetObjectName(Symbol pName) {
		mHandleObjectInfo.mObjectName = pName;
	}

	INLINE void SetObjectName(const char* pNameString) {
		SetObjectName(CRC64_CaseInsensitive(0,pNameString));
	}

};

//by lib
struct HandleUncached : HandleBase {
private:
	MetaClassDescription* mpDataDescription;
	void* mpObj;
public:

	HandleUncached() {
		mpObj = NULL;
		mpDataDescription = NULL;
	}

	HandleUncached& operator=(HandleUncached&& rhs) noexcept {
		mHandleObjectInfo.mObjectName = rhs.mHandleObjectInfo.mObjectName;
		Delete();
		mpDataDescription = rhs.mpDataDescription;
		mpObj = rhs.mpObj;
		rhs.mpDataDescription = NULL;
		rhs.mpObj = NULL;
		return *this;
	}

	HandleUncached& operator=(const HandleUncached& rhs) noexcept {
		mHandleObjectInfo.mObjectName = rhs.mHandleObjectInfo.mObjectName;
		Delete();
		mpDataDescription = rhs.mpDataDescription;
		if (rhs.mpObj) {
			mpObj = mpDataDescription->New();
			mpDataDescription->CopyConstruct(mpObj, rhs.mpObj);
		}
		return *this;
	}

	HandleUncached(const HandleUncached& rhs) noexcept {
		*this = rhs;
	}

	HandleUncached(HandleUncached&& rhs) noexcept {
		*this = std::move(rhs);
	}

	INLINE MetaClassDescription* GetTypeDesc() const {
		return mpDataDescription;
	}

	INLINE void* GetHandleObjectPointer() const {
		return mpObj;
	}

	template<typename T>
	T* GetValue() const {
		MetaClassDescription* type = ::GetMetaClassDescription<T>();
		if (type != mpDataDescription)
			return NULL;
		return (T*)GetHandleObjectPointer();
	}

	INLINE void Delete() {
		if (mpObj)
			mpDataDescription->Delete(mpObj);
		mpObj = NULL;
	}


	//takes ownership completely of param
	INLINE void SetObjectUnsafe(void* val) {
		if (mpObj)
			mpDataDescription->Delete(mpObj);
		mpObj = val;
	}

private:

	INLINE bool SetObject(MetaClassDescription* pDesc, void* pVal) {
		Delete();
		if (pDesc != mpDataDescription)
			mpDataDescription = pDesc;
		mpObj = pVal;
		return true;
	}

	static INLINE MetaClassDescription* GetStringDesc() {
		static MetaClassDescription* strDesc = NULL;
		if (!strDesc)
			strDesc = ::TelltaleToolLib_FindMetaClassDescription("String", true);
		if (!strDesc) {
			TelltaleToolLib_RaiseError("Meta not initialized: cannot find string description", ErrorSeverity::ERR);
			return NULL;
		}
		return strDesc;
	}

public:

	template<>
	String* GetValue<String>() const {
		MetaClassDescription* desc = GetStringDesc();
		if (!desc)
			return NULL;
		if (mpDataDescription != desc)
			return NULL;
		return (String*)GetHandleObjectPointer();
	}

	//TAKES OWNERSHIP
	template<typename T>
	bool SetValue(T* v) {
		MetaClassDescription* type = ::GetMetaClassDescription<T>();
		if (!type)
			return false;
		return SetObject(type, v);
	}

	//TAKES OWNERSHIP
	template<>
	bool SetValue<String>(String* v) {
		MetaClassDescription* strDesc = GetStringDesc();
		if (!strDesc)
			return false;
		return SetObject(strDesc, v);
	}

	~HandleUncached() {
		Delete();
	}

	explicit HandleUncached(MetaClassDescription* pDesc, bool bCreate) : HandleUncached() {
		mpDataDescription = pDesc;
		if (bCreate)
			mpObj = pDesc->New();
	}

	explicit HandleUncached(MetaClassDescription* pDesc) : HandleUncached(pDesc, false) {}


	explicit HandleUncached(MetaClassDescription* pDesc, void* pObj) {
		mpDataDescription = pDesc;
		mpObj = NULL;
		if (mpDataDescription)
			mpObj = pObj;
	}

	void CreateDefaultObject() {
		Delete();
		if (mpDataDescription)
			mpObj = mpDataDescription->New();
	}

};

template<typename T> struct Handle : public HandleBase {

	Handle<T>& operator=(const Symbol& o) {
		mHandleObjectInfo.mObjectName = o;
		return *this;
	}

	String GetObjectName() {
		static const String _NF = "<NotFound>";
		static const String _E = "<Empty>";
		if (mHandleObjectInfo.mObjectName.GetCRC() == 0)return _E;
		HashDatabase* db = TelltaleToolLib_GetGlobalHashDatabase();
		MetaClassDescription* desc = GetMetaClassDescription(typeid(T).name());
		if (!desc || !desc->mpExt)return _NF;
		if (!db)return _NF;
		HashDatabase::Page* page = NULL;
		String pagen = "Files_";
		pagen += desc->mpExt;
		pagen += '_';
		String gameid = sBlowfishKeys[sSetKeyIndex].game_id;
		std::transform(gameid.begin(), gameid.end(), gameid.begin(), ::toupper);
		pagen += gameid;
		page = db->FindPage(pagen.c_str());
		if (!page)return _NF;
		String ret;
		db->FindEntry(page, mHandleObjectInfo.mObjectName.GetCRC(), &ret);
		if (ret.size() == 0)
			ret = _NF;
		return ret;
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDescription, MetaMemberDescription* pCtx,
		void* pUserData) {
		return HandleBase::MetaOperation_SerializeAsync(pObj, pObjDescription, pCtx, pUserData);//originally this overloaded one
		//would have have extra code here specific to loading type T but we dont need it so just do the norm.
	}

};

template<typename T> struct HandleLock : public Handle<T> {

};

template<typename T> INLINE Handle<T> CreateHandle(const char* fileName) {
	Handle<T> ret;
	ret.mHandleObjectInfo.mObjectName.SetCRC(CRC64_CaseInsensitive(0, fileName));
	return ret;
}

typedef struct SoundEventData {
	Symbol mObjectName, mDisplayName;
	float mLength;
	Ptr<HandleObjectInfo> mpHOI;

	SoundEventData() = default;

} SoundEventSnapshotData;

#endif
