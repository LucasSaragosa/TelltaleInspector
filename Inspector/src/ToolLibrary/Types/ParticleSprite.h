// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _SPRITE
#define _SPRITE

#include "../Meta.hpp"
#include "SArray.h"
#include "T3Texture.h"
#include "DCArray.h"
#include "HandleObjectInfo.h"
#include "ObjectSpaceIntrinsics.h"

enum T3BlendMode {
	eBlendModeNormal = 0,
	eBlendModeAlpha = 1,
	eBlendModeAlphaAlphaTest = 2,
	eBlendModeAlphaTest = 3,
	eBlendModeInvAlphaTest = 4,
	eBlendModeAdd = 5,
	eBlendModeMultiply = 6,
	eBlendModeInvMultiply = 7,
	eBlendModeAlphaAdd = 8,
	eBlendModeAlphaSubtract = 9,
	eBlendModeAlphaInvAlphaTest = 10,
	eBlendModeAddAlphaTest = 11,
	eBlendModeAddInvAlphaTest = 12,
	eBlendModeMultiplyAlphaTest = 13,
	eBlendModeMultiplyInvAlphaTest = 14,
	eBlendMode_Count = 15,
	eBlendModeDefault = -1
};

struct BlendMode : EnumBase {
	T3BlendMode mVal;
};

//.SPRITE FILES
struct ParticleSprite {

	struct Animation {
		Symbol mName;
		long mStartFrame;
		long mFrameCount;
	};

	String mName;
	SArray<Handle<T3Texture>, 1> mhTexture;
	long mTextureX;
	long mTextureY;
	Vector2 mSpriteSize;
	BlendMode mBlendMode;
	DCArray<ParticleSprite::Animation> mAnimations;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		return eMetaOp_Fail;
		//return Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
	}

};

#endif