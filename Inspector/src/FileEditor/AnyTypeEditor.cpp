#include "ToolLibrary/Meta.hpp"
#include "imgui.h"
#include "../TelltaleInspector.h"
#include <sstream>
#include "ToolLibrary/Types/D3DMesh.h"
#include "../nfd.h"
#include "imgui_internal.h"

// Base UI for PROPS and any type viewer with ImGui

bool AnyTypeEditor::Math(u64 hash, void* pInst) {
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
		Math(quat_str, &transform->mRot);
		ImGui::Text("Translation:");
		Math(vec3_str, &transform->mTrans);
	}
	else if (hash == col_str) {
		Color* transform = (Color*)pInst;
		ImGui::Text("RGBA:");
		Math(vec4_str, transform);
	}
	else if (hash == sphere_str) {
		Sphere* s = (Sphere*)pInst;
		ImGui::Text("Center:");
		Math(vec3_str, &s->mCenter);
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

bool AnyTypeEditor::modify_handle(MetaClassDescription* desc, Symbol& sym, const char* key, std::string* pOutResolved, std::vector<std::string>* pExtraPossibleValues) {
	if (key == 0)
		key = desc->mpTypeInfoName;
	ImGui::OpenPopup(key);
	if (ImGui::BeginPopupModal(key)) {
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
				}else if(pOutResolved){
					*pOutResolved = resolve_buf;
				}
			}
			else
				mh_inputField = "";
		}
		if ((mh_inputField.empty() || mh_inputField._Starts_with("Symbol<")) && pExtraPossibleValues != 0) {
			for (auto it = pExtraPossibleValues->begin(); it != pExtraPossibleValues->end(); it++) {
				if (CRC64_CaseInsensitive(0, it->c_str()) == sym.GetCRC()) {
					mh_inputField = *it;
					break;
				}
			}
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
		if ((em || ends_with(mh_inputField, mh_handleRefType->mpExt)) && ImGui::Button("Done")) {
			sym.SetCRC(CRC64_CaseInsensitive(0, mh_inputField.c_str()));
			possible.clear();
			if (pOutResolved) {
				*pOutResolved = std::move(mh_inputField);
			}
			mh_handleRefType = nullptr;
			mh_inputField.clear();
			mh_comboitem = nullptr;
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			mh_item.SetCRC(0);
			mh_uid = 0;
			return true;
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
		ImGui::EndPopup();
	}
	else return true;
	return false;
}

//stores in prop key user database of hashes
void AnyTypeEditor::modify_symbol(Symbol& sym, const char* key, void* inst, const char* res, std::string* pOutResolved, std::vector<std::string>* pExtraPossibleValues) {
	ImGui::OpenPopup(key);
	std::string t{};
	if (ImGui::BeginPopupModal(key)) {
		if (mod_sym_inst != inst) {
			//setup
			mod_sym_nf = false;
			mod_sym_inst = inst;
			modify_symbol_field = "";
			mod_sym_final = "";
			if (res != 0) {
				mod_sym_final = modify_symbol_field = res;
			}
			else {
				std::string* pResolved = SymMap_ResolveHash(resolve_buf, sym_map, fn_map, sym.GetCRC(), check_all);
				if (!pResolved) {
					if (pExtraPossibleValues != 0) {
						for (auto it = pExtraPossibleValues->begin(); it != pExtraPossibleValues->end(); it++) {
							if (CRC64_CaseInsensitive(0, it->c_str()) == sym.GetCRC()) {
								t = *it;
								pResolved = &t;
								break;
							}
						}
					}
				}
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
			if (pOutResolved)
				*pOutResolved = mod_sym_final;
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

//returns 1 if user wants to create new element, 2 to remove
int AnyTypeEditor::ListTree(const char* prefix, int uid, void* data, void* (*getnext)(void* dat, void* prev, int index), int num_elem, MetaClassDescription* pDesc, int* selected)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	int ret = 0;
	u64 to_del = 0xFFFF'FFFFL;;
	std::string knb{};
	char knbr[17]{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
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

		if (node_open)
		{
			void* pitem = 0;
			for (int i = 0; i < num_elem; i++) {
				std::snprintf(knbr, 16, "[%d]", i);
				knb = knbr;
				pitem = getnext(data, pitem, i);
				bool hov{ false };
				TreeItem(pDesc, pitem, &knb, uid, to_del, i, true, hov, i);
				if (hov)
					*selected = i;
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
		if (to_del < 0xFFFF'FFFFL) {
			ret = 2;
			*selected = (int)(to_del & 0x7FFF'FFFF);
		}
		else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_N)) {
			ret = 1;
		}
		ImGui::EndTable();
	}
	ImGui::PopStyleVar();
	return ret;
}

void AnyTypeEditor::MapTreeKeyInput(MetaClassDescription* k, void* obj, bool reset) {
	static std::string symbol_in{};
	if (reset) {
		if (k->mHash == hash_sym) {
			u64 crc;
			if (((Symbol*)obj)->GetCRC() == 0)
				((Symbol*)obj)->SetCRC(crc = CRC64_CaseInsensitive(0, symbol_in.c_str()));
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
			ImGui::InputText("##str", &symbol_in);
			ImGui::Text("Or enter raw decimal hash (used if non zero):");
			ImGui::InputScalar("##u64", ImGuiDataType_U64, obj);
		}
	}
}

bool AnyTypeEditor::MapTreeKey(MetaClassDescription* k, std::string* dest, void* obj) {
	char bf[32];
	if (k->mHash == hash_str) {
		if (!dest)
			return true;
		String* pStr = (String*)obj;
		*dest = pStr->c_str();
		return true;
	}
	else if (k->mHash == hash_uint32) {
		if (!dest)
			return true;
		unsigned int* intval = (unsigned int*)obj;
		sprintf(bf, "%d", *intval);
		*dest = bf;
		return true;
	}
	else if (k->mHash == hash_long || k->mHash == hash_int32 || k->mHash == hash_int) {
		if (!dest)
			return true;
		int* intval = (int*)obj;
		sprintf(bf, "%d", *intval);
		*dest = bf;
		return true;
	}
	else if (k->mHash == hash_sym) {
		if (dest) {
			u64 sym = ((Symbol*)obj)->GetCRC();
			sprintf(bf, "Symbol<%llx>", sym);
			*dest = bf;
		}
		return true;
	}
	return false;
}

void AnyTypeEditor::TypeTree(const char* prefix, int uid, MetaClassDescription* type, void* value) {
	// Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
	ImGui::PushID(uid);

	// Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	bool node_open = ImGui::TreeNode(prefix);
	ImGui::TableSetColumnIndex(1);
	std::string kn{};
	u64 del{ 0xFFFF'FFFFL };
	if (node_open)
	{
		int i = 0;
		for (MetaMemberDescription* mem = type->mpFirstMember; mem != nullptr; mem = mem->mpNextMember) {
			kn = mem->mpName;
			bool hov{};
			TreeItem(mem->mpMemberDesc, (char*)value + mem->mOffset, &kn, uid, del, i++, false, hov, 0, mem, true);
		}
		ImGui::TreePop();
	}
	int ret = 0;
	ImGui::PopID();
}
//returns 1 if user wants to create new element, 2 to remove
int AnyTypeEditor::MapTree(const char* prefix, int uid, int* selected, Map<void*, void*>* pProxyMap)
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

	u64 to_del = 0xFFFF'FFFFL;
	std::string knb{};

	if (node_open)
	{
		void* pitem = 0;
		for (int i = 0; i < pProxyMap->GetSize(); i++) {
			MapTreeKey(pProxyMap->GetContainerKeyClassDescription(), &knb, pProxyMap->GetKey(i));
			pitem = pProxyMap->GetVal(i);
			bool hov{ false };
			TreeItem(pProxyMap->GetContainerDataClassDescription(), pitem, &knb, uid, to_del, i, true, hov, i);
			if (hov)
				*selected = i;
		}
		ImGui::TreePop();
	}
	int ret = 0;
	ImGui::PopID();
	if (to_del < 0xFFFF'FFFFL) {
		ret = 2;
		*selected = (int)(to_del & 0x7FFF'FFFF);
	}
	else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_N)) {
		ret = 1;
	}
	return ret;
}

static void* DCArray_PropGetter(void* array, void*, int i) {
	MetaClassDescription* clazz = ((DCArray<void*>*)(array))->GetContainerDataClassDescription();
	return ((DCArray<void*>*)(array))->GetElement(i);//can use this as its virtual
}

static void* SArray_PropGetter(void* array, void*, int i) {
	MetaClassDescription* clazz = ((SArray<void*, 1>*)(array))->GetContainerDataDescription();
	return (void*)((char*)((SArray<void*, 1>*)(array))->mData + i * clazz->mClassSize);
}

extern ImGuiContext* GImGui;

void AnyTypeEditor::TreeItem(MetaClassDescription* clazz, void* pRawData, std::string* kn, int uid, u64& to_del, int i, bool bAllowDel, bool& hovered, u64 keyHash, MetaMemberDescription* memCheck, bool disbleMemCheck) {
	if (memCheck && !disbleMemCheck && Meta_IsMemberDisabled(memCheck, 6)) {//only version 6 here older wont be used
		return;
	}
	if (true) {
		if (pRawData && clazz) {
			ImGui::PushID(i);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_OpenOnArrow;
			u64 typeHash = clazz->mHash;
			u64 hash = typeHash;//alias
			void* pInst = pRawData;//alias
			if (ImGui::TreeNodeEx(kn->c_str(), flags)) {
				hovered = ImGui::LastHovered();
				if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
					ImGui::SetTooltip("Type: %s", clazz->mpTypeInfoName);
				ImGui::TableSetColumnIndex(1);

				if (IsType(clazz, "PropertySet")) {
					PropTree((PropertySet*)pRawData, kn->c_str(), (int)pRawData & 0x7FFFFFF);
					if (hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
						ImGui::SetTooltip("Type: %s", clazz->mpTypeInfoName);
				}
				else if (starts_with("DCArray", clazz->mpTypeInfoName)) {
					DCArray<void*>* pProxyArray = (DCArray<void*>*)pRawData;
					MetaClassDescription* pElementType = pProxyArray->GetContainerDataClassDescription();
					if (pElementType == nullptr)
						goto enum_string;
					int selected = 0;
					int result = ListTree(kn->c_str(), (int)pRawData & 0x7FFFFFF, pProxyArray, &DCArray_PropGetter, pProxyArray->GetSize(), pElementType, &selected);
					if (hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
						ImGui::SetTooltip("(DC) Array of: %s", pElementType->mpTypeInfoName);
					else if (result != 2)
						result = 0;
					if (result == 1) {
						MetaClassDescription* pDescription = pElementType;
						if (!(pDescription->mpVTable[0]) || !(pDescription->mpVTable[2]) || !(pDescription->mpVTable[3])) {
							MessageBoxA(0, "The type you selected is not a concrete type (ie its an abstract class for programmers) so cannot be instantiated!", "!!", MB_ICONINFORMATION);
						}
						else {
							void* tmpobj = operator new(pDescription->mClassSize);
							if (tmpobj) {
								pDescription->Construct(tmpobj);
								pProxyArray->AddElement(0, 0, tmpobj);
								pDescription->Destroy(tmpobj);
								operator delete(tmpobj);
							}
							else {
								MessageBoxA(0, "Could not allocate new element", "Error", MB_ICONERROR);
							}
						}
					}
					else if (result == 2) {
						pProxyArray->RemoveElement(selected);
					}
				}
				else if (starts_with("SArray", clazz->mpTypeInfoName)) {
					SArray<void*, 1>* pProxyArray = (SArray<void*, 1>*)pRawData;
					MetaClassDescription* pElementType = pProxyArray->GetContainerDataDescription();
					if (pElementType == nullptr)
						goto enum_string;
					int selected = 0;
					int result = ListTree(kn->c_str(), (int)pRawData & 0x7FFFFFF, pProxyArray, &SArray_PropGetter, pProxyArray->NUM_DATA_ELEM, pElementType, &selected);
					if (hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
						ImGui::SetTooltip("(S) Array of: %s", pElementType->mpTypeInfoName);
				}
				else if (starts_with("Map", clazz->mpTypeInfoName)) {
					Map<void*, void*>* pProxyArray = (Map<void*, void*>*)pRawData;
					if (MapTreeKey(pProxyArray->GetContainerKeyClassDescription(), 0, 0)) {
						int selected = 0;
						int result = MapTree(kn->c_str(), (int)pRawData & 0x7FFFFFF, &selected, pProxyArray);
						if (hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped)) {
							ImGui::SetTooltip("Map of: %s => %s", pProxyArray->GetContainerKeyClassDescription()->mpTypeInfoName, pProxyArray->GetContainerDataClassDescription()->mpTypeInfoName);
						}
						else if(result != 2)
							result = 0;
						if (result == 1) {
							ignore_ctrln = true;
							MetaClassDescription* pDescriptionK = pProxyArray->GetContainerKeyClassDescription(), * pDescriptionV = pProxyArray->GetContainerDataClassDescription();
							if (!(pDescriptionK->mpVTable[0]) || !(pDescriptionK->mpVTable[2]) || !(pDescriptionK->mpVTable[3])
								|| !(pDescriptionV->mpVTable[0]) || !(pDescriptionV->mpVTable[2]) || !(pDescriptionV->mpVTable[3])) {
								MessageBoxA(0, "The key or value is not a concrete type (ie its an abstract class for programmers) so cannot be instantiated!", "!!", MB_ICONINFORMATION);
							}
							else {
								tmpkey = pDescriptionK->New();
								if (!tmpkey) {
									MessageBoxA(0, "Could not allocate new element key", "Error", MB_ICONERROR);
								}
								ImGui::OpenPopup("New Mapping");
							}
						}
						else if (result == 2) {
							pProxyArray->RemoveElement(selected);
						}
						if (tmpkey != 0 && ImGui::BeginPopupModal("New Mapping")) {
							ImGui::Text("Enter Mapping Key:");
							MapTreeKeyInput(pProxyArray->GetContainerKeyClassDescription(), tmpkey);
							MetaClassDescription* pDescriptionK = pProxyArray->GetContainerKeyClassDescription(), * pDescriptionV = pProxyArray->GetContainerDataClassDescription();
							if (ImGui::Button("Create")) {
								void* tmpobj = pDescriptionV->New();
								MapTreeKeyInput(pDescriptionK, tmpkey, true);
								if (tmpobj) {
									pDescriptionV->Construct(tmpobj);
									((ContainerInterface*)pProxyArray)->AddElement(0, tmpkey, tmpobj);
									pDescriptionV->Delete(tmpobj);
									pDescriptionK->Delete(tmpkey);
								}
								else {
									pDescriptionK->Delete(tmpkey);
									MessageBoxA(0, "Could not allocate new element value", "Error", MB_ICONERROR);
								}
								tmpkey = tmpobj = 0;
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::Button("Exit")) {
								if(tmpkey!=0)
									pDescriptionK->Delete(tmpkey);
								tmpkey = 0;
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
					}
					else {
						ImGui::Text("**MAP KEY EDITABLE SOON**");
						ImGui::TreePop();
						ImGui::PopID();
						return;
					}
				}else if ((disbleMemCheck && memCheck && memCheck->mpEnumDescriptions) || (clazz->mFlags.mFlags & MetaFlag_EnumWrapperClass || starts_with("Enum", clazz->mpTypeInfoName)
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
				}
				else {
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
					else if (!Math(typeHash, pInst)) {
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
						else if (!strcmp("AnimOrChore", clazz->mpTypeInfoName)) {
							if (ImGui::Button("Edit Anim Or Chore")) {
								ImGui::OpenPopup("Animation or Chore");
							}
							if (ImGui::BeginPopupModal("Animation or Chore")) {
								AnimOrChore* val = (AnimOrChore*)pRawData;
								if (val->mhAnim.mHandleObjectInfo.mObjectName.GetCRC() == 0 && val->mhChore.mHandleObjectInfo.mObjectName.GetCRC() == 0) {
									ImGui::Text("No Animation or Chore. Click to set either animation or chore");
									if (ImGui::Button("Set Chore"))
										ImGui::OpenPopup("Chore Handle");
									if (ImGui::BeginPopupModal("Chore Handle")) {
										ImGui::Text("Chore");
										ImGui::SameLine();
										if (ImGui::Button("Modify Handle Value") && mh_uid == 0) {
											mh_uid = uid;
											mh_item = Symbol{ CRC64_CaseInsensitive(0, kn->c_str()) };
										}
										else if (mh_uid == uid && mh_item == Symbol{ CRC64_CaseInsensitive(0, kn->c_str()) }) {
											modify_handle(GetMetaClassDescription<Chore>(), val->mhChore.mHandleObjectInfo.mObjectName);
										}
										if (ImGui::Button("Exit"))
											ImGui::CloseCurrentPopup();
										ImGui::EndPopup();
									}
									if (ImGui::Button("Set Animation"))
										ImGui::OpenPopup("Anim Handle");
									if (ImGui::BeginPopupModal("Anim Handle")) {
										ImGui::Text("Animation");
										ImGui::SameLine();
										if (ImGui::Button("Modify Handle Value") && mh_uid == 0) {
											mh_uid = uid;
											mh_item = Symbol{ CRC64_CaseInsensitive(0, kn->c_str()) };
										}
										else if (mh_uid == uid && mh_item == Symbol{ CRC64_CaseInsensitive(0, kn->c_str()) }) {
											modify_handle(GetMetaClassDescription<Animation>(), val->mhChore.mHandleObjectInfo.mObjectName);
										}
										if (ImGui::Button("Exit"))
											ImGui::CloseCurrentPopup();
										ImGui::EndPopup();
									}
								}
								else {
									if (val->mhChore.mHandleObjectInfo.mObjectName.GetCRC() != 0) {
										if (ImGui::Button("Set Chore"))
											ImGui::OpenPopup("Chore Handle");
										if (ImGui::BeginPopupModal("Chore Handle")) {
											ImGui::Text("Chore");
											ImGui::SameLine();
											if (ImGui::Button("Modify Handle Value") && mh_uid == 0) {
												mh_uid = uid;
												mh_item = Symbol{ CRC64_CaseInsensitive(0, kn->c_str()) };
											}
											else if (mh_uid == uid && mh_item == Symbol{ CRC64_CaseInsensitive(0, kn->c_str()) }) {
												modify_handle(GetMetaClassDescription<Chore>(), val->mhChore.mHandleObjectInfo.mObjectName);
											}
											if (ImGui::Button("Exit"))
												ImGui::CloseCurrentPopup();
											ImGui::EndPopup();
										}
									}
									else {
										if (ImGui::Button("Set Animation"))
											ImGui::OpenPopup("Anim Handle");
										if (ImGui::BeginPopupModal("Anim Handle")) {
											ImGui::Text("Animation");
											ImGui::SameLine();
											if (ImGui::Button("Modify Handle Value") && mh_uid == 0) {
												mh_uid = uid;
												mh_item = Symbol{ CRC64_CaseInsensitive(0, kn->c_str()) };
											}
											else if (mh_uid == uid && mh_item == Symbol{ CRC64_CaseInsensitive(0, kn->c_str()) }) {
												modify_handle(GetMetaClassDescription<Animation>(), val->mhAnim.mHandleObjectInfo.mObjectName);
											}
											if (ImGui::Button("Exit"))
												ImGui::CloseCurrentPopup();
											ImGui::EndPopup();
										}
									}
								}
								if (ImGui::Button("Reset Handles")) {
									val->mhChore.mHandleObjectInfo.mObjectName.SetCRC(0);
									val->mhAnim.mHandleObjectInfo.mObjectName.SetCRC(0);
								}
								if (ImGui::Button("Exit"))
									ImGui::CloseCurrentPopup();
								ImGui::EndPopup();
							}
						}
						else if (!strcmp("T3MaterialData", clazz->mpTypeInfoName)) {
							T3MaterialData* mat = (T3MaterialData*)pRawData;
							if (ImGui::Button("Edit Material Data")) {
								ImGui::OpenPopup("Material Data (T3MaterialData)");
							}
							if (ImGui::BeginPopupModal("Material Data (T3MaterialData)")) {
								bool node_open = ImGui::BeginTable("t3matdat", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable);
								if (node_open) {
									std::string n{};
									bool h{};
									ImGui::TableNextRow();
									TreeItem(GetMetaClassDescription<Symbol>(), &mat->mMaterialName, &(n = "Material Name"), 6900, to_del, i, false, h, 0, clazz->GetMemberDescription("mMaterialName"));
									ImGui::TableNextRow();
									TreeItem(GetMetaClassDescription<Symbol>(), &mat->mRuntimePropertiesName,
										&(n = "Runtime Properties Name"), 6901, to_del, i + 1, false, h, 0, clazz->GetMemberDescription("mRuntimePropertiesName"));
									ImGui::TableNextRow();
									TreeItem(GetMetaClassDescription<Symbol>(), &mat->mLegacyRenderTextureProperty, &(n = "Legacy Render Texture Property"),
										6902, to_del, i + 2, false, h, 0, clazz->GetMemberDescription("mLegacyRenderTextureProperty"));
									ImGui::TableNextRow();
									TreeItem(GetMetaClassDescription<Symbol>(), &mat->mLegacyBlendModeRuntimeProperty,
										&(n = "Legacy Blend Mode Property"), 6903, to_del, i + 3, false, h, 0, clazz->GetMemberDescription("mLegacyBlendModeRuntimeProperty"));
									ImGui::TableNextRow();
									TreeItem(GetMetaClassDescription<long>(), &mat->mDomain, &(n = "Domain"), 6904, to_del, i + 4, false, h, 0, clazz->GetMemberDescription("mDomain"));
									ImGui::TableNextRow();
									TreeItem(GetMetaClassDescription<long>(), &mat->mVersion, &(n = "Version"), 6905, to_del, i + 5, false, h, 0, clazz->GetMemberDescription("mVersion"));

									ImGui::TableNextRow();
									TreeItem(GetMetaClassDescription<DCArray<T3MaterialRuntimeProperty>>(), &mat->mRuntimeProperties, &(n = "Runtime Properties"), 6906, to_del,
										i + 6, false, h, 0, clazz->GetMemberDescription("mRuntimeProperties"));

									ImGui::TableNextRow();
									TreeItem(GetMetaClassDescription<DCArray<T3MaterialCompiledData>>(), &mat->mCompiledData2, &(n = "Compiled Data"), 6907, to_del,
										i + 7, false, h, 0, clazz->GetMemberDescription("mCompiledData2"));

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
								}
								else {
									MetaEnumDescription* desc = pEnum->mpEnumDescriptions;
									while (desc) {
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
						}
						else if (starts_with("BitSetBase", clazz->mpTypeInfoName)) {
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
						}
						else if (!strcmp("Flags", clazz->mpTypeInfoName)) {
							void* hash = (void*)(CRC64(0, kn->c_str()) ^ (u64)pRawData ^ (u64)clazz);
							ImGui::PushID(hash);
							if (ImGui::Button("Edit Bit Flags"))
								ImGui::OpenPopup("Flags");
							if (ImGui::BeginPopupModal("Flags")) {
								char b[10];
								u32& flags = ((Flags*)pRawData)->mFlags;
								if (disbleMemCheck && memCheck && memCheck->mpFlagDescriptions) {
									MetaFlagDescription* flagDesc = memCheck->mpFlagDescriptions;
									while (flagDesc) {
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
							std::string popupID = std::string(clazz->mpTypeInfoName) + "###" +std::to_string((std::uint64_t)hash);
							if (ImGui::Button("Edit Raw Data"))
								ImGui::OpenPopup(popupID.c_str());
							if (ImGui::BeginPopupModal(popupID.c_str())) {
								ImGui::Text("For lists/sets/maps: when the tooltip appears, Ctrl+N to add new mapping, Ctrl+D to delete hovered.");
								ImGui::Text("NOTE! The tooltip must display 'Map of' or 'Array of' when hovering to be able to create/delete!");
								if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
								{
									TypeTree("Type Class Members", uid + 1, clazz, pRawData);
									ImGui::EndTable();
								}
								if (ImGui::Button("Close")) {
									ImGui::CloseCurrentPopup();
								}
								ImGui::EndPopup();
							}
							ImGui::PopID();
						}
					}
				}
				if (bAllowDel && ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_D) && hovered) {
					to_del = keyHash;
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}
}

void AnyTypeEditor::PropTree(PropertySet* set, const char* prefix, int uid)
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

	u64 to_del = 0xFFFF'FFFFL;

	if (node_open)
	{
		for (int i = 0; i < set->mKeyMap.mSize; i++) {
			PropertySet::KeyInfo* m = set->mKeyMap.mpStorage + i;
			if (m->mpValue) {
				std::string* kn;
				std::string* keyName = SymMap_ResolveHash(resolve_buf, sym_map, fn_map, m->mKeyName.GetCRC(), prop_key);
				if (keyName == nullptr) {
					kn = &(sym_map[m->mKeyName.GetCRC()] = to_symbol(m->mKeyName.GetCRC()));
				}
				else
					kn = keyName;
				bool hovered{ false };
				TreeItem(m->mpValue->mpDataDescription, m->mpValue->mpValue, kn, uid, to_del, i, true, hovered, m->mKeyName.GetCRC());
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
						if (is_selected)
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
				if (ImGui::Button("Exit")) {
					nprop_field = "";
					nprop_type = "String";
					cur_new_parent = nullptr;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_N) && frame_check_addrem && !ignore_ctrln && !tmpkey) {
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
	if (to_del > 0xFFFF'FFFFL) {
		set->RemoveProperty(to_del);
	}
}

void AnyTypeEditor::Render(const char* unqLocalID, MetaClassDescription* pType, void* pInstance){
	std::string wndtitle{ unqLocalID };
	frame_check_addrem = true;
	u64 ignore{};
	bool ignoreHover{};
	if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
	{
		TreeItem(pType, pInstance, &wndtitle, 0, ignore, 0, false, ignoreHover, 0);
		ImGui::EndTable();
	}
}