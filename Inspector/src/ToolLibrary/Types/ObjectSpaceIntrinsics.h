// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _OBJ_INTRINS
#define _OBJ_INTRINS

#include "../Meta.hpp"

template<typename T> struct Curve {//not serialized
	T cf0, cf1, cf2, cf3;
};

struct Vector2 {
	union {
		union {
			float x;
			float u;
		};
		union {
			float y;
			float v;
		};
		float array[2];
	};
};

struct Vector4 {
	float x;
	float y;
	float z;
	float w;

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pCtx, void* pUserData) {
		static_cast<MetaStream*>(pUserData)->Key("X");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector4*>(pObj)->x);
		static_cast<MetaStream*>(pUserData)->Key("Y");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector4*>(pObj)->y);
		static_cast<MetaStream*>(pUserData)->Key("Z");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector4*>(pObj)->z);
		static_cast<MetaStream*>(pUserData)->Key("W");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector4*>(pObj)->w);
		return eMetaOp_Succeed;
	}

};

struct Vector3 {
	float x;
	float y;
	float z;

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pCtx, void* pUserData) {
		static_cast<MetaStream*>(pUserData)->Key("X");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector3*>(pObj)->x);
		static_cast<MetaStream*>(pUserData)->Key("Y");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector3*>(pObj)->y);
		static_cast<MetaStream*>(pUserData)->Key("Z");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Vector3*>(pObj)->z);
		return eMetaOp_Succeed;
	}

};

struct BoundingBox {
	Vector3 mMin, mMax;
};

struct Sphere {
	Vector3 mCenter;
	float mRadius;
};

struct Quaternion {
	float x;
	float y;
	float z;
	float w;

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pCtx, void* pUserData) {
		static_cast<MetaStream*>(pUserData)->Key("X");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Quaternion*>(pObj)->x);
		static_cast<MetaStream*>(pUserData)->Key("Y");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Quaternion*>(pObj)->y);
		static_cast<MetaStream*>(pUserData)->Key("Z");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Quaternion*>(pObj)->z);
		static_cast<MetaStream*>(pUserData)->Key("W");
		static_cast<MetaStream*>(pUserData)->serialize_float(&static_cast<Quaternion*>(pObj)->w);
		return eMetaOp_Succeed;
	}

};

struct Transform {
	Quaternion mRot;
	Vector3 mTrans;
	float mPaddingAlign;//to align it since vec3 is 12 bytes and rot is 16, 16 + 16 = 32, a nice one unlike 28
};

struct LocationInfo {
	String mAttachedAgent;
	Symbol mAttachedNode;
	Transform mInitialLocalTransform;
};

struct ResourceGroupInfo {
	Color mColor;
	long mPriority;
};

struct Polar
{
	float mR;
	float mTheta;
	float mPhi;
};

struct  __declspec(align(8)) Selectable
{
	BoundingBox mBoundingBox;
	int mSelectionOrder;
	bool mbGameSelectable;
};

namespace NavCam {
	enum Mode : i32 {
		eNone = 1,
		eLookAt = 2,
		eOrbit = 3,
		eAnimation_Track = 4,
		eAnimation_Time = 5,
		eAnimation_Pos_ProceduralLookAt = 6,
		eScenePosition = 7,
		eDynamicConversationCamera = 8,
	};

	struct EnumMode : EnumBase {
		Mode mVal;
	};
}

#endif