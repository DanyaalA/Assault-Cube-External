// HackAnyGamePt2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "mem.h"-
int main()
{
	HANDLE hProc = 0;

	uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0;
	bool bHealth = false, bAmmo = false, bRecoil = false;

	const int newValue = 133337;

	DWORD procId = GetProcId(L"ac_client.exe");

	if (procId)
	{
		hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

		localPlayerPtr = moduleBase + 0x10F4F4;

		healthAddr = FindDMAAddy(hProc, localPlayerPtr, { 0xF8 });
	}
	else
	{
		std::cout << "Process not found press enter to exit";
		getchar();
		return 0;
	}

	DWORD dwExit = 0;

	while (GetExitCodeProcess(hProc, &dwExit) && dwExit == STILL_ACTIVE)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bHealth = !bHealth;

		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bAmmo = !bAmmo;

			if (bAmmo)
			{
				//ff 06 = inc [esi]
				mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x06", 2, hProc);
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x0E", 2, hProc);
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bRecoil = !bRecoil;

			if (bRecoil)
			{
				mem::NopEx((BYTE*)(moduleBase + 0x63786), 10, hProc);
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProc);
			}
		}

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			return 0;

		}
		//Continous Write/Freeze

		if (bHealth)
		{
			mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newValue, sizeof(newValue), hProc);
		}

		Sleep(10);
	}

	std::cout << "Process Not Found. Press enter to exit \n";

	return 0;
}