#ifndef LOCOMOTION_DB_H
#define LOCOMOTION_DB_H

#include "../Meta.hpp"
#include "Map.h"

//.LDB FILES
struct LocomotionDB {

	struct AnimationInfo {
		unsigned __int64 mu64TimeStamp;
		String mzName;
		enum {
			Category_Idle = 0,
			Category_Start = 1,
			Category_Move = 2,
			Category_Stop = 3,
			Category_Turn = 4,
			Category_TurnAndStart = 5,
			Category_MoveAndTurn = 6,
			Category_StopAndTurn = 7,
			Category_Unknown = 8,
			Category_Number = 9,
			Category_None = -1,
		} meCategory;

		float mfDuration, mfStartSpeed, mfEndSpeed, mfTurnAngle, mfMoveDistance;
		bool mbMoveStart, mbMoveStop, mbMove, mbTurnLeft, mbTurnRight, mbTurn;

	};

	Map<String, AnimationInfo> mAnimInfoList;

};

#endif