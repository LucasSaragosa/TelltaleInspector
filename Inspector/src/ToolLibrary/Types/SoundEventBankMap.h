// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _SOUNDEVENT_BANK_MAP
#define _SOUNDEVENT_BANK_MAP

#include "../Meta.hpp"
#include "DCArray.h"
#include "Map.h"

//.SOUNDEVENTBANKMAP FILES
struct SoundEventBankMap {
	Map<String, DCArray<String>> mBankMap;
	bool mbLoadAllBanksGlobally;
};

struct SoundBankWaveMapEntry {
	float fLengthSeconds;
	String strFileName;
};

//.BANKWAVEMAP FILES
struct SoundBankWaveMap {

	Map<Symbol, SoundBankWaveMapEntry> mWaveMap;

};

#endif