// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.
#pragma warning(disable C4267 C4244 C4554 C4477)
#include "T3Texture.h"

T3SamplerStateBlock kDefault;

bool T3SamplerStateBlock::sInitialized = false;
T3SamplerStateBlock::SamplerStateEntry T3SamplerStateBlock::smEntries[6] = {};