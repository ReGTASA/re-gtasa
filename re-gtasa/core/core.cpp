#include <Windows.h>
#include <cstring>

#include "../main.h"
#include "../utils/patcher.h"

GtaArr<char, 0xC92368, 256> szGtaUserDirPath;
GtaArr<char, 0xC92268, 256> szGalleryDirPath;
GtaArr<char, 0xC92168, 256> szUserTracksDirPath;

// DONE
char* InitUserDirectories()
{
	debug("InitUserDirectories");

	HKEY hKey = NULL;
	DWORD dwType = 0;
	HANDLE hFile = NULL;

	if (RegOpenKeyEx(
		HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
		0, KEY_READ, &hKey))
	{
		strcpy(szGtaUserDirPath.Get(), "data");
		return szGtaUserDirPath.Get();
	}

	DWORD cbData = szGtaUserDirPath.SizeOf();
	if (RegQueryValueEx(hKey, "Personal", 0, &dwType, (LPBYTE)szGtaUserDirPath.Get(), &cbData))
	{
		RegCloseKey(hKey);
		strcpy(szGtaUserDirPath.Get(), "data");
		return szGtaUserDirPath.Get();
	}

	RegCloseKey(hKey);

	// GTA San Andreas User Files

	strcpy(&szGtaUserDirPath.Get()[strlen(szGtaUserDirPath.Get())], "\\GTA San Andreas User Files");

	hFile = CreateFile(szGtaUserDirPath.Get(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		CreateDirectory(szGtaUserDirPath.Get(), NULL);
	}
	else {
		CloseHandle(hFile);
	}

	// Gallery

	strcpy(szGalleryDirPath.Get(), szGtaUserDirPath.Get());
	strcat(szGalleryDirPath.Get(), "\\Gallery");

	hFile = CreateFile(szGalleryDirPath.Get(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		CreateDirectory(szGalleryDirPath.Get(), NULL);
	}
	else {
		CloseHandle(hFile);
	}

	// User Tracks

	strcpy(szUserTracksDirPath.Get(), szGtaUserDirPath.Get());
	strcat(szUserTracksDirPath.Get(), "\\User Tracks");

	hFile = CreateFile(szUserTracksDirPath.Get(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		CreateDirectory(szUserTracksDirPath.Get(), NULL);
	}
	else {
		CloseHandle(hFile);
	}

	return szGtaUserDirPath.Get();
}

STARTPATCHES
InjectHook(0x744FB0, &InitUserDirectories, PATCH_JUMP);
ENDPATCHES