#pragma once

#include <vector>
#include <algorithm>
#include "Utils/ByteImpl.hpp"

class MemoryWrapper
{
	std::vector<Byte> bytes;
	std::size_t index = 0;

public:
	MemoryWrapper() = default;
	MemoryWrapper(const MemoryWrapper&) = delete;

	MemoryWrapper(const std::vector<Byte>& bytes)
	{
		this->bytes = bytes;
	}

	void operator=(const std::vector<Byte>& bytes)
	{
		this->bytes = bytes;
	}

	MemoryWrapper& Skip(const std::size_t& count)
	{
		this->index += count;

		return *this;
	}

	MemoryWrapper& Set(const std::size_t& index)
	{
		this->index = index;

		return *this;
	}

	std::size_t Index() const
	{
		return this->index;
	}

	std::size_t Size() const
	{
		return this->bytes.size();
	}

	const Byte* Data() const
	{
		return this->bytes.data();
	}

	template<typename ObjectType>
	ObjectType Object(const std::size_t& offset, const bool& big_endian = false)
	{
		constexpr const std::size_t type_sz = sizeof(ObjectType);

		std::vector<Byte> bytes_copy = {};
		bytes_copy.resize(type_sz);

		std::memcpy(bytes_copy.data(), this->bytes.data() + offset, type_sz);

		if (big_endian)
		{
			std::reverse(bytes_copy.begin(), bytes_copy.end());
		}

		return *(ObjectType*)bytes_copy.data();
	}

	template<typename ObjectType>
	ObjectType NextObject(const bool& big_endian = false)
	{
		constexpr const std::size_t obj_sz = sizeof(ObjectType);

		std::vector<Byte> bytes_copy = {};
		bytes_copy.resize(obj_sz);

		std::memcpy(bytes_copy.data(), this->bytes.data() + this->index, obj_sz);
		this->index += obj_sz;

		if (big_endian)
		{
			std::reverse(bytes_copy.begin(), bytes_copy.end());
		}

		return *(ObjectType*)bytes_copy.data();
	}

	template<typename ObjectType>
	std::vector<ObjectType> Objects(const std::size_t& offset, const std::size_t& amount, const bool& big_endian = false)
	{
		std::vector<ObjectType> obj_copy;
		obj_copy.resize(amount);

		std::memcpy(obj_copy.data(), this->bytes.data() + offset, sizeof(ObjectType) * amount);

		if (big_endian)
		{
			std::reverse(obj_copy.begin(), obj_copy.end());
		}

		return obj_copy;
	}

	template<typename ObjectType>
	std::vector<ObjectType> NextObjects(const std::size_t& amount, const bool& big_endian = false)
	{
		const std::size_t byte_amount = sizeof(ObjectType) * amount;

		std::vector<ObjectType> obj_copy;
		obj_copy.resize(amount);

		std::memcpy(obj_copy.data(), this->bytes.data() + this->index, byte_amount);
		this->index += byte_amount;

		if (big_endian)
		{
			std::reverse(obj_copy.begin(), obj_copy.end());
		}

		return obj_copy;
	}
};