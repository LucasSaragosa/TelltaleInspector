#include "../TelltaleInspector.h"
#include <inttypes.h>
#include "imgui.h"
#include "../imstd/imgui_stdlib.h"

void PropTask::_render() {
	//TODO EMBEDDED PROPS
	lister.frame_check_addrem = true;
	if (ext_access_gate && *ext_access_gate == false) {
		ImGui::Text("This PropertySet set is not available as the parent file was unloaded.");
		return;
	}
	resolve_buf[0] = 0;
	if (selected) {
		ImGui::Text("PropertySet Editor for: %s", game_n);
		ImGui::Text("Use Ctrl+N to create a new property in the current tree branch");
		ImGui::Text("Use Ctrl+D to delete the highlighted property   ");
		ImGui::Text("For lists/sets/maps: when the tooltip appears, Ctrl+N to add new mapping, Ctrl+D to delete hovered.");
		ImGui::Text("NOTE! The tooltip must display 'Map of' or 'Array of' when hovering to be able to create/delete!");
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
						versionInfo.InjectVersionInfo(mTempStream);
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
						mTempStream.InjectVersionInfo(versionInfo);
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
						MessageBoxA(0, "That file is already a parent!", "!!", MB_ICONINFORMATION);
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
			lister.PropTree(&prop, "Properties", (int)this & 0x7FFFFFF);
			ImGui::EndTable();
		}
		ImGui::PopStyleVar();
		lister.ignore_ctrln = false;
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