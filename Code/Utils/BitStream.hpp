#pragma once

#include <vector>
#include "Utils/ByteImpl.hpp"
#include "Utils/Memory.hpp"
#include "Utils/Uuid.hpp"

class BitStream
{
	std::vector<Byte> data;
	int index = 0;

public:
	BitStream(MemoryWrapper& memory)
	{
		const std::size_t chunk_size = memory.Size() - memory.Index();
		data.resize(chunk_size);

		std::memcpy(data.data(), memory.Data() + memory.Index(), chunk_size);
	}

	int Index() const
	{
		return this->index;
	}

	int Size() const
	{
		return (int)this->data.size();
	}

	void Align()
	{
		index += (7 - ((index - 1) & 7));
	}

	void Move(const int& step)
	{
		index += step;
	}

	const Byte* Data() const
	{
		return this->data.data();
	}

	long long ReadNBytes(const int& count)
	{
		if (Size() < (index >> 3) + count) return 0;

		long long mem_off = (index >> 3);
		int offset = (index & 7);

		long long result = 0;
		index += count * 8;

		if (offset == 0)
		{
			for (long long i = 0; i < count; i++)
			{
				const std::size_t mem_off_full = (std::size_t)(mem_off + i);

				result |= (((long long)data[mem_off_full]) & 0xff) << ((count - i - 1ll) << 3ll);
			}

			return result;
		}

		for (long long i = 0; i < count; i++)
		{
			const std::size_t mem_off_full = (std::size_t)(mem_off + i);

			long a = ((((long)data[mem_off_full    ] & 0xff) << (    offset))) & 0xff;
			long b = ((((long)data[mem_off_full + 1] & 0xff) >> (8 - offset))) & 0xff;

			long long c = (a | b) & 0xff;

			result |= (c << ((count - i - 1ll) << 3ll));
		}

		return result;
	}

	union int_to_float_bits
	{
		int integer_bits;
		float converted_float_bits;
	};

	float ReadFloat()
	{
		union int_to_float_bits bits = {};
		bits.integer_bits = (int)ReadNBytes(4);

		return bits.converted_float_bits;
	}

	int ReadInt()
	{
		return (int)ReadNBytes(4);
	}

	int ReadShort()
	{
		return (int)ReadNBytes(2);
	}

	int ReadByte()
	{
		return (int)ReadNBytes(1);
	}

	SMUuid ReadUuid()
	{
		const long long first = ReadNBytes(8);
		const long long second = ReadNBytes(8);

		return SMUuid(second, first, true);
	}

	std::vector<Byte> ReadBytes(const int& length)
	{
		std::vector<Byte> bytes;
		bytes.resize(length);

		std::memcpy(bytes.data(), data.data() + (index >> 3), length);
		index += length * 8;

		return bytes;
	}

	std::string ReadString(const int& length)
	{
		std::vector<Byte> bytes = ReadBytes(length);

		return std::string(bytes.begin(), bytes.end());
	}
};