// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _PROCEDURAL
#define _PROCEDURAL

#include "../Meta.hpp"
#include "AnimOrChore.h"

//.LOOK FILES
struct Procedural_LookAt {

	struct EnumLookAtComputeStage : EnumBase {
		long mVal;//0 = idle look at, 1 = dialog chore look at, 2 = final look at
	};

	struct Constraint {
		float mMaxLeftRight;
		float mMinLeftRight;
		float mMaxUp;
		float mMinUp;
		float mLeftRightFixedOffsset;
		float mUpDownFixedOffsset;
	};

	String mHostNode;
	String mTargetAgent;
	String mTargetNode;
	Vector3 mTargetOffset;
	bool mbUsePrivateNode;
	AnimOrChore mhXAxisChore;
	AnimOrChore mhYAxisChore;
	bool mbRotateHostNode;
	float mLastLRAngle;
	float mLastUDAngle;
	float mLastLRWeight;
	float mLastUDWeight;
	EnumLookAtComputeStage mLookAtComputeStage;

};

#endif