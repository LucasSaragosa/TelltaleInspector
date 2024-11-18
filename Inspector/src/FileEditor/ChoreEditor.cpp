#include "imgui.h"
#include "../InspectorTemplate.h"
#include "../TelltaleInspector.h"
#include <intrin.h>
#include <filesystem>
#include "ToolLibrary/MetaStream_JSON.hpp"
#include "ToolLibrary/VersDB.h"
#include <unordered_map>
#include "../stb/std_image_write.h"
#include "../squish/squish.h"
#include "../imstd/imgui_memedit.h"
#include "ToolLibrary/types/Scene.h"
#include "ToolLibrary/types/AnimOrChore.h"
#include "ToolLibrary/Base64.h"
#include "../imstd/imgui_stdlib.h"
#include "ToolLibrary/Types/LanguageDatabase.h"
#include "ToolLibrary/Types/StyleGuide.h"

bool Inside(ImVec2 p1, ImVec2 p2, ImVec2 pt){
	float xmin = min(p1.x, p2.x);
	float xmax = max(p1.x, p2.x);
	float ymin = min(p1.y, p2.y);
	float ymax = max(p1.y, p2.y);
	return pt.x >= xmin && pt.x <= xmax && pt.y >= ymin && pt.y <= ymax;
}

struct ChoreUserData : TypeUserData
{

	void remove(unsigned long long hash){
		for(auto it = resolved.begin(); it != resolved.end(); it++){
			if(Symbol(it->c_str()).GetCRC() == hash){
				resolved.erase(it);
				break;
			}
		}
	}

	std::string find(unsigned long long hash){
		for (auto it = resolved.begin(); it != resolved.end(); it++) {
			if (Symbol(it->c_str()).GetCRC() == hash) {
				return *it;
			}
		}
		return "";
	}

	void add(std::string s){
		for (auto it = resolved.begin(); it != resolved.end(); it++) {
			if (StringCompareCaseInsensitive().operator()(s, *it)) {
				return;
			}
		}
		resolved.push_back(s);
	}

	virtual ~ChoreUserData() {
		sRuntime.gates[myAliveSlot] = 0;
		for (auto it = openTask.begin(); it != openTask.end(); it++) {
			(*it)->mark_as_todelete();
		}
	}

	Chore* mpChore;
	std::vector<std::string> resolved;
	std::unique_ptr<AnyTypeEditor> depends;
	bool resolvedDone = false;
	std::vector<InspectorTask*> openTask;
	int myAliveSlot = -1;

	bool mbOpenPopup = false;

	float totalHeight = 0.f;
	float viewRange[2] = { 0.f, 10.f };

	const char* newType = "StyleIdleTransitionsRes";
	std::string newAgentName = "";
	int deleteAgentIndex = -1;

	ChoreAgent* pSelected = 0;
	ChoreAgent* pInspectingAgent = 0;
	ChoreResource* pSelectedAgentResource = 0;
	int pSelectedAgentResourceIndex = -1;

	ChoreResource* pInspectingResource = 0;
	int inspectingBlockIndex = -1;

	TypeTask<Chore>* mpTask = 0;

	ChoreAgent dummyUnused;

	ChoreUserData() = default;

	void remove_dependent_task(void* pEmbedInst){
		for(auto it = openTask.begin(); it != openTask.end(); it++){
			if((*it)->GetUserInstance() == pEmbedInst){
				openTask.erase(it);
				break;
			}
		}
	}

	void UpdateUnusedResource(){
		dummyUnused.mResources.ClearElements();
		for (int i = 0; i < mpChore->mNumResources; i++) {
			dummyUnused.mResources.AddElement(0, 0, &i);
		}
		for (int k = 0; k < mpChore->mNumAgents; k++) {
			ChoreAgent* pAgent = mpChore->mPtrAgents[k];
			for (int j = 0; j < pAgent->GetNumResources(); j++) {
				for (int i = 0; i < dummyUnused.mResources.GetSize(); i++) {
					if (dummyUnused.mResources[i] == pAgent->mResources[j]) {
						dummyUnused.mResources.RemoveElement(i);
						break;
					}
				}
			}
		}
	}

};

void CheckboxFlags(const char* name, const char* unqID, int flagv, Flags& flags){
	bool flag = (flags.mFlags & flagv) != 0;
	ImGui::Text(name);
	ImGui::SameLine();
	ImGui::Checkbox(unqID, &flag);
	if (flag) {
		flags.mFlags |= flagv;
	}
	else {
		flags.mFlags &= ~flagv;
	}
}

void ChoreEmbed_Callback(InspectorTask* pTask, void* user){
	ChoreUserData& data = *(ChoreUserData*)user;
	for (auto it = data.openTask.begin(); it != data.openTask.end(); it++) {
		if ((*it) = pTask) {
			data.openTask.erase(it);
			break;
		}
	}
}

void Chore_RenderHeaderButtons(ChoreUserData& data){
	static MetaClassDescription* pDS = GetMetaClassDescription("DCArray<String>");
	Chore* chore = data.mpChore;
	ImGui::SameLine();
	ImGui::Text("Name:");
	ImGui::SameLine();
	ImGui::PushItemWidth(600);
	ImGui::InputText("##nm", &chore->mName);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::Text("Scene:");
	ImGui::SameLine();
	ImGui::PushItemWidth(600);
	ImGui::InputText("##f", &chore->mChoreSceneFile);
	ImGui::PopItemWidth();
	ImGui::Text("Render Delay:");
	ImGui::SameLine();
	ImGui::PushItemWidth(300);
	ImGui::InputInt("##rd", (int*)&chore->mRenderDelay);
	ImGui::SameLine();
	ImGui::PopItemWidth();
	if (ImGui::Button("Edit Dependencies"))
		ImGui::OpenPopup("Chore Dependencies");
	ImGui::SetNextWindowSize(ImVec2{600, 150});
	if (ImGui::BeginPopupModal("Chore Dependencies")) {
		data.mbOpenPopup = true;
		TextCentered("Chore Dependencies (Ctrl+N to add, Ctrl+D to remove - the hovered one)");
		chore->mDependencies.EnsureAlloc();
		data.depends->Render("Dependencies", pDS, chore->mDependencies.mpResNames);
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
	ImGui::SameLine();
	if (ImGui::Button("New Agent")) {
		ImGui::OpenPopup("ChoreNewAgent");
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove An Agent")) {
		ImGui::OpenPopup("Delete Agent");
	}
	ImGui::SameLine();
	ImGui::SetNextWindowSize(ImVec2{ 650, 150 });
	if (ImGui::BeginPopupModal("ChoreNewAgent")) {
		data.mbOpenPopup = true;
		ImGui::Text("Agent Name (must match with scene agent name):");
		ImGui::SameLine();
		ImGui::InputText("##aname", &data.newAgentName);
		if (ImGui::Button("Done") && data.newAgentName.length() > 0) {
			if (chore->FindAgent(Symbol(data.newAgentName.c_str()))!=-1) {
				MessageBoxA(0, "Agent already exists in current chore", data.newAgentName.c_str(), MB_ICONERROR);
			}else if(data.newAgentName == "**Unused Chore Resources**"){
				MessageBoxA(0, "Cannot create an agent with that name!", data.newAgentName.c_str(), MB_ICONERROR);
			}
			else {
				chore->AddAgent(data.newAgentName);
				data.add(data.newAgentName + " blocking");
				data.add(data.newAgentName + " path blocking");
				data.add(data.newAgentName + " voice");
				data.add(data.newAgentName + " lipsync");
				data.newAgentName.clear();
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::SameLine();
		if(ImGui::Button("Close")){
			data.newAgentName.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (data.pInspectingAgent != 0) {
		ImGui::OpenPopup("Edit Agent");
		ImGui::SetNextWindowSize(ImVec2{ 700, 600 });
		if (ImGui::BeginPopupModal("Edit Agent")) {
			data.mbOpenPopup = true;
			ChoreAgent& agent = *data.pInspectingAgent;
			ImGui::Text("Name:");
			ImGui::SameLine();
			ImGui::InputText("##n", &agent.mAgentName);
			ImGui::Text("Actor Binding:");
			ImGui::SameLine();
			ImGui::InputText("##aa", &agent.mAABinding.mActorName);
			ImGui::Text("Enable Agent If:");
			ImGui::SameLine();
			if(ImGui::Button("Edit Rule")){
				bool found = false;
				for (auto it = data.openTask.begin(); it != data.openTask.end(); it++) {
					if ((*it)->GetUserInstance() == &agent.mAgentEnabledRule) {
						found = true;
						break;
					}
				}
				if (found) {
					MessageBoxA(0, "You already have an open task editing this chore agents rule!", "Task exists!", MB_ICONWARNING);
				}
				else {
					//open task
					data.openTask.push_back(CreateAndOpenTypeTask("Rule",
						GetMetaClassDescription<Rule>(), "Chore", data.mpTask->id, &agent.mAgentEnabledRule, &ChoreEmbed_Callback, &data, data.myAliveSlot));
				}
			}
			ImGui::Text("Do Attach:");
			ImGui::SameLine();
			ImGui::Checkbox("##1", &agent.mAttachment.mbDoAttach);
			ImGui::Text("Attach To:");
			ImGui::SameLine();
			ImGui::InputText("##2", &agent.mAttachment.mAttachTo);
			ImGui::Text("Attach To Dialog Node:");
			ImGui::SameLine();
			ImGui::InputText("##3", &agent.mAttachment.mAttachToNode);
			ImGui::Text("Preserve World Position on Attach:");
			ImGui::SameLine();
			ImGui::Checkbox("##4", &agent.mAttachment.mbAttachPreserveWorldPos);
			ImGui::Text("Leave Attached when Complete:");
			ImGui::SameLine();
			ImGui::Checkbox("##5", &agent.mAttachment.mbLeaveAttachedWhenComplete);
			ImGui::Text("Attach To Position:");
			data.depends->Math(CRC64_CaseInsensitive(0, "Vector3"), &agent.mAttachment.mAttachPos);
			ImGui::Text("Attach To Rotation:");
			data.depends->Math(CRC64_CaseInsensitive(0,"Quaternion"), &agent.mAttachment.mAttachQuat);
			if (ImGui::Button("Close")) {
				data.pInspectingAgent = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::SetNextWindowSize(ImVec2{ 650, 150 });
	if (ImGui::BeginPopupModal("Delete Agent")) {
		data.mbOpenPopup = true;
		ImGui::Text("Select an agent from the dropdown below.");
		if (ImGui::BeginCombo("##dd",data.deleteAgentIndex != -1 ? data.mpChore->mPtrAgents[data.deleteAgentIndex]->GetAgentName()->c_str() : 0))
		{
			for(int i = 0; i < data.mpChore->mNumAgents; i++){
				bool bSelected = data.deleteAgentIndex == i;
				if(ImGui::Selectable(data.mpChore->mPtrAgents[i]->GetAgentName()->c_str(), bSelected)) {
					data.deleteAgentIndex = i;
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Delete") && data.deleteAgentIndex!=-1) {
			data.mpChore->RemoveAgent(data.deleteAgentIndex);
			data.deleteAgentIndex = -1;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			data.deleteAgentIndex = -1;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	//more here
	ImGui::Text("Total Length: %.05f sec", data.mpChore->mLength);
	ImGui::Text("Hold L-Shift and use mouse scrollbar to change viewable timeline region.");
	ImGui::SameLine();
	ImGui::Text("Hover over coloured strip (resource) and press I to inspect.");
	ImGui::Text("Hover over coloured strip (resource) and press A or D to add/remove a block.");
	ImGui::Text("Hover over block (red / blue inside resource) and press L-Shift + I to inspect");
	ImGui::SameLine();

	// POPUPS
	if(data.pInspectingResource != 0){
		if(data.inspectingBlockIndex != -1){
			//inspecting a block
			ChoreResource::Block& block = data.pInspectingResource->mBlocks[data.inspectingBlockIndex];
			ImGui::OpenPopup("Edit Block");
			ImGui::SetNextWindowSize(ImVec2{ 450, 400 });
			if (ImGui::BeginPopupModal("Edit Block")) {
				data.mbOpenPopup = true;
				block.mbSelected = true;
				ImGui::Text("Start:");
				ImGui::InputFloat("##s", &block.mStartTime);
				ImGui::Text("End:");
				ImGui::InputFloat("##e", &block.mEndTime);
				ImGui::Text("Loops:");
				ImGui::Checkbox("##l", &block.mbLoopingBlock);
				ImGui::Text("Runtime Time Scale:");
				ImGui::InputFloat("##sc", &block.mScale);
				if (block.mStartTime < 0.f)
					block.mStartTime = 0.f;
				if (block.mStartTime > block.mEndTime)
					block.mEndTime = block.mStartTime;
				if (block.mStartTime == 0.f && block.mEndTime == 0.f)
					block.mEndTime = 1.0f;
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();
					block.mbSelected = false;
					data.inspectingBlockIndex = -1;
					data.pInspectingResource = 0;
				}
				ImGui::EndPopup();
			}
		}else{
			//inspecting the resource
			ChoreResource& resource = *data.pInspectingResource;
			ImGui::OpenPopup("Edit Resource");
			ImGui::SetNextWindowSize(ImVec2{ 900, 800 });
			if (ImGui::BeginPopupModal("Edit Resource")) {
				data.mbOpenPopup = true;
				ImGui::Text("Note: if you change if its external/embedded, the current embedded resource will be deleted.");
				ImGui::Text("Resource Name is only really significant if its an external resource.");
				ImGui::Text("Resource File Scheme:");
				if(ImGui::BeginCombo("##type",resource.mbEmbedded?"Embedded Resource":"External Resource")){
					if (ImGui::Selectable("External Resource", !resource.mbEmbedded)) {
						ImGui::SetItemDefaultFocus();
						resource.mbEmbedded = false;
						if (resource.mhObjectDesc) {
							data.remove_dependent_task(resource.mhObjectEmbedded);
							resource.mhObjectDesc->Delete(resource.mhObjectEmbedded);
						}
						resource.mhObjectEmbedded = 0;
						resource.mhObjectDesc = 0;
						resource.mhObject.SetObjectName("");
					}
					if (ImGui::Selectable("Embedded Resource", resource.mbEmbedded)) {
						ImGui::SetItemDefaultFocus();
						resource.mbEmbedded = true;
						resource.mhObject.SetObjectName("");
						resource.mhObjectEmbedded = 0;
						resource.mhObjectDesc = 0;
					}
					ImGui::EndCombo();
				}
				if(resource.mbEmbedded){
					ImGui::Text("Embedded Resource: ");
					ImGui::SameLine();
					ImGui::SetNextWindowSize(ImVec2{ 650, 450 });
					if(ImGui::BeginPopupModal("Set Embedded Resource")){
						data.mbOpenPopup = true;
						ImGui::Text("Select the embedded resource type from below");
						ComboSelectType(data.newType);
						if (ImGui::Button("Create")) {
							MetaClassDescription* pDesc = TelltaleToolLib_FindMetaClassDescription(data.newType, true);
							if (pDesc->mpVTable[VTableFunction::eVTableNew] == 0) {
								MessageBoxA(0, "That type is abstract! This type cannot be instantiated and therefore cannot be used as an embedded resource!", "Abstract type!", MB_ICONERROR);
							}
							else {
								resource.mhObjectDesc = pDesc;
								resource.mhObjectEmbedded = pDesc->New();

								data.newType = "StyleIdleTransitionsRes";
								ImGui::CloseCurrentPopup();
							}
						}
						if(ImGui::Button("Close")){
							data.newType = "StyleIdleTransitionsRes";
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					if(resource.mhObjectDesc == 0){
						resource.mhObjectEmbedded = 0;
						if(ImGui::Button("None. Click to add")){
							ImGui::OpenPopup("Set Embedded Resource");
						}
					}else{
						ImGui::Text(resource.mhObjectDesc->mpTypeInfoName);
						ImGui::SameLine();
						if(ImGui::Button("Edit")){
							bool found = false;
							for (auto it = data.openTask.begin(); it != data.openTask.end(); it++) {
								if ((*it)->GetUserInstance() == resource.mhObjectEmbedded) {
									found = true;
									break;
								}
							}
							if(found){
								MessageBoxA(0, "You already have an open task editing this embedded resource!", "Task exists!", MB_ICONWARNING);
							}else{
								//open task
								data.openTask.push_back(CreateAndOpenTypeTask(resource.mhObjectDesc->mpTypeInfoName, 
									resource.mhObjectDesc, "Chore", data.mpTask->id, resource.mhObjectEmbedded, &ChoreEmbed_Callback, &data,data.myAliveSlot));
							}
						}
						ImGui::SameLine();
						if (ImGui::Button("Delete Embedded")) {
							data.remove_dependent_task(resource.mhObjectEmbedded);
							resource.mhObjectDesc->Delete(resource.mhObjectEmbedded);
							resource.mhObjectEmbedded = 0;
							resource.mhObjectDesc = 0;
						}
					}
				}
				ImGui::Text("Resource Name: ");
				ImGui::SameLine();
			
				if(ImGui::Button("Edit")){
					ImGui::OpenPopup("Resource Name");
				}
				if(ImGui::IsPopupOpen("Resource Name")){
					bool orElse = false;
					std::string res = data.find(resource.mResName.GetCRC());
					if (res.find('.') != std::string::npos) {
						std::string resext = std::filesystem::path(res).extension().string().substr(1);
						MetaClassDescription* mcd = TelltaleToolLib_FindMetaClassDescription(resext.c_str(), false);
						if (mcd) {
							std::string out{};
 							data.depends->modify_handle(mcd, resource.mResName, "Resource Name", &out, &data.resolved);
							if (out.length() > 0) {
								data.add(out);
								resource.mResName = CRC64_CaseInsensitive(0, out.c_str());
							}
						}
						else
							orElse = 1;
					}
					else orElse = 1;
					if (orElse) {
						std::string out{};
						data.depends->modify_symbol(resource.mResName, "Resource Name", &resource.mResName, res.empty() ? 0 : res.c_str(),&out, &data.resolved);
						if (out.length() > 0) {
							resource.mResName = CRC64_CaseInsensitive(0, out.c_str());
							data.add(out);
						}
					}
				}

				ImGui::Separator();
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
				ImGui::Text("Priority:");
				ImGui::SameLine();
				ImGui::InputScalar("##p", ImGuiDataType_U32, &resource.mPriority);
				CheckboxFlags("Master Cut (NOTE: only ONE resource in the whole chore should have this set!):", "##cb11", ChoreResource::eIsMasterCut, resource.mFlags);
				ImGui::Text("Auto Act Status:");
				ImGui::SameLine();
				ImGui::InputScalar("##aas", ImGuiDataType_U32, &resource.mAAStatus.m_Status);
				ImGui::Text("Enabled:");
				ImGui::SameLine();
				ImGui::Checkbox("##cb1", &resource.mbEnabled);
				ImGui::Text("No Pose:");
				ImGui::SameLine();
				ImGui::Checkbox("##cb2", &resource.mbNoPose);
				ImGui::Text("View Graphs:");
				ImGui::SameLine();
				ImGui::Checkbox("##cb3", &resource.mbViewGraphs);
				ImGui::Text("View Empty Graphs:");
				ImGui::SameLine();
				ImGui::Checkbox("##cb4", &resource.mbViewEmptyGraphs);
				ImGui::Text("View Properties:");
				ImGui::SameLine();
				ImGui::Checkbox("##cb5", &resource.mbViewProperties);
				ImGui::Text("Is Agent Resource:");
				ImGui::SameLine();
				ImGui::Checkbox("##cb6", &resource.mbIsAgentResource);
				ImGui::Text("View Resource Groups:");
				ImGui::SameLine();
				ImGui::Checkbox("##cb6", &resource.mbViewResourceGroups);
				ImGui::Text("Resource Group:");
				ImGui::SameLine();
				ImGui::InputText("##in", &resource.mResourceGroup);
				
				CheckboxFlags("Filter Mover Animation:", "##cb7", ChoreResource::eFilterMoverAnimation, resource.mFlags);
				CheckboxFlags("Mood Idle Persists:", "##cb9", ChoreResource::eMoodIdlePersists, resource.mFlags);
				CheckboxFlags("Force Mover Animation Inclusion:", "##cb10", ChoreResource::eForceInclusionOfMoverAnimation, resource.mFlags);

				if(ImGui::Button("Resource Properties")){
					bool found = false;
					for (auto it = data.openTask.begin(); it != data.openTask.end(); it++) {
						if ((*it)->GetUserInstance() == &resource.mResourceProperties) {
							found = true;
							break;
						}
					}
					if (found) {
						MessageBoxA(0, "You already have an open task editing this resource property set!", "Task exists!", MB_ICONWARNING);
					}
					else {
						//open task
						data.openTask.push_back(CreateAndOpenTypeTask("PropertySet",
							GetMetaClassDescription<PropertySet>(), "Chore", data.mpTask->id, &resource.mResourceProperties, &ChoreEmbed_Callback, &data, data.myAliveSlot));
					}
				}
		
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();
					data.inspectingBlockIndex = -1;
					data.pInspectingResource = 0;
				}
				ImGui::EndPopup();
			}
		}
	}

}

void Chore_RenderWindow(ChoreUserData& data){
	data.mbOpenPopup = false;
	auto* painter = ImGui::GetWindowDrawList();

	//window size is the size of the timeline
	const ImVec2 windowSize = ImGui::GetWindowSize() - ImVec2(0, 150);
	const ImVec2 C = ImGui::GetWindowPos() + ImVec2(0, 230 - ImGui::GetScrollY());

	Chore_RenderHeaderButtons(data);

	// EDITOR BACKGROUND

	painter->AddRectFilled(
		C,
		C + ImVec2(windowSize.x, data.totalHeight),
		ImColor(150,150,150,255)
	);

	// EDITOR SEP

	painter->AddRectFilled(
		C,
		C + ImVec2(windowSize.x, 2.0f),
		ImColor(150, 150, 150, 255)
	);

	// TIMELINE HEADER. FOR NOW: JUST SHOW 10 SECONDS AND LET USER SCROLL
	
	float y = 60.f;
	float initialY = ImGui::GetCursorPos().y + y;
	float range = data.viewRange[1] - data.viewRange[0];
	float choreLengthUpdate = 0.f;

	painter->AddRectFilled(
		C,
		C + ImVec2(windowSize.x, y),
		ImColor(80, 80, 80, 255)
	);
	
	char timeBuffer[16]{ 0 };
	for(int i = 0; i < 10; i++){
		float windowX = windowSize.x * ((float)i / 10.f) + C.x;
		if (i == 0)
			windowX = C.x + 2;
		else if (i == 10)
			windowX = windowSize.x + C.x - 2;
		painter->AddLine(ImVec2(windowX, C.y), ImVec2(windowX, C.y + data.totalHeight), ImColor(100, 100, 100, 255), 2.0);
		for(int j = 1; j < 5; j++){
			float subLineX = windowX + (windowSize.x * (1.0f / 10.f)) * ((float)j / 5.0f);
			painter->AddLine(ImVec2(subLineX, C.y + y), ImVec2(subLineX, C.y + data.totalHeight), ImColor(130, 130, 130, 255), 1.3f);
		}
		sprintf_s(timeBuffer, "%.03fs", data.viewRange[0] + ((float)i / 10.f) * range);
		painter->AddText(0, 15.0f, ImVec2(windowX + 5, C.y + 4), ImColor(190, 190, 190, 255), timeBuffer, timeBuffer + strlen(timeBuffer));
	}

	painter->AddRectFilled(
		C + ImVec2(0, y),
		C + ImVec2(windowSize.x, 0) + ImVec2(0, y-2),
		ImColor(40, 40, 40, 255)
	);

	// CHORE AGENTS

	bool bFinalUnusedLoop = false;
	for(int i = 0; i < data.mpChore->mNumAgents; i++){
		ChoreAgent* pAgent = bFinalUnusedLoop ? &data.dummyUnused : data.mpChore->mPtrAgents[i];
		ImVec2 topLeft = { C.x, C.y + y };
		ImVec2 btmRight = ImVec2{ C.x + windowSize.x, C.y + 24.f + y  };
		// add/remove resources with + and -
		painter->AddRectFilled(topLeft, btmRight, data.pSelected == pAgent ? ImColor(201, 75, 12, 240) : ImColor{82,82,82, 240});
		if (!bFinalUnusedLoop) {
			ImVec2 removeTL = ImVec2{ btmRight.x - 44.f, topLeft.y };
			{
				// -
				ImVec2 p1 = removeTL + ImVec2{ 4, 10 };
				ImVec2 p2 = removeTL + ImVec2{ 20, 14 };
				painter->AddRectFilled(p1, p2, ImColor(210, 131, 252, 240), 1.5f);
				if (!data.mbOpenPopup && Inside(removeTL, btmRight, ImGui::GetMousePos())) {
					data.mbOpenPopup = true;//already clicked
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
						//user clicked. delete selected
						if (data.pSelectedAgentResource == 0) {
							//none selected wtf
							MessageBoxA(0, "Please select a chore resource (coloured strip) using the arrow on the left of it.", "Select a resource", MB_ICONINFORMATION);
						}
						else {
							//remove the resource
							data.mpChore->mPtrAgents.AddElement(0, 0, &data.dummyUnused);
							data.mpChore->mNumAgents++;
							data.mpChore->RemoveResource(pAgent->mResources[data.pSelectedAgentResourceIndex]);
							data.mpChore->mPtrAgents.RemoveElement(data.mpChore->mPtrAgents.mSize - 1);
							data.mpChore->mNumAgents--;
							pAgent->mResources.RemoveElement(data.pSelectedAgentResourceIndex);
							data.UpdateUnusedResource();
							data.pSelectedAgentResource = 0;
							data.pSelectedAgentResourceIndex = -1;
						}
					}
				}
			}
			ImVec2 addTL = ImVec2{ btmRight.x - 70.f, topLeft.y };
			{
				// +
				ImVec2 p1 = addTL + ImVec2{ 4, 10 };
				ImVec2 p2 = addTL + ImVec2{ 20, 14 };
				painter->AddRectFilled(p1, p2, ImColor(210, 131, 252, 240), 1.5f);
				p1 = addTL + ImVec2{ 10, 4 };
				p2 = addTL + ImVec2{ 14, 20 };
				painter->AddRectFilled(p1, p2, ImColor(210, 131, 252, 240), 1.5f);
				if (!data.mbOpenPopup && Inside(addTL, ImVec2(removeTL.x, btmRight.y), ImGui::GetMousePos())) {
					data.mbOpenPopup = true;//already clicked
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
						//user clicked
						data.mpChore->AddResource(Symbol(), 0, 0, 0, 0);
						int v = data.mpChore->mNumResources - 1;;
						pAgent->mResources.AddElement(0, 0, &v);
						data.mpChore->mPtrResources[data.mpChore->GetNumResources() - 1]->mbEnabled = true;
						data.UpdateUnusedResource();
					}
				}
			}
			ImVec2 agentTL = ImVec2{ btmRight.x - 96.f, topLeft.y };
			{
				ImVec2 p1 = agentTL + ImVec2{ 4, 3 };
				ImVec2 p2 = agentTL + ImVec2{ 21, 7 };
				painter->AddRectFilled(p1, p2, ImColor(210, 131, 252, 240), 1.f);
				p1 = agentTL + ImVec2{ 4, 17 };
				p2 = agentTL + ImVec2{ 21, 21 };
				painter->AddRectFilled(p1, p2, ImColor(210, 131, 252, 240), 1.f);
				p1 = agentTL + ImVec2{ 11, 3 };
				p2 = agentTL + ImVec2{ 14, 21 };
				painter->AddRectFilled(p1, p2, ImColor(210, 131, 252, 240), 1.f);
				if (!data.mbOpenPopup && Inside(addTL, ImVec2(agentTL.x, btmRight.y), ImGui::GetMousePos())) {
					data.mbOpenPopup = true;//already clicked
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
						//user clicked
						data.pInspectingAgent = pAgent;
					}
				}
			}
		}
		if (!data.mbOpenPopup && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::GetMousePos().x > topLeft.x && ImGui::GetMousePos().x < btmRight.x && ImGui::GetMousePos().y > topLeft.y && ImGui::GetMousePos().y < btmRight.y) {
			if (data.pSelected == pAgent) {
				// ON DESELECT
				data.pSelected = 0;
				data.pSelectedAgentResource = 0;
				data.pSelectedAgentResourceIndex = -1;
			}
			else {
				data.pSelectedAgentResource = 0;
				data.pSelectedAgentResourceIndex = -1;
				data.pSelected = pAgent;//ON SELECT 
			}
		}
		if(data.pSelected == pAgent){
			ImVec2 p1 = topLeft + ImVec2{ 6, 6 };
			ImVec2 p2 = topLeft + ImVec2{ 18, 6 };
			ImVec2 p3 = topLeft + ImVec2{ 12, 18 };
			painter->AddTriangleFilled(p1, p2, p3, ImColor(65, 132, 240, 255));
		}else{
			ImVec2 p1 = topLeft + ImVec2{ 6, 6 };
			ImVec2 p2 = topLeft + ImVec2{ 6, 18 };
			ImVec2 p3 = topLeft + ImVec2{ 18, 12 };
			painter->AddTriangle(p1, p2, p3, ImColor(65, 132, 240, 255));
		}
		painter->AddText(topLeft + ImVec2{ 24, 2 }, ImColor{ 50,50,50,255 }, pAgent->GetAgentName()->c_str(), pAgent->GetAgentName()->c_str() + pAgent->GetAgentName()->length());
		y += 30.f;
		btmRight = btmRight + ImVec2(0, 30);
		topLeft = topLeft + ImVec2(0, 30);
		if(data.pSelected == pAgent){
			if (pAgent->GetNumResources() > 0) {
				painter->AddRectFilled(topLeft - ImVec2(0, 6), topLeft + ImVec2(24, 0), data.pSelectedAgentResource == data.mpChore->mPtrResources[pAgent->mResources[0]] ? ImColor(201, 75, 12, 150) : ImColor(200, 200, 200, 150));
				//painter->AddLine(ImVec2{ btmRight.x, topLeft.y - 6 }, ImVec2{ btmRight.x, topLeft.y }, ImColor{ 58,58,58,255 });
			}
			for(int j = 0; j < pAgent->GetNumResources(); j++){
				ChoreResource* pResource = data.mpChore->mPtrResources[pAgent->mResources[j]];
				btmRight = topLeft + ImVec2(24.f, 32.f);
				painter->AddRectFilled(topLeft, btmRight+ImVec2(0,8), data.pSelectedAgentResource == pResource ? ImColor(201, 75, 12, 150) : ImColor(200, 200, 200, 150));
				painter->AddLine(ImVec2{btmRight.x, topLeft.y-6}, ImVec2{btmRight.x, btmRight.y+8}, ImColor{58,58,58,255});
				if (!data.mbOpenPopup && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::GetMousePos().x > topLeft.x && ImGui::GetMousePos().x < btmRight.x && ImGui::GetMousePos().y > topLeft.y && ImGui::GetMousePos().y < btmRight.y) {
					if (data.pSelectedAgentResource == pResource) {
						// ON DESELECT
						data.pSelectedAgentResource = 0;
						data.pSelectedAgentResourceIndex = -1;
					}
					else {
						data.pSelectedAgentResource = pResource;//ON SELECT 
						data.pSelectedAgentResourceIndex = j;
					}
				}
				if (data.pSelectedAgentResource == pResource) {
					ImVec2 p1 = topLeft + ImVec2{ 6, 6 };
					ImVec2 p2 = topLeft + ImVec2{ 18, 6 };
					ImVec2 p3 = topLeft + ImVec2{ 12, 18 };
					painter->AddTriangleFilled(p1, p2, p3, ImColor(65, 132, 240, 255));
				}
				else {
					ImVec2 p1 = topLeft + ImVec2{ 6, 6 };
					ImVec2 p2 = topLeft + ImVec2{ 6, 18 };
					ImVec2 p3 = topLeft + ImVec2{ 18, 12 };
					painter->AddTriangle(p1, p2, p3, ImColor(65, 132, 240, 255));
				}
				char aPriority[64]{ 0 };
				sprintf_s(aPriority, "Priority: %d", pResource->mPriority);
				topLeft = topLeft + ImVec2(24, 0);
				btmRight = ImVec2(C.x + windowSize.x + 24.f, topLeft.y + 32.f);
				ImColor bgColor{ 209, 35, 13 };
				MetaClassDescription* handleType = 0;
				std::string res{};
				if(pResource->mbEmbedded){
					res = "Embedded<" + std::string(pResource->mhObjectDesc?pResource->mhObjectDesc->mpTypeInfoName:"None") + ">";
				}
				else {
					res = data.find(pResource->mResName.GetCRC());
				}
				if (res.length()>0) {
					if (res.find('.') == std::string::npos) {
						bgColor = ImColor(224, 214, 193, 255);//eg 'blocking'
					}
					else {
						std::string resext = std::filesystem::path(res).extension().string().substr(1);//lets hope is not eg 'hello.'. cannot end in a dot. not inthe hash db at least.
						handleType = TelltaleToolLib_FindMetaClassDescription(resext.c_str(), false);
					}
				}
				if(handleType!=0){
					if(handleType->MatchesHash(CRC64_CaseInsensitive(0,"Animation"))){
						bgColor = ImColor(252, 185, 177, 255);
					}
					else if (handleType->MatchesHash(CRC64_CaseInsensitive(0, "Chore"))) {
						bgColor = ImColor(166, 172, 255, 255);
					}
					else if (handleType->MatchesHash(CRC64_CaseInsensitive(0, "Procedural_LookAt"))) {
						bgColor = ImColor(255, 231, 176, 255);
					}
					else if (handleType->MatchesHash(CRC64_CaseInsensitive(0, "LanguageRes"))) {
						bgColor = ImColor(207, 255, 204, 255);
					}
					else if (handleType->MatchesHash(CRC64_CaseInsensitive(0, "Scene"))) {
						bgColor = ImColor(244, 186, 247, 255);
					}
				}
				//dont change brmright and topleft for rest of this nested loop
				painter->AddRectFilled(topLeft, btmRight, bgColor);
				painter->AddRect(topLeft, btmRight, ImColor(58, 58, 58, 255));
				bool userHasSelected = data.pInspectingResource != 0;
				for(int k=0; k < pResource->mBlocks.mSize; k++){
					ChoreResource::Block& pBlock = pResource->mBlocks[k];

					if (pBlock.mStartTime > data.viewRange[1] && pBlock.mEndTime < data.viewRange[0])
						continue;//out of view
					float lerpToRange = (btmRight.x - topLeft.x);
					float startX = (pBlock.mStartTime - data.viewRange[0]) / range;
					float endX = (pBlock.mEndTime - data.viewRange[0]) / range;
					startX *= lerpToRange;
					endX *= lerpToRange;

					//dont go over dropdown arrow nextto resource
					if (startX < 24.0f)
						startX = 24.0f;
					if (endX < 24.0f)
						endX = 24.0f;
					if (startX == endX)
						continue;

					//FOR NOW. Show blocks 
					ImVec2 tl = ImVec2{ topLeft.x + startX - 24.0f, topLeft.y + 3};
					ImVec2 br = ImVec2{ topLeft.x + endX - 24.0f, btmRight.y - 3};

					if (!data.mbOpenPopup && !userHasSelected && Inside(tl, br, ImGui::GetMousePos()) && ImGui::IsKeyReleased(ImGuiKey_I) && ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
						data.pInspectingResource = pResource;
						data.inspectingBlockIndex = k;
						userHasSelected = true;
					}
					else if (!data.mbOpenPopup && !userHasSelected && Inside(tl, br, ImGui::GetMousePos()) && ImGui::IsKeyReleased(ImGuiKey_D) && MessageBoxA(0,"Are your sure you want to delete this block?","?",MB_YESNO|MB_ICONQUESTION)==IDYES) {
						data.pInspectingResource = 0;
						data.inspectingBlockIndex = -1;//in case
						userHasSelected = true;//anything else next frame
						pResource->mBlocks.RemoveElement(k);
						break;//next loop worry about it
					}

					painter->AddRectFilled(tl, br, pBlock.mbLoopingBlock ? ImColor(252, 33, 55, 100) : ImColor(112, 250, 248, 100), 0.7f);
					painter->AddRect(tl, br, ImColor(58, 58, 58, 255), 0.7f);
				}
				if (res.empty())
					res = "<??>";
				float width = ImGui::GetFont()->CalcTextSizeA(13.f, FLT_MAX, 0, res.c_str(), res.c_str() + res.length(), 0).x;
				painter->AddText(0, 13, topLeft + ImVec2(0, 16), ImColor{ 50,50,50,255 }, aPriority, aPriority + strlen(aPriority));
				painter->AddText(0, 13, ImVec2(btmRight.x - width - 50, topLeft.y + 16), ImColor{ 50,50,50,255 }, res.c_str(), res.c_str() + res.length());

				if(!data.mbOpenPopup && !userHasSelected && Inside(btmRight, topLeft, ImGui::GetMousePos()) && ImGui::IsKeyReleased(ImGuiKey_I)){
					data.pInspectingResource = pResource;
					data.inspectingBlockIndex = -1;
				}
				else if (!data.mbOpenPopup && !userHasSelected && Inside(btmRight, topLeft, ImGui::GetMousePos()) && ImGui::IsKeyReleased(ImGuiKey_A)) {
					data.pInspectingResource = pResource;
					data.inspectingBlockIndex = pResource->mBlocks.mSize;
					float timeStartMid = ((ImGui::GetMousePos().x - topLeft.x) / (btmRight.x - topLeft.x)) * range;
					//make dummy size 1 second (0.5 ech way)
					float startTime = timeStartMid - 0.5f;
					float endTime = timeStartMid + 0.5f;
					if(startTime < 0.f){
						startTime = 0.f;
						endTime = 1.0f;
					}
					ChoreResource::Block block{};
					block.mStartTime = startTime;
					block.mEndTime = endTime;
					block.mScale = 1.0f;
					block.mbLoopingBlock = false;
					block.mbSelected = false;//unused
					pResource->mBlocks.AddElement(0, 0, &block);
				}

				y += 32.f;
				topLeft = topLeft + ImVec2(-24.f, 32.f);
			}
		}
		if(bFinalUnusedLoop){
			data.mpChore->mNumAgents--;
			break;
		}
		if(i == data.mpChore->mNumAgents - 1){
			data.mpChore->mNumAgents++;
			bFinalUnusedLoop = true;
		}
	}

	// END OF CHORE LINE

	if (data.mpChore->mLength > data.viewRange[0] && data.mpChore->mLength < data.viewRange[1]) {
		float pos = C.x + windowSize.x * ((data.mpChore->mLength - data.viewRange[0]) / range);
		painter->AddLine(ImVec2(pos, C.y), ImVec2(pos, C.y + data.totalHeight), ImColor(209, 53, 10, 255), 2.0);
	}

	ImGui::SetCursorPosY(y + initialY + 50.f);
	data.totalHeight = ImGui::GetCursorPosY();
	if (data.totalHeight < ImGui::GetWindowSize().y + C.y)
		data.totalHeight = ImGui::GetWindowSize().y + C.y;

	// MOUSE SCROLL. scroll 1 second with each one.

	if (!data.mbOpenPopup && ImGui::IsKeyDown(ImGuiKey_LeftShift)) {

		float deltaWheel = ImGui::GetIO().MouseWheel;
		if (deltaWheel >= 1.0f) {
			data.viewRange[0]++;
			data.viewRange[1]++;
		}
		else if (deltaWheel <= 1.f) {
			if (data.viewRange[0] + deltaWheel >= 0.0f && data.viewRange[1] + deltaWheel >= 0.f) {
				data.viewRange[0] += deltaWheel;
				data.viewRange[1] += deltaWheel;
			}
		}

	}

	// UPDATE LENGTH

	for(int i = 0; i < data.mpChore->mNumResources; i++){
		for(int j = 0; j < data.mpChore->mPtrResources[i]->mBlocks.GetSize(); j++){
			ChoreResource::Block& pBlock = data.mpChore->mPtrResources[i]->mBlocks[j];
			if (!pBlock.mbSelected && pBlock.mEndTime > choreLengthUpdate)
				choreLengthUpdate = pBlock.mEndTime;
		}
	}

	if(choreLengthUpdate > data.mpChore->mLength)
		data.mpChore->mLength = choreLengthUpdate;//chore length can be longer than the end time of the last block

}

template<>
void RenderTypeData<Chore>(TypeTask<Chore>* pChore, bool bInit) {
	if (bInit) {
		pChore->pUserData = new ChoreUserData;
		ChoreUserData& data = *(ChoreUserData*)pChore->pUserData;
		data.depends = std::make_unique<AnyTypeEditor>();
		data.mpChore = pChore->GetType();
		data.mpTask = pChore;
		data.myAliveSlot = sRuntime.gen_gate_slot();
		data.dummyUnused.mAgentName = "**Unused Chore Resources**";
		data.dummyUnused.mResources.Resize(data.mpChore->mNumResources);
		data.UpdateUnusedResource();
	}
	else {
		ChoreUserData& data = *(ChoreUserData*)pChore->pUserData;
		Chore* chore = data.mpChore;
		if (!data.resolvedDone) {
			data.resolvedDone = true;
			for (int i = 0; i < chore->mNumResources; i++) {
				ChoreResource* res = chore->mPtrResources[i];
				char result[1024];
				result[0] = 0;
				TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(0, res->mResName.GetCRC(), result);
				if (*result)
					data.add(std::string(result));
			}
			for (int i = 0; i < chore->mNumAgents; i++) {
				data.add(*chore->mPtrAgents[i]->GetAgentName());
				data.add(*chore->mPtrAgents[i]->GetAgentName() + " blocking");
				data.add(*chore->mPtrAgents[i]->GetAgentName() + " path blocking");
				data.add(*chore->mPtrAgents[i]->GetAgentName() + " lipsync");
				data.add(*chore->mPtrAgents[i]->GetAgentName() + " voice");
			}
		}
		TelltaleToolLib_SetBlowfishKey(pChore->id);
		Chore_RenderWindow(data);
	}

}