#pragma once
#include "pch.h"
#include "windows.h"

void Shoot();
void Trigger(HANDLE hProcHandle, DWORD LocalPlayer, DWORD iCrosshairId, DWORD EngineDLL, DWORD ClientDLL);
void GlowESP(HANDLE hProcHandle, DWORD ClientDLL, DWORD EngineDLL, DWORD GlowPointer, DWORD dwEntityList, DWORD dwGlowIndex);
int GetGameState(HANDLE hProcHandle, DWORD EngineDLL);
DWORD SetGlowPointer(HANDLE hProcHandle, DWORD ClientDLL);
DWORD SetLocalPlayer(HANDLE hProcHandle, DWORD ClientDLL);