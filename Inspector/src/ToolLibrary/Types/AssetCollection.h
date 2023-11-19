// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _ACOL
#define _ACOL

#include "../Meta.hpp"
#include "DCArray.h"

//.ACOL FILES
struct AssetCollection {
	DCArray<String> mIncludeMasks;
	DCArray<String> mExcludeMasks;
	String mPreFilter;
	//mChangedCallacks;
};

#endif