// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _BUNDLE
#define _BUNDLE

#include "../Meta.hpp"
#include "DCArray.h"

//.BUNDLE FILES - NOT IMPLEMENTED FULLY IN LIBRARY (NEEDS GAME ENGINE)
struct ResourceBundle {

	enum ResourceState : u32 {
		eResourceState_Unknown = 0,
		eResourceState_Internal = 1,
		eResourceState_External = 2,
		eResourceState_New = 3,
		eResourceState_InternalInOtherBundle = 4
	};

	struct ResourceInfo {
		Symbol mResNameSymbol;
		unsigned long mStreamOffset, mStreamSize;
		ResourceState mState;
		MetaClassDescription* mpDesc;
	};

	long mVersion;
	String mName;
	DCArray<ResourceInfo> mResourceInfo;

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		ResourceBundle* bundle = static_cast<ResourceBundle*>(pObj);
		if (meta->mMode == MetaStreamMode::eMetaStream_Read) {

		}
		else {
			meta->BeginAsyncSection();
			u64 currentpos;
			for (int i = 0; i < bundle->mResourceInfo.GetSize(); i++) {
				ResourceInfo* info = bundle->mResourceInfo.mpStorage + i;
				currentpos = meta->GetPos();
				if (info->mState != 
					ResourceState::eResourceState_InternalInOtherBundle) {

				}
			}
		}
		return eMetaOp_Succeed;
	}

};

#endif