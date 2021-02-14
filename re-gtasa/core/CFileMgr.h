#pragma once

#include <cstdio>

class CFileMgr
{
public:
	static void Initialize();
	static void ChangeDir(const char* dir);
	static void SetDir(const char* dir);
	static void SetDirMyDocuments();

	static size_t LoadFile(const char* filename, char* dest, int size, const char* mode);
	static FILE* OpenFile(const char* filename, const char* mode);
	static FILE* OpenFileForWriting(const char* filename);
	static FILE* OpenFileForAppending(const char* filename);
	static size_t Read(FILE* file, char* dest, size_t size);
	static size_t Write(FILE* file, const char* src, size_t size);
	static bool Seek(FILE* file, long offset, int origin);
	static bool ReadLine(FILE* file, char* dest, int num);
	static void CloseFile(FILE* file);

	static int GetFileLength(FILE* file);
	static int Tell(FILE* file);
	static int GetErrorReadWrite(FILE* file);

private:
	static GtaArr<char, 0xB71A60, 128> ms_dirName;
	static GtaArr<char, 0xB71AE0, 128> ms_rootDirName;
};