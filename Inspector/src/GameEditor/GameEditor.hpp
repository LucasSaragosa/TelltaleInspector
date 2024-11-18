#include "../TelltaleInspector.h"

#include "ToolLibrary/Lua/lua.hpp"
#include "ToolLibrary/Types/PreloadPackage.h"
#include "ToolLibrary/TTArchive2.hpp"
#include "ToolLibrary/T3/RenderScene.h"
extern "C" {
#include "ToolLibrary/Lua/decompile.h"
#include "ToolLibrary/lua/proto.h"
}

#include <thread>
#include <queue>
#include <mutex>

#include "IntrinsicMath.h"

struct ResourceSetDescription {
	std::string mLogicalName, mLogicalDest, mName, mSetName, mDescOverride;
	int mPriority, mGameDataPriority, mDescPriority;
	std::string mVersion, mEnableMode;
	//no local dir, should always be currentdirectory
	std::string mGameDataName, mGameDataEnableMode;
	bool mLocalDirIncludeBase, mLocalDirRecurse;
	std::vector<std::string> mLocalDirIncludeOnly;
	std::vector<std::string> mLocalDirExclude;
	std::vector<std::string> mGameDataArchives;
};

struct GameEditor_DiskResourceSet {

	enum Type {
		/*resdesc description*/
		RESOURCE_SET_DESCRIPTION,
		/*any file*/
		SINGLE_FILE,
		/*.ttarch file*/
		DATA_ARCHIVE,
		/*User puts files in this folder, and it can be created into an archive.*/
		USER_ARCHIVE,
	};

	Type mType;
	std::string mGameDataPathPost;
	ResourceSetDescription mResourceSetDesc;
	std::unique_ptr<MetaStream> mpSingleFileMS;
	std::string mName;
	bool mbEdited = false;

	// ARCHIVE DATA
	TTArchive2* mArchive2 = 0;
	std::atomic_bool* loaded;
	std::vector<std::string> filenames;

	std::string& GetDisplayName() {
		return mType == RESOURCE_SET_DESCRIPTION ? mResourceSetDesc.mName : mName;
	}

	inline bool IsUser(){
		for (int i = 0; i < sRuntime.settings.mUserResourceSets.mSize; i++) {
			if (iequals(mName, sRuntime.settings.mUserResourceSets[i])) {
				return true;
			}
		}
		return false;
	}

};

enum EditorView {
	RESDESCS,
	GAMEDATA,
	SCENE_EDITOR,
};

inline bool _GFG_StringMatches(const char* first, const char* second)
{
	if (*first == '\0' && *second == '\0')
		return true;

	if (*first == '*') {
		while (*(first + 1) == '*')
			first++;
	}

	if (*first == '*' && *(first + 1) != '\0'
		&& *second == '\0')
		return false;

	if (*first == '?' || *first == *second)
		return _GFG_StringMatches(first + 1, second + 1);

	if (*first == '*')
		return _GFG_StringMatches(first + 1, second) || _GFG_StringMatches(first, second + 1);
	return false;
}

class GameEditorTask;

struct RenderObjectBase {

	virtual ~RenderObjectBase(){}

	virtual void Render(GameEditorTask* pTask) = 0;

};

class GameEditorTask : public InspectorTask, public RenderThreadContext {
public:
	std::filesystem::path base_path{}, game_data{};
	std::filesystem::path packs{};

	class TempBuffer {
		char* mpBuffer;
		GameEditorTask* mpTask;
	public:
		inline char* GetBuffer() const {
			return mpBuffer;
		}
		inline TempBuffer(GameEditorTask* pTask, u32 size) : mpTask(pTask) {
			if (size > 4096)
				mpBuffer = new char[size];
			else
				mpBuffer = pTask->working_buffer;
		}
		inline ~TempBuffer() {
			if (mpBuffer != mpTask->working_buffer)
				delete[] mpBuffer;
			mpBuffer = 0;
		}
	};

	std::vector<RenderObjectBase*> renderObjects;
	std::string mError{};
	std::vector<std::string> mVersionStrs;
	std::vector<GameEditor_DiskResourceSet> mResourceSets;
	bool mbInit = false;
	bool bSelecting = true;
	std::string mInputRD = "_resdesc_50_XXX.lua";
	int mGameKeyIndex = 0;
	const char* mpGameID = nullptr;
	BlowfishKey mGameKey{ 0 };
	void* mpLuaArg = nullptr;
	std::string findFileField;
	char working_buffer[4096];
	u64 inputHash = 0;
	bool mbMakeEncrypt = false;
	bool mbMakeZlib = false;
	virtual void _render() override;
	int selectedArchiveIndex = -1;
	int selectedMakeArchiveIndex = -1;
	int resDescIndex = -1;
	std::atomic_bool* mpMakeArchiveWaiter = 0;
	std::atomic_uintptr_t progressMsg = 0;
	unsigned long long frameNum = 0;
	unsigned int sceneCamWindowHeight = 0;
	unsigned int sceneCamWindowWidth = 0;
	HANDLE sem = INVALID_HANDLE_VALUE;
	PreloadPackage::RuntimeDataScene* mpScenePreloadPackage = 0;
	std::mutex lock;
	bool mbScenePreloaded = false;
	bool mbScenePreloading = false;
	std::string mFilterString = "";
	std::vector<InspectorTask*> openTask;
	int aliveSlot = -1;
	bool mbSortFilename = false;//no need to atomic, is set before threads fired
	std::atomic_int loadedArchives = 0;
	int sentArchives = 0;
	PropertySet mLoadedResources;
	Scene* mpActiveScene = 0;
	std::string filterType;
	std::vector<String> erroredResources;
	AnyTypeEditor sceneLister;
	EditorView currentView = RESDESCS;

public:

	void _init_diskres(GameEditor_DiskResourceSet& set);

	void _init_load_resource_sets(std::string current_folder, std::string current_app, bool bFoldersOrFiles);

	static void AsyncMakeCb(const char*, float);

	static void AsyncLoadArchive2(void* task, void* a, void* b);

	static void AsyncMakeArchive2(void* task, void* a, void* b);

	static void AsyncDispatchPreloads(void* task, void*, void*);

	static void AsyncPreloadResource(void* task, void* index, void* b);

	void PreviewFile(std::string fileName, DataStream* pStream);

	DataStream* OpenResource(Symbol name, std::string& destName);

	void* FindLoadedResource(Symbol name);

	void* FindOrLoadResource(Symbol name, MetaClassDescription* pResourceClass);

	void RenderGameData();

	void RenderResDescs();

	void RenderSceneEditor();

	void OnSceneExit();

	void OnSceneLoad();

	void OnScenePostLoad();

	void SaveResDesc(GameEditor_DiskResourceSet& set, bool enc);

	void Init(std::filesystem::path base);

	void RenderScene();

	void RenderUI();

	virtual void* GetResource(Symbol name, MetaClassDescription* pMCD) override;

	virtual DataStream* OpenShaderPackage(const String& fxpack) override;

	inline deleteme(GameEditorTask)

	inline taskctor(GameEditorTask) {}

	~GameEditorTask();

};