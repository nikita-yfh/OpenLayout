#pragma once
#include <stdio.h>
#include <stdint.h>
#include <math.h>

class File{
public:
	File(const char *path, const char *mode);
	~File();

	bool IsOk() const;

	template<typename T>
	void Write(const T &value) {
		fwrite(&value,sizeof(T),1,file);
	}
	template<typename T>
	T Read() {
		T value;
		fread(&value,sizeof(T),1,file);
		return value;
	}

	template<typename T>
	File &operator<<(const T &value) {
		Write(value);
		return *this;
	}
	template<typename T>
	File &operator>>(T &value) {
		value = Read<T>();
		return *this;
	}

	template<typename T>
	float ReadAngle() {
		return Read<T>() / 1000.0f / 180.0f * M_PI;
	}
	template<typename T>
	void WriteAngle(float value) {
		Write<T>(value / M_PI * 180.0f * 1000.0f);
	}

	template<typename T>
	float ReadMm() {
		return Read<T>() / 10000.0f;
	}
	template<typename T>
	void WriteMm(float value) {
		Write<T>(value * 10000.0f);
	}

	inline void Write(const void *ptr, size_t size);
	inline void Read(void *ptr, size_t size);

	void WriteString(const char *value, uint8_t n);
	void ReadString(char *value, uint8_t n);

	void WriteString(const char *value);
	void ReadString(char *value);
	File &operator<<(const char *value) {
		WriteString(value);
		return *this;
	}
	File &operator>>(char *value) {
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

inline void File::Write(const void *ptr, size_t size) {
	fwrite(ptr, size, 1, file);
}
inline void File::Read(void *ptr, size_t size) {
	fread(ptr, size, 1, file);
}
