#pragma once

#include "../main.h"

#include <vector>
#include <algorithm>
#include <Windows.h>

#define WRAPPER __declspec(naked)
#define EAXJMP(a) { _asm mov eax, a _asm jmp eax }
#define VARJMP(a) { _asm jmp a }

#define DECL_VAR(type, name)		type* name = (type*)addr;
#define DECL_ARR(type, name, addr)	type** name = (type**)addr;
#define GET_VAR(name)				*(name)
#define GET_ARR(name)				*(name)

#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)

template<typename T, uintptr_t address>
struct GtaVar
{
public:
	T Get() {
		return *_pointer;
	}

	void Set(T value) {
		*_pointer = value;
	}

	size_t SizeOf() {
		return sizeof(T);
	}

private:
	T* _pointer = reinterpret_cast<T*>(address);
};

template<typename T, uintptr_t address, size_t size>
struct GtaArr
{
public:
	T* Get() {
		return _pointer;
	}

	size_t SizeOf() {
		return sizeof(T) * size;
	}
private:
	T* _pointer = reinterpret_cast<T*>(address);
};

enum
{
	PATCH_CALL,
	PATCH_JUMP,
	PATCH_NOTHING,
};

class StaticPatcher
{
	using Patcher = void(*)();

private:
	Patcher m_func;
	StaticPatcher* m_next;
	static StaticPatcher* ms_head;

	void Run() { m_func(); }

public:
	StaticPatcher(Patcher func);
	static void Apply();
};

extern std::vector<uintptr_t> usedAddresses;

template<typename AT, typename HT>
void InjectHook(AT address, HT hook, unsigned int nType = PATCH_NOTHING)
{
	if (std::any_of(usedAddresses.begin(), usedAddresses.end(),
		[address](AT value) { return (uintptr_t)value == address; })) {
		debug("Used address %#06x twice when injecting hook\n", address);
	}

	usedAddresses.push_back((uintptr_t)address);

	DWORD dwProtect[2];
	switch (nType)
	{
	case PATCH_JUMP:
		VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		*(BYTE*)address = 0xE9;
		break;
	case PATCH_CALL:
		VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		*(BYTE*)address = 0xE8;
		break;
	default:
		VirtualProtect((void*)((DWORD)address + 1), 4, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		break;
	}

	DWORD dwHook;
	_asm
	{
		mov		eax, hook
		mov		dwHook, eax
	}

	*(ptrdiff_t*)((DWORD)address + 1) = (DWORD)dwHook - (DWORD)address - 5;
	if (nType == PATCH_NOTHING)
		VirtualProtect((void*)((DWORD)address + 1), 4, dwProtect[0], &dwProtect[1]);
	else
		VirtualProtect((void*)address, 5, dwProtect[0], &dwProtect[1]);
}

#define STARTPATCHES static StaticPatcher Patcher([](){
#define ENDPATCHES });