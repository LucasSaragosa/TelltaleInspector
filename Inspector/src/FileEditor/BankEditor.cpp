#include "../TelltaleInspector.h"
#include <imgui.h>
#include "../nfd.h"
#include "ToolLibrary/FMOD/FSB5.h"

bool BankTask::getter(void* data, int index, const char** output)
{
	*output = ((FMODSample*)data)[index].mName.c_str();
	return true;
}

bool BankTask::sample_sorter(const FMODSample& lhs, const FMODSample& rhs) {
	std::less<std::string> l{};
	return l(lhs.mName, rhs.mName);
}

void BankTask::_render() {
	static bool strip{};
	ImGui::Checkbox("Strip Audio File Name UUIDs", &strip);
	mBank.SetStripNameUUIDs(strip);
	ImGui::Text("Audio Files");
	ImGui::ListBox("##audio", &selectedfile, &getter, (void*)mBank.GetSamples().data(), (u32)mBank.GetSamples().size());
	if (ImGui::Button("Override all & Import New Bank")) {
		nfdchar_t* outPath{};
		nfdresult_t result = NFD_OpenDialog("bank;fsb5", nullptr, &outPath);
		if (result == NFD_OKAY) {
			DataStreamFileDisc* pBankStream = OpenDataStreamFromDisc(outPath, READ);
			if (!pBankStream || pBankStream->IsInvalid()) {
				MessageBoxA(0, "Could not open the file!", "Error", MB_ICONERROR);
			}
			else {
				mBank.Reset();
				std::string error{};
				if (!mBank.Open(pBankStream, error)) {
					MessageBoxA(0, error.c_str(), "Could not open FMOD sound bank!", MB_ICONERROR);
				}
				else {
					std::sort(mBank.GetSamples().begin(), mBank.GetSamples().end(), &sample_sorter);
					selectedfile = 0;
				}
			}
		}
	}
	if (ImGui::Button("Extract Selected") && mBank.GetSamples().size() > 0) {
		nfdchar_t* outp{};
		if (NFD_PickFolder(0, &outp, L"Select Destination Folder") == NFD_OKAY) {
			std::filesystem::path folder{ outp };
			folder += "/";
			folder += mBank.GetSamples()[selectedfile].mName;
			DataStreamFileDisc out = _OpenDataStreamFromDisc_(folder.string().c_str(), WRITE);
			if (out.IsInvalid()) {
				MessageBoxA(0, "Could not open the file!", "Error", MB_ICONERROR);
			}
			else {
				std::string err{};
				DataStreamMemory* result = mBank.MakeAudioFile(selectedfile, err);
				if (!result) {
					MessageBoxA(0, err.c_str(), "Error extracting ogg vorbis", 0);
					return;
				}
				result->SetMode(DataStreamMode::eMode_Read);
				result->Transfer(&out, 0, result->GetSize());
				delete result;
				MessageBoxA(0, "Successfully extracted the audio file!", "Success", MB_ICONINFORMATION);
			}
		}

	}
	if (ImGui::Button("Extract All")) {
		nfdchar_t* outp{};
		if (NFD_PickFolder(0, &outp, L"Select Destination Folder") == NFD_OKAY) {
			int eerr = 0;
			std::filesystem::path folder{ outp };
			folder += "/";
			std::stringstream errs{};
			errs << "The following files could not be extracted:\n";
			std::string err{};
			for (int i = 0; i < mBank.GetSamples().size(); i++) {
				std::filesystem::path file = folder;
				file += mBank.GetSamples()[i].mName;
				DataStreamFileDisc out = _OpenDataStreamFromDisc_(file.string().c_str(), WRITE);
				if (out.IsInvalid()) {
					eerr++;
					errs << mBank.GetSamples()[i].mName << ": Could not open file on disk";
					continue;
				}
				DataStreamMemory* result = mBank.MakeAudioFile(i, err);
				if (!result) {
					eerr++;
					errs << mBank.GetSamples()[i].mName << ": " << err << "\n";
					continue;
				}
				result->SetMode(DataStreamMode::eMode_Read);
				result->Transfer(&out, 0, result->GetSize());
				delete result;
			}
			if (eerr > 0) {
				MessageBoxA(0, errs.str().c_str(), "Error extracting some of the audio files", MB_ICONERROR);
			}
			else
				MessageBoxA(0, "Successfully extracted all the audio files!", "Success", MB_ICONINFORMATION);
		}
	}
	if (ImGui::Button("Reset and Clear Files")) {
		mBank.Reset();
	}
}