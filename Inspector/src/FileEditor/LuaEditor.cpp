#include "../TelltaleInspector.h"
#include <imgui.h>
#include "../nfd.h"
#include "ToolLibrary/Lua/lua.hpp"
extern "C" {
#include "ToolLibrary/Lua/decompile.h"
#include "ToolLibrary/lua/proto.h"
}

#define toproto(L,i) getproto(L->top+(i))

bool getter(void* data, int index, const char** output)
{
	static char workBuf[1024]{ 0 };//safe bc not multithreaded
	*output = workBuf;
	ImportedFileType type = (ImportedFileType)((imported_file*)data)[index].userData;
	const char* typeStr = 0;
	if (type == _ERR)
		typeStr = "Error File";
	else if (type == UNK)
		typeStr = "Unknown";
	else if (type == LEO)
		typeStr = "Resource Description";
	else if (type == LEN)
		typeStr = "Game Script";
	else if (type == BIN)
		typeStr = "Standard Lua Bytecode";
	else if (type == SRC)
		typeStr = "Standard Lua Source";
	else
		typeStr = "??";
	sprintf_s(workBuf, "%s [%s]", ((imported_file*)data)[index].mName.c_str(), typeStr);
	return true;
}

void LuaTask::import_in_singlefile(const std::filesystem::path& phys, exitstrat& strat) {
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

bool LuaTask::is_raw_script(char* buffer, u64 size) {
	if (size > 1000)
		size = 1000;//cap it here, HUGELY small probability its encrypted but lovely and printable
	for (u64 i = 0; i < size; i++) {
		if (buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\t' && !std::isprint(buffer[i])) {
			if (buffer[i] != '\x1B')
				TTL_Log("invalid character in script, assuming not raw: %c\n", buffer[i]);
			return false;
		}
	}
	return true;
}

const char* LuaTask::reader(lua_State* L, void* ud, size_t* sizep)
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

int  LuaTask::writer(lua_State* L, const void* p, size_t size, void* u)
{
	return ((DataStream*)u)->SerializeWrite((const char*)p, size) ? 0 : 1;
}

char* LuaTask::luadec_strdup(const char* src) {
	return ((src) ? _strdup(src) : NULL);
}

bool LuaTask::do_luac(const char* Chunk, DataStream* pOut, char* pIn, u64 insize, bool bIsCompile) {
	if (bIsCompile) {
		reader(0, 0, &insize);
		if (lua_load(sRuntime.L, reader, pIn, Chunk, "t") != LUA_OK) {
			TTL_Log("could not lua_load bt, lua code syntax error etc %s\n", Chunk);
			return false;
		}
		Proto* f = toproto(sRuntime.L, -1);
		if (luaU_dump(sRuntime.L, f, writer, pOut, 0) != 0) {
			TTL_Log("could not luaU_dump %s\n", Chunk);
			return false;
		}
		return true;
	}
	else {
		int res = 0;
		if ((res = luaL_loadbufferx(sRuntime.L, pIn, insize, Chunk, "b")) != LUA_OK) {
			TTL_Log("could not lua_load bt, lua bytecode error etc(2) %s %d\n", Chunk, res);
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

// COMPILE IF NEEDED. THE ENCRYPT TO NORMAL TELLTALE SCRIPT.
bool LuaTask::convert_to_scriptn(DataStream* pIn, DataStream* pOut, const std::string& name, void* usr) {
	u64 inSize = pIn->GetSize();
	char* buffer = new char[inSize + 8];//+control bytes eg if enc/dec is bigger
	pIn->Serialize(buffer, inSize);

	bool lenc = ends_with(name, "lenc");
	bool alreadyEnc = false;
	bool alreadyCompiled = false;
	if (!memcmp("\x1BLua", buffer, 4))
		alreadyCompiled = true;
	if (!alreadyCompiled) {
		if (lenc) {
			alreadyEnc = !is_raw_script(buffer, inSize);
		}
		else {
			alreadyEnc = !memcmp("\x1BLEn", buffer, 4) || !memcmp("\x1BLEo", buffer, 4);
		}
	}
	if (alreadyEnc){
		//ensure its compiled. 
		//1. decrypt
		u32 outz{ 0 };
		u8* res = lenc ? TelltaleToolLib_DecryptLencScript((u8*)buffer, (u32)inSize, &outz) :
			TelltaleToolLib_DecryptScript((u8*)buffer, (u32)inSize, &outz);
		if(!memcmp("\x1BLua", res, 4)){
			//compiled and encrypted already, OK.
			free(res);
			pOut->Serialize(buffer, inSize);//serialize ORIGINAL. copy original data as its ok already.
			delete[] buffer;
			return true;
		}else if(is_raw_script((char*)res, outz)){
			//source code, compile it.
			memcpy(buffer, res, outz);//chnage buffer data to src script
			free(res);
			inSize = outz;
			alreadyEnc = alreadyCompiled = false;
		}else{
			//something else, likely wrong encryption key. forget about it.
			free(res);
			TTL_Log("could not decrypt script! %s\n", name.c_str());
			delete[] buffer;
			return false;
		}
	}

	u64 size = inSize;

	//1. compile it if needed
	if (!alreadyCompiled) {
		DataStreamMemory tempMem{ inSize };
		bool compileResult = do_luac(name.c_str(), &tempMem, buffer, inSize, true);
		if (!compileResult) {
			delete[] buffer;
			TTL_Log("failed to compile '%s' when converting to telltale compiled/enc script!\n", name.c_str());
			return false;
		}
		delete[] buffer;
		buffer = new char[size = tempMem.GetPosition()];
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
		u8* res = lenc ? TelltaleToolLib_EncryptLencScript((u8*)buffer, (u32)inSize, &outz) :
			TelltaleToolLib_EncryptScript((u8*)buffer, (u32)inSize, &outz);
		if (!res) {
			TTL_Log("could not encrypt script! %s\n", name.c_str());
			delete[] buffer;
			return false;
		}
		pOut->Serialize((char*)res, outz);
		free(res);
	}

	delete[] buffer;
	return true;
}

// ENSURE RAW. ENCRYPT TO RESDESC.
bool LuaTask::convert_to_scripto(DataStream* pIn, DataStream* pOut, const std::string& name, void* usr) {
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
			alreadyEnc = !memcmp("\x1BLEn", buffer, 4) || !memcmp("\x1BLEo", buffer, 4);
		}
	}
	if(alreadyCompiled){
		TTL_Log("could not convert to resource description %s because is already compiled!\n", name.c_str());
		delete[] buffer;
		return false;
	}
	if(alreadyEnc){
		//ensure its src code
		u32 outz{ 0 };
		u8* res = lenc ? TelltaleToolLib_DecryptLencScript((u8*)buffer, (u32)pIn->GetSize(), &outz) :
			TelltaleToolLib_DecryptScript((u8*)buffer, (u32)pIn->GetSize(), &outz);
		if(!memcmp("\x1BLua", res, 4)){
			//compiled
			TTL_Log("could not convert to resdesc, script is already compiled and needs decompilation! %s\n", name.c_str());
			delete[] buffer;
			free(res);
			return false;
		}else if(is_raw_script(buffer, pIn->GetSize())){
			//already src encrypted so OK.
			//just write it
			pOut->Serialize(buffer, pIn->GetSize());
			free(res);
			// no return
			alreadyEnc = true;
		}else{
			//something else, likely wrong encryption key. forget about it.
			free(res);
			TTL_Log("could not decrypt script! %s\n", name.c_str());
			delete[] buffer;
			return false;
		}
	}

	u64 size = pIn->GetSize();

	//1. encrypt it
	if (alreadyEnc) {
		//just write it
		pOut->Serialize(buffer, size);
	}
	else {
		u32 outz{};
		u8* res = lenc ? TelltaleToolLib_EncryptLencScript((u8*)buffer, (u32)pIn->GetSize(), &outz) :
			TelltaleToolLib_EncryptScript((u8*)buffer, (u32)pIn->GetSize(), &outz);
		if (!res) {
			TTL_Log("could not encrypt script! %s\n", name.c_str());
			delete[] buffer;
			return false;
		}
		pOut->Serialize((char*)res, outz);
		free(res);
	}

	delete[] buffer;
	return true;
}

// IF ENCRYPTED, DECRYPT. THATS IT.
bool LuaTask::convert_extract(DataStream* pIn, DataStream* pOut, const std::string& name, void* usr) {
	char* buffer = new char[pIn->GetSize()];
	pIn->Serialize(buffer, pIn->GetSize());
	if (!memcmp("\x1BLEn", buffer, 4) || !memcmp("\x1BLEo", buffer, 4)) {
		bool lenc = ends_with(name, "lenc");
		u32 outz{ 0 };
		u8* res = lenc ? TelltaleToolLib_DecryptLencScript((u8*)buffer, (u32)pIn->GetSize(), &outz) :
			TelltaleToolLib_DecryptScript((u8*)buffer, (u32)pIn->GetSize(), &outz);
		if (!(res[0] == '\x1B') && !is_raw_script((char*)res, outz)) {
			free(res);
			delete[] buffer;
			return false;
		}
		pOut->SerializeWrite((const char*)res, outz);
		free(res);
		delete[] buffer;
		((imported_file*)usr)->userData = BIN;
		return true;
	}else if (!memcmp("\x1BLua", buffer, 4)){
		pOut->SerializeWrite(buffer, pIn->GetSize());
		((imported_file*)usr)->userData = BIN;
		delete[] buffer;
		return true;
	}else if (is_raw_script(buffer, pIn->GetSize())) {
		pOut->SerializeWrite(buffer, pIn->GetSize());
		((imported_file*)usr)->userData = SRC;
		delete[] buffer;
		return true;
	}else{
		delete[] buffer;
		return false;
	}
}

void LuaTask::bulk_conv_all(bool(*converter)(DataStream* pIn, DataStream* pOut, const std::string& name, void* usr), ImportedFileType destSuccess) {

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

	bool replace_after = !mbConvertToFolder;
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
		if (script.mbIsMemory) {
			pIn = script.mMemoryStream;
			pIn->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
			pIn->SetMode(DataStreamMode::eMode_Read);
		}
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
		out.pMemoryStream->SetMode(DataStreamMode::eMode_Write);
		out.mDestIt = it;
		bool bResult = converter(pIn, out.pMemoryStream, script.mName, &script);
		if (!bResult) {
			error << script.mName << "\n";
			errs++;
			it->userData = _ERR;
		}
		else {
			out.size = (u32)out.pMemoryStream->GetPosition();
			convertedScripts.push_back(out);
			if(destSuccess!=UNK)//if it is unknown it is set in the converter using fileIndex
				it->userData = destSuccess;
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

void LuaTask::_render() {
	ImGui::Text("Select Game");
	gameid = select_gameid_dropdown(gameid);
	ImGui::Separator();
	ImGui::Text("Change Mode:");
	ImGui::SameLine();
	ImGui::Checkbox("##ch", &mbConvertToFolder);
	ImGui::Text("UN-CHECKED: Apply conversions to all files loaded below.");
	ImGui::Text("CHECKED: Don't apply conversions to files below, instead save new file data to a folder for each conversion.");
	ImGui::Separator();
	ImGui::Text("The part in square brackets is the detected file type. If it is 'Unknown' then perform a conversion on it.");
	ImGui::Separator();
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
	ImGui::SameLine();
	if (ImGui::Button("Remove Selected File")) {
		if (mFiles.size() > 0)
			mFiles.erase(mFiles.begin() + selectedfile);
		selectedfile = 0;
	}
	ImGui::SameLine();
	if(ImGui::Button("Save All Files") && mFiles.size() > 0){
		nfdchar_t* outp{};
		nfdresult_t dir = NFD_PickFolder(0, &outp, L"Select Destination Folder");
		if (dir == NFD_OKAY) {
			DataStreamMemory memory{ 0x1000 };
			for (auto& it : mFiles) {
				memory.SetMode(DataStreamMode::eMode_Write);
				memory.Truncate(0);
				memory.SetPosition(0, DataStreamSeekType::eSeekType_Begin);
				if (it.mbIsMemory) {
					it.mMemoryStream->SetMode(DataStreamMode::eMode_Read);
					it.mMemoryStream->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
					it.mMemoryStream->Copy(&memory, 0, 0, it.mMemoryStream->GetSize());
					it.mMemoryStream->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
				}
				else {
					DataStreamFileDisc file = _OpenDataStreamFromDisc_(it.mDiskPath.string().c_str(), READ);
					file.Copy(&memory, 0, 0, file.GetSize());
				}
				memory.SetMode(DataStreamMode::eMode_Read);
				memory.SetPosition(0, DataStreamSeekType::eSeekType_Begin);
				std::filesystem::path outfile = outp;
				outfile += "/";
				outfile += it.mName;
				DataStreamFileDisc out = _OpenDataStreamFromDisc_(outfile.string().c_str(), WRITE);
				memory.Copy(&out, 0, 0, memory.GetSize());
			}
			free(outp);
			MessageBoxA(0, "Saved all files!", "Done!", MB_ICONINFORMATION);
		}
	}
	ImGui::Separator();
	ImGui::Text("_rescdesc files (notice the 'c' between res and desc) register build information and are never encrypted / compiled. No use in this task.");
	ImGui::Separator();
	ImGui::Text("Use this to decrypt all the scripts. They will be lua source code if the file is a resdesc, else compiled lua bytecode.");
	if (ImGui::Button("Decrypt All") && mFiles.size() > 0) {
		bulk_conv_all(&convert_extract, UNK);
	}
	ImGui::Text("Use this to convert to a resource set description lua script (_resdesc_XX_XXX.lua). Note: will fail on compiled files! Must be source!");
	if (ImGui::Button("Convert All (To Resource Descriptions)") && mFiles.size() > 0) {
		bulk_conv_all(&convert_to_scripto, LEO);
	}
	ImGui::Text("Use this to convert to a normal encrypted/compiled telltale lua script inside archives.");
	if (ImGui::Button("Convert All (To Normal Telltale Script)") && mFiles.size() > 0) {
		bulk_conv_all(&convert_to_scriptn, LEN);
	}
}