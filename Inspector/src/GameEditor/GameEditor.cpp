#include "GameEditor.hpp"
#include "imgui.h"
#include "ToolLibrary/Types/PreloadPackage.h"
#include "ToolLibrary/T3/T3EffectCache.h"
#include "ToolLibrary/T3/T3Effect.h"

static void dumpstack(lua_State* L) {
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {
		TTL_Log("%d\t%s\t", i, luaL_typename(L, i));
		switch (lua_type(L, i)) {
		case LUA_TNUMBER:
			TTL_Log("%g\n", lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			TTL_Log("%s\n", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			TTL_Log("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
			break;
		case LUA_TNIL:
			TTL_Log("%s\n", "nil");
			break;
		default:
			TTL_Log("%p\n", lua_topointer(L, i));
			break;
		}
	}
}

/*
* local set = {}
set.name = "JesseMale204"
set.setName = "JesseMale204"
set.descriptionFilenameOverride = ""
set.logicalName = "<JesseMale204>"
set.logicalDestination = "<Minecraft204>"
set.priority = 233
set.localDir = _currentDirectory
set.enableMode = "localization"
set.version = "trunk"
set.descriptionPriority = 0
set.gameDataName = "JesseMale204 Game Data"
set.gameDataPriority = 0
set.gameDataEnableMode = "constant"
set.localDirIncludeBase = true
set.localDirRecurse = false
set.localDirIncludeOnly = nil
set.localDirExclude =
{
	"_dev/"
}
set.gameDataArchives =
{
	_currentDirectory .. "MC2_pc_JesseMale204_compressed.ttarch2",
	_currentDirectory .. "MC2_pc_JesseMale204_uncompressed.ttarch2"
}
RegisterSetDescription(set)*/

struct LessString_FirstChar {

	bool operator()(const std::string& lhs, const std::string& rhs){
		return lhs.size() > 3 && rhs.size() > 3 ? lhs.c_str()[0] < rhs.c_str()[0] ? true : false : false;
	}

};

void GameEmbed_Callback(InspectorTask* pTask, void* user) {
	GameEditorTask* task = (GameEditorTask*)user;
	if (!task)
		return;
	for (auto it = task->openTask.begin(); it != task->openTask.end(); it++) {
		if ((*it) = pTask) {
			task->openTask.erase(it);
			break;
		}
	}
}

void GameEditorTask::PreviewFile(std::string fileName, DataStream* pStream){
	u64 crc = CRC64_CaseInsensitive(0, fileName.c_str());
	for (auto it = openTask.begin(); it != openTask.end(); it++) {
		if ((u64)(*it)->mpParentUserData == crc) {
			MessageBoxA(0, "You already have a file with the same name open!", "Cannot open file", MB_ICONERROR);
			delete pStream;
			return;
		}
	}
	if (fileName.find('.') != std::string::npos) {
		std::string resext = std::filesystem::path(fileName).extension().string().substr(1);
		MetaClassDescription* mcd = TelltaleToolLib_FindMetaClassDescription(resext.c_str(), false);
		if (mcd == 0 || resext == "wav") {
			if (resext == "bank" || resext == "wav") {
				//InspectorTask* task = new TypeTask<Chore>(parentName, embedName, sRuntime.gen_id(), gameID, (Chore*)typeInstance);
				BankTask* task = new BankTask(fileName.c_str(), sRuntime.gen_id());
				task->mpParentUserData = (void*)CRC64_CaseInsensitive(0, fileName.c_str());
				task->SetDestroyCallback(GameEmbed_Callback, this, aliveSlot);
				sRuntime.open_tasks.push_back(task);
				openTask.push_back(task);
				std::string err{};
				if(!task->mBank.Open(pStream, err)){
					MessageBoxA(0, "The given file was not an FMOD sound bank but may actually be WAV data, or the file could not be read.", "Error", MB_ICONINFORMATION);
				}
			}else if(resext == "lenc" || resext == "lua"){
				LuaTask* task = new LuaTask(fileName.c_str(), sRuntime.gen_id());
				task->mpParentUserData = (void*)CRC64_CaseInsensitive(0, fileName.c_str());
				task->SetDestroyCallback(GameEmbed_Callback, this, aliveSlot);
				sRuntime.open_tasks.push_back(task);
				openTask.push_back(task);
				task->gameid = mpGameID;
				imported_file lua{};
				lua.mbIsMemory = true;
				lua.mName = fileName;
				lua.mMemoryStream = new DataStreamMemory(pStream->GetSize());
				task->mFiles.push_back(std::move(lua));
				pStream->Copy(lua.mMemoryStream, 0, 0, pStream->GetSize());
				delete pStream;
			}else if(resext == "t3fxpack"){
				ShaderTask* task = new ShaderTask(fileName.c_str(), sRuntime.gen_id());
				task->mpParentUserData = (void*)CRC64_CaseInsensitive(0, fileName.c_str());
				task->SetDestroyCallback(GameEmbed_Callback, this, aliveSlot);
				sRuntime.open_tasks.push_back(task);
				openTask.push_back(task);
				task->name = std::move(fileName);
				task->SetStream(mpGameID, pStream);//ownership by datastreamcontainer
			}
			else {
				MessageBoxA(0, "The file given type extension does not exist in Meta system for TelltaleInspector! Please open a different file or contact me...", fileName.c_str(), 0);
				delete pStream;
			}
		}
		else {
			void* pInst = mcd->New();
			if (pInst == 0) {
				MessageBoxA(0, "Could not allocate type", fileName.c_str(), 0);
				delete pStream;
			}
			else {
				{
					MetaStream stream{};
					stream.Open(pStream, MetaStreamMode::eMetaStream_Read, {});
					if (PerformMetaSerializeFull(&stream, pInst, mcd) != eMetaOp_Succeed) {
						MessageBoxA(0, "That file could not be opened! It likely is unsupported so please contact me with the file information.", fileName.c_str(), 0);
						return;//next frame...
					}
				}
				//delete pStream;, stream deletes it here
			}
			InspectorTask* task = 0;
			openTask.push_back(task=CreateAndOpenTypeTask(fileName.c_str(), mcd, "Game Editor", mpGameID, pInst, GameEmbed_Callback, this, aliveSlot));
			task->mpParentUserData = (void*)CRC64_CaseInsensitive(0, fileName.c_str());
		}
	}
	else {
		MessageBoxA(0, "Can't open this one :( (where's it's extension!)", fileName.c_str(), 0);
		delete pStream;
	}
}

void GameEditorTask::SaveResDesc(GameEditor_DiskResourceSet& set, bool enc)
{
	sSetKeyIndex = TelltaleToolLib_GetGameKeyIndex(mpGameID);
	std::ofstream outf{this->game_data / set.mGameDataPathPost.substr(1)};
	std::stringstream out{};
	out << "-- Exported from Telltale Inspector. Does not need to be encrypted!\n";
	out << "local set = {}\n";
	out << "set.name = \"" << set.mResourceSetDesc.mName << "\"\n";
	out << "set.setName = \"" << set.mResourceSetDesc.mSetName << "\"\n";
	out << "set.descriptionFilenameOverride = \"" << set.mResourceSetDesc.mDescOverride << "\"\n";
	out << "set.logicalName = \"" << set.mResourceSetDesc.mLogicalName << "\"\n";
	out << "set.logicalDestination = \"" << set.mResourceSetDesc.mLogicalDest << "\"\n";
	out << "set.priority = " << set.mResourceSetDesc.mPriority << "\n";
	out << "set.localDir = _currentDirectory\n";
	out << "set.enableMode = \"" << set.mResourceSetDesc.mEnableMode << "\"\n";
	out << "set.version = \"" << set.mResourceSetDesc.mVersion << "\"\n";
	out << "set.descriptionPriority = " << set.mResourceSetDesc.mDescPriority << "\n";
	out << "set.gameDataName = \"" << set.mResourceSetDesc.mGameDataName << "\"\n";
	out << "set.gameDataPriority = " << set.mResourceSetDesc.mGameDataPriority << "\n";
	out << "set.gameDataEnableMode = \"" << set.mResourceSetDesc.mGameDataEnableMode << "\"\n";
	out << "set.localDirIncludeBase = " << (set.mResourceSetDesc.mLocalDirIncludeBase?"true":"false") << "\n";
	out << "set.localDirRecurse = " << (set.mResourceSetDesc.mLocalDirRecurse ? "true" : "false") << "\n";
	//incl only
	if(set.mResourceSetDesc.mLocalDirIncludeOnly.size() == 0)
		out << "set.localDirIncludeOnly = nil\n";
	else{
		out << "set.localDirIncludeOnly = \n{\n";
		int i = 0;
		for(auto& it : set.mResourceSetDesc.mLocalDirIncludeOnly){
			out << "    \"" << it << "\"" << (i == set.mResourceSetDesc.mLocalDirIncludeOnly.size() - 1 ? "\n" : ",\n");
			i++;
		}
		out << "}\n";
	}
	//excl
	if (set.mResourceSetDesc.mLocalDirExclude.size() == 0)
		out << "set.localDirExclude = nil\n";
	else {
		out << "set.localDirExclude = \n{\n";
		int i = 0;
		for (auto& it : set.mResourceSetDesc.mLocalDirExclude) {
			out << "    \"" << it << "\"" << (i == set.mResourceSetDesc.mLocalDirExclude.size() - 1 ? "\n" : ",\n");
			i++;
		}
		out << "}\n";
	}
	//game data
	if (set.mResourceSetDesc.mGameDataArchives.size() == 0)
		out << "set.gameDataArchives = nil\n";
	else {
		out << "set.gameDataArchives = \n{\n";
		int i = 0;
		for (auto& it : set.mResourceSetDesc.mGameDataArchives) {
			out << "    _currentDirectory .. \"" << it << "\"" << (i == set.mResourceSetDesc.mGameDataArchives.size() - 1 ? "\n" : ",\n");
			i++;
		}
		out << "}\n";
	}
	out << "RegisterSetDescription(set)\n";
	std::string as = out.str();
	u32 outz{};
	u8* res = 0;
	if (ends_with(set.mGameDataPathPost, ".lenc"))
		res=TelltaleToolLib_EncryptLencScript((u8*)as.c_str(), (u32)as.length(), &outz);
	else if(enc)
		res=TelltaleToolLib_EncryptScript((u8*)as.c_str(), (u32)as.length(), &outz);
	if(res){
		outf.write((const char*)res, outz);
		free(res);
		res = 0;
	}else{
		outf << as;
	}
}

void EditVecString(std::vector<std::string>& list, const char* label)
{
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::Text(" (Hover me LShift+N to add, hover over element LShift+D to delete)");
	if (ImGui::IsItemHovered() && ImGui::IsKeyReleased(ImGuiKey_N) && ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
		list.push_back("");
	}
	std::string tmp = (std::string("##") + std::string(label));
	if (ImGui::BeginTable(tmp.c_str(), 1, ImGuiTableFlags_Borders)) {
		int i = 0;
		for (auto& it : list) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::PushID(i);
			ImGui::InputText("##", &it);
			ImGui::PopID();
			if (ImGui::IsItemHovered() && ImGui::IsKeyReleased(ImGuiKey_D) && ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
				auto it2 = list.cbegin();
				std::advance(it2, i);
				list.erase(it2);
				break;
			}
			i++;
		}
		ImGui::EndTable();
	}
}

static int luaRegisterSet(lua_State* L) {
	GameEditorTask* cur = sRuntime.current_editor_task;
	if (cur) {
		GameEditor_DiskResourceSet* set = (GameEditor_DiskResourceSet*)cur->mpLuaArg;

		//name
		lua_getfield(L, -1, "name");
		set->mResourceSetDesc.mName = lua_tolstring(L, -1, 0);
		lua_pop(L, 1);

		//logical name
		lua_getfield(L, -1, "setName");
		set->mResourceSetDesc.mSetName = lua_tolstring(L, -1, 0);
		lua_pop(L, 1);

		lua_getfield(L, -1, "descriptionFilenameOverride");
		set->mResourceSetDesc.mDescOverride = lua_tolstring(L, -1, 0);
		lua_pop(L, 1);

		lua_getfield(L, -1, "logicalName");
		set->mResourceSetDesc.mLogicalName = lua_tolstring(L, -1, 0);
		lua_pop(L, 1);

		lua_getfield(L, -1, "logicalDestination");
		set->mResourceSetDesc.mLogicalDest = lua_tolstring(L, -1, 0);
		lua_pop(L, 1);

		lua_getfield(L, -1, "priority");
		set->mResourceSetDesc.mPriority = lua_tointeger(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "enableMode");
		set->mResourceSetDesc.mEnableMode = lua_tolstring(L, -1, 0);
		lua_pop(L, 1);

		lua_getfield(L, -1, "version");
		set->mResourceSetDesc.mVersion = lua_tolstring(L, -1, 0);
		lua_pop(L, 1);

		lua_getfield(L, -1, "gameDataName");
		set->mResourceSetDesc.mGameDataName = lua_tolstring(L, -1, 0);
		lua_pop(L, 1);

		lua_getfield(L, -1, "gameDataPriority");
		set->mResourceSetDesc.mGameDataPriority = lua_tointegerx(L, -1, 0);
		lua_pop(L, 1);

		lua_getfield(L, -1, "descriptionPriority");
		set->mResourceSetDesc.mDescPriority = lua_tointegerx(L, -1, 0);
		lua_pop(L, 1);

		lua_getfield(L, -1, "gameDataEnableMode");
		set->mResourceSetDesc.mGameDataEnableMode = lua_tolstring(L, -1, 0);
		lua_pop(L, 1);

		lua_getfield(L, -1, "localDirIncludeBase");
		set->mResourceSetDesc.mLocalDirIncludeBase = lua_toboolean(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "localDirRecurse");
		set->mResourceSetDesc.mLocalDirRecurse = lua_toboolean(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "localDirIncludeOnly");
		if(lua_type(L, -1) == LUA_TTABLE){
			int n = lua_rawlen(L, -1);
			set->mResourceSetDesc.mLocalDirIncludeOnly.clear();
			set->mResourceSetDesc.mLocalDirIncludeOnly.reserve(n);
			for (int i = 0; i < n; i++) {
				lua_rawgeti(L, -1, i + 1);
				const char* val = lua_tolstring(L, -1, 0);
				if (!val) {
					TTL_Log("WARN: could not get array[n] from resdesc\n");
				}
				else if (*val != 0)
					set->mResourceSetDesc.mLocalDirIncludeOnly.push_back(val);
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 1);

		lua_getfield(L, -1, "localDirExclude");
		if (lua_type(L, -1) == LUA_TTABLE) {
			int n = lua_rawlen(L, -1);
			set->mResourceSetDesc.mLocalDirExclude.clear();
			set->mResourceSetDesc.mLocalDirExclude.reserve(n);
			for (int i = 0; i < n; i++) {
				lua_rawgeti(L, -1, i + 1);
				const char* val = lua_tolstring(L, -1, 0);
				if (!val) {
					TTL_Log("WARN: could not get array[n] from resdesc\n");
				}
				else if(*val != 0)
					set->mResourceSetDesc.mLocalDirExclude.push_back(val);
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 1);

		/*game data arcs*/
		lua_getfield(L, -1, "gameDataArchives");
		if (lua_type(L, -1) == LUA_TTABLE) {
			int n = lua_rawlen(L, -1);
			set->mResourceSetDesc.mGameDataArchives.clear();
			set->mResourceSetDesc.mGameDataArchives.reserve(n);
			for (int i = 0; i < n; i++) {
				lua_rawgeti(L, -1, i + 1);
				const char* val = lua_tolstring(L, -1, 0);
				if (!val) {
					TTL_Log("WARN: could not get array[n] from resdesc\n");
				}
				else if (*val != 0)
					set->mResourceSetDesc.mGameDataArchives.push_back(val);
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 1);

	}
	return lua_gettop(L);
}

int luaAddBuildVersionInfo(lua_State* L) {
	GameEditorTask* cur = sRuntime.current_editor_task;
	if (cur) {
		const char* versionInf = lua_tolstring(L, 1, 0);
		cur->mVersionStrs.push_back(versionInf);
	}
	return lua_gettop(L);
}

void GameEditorTask::_init_diskres(GameEditor_DiskResourceSet& set) {
	if (set.mType == GameEditor_DiskResourceSet::RESOURCE_SET_DESCRIPTION) {
		std::string f = std::string{ game_data.string() + set.mGameDataPathPost };
		DataStreamFile_PlatformSpecific stream = _OpenDataStreamFromDisc_(f.c_str(), READ);
		TempBuffer tmp{ this, (u32)stream.GetSize() +1};
		memset(working_buffer, 0, sizeof(working_buffer));
		stream.Serialize(tmp.GetBuffer(), stream.GetSize());
		tmp.GetBuffer()[stream.GetSize()] = 0;
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
			tmp.GetBuffer()[o] = 0;
			free(dec);
			ensure_reg("RegisterSetDescription", luaRegisterSet);
		}
		mpLuaArg = &set;
		lua_pushstring(sRuntime.L, "");
		lua_setglobal(sRuntime.L, "_currentDirectory");//always "" because in actual game currentDirectory is relative
		luaL_dostring(sRuntime.L, tmp.GetBuffer());
		if (bRemove)
			mResourceSets.pop_back();
	}
	else if (set.mType == GameEditor_DiskResourceSet::SINGLE_FILE) {

	}else if(set.mType == GameEditor_DiskResourceSet::DATA_ARCHIVE){
		//ttarch2/ttarch
	}
}

void GameEditorTask::_init_load_resource_sets(std::string current_folder, std::string current_app, bool bFF) {
	for (auto& file : std::filesystem::directory_iterator{ current_folder }) {
		std::string name = file.path().filename().string();
		if (file.is_regular_file()&&!bFF) {
			bool barc = ends_with(name, ".ttarch") || ends_with(name, ".ttarch2");
			if(barc){
				bool f = 0;
				//ensure we dont load user ones
				for(int x = 0; x < mResourceSets.size(); x++){
					if(mResourceSets[x].mType == GameEditor_DiskResourceSet::USER_ARCHIVE && iequals(mResourceSets[x].mName, name)){
						f = 1;
						break;
					}
				}
				if (f)
					continue;
			}
			GameEditor_DiskResourceSet& set = mResourceSets.emplace_back();
			set.mName = name;
			set.mGameDataPathPost = current_app + "/" + name;
			if (barc)
				set.mType = GameEditor_DiskResourceSet::DATA_ARCHIVE;
			else if (ends_with(name, ".lua") || ends_with(name, ".lenc"))
				set.mType = GameEditor_DiskResourceSet::RESOURCE_SET_DESCRIPTION;
			else
				set.mType = GameEditor_DiskResourceSet::SINGLE_FILE;
			_init_diskres(set);
		}
		else if (file.is_directory()&&bFF) {
			if(name._Starts_with("_TTI_Arc__")){
				GameEditor_DiskResourceSet& set = mResourceSets.emplace_back();
				set.mType = GameEditor_DiskResourceSet::USER_ARCHIVE;
				set.mGameDataPathPost = current_app + "/" + name;
				set.mName = name.substr(10);
				_init_diskres(set);
			}else _init_load_resource_sets(file.path().string(), current_app + "/" + file.path().filename().string(), bFF);
		}
	}
}

void GameEditorTask::_render() {
	sceneLister.frame_check_addrem = true;
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
			game_data = base_path.string() + std::string{ mGameKey.game_data_location };
			if (!std::filesystem::exists(base_path)) {
				mError = "Could not locate game data: " + game_data.string();
				return;
			}
			static constexpr const char* MIN_GAME = "MCSM";
			int MIN = TelltaleToolLib_GetGameKeyIndex(MIN_GAME);
			if (mGameKeyIndex >= MIN) {
				//LOAD RESDESCS
				_init_load_resource_sets(game_data.string(), "", true);
				_init_load_resource_sets(game_data.string(), "", false);
			}
			else {
				MessageBoxA(0, "That game selected is not supported. You game must be newer than or including the game in the caption of this message box.", sBlowfishKeys[MIN].game_name,MB_ICONERROR);
				mark_as_todelete();
			}
			mbSortFilename = MessageBoxA(0, "Would you like to sort all file names in the archive viewer? Selecting yes may take some time. If not, you can always filter by name or type.", "Sort files?", MB_YESNO) == IDYES;
			//mpWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "Scene View");
			sentArchives = 0;
			for (auto& it : mResourceSets) {
				if (it.mType == GameEditor_DiskResourceSet::DATA_ARCHIVE) {
					if (ends_with(it.mName, ".ttarch2")) {
						it.mArchive2 = new TTArchive2;
						it.mArchive2->userData = (void*)mpGameID;
						sentArchives++;
						Job j{};
						j.mpFn = &AsyncLoadArchive2;
						j.userA = &it;
						j.userB = _OpenDataStreamFromDisc((std::filesystem::path{ game_data } / it.mGameDataPathPost.substr(1)).string().c_str(), READ);
						j.mpUserInstance = this;
						j.name = "Async load Archive2 " + it.mName;
						JobScheduler::Scheduler().PostJob(this, std::move(j));
					}
				}
			}
		}
		TelltaleToolLib_SetBlowfishKey(mpGameID);
		if (mbInit) {
			RenderUI();
			RenderScene();
		}
	}
}

void GameEditorTask::RenderResDescs(){
	if (resDescIndex != -1) {
		ImGui::OpenPopup("ResourceDesc");
		if (ImGui::BeginPopupModal("ResourceDesc")) {
			mResourceSets[resDescIndex].mbEdited = 1;
			ImGui::Text("Editing: %s", mResourceSets[resDescIndex].mName.c_str());
			ImGui::Text("Name:");
			ImGui::SameLine();
			ImGui::InputText("##1", &mResourceSets[resDescIndex].mResourceSetDesc.mName);
			ImGui::Text("Set Name:");
			ImGui::SameLine();
			ImGui::InputText("##2", &mResourceSets[resDescIndex].mResourceSetDesc.mSetName);
			ImGui::Text("Logical Name:");
			ImGui::SameLine();
			ImGui::InputText("##3", &mResourceSets[resDescIndex].mResourceSetDesc.mLogicalName);
			ImGui::Text("Logical Destination:");
			ImGui::SameLine();
			ImGui::InputText("##4", &mResourceSets[resDescIndex].mResourceSetDesc.mLogicalDest);
			ImGui::Text("Priority:");
			ImGui::SameLine();
			ImGui::InputInt("##99", &mResourceSets[resDescIndex].mResourceSetDesc.mPriority);
			ImGui::Text("Version:");
			ImGui::SameLine();
			ImGui::InputText("##5", &mResourceSets[resDescIndex].mResourceSetDesc.mVersion);
			ImGui::Text("Description Filename Override:");
			ImGui::SameLine();
			ImGui::InputText("##6", &mResourceSets[resDescIndex].mResourceSetDesc.mDescOverride);
			ImGui::Text("Description Priority:");
			ImGui::SameLine();
			ImGui::InputInt("##61", &mResourceSets[resDescIndex].mResourceSetDesc.mDescPriority);
			ImGui::Text("Enable Mode:");
			ImGui::SameLine();
			ImGui::InputText("##7", &mResourceSets[resDescIndex].mResourceSetDesc.mEnableMode);
			ImGui::Text("Game Data Name:");
			ImGui::SameLine();
			ImGui::InputText("##8", &mResourceSets[resDescIndex].mResourceSetDesc.mGameDataName);
			ImGui::Text("Game Data Enable Mode:");
			ImGui::SameLine();
			ImGui::InputText("##8", &mResourceSets[resDescIndex].mResourceSetDesc.mGameDataEnableMode);
			ImGui::Text("Game Data Priority:");
			ImGui::SameLine();
			ImGui::InputInt("##9", &mResourceSets[resDescIndex].mResourceSetDesc.mGameDataPriority);
			ImGui::Text("Local Dir - Include Base:");
			ImGui::SameLine();
			ImGui::Checkbox("##13", &mResourceSets[resDescIndex].mResourceSetDesc.mLocalDirIncludeBase);
			ImGui::Text("Local Dir - Recursive Search");
			ImGui::SameLine();
			ImGui::Checkbox("##12", &mResourceSets[resDescIndex].mResourceSetDesc.mLocalDirRecurse);
			EditVecString(mResourceSets[resDescIndex].mResourceSetDesc.mLocalDirIncludeOnly, "Local Dir - Include Only");
			EditVecString(mResourceSets[resDescIndex].mResourceSetDesc.mLocalDirExclude, "Local Dir - Exclude");
			EditVecString(mResourceSets[resDescIndex].mResourceSetDesc.mGameDataArchives, "Game Data Archives");
			if (ImGui::Button("Close")) {
				resDescIndex = -1;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	ImGui::Text("Game Build Version Info");
	if (ImGui::BeginTable("##table1", 1, ImGuiTableFlags_Borders)) {
		for (auto& it : mVersionStrs) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(it.c_str());
		}
		ImGui::EndTable();
	}
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.f);
	ImGui::Text("Game Resource Sets. New:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(400.f);
	ImGui::InputText("##aaa", &mInputRD);
	ImGui::SameLine();
	if (ImGui::Button("Create") && (ends_with(mInputRD, ".lua") || ends_with(mInputRD, ".lenc")) && mInputRD._Starts_with("_resdesc_")) {
		bool ok = 1;
		for (auto& it : mResourceSets) {
			if (it.mType == GameEditor_DiskResourceSet::RESOURCE_SET_DESCRIPTION && iequals(mInputRD, it.mName)) {
				ok = 0;
				break;
			}
		}
		if (ok) {
			GameEditor_DiskResourceSet set{};
			set.mName = std::move(mInputRD);
			mInputRD = "_resdesc_50_XXX.lua";
			set.mType = GameEditor_DiskResourceSet::RESOURCE_SET_DESCRIPTION;
			set.mGameDataPathPost = "/" + set.mName;
			set.mResourceSetDesc.mName = set.mName.substr(12,set.mName.length() - 4);
			set.mResourceSetDesc.mSetName = set.mResourceSetDesc.mName;
			set.mResourceSetDesc.mPriority = 1;
			set.mResourceSetDesc.mLogicalDest = "<>";
			set.mResourceSetDesc.mLogicalName = "<" + set.mResourceSetDesc.mName + ">";
			if (!set.IsUser())
				sRuntime.settings.mUserResourceSets.AddElement(0, 0, &set.mName);
			mResourceSets.push_back(std::move(set));
			MessageBoxA(0, "Created! Ensure to save it once edited.", "Create success", MB_ICONINFORMATION);
		}else{
			MessageBoxA(0, "That already exists!", "Exists", MB_ICONERROR);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Save All Encrypted")) {
		for(auto& it : mResourceSets){
			if (it.mbEdited)
				SaveResDesc(it, 1);
			it.mbEdited = 0;
		}
		MessageBoxA(0, "All modified files where saved!", "Success", MB_ICONINFORMATION);
	}
	ImGui::SameLine();
	if (ImGui::Button("Save All Unencrypted")) {
		for (auto& it : mResourceSets) {
			if (it.mbEdited)
				SaveResDesc(it, 0);
			it.mbEdited = 0;
		}
		MessageBoxA(0, "All modified files where saved!", "Success", MB_ICONINFORMATION);
	}
	if (ImGui::BeginTable("##table2", 2, ImGuiTableFlags_Borders)) {
		int i = 0;
		for (auto& it : mResourceSets) {
			if (it.mType == GameEditor_DiskResourceSet::RESOURCE_SET_DESCRIPTION) {
				ImGui::TableNextRow();
				ImGui::PushID(i);
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("[%s] %s", it.IsUser()?"BY YOU":"GAME",it.mGameDataPathPost.substr(1).c_str());
				ImGui::TableSetColumnIndex(1);
				if (ImGui::Button("Edit Set Description")) {
					resDescIndex = i;
				}
				ImGui::SameLine();
				if(ImGui::Button("Save Unencrypted")){
					SaveResDesc(it, false);
					MessageBoxA(0, "Saved!", "Saved", MB_ICONINFORMATION);
				}
				ImGui::SameLine();
				if (ImGui::Button("Save Encrypted")) {
					SaveResDesc(it, true);
					MessageBoxA(0, "Saved!", "Saved", MB_ICONINFORMATION);
				}
				ImGui::PopID();
			}
			i++;
		}
		ImGui::EndTable();
	}
}

void GameEditorTask::AsyncLoadArchive2(void* _task, void* a, void* b){
	GameEditorTask* task = (GameEditorTask*)_task;
	GameEditor_DiskResourceSet* set = (GameEditor_DiskResourceSet*)a;
	TTArchive2* archive = set->mArchive2;
	TelltaleToolLib_SetBlowfishKey((const char*)archive->userData);
	archive->Activate((DataStream*)b);

	if (archive->mbActive) {
		set->filenames.reserve(archive->mResources.size());
		for (int i = 0; i < archive->mResources.size(); i++) {
			std::string name = archive->GetResourceName(archive->mResources[i].mNameCRC);
			set->filenames.push_back(std::move(name));
			if(task->mbSortFilename)
				std::sort(set->filenames.begin(), set->filenames.end());
			//else
			//	std::sort(set->filenames.begin(), set->filenames.end(), LessString_FirstChar{});
		}
		TTL_Log("-archive load success %s\n", set->mName.c_str());
	}else{
		TTL_Log("-failed to load %s\n", set->mName.c_str());
	}
	task->loadedArchives++;
}

thread_local GameEditorTask* pMakingTask = 0;

void GameEditorTask::AsyncMakeCb(const char* m, float){
	pMakingTask->progressMsg.store((uintptr_t)m);
}

void GameEditorTask::AsyncMakeArchive2(void* _task, void* a, void* b){
	GameEditorTask* task = (GameEditorTask*)_task;
	pMakingTask = task;
	
	// 
	std::filesystem::path folder = task->game_data / task->mResourceSets[task->selectedMakeArchiveIndex].mGameDataPathPost.substr(1);
	std::filesystem::path arc = task->game_data / task->mResourceSets[task->selectedMakeArchiveIndex].mName;
	std::vector<TTArchive2::ResourceCreateEntry> entries{};

	AsyncMakeCb("Gathering files", 0.f);
	
	for (auto& file : std::filesystem::directory_iterator{ folder }) {
		if(file.is_regular_file()){
			TTArchive2::ResourceCreateEntry e{};
			e.mpStream = 0;
			e.open_later = file.path().string();
			e.name = file.path().filename().string();
			entries.push_back(std::move(e));
		}
	}

	AsyncMakeCb("Creating archive", 0.f);
	
	bool f = false;
	DataStream* pStream = 0;
	if(!TTArchive2::Create(&AsyncMakeCb, pStream=_OpenDataStreamFromDisc(arc.string().c_str(), WRITE), entries, task->mbMakeEncrypt, task->mbMakeZlib, Compression::Library::ZLIB, 2)){
		AsyncMakeCb("Archive make fail!", 0.f);
		f = true;
		TTL_Log("ERROR: could not create and make archive!\n");
	}else{
		AsyncMakeCb("Archive creation success", 0.f);
	}
	delete pStream;
	if (f)
		DeleteFileA(arc.string().c_str());

	task->mpMakeArchiveWaiter->store(true);
	pMakingTask = 0;
}

void GameEditorTask::RenderGameData(){
	if (ImGui::BeginPopupModal("Find Symbol")) {
		ImGui::Text("Enter the symbol hash below (decimal)");
		ImGui::InputScalar("##if2", ImGuiDataType_U64, &inputHash, 0, 0);
		if (ImGui::Button("Find") && inputHash > 0xFFFF'FFFF) {
			bool done = false;
			for (auto& set : mResourceSets) {
				if(set.mType!=GameEditor_DiskResourceSet::DATA_ARCHIVE)
					continue;
				TTArchive2::ResourceEntry* entry= set.mArchive2->_FindResource(inputHash);
				if(entry){
					std::string n = set.mArchive2->GetResourceName(inputHash);
					MessageBoxA(0, n.c_str(), "Found (in this Game)!", MB_ICONINFORMATION);
					done = 1;
					break;
				}
			}
			if(!done){
				std::string d{};
				TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(0, inputHash, &d);
				if(d.length() == 0){
					MetaClassDescription* clazz = TelltaleToolLib_FindMetaClassDescription_ByHash(inputHash);
					if (clazz) {
						MessageBoxA(0, clazz->mpTypeInfoName, "Found (is a type hash)!", MB_ICONINFORMATION);
					}else{
						MessageBoxA(0, "Could not locate that hash anywhere in known databases or files!", "Not Found!", MB_ICONINFORMATION);
					}
				}else{
					MessageBoxA(0, d.c_str(), "Found (using Global DB)!", MB_ICONINFORMATION);
				}
			}
		}
		if (ImGui::Button("Close")) {
			inputHash = 0;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopupModal("Find File")) {
		ImGui::Text("Enter the file name below");
		ImGui::InputText("##if", &findFileField);
		ImGui::Text("Autofill file name (>4 characters)");
		if(findFileField.length() >= 5 && ImGui::BeginCombo("##combo",findFileField.c_str())){
			bool done = false;
			for(auto& set : mResourceSets){
				memcpy(working_buffer, set.mGameDataPathPost.c_str(), set.mGameDataPathPost.length());
				working_buffer[set.mGameDataPathPost.length()] = '/';
				for(auto& f : set.filenames){
					if(f._Starts_with(findFileField)){
						bool selected = false;
						memcpy(working_buffer + set.mGameDataPathPost.length() + 1, f.c_str(), f.length() + 1);
						ImGui::Selectable(working_buffer, &selected);
						if (selected) {
							findFileField = f;
							done = 1;
							break;
						}
					}
				}
				if (done)
					break;
			}
			ImGui::EndCombo();
		}
		if(ImGui::Button("Close")){
			findFileField = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopupModal("New Archive")) {
		ImGui::Text("Archive File:");
		ImGui::InputText("##if", &findFileField);
		ImGui::Text("This archive will be a folder on disk.");
		ImGui::Text("You can turn it into a .ttarch2 when you have the files.");
		ImGui::Text("Ensure you reference this from a resource description!");
		if (ImGui::Button("Create") && findFileField.size() > 0 && ends_with(findFileField,".ttarch2")) {
			bool ok = 1;
			for(int i = 0; i < mResourceSets.size(); i++){
				if(mResourceSets[i].mType == GameEditor_DiskResourceSet::DATA_ARCHIVE && iequals(mResourceSets[i].mName, findFileField)){
					MessageBoxA(0, "Already exists!", "Exists", MB_ICONERROR);
					ok = 0;
					break;
				}
			}
			if (ok) {
				std::filesystem::path p(game_data / (std::string("_TTI_Arc__") + findFileField));
				if (std::filesystem::exists(p)) {
					MessageBoxA(0, "An archive with that name already exists!", "Exists!", MB_ICONERROR);
				}
				else {
					std::filesystem::create_directory(p);
					GameEditor_DiskResourceSet set{};
					set.mName = findFileField;
					set.mType = GameEditor_DiskResourceSet::USER_ARCHIVE;
					set.mGameDataPathPost = (std::string("/_TTI_Arc__") + findFileField);
					mResourceSets.push_back(std::move(set));
					MessageBoxA(0, "Created archive!", "Success", MB_ICONINFORMATION);
					findFileField = "";
					ImGui::CloseCurrentPopup();
				}
			}
		}
		if (ImGui::Button("Close")) {
			findFileField = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (selectedMakeArchiveIndex != -1)
		ImGui::OpenPopup("Make Game Archive");
	if (ImGui::BeginPopupModal("Make Game Archive")) {
		if (mpMakeArchiveWaiter == 0) {
			ImGui::Text("Make %s options", mResourceSets[selectedMakeArchiveIndex].mName.c_str());
			ImGui::Text("Compress:");
			ImGui::SameLine();
			ImGui::Checkbox("##com", &mbMakeZlib);
			ImGui::Text("Encrypt:");
			ImGui::SameLine();
			ImGui::Checkbox("##enc", &mbMakeEncrypt);
			if (mbMakeEncrypt)
				mbMakeZlib = true;
			if (ImGui::Button("Make")) {
				mpMakeArchiveWaiter = new std::atomic_bool(false);
				progressMsg.store(0);
				Job j{};
				j.mpFn = &AsyncMakeArchive2;
				j.priority = 1;
				j.mpUserInstance = this;
				j.name = "TTArchive2::Create";
				j.userA = j.userB = 0;//no user, this holds state and is not accessed as popup modal.
				JobScheduler::Scheduler().PostJob(this, std::move(j));
			}
			if(ImGui::Button("Close")){
				selectedMakeArchiveIndex = -1;
				ImGui::CloseCurrentPopup();
				mpMakeArchiveWaiter = 0;
			}
		}else{
			if(mpMakeArchiveWaiter->load()){
				delete mpMakeArchiveWaiter;
				mpMakeArchiveWaiter = 0;
				MessageBoxA(0, "The archive was created!", "Success", MB_ICONINFORMATION);
				ImGui::CloseCurrentPopup();
				selectedMakeArchiveIndex = -1;
			}else{
				const char* stat = (const char*)progressMsg.load();
				ImGui::Text("Making archive. Status: %s", stat ? stat : "Setting up");
			}
		}
		ImGui::EndPopup();
	}
	if (selectedArchiveIndex != -1)
		ImGui::OpenPopup("Archive Options");
	if (ImGui::BeginPopupModal("Archive Options")) {
		if (ImGui::Button("Delete Archive")) {
			auto it = mResourceSets.begin();
			std::advance(it, selectedArchiveIndex);
			std::filesystem::path folder = game_data / it->mGameDataPathPost.substr(1);
			std::filesystem::remove_all(folder);
			mResourceSets.erase(it);
			selectedArchiveIndex = -1;
			MessageBoxA(0, "User archive was deleted", "Success", MB_ICONINFORMATION);
			ImGui::CloseCurrentPopup();
		}
		ImGui::Text("Click below to make the archive.");
		ImGui::Text("This will create the TTARCH(2) in the");
		ImGui::Text("game archives. Note the folder on disk");
		ImGui::Text("with the files won't be deleted!");
		if (ImGui::Button("Make Archive")) {
			ImGui::CloseCurrentPopup();
			mbMakeEncrypt = mbMakeZlib = 0;
			selectedMakeArchiveIndex = selectedArchiveIndex;
			selectedArchiveIndex = -1;
			ImGui::OpenPopup("Make Game Archive");
		}
		if (ImGui::Button("Close")) {
			selectedArchiveIndex = -1;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::Button("Find File Location")) {
		ImGui::OpenPopup("Find File");
	}
	ImGui::SameLine();
	if (ImGui::Button("Try Resolve Symbol")) {
		ImGui::OpenPopup("Find Symbol");
	}
	ImGui::SameLine();
	if (ImGui::Button("Create New Archive")) {
		findFileField = sBlowfishKeys[sSetKeyIndex].game_id + std::string("_pc_MyGameData_txmesh.ttarch2");
		ImGui::OpenPopup("New Archive");
	}
	ImGui::SameLine();
	ImGui::Text("Filter Type: ");
	ImGui::SameLine();
	int sel_ind{};
	if (ImGui::BeginCombo("##combo1", filterType.c_str()))
	{
		bool is_selected_ = filterType.length() == 0;
		if (ImGui::Selectable("All", &is_selected_))
			filterType.clear();
		for (int n = 0; n < (int)sRuntime.filterListMSTypes.size(); n++)
		{
			if (sRuntime.mcd_map[n]->mpExt == 0)
				continue;
			bool is_selected = !strcmp(filterType.c_str(), sRuntime.filterListMSTypes[n]);
			if (is_selected)
				sel_ind = n;
			if (ImGui::Selectable(sRuntime.filterListMSTypes[n], is_selected)) {
				filterType = sRuntime.filterListMSTypes[n];
				sel_ind = n;
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::Text("Filter Filename (use * to match wildcard):");
	ImGui::SameLine();
	ImGui::InputText("##ffn", &mFilterString);
	std::string filterExt = filterType.length() == 0 ? "" : std::string(TelltaleToolLib_FindMetaClassDescription(filterType.c_str(), true)->mpExt);
	if (ImGui::BeginTable("##table2", 2, ImGuiTableFlags_Borders)) {
		int i = 0;
		for (auto& it : mResourceSets) {
			if (it.mType == GameEditor_DiskResourceSet::DATA_ARCHIVE) {
				ImGui::PushID(i);
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				if(ImGui::TreeNodeEx(it.mGameDataPathPost.substr(1).c_str(), ImGuiTreeNodeFlags_OpenOnArrow)){
					for (auto& f : it.filenames){
						if (filterExt.length()!=0 && !ends_with(f, filterExt))
							continue;
						if(mFilterString.length() && !_GFG_StringMatches(mFilterString.c_str(), f.c_str()))
							continue;
						if (ImGui::TreeNodeEx(f.c_str(), ImGuiTreeNodeFlags_Leaf)) {
							ImGui::TreePop();
						}
						if(ImGui::IsItemHovered()){
							ImGui::BeginTooltip();
							ImGui::Text("Double click to open");
							if(ends_with(f,".scene"))
								ImGui::Text("L+Shift and Double click to set as active scene");
							ImGui::EndTooltip();
							if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
								u64 crc = CRC64_CaseInsensitive(0, f.c_str());
								TTArchive2::ResourceEntry* entry = it.mArchive2->_FindResource(crc);
								DataStream* pStream = it.mArchive2->GetResourceStream(entry);
								if(ImGui::IsKeyDown(ImGuiKey_LeftShift) && ends_with(f,".scene")){
									MetaClassDescription* pSceneM = GetMetaClassDescription <Scene>();
									MetaStream stream{};
									stream.Open(pStream, MetaStreamMode::eMetaStream_Read, {});
									OnSceneExit();
									if (PerformMetaSerializeFull(&stream, mpActiveScene=(Scene*)pSceneM->New(),pSceneM) != eMetaOp_Succeed) {
										MessageBoxA(0, "That scene could not be opened", f.c_str(), 0);
										delete mpActiveScene;
										mpActiveScene = 0;
										return;//next frame...
									}
									OnSceneLoad();
								}
								else {
									PreviewFile(f, pStream);
								}
							}
						}
					}
					ImGui::TreePop();
				}
				ImGui::TableSetColumnIndex(1);
				ImGui::PopID();
			}else if(it.mType == GameEditor_DiskResourceSet::USER_ARCHIVE){
				std::filesystem::path folder = game_data / it.mGameDataPathPost.substr(1);
				ImGui::PushID(i);
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(1);
				if (ImGui::Button("User Options")) {
					selectedArchiveIndex = i;
				}
				ImGui::SameLine();
				if (ImGui::Button("Open in System Explorer")) {
					ShellExecuteA(NULL, "open", folder.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
				}
				ImGui::TableSetColumnIndex(0);
				if (ImGui::TreeNodeEx(it.mName.c_str(), ImGuiTreeNodeFlags_OpenOnArrow)) {
					for (auto& file : std::filesystem::directory_iterator{ folder }) {
						ImGui::TableNextRow();
						if(file.is_regular_file()){
							std::string f = file.path().filename().string();
							ImGui::TableSetColumnIndex(0);
							if (ImGui::TreeNodeEx(f.c_str(), ImGuiTreeNodeFlags_Leaf)) {
								ImGui::TreePop();
							}
							if (ImGui::IsItemHovered()) {
								ImGui::BeginTooltip();
								ImGui::Text("Double click to open");
								if (ends_with(f, ".scene"))
									ImGui::Text("L+Shift and Double click to set as active scene");
								ImGui::EndTooltip();
								if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
									DataStream* pStream = OpenDataStreamFromDisc(file.path().string().c_str(), READ);
									if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && ends_with(f, ".scene")) {
										MetaClassDescription* pSceneM = GetMetaClassDescription <Scene>();
										MetaStream stream{};
										stream.Open(pStream, MetaStreamMode::eMetaStream_Read, {});
										OnSceneExit();
										if (PerformMetaSerializeFull(&stream, mpActiveScene = (Scene*)pSceneM->New(), pSceneM) != eMetaOp_Succeed) {
											MessageBoxA(0, "That scene could not be opened", f.c_str(), 0);
											delete mpActiveScene;
											mpActiveScene = 0;
											return;//next frame...
										}
										OnSceneLoad();
									}
									else {
										PreviewFile(f, pStream);
									}
								}
							}
							ImGui::TableSetColumnIndex(1);
							if (ImGui::Button("Delete File")) {
								std::filesystem::remove(file);
							}
						}
					}
					ImGui::TreePop();
				}
				ImGui::PopID();
			}
			i++;
		}
		ImGui::EndTable();
	}
}

void GameEditorTask::RenderUI(){
	//RENDER UI IN MAIN IMGUI EDITOR

	if (sentArchives >= loadedArchives.load()) {
		if (mbScenePreloading) {
			//DO NOT ACCESS ANYTHING APART FROM THESE GIVEN HERE!
			ImGui::Text("Currently loading scene resources.");
			ImGui::Text("Percentage Complete: %.03f%%", ((float)loadedArchives.load() / (float)sentArchives) * 100.f);
			if (loadedArchives.load() >= sentArchives) {
				mbScenePreloading = false;
				mbScenePreloaded = true;
				loadedArchives.store(0);
				sentArchives = 0;
				OnScenePostLoad();
			}
		}
		else {
			ImGui::Text("Waiting for archives to load: %.03f%% complete...", ((float)loadedArchives.load() / (float)sentArchives) * 100.f);
			if (sentArchives <= loadedArchives.load())
				sentArchives = -1;
		}
	}
	else {

		ImDrawList& ui = *ImGui::GetWindowDrawList();
		const ImVec2 startTL = ImVec2(0, ImGui::GetCursorPosY() + 10.f);
		const ImVec2 endBR = ImGui::GetWindowPos() + ImGui::GetWindowSize();
		float wWidth = endBR.x - startTL.x;
		float wHeight = endBR.y - startTL.y;
		TextCentered("Editing " + std::string(sBlowfishKeys[sSetKeyIndex].game_name));
		if(mpActiveScene){
			TextCentered("Editing scene " + mpActiveScene->mName);
		}
		TextCentered("Select editor view");
		if (ImGui::Button("Resource Set Descriptions")) {
			currentView = RESDESCS;
		}
		ImGui::SameLine();
		if (ImGui::Button("Game Data Archives")) {
			currentView = GAMEDATA;
		}
		ImGui::SameLine();
		if (ImGui::Button("Scene Editor")) {
			currentView = SCENE_EDITOR;
		}

		ImGui::Separator();

		if (currentView == RESDESCS)
			RenderResDescs();
		else if (currentView == GAMEDATA)
			RenderGameData();
		else if (currentView == SCENE_EDITOR)
			RenderSceneEditor();
		else
			ImGui::Text("Internal error: unknown view");

	}

}

DataStream* GameEditorTask::OpenResource(Symbol name, std::string& nameDest){
	if (mpActiveScene == 0){
		TTL_Log("Cannot open resource %llx: no active scene and patch sets",name.GetCRC());
		return 0;
	}
	for (auto& archive : mResourceSets) {
		TTArchive2* pArchive = archive.mType == GameEditor_DiskResourceSet::DATA_ARCHIVE ? archive.mArchive2 : 0;
		if (pArchive == 0) {
			continue;
		}
		else {
			TTArchive2::ResourceEntry* pEntry = pArchive->_FindResource(name);
			if (pEntry) {
				nameDest = pArchive->GetResourceName(name);
				return pArchive->GetResourceStream(pEntry);
			}
		}
	}
	//dont search others.
	return 0;
}

void* GameEditorTask::FindLoadedResource(Symbol name){
	return mLoadedResources.GetProperty(name);
}

void* GameEditorTask::FindOrLoadResource(Symbol name, MetaClassDescription* clazz) {
	void* pFound = FindLoadedResource(name);
	if (pFound)
		return pFound;
	std::string _name{};
	DataStream* pLoaded = OpenResource(name, _name);
	if(!pLoaded){
		//TTL_Log("ERROR: Could not find find/load resource %llx %s\n", name.GetCRC(), _name.c_str());
		//can be quiet, just wasnt found.
	}else{
		MetaStream stream{};
		stream.mbDontDeleteStream = false;//do delete the stream
		stream.Open(pLoaded, MetaStreamMode::eMetaStream_Read, {});
		void* pInst = mLoadedResources.AddProperty(name.GetCRC(), clazz, 0);
		if(PerformMetaSerializeFull(&stream, pInst, clazz) != eMetaOp_Succeed){
			TTL_Log("ERROR: could not load resource %llx %s\n", name.GetCRC(), _name.c_str());
			mLoadedResources.RemoveProperty(name);
			return 0;
		}
		return pInst;
	}
	return 0;
}

void GameEditorTask::AsyncDispatchPreloads(void* _task, void*, void*){
	GameEditorTask* task = (GameEditorTask*)_task;
	for(int i = 0; i < task->mpScenePreloadPackage->mResources.mSize; i++){
		Job jb{};
		jb.mpFn = &AsyncPreloadResource;
		jb.priority = 1;
		jb.userA = (void*)task->mpScenePreloadPackage->mResources[i].mResourceName.GetCRC();
		jb.userB = (void*)task->mpScenePreloadPackage->mResources[i].mMetaClassDescriptionCrc;
		jb.mpUserInstance = _task;
		jb.name = "";//name empty. lots of these jobs to don't need to print
		JobScheduler::Scheduler().PostJob(task, jb);
	}
}

void GameEditorTask::AsyncPreloadResource(void* _task, void* res, void* type) {
	GameEditorTask* task = (GameEditorTask*)_task;
	sSetKeyIndex = TelltaleToolLib_GetGameKeyIndex(task->mpGameID);
	MetaClassDescription* clazz = TelltaleToolLib_FindMetaClassDescription_ByHash((u64)type);
	Symbol resourceName = Symbol((uintptr_t)res);
	std::string _n{};
	if(clazz == 0){
		TTL_Log("ERROR unknown file type hash %llx. Cannot load resource %llx\n", (u64)type, resourceName.GetCRC());
	}
	else {
		task->lock.lock();// LOCK 
		DataStream* pStream = task->OpenResource(resourceName, _n);
		if (!pStream) {
			task->lock.unlock(); // UNLOCK
			std::string resolvedRes{};
			TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(0, resourceName.GetCRC(), &resolvedRes);
			if (resolvedRes.length() == 0) {
				char* tmp = new char[1024];
				snprintf(tmp, 1024, "Symbol<%llX>", resourceName.GetCRC());
				resolvedRes = tmp;
				delete[] tmp;
			}
			TTL_Log("Could not find resource %s in selected resource descriptions! Either it doesn't exist or you should select other resource sets!\n", resolvedRes.c_str());
			task->lock.lock();
			task->erroredResources.push_back(std::move(resolvedRes));
			task->lock.unlock();
		}
		else {
			DataStreamMemory memoryCopy = DataStreamMemory(pStream->GetSize(), 10);//create memory copy as sub streams will read from same ttarch stream: concurrent problems!
			pStream->Copy(&memoryCopy, 0, 0, pStream->GetSize());
			void* pInstance = task->mLoadedResources.AddProperty(resourceName.GetCRC(), clazz, 0);
			delete pStream;
			task->lock.unlock(); // UNLOCK
			memoryCopy.SetMode(DataStreamMode::eMode_Read);
			memoryCopy.SetPosition(0, DataStreamSeekType::eSeekType_Begin);

			//SAFE HERE
			MetaStream stream{};
			stream.mbDontDeleteStream = true;//dont delete it
			stream.Open(&memoryCopy, MetaStreamMode::eMetaStream_Read, {});
			if (PerformMetaSerializeFull(&stream, pInstance, clazz) != eMetaOp_Succeed) {
				TTL_Log("Could not load %s!\n", _n.c_str());
				// UNSAFE. LOCK.
				task->lock.lock();
				task->mLoadedResources.RemoveProperty(resourceName);
				task->erroredResources.push_back(std::move(_n));
				task->lock.unlock();
				//SAFE.
			}
			else {

				TTL_Log("Loaded %s successfully\n", _n.c_str());

			}
		}
	}
	task->loadedArchives++;
}

void GameEditorTask::RenderSceneEditor(){
	if (mpActiveScene == 0) {
		ImGui::Text("Please load a scene by L+Shift + double clicking on one from the game data view");
	}else{
		if(!mbScenePreloaded && ImGui::Button("Preload Scene")){
			Symbol preloadPackage = Symbol(mpActiveScene->mName.c_str());
			preloadPackage.Concat(".preloadpackagerts");
			PreloadPackage::RuntimeDataScene* pSceneData = (PreloadPackage::RuntimeDataScene*)FindOrLoadResource(preloadPackage, TelltaleToolLib_FindMetaClassDescription("PreloadPackage::RuntimeDataScene",true));
			if(!pSceneData || pSceneData->mResources.mSize == 0){
				MessageBoxA(0, "The scene preloadpackage could not be found or is empty.", "PreloadPackage::RuntimeDataScene error", MB_ICONERROR);
			}else{
				mpScenePreloadPackage = pSceneData;
				mbScenePreloading = true;
				erroredResources.clear();
				sentArchives = pSceneData->mResources.mSize;
				loadedArchives.store(0);
				Job j{};
				j.mpFn = &AsyncDispatchPreloads;
				j.priority = 1;
				j.mpUserInstance = this;
				j.name = "Async scene preload package resource load dispatch";
				JobScheduler::Scheduler().PostJob(this, j);
				return;
			}
		}
		{
			if (mbScenePreloaded) {
				if(ImGui::BeginTable("##errorfiles", 1, ImGuiTableFlags_Borders)){
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					if(ImGui::TreeNodeEx("Resources that failed to preload", ImGuiTreeNodeFlags_OpenOnArrow)){
						for (auto& err : erroredResources) {
							if (ImGui::TreeNodeEx(err.c_str(), ImGuiTreeNodeFlags_Leaf))
								ImGui::TreePop();
						}
						ImGui::TreePop();
					}
					ImGui::EndTable();
				}
			}
			if (ImGui::Button("Unload Scene")) {
				OnSceneExit();
				return;
			}
			Scene& scene = *mpActiveScene;
			auto paint = ImGui::GetWindowDrawList();
			ImGui::Text("Scene Agents");
			if (ImGui::BeginTable("##agents", 2, ImGuiTableFlags_Borders)) {
				int i = 0;
				for (auto& agentPtr : scene.mAgentList) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					Scene::AgentInfo& agent = *agentPtr;
					sceneLister.PropTree(&agent.mAgentSceneProps, agent.mAgentName.c_str(), i);
					ImGui::TableSetColumnIndex(1);

					i++;
				}
				ImGui::EndTable();
			}
		}
	}
}

/*
struct RenderObject_Mesh : public RenderObjectBase {

	struct TriangleSetInstance
	{
		BitSet<enum T3EffectFeature, 30, 54> mStateEffectFeatures;
		BitSet<enum T3EffectParameterType, 150, 0> mEffectParameterTypes;
		//T3EffectParameterGroup* mEffectParameters = 0;
		T3RenderStateBlock mStateBlock;
		BitSet<enum T3EffectFeature, 54, 0> mStaticEffectFeatures;
		T3EffectCacheRef mEffectRef[12];
		int mLightGroupIndex;
		unsigned int mRenderDirty;

		inline TriangleSetInstance() {
			mLightGroupIndex = -1;
			mRenderDirty = eRenderDirty_All;
		}

	};

	struct MeshLODInstance {
		Sphere mBoundingSphere;
		BoundingBox mBoundingBox;
		std::vector<TriangleSetInstance> mTriangleSets[2];
		BinaryBuffer mEffectParameterGroupBuffer;
		DCArray<Symbol> mBones;
		float mShadowDepthBias = 0.f;
		BitSet<T3EffectParameterType, eEffectParameter_Count, 0> mEffectParameterTypes;
	};

	struct VertexStateInstance {
		std::shared_ptr<T3GFXVertexState> mpVertexState[14];
	};

	struct MeshInstance {
		Sphere mBoundingSphere;
		BoundingBox mBoundingBox;
		std::vector<MeshLODInstance> mLODs;
		std::vector<VertexStateInstance> mVertexStates;
	};

	D3DMesh* mpBaseMesh = 0;
	DCArray<D3DMesh*> mpMeshList{};
	std::vector<MeshInstance> mMeshInstances;

	sf::Glsl::Mat4 xform = sf::Transform{};

	virtual void Render(GameEditorTask* pTask) override{



	}

	RenderObject_Mesh(GameEditorTask* pTask, D3DMesh* pMesh){
		mpBaseMesh = pMesh;
	}

	void Setup(){
		mMeshInstances.clear();
		for(int i = 0; i < mpMeshList.mSize; i++){
			// SETUP LODS
			MeshInstance& instance = mMeshInstances.emplace_back();
			D3DMesh& mesh = *mpMeshList[i];
			instance.mLODs.reserve(mesh.mMeshData.mLODs.mSize);
			instance.mBoundingBox = mesh.mMeshData.mBoundingBox;
			instance.mBoundingSphere = mesh.mMeshData.mBoundingSphere;
			for(int j = 0; j < mesh.mMeshData.mLODs.mSize; j++){
				T3MeshLOD& lod = mesh.mMeshData.mLODs[j];
				MeshLODInstance& lodInstance = instance.mLODs.emplace_back();
				lodInstance.mBoundingBox = lod.mBoundingBox;
				lodInstance.mBoundingSphere = lod.mBoundingSphere;
				lodInstance.mBones = lod.mBones;
				if (j == mesh.mMeshData.mLODs.mSize - 1)
					lodInstance.mShadowDepthBias = 0.001f;
				else
					lodInstance.mShadowDepthBias = 0.f;
				lodInstance.mEffectParameterTypes.Set(T3EffectParameterType::eEffectParameter_UniformBufferInstance, true);
				//T3EffectParameterGroupHeader header{};
				//header.InitializeHeader(lodInstance.mEffectParameterTypes);
				//T3EffectParameterGroupHeader* batchHeaders = new T3EffectParameterGroupHeader[lod.mNumBatches];
				//TODO WITH UPDATED
				u32 maxAlign = 0;
				u32 batchHeaderIndex = 0;
				u32 totalHeaderBufSize = 0;// header.mBufferScalarSize;
				for (int batchType = 0; batchType < 2; batchType++) {
					lodInstance.mTriangleSets[batchType].reserve(lod.mBatches[batchType].mSize);//mbatches[0]: normal data (non shadow)
					for (int k = 0; k < lod.mBatches[batchType].mSize; k++) {
						T3MeshBatch& batch = lod.mBatches[batchType][k];
						TriangleSetInstance& triangleSet = lodInstance.mTriangleSets[batchType].emplace_back();
						triangleSet.mEffectParameterTypes.Set(eEffectParameter_UniformBufferMeshBatch, 1);
						triangleSet.mEffectParameterTypes.Set(eEffectParameter_UniformBufferLights, 1);
						if (batch.mTextureIndices.mIndex[1] > 0)
							triangleSet.mEffectParameterTypes.Set(eEffectParameter_SamplerStaticShadowmap, 1);//optional shadowmap parameter for batch
						if (batch.mTextureIndices.mIndex[0] > 0)
							triangleSet.mEffectParameterTypes.Set(eEffectParameter_SamplerLightmapFlat, 1);//optional lightmap for batch
						//batchHeaders[batchHeaderIndex].InitializeHeader(triangleSet.mEffectParameterTypes);
						//if (batchHeaders[batchHeaderIndex].mMaxScalarAlign > maxAlign)
						//	maxAlign = batchHeaders[batchHeaderIndex].mMaxScalarAlign;
						//totalHeaderBufSize += batchHeaders[batchHeaderIndex].mBufferScalarSize;
					}
				}
				lodInstance.mEffectParameterGroupBuffer.SetData(totalHeaderBufSize, 0);
				char* pCurrentBuf = lodInstance.mEffectParameterGroupBuffer.mpData;
				//header.Create(pCurrentBuf);
				//pCurrentBuf += header.mBufferScalarSize;
				for(int k = 0; k < lod.mNumBatches; k++){
					//batchHeaders[k].Create(pCurrentBuf);
					//pCurrentBuf += batchHeaders[k].mBufferScalarSize;
				}
				//delete[] batchHeaders;
			}

			// SETUP VERTEX STATES
			instance.mVertexStates.reserve(mesh.mMeshData.mVertexStates.mSize);
			for(int j = 0; j < mesh.mMeshData.mVertexStates.mSize; j++){
				VertexStateInstance& state = instance.mVertexStates.emplace_back();
				std::shared_ptr<T3GFXVertexState> pCopy = std::make_shared<T3GFXVertexState>(*mesh.mMeshData.mVertexStates[j]);
			}

		}
	}

	inline ~RenderObject_Mesh() {}//no delete any meshes, not managed here.

};*/

void GameEditorTask::RenderScene(){

	/*sf::Event event;
	//Delta time since last frame was drawn
	double dt = frameNum == 0 ? 0. : (double)clock.getElapsedTime().asMicroseconds() * 1e-6;
	clock.restart();
	while (mpWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed){
			MessageBoxA(0, "You cannot close the scene view window! Minimize it, or exit the game editor from the main Inspector window!", "!!", MB_ICONINFORMATION);
		}else if (event.type == sf::Event::Resized) {
			mpWindow->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			sceneCamWindowWidth = event.size.width;
			sceneCamWindowHeight = event.size.height;
		}

		
	}
	mpWindow->clear(sf::Color::Black);

	// Draw
	

	mpWindow->display();*/
	frameNum++;
}

DataStream* GameEditorTask::OpenShaderPackage(const String& fxpack) {
	TTL_Log("Cannot load shader packages yet. IMPLEMENT ME");
	__debugbreak();
	return 0;
}

void* GameEditorTask::GetResource(Symbol name, MetaClassDescription* pMCD) {
	return FindOrLoadResource(name, pMCD);
}

//Pre preload.
void GameEditorTask::OnSceneLoad() {}

void GameEditorTask::OnScenePostLoad() {
	// Load the scene.

	/*
	*		Different types of agents:
	* 
		PathTo::OnSetupAgent((Ptr<Agent> *)pAgentGettingCreated.mpData, &shPathToProps);
		VfxGroup::OnSetupAgent((Ptr<Agent> *)v1, &shVfxGroupProps);
		NavCam::OnSetupAgent((Ptr<Agent> *)v1, &shNavCamProps);
		Text::OnSetupAgent((Ptr<Agent> *)v1, &shTextProps);
		Text2::OnSetupAgent((Ptr<Agent> *)v1, &shText2Props);
		Rollover::OnSetupAgent((Ptr<Agent> *)v1, &shRolloverProps);
		Trigger::OnSetupAgent((Ptr<Agent> *)v1, &shTriggerProps);
		Dialog::OnSetupAgent((Ptr<Agent> *)v1, &shDialogProps);
		FootSteps::OnSetupAgent((Ptr<Agent> *)v1, &shFootstepProps);
		Footsteps2::OnSetupAgent((Ptr<Agent> *)v1, &shFootstep2Props);
		PhysicsObject::OnSetupAgent((Ptr<Agent> *)v1, &shPhysicsObjectProps);
		WalkAnimator::OnSetupAgent((Ptr<Agent> *)v1, &shWalkAnimatorProps, 1);
		Style::OnSetupAgent((Ptr<Agent> *)v1, &shStyleProps);
		LipSync::OnSetupAgent((Ptr<Agent> *)v1, &shLipSyncProps);
		LipSync2::OnSetupAgent((Ptr<Agent> *)v1, &shLipSyncProps);
		HLSPlayer::OnSetupAgent((Ptr<Agent> *)v1, &shHLSPlayerProps);
		Sound3d::OnSetupAgent((Ptr<Agent> *)v1, &sh3dSoundProps);
		ParticleEmitter::OnSetupAgent((Ptr<Agent> *)v1, &shParticleEmitterProps, &shParticleEmitterMaterialProps);
		ParticleAffector::OnSetupAgent((Ptr<Agent> *)v1, &shParticleAffectorProps);
		SoundSnapshot::OnSetupAgent((Ptr<Agent> *)v1, &shSoundSnapshotProps);
		SoundEventEmitter::OnSetupAgent((Ptr<Agent> *)v1, &shSoundEventEmitterProps);
		SoundAmbienceInterface::OnSetupAgent((Ptr<Agent> *)v1, &shSoundAmbienceInterfaceProps);
		SoundListenerInterface::OnSetupAgent((Ptr<Agent> *)v1, &shSoundListenerInterfaceProps);
		SoundMusicInterface::OnSetupAgent((Ptr<Agent> *)v1, &shSoundMusicInterfaceProps);
		SoundReverbInterface::OnSetupAgent((Ptr<Agent> *)v1, &shSoundReverbInterfaceProps);
		SoundSfxInterface::OnSetupAgent((Ptr<Agent> *)v1, &shSoundSfxInterfaceProps);
		SoundEventPreloadInterface::OnSetupAgent((Ptr<Agent> *)v1, &shSoundEventPreloadInterfaceProps);
		VoiceSpeaker::OnSetupAgent((Ptr<Agent> *)v1, &shVoiceSpeakerProps);
		Viewport::OnSetupAgent((Ptr<Agent> *)v1, &shViewportProps);
		LightProbe::OnSetupAgent((Ptr<Agent> *)v1, &shLightProbeProps);
		EnvironmentTile::OnSetupAgent((Ptr<Agent> *)v1, &shEnvironmentTileProps);
		EnvironmentLightGroup::OnSetupAgent((Ptr<Agent> *)v1, &shEnvironmentLightGroupProps);
		EnvironmentLight::OnSetupAgent((Ptr<Agent> *)v1, &shEnvironmentLightProps);
		CinematicLight::OnSetupAgent((Ptr<Agent> *)v1, &shCinematicLightProps);
		CinematicLightRig::OnSetupAgent((Ptr<Agent> *)v1, &shCinematicLightRigProps);
		CinematicLightBlocking::OnSetupAgent((Ptr<Agent> *)v1, &shCinematicLightBlockingProps);
		Environment::OnSetupAgent((Ptr<Agent> *)v1, &shEnvironmentProps);
		AgentState::OnSetupAgent((Ptr<Agent> *)v1, &shAgentStateProps);
		RenderDecal::OnSetupAgent((Ptr<Agent> *)v1, &shDecalProps);
		RenderPostMaterial::OnSetupAgent((Ptr<Agent> *)v1, &shPostMaterialProps);
	*/

	RenderThreadContext::SetScene(mpActiveScene);

}

void GameEditorTask::OnSceneExit() {
	if (mpActiveScene) {
		for (auto& ptr : renderObjects)
			delete ptr;
		renderObjects.clear();
		GetMetaClassDescription<Scene>()->Delete(mpActiveScene);
		mpActiveScene = 0;
		mbScenePreloaded = mbScenePreloading = false;
		mpScenePreloadPackage = 0;
		erroredResources.clear();
		mLoadedResources.ClearKeys();
	}
}

void GameEditorTask::Init(std::filesystem::path base) {

	base_path = base;
	memset(working_buffer, 0, sizeof(working_buffer));
	aliveSlot = sRuntime.gen_gate_slot();
	TelltaleToolLib_InitializeT3(0);//TODO RENDER API
}

/*TODO: save sorted indicies to disk for GAME archives to help load time.
* 
* MESH KEYS:
* 
* .. - Material Override (hprop)
* .. - LightEnv Group (enumt3lightenvgroup)
* .. - Render Scene (symbol)
* .. - Render Camera (symbol)
* .. - Render Resolution (enumrendertextureresolution)
* .. - Render Clear Color (color)
* 
* TODO:
* - for twau/twd2 to be supported, add lenc support: when creating new fix _resdesc_50 prefix to _resourcedescriptions or whatever. 
* - for borderland/thrones: uses LUA not lenc, but uses LEn for all scripts (LEo does not exit in exe). so fix that
*/

GameEditorTask::~GameEditorTask() {

	// rest of clean up

	sRuntime.gates[aliveSlot] = 0;
	for (auto it = openTask.begin(); it != openTask.end(); it++) {
		(*it)->mark_as_todelete();
	}

	if (mpActiveScene)
		delete mpActiveScene;
	mpActiveScene = 0;

	for(auto& it : mResourceSets){
		if (it.mArchive2 != 0)
			delete it.mArchive2;
		it.mArchive2 = 0;
	}
	//mpWindow->close();
	CloseHandle(sem);
	sem = 0;
	//if (mpWindow)
	//	delete mpWindow;
	T3::Shutdown();
	sRuntime.current_editor_task = 0;
};