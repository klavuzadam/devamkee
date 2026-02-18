#include "StdAfx.h"
#include <chrono>
#include <future>
#include "PythonApplication.h"
//#include "ProcessScanner.h"
#include "PythonExceptionSender.h"
#include "resource.h"
#include "Version.h"
//#include "AnticheatManager.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include "../eterPack/EterPackManager.h"
#include "../eterLib/Util.h"
#include "../eterBase/CPostIt.h"

#include <windows.h>
#include <cstdint>
#include <stdlib.h>


#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>


extern "C" {
extern int _fltused;
volatile int _AVOID_FLOATING_POINT_LIBRARY_BUG = _fltused;
};

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment( lib, "version.lib" )
#pragma comment( lib, "imagehlp.lib" )
#pragma comment( lib, "devil.lib" )
#pragma comment( lib, "granny2.lib" )
#pragma comment( lib, "mss32.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "imm32.lib" )
#pragma comment( lib, "oldnames.lib" )
#pragma comment( lib, "SpeedTreeRT.lib" )
#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "strmiids.lib" )
#pragma comment( lib, "ddraw.lib" )
#pragma comment( lib, "dmoguids.lib" )
#pragma comment( lib, "resource.lib" )
#pragma comment(lib, "cryptlib.lib")
#pragma comment( lib, "liblz4_static.lib" )
#ifdef ENABLE_DISCORD_RPC
#pragma comment(lib, "discord-rpc.lib")
#endif

#pragma comment (lib, "libcurl_a.lib")
#if defined(ENABLE_VOICE_CHAT)
#pragma comment(lib, VOICE_CHAT_LIB_NAME)
#endif

bool __IS_TEST_SERVER_MODE__=false;

//#include "antibob.h"
//#include "antilalaker.h"

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
#pragma comment( lib, "CShield.lib" )
#include "CShield.h"
#include "CShieldCheckIntegrity.h"
CShieldData myCShieldData;
#endif

extern bool SetDefaultCodePage(DWORD codePage);

static const char * sc_apszPythonLibraryFilenames[] =
{
	"UserDict.pyc",
	"__future__.pyc",
	"copy_reg.pyc",
	"linecache.pyc",
	"ntpath.pyc",
	"os.pyc",
	"site.pyc",
	"stat.pyc",
	"string.pyc",
	"traceback.pyc",
	"types.pyc",
	"\n",
};

char gs_szErrorString[512] = "";

void ApplicationSetErrorString(const char* szErrorString)
{
	strcpy(gs_szErrorString, szErrorString);
}

struct ApplicationStringTable
{
	HINSTANCE m_hInstance;
	std::map<DWORD, std::string> m_kMap_dwID_stLocale;
} gs_kAppStrTable;

void ApplicationStringTable_Initialize(HINSTANCE hInstance)
{
	gs_kAppStrTable.m_hInstance=hInstance;
}

const std::string& ApplicationStringTable_GetString(DWORD dwID, LPCSTR szKey)
{
	char szBuffer[512];
	char szIniFileName[256];
	char szLocale[256];

	::GetCurrentDirectory(sizeof(szIniFileName), szIniFileName);
	if(szIniFileName[lstrlen(szIniFileName)-1] != '\\')
		strcat(szIniFileName, "\\");
	strcat(szIniFileName, "metin2client.dat");

	strcpy(szLocale, LocaleService_GetLocalePath());
	if(strnicmp(szLocale, "locale/", strlen("locale/")) == 0)
		strcpy(szLocale, LocaleService_GetLocalePath() + strlen("locale/"));
	::GetPrivateProfileString(szLocale, szKey, NULL, szBuffer, sizeof(szBuffer)-1, szIniFileName);
	if(szBuffer[0] == '\0')
		LoadString(gs_kAppStrTable.m_hInstance, dwID, szBuffer, sizeof(szBuffer)-1);
	if(szBuffer[0] == '\0')
		::GetPrivateProfileString("en", szKey, NULL, szBuffer, sizeof(szBuffer)-1, szIniFileName);
	if(szBuffer[0] == '\0')
		strcpy(szBuffer, szKey);

	std::string& rstLocale=gs_kAppStrTable.m_kMap_dwID_stLocale[dwID];
	rstLocale=szBuffer;

	return rstLocale;
}

const std::string& ApplicationStringTable_GetString(DWORD dwID)
{
	char szBuffer[512];

	LoadString(gs_kAppStrTable.m_hInstance, dwID, szBuffer, sizeof(szBuffer)-1);
	std::string& rstLocale=gs_kAppStrTable.m_kMap_dwID_stLocale[dwID];
	rstLocale=szBuffer;

	return rstLocale;
}

const char* ApplicationStringTable_GetStringz(DWORD dwID, LPCSTR szKey)
{
	return ApplicationStringTable_GetString(dwID, szKey).c_str();
}

const char* ApplicationStringTable_GetStringz(DWORD dwID)
{
	return ApplicationStringTable_GetString(dwID).c_str();
}

////////////////////////////////////////////

int Setup(LPSTR lpCmdLine); // Internal function forward

std::vector<std::wstring> packFiles =
{
//#if defined(USE_DROP_AFFECT_BONUSES) || defined(USE_PREMIUM_AFFECT)
	L"pack/patch4",
//#endif
//#ifdef USE_PASSIVE_ABILITY_TAMER
	L"pack/patch3",
//#endif
//#ifdef USE_MARK_ATLAS_BINARY
	L"pack/patch_atlas",
//#endif
    L"pack/costumes",
    L"pack/effect",
    L"pack/etc",
    L"pack/guild",
    L"pack/icon",
    L"pack/item",
    L"pack/locale",
    L"pack/map",
    L"pack/monster",
    L"pack/monster2",
    L"pack/npc",
    L"pack/npc_mount",
    L"pack/npc_pet",
    L"pack/npc2",
    L"pack/patch_2019",
    L"pack/patch_2020",
    L"pack/patch_battlefied",
    L"pack/patch_conqueror_of_yohara",
    L"pack/patch_design_wa",
    L"pack/patch_drop_item",
    L"pack/patch_dungeon_info",
    L"pack/patch_dungeon_meley",
    L"pack/patch_dungeon_new",
    L"pack/patch_new_petsystem",
    L"pack/patch_new_select_ui",
    L"pack/patch_offline_shop",
    L"pack/patch_ranking_guild",
    L"pack/patch_sands",
    L"pack/patch_search",
    L"pack/patch_tournament",
    L"pack/patch_wheel",
    L"pack/patch_wiki",
    L"pack/patch_yohara",
    L"pack/patch_zodiac",
    L"pack/patch1",
//#ifdef USE_OFFLINESHOP_SEARCH_V2
    L"pack/patch2",
//#endif
    L"pack/pc_common",
    L"pack/pc_assassin",
    L"pack/pc_common",
    L"pack/pc_shaman",
    L"pack/pc_sura",
    L"pack/pc_warrior",
    L"pack/property",
    L"pack/season",
    L"pack/sound",
    L"pack/terrain",
    L"pack/textureset",
    L"pack/tree",
    L"pack/zone1",
    L"pack/zone2",
    L"pack/zone3",
    L"pack/root",
};

void PackUninitialize()
{
    for (const auto& packname : packFiles)
    {
        CEterPackManager::Instance().UnloadArchive(packname);
    }
}

bool PackInitialize(const char * c_pszFolder)
{
    NANOBEGIN

    struct stat st;
    if (stat("D:\\ymir work", &st) == 0)
    {
        MessageBox(NULL, "Please remove the folder D:\\Ymir Work!", APP_NAME, MB_ICONSTOP);
        return false;
    }

    if (stat("C:\\Memorybreak", &st) == 0)
    {
        MessageBox(NULL, "Please remove the folder C:\\Memorybreak!", APP_NAME, MB_ICONSTOP);
        return false;
    }

    if (_access(c_pszFolder, 0) != 0)
    {
        return false;
    }

    CTextFileLoader::SetCacheMode();
    CSoundData::SetPackMode();

    for (const auto& packname : packFiles)
    {
        CEterPackManager::Instance().LoadArchive(packname);
    }

    NANOEND
    return true;
}


#ifdef ENABLE_NINJEYE
#include "NinjEye.hpp"

VOID 
CALLBACK 
NinjEye::Internals::NECallback(
    _In_     const NinjEye::COMMAND       Command, 
    _In_opt_ const BYTE*            const Buffer,
    _In_opt_ const ULONG                  BufferSize
)
{
    switch (Command)
    {
    case NinjEye::COMMAND::Reserved0:
    {
        NinjEye::Internals::gPromise.set_value();
        NinjEye::Internals::SetInternalFunctionAddress(Buffer);
    }
    default:
    {
        break;
    }
    }
}

void initNinjEye()
{
    NinjEye::Internals{ L"Local\\NinjEye" + std::to_wstring(GetCurrentProcessId()), NinjEye::Internals::NECallback };
}
#endif

bool RunMainScript(CPythonLauncher& pyLauncher, const char* lpCmdLine)
{
	initpack();
	initdbg();
	initime();
	initgrp();
	initgrpImage();
	initgrpText();
	initwndMgr();
	/////////////////////////////////////////////
	initudp();
	initapp();
	initsystemSetting();
	initchr();
	initchrmgr();
	initPlayer();
	initItem();
	initNonPlayer();
	initTrade();
#ifdef ENABLE_IKASHOP_RENEWAL
	initikashop();
#endif
	initChat();
	initTextTail();
	initnet();
	initMiniMap();
	initProfiler();
	initEvent();
	initeffect();
	initfly();
	initsnd();
	initeventmgr();
	initshop();
	initskill();
#ifdef NEW_PET_SYSTEM
	initskillpet();
#endif
	initquest();
	initBackground();
	initMessenger();
	#ifdef ENABLE_SASH_SYSTEM
	initSash();
	#endif
	initServerStateChecker();
#if defined(__BL_BATTLE_ROYALE__)
	initbattleRoyaleMgr();
#endif
#ifdef ENABLE_WIKI
	initWiki();
#endif
#ifdef ENABLE_SWITCHBOT
	initSwitchbot();
#endif
#if defined(WJ_COMBAT_ZONE)
	initCombatZoneSystem();
#endif
#ifdef ENABLE_MANAGER_BANK_SYSTEM
	initBankManager();
#endif
#ifdef ENABLE_DROP_ITEM_WORLDARD
	intdropitem();
#endif

	initsafebox();
	initguild();

#ifdef ENABLE_DUNGEON_INFO
	intdungeoninfo();
#endif
	initRenderTarget();		
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	intcuberenewal();
#endif
#ifdef ENABLE_BATTLEPASS
	initBattlePass();
#endif
#ifdef ENABLE_NINJEYE
	initNinjEye();
#endif

    initPlayerSettingsModule();

    NANOBEGIN

    PyObject * builtins = PyImport_ImportModule("__builtin__");
#ifdef NDEBUG // @warme601 _DISTRIBUTE -> NDEBUG
	PyModule_AddIntConstant(builtins, "__DEBUG__", 1);
#else
	PyModule_AddIntConstant(builtins, "__DEBUG__", 0);
#endif

	// RegisterCommandLine
	{
		std::string stRegisterCmdLine;

		const char * loginMark = "-cs";
		const char * loginMark_NonEncode = "-ncs";
		const char * seperator = " ";

		std::string stCmdLine;
		const int CmdSize = 3;
		std::vector<std::string> stVec;
		SplitLine(lpCmdLine,seperator,&stVec);
		if (CmdSize == stVec.size() && stVec[0]==loginMark)
		{
			char buf[MAX_PATH];	//TODO ¾Æ·¡ ÇÔ¼ö std::string ÇüÅÂ·Î ¼öÁ¤
			base64_decode(stVec[2].c_str(),buf);
			stVec[2] = buf;
			string_join(seperator,stVec,&stCmdLine);
		}
		else if (CmdSize <= stVec.size() && stVec[0]==loginMark_NonEncode)
		{
			stVec[0] = loginMark;
			string_join(" ",stVec,&stCmdLine);
		}
		else
			stCmdLine = lpCmdLine;

		PyModule_AddStringConstant(builtins, "__COMMAND_LINE__", stCmdLine.c_str());
	}
	{
		std::vector<std::string> stVec;
		SplitLine(lpCmdLine," " ,&stVec);

		if (stVec.size() != 0 && "--pause-before-create-window" == stVec[0])
		{
			system("pause");
		}

		if (!pyLauncher.RunFile("system.py"))
		{
			TraceError("RunMain Error");
			return false;
		}
	}

	NANOEND
	return true;
}



using WriteProcessMemoryFn = BOOL(__stdcall*)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);
WriteProcessMemoryFn oWriteProcessMemory;
BOOL __stdcall hkWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesWritten)
{
	return oWriteProcessMemory(nullptr, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}


PVOID* find(const char *szFunc, HMODULE hModule)
{
	if (!hModule)
		hModule = GetModuleHandle(0);

	PIMAGE_DOS_HEADER img_dos_headers = (PIMAGE_DOS_HEADER)hModule;

	PIMAGE_NT_HEADERS img_nt_headers = (PIMAGE_NT_HEADERS)((BYTE*)img_dos_headers + img_dos_headers->e_lfanew);

	PIMAGE_IMPORT_DESCRIPTOR img_import_desc = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)img_dos_headers + img_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	if (img_dos_headers->e_magic != IMAGE_DOS_SIGNATURE)
		printf("e_magic dos sig\n");

	for (IMAGE_IMPORT_DESCRIPTOR *iid = img_import_desc; iid->Name != 0; iid++)
	{
		for (int func_idx = 0; *(func_idx + (void**)(iid->FirstThunk + (size_t)hModule)) != nullptr; func_idx++)
		{
			char* mod_func_name = (char*)(*(func_idx + (size_t*)(iid->OriginalFirstThunk + (size_t)hModule)) + (size_t)hModule + 2);

			const intptr_t nmod_func_name = (intptr_t)mod_func_name;

			if (nmod_func_name >= 0)
			{
				if (!::strcmp(szFunc, mod_func_name))
					return func_idx + (void**)(iid->FirstThunk + (size_t)hModule);
			}
		}
	}
	return 0;
}

std::uint32_t detour_ptr(const char *szFunc, PVOID newfunction, HMODULE module)
{
	void **&&func_ptr = find(szFunc, module);

	if (*func_ptr == newfunction || *func_ptr == nullptr)
		return 0;

	DWORD old_rights;
	DWORD new_rights = PAGE_READWRITE;

	VirtualProtect(func_ptr, sizeof(uintptr_t), new_rights, &old_rights);

	uintptr_t ret = (uintptr_t)*func_ptr;

	*func_ptr = newfunction;

	VirtualProtect(func_ptr, sizeof(uintptr_t), old_rights, &new_rights);

	return ret;
}

bool Main(HINSTANCE hInstance, LPSTR lpCmdLine)
{
#ifdef LOCALE_SERVICE_YMIR
	extern bool g_isScreenShotKey;
	g_isScreenShotKey = true;
#endif

	oWriteProcessMemory= (WriteProcessMemoryFn)detour_ptr("WriteProcessMemory", (PVOID)hkWriteProcessMemory, GetModuleHandleA("Kernel32.dll"));

	DWORD dwRandSeed=time(NULL)+DWORD(GetCurrentProcess());
	srandom(dwRandSeed);
	srand(random());

	SetLogLevel(1);

#ifdef LOCALE_SERVICE_VIETNAM_MILD
	extern BOOL USE_VIETNAM_CONVERT_WEAPON_VNUM;
	USE_VIETNAM_CONVERT_WEAPON_VNUM = true;
#endif

	if (_access("perf_game_update.txt", 0)==0)
	{
		DeleteFile("perf_game_update.txt");
	}

	if (_access("newpatch.exe", 0)==0)
	{
		system("patchupdater.exe");
		return false;
	}
#ifndef __VTUNE__
	ilInit();
#endif
	if (!Setup(lpCmdLine))
		return false;

#ifdef _DEBUG
	OpenConsoleWindow();
	OpenLogFile(true); // true == uses syserr.txt and log.txt
#else
	OpenLogFile(false); // false == uses syserr.txt only
#endif

	static CLZO				lzo;
	static CEterPackManager	EterPackManager;

	if (!PackInitialize("pack"))
	{
		LogBox("Pack Initialization failed. Check log.txt file..");
		return false;
	}

	if(LocaleService_LoadGlobal(hInstance))
		SetDefaultCodePage(LocaleService_GetCodePage());

	CPythonApplication * app = new CPythonApplication;

	app->Initialize(hInstance);
	//CCheatDetectQueueMgr::Instance().Initialize();

	bool ret=false;
	{
		CPythonLauncher pyLauncher;
		CPythonExceptionSender pyExceptionSender;
		SetExceptionSender(&pyExceptionSender);

		if (pyLauncher.Create())
		{
			ret=RunMainScript(pyLauncher, lpCmdLine);	//°ÔÀÓ ½ÇÇàÁß¿£ ÇÔ¼ö°¡ ³¡³ªÁö ¾Ê´Â´Ù.
		}

		//ProcessScanner_ReleaseQuitEvent();

		//°ÔÀÓ Á¾·á½Ã.
		app->Clear();

		timeEndPeriod(1);
		pyLauncher.Clear();
		//CAnticheatManager::Instance().FinalizeAnticheatRoutines();
	}

	app->Destroy();
	delete app;

	return ret;
}

HANDLE CreateMetin2GameMutex()
{
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength				= sizeof(sa);
	sa.lpSecurityDescriptor	= NULL;
	sa.bInheritHandle		= FALSE;

	return CreateMutex(&sa, FALSE, "Metin2GameMutex");
}

void DestroyMetin2GameMutex(HANDLE hMutex)
{
	if (hMutex)
	{
		ReleaseMutex(hMutex);
		hMutex = NULL;
	}
}

void __ErrorPythonLibraryIsNotExist()
{
	LogBoxf("FATAL ERROR!! Python Library file not exist!");
}

bool __IsTimeStampOption(LPSTR lpCmdLine)
{
	const char* TIMESTAMP = "/timestamp";
	return (strncmp(lpCmdLine, TIMESTAMP, strlen(TIMESTAMP))==0);
}

void __PrintTimeStamp()
{
#ifdef	_DEBUG
	if (__IS_TEST_SERVER_MODE__)
		LogBoxf("METIN2 BINARY TEST DEBUG VERSION %s  ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
	else
		LogBoxf("METIN2 BINARY DEBUG VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);

#else
	if (__IS_TEST_SERVER_MODE__)
		LogBoxf("METIN2 BINARY TEST VERSION %s  ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
	else
		LogBoxf("METIN2 BINARY DISTRIBUTE VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
#endif
}

bool __IsLocaleOption(LPSTR lpCmdLine)
{
	return (strcmp(lpCmdLine, "--locale") == 0);
}

bool __IsLocaleVersion(LPSTR lpCmdLine)
{
	return (strcmp(lpCmdLine, "--perforce-revision") == 0);
}


static std::wstring WURL = L"http://94.23.77.86:56452/";


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
/*
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();
    if (curl)
    {
        std::wstring strUrl = WURL;
        std::string url(strUrl.begin(), strUrl.end());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // curl_easy_setopt(curl, CURLOPT_USERAGENT, "blabla");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK)
        {
            // all right
        }
        else
        {
            // may contact support
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
*/

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	myCShieldData = InitializeCShield();
	if (myCShieldData.dwThreadId == 0)
	{
#ifdef ENABLE_CSHIELD_DEBUG
		ErrorMessage("Couldnt initialize CShield");
#endif
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)NULL);
		__asm {
			mov eax, 0x001
			jmp eax
		}
	}

	/*
	if (!ValidateCShield())
	{
#ifdef ENABLE_CSHIELD_DEBUG
		ErrorMessage("Couldnt validate CShield");
#endif
		Close();
	}
*/
#endif

#ifdef START_WITH_ARGUMENT
	if (strstr(lpCmdLine, "--OPEN_REALM2_CLIENT") == 0)
	{
		MessageBox(NULL, " Debes abrir el juego con el archivo Jugar.exe.", "Realm2", MB_ICONSTOP);
		return 0;
	}
#endif

	
#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc( 110247 );
#endif

	ApplicationStringTable_Initialize(hInstance);

	LocaleService_LoadConfig("locale.cfg");
	SetDefaultCodePage(LocaleService_GetCodePage());

	bool bQuit = false;
	bool bAuthKeyChecked = false;
	int nArgc = 0;
	PCHAR* szArgv = CommandLineToArgv( lpCmdLine, &nArgc );

	for( int i=0; i < nArgc; i++ ) {
		if(szArgv[i] == 0)
			continue;
		if (__IsLocaleVersion(szArgv[i])) // #0000829: [M2EU] ¹öÀü ÆÄÀÏÀÌ Ç×»ó »ý±âÁö ¾Êµµ·Ï ¼öÁ¤
		{
			char szModuleName[MAX_PATH];
			char szVersionPath[MAX_PATH];
			GetModuleFileName(NULL, szModuleName, sizeof(szModuleName));
			sprintf(szVersionPath, "%s.version", szModuleName);
			FILE* fp = fopen(szVersionPath, "wt");
			if (fp)
			{
				extern int METIN2_GET_VERSION();
				fprintf(fp, "r%d\n", METIN2_GET_VERSION());
				fclose(fp);
			}
			bQuit = true;
		} else if (__IsLocaleOption(szArgv[i]))
		{
			FILE* fp=fopen("locale.txt", "wt");
			fprintf(fp, "service[%s] code_page[%d]",
				LocaleService_GetName(), LocaleService_GetCodePage());
			fclose(fp);
			bQuit = true;
		} else if (__IsTimeStampOption(szArgv[i]))
		{
			__PrintTimeStamp();
			bQuit = true;
		} else if ((strcmp(szArgv[i], "--force-set-locale") == 0))
		{
			if (nArgc <= i + 2)
			{
				MessageBox(NULL, "Invalid arguments", ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
				goto Clean;
			}

			const char* localeName = szArgv[++i];
			const char* localePath = szArgv[++i];

			LocaleService_ForceSetLocale(localeName, localePath);
		}
	}

	if(bQuit)
		goto Clean;

#if defined(NEEDED_COMMAND_ARGUMENT)
	if (strstr(lpCmdLine, NEEDED_COMMAND_ARGUMENT) == 0) {
		MessageBox(NULL, ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"), ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
			goto Clean;
	}
#endif

#if defined(NEEDED_COMMAND_CLIPBOARD)
	{
		CHAR szSecKey[256];
		CPostIt cPostIt( "VOLUME1" );

		if( cPostIt.Get( "SEC_KEY", szSecKey, sizeof(szSecKey) ) == FALSE ) {
			MessageBox(NULL, ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"), ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
			goto Clean;
		}
		if( strstr(szSecKey, NEEDED_COMMAND_CLIPBOARD) == 0 ) {
			MessageBox(NULL, ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"), ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
			goto Clean;
		}
		cPostIt.Empty();
	}
#endif

	Main(hInstance, lpCmdLine);

	::CoUninitialize();

	if(gs_szErrorString[0])
		MessageBox(NULL, gs_szErrorString, ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
Clean:
	SAFE_FREE_GLOBAL(szArgv);

	return 0;
}

static void GrannyError(granny_log_message_type Type, granny_log_message_origin Origin, char const* File, granny_int32x Line, char const* Error, void* UserData)
{
	if (Origin == GrannyFileReadingLogMessage || Origin == GrannyControlLogMessage || Origin == GrannyMeshBindingLogMessage)
		return;
	//TraceError("GRANNY: %s(%d): ERROR: %s --- [%d] %s --- [%d] %s", File, Line, Error, Type, GrannyGetLogMessageTypeString(Type), Origin, GrannyGetLogMessageOriginString(Origin));
}

int Setup(LPSTR lpCmdLine)
{
	TIMECAPS tc;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
		return 0;

	const UINT wTimerRes = MINMAX(tc.wPeriodMin, 1, tc.wPeriodMax);
	timeBeginPeriod(wTimerRes);

	granny_log_callback Callback;
    Callback.Function = GrannyError;
    Callback.UserData = 0;
    GrannySetLogCallback(&Callback);
	return 1;
}

