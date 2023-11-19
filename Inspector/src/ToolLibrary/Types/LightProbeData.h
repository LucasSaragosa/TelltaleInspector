// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "../Meta.hpp"
#include "ObjectSpaceIntrinsics.h"

//LIGHT PROBES ARE NOT SUPPORTED IN THIS LIBRARY.

enum T3LightQualityType {
	eLightQuality_High = 0,
	eLightQuality_Medium = 1,
	eLightQuality_Low = 2,
	eLightQuality_Lowest = 3,
	eLightQuality_Count = 4
};

struct Tetrahedron {
	Vector3 mMatrix0, mMatrix1, mMatrix2;
	int mNeighbourIndex[4];
	short mPointIndex[4];
};

struct TetrahedralMeshData {
	long mVersion;
	long mTetrahedraCount;
	long mVertexCount;

	void* mpMemoryBuffer;
	Vector3* mpVertices;
	Tetrahedron* mpTetrahedra;
	int mAllocationSize;

	bool Allocate() {
		mAllocationSize = 12 * (mVertexCount + 5 * mTetrahedraCount);
		mpMemoryBuffer = ::operator new[](mAllocationSize);
		if (mpMemoryBuffer) {
			mpTetrahedra = (Tetrahedron*)mpMemoryBuffer;
			mpVertices = (Vector3*)&(((Tetrahedron*)mpMemoryBuffer)[mTetrahedraCount].mMatrix0);
		}
		return mpMemoryBuffer != NULL;
	}

	inline void _DoDeallocate() {
		if (mpMemoryBuffer)
			delete[] mpMemoryBuffer;
		mpTetrahedra = NULL;
		mpVertices = NULL;
		mpMemoryBuffer = NULL;
		mAllocationSize = 0;
		mTetrahedraCount = 0;
		mVertexCount = 0;
	}

	void ReadBlock(MetaStream* stream) {
		stream->serialize_bytes(mpMemoryBuffer, mAllocationSize);
	}

	~TetrahedralMeshData()
	{
		_DoDeallocate();
	}

	TetrahedralMeshData() {
		mVersion = 0;
		mpTetrahedra = NULL;
		mpVertices = NULL;
		mpMemoryBuffer = NULL;
		mAllocationSize = 0;
		mTetrahedraCount = 0;
		mVertexCount = 0;
	}

};

struct LightProbeState {
	Color mSH[4];
	Vector4 mWeights;
	int mTetIndex;
	int mProbeIndex[4];
	int mProbeCount;
};

//.PROBE FILES
struct LightProbeData {

	struct CompressedProbeSH {
		i16 mRed[4], mGreen[4], mBlue[4];
	};

	String mName;
	long mVersion;
	TetrahedralMeshData mTetrahedralMeshData;
	CompressedProbeSH* mProbeSH[4];

	void _Validate() {}

	bool _ReadLegacySHData(MetaStream* stream, CompressedProbeSH* pProbeSH, unsigned int probeCount) {
		char* temp = (char*)malloc(56 * probeCount);
		stream->serialize_bytes(temp, 56 * probeCount);
		CompressedProbeSH* v5 = pProbeSH;
		signed __int64 v8;
		char* v9;
		unsigned int v4 = probeCount;
		if (v4) {
			__int64 v10;
			v8 = (signed __int64)&v5->mBlue[1];
			v9 = temp + 38;
			v10 = v4;
			do
			{
				*(i16*)(v8 - 18) = *((i16*)v9 - 19);
				*(i16*)(v8 - 10) = *((i16*)v9 - 10);
				*(i16*)(v8 - 2) = *((i16*)v9 - 1);
				*(i16*)(v8 - 16) = *((i16*)v9 - 18);
				*(i16*)(v8 - 8) = *((i16*)v9 - 9);
				*(i16*)v8 = *(i16*)v9;
				*(i16*)(v8 - 14) = *((i16*)v9 - 17);
				*(i16*)(v8 - 6) = *((i16*)v9 - 8);
				*(i16*)(v8 + 2) = *((i16*)v9 + 1);
				*(i16*)(v8 - 12) = *((i16*)v9 - 16);
				*(i16*)(v8 - 4) = *((i16*)v9 - 7);
				*(i16*)(v8 + 4) = *((i16*)v9 + 2);
				v9 += 56;
				v8 += 24i64;
				--v10;
			} while (v10);
		}
		free(temp);
		return true;
	}

	CompressedProbeSH* _Allocate(int probeCount, T3LightQualityType quality) {
		CompressedProbeSH* result = mProbeSH[(int)quality];
		if (result != NULL)
			return result;
		return mProbeSH[(int)quality] = new CompressedProbeSH[probeCount];
	}

	LightProbeData() {
		for (int i = 0; i < 4; i++)
			mProbeSH[i] = NULL;
		mVersion = 0;
	}

	inline void Clear() {
		for (int i = 0; i < 4; i++)
			if (mProbeSH[i] != NULL)
				delete[] mProbeSH[i];
		mTetrahedralMeshData._DoDeallocate();
		memset(mProbeSH, NULL, sizeof(CompressedProbeSH*) * 4);
	}

	~LightProbeData() {
		Clear();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		return eMetaOp_Succeed;
	}

};