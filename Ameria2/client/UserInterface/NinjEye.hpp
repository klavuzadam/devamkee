#ifndef _NINJEYE_HPP_
#define _NINJEYE_HPP_
//
//   Copyright (C) 2022 NinjEye
//   Author(s)    : Mihai PANDURU (mpanduru@ninjeye.net)
//

#include <future>

namespace NinjEye
{
typedef enum _COMMAND
{
    Reserved0,
    Reserved1,
    Reserved2,
    Reserved3,

    MaxCommand
} COMMAND, * PCOMMAND;

class Internals
{
    static std::promise<VOID> gPromise;
    static ULONG              gInternalFunctionAddress;

    typedef 
    VOID(CALLBACK* Callback)(
        _In_     const COMMAND      Command, 
        _In_opt_ const BYTE* const  Buffer,
        _In_opt_ const ULONG        BufferSize
    );

    typedef struct _CONFIG
    {
        PVOID License;
        PVOID Callback;
    } CONFIG, * PCONFIG;

    CONFIG config = { 0 };
    HANDLE semaphoreHandle = nullptr;
    DWORD lastError = 0;
    std::future<VOID> CanCleanUp{ };

public:
    explicit Internals(
        _In_ const std::wstring& License, 
        _In_ const Callback      NECallback
    )
    {
        BOOL status = Prepare(License, NECallback);
        if (!status)
        {
            return;
        }

        CanCleanUp = gPromise.get_future();

        status = Start();
        if (!status)
        {
            TerminateProcess(GetCurrentProcess(), 0);
            return;
        }

        CanCleanUp.get(); // wait for INITIALIZE callback
    }
    ~Internals()
    {
        BOOL status = CleanUp();
        if (!status)
        {
            return;
        }
    }

    Internals(_In_ const Internals& other) = delete;
    Internals& operator=(_In_ const Internals& other) = delete;

    Internals(_Inout_ Internals&& other) = delete;
    Internals& operator=(_Inout_ Internals&& other) = delete;

    static
    VOID
    CALLBACK
    NECallback(
        _In_     const NinjEye::COMMAND Command,
        _In_opt_ const BYTE* const      Buffer,
        _In_opt_ const ULONG            BufferSize
    );

    static
    VOID
    SetInternalFunctionAddress(_In_ const BYTE* const Buffer)
    {
        gInternalFunctionAddress = reinterpret_cast<ULONG>(Buffer);
    }

    static
    ULONG
    CallInternalFunction(_In_ ULONG Message)
    {
        return ((ULONG(*)(ULONG))gInternalFunctionAddress)(Message);
    }

private:
    BOOL 
    Prepare(
        _In_ const std::wstring& License, 
        _In_ const Callback      NECallback
    )
    {
        const size_t LicenseLength = License.size();
        config.Callback = NECallback;
        config.License = new WCHAR[LicenseLength + 1];
        memcpy(config.License, License.c_str(), LicenseLength * sizeof(WCHAR));
        *((PWCHAR)config.License + LicenseLength) = UNICODE_NULL;

        /// DO NOT MODIFY THIS
        semaphoreHandle = CreateSemaphoreExW(nullptr, (LONG)(((ULONG)&config & 0x80000000) != 0), (LONG)((ULONG)&config & 0x7FFFFFFF), (LPCWSTR)config.License, 0, SEMAPHORE_ALL_ACCESS);

        ULONG license = 0xC690B00BUL;
        memcpy(config.License, &license, sizeof(license));

        lastError = GetLastError();
        if (lastError == ERROR_ALREADY_EXISTS)
        {
            return FALSE;
        }
        
        return semaphoreHandle && semaphoreHandle != INVALID_HANDLE_VALUE;
    }

    BOOL
    Start()
    {
        HMODULE NinjEyeDLL = LoadLibraryW(L"ninjeye.dll");
        if (!NinjEyeDLL)
        {
            lastError = GetLastError();
            return FALSE;
        }

        return TRUE;
    }

    BOOL 
    CleanUp()
    {
        BOOL status = FALSE;
        if (semaphoreHandle)
        {
            status = CloseHandle(semaphoreHandle);
            if (!status)
            {
                lastError = GetLastError();
            }
            else
            {
                semaphoreHandle = nullptr;
            }
        }

        if (config.License)
        {
            ZeroMemory(config.License, wcslen((PCWCHAR)config.License));
            delete[] config.License;
            config.License = nullptr;
        }

        if (config.Callback)
        {
            config.Callback = nullptr;
        }

        return status;
    }
};
}
#endif//_NINJEYE_HPP_