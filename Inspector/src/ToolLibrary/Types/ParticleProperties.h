#ifndef _PARTICLE_PROPS
#define _PARTICLE_PROPS

#include "../Meta.hpp"
#include "DCArray.h"
#include "ObjectSpaceIntrinsics.h"
#include "HandleObjectInfo.h"
#include "Chore.h"
#include "Animation.h"
#include "T3Texture.h"

struct ParticlePropConnect {
	ParticlePropModifier mModType;
	ParticlePropDriver mDriveType;
	float mDriveMin, mDriveMax;
	float mModMin, mModMax;
	bool mInvert;
};

struct ParticlePropertySamples {

	struct Sample {
		Vector4 mPosition, mOrientation, mColor;
	};

	Sample* mpSamples;
	long mSampleCount;

	ParticlePropertySamples() : mSampleCount(0), mpSamples(0) {}

	~ParticlePropertySamples() {
		if (mpSamples)
			delete[] mpSamples;
		mpSamples = 0;
		mSampleCount = 0;
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* pStream = (MetaStream*)pUserData;
		ParticlePropertySamples* me = (ParticlePropertySamples*)pObj;
		MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (result == eMetaOp_Succeed) {
			if (pStream->IsRead()) {
				if (me->mpSamples)
					delete[] me->mpSamples;
				me->mpSamples = new Sample[me->mSampleCount];
			}
			pStream->serialize_bytes(me->mpSamples, me->mSampleCount * sizeof(Sample));//can just do this, engine used this in tool only
		}
		return result;
	}

};

//.PARTICLE FILES - TODO
struct ParticleProperties {

	enum AnimationDataFlags {
		eAnimData_Chore = 1,
		eAnimData_Samples = 2,
	};

	struct AnimationParams {
		Vector3 mPositionMin;
		Vector3 mPositionMax;
	};

	struct Animation {

		Symbol mName;
		AnimationParams mParams;
		Flags mDataFlags;
		Ptr<ParticlePropertySamples> mpSamples;
		Handle<Chore> mhChore;
		Ptr<T3Texture> mpTexture[4];

	};

	String mName;
	Flags mTextureFlags;
	long mTextureCount;
	DCArray<Animation> mAnimations;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		ParticleProperties* me = (ParticleProperties*)pObj;
		for (int i = 0; i < me->mAnimations.GetSize(); i++) {
			Animation& anim = me->mAnimations[i];
			//if(anim.mDataFlags.mFlags)
		}
		if (me->mTextureCount > 0) {
			//TODO
		}
		return result;
	}

};

#endif