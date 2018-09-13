#include "pch.h"
#include <iostream>
#include "windows.h"
#include "tlhelp32.h"
#include "Process.h"

char ClientModule[] = "client_panorama.dll";
char EngineModule[] = "engine.dll";
DWORD dwProcessId = NULL;

HANDLE GetHandle()
{
	LPCSTR LGameWindow = "Counter-Strike: Global Offensive";
	HANDLE hProcHandle = NULL;
	HWND hGameWindow = NULL;
	do 
	{
		Sleep(25);
		hGameWindow = FindWindow(NULL, LGameWindow);
		GetWindowThreadProcessId(hGameWindow, &dwProcessId);
		hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	} while (!hGameWindow);
	std::cout << "game found\n";
	return hProcHandle;
}

HMODULE FindModule(char *szModuleName, DWORD dwProcessId)
{
	if (!szModuleName || !dwProcessId) { return NULL; }
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId); 
	if (hSnap == INVALID_HANDLE_VALUE) { return NULL; }
	MODULEENTRY32 me; 
	me.dwSize = sizeof(MODULEENTRY32);

	Module32First(hSnap, &me);
	while (strcmp(me.szModule, szModuleName)) 
	{
		Module32Next(hSnap, &me);
	}
	CloseHandle(hSnap); 
	std::cout << "module found\n";
	return me.hModule;
}