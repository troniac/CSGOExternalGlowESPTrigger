#include "pch.h"
#include <iostream>
#include "windows.h"
#include <stdio.h>
#include <tchar.h>
#include "tlhelp32.h"
#include "Process.h"
#include "CheatFuncs.h"
#include <thread>
#include <string>
#include <vector>

namespace Offsets
{
	DWORD dwLocalPlayer = 0xC5E87C; 
	DWORD iCrosshairId = 0xB2B8; 
	DWORD iTeamNum = 0xF0;
	DWORD dwEntityList = 0x4C3B384; 
	DWORD dwGlowObject = 0x517A668; 
	DWORD dwGlowIndex = 0xA320; 
	DWORD dwClientState = 0x588A74; 
	DWORD dwClientState_State = 0x108;
}

/*std::vector<std::string> HWIDs = { 
	"",
};*/

namespace Buffers
{
	DWORD LocalPlayer = NULL; 
	DWORD GlowPointer = NULL; 
}

/*std::string GetHWID() //Checks current user's GUID.
{
	HW_PROFILE_INFO HwProfInfo;
	GetCurrentHwProfile(&HwProfInfo);
	std::string CurrentHWID = HwProfInfo.szHwProfileGuid;
	return CurrentHWID;
}*/

int main()
{
	SetConsoleTitle("CSGO Triggerbot + GlowESP");
	/*std::string CurrentHWID = GetHWID(); //Gets current user's GUID.
	bool allowAccess{ false };
	for (const auto &Element : HWIDs) 
		if (CurrentHWID == Element) 
		{
			allowAccess = true;
			break;
		}
	if (!allowAccess) 
	{
		return 0;
	}*/

	HANDLE hProcHandle = GetHandle(); 
	Sleep(3000); 
	DWORD ClientDLL = (DWORD)FindModule(ClientModule, dwProcessId); 
	DWORD EngineDLL = (DWORD)FindModule(EngineModule, dwProcessId); 

	std::cout << "Triggerbot key: XBUTTON2 (side mouse button)\n" << "GlowESP toggle key: XBUTTON1 (side mouse button)\n";

	while (true)
	{
		Sleep(25);
		if (GetGameState(hProcHandle, EngineDLL) == 6) 
													   
			break;
	}

	std::thread Triggerbot(Trigger, hProcHandle, Buffers::LocalPlayer, Offsets::iCrosshairId, EngineDLL, ClientDLL);
	std::thread Glow(GlowESP, hProcHandle, ClientDLL, EngineDLL, Buffers::GlowPointer, Offsets::dwEntityList, Offsets::dwGlowIndex); 
																																	 
	Triggerbot.join();
	Glow.join();
}

DWORD SetLocalPlayer(HANDLE hProcHandle, DWORD ClientDLL)
{
	ReadProcessMemory(hProcHandle, (PBYTE)(ClientDLL + Offsets::dwLocalPlayer), &Buffers::LocalPlayer, 4, NULL); 
	return Buffers::LocalPlayer;
}

DWORD SetGlowPointer(HANDLE hProcHandle, DWORD ClientDLL) 
{
	ReadProcessMemory(hProcHandle, (PBYTE)(ClientDLL + Offsets::dwGlowObject), &Buffers::GlowPointer, 4, NULL);
	return Buffers::GlowPointer;
}

int GetGameState(HANDLE hProcHandle, DWORD EngineDLL)
{
	static DWORD EnginePointer = NULL;
	static int GameState = NULL;
	ReadProcessMemory(hProcHandle, (PBYTE)(EngineDLL + Offsets::dwClientState), &EnginePointer, 4, NULL);
	ReadProcessMemory(hProcHandle, (PBYTE)(EnginePointer + Offsets::dwClientState_State), &GameState, 4, NULL);
	return GameState;
}