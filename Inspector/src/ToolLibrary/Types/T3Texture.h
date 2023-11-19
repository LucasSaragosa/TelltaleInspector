// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _D3DTX
#define _D3DTX

#include "ToolProps.h"
#include "../TelltaleToolLibrary.h"
#include "List.h"
#include "../Meta.hpp"
#include "ObjectSpaceIntrinsics.h"
#include "DCArray.h"
#include "../DataStream/DataStream.h"
#include "SArray.h"

enum T3SamplerStateValue : unsigned int {
	eSamplerState_WrapU_Value = 0x0,
	eSamplerState_WrapV_Value = 0x1,
	eSamplerState_Filtered_Value = 0x2,
	eSamplerState_BorderColor_Value = 0x3,
	eSamplerState_GammaCorrect_Value = 0x4,
	eSamplerState_MipBias_Value = 0x5,
	eSamplerState_Count = 0x6,
};

struct T3SamplerStateBlock;
extern T3SamplerStateBlock kDefault;

struct T3RenderStateBlock {
	SArray<u32, 3> mData;
};

struct T3SamplerStateBlock {
	unsigned int mData;

	struct SamplerStateEntry {
		unsigned int mShift;
		unsigned int mMask;
	};

	static bool sInitialized;
	static SamplerStateEntry smEntries[6];

	static void Initialize() {
		if (sInitialized)return;
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_WrapU_Value].mShift = 0x0;//0x14102C988
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_WrapU_Value].mMask = 0xF;//0x14102C98C, 4 bits, so a nibble of data
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_WrapV_Value].mShift = 0x4;//0x14102C990
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_WrapV_Value].mMask = 0xF0;//0x14102C994, 4 bits, so a nibble of data
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_Filtered_Value].mShift = 0x8;//0x14102C998
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_Filtered_Value].mMask = 0x100;//0x14102C99C //1 bit, so a bool
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_BorderColor_Value].mShift = 0x9;//0x14102C9A0
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_BorderColor_Value].mMask = 0x1E00;//0x14102C9A4, 4 bits, so a nibble of data
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_GammaCorrect_Value].mShift = 0xD;//0x14102C9A8
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_GammaCorrect_Value].mMask = 0x2000;//0x14102C9AC, 1 bit so a bool
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_MipBias_Value].mShift = 0xE;//0x14102C9B0
		smEntries[(unsigned int)T3SamplerStateValue::eSamplerState_MipBias_Value].mMask = 0x3FC000;//0x14102C9B4, 8 bits, so a byte of data
		kDefault.mData = (((kDefault.mData & 0xFFFFFFF0 | 1) 
			& 0xFFFFFF0F | 0x10) & 0xFFFFFEFF | 0x100) & 0xFFC001FF;//273, filtered yes, wrap u and v are 1
		sInitialized = true;
	}

	T3SamplerStateBlock() : mData(0) { Initialize(); }

	void SetStateMask(T3SamplerStateValue state) {
		mData |= smEntries[state].mMask;
	}

	T3SamplerStateBlock& Merge(T3SamplerStateBlock& result, 
		T3SamplerStateBlock const& rhs, 
		T3SamplerStateBlock const& mask) {
		result.mData = mask.mData & rhs.mData | mData & ~mask.mData;
		return result;
	}

	u64 InternalGetSamplerState(T3SamplerStateValue state) {
		return (this->mData & smEntries[state].mMask) >> (smEntries[state].mShift);
	}

	void InternalSetSamplerState(T3SamplerStateValue state, unsigned int value) {
		mData &= ~smEntries[state].mMask;
		mData |= value << smEntries[state].mShift;
	}

	u64 DecrementMipBias(unsigned int steps) {
		float v2 = 8.0;
		u64 result;
		unsigned int v3 = steps + (smEntries[5].mMask & mData) >> smEntries[5].mShift;
		if (v3 < v2)
			v2 = (float)v3;
		result = (unsigned int)floorf(v2);
		unsigned int v5 = mData & ~smEntries[5].mMask;
		mData = v5 | (result << smEntries[5].mShift);
		return result;
	}

};

enum PlatformType {
	ePlatform_None = 0x0,
	ePlatform_All = 0x1,
	ePlatform_PC = 0x2,
	ePlatform_Wii = 0x3,
	ePlatform_Xbox = 0x4,
	ePlatform_PS3 = 0x5, 
	ePlatform_Mac = 0x6,
	ePlatform_iPhone = 0x7,
	ePlatform_Android = 0x8,
	ePlatform_Vita = 0x9,
	ePlatform_Linux = 0xA,
	ePlatform_PS4 = 0xB,
	ePlatform_XBOne = 0xC,
	ePlatform_WiiU = 0xD,
	ePlatform_Win10 = 0xE,
	ePlatform_NX = 0xF,
	ePlatform_Count = 0x10, 

};

struct EnumPlatformType : EnumBase {
	PlatformType mVal;
};

enum T3SurfaceFormat {
	eSurface_Unknown = 0xFFFFFFFF, 
	eSurface_ARGB8 = 0x0,
	eSurface_ARGB16 = 0x1,
	eSurface_RGB565 = 0x2,
	eSurface_ARGB1555 = 0x3, 
	eSurface_ARGB4 = 0x4, 
	eSurface_ARGB2101010 = 0x5,
	eSurface_R16 = 0x6,
	eSurface_RG16 = 0x7,
	eSurface_RGBA16 = 0x8,
	eSurface_RG8 = 0x9, 
	eSurface_RGBA8 = 0xA,
	eSurface_R32 = 0xB,
	eSurface_RG32 = 0xC,
	eSurface_RGBA32 = 0xD,
	eSurface_RGBA8S = 0xF,
	eSurface_A8 = 0x10, 
	eSurface_L8 = 0x11,
	eSurface_AL8 = 0x12,
	eSurface_L16 = 0x13,
	eSurface_RG16S = 0x14,
	eSurface_RGBA16S = 0x15,
	eSurface_R16UI = 0x16,
	eSurface_RG16UI = 0x17,
	eSurface_R16F = 0x20, 
	eSurface_RG16F = 0x21,
	eSurface_RGBA16F = 0x22,
	eSurface_R32F = 0x23,
	eSurface_RG32F = 0x24,
	eSurface_RGBA32F = 0x25,
	eSurface_RGBA1010102F = 0x26,
	eSurface_RGB111110F = 0x27,
	eSurface_RGB9E5F = 0x28,
	eSurface_DepthPCF16 = 0x30,
	eSurface_DepthPCF24 = 0x31,
	eSurface_Depth16 = 0x32,
	eSurface_Depth24 = 0x33,
	eSurface_DepthStencil32 = 0x34,
	eSurface_Depth32F = 0x35,
	eSurface_Depth32F_Stencil8 = 0x36,
	eSurface_Depth24F_Stencil8 = 0x37,
	//eSurface_BC1 = 0x40,
	//eSurface_BC2 = 0x41
	eSurface_DXT1 =64,
	eSurface_DXT3 = 65,
	eSurface_DXT5= 66,
	eSurface_DXT5A = 67,
	eSurface_DXTN = 68,
	eSurface_CTX1 = 69,
	eSurface_BC6 = 70,
	eSurface_BC7 = 71,
	eSurface_PVRTC2 = 80,
	eSurface_PVRTC4 = 81,
	eSurface_PVRTC2a = 82,
	eSurface_PVRTC4a = 83,
	eSurface_ATC_RGB = 96,
	eSurface_ATC_RGB1A = 97,
	eSurface_ATC_RGBA = 98,
	eSurface_ETC1_RGB = 112,
	eSurface_ETC2_RGB = 113,
	eSurface_ETC2_RGB1A = 114,
	eSurface_ETC2_RGBA = 115,
	eSurface_ETC2_R = 116,
	eSurface_ETC2_RG =117,
	eSurface_ATSC_RGBA_4x4 = 128,
	eSurface_R8 = 14,
};

struct T3ToonGradientRegion {
	Color mColor;
	float mSize;
	float mGradientSize;
};

enum T3TextureLayout {
	eTextureLayout_2D = 0,//slices=1
	eTextureLayout_Cube = 1,//slices=1
	eTextureLayout_3D = 2,//slices=depth
	eTextureLayout_2DArray = 3,//slices=arraysize
	eTextureLayout_CubeArray = 4,//slices=arraysize
	eTextureLayout_Count = 5,
	eTextureLayout_Unknown = 0xFFFFFFFF
};

enum T3SurfaceGamma {
	eSurfaceGamma_Linear = 0,
	eSurfaceGamma_sRGB = 1,
	sSurfaceGamma_Unknown = 0xFFFFFFFF
};

enum T3SurfaceMultisample {
	eSurfaceMultisample_None = 0,
	eSurfaceMultisample_2x = 1,
	eSurfaceMultisample_4x = 2,
	eSurfaceMultisample_8x = 3,
	eSurfaceMultisample_16x = 4
};

enum T3ResourceUsage {
	eResourceUsage_Static = 0,
	eResourceUsage_Dynamic = 1,
	eResourceUsage_System = 2,
	eResourceUsage_RenderTarget = 3,
	eResourceUsage_ShaderWrite = 4
};


enum RenderSwizzleType : char {
	eRenderSwizzle_X = 0x0, 
	eRenderSwizzle_Y = 0x1,
	eRenderSwizzle_Z = 0x2,
	eRenderSwizzle_W = 0x3,
	eRenderSwizzle_Zero = 0x4,
	eRenderSwizzle_One = 0x5,
	eRenderSwizzle_R = 0x0,
	eRenderSwizzle_G = 0x1,
	eRenderSwizzle_B = 0x2,
	eRenderSwizzle_A = 0x3,
};

struct RenderSwizzleParams {
	RenderSwizzleType mSwizzle[4];

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		static int _ID = TelltaleToolLib_GetGameKeyIndex("BATMAN");
		RenderSwizzleParams* swiz = static_cast<RenderSwizzleParams*>(pObj);
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		u32 blockSize = 8;
		if (sSetKeyIndex >= _ID) {
			meta->serialize_uint32(&blockSize);
			if (blockSize != 8) {
				TelltaleToolLib_RaiseError("Swizzle block not correct size", ErrorSeverity::ERR);
				return eMetaOp_Fail;
			}
		}
		meta->serialize_int8((char*)&swiz->mSwizzle[0]);
		meta->serialize_int8((char*)&swiz->mSwizzle[1]);
		meta->serialize_int8((char*)&swiz->mSwizzle[2]);
		meta->serialize_int8((char*)&swiz->mSwizzle[3]);
		return eMetaOp_Succeed;
	}

};

//.D3DTX FILES
struct T3Texture {

	enum TextureType {
		eTxUnknown = 0x0,
		eTxLightmap_V0 = 0x1,
		eTxBumpmap = 0x2,
		eTxNormalMap = 0x3,
		eTxUNUSED1 = 0x4,
		eTxUNUSED0 = 0x5,
		eTxSubsurfaceScatteringMap_V0 = 0x6,
		eTxSubsurfaceScatteringMap = 0x7,
		eTxDetailMap = 0x8,
		eTxStaticShadowMap = 0x9,
		eTxLightmapHDR = 0xA,
		eTxSDFDetailMap = 0xB,
		eTxEnvMap = 0xC,
		eTxSpecularColor = 0xD,
		eTxToonLookup = 0xE,
		eTxStandard = 0xF,
		eTxOutlineDiscontinuity = 0x10,
		eTxLightmapHDRScaled = 0x11, 
		eTxEmissiveMap = 0x12, 
		eTxParticleProperties = 0x13,
		eTxBrushNormalMap = 0x14,
		eTxUNUSED2 = 0x15,
		eTxNormalGlossMap = 0x16,
		eTxLookup = 0x17,
		eTxAmbientOcclusion = 0x18,
		eTxPrefilteredEnvCubeMapHDR = 0x19,
		eTxBrushLookupMap = 0x1A,
		eTxVector2Map = 0x1B, 
		eTxNormalDxDyMap = 0x1C,
		eTxPackedSDFDetailMap = 0x1D,
		eTxSingleChannelSDFDetailMap = 0x1E,
		eTxLightmapDirection = 0x1F,
		eTxLightmapStaticShadows = 0x20,
		eTxLightStaticShadowMapAtlas = 0x21,
		eTxLightStaticShadowMap = 0x22,
		eTxPrefilteredEnvCubeMapHDRScaled = 0x23, 
		eTxLightStaticShadowVolume = 0x24,
		eTxLightmapAtlas = 0x25,
		eTxNormalXYMap = 0x26,
		eTxLightmapFlatAtlas = 0x27,
		eTxLookupXY = 0x28,
		eTxObjectNormalMap = 0x29
	};

	enum ColorMode {
		eTxColorFull = 0,
		eTxColorBlack = 1,
		eTxColorGrayscale = 2,
		eTxColorGrayscaleAlpha = 3,
		eTxColorUnknown = 0xFFFFFFFF
	};

	struct AuxilaryData {
		Symbol mType;
		BinaryBuffer mData;
	};

	struct RegionStreamHeader {
		int mFaceIndex;
		int mMipIndex;
		int mMipCount;
		int mDataSize;
		int mPitch;
		int mSlicePitch;
		DataStream* mpAsyncStream;//moved here, so its easier to set data

		RegionStreamHeader() : mpAsyncStream(NULL) {}

		~RegionStreamHeader() {
			if (mpAsyncStream)
				delete mpAsyncStream;
		}

	};

	enum AlphaMode {
		eTxNoAlpha = 0,
		eTxAlphaTest = 1,
		eTxAlphaBlend = 2,
		eTxAlphaUnkown = 0xFFFFFFFF//telltale spelt it wrong not me
	};

	long mVersion;
	T3SamplerStateBlock mSamplerState;
	EnumPlatformType mPlatform;
	String mName;
	String mImportName;
	float mImportScale;
	float mImportSpecularPower;//old games
	ToolProps mToolProps;
	long mNumMipLevels;
	long mWidth;
	long mHeight;
	long mDepth;
	long mArraySize;
	T3SurfaceFormat mSurfaceFormat;
	T3TextureLayout mTextureLayout;
	T3SurfaceGamma mSurfaceGamma;
	T3SurfaceMultisample mSurfaceMultisample;
	T3ResourceUsage mResourceUsage;
	TextureType mType;
	int mNormalMapFormat;//MCSM and below
	RenderSwizzleParams mSwizzle;//MICHONNE and above
	float mSpecularGlossExponent;
	float mHDRLightmapScale;
	float mToonGradientCutoff;
	AlphaMode mAlphaMode;
	ColorMode mColorMode;
	Vector2 mUVOffset;
	Vector2 mUVScale;
	//next 4 are not serialized (has serialize disable set)
	int mNumMipLevelsAllocated;
	int mNumSurfacesRequested;
	int mNumSurfacesRequired;
	int mNumSurfacesLoaded;
	DCArray<Symbol> mArrayFrameNames;
	DCArray<T3ToonGradientRegion> mToonRegions;

	List<AuxilaryData*> mAuxilaryData;//should be linkedlist which uses pointers

	T3Texture() {
		mImportScale = 1.0f;
		mUVScale.x = 1.0f;
		mUVScale.x = 1.0f;
		mArraySize = 1;
		mDepth = 1;
		mTextureLayout = eTextureLayout_2D;
		mSurfaceFormat = T3SurfaceFormat::eSurface_Unknown;
		mSurfaceMultisample = T3SurfaceMultisample::eSurfaceMultisample_None;
		mSurfaceGamma = T3SurfaceGamma::sSurfaceGamma_Unknown;
		mUVOffset.x = 0;
		mUVOffset.y = 0;
		mToonGradientCutoff = -1.0f;
		mSpecularGlossExponent = 8.0f;
		mHDRLightmapScale = 6.0f;
		mSwizzle.mSwizzle[0] = RenderSwizzleType::eRenderSwizzle_R;
		mSwizzle.mSwizzle[1] = RenderSwizzleType::eRenderSwizzle_G;
		mSwizzle.mSwizzle[2] = RenderSwizzleType::eRenderSwizzle_B;
		mSwizzle.mSwizzle[3] = RenderSwizzleType::eRenderSwizzle_A;
		mAlphaMode = AlphaMode::eTxAlphaUnkown;
		mPlatform.mVal = PlatformType::ePlatform_PC;
		mType = TextureType::eTxStandard;
		mNumSurfacesLoaded = 0;
		mNumSurfacesRequested = 0;
		mNumSurfacesRequired = 0;
		mNumMipLevelsAllocated = 0;
	}

	bool IsCompressed() {
		int v1 = (int)mSurfaceFormat;
		return (unsigned int)(v1 - 64) <= 5
			|| v1 >= 64 && (v1 <= 68 || (unsigned int)(v1 - 70) <= 1)
			|| (unsigned int)(v1 - 80) <= 3
			|| v1 == 128;
	}

	inline int GetNumSlices() {
		int result = 0;
		if (mTextureLayout == T3TextureLayout::eTextureLayout_3D) {
			result = mDepth;
		}
		else if ((unsigned int)(mTextureLayout - 3) > 1) {
			result = 1;
		}
		else {
			result = mArraySize;
		}
		return result;
	}

	inline int GetNumFaces() {
		int result = 0;
		if (mTextureLayout == T3TextureLayout::eTextureLayout_CubeArray) {
			result = 6 * mArraySize;
		}
		else if (mTextureLayout == T3TextureLayout::eTextureLayout_2DArray) {
			result = mArraySize * 1;
		}
		else {
			result = mTextureLayout == T3TextureLayout::eTextureLayout_Cube ? 6 : 1;
		}
		return result;
	}

	/// <summary>
	/// Gets texture data from this texture. The mip index is index of the mipmap texture data to retrieve (between 0 and mNumMipLevels-1)
	/// ArrayIndex is the index of the region in the mipmap of texture data to get. In a cube map, this is an index between 0 and 5.
	/// The mArraySize member variable tells you how many regions each mipmap has (in a cube map, its 6, in a standard its 1).
	/// This will return NULL if the indicies passed are invalid or there is no loaded texture data.
	/// </summary>
	DataStream* GetTxData(int pMipIndex, int pArrayIndex = 0) {
		if (pMipIndex >= mNumMipLevels || pArrayIndex >= mArraySize)
			return NULL;
		for (int i = 0; i < mRegionHeaders.GetSize(); i++) {
			RegionStreamHeader* header = mRegionHeaders.mpStorage + i;
			if (header->mMipIndex == pMipIndex && header->mFaceIndex == pArrayIndex) {
				return header->mpAsyncStream;
			}
		}
		return NULL;
	}

	bool SetTxData(DataStream* stream, int pMipIndex, int pArrayIndex = 0) {
		if (pMipIndex >= mNumMipLevels || pArrayIndex >= mArraySize)
			return false;
		for (int i = 0; i < mRegionHeaders.GetSize(); i++) {
			RegionStreamHeader* header = mRegionHeaders.mpStorage + i;
			if (header->mMipIndex == pMipIndex && header->mFaceIndex == pArrayIndex) {
				if (header->mpAsyncStream && header->mpAsyncStream != stream)
					delete header->mpAsyncStream;
				header->mpAsyncStream = stream;
				return true;
			}
		}
		return false;
	}

	static const char* GetTextureTypeName(TextureType type) {
		const char* result; //RAX REGISTER
		switch (type)
		{
		case 0:
			result = "Unknown";
			break;
		case 1:
			result = "LightMapV0";
			break;
		case 2:
			result = "BumpMap";
			break;
		case 3:
			result = "NormalMap";
			break;
		case 4:
			result = "UNUSED0";
			break;
		case 5:
			result = "UNUSED1";
			break;
		case 6:
			result = "SubsurfaceScatteringMapV0";
			break;
		case 7:
			result = "SubsurfaceScatteringMap";
			break;
		case 8:
			result = "DetailMap";
			break;
		case 9:
			result = "StaticShadowMap";
			break;
		case 10:
			result = "LightmapHDR";
			break;
		case 11:
			result = "SharpDetailMap";
			break;
		case 12:
			result = "EnvMap";
			break;
		case 13:
			result = "SpecularColorMap";
			break;
		case 14:
			result = "ToonLookupMap";
			break;
		case 15:
			result = "DiffuseColorMap";
			break;
		case 16:
			result = "OutlineMap";
			break;
		case 17:
			result = "LightmapHDRScaled";
			break;
		case 18:
			result = "EmissiveMap";
			break;
		case 19:
			result = "ParticleProperties";
			break;
		case 20:
			result = "BrushNormalMap";
			break;
		case 21:
			result = "UNUSED2";
			break;
		case 22:
			result = "NormalGlossMap";
			break;
		case 23:
			result = "LookupMap";
			break;
		case 40:
			result = "LookupXYMap";
			break;
		case 24:
			result = "AmbientOcclusionMap";
			break;
		case 25:
			result = "PrefilteredEnvCubeMapHDR";
			break;
		case 35:
			result = "PrefilteredEnvCubeMapHDRScaled";
			break;
		case 26:
			result = "BrushLookupMap";
			break;
		case 27:
			result = "Vector2Map";
			break;
		case 28:
			result = "NormalDxDyMap";
			break;
		case 29:
			result = "PackedSDFMap";
			break;
		case 30:
			result = "SingleChannelSDFMap";
			break;
		case 31:
			result = "LightmapDirection";
			break;
		case 32:
			result = "LightmapStaticShadows";
			break;
		case 33:
			result = "LightStaticShadowMapAtlas";
			break;
		case 34:
			result = "LightStaticShadowMap";
			break;
		case 38:
			result = "NormalXYMap";
			break;
		case 41:
			result = "ObjectNormalMap";
			break;
		default:
			result = "InvalidMap";
			break;
		}
		return result;
	}

	static T3SurfaceGamma GetDefaultGammaForType(TextureType type) {
		return type >= TextureType::eTxLightmapFlatAtlas ? T3SurfaceGamma::eSurfaceGamma_sRGB: T3SurfaceGamma::eSurfaceGamma_Linear;
	}

	~T3Texture() {
		for (auto it = mAuxilaryData.begin(); it != mAuxilaryData.end(); it++) {
			delete* it;
		}
	}

	struct StreamHeader {
		int mRegionCount;
		int mAuxDataCount;
		int mTotalDataSize;
	};

	DCArray<RegionStreamHeader> mRegionHeaders;

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, 
		MetaClassDescription* pObjDesc, MetaMemberDescription* pCtx, void* pUserData) {
		MetaStream* stream = static_cast<MetaStream*>(pUserData);
		T3Texture* tex = static_cast<T3Texture*>(pObj);
		MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj, pObjDesc, pCtx, pUserData);
		if (result == eMetaOp_Succeed) {
			int arraysize = 0;
			if (tex->mTextureLayout == eTextureLayout_CubeArray)
				arraysize = 6 * tex->mArraySize;
			else if (tex->mTextureLayout == eTextureLayout_2DArray)
				arraysize = tex->mArraySize;
			else {
				arraysize = tex->mTextureLayout == eTextureLayout_Cube ? 6 : 1;
			}
			tex->mArraySize = arraysize;
			StreamHeader header;
			if (stream->mMode == MetaStreamMode::eMetaStream_Read) {
				PerformMetaSerializeAsync(stream, &header);
				if (header.mRegionCount > tex->mRegionHeaders.mCapacity)
					tex->mRegionHeaders.ReserveAndResize(header.mRegionCount - tex->mRegionHeaders.mCapacity);
				else 
					tex->mRegionHeaders.mSize = header.mRegionCount;
				for (int i = 0; i < header.mRegionCount; i++) {
					RegionStreamHeader* ptr = tex->mRegionHeaders.mpStorage + i;
					PerformMetaSerializeAsync(stream, ptr);
					if (!ptr->mMipCount)
						ptr->mMipCount = 1;
					if (!ptr->mSlicePitch)
						ptr->mSlicePitch = ptr->mDataSize;
				}
				if (header.mAuxDataCount) {
					stream->BeginBlock();
					for (int i = 0; i < header.mAuxDataCount; i++) {
						AuxilaryData* data = new AuxilaryData;
						PerformMetaSerializeAsync(stream, data);
						tex->mAuxilaryData.push_back(data);
					}
					stream->EndBlock();
				}
				stream->BeginAsyncSection();
				if (header.mTotalDataSize > 0) {
					for (int i = 0; i < header.mRegionCount; i++) {
						RegionStreamHeader* region = tex->mRegionHeaders.mpStorage + i;
						if (region->mpAsyncStream)
							delete region->mpAsyncStream;
						DataStreamMemory* mem  = new DataStreamMemory(region->mDataSize, 0x1000);
						region->mpAsyncStream = mem;
						stream->serialize_bytes(mem->mMemoryBuffer, region->mDataSize);
						mem->SetMode(DataStreamMode::eMode_Read);
					}
				}
				stream->EndAsyncSection();
			}
			else {
				int regions = arraysize * tex->mNumMipLevels;//v13
				//arraysize = v12, v43 = temp
				//v81 = arraysize
				header.mRegionCount = regions;
				int tot = 0;
				for (int i = 0; i < tex->mRegionHeaders.GetSize(); i++) {
					RegionStreamHeader* header = tex->mRegionHeaders.mpStorage + i;
					if (!header->mpAsyncStream) {
						TelltaleToolLib_RaiseError("Could not finish writing texture: No texture data found in region", ErrorSeverity::WARN);
						return eMetaOp_Fail;
					}
					if (header->mpAsyncStream->GetSize() != header->mDataSize)
						header->mDataSize = header->mpAsyncStream->GetSize();
					tot += header->mDataSize;
				}
				header.mTotalDataSize = tot;
				header.mAuxDataCount = tex->mAuxilaryData.size();
				PerformMetaSerializeAsync(stream, &header);
				for (int i = 0; i < tex->mRegionHeaders.GetSize(); i++) {
					PerformMetaSerializeAsync(stream, tex->mRegionHeaders.mpStorage + i);
				}
				if (header.mAuxDataCount) {
					stream->BeginBlock();
					for (int i = 0; i < header.mAuxDataCount; i++) {
						PerformMetaSerializeAsync(stream, tex->mAuxilaryData[i]);
					}
					stream->EndBlock();
				}
				stream->BeginAsyncSection();
				char buf[40];
				for (int i = 0; i < tex->mRegionHeaders.GetSize(); i++) {
					RegionStreamHeader* header = tex->mRegionHeaders.mpStorage + i;
					header->mpAsyncStream->SetMode(DataStreamMode::eMode_Read);
					header->mpAsyncStream->SetPosition(0,DataStreamSeekType::eSeekType_Begin);
					char* tmp = new char[header->mpAsyncStream->GetSize()];
					header->mpAsyncStream->Serialize(tmp, header->mpAsyncStream->GetSize());
					sprintf(buf, "Mip %d", header->mMipIndex);
					stream->Key(buf);
					stream->serialize_bytes(tmp, header->mpAsyncStream->GetSize());
					delete[] tmp;
				}
				stream->EndAsyncSection();
			}
		}
		return result;
	}

};

#endif