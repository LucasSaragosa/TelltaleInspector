// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _LANDB
#define _LANDB

#include "../Meta.hpp"
#include "Map.h"
#include "DCArray.h"
#include "LocalizationRegistry.h"
#include "HandleObjectInfo.h"
#include "Animation.h"
#include "SoundData.h"

struct ProjectDatabaseIDPair {
	long mProjectID, mDBID;//db id
};

struct LocalizeInfo {
	Flags mFlags;
};

struct LanguageResProxy {

	struct Pending {
		String mPrefix, mText;
		Flags mFlags;
	};

	u32 mID;
};

struct LanguageResLocal {
	String mPrefix;
	String mText;
	LocalizeInfo mLocalInfo;
};

//.LANG FILES
struct LanguageRes {
	Symbol mResName;
	unsigned long mID;
	unsigned long mIDAlias;
	Handle<Animation> mhAnimation;
	Handle<SoundData> mhVoiceData;
	DCArray<LanguageResLocal> mLocalData;
	float mLengthOverride;
	LanguageResLocal mResolvedLocalData;
	RecordingUtils::EnumRecordingStatus mRecordingStatus;
	Flags mFlags;//1 = synthesized, 2 = ignore text lipsync, 4 = hide subtitles, 8 = is voiced, 16 = is silent, 32 = no animation
};

//.LANDB FILES
struct LanguageDB : public UID::Owner, public UID::Generator {

	Map<unsigned int, LanguageRes> mLanguageResources;
	LocalizationRegistry mRegistry;
	Flags mFlags;
	long mProjectID;
	DCArray<ProjectDatabaseIDPair> mExpandedIDRanges;

	struct _DebugMapping {
		String mInternalLanguageRes;
		String mAnimation;
		String mVoiceSoundData;
		_DebugMapping() = default;
	};

	_DebugMapping** mDebugFileMappings;
	String** mDebugRegistrySymbolValueMappings;
	String** mDebugRegistrySymbolValueMappingsReverse;
	int mNumDebugRegistrySymbolMappings, mNumDebugRegistrySymbolMappingsReverse;
	int mNumDebugFileMappings;

	void _del_internal() {
		if (mDebugFileMappings) {
			for (int i = 0; i < mNumDebugFileMappings; i++) {
				delete mDebugFileMappings[i];
			}
			free(mDebugFileMappings);
			mDebugFileMappings = NULL;
			mNumDebugFileMappings = 0;
		}
		if (mDebugRegistrySymbolValueMappings) {
			for (int i = 0; i < mNumDebugRegistrySymbolMappings; i++) {
				delete mDebugRegistrySymbolValueMappings[i];
			}
			free(mDebugRegistrySymbolValueMappings);
			mDebugRegistrySymbolValueMappings = NULL;
			mNumDebugRegistrySymbolMappings = 0;
		}
		if (mDebugRegistrySymbolValueMappingsReverse) {
			for (int i = 0; i < mNumDebugRegistrySymbolMappingsReverse; i++) {
				delete mDebugRegistrySymbolValueMappingsReverse[i];
			}
			free(mDebugRegistrySymbolValueMappingsReverse);
			mDebugRegistrySymbolValueMappingsReverse = NULL;
			mNumDebugRegistrySymbolMappingsReverse = 0;
		}
	}

	~LanguageDB() {
		_del_internal();
	}

	LanguageDB() {
		mDebugFileMappings = NULL;
		mNumDebugFileMappings = 0;
		mNumDebugRegistrySymbolMappings = 0;
		mNumDebugRegistrySymbolMappingsReverse = 0;
		mDebugRegistrySymbolValueMappings = NULL;
		mDebugRegistrySymbolValueMappingsReverse = NULL;
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		//looks empty but originally there is a debug section in the langdb, which of course in is not shipped and is only used internally
		//the debug section contains list of string triplets in the format {lang file, anm file, sound data file}
		//the size of the list is the 
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		LanguageDB* db = static_cast<LanguageDB*>(pObj);
		if (meta->mStreamVersion >= 5 && meta->mSection[2].mCompressedSize > 0) {
			meta->_SetSection(MetaStream::SectionType::eSection_Debug);
			if (meta->mMode == MetaStreamMode::eMetaStream_Read) {
				int num = db->mLanguageResources.GetSize();
				db->_del_internal();
				db->mNumDebugFileMappings = num;
				db->mDebugFileMappings = (_DebugMapping**)malloc(sizeof(_DebugMapping*) * num);
				for (int i = 0; i < num; i++) {
					_DebugMapping* m = new _DebugMapping;
					meta->serialize_String(&m->mInternalLanguageRes);
					meta->serialize_String(&m->mAnimation);
					meta->serialize_String(&m->mVoiceSoundData);
					db->mDebugFileMappings[i] = m;
				}
				num = db->mRegistry.mFlagIndexMap.GetSize();
				db->mDebugRegistrySymbolValueMappings = (String**)malloc(sizeof(String*) * num);
				db->mNumDebugRegistrySymbolMappings = num;
				for (int i = 0; i < num; i++) {
					String* str = new String();
					meta->serialize_String(str);
					db->mDebugRegistrySymbolValueMappings[i] = str;
				}
				num = db->mRegistry.mFlagIndexMapReverse.GetSize();
				db->mDebugRegistrySymbolValueMappingsReverse = (String**)malloc(sizeof(String*) * num);
				db->mNumDebugRegistrySymbolMappingsReverse = num;
				for (int i = 0; i < num; i++) {
					String* str = new String();
					meta->serialize_String(str);
					db->mDebugRegistrySymbolValueMappingsReverse[i] = str;
				}
			}
			else {
				if (db->mDebugFileMappings) {
					for (int i = 0; i < db->mNumDebugFileMappings; i++) {
						meta->serialize_String(&db->mDebugFileMappings[i]->mInternalLanguageRes);
						meta->serialize_String(&db->mDebugFileMappings[i]->mAnimation);
						meta->serialize_String(&db->mDebugFileMappings[i]->mVoiceSoundData);
					}
				}
				if (db->mDebugRegistrySymbolValueMappings) {
					for (int i = 0; i < db->mNumDebugRegistrySymbolMappings; i++)
						meta->serialize_String(db->mDebugRegistrySymbolValueMappings[i]);
				}
				if (db->mDebugRegistrySymbolValueMappingsReverse) {
					for (int i = 0; i < db->mNumDebugRegistrySymbolMappingsReverse; i++)
						meta->serialize_String(db->mDebugRegistrySymbolValueMappingsReverse[i]);
				}
			}
			meta->_SetSection(MetaStream::SectionType::eSection_Default);
		}
		return r;
	}

};

#endif