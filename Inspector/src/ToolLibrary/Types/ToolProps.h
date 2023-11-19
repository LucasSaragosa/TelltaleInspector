// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _TOOL_PROPS
#define _TOOL_PROPS

#include "../Meta.hpp"
#include "../TelltaleToolLibrary.h"
#include "PropertySet.h"

struct ToolProps {
	bool mbHasProps;
	//in the engine there would be more members here (probably PropertySet mProps) but its built as the runtime engine, not the tool.

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDesc, MetaMemberDescription* pCtx,
		void* pUserData) {
		MetaStream* stream = static_cast<MetaStream*>(pUserData);
		ToolProps* props = static_cast<ToolProps*>(pObj);
		if (stream->mMode == MetaStreamMode::eMetaStream_Write) {
			props->mbHasProps = false;//we are not in the tool engine, runtime engine. would normally be a #ifdef COMPILING_AS_TOOL etc
		}
		stream->serialize_bool(&props->mbHasProps);
		if (props->mbHasProps) {
			PropertySet prop;
			PerformMetaSerializeAsync(stream, &prop);
		}
		return eMetaOp_Succeed;
	}

};

#endif