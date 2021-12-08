#pragma once

#include <algorithm>
#include <vector>

#include "Console.hpp"
#include "SMReaders/ByteImpl.hpp"

class MemoryWrapper
{
	Byte* bytes;
	std::size_t index = 0;

public:
	MemoryWrapper() = default;
	MemoryWrapper(const MemoryWrapper&) = delete;

	MemoryWrapper(Byte* bytes)
	{
		this->bytes = bytes;
	}

	void operator=(Byte* bytes)
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

	template<typename ObjectType>
	ObjectType Object(const std::size_t& offset)
	{
		constexpr const std::size_t type_sz = sizeof(ObjectType);

		std::vector<Byte> bytes_copy = {};
		bytes_copy.resize(type_sz);

		std::memcpy(bytes_copy.data(), this->bytes + offset, type_sz);

		return *(ObjectType*)bytes_copy.data();
	}

	template<typename ObjectType>
	ObjectType NextObject()
	{
		constexpr const std::size_t obj_sz = sizeof(ObjectType);

		std::vector<Byte> bytes_copy = {};
		bytes_copy.resize(obj_sz);

		std::memcpy(bytes_copy.data(), this->bytes + this->index, obj_sz);
		this->index += obj_sz;

		return *(ObjectType*)bytes_copy.data();
	}

	template<typename ObjectType>
	std::vector<ObjectType> Objects(const std::size_t& offset, const std::size_t& amount)
	{
		std::vector<ObjectType> obj_copy;
		obj_copy.resize(amount);

		std::memcpy(obj_copy.data(), this->bytes + offset, sizeof(ObjectType) * amount);

		return obj_copy;
	}

	template<typename ObjectType>
	std::vector<ObjectType> NextObjects(const std::size_t& amount)
	{
		const std::size_t byte_amount = sizeof(ObjectType) * amount;

		std::vector<ObjectType> obj_copy;
		obj_copy.resize(amount);

		std::memcpy(obj_copy.data(), this->bytes + this->index, byte_amount);
		this->index += byte_amount;

		return obj_copy;
	}
};