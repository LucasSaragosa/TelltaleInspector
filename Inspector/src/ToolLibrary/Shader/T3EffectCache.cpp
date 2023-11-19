#include "T3EffectCache.h"

// LITTLE ENDIAN MACHINE ASSUMED

bool T3EffectCacheInternal::LoadPackage(T3EffectCachePackage& package, DataStream& stream)
{
	if (!LoadPackageHeader(package.mHeader, stream))
		return false;
	if (package.mHeader.mVersion < 196)
		return false;//too old, only supports WDC
	package.mPrograms.Clear(0);
	package.mShaders.Clear(0);
	if (!package.mPrograms.Resize(package.mHeader.mProgramCount))
		return false;
	if(!package.mPrograms.Resize(package.mHeader.mShaderCount))
		return false;
	T3EffectCachePackageProgram program{};
	T3EffectCachePackageShader shader{};
	for (int i = 0; i < package.mHeader.mShaderCount; i++) {
		if (!LoadPackageShader(shader, stream))
			return false;
		package.mShaders.AddElement(0, 0, &shader);
	}
	//for (int i = 0; i < package.mHeader.mProgramCount; i++) {
	//	if (!LoadPackageProgram(program, stream))
	//		return false;
	//	package.mPrograms.AddElement(0, 0, &program);
	//}
	return true;
}

bool T3EffectCacheInternal::LoadPackageHeader(T3EffectCachePackageHeader& p, DataStream& stream)
{
	char buf[8];
	if (!stream.Serialize(buf, 4))
		return false;
	if (*((unsigned int*)buf) != 0x844FF3C8)
		return false;//bad header
	p.mMagic = *((unsigned int*)buf);
	if (!stream.Serialize(buf, 4))
		return false;
	p.mVersion = *((unsigned int*)buf);
	if (!stream.Serialize(buf, 4))
		return false;
	p.mProgramCount = *((unsigned int*)buf);
	if (!stream.Serialize(buf, 4))
		return false;
	p.mShaderCount = *((unsigned int*)buf);
	if (!stream.Serialize(buf, 4))
		return false;
	p.mShaderDataOffset = *((unsigned int*)buf);
	return true;
}

bool T3EffectCacheInternal::LoadPackageShader(T3EffectCachePackageShader& p, DataStream& stream)
{
	char buf[4 * 5];
	if (!stream.Serialize(buf, 4))
		return false;
	p.mShaderType = (T3MaterialShaderType) * ((unsigned int*)buf);
	if (!stream.Serialize(buf, 4 * 5))
		return false;
	p.mNeededParameters.mWords[0] = *((unsigned int*)buf);
	p.mNeededParameters.mWords[1] = *((unsigned int*)(buf + 4));
	p.mNeededParameters.mWords[2] = *((unsigned int*)(buf + 8));
	p.mNeededParameters.mWords[3] = *((unsigned int*)(buf + 12));
	p.mNeededParameters.mWords[4] = *((unsigned int*)(buf + 16));
	if (!stream.Serialize(buf, 4))
		return false;
	p.mOffset = *((unsigned int*)buf);
	if (!stream.Serialize(buf, 4))
		return false;
	p.mSize = *((unsigned int*)buf);
	return true;
}

bool T3EffectCacheInternal::LoadPackageProgram(T3EffectCachePackageProgram& p, DataStream& stream)
{
	char buf[4*3];
	if (!stream.Serialize(buf, 4))
		return false;
	p.mEffectType = (T3EffectType) * ((unsigned int*)buf);
	if (!stream.Serialize(buf, 4 * 3))
		return false;
	p.mEffectFeatures.mWords[0] = *((unsigned int*)buf);
	p.mEffectFeatures.mWords[1] = *((unsigned int*)(buf+4));
	p.mEffectFeatures.mWords[2] = *((unsigned int*)(buf+8));
	if (!stream.Serialize(buf, 8))
		return false;
	p.mMaterialCrc = *((unsigned long long*)buf);
	if (!stream.Serialize(buf, 4))
		return false;
	p.mEffectQuality = (T3EffectQuality) * ((unsigned int*)buf);
	if (!stream.Serialize(buf, 4))
		return false;
	p.mPassCount = * ((unsigned int*)buf);
	if (!stream.Serialize(buf, 4))
		return false;
	p.mSamplerCount = *((unsigned int*)buf);
	return true;
}

bool T3EffectCacheInternal::LoadShaderPayload(const T3EffectCachePackage& p, const T3EffectCachePackageShader& shader,
	char* pDest, DataStream& stream)
{
	if (!stream.SetPosition(shader.mOffset, DataStreamSeekType::eSeekType_Begin))//todo check
		return false;
	return stream.Serialize(pDest, shader.mSize);
}

unsigned int T3EffectCacheInternal::GetShaderPayloadSize(const T3EffectCachePackageShader& shader)
{
	return shader.mSize;
}

struct _T3EffectFeatureDesc {
	const char* mName;
	T3EffectFeature mFeature;
};

struct _T3EffectParameterTypeDesc {
	const char* mName;
	T3EffectParameterType mParamType;
};

struct _EnumDesc {
	const char* mName;
	int mVal;
};

_T3EffectFeatureDesc sFeatureDesc[] = {
	//STATIC
	{"None", T3EffectFeature::eEffectFeature_None},
	{"Tool", T3EffectFeature::eEffectFeature_Tool},
	{"Deformable", T3EffectFeature::eEffectFeature_Deformable},
	{"Deformable Single", T3EffectFeature::eEffectFeature_DeformableSingle},
	{"Deformable Compute", T3EffectFeature::eEffectFeature_DeformableCompute},
	{"Light Map", T3EffectFeature::eEffectFeature_LightMap},
	{"Static Shadow Map", T3EffectFeature::eEffectFeature_StaticShadowMap},
	{"Vertex Color", T3EffectFeature::eEffectFeature_VertexColor},
	{"Soft Depth Test", T3EffectFeature::eEffectFeature_SoftDepthTest},
	{"Supports HBAO", T3EffectFeature::eEffectFeature_SupportsHBAO},
	{"Particle Facing", T3EffectFeature::eEffectFeature_ParticleFacing},
	{"Particle Facing 3D", T3EffectFeature::eEffectFeature_ParticleFacing3D},
	{"Particle Smooth Animation", T3EffectFeature::eEffectFeature_ParticleSmoothAnimation},
	{"Particle Rotation 3D", T3EffectFeature::eEffectFeature_ParticleRotation3D},
	{"Particle Rotation 3D Texture", T3EffectFeature::eEffectFeature_ParticleRotation3DTexture},
	{"Particle Texture Coord", T3EffectFeature::eEffectFeature_ParticleTexCoord},
	{"Post Levels", T3EffectFeature::eEffectFeature_PostLevels},
	{"Post Radial Blur", T3EffectFeature::eEffectFeature_PostRadialBlur},
	{"Post Tonemap", T3EffectFeature::eEffectFeature_PostTonemap},
	{"Post Tonemap Intensity", T3EffectFeature::eEffectFeature_PostTonemapIntensity},
	{"Post Tonemap Filmic", T3EffectFeature::eEffectFeature_PostTonemapFilmic},
	{"Post Tonemap Filmic RGB", T3EffectFeature::eEffectFeature_PostTonemapFilmicRGB},
	{"Post Vignette Tint", T3EffectFeature::eEffectFeature_PostVignetteTint},
	{"Post Depth of Field 1x", T3EffectFeature::eEffectFeature_PostDepthOfField1x},
	{"Post Depth of Field 2x", T3EffectFeature::eEffectFeature_PostDepthOfField2x},
	{"Post Depth of Field 3x", T3EffectFeature::eEffectFeature_PostDepthOfField3x},
	{"Post New Depth of Field", T3EffectFeature::eEffectFeature_PostNewDepthOfField},
	{"Post New Depth of Field - Low", T3EffectFeature::eEffectFeature_PostNewDepthOfField_Low},
	{"Post New Depth of Field - Medium", T3EffectFeature::eEffectFeature_PostNewDepthOfField_Medium},
	{"Post New Depth of Field - High", T3EffectFeature::eEffectFeature_PostNewDepthOfField_High},
	{"Post New Depth of Field - No MRT", T3EffectFeature::eEffectFeature_PostNewDepthOfField_NoMRT},
	{"Post New Depth of Field - MC Legacy", T3EffectFeature::eEffectFeature_PostNewDepthOfFieldMCLegacy},
	{"Post Bokeh - Uses GS", T3EffectFeature::eEffectFeature_PostBokehUsesGS},
	{"Post Bokeh - Uses Aberration", T3EffectFeature::eEffectFeature_PostBokehUsesAberration},
	{"Boken Z Test", T3EffectFeature::eEffectFeature_BokehZTest},
	{"Bokeh Scale Occluded", T3EffectFeature::eEffectFeature_BokehScaleOccluded},
	{"Post Bokeh", T3EffectFeature::eEffectFeature_PostBokeh},
	{"Post Debug HBAO", T3EffectFeature::eEffectFeature_PostDebugHBAO},
	{"Post Convert sRGB", T3EffectFeature::eEffectFeature_PostConvert_sRGB},
	{"Post HDR 10", T3EffectFeature::eEffectFeature_PostHDR10},
	{"Post HDR Multi", T3EffectFeature::eEffectFeature_PostHDRMulti},
	{"Depth of Field Vignette", T3EffectFeature::eEffectFeature_DepthOfFieldVignette},
	{"Post Brush Outline", T3EffectFeature::eEffectFeature_PostBrushOutline},
	{"Post Brush Depth of Field", T3EffectFeature::eEffectFeature_PostBrushDepthOfField},
	{"Post Texture Array", T3EffectFeature::eEffectFeature_PostTextureArray},
	{"Post Shadow Input 2x", T3EffectFeature::eEffectFeature_PostShadowInput2x},
	{"Post Shadow SDSM", T3EffectFeature::eEffectFeature_PostShadowSDSM},
	{"Shadow Apply Tile", T3EffectFeature::eEffectFeature_ShadowApplyTile},
	{"Shadow Apply Modulated", T3EffectFeature::eEffectFeature_ShadowApplyModulated},
	{"Shadow Apply Gobo", T3EffectFeature::eEffectFeature_ShadowApplyGobo},
	{"HBAO Deinterleaved", T3EffectFeature::eEffectFeature_HBAODeinterleaved},
	{"HBAO Deinterleaved 2x", T3EffectFeature::eEffectFeature_HBAODeinterleaved2x},
	{"HBAO Deinterleaved 4x", T3EffectFeature::eEffectFeature_HBAODeinterleaved4x},
	{"Bake Flat Lighting", T3EffectFeature::eEffectFeature_BakeFlatLighting},
	//DYNAMIC
	{"Rim Light", T3EffectFeature::eEffectFeature_RimLight},
	{"Specular Environment", T3EffectFeature::eEffectFeature_SpecularEnvironment},
	{"Shadow", T3EffectFeature::eEffectFeature_Shadow},
	{"Environment Lighting", T3EffectFeature::eEffectFeature_EnvLighting},
	{"Cinematic Lighting", T3EffectFeature::eEffectFeature_CinLighting},
	{"Environment Key Light", T3EffectFeature::eEffectFeature_EnvKeyLight},
	{"Environment Key Shadow", T3EffectFeature::eEffectFeature_EnvKeyShadow},
	{"Environment Light 2x", T3EffectFeature::eEffectFeature_EnvLight2x},
	{"Environment Light 3x", T3EffectFeature::eEffectFeature_EnvLight3x},
	{"Enlighten Map", T3EffectFeature::eEffectFeature_EnlightenMap},
	{"Glow Output", T3EffectFeature::eEffectFeature_GlowOutput},
	{"GBuffer Ouput", T3EffectFeature::eEffectFeature_GBufferOutput},
	{"Decal Apply", T3EffectFeature::eEffectFeature_DecalApply},
	{"Clip Plane", T3EffectFeature::eEffectFeature_ClipPlane},
	{"Camera Clip Planes", T3EffectFeature::eEffectFeature_CameraClipPlanes},
	{"Shadow Cascades", T3EffectFeature::eEffectFeature_ShadowCascades},
	{"Shadow Depth 2", T3EffectFeature::eEffectFeature_ShadowDepth2},
	{"Shadow Output Color", T3EffectFeature::eEffectFeature_ShadowOutputColor},
	{"Alpha Test", T3EffectFeature::eEffectFeature_AlphaTest},
	{"MSAA Alpha Test", T3EffectFeature::eEffectFeature_MSAAAlphaTest},
	{"Dissolve", T3EffectFeature::eEffectFeature_Dissolve},
	{"Sharp Shadows", T3EffectFeature::eEffectFeature_SharpShadows},
	{"Linear Depth Fetch", T3EffectFeature::eEffectFeature_LinearDepthFetch},
	{"Frame Buffer Fetch", T3EffectFeature::eEffectFeature_FrameBufferFetch},
	{"Depth Buffer Fetch", T3EffectFeature::eEffectFeature_DepthBufferFetch},
	{"Crease Lines", T3EffectFeature::eEffectFeature_CreaseLines},
	{"Boundary Lines", T3EffectFeature::eEffectFeature_BoundaryLines},
	{"Jagged Lines", T3EffectFeature::eEffectFeature_JaggedLines},
	{"Smooth Lines", T3EffectFeature::eEffectFeature_SmoothLines},
	{"Debug", T3EffectFeature::eEffectFeature_Debug},
};

_T3EffectParameterTypeDesc sParamTypeDesc[] = {
	{"Unknown", T3EffectParameterType::eEffectParameter_Unknown},
	//{"First Uniform Buffer", T3EffectParameterType::eEffectParameter_FirstUniformBuffer},
	{"Uniform Buffer - Camera", T3EffectParameterType::eEffectParameter_UniformBufferCamera},
	{"Uniform Buffer - Scene", T3EffectParameterType::eEffectParameter_UniformBufferScene},
	{"Uniform Buffer - Scene Tool", T3EffectParameterType::eEffectParameter_UniformBufferSceneTool},
	{"Uniform Buffer - Object", T3EffectParameterType::eEffectParameter_UniformBufferObject},
	{"Uniform Buffer - Instance", T3EffectParameterType::eEffectParameter_UniformBufferInstance},
	{"Uniform Buffer - Lights", T3EffectParameterType::eEffectParameter_UniformBufferLights},
	{"Uniform Buffer - Lights Environment", T3EffectParameterType::eEffectParameter_UniformBufferLightEnv},
	{"Uniform Buffer - Lights Cinematic", T3EffectParameterType::eEffectParameter_UniformBufferLightCin},
	{"Uniform Buffer - Skinning", T3EffectParameterType::eEffectParameter_UniformBufferSkinning},
	{"Uniform Buffer - Shadow", T3EffectParameterType::eEffectParameter_UniformBufferShadow},
	{"Uniform Buffer - Particle", T3EffectParameterType::eEffectParameter_UniformBufferParticle},
	{"Uniform Buffer - Decal", T3EffectParameterType::eEffectParameter_UniformBufferDecal},
	{"Uniform Buffer - Simple", T3EffectParameterType::eEffectParameter_UniformBufferSimple},
	{"Uniform Buffer - Gaussian", T3EffectParameterType::eEffectParameter_UniformBufferGaussian},
	{"Uniform Buffer - Post", T3EffectParameterType::eEffectParameter_UniformBufferPost},
	{"Uniform Buffer - Brush", T3EffectParameterType::eEffectParameter_UniformBufferBrush},
	{"Uniform Buffer - Light Environment Data - High", T3EffectParameterType::eEffectParameter_UniformBufferLightEnvData_High},
	{"Uniform Buffer - Light Environment Data - Medium", T3EffectParameterType::eEffectParameter_UniformBufferLightEnvData_Medium},
	{"Uniform Buffer - Light Environment Data - Low", T3EffectParameterType::eEffectParameter_UniformBufferLightEnvData_Low},
	{"Uniform Buffer - Light Environment View", T3EffectParameterType::eEffectParameter_UniformBufferLightEnvView},
	{"Uniform Buffer - Light Ambient", T3EffectParameterType::eEffectParameter_UniformBufferLightAmbient },
	{"Uniform Buffer - Shadow Volume", T3EffectParameterType::eEffectParameter_UniformBufferShadowVolume},
	{"Uniform Buffer - Mesh", T3EffectParameterType::eEffectParameter_UniformBufferMesh},
	{"Uniform Buffer - Mesh Batch", T3EffectParameterType::eEffectParameter_UniformBufferMeshBatch},
	{"Uniform Buffer - Mesh Dynamic Batch", T3EffectParameterType::eEffectParameter_UniformBufferMeshDynamicBatch},
	{"Uniform Buffer - Mesh Debug Batch", T3EffectParameterType::eEffectParameter_UniformBufferMeshDebugBatch},
	{"Uniform Buffer - HBAO", T3EffectParameterType::eEffectParameter_UniformBufferHBAO},
	{"Uniform Buffer - Material Tool", T3EffectParameterType::eEffectParameter_UniformBufferMaterialTool},
	{"Uniform Buffer - Material Base", T3EffectParameterType::eEffectParameter_UniformBufferMaterialBase},
	{"Uniform Buffer - Material Main", T3EffectParameterType::eEffectParameter_UniformBufferMaterialMain},

	{"Generic Buffer - Skinning", T3EffectParameterType::eEffectParameter_GenericBufferSkinning},
	{"Generic Buffer - 0 Vertex In", T3EffectParameterType::eEffectParameter_GenericBuffer0VertexIn},
	{"Generic Buffer - 1 Vertex Out", T3EffectParameterType::eEffectParameter_GenericBuffer1VertexIn},
	{"Generic Buffer - Vertex Out", T3EffectParameterType::eEffectParameter_GenericBufferVertexOut},
	{"Generic Buffer - Mesh Bounds", T3EffectParameterType::eEffectParameter_GenericBufferMeshBounds},
	{"Generic Buffer - Indices", T3EffectParameterType::eEffectParameter_GenericBufferIndices},
	{"Generic Buffer - Indirect Arguments", T3EffectParameterType::eEffectParameter_GenericBufferIndirectArgs},
	{"Generic Buffer - Start Instance", T3EffectParameterType::eEffectParameter_GenericBufferStartInstance},
	{"Generic Buffer - Depth Range", T3EffectParameterType::eEffectParameter_GenericBufferDepthRange},
	{"Generic Buffer - Previous Depth Range", T3EffectParameterType::eEffectParameter_GenericBufferPrevDepthRange},
	{"Generic Buffer - Light Grid", T3EffectParameterType::eEffectParameter_GenericBufferLightGrid},
	{"Generic Buffer - Light Z Bin", T3EffectParameterType::eEffectParameter_GenericBufferLightZBin},
	{"Generic Buffer - Light Group Mask", T3EffectParameterType::eEffectParameter_GenericBufferLightGroupMask},
	{"Generic Buffer - Shadow Cascades", T3EffectParameterType::eEffectParameter_GenericBufferShadowCascades},
	{"Generic Buffer - Cinematic Shadow Data", T3EffectParameterType::eEffectParameter_GenericBufferCinShadowData},
	{"Generic Buffer - Duster Data", T3EffectParameterType::eEffectParameter_GenericBufferDusterData},
	{"Generic Buffer - Duster Visibility", T3EffectParameterType::eEffectParameter_GenericBufferDusterVisibility},
	{"Generic Buffer - Waveform Monitor", T3EffectParameterType::eEffectParameter_GenericBufferWaveformMonitor},
	{"Generic Buffer - Input 0", T3EffectParameterType::eEffectParameter_GenericBufferInput0},
	{"Generic Buffer - Input 1", T3EffectParameterType::eEffectParameter_GenericBufferInput1},
	{"Generic Buffer - Input 2", T3EffectParameterType::eEffectParameter_GenericBufferInput2},
	{"Generic Buffer - Input 3", T3EffectParameterType::eEffectParameter_GenericBufferInput3},
	{"Generic Buffer - Input 4", T3EffectParameterType::eEffectParameter_GenericBufferInput4},
	{"Generic Buffer - Input 5", T3EffectParameterType::eEffectParameter_GenericBufferInput5},
	{"Generic Buffer - Input 6", T3EffectParameterType::eEffectParameter_GenericBufferInput6},
	{"Generic Buffer - Input 7", T3EffectParameterType::eEffectParameter_GenericBufferInput7},
	{"Generic Buffer - Output 0", T3EffectParameterType::eEffectParameter_GenericBufferOutput0},
	{"Generic Buffer - Output 1", T3EffectParameterType::eEffectParameter_GenericBufferOutput1},
	{"Generic Buffer - Output 2", T3EffectParameterType::eEffectParameter_GenericBufferOutput2},
	{"Generic Buffer - Output 3", T3EffectParameterType::eEffectParameter_GenericBufferOutput3},
	{"Generic Buffer - Output 4", T3EffectParameterType::eEffectParameter_GenericBufferOutput4},
	{"Sampler - Diffuse", T3EffectParameterType::eEffectParameter_SamplerDiffuse},
	{"Sampler - Static Shadowmap", T3EffectParameterType::eEffectParameter_SamplerStaticShadowmap},
	{"Sampler - Shadowmap", T3EffectParameterType::eEffectParameter_SamplerShadowmap},
	{"Sampler - Projected", T3EffectParameterType::eEffectParameter_SamplerProjected},
	{"Sampler - Brush Near", T3EffectParameterType::eEffectParameter_SamplerBrushNear},
	{"Sampler - Brush Far", T3EffectParameterType::eEffectParameter_SamplerBrushFar},
	{"Sampler - Environment", T3EffectParameterType::eEffectParameter_SamplerEnvironment},
	{"Sampler - Bokeh Pattern", T3EffectParameterType::eEffectParameter_SamplerBokehPattern},
	{"Sampler - Noise LUT", T3EffectParameterType::eEffectParameter_SamplerNoiseLUT},
	{"Sampler - HLS Movie Y", T3EffectParameterType::eEffectParameter_SamplerHLSMovieY},
	{"Sampler - HLS Movie C", T3EffectParameterType::eEffectParameter_SamplerHLSMovieC},
	{"Sampler - HLS Movie RGB", T3EffectParameterType::eEffectParameter_SamplerHLSMovieRGB},
	{"Sampler - Back Buffer", T3EffectParameterType::eEffectParameter_SamplerBackbuffer},
	{"Sampler - Back Buffer HDR", T3EffectParameterType::eEffectParameter_SamplerBackbufferHDR},
	{"Sampler - Back Buffer HDR Previous", T3EffectParameterType::eEffectParameter_SamplerBackbufferHDRPrev},
	{"Sampler - Back Buffer HDR Resolved", T3EffectParameterType::eEffectParameter_SamplerBackbufferHDRResolved},
	{"Sampler - Depth Buffer", T3EffectParameterType::eEffectParameter_SamplerDepthbuffer},
	{"Sampler - Linear Depth", T3EffectParameterType::eEffectParameter_SamplerLinearDepth},
	{"Sampler - Linear Depth Previous", T3EffectParameterType::eEffectParameter_SamplerLinearDepthPrev},
	{"Sampler - Alpha Mesh Linear Depth", T3EffectParameterType::eEffectParameter_SamplerAlphaMeshLinearDepth},
	{"Sampler - Stencil", T3EffectParameterType::eEffectParameter_SamplerStencil},
	{"Sampler - GBuffer 0", T3EffectParameterType::eEffectParameter_SamplerGBuffer0},
	{"Sampler - GBuffer 1", T3EffectParameterType::eEffectParameter_SamplerGBuffer1},
	{"Sampler - DBuffer 0", T3EffectParameterType::eEffectParameter_SamplerDBuffer0},
	{"Sampler - DBuffer 1", T3EffectParameterType::eEffectParameter_SamplerDBuffer1},
	{"Sampler - Deferred Shadows", T3EffectParameterType::eEffectParameter_SamplerDeferredShadows},
	{"Sampler - Deferred Modulated Shadows", T3EffectParameterType::eEffectParameter_SamplerDeferredModulatedShadows},
	{"Sampler - Deferred Shadows Previous", T3EffectParameterType::eEffectParameter_SamplerDeferredShadowsPrev},
	{"Sampler - Deferred Light 0", T3EffectParameterType::eEffectParameter_SamplerDeferredLight0},
	{"Sampler - Defered Light 1", T3EffectParameterType::eEffectParameter_SamplerDeferredLight1},
	{"Sampler - Depth of Field - Blur 1x", T3EffectParameterType::eEffectParameter_SamplerDofBlur1x},
	{"Sampler - Depth of Field - Blur 2x", T3EffectParameterType::eEffectParameter_SamplerDofBlur2x},
	{"Sampler - Depth of Field - Blur 3x", T3EffectParameterType::eEffectParameter_SamplerDofBlur3x},
	{"New Depth of Field - Half", T3EffectParameterType::eEffectParameter_NewDepthOfFieldHalf},
	{"New Depth of Field - Near H", T3EffectParameterType::eEffectParameter_NewDofNearH},
	{"New Depth of Field - Near V", T3EffectParameterType::eEffectParameter_NewDofNearV},
	{"New Depth of Field - Far H1x", T3EffectParameterType::eEffectParameter_NewDofFarH1x},
	{ "Sampler - New Depth of Field - Far V1x", T3EffectParameterType::eEffectParameter_NewDofFarV1x },
	{ "Bokeh", T3EffectParameterType::eEffectParameter_Bokeh },
	{ "SS Lines", T3EffectParameterType::eEffectParameter_SSLines },
	{ "Sampler - Light Environment Shadow Gobo", T3EffectParameterType::eEffectParameter_SamplerLightEnvShadowGobo },
	{ "Sampler - Static Shadow Volume", T3EffectParameterType::eEffectParameter_SamplerStaticShadowVolume},
	{ "Shadow EVSM Sampler - Shadow 0", T3EffectParameterType::eEffectParameter_SamplerEVSMShadow0 },
	{ "Shadow EVSM Sampler - Shadow 1", T3EffectParameterType::eEffectParameter_SamplerEVSMShadow1 },
	{ "Sampler - SMAA Area Lookup", T3EffectParameterType::eEffectParameter_SamplerSMAAAreaLookup },
	{ "Sampler - SMAA Search Lookup", T3EffectParameterType::eEffectParameter_SamplerSMAASearchLookup },
	{ "Sampler - Random", T3EffectParameterType::eEffectParameter_SamplerRandom },
	{ "Sampler - Noise", T3EffectParameterType::eEffectParameter_SamplerNoise },
	{ "Sampler - Tetrahedral Lookup", T3EffectParameterType::eEffectParameter_SamplerTetrahedralLookup},
	{ "Sampler - Lightmap", T3EffectParameterType::eEffectParameter_SamplerLightmap },
	{ "Sampler - Lightmap Flat", T3EffectParameterType::eEffectParameter_SamplerLightmapFlat },
	{ "Sampler - Bone Matrices", T3EffectParameterType::eEffectParameter_SamplerBoneMatrices },
	{ "Sampler - Debug Overlay", T3EffectParameterType::eEffectParameter_SamplerDebugOverlay },
	{ "Sampler - Software Occlusion", T3EffectParameterType::eEffectParameter_SamplerSoftwareOcclusion },
	{ "Input Material Sampler - Input 0", T3EffectParameterType::eEffectParameter_SamplerMaterialInput0 },
	{ "Input Material Sampler - Input 1", T3EffectParameterType::eEffectParameter_SamplerMaterialInput1 },
	{ "Input Material Sampler - Input 2", T3EffectParameterType::eEffectParameter_SamplerMaterialInput2 },
	{ "Input Material Sampler - Input 3", T3EffectParameterType::eEffectParameter_SamplerMaterialInput3 },
	{ "Input Material Sampler - Input 4", T3EffectParameterType::eEffectParameter_SamplerMaterialInput4 },
	{ "Input Material Sampler - Input 5", T3EffectParameterType::eEffectParameter_SamplerMaterialInput5 },
	{ "Input Material Sampler - Input 6", T3EffectParameterType::eEffectParameter_SamplerMaterialInput6 },
	{ "Input Material Sampler - Input 7", T3EffectParameterType::eEffectParameter_SamplerMaterialInput7 },
	{ "Input Material Sampler - Input 8", T3EffectParameterType::eEffectParameter_SamplerMaterialInput8 },
	{ "Input Material Sampler - Input 9", T3EffectParameterType::eEffectParameter_SamplerMaterialInput9 },
	{ "Input Material Sampler - Input 10", T3EffectParameterType::eEffectParameter_SamplerMaterialInput10 },
	{ "Input Material Sampler - Input 11", T3EffectParameterType::eEffectParameter_SamplerMaterialInput11 },
	{ "Input Material Sampler - Input 12", T3EffectParameterType::eEffectParameter_SamplerMaterialInput12 },
	{ "Input Material Sampler - Input 13", T3EffectParameterType::eEffectParameter_SamplerMaterialInput13 },
	{ "Input Material Sampler - Input 14", T3EffectParameterType::eEffectParameter_SamplerMaterialInput14 },
	{ "Input Material Sampler - Input 15", T3EffectParameterType::eEffectParameter_SamplerMaterialInput15 },
	{ "Sampler - FXAA Console 360 Texture Exp Bias Negative One", T3EffectParameterType::eEffectParameter_SamplerFxaaConsole360TexExpBiasNegOne },
	{ "Sampler - FXAA Console 360 Texture Exp Bias Negative Two", T3EffectParameterType::eEffectParameter_SamplerFxaaConsole360TexExpBiasNegTwo },
	{ "Post Output Sampler - Output 0", T3EffectParameterType::eEffectParameter_SamplerPostOutput0},
	{ "Post Output Sampler - Output 1", T3EffectParameterType::eEffectParameter_SamplerPostOutput1 },
	{ "Post Output Sampler - Output 2", T3EffectParameterType::eEffectParameter_SamplerPostOutput2 },
	{ "Post Output Sampler - Output 3", T3EffectParameterType::eEffectParameter_SamplerPostOutput3 },
	{ "Post Input Sampler - Input 0", T3EffectParameterType::eEffectParameter_SamplerPostInput0 },
	{ "Post Input Sampler - Input 1", T3EffectParameterType::eEffectParameter_SamplerPostInput1 },
	{ "Post Input Sampler - Input 2", T3EffectParameterType::eEffectParameter_SamplerPostInput2 },
	{ "Post Input Sampler - Input 3", T3EffectParameterType::eEffectParameter_SamplerPostInput3 },
	{ "Post Input Sampler - Input 4", T3EffectParameterType::eEffectParameter_SamplerPostInput4 },
	{ "Post Input Sampler - Input 5", T3EffectParameterType::eEffectParameter_SamplerPostInput5 },
	{ "Post Input Sampler - Input 6", T3EffectParameterType::eEffectParameter_SamplerPostInput6 },
	{ "Post Input Sampler - Input 7", T3EffectParameterType::eEffectParameter_SamplerPostInput7 },
	{ "Particle Sampler - Position", T3EffectParameterType::eEffectParameter_SamplerParticlePosition },
	{ "Particle Sampler - Orientation", T3EffectParameterType::eEffectParameter_SamplerParticleOrientation},
	{ "Particle Sampler - Color", T3EffectParameterType::eEffectParameter_SamplerParticleColor },
	{ "Particle Sampler - Rotation 3D", T3EffectParameterType::eEffectParameter_SamplerParticleRotation3D },
	{ "Sampler - Enlighten", T3EffectParameterType::eEffectParameter_SamplerEnlighten },
};

constexpr int NumFeatures = sizeof(sFeatureDesc) / sizeof(_T3EffectFeatureDesc);

constexpr int NumParamTypes = sizeof(sParamTypeDesc) / sizeof(_T3EffectParameterTypeDesc);

const char* T3EffectCacheInternal::GetFeatureName(T3EffectFeature ft)
{
	for (int i = 0; i < NumFeatures; i++) {
		if (ft == sFeatureDesc[i].mFeature)
			return sFeatureDesc[i].mName;
	}
	return nullptr;
}

T3EffectFeature T3EffectCacheInternal::GetFeatureEnum(const char* p)
{
	for (int i = 0; i < NumFeatures; i++) {
		if (!_stricmp(p, sFeatureDesc[i].mName))
			return sFeatureDesc[i].mFeature;
	}
	return T3EffectFeature::eEffectFeature_None;
}

const char* T3EffectCacheInternal::GetParameterTypeName(T3EffectParameterType ft)
{
	for (int i = 0; i < NumParamTypes; i++) {
		if (ft == sParamTypeDesc[i].mParamType)
			return sParamTypeDesc[i].mName;
	}
	return nullptr;
}

T3EffectParameterType T3EffectCacheInternal::GetParameterTypeEnum(const char*p)
{
	for (int i = 0; i < NumParamTypes; i++) {
		if (!_stricmp(p, sParamTypeDesc[i].mName))
			return sParamTypeDesc[i].mParamType;
	}
	return T3EffectParameterType::eEffectParameter_Unknown;
}

_EnumDesc sShaderTypes[] = {
	{"None", T3MaterialShaderType::eMaterialShader_None},
	{"Vertex", T3MaterialShaderType::eMaterialShader_Vertex},
	{"Pixel", T3MaterialShaderType::eMaterialShader_Pixel},
	{"Geometry", T3MaterialShaderType::eMaterialShader_Geometry},
	{"Compute", T3MaterialShaderType::eMaterialShader_Compute},
};

const char* T3EffectCacheInternal::GetShaderTypeName(T3MaterialShaderType a)
{
	for (int i = 0; i < 5; i++) {
		if (a == sShaderTypes[i].mVal)
			return sShaderTypes[i].mName;
	}
	return nullptr;
}

T3MaterialShaderType T3EffectCacheInternal::GetShaderType(const char* n)
{
	for (int i = 0; i < 5; i++) {
		if (!_stricmp(n, sShaderTypes[i].mName))
			return (T3MaterialShaderType)sShaderTypes[i].mVal;
	}
	return T3MaterialShaderType::eMaterialShader_None;
}
