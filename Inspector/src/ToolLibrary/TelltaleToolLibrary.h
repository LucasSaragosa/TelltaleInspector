// This file was written by Lucas Saragosa. If you use this code or library,
// I ask you to you give credit to me and the amazing Telltale Games.


#ifndef _TTToolLib
#define _TTToolLib

/*SPECIAL VERSION FOR UI*/
#define _VERSION "8.0.0"

#include <type_traits>
#include <string>
#include "DataStream/DataStream.h"
#include "Blowfish.h"

//64 BIT! ONLY!

#ifdef _MSC_VER
#ifdef _DEBUG
#define DEBUGMODE
#else
#define RELEASEMODE
#endif
#else
#error "Please implement debug macro check for this compiler!"
#endif

#define INLINE __inline
#define FORCE_INLINE __forceinline

#define _TTToolLib_Exp extern "C" __declspec(dllexport)

#define TTEXPORT _TTToolLib_Exp

#define _TTTLib_PrintMetaClasses() TelltaleToolLib_DumpClassInfo(_PrintfDumper)

typedef std::make_unsigned<__int64>::type u64;
typedef std::make_unsigned<__int32>::type u32;
typedef std::make_unsigned<__int16>::type u16;
typedef std::make_unsigned<__int8> ::type u8;
typedef __int8  i8;
typedef __int16 i16;
typedef __int32 i32;
typedef __int64 i64;
typedef std::basic_string<i8,std::char_traits<i8>, std::allocator<i8>> String;
struct Flags;
template<typename T> class Ptr;
struct MetaClassDescription;
struct MetaMemberDescription;
enum MetaOpResult;
class MetaStream;

enum MetaMemberDescriptionParam {
	eMMDP_Name = 1,//stores 8 bytes (ptr)
	eMMDP_Offset = 2,//stores 8 bytes
	eMMDP_Flags = 3,//stores 4 bytes
	eMMDP_HostClass = 4,//stores 8 bytes (ptr)
	eMMDP_MemberClassDesc = 5,//stores 8 bytes (ptr)
	eMMDP_NextMember = 6,//stores 8 bytes (ptr)
	eMMDP_GameVersionRange = 7,//stores 12 bytes, first is skip version, second are min and max for game range this member is in (else -1 for all 3 ints)
};

enum MetaClassDescriptionParam {
	eMCDP_Extension = 1,//stores 8 bytes (ptr)
	eMCDP_Name = 2,//same as calling getmetaclassdescriptionname
	eMCDP_Hash = 3,//stores 8 bytes
	eMCDP_Flags = 4,//stores 4 bytes
	eMCDP_ClassSize = 5,//stores 4 bytes
	eMCDP_FirstMemberPtr = 6,//stores 8 bytes (ptr)
	eMCDP_OperationsList = 7,//stores 8 bytes (ptr)
	eMCDP_VTable = 8,//stores 8 bytes (ptr to array of ptrs)
	eMCDP_SerializeAccel = 9, //stores 8 bytes (ptr)
};

class MetaClassDescription;
class TelltaleVersionDatabase;

/*Used to manage the state of loading older classes from older games and engine versions.*/
/*You are strongly advised NOT to modify any of these variables, use the functions and if you really*/
/*need to then only read them!*/
/**
 * The real use of these is that if enabled and loaded for the currently set game ID, is that
 * FindMetaClassDescription will return a *proxy* meta class description instead to ensure
 * its the correct version of the currently set game ID.
 */
struct ProxyMetaState {

	const char* mpStateGameID = NULL;
	TelltaleVersionDatabase* mpStateGameDB = NULL;
	MetaClassDescription* mpProxyClassList = NULL;

};

/**
 * Returns true if the given game ID can support proxy classes, which is determined if the game ID
 * has its Version Database (.VersDB) loaded, which can be done from the TelltaleToolLib_SetProxyVersionDatabases function.
 * This may also return false if the given game is not supported for version databases yet. See VersDB.h MINIMUM_VERSDB
 * and also Blowfish.h for the minimum game and therefore games (ones including and after) that *are* supported.
 */
_TTToolLib_Exp bool TelltaleToolLib_ProxyClassesAvail(const char* pGameID);

/**
 * 
 * Generates (either calling new and appending to the proxy class list, or retrieving existing one) a new proxy meta class
 * description for the type symbol hash. If the hash is invalid for the current game (ie the type does not exit in the game
 * engine at that point) then this will return NULL. bGetOnly can be true if you don't want to create one but check if it 
 * is already loaded.
 * 
 */
_TTToolLib_Exp MetaClassDescription* TelltaleToolLib_GenerateMetaClassDescriptionProxy(ProxyMetaState* pState, u64 typeSymbolCrc, bool bGetOnly = false);

/**
 * If you want to use the proxy meta state for older games before reading in a file or getting a meta class description,
 * then enable it first. Returns the previous state. This is DEFAULT FALSE!
 */
_TTToolLib_Exp bool TelltaleToolLib_SetUsingProxyMetaStates(bool bOnOff);

/*
*
* Resets the given proxy state, clearing all of its proxy classes from memory. Also called when shutting down.
* This clears and DELETES all proxy class descriptions which were alive so be careful with your references to them, check their
* proxy flag if its set before calling this, then you know this will delete it.
* 
*/
_TTToolLib_Exp void TelltaleToolLib_ResetProxyMetaState(ProxyMetaState* pState);

/**
 * 
 * Gets the proxy meta state for the library. This is a static object and should not be deleted.
 * Pass in the game ID which it is attached to, or null to use the currently set one. This will return
 * NULL is the game ID is invalid or if TelltaleToolLib_ProxyClassesAvail is false for the given ID!
 * 
 */
_TTToolLib_Exp ProxyMetaState* TelltaleToolLib_GetProxyMetaState(const char* pGameID = nullptr);

typedef void (*DumpClassInfoF)(const char* const _Fmt, const char* _ParamA, const char* _ParamB);

void _PrintfDumper(const char* const _Fmt, const char* _ParamA, const char* _ParamB);

/*
* Gets the version of the TelltaleTool library as a string.
*/
_TTToolLib_Exp const char* TelltaleToolLib_GetVersion();

/*
* Dumps class information in a tabbed format
*/
_TTToolLib_Exp void TelltaleToolLib_DumpClassInfo(DumpClassInfoF _Dumper);

/*
* Makes the type name of the given string pointer to one which is ready to be hashed. 
* The parameter needs to be a pointer to a string which is allocated with calloc/malloc.
* This string is freed and the new string (allocated with calloc) is set to the parameter pointer's value.
*/
_TTToolLib_Exp void TelltaleToolLib_MakeInternalTypeName(char**);

/*
* If you have lots of .VersDB files which contain dumped game meta class descriptions in V2 format, then you can
* call this after Initialize to set the folder which contains the files. The library will try and load each file from
* this folder which is a game ID with a VersDB extension, eg WDC.VersDB or MCSM.VersDB. Returns the number of loaded
* or found version databases. Check the error callbacks if any could not be loaded. Returns 0 if the folder was invalid or 
* contained no files. If there was an error loading a database it returns -1. 
* IMPORTANT: The folder must be a full path from C: !
*/
_TTToolLib_Exp int TelltaleToolLib_SetProxyVersionDatabases(const char* pFolder);

/*
* Initialize the library. This must be called before you call any file reading and writing functions.
* Must pass a game id for the game that you are going to be working with files from. This is used for decryption keys.
* See TelltaleToolLib_SetBlowfishKey. The game id *can* be NULL, where it will be set to the default game id which can be 
* found in the Blowfish translation unit. If you pass an invalid game id, this will return false and fail. Passing NULL succeeds 
* with the default ID, this goes aswell for the SetBlowfishKey function.
*/
_TTToolLib_Exp bool TelltaleToolLib_Initialize(const char* game_id);

/*Returns true if the given game id is an old telltale game (.ttarch, not .ttarch2 etc). Used for different file saving types*/
_TTToolLib_Exp bool TelltaleToolLib_IsGameIDOld(const char* game_id);

/*
* Returns the first meta class description in the list of all meta class description types.
*/
_TTToolLib_Exp MetaClassDescription* TelltaleToolLib_GetFirstMetaClassDescription();

/*
* Inserts the next meta class description in the list into the parameter, or NULL is inserted if there are no more after it.
*/
_TTToolLib_Exp void TelltaleToolLib_GetNextMetaClassDescription(MetaClassDescription**);

/*
* Gets various information parameters from the given MCD and stores them in the function params
*/
_TTToolLib_Exp void TelltaleToolLib_GetMetaClassDescriptionInfo(MetaClassDescription*, void*,MetaClassDescriptionParam);

/*
* Gets various information parameters from the given MMD and stores them in the function params (middle one)
*/
_TTToolLib_Exp void TelltaleToolLib_GetMetaMemberDescriptionInfo(MetaMemberDescription*, void*, MetaMemberDescriptionParam);

/*
* Gets the next member in the meta class description from the parameter and stores it in the parameter. If there is none after the passed
* one then NULL is inserted.
*/
_TTToolLib_Exp void TelltaleToolLib_GetNextMetaMemberDescription(MetaMemberDescription**);

/*
* Sets the current game ID which is used for decryption of files when needed. Game IDs can be found in the github repo.
*/
_TTToolLib_Exp bool TelltaleToolLib_SetBlowfishKey(const char* game_id);

/*
* Gets the game id of the currently set blowfish key
*/
_TTToolLib_Exp const char* TelltaleToolLib_GetBlowfishKey();

_TTToolLib_Exp void* TelltaleToolLib_CreateClassInstance(MetaClassDescription* pClass);

_TTToolLib_Exp void TelltaleToolLib_DeleteClassInstance(MetaClassDescription* pClass, void* pInstance);

/*
* Searches for the given meta class description file extension or name. If the second parameter is false, then its by extension
* else its by the name. For example, for telltale texture files, use ("T3Texture",true) or ("d3dtx",false) to get the class for it.
*/
_TTToolLib_Exp MetaClassDescription* TelltaleToolLib_FindMetaClassDescription(const char*, bool bByNameOrExt);

/*
* Same as TelltaleToolLib_FindMetaClassDescription, but searches by hash.
*/
_TTToolLib_Exp MetaClassDescription* TelltaleToolLib_FindMetaClassDescription_ByHash(u64);

/*
* Gets the name of the given meta class description
*/
_TTToolLib_Exp const char* TelltaleToolLib_GetMetaClassDescriptionName(MetaClassDescription*);

/*
* When initialize is called (else this would return 0), this returns the amount of meta types that are initialized.
*/
_TTToolLib_Exp i32 TelltaleToolLib_GetMetaTypesCount();

/*
* Returns if this library is initialized yet.
*/
_TTToolLib_Exp bool TelltaleToolLib_Initialized();

/*
* Creates a datastream for the given file path.
*/
_TTToolLib_Exp DataStream* TelltaleToolLib_CreateDataStream(const char* fp, DataStreamMode);

/*
* Deletes the given data stream. Only use this if you know what you're doing, normally its deleted internally.
*/
_TTToolLib_Exp void TelltaleToolLib_DeleteDataStream(DataStream*);

/*
* A static 1024 byte char buffer for strings. This can be used by other programs to store strings passed in between the library.
*/
_TTToolLib_Exp char* TelltaleToolLib_Alloc_GetFixed1024ByteStringBuffer();

/*
* A static 8 byte buffer to store pointers. Useful when using for loops which need to get the next class/member and can store the 
* pointer to it in this buffer to save memory allocations.
*/
_TTToolLib_Exp char* TelltaleToolLib_Alloc_GetFixed8BytePointerBuffer();

//These next two family of functions return a buffer allocated with MALLOC! So use FREE. Returns NULL if a problem occurred.
//These only work for LUA files.

_TTToolLib_Exp u8* TelltaleToolLib_EncryptScript(u8* data, u32 size, u32* outsize);
_TTToolLib_Exp u8* TelltaleToolLib_DecryptScript(u8* data, u32 size, u32* outsize);

//These next two family of functions return a buffer allocated with MALLOC! So use FREE. Returns NULL if a problem occurred.
//These only work for LENC files.

_TTToolLib_Exp u8* TelltaleToolLib_EncryptLencScript(u8* data, u32 size, u32 *outsize);
_TTToolLib_Exp u8* TelltaleToolLib_DecryptLencScript(u8* data, u32 size, u32* outsize);

class HashDatabase;
class DataStream;

/*
* Sets the global hash database used to search CRCs. It is very important that its set before you read or write most types (eg PropertySet)
* If one is already set, then this deletes the old use using operator delete. Can be set to NULL.
*/
_TTToolLib_Exp void TelltaleToolLib_SetGlobalHashDatabase(HashDatabase*);

/*
* Does the same as the normal set global hash db, but creates it for you. Pass in the reading file stream to it. This stream 
* is deleted by the library so DONT DELETE IT!
*/
_TTToolLib_Exp void TelltaleToolLib_SetGlobalHashDatabaseFromStream(DataStream*);

/*
* Gets the game index of the given game id. This is used to match when a game is released with others. Used internally make sure the 
* current file is being read correctly with the correct version range
*/
_TTToolLib_Exp i32 TelltaleToolLib_GetGameKeyIndex(const char* pGameID);

/*
* Gets the global hash database, or NULL if its not been set.
*/
_TTToolLib_Exp HashDatabase* TelltaleToolLib_GetGlobalHashDatabase();

/*
* Frees all non-static memory related to this library. 
*/
_TTToolLib_Exp void TelltaleToolLib_Free();

TTEXPORT void* TelltaleToolLib_Malloc(unsigned long size);
TTEXPORT void TelltaleToolLib_Dealloc(void*);

enum ErrorSeverity {
	NOTIFY,
	WARN,
	ERR,
	CRITICAL_ERROR
};

typedef void (*ErrorCallbackF)(const char* _Msg, ErrorSeverity _Severity);

/*
* Sets the error callback which will be called when any error occurs.
*/
_TTToolLib_Exp void TelltaleToolLib_SetErrorCallback(ErrorCallbackF _Func);

_TTToolLib_Exp void TelltaleToolLib_RaiseError(const char* _Msg, ErrorSeverity _S);

_TTToolLib_Exp const char* TelltaleToolLib_GetLastError();

//_TTToolLib_Exp MetaOpResult
//TelltaleToolLib_PerformMetaSerialize(MetaClassDescription* pObjectDescription, void* pObject, MetaStream* pUserData);

extern bool sInitialized;

struct Flags {
	u32 mFlags;

	Flags(u32 i) : mFlags(i) {}

	Flags() {}

	INLINE Flags& operator=(u32 i) {
		mFlags = i;
		return *this;
	}
	INLINE Flags& operator|=(u32 i) {
		mFlags |= i;
		return *this;
	}
	INLINE Flags& operator&=(u32 i) {
		mFlags &= i;
		return *this;
	}
};

template<int Bits, typename T>
struct TTBitArray {
	enum NoInit {
		eNoInit = 0
	};

	T mBits[Bits / 8];

};

namespace Geo {

	struct GeoGuid {
		u64 _Id[2];
	};

}

template<typename T> void PtrModifyRefCount(T*, int delta) {}//deprecated in engine/not needed for shipping

//telltale impl of pointers, mainly its calls to the ref count . this doesnt work lol
template<typename T>
class Ptr {
public:
	T* mpData;
public:

	Ptr() = default;

	Ptr(const T* pObj) : mpData(pObj) {}

	Ptr(const Ptr<T>& other) : mpData(other.mpData) {}

	~Ptr() {}

	Ptr<T>& operator=(const Ptr<T>& pObj) {
		mpData = pObj.mpData;
		return *this;
	}

	Ptr<T>& operator=(T* pOther) {
		mpData = pOther;
		return *this;
	}

	void Assign(const T* pObj) {
		mpData = pObj;
	}

	void DeleteObject() {
		if (mpData) {
			delete mpData;
		}
	}

};

#endif
