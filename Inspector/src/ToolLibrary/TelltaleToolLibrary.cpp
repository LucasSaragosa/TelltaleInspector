// This file was written by Lucas Saragosa. If you use this code or library,
// I ask you to you give credit to me and the amazing Telltale Games.
#pragma warning(disable C4267 C4244 C4554 C4477)
#include "TelltaleToolLibrary.h"
#include "Meta.hpp"
#include "MetaInitialize.h"
#include "HashDB/HashDB.h"
#include "VersDB.h"
#include "DataStream/DataStream.h"

#include <vector>
#include <filesystem>

static TelltaleVersionDatabase* sVersionDBs[KEY_COUNT]{ 0 };
static bool sbUsingStates = false;
static ProxyMetaState sProxyState[KEY_COUNT]{ 0 };

_TTToolLib_Exp bool TelltaleToolLib_ProxyClassesAvail(const char* pGameID)
{
    int nMinGame = TelltaleToolLib_GetGameKeyIndex(MINIMUM_VERSDB_GAME);
    assert(nMinGame != -1);
    int nGameIndex = TelltaleToolLib_GetGameKeyIndex(pGameID);
    if (nGameIndex == -1)
        return false;
    if (nMinGame > nGameIndex)
        return false;
    return sVersionDBs[nGameIndex] != nullptr;
}

_TTToolLib_Exp MetaClassDescription* TelltaleToolLib_GenerateMetaClassDescriptionProxy(ProxyMetaState* pState, u64 typeSymbolCrc, bool bGetOnly /*= false*/)
{
    if (!pState || !typeSymbolCrc || !pState->mpStateGameDB)
        return NULL;
    MetaClassDescription* pProxyClass = pState->mpProxyClassList;
    MetaClassDescription* pTail = nullptr;
    while (pProxyClass) {
        if (pProxyClass->mHash == typeSymbolCrc)
            return pProxyClass;
        pTail = pProxyClass;
        pProxyClass = pProxyClass->pNextMetaClassDescription;
    }
    if (bGetOnly)
        return NULL;

    TelltaleVersionDatabase::ClassEntry* pSerializedClass = pState->mpStateGameDB->FindClass(typeSymbolCrc);
    if (!pSerializedClass)
        return NULL;

    pProxyClass = new MetaClassDescription;
    pProxyClass->mpExt = (const char*)pSerializedClass->mpExt;
    pProxyClass->mClassAlign = 8;
    pProxyClass->mClassSize = pSerializedClass->mClassSize;
    pProxyClass->mFlags = pSerializedClass->mFlags;
    pProxyClass->mpTypeInfoExternalName = (const char*)pSerializedClass->mpTypeInfoName;
    pProxyClass->mHash = pSerializedClass->mHash;
    bool p = sbUsingStates;
    sbUsingStates = false;
    MetaClassDescription* ac = TelltaleToolLib_FindMetaClassDescription_ByHash(typeSymbolCrc);
    if (ac)
        memcpy(pProxyClass->mpVTable, ac->mpVTable, sizeof(ac->mpVTable));
    else
        memset(pProxyClass->mpVTable, 0, sizeof(pProxyClass->mpVTable));
    sbUsingStates = p;
    pProxyClass->mMetaOperationsList = NULL;
    MetaMemberDescription* pPrev = NULL;
    TelltaleVersionDatabase::MemberEntry* pMember{};
    do {
        if (pMember)
            pMember = pState->mpStateGameDB->mMembers.mpStorage + pMember->mpNextMember;
        else
            pMember = pState->mpStateGameDB->mMembers.mpStorage + pSerializedClass->mpFirstMember;
        if (pMember) {
            MetaMemberDescription* pNewMember = new MetaMemberDescription;
            pNewMember->mFlags = pMember->mFlags;
            pNewMember->mOffset = pMember->mOffset;
            //TODO
        }
    } while (pMember != nullptr);

    if (pTail == nullptr)
        pState->mpProxyClassList = pProxyClass;
    else
        pTail->pNextMetaClassDescription = pProxyClass;
    return pProxyClass;
}

_TTToolLib_Exp void TelltaleToolLib_ResetProxyMetaState(ProxyMetaState* pState)
{
    if (pState) {
        MetaClassDescription* pProxy = pState->mpProxyClassList;
        //TODO
    }
}

_TTToolLib_Exp bool TelltaleToolLib_SetUsingProxyMetaStates(bool bOnOff)
{
    bool bState = sbUsingStates;
    sbUsingStates = bOnOff;
    return bState;
}

_TTToolLib_Exp ProxyMetaState* TelltaleToolLib_GetProxyMetaState(const char* pGameID)
{
    int index = TelltaleToolLib_GetGameKeyIndex(pGameID);
    if (index == -1)
        return NULL;
    if (!TelltaleToolLib_ProxyClassesAvail(pGameID))
        return NULL;
    return &sProxyState[index];
}

_TTToolLib_Exp int TelltaleToolLib_SetProxyVersionDatabases(const char* pFolder)
{
    if (!pFolder || !(*pFolder))
        return 0;
    int nNum = 0;
    std::filesystem::path folderPath{ pFolder };
    std::string f = folderPath.string();
    if (!(f.at(f.length() - 1) == '/' || f.at(f.length() - 1) == '\\'))
        folderPath += "/";
    for (auto& it : std::filesystem::directory_iterator{ folderPath }) {
        if (it.is_regular_file() && it.path().extension().string() == ".VersDB") {
			size_t lastindex = it.path().filename().string().find_last_of(".");
			std::string rawname = it.path().filename().string().substr(0, lastindex);
            int index = TelltaleToolLib_GetGameKeyIndex(rawname.c_str());
            if (index == -1) {
                TelltaleToolLib_RaiseError("Found VersDB with a name that doesnt belong to a valid game ID! Ignoring.", ErrorSeverity::WARN);
                continue;
            }
            if (sVersionDBs[index] != nullptr)
                delete sVersionDBs[index];
            sVersionDBs[index] = new TelltaleVersionDatabase;
            DataStreamFile_Win in = _OpenDataStreamFromDisc_(it.path().string().c_str(), READ);
            if (in.IsInvalid() || !sVersionDBs[index]->SerializeIn(&in)) {
				TelltaleToolLib_RaiseError("Could not open or load a VersDB file", ErrorSeverity::ERR);
                return -1;
            }
            sProxyState[index].mpStateGameDB = sVersionDBs[index];
            nNum++;
        }
    }
    return nNum;
}

i32 TelltaleToolLib_GetGameKeyIndex(const char* pGameID) {
    if (pGameID) {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (!_stricmp(sBlowfishKeys[i].game_id, pGameID)) {
                return i;
            }
        }
    }
    return -1;
}

/*
MetaOpResult TelltaleToolLib_PerformMetaSerialize(MetaClassDescription* pObjectDescription, void* pObject, MetaStream* pUserData) {
    if (!pObjectDescription || !pObject || !pUserData)
        return eMetaOp_Fail;
    return PerformMetaSerializeFull(pUserData, pObject, pObjectDescription);
}
*/

HashDatabase* sgHashDB = NULL;
bool sInitialized = false;
const char* s_lastError = nullptr;

void _DefaultCallback(const char* msg, ErrorSeverity e) {
#ifdef DEBUGMODE
    printf("ERROR: %s: [%s]\n",msg, e == ErrorSeverity::CRITICAL_ERROR ? "CRITICAL" : e == ErrorSeverity::NOTIFY ? "NOTIFY" 
    : e == ErrorSeverity::WARN ? "WARNING" : "ERR");
#endif
    if (e == ErrorSeverity::CRITICAL_ERROR)exit(-1);
}

ErrorCallbackF sDefaultErrorCallback = _DefaultCallback;

void TelltaleToolLib_SetErrorCallback(ErrorCallbackF _Func) {
    sDefaultErrorCallback = _Func;
}

void TelltaleToolLib_RaiseError(const char* _Msg, ErrorSeverity _S) {
    sDefaultErrorCallback(_Msg, _S);
    s_lastError = _Msg;
}

_TTToolLib_Exp const char* TelltaleToolLib_GetLastError() {
    return s_lastError;
}

TTEXPORT void* TelltaleToolLib_Malloc(unsigned long size) {
    return malloc(size);
}

TTEXPORT void TelltaleToolLib_Dealloc(void* ptr) {
    if(ptr)
        return free(ptr);
}

_TTToolLib_Exp void TelltaleToolLib_DeleteClassInstance(MetaClassDescription* pClass, void* inst) {
    pClass->Delete(inst);
}

_TTToolLib_Exp void* TelltaleToolLib_CreateClassInstance(MetaClassDescription* pClass) {
    if (pClass) {
        return pClass->New();
    }
    return nullptr;
}

void TelltaleToolLib_Free() {
    if (sgHashDB) {
        delete sgHashDB;
        sgHashDB = NULL;
    }
    sDefaultErrorCallback = _DefaultCallback;
    for (int i = 0; i < KEY_COUNT; i++) {
        TelltaleToolLib_ResetProxyMetaState(&sProxyState[i]);
        if (sVersionDBs[i])
            delete sVersionDBs[i];
    }
    memset(sVersionDBs, 0, sizeof(TelltaleVersionDatabase*) * KEY_COUNT);
    memset(sProxyState, 0, sizeof(ProxyMetaState) * KEY_COUNT);
    __ReleaseSVI_Internal();
}

void TelltaleToolLib_SetGlobalHashDatabaseFromStream(DataStream* stream) {
    if(stream)
        TelltaleToolLib_SetGlobalHashDatabase(new HashDatabase(stream));
}

void TelltaleToolLib_SetGlobalHashDatabase(HashDatabase* db) {
    if (sgHashDB)
        delete sgHashDB;
    sgHashDB = db;
    if(db)
        db->Open();
}

HashDatabase* TelltaleToolLib_GetGlobalHashDatabase() {
    return sgHashDB;
}

u8* TelltaleToolLib_EncryptLencScript(u8* data, u32 size, u32* outsize) {
    u8* ret = (u8*)malloc(size);
    memcpy(ret, data, size);
    *outsize = size;
    TelltaleToolLib_BlowfishEncrypt(ret, size, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
    return ret;
}

u8* TelltaleToolLib_DecryptLencScript(u8* data, u32 size, u32* outsize) {
    u8* ret = (u8*)malloc(size);
    memcpy(ret, data, size);
    *outsize = size;
    TelltaleToolLib_BlowfishDecrypt(ret, size, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
    return ret;
}

u8* TelltaleToolLib_EncryptScript(u8* data, u32 size, u32 *outsize) {
    if (!data || 8 > size)return NULL;
    if (*(int*)data == *(const int*)"\x1BLua") {
        u8* ret = (u8*)malloc(size);
        memcpy(ret, data, size);
        *outsize = size;
        *(int*)ret = 1850035227;//LEn
        TelltaleToolLib_BlowfishEncrypt(ret + 4, size - 4, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
        return ret;
    }
    else if (*(int*)data != *(const int*)"\x1BLEo" && *(int*)data != *(const int*)"\x1BLEn") {
        u8* ret = (u8*)malloc(size + 4);
        memcpy(ret + 4, data, size);
        *outsize = size + 4;
        *(int*)ret = 1866812443;//LEo
        TelltaleToolLib_BlowfishEncrypt(ret + 4, size, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
        return ret;
    }
    u8* ret = (u8*)malloc(size);
    memcpy(ret, data, size);
    *outsize = size;
    return ret;
}

u8* TelltaleToolLib_DecryptScript(u8* data, u32 size, u32* outsize) {
    if (8 >= size || !data)return NULL;
    if (*(char*)data != '\x1B') {
        //plain old text version workaround
        u8* ret = (u8*)malloc(size);
        memcpy(ret, data, size);
        *outsize = size;
        return ret;
    }
    if (*(int*)data == *(const int*)"\x1BLEo") {
        u8* ret = (u8*)malloc(size - 4);
        memcpy(ret, data + 4, size - 4);
        *outsize = size - 4;
        TelltaleToolLib_BlowfishDecrypt(ret, size - 4, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
        return ret;
    }
    u8* ret = (u8*)malloc(size);
    memcpy(ret, data, size);
    if (*(int*)data == *(const int*)"\x1BLEn")
        *(int*)ret = 1635077147;
    TelltaleToolLib_BlowfishDecrypt(ret + 4, size - 4, sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
    *outsize = size;
    return ret;
}

const char* TelltaleToolLib_GetMetaClassDescriptionName(MetaClassDescription* pObjDesc) {
    return pObjDesc->mpTypeInfoName;
}

DataStream* TelltaleToolLib_CreateDataStream(const char* fp, DataStreamMode mode) {
    return new DataStreamFileDisc((FileHandle)PlatformSpecOpenFile(fp, mode), mode);
}

void TelltaleToolLib_DeleteDataStream(DataStream* stream) {
    delete stream;//calls destructor
}

char* TelltaleToolLib_Alloc_GetFixed1024ByteStringBuffer() {
    static char buf[1024];
    return &buf[0];
}

char* TelltaleToolLib_Alloc_GetFixed8BytePointerBuffer() {
    static char buf[8];
    return &buf[0];
}

void TelltaleToolLib_GetNextMetaClassDescription(MetaClassDescription** pObjDescPtr) {
    if (pObjDescPtr && *pObjDescPtr) *pObjDescPtr = (*pObjDescPtr)->pNextMetaClassDescription;
}

void TelltaleToolLib_GetNextMetaMemberDescription(MetaMemberDescription** pMemberDescPtr) {
    if (pMemberDescPtr && *pMemberDescPtr)*pMemberDescPtr = (*pMemberDescPtr)->mpNextMember;
}

MetaClassDescription* TelltaleToolLib_FindMetaClassDescription_ByHash(u64 pHash) {
    for (MetaClassDescription* i = TelltaleToolLib_GetFirstMetaClassDescription(); i;) {
        if (i->mHash == pHash)
            return i;
        TelltaleToolLib_GetNextMetaClassDescription(&i);
    }
    return NULL;
}

MetaClassDescription* TelltaleToolLib_FindMetaClassDescription(const char* pStr, bool pIsName) {
    if (pIsName) {
        u64 crc = CRC64_CaseInsensitive(0, pStr);
        for (MetaClassDescription* i = TelltaleToolLib_GetFirstMetaClassDescription(); i;) {          
            if (i->mHash == crc)
                return i;
            TelltaleToolLib_GetNextMetaClassDescription(&i);
        }
    }
    else {
        for (MetaClassDescription* i = TelltaleToolLib_GetFirstMetaClassDescription(); i;) {
            if (!i->mpExt) {//stfu:(cba
                TelltaleToolLib_GetNextMetaClassDescription(&i);
                continue;
            }
            if (!_stricmp(pStr,i->mpExt))
                return i;
            TelltaleToolLib_GetNextMetaClassDescription(&i);
        }
    }
    return NULL;
}

void TelltaleToolLib_GetMetaMemberDescriptionInfo(MetaMemberDescription* pMemberDesc, void* pDest, MetaMemberDescriptionParam param) {
    switch (param) {
    case MetaMemberDescriptionParam::eMMDP_Name:
        *static_cast<const void**>(pDest) = pMemberDesc->mpName;
        break;
    case MetaMemberDescriptionParam::eMMDP_Offset:
        *static_cast<u64*>(pDest) = pMemberDesc->mOffset;
        break;
    case MetaMemberDescriptionParam::eMMDP_Flags:
        *static_cast<u32*>(pDest) = pMemberDesc->mFlags;
        break;
    case MetaMemberDescriptionParam::eMMDP_HostClass:
        *static_cast<MetaClassDescription**>(pDest) = pMemberDesc->mpHostClass;
        break;
    case MetaMemberDescriptionParam::eMMDP_MemberClassDesc:
        *static_cast<MetaClassDescription**>(pDest) = pMemberDesc->mpMemberDesc;
        break;
    case MetaMemberDescriptionParam::eMMDP_NextMember:
        *static_cast<MetaMemberDescription**>(pDest) = pMemberDesc->mpNextMember;
        break;
    case MetaMemberDescriptionParam::eMMDP_GameVersionRange:
        *static_cast<u32*>((void*)((char*)pDest + 0)) = pMemberDesc->mSkipVersion;
        *static_cast<u32*>((void*)((char*)pDest + 4)) = pMemberDesc->mGameIndexVersionRange.min;
        *static_cast<u32*>((void*)((char*)pDest + 8)) = pMemberDesc->mGameIndexVersionRange.max;
        break;
    default:
        return;
    }
}

void TelltaleToolLib_GetMetaClassDescriptionInfo(MetaClassDescription* pObj, void* pDest, MetaClassDescriptionParam param) {
    switch (param) {
    case MetaClassDescriptionParam::eMCDP_Extension:
        *static_cast<const char**>(pDest) = pObj->mpExt;
        break;
    case MetaClassDescriptionParam::eMCDP_Name:
        *static_cast<const char**>(pDest) = pObj->mpTypeInfoName;
        break;
    case MetaClassDescriptionParam::eMCDP_Hash:
        *static_cast<u64*>(pDest) = pObj->mHash;
        break;
    case MetaClassDescriptionParam::eMCDP_Flags:
        *static_cast<u32*>(pDest) = pObj->mFlags.mFlags;
        break;
    case MetaClassDescriptionParam::eMCDP_ClassSize:
        *static_cast<u32*>(pDest) = pObj->mClassSize;
        break;
    case MetaClassDescriptionParam::eMCDP_FirstMemberPtr:
        *static_cast<MetaMemberDescription**>(pDest) = pObj->mpFirstMember;
        break;
    case MetaClassDescriptionParam::eMCDP_OperationsList:
        *static_cast<MetaOperationDescription**>(pDest) = pObj->mMetaOperationsList;
        break;
    case MetaClassDescriptionParam::eMCDP_VTable:
        *static_cast<void***>(pDest) = &pObj->mpVTable[0];
        break;
    case MetaClassDescriptionParam::eMCDP_SerializeAccel:
        *static_cast<MetaSerializeAccel**>(pDest) = pObj->mpSerializeAccel;
        break;
    default:
        return;
    }
}

MetaClassDescription* TelltaleToolLib_GetFirstMetaClassDescription() {
    return spFirstMetaClassDescription;
}

const char* TelltaleToolLib_GetVersion() {
	return _VERSION;
}

i32 TelltaleToolLib_GetMetaTypesCount() {
    return sMetaTypesCount;
}

bool TelltaleToolLib_Initialized() {
    return sInitialized;
}

bool TelltaleToolLib_SetBlowfishKey(const char* game_id) {
    if (game_id) {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (!_stricmp(sBlowfishKeys[i].game_id, game_id)) {
                sSetKeyIndex = i;
                return true;
                break;
            }
        }
    }
    else sSetKeyIndex = DEFAULT_BLOWFISH_GAME_KEY;
    return false;
    //if (spBlowfish) {
    //    spBlowfish->Init(sBlowfishKeys[sSetKeyIndex].game_key);
    //}
}

const char* TelltaleToolLib_GetBlowfishKey() {
    return sBlowfishKeys[sSetKeyIndex].game_id;
}

bool TelltaleToolLib_Initialize(const char* game_id) {
    if (sInitialized)return false;
    if (game_id) {
        const BlowfishKey* k = NULL;
        for (int i = 0; i < KEY_COUNT; i++) {
            if (!_stricmp(sBlowfishKeys[i].game_id, game_id)) {
                k = &sBlowfishKeys[i];
                sSetKeyIndex = i;
                break;
            }
        }
        if (k == NULL)return false;
       // Blowfish::Initialize(k);
    }
  //  else Blowfish::Initialize(&sBlowfishKeys[sSetKeyIndex]);
    MetaInit::Initialize();//init all types
    memset(sVersionDBs, 0, sizeof(TelltaleVersionDatabase*) * KEY_COUNT);
    memset(sProxyState, 0, sizeof(ProxyMetaState) * KEY_COUNT);
    for (int i = 0; i < KEY_COUNT; i++) {
        sProxyState[i].mpStateGameID = sBlowfishKeys[i].game_id;
    }
    sbUsingStates = false;
    sInitialized = true;
    return true;
}


_TTToolLib_Exp bool TelltaleToolLib_IsGameIDOld(const char* game_id)
{
    int id = TelltaleToolLib_GetGameKeyIndex(game_id);
    return id <= 51;
}

//I KNOW ITS BAD FOR ALLOCATIONS BUT IM SORRY I CANT BE ASKED TO CHANGE IT D:
void TelltaleToolLib_MakeInternalTypeName(char** _StringPtr) {
    static std::string _sRepl_A = "struct ";
    static std::string _sRepl_B = "class ";
    static std::string _sRepl_C = "enum ";
    static std::string _sRepl_D = "std::";
    static std::string _sReplLast = "basic_string<char,char_traits<char>,allocator<char>>";
    static std::string _sRepl_E = " ";
    static std::string _sNothing = "";
	std::string ReplStr(*_StringPtr);
    size_t start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_A, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_A.length(), _sNothing);
    }
    start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_B, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_B.length(), _sNothing);
    }
    start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_C, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_C.length(), _sNothing);
    }
    start_pos = 0;
    while ((start_pos = ReplStr.find(_sRepl_D, start_pos)) != std::string::npos) {
        ReplStr.replace(start_pos, _sRepl_D.length(), _sNothing);
    }
	start_pos = 0;
	while ((start_pos = ReplStr.find(_sRepl_E, start_pos)) != std::string::npos) {
		ReplStr.replace(start_pos, _sRepl_E.length(), _sNothing);
	}
	start_pos = 0;
	while ((start_pos = ReplStr.find(_sReplLast, start_pos)) != std::string::npos) {
		ReplStr.replace(start_pos, _sReplLast.length(), "String");
	}
    u32 slen = ReplStr.length();
    char* nbuf = (char*)malloc(slen+1);
    memcpy(nbuf, ReplStr.c_str(), slen);
    //for (int i = 0; i < slen; i++) {
    //    nbuf[i] |= 0b100000;
    //}
    nbuf[slen] = 0i8;
    free(*_StringPtr);
    *_StringPtr = nbuf;
}

void _PrintfDumper(const char* const _Fmt, const char* _ParamA, const char* _ParamB) {
    if (_ParamA && _ParamB) {
        printf(_Fmt, _ParamA, _ParamB);
    }
    else if (_ParamA) {
        printf(_Fmt, _ParamA);
    }
    else {
        printf(_Fmt);
    }
}


void printMembers(int tabs, MetaMemberDescription* mem, DumpClassInfoF _Dumper) {
    while (mem) {
        for (int i = 0; i < tabs; i++)
            _Dumper("\t", NULL, NULL);
        _Dumper("[%s] %s\n", mem->mpMemberDesc->mpTypeInfoName, mem->mpName);
        if (mem->mpMemberDesc->mpFirstMember)
            printMembers(tabs + 1, mem->mpMemberDesc->mpFirstMember,_Dumper);
        mem = mem->mpNextMember;
    }
}

void TelltaleToolLib_DumpClassInfo(DumpClassInfoF _Dumper) {
    MetaClassDescription* clazz = TelltaleToolLib_GetFirstMetaClassDescription();
    for (int i = 0; i < TelltaleToolLib_GetMetaTypesCount(); i++) {
        _Dumper("Class: %s\n", clazz->mpTypeInfoName,NULL);
        if (clazz->mpFirstMember) {
            printMembers(1, clazz->mpFirstMember,_Dumper);
        }
        TelltaleToolLib_GetNextMetaClassDescription(&clazz);
        _Dumper("\n\n",NULL,NULL);
    }
}