// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _BLENDGRAPH
#define _BLENDGRAPH

#include "AnimOrChore.h"
#include "../Meta.hpp"
#include "HandleObjectInfo.h"
#include "DCArray.h"
#include "KeyframedValue.h"

struct FlagsT3LightEnvGroupSet {
	Flags mFlags;
};

enum T3LightEnvGroup
{
	eLightEnvGroup_None = 0xFFFFFFFE,
	eLightEnvGroup_Default = 0xFFFFFFFF,
	eLightEnvGroup_Group0 = 0x0,
	eLightEnvGroup_Group1 = 0x1,
	eLightEnvGroup_Group2 = 0x2,
	eLightEnvGroup_Group3 = 0x3,
	eLightEnvGroup_Group4 = 0x4,
	eLightEnvGroup_Group5 = 0x5,
	eLightEnvGroup_Group6 = 0x6,
	eLightEnvGroup_Group7 = 0x7,
	eLightEnvGroup_CountWithNoAmbient = 0x8,
	eLightEnvGroup_Group8_Unused = 0x8,
	eLightEnvGroup_Group9_Unused = 0x9,
	eLightEnvGroup_Group10_Unused = 0xA,
	eLightEnvGroup_Group11_Unused = 0xB,
	eLightEnvGroup_Group12_Unused = 0xC,
	eLightEnvGroup_Group13_Unused = 0xD,
	eLightEnvGroup_Group14_Unused = 0xE,
	eLightEnvGroup_Group15_Unused = 0xF,
	eLightEnvGroup_AmbientGroup0 = 0x10,
	eLightEnvGroup_AmbientGroup1 = 0x11,
	eLightEnvGroup_AmbientGroup2 = 0x12,
	eLightEnvGroup_AmbientGroup3 = 0x13,
	eLightEnvGroup_Count = 0x14,
};

struct EnumT3LightEnvGroup : EnumBase
{
	T3LightEnvGroup mVal;
};

enum RenderMaskWrite
{
	eRenderMaskWrite_None = 0x1,
	eRenderMaskWrite_Set = 0x2,
	eRenderMaskWrite_Clear = 0x3,
};

enum RenderMaskTest
{
	eRenderMaskTest_None = 0x1,
	eRenderMaskTest_Set = 0x2,
	eRenderMaskTest_Clear = 0x3,
};

struct EnumRenderMaskWrite : EnumBase
{
	RenderMaskWrite mVal;
};

struct EnumRenderMaskTest : EnumBase
{
	RenderMaskTest mVal;
};

enum T3LightEnvType
{
	eLightEnvType_Point = 0x0,
	eLightEnvType_Spot = 0x1,
	eLightEnvType_DirectionalKey = 0x2,
	eLightEnvType_Ambient = 0x3,
	eLightEnvType_DirectionalAmbient = 0x4,
	eLightEnvType_Count = 0x5,
};

struct EnumT3LightEnvType {
	T3LightEnvType mVal;
};

struct CorrespondencePoint {
	Flags mFlags;
	Flags mEaseOutStartFlags;
	Flags mEaseOutEndFlags;
	Flags mEaseInStartFlags;
	Flags mEaseInEndFlags;
	Flags mSteeringFlags;
	Flags mTransitionFlags;
	float mfTime;
	String mComment;
};

struct BlendEntry {
	Vector3 mParameterValues;
	AnimOrChore mAnimOrChore;
	DCArray<CorrespondencePoint> mCorrespondencePoints;
	float mfAnimOrChoreLength;
	String mComment;
};

//.BGH FILES
struct BlendGraph {

	enum BlendGraphType {
		eBlendgraph_Looping = 0,
		eBlendgraph_NonLooping = 1
	};

	struct EnumBlendGraphType : EnumBase {
		BlendGraphType mVal;
	};

	BlendGraph() {
		mpGeometry = NULL;
	}

	DCArray<Symbol> mParameters, mFrozenParameterNames;//frozen params removed in WD3 only, fuck knows why
	DCArray<float> mDampeningConstants;
	DCArray<BlendEntry> mEntries;
	EnumBlendGraphType mBlendGraphType;
	bool mDampen;
	float mfTimeScale;
	String mComment;
	void* mpGeometry;
	Handle<Chore> mhBlendGraphAuxiliaryChore;
	long mbInvertParameters;
	long mVersion;
	long mNumGeometryDimensions;
	long mNumDimensions;
	DCArray<int> mParameterOrder;

	//static void ConvertEaseInFlagsToV6(int v5Flags, CorrespondencePoint* cp) {}

	//static void ConvertEaseOutFlagsToV6(int v5Flags, CorrespondencePoint* cp) {}

	constexpr static float kDefaultConstants[4] = { 6.0f,2.0f,2.0f,0.0f };

	INLINE void ClearGeometry() {
		if (mpGeometry) {
			if (mNumGeometryDimensions == 1) {
				delete (KeyframedValue<int>*)mpGeometry;
			}
			else if (mNumGeometryDimensions == 2) {
				delete (Map<float, KeyframedValue<int>>*)mpGeometry;
			}
			else if (mNumGeometryDimensions == 3) {
				delete (Map<float, Map<float, KeyframedValue<int>>>*)mpGeometry;
			}
			else
				delete mpGeometry;
		}
	}

	~BlendGraph() {
		ClearGeometry();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(BlendGraph, bg);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			long ver = bg->mVersion;
			if (ver < 4)
				bg->mNumGeometryDimensions = bg->mNumDimensions;
			if (ver < 5 && bg->mBlendGraphType.mVal == BlendGraphType::eBlendgraph_NonLooping)
				bg->mFrozenParameterNames = bg->mParameters;
			bool hasgeom = bg->mpGeometry != NULL;
			meta->serialize_bool(&hasgeom);
			if (meta->IsRead())
				bg->ClearGeometry();
			if (hasgeom) {
				MetaClassDescription* geom = NULL;
				if (bg->mNumGeometryDimensions == 1) {
					if (meta->IsRead())
						bg->mpGeometry = new KeyframedValue<int>;
					geom = ::GetMetaClassDescription<KeyframedValue<int>>();
				}
				else if (bg->mNumGeometryDimensions == 2) {
					if (meta->IsRead())
						bg->mpGeometry = new Map<float, KeyframedValue<int>>;
					geom = ::GetMetaClassDescription<Map<float, KeyframedValue<int>>>();
				}
				else {
					if (bg->mNumGeometryDimensions != 3)
						return eMetaOp_Fail;
					if (meta->IsRead())
						bg->mpGeometry = new Map<float, Map<float, KeyframedValue<int>>>;
					geom = ::GetMetaClassDescription<Map<float, Map<float, KeyframedValue<int>>>>();
				}
				r = PerformMetaSerializeFull(meta, bg->mpGeometry, geom);
				if (r != eMetaOp_Succeed)
					return r;
			}

		}
		return r;
	}

};

//.BGM FILES
struct BlendGraphManager {
	float mfTransitionTime;
	AnimOrChore mIdleAnimOrChore;
	bool mbUseAnimationMoverData;
	Handle<BlendGraph> mhFreewalkStartGraph, mhFreewalkLoopGraph, mhFreewalkStopGraph, mhTurnToFaceGraph,//turn to face in newer
		mhChoredMovementStartGraph, mhChoredMovementLoopGraph, mhChoredMovementStopGraph;//older games this line
	long mVersion;
	bool mbUseAlgorithmicHeadTurn;
	float mfMaxManualSteeringVelocityInDegrees, mfMinManualSteeringVelocityInDegrees;
	float mfMaxLeanInPercentVelocity, mfMinLeanInPercentVelocity;
	float mfWalkSpeedScale;
	float mfRunSpeedScale;
};

#endif