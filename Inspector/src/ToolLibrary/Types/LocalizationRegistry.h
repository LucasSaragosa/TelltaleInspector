// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _LOCREG
#define _LOCREG

#include "../Meta.hpp"
#include "Map.h"
#include "ToolProps.h"

//.LOCREG FILES
struct LocalizationRegistry {

	Map<Symbol, int, Symbol::CompareCRC> mFlagIndexMap;
	Map<int, Symbol, std::less<int>> mFlagIndexMapReverse;
	ToolProps mToolProps;

};

struct Localization {

	struct Language {

		String mName;
		String mDisplayText;
		String mAudioResourceSetName;
		Map<String, String, std::less<String> > mPlatformToSyncFSLocation;
		Map<String, Map<String, String, std::less<String> >, std::less<String> > mPlatformToSubgroupToAudioSyncFSLocation;
		Map<String, String, std::less<String> > mSubgroupToResourceSetName;
		Flags mFlags;
		unsigned int mIndex;
		unsigned int mVersionNumber;
		unsigned int mAudioVersionNumber;

	};

};

#endif