#include "pch.h"
#include <windows.h>
#include "CheatFuncs.h"
#include <iostream>

void Shoot() 
{
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wVk = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = KEYEVENTF_SCANCODE;
	ip.ki.wScan = 0x19; 
	SendInput(1, &ip, sizeof(INPUT));
	Sleep(25);
	ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}

void Trigger(HANDLE hProcHandle, DWORD LocalPlayer, DWORD iCrosshairId, DWORD EngineDLL, DWORD ClientDLL) 
{
	const auto TriggerKey{ VK_XBUTTON2 }; 

	DWORD CrosshairId = NULL;

	while (true)
	{
		Sleep(25);
		while (GetGameState(hProcHandle, EngineDLL) == 6)
		{
			Sleep(10000);
			LocalPlayer = SetLocalPlayer(hProcHandle, ClientDLL); 
			std::cout << "set LocalPlayer to: " << LocalPlayer << "\n";
			while (GetGameState(hProcHandle, EngineDLL) == 6)
			{
				ReadProcessMemory(hProcHandle, (PBYTE)(LocalPlayer + iCrosshairId), &CrosshairId, 4, NULL); 
				if (GetAsyncKeyState(TriggerKey) && CrosshairId > 0) 
					Shoot();
				Sleep(25);
			}

		}
	}
}

void GlowESP(HANDLE hProcHandle, DWORD ClientDLL, DWORD EngineDLL, DWORD GlowPointer, DWORD dwEntityList, DWORD dwGlowIndex)
{
	const auto GlowKey{ VK_XBUTTON1 }; 

	struct Glow 
	{
		float r;
		float g;
		float b;
		float a;
	};

	struct BoolGlow
	{
		bool renderOccluded = true;
		bool renderUnoccluded = false;
	};

	Glow GlowRender{ 1.f, 0.f, 0.f, 0.65f }; 
	BoolGlow Glow2{ true,true };

	int Glow_CurrentPlayer = NULL; 
	int Glow_CurrentPlayerGlowIndex = NULL; 

	bool isGlow(true); 

	while (true)
	{
		Sleep(25);
		while (GetGameState(hProcHandle, EngineDLL) == 6)
		{
			std::cout << "enabling in 10 seconds\n";
			Sleep(10000);
			GlowPointer = SetGlowPointer(hProcHandle, ClientDLL); 
			std::cout << "set GlowPointer to: " << GlowPointer << "\n";
			while (GetGameState(hProcHandle, EngineDLL) == 6)
			{
				if (GetAsyncKeyState(GlowKey) & 1) 
				{
					isGlow = !isGlow;
				}

				if (isGlow)
				{
					for (int i = 0; i < 32; i++) 
					{
						ReadProcessMemory(hProcHandle, (PBYTE)(ClientDLL + dwEntityList + (i * 16)), &Glow_CurrentPlayer, 4, NULL); 
						ReadProcessMemory(hProcHandle, (PBYTE)((Glow_CurrentPlayer + dwGlowIndex)), &Glow_CurrentPlayerGlowIndex, 4, NULL);
						WriteProcessMemory(hProcHandle, (PBYTE)((GlowPointer + ((Glow_CurrentPlayerGlowIndex * 0x38) + 0x4))), &GlowRender, sizeof(GlowRender), NULL);
						WriteProcessMemory(hProcHandle, (PBYTE)((GlowPointer + ((Glow_CurrentPlayerGlowIndex * 0x38) + 0x24))), &Glow2, sizeof(Glow2), NULL);
						Sleep(1);
					}
				}
			}
		}
	}
}