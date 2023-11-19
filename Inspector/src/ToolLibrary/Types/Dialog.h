// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _DLG
#define _DLG

#include "../Meta.hpp"
#include "LanguageDB.h"
#include "PropertySet.h"
#include "Set.h"
#include "Map.h"
#include "T3Texture.h"
#include "HandleObjectInfo.h"
#include "Chore.h"
#include "Rules.h"

//.DLOG FILES
struct Dlg;

//SEE https://www.atlassian.com/software/jira - telltale API for using it
struct JiraRecord {
	//contains runtime data, not needed
	size_t _empty;
};

//SEE https://www.atlassian.com/software/jira - telltale API for using it
struct JiraRecordManager {
	Map<String, JiraRecord*, std::less<String>> mRecords;

	void _DeleteData() {
		for (int i = 0; i < mRecords.GetSize(); i++) {
			delete mRecords[i].second;
		}
		mRecords.ClearElements();
	}

	~JiraRecordManager() {
		_DeleteData();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(JiraRecordManager, jira);
		if (meta->BeginDebugSection()) {
			if (meta->IsRead())
				jira->_DeleteData();
			u32 num = jira->mRecords.GetSize();
			meta->serialize_uint32(&num);
			if (meta->IsRead()) {
				String str{};
				JiraRecord *record = new JiraRecord;
				for (int i = 0; i < num; i++) {
					meta->serialize_String(&str);
					jira->mRecords.AddElement(NULL, &str, &record);
				}
			}
			else {
				String p;
				for (int i = 0; i < num; i++) {
					p = jira->mRecords[i].first;
					meta->serialize_String(&p);
				}
			}
			meta->EndDebugSection();
		}
		return eMetaOp_Succeed;
	}

};

struct DlgObjID {
	Symbol mID;

	inline bool operator<(const DlgObjID& rhs) const {
		return mID < rhs.mID;
	}
};

struct DlgObjIDLess {

	bool operator()(const DlgObjID& lhs, const DlgObjID& rhs) const {
		return lhs.mID < rhs.mID;
	}

};

struct DlgObjIDOwner {

	DlgObjID mDlgObjID;

};


struct DlgObjectProps {

	enum PropsTypeT : u32 {
		eUserProps = 1,
		eProductionProps = 2,
		eToolProps = 4
	};

	PropertySet* mpUserProps, *mpProductionProps, *mpToolProps;
	Flags mFlags;//PropsTypeT, makes sense to be an enum oh well

	PropertySet* PropsByType(PropsTypeT pType) {
		if (pType == eUserProps) {
			return mpUserProps ? mpUserProps : mpUserProps = new PropertySet;
		}
		else if (pType == eProductionProps) {
			return mpProductionProps ? mpProductionProps : mpProductionProps = new PropertySet;
		}
		else {
			return mpToolProps ? mpToolProps : mpToolProps = new PropertySet;
		}
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(DlgObjectProps, obj);
		if (obj->mpUserProps)
			obj->mFlags |= PropsTypeT::eUserProps;
		if (obj->mpProductionProps)
			obj->mFlags |= PropsTypeT::eProductionProps;
		if (obj->mpToolProps)
			obj->mFlags |= PropsTypeT::eToolProps;
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			if (meta->IsRead()) {
				u32 flags = obj->mFlags.mFlags;
				obj->_DeleteData();
				obj->mFlags = flags;
			}
			if (obj->mFlags.mFlags & eUserProps) {
				if (!obj->mpUserProps)
					obj->mpUserProps = new PropertySet();
				r = PerformMetaSerializeAsync<PropertySet>(meta, obj->mpUserProps);
				if (r != eMetaOp_Succeed)
					return r;
			}
			if (obj->mFlags.mFlags & eProductionProps) {
				if (!obj->mpProductionProps)
					obj->mpProductionProps = new PropertySet();
				r = PerformMetaSerializeAsync<PropertySet>(meta, obj->mpProductionProps);
				if (r != eMetaOp_Succeed)
					return r;
			}
			if (obj->mFlags.mFlags & eToolProps) {
				if (!obj->mpToolProps)
					obj->mpToolProps = new PropertySet();
				r = PerformMetaSerializeAsync<PropertySet>(meta, obj->mpToolProps);
			}
		}
		return r;
	}

	DlgObjectProps() {
		mpUserProps = NULL;
		mpProductionProps = NULL;
		mpToolProps = NULL;
		mFlags.mFlags = 0;
	}

	void _DeleteData() {
		if (mpUserProps)
			delete mpUserProps;
		if (mpProductionProps)
			delete mpProductionProps;
		if (mpToolProps)
			delete mpToolProps;
		mpUserProps = NULL;
		mpProductionProps = NULL;
		mpToolProps = NULL;
		mFlags.mFlags = 0;
	}

	~DlgObjectProps() {
		_DeleteData();
	}

};

struct DlgObjectPropsOwner {
	DlgObjectProps mDlgObjectProps;
};

namespace DlgConstants {
	enum ChainContextTypeID {
		eCCUnspecified = 1,
		eCCAction = 2,
		eCCData = 3
	};
	enum DlgNodeClassID {
		eNodeClassStart = 0,
		eNodeChoices = 1,
		eNodeConditional = 2,
		eNodeChore = 3,
		eNodeExchange = 4,
		eNodeExit = 5,
		eNodeIdle = 6,
		eNodeJump = 7,
		eNodeLogic = 8,
		eNodeScript = 9,
		eNodeSequence = 10,
		eNodeStart = 11,
		eNodeText = 12,
		eNodeWait = 13,
		eNodeNotes = 14,
		eNodeChancelChoices = 15,
		eNodeParallel = 0x10,
		eNodeMarker = 0x11,
		eNodeStoryboard = 0x12,
		eNodeStats = 0x13,
		eNodeClassLast = 0x14,
		eNodeClassInvalid = 0x15,
		eNodeClassAll = 0x16,
		eChildChoices = 0x17,
		eChildConditional = 0x18
	};
	enum DlgChildClassID {
		eInvalid = 0xFFFFFFFF,
		eChildClassStart = 0x63,
		eChildClassChoice = 0x64,
		eChildClassChoicesChildPre = 0x65,
		eChildClassChoicesChildPost = 0x66,
		eChildClassElement = 0x67,
		eChildClassCase = 0x68,
		eChildClassFolderChild = 0x69,
		eChildClassPElement = 0x6A,
		eChildClassCohort = 0x6B,
		eChildClassLast = 0x6C
	};
	//enum DlgClassAIID {
		//typedefd! D: cant see values
	//};
	enum DlgObjectClassFlag {
		eObjClassInvalid = 0,
		eObjClassDialog = 1,
		eObjClassFolder = 2,
		eObjClassNodeChoices = 4,
		eObjClassNodeConditional = 8,
		eObjClassNodeChore = 16,
		eObjClassNodeExchange = 32,
		eObjClassNodeExit = 64,
		eObjClassNodeIdle = 128,
		eObjClassNodeJump = 256,
		eObjClassNodeLogic = 512,
		eObjClassNodeScript = 1024,
		eObjClassNodeSequence = 2048,
		eObjClassNodeStart = 4096,
		eObjClassNodeText = 8192,
		eObjClassNodeWait = 16384,
		eObjClassNodeNotes = 0x8000,
		eObjClassNodeChancelChoices = 0x10000,
		eObjClassNodeParallel = 0x20000,
		eObjClassNodeMarker = 0x40000,
		eObjClassNodeStoryboard = 0x80000,
		eObjClassNodeStats = 0x100000,
		eObjClassChildChoices = 0x200000,
		eObjClassChildChoicesPre = 0x400000,
		eObjClassChildChoicesPost = 0x800000,
		eObjClassChildSequence = 0x1000000,
		eObjClassChildConditional = 0x2000000,
		eObjClassChildFolder = 0x4000000,
		eObjClassChildParallel = 0x8000000,
		eObjClassChildStats = 0x10000000
	};
}

struct DlgNodeLink : DlgObjIDOwner {//no UID
	DlgConstants::ChainContextTypeID mRequiredCCType;//chain context type
};

struct DlgChainHead : DlgObjIDOwner {
	DlgNodeLink mLink;
};

struct DlgDownstreamVisibilityConditions {
	Flags mNodeTypeFlags;
	long mMaxNumNodeEvals;
};

struct DlgVisibilityConditions {
	Rule* mpRule;
	bool mbDiesOff;
	Flags mFlags;//1 means has a rule
	DlgDownstreamVisibilityConditions mDownstreamVisCond;
	String mScriptVisCond;//script function (LUA)

	DlgVisibilityConditions() {
		mpRule = NULL;
	}

	INLINE void CreateRule() {
		if (!mpRule)
			mpRule = new Rule;
		mFlags |= 1;
	}

	INLINE void RemoveRule() {
		if (mpRule)
			delete mpRule;
		mFlags &= ~1;
	}

	INLINE Rule* GetVisRule() {
		CreateRule();
		return mpRule;
	}

	~DlgVisibilityConditions() {
		RemoveRule();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(DlgVisibilityConditions, cond);
		if (meta->IsWrite()) {
			if (cond->mFlags.mFlags & 1 && !cond->mpRule) {
				cond->mpRule = new Rule;
			}
			else if(cond->mpRule && !(cond->mFlags.mFlags & 1)){
				cond->mFlags.mFlags |= 1;
			}
		}
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r != eMetaOp_Succeed)
			return r;
		if (cond->mFlags.mFlags & 1)
			r = PerformMetaSerializeAsync<Rule>(meta, cond->GetVisRule());
		return r;
	}

};

struct DlgVisibilityConditionsOwner {
	DlgVisibilityConditions mVisCond;
};

//struct DlgStatePropKeyOwner { //state not serialized, dlgnode originally implements
//	Map<int, Symbol, std::less<int>> mPropKeys;
//	virtual ~DlgStatePropKeyOwner() {}
//};

struct DlgChild : DlgChainHead, DlgVisibilityConditionsOwner, DlgObjectPropsOwner{
	Symbol mName;
	DlgNodeLink mParent;
	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgChild>();
	}
	virtual DlgConstants::DlgChildClassID GetTypeID() {
		return DlgConstants::DlgChildClassID::eInvalid;
	}
};

struct DlgChildSet {
	DCArray<DlgChild*> mChildren;
	//not serialized
	DlgNodeLink mParent;

	INLINE void _DeleteData() {
		for (int i = 0; i < mChildren.GetSize(); i++) {
			delete mChildren[i];
		}
		mChildren.ClearElements();
	}

	~DlgChildSet() {
		_DeleteData();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(DlgChildSet, set);
		u32 num = set->mChildren.GetSize();
		if (meta->IsRead())
			set->_DeleteData();
		meta->serialize_uint32(&num);
		u64 crc;
		MetaClassDescription* d;
		MetaClassDescription* childDesc = ::GetMetaClassDescription<DlgChild>();
		if (!childDesc) {
			TelltaleToolLib_RaiseError("Cannot serialize dialog child set "
				"until meta has been initialized (no DlgChild type)", ErrorSeverity::ERR);
			return eMetaOp_Fail;
		}
		for (int i = 0; i < num; i++) {
			if (meta->IsWrite()) {
				DlgChild* child = set->mChildren[i];
				crc = child->GetMetaClassDescription()->mHash;
				meta->serialize_uint64(&crc);
				MetaOpResult r = PerformMetaSerializeFull(meta, child, child->GetMetaClassDescription());
				if (r != eMetaOp_Succeed)
					return r;
			}
			else {
				meta->serialize_uint64(&crc);
				d = TelltaleToolLib_FindMetaClassDescription_ByHash(crc);
				if (!d) {
#ifdef DEBUGMODE
					printf("Not found [dialog child]: %llX\n", crc);
#endif
					TelltaleToolLib_RaiseError("Dialog child set contains unknown child type", ErrorSeverity::ERR);
					return eMetaOp_Fail;
				}
				void* inst = d->New();
				if (!inst)
					return eMetaOp_OutOfMemory;
				MetaOpResult r = PerformMetaSerializeFull(meta, inst, d);
				if (r != eMetaOp_Succeed)
					return r;
				DlgChild* base = (DlgChild*)d->CastToBase(inst, childDesc);
				set->mChildren.AddElement(0, NULL, &base);
			}
		}
		return eMetaOp_Succeed;
	}

	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgChildSet>();
	}

	virtual MetaClassDescription* GetChildDesc() {
		return ::GetMetaClassDescription<DlgChild>();
	}

};

struct DlgFolder : DlgObjIDOwner, DlgObjectPropsOwner, DlgChildSet, UID::Owner {//UIDs removed from WD4

	Symbol mName;
	PropertySet mProdReportProps;//production report properties

	MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgFolder>();
	}

};

struct DlgNode : DlgObjIDOwner, DlgObjectPropsOwner, 
	DlgVisibilityConditionsOwner, UID::Owner {

	DlgNodeLink mPrev, mNext;
	Symbol mName;
	Flags mFlags;
	DlgConstants::ChainContextTypeID mChainContextTypeID;

	virtual ~DlgNode() {}

	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgNode>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() {
		return DlgConstants::DlgNodeClassID::eNodeClassInvalid;
	}

};

struct DlgNodeStart : DlgNode {

	PropertySet mProdReportProps;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeStart>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeStart;
	}

};

struct DlgNodeCancelChoices : DlgNode {

	enum CancelGroupT {
		eAllActiveChoices = 1,
	};

	CancelGroupT mCancelGroup;

	DlgNodeCancelChoices() {
		mCancelGroup = CancelGroupT::eAllActiveChoices;
	}

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeCancelChoices>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeChancelChoices;
	}

};
struct DlgCondition : DlgObjIDOwner{
	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgCondition>();
	}
};

struct DlgConditionSet {
	DCArray<DlgCondition*> mConditions;
	virtual ~DlgConditionSet() {
		for (int i = 0; i < mConditions.GetSize(); i++)
			delete mConditions[i];
		mConditions.ClearElements();
	}
	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(DlgConditionSet, set);
		u32 num = set->mConditions.GetSize();
		meta->serialize_uint32(&num);
		u64 crc;
		MetaClassDescription* mcd;
		MetaOpResult r;
		MetaClassDescription* condDesc = ::GetMetaClassDescription<DlgCondition>();
		if (!condDesc) {
			TelltaleToolLib_RaiseError("Cannot serialize dialog condition, "
				"meta not initialized: could not "
				"find condition metaclass", ErrorSeverity::ERR);
			return eMetaOp_Fail;
		}
		for (int i = 0; i < num; i++) {
			if (meta->IsWrite()) {
				mcd = set->mConditions[i]->GetMetaClassDescription();
				crc = mcd->mHash;
				meta->serialize_uint64(&crc);
				r = PerformMetaSerializeFull(meta, set->mConditions[i], mcd);
				if (r != eMetaOp_Succeed)
					return r;
			}else{
				meta->serialize_uint64(&crc);
				mcd = TelltaleToolLib_FindMetaClassDescription_ByHash(crc);
				if (!mcd) {
#ifdef DEBUGMODE
					printf("Could not find dialog condition subclass %llX\n", crc);
#endif
					TelltaleToolLib_RaiseError(
						"Dialog condition subclass not found", ErrorSeverity::ERR);
					return eMetaOp_Fail;
				}
				void* inst = mcd->New();
				if (!inst)
					return eMetaOp_OutOfMemory;
				DlgCondition* cond = (DlgCondition*)mcd->CastToBase(inst, condDesc);
				r = PerformMetaSerializeFull(meta, inst, mcd);
				if (r != eMetaOp_Succeed)
					return r;
				set->mConditions.AddElement(0, NULL, &cond);
			}
		}
		return eMetaOp_Succeed;
	}
};

struct DlgConditionTime : DlgCondition {

	enum DurationClass : u32 {
		eTimed = 1,
		eIndefinitely = 2
	};

	struct EnumDurationClass {
		DurationClass mVal;
	};

	float mSeconds;
	EnumDurationClass mDurationClass;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgConditionTime>();
	}
};

struct DlgConditionRule : DlgCondition {

	Rule mRule;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgConditionRule>();
	}
};

struct DlgConditionInput : DlgCondition {
	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgConditionInput>();
	}
};

struct DlgChoice : DlgChild, DlgConditionSet {
	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgChoice>();
	}
	virtual DlgConstants::DlgChildClassID GetTypeID() override {
		return DlgConstants::DlgChildClassID::eChildClassChoice;
	}
};

struct DlgChildSetChoice : DlgChildSet {
	virtual MetaClassDescription* GetChildDesc() override {
		return ::GetMetaClassDescription<DlgChoice>();
	}
	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgChildSetChoice>();
	}
};

struct DlgChoicesChildPre : DlgChild {
	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgChoicesChildPre>();
	}
	virtual DlgConstants::DlgChildClassID GetTypeID() override {
		return DlgConstants::DlgChildClassID::eChildClassChoicesChildPre;
	}
};

struct DlgChoicesChildPost : DlgChild {
	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgChoicesChildPost>();
	}
	virtual DlgConstants::DlgChildClassID GetTypeID() override {
		return DlgConstants::DlgChildClassID::eChildClassChoicesChildPost;
	}
};

struct DlgChildSetChoicesChildPre : DlgChildSet {
	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgChildSetChoicesChildPre>();
	}
	virtual MetaClassDescription* GetChildDesc() override {
		return ::GetMetaClassDescription<DlgChoicesChildPre>();
	}
};

struct DlgChildSetChoicesChildPost : DlgChildSet {
	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgChildSetChoicesChildPost>();
	}
	virtual MetaClassDescription* GetChildDesc() override {
		return ::GetMetaClassDescription<DlgChoicesChildPost>();
	}
};

struct DlgConditionalCase : DlgChild {
	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgConditionalCase>();
	}
	virtual DlgConstants::DlgChildClassID GetTypeID() override {
		return DlgConstants::DlgChildClassID::eChildClassCase;
	}
};

struct DlgChildSetConditionalCase : DlgChildSet {
	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgChildSetConditionalCase>();
	}
	virtual MetaClassDescription* GetChildDesc() override {
		return ::GetMetaClassDescription<DlgConditionalCase>();
	}
};

struct DateStamp {
	u8 mSec, mMin, mHour, mMday, mMon, mYear, mWday, mIsdst;//direct 
	u16 mYday;//365>2^8-1
};

struct Note : UID::Generator, UID::Owner {

	struct Entry : UID::Owner, DlgObjIDOwner {
		String mAuthor;
		DateStamp mStamp;
		String mCategory;
		String mText;
	};

	DCArray<Entry*> mEntries;
	String mName;

	INLINE void _DeleteData() {
		for (int i = 0; i < mEntries.GetSize(); i++)
			delete mEntries[i];
		mEntries.ClearElements();
	}

	~Note() {
		_DeleteData();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(Note, note);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			if (meta->IsRead())
				note->_DeleteData();
			u32 num = note->mEntries.GetSize();
			meta->serialize_uint32(&num);
			MetaClassDescription* entryDesc = ::GetMetaClassDescription<Entry>();
			if (!entryDesc) {
				TelltaleToolLib_RaiseError("Meta not initialized to serialize note entry", ErrorSeverity::ERR);
				return eMetaOp_Fail;
			}
			for (int i = 0; i < num; i++) {
				if (meta->IsRead()) {
					Entry* e = new Entry;
					r=PerformMetaSerializeFull(meta, e, entryDesc);
					if (r != eMetaOp_Succeed)
						return r;
					note->mEntries.AddElement(0, NULL, &e);
				}
				else {
					r = PerformMetaSerializeFull(meta, note->mEntries.mpStorage[i], entryDesc);
					if (r != eMetaOp_Succeed)
						return r;
				}
			}
		}
		return r;
	}

};

struct NoteCollection : UID::Generator{
	MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription <NoteCollection>();
	}
	Map<int, Note*, std::less<int>> mNotes;//ignore key vals, i just set them to 0 - size-1 since they are runtime UIDs, not serializee

	INLINE void _Del() {
		for (int i = 0; i < mNotes.GetSize(); i++)
			delete mNotes[i].second;
		mNotes.ClearElements();
	}

	~NoteCollection() {
		for (int i = 0; i < mNotes.GetSize(); i++)
			delete mNotes[i].second;
		mNotes.ClearElements();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(NoteCollection, col);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			if (meta->IsRead())
				col->_Del();
			u32 num = col->mNotes.GetSize();
			meta->serialize_uint32(&num);
			for (int i = 0; i < num; i++) {
				if (meta->IsRead()) {
					Note* note = new Note;
					PerformMetaSerializeAsync(meta, note);
					col->mNotes.AddElement(0, &i, &note);
				}
				else {
					PerformMetaSerializeAsync(meta, col->mNotes[i].second);
				}
			}
		}
		return r;
	}

};

struct DlgNodeCriteria {

	enum TestT {
		eRequired = 1,
		eForbidden = 2
	};

	enum ThresholdT {
		eAny = 1, eAll = 2
	};

	enum DefaultResultT {
		 eDefaultToPass = 1,
		 eDefaultToNotPass = 2,
		 eDefaultToNotPassUnlessTransparent =3
	};

	struct EnumTestT {
		TestT mVal;
	};

	struct EnumThresholdT {
		ThresholdT mVal;
	};

	struct EnumDefaultResultT {
		DefaultResultT mVal;
	};

	EnumTestT mTestType;
	ThresholdT mFlagsThreshold, mCriteriaThreshold;
	DefaultResultT mDefaultResult;
	Flags mClassFlags;//dlgconstants class flags
	Set<int> mClassIDs;


};

struct DlgLine : UID::Owner, DlgObjIDOwner{
	LanguageResProxy mLangResProxy;
};

struct DlgLineCollection : UID::Generator {
	Map<int, DlgLine, std::less<int>> mLines;
	MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgLineCollection>();
	}

};

// --
//note dlgnodejump is below Dlg struct since it uses it handle

struct DlgNodeStats : DlgNode {

	struct Cohort : DlgChild {

		Handle<T3Texture> mhImage;
		LanguageResProxy mDisplayText1, mDisplayText2;
		String mLayout;
		LanguageResProxy mSummaryDisplayText;

		virtual MetaClassDescription* GetMetaClassDescription() {
			return ::GetMetaClassDescription<Cohort>();
		}

		virtual DlgConstants::DlgChildClassID GetTypeID() override {
			return DlgConstants::DlgChildClassID::eChildClassCohort;
		}

	};

	struct DlgChildSetCohort : DlgChildSet {
		virtual MetaClassDescription* GetMetaClassDescription() override {
			return ::GetMetaClassDescription<DlgChildSetCohort>();
		}
		virtual MetaClassDescription* GetChildDesc() override {
			return ::GetMetaClassDescription<Cohort>();
		}
	};

	enum StatsType {
		kChoices = 1,
		kExtended = 2,
		kCrowdPlay = 3,
		kRelationships = 4
	};

	struct EnumStatsType : EnumBase {
		StatsType mVal;
	};

	DlgChildSetCohort mCohorts;
	EnumStatsType mStatsType;
	Handle<T3Texture> mhImage;
	LanguageResProxy mDisplayText;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeStats>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeStats;
	}

};

struct DlgNodeStoryBoard : DlgNode {

	Symbol mStoryBoardImage;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeStoryBoard>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeStoryboard;
	}

};

struct DlgNodeMarker : DlgNode {
	//marker node, empty
	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeMarker>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeMarker;
	}

};

struct DlgNodeText : DlgNode {

	LanguageResProxy mLangResProxy;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeText>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeText;
	}

};

struct DlgNodeParallel : DlgNode {

	struct PElement : DlgChild {
		virtual MetaClassDescription* GetMetaClassDescription() {
			return ::GetMetaClassDescription<PElement>();
		}
		virtual DlgConstants::DlgChildClassID GetTypeID() override {
			return DlgConstants::DlgChildClassID::eChildClassPElement;
		}
	};

	struct DlgChildSetElement : DlgChildSet {
		virtual MetaClassDescription* GetMetaClassDescription() override {
			return ::GetMetaClassDescription<DlgChildSetElement>();
		}
		virtual MetaClassDescription* GetChildDesc() override {
			return ::GetMetaClassDescription<PElement>();
		}
	};

	DlgChildSetElement mPElements;//parallel elements
	DlgNodeCriteria mElemUseCriteria;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeParallel>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeParallel;
	}

};

struct DlgNodeLogic : DlgNode {

	Rule mRule;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeLogic>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeLogic;
	}

};

struct DlgNodeIdle : DlgNode {

	enum OverrideOption {
		eUseDefaults = 1,
		eOverride = 2
	};

	struct EnumOverrideOption : EnumBase {
		OverrideOption mVal;
	};

	Handle<Chore> mhIdle;
	float mTransitionTimeOverride;
	int mTransitionStyleOverride;
	int mIdleSlot;
	EnumOverrideOption mOverrideOptionTime, mOverrideOptionStyle;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeIdle>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeIdle;
	}

};

struct DlgNodeExit : DlgNode {

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeExit>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeExit;
	}

};

struct DlgNodeExchange : DlgNode {

	enum EntryType {
		eLine = 1,
		eNote = 2
	};

	struct Entry {
		int mID;
		EntryType mType;
	};
	
	float mPriority;
	Handle<Chore> mhChore;
	NoteCollection* mpNotes;
	DlgLineCollection* mpLines;
	DCArray<Entry> mEntries;

	DlgLineCollection* GetLines() {
		if (!mpLines) {
			mpLines = new DlgLineCollection;
			mFlags |= 2;
		}
		return mpLines;
	}

	NoteCollection* GetNotes() {
		if (!mpNotes) {
			mpNotes = new NoteCollection;
			mFlags |= 1;
		}
		return mpNotes;
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(DlgNodeExchange, node);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			if (node->mFlags.mFlags & 1) {
				if (!node->mpNotes)
					node->mpNotes = new NoteCollection;
				r=PerformMetaSerializeAsync(meta, node->mpNotes);
				if (r != eMetaOp_Succeed)
					return r;
			}
			if (node->mFlags.mFlags & 2) {
				if (!node->mpLines)
					node->mpLines = new DlgLineCollection;
				r = PerformMetaSerializeAsync(meta, node->mpLines);
			}
		}
		return r;
	}

	DlgNodeExchange() {
		mpNotes = NULL;
		mpLines = NULL;
		mFlags.mFlags = 0;
	}

	virtual ~DlgNodeExchange() {
		if (mpLines)
			delete mpLines;
		if (mpNotes)
			delete mpNotes;
		mpNotes = NULL;
		mpLines = NULL;
	}

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeExchange>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeExchange;
	}

};

struct DlgNodeWait : DlgNode, DlgConditionSet {
	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeWait>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeExchange;
	}
};

struct DlgNodeChore : DlgNode {

	Handle<Chore> mChore;
	int mPriority;
	bool mLooping;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeChore>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeChore;
	}

};

struct DlgNodeConditional : DlgNode {

	DlgChildSetConditionalCase mCases;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeConditional>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeConditional;
	}

};

struct DlgNodeChoices : DlgNode {

	DlgChildSetChoice mChoices;
	DlgChildSetChoicesChildPre mPreChoice;
	DlgChildSetChoicesChildPost mPostChoice;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeChoices>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeChoices;
	}

};

struct DlgNodeSequence : DlgNode {

	enum PlaybackModeT {
		eSequential = 1,
		eShuffle = 2
	};

	enum PlayPositionT {
		eUnspecified =1,
		eFirst =2,
		eLast = 3
	};

	enum RepeatT {
		eIndefinitely = 1,
		eOne = 2,
		eTwo = 3,
		eThree = 4,
		eFour = 5,
		eFive = 6,
		eSix = 7,
		eMaxPlusOne = 8
	};

	enum LifetimeModeT {
		eLooping = 1,
		eSingleSequence = 2,
		eSingleSequenceRepeatFinal = 3
	};

	struct Element : DlgChild {
		RepeatT mRepeat;
		PlayPositionT mPlayPosition;
		virtual MetaClassDescription* GetMetaClassDescription() {
			return ::GetMetaClassDescription<Element>();
		}
		virtual DlgConstants::DlgChildClassID GetTypeID() {
			return DlgConstants::DlgChildClassID::eChildClassElement;
		}
	};

	struct DlgChildSetElement : DlgChildSet {
		virtual MetaClassDescription* GetMetaClassDescription() override {
			return ::GetMetaClassDescription<DlgChildSetElement>();
		}
		virtual MetaClassDescription* GetChildDesc() override {
			return ::GetMetaClassDescription<Element>();
		}
	};

	PlaybackModeT mPlaybackMode;
	LifetimeModeT mLifetimeMode;
	DlgChildSetElement mElements;
	DlgNodeCriteria mElemUseCriteria;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeSequence>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeSequence;
	}

};

struct DlgNodeScript : DlgNode {

	String mScriptText;
	bool mbBlocking;
	bool mbExecuteOnInstanceRetire;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeScript>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeScript;
	}

};

struct DlgNodeNotes : DlgNode {

	String mNoteText;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeNotes>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeNotes;
	}

};

struct DlgFolderChild : DlgChild {
	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription<DlgFolderChild>();
	}
	virtual DlgConstants::DlgChildClassID GetTypeID() override {
		return DlgConstants::DlgChildClassID::eChildClassFolderChild;
	}
};

//.DLOG FILES
struct Dlg : DlgObjIDOwner, UID::Generator {//UID im not 100% sure since its only in older games, looks like a UID 

	DCArray<DlgFolder*> mFolders;
	DCArray<DlgNode*> mNodes;

	void _DeleteData() {
		for (int i = 0; i < mFolders.GetSize(); i++)
			delete mFolders[i];
		mFolders.ClearElements();
		for (int i = 0; i < mNodes.GetSize(); i++)
			delete mNodes[i];
		mNodes.ClearElements();
	}

	~Dlg() {
		_DeleteData();
	}
	
	String mName;
	long mVersion;
	DlgObjID mDefFolderID;
	LanguageDB mLangDB;
	unsigned long mProjectID;
	Symbol mResourceLocationID;
	long mChronology;
	Flags mFlags;//values in DlgNode::ClassFlags
	DependencyLoader<1> mDependencies;
	PropertySet mProdReportProps;
	JiraRecordManager mJiraRecordManager;
	bool mbHasToolOnlyData;

	//no idea but games newer than (and including) the walking dead season 2 have this flag at the end of the file
	bool mbToolFlag;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(Dlg, dlog);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription,
			pContextDescription, pUserData);
		MetaClassDescription* nodeDesc = ::GetMetaClassDescription<DlgNode>();
		static int twd2 = -1;
		if (!nodeDesc) {
			TelltaleToolLib_RaiseError(
				"Cannot serialize dialog, meta not initialized (node meta not found)",
				ErrorSeverity::ERR
			);
			return eMetaOp_Fail;
		}
		else if(twd2==-1){
			twd2 = TelltaleToolLib_GetGameKeyIndex("WD2");
		}
		if (r == eMetaOp_Succeed) {
			if (meta->IsRead())
				dlog->_DeleteData();
			u32 folders = dlog->mFolders.GetSize();
			meta->serialize_uint32(&folders);
			for (int i = 0; i < folders; i++) {
				if (meta->IsWrite()) {
					PerformMetaSerializeFull(meta, dlog->mFolders[i],
						dlog->mFolders[i]->GetMetaClassDescription());
				}
				else {
					DlgFolder* f = new DlgFolder;
					PerformMetaSerializeAsync<DlgFolder>(meta, f);
					dlog->mFolders.AddElement(0, NULL, &f);
				}
			}
			u32 nodes = dlog->mNodes.GetSize();
			meta->serialize_uint32(&nodes);
			u64 hash;
			MetaClassDescription* clazz;
			if (meta->IsRead()) {
				for (int i = 0; i < nodes; i++) {
					meta->serialize_uint64(&hash);
					clazz = TelltaleToolLib_FindMetaClassDescription_ByHash(hash);
					if (!clazz) {
						static char temp[150];
						sprintf(temp, "Dialog contains unknown subclass of dialog node %llX", hash);
						TelltaleToolLib_RaiseError(
							temp,
							ErrorSeverity::ERR);
						return eMetaOp_Fail;
					}
					void* inst = clazz->New();
					if (!inst)
						return eMetaOp_OutOfMemory;
					DlgNode* base = (DlgNode*)clazz->CastToBase(inst, nodeDesc);
					if (!base) {
						TelltaleToolLib_RaiseError(
							"Dialog node in dialog does not subclass from DlgNode",
							ErrorSeverity::ERR);
						return eMetaOp_Fail;
					}
					r=PerformMetaSerializeFull(meta, inst, clazz);
					if (r != eMetaOp_Succeed)
						return r;
					dlog->mNodes.AddElement(0, NULL, &base);
				}
			}
			else {
				for (int i = 0; i < nodes; i++) {
					DlgNode* node = dlog->mNodes[i];
					hash = node->GetMetaClassDescription()->mHash;
					meta->serialize_uint64(&hash);
					PerformMetaSerializeFull(meta,
						node, node->GetMetaClassDescription());
				}
			}
			if (sSetKeyIndex >= twd2) {
				meta->serialize_bool(&dlog->mbToolFlag);
			}
		}
		return r;
	}

};


struct DlgNodeJump : DlgNode {

	enum JumpBehaviour {
		eJumpAndExecute = 1,
		eJumpExecuteAndReturn = 2,
		eReturn = 3
	};

	enum JumpTargetClass {
		eToName = 1,
		eToParent = 2,
		eToNodeAfterParentWaitNode = 3
	};

	enum VisibilityBehaviour {
		eIgnoreVisibility = 1,
		eObeyVisibility = 2,
	};

	struct EnumJumpTargetClass :EnumBase {
		JumpTargetClass mVal;
	};

	struct EnumJumpBehaviour : EnumBase {
		JumpBehaviour mVal;
	};

	struct EnumVisibilityBehaviour : EnumBase {
		VisibilityBehaviour mVal;
	};

	DlgNodeLink mJumpToLink;
	Symbol mJumpToName;
	EnumJumpTargetClass mJumpTargetClass;
	EnumJumpBehaviour mJumpBehaviour;
	EnumVisibilityBehaviour mVisibilityBehaviour;
	int mChoiceTransparency;
	Handle<Dlg> mhJumpToDlg;

	virtual MetaClassDescription* GetMetaClassDescription() override {
		return ::GetMetaClassDescription<DlgNodeJump>();
	}

	virtual DlgConstants::DlgNodeClassID GetClassID() override {
		return DlgConstants::DlgNodeClassID::eNodeJump;
	}

};

struct DialogInstance {
	struct InstanceID {
		unsigned __int32 mID;
	};
};

/* ------------------------THE REST OF THIS HEADER IS OLDER DIALOGS, .DLG FILES (OLDER GAMES). SAME JOB AS ABOVE, BUT FOR OLDER ONES ---------------------------*/

struct DialogBase {

};

struct DialogDialog : DialogBase {

};

struct DialogBranch : DialogBase {

};

struct DialogItem : DialogBase {

	enum PlaybackMode {
		sequential_looping = 0x0,
		sequential_repeat_final = 0x1,
		sequential_die_off = 0x2,
		shuffle_repeat_all = 0x3,
		shuffle_repeat_final = 0x4,
		shuffle_die_off = 0x5,
		first_then_shuffle_repeat_remaining = 0x6,
	};

	struct EnumPlaybackMode {
		PlaybackMode mVal;
	};

};

struct DialogExchange : DialogBase {

};

struct DialogText : DialogBase {

};

struct DialogLine : DialogBase {

};

//.DLG FILES
struct DialogResource {
	long miNextDialogID, miNextBranchID, miNextItemID, miNextExchangeID, miNextLineID, miNextTextID, miNextChoreID;
	DCArray<int> mDialogs, mSoloItems, mTexts;
	long mProjectID;
	String mResourcePath;
	unsigned long mTaskID;
	Map<int, Ptr<DialogDialog>> mResDialogs;
	Map<int, Ptr<DialogBranch>> mResBranches;
	Map<int, Ptr<DialogItem>> mResItems;
	Map<int, Ptr<DialogExchange>> mResExchanges;
	Map<int, Ptr<DialogLine>> mResLines;
	Map<int, Ptr<DialogText>> mResTexts;

	static inline METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* pStream = (MetaStream*)pUserData;
		DialogResource* pThis = (DialogResource*)pObj;
		MetaOpResult res = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (res == eMetaOp_Succeed) {
			int nDialogs{ pThis->mResDialogs.GetSize() };
			int nBranches{ pThis->mResBranches.GetSize() };
			int nItems{ pThis->mResItems.GetSize() };
			int nEx{ pThis->mResExchanges.GetSize() };
			int nLines{ pThis->mResLines.GetSize() };
			int nTexts(pThis->mTexts.GetSize());
			pStream->serialize_uint32((u32*)&nDialogs);
			pStream->serialize_uint32((u32*)&nBranches);
			pStream->serialize_uint32((u32*)&nItems);
			pStream->serialize_uint32((u32*)&nEx);
			pStream->serialize_uint32((u32*)&nLines);
			pStream->serialize_uint32((u32*)&nTexts);

			//TODO 

		}

		return res;
	}

	~DialogResource() {
		for (auto& it : mResDialogs)
			it.second.DeleteObject();
		for (auto& it : mResBranches)
			it.second.DeleteObject();
		for (auto& it : mResItems)
			it.second.DeleteObject();
		for (auto& it : mResExchanges)
			it.second.DeleteObject();
		for (auto& it : mResLines)
			it.second.DeleteObject();
		for (auto& it : mResTexts)
			it.second.DeleteObject();
		mResDialogs.clear();
		mResBranches.clear();
		mResItems.clear();
		mResExchanges.clear();
		mResLines.clear();
		mResTexts.clear();
	}

};

#endif