// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _COMPRESSION
#define _COMPRESSION

#include "Zlib/zlib.h"
//Windows platform specific DLL loading for OODLE
#include <Windows.h>

#define _TTToolLib_Exp extern "C" __declspec(dllexport)

#define TTEXPORT _TTToolLib_Exp

namespace Compression {

	typedef long long (*OodleLZ_Compress)(int algo, const void *pSrc, unsigned int srcLen, void* dst, long long max, void* a, void* b, void* c);
	//typedef long long (*OodleLZ_Compress)(int algo, unsigned char* in, int insz, unsigned char* out, long long max, void* a, void* b, void* c);
	typedef int (*OodleLZ_Decompress)(void* in, int insz, void* out, long long outsz, long long a, long long b, long long c, void* d, void* e, void* f, void* g, void* h, void* i, long long j);

	enum class Library {
		ZLIB = 0, OODLE = 1, END_LIBRARY = 2
	};

	TTEXPORT void UnloadOodleLibrary(HMODULE mod);

	TTEXPORT HMODULE LoadOodleLibrary(const char* pDLLName);

	TTEXPORT bool ZlibDecompress(void* pDst, unsigned int* pDstLength, const void* pSrc, unsigned int srcLength);

	TTEXPORT bool ZlibCompress(void* pDst, unsigned int* pDstLength, const void* pSrc, unsigned int srcLength);

	TTEXPORT bool OodleLZCompress(void* pDst, unsigned int* dstLength, const void* pSrc, unsigned int srcLength, HMODULE);

	TTEXPORT bool OodleLZDecompress(void* pDst, unsigned int dstLength, const void* pSrc, unsigned int srcLength, HMODULE);

}

#endif