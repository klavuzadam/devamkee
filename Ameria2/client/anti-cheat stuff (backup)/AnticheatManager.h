#pragma once
#include "CheatQuarentineManager.h"
#include "WMI_Monitor.h"
#include "NTDefinations.h"

enum ENetworkScanIDs
{
	NET_SCAN_ID_NULL,
	NET_SCAN_ID_PROCESS_HASH,
	NET_SCAN_ID_MAX
};

typedef struct _MODULE_SECTIONS_HASH_DATA
{
	DWORD dwModuleBase;
	WCHAR wszModuleName[1024];

	DWORD dwPeHeaderHash;
	DWORD dwTextHash;
	DWORD dwRDataHash;
	DWORD dwEDataHash;
	DWORD dwRsrcHash;
	DWORD dwRelocHash;
} SModuleSectionsHashCtx, *PModuleSectionsHashCtx;

class CAnticheatManager : public CSingleton <CAnticheatManager>
{
public:
	CAnticheatManager();
	virtual ~CAnticheatManager();

	DWORD ServiceMessageBox(const std::string & szTitle, const std::string & szMessage, WORD wType);
	bool IsFileExist(const std::string & szFileName);
	bool IsDirExist(const std::string & szDirName);
	
	std::string GetMd5(const BYTE* pData, std::size_t nSize);
	std::string GetFileMd5(const std::string & szName);

	void InitializeAnticheatRoutines(HINSTANCE hInstance, HWND hWnd);
	void FinalizeAnticheatRoutines();

	void InitializeExceptionFilters();
	void RemoveExceptionHandlers();

	void InitScreenProtection();
	void RemoveScreenProtection();
	void CheckScreenProtection();

	bool IsBlockedStartAddress(DWORD dwThreadId, DWORD dwStartAddress, LPDWORD pdwViolationID, std::string * pstrViolationOwnerModule);

	void InitializeAnticheatWatchdog();
	void RemoveAnticheatWatchdog();
	UINT_PTR GetAnticheatWatchdogTimerID() const { return m_iAnticheatWatchdogTimerID; };
	void SendWatchdogSignal(bool bStat) { m_bWatchdogStatus = bStat; };
	bool GetWatchdogStatus() const { return m_bWatchdogStatus; };

	bool InitializeMemoryWatchdogs();
	bool IsMemoryWatchdogsTriggered();

	void InitializeDumpProtection();
	bool IsMemoryDumped();

	bool BlockAccess(HANDLE hTarget);
	bool DenyProcessAccess();
	bool DenyThreadAccess();

	bool SimpleDebugCheck();
	bool DebugCheckBugControl();
	bool DebugObjectHandleCheck();
	bool SharedUserDataCheck();
	bool GlobalFlagsCheck();

	bool InitializeWindowWatcherThread();
	bool DestroyWindowWatcher();
	void RegisterWindowHookHandler(HWINEVENTHOOK hook) { m_hWndHandlerHook = hook; };
	DWORD GetWindowWatcherThreadId() { return m_dwWndCheckThreadId; };

	void CheckMainFolderFiles();
	void CheckMilesFolderForMilesPlugins();
	void CheckLibFolderForPythonLibs();
	void CheckYmirFolder();

	void CheckMappedFiles();

	void CreateProcessHashList();
	void AppendProcessHandle(HANDLE hProcess);
	void ScanProcess(HANDLE hProcess);
	void CloseUselessProcessHandles();
	void ParseCheatBlacklist(const std::string & szContent);

	void InitializeRawWindowHook();
	void DestroyRawWindowHook();

	void InitializeWindowMessageHook();
	void DestroyWindowMessageHook();

	void OnKeyPress(int iKeyIdx, bool bStatus);
	bool GetKeyStatus(int iKeyIdx);

	HWND GetMainWindow() { return m_hWnd; };
	HINSTANCE GetMainInstance() { return m_hInstance; };
	WNDPROC GetWindowMsgProc() { return m_wndpOldProc; };

	void SaveModuleSectionHashList(std::shared_ptr <SModuleSectionsHashCtx> vHashList)
	{
		m_vModuleSectionHashContainer.push_back(vHashList);
	}
	std::vector < std::shared_ptr <SModuleSectionsHashCtx> > GetModuleSectionHashContainer()
	{
		return m_vModuleSectionHashContainer;
	}

	void InitSectionHashes();
	bool HasCorruptedModuleSection();

protected:
	bool ActivateScreenProtection(bool bEnabled);
	bool GetScreenProtectionStatus();

private:
	std::shared_ptr < CCheatQuarentineMgr > m_pQuerantineMgr;
	std::shared_ptr < CWMI_Monitor >		m_pWmiMonitorMgr;

	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	WNDPROC		m_wndpOldProc;
	HHOOK		m_hkMessageHook;

	std::map <int /* iKeyIdx */, bool /* bStatus */> m_mapKeyStatusList;

	std::vector < std::shared_ptr <SModuleSectionsHashCtx> > m_vModuleSectionHashContainer;

	UINT_PTR		m_iAnticheatWatchdogTimerID;
	bool			m_bWatchdogStatus;

	HWINEVENTHOOK	m_hWndHandlerHook;
	HANDLE			m_hWndCheckThread;
	DWORD			m_dwWndCheckThreadId;
	HANDLE			m_hWndThreadWaitObj;

	HANDLE			m_hSyncProc;
	HANDLE			m_hSyncWaitObj;

	std::map <HWND, bool>	m_mScreenProtectionStatus;

	LPVOID					m_pDumpGuardMem;
	std::vector < LPVOID >	m_pMemoryWatchdogs;
	std::vector < LPVOID >	m_pMemoryDummyPages;

	std::vector < HANDLE >	m_vProcessHandles;
};

