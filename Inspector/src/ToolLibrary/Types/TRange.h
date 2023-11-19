// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _RANGE_T
#define _RANGE_T

template<typename T>
struct TRange {
	T min, max;

};

//Types the Telltale Tool implements
template class TRange<unsigned int>;
template class TRange<float>;

#endif