#pragma once

#include "print.h"
#include "break.h"
#include <vector>
namespace Utility
{
	class MemoryWriter
	{
	public:
		MemoryWriter() {};
		MemoryWriter(size_t buffersize) 
		{
			Reserve(buffersize);
		};

		MemoryWriter(const MemoryWriter&) = delete;
		MemoryWriter(MemoryWriter&& rhs)
		{
			buffer = rhs.buffer;
			rhs.buffer.clear();
		}

		//value types (e.g: int)
		template<typename T, std::enable_if_t<!std::is_void_v<T>, int> = 0 >
		void Write(const T& val)
		{
			constexpr size_t dataSize = sizeof(T);
			size_t initialSize = buffer.size();
			buffer.resize(buffer.size() + dataSize);
			memcpy(buffer.data() + initialSize, &val, dataSize);
		}

		template<typename T, std::enable_if_t<std::is_pointer_v<T>, int> = 0 >
		void Write(T ptr, size_t numElements)
		{
			constexpr size_t dataSize = sizeof(std::remove_pointer_t<T>);
			size_t initialSize = buffer.size();
			buffer.resize(buffer.size() + dataSize * numElements);
			memcpy(buffer.data() + initialSize, ptr, dataSize * numElements);
		}

		//array types (e.g: char[4])
		template<typename T, size_t numElements, std::enable_if_t<std::is_array_v<T>, int> = 0 >
		void Write(T(&source)[numElements])
		{
			constexpr size_t dataSize = sizeof(T) * numElements;
			size_t initialSize = buffer.size();
			buffer.resize(buffer.size() + dataSize);
			memcpy(buffer.data() + initialSize, source, dataSize);
		}

		//pointer type (e.g: char*)
		template<typename T, size_t numElements, typename U, std::enable_if_t<std::is_pointer_v<U> && !std::is_array_v<U>, int> = 0>
		void Write(U source)
		{
			constexpr size_t dataSize = sizeof(std::remove_pointer_t<U>);
			size_t initialSize = buffer.size();
			buffer.resize(buffer.size() + dataSize * numElements);
			memcpy(buffer.data() + initialSize, &source, dataSize * numElements);
		}

		void Reserve(size_t size)
		{
			buffer.reserve(size);
		}

		void Clear()
		{
			buffer.clear();
		}

		void ShrinkToFit()
		{
			buffer.shrink_to_fit();
		}

		const char* GetData() const { return buffer.data(); }
		size_t GetSize() const { return buffer.size(); }

		std::vector<char> buffer;
	};

}