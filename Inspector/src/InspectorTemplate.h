#include "ToolLibrary/Types/List.h"
#include "ToolLibrary/Types/Set.h"
#include "ToolLibrary/Types/Deque.h"
#include "ToolLibrary/Types/Map.h"

struct _ImGui_Getter_Ctx {
	char namebuf[16];
};

struct RenderInspectorProxyOutType {
	MetaClassDescription* pOutKeyType = nullptr;
	MetaClassDescription* pOutValType = nullptr;
};

struct RenderInspectorProxyOutInst {
	void* mpKeyInst;
	void* mpValInst;
};


inline static bool _ImGui_Getter(void* pd, int idx, const char** outn) {
	_ImGui_Getter_Ctx* ctx = (_ImGui_Getter_Ctx*)pd;
	sprintf(ctx->namebuf, "[%d]", idx);
	*outn = ctx->namebuf;
	return true;
}

inline void _RenderInspector_1DCol(ContainerInterface* pMyself, const char* pMem, const char* pimID,
	MetaClassDescription** ppOutSelectedType, void** pOutSelectedInst, const char** n, u32* s, int* oidx) {
	_ImGui_Getter_Ctx ctx{};

	*n = "List";
	*s = ((ContainerInterface*)pMyself)->GetSize();
	*ppOutSelectedType = pMyself->GetContainerDataClassDescription();
	ImGui::ListBox(pimID, oidx, &_ImGui_Getter, (void*)&ctx, (int)*s);

	if (*s) {

		*pOutSelectedInst = pMyself->GetElement(*oidx);

	}

}

inline void _RenderInspector_Map(ContainerInterface* pMyself, const char* pMem, const char* pimID, MetaClassDescription** ppOutSelectedType, void** pOutSelectedInst,
	const char** n, u32* s, int* oidx) {
	RenderInspectorProxyOutInst* pOutInstances = (RenderInspectorProxyOutInst*)pOutSelectedInst;
	RenderInspectorProxyOutType* pOutTypes = (RenderInspectorProxyOutType*)ppOutSelectedType;
	pOutTypes->pOutKeyType = pMyself->GetContainerKeyClassDescription();
	pOutTypes->pOutValType = pMyself->GetContainerDataClassDescription();
	_ImGui_Getter_Ctx ctx{};

	*s = pMyself->GetSize();
	//*n = "Map"; no need as the calee should know its a map

	ImGui::ListBox(pimID, oidx, &_ImGui_Getter, (void*)&ctx, (int)*s);

	if (*s) {

		pOutInstances->mpKeyInst = ((ContainerInterface*)pMyself)->GetKey(*oidx);
		pOutInstances->mpValInst = ((ContainerInterface*)pMyself)->GetElement(*oidx);

	}

}
