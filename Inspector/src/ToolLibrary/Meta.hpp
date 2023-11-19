// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _META
#define _META

#pragma warning (disable : 4018 4244 4267 4554 6387 4099 4305 4715 6387 4319 4227)

#include "TelltaleToolLibrary.h"
#include <string>
#include <math.h>
#include <typeinfo>
#include <stack>
#include <vector>
#include <stack>
#include "Types/TRange.h"
#include "HashManager.h"
#include "HashDB/HashDB.h"

//set to false to disallow the debug section of meta stream files to be loaded. default true

#ifndef METASTREAM_ENABLE_DEBUG
#define METASTREAM_ENABLE_DEBUG true
#endif

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) || \
    defined(__ARMEB__) || \
    defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
    defined(__LITTLE_ENDIAN__) || \
    defined(__ARMEL__) || \
    defined(__THUMBEL__) || \
    defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 0
#elif defined(_MSC_VER) 
#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 0
#else
#error "Could not detect endian"
#endif

#define METAOP_FUNC_DEF(_FuncName) MetaOpResult MetaOperation_##_FuncName(void *,MetaClassDescription*,MetaMemberDescription*,void*);
#define METAOP_FUNC_IMPL(_FuncName) MetaOpResult Meta::MetaOperation_##_FuncName(void *pObj,MetaClassDescription* pObjDescription,\
	MetaMemberDescription *pContextDescription,void *pUserData)

#define METAOP_FUNC_IMPL_(NS,_FuncName) MetaOpResult NS::MetaOperation_##_FuncName(void *pObj,MetaClassDescription* pObjDescription,\
	MetaMemberDescription *pContextDescription,void *pUserData)

#define METAOP_FUNC_IMPL__(_FuncName) MetaOpResult MetaOperation_##_FuncName(void *pObj,MetaClassDescription* pObjDescription,\
	MetaMemberDescription *pContextDescription,void *pUserData)

#define CAST_METAOP(Ty, _name) MetaStream* meta = static_cast<MetaStream*>(pUserData); Ty* _name = static_cast<Ty*>(pObj);

struct EnumBase {};

struct DebugString : String {

	static METAOP_FUNC_DEF(SerializeAsync);

};

struct Guide {
	float m_Time;
	long m_Bitfield;
	long m_AutoActRole;
};

struct ParticleLODKey
{
	float mCountScale;
	float mStrideScale;
	float mDivisionScale;
	float mLifeScale;
};

struct T3LightCinematicRigLOD
{
	Flags mFlags;
};

enum LightCellBlendMode
{
	eLightCellBlendMode_Normal = 0x0,
	eLightCellBlendMode_Dodge = 0x1,
	eLightCellBlendMode_Multiply = 0x2,
	eLightCellBlendMode_Screen = 0x3,
	eLightCellBlendMode_Overlay = 0x4,
};

enum T3NPRSpecularType
{
	T3NPRSpecularType_None = 0x0,
	T3NPRSpecularType_Isotropic = 0x1,
	T3NPRSpecularType_Anisotropic = 0x2,
};

enum ParticlePropDriver
{
	ePartPropDriver_EmitterSpeed = 0x1,
	ePartPropDriver_DistanceToTarget = 0x2,
	ePartPropDriver_BurstTime = 0x3,
	ePartPropDriver_CameraDot = 0x4,
	ePartPropDriver_KeyControl01 = 0x5,
	ePartPropDriver_KeyControl02 = 0x6,
	ePartPropDriver_KeyControl03 = 0x7,
	ePartPropDriver_KeyControl04 = 0x8,
	ePartPropDriver_DistanceToCamera = 0x9,
};

enum T3DetailShadingType
{
	T3DetailShadingType_No_Detail_Map = 0x0,
	T3DetailShadingType_Old_Toon = 0x1,
	T3DetailShadingType_Sharp_Detail = 0x2,
	T3DetailShadingType_Packed_Detail_And_Tiled_Packed_Detail = 0x3,
	T3DetailShadingType_Packed_Detail = 0x4,
	T3DetailShadingType_Single_Channel_Detail = 0x5,
	T3DetailShadingType_Animated_Detail = 0x6,
};

enum ParticlePropModifier
{
	ePartPropModifier_Constraint_Length = 0x0,
	ePartPropModifier_Effect_Scale = 0x1,
	ePartPropModifier_Geometry_Turbulence = 0x2,
	ePartPropModifier_Global_Alpha = 0x3,
	ePartPropModifier_Global_Acceleration = 0x4,
	ePartPropModifier_Max_Particles = 0x5,
	ePartPropModifier_PP_Scale = 0x6,
	ePartPropModifier_PP_Lifespan = 0x7,
	ePartPropModifier_PP_Rotation = 0x8,
	ePartPropModifier_PP_RotationSpeed = 0x9,
	ePartPropModifier_PP_Speed = 0xA,
	ePartPropModifier_PP_Intensity = 0xB,
	ePartPropModifier_Time_Scale = 0xC,
	ePartPropModifier_Sprite_Animation_Rate = 0xD,
	ePartPropModifier_Sprite_Animation_Cycles = 0xE,
	ePartPropModifier_Spawn_Angle = 0xF,
	ePartPropModifier_Spawn_Volume_Sweep = 0x10,
	ePartPropModifier_Spawn_Volume_Sweep_Offset = 0x11,
	ePartPropModifier_Target_Render_Lerp = 0x12,
	ePartPropModifier_Velocity_Turbulence_Force = 0x13,
	ePartPropModifier_Velocity_Turbulence_Speed = 0x14,
	ePartPropModifier_Velocity_Timescale_Modifier = 0x15,
	ePartPropModifier_KeyControl01 = 0x16,
	ePartPropModifier_KeyControl02 = 0x17,
	ePartPropModifier_KeyControl03 = 0x18,
	ePartPropModifier_KeyControl04 = 0x19,
	ePartPropModifier_PP_Alpha = 0x1A,
	ePartPropModifier_Geometry_Scale = 0x1B,
	ePartPropModifier_Enable = 0x1C,
	ePartPropModifier_Count = 0x1D,
};

struct  EnumT3DetailShadingType : EnumBase
{
	T3DetailShadingType mVal;
};

/* 25792 */
struct  EnumT3NPRSpecularType : EnumBase
{
	T3NPRSpecularType mVal;
};

struct  EnumParticlePropModifier : EnumBase
{
	ParticlePropModifier mVal;
};

struct  EnumParticlePropDriver : EnumBase
{
	ParticlePropDriver mVal;
};

enum RenderTextureResolution
{
	eRenderTextureResolution_Small = 0x1,
	eRenderTextureResolution_Medium = 0x2,
	eRenderTextureResolution_Default = 0x3,
	eRenderTextureResolution_Ultra = 0x4,
};

struct EnumRenderTextureResolution : EnumBase {
	RenderTextureResolution mVal;
};

struct FilterArea {
	String mText;
};

struct MovieCaptureInfo {

	enum CompressorType {
		IV50 = 5,
		MSVC = 4,
		IV32 = 3,
		CVID = 2,
		Uncompressed = 1
	};

	struct EnumCompressorType : EnumBase {
		CompressorType mVal;
	};

	int mFPS;
	EnumCompressorType mCType;

};

enum TextOrientationType
{
	eTextOrientation_Screen = 0x0,
	eTextOrientation_WorldZ = 0x1,
	eTextOrientation_WorldXYZ = 0x2,
};

enum DepthOfFieldType
{
	eDepthOfFieldType_Default = 0x1,
	eDepthOfFieldType_Brush = 0x2,
};

enum T3LightEnvBakeOnStatic
{
	eLightEnvBakeOnStatic_Default = 0x0,
	eLightEnvBakeOnStatic_AlwaysAllow = 0x1,
	eLightEnvBakeOnStatic_NeverAllow = 0x2,
};

enum T3LightEnvLODBehavior
{
	eLightEnvLOD_Disable = 0x0,
	eLightEnvLOD_BakeOnly = 0x1,
};

enum T3LightEnvEnlightenBakeBehavior
{
	eLightEnvEnlightenBake_Auto = 0x0,
	eLightEnvEnlightenBake_Enable = 0x1,
	eLightEnvEnlightenBake_Disable = 0x2,
};

enum BokehOcclusionType
{
	eBokehOcclusionDisabled = 0x0,
	eBokehOcclusionZTestAndScaleOccluded = 0x1,
	eBokehOcclusionScaleOccluded = 0x2,
	eBokehOcclusionZTest = 0x3,
};

struct  EnumBokehOcclusionType : EnumBase
{

	EnumBokehOcclusionType(int val = 0) {
		mVal = (BokehOcclusionType)val;
	}

	BokehOcclusionType mVal;
};

enum GlowQualityLevel
{
	eGlowQualityLevelOld = 0x0,
	eGlowQualityLevelLow = 0x1,
	eGlowQualityLevelMedium = 0x2,
	eGlowQualityLevelHigh = 0x3,
};

struct  EnumGlowQualityLevel : EnumBase
{
	GlowQualityLevel mVal;
};

enum BokehQualityLevel
{
	eBokehQualityLevelDisabled = 0x0,
	eBokehQualityLevelLow = 0x1,
	eBokehQualityLevelMedium = 0x2,
	eBokehQualityLevelHigh = 0x3,
};

struct  EnumBokehQualityLevel : EnumBase
{
	BokehQualityLevel mVal;
};

enum DOFQualityLevel
{
	eDofQualityLevelDisabled = 0x0,
	eDOFQualityLevelLow = 0x1,
	eDOFQualityLevelMedium = 0x2,
	eDOFQualityLevelHigh = 0x3,
};

struct  EnumDOFQualityLevel : EnumBase
{
	DOFQualityLevel mVal;
};

enum RenderTAAJitterType
{
	eRenderTAAJitter_None = 0x1,
	eRenderTAAJitter_Uniform2x = 0x2,
	eRenderTAAJitter_Hammersley4x = 0x3,
	eRenderTAAJitter_Hammersley8x = 0x4,
};

struct  EnumRenderTAAJitterType : EnumBase
{
	RenderTAAJitterType mVal;
};

enum RenderAntialiasType
{
	eRenderAntialias_None = 0x0,
	eRenderAntialias_FXAA = 0x1,
	eRenderAntialias_SMAA = 0x2,
	eRenderAntialias_MSAA_2x = 0x3,
	eRenderAntialias_MSAA_4x = 0x4,
	eRenderAntialias_MSAA_8x = 0x5,
	eRenderAntialias_TAA_MSAA_2x = 0x6,
	eRenderAntialias_TAA_MSAA_4x = 0x7,
	eRenderAntialias_TAA_MSAA_8x = 0x8,
	eRenderAntialias_TAA = 0x9,
};

struct  EnumRenderAntialiasType : EnumBase
{
	RenderAntialiasType mVal;
};

enum HBAOResolution
{
	eHBAOResolutionFull = 0x0,
	eHBAOResolutionHalf = 0x1,
	eHBAOResolutionQuarter = 0x2,
};

struct  EnumHBAOResolution : EnumBase
{
	HBAOResolution mVal;
};

enum HBAOQualityLevel
{
	eHBAOQualityLevelLowest = 0x0,
	eHBAOQualityLevelLow = 0x1,
	eHBAOQualityLevelMedium = 0x2,
	eHBAOQualityLevelHigh = 0x3,
	eHBAOQualityLevelHighest = 0x4,
};
struct  EnumHBAOQualityLevel : EnumBase
{
	HBAOQualityLevel mVal;
};

enum HBAOPerPixelNormals
{
	eHBAOPerPixelNormalsGBuffer = 0x0,
	eHBAOPerPixelNormalsReconstructed = 0x1,
};

struct  EnumHBAOPerPixelNormals : EnumBase
{
	HBAOPerPixelNormals mVal;
};

enum HBAODeinterleaving
{
	eHBAODeinterleaving_Disabled = 0x0,
	eHBAODeinterleaving_2x = 0x1,
	eHBAODeinterleaving_4x = 0x2,
};

struct  EnumHBAODeinterleaving : EnumBase
{
	HBAODeinterleaving mVal;
};

enum HBAOPreset
{
	eHBAOPresetFromTool = 0x0,
	eHBAOPresetXBone = 0x1,
	eHBAOPresetPS4 = 0x2,
	eHBAOPresetDisabled = 0x3,
	eHBAOPresetLow = 0x4,
	eHBAOPresetMedium = 0x5,
	eHBAOPresetHigh = 0x6,
	eHBAOPresetUltra = 0x7,
};

struct  EnumHBAOPreset : EnumBase
{
	HBAOPreset mVal;
};

enum HBAOBlurQuality
{
	eHBAOBlurQualityNone = 0x0,
	eHBAOBlurQualityNarrow = 0x1,
	eHBAOBlurQualityMedium = 0x2,
	eHBAOBlurQualityWide = 0x3,
	eHBAOBlurQualityExtraWide = 0x4,
};


struct  EnumHBAOBlurQuality : EnumBase
{
	HBAOBlurQuality mVal;
};


struct  EnumT3LightEnvBakeOnStatic : EnumBase
{
	T3LightEnvBakeOnStatic mVal;
};

struct  EnumDepthOfFieldType : EnumBase
{
	DepthOfFieldType mVal;
};

struct  EnumT3LightEnvLODBehavior : EnumBase
{
	T3LightEnvLODBehavior mVal;
};

struct  EnumT3LightEnvEnlightenBakeBehavior : EnumBase
{
	T3LightEnvEnlightenBakeBehavior mVal;
};

struct  EnumTextOrientationType : EnumBase
{
	TextOrientationType mVal;
};


struct  EnumLightCellBlendMode : EnumBase
{
	LightCellBlendMode mVal;
};

enum T3MaterialNormalSpaceType
{
	eMaterialNormalSpace_Tangent = 0x0,
	eMaterialNormalSpace_World = 0x1,
	eMaterialNormalSpace_Count = 0x2,
};

enum T3MaterialLODFullyRough
{
	eMaterialFullyRough_Never = 0x0,
	eMaterialFullyRough_OnLowestQuality = 0x1,
	eMaterialFullyRough_OnLowQuality = 0x2,
	eMaterialFullyRough_Always = 0x3,
};

struct  EnumT3MaterialNormalSpaceType : EnumBase
{
	T3MaterialNormalSpaceType mVal;
};

struct  EnumT3MaterialLODFullyRough : EnumBase
{
	T3MaterialLODFullyRough mVal;
};

/* 3514 */
struct T3LightEnvInternalData
{

	struct QualityEntry
	{
		unsigned int mShadowLayer;
		Flags mFlags;
	};

	T3LightEnvInternalData::QualityEntry mEntryForQuality[4];
	int mStationaryLightIndex;
};

/* 974 */
enum EmitterColorType
{
	eEmitterColorType_Constant = 0x1,
	eEmitterColorType_Random_Lerp = 0x2,
	eEmitterColorType_Random_Discrete = 0x3,
	eEmitterColorType_BurstLife_Lerp = 0x4,
	eEmitterColorType_Index_Lerp = 0x5,
	eEmitterColorType_KeyControl01 = 0x6,
};

/* 975 */
enum EmitterSpriteAnimationType
{
	eEmitterSpriteAnimationType_Linear = 0x1,
	eEmitterSpriteAnimationType_Random = 0x2,
};

/* 976 */
enum EmitterSpriteAnimationSelection
{
	eEmitterSpriteAnimationSelection_Random = 0x1,
	eEmitterSpriteAnimationSelection_LinearLoop = 0x2,
	eEmitterSpriteAnimationSelection_LinearStretch = 0x3,
	eEmitterSpriteAnimationSelection_KeyControl01 = 0x4,
};

enum ParticleSortMode
{
	eParticleSortMode_None = 0x1,
	eParticleSortMode_ByDistance = 0x2,
	eParticleSortMode_YoungestFirst = 0x3,
	eParticleSortMode_OldestFirst = 0x4,
};

enum EmitterSpawnShape
{
	eEmitterSpawn_Box = 0x1,
	eEmitterSpawn_Sphere = 0x2,
	eEmitterSpawn_Cylinder = 0x3,
	eEmitterSpawn_ToTarget = 0x4,
	eEmitterSpawn_Particle = 0x5,
	eEmitterSpawn_ParticleInterpolate = 0x6,
	eEmitterSpawn_Bones = 0x7,
	eEmitterSpawn_BoneBoxes = 0x8,
};

struct PointOfInterestBlocking {};

/* 993 */
enum EmitterBoneSelection
{
	eEmitterBoneSelection_All = 0x1,
	eEmitterBoneSelection_Children = 0x2,
};

enum EmitterParticleCountType
{
	eEmitterParticleCountType_Count = 0x1,
	eEmitterParticleCountType_SpawnVolParticleCount = 0x2,
};

enum EmitterConstraintType
{
	eEmitterConstraintType_None = 0x1,
	eEmitterConstraintType_0Point = 0x2,
	eEmitterConstraintType_1Point = 0x3,
	eEmitterConstraintType_2Point = 0x4,
};

struct  EnumParticleSortMode : EnumBase
{
	ParticleSortMode mVal;
};

struct  EnumEmitterSpawnShape : EnumBase
{
	EmitterSpawnShape mVal;
};

struct  EnumEmitterBoneSelection : EnumBase
{
	EmitterBoneSelection mVal;
};

/* 13215 */
struct  EnumEmitterColorType : EnumBase
{
	EmitterColorType mVal;
};

/* 13217 */
struct  EnumEmitterSpriteAnimationType : EnumBase
{
	EmitterSpriteAnimationType mVal;
};

/* 13219 */
struct  EnumEmitterSpriteAnimationSelection : EnumBase
{
	EmitterSpriteAnimationSelection mVal;
};

struct EnumEmitterConstraintType : EnumBase
{
	EmitterConstraintType mVal;
};

struct EnumEmitterParticleCountType : EnumBase
{
	EmitterParticleCountType mVal;
};

struct MetaClassDescription;
struct MetaMemberDescription;
class Symbol; 

enum ConstraintReferenceFrame : int{
	World = 0,
	Root = 1,
	Parent = 2,
};

struct AnimationConstraint {
	enum LockMode : int{
		eTrafoMode = 0,/*pos and rot*/
		ePosMode = 1,/*pos*/
		eOriMode = 2,/*rot*/
	};
	enum Node : int {
		eWorld = 0x0,
		eRoot = 0x1,
		eSpine1 = 0x2,
		eSpine2 = 0x3,
		eSpine3 = 0x4,
		eSpine4 = 0x5,
		eShoulder_L = 0x6,
		eArm_L = 0x7,
		eElbow_L = 0x8,
		eWrist_L = 0x9,
		eMiddleF1_L = 0xA,
		eMiddleF2_L = 0xB,
		eMiddleF3_L = 0xC,
		eIndexF1_L = 0xD,
		eIndexF2_L = 0xE,
		eIndexF3_L = 0xF,
		eThumb1_L = 0x10,
		eThumb2_L = 0x11,
		eThumb3_L = 0x12,
		eRingF1_L = 0x13,
		eRingF2_L = 0x14,
		eRingF3_L = 0x15,
		ePinkyF1_L = 0x16,
		ePinkyF2_L = 0x17,
		ePinkyF3_L = 0x18,
		eShoulder_R = 0x19,
		eArm_R = 0x1A,
		eElbow_R = 0x1B,
		eWrist_R = 0x1C,
		ePinkyF1_R = 0x1D,
		ePinkyF2_R = 0x1E,
		ePinkyF3_R = 0x1F,
		eMiddleF1_R = 0x20,
		eMiddleF2_R = 0x21,
		eMiddleF3_R = 0x22,
		eIndexF1_R = 0x23,
		eIndexF2_R = 0x24,
		eIndexF3_R = 0x25,
		eThumb1_R = 0x26,
		eThumb2_R = 0x27,
		eThumb3_R = 0x28,
		eRingF1_R = 0x29,
		eRingF2_R = 0x2A,
		eRingF3_R = 0x2B,
		eNeck = 0x2C,
		eNeck2 = 0x2D,
		eHead = 0x2E,
		eEye_L = 0x2F,
		eEye_R = 0x30,
		ePelvis = 0x31,
		eLeg_L = 0x32,
		eKnee_L = 0x33,
		eAnkle_L = 0x34,
		eLeg_R = 0x35,
		eKnee_R = 0x36,
		eAnkle_R = 0x37,
		eNumNodes = 0x38,
	};
};

enum T3LightEnvMobility : int{
	eLightEnvMobility_Static = 0,
	eLightEnvMobility_Stationary = 1,
	eLightEnvMobility_Moveable = 2,
};

enum T3LightEnvShadowType : int {
	eLightEnvShadowType_None = 0,
	eLightEnvShadowType_PerLight = 2,
	eLightEnvShadowType_Modulated = 3,
};

enum T3LightEnvShadowQuality : int {
	eLightEnvShadowQuality_Low = 0,
	eLightEnvShadowQuality_Medium = 1,
	eLightEnvShadowQuality_High = 2,
};

enum HBAOParticipationType : int {
	eHBAOParticipationTypeAuto = 0,
	eHBAOParticipationTypeForceOn = 1,
	eHBAOParticipationTypeForceOff = 2,
};

struct EnumT3LightEnvMobility : EnumBase {
	T3LightEnvMobility mVal;
};

struct EnumT3LightEnvShadowType : EnumBase {
	T3LightEnvShadowType mVal;
};

struct EnumT3LightEnvShadowQuality : EnumBase {
	T3LightEnvShadowQuality mVal;
};

struct EnumHBAOParticipationType : EnumBase {
	HBAOParticipationType mVal;
};

enum T3LightEnvLODFlags {
	eLODHigh = 0x10000,
	eLODMedium = 0x20000,
	eLODLow = 0x40000,
};

struct T3LightEnvLOD {
	Flags mFlags;
};

struct RootKey {
	bool mTranslationConstraint__Enabled;
	float mTranslationConstraint_MaxVelocity;
	float mTranslationConstraint_MaxAcceleration;
	bool mRotationConstraint__Enabled;
	float mRotationConstraint_MaxBendAngularVelocity;
	float mRotationConstraint_MaxBendAngularAcceleration;
	float mRotationConstraint_MaxTwistAngularVelocity;
	float mRotationConstraint_MaxTwistAngularAcceleration;
};

struct PivotJointKey {
	bool mBoneLengthConstraint__Enabled;
	bool mBoneLengthConstraint_RestRelative;
	float mBoneLengthConstraint_NodeMobility;
	float mBoneLengthConstraint_ParentMobility;
	bool mAngleConstraint__Enabled;
	float mAngleConstraint_MinHorizontalBendAngle;
	float mAngleConstraint_MaxHorizontalBendAngle;
	float mAngleConstraint_MinVerticalBendAngle;
	float mAngleConstraint_MaxVerticalBendAngle;
	bool mTranslationConstraint__Enabled;
	ConstraintReferenceFrame mTranslationConstraint_ReferenceFrame;
	float mTranslationConstraint_MaxVelocity;
	float mTranslationConstraint_MaxAcceleration;
	bool mRotationConstraint__Enabled;
	ConstraintReferenceFrame mRotationConstraint_ReferenceFrame;
	float mRotationConstraint_MaxBendAngularVelocity;
	float mRotationConstraint_MaxBendAngularAcceleration;
};

struct BallJointKey {
	bool mBoneLengthConstraint__Enabled;
	bool mBoneLengthConstraint_RestRelative;
	float mBoneLengthConstraint_NodeMobility;
	float mBoneLengthConstraint_ParentMobility;
	bool mAngleConstraint__Enabled;
	float mAngleConstraint_MaxBendAngle;
	bool mTranslationConstraint__Enabled;
	ConstraintReferenceFrame mTranslationConstraint_ReferenceFrame;
	float mTranslationConstraint_MaxVelocity;
	float mTranslationConstraint_MaxAcceleration;
	bool mRotationConstraint__Enabled;
	ConstraintReferenceFrame mRotationConstraint_ReferenceFrame;
	float mRotationConstraint_MaxBendAngularVelocity;
	float mRotationConstraint_MaxBendAngularAcceleration;
};

struct HingeJointKey {
	bool mBoneLengthConstraint__Enabled;
	bool mBoneLengthConstraint_RestRelative;
	float mBoneLengthConstraint_NodeMobility;
	float mBoneLengthConstraint_ParentMobility;
	bool mAngleConstraint__Enabled;
	float mAngleConstraint_MinBendAngle;
	float mAngleConstraint_MaxBendAngle;
	bool mTranslationConstraint__Enabled;
	ConstraintReferenceFrame mTranslationConstraint_ReferenceFrame;
	float mTranslationConstraint_MaxVelocity;
	float mTranslationConstraint_MaxAcceleration;
	bool mRotationConstraint__Enabled;
	ConstraintReferenceFrame mRotationConstraint_ReferenceFrame;
	float mRotationConstraint_MaxBendAngularVelocity;
	float mRotationConstraint_MaxBendAngularAcceleration;
};


struct BallTwistJointKey {

	bool mBoneLengthConstraint__Enabled;
	bool mBoneLengthConstraint_RestRelative;
	float mBoneLengthConstraint_NodeMobility;
	float mBoneLengthConstraint_ParentMobility;
	bool mAngleConstraint__Enabled;
	float mAngleConstraint_MaxBendAngle;
	float mAngleConstraint_MinTwistAngle;
	float mAngleConstraint_MaxTwistAngle;
	bool mTranslationConstraint__Enabled;
	ConstraintReferenceFrame mTranslationConstraint_ReferenceFrame;
	float mTranslationConstraint_MaxVelocity;
	float mTranslationConstraint_MaxAcceleration;
	bool mRotationConstraint__Enabled;
	ConstraintReferenceFrame mRotationConstraint_ReferenceFrame;
	float mRotationConstraint_MaxBendAngularVelocity;
	float mRotationConstraint_MaxBendAngularAcceleration;
	float mRotationConstraint_MaxTwistAngularVelocity;
	float mRotationConstraint_MaxTwistAngularAcceleration;

};

struct PlaceableBallTwistJointKey {
	bool mBoneLengthConstraint__Enabled;
	bool mBoneLengthConstraint_RestRelative;
	float mBoneLengthConstraint_NodeMobility;
	float mBoneLengthConstraint_ParentMobility;
	bool mAngleConstraint__Enabled;
	float mAngleConstraint_MaxBendAngle;
	float mAngleConstraint_MinTwistAngle;
	float mAngleConstraint_MaxTwistAngle;
	bool mPlacementLockToAnimConstraint__Enabled;
	AnimationConstraint::LockMode mPlacementLockToAnimConstraint_LockMode;
	bool mPlacementLockToNodeConstraint__Enabled;
	AnimationConstraint::Node mPlacementLockToNodeConstraint_Node;
	AnimationConstraint::LockMode mPlacementLockToNodeConstraint_LockMode;
	bool mTranslationConstraint__Enabled;
	ConstraintReferenceFrame mTranslationConstraint_ReferenceFrame;
	float mTranslationConstraint_MaxVelocity;
	float mTranslationConstraint_MaxAcceleration;
	bool mRotationConstraint__Enabled;
	ConstraintReferenceFrame mRotationConstraint_ReferenceFrame;
	float mRotationConstraint_MaxBendAngularVelocity;
	float mRotationConstraint_MaxBendAngularAcceleration;
	float mRotationConstraint_MaxTwistAngularVelocity;
	float mRotationConstraint_MaxTwistAngularAcceleration;
};

struct LinkedBallTwistJointKey {

	bool mBoneLengthConstraint__Enabled;
	bool mBoneLengthConstraint_RestRelative;
	float mBoneLengthConstraint_NodeMobility;
	float mBoneLengthConstraint_ParentMobility;
	bool mAngleConstraint__Enabled;
	float mAngleConstraint_MaxBendAngle;
	float mAngleConstraint_MinTwistAngle;
	float mAngleConstraint_MaxTwistAngle;
	bool mTranslationConstraint__Enabled;
	ConstraintReferenceFrame mTranslationConstraint_ReferenceFrame;
	float mTranslationConstraint_MaxVelocity;
	float mTranslationConstraint_MaxAcceleration;
	bool mRotationConstraint__Enabled;
	ConstraintReferenceFrame mRotationConstraint_ReferenceFrame;
	float mRotationConstraint_MaxBendAngularVelocity;
	float mRotationConstraint_MaxBendAngularAcceleration;
	float mRotationConstraint_MaxTwistAngularVelocity;
	float mRotationConstraint_MaxTwistAngularAcceleration;
	bool mLinkBendConstraint__Enabled;
	float mLinkBendConstraint_Strength;
	float mLinkBendConstraint_EchoDelay;
	bool mLinkTwistConstraint__Enabled;
	float mLinkTwistConstraint_Strength;
	float mLinkTwistConstraint_EchoDelay;

};

enum EmitterTrigglerEnable {
	eEmitterTriggerEnable_None = 1,
	eEmitterTriggerEnable_AgentVis = 2,
	eEmitterTriggerEnable_AgentVisInvert = 3,
	eEmitterTriggerEnable_Default = 4,
};

enum EmittersEnableType {
	eEmittersEnableType_All = 1,
	eEmittersEnableType_Random = 2,
	eEmittersEnableType_Sequential = 3,
};

struct EnumEmitterTriggerEnable : EnumBase {
	EmitterTrigglerEnable mVal;
};

struct EnumEmittersEnableType : EnumBase {
	EmittersEnableType mVal;
};

enum ParticleAffectorType {
	eParticleAffectorType_Force = 1,
	eParticleAffectorType_Attractor = 2,
	eParticleAffectorType_KillPlane = 3,
	eParticleAffectorType_KillBox = 4,
	eParticleAffectorType_CollisionPlane = 5,
	eParticleAffectorType_CollisionSphere = 6,
	eParticleAffectorType_CollisionBox = 7,
	eParticleAffectorType_CollisionCylinder = 8,
};

enum ParticleGeometryType {
	eParticleGeometry_Sprite = 1,
	eParticleGeometry_Quad = 2,
	eParticleGeometry_Streak = 3,
	eParticleGeometry_Strip = 4,
	eParticleGeometry_StripFacing = 5,
	eParticleGeometry_None = 6
};

struct EnumParticleAffectorType : EnumBase {
	ParticleAffectorType mVal;
};

struct EnumParticleGeometryType : EnumBase {
	ParticleGeometryType mVal;
};

struct PhysicsObject {

	enum ePhysicsCollisionType {
		PhysicsCollisionType_InterAgent = 0,
		PhysicsCollisionType_Raycast = 1,
	};

	enum ePhysicsBoundingVolumeType {
		PhysicsBoundingVolumeType_Cylinder = 0,
		PhysicsBoundingVolumeType_Box = 1,
		PhysicsBoundingVolumeType_Sphere = 2,
	};

	struct EnumePhysicsBoundingVolumeType : EnumBase {
		ePhysicsBoundingVolumeType mVal;
	};

	struct EnumePhysicsCollisionType : EnumBase {
		ePhysicsCollisionType mVal;
	};

	bool mbEnabledPropertyOn;
	float mfBoundingVolumeScalingFactor;
	PhysicsObject::ePhysicsBoundingVolumeType mBoundingVolumeType;
	PhysicsObject::ePhysicsCollisionType mCollisionType;

};

struct CameraFacingTypes {
	int mCameraFacingType;

	enum cFacingTypes {
		eFacing = 0,
		eFacingY = 1,
		eFacingLocalY = 2,
	};

};

class MetaStream;
/*removes class,struct,std etc*/
String MetaVersion_ConvertTypeName(const String& from);
bool MetaVersion_MemberExistsInCurrentVersion(MetaMemberDescription* pDesc, MetaStream* stream);

const u32 EncrypedVersionHeaders[] = {
	0x64AFDEAA,//version 3
	0x64AFDEBB,//version 3 (encrypted MCOM)
	0xFB4A1764,//version 2
	0xEB794091,//version 2
	0x64AFDEFB,//version 2
};

//reason they are reversed is because in the executables these are stored as INTS and because of the endian they are reversed
constexpr const char VersionHeaders[][5] = {
#if LITTLE_ENDIAN == 1
	"SEBM",//version 0, Meta Binary Encrypted Stream
	"NIBM",//version 0, Meta BINary
	"ERTM",//version 3, Meta Type REference (I think, more like meta try reverse engineer (assholes, god im lonely)) maybe MT in MTRE is meta
	"MOCM",//version 3, Meta COMpressed, compressed version of MTRE. this might be wrong, im thinking its more meta communication?
	//maybe for example transporting stuff in the engine?
	"4VSM",//version 4, Meta Stream Version 4
	"5VSM",//version 5
	"6VSM",//version 6
#else
	"MBES",//version 0, Meta Binary Encrypted Stream
	"MBIN",//version 0, Meta BINary
	"MTRE",//version 3, no clue
	"MCOM",//version 3, Meta COMpressed
	"MSV4",//version 4, Meta Stream Version 4
	"MSV5",//version 5
	"MSV6",//version 6
#endif
};

constexpr const char* VersionHeadersStr[7] = {
	"MBES - Meta Binary Encrypted Stream",//version 0, Meta Binary Encrypted Stream
	"MBIN - Meta Binary",//version 0, Meta BINary
	"MTRE = Meta Try Reverse Engineer (this) - I just did :P",//version 3, no clue
	"MCOM - Meta Compressed",//version 3, Meta COMpressed
	"MSV4 - Mea Stream Version 4",//version 4, Meta Stream Version 4
	"MSV5 - Meta Stream Version 5",//version 5
	"MSV6 - Meta Stream Version 6",//version 6
};

void __ReleaseSVI_Internal();
void __RegisterSVI_Internal(void* svi);

//returns if the given member is not in the current game version (currently set game index)
bool Meta_IsMemberDisabled(MetaMemberDescription* member, i32 streamVersion);

struct SerializedVersionInfo {

	SerializedVersionInfo* mpNext = nullptr;

	struct MemberDesc {
		String mName;
		String mTypeName;
		u64 mTypeNameSymbolCrc;
		bool mbBlocked;
		u32 mSize;
		u32 mVersionCrc;
		MetaMemberDescription* mpMemberDesc;
	};

	String mFileName;
	u64 mTypeSymbolCrc=0;
	u32 mVersionCrc=0;
	u32 mSize=0;
	bool mbBlocked=false;
	std::vector<MemberDesc> mMembers;//DCArrayNM<MemberDesc> mMembers;
	bool mbOldVersion = false;//true for games in old .ttarch (sam max 1-3 etc), false for new games (twau>=)

	static SerializedVersionInfo* RetrieveCompiledVersionInfo(MetaClassDescription* pObjDescription);

	//Originally would save to <Tool>/Meta/<file> This saves in .vers format. This writes everything (including header).
	//Vers file (serialized versions) names are in the format %s1(%s2).vers , where %s1 is the type name, 
	//and %s2 is the base 36 of the version CRC. Returns a datastream pointer, which you need to delete
	//Leave the versName as empty if you already have  the type symbol CRC set and it will be set to mFileName = versName = calc_crc...()
	// NOTE: THIS WILL ONLY WORK WHEN mbOldVersion is FALSE. OLD VERSIONS CANNOT BE SAVED DUE TO THEIR TYPE NAMES (any they shouldn't be saved !!)
	DataStream* Save(String* pVersName);

	static String MakeVersionFileName(const char* typeName, u32 tn);

	//TAKES OWNERSHIP OF STREAM PARAM, IT WILL BE DELETED AFTER USE
	//Loads this version information from a .vers.
	//There should be three parameters (type name crc, version crc) and the engine would load the .vers from that
	void RetrieveVersionInfo(/*u64 typeNameCrc, u32 typeVersionCrc, */ const char* versFileName, DataStream* stream);

};

static constexpr u32 GetMetaMagic(int versionID) {
	if (versionID >= 0 && versionID <= 6)return *((u32*)VersionHeaders[versionID]);
	return 0;
}

enum MetaOpResult {
	eMetaOp_Fail = 0x0,
	eMetaOp_Succeed = 0x1,
	eMetaOp_Invalid = 0x2,
	eMetaOp_OutOfMemory = 0x3,
	eMetaOp_SymbolNotFoundInDB = 0x4,
	eMetaOp_SymbolNotFound = 0x4,
	eMetaOp_MAX = 0x5,//normally 4
};

namespace UID {
	typedef int idT;

	struct __declspec(align(8)) Generator {
		static constexpr idT msUninitID = -1;
		idT miNextUniqueID;

		Generator() : miNextUniqueID(1) {}

		static idT UninitID() {
			return 0xFFFFFFFF;
		}

		inline void Reset() {
			miNextUniqueID = 1;
		}

		inline bool ValidID(idT id) {
			return id >= 1 && id < this->miNextUniqueID;
		}

		inline idT GetNextUniqueID(bool bIncrement) {
			idT _t = miNextUniqueID;
			if (miNextUniqueID == 0xFFFFFFFF) {
				miNextUniqueID = 1;
				_t = 1;
			}
			else if (bIncrement)
				miNextUniqueID++;
			return _t;
		}
	};

	struct __declspec(align(8)) Owner {
		idT miUniqueID;

		Owner() : miUniqueID(-1) {}

		Owner(Generator& gen) {
			miUniqueID = gen.GetNextUniqueID(true);
		}

	};

};

struct T3VertexSampleDataBase {
	//mAllocator, mRefCount
	int mNumVerts;
	int mVertSize;
	char* mpData;

	INLINE T3VertexSampleDataBase() : mpData(NULL) {}

	INLINE ~T3VertexSampleDataBase() {
		if (mpData)
			free(mpData);
	}

	static METAOP_FUNC_DEF(SerializeAsync);

};

constexpr const char* spVersNameFormat = "%s(%s).vers";

struct ZTestFunction {
	enum zFuncTypes {
		eNever = 0x1, eLess = 0x2, eEqual = 0x3, eLessEqual = 0x4, eGreater = 0x5, eNotEqual = 0x6, eGreaterEqual = 0x7, eAlways = 0x8,
	};
	zFuncTypes mZTestType;
};

struct Color {
	float r, g, b, a;

	inline bool operator<(const Color& rhs) const {
		if (r == rhs.r) {
			bool v11 = false, v15 = false;
			float v16, v17;
			if (rhs.g != g || (v16 = rhs.b, v17 = b, v16 != v17))
				v15 = v17 > v16;
			v11 = !v15;
			return v11;
		}
		else {
			return r <= rhs.r;
		}
	}

};

struct ColorHDR {
	float r, g, b, intensity;
};

namespace Meta {

	//set the version crc in the serializedversioninfo ( IS IMPLEMENTED, intellisense my ass)
	MetaOpResult MetaOperation_SerializedVersionInfo(void* pObj,
		MetaClassDescription* pObjDescription, MetaMemberDescription* pContextDesc,
		void* pUserData);

	struct Equivalence {
		bool mbEqual;
		void* mpOther;
	};

	typedef void (*EnumerateMembersFunc)(void*, MetaClassDescription*, MetaMemberDescription*);

	struct EnumerateMembersInfo {
		EnumerateMembersFunc mpFunc;
		std::vector<void*> mArgs;//DCArray<void*>
	};

	struct ConvertFromInfo {
		const void* mpFromObject;
		MetaClassDescription* mpFromObjDescription;
	};

	METAOP_FUNC_DEF(GetObjectName)

	METAOP_FUNC_DEF(EnumerateMembers)

	METAOP_FUNC_DEF(Destroy)

	MetaOpResult MetaOperation_SerializeAsync(void*, MetaClassDescription*,
		MetaMemberDescription*, void*);

	METAOP_FUNC_DEF(SerializeMain)

	METAOP_FUNC_DEF(Equivalence)

	METAOP_FUNC_DEF(AsyncSave)

};

const struct MetaStreamParams {
	bool mbCompress;
};

enum class MetaStreamMode {
	eMetaStream_Closed = 0x0, eMetaStream_Read = 0x1, eMetaStream_Write = 0x2,
};

struct MetaVersionInfo {
	u64 mTypeSymbolCrc;
	u32 mVersionCrc;
};

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args);

std::string SymToString(Symbol* pSymbol);

//If you have opened and it errored on Open, then you have to reset the input stream to its offset when first passed into Open,
//can implement child class MetaStream_JSON
class MetaStream {

public:

	enum RuntimeFlags {
		eWriteback = 0x1,
		eStreamIsCompiledVersion = 0x2,
		eIsUndo = 0x4
	};

	enum SectionType : u32 {
		eSection_Header = 0x0,
		eSection_Default = 0x1,
		eSection_Debug = 0x2,
		eSection_Async = 0x3, 
		eSection_Count = 0x4
	};

	enum class StreamType {
		eStream_Binary = 0x0,
		eStream_JSON = 0x1
	};

	struct BlockInfo {
		u32 mBlockLength;//or start pos
	};

	struct SectionInfo {
		DataStream* mpStream;//mpDataStream
		u64 mCompressedSize;
		std::stack<BlockInfo> mBlockInfo;//LIFO
		bool mbEnable;
		bool mbCompressed;
		//mReadBuffer, dont need it

		//u64 mStreamSize, mStreamOffset, mStreamPosition;

		SectionInfo() {
			mbEnable = true;
			mpStream = nullptr;
			mCompressedSize = 0;
			mbCompressed = false;
		}

	};

	struct WriteParams {//not in telltale tool, used for this library to write meta streams
		bool mbEncryptStream : 1;
		u8 mEncryptVersion : 7;//1 2 or 3
		bool mbSerializeAsCompressVersion;//MCOM. not supported (unknown int, and no files of this type in public, must be engine private).
		
		WriteParams() : mbEncryptStream(false), mEncryptVersion(3), mbSerializeAsCompressVersion(false) {}

	};

	//normally is separated into SubStreamInfo, each one could be a metastream or just data. for simplicity just done it like this

	SectionInfo mSection[(int)SectionType::eSection_Count];
	std::vector<MetaVersionInfo> mVersionInfo;
	std::vector<MetaClassDescription*> mSerializedTypes;
	MetaStreamParams mParams{};
	int mDebugSectionDepth = 0;
	SectionType mCurrentSection = eSection_Default;
	WriteParams mWriteParams{};

	struct MissingMemberCallbackInfo {
		bool(*mMissingMemberCallback)(SerializedVersionInfo::MemberDesc*, void*);
		void* mpUserData;
	};

	std::vector<MissingMemberCallbackInfo> mMissingMemberCallbacks;

	/*
	* Stream Versions:
	* 0: MBIN (and MBES if encrypted)
	* 1: Encrypted MBIN (weird headers)
	* 2: MTRE (if the meta stream header is not MTRE its a weird encrypted header)
	* 3: MCOM
	* 4: MSV4 - No default section, just async and debug
	* 5: MSV5 
	* 6: MSV6
	*/
	u32 mStreamVersion = 0;
	DataStream* mpReadWriteStream = nullptr;
	MetaStreamMode mMode = MetaStreamMode::eMetaStream_Closed;
	//Blowfish* mpBlowfish;
	Flags mRuntimeFlags;//flag values: RuntimeFlags enum
	//char mName[260];

	INLINE bool IsRead() {
		return mMode == MetaStreamMode::eMetaStream_Read;
	}

	INLINE bool IsWrite() {
		return mMode == MetaStreamMode::eMetaStream_Write;
	}

	bool mbDontDeleteStream = false;//by lib

	bool mbErrored = false;

	INLINE virtual MetaStream::StreamType GetStreamType() { return StreamType::eStream_Binary; }
	virtual u64 Close();//returns size of stream
	virtual bool Attach(DataStream*, MetaStreamMode, MetaStreamParams);
	//IMPORTANT: THIS CLASS TAKES *OWNERSHIP* OF THE DATASTREAM AND WILL DELETE IT WHEN DONE! NOT VIRTUAL, CALLS ATTACH
	virtual void Open(DataStream*, MetaStreamMode, MetaStreamParams);
	virtual void DisableDebugSection();
	virtual u64 GetPartialStreamSize();
	void PushMissingMemberCallback(bool(*cb)(
		SerializedVersionInfo::MemberDesc*, void*),
	void* mpUserData);
	void PopMissingMemberCallback();
protected:
	virtual i64 ReadData(void*, u32);
	virtual i64 WriteData(void*, u32);
public:
	virtual bool BeginAsyncSection();
	virtual void EndAsyncSection();
	virtual bool HasAsyncSection();
	virtual bool BeginDebugSection();
	virtual void EndDebugSection();
	virtual bool HasDebugSection();
	virtual u64 GetSize();
	virtual u64 GetPos();
	virtual void SetPos(u64);
	virtual void Advance(int numBytes);
	virtual void BeginBlock();
	virtual void EndBlock();
	virtual void SkipToEndOfCurrentBlock();

	INLINE void BeginObject(Symbol* pSymbol, bool bArray) {
		BeginObject(SymToString(pSymbol).c_str(), bArray);
	}

	INLINE void EndObject(Symbol* pSymbol) {
		EndObject(SymToString(pSymbol).c_str());
	}

	INLINE void InjectVersionInfo(MetaStream& from) {
		mVersionInfo = from.mVersionInfo;
		mStreamVersion = from.mStreamVersion;
	}

	virtual bool NeedsKey();

	virtual void BeginObject(const char*, bool bArray);
	virtual void EndObject(const char*);
	virtual void BeginObject(const char*, MetaClassDescription*, MetaMemberDescription*, bool bArray);
	virtual void EndObject(const char*, MetaClassDescription*, MetaMemberDescription*);

	void AddVersion(const SerializedVersionInfo*);
	MetaVersionInfo* GetStreamVersion(u64 typeSymbolCrc);
	MetaVersionInfo* GetStreamVersion(MetaClassDescription* d);
	virtual void serialize_String(String*);
	virtual void serialize_Symbol(Symbol*);
	virtual void serialize_bool(bool*);
	int serialize_bytes(void*, u32);
	bool _ReadHeader(DataStream* partial, u64, u64* pOutBytesNeeded);
	void _WriteHeader();
	void _FinalizeStream();
	bool _SetSection(SectionType);
	virtual void serialize_double(long double*);
	virtual void serialize_float(float*);
	virtual void serialize_uint16(u16*);
	virtual void serialize_uint32(u32*);
	virtual void serialize_uint64(u64*);
	virtual void serialize_int8(char*);

	virtual void HintSymbol(const char* symbolData);

	virtual void InsertStringHint(const char* pK, const char* pS);
	virtual const char* SkipStringHint(const char* pK);
	virtual void Key(const char* pS);

	void SwitchToMode(MetaStreamMode newMode,DataStream* inputOrOutputStream);//by lib, a useful function

	MetaStream();
	~MetaStream();

};

enum HTextAlignmentType {
	eHTextAlignment_None = 0,
	eHTextAlignment_LeftJustified = 1,
	eHTextAlignment_Centered = 2,
	eHTextAlignment_RightJustified = 3,
};

enum VTextAlignmentType {
	eVTextAlignment_None = 0,
	eVTextAlignment_Top = 1,
	eVTextAlignment_Middle = 2,
	eVTextAlignment_Bottom = 3,
};

enum TonemapType {
	eTonemapType_Default = 1,
	eTonemapType_Filmic = 2,
};

struct EnumTonemapType {
	TonemapType mVal;
};

struct EnumHTextAlignmentType {
	HTextAlignmentType mVal;
};

enum T3MaterialSwizzleType
{
	eMaterialSwizzle_None = 0x0,
	eMaterialSwizzle_X = 0x1,
	eMaterialSwizzle_Y = 0x2,
	eMaterialSwizzle_Z = 0x3,
	eMaterialSwizzle_W = 0x4,
	eMaterialSwizzle_Zero = 0x5,
	eMaterialSwizzle_One = 0x6,
	eMaterialSwizzle_FirstComponent = 0x1,
	eMaterialSwizzle_FirstNumeric = 0x5,
};

//.TRANS FILES
struct StyleIdleTransitionsRes {
	String mOwningAgent, mGuideName;
};

struct EnumT3MaterialSwizzleType : EnumBase {
	T3MaterialSwizzleType mVal;
};

struct T3MaterialSwizzleParams
{
	union {
		EnumT3MaterialSwizzleType mSwizzle[4];
		u32 __reserve;
	};
};

struct EnumVTextAlignmentType {
	VTextAlignmentType mVal;
};

struct TextAlignmentType
{
	int mAlignmentType;
};

typedef void* (*FuncNew)(void);
typedef void (*FuncDestroy)(void*);
typedef void (*FuncDelete)(void*);
typedef void (*FuncCopyConstruct)(void*, void*);
typedef void (*FuncConstruct)(void*);
typedef void (*FuncCastToConcreteObject)(void**, MetaClassDescription**);

extern i32 sMetaTypesCount;

class Symbol {
private:
	static char smSymbolBuffer[sizeof(u64) * 2 + 1];//1byte= 2 hex chars
	u64 mCrc64;
public:

	struct CompareCRC {
		bool operator()(const Symbol _Left, const Symbol _Right) const {
			return _Left.mCrc64 < _Right.mCrc64;
		}
	};

	constexpr Symbol(void) : mCrc64(0) {}

	constexpr Symbol(const char const* pString) : mCrc64(CRC64_CaseInsensitive(0, pString)) {}

	//constexpr Symbol(const String& pString) {
	//	mCrc64 = CRC64_CaseInsensitive(0, pString.c_str());
	//}

	constexpr Symbol(u64 crc) : mCrc64(crc) {}
	//constexpr Symbol(const char* const pString) {
	//	mCrc64 = CRC64_CaseInsensitive(0, pString);
	//}

	constexpr Symbol& operator=(const Symbol& rhs) {
		this->mCrc64 = rhs.mCrc64;
		return *this;
	}

	INLINE const char* CRCAsCstr() const {//not async
		sprintf(smSymbolBuffer, "%llx", mCrc64);
		return smSymbolBuffer;
	}

	constexpr INLINE void SetCRC(u64 crc) { mCrc64 = crc; }

	constexpr Symbol& Concat(const char* p) {
		mCrc64 = CRC64_CaseInsensitive(mCrc64, p);
		return *this;
	}

	constexpr INLINE u64 GetCRC() const {
		return mCrc64;
	}

	//AsConcat isnt needed

	INLINE operator const char* () const { return CRCAsCstr(); }

	static MetaOpResult MetaOperation_Equivalence(void* pObj,
		MetaClassDescription* pClassDescription, MetaMemberDescription* pContextDescription,
		void* pUserData) {//userdata=meta::equivalence
		static_cast<Meta::Equivalence*>(pUserData)->mbEqual = static_cast<Symbol*>(pObj)->mCrc64 ==
			static_cast<Symbol*>(static_cast<Meta::Equivalence*>(pUserData)->mpOther)->mCrc64;
		return MetaOpResult::eMetaOp_Succeed;
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pClassDescription,
		MetaMemberDescription* pContextDescription, void* pUserData) {
		static_cast<MetaStream*>(pUserData)->serialize_Symbol(static_cast<Symbol*>(pObj));
		return eMetaOp_Succeed;
	}

	INLINE bool operator==(const Symbol& o) {
		return o.mCrc64 == mCrc64;
	}

	INLINE bool operator!=(const Symbol& o) {
		return o.mCrc64 != mCrc64;
	}

	static const Symbol kEmptySymbol;
	static const Symbol kTransitionMapKey;
	static const Symbol kAnimGroupArmLeft;
	static const Symbol kAnimGroupArmRight;
	static const Symbol kAnimGroupBodyLower;
	static const Symbol kAnimGroupBodyUpper;
	static const Symbol kAnimGroupBrowLeft;
	static const Symbol kAnimGroupBrowRight;
	static const Symbol kAnimGroupBrows;
	static const Symbol kAnimGroupCheekLeft;
	static const Symbol kAnimGroupCheekRight;
	static const Symbol kAnimGroupCheeks;
	static const Symbol kAnimGroupExtra;
	static const Symbol kAnimGroupEyeLids;
	static const Symbol kAnimGroupEyes;
	static const Symbol kAnimGroupFace;
	static const Symbol kAnimGroupFingersLeft;
	static const Symbol kAnimGroupFingersRight;
	static const Symbol kAnimGroupFootLeft;
	static const Symbol kAnimGroupFootRight;
	static const Symbol kAnimGroupHair;
	static const Symbol kAnimGroupHandRight;
	static const Symbol kAnimGroupHandLeft;
	static const Symbol kAnimGroupHead;
	static const Symbol kAnimGroupIndexFLeft;
	static const Symbol kAnimGroupIndexFRight;
	static const Symbol kAnimGroupLegLeft;
	static const Symbol kAnimGroupLegRight;
	static const Symbol kAnimGroupMouth;
	static const Symbol kAnimGroupNeck;
	static const Symbol kAnimGroupRoot;
	static const Symbol kAnimGroupShoulderLeft;
	static const Symbol kAnimGroupShoulderRight;
	static const Symbol kAnimGroupGroupSkirt;
	static const Symbol kAnimGroupGroupTail;
	static const Symbol kAnimGroupThumbLeft;
	static const Symbol kAnimGroupThumbRight;

	static const Symbol kPropKeyViewportOrigin;
	static const Symbol kPropKeyViewportSize;
	static const Symbol kPropKeyViewportCamera;
	static const Symbol kPropKeyViewportRelative;
	static const Symbol kPropKeyAllowInterpolation;
	static const Symbol kBGMStartPos;

	static const Symbol sAssertSymbol;
	static const Symbol sAssertTimeStampSymbol;
	static const Symbol sAssertFileSymbol;
	static const Symbol sAssertLineSymbol;
	static const Symbol sGeneralSymbol;
	static const Symbol sChoreSymbol;
	static const Symbol sLuaSymbol;
	static const Symbol sGraphicsSymbol;
	static const Symbol sDialogSymbol;
	static const Symbol sAudioSymbol;
	static const Symbol sQtSymbol;//telltale use qt in the tool!!!
	static const Symbol sExportSymbol;
	static const Symbol sCoreSymbol;

	static const Symbol kPropKeySource;
	static const Symbol kPropKeyWidth;
	static const Symbol kPropKeyHeight;
	static const Symbol kPropKeyPause;
	static const Symbol kPropKeyVolume;
	static const Symbol kPropKeyMediaTimeout;
	static const Symbol kPropKeyVolumeMaster;
	static const Symbol kPropKeyVolumeMusic;
	static const Symbol kPropKeyVolumeSound;
	static const Symbol kPropKeyVolumeVoice;
	static const Symbol kPropKeyMuteVoices;
	static const Symbol kPropKeyVolumeAmbient;
	static const Symbol kPropKeyMixMaster;
	static const Symbol kPropKeyMixMusic;
	static const Symbol kPropKeyMixSound;
	static const Symbol kPropKeyMixVoice;
	static const Symbol kPropKeyMixAmbient;
	static const Symbol kPropKeySoundMode;
	static const Symbol kCompressAnimations;
	static const Symbol kEncryptMetaStreams;
	static const Symbol kPropKeyROMRuntimeLoad;
	static const Symbol kPropKeyROMErrorDialogResource;
	static const Symbol kPropKeyROMErrorTitle;
	static const Symbol kPropKeyROMErrorMessage;
	static const Symbol kPropKeyROMPackSubDir;
	static const Symbol kPropKeyTextMinSec;
	static const Symbol kPropKeyTextSecPerLetter;
	static const Symbol kPropKeyUserSpaceAsArchive;
	static const Symbol kPropKeyDisableDevFolders;
	static const Symbol kPropKeyUsageOS;
	static const Symbol kPropKeyUsageCPU;
	static const Symbol kPropKeyUsageGPU;
	static const Symbol kPropKeyUsageDeviceType;
	static const Symbol kPropKeyUsageDeviceModel;
	static const Symbol kPropKeyUsageDeviceName;
	static const Symbol kPropKeyUsageLaunchCount;
	static const Symbol kPropKeyUsageShutdownCount;
	static const Symbol kPropKeyUsageSessionExecutionTime;
	static const Symbol kPropKeyUsageTotalExecutionTime;
	static const Symbol kChoreAgentGroupFilterIncludeNonSkeleton;
	static const Symbol kPropOpenLocalizations;
	static const Symbol kHDScreenShotResolution;
	static const Symbol kBlocking;
	static const Symbol kBlockingOffset;
	static const Symbol kBlockingOffsetContribution;
	static const Symbol kResourceNameSuffix;
	static const Symbol kLookAtBlockingAnimation;
	static const Symbol kLookAtBlockingOffsetAnimation;
	static const Symbol kLookAtBlockingOffsetContributionAnimation;

};

template<typename T, typename U> constexpr size_t memberOffset(U T::* member)
{
	return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

template<typename Base, typename Derived> constexpr size_t parentOffset() {
	return (reinterpret_cast<char*>(static_cast<Base*>(reinterpret_cast<Derived*>(0x10000000))) - reinterpret_cast<char*>(0x10000000));
}

MetaClassDescription* GetMetaClassDescription(const char* typeInfoName);

//would have used to be inclined getmetaaclassdescription functions, this would have had template specializations for all types.
template<typename T>
struct MetaClassDescription_Typed {

	static MetaClassDescription* GetMetaClassDescription() {
		return GetMetaClassDescription(NULL);
	}

	static MetaClassDescription* GetMetaClassDescription
	(const char* type);//originally this would be specialized

	static void* New(void) {
		void* ptr = operator new(sizeof(T));
		new (ptr) T();
		return ptr;
	}

	static void Destroy(void* pObj) {
		static_cast<T*>(pObj)->~T();
	}

	static void Construct(void* pObj) {
		new(pObj) T();
	}

	static void CopyConstruct(void* pDest, void* pSrc) {
		Construct(pDest);
		*static_cast<T*>(pDest) = *static_cast<T*>(pSrc);
	}

	static void Delete(void* pObj) {
		static_cast<T*>(pObj)->~T();
		operator delete(pObj);
	}

};

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	//if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

enum VTableFunction {
	eVTableNew = 0,
	eVTableDelete = 1,
	eVTableConstruct = 2,
	eVTableCopyConstruct = 3,
	eVTableDestroy = 4,
	//eVTableCastToConcreteObject = 5,
	eVTableCount = 5//6
};

struct BlendCameraResource {};

enum MetaFlag : int {
	//Dont serialize this type
	MetaFlag_MetaSerializeDisable = 1,
	//Dont put a block size infront of this type
	MetaFlag_MetaSerializeBlockingDisabled = 2,
	//Place in game engine property menu
	MetaFlag_PlaceInAddPropMenu = 4,
	//No caption in the panel its in
	MetaFlag_NoPanelCaption = 8,
	//This type is a base class
	MetaFlag_BaseClass = 0x10,
	//Dont show this in the game engine editor
	MetaFlag_EditorHide = 0x20,
	//Is an enum, of type int
	MetaFlag_EnumIntType = 0x40,
	//Is an enum of type string
	MetaFlag_EnumStringType = 0x80,
	//Is a container type (eg a map, list, array)
	MetaFlag_ContainerType = 0x100,
	//Is a script enum type (enum used in lua scripts)
	MetaFlag_ScriptEnum = 0x200,
	//The name of this type (in meta member descriptions) is allocated on the heap
	MetaFlag_Heap = 0x400,
	//Serialized or created from lua scripts
	MetaFlag_ScriptTransient = 0x800,
	//Not seen this used yet
	MetaFlag_SelectAgentType = 0x1000,
	//Not seen this used yet, just object state is a meta operation
	MetaFlag_SkipObjectState = 0x2000,
	//Not seen this used yet
	MetaFlag_NotCacheable = 0x4000,
	//This type wraps an enum. Eg, this type is a struct EnumPlatformType{PlatformType mType}, where PlatformType is an enum
	//Reason its like this is because the enumplatformtype struct inherits from EnumBase and that has a seperate description
	MetaFlag_EnumWrapperClass = 0x8000,
	//Not seen this used yet
	MetaFlag_TempDescription = 0x10000,
	//This type is a handle (reference to a file). If serialized this is a CRC
	MetaFlag_Handle = 0x20000,
	//This type has a list of flags present with it (eg FlagsT3LightEnvGroupSet)
	MetaFlag_FlagType = 0x40000,
	//Not seen this used yet
	MetaFlag_SelectFolderType = 0x80000,
	//This type has no members
	MetaFlag_Memberless = 0x100000,
	//This type is a renderable resource (eg a texture or font)
	MetaFlag_RenderResource = 0x200000,
	//If this type has a block around it but the size of the serialized version is not always one value
	MetaFlag_MetaSerializeNonBlockedVariableSize = 0x400000 | MetaFlag_MetaSerializeBlockingDisabled,
	//Not seen this used yet
	MetaFlag_EmbeddedCacheableResource = 0x800000,
	//Not seen this used yet
	MetaFlag_VirtualResource = 0x1000000,
	//Not seen this used yet
	MetaFlag_DontAsyncLoad = 0x2000000,
	//If this type is not a meta file
	MetaFlag_IsNotMetaFile = 0x4000000,
	//If this type has been initialized
	Internal_MetaFlag_Initialized = 0x20000000,
	//By library, if this is set then the class is a proxy class and is prone to deletion (see reset proxy meta)
	Internal_MetaFlag_IsProxied = 0x40000000,
	Internal_MetaFlag_ShowAsHex = 0x10000000
};

/*
*
00000050 ReadData        dq ? ; offset 80
00000058 WriteData       dq ? ; offset 88
00000060 ReadDataStream  dq ? ; offset 96
00000068 BeginAsyncSection dq ? ; offset 104
00000070 EndAsyncSection dq ? ; offset 112
00000078 HasAsyncSection dq ? ; offset 120
00000080 BeginDebugSection dq ? ; offset 128
00000088 EndDebugSection dq ? ; offset 136
00000090 HasDebugSection dq ? ; offset 144
00000098 GetSize         dq ? ; offset 152
000000A0 GetPos          dq ? ; offset 160
000000A8 SetPos          dq ? ; offset 168
000000B0 Advance         dq ? ; offset 176
000000B8 BeginBlock      dq ? ; offset 184
000000C0 EndBlock        dq ? ; offset 192
000000C8 SkipToEndOfCurrentBlock dq ? ; offset 200
000000D0 BeginObject     dq ? ; offset 208
000000E8 EndObject       dq ? ; offset 232
00000100 BeginAnonObject dq ? ; offset 256
00000108 EndAnonObject   dq ? ; offset 264
00000110 SetObjectAsArrayType dq ? ; offset 272
00000118 AddVersion      dq ? ; offset 280
00000120 serialize_float dq ? ; offset 288
00000128 serialize_double dq ? ; offset 296
00000130 serialize_int   dq ? ; offset 304
00000138 serialize_uint  dq ? ; offset 312
00000140 serialize_int8  dq ? ; offset 320
00000148 serialize_uint8 dq ? ; offset 328
00000150 serialize_int16 dq ? ; offset 336
00000158 serialize_uint16 dq ? ; offset 344
00000160 serialize_int32 dq ? ; offset 352
00000168 serialize_uint32 dq ? ; offset 360
00000170 serialize_int64 dq ? ; offset 368
00000178 serialize_uint64 dq ? ; offset 376
00000180 serialize_String dq ? ; offset 384
00000188 serialize_Symbol dq ? ; offset 392
00000190 serialize_bool  dq ? ; offset 400
00000198 serialize_bytes dq ? ; offset 408*/

struct MetaMemberDescription;
struct MetaClassDescription;

struct MetaSerializeAccel {

	MetaSerializeAccel() : mpFunctionMain(NULL), mpFunctionAsync(NULL), mpMemberDesc(NULL) {}

	MetaOpResult(__cdecl* mpFunctionAsync)(void*, MetaClassDescription*,
		MetaMemberDescription*, void*);
	MetaOpResult(__cdecl* mpFunctionMain)(void*, MetaClassDescription*,
		MetaMemberDescription*, void*);
	MetaMemberDescription* mpMemberDesc;
};

enum RenderLightmapUVGenerationType {
	eRenderLightmapUVGeneration_Default = 0,
	eRenderLightmapUVGeneration_Auto = 1,
	eRenderLightmapUVGeneration_UV0 = 2,
	eRenderLightmapUVGeneration_UV1 = 3,
	eRenderLightmapUVGeneration_UV2 = 4,
	eRenderLightmapUVGeneration_UV3 = 5,
};

struct EnumRenderLightmapUVGenerationType {
	RenderLightmapUVGenerationType mVal;
};

typedef MetaOpResult(*MetaOperation)(void*, MetaClassDescription*, MetaMemberDescription*, void*);

struct MetaOperationDescription {

	enum sIDs {
		eMetaOpZero = 0x0,
		eMetaOpOne = 0x1,
		eMetaOpTwo = 0x2,//AddToChore
		eMetaOpThree = 0x3,
		//add to panel
		eMetaOpFour = 0x4,
		eMetaOpFive = 0x5,
		//convert
		eMetaOpSix = 0x6,
		eMetaOpSeven = 0x7,
		eMetaOpEight = 0x8,
		//equivalence
		eMetaOpNine = 0x9,
		eMetaOpTen = 0x0A,
		eMetaOpEleven = 0x0B,//GetLength
		eMetaOpTwelve = 0x0C,
		eMetaOpThirteen = 0x0D,
		eMetaOpFourteen = 0x0E,
		eMetaOpFifteen = 0x0F,//ObjectState
		eMetaOpSixteen = 0x10,//OpenEditor
		eMetaOpSeventeen = 0x11,
		eMetaOpEighteen = 0x12,//ScriptLock
		eMetaOpNineteen = 0x13,//ScriptUnlock
		eMetaOpTwenty = 0x14,
		eMetaOpTwentyOne = 0x15,//SerializedVersionInfo
		eMetaOpTwentyTwo = 0x16,//SetObjectName
		eMetaOpTwentyThree = 0x17,//ToString
		eMetaOpTwentyFour = 0x18,//GetName
		eMetaOpTwentyFive = 0x19,
		eMetaOpTwentySix = 0x1A,
		eMetaOpTwentySeven = 0x1B,
		eMetaOpTwentyEight = 0x1C,//CollectTyped
		eMetaOpTwentyNine = 0x1D,
		eMetaOpThirty = 0x1E,
		eMetaOpThirtyOne = 0x1F,
		eMetaOpThirtyTwo = 0x20,
		eMetaOpThirtyThree = 0x21,
		eMetaOpThirtyFour = 0x22,
		eMetaOpThirtyFive = 0x23,
		eMetaOpThirtySix = 0x24,
		eMetaOpThirtySeven = 0x25,
		eMetaOpThirtyEight = 0x26,
		eMetaOpThirtyNine = 0x27,
		eMetaOpForty = 0x28,
		eMetaOpFortyOne = 0x29,
		eMetaOpFortyTwo = 0x2A,
		eMetaOpFortyThree = 0x2B,
		eMetaOpFortyFour = 0x2C,
		eMetaOpFortyFive = 0x2D,
		eMetaOpFortySix = 0x2E,
		eMetaOpFortySeven = 0x2F,
		eMetaOpFortyEight = 0x30,
		eMetaOpFortyNine = 0x31,
		eMetaOpFifty = 0x32,
		eMetaOpFiftyOne = 0x33,
		eMetaOpFiftyTwo = 0x34,
		eMetaOpFiftyThree = 0x35,
		eMetaOpFiftyFour = 0x36,//PreloadDependentResources
		eMetaOpFiftyFive = 0x37,
		eMetaOpFiftySix = 0x38,
		eMetaOpFiftySeven = 0x39,
		eMetaOpFiftyEight = 0x3A,
		eMetaOpFiftyNine = 0x3B,
		eMetaOpSixty = 0x3C,
		eMetaOpSixtyOne = 0x3D,
		eMetaOpSixtyTwo = 0x3E,
		eMetaOpGetAssetCost = 0x3F,
		eMetaOpFromUiString = 0x40,
		eMetaOpToUiString = 0x41,
		eMetaOpWidgetFromClass = 0x42,
		eMetaOpUserOpen = 0x43,
		eMetaOpSave = 0x44,
		eMetaOpGetVersion = 0x45,
		eMetaOpCopy = 0x46,
		eMetaOpDelete = 0x47,
		eMetaOpAddToCache = 0x48,
		eMetaOpRemoveFromCache = 0x49,
		eMetaOpSerializeAsync = 0x4A,
		eMetaOpSerializeMain = 0x4B,
		eMetaOpDestroy = 0x4C,
		eMetaOpGetEstimatedVramUsage = 0x4D,
		eMetaOpEnumerateMembers = 0x4E,
		eMetaOpSetupPropertyValue = 0x4F,
		eMetaOpSavePropertyValue = 0x50,
		eMetaOpRecursiveVersionCRC = 0x51,
		eMetaOpNewResource = 0x52,
		eNumMetaOps = 0x53,
	};

	sIDs id;
	MetaOperation mpOpFn;
	MetaOperationDescription* mpNext;
};

struct LightType {
	int mLightType;
};

struct MetaClassDescription {
	const char* mpExt;
	const char* mpTypeInfoName;
	const char* mpTypeInfoExternalName;//for lib
	u64 mHash;
	Flags mFlags;
	u32 mClassSize;
	u32 mClassAlign;
	SerializedVersionInfo* mpCompiledVersionSerializedVersionInfo;//atomic, lazily eval
	MetaMemberDescription* mpFirstMember;
	MetaOperationDescription* mMetaOperationsList;
	MetaClassDescription* pNextMetaClassDescription;
	void* mpVTable[5/*6*/];
	MetaSerializeAccel* mpSerializeAccel;//atomic
	bool mbNameIsHeapAllocated;//created by lib
	bool mbIsIntrinsic;//created by lib, intrinsics arent added to header. where does this filter?? no clue, so i have to add this
	//oh wait for the value above, nevermind just realised metaoperation_serializeasync is not ever called on intrinsics.
	//its only called other objects. since we check for the overloaded specialization function for serialize which is set.
	bool mbTypeUnfinished = false;//if true, this type has not been seen serialized yet and needs pending file examples. lib will let you know if we find one, so i can add support for the file
	bool mbHiddenInternal = false;//internal to code such as custom types not in the telltale tool so wont be found using getmetaclassdescription functions

	String* GetToolDescriptionName(String* result);
	void Delete(void* pObj);
	void Destroy(void* pObj);
	void* New();
	void Construct(void*);
	void CopyConstruct(void*, void*);
	MetaClassDescription() : mbNameIsHeapAllocated(false), mbIsIntrinsic(false), mpExt(NULL) {}
	~MetaClassDescription();
	bool MatchesHash(u64 hash);
	void GetDescriptionSymbol(Symbol*);
	Symbol GetDescriptionSymbol();
	MetaMemberDescription* GetMemberDescription(const char* memberName);
	INLINE MetaMemberDescription* GetMemberDescription(String* _Str) {
		return GetMemberDescription(_Str->c_str());
	}
	INLINE void Initialize(const std::type_info& info) {
		//i know its slow but it doesnt need to be super  fast and i cba to change this (heap allocations/deallocations)
		char* buf = (char*)calloc(1, strlen(info.name()) + 1);
		memcpy(buf, info.name(), strlen(info.name()));
		TelltaleToolLib_MakeInternalTypeName(&buf);
		mbNameIsHeapAllocated = true;
		Initialize(buf);
	}
	INLINE void Initialize2(const char* s) {
		//i know its slow but it doesnt need to be super  fast and i cba to change this (heap allocations/deallocations)
		char* buf = (char*)calloc(1, strlen(s) + 1);
		memcpy(buf, s, strlen(s));
		TelltaleToolLib_MakeInternalTypeName(&buf);
		mbNameIsHeapAllocated = true;
		Initialize(buf);
	}
	//DO NOT USE typeid(type).name(), THIS IS NOT THE RIGHT FORMAT, use typeid(type) and use the overloaded function!!
	void Initialize(const char*);
	void Insert();
	bool IsDerivedFrom(MetaClassDescription* pDesc);
	void InstallSpecializedMetaOperation(MetaOperationDescription*);
	MetaOperation GetOperationSpecialization(int ID);
	//void CastToConcreteObject(void** pObj, MetaClassDescription** pDesc); //doesnt exist
	void* CastToBase(const void* pObj, MetaClassDescription* pBaseClassDesc);

	inline MetaMemberDescription* GetEnumMember();

};

INLINE std::string SymToString(Symbol* pSymbol) {
	return string_format("Symbol<%I64X>", pSymbol->GetCRC());
}

extern MetaClassDescription* spFirstMetaClassDescription;

struct MetaEnumDescription {
	const char* mpEnumName;
	i32 mFlags;
	i32 mEnumIntValue;
	MetaEnumDescription* mpNext;
};

struct MetaFlagDescription {
	const char* mpFlagName;
	i32 mFlagValue;
	MetaFlagDescription* mpNext;
};

MetaSerializeAccel* MetaSerialize_GenerateAccel(MetaClassDescription*);

struct MetaMemberDescription {
	const char* mpName;
	/*ony use for intrinsics for json (eg ints etc, not compound types or enums)*/
	const char* mpIntrinsicKeyOverride;
	i64 mOffset;
	i32 mFlags;
	MetaClassDescription* mpHostClass;
	MetaMemberDescription* mpNextMember;
	union {
		MetaEnumDescription* mpEnumDescriptions;
		MetaFlagDescription* mpFlagDescriptions;
	};
	MetaClassDescription* mpMemberDesc;
	i32 mMinMetaVersion;//by lib for other game support
	TRange<int> mGameIndexVersionRange;
	i32 mSkipVersion;
	inline MetaMemberDescription() {
		mMinMetaVersion = -1;
		mSkipVersion = -1;
		mpName = NULL;
		mOffset = 0;
		mFlags = 0;
		mpHostClass = NULL;
		mpNextMember = NULL;
		mpIntrinsicKeyOverride = NULL;
		mpMemberDesc = NULL;
		mpEnumDescriptions = NULL;
		mGameIndexVersionRange.min = -1;
		mGameIndexVersionRange.max = -1;
	}
	~MetaMemberDescription();
};

inline MetaMemberDescription* MetaClassDescription::GetEnumMember() {
	MetaMemberDescription* first = mpFirstMember;
	while (first) {
		if (first->mFlags & MetaFlag_EnumIntType)
			return first;
		first = first->mpNextMember;
	}
	return 0;
}

METAOP_FUNC_DEF(SerializeIntrinsicAsyncuint8)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncuint16)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncuint32)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncuint64)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncint8)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncint16)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncint32)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncint64)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncfloat)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncdouble)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncbool)
METAOP_FUNC_DEF(SerializeIntrinsicAsyncString)

//Object state meta operations are like minecrafts 'getmetaint' or whatever from block classes, gets info as an int (defults to CRC32)
METAOP_FUNC_DEF(ObjectStateIntrinsic1);
METAOP_FUNC_DEF(ObjectStateIntrinsic2);
METAOP_FUNC_DEF(ObjectStateIntrinsic4);
METAOP_FUNC_DEF(ObjectStateIntrinsic8);
METAOP_FUNC_DEF(ObjectStateIntrinsicString);

METAOP_FUNC_DEF(EquivalenceIntrinsic1);
METAOP_FUNC_DEF(EquivalenceIntrinsic2);
METAOP_FUNC_DEF(EquivalenceIntrinsic4);
METAOP_FUNC_DEF(EquivalenceIntrinsic8);
METAOP_FUNC_DEF(EquivalenceIntrinsicString);

//There are more like Arithmetic, Comparison, Interpolate, but this library doesnt need them until needed.

//Serialize or deserialize the given type. Use PerformMetaSerializeAsync which gets the meta class description for you.
//If you are using this library not as a template library (but a compiled one) then use this!
MetaOpResult PerformMetaSerializeFull(MetaStream* pStream, void* pObj, MetaClassDescription* pDesc);

template<typename T> MetaOpResult PerformMetaSerializeAsync(MetaStream* pStream, T* pObj) {
	MetaClassDescription* desc = GetMetaClassDescription(typeid(T).name());
	if (!desc || !pStream)return eMetaOp_Fail;
	return PerformMetaSerializeFull(pStream, pObj, desc);
}

namespace RenderPostMaterial {
	constexpr Symbol kPropKeyMaterialTime("Post - Material Time");
	constexpr Symbol kPropKeyMaterial("Post - Material");
	constexpr Symbol kPropKeyRenderLayer("Post - Render Layer");
}

enum T3MaterialLightModelType {
	eMaterialLightModel_Unlit = 0,
	eMaterialLightModel_VertexDiffuse = 1,
	eMaterialLightModel_Diffuse = 2,
	eMaterialLightModel_Phong = 3,
	eMaterialLightModel_PhongGloss = 4,
	eMaterialLightModel_Toon = 5,
	eMaterialLightModel_NPR_Depreceated = 6,
	eMaterialLightModel_PBS = 7,
	eMaterialLightModel_Cloth = 8,
	eMaterialLightModel_Hair = 9,
	eMaterialLightModel_Skin = 0xA,
	eMaterialLightModel_HybridCloth = 0xB,
	eMaterialLightModel_DiffuseNoDirection = 0xC,
	eMaterialLightModel_HybridHair = 0xD,
	eMaterialLightModel_Count = 0xE,
	eMaterialLightModel_Default = 0xFFFFFFFF
};

struct EnumT3MaterialLightModelType : EnumBase {
	T3MaterialLightModelType mVal;
};

namespace T3MaterialUtil {
	constexpr Symbol kPropKeyAlphaMeshCullsLines("Material - Force Linear Culls Lines");
	constexpr Symbol kPropKeyBlendMode("Material - Blend Mode");
	constexpr Symbol kPropKeyShadowCastEnable("Material - Shadow Cast Enable");
	constexpr Symbol kPropKeyCellBands("Material - Cell Bands");
	constexpr Symbol kPropKeyClothOffsetScale("Material - Cloth Offset Fix");
	constexpr Symbol kPropKeyConformNormal("Material - Conform Normal");
	constexpr Symbol kPropKeyDecalNormalOpacity("Material - Decal Normal Opacity");
	constexpr Symbol kPropKeyDoubleSided("Material - Double Sided");
	constexpr Symbol kPropKeyDoubleSidedNormals("Material - Double Sided Normals");
	constexpr Symbol kPropKeyDrawHiddenLines("Material - Draw Hidden Lines");
	constexpr Symbol kPropKeyEnlightenAlbedoColor("Material - Enlighten Albedo Color");
	constexpr Symbol kPropKeyEnlightenAlbedoIntensity("Material - Enlighten Albedo Intensity");
	constexpr Symbol kPropKeyEnlightenEmissiveColor("Material - Enlighten Emissive Color");
	constexpr Symbol kPropKeyEnlightenEmissiveIntensity("Material - Enlighten Emissive Intensity");
	constexpr Symbol kPropKeyEnlightenPrecompute("Material - Enlighten Precompute");
	constexpr Symbol kPropKeyEnlightenTransparency("Material - Enlighten Transparency");
	constexpr Symbol kPropKeyGlossExponent("Material - Gloss Exponent");
	constexpr Symbol kPropKeyGlowIntensity("Material - Glow Intensity");
	constexpr Symbol kPropKeyHairTerms("Material - Hair Terms");
	constexpr Symbol kPropKeyHorizonFade("Material - Horizon Fade");
	constexpr Symbol kPropKeyLODFullyRough("Material - LOD Fully Rough");
	constexpr Symbol kPropKeyLightGroupKey("Material - Light Group Key");
	constexpr Symbol kPropKeyLightModel("Material - Light Model");
	constexpr Symbol kPropKeyLightWrap("Material - Light Wrap");
	constexpr Symbol kPropKeyLineCreaseAngle("Material - Crease Angle");
	constexpr Symbol kPropKeyLineGenerateBoundaries("Material - Generate Boundaries");
	constexpr Symbol kPropKeyLineGenerateCreases("Material - Generate Creases");
	constexpr Symbol kPropKeyLineGenerateJagged("Material - Generate Jagged"); 
	constexpr Symbol kPropKeyLineGenerateSilhouette("Material - Generate Silhouette");
	constexpr Symbol kPropKeyLineGenerateSmooth("Material - Generate Smooth");
	constexpr Symbol kPropKeyLineLightingId("Material - Line Width Light Id");
	constexpr Symbol kPropKeyLineLightingType("Material - Line Width Lighting Type");
	constexpr Symbol kPropKeyLineMaxWidth("Material - Maximum Line Width");
	constexpr Symbol kPropKeyLineMinWidth("Material - Minimum Line Width");
	constexpr Symbol kPropKeyLinePatternRepeat("Material - Line Patttern Repeat");
	constexpr Symbol kPropKeyLineSmoothJaggedCreaseAngle("Material - Smooth/Jagged Crease Angle");
	constexpr Symbol kPropKeyLineWidthFromLighting("Material - Line Width From Lighting");
	constexpr Symbol kPropKeyLitLineBias("Material - Lit Line Bias");
	constexpr Symbol kPropKeyLitLineScale("Material - Lit Line Scale");
	constexpr Symbol kPropKeyNPRLineAlphaFalloff("Material - Line Alpha Falloff Overrides");
	constexpr Symbol kPropKeyNPRLineFalloff("Material - Line Falloff Overrides");
	constexpr Symbol kPropKeyNormalSpace("Material - Normal Space");
	constexpr Symbol kPropKeyOutlineColor("Material - Outline Color");
	constexpr Symbol kPropKeyOutlineInvertColor("Material - Outline Invert Color");
	constexpr Symbol kPropKeyOutlineZRange("Material - Outline Z Range");
	constexpr Symbol kPropKeyParticleAgent3DRotation("Material - Particle Agent 3D Rotation");
	constexpr Symbol kPropKeyParticleFaceDirection("Material - Particle Face Direction"); 
	constexpr Symbol kPropKeyParticleGeometryOrientByRotation("Material - Particle Geometry Oreint By Rotation");
	constexpr Symbol kPropKeyParticleGeometryType("Material - Particle GeometryType");
	constexpr Symbol kPropKeyParticleQuantizeLightmap("Material - Lightmap Quantize");
	constexpr Symbol kPropKeyRimLightEnable("Material - Light Rim Enable");
	constexpr Symbol kPropKeyShadowEnable("Material - Shadow Receive Enable");
	constexpr Symbol kPropKeySpecularCellBand("Material - Specular Cell Band");
	constexpr Symbol kPropKeySpecularPower("Material - Specular Power");
	constexpr Symbol kPropKeyToonShades("Material - Toon Shades");
	constexpr Symbol kPropKeyToonTexture("Material - Toon Texture");
	constexpr Symbol kPropKeyUseArtistNormal("Material - Smooth Lines From Artist Normal");
	constexpr Symbol kPropKeyVisible("Material - Visible");
	constexpr Symbol kPropKeyDiffuseTexture("Material - Diffuse Texture");
}

constexpr Symbol kAlignBottom("Bottom");
constexpr Symbol kAlignCenter("Center");
constexpr Symbol kAlignLeft("Left");
constexpr Symbol kAlignMiddle("Middle");
constexpr Symbol kNone("None");
constexpr Symbol kAlignRight("Right");
constexpr Symbol kAlignTop("Top");
constexpr Symbol kRuntimDlgLogicName("runtime_dialog_logic.prop");
constexpr Symbol kUITestCursorTexName("cursor_point.d3dtx");
constexpr Symbol kDlgStateLogicKey("all_dlg_state.prop");

namespace DlgUtils {
	constexpr Symbol kDlgSystemInfoPropName("dialog_system_info.prop");
}

constexpr Symbol kPropKeyLightInternalData("LightEnv - Internal Data");
constexpr const char*kHashUnknown = "unknown";
constexpr const char*kTelltaleBackendServerCohortKey = "Telltale Server Cohort";
constexpr const char*kTelltaleBackendServerURLDefault = "https:://services.telltalegames.com";
constexpr const char*kHDScreenShotResolution = "Screenshot resolution";

constexpr Symbol kIncomingContribution("Incoming Contribution");
constexpr Symbol kOutgoingContribution("Outgoing Contribution");
constexpr Symbol kOwningAgentKey("Owning Agent");
constexpr Symbol kStyleGuideKey("Style Guide Type");
constexpr Symbol kStyleIdleTransitionsPropKey("module_style_idle_transitions.prop");
constexpr const char*kTransitionEndTag(":End Transition");
constexpr const char*kTransitionKey("Transition");
constexpr const char*kTransitionStartTag(":Start Transition");
constexpr Symbol kPropKeyForceVisibleInEnlighten("Render Enlighten Force Visible");
constexpr Symbol kPropKeyForceVisibleInShadow("Render Shadow Force Visible");
constexpr const char*kSceneChoreCameraLayer = "Chore";
constexpr const char*k3dSoundParametersPropName = "module_sound_3d_params.prop";
constexpr Symbol kNeutralPhoneme(6675906533782001351i64);
constexpr Symbol kPropKeyBoundingVolumeScalingFactor("Bounding Volume Scaling Factor");
constexpr Symbol kPropKeyBoundingVolumeType("Bounding Volume Type");
constexpr Symbol kPropKeyCollisionType("Collision Type");
constexpr Symbol kPropKeyCollisionsEnabled("Collisions Enabled");
constexpr Symbol kComputeStage("Compute Stage");
constexpr Symbol kConstraints("Constraints");
constexpr Symbol kHostNodeKey("Host Agent Node");
constexpr Symbol kMaxAngleIncrement("Maximum Per Second Angle Increment");
constexpr Symbol kProceduralLookAtPropName("module_procedual_look_at.prop");
constexpr Symbol kRotateHostNode("Rotate Host Node");
constexpr Symbol kTargetKey("Target Agent");
constexpr Symbol kTargetNodeKey("Target Agent Node");
constexpr Symbol kTargetNodeOffKey("Target Node Agent Offset");
constexpr Symbol kUsePrivateNode("Use Private Node");
constexpr Symbol kXAxisChore("X Axis  Chore");
constexpr Symbol kYAxisChore("Y Axis Chore");

namespace Environment {
	constexpr Symbol kPropKeyEnabledOnHigh("Env - Enabled On High");
	constexpr Symbol kPropKeyEnabledOnLow("Env - Enabled On Low");
	constexpr Symbol kPropKeyEnabledOnMedium("Env - Enabled On Medium");
	constexpr Symbol kPropKeyEnabled("Env - Enabled");
	constexpr Symbol kPropKeyFogHeightFalloff("Env - Fog Height Fallofff");
	constexpr Symbol kPropKeyFogDensity("Env - Fog Density");
	constexpr Symbol kPropKeyFogColor("Env - Fog Color");
	constexpr Symbol kPropKeyFogHeight("Env - Fog Height");
	constexpr Symbol kPropKeyFogMaxOpacity("Env - Fog Max Opacity");
	constexpr Symbol kPropKeyFogStartDistance("Env - Fog Start Distance");
	constexpr Symbol kPropKeyLightGroupSet("Env - Groups");
	constexpr Symbol kPropKeyPriority("Env - Priority");
}

constexpr Symbol kPropKeyEnableCamCuts("Enable Camera Cuts");

namespace EnvironmentTile {
	constexpr Symbol kPropKeyLightProbeData("EnvTile - Light Probe Data");
	constexpr Symbol kPropKeyReflectionLocalEnable("EnvTile - Reflection Local Enable");
	constexpr Symbol kPropKeyReflectionTexture("EnvTile - Reflection Texture");
}

namespace EnvironmentLight {
	constexpr Symbol kPropKeyAllowBaseOnStatic("EnvLight - Bake Allowed on Static");
	constexpr Symbol kPropKeyColor("EnvLight - Color");
	constexpr Symbol kPropKeyDiffuseIntensity("EnvLight - Intensity Diffuse");
	constexpr Symbol kPropKeyDimmer("EnvLight - Intensity Dimmer");
	constexpr Symbol kPropKeyDistanceFalloff("EnvLight - Distance Falloff");
	constexpr Symbol kPropKeyEnabled("EnvLight - Enabled");
	constexpr Symbol kPropKeyEnlightenBakeBehaviour("EnvLight - Enlighten Bake Behaviour");
	constexpr Symbol kPropKeyEnlightenLightIntensity("EnvLight - Enlighten Intensity");
	constexpr Symbol kPropKeyGroupEnabled("EnvLight - Enabled Group");
	constexpr Symbol kPropKeyHBAOParticipationType("EnvLight - HBAO Participation Type");
	constexpr Symbol kPropKeyInnerConeAngle("EnvLight - Spot Angle Inner");
	constexpr Symbol kPropKeyIntensity("EnvLight - Intensity");
	constexpr Symbol kPropKeyInternalData("__T3LightEnvInternalData__");
	constexpr Symbol kPropKeyLODBehaviour("EnvLight - LOD Behaviour");
	constexpr Symbol kPropKeyLOD("EnvLight - LOD Active");
	constexpr Symbol kPropKeyLightGroupSet("EnvLight - Groups");
	constexpr Symbol kPropKeyLocalPosition("EnvLight - Local Position");
	constexpr Symbol kPropKeyLocalRotation("EnvLight - Local Rotation");
	constexpr Symbol kPropKeyMobility("EnvLight - Mobility");
	constexpr Symbol kPropKeyNPRBandThresholds("EnvLight - NPR Band Thresholds");
	constexpr Symbol kPropKeyNPRBanding("EnvLight - NPR Banding");
	constexpr Symbol kPropKeyOpacity("EnvLight - Opacity");
	constexpr Symbol kPropKeyOuterConeAngle("EnvLight - Spot Angle Outer");
	constexpr Symbol kPropKeyPriority("EnvLight - Priority");
	constexpr Symbol kPropKeyRadius("EnvLight - Radius");
	constexpr Symbol kPropKeyShadowDepthBias("EnvLight - Shadow Depth Bias");
	constexpr Symbol kPropKeyShadowGoboName("EnvLight - Shadow Gobo");
	constexpr Symbol kPropKeyShadowGoboScaleU("EnvLight - Shadow Gobo Scale U");
	constexpr Symbol kPropKeyShadowGoboScaleV("EnvLight - Shadow Gobo Scale V");
	constexpr Symbol kPropKeyShadowGoboTranslateU("EnvLight - Shadow Gobo Translate U");
	constexpr Symbol kPropKeyShadowGoboTranslateV("EnvLight - Shadow Gobo Translate V");
	constexpr Symbol kPropKeyShadowMapQualityDistanceScale("EnvLight - Shadow Map Quality Distance Scale");
	constexpr Symbol kPropKeyShadowModulatedIntensity("EnvLight - Shadow Modulated Intensity");
	constexpr Symbol kPropKeyShadowNearClip("EnvLight - Shadow Near Clip");
	constexpr Symbol kPropKeyShadowQuality("EnvLight - Shadow Quality");
	constexpr Symbol kPropKeyShdowSoftness("EnvLight - Shadow Softness");
	constexpr Symbol kPropKeyShadowType("EnvLight - Shadow Type");
	constexpr Symbol kPropKeyVisibleThresholdScale("EnvLight - Visible Threshold Scale");
	constexpr Symbol kPropKeyWrap("EnvLight - Wrap");
}

namespace EnvironmentLightGroup {
	constexpr Symbol kPropKeyEnabled("EnvLightGroup - Enabled");
	constexpr Symbol kPropKeyEnlightenLightIntensity("EnvLightGroup - Enlighten Intensity");
	constexpr Symbol kPropKeyLightGroupSet("EnvLightGroup - Groups");
	constexpr Symbol kPropKeyPriority("EnvLightGroup - Priority");
}

enum MeshDebugRenderType {
	eMeshRender_Solid = 1,
	eMeshRender_Wireframe = 2
};

struct EnumMeshDebugRenderType : EnumBase {
	MeshDebugRenderType mVal;
};

namespace CinematicLight {
	constexpr Symbol kPropKeyColor("CinLight - Color");
	constexpr Symbol kPropKeyDiffuseIntensity("CinLight - Intensity Diffuse");
	constexpr Symbol kPropKeyDimmer("CinLight - Dimmer");
	constexpr Symbol kPropKeyEnabled("CinLight - Enabled");
	constexpr Symbol kPropKeyHBAOParticipationType("CinLight - HBAO Participation Type");
	constexpr Symbol kPropKeyIntensity("CinLight - Intensity");
	constexpr Symbol kPropKeyNPRBandThresholds("CinLight - NPR Band Thresholds");
	constexpr Symbol kPropKeyNPRBanding("CinLight - NPR Banding");
	constexpr Symbol kPropKeyOpacity("CinLight - Opacity");
	constexpr Symbol kPropKeyShadowIntensity("CinLight - Shadow Intensity");
	constexpr Symbol kPropKeyShadowTraceLength("CinLight - Shadow Trace Length");
	constexpr Symbol kPropKeySpecularIntensity("CinLight - Intensity Specular");
	constexpr Symbol kPropKeyWrap("CinLight - Wrap");
}

namespace CinematicLightRig {
	constexpr Symbol kPropKeyBackFOV("CinRig - Back FOV");
	constexpr Symbol kPropKeyEnlightenIntensity("CinRig - Enlighten Intensity");
	constexpr Symbol kPropKeyEnlightenSaturation("CinRig - Enlighten Saturation");
	constexpr Symbol kPropKeyLOD("CinRig - LOD");
	constexpr Symbol kPropKeyLightCinIntensity("CinRig - Light Cin Intensity");
	constexpr Symbol kPropKeyLightEnvIntensity("CinRig - Intensity EnvLight");
	constexpr Symbol kPropKeyLightNameFill("CinRig - Light Agent Fill");
	constexpr Symbol kPropKeyLightNameKey("CinRig - Light Agent Key");
	constexpr Symbol kPropKeyNameRim("CinRig - Light Agent Rim");
	constexpr Symbol kPropKeySelfShadowing("CinRig - Shadow Enable");
}

namespace ChorecordingParameters {
	constexpr Symbol kChorecordingCutName("Chorecording");
}

namespace SoundSystemInternal {
	constexpr Symbol kAudioAgentPrefix("agent_");
	constexpr Symbol kAudioLogicPrefix("logic_");
	constexpr Symbol kBusAmbient("ambient");
	constexpr Symbol kBusMaster("master");
	constexpr Symbol kBusMusic("music");
	constexpr Symbol kBusSfx("sfx");
	constexpr Symbol kVox("voice");
	constexpr Symbol kMasterBusFileName("master.audiobus");
	constexpr Symbol kSoundDataPropName("module_sound_data.prop");
}

struct Agent {

	static inline constexpr Symbol kGroupVisibilityKey = Symbol("Group - Visible");
	static inline constexpr Symbol kRuntimeVisibilityKey = Symbol("Runtime: Visible");

	char __r;

};

namespace TTSQL {
	constexpr Symbol kDatabaseNameKey("SQL Database Name");
	constexpr Symbol kDatabasePasswordKey("SQL Password");
	constexpr Symbol kDatabaseUserNameKey("SQL User Name");
	constexpr Symbol kLocalHost("localhost");
	constexpr Symbol kPortKey("SQL Port");
	constexpr Symbol kServerNameKey("SQL Server Name");
	constexpr Symbol kTunnelHost("SSH Tunnel Host");
	constexpr Symbol kTunnelPassword("SSH Tunnel Password");
	constexpr Symbol kTunneledUsername("SSH Tunnel Username");
	constexpr Symbol kTunneledServerName("SSH Tunneled Server Name");
	constexpr Symbol kTunnelingEnabled("SSH Tunneling Enabled");
	constexpr Symbol kUseLocalDataKey("SQL Use Only Local Database");
}

constexpr Symbol kPropKeyCurElemIndex("cur elem index");
constexpr Symbol kPropKeyElemData("elem data");
constexpr Symbol kPropKeyElemUses("elem uses in cycle");
constexpr Symbol kPropKeyFinalShuffe("final shuffle");
constexpr Symbol kPropKeyFinished("finished");
constexpr Symbol kPropKeyNumCompleteCycles("complete cycles");
constexpr Symbol kPropKeyVisDiesOff("vis dies off");

namespace T3MaterialInternal {
	constexpr Symbol kPropKeyExpressionTree("__T3MaterialExpressionTree__");
	constexpr Symbol kPropKeyLegacyParams("__T3LegacyMaterialParams__");
	constexpr Symbol kPropKeyMaterialData("__T3MaterialData__");
}

namespace RenderDecal {
	constexpr Symbol kPropKeyDoubleSided("Decal - Double Sided");
	constexpr Symbol kPropKeyMaterialTime("Decal - Material Time");
	constexpr Symbol kPropKeyMaterial("Decal - Material");
	constexpr Symbol kPropKeyNormalThreshold("Decal - Normal Threshold");
	constexpr Symbol kPropKeyRenderLayer("Decal - Render Layer");
	constexpr Symbol kPropKeyScale("Decal - Scale");
	constexpr Symbol kPropKeySize("Decal - Size");
	constexpr Symbol kPropKeyStatic("Decal - Static");
	constexpr Symbol kPropKeyVisibileThresholdScale("Decal - Visible Threshold Scale");
}

struct ScriptEnum {

	constexpr static const char* ENUM_LIGHT_COMPOSER_NODE_LOCATION = "ScriptEnum:LightComposerNodeLocation";
	constexpr static const char* ENUM_TEXT_COLOUR_STYLE = "ScriptEnum:TextColorStyle";
	constexpr static const char* ENUM_GAMEPAD_BUTTON = "ScriptEnum:GamepadButton";
	constexpr static const char* ENUM_LIGHT_COMPOSER_LIGHT_SOURCE_QUADRANT = "ScriptEnum:LightComposerLightSourceQuadrant";
	constexpr static const char* ENUM_LIGHT_COMPOSER_CAMERA_ZONE = "ScriptEnum:LightComposerCameraZone";
	constexpr static const char* ENUM_AI_DUMMY_POSITION = "ScriptEnum:AIDummyPos";
	constexpr static const char* ENUM_BLEND_TYPE = "ScriptEnum:BlendTypes";
	constexpr static const char* ENUM_RETICLE_ACTIONS = "ScriptEnum:ReticleActions";
	constexpr static const char* ENUM_STRUGGLE_TYPE = "ScriptEnum:StruggleType";
	constexpr static const char* ENUM_AI_PATROL_TYPE = "ScriptEnum:AIPatrolType";
	constexpr static const char* ENUM_MENU_ALIGN = "ScriptEnum:MenuAlignment";
	constexpr static const char* ENUM_MENU_VERTICAL_ALIGN = "ScriptEnum:MenuVerticalAlignment";
	constexpr static const char* ENUM_UI_COLOUR = "ScriptEnum:UIColor";
	constexpr static const char* ENUM_CHASE_FORWARD_VECTOR = "ScriptEnum:ChaseForwardVector";
	constexpr static const char* ENUM_RETICLE_DISPLAY_MODE = "ScriptEnum:ReticleDisplayMode";
	constexpr static const char* ENUM_CONTROLLER_BUTTONS = "ScriptEnum:ControllerButtons";
	constexpr static const char* ENUM_DIALOG_MODE = "ScriptEnum:DialogMode";
	constexpr static const char* ENUM_USEABLE_TYPE = "ScriptEnum:UseableType";
	constexpr static const char* ENUM_AI_AGENT_STATE = "ScriptEnum:AIAgentState";
	constexpr static const char* ENUM_QUICK_TIME_EVENT_TYPE = "ScriptEnum:QTE_Type";

	String mCurValue;
};

struct BinaryBuffer {

	char* mpData;
	int mDataSize;

	BinaryBuffer() : mpData(NULL), mDataSize(0) {}

	~BinaryBuffer() {
		if (mpData)
			delete[] mpData;
	}

	bool SetData(int dataSize, const void* pData) {
		if (mpData)
			delete[] mpData;
		if (dataSize > 0) {
			mpData = (char*)operator new[](dataSize);
			if (mpData) {
				mDataSize = dataSize;
				if (pData)
					memmove(mpData, pData, mDataSize);
				else
					memset(mpData, 0, mDataSize);
			}
			else return false;
		}
		return true;
	}

	void Memset(char v) {
		if (mpData)
			memset(mpData, v, mDataSize);
	}

	void Swap(BinaryBuffer& other) {
		char* my = mpData;
		mpData = other.mpData;
		other.mpData = my;
	}

	void Clear() {
		if (mpData)
			delete[] mpData;
	}

	bool AllocData(/*u64*/ int dataSize) {
		if (mpData)
			delete[] mpData;
		if (dataSize > 0) {
			mpData = (char*)operator new[](dataSize);
			if (mpData) {
				mDataSize = dataSize;
			}
			else return false;
		}
		return true;
	}

	BinaryBuffer(const BinaryBuffer& other) {
		mpData = NULL;
		mDataSize = 0;
		SetData(other.mDataSize, other.mpData);
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDesc, 
			MetaMemberDescription* pCtx, void* pUserData) {
		MetaStream* stream = static_cast<MetaStream*>(pUserData);
		BinaryBuffer* bb = static_cast<BinaryBuffer*>(pObj);
		u32 size = bb->mDataSize;
		stream->serialize_uint32(&size);
		bb->mDataSize = size;
		if (stream->mMode == MetaStreamMode::eMetaStream_Read) {
			if (bb->mpData)
				delete[] bb->mpData;
			bb->mpData = new char[size];
			if (!bb->mpData) {
				stream->Advance(size);
				return eMetaOp_OutOfMemory;
			}
			memset(bb->mpData, 0, size);
		}
		stream->serialize_bytes(bb->mpData, size);
		return eMetaOp_Succeed;
	}

};

//.T3BULLET FILES
struct PhysicsData {
	BinaryBuffer mDataBuffer;
};

#include "Types/TRange.h"

namespace Acting {

	constexpr TRange<float> kSecBetweenLinesRange{ 0.25,0.35 };
	constexpr TRange<float> kProcLookatSpeakerDelayRange{ 0.0f, 0.15f };
	constexpr TRange<float> kProcLookAtDefaultBeginDelayRange{ 0.25f,0.75f };
	constexpr TRange<float> kProcLookAtDefaultEndDelayRange{ 0.08f,0.25f };
	constexpr TRange<float> kProcLookAtFinalEndDelayRange{ 0.0f,0.2f };
	constexpr TRange<float> sContributionScaleRangeDef{ 1.0f,1.0f };
	constexpr TRange<float> sAnimScaleRangeDef{ 1.0f,1.0f };
	constexpr Symbol kBaseLookAtPriKey{ 0x79E37DC8F4EA91E5 };
	constexpr Symbol kLookAtPriIncrementKey{ 0x0A398C78DEBB2D503 };
	constexpr Symbol kLookAtFadeTimeKey{ 0x31E84EFD0F4F164A };
	constexpr Symbol kLookAtGenerateInProjectKey{ 0x3619B30FCD406D18 };
	constexpr Symbol kPrefUseLegacyLookAtMaxAngle{ 0x9906F5914B07805F };
	constexpr Symbol kNoMoverDataInIdlesKey{ 0x85E2318F7535F345 };
	constexpr Symbol kTalkingDefaultMinInitialDelayKey{ 0x987C4BFD3365F7B4 };
	constexpr Symbol kListeningDefaultMinInitialDelayKey{ 0x6F44D84461CEEE36 };
	constexpr Symbol kAlwaysDefaultMinInitialDelayKey{ 0x37DB3E605513B158 };
	constexpr Symbol kTalkingDefaultMaxInitialDelayKey{ 0x6F001ADEABBDD403 };
	constexpr Symbol kListeningDefaultMaxInitialDelayKey{ 0x9D3C011E171914F7 };
	constexpr Symbol kAlwaysDefaultMaxInitialDelayKey{ 0x7DE1C03C3E2E54EE };
	constexpr Symbol kAnimFadeTimeDefKey{ 0x3AE879EB1625184D };
	constexpr Symbol kLookatStrengthKey{ 0x0EE55C02999ED1F98 };
	constexpr Symbol kAnimPreDelayDefKey{ 0x17C95DFE007ECEB2 };
	constexpr Symbol kAnimPostDelayDefKey{ 0x9A627A4316571A81 };
	constexpr Symbol kAnimPriorityDefKey{ 0x0E204FD2E718CCA1B };
	constexpr Symbol kAnimBlendingDefKey{ 0x0C1E314190D01F26 };
	constexpr Symbol kAnimScaleRangeDefKey{ 0x632C08E64BE46194 };
	constexpr Symbol kContributionScaleRangeDefKey{ 0x6DA3E623CF54498A };
	constexpr Symbol kPauseCommandDefKey{ 0x0FB9EDBBE2BE479B2 };
	constexpr Symbol kValidateEmbeddedCommandsKey{ 0x1CD8B1FCD7E493C8 };
	constexpr Symbol kStyleIdleTransitionTimeKey{ 0x6BDE8E8578A2708F };
	constexpr Symbol kStyleIdleTransitionInTimeOverrideKey{ 0x0A88B3F9F7218A160 };
	constexpr Symbol kStyleIdleTransitionOutTimeOverrideKey{ 0x0B8DEF12AFA1DAB4A };
	constexpr Symbol kStyleBaseIdleTransitionTimeKey{ 0x3974E6A76464C68A };
	constexpr Symbol kValidateVoicePreAAKey{ 0x0EA4E8DD73A00249D };
	constexpr Symbol kUseNewActingKey{ 0x1D7F3F7C5E624254 };
	constexpr Symbol kDisplayLookAtDebugDataKey{ 0x0FB304862D67CE4A4 };
	constexpr Symbol kDisplayDebugPathKey{ 0x0DDE767E8D498D4D1 };
	constexpr Symbol kSetDefaultIntensityKey{ 0x0E29CC88BC83A9DA8 };
	constexpr Symbol kDefaultIntensityValue{ 0x8B0C232138FF0D8A };
	constexpr Symbol kSetDefaultAccentTags{ 0x7F3B16B6CB2F79FE };
	constexpr Symbol kFixPopInAdditiveIdleTransition{ 0x0B72FDFD28B8C051F };
	constexpr Symbol kTalkingIdleTransitionTimeKey{ 0x12C9352DE9F6D9B3 };
	constexpr Symbol kActingIntensityKey{ 0x0DE48259EB8929B6 };
	constexpr Symbol kActingFaceAccentKey{ 0xCBAE23CE3F2B0CCE };
	constexpr Symbol kActingBodyAccentKey{ 0xEFFC9824AB71EF3D };
	constexpr Symbol kActingHead1AccentKey{ 0x7D37DAB358DDE48C };
	constexpr Symbol kActingHead2AccentKey{ 0xC86CE44964FFF54B };
	constexpr Symbol kLookAtStopKey{ 0x66E772BA8E023A4D };
	constexpr Symbol kGenerateAccentsFromLanguageResources{ 0x73AF59848641A183 };
	constexpr Symbol kEnableNewLookats{ 0x69967C61AE2C7675 };
	constexpr Symbol kUseCurvedPath{ 0x8A29C4B8ACC1A104 };
	constexpr Symbol kLegacyEnableTargetedBlockingOnAttachments{ 0x6B8EE0DEAD524B98 };
	constexpr Symbol kLegacyUseOldBGMIdleBehavior{ 0x0E35173FA6BB35AD1 };
	constexpr Symbol kPriorityKey{ 0x19E1083970E1804D };
	constexpr Symbol kFadeTimeKey{ 0x5252B0398028EEE7 };
	constexpr Symbol kPreDelayKey{ 0x49AE373C8672B756 };
	constexpr Symbol kPostDelayKey{ 0x5EAA376E1C28BFEB };
	constexpr Symbol kBlendingKey{ 0x8CD42C9E0BD6AD43 };
	constexpr Symbol kScaleRangeKey{ 0x0D07B6742C1C50C1F };
	constexpr Symbol kStyleMumbleMouthKey{ 0x7D10D6DE5CE9F014 };
	constexpr Symbol kContributionRangeKey{ 0x151FE38B3A415A9D };
	constexpr Symbol kResourceGroupsKey{ 0x89CC3329DF3EC278 };
	constexpr Symbol kStartOffsetRangeKey{ 0x54BF2651351D34B9 };
	constexpr Symbol kStyleTransitionTimeKey{ 0x0B7535A357B22778 };
	constexpr Symbol kPropertyKeyBlockOverrunPercentage{ 0x38AA965019501AF9 };
	constexpr Symbol kPropertyKeyIntensityContributionMultiplierRange{ 0x9525F5C22FC0F5D1 };
	constexpr Symbol kPropertyKeyIntensityScaleMultiplierRange{ 0x272FA01DF8115368 };
	constexpr Symbol kPropertyKeyValidIntensityRange{ 0x6C92A5EB51A06379 };
	constexpr Symbol kPropertyKeyPaletteValidIntensityRange{ 0xDB68FF2D8AE9697B };
	constexpr Symbol kPropertyKeyIntensityTimeBetweenActionsMultiplierRange{ 0x8A21C545508159E };
	constexpr Symbol kRuntimeApplyChoreGenConflictToAllKey{ 0x6890F50FAF563600 };
	constexpr Symbol kRuntimeChoreGenConflictActionKey{ 0x345FF590FDEC8D01 };
}


template<int N>
struct __BitSet_BaseN {
	static constexpr int _TyN = (N / 32) + (N % 32 == 0 ? 0 : 1);
};

template<int N>
struct BitSetBase {
	u32 mWords[N];

	static constexpr int NumWords = N;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		for (int i = 0; i < N; i++) {
			((MetaStream*)pUserData)->serialize_uint32(((BitSetBase<N>*)pObj)->mWords + i);
		}
		return eMetaOp_Succeed;
	}

};

template<typename T, int Num, int InitialVal = 0>
struct BitSet : BitSetBase<__BitSet_BaseN<Num>::_TyN> {

	static constexpr int EnumValCount = Num;
	static constexpr int InitialEnumVal = InitialVal;

	virtual bool operator[](T f) {
		unsigned int v = (unsigned int)f - InitialVal;
		return (this->mWords[v >> 5] & (1 << (v & 0x1F))) != 0;
	}

	virtual void Clear(T f) {
		Set(f, false);
	}

	virtual void Set(T f, bool bValue) {
		unsigned int v = (unsigned int)f - InitialVal;
		if (bValue)
			this->mWords[v >> 5] |= (1 << (v & 0x1F));
		else
			this->mWords[v>>5] &= ~(1 << (v & 0x1F));
	}

	/*import bits*/
	virtual void Set(const BitSet& rhs) {
		for (int i = 0; i < Num; i++) {
			this->mWords[i] |= rhs.mWords[i];
		}
	}

	virtual void GetTypeInformation(MetaClassDescription** pDestType, int* pDestNum, int* pDestInitial){
		*pDestInitial = InitialEnumVal;
		*pDestNum = Num;
		*pDestType = GetMetaClassDescription<T>();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		for (int i = 0; i < BitSetBase::NumWords; i++) {
			((MetaStream*)pUserData)->serialize_uint32(((BitSet*)pObj)->mWords + i);
		}
		return eMetaOp_Succeed;
	}

};

inline const char* GetMetaEnumName(MetaMemberDescription* pMem, int enumVal) {
	MetaEnumDescription* pEnum = pMem->mpEnumDescriptions;
	while (pEnum) {
		if (pEnum->mEnumIntValue == enumVal)
			return pEnum->mpEnumName;
		pEnum = pEnum->mpNext;
	}
	return "";
}

inline const char* GetMetaFlagName(MetaMemberDescription* pMem, int enumVal) {
	MetaFlagDescription* pEnum = pMem->mpFlagDescriptions;
	while (pEnum) {
		if (pEnum->mFlagValue == enumVal)
			return pEnum->mpFlagName;
		pEnum = pEnum->mpNext;
	}
	return "";
}

/*
* Tries to find the symbol name for the given symbol. Make sure the global hash database is set. This is will try search all pages in the 
* current game set (using set blowfish key). This can be slow! If the value is found, the outpageref pointer is set to the page it was
* found in in case you want to find more symbol names in that page. Returns <NotFound> if not found, or <Empty> if the CRC is 0.
* Use once the library is initialized.
*/
String FindSymbolName(const Symbol&, HashDatabase::Page*& outpageref);

/*
* Gets the constant list of file extension strings that the library currently supports.
*/
const std::vector<const char*>* GetMetaFileExtensions();

namespace RecordingUtils {

	enum ScriptType {
		STScript = 1,
		STActor = 2,
		STEngineer = 3,
		STTakeSheet = 4
	};

	enum RecordingStatus {
		eRecordingStatus_None = 0,
		eRecordingStatus_AtStudio = 1,
		eRecordingStatus_Recorded = 2,
		eRecordingStatus_Delivered = 3
	};

	struct EnumRecordingStatus : public EnumBase {
		RecordingStatus mVal;
	};

	struct EnumScriptType : public EnumBase {
		ScriptType mVal;
	};

}

_TTToolLib_Exp MetaStream* TelltaleToolLib_CreateAndOpenMetaStream(const char* pFileMemory, int fileSize);
/*returns the new meta stream file data, poutsize contains the size. deletes pStream. IF outsize == nullptr, then assuming you dont want the data*/
_TTToolLib_Exp void* TelltaleToolLib_DeleteAndFlushMetaStream(MetaStream* pStream, u64* pOutSize);
_TTToolLib_Exp MetaOpResult TelltaleToolLib_SerializeMetaStream(MetaStream* pStream, MetaClassDescription* pClass, void* pClassInstance);
_TTToolLib_Exp void TelltaleToolLib_MetaStreamSwitchToWrite(MetaStream* pStream);

template<typename T>
MetaClassDescription* GetMetaClassDescription() {
	MetaClassDescription* clazz = TelltaleToolLib_GetFirstMetaClassDescription();
	const char* tn = typeid(T).name();
	while (clazz != NULL) {
		if (!_stricmp(tn, clazz->mpTypeInfoExternalName))
			return clazz;
		TelltaleToolLib_GetNextMetaClassDescription(&clazz);
	}
	return NULL;
}

struct IdleTransitionSettings
{
	float mTransitionTime;
	int mTransitionStyleDummy;
};

struct IdleSlotDefaults
{
	IdleTransitionSettings mTransitionInDefault;
	IdleTransitionSettings mTransitionDefault;
	IdleTransitionSettings mTransitionOutDefault;
	int mBasePriority;
	Symbol mSlotFriendlyName;
};


struct FileNameBase {
	Symbol mFileName;
	//MetaClassDescription* mpMetaClassDescription;
};

template<typename T>
struct FileName : FileNameBase {

	inline bool operator<(const FileName& rhs) const {
		return mFileName < rhs.mFileName;
	}

};


#endif
