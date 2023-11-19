// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#pragma warning (disable : 4018 4244 4267 4554 6387 4099)
#include "Meta.hpp"
#include <algorithm>
#include "HashDB/HashDB.h"
#include <typeinfo>
#include <bit>
#include "Types/DCArray.h"
#include "Base64.h"
#include "VersDB.h"
#include "LegacyTypes/D3DMeshLegacy.h"

extern TelltaleVersionDatabase* sVersionDBs[KEY_COUNT];

METAOP_FUNC_IMPL_(DebugString, SerializeAsync) {
	MetaStream* pStream = (MetaStream*)pUserData;
	DebugString* me = (DebugString*)pObj;
	pStream->serialize_String(me);
	return eMetaOp_Succeed;
}

String MetaVersion_ConvertTypeName(const String& from)
{
	char* temp = (char*)malloc(from.length() + 1);
	memcpy(temp, from.c_str(), from.length() + 1);
	TelltaleToolLib_MakeInternalTypeName(&temp);
	String ret{ temp };
	free(temp);
	return std::move(ret);
}


bool MetaVersion_MemberExistsInCurrentVersion(MetaMemberDescription* member, MetaStream* stream)
{
	bool disable = false;
	if (!disable && member->mMinMetaVersion != -1 && member->mMinMetaVersion > stream->mStreamVersion)
		disable = true;
	if (member->mGameIndexVersionRange.min != -1 && member->mGameIndexVersionRange.max != -1) {
		if (member->mGameIndexVersionRange.max >= member->mGameIndexVersionRange.min) {
			disable = !(sSetKeyIndex >= member->mGameIndexVersionRange.min &&
				member->mGameIndexVersionRange.max >= sSetKeyIndex || sSetKeyIndex == member->mGameIndexVersionRange.min
				|| sSetKeyIndex == member->mGameIndexVersionRange.max);
		}
		else {
			disable = !(sSetKeyIndex >= member->mGameIndexVersionRange.min || member->mGameIndexVersionRange.max >= sSetKeyIndex);
		}
	}
	else {
		if (!disable && member->mGameIndexVersionRange.min != -1)
			disable = !(sSetKeyIndex >= member->mGameIndexVersionRange.min);
		if (!disable && member->mGameIndexVersionRange.max != -1)
			disable = !(member->mGameIndexVersionRange.max >= sSetKeyIndex);
	}
	if (!disable && member->mSkipVersion != -1)
		disable = sSetKeyIndex == member->mSkipVersion;
	return !disable;
}

i32 sMetaTypesCount = 0;
MetaClassDescription* spFirstMetaClassDescription = NULL;
char Symbol::smSymbolBuffer[sizeof(u64) * 2 + 1];//1byte= 2 hex chars
std::vector<const char*> sMetaFileExtensions;
SerializedVersionInfo* pSVIs = nullptr;

void __ReleaseSVI_Internal()
{
	SerializedVersionInfo* svi = pSVIs;
	while (svi) {
		
		SerializedVersionInfo* ssvi = svi->mpNext;
		delete svi;
		svi = ssvi;
	}
	pSVIs = nullptr;
}


void __RegisterSVI_Internal(void* svi)
{
	if (pSVIs) {
		SerializedVersionInfo* p = (SerializedVersionInfo*)svi;
		p->mpNext = pSVIs;
		pSVIs = p;
	}
	else {
		pSVIs = (SerializedVersionInfo*)svi;
	}
}

const Symbol Symbol::kEmptySymbol((u64)0x0);
const Symbol Symbol::kTransitionMapKey(0x1D699F33F83B11E1);
const Symbol Symbol::kAnimGroupArmLeft(0x0FDD033CA83C7CC60);
const Symbol Symbol::kAnimGroupArmRight(0x7AD0E8E64D6CABEE);
const Symbol Symbol::kAnimGroupBodyLower(0x0F50922E6477143B);
const Symbol Symbol::kAnimGroupBodyUpper(0x8A9036F7BCFD7AB3);
const Symbol Symbol::kAnimGroupBrowLeft(0x2EF0563AB7780155);
const Symbol Symbol::kAnimGroupBrowRight(0x0D766A5C40F201556);
const Symbol Symbol::kAnimGroupBrows(0x45C088C7AFC1C5A2);
const Symbol Symbol::kAnimGroupCheekLeft(0x9878FE96AE9C6D5);
const Symbol Symbol::kAnimGroupCheekRight(0x0E2E7C39F4B5829E2);
const Symbol Symbol::kAnimGroupCheeks(0x7E23A497CC8AD9A0);
const Symbol Symbol::kAnimGroupExtra(0x9E992D90A9EC7560);
const Symbol Symbol::kAnimGroupEyeLids(0x6ADD162AB8166D49);
const Symbol Symbol::kAnimGroupEyes(0x600E10E7529B1FF2);
const Symbol Symbol::kAnimGroupFace(0x6533C11BB1C71CB4);
const Symbol Symbol::kAnimGroupFingersLeft(0x8AD72C8D60925751);
const Symbol Symbol::kAnimGroupFingersRight(0x83AF6069B7C5829A);
const Symbol Symbol::kAnimGroupFootLeft(0x6EF8597BC4434C7);
const Symbol Symbol::kAnimGroupFootRight(0x96A935BA0E535536);
const Symbol Symbol::kAnimGroupHair(0x9854D6240AAA96DA);
const Symbol Symbol::kAnimGroupHandRight(0x79B93BD7D3511C18);
const Symbol Symbol::kAnimGroupHandLeft(0x0AC117633A8CC4BFC);
const Symbol Symbol::kAnimGroupHead(0x0CEEA01C1175FD3D4);
const Symbol Symbol::kAnimGroupIndexFLeft(0x97F7BCC2CFC25406);
const Symbol Symbol::kAnimGroupIndexFRight(0x5CA71C1DCC0A5387);
const Symbol Symbol::kAnimGroupLegLeft(0x94C7B1675F57225B);
const Symbol Symbol::kAnimGroupLegRight(0x0ABBB9BB1A3425532);
const Symbol Symbol::kAnimGroupMouth(0x0DE8CDE290439CFC);
const Symbol Symbol::kAnimGroupNeck(0x16FC55CE88D82BDD);
const Symbol Symbol::kAnimGroupRoot(0x0C8103D8109BF1619);
const Symbol Symbol::kAnimGroupShoulderLeft(0x0DBCBC9B1C0029);
const Symbol Symbol::kAnimGroupShoulderRight(0x0B842BB0F0BF13ACF);
const Symbol Symbol::kAnimGroupGroupSkirt(0x5CD6D053D121F4E5);
const Symbol Symbol::kAnimGroupGroupTail(0x540D53220EEA6FAE);
const Symbol Symbol::kAnimGroupThumbLeft(0x0CEA62D9A39C5B699);
const Symbol Symbol::kAnimGroupThumbRight(0x61981360040A876A);

const Symbol Symbol::kPropKeyViewportOrigin(0x813A2B6AA407BAEE);
const Symbol Symbol::kPropKeyViewportSize(0x352833D1C5C2BEF0);
const Symbol Symbol::kPropKeyViewportCamera(0x0F2C98EBF158AAD7C);
const Symbol Symbol::kPropKeyViewportRelative(0x9781E4AAD6D211FE);
const Symbol Symbol::kPropKeyAllowInterpolation(0x15E9ACA571067FF4);
const Symbol Symbol::kBGMStartPos(0x3996E3E0703235DF);

const Symbol Symbol::kBlocking(0x61FAE3CC00BF36E0);
const Symbol Symbol::kBlockingOffset(0x0D018ACA4A3C726F0);
const Symbol Symbol::kBlockingOffsetContribution(0x0FDD95D1052D4961E);
const Symbol Symbol::kResourceNameSuffix(0x2A5E5635F76B351C);
const Symbol Symbol::kLookAtBlockingAnimation(0x7F4CF374C5BF53AB);
const Symbol Symbol::kLookAtBlockingOffsetAnimation(0x85DB97FE2564A1FF);
const Symbol Symbol::kLookAtBlockingOffsetContributionAnimation(0x4EB28F2E4D6717D0);

const Symbol Symbol::kPropKeySource(0x71849165A7FEBD5D);
const Symbol Symbol::kPropKeyWidth(0x27A3026EB33AD3C0);
const Symbol Symbol::kPropKeyHeight(0x0CBC95316A497A878);
const Symbol Symbol::kPropKeyPause(0x9EB283D7925D9D8);
const Symbol Symbol::kPropKeyVolume(0x6FFDEE0FFB746B30);
const Symbol Symbol::kPropKeyMediaTimeout(0x55932B2532CBD67D);
const Symbol Symbol::kPropKeyVolumeMaster(0x0EA625C4F584F9E98);
const Symbol Symbol::kPropKeyVolumeMusic(0x0E0F0CA9C0B601F3A);
const Symbol Symbol::kPropKeyVolumeSound(0x0F1CD8FFFFE1CB324);
const Symbol Symbol::kPropKeyVolumeVoice(0x0D6C6E8FBC7217F7A);
const Symbol Symbol::kPropKeyMuteVoices(0x2584A25244B91688);
const Symbol Symbol::kPropKeyVolumeAmbient(0x8AE2D5ACAEEB9A75);
const Symbol Symbol::kPropKeyMixMaster(0x0DF545B0016B9C1AD);
const Symbol Symbol::kPropKeyMixMusic(0x427A9D4CAC21CC77);
const Symbol Symbol::kPropKeyMixSound(0x5347D82F595D6069);
const Symbol Symbol::kPropKeyMixVoice(0x744CBF2B6060AC37);
const Symbol Symbol::kPropKeyMixAmbient(0x1D61944C4E3EFB08);
const Symbol Symbol::kPropKeySoundMode(0x46CD0D2CD3CC671);
const Symbol Symbol::kCompressAnimations(0x0CA49FE4BC74493D8);
const Symbol Symbol::kEncryptMetaStreams(0x499CBE2087A14494);
const Symbol Symbol::kPropKeyROMRuntimeLoad(0x0E9A35CE893FF34E8);
const Symbol Symbol::kPropKeyROMErrorDialogResource(0x0F7668BDAC61E1072);
const Symbol Symbol::kPropKeyROMErrorTitle(0x416A401EC7E25730);
const Symbol Symbol::kPropKeyROMErrorMessage(0x4F40F0541CE2EA62);
const Symbol Symbol::kPropKeyROMPackSubDir(0x46F440F780023400);
const Symbol Symbol::kPropKeyTextMinSec(0x8527B917537F65C9);
const Symbol Symbol::kPropKeyTextSecPerLetter(0x83CAF72DF8F2EB1F);
const Symbol Symbol::kPropKeyUserSpaceAsArchive(0x33A683C29F0962CC);
const Symbol Symbol::kPropKeyDisableDevFolders(0x0FF1C9CC5453D63FF);
const Symbol Symbol::kPropKeyUsageOS(0x7374FDE41AF574B);
const Symbol Symbol::kPropKeyUsageCPU(0x0A32F8ABB6EBA4D07);
const Symbol Symbol::kPropKeyUsageGPU(0x2C9454C785E62E1B);
const Symbol Symbol::kPropKeyUsageDeviceType(0x458D1143CC5DC209);
const Symbol Symbol::kPropKeyUsageDeviceModel(0x8C5CF6463F8DD03D);
const Symbol Symbol::kPropKeyUsageDeviceName(0x8DB33658E3924173);
const Symbol Symbol::kPropKeyUsageLaunchCount(0x0D33D1B60305BFB00);
const Symbol Symbol::kPropKeyUsageShutdownCount(0x0DE7CD8777A93CC7C);
const Symbol Symbol::kPropKeyUsageSessionExecutionTime(0x0E315818985ABC18E);
const Symbol Symbol::kPropKeyUsageTotalExecutionTime(0x9F0163E810C0D1D6);
const Symbol Symbol::kChoreAgentGroupFilterIncludeNonSkeleton(0x0A630F7D394B1D054);
const Symbol Symbol::kPropOpenLocalizations(0x0F568BF7757E177CE);
const Symbol Symbol::kHDScreenShotResolution(0x14E6CA055FFEDE3A);

const Symbol Symbol::sAssertSymbol(0x73D89AB1D1AEC10);
const Symbol Symbol::sAssertTimeStampSymbol(0x0EC06AC175838C164);
const Symbol Symbol::sAssertFileSymbol(0x6A6B57F5CA393AEE);
const Symbol Symbol::sAssertLineSymbol(0x5D99B960E7BFA618);
const Symbol Symbol::sGeneralSymbol(0x93EBDD1A46882401);
const Symbol Symbol::sChoreSymbol(0x72B05EE52AB65857);
const Symbol Symbol::sLuaSymbol(0x7FBCF1D65029EC81);
const Symbol Symbol::sGraphicsSymbol(0x86B29480918CF4D3);
const Symbol Symbol::sDialogSymbol(0x0D8E59A21C780683D);
const Symbol Symbol::sAudioSymbol(0x841BFF4874C569FE);
const Symbol Symbol::sQtSymbol(0x0A4BD597513632DD6);
const Symbol Symbol::sExportSymbol(0x805FECD924A749DB);
const Symbol Symbol::sCoreSymbol(0x0A7C199CDCEFC0681);

void SerializedVersionInfo::RetrieveVersionInfo(const char* versFileName, DataStream* stream) {
	MetaStream meta;
	meta.Open(stream, MetaStreamMode::eMetaStream_Read, { 0 });
	u32 i;
	meta.serialize_uint32(&i);
	mFileName = versFileName;
	if (i == -1) {//reason we have this -1 (0xFFFFFFFF) header is because old .vers used to start with the .vers file name 
		//and the first int would therefore be the size of it. no string is 0xFFFFFFFF bytes long, so thats how we can distinguish
		mMembers.clear();
		meta.serialize_uint32(&i);
		if (i == 0) {//.vers version 0, never seen it!
			String str;
			meta.serialize_String(&str);
			MetaClassDescription* mcd = GetMetaClassDescription(str.c_str());
			if (mcd)
				mTypeSymbolCrc = mcd->mHash;
		}
		else if (i == 1) {//version 1, normal
			meta.serialize_uint64(&mTypeSymbolCrc);
		}
		meta.serialize_uint32(&mVersionCrc);
		meta.serialize_uint32(&mSize);
		meta.serialize_bool(&mbBlocked);
		u32 members;
		meta.serialize_uint32(&members);
		mMembers.clear();
		mMembers.reserve(members);
		for (int x = 0; x < members; x++) {
			SerializedVersionInfo::MemberDesc desc;
			meta.serialize_String(&desc.mName);
			if (i == 1)
				meta.serialize_uint64(&desc.mTypeNameSymbolCrc);
			else {
				String str;
				meta.serialize_String(&str);
				MetaClassDescription* mcd = GetMetaClassDescription(str.c_str());
				if (mcd)
					desc.mTypeNameSymbolCrc = mcd->mHash;
			}
			meta.serialize_uint32(&desc.mSize);
			meta.serialize_bool(&desc.mbBlocked);
			meta.serialize_uint32(&desc.mVersionCrc);
			mMembers.push_back(desc);
		}
		for (int x = 0; x < members; x++) {
			meta.serialize_String(&mMembers[x].mTypeName);
		}
		mbOldVersion = false;
	}
	else {
		mbOldVersion = true;
		mFileName = String(i, 'X');
		meta.serialize_bytes((void*)mFileName.c_str(), i);
		String typen{};
		meta.serialize_String(&typen);
		typen = MetaVersion_ConvertTypeName(typen);
		mTypeSymbolCrc = CRC64_CaseInsensitive(0, typen.c_str());
		meta.serialize_uint32(&mVersionCrc);
		meta.serialize_uint32(&mSize);
		meta.serialize_bool(&mbBlocked);
		u32 mem{};
		meta.serialize_uint32(&mem);
		mMembers.clear();
		mMembers.reserve(mem);
		for (int i = 0; i < mem; i++) {
			MemberDesc desc{};
			meta.serialize_String(&desc.mName);
			meta.serialize_String(&desc.mTypeName);
			desc.mTypeName = MetaVersion_ConvertTypeName(desc.mTypeName);
			meta.serialize_uint32(&desc.mSize);
			meta.serialize_bool(&desc.mbBlocked);
			meta.serialize_uint32(&desc.mVersionCrc);
			desc.mpMemberDesc = nullptr;
			desc.mTypeNameSymbolCrc = CRC64_CaseInsensitive(0, desc.mTypeName.c_str());
		}
	}
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

DataStream* SerializedVersionInfo::Save(String* name) {
	if (mbOldVersion)
		return nullptr;
	if (name != nullptr && name->size() == 0) {
		MetaClassDescription* pMCD = TelltaleToolLib_FindMetaClassDescription_ByHash(mTypeSymbolCrc);
		mFileName = *name = MakeVersionFileName(pMCD ? pMCD->mpTypeInfoName : "", mVersionCrc);
	}
	if (name != nullptr)
		mFileName = *name;
	DataStream* stream = new DataStreamMemory(0, 0x100);
	MetaStream meta{};
	meta.mbDontDeleteStream = true;
	meta.Open(stream, MetaStreamMode::eMetaStream_Write, { 0 });
	meta._SetSection(MetaStream::eSection_Default);
	u32 i = -1;
	meta.serialize_uint32(&i);
	i = 1;
	meta.serialize_uint32(&i);
	meta.serialize_uint64(&mTypeSymbolCrc);
	meta.serialize_uint32(&mVersionCrc);
	meta.serialize_uint32(&mSize);
	meta.serialize_bool(&mbBlocked);
	i = mMembers.size();
	meta.serialize_uint32(&i);
	for (int x = 0; x < mMembers.size(); x++) {
		SerializedVersionInfo::MemberDesc* desc = mMembers.data() + x;
		meta.serialize_String(&desc->mName);
		meta.serialize_uint64(&desc->mTypeNameSymbolCrc);
		meta.serialize_uint32(&desc->mSize);
		meta.serialize_bool(&desc->mbBlocked);
		meta.serialize_uint32(&desc->mVersionCrc);
	}
	for (int x = 0; x < mMembers.size(); x++) {
		SerializedVersionInfo::MemberDesc* desc = mMembers.data() + x;
		meta.serialize_String(&desc->mTypeName);
	}
	meta.Close();
	return stream;
}


String SerializedVersionInfo::MakeVersionFileName(const char* typeName, u32 tn)
{
	char* temp = (char*)malloc(512);
	char buf[30];
	_ui64toa(tn, buf, 36);
	std::string fn{ typeName };
	replace(fn, "<", "_");
	replace(fn, ">", "_");
	replace(fn, "::", "__");
	replace(fn, " ", "_");
	sprintf(temp, "%s(%s).vers", fn.c_str(), buf);
	String ret{ temp };
	free(temp);
	return std::move(ret);
}

void MetaStream::AddVersion(const SerializedVersionInfo* version) {
	for (int i = 0; i < mVersionInfo.size(); i++) {
		if (mVersionInfo[i].mTypeSymbolCrc == version->mTypeSymbolCrc)return;
	}
	mVersionInfo.push_back(MetaVersionInfo{ version->mTypeSymbolCrc, version->mVersionCrc });
}

void MetaStream::_WriteHeader() {
	//if (!mSection[0].mpStream || !mSection[0].mpStream->IsWrite())return;
	_SetSection(SectionType::eSection_Header);
	if (!mSection[0].mpStream) {
		printf("WARN: could not write header to meta stream");
		return;
	}
	u32 magic;
	if (mStreamVersion == 6 || mStreamVersion == 5) {//MSV5 and MSV6 are the same in serialized form. dont know why they needed MSV6,
		//MSV5 was the same lol, thinking its something backend. would need to look into a MSV5 game executable but dont have any pbds for 'em
		magic = GetMetaMagic(mStreamVersion);
		serialize_uint32(&magic);

		u32 default_ = mSection[(int)SectionType::
			eSection_Default].mpStream ? mSection[(int)SectionType::
			eSection_Default].mpStream->GetSize() : 0;
		mSection[(int)SectionType::
			eSection_Default].mCompressedSize = default_;

		u32 debug_ = mSection[(int)SectionType::
			eSection_Debug].mpStream ? mSection[(int)SectionType::
			eSection_Debug].mpStream->GetSize() : 0;
		mSection[(int)SectionType::
			eSection_Debug].mCompressedSize = debug_;

		u32 async_ = mSection[(int)SectionType::
			eSection_Async].mpStream ? mSection[(int)SectionType::
			eSection_Async].mpStream->GetSize() : 0;
		mSection[(int)SectionType::
			eSection_Async].mCompressedSize = async_;

		if (mSection[(int)SectionType::
			eSection_Default].mbCompressed)
			default_ |= 0x80000000;
		if (mSection[(int)SectionType::
			eSection_Debug].mbCompressed)
			debug_ |= 0x80000000;
		if (mSection[(int)SectionType::
			eSection_Async].mbCompressed)
			async_ |= 0x80000000;
		serialize_uint32(&default_);
		serialize_uint32(&debug_);
		serialize_uint32(&async_);
		u32 vers = mVersionInfo.size();
		serialize_uint32(&vers);
		for (int i = 0; i < vers; i++) {
			MetaVersionInfo verinfo = mVersionInfo[i];
			serialize_uint64(&verinfo.mTypeSymbolCrc);
			serialize_uint32(&verinfo.mVersionCrc);
		}
	}
	else {
		const char* errmsg = "Cannot write meta stream with version %d: Only version 5 & 6 are writable with this library";
		char endbuf[sizeof(errmsg) + 30];
		sprintf(endbuf, errmsg, mStreamVersion);
		TelltaleToolLib_RaiseError(errmsg, ErrorSeverity::ERR);
	}
}


void MetaStream::Key(const char* pS)
{

}

void MetaStream::SwitchToMode(MetaStreamMode newMode, DataStream* newStream) {
	if (newMode == mMode || !newStream)return;
	if (mpReadWriteStream && mpReadWriteStream != newStream && !mbDontDeleteStream)
		delete mpReadWriteStream;
	mSection[0].mpStream = newStream;
	if (mSection[1].mpStream)
		delete mSection[1].mpStream;
	mSection[1].mpStream = NULL;
	if (mSection[2].mpStream)
		delete mSection[2].mpStream;
	mSection[2].mpStream = NULL;
	if (mSection[3].mpStream)
		delete mSection[3].mpStream;
	mSection[3].mpStream = NULL;
	Open(newStream, newMode, { 0 });
}

bool MetaStream::_ReadHeader(DataStream* stream, u64 completeStreamSize,
	u64* pOutBytesNeeded) {
	if (!stream->IsRead())return false;
	mCurrentSection = SectionType::eSection_Header;
	mSection[0].mpStream = stream;
	u32 versionmagic, numvers;
	mStreamVersion = 0;
	bool isCompiled = true, encryptstream = false;
	mVersionInfo.clear();
	serialize_uint32(&versionmagic);
	if (versionmagic == GetMetaMagic(1) ||
		versionmagic == GetMetaMagic(2)) {//MBIN, MTRE
		mStreamVersion = versionmagic == GetMetaMagic(2) ? 3 : 1;
	}
	else if (versionmagic == GetMetaMagic(4) || versionmagic == GetMetaMagic(6) || versionmagic == GetMetaMagic(5)) {//MSV4,5,6
		u32 default_ = 0, debug_, async_;
		mStreamVersion = versionmagic == GetMetaMagic(4) ? 4 : versionmagic == GetMetaMagic(5) ? 5 : 6;
		if (mStreamVersion >= 5)
			serialize_uint32(&default_);
		serialize_uint32(&debug_);
		serialize_uint32(&async_);
		//u32 asyncsomething = 0;
		if (static_cast<i32>(default_) < 0) {//same as & 0x80000000 since if 0x80000000 is set then its negative
			mSection[1].mbCompressed = true;
			default_ &= 0x7FFFFFFF;
		}
		if (debug_ & 0x80000000) {
			mSection[2].mbCompressed = true;
			debug_ &= 0x7FFFFFFF;
		}
		if (async_ & 0x80000000) {
			mSection[3].mbCompressed = true;
			async_ &= 0x7FFFFFFF;
			//asyncsomething = 8 * (async_ >> 12);
		}
		mSection[1].mCompressedSize = default_;
		mSection[2].mCompressedSize = debug_;
		mSection[3].mCompressedSize = async_;
	}
	else {//MBES,MCOM and encrypted streams
		mWriteParams.mbEncryptStream = true;
		if (versionmagic == GetMetaMagic(0)) {//MBES
			mStreamVersion = 1;
		}
		else if (versionmagic == GetMetaMagic(3) || versionmagic == EncrypedVersionHeaders[1]) {//MCOM
			mStreamVersion = 3;
			Advance(4);//some int
			if (versionmagic == GetMetaMagic(3)) {
				mWriteParams.mbSerializeAsCompressVersion = true;
				goto VersionInfo;
			}
		}
		else if (versionmagic == EncrypedVersionHeaders[0]) {
			mStreamVersion = 3;
		}
		else if (versionmagic == EncrypedVersionHeaders[2] ||
			versionmagic == EncrypedVersionHeaders[3] ||
			versionmagic == EncrypedVersionHeaders[4]) {
			mStreamVersion = 2;
		}
		if (!mStreamVersion)return false;
		mWriteParams.mEncryptVersion = mStreamVersion;
		DataStream* nstream = new DataStreamLegacyEncrypted(mSection[0].mpStream, mStreamVersion, stream->GetPosition());
		mSection[0].mpStream = nstream;//dont delete old one, refernced still in metastream::mreadwritestream
		encryptstream = true;
	}
VersionInfo:
	serialize_uint32(&numvers);
	if (numvers > 0 && numvers <= 0x3E8) {
		mVersionInfo.reserve(numvers);
		for (int i = 0; i < numvers; i++) {
			MetaVersionInfo verinfo{ 0 };
			if (mStreamVersion >= 3) {
				serialize_uint64(&verinfo.mTypeSymbolCrc);
			}
			else {
				String typeName;
				serialize_String(&typeName);
				char* buf = (char*)calloc(1, typeName.size() + 1);
				if (!buf)return false;
				memcpy(buf, typeName.c_str(), typeName.size());
				TelltaleToolLib_MakeInternalTypeName(&buf);
				typeName = buf;
				free(buf);
				//SubsituteClassNames tf? cba
				verinfo.mTypeSymbolCrc = CRC64_CaseInsensitive(0, typeName.c_str());
			}
			serialize_uint32(&verinfo.mVersionCrc);
			MetaClassDescription* desc = TelltaleToolLib_FindMetaClassDescription_ByHash(verinfo.mTypeSymbolCrc);
			if (desc)SerializedVersionInfo::RetrieveCompiledVersionInfo(desc);
			if (!desc || !desc->mpCompiledVersionSerializedVersionInfo ||
				verinfo.mVersionCrc != desc->mpCompiledVersionSerializedVersionInfo->mVersionCrc)
				isCompiled = false;
			mVersionInfo.push_back(verinfo);
		}
	}
	mSection[0].mCompressedSize = GetPos();
	if (mStreamVersion <= 3) {//MBIN,MBES,MTRE,MCOM (unsectioned streams)
		if (encryptstream)mSection[0].mCompressedSize -= 4;
		mSection[1].mCompressedSize = stream->GetSize() - mSection[0].mCompressedSize;
		mSection[2].mCompressedSize = 0;
		mSection[3].mCompressedSize = 0;
	}
	if (isCompiled) {
		mRuntimeFlags |= (int)RuntimeFlags::eStreamIsCompiledVersion;
	}
	else {
		mRuntimeFlags &= ~((int)RuntimeFlags::eStreamIsCompiledVersion);
	}
	return true;
}

void MetaStream::BeginBlock() {
	SectionInfo& sect = mSection[(int)mCurrentSection];
	BlockInfo b{ GetPos() };
	serialize_uint32(&b.mBlockLength);
	if (mMode == MetaStreamMode::eMetaStream_Read)
		b.mBlockLength += GetPos() - 4;//set block len to end of block offset
	sect.mBlockInfo.push(b);
}

void MetaStream::EndObject(const char* name, MetaClassDescription* pDesc, MetaMemberDescription* pContext) {
	if (!(pDesc->mFlags.mFlags & MetaFlag::MetaFlag_MetaSerializeBlockingDisabled)
		&& !(pContext->mFlags & MetaFlag::MetaFlag_MetaSerializeBlockingDisabled))
		EndBlock();
}


void MetaStream::EndObject(const char*)
{

}

bool MetaStream::NeedsKey()
{
	return false;
}

MetaClassDescription* GetMetaClassDescription(const char* typeInfoName) {
	if (!sInitialized)return NULL;
	MetaClassDescription* i = spFirstMetaClassDescription;
	if (typeInfoName == NULL) {
		return NULL;
	}
	else {
		for (i; i; i = i->pNextMetaClassDescription) {
			if (i->mbHiddenInternal)
				continue;
			if (!_strcmpi(typeInfoName, i->mpTypeInfoExternalName))
				return i;
		}
	}
	return NULL;
}

template<typename T> MetaClassDescription* MetaClassDescription_Typed<T>::GetMetaClassDescription(const char* type) {
	if (!sInitialized)return NULL;
	MetaClassDescription* i = spFirstMetaClassDescription;
	if (type == NULL) {
		for (i; i; i = i->pNextMetaClassDescription) {
			if (i->mbHiddenInternal)
				continue;
			if (!_strcmpi(typeid(T).name(), i->mpTypeInfoExternalName))
				return i;
		}
	}
	else {
		for (i; i; i = i->pNextMetaClassDescription) {
			if (i->mbHiddenInternal)
				continue;
			if (!_strcmpi(typeid(T).name(), type))
				return i;
		}
	}
	return NULL;
}

SerializedVersionInfo* SerializedVersionInfo::RetrieveCompiledVersionInfo(MetaClassDescription* pObjDescription) {
	SerializedVersionInfo* result = pObjDescription->mpCompiledVersionSerializedVersionInfo;
	if (!result) {
		result = new SerializedVersionInfo;
		__RegisterSVI_Internal(result);
		MetaOperation op = pObjDescription->GetOperationSpecialization(MetaOperationDescription::eMetaOpTwentyOne);
		if (op) {
			op(NULL, pObjDescription, NULL, result);
		}
		else {
			Meta::MetaOperation_SerializedVersionInfo(NULL, pObjDescription, NULL, result);
		}
		pObjDescription->mpCompiledVersionSerializedVersionInfo = result;
	}
	return result;
}

void MetaStream::BeginObject(const char* name, MetaClassDescription* pDesc, MetaMemberDescription* pContext,bool) {
	if (!(pDesc->mFlags.mFlags & MetaFlag::MetaFlag_MetaSerializeBlockingDisabled)
		&& !(pContext->mFlags & MetaFlag::MetaFlag_MetaSerializeBlockingDisabled))
		BeginBlock();
}


void MetaStream::BeginObject(const char*, bool bArray)
{
	// nothing DO NOT CHANGE.
}

void MetaStream::SkipToEndOfCurrentBlock() {//used in read mode really
	SectionInfo& sect = mSection[(int)mCurrentSection];
	if (sect.mBlockInfo.size()) {
		BlockInfo b = sect.mBlockInfo.top();
		SetPos(b.mBlockLength);
		sect.mBlockInfo.pop();
	}
}

void MetaStream::EndBlock() {
	SectionInfo& sect = mSection[(int)mCurrentSection];
	BlockInfo b = sect.mBlockInfo.top();
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		u64 currentpos = GetPos();
		SetPos(b.mBlockLength);
		u32 blocklen = currentpos - b.mBlockLength;
		serialize_uint32(&blocklen);
		SetPos(currentpos);
	}
	else if (GetPos() != b.mBlockLength) {
		SetPos(b.mBlockLength);
	}
	sect.mBlockInfo.pop();
}

void MetaStream::PushMissingMemberCallback(bool(*cb)(SerializedVersionInfo::MemberDesc*, void*),
	void* ud) {
	MissingMemberCallbackInfo i;
	i.mpUserData = ud;
	i.mMissingMemberCallback = cb;
	mMissingMemberCallbacks.push_back(i);
}

void MetaStream::PopMissingMemberCallback() {
	mMissingMemberCallbacks.pop_back();
}

u64 MetaStream::GetPos() {
	return mSection[(int)mCurrentSection].mpStream->GetPosition();
}

MetaVersionInfo* MetaStream::GetStreamVersion(u64 typeSymbolCrc) {//could use auto
	for (std::vector<MetaVersionInfo>::iterator it = mVersionInfo.begin(); it != mVersionInfo.end(); it++) {
		if (it->mTypeSymbolCrc == typeSymbolCrc)return mVersionInfo.data() + std::distance(mVersionInfo.begin(), it);
	}
	return NULL;
}

MetaVersionInfo* MetaStream::GetStreamVersion(MetaClassDescription* d) { return GetStreamVersion(d->mHash); };

void MetaStream::SetPos(u64 pos) {
	mSection[(int)mCurrentSection].mpStream->SetPosition(pos, DataStreamSeekType::eSeekType_Begin);
}

u64 MetaStream::GetSize() {
	return mSection[(int)mCurrentSection].mpStream->GetSize();
}

u64 MetaStream::GetPartialStreamSize() {
	return mSection[(int)SectionType::eSection_Default].mpStream->GetSize()
		+ mSection[(int)SectionType::eSection_Async].mpStream->GetSize()
		+ mSection[(int)SectionType::eSection_Debug].mpStream->GetSize();
}

bool MetaStream::HasAsyncSection() {
	return mSection[(int)SectionType::eSection_Async].mpStream->GetSize() != 0;
}

bool MetaStream::HasDebugSection() {
	return mSection[(int)SectionType::eSection_Debug].mpStream->GetSize() != 0;
}

void MetaStream::EndAsyncSection() {
	if (mCurrentSection == SectionType::eSection_Async) {
		_SetSection(SectionType::eSection_Default);
	}
}

bool MetaStream::BeginAsyncSection() {
	return mCurrentSection == SectionType::eSection_Default ? _SetSection(SectionType::eSection_Async) : false;
}

void MetaStream::DisableDebugSection() {
	SectionInfo& debug = mSection[(int)SectionType::eSection_Debug];
	debug.mbEnable = false;
	if (debug.mpStream)
		delete debug.mpStream;
	debug.mpStream = nullptr;
	if (mCurrentSection == SectionType::eSection_Debug)
		EndDebugSection();
}

void MetaStream::EndDebugSection() {
	if (mCurrentSection == SectionType::eSection_Debug && mDebugSectionDepth > 0) {
		mDebugSectionDepth--;
		if (!mDebugSectionDepth)
			_SetSection(SectionType::eSection_Default);
	}
}

bool MetaStream::BeginDebugSection() {
	if ((mCurrentSection == SectionType::eSection_Default && !mDebugSectionDepth && _SetSection(SectionType::eSection_Debug))
		|| (mCurrentSection == SectionType::eSection_Debug && mDebugSectionDepth > 0)) {
		if (IsRead() && !mSection[2].mpStream->GetSize()) {
			_SetSection(eSection_Default);
			return false;
		}
		mDebugSectionDepth++;
		return true;
	}
	return false;
}

void MetaStream::Advance(int numBytes) {
	mpReadWriteStream->SetPosition(numBytes, DataStreamSeekType::eSeekType_Current);
}

Symbol MetaClassDescription::GetDescriptionSymbol() {
	String result;
	GetToolDescriptionName(&result);
	Symbol nsym(result.c_str());
	return nsym;
}

void MetaStream::serialize_Symbol(Symbol* symbol) {
	static MetaClassDescription* desc = NULL;
	if (!desc) {
		desc = GetMetaClassDescription(typeid(Symbol).name());
		if (!desc) {
			TelltaleToolLib_RaiseError("Meta not initialized to serialize 'Symbol'",ErrorSeverity::ERR);
			return;
		}
	}
	if (mMode == MetaStreamMode::eMetaStream_Write) {
		SerializedVersionInfo* versionInfo = desc->mpCompiledVersionSerializedVersionInfo;
		if (!versionInfo)
			versionInfo = SerializedVersionInfo::RetrieveCompiledVersionInfo(desc);
		AddVersion(desc->mpCompiledVersionSerializedVersionInfo);
	}
	u64 crc = symbol->GetCRC();
	serialize_uint64(&crc);
	symbol->SetCRC(crc);
	/*if (mStreamVersion < 4 || BeginDebugSection()) {
		u32 i = 0;
		serialize_uint32(&i);//string
		Advance(i);
		if (mStreamVersion >= 4)
			EndDebugSection();
	}*/
}

_TTToolLib_Exp MetaStream* TelltaleToolLib_CreateAndOpenMetaStream(const char* pFileMemory, int fileSize) {
	MetaStream* pStream = new MetaStream();
	if (pStream) {
		DataStreamMemory* pMemoryStream = new DataStreamMemory(const_cast<char*>(pFileMemory), fileSize, DataStreamMode::eMode_Read);
		if (!pMemoryStream) {
			delete pStream;
			TelltaleToolLib_RaiseError("Data stream could not be allocated for opening meta stream", ErrorSeverity::ERR);
			return nullptr;
		}
		pStream->Open(pMemoryStream, MetaStreamMode::eMetaStream_Read, {});
	}else
		TelltaleToolLib_RaiseError("Could not allocate meta stream", ErrorSeverity::ERR);
	return pStream;
}

_TTToolLib_Exp void TelltaleToolLib_MetaStreamSwitchToWrite(MetaStream* pStream) {
	if (pStream) {
		DataStreamMemory* pNewWriteStream = new DataStreamMemory(0x10000);
		pStream->SwitchToMode(MetaStreamMode::eMetaStream_Write, pNewWriteStream);
	}
}

_TTToolLib_Exp void* TelltaleToolLib_DeleteAndFlushMetaStream(MetaStream* pStream, u64* sz) {
	void* ret = nullptr;
	if (pStream) {
		if (sz) {
			if (pStream->mMode != MetaStreamMode::eMetaStream_Write) {
				delete pStream;
				TelltaleToolLib_RaiseError("Cannot get data from stream because its not in write mode", WARN);
				return ret;
			}
			*sz = pStream->Close();
			ret = TelltaleToolLib_Malloc(*sz);
			DataStreamMemory* pMemory = (DataStreamMemory*)pStream->mpReadWriteStream;
			memcpy(ret, pMemory->mMemoryBuffer, *sz);
		}
		delete pStream;
	}
	return ret;
}

_TTToolLib_Exp MetaOpResult TelltaleToolLib_SerializeMetaStream(MetaStream* pStream, MetaClassDescription* pClass, void* pClassInstance) {
	if (!pStream || !pClass || !pClassInstance)
		return MetaOpResult::eMetaOp_Fail;
	return PerformMetaSerializeFull(pStream, pClassInstance, pClass);
}


MetaStream::MetaStream() {
	MetaStream* v1;
	v1 = this;
	this->mpReadWriteStream = 0i64;
	this->mMode = MetaStreamMode::eMetaStream_Closed;
	this->mStreamVersion = 0;
	v1->mRuntimeFlags.mFlags = 0;
	//int len = strlen(Name);
	//memcpy(mName, Name, len >= 260 ? 259 : len);
}

void MetaStream::serialize_uint64(u64* param) {
	/*if (std::endian::native == std::endian::big) {
		if (mMode == MetaStreamMode::eMetaStream_Read)
			ReadData(param, 8);
		u64 p = 0;
		p |= ((*param & 0x00000000000000FF) << 56);
		p |= ((*param & 0x000000000000FF00) << 40);
		p |= ((*param & 0x0000000000FF0000) << 24);
		p |= ((*param & 0x00000000FF000000) <<  8);
		p |= ((*param & 0x000000FF00000000) >>  8);
		p |= ((*param & 0x0000FF0000000000) >> 24);
		p |= ((*param & 0x00FF000000000000) >> 40);
		p |= ((*param & 0xFF00000000000000) >> 56);
		if (mMode == MetaStreamMode::eMetaStream_Write)
			WriteData(&p, 8);
		else
			*param = p;
		return;
	}*/
	if (mMode == MetaStreamMode::eMetaStream_Read) {
		ReadData(param, 8);
	}
	else {
		WriteData(param, 8);
	}
}


void MetaStream::serialize_uint32(u32* param) {
	/*if (std::endian::native == std::endian::big) {
		if (mMode == MetaStreamMode::eMetaStream_Read)
			ReadData(param, 4);
		u32 p = 0;
		p |= ((*param & 0xFF000000) >> 24);
		p |= ((*param & 0xFF0000) >> 8);
		p |= ((*param & 0xFF00) << 8);
		p |= ((*param & 0xFF) << 24);
		if (mMode == MetaStreamMode::eMetaStream_Write)
			WriteData(&p, 4);
		else
			*param = p;
		return;
	}*/
	if (mMode == MetaStreamMode::eMetaStream_Read) {
		ReadData(param, 4);
	}
	else {
		WriteData(param, 4);
	}
}


void MetaStream::serialize_uint16(u16* param) {
	/*if (std::endian::native == std::endian::big) {
		if (mMode == MetaStreamMode::eMetaStream_Read)
			ReadData(param, 2);
		u16 p = 0;
		p |= ((*param & 0x00FF) << 8);
		p |= ((*param & 0xFF00) >> 8);
		if (mMode == MetaStreamMode::eMetaStream_Write)
			WriteData(&p, 2);
		else
			*param = p;
		return;
	}*/
	if (mMode == MetaStreamMode::eMetaStream_Read) {
		ReadData(param, 2);
	}
	else {
		WriteData(param, 2);
	}
}

void MetaStream::serialize_int8(char* param) {
	if (mMode == MetaStreamMode::eMetaStream_Read) {
		ReadData(param, 1);
	}
	else {
		WriteData(param, 1);
	}
}


void MetaStream::HintSymbol(const char* symbolData)
{

}

void MetaStream::InsertStringHint(const char* pK, const char* pS)
{

}

const char* MetaStream::SkipStringHint(const char* pK)
{
	return nullptr;
}

void MetaStream::serialize_float(float* param) {
	serialize_uint32((u32*)param);
}

void MetaStream::serialize_double(long double* param) {
	serialize_uint64((u64*)param);
}

void MetaStream::serialize_bool(bool* param) {
	char val = (*param != 0) + 0x30;
	serialize_bytes((void*)&val, 1);
	if ((u8)(val - 0x30) <= 1u) {
		*param = val == 0x31;
	}
	else {
		int x = 00'00'00'00;
		serialize_bytes(&x, 3);
		*param = val || x;
	}
}

void MetaStream::serialize_String(String* param) {
	static char sTempBuffer[512];
	u32 size = param->size();
	serialize_uint32(&size);
	if (mMode == MetaStreamMode::eMetaStream_Read) {
		if (512 >= size) {
			ReadData(sTempBuffer, size);
			param->assign(sTempBuffer, size);
		}
		else {
			void* tempbuffer = malloc(size);
			ReadData(tempbuffer, size);
			param->assign((char*)tempbuffer, size);
			free(tempbuffer);
		}
	}
	else if (size) {
		WriteData((void*)param->c_str(), size);
	}
}

i64 MetaStream::WriteData(void* bytes, u32 size) {
	if (!bytes)return NULL;
	if (!size)return 0;
	DataStream* stream = mSection[(int)mCurrentSection].mpStream;
	if (!stream)return -1;
	if (!stream->IsWrite())return -1;
	if (!stream->Serialize((char*)bytes, size))return -1;
	return size;
}

i64 MetaStream::ReadData(void* bytes, u32 size) {
	if (!bytes)return NULL;
	if (!size)return 0;
	DataStream* stream = mSection[(int)mCurrentSection].mpStream;
	if (!stream)return -1;
	if (!stream->IsRead())return -1;//can convert -1 (signed) from 64 to 32bit without problem as its 0xFFFFFFFF etc
	if (!stream->Serialize((char*)bytes, size))return -1;
	return size;
}

int MetaStream::serialize_bytes(void* bytes, u32 size) {
	if (!bytes && size)return eMetaOp_Fail;
	return mMode == MetaStreamMode::eMetaStream_Read ? ReadData(bytes, size) : WriteData(bytes, size);
}

void _unused_progressf(const char*, const float) {}

//checks if chunks need compressing and compresses
void MetaStream::_FinalizeStream() {
	for (int i = 1; i <= 3; i++) {
		SectionInfo& sect = mSection[i];
		if (mParams.mbCompress && sect.mpStream->GetSize() > 0x8000) {
			DataStreamContainerParams params{};
			DataStreamMemory* out_stream = new DataStreamMemory(0x10000, 0x10000);
			params.mbEncrypt = false;
			params.mWindowSize = 0x10000;
			params.mCompressionLibrary = Compression::Library::ZLIB;
			params.mDstOffset = 0;
			params.mpSrcStream = sect.mpStream;
			params.mpDstStream = out_stream;
			params.mbCompress = true;
			sect.mbCompressed = true;
			sect.mpStream = out_stream;
			DataStreamContainer::Create(
				_unused_progressf, params, 
				params.mpSrcStream->GetSize());
			delete params.mpSrcStream;
		}
		else {
			sect.mCompressedSize = sect.mpStream ? sect.mpStream->GetSize() : 0;
			sect.mbCompressed = false;
		}
	}
}

bool MetaStream::_SetSection(SectionType s) {
	SectionInfo& sect = mSection[(int)s];
	if (sect.mpStream) {
		mCurrentSection = s;
		//sect.mpStream->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
		return true;
	}
	if (!sect.mbEnable || mMode != MetaStreamMode::eMetaStream_Write)return false;
	sect.mpStream = new DataStreamMemory(0, 0x4000ui64);
	mCurrentSection = s;
	return true;
}

MetaStream::~MetaStream() {
	Close();
	if (mpReadWriteStream) {
		if (mpReadWriteStream != mSection[0].mpStream)
			delete mSection[0].mpStream;
		if (!mbDontDeleteStream)
			delete mpReadWriteStream;
	}
}


bool MetaStream::Attach(DataStream* stream, MetaStreamMode mode, MetaStreamParams params) {
	if (mode == MetaStreamMode::eMetaStream_Closed || !stream)return false;
	this->mMode = mode;
	this->mpReadWriteStream = stream;
	if (mode != MetaStreamMode::eMetaStream_Read) {
		if (!mStreamVersion)
			this->mStreamVersion = 6;//MSV6
		mParams = params;
		//mSection[0].mpStream = stream;
		_SetSection(MetaStream::SectionType::eSection_Default);
		return true;
	}
	u64 completeStreamSize = stream->GetSize();
	if (_ReadHeader(stream, completeStreamSize, NULL)) {
		u64 offset = mSection[0].mCompressedSize;
		for (int i = 1; i <= 3; i++) {//for each section (default,async,debug)
			SectionInfo& currentSect = mSection[i];
			if (currentSect.mCompressedSize) {
				currentSect.mpStream = mSection[0].mpStream->GetSubStream(offset, currentSect.mCompressedSize);
				if (currentSect.mbCompressed) {
					DataStreamContainerParams params{};
					u64 u=0;
					params.mpSrcStream = currentSect.mpStream;
					DataStreamContainer* c = new DataStreamContainer(params);
					c->Read(0, &u);
					currentSect.mpStream = c;
				}
				offset += currentSect.mCompressedSize;
			}
		}
		if (!METASTREAM_ENABLE_DEBUG) {
			//delete debug stream since this version was not built in debug mode, was built in release
			DataStream* debugStream = mSection
				[(int)SectionType::eSection_Debug].mpStream;
			if (debugStream)
				delete debugStream;
			mSection[(int)SectionType::eSection_Debug].mpStream = NULL;
			mSection[(int)SectionType::eSection_Debug].mCompressedSize = 0;
		}
		mCurrentSection = SectionType::eSection_Default;
	}
	else {
		mbErrored = true;
		return false;
	}
	return true;
}



u64 MetaStream::Close() {
	if (this->mMode != MetaStreamMode::eMetaStream_Closed) {
		u64 completeStreamSize = 0;
		if (mMode == MetaStreamMode::eMetaStream_Read && !mbErrored) {
			completeStreamSize = mSection[0].mpStream->GetSize();
			for (int i = 1; i <= 3; i++) {
				if (mSection[i].mpStream)
					completeStreamSize += mSection[i].mpStream->GetSize();
			}
		}
		else if (mMode == MetaStreamMode::eMetaStream_Write) {
			if (!mpReadWriteStream)return 0;
			_FinalizeStream();
			_WriteHeader();
			for (int i = (int)SectionType::eSection_Header; i < (int)SectionType::eSection_Count; i++) {
				if (mSection[i].mpStream) {
					mSection[i].mCompressedSize = mSection[i].mpStream->GetSize();
					mSection[i].mpStream->Transfer(mpReadWriteStream, 0, mSection[i].mCompressedSize);
				}
			}
			completeStreamSize = mpReadWriteStream->GetSize();
		}
		for (int i = 1; i < (int)SectionType::eSection_Count; i++) {
			if (mSection[i].mpStream && mSection[i].mpStream->GetSize()) {
				if (mSection[i].mpStream)
					delete mSection[i].mpStream;
				mSection[i].mpStream = NULL;
				mSection[i].mCompressedSize = 0;
				mSection[i].mbEnable = true;
			}
		}
		mMode = MetaStreamMode::eMetaStream_Closed;
		return completeStreamSize;
	}
	return 0;
}

void MetaStream::Open(DataStream* stream, MetaStreamMode mode, MetaStreamParams p) {
	mbErrored = false;
	//here, the datastream would be loaded from the resource getters, since its not the tool
	//we can just let you pass the file stream in/datastream
	Attach(stream, mode, p);
}

METAOP_FUNC_IMPL_(T3VertexSampleDataBase, SerializeAsync) {
	MetaStream* ms = static_cast<MetaStream*>(pUserData);
	MetaOpResult result = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
	if (result == MetaOpResult::eMetaOp_Succeed) {
		T3VertexSampleDataBase* db = static_cast<T3VertexSampleDataBase*>(pObj);
		int verts_size = db->mNumVerts * db->mVertSize;
		if (verts_size) {
			if (ms->mMode == MetaStreamMode::eMetaStream_Read) {
				db->mpData = (char*)malloc(verts_size);
			}
			ms->serialize_bytes(db->mpData, verts_size);
		}
	}
	return result;
}

MetaMemberDescription::~MetaMemberDescription() {
	if (this->mFlags & (MetaFlag::MetaFlag_EnumIntType | MetaFlag::MetaFlag_EnumStringType)) {
		MetaEnumDescription* i = this->mpEnumDescriptions;
		while (i) {
			if (i->mpEnumName && (i->mFlags & MetaFlag::MetaFlag_Heap)) {
				free((void*)i->mpEnumName);
				MetaEnumDescription* next = i->mpNext;
				free(i);
				i = next;
				continue;
			}
			else {
				i->mpEnumName = NULL;
				i->mFlags = 0;
				i->mpNext;
			}

			i = i->mpNext;
		}
	}
	else if (this->mFlags & MetaFlag::MetaFlag_FlagType) {
		for (MetaEnumDescription* i = this->mpEnumDescriptions; i; i = i->mpNext) {
			i->mpEnumName = NULL;
			i->mFlags = 0;
		}
	}
}

void* MetaClassDescription::CastToBase(const void* pObj, MetaClassDescription* pBaseClassDesc) {
	if (this == pBaseClassDesc)
		return (void*)pObj;
	void* result = NULL;
	MetaMemberDescription* mem = mpFirstMember;
	if (mem) {
		while (true) {
			if (mem->mFlags & MetaFlag::MetaFlag_BaseClass) {
				if (result = mem->mpMemberDesc->CastToBase(&((char*)pObj)[mem->mOffset], pBaseClassDesc))
					return result;
			}
			mem = mem->mpNextMember;
			if (!mem)
				break;
		}
	}
	return NULL;
}

MetaOperation MetaClassDescription::GetOperationSpecialization(int ID) {
	if (this->mMetaOperationsList) {
		MetaOperationDescription* i = mMetaOperationsList;
		while (i->id != ID) {
			i = i->mpNext;
			if (!i)return NULL;
		}
		return i->mpOpFn;
	}
	else {
		return NULL;
	}
}

void MetaClassDescription::InstallSpecializedMetaOperation(MetaOperationDescription* pNewOperation) {
	if (this->mMetaOperationsList) {
		MetaOperationDescription* i = this->mMetaOperationsList;
		while (i->id != pNewOperation->id) {
			i = i->mpNext;
			if (!i) {
				pNewOperation->mpNext = this->mMetaOperationsList;
				this->mMetaOperationsList = pNewOperation;
				return;
			}
		}
	}
	else {
		pNewOperation->mpNext = this->mMetaOperationsList;//this->mMetaOperationsList is NULL so its just NULL
		this->mMetaOperationsList = pNewOperation;
	}
}

bool MetaClassDescription::IsDerivedFrom(MetaClassDescription* pDesc) {
	if (this == pDesc)return true;
	if (pDesc) {
		for (MetaMemberDescription* i = this->mpFirstMember; i; i = i->mpNextMember) {
			if (i->mFlags & MetaFlag::MetaFlag_BaseClass) {
				if (i->mpMemberDesc->IsDerivedFrom(pDesc))return true;
			}
		}
	}
	return false;
}

void MetaClassDescription::Insert() {
	if (!pNextMetaClassDescription) {
		pNextMetaClassDescription = spFirstMetaClassDescription;
		spFirstMetaClassDescription = this;
	}
	this->mFlags.mFlags |= MetaFlag::Internal_MetaFlag_Initialized;
	if (mpExt) {
		sMetaFileExtensions.push_back(mpExt);
	}
}

void MetaClassDescription::Initialize(const char* typeInfoName) {
	mpTypeInfoName = typeInfoName;
	if(!mpTypeInfoExternalName)
		mpTypeInfoExternalName = typeInfoName;
	mHash = CRC64_CaseInsensitive(0, typeInfoName);
}

MetaMemberDescription* MetaClassDescription::GetMemberDescription(const char* memberName) {
	for (MetaMemberDescription* i = mpFirstMember; i; i = i->mpNextMember) {
		if (!strcmp(i->mpName, memberName))return i;
	}
	return NULL;
}

void MetaClassDescription::GetDescriptionSymbol(Symbol* sym) {
	String result;
	GetToolDescriptionName(&result);
	Symbol nsym(result.c_str());
	*sym = nsym;
}


METAOP_FUNC_IMPL(Destroy) {
	pObjDescription->Delete(pObj);
	return MetaOpResult::eMetaOp_Succeed;
}

void MetaClassDescription::CopyConstruct(void* pDest, void* pSrc) {
	if (mpVTable && mpVTable[3])
		((FuncCopyConstruct)mpVTable[3])(pDest, pSrc);
}

bool MetaClassDescription::MatchesHash(u64 o) {
	return o == mHash;
}

MetaClassDescription::~MetaClassDescription() {
	if (this->mpSerializeAccel)
		delete[] this->mpSerializeAccel;
	if (mbNameIsHeapAllocated)
		free((void*)mpTypeInfoName);
}

void MetaClassDescription::Construct(void* pObj) {
	if (mpVTable && mpVTable[2] && pObj)
		((FuncConstruct)(mpVTable[2]))(pObj);
}

void* MetaClassDescription::New() {
	if (mpVTable && mpVTable[0])
		return ((FuncNew)mpVTable[0])();
	return NULL;
}

void MetaClassDescription::Destroy(void* pObj) {
	if (mpVTable && mpVTable[4] && pObj)
		((FuncDelete)(mpVTable[4]))(pObj);
}

void MetaClassDescription::Delete(void* pObj) {
	if (mpVTable && mpVTable[1] && pObj)
		((FuncDelete)(mpVTable[1]))(pObj);
}

String* MetaClassDescription::GetToolDescriptionName(String* result) {
	int size = strlen(mpTypeInfoName);
	char* tmp = (char*)calloc(1, size + 1);
	memcpy(tmp, mpTypeInfoName, size);
	TelltaleToolLib_MakeInternalTypeName(&tmp);
	new (result) String(tmp);
	free(tmp);
	return result;
}

METAOP_FUNC_IMPL(EnumerateMembers) {
	Meta::EnumerateMembersInfo* handle = static_cast<Meta::EnumerateMembersInfo*>(pUserData);
	if (!handle)return MetaOpResult::eMetaOp_Fail;
	for (MetaMemberDescription* member = pObjDescription->mpFirstMember; member; member = member->mpNextMember) {
		handle->mpFunc(&((char*)pObj)[member->mOffset], member->mpMemberDesc, member);
	}
	return MetaOpResult::eMetaOp_Succeed;
}

METAOP_FUNC_IMPL(Equivalence) {
	MetaOpResult(*function)(void*, MetaClassDescription*, MetaMemberDescription*, void*);
	Meta::Equivalence* userData = static_cast<Meta::Equivalence*>(pUserData);
	char isEqual;
	if (pObjDescription->mpFirstMember) {
		userData->mbEqual = 1;
		for (MetaMemberDescription* i = pObjDescription->mpFirstMember; i; i = i->mpNextMember) {
			isEqual = false;
			function = i->mpMemberDesc->GetOperationSpecialization(MetaOperationDescription::sIDs::eMetaOpNine);
			if (function) {
				function(&((char*)pObj)[i->mOffset], i->mpMemberDesc, i, &isEqual);
			}
			else {
				Meta::MetaOperation_Equivalence(&((char*)pObj)[i->mOffset], i->mpMemberDesc, i, &isEqual);
			}
			if (!isEqual)
				break;
			if (!i->mpNextMember) return MetaOpResult::eMetaOp_Succeed;
		}
		userData->mbEqual = 0;

	}
	else {
		userData->mbEqual = memcmp(pObj, userData->mpOther, pObjDescription->mClassSize) == 0;
	}
	return MetaOpResult::eMetaOp_Fail;
}

METAOP_FUNC_IMPL(GetObjectName) {
	static String szConstObjName("T3 Object");
	String* pDestStr = static_cast<String*>(pUserData);
	if (pDestStr)
		pDestStr->operator=(szConstObjName.c_str());
	return MetaOpResult::eMetaOp_Succeed;
}

MetaSerializeAccel* MetaSerialize_GenerateAccel(MetaClassDescription* pObj) {
	if (!pObj->mpFirstMember)
		return NULL;
	if (pObj->mpSerializeAccel)return pObj->mpSerializeAccel;
	MetaMemberDescription* member = pObj->mpFirstMember;
	u64 toserialize = 0;
	do {
		bool dont = member->mFlags & MetaFlag::MetaFlag_MetaSerializeDisable || member->mpMemberDesc->mFlags.mFlags & MetaFlag_MetaSerializeDisable;
		member = member->mpNextMember;
		if (!dont)
			toserialize++;
	} while (member);
	if (toserialize > 0) {
		MetaSerializeAccel* accels = new MetaSerializeAccel[toserialize + 1i64];
		MetaSerializeAccel* first = accels;
		member = pObj->mpFirstMember;
		do {
			MetaClassDescription* memberDesc = member->mpMemberDesc;
			if (!(memberDesc->mFlags.mFlags & MetaFlag_MetaSerializeDisable) && !(member->mFlags & MetaFlag_MetaSerializeDisable)) {
				accels->mpMemberDesc = member;
				MetaOperation op = memberDesc->GetOperationSpecialization(74);
				accels->mpFunctionAsync = op ? op : Meta::MetaOperation_SerializeAsync;
				op = memberDesc->GetOperationSpecialization(75);
				accels->mpFunctionMain = op ? op : Meta::MetaOperation_SerializeMain;
				accels++;
			}
			member = member->mpNextMember;
		} while (member);
		//reason we have this trailing one is because its the null terminator. bit stupid they could just store a size to spare
		//them 20 or so bytes but telltale ok then
		first[toserialize].mpFunctionAsync = 0;
		first[toserialize].mpFunctionMain = 0;
		first[toserialize].mpMemberDesc = 0;
		pObj->mpSerializeAccel = first;
		return first;
	}
	return NULL;
}

METAOP_FUNC_IMPL(SerializeMain) {
	MetaSerializeAccel* accelerated = pObjDescription->mpSerializeAccel;
	if (accelerated || ((accelerated = MetaSerialize_GenerateAccel(pObjDescription)) != NULL)) {
		while (true) {
			if (!accelerated->mpFunctionMain)return eMetaOp_Succeed;//nothing to serialize
			MetaOpResult result = accelerated->mpFunctionMain(&((char*)pObj)[accelerated->mpMemberDesc->mOffset],
				accelerated->mpMemberDesc->mpMemberDesc, accelerated->mpMemberDesc, pUserData);
			if (result != eMetaOp_Succeed)return result;
			accelerated++;
			if (!accelerated)
				break;
		}
	}
	return eMetaOp_Succeed;//can still succeed since it could be memberless!
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncuint8) {
	static_cast<MetaStream*>(pUserData)->serialize_bytes(pObj, 1);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncuint16) {
	static_cast<MetaStream*>(pUserData)->serialize_uint16((u16*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncuint32) {
	static_cast<MetaStream*>(pUserData)->serialize_uint32((u32*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncuint64) {
	static_cast<MetaStream*>(pUserData)->serialize_uint64((u64*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncint8) {
	static_cast<MetaStream*>(pUserData)->serialize_bytes(pObj, 1);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncint16) {
	static_cast<MetaStream*>(pUserData)->serialize_uint16((u16*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncint32) {
	static_cast<MetaStream*>(pUserData)->serialize_uint32((u32*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncint64) {
	static_cast<MetaStream*>(pUserData)->serialize_uint64((u64*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncfloat) {
	static_cast<MetaStream*>(pUserData)->serialize_float((float*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncdouble) {
	static_cast<MetaStream*>(pUserData)->serialize_double((long double*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(ObjectStateIntrinsic1) {
	*(u32*)pUserData = CRC32(*(u32*)pUserData, (const char* const)pObj, 1);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(ObjectStateIntrinsicString) {
	*(u32*)pUserData = CRC32(*(u32*)pUserData, ((String*)pObj)->c_str(), ((String*)pObj)->size());
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(EquivalenceIntrinsic1)
{
	static_cast<Meta::Equivalence*>(pUserData)->mbEqual = (u8*)pObj == (u8*)static_cast<Meta::Equivalence*>(pUserData)->mpOther;
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(EquivalenceIntrinsic2) {
	static_cast<Meta::Equivalence*>(pUserData)->mbEqual = (u16*)pObj == (u16*)static_cast<Meta::Equivalence*>(pUserData)->mpOther;
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(EquivalenceIntrinsic4)
{
	static_cast<Meta::Equivalence*>(pUserData)->mbEqual = (u32*)pObj == (u32*)static_cast<Meta::Equivalence*>(pUserData)->mpOther;
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(EquivalenceIntrinsic8) {
	static_cast<Meta::Equivalence*>(pUserData)->mbEqual = (u64*)pObj == (u64*)static_cast<Meta::Equivalence*>(pUserData)->mpOther;
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(EquivalenceIntrinsicString) {
	Meta::Equivalence* e = static_cast<Meta::Equivalence*>(pUserData);
	String* s1 = (String*)pObj;
	String* s2 = (String*)e->mpOther;
	e->mbEqual = s1->size() == s2->size() && !memcmp(s1->c_str(), s2->c_str(), s1->size());
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(ObjectStateIntrinsic2) {
	*(u32*)pUserData = CRC32(*(u32*)pUserData, (const char* const)pObj, 2);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(ObjectStateIntrinsic4) {
	*(u32*)pUserData = CRC32(*(u32*)pUserData, (const char* const)pObj, 4);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(ObjectStateIntrinsic8) {
	*(u32*)pUserData = CRC32(*(u32*)pUserData, (const char* const)pObj, 8);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncbool) {
	static_cast<MetaStream*>(pUserData)->serialize_bool((bool*)pObj);
	return eMetaOp_Succeed;
}

METAOP_FUNC_IMPL__(SerializeIntrinsicAsyncString) {
	static_cast<MetaStream*>(pUserData)->serialize_String((String*)pObj);
	return eMetaOp_Succeed;
}

bool Meta_IsMemberDisabled(MetaMemberDescription* member, i32 streamVersion){
	bool disable = member->mFlags & 1 || member->mpMemberDesc && member->mpMemberDesc->mFlags.mFlags & 1;
	if (!disable && member->mMinMetaVersion != -1 && member->mMinMetaVersion > streamVersion)
		disable = true;
	if (member->mGameIndexVersionRange.min != -1 && member->mGameIndexVersionRange.max != -1) {
		if (member->mGameIndexVersionRange.max >= member->mGameIndexVersionRange.min) {
			disable = !(sSetKeyIndex >= member->mGameIndexVersionRange.min &&
				member->mGameIndexVersionRange.max >= sSetKeyIndex || sSetKeyIndex == member->mGameIndexVersionRange.min
				|| sSetKeyIndex == member->mGameIndexVersionRange.max);
		}
		else {
			disable = !(sSetKeyIndex >= member->mGameIndexVersionRange.min || member->mGameIndexVersionRange.max >= sSetKeyIndex);
		}
	}
	else {
		if (!disable && member->mGameIndexVersionRange.min != -1)
			disable = !(sSetKeyIndex >= member->mGameIndexVersionRange.min);
		if (!disable && member->mGameIndexVersionRange.max != -1)
			disable = !(member->mGameIndexVersionRange.max >= sSetKeyIndex);
	}
	if (!disable && member->mSkipVersion != -1)
		disable = sSetKeyIndex == member->mSkipVersion;
	return disable;
}

//pObj = object to serialize, pUserData = metastream
MetaOpResult Meta::MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDescription, MetaMemberDescription* pCtx,void* pUserData) {
	if (!pObjDescription)return eMetaOp_Fail;
	if (pObjDescription->mFlags.mFlags & (int)MetaFlag::MetaFlag_MetaSerializeDisable ||
		pCtx && pCtx->mFlags & (int)MetaFlag::MetaFlag_MetaSerializeDisable)
		return eMetaOp_Invalid;
	MetaStream* stream = static_cast<MetaStream*>(pUserData);
	if(pObjDescription->mbTypeUnfinished){
		TelltaleToolLib_RaiseError("Trying to serialize an unfinished type! It is vital you report this and the file you encounter to me! Discord: lucassaragosa#0152, check github!", ErrorSeverity::ERR);
		return eMetaOp_Fail;
	}
	if (pCtx && pCtx->mpName) {
		stream->BeginObject(pCtx->mpName, false);
	}
	else stream->BeginObject(pObjDescription->mpTypeInfoName,false);
	if (stream->mMode == MetaStreamMode::eMetaStream_Write) {
		SerializedVersionInfo* ver = SerializedVersionInfo::RetrieveCompiledVersionInfo(pObjDescription);
		if (ver && !pObjDescription->mbIsIntrinsic) {
			//stream->AddVersion(ver);//we dont realy need this
		}
		MetaSerializeAccel* accel = pObjDescription->mpSerializeAccel;
		if (!accel)accel = MetaSerialize_GenerateAccel(pObjDescription);
		if (accel && accel->mpFunctionAsync) {
			int i = 0;
			bool blocked = false;
			while (true) {
				if (!accel[i].mpFunctionAsync)break;
				MetaSerializeAccel a = accel[i];
				bool allow = !Meta_IsMemberDisabled(a.mpMemberDesc, stream->mStreamVersion);
				if (allow && (!(a.mpMemberDesc->mMinMetaVersion != -1 && a.mpMemberDesc->mMinMetaVersion > stream->mStreamVersion))) {
					if (a.mpMemberDesc->mFlags & (int)MetaFlag::MetaFlag_MetaSerializeBlockingDisabled
						|| a.mpMemberDesc->mpMemberDesc->mFlags.mFlags & (int)MetaFlag::MetaFlag_MetaSerializeBlockingDisabled) {
						blocked = false;
					}
					else {
						stream->BeginBlock();
						blocked = true;
					}
					stream->BeginObject(a.mpMemberDesc->mpName, false);
					//if (a.mpMemberDesc->mpIntrinsicKeyOverride)
						//stream->Key(a.mpMemberDesc->mpIntrinsicKeyOverride);
					MetaOpResult result = a.mpFunctionAsync(((char*)pObj) + a.mpMemberDesc->mOffset,
						a.mpMemberDesc->mpMemberDesc, a.mpMemberDesc, pUserData);
					stream->EndObject(a.mpMemberDesc->mpName);
					if (blocked)
						stream->EndBlock();
					if (result != MetaOpResult::eMetaOp_Succeed)break;
				}
				i++;
			}
		}
		if (pCtx && pCtx->mpName) {
			stream->EndObject(pCtx->mpName);
		}
		else stream->EndObject(pObjDescription->mpTypeInfoName);
		return eMetaOp_Succeed;
	}
	//if (!(stream->mRuntimeFlags.mFlags & (int)MetaStream::RuntimeFlags::eStreamIsCompiledVersion)) {}
	SerializedVersionInfo* ver = SerializedVersionInfo::RetrieveCompiledVersionInfo(pObjDescription);
	if (!ver)return eMetaOp_Fail;
	for (int i = 0; i < ver->mMembers.size(); i++) {
		SerializedVersionInfo::MemberDesc desc = ver->mMembers[i];
		MetaMemberDescription* member = desc.mpMemberDesc;
		bool disable = Meta_IsMemberDisabled(member, stream->mStreamVersion);
		if (disable) {
			//printf("DISABLE %s::%s\n", pObjDescription->mpTypeInfoName, member->mpName);
			continue;
		}
		if (!member)break;
		if (desc.mbBlocked)
			stream->BeginBlock();
		MetaOperation serasync = member->mpMemberDesc->GetOperationSpecialization(74);
		MetaOpResult r;
		if (serasync) {
			r = serasync(((char*)pObj) + member->mOffset, member->mpMemberDesc, member, pUserData);
		}
		else {
			r = MetaOperation_SerializeAsync(((char*)pObj) + member->mOffset, member->mpMemberDesc, member, pUserData);
		}
		if (r != eMetaOp_Succeed) {
			TelltaleToolLib_RaiseError(pObjDescription->mpTypeInfoExternalName, ERR);
			return r;
		}
		if (desc.mbBlocked) {
			stream->EndBlock();
		}
	}
	if (pCtx && pCtx->mpName) {
		stream->EndObject(pCtx->mpName);
	}
	else stream->EndObject("Object");
	return eMetaOp_Succeed;
}

MetaOpResult PerformMetaSerializeFull(MetaStream* pStream, void* pObj, MetaClassDescription* pDesc) {
	if (!pStream || !pDesc)return eMetaOp_Fail;
	MetaOperation async;// , main;
	async = pDesc->GetOperationSpecialization(74);
	//main = pDesc->GetOperationSpecialization(75);
	if (!async)async = Meta::MetaOperation_SerializeAsync;
	//if (!pStream->mSection[MetaStream::eSection_Default].mpStream) {
	//	TelltaleToolLib_RaiseError("Cannot perform meta serialize: meta stream was invalid (corrupt?)", ErrorSeverity::ERR);
	//	return eMetaOp_Fail;
	//}
	//if (!main)main = Meta::MetaOperation_SerializeMain;
	if(pDesc->mpExt&&std::find(pStream->mSerializedTypes.begin(), pStream->mSerializedTypes.end(), pDesc) == pStream->mSerializedTypes.end())
		pStream->mSerializedTypes.push_back(pDesc);
	MetaOpResult result = async(pObj, pDesc, NULL, pStream);
	//if (result == eMetaOp_Succeed)result = main(pObj, pDesc, NULL, pStream);
	return result;
}

METAOP_FUNC_IMPL(AsyncSave) {
	return PerformMetaSerializeFull(static_cast<MetaStream*>(pUserData), pObj, pObjDescription);
}

METAOP_FUNC_IMPL(SerializedVersionInfo) {
	//if pObj, warn its not needed in the call
	SerializedVersionInfo* ver = static_cast<SerializedVersionInfo*>(pUserData);
	if (ver) {
		if (pObjDescription->mFlags.mFlags & MetaFlag::MetaFlag_MetaSerializeDisable ||
			pContextDescription && pContextDescription->mFlags & MetaFlag::MetaFlag_MetaSerializeDisable)
			return MetaOpResult::eMetaOp_Invalid;//if its not serialized no need for this
		ver->mTypeSymbolCrc = pObjDescription->mHash;
		ver->mSize = pObjDescription->mClassSize;
		char hashBuffer[8];
		ver->mbBlocked = (pObjDescription->mFlags.mFlags & (int)MetaFlag_MetaSerializeBlockingDisabled) == 0;
		u32 blockedv = ver->mbBlocked ? 0xFFFF'FFFF : 0;
		ver->mVersionCrc = CRC32(0, (const char*)&blockedv, 4);
		for (MetaMemberDescription* i = pObjDescription->mpFirstMember; i; i = i->mpNextMember) {
			if (i->mFlags & 1)continue;//flag 1 = metaflags:: dont seralized etc
			SerializedVersionInfo::MemberDesc member;
			member.mpMemberDesc = i;
			member.mSize = i->mpMemberDesc->mClassSize;
			member.mbBlocked = true;
			member.mTypeNameSymbolCrc = i->mpMemberDesc->mHash;
			member.mVersionCrc = SerializedVersionInfo::RetrieveCompiledVersionInfo(i->mpMemberDesc)->mVersionCrc;
			//member.mbBlocked = !i->mpMemberDesc->mbIsIntrinsic;
			if (((bool)(i->mpMemberDesc->mFlags.mFlags & (int)MetaFlag_MetaSerializeBlockingDisabled))) {
				member.mbBlocked = false;
			}
			if (((bool)(i->mFlags & (int)MetaFlag_MetaSerializeBlockingDisabled))) {
				member.mbBlocked = false;
			}
			member.mName = String(i->mpName);
			member.mTypeName = String(i->mpMemberDesc->mpTypeInfoName);
			ver->mVersionCrc = CRC32(ver->mVersionCrc, member.mName.c_str(),
				member.mName.length());
			memcpy(hashBuffer, &i->mpMemberDesc->mHash, 8);
			ver->mVersionCrc = CRC32(ver->mVersionCrc, hashBuffer, 8);
			memcpy(hashBuffer, &member.mbBlocked, 1);
			ver->mVersionCrc = CRC32(ver->mVersionCrc, hashBuffer, 1);
			ver->mMembers.push_back(member);
		}
	}
	return MetaOpResult::eMetaOp_Succeed;
}

String FindSymbolName(const Symbol& m, HashDatabase::Page*& outpageref) {
	String _NF = "<NotFound:";
	static const String _E = "<Empty>";
	if (m.GetCRC() == 0)return _E;
	HashDatabase* db = TelltaleToolLib_GetGlobalHashDatabase();
	if (!db) {
		_NF += m.CRCAsCstr();
		_NF += ">";
		return _NF;
	}
	HashDatabase::Page* page = NULL;
	String pagen = "Files_";
	const std::vector<const char*>* extensions = &sMetaFileExtensions;
	String ret;
	String gameid = sBlowfishKeys[sSetKeyIndex].game_id;
	if (outpageref) {
		page = outpageref;
		db->FindEntry(page, m.GetCRC(), &ret);
		if (ret.size()) {
			return ret;
		}
	}
	else {
		for (int i = 0; i < extensions->size(); i++) {
			pagen += extensions->operator[](i);
			pagen += '_';
			std::transform(gameid.begin(), gameid.end(), gameid.begin(), ::toupper);
			pagen += gameid;
			page = db->FindPage(pagen.c_str());
			if (!page) {
				pagen = "Files_";
				continue;
			}
			db->FindEntry(page, m.GetCRC(), &ret);
			if (ret.size()) {
				outpageref = page;
				return ret;
			}
			pagen = "Files_";
		}
	}
	_NF += m.CRCAsCstr();
	_NF += ">";
	return _NF;
}

const std::vector<const char*>* GetMetaFileExtensions() {
	return &sMetaFileExtensions;
}