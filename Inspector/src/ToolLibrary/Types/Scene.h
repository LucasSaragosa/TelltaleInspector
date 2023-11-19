// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _SCENE
#define _SCENE

#include "PropertySet.h"
#include "../Meta.hpp"
#include "../TelltaleToolLibrary.h"
#include "HandleObjectInfo.h"
#include "List.h"
#include "DCArray.h"
#include "T3Texture.h"

//.SCENE FILES
struct Scene {//only included whats serialized and needed. this is a very very big struct! (class in the engine)

	static constexpr Symbol kSceneLocationInfo{ 0x0CDAB21B527F20135 };
	static constexpr Symbol kSceneStartPosKey{ 0x0A1FB3B4404D8C439 };
	static constexpr Symbol kSceneStartRotKey{ 0x53F29BE330D2D613 };
	static constexpr Symbol kSceneStartVisibilityKey{ 0x2AA2DE83FB51649C };
	static constexpr Symbol kSceneTransientKey{ 0x0C120B5E7442846DA };
	static constexpr Symbol kSceneAttachedKey{ 0x9ABEF281E8A46346 };
	static constexpr Symbol kSceneAttachedAgentKey{ 0x42BF5BF71E37A541 };
	static constexpr Symbol kSceneAttachedNodeKey{ 0x990221055FE50803 };
	static constexpr Symbol kSceneQualitySettingKey{ 0x349D77B9827E1962 };
	static constexpr Symbol kSceneUseDeprecatedChoreCameraLayers{ 0x6961EFFDCDAE4D59 };

	struct AgentInfo {
		void* mpAgent;//no need for agent
		String mAgentName;
		PropertySet mAgentSceneProps;
		//if in future i add a serialize async func then update scene serialize

		AgentInfo() {
			mpAgent = NULL;
			mAgentName = String("New Agent");
		}

	};

	struct AgentQualitySettings {
		Flags mFlags;//flag val 1 = exlude from low quality builds
	};

	//these 2 are not serialized
	float mTimeScale;
	bool mbActive;

	bool mbHidden;
	String mName;
	DCArray<HandleLock<Scene>> mReferencedScenes;
	//only add values to this using operator new! takes ownership
	List<AgentInfo*> mAgentList;

	Scene() = default;

	Scene(const Scene& o) = delete;//NO COPIES!
	Scene& operator=(const Scene&) = delete;

	Scene(Scene&&) = delete;
	Scene& operator=(Scene&&) = delete;

	~Scene() {
		for (auto it = mAgentList.begin(); it != mAgentList.end(); it++) {
			delete *it;
		}
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDesc, 
		MetaMemberDescription* c, void* pUserData) {
		MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj, pObjDesc, c, pUserData);
		if (result == MetaOpResult::eMetaOp_Succeed) {
			MetaStream* stream = static_cast<MetaStream*>(pUserData);
			Scene* scene = static_cast<Scene*>(pObj);
			stream->BeginBlock();
			u32 num = scene->mAgentList.size();
			stream->serialize_uint32(&num);
			MetaClassDescription* inf_mcd = GetMetaClassDescription(typeid(AgentInfo).name());
			if (!inf_mcd)return MetaOpResult::eMetaOp_Fail;//!!
			if (stream->mMode == MetaStreamMode::eMetaStream_Read) {
				for (int i = 0; i < num; i++) {
					AgentInfo *info = new AgentInfo;
					result = Meta::MetaOperation_SerializeAsync(info, inf_mcd, c, pUserData);
					if (result != eMetaOp_Succeed)break;
					scene->mAgentList.push_back(info);
				}
			}
			else {
				for (auto it = scene->mAgentList.begin(); it != scene->mAgentList.end(); it++) {
					result = Meta::MetaOperation_SerializeAsync(*it, inf_mcd, c, pUserData);
					if (result != eMetaOp_Succeed)break;
				}
			}
			stream->EndBlock();
		}
		return result;
	}

	AgentInfo* CreateAgent(String pAgentName) {
		AgentInfo* info = new AgentInfo;
		info->mAgentName = pAgentName;
		mAgentList.push_back(info);
		return info;
	}

	//wont bother with serialize main :P

};

struct ClipResourceFilter {
	Set<Symbol> mResources;
	bool mbExclusiveMode;
};

struct PerAgentClipResourceFilter {
	Map<String, ClipResourceFilter, StringCompareCaseInsensitive> mIncludedAgents;
	Set<String, StringCompareCaseInsensitive> mExcludedAgents;
	bool mbExclusiveMode;
};

struct T3LightSceneInternalData {

	struct LightmapPage {
		Handle<T3Texture> mhTextureAtlas;
		Flags mFlags;
	};

	struct QualityEntry {
		DCArray<LightmapPage> mLightmapPages;
		Handle<T3Texture> mhStaticShadowVolumeTexture;
	};

	QualityEntry mEntryForQuality[4];
	unsigned int mStationaryLightCount;
	unsigned int mBakeVersion;

};

#endif