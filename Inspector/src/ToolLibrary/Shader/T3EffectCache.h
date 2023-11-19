#ifndef _T3EFFECT_CACHE_H
#define _T3EFFECT_CACHE_H

#include "../TelltaleToolLibrary.h"
#include "../Types/DCArray.h"
#include "../Types/D3DMesh.h"
#include "../Types/T3Texture.h"
#include "../Types/SArray.h"
#include "../Types/List.h"


enum T3EffectQuality
{
	eEffectQuality_Default = 0xFFFFFFFF,
	eEffectQuality_High = 0x0,
	eEffectQuality_Medium = 0x1,
	eEffectQuality_LowPlus = 0x2,
	eEffectQuality_Low = 0x3,
	eEffectQuality_Lowest = 0x4,
	eEffectQuality_Count = 0x5,
};

struct T3EffectCachePackageHeader {

	u32 mMagic;//0x844FF83
	u32 mVersion;//on PC this is 157
	u32 mProgramCount;
	u32 mShaderCount;
	u32 mShaderDataOffset;
};


enum T3EffectFeature
{
	eEffectFeature_None = 0xFFFFFFFF,
	eEffectFeature_FirstStatic = 0x0,
	eEffectFeature_Tool = 0x0,
	eEffectFeature_Deformable = 0x1,
	eEffectFeature_DeformableSingle = 0x2,
	eEffectFeature_DeformableCompute = 0x3,
	eEffectFeature_LightMap = 0x4,
	eEffectFeature_StaticShadowMap = 0x5,
	eEffectFeature_VertexColor = 0x6,
	eEffectFeature_SoftDepthTest = 0x7,
	eEffectFeature_SupportsHBAO = 0x8,
	eEffectFeature_ParticleFacing = 0x9,
	eEffectFeature_ParticleFacing3D = 0xA,
	eEffectFeature_ParticleSmoothAnimation = 0xB,
	eEffectFeature_ParticleRotation3D = 0xC,
	eEffectFeature_ParticleRotation3DTexture = 0xD,
	eEffectFeature_ParticleTexCoord = 0xE,
	eEffectFeature_PostLevels = 0xF,
	eEffectFeature_PostRadialBlur = 0x10,
	eEffectFeature_PostTonemap = 0x11,
	eEffectFeature_PostTonemapIntensity = 0x12,
	eEffectFeature_PostTonemapFilmic = 0x13,
	eEffectFeature_PostTonemapFilmicRGB = 0x14,
	eEffectFeature_PostTonemapDOF = 0x15,
	eEffectFeature_PostVignetteTint = 0x16,
	eEffectFeature_PostDepthOfField1x = 0x17,
	eEffectFeature_PostDepthOfField2x = 0x18,
	eEffectFeature_PostDepthOfField3x = 0x19,
	eEffectFeature_PostNewDepthOfField = 0x1A,
	eEffectFeature_PostNewDepthOfField_Low = 0x1B,
	eEffectFeature_PostNewDepthOfField_Medium = 0x1C,
	eEffectFeature_PostNewDepthOfField_High = 0x1D,
	eEffectFeature_PostNewDepthOfField_NoMRT = 0x1E,
	eEffectFeature_PostNewDepthOfFieldMCLegacy = 0x1F,
	eEffectFeature_PostBokehUsesGS = 0x20,
	eEffectFeature_PostBokehUsesAberration = 0x21,
	eEffectFeature_BokehZTest = 0x22,
	eEffectFeature_BokehScaleOccluded = 0x23,
	eEffectFeature_PostBokeh = 0x24,
	eEffectFeature_PostDebugHBAO = 0x25,
	eEffectFeature_PostConvert_sRGB = 0x26,
	eEffectFeature_PostHDR10 = 0x27,
	eEffectFeature_PostHDRMulti = 0x28,
	eEffectFeature_DepthOfFieldVignette = 0x29,
	eEffectFeature_PostBrushOutline = 0x2A,
	eEffectFeature_PostBrushDepthOfField = 0x2B,
	eEffectFeature_PostTextureArray = 0x2C,
	eEffectFeature_PostShadowInput2x = 0x2D,
	eEffectFeature_PostShadowSDSM = 0x2E,
	eEffectFeature_ShadowApplyTile = 0x2F,
	eEffectFeature_ShadowApplyModulated = 0x30,
	eEffectFeature_ShadowApplyGobo = 0x31,
	eEffectFeature_HBAODeinterleaved = 0x32,
	eEffectFeature_HBAODeinterleaved2x = 0x33,
	eEffectFeature_HBAODeinterleaved4x = 0x34,
	eEffectFeature_BakeFlatLighting = 0x35,
	eEffectFeature_LastStatic = 0x35,
	eEffectFeature_FirstDynamic = 0x36,
	eEffectFeature_RimLight = 0x36,
	eEffectFeature_SpecularEnvironment = 0x37,
	eEffectFeature_Shadow = 0x38,
	eEffectFeature_EnvLighting = 0x39,
	eEffectFeature_CinLighting = 0x3A,
	eEffectFeature_EnvKeyLight = 0x3B,
	eEffectFeature_EnvKeyShadow = 0x3C,
	eEffectFeature_EnvLight2x = 0x3D,
	eEffectFeature_EnvLight3x = 0x3E,
	eEffectFeature_EnlightenMap = 0x3F,
	eEffectFeature_GlowOutput = 0x40,
	eEffectFeature_GBufferOutput = 0x41,
	eEffectFeature_DecalApply = 0x42,
	eEffectFeature_ClipPlane = 0x43,
	eEffectFeature_CameraClipPlanes = 0x44,
	eEffectFeature_ShadowCascades = 0x45,
	eEffectFeature_ShadowDepth2 = 0x46,
	eEffectFeature_ShadowOutputColor = 0x47,
	eEffectFeature_AlphaTest = 0x48,
	eEffectFeature_MSAAAlphaTest = 0x49,
	eEffectFeature_Dissolve = 0x4A,
	eEffectFeature_SharpShadows = 0x4B,
	eEffectFeature_LinearDepthFetch = 0x4C,
	eEffectFeature_FrameBufferFetch = 0x4D,
	eEffectFeature_DepthBufferFetch = 0x4E,
	eEffectFeature_CreaseLines = 0x4F,
	eEffectFeature_BoundaryLines = 0x50,
	eEffectFeature_JaggedLines = 0x51,
	eEffectFeature_SmoothLines = 0x52,
	eEffectFeature_Debug = 0x53,
	eEffectFeature_LastDynamic = 0x53,
	eEffectFeature_Count = 0x54,
	eEffectFeature_StaticCount = 0x36,
	eEffectFeature_DynamicCount = 0x1E,
};

enum T3EffectParameterType
{
	eEffectParameter_Unknown = 0xFFFFFFFF,
	eEffectParameter_FirstUniformBuffer = 0x0,
	eEffectParameter_UniformBufferCamera = 0x0,
	eEffectParameter_UniformBufferScene = 0x1,
	eEffectParameter_UniformBufferSceneTool = 0x2,
	eEffectParameter_UniformBufferObject = 0x3,
	eEffectParameter_UniformBufferInstance = 0x4,
	eEffectParameter_UniformBufferLights = 0x5,
	eEffectParameter_UniformBufferLightEnv = 0x6,
	eEffectParameter_UniformBufferLightCin = 0x7,
	eEffectParameter_UniformBufferSkinning = 0x8,
	eEffectParameter_UniformBufferShadow = 0x9,
	eEffectParameter_UniformBufferParticle = 0xA,
	eEffectParameter_UniformBufferDecal = 0xB,
	eEffectParameter_UniformBufferSimple = 0xC,
	eEffectParameter_UniformBufferGaussian = 0xD,
	eEffectParameter_UniformBufferPost = 0xE,
	eEffectParameter_UniformBufferBrush = 0xF,
	eEffectParameter_UniformBufferLightEnvData_High = 0x10,
	eEffectParameter_UniformBufferLightEnvData_Medium = 0x11,
	eEffectParameter_UniformBufferLightEnvData_Low = 0x12,
	eEffectParameter_UniformBufferLightEnvView = 0x13,
	eEffectParameter_UniformBufferLightAmbient = 0x14,
	eEffectParameter_UniformBufferShadowVolume = 0x15,
	eEffectParameter_UniformBufferMesh = 0x16,
	eEffectParameter_UniformBufferMeshBatch = 0x17,
	eEffectParameter_UniformBufferMeshDynamicBatch = 0x18,
	eEffectParameter_UniformBufferMeshDebugBatch = 0x19,
	eEffectParameter_UniformBufferHBAO = 0x1A,
	eEffectParameter_UniformBufferMaterialTool = 0x1B,
	eEffectParameter_UniformBufferMaterialBase = 0x1C,
	eEffectParameter_UniformBufferMaterialMain = 0x1D,
	eEffectParameter_LastUniformBuffer = 0x1D,
	eEffectParameter_FirstGenericBuffer = 0x1E,
	eEffectParameter_GenericBufferSkinning = 0x1E,
	eEffectParameter_GenericBuffer0VertexIn = 0x1F,
	eEffectParameter_GenericBuffer1VertexIn = 0x20,
	eEffectParameter_GenericBufferVertexOut = 0x21,
	eEffectParameter_GenericBufferMeshBounds = 0x22,
	eEffectParameter_GenericBufferIndices = 0x23,
	eEffectParameter_GenericBufferIndirectArgs = 0x24,
	eEffectParameter_GenericBufferStartInstance = 0x25,
	eEffectParameter_GenericBufferDepthRange = 0x26,
	eEffectParameter_GenericBufferPrevDepthRange = 0x27,
	eEffectParameter_GenericBufferLightGrid = 0x28,
	eEffectParameter_GenericBufferLightZBin = 0x29,
	eEffectParameter_GenericBufferLightGroupMask = 0x2A,
	eEffectParameter_GenericBufferShadowCascades = 0x2B,
	eEffectParameter_GenericBufferCinShadowData = 0x2C,
	eEffectParameter_GenericBufferDusterData = 0x2D,
	eEffectParameter_GenericBufferDusterVisibility = 0x2E,
	eEffectParameter_GenericBufferWaveformMonitor = 0x2F,
	eEffectParameter_GenericBufferInput0 = 0x30,
	eEffectParameter_GenericBufferInput1 = 0x31,
	eEffectParameter_GenericBufferInput2 = 0x32,
	eEffectParameter_GenericBufferInput3 = 0x33,
	eEffectParameter_GenericBufferInput4 = 0x34,
	eEffectParameter_GenericBufferInput5 = 0x35,
	eEffectParameter_GenericBufferInput6 = 0x36,
	eEffectParameter_GenericBufferInput7 = 0x37,
	eEffectParameter_GenericBufferOutput0 = 0x38,
	eEffectParameter_GenericBufferOutput1 = 0x39,
	eEffectParameter_GenericBufferOutput2 = 0x3A,
	eEffectParameter_GenericBufferOutput3 = 0x3B,
	eEffectParameter_GenericBufferOutput4 = 0x3C,
	eEffectParameter_LastGenericBuffer = 0x3C,
	eEffectParameter_FirstSampler = 0x3D,
	eEffectParameter_SamplerDiffuse = 0x3D,
	eEffectParameter_SamplerStaticShadowmap = 0x3E,
	eEffectParameter_SamplerShadowmap = 0x3F,
	eEffectParameter_SamplerProjected = 0x40,
	eEffectParameter_SamplerBrushNear = 0x41,
	eEffectParameter_SamplerBrushFar = 0x42,
	eEffectParameter_SamplerEnvironment = 0x43,
	eEffectParameter_SamplerBokehPattern = 0x44,
	eEffectParameter_SamplerNoiseLUT = 0x45,
	eEffectParameter_SamplerHLSMovieY = 0x46,
	eEffectParameter_SamplerHLSMovieC = 0x47,
	eEffectParameter_SamplerHLSMovieRGB = 0x48,
	eEffectParameter_SamplerBackbuffer = 0x49,
	eEffectParameter_SamplerBackbufferHDR = 0x4A,
	eEffectParameter_SamplerBackbufferHDRPrev = 0x4B,
	eEffectParameter_SamplerBackbufferHDRResolved = 0x4C,
	eEffectParameter_SamplerDepthbuffer = 0x4D,
	eEffectParameter_SamplerLinearDepth = 0x4E,
	eEffectParameter_SamplerLinearDepthPrev = 0x4F,
	eEffectParameter_SamplerAlphaMeshLinearDepth = 0x50,
	eEffectParameter_SamplerStencil = 0x51,
	eEffectParameter_SamplerGBuffer0 = 0x52,
	eEffectParameter_SamplerGBuffer1 = 0x53,
	eEffectParameter_SamplerDBuffer0 = 0x54,
	eEffectParameter_SamplerDBuffer1 = 0x55,
	eEffectParameter_SamplerDeferredShadows = 0x56,
	eEffectParameter_SamplerDeferredModulatedShadows = 0x57,
	eEffectParameter_SamplerDeferredShadowsPrev = 0x58,
	eEffectParameter_SamplerDeferredLight0 = 0x59,
	eEffectParameter_SamplerDeferredLight1 = 0x5A,
	eEffectParameter_SamplerDofBlur1x = 0x5B,
	eEffectParameter_SamplerDofBlur2x = 0x5C,
	eEffectParameter_SamplerDofBlur3x = 0x5D,
	eEffectParameter_NewDepthOfFieldHalf = 0x5E,
	eEffectParameter_NewDofNearH = 0x5F,
	eEffectParameter_NewDofNearV = 0x60,
	eEffectParameter_NewDofFarH1x = 0x61,
	eEffectParameter_NewDofFarV1x = 0x62,
	eEffectParameter_Bokeh = 0x63,
	eEffectParameter_SSLines = 0x64,
	eEffectParameter_SamplerLightEnvShadowGobo = 0x65,
	eEffectParameter_SamplerStaticShadowVolume = 0x66,
	eEffectParameter_FirstEVSMShadowSampler = 0x67,
	eEffectParameter_SamplerEVSMShadow0 = 0x67,
	eEffectParameter_SamplerEVSMShadow1 = 0x68,
	eEffectParameter_LastEVSMShadowSampler = 0x68,
	eEffectParameter_SamplerSMAAAreaLookup = 0x69,
	eEffectParameter_SamplerSMAASearchLookup = 0x6A,
	eEffectParameter_SamplerRandom = 0x6B,
	eEffectParameter_SamplerNoise = 0x6C,
	eEffectParameter_SamplerTetrahedralLookup = 0x6D,
	eEffectParameter_SamplerLightmap = 0x6E,
	eEffectParameter_SamplerLightmapFlat = 0x6F,
	eEffectParameter_SamplerBoneMatrices = 0x70,
	eEffectParameter_SamplerDebugOverlay = 0x71,
	eEffectParameter_SamplerSoftwareOcclusion = 0x72,
	eEffectParameter_FirstMaterialInputSampler = 0x73,
	eEffectParameter_SamplerMaterialInput0 = 0x73,
	eEffectParameter_SamplerMaterialInput1 = 0x74,
	eEffectParameter_SamplerMaterialInput2 = 0x75,
	eEffectParameter_SamplerMaterialInput3 = 0x76,
	eEffectParameter_SamplerMaterialInput4 = 0x77,
	eEffectParameter_SamplerMaterialInput5 = 0x78,
	eEffectParameter_SamplerMaterialInput6 = 0x79,
	eEffectParameter_SamplerMaterialInput7 = 0x7A,
	eEffectParameter_SamplerMaterialInput8 = 0x7B,
	eEffectParameter_SamplerMaterialInput9 = 0x7C,
	eEffectParameter_SamplerMaterialInput10 = 0x7D,
	eEffectParameter_SamplerMaterialInput11 = 0x7E,
	eEffectParameter_SamplerMaterialInput12 = 0x7F,
	eEffectParameter_SamplerMaterialInput13 = 0x80,
	eEffectParameter_SamplerMaterialInput14 = 0x81,
	eEffectParameter_SamplerMaterialInput15 = 0x82,
	eEffectParameter_LastMaterialInputSampler = 0x82,
	eEffectParameter_SamplerFxaaConsole360TexExpBiasNegOne = 0x83,
	eEffectParameter_SamplerFxaaConsole360TexExpBiasNegTwo = 0x84,
	eEffectParameter_FirstPostOutputSampler = 0x85,
	eEffectParameter_SamplerPostOutput0 = 0x85,
	eEffectParameter_SamplerPostOutput1 = 0x86,
	eEffectParameter_SamplerPostOutput2 = 0x87,
	eEffectParameter_SamplerPostOutput3 = 0x88,
	eEffectParameter_LastPostOutputSampler = 0x88,
	eEffectParameter_FirstPostInputSampler = 0x89,
	eEffectParameter_SamplerPostInput0 = 0x89,
	eEffectParameter_SamplerPostInput1 = 0x8A,
	eEffectParameter_SamplerPostInput2 = 0x8B,
	eEffectParameter_SamplerPostInput3 = 0x8C,
	eEffectParameter_SamplerPostInput4 = 0x8D,
	eEffectParameter_SamplerPostInput5 = 0x8E,
	eEffectParameter_SamplerPostInput6 = 0x8F,
	eEffectParameter_SamplerPostInput7 = 0x90,
	eEffectParameter_LastPostInputSampler = 0x90,
	eEffectParameter_FirstParticleSampler = 0x91,
	eEffectParameter_SamplerParticlePosition = 0x91,
	eEffectParameter_SamplerParticleOrientation = 0x92,
	eEffectParameter_SamplerParticleColor = 0x93,
	eEffectParameter_SamplerParticleRotation3D = 0x94,
	eEffectParameter_LastParticleSampler = 0x94,
	eEffectParameter_SamplerEnlighten = 0x95,
	eEffectParameter_LastSampler = 0x95,
	eEffectParameter_Count = 0x96,
	eEffectParameter_UniformBufferCount = 0x1E,
	eEffectParameter_GenericBufferCount = 0x1F,
	eEffectParameter_SamplerCount = 0x59,
	eEffectParameter_MaterialInputSamplerCount = 0x10,
	eEffectParameter_PostInputSamplerCount = 0x8,
	eEffectParameter_PostOutputSamplerCount = 0x4,
	eEffectParameter_ParticleSamplerCount = 0x4,
	eEffectParameter_EVSMShadowSamplerCount = 0x2,
};

struct T3EffectCachePackageProgram {
	T3EffectType mEffectType;
	BitSet<T3EffectFeature, 84, 0> mEffectFeatures;
	unsigned __int64 mMaterialCrc;
	T3EffectQuality mEffectQuality;
	unsigned int mPassCount, mSamplerCount;
};

enum T3MaterialShaderType
{
	eMaterialShader_None = 0xFFFFFFFF,
	eMaterialShader_Vertex = 0x0,
	eMaterialShader_Pixel = 0x1,
	eMaterialShader_Geometry = 0x2,
	eMaterialShader_Compute = 0x3,
	eMaterialShader_Count = 0x4,
};

struct T3EffectCachePackageShader {
	//unsigned int mShaderType;
	T3MaterialShaderType mShaderType;
	BitSet<T3EffectParameterType, 150, 0> mNeededParameters;
	unsigned int mOffset;
	unsigned int mSize;
};

struct T3EffectCacheParams
{
	BitSet<enum T3EffectFeature, 54, 0> mStaticFeatures;
	T3EffectQuality mQuality;
	unsigned __int64 mMaterialCrc;
};

//by library
struct T3EffectCachePackage {
	T3EffectCachePackageHeader mHeader;
	DCArray<T3EffectCachePackageProgram> mPrograms;
	DCArray<T3EffectCachePackageShader> mShaders;
};

namespace T3EffectCacheInternal {

	bool LoadPackage(T3EffectCachePackage& package, DataStream& stream);

	bool LoadPackageHeader(T3EffectCachePackageHeader& p, DataStream& stream);

	bool LoadPackageShader(T3EffectCachePackageShader& p, DataStream& stream);

	bool LoadPackageProgram(T3EffectCachePackageProgram& p, DataStream& stream);


	/*
	* PAYLOAD FORMAT:
	* 4byte: num of params
	* 4byte: size of platform compiled shader
	* for num of params:
	* 7byte: param value (not sure on the actual data in it, most likely a bitset)
	* 
	* shader compiled
	*/
	bool LoadShaderPayload(const T3EffectCachePackage& p, const T3EffectCachePackageShader& shader, char* pDest, DataStream& stream);

	unsigned int GetShaderPayloadSize(const T3EffectCachePackageShader& shader);

	// Util functions

	const char* GetFeatureName(T3EffectFeature ft);

	T3EffectFeature GetFeatureEnum(const char*);

	const char* GetParameterTypeName(T3EffectParameterType ft);

	T3EffectParameterType GetParameterTypeEnum(const char*);

	const char* GetShaderTypeName(T3MaterialShaderType sh);

	T3MaterialShaderType GetShaderType(const char* n);

}

#endif