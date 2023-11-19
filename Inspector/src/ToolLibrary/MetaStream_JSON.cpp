#include "MetaStream_JSON.hpp"
#include "Base64.h"

MetaStream_JSON::MetaStream_JSON(eJSONMode m) : MetaStream(), mpParser(nullptr), mpWriter(nullptr)
{
	mJSONMode = m;
	pCachedSymbolVal = nullptr;
	mpRead = nullptr;
}

LookaheadParserHandler::LookaheadParserHandler(char* str) : v_(), st_(kInit), r_(), ss_(str) {
	r_.IterativeParseInit();
	ParseNext();
}

void LookaheadParserHandler::ParseNext() {
	if (r_.HasParseError()) {
		st_ = kError;
		return;
	}

	r_.IterativeParseNext<parseFlags>(ss_, *this);
}


bool LookaheadParser::EnterObject() {
	if (st_ != kEnteringObject) {
		st_ = kError;
		return false;
	}

	ParseNext();
	return true;
}

bool LookaheadParser::EnterArray() {
	if (st_ != kEnteringArray) {
		st_ = kError;
		return false;
	}

	ParseNext();
	return true;
}

const char* LookaheadParser::NextObjectKey() {
	if (st_ == kHasKey) {
		const char* result = v_.GetString();
		ParseNext();
		return result;
	}

	if (st_ != kExitingObject) {
		st_ = kError;
		return 0;
	}

	ParseNext();
	return NextObjectKey();
}

bool LookaheadParser::NextArrayValue() {
	if (st_ == kExitingArray) {
		ParseNext();
		return false;
	}

	if (st_ == kError || st_ == kExitingObject || st_ == kHasKey) {
		st_ = kError;
		return false;
	}

	return true;
}

unsigned long long LookaheadParser::GetULong() {
	if (st_ != kHasNumber || !v_.IsUint64()) {
		st_ = kError;
		return 0;
	}

	unsigned long long result = v_.GetUint64();
	ParseNext();
	return result;
}

int LookaheadParser::GetInt() {
	if (st_ != kHasNumber || !v_.IsInt()) {
		st_ = kError;
		return 0;
	}

	int result = v_.GetInt();
	ParseNext();
	return result;
}

double LookaheadParser::GetDouble() {
	if (st_ != kHasNumber) {
		st_ = kError;
		return 0.;
	}

	double result = v_.GetDouble();
	ParseNext();
	return result;
}

bool LookaheadParser::GetBool() {
	if (st_ != kHasBool) {
		st_ = kError;
		return false;
	}

	bool result = v_.GetBool();
	ParseNext();
	return result;
}

void LookaheadParser::GetNull() {
	if (st_ != kHasNull) {
		st_ = kError;
		return;
	}

	ParseNext();
}

const char* LookaheadParser::GetString() {
	if (st_ != kHasString) {
		st_ = kError;
		return 0;
	}

	const char* result = v_.GetString();
	ParseNext();
	return result;
}

void LookaheadParser::SkipOut(int depth) {
	do {
		if (st_ == kEnteringArray || st_ == kEnteringObject) {
			++depth;
		}
		else if (st_ == kExitingArray || st_ == kExitingObject) {
			--depth;
		}
		else if (st_ == kError) {
			return;
		}

		ParseNext();
	} while (depth > 0);
}

void LookaheadParser::SkipValue() {
	SkipOut(0);
}

void LookaheadParser::SkipArray() {
	SkipOut(1);
}

void LookaheadParser::SkipObject() {
	SkipOut(1);
}

Value* LookaheadParser::PeekValue() {
	if (st_ >= kHasNull && st_ <= kHasKey) {
		return &v_;
	}

	return 0;
}

int LookaheadParser::PeekType() {
	if (st_ >= kHasNull && st_ <= kHasKey) {
		return v_.GetType();
	}

	if (st_ == kEnteringArray) {
		return kArrayType;
	}

	if (st_ == kEnteringObject) {
		return kObjectType;
	}

	return -1;
}

u64 MetaStream_JSON::Close()
{
	if (this->mMode != MetaStreamMode::eMetaStream_Closed) {
		u64 completeStreamSize = mpReadWriteStream->GetSize();
		if (mMode == MetaStreamMode::eMetaStream_Write) {
			if (mpWriter->mBuffer.GetLength() == 0) {
				TelltaleToolLib_RaiseError("Not writing json meta stream because no payload data was found, try calling a serialize meta operation.", ErrorSeverity::WARN);
				return 0;
			}
			JSONWriter headerWriter{};
			headerWriter.mWriter.StartObject();

			headerWriter.mWriter.Key("Watermark");
			headerWriter.mWriter.String("Converted to JSON by Lucas Saragosa using Telltale Tool Lib Version " _VERSION);

			headerWriter.mWriter.Key("Game ID - DO NOT CHANGE");
			headerWriter.mWriter.String(TelltaleToolLib_GetBlowfishKey());

			headerWriter.mWriter.Key("_serializedTypes");
			headerWriter.mWriter.StartObject();

			headerWriter.mWriter.Key("mSize");
			headerWriter.mWriter.Uint((unsigned int)mSerializedTypes.size());

			headerWriter.mWriter.Key("DCArray");
			headerWriter.mWriter.StartArray();

			for (auto it = mSerializedTypes.begin(); it != mSerializedTypes.end(); it++) {
				headerWriter.mWriter.String((*it)->mpTypeInfoName);
			}

			headerWriter.mWriter.EndArray();
			headerWriter.mWriter.EndObject();

			headerWriter.mWriter.Key("_metaVersionInfo");
			headerWriter.mWriter.StartObject();

			int s = (int)mVersionInfo.size();

			for (int i = 0; i < mVersionInfo.size(); i++) {

				MetaClassDescription* pClazz = TelltaleToolLib_FindMetaClassDescription_ByHash(mVersionInfo[i].mTypeSymbolCrc);
				if (!pClazz) {
					s--;
				}
			}

			headerWriter.mWriter.Key("mSize");
			headerWriter.mWriter.Uint((unsigned int)s);

			headerWriter.mWriter.Key("DCArray");
			headerWriter.mWriter.StartArray();

			for (int i = 0; i < mVersionInfo.size(); i++) {
				MetaClassDescription* pClazz = TelltaleToolLib_FindMetaClassDescription_ByHash(mVersionInfo[i].mTypeSymbolCrc);
				if (!pClazz) {
					TelltaleToolLib_RaiseError("Could not find meta class description when writing json header", ErrorSeverity::WARN);
					//return 0;
					continue;
				}
				headerWriter.mWriter.StartObject();

				headerWriter.mWriter.Key("mTypeSymbolCrc");
				headerWriter.mWriter.Uint64(mVersionInfo[i].mTypeSymbolCrc);

				headerWriter.mWriter.Key("Type Name");
				headerWriter.mWriter.String(pClazz->mpTypeInfoName);

				headerWriter.mWriter.Key("mVersionCrc");
				headerWriter.mWriter.Uint(mVersionInfo[i].mVersionCrc);

				headerWriter.mWriter.EndObject();
			}

			headerWriter.mWriter.EndArray();
			headerWriter.mWriter.EndObject();
			headerWriter.mWriter.EndObject();

			mpWriter->mWriter.EndObject();

			mpReadWriteStream->Serialize(const_cast<char*>(headerWriter.mBuffer.GetString()), headerWriter.mBuffer.GetSize());//write header
			mpReadWriteStream->Serialize(const_cast<char*>("\n"), 1);//new line
			mpReadWriteStream->Serialize(const_cast<char*>(mpWriter->mBuffer.GetString()), mpWriter->mBuffer.GetSize());//write payload
			//DONE!
		}
		else if (mMode == MetaStreamMode::eMetaStream_Read) {
			//nothing 
		}
		mMode = MetaStreamMode::eMetaStream_Closed;
		_DeleteJSONData();
		return completeStreamSize;
	}
}

bool MetaStream_JSON::Attach(DataStream* stream, MetaStreamMode mode, MetaStreamParams params)
{
	if (mode == MetaStreamMode::eMetaStream_Closed || !stream)return false;
	_DeleteJSONData();
	this->mMode = mode;
	this->mpReadWriteStream = stream;
	if (mode != MetaStreamMode::eMetaStream_Read) {
		if (!mStreamVersion)
			this->mStreamVersion = 6;//MSV6
		mParams = params;
	}
	if (mode == MetaStreamMode::eMetaStream_Write) {
		mpWriter = new JSONWriter;
		mpWriter->mWriter.StartObject();
	}
	else if (mode == MetaStreamMode::eMetaStream_Read) {
		if (mpRead)
			delete[] mpRead;
		mpRead = new char[(unsigned int)mpReadWriteStream->GetSize()];
		if (!mpRead) {
			TelltaleToolLib_RaiseError("Could not allocate JSON in memory", ErrorSeverity::ERR);
			return false;
		}
		mpReadWriteStream->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
		mpReadWriteStream->Serialize(mpRead, mpReadWriteStream->GetSize());
		mpParser = new LookaheadParser(mpRead);
		mpParser->EnterObject();
		
		mpParser->NextObjectKey();
		mpParser->SkipValue();//watermark

		mpParser->NextObjectKey();
		if (0 != _stricmp(TelltaleToolLib_GetBlowfishKey(), mpParser->GetString())) {
			TelltaleToolLib_RaiseError("Bad meta stream JSON: game mismatch. Do not use this tool to convert files between game versions.", ErrorSeverity::ERR);
			return false;
		}

		//serialized types data
		if (0 != _stricmp("_serializedTypes", mpParser->NextObjectKey())) {
			TelltaleToolLib_RaiseError("Bad meta stream JSON: no serialized types data (perhaps using old version?)", ErrorSeverity::ERR);
			return false;
		}
		mpParser->EnterObject();
		mpParser->NextObjectKey();
		int numTypes = mpParser->GetInt();
		mSerializedTypes.clear();
		mSerializedTypes.reserve(numTypes);
		mpParser->NextObjectKey();
		mpParser->EnterArray();
		for (int i = 0; i < numTypes; i++) {
			if (!mpParser->NextArrayValue()) {
				TelltaleToolLib_RaiseError("Bad meta stream JSON: serialized types count mismatch", ErrorSeverity::ERR);
				return false;
			}
			MetaClassDescription* pClazz = TelltaleToolLib_FindMetaClassDescription(mpParser->GetString(), true);
			if (!pClazz) {
				TelltaleToolLib_RaiseError("Bad meta stream JSON: unknown type is serialized", ErrorSeverity::ERR);
				return false;
			}
			mSerializedTypes.push_back(pClazz);
		}
		mpParser->NextArrayValue();
		if (0 != _stricmp("_metaVersionInfo", mpParser->NextObjectKey())) {
			TelltaleToolLib_RaiseError("Bad meta stream JSON: no meta version information found", ErrorSeverity::ERR);
			return false;
		}
		//meta version info
		mpParser->EnterObject();
		mpParser->NextObjectKey();
		numTypes = mpParser->GetInt();
		mVersionInfo.clear();
		mVersionInfo.reserve(numTypes);
		mpParser->NextObjectKey();
		mpParser->EnterArray();
		const char* b;
		for (int i = 0; i < numTypes; i++) {
			if (!mpParser->NextArrayValue()) {
				TelltaleToolLib_RaiseError("Bad meta stream JSON: version information count mismatch", ErrorSeverity::ERR);
				return false;
			}
			mpParser->EnterObject();
			b = mpParser->NextObjectKey();
			unsigned long long typeSymbolCrc = mpParser->GetULong();
			b=mpParser->NextObjectKey();
			mpParser->SkipValue();//type name str
			b=mpParser->NextObjectKey();
			unsigned int verCrc = (unsigned int)mpParser->GetInt();
			if (!TelltaleToolLib_FindMetaClassDescription_ByHash(typeSymbolCrc)) {
				TelltaleToolLib_RaiseError("Bad meta stream JSON: unknown type in version information", ErrorSeverity::ERR);
				return false;
			}
			MetaVersionInfo inf{};
			inf.mVersionCrc = verCrc;
			inf.mTypeSymbolCrc = typeSymbolCrc;
			mVersionInfo.push_back(_STD move(inf));
			mpParser->NextObjectKey();
		}
		mpParser->NextArrayValue();
		mpParser->EnterObject();//payload data

	}
	mbErrored = false;
	return true;
}

u64 MetaStream_JSON::GetPartialStreamSize()
{
	return mpReadWriteStream->GetSize();
}

i64 MetaStream_JSON::WriteData(void* d, u32 z)
{
	if (NeedsKey())
		mpWriter->mWriter.Key("Binary Data Value");
	mpWriter->mWriter.String(websocketpp::base64_encode((unsigned char const*)d, (size_t)z).c_str());
	return z;
}

i64 MetaStream_JSON::ReadData(void* d, u32 z)
{
	
	return z;
}

void MetaStream_JSON::DisableDebugSection()
{

}

void MetaStream_JSON::EndAsyncSection()
{
	mpWriter->mWriter.EndObject();
	return;
}


void MetaStream_JSON::EndDebugSection()
{
	mpWriter->mWriter.EndObject();
	return;
}

bool MetaStream_JSON::HasDebugSection()
{
	return true;
}

// DO NOTHING

u64 MetaStream_JSON::GetPos()
{
	return 0;
}

void MetaStream_JSON::SetPos(u64)
{

}

void MetaStream_JSON::Advance(int numBytes)
{

}

void MetaStream_JSON::SkipToEndOfCurrentBlock()
{
	
}
// ----------------------

void MetaStream_JSON::BeginBlock()
{
	//nothing 
}

void MetaStream_JSON::EndBlock()
{
	//nothing
}

void MetaStream_JSON::BeginObject(const char* n, MetaClassDescription*, MetaMemberDescription*,bool arr)
{
	return BeginObject(n,arr);
}

void MetaStream_JSON::BeginObject(const char* n, bool bArray)
{
	mpWriter->mWriter.Key(n);
	if (bArray)
		mpWriter->mWriter.StartArray();
	else
		mpWriter->mWriter.StartObject();
	mbObjectIsArrayStack.push(bArray);
}

void MetaStream_JSON::EndObject(const char* n, MetaClassDescription*, MetaMemberDescription*)
{
	return EndObject(n);
}

void MetaStream_JSON::EndObject(const char* n)
{
	bool bArray = mbObjectIsArrayStack.top();
	mbObjectIsArrayStack.pop();
	if (bArray)
		mpWriter->mWriter.EndArray();
	else
		mpWriter->mWriter.EndObject();
}

void MetaStream_JSON::serialize_String(String* p)
{
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		if (NeedsKey())
			mpWriter->mWriter.Key("String Value");
		mpWriter->mWriter.String(p->c_str());
	}
}

void MetaStream_JSON::serialize_Symbol(Symbol* p)
{
	//SYMBOLS ARE WRITTEN AS STRINGS
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		String tmp{};
		if (NeedsKey())
			mpWriter->mWriter.Key("Symbol Value");
		if (!pCachedSymbolVal) {
			HashDatabase* db = TelltaleToolLib_GetGlobalHashDatabase();
			if (db) {
				db->FindEntry(nullptr, p->GetCRC(), &tmp);
				if (!tmp.empty())
					pCachedSymbolVal = tmp.c_str();
			}
		}
		const char* sym = pCachedSymbolVal ? pCachedSymbolVal : p->CRCAsCstr();
		pCachedSymbolVal = nullptr;
		mpWriter->mWriter.String(sym);
	}
}

void MetaStream_JSON::serialize_bool(bool* p)
{
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		if (NeedsKey())
			mpWriter->mWriter.Key("Boolean Value");
		mpWriter->mWriter.Bool(*p);
	}
}

void MetaStream_JSON::serialize_double(long double* p)
{
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		if (NeedsKey())
			mpWriter->mWriter.Key("Double Value");
		mpWriter->mWriter.Double((double) * p);
	}
}

void MetaStream_JSON::serialize_float(float* p)
{
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		if (NeedsKey())
			mpWriter->mWriter.Key("Float Value");
		mpWriter->mWriter.Double((double)*p);
	}
}

void MetaStream_JSON::serialize_uint16(u16* p)
{
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		if (NeedsKey())
			mpWriter->mWriter.Key("UInt16 Value");
		mpWriter->mWriter.Uint((unsigned long long) *p);
	}
}

void MetaStream_JSON::serialize_uint32(u32* p)
{
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		if (NeedsKey())
			mpWriter->mWriter.Key("UInt32 Value");
		mpWriter->mWriter.Uint64((unsigned long long)*p);
	}
}

void MetaStream_JSON::serialize_uint64(u64* p)
{
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		if (NeedsKey())
			mpWriter->mWriter.Key("UInt64 Value");
		mpWriter->mWriter.Uint64((uint64_t)*p);
	}
}

void MetaStream_JSON::serialize_int8(char* p)
{
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		if (NeedsKey())
			mpWriter->mWriter.Key("Int8 Value");
		mpWriter->mWriter.Uint((unsigned long long) *p);
	}
}

void MetaStream_JSON::InsertStringHint(const char* pK, const char* pS)
{
	//if (mMode == MetaStreamMode::eMetaStream_Write) {
	//	mpWriter->mWriter.Key(pK);
	//	mpWriter->mWriter.String(pS);
	//}
}

const char* MetaStream_JSON::SkipStringHint(const char* pK)
{
	return nullptr;
}

u64 MetaStream_JSON::GetSize()
{
	return mpReadWriteStream->GetSize();
}

bool MetaStream_JSON::HasAsyncSection()
{
	return true;
}

void MetaStream_JSON::Key(const char* pS)
{
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		if(mpWriter->mWriter.NeedsKey())
			mpWriter->mWriter.Key(pS);
	}
}

void MetaStream_JSON::HintSymbol(const char* symbolData)
{
	pCachedSymbolVal = symbolData;
}

bool MetaStream_JSON::NeedsKey() {
	return mpWriter ? mpWriter->mWriter.NeedsKey() : false;
}

bool MetaStream_JSON::BeginDebugSection()
{
	mpWriter->mWriter.Key("Debug Data");
	mpWriter->mWriter.StartObject();
	return true;
}

bool MetaStream_JSON::BeginAsyncSection()
{
	mpWriter->mWriter.Key("Async Data");
	mpWriter->mWriter.StartObject();
	return true;
}
