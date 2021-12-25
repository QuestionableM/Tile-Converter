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

bool File::Exists(const std::wstring& path)
{
	std::error_code ec;
	bool exists = fs::exists(path, ec);

	return (!ec && exists);
}