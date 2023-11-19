// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _ANIMATED_VALUE_INTERFACE
#define _ANIMATED_VALUE_INTERFACE

#include "../Meta.hpp"
#include "ObjectSpaceIntrinsics.h"
#include "../TelltaleToolLibrary.h"

struct AnimationValueSerializeContext {
	MetaStream* mStream;
	char* mpBuffer;
	int mBufferSize;
	int mBufferOffset;

	void BeginValue() {
		mBufferOffset = (mBufferOffset + 0x1F) & 0xFFFFFFE0;
	}

	char* Allocate(int size) {
		int o = mBufferOffset;
		mBufferOffset += ((size + 3) & 0xFFFFFFFC);
		return &mpBuffer[o];
	}

};

struct AnimationValueInterfaceBase {

	enum Flags {
		eDisabled = 0x1,
		eTimeBehavior = 0x2,
		eWeightBehavior = 0x4,
		eMoverAnim = 0x10,
		ePropertyAnimation = 0x20,
		eTextureMatrixAnimation = 0x40,
		eAudioDataAnimation = 0x80,
		eDontOptimize = 0x100,
		eHomogeneous = 0x200,
		eMixerScaled = 0x400,
		eMixerHomogeneous = 0x800,
		eStyleAnimation = 0x1000,
		ePropForceUpdate = 0x2000,
		eMixerOwned = 0x4000,
		eMixerDirty = 0x8000, 
		eAdditive = 0x10000,
		eExternallyOwned = 0x20000,
		eDontMixPausedControllers = 0x40000,
		eRuntimeAnimation = 0x80000,
		eTransientAnimation = 0x100000,
		eToolOnly = 0x200000,
		eKeyedAttachmentAnimation = 0x400000,
		eMixerWeightedBlend = 0x800000,
		eValueKind = 0xFF000000,
	};

	Symbol mName;
	u32 mFlags;
	//DO NOT TOUCH
	u16 mVersion;
	 
	AnimationValueInterfaceBase() :mFlags(0), mVersion(0){}

	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<AnimationValueInterfaceBase>();
	}

	virtual MetaClassDescription* GetValueClassDescription() {
		return NULL;
	}

	virtual void* GetMetaClassObjPointer() {
		return this;
	}

	INLINE bool GetAdditive() {
		//if dirty, clearmixer etc
		return !(mFlags & Flags::eAdditive);
	}

	INLINE bool GetActive() {
		return !(mFlags & Flags::eDisabled);
	}

	virtual float GetMinTime() {
		return 10'000.0f;
	}

	INLINE void SetType(int type) {
		mFlags |= (type << 24);
	}

	virtual MetaOpResult SerializeIn(AnimationValueSerializeContext* context, u32 version) {
		return PerformMetaSerializeFull(context->mStream, GetMetaClassObjPointer(), GetMetaClassDescription());
	}

	virtual void SerializeOut(MetaStream* stream) {
		PerformMetaSerializeFull(stream, GetMetaClassObjPointer(), GetMetaClassDescription());
	}

	virtual ~AnimationValueInterfaceBase() {}

};

template<typename T> struct AnimatedValueInterface : AnimationValueInterfaceBase {

	virtual MetaClassDescription* GetValueClassDescription() override {
		return ::GetMetaClassDescription<T>();
	}

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<AnimatedValueInterface<T>>();
	}

};

struct Float4 {
	float v[4];//most likely platform specific for accel computation
};

struct SkeletonPose {
	Float4* mEntries;

	SkeletonPose() {
		mEntries = NULL;
	}

	void SetTransform(int i, Transform* src) {
		mEntries[0].v[(i & 3) + 4 * (7 * (i / 4))] = src->mTrans.x;
		mEntries[1].v[(i & 3) + 4 * (7 * (i / 4))] = src->mTrans.y;
		mEntries[2].v[(i & 3) + 4 * (7 * (i / 4))] = src->mTrans.z;
		mEntries[3].v[(i & 3) + 4 * (7 * (i / 4))] = src->mRot.x;
		mEntries[4].v[(i & 3) + 4 * (7 * (i / 4))] = src->mRot.y;
		mEntries[5].v[(i & 3) + 4 * (7 * (i / 4))] = src->mRot.z;
		mEntries[6].v[(i & 3) + 4 * (7 * (i / 4))] = src->mRot.w;
	}

	void GetTransform(Transform* dst, int i) {
		int index = (i & 3) + 28 * (i / 4);
		dst->mTrans.x = mEntries[0].v[index];
		dst->mTrans.y = mEntries[1].v[index];
		dst->mTrans.z = mEntries[2].v[index];
		dst->mRot.x = mEntries[3].v[index];
		dst->mRot.y = mEntries[4].v[index];
		dst->mRot.z = mEntries[5].v[index];
		dst->mRot.w = mEntries[6].v[index];
	}

	~SkeletonPose() = default;

};

struct CompressedSkeletonPoseKeys2 : AnimationValueInterfaceBase {

	//format for data header in mpData once serialized in
	struct Header {
		Vector3 mMinDeltaV, mRangeDeltaV, mMinDeltaQ, mRangeDeltaQ, mMinVector, mRangeVector;
		int mRangeTime, mBoneCount, mSampleDataSize;
	};

	char* mpData;
	u32 mDataSize;
	bool mDataExternallyOwned;

	CompressedSkeletonPoseKeys2() {
		mpData = NULL;
		mDataSize = 0;
		mDataExternallyOwned = false;
	}

	virtual void SerializeOut(MetaStream* stream) override {
		stream->serialize_uint32(&mDataSize);
		stream->serialize_bytes(mpData, mDataSize);
	}

	virtual void* GetMetaClassObjPointer() override {
		return this;
	}

	virtual MetaOpResult SerializeIn(AnimationValueSerializeContext* context, u32 version) override {
		context->mStream->serialize_uint32(&mDataSize);
		mDataExternallyOwned = true;
		mpData = context->Allocate(mDataSize);
		context->mStream->serialize_bytes(mpData, mDataSize);
		return eMetaOp_Succeed;
	}

	virtual MetaClassDescription* GetValueClassDescription() override {
		return ::GetMetaClassDescription<SkeletonPose>();
	}

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<CompressedSkeletonPoseKeys2>();
	}

	INLINE void _ReleaseData() {
		if (!mDataExternallyOwned && mpData && mDataSize > 0) {
			delete[] mpData;
			mpData = NULL;
			mDataSize = 0;
			mDataExternallyOwned = false;
		}
	}

	~CompressedSkeletonPoseKeys2() {
		_ReleaseData();
	}

};

struct SkeletonPoseValue : AnimationValueInterfaceBase{

	struct Sample {
		float mTime;
		float mRecipTimeToNextSample;
		DCArray<Transform> mValues;
		DCArray<int> mTangents;
	};

	struct BoneEntry {
		Symbol mName;
		u32 mFlags;
	};

	virtual MetaClassDescription* GetValueClassDescription() override {
		return ::GetMetaClassDescription<SkeletonPose>();
	}

	virtual void* GetMetaClassObjPointer() override {
		return this;
	}

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<SkeletonPoseValue>();
	}

	DCArray<BoneEntry> mBones;
	DCArray<Sample> mSamples;

};

struct CompressedSkeletonPoseKeys : AnimationValueInterfaceBase {

	//format for data header in mpData once serialized in
	struct Header {
		Vector4 mMinDeltaV, mRangeDeltaV, mMinDeltaQ, mRangeDeltaQ, mMinVector, mRangeVector;
		int mBoneCount, mSampleCount, mValueCount, mBlockCount;
	};

	char* mpData;
	u32 mDataSize;
	bool mDataExternallyOwned;

	CompressedSkeletonPoseKeys() {
		mpData = NULL;
		mDataSize = 0;
		mDataExternallyOwned = false;
		mFlags = 0;
	}

	virtual void* GetMetaClassObjPointer() override {
		return this;
	}

	virtual MetaOpResult SerializeIn(AnimationValueSerializeContext* context, u32 version) override {
		context->mStream->serialize_uint32(&mDataSize);
		mDataExternallyOwned = true;
		mpData = context->Allocate((mDataSize + 0x1f) & 0xFFFFFFE0);
		context->mStream->serialize_bytes(mpData, mDataSize);
		return eMetaOp_Succeed;
	}

	virtual MetaClassDescription* GetValueClassDescription() override {
		return ::GetMetaClassDescription<SkeletonPose>();
	}

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<CompressedSkeletonPoseKeys>();
	}

	virtual void SerializeOut(MetaStream* stream) override {
		stream->serialize_uint32(&mDataSize);
		stream->serialize_bytes(mpData, mDataSize);
	}

	~CompressedSkeletonPoseKeys() {
		if (!mDataExternallyOwned && mpData && mDataSize > 0) {
			delete[] mpData;
			mDataExternallyOwned = false;
			mpData = NULL;
			mDataSize = 0;
		}
	}

};

template<typename T>
struct SingleValue : AnimationValueInterfaceBase {

	T mComputedValue;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		SingleValue<T>* pThis = (SingleValue<T>*)pObj;
		MetaClassDescription* pClass = GetMetaClassDescription<T>();
		MetaOperation op = pClass->GetOperationSpecialization(74);
		if (!op)
			op = Meta::MetaOperation_SerializeAsync;
		return op(&pThis->mComputedValue, pClass, 0, pUserData);
	}

};

#endif