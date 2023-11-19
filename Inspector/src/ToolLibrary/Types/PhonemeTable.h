// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.


#ifndef _PTABLE
#define _PTABLE

#include "../Meta.hpp"
#include "Map.h"
#include "AnimOrChore.h"

struct PhonemeKey {
	Symbol mPhoneme;
	float mFadeInTime, mHoldTime, mFadeOutTime, mTargetContribution;
	
	static METAOP_FUNC_IMPL__(SerializeAsync) {
		return Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
	}

};

//.PTABLE FILES
struct PhonemeTable {
	
	struct PhonemeEntry {	
		AnimOrChore mAnimation;
		float mContributionScalar, mTimeScalar;

		PhonemeEntry() {
			mContributionScalar = 1.0f;
			mTimeScalar = 1.0f;
		}

	};

	String mName;
	float mContributionScaler;
	Map<Symbol, PhonemeEntry, Symbol::CompareCRC> mAnimations;

	PhonemeTable() {
		mContributionScaler = 1.0f;
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		return Meta::MetaOperation_SerializeAsync
		(pObj, pObjDescription, pContextDescription, pUserData);
	}

};

#endif