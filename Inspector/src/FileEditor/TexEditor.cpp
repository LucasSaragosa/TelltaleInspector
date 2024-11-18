#include "../TelltaleInspector.h"
#include "../stb/std_image_write.h"
#include "../squish/squish.h"
#include "../imstd/imgui_stdlib.h"

inline u32 rotate_left(u32 i, u32 bits) {
	return (i << bits) | (i >> (32 - bits));
}

inline u32 fix_pixel(u32 argb) {
	u32 red = (argb >> 16) & 0xff;
	u32 blue = (argb >> 0) & 0xff;
	u32 green = (argb >> 8) & 0xff;
	u32 alpha = (argb >> 24) & 0xff;
	return (alpha << 24) | (blue << 16) | (green << 8) | (red << 0);
}

bool convert_to_png(T3Texture& texture, std::string outpng, std::string& err) {
	DataStream* pTexels = texture.GetTxData(0, 0);
	if (!pTexels) {
		err = "Could not get texture data from texture!!";
		return false;
	}
	pTexels->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
	char* texbuf = (char*)malloc(pTexels->GetSize());
	if (!pTexels->Serialize(texbuf, pTexels->GetSize())) {
		free(texbuf);
		err = "Could not read texture data";
		return false;
	}

	if (texture.mSurfaceFormat == eSurface_RGBA8)
		;//all ok
	else if (texture.mSurfaceFormat == eSurface_ARGB8) {
		//flip alpha byte
		int num = (texture.mWidth * texture.mHeight);
		u32* raw = (u32*)texbuf;
		for (int i = 0; i < num; i++)
			raw[i] = fix_pixel(raw[i]);
	}
	else if (texture.mSurfaceFormat == eSurface_DXT1) {
		char* dest = (char*)malloc(texture.mWidth * texture.mHeight * 4);
		squish::DecompressImage((squish::u8*)dest, texture.mWidth, texture.mHeight, texbuf, squish::kDxt1);
		free(texbuf);
		texbuf = dest;
	}
	else if (texture.mSurfaceFormat == eSurface_DXT3) {
		char* dest = (char*)malloc(texture.mWidth * texture.mHeight * 4);
		squish::DecompressImage((squish::u8*)dest, texture.mWidth, texture.mHeight, texbuf, squish::kDxt3);
		free(texbuf);
		texbuf = dest;
	}
	else if (texture.mSurfaceFormat == eSurface_DXT5) {
		char* dest = (char*)malloc(texture.mWidth * texture.mHeight * 4);
		squish::DecompressImage((squish::u8*)dest, texture.mWidth, texture.mHeight, texbuf, squish::kDxt5);
		free(texbuf);
		texbuf = dest;
	}
	else {
		free(texbuf);
		err = std::string{ "Cannot convert the current surface format as of yet! => " } + std::to_string((int)texture.mSurfaceFormat);
		return false;
	}

	if (stbi_write_png(outpng.c_str(), texture.mWidth, texture.mHeight, 4, texbuf, 4 * texture.mWidth) == 0) {
		err = "Could not write PNG file to destination file";
		free(texbuf);
		return false;
	}
	free(texbuf);
	return true;
}

void TextureTask::_render() {
	if (id == 0) {
		ImGui::Text("Select game texture belongs to");
		id = select_gameid_dropdown(id);
		return;
	}
	if (loaded) {
		TelltaleToolLib_SetBlowfishKey(id);
		ImGui::InputText("Texture Name", &mpTexture->mName);
		ImGui::InputText("Texture Import Name", &mpTexture->mImportName);
		MetaMemberDescription* formatDesc = pTextureMCD->GetMemberDescription("mSurfaceFormat");
		texfmt = GetMetaEnumName(formatDesc, mpTexture->mSurfaceFormat);
		ImGui::Text("Note: Changing this does not change actual internal pixel format !!");
		if (ImGui::BeginCombo("Texture Format", texfmt)) {
			MetaEnumDescription* pEnum = formatDesc->mpEnumDescriptions;
			while (pEnum) {
				if (ImGui::Selectable(pEnum->mpEnumName)) {
					ImGui::SetItemDefaultFocus();
					mpTexture->mSurfaceFormat = (T3SurfaceFormat)pEnum->mEnumIntValue;
				}
				pEnum = pEnum->mpNext;
			}
			ImGui::EndCombo();
		}

	}
	else {
		if (!ok)
			ImGui::Text((std::string{ "Error opening last file: " } + err).c_str());
		nfdchar_t* outp{ 0 };
		if (ImGui::Button("Open Texture File") && NFD_OKAY == NFD_OpenDialog("d3dtx", 0, &outp)) {
			std::string fp = outp;
			free(outp);
			TelltaleToolLib_SetBlowfishKey(id);
			if (!mStream.Attach(_OpenDataStreamFromDisc(fp.c_str(), READ), MetaStreamMode::eMetaStream_Read, {})) {
				err = "Error opening meta stream file, are you sure this is a telltale file?";
				ok = false;
				return;
			}
			if (mpTexture)
				delete mpTexture;
			mpTexture = new T3Texture;
			if (PerformMetaSerializeAsync(&mStream, mpTexture) != eMetaOp_Succeed) {
				err = "Error opening the file as a texture, are you sure the game is correct and its a texture?";
				ok = false;
				return;
			}
			ok = loaded = true;
			err = "";
		}
	}
	nfdchar_t* outp{ 0 };
	if (ImGui::Button("Convert Whole Folder") && NFD_OKAY == NFD_PickFolder(0, &outp, L"Select folder containg D3DTX files")) {
		std::string texFolder = outp;
		free(outp);
		std::string pngFolder = texFolder + "/Converted/";
		std::vector<std::string> failed_files{};
		TelltaleToolLib_SetBlowfishKey(id);
		std::filesystem::create_directories(pngFolder);
		for (auto& file : std::filesystem::directory_iterator{ std::filesystem::path{texFolder} }) {
			if (file.is_regular_file() && ends_with(file.path().filename().string(), ".d3dtx")) {
				if (mpTexture)
					delete mpTexture;
				mpTexture = new T3Texture;
				if (!mStream.Attach(_OpenDataStreamFromDisc(file.path().string().c_str(), READ), MetaStreamMode::eMetaStream_Read, {})) {
					failed_files.push_back(file.path().filename().string() + " => Could not open meta stream");
					continue;
				}
				if (PerformMetaSerializeAsync(&mStream, mpTexture) != eMetaOp_Succeed) {
					failed_files.push_back(file.path().filename().string() + " => Could not open meta stream as texture, incorrect game?");
					continue;
				}
				std::string pngf = pngFolder + file.path().filename().string();
				pngf = pngf.substr(0, pngf.find_last_of('.')) + ".png";
				if (!convert_to_png(*mpTexture, pngf, err)) {
					failed_files.push_back(file.path().filename().string() + " => Error converting to PNG => " + err);
					continue;
				}
				TTL_Log("convert success: %s\n", file.path().filename().string().c_str());
			}
		}
		if (failed_files.size() > 0)
			TTL_Log("Failed to convert the following textures:\n");
		for (auto& failed : failed_files) {
			TTL_Log("-%s\n", failed.c_str());
		}
		MessageBoxA(0, "All files were converted! Check console for any ones that had problems.", "Finished!", MB_ICONINFORMATION);
	}
	outp = 0;
	if (loaded && ok && ImGui::Button("Convert to PNG") && NFD_OKAY == NFD_SaveDialog("png", 0, &outp, L"Select output file")) {
		std::string outpng = outp;
		if (!ends_with(outpng, ".png"))
			outpng += ".png";
		if (convert_to_png(*mpTexture, outpng, err)) {
			MessageBoxA(0, "The texture was successfully converted to PNG!", "Success!", MB_ICONINFORMATION);
		}
		else {
			MessageBoxA(0, err.c_str(), "Error converting texture to PNG", MB_ICONERROR);
		}
	}
}