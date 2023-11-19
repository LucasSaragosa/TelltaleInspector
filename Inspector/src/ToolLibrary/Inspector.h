#ifndef _TT_INSPECTOR_H
#define _TT_INSPECTOR_H

#include "Meta.hpp"
#include <functional>

#ifdef BUILDING_INSPECTOR

#include "imgui.h"

class ContainerRenderAdapter {
public:

	virtual void _RenderInspector(const char* pMem, const char* pimID,
		MetaClassDescription** ppOutSelectedType, 
		void** ppOutSelectedInst, const char** ppOutContainerNameType, u32* pOutContainerSize, int* selectedIdx) = 0;

};

#else

class ContainerRenderAdapter {

};

#endif

#endif