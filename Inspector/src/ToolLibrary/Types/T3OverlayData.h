// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _OVERLAY
#define _OVERLAY

#include "../Meta.hpp"
#include "HandleObjectInfo.h"
#include "ObjectSpaceIntrinsics.h"
#include "ParticleSprite.h"
#include "Dialog.h"
#include "T3Texture.h"
#include "DCArray.h"
#include "Font.h"
#include "Chore.h"

struct T3OverlaySpriteParams {
	Handle<ParticleSprite> mhSprite;
	Vector2 mInitialPosition;
	Vector2 mSize;
	Symbol mAnimation;
	float mAnimationSpeed;
	Flags mFlags;//flag value of 1 means its an animation loop
};

struct T3OverlayTextParams {
	Handle<Font> mhFont;
	Handle<Dlg> mhDlg;
	Symbol mDlgNodeName;
	String mText;
	Vector2 mInitialPosition;
};

struct T3OverlayObjectData_Text {
	Symbol mName;
	T3OverlayTextParams mParams;
};

struct T3OverlayObjectData_Sprite {
	Symbol mName;
	T3OverlaySpriteParams mParams;
};

struct T3OverlayParams {
	Handle<T3Texture> mhBackgroundTexture;
	Handle<Chore> mhChore;
	float mMinDisplayTime;
	float mFadeTime;
};

//.OVERLAY FILES
struct T3OverlayData {

	String mName;
	DCArray<T3OverlayObjectData_Sprite> mSpriteObjects;
	DCArray<T3OverlayObjectData_Text> mTextObjects;
	T3OverlayParams mParams;

};

#endif