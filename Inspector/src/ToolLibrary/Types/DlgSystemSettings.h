// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _DSS
#define _DSS

#include "../TelltaleToolLibrary.h"
#include "DCArray.h"
#include "../Meta.hpp"
#include "PropertySet.h"
#include "HandleObjectInfo.h"

struct DlgObjectPropsMap : public UID::Generator {

	struct GroupDefinition : public UID::Owner {
		int mVer;
		int mGroupCat;
		Handle<PropertySet> mhProps;

		GroupDefinition() {
			mVer = 2;
			mGroupCat = 1000;
		}

		static MetaOpResult MetaOperation_SerializeAsync(void* pObj,
			MetaClassDescription* pObjDesc, MetaMemberDescription* pCtx, void* pUserData) {
			return Meta::MetaOperation_SerializeAsync(pObj, pObjDesc, pCtx, pUserData);
		}

	};

	DCArray<GroupDefinition*> mGroupDefs;

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj,
		MetaClassDescription* pObjDesc, MetaMemberDescription* pCtx, void* pUserData) {
		MetaStream* stream = static_cast<MetaStream*>(pUserData);
		DlgObjectPropsMap* map = static_cast<DlgObjectPropsMap*>(pObj);
		u32 num = map->mGroupDefs.GetSize();
		stream->serialize_uint32(&num);
		bool write = stream->mMode == MetaStreamMode::eMetaStream_Write;
		MetaClassDescription* mcd = GetMetaClassDescription(
			typeid(GroupDefinition).name());
		if (!mcd)return eMetaOp_Fail;
		MetaOperation ser = mcd->GetOperationSpecialization(74);
		if (!ser)
			ser = Meta::MetaOperation_SerializeAsync;
		if (!write) {
			map->mGroupDefs.Clear(0);
			map->mGroupDefs.ReserveAndResize(num);
			for (int i = 0; i < num; i++) {
				map->mGroupDefs.mpStorage[i] = new GroupDefinition;
			}
		}
		for (int i = 0; i < num; i++) {
			ser(map->mGroupDefs.mpStorage[i], mcd, NULL, pUserData);
		}
		return eMetaOp_Succeed;
	}

};

//.DSS FILES
struct DlgSystemSettings {

	static constexpr const char* msSysFilename = "dialog_system_settings.dss";

	DlgObjectPropsMap mPropsMapUser;
	DlgObjectPropsMap mPropsMapProduction;
};

#endif