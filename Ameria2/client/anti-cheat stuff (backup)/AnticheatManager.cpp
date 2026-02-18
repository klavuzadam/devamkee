#include "StdAfx.h"
#include "AnticheatManager.h"
#include "CheatQueueManager.h"
#include "../eterBase/SimpleTimer.h"
#include "Locale_inc.h"

#include <XORstr.h>
#include <Wtsapi32.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <cryptopp/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

#undef CRYPTOPP_ENABLE_NAMESPACE_WEAK

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#pragma comment( lib, "psapi.lib" )
#pragma comment( lib, "wtsapi32.lib" )
#pragma comment( lib, "wbemuuid.lib" )
#pragma comment( lib, "Netapi32.lib" )


static auto gs_pWatchdogCheckTimer = std::make_unique <CSimpleTimer <std::chrono::milliseconds> >();

void NTAPI OnSyncWait(PVOID pParam, BOOLEAN)
{
	static auto bBlocked = false;
	if (bBlocked)
		return;

	if (gs_pWatchdogCheckTimer->diff() > 30000)
	{
		if (CAnticheatManager::Instance().GetWatchdogStatus() == false)
		{
			CCheatDetectQueueMgr::Instance().AppendDetection(WATCHDOG_SIGNAL_LOST, 0, "");
			bBlocked = true;
		}
		CAnticheatManager::Instance().SendWatchdogSignal(false);

		gs_pWatchdogCheckTimer->reset();
	}
}


CAnticheatManager::CAnticheatManager() :
	m_hWndThreadWaitObj(INVALID_HANDLE_VALUE)
{
}

CAnticheatManager::~CAnticheatManager()
{
}


void CAnticheatManager::InitializeAnticheatRoutines(HINSTANCE hInstance, HWND hWnd)
{
#ifndef ENABLE_ANTICHEAT
	return;
#endif

	m_pQuerantineMgr = std::make_shared<CCheatQuarentineMgr>();
	m_pWmiMonitorMgr = std::make_shared<CWMI_Monitor>();

	m_hInstance = hInstance;
	m_hWnd = hWnd;
	m_wndpOldProc = nullptr;
	m_hkMessageHook = nullptr;

 	m_mapKeyStatusList[0] = false;
 	m_mapKeyStatusList[1] = false;
 	m_mapKeyStatusList[2] = false;

#ifndef _DEBUG
	SimpleDebugCheck();
	DebugCheckBugControl();
	DebugObjectHandleCheck();
	SharedUserDataCheck();
	GlobalFlagsCheck();
#endif

	CheckMainFolderFiles();
	CheckMilesFolderForMilesPlugins();
	CheckLibFolderForPythonLibs();
	CheckYmirFolder();

///	CheckMappedFiles();

	BlockAccess(GetCurrentProcess());
	BlockAccess(GetCurrentThread());
	DenyProcessAccess();
	DenyThreadAccess();

	InitializeExceptionFilters();
///	InitializeDumpProtection();
	InitializeAnticheatWatchdog();

	/*
	// TODO: Add minimize and restore messages for block keyboard stuck
	InitializeWindowMessageHook();
	InitializeRawWindowHook();
	*/

	InitializeWindowWatcherThread();
	m_pWmiMonitorMgr->InitializeWMIWatcherThread();

	InitSectionHashes();
	CreateProcessHashList();

	m_hSyncProc = OpenProcess(SYNCHRONIZE, FALSE, GetCurrentProcessId());
	if (m_hSyncProc && m_hSyncProc != INVALID_HANDLE_VALUE)
	{
		gs_pWatchdogCheckTimer->reset();

		auto bRegisterWaitRet = RegisterWaitForSingleObject(&m_hSyncWaitObj, m_hSyncProc, OnSyncWait, nullptr, 5000, WT_EXECUTEDEFAULT);
		if (bRegisterWaitRet == false)
		{
			CCheatDetectQueueMgr::Instance().AppendDetection(SYNC_PROC_REGISTER_FAIL, GetLastError(), "");
		}
	}
}

void CAnticheatManager::FinalizeAnticheatRoutines()
{
#ifndef ENABLE_ANTICHEAT
	return;
#endif

	if (m_hSyncWaitObj && m_hSyncWaitObj != INVALID_HANDLE_VALUE)
		UnregisterWait(m_hSyncWaitObj);
	if (m_hSyncProc && m_hSyncProc != INVALID_HANDLE_VALUE)
		CloseHandle(m_hSyncProc);

	RemoveAnticheatWatchdog();
	RemoveExceptionHandlers();

	DestroyWindowWatcher();
	m_pWmiMonitorMgr->DestroyWMIMonitor();

	/*
	DestroyWindowMessageHook();
	DestroyRawWindowHook();
	*/
}


void CAnticheatManager::ParseCheatBlacklist(const std::string & szContent)
{
	int iScanID = -1;

	try
	{
		boost::property_tree::ptree root;
		std::istringstream iss(szContent);
		boost::property_tree::read_json(iss, root);

		/*
		for (ptree::const_iterator v = root.begin(); v != root.end(); ++v)
		{
			std::string key = v->first;
			auto value = v->second.get_value<std::string>();
			TraceError("key: %s value %s", key.c_str(), value.c_str());
		}
		*/

		auto headerTree = root.get_child(XOR("Header"));
		if (headerTree.empty())
		{
#ifdef _DEBUG
			TraceError("Blacklist header not found!");
#endif
			return;
		}

		auto scanId = headerTree.get<std::string>(XOR("ScanID"));
		auto rowCount = headerTree.get<std::string>(XOR("RowCount"));
		auto fieldCount = headerTree.get<std::string>(XOR("FieldCount"));

		if (scanId.empty() || rowCount.empty() || fieldCount.empty())
		{
#ifdef _DEBUG
			TraceError("Blacklist header context not found! ID: %s Row: %s Field: %s", scanId.c_str(), rowCount.c_str(), fieldCount.c_str());
#endif
			return;
		}

#ifdef _DEBUG
		TraceError("Blacklist process started! ScanID: %s Row Count: %s Field Count: %s", scanId.c_str(), rowCount.c_str(), fieldCount.c_str());
#endif

		iScanID = std::stoi(scanId);
		if (!(iScanID > NET_SCAN_ID_NULL && iScanID < NET_SCAN_ID_MAX))
		{
#ifdef _DEBUG
			TraceError("Unallowed scan ID: %d", iScanID);
#endif
			return;
		}

		auto nRowCount = std::stoi(rowCount);
		for (auto i = 1; i <= nRowCount; ++i)
		{
//			TraceError("Current row idx: %d", i);

			auto strCurrNode = std::to_string(i);
			auto currNode = root.get_child(strCurrNode);
			if (currNode.empty())
			{
#ifdef _DEBUG
				TraceError("Current node: %s is empty!", strCurrNode.c_str());
#endif
				continue;
			}

			switch (iScanID)
			{
				case NET_SCAN_ID_PROCESS_HASH:
				{
					auto strMd5Hash = currNode.get<std::string>(XOR("Md5Hash"));

	//				TraceError("Node process started! ScanID: %d Row: %u Hash: %s", iScanID, i, strMd5Hash.c_str());

					CCheatQuarentineMgr::Instance().AppendProcessMd5(strCurrNode, strMd5Hash, true);

	//				TraceError("Current node appended to quarentine!");

				} break;

				default:
	#ifdef _DEBUG
					TraceError("Unknown scan ID: %d", iScanID);
	#endif
					break;
			}
		}
	}
	catch (const std::exception & e)
	{
#ifdef _DEBUG
		TraceError("Blacklist json parse exception: %s", e.what());
#else
		UNREFERENCED_PARAMETER(e);
#endif
	}


	if (iScanID != -1)
	{
		CCheatQuarentineMgr::Instance().ProcessQuarentineList(iScanID, false);
	}
}


DWORD CAnticheatManager::ServiceMessageBox(const std::string & szTitle, const std::string & szMessage, WORD wType)
{
	BOOL bRet = FALSE;
	auto dwResponse = 0UL;

	bRet = WTSSendMessageA(
		WTS_CURRENT_SERVER_HANDLE,							// hServer
		WTSGetActiveConsoleSessionId(),						// ID for the console seesion (1)
		const_cast<LPSTR>(szTitle.c_str()),					// MessageBox Caption
		static_cast<DWORD>(strlen(szTitle.c_str())),		// Length of caption
		const_cast<LPSTR>(szMessage.c_str()),				// MessageBox Text
		static_cast<DWORD>(strlen(szMessage.c_str())),		// Length of text
		wType,												// Buttons, etc
		0,													// Timeout period in seconds (0 = infinite)
		&dwResponse,										// What button was clicked (if bWait == TRUE)
		FALSE												// bWait - Blocks until user click
	);

	return dwResponse;
}

bool CAnticheatManager::IsFileExist(const std::string & szFileName)
{
	auto dwAttrib = GetFileAttributesA(szFileName.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool CAnticheatManager::IsDirExist(const std::string & szDirName)
{
	auto dwAttrib = GetFileAttributesA(szDirName.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


std::string CAnticheatManager::GetMd5(const BYTE* pData, std::size_t nSize)
{
	std::string szOutput;

	CryptoPP::Weak::MD5 hash;
	uint8_t csum[CryptoPP::Weak::MD5::DIGESTSIZE];

	hash.CalculateDigest(csum, pData, nSize);
	CryptoPP::StringSource(csum, sizeof(csum), true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(szOutput))));

	std::transform(szOutput.begin(), szOutput.end(), szOutput.begin(), tolower);
	return szOutput;
}

std::string CAnticheatManager::GetFileMd5(const std::string & szName)
{
	std::string szOutput;

	CMappedFile pMappedFile;
	LPCVOID pMappedFileData;
	if (!pMappedFile.Create(szName.c_str(), &pMappedFileData, 0, 0))
		return szOutput;

	szOutput = GetMd5((const BYTE*)pMappedFileData, pMappedFile.Size());

	pMappedFile.Destroy();
	return szOutput;
}

