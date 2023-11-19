// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _WBOX
#define _WBOX

#include "../Meta.hpp"
#include "ObjectSpaceIntrinsics.h"
#include "DCArray.h"
#include "SArray.h"
#include "SoundFootsteps.h"

//.WBOX FILES
struct WalkBoxes {

	struct Edge {
		Flags mFlags;
		int mV1, mV2;
		int mEdgeDest, mEdgeDestEdge, mEdgeDir;
		float mMaxRadius;
	};

	struct Tri {
		SoundFootsteps::EnumMaterial mFootstepMaterial;
		Flags mFlags;
		int mNormal;//INDEX
		int mQuadBuddy;//INDEX
		float mMaxRadius;
		SArray<int, 3> mVerts;
		SArray<Edge, 3> mEdgeInfo;
		SArray<int, 3> mVertOffsets;
		SArray<float, 3> mVertScales;
		
		Tri() {
			mFootstepMaterial.mVal = SoundFootsteps::Material::Default;
			mQuadBuddy = -1;
			mNormal = 0;
			mMaxRadius = 0;
			mFlags.mFlags = 0;
		}

	};

	struct Vert {
		Flags mFlags;
		Vector3 mPos;

		Vert() {
			mPos.y = 0;
			mFlags.mFlags = 0;
		}

	};

	struct Quad {
		SArray<int, 4> mVerts;
	};

	String mName;
	DCArray<Tri> mTris;
	DCArray<Vert> mVerts;
	DCArray<Vector3> mNormals;
	DCArray<Quad> mQuads;

	void UpdateAll() {
		//yh im not converting 1500 lines of code
	}

	int AddVert(Vector3 vec, bool bForceUnique) {
		int result = 0;
		if (bForceUnique || mVerts.mSize <= 0) {
		unique:
			if (mVerts.mCapacity == mVerts.mSize) {
				mVerts.Resize(mVerts.mCapacity < 4 ? 4 : mVerts.mCapacity);
			}
			Vert* nvert = mVerts.mpStorage + mVerts.mSize;
			nvert->mFlags.mFlags = 0;
			nvert->mPos.x = vec.x;
			nvert->mPos.y = vec.y;
			nvert->mPos.z = vec.z;
			mVerts.mSize++;
			result = mVerts.mSize - 1;
		}
		else {
			float v7 = vec.x;
			float v9 = vec.y;
			float v10 = vec.z;
			int index = 0;
			float* v12 = &mVerts.mpStorage->mPos.z;
			while ((float)((float)((float)((float)(v9 - *(v12 - 1)) * (float)(v9 - *(v12 - 1)))
				+ (float)((float)(v7 - *(v12 - 2)) * (float)(v7 - *(v12 - 2))))
				+ (float)((float)(v10 - *v12) * (float)(v10 - *v12))) >= 1.0e-12)
			{
				++index;
				if (index >= mVerts.mSize)
					goto unique;
				v12 = &mVerts.mpStorage[index].mPos.z;
			}
			result = index;
		}
		return result;
	}

	//returns index in tri array of new tri
	int AddTri(Vector3 v0, Vector3 v1, Vector3 v2, bool bForceUnique) {
		if (mTris.mCapacity == mTris.mSize) {
			mTris.Resize(mTris.mCapacity < 4 ? 4 : mTris.mCapacity);
		}
		Tri* ntri = mTris.mpStorage + mTris.mSize;
		mTris.mSize++;
		ntri->mVerts.mData[0] = AddVert(v0,bForceUnique);
		ntri->mVerts.mData[1] = AddVert(v1, bForceUnique);
		ntri->mVerts.mData[2] = AddVert(v2, bForceUnique);
		UpdateAll();
		return mTris.mSize - 1;//index
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDesc, MetaMemberDescription* pCtx, void* pUserData) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDesc, pCtx, pUserData);
		if (r != eMetaOp_Succeed)return r;
		WalkBoxes* v6 = static_cast<WalkBoxes*>(pObj);
		int v7; // ebp@1
		__int64 v8; // rsi@2
		WalkBoxes::Vert* v9; // rcx@3
		WalkBoxes::Quad* v10; // rbx@3
		signed __int64 v11; // rax@3
		int v12; // xmm1_4@3
		int v13; // xmm0_4@3
		Vector3 v14; // r9@3
		Vector3 v15; // r8@3
		WalkBoxes::Vert* v16; // rcx@3
		signed __int64 v17; // rax@3
		int v18; // xmm1_4@3
		int v19; // xmm0_4@3
		Vector3 v20; // r9@3
		Vector3 v21; // r8@3
		float v23; // [sp+30h] [bp-28h]@3
		int v24; // [sp+34h] [bp-24h]@3
		int v25; // [sp+38h] [bp-20h]@3
		float v26; // [sp+40h] [bp-18h]@3
		int v27; // [sp+44h] [bp-14h]@3
		int v28; // [sp+48h] [bp-10h]@3
		v7 = 0;
		v7 = 0;
		if (v6->mQuads.mSize > 0)
		{
			v8 = 0i64;
			do
			{
				v9 = v6->mVerts.mpStorage;
				v10 = v6->mQuads.mpStorage;
				v11 = v10[v8].mVerts.mData[0];
				v12 = v9[v10[v8].mVerts.mData[0]].mPos.y;
				v23 = v9[v10[v8].mVerts.mData[0]].mPos.x;
				v13 = v9[(unsigned __int64)(4 * v11) / 0x10].mPos.z;
				v14 = v9[v10[v8].mVerts.mData[2]].mPos;
				v24 = v12;
				v15 = v9[v10[v8].mVerts.mData[1]].mPos;
				v25 = v13;
				v6->AddTri(v9[v10[v8].mVerts.mData[0]].mPos, v15, v14, 0);
				v16 = v6->mVerts.mpStorage;
				v17 = v10[v8].mVerts.mData[3];
				v18 = v16[v10[v8].mVerts.mData[3]].mPos.y;
				v26 = v16[v10[v8].mVerts.mData[3]].mPos.x;
				v19 = v16[(unsigned __int64)(4 * v17) / 0x10].mPos.z;
				v20 = v16[v10[v8].mVerts.mData[1]].mPos;
				v27 = v18;
				v21 = v16[v10[v8].mVerts.mData[2]].mPos;
				v28 = v19;
				v6->AddTri(v16[v10[v8].mVerts.mData[3]].mPos, v21, v20, 0);
				++v7;
				++v8;
			} while (v7 < v6->mQuads.mSize);
		}
		v6->mQuads.mSize = 0;
		v6->UpdateAll();
		return r;
	}

};

#endif