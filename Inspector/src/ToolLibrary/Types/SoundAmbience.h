// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _SOUND_AMBIENCE_H
#define _SOUND_AMBIENCE_H

#include "../Meta.hpp"
#include "DCArray.h"
#include "TRange.h"
#include "Map.h"

struct SoundEventNameBase {

	Symbol mEventGuid, mEventDisplayName;

	enum NameType {
		NT_DEFAULT = 0,
		NT_SNAPSHOT = 1,
		NT_DIALOG = 2
	};

};

template<SoundEventNameBase::NameType>
struct SoundEventName : public SoundEventNameBase {};

namespace SoundAmbience {

	struct EventContext : SoundEventNameBase {
		SoundEventName<SoundEventNameBase::NT_DEFAULT> mEventname;
		float mPlayChance;
		TRange<float> mSilentTimeRange, mPlayTimeRange, mVolumeRangedB, mVolumeFadeTimeRange;
	};
	
	//.AMBIENCE FILES
	struct AmbienceDefinition {

		DCArray<EventContext> mEvents;

	};

};

#endif