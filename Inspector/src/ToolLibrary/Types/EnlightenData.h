// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _ENL
#define _ENL

#include "DCArray.h"
#include <emmintrin.h>
#include "../Meta.hpp"
#include "../TelltaleToolLibrary.h"
#include "ObjectSpaceIntrinsics.h"

struct EnlightenSignature {
	u64 mSignature;
	u32 mMagicNumber, mVersion;
};

namespace Enlighten {

	struct RadProbeSetMetaData {
		Geo::GeoGuid m_ProbeSetId;
		int m_NumProbes;
		int m_NumProbesOutput;
		int m_SHOrder;
		int m_SHOverEnvVis;
		int m_MaxBudget;
		int m_MaxProbeSize;
		int m_RequiredWorkspaceSize;
		int m_RequiredTemporalCoherenceBufferSize;
		int m_EnvironmentResolution;
		unsigned __int16 m_IsRegularGrid;
		unsigned __int16 m_RegularGridResolutionX;
		unsigned __int16 m_RegularGridResolutionY;
		unsigned __int16 m_RegularGridResolutionZ;
		unsigned int m_Padding;
		__m128d m_BoundingBoxOrigin;
		__m128d m_BoundingBoxBasisX;
		__m128d m_BoundingBoxBasisY;
		__m128d m_BoundingBoxBasisZ;
	};

	enum eRadDataType : unsigned __int16 {
		RDT_INVALID = 0x0,
		RDT_RAD_CORE = 0x1,
		RDT_RAD_PROJECTION_DATA = 0x2,
		RDT_INPUT_WORKSPACE = 0x4,
		RDT_PROBE_SET_CORE = 0x5,
		RDT_CAW_CLUSTER_MATERIALS = 0x6,
		RDT_TAW_MATERIAL_RUNS = 0x7,
		RDT_AW_MATERIAL_GUIDS = 0x8,
		RDT_INPUT_LIGHTING_BUFFER = 0x9,
		RDT_RAD_CUBE_MAP_CORE = 0xA, 
		RDT_INPUT_LIGHTING_CUBE_MAP_CORE = 0xB,
		RDT_INPUT_PROJECTED_POINTS = 0xF,
		RDT_INPUT_SOURCE_MESH_DATA = 0x10,
		RDT_PROBE_SET_OCTREE = 0x11,
		RDT_PROBE_SET_INTERPOLATION = 0x12,
		RDT_PROBE_SET_VISIBILITY = 0x13,
		RDT_PROBE_SET_OCTREE_VISIBILITY = 0x14,
		RDT_INPUT_HQ_BOUNCE_COORDINATES = 0x15,
		RDT_INPUT_CLUSTER_PROBE_OFFSETS = 0x16,
		RDT_INPUT_CLUSTER_PROBE_BOUNCE_DATA = 0x17,
		RDT_ENTIRE_PROBE_SET_CORE = 0x18,
		RDT_CLUSTER_ALBEDO_WORKSPACE = 0x19,
		RDT_PROBE_SET_DEBUG_DATA = 0x1A,
		RDT_RAD_RESAMPLING_DATA = 0x1B,
		RDT_ALBEDO_BUFFER = 0x1C,
		RDT_EMISSIVE_BUFFER = 0x1D,
		RDT_INCIDENT_LIGHTING_BUFFER = 0x1E,
		RDT_DYNAMIC_MATERIAL_WORKSPACE = 0x1F,
		RDT_TRANSPARENCY_BUFFER = 0x20, 
		RDT_RAD_CORE_BOUNCE = 0x21,
	};

	enum eSolverType : unsigned __int16 {
		SOLVER_REFERENCE = 0,
		SOLVER_SEE = 1,
		SOLVEN_NEON = 2
	};

	struct RadDataBlock {
		union {
			void* m_DataStart;
			unsigned __int64 m_DataStartU64;
		};
		int m_Length;
		eRadDataType m_DataType;
		eSolverType m_SolverType;
	};

	struct RadProbeSetCore {
		RadProbeSetMetaData m_MetaData;
		RadDataBlock m_ProbeSetPrecomp;
		RadDataBlock m_EntireProbeSetPrecomp;
		RadDataBlock m_InterpolationData;
		RadDataBlock m_VisibilityData;
		RadDataBlock m_DebugData;
	};

}

struct EnlightenSystemData {
	Symbol mName;
	Symbol mEnvTileName;
	//u64 mpRadCore;
	//u64 mpInputWorkspace;
	//u64 mpCAWMaterialData;
	//u64 mpPrecomputedVisibilityData;
	BinaryBuffer mRadSystemCore;
	BinaryBuffer mInputWorkspace;
	BinaryBuffer mClusterAlbedoWorkspaceMaterial;
	BinaryBuffer mPrecomputedVisibility;
};

struct EnlightenProbeData {
	Symbol mEnvTileName;
	Enlighten::RadProbeSetCore mpRadProbeSetCore;//ptr but not in lib. this is the format of the data below
	BinaryBuffer mRadProbeSetCore;
};

//.ENL FILES
struct EnlightenData {
	EnlightenSignature mSignature;
	String mName;
	DCArray<EnlightenSystemData> mSystemData;
	DCArray<EnlightenProbeData> mProbeData;
};

struct EnlightenModule {

	struct EnlightenCubemapSettings {
		int mFaceWidth;
		Vector3 mBoxOrigin;
	};

	enum eProbeResolution : int
	{
		ProbeResolution_Full = 0x0,
		ProbeResolution_1_2 = 0x1,
		ProbeResolution_1_4 = 0x2,
		ProbeResolution_1_8 = 0x3,
	};

	struct EnumeProbeResolution : EnumBase {
		eProbeResolution mVal;
	};

	enum eQuality : int {
		Quality_Background = 0,
		Quality_Low = 1,
		Quality_Medium = 2,
		Quality_High = 3,
		Quality_Auto = 4,
	};

	struct EnumeQuality : EnumBase {
		eQuality mVal;
	};

	struct EnlightenAdaptiveProbeVolumeSettings {
		EnumeQuality mQuality;
	};

	struct EnlightenAutoProbeVolumeSettings {
		EnumeQuality mQuality;
		int mMinProbeSetSize;
		float mMergingThreshold;
		int mMinProbeSpacing, mMaxProbeSpacing;
	};

	struct __declspec(align(4)) EnlightenLightSettings
	{
		float mEnlightenSaturation;
		bool mEnlightenOnly;
		bool mCastDynamicEnlightenShadows;
	};

	struct EnlightenProbeVolumeSettings {
		EnumeQuality mQuality;
		String mLightmapType;
		Vector3 mResolution;
	};

	struct EnlightenSystemSettings {
		EnumeQuality mDefaultQuality;
		EnumeProbeResolution mAdaptiveProbeResolution;
		bool mDisableEnlighten;
	};

	enum eInstanceType
	{
		InstanceType_Radiosity = 0x0,
		InstanceType_StaticSetDressing_Unused = 0x1,
		InstanceType_FullyDynamic = 0x2,
		InstanceType_ProbeRadiosity = 0x3,
		InstanceType_Auto = 0x4,
		InstanceType_Disabled = 0x5,
	};

	enum eUpdateMethod
	{
		UpdateMethod_Dynamic = 0x0,
		UpdateMethod_Static = 0x1,
		UpdateMethod_Auto = 0x2,
	};

	enum eQualityWithDefault
	{
		QualityWithDefault_Background = 0x0,
		QualityWithDefault_Low = 0x1,
		QualityWithDefault_Medium = 0x2,
		QualityWithDefault_High = 0x3,
		QualityWithDefault_Auto = 0x4,
		QualityWithDefault_UseDefault = 0x5,
	};

	/* 3504 */
	struct EnumeUpdateMethod : EnumBase
	{
		EnlightenModule::eUpdateMethod mVal;
	};

	struct  EnumeInstanceType : EnumBase
	{
		EnlightenModule::eInstanceType mVal;
	};

	struct EnumeQualityWithDefault : EnumBase
	{
		EnlightenModule::eQualityWithDefault mVal;
	};

	struct EnlightenPrimitiveSettings
	{
		String mSystemId;
		EnumeInstanceType mEnlightenLightingMode;
		EnumeUpdateMethod mEnlightenUpdateMethod;
		EnumeQualityWithDefault mEnlightenQuality;
	};

	enum eDistributedBuildSystem
	{
		DistributedBuildSystem_None = 0x0,
		DistributedBuildSystem_SN_DBS = 0x1,
		DistributedBuildSystem_Incredibuild = 0x2,
	};

	enum eSceneOptimisationMode
	{
		SceneOptimisationMode_None = 0x0,
		SceneOptimisationMode_EqualPixelArea = 0x1,
		SceneOptimisationMode_Voxelisation = 0x2,
	};

	enum eBackfaceType
	{
		BackfaceType_EBT_Auto = 0x0,
		BackfaceType_EBT_Invalid = 0x1,
		BackfaceType_EBT_Black = 0x2,
		BackfaceType_EBT_Transparent = 0x3,
		BackfaceType_EBT_Double_Sided = 0x4,
	};

	enum eAutoUVSimplificationMode
	{
		AutoUVSimplificationMode_EAUM_Automatic = 0x0,
		AutoUVSimplificationMode_EAUM_Override = 0x1,
		AutoUVSimplificationMode_EAUM_Disable = 0x2,
	};

	enum eProbeSampleMethod
	{
		ProbeSampleMethod_UseBounds = 0x0,
		ProbeSampleMethod_ForceSingle = 0x1,
		ProbeSampleMethod_ForceMultiple = 0x2,
	};

	enum eDisplayQuality
	{
		DisplayQuality_Low = 0x0,
		DisplayQuality_Medium = 0x1,
		DisplayQuality_High = 0x2,
	};

	enum eRadiositySampleRate
	{
		RadiositySampleRate_Low = 0x0,
		RadiositySampleRate_Medium = 0x1,
		RadiositySampleRate_High = 0x2,
		RadiositySampleRate_VeryHigh = 0x3,
	};

	enum eUpdateMethodWithDefault
	{
		UpdateMethodWithDefault_Dynamic = 0x0,
		UpdateMethodWithDefault_Static = 0x1,
		UpdateMethodWithDefault_UseLevelDefault = 0x2,
	};

	enum eProbeResolutionWithDefault
	{
		ProbeResolutionWithDefault_Full = 0x0,
		ProbeResolutionWithDefault_1_2 = 0x1,
		ProbeResolutionWithDefault_1_4 = 0x2,
		ProbeResolutionWithDefault_1_8 = 0x3,
		ProbeResolutionWithDefault_UseLevelDefault = 0x4,
	};

	enum eAgentUsage
	{
		AgentUsage_Default = 0x0,
		AgentUsage_Aggressive = 0x1,
		AgentUsage_Conservative = 0x2,
	};

	enum eSimplifyMode
	{
		SimplifyMode_none = 0x0,
		SimplifyMode_simplifyNoUvs = 0x1,
		SimplifyMode_simplifyUsingUvs = 0x2,
	};

	struct EnumeDistributedBuildSystem : EnumBase {
		eDistributedBuildSystem mVal;
	};

	struct EnumeSceneOptimisationMode : EnumBase {
		eSceneOptimisationMode mVal;
	};

	struct EnumeBackfaceType : EnumBase {
		eBackfaceType mVal;
	};

	struct EnumeAutoUVSimplificationMode : EnumBase {
		eAutoUVSimplificationMode mVal;
	};

	struct EnumeProbeSampleMethod : EnumBase {
		eProbeSampleMethod mVal;
	};

	struct EnumeDisplayQuality : EnumBase {
		eDisplayQuality mVal;
	};

	struct EnumeRadiositySampleRate : EnumBase {
		eRadiositySampleRate mVal;
	};

	struct EnumeUpdateMethodWithDefault : EnumBase {
		eUpdateMethodWithDefault mVal;
	};

	struct EnumeProbeResolutionWithDefault : EnumBase {
		eProbeResolutionWithDefault mVal;
	};

	struct EnumeAgentUsage : EnumBase {
		eAgentUsage mVal;
	};

	struct EnumeSimplifyMode : EnumBase {
		eSimplifyMode mVal;
	};

	struct EnlightenMeshSettings {

		struct AutoUVSettings {
			EnumeSimplifyMode mSimplificationMode;
			float mMaxDistance;
			float mMaxInitialNormalDeviation;
			float mMaxGeneralNormalDeviation;
			float mExpansionFactor;
			float mSignificantAreaRatio;
		};

		EnumeInstanceType mEnlightenLightingMode;
		AutoUVSettings mAutoUVSettings;
		EnumeQuality mEnlightenQuality;

	};

};

#endif