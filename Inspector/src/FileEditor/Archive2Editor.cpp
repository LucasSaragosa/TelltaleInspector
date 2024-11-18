#include "../TelltaleInspector.h"
#include <imgui.h>
#include "../nfd.h"
#include "ToolLibrary/TTArchive2.hpp"

void ArchiveTask::import_in_singlefile(const std::filesystem::path& phys, exitstrat& strat) {
	filet inst{ std::move(phys.filename().string()), phys.string() };
	for (auto& f : file_names) {
		if (iequals(f.file, inst.file)) {
			std::stringstream tmp{};
			tmp << "Found a file name conflict. Replace(yes), ignore(no), cancel(select what I do for any more conflicts)? ";
			tmp << inst.file;
			int res = MessageBoxA(0, tmp.str().c_str(), "Name conflict", MB_ICONINFORMATION | MB_YESNOCANCEL);
			if (res == IDCANCEL) {
				if (MessageBoxA(0, "Replace all (yes), ignore all (no)", "Select", MB_ICONINFORMATION | MB_YESNO) == IDYES)
					strat = accept_all;
				else
					strat = ignore_all;
				res = strat == accept_all ? IDYES : IDNO;
			}
			if (res == IDYES) {
				f.path = phys.string();
				return;
			}
			continue;
		}
	}
	file_names.push_back(inst);
}

void ArchiveTask::_render() {
	ImGui::Text("Archive Game:");
	selected_game_id = select_gameid_dropdown(selected_game_id);
	ImGui::Checkbox("Zlib Compressed", &bCompressedZ);
	ImGui::Checkbox("Oodle Compressed", &bCompressedOodle);
	ImGui::Checkbox("Encrypted", &bEncrypted);
	ImGui::Text("Version should be 0 (TTA2), 1 (TTA3) or 2 (TTA4). Any is OK, but TTA4 is used in most games.");
	ImGui::InputInt("Version", (int*)&arch.mVersion);
	if (arch.mVersion > 2 || arch.mVersion < 0)
		arch.mVersion = 0;
	if (bState) {
		ImGui::Text("Files");
		ImGui::ListBox("##Files_B", &selectedfile, &getter, (void**)file_names.data(), (int)file_names.size());
		if (ImGui::Button("Add File(s)")) {
			MessageBoxA(NULL, "Please note: you can add directories full of files, but directories in selected directories"
				" wont be imported, only the files in the selected ones!", "Information", MB_ICONINFORMATION);
			nfdpathset_t paths{};
			nfdresult_t result = NFD_OpenDialogMultiple(0, 0, &paths);
			if (result == NFD_OKAY) {
				exitstrat strat = exitstrat::ask;
				int num = NFD_PathSet_GetCount(&paths);
				file_names.reserve(file_names.size() + num);
				for (int i = 0; i < num; i++) {
					nfdchar_t* path = NFD_PathSet_GetPath(&paths, i);
					std::filesystem::path phys{ path };
					if (std::filesystem::is_directory(phys)) {
						std::filesystem::directory_iterator iter{ phys };
						for (auto& it : iter) {
							if (std::filesystem::is_regular_file(phys))
								import_in_singlefile(it, strat);
						}
					}
					else if (std::filesystem::is_regular_file(phys)) {
						import_in_singlefile(phys, strat);
					}
					else {
						MessageBoxA(NULL, path, "File/Folder Invalid, not including!", MB_ICONERROR);
						continue;
					}
				}
				std::sort(file_names.begin(), file_names.end(), &filet_sorter);
			}
		}
		if (ImGui::Button("Extract Selected") && file_names.size() > 0) {
			nfdchar_t* outp{};
			if (NFD_PickFolder(0, &outp, L"Select Destination Folder") == NFD_OKAY) {
				std::filesystem::path folder{ outp };
				folder += "/";
				if (file_names[selectedfile].path.size() > 0) {
					std::filesystem::path pth{ file_names[selectedfile].path };
					//just copy
					folder += std::filesystem::path{ file_names[selectedfile].path }.filename().string();
					if (!std::filesystem::copy_file(pth, folder)) {
						MessageBoxA(0, "Could not extract (copy over) the file!", "Error", MB_ICONERROR);
					}
					else {
						MessageBoxA(0, "Successfully extracted (copied) the file!", "Success", MB_ICONINFORMATION);
					}
				}
				else {
					folder += file_names[selectedfile].file;
					DataStreamFileDisc out = _OpenDataStreamFromDisc_(folder.string().c_str(), WRITE);
					bool f = false;
					{
						u64 crc = CRC64_CaseInsensitive(0, file_names[selectedfile].file.c_str());
						for (auto& it : arch.mResources) {
							if (it.mNameCRC == crc) {
								f = true;
								DataStream* pStream = arch.GetResourceStream(&it);
								if (pStream) {
									pStream->Transfer(&out, 0, pStream->GetSize());
								}
								else f = false;
								break;
							}
						}
					}
					if (f) {
						MessageBoxA(0, "Successfully extracted the file!", "Success", MB_ICONINFORMATION);
					}
					else {
						MessageBoxA(0, "There was a problem extracting this file. Contact me!", "Error", MB_ICONERROR);
					}
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
				for (int i = 0; i < file_names.size(); i++) {
					if (file_names[i].path.size() > 0) {
						std::filesystem::path pth{ file_names[i].path };
						//just copy
						std::filesystem::path op{ folder };
						op += std::filesystem::path{ file_names[i].path }.filename().string();
						if (!std::filesystem::copy_file(pth, op)) {
							eerr++;
							err = "Could not copy";
							;								errs << std::filesystem::path{ file_names[i].path }.filename().string() << ": " << err << "\n";
						}

					}
					else {
						std::filesystem::path file = folder;
						std::string& fname = file_names[i].file;
						file += fname;
						DataStreamFileDisc out = _OpenDataStreamFromDisc_(file.string().c_str(), WRITE);
						bool f = false;
						{
							u64 crc = CRC64_CaseInsensitive(0, fname.c_str());
							for (auto& it : arch.mResources) {
								if (it.mNameCRC == crc) {
									f = true;
									DataStream* pStream = arch.GetResourceStream(&it);
									if (pStream) {
										pStream->Transfer(&out, 0, pStream->GetSize());
									}
									else f = false;
									break;
								}
							}
						}
						if (!f) {
							eerr++;
							err = "Could not extract";
							errs << fname << ": " << err << "\n";
						}
					}
				}
				if (eerr > 0) {
					MessageBoxA(0, errs.str().c_str(), "Error extracting some of the files", MB_ICONERROR);
				}
				else
					MessageBoxA(0, "Successfully extracted all the files!", "Success", MB_ICONINFORMATION);
			}
		}
		if (ImGui::Button("Remove Selected File")) {
			if (file_names.size() > 0)
				file_names.erase(file_names.begin() + selectedfile);
			selectedfile = 0;
		}
		if (ImGui::Button("Export Archive")) {
			if (file_names.size() == 0)
				return;
			if (bCompressedOodle && bCompressedZ) {
				MessageBoxA(0, "You can only have one compression type!", "!!", MB_ICONHAND);
				return;
			}
			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_SaveDialog("ttarch2", 0, &outPath);
			if (result == NFD_OKAY) {
				if (!(sRuntime.settings.mFlags.mFlags & eISF_ShownTTArchMessage)) {
					MessageBoxA(0, "You are about to create an archive. Ensure the compression/encryption"
						" is set, game is set and know that this may take time (~10s to 3-5mins).", "Create", MB_ICONINFORMATION);
					sRuntime.settings.mFlags |= eISF_ShownTTArchMessage;
				}
				int numinvalid = 0;
				std::string op = outPath;
				if (ends_with(op, ".ttarch2"))
					op += ".ttarch2";
				DataStreamFileDisc dst = _OpenDataStreamFromDisc_(op.c_str(), WRITE);
				if (dst.IsInvalid())
					return;
				TTL_Log("opening file streams...\n");
				std::vector<TTArchive2::ResourceCreateEntry> entries{};
				entries.reserve(file_names.size());
				for (auto& file : file_names) {
					TTArchive2::ResourceCreateEntry entry{};
					entry.name = file.file;
					if (file.path.size() == 0) {//from other ttarch
						entry.mpStream = new DataStreamSubStream(arch.mpResourceStream, (unsigned __int64)file.sz, file.off);
					}
					else {
						std::filesystem::path fspath{ file.path };
						if (std::filesystem::exists(fspath) && std::filesystem::is_regular_file(fspath)) {
							entry.mpStream = _OpenDataStreamFromDisc(fspath.string().c_str(), READ);
							if (!entry.mpStream || entry.mpStream->IsInvalid()) {
								goto invalid_file;
							}
						}
						else {
						invalid_file:
							numinvalid++;
							continue;
						}
					}
					entries.push_back(_STD move(entry));
				}
				TTL_Log("creating archive...\n");
				bool result = TTArchive2::Create(0, &dst, entries, bEncrypted, bCompressedOodle || bCompressedZ,
					bCompressedOodle ? Compression::Library::OODLE : Compression::Library::ZLIB, arch.mVersion);
				if (!result) {
					MessageBoxA(0, "There was a problem creating the archive. Please contact me! (info tab)", "Error creating archive!", MB_ICONERROR);
					return;
				}
				MessageBoxA(0, "Successfully created the archive!", "Success!", MB_ICONINFORMATION);
			}
		}
	}
	else {
		ImGui::Text("Click open archive to import all files from an existing archive. You can only add one archive, and after that you can add files and directories from your filesystem.");
		ImGui::Text("Click new archive to not import an existing archive, but create your own from scratch.");
		if (ImGui::Button("Open Archive") && selected_game_id) {
			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_OpenDialog("ttarch2", NULL, &outPath);
			if (result == NFD_OKAY) {
				TelltaleToolLib_SetBlowfishKey(selected_game_id);
				MessageBoxA(NULL, "Attempting to load archive, may take a while (click okay)", "Archive Info", MB_ICONINFORMATION);
				std::filesystem::path inpath{ outPath };
				task_name = inpath.filename().string();
				DataStreamFileDisc* strm = _OpenDataStreamFromDisc(outPath, READ);
				arch.Activate(strm);
				if (!arch.mbActive) {
					MessageBoxA(NULL, "Could not open archive! It may be open in another app. (Check its from the right game!)", "Error loading TTArchive", MB_ICONERROR);
				}
				else {
					DataStreamContainer* pContainer = (DataStreamContainer*)arch.mpInStream;
					bEncrypted = pContainer->IsEncrypted();
					if (pContainer->IsCompressed()) {
						bCompressedOodle = pContainer->GetCompressionLibrary() == Compression::Library::OODLE;
						bCompressedZ = pContainer->GetCompressionLibrary() == Compression::Library::ZLIB;
					}
					else
						bCompressedZ = bCompressedOodle = false;
					bState = true;
					String name{};
					file_names.reserve(arch.mResources.size());
					for (auto& resource : arch.mResources) {
						name = arch.GetResourceName(resource.mNameCRC);
						file_names.push_back(filet{ std::move(name), {}, resource.mOffset, resource.mSize });
					}
					std::sort(file_names.begin(), file_names.end(), &filet_sorter);
					//DONE!
				}
			}
		}
		else if (ImGui::Button("New Archive") && selected_game_id) {
			bState = true;
			arch.mVersion = 2;
		}
	}
}