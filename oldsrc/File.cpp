#include "File.h"
#include <string.h>

File::File(const char *path, const char *mode) {
	file = fopen(path, mode);
}
File::~File() {
	if(IsOk())
		fclose(file);
}
bool File::IsOk() const{
	return file != nullptr;
}
void File::WriteString(const char *str, uint8_t n) {
	Write<uint8_t>(strlen(str));
	fwrite(str, n, 1, file);
}
void File::ReadString(char *str, uint8_t n) {
	uint8_t len = Read<uint8_t>();
	fread(str, n, 1, file);
	str[len] = '\0';
}
void File::WriteString(const char *str) {
	uint32_t len = strlen(str);
	Write<uint32_t>(len);
	fwrite(str, len, 1, file);
}
void File::ReadString(char *str) {
	uint32_t len = Read<uint32_t>();
	fread(str, len, 1, file);
}
void File::WriteNull(size_t num) {
	uint8_t null = 0x00;
	for(int i = 0; i < num; i++)
		putc(null, file);
}
void File::ReadNull(size_t num) {
	for(int i = 0; i < num; i++)
		fgetc(file);
}

