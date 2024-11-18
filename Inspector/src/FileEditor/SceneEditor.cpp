#include "../TelltaleInspector.h"
#include "imgui.h"
#include "../imstd/imgui_stdlib.h"

void SceneTask::_render() {
	if (id == nullptr) {
		id = select_gameid_dropdown(id);
	}
	else {
		if (state) {
			ImGui::Text("Hidden");
			ImGui::SameLine();
			ImGui::Checkbox("##", &mpScene->mbHidden);
			ImGui::Text("Name");
			ImGui::SameLine();
			ImGui::InputText("##", &mpScene->mName);
			if (ImGui::Button("Save")) {
				nfdchar_t* fp = 0;
				if (NFD_SaveDialog("scene", 0, &fp) == NFD_OKAY) {
					DataStreamFile_PlatformSpecific stream = _OpenDataStreamFromDisc_(fp, WRITE);
					free(fp);
					MetaStream s{};
					s.InjectVersionInfo(mmStream);
					s.Open(&stream, MetaStreamMode::eMetaStream_Write, {});
					s.mbDontDeleteStream = true;
					if (PerformMetaSerializeAsync(&s, mpScene) != eMetaOp_Succeed) {
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
				if (ImGui::Button("Create")) {
					if (newAgentName == "" || mpScene->GetAgent(newAgentName.c_str())) {
						MessageBoxA(0, "That agent already exists or you have not entered a name!", "!!", MB_ICONERROR);
					}
					else {
						mpScene->CreateAgent(std::move(newAgentName));
						sort_agents();
						ImGui::CloseCurrentPopup();
					}
				}
				if (ImGui::Button("Exit")) {
					ImGui::CloseCurrentPopup();
					newAgentName = "";
				}
				ImGui::EndPopup();
			}
			ImGui::Text("To delete an agent, hover over its name and use Ctrl+D");
			ImGui::Text("Agents");
			if (ImGui::BeginTable("agentstab", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable)) {
				int index = 0;
				for (auto it = mpScene->mAgentList.begin(); it != mpScene->mAgentList.end(); it++) {
					Scene::AgentInfo* pAgent = *it;
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();

					bool node_open = ImGui::TreeNode(pAgent->mAgentName.c_str());
					ImGui::TableSetColumnIndex(1);
					if (ImGui::IsItemHovered() && ImGui::IsKeyReleased(ImGuiKey_D) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
						if (open_index == -1) {
							mpScene->mAgentList.remove(*it);
							break;
						}
						else {
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
							open_gate = (bool*)&sRuntime.gates[sRuntime.gen_gate_slot()];
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
		}
		else {
			ImGui::Text("Click NEW to create a new scene and OPEN to open an existing scene to edit");
			if (ImGui::Button("New")) {
				state = true;
				mpScene->mbHidden = false;
				mpScene->mName = "adv_myNewScene.scene";
			}
			if (ImGui::Button("Open")) {
				nfdchar_t* fp{};
				if (NFD_OpenDialog("scene", 0, &fp) == NFD_OKAY) {
					DataStreamFile_PlatformSpecific stream = _OpenDataStreamFromDisc_(fp, READ);
					free(fp);
					MetaStream mStream;
					mStream.Open(&stream, MetaStreamMode::eMetaStream_Read, {});
					mStream.mbDontDeleteStream = true;
					if (PerformMetaSerializeAsync(&mStream, mpScene) != eMetaOp_Succeed) {
						MessageBoxA(0, "Could not read scene file, please contact me!", "Error reading file", MB_ICONERROR);
					}
					else {
						state = true;
						sort_agents();
					}
					mmStream.InjectVersionInfo(mStream);
				}
			}
		}
	}
}

void ScenePropDelegate(PropertySet& set, void* userdata) {
	SceneTask* task = (SceneTask*)userdata;
	task->open_index = -1;
}//no save changes needed
