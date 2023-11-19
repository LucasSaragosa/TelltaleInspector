// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#include "../Meta.hpp"

#ifndef _STATIC_ARRAY
#define _STATIC_ARRAY

template<typename T, int N> class SArray: public ContainerDataAttach<T> {
public:
	T mData[N];
	int NUM_DATA_ELEM = N;

	SArray() = default;

	virtual MetaClassDescription* GetContainerDataDescription() const {
		return GetMetaClassDescription<T>();
	}

	static MetaOpResult MetaOperation_SerializeMain(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pContext,
		void* pUserData) {
		SArray<T, N>* array = static_cast<SArray<T, N>*>(pObj);
		MetaClassDescription* vtype = GetMetaClassDescription(typeid(T).name());
		if (!array || !vtype)return eMetaOp_Fail;
		MetaOperation op = vtype->GetOperationSpecialization(75);
		if (!op)op = Meta::MetaOperation_SerializeMain;
		for (int i = 0; i < N; i++) {
			if (op(&array->mData[i], vtype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
		}
		return eMetaOp_Succeed;
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pDesc, MetaMemberDescription* pContext,
		void* pUserData) {
		SArray<T, N>* array = static_cast<SArray<T, N>*>(pObj);
		MetaStream* meta = static_cast<MetaStream*>(pUserData);
		MetaClassDescription* vtype = GetMetaClassDescription(typeid(T).name());
		if (!array || !meta)return eMetaOp_Fail;
		if (!vtype) {
#ifdef DEBUGMODE
			printf("Value type %s has no metaclass\n", typeid(T).name());
#endif
			TelltaleToolLib_RaiseError("Value type metaclass not found for SArray type", ErrorSeverity::ERR);
			return eMetaOp_Fail;
		}
		MetaOperation op = vtype->GetOperationSpecialization(74);
		if (!op)op = Meta::MetaOperation_SerializeAsync;
		meta->BeginObject("SArray", true);
		for (int i = 0; i < N; i++) {
			if (op(&array->mData[i], vtype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
		}
		meta->EndObject("SArray");
		return eMetaOp_Succeed;
	}

};

#endif