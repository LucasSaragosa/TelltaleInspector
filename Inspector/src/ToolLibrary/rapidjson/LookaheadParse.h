#include "reader.h"
#include "document.h"
#include <iostream>

RAPIDJSON_DIAG_PUSH
#ifdef __GNUC__
RAPIDJSON_DIAG_OFF(effc++)
#endif

// This example demonstrates JSON token-by-token parsing with an API that is
// more direct; you don't need to design your logic around a handler object and
// callbacks. Instead, you retrieve values from the JSON stream by calling
// GetInt(), GetDouble(), GetString() and GetBool(), traverse into structures
// by calling EnterObject() and EnterArray(), and skip over unwanted data by
// calling SkipValue(). When you know your JSON's structure, this can be quite
// convenient.
//
// If you aren't sure of what's next in the JSON data, you can use PeekType() and
// PeekValue() to look ahead to the next object before reading it.
//
// If you call the wrong retrieval method--e.g. GetInt when the next JSON token is
// not an int, EnterObject or EnterArray when there isn't actually an object or array
// to read--the stream parsing will end immediately and no more data will be delivered.
//
// After calling EnterObject, you retrieve keys via NextObjectKey() and values via
// the normal getters. When NextObjectKey() returns null, you have exited the
// object, or you can call SkipObject() to skip to the end of the object
// immediately. If you fetch the entire object (i.e. NextObjectKey() returned  null),
// you should not call SkipObject().
//
// After calling EnterArray(), you must alternate between calling NextArrayValue()
// to see if the array has more data, and then retrieving values via the normal
// getters. You can call SkipArray() to skip to the end of the array immediately.
// If you fetch the entire array (i.e. NextArrayValue() returned null),
// you should not call SkipArray().
//
// This parser uses in-situ strings, so the JSON buffer will be altered during the
// parse.

using namespace rapidjson;


class LookaheadParserHandler {
public:
	bool Null() { st_ = kHasNull; v_.SetNull(); return true; }
	bool Bool(bool b) { st_ = kHasBool; v_.SetBool(b); return true; }
	bool Int(int i) { st_ = kHasNumber; v_.SetInt(i); return true; }
	bool Uint(unsigned u) { st_ = kHasNumber; v_.SetUint(u); return true; }
	bool Int64(int64_t i) { st_ = kHasNumber; v_.SetInt64(i); return true; }
	bool Uint64(uint64_t u) { st_ = kHasNumber; v_.SetUint64(u); return true; }
	bool Double(double d) { st_ = kHasNumber; v_.SetDouble(d); return true; }
	bool RawNumber(const char*, SizeType, bool) { return false; }
	bool String(const char* str, SizeType length, bool) { st_ = kHasString; v_.SetString(str, length); return true; }
	bool StartObject() { st_ = kEnteringObject; return true; }
	bool Key(const char* str, SizeType length, bool) { st_ = kHasKey; v_.SetString(str, length); return true; }
	bool EndObject(SizeType) { st_ = kExitingObject; return true; }
	bool StartArray() { st_ = kEnteringArray; return true; }
	bool EndArray(SizeType) { st_ = kExitingArray; return true; }

	LookaheadParserHandler(char* str);
	void ParseNext();

	enum LookaheadParsingState {
		kInit,
		kError,
		kHasNull,
		kHasBool,
		kHasNumber,
		kHasString,
		kHasKey,
		kEnteringObject,
		kExitingObject,
		kEnteringArray,
		kExitingArray
	};

	Value v_;
	LookaheadParsingState st_;
	Reader r_;
	InsituStringStream ss_;

	static const int parseFlags = kParseDefaultFlags | kParseInsituFlag;
};

class LookaheadParser : public LookaheadParserHandler {
public:
	LookaheadParser(char* str) : LookaheadParserHandler(str) {}

	bool EnterObject();
	bool EnterArray();
	const char* NextObjectKey();
	bool NextArrayValue();
	int GetInt();
	unsigned long long GetULong();
	double GetDouble();
	const char* GetString();
	bool GetBool();
	void GetNull();

	void SkipObject();
	void SkipArray();
	void SkipValue();
	Value* PeekValue();
	int PeekType(); // returns a rapidjson::Type, or -1 for no value (at end of object/array)

	bool IsValid() { return st_ != kError; }

protected:
	void SkipOut(int depth);
};

RAPIDJSON_DIAG_POP