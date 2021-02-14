#include <direct.h>
#include <cstring>

#include "../utils/patcher.h"
#include "CFileMgr.h"

char* InitUserDirectories();

GtaArr<char, 0xB71A60, 128> CFileMgr::ms_dirName;
GtaArr<char, 0xB71AE0, 128> CFileMgr::ms_rootDirName;

// DONE
void CFileMgr::Initialize()
{
	_getcwd(ms_rootDirName.Get(), ms_rootDirName.SizeOf());
	strcat(ms_rootDirName.Get(), "\\");
}

// DONE
void CFileMgr::ChangeDir(const char* dir)
{
	if (dir[0] == '\\')
	{
		strcpy(ms_dirName.Get(), ms_rootDirName.Get());
		dir++;
	}

	if (dir[0] != '\0')
	{
		strcat(ms_dirName.Get(), dir);
		if (dir[strlen(dir) - 1] != '\\') {
			strcat(ms_dirName.Get(), "\\");
		}
	}

	_chdir(ms_dirName.Get());
}

// DONE
void CFileMgr::SetDir(const char* dir)
{
	strcpy(ms_dirName.Get(), ms_rootDirName.Get());

	if (dir[0] != '\0')
	{
		strcat(ms_dirName.Get(), dir);
		if (dir[strlen(dir) - 1] != '\\') {
			strcat(ms_dirName.Get(), "\\");
		}
	}

	_chdir(ms_dirName.Get());
}

// DONE
void CFileMgr::SetDirMyDocuments()
{
	strcpy(ms_dirName.Get(), InitUserDirectories());
	_chdir(ms_dirName.Get());
}

// ODNE
size_t CFileMgr::LoadFile(const char* filename, char* dest, int size, const char* mode)
{
	FILE* f = fopen(filename, mode);
	if (f == nullptr) {
		return -1;
	}

	// читаем блоками по 0x4000 байт
	size_t count = 0;
	while (true)
	{
		// unsigned -> signed
		int n = fread(&dest[count], 1, 0x4000, f);
		// ошибка чтения (в идеале использовать feof/ferror)
		if (n < 0) { 
			return -1;
		}

		count += n;
		// прочитан блок меньше 0x4000 (конец файла)
		if (n != 0x4000) {
			dest[count] = 0;
			fclose(f);
			return count;
		}
	}
}

// DONE
FILE* CFileMgr::OpenFile(const char* filename, const char* mode)
{
	return fopen(filename, mode);
}

// DONE
FILE* CFileMgr::OpenFileForWriting(const char* filename)
{
	return OpenFile(filename, "wb");
}

// DONE
FILE* CFileMgr::OpenFileForAppending(const char* filename)
{
	return OpenFile(filename, "a");
}

// DONE
size_t CFileMgr::Read(FILE* file, char* dest, size_t size)
{
	return fread(dest, 1, size, file);
}

// DONE
size_t CFileMgr::Write(FILE* file, const char* src, size_t size)
{
	return fwrite(src, 1, size, file);
}

// DONE
bool CFileMgr::Seek(FILE* file, long offset, int origin)
{
	return fseek(file, offset, origin) != NULL;
}

// DONE
bool CFileMgr::ReadLine(FILE* file, char* dest, int num)
{
	return fgets(dest, num, file) != NULL;
}

// DONE
void CFileMgr::CloseFile(FILE* file)
{
	fclose(file);
}

// TODO: RwOsGetFileInterface()
int CFileMgr::GetFileLength(FILE* file)
{
	// сохраняем текущую позицию в файле
	int savePos = ftell(file);
	// переходим в конец файла
	fseek(file, 0, SEEK_END);
	// получаем позицию конца файла
	int fileLength = ftell(file);
	// восстанавливаем позицию указателя
	fseek(file, savePos, SEEK_SET);

	return fileLength;
}

// TODO: RwOsGetFileInterface()
int CFileMgr::Tell(FILE* file)
{
	return ftell(file);
}

// TODO: не соответствует оригинальному коду
int CFileMgr::GetErrorReadWrite(FILE* file)
{
	return ferror(file);
}

STARTPATCHES
InjectHook(0x5386F0, &CFileMgr::Initialize, PATCH_JUMP);
InjectHook(0x538730, &CFileMgr::ChangeDir, PATCH_JUMP);
InjectHook(0x5387D0, &CFileMgr::SetDir, PATCH_JUMP);
InjectHook(0x538860, &CFileMgr::SetDirMyDocuments, PATCH_JUMP);
InjectHook(0x538890, &CFileMgr::LoadFile, PATCH_JUMP);
InjectHook(0x538900, &CFileMgr::OpenFile, PATCH_JUMP);
InjectHook(0x538910, &CFileMgr::OpenFileForWriting, PATCH_JUMP);
InjectHook(0x538930, &CFileMgr::OpenFileForAppending, PATCH_JUMP);
InjectHook(0x538950, &CFileMgr::Read, PATCH_JUMP);
InjectHook(0x538970, &CFileMgr::Write, PATCH_JUMP);
InjectHook(0x538990, &CFileMgr::Seek, PATCH_JUMP);
InjectHook(0x5389B0, &CFileMgr::ReadLine, PATCH_JUMP);
InjectHook(0x5389D0, &CFileMgr::CloseFile, PATCH_JUMP);
InjectHook(0x5389E0, &CFileMgr::GetFileLength, PATCH_JUMP);
InjectHook(0x538A20, &CFileMgr::Tell, PATCH_JUMP);
InjectHook(0x538A50, &CFileMgr::GetErrorReadWrite, PATCH_JUMP);
ENDPATCHES