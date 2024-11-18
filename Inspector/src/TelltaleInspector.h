#pragma once

// ==================================================================================== INCLUDES AND MACROS ====================================================================================


#include "ToolLibrary/Types/Dialog.h"
#include "ToolLibrary/Types/AnimOrChore.h"
#include "imstd/imgui_stdlib.h"
#include "ToolLibrary/MetaInitHelpers.h"
#include "imstd/imgui_memedit.h"
#include <memory>
#include <string>
#include "ToolLibrary/TTArchive2.hpp"
#include "ToolLibrary/FMOD/FSB5.h"
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "imgui.h"
#include "ToolLibrary/Lua/lua.hpp"
#include "ToolLibrary/Types/D3DMesh.h"
#include "ToolLibrary/LegacyTypes/D3DMeshLegacy.h"
#include "nfd.h"
#include "ToolLibrary/Types/Scene.h"
#include <mutex>
#include "imstd/imgui_stdlib.h"
#include "ToolLibrary/T3/T3EffectCache.h"
#include "ToolLibrary/Types/DCArray.h"
#include "ImText/TextEditor.h"

#include <vector>
#include <queue>
#include <thread>
#include <mutex>

#define APP_VERSION "v2.0.0"

#define APP_HELP_DESC " Use the toolbar to open windows to mod specific files."\
" More tools will come in the future. Do not rush or you will make mistakes! Read all information I give you and ASK on discord if anything is wrong."\
" Most tools will require you to select a game before you input the files. You MUST ensure the file(s) are from the right game or the app will likely crash. This also"\
" means that the app could interpret the file wrong which may cause it to use *many* gigabytes of ram mistakenly and this may even crash your PC so BE CAREFUL."\
" Please also note, old games before the wolf among us will not work in this tool (they may work for few small files.) This app should be used mainly for "\
"new games such as the walking dead collection, etc."

#define taskctor(T) T(const char* name, int id) : InspectorTask(name, id)
//mark as to delete obviously doesnt need to be virtual = 0, but old fix saves needing to go and edit all
#define deleteme() virtual void _deleteme() override { delete this; }

const u64 hash_int8 = CRC64_CaseInsensitive(0, "int8");
const u64 hash_uint16 = CRC64_CaseInsensitive(0, "uint16");
const u64 hash_int16 = CRC64_CaseInsensitive(0, "int16");
const u64 hash_int = CRC64_CaseInsensitive(0, "int");
const u64 hash_long = CRC64_CaseInsensitive(0, "long");//32 bit
const u64 hash_uint64 = CRC64_CaseInsensitive(0, "uint64");
const u64 hash_float = CRC64_CaseInsensitive(0, "float");
const u64 hash_int64 = CRC64_CaseInsensitive(0, "int64");
const u64 hash_double = CRC64_CaseInsensitive(0, "double");
const u64 hash_int32 = CRC64_CaseInsensitive(0, "int32");
const u64 hash_uint32 = CRC64_CaseInsensitive(0, "uint32");
const u64 hash_sym = CRC64_CaseInsensitive(0, "Symbol");
const u64 hash_uint8 = CRC64_CaseInsensitive(0, "uint8");
const u64 hash_bool = CRC64_CaseInsensitive(0, "bool");
const u64 hash_str = CRC64_CaseInsensitive(0, "String");

const u64 sphere_str = CRC64_CaseInsensitive(0, "Sphere");
const u64 tran_str = CRC64_CaseInsensitive(0, "Transform");
const u64 vec2_str = CRC64_CaseInsensitive(0, "Vector2");
const u64 vec3_str = CRC64_CaseInsensitive(0, "Vector3");
const u64 vec4_str = CRC64_CaseInsensitive(0, "Vector4");
const u64 col_str = CRC64_CaseInsensitive(0, "Color");
const u64 quat_str = CRC64_CaseInsensitive(0, "Quaternion");

// ==================================================================================== UTIL FUNCTIONS ====================================================================================

class MeshTask;
class SceneTask;

typedef void (save_changes_fndecl)(PropertySet& prop, void* userdata);

template<typename T>
bool DCArray_Contains(DCArray<T>& arr, T& val) {
	for (int i = 0; i < arr.mSize; i++)
		if (arr.mpStorage[i] == val)
			return true;
	return false;
}

template<typename T>
void DCArray_Push(DCArray<T>& arr, T* val) {
	if (arr.mSize == arr.mCapacity) {
		arr.Resize(4);//bit different to telltale who cares
	}
	arr.SetElement(arr.mSize, 0, val);
	arr.mSize++;
}

inline const char* select_gameid_dropdown(const char* last) {
	const char* selected = last;
	if (last)
		last = sBlowfishKeys[TelltaleToolLib_GetGameKeyIndex(last)].game_name;
	if (ImGui::BeginCombo("##none", last ? last : "Select")) {
		for (int i = 52; i < KEY_COUNT; i++) {//52 an on (wolf among us and newer)
			if (ImGui::Selectable(sBlowfishKeys[i].game_name)) {
				selected = sBlowfishKeys[i].game_id;
				TelltaleToolLib_SetBlowfishKey(sBlowfishKeys[i].game_id);
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	return selected;
}

template <typename T>
std::basic_string<T> lowercase(const std::basic_string<T>& s)
{
	std::basic_string<T> s2 = s;
	std::transform(s2.begin(), s2.end(), s2.begin(),
		[](const T v) { return static_cast<T>(std::tolower(v)); });
	return s2;
}

inline bool IsType(MetaClassDescription* clazz, const char* tn) {
	return Symbol{ tn }.GetCRC() == clazz->mHash;
}

inline bool iequals(const std::string& aa, const std::string& bb)
{
	const char* a = aa.c_str(), * b = bb.c_str();
	for (;; a++, b++) {
		int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
		if (d != 0 || !*a)
			return d == 0 ? true : false;
	}
	return true;
}

void MeshPropDelegate(PropertySet& set, void* userdata);

void ScenePropDelegate(PropertySet& set, void* userdata);

inline bool agent_sorter(Scene::AgentInfo*& lhs, Scene::AgentInfo*& rhs) {
	std::less<std::string> strl{};
	return strl(lhs->mAgentName, rhs->mAgentName);
}

inline std::string open_file(const char* filter) {
	nfdchar_t* d{ 0 };
	if (NFD_OpenDialog(filter, 0, &d) == NFD_OKAY) {
		std::string str{ d };
		free(d);
		return std::move(str);
	}
	return "";
}

class InspectorTask;
class GameEditorTask;
struct lua_State;

struct filet {
	std::string file, path;
	u64 off = 0;
	u32 sz = 0;

	filet(filet&& rhs) {
		file = std::move(rhs.file);
		path = std::move(rhs.path);
		off = rhs.off;
		sz = rhs.sz;
	}

	filet& operator=(filet&& rhs) {
		file = std::move(rhs.file);
		path = std::move(rhs.path);
		off = rhs.off;
		sz = rhs.sz;
		return *this;
	}

	filet(const filet&) = default;
	filet& operator=(const filet&) = default;

	filet(std::string&& n, std::string path, u64 off = 0, u32 s = 0) : file(_STD move(n)) {
		this->path = path;
		this->off = off;
		this->sz = s;
	}
};

inline bool filet_sorter(const filet& lhs, const filet& rhs) {
	std::less<std::string> strl{};
	return strl(lhs.file, rhs.file);
}


enum exitstrat {
	accept_all,
	ignore_all,
	ask,
};

enum resolve_mode {
	file_name = 0,
	prop_key = 1,
	check_all = 2,
};

struct PersistentData {
	void (*mDealloc)(void* pData) = nullptr;
	void* pDataInstance = nullptr;

	PersistentData() = default;

	~PersistentData() {
		if (mDealloc && pDataInstance)
			mDealloc(pDataInstance);
		mDealloc = nullptr;
		pDataInstance = nullptr;
	}

	PersistentData(const PersistentData&) = delete;
	PersistentData& operator=(const PersistentData&) = delete;

	PersistentData(PersistentData&& rhs) {
		mDealloc = rhs.mDealloc;
		pDataInstance = rhs.pDataInstance;
		rhs.mDealloc = nullptr;
		rhs.pDataInstance = nullptr;
	}

	PersistentData& operator=(PersistentData&& rhs) {
		mDealloc = rhs.mDealloc;
		pDataInstance = rhs.pDataInstance;
		rhs.mDealloc = nullptr;
		rhs.pDataInstance = nullptr;
		return *this;
	}

};

struct ArrayPersistanceL {
	int selected = 0;
	bool mbSelectedOpen = false;
	bool mbShouldClose = false;
	char name_buf[256];
};

struct ArrayPersistance {
	int selected = 0;
	char name_buf[16];
	bool mbSelectedOpen = false;
	bool mbShouldClose = false;
};

struct PropGetterDelegate {
	ArrayPersistanceL* papl;
	PropertySet::KeyInfo* pinf;
};

inline bool prop_getter(void* data, int index, const char** output)
{
	PropGetterDelegate* del = (PropGetterDelegate*)data;
	sprintf((char*)del->papl->name_buf, "[%d] %s Value", index,
		(del->pinf + index)->mpValue->mpDataDescription->mpTypeInfoName);
	*output = (char*)del->papl->name_buf;
	return true;
}

inline void getname(std::string& dest, TTArchive2& arc, TTArchive2::ResourceEntry* pEntry, const std::filesystem::directory_entry& entry) {
	std::string tmp{ entry.path().filename().string() };
	if (tmp.length() == 0)
		tmp = "<ArchiveNameInvalid>";
	tmp += "/";
	std::string name = arc.GetResourceName(pEntry->mNameCRC);
	dest = tmp + name;
}

inline bool dcarray_getter(void* data, int index, const char** output)
{
	sprintf((char*)data, "[%d]", index);
	*output = (char*)data;
	return true;
}

inline void array_persist_del(void* inst) {
	delete ((ArrayPersistance*)inst);
}

inline void array_persist_del_l(void* inst) {
	delete ((ArrayPersistanceL*)inst);
}

class MetaStreamTask;

extern u64 renderProxyID;

inline static const float kD3DTextureBorders[2][4] = {
		{0.0f,0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f,1.0f}
};

inline static const char* kD3DTextureWraps[3] = {
	"Clamp", "Wrap", "Border"
};

struct MCDRenderMapProxy {

	u64 _ID;
	MetaClassDescription* pKeyType, * pValType;
	void* pKeyInst, * pValInst;

	MCDRenderMapProxy() {
		_ID = renderProxyID;
	}

};

template< typename T, typename Pred >
typename std::vector<T>::iterator
insert_sorted(std::vector<T>& vec, T const& item, Pred pred)
{
	return vec.insert
	(
		std::upper_bound(vec.begin(), vec.end(), item, pred),
		item
	);
}

static inline u64 MakePersistentDataID(MetaClassDescription* mcd, void* pInst) {
	return u64{ mcd->mHash ^ ((u64)pInst) };
}

inline MetaStream* GetStreamHelper(MetaStreamTask* pTask);

int luaAddBuildVersionInfo(lua_State* L);

typedef int (*LuaCClosure)(lua_State*);

inline std::string tex(u64 hash, std::vector<std::string>& texture_names, bool with_ext) {
	for (auto& it : texture_names)
		if (CRC64_CaseInsensitive(0, it.c_str()) == hash)
			return it.substr(0, it.find_last_of('.'));
	return "not_found";
}

class MeshTask;
typedef void (mtl_delta_cb)(MeshTask*, std::string&);

//CONVERT NEW D3DMESH => OBJ (GAMES NEWER AND INCLUDING THE WALKING DEAD: MICHONNE)
bool convert_obj_newer(D3DMesh& mesh, const char* gid, std::string objfile, std::string mtlfile, std::string& err, std::vector<std::string>& texnames, mtl_delta_cb mtlCB, MeshTask* t);
bool convert_obj_legacy(LegacyD3DMesh& mesh, const char* gid, std::string objfile, std::string mtlfile, std::string& err, std::vector<std::string>& texnames, mtl_delta_cb mtlCB, MeshTask* t);
bool convert_obj_legacy_OLD(LegacyD3DMesh& mesh, const char* gid, std::string objfile, std::string mtlfile, std::string& err, std::vector<std::string>& texnames, mtl_delta_cb mtlCB, MeshTask* t);

// ==================================================================================== SYMBOL MAP DEFINITIONS ====================================================================================

//.SYMMAP
struct SymbolMap {

	u64* map;
	u32 map_s;
	std::vector<std::string> strmap;

	struct _Serialized {
		u8 hash[8];
		u16 len;
		inline u64 get() {
			return *((u64*)(this->hash));
		}
	};

	void load(const char* path);

	void save(const char* path);

	bool resolve(u64 hash, std::string& dest);

	void insert(std::string& val);
	SymbolMap();

	~SymbolMap();

};

void SymMap_InsertUserHash(const char* str, resolve_mode type);

std::string* SymMap_ResolveHash(char* resolve_buf, std::unordered_map<u64, std::string>& db, std::vector<int>& dbFilenames, u64 hash, resolve_mode md);

// ==================================================================================== RUNTIME STRUCTS ====================================================================================

enum InspectorSettingsFlags {
	eISF_ShownTTArchMessage = 1
};

enum InspectorCol : u32 {
	eICol_Blue = 0,
	eICol_Green = 1,
	eICol_Red = 2,
	eICol_Orange = 3,
	eICol_Yellow = 4,
	eICol_Purple = 5
};

constexpr const char* ColorName[] {
	"Blue",
	"Green",
	"Red",
	"Orange",
	"Yellow",
	"Purple"
};

constexpr int NUM_UI_COLORS = 6;

struct InspectorSettings {

	DCArray<String> mUserResourceSets;
	bool mbShownIntroMessage = false;
	bool mbShowConsole = false;
	Flags mFlags;
	InspectorCol mUICol = eICol_Green;

	INLINE static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		return result;
	}

	INLINE static MetaClassDescription* GetMetaClassDescription() {
		static MetaClassDescription meta_set{};
		if (meta_set.mFlags.mFlags & Internal_MetaFlag_Initialized)
			return &meta_set;
		MetaClassDescription& meta_flags = *::GetMetaClassDescription<Flags>();
		MetaClassDescription& meta_bool = *::GetMetaClassDescription<bool>();
		MetaClassDescription& meta_astr = *::GetMetaClassDescription("DCArray<String>");
		MetaClassDescription& meta_int = *::GetMetaClassDescription("uint32");
		{
			DEFINET_3(set, InspectorSettings);
			meta_set.mbHiddenInternal = true;
			EXT(set, ttcfg);
			FIRSTMEM2(set, mbShownIntroMessage, InspectorSettings, bool, 0);
			NEXTMEM2(set, mFlags, InspectorSettings, flags, 0, mbShownIntroMessage);
			NEXTMEM2(set, mbShowConsole, InspectorSettings, bool, 0, mFlags);
			NEXTMEM2(set, mUserResourceSets, InspectorSettings, astr, 0, mbShowConsole);
			NEXTMEM2(set, mUICol, InspectorSettings, int, 0, mUserResourceSets);
			SERIALIZER(set, InspectorSettings);
			ADD(set);
			return &meta_set;
		}
	}

};

struct InspectorRuntime {
	int running_id = 0;
	std::string game_id = "";
	std::vector<InspectorTask*> open_tasks;
	std::vector<InspectorTask*> pending_deletions;
	std::vector<u64> registered_fns;
	lua_State* L;
	GameEditorTask* current_editor_task = nullptr;

	inline int gen_id() {
		return ++running_id;
	}
	
	inline int gen_gate_slot(){
		if (gates.size() >= 1048575) {//very very rare (probably will never happen), 1 MILLION slots! how long have you been editing? years!??
			MessageBoxA(0, "Too many gate slots have been used up! Application needs to abort.", "Slot capacity", MB_ICONERROR);
			abort();
		}else if(gates.size() >= 1040000){
			MessageBoxA(0, "Many gate slots have been used! Please save all your work and re-open the application executable otherwise soon the application may need to force abort!", "Slot capacity", MB_ICONWARNING);
		}
		if (gates.size() == 0)
			gates.reserve(1040000);
		gates.push_back(1);
		return gates.size() - 1;
	}

	std::string filterListMSFiles{};

	std::vector<const char*> filterListMSTypes{};
	std::vector<MetaClassDescription*> mcd_map{};
	std::vector<InspectorTask*> queued;

	//ints need to be used as pointers to the values need to be used, so cant use optimized bitset version. only 4MB, so not too bad for most modern machines
	std::vector<int> gates{};

	SymbolMap symbolMaps[2];

	InspectorSettings settings;

	std::mutex ItemsLock;
	std::vector<std::string> Items;

};

extern InspectorRuntime sRuntime;

// ==================================================================================== HELPER: ANY TYPE EDITOR ====================================================================================

class AnyTypeEditor {
public:

	std::vector<String> possible;
	MetaClassDescription* mh_handleRefType = nullptr;
	std::string mh_inputField;
	const char* mh_comboitem;
	int mh_uid = 0;
	Symbol mh_item;
	std::string modify_symbol_field;
	std::string mod_sym_final;
	bool mod_sym_nf = false;
	void* mod_sym_inst = nullptr;
	MemoryEditor bbEditor;
	MetaStream versionInfo{};
	//map new item data
	void* tmpkey = 0;
	char resolve_buf[1024];
	bool ignore_ctrln = false;
	std::string nprop_field;
	void* cur_new_parent;
	const char* nprop_type = "String";
	bool frame_check_addrem = false;
	std::unordered_map<u64, std::string> sym_map;
	std::vector<int> fn_map;

	friend class PropTask;

	bool modify_handle(MetaClassDescription* desc, Symbol& sym, const char* key = 0, std::string* pOutResolved = 0, std::vector<std::string>* pExtraPossibleValues = 0);

	void modify_symbol(Symbol& sym, const char* key, void* inst, const char* pResolved = 0, std::string* pOutResolved = 0, std::vector<std::string>* pExtraPossibleValues = 0);

	bool Math(u64 hash, void* pInst);

	int ListTree(const char* prefix, int uid, void* data, void* (*getnext)(void* dat, void* prev, int index), int num_elem, MetaClassDescription* pDesc, int* selected);

	void MapTreeKeyInput(MetaClassDescription* k, void* obj, bool reset = false);

	bool MapTreeKey(MetaClassDescription* k, std::string* dest, void* obj);

	void TypeTree(const char* prefix, int uid, MetaClassDescription* type, void* value);

	int MapTree(const char* prefix, int uid, int* selected, Map<void*, void*>* pProxyMap);

	void TreeItem(MetaClassDescription* clazz, void* pRawData, std::string* kn, int uid, u64& to_del, int i, bool bAllowDel, bool& hovered, u64 keyHash, MetaMemberDescription* memCheck = 0, bool disbleMemCheck = false);

	void PropTree(PropertySet* set, const char* prefix, int uid);

	//unqLocalID should be unoique to current scope in imgui and also is the name of the child which contains the pInstance of pType
	void Render(const char* unqLocalID, MetaClassDescription* pType, void* pInstance);

};

// ==================================================================================== INSPECTOR TASK HELPER AND BASE CLASS ====================================================================================

typedef void (*InspectorTask_DestroyCb)(InspectorTask* pInstance, void* pUserData);

class InspectorTask {
protected:


	std::string task_name;
	int task_id;
	std::vector<int> mSubmittedJobs{};

	InspectorTask_DestroyCb mpDestroyCallback;
	void* mpUserData;
	int mbCallDestroySlot = -1;

	virtual void _render() = 0;

	friend class JobScheduler;

public:

	bool HasAsyncDependencies();

	void* mpParentUserData = 0;

	virtual void _deleteme() = 0;

	inline int GetDestroyState(int defaultIfNoSlot){
		return mbCallDestroySlot == -1 ? defaultIfNoSlot : sRuntime.gates[mbCallDestroySlot];
	}

	inline void mark_as_todelete() { sRuntime.pending_deletions.push_back(this); }

	inline void SetDestroyCallback(InspectorTask_DestroyCb pCallbackFunction, void* pUserData, int destroyOnlyIf1_GateIndex = -1){
		mpDestroyCallback = pCallbackFunction;
		mbCallDestroySlot = destroyOnlyIf1_GateIndex;
		mpUserData = pUserData;
	}

	//only needed for some tasks. gets the current type instance being rendered.
	inline virtual void* GetUserInstance() {
		return 0;
	}

	virtual ~InspectorTask() {}

	InspectorTask(const char* name, int id);

	void render();

	friend class TelltaleInspector;

};

//No ownership semantics. Delete this task before deleting the parent task, using mark as to delete
class AnyTypeTask : public InspectorTask {
public:

	AnyTypeEditor mATE;
	MetaClassDescription* mpType;
	void* mpInstance;

	virtual void _render() override;

	INLINE AnyTypeTask(const char* wName, int rtID, MetaClassDescription* mcd, void* inst) : InspectorTask(wName, rtID) {
		mpInstance = inst;
		mpType = mcd;
	}

	virtual void* GetUserInstance() override {
		return mpInstance;
	}

	deleteme(AnyTypeTask)

	taskctor(AnyTypeTask) {}

};

struct TypeUserData {
	virtual ~TypeUserData() {}
};

class TypeTaskBase : public InspectorTask {
public:

	deleteme(TypeTaskBase)

	taskctor(TypeTaskBase) {}

	virtual ~TypeTaskBase() {}

};

template<typename T>
class TypeTask : public TypeTaskBase {
public:

	MetaClassDescription* mpTypeDesc = 0;
	T _mType;
	T* mpType = 0;//extern
	const char* id = 0;
	bool imported = false;
	MetaStream versionInfo{};
	TypeUserData* pUserData = 0;
	const char* error = 0;
	bool _delegated_init_flag = false;
	const char* parent = 0;

	virtual void _render() override;

	INLINE TypeTask(const char* wName, int rtID) : TypeTaskBase(wName, rtID) {}

	//Create new type task from already open data stream (which gets owned, it will be deleted by this function). Allows for file to not just come from user open from disk but can come from .TTARCH2 or anywhere.
	INLINE TypeTask(const char* windowName, int runtimeGeneratedID, const char* gameID, DataStream* pMyNewFileInStream) : TypeTaskBase(windowName, runtimeGeneratedID) {
		id = gameID;
		mpTypeDesc = GetMetaClassDescription<T>();
		MetaStream stream{};
		stream.InjectVersionInfo(versionInfo);
		stream.mbDontDeleteStream = true;
		stream.Open(pMyNewFileInStream, MetaStreamMode::eMetaStream_Read, {});
		imported = false;
		if (stream.mbErrored) {
			MessageBoxA(0, error = "Could not open stream", "Error opening file", MB_ICONERROR);
		}
		else {
			if (PerformMetaSerializeAsync(&stream, &_mType) != eMetaOp_Succeed) {
				MessageBoxA(0, error = "Could not import file, please contact me!", "Error", MB_ICONERROR);
			}
			else imported = true;
		}
		_delegated_init_flag = true;
	}

	//Create new type task from external Type pointer. Parent is the string displayed at the top of the window, eg the file its from. Render type data with initialise as true is called on first render
	INLINE TypeTask(const char* myParent, const char* windowName, int rtGenID, const char* gameID, T* pExternal) : TypeTaskBase(windowName, rtGenID) {
		mpType = pExternal;
		id = gameID;
		mpTypeDesc = GetMetaClassDescription<T>();
		imported = true;
		_delegated_init_flag = true;
	}

public:

	INLINE T* GetType(){
		return mpType != 0 ? mpType : &_mType;
	}

	virtual void* GetUserInstance() override {
		return GetType();
	}

	INLINE ~TypeTask() {
		if (pUserData)
			delete pUserData;
		pUserData = 0;
	}

};

//Delete when finished. Must not stay alive after the parent typetask is destroyed
InspectorTask* CreateAndOpenTypeTask(const char* embedNameUnique, MetaClassDescription* typeClass, const char* parentName, const char* gameID, void* typeInstance, InspectorTask_DestroyCb destroyCb, void* pDestroyCbUD, int destroyGateSlot = -1);

INLINE void TextCentered(std::string text) {
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.c_str());
}

template<typename T>
void RenderTypeData(TypeTask<T>* pTask, bool bInit) {
	static_assert(false, "No specialization found for render type data T");
};

template<>
void RenderTypeData<Dlg>(TypeTask<Dlg>* pTask, bool bInit);

template<>
void RenderTypeData<Chore>(TypeTask<Chore>* pTask, bool bInit);

template<>
void RenderTypeData<Animation>(TypeTask<Animation>* pTask, bool bInit);

// ==================================================================================== ALL TASK DEFINITIONS ====================================================================================

struct imported_file {
	std::string mName;
	DataStreamMemory* mMemoryStream;
	std::filesystem::path mDiskPath;
	bool mbIsMemory = false;
	int userData = 0;
};

class ArchiveTask : public InspectorTask {
public:

	const char* selected_game_id = nullptr;
	bool bState = false;
	bool bImported = false;
	bool bCompressedZ = false, bCompressedOodle = false, bEncrypted = false;
	TTArchive2 arch;
	std::vector<filet> file_names;
	int selectedfile = 0;

	inline deleteme();

	inline taskctor(ArchiveTask) {}

	inline static bool getter(void* data, int index, const char** output)
	{
		*output = ((filet*)data)[index].file.c_str();
		return true;
	}

	void import_in_singlefile(const std::filesystem::path& phys, exitstrat& strat);

	virtual void _render() override;

};

enum ImportedFileType {
	UNK = 0,
	SRC = 1,//src lua
	BIN = 2,//bin lua LuaR
	LEN = 3,//'LEn' for Lua Encrypted
	LEO = 4,//'LEn => LEo' o meaning outside of archives most likely.
	_ERR = 5,
};

class LuaTask : public InspectorTask {
public:

	std::vector<imported_file> mFiles;
	int selectedfile = 0;
	const char* gameid = nullptr;
	bool mbConvertToFolder = false;

	void import_in_singlefile(const std::filesystem::path& phys, exitstrat& strat);

	static bool is_raw_script(char* buffer, u64 size);

	static const char* reader(lua_State* L, void* ud, size_t* sizep);

	static int writer(lua_State* L, const void* p, size_t size, void* u);

	static inline char* luadec_strdup(const char* src);

	static bool do_luac(const char* Chunk, DataStream* pOut, char* pIn, u64 insize, bool bIsCompile);

	static bool convert_to_scriptn(DataStream* pIn, DataStream* pOut, const std::string& name, void* usr);

	static bool convert_to_scripto(DataStream* pIn, DataStream* pOut, const std::string& name, void* usr);

	static bool convert_extract(DataStream* pIn, DataStream* pOut, const std::string& name, void* usr);

	void bulk_conv_all(bool(*converter)(DataStream* pIn, DataStream* pOut, const std::string& name, void* usr), ImportedFileType successType);

	virtual void _render() override;
	inline deleteme(LuaTask)

	inline taskctor(LuaTask) {}

};

class BankTask : public InspectorTask {
public:

	int selectedfile;
	FMODSoundBank mBank{};

	static bool getter(void* data, int index, const char** output);

	static bool sample_sorter(const FMODSample& lhs, const FMODSample& rhs);

	virtual void _render() override;

	inline deleteme(BankTask)

	inline taskctor(BankTask) {}

};

class MetaStreamTask : public InspectorTask {

	friend class MetaStreamSubType;

	DataStream* pSourceStream;
	std::string mFileName;
	const char* gameid = nullptr;
	MetaClassDescription* mpType;
	MetaStream mStream;
	void* mpInstance;
	bool mbInvalidMS = false;
	bool mbIsEmpty = false;
	bool mbMemErr = false;
	bool mbUnknownFile = false;
	bool mbVerCancel = false;
	bool mbSerializeErr = false;
	std::string mVersName;
	MetaStreamSubType* mpRoot;

	std::vector<int> mDBFilenamePages;
	std::unordered_map<u64, std::string> mResolvedHashes;
	bool mbHasResolved = false;

	char resolve_buf[1024];//KEEP THIS LAST

	virtual void _render() override;

public:

	std::string* ResolveHash(u64 hash, resolve_mode md);

	MetaStreamTask(DataStream* pStream, std::string fileName, const char* gameid);

	virtual ~MetaStreamTask();

	deleteme(MetaStreamTask)

	taskctor(MetaStreamTask) {}

	friend MetaStream* GetStreamHelper(MetaStreamTask* pTask);

};

class MetaStreamSubType {
public:

	friend class ContainerRenderAdapter;

	u64 mBaseIDHash;
	char mID[32];
	MetaClassDescription* mpTypeDesc = nullptr;
	void* mpInstance = nullptr;
	MetaStreamTask* mpTask = nullptr;
	int mIDGen = 0;
	bool mbChildRendered = false;

	std::string mHeaderStr;

	std::unordered_map<u64, PersistentData> mPersistentData{};

	MetaStreamSubType* mpChildSub = nullptr;
	MetaStreamSubType* mpKeyChildSub = nullptr;

	MetaStreamSubType(const MetaStreamSubType&) = delete;
	MetaStreamSubType& operator=(const MetaStreamSubType&) = delete;

	MetaStreamSubType(MetaStreamSubType&&) = default;
	MetaStreamSubType& operator=(MetaStreamSubType&&) = default;

	MetaStreamSubType() = default;

	void ResetChild();

	void Reset();

	void Init(MetaStreamTask* pTask, void* pInst, MetaClassDescription* pType);

	const char* GenNextID();
	void RenderChild(void* pInst, MetaClassDescription* pType, int id, bool* pShouldClose, std::string hdr = "");
	void render_intrin_mcd(const char* mem, MetaClassDescription* mcd, void* pInst, float depth);

	void render_intrin_mcd_enum(const char* mem, MetaClassDescription* mcd, void* pInst, float depth, MetaMemberDescription* pMemDesc);

	void render_intrin_mcd_flags(const char* mem, MetaClassDescription* mcd, void* pInst, float depth, MetaMemberDescription* pMemDesc);

	static const inline u64 trangef_str = CRC64_CaseInsensitive(0, "TRange<float>");

	void render_intrin_mcd_trange(const char* mem, MetaClassDescription* mcd, void* pInst, float depth);

	void render_intrin_mcd_math(const char* mem, MetaClassDescription* mcd, void* pInst, float depth, u64 hash = 0);

	void render_mcd_array(const char* mem, u32 arraySize, void* pArray, MetaClassDescription* pArrayType, float depth);
	void render_mcd_std_cont(const char* mem, MetaClassDescription* mcd, void* pInst, float depth);
	void render_mcd_map(const char* mem, MetaClassDescription* mcd, void* pInst, float depth);

	void render_mcd_dcarray(const char* mem, MetaClassDescription* mcd, void* pInst, float depth);

	void render_mcd_sarray(const char* mem, MetaClassDescription* mcd, void* pInst, float depth);

	bool render_mcd_internal(const char* mem, MetaClassDescription* mcd, void* pInstance, float depth);
	static bool mesh_cb(void* pd, int idx, const char** pout);
	void render_mcd(MetaClassDescription* mcd, void* pInstance, float depth = 0.0f);
	void Render(int current_id, float depth);

};

inline MetaStream* GetStreamHelper(MetaStreamTask* pTask) {
	return &pTask->mStream;
}

class ContainerTask : public InspectorTask {

	std::string file;
	DataStreamContainer* container;
	unsigned long long size = 0;

	virtual void _render() override;

public:

	ContainerTask(const char* tt, int id);

	~ContainerTask();

	inline deleteme(ContainerTask);

};

class MeshTask : public InspectorTask {
public:

	friend void MeshPropDelegate(PropertySet& set, void* userdata);//no save changes needed

	bool bLegacy = false;
	bool bLoaded = false;
	LegacyD3DMesh* mpLegacyMesh;
	D3DMesh* mpNormalMesh;
	MetaStream mStream;
	const char* id = nullptr;
	const char* err = 0;
	std::vector<std::string> texture_names;
	std::vector<std::string> unk_textures;
	std::vector<std::string> norm_materials;

	std::unordered_map<u64, std::string> material_name_db;
	std::vector<int> filenamedb;
	char resolve_buf[1024];

	std::string mtl_prefix, mtl_suffix;
	bool mtl_remove_png = false;
	bool bLowerCase = false;

	void process_mtl(std::string& in);

	static void __declspec(noinline) process_mtl_cb(MeshTask* task, std::string& in);

	bool try_open_mesh(std::string file);

	void render_mtl_edit();

	int open_index = -1;
	bool* open_gate = 0;

	virtual void _render() override;

	~MeshTask();

	inline deleteme(MeshTask)

	inline taskctor(MeshTask) {
		mpLegacyMesh = 0;
		mpNormalMesh = 0;
	}

};

class TextureTask : public InspectorTask {
public:

	T3Texture* mpTexture;
	MetaStream mStream;
	const char* id;
	bool ok;
	bool loaded;
	std::string err;
	MetaClassDescription* pTextureMCD;
	const char* texfmt = 0;

	virtual void _render() override;

	inline deleteme(TextureTask)

	inline ~TextureTask() {
		if (mpTexture)
			delete mpTexture;
		mpTexture = 0;
	}

	inline taskctor(TextureTask) {
		mpTexture = 0;
		err = "";
		ok = loaded = false;
		ok = true;
		id = 0;
		pTextureMCD = GetMetaClassDescription<T3Texture>();
	}

}; 

class PropTask : public InspectorTask {
	struct {
		PropertySet __mNormal, * __mpExternal;
	} _int;
public:
	inline void set_prop(std::string&& name, bool* b_access_gate, const char* gameid, save_changes_fndecl* change_fn, PropertySet* ext, void* user) {
		save_changes = change_fn;
		_int.__mpExternal = ext;
		game = gameid;
		game_n = sBlowfishKeys[TelltaleToolLib_GetGameKeyIndex(game)].game_name;
		selected = true;
		imp_yet = true;
		ext_access_gate = b_access_gate;
		prop_name = _STD move(name);
		ext_user_data = user;
	}
private:

	bool* ext_access_gate = nullptr;
	std::string prop_name;

	inline PropertySet& Props() {
		return save_changes == nullptr ? _int.__mNormal : *_int.__mpExternal;
	}

	void* ext_user_data = nullptr;
	const char* game = nullptr;
	const char* game_n = nullptr;
	bool replace_all = false;
	bool selected = false;
	bool imp_yet = false;
	bool update_parents = true;

	bool frame_check_addrem = false;
	void* cur_new_parent = nullptr;
	std::string nprop_field;
	const char* nprop_type = "String";

	save_changes_fndecl* save_changes = nullptr;

	std::unordered_map<u64, std::string> sym_map;
	std::vector<int> fn_map;

	int streamVersion;

	MetaStream versionInfo{};

	std::string nparent{};

	char resolve_buf[1024];

	AnyTypeEditor lister;

	virtual void _render() override;

public:

	virtual void* GetUserInstance() override {
		return &Props();
	}

	inline deleteme()

	inline taskctor(PropTask) {
		save_changes = 0;
	}

	inline ~PropTask() {
		if (save_changes != nullptr)
			save_changes(Props(), ext_user_data);
	}

};

class SceneTask : public InspectorTask {
public:

	friend void ScenePropDelegate(PropertySet& set, void* userdata);

	MetaStream mmStream;
	Scene* mpScene;
	bool mbOwnScene = false;
	void(*delete_inst)(Scene* inst);
	const char* id = nullptr;
	bool state = false;

	int open_index = -1;
	bool* open_gate = 0;

	std::string newAgentName = "";

	inline void sort_agents() {
		mpScene->mAgentList.sort(&agent_sorter);
	}

	virtual void _render() override;

	inline deleteme(SceneTask)

	inline taskctor(SceneTask) {
		mpScene = new Scene;
		mbOwnScene = 1;
	}

	inline SceneTask(Scene* pScene, void(*di)(Scene* inst), const char* wName, int id) : SceneTask(wName, id){
		mpScene = pScene;
		delete_inst = di;
	};

	inline ~SceneTask(){
		if (mbOwnScene)
			delete mpScene;
		else if(delete_inst)
			delete_inst(mpScene);
		mpScene = 0;
		if(open_gate!=0)
			*open_gate = false;
	}

};

class ShaderTask : public InspectorTask {
public:

	int numImports = 0;
	int importedVersion = 0;
	const char* id = 0;
	bool imported = false;
	String name{};
	TTArchive2 archive{};
	TextEditor mUserCode;
	bool mbEditingUserCode = false;
	std::atomic_bool awaitingLoad = false;
	bool mbWaiting = false;
	std::atomic_uintptr_t mpProgressString = 0;
	T3RenderStateBlock* mpEditingRS = 0;
	T3RenderStateBlock* mpEditingRSMask = 0;
	DataStream* mpDeleteAfter = 0;

	T3EffectType mFilterType = WDC_eEffect_None;
	StaticFeatures mFilterFeatures{};
	T3EffectQuality mFilterQuality = eEffectQuality_Default;
	int mNewFilterFeature = 0;

	int selectedShaderNode = -1;
	int selectedVariant = -1;
	LoadedProgramPass* mpOpenPass = 0;
	bool mbOpenPassCompute = false;

	enum
	{
		NONE = 0,
		BUCKET = 1,
		VAR = 2,//ignore
		NEW_BUCKET= 3,
		RENDER_STATE = 4,
		SHADER = 5,
	} selectedNodeType = NONE;

	struct _EditingBucket {

		T3EffectCacheParams mParams;
		T3EffectType mEffect;
		std::string mMatCRC;
		int newStaticFeat = 0;
		DynamicFeatures newVariantFeatures;
		int newDynamicFeat = 0;

		inline _EditingBucket(const T3EffectCacheContext& localContext) : mParams(localContext), mEffect((T3EffectType)0), mMatCRC("") {}

	} mEditingBucket;

	//UNSAFE WHEN LOADING VARIABLES:
	bool mbLoadResult = false;
	T3EffectCacheContext mLocalCacheContext;
	std::vector<LoadedShader> mShaders;
	std::vector<LoadedBucket> mBuckets;

	static void AsyncDoLoad(void* inst, void* a, void* b);

	static void AsyncDoSave(void* inst, void* a, void* b);

	void SetStream(const char* pGameID, DataStream* _pFxStream);

	virtual void _render() override;

	inline ~ShaderTask() {
		if (mpDeleteAfter)
			delete mpDeleteAfter;
		mpDeleteAfter = 0;
	}

	inline deleteme(ShaderTask)

	inline taskctor(ShaderTask), mEditingBucket(mLocalCacheContext), mpDeleteAfter(0) {}

};

// ==================================================================================== SIMPLE JOB SYSTEM ====================================================================================

enum JobPriority {
	PRIORITY_DISALLOW_TASK_EXIT = 99999
};

struct Job {

	void(*mpFn)(void* pInstUserData, void* userDataA, void* userDataB);
	void* userA, * userB, * mpUserInstance;
	int priority = 1;
	int handle = 0;
	std::string name = "";

};

inline bool operator<(const Job& a, const Job& b) {
	return a.priority < b.priority;
}

class JobScheduler {
public:

	static JobScheduler* mpScheduler;

	static inline void Initialize(){
		if (!mpScheduler)
			mpScheduler = new JobScheduler;
	}

	static inline void Shutdown(){
		if (mpScheduler)
			delete mpScheduler;
		mpScheduler = 0;
	}

	static inline JobScheduler& Scheduler() {
		return *mpScheduler;
	}

	std::vector<std::thread> threadPool{};
	std::priority_queue<Job, std::vector<Job>, std::less<Job>> pendingJobs{};
	std::mutex lock{};
	std::atomic_bool* pRunningGuard = 0;
	HANDLE sem = INVALID_HANDLE_VALUE;
	std::vector<int> aliveJobs{};
	int runningHnd = 0;

	JobScheduler();
	~JobScheduler();

	//returns job ID
	int PostJob(InspectorTask* pMyTask, Job job);

	bool Finished(int handle);

	static void ThreadProc(JobScheduler* me, int threadIndex, std::atomic_bool* pRunning);

};

// ==================================================================================== MORE UTILS ====================================================================================

//should be set to 'String'
inline void ComboSelectType(const char*& selectedType){
	if (ImGui::BeginCombo("##combo1", selectedType))
	{
		for (int n = 0; n < (int)sRuntime.filterListMSTypes.size(); n++)
		{
			bool is_selected = !strcmp(selectedType, sRuntime.filterListMSTypes[n]);
			if (ImGui::Selectable(sRuntime.filterListMSTypes[n], is_selected)) {
				selectedType = sRuntime.filterListMSTypes[n];
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
				selectedType = sRuntime.filterListMSTypes[n];
			}
		}
		ImGui::EndCombo();
	}
}

inline void register_lua(const char* name, LuaCClosure fn) {
	lua_pushcclosure(sRuntime.L, fn, 0);
	lua_setglobal(sRuntime.L, name);
}

inline void ensure_reg(const char* fnname, LuaCClosure fn) {
	u64 hash = CRC64_CaseInsensitive(0, fnname);
	for (auto& h : sRuntime.registered_fns)
		if (h == hash)
			return;
	sRuntime.registered_fns.push_back(hash);
	register_lua(fnname, fn);
}

inline ImVec4 operator*(const ImVec4& vec, const float mult) {
	return ImVec4{ vec.x * mult, vec.y * mult, vec.z * mult, vec.w };
}

inline ImVec2 operator+(const ImVec2& vec, const float value) {
	return ImVec2{ vec.x + value, vec.y + value };
}

inline ImVec2 operator+(const ImVec2& vec, const ImVec2 value) {
	return ImVec2{ vec.x + value.x, vec.y + value.y };
}

inline void operator-=(ImVec2& vec, const float value) {
	vec.x -= value;
	vec.y -= value;
}

inline ImVec2 operator-(const ImVec2& vec, const float value) {
	return ImVec2{ vec.x - value, vec.y - value };
}

inline ImVec2 operator-(const ImVec2& vec, const ImVec2 value) {
	return ImVec2{ vec.x - value.x, vec.y - value.y };
}

inline ImVec2 operator*(const ImVec2& vec, const float value) {
	return ImVec2{ vec.x * value, vec.y * value };
}

inline ImVec2 operator*(const ImVec2& vec, const ImVec2 value) {
	return ImVec2{ vec.x * value.x, vec.y * value.y };
}

class CenteredControlWrapper {
public:
	inline explicit CenteredControlWrapper(bool result) : result_(result) {}

	inline operator bool() const {
		return result_;
	}

private:
	bool result_;
};

class ControlCenterer {
public:
	inline ControlCenterer(ImVec2 windowSize) : windowSize_(windowSize) {}

	template<typename Func>
	CenteredControlWrapper operator()(Func control) const {
		ImVec2 originalPos = ImGui::GetCursorPos();

		// Draw offscreen to calculate size
		ImGui::SetCursorPos(ImVec2(-10000.0f, -10000.0f));
		control();
		ImVec2 controlSize = ImGui::GetItemRectSize();

		// Draw at centered position
		ImGui::SetCursorPos(ImVec2((windowSize_.x - controlSize.x) * 0.5f, originalPos.y));
		control();

		return CenteredControlWrapper(ImGui::IsItemClicked());
	}

private:
	ImVec2 windowSize_;
};

#define IMGUI_CENTERED(control) ControlCenterer{ImGui::GetWindowSize()}([&]() { control; })