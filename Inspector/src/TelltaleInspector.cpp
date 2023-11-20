#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "imgui.h"
#include <string>
#include <algorithm>

/*

	THIS FILE WAS WRITTEN SOLELY BY LUCAS SARAGOSA FOR THE TELLTALE INSPECTOR APP. THIS IS IN NO WAY AFFILIATED WITH TELLTALE GAMES.

	PLEASE IGNORE THE GODAWFUL CODE PRACTICE. I WANT THIS APP TO PROGRESS QUICKLY AS PRIORITY !!

	TO READ THROUGH CODE, IN VISUAL STUDIO USE Ctrl+M+O to collapse all.

*/

#ifndef BUILDING_INSPECTOR
#error "Must define BUILDING_INSPECTOR in the preprocessor to building the inspector"
#endif
#include "Walnut/Image.h"
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
#include "ToolLibrary/FSB5.h"
#include "ToolLibrary/MetaStream_JSON.hpp"
#include "ToolLibrary/VersDB.h"
#include "ToolLibrary/LegacyTypes/D3DMeshLegacy.h"
#include <unordered_map>
#include "stb/std_image_write.h"
#include "squish/squish.h"
#include "imstd/imgui_memedit.h"
#include "ToolLibrary/types/Scene.h"

inline void* operator new(size_t s) {
	return (void*)calloc(1, s);
}

#define APP_VERSION "ALPHA v1.0.2"

#define APP_HELP_DESC " Use the toolbar to open windows to mod specific files. This app can at the moment: "\
"edit prop files, create and extract TTARCH2 archives, extract .BANK sound data to OGG files, open any file raw data and export to JSON, "  \
"convert textures to PNG, extract shader bundles (t3fxpack), convert encrypt/decrypt lua/lenc and in bulk, convert and edit meshes and more. "\
"More tools will come in the future. Do not rush or you will make mistakes! Read all information I give you and ASK on discord if anything is wrong."\
" Most tools will require you to select a game before you input the files. You MUST ensure the file(s) are from the right game or the app will likely crash. This also"\
" means that the app could interpret the file wrong which may cause it to use *many* gigabytes of ram mistakenly and this may even crash your PC so BE CAREFUL."\
" Please also note, old games before the wolf among us will not work in this tool (they may work for few small files.) This app should be used mainly for "\
"new games such as the walking dead collection, etc."

enum InspectorSettingsFlags {
	eISF_ShownTTArchMessage = 1
};

struct InspectorSettings {

	bool mbShownIntroMessage = false;
	Flags mFlags;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		return result;
	}

	static MetaClassDescription* GetMetaClassDescription() {
		static MetaClassDescription meta_set{};
		if (meta_set.mFlags.mFlags & Internal_MetaFlag_Initialized)
			return &meta_set;
		MetaClassDescription& meta_flags = *::GetMetaClassDescription<Flags>();
		MetaClassDescription& meta_bool = *::GetMetaClassDescription<bool>();
		{
			DEFINET_3(set, InspectorSettings);
			meta_set.mbHiddenInternal = true;
			EXT(set, ttcfg);
			FIRSTMEM2(set, mbShownIntroMessage, InspectorSettings, bool, 0);
			NEXTMEM2(set, mFlags, InspectorSettings, flags, 0, mbShownIntroMessage);
			SERIALIZER(set, InspectorSettings);
			ADD(set);
			return &meta_set;
		}
	}

};

bool starts_with(const char* pre, const char* str)
{
	return strncmp(pre, str, strlen(pre)) == 0;
}

inline bool ends_with(const std::string& value, std::string ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}


template< typename T >
std::string to_symbol(T i)
{
	std::stringstream stream;
	stream << "Symbol<"
		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i << ">";
	return stream.str();
}

enum resolve_mode {
	file_name = 0,
	prop_key = 1,
	check_all = 2,
};

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

	void load(const char* path) {
		strmap.clear();
		DataStreamFile_Win ds = _OpenDataStreamFromDisc_(path, READ);
		MetaStream stream{};
		stream.Open(&ds, MetaStreamMode::eMetaStream_Read, {});
		stream.mbDontDeleteStream = true;
		
		u32 num{};
		stream.serialize_uint32(&num);

		if (map)
			delete[] map;
		map = new u64[num];
		map_s = num;
		_Serialized* tmp = (_Serialized*)new char[sizeof(_Serialized) * num];
		stream.serialize_bytes((void*)tmp, sizeof(_Serialized) * num);

		strmap.clear();
		strmap.reserve(num);
		for (u32 i = 0; i < num; i++) {
			map[i] = tmp[i].get();
			std::string& s = strmap.emplace_back();
			s.resize(tmp[i].len);
			stream.serialize_bytes(s.data(), tmp[i].len);
		}
		delete[] (char*)tmp;
	}

	void save(const char* path) {
		DataStreamFile_Win ds = _OpenDataStreamFromDisc_(path, WRITE);
		MetaStream stream{};
		stream.Open(&ds, MetaStreamMode::eMetaStream_Write, {});
		stream.mbDontDeleteStream = true;

		u32 num = (u32)strmap.size();
		stream.serialize_uint32(&num);

		_Serialized* tmp = (_Serialized*)new char[sizeof(_Serialized) * num];
		for (u32 i = 0; i < num; i++) {
			tmp[i].len = (u16)(strmap[i].length() & 0xFFFF);
			u64 hsh = CRC64_CaseInsensitive(0, strmap[i].c_str());
			*((u64*)tmp[i].hash) = hsh;
		}
		stream.serialize_bytes((void*)tmp, sizeof(_Serialized) * num);
		delete[](char*)tmp;
		for (u32 i = 0; i < num; i++) {
			stream.serialize_bytes(strmap[i].data(), strmap[i].length());
		}

		stream.Close();
	}

	bool resolve(u64 hash, std::string& dest) {
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

	void insert(std::string& val) {
		strmap.push_back(val);
	}

	SymbolMap() {
		map = 0;
		map_s = 0;
	}

	~SymbolMap() {
		if (map)
			delete[] map;
		map = 0;
	}

};

#define taskctor(T) T(const char* name, int id) : InspectorTask(name, id)
#define deleteme() virtual void _deleteme() override { delete this; } void mark_as_todelete() { sRuntime.pending_deletions.push_back(this);}

class InspectorTask;
class GameEditorTask;

static struct {
	int running_id = 0;
	std::string game_id = "";
	std::vector<InspectorTask*> open_tasks;
	std::vector<InspectorTask*> pending_deletions;
	std::vector<u64> registered_fns;
	lua_State* L;
	GameEditorTask* current_editor_task = nullptr;
	int gen_id() {
		return ++running_id;
	}

	std::string filterListMSFiles{};

	std::vector<const char*> filterListMSTypes{};
	std::vector<MetaClassDescription*> mcd_map{};
	std::vector<InspectorTask*> queued;

	std::vector<int> gates{};

	SymbolMap symbolMaps[2];

	InspectorSettings settings;

} sRuntime;

int luaRegisterSet(lua_State* L);
int luaAddBuildVersionInfo(lua_State* st);

typedef int (*LuaCClosure)(lua_State*);

static void register_lua(const char* name, LuaCClosure fn) {
	lua_pushcclosure(sRuntime.L, fn, 0);
	lua_setglobal(sRuntime.L, name);
}

static void ensure_reg(const char* fnname, LuaCClosure fn) {
	u64 hash = CRC64_CaseInsensitive(0, fnname);
	for (auto& h : sRuntime.registered_fns)
		if (h == hash)
			return;
	sRuntime.registered_fns.push_back(hash);
	register_lua(fnname, fn);
}

const char* select_gameid_dropdown(const char* last) {
	const char* selected = last;
	if (last)
		last = sBlowfishKeys[TelltaleToolLib_GetGameKeyIndex(last)].game_name;
	if (ImGui::BeginCombo("##none",last ? last : "Select")) {
		for (int i = 52; i < KEY_COUNT; i++) {//52 an on (wolf among us and newer)
			if (ImGui::Selectable(sBlowfishKeys[i].game_name)) {
				selected = sBlowfishKeys[i].game_id;
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	return selected;
}

void SaveSettings();

class InspectorTask {
protected:

	std::string task_name;
	int task_id;

	virtual void _render() = 0;

public:

	virtual void _deleteme() = 0;

	virtual ~InspectorTask() {}

	InspectorTask(const char* name, int id) : task_name(name), task_id(id) {
		SaveSettings();//save settings each new task
	}

	void render() {
		ImVec2 wpos = ImGui::GetWindowPos();
		ImGui::SetNextWindowPos(ImVec2{ wpos.x + 50, wpos.y + 50 }, ImGuiCond_FirstUseEver);
		//ImGui::SetNextWindowSize(ImVec2{ 600, 500 });
		char id[100];
		sprintf(id, "%s###%d", task_name.c_str(), task_id);
		if (ImGui::Begin(id, 0, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
			if (ImGui::Button("Exit Task")) {
				sRuntime.pending_deletions.push_back(this);
				ImGui::End();
				return;
			}
			_render();
			ImGui::End();
		}
	}

};

bool iequals(const std::string& a, const std::string& b)
{
	return std::equal(a.begin(), a.end(),
		b.begin(), b.end(),
		[](char a, char b) {
			return tolower(a) == tolower(b);
		});
}

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

bool filet_sorter(const filet& lhs, const filet& rhs) {
	std::less<std::string> strl{};
	return strl(lhs.file, rhs.file);
}

enum exitstrat {
	accept_all,
	ignore_all,
	ask,
};

std::string open_file(const char* filter) {
	nfdchar_t* d{ 0 };
	if (NFD_OpenDialog(filter, 0, &d) == NFD_OKAY) {
		std::string str{ d };
		free(d);
		return std::move(str);
	}
	return "";
}

class ArchiveTask : public InspectorTask {
public:

	const char* selected_game_id = nullptr;
	bool bState = false;
	bool bImported = false;
	bool bCompressedZ = false, bCompressedOodle = false, bEncrypted = false;
	TTArchive2 arch;
	std::vector<filet> file_names;
	int selectedfile = 0;

	deleteme();

	taskctor(ArchiveTask) {}

	static bool getter(void* data, int index, const char** output)
	{
		*output = ((filet*)data)[index].file.c_str();
		return true;
	}

	void import_in_singlefile(const std::filesystem::path& phys, exitstrat& strat) {
		filet inst{ std::move(phys.filename().string()), phys.string() };
		for (auto& f : file_names) {
			if (iequals(f.file, inst.file)) {
				std::stringstream tmp{};
				tmp << "Found a file name conflict. Replace(yes), ignore(no), cancel(select what I do for any more conflicts)? ";
				tmp << inst.file;
				int res = MessageBoxA(0, tmp.str().c_str(), "Name conflict", MB_ICONINFORMATION | MB_YESNOCANCEL);
				if (res == IDCANCEL) {
					if (MessageBoxA(0, "Replace all (yes), ignore all (no)", "Select", MB_ICONINFORMATION | MB_YESNO) == IDYES)
						strat = accept_all;
					else
						strat = ignore_all;
					res = strat == accept_all ? IDYES : IDNO;
				}
				if (res == IDYES) {
					f.path = phys.string();
					return;
				}
				continue;
			}
		}
		file_names.push_back(inst);
	}

	virtual void _render() override {
		ImGui::Text("Archive Game:");
		selected_game_id = select_gameid_dropdown(selected_game_id);
		ImGui::Checkbox("Zlib Compressed", &bCompressedZ);
		ImGui::Checkbox("Oodle Compressed", &bCompressedOodle);
		ImGui::Checkbox("Encrypted", &bEncrypted);
		ImGui::Text("Version should be 0 (TTA2), 1 (TTA3) or 2 (TTA4). Any is OK, but TTA4 is used in most games.");
		ImGui::InputInt("Version", (int*) & arch.mVersion);
		if (arch.mVersion > 2 || arch.mVersion < 0)
			arch.mVersion = 0;
		if (bState) {
			ImGui::Text("Files");
			ImGui::ListBox("##Files_B", &selectedfile, &getter, (void**)file_names.data(), (int)file_names.size());
			if (ImGui::Button("Add File(s)")) {
				MessageBoxA(NULL, "Please note: you can add directories full of files, but directories in selected directories"
					" wont be imported, only the files in the selected ones!", "Information", MB_ICONINFORMATION);
				nfdpathset_t paths{};
				nfdresult_t result = NFD_OpenDialogMultiple(0, 0, &paths);
				if (result == NFD_OKAY) {
					exitstrat strat = exitstrat::ask;
					int num = NFD_PathSet_GetCount(&paths);
					file_names.reserve(file_names.size() + num);
					for (int i = 0; i < num; i++) {
						nfdchar_t* path = NFD_PathSet_GetPath(&paths, i);
						std::filesystem::path phys{ path };
						if (std::filesystem::is_directory(phys)) {
							std::filesystem::directory_iterator iter{ phys };
							for (auto& it : iter) {
								if(std::filesystem::is_regular_file(phys))
									import_in_singlefile(it, strat);
							}
						}
						else if (std::filesystem::is_regular_file(phys)) {
							import_in_singlefile(phys, strat);
						}
						else {
							MessageBoxA(NULL, path, "File/Folder Invalid, not including!", MB_ICONERROR);
							continue;
						}
					}
					std::sort(file_names.begin(), file_names.end(), &filet_sorter);
				}
			}
			if (ImGui::Button("Extract Selected") && file_names.size() > 0) {
				nfdchar_t* outp{};
				if (NFD_PickFolder(0, &outp, L"Select Destination Folder") == NFD_OKAY) {
					std::filesystem::path folder{ outp };
					folder += "/";
					if (file_names[selectedfile].path.size() > 0) {
						std::filesystem::path pth{ file_names[selectedfile].path };
						//just copy
						folder += std::filesystem::path{ file_names[selectedfile].path }.filename().string();
						if (!std::filesystem::copy_file(pth, folder)) {
							MessageBoxA(0, "Could not extract (copy over) the file!", "Error", MB_ICONERROR);
						}
						else {
							MessageBoxA(0, "Successfully extracted (copied) the file!", "Success", MB_ICONINFORMATION);
						}
					}
					else {
						folder += file_names[selectedfile].file;
						DataStreamFileDisc out = _OpenDataStreamFromDisc_(folder.string().c_str(), WRITE);
						bool f = false;
						{
							u64 crc = CRC64_CaseInsensitive(0, file_names[selectedfile].file.c_str());
							for (auto& it : arch.mResources) {
								if (it.mNameCRC == crc) {
									f = true;
									DataStream* pStream = arch.GetResourceStream(&it);
									if (pStream) {
										pStream->Transfer(&out, 0, pStream->GetSize());
									}
									else f = false;
									break;
								}
							}
						}
						if (f) {
							MessageBoxA(0, "Successfully extracted the file!", "Success", MB_ICONINFORMATION);
						}
						else {
							MessageBoxA(0, "There was a problem extracting this file. Contact me!", "Error", MB_ICONERROR);
						}
					}
				}

			}
			if (ImGui::Button("Extract All")) {
				nfdchar_t* outp{};
				if (NFD_PickFolder(0, &outp, L"Select Destination Folder") == NFD_OKAY) {
					int eerr = 0;
					std::filesystem::path folder{ outp };
					folder += "/";
					std::stringstream errs{};
					errs << "The following files could not be extracted:\n";
					std::string err{};
					for (int i = 0; i < file_names.size(); i++) {
						if (file_names[i].path.size() > 0) {
							std::filesystem::path pth{ file_names[i].path };
							//just copy
							std::filesystem::path op{ folder };
							op += std::filesystem::path{ file_names[i].path }.filename().string();
							if (!std::filesystem::copy_file(pth, op)) {
								eerr++;
								err = "Could not copy";
;								errs << std::filesystem::path{ file_names[i].path }.filename().string() << ": " << err << "\n";
							}
							
						}
						else {
							std::filesystem::path file = folder;
							std::string& fname = file_names[i].file;
							file += fname;
							DataStreamFileDisc out = _OpenDataStreamFromDisc_(file.string().c_str(), WRITE);
							bool f = false;
							{
								u64 crc = CRC64_CaseInsensitive(0, fname.c_str());
								for (auto& it : arch.mResources) {
									if (it.mNameCRC == crc) {
										f = true;
										DataStream* pStream = arch.GetResourceStream(&it);
										if (pStream) {
											pStream->Transfer(&out, 0, pStream->GetSize());
										}
										else f = false;
										break;
									}
								}
							}
							if (!f) {
								eerr++;
								err = "Could not extract";
								errs << fname << ": " << err << "\n";
							}
						}				
					}
					if (eerr > 0) {
						MessageBoxA(0, errs.str().c_str(), "Error extracting some of the files", MB_ICONERROR);
					}
					else
						MessageBoxA(0, "Successfully extracted all the files!", "Success", MB_ICONINFORMATION);
				}
			}
			if (ImGui::Button("Remove Selected File")) {
				if(file_names.size() > 0)
					file_names.erase(file_names.begin() + selectedfile);
				selectedfile = 0;
			}
			if (ImGui::Button("Export Archive")) {
				if (file_names.size() == 0)
					return;
				if (bCompressedOodle && bCompressedZ) {
					MessageBoxA(0, "You can only have one compression type!", "!!", MB_ICONHAND);
					return;
				}
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_SaveDialog("ttarch2", 0, &outPath);
				if (result == NFD_OKAY) {
					if (!(sRuntime.settings.mFlags.mFlags & eISF_ShownTTArchMessage)) {
						MessageBoxA(0, "You are about to create an archive. Ensure the compression/encryption"
							" is set, game is set and know that this may take time (~10s to 3-5mins).", "Create", MB_ICONINFORMATION);
						sRuntime.settings.mFlags |= eISF_ShownTTArchMessage;
					}
					int numinvalid = 0;
					std::string op = outPath;
					if (ends_with(op, ".ttarch2"))
						op += ".ttarch2";
					DataStreamFileDisc dst = _OpenDataStreamFromDisc_(op.c_str(), WRITE);
					if (dst.IsInvalid())
						return;
					printf("opening file streams...\n");
					TTArchive2 out{};
					std::vector<TTArchive2::ResourceCreateEntry> entries{};
					entries.reserve(file_names.size());
					for (auto& file : file_names) {
						TTArchive2::ResourceCreateEntry entry{};
						entry.name = file.file;
						if (file.path.size() == 0) {//from other ttarch
							entry.mpStream = new DataStreamSubStream(arch.mpResourceStream, (unsigned __int64)file.sz, file.off);
						}
						else {
							std::filesystem::path fspath{ file.path };
							if (std::filesystem::exists(fspath) && std::filesystem::is_regular_file(fspath)) {
								entry.mpStream = _OpenDataStreamFromDisc(fspath.string().c_str(), READ);
								if (!entry.mpStream || entry.mpStream->IsInvalid()) {
									goto invalid_file;
								}
							}
							else {
							invalid_file:
								numinvalid++;
								continue;
							}
						}
						entries.push_back(_STD move(entry));
					}
					printf("creating archive...\n");
					bool result = out.Create(0, &dst, entries.data(), entries.size(), bEncrypted, bCompressedOodle || bCompressedZ,
						bCompressedOodle ? Compression::Library::OODLE : Compression::Library::ZLIB, arch.mVersion);
					if (!result) {
						MessageBoxA(0, "There was a problem creating the archive. Please contact me! (info tab)", "Error creating archive!", MB_ICONERROR);
						return;
					}
					MessageBoxA(0, "Successfully created the archive!", "Success!", MB_ICONINFORMATION);
				}
			}
		}
		else {
			ImGui::Text("Click open archive to import all files from an existing archive. You can only add one archive, and after that you can add files and directories from your filesystem.");
			ImGui::Text("Click new archive to not import an existing archive, but create your own from scratch.");
			if (ImGui::Button("Open Archive") && selected_game_id) {
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("ttarch2", NULL, &outPath);
				if (result == NFD_OKAY) {
					TelltaleToolLib_SetBlowfishKey(selected_game_id);
					MessageBoxA(NULL, "Attempting to load archive, may take a while (click okay)", "Archive Info", MB_ICONINFORMATION);
					std::filesystem::path inpath{ outPath };
					task_name = inpath.filename().string();
					DataStreamFileDisc* strm = _OpenDataStreamFromDisc(outPath, READ);
					arch.Activate(strm);
					if (!arch.mbActive) {
						MessageBoxA(NULL, "Could not open archive! It may be open in another app. (Check its from the right game!)", "Error loading TTArchive", MB_ICONERROR);
					}
					else {
						DataStreamContainer* pContainer = (DataStreamContainer*)arch.mpInStream;
						bEncrypted = pContainer->IsEncrypted();
						if (pContainer->IsCompressed()) {
							bCompressedOodle = pContainer->GetCompressionLibrary() == Compression::Library::OODLE;
							bCompressedZ = pContainer->GetCompressionLibrary() == Compression::Library::ZLIB;
						}
						else
							bCompressedZ = bCompressedOodle = false;
						bState = true;
						String name{};
						file_names.reserve(arch.mResources.size());
						for (auto& resource : arch.mResources) {
							bool b = arch.GetResourceName(resource.mNameCRC, &name) == NULL;
							file_names.push_back(filet{ std::move(name), {}, resource.mOffset, resource.mSize });
						}
						std::sort(file_names.begin(), file_names.end(), &filet_sorter);
						//DONE!
					}
				}
			}
			else if (ImGui::Button("New Archive") && selected_game_id) {
				bState = true;
				arch.mVersion = 2;
			}
		}
	}

};

void SaveSettings() {
	std::filesystem::path settingsPath = std::filesystem::current_path();
	settingsPath += "/InspectorSettings.ttcfg";
	MetaStream stream{};
	DataStreamFileDisc fstream = _OpenDataStreamFromDisc_(settingsPath.string().c_str(), WRITE);
	stream.Open(&fstream, MetaStreamMode::eMetaStream_Write, {});
	stream.mbDontDeleteStream = true;
	if (stream.mbErrored) {
		MessageBoxA(0, "Could not open settings to save..", "Error in shutdown", MB_ICONERROR);
		exit(-1);
	}
	MetaClassDescription* settingsDesc = InspectorSettings::GetMetaClassDescription();
	if (PerformMetaSerializeFull(&stream, &sRuntime.settings, settingsDesc) != MetaOpResult::eMetaOp_Succeed) {
		MessageBoxA(0, "Could not save settings", "Error in detach", MB_ICONERROR);
		exit(-1);
	}
#ifdef _DEBUG
	printf("-Saved user settings\n");
#endif
	stream.Close();
}

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
		printf("LUA ERROR: %s\n", err);
		return 0;
	}

	static bool srt_mcd(const MetaClassDescription* lhs, const MetaClassDescription* rhs) {
		if (lhs->mbIsIntrinsic != rhs->mbIsIntrinsic)
			return lhs->mbIsIntrinsic;
		std::less<std::string> strc{};
		return strc(lhs->mpTypeInfoName, rhs->mpTypeInfoName);
	}

	virtual void OnAttach() {
		TelltaleToolLib_Initialize(NULL);
		DataStreamFileDisc* db = _OpenDataStreamFromDisc("Database/ToolLibrary.HashDB", READ);
		if (db->IsInvalid()) {
			delete db;
			MessageBoxA(NULL, "Could not locate the hash database "
				"(ToolLibrary.HashDB). Ensure the installation of this tool is correct!", "Installation Error", MB_ICONERROR);
			exit(-1);
		}
		TelltaleToolLib_SetGlobalHashDatabaseFromStream(db);
		printf("Using TelltaleToolLib(modified) v%s\n", TelltaleToolLib_GetVersion());
		if (_setmaxstdio(8192) == -1) {
			MessageBoxA(NULL, "Could not set max stdio limit, please contact me with your machine information."
				" This will mean that you cannot add over ~100 custom files to an archive at a time!", "Error", MB_ICONERROR);
		}
		std::filesystem::path dbs = std::filesystem::current_path();
		dbs += "/Database";
		int numVers = TelltaleToolLib_SetProxyVersionDatabases(dbs.string().c_str());
		if (numVers == -1) {
			MessageBoxA(NULL, "Could not load some of the Version Databases. "
				"Please ensure the installation of this tool is correct!", "Installation Error", MB_ICONERROR);
			exit(-1);
		}
		else {
			printf("Found and loaded %d version databases\n", numVers);
		}
		const char* last = TelltaleToolLib_GetLastError();
		if (last)
			printf("Last Error: %s\n", last);
		sRuntime.L = lua_newstate(&l_alloc, 0);
		luaL_openlibs(sRuntime.L);
		lua_atpanic(sRuntime.L, &luapanic);
		luadec_setstate(sRuntime.L);
		InitOperators();

		dbs = std::filesystem::current_path();
		dbs += "/User_Files.symmap";
		std::filesystem::path files = dbs;
		dbs = std::filesystem::current_path();
		dbs += "/User_PropKeys.symmap";
		std::filesystem::path keys = dbs;
		if (std::filesystem::exists(files)) {
			sRuntime.symbolMaps[file_name].load(files.string().c_str());
		}
		if (std::filesystem::exists(keys)) {
			sRuntime.symbolMaps[prop_key].load(keys.string().c_str());
		}

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

		std::filesystem::path settingsPath = std::filesystem::current_path();
		settingsPath += "/InspectorSettings.ttcfg";
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
				printf("-Loaded user settings\n");
#endif
			}
		}
		if(!sRuntime.settings.mbShownIntroMessage){
			ULONGLONG start = GetTickCount64();
#define WAIT_TIME 30
#define WAIT_TIME_S "30" 
			while((GetTickCount64() - start) < WAIT_TIME * 1000)
			{
				MessageBoxA(0, "You can exit this after " WAIT_TIME_S " seconds. It is vital you read all these message boxes before any sort of modding."
					" This is your first use of this app, which is designed to let you mod games by Telltale Games (games preceding and not including The Expanse). "
					APP_HELP_DESC
					, "Welcome to the Telltale Inspector", MB_ICONINFORMATION);
			}

			if(MessageBoxA(0, "CONTACT ME! Any bugs (I am aware of lots) or queries, message me on the modding discord"
				"  tagging @lucassaragosa. Please click YES to get an invite to the discord.", "Contact information", MB_YESNO) == IDYES){
				ShellExecuteA(NULL, "open", "https://discord.com/invite/HqpnTenqwp", NULL, NULL, SW_SHOWNORMAL);
			}
			MessageBoxA(0, "Last bit of information! This was made by discord @lucassaragosa and any questions ask me or David M/staff modders on the discord."
				" It is NOT a good idea to convert files between different games using this tool. Your files will likely break the game.", "!!", MB_ICONINFORMATION);
			MessageBoxA(0, "BE WARNED! This is an PRE-ALPHA stage of this app and will have many bugs. Please keep this in mind! HAPPY MODDING", "WARNING!", MB_ICONINFORMATION);
			sRuntime.settings.mbShownIntroMessage = true;
		}

	}

	virtual void OnDetach() {
		lua_close(sRuntime.L);
		TelltaleToolLib_Free();
		std::filesystem::path dbs;
		dbs = std::filesystem::current_path();
		dbs += "/User_Files.symmap";
		std::filesystem::path files = dbs;
		dbs = std::filesystem::current_path();
		dbs += "/User_PropKeys.symmap";
		std::filesystem::path keys = dbs;
		sRuntime.symbolMaps[file_name].save(files.string().c_str());
		sRuntime.symbolMaps[prop_key].save(keys.string().c_str());

		SaveSettings();
	}

	virtual void OnUIRender() override
	{
		for (auto it = sRuntime.pending_deletions.begin(); it != sRuntime.pending_deletions.end(); it++) {
			InspectorTask* task = *it;
			auto at = std::find(sRuntime.open_tasks.begin(), sRuntime.open_tasks.end(), task);
			sRuntime.open_tasks.erase(at);
			task->_deleteme();
		}
		for (auto it = sRuntime.queued.begin(); it != sRuntime.queued.end(); it++)
			sRuntime.open_tasks.push_back(*it);
		sRuntime.queued.clear();
		sRuntime.pending_deletions.clear();
		for (auto it : sRuntime.open_tasks) {
			it->render();
		}
	}
};

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

struct imported_file {
	std::string mName;
	DataStreamMemory* mMemoryStream;
	std::filesystem::path mDiskPath;
	bool mbIsMemory = false;

};

class LuaTask : public InspectorTask {

	std::vector<imported_file> mFiles;
	int selectedfile = 0;
	const char* gameid = nullptr;

	static bool getter(void* data, int index, const char** output)
	{
		*output = ((imported_file*)data)[index].mName.c_str();
		return true;
	}

	void import_in_singlefile(const std::filesystem::path& phys, exitstrat& strat) {
		imported_file newfile{};
		newfile.mbIsMemory = false;
		newfile.mName = phys.filename().string();
		new (&newfile.mDiskPath) std::filesystem::path{ phys };
		for (auto& f : mFiles) {
			if (iequals(f.mName, newfile.mName)) {
				std::stringstream tmp{};
				tmp << "Found a file name conflict. Replace(yes), ignore(no), cancel(select what I do for any more conflicts)? ";
				tmp << newfile.mName;
				int res = MessageBoxA(0, tmp.str().c_str(), "Name conflict", MB_ICONINFORMATION | MB_YESNOCANCEL);
				if (res == IDCANCEL) {
					if (MessageBoxA(0, "Replace all (yes), ignore all (no)", "Select", MB_ICONINFORMATION | MB_YESNO) == IDYES)
						strat = accept_all;
					else
						strat = ignore_all;
					res = strat == accept_all ? IDYES : IDNO;
				}
				if (res == IDYES) {
					newfile.mDiskPath = phys;
					return;
				}
				continue;
			}
		}
		mFiles.push_back(_STD move(newfile));
	}

	static bool is_raw_script(char* buffer, u64 size) {
		if (size > 1000)
			size = 1000;//cap it here, HUGELY small probability its encrypted but lovely and printable
		for (u64 i = 0; i < size; i++) {
			if (buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\t' && !std::isprint(buffer[i])) {
				if (buffer[i] != '\x1B')
					printf("invalid character in script, assuming not raw: %c\n", buffer[i]);
				return false;
			}
		}
		return true;
	}

#define toproto(L,i) getproto(L->top+(i))

	static const char* reader(lua_State* L, void* ud, size_t* sizep)
	{
		static u64 size = 0;
		if ((void*)L == NULL) {
			size = *sizep;
		}
		else if (size > 0) {
			const char* ret = (const char*)ud;
			*sizep = size;
			size = 0;
			return ret;
		}
		else return nullptr;
	}

	static int writer(lua_State* L, const void* p, size_t size, void* u)
	{
		UNUSED(L);
		return ((DataStream*)u)->SerializeWrite((const char*)p, size) ? 0 : 1;
	}

	static inline char* luadec_strdup(const char* src) {
		return ((src) ? strdup(src) : NULL);
	}

	static bool do_luac(const char* Chunk, DataStream* pOut, char* pIn, u64 insize, bool bIsCompile) {
		if (bIsCompile) {
			reader(0, 0, &insize);
			if (lua_load(sRuntime.L, reader, pIn, Chunk, "t") != LUA_OK) {
				printf("could not lua_load bt, lua code syntax error etc %s\n", Chunk);
				return false;
			}
			Proto* f = toproto(sRuntime.L, -1);
			if (luaU_dump(sRuntime.L, f, writer, pOut, 0) != 0){
				printf("could not luaU_dump %s\n", Chunk);
				return false;
			}
			return true;
		}
		else {
			int res = 0;
			if ((res=luaL_loadbufferx(sRuntime.L, pIn, insize, Chunk, "b")) != LUA_OK) {
				printf("could not lua_load bt, lua bytecode error etc(2) %s %d\n", Chunk, res);
				return false;
			}
			Proto* f = toproto(sRuntime.L, -1);
			char* code = ProcessCode(f, 0, 0, luadec_strdup("0"));
			if (code) {
				pOut->SerializeStringWrite(code);
				free(code);
				return true;
			}
			
		}
		return false;
	}

	static bool convert_to_scriptn(DataStream* pIn, DataStream* pOut, const std::string& name, int flag) {
		char* buffer = new char[pIn->GetSize()];
		pIn->Serialize(buffer, pIn->GetSize());

		bool lenc = ends_with(name, "lenc");
		bool alreadyEnc = false;
		bool alreadyCompiled = false;
		if (!memcmp("\x1BLua", buffer, 4))
			alreadyCompiled = true;
		if (!alreadyCompiled) {
			if (lenc) {
				alreadyEnc = !is_raw_script(buffer, pIn->GetSize());
			}
			else {
				alreadyEnc = !memcmp("\x1BLen", buffer, 4) || !memcmp("\x1BLeo", buffer, 4);
			}
		}

		u64 size = pIn->GetSize();

		//1. compile it if needed
		if (!alreadyCompiled) {
			DataStreamMemory tempMem{ pIn->GetSize() };
			bool compileResult = do_luac(name.c_str(), &tempMem, buffer, pIn->GetSize(), true);
			if (!compileResult) {
				delete[] buffer;
				printf("failed to compile '%s' when converting to telltale compiled/enc script!\n", name.c_str());
				return false;
			}
			delete[] buffer;
			buffer = new char[size=tempMem.GetPosition()];
			tempMem.SetPosition(0, DataStreamSeekType::eSeekType_Begin);
			tempMem.SetMode(DataStreamMode::eMode_Read);
			tempMem.Serialize(buffer, size);

		}

		//2. encrypt it if needed
		if (alreadyEnc) {
			//just write it
			pOut->Serialize(buffer, size);
		}
		else {
			u32 outz{};
			u8* res = lenc ? TelltaleToolLib_EncryptLencScript((u8*)buffer, (u32)pIn->GetSize(), &outz) :
				TelltaleToolLib_EncryptScript((u8*)buffer, (u32)pIn->GetSize(), &outz);
			if (!res) {
				printf("could not encrypt script! %s\n", name.c_str());
				delete[] buffer;
				return false;
			}
			pOut->Serialize((char*)res, outz);
			free(res);
		}

		delete[] buffer;
		return true;
	}

	//flag1=raw rescdesc,flag0=enc resdesc,flag2=plain src resdesc same as flag1
	static bool convert_extract_resdesc_c(DataStream* pIn, DataStream* pOut, const std::string& name, int flag) {
		char* buffer = new char[pIn->GetSize()];
		pIn->Serialize(buffer, pIn->GetSize());
		if (!memcmp("\x1BLuaR", buffer, 5)) {
			printf("cannot convert to resdesc '%s' as its compiled. the decompiler does not work ! !\n", name.c_str());
			return false;
		}

		bool alreadyEnc;
		bool lenc = ends_with(name, "lenc");
		if (lenc) {
			alreadyEnc = !is_raw_script(buffer, pIn->GetSize());
		}
		else {
			alreadyEnc = !memcmp("\x1BLEn", buffer, 4) || !memcmp("\x1BLEo", buffer, 4);
		}

		u32 outz{ 0 };
		u8* res{ 0 };
		if (alreadyEnc && flag == 0) {
			//nothing needed
			pOut->SerializeWrite(buffer, pIn->GetSize());
			delete[] buffer;
			return true;
		}
		else if (alreadyEnc && flag == 1) {//decrypt it only
			res = lenc ? TelltaleToolLib_DecryptLencScript((u8*)buffer, (u32)pIn->GetSize(), &outz) :
				TelltaleToolLib_DecryptScript((u8*)buffer, (u32)pIn->GetSize(), &outz);
			if (is_raw_script((char*)res, outz)) {
				//converted to src, return its
				pOut->SerializeWrite((char*)res, outz);
				delete[] buffer;
				free(res);
				return true;
			}
			//either wrong key or its compiled, either way cant help there
			printf("cannot convert %s to src, either wrong key or is compiled (resource descriptions can sometimes be,"
				" try sending it to bytecode!\n", name.c_str());
			if(res)
				free(res);
			delete[] buffer;
			return false;
		}

		//if its compiled
		if (!memcmp(buffer, "\x1BLua", 4)) {
			delete[] buffer;
			printf("cannot convert %s to src, already compiled - cannot decompile!\n");
			return false;
		}

		//we want it encrypted
		if (flag == 0) {

			//not encrypted, so encrypt it
			res = lenc ? TelltaleToolLib_EncryptLencScript((u8*)buffer, (u32)pIn->GetSize(), &outz) :
				TelltaleToolLib_EncryptScript((u8*)buffer, (u32)pIn->GetSize(), &outz);

			bool result = pOut->SerializeWrite((const char*)res, outz);

			free(res);
			delete[] buffer;

			return result;

		}
		else {//we want it in src, return it
			pOut->SerializeWrite(buffer, pIn->GetSize());
			delete[] buffer;
			return true;
		}
	}

	static bool convert_extract(DataStream* pIn, DataStream* pOut, const std::string& name, int flag) {
		char* buffer = new char[pIn->GetSize()];
		pIn->Serialize(buffer, pIn->GetSize());
		if (is_raw_script(buffer, pIn->GetSize()) && flag == 1) {
			pOut->SerializeWrite(buffer, pIn->GetSize());
			delete[] buffer;
			return true;
		}
		bool lenc = ends_with(name, "lenc");
		u32 outz{ 0 };
		u8* res;
		bool israw, dontfree = false;
		if (pIn->GetSize() > 5 && memcmp(buffer, "\x1BLuaR",5) == 0) {
			israw = false;
			dontfree = true;
			res = (u8*)buffer;
			outz = (u32)pIn->GetSize();
		}
		else {
			res = lenc ? TelltaleToolLib_DecryptLencScript((u8*)buffer, (u32)pIn->GetSize(), &outz) :
				TelltaleToolLib_DecryptScript((u8*)buffer, (u32)pIn->GetSize(), &outz);
			if (!(res[0] == '\x1B') && !is_raw_script((char*)res, outz)) {
				free(res);
				delete[] buffer;
				return false;
			}
			israw = is_raw_script((char*)res, outz);
		}
		if (flag == 0) {
			if (israw) {
				bool luac = do_luac(name.c_str(), pOut, (char*)res, outz, true);
				if(!dontfree)
					free(res);
				delete[] buffer;
				return luac;
			}
			else {
				pOut->SerializeWrite((const char*)res, outz);
				if (!dontfree)
					free(res);
			}
			delete[] buffer;
			return true;
		}
		else {
			if (israw) {
				pOut->SerializeWrite((const char*)res, outz);
				if (!dontfree)
					free(res);
				delete[] buffer;
				return true;
			}
			else {
				bool luac = do_luac(name.c_str(), pOut, (char*)res, outz, false);
				if (!dontfree)
					free(res);
				delete[] buffer;
				return luac;
			}
		}
	}

	void bulk_conv_all(bool(*converter)(DataStream* pIn, DataStream* pOut, const std::string& name, int flag), int flag_value) {
		
		struct converted {
			DataStreamMemory* pMemoryStream;
			std::string name;
			u32 size = 0;
			decltype(mFiles)::iterator mDestIt;
		};

		if (gameid == nullptr) {
			MessageBoxA(0, "Ensure you have selected the game ID first!", "Select Game ID!", MB_ICONWARNING);
			return;
		}
		TelltaleToolLib_SetBlowfishKey(gameid);

		int result = MessageBoxA(0, "Would you like to replace all loaded files in this task with the converted ones (yes)"
			" or save them to a folder (no)?", "Telltale Inspector",
			MB_ICONQUESTION | MB_YESNO);

		bool replace_after = result == IDYES;
		nfdchar_t* outp{};

		if (!replace_after) {

			nfdresult_t dir = NFD_PickFolder(0, &outp, L"Select Destination Folder");
			if (dir != NFD_OKAY)
				return;

		}

		std::vector<converted> convertedScripts{};
		convertedScripts.reserve(mFiles.size());
		std::stringstream error{};
		int errs = 0;
		error << "The following files could not be converted:\n";
		std::vector<std::string> invalidStreams{};
		for (auto it = mFiles.begin(); it != mFiles.end(); it++) {
			auto& script = *it;
			DataStream* pIn = nullptr;
			bool bNeedsDel = !script.mbIsMemory;
			if (script.mbIsMemory)
				pIn = script.mMemoryStream;
			else
				pIn = _OpenDataStreamFromDisc(script.mDiskPath.string().c_str(), READ);
			if (!pIn) {
				MessageBoxA(0, "Could not open the file (see title bar). It either was deleted from your file system or could not be loaded."
					" It will be discarded.", script.mName.c_str(), MB_ICONERROR);
				invalidStreams.push_back(it->mName);
				continue;
			}
			converted out{};
			out.name = script.mName;
			out.pMemoryStream = new DataStreamMemory(pIn->GetSize() + 20);
			out.mDestIt = it;
			bool bResult = converter(pIn, out.pMemoryStream, script.mName, flag_value);
			if (!bResult) {
				error << script.mName << "\n";
				errs++;
			}
			else {
				out.size = (u32)out.pMemoryStream->GetPosition();
				convertedScripts.push_back(out);
			}
			if (bNeedsDel)
				delete pIn;
		}

		for (auto& v : convertedScripts) {

			if (replace_after) {
				imported_file& ac = *v.mDestIt;
				if (ac.mbIsMemory)
					delete ac.mMemoryStream;
				ac.mbIsMemory = true;
				ac.mMemoryStream = v.pMemoryStream;
				ac.mMemoryStream->Truncate(v.size);
				ac.mMemoryStream->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
				v.pMemoryStream = nullptr;
			}
			else {
				std::filesystem::path outfile = outp;
				outfile += "/";
				outfile += v.name;
				DataStreamFileDisc strm = _OpenDataStreamFromDisc_(outfile.string().c_str(), WRITE);
				v.pMemoryStream->Transfer(&strm, 0, v.size);
				delete v.pMemoryStream;
			}

		}

		for (auto& it : invalidStreams) {
			for (auto it1 = mFiles.begin(); it1 != mFiles.end(); it1++) {
				if (iequals(it, it1->mName)) {
					mFiles.erase(it1);
					goto outer;
				}
			}
		outer:
			continue;
		}

		if (errs) {
			MessageBoxA(0, error.str().c_str(), "Error converting some scripts", MB_ICONERROR);
		}
		else {
			MessageBoxA(0, "All scripts were converted successfully!", "Success!", MB_ICONINFORMATION);
		}
	}

	virtual void _render() override {
		ImGui::Text("Select Game");
		gameid = select_gameid_dropdown(gameid);
		ImGui::Text("Files");
		ImGui::ListBox("##files_A", &selectedfile, &getter, (void**)mFiles.data(), (int)mFiles.size());
		if (ImGui::Button("Add File(s)")) {
			nfdpathset_t paths{};
			nfdresult_t result = NFD_OpenDialogMultiple("lua;lenc", 0, &paths);
			if (result == NFD_OKAY) {
				exitstrat strat = exitstrat::ask;
				int num = NFD_PathSet_GetCount(&paths);
				mFiles.reserve(mFiles.size() + num);
				for (int i = 0; i < num; i++) {
					nfdchar_t* path = NFD_PathSet_GetPath(&paths, i);
					std::filesystem::path phys{ path };
					if (std::filesystem::is_regular_file(phys)) {
						import_in_singlefile(phys, strat);
					}
					else {
						MessageBoxA(NULL, path, "File Invalid, not including!", MB_ICONERROR);
						continue;
					}
				}
			}
		}
		if (ImGui::Button("Remove Selected File")) {
			if (mFiles.size() > 0)
				mFiles.erase(mFiles.begin() + selectedfile);
			selectedfile = 0;
		}
		ImGui::Text("Use this to convert to plain lua source code. Note that the decompiler in C++ (luadec, or luashi*) doesn't work >:(."
		"\nTried everything, but no luck with it. Your going to need to use lua decompiler written in java, it works very well.");
		
		if (ImGui::Button("Convert All => Lua Source") && mFiles.size() > 0) {
			//bulk_conv_all(&convert_extract, 1);
		}
		ImGui::Text("Use this to convert to compiled lua bytecode.");
		if (ImGui::Button("Convert All => Lua Bytecode") && mFiles.size() > 0) {
			bulk_conv_all(&convert_extract, 0);
		}
		ImGui::Text("Use this to convert to a resource description lua script.");
		if (ImGui::Button("Convert All => Resdesc Lua") && mFiles.size() > 0) {
			bulk_conv_all(&convert_extract_resdesc_c, 0);
		}
		ImGui::Text("Use this to convert to a raw resource description lua script.");
		if (ImGui::Button("Convert All => Rescdesc Lua (raw)") && mFiles.size() > 0) {
			bulk_conv_all(&convert_extract_resdesc_c, 1);
		}
		ImGui::Text("Use this to convert a resource description to lua source.");
		if (ImGui::Button("Convert All => Resdesc Lua Source") && mFiles.size() > 0) {
			bulk_conv_all(&convert_extract_resdesc_c, 1);
		}
		ImGui::Text("Use this to convert to a normal encrypted/compiled telltale lua script inside archives.");
		if (ImGui::Button("Convert All => Lua Script") && mFiles.size() > 0) {
			bulk_conv_all(&convert_to_scriptn, 0);
		}
	}

public:

	deleteme(LuaTask)

	taskctor(LuaTask) {}

};

class BankTask : public InspectorTask {

	int selectedfile;
	FMODSoundBank mBank{};

	static bool getter(void* data, int index, const char** output)
	{
		*output = ((FMODSample*)data)[index].mName.c_str();
		return true;
	}

	static bool sample_sorter(const FMODSample& lhs, const FMODSample& rhs) {
		std::less<std::string> l{};
		return l(lhs.mName, rhs.mName);
	}

	virtual void _render() override {
		static bool strip{};
		ImGui::Checkbox("Strip Audio File Name UUIDs", &strip);
		mBank.SetStripNameUUIDs(strip);
		ImGui::Text("Audio Files");
		ImGui::ListBox("##audio", &selectedfile, &getter, (void*)mBank.GetSamples().data(), (u32)mBank.GetSamples().size());
		if (ImGui::Button("Override all & Import New Bank")) {
			nfdchar_t* outPath{};
			nfdresult_t result = NFD_OpenDialog("bank;fsb5", nullptr, &outPath);
			if (result == NFD_OKAY) {
				DataStreamFileDisc* pBankStream = OpenDataStreamFromDisc(outPath, READ);
				if (!pBankStream || pBankStream->IsInvalid()) {
					MessageBoxA(0, "Could not open the file!", "Error", MB_ICONERROR);
				}
				else {
					mBank.Reset();
					std::string error{};
					if (!mBank.Open(pBankStream, error)) {
						MessageBoxA(0, error.c_str(), "Could not open FMOD sound bank!", MB_ICONERROR);
					}
					else {
						std::sort(mBank.GetSamples().begin(), mBank.GetSamples().end(), &sample_sorter);
						selectedfile = 0;
					}
				}
			}
		}
		if (ImGui::Button("Extract Selected") && mBank.GetSamples().size() > 0) {
			nfdchar_t* outp{};
			if (NFD_PickFolder( 0, &outp, L"Select Destination Folder") == NFD_OKAY) {
				std::filesystem::path folder{ outp };
				folder += "/";
				folder += mBank.GetSamples()[selectedfile].mName;
				DataStreamFileDisc out = _OpenDataStreamFromDisc_(folder.string().c_str(), WRITE);
				if (out.IsInvalid()) {
					MessageBoxA(0, "Could not open the file!", "Error", MB_ICONERROR);
				}
				else {
					std::string err{};
					DataStreamMemory* result = mBank.MakeAudioFile(selectedfile, err);
					if (!result) {
						MessageBoxA(0, err.c_str(), "Error extracting ogg vorbis", 0);
						return;
					}
					result->SetMode(DataStreamMode::eMode_Read);
					result->Transfer(&out, 0, result->GetSize());
					delete result;
					MessageBoxA(0, "Successfully extracted the audio file!", "Success", MB_ICONINFORMATION);
				}
			}
			
		}
		if (ImGui::Button("Extract All")) {
			nfdchar_t* outp{};
			if (NFD_PickFolder(0, &outp, L"Select Destination Folder") == NFD_OKAY) {
				int eerr = 0;
				std::filesystem::path folder{ outp };
				folder += "/";
				std::stringstream errs{};
				errs << "The following files could not be extracted:\n";
				std::string err{};
				for(int i = 0; i < mBank.GetSamples().size(); i++){
					std::filesystem::path file = folder;
					file += mBank.GetSamples()[i].mName;
					DataStreamFileDisc out = _OpenDataStreamFromDisc_(file.string().c_str(), WRITE);
					if (out.IsInvalid()) {
						eerr++;
						errs << mBank.GetSamples()[i].mName << ": Could not open file on disk";
						continue;
					}
					DataStreamMemory* result = mBank.MakeAudioFile(i, err);
					if (!result) {
						eerr++;
						errs << mBank.GetSamples()[i].mName << ": " << err << "\n";
						continue;
					}
					result->SetMode(DataStreamMode::eMode_Read);
					result->Transfer(&out, 0, result->GetSize());
					delete result;
				}
				if (eerr > 0) {
					MessageBoxA(0, errs.str().c_str(), "Error extracting some of the audio files", MB_ICONERROR);
				}else
					MessageBoxA(0, "Successfully extracted all the audio files!", "Success", MB_ICONINFORMATION);
			}
		}
		if (ImGui::Button("Reset and Clear Files")) {
			mBank.Reset();
		}
	}

public:

	deleteme(BankTask)

	taskctor(BankTask) {}

};

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

static bool prop_getter(void* data, int index, const char** output)
{
	PropGetterDelegate* del = (PropGetterDelegate*)data;
	sprintf((char*)del->papl->name_buf, "[%d] %s Value", index, 
		(del->pinf + index)->mpValue->mpDataDescription->mpTypeInfoName);
	*output = (char*)del->papl->name_buf;
	return true;
}

static bool dcarray_getter(void* data, int index, const char** output)
{
	sprintf((char*)data, "[%d]", index);
	*output = (char*)data;
	return true;
}

void array_persist_del(void* inst) {
	delete ((ArrayPersistance*)inst);
}

void array_persist_del_l(void* inst) {
	delete ((ArrayPersistanceL*)inst);
}

class MetaStreamTask;

const u64 renderProxyID = 0xDEADBEEF;

struct MCDRenderMapProxy {

	u64 _ID;
	MetaClassDescription* pKeyType, * pValType;
	void* pKeyInst, * pValInst;

	MCDRenderMapProxy() {
		_ID = renderProxyID;
	}

};

static inline u64 MakePersistentDataID(MetaClassDescription* mcd, void* pInst) {
	return u64{ mcd->mHash ^ ((u64)pInst) };
}

inline MetaStream* GetStreamHelper(MetaStreamTask* pTask);

static const float kD3DTextureBorders[2][4] = {
		{0.0f,0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f,1.0f}
};

static const char* kD3DTextureWraps[3] = {
	"Clamp", "Wrap", "Border"
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

	void ResetChild() {
		if (mpChildSub) {
			mpChildSub->Reset();
			delete mpChildSub;
		}
		mpChildSub = nullptr;

		if (mpKeyChildSub) {
			mpKeyChildSub->Reset();
			delete mpKeyChildSub;
		}
		mpKeyChildSub = nullptr;
	}

	void Reset() {
		mPersistentData.clear();
		ResetChild();
	}

	void Init(MetaStreamTask* pTask, void* pInst, MetaClassDescription* pType) {
		mpTypeDesc = pType;
		mpInstance = pInst;
		mpTask = pTask;
	}

	const char* GenNextID() {
		const char* pID = (const char*)&mIDGen;
		sprintf(mID, "##MCD_%llX", CRC64(mBaseIDHash, pID, 8));
		mIDGen++;
		return mID;
	}

	void RenderChild(void* pInst, MetaClassDescription* pType, int id, bool* pShouldClose, std::string hdr = "") {
		if (!mbChildRendered) {
			mbChildRendered = true;
			if (*((u64*)pInst) == renderProxyID) {
				//RENDER MAP CHILD WINDOW
				MCDRenderMapProxy proxy = *(MCDRenderMapProxy*)pInst;
				if (mpChildSub) {
					if (mpChildSub->mpInstance != proxy.pValInst || mpChildSub->mpTypeDesc != proxy.pValInst) {
						mpChildSub->Reset();
						mpChildSub->Init(mpTask, proxy.pValInst, proxy.pValType);
						mpChildSub->mHeaderStr = hdr;
					}
				}
				else {
					mpChildSub = new MetaStreamSubType{};
					mpChildSub->Init(mpTask, proxy.pValInst, proxy.pValType);
					mpChildSub->mHeaderStr = hdr;
				}

				if (mpKeyChildSub) {
					if (mpKeyChildSub->mpInstance != proxy.pKeyInst || mpKeyChildSub->mpTypeDesc != proxy.pKeyInst) {
						mpKeyChildSub->Reset();
						mpKeyChildSub->Init(mpTask, proxy.pKeyInst, proxy.pKeyType);
						mpChildSub->mHeaderStr = hdr;
					}
				}
				else {
					mpKeyChildSub = new MetaStreamSubType{};
					mpKeyChildSub->Init(mpTask, proxy.pKeyInst, proxy.pKeyType);
					mpChildSub->mHeaderStr = hdr;
				}

				ImGui::PushID(id ^ 0x9969'6966);
				ImGui::OpenPopup(proxy.pValType->mpTypeInfoName);
				if (ImGui::BeginPopup(proxy.pValType->mpTypeInfoName, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
					if (ImGui::Button("Close")) {
						*pShouldClose = true;
					}
					ImGui::Text("Mapping Key (%s) =>", proxy.pKeyType->mpTypeInfoName);
					mpKeyChildSub->Render(~id, 0.f);
					
					ImGui::Separator();

					ImGui::Text("Mapping Value (%s) =>", proxy.pValType->mpTypeInfoName);
					mpChildSub->Render(id, 0.f);


					ImGui::EndPopup();
				}
				ImGui::PopID();
			}
			else {
				if (mpChildSub) {
					if (mpChildSub->mpInstance != pInst || mpChildSub->mpTypeDesc != pType) {
						mpChildSub->Reset();
						mpChildSub->Init(mpTask, pInst, pType);
						mpChildSub->mHeaderStr = hdr;
					}
				}
				else {
					mpChildSub = new MetaStreamSubType{};
					mpChildSub->Init(mpTask, pInst, pType);
					mpChildSub->mHeaderStr = hdr;
				}
				ImGui::PushID(id ^ 0x6969'6969);
				ImGui::OpenPopup(pType->mpTypeInfoName);
				if (ImGui::BeginPopup(pType->mpTypeInfoName,  ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
					if (ImGui::Button("Close")) {
						*pShouldClose = true;
					}
					mpChildSub->Render(id, 0.f);
					ImGui::EndPopup();
				}
				ImGui::PopID();
			}
		}
	}

	void render_intrin_mcd(const char* mem, MetaClassDescription* mcd, void* pInst, float depth) {
		char buf[32];
		u64 typeHash = mcd->mHash;
		ImGui::Text(mem);
		ImGui::SameLine();
		if (typeHash == hash_int8) {
			i8 step = 1, maxstep = 100;
			ImGui::InputScalar(GenNextID(), ImGuiDataType_S8, pInst, &step, &maxstep);
		}
		else if (typeHash == hash_uint8) {
			u8 step = 1, maxstep = 100;
			ImGui::InputScalar(GenNextID(), ImGuiDataType_U8, pInst, &step, &maxstep);
		}
		else if (typeHash == hash_uint16) {
			u16 step = 1, maxstep = 100;
			ImGui::InputScalar(GenNextID(), ImGuiDataType_U16, pInst, &step, &maxstep);
		}
		else if (typeHash == hash_int16) {
			i16 step = 1, maxstep = 100;
			ImGui::InputScalar(GenNextID(), ImGuiDataType_S16, pInst, &step, &maxstep);
		}
		else if (typeHash == hash_long || typeHash == hash_int || typeHash == hash_int32) {
			i32 step = 1, maxstep = 100;
			ImGui::InputScalar(GenNextID(), ImGuiDataType_S32, pInst, &step, &maxstep);
		}
		else if (typeHash == hash_uint32) {
			u32 step = 1, maxstep = 100;
			ImGui::InputScalar(GenNextID(), ImGuiDataType_U32, pInst, &step, &maxstep);
		}
		else if (typeHash == hash_uint64) {
			u64 step = 1, maxstep = 100;
			ImGui::InputScalar(GenNextID(), ImGuiDataType_U64, pInst, &step, &maxstep);
		}
		else if (typeHash == hash_int64) {
			i64 step = 1, maxstep = 100;
			ImGui::InputScalar(GenNextID(), ImGuiDataType_S64, pInst, &step, &maxstep);
		}
		else if (typeHash == hash_double) {
			ImGui::InputDouble(GenNextID(), (double*)pInst);
		}
		else if (typeHash == hash_float) {
			ImGui::InputFloat(GenNextID(), (float*)pInst);
		}
		else if (typeHash == hash_bool) {
			ImGui::Checkbox(GenNextID(), (bool*)pInst);
		}
		else if (typeHash == hash_str) {
			ImGui::InputText(GenNextID(), (String*)pInst);
		}
		else if (typeHash == hash_sym) {
			char buf[30];
			u64 hash = ((Symbol*)pInst)->GetCRC();
			std::string* pResolved = mpTask->ResolveHash(hash, check_all);
			
			if (pResolved) {

				ImGui::InputText(GenNextID(), pResolved, ImGuiInputTextFlags_ReadOnly);//no edit, TODO add a button to edit
				//from a seperate popup. //NOTE, IF MAKING IT EDITABLE, ENSURE TO REMOVE THE OLD HASH FROM THE RESOLVED MAP!!!

			}
			else {

				if (hash)
					sprintf(buf, "Symbol<%llX>", hash);
				else
					sprintf(buf, "No Symbol Value");
				ImGui::InputText(GenNextID(), buf, 30, ImGuiInputTextFlags_ReadOnly);

			}
		}
		else {
			ImGui::Text("UNKNOWN TYPE FOUND HERE - CONTACT ME ASAP!");
		}
	}

	void render_intrin_mcd_enum(const char* mem, MetaClassDescription* mcd, void* pInst, float depth, MetaMemberDescription* pMemDesc) {
		int enumVal = *(int*)pInst;
		MetaEnumDescription* pSelectedDesc = nullptr;
		MetaEnumDescription* pEnum = pMemDesc->mpEnumDescriptions;
		while (pEnum) {
			if (pEnum->mEnumIntValue == enumVal) {
				pSelectedDesc = pEnum;
				break;
			}
			pEnum = pEnum->mpNext;
		}
		pEnum = pMemDesc->mpEnumDescriptions;
		ImGui::Text("%s | Enum: %s", mem, pMemDesc->mpMemberDesc->mpTypeInfoName);
		if (ImGui::BeginCombo(GenNextID(), pSelectedDesc ? pSelectedDesc->mpEnumName : "Not Set")) {
			while (pEnum) {
				if (ImGui::Selectable(pEnum->mpEnumName)) {
					enumVal = *((int*)pInst) = pEnum->mEnumIntValue;
					ImGui::SetItemDefaultFocus();
				}
				pEnum = pEnum->mpNext;
			}
			ImGui::EndCombo();
		}
	}

	void render_intrin_mcd_flags(const char* mem, MetaClassDescription* mcd, void* pInst, float depth, MetaMemberDescription* pMemDesc) {

		ImGui::Text("%s | Flags: %s", mem, pMemDesc->mpMemberDesc->mpTypeInfoName);

		ImGui::Indent(5.0f);

		u32 flagsVal = *((u32*)pInst);
		MetaFlagDescription* pFlag = pMemDesc->mpFlagDescriptions;
		while (pFlag) {
			bool bSelect = (flagsVal & (u32)pFlag->mFlagValue) != 0;
			bool bPrev = bSelect;
			ImGui::Checkbox(pFlag->mpFlagName, &bSelect);
			if (bPrev != bSelect) {
				if (bSelect)
					*((int*)pInst) |= pFlag->mFlagValue;
				else
					*((int*)pInst) &= (~pFlag->mFlagValue);
			}
			pFlag = pFlag->mpNext;
		}

		ImGui::Unindent(5.0f);
	}

	static const u64 trangef_str = CRC64_CaseInsensitive(0, "TRange<float>");

	void render_intrin_mcd_trange(const char* mem, MetaClassDescription* mcd, void* pInst, float depth) {
		ImGui::Text("%s | %s", mem, mcd->mpTypeInfoName);
		ImGui::Text("Min:");
		ImGui::SameLine();
		char mn[8], mx[8];
		memcpy(mn, pInst, 4);
		memcpy(mx, ((char*)pInst) + 4, 4);
		bool bIsFloat = mcd->mHash == trangef_str;
		
		ImGui::InputScalar("##", bIsFloat ? ImGuiDataType_Float : ImGuiDataType_U32, mn);
		ImGui::SameLine();
		ImGui::Text("Max:");
		ImGui::InputScalar("##", bIsFloat ? ImGuiDataType_Float : ImGuiDataType_U32, mx);

		memcpy(pInst, mn, 4);
		memcpy((char*)pInst + 4, mx, 4);

	}

	void render_intrin_mcd_math(const char* mem, MetaClassDescription* mcd, void* pInst, float depth, u64 hash = 0) {
		hash = mcd ? mcd->mHash : hash;
		if(mcd)
			ImGui::Text("%s | %s", mem, mcd->mpTypeInfoName);
		else
			ImGui::Text("%s", mem);
		if (hash == vec4_str || hash == quat_str) {
			ImGui::SameLine();
			Vector4* vec = (Vector4*)pInst;//same as quat
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X"))
				vec->x = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat(GenNextID(), &vec->x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y"))
				vec->y = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat(GenNextID(), &vec->y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z"))
				vec->z = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat(GenNextID(), &vec->z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			if (ImGui::Button("W"))
				vec->w = 0.0f;

			ImGui::SameLine();
			ImGui::DragFloat(GenNextID(), &vec->w, 0.1f, 0.0f, 0.0f, "%.2f");

		}
		else if (hash == vec3_str) {
			ImGui::SameLine();
			Vector3* vec = (Vector3*)pInst;//same as quat
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X"))
				vec->x = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat(GenNextID(), &vec->x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y"))
				vec->y = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat(GenNextID(), &vec->y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z"))
				vec->z = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat(GenNextID(), &vec->z, 0.1f, 0.0f, 0.0f, "%.2f");

		}
		else if (hash == vec2_str) {
			ImGui::SameLine();
			Vector2* vec = (Vector2*)pInst;//same as quat
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X"))
				vec->x = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat(GenNextID(), &vec->x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y"))
				vec->y = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat(GenNextID(), &vec->y, 0.1f, 0.0f, 0.0f, "%.2f");
		}
		else if (hash == tran_str) {
			Transform* transform = (Transform*)pInst;
			ImGui::Text("%s | Transform", mem);
			ImGui::Indent(10.f);
			render_intrin_mcd_math("Rotation", nullptr, &transform->mRot, depth + 10.f, quat_str);
			render_intrin_mcd_math("Translation", nullptr, &transform->mTrans, depth + 10.f, vec3_str);
			ImGui::Unindent(10.f);
		}
		else if (hash == col_str) {
			Color* transform = (Color*)pInst;
			ImGui::Text("%s | Colour", mem);
			render_intrin_mcd_math("RGBA", nullptr, transform, depth, vec4_str);
		}
		else if (hash == sphere_str) {
			Sphere* s = (Sphere*)pInst;
			ImGui::Text("%s | Sphere", mem);
			ImGui::Indent(10.f);
			render_intrin_mcd_math("Center", nullptr, &s->mCenter, depth, vec3_str);
			ImGui::Text("Radius");
			ImGui::SameLine();
			ImGui::DragFloat(GenNextID(), &s->mRadius, 0.1f, 0.0f, 0.f);
			ImGui::Unindent(10.f);
		}
	}

	//fwd decl
	void render_mcd_array(const char* mem, u32 arraySize, void* pArray, MetaClassDescription* pArrayType, float depth);
	void render_mcd_std_cont(const char* mem, MetaClassDescription* mcd, void* pInst, float depth);
	void render_mcd_map(const char* mem, MetaClassDescription* mcd, void* pInst, float depth);

	void render_mcd_dcarray(const char* mem, MetaClassDescription* mcd, void* pInst, float depth) {
		DCArray<void*>* pDummyArray = (DCArray<void*>*)pInst;
		MetaClassDescription* pArrayType = pDummyArray->GetContainerDataClassDescription();
		u32 numElements = pDummyArray->GetSize();
		void* pArrayData = (void*)pDummyArray->mpStorage;
		render_mcd_array(mem, numElements, pArrayData, pArrayType, depth);
	}

	void render_mcd_sarray(const char* mem, MetaClassDescription* mcd, void* pInst, float depth) {
		SArray<void*, 1>* pDummyArray = (SArray<void*, 1>*)pInst;
		MetaClassDescription* pArrayType = pDummyArray->mpContainerDataType;
		u32 numElements = pDummyArray->NUM_DATA_ELEM;
		void* pArrayData = (void*)pDummyArray->mData;
		render_mcd_array(mem, numElements, pArrayData, pArrayType, depth);
	}

	bool render_mcd_internal(const char* mem, MetaClassDescription* mcd, void* pInstance, float depth) {
		if (mcd->mbIsIntrinsic || mcd->MatchesHash(CRC64_CaseInsensitive(0,"Symbol")))
			render_intrin_mcd(mem, mcd, pInstance, depth);
		else {
			if (strlen(mcd->mpTypeInfoName) >= 6 && !memcmp(mcd->mpTypeInfoName, "TRange", 6)) {
				render_intrin_mcd_trange(mem, mcd, pInstance, depth);
			}
			else if (strlen(mcd->mpTypeInfoName) >= 7 && !memcmp(mcd->mpTypeInfoName, "DCArray", 7)) {
				render_mcd_dcarray(mem, mcd, pInstance, depth);
			}
			else if (strlen(mcd->mpTypeInfoName) >= 4 && !memcmp(mcd->mpTypeInfoName, "Set<", 4)) {
				render_mcd_std_cont(mem, mcd, pInstance, depth);
			}
			else if (strlen(mcd->mpTypeInfoName) >= 5 && !memcmp(mcd->mpTypeInfoName, "List<", 5)) {
				render_mcd_std_cont(mem, mcd, pInstance, depth);
			}
			else if (strlen(mcd->mpTypeInfoName) >= 4 && !memcmp(mcd->mpTypeInfoName, "Map<", 4)) {
				render_mcd_map(mem, mcd, pInstance, depth);
			}
			else if (strlen(mcd->mpTypeInfoName) >= 7 && !memcmp(mcd->mpTypeInfoName, "SArray<", 7)) {
				render_mcd_sarray(mem, mcd, pInstance, depth);
			}
			else if (strlen(mcd->mpTypeInfoName) >= 6 && !memcmp(mcd->mpTypeInfoName, "Deque<", 6)) {
				render_mcd_std_cont(mem, mcd, pInstance, depth);
			}
			else if (strlen(mcd->mpTypeInfoName) >= 7 && !memcmp(mcd->mpTypeInfoName, "Handle<", 7)) {
				Handle<void*>* proxy = (Handle<void*>*)pInstance;
				ImGui::Text("File Reference (%s) | %s", mcd->mpTypeInfoName, mem);
				ImGui::Indent(2.0f);
				const char* id = GenNextID();
				if (proxy->mHandleObjectInfo.mObjectName.GetCRC()) {
					std::string* pResolved = mpTask->ResolveHash(proxy->mHandleObjectInfo.mObjectName.GetCRC(), file_name);
					if (pResolved) {// TODO and below, again if making editable, have a button for this and popup, same from handlelock
						ImGui::InputText(id, pResolved, ImGuiInputTextFlags_ReadOnly);
					}
					else {
						char buf[30];
						buf[0] = 0;
						sprintf(buf, "Symbol<%llX>", proxy->mHandleObjectInfo.mObjectName.GetCRC());
						ImGui::Text(buf);
					}
				}
				else
					ImGui::Text("No Reference");
				ImGui::Unindent(2.0f);
			}
			else if (strlen(mcd->mpTypeInfoName) >= 11 && !memcmp(mcd->mpTypeInfoName, "HandleLock<", 11)) {
				Handle<void*>* proxy = (Handle<void*>*)pInstance;
				ImGui::Text("Locked File Reference (%s) | %s", mcd->mpTypeInfoName, mem);
				ImGui::Indent(2.0f);
				const char* id = GenNextID();
				if (proxy->mHandleObjectInfo.mObjectName.GetCRC()) {
					std::string* pResolved = mpTask->ResolveHash(proxy->mHandleObjectInfo.mObjectName.GetCRC(), file_name);
					if (pResolved) {// TODO and below, again if making editable, have a button for this and popup, same from handlelock
						ImGui::InputText(id, pResolved, ImGuiInputTextFlags_ReadOnly);
					}
					else {
						char buf[30];
						buf[0] = 0;
						sprintf(buf, "Symbol<%llX>", proxy->mHandleObjectInfo.mObjectName.GetCRC());
						ImGui::Text(buf);
					}
				}
				else
					ImGui::Text("No Reference");
				ImGui::Unindent(2.0f);
			}
			else if (strlen(mcd->mpTypeInfoName) >= 11 && !memcmp(mcd->mpTypeInfoName, "PropertySet", 12)) {//12, ensure null is too
				
				//render prop set
				PropertySet* pProps = (PropertySet*)pInstance;
				PropertySet::KeyInfo* pPropKeys = pProps->mKeyMap.mpStorage;
				PropGetterDelegate del{};
				del.pinf = pPropKeys;

				ImGui::Indent(5.f);

				//id_gen ONLY ACCESS BEFORE CALLING GEN_ID !!
				PersistentData& persist = mPersistentData[MakePersistentDataID(mcd, pProps)];
				if (persist.pDataInstance == nullptr) {
					persist.mDealloc = &array_persist_del;
					persist.pDataInstance = new ArrayPersistanceL{ 0 };
				}
				ArrayPersistanceL* arr = (ArrayPersistanceL*)persist.pDataInstance;
				del.papl = arr;

				//render the array and selected
				bool bOpenSelected{};
				ImGui::Text("Property Set:");
				ImGui::PushID((int)(((Symbol{ mem }.GetCRC()) ^ mcd->mHash) & 0xFFFF'FFFF));
				bOpenSelected = ImGui::Button("Open Selected Mapping");
				ImGui::PopID();
				ImGui::ListBox(GenNextID(), &arr->selected, &prop_getter, &del, pProps->GetNumKeys());

				/*Do not remove this, important as it the child window needs a unique ID even if theres code after this if else if block*/
				GenNextID();
				if (arr->mbShouldClose) {
					arr->mbSelectedOpen = false;
					ResetChild();
					arr->mbShouldClose = false;
				}
				else if (arr->mbSelectedOpen || (pProps->GetNumKeys() && bOpenSelected)) {
					if (!arr->mbSelectedOpen) {
						arr->mbSelectedOpen = true;
						//just been selected
					}
					
					PropertySet::KeyInfo* pSelected = pPropKeys + arr->selected;
					
					std::string* pResolved = mpTask->ResolveHash(pSelected->mKeyName.GetCRC(), prop_key);

					if (pSelected->mKeyName.GetCRC()) {
						if(!pResolved)
							sprintf(arr->name_buf, "Symbol<%llX>", pSelected->mKeyName.GetCRC());
					}
					else {
						sprintf(arr->name_buf, "No Key Name");
					}

					RenderChild(pSelected->mpValue->mpValue, pSelected->mpValue->mpDataDescription, mIDGen, &arr->mbShouldClose,
						pResolved ? pResolved->c_str() : arr->name_buf);
				}

				ImGui::Unindent(5.f);

			}
			else {

				u64 hash = mcd->mHash;
				if (hash == sphere_str || hash == vec2_str || hash == vec3_str || hash == vec4_str || hash == tran_str || hash == col_str
					|| hash == quat_str) {
					render_intrin_mcd_math(mem, mcd, pInstance, depth);
				}
				else {
					return false;//nested so dont
				}
			}
			return true;
		}
		return true;
	}

	static bool mesh_cb(void* pd, int idx, const char** pout) {
		//DCArray<HandleUncached>* pArray = (DCArray<HandleUncached>*)pd;
		char* buf = ((char*)pd);
		sprintf(buf, "[%d]", idx);
		*pout = buf;
		return true;
	}

	void render_mcd(MetaClassDescription* mcd, void* pInstance, float depth = 0.0f) {
		MetaMemberDescription* pMem = mcd->mpFirstMember;
		if (!render_mcd_internal(mcd->mpTypeInfoName, mcd, pInstance, depth)){
			ImGui::Indent(depth);
			while (pMem) {
				if (MetaVersion_MemberExistsInCurrentVersion(pMem, GetStreamHelper(mpTask))) {
					if (pMem->mFlags & MetaFlag_EditorHide) {
						ImGui::Text("The variable below is internal to the engine. Use it at your own risk.");
					}
					ImGui::NewLine();
					const char* mem = pMem->mpName;

					//D3DMesh mInternalResources workaround
					if (mcd->mHash == 0x98a19836cf4ccb7d && strlen(mem) >= 18 && !memcmp(mem, "mInternalResources", 18)) {
						DCArray<HandleUncached>& handles = ((D3DMesh*)((char*)pInstance))->mInternalResources;
						ImGui::Indent(5.f);
						
						ImGui::Text("Mesh Internal Resources");

						PersistentData& persist = mPersistentData[MakePersistentDataID(mcd, &handles)];
						if (persist.pDataInstance == nullptr) {
							persist.mDealloc = &array_persist_del;
							persist.pDataInstance = new ArrayPersistance{ 0 };
						}
						ArrayPersistance* arr = (ArrayPersistance*)persist.pDataInstance;

						bool bOpenSelected{};
						ImGui::PushID((int)(((Symbol{ mem }.GetCRC()) ^ ((uint64_t) & handles)) & 0xFFFF'FFFF));
						bOpenSelected = ImGui::Button("Open Selected Element");
						ImGui::PopID();
						ImGui::ListBox(GenNextID(), &arr->selected, &mesh_cb, (void*)arr->name_buf, handles.GetSize());

						/*Do not remove this, important as it the child window needs a unique ID even if theres code after this if else if block*/
						GenNextID();
						if (arr->mbShouldClose) {
							arr->mbSelectedOpen = false;
							ResetChild();
							arr->mbShouldClose = false;
						}
						else if (arr->mbSelectedOpen || (handles.GetSize() && bOpenSelected)) {
							if (!arr->mbSelectedOpen) {
								arr->mbSelectedOpen = true;
								//just been selected
							}
							HandleUncached* pUncached = handles.mpStorage + arr->selected;

							std::string* pResolved = mpTask->ResolveHash(pUncached->mHandleObjectInfo.mObjectName.GetCRC(), prop_key);
							
							char buf[32];
							buf[0] = 0;
							if (!pResolved && pUncached->mHandleObjectInfo.mObjectName.GetCRC()) {
								sprintf(buf, "Symbol<%llX>", pUncached->mHandleObjectInfo.mObjectName.GetCRC());
							}

							RenderChild(pUncached->GetHandleObjectPointer(), pUncached->GetTypeDesc(), mIDGen, &arr->mbShouldClose, 
								pResolved ? pResolved->c_str() : buf[0] == 0 ? "Unnamed Resource" : buf);
						}

						ImGui::Unindent(5.f);
					}
					//T3Texture mSamplerState workaround
					else if (mcd->mHash == 0x3E249E7E6F38CCE2 && strlen(mem) >= 13 && !memcmp(mem, "mSamplerState", 13)) {
						T3SamplerStateBlock& pStateBlock = ((T3Texture*)((char*)pInstance))->mSamplerState;
						ImGui::Text("Texture Sampler State | NOTE: The values only apply to PC versions (D3D11) - ensure the texture platform!");
						ImGui::Indent(5.0f);

						u32 wrapU = (u8)pStateBlock.InternalGetSamplerState(eSamplerState_WrapU_Value); // 1 NIBBLE 
						u32 wrapV = (u8)pStateBlock.InternalGetSamplerState(eSamplerState_WrapV_Value); // 1 NIBBLE 
						bool bFiltered = pStateBlock.InternalGetSamplerState(eSamplerState_Filtered_Value) == 0 ? false : true;
						bool bGC = pStateBlock.InternalGetSamplerState(eSamplerState_GammaCorrect_Value) == 0 ? false : true;//gamma correct
						signed int mipBias = (signed int)(pStateBlock.InternalGetSamplerState(eSamplerState_MipBias_Value) & 0xFF);//1 BYTE
						u8 border = (u8)pStateBlock.InternalGetSamplerState(eSamplerState_BorderColor_Value); // 1 NIBBLE, 0 = black 1 = white

						//wrapU and V: 0 = clamp, 1 = wrap, 2 = border
						int acMipBias = (-mipBias) >> 2;
						bool bBorder = border == 1;
						
						ImGui::Checkbox("Gamma Correct", &bGC);
						ImGui::Checkbox("Is Filtered", &bFiltered);
						ImGui::InputInt("Mip Map Bias", &acMipBias, 1);

						if (border <= 1) {
							ImGui::Checkbox("Border Colour [1=White,0=Black]", &bBorder);
							pStateBlock.InternalSetSamplerState(eSamplerState_BorderColor_Value, bBorder ? 1 : 0);
						}//ELSE BORDER COLOUR IS DIFFERENT!

						pStateBlock.InternalSetSamplerState(eSamplerState_GammaCorrect_Value, bGC ? 1 : 0);
						pStateBlock.InternalSetSamplerState(eSamplerState_Filtered_Value, bFiltered ? 1 : 0);

						//mip bias 
						mipBias = (-acMipBias) << 2;
						if (mipBias > 127)
							mipBias = 127;
						else if (mipBias < -127)
							mipBias = -127;
						pStateBlock.InternalSetSamplerState(eSamplerState_MipBias_Value, (unsigned int)(mipBias & 0xFFFFFFFF));

						if (wrapU <= 2 && wrapV <= 2) {
							//now wraps
							ImGui::Combo("U Wrap Mode", (int*)&wrapU, kD3DTextureWraps, 3);
							ImGui::Combo("V Wrap Mode", (int*)&wrapV, kD3DTextureWraps, 3);

							if (wrapV > 2)
								wrapV = 0;
							if (wrapU > 2)
								wrapU = 0;

							pStateBlock.InternalSetSamplerState(eSamplerState_WrapU_Value, wrapU);
							pStateBlock.InternalSetSamplerState(eSamplerState_WrapV_Value, wrapV);

						}

						ImGui::Unindent(5.0f);
					}
					else if (pMem->mFlags & MetaFlag_FlagType) {
						render_intrin_mcd_flags(mem, pMem->mpMemberDesc, (void*)((char*)pInstance + pMem->mOffset), depth, pMem);
					}
					else if ((pMem->mFlags & MetaFlag_EnumIntType) || (pMem->mFlags & MetaFlag_EnumWrapperClass)) {
						render_intrin_mcd_enum(mem, pMem->mpMemberDesc, (void*)((char*)pInstance + pMem->mOffset), depth, pMem);
					}
					else {
						if (!render_mcd_internal(mem, pMem->mpMemberDesc, (void*)((char*)pInstance + pMem->mOffset), depth)) {
							render_mcd(pMem->mpMemberDesc, (void*)((char*)pInstance + pMem->mOffset), depth + 10.f);
							ImGui::Separator();
						}
					}
				}
				pMem = pMem->mpNextMember;
			}
			ImGui::Unindent(depth);
		}
	}

	void Render(int current_id, float depth) {
		if (mHeaderStr.size() > 0) {
			ImGui::Text("Property Key: %s", mHeaderStr.c_str());
			ImGui::Text("Property Value:");
		}
		mIDGen = 0;
		mbChildRendered = false;
		mBaseIDHash = CRC64(0, (const char*)&current_id, 4);
		render_mcd(mpTypeDesc, mpInstance, depth);
		mbChildRendered = false;
	}

};

void MetaStreamSubType::render_mcd_array(const char* mem, u32 arraySize, void* pArray, MetaClassDescription* pArrayType, float depth) {

	//id_gen ONLY ACCESS BEFORE CALLING GEN_ID !!
	PersistentData& persist = mPersistentData[MakePersistentDataID(pArrayType, pArray)];
	if (persist.pDataInstance == nullptr) {
		persist.mDealloc = &array_persist_del;
		persist.pDataInstance = new ArrayPersistance{ 0 };
	}
	ArrayPersistance* arr = (ArrayPersistance*)persist.pDataInstance;

	//render the array and selected
	bool bOpenSelected{};
	ImGui::Text("%s | Array of %s values", mem, pArrayType->mpTypeInfoName);
	ImGui::PushID((int)(((Symbol{ mem }.GetCRC()) ^ pArrayType->mHash) & 0xFFFF'FFFF));
	bOpenSelected = ImGui::Button("Open Selected Element");
	ImGui::PopID();
	ImGui::ListBox(GenNextID(), &arr->selected, &dcarray_getter, (void*)arr->name_buf, arraySize);

	/*Do not remove this, important as it the child window needs a unique ID even if theres code after this if else if block*/
	GenNextID();
	if (arr->mbShouldClose) {
		arr->mbSelectedOpen = false;
		ResetChild();
		arr->mbShouldClose = false;
	}
	else if (arr->mbSelectedOpen || (arraySize && bOpenSelected)) {
		if (!arr->mbSelectedOpen) {
			arr->mbSelectedOpen = true;
			//just been selected
		}
		char* pArrayCh = (char*)pArray;
		u32 cSize = pArrayType->mClassSize;
		u32 cAlign = pArrayType->mClassAlign;
		if (((size_t)pArrayCh % cAlign) != 0){
			MessageBoxA(0, "Serious error has occured when rendering array! Address of array is not aligned to type align!"
				" Please contact me with this error asap!", pArrayType->mpTypeInfoName,
				MB_ICONERROR);
			arr->mbShouldClose = true;
			arr->mbSelectedOpen = false;
			return;
		}
		u32 paddingPerInst = cAlign - (cSize % cAlign);
		//cSize += paddingPerInst;
		void* pArrayItem = pArrayCh + cSize * arr->selected;
		RenderChild(pArrayItem, pArrayType, mIDGen, &arr->mbShouldClose);
	}
}

MetaStreamTask::MetaStreamTask(DataStream* pStream, std::string fileName, const char* gameid) : InspectorTask(fileName.c_str(), sRuntime.gen_id()) {
	this->gameid = gameid;
	mpType = nullptr;
	mpInstance = nullptr;
	mFileName = fileName;
	pSourceStream = pStream;
	mpRoot = nullptr;
}

MetaStreamTask::~MetaStreamTask() {
	if (pSourceStream)
		delete pSourceStream;
	pSourceStream = nullptr;
	gameid = nullptr;
	if (mpType && mpInstance)
		mpType->Delete(mpInstance);
	mpInstance = nullptr;
	mpType = nullptr;
	if (mpRoot)
		delete mpRoot;
}

void MetaStreamSubType::render_mcd_std_cont(const char* mem, MetaClassDescription* mcd, void* pInst, float depth) {

	PersistentData& persist = mPersistentData[MakePersistentDataID(mcd, pInst)];
	if (persist.pDataInstance == nullptr) {
		persist.mDealloc = &array_persist_del;
		persist.pDataInstance = new ArrayPersistance{ 0 };
	}
	ArrayPersistance* arr = (ArrayPersistance*)persist.pDataInstance;

	//render the array and selected
	bool bOpenSelected{};

	ContainerRenderAdapter* pRenderAdapter = static_cast<ContainerRenderAdapter*>(pInst);

	GenNextID();
	MetaClassDescription* pOutType{};
	void* pOutSelected{};
	const char* n{};
	u32 csize{};
	pRenderAdapter->_RenderInspector(mem, mID, &pOutType, &pOutSelected,&n,&csize, &arr->selected);

	if (pOutType == nullptr) {
		ImGui::Text("Container contains types which cannot be rendered in the UI (likely pointers, dont worry!)");
		return;
	}

	ImGui::Text("%s | %s of %s values", mem, n, pOutType->mpTypeInfoName);
	
	ImGui::PushID((int)(((Symbol{ mem }.GetCRC()) ^ mcd->mHash) & 0xFFFF'FFFF));
	bOpenSelected = ImGui::Button("Open Selected Element");
	ImGui::PopID();

	/*Do not remove this, important as it the child window needs a unique ID even if theres code after this if else if block*/
	GenNextID();
	if (arr->mbShouldClose) {
		arr->mbSelectedOpen = false;
		ResetChild();
		arr->mbShouldClose = false;
	}
	else if (arr->mbSelectedOpen || (csize && bOpenSelected)) {
		if (!arr->mbSelectedOpen) {
			arr->mbSelectedOpen = true;
			//just been selected
		}
		void* pArrayItem = pOutSelected;
		RenderChild(pArrayItem, pOutType, mIDGen, &arr->mbShouldClose);
	}
}

void MetaStreamSubType::render_mcd_map(const char* mem, MetaClassDescription* mcd, void* pInst, float depth) {
	ContainerRenderAdapter* pRenderAdapter = static_cast<ContainerRenderAdapter*>(pInst);
	RenderInspectorProxyOutInst kv{};
	RenderInspectorProxyOutType kvt{};

	PersistentData& persist = mPersistentData[MakePersistentDataID(mcd, pInst)];
	if (persist.pDataInstance == nullptr) {
		persist.mDealloc = &array_persist_del;
		persist.pDataInstance = new ArrayPersistance{ 0 };
	}
	ArrayPersistance* arr = (ArrayPersistance*)persist.pDataInstance;

	GenNextID();
	u32 size{};
	pRenderAdapter->_RenderInspector(mem, mID, (MetaClassDescription**)&kvt, (void**)&kv, nullptr, &size, &arr->selected);
	
	if (kvt.pOutKeyType == nullptr || kvt.pOutValType == nullptr) {
		ImGui::Text("Map Contains types which cannot be rendered in the UI (likely pointers, dont worry!)");
		return;
	}

	ImGui::Text("%s | Map of %s to %s", mem, kvt.pOutKeyType->mpTypeInfoName, kvt.pOutValType->mpTypeInfoName);
	
	ImGui::PushID((int)(((Symbol{ mem }.GetCRC()) ^ mcd->mHash) & 0xFFFF'FFFF));
	bool bOpenSelected = ImGui::Button("Open Selected Element");
	ImGui::PopID();

	/*Do not remove this, important as it the child window needs a unique ID even if theres code after this if else if block*/
	GenNextID();
	if (arr->mbShouldClose) {
		arr->mbSelectedOpen = false;
		ResetChild();
		arr->mbShouldClose = false;
	}
	else if (arr->mbSelectedOpen || (size && bOpenSelected)) {
		if (!arr->mbSelectedOpen) {
			arr->mbSelectedOpen = true;
			//just been selected
		}
		
		MCDRenderMapProxy proxy{};
		proxy.pKeyInst = kv.mpKeyInst;
		proxy.pValInst = kv.mpValInst;
		proxy.pKeyType = kvt.pOutKeyType;
		proxy.pValType = kvt.pOutValType;

		RenderChild(&proxy, nullptr, mIDGen, &arr->mbShouldClose);
	}

}

void MetaStreamTask::_render() {
	if (mFileName.size() > 0) {
		ImGui::Text("File Information:");
		ImGui::Text("%s: belongs to %s", mFileName.c_str(), sBlowfishKeys[TelltaleToolLib_GetGameKeyIndex(gameid)].game_name);
		if (mpType == nullptr && !mbInvalidMS) {
			mStream.mVersionInfo.clear();
			TelltaleToolLib_SetBlowfishKey(gameid);
			mStream.Open(pSourceStream, MetaStreamMode::eMetaStream_Read, {});
			if (mpRoot)
				mpRoot->Reset();
			else
				mpRoot = new MetaStreamSubType();
			pSourceStream = nullptr;
			if (mStream.mbErrored) {
				mbInvalidMS = true;
				goto bad__;
			}
			std::string ext = std::filesystem::path{ mFileName }.extension().string().substr(1);
			MetaClassDescription* pShouldBe = TelltaleToolLib_FindMetaClassDescription(ext.c_str(), false);
			if (pShouldBe == nullptr) {
				mbUnknownFile = true;
				mbInvalidMS = true;
				goto bad__;
			}
			if (mStream.mVersionInfo.size() == 0) {
				mbInvalidMS = true;
				mbIsEmpty = true;
				goto bad__;
			}
			MetaClassDescription* pFirstDesc = TelltaleToolLib_FindMetaClassDescription_ByHash(mStream.mVersionInfo[0].mTypeSymbolCrc);
			if (pFirstDesc == nullptr) {
				mbUnknownFile = true;
				mbInvalidMS = true;
				goto bad__;
			}
			if (pFirstDesc != pShouldBe) {
				MessageBoxA(0, "The file you are trying to load has the wrong extension! The correct extension will be shown after!"
					, "Whats happened here...", MB_ICONWARNING);
			}
			if (pFirstDesc->mpExt == nullptr)
				mpType = pShouldBe;
			else
				mpType = pFirstDesc;
			mpInstance = mpType->New();
			if (!mpInstance) {
				mbMemErr = true;
				mbInvalidMS = true;
				goto bad__;
			}
			u32 libVersion = SerializedVersionInfo::RetrieveCompiledVersionInfo(mpType)->mVersionCrc;
			u32 fileVersion = mStream.mVersionInfo[0].mVersionCrc;
			if (libVersion != fileVersion) {
				if (MessageBoxA(0, "WARNING: The file you are opening has a different version than the library supports. This is "
					"a very common warning and is likely because this app was made using the walking dead definitive series, and "
					"the file you are opening likely belongs to a game before that. You can, at your risk, try and open it anyway but"
					" it is possible that either the app with crash (your memory usage will explode!) or it might work. Contact me if you"
					" want a specific game support for this type of file. Click yes to open anyway.", "WARNING!",
					MB_ICONWARNING | MB_YESNO) != IDYES) {
					mStream.Close();
					mpType->Delete(mpInstance);
					mpInstance = nullptr;
					mbVerCancel = true;
					mbInvalidMS = true;
					mpType = nullptr;
					goto bad__;
				}
			}
			MetaOpResult result = PerformMetaSerializeFull(&mStream, mpInstance, mpType);
			if (result != MetaOpResult::eMetaOp_Succeed) {
				mbSerializeErr = true;
				mbInvalidMS = true;
				mpType->Delete(mpInstance);
				mpInstance = nullptr;
				mpType = nullptr;
				goto bad__;
			}
			mVersName = SerializedVersionInfo::MakeVersionFileName(mpType->mpTypeInfoName, mStream.mVersionInfo[0].mVersionCrc);
			mpRoot->Init(this, mpInstance, mpType);
		}
		if (mbInvalidMS) {
		bad__:
			ImGui::NewLine();
			if (mbIsEmpty) {
				ImGui::Text("The meta stream file is empty! This is not an error, it just doesn't have any data in it lol");
			}
			else if (mbSerializeErr)
				ImGui::Text("This file could not be loaded as it contains unknown types. Please contact me so I can add support."
					"\nYou may see this error a lot, thanks OK and it will get better over time!");
			else if (mbUnknownFile)
				ImGui::Text("The file type is unknown, although it is a meta stream. Please contact with me with the file!");
			else if (mbMemErr)
				ImGui::Text("The file could not be loaded as it could be loaded in memory. Please contact me with your PC and the file.");
			else if (mbVerCancel)
				ImGui::Text("The file load was cancelled due to version issues. Go to Help => Contact Me for help.");
			else
				ImGui::Text("The file is invalid, or is not a meta stream file. Please close this task and see contact help for help.");
			return;
		}
		ImGui::NewLine();
		if (mpType->mpExt)
			ImGui::Text("Meta Stream Type: %s (.%s files)", mpType->mpTypeInfoName, mpType->mpExt);
		else
			ImGui::Text("Meta Stream Type: %s", mpType->mpTypeInfoName);
		ImGui::Text("Meta Stream Version: %s", VersionHeadersStr[mStream.mStreamVersion]);
		ImGui::Text("Meta Stream Type Version File: %s\nMeta Stream Type Version: 0x%08X (file has version 0x%08X)",
			mVersName.c_str(), SerializedVersionInfo::RetrieveCompiledVersionInfo(mpType)->mVersionCrc, mStream.mVersionInfo[0].mVersionCrc);
		ImGui::SameLine();
		if (ImGui::Button("Save Version File")) {
			nfdchar_t* outp{};
			if (NFD_PickFolder(0, &outp, L"Select Destination Folder") == NFD_OKAY) {
				std::filesystem::path pth{ outp };
				pth += '/';
				std::string name{};
				DataStream* pStream = SerializedVersionInfo::RetrieveCompiledVersionInfo(mpType)->Save(&name);
				if (pStream) {
					pth += name;
					DataStreamFileDisc out = _OpenDataStreamFromDisc_(pth.string().c_str(), WRITE);
					if (out.IsInvalid()) {
						MessageBoxA(0, "Could not open the destination file! Contact me!", "Error", MB_ICONERROR);
					}
					else {
						pStream->SetMode(DataStreamMode::eMode_Read);
						pStream->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
						pStream->Transfer(&out, 0, pStream->GetSize());
						MessageBoxA(0, "Successfully exported the version file!", "Success", MB_ICONINFORMATION);
					}
					delete pStream;
				}
				else {
					MessageBoxA(0, "Could not create the vers file. Please contact me.", "Error", MB_ICONERROR);
				}
			}
		}

		//display the contents of the file

		const char* beforeID = TelltaleToolLib_GetBlowfishKey();
		TelltaleToolLib_SetBlowfishKey(gameid);

		ImGui::Separator();
		ImGui::Text("File Data:");
		if (ImGui::Button("Export & Save")) {
			nfdchar_t* out{};
			if (NFD_PickFolder(0, &out, L"Select Output Folder") == NFD_OKAY) {
				std::filesystem::path p{ out };
				p += "/";
				p += mFileName;
				DataStreamFileDisc outFStream = _OpenDataStreamFromDisc_(p.string().c_str(), WRITE);
				if (outFStream.IsInvalid()) {
					MessageBoxA(0, "Could not open the output file!", "Error", MB_ICONERROR);
				}
				else {
					MetaStream outStream{};
					outStream.mbDontDeleteStream = true;
					outStream.InjectVersionInfo(mStream);
					outStream.Open(&outFStream, MetaStreamMode::eMetaStream_Write, {});
					MetaOpResult result = PerformMetaSerializeFull(&outStream, mpInstance, mpType);
					if (result != eMetaOp_Succeed) {
						MessageBoxA(0, "There was a problem writing the file (meta stream result!=ok). Please contact me", "Error writing",
							MB_ICONERROR);
						outStream.mpReadWriteStream = nullptr;
					}
					else {
						outStream.Close();
						MessageBoxA(0, "Successfully exported the file!", "Done!", MB_ICONINFORMATION);
					}
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Export As JSON")) {
			nfdchar_t* out{};
			if (NFD_PickFolder(0, &out, L"Select Output Folder") == NFD_OKAY) {
				std::filesystem::path p{ out };
				p += "/";
				p += mFileName;
				p += ".json";
				DataStreamFileDisc outFStream = _OpenDataStreamFromDisc_(p.string().c_str(), WRITE);
				if (outFStream.IsInvalid()) {
					MessageBoxA(0, "Could not open the output file!", "Error", MB_ICONERROR);
				}
				else {
					if (MessageBoxA(0, "Please note exporting JSON could take a significant amount of time! I have to search the file name database"
						" so that file names are not hashes but actual file names! Click OK to continue..",
						"You have been warned lol", MB_ICONINFORMATION) == IDOK) {
						MetaStream_JSON outStream{ MetaStream_JSON::eJSONMode::eCompleteMetaStream };
						outStream.mbDontDeleteStream = true;
						outStream.InjectVersionInfo(mStream);
						outStream.Open(&outFStream, MetaStreamMode::eMetaStream_Write, {});
						MetaOpResult result = PerformMetaSerializeFull(&outStream, mpInstance, mpType);
						if (result != eMetaOp_Succeed) {
							MessageBoxA(0, "There was a problem writing the file (meta stream result!=ok). Please contact me", "Error writing",
								MB_ICONERROR);
							outStream.mpReadWriteStream = nullptr;
						}
						else {
							outStream.Close();
							MessageBoxA(0, "Successfully exported the file!", "Done!", MB_ICONINFORMATION);
						}
					}
				}
			}
		}
		ImGui::Text("If you would like to resolve hashes, click here: ");
		ImGui::SameLine();
		if (ImGui::Button("Resolve Hashes") && !mbHasResolved) {
			if (MessageBoxA(0, "Resolving hashes can take time! Are you sure? Resolving hashes will mean all 'Symbol<..> hashes are converted"
				" mostly to file names and property keys used.", "Time warning", MB_ICONINFORMATION | MB_YESNO) == IDYES) {
				mbHasResolved = true;
			}
		}

		mpRoot->Render(0, 0.f);

		TelltaleToolLib_SetBlowfishKey(beforeID);

	}
}

std::string* MetaStreamTask::ResolveHash(u64 hash, resolve_mode md) {
	if (hash == 0)
		return nullptr;
	if (mbHasResolved) {
		std::string& resolved = mResolvedHashes[hash];
		if (resolved == "__UNKHPRXY__")//enough to be used in ss buffer
			return nullptr;//not found
		if (resolved.length() == 0) {
			//find the new hash
			if (md == check_all) {
				resolve_buf[0] = 0;
				TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(0, hash, resolve_buf);
				if (resolve_buf[0] == 0) {
					//could not find
					resolved = "__UNKHPRXY__";
					return nullptr;
				}
				else {
					resolved = resolve_buf;
				}
			}
			else if (md == prop_key) {
				resolve_buf[0] = 0;
				TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(
					TelltaleToolLib_GetGlobalHashDatabase()->FindPage("PropertySet-Keys"), hash, resolve_buf);
				if (resolve_buf[0] == 0) {
					//could not find
					resolved = "__UNKHPRXY__";
					return nullptr;
				}
				else {
					resolved = resolve_buf;
				}
			}
			else if (md == file_name) {
				HashDatabase* db = TelltaleToolLib_GetGlobalHashDatabase();
				if (mDBFilenamePages.size() == 0) {
					char tmp[32];
					sprintf(tmp, "%s", TelltaleToolLib_GetBlowfishKey());
					std::transform(tmp, tmp + strlen(tmp), tmp, ::toupper);
					//find all pages name 'Files_GAMENAME_***'
					mDBFilenamePages.reserve(50);//around ~50 types of files
					HashDatabase::Page* pg = nullptr;
					int idx = 0;
					do {
						if (idx >= db->NumPages())
							break;
						pg = db->PageAt(idx);
						if (ends_with(pg->pageName, tmp)) {
							mDBFilenamePages.push_back(idx);
						}
						idx++;
					} while (pg != nullptr);
				}
				resolve_buf[0] = 0;
				for (auto pgit = mDBFilenamePages.begin(); pgit != mDBFilenamePages.end(); pgit++) {
					HashDatabase::Page* pg = db->PageAt(*pgit);
					db->FindEntry(pg, hash, resolve_buf);
					if (resolve_buf[0] != 0) {
						resolved = resolve_buf;
						return &resolved;
					}
				}
				resolved = "__UNKHPRXY__";
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

inline MetaStream* GetStreamHelper(MetaStreamTask* pTask) {
	return &pTask->mStream;
}

void getname(std::string& dest, TTArchive2& arc, TTArchive2::ResourceEntry* pEntry, const std::filesystem::directory_entry& entry) {
	std::string tmp{entry.path().filename().string()};
	if (tmp.length() == 0)
		tmp = "<ArchiveNameInvalid>";
	tmp += "/";
	std::string name{};
	arc.GetResourceName(pEntry->mNameCRC, &name);
	dest = tmp + name;
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
			printf("%s : starting walk\n", archp.path().filename().string().c_str());
			TTArchive2 arch{};
			DataStream* stream = OpenDataStreamFromDisc(archp.path().string().c_str(), READ);
			if (((DataStreamFile_Win*)stream)->mHandle == 0) {
				printf("too large, %s TODO", archp.path().string().c_str());
				continue;
			}
			arch.Activate(stream);
			DataStream* pResource{};
			for (auto& file : arch.mResources) {
				arch.GetResourceName(file.mNameCRC, &name);
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
				printf("%s : no files or invalid\n", archp.path().filename().string().c_str());
			}
			else {
				printf("%s : done\n", archp.path().filename().string().c_str());
			}
		}
	}
	printf("dumping...\n");
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
	printf("\ndone!\n");
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
		if (TelltaleToolLib_FindMetaClassDescription_ByHash(pClass->mHash) != nullptr)
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
		printf("- write code: %s\n", (const char*)pClass->mpTypeInfoName);
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
	printf("-done\n");
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
		printf("- write %s\n", (const char*)pClass->mpTypeInfoName);
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
	printf("-done\n");
}

class ContainerTask : public InspectorTask {

	std::string file;
	DataStreamContainer* container;
	unsigned long long size = 0;

	virtual void _render() override {
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
				DataStreamFile_Win out = _OpenDataStreamFromDisc_(o, WRITE);
				container->Transfer(&out, 0, size);
				free(o);
				MessageBoxA(0, "Successfully extracted data stream container data!", "Finished", MB_ICONINFORMATION);
			}
		}
	}

public:

	ContainerTask(const char* tt, int id) : InspectorTask(tt, id) {
		container = 0;
	}

	~ContainerTask() {
		if (container)
			delete container;
	}

	deleteme(ContainerTask)

};

struct ResourceSetDescription {
	std::string mLogicalName, mLogicalDest, mName, mSetName, mDescOverride;
	int mPriority, mGameDataPriority;
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
	};

	Type mType;
	std::string mGameDataPathPost;
	ResourceSetDescription mResourceSetDesc;
	std::unique_ptr<MetaStream> mpSingleFileMS;
	std::string mName;

	std::string& GetDisplayName() {
		return mType == RESOURCE_SET_DESCRIPTION ? mResourceSetDesc.mName : mName;
	}

};

class GameEditorTask : public InspectorTask {
public:
	std::filesystem::path base_path{}, game_data{};
	std::filesystem::path packs{};

	class TempBuffer {
		char* mpBuffer;
		GameEditorTask* mpTask;
	public:
		char* GetBuffer() const {
			return mpBuffer;
		}
		TempBuffer(GameEditorTask* pTask, u32 size) : mpTask(pTask) {
			if (size > 4096)
				mpBuffer = new char[size];
			else
				mpBuffer = pTask->working_buffer;
		}
		~TempBuffer() {
			if (mpBuffer != mpTask->working_buffer)
				delete[] mpBuffer;
			mpBuffer = 0;
		}
	};

	std::string mError{};
	std::vector<std::string> mVersionStrs;
	std::vector<GameEditor_DiskResourceSet> mResourceSets;
	bool mbInit = false;
	bool bSelecting = true;

	int mGameKeyIndex = 0;
	const char* mpGameID = nullptr;
	BlowfishKey mGameKey{0};
	bool bLegacyEngine = false;
	void* mpLuaArg = nullptr;

	char working_buffer[4096];

	void _init_diskres(GameEditor_DiskResourceSet& set) {
		if (set.mType == GameEditor_DiskResourceSet::RESOURCE_SET_DESCRIPTION) {
			std::string f = std::string{ game_data.string() + set.mGameDataPathPost };
			DataStreamFile_Win stream = _OpenDataStreamFromDisc_(f.c_str(), READ);
			TempBuffer tmp{ this, (u32)stream.GetSize() };
			stream.Serialize(tmp.GetBuffer(), stream.GetSize());
			bool bRemove = false;
			if (set.mName._Starts_with("_rescdesc")) {
				//version one
				bRemove = true;
				ensure_reg("GameEngine_AddBuildVersionInfo", luaAddBuildVersionInfo);
			}
			else {
				char* dec;
				u32 o{};
				if (ends_with(set.mName, ".lenc"))
					dec = (char*)TelltaleToolLib_DecryptLencScript((u8*)tmp.GetBuffer(), stream.GetSize(), &o);
				else
					dec = (char*)TelltaleToolLib_DecryptScript((u8*)tmp.GetBuffer(), stream.GetSize(), &o);
				memcpy(tmp.GetBuffer(), dec, o);
				free(dec);
				ensure_reg("RegisterSetDescription", luaRegisterSet);
			}
			mpLuaArg = &set;
			lua_pushstring(sRuntime.L, "");
			lua_setglobal(sRuntime.L, "_currentDirectory");
			luaL_dostring(sRuntime.L, tmp.GetBuffer());
			if (bRemove)
				mResourceSets.pop_back();
		}
		else if (set.mType == GameEditor_DiskResourceSet::SINGLE_FILE) {

		}//else ttarch, TODO NORMAL TTARCH FILE
	}

	void _init_load_resource_sets(std::string current_folder, std::string current_app) {
		for (auto& file : std::filesystem::directory_iterator{ current_folder }) {
			if (file.is_regular_file()) {
				std::string name = file.path().filename().string();
				if (!ends_with(name, ".ttarch2")) {
					GameEditor_DiskResourceSet& set = mResourceSets.emplace_back();
					set.mName = name;
					set.mGameDataPathPost = current_app + "/" + name;
					if (ends_with(name, ".ttarch"))
						set.mType = GameEditor_DiskResourceSet::DATA_ARCHIVE;
					else if (ends_with(name, ".lua") || ends_with(name, ".lenc"))
						set.mType = GameEditor_DiskResourceSet::RESOURCE_SET_DESCRIPTION;
					else
						set.mType = GameEditor_DiskResourceSet::SINGLE_FILE;
					_init_diskres(set);
				}
			}
			else if (file.is_directory()) {
				_init_load_resource_sets(file.path().string(), current_app + "/" + file.path().filename().string());
			}
		}
	}

	virtual void _render() override {
		sRuntime.current_editor_task = this;
		if (mError.size() > 0) {
			ImGui::Text("%s", mError.c_str());
		}
		else {
			if (!mbInit) {
				if (bSelecting) {
					ImGui::Text("Select the game which you want to open the editor for.");
					mpGameID = select_gameid_dropdown(mpGameID);
					if (ImGui::Button("Select") && mpGameID)
						bSelecting = false;
					else
						return;
				}
				TelltaleToolLib_SetBlowfishKey(mpGameID);
				mbInit = true;
				std::string bps = base_path.string();
				if (bps.length() == 0) {
					mError = "Invalid base path: no file specified";
					return;
				}
				if (!(bps[bps.length() - 1] == '/' || bps[bps.length() - 1] == '\\'))
					bps += '/';
				base_path = bps;
				memcpy((void*)&mGameKey, (const void*)&sBlowfishKeys[mGameKeyIndex = TelltaleToolLib_GetGameKeyIndex(mpGameID)], sizeof(BlowfishKey));
				game_data = base_path.string() + std::string{mGameKey.game_data_location};
				if (!std::filesystem::exists(base_path)) {
					mError = "Could not locate game data: " + game_data.string();
					return;
				}
				int TWUA = TelltaleToolLib_GetGameKeyIndex("fables");
				if (mGameKeyIndex >= TWUA) {
					//LOAD RESDESCS
					_init_load_resource_sets(game_data.string(),"");
				}
				else {
					bLegacyEngine = true;
					//GENERATE RESDESCS
					//CHECK, GAME DATA MAY BE A FILE
				}
			}
			TelltaleToolLib_SetBlowfishKey(mpGameID);
			//render here

		}
	}

public:

	void Init(std::filesystem::path base) {
		base_path = base;
		memset(working_buffer, 0, sizeof(working_buffer));
	}

	deleteme(GameEditorTask)

	taskctor(GameEditorTask) {}

};

static void dumpstack(lua_State* L) {
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {
		printf("%d\t%s\t", i, luaL_typename(L, i));
		switch (lua_type(L, i)) {
		case LUA_TNUMBER:
			printf("%g\n", lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			printf("%s\n", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
			break;
		case LUA_TNIL:
			printf("%s\n", "nil");
			break;
		default:
			printf("%p\n", lua_topointer(L, i));
			break;
		}
	}
}

static int luaRegisterSet(lua_State* L) {
	GameEditorTask* cur = sRuntime.current_editor_task;
	if (cur) {
		GameEditor_DiskResourceSet* set = (GameEditor_DiskResourceSet*)cur->mpLuaArg;
		/*lua_getfield(L, 1, "name");
		lua_getfield(L, 1, "setName");
		lua_getfield(L, 1, "descriptionFilenameOverride");
		lua_getfield(L, 1, "logicalName");
		lua_getfield(L, 1, "logicalDestination");
		lua_getfield(L, 1, "priority");
		lua_getfield(L, 1, "enableMode");
		lua_getfield(L, 1, "version");
		lua_getfield(L, 1, "descriptionPriority");
		lua_getfield(L, 1, "gameDataName");
		lua_getfield(L, 1, "gameDataPriority");
		lua_getfield(L, 1, "gameDataEnableMode");
		lua_getfield(L, 1, "localDirIncludeBase");
		lua_getfield(L, 1, "localDirRecurse");
		lua_getfield(L, 1, "localDirIncludeOnly");
		lua_getfield(L, 1, "localDirExclude");*/
		
		/*game data arcs*/
		lua_getfield(L, -1, "gameDataArchives");
		int n = lua_rawlen(L, -1);
		set->mResourceSetDesc.mGameDataArchives.clear();
		set->mResourceSetDesc.mGameDataArchives.reserve(n);
		for (int i = 0; i < n; i++) {
			lua_pushnumber(L, i);
			lua_gettable(L, -2);
			const char* tn = lua_typename(L, lua_type(L, -1));
			const char* val = lua_tolstring(L, -1, 0);
			dumpstack(L);
			set->mResourceSetDesc.mGameDataArchives.push_back(val);
			lua_settop(L,-1);
		}
		lua_settop(L, -1);

		//TODO the rest...

	}
	return lua_gettop(L);
}

static int luaAddBuildVersionInfo(lua_State* L) {
	GameEditorTask* cur = sRuntime.current_editor_task;
	if (cur) {
		const char* versionInf = lua_tolstring(L, 1, 0);
		cur->mVersionStrs.push_back(versionInf);
	}
	return lua_gettop(L);
}

std::string tex(u64 hash,std::vector<std::string>& texture_names, bool with_ext) {
	for (auto& it : texture_names)
		if (CRC64_CaseInsensitive(0, it.c_str()) == hash)
			return it.substr(0, it.find_last_of('.'));
	return "not_found";
}

//11 is not in, i think its d3d maybe its half float

inline int decompose_type_size(int comtype) {
	if (comtype == 1)
		return 4;
	else if (comtype == 2 || comtype == 3)
		return 1;
	else if (comtype == 4 || comtype == 5)
		return 2;
	else if (comtype == 11)
		return 2;//half float 16bit
	else {
		printf("wtf unknown type again\n");
	}
}

inline float decompose_to_float(void* data, GFXPlatformFormat fmt, int typeoff) {
	if (fmt == eGFXPlatformFormat_F32x2 || fmt == eGFXPlatformFormat_F32x4 || fmt == eGFXPlatformFormat_F32x3) {
		return ((float*)data)[typeoff];
	}
	else if (fmt == eGFXPlatformFormat_SN16 || fmt == eGFXPlatformFormat_SN16x2 || fmt == eGFXPlatformFormat_SN16x4) {
		i16* raw = (i16*)data;
		i16 val = raw[typeoff];
		float asf = (float)val;
		return asf / 32767.f;
	}
	else if (fmt == eGFXPlatformFormat_UN16 || fmt == eGFXPlatformFormat_UN16x2 || fmt == eGFXPlatformFormat_UN16x4) {
		u16* raw = (u16*)data;
		u16 val = raw[typeoff];
		float asf = (float)val;
		return asf / 65536.f;
	}
	else if (fmt == eGFXPlatformFormat_SN8 || fmt == eGFXPlatformFormat_SN8x2 || fmt == eGFXPlatformFormat_SN8x4) {
		i8* raw = (i8*)data;
		i8 val = raw[typeoff];
		float asf = (float)val;
		return asf / 127.f;
	}
	else if (fmt == eGFXPlatformFormat_UN8 || fmt == eGFXPlatformFormat_UN8x2 || fmt == eGFXPlatformFormat_UN8x4) {
		u8* raw = (u8*)data;
		u8 val = raw[typeoff];
		float asf = (float)val;
		return asf / 255.f;
	}
	else {
		printf("ERROR: Not supported to decompose platform format to float, PLEASE CONTACT ME WITH THIS MESH NAME AND GAME\n");
		return 0.f;
	}
}

inline float decompose_new_UV(void* data, GFXPlatformFormat fmt, int typeoff, T3MeshTexCoordTransform transform) {
	float asf = decompose_to_float(data, fmt, typeoff);
	asf *= transform.mScale.array[typeoff];
	asf += transform.mOffset.array[typeoff];
	if (typeoff == 1)
		asf = 1 - asf;
	bool bNeg = asf <= 0.f;
	asf = bNeg ? -asf : asf;
	if (asf >= 1.f && asf <= 1.01f)
		asf = 1.0f;
	if (asf > 1.0f)
		asf = asf - (float)((int)asf);
	if (bNeg)
		asf = 0.f;
	return asf;
}

inline float decompose_normal_int(void* data, int comtype, int typeoff, bool big_endian/*for type 11 half float*/, bool is_uv = false) {
	data = ((char*)data + typeoff * decompose_type_size(comtype));
	//type: 1 = float, 2= i8, 3= u8, 4 = i16 5 = u16
	if (comtype == 1)
		return *((float*)data);
	else if (comtype == 2)
		return ((float)(*(i8*)data)) / 255.f;
	else if(comtype == 3)
		return ((float)(*(u8*)data)) / 255.f;
	else if(comtype == 4) // MOST USED
		return ((float)(*(i16*)data)) / 32767.f;
	else if (comtype == 5)
		return ((float)(*(u16*)data)) / 65536.f;
	else if (comtype == 11) {
		/*fn convertTo32 input16 = (
		inputAsInt = input16
		sign = bit.get inputAsInt 16
		exponent = (bit.shift (bit.and inputAsInt (bit.hexasint "7C00")) -10) as integer - 16
		fraction = bit.and inputAsInt (bit.hexasint "03FF")
		if sign==true then sign = 1 else sign = 0
		exponentF = exponent + 127
		outputAsFloat = bit.or (bit.or (bit.shift fraction 13) (bit.shift exponentF 23)) (bit.shift sign 31)
		return bit.intasfloat outputasfloat
	)
	fn readHalfFloat fstream = (
	return convertTo32(Readshort fstream)
	)*/
		u16 lo = *((u8*)data);
		u16 hi = *((u8*)data + 1);
		u16 raw = big_endian ? ((lo << 8) | hi) : ((hi << 8) | lo);
		u32 sign = (raw & 0x8000) ? 1 : 0;
		int exp = ((raw >> 10) & 0x1F) - 16;//-16 because 0-31 is two way: bottom are negative exp sign
		u32 mantissa = raw >> 6;
		exp += 127;
		u32 raw_float = (sign << 31) | ((u32)exp << 23) | (mantissa);
		float asf = *((float*)&raw_float);
		if (std::fabsf(asf) < 0.00005f)
			asf = 0;//sometimes this fucks up
		if (is_uv) {
			asf *= 2;//some reason its halfed??
			if (asf >= 1.f && asf <= 1.01f)
				asf = 1.0f;
			if (typeoff == 1)//the V in UV
				asf = 1 - asf;
		}
		return asf;
	}
	else {
		printf("WARNING: UNKNOWN VERTEX COMPONENT TYPE FORMAT!! RETURNING 0 !! \n");
		return 0.f;
	}
}

inline float decompose_normal(void* data, int comtype, int typeoff, bool big_endian/*for type 11 half float*/, bool is_uv = false) {
	float asf = decompose_normal_int(data, comtype, typeoff, big_endian, is_uv);
	if (asf < 0.0f)
		asf = -asf;
	if (asf > 1.0f)
		asf = asf - (float)((int)asf);//WTF
	return asf;
}

GFXPlatformAttributeParams* get_params(T3GFXVertexState& state, GFXPlatformVertexAttribute attrib) {
	for (int i = 0; i < state.mAttributeCount; i++) {
		if (state.mAttributes[i].mAttribute == attrib)
			return state.mAttributes + i;
	}
	return 0;
}

void write_face(std::ofstream& obj, unsigned short* currentFace, int off) {
	obj << "f "
		<< (currentFace[0] + 1 + off) << "/" << (currentFace[0] + 1 + off) << "/" << (currentFace[0] + 1 + off) << " ";
	obj << (currentFace[1] + 1 + off) << "/" << (currentFace[1] + 1 + off) << "/" << (currentFace[1] + 1 + off) << " ";
	obj << (currentFace[2] + 1 + off) << "/" << (currentFace[2] + 1 + off) << "/" << (currentFace[2] + 1 + off) << "\n\n";
}

class MeshTask;
typedef void (mtl_delta_cb)(MeshTask*, std::string&);

//CONVERT NEW D3DMESH => OBJ (GAMES NEWER AND INCLUDING THE WALKING DEAD: MICHONNE)
bool convert_obj_newer(D3DMesh& mesh, const char* gid, std::string objfile, std::string mtlfile, std::string& err, std::vector<std::string>& texnames, mtl_delta_cb mtlCB, MeshTask* t) {
	std::ofstream obj{ objfile };
	TelltaleToolLib_SetBlowfishKey(gid);
	std::ofstream mtl{ mtlfile };
	obj << "mtllib " << (std::filesystem::path{ mtlfile }.filename().string()) << "\n\n";
	obj << "# Converted to OBJ by The Telltale Inspector\n";
	obj << "# https://github.com/LucasSaragosa\n\n\n";
	mtl << "# Converted to MTL by The Telltale Inspector\n";
	mtl << "# https://github.com/LucasSaragosa\n";

	if (mesh.mMeshData.mLODs.GetSize() == 0) {
		printf("WARNING: Mesh contains no LODs, assuming its empty..\n");
		return true;
	}

	std::vector<void*> full_index_buffer{};
	for (int i = 0; i < mesh.mMeshData.mVertexStates.mSize; i++) {
		u32 size = 0;
		for (int j = 0; j < mesh.mMeshData.mVertexStates[i]->mIndexBufferCount; j++) {
			T3GFXBuffer* buf = mesh.mMeshData.mVertexStates[i]->mpIndexBuffer[j];
			if (buf->mBufferFormat != eGFXPlatformFormat_U16) {
				err = "Index buffer format is not U16";
				return false;
			}
			size += buf->mCount * buf->mStride;
		}
		void* p = malloc(size);
		u32 offset = 0;
		for (int j = 0; j < mesh.mMeshData.mVertexStates[i]->mIndexBufferCount; j++) {
			T3GFXBuffer* buf = mesh.mMeshData.mVertexStates[i]->mpIndexBuffer[j];
			memcpy(((char*)p) + offset, buf->mpCPUBuffer, buf->mCount * buf->mStride);
			offset += buf->mCount * buf->mStride;
		}
		full_index_buffer.push_back(p);
	}

	T3MeshLOD& lod = mesh.mMeshData.mLODs[0];//highest lod will be exported.
	//TODO if michonne look into mBatchesM

	T3GFXVertexState& vertexState = *mesh.mMeshData.mVertexStates[lod.mVertexStateIndex];
	T3GFXBuffer& positionsBuffer = *vertexState.mpVertexBuffer[get_params(vertexState, eGFXPlatformAttribute_Position)->mBufferIndex];//must have positions
	void* faces = full_index_buffer[lod.mVertexStateIndex];

	//WRITE POSITIONS
	for (int i = 0; i < positionsBuffer.mCount; i++) {
		Vector3 vert{};
		if (get_params(vertexState, eGFXPlatformAttribute_Position)->mFormat == eGFXPlatformFormat_UN16x4) {
			u16* raw = (u16*)((char*)positionsBuffer.mpCPUBuffer + (positionsBuffer.mStride * i) + get_params(vertexState, eGFXPlatformAttribute_Position)->mBufferOffset);
			Vector3 posScale = mesh.mMeshData.mPositionScale;
			Vector3 posOffset = mesh.mMeshData.mPositionOffset;//position W offset is for the last one but we dont need it
			vert.x = ((((float)raw[0]) / 65536.f) * posScale.x) + posOffset.x;
			vert.y = ((((float)raw[1]) / 65536.f) * posScale.y) + posOffset.y;
			vert.z = ((((float)raw[2]) / 65536.f) * posScale.z) + posOffset.z;
		}//might another fucking weird one idk ill do later if i see it
		else {
			if (get_params(vertexState, eGFXPlatformAttribute_Position)->mFormat != eGFXPlatformFormat_F32x3) {
				err = "Positions format is not F32x3!";
				return false;
			}
			int vertind = i;
			float* currentVert = (float*)((char*)positionsBuffer.mpCPUBuffer + (positionsBuffer.mStride * vertind) + get_params(vertexState, eGFXPlatformAttribute_Position)->mBufferOffset);
			vert = { currentVert[0], currentVert[1], currentVert[2] };
			if (vert.x > 0.f && vert.x < 0.00001f)
				vert.x = 0.f;
			if (vert.y > 0.f && vert.y < 0.00001f)
				vert.y = 0.f;
			if (vert.z > 0.f && vert.z < 0.00001f)
				vert.z = 0.f;
		}
		obj << "v " << vert.x << " " << vert.y << " " << vert.z << "\n";
	}

	//WRITE UVS
	if (get_params(vertexState, eGFXPlatformAttribute_TexCoord)->mFormat != eGFXPlatformFormat_None) {
		T3GFXBuffer& uvBuffer = *vertexState.mpVertexBuffer[get_params(vertexState, eGFXPlatformAttribute_TexCoord)->mBufferIndex];
		GFXPlatformFormat fmt = get_params(vertexState, eGFXPlatformAttribute_TexCoord)->mFormat;
		T3MeshTexCoordTransform uvTransform = mesh.mMeshData.mTexCoordTransform[0];//first UVs
		for (int i = 0; i < uvBuffer.mCount; i++) {
			void* currentVert = (float*)((char*)uvBuffer.mpCPUBuffer + (uvBuffer.mStride * i) + get_params(vertexState, eGFXPlatformAttribute_TexCoord)->mBufferOffset);
			obj << "vt " << decompose_new_UV(currentVert, fmt, 0, uvTransform) << " " << decompose_new_UV(currentVert, fmt, 1, uvTransform) << "\n";
		}
	}

	//WRITE NORMALS
	if (get_params(vertexState, eGFXPlatformAttribute_Normal)->mFormat != eGFXPlatformFormat_None) {
		T3GFXBuffer& nmBuffer = *vertexState.mpVertexBuffer[get_params(vertexState, eGFXPlatformAttribute_Normal)->mBufferIndex];
		GFXPlatformFormat fmt = get_params(vertexState, eGFXPlatformAttribute_Normal)->mFormat;
		for (int i = 0; i < nmBuffer.mCount; i++) {
			void* currentVert = (float*)((char*)nmBuffer.mpCPUBuffer + (nmBuffer.mStride * i) + get_params(vertexState, eGFXPlatformAttribute_Normal)->mBufferOffset);
			obj << "vn " << decompose_to_float(currentVert, fmt, 0) << " " << decompose_to_float(currentVert, fmt, 1) << " " << decompose_to_float(currentVert, fmt, 2) << "\n";
		}
	}

	bool bexit = false;
	for (int j = 0; j < 1; j++) {//only do one batch array? no idea what second one is
		for (int k = 0; k < lod.mBatches[j].GetSize(); k++) {
			bexit = false;
			T3MeshBatch& batch = lod.mBatches[j][k];
			T3MeshMaterial& material = mesh.mMeshData.mMaterials[batch.mMaterialIndex];
			PropertySet* pMaterialProps = nullptr;
			for (int i = 0; i < mesh.mInternalResources.mSize; i++) {
				if (mesh.mInternalResources[i].mHandleObjectInfo.mObjectName == material.mhMaterial.mHandleObjectInfo.mObjectName) {
					pMaterialProps = (PropertySet*)mesh.mInternalResources[i].GetHandleObjectPointer();
					if (pMaterialProps == nullptr) {
						printf("ERROR: Material internal resource properties could not be found\n");
						err = "Material internal resource properties could not be found";
						return false;
					}
					if (mesh.mInternalResources[i].GetTypeDesc() != GetMetaClassDescription<PropertySet>()) {
						printf("ERROR: Ignoring material inside mesh, because its resource is not a property set!!\n");
						bexit = true;
						break;
					}
					break;
				}
			}
			if (bexit)
				continue;
			Handle<T3Texture>* pTexHandle = nullptr;
			if (pMaterialProps == nullptr) {
				printf("WARNING: Mesh contains no internal resources, skipping batch...\n");
				continue;
			}
			if ((pTexHandle = (Handle<T3Texture>*) pMaterialProps->GetProperty("Material - Diffuse Texture")) == nullptr) {
				printf("WARNING: Skipping mesh batch for material inside mesh because it does not have an associated diffuse texture!\n");
				continue;
			}
			std::string texFile = "";//with D3DTX extension
			for(auto& texName: texnames)
				if (Symbol{ texName.c_str() }.GetCRC() == pTexHandle->mHandleObjectInfo.mObjectName.GetCRC()) {
					texFile = texName;
					break;
				}
			if (texFile.length() == 0) {
				printf("WARNING: Skipping mesh batch for material inside mesh because its texture file could not be located from the material!\n");
				continue;
			}

			texFile = texFile.substr(0, texFile.find_last_of('.'));

			/*WRITE MATERIAL INFO*/
			mtl << "\n\nnewmtl " << texFile << "\n";
			//anything else?

			obj << "\n\no " << texFile << "\n";
			obj << "g " << texFile << "\n";
			obj << "usemtl " << texFile << "\ns 1\n\n";

			mtlCB(t, texFile);
			mtl << "\tmap_Kd " << texFile << "\n";

			//WRITE FACES
			for (int i = 0; i < batch.mNumPrimitives; i++) {
				int faceIndex = (batch.mStartIndex / 3) + i;
				unsigned short* currentFace = (unsigned short*)((char*)faces + sizeof(unsigned short) * faceIndex * 3);

				write_face(obj, currentFace,batch.mBaseIndex);
			}
		}
	}
	
	for (auto& ptr : full_index_buffer)
		free(ptr);
	return true;
}

bool convert_obj_legacy(LegacyD3DMesh& mesh, const char* gid, std::string objfile, std::string mtlfile,std::string&err, std::vector<std::string>& texnames, mtl_delta_cb mtlCB, MeshTask* t) {
	std::ofstream obj{ objfile };
	std::ofstream mtl{ mtlfile };
	obj << "mtllib " << (std::filesystem::path{ mtlfile }.filename().string()) << "\n\n";
	obj << "# Converted to OBJ by The Telltale Inspector\n";
	obj << "# https://github.com/LucasSaragosa\n\n\n";
	mtl << "# Converted to MTL by The Telltale Inspector\n";
	mtl << "# https://github.com/LucasSaragosa\n";

	if (mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mType != 1) {
		printf("Cannot convert mesh: unknown vert pos type!\n");
		return false;
	}
	if (mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mCount != 3) {
		printf("Cannot convert mesh: unknown vert pos type (not 3 components)!\n");
		return false;
	}

	if (mesh.mIndexBuffer.mFormat != 0x65 || mesh.mIndexBuffer.mFlags.mFlags != 0) {
		printf("Cannot convert mesh: untested index buffer flags / format\n");
		return false;
	}

	int posOffset = mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mOffset;

	/*Write actual mesh data*/

	for (int j = 0; j < 2; j++) {

		//verts
		for (int vert = 0; vert < mesh.mVertexBuffer[j].mNumVerts; vert++) {
			float* currentVert = (float*)((char*)mesh.mVertexBuffer[j].pData + (mesh.mVertexBuffer[j].mVertSize * vert) + posOffset);
			if (currentVert[0] > 0.f && currentVert[0] < 0.00001f)
				currentVert[0] = 0.f;
			if (currentVert[1] > 0.f && currentVert[1] < 0.00001f)
				currentVert[1] = 0.f;
			if (currentVert[2] > 0.f && currentVert[2] < 0.00001f)
				currentVert[2] = 0.f;
			obj << "v " << currentVert[0] << " " << currentVert[1] << " " << currentVert[2] << "\n";//warn: dont use decompose normal, write own function for half since it does some magic
		}

		obj << "\n\n";

		//uvs, todo are they in second vert buffer???
		if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mCount > 0) {
			if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mCount != 2) {
				printf("unknown vert UV type (not 2 components)!\n");
			}
			int type = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mType;
			int off = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mOffset;
			for (int vert = 0; vert < mesh.mVertexBuffer[j].mNumVerts; vert++) {
				void* currentVert = (void*)((char*)mesh.mVertexBuffer[j].pData + (mesh.mVertexBuffer[j].mVertSize * vert) + off);
				obj << "vt " << decompose_normal(currentVert, type, 0, 0, true) << " " << decompose_normal(currentVert, type, 1, 0, true) << "\n";
			}
		}

		obj << "\n\n";

		//normals
		if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mCount > 0) {
			if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mCount != 3) {
				printf("unknown vert normal type (not 3 components)!\n");
			}
			int type = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mType;
			int off = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mOffset;
			for (int vert = 0; vert < mesh.mVertexBuffer[j].mNumVerts; vert++) {
				void* currentVert = (void*)((char*)mesh.mVertexBuffer[j].pData + (mesh.mVertexBuffer[j].mVertSize * vert) + off);
				obj << "vn " << decompose_normal(currentVert, type, 0, 0) << " " << decompose_normal(currentVert, type, 1, 0) << " " << decompose_normal(currentVert, type, 2, 0) << "\n";
			}
		}

		// ...

	}

	int faceStart = 0;
	for (int i = 0; i < mesh.mTriangleSets.GetSize(); i++) {
		LegacyD3DMesh::TriangleSet& set = mesh.mTriangleSets[i];
		if (set.mTxIndex.mData[0] == -1) {
			printf("Mesh triangle set has no diffuse texture !!");//no diffuse tex? wtf
			return false;
		}
		LegacyD3DMesh::Texture& diffuse = mesh.mTextures.mData[LegacyD3DMesh::TEXTURE_INDEX_DIFFUSE][set.mTxIndex.mData[LegacyD3DMesh::TEXTURE_INDEX_DIFFUSE]];

		std::string textureF = tex(diffuse.mName.mHandleObjectInfo.mObjectName.GetCRC(), texnames, true);

		/*Write the texture material information*/
		mtl << "\n\nnewmtl " << textureF << "\n";
		//anything else in the mtl here?
		mtl << "\tKs " << set.mSpecularColor.r << " " << set.mSpecularColor.g << " " << set.mSpecularColor.b << "\n";
		mtl << "\tNs " << set.mSpecularPower << "\n";
		mtl << "\tKd " << set.mDiffuseColor.r << " " << set.mDiffuseColor.g << " " << set.mDiffuseColor.b << "\n";
		mtl << "\tKa " << set.mAmbientColor.r << " " << set.mAmbientColor.g << " " << set.mAmbientColor.b << "\n";


		obj << "\n\no " << textureF << "\n";
		obj << "g " << textureF << "\n";
		obj << "usemtl " << textureF << "\ns 1\n\n";

		//faces
		for (int i = 0; i < set.mNumTotalIndices / 3; i++) {
			int faceIndex = faceStart / 3 + i;
			unsigned short* currentFace = (unsigned short*)((char*)mesh.mIndexBuffer.pData + sizeof(unsigned short) * faceIndex * 3);
			obj << "f " << (currentFace[0] + 1) << "/" << (currentFace[0] + 1) << "/" << (currentFace[0] + 1) << " ";
			obj << (currentFace[1] + 1) << "/" << (currentFace[1] + 1) << "/" << (currentFace[1] + 1) << " ";
			obj << (currentFace[2] + 1) << "/" << (currentFace[2] + 1) << "/" << (currentFace[2] + 1) << "\n\n";
		}

		mtlCB(t, textureF);
		mtl << "\tmap_Kd " << textureF << "\n";

		faceStart += set.mNumTotalIndices;
	}
	
	return true;
}

bool convert_obj_legacy_OLD(LegacyD3DMesh& mesh, const char* gid, std::string objfile, std::string mtlfile, std::string& err, std::vector<std::string>& texnames, mtl_delta_cb mtlCB, MeshTask* t) {
	std::ofstream obj{ objfile };
	std::ofstream mtl{ mtlfile };
	obj << "mtllib " << (std::filesystem::path{ mtlfile }.filename().string()) << "\n\n";
	obj << "# Converted to OBJ by The Telltale Inspector\n";
	obj << "# https://github.com/LucasSaragosa\n\n\n";
	mtl << "# Converted to MTL by The Telltale Inspector\n";
	mtl << "# https://github.com/LucasSaragosa\n";

	if (mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mType != 1) {
		printf("Cannot convert mesh: unknown vert pos type!\n");
		return false;
	}
	if (mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mCount != 3) {
		printf("Cannot convert mesh: unknown vert pos type (not 3 components)!\n");
		return false;
	}

	if (mesh.mIndexBuffer.mFormat != 0x65 || mesh.mIndexBuffer.mFlags.mFlags != 0) {
		printf("Cannot convert mesh: untested index buffer flags / format\n");
		return false;
	}

	int faceStart = 0;
	for (int i = 0; i < mesh.mTriangleSets.GetSize(); i++) {
		LegacyD3DMesh::TriangleSet& set = mesh.mTriangleSets[i];
		if (set.mTxIndex.mData[0] == -1) {
			printf("Mesh triangle set has no diffuse texture !!");//no diffuse tex? wtf
			return false;
		}
		LegacyD3DMesh::Texture& diffuse = mesh.mTextures.mData[LegacyD3DMesh::TEXTURE_INDEX_DIFFUSE][set.mTxIndex.mData[LegacyD3DMesh::TEXTURE_INDEX_DIFFUSE]];

		std::string textureF = tex(diffuse.mName.mHandleObjectInfo.mObjectName.GetCRC(), texnames, true);

		/*Write the texture material information*/
		mtl << "\n\nnewmtl " << textureF << "\n";
		//anything else in the mtl here?
		mtl << "\tKs " << set.mSpecularColor.r << " " << set.mSpecularColor.g << " " << set.mSpecularColor.b << "\n";
		mtl << "\tNs " << set.mSpecularPower << "\n";
		mtl << "\tKd " << set.mDiffuseColor.r << " " << set.mDiffuseColor.g << " " << set.mDiffuseColor.b << "\n";
		mtl << "\tKa " << set.mAmbientColor.r << " " << set.mAmbientColor.g << " " << set.mAmbientColor.b << "\n";

		int posOffset = mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mOffset;

		/*Write actual mesh data*/

		for (int j = 0; j < 2; j++) {

			//verts
			for (int vert = set.mMinVertIndex; vert <= set.mMaxVertIndex; vert++) {
				float* currentVert = (float*)((char*)mesh.mVertexBuffer[j].pData + (mesh.mVertexBuffer[j].mVertSize * vert) + posOffset);
				if (currentVert[0] > 0.f && currentVert[0] < 0.00001f)
					currentVert[0] = 0.f;
				if (currentVert[1] > 0.f && currentVert[1] < 0.00001f)
					currentVert[1] = 0.f;
				if (currentVert[2] > 0.f && currentVert[2] < 0.00001f)
					currentVert[2] = 0.f;
				obj << "v " << currentVert[0] << " " << currentVert[1] << " " << currentVert[2] << "\n";//warn: dont use decompose normal, write own function for half since it does some magic
			}

			obj << "\n\n";

			//uvs, todo are they in second vert buffer???
			if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mCount > 0) {
				if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mCount != 2) {
					printf("unknown vert UV type (not 2 components)!\n");
				}
				int type = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mType;
				int off = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mOffset;
				for (int vert = set.mMinVertIndex; vert <= set.mMaxVertIndex; vert++) {
					void* currentVert = (void*)((char*)mesh.mVertexBuffer[j].pData + (mesh.mVertexBuffer[j].mVertSize * vert) + off);
					obj << "vt " << decompose_normal(currentVert, type, 0, 0, true) << " " << decompose_normal(currentVert, type, 1, 0, true) << "\n";
				}
			}

			obj << "\n\n";

			//normals
			if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mCount > 0) {
				if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mCount != 3) {
					printf("unknown vert normal type (not 3 components)!\n");
				}
				int type = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mType;
				int off = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mOffset;
				for (int vert = set.mMinVertIndex; vert <= set.mMaxVertIndex; vert++) {
					void* currentVert = (void*)((char*)mesh.mVertexBuffer[j].pData + (mesh.mVertexBuffer[j].mVertSize * vert) + off);
					obj << "vn " << decompose_normal(currentVert, type, 0, 0) << " " << decompose_normal(currentVert, type, 1, 0) << " " << decompose_normal(currentVert, type, 2, 0) << "\n";
				}
			}

			// ...

		}

		obj << "\n\no " << textureF << "\n";
		obj << "g " << textureF << "\n";
		obj << "usemtl " << textureF << "\ns 1\n\n";

		//faces
		for (int i = 0; i < set.mNumTotalIndices / 3; i++) {
			int faceIndex = faceStart / 3 + i;
			unsigned short* currentFace = (unsigned short*)((char*)mesh.mIndexBuffer.pData + sizeof(unsigned short) * faceIndex * 3);
			obj << "f " << (currentFace[0] + 1) << "/" << (currentFace[0] + 1) << "/" << (currentFace[0] + 1) << " ";
			obj << (currentFace[1] + 1) << "/" << (currentFace[1] + 1) << "/" << (currentFace[1] + 1) << " ";
			obj << (currentFace[2] + 1) << "/" << (currentFace[2] + 1) << "/" << (currentFace[2] + 1) << "\n\n";
		}

		mtlCB(t, textureF);
		mtl << "\tmap_Kd " << textureF << "\n";

		faceStart += set.mNumTotalIndices;
	}

	return true;
}

template <typename T>
std::basic_string<T> lowercase(const std::basic_string<T>& s)
{
	std::basic_string<T> s2 = s;
	std::transform(s2.begin(), s2.end(), s2.begin(),
		[](const T v) { return static_cast<T>(std::tolower(v)); });
	return s2;
}


static std::string* SymMap_ResolveHash(char* resolve_buf, std::unordered_map<u64, std::string>& db, std::vector<int>& dbFilenames, u64 hash, resolve_mode md) {
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
						if (ends_with(pg->pageName, tmp)) {
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

inline u32 rotate_left(u32 i, u32 bits) {
	return (i << bits) | (i >> (32 - bits));
}

inline u32 fix_pixel(u32 argb) {
	u32 red = (argb >> 16) & 0xff;
	u32 blue = (argb >> 0) & 0xff;
	u32 green = (argb >> 8) & 0xff;
	u32 alpha = (argb >> 24) & 0xff;
	return (alpha << 24) | (blue << 16) | (green << 8) | (red << 0);
}

bool convert_to_png(T3Texture& texture, std::string outpng, std::string& err) {
	DataStream* pTexels = texture.GetTxData(0, 0);
	if (!pTexels) {
		err = "Could not get texture data from texture!!";
		return false;
	}
	pTexels->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
	char* texbuf = (char*)malloc(pTexels->GetSize());
	if (!pTexels->Serialize(texbuf, pTexels->GetSize())) {
		free(texbuf);
		err = "Could not read texture data";
		return false;
	}

	if (texture.mSurfaceFormat == eSurface_RGBA8)
		;//all ok
	else if (texture.mSurfaceFormat == eSurface_ARGB8) {
		//flip alpha byte
		int num = (texture.mWidth * texture.mHeight);
		u32* raw = (u32*)texbuf;
		for (int i = 0; i < num; i++)
			raw[i] = fix_pixel(raw[i]);
	}
	else if (texture.mSurfaceFormat == eSurface_DXT1) {
		char* dest = (char*)malloc(texture.mWidth * texture.mHeight * 4);
		squish::DecompressImage((squish::u8*)dest, texture.mWidth, texture.mHeight, texbuf, squish::kDxt1);
		free(texbuf);
		texbuf = dest;
	}
	else if (texture.mSurfaceFormat == eSurface_DXT3) {
		char* dest = (char*)malloc(texture.mWidth * texture.mHeight * 4);
		squish::DecompressImage((squish::u8*)dest, texture.mWidth, texture.mHeight, texbuf, squish::kDxt3);
		free(texbuf);
		texbuf = dest;
	}
	else if (texture.mSurfaceFormat == eSurface_DXT5) {
		char* dest = (char*)malloc(texture.mWidth * texture.mHeight * 4);
		squish::DecompressImage((squish::u8*)dest, texture.mWidth, texture.mHeight, texbuf, squish::kDxt5);
		free(texbuf);
		texbuf = dest;
	}
	else {
		free(texbuf);
		err = std::string{ "Cannot convert the current surface format as of yet! => " } + std::to_string((int)texture.mSurfaceFormat);
		return false;
	}

	if (stbi_write_png(outpng.c_str(), texture.mWidth, texture.mHeight, 4, texbuf, 4 * texture.mWidth) == 0) {
		err = "Could not write PNG file to destination file";
		free(texbuf);
		return false;
	}
	free(texbuf);
	return true;
}

class TextureTask : public InspectorTask {

	T3Texture* mpTexture;
	MetaStream mStream;
	const char* id;
	bool ok;
	bool loaded;
	std::string err;
	MetaClassDescription* pTextureMCD;
	const char* texfmt = 0;

	virtual void _render() override {
		if (id == 0) {
			ImGui::Text("Select game texture belongs to");
			id = select_gameid_dropdown(id);
			return;
		}
		if (loaded) {
			TelltaleToolLib_SetBlowfishKey(id);
			ImGui::InputText("Texture Name", &mpTexture->mName);
			ImGui::InputText("Texture Import Name", &mpTexture->mImportName);
			MetaMemberDescription* formatDesc = pTextureMCD->GetMemberDescription("mSurfaceFormat");
			texfmt = GetMetaEnumName(formatDesc, mpTexture->mSurfaceFormat);
			ImGui::Text("Note: Changing this does not change actual internal pixel format !!");
			if (ImGui::BeginCombo("Texture Format", texfmt)) {
				MetaEnumDescription* pEnum = formatDesc->mpEnumDescriptions;
				while (pEnum) {
					if (ImGui::Selectable(pEnum->mpEnumName)) {
						ImGui::SetItemDefaultFocus();
						mpTexture->mSurfaceFormat = (T3SurfaceFormat)pEnum->mEnumIntValue;
					}
					pEnum = pEnum->mpNext;
				}
				ImGui::EndCombo();
			}
			
		}
		else {
			if (!ok)
				ImGui::Text((std::string{"Error opening last file: "} + err).c_str());
			nfdchar_t* outp{ 0 };
			if (ImGui::Button("Open Texture File") && NFD_OKAY == NFD_OpenDialog("d3dtx", 0, &outp)) {
				std::string fp = outp;
				free(outp);
				TelltaleToolLib_SetBlowfishKey(id);
				if (!mStream.Attach(_OpenDataStreamFromDisc(fp.c_str(), READ), MetaStreamMode::eMetaStream_Read, {})) {
					err = "Error opening meta stream file, are you sure this is a telltale file?";
					ok = false;
					return;
				}
				if (mpTexture)
					delete mpTexture;
				mpTexture = new T3Texture;
				if (PerformMetaSerializeAsync(&mStream, mpTexture) != eMetaOp_Succeed) {
					err = "Error opening the file as a texture, are you sure the game is correct and its a texture?";
					ok = false;
					return;
				}
				ok = loaded = true;
				err = "";
			}
		}
		nfdchar_t* outp{ 0 };
		if (ImGui::Button("Convert Whole Folder") && NFD_OKAY == NFD_PickFolder(0, &outp, L"Select folder containg D3DTX files")) {
			std::string texFolder = outp;
			free(outp);
			std::string pngFolder = texFolder + "/Converted/";
			std::vector<std::string> failed_files{};
			TelltaleToolLib_SetBlowfishKey(id);
			std::filesystem::create_directories(pngFolder);
			for (auto& file : std::filesystem::directory_iterator{ std::filesystem::path{texFolder} }) {
				if (file.is_regular_file() && ends_with(file.path().filename().string(), ".d3dtx")) {
					if (mpTexture)
						delete mpTexture;
					mpTexture = new T3Texture;
					if (!mStream.Attach(_OpenDataStreamFromDisc(file.path().string().c_str(), READ), MetaStreamMode::eMetaStream_Read, {})) {
						failed_files.push_back(file.path().filename().string() + " => Could not open meta stream");
						continue;
					}
					if (PerformMetaSerializeAsync(&mStream, mpTexture) != eMetaOp_Succeed) {
						failed_files.push_back(file.path().filename().string() + " => Could not open meta stream as texture, incorrect game?");
						continue;
					}
					std::string pngf = pngFolder + file.path().filename().string();
					pngf = pngf.substr(0, pngf.find_last_of('.')) + ".png";
					if (!convert_to_png(*mpTexture, pngf, err)) {
						failed_files.push_back(file.path().filename().string() + " => Error converting to PNG => " + err);
						continue;
					}
					printf("convert success: %s\n", file.path().filename().string().c_str());
				}
			}
			if (failed_files.size() > 0)
				printf("Failed to convert the following textures:\n");
			for (auto& failed : failed_files) {
				printf("-%s\n",failed.c_str());
			}
			MessageBoxA(0, "All files were converted! Check console for any ones that had problems.", "Finished!", MB_ICONINFORMATION);
		}
		outp = 0;
		if (loaded && ok && ImGui::Button("Convert to PNG") && NFD_OKAY == NFD_SaveDialog("png", 0, &outp, L"Select output file")) {
			std::string outpng = outp;
			if (!ends_with(outpng, ".png"))
				outpng += ".png";
			if (convert_to_png(*mpTexture, outpng, err)) {
				MessageBoxA(0, "The texture was successfully converted to PNG!", "Success!", MB_ICONINFORMATION);
			}
			else {
				MessageBoxA(0, err.c_str(), "Error converting texture to PNG", MB_ICONERROR);
			}
		}
	}

public:

	deleteme(TextureTask)

	~TextureTask(){
		if (mpTexture)
			delete mpTexture;
		mpTexture = 0;
	}

	taskctor(TextureTask) {
		mpTexture = 0;
		err = "";
		ok = loaded = false;
		ok = true;
		id = 0;
		pTextureMCD = GetMetaClassDescription<T3Texture>();
	}

};

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

bool IsType(MetaClassDescription* clazz, const char* tn) {
	return Symbol{ tn }.GetCRC() == clazz->mHash;
}

class PropTask : public InspectorTask {
	struct {
		PropertySet __mNormal, * __mpExternal;
	} _int;
public:
	void set_prop(std::string&& name, bool* b_access_gate, const char* gameid, save_changes_fndecl* change_fn, PropertySet* ext, void* user) {
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

	std::vector<String> possible;
	MetaClassDescription* mh_handleRefType = nullptr;
	std::string mh_inputField;
	const char* mh_comboitem;

	int mh_uid = 0;
	bool ignore_ctrln = false;
	int streamVersion;
	Symbol mh_item;

	std::string modify_symbol_field;
	std::string mod_sym_final;
	bool mod_sym_nf = false;
	void* mod_sym_inst = nullptr;

	//map new item data
	void* tmpkey = 0;

	std::string nparent{};

	char resolve_buf[1024];

	bool modify_handle(MetaClassDescription* desc, Symbol& sym) {
		ImGui::OpenPopup(desc->mpTypeInfoName);
		if (ImGui::BeginPopupModal(desc->mpTypeInfoName)) {
			if (possible.size() == 0) {//fihnd all Files_
				std::string str = desc->mpTypeInfoName;
				unsigned first = str.find("<");
				unsigned last = str.find(">");
				std::string mcd = str.substr(first + 1, last - first - 1);
				mh_handleRefType = TelltaleToolLib_FindMetaClassDescription(mcd.c_str(), true);
				if (mh_handleRefType == nullptr) {
					MessageBoxA(0, "Internal error please contact me: MH_HANDLEREFTYPE", "!!", MB_ICONERROR);
					ImGui::EndPopup();
					return true;
				}
				resolve_buf[0] = 0;
				HashDatabase::Page* page = NULL;
				String pagen = "Files_";
				pagen += mh_handleRefType->mpExt;
				pagen += '_';
				String gameid = sBlowfishKeys[sSetKeyIndex].game_id;
				std::transform(gameid.begin(), gameid.end(), gameid.begin(), ::toupper);
				pagen += gameid;
				page = TelltaleToolLib_GetGlobalHashDatabase()->FindPage(pagen.c_str());
				TelltaleToolLib_GetGlobalHashDatabase()->DumpPage(page, possible);
				if (sym.GetCRC() != 0) {
					TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(page, sym.GetCRC(), resolve_buf);
					mh_inputField = resolve_buf;
					if (mh_inputField.size() == 0) {
						mh_inputField = "Symbol<";
						std::stringstream stream;
						stream << std::hex << sym.GetCRC();
						std::string result(stream.str());
						mh_inputField += stream.str();
						mh_inputField += ">";
					}
				}
				else
					mh_inputField = "";
			}
			ImGui::Text("Type in the filename below. Use the combo box to fill in the rest of the valid filenames for the selected game.");
			ImGui::Text("File:");
			ImGui::SameLine();
			ImGui::InputText("##mhfield", &mh_inputField);
			bool em = mh_inputField.size() == 0;
			if (possible.size() > 0) {
				ImGui::Text("Autofill Filename:");
				ImGui::SameLine();
				if (ImGui::BeginCombo("##mbsel", mh_comboitem)) {
					for (auto it = possible.begin(); it != possible.end(); it++) {
						if (em || it->_Starts_with(mh_inputField)) {
							if (ImGui::Selectable(it->c_str())) {
								mh_inputField = *it;
							}
						}
					}
					ImGui::EndCombo();
				}
			}
			if (ImGui::Button("Quit")) {
				possible.clear();
				mh_handleRefType = nullptr;
				mh_inputField.clear();
				mh_comboitem = nullptr;
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				mh_item.SetCRC(0);
				mh_uid = 0;
				return true;
			}
			if ((em || ends_with(mh_inputField, mh_handleRefType->mpExt)) && ImGui::Button("Done")) {
				sym.SetCRC(CRC64_CaseInsensitive(0, mh_inputField.c_str()));
				possible.clear();
				mh_handleRefType = nullptr;
				mh_inputField.clear();
				mh_comboitem = nullptr;
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				mh_item.SetCRC(0);
				mh_uid = 0;
				return true;
			}
			ImGui::EndPopup();
		}
		else return true;
		return false;
	}

	//stores in prop key user database of hashes
	void modify_symbol(Symbol& sym, const char* key, void* inst) {
		ImGui::OpenPopup(key);
		if (ImGui::BeginPopupModal(key)) {
			if (mod_sym_inst != inst) {
				//setup
				mod_sym_nf = false;
				mod_sym_inst = inst;
				modify_symbol_field = "";
				mod_sym_final = "";
				std::string* pResolved = SymMap_ResolveHash(resolve_buf, sym_map, fn_map, sym.GetCRC(), check_all);
				if (pResolved) {
					mod_sym_final = modify_symbol_field = *pResolved;
				}
				else if (sym.GetCRC() != 0) {
					mod_sym_nf = true;
					std::string mh_inputField = "Symbol<";
					std::stringstream stream;
					stream << std::hex << sym.GetCRC();
					std::string result(stream.str());
					mh_inputField += stream.str();
					mh_inputField += ">";
					mod_sym_final = std::move(mh_inputField);
				}
			}

			ImGui::Text("Symbol Value:");
			ImGui::SameLine();
			if (sym.GetCRC() == 0) {
				ImGui::Text("No Value");
			}
			else if (mod_sym_nf) {
				ImGui::Text(mod_sym_final.c_str());
			}
			else {
				ImGui::InputText("##modsymf", &mod_sym_final, ImGuiInputTextFlags_ReadOnly);
			}
			ImGui::Text("To edit, type in the new symbol value then click set to modify the actual value above");
			ImGui::InputText("##msfield", &modify_symbol_field);
			ImGui::SameLine();
			if (ImGui::Button("Set")) {
				mod_sym_final = modify_symbol_field;
				mod_sym_nf = modify_symbol_field.length() == 0;
				sym.SetCRC(CRC64_CaseInsensitive(0, mod_sym_final.c_str()));
				sRuntime.symbolMaps[prop_key].insert(mod_sym_final);
			}
			if (ImGui::Button("Done")) {
				ImGui::CloseCurrentPopup();
				mod_sym_inst = 0;
				mod_sym_nf = false;
				mod_sym_final = modify_symbol_field = "";
			}

			ImGui::EndPopup();
		}
	}

	bool PropTree_Math(u64 hash, void* pInst) {
		ImGui::PushID(pInst);
		if (hash == vec4_str || hash == quat_str) {
			//ImGui::SameLine();
			Vector4* vec = (Vector4*)pInst;//same as quat
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X"))
				vec->x = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &vec->x, 0.1f, 0.0f, 0.0f, "%.2f");
			//ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y"))
				vec->y = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &vec->y, 0.1f, 0.0f, 0.0f, "%.2f");
			//ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z"))
				vec->z = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &vec->z, 0.1f, 0.0f, 0.0f, "%.2f");
			//ImGui::SameLine();

			if (ImGui::Button("W"))
				vec->w = 0.0f;

			ImGui::SameLine();
			ImGui::DragFloat("##W", &vec->w, 0.1f, 0.0f, 0.0f, "%.2f");

		}
		else if (hash == vec3_str) {
			//ImGui::SameLine();
			Vector3* vec = (Vector3*)pInst;//same as quat
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X"))
				vec->x = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &vec->x, 0.1f, 0.0f, 0.0f, "%.2f");
			//ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y"))
				vec->y = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &vec->y, 0.1f, 0.0f, 0.0f, "%.2f");
			//ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z"))
				vec->z = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &vec->z, 0.1f, 0.0f, 0.0f, "%.2f");

		}
		else if (hash == vec2_str) {
			//ImGui::SameLine();
			Vector2* vec = (Vector2*)pInst;//same as quat
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X"))
				vec->x = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &vec->x, 0.1f, 0.0f, 0.0f, "%.2f");
			//ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y"))
				vec->y = 0.0f;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &vec->y, 0.1f, 0.0f, 0.0f, "%.2f");
		}
		else if (hash == tran_str) {
			Transform* transform = (Transform*)pInst;
			ImGui::Text("Rotation:");
			PropTree_Math(quat_str, &transform->mRot);
			ImGui::Text("Translation:");
			PropTree_Math(vec3_str, &transform->mTrans);
		}
		else if (hash == col_str) {
			Color* transform = (Color*)pInst;
			ImGui::Text("RGBA:");
			PropTree_Math(vec4_str, transform);
		}
		else if (hash == sphere_str) {
			Sphere* s = (Sphere*)pInst;
			ImGui::Text("Center:");
			PropTree_Math(vec3_str, &s->mCenter);
			ImGui::Text("Radius:");
			ImGui::SameLine();
			ImGui::DragFloat("##R", &s->mRadius, 0.1f, 0.0f, 0.f);
		}
		else {
			ImGui::PopID();
			return false;
		}
		ImGui::PopID();
		return true;
	}

	//returns 1 if user wants to create new element, 2 to remove
	int ListTree(const char* prefix, int uid, void* data, void* (*getnext)(void* dat, void* prev, int index), int num_elem, MetaClassDescription* pDesc, int* selected)
	{
		*selected = -1;
		// Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
		ImGui::PushID(uid);

		// Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		bool node_open = ImGui::TreeNode(prefix);
		ImGui::TableSetColumnIndex(1);

		u64 to_del = 0;
		std::string knb{};
		char knbr[17]{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

		if (node_open)
		{
			void* pitem = 0;
			for (int i = 0; i < num_elem; i++) {
				std::snprintf(knbr, 16, "[%d]", i);
				knb = knbr;
				pitem = getnext(data,pitem, i);
				bool hov{ false };
				PropTreeItem(pDesc, pitem, &knb, uid, to_del, i, true, hov);
				if (hov)
					*selected = i;
				if(to_del != 0){
					break;
				}
			}
			ImGui::TreePop();
		}
		int ret = 0;
		ImGui::PopID();
		if (to_del != 0) {
			ret = 2;
			*selected = (int)(to_del & 0x7FFF'FFFF);
		}
		else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_N) && *selected != -1) {
			ret = 1;
		}
		return ret;
	}

	void MapTreeKeyInput(MetaClassDescription* k, void* obj, bool reset = false){
		static std::string symbol_in{};
		if(reset){
			if (k->mHash == hash_sym) {
				u64 crc;
				if(((Symbol*)obj)->GetCRC() == 0)
					((Symbol*)obj)->SetCRC(crc=CRC64_CaseInsensitive(0, symbol_in.c_str()));
				symbol_in = "";
			}
		}
		else {
			if (k->mHash == hash_str) {
				ImGui::InputText("##", (std::string*)obj);
			}
			else if (k->mHash == hash_uint32) {
				u8 step = 1, maxstep = 100;
				ImGui::InputScalar("##", ImGuiDataType_U32, obj, &step, &maxstep);
			}
			else if (k->mHash == hash_uint32) {
				u8 step = 1, maxstep = 100;
				ImGui::InputScalar("##", ImGuiDataType_U32, obj, &step, &maxstep);
			}
			else if (k->mHash == hash_long || k->mHash == hash_int32 || k->mHash == hash_int) {
				u8 step = 1, maxstep = 100;
				ImGui::InputScalar("##", ImGuiDataType_S32, obj, &step, &maxstep);
			}
			else if (k->mHash == hash_sym) {
				ImGui::InputText("##", (std::string*)obj);
				ImGui::Text("Or enter raw decimal hash (used if non zero):");
				ImGui::InputScalar("##", ImGuiDataType_U64, obj);
			}
		}
	}

	bool MapTreeKey(MetaClassDescription* k, std::string* dest, void* obj){
		char bf[32];
		if(k->mHash == hash_str){
			if (!dest)
				return true;
			String* pStr = (String*)obj;
			*dest = pStr->c_str();
			return true;
		}else if(k->mHash == hash_uint32){
			if (!dest)
				return true;
			unsigned int* intval = (unsigned int*)obj;
			sprintf(bf, "%" PRIu32, *intval);
			*dest = bf;
			return true;
		}else if(k->mHash == hash_long || k->mHash == hash_int32 || k->mHash == hash_int){
			if (!dest)
				return true;
			int* intval = (int*)obj;
			sprintf(bf, "%d", *intval);
			*dest = bf;
			return true;
		}else if(k->mHash == hash_sym){
			if(dest){
				u64 sym = ((Symbol*)obj)->GetCRC();
				sprintf(bf, "Symbol<%" PRIx64 ">", sym);
				*dest = bf;
			}
			return true;
		}
		return false;
	}

	void TypeTree(const char* prefix, int uid, MetaClassDescription* type, void* value){
		// Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
		ImGui::PushID(uid);

		// Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		bool node_open = ImGui::TreeNode(prefix);
		ImGui::TableSetColumnIndex(1);
		std::string kn{};
		u64 del{};
		if (node_open)
		{
			int i = 0;
			for (MetaMemberDescription* mem = type->mpFirstMember; mem!=nullptr; mem=mem->mpNextMember) {
				kn = mem->mpName;
				bool hov{};
				PropTreeItem(mem->mpMemberDesc, (char*)value + mem->mOffset, &kn, uid, del, i++, false, hov, mem, true);
			}
			ImGui::TreePop();
		}
		int ret = 0;
		ImGui::PopID();
	}

	MemoryEditor bbEditor;

	//returns 1 if user wants to create new element, 2 to remove
	int MapTree(const char* prefix, int uid, int* selected, Map<void*,void*>* pProxyMap)
	{
		*selected = -1;
		// Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
		ImGui::PushID(uid);

		// Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		bool node_open = ImGui::TreeNode(prefix);
		ImGui::TableSetColumnIndex(1);

		u64 to_del = 0;
		std::string knb{};

		if (node_open)
		{
			void* pitem = 0;
			for (int i = 0; i < pProxyMap->GetSize(); i++) {
				MapTreeKey(pProxyMap->GetContainerKeyClassDescription(), &knb, pProxyMap->GetKey(i));
				pitem = pProxyMap->GetVal(i);
				bool hov{ false };
				PropTreeItem(pProxyMap->GetContainerDataClassDescription(), pitem, &knb, uid, to_del, i, true, hov);
				if (hov)
					*selected = i;
				if (to_del != 0) {
					break;
				}
			}
			ImGui::TreePop();
		}
		int ret = 0;
		ImGui::PopID();
		if (to_del != 0) {
			ret = 2;
			*selected = (int)(to_del & 0x7FFF'FFFF);
		}
		else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_N) && *selected != -1) {
			ret = 1;
		}
		return ret;
	}

	static void* DCArray_PropGetter(void* array,void*, int i){
		MetaClassDescription* clazz = ((DCArray<void*>*)(array))->GetContainerDataClassDescription();
		return ((DCArray<void*>*)(array))->GetElement(i);//can use this as its virtual
	}

	static void* SArray_PropGetter(void* array, void*, int i) {
		MetaClassDescription* clazz = ((SArray<void*,1>*)(array))->GetContainerDataDescription();
		return (void*)((char*)((SArray<void*, 1>*)(array))->mData + i * clazz->mClassSize);
	}

	void PropTreeItem(MetaClassDescription* clazz, void* pRawData, std::string* kn, int uid, u64& to_del, int i, bool bAllowDel, bool& hovered, MetaMemberDescription* memCheck = 0, bool disbleMemCheck = false) {
		if (memCheck && !disbleMemCheck && Meta_IsMemberDisabled(memCheck, 6)) {//only version 6 here older wont be used
			return;
		}
		if (true) {
			if (pRawData && clazz) {
				ImGui::PushID(i);

				if (IsType(clazz, "PropertySet")) {
					PropTree((PropertySet*)pRawData, kn->c_str(), (int)pRawData & 0x7FFFFFF);
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
						ImGui::SetTooltip("Type: %s", clazz->mpTypeInfoName);
					ImGui::NextColumn();
					ImGui::PopID();
					return;
				}
				else if (starts_with("DCArray", clazz->mpTypeInfoName)) {
					DCArray<void*>* pProxyArray = (DCArray<void*>*)pRawData;
					MetaClassDescription* pElementType = pProxyArray->GetContainerDataClassDescription();
					if (pElementType == nullptr)
						goto enum_string;
					int selected = 0;
					int result = ListTree(kn->c_str(), (int)pRawData & 0x7FFFFFF, pProxyArray, &DCArray_PropGetter, pProxyArray->GetSize(), pElementType, &selected);
					if(result == 1){
						MetaClassDescription* pDescription = pElementType;
						if (!(pDescription->mpVTable[0]) || !(pDescription->mpVTable[2]) || !(pDescription->mpVTable[3])) {
							MessageBoxA(0, "The type you selected is not a concrete type (ie its an abstract class for programmers) so cannot be instantiated!", "!!", MB_ICONINFORMATION);
						}
						else {
							void* tmpobj = operator new(pDescription->mClassSize);
							if(tmpobj){
								pDescription->Construct(tmpobj);
								pProxyArray->AddElement(0, 0, tmpobj);
								pDescription->Destroy(tmpobj);
								operator delete(tmpobj);
							}else{
								MessageBoxA(0, "Could not allocate new element", "Error", MB_ICONERROR);
							}
						}
					}else if(result == 2){
						pProxyArray->RemoveElement(selected);
					}
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
						ImGui::SetTooltip("(DC) Array of: %s", pElementType->mpTypeInfoName);
					ImGui::NextColumn();
					ImGui::PopID();
					return;
				}
				else if (starts_with("SArray", clazz->mpTypeInfoName)) {
					SArray<void*,1>* pProxyArray = (SArray<void*,1>*)pRawData;
					MetaClassDescription* pElementType = pProxyArray->GetContainerDataDescription();
					if (pElementType == nullptr)
						goto enum_string;
					int selected = 0;
					int result = ListTree(kn->c_str(), (int)pRawData & 0x7FFFFFF, pProxyArray, &SArray_PropGetter, pProxyArray->NUM_DATA_ELEM, pElementType, &selected);
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
						ImGui::SetTooltip("(S) Array of: %s", pElementType->mpTypeInfoName);
					ImGui::NextColumn();
					ImGui::PopID();
					return;
				}
				else if (starts_with("Map", clazz->mpTypeInfoName)) {
					Map<void*, void*>* pProxyArray = (Map<void*, void*>*)pRawData;
					if (MapTreeKey(pProxyArray->GetContainerKeyClassDescription(), 0, 0)) {
						int selected = 0;
						int result = MapTree(kn->c_str(), (int)pRawData & 0x7FFFFFF, &selected, pProxyArray);
						if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
							ImGui::SetTooltip("Map of: %s => %s", pProxyArray->GetContainerKeyClassDescription()->mpTypeInfoName, pProxyArray->GetContainerDataClassDescription()->mpTypeInfoName);
						if (result == 1) {
							ignore_ctrln = true;
							MetaClassDescription* pDescriptionK = pProxyArray->GetContainerKeyClassDescription(), * pDescriptionV = pProxyArray->GetContainerDataClassDescription();
							if (!(pDescriptionK->mpVTable[0]) || !(pDescriptionK->mpVTable[2]) || !(pDescriptionK->mpVTable[3])
								|| !(pDescriptionV->mpVTable[0]) || !(pDescriptionV->mpVTable[2]) || !(pDescriptionV->mpVTable[3])) {
								MessageBoxA(0, "The key or value is not a concrete type (ie its an abstract class for programmers) so cannot be instantiated!", "!!", MB_ICONINFORMATION);
							}
							else {
								tmpkey = operator new(pDescriptionK->mClassSize);
								if(!tmpkey){
									MessageBoxA(0, "Could not allocate new element key", "Error", MB_ICONERROR);
								}
								pDescriptionK->Construct(tmpkey);
								ImGui::OpenPopup("New Mapping");
							}
						}
						else if (result == 2) {
							pProxyArray->RemoveElement(selected);
						}
						if(tmpkey != 0 && ImGui::BeginPopupModal("New Mapping")){
							ImGui::Text("Enter Mapping Key:");
							MapTreeKeyInput(pProxyArray->GetContainerKeyClassDescription(), tmpkey);
							if(ImGui::Button("Create")){
								MetaClassDescription* pDescriptionK = pProxyArray->GetContainerKeyClassDescription(), * pDescriptionV = pProxyArray->GetContainerDataClassDescription();
								void* tmpobj = operator new(pDescriptionV->mClassSize);
								MapTreeKeyInput(pDescriptionK, tmpkey, true);
								if (tmpobj) {
									pDescriptionV->Construct(tmpobj);
									pProxyArray->AddElement(0, tmpkey, tmpobj);
									pDescriptionK->Destroy(tmpobj);
									pDescriptionV->Destroy(tmpkey);
									operator delete(tmpobj);
									operator delete(tmpkey);
								}
								else {
									operator delete(tmpkey);
									MessageBoxA(0, "Could not allocate new element value", "Error", MB_ICONERROR);
								}
								tmpkey = tmpobj = 0;
								ImGui::CloseCurrentPopup();
							}
							if(ImGui::Button("Exit")){
								operator delete(tmpkey);
								tmpkey = 0;
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
						ImGui::NextColumn();
						ImGui::PopID();
						return;
					}else{
						ImGui::Text("**MAP KEY EDITABLE SOON**");
						return;
					}
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
				ImGui::TreeNodeEx(kn->c_str(), flags);
				hovered = ImGui::LastHovered();
				if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
					ImGui::SetTooltip("Type: %s", clazz->mpTypeInfoName);
				ImGui::TableSetColumnIndex(1);

				if (bAllowDel && ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_D) && hovered) {
					to_del = CRC64_CaseInsensitive(0, kn->c_str());
				}

				u64 typeHash = clazz->mHash;
				u64 hash = typeHash;//alias
				void* pInst = pRawData;//alias

				if ((disbleMemCheck && memCheck && memCheck->mpEnumDescriptions) || (clazz->mFlags.mFlags & MetaFlag_EnumWrapperClass || starts_with("Enum", clazz->mpTypeInfoName)
					|| (clazz->mpFirstMember && (clazz->mpFirstMember->mFlags & MetaFlag_EnumIntType)))) {//only for enum INT types, enum strings i havent seen yet? see metaclassdescrition::getenummember
					MetaMemberDescription* pMemDesc;
					MetaEnumDescription* pEnum;
					if ((disbleMemCheck && memCheck && memCheck->mpEnumDescriptions)) {
						pEnum = memCheck->mpEnumDescriptions;
						pMemDesc = memCheck;
					}
					else {
						pMemDesc = clazz->GetEnumMember();
						if (pMemDesc != 0) {
							pEnum = pMemDesc->mpEnumDescriptions;
						}
						else goto cont;
					}
					int enumVal = *(int*)pInst;
					MetaEnumDescription* pSelectedDesc = nullptr;
					while (pEnum) {
						if (pEnum->mEnumIntValue == enumVal) {
							pSelectedDesc = pEnum;
							break;
						}
						pEnum = pEnum->mpNext;
					}
					pEnum = pMemDesc->mpEnumDescriptions;
					if (ImGui::BeginCombo("##", pSelectedDesc ? pSelectedDesc->mpEnumName : "No Value Set")) {
						while (pEnum) {
							if (ImGui::Selectable(pEnum->mpEnumName)) {
								enumVal = *((int*)pInst) = pEnum->mEnumIntValue;
								ImGui::SetItemDefaultFocus();
							}
							pEnum = pEnum->mpNext;
						}
						ImGui::EndCombo();
					}
					ImGui::NextColumn();
					ImGui::PopID();
					return;
				}
			cont:
				if (typeHash == hash_int8) {
					i8 step = 1, maxstep = 100;
					ImGui::InputScalar("##value", ImGuiDataType_S8, pInst, &step, &maxstep);
				}
				else if (typeHash == hash_uint8) {
					u8 step = 1, maxstep = 100;
					ImGui::InputScalar("##value", ImGuiDataType_U8, pInst, &step, &maxstep);
				}
				else if (typeHash == hash_uint16) {
					u16 step = 1, maxstep = 100;
					ImGui::InputScalar("##value", ImGuiDataType_U16, pInst, &step, &maxstep);
				}
				else if (typeHash == hash_int16) {
					i16 step = 1, maxstep = 100;
					ImGui::InputScalar("##value", ImGuiDataType_S16, pInst, &step, &maxstep);
				}
				else if (typeHash == hash_long || typeHash == hash_int || typeHash == hash_int32) {
					i32 step = 1, maxstep = 100;
					ImGui::InputScalar("##value", ImGuiDataType_S32, pInst, &step, &maxstep);
				}
				else if (typeHash == hash_uint32) {
					u32 step = 1, maxstep = 100;
					ImGui::InputScalar("##value", ImGuiDataType_U32, pInst, &step, &maxstep);
				}
				else if (typeHash == hash_int64) {
					u64 step = 1, maxstep = 100;
					ImGui::InputScalar("##value", ImGuiDataType_S64, pInst, &step, &maxstep);
				}
				else if (typeHash == hash_uint64) {
					if (disbleMemCheck && memCheck && memCheck->mFlags & Internal_MetaFlag_ShowAsHex) {
						if ((ImGui::Button("Modify Hex Value") && mod_sym_inst == 0) || (mod_sym_inst == pInst && pInst != 0))
							modify_symbol(*((Symbol*)pInst), kn->c_str(), pInst);//can interpret as a symbol
					}
					else {
						i64 step = 1, maxstep = 100;
						ImGui::InputScalar("##value", ImGuiDataType_U64, pInst, &step, &maxstep);
					}
				}
				else if (typeHash == hash_double) {
					ImGui::InputDouble("##value", (double*)pInst);
				}
				else if (typeHash == hash_float) {
					ImGui::InputFloat("##value", (float*)pInst);
				}
				else if (typeHash == hash_bool) {
					ImGui::Checkbox("##value", (bool*)pInst);
				}
				else if (typeHash == hash_str) {
					ImGui::InputText("##value", (String*)pInst);
				}
				else if (typeHash == hash_sym) {
					if ((ImGui::Button("Modify Symbol Value") && mod_sym_inst == 0) || (mod_sym_inst == pInst && pInst != 0))
						modify_symbol(*((Symbol*)pInst), kn->c_str(), pInst);
				}
				else if (!PropTree_Math(typeHash, pInst)) {			
					if (starts_with("Handle", clazz->mpTypeInfoName)) {
						if (ImGui::Button("Modify Handle Value") && mh_uid == 0) {
							mh_uid = uid;
							mh_item = Symbol{ CRC64_CaseInsensitive(0, kn->c_str()) };
						}
						else if (mh_uid == uid && mh_item == Symbol{ CRC64_CaseInsensitive(0, kn->c_str()) }) {
							Handle<void*>* typecastraw = (Handle<void*>*)pRawData;
							modify_handle(clazz, typecastraw->mHandleObjectInfo.mObjectName);
						}
					}
					else if (!strcmp("T3MaterialData", clazz->mpTypeInfoName)) {
						T3MaterialData* mat = (T3MaterialData*)pRawData;
						if (ImGui::Button("Edit Material Data")) {
							ImGui::OpenPopup("Material Data (T3MaterialData)");
						}
						if (ImGui::BeginPopupModal("Material Data (T3MaterialData)")) {
							bool node_open = ImGui::BeginTable("t3matdat", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable);
							if(node_open){
								std::string n{};
								bool h{};
								ImGui::TableNextRow();
								PropTreeItem(GetMetaClassDescription<Symbol>(), &mat->mMaterialName, &(n = "Material Name"), 6900, to_del, i, false, h,clazz->GetMemberDescription("mMaterialName"));
								ImGui::TableNextRow();
								PropTreeItem(GetMetaClassDescription<Symbol>(), &mat->mRuntimePropertiesName, 
									&(n = "Runtime Properties Name"), 6901, to_del, i+1, false, h,clazz->GetMemberDescription("mRuntimePropertiesName"));
								ImGui::TableNextRow();
								PropTreeItem(GetMetaClassDescription<Symbol>(), &mat->mLegacyRenderTextureProperty, &(n = "Legacy Render Texture Property"), 
									6902, to_del, i+2, false, h,clazz->GetMemberDescription("mLegacyRenderTextureProperty"));
								ImGui::TableNextRow();
								PropTreeItem(GetMetaClassDescription<Symbol>(), &mat->mLegacyBlendModeRuntimeProperty, 
									&(n = "Legacy Blend Mode Property"), 6903, to_del, i+3, false, h,clazz->GetMemberDescription("mLegacyBlendModeRuntimeProperty"));
								ImGui::TableNextRow();
								PropTreeItem(GetMetaClassDescription<long>(), &mat->mDomain, &(n = "Domain"), 6904, to_del, i+4, false, h,clazz->GetMemberDescription("mDomain"));
								ImGui::TableNextRow();
								PropTreeItem(GetMetaClassDescription<long>(), &mat->mVersion, &(n = "Version"), 6905, to_del, i+5, false, h,clazz->GetMemberDescription("mVersion"));

								ImGui::TableNextRow();
								PropTreeItem(GetMetaClassDescription<DCArray<T3MaterialRuntimeProperty>>(), &mat->mRuntimeProperties, &(n = "Runtime Properties"), 6906, to_del,
									i + 6, false,h, clazz->GetMemberDescription("mRuntimeProperties"));

								ImGui::TableNextRow();
								PropTreeItem(GetMetaClassDescription<DCArray<T3MaterialCompiledData>>(), &mat->mCompiledData2, &(n = "Compiled Data"), 6907, to_del,
									i + 7, false,h, clazz->GetMemberDescription("mCompiledData2"));

								ImGui::EndTable();
							}
							if (ImGui::Button("Close"))
								ImGui::CloseCurrentPopup();
							ImGui::End();
						}
					}
					else if (!strcmp("BinaryBuffer", clazz->mpTypeInfoName)) {
						if (ImGui::Button("Save Buffer")) {
							nfdchar_t* outpth{};
							if (NFD_SaveDialog(0, "bin", &outpth, L"Choose output binary file") == NFD_OKAY) {
								DataStreamFileDisc out = _OpenDataStreamFromDisc_(outpth, WRITE);
								if (out.IsInvalid())
									MessageBoxA(0, "Could not open file", "Error opening file", MB_ICONERROR);
								else {
									BinaryBuffer* bb = (BinaryBuffer*)pRawData;
									if (bb->mDataSize)
										out.Serialize(bb->mpData, (unsigned long long)bb->mDataSize);
								}
							}
						}
						ImGui::SameLine();
						if (ImGui::Button("Load Buffer")) {
							nfdchar_t* outpth{};
							if (NFD_OpenDialog(0, "bin", &outpth) == NFD_OKAY) {
								DataStreamFileDisc out = _OpenDataStreamFromDisc_(outpth, READ);
								if (out.IsInvalid())
									MessageBoxA(0, "Could not open file", "Error opening file", MB_ICONERROR);
								else {
									BinaryBuffer* bb = (BinaryBuffer*)pRawData;
									if (bb->AllocData((int)out.GetSize())) {
										out.Serialize(bb->mpData, out.GetSize());
									}
									else
										MessageBoxA(0, "Error reading in data from binary file", "Error", MB_ICONERROR);
								}
							}
						}
						ImGui::SameLine();
						if (ImGui::Button("Memory Editor")) {
							ImGui::OpenPopup("Binary Buffer Editor");
						}
						if (ImGui::BeginPopupModal("Binary Buffer Editor")) {
							BinaryBuffer* bb = (BinaryBuffer*)pRawData;
							bbEditor.DrawContents(bb->mpData, bb->mDataSize);
							ImGui::SameLine();
							if (ImGui::Button("Close"))
								ImGui::CloseCurrentPopup();
							ImGui::EndPopup();
						}
					}
					else if (starts_with("BitSet<", clazz->mpTypeInfoName)) {
						void* hash = (void*)(CRC64(0, kn->c_str()) ^ (u64)pRawData ^ (u64)clazz);
						ImGui::PushID(hash);
						if (ImGui::Button("Edit Bitset")) {
							ImGui::OpenPopup("Bitset");
						}
						if (ImGui::BeginPopupModal("Bitset")) {
							BitSet<BokehOcclusionType, 1, 0>* pProxy = (BitSet<BokehOcclusionType, 1, 0>*)pRawData;
							MetaClassDescription* sEnumType = 0;
							int num = 0;
							int initial = 0;
							pProxy->GetTypeInformation(&sEnumType, &num, &initial);
							MetaMemberDescription* pEnum = sEnumType->GetEnumMember();
							if (pEnum == 0) {
								ImGui::Text("Contact me! Type does not have attached enum information");
							}else{
								MetaEnumDescription* desc = pEnum->mpEnumDescriptions;
								while(desc){
									if (desc->mEnumIntValue >= initial) {
										bool bit = pProxy->operator[]((BokehOcclusionType)desc->mEnumIntValue);
										ImGui::Checkbox(desc->mpEnumName, &bit);
										pProxy->Set(BokehOcclusionType{ (BokehOcclusionType)desc->mEnumIntValue }, bit);
									}
									desc = desc->mpNext;
								}
							}
							if (ImGui::Button("Close")) {
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
						ImGui::PopID();
					}else if(starts_with("BitSetBase", clazz->mpTypeInfoName)){
						u32* words = (u32*)pRawData;
						void* hash = (void*)(CRC64(0, kn->c_str()) ^ (u64)pRawData ^ (u64)clazz);
						ImGui::PushID(hash);
						std::string wordsStr = clazz->mpTypeInfoName + 11;
						wordsStr = wordsStr.substr(0, wordsStr.find('>'));
						u32 wordsn{};
						sscanf(wordsStr.c_str(), "%d", &wordsn);
						if (ImGui::Button("Edit Bitset")) {
							ImGui::OpenPopup("Bitset");
							bbEditor.OptShowDataPreview = true;
						}
						if (ImGui::BeginPopupModal("Bitset")) {
							bbEditor.DrawContents(words, wordsn * 4);
							if (ImGui::Button("Close"))
								ImGui::CloseCurrentPopup();
							ImGui::EndPopup();
						}
						ImGui::PopID();
					}else if(!strcmp("Flags",clazz->mpTypeInfoName)){
						void* hash = (void*)(CRC64(0, kn->c_str()) ^ (u64)pRawData ^ (u64)clazz);
						ImGui::PushID(hash);
						if (ImGui::Button("Edit Bit Flags"))
							ImGui::OpenPopup("Flags");
						if(ImGui::BeginPopupModal("Flags")){
							char b[10];
							u32& flags = ((Flags*)pRawData)->mFlags;
							if(disbleMemCheck && memCheck && memCheck->mpFlagDescriptions){
								MetaFlagDescription* flagDesc = memCheck->mpFlagDescriptions;
								while(flagDesc){
									bool bit = (flags & (u32)flagDesc->mFlagValue) != 0;
									ImGui::Checkbox(flagDesc->mpFlagName, &bit);
									if (bit)
										flags |= (u32)flagDesc->mFlagValue;
									else
										flags &= ~((u32)flagDesc->mFlagValue);
									flagDesc = flagDesc->mpNext;
								}
							}
							else {
								for (unsigned int i = 0; i < 32; i++) {
									bool bit = (flags >> i) & 1;
									sprintf(b, "Bit %d", i + 1);
									ImGui::Checkbox(b, &bit);
									if (bit)
										flags |= (1 << i);
									else
										flags &= ~(1u << i);
								}
							}
							if (ImGui::Button("Close"))
								ImGui::CloseCurrentPopup();
							ImGui::EndPopup();
						}
						ImGui::PopID();
					}
					else {
					enum_string:
						void* hash = (void*)(CRC64(0, kn->c_str()) ^ (u64)pRawData ^ (u64)clazz);
						ImGui::PushID(hash);
						if (ImGui::Button("Edit Raw Data"))
							ImGui::OpenPopup(clazz->mpTypeInfoName);
						if(ImGui::BeginPopupModal(clazz->mpTypeInfoName)){
							if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
							{
								TypeTree("Type Class Members", uid+1, clazz, pRawData);
								ImGui::EndTable();
							}
							if(ImGui::Button("Close")){
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
						ImGui::PopID();
					}
				}

				ImGui::NextColumn();
				ImGui::PopID();
			}
		}
	}

	void PropTree(PropertySet* set, const char* prefix, int uid)
	{
		// Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
		ImGui::PushID(uid);

		// Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		bool node_open = ImGui::TreeNode(prefix);
		ImGui::TableSetColumnIndex(1);

		MetaClassDescription* pAcDesc = nullptr;

		u64 to_del = 0;

		if (node_open)
		{
			for (int i = 0; i < set->mKeyMap.mSize; i++) {
				PropertySet::KeyInfo* m = set->mKeyMap.mpStorage + i;
				if (m->mpValue) {
					std::string* kn;
					std::string* keyName = SymMap_ResolveHash(resolve_buf, sym_map, fn_map,  m->mKeyName.GetCRC(), prop_key);
					if (keyName == nullptr) {
						kn = &(sym_map[m->mKeyName.GetCRC()] = to_symbol(m->mKeyName.GetCRC()));
					}
					else
						kn = keyName;
					bool hovered{ false };
					PropTreeItem(m->mpValue->mpDataDescription, m->mpValue->mpValue, kn, uid, to_del, i, true, hovered);
				}
			}
			if (cur_new_parent == set) {
				ImGui::OpenPopup("New Property");
				if (ImGui::BeginPopupModal("New Property")) {
					ImGui::Text("Name");
					ImGui::SameLine();
					ImGui::InputText("##np", &nprop_field);
					ImGui::Text("Type");
					ImGui::SameLine();
					int sel_ind{};
					if (ImGui::BeginCombo("##combo1", nprop_type))
					{
						for (int n = 0; n < (int)sRuntime.filterListMSTypes.size(); n++)
						{
							bool is_selected = !strcmp(nprop_type, sRuntime.filterListMSTypes[n]);
							if(selected)
								sel_ind = n;
							if (ImGui::Selectable(sRuntime.filterListMSTypes[n], is_selected)) {
								nprop_type = sRuntime.filterListMSTypes[n];
								sel_ind = n;
							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
					if (ImGui::Button("Create")) {
						if (nprop_field.size() == 0) {
							MessageBoxA(0, "Please enter a property name!", "!!", MB_ICONINFORMATION);
						}
						else if (set->ExistsKey(Symbol{ nprop_field.c_str() }, false)) {
							MessageBoxA(0, "A property with that name already exists in the current property set!", "!!", MB_ICONINFORMATION);
						}
						else {
							MetaClassDescription* pDescription = TelltaleToolLib_FindMetaClassDescription(nprop_type, true);
							if (!(pDescription->mpVTable[0]) || !(pDescription->mpVTable[2]) || !(pDescription->mpVTable[3])) {
								MessageBoxA(0, "The type you selected is not a concrete type (ie its an abstract class for programmers) so cannot be instantiated!", "!!", MB_ICONINFORMATION);
							}
							else {
								pAcDesc = pDescription;
								ImGui::CloseCurrentPopup();
							}
						}
					}
					if(ImGui::Button("Exit")){
						nprop_field = "";
						nprop_type = "String";
						cur_new_parent = nullptr;
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_N) && frame_check_addrem && !ignore_ctrln && !tmpkey) {
				frame_check_addrem = false;
				cur_new_parent = set;
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
		if (pAcDesc) {
			set->AddProperty(nprop_field.c_str(), pAcDesc, 0);//create new
			sRuntime.symbolMaps[prop_key].insert(nprop_field); 
			nprop_field = "";
			nprop_type = "String";
			cur_new_parent = nullptr;
		}
		if (to_del!=0) {
			set->RemoveProperty(to_del);
		}
	}

	virtual void _render() override {
		//TODO EMBEDDED PROPS
		frame_check_addrem = true;
		if (ext_access_gate && *ext_access_gate == false) {
			ImGui::Text("This PropertySet set is not available as the parent file was unloaded.");
			return;
		}
		resolve_buf[0] = 0;
		if (selected) {
			ImGui::Text("PropertySet Editor for: %s", game_n);
			ImGui::Text("Press Ctrl+N to create a new property in the current tree branch");
			ImGui::Text("Press Ctrl+D to delete the highlighted property   ");
			ImGui::Text("Name:");
			ImGui::SameLine();
			ImGui::InputText("##pname", &prop_name, save_changes == nullptr ? 0 : ImGuiInputTextFlags_ReadOnly);
			ImGui::Checkbox("Replace Existing On Import", &replace_all);
			ImGui::Checkbox("Update Parents On Import", &update_parents);
			if (ImGui::Button("Import Property Set")) {
				TelltaleToolLib_SetBlowfishKey(game);
				nfdchar_t* outp = nullptr;            
				if (NFD_OpenDialog("prop", 0, &outp) == NFD_OKAY) {
					MetaStream mTempStream{};
					DataStreamFileDisc* prop = OpenDataStreamFromDisc(outp, READ);
					mTempStream.Open(prop, MetaStreamMode::eMetaStream_Read, {});
					if (mTempStream.mbErrored) {
						MessageBoxA(0, "Could not open the prop file (meta stream error). Please contact me, using the contact tab above.", "Error", MB_ICONERROR);
					}
					else {
						PropertySet tempProp{};
						if (PerformMetaSerializeAsync<PropertySet>(&mTempStream, &tempProp) != eMetaOp_Succeed) {
							MessageBoxA(0, "Could not open the prop file (prop error). Please contact me, using the contact tab above.", "Error", MB_ICONERROR);
						}
						else {
							PropertySet* pDestProps = &Props();
							if (!imp_yet) {
								imp_yet = true;
								prop_name = std::filesystem::path{ outp }.filename().string();
								pDestProps->mPropVersion = tempProp.mPropVersion;
								pDestProps->mPropertyFlags = 0;
							}
							/*update and import parent props if needed*/
							if (update_parents) {
								for (int i = 0; i < tempProp.mParentList.mSize; i++) {
									PropertySet::ParentInfo* parent = tempProp.mParentList.mpStorage + i;
									if (DCArray_Contains(pDestProps->mParentList, *parent))
										continue;
									DCArray_Push(pDestProps->mParentList, parent);
								}
							}
							/*import props*/
							for (int i = 0; i < tempProp.mKeyMap.mSize; i++) {
								PropertySet::KeyInfo* ky = tempProp.mKeyMap.mpStorage + i;
								//change prop val
								bool updated = false;
								for (int j = 0; j < pDestProps->mKeyMap.mSize; j++) {
									PropertySet::KeyInfo* existing = pDestProps->mKeyMap.mpStorage + j;
									if (existing->mKeyName == ky->mKeyName) {
										updated = true;
										if (replace_all) {
											existing->mFlags = ky->mFlags;
											existing->mpValue = std::move(ky->mpValue);
										}
										break;
									}
								}
								if (!updated) {
									pDestProps->mKeyMap.AddElementMove(0, 0, ky);
								}
							}
							//done
							return;//wait till next frame
						}
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Save To File")) {
				nfdchar_t* outp{ 0 };
				if (NFD_PickFolder(0, &outp, L"Select output folder") == NFD_OKAY) {
					std::string pth{ outp };
					pth += "/";
					std::string nm{ prop_name };
					if (nm.size() == 0) {
						MessageBoxA(0, "Please enter a file name into the property file name text field!", "Enter name", MB_ICONINFORMATION);
					}
					else {
						if (!ends_with(nm, ".prop"))
							nm += ".prop";
						pth += _STD move(nm);
						DataStreamFileDisc* ds = _OpenDataStreamFromDisc(pth.c_str(), WRITE);
						if (ds == nullptr) {
							MessageBoxA(0, "The file could not be opened for writing!", "Error", MB_ICONERROR);
						}
						else {
							MetaStream mTempStream{};
							mTempStream.Open(ds, MetaStreamMode::eMetaStream_Write, {});
							if (PerformMetaSerializeAsync<PropertySet>(&mTempStream, &Props()) != eMetaOp_Succeed) {
								MessageBoxA(0, "There was a problem serializing the property set to the meta stream. Please contact me (PROPTASK)", "Error", MB_ICONERROR);
							}
							else {
								mTempStream.Close();
								MessageBoxA(0, "The property set was exported!", "Success!", MB_ICONINFORMATION);
							}
						}
					}
					free(outp);
				}
			}
			PropertySet& prop = Props();
			ImGui::Separator();
			ImGui::SetWindowFontScale(1.3f);
			ImGui::Text("Parent Property Files");
			ImGui::SameLine();
			ImGui::InputText("##addpar", &nparent);
			ImGui::SameLine();
			bool sel = ImGui::Button("Add Parent");
			if (sel) {
				if (nparent.size() == 0) {
					MessageBoxA(0, "Please enter a name into the text box!", "!!", MB_ICONINFORMATION);
				}
				else {
					if (!ends_with(nparent, ".prop"))
						nparent += ".prop";
					bool b = false;
					Symbol sym{ nparent.c_str() };
					for (int i = 0; i < prop.mParentList.mSize; i++) {
						if (sym == prop.mParentList.mpStorage[i].mhParent.mHandleObjectInfo.mObjectName) {
							b = true;
							MessageBoxA(0, "That file is already a parent!","!!", MB_ICONINFORMATION);
							break;
						}
					}
					if (!b) {
						Handle<PropertySet> hprop{};
						hprop.SetObjectName(sym);
						prop.AddParent(hprop);
						nparent = "";
					}
				}
			}
			ImGui::SameLine();
			sel = ImGui::Button("Remove Parent");  
			if (sel && prop.mParentList.mSize > 0) {
				if (nparent.size() == 0) {
					MessageBoxA(0, "Please enter a name into the text box!", "!!", MB_ICONINFORMATION);
				}
				else {
					if (!ends_with(nparent, ".prop"))
						nparent += ".prop";
					Symbol sym{ nparent.c_str() };
					for (int i = 0; i < prop.mParentList.mSize; i++) {
						if (sym == prop.mParentList.mpStorage[i].mhParent.mHandleObjectInfo.mObjectName) {
							Handle<PropertySet> h{};
							h.SetObjectName(sym);
							prop.RemoveParent(h);
							break;
						}
					}
					nparent = "";
				}
			}
			ImGui::SetWindowFontScale(1.f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
			if (ImGui::BeginTable("split1", 1, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
			{
				for (int i = 0; i < prop.mParentList.mSize; i++) {
					Handle<PropertySet> hProp = prop.mParentList.mpStorage[i].mhParent;
					std::string* kn;
					std::string* keyName = SymMap_ResolveHash(resolve_buf, sym_map, fn_map, hProp.mHandleObjectInfo.mObjectName.GetCRC(), file_name);
					if (keyName == nullptr) {
						kn = &(sym_map[hProp.mHandleObjectInfo.mObjectName.GetCRC()] = to_symbol(hProp.mHandleObjectInfo.mObjectName.GetCRC()));
					}
					else
						kn = keyName;
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					ImGui::Text(kn->c_str());
				}
				ImGui::EndTable();
			}
			ImGui::PopStyleVar();
			ImGui::Separator();
			ImGui::SetWindowFontScale(1.3f);
			ImGui::Text("Properties");
			ImGui::SetWindowFontScale(1.f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
			if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
			{
				PropTree(&prop, "Properties", (int)this & 0x7FFFFFF);
				ImGui::EndTable();
			}
			ImGui::PopStyleVar();
			ignore_ctrln = false;
			//SHOULD NOT BE ABLE TO OPEN ANOTHER PROP AND IMPORT WHEN CURRENTLY INSPECTING ELEMENTS, IF ELEMENT IS REPLACED WITJ DIFFERENT TYPE THEN LOTS OF PROBLEMS HAPPEN
		}
		else {
			game = select_gameid_dropdown(game);
			if (game && ImGui::Button("Select")) {
				selected = true;
				game_n = sBlowfishKeys[TelltaleToolLib_GetGameKeyIndex(game)].game_name;
			}
		}
	}

public:

	deleteme()

	taskctor(PropTask) {
		save_changes = 0;
	}

	~PropTask() {
		if (save_changes != nullptr)
			save_changes(Props(), ext_user_data);
	}

};


class MeshTask : public InspectorTask {


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

	void process_mtl(std::string& in) {
		if (!mtl_remove_png)
			in += ".png";
		in = mtl_prefix + in + mtl_suffix;
		if (bLowerCase)
			in = lowercase(in);

	}

	static void __declspec(noinline) process_mtl_cb(MeshTask* task, std::string& in) {
		task->process_mtl(in);
	}

	bool try_open_mesh(std::string file) {
		if (!mStream.Attach(_OpenDataStreamFromDisc(file.c_str(), READ), MetaStreamMode::eMetaStream_Read, {})) {
			err = "Invalid meta stream, are you sure this is a D3DMesh?";
			return false;
		}
		if (TelltaleToolLib_GetGameKeyIndex(id) == TelltaleToolLib_GetGameKeyIndex("mcsm")) {
			if (mpLegacyMesh)
				delete mpLegacyMesh;
			mpLegacyMesh = new LegacyD3DMesh;
			if (mpNormalMesh)
				delete mpNormalMesh;
			mpNormalMesh = 0;
			MetaOpResult result = PerformMetaSerializeAsync(&mStream, mpLegacyMesh);
			if (result != eMetaOp_Succeed) {
				err = "We was a problem reading from the main mesh data! Please contact me urgently.";
				return false;
			}
			bLoaded = bLegacy = true;
			texture_names.clear();
			unk_textures.clear();
			std::string page = "Files_D3DTX_";
			page += id;
			String res = "";
			for (int i = 0; i < 14; i++) {
				DCArray<LegacyD3DMesh::Texture>* textures = mpLegacyMesh->mTextures.mData + i;
				for (int x = 0; i < textures->GetSize(); i++) {
					TelltaleToolLib_GetGlobalHashDatabase()->FindEntry((HashDatabase::Page*)0, textures->operator[](i).mName.mHandleObjectInfo.mObjectName.GetCRC(), &res);
					if (res.length() == 0) {
						unk_textures.push_back(to_symbol(textures->operator[](i).mName.mHandleObjectInfo.mObjectName.GetCRC()));
					}
					else {
						texture_names.push_back(std::move(res));
					}
					res = "";
				}
			}
		}
		else if (TelltaleToolLib_GetGameKeyIndex(id) >= TelltaleToolLib_GetGameKeyIndex("batman")) {
			if (mpLegacyMesh)
				delete mpLegacyMesh;
			mpLegacyMesh = 0;
			if (mpNormalMesh)
				delete mpNormalMesh;
			mpNormalMesh = new D3DMesh;
			MetaOpResult result = PerformMetaSerializeAsync(&mStream, mpNormalMesh);
			if (result != eMetaOp_Succeed) {
				err = "We was a problem reading from the main mesh data! Please contact me urgently. (2)";
				return false;
			}
			bLoaded = true;
			bLegacy = false;
			texture_names.clear();
			unk_textures.clear();
			std::string page = "Files_D3DTX_";
			page += id;
			String res = "";
			for (int i = 0; i < mpNormalMesh->mInternalResources.mSize; i++) {
				HandleUncached& resource = mpNormalMesh->mInternalResources[i];
				if (resource.GetTypeDesc() != GetMetaClassDescription<PropertySet>()) {
					printf("WARNING: Mesh has internal resource which is not a property set! Type: %s\n", resource.GetTypeDesc()->mpTypeInfoExternalName);
					continue;
				}
				PropertySet* props = (PropertySet*)resource.GetHandleObjectPointer();
				Handle<T3Texture>* pDiffuseTex = (Handle<T3Texture>*)props->GetProperty("Material - Diffuse Texture");
				if (pDiffuseTex == nullptr) {
					printf("WARNING: Mesh internal resource properties does not contain a diffuse texture reference!\n");
					continue;
				}
				if (props->GetPropertyType("Material - Diffuse Texture") != GetMetaClassDescription<Handle<T3Texture>>()) {
					printf("ERROR: Mesh internal resource property texture reference is not a Handle<T3Texture>!!\n");
					continue;
				}
				u64 texCRC = pDiffuseTex->mHandleObjectInfo.mObjectName.GetCRC();
				TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(TelltaleToolLib_GetGlobalHashDatabase()->FindPage(page.c_str()), texCRC, &res);
				if (res.length() == 0) {
					unk_textures.push_back(std::string{ "Symbol<" } + std::to_string(texCRC) + std::string{ ">" });
				}
				else {
					texture_names.push_back(std::move(res));
				}
				res = "";
			}
			page = "Files_PROP_";
			page += id;
			for (int i = 0; i < mpNormalMesh->mInternalResources.mSize; i++) {
				HandleUncached& resource = mpNormalMesh->mInternalResources[i];
				PropertySet* props = (PropertySet*)resource.GetHandleObjectPointer();
				for (int k = 0; k < props->mParentList.mSize; k++) {
					u64 matCRC = props->mParentList[k].mhParent.mHandleObjectInfo.mObjectName.GetCRC();
					TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(TelltaleToolLib_GetGlobalHashDatabase()->FindPage(page.c_str()), matCRC, &res);
					bool sk = false;
					for (auto& already : norm_materials)
						if (already == res)
							sk = true;
					if (res.length() > 0 && !sk) {
						norm_materials.push_back(std::move(res));
					}
					res = "";
				}
			}
			return true;
		}
		else {
			err = "The currently selected game is not currently supported for opening at the moment!";
			return false;
		}
		return true;
	}

	void render_mtl_edit() {
		ImGui::Separator();
		ImGui::Text("The following can be used to modify the texture file references in the MTL files which are converted. Default is: tex.d3dtx => tex.png");
		ImGui::InputText("Texture Prefix", &mtl_prefix);
		ImGui::InputText("Texture Suffix", &mtl_suffix);
		ImGui::Checkbox("Strip PNG Extension", &mtl_remove_png);
		ImGui::Checkbox("Lowercase MTL References", &bLowerCase);
		ImGui::Separator();
	}

	int open_index = -1;
	bool* open_gate = 0;

	virtual void _render() override {
		if (id && ImGui::Button("Convert Whole Folder")) {
			TelltaleToolLib_SetBlowfishKey(id);
			nfdchar_t* outpath{ 0 };
			if (NFD_OKAY == NFD_PickFolder(0, &outpath, L"Select Folder with D3DMeshes")) {
				std::string folder = outpath;
				free(outpath);
				std::string outputFolder = folder + "/Converted/";
				std::filesystem::create_directory(outputFolder);
				std::vector<std::string> failed{};
				for (auto& it : std::filesystem::directory_iterator(std::filesystem::path{ folder })) {
					if (it.is_regular_file() && ends_with(it.path().filename().string(), ".d3dmesh")) {
						texture_names.clear();
						unk_textures.clear();
						if (mpLegacyMesh)
							delete mpLegacyMesh;
						if (mpNormalMesh)
							delete mpNormalMesh;
						mpNormalMesh = 0;
						mpLegacyMesh = 0;
						std::string outobj = outputFolder + it.path().filename().string();
						outobj = outobj.substr(0, outobj.find_last_of('.')) + ".obj";
						std::string outmtl = outobj.substr(0, outobj.find_last_of('.')) + ".mtl";
						if (!try_open_mesh(it.path().string())) {
							failed.push_back("Could not open meta stream: " + it.path().filename().string());
							continue;
						}
						std::string err{};
						if (mpLegacyMesh) {
							if (!convert_obj_legacy(*mpLegacyMesh, id, outobj, outmtl, err, texture_names, process_mtl_cb, this))
								failed.push_back(it.path().filename().string() + " => " + err);
							else
								printf("convert success: %s\n", it.path().filename().string().c_str());
						}
						else {
							if (!convert_obj_newer(*mpNormalMesh, id, outobj, outmtl, err, texture_names, process_mtl_cb, this))
								failed.push_back(it.path().filename().string() + " => " + err);
							else
								printf("convert success: %s\n", it.path().filename().string().c_str());
						}
						if (mpLegacyMesh)
							delete mpLegacyMesh;
						if (mpNormalMesh)
							delete mpNormalMesh;
						mpNormalMesh = 0;
						mpLegacyMesh = 0;
						bLoaded = bLegacy = false;
					}
				}
				for (auto& fail : failed)
					printf("A file was not converted: %s\n", fail.c_str());
			}
		}
		if (err) {
			ImGui::Text("An error was encountered while opening this file:");
			ImGui::Text(err);
			return;
		}
		if (!bLoaded) {
			ImGui::Text("Select the game the mesh belongs to:");
			id = select_gameid_dropdown(id);
			if (id && ImGui::Button("Open File")) {
				TelltaleToolLib_SetBlowfishKey(id);
				std::string file = open_file("d3dmesh");
				if (!try_open_mesh(file))
					return;
			}
		}
		TelltaleToolLib_SetBlowfishKey(id);
		render_mtl_edit();
		if (bLegacy && bLoaded) {
			ImGui::InputInt("Mesh Version", &mpLegacyMesh->mVersion);
			ImGui::InputText("Mesh Name", &mpLegacyMesh->mName);
			if (ImGui::Button("Export D3DMesh")) {
				MessageBoxA(0, "Legacy meshes (the one you have loaded) cannot be re-exported at the moment.", "Error", MB_ICONERROR);
			}
			ImGui::SameLine();
			if (ImGui::Button("Convert to OBJ")) {
				MessageBoxA(0, "WARNING. Exporting meshes may sometimes not work, and in most exported OBJs the UVs are screwed.", "Warning", MB_ICONINFORMATION);
				nfdchar_t* out{ 0 };
				if (NFD_OKAY == NFD_SaveDialog("obj", 0, &out, L"Select output OBJ")) {
					std::string outfile = out;
					if (!ends_with(outfile, ".obj"))
						outfile += ".obj";
					free(out);
					std::string outmtl = outfile.substr(0, outfile.find_last_of('.')) + ".mtl";
					std::string er{};
					if (convert_obj_legacy(*mpLegacyMesh, id, outfile, outmtl, er, texture_names, process_mtl_cb, this)) {
						MessageBoxA(0, "The mesh was successfully converted to WaveFront's OBJ Format! Textures should be PNG!", "Success!", MB_ICONINFORMATION);
					}
					else {
						MessageBoxA(0, er.c_str(), "Error converting mesh!", MB_ICONERROR);
					}
				}
			}
			if (ImGui::ListBoxHeader("Mesh Textures")) {
				for (auto& tex : texture_names) {
					ImGui::Selectable(tex.c_str());
				}
				ImGui::ListBoxFooter();
			}
			if (ImGui::ListBoxHeader("Unresolved Mesh Textures")) {
				for (auto& tex : unk_textures) {
					ImGui::Selectable(tex.c_str());
				}
				ImGui::ListBoxFooter();
			}
		}
		else if (bLoaded) {
			ImGui::InputInt("Mesh Version", (int*)&mpNormalMesh->mVersion);
			ImGui::InputText("Mesh Name", &mpNormalMesh->mName);
			if (ImGui::Button("Export D3DMesh")) {
				MessageBoxA(0, "WARNING. You should have only changed data or resources!", "Warning", MB_ICONINFORMATION);
				nfdchar_t* out{ 0 };
				if (NFD_OKAY == NFD_SaveDialog("d3dmesh", 0, &out, L"Select output D3DMesh")) {
					std::string outfile = out;
					if (!ends_with(outfile, ".d3dmesh"))
						outfile += ".d3dmesh";
					free(out);
					DataStreamFileDisc fs = _OpenDataStreamFromDisc_(outfile.c_str(), WRITE);
					MetaStream ms{};
					ms.InjectVersionInfo(mStream);
					ms.mbDontDeleteStream = true;
					ms.Open(&fs, MetaStreamMode::eMetaStream_Write, {});
					if (ms.mbErrored) {
						MessageBoxA(0, "Error opening the file", "Error", MB_ICONERROR);
					}
					else {
						if (PerformMetaSerializeAsync(&ms, mpNormalMesh) != eMetaOp_Succeed){
							MessageBoxA(0, "Could not serialize mesh data! Contact me.", "Error in serializing", MB_ICONERROR);
						}else{
							MessageBoxA(0, "Successfully saved D3DMesh!", "Success", MB_ICONINFORMATION);
						}
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Convert to OBJ")) {
				nfdchar_t* out{ 0 };
				if (NFD_OKAY == NFD_SaveDialog("obj", 0, &out, L"Select output OBJ")) {
					std::string outfile = out;
					if (!ends_with(outfile, ".obj"))
						outfile += ".obj";
					free(out);
					std::string outmtl = outfile.substr(0, outfile.find_last_of('.')) + ".mtl";
					std::string er{};
					if (convert_obj_newer(*mpNormalMesh, id, outfile, outmtl, er, texture_names, process_mtl_cb, this)) {
						MessageBoxA(0, "The mesh was successfully converted to WaveFront's OBJ Format! Textures should be PNG!", "Success!", MB_ICONINFORMATION);
					}
					else {
						MessageBoxA(0, er.c_str(), "Error converting mesh!", MB_ICONERROR);
					}
				}
			}
			if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable)) {
				for (int i = 0; i < mpNormalMesh->mInternalResources.GetSize(); i++) {
					Symbol name = mpNormalMesh->mInternalResources.mpStorage[i].GetObjectName();
					MetaClassDescription* resourceType = mpNormalMesh->mInternalResources.mpStorage[i].GetTypeDesc();
					void* resource = mpNormalMesh->mInternalResources.mpStorage[i].GetHandleObjectPointer();
					std::string* str = SymMap_ResolveHash(resolve_buf, material_name_db, filenamedb, name.GetCRC(), prop_key);
					if (str == nullptr)
						str = SymMap_ResolveHash(resolve_buf, material_name_db, filenamedb, name.GetCRC(), file_name);
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					const char* prefix;
					std::string ps{};
					if (str) {
						prefix = str->c_str();
					}
					else {
						ps = SymToString(&name);
						prefix = ps.c_str();
					}
					bool node_open = ImGui::TreeNode(prefix);
					ImGui::TableSetColumnIndex(1);
					if (resourceType->mHash == 0xCD75DC4F6B9F15D2) {//propertyset hash
						ImGui::PushID(i);
						if (ImGui::Button("Open Prop Resource")) {
							if (open_index != -1) {
								MessageBoxA(0, "You already have a D3DMesh resource open, you can only edit one a time!", "Could not open", MB_ICONERROR);
							}
							else {
								open_index = i;
								PropertySet* set = (PropertySet*)resource;
								std::string nm = str ? *str : ("Mesh Resource " + SymToString(&name));
								PropTask* task = new PropTask(nm.c_str(), sRuntime.gen_id());
								sRuntime.queued.push_back(task);
								if (sRuntime.gates.size() == 0)
									sRuntime.gates.reserve(4096);
								sRuntime.gates.push_back(1);
								open_gate = (bool*)(sRuntime.gates.data() + sRuntime.gates.size() - 1);
								task->set_prop(std::move(nm), open_gate, id, & MeshPropDelegate, set, this);
							}
						}
						ImGui::PopID();
					}
					else {
						ImGui::Text("Type:%s", resourceType->mpTypeInfoName);
					}
					if (node_open)
						ImGui::TreePop();
				}
				ImGui::EndTable();
			}
			if (ImGui::ListBoxHeader("Mesh Textures")) {
				for (auto& tex : texture_names) {
					ImGui::Selectable(tex.c_str());
				}
				ImGui::ListBoxFooter();
			}
			if (ImGui::ListBoxHeader("Mesh Parent Materials")) {
				for (auto& tex : norm_materials) {
					ImGui::Selectable(tex.c_str());
				}
				ImGui::ListBoxFooter();
			}
			if (ImGui::ListBoxHeader("Unresolved Mesh Textures")) {
				for (auto& tex : unk_textures) {
					ImGui::Selectable(tex.c_str());
				}
				ImGui::ListBoxFooter();
			}
		}
	}

	~MeshTask() {
		if (mpNormalMesh)
			delete mpNormalMesh;
		if (mpLegacyMesh)
			delete mpLegacyMesh;
		if (open_gate)
			*open_gate = false;
		open_gate = 0;
	}

public:

	deleteme(MeshTask)

	taskctor(MeshTask) {
		mpLegacyMesh = 0;
		mpNormalMesh = 0;
	}

};

/*class Task : public InspectorTask {

	virtual void _render() override {

	}

public:

	deleteme(Task)

	taskctor(Task){}

};*/

static void MeshPropDelegate(PropertySet& set, void* userdata) {
	MeshTask* task = (MeshTask*)userdata;
	task->open_index = -1;
}//no save changes needed

bool agent_sorter(Scene::AgentInfo*& lhs, Scene::AgentInfo*& rhs) {
	std::less<std::string> strl{};
	return strl(lhs->mAgentName, rhs->mAgentName);
}

class SceneTask : public InspectorTask {

	friend void ScenePropDelegate(PropertySet& set, void* userdata);

	MetaStream mStream;
	Scene mScene;
	const char* id = nullptr;
	bool state = false;

	int open_index = -1;
	bool* open_gate = 0;

	std::string newAgentName = "";

	void sort_agents(){
		mScene.mAgentList.sort(&agent_sorter);
	}

	virtual void _render() override {
		if (id == nullptr) {
			id = select_gameid_dropdown(id);
		}else{
			if (state) {
				ImGui::Text("Hidden");
				ImGui::SameLine();
				ImGui::Checkbox("##", &mScene.mbHidden);
				ImGui::Text("Name");
				ImGui::SameLine();
				ImGui::InputText("##", &mScene.mName);
				if(ImGui::Button("Save")){
					nfdchar_t* fp = 0;
					if (NFD_SaveDialog("scene", 0, &fp) == NFD_OKAY) {
						DataStreamFile_Win stream = _OpenDataStreamFromDisc_(fp, WRITE);
						free(fp);
						mStream.Open(&stream, MetaStreamMode::eMetaStream_Write, {});
						mStream.mbDontDeleteStream = true;
						if (PerformMetaSerializeAsync(&mStream, &mScene) != eMetaOp_Succeed) {
							MessageBoxA(0, "Could not write scene file, please contact me!", "Error writing file", MB_ICONERROR);
						}
						else {
							MessageBoxA(0, "Successfully saved scene file!", "Saved", MB_ICONINFORMATION);
						}
					}
				}
				if (ImGui::Button("Create New Agent")) {
					if (open_index == -1)
						ImGui::OpenPopup("Create Agent");
					else
						MessageBoxA(0, "Cannot create a new agent while you have an agent open currently", "Cannot open", MB_ICONERROR);
				}
				if (ImGui::BeginPopupModal("Create Agent")) {
					ImGui::Text("Agent Name");
					ImGui::InputText("##", &newAgentName);
					if(ImGui::Button("Create")){
						if(newAgentName == "" || mScene.GetAgent(newAgentName.c_str())){
							MessageBoxA(0, "That agent already exists or you have not entered a name!", "!!", MB_ICONERROR);
						}
						else {
							ImGui::CloseCurrentPopup();
							newAgentName = "";
						}
						mScene.CreateAgent(std::move(newAgentName));
						sort_agents();
					}
					if(ImGui::Button("Exit")){
						ImGui::CloseCurrentPopup();
						newAgentName = "";
					}
					ImGui::EndPopup();
				}
				ImGui::Text("To delete an agent, hover over its name and use Ctrl+D");
				ImGui::Text("Agents");
				if (ImGui::BeginTable("agentstab", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable)) {
					int index = 0;
					for(auto it = mScene.mAgentList.begin(); it != mScene.mAgentList.end(); it++){
						Scene::AgentInfo* pAgent = *it;
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::AlignTextToFramePadding();
						
						bool node_open = ImGui::TreeNode(pAgent->mAgentName.c_str());
						ImGui::TableSetColumnIndex(1);
						if (ImGui::IsItemHovered() && ImGui::IsKeyReleased(ImGuiKey_D) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
							if (open_index == -1) {
								mScene.mAgentList.remove(*it);
								break;
							}else{
								MessageBoxA(0, "Cannot delete an agent while you have an agent open currently", "Cannot delete", MB_ICONERROR);
							}
						}
						ImGui::PushID(index);
						if (ImGui::Button("Open Agent Props")) {
							if (open_index != -1) {
								MessageBoxA(0, "You already have a scene agent open, you can only edit one a time!", "Could not open", MB_ICONERROR);
							}
							else {
								open_index = index;
								PropertySet* set = (PropertySet*)&pAgent->mAgentSceneProps;
								std::string& nm = pAgent->mAgentName;
								PropTask* task = new PropTask(nm.c_str(), sRuntime.gen_id());
								sRuntime.queued.push_back(task);
								if (sRuntime.gates.size() == 0)
									sRuntime.gates.reserve(4096);
								sRuntime.gates.push_back(1);
								open_gate = (bool*)(sRuntime.gates.data() + sRuntime.gates.size() - 1);
								task->set_prop(std::string(nm), open_gate, id, &ScenePropDelegate, set, this);
							}
						}
						ImGui::PopID();
						if (node_open)
							ImGui::TreePop();
						index++;
					}
					ImGui::EndTable();
				}
			}else{
				ImGui::Text("Click NEW to create a new scene and OPEN to open an existing scene to edit");
				if(ImGui::Button("New")){
					state = true;
					mScene.mbHidden = false;
					mScene.mName = "adv_myNewScene.scene";
				}
				if(ImGui::Button("Open")){
					nfdchar_t* fp{};
					if(NFD_OpenDialog("scene",0,&fp) == NFD_OKAY){
						DataStreamFile_Win stream = _OpenDataStreamFromDisc_(fp, READ);
						free(fp);
						mStream.Open(&stream, MetaStreamMode::eMetaStream_Read, {});
						mStream.mbDontDeleteStream = true;
						if(PerformMetaSerializeAsync(&mStream, &mScene) != eMetaOp_Succeed){
							MessageBoxA(0, "Could not read scene file, please contact me!", "Error reading file", MB_ICONERROR);
						}else{
							state = true;
							sort_agents();
						}
					}
				}
			}
		}
	}

public:

	deleteme(SceneTask)

	taskctor(SceneTask) {}

};

static void ScenePropDelegate(PropertySet& set, void* userdata) {
	SceneTask* task = (SceneTask*)userdata;
	task->open_index = -1;
}//no save changes needed

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
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

	static bool tryingopen = false;
	Walnut::ApplicationSpecification spec;
	spec.Name = "Telltale Inspector " APP_VERSION;

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<TelltaleInspector>();
	app->SetMenubarCallback([app]()
	{
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
								printf("-unknown file type %s IGNORING\n", path);
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
		if (ImGui::BeginMenu("File"))
		{
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
			if (false && ImGui::MenuItem("Open Full Game Editor")) {//  TODO EVENTUALLY ADD SUPPORT FOR THIS WHEN GET TO IT.
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
			ImGui::EndMenu();
		}
	});
	return app;
}