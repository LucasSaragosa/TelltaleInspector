// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _LANGDB
#define _LANGDB

#include "../Meta.hpp"
#include "Map.h"
#include <string.h>
#include "Animation.h"
#include "SoundData.h"
#include "HandleObjectInfo.h"

//.LANGRES FILES
struct LanguageResource {

	u32 mId;
	Symbol mResName;
	String mPrefix;
	String mText;
	Handle<Animation> mhAnimation;
	Handle<SoundData> mhVoiceData;
	bool mShared;
	bool mAllowSharing;
	bool mbNoAnim;
	void* mpLipsyncAnimation;
	Flags mFlags;

	//Flags mRuntimeFlags;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj,
			pObjDescription, pContextDescription, pUserData);
		char _Buffer[25];
		sprintf(_Buffer, "%d.langres", ((LanguageResource*)pObj)->mId);
		((LanguageResource*)pObj)->mResName = _Buffer;
		//((LanguageResource*)pObj)->mRuntimeFlags |= 1u;
		return r;
	}

};

constexpr const char* kDefaultLangDB = "english.langdb";

//.LANGDB FILES
struct LanguageDatabase {
	String mName;
	//bool mbInitialized;
	Map<int, LanguageResource> mLanguageResources;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		return Meta::MetaOperation_SerializeAsync(pObj,
			pObjDescription, pContextDescription, pUserData);
	}

};

#endif