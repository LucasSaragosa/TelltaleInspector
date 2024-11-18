#include "glad.h"

#include "TelltaleInspector.h"
#include "GameEditor/IntrinsicMath.h"
#include <map>
#include "ToolLibrary/T3/T3Effect.h"
#include <fstream>
#include "ToolLibrary/T3/Camera.h"
#include "ToolLibrary/T3/Render.hpp"

#pragma comment (lib, "opengl32.lib")

struct {
	int code;
	LPCSTR message;
} err = { 0, nullptr };

struct {
	LPCSTR title;
	HWND hndl;
	HDC deviceContext;
	HGLRC renderContext;
	int prevX, prevY, width, height, prevWidth, prevHeight, resX, resY;
	bool fullscreen;
} window = { "Testing Render", nullptr, nullptr, nullptr, 0, 0, 0, 0, 0, 0, 640, 480, false };

u64 frameNum = 0;
u32 w = 0, h = 0;
float deltaTime = 0.0f;
Camera cam{};
BitSet<GFXPlatformCapability, eGFXPlatformCap_Count> sOpenGLCaps{};

struct Uniform {
	GLuint id;
};

bool ToGL(T3SurfaceFormat fmt, GLenum& glFormat/*internalformat: gpu fmt: nchannels and type*/, GLenum& inputBufferFormat, GLenum& inputBufferDataType, bool& outCompression, bool sRGB){
	if(fmt == eSurface_ARGB8){
		glFormat = GL_RGBA8;
		inputBufferFormat = GL_BGRA;
		inputBufferDataType = GL_UNSIGNED_INT_8_8_8_8_REV;
	}else if(fmt == eSurface_ARGB16){
		glFormat = GL_RGBA16;//note this format may be weird
		inputBufferFormat = GL_BGRA;
		inputBufferDataType = GL_UNSIGNED_SHORT;
		TTL_Log("Loading ARGB16 format: openGL binding might be wrong! Check this");
	}else if(fmt == eSurface_RGB565){
		glFormat = GL_RGBA8;
		inputBufferFormat = GL_RGB;
		inputBufferDataType = GL_UNSIGNED_SHORT_5_6_5;
	}
	else if (fmt == eSurface_ARGB1555) {
		glFormat = GL_RGBA8;
		inputBufferFormat = GL_BGRA;
		inputBufferDataType = GL_UNSIGNED_SHORT_1_5_5_5_REV;
	}
	else if (fmt == eSurface_ARGB4) {
		glFormat = GL_RGBA8;
		inputBufferFormat = GL_BGRA;
		inputBufferDataType = GL_UNSIGNED_SHORT_4_4_4_4_REV;
	}
	else if (fmt == eSurface_ARGB2101010) {
		glFormat = GL_RGBA8;
		inputBufferFormat = GL_BGRA;
		inputBufferDataType = GL_UNSIGNED_INT_2_10_10_10_REV;
	}
	else if (fmt == eSurface_R16) {
		glFormat = GL_R16;
		inputBufferFormat = GL_RED;
		inputBufferDataType = GL_UNSIGNED_SHORT;//LITTLE ENDIAN ASSUMED
	}
	else if (fmt == eSurface_RG16) {
		glFormat = GL_RG16;
		inputBufferFormat = GL_RG;
		inputBufferDataType = GL_UNSIGNED_SHORT;
	}
	else if (fmt == eSurface_RGBA16) {
		glFormat = GL_RGBA16;
		inputBufferFormat = GL_RGBA;
		inputBufferDataType = GL_UNSIGNED_SHORT;
	}
	else if (fmt == eSurface_RG8) {
		glFormat = GL_RG8;
		inputBufferFormat = GL_RG;
		inputBufferDataType = GL_UNSIGNED_BYTE;
	}
	else if (fmt == eSurface_RGBA8) {
		glFormat = GL_RGBA8;
		inputBufferFormat = GL_RGBA;
		inputBufferDataType = GL_UNSIGNED_BYTE;
	}
	else if (fmt == eSurface_R32) {
		glFormat = GL_R32UI;
		inputBufferFormat = GL_RED;
		inputBufferDataType = GL_UNSIGNED_INT;
	}
	else if (fmt == eSurface_RG32) {
		glFormat = GL_RG32UI;
		inputBufferFormat = GL_RG;
		inputBufferDataType = GL_UNSIGNED_INT;
	}
	else if (fmt == eSurface_RGBA32) {
		glFormat = GL_RGBA32UI;
		inputBufferFormat = GL_RGBA;
		inputBufferDataType = GL_UNSIGNED_INT;
	}
	else if (fmt == eSurface_R8) {
		glFormat = GL_R8;
		inputBufferFormat = GL_RED;
		inputBufferDataType = GL_UNSIGNED_BYTE;
	}
	else if (fmt == eSurface_RGBA8S) {
		glFormat = GL_RGBA8_SNORM;
		inputBufferFormat = GL_RGBA;
		inputBufferDataType = GL_BYTE;
	}//A8 not supported
	else if (fmt == eSurface_L8) {
		glFormat = GL_R8;
		inputBufferFormat = GL_RED;
		inputBufferDataType = GL_UNSIGNED_BYTE;
	}//AL8 not supported
	else if (fmt == eSurface_L16) {
		glFormat = GL_R16;
		inputBufferFormat = GL_RED;
		inputBufferDataType = GL_UNSIGNED_SHORT;
	}
	else if (fmt == eSurface_RG16S) {
		glFormat = GL_RG16_SNORM;
		inputBufferFormat = GL_RG;
		inputBufferDataType = GL_SHORT;
	}
	else if (fmt == eSurface_RGBA16S) {
		glFormat = GL_RGBA16_SNORM;
		inputBufferFormat = GL_RGBA;
		inputBufferDataType = GL_SHORT;
	}
	else if (fmt == eSurface_R16UI) {
		glFormat = GL_R16UI;
		inputBufferFormat = GL_RED;
		inputBufferDataType = GL_UNSIGNED_SHORT;
	}
	else if (fmt == eSurface_RG16UI) {
		glFormat = GL_RG16UI;
		inputBufferFormat = GL_RG;
		inputBufferDataType = GL_UNSIGNED_SHORT;
	}
	else if (fmt == eSurface_R16F) {
		glFormat = GL_R16F;
		inputBufferFormat = GL_RED;
		inputBufferDataType = GL_HALF_FLOAT;
	}
	else if (fmt == eSurface_RG16F) {
		glFormat = GL_RG16F;
		inputBufferFormat = GL_RG;
		inputBufferDataType = GL_HALF_FLOAT;
	}else if (fmt == eSurface_RGBA16F) {
		glFormat = GL_RGBA16F;
		inputBufferFormat = GL_RGBA;
		inputBufferDataType = GL_HALF_FLOAT;
	}
	else if (fmt == eSurface_R32F) {
		glFormat = GL_R32F;
		inputBufferFormat = GL_RED;
		inputBufferDataType = GL_FLOAT;
	}
	else if (fmt == eSurface_RG32F) {
		glFormat = GL_RG32F;
		inputBufferFormat = GL_RG;
		inputBufferDataType = GL_FLOAT;
	}
	else if (fmt == eSurface_RGBA32F) {
		glFormat = GL_RGBA32F;
		inputBufferFormat = GL_RGBA;
		inputBufferDataType = GL_FLOAT;
	}//RGBA 1010102,111110 float not supported
	else if (fmt == eSurface_RGB9E5F) {
		glFormat = GL_RGB9_E5;
		inputBufferFormat = GL_RGB;
		inputBufferDataType = GL_UNSIGNED_INT_5_9_9_9_REV;
	}
	else if (fmt == eSurface_Depth16) {
		glFormat = GL_DEPTH_COMPONENT16;
		inputBufferFormat = GL_DEPTH_COMPONENT;
		inputBufferDataType = GL_UNSIGNED_SHORT;
	}//no pcf depths
	else if (fmt == eSurface_Depth24) {
		glFormat = GL_DEPTH_COMPONENT24;
		inputBufferFormat = GL_DEPTH_COMPONENT;
		inputBufferDataType = GL_UNSIGNED_INT;
	}
	else if (fmt == eSurface_DepthStencil32) {
		glFormat = GL_DEPTH24_STENCIL8;
		inputBufferFormat = GL_DEPTH_STENCIL;
		inputBufferDataType = GL_UNSIGNED_INT_24_8;
	}
	else if (fmt == eSurface_Depth32F) {
		glFormat = GL_DEPTH_COMPONENT32F;
		inputBufferFormat = GL_DEPTH_COMPONENT;
		inputBufferDataType = GL_FLOAT;
	}
	else if (fmt == eSurface_Depth32F_Stencil8) {
		glFormat = GL_DEPTH32F_STENCIL8;
		inputBufferFormat = GL_DEPTH_STENCIL;
		inputBufferDataType = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
	}//depth24FLOAT stencil8 not supported
	else if (fmt == eSurface_DXT1) {
		outCompression = 1;
		glFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;//BC1 (S3TC family - 3 channels)
	}
	else if (fmt == eSurface_DXT3) {
		outCompression = 1;
		glFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;//BC2 (S3TC family)
	}
	else if (fmt == eSurface_DXT5) {
		outCompression = 1;
		glFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;//BC3 (S3TC family)
	}
	else if (fmt == eSurface_DXT5A) {
		outCompression = 1;
		glFormat = GL_COMPRESSED_RED_RGTC1;//BC4 (RGTC family - 2 channels RG)
	}
	else if (fmt == eSurface_BC6) {
		outCompression = 1;
		glFormat = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;//BC6 (BPTC family)
	}
	else if (fmt == eSurface_BC7) {
		outCompression = 1;
		glFormat = sRGB ? GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM : GL_COMPRESSED_RGBA_BPTC_UNORM;//BC7 (BPTC family)
	}else{
		return false;
	}
	return true;
}

class GLAdapters : public ToolLibRenderAdapters {
public:

	~GLAdapters() {
		for (int i = 0; i < 256; i++) {
			for (auto& it : mFreeUniformBuffers[i]) {
				glDeleteBuffers(1, &it.id);
			}
			for (auto& it : mUsedUniformBuffers[i]) {
				glDeleteBuffers(1, &it.id);
			}
			mUsedUniformBuffers[i].clear();
			mFreeUniformBuffers[i].clear();
		}
	}

	GFXPlatformScissorRect mScissor;
	std::shared_ptr<T3GFXVertexState> mpVertexState;
	std::vector<Uniform> mFreeUniformBuffers[256];
	std::vector<Uniform> mUsedUniformBuffers[256];
	GLuint mUserFBO;

	struct RenderTargetStackEntry {
		T3RenderTargetSet mRenderTarget;
		T3RenderViewport mViewport;
	};

	std::vector<RenderTargetStackEntry> mRenderTargetStack;

	T3RenderViewport mViewport;
	T3RenderTargetSet mCurrentRenderTarget;
	GLuint mCurrentFBO;

	/**
	 * Upload the static vertices in the given vertex state to the GPU. Most of this time this can just call UpdateGFXBufferFromCPUBuffer with the vert buffer.
	 */
	void EndStaticVertices(T3GFXVertexState& state) override {
		UpdateGFXBufferFromCPUBuffer(state.mpVertexBuffer[0].get());
	}

	/**
	 * Upload the static indices in the given vertex state to the GPU. Most of this time this can just call UpdateGFXBufferFromCPUBuffer with the index buffer.
	 */
	void EndStaticIndices(T3GFXVertexState& state) override {
		UpdateGFXBufferFromCPUBuffer(state.mpIndexBuffer[0].get());
	}

	void UpdateGFXBufferFromCPUBuffer(T3GFXBuffer* pBuffer) override {
		GLuint vbo = pBuffer->GetRenderPlatformHandle_UInt();
		//TEMP
		GLint binding = pBuffer->mBufferFormat == eGFXPlatformFormat_U16 ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
		glBindBuffer(binding, vbo);
		glBufferData(binding, pBuffer->mCount * pBuffer->mStride, pBuffer->mpCPUBuffer, GL_STATIC_DRAW);//static draw for now
	}

	void OnResourceCreation(T3GFXResource* pBuffer) override
	{
		if(pBuffer->mResourceType == eGFXPlatformResource_Buffer){
			GLuint vbo{};
			glGenBuffers(1, &vbo);
			pBuffer->SetRenderPlatformHandle_UInt(vbo);
		}else if(pBuffer->mResourceType == eGFXPlatformResource_VertexState){
			GLuint vao{};
			glGenVertexArrays(1, &vao);
			pBuffer->SetRenderPlatformHandle_UInt(vao);
		}
	}

	void OnResourceDestroy(T3GFXResource* pBuffer) override {
		if (pBuffer->mResourceType == eGFXPlatformResource_Buffer) {
			GLuint vbo = pBuffer->GetRenderPlatformHandle_UInt();
			glDeleteBuffers(1, &vbo);
		}
		else if (pBuffer->mResourceType == eGFXPlatformResource_VertexState) {
			GLuint vao = pBuffer->GetRenderPlatformHandle_UInt();
			glDeleteVertexArrays(1, &vao);
		}
	}

	bool TestCap(GFXPlatformCapability cap) override {
		return sOpenGLCaps[cap];
	}

	virtual bool BindBuffer(T3EffectParameterBoundState& state, RenderFrameStats& stats, T3EffectParameterType paramType, T3EffectParameterBuffer* pBuffer, u32 bufferOffset) override {
		GFXPlatformProgram::ParameterHeader programParam = {};
		GFXPlatformShader::ParameterHeader shaderParam = {};

		if (state.mpCurrentProgram)
			programParam = state.mpCurrentProgram->mParameters[paramType];
		if(state.mpCurrentComputeShader) 
			shaderParam = state.mpCurrentComputeShader->mParameters[paramType];

		if (programParam.mVertexLocation != (char)-1) {
			glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)programParam.mVertexLocation, pBuffer->mpGFXBuffer->GetRenderPlatformHandle_UInt());
			stats.mBufferBinds++;
		}

		if (programParam.mPixelLocation != (char)-1) {
			glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)programParam.mPixelLocation, pBuffer->mpGFXBuffer->GetRenderPlatformHandle_UInt());
			stats.mBufferBinds++;
		}

		if (programParam.mGeometryLocation != (char)-1) {
			glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)programParam.mGeometryLocation, pBuffer->mpGFXBuffer->GetRenderPlatformHandle_UInt());
			stats.mBufferBinds++;
		}

		if (shaderParam.mLocation != (char)-1) {
			glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)shaderParam.mLocation, pBuffer->mpGFXBuffer->GetRenderPlatformHandle_UInt());
			stats.mBufferBinds++;
		}

		return true;
	}

	virtual bool BindBuffer_0(T3EffectParameterBoundState& state, RenderFrameStats& stats, T3EffectParameterType paramType, const void* pData, u32 scalarSize) override {
		if ((state.mBufferBinding[paramType].mpBufferData != pData || state.mBufferBinding[paramType].mBufferDataSize != scalarSize) && (state.mpCurrentProgram||state.mpCurrentComputeShader)) {
			u32 count = (scalarSize+3) >> 2;
			state.mBufferBinding[paramType].mpBufferData = pData;
			state.mBufferBinding[paramType].mBufferDataSize = scalarSize;
			
			GFXPlatformProgram::ParameterHeader programParam = {};
			GFXPlatformShader::ParameterHeader shaderParam = {};

			if (state.mpCurrentProgram)
				programParam = state.mpCurrentProgram->mParameters[paramType];
			if (state.mpCurrentComputeShader)
				shaderParam = state.mpCurrentComputeShader->mParameters[paramType];

			if(programParam.mVertexLocation != (char)-1 || programParam.mPixelLocation != programParam.mVertexLocation 
				|| programParam.mGeometryLocation != programParam.mVertexLocation || shaderParam.mLocation != programParam.mVertexLocation){

				u16 uniformListIndex = max(shaderParam.mScalarSize, max(programParam.mGeometryScalarSize, max(programParam.mPixelScalarSize, programParam.mVertexScalarSize)));
				uniformListIndex = (((uniformListIndex + (u16)63) & (u16)0xFFC0) - (u16)1u) >> 10;//between 0 and 255

				Uniform uniform{};

				if(mFreeUniformBuffers[uniformListIndex].size()){
					uniform = mFreeUniformBuffers[uniformListIndex].back();
					mFreeUniformBuffers[uniformListIndex].pop_back();
				}else{
					glGenBuffers(1, &uniform.id);
				}

				mUsedUniformBuffers->push_back(uniform);

				//set the buffer data
				glNamedBufferData(uniform.id, 16 * count, pData, GL_STATIC_DRAW);

				if (programParam.mVertexLocation != (char)-1) {
					glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)programParam.mVertexLocation, uniform.id);
					stats.mBufferBinds++;
				}

				if (programParam.mPixelLocation != (char)-1) {
					glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)programParam.mPixelLocation, uniform.id);
					stats.mBufferBinds++;
				}

				if (programParam.mGeometryLocation != (char)-1) {
					glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)programParam.mGeometryLocation, uniform.id);
					stats.mBufferBinds++;
				}

				if (shaderParam.mLocation != (char)-1) {
					glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)shaderParam.mLocation, uniform.id);
					stats.mBufferBinds++;
				}

			}

			return true;
		}
		else return false;
	}

	void _BindGeneric(T3EffectParameterType type, GLuint buffer, GLuint shaderLocation, T3BufferView& view) {
		//TODO
	}

	virtual bool BindGenericBuffer(T3EffectParameterBoundState& state, RenderFrameStats& stats, T3EffectParameterType paramType, T3GFXBuffer* pBuffer, T3BufferView& baseView) override {
		GFXPlatformProgram::ParameterHeader programParam = {};
		GFXPlatformShader::ParameterHeader shaderParam = {};

		if (state.mpCurrentProgram)
			programParam = state.mpCurrentProgram->mParameters[paramType];
		if (state.mpCurrentComputeShader)
			shaderParam = state.mpCurrentComputeShader->mParameters[paramType];

		
		// TODO !

		return true;
	}

	void _BindTex(T3Texture* tex,T3SamplerStateBlock samplerState,T3TextureView& view, u32 mipBias,T3EffectParameterType paramType){
		if (mipBias && paramType >= eEffectParameter_FirstMaterialInputSampler && paramType <= eEffectParameter_LastMaterialInputSampler)
			samplerState.DecrementMipBias(mipBias);

		float kBorderColors[2][4] = { {0.f,0.f,0.f,0.f},{1.f,1.f,1.f,1.f} };

		GLenum target;
		if (tex->mTextureLayout == eTextureLayout_2D)
			target = GL_TEXTURE_2D;
		else if (tex->mTextureLayout == eTextureLayout_2DArray)
			target = GL_TEXTURE_2D_ARRAY;
		else if (tex->mTextureLayout == eTextureLayout_3D)
			target = GL_TEXTURE_3D;
		else if (tex->mTextureLayout == eTextureLayout_Cube)
			target = GL_TEXTURE_CUBE_MAP;
		else if (tex->mTextureLayout == eTextureLayout_CubeArray)
			target = GL_TEXTURE_CUBE_MAP_ARRAY;
		else
			target = GL_TEXTURE_2D;//!?

		glBindTexture(target, tex->GetRenderPlatformHandle_UInt());

		//set sampler state
		glTexParameteri(target, GL_TEXTURE_WRAP_S, samplerState.InternalGetSamplerState(eSamplerState_WrapU_Value) == TEXTURE_WRAP_WRAP ? GL_REPEAT :
			samplerState.InternalGetSamplerState(eSamplerState_WrapU_Value) == TEXTURE_WRAP_BORDER ? GL_CLAMP_TO_BORDER : GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, samplerState.InternalGetSamplerState(eSamplerState_WrapV_Value) == TEXTURE_WRAP_WRAP ? GL_REPEAT :
			samplerState.InternalGetSamplerState(eSamplerState_WrapV_Value) == TEXTURE_WRAP_BORDER ? GL_CLAMP_TO_BORDER : GL_CLAMP_TO_EDGE);

		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, kBorderColors[samplerState.InternalGetSamplerState(eSamplerState_BorderColor_Value) == TEXTURE_BORDER_COLOR_BLACK ? 0 : 1]);

		//these can stay as defaults for now. also can be anisotropic, wont bother with that
		if(samplerState.InternalGetSamplerState(eSamplerState_Filtered_Value)){
			//filter D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT
			//cmp less
			//glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			//glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}else{
			//filter D3D11_FILTER_MIN_MAG_MIP_POINT
			//cmp never
			//glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			//glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glTexParameterf(target, GL_TEXTURE_LOD_BIAS, (float)samplerState.InternalGetSamplerState(eSamplerState_MipBias_Value) * -0.25f);

		glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}

	virtual bool BindTexture(T3EffectParameterBoundState& state, RenderFrameStats& stats, T3EffectParameterType paramType, T3Texture* pTexture, T3TextureView& baseView) override {
		GFXPlatformProgram::ParameterHeader programParam = {};
		GFXPlatformShader::ParameterHeader shaderParam = {};

		if (state.mpCurrentProgram)
			programParam = state.mpCurrentProgram->mParameters[paramType];
		if (state.mpCurrentComputeShader)
			shaderParam = state.mpCurrentComputeShader->mParameters[paramType];

		char sLocations[4] = {};
		sLocations[0] = shaderParam.mSamplerLocation;
		sLocations[1] = programParam.mGeometrySamplerLocation;
		sLocations[2] = programParam.mPixelSamplerLocation;
		sLocations[3] = programParam.mVertexSamplerLocation;

		for (int i = 0; i < 4; i++) {

			if (sLocations[i] != (char)-1) {
				glActiveTexture(GL_TEXTURE0 + (GLenum)sLocations[i]);
				_BindTex(pTexture, pTexture == 0 ? kDefault.Merge(state.mTextureBinding[paramType - eEffectParameter_FirstSampler].mSamplerState, state.mTextureBinding[paramType - eEffectParameter_FirstSampler].mSamplerStateMask)
					: pTexture->mSamplerState.Merge(state.mTextureBinding[paramType - eEffectParameter_FirstSampler].mSamplerState, state.mTextureBinding[paramType - eEffectParameter_FirstSampler].mSamplerStateMask), baseView
				, pTexture == 0 ? 0 : state.mMaterialSamplerMipBias, paramType);
				state.mTextureBinding[paramType - eEffectParameter_FirstSampler].mpTexture = pTexture;
				state.mTextureBinding[paramType - eEffectParameter_FirstSampler].mView = baseView;
				stats.mTextureBinds++;
			}

		}

		return true;
	}

	virtual void UpdateTextureData(T3Texture* pTexture, const T3TextureView& view, void* pTexData, u32 texDataSize, u32 mwidth, u32 mheight) override {
		GLenum fmt = 0, ty = 0, cmp = 0;
		bool bCompressed = false;
		if (!ToGL(pTexture->mSurfaceFormat, cmp, fmt, ty, bCompressed, pTexture->mSurfaceGamma == eSurfaceGamma_sRGB)) {
			TelltaleToolLib_RaiseError("Cannot load texture from disk, it is not a PC standard texture - (this means it likely is from another platform)!", CRITICAL_ERROR);
			__debugbreak();
			return;
		}
		if (pTexture->mTextureLayout == eTextureLayout_2D)
			glTexImage2D(GL_TEXTURE_2D, (GLint)view.mMipIndex, cmp, mwidth, mheight, 0, fmt, ty, pTexData);

		// TODO    

	}

	virtual void GetResolution(u32& outWidth, u32& outHeight) override {
		outWidth = w;
		outHeight = h;
	}

	virtual float GetDeltaTime() override {
		return deltaTime;
	}

	virtual u64 GetFrameIndex() override {
		return frameNum;
	}

	virtual PlatformType GetPlatform() override {
		return ePlatform_PC;
	}

	virtual void PushRenderTarget(const T3RenderTargetSet& set, const T3RenderClear& renderClear) override {
		auto& entry = mRenderTargetStack.emplace_back();
		entry.mRenderTarget = set;
		entry.mViewport = mViewport;
		SwitchRenderTarget(entry.mRenderTarget, renderClear, true, true, true);
	}

	virtual void PopRenderTarget() override {
		if(mRenderTargetStack.size()){
			RenderTargetStackEntry entry = mRenderTargetStack.back();
			mRenderTargetStack.pop_back();
			T3RenderClear clear{};
			clear.mbColor = clear.mbDepth = clear.mbStencil = true;
			SwitchRenderTarget(entry.mRenderTarget, clear, true, true, false);
			SetCurrentViewport(entry.mViewport);
		}
	}

	virtual void SetCurrentViewport(const T3RenderViewport& viewport) override {
		mViewport = viewport;
		glViewport(mViewport.mOriginX, mViewport.mOriginY, mViewport.mWidth, mViewport.mHeight);
	}

	virtual void SwitchRenderTarget(const T3RenderTargetSet& set, const T3RenderClear& renderClear, bool bSetColorTarget, bool bSetDepthTarget, bool bClear) override {
		if (!bSetDepthTarget && !bSetColorTarget)
			return;
		if(mCurrentFBO != mUserFBO){
			mCurrentFBO = mUserFBO;
			glBindFramebuffer(GL_FRAMEBUFFER, mUserFBO);
		}
		if(bSetColorTarget){
			for(int i = 0; i < 8; i++){
				mCurrentRenderTarget.mRenderTarget[i] = set.mRenderTarget[i];
				if(set.mRenderTarget[i].mpTexture){
					T3Texture* pRT = (mCurrentRenderTarget.mRenderTarget[i].mpTexture = set.mRenderTarget[i].mpTexture).get();				
					glNamedFramebufferTextureLayer(mCurrentFBO, GL_COLOR_ATTACHMENT0 + i, pRT->GetRenderPlatformHandle_HI_UInt(),
						(GLuint)set.mRenderTarget[i].mMipLevel, (GLuint)set.mRenderTarget[i].mSliceIndex);
				}
			}
		}
		if(bSetDepthTarget){
			if (set.mDepthTarget.mpTexture) {
				T3Texture* pRT = (mCurrentRenderTarget.mDepthTarget.mpTexture = set.mDepthTarget.mpTexture).get();
				glNamedFramebufferTextureLayer(mCurrentFBO, GL_DEPTH_STENCIL_ATTACHMENT, pRT->GetRenderPlatformHandle_HI_UInt(),
					(GLuint)set.mDepthTarget.mMipLevel, (GLuint)set.mDepthTarget.mSliceIndex);
			}
		}
		mCurrentRenderTarget.mWidth = set.mWidth;
		mCurrentRenderTarget.mHeight = set.mHeight;
	}

	virtual void Clear(const T3RenderClear& clear) override {
		glClearColor(clear.mClearColor.r, clear.mClearColor.g, clear.mClearColor.b, clear.mClearColor.a);
		glClearDepthf(clear.mClearDepth);
		glClearStencil(clear.mClearStencil);
	}

	virtual void SwitchDefaultRenderTarget(const T3RenderClear& clear) override {
		if(mCurrentFBO != 0){
			mCurrentFBO = 0;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void OnSizeChanged(u32 x, u32 y){
		w = x;
		h = y;
	}

	void Init(){
		glGenFramebuffers(1, &mUserFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Shutdown(){
		glDeleteFramebuffers(1, &mUserFBO);
	}

};

static GLAdapters* mpAdapters;

static void registerClass(HINSTANCE instance);
static void createContext();
static void createWindow(HINSTANCE instance);
static void centerWindow();

void Draw() {
	//Delta time since last frame was drawn
	//double dt = frameNum == 0 ? 0. : (double)cl.getElapsedTime().asMicroseconds() * 1e-6;
	//cl.restart();
	//deltaTime = (float)dt;


	frameNum++;
}

void Log(const char* fmt, va_list va){
	vprintf_s(fmt, va);
}

void TestStuff() {
	//STATE
	HINSTANCE hInstance = GetModuleHandle(NULL);

	registerClass(hInstance);
	createWindow(hInstance);
	createContext();

	wglMakeCurrent(window.deviceContext, window.renderContext);
	ShowWindow(window.hndl, 0);
	centerWindow();
	UpdateWindow(window.hndl);

	gladLoadGL();
	mpAdapters = new GLAdapters;
	TelltaleToolLib_Initialize("MC2");
	TelltaleToolLib_InitializeT3(mpAdapters);
	TelltaleToolLib_SetLoggerHook(&Log);

	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	mpAdapters->Shutdown();
	TelltaleToolLib_Free();
	delete mpAdapters;
}

static void
centerWindow()
{
	MONITORINFO mi = { sizeof(mi) };

	GetMonitorInfo(MonitorFromWindow(window.hndl, MONITOR_DEFAULTTONEAREST), &mi);
	int x = (mi.rcMonitor.right - mi.rcMonitor.left - window.width) / 2;
	int y = (mi.rcMonitor.bottom - mi.rcMonitor.top - window.height) / 2;

	SetWindowPos(window.hndl, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
}

static void
setFullscreen(bool fullscreen)
{
	DWORD style = GetWindowLong(window.hndl, GWL_STYLE);
	if (fullscreen)
	{
		RECT rect;
		MONITORINFO mi = { sizeof(mi) };
		GetWindowRect(window.hndl, &rect);
		window.prevX = rect.left;
		window.prevY = rect.top;
		window.prevWidth = rect.right - rect.left;
		window.prevHeight = rect.bottom - rect.top;

		GetMonitorInfo(MonitorFromWindow(window.hndl, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowLong(window.hndl, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(window.hndl, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
	else
	{
		MONITORINFO mi = { sizeof(mi) };
		UINT flags = SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW;
		GetMonitorInfo(MonitorFromWindow(window.hndl, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowLong(window.hndl, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
		SetWindowPos(window.hndl, HWND_NOTOPMOST, window.prevX, window.prevY, window.prevWidth, window.prevHeight, flags);
	}
}


static LRESULT CALLBACK
wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (message)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
		Draw();
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		mpAdapters->OnSizeChanged((u32)LOWORD(lParam), (u32)HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		/* ESC */
		if (wParam == 27)
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		/* F11 */
		else if (wParam == 122)
			setFullscreen(window.fullscreen = !window.fullscreen);
		break;
	case WM_CLOSE:
		wglMakeCurrent(window.deviceContext, NULL);
		wglDeleteContext(window.renderContext);
		ReleaseDC(hWnd, window.deviceContext);
		DestroyWindow(hWnd);
		/* stop event queue thread */
		PostQuitMessage(0);
		break;
	default:
		result = DefWindowProc(hWnd, message, wParam, lParam);
	}
	return result;
}

static void
registerClass(HINSTANCE instance)
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"_T3Renderer";
	wcex.hIconSm = NULL;

	if (!RegisterClassExW(&wcex))
	{}
}

static void
createContext()
{
	window.deviceContext = GetDC(window.hndl);
	if (window.deviceContext)
	{
		int pixelFormat;
		PIXELFORMATDESCRIPTOR pixelFormatDesc;

		/* initialize bits to 0 */
		memset(&pixelFormatDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pixelFormatDesc.nVersion = 1;
		pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
		pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
		pixelFormatDesc.cColorBits = 32;
		pixelFormatDesc.cAlphaBits = 8;
		pixelFormatDesc.cDepthBits = 24;

		pixelFormat = ChoosePixelFormat(window.deviceContext, &pixelFormatDesc);
		if (pixelFormat)
		{
			if (SetPixelFormat(window.deviceContext, pixelFormat, &pixelFormatDesc))
			{
				window.renderContext = wglCreateContext(window.deviceContext);
				if (!window.renderContext)
				{
					err.message = "wglCreateContext failed: Can not create render context.";
				}
			}
			else
			{
				err.message = "SetPixelFormat failed: Can not create render context.";
			}
		}
		else
		{
			err.message = "ChoosePixelFormat failed: Can not create render context.";
		}
	}
	else
	{
		err.message = "GetDC failed: Can not create device context.";
	}
}

static void
createWindow(HINSTANCE instance)
{
	DWORD  style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	RECT rect = { 0, 0, window.resX, window.resY };

	if (AdjustWindowRect(&rect, style, false))
	{
		/* compute window size including border */
		window.width = rect.right - rect.left;
		window.height = rect.bottom - rect.top;

		window.hndl = CreateWindowA("_T3Renderer", window.title, style, 0, 0, window.width, window.height, nullptr, nullptr, instance, nullptr);
		if (!window.hndl)
		{
			err.message = "CreateWindowW failed: Can not create window.";
		}
	}
	else
	{
		err.message = "AdjustWindowRect failed: Can not create window.";
	}
}