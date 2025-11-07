#include <iostream>
#include "mem.h"

#pragma once

enum ASM : uint8_t
{
    Push = 0x68,
    Mov = 0x48,
    Movss = 0xF3,
    Jmp = 0xE9,

    RCX = 0xB9, // 8 byte pad after
    RDX = 0xBA, // 8 byte pad after
    RBX = 0x8B, // 8 byte pad after

    dl = 0xB2, // 1 byte pad after(includes mov instruction)

    Xmm0 = 0x05, //8 byte pad after
    Xmm2 = 0x15, //8 byte pad after
};

class CArg
{
public:
    ASM reg;
    uintptr_t value;

    void WriteArg(uintptr_t& address)
    {
        if (reg == ASM::Mov 
         || reg == ASM::Jmp
         || reg == ASM::Movss)
            return;

        if (reg == ASM::Xmm0 || 
            reg == ASM::Xmm2)
        {
            std::vector<uint8_t> movssBytes = { 0xF3, 0x0F, 0x10 };
            mem->WriteBytes(address, movssBytes);
            address += movssBytes.size();

            mem->Write<ASM>(address, reg);
            address += sizeof(ASM);

            uintptr_t disp = value - (address + sizeof(uint32_t));
            mem->Write<uint32_t>(address, static_cast<uint32_t>(disp));
            address += sizeof(uint32_t);
            return;
        }

        if (reg != ASM::Push || reg != ASM::dl)
        {
            mem->Write<ASM>(address, ASM::Mov);
            address += sizeof(ASM);
        }

        mem->Write<ASM>(address, reg);
        address += sizeof(ASM);

        

        if (reg == ASM::dl)
        {
            mem->Write<char>(address, value);
            address += sizeof(char);
        }
        else
        {
            mem->Write<uintptr_t>(address, value);
            address += sizeof(uintptr_t);
        }
    }
};

class WCL
{
public:
    uintptr_t FuncAlloc(const uintptr_t funcAddress)
    {
        uintptr_t allocAddress = NULL;

        for (int i = 0; i < 100000; i++)
        {
            allocAddress = mem->Allocate(funcAddress - (i * MemPage), MemPage);
            if (allocAddress)
                break;
        }

        return allocAddress;
    }

    void CallFunction(const uintptr_t FuncAddress, std::vector<CArg> args = {}, const bool BypassSafeDelay = false)
    {
        if (!FuncAddress || !mem->Read<uintptr_t>(FuncAddress))
            return;

        if (args.empty())
        {
            mem->CallThread(FuncAddress);

            return;
        }

        const uintptr_t codeCave = FuncAlloc(FuncAddress);
        if (!codeCave)
            return;

        uintptr_t iCodeCave = codeCave;
        //iCodeCave += 0x2;

        for (CArg& arg : args)
        {
            arg.WriteArg(iCodeCave);
        }

        mem->Write<ASM>(iCodeCave, ASM::Jmp);
        iCodeCave += sizeof(ASM);

        uintptr_t callAddress = (FuncAddress - sizeof(uint32_t)) - iCodeCave;
        mem->Write<uintptr_t>(iCodeCave, callAddress);
        iCodeCave += sizeof(uintptr_t);

        //std::cout << std::hex << codeCave << std::endl;
        //while (!GetAsyncKeyState(VK_HOME)) { if (GetAsyncKeyState(VK_END)) { mem->Free(codeCave, MemPage); return; } }
        mem->CallThread(codeCave);
        mem->Free(codeCave, MemPage);

        if (!BypassSafeDelay)
            Sleep(50);

		//return true;//success
    }
};
WCL* wcl = new WCL();