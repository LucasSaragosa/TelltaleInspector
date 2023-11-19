// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _MAP
#define _MAP

#include "Container.h"
#include <map>
#include "../Inspector.h"

#ifdef BUILDING_INSPECTOR

struct RenderInspectorProxyOutType {
	MetaClassDescription* pOutKeyType = nullptr;
	MetaClassDescription* pOutValType = nullptr;
};

struct RenderInspectorProxyOutInst {
	void* mpKeyInst;
	void* mpValInst;
};

#endif

//we dont need to add an stdless template , since these arent added to version headers (meta::metaoperation_serializeasync never called)
template<typename K, typename V, typename C = std::less<K>> class Map : 
	public ContainerRenderAdapter, public ContainerInterface, public std::multimap<K, V, std::less<K>>, public ContainerDataAttach<V> {
public:

	MetaClassDescription* mpContainerKeyType;

	virtual int GetSize() {
		return this->size();
	}

#ifdef BUILDING_INSPECTOR

	struct _ImGui_Getter_Ctx {
		char namebuf[16];
	};

	inline static bool _ImGui_Getter(void* pd, int idx, const char** outn) {
		_ImGui_Getter_Ctx* ctx = (_ImGui_Getter_Ctx*)pd;
		sprintf(ctx->namebuf, "[%d]", idx);
		*outn = ctx->namebuf;
		return true;
	}

	virtual void _RenderInspector(const char* pMem, const char* pimID, MetaClassDescription** ppOutSelectedType, void** pOutSelectedInst,
		const char** n, u32* s, int* oidx) override {
		RenderInspectorProxyOutInst* pOutInstances = (RenderInspectorProxyOutInst*)pOutSelectedInst;
		RenderInspectorProxyOutType* pOutTypes = (RenderInspectorProxyOutType*)ppOutSelectedType;
		pOutTypes->pOutKeyType = mpContainerKeyType;
		pOutTypes->pOutValType = mpContainerDataType;
		_ImGui_Getter_Ctx ctx{};

		*s = this->GetSize();
		//*n = "Map"; no need as the calee should know its a map

		ImGui::ListBox(pimID, oidx, &_ImGui_Getter, (void*)&ctx, (int)*s);

		if (*s) {

			auto selectedIt = this->begin();
			std::advance(selectedIt, *oidx);
		
			pOutInstances->mpKeyInst = (void*)&(selectedIt->first);
			pOutInstances->mpValInst = (void*)&(selectedIt->second);

		}

	}

#endif

	//no need for serialize main

	Map() {
		mpContainerKeyType = ::GetMetaClassDescription<K>();
	}

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj,
		MetaClassDescription* pDesc, MetaMemberDescription* mCtx, void* pUserData) {
		Map<K, V>* map = static_cast<Map<K, V>*>(pObj);
		if (!map)return eMetaOp_Fail;
		MetaClassDescription* ktype = map->GetContainerKeyClassDescription();
		MetaClassDescription* vtype = map->GetContainerDataClassDescription();
		if (!vtype || !ktype)return eMetaOp_Fail;
		MetaOperation opk = ktype->GetOperationSpecialization(74);
		if (!opk)opk = Meta::MetaOperation_SerializeAsync;
		MetaOperation opv = vtype->GetOperationSpecialization(74);
		if (!opv)opv = Meta::MetaOperation_SerializeAsync;
		unsigned int size = map->GetSize();
		static_cast<MetaStream*>(pUserData)->serialize_uint32(&size);
		for (int i = 0; i < size; i++) {
			if (static_cast<MetaStream*>(pUserData)->mMode == MetaStreamMode::eMetaStream_Write) {
				auto it = map->begin();
				std::advance(it, i);
				if (opk((void*)&it->first, ktype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
				if (opv((void*)&it->second, vtype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
			}
			else {
				K k = K();
				V v = V();
				if (opk(&k, ktype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
				if (opv(&v, vtype, NULL, pUserData) != eMetaOp_Succeed)return eMetaOp_Fail;
				map->AddElement(0, &k, &v);
			}
		}
		return eMetaOp_Succeed;
	}

	INLINE virtual bool KeyedContainer() { return true; }

	INLINE virtual void* GetKey(int index) { 
		auto it = this->begin();
		std::advance(it, index);
		return (void*)&it->first;
	}

	INLINE virtual void* GetVal(int index) {
		auto it = this->begin();
		std::advance(it, index);
		return (void*)&it->second;
	}


	virtual MetaClassDescription* GetContainerKeyClassDescription() {
		return mpContainerKeyType;
	}

	const std::pair<K,V> operator[](int at_index) {
		int i = 0;
		for (auto it = this->begin(); it != this->end(); it++, i++) {
			if (i == at_index) {
				return *it;
				break;
			}
		}
		return *(std::pair<K, V>*)NULL;
	}

	virtual MetaClassDescription* GetContainerDataClassDescription() {
		return mpContainerDataType;
	}

	INLINE virtual bool IsResizable() { return true; }

	virtual bool AllocateElements(int total) {
		return false;
	}

	virtual void ClearElements() {
		this->clear();
	}

	virtual void RemoveElement(int at_index) {
		int i = 0;
		for (auto it = this->begin(); it != this->end(); it++, i++) {
			if (i == at_index) {
				this->erase(it);
				break;
			}
		}
	}

	virtual void AddElement(int at_index, const void* pKeyData, void* pValueToAdd) {
		this->emplace(std::make_pair(*(K*)pKeyData, *(V*)pValueToAdd));
	}

	//DO NOT USE THIS IN MAP!
	virtual void SetElement(int at_index, const void* pKeyData, void* pValue) {}

	//DO NOT USE THIS IN MAP!
	virtual String* GetElementName(String* result, int index) {
		return NULL;
	}

	//DO NOT USE THIS IN MAP!
	virtual void* GetElement(int at_index) {
		return NULL;
	}
};

#endif