// Checkout the newer version of the IATPatcher in CommonLib
// https://github.com/mayossi/CommonLib/tree/main/src/Windows/Hook/IATHookInstaller



#include <Windows.h>

#include "IATPatcher.h"
#include "Windows/Hook/Hook.h"

int WINAPI hookedMessageBoxW(const HWND hWindow, LPCWSTR lpText, LPCWSTR lpCaption, const UINT uType)
{
	const HMODULE hUser32 = GetModuleHandle(L"USER32.dll");
	if (!hUser32)
	{
		return 0;
	}

	const auto originalMessageBoxW = reinterpret_cast<int (WINAPI*)(HWND, LPCWSTR, LPCWSTR, UINT)>(
		GetProcAddress(hUser32, "MessageBoxW"));

	if (!originalMessageBoxW)
	{
		return 0;
	}

	return originalMessageBoxW(hWindow, L"Hooked", L"Hooked!!!", uType);
}

int main()
{
	clib::windows::hook::Hook hook = clib::windows::hook::Hook{
		"user32.dll",
		"MessageBoxW",
		hookedMessageBoxW,
		MessageBoxW
	};

	// Installing a hook on MessageBoxW.
	patcher::IATPatcher patcher;
	patcher.install(hook);

	// This call will be hooked, our function HookedMessageBoxW will be called instead.
	MessageBoxW(NULL, L"This is some text!", L"This is some text", NULL);
	
	// Uninstalling the hook.
	patcher.uninstall(hook);

	// This call will work normally.
	MessageBoxW(NULL, L"This is some text!", L"This is some text", NULL);
}
