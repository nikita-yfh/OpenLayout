#pragma once
#include <stdio.h>
#include <stdint.h>

class File{
public:
	File(const char *path, const char *mode);
	~File();

	bool IsOk() const;

	template<typename T>
	void Write(const T &value){
		fwrite(&value,sizeof(T),1,file);
	}
	template<typename T>
	T Read(){
		T value;
		fread(&value,sizeof(T),1,file);
		return value;
	}
	template<typename T>
	File &operator<<(const T &value){
		Write(value);
		return *this;
	}
	template<typename T>
	File &operator>>(T &value){
		value = Read<T>();
		return *this;
	}

	template<typename T>
	T ReadMm(){
		return Read<T>() / 10000.0f;
	}
	template<typename T>
	void WriteMm(const T &value){
		Write<T>(value);
	}

	void Write(const void *ptr, size_t size);
	void Read(void *ptr, size_t size);

	void WriteString(const char *value, uint8_t n);
	void ReadString(char *value, uint8_t n);

	void WriteString(const char *value);
	void ReadString(char *value);
	File &operator<<(const char *value){
		WriteString(value);
		return *this;
	}
	File &operator>>(char *value){
		ReadString(value);
		return *this;
	}

	void WriteNull(size_t num);
	void ReadNull(size_t num);
private:
	FILE *file;

	File(const File&) = delete;
	void operator=(const File&) = delete;
};
