#include "StdAfx.h"
#include "AnticheatManager.h"
#include "CheatQueueManager.h"
#include "NTDefinations.h"

#include <XORstr.h>

TEB_MY* GetCurrentTeb()
{
	return reinterpret_cast<TEB_MY*>(
#ifdef _M_X64
		__readgsqword(offsetof(NT_TIB, Self))
#elif _M_IX86
		__readfsdword(offsetof(NT_TIB, Self))
#else
#error "architecture unsupported"
#endif
		);
}
PEB_MY* GetCurrentPEB()
{
	return reinterpret_cast<PEB_MY*>(GetCurrentTeb()->ProcessEnvironmentBlock);
}


bool CAnticheatManager::GlobalFlagsCheck()
{
	auto pImageBase = (PBYTE)m_hInstance;
	if (!pImageBase)
		return false;

	auto pIDH = (PIMAGE_DOS_HEADER)pImageBase;
	if (!pIDH || pIDH->e_magic != IMAGE_DOS_SIGNATURE)
		return false;

	auto pINH = (PIMAGE_NT_HEADERS)(pImageBase + pIDH->e_lfanew);
	if (!pINH || pINH->Signature != IMAGE_NT_SIGNATURE)
		return false;

	auto pImageLoadConfigDirectory = (PIMAGE_LOAD_CONFIG_DIRECTORY)(pImageBase + pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress);
	if (pImageLoadConfigDirectory->GlobalFlagsClear)
	{
		CCheatDetectQueueMgr::Instance().AppendDetection(DEBUG_DETECT_GLOBALFLAGS, 0, "");
		return true;
	}

	return false;
}

bool CAnticheatManager::SharedUserDataCheck()
{
	auto pUserSharedData = (KUSER_SHARED_DATA *)0x7FFE0000; // The fixed user mode address of KUSER_SHARED_DATA

	BOOLEAN KdDebuggerEnabled = (pUserSharedData->KdDebuggerEnabled & 0x1) == 0x1;
	BOOLEAN KdDebuggerNotPresent = (pUserSharedData->KdDebuggerEnabled & 0x2) == 0x0;

	if (KdDebuggerEnabled || !KdDebuggerNotPresent)
	{
		CCheatDetectQueueMgr::Instance().AppendDetection(DEBUG_DETECT_KERNEL, 0, "");
		return true;
	}

	return false;
}

bool CAnticheatManager::DebugObjectHandleCheck()
{
	typedef NTSTATUS(WINAPI* TNtQueryInformationProcess)(HANDLE a, ULONG b, PVOID c, ULONG d, PULONG e);

	auto hNtdll = LoadLibraryA(XOR("ntdll.dll"));
	if (!hNtdll)
		return false;

	auto pNtQueryInformationProcess = GetProcAddress(hNtdll, XOR("NtQueryInformationProcess"));
	if (!pNtQueryInformationProcess)
		return false;
	auto NtQueryInformationProcess = (TNtQueryInformationProcess)pNtQueryInformationProcess;

	auto hDebugObject = HANDLE(INVALID_HANDLE_VALUE);
	auto ntStatus = NtQueryInformationProcess(GetCurrentProcess(), /* ProcessDebugObjectHandle */ 30, &hDebugObject, sizeof(HANDLE), NULL);
	if (!NT_SUCCESS(ntStatus))
		return false;

	if (ntStatus != STATUS_PORT_NOT_SET)
	{
		CCheatDetectQueueMgr::Instance().AppendDetection(DEBUG_DETECT_DBGOBJ_1, 0, "");
		return true;
	}

	if (ntStatus == STATUS_PORT_NOT_SET && hDebugObject)
	{
		CCheatDetectQueueMgr::Instance().AppendDetection(DEBUG_DETECT_DBGOBJ_2, 0, "");
		return true;
	}

	return false;
}

bool CAnticheatManager::DebugCheckBugControl()
{
	typedef NTSTATUS(WINAPI* TNtQueryInformationProcess)(HANDLE a, ULONG b, PVOID c, ULONG d, PULONG e);

	auto hNtdll = LoadLibraryA(XOR("ntdll.dll"));
	if (!hNtdll)
		return false;

	auto pNtQueryInformationProcess = GetProcAddress(hNtdll, XOR("NtQueryInformationProcess"));
	if (!pNtQueryInformationProcess)
		return false;
	auto NtQueryInformationProcess = (TNtQueryInformationProcess)pNtQueryInformationProcess;

	auto dwBugCheck = 0UL;
	auto ntStatus = NtQueryInformationProcess(GetCurrentProcess(), /* ProcessDebugObjectHandle */ 30, (PVOID)&dwBugCheck, sizeof(dwBugCheck), &dwBugCheck);
	if (ntStatus == STATUS_PORT_NOT_SET && dwBugCheck != 4)
	{
		CCheatDetectQueueMgr::Instance().AppendDetection(DEBUG_DETECT_BUGCHECK, 0, "");
		return true;
	}

	return false;
}

bool CAnticheatManager::SimpleDebugCheck()
{
	typedef NTSTATUS(WINAPI* TNtQueryInformationProcess)(HANDLE a, ULONG b, PVOID c, ULONG d, PULONG e);

	auto hNtdll = LoadLibraryA(XOR("ntdll.dll"));
	if (!hNtdll)
		return false;

	auto pNtQueryInformationProcess = GetProcAddress(hNtdll, XOR("NtQueryInformationProcess"));
	if (!pNtQueryInformationProcess)
		return false;
	auto NtQueryInformationProcess = (TNtQueryInformationProcess)pNtQueryInformationProcess;

	auto pPEB = GetCurrentPEB();
	if (pPEB && pPEB->BeingDebugged)
	{
		CCheatDetectQueueMgr::Instance().AppendDetection(DEBUG_DETECT_PEB, 0, "");
		return true;
	}

	auto dwFlags = 0UL;
	auto dwReturnSize = 0UL;
	if (NT_SUCCESS(NtQueryInformationProcess(GetCurrentProcess(), /* ProcessDebugFlags */ 31, &dwFlags, sizeof(dwFlags), &dwReturnSize)))
	{
		if (dwReturnSize != sizeof(dwFlags) || dwFlags == 0)
		{
			CCheatDetectQueueMgr::Instance().AppendDetection(DEBUG_DETECT_FLAGS, 0, "");
			return true;
		}
	}

	return false;
}

