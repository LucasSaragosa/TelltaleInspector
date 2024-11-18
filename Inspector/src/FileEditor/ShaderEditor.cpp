#include "../TelltaleInspector.h"
#include "imgui.h"
#include "../imstd/imgui_stdlib.h"
#include "ToolLibrary/MetaStream_JSON.hpp"
#include "ToolLibrary/Base64.h"
#include "ToolLibrary/T3/T3Effect.h"
#include "ToolLibrary/TTArchive2.hpp"

struct _CmpBucket {

	inline bool operator()(const LoadedBucket& l, const LoadedBucket& r) {
		return l.mBuiltName < r.mBuiltName;
	}

};

struct _CmpPgm {

	inline bool operator()(const LoadedProgram& l, const LoadedProgram& r) {
		return l.mBuildFeatureList.size() < r.mBuildFeatureList.size();
	}

};

static const std::string C_NO_FEATURES = "No Additional Variant Features";

void ShaderTask::SetStream(const char* pGameID, DataStream* pFxStream){
	if(awaitingLoad){
		TTL_Log("Cannot set shader stream: already loading another shader!");
	}
	if (!pFxStream)
		return;
	else {
		mLocalCacheContext.ResetContext();
		mBuckets.clear();
		id = pGameID;
		mLocalCacheContext.SetGameID(pGameID);
		if(!mLocalCacheContext._OK()){
			MessageBoxA(0, "At the moment the Inspector cannot open effect packs from this game!", "Unsupported effect pack version", MB_ICONERROR);
			delete pFxStream;
			return;
		}
		mEditingBucket.newStaticFeat = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature;
		mEditingBucket.newDynamicFeat = mLocalCacheContext.mpVersionDatabase->mFirstDynamicFeature;
		mEditingBucket.newVariantFeatures.SetTether(mLocalCacheContext.mpVersionDatabase);
		mEditingBucket.mParams.mStaticFeatures.SetTether(mLocalCacheContext.mpVersionDatabase);
		mFilterFeatures.SetTether(mLocalCacheContext.mpVersionDatabase);
		mNewFilterFeature = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature;
		if (mpDeleteAfter)
			delete mpDeleteAfter;
		mpDeleteAfter = pFxStream;
		Job j{};
		j.name = "Shader pack load " + name + "";
		j.userA = pFxStream;
		j.userB = (void*)id;
		j.mpUserInstance = this;
		j.priority = 1;
		j.mpFn = &ShaderTask::AsyncDoLoad;
		JobScheduler::Scheduler().PostJob(this, j);
		awaitingLoad.store(true);
		mbWaiting = true;
	}
}

inline T3EffectQuality select_quality_dropdown(T3EffectCacheContext& context, T3EffectQuality last) {
	T3EffectQuality selected = last;
	if (ImGui::BeginCombo("##none", last == eEffectQuality_Default ? "Select" : context.mpVersionDatabase->mQualities[selected].mName)) {
		for (int i = 0; i < context.mpVersionDatabase->mQualities.size(); i++) {//52 an on (wolf among us and newer)
			if (ImGui::Selectable(context.mpVersionDatabase->mQualities[i].mName)) {
				selected = (T3EffectQuality)i;
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	return selected;
}

void select_dropdown(T3RenderStateBlock* maskblock, const char* name, const char* id, T3RenderStateBlock* block, const char** values, int valuesNum, T3RenderStateValue value){
	u32 val = block->GetRenderStateValue(value);
	ImGui::Text(name);
	ImGui::SameLine();
	if (ImGui::BeginCombo(id, values[val])) {
		for (int i = 0; i < valuesNum; i++) {//52 an on (wolf among us and newer)
			if (ImGui::Selectable(values[i])) {
				val = (u32)i;
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	block->SetRenderStateValue(value, val);
	if (val)
		maskblock->mData.mData[T3RenderStateBlock::smEntries[value].mWord] |= T3RenderStateBlock::smEntries[value].mMask;
	else
		maskblock->mData.mData[T3RenderStateBlock::smEntries[value].mWord] &= ~T3RenderStateBlock::smEntries[value].mMask;
}

void select_bool(T3RenderStateBlock* maskblock, const char* name, const char* id, T3RenderStateBlock* block, T3RenderStateValue value, T3RenderStateBlock::RenderStateEntry* pOverride = 0) {
	u32 val = pOverride ? (((block->mData.mData[pOverride->mWord]) & pOverride->mMask) >> pOverride->mShift) : block->GetRenderStateValue(value);
	ImGui::Text(name);
	ImGui::SameLine();
	ImGui::Checkbox(id, (bool*)&val);
	if (pOverride){
		block->mData.mData[pOverride->mWord] &= ~pOverride->mMask;
		block->mData.mData[pOverride->mWord] |= ((val << pOverride->mShift) & pOverride->mMask);
		if (val)
			maskblock->mData.mData[pOverride->mWord] |= pOverride->mMask;
		else
			maskblock->mData.mData[pOverride->mWord] &= ~pOverride->mMask;
	}
	else {
		if (val)
			maskblock->mData.mData[T3RenderStateBlock::smEntries[value].mWord] |= T3RenderStateBlock::smEntries[value].mMask;
		else
			maskblock->mData.mData[T3RenderStateBlock::smEntries[value].mWord] &= ~T3RenderStateBlock::smEntries[value].mMask;
		block->SetRenderStateValue(value, val);
	}
}

void select_uint8(T3RenderStateBlock* maskblock, const char* name, const char* id, T3RenderStateBlock* block, T3RenderStateValue value) {
	u32 val = block->GetRenderStateValue(value);
	ImGui::Text(name);
	ImGui::SameLine();
	ImGui::InputScalar(id, ImGuiDataType_U32,&val);
	if (val > 255)
		val = 255;
	block->SetRenderStateValue(value, val);
	if (val)
		maskblock->mData.mData[T3RenderStateBlock::smEntries[value].mWord] |= T3RenderStateBlock::smEntries[value].mMask;
	else
		maskblock->mData.mData[T3RenderStateBlock::smEntries[value].mWord] &= ~T3RenderStateBlock::smEntries[value].mMask;
}

void ShaderTask::_render() {
	id = select_gameid_dropdown(id);
	if (id) {
		ImGui::Text("Name");
		ImGui::SameLine();
		ImGui::InputText("##nmin", &name);
		bool bAwaiting = mbWaiting ? awaitingLoad.load() : false;
		if (!bAwaiting && !imported && ImGui::Button("Import Shaders")) {
			nfdchar_t* p{};
			DataStream* pFxStream = 0;
			if (NFD_OpenDialog("t3fxpack;ttarch2", 0, &p) == NFD_OKAY) {
				std::string path = p;
				free(p);
				if (ends_with(path, "ttarch2")) {
					DataStream* pSrc = OpenDataStreamFromDisc(path.c_str(), READ);
					if (!pSrc || pSrc->IsInvalid()) {
						MessageBoxA(0, "Could not open container file", "Error", MB_ICONERROR);
					}
					else {
						archive.Activate(pSrc);
						if (!archive.mbActive) {
							MessageBoxA(0, "Invalid archive", "Error", MB_ICONERROR);
							mark_as_todelete();
						}
						else {
							String n{};
							if (archive.mResources.size() != 1 || !ends_with(archive.GetResourceName(archive.mResources[0].mNameCRC), "t3fxpack")) {
								MessageBoxA(0, "Not a shader archive, or this shader archive is for an older game which is not supported yet.", "Error", MB_ICONERROR);
								mark_as_todelete();
							}
							pFxStream = archive.GetResourceStream(archive.mResources.data());
							if (name.length() == 0)
								name = std::move(n);
						}
					}
				}
				else {
					pFxStream = OpenDataStreamFromDisc(path.c_str(), READ);
					if (name.length() == 0)
						name = std::filesystem::path{ path }.filename().string();
				}
				SetStream(id, pFxStream);
			}
		}
		else if (mbWaiting && !bAwaiting) {
			mbWaiting = false;
			imported = mbLoadResult;
			if (!mbLoadResult) {
				MessageBoxA(0, "Could not save/load the given shader pack. Please contact me, it may not be supported yet.", "Shader sl error", MB_ICONERROR);
			}
			if (numImports > 1)
				imported = 1;//export failed
		}
		else if (mbWaiting) {
			const char* pstr = (const char*)mpProgressString.load();
			if (!pstr)
				pstr = "Submitting task";
			ImGui::Text("Shader pack exec, please wait: %s", pstr);
		}else if(imported){
			if(ImGui::Button("Export Effect Package")){

				MessageBoxA(0, "Shader Pack Exportation is not yet available.", "Not Available", MB_ICONERROR);
				return;//SavePackage only writes shader header, not programs or shader data


				if (name.length() == 0 || !ends_with(name, ".t3fxpack")) {
					MessageBoxA(0, "The effect package name must end in .T3FXPACK! Change the name before exporting.", "Name error", MB_ICONERROR);
				}
				else {
					bool bAsArchive = MessageBoxA(0, "Would you like to export it as an archive (.TTARCH2, replacable in Archives) - else it will be a plain shader pack (.T3FXPACK)?",
						"Output Format", MB_YESNO) == IDYES;
					nfdchar_t* p{};
					if (NFD_PickFolder(0, &p, L"Select output folder")) {
						std::string sname = name;
						if (bAsArchive)
							sname = sname.substr(0, sname.find_last_of('.')) + ".ttarch2";
						std::string path = p;
						path += "/";
						path += sname;
						free(p);
						DataStream* pFxStream = OpenDataStreamFromDisc(path.c_str(), WRITE);
						Job j{};
						j.name = "Shader pack save " + sname + "";
						j.userA = pFxStream;
						j.userB = (void*)id;
						j.mpUserInstance = this;
						if (bAsArchive)
							j.userA = (void*)((u64)j.userA | (1ull << 63));//shut up it works
						j.priority = 1;
						j.mpFn = &ShaderTask::AsyncDoSave;
						JobScheduler::Scheduler().PostJob(this, j);
						awaitingLoad.store(true);
						mbWaiting = true;
						imported = false;
						return;
					}
				}
			}
			char tmp[32];
			int toopen = 0;
			if(ImGui::Button("What is a bucket? What are programs? Click for help.")){
				ImGui::OpenPopup("help.");
			}
			if(ImGui::BeginPopup("help.")){
				ImGui::Text("BUCKET: Shader buckets are a distinct effect type, quality, optional material and set of static features.");
				ImGui::Text("A bucket is a container for a set of variants, in which each variant has a runnable set of compute/vertex/pixel/geometry shaders.");
				ImGui::Text("VARIANT: A variant is just a shader program, with a set of dynamic features (this is what varies).");
				ImGui::Text("PROGRAM: A shader program is what can be bound to the rendering API for a draw call, heavily used in the rendering pipeline.");
				ImGui::Text("FEATURES: Effect features are either static or dynamic, as said earlier static ones are per bucket, a set of dynamic ones determine the variant.");
				ImGui::Text("Example of static features are lightmap, vertex colour, deformable. Dynamic examples are shadow, sharp shadow, cin/env light, etc.");
				ImGui::Text("See the Bucket naming help for more information on each effect feature and type.");
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			if(ImGui::Button("Bucket Naming Help")){
				ImGui::OpenPopup("helpbckt");
			}
			if(ImGui::BeginPopup("helpbckt")){
				ImGui::Text("Format: <EffectType>_[Features]_[MaterialHash]_[Quality]_[Debug]");
				ImGui::Text("Effect Type: A rendering effect which may require a draw call. Eg mesh, glow, text.");
				if (ImGui::Button("View All Effect Types and Values")) {
					ImGui::CloseCurrentPopup();
					toopen = 1;
				}
				ImGui::Text("Features: Static features relating to the type. Eg lightmaps, deformable, vertex colors.");
				if (ImGui::Button("View All Effect Features and Values")) {
					ImGui::CloseCurrentPopup();
					toopen = 2;
				}
				ImGui::Text("Material Hash: Prefixed with M, this is the optional material required for this bucket of programs.");
				ImGui::Text("Quality: Used to select this bucket of programs depending on game settings quality");
				if (ImGui::Button("View All Qualities and Values")) {
					ImGui::CloseCurrentPopup();
					toopen = 3;
				}
				ImGui::Text("Debug: Never set here, but set if it is a debug program.");
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			if(toopen == 1)
				ImGui::OpenPopup("vieweff");
			if (toopen == 2)
				ImGui::OpenPopup("viewfeat");
			if(toopen == 3)
				ImGui::OpenPopup("viewq");
			if(ImGui::BeginPopup("vieweff")){
				if (ImGui::BeginTable("Effect Types", 3)) {
					ImGui::TableSetupColumn("Effect Name", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Effect Macro List", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Effect File", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableHeadersRow();
					ImGui::TableNextRow();
					for(int i = 0; i < mLocalCacheContext.mpVersionDatabase->mEffects.size(); i++){
						ImGui::TableSetColumnIndex(0);
						ImGui::Text(mLocalCacheContext.mpVersionDatabase->mEffects[i].mName);
						if (ImGui::IsItemHovered()) {
							ImGui::BeginTooltip();
							ImGui::Text(mLocalCacheContext.mpVersionDatabase->mEffects[i].mFXFileName);
							ImGui::EndTooltip();
						}
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(mLocalCacheContext.mpVersionDatabase->mEffects[i].mMacroList);
						ImGui::TableSetColumnIndex(2);
						ImGui::Text(mLocalCacheContext.mpVersionDatabase->mEffects[i].mFXFileName);
						ImGui::TableNextRow();
					}
					ImGui::EndTable();
				}
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("viewfeat")) {
				if (ImGui::BeginTable("Effect Features", 3)) {
					ImGui::TableSetupColumn("Feature", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Is Dynamic", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Feature Name", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableHeadersRow();
					ImGui::TableNextRow();
					for (int i = 0; i < mLocalCacheContext.mpVersionDatabase->mFeatures.size(); i++) {
						ImGui::TableSetColumnIndex(0);
						ImGui::Text(mLocalCacheContext.mpVersionDatabase->mFeatures[i].mSuffix);
						if (ImGui::IsItemHovered() && mLocalCacheContext.mpVersionDatabase->mFeatures[i].mTag && *mLocalCacheContext.mpVersionDatabase->mFeatures[i].mTag) {
							ImGui::BeginTooltip();
							ImGui::Text(mLocalCacheContext.mpVersionDatabase->mFeatures[i].mTag);
							ImGui::EndTooltip();
						}
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(mLocalCacheContext.mpVersionDatabase->mFeatures[i].mbDynamic ? "Yes" : "No");
						ImGui::TableSetColumnIndex(2);
						ImGui::Text(mLocalCacheContext.mpVersionDatabase->mFeatures[i].mName);
						ImGui::TableNextRow();
					}
					ImGui::EndTable();
				}
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("viewq")) {
				if (ImGui::BeginTable("Effect Qualities", 2)) {
					ImGui::TableSetupColumn("Quality", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Quality Name", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableHeadersRow();
					ImGui::TableNextRow();
					for (int i = 0; i < mLocalCacheContext.mpVersionDatabase->mQualities.size(); i++) {
						ImGui::TableSetColumnIndex(0);
						ImGui::Text(mLocalCacheContext.mpVersionDatabase->mQualities[i].mSuffix);
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(mLocalCacheContext.mpVersionDatabase->mQualities[i].mName);
						ImGui::TableNextRow();
					}
					ImGui::EndTable();
				}
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			if(selectedNodeType == BUCKET){
				ImGui::OpenPopup("Shader Bucket Settings");
				if(ImGui::BeginPopupModal("Shader Bucket Settings")){
					LoadedBucket& bucket = mBuckets[selectedShaderNode];
					TextCentered("Bucket Parameters");
					ImGui::Text("Effect Type");
					if(ImGui::BeginCombo("##tycm", mLocalCacheContext.mpVersionDatabase->mEffects[mEditingBucket.mEffect].mName)){
						for(int i = 0; i < mLocalCacheContext.mpVersionDatabase->mEffects.size(); i++){
							if (ImGui::Selectable(mLocalCacheContext.mpVersionDatabase->mEffects[i].mName))
								mEditingBucket.mEffect = (T3EffectType)i;
						}
						ImGui::EndCombo();
					}
					ImGui::Text("Effect Quality");
					if (ImGui::BeginCombo("##qcm", mLocalCacheContext.mpVersionDatabase->mQualities[mEditingBucket.mParams.mQuality].mName)) {
						for (int i = 0; i < mLocalCacheContext.mpVersionDatabase->mQualities.size(); i++) {
							if (ImGui::Selectable(mLocalCacheContext.mpVersionDatabase->mQualities[i].mName))
								mEditingBucket.mParams.mQuality = (T3EffectQuality)i;
						}
						ImGui::EndCombo();
					}
					ImGui::Text("Effect Material CRC64");
					ImGui::InputText("##matc", &mEditingBucket.mMatCRC);
					ImGui::Text("Static Effect Features");
					int nSt = 0;
					for (int i = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature; i < 
						mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature + mLocalCacheContext.mpVersionDatabase->mStaticFeaturesCount; i++){
						if (mEditingBucket.mParams.mStaticFeatures[(T3EffectFeature)(i)]) {
							ImGui::PushID(i);
							if(ImGui::Button("Remove")){
								mEditingBucket.mParams.mStaticFeatures.Set((T3EffectFeature)i, false);
							}
							ImGui::SameLine();
							ImGui::BulletText("%s", mLocalCacheContext.mpVersionDatabase->mFeatures[i].mName);
							ImGui::PopID();
							nSt++;
						}
					}
					if (!nSt)
						ImGui::Text("No Static Features");
					ImGui::Text("Add Static Feature:");
					ImGui::SameLine();
					if(ImGui::BeginCombo("##stccm", mLocalCacheContext.mpVersionDatabase->mFeatures[mEditingBucket.newStaticFeat].mName)){
						for (int i = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature; i <
							mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature + mLocalCacheContext.mpVersionDatabase->mStaticFeaturesCount; i++) {
							if (ImGui::Selectable(mLocalCacheContext.mpVersionDatabase->mFeatures[i].mName)) {
								mEditingBucket.newStaticFeat = i;   
							}
						}
						ImGui::EndCombo();
					}
					ImGui::SameLine();
					if (ImGui::Button("Add")){
						mEditingBucket.mParams.mStaticFeatures.Set((T3EffectFeature)mEditingBucket.newStaticFeat, 1);
					}
					ImGui::Separator();
					TextCentered("New Variant");
					ImGui::Text("New Variant Dynamic Effect Features");
					int nDyn = 0;
					for (int i = mLocalCacheContext.mpVersionDatabase->mFirstDynamicFeature;
						i < mLocalCacheContext.mpVersionDatabase->mFirstDynamicFeature+ mLocalCacheContext.mpVersionDatabase->mDynamicFeaturesCount; i++) {
						if (mEditingBucket.newVariantFeatures[(T3EffectFeature)(i)]) {
							nDyn++;
							ImGui::PushID(i);
							if (ImGui::Button("Remove")) {
								mEditingBucket.newVariantFeatures.Set((T3EffectFeature)i, false);
							}
							ImGui::SameLine();
							ImGui::BulletText("%s", mLocalCacheContext.mpVersionDatabase->mFeatures[i].mName);
							ImGui::PopID();
						}
					}
					if (!nDyn)
						ImGui::Text(C_NO_FEATURES.c_str());
					ImGui::Text("Add Dynamic Feature:");
					ImGui::SameLine();
					if (ImGui::BeginCombo("##dtccm", mLocalCacheContext.mpVersionDatabase->mFeatures[mEditingBucket.newDynamicFeat].mName)) {
						for (int i = mLocalCacheContext.mpVersionDatabase->mFirstDynamicFeature; i <
							mLocalCacheContext.mpVersionDatabase->mDynamicFeaturesCount + mLocalCacheContext.mpVersionDatabase->mFirstDynamicFeature; i++) {
							if (ImGui::Selectable(mLocalCacheContext.mpVersionDatabase->mFeatures[i].mName)) {
								mEditingBucket.newDynamicFeat = i;
							}
						}
						ImGui::EndCombo();
					}
					ImGui::SameLine();
					ImGui::PushID("_d");
					if (ImGui::Button("Add")) {
						mEditingBucket.newVariantFeatures.Set((T3EffectFeature)mEditingBucket.newDynamicFeat, 1);
					}
					ImGui::PopID();
					if(ImGui::Button("Create")){
						bool ok = 1;
						for (auto& variant : bucket.mVariants)
							if (variant.mVariantFeatures == mEditingBucket.newVariantFeatures)
								ok = 0;//break lol
						if (!ok) {
							MessageBoxA(0, "There is already a variant in this bucket which matches those features!", "Exists", MB_ICONERROR);
						}else{
							LoadedProgram& var = bucket.mVariants.emplace_back();
							var.mbCompute = mLocalCacheContext.mpVersionDatabase->mEffects[bucket.mEffect].mCategory == eEffectCategory_Compute;
							var.mVariantFeatures = mEditingBucket.newVariantFeatures;
							EffectFeatures expanded = var.mVariantFeatures;
							var.mBuildFeatureList = T3::BuildSuffix(mLocalCacheContext, expanded);
							mEditingBucket.newDynamicFeat = mLocalCacheContext.mpVersionDatabase->mFirstDynamicFeature;
							mEditingBucket.newStaticFeat = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature;
							mEditingBucket.newVariantFeatures.SetAll(false);
							var.mPasses.emplace_back();//default
							MessageBoxA(0, "Success! New variant was created.", "Success", MB_ICONINFORMATION);
						}
					}
					ImGui::Separator();
					if(ImGui::Button("Done (Apply Changed Settings)")){
						bool ok = false;
						char* str, * end;
						unsigned long long result;
						str = (char*)mEditingBucket.mMatCRC.c_str();
						errno = 0;
						result = strtoull(str, &end, 16);
						if (result == 0 && end == str) {
							MessageBoxA(0, "The material CRC has not a valid hex number!", "Error", MB_ICONERROR);
						}
						else if (result == ULLONG_MAX && errno) {
							MessageBoxA(0, "The material CRC is too large (> u64 max)! This cannot be a valid one, make sure its a valid material.", "Error", MB_ICONERROR);
						}
						else if (*end) {
							MessageBoxA(0, "Invalid material CRC! Invalid suffix", "Error", MB_ICONERROR);
						}else{
							ok = true;
						}
						if (ok) {
							for (int bucketIndex = 0; bucketIndex < mBuckets.size(); bucketIndex++) {
								if (mBuckets[bucketIndex].mEffect == mEditingBucket.mEffect &&
									mBuckets[bucketIndex].mParams.mMaterialCrc == mEditingBucket.mParams.mMaterialCrc &&
									mBuckets[bucketIndex].mParams.mQuality == mEditingBucket.mParams.mQuality &&
									mBuckets[bucketIndex].mParams.mStaticFeatures == mEditingBucket.mParams.mStaticFeatures) {
									if (bucketIndex == selectedShaderNode)//no changes
										break;
									ok = false;
									break;
								}
							}
							if (ok) {
								bucket.mParams.mMaterialCrc = result;
								bucket.mEffect = mEditingBucket.mEffect;
								bucket.mParams = mEditingBucket.mParams;
								EffectFeatures tmpExp = bucket.mParams.mStaticFeatures;
								bucket.mBuiltName = T3::BuildName(mLocalCacheContext, bucket.mEffect, tmpExp, 
									EffectQualities(bucket.mParams.mQuality, mLocalCacheContext.mpVersionDatabase),
									bucket.mParams.mMaterialCrc = result, false, "");
								selectedShaderNode = selectedVariant = -1;
								selectedNodeType = NONE;
								ImGui::CloseCurrentPopup();
							}
							else {
								MessageBoxA(0, "You have changed bucket settings. The new settings match an already existing bucket!", "Error", MB_ICONERROR);
							}
						}
					}
					ImGui::SameLine();
					if(ImGui::Button("Close (Don't Apply Changed Settings)")){
						selectedShaderNode = selectedVariant = -1;
						selectedNodeType = NONE;
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
					if(ImGui::Button("Delete Bucket")){
						auto it = mBuckets.begin();
						std::advance(it, selectedShaderNode);
						mBuckets.erase(it);
						selectedShaderNode = selectedVariant = -1;
						selectedNodeType = NONE;
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
			if(selectedNodeType == RENDER_STATE){
				ImGui::OpenPopup("Render State");
				if (ImGui::BeginPopupModal("Render State")) {
					if(TelltaleToolLib_GetGameKeyIndex(id) <= TelltaleToolLib_GetGameKeyIndex("WDM")){
						ImGui::CloseCurrentPopup();
						selectedNodeType = NONE;
						mpEditingRS = mpEditingRSMask = 0;
						MessageBoxA(0, "Render state in older games cannot be modified as the bitset bit ranges have not been "
							"analysed! You can still modify shaders.", "Error", MB_ICONERROR);
					}
					else {
						ImGui::Text("Normal Render State is what is added to the render state bitset.");

						select_bool(mpEditingRSMask, "Z Enabled", "##b", mpEditingRS, eRSZEnable_Value);
						select_bool(mpEditingRSMask, "Z Write Enabled", "##c", mpEditingRS, eRSZWriteEnable_Value);
						const char* arr[] = { "Never","Less","Equal","Less Equal","Greater","Not Equal","Greater Equal","Always" };
						select_dropdown(mpEditingRSMask, "Z Function", "##d", mpEditingRS, arr, 8, eRSZFunc_Value);
						const char* arr_0[] = { "None","Clockwise","Counter Clockwise" };
						select_dropdown(mpEditingRSMask, "Cull Mode", "##e", mpEditingRS, arr_0, 3, eRSCullMode_Value);
						select_bool(mpEditingRSMask, "Invert Cull Mode", "##f", mpEditingRS, eRSInvertCullMode_Value);
						select_bool(mpEditingRSMask, "Enable Alpha to Coverage", "##g", mpEditingRS, eRSAlphaToCoverage_Value);
						select_bool(mpEditingRSMask, "Enable Alpha Blend", "##h", mpEditingRS, eRSAlphaBlendEnable_Value);
						const char* arr_1[] = { "Add","Subtract","Reverse Substract","Min","Max" };
						select_dropdown(mpEditingRSMask, "Blend Operation", "##i", mpEditingRS, arr_1, 5, eRSBlendOp_Value);
						const char* arr_2[] = { "Zero","One","Source Color","Inverse Source Color","Source Alpha","Inverse Source Alpha",
							"Destination Alpha","Inverse Destination Alpha","Destination Color","Inverse Destination Color","Color" };
						select_dropdown(mpEditingRSMask, "Source Blend", "##j", mpEditingRS, arr_2, 11, eRSSrcBlend_Value);
						select_dropdown(mpEditingRSMask, "Destination Blend", "##k", mpEditingRS, arr_2, 11, eRSDstBlend_Value);
						T3RenderStateBlock::RenderStateEntry tmpEntry = { 0,22,0x400000 };
						select_bool(mpEditingRSMask, "Enable Color Write R", "##l0", mpEditingRS, eRSColorWriteEnable_Value, &tmpEntry);
						tmpEntry = { 0,23,0x800000 };
						select_bool(mpEditingRSMask, "Enable Color Write G", "##l1", mpEditingRS, eRSColorWriteEnable_Value, &tmpEntry);
						tmpEntry = { 0,24,0x1000000 };
						select_bool(mpEditingRSMask, "Enable Color Write B", "##l2", mpEditingRS, eRSColorWriteEnable_Value, &tmpEntry);
						tmpEntry = { 0,25,0x2000000 };
						select_bool(mpEditingRSMask, "Enable Color Write A", "##l3", mpEditingRS, eRSColorWriteEnable_Value, &tmpEntry);
						select_bool(mpEditingRSMask, "Enable Separate Blend", "##m", mpEditingRS, eRSSeparateBlendEnable_Value);
						select_dropdown(mpEditingRSMask, "Alpha Blend Operation", "##n", mpEditingRS, arr_1, 5, eRSBlendOpAlpha_Value);
						select_dropdown(mpEditingRSMask, "Alpha Source Blend", "##o", mpEditingRS, arr_2, 11, eRSSrcBlendAlpha_Value);
						select_dropdown(mpEditingRSMask, "Alpha Destination Blend", "##p", mpEditingRS, arr_2, 11, eRSDstBlendAlpha_Value);
						select_bool(mpEditingRSMask, "Enable Stencil", "##q", mpEditingRS, eRSStencilEnable_Value);
						const char* arr_3[] = { "Keep","Zero","Replace","Increment Saturation","Decrement Saturation","Invert","Increment","Decrement" };
						select_dropdown(mpEditingRSMask, "Stencil Fail Operation", "##r", mpEditingRS, arr_3, 8, eRSStencilFail_Value);
						select_dropdown(mpEditingRSMask, "Stencil Z Fail Operation", "##s", mpEditingRS, arr_3, 8, eRSStencilZFail_Value);
						select_dropdown(mpEditingRSMask, "Stencil Pass Operation", "##t", mpEditingRS, arr_3, 8, eRSStencilPass_Value);
						select_dropdown(mpEditingRSMask, "Stencil Pass Function", "##u", mpEditingRS, arr, 8, eRSStencilFunc_Value);
						select_uint8(mpEditingRSMask, "Stencil Ref Value UInt8", "##v", mpEditingRS, eRSStencilRef_Value);
						select_uint8(mpEditingRSMask, "Stencil Read Bitwise Mask UInt8", "##w", mpEditingRS, eRSStencilMask_Value);
						select_uint8(mpEditingRSMask, "Stencil Write Bitwise Mask UInt8", "##x", mpEditingRS, eRSStencilWriteMask_Value);
						const char* arr_4[] = { "Solid","Wireframe" };
						select_dropdown(mpEditingRSMask, "Rasterizer Fill Mode", "##y", mpEditingRS, arr_4, 2, eRSFillMode_Value);
						select_bool(mpEditingRSMask, "Enable Depth Bounds", "##z", mpEditingRS, eRSDepthBoundsEnable_Value);
						const char* arr_5[] = { "0","10" };
						select_bool(mpEditingRSMask, "Enable Z Bias", "##a", mpEditingRS, eRSZBias_Value);
						select_dropdown(mpEditingRSMask, "Z Bias", "##aaa", mpEditingRS, arr_5, 2, eRSZOffset_Value);
						select_bool(mpEditingRSMask, "Scissor Enabled", "##aa", mpEditingRS, eRSScissorEnable_Value);
						select_bool(mpEditingRSMask, "Depth Clip Enabled", "##ab", mpEditingRS, eRSZClip_Value);
						select_bool(mpEditingRSMask, "Invert Z", "##ac", mpEditingRS, eRSZInvert_Value);
						if (ImGui::Button("Close")) {
							ImGui::CloseCurrentPopup();
							selectedNodeType = NONE;
							mpEditingRS = mpEditingRSMask = 0;
						}
					}
					ImGui::EndPopup();
				}
			}else if(selectedNodeType == SHADER){
				ImGui::OpenPopup("Shader Options");
				if(ImGui::BeginPopupModal("Shader Options")){
					if(mbOpenPassCompute){
						if(mpOpenPass->mShader[0] != -1 && ImGui::Button("Extract Raw Compute Shader")){
							nfdchar_t* outp = 0;
							if(NFD_PickFolder(0, &outp, L"Select Output Folder") == NFD_OKAY){
								std::string fp = outp;
								fp += "/ComputeShader.bin";
								DataStreamFileDisc out = _OpenDataStreamFromDisc_(fp.c_str(), WRITE);
								mShaders[mpOpenPass->mShader[0]].mpShaderData->Transfer(&out, 0, mShaders[mpOpenPass->mShader[0]].mLoadedBinSize);
								MessageBoxA(0, "Successfully extracted the raw shader binary! This may include platform specific parameter headers.", "Success", MB_ICONINFORMATION);	
								free(outp);
							}
						}
					}else{
						const char* arr[3] = { "Extract Raw Vertex Shader","Extract Raw Pixel Shader","Extract Raw Geometry Shader" };
						const char* arr1[3] = { "/VertexShader.bin","/PixelShader.bin","/GeometryShader.bin" };
						for(int i = 0; i < 3; i++){
							if (mpOpenPass->mShader[i] != -1 && ImGui::Button(arr[i])) {
								nfdchar_t* outp = 0;
								if (NFD_PickFolder(0, &outp, L"Select Output Folder") == NFD_OKAY) {
									std::string fp = outp;
									fp += arr1[i];
									DataStreamFileDisc out = _OpenDataStreamFromDisc_(fp.c_str(), WRITE);
									mShaders[mpOpenPass->mShader[i]].mpShaderData->Transfer(&out, 0, mShaders[mpOpenPass->mShader[i]].mLoadedBinSize);
									MessageBoxA(0, "Successfully extracted the raw shader binary Shader.bin! This may include platform specific parameter headers.", "Success", MB_ICONINFORMATION);
									free(outp);
								}
							}
						}
					}
					if(ImGui::Button("Close")){
						ImGui::CloseCurrentPopup();
						selectedNodeType = NONE;
						mpOpenPass = 0;
						mbOpenPassCompute = false;
					}
					ImGui::EndPopup();
				}
			}
			if((ImGui::Button("New Bucket") && selectedNodeType == NONE) || selectedNodeType == NEW_BUCKET){
				if (selectedNodeType == NONE) {
					selectedNodeType = NEW_BUCKET;
					mEditingBucket.mMatCRC = "0";
				}
				ImGui::OpenPopup("New Effect Bucket");
				if(ImGui::BeginPopupModal("New Effect Bucket")){
					ImGui::Text("Effect Type");
					if (ImGui::BeginCombo("##tycm", mLocalCacheContext.mpVersionDatabase->mEffects[mEditingBucket.mEffect].mFullName.c_str())) {
						for (int i = 0; i < mLocalCacheContext.mpVersionDatabase->mEffects.size(); i++) {
							if (ImGui::Selectable(mLocalCacheContext.mpVersionDatabase->mEffects[i].mFullName.c_str()))
								mEditingBucket.mEffect = (T3EffectType)i;
						}
						ImGui::EndCombo();
					}
					ImGui::Text("Effect Quality");
					if (ImGui::BeginCombo("##qcm", mLocalCacheContext.mpVersionDatabase->mQualities[mEditingBucket.mParams.mQuality].mName)) {
						for (int i = 0; i < mLocalCacheContext.mpVersionDatabase->mQualities.size(); i++) {
							if (ImGui::Selectable(mLocalCacheContext.mpVersionDatabase->mQualities[i].mName))
								mEditingBucket.mParams.mQuality = (T3EffectQuality)i;
						}
						ImGui::EndCombo();
					}
					ImGui::Text("Effect Material CRC64");
					ImGui::InputText("##matc", &mEditingBucket.mMatCRC);
					ImGui::Text("Static Effect Features");
					int nSt = 0;
					for (int i = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature; 
						i < mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature+ mLocalCacheContext.mpVersionDatabase->mStaticFeaturesCount; i++) {
						if (mEditingBucket.mParams.mStaticFeatures[(T3EffectFeature)(i)]) {
							ImGui::PushID(i);
							if (ImGui::Button("Remove")) {
								mEditingBucket.mParams.mStaticFeatures.Set((T3EffectFeature)i, false);
							}
							ImGui::SameLine();
							ImGui::BulletText("%s", mLocalCacheContext.mpVersionDatabase->mFeatures[i].mName);
							ImGui::PopID();
							nSt++;
						}
					}
					if (!nSt)
						ImGui::Text("No Static Features");
					ImGui::Text("Add Static Feature:");
					ImGui::SameLine();
					if (ImGui::BeginCombo("##stccm", mLocalCacheContext.mpVersionDatabase->mFeatures[mEditingBucket.newStaticFeat].mName)) {
						for (int i = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature;
							i < mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature + mLocalCacheContext.mpVersionDatabase->mStaticFeaturesCount; i++) {
							if (ImGui::Selectable(mLocalCacheContext.mpVersionDatabase->mFeatures[i].mName)) {
								mEditingBucket.newStaticFeat = i;
							}
						}
						ImGui::EndCombo();
					}
					ImGui::SameLine();
					if (ImGui::Button("Add")) {
						mEditingBucket.mParams.mStaticFeatures.Set((T3EffectFeature)mEditingBucket.newStaticFeat, 1);
					}
					if(ImGui::Button("Create")){
						bool ok = false;
						char* str, * end;
						unsigned long long result;
						str = (char*)mEditingBucket.mMatCRC.c_str();
						errno = 0;
						result = strtoull(str, &end, 16);
						if (result == 0 && end == str) {
							MessageBoxA(0, "The material CRC has not a valid hex number!", "Error", MB_ICONERROR);
						}
						else if (result == ULLONG_MAX && errno) {
							MessageBoxA(0, "The material CRC is too large (> u64 max)! This cannot be a valid one, make sure its a valid material.", "Error", MB_ICONERROR);
						}
						else if (*end) {
							MessageBoxA(0, "Invalid material CRC! Invalid suffix", "Error", MB_ICONERROR);
						}
						else {
							ok = true;
						}
						if (ok) {
							std::string existsing{};
							for (int bucketIndex = 0; bucketIndex < mBuckets.size(); bucketIndex++) {
								if (mBuckets[bucketIndex].mEffect == mEditingBucket.mEffect &&
									mBuckets[bucketIndex].mParams.mMaterialCrc == mEditingBucket.mParams.mMaterialCrc &&
									mBuckets[bucketIndex].mParams.mQuality == mEditingBucket.mParams.mQuality &&
									mBuckets[bucketIndex].mParams.mStaticFeatures == mEditingBucket.mParams.mStaticFeatures) {
									if (bucketIndex == selectedShaderNode)//no changes
										break;
									ok = false;
									existsing = mBuckets[bucketIndex].mBuiltName;
									break;
								}
							}
							if (ok) {
								LoadedBucket bucket{ {mLocalCacheContext} };
								bucket.mEffect = mEditingBucket.mEffect;
								bucket.mParams = mEditingBucket.mParams;
								bucket.mParams.mMaterialCrc = result;
								LoadedProgram defVariant{};
								defVariant.mbCompute = mLocalCacheContext.mpVersionDatabase->mEffects[bucket.mEffect].mCategory == eEffectCategory_Compute;
								defVariant.mBuildFeatureList = C_NO_FEATURES;
								LoadedProgramPass defPass{};
								if(mBuckets.size() && mBuckets[0].mVariants.size() && mBuckets[0].mVariants[0].mPasses.size()){
									//for render state each game is different and they are pretty much *all* the same (99%)
									//so just set to the most common one.
									defPass.mRenderState = mBuckets[0].mVariants[0].mPasses[0].mRenderState;
									defPass.mRenderStateMask = mBuckets[0].mVariants[0].mPasses[0].mRenderStateMask;
								}
								defVariant.mPasses.push_back(std::move(defPass));
								bucket.mVariants.push_back(std::move(defVariant));
								EffectFeatures tmpExp = bucket.mParams.mStaticFeatures;
								std::string bname = bucket.mBuiltName = T3::BuildName(mLocalCacheContext, bucket.mEffect, tmpExp,
									EffectQualities(bucket.mParams.mQuality),
									bucket.mParams.mMaterialCrc, false, "");
								insert_sorted(mBuckets, std::move(bucket), _CmpBucket{});
								mEditingBucket.mEffect = (T3EffectType)0;
								mEditingBucket.mMatCRC.clear();
								mEditingBucket.mParams.mMaterialCrc = 0;
								mEditingBucket.mParams.mQuality = (T3EffectQuality)0;
								mEditingBucket.newStaticFeat = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature;
								mEditingBucket.mParams.mStaticFeatures.SetAll(false);
								selectedNodeType = NONE;
								ImGui::CloseCurrentPopup();
								MessageBoxA(0, "Success! The new bucket was created. The title of this message box is the name of it!", bname.c_str(), MB_ICONINFORMATION);
							}
							else {
								MessageBoxA(0, "These new bucket settings already match an existing bucket!", existsing.c_str(), MB_ICONERROR);
							}
						}
					}
					if(ImGui::Button("Cancel")){
						mEditingBucket.mEffect = (T3EffectType)0;
						mEditingBucket.mMatCRC.clear();
						mEditingBucket.mParams.mMaterialCrc = 0;
						mEditingBucket.mParams.mQuality = (T3EffectQuality)0;
						mEditingBucket.newStaticFeat = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature;
						mEditingBucket.mParams.mStaticFeatures.SetAll(false);
						selectedNodeType = NONE;
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
			ImGui::Separator();
			TextCentered("Show Buckets Matching Filter");
			ImGui::Text("Effect Type");
			if (ImGui::BeginCombo("##1tycm", mFilterType == WDC_eEffect_None ? "All" : mLocalCacheContext.mpVersionDatabase->mEffects[mFilterType].mFullName.c_str())) {
				if (ImGui::Selectable("All"))
					mFilterType = WDC_eEffect_None;
				for (int i = 0; i < mLocalCacheContext.mpVersionDatabase->mEffects.size(); i++) {
					if (ImGui::Selectable(mLocalCacheContext.mpVersionDatabase->mEffects[i].mFullName.c_str()))
						mFilterType = (T3EffectType)i;
				}
				ImGui::EndCombo();
			}
			ImGui::Text("Effect Quality");
			if (ImGui::BeginCombo("##1qcm", mFilterQuality == eEffectQuality_Default ? "All" : mLocalCacheContext.mpVersionDatabase->mQualities[mFilterQuality].mName)) {
				if (ImGui::Selectable("All"))
					mFilterQuality = eEffectQuality_Default;
				for (int i = 0; i < mLocalCacheContext.mpVersionDatabase->mQualities.size(); i++) {
					if (ImGui::Selectable(mLocalCacheContext.mpVersionDatabase->mQualities[i].mName))
						mFilterQuality = (T3EffectQuality)i;
				}
				ImGui::EndCombo();
			}
			ImGui::Text("Static Effect Features");
			int nSt = 0;
			for (int i = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature; i <
				mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature+ mLocalCacheContext.mpVersionDatabase->mStaticFeaturesCount; i++) {
				if (mFilterFeatures[(T3EffectFeature)(i)]) {
					ImGui::PushID(i);
					if (ImGui::Button("Remove")) {
						mFilterFeatures.Set((T3EffectFeature)i, false);
					}
					ImGui::SameLine();
					ImGui::BulletText("%s", mLocalCacheContext.mpVersionDatabase->mFeatures[i].mName);
					ImGui::PopID();
					nSt++;
				}
			}
			if (!nSt)
				ImGui::Text("All");
			ImGui::Text("Add Static Feature:");
			ImGui::SameLine();
			if (ImGui::BeginCombo("##stccm", mLocalCacheContext.mpVersionDatabase->mFeatures[mNewFilterFeature].mName)) {
				for (int i = mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature; i < 
					mLocalCacheContext.mpVersionDatabase->mFirstStaticFeature+ mLocalCacheContext.mpVersionDatabase->mStaticFeaturesCount; i++) {
					if (ImGui::Selectable(mLocalCacheContext.mpVersionDatabase->mFeatures[i].mName)) {
						mNewFilterFeature = i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			if (ImGui::Button("Add")) {
				mFilterFeatures.Set((T3EffectFeature)mNewFilterFeature, 1);
			}
			ImGui::Separator();
			ImGui::Text("WARNING: Changing data such as bucket static features will mean you need to modify the shader code, otherwise the GPU will crash !!");
			ImGui::Text("Shader Pack Bucket Tree");
			if (ImGui::BeginTable("##packdata", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_ScrollX)) {
				int bind = -1;
				bool bDoFeaturesFilter = !mFilterFeatures.IsZero();
				for(auto& bucket: mBuckets){
					bind++;
					if(mFilterType != WDC_eEffect_None && bucket.mEffect != mFilterType)
						continue;
					if (mFilterQuality != eEffectQuality_Default && bucket.mParams.mQuality != mFilterQuality)
						continue;
					if(bDoFeaturesFilter){
						StaticFeatures comb = bucket.mParams.mStaticFeatures;
						if (comb.IsZero())
							continue;
						comb.Mask(mFilterFeatures);
						if (comb != mFilterFeatures)
							continue;
					}
					ImGui::TableNextRow();
			 		ImGui::TableSetColumnIndex(0);
					ImGui::PushID(bind);
					if(ImGui::TreeNode(bucket.mBuiltName.c_str())){
						ImGui::TableSetColumnIndex(1);
						if(ImGui::Button("Bucket Settings") && selectedNodeType == NONE){
							selectedNodeType = BUCKET;
							selectedShaderNode = bind;
							selectedVariant = -1;
							mEditingBucket.mEffect = bucket.mEffect;
							mEditingBucket.mParams = bucket.mParams;
							mEditingBucket.mMatCRC = string_format("%llX", bucket.mParams.mMaterialCrc);
						}
						ImGui::TableSetColumnIndex(0);
						ImGui::TableNextRow();
						int vind = 0;
						for(auto& variant : bucket.mVariants){
							ImGui::PushID(vind);
							ImGui::TableSetColumnIndex(0);
							if(ImGui::TreeNode(variant.mBuildFeatureList.c_str())){
								ImGui::TableSetColumnIndex(1);
								if (ImGui::Button("Delete Variant") && selectedNodeType == NONE) {
									if(variant.mBuildFeatureList == C_NO_FEATURES || bucket.mVariants.size() < 2){
										MessageBoxA(0, "You cannot delete the default variant!", "Cannot delete this", MB_ICONERROR);
									}else if(bucket.mVariants.size() > 1){
										auto it = bucket.mVariants.begin();
										std::advance(it, vind);
										bucket.mVariants.erase(it);
										ImGui::PopID();
										ImGui::TreePop();
										break;
									}
								}
								ImGui::TableSetColumnIndex(0);
								ImGui::TableNextRow();
								int indx = 0;
								for (auto& pass : variant.mPasses) {
									ImGui::PushID(indx + 1);
									ImGui::TableSetColumnIndex(0);
									sprintf(tmp, "Pass [%d]", indx++);
									if (ImGui::TreeNode(tmp)) {
										ImGui::TableSetColumnIndex(1);
										if(ImGui::Button("Shader Options") && selectedNodeType == NONE){
											selectedNodeType = SHADER;
											mpOpenPass = &pass;
											mbOpenPassCompute = variant.mbCompute;
										}
										ImGui::TableSetColumnIndex(0);
										ImGui::TableNextRow();
										ImGui::TableSetColumnIndex(0);
										if (variant.mbCompute) {
											ImGui::Text("Thread Group Size X");
											ImGui::SameLine();
											ImGui::InputInt("##tgx", (int*)&pass.threadGroupSize[0]);
											ImGui::Text("Thread Group Size Y");
											ImGui::SameLine();
											ImGui::InputInt("##tgy", (int*)&pass.threadGroupSize[1]);
											ImGui::Text("Thread Group Size Z");
											ImGui::SameLine();
											ImGui::InputInt("##tgz", (int*)&pass.threadGroupSize[2]);
										}else{
											if(ImGui::Button("Edit Render State") && selectedNodeType == NONE){
												selectedNodeType = RENDER_STATE;
												mpEditingRS = &pass.mRenderState;
												mpEditingRSMask = &pass.mRenderStateMask;
											}
										}
										ImGui::TreePop();
									}
									ImGui::PopID();
									ImGui::TableNextRow();
								}
								ImGui::TreePop();
							}
							ImGui::PopID();
							ImGui::TableNextRow();
							vind++;
						}
						ImGui::TreePop();
					}
					ImGui::PopID();
				}
				ImGui::EndTable();
			}
		}
	}
}

void ShaderTask::AsyncDoSave(void* inst, void* a, void* b) {
	ShaderTask& task = *(ShaderTask*)inst;
	DataStream* pFxStream = (DataStream*)((u64)a & ~(1ull << 63));
	DataStream* pDeletable = pFxStream;
	DataStreamFileDisc disc{};
	bool bExportArchive = (u64)a >> 63;

	if (bExportArchive) {
		char buf1[MAX_PATH];
		char buf2[MAX_PATH];
		GetTempPathA(MAX_PATH, buf1);
		GetTempFileNameA(buf1, "create_fxpack_ttlib", 0, buf2);
		disc = DataStreamFileDisc(PlatformSpecOpenFile(buf2, WRITE), DataStreamMode::eMode_Write);
		pFxStream = &disc;
	}

	task.mpProgressString.store((uintptr_t)"Writing package");

	TelltaleToolLib_SetBlowfishKey((const char*)b);
	
	int version = task.importedVersion;
	if (version == 0) {
		task.mpProgressString.store((uintptr_t)"Shader pack version mismatch");
		MessageBoxA(0, "The imported shader pack did not load or was corrupt. The exported version will be set to the PC version.", "Warning", MB_ICONINFORMATION);
		version = sBlowfishKeys[sSetKeyIndex].pc_fxpack_ver;
		if(version == 0){
			task.mpProgressString.store((uintptr_t)"Error invalid game");
			MessageBoxA(0, "The current game cannot export effect packages! They don't exist in older games.", "Error", MB_ICONERROR);
			task.awaitingLoad.store(false);
			return;
		}
		task.mpProgressString.store((uintptr_t)"Writing package");
	}

	task.mbLoadResult = T3EffectCache::SavePackage(task.mLocalCacheContext.mpShaderBinary, task.mShaders, task.mBuckets, pFxStream, version);

	if(task.mbLoadResult && bExportArchive){
		task.mpProgressString.store((uintptr_t)"Writing wrapping archive");
		std::vector<TTArchive2::ResourceCreateEntry> e{};
		TTArchive2::ResourceCreateEntry entry{};
		entry.mpStream = pFxStream;
		pFxStream->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
		pFxStream->SetMode(DataStreamMode::eMode_Read);
		entry.name = task.name;
		if (entry.name.length() == 0)
			entry.name = "0.t3fxpack";//why havent they named it?
		e.push_back(std::move(entry));
		task.mbLoadResult = TTArchive2::Create(0, pDeletable, e, 0, 0, Compression::Library::ZLIB, 2U);
	}

	task.mpProgressString.store((uintptr_t)"Finished writing package");

	delete pDeletable;

	if (task.mbLoadResult)
		MessageBoxA(0, "The effect pack was successfully saved!", "Success!", MB_ICONINFORMATION);

	task.awaitingLoad.store(false);
}

void ShaderTask::AsyncDoLoad(void* inst, void* a, void* b){
	ShaderTask& task = *(ShaderTask*)inst;
	DataStream* pFxStream = (DataStream*)a;
	task.numImports++;

	task.mpProgressString.store((uintptr_t)"Reading package");

	TelltaleToolLib_SetBlowfishKey((const char*)b);
	int ver{};
	if (!T3EffectCache::LoadPackage(task.mLocalCacheContext, pFxStream, ver)) {
		task.mbLoadResult = false;
		task.mpProgressString.store((uintptr_t)"Failed");
	}
	else {
		task.mbLoadResult = true;
		task.mpProgressString.store((uintptr_t)"Loading shaders");
		int srcState = 0;//0: not checked, 1: binary, 2: src code (opengl,metal)
		for(T3EffectCacheShader* shader = task.mLocalCacheContext.mShaders.head(); shader; shader = shader->next){
			LoadedShader& loaded = task.mShaders.emplace_back();
			loaded.mTypeName = shader->mShaderType == eMaterialShader_Vertex ? "Vertex" : shader->mShaderType == eMaterialShader_Pixel ? "Fragment" : shader->mShaderType == eMaterialShader_Geometry ? "Geometry" : "Compute";
			loaded.mNeededParameters = shader->mNeededParameters;
			loaded.mLoadedBinOff = shader->mOffset;
			loaded.mLoadedBinSize = shader->mSize;
		}
		task.mpProgressString.store((uintptr_t)"Loading programs and passes");
		for (T3EffectCacheBucket* bucket = task.mLocalCacheContext.mpFirstBucket; bucket; bucket = bucket->mpNext) {
			LoadedBucket& b = task.mBuckets.emplace_back();
			EffectFeatures feat = bucket->mParams.mStaticFeatures;
			b.mParams = bucket->mParams;
			b.mEffect = (T3EffectType)bucket->mEffectType;
			bool bCompute = task.mLocalCacheContext.mpVersionDatabase->mEffects[bucket->mEffectType].mCategory == eEffectCategory_Compute;
			b.mBuiltName = T3::BuildName(task.mLocalCacheContext, (T3EffectType)bucket->mEffectType, feat,
				EffectQualities(bucket->mParams.mQuality, task.mLocalCacheContext.mpVersionDatabase), bucket->mParams.mMaterialCrc, false, "");
			for(int i = 0; i < bucket->mProgramHashSize; i++){
				if(bucket->mProgramHash[i]){
					for(T3EffectCacheProgramHashEntry* pEntry = bucket->mProgramHash[i]; pEntry; pEntry=pEntry->mpNext){
						LoadedProgram& pgm = b.mVariants.emplace_back();
						pgm.mVariantFeatures = pEntry->mpProgram->mFeatures;
						EffectFeatures asFull = pgm.mVariantFeatures;
						pgm.mBuildFeatureList = T3::BuildSuffix(task.mLocalCacheContext, asFull);
						if (pgm.mBuildFeatureList.length() == 0)
							pgm.mBuildFeatureList = C_NO_FEATURES;
						pgm.mbCompute = bCompute;
						pgm.mPasses.reserve(pEntry->mpProgram->mPassCount);
						for(int k = 0; k < pEntry->mpProgram->mPassCount; k++){
							LoadedProgramPass& pass = pgm.mPasses.emplace_back();
							if(bCompute){
								T3EffectCachePass_Compute* pComputePass = pEntry->mpProgram->mPassesCompute + k;
								pass.mShader[0] = pComputePass->mInitialShaderIndex;
								pass.threadGroupSize[0] = pComputePass->mThreadGroupSizeX;
								pass.threadGroupSize[1] = pComputePass->mThreadGroupSizeY;
								pass.threadGroupSize[2] = pComputePass->mThreadGroupSizeZ;
							}else{
								T3EffectCachePass_Draw* pDrawPass = pEntry->mpProgram->mPassesDraw + k;
								for(int j = 0; j < 3; j++){
									if (pDrawPass->mpDrawShader[j])
										pass.mShader[j] = pDrawPass->mInitialShaderIndex[j];
									else
										pass.mShader[j] = -1;
								}
								pass.mRenderState = pDrawPass->mRenderStateBlock;
								pass.mRenderStateMask = pDrawPass->mRenderStateMask;
							}
						}
					}
				}
			}
		}
		task.mpProgressString.store((uintptr_t)"Creating shader data substreams");
		for(auto& shader: task.mShaders){
			shader.mpShaderData = std::make_shared<DataStreamSubStream>(task.mLocalCacheContext.mpShaderBinary.get(), (u64)shader.mLoadedBinSize, (u64)shader.mLoadedBinOff);
		}
		task.mpProgressString.store((uintptr_t)"Sorting buckets");
		std::sort(task.mBuckets.begin(), task.mBuckets.end(), _CmpBucket{});
		task.mpProgressString.store((uintptr_t)"Sorting bucket program variants");
		for(auto& bucket: task.mBuckets){
			std::sort(bucket.mVariants.begin(), bucket.mVariants.end(), _CmpPgm{});
		}
		task.importedVersion = ver;
		task.mpProgressString.store((uintptr_t)"Done");
	}

	task.awaitingLoad.store(false);
}
