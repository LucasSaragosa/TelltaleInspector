 // This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _RULES
#define _RULES

#include "../Meta.hpp"
#include "HandleObjectInfo.h"
#include "PropertySet.h"
#include "Map.h"
#include "DCArray.h"

struct LogicGroup {

	struct LogicItem : PropertySet {
		String mName;
		Map<Symbol, bool, std::less<Symbol>> mKeyNegateList;
		Map<Symbol, int, std::less<Symbol>> mKeyComparisonList, mKeyActionList;
		DCArray<String> mReferenceKeyList;
	};

	long mOperator;
	Map<String, LogicItem, std::less<String>> mItems;
	DCArray<LogicGroup> mLogicGroups;
	long mGroupOperator;
	long mType;
	String mName;

};

struct Rule {

	String mName;
	String mRuntimePropName;
	Flags mFlags;
	LogicGroup mConditions, mActions, mElse;
	String mAgentCategory;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {

		}
		return r;
	}

};

//.RULES FILES
struct Rules {
	Flags mFlags;
	Handle<PropertySet> mhLogicProps;
	Map<String, Rule*, std::less<String>> mRuleMap;

	~Rules() {
		for (int i = 0; i < mRuleMap.GetSize(); i++) {
			delete mRuleMap[i].second;
		}
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		Rules* rules = static_cast<Rules*>(pObj);
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		bool read = meta->mMode == MetaStreamMode::eMetaStream_Read;
		MetaClassDescription* ruleDesc = GetMetaClassDescription<Rule>();
		if (r == eMetaOp_Succeed) {
			meta->BeginBlock();
			u32 num = rules->mRuleMap.GetSize();
			meta->serialize_uint32(&num);
			String string{};
			std::pair<String, Rule*> p;
			if (read) {
				for (int i = 0; i < rules->mRuleMap.GetSize(); i++) {
					delete rules->mRuleMap[i].second;
				}
				rules->mRuleMap.ClearElements();
			}
			for (int i = 0; i < num; i++) {
				if (read) {
					meta->serialize_String(&string);
					Rule* pRule = 0;
					rules->mRuleMap.AddElement(0, &string, &pRule);
				}
				else {
					p = rules->mRuleMap[i];
					meta->serialize_String(&p.first);
				}
			}
			for (int i = 0; i < num; i++) {
				if (read) {
					//if(!old)
						//meta->serialize_String(&string);
					Rule* rule = new Rule;
					PerformMetaSerializeAsync<Rule>(meta, rule);
					auto it = rules->mRuleMap.begin();
					std::advance(it, i);
					it->second = rule;
				}
				else {
					p = rules->mRuleMap[i];
					//if(!old)
						//meta->serialize_String(&p.first);
					PerformMetaSerializeAsync<Rule>(meta, p.second);
				}
			}
			meta->EndBlock();
		}
		return r;
	}

};

#endif