#include "../../TelltaleInspector.h"
#include "../../nfd.h"
#include "ToolLibrary/TelltaleToolLibrary.h"
#include "imgui.h"
#include "../../imstd/imgui_stdlib.h"

void MeshTask::process_mtl(std::string& in) {
	if (!mtl_remove_png)
		in += ".png";
	in = mtl_prefix + in + mtl_suffix;
	if (bLowerCase)
		in = lowercase(in);

}

void MeshTask::process_mtl_cb(MeshTask* task, std::string& in) {
	task->process_mtl(in);
}

bool MeshTask::try_open_mesh(std::string file) {
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
				TTL_Log("WARNING: Mesh has internal resource which is not a property set! Type: %s\n", resource.GetTypeDesc()->mpTypeInfoExternalName);
				continue;
			}
			PropertySet* props = (PropertySet*)resource.GetHandleObjectPointer();
			Handle<T3Texture>* pDiffuseTex = (Handle<T3Texture>*)props->GetProperty("Material - Diffuse Texture");
			if (pDiffuseTex == nullptr) {
				TTL_Log("WARNING: Mesh internal resource properties does not contain a diffuse texture reference!\n");
				continue;
			}
			if (props->GetPropertyType("Material - Diffuse Texture") != GetMetaClassDescription<Handle<T3Texture>>()) {
				TTL_Log("ERROR: Mesh internal resource property texture reference is not a Handle<T3Texture>!!\n");
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

void MeshTask::render_mtl_edit() {
	ImGui::Separator();
	ImGui::Text("The following can be used to modify the texture file references in the MTL files which are converted. Default is: tex.d3dtx => tex.png");
	ImGui::InputText("Texture Prefix", &mtl_prefix);
	ImGui::InputText("Texture Suffix", &mtl_suffix);
	ImGui::Checkbox("Strip PNG Extension", &mtl_remove_png);
	ImGui::Checkbox("Lowercase MTL References", &bLowerCase);
	ImGui::Separator();
}

void MeshTask::_render() {
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
							TTL_Log("convert success: %s\n", it.path().filename().string().c_str());
					}
					else {
						if (!convert_obj_newer(*mpNormalMesh, id, outobj, outmtl, err, texture_names, process_mtl_cb, this))
							failed.push_back(it.path().filename().string() + " => " + err);
						else
							TTL_Log("convert success: %s\n", it.path().filename().string().c_str());
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
				TTL_Log("A file was not converted: %s\n", fail.c_str());
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
					if (PerformMetaSerializeAsync(&ms, mpNormalMesh) != eMetaOp_Succeed) {
						MessageBoxA(0, "Could not serialize mesh data! Contact me.", "Error in serializing", MB_ICONERROR);
					}
					else {
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
							task->set_prop(std::move(nm), open_gate, id, &MeshPropDelegate, set, this);
						}
					}
					ImGui::PopID();
				}
				else {
					ImGui::Text("Type: %s", resourceType->mpTypeInfoName);
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

MeshTask::~MeshTask() {
	if (mpNormalMesh)
		delete mpNormalMesh;
	if (mpLegacyMesh)
		delete mpLegacyMesh;
	if (open_gate)
		*open_gate = false;
	open_gate = 0;
}

void MeshPropDelegate(PropertySet& set, void* userdata) {
	MeshTask* task = (MeshTask*)userdata;
	task->open_index = -1;
}//no save changes needed