// This file was written by Lucas Saragosa. Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#pragma once

#pragma warning (disable : 4018 4244 4267 4554 6387 4099)

#include "../Compression.h"
#include <cstdio>
#include <memory>
#include <vector>
#include <list>

#ifndef _DATASTREAM
#define _DATASTREAM

#define DEFAULT_GROWTH_FACTOR 0x1000

typedef HANDLE FileHandle;

#define READ DataStreamMode::eMode_Read
#define WRITE DataStreamMode::eMode_Write

//Returns a new instance as an object directly 
#define _OpenDataStreamFromDisc_(file_path, mode) DataStreamFileDisc(\
PlatformSpecOpenFile(file_path,\
	mode),\
	mode)

//Returns a new instance as a pointer which needs to be deleted. Most classes
//will delete it when done with it (consumers)
#define _OpenDataStreamFromDisc(file_path, mode) \
new _OpenDataStreamFromDisc_(file_path, mode)

//With line terminator
#define OpenDataStreamFromDisc(file_path, mode) \
_OpenDataStreamFromDisc(file_path, mode);

HANDLE openfile_s_(const char* fp, const char* m);

//this is for windows, if on POSIX then include unistd and set the platform specific truncate function to truncate
#include <io.h>
#define PlatformSpecTrunc(handle, newsize) SetEndOfFile((HANDLE)handle)
#define PlatformSpecOpenFile(file_path,mode)\
 openfile_s_(file_path,mode == DataStreamMode::eMode_Write ? "wb" : "rb")
#define PlatforSpecCloseFile(handle) CloseHandle(handle)

enum class DataStreamMode : unsigned char {
	eMode_Unset = 0,
	eMode_Read = 1,
	eMode_Write = 2
};

enum class DataStreamSeekType : unsigned char {
	eSeekType_Begin = 0,
	eSeekType_Current = 1,
	eSeekType_End = 2
};

class DataStreamSubStream;

/*
* A data stream. Abstract class which represents a stream of data (bytes) being read or written.
* Class is *not copyable* but is moveable.
*/
class DataStream {
public:
	DataStreamMode mMode;
	int mSubStreams;

	virtual bool Copy(DataStream* pDst, unsigned __int64 pDstOffset, unsigned __int64 pSrcOffset, unsigned __int64 size);

	/*
	* Serialize bytes. First is the buffer, second is the size. The mode member variable decides if its write or reading into the buffer.
	* Returns if this function was successful. 
	*/
	virtual bool Serialize(char*, unsigned __int64) = 0;

	/*
	* Serialize helper function, to write a const pointer instead.
	* This is not the writing function, Serialize writes and reads depending
	* on the mode.
	*/
	virtual bool SerializeWrite(const char* ptr, unsigned __int64 size) {
		if (IsWrite()) {
			return Serialize((char*)((void*)ptr), size);
		}
		else if (!size)return true;
		return false;
	}

	virtual bool SerializeStringRead(char* dest, unsigned __int64 size) {
		if (IsRead()) {
			return Serialize(dest, size);
		}
		return false;
	}

	virtual bool SerializeStringWrite(const char* str) {
		return SerializeWrite(str, strlen(str));
	}

	/*
	* Gets the size in bytes of this stream.
	*/
	virtual unsigned __int64 GetSize() const = 0;

	/*
	* Transfers bytes from this stream to the given stream
	*/
	virtual bool Transfer(DataStream* dst, unsigned __int64 off, unsigned __int64 size) = 0;

	/*
	* Gets the position (offset) of this stream.
	*/
	virtual unsigned __int64 GetPosition() const = 0;

	/*
	* Sets the position or offset of this stream. The position is deduced by the seek type parameter.
	*/
	virtual bool SetPosition(signed __int64, DataStreamSeekType) = 0;

	/*
	* Truncates this stream to the given new size. If its over the stream size, adds zeros (if this stream can do that, otherwise will
	* return false) otherwise it will remove all excess bytes. Not available for all types of data stream.
	* Only works in write mode (will return false if its not in write mode)
	*/
	virtual bool Truncate(unsigned __int64) = 0;

	/*
	* Gets a sub-stream of this stream. A sub-stream is a READ-ONLY (will return null if its not read mode) stream which points
	* to a section of a data stream. Like a std::string_view for a std::basic_string/std::string
	*/
	virtual DataStreamSubStream* GetSubStream(unsigned __int64 off,unsigned __int64 size);
	
	/*
	* Sets the mode of this stream. If there are substreams attached to this one and you try to set it to not read then it fails.
	*/
	bool SetMode(DataStreamMode);

	virtual bool IsRead() { return mMode == DataStreamMode::eMode_Read; }
	virtual bool IsWrite() { return mMode == DataStreamMode::eMode_Write; }
	virtual bool IsInvalid() { return mMode == DataStreamMode::eMode_Unset; }

	DataStream& operator=(DataStream&&) ;
	DataStream& operator=(DataStream const&) = delete;
	DataStream(DataStream& const) = delete;
	DataStream(DataStream&&) ;
	DataStream() : mMode(DataStreamMode::eMode_Unset), mSubStreams(0) {}
	DataStream(DataStreamMode mode) : mMode(mode), mSubStreams(0) {}

	virtual ~DataStream() = default;

};

/*
* A data stream implementation which comes from a file on disc (specificly for Windows, implement this for other platforms if you need!).
* This class can be optimized a lot (using runtime buffers like Java's BufferedReader and BufferedWriter), so feel free to reimplement that.
*/
class DataStreamFile_Win : public DataStream {
public:
	HANDLE mHandle;
	__int64 mStreamOffset, mStreamSize;

	bool Serialize(char*, unsigned __int64);
	unsigned __int64 GetSize() const { return mStreamSize; }
	unsigned __int64 GetPosition() const { return mStreamOffset; };
	bool SetPosition(signed __int64, DataStreamSeekType);
	bool Truncate(unsigned __int64);
	//cant transfer from a file, only used for memory streams
	bool Transfer(DataStream* dst, unsigned __int64 off, unsigned __int64 size);

	DataStreamFile_Win(HANDLE H, DataStreamMode);
	DataStreamFile_Win(DataStreamFile_Win&&);
	DataStreamFile_Win& operator=(DataStreamFile_Win&&);
	DataStreamFile_Win(DataStreamFile_Win const&) = delete;
	DataStreamFile_Win& operator=(DataStreamFile_Win& const) = delete;
	virtual ~DataStreamFile_Win() { 
		if (!IsInvalid()) {
			PlatforSpecCloseFile((HANDLE)mHandle);
		}
	};

};

class DataStreamSubStream : public DataStream {
public:
	DataStream* mpBase;
	unsigned __int64 mOffset, mStreamOffset, mSize;

	bool Serialize(char*, unsigned __int64);

	unsigned __int64 GetSize() const { return mSize; }

	unsigned __int64 GetPosition() const { return mOffset; };

	bool SetPosition(signed __int64, DataStreamSeekType);

	bool Truncate(unsigned __int64 newz) {
		if (newz <= mSize) {
			mSize = newz;
			if (mOffset > mSize)
				mOffset = mSize;
			return true;
		}
		return false;
	};

	bool Transfer(DataStream* dst, unsigned __int64 off, unsigned __int64 size);

	virtual DataStreamSubStream* GetSubStream(unsigned __int64 off, unsigned __int64 size);

	DataStreamSubStream(DataStream*, unsigned __int64  size);//substream starts at base stream current offset, size is size (param 2)
	DataStreamSubStream(DataStream*, unsigned __int64 size, unsigned __int64 baseOffset);
	DataStreamSubStream(DataStreamSubStream&&);
	DataStreamSubStream& operator=(DataStreamSubStream&&);
	DataStreamSubStream(DataStreamSubStream& const) = delete;
	DataStreamSubStream& operator=(DataStreamSubStream& const) = delete;

	~DataStreamSubStream();

};

class DataStreamMemory : public DataStream {
public:
	//std::vector<char*> mPageTable;
	//unsigned __int64 mPageSize;
	unsigned __int64 mSize;
	unsigned __int64 mOffset;
	unsigned __int64 mGFact = DEFAULT_GROWTH_FACTOR;
	void* mMemoryBuffer;

	bool Serialize(char*, unsigned __int64);
	unsigned __int64 GetSize() const { return mSize; }
	unsigned __int64 GetPosition() const { return mOffset; }
	bool SetPosition(signed __int64, DataStreamSeekType);
	bool Truncate(unsigned __int64 new_size);
	bool Transfer(DataStream* dst, unsigned __int64 off, unsigned __int64 size);

	inline unsigned __int64 GetMemoryBufferSize() {
		int memorybufsize = mSize;
		if (mSize % mGFact)memorybufsize += mGFact - (mSize % mGFact);
		return memorybufsize;
	}


	//buffer param needs to be allocated with malloc/calloc
	DataStreamMemory(void* buffer, unsigned __int64 size,DataStreamMode m) : mMemoryBuffer(buffer), mSize(size), mOffset(0), DataStream(m) {}
	DataStreamMemory(void* buffer, unsigned __int64 size, unsigned __int64 growthFactor, DataStreamMode m)
		: mMemoryBuffer(buffer), mSize(size), mOffset(0), mGFact(growthFactor), DataStream(m) {};
	DataStreamMemory(unsigned __int64 initialSize);
	DataStreamMemory(unsigned __int64 initialSize, unsigned __int64 growthFactor);
	DataStreamMemory(DataStreamMemory&&);
	DataStreamMemory& operator=(DataStreamMemory&&);
	DataStreamMemory(DataStreamMemory const&) = delete;
	DataStreamMemory& operator=(DataStreamMemory& const) = delete;
	~DataStreamMemory();
};

//*NOT COPYABLE OR MOVABLE* A legacy encrypted stream which decrypts in chunks for old games which us MBIN, MBES.
//NOT WRITABLE, ONLY READABLE!
class DataStreamLegacyEncrypted : public DataStream {
	DataStream* mpBase;
	unsigned int mHeader;//start pos
	unsigned int mEncryptSize, mEncryptInterval, mEncryptSkip;
	unsigned __int64 mSize, mOffset;
	int mCurrentBlock;
public:
	char mBuf[0x100];

	bool Serialize(char*, unsigned __int64);
	unsigned __int64 GetSize() const { return mSize + mHeader; }
	unsigned __int64 GetPosition() const { return mOffset + mHeader; }
	bool SetPosition(signed __int64, DataStreamSeekType);
	bool Truncate(unsigned __int64 new_size) {
		return false;
	};
	bool Transfer(DataStream* dst, unsigned __int64 off, unsigned __int64 size) { return false; }
	DataStreamLegacyEncrypted(DataStream*,int version, unsigned int startPos);
	DataStreamLegacyEncrypted(DataStreamLegacyEncrypted&&) = delete;
	DataStreamLegacyEncrypted& operator=(DataStreamLegacyEncrypted&&) = delete;
	DataStreamLegacyEncrypted(DataStreamLegacyEncrypted const&) = delete;
	DataStreamLegacyEncrypted& operator=(DataStreamLegacyEncrypted& const) = delete;

};

struct DataStreamContainerParams {
	DataStream* mpSrcStream;
	DataStream* mpDstStream;
	//offset which to start serialing to in the destination stream
	unsigned __int64 mDstOffset;
	unsigned __int32 mWindowSize;
	bool mbCompress;
	bool mbEncrypt;
	Compression::Library mCompressionLibrary;

	DataStreamContainerParams() : mWindowSize(0x10000), mCompressionLibrary(Compression::Library::ZLIB) {}

};
//A compressed/encrypted data stream container used for READING data.
//To write use Create. This is where TTNC,TTCZ,TTCE,TTCe,TTCz is.
//In read mode takes ownership of the DataStreamContainerParams::mSrcStream (deletes)
class DataStreamContainer : public DataStream {

	//page and chunk are synonymous

	void GetChunk(unsigned __int64 index);

	inline unsigned __int64 GetCompressedPageSize(unsigned __int32 index);

public:

	typedef void (*ProgressF)(const char* _Msg, float _Progress);

	unsigned __int64 mStreamOffset, mStreamPosition, mStreamSize;
	unsigned __int64 mStreamStart;
	DataStreamContainerParams mParams;
	char* mpCachedPage;//0x32
	char* mpReadTransitionBuf;
	signed __int32 mCurrentIndex;// , mCacheablePages;
	unsigned __int64* mPageOffsets;
	unsigned __int64 mNumPages;
	bool ok = false;

	//init from src stream
	void Read(unsigned __int64 offset, unsigned __int64* pContainerSize);

	//Creates a TT data stream container with the parameters. Serializes from src to dest. srcInStreamSize is the amount of bytes to
	//serialize from the src stream from the source streams current offset
	//progres function starts progress percentage at 80 (assumming you use this with .ttarch, otherwise just modify the function input)
	static void Create(ProgressF, DataStreamContainerParams, unsigned __int64 srcInStreamSize);
	bool SetPosition(signed __int64, DataStreamSeekType);
	bool Serialize(char*, unsigned __int64);

	unsigned __int64 GetSize() const { return mStreamSize; }
	unsigned __int64 GetPosition() const { return mStreamPosition; }

	bool Truncate(unsigned __int64 new_size) {
		return false;
	};

	bool Transfer(DataStream* dst, unsigned __int64 off, unsigned __int64 size) {
		return Copy(dst, dst->GetPosition(), off, size);
	}

	DataStreamContainer(DataStreamContainerParams params) : mParams(params), mStreamOffset(0), /*mCacheablePages(-1),*/ mpReadTransitionBuf(NULL),
		mStreamSize(0), mStreamStart(0),
		mCurrentIndex(-1), mStreamPosition(0), mNumPages(0), mPageOffsets(NULL), mpCachedPage(NULL)
		, DataStream(DataStreamMode::eMode_Read) {
	}//Create

	~DataStreamContainer();

	inline Compression::Library GetCompressionLibrary() { return mParams.mCompressionLibrary; }

	inline bool IsCompressed() {
		return mParams.mbCompress;
	};

	inline bool IsEncrypted() {
		return mParams.mbEncrypt;
	}

	DataStreamContainer(DataStreamContainer&&) = delete;
	DataStreamContainer& operator=(DataStreamContainer&&) = delete;
	DataStreamContainer(DataStreamContainer const&) = delete;
	DataStreamContainer& operator=(DataStreamContainer& const) = delete;

};

/*
* Change this to the class you want to use as the actual file reading implementation of DataStream for your platform.
*/
typedef DataStreamFile_Win DataStreamFileDisc;

#endif