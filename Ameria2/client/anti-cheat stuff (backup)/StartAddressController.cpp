#include "StdAfx.h"
#include "AnticheatManager.h"
#include "CheatQueueManager.h"
#include "../eterBase/WinVerHelper.h"

#include <XORstr.h>
#include <Psapi.h>

enum EBlacklistedShellCodes
{
	ShellCodeNull,
	LdrLoadShellCode1,
	ManualMapShellCode1,
	ManualMapShellCode2,
	ManualMapShellCode3,
	ReflectiveShellCode1,
	ManualLoadShellCode1,
	ThreadHijackShellCode1,
	ThreadHijackShellCode2,
	ThreadHijackShellCode3,
	CreateRemoteThreadExShellCode1,
	CodeInjectionShellCode1,
	AutoInjectorLalakerShellCode,
	EaxLoadLibraryA,
	EaxLoadLibraryW,
	EaxLoadLibraryExA,
	EaxLoadLibraryExW,
	EaxLoadLibraryA_KBase,
	EaxLoadLibraryW_KBase,
	EaxLoadLibraryExA_KBase,
	EaxLoadLibraryExW_KBase,
	EaxLdrLoadDll,
	EaxSetWindowHookEx,
	EaxPython,
	EaxRtlUserThreadStart,
	EaxSetWindowHookEx2,
	EaxNtCreateThread,
	EaxNtCreateThreadEx,
	EaxRtlCreateUserThread,
	EaxUnknownState,
	EaxBadPointerType,
	EaxBadProtectType,
	QueryWorkingSetExFail,
	QueryWorkingSetExNotValid,
	NullCharacteristics,
	EaxMainProcess,
	EaxMappedCode,
};

inline int FilterShellcode(LPVOID lpTargetAddress)
{
	auto pMemory = (BYTE*)lpTargetAddress;
	BYTE shellLdrLoad[5] = { 0x55, 0x8B, 0xEC, 0x8D, 0x5 };
	BYTE shellManualMp[6] = { 0x55, 0x8B, 0xEC, 0x51, 0x53, 0x8B };
	BYTE shellReflective[8] = { 0x55, 0x89, 0xE5, 0x53, 0x83, 0xEC, 0x54, 0x8B };
	BYTE shellMLoad[8] = { 0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x20, 0x53, 0x56 };
	BYTE shellhijack[10] = { 0x68, 0xCC, 0xCC, 0xCC, 0xCC, 0x60, 0x9C, 0xBB, 0xCC, 0xCC };
	BYTE shellhijack2[10] = { 0x60, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x5B, 0x81, 0xEB, 0x06 };
	BYTE shellhijack3[10] = { 0x56, 0x8B, 0x35, 0x00, 0xC0, 0x27, 0x6A, 0x57, 0x8B, 0x3D };
	BYTE shellcreateremotethreadex[10] = { 0xE8, 0x1D, 0x00, 0x00, 0x00, 0x50, 0x68, 0x58, 0x58, 0xC3 };
	BYTE shellcodeinjectrosdevil[8] = { 0x68, 0xAC, 0xCE, 0xEA, 0xAC, 0x9C, 0x60, 0x68 };
	BYTE shellcodeLalakerAuto[8] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC, 0x5D, 0xFF, 0x25 };

	// LdrLoadDll, LdrpLoadDll, ManualMap1
	if (memcmp(lpTargetAddress, &shellLdrLoad, 5) == 0)
		return LdrLoadShellCode1;

	// ManualMap2
	if (memcmp(lpTargetAddress, &shellManualMp, 6) == 0)
		return ManualMapShellCode1;

	// ManualMap3 ++
	if (*pMemory == 0x68 && *(pMemory + 5) == 0x68)
	{
		if (*(pMemory + 10) == 0xB8)
			return ManualMapShellCode2;
		else if (*(pMemory + 10) == 0x68)
			return ManualMapShellCode3;
	}

	// Reflective
	if (memcmp(lpTargetAddress, &shellReflective, sizeof(shellReflective)) == 0)
		return ReflectiveShellCode1;

	// Manual Load
	if (memcmp(lpTargetAddress, &shellMLoad, sizeof(shellMLoad)) == 0)
		return ManualLoadShellCode1;

	// Thread hijack 1
	if (memcmp(lpTargetAddress, &shellhijack, sizeof(shellhijack)) == 0)
		return ThreadHijackShellCode1;

	// Thread hijack 2
	if (memcmp(lpTargetAddress, &shellhijack2, sizeof(shellhijack2)) == 0)
		return ThreadHijackShellCode2;

	// Thread hijack 3
	if (memcmp(lpTargetAddress, &shellhijack3, sizeof(shellhijack3)) == 0)
		return ThreadHijackShellCode3;

	// Createremotethreadex 1
	if (memcmp(lpTargetAddress, &shellcreateremotethreadex, sizeof(shellcreateremotethreadex)) == 0)
		return CreateRemoteThreadExShellCode1;

	// Code injection 1
	if (memcmp(lpTargetAddress, &shellcodeinjectrosdevil, sizeof(shellcodeinjectrosdevil)) == 0)
		return CodeInjectionShellCode1;

	// Lalaker auto injector
	if (memcmp(lpTargetAddress, &shellcodeLalakerAuto, sizeof(shellcodeLalakerAuto)) == 0)
		return AutoInjectorLalakerShellCode;

	return ShellCodeNull;
}

bool CAnticheatManager::IsBlockedStartAddress(DWORD dwThreadId, DWORD dwStartAddress, LPDWORD pdwViolationID, std::string * pstrViolationOwnerModule)
{
	typedef NTSTATUS(WINAPI* TNtQueryInformationThread)(HANDLE, LONG, PVOID, ULONG, PULONG);
	
	auto hKernel32 = GetModuleHandleA(XOR("kernel32.dll"));
	if (!hKernel32)
	{
//		TraceError("!hKernel32");
		return false;
	}

	auto hUser32 = LoadLibraryA(XOR("user32.dll"));
	if (!hUser32)
	{
//		TraceError("!hUser32");
		return false;
	}

	auto hNtdll = LoadLibraryA(XOR("ntdll.dll"));
	if (!hNtdll)
	{
//		TraceError("!hNtdll");
		return false;
	}

	auto hPython27 = GetModuleHandleA(XOR("python27.dll"));
	if (!hPython27)
	{
//		TraceError("!hPython27");
		return false;
	}

	DWORD dwThreadStartAddress = 0;

	auto hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, dwThreadId);
	if (hThread && hThread != INVALID_HANDLE_VALUE)
	{
		auto pNtQueryInformationThread = GetProcAddress(hNtdll, XOR("NtQueryInformationThread"));
		if (pNtQueryInformationThread)
		{
			auto NtQueryInformationThread = reinterpret_cast<TNtQueryInformationThread>(pNtQueryInformationThread);

			auto ntStatus = NtQueryInformationThread(hThread, /* ThreadQuerySetWin32StartAddress */ 9, &dwThreadStartAddress, sizeof(dwThreadStartAddress), NULL);
		}
	}

	if (!dwThreadStartAddress)
		dwThreadStartAddress = dwStartAddress;

	if (!dwThreadStartAddress)
	{
//		TraceError("!dwThreadStartAddress");
		return false;
	}

	char szFileName[1024] = { 0 };
	GetMappedFileNameA(GetCurrentProcess(), (LPVOID)dwThreadStartAddress, szFileName, sizeof(szFileName));

//	if (!szFileName || !strlen(szFileName))
//		strcpy_s(szFileName, "");

	auto dwDetectedType = 0UL;

	auto dwLoadLibraryA = (DWORD)GetProcAddress(hKernel32, XOR("LoadLibraryA"));
	auto dwLoadLibraryW = (DWORD)GetProcAddress(hKernel32, XOR("LoadLibraryW"));
	auto dwLoadLibraryExA = (DWORD)GetProcAddress(hKernel32, XOR("LoadLibraryExA"));
	auto dwLoadLibraryExW = (DWORD)GetProcAddress(hKernel32, XOR("LoadLibraryExW"));

	auto dwLdrLoadDll = (DWORD)GetProcAddress(hNtdll, XOR("LdrLoadDll"));

	//

	MODULEINFO user32ModInfo = { 0 };
	if (!GetModuleInformation(GetCurrentProcess(), hUser32, &user32ModInfo, sizeof(user32ModInfo)))
	{
//		TraceError("!user32ModInfo");
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return false;
	}

	auto dwUser32Low = (DWORD)user32ModInfo.lpBaseOfDll;
	auto dwUser32Hi = (DWORD)user32ModInfo.lpBaseOfDll + user32ModInfo.SizeOfImage;

	MODULEINFO pythonModInfo = { 0 };
	if (!GetModuleInformation(GetCurrentProcess(), hPython27, &pythonModInfo, sizeof(pythonModInfo)))
	{
//		TraceError("!pythonModInfo");
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return false;
	}

	auto dwPythonLow = (DWORD)pythonModInfo.lpBaseOfDll;
	auto dwPythonHi = (DWORD)pythonModInfo.lpBaseOfDll + pythonModInfo.SizeOfImage;

	MEMORY_BASIC_INFORMATION mbiCaller = { 0 };
	if (!VirtualQuery((LPCVOID)dwThreadStartAddress, &mbiCaller, sizeof(mbiCaller)))
	{
//		TraceError("!VirtualQuery");
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return false;
	}

	int iShellInjectionRet = FilterShellcode((LPVOID)dwThreadStartAddress);
	if (iShellInjectionRet)
	{
		if (pdwViolationID) *pdwViolationID = iShellInjectionRet;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}

	if (dwThreadStartAddress == dwLoadLibraryA)
	{
		if (pdwViolationID) *pdwViolationID = EaxLoadLibraryA;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}
	else if (dwThreadStartAddress == dwLoadLibraryW)
	{
		if (pdwViolationID) *pdwViolationID = EaxLoadLibraryW;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}
	else if (dwThreadStartAddress == dwLoadLibraryExA)
	{
		if (pdwViolationID) *pdwViolationID = EaxLoadLibraryExA;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}
	else if (dwThreadStartAddress == dwLoadLibraryExW)
	{
		if (pdwViolationID) *pdwViolationID = EaxLoadLibraryExW;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}
	else if (dwThreadStartAddress == dwLdrLoadDll)
	{
		if (pdwViolationID) *pdwViolationID = EaxLdrLoadDll;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}
	else if (dwThreadStartAddress >= dwUser32Low && dwThreadStartAddress <= dwUser32Hi)
	{
		if (pdwViolationID) *pdwViolationID = EaxSetWindowHookEx;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}
	else if (dwThreadStartAddress >= dwPythonLow && dwThreadStartAddress <= dwPythonHi)
	{
		if (pdwViolationID) *pdwViolationID = EaxPython;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}

	if (IsWindowsVistaOrGreater())
	{
		auto dwRtlUserThreadStart = (DWORD)GetProcAddress(hNtdll, XOR("RtlUserThreadStart"));
		if (dwThreadStartAddress == dwRtlUserThreadStart)
		{
			if (pdwViolationID) *pdwViolationID = EaxRtlUserThreadStart;
			if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
			if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
			return true;
		}

		auto dwNtCreateThread = (DWORD)GetProcAddress(hNtdll, XOR("NtCreateThread"));
		if (dwThreadStartAddress == dwNtCreateThread)
		{
			if (pdwViolationID) *pdwViolationID = EaxNtCreateThread;
			if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
			if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
			return true;
		}

		auto dwNtCreateThreadEx = (DWORD)GetProcAddress(hNtdll, XOR("NtCreateThreadEx"));
		if (dwThreadStartAddress == dwNtCreateThreadEx)
		{
			if (pdwViolationID) *pdwViolationID = EaxNtCreateThreadEx;
			if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
			if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
			return true;
		}

		auto dwRtlCreateUserThread = (DWORD)GetProcAddress(hNtdll, XOR("RtlCreateUserThread"));
		if (dwThreadStartAddress == dwRtlCreateUserThread)
		{
			if (pdwViolationID) *pdwViolationID = EaxRtlCreateUserThread;
			if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
			if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
			return true;
		}
	}

	auto hKernelbase = LoadLibraryA(XOR("kernelbase.dll"));
	if (hKernelbase)
	{
		auto dwLoadLibraryA_KBase = (DWORD)GetProcAddress(hKernelbase, XOR("LoadLibraryA"));
		if (dwLoadLibraryA_KBase && dwThreadStartAddress == dwLoadLibraryA_KBase)
		{
			if (pdwViolationID) *pdwViolationID = EaxLoadLibraryA_KBase;
			if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
			if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
			return true;
		}

		auto dwLoadLibraryW_KBase = (DWORD)GetProcAddress(hKernelbase, XOR("LoadLibraryW"));
		if (dwLoadLibraryW_KBase && dwThreadStartAddress == dwLoadLibraryW_KBase)
		{
			if (pdwViolationID) *pdwViolationID = EaxLoadLibraryW_KBase;
			if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
			if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
			return true;
		}

		auto dwLoadLibraryExA_KBase = (DWORD)GetProcAddress(hKernelbase, XOR("LoadLibraryExA"));
		if (dwLoadLibraryExA_KBase && dwThreadStartAddress == dwLoadLibraryExA_KBase)
		{
			if (pdwViolationID) *pdwViolationID = EaxLoadLibraryExA_KBase;
			if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
			if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
			return true;
		}

		auto dwLoadLibraryExW_KBase = (DWORD)GetProcAddress(hKernelbase, XOR("LoadLibraryExW"));
		if (dwLoadLibraryExW_KBase && dwThreadStartAddress == dwLoadLibraryExW_KBase)
		{
			if (pdwViolationID) *pdwViolationID = EaxLoadLibraryExW_KBase;
			if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
			if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
			return true;
		}
	}

	if (mbiCaller.State != MEM_COMMIT)
	{
		if (pdwViolationID) *pdwViolationID = EaxUnknownState;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}

	if (mbiCaller.AllocationProtect == PAGE_EXECUTE_READWRITE)
	{
		if (pdwViolationID) *pdwViolationID = EaxBadProtectType;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}

	if (IsWindowsVistaOrGreater())
	{
		PSAPI_WORKING_SET_EX_INFORMATION pworkingSetExInformation = { 0 };
		pworkingSetExInformation.VirtualAddress = (LPVOID)dwThreadStartAddress;

		if (FALSE == QueryWorkingSetEx(GetCurrentProcess(), &pworkingSetExInformation, sizeof(pworkingSetExInformation)))
		{
			if (pdwViolationID) *pdwViolationID = QueryWorkingSetExFail;
			if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
			if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
			return true;
		}

		if (!pworkingSetExInformation.VirtualAttributes.Valid)
		{
			if (pdwViolationID) *pdwViolationID = QueryWorkingSetExNotValid;
			if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
			if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
			return true;
		}
	}

	auto szLowerFileName = std::string(szFileName);
	std::transform(szLowerFileName.begin(), szLowerFileName.end(), szLowerFileName.begin(), tolower);

	char szExeName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szExeName, sizeof(szExeName));

	auto strLowerExeName = std::string(szExeName);
	std::transform(strLowerExeName.begin(), strLowerExeName.end(), strLowerExeName.begin(), tolower);

	if (szFileName && szLowerFileName == strLowerExeName)
	{
		if (pdwViolationID) *pdwViolationID = EaxMainProcess;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}

	if (szLowerFileName.empty() && mbiCaller.Type == MEM_PRIVATE && mbiCaller.RegionSize == 0x1000)
	{
		if (pdwViolationID) *pdwViolationID = EaxMappedCode;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}

	auto pCurrentSecHdr = (IMAGE_SECTION_HEADER*)dwThreadStartAddress;
	if (pCurrentSecHdr && !pCurrentSecHdr->Characteristics)
	{
		if (pdwViolationID) *pdwViolationID = NullCharacteristics;
		if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
		if (pstrViolationOwnerModule) *pstrViolationOwnerModule = szFileName;
		return true;
	}

	if (hThread && hThread != INVALID_HANDLE_VALUE) CloseHandle(hThread);
	return false;
}


