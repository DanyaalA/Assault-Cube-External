// AssaultCubeTrainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <windows.h>
#include "proc.h"


int main()
{
    //Get ProcID
    DWORD procID = GetProcID(L"ac_client.exe");

    //Get Module Base
    uintptr_t modBase = GetModuleBaseAddress(procID, L"ac_client.exe");

    //Get Handle to Process
    HANDLE hProc = 0;
    hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

    //Resolve Base Addy for pointer
    uintptr_t dynamicPtrBaseAddr = modBase + 0x10F4F4;

    std::cout << "DynamicPtrbaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    //Resolve Ammo Pointer
    uintptr_t ammoAddr = FindDMAAddy(hProc, dynamicPtrBaseAddr, { 0x374, 0x14, 0x0 });
    
    std::cout << "ammoPtr = " << "0x" << std::hex << ammoAddr << std::endl;


    //Read Ammo Val
    int ammoVal = 0;
    ReadProcessMemory(hProc, (BYTE*)ammoAddr, &ammoVal, sizeof(ammoVal), nullptr);

    std::cout << "Ammo Value" << std::dec << ammoVal << std::endl;

    //Write To Ammo Val
    int newAmmo = 1337;
    WriteProcessMemory(hProc, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    //Read New Val
    ReadProcessMemory(hProc, (BYTE*)ammoAddr, &ammoVal, sizeof(ammoVal), nullptr);
    std::cout << "Ammo Value" << std::dec << ammoVal << std::endl;

    getchar();
    return 0;
}

