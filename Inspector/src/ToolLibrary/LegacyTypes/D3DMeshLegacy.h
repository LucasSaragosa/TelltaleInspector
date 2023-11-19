#pragma once

#include "../Meta.hpp"
#include "../Types/D3DMesh.h"
#include "../types/ObjectSpaceIntrinsics.h"
#include "../Types/DCArray.h"
#include "../Types/SArray.h"
#include "../types/Map.h"

class LegacyD3DMesh {
public:

	class T3VertexComponent {
	public:

		unsigned int mOffset;
		unsigned int mCount;
		int mType;

	};

	//mVertexComponents[THIS]
	enum T3VertexComponentType {
		COMPONENT_TYPE_POSITION = 0,
		COMPONENT_TYPE_UV = 1,
		COMPONENT_TYPE_NORMAL = 2,
		COMPONENT_TYPE_BLEND_WEIGHT = 3,
		COMPONENT_TYPE_BLEND_INDEX = 4,
		COMPONENT_TYPE_COLOUR = 5,
		// ??
		COMPONENT_TYPE_UNSURE_SOFTWARE_SKINNING_DATA = 6,
		COMPONENT_TYPE_SMOOTH_NORMAL = 7,
		COMPONENT_TYPE_TANGENT = 8,
		COMPONENT_TYPE_UV2 = 9,
		COMPONENT_TYPE_UV3 = 10,
		COMPONENT_TYPE_UV4 = 11,
		// ??
		COMPONENT_TYPE_UNSURE_INTERLEAVED_DATA = 12,

	};

	class T3VertexBuffer {
	public:

		int mNumVerts;
		int mVertSize;
		Flags mFlags;
		int mUsage;
		SArray<T3VertexComponent, 13> mVertexComponents;
		void* pData;

		T3VertexBuffer() : mVertexComponents() {
			pData = 0;
			mFlags = mVertSize = mNumVerts = mUsage = 0;
		}

		~T3VertexBuffer() {
			if (pData)
				free(pData);
			pData = 0;
		}

	};

	class T3IndexBuffer {
	public:

		int mFormat;
		int mNumIndicies;
		Flags mFlags;
		int mUsage;
		void* pData;

		T3IndexBuffer() {
			pData = 0;
			mFlags = mNumIndicies = mUsage = mFormat = 0;
		}

		~T3IndexBuffer() {
			if (pData)
				free(pData);
			pData = 0;
		}

	};

	static inline int get_index_buffer_format_size(int fmt) {
		return 2;
	}

	struct TriangleSet {
		Flags mFlags;
		int mBonePaletteIndex;
		int mLocalTransformPaletteIndex;
		int mLocalTransformIndex;
		int mMinVertIndex;
		int mMaxVertIndex;
		int mStartIndex;
		int mNumPrimitives;
		BoundingBox mBoundingBox;
		Sphere mBoundingSphere;
		SArray<int, 14> mTxIndex;
		DCArray<unsigned short> mTriStrips;
		int mNumTotalIndices;
		float mfBumpHeight;
		float mfDetailBumpHeight;
		float mSpecularPower;
		Color mSpecularColor;
		Color mAmbientColor;
		Color mToonOutlineColor;
		Color mToonOutlineInvertColor;
		float mToonOutlineSize;
		float mToonMaxZConstOutlineSize;
		float mToonMinZConstOutlineSize;
		float mGlowIntensity;
		float mReceiveShadowIntensity;
		int mAlphaMode;
		float mfReflectivity;
		int mToonShades;
		int mUVGenMode;
		float mUVScreenSpaceScaling;
		float mSpecularOnAlpha;
		float mSubsurfaceScateringRadius;
		Color mDiffuseColor;
	};

	struct VertexAnimation {

	};

	struct PaletteEntry {
		Symbol mBoneName;
		BoundingBox mBoundingBox;
		Sphere mBoundingSphere;
		int mNumVerts;
	};

	struct SkinningEntry {
		
	};

	struct BoneEntry {

	};

	struct LocalTransformEntry {
		Transform mTransform;
		int mCameraFacingType;
	};

	struct Texture {
		Handle<T3Texture> mName;
		Flags mFlags;
		Symbol mNameSymbol;
		BoundingBox mBoundingBox;
		Sphere mBoundingSphere;
		float mMaxObjAreaPerUVArea;
		float mAverageObjAreaPerUVArea;
	};

	struct AnimatedVertexEntry {

	};

	struct AnimatedVertexGroupEntry {

	};

	enum MeshFlags {
		eHasIndexBuffer = 1,
		eHasPosStream = 2,
		eHasSoftwareSkinningStream = 0x400000,
		eHasInterleavedStream = 0x200000,
		eHasVertexAnimation = 0x10000,
		eHasColorStream = 0x800,
		eHasTangentStream = 0x400,
		eHasUV4Stream = 0x200,
		eHasUV3Stream = 0x100,
		eHasUV2Stream = 0x80,
		eHasUV1Stream = 0x40,
		eHasBlendIdxStream = 0x20,
		eHasBlendWeightStream = 0x10,
		eHasSmoothNormStream = 0x8,
		eHasNormStream = 0x4,

		eIsManualSort = 0x100000,
		eHasZeroVertexAlpha = 0x80000,
		eTriangleSetsFixedUp =0x40000,
	};

	enum T3TextureIndex {
		TEXTURE_INDEX_DIFFUSE = 0,
		TEXTURE_INDEX_LIGHTMAP_V0 = 1,
		TEXTURE_INDEX_NORMAL_MAP = 2,
		TEXTURE_INDEX_ENV_MAP = 3,
		TEXTURE_INDEX_DETAIL = 4,
		TEXTURE_INDEX_UNKNOWN0 = 5,//DETAIL NORMAL? BUMPMAP?
		TEXTURE_INDEX_SPECULAR = 6,
		TEXTURE_INDEX_UNKNOWN1 = 7,//??
		TEXTURE_INDEX_UNKNOWN2 = 8,//LIGHTWARP?
		TEXTURE_INDEX_UNKNOWN3 = 9,//??
		TEXTURE_INDEX_STATIC_SHADOW = 10,
		TEXTURE_INDEX_EMMISIVE = 11,
		TEXTURE_INDEX_NORMAL_GLOSS = 12,//TODO CHECK
		TEXTURE_INDEX_AMBIENT_OCCLUSION = 13,
		//unkowns could be: subsurface scat (normal or v0), lightmap hdr and scaled, sdf detail, spec colour, toon lookup, outling discontinuinity, particle props, lookup map, prefiltered cube, brush lookup,
	};

	String mName;
	int mVersion;
	Flags mFlags;
	BoundingBox mBoundingBox;
	Sphere mBoundingSphere;
	DCArray<TriangleSet> mTriangleSets;
	DCArray<VertexAnimation> mVertexAnimations;
	DCArray<DCArray<PaletteEntry>> mBonePalettes;
	DCArray<SkinningEntry> mSkinningData;
	DCArray<BoneEntry> mBoneData;
	DCArray<DCArray<LocalTransformEntry>> mLocalTransformPalettes;
	int mTriangleStripState;
	int mAnimatedVertexCount;
	SArray<DCArray<Texture>, 14> mTextures;/*T3TEXTUREINDEX*/
	Vector2 mDiffuseScale, mLightMapScale, mShadowMapScale, mDetailScale, mScaledUVScale;
	DCArray<AnimatedVertexEntry> mToolAnimatedVertexEntries;
	Map<Symbol, AnimatedVertexGroupEntry> mToolAnimatedVertexGroupEntries;
	ToolProps mToolProps;

	T3IndexBuffer mIndexBuffer;
	T3VertexBuffer mVertexBuffer[2];

	LegacyD3DMesh() {
		mName = "";
		mVersion = 0;
		mFlags = 0;
		mIndexBuffer.pData = 0;
		mVertexBuffer[0].pData = mVertexBuffer[1].pData = 0;
	}

	INLINE static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult res = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (res == eMetaOp_Succeed) {
			LegacyD3DMesh* pThis = (LegacyD3DMesh*)pObj;
			PerformMetaSerializeAsync((MetaStream*)pUserData, &pThis->mIndexBuffer);
			if (pThis->mFlags.mFlags & eHasIndexBuffer) {
				if (pThis->mIndexBuffer.pData)
					free(pThis->mIndexBuffer.pData);
				int size = 0;
				pThis->mIndexBuffer.pData = malloc(size = (pThis->mIndexBuffer.mNumIndicies * get_index_buffer_format_size(pThis->mIndexBuffer.mFormat)));
				((MetaStream*)pUserData)->serialize_bytes(pThis->mIndexBuffer.pData, size);
			}
			for (int i = 0; i < 2; i++) {
				PerformMetaSerializeAsync((MetaStream*)pUserData, &pThis->mVertexBuffer[i]);
				if (pThis->mVertexBuffer[i].pData)
					free(pThis->mVertexBuffer[i].pData);
				pThis->mVertexBuffer[i].pData = malloc(pThis->mVertexBuffer[i].mNumVerts * pThis->mVertexBuffer[i].mVertSize);
				((MetaStream*)pUserData)->serialize_bytes(pThis->mVertexBuffer[i].pData, pThis->mVertexBuffer[i].mNumVerts * pThis->mVertexBuffer[i].mVertSize);
			}
		}
		return res;
	}

};