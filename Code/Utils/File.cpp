#include "File.hpp"

#include <fstream>

#include <filesystem>
namespace fs = std::filesystem;

std::vector<Byte> File::ReadFileBytes(const std::wstring& path)
{
	std::ifstream input_file(path, std::ios::binary);
	std::vector<Byte> file_bytes = {};

	if (input_file.is_open())
	{
		input_file.seekg(0, std::ios::end);
		const std::size_t file_size = (std::size_t)input_file.tellg();
		input_file.seekg(0, std::ios::beg);

		file_bytes.resize(file_size);
		input_file.read((char*)file_bytes.data(), file_size);

		input_file.close();
	}

	return file_bytes;
}

std::string File::ReadToString(const std::wstring& path)
{
	std::ifstream input_file(path, std::ios::binary);
	std::string file_string;

	if (input_file.is_open())
	{
		input_file.seekg(0, std::ios::end);
		const std::size_t file_size = (std::size_t)input_file.tellg();
		input_file.seekg(0, std::ios::beg);

		file_string.resize(file_size);
		input_file.read(file_string.data(), file_size);

		input_file.close();
	}

	return file_string;
}

bool File::Exists(const std::wstring& path)
{
	std::error_code ec;
	bool exists = fs::exists(path, ec);

	return (!ec && exists);
}

bool File::CreateDirectorySafe(const std::wstring& path)
{
	std::error_code e_error;
	bool exists = fs::exists(path, e_error);
	bool exists_correct = (!e_error && exists);

	std::error_code f_error;
	bool file_created = fs::create_directory(path, f_error);
	bool file_correct = (!f_error && file_created);

	return (exists_correct || file_correct);
}

std::wstring File::OpenFileDialog(
	const std::wstring& title,
	FILEOPENDIALOGOPTIONS options,
	LPCWSTR filter,
	HWND owner
) {
	std::wstring _Output = L"";

	HRESULT hr = CoInitializeEx(NULL, COINIT_DISABLE_OLE1DDE | COINIT_APARTMENTTHREADED);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;

		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			pFileOpen->SetOptions(options);
			pFileOpen->SetTitle(title.c_str());
			hr = pFileOpen->Show(owner);

			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						_Output = std::wstring(pszFilePath);
						CoTaskMemFree(pszFilePath);
					}

					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}

	return _Output;
}