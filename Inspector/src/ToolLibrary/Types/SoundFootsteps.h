// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _SOUND_FOOTSTEPS
#define _SOUND_FOOTSTEPS

#include "../Meta.hpp"
#include "SoundData.h"
#include "Map.h"
#include "DCArray.h"
#include "SoundAmbience.h"

namespace SoundFootsteps {

	enum Material {
		Default = 0x1, 
		Carpet = 0x2,
		Concrete = 0x3, 
		Concrete_Wet = 0x4, 
		Dirt = 0x5, 
		Grass = 0x6, 
		Grass_Tall = 0x7,
		Gravel = 0x8,
		Leaves = 0x9,
		Linoleum = 0xA,
		Metal_Thick = 0xB,
		Metal_Thin = 0xC,
		Mud = 0xD,
		Puddle = 0xE, 
		Sand = 0xF, 
		Snow = 0x10,
		Tile_Hard = 0x11,
		Wood = 0x12,
	};

	struct EnumMaterial : EnumBase {
		Material mVal;

		inline bool operator<(const EnumMaterial& rhs) const {
			return mVal < rhs.mVal;
		}

	};

};

struct FootSteps {

	struct FootstepBank {
		DCArray<Handle<SoundData>> mSounds;
		Map<SoundFootsteps::EnumMaterial, DCArray<Handle<SoundData>>> mMaterialFootsteps;
	};

};


struct Footsteps2 {

	struct FootstepBank {

		SoundEventName<SoundEventNameBase::NT_DEFAULT> mEventName;
		Map<SoundFootsteps::EnumMaterial, SoundEventName<SoundEventNameBase::NT_DEFAULT>> mMaterialMap;

	};

};

#endif