// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _LLM
#define _LLM

#include "../Meta.hpp"
#include "DCArray.h"
#include "Dialog.h"
#include "Set.h"
#include "TRange.h"
#include "HandleObjectInfo.h"

//.LLM FILES
struct LanguageLookupMap {

	constexpr static const char* smMapName = "LanguageLookup.llm";

	struct DlgIDSet {
		TRange<unsigned int> mIDRange;
		Set<unsigned int> mAdditionalIDs;
		Handle<Dlg> mhDlg;
	};

	DCArray<DlgIDSet> mIDSets;

};

#endif