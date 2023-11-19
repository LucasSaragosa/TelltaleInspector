// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _TRECT
#define _TRECT

template<typename T>
struct TRect {
	T left, right, top, bottom;
};

template class TRect<float>;

#endif