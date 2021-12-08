#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "SMReaders/ByteImpl.hpp"

class FileStream
{
	std::ifstream* iStream = nullptr;
	std::size_t iSize;

public:
	inline bool isOpen()
	{
		return iStream->is_open();
	}

	inline void Skip(const std::size_t& bytes)
	{
		iStream->seekg(bytes + iStream->tellg());
	}

	inline std::size_t GetSize()
	{
		return iSize;
	}

	inline std::size_t GetIndex()
	{
		return (std::size_t)iStream->tellg();
	}


	template<typename ObjType>
	inline ObjType NextObject()
	{
		constexpr const std::size_t type_sz = sizeof(ObjType);

		std::vector<std::byte> byte_copy = {};
		byte_copy.resize(type_sz);

		iStream->read((char*)byte_copy.data(), type_sz);

		return *(ObjType*)byte_copy.data();
	}

	template<typename ObjType>
	inline std::vector<Byte> NextObject(const std::size_t& count)
	{
		constexpr const std::size_t type_sz = sizeof(ObjType);
		const std::size_t full_size = type_sz * count;

		std::vector<Byte> bytes_copy = {};
		bytes_copy.resize(full_size);

		iStream->read((char*)bytes_copy.data(), full_size);

		return bytes_copy;
	}



	FileStream(const std::wstring& file)
	{
		iStream = new std::ifstream(file, std::ios::binary);

		if (isOpen())
		{
			iStream->seekg(0, std::ios::end);
			iSize = (std::size_t)iStream->tellg();
			iStream->seekg(0, std::ios::beg);
		}
	}

	~FileStream()
	{
		if (iStream->is_open())
			iStream->close();

		delete iStream;
	}
};