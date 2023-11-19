// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#pragma once

#include "../Meta.hpp"

template<typename T>
class ContainerDataAttach {
public:
	MetaClassDescription* mpContainerDataType;

	inline ContainerDataAttach() {
		mpContainerDataType = GetMetaClassDescription<T>();
	}

};

class ContainerInterface {
public:

	struct Iterator {
		void* mpInternalIter;
		void(*mfpDestroy)(void*);

		~Iterator() {
			if (mfpDestroy)
				mfpDestroy(mpInternalIter);
		}

	};

	virtual int GetSize() = 0;
	virtual bool KeyedContainer() = 0;
	virtual void* GetKey(int) = 0;
	virtual MetaClassDescription* GetContainerKeyClassDescription() = 0;
	virtual MetaClassDescription* GetContainerDataClassDescription() = 0;
	virtual bool IsResizable() = 0;
	virtual bool AllocateElements(int total) = 0;
	virtual void ClearElements() = 0;
	virtual void RemoveElement(int at_index) = 0;
	virtual void AddElement(int at_index, const void* pKeyData, void* pValueToAdd) = 0;
	virtual void SetElement(int at_index, const void* pKeyData, void* pValue) = 0;
	virtual String* GetElementName(String* result,int index) = 0;
	virtual void* GetElement(int) = 0;

	static void DestroyIterator(void* it) {
		delete it;
	}

	ContainerInterface() {}
	virtual ~ContainerInterface() {}

	virtual Iterator* BeginIteration(Iterator* result) {
		result->mpInternalIter = new int(0);
		result->mfpDestroy = DestroyIterator;
		return result;
	}

	virtual void ResetIteration(Iterator* pIterationData) {
		if (pIterationData->mfpDestroy)
			pIterationData->mfpDestroy(pIterationData->mpInternalIter);
		BeginIteration(pIterationData);
	}

	virtual bool IsIterationFinished(Iterator* pIterationData) {
		return *(int*)pIterationData->mpInternalIter >= GetSize();
	}

	virtual bool AdvanceIteration(Iterator* pIterationData) {
		return ++(*(int*)pIterationData->mpInternalIter) < GetSize();
	}

	virtual String* GetIteratedElementName(String* result, Iterator* pIterationData) {
		return GetElementName(result, *(int*)pIterationData->mpInternalIter);
	}

	virtual void* GetIteratedElement(Iterator* pIterationData) {
		return GetElement(*(int*)pIterationData->mpInternalIter);
	}

	virtual void* GetIteratedKey(Iterator* pIterationData) {
		return GetKey(*(int*)pIterationData->mpInternalIter);
	}

	virtual MetaClassDescription* GetMetaClassDescription() {
		return ::GetMetaClassDescription(typeid(ContainerInterface).name());
	}

};