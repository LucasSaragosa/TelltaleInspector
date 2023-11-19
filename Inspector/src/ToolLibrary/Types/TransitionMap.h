// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _TMAP
#define _TMAP

#include "Map.h"
#include "KeyframedValue.h"
#include "../Meta.hpp"

struct TransitionRemapper {
	KeyframedValue<float> mRemapKeys;
};

//.TMAP FILES
struct TransitionMap {

	struct TransitionMapInfo {
		//not serialized
		int mPriority;
		TransitionRemapper mRemapper;
	};

	Map<Symbol, TransitionMapInfo, Symbol::CompareCRC> mTransitionRemappers;

};

#endif