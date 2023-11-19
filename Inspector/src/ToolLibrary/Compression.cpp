// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.
#pragma warning(disable C4267 C4244 C4554 C4477)
#include "Compression.h"
#include <iostream>

HMODULE Compression::LoadOodleLibrary(const char* pDLLName) {
	return LoadLibraryA(pDLLName);
}

void Compression::UnloadOodleLibrary(HMODULE mod) {
	FreeLibrary(mod);
}

bool Compression::OodleLZCompress(void* pDst, unsigned int* dstLength, const void* pSrc, unsigned int srcLength,HMODULE l) {
	OodleLZ_Compress compressor = (OodleLZ_Compress)GetProcAddress(l, "OodleLZ_Compress");
	if (!compressor)return false;
	*dstLength = compressor(6, pSrc, srcLength, pDst,7,0,0,0);
	return true;
}

bool Compression::OodleLZDecompress(void* pDst, unsigned int dstLength, const void* pSrc, unsigned int srcLength, HMODULE l) {
	OodleLZ_Decompress decompressor = (OodleLZ_Decompress)GetProcAddress(l, "OodleLZ_Decompress");
	if (!decompressor)return false;
	decompressor((void*)pSrc, srcLength, pDst, dstLength, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, 3);
	return true;
}

bool Compression::ZlibDecompress(void* dest, unsigned int* destLen, const void* source, unsigned int sourceLen) {
	z_stream stream;
	int err;
	const uInt max = (uInt)-1;
	uLong len, left;
	unsigned char buf[1];

	len = sourceLen;
	if (*destLen) {
		left = *destLen;
		*destLen = 0;
	}
	else {
		left = 1;
		dest = buf;
	}

	stream.next_in = (z_const Bytef*)source;
	stream.avail_in = 0;
	stream.zalloc = (alloc_func)0;
	stream.zfree = (free_func)0;
	stream.opaque = (voidpf)0;

	err = inflateInit2(&stream, -15);
	if (err != Z_OK) return err;

	stream.next_out = (Bytef*)dest;
	stream.avail_out = 0;

	do {
		if (stream.avail_out == 0) {
			stream.avail_out = left > (uLong)max ? max : (uInt)left;
			left -= stream.avail_out;
		}
		if (stream.avail_in == 0) {
			stream.avail_in = len > (uLong)max ? max : (uInt)len;
			len -= stream.avail_in;
		}
		err = inflate(&stream, Z_NO_FLUSH);
	} while (err == Z_OK);

	sourceLen -= len + stream.avail_in;
	if (dest != buf)
		*destLen = stream.total_out;
	else if (stream.total_out && err == Z_BUF_ERROR)
		left = 1;

	inflateEnd(&stream);
	return  err == Z_STREAM_END ? true /*Z_OK*/ :
		err == Z_NEED_DICT ? false /*Z_DATA_ERROR*/ :
		err == Z_BUF_ERROR && left + stream.avail_out ? false /*Z_DATA_ERROR*/ :
		false /*err*/;
}

bool Compression::ZlibCompress(void* pDst, unsigned int* pDstLength, const void* pSrc, unsigned int srcLength) {
	z_stream_s stream{ 0 };
	stream.next_in = (Bytef*)pSrc;
	stream.avail_in = srcLength;
	if(!*pDstLength)*pDstLength = compressBound(srcLength);
	stream.avail_out = *pDstLength;
	stream.next_out = (Bytef*)pDst;
	if (!deflateInit2_(&stream, Z_BEST_SPEED, Z_DEFLATED, -15, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY, "1.2.8", sizeof(stream))) {
		if (deflate(&stream, Z_FINISH) == 1) {//z finish
			*pDstLength = stream.total_out;
			return deflateEnd(&stream) == Z_OK;
		}
		deflateEnd(&stream);
	}
	return false;
}