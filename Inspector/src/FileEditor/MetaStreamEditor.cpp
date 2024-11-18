#include "ToolLibrary/Meta.hpp"
#include "imgui.h"
#include "../TelltaleInspector.h"
#include "../InspectorTemplate.h"
#include <sstream>
#include "../nfd.h"
#include "imgui_internal.h"
#include "ToolLibrary/MetaStream_JSON.hpp"
#include "ToolLibrary/Types/D3DMesh.h"
#include "ToolLibrary/T3/T3Effect.h"

void MetaStreamSubType::ResetChild() {
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

void MetaStreamSubType::Reset() {
	mPersistentData.clear();
	ResetChild();
}

void MetaStreamSubType::Init(MetaStreamTask* pTask, void* pInst, MetaClassDescription* pType) {
	mpTypeDesc = pType;
	mpInstance = pInst;
	mpTask = pTask;
}

const char* MetaStreamSubType::GenNextID() {
	const char* pID = (const char*)&mIDGen;
	sprintf(mID, "##MCD_%llX", CRC64(mBaseIDHash, pID, 8));
	mIDGen++;
	return mID;
}

void MetaStreamSubType::RenderChild(void* pInst, MetaClassDescription* pType, int id, bool* pShouldClose, std::string hdr) {
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
			if (ImGui::BeginPopup(pType->mpTypeInfoName, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
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

void MetaStreamSubType::render_intrin_mcd(const char* mem, MetaClassDescription* mcd, void* pInst, float depth) {
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

void MetaStreamSubType::render_intrin_mcd_enum(const char* mem, MetaClassDescription* mcd, void* pInst, float depth, MetaMemberDescription* pMemDesc) {
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

void MetaStreamSubType::render_intrin_mcd_flags(const char* mem, MetaClassDescription* mcd, void* pInst, float depth, MetaMemberDescription* pMemDesc) {

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

void MetaStreamSubType::render_intrin_mcd_trange(const char* mem, MetaClassDescription* mcd, void* pInst, float depth) {
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

void MetaStreamSubType::render_intrin_mcd_math(const char* mem, MetaClassDescription* mcd, void* pInst, float depth, u64 hash) {
	hash = mcd ? mcd->mHash : hash;
	if (mcd)
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

void MetaStreamSubType::render_mcd_dcarray(const char* mem, MetaClassDescription* mcd, void* pInst, float depth) {
	DCArray<void*>* pDummyArray = (DCArray<void*>*)pInst;
	MetaClassDescription* pArrayType = pDummyArray->GetContainerDataClassDescription();
	u32 numElements = pDummyArray->GetSize();
	void* pArrayData = (void*)pDummyArray->mpStorage;
	render_mcd_array(mem, numElements, pArrayData, pArrayType, depth);
}

void MetaStreamSubType::render_mcd_sarray(const char* mem, MetaClassDescription* mcd, void* pInst, float depth) {
	SArray<void*, 1>* pDummyArray = (SArray<void*, 1>*)pInst;
	MetaClassDescription* pArrayType = pDummyArray->mpContainerDataType;
	u32 numElements = pDummyArray->NUM_DATA_ELEM;
	void* pArrayData = (void*)pDummyArray->mData;
	render_mcd_array(mem, numElements, pArrayData, pArrayType, depth);
}

bool MetaStreamSubType::render_mcd_internal(const char* mem, MetaClassDescription* mcd, void* pInstance, float depth) {
	if (mcd->mbIsIntrinsic || mcd->MatchesHash(CRC64_CaseInsensitive(0, "Symbol")))
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
					if (!pResolved)
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

bool MetaStreamSubType::mesh_cb(void* pd, int idx, const char** pout) {
	//DCArray<HandleUncached>* pArray = (DCArray<HandleUncached>*)pd;
	char* buf = ((char*)pd);
	sprintf(buf, "[%d]", idx);
	*pout = buf;
	return true;
}

void MetaStreamSubType::render_mcd(MetaClassDescription* mcd, void* pInstance, float depth) {
	MetaMemberDescription* pMem = mcd->mpFirstMember;
	if (!render_mcd_internal(mcd->mpTypeInfoName, mcd, pInstance, depth)) {
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
					ImGui::PushID((int)(((Symbol{ mem }.GetCRC()) ^ ((uint64_t)&handles)) & 0xFFFF'FFFF));
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

void MetaStreamSubType::Render(int current_id, float depth) {
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
		if (((size_t)pArrayCh % cAlign) != 0) {
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

	GenNextID();
	MetaClassDescription* pOutType{};
	void* pOutSelected{};
	const char* n{};
	u32 csize{};
	_RenderInspector_1DCol(mcd->mpVTable_ToContainerInterface(pInst), mem, mID, &pOutType, &pOutSelected, &n, &csize, &arr->selected);

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
	_RenderInspector_Map(mcd->mpVTable_ToContainerInterface(pInst), mem, mID, (MetaClassDescription**)&kvt, (void**)&kv, nullptr, &size, &arr->selected);

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
						if (ends_with(pg->mPageName, tmp)) {
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