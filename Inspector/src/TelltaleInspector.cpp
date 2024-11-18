 #include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "imgui.h"
#include <string>
#include <algorithm>
#include "ToolLibrary/T3/T3Effect.h"

// ==================================================================================== INCLUDE AND MACROS ====================================================================================

//set to true to make exe only build the .HashDB and then quit.
#define APP_MODE_ONLY_CREATE_HASH_DB 0

#if defined(DEBUG) || !defined(NDEBUG)

//Optional (if not set to 'nullptr' this macro can be the type task which is run when the app loads in debug mode to make debuggign a specific file or editor easier.
//for chore eg new TypeTask<Chore>("Chore Editor", sRuntime.gen_id())
//#define DEBUG_INITIAL_TASK() new ShaderTask("Shader Editor",sRuntime.gen_id()); ((ShaderTask*)task)->SetStream("MCSM",_OpenDataStreamFromDisc("C:/Users/lucas/Desktop/MCSM - TXMESH/MCSM_pc_Shaders_mem.t3fxpack",READ))

//new GameEditorTask("Game Editor", sRuntime.gen_id()); ((GameEditorTask*)task)->Init("d:/games/minecraft story mode - season two"); ((GameEditorTask*)task)->bSelecting = false; ((GameEditorTask*)task)->mpGameID = "MC2"

//new TypeTask<Chore>("Chore Editor", sRuntime.gen_id(), "MC2", _OpenDataStreamFromDisc("c:/users/lucas/desktop/env_caveWonders_cs_Enter_1.chore", READ))

#define DEBUG_INITIAL_TASK() /*ALWAYS NULLPTR FOR THIS LINE*/ nullptr

#else
#define DEBUG_INITIAL_TASK() /*ALWAYS NULLPTR FOR THIS LINE*/ nullptr
#endif
/*

	THIS FILE WAS WRITTEN SOLELY BY LUCAS SARAGOSA FOR THE TELLTALE INSPECTOR APP. THIS IS IN NO WAY AFFILIATED WITH TELLTALE GAMES.

	PLEASE IGNORE THE GODAWFUL CODE PRACTICE. I WANT THIS APP TO PROGRESS QUICKLY AS PRIORITY !!

	TO READ THROUGH CODE, IN VISUAL STUDIO USE Ctrl+M+O to collapse all.

*/

#ifndef BUILDING_INSPECTOR
#error "Must define BUILDING_INSPECTOR in the preprocessor to building the inspector"
#endif
#include "Walnut/Image.h"
#include "InspectorTemplate.h"
#include "ToolLibrary/TelltaleToolLibrary.h"
#include "ToolLibrary/Meta.hpp"
#include "ToolLibrary/MetaInitHelpers.h"
#include "ToolLibrary/Blowfish.h"
#include "ToolLibrary/TTArchive2.hpp"
#include "ToolLibrary/types/SoundReverbDefinition.h"
#include <fstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include "imstd/imgui_stdlib.h"
#include "nfd.h"
#include <sstream>
#include "ToolLibrary/Lua/lua.hpp"
extern "C" {
#include "ToolLibrary/Lua/decompile.h"
#include "ToolLibrary/lua/proto.h"
}
#include <intrin.h>
#include <filesystem>
#include "ToolLibrary/FMOD/FSB5.h"
#include "ToolLibrary/MetaStream_JSON.hpp"
#include "ToolLibrary/VersDB.h"
#include "ToolLibrary/LegacyTypes/D3DMeshLegacy.h"
#include <unordered_map>
#include "stb/std_image_write.h"
#include "squish/squish.h"
#include "imstd/imgui_memedit.h"
#include "ToolLibrary/Types/Scene.h"
#include "ToolLibrary/Types/AnimOrChore.h"
#include "ToolLibrary/Base64.h"
#include "ToolLibrary/T3/T3EffectCache.h"
#include "ToolLibrary/types/Dialog.h"
#include <imgui_node_editor.h>
#include "TelltaleInspector.h"
#include "GameEditor/GameEditor.hpp"

// ==================================================================================== SYMBOL MAP ====================================================================================

void SymbolMap::load(const char* path) {
	strmap.clear();
	DataStreamFile_PlatformSpecific ds = _OpenDataStreamFromDisc_(path, READ);
	MetaStream stream{};
	stream.Open(&ds, MetaStreamMode::eMetaStream_Read, {});
	stream.mbDontDeleteStream = true;
		
	u32 magic{};
	char buf[0x7FF + 1];
	memset(buf, 0, 0x7FF + 1);
	stream.serialize_uint32(&magic);
	if(magic == 0xDEADBEEF){
		u32 num{};
		stream.serialize_uint32(&num);
		if(num < 0x7FFFF){
			for(u32 i = 0; i < num; i++){
				u32 len{};
				stream.serialize_uint32(&len);
				if (len > 0x7FF)
					return;
				stream.serialize_bytes(buf, len);
				buf[len] = 0;
				strmap.push_back(std::string(buf));
			}
		}
	}

	
}

void SymbolMap::save(const char* path) {
	DataStreamFile_PlatformSpecific ds = _OpenDataStreamFromDisc_(path, WRITE);
	MetaStream stream{};
	stream.Open(&ds, MetaStreamMode::eMetaStream_Write, {});
	stream.mbDontDeleteStream = true;

	u32 magic = 0xDEADBEEF;
	stream.serialize_uint32(&magic);

	u32 num = (u32)strmap.size();
	stream.serialize_uint32(&num);

	for(auto& s : strmap){
		u32 l = (u32)s.length();
		stream.serialize_uint32(&l);
		stream.serialize_bytes((void*)s.c_str(), l);
	}

	stream.Close();
}

bool SymbolMap::resolve(u64 hash, std::string& dest) {
	for (u32 i = 0; i < map_s; i++) {
		if (map[i] == hash) {
			dest = strmap[i];
			return true;
		}
	}
	u32 rt = strmap.size() - map_s;
	for (u32 i = 0; i < rt; i++) {
		u64 test = CRC64_CaseInsensitive(0, strmap[map_s + i].c_str());
		if (test == hash) {
			dest = strmap[map_s + i];
			return true;
		}
	}
	dest = "";
	return false;
}

void SymbolMap::insert(std::string& val) {
	for(auto it = strmap.begin(); it!=strmap.end();it++){
		if (StringCompareCaseInsensitive().operator()(*it, val))
			return;
	}
	strmap.push_back(val);
}

SymbolMap::SymbolMap() {
	map = 0;
	map_s = 0;
}

SymbolMap::~SymbolMap() {
	if (map)
		delete[] map;
	map = 0;
}

void SymMap_InsertUserHash(const char* str, resolve_mode type) {
	if (type == prop_key) {
		std::string v{ str };
		sRuntime.symbolMaps[prop_key].insert(v);
	}
	else if (type == file_name) {
		std::string v{ str };
		sRuntime.symbolMaps[file_name].insert(v);
	}
}

std::string* SymMap_ResolveHash(char* resolve_buf, std::unordered_map<u64, std::string>& db, std::vector<int>& dbFilenames, u64 hash, resolve_mode md) {
	if (hash == 0)
		return nullptr;
	if (true) {
		std::string& resolved = db[hash];
		if (resolved == "__UNKHPRXY__")//enough to be used in ss buffer
			return nullptr;//not found
		if (resolved.length() == 0) {
			//find the new hash
			if (md == check_all) {
				resolve_buf[0] = 0;
				TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(0, hash, resolve_buf);
				if (resolve_buf[0] == 0) {
					//could not find
					if (sRuntime.symbolMaps[prop_key].resolve(hash, resolved)) {
						db[hash] = resolved;
					}
					else if (sRuntime.symbolMaps[file_name].resolve(hash, resolved)) {
						db[hash] = resolved;
					}
					else {
						resolved = "__UNKHPRXY__";
						db[hash] = resolved;
						return nullptr;
					}
				}
				else {
					resolved = resolve_buf;
					db[hash] = resolved;
				}
			}
			else if (md == prop_key) {
				resolve_buf[0] = 0;
				TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(
					TelltaleToolLib_GetGlobalHashDatabase()->FindPage("PropertySet-Keys"), hash, resolve_buf);
				if (resolve_buf[0] == 0) {
					//could not find
					//check user
					if (sRuntime.symbolMaps[prop_key].resolve(hash, resolved)) {
						db[hash] = resolved;
					}
					else {
						resolved = "__UNKHPRXY__";
						db[hash] = resolved;
						return nullptr;
					}
				}
				else {
					resolved = resolve_buf;
					db[hash] = resolved;
				}
			}
			else if (md == file_name) {
				if (sRuntime.symbolMaps[file_name].resolve(hash, resolved)) {
					db[hash] = resolved;
					return &resolved;
				}
				HashDatabase* _db = TelltaleToolLib_GetGlobalHashDatabase();
				if (dbFilenames.size() == 0) {
					char tmp[32];
					sprintf(tmp, "%s", TelltaleToolLib_GetBlowfishKey());
					std::transform(tmp, tmp + strlen(tmp), tmp, ::toupper);
					//find all pages name 'Files_GAMENAME_***'
					dbFilenames.reserve(50);//around ~50 types of files
					HashDatabase::Page* pg = nullptr;
					int idx = 0;
					do {
						if (idx >= _db->NumPages())
							break;
						pg = _db->PageAt(idx);
						if (ends_with(pg->mPageName, tmp)) {
							dbFilenames.push_back(idx);
						}
						idx++;
					} while (pg != nullptr);
				}
				resolve_buf[0] = 0;
				for (auto pgit = dbFilenames.begin(); pgit != dbFilenames.end(); pgit++) {
					HashDatabase::Page* pg = _db->PageAt(*pgit);
					_db->FindEntry(pg, hash, resolve_buf);
					if (resolve_buf[0] != 0) {
						resolved = resolve_buf;
						db[hash] = resolved;
						return &resolved;
					}
				}
				resolved = "__UNKHPRXY__";
				db[hash] = resolved;
				return nullptr;
			}
			else
				return nullptr;//wtf
			return &resolved;
		}
		else
			return &resolved;//already resolved
	}
	return nullptr;
}


// ==================================================================================== FORWARD DECLS ====================================================================================

InspectorRuntime sRuntime{};

int luaRegisterSet(lua_State* L);
int luaAddBuildVersionInfo(lua_State* st);

void SaveSettings();

u64 renderProxyID = 0xDEADBEEF;

// ==================================================================================== UTIL TASKS ====================================================================================

InspectorTask::InspectorTask(const char* name, int id) : task_name(name), task_id(id) {
	SaveSettings();//save settings each new task
}

void InspectorTask::render() {
	ImVec2 wpos = ImGui::GetWindowPos();
	ImGui::SetNextWindowPos(ImVec2{ wpos.x + 50, wpos.y + 50 }, ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2{ 600, 500 });
	char id[100];
	sprintf(id, "%s###%d", task_name.c_str(), task_id);
	if (ImGui::Begin(id, 0, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
		if (ImGui::Button("Exit Task")) {
			if (HasAsyncDependencies()) {
				MessageBoxA(0, "You cannot end this task yet! There are async tasks being executed. Please wait and try again in a bit.", "Whoa there!", MB_ICONEXCLAMATION);
			}
			else {
				sRuntime.pending_deletions.push_back(this);
				ImGui::End();
				return;
			}
		}
		_render();
		ImGui::End();
	}
}

void SaveSettings() {
	std::filesystem::path settingsPath = std::filesystem::current_path();
	settingsPath += "/UserSettings/InspectorSettings.ttcfg";
	MetaStream stream{};
	DataStreamFileDisc fstream = _OpenDataStreamFromDisc_(settingsPath.string().c_str(), WRITE);
	stream.Open(&fstream, MetaStreamMode::eMetaStream_Write, {});
	stream.mbDontDeleteStream = true;
	if (stream.mbErrored) {
		MessageBoxA(0, "Could not open settings to save..", "Error in shutdown", MB_ICONERROR);
		exit(-1);
	}
	MetaClassDescription* settingsDesc = InspectorSettings::GetMetaClassDescription();
	stream.mVersionInfo.push_back(MetaVersionInfo{settingsDesc->mHash, SerializedVersionInfo::RetrieveCompiledVersionInfo(settingsDesc)->mVersionCrc});
	if (PerformMetaSerializeFull(&stream, &sRuntime.settings, settingsDesc) != MetaOpResult::eMetaOp_Succeed) {
		MessageBoxA(0, "Could not save settings", "Error in detach", MB_ICONERROR);
		exit(-1);
	}
#ifdef _DEBUG
	TTL_Log("-Saved user settings\n");
#endif
	stream.Close();
}

void LogHook(const char* const _Fmt, va_list va);

static inline ImVec4 ImLerp(const ImVec4& a, const ImVec4& b, float t) { return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }

static void InternalSetStyle(ImVec3 stColour) {
	ImVec3 darkGrey = ImVec3{ 0.15f,0.15f,0.15f };
	ImVec3 vdarkGrey = ImVec3{ 0.11f,0.11f,0.11f };
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(darkGrey, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(vdarkGrey, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(vdarkGrey, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(vdarkGrey, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(stColour, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(stColour, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(stColour, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(darkGrey, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(stColour, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.52f, 0.18f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(stColour, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(darkGrey, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(stColour, 0.4f);
	colors[ImGuiCol_ButtonActive] = ImVec4(stColour, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(stColour, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(stColour, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(stColour, 1.00f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(stColour, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(stColour, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(stColour, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(stColour, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(stColour, 0.95f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.90f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(stColour, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(stColour, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(stColour, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

static ImVec3 UIColors[] {
	{23.f / 255.f, 66.f / 255.f, 135.f / 255.f},//b
	{36.f / 255.f, 62.f / 255.f, 37.f / 255.f},//g
	{134.f / 255.f, 4.f / 255.f, 4.f / 255.f},//r
	{134.f / 255.f, 52.f / 255.f, 4.f / 255.f},//o
	{134.f / 255.f, 100.f / 255.f, 4.f / 255.f},//y
	{104.f / 255.f, 4.f / 255.f, 132.f / 255.f}//p
};

class TelltaleInspector : public Walnut::Layer
{
public:


	static void* l_alloc(void* ud, void* ptr, size_t osize,
		size_t nsize) {
		(void)ud;  (void)osize;  /* not used */
		if (nsize == 0) {
			free(ptr);
			return NULL;
		}
		else
			return realloc(ptr, nsize);
	}

	static int luapanic(lua_State* L) {
		const char* err = lua_tolstring(L, -1, 0);
		TTL_Log("LUA ERROR: %s\n", err);
		return 0;
	}

	static bool srt_mcd(const MetaClassDescription* lhs, const MetaClassDescription* rhs) {
		if (lhs->mbIsIntrinsic != rhs->mbIsIntrinsic)
			return lhs->mbIsIntrinsic;
		std::less<std::string> strc{};
		return strc(lhs->mpTypeInfoName, rhs->mpTypeInfoName);
	}

	virtual void OnAttach() {
		//INIT
		TelltaleToolLib_Initialize(NULL);
		TelltaleToolLib_SetLoggerHook(&LogHook);

		//LOAD LIBRARY DATA ARCHIVES
		DataStreamFileDisc* dataArchive = OpenDataStreamFromDisc("TTL_Inspector200_ALL_data.ttarch2", READ);
		bool bDebug = false;
#ifdef DEBUGMODE
		//bDebug = true; skip for now to test archife works
#endif
		if (bDebug || dataArchive->IsInvalid()) {
			delete dataArchive;
			if (!bDebug)
				TTL_Log("WARN: Attempting to use database folders instead of database archive as it could be found");
			DataStreamFileDisc* db = _OpenDataStreamFromDisc("./_Dev/Database/ToolLibrary.HashDB", READ);
			if (db->IsInvalid()) {
				delete db;
				MessageBoxA(NULL, "Could not locate the hash database "
					"(ToolLibrary.HashDB). Ensure the installation of this tool is correct!", "Installation Error", MB_ICONERROR);
				exit(-1);
			}
			TelltaleToolLib_SetGlobalHashDatabaseFromStream(db);
			TTL_Log("Using TelltaleToolLib(modified) v%s\n", TelltaleToolLib_GetVersion());
			if (_setmaxstdio(8192) == -1) {
				MessageBoxA(NULL, "Could not set max stdio limit, please contact me with your machine information."
					" This will mean that you cannot add over ~500 custom files to an archive at a time!", "Error", MB_ICONERROR);
			}
			std::filesystem::path dbs = std::filesystem::current_path();
			dbs += "/_Dev/Database";
			int numVers = TelltaleToolLib_SetProxyVersionDatabases(dbs.string().c_str());
			if (numVers == -1) {
				MessageBoxA(NULL, "Could not load some of the Version Databases. "
					"Please ensure the installation of this tool is correct!", "Installation Error", MB_ICONERROR);
				exit(-1);
			}
			else {
				TTL_Log("Found and loaded %d version databases\n", numVers);
			}
			dbs = std::filesystem::current_path();
			dbs += "/_Dev/Caches";
			TelltaleToolLib_LoadEffectCaches(dbs.string().c_str());
		}else{
			TTArchive2 archive{};
			archive.Activate(dataArchive);
			if(!archive.mbActive){
				MessageBoxA(NULL, "The inspector data archive could not be loaded! Please contact me ASAP."
					"Please ensure the installation of this tool is correct!", "Installation Error", MB_ICONERROR);
				exit(-1);
			}
			TTL_Log("Found and loaded %d version databases\n", TelltaleToolLib_SetProxyVersionDatabasesFromArchive(archive));
			DataStream* pHashDB = archive.GetResourceStream(archive._FindResource("ToolLibrary.HashDB"));
			if(!pHashDB){
				MessageBoxA(NULL, "Could not find the library hash database. Please contact me."
					"Please ensure the installation of this tool is correct!", "Installation Error", MB_ICONERROR);
				exit(-1);
			}
			DataStreamMemory* pCachedHashDB = new DataStreamMemory(pHashDB->GetSize());
			if(!pHashDB->Copy(pCachedHashDB, 0, 0, pHashDB->GetSize())){
				MessageBoxA(NULL, "Could not read bytes from the library hash database. Please contact me."
					"Please ensure the installation of this tool is correct!", "Installation Error", MB_ICONERROR);
				exit(-1);
			}
			pCachedHashDB->SetMode(DataStreamMode::eMode_Read);
			pCachedHashDB->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
			delete pHashDB;
			TelltaleToolLib_SetGlobalHashDatabaseFromStream(pCachedHashDB);
			TelltaleToolLib_LoadEffectCachesFromArchive(archive);
		}
		const char* last = TelltaleToolLib_GetLastError();
		if (last)
			TTL_Log("Last Error: %s\n", last);
		sRuntime.L = lua_newstate(&l_alloc, 0);
		luaL_openlibs(sRuntime.L);
		lua_atpanic(sRuntime.L, &luapanic);
		luadec_setstate(sRuntime.L);
		InitOperators();

		// LOAD SYMBOL MAPS
		std::filesystem::path dbs = std::filesystem::current_path();
		dbs = std::filesystem::current_path();
		dbs += "/UserSettings/User_Files.symmap";
		std::filesystem::path files = dbs;
		dbs = std::filesystem::current_path();
		dbs += "/UserSettings/User_PropKeys.symmap";
		std::filesystem::path keys = dbs;
		if (std::filesystem::exists(files)) {
			sRuntime.symbolMaps[file_name].load(files.string().c_str());
		}
		if (std::filesystem::exists(keys)) {
			sRuntime.symbolMaps[prop_key].load(keys.string().c_str());
		}

		//CREATE META TYPE LIST STRING CACHES
		std::stringstream tmp{};
		MetaClassDescription* pDesc = TelltaleToolLib_GetFirstMetaClassDescription();
		while (pDesc) {
			if (!pDesc->mbHiddenInternal) {
				sRuntime.mcd_map.push_back(pDesc);
				TelltaleToolLib_GetNextMetaClassDescription(&pDesc);
			}
		}
		std::sort(sRuntime.mcd_map.begin(), sRuntime.mcd_map.end(), &srt_mcd);
		sRuntime.filterListMSTypes.reserve(sRuntime.mcd_map.size());
		for (auto it : sRuntime.mcd_map) {
			sRuntime.filterListMSTypes.push_back(it->mpTypeInfoName);
			if(it->mpExt)
				tmp << ";" << it->mpExt;
		}
		sRuntime.filterListMSFiles = tmp.str().substr(1);

		//LOAD INSPECTOR SETTINGS
		std::filesystem::path settingsPath = std::filesystem::current_path();
		settingsPath += "/UserSettings/InspectorSettings.ttcfg";
		if (std::filesystem::exists(settingsPath)) {
			if (std::filesystem::file_size(settingsPath) == 0) {
				sRuntime.settings.mbShownIntroMessage = true;
			}
			else {
				MetaStream stream{};
				DataStreamFileDisc fstream = _OpenDataStreamFromDisc_(settingsPath.string().c_str(), READ);
				stream.Open(&fstream, MetaStreamMode::eMetaStream_Read, {});
				stream.mbDontDeleteStream = true;
				if (stream.mbErrored) {
					MessageBoxA(0, "Could not open settings please delete settings file.", "Error in init", MB_ICONERROR);
					exit(-1);
				}
				MetaClassDescription* settingsDesc = InspectorSettings::GetMetaClassDescription();
				if (PerformMetaSerializeFull(&stream, &sRuntime.settings, settingsDesc) != eMetaOp_Succeed) {
					MessageBoxA(0, "Could not read settings please delete settings file.", "Error in init", MB_ICONERROR);
					exit(-1);
				}
#ifdef _DEBUG
				TTL_Log("-Loaded user settings\n");
#endif
			}
		}
		
		//SHOW THE USER INTRO MESSAGE IF NEEDED
		if(!sRuntime.settings.mbShownIntroMessage){
			ULONGLONG start = GetTickCount64();
#define WAIT_TIME 30
#define WAIT_TIME_S "30" 
			while((GetTickCount64() - start) < WAIT_TIME * 1000)
			{
				MessageBoxA(0, "You can exit this after " WAIT_TIME_S " seconds. It is vital you read all these message boxes before any sort of modding."
					" This is your first use of this app (or version of it), in which it is designed to let you mod games by Telltale Games (games preceding and not including The Expanse). "
					APP_HELP_DESC
					, "Welcome to the Telltale Inspector", MB_ICONINFORMATION);
			}

			if(MessageBoxA(0, "CONTACT ME! Any bugs (I am aware of lots) or queries, message me on the modding discord"
				"  tagging @lucassaragosa. Please click YES to get an invite to the discord.", "Contact information", MB_YESNO) == IDYES){
				ShellExecuteA(NULL, "open", "https://discord.com/invite/HqpnTenqwp", NULL, NULL, SW_SHOWNORMAL);
			}
			MessageBoxA(0, "Last bit of information! This was made by discord @lucassaragosa and any questions ask me staff modders on the discord."
				" It is NOT a good idea to convert files between different games using this tool. Your files will likely break the game.", "!!", MB_ICONINFORMATION);
			MessageBoxA(0, "BE WARNED! This app is in continual development and will have bugs. Please donate to keep interest! HAPPY MODDING", "WARNING!", MB_ICONINFORMATION);
			sRuntime.settings.mbShownIntroMessage = true;
		}
		JobScheduler::Initialize();
		InspectorTask* task = DEBUG_INITIAL_TASK();
		if (task)
			sRuntime.open_tasks.push_back(task);
		InternalSetStyle(UIColors[sRuntime.settings.mUICol]);
	}

	virtual void OnDetach() {
		JobScheduler::Shutdown();
		std::filesystem::path dbs;
		dbs = std::filesystem::current_path();
		dbs += "/UserSettings/User_Files.symmap";
		std::filesystem::path files = dbs;
		dbs = std::filesystem::current_path();
		dbs += "/UserSettings/User_PropKeys.symmap";
		std::filesystem::path keys = dbs;
		sRuntime.symbolMaps[file_name].save(files.string().c_str());
		sRuntime.symbolMaps[prop_key].save(keys.string().c_str());
		SaveSettings();
		lua_close(sRuntime.L);
		TelltaleToolLib_Free();
	}

	virtual void OnUIRender() override
	{
		auto pending_deletions = std::move(sRuntime.pending_deletions);//cache, any addded are done next frame
		for (auto it = pending_deletions.begin(); it != pending_deletions.end(); it++) {
			InspectorTask* task = *it;
			auto at = std::find(sRuntime.open_tasks.begin(), sRuntime.open_tasks.end(), task);
			sRuntime.open_tasks.erase(at);
			if (task->mpDestroyCallback != 0 && ((task->mbCallDestroySlot != -1 && sRuntime.gates[task->mbCallDestroySlot] == 1) || (task->mbCallDestroySlot == -1))) {
				task->mpDestroyCallback(task, task->mpUserData);
			}
			task->_deleteme();
		}
		for (auto it = sRuntime.queued.begin(); it != sRuntime.queued.end(); it++)
			sRuntime.open_tasks.push_back(*it);
		sRuntime.queued.clear();
		for (auto it : sRuntime.open_tasks) {
			it->render();
		}
		//console window
		ImVec2 wpos = ImGui::GetWindowPos();
		ImGui::SetNextWindowPos(ImVec2{ wpos.x + 50, wpos.y + 50 }, ImGuiCond_FirstUseEver);
		//ImGui::SetNextWindowSize(ImVec2{ 600, 500 });
		if (sRuntime.settings.mbShowConsole && ImGui::Begin("Console", 0, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
			if(ImGui::Button("Clear Console")){
				std::unique_lock g { sRuntime.ItemsLock };
				sRuntime.Items.clear();
			}
			ImGui::Separator();
			{
				const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
				if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
				{
					std::unique_lock g{ sRuntime.ItemsLock };
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
					for (int i = 0; i < sRuntime.Items.size(); i++)
					{
						const char* item = sRuntime.Items[i].c_str();
						ImVec4 color;
						bool has_color = false;
						if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
						else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
						if (has_color)
							ImGui::PushStyleColor(ImGuiCol_Text, color);
						ImGui::TextUnformatted(item);
						if (has_color)
							ImGui::PopStyleColor();
					}
					ImGui::PopStyleVar();
				}
				ImGui::EndChild();
			}
			ImGui::End();
		}
	}

};

void LogHook(const char* const _Fmt, va_list va) {
	char buf[1024]{ 0 };
	int n;
	if((n=vsprintf_s(buf, _Fmt, va))>0){
		bool ex = n > 0 && buf[n - 1] != '\n';
		if (ex)
			printf("\n");
		std::unique_lock _{ sRuntime.ItemsLock };
		std::string text = buf;
		if (ex)
			text += "\n";
		sRuntime.Items.push_back(std::move(text));
	}
}

class ContactTask : public InspectorTask {

	virtual void _render() override {
		const char* tt = "https://discord.gg/SPdn5yZr6M";
		const char* mcsm = "https://discord.gg/AzEfQCNj9S";
		ImGui::Text("Contact me on discord at: lucassaragosa#0152");
		ImGui::NewLine();
		ImGui::Text("Contact me on instagram at: lukassaragosa");
		ImGui::NewLine();
		ImGui::Text("Why not join the Telltale Community?");
		if (ImGui::Button("Modding Discord")) {
			ShellExecuteA(NULL, "open", tt, NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::NewLine();
		ImGui::Text("Fan of MCSM? Join the MCSM rebuilt server run by my team! Mod talk there too:");
		if (ImGui::Button("MCSM Rebuilt Discord")) {
			ShellExecuteA(NULL, "open", mcsm, NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::NewLine();
		ImGui::Text("Thank you for downloading my tool. Please follow my github!! (code there aswell): ");
		if (ImGui::Button("My Github")) {
			ShellExecuteA(NULL, "open", "https://github.com/LucasSaragosa", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::NewLine();
		ImGui::Text("Special thanks: Simon Pinfold (FSB5 format), RandomTBush (some D3DMesh old formats), David M & Violet, Telltale Games");
	}

public:

	deleteme(ContactTask)

	taskctor(ContactTask){}

};

class HelpTask : public InspectorTask {

	virtual void _render() override {
		ImGui::Text("This tool by me (Lucas Saragosa) is a modding tool for all (newer) Telltale Games.");
		ImGui::NewLine();
		ImGui::Text("This works for all games newer (and including) The Wolf Among Us (S1). This tool is only for games");
		ImGui::NewLine();
		ImGui::Text("made under the original Telltale Games (not skybound) engine (Telltale Tool).");
		ImGui::NewLine();
		ImGui::Text(APP_HELP_DESC);
	}

public:

	deleteme(HelpTask)

	taskctor(HelpTask) {}

};

ContainerTask::ContainerTask(const char* tt, int id) : InspectorTask(tt, id) {
	container = 0;
}

ContainerTask::~ContainerTask() {
	if (container)
		delete container;
}

void ContainerTask::_render() {
	if (file.length() == 0) {
		file = open_file(0);
		DataStreamContainerParams p{};
		p.mpSrcStream = OpenDataStreamFromDisc(file.c_str(), READ);
		container = new DataStreamContainer(p);
		container->Read(0, &size);
		if (!container->ok) {
			MessageBoxA(0, "Could not open the given container, it is invalid !", "Error opening file", MB_ICONERROR);
		}
	}
	if (ImGui::Button("Extract Data")) {
		nfdchar_t* o = 0;
		if (NFD_SaveDialog(0, 0, &o, L"Select output file") == NFD_OKAY) {
			DataStreamFile_PlatformSpecific out = _OpenDataStreamFromDisc_(o, WRITE);
			container->Transfer(&out, 0, size);
			free(o);
			MessageBoxA(0, "Successfully extracted data stream container data!", "Finished", MB_ICONINFORMATION);
		}
	}
}


template<typename T>
void TypeTask<T>::_render() {
	if (mpTypeDesc == 0)
		mpTypeDesc = GetMetaClassDescription<T>();
	if (mpType) {
		::RenderTypeData<T>(this, _delegated_init_flag);
		_delegated_init_flag = false;
	}
	else {
		if (error != 0) {
			ImGui::Text("Could not open the file: %s", error);
			if (ImGui::Button("Reset")) {
				id = error = 0;
				imported = false;
				versionInfo.mVersionInfo.clear();
				mpTypeDesc->Destroy(&_mType);
				mpTypeDesc->Construct(&_mType);
				if (pUserData)
					delete pUserData;
				pUserData = 0;
			}
			return;
		}
		id = select_gameid_dropdown(id);
		if (id != 0) {
			if (imported) {
				if (ImGui::Button("Save File")) {
					nfdchar_t* p{};
					if (NFD_SaveDialog(mpTypeDesc->mpExt, 0, &p) == NFD_OKAY) {
						std::string path = p;
						free(p);
						DataStreamFile_PlatformSpecific in = _OpenDataStreamFromDisc_(path.c_str(), WRITE);
						MetaStream stream{};
						stream.InjectVersionInfo(versionInfo);
						stream.mbDontDeleteStream = true;
						stream.Open(&in, MetaStreamMode::eMetaStream_Write, {});
						if (stream.mbErrored) {
							MessageBoxA(0, "Could not open stream", "Error opening file", MB_ICONERROR);
						}
						else {
							if (PerformMetaSerializeAsync(&stream, &_mType) != eMetaOp_Succeed) {
								MessageBoxA(0, "Could not export file, please contact me!", "Error", MB_ICONERROR);
							}
							else {
								MessageBoxA(0, "Successfully exported!", "Success", MB_ICONINFORMATION);
							}
						}
					}
				}
				::RenderTypeData<T>(this, _delegated_init_flag);
				_delegated_init_flag = false;
			}
			else {
				if (ImGui::Button("Import File")) {
					nfdchar_t* p{};
					if (NFD_OpenDialog(mpTypeDesc->mpExt, 0, &p) == NFD_OKAY) {
						std::string path = p;
						free(p);
						DataStreamFile_PlatformSpecific in = _OpenDataStreamFromDisc_(path.c_str(), READ);
						MetaStream stream{};
						stream.mbDontDeleteStream = true;
						stream.Open(&in, MetaStreamMode::eMetaStream_Read, {});
						if (stream.mbErrored) {
							MessageBoxA(0, "Invalid meta stream", "Error opening file", MB_ICONERROR);
						}
						else {
							if (PerformMetaSerializeAsync(&stream, &_mType) != eMetaOp_Succeed) {
								MessageBoxA(0, "Could not import file, please contact me!", "Error", MB_ICONERROR);
							}
							else {
								imported = true;
								versionInfo.InjectVersionInfo(stream);
								::RenderTypeData<T>(this, true);
							}
						}
					}
				}
				if (ImGui::Button("New From Scratch")) {
					imported = true;
					::RenderTypeData<T>(this, true);
				}
			}
		}
	}
}

// ==================================================================================== UTIL FUNCTIONS NOT COMPILED IN MAIN EXE. ====================================================================================

void _L(const char* m, float f){
	printf("-%f%%: %s\n", f, m);
}

void PackageInspectorFiles() {
	std::vector<TTArchive2::ResourceCreateEntry> entries{};
	std::filesystem::path dbs = std::filesystem::current_path();
	dbs += "\\_Dev\\Database";
	for(auto& file : std::filesystem::directory_iterator{ dbs }){
		if (!file.is_regular_file())
			continue;
		TTArchive2::ResourceCreateEntry& e = entries.emplace_back();
		e.name = file.path().filename().string();
		e.open_later = file.path().string();
	}
	dbs = std::filesystem::current_path();
	dbs += "\\_Dev\\Caches";
	for (auto& file : std::filesystem::directory_iterator{ dbs }) {
		if (!file.is_regular_file())
			continue;
		TTArchive2::ResourceCreateEntry& e = entries.emplace_back();
		e.name = file.path().filename().string();
		e.open_later = file.path().string();
	}
	DataStreamFileDisc out = _OpenDataStreamFromDisc_("TTL_Inspector200_ALL_data.ttarch2", WRITE);
	TTArchive2::Create(&_L, &out, entries, false, true, Compression::Library::OODLE, 2);
	printf("-done!\n");
	exit(0);
}

void walkthrough_files(const char* game, const char* id) {
	std::string out = "C:\\Users\\lucas\\Desktop\\vers\\unk\\";

	out += id;
	out += ".txt";

	std::map<u64, std::list<std::string>> unknowns{};
	std::list<std::string> unknownTypes{};
	std::list<std::string> emptyUnkFiles{};
	std::string name{};

	TelltaleToolLib_SetBlowfishKey(id);

	for (auto& archp : std::filesystem::directory_iterator(game)) {
		if (ends_with(archp.path().string(), ".ttarch2")) {
			TTL_Log("%s : starting walk\n", archp.path().filename().string().c_str());
			TTArchive2 arch{};
			DataStream* stream = OpenDataStreamFromDisc(archp.path().string().c_str(), READ);
			if (((DataStreamFile_PlatformSpecific*)stream)->mHandle == 0) {
				TTL_Log("too large, %s TODO", archp.path().string().c_str());
				continue;
			}
			arch.Activate(stream);
			DataStream* pResource{};
			for (auto& file : arch.mResources) {
				name = arch.GetResourceName(file.mNameCRC);
				if (ends_with(name, "bank") || ends_with(name, "lua") ||
					ends_with(name, "lenc") || ends_with(name, "wav") || ends_with(name, "ogg"))
					continue;
				pResource = arch.GetResourceStream(&file);
				MetaStream temp{};
				temp.Open(pResource, MetaStreamMode::eMetaStream_Read, {});

 				if (temp.mVersionInfo.size() == 0) {
					getname(name, arch, &file, archp);
					emptyUnkFiles.push_back(_STD move(name));
				}
				else {
					u64 first = temp.mVersionInfo[0].mTypeSymbolCrc;
					bool found = TelltaleToolLib_FindMetaClassDescription_ByHash(first) ? 1 : 0;
					if (!found) {
						getname(name, arch, &file, archp);
						unknownTypes.push_back(_STD move(name));
					}
					else if(temp.mVersionInfo.size() > 1) {
						for (int i = 1; i < temp.mVersionInfo.size(); i++) {
							first = temp.mVersionInfo[i].mTypeSymbolCrc;
							found = TelltaleToolLib_FindMetaClassDescription_ByHash(first) ? 1 : 0;
							if (!found) {
								getname(name, arch, &file, archp);
								unknowns[first].push_back(_STD move(name));
							}
						}
					}
				}

				//delete pResource; stream takes it
			}
			if (arch.mResources.size() == 0) {
				TTL_Log("%s : no files or invalid\n", archp.path().filename().string().c_str());
			}
			else {
				TTL_Log("%s : done\n", archp.path().filename().string().c_str());
			}
		}
	}
	TTL_Log("dumping...\n");
	std::ofstream os{ out };
	os << "\n\n\n\nUnknown Empty Files" << std::endl;
	for (auto& it : emptyUnkFiles) {
		os << it << std::endl;
	}
	os << "\n\n\n\n\n\n\n\n\n\n\n\n\n\nUnknown Types" << std::endl;
	for (auto& it : unknownTypes) {
		os << it << std::endl;
	}os << "\n\n\n\n\n\n\n\n\n\n\n\nUnknown Embedded Types" << std::endl;
	for (auto& it : unknowns) {
		os << "Hash: " << std::hex << it.first << std::endl;
		for (auto& f : it.second) {
			os << "\t - " << f << "\n";
		}
	}
	os.close();
	TTL_Log("\ndone!\n");
}

std::string _gen_rand() {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(12);
	tmp_s += "M_";

	for (int i = 0; i < 10; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return tmp_s;
}

void dump_game_classes_init_code(const char* id) {
	TelltaleToolLib_SetBlowfishKey(id);
	TelltaleVersionDatabase* pDB = TelltaleToolLib_GetProxyMetaState(id)->mpStateGameDB;
	std::ofstream out{ std::string{"c:/users/lucas/desktop/vers/_codedump/"} + id + std::string{".txt"}, std::ios_base::out };
	std::string cur{};
	std::stringstream con{};
	std::vector<std::string> broken_down{};
	TelltaleVersionDatabase::ClassEntry* pClass{};
	for (int i = 0; i < pDB->mClasses.mSize; i++) {
		pClass = pDB->mClasses.mpStorage + i;
		//if (TelltaleToolLib_FindMetaClassDescription_ByHash(pClass->mHash) != nullptr)
			;// continue;
		out << "\n\n";
		std::string niceNamee = MetaVersion_ConvertTypeName((const char*)pClass->mpTypeInfoName);
		if (niceNamee._Starts_with("DCArray") || niceNamee._Starts_with("List") || niceNamee._Starts_with("Map") || niceNamee._Starts_with("Set")
			|| niceNamee._Starts_with("DArray") || niceNamee._Starts_with("Deque") || niceNamee._Starts_with("Handle")
			|| niceNamee._Starts_with("KeyframedValue") || niceNamee._Starts_with("AnimatedValueInterface")) {
			con << "\t\t\t// " << niceNamee << "\n";
			continue;
		}
		out << "\t\t\tDEFINET2(" << (cur=_gen_rand()) << ", " << niceNamee << ");\n";
		u32 flags = pClass->mFlags;
		flags &= ~(Internal_MetaFlag_Initialized | Internal_MetaFlag_IsProxied);
		if (flags)
			out << "\t\t\tADDFLAGS(" << cur << ", 0x" << std::hex << flags << std::dec << ");\n";
		if (pClass->mpExt)
			out << "\t\t\tEXT(" << cur << ", " << (const char*)pClass->mpExt << ");\n";
		TelltaleVersionDatabase::MemberEntry* pMember{}, * pPrevious{0};
		TTL_Log("- write code: %s\n", (const char*)pClass->mpTypeInfoName);
		if (pClass->mpFirstMember != -1) {
			pMember = pDB->mMembers.mpStorage + pClass->mpFirstMember;
			while (pMember != nullptr) {
				std::string member = (const char*)pMember->mpName;
				std::string niceName = MetaVersion_ConvertTypeName((const char*)((pDB->mClasses.mpStorage + pMember->mpMemberDesc)->mpTypeInfoName));
				if (pPrevious == nullptr)
					out << "\t\t\tFIRSTMEM";
				else
					out << "\t\t\tNEXTMEM";
				if (member._Starts_with("Baseclass") || member.find('[') != std::string::npos) {
					out << "1(" << cur << ", \"" << member << "\", " << member << ", ";
					if (pPrevious == nullptr) {
						if (pMember->mpNextMember == -1) {
							out << "ERROR_NO_MEMBER_ALIAS_AVAILABLE";
						}
						else {
							TelltaleVersionDatabase::MemberEntry* next = pDB->mMembers.mpStorage + pMember->mpNextMember;
							out << (const char*)next->mpName;
						}
					}
					else {
						//multiple baseclasses might fuck this up?
						out << (const char*)pPrevious->mpName;
					}
				}
				else {
					out << "2(" << cur << ", " << member;
				}
				out << ", " << niceNamee << ", /*" << niceName << "*/, 0x";
				out << std::hex << pMember->mFlags << std::dec;
				if (pPrevious != nullptr)
					out << ", " << (const char*)pPrevious->mpName;
				out << ");\n";

				if (pMember->mpEnumDesc != -1) {
					TelltaleVersionDatabase::EnumEntry* pEnum{}, * pPrevious{ 0 };
					pEnum = pDB->mEnums.mpStorage + pMember->mpEnumDesc;
					std::string prev_id{};
					std::string id{};
					while (pEnum) {

						if (pPrevious)
							out << "\t\t\tNEXTENUM2(";
						else
							out << "\t\t\tFIRSTENUM2(";
						out << cur << ", " << member << ", \"" << (const char*)pEnum->mpEnumName << "\", ";
						id = _gen_rand();
						out << id << ", " << pEnum->mEnumValue << ", " << std::hex << pEnum->mFlags << std::dec;
						if (pPrevious)
							out << ", " << prev_id;
						out << ");\n";

						pPrevious = pEnum;
						prev_id = id;
						if (pEnum->mpNext != -1)
							pEnum = pDB->mEnums.mpStorage + pEnum->mpNext;
						else
							pEnum = nullptr;
					}
				}
				else if (pMember->mpFlagDesc != -1) {
					TelltaleVersionDatabase::FlagEntry* pFlag{}, * pPrevious{ 0 };
					pFlag = pDB->mFlags.mpStorage + pMember->mpFlagDesc;
					std::string prev_id{};
					std::string id{};
					while (pFlag) {

						if (pPrevious)
							out << "\t\t\tNEXTFLAG(";
						else
							out << "\t\t\tFIRSTFLAG(";
						out << cur << ", " << member << ", \"" << (const char*)pFlag->mpFlagName << "\", ";
						id = _gen_rand();
						out << id << ", 0x" << std::hex << pFlag->mFlagValue << std::dec;
						if (pPrevious)
							out << ", " << prev_id;
						out << ");\n";

						pPrevious = pFlag;
						prev_id = id;
						if (pFlag->mpNext != -1)
							pFlag = pDB->mFlags.mpStorage + pFlag->mpNext;
						else
							pFlag = nullptr;
					}
				}

				pPrevious = pMember;
				if (pMember->mpNextMember != -1)
					pMember = pDB->mMembers.mpStorage + pMember->mpNextMember;
				else
					pMember = 0;
			}
		}
		out << "\t\t\tADD(" << cur << ");\n";
	}
	out << "\n\n" << con.str();
	TTL_Log("-done\n");
}

void dump_game_classes(const char* id) {
	TelltaleToolLib_SetBlowfishKey(id);
	TelltaleVersionDatabase* pDB = TelltaleToolLib_GetProxyMetaState(id)->mpStateGameDB;
	std::ofstream out{ std::string{"c:/users/lucas/desktop/vers/_typesdump/"} + id + std::string{".txt"}, std::ios_base::out};
	std::stringstream enums{};
	TelltaleVersionDatabase::ClassEntry* pClass{};
	for (int i = 0; i < pDB->mClasses.mSize; i++) {
		pClass = pDB->mClasses.mpStorage + i;
		if (TelltaleToolLib_FindMetaClassDescription_ByHash(pClass->mHash) != nullptr)
			continue;
		out << "\n\n";
		if (pClass->mpExt)
			out << "//" << (const char*)pClass->mpExt << " FILES\n";
		out << (const char*)pClass->mpTypeInfoName << " {\npublic:\n\n";
		TelltaleVersionDatabase::MemberEntry* pMember{};
		TTL_Log("- write %s\n", (const char*)pClass->mpTypeInfoName);
		if (pClass->mpFirstMember!=-1) {
			pMember = pDB->mMembers.mpStorage + pClass->mpFirstMember;
			while (pMember != nullptr) {
				std::string niceName = MetaVersion_ConvertTypeName((const char*)((pDB->mClasses.mpStorage + pMember->mpMemberDesc)->mpTypeInfoName));
				out << "\t" << niceName << " " << (const char*)pMember->mpName << ";\n";

				if (pMember->mpEnumDesc!=-1) {
					enums << "enum " << (const char*)((pDB->mClasses.mpStorage + pMember->mpMemberDesc)->mpTypeInfoName) << " {\n\n";
					TelltaleVersionDatabase::EnumEntry* pEnum{};
					pEnum = pDB->mEnums.mpStorage + pMember->mpEnumDesc;
					while (pEnum) {

						enums << "\t" << (const char*)pEnum->mpEnumName << " = " << pEnum->mEnumValue << ",\n";

						if (pEnum->mpNext!=-1)
							pEnum = pDB->mEnums.mpStorage + pEnum->mpNext;
						else
							pEnum = nullptr;
					}
					enums << "\n};\n\n\n";
				}
				else if (pMember->mpFlagDesc!=-1) {
					enums << "/*FLAGS*/ enum " << (const char*)((pDB->mClasses.mpStorage + pMember->mpMemberDesc)->mpTypeInfoName) << " {\n\n";
					TelltaleVersionDatabase::FlagEntry* pFlag{};
					pFlag = pDB->mFlags.mpStorage + pMember->mpFlagDesc;
					while (pFlag) {

						enums << "\t"  << (const char*)pFlag->mpFlagName << " = 0x" << std::hex << pFlag->mFlagValue << std::dec << ",\n";

						if (pFlag->mpNext!=-1)
							pFlag = pDB->mFlags.mpStorage + pFlag->mpNext;
						else
							pFlag = nullptr;
					}
					enums << "\n};\n\n\n";
				}

				if (pMember->mpNextMember!=-1)
					pMember = pDB->mMembers.mpStorage + pMember->mpNextMember;
				else
					pMember = 0;
			}
		}
		out << "\n};";
	}
	out << "\n\n\n";
	out << enums.str();
	TTL_Log("-done\n");
}

/*class Task : public InspectorTask {

	virtual void _render() override {

	}

public:

	deleteme(Task)

	taskctor(Task){}

};*/

// ==================================================================================== APPLICATION ====================================================================================

void _CreateHashDatabase(){
	bool bDoCreate = APP_MODE_ONLY_CREATE_HASH_DB;
	if(bDoCreate){
		{
			DataStreamFile_PlatformSpecific out = _OpenDataStreamFromDisc_("Database/ToolLibrary.HashDB", WRITE);
			if (HashDatabase::Create("_Dev/LIBTT_DB.txt", &out, true, true, false))
				TTL_Log("-Created database\n");
			else
				TTL_Log("-Creation failed\n");
		}
		exit(1);
	}
}

void TestStuff();

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)       
{
	//PackageInspectorFiles();
	//exit(1);
	// TEST APP RUNS.

	//TelltaleToolLib_Initialize("mcsm");
	//TelltaleToolLib_SetProxyVersionDatabases("c:/vunder/telltale/telltale tool - mods/walnut/walnutapp/Database");
	//dump_game_classes_init_code("mcsm");
	/*dump_game_classes("batman");
	dump_game_classes("batman2");
	dump_game_classes("mc2");
	dump_game_classes("marvel");
	dump_game_classes("wd3");
	dump_game_classes("wd4");
	dump_game_classes("sammaxremaster");
	dump_game_classes("mcsm");
	dump_game_classes("thrones");
	dump_game_classes("michonne");
	dump_game_classes("borderlands");*/
	//exit(1);
	//TelltaleToolLib_Initialize(NULL);
	//walkthrough_files("d:/games/the walking dead - definitive series/archives/", "wdc");
	//return 0;

	// INSPECTOR ENTRY POINT

	//TestStuff();
	//exit(0);

	static bool tryingopen = false;
	Walnut::ApplicationSpecification spec;
	spec.Name = "Telltale Inspector " APP_VERSION;

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<TelltaleInspector>();
	app->SetMenubarCallback([app]()
	{
		if(ImGui::BeginPopupModal("Select UI Color")){
			if(ImGui::BeginCombo("##colo",ColorName[sRuntime.settings.mUICol])){
				for (int i = 0; i < NUM_UI_COLORS; i++) {
					if(ImGui::Selectable(ColorName[i])){
						sRuntime.settings.mUICol = (InspectorCol)i;
						InternalSetStyle(UIColors[i]);
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::Button("Done"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
;		}
		static const char* gameid_ = nullptr;
		if (tryingopen) {
			ImGui::OpenPopup("Select Game");
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2{ 500, 150 });
		}
		if (ImGui::BeginPopupModal("Select Game")) {
			ImGui::Text("This is the game which the meta stream file is part of.");
			gameid_ = select_gameid_dropdown(gameid_);
			if (gameid_ != nullptr && ImGui::Button("Select")) {
				tryingopen = false;
				ImGui::CloseCurrentPopup();
				nfdpathset_t set{};
				if (NFD_OpenDialogMultiple(sRuntime.filterListMSFiles.c_str(), 0, &set) == nfdresult_t::NFD_OKAY) {
					const char* gameid = gameid_;
					int num = NFD_PathSet_GetCount(&set);
					if (num > 25) {
						MessageBoxA(0, "You cannot open more than 25 files at a time, this is to ensure the program doesn't crash.", "Too many!",
							MB_ICONERROR);
					}
					else {
						for (int i = 0; i < num; i++) {
							nfdchar_t* path = NFD_PathSet_GetPath(&set, i);
							if (TelltaleToolLib_FindMetaClassDescription(std::filesystem::path{path}.extension().string().substr(1).c_str(),false)
								 == nullptr) {
								TTL_Log("-unknown file type %s IGNORING\n", path);
							}else if (std::filesystem::is_regular_file(path)) {
								DataStream* pStream = _OpenDataStreamFromDisc(path, READ);
								if (!pStream)
									continue;
								sRuntime.open_tasks.push_back(new MetaStreamTask{
									pStream, std::filesystem::path{path}.filename().string(), gameid });
							}
						}
					}
					NFD_PathSet_Free(&set);
				}
				gameid_ = nullptr;
			}
			ImGui::EndPopup();
		}
		if (ImGui::BeginMenu("Inspector"))
		{
			if (sRuntime.settings.mbShowConsole && ImGui::MenuItem("Hide Console Window")) {
				sRuntime.settings.mbShowConsole = false;
			}else if(!sRuntime.settings.mbShowConsole && ImGui::MenuItem("Show Console Window")){
				sRuntime.settings.mbShowConsole = true;
			}
			if (ImGui::MenuItem("CRT: Dump Memory Leaks"))
				_CrtDumpMemoryLeaks();
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("New")) {
			if (ImGui::MenuItem("TTArchive Task")) {
				InspectorTask* task = new ArchiveTask{ "TTArchive", sRuntime.gen_id() };
				sRuntime.open_tasks.push_back(task);
			}
			if (ImGui::MenuItem("Lua Converter Task")) {
				InspectorTask* task = new LuaTask{ "Lua Converter", sRuntime.gen_id() };
				sRuntime.open_tasks.push_back(task);
			}
			if (ImGui::MenuItem("Soundbank Extractor Task")) {
				InspectorTask* task = new BankTask{ "FMOD Soundbank Extract", sRuntime.gen_id() };
				sRuntime.open_tasks.push_back(task);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Info")) {
			if (ImGui::MenuItem("Help")) {
				InspectorTask* task = new HelpTask{ "Help", sRuntime.gen_id() };
				sRuntime.open_tasks.push_back(task);
			}
			if (ImGui::MenuItem("Contact Me")) {
				InspectorTask* task = new ContactTask{ "Contact Info", sRuntime.gen_id()   };
				sRuntime.open_tasks.push_back(task);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Open")) {
			if (ImGui::MenuItem("Open Meta Stream File(s)")) {
				tryingopen = true;		
			}
			if (ImGui::MenuItem("Open Full Game Editor")) {
				if(sRuntime.current_editor_task){
					MessageBoxA(0, "You already have a full game editor task open! Only one can be open a time!", "Task type instance open", MB_ICONWARNING);
				}
				else {
					nfdchar_t* folder{};
					MessageBoxA(0, "In the file dialog, please select the FOLDER which contains the .exe of the telltale game."
						" This folder should contain a folder called Pack or Archives. If the game is older and it has folders called "
						"'Episode 1[XX] - ...' then select *that* folder, that contains the executable for *that* episode! ", "Note", MB_OK);
					if (NFD_PickFolder(0, &folder, L"Select Game Directory (with .exe inside)") == NFD_OKAY) {
						std::filesystem::path p{ folder };
						GameEditorTask* task = new GameEditorTask{ "Game Editor", sRuntime.gen_id() };
						task->Init(p);
						sRuntime.open_tasks.push_back(task);
					}
				}
			}
			if (ImGui::MenuItem("Extract Container Stream")) {
				ContainerTask* task = new ContainerTask("Extract Container", sRuntime.gen_id());
				sRuntime.open_tasks.push_back(task);
			}
			if (ImGui::MenuItem("Open D3DMesh")) {
				MeshTask* task = new MeshTask("D3DMesh Editor", sRuntime.gen_id());
				sRuntime.open_tasks.push_back(task);
			}
			if (ImGui::MenuItem("Open T3Texture")) {
				TextureTask* task = new TextureTask("Texture Editor", sRuntime.gen_id());
				sRuntime.open_tasks.push_back(task);
			}
			if (ImGui::MenuItem("Open Prop")) {
				PropTask* task = new PropTask("Property Editor", sRuntime.gen_id());
				sRuntime.open_tasks.push_back(task);
			}
			if (ImGui::MenuItem("Open Scene")) {
				SceneTask* task = new SceneTask("Scene Editor", sRuntime.gen_id());
				sRuntime.open_tasks.push_back(task);
			}
			if (ImGui::MenuItem("Open Shader")) {
				ShaderTask* task = new ShaderTask("Shader Editor", sRuntime.gen_id());
				sRuntime.open_tasks.push_back(task);
			}
			if (ImGui::MenuItem("Open Chore")) {
				TypeTask<Chore>* task = new TypeTask<Chore>("Chore Editor", sRuntime.gen_id());
				sRuntime.open_tasks.push_back(task);
			}
			if (ImGui::MenuItem("Open Animation [WIP]")) {
				TypeTask<Animation>* task = new TypeTask<Animation>("Animation Data Viewer", sRuntime.gen_id());
				sRuntime.open_tasks.push_back(task);
			}
			/*if(ImGui::MenuItem("Open Dialog")){
				TypeTask<Dlg>* task = new TypeTask<Dlg>("Dialog Editor", sRuntime.gen_id());
				sRuntime.open_tasks.push_back(task);
			}*/
			ImGui::EndMenu();
		}
		bool o = false;
		if(ImGui::BeginMenu("Set UI Color")){
			o = 1;
			ImGui::EndMenu();
		}
		if(o)
			ImGui::OpenPopup("Select UI Color");
	});
	return app;
}

void PropTaskAdapterFn(PropertySet& p, void* ud) {
	// .
}

void AnyTypeTask::_render(){
	mATE.Render(mpType->mpTypeInfoName, mpType, mpInstance);
}

void DeleteSceneCb(Scene* pScene) {
	GetMetaClassDescription<Scene>()->Delete(pScene);
}

InspectorTask* CreateAndOpenTypeTask(const char* embedName, MetaClassDescription* typeClass, const char* parentName, const char* gameID, void* typeInstance, InspectorTask_DestroyCb cb, void* ud, int slot){
	if (typeClass == GetMetaClassDescription<Chore>()) {
		InspectorTask* task = new TypeTask<Chore>(parentName, embedName, sRuntime.gen_id(), gameID, (Chore*)typeInstance);
		task->SetDestroyCallback(cb, ud, slot);
		sRuntime.open_tasks.push_back(task);
		return task;
	}else if(typeClass == GetMetaClassDescription<Scene>()){
		SceneTask* task = new SceneTask((Scene*)typeInstance, DeleteSceneCb, embedName, sRuntime.gen_id());
		task->SetDestroyCallback(cb, ud, slot);
		task->mbOwnScene = false;
		task->id = gameID;
		task->state = true;
		sRuntime.open_tasks.push_back(task);
		return task;
	}
	else if (typeClass == GetMetaClassDescription<Animation>()) {
		TypeTask<Animation>* task = new TypeTask<Animation>(parentName, embedName, sRuntime.gen_id(), gameID, (Animation*)typeInstance);
		task->SetDestroyCallback(cb, ud, slot);
		sRuntime.open_tasks.push_back(task);
		return task;
	}
	else if (typeClass == GetMetaClassDescription<T3Texture>()) {
		TextureTask* task = new TextureTask(embedName, sRuntime.gen_id());
		task->SetDestroyCallback(cb, ud, slot);
		sRuntime.open_tasks.push_back(task);
		task->loaded = true;
		task->mpTexture = (T3Texture*)typeInstance;
		task->ok = true;
		return task;
	}else if(typeClass == GetMetaClassDescription<PropertySet>()){
		PropTask* task = new PropTask("Property Editor", sRuntime.gen_id());
		task->SetDestroyCallback(cb, ud, slot);
		sRuntime.queued.push_back(task);
		bool* open_gate = (bool*)&sRuntime.gates[sRuntime.gen_gate_slot()];
		task->set_prop(std::string(embedName), open_gate, gameID, &PropTaskAdapterFn, (PropertySet*)typeInstance, 0);
		return task;
	}else{
		InspectorTask* pTask = new AnyTypeTask(embedName, sRuntime.gen_id(), typeClass, typeInstance);
		pTask->SetDestroyCallback(cb, ud, slot);
		sRuntime.open_tasks.push_back(pTask);
		return pTask;
	}
}
