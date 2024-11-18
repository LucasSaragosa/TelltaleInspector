#include "imgui.h"
#include "../InspectorTemplate.h"
#include "../TelltaleInspector.h"
#include <intrin.h>
#include <filesystem>
#include "ToolLibrary/FMOD/FSB5.h"
#include "ToolLibrary/MetaStream_JSON.hpp"
#include "ToolLibrary/VersDB.h"
#include "ToolLibrary/LegacyTypes/D3DMeshLegacy.h"
#include <unordered_map>
#include "../stb/std_image_write.h"
#include "../squish/squish.h"
#include "../imstd/imgui_memedit.h"
#include "ToolLibrary/Types/Scene.h"
#include "ToolLibrary/Types/AnimOrChore.h"
#include "ToolLibrary/Base64.h"
#include "ToolLibrary/T3/T3EffectCache.h"
#include "ToolLibrary/Types/Dialog.h"
#include <imgui_node_editor.h>

namespace ed = ax::NodeEditor;

struct Node {
	int nativeIndex = 0;//index into array inside dialog
	int prev = 0, next = 0;
	ed::PinId myid{};
};

struct DlgUserData : TypeUserData
{

	Dlg* mpDialog = 0;
	ed::EditorContext* mContext = 0;

	virtual ~DlgUserData() {
		ed::DestroyEditor(mContext);
		mContext = 0;
	}

	std::vector<Node> nodes;

	int alloc_node(const char* clazz) {
		int ret = (int)nodes.size();
		nodes.push_back(Node{});
		u64 hash = CRC64_CaseInsensitive(0, clazz);
		MetaClassDescription* pDesc = TelltaleToolLib_FindMetaClassDescription_ByHash(hash);
		if(!pDesc){
			MessageBoxA(0, "Node class not found", clazz, MB_ICONERROR);
			std::abort();
			return -1;
		}
		void* pInst = mpDialog->new_helper(sizeof(pDesc->mClassSize));
		if(!pInst){
			MessageBoxA(0, "Node could not be allocated on heap", clazz, MB_ICONERROR);
			std::abort();
			return -1;
		}
		pDesc->Construct(pInst);
		nodes[ret].nativeIndex = mpDialog->mNodes.GetSize();
		mpDialog->mNodes.AddElement(0, 0, &pInst);
		((DlgNode*)mpDialog->mNodes.GetElement(nodes[ret].nativeIndex))->miUniqueID = mpDialog->GetNextUniqueID(1);
		return ret;
	}

	DlgNode* node_at(int non_native_index)
	{
		return *(DlgNode**)mpDialog->mNodes.GetElement(nodes[non_native_index].nativeIndex);
	}

};

void RenderTree(DlgChildSet* pSet, DlgUserData& data, int& id){
	for (int j = 0; j < pSet->mChildren.GetSize(); j++) {
		DlgChild* pChild = pSet->mChildren[j];
		if(ImGui::TreeNodeEx("##", ImGuiTreeNodeFlags_None, "DlgChildSymbol<%llX>: %s", pChild->mName.GetCRC(), pChild->GetMetaClassDescription()->mpTypeInfoName)){
			try {
				DlgChildSet* pSet1 = dynamic_cast<DlgChildSet*>(pChild);
				if(pSet1){
					RenderTree(pSet1, data, id);
				}
			}catch(_STD exception){}
			ImGui::TreePop();
		}
	}
}

template<>
void RenderTypeData<Dlg>(TypeTask<Dlg>* pDialog, bool bInit) {
	if (bInit) {
		pDialog->pUserData = new DlgUserData{};
		DlgUserData& data = *(DlgUserData*)pDialog->pUserData;
		data.mContext = ed::CreateEditor();
	}
	else {
		DlgUserData& data = *(DlgUserData*)pDialog->pUserData;
		data.mpDialog = pDialog->GetType();
		ImGui::Text("Name");
		ImGui::SameLine();
		ImGui::InputText("##nm", &pDialog->GetType()->mName);

		if(!bInit && data.mpDialog->mNodes.mSize == 0){
			//init stuff again, make start node
			data.alloc_node("DlgNodeStart");
		}

		if (ImGui::BeginTable("grid_dlg", 2)) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			if (ImGui::TreeNode("Dialog Tree")) {
				int id = 100;
				for(int i = 0; i < data.mpDialog->mFolders.GetSize(); i++){
					DlgFolder* pFolder = data.mpDialog->mFolders[i];
					if (ImGui::TreeNodeEx("##", ImGuiTreeNodeFlags_None, "DlgFolderSymbol<%llX>", pFolder->mName.GetCRC())) {
						RenderTree(pFolder, data, ++id);
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			ImGui::TableSetColumnIndex(1);
			ed::SetCurrentEditor(data.mContext);
			ed::Begin("Dialog Graph", ImVec2(0.0, 0.0f));



			ed::End();
			ed::SetCurrentEditor(nullptr);
			ImGui::EndTable();
		}
	}
}