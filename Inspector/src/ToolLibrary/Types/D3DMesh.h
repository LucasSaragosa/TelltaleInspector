// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _D3DMESH
#define _D3DMESH

#include "../Meta.hpp"
#include "DCArray.h"
#include "HandleObjectInfo.h"
#include "T3Texture.h"
#include "Map.h"
#include "ObjectSpaceIntrinsics.h"
#include "ToolProps.h"
#include "ParticleSprite.h"

/* template for funcs
static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(T, occ);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {

		}
		return r;
	}
*/

struct MeshSceneEnlightenData {
	Symbol mSystemName;
	Vector4 mUVTransform;
	Flags mFlags;
};

struct MeshSceneLightmapData {

	struct Entry {
		Symbol mMeshName;
		unsigned long mLODIndex;
		long mLightQuality;
		Vector2 mTextureScale;
		Vector2 mTextureOffset;
		unsigned long mTexturePage;
	};

	DCArray<Entry> mEntries;
	DCArray<unsigned short> mStationaryLightIndices;
	Flags mFlags;

};

enum GFXPlatformVertexAttribute {
	eGFXPlatformAttribute_Position = 0,
	eGFXPlatformAttribute_Normal = 1,
	eGFXPlatformAttribute_Tangent = 2,
	eGFXPlatformAttribute_BlendWeight = 3,
	eGFXPlatformAttribute_BlendIndex = 4,
	eGFXPlatformAttribute_Color = 5,
	eGFXPlatformAttribute_TexCoord = 6,
	eGFXPlatformAttribute_Count = 7,
};

enum GFXPlatformFormat {
	eGFXPlatformFormat_None = 0x0,
	eGFXPlatformFormat_F32 = 0x1,
	eGFXPlatformFormat_F32x2 = 0x2,
	eGFXPlatformFormat_F32x3 = 0x3,
	eGFXPlatformFormat_F32x4 = 0x4,
	eGFXPlatformFormat_F16x2 = 0x5,
	eGFXPlatformFormat_F16x4 = 0x6,
	eGFXPlatformFormat_S32 = 0x7,
	eGFXPlatformFormat_U32 = 0x8,
	eGFXPlatformFormat_S32x2 = 0x9,
	eGFXPlatformFormat_U32x2 = 0xA,
	eGFXPlatformFormat_S32x3 = 0xB,
	eGFXPlatformFormat_U32x3 = 0xC,
	eGFXPlatformFormat_S32x4 = 0xD,
	eGFXPlatformFormat_U32x4 = 0xE,
	eGFXPlatformFormat_S16 = 0xF,
	eGFXPlatformFormat_U16 = 0x10,
	eGFXPlatformFormat_S16x2 = 0x11,
	eGFXPlatformFormat_U16x2 = 0x12,
	eGFXPlatformFormat_S16x4 = 0x13,
	eGFXPlatformFormat_U16x4 = 0x14,
	/*N stands for normalised, so these are fractions of the bidwidth as a float, so take as int then divide by all 1s of the bigwidth as a float to get float val*/
	eGFXPlatformFormat_SN16 = 0x15,
	eGFXPlatformFormat_UN16 = 0x16,
	eGFXPlatformFormat_SN16x2 = 0x17,
	eGFXPlatformFormat_UN16x2 = 0x18,
	eGFXPlatformFormat_SN16x4 = 0x19,
	eGFXPlatformFormat_UN16x4 = 0x1A,
	eGFXPlatformFormat_S8 = 0x1B,
	eGFXPlatformFormat_U8 = 0x1C,
	eGFXPlatformFormat_S8x2 = 0x1D,
	eGFXPlatformFormat_U8x2 = 0x1E,
	eGFXPlatformFormat_S8x4 = 0x1F,
	eGFXPlatformFormat_U8x4 = 0x20,
	eGFXPlatformFormat_SN8 = 0x21,
	eGFXPlatformFormat_UN8 = 0x22,
	eGFXPlatformFormat_SN8x2 = 0x23,
	eGFXPlatformFormat_UN8x2 = 0x24,
	eGFXPlatformFormat_SN8x4 = 0x25, 
	eGFXPlatformFormat_UN8x4 = 0x26,
	eGFXPlatformFormat_SN10_SN11_SN11 = 0x27,
	eGFXPlatformFormat_SN10x3_SN2 = 0x28,//?? lol
	eGFXPlatformFormat_UN10x3_UN2 = 0x29,
	eGFXPlatformFormat_D3DCOLOR = 0x2A,//DIRECT 3D colors (see d3d manual)
	eGFXPlatformFormat_Count = 0x2B,
};

enum T3MaterialPassType {
	eMaterialPass_FirstMesh = 0,
	eMaterialPass_Main = 0,
	eMaterialPass_PreZ = 1,
	eMaterialPass_GBuffer = 2,
	eMaterialPass_GBuffer_Lines = 3,
	eMaterialPass_ParticleCount = 3,
	eMaterialPass_DecalCount = 3,
	eMaterialPass_GBuffer_Outline = 4,
	eMaterialPass_Glow = 5,
	eMaterialPass_ShadowMap = 6,
	eMaterialPass_Outline = 7,
	eMaterialPass_LinearDepth = 8,
	eMaterialPass_LinesCS_Generate = 9,
	eMaterialPass_LinesCS_Rasterize = 0x0A,
	eMaterialPass_LinesCS_ForceLinearDepth = 0x0B,
	eMaterialPass_LastMesh = 0x0B,
	eMaterialPass_FirstParticle = 0xC,
	eMaterialPass_ParticleMain = 0x0C,
	eMaterialPass_MeshCount = 0x0C,
	eMaterialPass_ParticleGlow = 0x0D,
	eMaterialPass_ParticleLinearDepth = 0x0E,
	eMaterialPass_LastParticle = 0x0E,
	eMaterialPass_FirstDecal = 0x0F,
	eMaterialPass_DecalGBuffer = 0x0F,
	eMaterialPass_DecalEmissive = 0x10,
	eMaterialPass_DecalGlow = 0x11,
	eMaterialPass_LastDecal = 0x11,
	eMaterialPass_Post = 0x12,
	eMaterialPass_MayaShader = 0x13,
	eMaterialPass_LightBake = 0x14,
	eMaterialPass_Count = 0x15,
};

enum T3EffectType {
	eEffect_Mesh = 0x0,
	eEffect_Particle = 0x1,
	eEffect_ParticleGlow = 0x2,
	eEffect_ParticleForceLinearDepth = 0x3,
	eEffect_Particle_Xbox_Instanced = 0x4,
	eEffect_DeferredDecalGBuffer = 0x5,
	eEffect_DeferredDecalEmissive = 0x6,
	eEffect_DeferredDecalGlow = 0x7,
	eEffect_SceneSimple = 0x8,
	eEffect_SceneSimple_Texture = 0x9,
	eEffect_SceneSimple_Cone = 0x0A,
	eEffect_SceneNPRLinesCS_Rasterize = 0x0B,
	eEffect_SceneNPRLinesCS_ForceLinearDepth = 0x0C,
	eEffect_ScenePreZ = 0x0D,
	eEffect_ScenePreZLines = 0x0E,
	eEffect_ForceLinearDepth = 0x0F,
	eEffect_SceneShadowMap = 0x10,
	eEffect_SceneGlow = 0x11,
	eEffect_SceneGBuffer = 0x12,
	eEffect_SceneGBufferLines = 0x13,
	eEffect_SceneToonOutline = 0x14,
	eEffect_SceneToonOutline2 = 0x15,
	eEffect_SceneToonOutline2_GBuffer = 0x16,
	eEffect_FX_GlowPrepare = 0x17,
	eEffect_FX_Glow = 0x18,
	eEffect_FX_DepthOfFieldPrepare = 0x19,
	eEffect_FX_GaussianBlur = 0x1A,
	eEffect_FX_GaussianBlur_Glow = 0x1B,
	eEffect_FX_GaussianBlur_Outline = 0x1C,
	eEffect_FX_BlurUp = 0x1D,
	eEffect_FX_MotionBlur = 0x1E,
	eEffect_FX_MergeColorDepth = 0x1F,
	eEffect_FX_PostColor = 0x20,
	eEffect_DynamicLightMap = 0x21,
	eEffect_FX_RadialBlur = 0x22,
	eEffect_FX_FXAA = 0x23,
	eEffect_FX_SMAA_EdgeDetect = 0x24,
	eEffect_FX_SMAA_BlendWeight = 0x25,
	eEffect_FX_SMAA_Final = 0x26,
	eEffect_FX_GBufferResolve = 0x27,
	eEffect_FX_GBufferDownSample = 0x28,
	eEffect_FX_LinearDepthDownSample = 0x29,
	eEffect_FX_DepthDownSample = 0x2A,
	eEffect_FX_StencilMaskRestore = 0x2B,
	eEffect_FX_OcclusionFilter = 0x2C,
	eEffect_FX_Null = 0x2D,
	eEffect_FX_Copy = 0x2E,
	eEffect_FX_CopyDepth = 0x2F,
	eEffect_FX_CompositeAlpha = 0x30,
	eEffect_FX_CompositeQuarter = 0x31,
	eEffect_FX_CreateOutlineSrc = 0x32,
	eEffect_FX_CreateOutline = 0x33,
	eEffect_FX_BrushOutline = 0x34,
	eEffect_FX_BrushOutline_Simple = 0x35,
	eEffect_FX_BrushOffsetApply = 0x36,
	eEffect_FX_BrushOutlineBlurPrepare = 0x37,
	eEffect_FX_BrushOutlineBlur = 0x38,
	eEffect_FX_ApplyModulatedShadow = 0x39,
	eEffect_FX_FinalResolve = 0x3A,
	eEffect_FX_FinalDebugOverlay = 0x3B,
	eEffect_SceneText = 0x3C,
	eEffect_SceneSimple_Texture_AlphaOnly = 0x3D,
	eEffect_SceneHLSMovieYUV = 0x3E,
	eEffect_SceneHLSMovieRGB = 0x3F,
	eEffect_DirectionalLightShadow = 0x40,
	eEffect_DirectionalLightShadow_High = 0x41,
	eEffect_ProjectedTextureShadow = 0x42,
	eEffect_ProjectedTextureColor = 0x43,
	eEffect_EnvLightShadowMark_Point = 0x44,
	eEffect_EnvLightShadowMark_Spot = 0x45,
	eEffect_EnvLightShadowApply_Point = 0x46,
	eEffect_EnvLightShadowApply_Spot = 0x47,
	eEffect_EnvLightShadowApply_Directional = 0x48,
	eEffect_EnvLightShadowApply_DirectionalFade = 0x49,
	eEffect_EnvLightShadowApply_DirectionalBlend = 0x4A,
	eEffect_EnvLightShadowApply_SDSM_Directional = 0x4B,
	eEffect_EnvLightShadowApply_SDSM_DirectionalFade = 0x4C,
	eEffect_ShadowMark_SDSM = 0x4D,
	eEffect_ShadowVolumeApply = 0x4E,
	eEffect_ShadowTrace = 0x4F,
	eEffect_ShadowTrace_HBAOLow = 0x50,
	eEffect_ShadowTrace_HBAOMid = 0x51,
	eEffect_ShadowTrace_HBAOHigh = 0x52,
	eEffect_ShadowTrace_MSAA = 0x53,
	eEffect_ShadowTrace_HBAOLow_MSAA = 0x54,
	eEffect_ShadowTrace_HBAOMid_MSAA = 0x55,
	eEffect_ShadowTrace_HBAOHigh_MSAA = 0x56,
	eEffect_FX_DeferredFilter_Light = 0x57,
	eEffect_FX_DeferredFilter_Shadow = 0x58,
	eEffect_FX_DeferredFilter_LightShadow = 0x59,
	eEffect_FX_CreateOutlineSrcQuarter = 0x5A,
	eEffect_FX_NewGlowH1x = 0x5B,
	eEffect_FX_NewGlowV1x = 0x6B,
	eEffect_FX_NewGlowPresent = 0x7B,
	eEffect_FX_NewDepthOfFieldPreparE, alf = 0x7C,
	eEffect_FX_DepthOfFieldBlurH1x = 0x7D,
	eEffect_FX_DepthOfFieldBlurV1x = 0x7E,
	eEffect_FX_DepthOfFieldBlurH = 0x7F,
	eEffect_FX_DepthOfFieldBlurV = 0x80,
	eEffect_FX_DepthOfFieldBlurHNear = 0x81,
	eEffect_FX_DepthOfFieldBlurHFar = 0x82,
	eEffect_FX_DepthOfFieldBlurVNear = 0x83,
	eEffect_FX_DepthOfFieldBlurVFar = 0x84,
	eEffect_FX_ShadowFilter_Pass0 = 0x85,
	eEffect_FX_ShadowFilter_Pass0_TAA = 0x86,
	eEffect_FX_ShadowFilter_Blur0 = 0x87,
	eEffect_FX_ShadowFilter_Blur1 = 0x88,
	eEffect_FX_HBAOLowestQuality = 0x89,
	eEffect_FX_HBAOLowQuality = 0x8A,
	eEffect_FX_HBAOMediumQuality = 0x8B,
	eEffect_FX_HBAOHighQuality = 0x8C,
	eEffect_FX_HBAOHighestQuality = 0x8D,
	eEffect_FX_HBAODeinterleaveDepth = 0x8E,
	eEffect_FX_HBAODeinterleaveNormals = 0x8F,
	eEffect_FX_HBAOReinterleave = 0x90,
	eEffect_FX_SSLines = 0x91,
	eEffect_FX_SSLinesResolve_1x_Pass0 = 0x92,
	eEffect_FX_SSLinesResolve_1x_Pass1 = 0x93,
	eEffect_FX_SSLinesResolve_2x_Pass0 = 0x94,
	eEffect_FX_SSLinesResolve_2x_Pass1 = 0x95,
	eEffect_FX_SSLinesResolve_3x_Pass0 = 0x96,
	eEffect_FX_SSLinesResolve_3x_Pass1 = 0x97,
	eEffect_FX_SSLinesResolve_4x_Pass0 = 0x98,
	eEffect_FX_SSLinesResolve_4x_Pass1 = 0x99,
	eEffect_FX_MSAAResolve_2x = 0x9A,
	eEffect_FX_MSAAResolve_4x = 0x9B,
	eEffect_FX_MSAAResolve_8x = 0x9C,
	eEffect_FX_MSAASceneResolve_2x = 0x9D,
	eEffect_FX_MSAASceneResolve_4x = 0x9E,
	eEffect_FX_MSAASceneResolve_8x = 0x9F,
	eEffect_FX_SceneResolve_TAA = 0x0A0,
	eEffect_FX_MSAASceneResolve_TAA_2x = 0x0A1,
	eEffect_FX_MSAASceneResolve_TAA_4x = 0x0A2,
	eEffect_FX_MSAASceneResolve_TAA_8x = 0x0A3,
	eEffect_FX_MSAAGBufferResolve_2x = 0x0A4,
	eEffect_FX_MSAAGBufferResolve_4x = 0x0A5,
	eEffect_FX_MSAAGBufferResolve_8x = 0x0A6,
	eEffect_MakeMipLevel = 0x0A7,
	eEffect_MakeMipLevel_BlurH = 0x0A8,
	eEffect_MakeMipLevel_BlurV = 0x0A9,
	eEffect_MakeMipLevel_Array = 0x0AA,
	eEffect_CS_ComputeTest = 0x0AB,
	eEffect_CS_ApplySkinning_0Normal = 0x0AC,
	eEffect_CS_ApplySkinning_1Normal = 0x0AD,
	eEffect_CS_ApplySkinning_2Normal = 0x0AE,
	eEffect_CS_ApplySkinning_3Normal = 0x0AF,
	eEffect_CS_ApplySkinning_Float3Position_0Normal = 0x0B0,
	eEffect_CS_ApplySkinning_Float3Position_1Normal = 0x0B1,
	eEffect_CS_ApplySkinning_Float3Position_2Normal = 0x0B2,
	eEffect_CS_ApplySkinning_Float3Position_3Normal = 0x0B3,
	eEffect_CS_LineGeneration = 0x0B4,
	eEffect_CS_ClearIndirectDraw = 0x0B5,
	eEffect_CS_LightClustering2D = 0x0B6,
	eEffect_CS_LightClustering3D = 0x0B7,
	eEffect_CS_ShadowCascadeDistances_Manual = 0x0B8,
	eEffect_CS_ShadowCascadeDistances_Auto = 0x0B9,
	eEffect_CS_ShadowCascadeBounds_Pass0 = 0x0BA,
	eEffect_CS_ShadowCascadeBounds_PassN = 0x0BB,
	eEffect_CS_ShadowCascades = 0x0BC,
	eEffect_CS_ShadowCascades_ComputedBounds = 0x0BD,
	eEffect_CS_CullShadowCasters = 0x0BE,
	eEffect_CS_EnvLightMakeEVSM_Linear_Blur0 = 0x0BF,
	eEffect_CS_EnvLightMakeEVSM_Linear_Blur3x3 = 0x0C0,
	eEffect_CS_EnvLightMakeEVSM_Linear_Blur9x9 = 0x0C1,
	eEffect_CS_EnvLightMakeEVSM_NonLinear_Blur0 = 0x0C2,
	eEffect_CS_EnvLightMakeEVSM_NonLinear_Blur3x3 = 0x0C3,
	eEffect_CS_EnvLightMakeEVSM_NonLinear_Blur9x9 = 0x0C4,
	eEffect_CS_DepthReduction_Pass0_8x8 = 0x0C5,
	eEffect_CS_DepthReduction_Pass0_16x16 = 0x0C6,
	eEffect_CS_DepthReduction_PassN = 0x0C7,
	eEffect_CS_DepthReduction_PassFinal = 0x0C8,
	eEffect_CS_BokE, Sprites = 0x0C9,
	eEffect_FX_RasterizeBokeh = 0x0CA,
	eEffect_CS_EnlightenMakeDusters_Point = 0x0CB,
	eEffect_CS_EnlightenMakeDusters_Spot = 0x0CC,
	eEffect_CS_EnlightenMakeDusters_Directional = 0x0CD,
	eEffect_CS_WaveformMonitor = 0x0CE,
	eEffect_CS_EnvLightApplyDeferred = 0x0CF,
	eEffect_CS_EnvLightApplyDeferred_LowRes = 0x0D0,
	eEffect_BakeObjectNormals_Tool = 0x0D1,
	eEffect_BakeObjectNormals_NormalMap_Tool = 0x0D2,
	eEffect_BakeTangentNormals_Tool = 0x0D3,
	eEffect_BakeLighting_Tool = 0x0D4,
	eEffect_CS_LightmapBlurH_Tool = 0x0D5,
	eEffect_CS_LightmapBlurV_Tool = 0x0D6,
	eEffect_CS_LightmapFilter_Tool = 0x0D7,
	eEffect_CS_LightmapConvert_Tool = 0x0D8,
	eEffect_CS_BakeShadowVolume_Tool = 0x0D9,
	eEffect_CS_ComputeAsyncTest = 0x0DA,
	eEffect_Count = 0x0DB,
	eEffect_None = 0x0FFFFFFFF,
};

enum T3MaterialChannelType {
	eMaterialChannel_SurfaceNormal = 0x0,
	eMaterialChannel_DiffuseColor = 0x1,
	eMaterialChannel_SpecularColor = 0x2,
	eMaterialChannel_VertexColorCount = 0x2,
	eMaterialChannel_EmissiveColor = 0x3,
	eMaterialChannel_VertexNormal = 0x4,
	eMaterialChannel_TexCoordCount = 0x4,
	eMaterialChannel_InvertShadow = 0x5,
	eMaterialChannel_Alpha_Legacy = 0x6,
	eMaterialChannel_Gloss = 0x7,
	eMaterialChannel_AmbientOcclusion = 0x8,
	eMaterialChannel_Glow = 0x9,
	eMaterialChannel_OutlineSize = 0x0A,
	eMaterialChannel_VertexOffset = 0x0B,
	eMaterialChannel_VertexColor0 = 0x0C,
	eMaterialChannel_VertexColor1 = 0x0D,
	eMaterialChannel_TexCoord0 = 0x0E,
	eMaterialChannel_TexCoord1 = 0x0F,
	eMaterialChannel_TexCoord2 = 0x10,
	eMaterialChannel_TexCoord3 = 0x11,
	eMaterialChannel_NPR_Hatching = 0x18,
	eMaterialChannel_LineColor = 0x19,
	eMaterialChannel_LineVisibility = 0x1A,
	eMaterialChannel_LineGenerationStyle = 0x1B,
	eMaterialChannel_LineWidth = 0x1C,
	eMaterialChannel_Reserved0 = 0x1C,
	eMaterialChannel_Reserved1 = 0x1D,
	eMaterialChannel_Reserved2 = 0x1E,
	eMaterialChannel_Reserved3 = 0x1F,
	eMaterialChannel_Reserved4 = 0x20,
	eMaterialChannel_DetailColor = 0x21,
	eMaterialChannel_DetailAlpha = 0x22,
	eMaterialChannel_DiffuseAlbedoColor = 0x23,
	eMaterialChannel_FinalColor = 0x24,
	eMaterialChannel_LineAlpha = 0x25,
	eMaterialChannel_Opacity = 0x26,
	eMaterialChannel_OpacityMask = 0x27,
	eMaterialChannel_SpecularAlbedoColor = 0x28,
	eMaterialChannel_Anisotropy = 0x29,
	eMaterialChannel_SmoothSurfaceNormal = 0x2A,
	eMaterialChannel_AnisotropyMask = 0x2B,
	eMaterialChannel_AnisotropyTangent = 0x2C,
	eMaterialChannel_SecondarySpecularAlbedoColor = 0x2D,
	eMaterialChannel_Count = 0x2E,
	eMaterialChannel_Custom = 0x0FFFF,
	eMaterialChannel_None = 0x0FFFFFFFF,

};

enum T3MaterialPropertyType {
	eMaterialProperty_GlossExponent = 0,
	eMaterialProperty_GlowIntensity = 1,
	eMaterialProperty_SpecularPower = 2,
	eMaterialProperty_OutlineColor = 3,
	eMaterialProperty_OutlineInvertColor = 4,
	eMaterialProperty_OutlineZRange = 5,
	eMaterialProperty_ToonGradientCutoff = 6,
	eMaterialProperty_LineGenerateCreases = 7,
	eMaterialProperty_LineGenerateBoundaries = 0x8,
	eMaterialProperty_LineCreaseAngle = 0x9,
	eMaterialProperty_LineSmoothJaggedCreaseAngle = 0x0A,
	eMaterialProperty_LineGenerateSmooth = 0x0B,
	eMaterialProperty_LineGenerateJagged = 0x0C,
	eMaterialProperty_LineMinWidth = 0x0D,
	eMaterialProperty_LineMaxWidth = 0x0E,
	eMaterialProperty_LineWidthFromLighting = 0x0F,
	eMaterialProperty_LineLightingType = 0x10,
	eMaterialProperty_LineLightingId = 0x11,
	eMaterialProperty_LinePatternRepeat = 0x12,
	eMaterialProperty_LitLineBias = 0x13,
	eMaterialProperty_LitLineScale = 0x14,
	eMaterialProperty_ConformNormal = 0x15,
	eMaterialProperty_NPRLineFalloff = 0x16,
	eMaterialProperty_NPRLineAlphaFalloff = 0x17,
	eMaterialProperty_DrawHiddenLines = 0x18,
	eMaterialProperty_AlphaMeshCullsLines = 0x19,
	eMaterialProperty_UseArtistNormal = 0x1A,
	eMaterialProperty_HorizonFade = 0x1B,
	eMaterialProperty_HairTerms = 0x1C,
	eMaterialProperty_ClothOffsetScale = 0x1D,
	eMaterialProperty_LightWrap = 0xE,
	eMaterialProperty_Count = 0x1F,
	eMaterialProperty_None = 0x0FFFFFFFF,
	
};

enum T3MaterialValueType {
	eMaterialValue_Float = 0,
	eMaterialValue_Float2 = 1,
	eMaterialValue_Float3 = 2,
	eMaterialValue_Float4 = 3,
	eMaterialValue_Channels = 4,
	eMaterialValue_Count = 5,
	eMaterialValue_None = 0xFFFFFFFF
};

struct T3MaterialParameter {
	Symbol mName;
	T3MaterialPropertyType mPropertyType;
	T3MaterialValueType mValueType;
	//in michonne its mscalaroffset[2]
	long mFlags, mScalarOffset[4];//in marvel and above its just mScalarOffset, not an array
	long mPreShaderScalarOffset, mNestedMaterialIndex;
};

struct T3MaterialEnlightenPrecomputeParams {
	float mIndirectReflectivity;
	float mIndirectTransparency;
	T3MaterialEnlightenPrecomputeParams() : mIndirectReflectivity(1.0f) {}
};

struct T3MaterialTexture {
	Symbol mName, mTextureName, mTextureNameWithoutExtension;
	T3TextureLayout mLayout;
	T3MaterialPropertyType mPropertyType;//wd4+
	BitSetBase<1> mTextureTypes;
	long mFirstParamIndex, mParamCount, mTextureIndex, mNestedMaterialIndex;
};

struct T3MaterialTransform2D {
	Symbol mParameterPrefix;
	long mFlags, mScalarOffset0[4], mScalarOffset1[4], mPreShaderScalarOffset0, mPreShaderScalarOffset1;//not arrays in games newer marvel (inc)
	long mNestedMaterialIndex;//mflags only in marvel and above
};

struct T3MaterialNestedMaterial {
	Handle<PropertySet> mhMaterial;//material_x.prop
};

struct T3MaterialPreShader {
	T3MaterialValueType mValueType;
	long mFlags, mPreShaderOffset, mScalarParameterOffset[4];//flags newer and inc marvel
};

struct T3MaterialStaticParameter {
	Symbol mName;
	long mNestedMaterialIndex;
};

enum T3MaterialTextureParamType {
	eMaterialTextureParam_Unused = 0,
	eMaterialTextureParam_Count = 1,
	eMaterialTextureParam_None = 0xFFFFFFFF,
};

struct T3MaterialTextureParam {
	T3MaterialTextureParamType mParamType;
	T3MaterialValueType mValueType;
	long mFlags, mScalarOffset;
};

struct T3MaterialPassData {
	T3MaterialPassType mPassType;
	BlendMode mBlendMode;
	u64 mMaterialCrc;
};

struct T3MaterialRuntimeProperty {
	Symbol mName, mRuntimeName;
};

enum T3MaterialQualityType {
	eMaterialQuality_High = 0,
	eMaterialQuality_Low = 1,
	eMaterialQuality_Lowest = 2,
	eMaterialQuality_Count = 3
};

struct T3MaterialCompiledData {
	DCArray<T3MaterialParameter> mParameters;
	DCArray<T3MaterialTexture> mTextures;
	DCArray<T3MaterialTransform2D> mTransforms;
	DCArray<T3MaterialNestedMaterial> mNestedMaterials;
	DCArray<T3MaterialPreShader> mPreShaders;
	DCArray<T3MaterialStaticParameter> mStaticParameters;
	DCArray<T3MaterialTextureParam> mTextureParams;//TODO CHECK WD3!
	DCArray<T3MaterialPassData> mPasses;
	T3MaterialQualityType mMaterialQuality;//>=wd4
	BitSetBase<1> mMaterialBlendModes, mMaterialPasses;
	BitSet<T3MaterialChannelType, 46, 0> mMaterialChannels;//>=wd3
	BitSet<T3MaterialChannelType, 32, 0> mMaterialChannels2;//michonne
	BitSetBase<3> mShaderInputs;//>=wd4
	BitSetBase<2> mShaderInputs2;//batman2>=
	BitSetBase<1> mShaderInputs3;//michonne
	BitSetBase<1> mSceneTextures, mOptionalPropertyTypes;//scene textures >=wd4
	BinaryBuffer mPreShaderBuffer;
	Flags mFlags;
	long mParameterBufferScalarSize[4];//size 2 array in games newer and including marvels GoG
	long mPreShaderParameterBufferScalarSize;

};

enum T3MaterialDomainType {
	 eMaterialDomain_Mesh = 0,
	 eMaterialDomain_Particle = 1,
	 eMaterialDomain_Decal = 2,
	 eMaterialDomain_Post = 3,
	 eMaterialDomain_ExportMeshShader = 4,
	 eMaterialDomain_Count = 5,
	 eMaterialDomain_None = 0xFFFFFFFF,
};

struct T3MaterialData {
	Symbol mMaterialName, mRuntimePropertiesName, mLegacyRenderTextureProperty;
	Symbol mLegacyBlendModeRuntimeProperty;
	T3MaterialDomainType mDomain;//wd4+
	LightType mLightType;//batman2 and below
	DCArray<T3MaterialRuntimeProperty> mRuntimeProperties;
	Flags mFlags, mRuntimeFlags;//runtimeflags not serialized
	long mVersion;
	float mMaxDistance;//batman2 and below, not sure if its a float/int always 0
	DCArray<T3MaterialCompiledData> mCompiledData2;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(T3MaterialData, data);
		static i32 wd4 = -1;
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r != eMetaOp_Succeed)
			return r;
		if (wd4 == -1)
			wd4 = TelltaleToolLib_GetGameKeyIndex("WD4");
		if (sSetKeyIndex >= wd4) {
			//r = PerformMetaSerializeAsync<DCArray<T3MaterialCompiledData>>(meta, &data->mCompiledData2);
			//if (r != eMetaOp_Succeed)
			//	return r;
		}
		else {
			u32 num = data->mCompiledData2.GetSize();
			meta->serialize_uint32(&num);
			if (meta->IsRead()) {
				if (data->mCompiledData2.mpStorage)
					delete[] data->mCompiledData2.mpStorage;
				data->mCompiledData2.mpStorage = new T3MaterialCompiledData[num];
				data->mCompiledData2.mSize = num;
				data->mCompiledData2.mCapacity = num;
			}
			u32 index = 0;
			for (u32 i = 0; i < num; i++, index++) {
				meta->serialize_uint32(&index);
				if (index >= num) {
					TelltaleToolLib_RaiseError("Material index was larger than number of materials", ErrorSeverity::ERR);
					return eMetaOp_Fail;
				}
				r=PerformMetaSerializeAsync<T3MaterialCompiledData>(meta, data->mCompiledData2.mpStorage + index);
				if (r != eMetaOp_Succeed)
					return r;
			}
		}
		return r;
	}

};

enum T3MeshMaterialFlags : u32 {
	eMeshMaterialFlag_Embedded = 1,
	eMeshMaterialFlag_LODGenerated = 2
};

struct T3MeshMaterial {
	Handle<PropertySet> mhMaterial;/*points to internal resource inside mesh data*/
	Symbol mBaseMaterialName, mLegacyRenderTextureProperty;//legacy >michonne excl
	BoundingBox mBoundingBox;
	Sphere mBoundingSphere;
	Flags mFlags;
};

enum T3MeshBatchUsageFlag {
	eMeshBatchUsage_Deformable = 1,
	eMeshBatchUsage_DeformableSingle = 2,
	eMeshBatchUsage_DoubleSided = 4,
	eMeshBatchUsage_TriangleStrip = 8
};

struct T3MeshTextureIndices {
	int mIndex[2];

	T3MeshTextureIndices(){
		mIndex[0] = -1;
		mIndex[1] = -1;
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(T3MeshTextureIndices, tex);
		if (meta->IsWrite()) {
			for (u32 i = 0; i < 2; i++) {
				u32 index = tex->mIndex[i];
				if ((index & 0x80000000) == 0) {
					meta->serialize_uint32(&i);
					meta->serialize_uint32(&index);
				}
			}
			u32 i = (u32)-1;
			meta->serialize_uint32(&i);
		}
		else {
			u32 x;
			u32 i = (u32)-1;
			meta->serialize_uint32(&i);
			while ((i & 0x80000000) == 0) {
				meta->serialize_uint32(&x);
				if (2 > i)
					tex->mIndex[i] = x;
				meta->serialize_uint32(&i);
			}
		}
		return eMetaOp_Succeed;
	}

};

struct T3MeshPropertyEntry {
	String mIncludeFilter, mExcludeFilter;
	Handle<PropertySet> mhProperties;
	long mPriority;
};

struct T3MeshBatch {
	BoundingBox mBoundingBox;
	Sphere mBoundingSphere;
	Flags mBatchUsage;
	long mMinVertIndex, mMaxVertIndex, mBaseIndex, mStartIndex, mNumPrimitives, mNumIndices;//base index and indices in marvel and above
	T3MeshTextureIndices mTextureIndices;
	long mMaterialIndex, mAdjacencyStartIndex;//adjancency games newer (excl) than michonne
	int mLocalTransformIndex, mBonePaletteIndex;//wd3 and below
};

struct T3MeshLOD {
	DCArray<T3MeshBatch> mBatches[2];
	DCArray<T3MeshBatch> mBatchesM;//michonne
	BitSetBase<1> mVertexStreams;
	BoundingBox mBoundingBox;
	Sphere mBoundingSphere;
	Flags mFlags;//>=wd4
	//primitives,batches not in michonne
	//vertex state index, index into T3MeshData::mVertexStates (buffers)
	//vertex start,count, atlas width and height only in wd4 or higher
	long mVertexStateIndex, mNumPrimitives, mNumBatches, mVertexStart, mVertexCount, mTextureAtlasWidth, mTextureAtlasHeight;
	//distance only in wd4 or higher
	float mPixelSize, mDistance;
	DCArray<Symbol> mBones;
	//default serializer
};

enum T3MeshTextureType {
	eMeshTexture_LightMap = 0,
	eMeshTexture_ShadowMap = 1,
	eMeshTexture_Count = 2,
	eMeshTexture_None = 0xFFFFFFFF
};

struct T3MeshTexture {
	T3MeshTextureType mTextureType;
	Handle<T3Texture> mhTexture;
	Symbol mNameSymbol;
	BoundingBox mBoundingBox;
	Sphere mBoundingSphere;
	float mMaxObjAreaPerUVArea;
	float mAverageObjAreaPerUVArea;
};

struct T3MeshMaterialOverride {
	Handle<PropertySet> mhOverrideMaterial;
	long mMaterialIndex;
};

struct T3MeshBoneEntry {
	Symbol mBoneName;//use hash db
	BoundingBox mBoundingBox;
	Sphere mBoundingSphere;
	long mNumVerts;
};

enum T3CameraFacingType {
	eCameraFacing_None = 0,
	eCameraFacing_Enable = 1,
	eCameraFacing_EnableY = 2,
	eCameraFacing_EnableLocalY = 3
};

struct T3MeshLocalTransformEntry {
	Transform mTransform;
	T3CameraFacingType mCameraFacingType;
};

struct T3MaterialRequirements {
	BitSetBase<1> mPasses;
	BitSet<T3MaterialChannelType, 46, 0> mChannels;
	BitSet<T3MaterialChannelType, 32, 0> mChannels2;//michonne>=
	BitSetBase<3> mInputs;//>=wd4
	BitSetBase<2> mInputs2;//batman>=
	BitSetBase<1> mInputs3;//michonne
};

struct T3MeshEffectPreloadDynamicFeatures {
	BitSetBase<1> mDynamicFeatures;
	long mPriority;
};

struct T3MeshEffectPreloadEntry {
	T3EffectType mEffectType;
	BitSetBase<3> mStaticEffectFeatures;
	u64 mMaterialCRC;
	DCArray<T3MeshEffectPreloadDynamicFeatures> mDynamicEffectFeatures;
};

struct T3MeshEffectPreload {
	long mEffectQuality;
	DCArray<T3MeshEffectPreloadEntry> mEntries;
	long mTotalEffectCount;
};

enum T3MeshEndianType {
	eMeshEndian_Default = 0,
	eMeshEndian_Swap = 1,
	eMeshEndian_SwapBytesAsWord = 2
};

struct T3MeshTexCoordTransform {
	Vector2 mScale, mOffset;
	T3MeshTexCoordTransform() {
		mScale.x = 1.0f;
		mScale.y = 1.0f;
		mOffset.x = 0.0f;
		mOffset.y = 0.0f;
	}
};

struct T3MeshCPUSkinningData {
	GFXPlatformFormat mPositionFormat, mNormalFormat, mWeightFormat;
	BitSetBase<1> mVertexStreams;
	long mNormalCount, mWeightOffset, mVertexSize, mWeightSize;//weight off/size >= wd4
	BinaryBuffer mData;
};

struct GFXPlatformAttributeParams {
	GFXPlatformVertexAttribute mAttribute;
	GFXPlatformFormat mFormat;
	//GFXPlatformVertexFrequency mFrequency;
	long mAttributeIndex;
	long mBufferIndex, mBufferOffset;
};

enum GFXPlatformBufferUsage {
	eGFXPlatformBuffer_None = 0,
	eGFXPlatformBuffer_Vertex = 1,
	eGFXPlatformBuffer_Index = 2,
	eGFXPlatformBuffer_Uniform = 4,
	eGFXPlatformBuffer_ShaderRead = 8,
	eGFXPlatformBuffer_ShaderWrite = 0x10,
	eGFXPlatformBuffer_ShaderReadWrite = 0x18,
	eGFXPlatformBuffer_ShaderRawAccess = 0x20,
	eGFXPlatformBuffer_ShaderReadRaw = 0x28,
	eGFXPlatformBuffer_ShaderWriteRaw = 0x30,
	eGFXPlatformBuffer_ShaderReadWriteRaw = 0x38,
	eGFXPlatformBuffer_DrawIndirectArgs = 0x40,
	eGFXPlatformBuffer_SingleValue = 0x80,
};

struct T3GFXBuffer {
	GFXPlatformFormat mBufferFormat;
	GFXPlatformBufferUsage mBufferUsage;
	long mStride, mCount;
	T3ResourceUsage mResourceUsage;
	void* mpCPUBuffer;

	T3GFXBuffer() {
		mpCPUBuffer = NULL;
	}

	~T3GFXBuffer() {
		if(mpCPUBuffer)
			free(mpCPUBuffer);
	}

	void AsyncRead(MetaStream* stream) {
		stream->BeginAsyncSection();
		if (mpCPUBuffer)
			free(mpCPUBuffer);
		mpCPUBuffer = malloc(mStride * mCount);
		stream->serialize_bytes(mpCPUBuffer, mStride * mCount);
		stream->EndAsyncSection();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(T3GFXBuffer, buf);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed && meta->IsWrite() && buf->mpCPUBuffer) {
			meta->BeginAsyncSection();
			meta->Key("T3 Graphics Buffer Binary Data");
			meta->serialize_bytes(buf->mpCPUBuffer, buf->mStride * buf->mCount);
			meta->EndAsyncSection();
		}
		return r;
	}

};

struct GFXPlatformVertexLayout {
	int mVertexBufferStrides[32];
	GFXPlatformAttributeParams mAttributes[32];
	GFXPlatformFormat mIndexFormat;
	int mVertexCountPerInstance, mVertexBufferCount, mAttributeCount;
};

struct T3GFXVertexState {
	long mVertexCountPerInstance, mIndexBufferCount, mVertexBufferCount, mAttributeCount;
	GFXPlatformAttributeParams mAttributes[32];
	T3GFXBuffer* mpIndexBuffer[4];
	T3GFXBuffer* mpVertexBuffer[32];

	T3GFXVertexState() {
		for (int i = 0; i < 4; i++)
			mpIndexBuffer[i] = NULL;
		for (int i = 0; i < 32; i++)
			mpVertexBuffer[i] = NULL;
	}

	~T3GFXVertexState() {
		for (int i = 0; i < 4; i++)
			if (mpIndexBuffer[i])
				delete mpIndexBuffer[i];
		for (int i = 0; i < 32; i++)
			if (mpVertexBuffer[i])
				delete mpVertexBuffer[i];
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(T3GFXVertexState, state);
		int mich = TelltaleToolLib_GetGameKeyIndex("MICHONNE");
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			if (state->mAttributeCount > 32) {
				TelltaleToolLib_RaiseError("Too many attributes in vertex state!", ErrorSeverity::ERR);
				return eMetaOp_Fail;
			}
			if (state->mVertexBufferCount > 32) {
				TelltaleToolLib_RaiseError("Too many vertex buffers in vertex state!", ErrorSeverity::ERR);
				return eMetaOp_Fail;
			}
			if (state->mIndexBufferCount > 4) {
				TelltaleToolLib_RaiseError("Too many index buffers in vertex state!", ErrorSeverity::ERR);
				return eMetaOp_Fail;
			}
			if (state->mAttributeCount) {
				MetaClassDescription* mcd = ::GetMetaClassDescription<GFXPlatformAttributeParams>();
				if (!mcd) {
					TelltaleToolLib_RaiseError("Could not find GFX platform attributes metaclass", ErrorSeverity::ERR);
					return eMetaOp_Fail;
				}
				for (int i = 0; i < state->mAttributeCount; i++) {
					r = PerformMetaSerializeFull(meta, &state->mAttributes[i], mcd);
					if (r != eMetaOp_Succeed)
						return r;
				}
			}
			if (sSetKeyIndex > mich) {
				for (int i = 0; i < state->mIndexBufferCount; i++) {
					if (meta->IsRead()) {
						if (state->mpIndexBuffer[i])
							delete state->mpIndexBuffer[i];
						state->mpIndexBuffer[i] = new T3GFXBuffer;
					}
					if (!state->mpIndexBuffer[i]) {
						TelltaleToolLib_RaiseError("Could not find or create index buffers", ErrorSeverity::ERR);
						return eMetaOp_OutOfMemory;
					}
					r = PerformMetaSerializeAsync<T3GFXBuffer>(meta, state->mpIndexBuffer[i]);
					if (r != eMetaOp_Succeed)
						return r;
				}
			}
			else{
				state->mIndexBufferCount = 1;
				bool hasIndexBuffer = state->mpIndexBuffer[0] != NULL;
				meta->serialize_bool(&hasIndexBuffer);
				if (hasIndexBuffer) {
					if (meta->IsRead()) {
						if (state->mpIndexBuffer[0])
							delete state->mpIndexBuffer[0];
						state->mpIndexBuffer[0] = new T3GFXBuffer;
					}
					else {
						if (!state->mpIndexBuffer[0])
							state->mpIndexBuffer[0] = new T3GFXBuffer;
					}
					r = PerformMetaSerializeAsync<T3GFXBuffer>(meta, state->mpIndexBuffer[0]);
					if (r != eMetaOp_Succeed)
						return r;
				}
			}
			for (int i = 0; i < state->mVertexBufferCount; i++) {
				if (meta->IsRead()) {
					if (state->mpVertexBuffer[i])
						delete state->mpVertexBuffer[i];
					state->mpVertexBuffer[i] = new T3GFXBuffer;
				}
				if (!state->mpVertexBuffer[i]) {
					TelltaleToolLib_RaiseError("Could not find or create vertex buffers", ErrorSeverity::ERR);
					return eMetaOp_OutOfMemory;
				}
				r = PerformMetaSerializeAsync<T3GFXBuffer>(meta, state->mpVertexBuffer[i]);
				if (r != eMetaOp_Succeed)
					return r;
			}
			if (meta->IsRead()) {
				for (int i = 0; i < state->mIndexBufferCount; i++) {
					T3GFXBuffer* buf = state->mpIndexBuffer[i];
					if (buf)
						buf->AsyncRead(meta);
				}
				for (int i = 0; i < state->mVertexBufferCount; i++) {
					T3GFXBuffer* buf = state->mpVertexBuffer[i];
					if (buf)
						buf->AsyncRead(meta);
				}
			}//written in perform meta serialize async
		}
		return r;
	}

};

struct T3MeshData {

	enum MeshFlags {
		eDeformable = 1,
		eHasLocalCameraFacing = 2,
		eHasLocalCameraFacingY = 4,
		eHasLocalCameraFacingLocalY = 8,
		eHasCPUSkinning = 0x10,
		eHasComputeSkinning = 0x20,
	};

	DCArray<T3MeshLOD> mLODs;
	DCArray<T3MeshTexture> mTextures;
	DCArray<T3MeshMaterial> mMaterials;
	DCArray<T3MeshMaterialOverride> mMaterialOverrides;
	DCArray<T3MeshBoneEntry> mBones;
	DCArray<T3MeshLocalTransformEntry> mLocalTransforms;
	DCArray<T3GFXVertexState*> mVertexStates;
	T3MaterialRequirements mMaterialRequirements;
	BitSetBase<1> mVertexStreams;
	T3MeshCPUSkinningData* mpCPUSkinningData;
	BoundingBox mBoundingBox;
	Sphere mBoundingSphere;
	T3MeshEndianType mEndianType;
	Vector3 mPositionScale, mPositionWScale, mPositionOffset;//all three in marvel and above
	float mLightmapTexelAreaPerSurfaceArea;
	Symbol mPropertyKeyBase;//all keys in internal resources concat onto this crc (crc with this as the start CRC64). very anoying.
	long mVertexCount;
	Flags mFlags;
	DCArray<T3MeshEffectPreload> mMeshPreload;//>=wd4
	T3MeshTexCoordTransform mTexCoordTransform[4];

	T3MeshData() {
		mpCPUSkinningData = NULL;
	}

	~T3MeshData() {
		for (int i = 0; i < mVertexStates.GetSize(); i++)
			delete mVertexStates[i];
		mVertexStates.ClearElements();
		if (mpCPUSkinningData)
			delete mpCPUSkinningData;
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(T3MeshData,data);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			if (meta->IsWrite()) {
				u32 x[4];
				u32 z = 0;
				memset(x, 0, 0x10);
				for (int y = 0; y < 4; y++) {
					if (1.0f != data->mTexCoordTransform[y].mScale.x || 1.0f != data->mTexCoordTransform[y].mScale.y
						|| 0.0f != data->mTexCoordTransform[y].mOffset.x || 0.0f != data->mTexCoordTransform[y].mOffset.y) {
						x[z++] = y;
					}
				}
				MetaClassDescription* desc = NULL;
				meta->serialize_uint32(&z);
				for (int i = 0; i < z; i++) {
					if (!desc) {
						desc = ::GetMetaClassDescription<T3MeshTexCoordTransform>();
						if (!desc) {
							TelltaleToolLib_RaiseError("Cant find tex coordinate transform metaclass", ErrorSeverity::ERR);
							return eMetaOp_Fail;
						}
					}
					meta->serialize_uint32(&x[i]);
					r = PerformMetaSerializeFull(meta, &data->mTexCoordTransform[x[i]], desc);
					if (r != eMetaOp_Succeed)
						return r;
				}
			}
			else {
				MetaClassDescription* desc = ::GetMetaClassDescription<T3MeshTexCoordTransform>();
				if (!desc) {
					TelltaleToolLib_RaiseError("Cant find tex coordinate transform metaclass", ErrorSeverity::ERR);
					return eMetaOp_Fail;
				}
				u32 transforms;
				meta->serialize_uint32(&transforms);
				u32 v;
				for (int i = 0; i < transforms; i++) {
					meta->serialize_uint32(&v);
					r = PerformMetaSerializeFull(meta, &data->mTexCoordTransform[v], desc);
					if (r != eMetaOp_Succeed)
						return r;
				}
				if (data->mFlags.mFlags & MeshFlags::eHasCPUSkinning) {
					if (data->mpCPUSkinningData && meta->IsRead())
						delete data->mpCPUSkinningData;
					if (meta->IsRead())
						data->mpCPUSkinningData = new T3MeshCPUSkinningData;
					r = PerformMetaSerializeAsync<T3MeshCPUSkinningData>(meta, data->mpCPUSkinningData);
					if (r != eMetaOp_Succeed)
						return r;
				}
			}
			u32 v = data->mVertexStates.GetSize();
			meta->serialize_uint32(&v);
			T3GFXVertexState* state;
			for (int i = 0; i < v; i++) {
				if (meta->IsRead()) {
					state = new T3GFXVertexState;
					if (!state)
						return eMetaOp_OutOfMemory;
					data->mVertexStates.AddElement(0, NULL, &state);
				}
				else {
					state = data->mVertexStates[i];
				}
				r = PerformMetaSerializeAsync<T3GFXVertexState>(meta, state);
				if (r != eMetaOp_Succeed)
					return r;
			}
		}
		return r;
	}

};

struct T3OcclusionMeshBatch {
	Flags mFlags;
	long mStartIndex, mNumTriangles;
};

struct T3OcclusionMeshData {
	BinaryBuffer mData;
	BoundingBox mBoundingBox;
	Sphere mBoundingSphere;
	DCArray<T3OcclusionMeshBatch> mBatches;
	long mVertexCount;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(T3OcclusionMeshData, occ);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			//... not needed
		}
		return r;
	}

};

//.D3DMESH FILES
struct D3DMesh {

	String mName;
	long mVersion;
	T3MeshData mMeshData;
	DCArray<HandleUncached> mInternalResources;
	ToolProps mToolProps;
	//below is only in games newer than WD michonne (exclusive)
	EnumRenderLightmapUVGenerationType mLightmapUVGenerationType;//batman2 and below
	float mLightmapTexelAreaPerSurfaceArea;//in games older and including batman2
	float mLightmapGlobalScale;
	long mLightmapTexCoordVersion;
	//MeshDebugRenderType mType;//batman2 and below,99%sure
	long mLightmapTextureWidth, mLightmapTextureHeight;//batman2 and below
	u64 mLODParamCRC;//wd4+
	T3OcclusionMeshData* mpOcclusionMeshData;

	D3DMesh() {
		mpOcclusionMeshData = NULL;
		mLightmapGlobalScale = 1.0f;
		mLightmapTextureWidth = mLightmapTextureHeight = 0;
	}

	PropertySet* GetMeshProps() {
		String propsResource = "\"";
		propsResource.append(mName);
		propsResource.append("\" Mesh Properties");
		Symbol sym(propsResource.c_str());
		for (int i = 0; i < mInternalResources.GetSize(); i++) {
			HandleUncached& handle = mInternalResources[i];
			if (handle.GetObjectName().operator==(sym)) {
				return handle.GetValue<PropertySet>();
			}
			if (handle.GetTypeDesc() == ::GetMetaClassDescription<PropertySet>())
				return (PropertySet*)handle.GetHandleObjectPointer();
		}
		return NULL;
	}

	T3MaterialData* GetMaterialData(PropertySet* pMeshProps) {
		if (!pMeshProps)
			return NULL;
		return pMeshProps->GetProperty<T3MaterialData>(
			T3MaterialInternal::kPropKeyMaterialData);
	}

	~D3DMesh() {
		if (mpOcclusionMeshData)
			delete mpOcclusionMeshData;
		//for (int i = 0; i < mInternalResources.GetSize(); i++) {
		//	delete mInternalResources[i];
		//}
		mInternalResources.ClearElements();
	}

	static MetaOpResult SerializeAsyncInternalResources(MetaStream* meta, D3DMesh* mesh) {
		u32 res = mesh->mInternalResources.GetSize();
		MetaOpResult r = eMetaOp_Succeed;
		meta->serialize_uint32(&res);
		if (meta->IsRead()) {
			Symbol name{};
			MetaClassDescription* mcd = NULL;
			u64 symbolCrc = 0;
			for (int i = 0; i < res; i++) {
				meta->serialize_Symbol(&name);
				meta->serialize_uint64(&symbolCrc);
				mcd = TelltaleToolLib_FindMetaClassDescription_ByHash(symbolCrc);
				meta->BeginBlock();
				if (!mcd) {
					//normally raise a warn (telltale just ignore the resource) if its an invalid type
#ifdef DEBUGMODE
					printf("Mesh internal resource type not found from symbol %llX\n", symbolCrc);
#endif
					TelltaleToolLib_RaiseError("D3DMesh internal resource type not found", ErrorSeverity::ERR);
					return eMetaOp_Fail;
				}
				void* inst = mcd->New();
				if (!inst)
					return eMetaOp_OutOfMemory;
				//HandleUncached* handle = new HandleUncached(mcd, inst);
				//if (!handle) {
				//	mcd->Delete(inst);
				//	return eMetaOp_OutOfMemory;
				//}
				HandleUncached handle{ mcd, inst };
				handle.SetObjectName(name);
				r=PerformMetaSerializeFull(meta, handle.GetHandleObjectPointer(), handle.GetTypeDesc());
				if (r != eMetaOp_Succeed) {
					//delete handle;
					return r;
				}
				mesh->mInternalResources.AddElementMove(0, NULL, &handle);
				meta->EndBlock();
			}
		}
		else {
			for (int i = 0; i < res; i++) {
				meta->serialize_Symbol(&mesh->mInternalResources[i].mHandleObjectInfo.mObjectName);
				meta->serialize_uint64(&mesh->mInternalResources[i].GetTypeDesc()->mHash);
				meta->BeginBlock();
				r=PerformMetaSerializeFull(meta, mesh->mInternalResources[i].GetHandleObjectPointer(), 
					mesh->mInternalResources[i].GetTypeDesc());
				if (r != eMetaOp_Succeed)
					return r;
				meta->EndBlock();
			}
		}
		return eMetaOp_Succeed;
	}

#define ENDBATFIX() if(batfix)sSetKeyIndex = TelltaleToolLib_GetGameKeyIndex("BATMAN");

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(D3DMesh, mesh);
		bool batfix = false;
		if (sSetKeyIndex == TelltaleToolLib_GetGameKeyIndex("BATMAN")) {
			batfix = true;
			sSetKeyIndex = TelltaleToolLib_GetGameKeyIndex("WD4");
		}
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			if (batfix && mesh->mVersion != 46) {
				TelltaleToolLib_RaiseError("Cannot serialize D3DMesh: Only BAT Season 1's latest PC release is supported",ErrorSeverity::ERR);
				ENDBATFIX();
				return eMetaOp_Fail;
			}
			if (mesh->mVersion >= 22) {
				r = SerializeAsyncInternalResources(meta, mesh);
				if (r != eMetaOp_Succeed) {
					ENDBATFIX();
					return r;
				}
			}
			//MCSM
			if (false) {

			}else{ 
				if (19 > mesh->mVersion) {
					//TODO not supported in WDC
					TelltaleToolLib_RaiseError("Cannot serialize D3DMeshes from games older and including MC: Story Mode... with D3DMesh. See LegacyD3DMesh!", ErrorSeverity::ERR);
					ENDBATFIX();
					return eMetaOp_Fail;
				}
				u32 hasToolData = 0;
				meta->serialize_uint32(&hasToolData);
				if (hasToolData) {
					meta->BeginBlock();
					TelltaleToolLib_RaiseError("Mesh contains tool data; "
						"skipping block (safely)",
						ErrorSeverity::WARN);
					meta->SkipToEndOfCurrentBlock();
					meta->EndBlock();
				}
				if (mesh->mVersion >= 52) {
					bool hasOcclusionData = mesh->mpOcclusionMeshData != NULL;
					meta->serialize_bool(&hasOcclusionData);
					if (hasOcclusionData) {
						meta->BeginBlock();
						if (!mesh->mpOcclusionMeshData)
							mesh->mpOcclusionMeshData = new T3OcclusionMeshData;
						r = PerformMetaSerializeAsync<T3OcclusionMeshData>(meta, mesh->mpOcclusionMeshData);
						if (r != eMetaOp_Succeed) {
							ENDBATFIX();
							return r;
						}
						meta->EndBlock();
					}
				}
				meta->BeginBlock();
				r = PerformMetaSerializeAsync<T3MeshData>(meta, &mesh->mMeshData);
				if (r != eMetaOp_Succeed) {
					ENDBATFIX();
					return r;
				}
				meta->EndBlock();
			}
		}
		ENDBATFIX();
		return r;
	}

};

#endif