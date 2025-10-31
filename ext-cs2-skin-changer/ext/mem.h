#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cstdint>
#include <vector>

#define MemPage 0xFFF//4095 bytes

class Memory {
private:
public:
    DWORD processId = 0;
    HANDLE hProcess = nullptr;

    HANDLE CreateThread(uintptr_t funcAddress, LPVOID param = nullptr) const
    {
        if (!hProcess || !funcAddress)
            return nullptr;

        HANDLE hThread = CreateRemoteThread(
            hProcess,
            nullptr,              // default security
            0,                    // default stack
            reinterpret_cast<LPTHREAD_START_ROUTINE>(funcAddress),
            param,                // optional parameter
            0,                    // run immediately
            nullptr               // thread ID
        );

        return hThread; // caller should CloseHandle
    }

    // Get process ID from name
    DWORD GetProcessIdByName(const std::wstring& name) {
        DWORD pid = 0;
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap == INVALID_HANDLE_VALUE)
            return 0;

        PROCESSENTRY32W entry{};
        entry.dwSize = sizeof(entry);

        if (Process32FirstW(snap, &entry)) {
            do {
                if (!_wcsicmp(entry.szExeFile, name.c_str())) {
                    pid = entry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snap, &entry));
        }

        CloseHandle(snap);
        return pid;
    }

public:
    // Constructor: auto-attaches to cs2.exe
    Memory(const std::wstring& processName = L"cs2.exe") {
        processId = GetProcessIdByName(processName);
        if (!processId)
            throw std::runtime_error("Failed to find process");

        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
        if (!hProcess)
            throw std::runtime_error("Failed to open process handle");
    }

    ~Memory() {
        if (hProcess) CloseHandle(hProcess);
    }

    // Template Read
    template <typename T>
    T Read(uintptr_t address) const {
        T buffer{};
        ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), nullptr);
        return buffer;
    }

    std::string ReadCUtlSymbolLarge(uintptr_t address) const {
        // Read the raw ID (pointer) from the target address
        uintptr_t id = Read<uintptr_t>(address);
        // UTL_INVAL_SYMBOL_LARGE is ~0 (all bits 1) for invalid symbols:contentReference[oaicite:6]{index=6}
        if (id == static_cast<uintptr_t>(-1)) {
            return std::string();  // empty string
        }
        // Read characters one by one until null terminator
        std::string result;
        while (true) {
            char c = Read<char>(id);
            if (c == '\0') {
                break;
            }
            result.push_back(c);
            ++id;
        }
        return result;
    }


    // Template Write
    template <typename T>
    void Write(uintptr_t address, const T& value) const {
        WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(address), &value, sizeof(T), nullptr);
    }

    void WriteString(uintptr_t pAddress, std::string string)
    {
        for (int i = 0; i< string.size(); i++)
        {
            Write<char>(pAddress + i, string[i]);
        }
    }

    // Module base address getter
    uintptr_t GetModuleBase(const std::wstring& moduleName) const {
        uintptr_t base = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
        if (snapshot == INVALID_HANDLE_VALUE)
            return 0;

        MODULEENTRY32W moduleEntry{};
        moduleEntry.dwSize = sizeof(moduleEntry);

        if (Module32FirstW(snapshot, &moduleEntry)) {
            do {
                if (!_wcsicmp(moduleEntry.szModule, moduleName.c_str())) {
                    base = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                    break;
                }
            } while (Module32NextW(snapshot, &moduleEntry));
        }

        CloseHandle(snapshot);
        return base;
    }

    uintptr_t SigScan(const std::wstring& moduleName, const std::string& pattern) const 
    {
        uintptr_t base = GetModuleBase(moduleName);
        if (!base) return 0;

        // Read module size
        MODULEENTRY32W modEntry{};
        modEntry.dwSize = sizeof(modEntry);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
        if (snapshot == INVALID_HANDLE_VALUE) return 0;

        SIZE_T moduleSize = 0;
        if (Module32FirstW(snapshot, &modEntry)) {
            do {
                if (!_wcsicmp(modEntry.szModule, moduleName.c_str())) {
                    moduleSize = modEntry.modBaseSize;
                    break;
                }
            } while (Module32NextW(snapshot, &modEntry));
        }

        CloseHandle(snapshot);
        if (moduleSize == 0) return 0;

        // Read full module into buffer
        std::vector<std::uint8_t> buffer(moduleSize);
        SIZE_T bytesRead;
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(base), buffer.data(), moduleSize, &bytesRead))
            return 0;

        // Convert pattern string to bytes
        auto patternToBytes = [](const std::string& pattern) {
            std::vector<std::pair<uint8_t, bool>> bytes;
            const char* start = pattern.c_str();
            const char* end = start + pattern.length();

            while (start < end) {
                if (*start == '?') {
                    ++start;
                    if (*start == '?') ++start;
                    bytes.emplace_back(0, true);
                }
                else {
                    bytes.emplace_back(static_cast<uint8_t>(strtoul(start, const_cast<char**>(&start), 16)), false);
                }
                while (*start == ' ') ++start;
            }
            return bytes;
            };

        auto sig = patternToBytes(pattern);
        size_t sigSize = sig.size();

        // Scan buffer for signature
        for (size_t i = 0; i < buffer.size() - sigSize; ++i) {
            bool found = true;
            for (size_t j = 0; j < sigSize; ++j) {
                if (!sig[j].second && buffer[i + j] != sig[j].first) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return base + i;
            }
        }

        return 0;  // Not found
    }

    uintptr_t GetVtableFunc(const uintptr_t& Vtable, uint16_t Index)
    {
        return Read<uintptr_t>(Vtable + (sizeof(uintptr_t) * Index));
    }

    bool IsAllocated(uintptr_t address)
    {
        MEMORY_BASIC_INFORMATION mbi;
        SIZE_T result = VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi));

        if (result == 0)
            return false; // Query failed

        // Check if the memory is committed and accessible
        if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_NOACCESS) == 0 && (mbi.Protect & PAGE_GUARD) == 0)
        {
            return true;
        }

        return false;
    }

    uintptr_t GetFreeMemoryRegion(size_t size)
    {
        uintptr_t address = NULL;
        size_t validRegionCount = NULL;
        while (true)
        {
            if (IsAllocated(address) && !Read<uint8_t>(address))
                validRegionCount++;
            else
                validRegionCount = NULL;

            if (validRegionCount == size)
                return address - validRegionCount + 1;

            address++;
        }
    }

    void Patch(uintptr_t address, int size)
    {
        if (!address)
            return;

        for (int i = 0; i < size; i++)
        {
            Write<uint8_t>(address + i, 0x90);
        }
    }

    void WriteBytes(uintptr_t address, std::vector<uint8_t> bytes)
    {
        for (int i = 0; i < bytes.size(); i++)
        {
            Write<uint8_t>(address + i, bytes[i]);
        }
    }

    std::vector<uint8_t> ReadBytes(uintptr_t address, int size)
    {
        std::vector<uint8_t> bytes;
        for (int i = 0; i < size; i++)
        {
            bytes.push_back(Read<uint8_t>(address + i));
        }
        return bytes;
    }

    void SwapPatch(uintptr_t address, int bytesSize, int delay = 0)
    {
        if (!address)
            return;

        std::vector<uint8_t> ogBytes = ReadBytes(address, bytesSize);
        Patch(address, bytesSize);
        Sleep(delay);
        WriteBytes(address, ogBytes);
    }

    inline uintptr_t ResolveRelativeAddress(uintptr_t instruction, int offsetOffset = 3, int instructionSize = 7)
    {
        int32_t relativeOffset = Read<int32_t>(instruction + offsetOffset);
        return instruction + instructionSize + relativeOffset;
    }

    uintptr_t Allocate(uintptr_t address = NULL, size_t size = NULL, DWORD protection = PAGE_EXECUTE_READWRITE) const {
        LPVOID addr = VirtualAllocEx(
            hProcess,            
            reinterpret_cast<LPVOID>(address),
            size,                 
            MEM_COMMIT | MEM_RESERVE,
            protection           
        );

        return reinterpret_cast<uintptr_t>(addr);
    }

    bool Free(uintptr_t address, size_t size = 0) const
    {
        // size can be 0 when using MEM_RELEASE
        // MEM_RELEASE requires size = 0
        return VirtualFreeEx(
            hProcess,
            reinterpret_cast<LPVOID>(address),
            0,                // must be 0 for MEM_RELEASE
            MEM_RELEASE
        ) != 0;
    }

    
    void CallThread(uintptr_t funcAddress, LPVOID arg = nullptr)
    {
        if (!funcAddress)
            return;

        HANDLE hThread = CreateThread(funcAddress, arg);
        if (hThread) {
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
        }
    }
};
Memory* mem = new Memory();