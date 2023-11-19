// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _AUDIOBUS
#define _AUDIOBUS

#include "Map.h"
#include "HandleObjectInfo.h"
#include "../Meta.hpp"
#include "AssetCollection.h"

struct SoundBusSnapshot {

	//.BUSSNAPSHOT FILES
	struct Snapshot {
		float mfFadeTime;
		Map<Symbol, float> mBusVolumes;
	};

	//.SNAPSHOTSUITE FILES
	struct SnapshotSuite {
		Map<Symbol, Handle<Snapshot>> mSnapshotMap;
	};

};

struct SoundBusSystem {

	struct BusDescription {
		Map<String, BusDescription, std::less<String>> children;
		float fVolumedB;
		float fReverbWetLeveldB;
		float fLowPassFrequencyCutoff;
		float fHighPassFrequencyCutoff;
		bool bEnableLowPass;
		bool bEnableHighPass;
		AssetCollection autoAssignPatternCollection;
	};

	//.AUDIOBUS FILES
	struct BusHolder {
		BusDescription masterBus;
		Map<Symbol, Symbol, std::less<Symbol>> assetMap;
	};

};

#endif