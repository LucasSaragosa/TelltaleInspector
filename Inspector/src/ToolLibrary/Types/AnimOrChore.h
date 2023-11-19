// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _ANMORCHORE
#define _ANMORCHORE

#include "HandleObjectInfo.h"
#include "Animation.h"
#include "Chore.h"

struct AnimOrChore {
	Handle<Animation> mhAnim;
	Handle<Chore> mhChore;
};

#endif