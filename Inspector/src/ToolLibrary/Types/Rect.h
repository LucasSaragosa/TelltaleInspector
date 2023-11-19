// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

struct Rect /*synonymous with doing : public tagRECT from the WIN32 API (compiler would see them as identical!)*/{
	int left, top, right, bottom;
};