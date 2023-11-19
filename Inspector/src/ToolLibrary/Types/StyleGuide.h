// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _STYLE_GUIDE
#define _STYLE_GUIDE

#include "../Meta.hpp"
#include "TRange.h"
#include "DCArray.h"
#include "HandleObjectInfo.h"
#include "PropertySet.h"
#include "TransitionMap.h"
#include "List.h"
#include "AnimOrChore.h"
#include "Map.h"

constexpr Symbol styleIdlesOnKey{ 0x225F4A10E80B37AD };
constexpr Symbol styleBaseIdleKey{ 0x1DB0F9232E38E12F };
constexpr Symbol styleIdleKey1{ 0x16637976BEE9313D };
constexpr Symbol styleGuideKey1{ 0x3A19C2CC84E6A163 };
constexpr Symbol styleIdleKey2{ 0x0E630E104C6B0DA01 };
constexpr Symbol styleGuideKey2{ 0x8BA4000EFA24BABE };
constexpr Symbol styleIdleKey3{ 0x0A4C000EF6F5AEC92 };
constexpr Symbol styleGuideKey3{ 0x0C954E1E553CE8C2D };
constexpr Symbol styleIdleKey4{ 0x2AE244969B265BF8 };
constexpr Symbol styleGuideKey4{ 0x4776A59CA7B23B47 };
constexpr Symbol styleIdleKey5{ 0x6812A57D32CC6D6B };
constexpr Symbol styleGuideKey5{ 0x58644770E580DD4 };
constexpr Symbol styleUser1Key{ 0x0BCE3F22B573AE08F };
constexpr Symbol styleUser2Key{ 0x7BF2D017AD04BB3A };
constexpr Symbol styleUser3Key{ 0x390231FC04EE8DA9 };
constexpr Symbol styleUser4Key{ 0x0B7207585F0923AC3 };
constexpr Symbol styleUser5Key{ 0x0F5D0946E59780C50 };

struct ResourceGroups {
	Map<Symbol, float, Symbol::CompareCRC> mGroups;
};

struct ActingOverridablePropOwner {
	static constexpr u32 kHeader = 0xBEEFF00D;//lmfao
	Flags mSerializationFlags;
	PropertySet* mpOverridableValues;
	Handle<PropertySet> mhParent;

	ActingOverridablePropOwner() : mpOverridableValues(NULL) {
		mSerializationFlags.mFlags = 0;
	}

	ActingOverridablePropOwner(Handle<PropertySet>& pHandle) {
		mhParent = pHandle;
		mpOverridableValues = NULL;
		mSerializationFlags.mFlags = 0;
	}

	~ActingOverridablePropOwner() {
		if (mpOverridableValues)
			delete mpOverridableValues;
	}

	void CreateOverridableValuesPropertySet() {
		if (!this->mpOverridableValues) {
			mpOverridableValues = new PropertySet;
		}
	}

	TRange<float>* GetStartOffsetRange() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kStartOffsetRangeKey, false)) {
			mpOverridableValues->CreateKey(Acting::kStartOffsetRangeKey,
				GetMetaClassDescription(typeid(TRange<float>).name()));
		}
		return mpOverridableValues->GetProperty<TRange<float>>(Acting::kStartOffsetRangeKey);
	}

	TRange<float>* GetIntensityScaleMutliplierRange() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPropertyKeyIntensityScaleMultiplierRange, false)) {
			mpOverridableValues->CreateKey(Acting::kPropertyKeyIntensityScaleMultiplierRange,
				GetMetaClassDescription(typeid(TRange<float>).name()));
		}
		return mpOverridableValues->GetProperty<TRange<float>>
			(Acting::kPropertyKeyIntensityScaleMultiplierRange);
	}

	TRange<float>* GetScaleRange() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kScaleRangeKey, false)) {
			mpOverridableValues->CreateKey(Acting::kScaleRangeKey,
				GetMetaClassDescription(typeid(TRange<float>).name()));
		}
		return mpOverridableValues->GetProperty<TRange<float>>
			(Acting::kScaleRangeKey);
	}

	TRange<float>* GetContributionRange() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kContributionRangeKey, false)) {
			mpOverridableValues->CreateKey(Acting::kContributionRangeKey,
				GetMetaClassDescription(typeid(TRange<float>).name()));
		}
		return mpOverridableValues->GetProperty<TRange<float>>
			(Acting::kContributionRangeKey);
	}

	TRange<float>* GetIntensityTimeBetweenActionsMutliplierRange() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPropertyKeyIntensityTimeBetweenActionsMultiplierRange, false)) {
			mpOverridableValues->CreateKey(Acting::kPropertyKeyIntensityTimeBetweenActionsMultiplierRange,
				GetMetaClassDescription(typeid(TRange<float>).name()));
		}
		return mpOverridableValues->GetProperty<TRange<float>>
			(Acting::kPropertyKeyIntensityTimeBetweenActionsMultiplierRange);
	}

	void SetBlending(EnumeTangentModes mode) {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kBlendingKey, false)) {
			mpOverridableValues->CreateKey(Acting::kBlendingKey,
				GetMetaClassDescription(typeid(EnumeTangentModes).name()));
		}
		*mpOverridableValues->GetProperty<EnumeTangentModes>(Acting::kBlendingKey)
			= mode;
	}

	void SetPriority(int v) {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPriorityKey, false)) {
			mpOverridableValues->CreateKey(Acting::kPriorityKey,
				GetMetaClassDescription(typeid(long).name()));
		}
		*mpOverridableValues->GetProperty<long>(Acting::kPriorityKey)
			= v;
	}

	void SetFadeInOut(float v) {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kFadeTimeKey, false)) {
			mpOverridableValues->CreateKey(Acting::kFadeTimeKey,
				GetMetaClassDescription(typeid(float).name()));
		}
		*mpOverridableValues->GetProperty<float>(Acting::kFadeTimeKey)
			= v;
	}

	void SetBlockOverrunPercent(float v) {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPropertyKeyBlockOverrunPercentage, false)) {
			mpOverridableValues->CreateKey(Acting::kPropertyKeyBlockOverrunPercentage,
				GetMetaClassDescription(typeid(float).name()));
		}
		*mpOverridableValues->GetProperty<float>(Acting::kPropertyKeyBlockOverrunPercentage)
			= v;
	}

	void SetPostDelay(float v) {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPostDelayKey, false)) {
			mpOverridableValues->CreateKey(Acting::kPostDelayKey,
				GetMetaClassDescription(typeid(float).name()));
		}
		*mpOverridableValues->GetProperty<float>(Acting::kPostDelayKey)
			= v;
	}

	void SetPreDelay(float v) {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPreDelayKey, false)) {
			mpOverridableValues->CreateKey(Acting::kPreDelayKey,
				GetMetaClassDescription(typeid(float).name()));
		}
		*mpOverridableValues->GetProperty<float>(Acting::kPreDelayKey)
			= v;
	}

	float GetPostDelay() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPostDelayKey, false)) {
			mpOverridableValues->CreateKey(Acting::kPostDelayKey,
				GetMetaClassDescription(typeid(float).name()));
		}
		return *mpOverridableValues->GetProperty<float>
			(Acting::kPostDelayKey);
	}

	void CreateKeys() {
		CreateOverridableValuesPropertySet();
		mpOverridableValues->CreateKey(Acting::kPriorityKey,
			GetMetaClassDescription(typeid(long).name()));
		mpOverridableValues->CreateKey(Acting::kFadeTimeKey,
			GetMetaClassDescription(typeid(float).name()));
		mpOverridableValues->CreateKey(Acting::kPreDelayKey,
			GetMetaClassDescription(typeid(float).name()));
		mpOverridableValues->CreateKey(Acting::kPostDelayKey,
			GetMetaClassDescription(typeid(float).name()));
		mpOverridableValues->CreateKey(Acting::kScaleRangeKey,
			GetMetaClassDescription(typeid(TRange<float>).name()));
		mpOverridableValues->CreateKey(Acting::kBlendingKey,
			GetMetaClassDescription(typeid(EnumeTangentModes).name()));
		mpOverridableValues->CreateKey(Acting::kContributionRangeKey,
			GetMetaClassDescription(typeid(TRange<float>).name()));
		mpOverridableValues->CreateKey(Acting::kResourceGroupsKey,
			GetMetaClassDescription(typeid(ResourceGroups).name()));
		mpOverridableValues->CreateKey(Acting::kStartOffsetRangeKey,
			GetMetaClassDescription(typeid(TRange<float>).name()));
		mpOverridableValues->CreateKey(Acting::kPropertyKeyBlockOverrunPercentage,
			GetMetaClassDescription(typeid(float).name()));
		mpOverridableValues->CreateKey(Acting::kStartOffsetRangeKey,
			GetMetaClassDescription(typeid(TRange<float>).name()));
		mpOverridableValues->CreateKey(Acting::kPropertyKeyIntensityTimeBetweenActionsMultiplierRange,
			GetMetaClassDescription(typeid(TRange<float>).name()));
		mpOverridableValues->CreateKey(Acting::kPropertyKeyIntensityContributionMultiplierRange,
			GetMetaClassDescription(typeid(TRange<float>).name()));
		mpOverridableValues->CreateKey(Acting::kPropertyKeyIntensityScaleMultiplierRange,
			GetMetaClassDescription(typeid(TRange<float>).name()));
	}

	int GetPriority() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPriorityKey, false)) {
			mpOverridableValues->CreateKey(Acting::kPriorityKey,
				GetMetaClassDescription(typeid(long).name()));
		}
		return *mpOverridableValues->GetProperty<long>
			(Acting::kPriorityKey);
	}

	float GetBlockOverrunPercent() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPropertyKeyBlockOverrunPercentage, false)) {
			mpOverridableValues->CreateKey(Acting::kPropertyKeyBlockOverrunPercentage,
				GetMetaClassDescription(typeid(float).name()));
		}
		return *mpOverridableValues->GetProperty<float>
			(Acting::kPropertyKeyBlockOverrunPercentage);
	}

	float GetPreDelay() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPreDelayKey, false)) {
			mpOverridableValues->CreateKey(Acting::kPreDelayKey,
				GetMetaClassDescription(typeid(float).name()));
		}
		return *mpOverridableValues->GetProperty<float>
			(Acting::kPreDelayKey);
	}

	EnumeTangentModes GetBlending() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kBlendingKey, false)) {
			mpOverridableValues->CreateKey(Acting::kBlendingKey,
				GetMetaClassDescription(typeid(EnumeTangentModes).name()));
		}
		return *mpOverridableValues->GetProperty<EnumeTangentModes>(Acting::kBlendingKey);
	}

	TRange<float>* GetIntensityContributionMutliplierRange() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kPropertyKeyIntensityContributionMultiplierRange, false)) {
			mpOverridableValues->CreateKey(Acting::kPropertyKeyIntensityContributionMultiplierRange,
				GetMetaClassDescription(typeid(TRange<float>).name()));
		}
		return mpOverridableValues->GetProperty<TRange<float>>
			(Acting::kPropertyKeyIntensityContributionMultiplierRange);
	}

	ResourceGroups* GetResourceGroups() {
		CreateOverridableValuesPropertySet();
		if (!mpOverridableValues->ExistsKey(Acting::kResourceGroupsKey, false)) {
			mpOverridableValues->CreateKey(Acting::kResourceGroupsKey,
				GetMetaClassDescription(typeid(ResourceGroups).name()));
		}
		return mpOverridableValues->GetProperty<ResourceGroups>
			(Acting::kResourceGroupsKey);
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {//this function is overly stupid
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		ActingOverridablePropOwner* data = static_cast<ActingOverridablePropOwner*>(pObj);
		u32 header = kHeader;//header which cant be a prop version
		data->mSerializationFlags.mFlags = 0;
		if (!data->mpOverridableValues || !data->mpOverridableValues->GetNumKeys())
			data->mSerializationFlags.mFlags |= 1u;
		u64 pos = meta->GetPos();
		MetaClassDescription* clazz = TelltaleToolLib_FindMetaClassDescription("long", true);
		if (!clazz)
			return eMetaOp_Fail;
		PerformMetaSerializeFull(meta, &header, clazz);
		if (kHeader == header) {
			//does not call this function calls meta:: . reads/writes flags
			MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj,
				pObjDescription, pContextDescription, pUserData);
			if (r != eMetaOp_Succeed)
				return r;
		}
		else if (meta->mMode == MetaStreamMode::eMetaStream_Read) {
			meta->SetPos(pos);
		}
		if (!(data->mSerializationFlags.mFlags & 1) || header != kHeader) {
			if (meta->mMode == MetaStreamMode::eMetaStream_Read)
				data->CreateOverridableValuesPropertySet();
			PerformMetaSerializeAsync(meta, data->mpOverridableValues);
		}
		return eMetaOp_Succeed;
	}

};

struct ActingResource : public ActingOverridablePropOwner {
	AnimOrChore mResource;
	Flags mRuntimeFlags;
	TRange<float> mValidIntensityRange;

	ActingResource() {
		mValidIntensityRange.max = 2;
	}

};

struct ActingResourceOwner : public ActingOverridablePropOwner { };

struct ActingPalette : public ActingResourceOwner, public UID::Owner {

	enum PaletteFlags {
		eResOnHeapVer = 1
	};

	enum ActiveDuring {
		always = 1,
		talking = 2,
		listening = 3
	};

	struct EnumActiveDuring {
		ActiveDuring mVal;
	};

	enum EndRelativeTo {
		beginning = 1,
		end = 2,
		transition = 3
	};

	struct EnumEndRelativeTo {
		EndRelativeTo mVal;
	};

	enum Overrun {
		disallowed = 0,
		allowed = 1
	};

	struct EnumOverrun {
		Overrun mVal;
	};

	String mName;
	EnumActiveDuring mActiveDuring;
	TRange<float> mTimeBetweenActions;
	TRange<float> mFirstActionDelayRange;
	TRange<float> mSpilloutBufPreRange;
	TRange<float> mSpilloutBufPostRange;
	TRange<float> mLatestStartOffsetRange;
	TRange<float> mValidIntensityRange;
	DCArray<ActingResource*> mResourcePtrs;
	long mGroupMembershipUID;
	Flags mFlags;
	EnumEndRelativeTo mFirstActionEndRel, mEndOffsetRel, mLatestStartOffsetRel;
	EnumOverrun mOverrunAllowed, mMoodOverrunAllowed;
	bool mDisableAct;
	long mJunket;

	ActingPalette() {
		mResourcePtrs = DCArray<ActingResource*>();
	}

	~ActingPalette() {
		for (int i = 0; i < mResourcePtrs.GetSize(); i++)
			delete mResourcePtrs[i];
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		ActingPalette* data = static_cast<ActingPalette*>(pObj);
		if (meta->mMode == MetaStreamMode::eMetaStream_Write)
			data->mFlags |= 1;
		MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription,
			pContextDescription, pUserData);
		if (result != eMetaOp_Succeed)
			return result;
		u32 numresources = data->mResourcePtrs.GetSize();
		meta->serialize_uint32(&numresources);
		if (meta->mMode == MetaStreamMode::eMetaStream_Read) {
			for (int i = 0; i < data->mResourcePtrs.GetSize(); i++) {
				delete data->mResourcePtrs[i];
			}
			data->mResourcePtrs.Clear(0);
			data->mResourcePtrs.ReserveAndResize(numresources);
			for (int i = 0; i < numresources; i++) {
				ActingResource* resource = *(data->mResourcePtrs.mpStorage + i);
				if (!resource) {
					resource = new ActingResource;
					data->mResourcePtrs.mpStorage[i] = resource;
				}
				PerformMetaSerializeAsync(meta, resource);
			}
			if (data->mFlags.mFlags & 2)
				data->mFlags |= 1;
			return eMetaOp_Succeed;
		}
		if (meta->mMode != MetaStreamMode::eMetaStream_Write)
			return eMetaOp_Succeed;
		//some sort of debug section next but in the default sect
		//just serializes ontop of above
		//only read in debug mode of the engine or something
		//think it reads to some global variable? anyway not needed here
		return result;
	}

};

struct ActingAccentPalette : public ActingResourceOwner, public UID::Owner {

	enum Overrun {
		disallowed = 0,
		allowed = 1
	};

	struct EnumOverrun {
		Overrun mVal;
	};

	String mName;
	TRange<float> mStartOffsetRange;
	EnumOverrun mMoodOverrunAllowed;
	bool mDisableAct;
	TRange<float> mValidIntensityRange;
	TRange<float> mSpilloutBufPostRange;
	float mRandomChance;
	long mTrackID;//ENUM: 1= Body, 2= Face, 3 = Head 1, 4 = Head 2
	DCArray<ActingResource*> mResourcePtrs;
	long mGroupMembershipUID;
	Flags mFlags;
	long mVersion;

	ActingAccentPalette() {
		mResourcePtrs = DCArray<ActingResource*>();
	}

	~ActingAccentPalette() {
		for (int i = 0; i < mResourcePtrs.GetSize(); i++)
			delete mResourcePtrs[i];
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		ActingAccentPalette* data = static_cast<ActingAccentPalette*>(pObj);
		MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj,
			pObjDescription, pContextDescription, pUserData);
		if (result != eMetaOp_Succeed)
			return result;
		u32 numresources = data->mResourcePtrs.GetSize();
		meta->serialize_uint32(&numresources);
		if (meta->mMode == MetaStreamMode::eMetaStream_Read) {
			for (int i = 0; i < data->mResourcePtrs.GetSize(); i++) {
				delete data->mResourcePtrs[i];
			}
			data->mResourcePtrs.Clear(0);
			data->mResourcePtrs.ReserveAndResize(numresources);
			for (int i = 0; i < numresources; i++) {
				ActingResource* resource = *(data->mResourcePtrs.mpStorage + i);
				if (!resource) {
					resource = new ActingResource;
					data->mResourcePtrs.mpStorage[i] = resource;
				}
				PerformMetaSerializeAsync(meta, resource);
			}
			if (data->mFlags.mFlags & 2)
				data->mFlags |= 1;
			return eMetaOp_Succeed;
		}
		if (meta->mMode != MetaStreamMode::eMetaStream_Write)
			return eMetaOp_Succeed;
		//some sort of debug section next but in the default sect
		//just serializes ontop of above
		//only read in debug mode of the engine or something
		//think it reads to some global variable? anyway not needed here
		return result;
	}

};

struct ActingPaletteGroup : public UID::Owner {

	struct ActingPaletteTransition {
		String mTransition;
		AnimOrChore mTransitionIn;
		float mCenterOffset;
		float mPreDelay;
		float mPostDelay;
		float mFadeTime;
	};

	enum IdleTransition {
		transitionLinear = 1,
		transitionEaseInOut = 2,
		transitionUnused = 3
	};

	struct EnumIdleTransition {
		IdleTransition mVal;
	};

	String mName;
	AnimOrChore mIdle;
	AnimOrChore mTalkingIdle;
	AnimOrChore mMumbleMouth;
	float mWeight;
	AnimOrChore mTransitionIn;
	AnimOrChore mTransitionOut;
	List<ActingPaletteTransition> mTransitions;
	float mIdleTransitionTimeOverride;
	Handle<TransitionMap> mhIdleTransitionMap;
	EnumIdleTransition mIdleTransitionKind;
	float mRandomAutoMin;
	float mRandomAutoMax;

	ActingPaletteGroup() {
		mName = String("New Palette Group");
		mWeight = 1.0f;
		mIdleTransitionTimeOverride = -1.0f;
		mIdleTransitionKind.mVal = IdleTransition::transitionLinear;
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		//MetaStream* meta = static_cast<MetaStream*>(pUserData);
		//ActingPaletteGroup* data = static_cast<ActingPaletteGroup*>(pObj);
		//delegating serialization functions like this would probably have debug
		//code for non release modes
		return Meta::MetaOperation_SerializeAsync(pObj, pObjDescription,
			pContextDescription, pUserData);
	}

};

struct ActingPaletteClass : public UID::Generator, public UID::Owner, public ActingOverridablePropOwner {
	String mName;
	DCArray<ActingPalette*> mPalettePtrs;
	DCArray<ActingAccentPalette*> mAccentPalettePtrs;
	DCArray<ActingPaletteGroup*> mPaletteGroupPtrs;
	DCArray<String> mAlternateNames;
	long mDefaultPaletteGroupID;
	Flags mFlags;
	bool mInstantChange;

	ActingPaletteGroup* AddPaletteGroup() {
		ActingPaletteGroup* palette = new ActingPaletteGroup;
		palette->miUniqueID = GetNextUniqueID(true);
		mPaletteGroupPtrs.AddElement(0, NULL, &palette);
		return palette;
	}

	ActingAccentPalette* AddAccentPalette() {
		ActingAccentPalette* palette = new ActingAccentPalette;
		palette->miUniqueID = GetNextUniqueID(true);
		mAccentPalettePtrs.AddElement(0, NULL, &palette);
		return palette;
	}

	ActingPalette* AddPalette() {
		ActingPalette* palette = new ActingPalette;
		palette->miUniqueID = GetNextUniqueID(true);
		mPalettePtrs.AddElement(0, NULL, &palette);
		return palette;
	}

	~ActingPaletteClass() {
		for (int i = 0; i < mPalettePtrs.GetSize(); i++) {
			delete mPalettePtrs[i];
		}
		for (int i = 0; i < mAccentPalettePtrs.GetSize(); i++) {
			delete mAccentPalettePtrs[i];
		}
		for (int i = 0; i < mPaletteGroupPtrs.GetSize(); i++) {
			delete mPaletteGroupPtrs[i];
		}
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		ActingPaletteClass* data = static_cast<ActingPaletteClass*>(pObj);
		MetaStreamMode mode = meta->mMode;
		if (mode == MetaStreamMode::eMetaStream_Write
			&& data->mAccentPalettePtrs.GetSize()) {
			data->mFlags |= 3;
		}
		MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj,
			pObjDescription, pContextDescription, pUserData);
		if (result != eMetaOp_Succeed)
			return result;
		data->mFlags |= 1;
		u32 num = 0;
		if (mode != MetaStreamMode::eMetaStream_Closed) {
			num = data->mPalettePtrs.GetSize();
			meta->serialize_uint32(&num);
			if (mode == MetaStreamMode::eMetaStream_Read) {
				UID::idT id = data->GetNextUniqueID(true);
				for (int i = 0; i < num; i++) {
					MetaOpResult r = PerformMetaSerializeAsync(meta, data->AddPalette());
					if (r != eMetaOp_Succeed)
						return r;
				}
				data->miUniqueID = id;
			}
			else {
				for (int i = 0; i < num; i++) {
					MetaOpResult r = PerformMetaSerializeAsync(meta, data->mPalettePtrs.operator[](i));
					if (r != eMetaOp_Succeed)
						return r;
				}
			}
			if (data->mFlags.mFlags & 0b10) {
				num = data->mAccentPalettePtrs.GetSize();
				meta->serialize_uint32(&num);
				if (mode == MetaStreamMode::eMetaStream_Read) {
					UID::idT id = data->GetNextUniqueID(true);
					for (int i = 0; i < num; i++) {
						MetaOpResult r = PerformMetaSerializeAsync(meta, data->AddAccentPalette());
						if (r != eMetaOp_Succeed)
							return r;
					}
					data->miUniqueID = id;
				}
				else {
					for (int i = 0; i < num; i++) {
						MetaOpResult r = PerformMetaSerializeAsync(meta, data->mAccentPalettePtrs.operator[](i));
						if (r != eMetaOp_Succeed)
							return r;
					}
				}
			}
			num = data->mPaletteGroupPtrs.GetSize();
			meta->serialize_uint32(&num);
			if (mode == MetaStreamMode::eMetaStream_Read) {
				UID::idT id = data->GetNextUniqueID(true);
				for (int i = 0; i < num; i++) {
					MetaOpResult r = PerformMetaSerializeAsync(meta, data->AddPaletteGroup());
					if (r != eMetaOp_Succeed)
						return r;
				}
				data->miUniqueID = id;
			}
			else {
				for (int i = 0; i < num; i++) {
					MetaOpResult r = PerformMetaSerializeAsync(meta, data->mPaletteGroupPtrs.operator[](i));
					if (r != eMetaOp_Succeed)
						return r;
				}
			}
			//not needed
		}
		return eMetaOp_Succeed;
	}
};

constexpr const char* strUserClassName = "__uSeR_pAl_cLaSS___";

//.STYLE FILES
struct StyleGuide : public UID::Generator, public ActingOverridablePropOwner {
	long mDefPaletteClassID;
	bool mbGeneratesLookAts;
	DCArray<ActingPaletteClass*> mPaletteClassPtrs;
	Flags mFlags;
	DCArray<ActingPaletteClass> mPaletteClasses;
	long mDefPaletteClassIndex;

	StyleGuide() {
		mDefPaletteClassID = UID::Generator::msUninitID;
		mDefPaletteClassIndex = -1;
		mFlags.mFlags = 0;
		mbGeneratesLookAts = true;
	}

	~StyleGuide() {
		for (int i = 0; i < mPaletteClassPtrs.GetSize(); i++)
			delete mPaletteClassPtrs[i];
	}

	typedef ActingPalette::PaletteFlags StyleGuideFlags;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		StyleGuide* data = static_cast<StyleGuide*>(pObj);
		MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription,
			pContextDescription, pUserData);
		if (result != MetaOpResult::eMetaOp_Succeed)
			return result;
		u32 num = data->mPaletteClassPtrs.GetSize();
		meta->serialize_uint32(&num);
		if (meta->mMode == MetaStreamMode::eMetaStream_Read) {
			for (int i = 0; i < num; i++) {
				result = PerformMetaSerializeAsync(meta, data->AddPaletteClass());
				if (result != MetaOpResult::eMetaOp_Succeed)
					return result;
			}
		}
		else if (meta->mMode == MetaStreamMode::eMetaStream_Write) {
			for (int i = 0; i < num; i++) {
				result = PerformMetaSerializeAsync(meta, data->mPaletteClassPtrs[i]);
				if (result != MetaOpResult::eMetaOp_Succeed)
					return result;
			}
		}
		else return eMetaOp_Invalid;
		return result;
	}

	ActingPaletteClass* AddPaletteClass() {
		ActingPaletteClass* clazz = new ActingPaletteClass;
		clazz->miUniqueID = GetNextUniqueID(true);
		this->mPaletteClassPtrs.AddElement(0, NULL, &clazz);
		return clazz;
	}

};

struct StyleGuideRef {
	Handle<StyleGuide> mhStyleGuide;
	int mPaletteClassUID;
	int mPaletteClassGroupUID;
	bool mOverridden;
	int mPaletteClassIndex;
	DCArray<bool> mPalettesUsed;
};

#endif