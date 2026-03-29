#pragma once
#pragma warning (disable: 4996)
#include "print.h"
#include "break.h"
namespace Utility
{
	class BinaryWriter
	{
	public:
		BinaryWriter(char* filepath) : path(filepath), file(nullptr) 
		{
			file = fopen(path.c_str(), "wb");
		};
		BinaryWriter(const std::string& filepath) : path(filepath), file(nullptr) 
		{
			file = fopen(path.c_str(), "wb");
		};

		~BinaryWriter()
		{
			if(IsGood())
				fclose(file);
		}
		BinaryWriter(const BinaryWriter&) = delete;
		BinaryWriter(BinaryWriter&& rhs) noexcept
		{
			file = rhs.file;
			path = rhs.path;
			rhs.path = "";
			rhs.file = nullptr;
		}

		//value types (e.g: int)
		template<typename T, std::enable_if_t<!std::is_void_v<T>, int> = 0 >
		void Write(const T& val)
		{
			constexpr size_t dataSize = sizeof(T);
			fwrite(&val, dataSize, 1, file);
		}

		template<typename T, std::enable_if_t<std::is_pointer_v<T>, int> = 0 >
		void Write(T ptr, size_t numElements)
		{
			constexpr size_t dataSize = sizeof(std::remove_pointer_t<T>);
			fwrite(ptr, dataSize, numElements, file);
		}

		//array types (e.g: char[4])
		template<typename T, size_t numElements, std::enable_if_t<std::is_array_v<T>, int> = 0 >
		void Write(T(&source)[numElements])
		{
			constexpr size_t dataSize = sizeof(T) * numElements;
			fwrite(source, dataSize, 1, file);
		}

		//pointer type (e.g: char*)
		template<typename T, size_t numElements, typename U, std::enable_if_t<std::is_pointer_v<U> && !std::is_array_v<U>, int> = 0>
		void Write(U source)
		{
			constexpr size_t dataSize = sizeof(std::remove_pointer_t<U>);
			fwrite(source, dataSize, numElements, file);
		}

		bool IsGood() const { return file != nullptr; }
		void Close() { if (IsGood()) fclose(file), file = nullptr; }
		std::string path;
		FILE* file;
	};

}