#include "../TelltaleInspector.h"

struct AnimationUserData : TypeUserData {

	Animation* mpAnimation;

	char resolve_buf[1024];

	std::unordered_map<u64, std::string> db; 
	std::vector<int> dbFilenames;

	AnimationUserData() = default;

};

template<>
void RenderTypeData<Animation>(TypeTask<Animation>* pTask, bool bInit){
	if(bInit){
		pTask->pUserData = new AnimationUserData();
		((AnimationUserData*)pTask->pUserData)->mpAnimation = pTask->GetType();
	}else{
		AnimationUserData& data = *((AnimationUserData*)pTask->pUserData);
		Animation& anim = *data.mpAnimation;
		ImGui::Text("Additive Mask:");
		ImGui::SameLine();
		ImGui::InputFloat("##", &anim.mAdditiveMask);
		std::string* resolved = SymMap_ResolveHash(data.resolve_buf, data.db, data.dbFilenames, anim.mName.GetCRC(), check_all);
		ImGui::Text("Name:");
		ImGui::SameLine();
		if(resolved){
			ImGui::Text(resolved->c_str());
		}else{
			ImGui::Text("Symbol<%llx>", anim.mName.GetCRC());
		}
		ImGui::Separator();
		for(int i = 0; i < anim.mValues.mSize; i++){
			AnimationValueInterfaceBase* pInterface = anim.mValues[i];
			std::string* resolved = SymMap_ResolveHash(data.resolve_buf, data.db, data.dbFilenames, pInterface->mName.GetCRC(), check_all);
			ImGui::Text("Value Name:");
			ImGui::SameLine();
			if (resolved) {
				ImGui::Text(resolved->c_str());
			}
			else {
				ImGui::Text("Symbol<%llx>", anim.mName.GetCRC());
			}
			ImGui::Text("Type: %s", AnimationValueInterfaceBase::GetValueType(pInterface->GetType()));
			ImGui::Separator();
		}
		//TODO actually provide useful help not just the serialized types. 
	}
}