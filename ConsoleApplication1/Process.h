#pragma once
#include "pch.h"
#include "windows.h"

extern DWORD dwProcessId;
char ClientModule[];
char EngineModule[];

HMODULE FindModule(char *szModuleName, DWORD dwProcessId);
HANDLE GetHandle();
