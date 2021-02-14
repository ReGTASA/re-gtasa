#include <Windows.h>

#include "utils/patcher.h"

void debug(const char* format, ...)
{
	static const int re3_buffsize = 1024;
	static char re3_buff[re3_buffsize];

	va_list va;
	va_start(va, format);
	vsprintf_s(re3_buff, re3_buffsize, format, va);
	va_end(va);

	printf("%s\n", re3_buff);
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);

		debug("re-gtasa loaded");

		StaticPatcher::Apply();
	}

	return TRUE;
}