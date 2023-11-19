#ifndef _META_STREAM_JSON_H
#define _META_STREAM_JSON_H

#include "Meta.hpp"
#include "Types/List.h"
#include "rapidjson/LookaheadParse.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

class MetaStream_JSON : public MetaStream {
public:

	enum eJSONMode : int {
		eCompleteMetaStream = 1,
		//eServerConsumptionOnly = 2,
		eVersionlessMetaStream = 3,
		//eCleanVersionlessJson = 4,
	};
	
	MetaStream_JSON(eJSONMode);
	
	INLINE ~MetaStream_JSON() {
		_DeleteJSONData();
		Close();
	}

	INLINE virtual MetaStream::StreamType GetStreamType() override { return StreamType::eStream_JSON; };
	virtual u64 Close() override;//returns size of stream
	virtual bool Attach(DataStream*, MetaStreamMode, MetaStreamParams) override;

	virtual u64 GetPartialStreamSize() override;

protected:

	virtual i64 ReadData(void*, u32) override;
	virtual i64 WriteData(void*, u32) override;

public:

	virtual void DisableDebugSection() override;
	virtual bool BeginAsyncSection() override;
	virtual void EndAsyncSection() override;
	virtual bool HasAsyncSection() override;
	virtual bool BeginDebugSection() override;
	virtual void EndDebugSection() override;
	virtual bool HasDebugSection() override;

	virtual u64 GetSize() override;

	/*TODO IMPORTANT:  CHECK ALL OCCURENCES THESE DONT DO ANYTHING*/
	virtual u64 GetPos() override;
	virtual void SetPos(u64) override;
	virtual void Advance(int numBytes) override;
	virtual void SkipToEndOfCurrentBlock() override;

	virtual void BeginBlock() override;
	virtual void EndBlock() override;

	virtual void BeginObject(const char*, bool bArray) override;
	virtual void EndObject(const char*) override;

	virtual void BeginObject(const char*, MetaClassDescription*, MetaMemberDescription*, bool bArray) override;
	virtual void EndObject(const char*, MetaClassDescription*, MetaMemberDescription*) override;

	virtual void serialize_String(String*) override;
	virtual void serialize_Symbol(Symbol*) override;
	virtual void serialize_bool(bool*) override;
	virtual void serialize_double(long double*) override;
	virtual void serialize_float(float*) override;
	virtual void serialize_uint16(u16*) override;
	virtual void serialize_uint32(u32*) override;
	virtual void serialize_uint64(u64*) override;
	virtual void serialize_int8(char*) override;

	virtual void InsertStringHint(const char* pK, const char* pS) override;
	virtual const char* SkipStringHint(const char* pK) override;

	virtual void Key(const char* pS) override;
	virtual void HintSymbol(const char* symbolData) override;

	struct JSONWriter {
		rapidjson::StringBuffer mBuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> mWriter;

		INLINE JSONWriter() : mBuffer(), mWriter(mBuffer) {}

	};

	virtual bool NeedsKey() override;

	INLINE void _DeleteJSONData() {
		if (mpRead)
			delete[] mpRead;
		if (mpParser)
			delete mpParser;
		if (mpWriter)
			delete mpWriter;
		mpWriter = nullptr;
		mpParser = nullptr;
		mpRead = nullptr;
	}

	char* mpRead;
	eJSONMode mJSONMode;
	LookaheadParser* mpParser;
	JSONWriter* mpWriter;
	std::stack<bool> mbObjectIsArrayStack;
	const char* pCachedSymbolVal;

};

#endif