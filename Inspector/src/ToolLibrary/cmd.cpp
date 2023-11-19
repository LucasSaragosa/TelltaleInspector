#include "TelltaleToolLibrary.h"
#include "MetaStream_JSON.hpp"
#include "TTArchive2.hpp"
#include "Shader/T3EffectCache.h"
#include "Base64.h"

#include <iostream>
#include <vector>
#include <Windows.h>
#include <Shlwapi.h>

const char* Get() {
	static char buf[1024];
	std::cin >> buf;
	return buf;
}

void Help() {
	printf("\nAll game IDs:\n\n");
	for (int i = 0; i < KEY_COUNT; i++) {
		printf("- %s\n", sBlowfishKeys[i].game_id);
	}
	printf("\n\tUsage: TelltaleModTool.exe <GAME ID> <mode> "
		"<input file/folder> \n\t\t<output file/folder> [options]: Available modes:\n\t\t\t-to_json\n\t\t\t-to_bin\n\t\t\t"
		"-create_arch\n\t\t\t-extract_arch\n\t\t\t-extract_shader\n\t\t\t-extract_container");
	printf("\n\n\tTo JSON exports a telltale file (any file which isnt .ttarch2/.t3fxpack/.lua/.lenc) to a modifyable text format\n"
		"\tTo BIN exports a JSON created with to_json back into telltale binary format. Use this once your file has been modified.\n"
		"\tCreate Arch makes a .ttarch2 from the given input dir. Available (can use multiple, not both compressions) options are: "
		"\n\t\t-encrypt, -compress, -compress_oodle\n"
		"\tExtract Arch extracts a .ttarch2 into an output folder\n"
		"\tExtract Shader extracts a .t3fxpack to JSON\n"
		"\tExtract Container extracts any contained data stream (uncompressed/unencrypts). Useful for modders working with archives/shaders/saves\n");
}

#define flag_InputFile 1
#define flag_OutputFile 2

enum toolops {
	encrypt = 1,
	compressnormal = 2,
	compressoodle = 4,

};

enum ptype {
	folder = 1,
	file = 2,
	invalid = 0,
};

ptype ffType(const char* p) {
	if (!PathFileExistsA(p))
		return ptype::invalid;
	DWORD attrib = GetFileAttributesA(p);
	if (attrib & FILE_ATTRIBUTE_DIRECTORY)
		return ptype::folder;
	return ptype::file;
}

enum mode {
	JSON = 8 | flag_InputFile | flag_OutputFile,
	BIN = 16 | flag_InputFile | flag_OutputFile,
	C_ARCH = 32 | flag_OutputFile,
	E_ARCH = 64 | flag_InputFile,
	IMPORT_SHADER = 128 | flag_InputFile | flag_OutputFile,
	CONTAINER_EXTRACT = 256 | flag_InputFile | flag_OutputFile,
};

void ttarchProgress(const char* m, float f) {
	printf("\t%s: %f\n", m, f);
}

bool bInit = false;

void DoApp(std::vector<std::string>& args) {
	printf("Telltale Modding Tools - Command Line Version: By Lucas Saragosa for all Telltale Games from and including TWAU\n");
	printf("\tLibrary Version " _VERSION "\n");
	printf("\n\n** NOTE THIS IS A WORK IN PROGRESS TOOL **\n\n");
	if (args.size() <= 3) {
		Help();
		printf("\n\tNot enough arguments\n");
		return;
	}
	unsigned int ops{};
	std::string& game = args[0];
	if (!TelltaleToolLib_Initialize(game.c_str())) {
		Help();
		printf("\n\tYou entered an invalid game id.\n");
		return;
	}
	bInit = true;
	DataStreamFileDisc* db = _OpenDataStreamFromDisc("ToolLibrary.HashDB", READ);
	if(db->IsInvalid()){
		delete db;
		printf("\n\tCould not locate the hash database (ToolLibrary.HashDB). Ensure the installation of this tool is correct!\n");
		return;
	}
	TelltaleToolLib_SetGlobalHashDatabaseFromStream(db);
	std::string& mod = args[1];
	mode m;
	if (mod == "to_json")
		m = mode::JSON;
	else if (mod == "to_bin")
		m = mode::BIN;
	else if (mod == "create_arch")
		m = mode::C_ARCH;
	else if (mod == "extract_arch")
		m = mode::E_ARCH;
	else if (mod == "extract_shader")
		m = mode::IMPORT_SHADER;
	else if (mod == "extract_container")
		m = mode::CONTAINER_EXTRACT;
	else {
		Help();
		printf("\n\tYou entered an invalid mode (see the help above)\n");
		return;
	}
	if (args[2].length() && (args[2].at(args[2].length() - 1) == '/' || args[2].at(args[2].length() - 1) == '\\')) {
		printf("\n\tOne or more paths contain a slash at the end, please remove these\n");
		return;
	}
	if (args[3].length() && (args[3].at(args[3].length() - 1) == '/' || args[3].at(args[3].length() - 1) == '\\')) {
		printf("\n\tOne or more paths contain a slash at the end, please remove these\n");
		return;
	}
	ptype inputState = ffType(args[2].c_str());
	ptype outputState = ffType(args[3].c_str());
	if (inputState == ptype::invalid) {
		printf("\n\tInput file/folder does not exist: %s\n", args[2].c_str());
		return;
	}
	if (!(m & flag_OutputFile) && outputState != ptype::folder) {
		printf("\n\tOutput directory does not exist\n");
		return;
	}
	if ((m & flag_InputFile) && inputState != ptype::file) {
		printf("\n\tInput file is a directory, not a file\n");
		return;
	}
	if (!(m & flag_InputFile) && inputState != ptype::folder) {
		printf("\n\tInput directory is a file, not a directory (folder)\n");
		return;
	}
	for (int i = 4; i < args.size(); i++) {
		std::string& op = args[i];
		if (op == "-encrypt")
			ops |= toolops::encrypt;
		else if (op == "-compress")
			ops |= toolops::compressnormal;
		else if (op == "-compress_oodle")
			ops |= toolops::compressoodle;
		else {
			Help();
			printf("\n\tUnknown option '%s'\n", op.c_str());
			return;
		}
	}
	printf("\n");
	if (m == mode::C_ARCH) {
		if (_setmaxstdio(8192) == -1) {
			printf("\n\tCould not set max stdio limit, please contact me with your machine information\n");
			return;
		}
		TTArchive2 archive{};
		std::vector<TTArchive2::ResourceCreateEntry> files{};
		WIN32_FIND_DATAA fd{};
		std::string fpp = args[2] + "/*";
		HANDLE h = FindFirstFileA(fpp.c_str(), &fd);
		int i = 1;
		do {
			if (fd.cFileName[0] == '.')
				continue;
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;
			if (i >= 8100) {
				for (int x = 0; x < files.size(); x++)
					delete files[x].mpStream;//close
				printf("\n\tYou are trying to pack too many files! Please ensure you are using less than 8100 files in one archive\n");
				return;
			}
			TTArchive2::ResourceCreateEntry entry{};
			entry.name = fd.cFileName;
			std::string fullPath = (args[2] + "/") + entry.name;
			entry.mpStream = _OpenDataStreamFromDisc(fullPath.c_str(), READ);
			i++;
			files.push_back(_STD move(entry));
		} while (FindNextFileA(h, &fd));
		FindClose(h);
		DataStreamFileDisc dst = _OpenDataStreamFromDisc_(args[3].c_str(), WRITE);
		archive.Create(&ttarchProgress, &dst, &files[0], (int)files.size(), (ops & toolops::encrypt) ? true : false,
			((ops & toolops::compressnormal) || (ops & toolops::compressoodle)) ? true : false,
			(ops & toolops::compressoodle) ? Compression::Library::OODLE : Compression::Library::ZLIB,
			TelltaleToolLib_GetGameKeyIndex(game.c_str()) >= 46 ? 2 : 1);
		printf("\n\tCreated the archive successfully!\n");
	}
	else if (m == mode::E_ARCH) {
		TTArchive2 arch{};
		DataStreamFileDisc* strm = _OpenDataStreamFromDisc(args[2].c_str(), READ);
		arch.Activate(strm);
		printf("\n\tExtracting files...\n");
		String result{};
		std::string outFolder = args[3] + "/";
		for (auto it = arch.mResources.begin(); it != arch.mResources.end(); it++) {
			arch.GetResourceName(it->mNameCRC, &result);
			DataStream* resourceStream = arch.GetResourceStream(&(*it));
			std::string filepath = outFolder + result;
			DataStreamFileDisc to = _OpenDataStreamFromDisc_(filepath.c_str(), WRITE);
			resourceStream->Transfer(&to, 0, resourceStream->GetSize());
			delete resourceStream;
			printf("\t- %s\n", result.c_str());
		}
		printf("\n\tExtracted all files successfully!\n");
	}
	else if (m == mode::JSON) {
		std::string fileExt = args[2].substr(args[2].find_last_of(".") + 1);
		MetaClassDescription* pType = TelltaleToolLib_FindMetaClassDescription(fileExt.c_str(), false);
		if (!pType) {
			printf("\n\tThe file extension is not valid or supported yet. Try files such as .prop, .d3dmesh etc\n");
			return;
		}
		MetaStream inStream{};
		void* pInstance = pType->New();
		if (!pInstance) {
			printf("\n\tCould not allocate\n");
			return;
		}
		printf("\n\tReading from %s [%s]...\n\n", args[2].c_str(), pType->mpTypeInfoName);
		inStream.Open(_OpenDataStreamFromDisc(args[2].c_str(), READ), MetaStreamMode::eMetaStream_Read, {});
		MetaOpResult r = PerformMetaSerializeFull(&inStream, pInstance, pType);
		if (r != eMetaOp_Succeed) {
			printf("\n\tThis file could not be read fully. Please contact me so I can fix this!\n");
			return;
		}
		printf("\n\tWriting JSON to %s...\n", args[3].c_str());
		MetaStream_JSON outStream{MetaStream_JSON::eJSONMode::eCompleteMetaStream};
		outStream.InjectVersionInfo(inStream);
		outStream.Open(_OpenDataStreamFromDisc(args[3].c_str(), WRITE), MetaStreamMode::eMetaStream_Write, {});
		r = PerformMetaSerializeFull(&outStream, pInstance, pType);
		if (r != eMetaOp_Succeed) {
			printf("\n\tThis file could not be written into JSON fully. Please contact me so I can fix this!\n");
			return;
		}
		outStream.Close();
		pType->Delete(pInstance);
		printf("\n\tExported JSON successfully!\n");
	}
	else if (m == mode::BIN) {
		MetaStream_JSON inStream{MetaStream_JSON::eJSONMode::eCompleteMetaStream};
		//inStream.Open(_OpenDataStreamFromDisc(args[2].c_str(), READ), MetaStreamMode::eMetaStream_Read, {});
		printf("WIP\n");
	}
	else if (m == mode::IMPORT_SHADER) {
		std::string fileExt = args[2].substr(args[2].find_last_of(".") + 1);
		if (fileExt != "t3fxpack") {
			printf("Invalid shader file. If it is in an archive (eg XXX_Shaders.ttarch2) extract and then put that file here\n");
			//return;
		}
		DataStreamFileDisc* rawContained = _OpenDataStreamFromDisc(args[2].c_str(), READ);
		DataStreamContainerParams params{};
		params.mpSrcStream = rawContained;//take ownership
		DataStreamContainer effectPackage{ params };
		unsigned __int64 size = rawContained->GetSize();
		effectPackage.Read(0, &size);

		printf("\tLoading T3EffectPackage (T3EffectCache) %s [0x%llX bytes]: please wait...\n", args[2].c_str(), size);
		T3EffectCachePackage package{};

		if (!T3EffectCacheInternal::LoadPackage(package, effectPackage)) {
			printf("\tCould not load package. Maybe its corrupt (or its version is too old)? Please contact me and send the file!\n");
			return;
		}
		printf("\tLoaded the effect package. Writing to disk...\n");
		printf("\n\tInformation:\n\t\tPrograms: %d\n\t\tShaders: %d\n\t\tVersion: %d\n", 
			package.mHeader.mProgramCount, package.mHeader.mShaderCount, package.mHeader.mVersion);

		unsigned int biggestShaderSize = 0;
		for (int i = 0; i < package.mHeader.mShaderCount; i++) {
			if (package.mShaders[i].mSize > biggestShaderSize)
				biggestShaderSize = package.mShaders[i].mSize;
		}

		if (biggestShaderSize >= size) {
			printf("\tShader is too large, perhaps its corrupt? Contact me.\n");
			return;
		}

		char* shaderBuffer = new char[biggestShaderSize];

		printf("\nWriting Header...\n");
		MetaStream_JSON::JSONWriter writer{};
		writer.mWriter.StartObject();
		writer.mWriter.Key("Version");
		writer.mWriter.Int(package.mHeader.mVersion);
		writer.mWriter.Key("Shader Count");
		writer.mWriter.Int(package.mHeader.mShaderCount);
		writer.mWriter.Key("Program Count");
		writer.mWriter.Int(package.mHeader.mProgramCount);
		writer.mWriter.Key("Programs");
		writer.mWriter.StartArray();
		printf("\nWriting Programs...\n");
		for (int i = 0; i < package.mHeader.mProgramCount; i++) {
			break;//
			T3EffectCachePackageProgram* pProgram = package.mPrograms.mpStorage + i;
			writer.mWriter.StartObject();
			writer.mWriter.Key("Effect Type");
			writer.mWriter.Int((int)pProgram->mEffectType);
			writer.mWriter.Key("Material CRC");
			writer.mWriter.Uint64(pProgram->mMaterialCrc);
			writer.mWriter.Key("Effect Quality");
			writer.mWriter.Int((int)pProgram->mEffectQuality);
			writer.mWriter.Key("Pass Count");
			writer.mWriter.Int((int)pProgram->mPassCount);
			writer.mWriter.Key("Sampler Count");
			writer.mWriter.Int((int)pProgram->mSamplerCount);
			writer.mWriter.Key("Effect Features");
			writer.mWriter.StartArray();
			for (int i = 0; i < decltype(pProgram->mEffectFeatures)::EnumValCount; i++) {
				if (pProgram->mEffectFeatures[(T3EffectFeature)i])
					writer.mWriter.String(T3EffectCacheInternal::GetFeatureName((T3EffectFeature)i));
			}
			writer.mWriter.EndArray();
			writer.mWriter.EndObject();
		}
		writer.mWriter.EndArray();

		printf("\nWriting Shaders...\n");
		writer.mWriter.Key("Shaders");
		writer.mWriter.StartArray();
		double percentage = 0.0;
		for (int i = 0; i < package.mHeader.mShaderCount; i++) {
			percentage = (double)i / package.mHeader.mShaderCount * 100;
			T3EffectCachePackageShader* pShader = package.mShaders.mpStorage + i;
			writer.mWriter.StartObject();
			writer.mWriter.Key("Shader Type");
			writer.mWriter.String(T3EffectCacheInternal::GetShaderTypeName(pShader->mShaderType));
			writer.mWriter.Key("Needed Parameters");
			writer.mWriter.StartArray();
			for (int i = 0; i < decltype(pShader->mNeededParameters)::EnumValCount; i++) {
				if (pShader->mNeededParameters[(T3EffectParameterType)i])
					writer.mWriter.String(T3EffectCacheInternal::GetParameterTypeName((T3EffectParameterType)i));
			}
			writer.mWriter.EndArray();
			T3EffectCacheInternal::LoadShaderPayload(package, *pShader, shaderBuffer, effectPackage);

			int shSize = pShader->mSize;
			int params{ 0 };
			params = *(int*)shaderBuffer;
			int headerSize = 8 + 7 * params;
			int pos = 8;
			if (pShader->mShaderType == eMaterialShader_Vertex) {
				headerSize += 8;
				pos += 8;
				writer.mWriter.Key("Vertex Attribute Set");
				unsigned long long set{ 0 };
				memcpy(((char*)&set), shaderBuffer + 8, 8);
				writer.mWriter.Uint64(set);
			}

			writer.mWriter.Key("Parameter Data");
			writer.mWriter.StartArray();
			shSize -= headerSize;
			for (int x = 0; x < params; x++) {
				unsigned long long param{ 0 };
				memcpy(((char*)&param), shaderBuffer + pos, 7);
				writer.mWriter.Uint64(param);
				pos += 7;
			}
			writer.mWriter.EndArray();

			writer.mWriter.Key("Shader Data");
			std::string base64 = websocketpp::base64_encode((unsigned char*)shaderBuffer + headerSize, (size_t)shSize);
			writer.mWriter.String(base64.c_str());
			writer.mWriter.EndObject();
			if ((i % 5) == 0)
				printf("\t%f%%...\n",percentage);
		}
		writer.mWriter.EndArray();

		delete[] shaderBuffer;

		DataStreamFileDisc outStream = _OpenDataStreamFromDisc_(args[3].c_str(), WRITE);
		outStream.SerializeWrite(writer.mBuffer.GetString(), writer.mBuffer.GetSize());

		printf("\nDone!\n");
	}
	else if (m == mode::CONTAINER_EXTRACT) {
		DataStreamFileDisc* rawContained = _OpenDataStreamFromDisc(args[2].c_str(), READ);
		DataStreamContainerParams params{};
		params.mpSrcStream = rawContained;//take ownership
		DataStreamContainer container{ params };
		unsigned long long x{};
		container.Read(0, &x);
		if (x == 0) {
			printf("\n\tThe container is invalid or corrupt. Only use this tool if you know what you're doing.\n");
			return;
		}
		int enc = container.IsEncrypted() ? 1 : 0;
		Compression::Library com = container.GetCompressionLibrary();
		const char* encr = enc ? "Blowfish" : "None";
		const char* compr = com == Compression::Library::OODLE ? "Oodle" : com == Compression::Library::ZLIB ? "Deflate" : "None";
		printf("\n\tContainer details:\n\t\t- Compression: %s\n\t\t- Encryption: %s\n\n\tCopying...", compr, encr);
		DataStreamFileDisc destFile = _OpenDataStreamFromDisc_(args[3].c_str(), WRITE);
		void* tempBuffer = malloc(x);
		if (!tempBuffer) {
			printf("Could not allocate space in memory for temporary file data! File too big.\n");
			return;
		}
		container.Serialize((char*)tempBuffer, x);
		destFile.SerializeWrite((char*)tempBuffer, x);
		free(tempBuffer);
		printf("\n\tDone!");
	}
	else {
		Help();
		printf("\n\tInvalid mode or not supported yet\n");
	}
	printf("\n\nThank you for using my tool! Made by Lucas Saragosa (instagram: lukassaragosa). Any problems, let me know on discord: lucassaragosa#0152\n");
}

int __main(int argc, char* argv[]) {
	if (argc < 1)
		return 2;//no!
	
	std::vector<std::string> argData{};
	argData.reserve(argc - 1);
	for (int i = 1; i < argc; i++) {
		argData.push_back(argv[i]);
	}

	DoApp(argData);

	if (bInit)
		TelltaleToolLib_Free(); 

	return 0;
}