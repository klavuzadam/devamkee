#include "StdAfx.h"
#include "PythonNetworkStream.h"
#include "Packet.h"

#include "PythonGuild.h"
#include "PythonCharacterManager.h"
#include "PythonPlayer.h"
#include "PythonBackground.h"
#include "PythonMiniMap.h"
#include "PythonTextTail.h"
#include "PythonItem.h"
#include "PythonChat.h"
#include "PythonShop.h"
#include "PythonExchange.h"
#include "PythonQuest.h"
#include "PythonEventManager.h"
#include "PythonMessenger.h"
#include "PythonApplication.h"

#ifdef BATTLEPASS_WORLDARD
#include "PythonBattlePass.h"
#endif

#ifdef ENABLE_NINJEYE
#include "NinjEye.hpp"
#endif

#ifdef ENABLE_BADGE_NOTIFICATION_MANAGER
#include "BadgeNotificationManager.h"
#endif
#ifdef ENABLE_DUNGEON_INFO
#include "PythonDungeonInfo.h"
#endif

#include "../EterPack/EterPackManager.h"
#include "../gamelib/ItemManager.h"

#include "AbstractApplication.h"
#include "AbstractCharacterManager.h"
#include "InstanceBase.h"
#ifdef ENABLE_MANAGER_BANK_SYSTEM
#include "PythonManagerBank.h"
#endif
#include "ProcessCRC.h"

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
#include "CShield.h"
#include "CShieldCheckIntegrity.h"
#endif

#if defined(WJ_COMBAT_ZONE)
#include "PythonCombatZone.h"
#endif
//#include <XORstr.h>

#ifdef ENABLE_DROP_ITEM_WORLDARD
#include "PythonDropItem.h"
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
#include "PythonCubeRenewal.h"
#endif
#include <thread>
#include <chrono>
BOOL gs_bEmpireLanuageEnable = TRUE;

#ifdef ENABLE_DISCORD_RPC
#include <discord-rpc-3.4.0/discord_rpc.h>
#include "Discord.h"
#endif

static std::vector <std::string> vWhiteList = //Lista de la carpeta raiz
{
"Protect.dll",
"arrancarstart.bat",
"artpclnt.dll",
"Jugar.exe",
"bdvid32.dll",
"BETA-SERVER.bat",
"BGM",
"channel.inf",
"config.exe",
"config.ini",
"devil.dll",
"DSETUP.dll",
"errorlog.exe",
"ErrorLog.txt",
"FileArchiver.exe",
"granny2.dll",
"ijl15.dll",
"ilu.dll",
"is_dev.txt",
"lib",
"locale.cfg",
"log.txt",
"logininfo.xml",
"mark",
"metin2.cfg",
"metin2.ico",
"metin2client.exe",
"Metin2EU.ini",
"Microsoft.WindowsAPICodePack.dll",
"Microsoft.WindowsAPICodePack.Shell.dll",
"miles",
"mouse.cfg",
"mscoree.dll",
"MSS32.DLL",
"msvcp120.dll",
"msvcp60.dll",
"MSVCRTD.DLL",
"Options.txt",
"pack",
"Modulo1",
"pack_maker",
"patchw32.dll",
"PropertyGen.exe",
"prueba.bat",
"python22.dll",
"python27.dll",
"resource.dll",
"screenshot",
"shop.cfg",
"shops",
"SpeedTreeRT.dll",
"syserr.txt",
"tbb.dll",
"unicows.dll",
"start.py1",
"upload",
"font.cfg",
"switchbot.pkl"

};

static std::vector <std::string> vWhiteListPack = // Lista de la carpeta pack
{
"BGM",
"Effect",
"ETC",
"Green_dragon_valley",
"guild",
"icon",
"Index",
"indoordeviltower1",
"indoormonkeydungeon1",
"indoormonkeydungeon2",
"indoormonkeydungeon3",
"indoorspiderdungeon1",
"inventory_locked",
"item",
"mt2_patch_pascua",
"locale",
"metin2_map_devilscatacomb",
"metin2_patch_10th",
"metin2_patch_5th_armor",
"metin2_patch_acce",
"metin2_patch_acce_w",
"metin2_patch_attendance",
"metin2_patch_battlefied",
"metin2_patch_calendar",
"metin2_patch_common",
"metin2_patch_costume_soccer",
"metin2_patch_dance",
"metin2_patch_dawnmist_dungeon",
"metin2_patch_dawnmist_dungeon_mob",
"metin2_patch_discord",
"metin2_patch_dragon_rock",
"metin2_patch_dragon_rock_mobs",
"metin2_patch_dragon_rock_mobs_texcache",
"metin2_patch_dragon_rock_texcache",
"metin2_patch_ds",
"metin2_patch_dss_box",
"metin2_patch_dungeon_info",
"metin2_patch_e1",
"metin2_patch_easter1",
"metin2_patch_easter2016",
"metin2_patch_etc",
"metin2_patch_etc_costume1",
"metin2_patch_eu2",
"metin2_patch_eu3",
"metin2_patch_eu4",
"metin2_patch_event",
"metin2_patch_flame",
"metin2_patch_flame_dragonlair",
"metin2_patch_flame_dungeon",
"metin2_patch_guildrenewal",
"metin2_patch_guildwar",
"metin2_patch_guildwar_object",
"metin2_patch_halloween",
"metin2_patch_halloween_2018",
"metin2_patch_hide_costume",
"metin2_patch_kingdoomswar",
"metin2_patch_mineral",
"metin2_patch_minigame_okey",
"metin2_patch_monster_card",
"metin2_patch_mundi",
"metin2_patch_myshop_deco",
"metin2_patch_new_halloween",
"metin2_patch_new_halloween_weapon",
"metin2_patch_new_pet",
"metin2_patch_new_select_ui",
"metin2_patch_party",
"metin2_patch_pc3",
"metin2_patch_pc3_m",
"metin2_patch_pepsi",
"metin2_patch_pet1",
"metin2_patch_pet2",
"metin2_patch_privateshop",
"metin2_patch_public",
"metin2_patch_ramadan",
"metin2_patch_ramadan_costume",
"metin2_patch_sd",
"metin2_patch_search",
"metin2_patch_second_guildrenewal",
"metin2_patch_snow",
"metin2_patch_snow_dungeon",
"metin2_patch_summer",
"metin2_patch_user_situation",
"metin2_patch_valentine_pet",
"metin2_patch_w20_etc",
"metin2_patch_w20_sound",
"metin2_patch_w21_etc",
"metin2_patch_w21_mobs",
"metin2_patch_w21_mobs_m",
"metin2_patch_xmas",
"miki_p",
"Monster",
"monster2",
"new_petsystem",
"NPC",
"npc2",
"Outdoor",
"OutdoorA1",
"OutdoorA2",
"OutdoorA3",
"OutdoorB1",
"OutdoorB3",
"OutdoorC1",
"OutdoorC3",
"outdoordesert1",
"outdoorduel",
"outdoorempirebattle1",
"outdoorfielddungeon1",
"outdoorflame1",
"outdoorgmguildbuild",
"outdoorguild1",
"outdoorguild2",
"outdoorguild3",
"outdoormilgyo1",
"OutdoorSnow1",
"outdoort1",
"outdoort2",
"outdoort3",
"outdoort4",
"outdoortrent",
"outdoortrent02",
"outdoorwedding",
"patch1",
"PC",
"pc2",
"property",
"root",
"season1",
"season2",
"season3_eu",
"select_create",
"skipia_1",
"Sound",
"sound2",
"sound_m",
"Terrain",
"textureset",
"Tree",
"uiloading",
"uiscript",
"WeaponSkin",
"w_m_g",
"zone2",
"patch_arboreal",
"metin2_patch_drop_item",
"Zone",
"metin2_patch_cube_renewal",
"metin2_patch_guias",
"patch_dungeon_new",
"metin2_patch_multi_evolution",
"pack2019",
"pack_2020",
"metin2_patch_ranking_guild",
"metin2_patch_elements",
"metin2_patch_new_maps",
"metin2_patch_d_rework",
"patch_design_wa",
"metin2_patch_01",
"metin2_patch_carcaj",
"patch_desafio",
"metin2_patch_shining",
"metin2_patch_battlepass"

};

static std::vector <std::string> vWhiteListlib = // Lista de la carpeta lib
{
"abc.pyc",
"account_1.cfg",
"account_2.cfg",
"account_3.cfg",
"account_4.cfg",
"bisect.pyc",
"calendar.pyc",
"codecs.pyc",
"collections.pyc",
"copy.pyc",
"copy_reg.pyc",
"encodings",
"fnmatch.pyc",
"font.pyc",
"functools.pyc",
"genericpath.pyc",
"heapq.pyc",
"keyword.pyc",
"linecache.pyc",
"locale.pyc",
"ntpath.pyc",
"os.pyc",
"pyexpat.pyd",
"pyexpat_d.pdb",
"pyexpat_d.pyd",
"re.pyc",
"shutil.pyc",
"site.pyc",
"socket.pyc",
"sre_compile.pyc",
"sre_constants.pyc",
"sre_parse.pyc",
"stat.pyc",
"string.pyc",
"sysconfig.pyc",
"traceback.pyc",
"types.pyc",
"UserDict.pyc",
"warnings.pyc",
"weakref.pyc",
"xml",
"_abcoll.pyc",
"_locale.pyc",
"_socket.pyd",
"_strptime.pyc",
"_weakrefset.pyc",
"__future__.pyc",
"wa_ac.py",
"base64.pyc",
"struct.pyc",
};

static std::string GetRootPath()
{
	char szBuffer[MAX_PATH];
	if (GetModuleFileNameA(NULL, szBuffer, MAX_PATH))
	{
		std::string szPath = std::string(szBuffer);
		size_t nPos = szPath.find_last_of("\\/");
		szPath = szPath.substr(0, nPos);

		DWORD dwAttrib = GetFileAttributesA(szPath.c_str());
		if (szPath.empty() == false && dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
		{
			return szPath;
		}
	}
	return std::string("");
}

static void FileChecks()
{
	std::string szRootPath = GetRootPath();
	if (szRootPath.empty())
	{
		//		printf("Root path not found! Error: %u\n", GetLastError());
		return;
	}
	std::string szSearchPath = szRootPath + "/lib/*.*";

	WIN32_FIND_DATAA w32fd;
	HANDLE hFind = FindFirstFileA(szSearchPath.c_str(), &w32fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			std::string szCurrentItem = w32fd.cFileName;
			if (szCurrentItem != "." && szCurrentItem != "..")
			{
				//				printf("\tCurrent %s\n", szCurrentItem.c_str());

				if (std::find(vWhiteListlib.begin(), vWhiteListlib.end(), szCurrentItem) == vWhiteListlib.end()) // not whitelisted item
				{
					//					printf("\tUnknown file: %s\n", szCurrentItem.c_str());

					if (DeleteFileA(szCurrentItem.c_str()) == FALSE /* can not removed */ || _access(szCurrentItem.c_str(), 0) == 0 /* removed but still exist */)
					{
						std::string szWarn = "Unknown file detected: " + szCurrentItem + ", Delete this file.";
						MessageBoxA(0, szWarn.c_str(), 0, 0);

						PostQuitMessage(0);
					}
				}
			}
		} while (FindNextFileA(hFind, &w32fd));
		FindClose(hFind);
	}
}

static void FileCheckPack()
{
	std::string szRootPath = GetRootPath();
	if (szRootPath.empty())
	{
		//		printf("Root path not found! Error: %u\n", GetLastError());
		return;
	}
	std::string szSearchPath = szRootPath + "/pack/*.*";

	WIN32_FIND_DATAA w32fd;
	HANDLE hFind = FindFirstFileA(szSearchPath.c_str(), &w32fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			std::string szCurrentItem = w32fd.cFileName;
			if (szCurrentItem != "." && szCurrentItem != "..")
			{
				//				printf("\tCurrent %s\n", szCurrentItem.c_str());

				if (std::find(vWhiteListPack.begin(), vWhiteListPack.end(), szCurrentItem) == vWhiteListPack.end()) // not whitelisted item
				{
					//					printf("\tUnknown file: %s\n", szCurrentItem.c_str());

					if (DeleteFileA(szCurrentItem.c_str()) == FALSE /* can not removed */ || _access(szCurrentItem.c_str(), 0) == 0 /* removed but still exist */)
					{
						std::string szWarn = "Unknown file detected: " + szCurrentItem + ", Delete this file.";
						MessageBoxA(0, szWarn.c_str(), 0, 0);

						PostQuitMessage(0);
					}
				}
			}
		} while (FindNextFileA(hFind, &w32fd));
		FindClose(hFind);
	}
}

static void FileCheckRoutine()
{
	std::string szRootPath = GetRootPath();
	if (szRootPath.empty())
	{
		//		printf("Root path not found! Error: %u\n", GetLastError());
		return;
	}
	//	printf("Root: %s\n", szRootPath.c_str());
	std::string szSearchPath = szRootPath + "/*.*";

	WIN32_FIND_DATAA w32fd;
	HANDLE hFind = FindFirstFileA(szSearchPath.c_str(), &w32fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			std::string szCurrentItem = w32fd.cFileName;
			if (szCurrentItem != "." && szCurrentItem != "..")
			{
				//				printf("\tCurrent %s\n", szCurrentItem.c_str());

				if (std::find(vWhiteList.begin(), vWhiteList.end(), szCurrentItem) == vWhiteList.end()) // not whitelisted item
				{
					//					printf("\tUnknown file: %s\n", szCurrentItem.c_str());

					if (DeleteFileA(szCurrentItem.c_str()) == FALSE /* can not removed */ || _access(szCurrentItem.c_str(), 0) == 0 /* removed but still exist */)
					{
						std::string szWarn = "Unknown file detected: " + szCurrentItem + ", Delete this file.";
						MessageBoxA(0, szWarn.c_str(), 0, 0);

						PostQuitMessage(0);
					}
				}
			}
		} while (FindNextFileA(hFind, &w32fd));
		FindClose(hFind);
	}
}

void CPythonNetworkStream::__RefreshAlignmentWindow()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshAlignment", Py_BuildValue("()"));
}

void CPythonNetworkStream::__RefreshTargetBoardByVID(DWORD dwVID)
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshTargetBoardByVID", Py_BuildValue("(i)", dwVID));
}

void CPythonNetworkStream::__RefreshTargetBoardByName(const char* c_szName)
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshTargetBoardByName", Py_BuildValue("(s)", c_szName));
}

void CPythonNetworkStream::__RefreshTargetBoard()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshTargetBoard", Py_BuildValue("()"));
}

void CPythonNetworkStream::__RefreshGuildWindowGradePage()
{
	m_isRefreshGuildWndGradePage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowSkillPage()
{
	m_isRefreshGuildWndSkillPage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowMemberPageGradeComboBox()
{
	m_isRefreshGuildWndMemberPageGradeComboBox = true;
}

void CPythonNetworkStream::__RefreshGuildWindowMemberPage()
{
	m_isRefreshGuildWndMemberPage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowBoardPage()
{
	m_isRefreshGuildWndBoardPage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowInfoPage()
{
	m_isRefreshGuildWndInfoPage = true;
}

void CPythonNetworkStream::__RefreshMessengerWindow()
{
	m_isRefreshMessengerWnd = true;
}

void CPythonNetworkStream::__RefreshSafeboxWindow()
{
	m_isRefreshSafeboxWnd = true;
}

void CPythonNetworkStream::__RefreshMallWindow()
{
	m_isRefreshMallWnd = true;
}

void CPythonNetworkStream::__RefreshSkillWindow()
{
	m_isRefreshSkillWnd = true;
}

void CPythonNetworkStream::__RefreshExchangeWindow()
{
	m_isRefreshExchangeWnd = true;
}

void CPythonNetworkStream::__RefreshStatus()
{
	m_isRefreshStatus = true;
}

void CPythonNetworkStream::__RefreshCharacterWindow()
{
	m_isRefreshCharacterWnd = true;
}

void CPythonNetworkStream::__RefreshInventoryWindow()
{
	m_isRefreshInventoryWnd = true;
}

void CPythonNetworkStream::__RefreshEquipmentWindow()
{
	m_isRefreshEquipmentWnd = true;
}

void CPythonNetworkStream::__RefreshAffectWindow()
{
	m_isRefreshAffectWindow = true;
}

#ifdef USE_OFFLINESHOP_SEARCH_V2
void CPythonNetworkStream::__RefreshShopSearchWindow()
{
    m_isRefreshShopSearchWnd = true;
}
#endif

void CPythonNetworkStream::__SetGuildID(DWORD id)
{
	if (m_dwGuildID != id)
	{
		m_dwGuildID = id;
		IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();

		for (int i = 0; i < PLAYER_PER_ACCOUNT4; ++i)
			if (!strncmp(m_akSimplePlayerInfo[i].szName, rkPlayer.GetName(), CHARACTER_NAME_MAX_LEN))
			{
				m_adwGuildID[i] = id;

				std::string  guildName;
				if (CPythonGuild::Instance().GetGuildName(id, &guildName))
				{
					m_astrGuildName[i] = guildName;
				}
				else
				{
					m_astrGuildName[i] = "";
				}
			}
	}
}

struct PERF_PacketInfo
{
	DWORD dwCount;
	DWORD dwTime;

	PERF_PacketInfo()
	{
		dwCount = 0;
		dwTime = 0;
	}
};


#ifdef __PERFORMANCE_CHECK__

class PERF_PacketTimeAnalyzer
{
public:
	~PERF_PacketTimeAnalyzer()
	{
		FILE* fp = fopen("perf_dispatch_packet_result.txt", "w");

		for (std::map<DWORD, PERF_PacketInfo>::iterator i = m_kMap_kPacketInfo.begin(); i != m_kMap_kPacketInfo.end(); ++i)
		{
			if (i->second.dwTime > 0)
				fprintf(fp, "header %d: count %d, time %d, tpc %d\n", i->first, i->second.dwCount, i->second.dwTime, i->second.dwTime / i->second.dwCount);
		}
		fclose(fp);
	}

public:
	std::map<DWORD, PERF_PacketInfo> m_kMap_kPacketInfo;
};

PERF_PacketTimeAnalyzer gs_kPacketTimeAnalyzer;

#endif

// Game Phase ---------------------------------------------------------------------------
void CPythonNetworkStream::GamePhase()
{
#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	if (clientCode != 0)
	{
		std::string message = "CShield-ClientSide-ErrorCode: " + std::to_string(clientCode);
		NotifyHack(message.c_str());
		clientCode = 0;
	}
#endif

	if (!m_kQue_stHack.empty())
	{
		__SendHack(m_kQue_stHack.front().c_str());
		m_kQue_stHack.pop_front();
	}

	TPacketHeader header = 0;
	bool ret = true;

#ifdef __PERFORMANCE_CHECK__
	DWORD timeBeginDispatch = timeGetTime();

	static std::map<DWORD, PERF_PacketInfo> kMap_kPacketInfo;
	kMap_kPacketInfo.clear();
#endif

    const DWORD MAX_RECV_COUNT = 8 * 2;
    const DWORD SAFE_RECV_BUFSIZE = 8192 * 2;
    DWORD dwRecvCount = 0;
    DWORD dwRecvItem = 0;

    while (ret)
    {
        if (dwRecvCount++ >= MAX_RECV_COUNT - 1 && GetRecvBufferSize() < SAFE_RECV_BUFSIZE && m_strPhase == "Game")
        {
            break;
        }

        if (!CheckPacket(&header))
        {
            break;
        }

#ifdef _DEBUG
        Tracenf("RECV HEADER: %u (phase: %s)", header, m_strPhase.c_str());
#endif

#ifdef __PERFORMANCE_CHECK__
		DWORD timeBeginPacket = timeGetTime();
#endif

        if (header == HEADER_GC_CLOSECLIENT)
        {
            RecvCloseClient();
            break;
        }

		switch (header)
		{
		case HEADER_GC_OBSERVER_ADD:
			ret = RecvObserverAddPacket();
			break;
		case HEADER_GC_OBSERVER_REMOVE:
			ret = RecvObserverRemovePacket();
			break;
		case HEADER_GC_OBSERVER_MOVE:
			ret = RecvObserverMovePacket();
			break;
		case HEADER_GC_WARP:
			ret = RecvWarpPacket();
			break;

		case HEADER_GC_PHASE:
			ret = RecvPhasePacket();
#ifdef USE_CAPTCHA_SYSTEM
            if (!CPythonApplication::Instance().IsWindowVisisble())
            {
                CPythonApplication::Instance().SetWindowInvisible(false);
            }
#endif
			return; // 도중에 Phase 가 바뀌면 일단 무조건 GamePhase 탈출 - [levites]
			break;
			
		case HEADER_GC_RANK_INFO:
			ret = RecvRankInfo();
			break;

		case HEADER_GC_CHEAT_BLACKLIST:
			ret = RecvCheatBlacklist();
			break;
#ifdef ENABLE_AURA_SYSTEM
			case HEADER_GC_AURA:
				ret = RecvAuraPacket();
				break;
#endif
		case HEADER_GC_PVP:
			ret = RecvPVPPacket();
			break;

		case HEADER_GC_DUEL_START:
			ret = RecvDuelStartPacket();
			break;

		case HEADER_GC_CHARACTER_ADD:
			ret = RecvCharacterAppendPacket();
			break;

		case HEADER_GC_CHAR_ADDITIONAL_INFO:
			ret = RecvCharacterAdditionalInfo();
			break;

		case HEADER_GC_CHARACTER_ADD2:
			ret = RecvCharacterAppendPacketNew();
			break;

		case HEADER_GC_CHARACTER_UPDATE:
			ret = RecvCharacterUpdatePacket();
			break;

		case HEADER_GC_CHARACTER_UPDATE2:
			ret = RecvCharacterUpdatePacketNew();
			break;

		case HEADER_GC_CHARACTER_DEL:
			ret = RecvCharacterDeletePacket();
			break;

        case HEADER_GC_CHAT:
        {
            ret = RecvChatPacket();
            break;
        }

		case HEADER_GC_SYNC_POSITION:
			ret = RecvSyncPositionPacket();
			break;

		case HEADER_GC_OWNERSHIP:
			ret = RecvOwnerShipPacket();
			break;

		case HEADER_GC_WHISPER:
			ret = RecvWhisperPacket();
			break;
#ifdef ENABLE_SUPPORT_SYSTEM
		case HEADER_GC_SUPPORT_SKILL:
			ret = RecvSupportUseSkill();
			break;
#endif
		case HEADER_GC_CHARACTER_MOVE:
			ret = RecvCharacterMovePacket();
			break;

			// Position
		case HEADER_GC_CHARACTER_POSITION:
			ret = RecvCharacterPositionPacket();
			break;

			// Battle Packet
		case HEADER_GC_STUN:
			ret = RecvStunPacket();
			break;

		case HEADER_GC_DEAD:
			ret = RecvDeadPacket();
			break;

		case HEADER_GC_PLAYER_POINT_CHANGE:
			ret = RecvPointChange();
			break;

			// item packet.
        case HEADER_GC_ITEM_SET:
        {
            dwRecvItem += 1;

            if (dwRecvItem == 2)
            {
                dwRecvCount -= 1;
                dwRecvItem = 0;
            }

            ret = RecvItemSetPacket();
            break;
        }

		case HEADER_GC_ITEM_SET2:
			ret = RecvItemSetPacket2();
			break;

		case HEADER_GC_ITEM_USE:
			ret = RecvItemUsePacket();
			break;

		case HEADER_GC_ITEM_UPDATE:
			ret = RecvItemUpdatePacket();
			break;
#ifdef USE_EXPANDED_ITEM_REFRESH
        case HEADER_GC_ITEM_UPDATE_COUNT:
        {
            ret = RecvItemUpdatePacket(true);
            break;
        }
#endif
		case HEADER_GC_ITEM_GROUND_ADD:
			ret = RecvItemGroundAddPacket();
			break;

		case HEADER_GC_ITEM_GROUND_DEL:
			ret = RecvItemGroundDelPacket();
			break;

		case HEADER_GC_ITEM_OWNERSHIP:
			ret = RecvItemOwnership();
			break;

		case HEADER_GC_QUICKSLOT_ADD:
			ret = RecvQuickSlotAddPacket();
			break;

		case HEADER_GC_QUICKSLOT_DEL:
			ret = RecvQuickSlotDelPacket();
			break;

		case HEADER_GC_QUICKSLOT_SWAP:
			ret = RecvQuickSlotMovePacket();
			break;

		case HEADER_GC_MOTION:
			ret = RecvMotionPacket();
			break;

#if defined(__BL_67_ATTR__)
		case HEADER_GC_OPEN_67_ATTR:
			ret = Recv67AttrOpenPacket();
			break;
#endif

		case HEADER_GC_SHOP:
			ret = RecvShopPacket();
			break;

#if defined(__BL_BATTLE_ROYALE__)
		case HEADER_GC_BATTLE_ROYALE:
			ret = RecvBattleRoyale();
			break;
#endif

		case HEADER_GC_EXCHANGE:
			ret = RecvExchangePacket();
			break;

		case HEADER_GC_QUEST_INFO:
			ret = RecvQuestInfoPacket();
			break;

		case HEADER_GC_REQUEST_MAKE_GUILD:
			ret = RecvRequestMakeGuild();
			break;

		case HEADER_GC_PING:
			ret = RecvPingPacket();
			break;

		case HEADER_GC_SCRIPT:
			ret = RecvScriptPacket();
			break;

		case HEADER_GC_QUEST_CONFIRM:
			ret = RecvQuestConfirmPacket();
			break;

		case HEADER_GC_TARGET:
			ret = RecvTargetPacket();
			break;
#ifdef ENABLE_SEND_TARGET_INFO
		case HEADER_GC_TARGET_INFO:
			ret = RecvTargetInfoPacket();
			break;
#endif
		case HEADER_GC_DAMAGE_INFO:
			ret = RecvDamageInfoPacket();
			break;
#if defined(WJ_COMBAT_ZONE)
		case HEADER_GC_RANKING_COMBAT_ZONE:
			ret = __RecvCombatZoneRankingPacket();
			break;

		case HEADER_GC_SEND_COMBAT_ZONE:
			ret = __RecvCombatZonePacket();
			break;
#endif
		case HEADER_GC_MOUNT:
			ret = RecvMountPacket();
			break;
#ifdef ENABLE_MANAGER_BANK_SYSTEM
		case HEADER_GC_BANK:
			ret = RecvAccountBankPacket();
			break;
#endif		
		case HEADER_GC_CHANGE_SPEED:
			ret = RecvChangeSpeedPacket();
			break;

#ifdef ENABLE_SHOW_CHEST_DROP
		case HEADER_GC_CHESTDROP:
			ret = RecvChestDrop();
			break;
#endif
		case HEADER_GC_PLAYER_POINTS:
			ret = __RecvPlayerPoints();
			break;

		case HEADER_GC_CREATE_FLY:
			ret = RecvCreateFlyPacket();
			break;

		case HEADER_GC_FLY_TARGETING:
			ret = RecvFlyTargetingPacket();
			break;

		case HEADER_GC_ADD_FLY_TARGETING:
			ret = RecvAddFlyTargetingPacket();
			break;

		case HEADER_GC_SKILL_LEVEL:
			ret = RecvSkillLevel();
			break;

		case HEADER_GC_SKILL_LEVEL_NEW:
			ret = RecvSkillLevelNew();
			break;

		case HEADER_GC_MESSENGER:
			ret = RecvMessenger();
			break;

		case HEADER_GC_GUILD:
			ret = RecvGuild();
			break;

		case HEADER_GC_PARTY_INVITE:
			ret = RecvPartyInvite();
			break;

		case HEADER_GC_PARTY_ADD:
			ret = RecvPartyAdd();
			break;

		case HEADER_GC_PARTY_UPDATE:
			ret = RecvPartyUpdate();
			break;

		case HEADER_GC_PARTY_REMOVE:
			ret = RecvPartyRemove();
			break;

		case HEADER_GC_PARTY_LINK:
			ret = RecvPartyLink();
			break;

		case HEADER_GC_PARTY_UNLINK:
			ret = RecvPartyUnlink();
			break;

		case HEADER_GC_PARTY_PARAMETER:
			ret = RecvPartyParameter();
			break;

		case HEADER_GC_SAFEBOX_SET:
			ret = RecvSafeBoxSetPacket();
			break;

		case HEADER_GC_SAFEBOX_DEL:
			ret = RecvSafeBoxDelPacket();
			break;

		case HEADER_GC_SAFEBOX_WRONG_PASSWORD:
			ret = RecvSafeBoxWrongPasswordPacket();
			break;

		case HEADER_GC_SAFEBOX_SIZE:
			ret = RecvSafeBoxSizePacket();
			break;

		case HEADER_GC_SAFEBOX_MONEY_CHANGE:
			ret = RecvSafeBoxMoneyChangePacket();
			break;

		case HEADER_GC_FISHING:
			ret = RecvFishing();
			break;

		case HEADER_GC_DUNGEON:
			ret = RecvDungeon();
			break;

		case HEADER_GC_TIME:
			ret = RecvTimePacket();
			break;

		case HEADER_GC_WALK_MODE:
			ret = RecvWalkModePacket();
			break;

		case HEADER_GC_CHANGE_SKILL_GROUP:
			ret = RecvChangeSkillGroupPacket();
			break;

		case HEADER_GC_REFINE_INFORMATION_NEW:
			ret = RecvRefineInformationPacket();
			break;

		case HEADER_GC_SEPCIAL_EFFECT:
			ret = RecvSpecialEffect();
			break;
#ifndef USE_MARK_ATLAS_BINARY
		case HEADER_GC_NPC_POSITION:
			ret = RecvNPCList();
			break;
#endif
		case HEADER_GC_CHANNEL:
			ret = RecvChannelPacket();
			break;

		case HEADER_GC_VIEW_EQUIP:
			ret = RecvViewEquipPacket();
			break;

		case HEADER_GC_LAND_LIST:
			ret = RecvLandPacket();
			break;

			//case HEADER_GC_TARGET_CREATE:
			//	ret = RecvTargetCreatePacket();
			//	break;

		case HEADER_GC_TARGET_CREATE_NEW:
			ret = RecvTargetCreatePacketNew();
			break;

		case HEADER_GC_TARGET_UPDATE:
			ret = RecvTargetUpdatePacket();
			break;

		case HEADER_GC_TARGET_DELETE:
			ret = RecvTargetDeletePacket();
			break;

		case HEADER_GC_AFFECT_ADD:
			ret = RecvAffectAddPacket();
			break;

		case HEADER_GC_AFFECT_REMOVE:
			ret = RecvAffectRemovePacket();
			break;

		case HEADER_GC_MALL_OPEN:
			ret = RecvMallOpenPacket();
			break;

		case HEADER_GC_MALL_SET:
			ret = RecvMallItemSetPacket();
			break;

		case HEADER_GC_MALL_DEL:
			ret = RecvMallItemDelPacket();
			break;

#ifdef ENABLE_YOHARA_SYSTEM
		case HEADER_GC_SUNGMA_ATTR_UPDATE:
			ret = RecvSungmaAttrUpdatePacket();
			break;
#endif

		case HEADER_GC_LOVER_INFO:
			ret = RecvLoverInfoPacket();
			break;

		case HEADER_GC_LOVE_POINT_UPDATE:
			ret = RecvLovePointUpdatePacket();
			break;

		case HEADER_GC_DIG_MOTION:
			ret = RecvDigMotionPacket();
			break;

		case HEADER_GC_HANDSHAKE:
			RecvHandshakePacket();
			return;
			break;

		case HEADER_GC_HANDSHAKE_OK:
			RecvHandshakeOKPacket();
			return;
			break;

#ifdef ENABLE_ITEMSHOP
		case HEADER_GC_ITEMSHOP:
		{
			ret = RecvItemShop();
			break;
		}
#endif
#ifdef ENABLE_NEW_DETAILS_GUI
		case HEADER_GC_KILL_LOG:
			ret = RecvKillLOG();
			break;
#endif
#ifdef _IMPROVED_PACKET_ENCRYPTION_
		case HEADER_GC_KEY_AGREEMENT:
			RecvKeyAgreementPacket();
			return;
			break;

		case HEADER_GC_KEY_AGREEMENT_COMPLETED:
			RecvKeyAgreementCompletedPacket();
			return;
			break;
#endif
		case HEADER_GC_SPECIFIC_EFFECT:
			ret = RecvSpecificEffect();
			break;


		case HEADER_GC_DRAGON_SOUL_REFINE:
			ret = RecvDragonSoulRefine();
			break;
			
#ifdef ENABLE_SWITCHBOT
			case HEADER_GC_SWITCHBOT:
				ret = RecvSwitchbotPacket();
				break;
#endif
			
#ifdef ENABLE_SWITCHBOT_WORLDARD
		case HEADER_GC_SWITCHBOT:
			ret = RecvSwitchBot();
			break;
#endif

		case HEADER_GC_UNK_213: // @fixme007
			ret = RecvUnk213();
			break;
#ifdef ENABLE_SASH_SYSTEM
		case HEADER_GC_SASH:
			ret = RecvSashPacket();
			break;
#endif
#ifdef ENABLE_CALENDAR_SYSTEM
		case HEADER_GC_CALENDAR_SYSTEM:
			ret = RecvCalendarSystem();
			break;

		case HEADER_GC_CALENDAR_SYSTEM_ADM:
			ret = RecvCalendarSystemADM();
			break;
#endif


#ifdef ENABLE_DUNGEON_INFO
		case HEADER_GC_DUNGEON_INFO_SYSTEM:
			ret = RecvDungeonInfoPacket();
			break;
#endif
#ifdef ENABLE_BATTLEPASS
		case net_battlepass::HEADER_GC_BATTLEPASS:
			ret = BattlePassManager::RecvBattlePassPacket();
			break;
#endif
#ifdef ENABLE_KINGDOMS_WAR
		case HEADER_GC_KINGDOMSWAR:
			ret = RecvKingdomsWarPacket();
			break;
#endif
#ifdef ENABLE_DROP_ITEM_WORLDARD
		case HEADER_GC_DROP_ITEM_SYSTEM:
			ret = RecvDropItemPacket();
			break;
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
		case HEADER_GC_CUBE_RENEWAL:
			ret = RecvCubeRenewalPacket();
			break;
#endif

#ifdef BATTLEPASS_WORLDARD
		case HEADER_GC_BATTLEPASS:
			ret = RecvBattlePassPacket();
			break;
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
		case HEADER_GC_TOURNAMENT_ADD:
			ret = RecvTournamentAdd();
			break;
#endif
#ifdef ENABLE_EVENT_MANAGER
		case HEADER_GC_EVENT_MANAGER:
			ret = RecvEventManager();
			break;
#endif

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
		case HEADER_GC_CSHIELD:
			ret = RecvCShieldPacket();
			break;
#endif
#ifdef ENABLE_EXCHANGE_LOG
		case HEADER_GC_EXCHANGE_LOG:
			ret = RecvExchangeLog();
			break;
#endif
#ifdef ENABLE_MINI_GAME_CATCH_KING
		case HEADER_GC_MINI_GAME_CATCH_KING:
			ret = RecvMiniGameCatchKingPacket();
			break;
#endif
#ifdef ENABLE_RANKING
		case HEADER_GC_RANKING_SEND:
			ret = RecvRankingTable();
			break;
#endif
#ifdef __BUFFI_SUPPORT__
		case HEADER_GC_BUFFI_SKILL:
			ret = RecvBuffiSkill();
			break;
#endif
#ifdef ENABLE_IKASHOP_RENEWAL
			case HEADER_GC_NEW_OFFLINESHOP:
				ret = RecvOfflineshopPacket();
				break;
#endif
#ifdef USE_CAPTCHA_SYSTEM
        case HEADER_GC_CAPTCHA:
        {
            ret = RecvCaptcha();
            break;
        }
#endif
#ifdef ENABLE_NINJEYE
		case HEADER_GC_NINJEYE_REQUEST:
			ret = RecvNinjEyeRequest();
			break;
#endif

#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
		case HEADER_GC_HIT_COUNT_INFO:
			ret = RecvHitCountInfo();
			break;
#endif
#if defined(ENABLE_VOICE_CHAT)
		case HEADER_GC_VOICE_CHAT:
			ret = RecvVoiceChatPacket();
			break;
#endif
#ifdef USE_CRYSTAL_SYSTEM
		case HEADER_GC_SHARD_CRAFT:
		{
			ret = RecvShardCraft();
			break;
		}
#endif
		default:
			ret = RecvDefaultPacket(header);
			break;
		}
#ifdef __PERFORMANCE_CHECK__
		DWORD timeEndPacket = timeGetTime();

		{
			PERF_PacketInfo& rkPacketInfo = kMap_kPacketInfo[header];
			rkPacketInfo.dwCount++;
			rkPacketInfo.dwTime += timeEndPacket - timeBeginPacket;
		}

		{
			PERF_PacketInfo& rkPacketInfo = gs_kPacketTimeAnalyzer.m_kMap_kPacketInfo[header];
			rkPacketInfo.dwCount++;
			rkPacketInfo.dwTime += timeEndPacket - timeBeginPacket;
		}
#endif
	}

#ifdef __PERFORMANCE_CHECK__
	DWORD timeEndDispatch = timeGetTime();

	if (timeEndDispatch - timeBeginDispatch > 2)
	{
		static FILE* fp = fopen("perf_dispatch_packet.txt", "w");

		fprintf(fp, "delay %d\n", timeEndDispatch - timeBeginDispatch);
		for (std::map<DWORD, PERF_PacketInfo>::iterator i = kMap_kPacketInfo.begin(); i != kMap_kPacketInfo.end(); ++i)
		{
			if (i->second.dwTime > 0)
				fprintf(fp, "header %d: count %d, time %d\n", i->first, i->second.dwCount, i->second.dwTime);
		}
		fputs("=====================================================\n", fp);
		fflush(fp);
	}
#endif

	if (!ret)
		RecvErrorPacket(header);

	static DWORD s_nextRefreshTime = ELTimer_GetMSec();
	static DWORD s_nextFileCheckTime = ELTimer_GetMSec();

	DWORD curTime = ELTimer_GetMSec();

	//if (s_nextRefreshTime > curTime)
	//	return;


	if (s_nextFileCheckTime <= curTime)
	{
		// FileCheckRoutine();
		// FileCheckPack();
		// FileChecks();
		s_nextFileCheckTime = curTime + 800;
	}


	if (m_isRefreshCharacterWnd)
	{
		m_isRefreshCharacterWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshCharacter", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshEquipmentWnd)
	{
		m_isRefreshEquipmentWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshEquipment", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshInventoryWnd)
	{
		m_isRefreshInventoryWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshInventory", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshExchangeWnd)
	{
		m_isRefreshExchangeWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshExchange", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshSkillWnd)
	{
		m_isRefreshSkillWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSkill", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshSafeboxWnd)
	{
		m_isRefreshSafeboxWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSafebox", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshMallWnd)
	{
		m_isRefreshMallWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshMall", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshStatus)
	{
		m_isRefreshStatus = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshStatus", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshMessengerWnd)
	{
		m_isRefreshMessengerWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshMessenger", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndInfoPage)
	{
		m_isRefreshGuildWndInfoPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildInfoPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndBoardPage)
	{
		m_isRefreshGuildWndBoardPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildBoardPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndMemberPage)
	{
		m_isRefreshGuildWndMemberPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildMemberPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndMemberPageGradeComboBox)
	{
		m_isRefreshGuildWndMemberPageGradeComboBox = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildMemberPageGradeComboBox", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndSkillPage)
	{
		m_isRefreshGuildWndSkillPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildSkillPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndGradePage)
	{
		m_isRefreshGuildWndGradePage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildGradePage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}
	if (m_isRefreshAffectWindow)
	{
		m_isRefreshAffectWindow = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshAffectWindow", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}
#ifdef USE_OFFLINESHOP_SEARCH_V2
    if (m_isRefreshShopSearchWnd)
    {
        m_isRefreshShopSearchWnd = false;
        PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshShopSearch", Py_BuildValue("()"));
        s_nextRefreshTime = curTime + 300;
    }
#endif

	//CHAR szGame[] = { 'G', 'a', 'm', 'e', 0x0 };
	//if (!strcmp(m_strPhase.c_str(), szGame))
	//{
	//	CCheatDetectQueueMgr::Instance().OnTick();
	//}
}

void CPythonNetworkStream::__InitializeGamePhase()
{
	__ServerTimeSync_Initialize();

	m_isRefreshStatus = false;
	m_isRefreshCharacterWnd = false;
	m_isRefreshEquipmentWnd = false;
	m_isRefreshInventoryWnd = false;
	m_isRefreshExchangeWnd = false;
	m_isRefreshSkillWnd = false;
	m_isRefreshSafeboxWnd = false;
	m_isRefreshMallWnd = false;
	m_isRefreshMessengerWnd = false;
	m_isRefreshGuildWndInfoPage = false;
	m_isRefreshGuildWndBoardPage = false;
	m_isRefreshGuildWndMemberPage = false;
	m_isRefreshGuildWndMemberPageGradeComboBox = false;
	m_isRefreshGuildWndSkillPage = false;
	m_isRefreshGuildWndGradePage = false;
	m_isRefreshAffectWindow = false;
#ifdef USE_OFFLINESHOP_SEARCH_V2
    m_isRefreshShopSearchWnd = false;
#endif

	m_EmoticonStringVector.clear();

	m_pInstTarget = NULL;
}

void CPythonNetworkStream::Warp(long lGlobalX, long lGlobalY)
{
	auto& rkBgMgr = CPythonBackground::Instance();
//	rkBgMgr.SetShadowLevel(CPythonBackground::SHADOW_NONE);

	rkBgMgr.Destroy();
	rkBgMgr.Create();

	rkBgMgr.Warp(lGlobalX, lGlobalY);

	//rkBgMgr.SetShadowLevel(CPythonBackground::SHADOW_ALL);
	rkBgMgr.RefreshShadowLevel();

    long lLocalX = lGlobalX;
    long lLocalY = lGlobalY;
    __GlobalPositionToLocalPosition(lLocalX, lLocalY);

    float fHeight = CPythonBackground::Instance().GetHeight(float(lLocalX), float(lLocalY));

    IAbstractApplication& rkApp = IAbstractApplication::GetSingleton();
    rkApp.SetCenterPosition(float(lLocalX), float(lLocalY), fHeight);

    __ShowMapName(lLocalX, lLocalY);
}

void CPythonNetworkStream::__ShowMapName(long lLocalX, long lLocalY)
{
	const std::string& c_rstrMapFileName = CPythonBackground::Instance().GetWarpMapName();
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ShowMapName", Py_BuildValue("(sii)", c_rstrMapFileName.c_str(), lLocalX, lLocalY));

}

void CPythonNetworkStream::__LeaveGamePhase()
{
	CInstanceBase::ClearPVPKeySystem();

	__ClearNetworkActorManager();

	m_bComboSkillFlag = FALSE;

#if defined(ENABLE_VOICE_CHAT)
	VoiceChat::Instance().Destroy();
#endif

	IAbstractCharacterManager& rkChrMgr = IAbstractCharacterManager::GetSingleton();
	rkChrMgr.Destroy();

	CPythonItem& rkItemMgr = CPythonItem::Instance();
	rkItemMgr.Destroy();
#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef ENABLE_IKASHOP_ENTITIES
	CPythonIkarusShop::instance().DeleteEntities();
#endif
#endif
}

void CPythonNetworkStream::SetGamePhase()
{
	if ("Game" != m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Game Phase ##");
	Tracen("");

	m_strPhase = "Game";

	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::GamePhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveGamePhase);

#if defined(ENABLE_VOICE_CHAT)
	VoiceChat::Instance().Initialize();
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_ReloadVoiceChatVolumes", Py_BuildValue("()"));
#endif

	// Main Character 등록O

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());

#ifdef ENABLE_TRACK_WINDOW
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "TrackWindowUpdate", Py_BuildValue("()"));
#endif

	__RefreshStatus();
}

bool CPythonNetworkStream::RecvCheatBlacklist()
{
	TPacketGCCheatBlacklist kBlacklistPacket;

	if (!Recv(sizeof(kBlacklistPacket), &kBlacklistPacket))
	{
		return false;
	}

	//CAnticheatManager::Instance().ParseCheatBlacklist(kBlacklistPacket.content);

	return true;
}


#ifdef ENABLE_SEND_TARGET_INFO
bool CPythonNetworkStream::SendTargetInfoLoadPacket(DWORD dwVID)
{
	TPacketCGTargetInfoLoad TargetInfoLoadPacket;
	TargetInfoLoadPacket.header = HEADER_CG_TARGET_INFO_LOAD;
	TargetInfoLoadPacket.vid = dwVID;

	if (!Send(sizeof(TargetInfoLoadPacket), &TargetInfoLoadPacket))
		return false;

	return SendSequence();
}
#endif

bool CPythonNetworkStream::RecvObserverAddPacket()
{
	TPacketGCObserverAdd kObserverAddPacket;
	if (!Recv(sizeof(kObserverAddPacket), &kObserverAddPacket))
		return false;

	CPythonMiniMap::Instance().AddObserver(
		kObserverAddPacket.vid,
		kObserverAddPacket.x * 100.0f,
		kObserverAddPacket.y * 100.0f);

	return true;
}

bool CPythonNetworkStream::RecvUnk213() // @fixme007
{
	TPacketGCUnk213 kUnk213Packet;
	if (!Recv(sizeof(TPacketGCUnk213)), &kUnk213Packet)
		return false;
	return true;
}

bool CPythonNetworkStream::RecvObserverRemovePacket()
{
	TPacketGCObserverAdd kObserverRemovePacket;
	if (!Recv(sizeof(kObserverRemovePacket), &kObserverRemovePacket))
		return false;

	CPythonMiniMap::Instance().RemoveObserver(
		kObserverRemovePacket.vid
	);

	return true;
}

bool CPythonNetworkStream::RecvObserverMovePacket()
{
	TPacketGCObserverMove kObserverMovePacket;
	if (!Recv(sizeof(kObserverMovePacket), &kObserverMovePacket))
		return false;

	CPythonMiniMap::Instance().MoveObserver(
		kObserverMovePacket.vid,
		kObserverMovePacket.x * 100.0f,
		kObserverMovePacket.y * 100.0f);

	return true;
}


bool CPythonNetworkStream::RecvWarpPacket()
{
    TPacketGCWarp kWarpPacket;
    if (!Recv(sizeof(kWarpPacket), &kWarpPacket))
    {
        return false;
    }

    __DirectEnterMode_Set(m_dwSelectedCharacterIndex, true);
    CNetworkStream::Connect((DWORD)kWarpPacket.lAddr, kWarpPacket.wPort);
    return true;
}

bool CPythonNetworkStream::RecvDuelStartPacket()
{
	TPacketGCDuelStart kDuelStartPacket;
	if (!Recv(sizeof(kDuelStartPacket), &kDuelStartPacket))
		return false;

	DWORD count = (kDuelStartPacket.wSize - sizeof(kDuelStartPacket)) / sizeof(DWORD);

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();

	CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();
	if (!pkInstMain)
	{
		TraceError("CPythonNetworkStream::RecvDuelStartPacket - MainCharacter is NULL");
		return false;
	}
	DWORD dwVIDSrc = pkInstMain->GetVirtualID();
	DWORD dwVIDDest;

	for (DWORD i = 0; i < count; i++)
	{
		Recv(sizeof(dwVIDDest), &dwVIDDest);
		CInstanceBase::InsertDUELKey(dwVIDSrc, dwVIDDest);
	}

	if (count == 0)
		pkInstMain->SetDuelMode(CInstanceBase::DUEL_CANNOTATTACK);
	else
		pkInstMain->SetDuelMode(CInstanceBase::DUEL_START);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));

	rkChrMgr.RefreshAllPCTextTail();

	return true;
}

bool CPythonNetworkStream::RecvPVPPacket()
{
	TPacketGCPVP kPVPPacket;
	if (!Recv(sizeof(kPVPPacket), &kPVPPacket))
		return false;

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();

	switch (kPVPPacket.bMode)
	{
	case PVP_MODE_AGREE:
		rkChrMgr.RemovePVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);

		// 상대가 나(Dst)에게 동의를 구했을때
		if (rkPlayer.IsMainCharacterIndex(kPVPPacket.dwVIDDst))
			rkPlayer.RememberChallengeInstance(kPVPPacket.dwVIDSrc);

		// 상대에게 동의를 구한 동안에는 대결 불능
		if (rkPlayer.IsMainCharacterIndex(kPVPPacket.dwVIDSrc))
			rkPlayer.RememberCantFightInstance(kPVPPacket.dwVIDDst);
		break;
	case PVP_MODE_REVENGE:
	{
		rkChrMgr.RemovePVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);

		DWORD dwKiller = kPVPPacket.dwVIDSrc;
		DWORD dwVictim = kPVPPacket.dwVIDDst;

		// 내(victim)가 상대에게 복수할 수 있을때
		if (rkPlayer.IsMainCharacterIndex(dwVictim))
			rkPlayer.RememberRevengeInstance(dwKiller);

		// 상대(victim)가 나에게 복수하는 동안에는 대결 불능
		if (rkPlayer.IsMainCharacterIndex(dwKiller))
			rkPlayer.RememberCantFightInstance(dwVictim);
		break;
	}

	case PVP_MODE_FIGHT:
		rkChrMgr.InsertPVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDSrc);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDDst);
		break;
	case PVP_MODE_NONE:
		rkChrMgr.RemovePVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDSrc);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDDst);
		break;
	}

	// NOTE : PVP 토글시 TargetBoard 를 업데이트 합니다.
	__RefreshTargetBoardByVID(kPVPPacket.dwVIDSrc);
	__RefreshTargetBoardByVID(kPVPPacket.dwVIDDst);

	return true;
}

// DELETEME
/*
void CPythonNetworkStream::__SendWarpPacket()
{
	TPacketCGWarp kWarpPacket;
	kWarpPacket.bHeader=HEADER_GC_WARP;
	if (!Send(sizeof(kWarpPacket), &kWarpPacket))
	{
		return;
	}
}
*/
void CPythonNetworkStream::NotifyHack(const char* c_szMsg)
{
	if (!m_kQue_stHack.empty())
		if (c_szMsg == m_kQue_stHack.back())
			return;

	m_kQue_stHack.push_back(c_szMsg);
}

bool CPythonNetworkStream::__SendHack(const char* c_szMsg)
{
	Tracen(c_szMsg);

	TPacketCGHack kPacketHack;
	kPacketHack.bHeader = HEADER_CG_HACK;
	strncpy(kPacketHack.szBuf, c_szMsg, sizeof(kPacketHack.szBuf) - 1);

	if (!Send(sizeof(kPacketHack), &kPacketHack))
		return false;

	return SendSequence();
}
#ifdef NEW_PET_SYSTEM
bool CPythonNetworkStream::PetSetNamePacket(const char* petname)
{
	TPacketCGRequestPetName PetSetName;
	PetSetName.byHeader = HEADER_CG_PetSetName;
	strncpy(PetSetName.petname, petname, 12);
	PetSetName.petname[12] = '\0';

	if (!Send(sizeof(PetSetName), &PetSetName))
		return false;

	return SendSequence();
}
#endif
bool CPythonNetworkStream::SendMessengerAddByVIDPacket(DWORD vid)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_ADD_BY_VID;
	if (!Send(sizeof(packet), &packet))
		return false;
	if (!Send(sizeof(vid), &vid))
		return false;
	return SendSequence();
}

bool CPythonNetworkStream::SendHackNotification(const char* c_szMsg, const char* c_szInfo, const char* c_szHwid)
{
	TPacketCGHack kPacketHack;
	kPacketHack.bHeader = HEADER_CG_HACK;
	strncpy_s(kPacketHack.szBuf, c_szMsg, sizeof(kPacketHack.szBuf) - 1);
	strncpy_s(kPacketHack.szInfo, c_szInfo, sizeof(kPacketHack.szInfo) - 1);
	//	strncpy_s(kPacketHack.szHwid, c_szHwid, sizeof(kPacketHack.szHwid) - 1);

	if (!Send(sizeof(kPacketHack), &kPacketHack))
	{
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendMessengerAddByNamePacket(const char* c_szName)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_ADD_BY_NAME;
	if (!Send(sizeof(packet), &packet))
		return false;
	char szName[CHARACTER_NAME_MAX_LEN];
	strncpy(szName, c_szName, CHARACTER_NAME_MAX_LEN - 1);
	szName[CHARACTER_NAME_MAX_LEN - 1] = '\0'; // #720: 메신저 이름 관련 버퍼 오버플로우 버그 수정

	if (!Send(sizeof(szName), &szName))
		return false;
	Tracef(" SendMessengerAddByNamePacket : %s\n", c_szName);
	return SendSequence();
}

bool CPythonNetworkStream::SendMessengerRemovePacket(const char* c_szKey, const char* c_szName)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_REMOVE;
	if (!Send(sizeof(packet), &packet))
		return false;
	char szKey[CHARACTER_NAME_MAX_LEN];
	strncpy(szKey, c_szKey, CHARACTER_NAME_MAX_LEN - 1);
	if (!Send(sizeof(szKey), &szKey))
		return false;
	__RefreshTargetBoardByName(c_szName);
	return SendSequence();
}
#ifdef ENABLE_MESSENGER_BLOCK
bool CPythonNetworkStream::SendMessengerBlockAddByVIDPacket(DWORD vid)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_BLOCK_ADD_BY_VID;
	if (!Send(sizeof(packet), &packet))
		return false;
	if (!Send(sizeof(vid), &vid))
		return false;
	return SendSequence();
}

bool CPythonNetworkStream::SendMessengerBlockRemoveByVIDPacket(DWORD vid)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_BLOCK_REMOVE_BY_VID;
	if (!Send(sizeof(packet), &packet))
		return false;
	if (!Send(sizeof(vid), &vid))
		return false;
	return SendSequence();
}

bool CPythonNetworkStream::SendMessengerBlockAddByNamePacket(const char* c_szName)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_BLOCK_ADD_BY_NAME;
	if (!Send(sizeof(packet), &packet))
		return false;
	char szName[CHARACTER_NAME_MAX_LEN];
	strncpy(szName, c_szName, CHARACTER_NAME_MAX_LEN - 1);
	szName[CHARACTER_NAME_MAX_LEN - 1] = '\0';

	if (!Send(sizeof(szName), &szName))
		return false;
#ifdef _DEBUG
	Tracef(" SendMessengerBlockAddByNamePacket : %s\n", c_szName);
#endif
	return SendSequence();
}

bool CPythonNetworkStream::SendMessengerBlockRemovePacket(const char* c_szKey, const char* c_szName)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_BLOCK_REMOVE;
	if (!Send(sizeof(packet), &packet))
		return false;
	char szKey[CHARACTER_NAME_MAX_LEN];
	strncpy(szKey, c_szKey, CHARACTER_NAME_MAX_LEN - 1);
	if (!Send(sizeof(szKey), &szKey))
		return false;
	__RefreshTargetBoardByName(c_szName);
	return SendSequence();
}
#endif


bool CPythonNetworkStream::SendCharacterStatePacket(const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{
	NANOBEGIN
		if (!__CanActMainInstance())
			return true;

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	if (CheckValues())
	{
		clientCode = 44;
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)NULL);
		__asm {
			mov eax, 0x001
			jmp eax
		}
	}
#endif

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();
	if (pkInstMain) {
		if (!pkInstMain->IsGoing() && pkInstMain->__CanSkipCollision()) {
			__SendHack("Hack");
		}
	}

	if (fDstRot < 0.0f)
		fDstRot = 360 + fDstRot;
	else if (fDstRot > 360.0f)
		fDstRot = fmodf(fDstRot, 360.0f);

	// TODO: 나중에 패킷이름을 바꾸자
	TPacketCGMove kStatePacket;
	kStatePacket.bHeader = HEADER_CG_CHARACTER_MOVE;
	kStatePacket.bFunc = eFunc;
	kStatePacket.bArg = uArg;
	kStatePacket.bRot = fDstRot / 5.0f;
	kStatePacket.lX = long(c_rkPPosDst.x);



	kStatePacket.lY = long(c_rkPPosDst.y);
	{
		CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetMainInstancePtr();
		if (eFunc == 3 && uArg > 21) { return true; }

		static UINT c_packet = 0;
		static DWORD t_packet = 0;
		DWORD t = timeGetTime();
		if (t_packet + 500 < t) {
			c_packet = 0;
			t_packet = t;
		}
		c_packet++;
		if (c_packet > 8) { return true; }

		static UINT l_func = 0;
		static DWORD t_f = 0;

		static float f_x = 0.0f;
		static float f_y = 0.0f;
		const D3DXVECTOR3& cpos = pkInst->GetGraphicThingInstancePtr()->GetPosition();
		if (f_x == cpos.x && f_y == cpos.y && t - t_f < 1000 && l_func == eFunc && (eFunc < 3)) {
			return true;
		}
		f_x = cpos.x;
		f_y = cpos.y;

		if (l_func == 0 && eFunc == 0 && t_f + 10 > t) { return true; }
		l_func = eFunc;
		t_f = t;
		if (cpos.x == 0.0f && cpos.y == -0.0f) {
			kStatePacket.lX = long(c_rkPPosDst.x);
			kStatePacket.lY = long(c_rkPPosDst.y);
		}
		else {
			kStatePacket.lX = long(cpos.x);
			kStatePacket.lY = long(cpos.y < 0.0f ? cpos.y * -1.0f : cpos.y);
		}
	}

	{
		CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetMainInstancePtr();
		if (eFunc == 3 && uArg > 21) { return true; }

		static UINT c_packet = 0;
		static DWORD t_packet = 0;
		DWORD t = timeGetTime();
		if (t_packet + 500 < t) {
			c_packet = 0;
			t_packet = t;
		}
		c_packet++;
		if (c_packet > 8) { return true; }

		static UINT l_func = 0;
		static DWORD t_f = 0;

		static float f_x = 0.0f;
		static float f_y = 0.0f;
		const D3DXVECTOR3& cpos = pkInst->GetGraphicThingInstancePtr()->GetPosition();
		if (f_x == cpos.x && f_y == cpos.y && t - t_f < 1000 && l_func == eFunc && (eFunc < 3)) {
			return true;
		}
		f_x = cpos.x;
		f_y = cpos.y;

		if (l_func == 0 && eFunc == 0 && t_f + 10 > t) { return true; }
		l_func = eFunc;
		t_f = t;
		if (cpos.x == 0.0f && cpos.y == -0.0f) {
			kStatePacket.lX = long(c_rkPPosDst.x);
			kStatePacket.lY = long(c_rkPPosDst.y);
		}
		else {
			kStatePacket.lX = long(cpos.x);
			kStatePacket.lY = long(cpos.y < 0.0f ? cpos.y * -1.0f : cpos.y);
		}
	}

	{
		CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetMainInstancePtr();
		if (eFunc == 3 && uArg > 21) { return true; }

		static UINT c_packet = 0;
		static DWORD t_packet = 0;
		DWORD t = timeGetTime();
		if (t_packet + 500 < t) {
			c_packet = 0;
			t_packet = t;
		}
		c_packet++;
		if (c_packet > 8) { return true; }

		static UINT l_func = 0;
		static DWORD t_f = 0;

		static float f_x = 0.0f;
		static float f_y = 0.0f;
		const D3DXVECTOR3& cpos = pkInst->GetGraphicThingInstancePtr()->GetPosition();
		if (f_x == cpos.x && f_y == cpos.y && t - t_f < 1000 && l_func == eFunc && (eFunc < 3)) {
			return true;
		}
		f_x = cpos.x;
		f_y = cpos.y;

		if (l_func == 0 && eFunc == 0 && t_f + 10 > t) { return true; }
		l_func = eFunc;
		t_f = t;
		if (cpos.x == 0.0f && cpos.y == -0.0f) {
			kStatePacket.lX = long(c_rkPPosDst.x);
			kStatePacket.lY = long(c_rkPPosDst.y);
		}
		else {
			kStatePacket.lX = long(cpos.x);
			kStatePacket.lY = long(cpos.y < 0.0f ? cpos.y * -1.0f : cpos.y);
		}
	}

	kStatePacket.dwTime = ELTimer_GetServerMSec();

	assert(kStatePacket.lX >= 0 && kStatePacket.lX < 204800);

	__LocalPositionToGlobalPosition(kStatePacket.lX, kStatePacket.lY);

	if (!Send(sizeof(kStatePacket), &kStatePacket))
	{
		Tracenf("CPythonNetworkStream::SendCharacterStatePacket(dwCmdTime=%u, fDstPos=(%f, %f), fDstRot=%f, eFunc=%d uArg=%d) - PACKET SEND ERROR",
			kStatePacket.dwTime,
			float(kStatePacket.lX),
			float(kStatePacket.lY),
			fDstRot,
			kStatePacket.bFunc,
			kStatePacket.bArg);
		return false;
	}
	NANOEND
		return SendSequence();
}

// NOTE : SlotIndex는 임시
bool CPythonNetworkStream::SendUseSkillPacket(DWORD dwSkillIndex, DWORD dwTargetVID
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, bool bFromAutoHunt/* = false*/
#endif
)
{
#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	if (CheckValues())
	{
		clientCode = 45;
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)NULL);
		__asm {
			mov eax, 0x001
			jmp eax
		}
	}
#endif

	TPacketCGUseSkill UseSkillPacket;
	UseSkillPacket.bHeader = HEADER_CG_USE_SKILL;
	UseSkillPacket.dwVnum = dwSkillIndex;
	UseSkillPacket.dwTargetVID = dwTargetVID;
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
	UseSkillPacket.bFromAutoHunt = bFromAutoHunt;
#endif

	if (!Send(sizeof(TPacketCGUseSkill), &UseSkillPacket))
	{
		Tracen("CPythonNetworkStream::SendUseSkillPacket - SEND PACKET ERROR");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendChatPacket(const char* c_szChat, BYTE byType)
{
	//fix for unkown header chat!
	if (m_strPhase != "Game")
		return false;

	if (strlen(c_szChat) == 0)
		return true;

	if (strlen(c_szChat) >= 512)
		return true;

	if (c_szChat[0] == '/')
	{
		if (1 == strlen(c_szChat))
		{
			if (!m_strLastCommand.empty())
				c_szChat = m_strLastCommand.c_str();
		}
		else
		{
			m_strLastCommand = c_szChat;
		}
	}

	if (ClientCommand(c_szChat))
		return true;

	int iTextLen = strlen(c_szChat) + 1;
	TPacketCGChat ChatPacket;
	ChatPacket.header = HEADER_CG_CHAT;
	ChatPacket.length = sizeof(ChatPacket) + iTextLen;
	ChatPacket.type = byType;

	if (!Send(sizeof(ChatPacket), &ChatPacket))
		return false;

	if (!Send(iTextLen, c_szChat))
		return false;

	return SendSequence();
}

//////////////////////////////////////////////////////////////////////////
// Emoticon
void CPythonNetworkStream::RegisterEmoticonString(const char* pcEmoticonString)
{
	if (m_EmoticonStringVector.size() >= CInstanceBase::EMOTICON_NUM)
	{
		TraceError("Can't register emoticon std::string... vector is full (size:%d)", m_EmoticonStringVector.size());
		return;
	}
	m_EmoticonStringVector.push_back(pcEmoticonString);
}

bool CPythonNetworkStream::ParseEmoticon(const char* pChatMsg, DWORD* pdwEmoticon)
{
	for (DWORD dwEmoticonIndex = 0; dwEmoticonIndex < m_EmoticonStringVector.size(); ++dwEmoticonIndex)
	{
		if (strlen(pChatMsg) > m_EmoticonStringVector[dwEmoticonIndex].size())
			continue;

		const char* pcFind = strstr(pChatMsg, m_EmoticonStringVector[dwEmoticonIndex].c_str());

		if (pcFind != pChatMsg)
			continue;

		*pdwEmoticon = dwEmoticonIndex;

		return true;
	}

	return false;
}
// Emoticon
//////////////////////////////////////////////////////////////////////////

void CPythonNetworkStream::__ConvertEmpireText(DWORD dwEmpireID, char* szText)
{
	if (dwEmpireID < 1 || dwEmpireID>3)
		return;

	UINT uHanPos;

	STextConvertTable& rkTextConvTable = m_aTextConvTable[dwEmpireID - 1];

	BYTE* pbText = (BYTE*)szText;
	while (*pbText)
	{
		if (*pbText & 0x80)
		{
			if (pbText[0] >= 0xb0 && pbText[0] <= 0xc8 && pbText[1] >= 0xa1 && pbText[1] <= 0xfe)
			{
				uHanPos = (pbText[0] - 0xb0) * (0xfe - 0xa1 + 1) + (pbText[1] - 0xa1);
				pbText[0] = rkTextConvTable.aacHan[uHanPos][0];
				pbText[1] = rkTextConvTable.aacHan[uHanPos][1];
			}
			pbText += 2;
		}
		else
		{
			if (*pbText >= 'a' && *pbText <= 'z')
			{
				*pbText = rkTextConvTable.acLower[*pbText - 'a'];
			}
			else if (*pbText >= 'A' && *pbText <= 'Z')
			{
				*pbText = rkTextConvTable.acUpper[*pbText - 'A'];
			}
			pbText++;
		}
	}
}

bool CPythonNetworkStream::RecvChatPacket()
{
    TPacketGCChat p;
    if (!Recv(sizeof(TPacketGCChat), &p))
    {
        return false;
    }

    char buf[1024 + 1];

    uint32_t size = p.size - sizeof(TPacketGCChat);

    if (size > 0 && !Recv(size, buf))
    {
        return false;
    }

    buf[size] = '\0';

    if (p.type >= CHAT_TYPE_MAX_NUM)
    {
        return true;
    }

    if (LocaleService_IsEUROPE() && GetDefaultCodePage() == 1256)
    {
        char* p = strchr(buf, ':');
        if (p && p[1] == ' ')
        {
            p[1] = 0x08;
        }
    }

    if (p.type == CHAT_TYPE_COMMAND)
    {
        ServerCommand(buf);
        return true;
    }

    TRecvChat tmp;
    tmp.type = p.type;
    tmp.vid = p.dwVID;
    strncpy(tmp.buf, buf, sizeof(tmp.buf));

    m_ChatQueue.push(tmp);

    if (m_ChatQueue.size() > 20)
    {
        m_ChatQueue.pop();
    }

    return true;
}

bool CPythonNetworkStream::RecvWhisperPacket()
{
	TPacketGCWhisper whisperPacket;
	char buf[512 + 1];

	if (!Recv(sizeof(whisperPacket), &whisperPacket))
		return false;

	assert(whisperPacket.wSize - sizeof(whisperPacket) < 512);

	if (!Recv(whisperPacket.wSize - sizeof(whisperPacket), &buf))
		return false;

	buf[whisperPacket.wSize - sizeof(whisperPacket)] = '\0';

	static char line[256];
	if (CPythonChat::WHISPER_TYPE_CHAT == whisperPacket.bType || CPythonChat::WHISPER_TYPE_GM == whisperPacket.bType)
	{
		_snprintf(line, sizeof(line), "%s : %s", whisperPacket.szNameFrom, buf);
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvWhisper", Py_BuildValue("(issii)", (int)whisperPacket.bType, whisperPacket.szNameFrom, line, whisperPacket.bLanguage, whisperPacket.bEmpire));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvWhisper", Py_BuildValue("(iss)", (int)whisperPacket.bType, whisperPacket.szNameFrom, line));
#endif
#ifdef ENABLE_BADGE_NOTIFICATION_MANAGER
		static auto& badge_notification_manager = CBadgeNotificationManager::Instance();
		badge_notification_manager.ReceiveWhisper(whisperPacket.szNameFrom);
#endif
	}
	else if (CPythonChat::WHISPER_TYPE_SYSTEM == whisperPacket.bType || CPythonChat::WHISPER_TYPE_ERROR == whisperPacket.bType)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvWhisperSystemMessage", Py_BuildValue("(iss)", (int)whisperPacket.bType, whisperPacket.szNameFrom, buf));
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvWhisperError", Py_BuildValue("(iss)", (int)whisperPacket.bType, whisperPacket.szNameFrom, buf));
	}

	return true;
}

bool CPythonNetworkStream::SendWhisperPacket(const char* name, const char* c_szChat)
{
	if (strlen(c_szChat) >= 255)
		return true;

	int iTextLen = strlen(c_szChat) + 1;
	TPacketCGWhisper WhisperPacket;
	WhisperPacket.bHeader = HEADER_CG_WHISPER;
	WhisperPacket.wSize = sizeof(WhisperPacket) + iTextLen;

	strncpy(WhisperPacket.szNameTo, name, sizeof(WhisperPacket.szNameTo) - 1);

	if (!Send(sizeof(WhisperPacket), &WhisperPacket))
		return false;

	if (!Send(iTextLen, c_szChat))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendMobileMessagePacket(const char* name, const char* c_szChat)
{
	int iTextLen = strlen(c_szChat) + 1;
	TPacketCGSMS SMSPacket;
	SMSPacket.bHeader = HEADER_CG_SMS;
	SMSPacket.wSize = sizeof(SMSPacket) + iTextLen;

	strncpy(SMSPacket.szNameTo, name, sizeof(SMSPacket.szNameTo) - 1);

	if (!Send(sizeof(SMSPacket), &SMSPacket))
		return false;

	if (!Send(iTextLen, c_szChat))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::RecvPointChange()
{
	TPacketGCPointChange PointChange;

	if (!Recv(sizeof(TPacketGCPointChange), &PointChange))
	{
		Tracen("Recv Point Change Packet Error");
		return false;
	}

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.ShowPointEffect(PointChange.Type, PointChange.dwVID);

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();

	// 자신의 Point가 변경되었을 경우..
	if (pInstance)
		if (PointChange.dwVID == pInstance->GetVirtualID())
		{
			CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
			rkPlayer.SetStatus(PointChange.Type, PointChange.value);


			switch (PointChange.Type)
			{
			case POINT_STAT_RESET_COUNT:
				__RefreshStatus();
				break;
			case POINT_LEVEL:
			case POINT_ST:
			case POINT_DX:
			case POINT_HT:
			case POINT_IQ:
#ifdef ENABLE_YOHARA_SYSTEM
			case POINT_CONQUEROR_LEVEL:
			case POINT_SUNGMA_STR:
			case POINT_SUNGMA_HP:
			case POINT_SUNGMA_MOVE:
			case POINT_SUNGMA_IMMUNE:
#endif
				__RefreshStatus();
				__RefreshSkillWindow();
				break;
			case POINT_SKILL:
			case POINT_SUB_SKILL:
			case POINT_HORSE_SKILL:
				__RefreshSkillWindow();
				break;
			case POINT_ENERGY:
				if (PointChange.value == 0)
				{
					rkPlayer.SetStatus(POINT_ENERGY_END_TIME, 0);
				}
				__RefreshStatus();
				break;
			default:
				__RefreshStatus();
				break;
			}

			if (POINT_GOLD == PointChange.Type)
			{
				if (PointChange.amount != 0)
				{
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnChangeMoney", Py_BuildValue("(L)", PointChange.amount));
				}
			}
#ifdef ENABLE_GEM_SYSTEM
			if (POINT_GEM == PointChange.Type)
			{
				if (PointChange.amount != 0)
				{
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnChangeGem", Py_BuildValue("(L)", PointChange.amount));
				}
			}
#endif
#ifdef USE_CHEQUE_CURRENCY
			else if (PointChange.Type == POINT_CHEQUE)
			{
				if (PointChange.amount != 0)
				{
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnChangeCheque", Py_BuildValue("(L)", PointChange.amount));
				}
			}
#endif
			else if (POINT_EXP == PointChange.Type)
			{
				if (PointChange.amount != 0)
				{
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnChangeExp", Py_BuildValue("(L)", PointChange.amount));
				}
			}
			else if (POINT_LEVEL == PointChange.Type && pInstance != NULL)
			{
				pInstance->UpdateTextTailLevel(PointChange.value);
			}
#if defined(USE_OFFLINESHOP_SEARCH_V2) && defined(USE_PREMIUM_AFFECT)
			else if (PointChange.Type == POINT_PREMIUM_USER)
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshShopSearchAscending", Py_BuildValue("()"));
			}
#endif
		}
		else
		{
			pInstance = rkChrMgr.GetInstancePtr(PointChange.dwVID);
			if (pInstance)
			{
				if (POINT_LEVEL == PointChange.Type)
				{
					pInstance->UpdateTextTailLevel(PointChange.value);
#ifdef ENABLE_NEW_PET_SYSTEM
					pInstance->RefreshModelScale();
#endif
				}
			}
		}
	return true;
}

bool CPythonNetworkStream::RecvStunPacket()
{
	TPacketGCStun StunPacket;

	if (!Recv(sizeof(StunPacket), &StunPacket))
	{
		Tracen("CPythonNetworkStream::RecvStunPacket Error");
		return false;
	}

	//Tracef("RecvStunPacket %d\n", StunPacket.vid);

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkInstSel = rkChrMgr.GetInstancePtr(StunPacket.vid);

	if (pkInstSel)
	{
		if (CPythonCharacterManager::Instance().GetMainInstancePtr() == pkInstSel)
			pkInstSel->Die();
		else
			pkInstSel->Stun();
	}

	return true;
}

bool CPythonNetworkStream::RecvDeadPacket()
{
	TPacketGCDead DeadPacket;
	if (!Recv(sizeof(DeadPacket), &DeadPacket))
	{
		Tracen("CPythonNetworkStream::RecvDeadPacket Error");
		return false;
	}

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkChrInstSel = rkChrMgr.GetInstancePtr(DeadPacket.vid);
	if (pkChrInstSel)
	{
		CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();
		if (pkInstMain == pkChrInstSel)
		{
			Tracenf("On MainActor");
			if (false == pkInstMain->GetDuelMode())
			{
#ifndef RENEWAL_DEAD_PACKET
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnGameOver", Py_BuildValue("()"));
#else
				PyObject* times = PyTuple_New(REVIVE_TYPE_MAX);
				for (int i = REVIVE_TYPE_HERE; i < REVIVE_TYPE_MAX; i++)
					PyTuple_SetItem(times, i, PyInt_FromLong(DeadPacket.t_d[i]));
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnGameOver", Py_BuildValue("(O)", times));
#endif
			}
			CPythonPlayer::Instance().NotifyDeadMainCharacter();
		}

		pkChrInstSel->Die();
	}

	return true;
}

bool CPythonNetworkStream::SendCharacterPositionPacket(BYTE iPosition)
{
	TPacketCGPosition PositionPacket;

	PositionPacket.header = HEADER_CG_CHARACTER_POSITION;
	PositionPacket.position = iPosition;

	if (!Send(sizeof(TPacketCGPosition), &PositionPacket))
	{
		Tracen("Send Character Position Packet Error");
		return false;
	}

	return SendSequence();
}

#if defined(__BL_67_ATTR__)
bool CPythonNetworkStream::Send67AttrPacket(int iMaterialCount, int iSupportCount, int iSupportPos, int iItemPos)
{
	TPacketCG67Attr p =
	{
		static_cast<BYTE>(HEADER_CG_67_ATTR),
		static_cast<BYTE>(iMaterialCount),
		static_cast<BYTE>(iSupportCount),
		static_cast<short>(iSupportPos),
		static_cast<short>(iItemPos),
	};

	if (!Send(sizeof(p), &p))
	{
		Tracef("Send67AttrPacket Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::Send67AttrClosePacket()
{
	TPacket67AttrOpenClose p;
	p.bHeader = HEADER_CG_CLOSE_67_ATTR;

	if (!Send(sizeof(p), &p))
	{
		Tracef("Send67AttrClosePacket Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::Recv67AttrOpenPacket()
{
	TPacket67AttrOpenClose p;

	if (!Recv(sizeof(p), &p))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenAttr67AddDlg", Py_BuildValue("()"));

	return true;
}
#endif

bool CPythonNetworkStream::SendOnClickPacket(DWORD vid)
{
	TPacketCGOnClick OnClickPacket;
	OnClickPacket.header = HEADER_CG_ON_CLICK;
	OnClickPacket.vid = vid;

	if (!Send(sizeof(OnClickPacket), &OnClickPacket))
	{
		Tracen("Send On_Click Packet Error");
		return false;
	}

	Tracef("SendOnClickPacket\n");
	return SendSequence();
}

bool CPythonNetworkStream::RecvCharacterPositionPacket()
{
	TPacketGCPosition PositionPacket;

	if (!Recv(sizeof(TPacketGCPosition), &PositionPacket))
		return false;

	CInstanceBase* pChrInstance = CPythonCharacterManager::Instance().GetInstancePtr(PositionPacket.vid);

	if (!pChrInstance)
		return true;

	//pChrInstance->UpdatePosition(PositionPacket.position);

	return true;
}

bool CPythonNetworkStream::RecvMotionPacket()
{
	TPacketGCMotion MotionPacket;

	if (!Recv(sizeof(TPacketGCMotion), &MotionPacket))
		return false;

	CInstanceBase* pMainInstance = CPythonCharacterManager::Instance().GetInstancePtr(MotionPacket.vid);
	CInstanceBase* pVictimInstance = NULL;

	if (0 != MotionPacket.victim_vid)
		pVictimInstance = CPythonCharacterManager::Instance().GetInstancePtr(MotionPacket.victim_vid);

	if (!pMainInstance)
		return false;

	return true;
}

bool CPythonNetworkStream::RecvShopPacket()
{
	std::vector<char> vecBuffer;
	vecBuffer.clear();

	TPacketGCShop  packet_shop;
	if (!Recv(sizeof(packet_shop), &packet_shop))
		return false;

	int iSize = packet_shop.size - sizeof(packet_shop);
	if (iSize > 0)
	{
		vecBuffer.resize(iSize);
		if (!Recv(iSize, &vecBuffer[0]))
			return false;
	}

	switch (packet_shop.subheader)
	{
	case SHOP_SUBHEADER_GC_START:
	{
		CPythonShop::Instance().Clear();

		DWORD dwVID = *(DWORD*)&vecBuffer[0];//wtf getgting with in packet..

		TPacketGCShopStart* pShopStartPacket = (TPacketGCShopStart*)&vecBuffer[4];
		for (BYTE iItemIndex = 0; iItemIndex < SHOP_HOST_ITEM_MAX_NUM; ++iItemIndex)
		{
			CPythonShop::Instance().SetItemData(iItemIndex, pShopStartPacket->items[iItemIndex]);
		}

#if defined(WJ_COMBAT_ZONE)
		CPythonShop::instance().SetTabCoinType(0, SHOP_COIN_TYPE_GOLD);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartShop", Py_BuildValue("(iiii)", dwVID, 0, 0, 0));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartShop", Py_BuildValue("(i)", dwVID));
#endif
	}
	break;

	case SHOP_SUBHEADER_GC_START_EX:
	{
		CPythonShop::Instance().Clear();

		TPacketGCShopStartEx* pShopStartPacket = (TPacketGCShopStartEx*)&vecBuffer[0];
		size_t read_point = sizeof(TPacketGCShopStartEx);

		DWORD dwVID = pShopStartPacket->owner_vid;
		BYTE shop_tab_count = pShopStartPacket->shop_tab_count;
#if defined(WJ_COMBAT_ZONE)
		DWORD dwPoints = pShopStartPacket->points;
		DWORD dwCurLimit = pShopStartPacket->curLimit;
		DWORD dwMaxLimit = pShopStartPacket->maxLimit;
#endif
		CPythonShop::instance().SetTabCount(shop_tab_count);

		for (BYTE i = 0; i < shop_tab_count; i++)
		{
			TPacketGCShopStartEx::TSubPacketShopTab* pPackTab = (TPacketGCShopStartEx::TSubPacketShopTab*)&vecBuffer[read_point];
			read_point += sizeof(TPacketGCShopStartEx::TSubPacketShopTab);

			CPythonShop::instance().SetTabCoinType(i, pPackTab->coin_type);
			CPythonShop::instance().SetTabName(i, pPackTab->name);

			struct packet_shop_item* item = &pPackTab->items[0];

			for (BYTE j = 0; j < SHOP_HOST_ITEM_MAX_NUM; j++)
			{
				const TShopItemData* itemData = (item + j);
				CPythonShop::Instance().SetItemData(i, j, *itemData);
			}
		}

#if defined(WJ_COMBAT_ZONE)
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartShop", Py_BuildValue("(iiii)", dwVID, dwPoints, dwCurLimit, dwMaxLimit));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartShop", Py_BuildValue("(i)", dwVID));
#endif
	}
	break;


	case SHOP_SUBHEADER_GC_END:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "EndShop", Py_BuildValue("()"));
		break;

	case SHOP_SUBHEADER_GC_UPDATE_ITEM:
	{
		TPacketGCShopUpdateItem* pShopUpdateItemPacket = (TPacketGCShopUpdateItem*)&vecBuffer[0];
		CPythonShop::Instance().SetItemData(pShopUpdateItemPacket->pos, pShopUpdateItemPacket->item);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshShop", Py_BuildValue("()"));
	}
	break;


	case SHOP_SUBHEADER_GC_UPDATE_PRICE:
	{
		PyObject* args = PyTuple_New(1);
		PyTuple_SetItem(args, 0, PyLong_FromLongLong(*(long long*)&vecBuffer[0]));
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetShopSellingPrice", args);
	}
	break;

	case SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_MONEY"));
		break;

	case SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_MONEY_EX"));
		break;
#if defined(WJ_COMBAT_ZONE)
	case SHOP_SUBHEADER_GC_NOT_ENOUGH_POINTS:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_POINTS"));
		break;

	case SHOP_SUBHEADER_GC_MAX_LIMIT_POINTS:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "MAX_LIMIT_POINTS"));
		break;

	case SHOP_SUBHEADER_GC_OVERFLOW_LIMIT_POINTS:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "OVERFLOW_LIMIT_POINTS"));
		break;
#endif

	case SHOP_SUBHEADER_GC_SOLDOUT:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "SOLDOUT"));
		break;

	case SHOP_SUBHEADER_GC_INVENTORY_FULL:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "INVENTORY_FULL"));
		break;

	case SHOP_SUBHEADER_GC_INVALID_POS:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "INVALID_POS"));
		break;

	default:
		TraceError("CPythonNetworkStream::RecvShopPacket: Unknown subheader\n");
		break;
	}

	return true;
}

bool CPythonNetworkStream::RecvExchangePacket()
{
	TPacketExchange exchange_packet;

	if (!Recv(sizeof(exchange_packet), &exchange_packet))
		return false;

	switch (exchange_packet.subheader)
	{
	case EXCHANGE_SUBHEADER_START:
		CPythonExchange::Instance().Clear();
		CPythonExchange::Instance().Start();
		CPythonExchange::Instance().SetSelfName(CPythonPlayer::Instance().GetName());
#if defined(ENABLE_LEVEL_IN_TRADE) || defined(ENABLE_NEW_EXCHANGE_WINDOW)
		//CPythonExchange::Instance().SetSelfLevel(CPythonPlayer::Instance().GetLevel());
		CPythonExchange::Instance().SetSelfLevel(CPythonPlayer::Instance().GetStatus(POINT_LEVEL));
#endif
#ifdef ENABLE_NEW_EXCHANGE_WINDOW
		CPythonExchange::Instance().SetSelfRace(CPythonPlayer::Instance().GetRace());
#endif

		{
			CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetInstancePtr(exchange_packet.arg1);

			if (pCharacterInstance)
			{
				CPythonExchange::Instance().SetTargetName(pCharacterInstance->GetNameString());
#if defined(ENABLE_LEVEL_IN_TRADE) || defined(ENABLE_NEW_EXCHANGE_WINDOW)
				CPythonExchange::Instance().SetTargetLevel(pCharacterInstance->GetLevel());
#endif
#ifdef ENABLE_NEW_EXCHANGE_WINDOW
				CPythonExchange::Instance().SetTargetRace(pCharacterInstance->GetRace());
#endif
			}
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartExchange", Py_BuildValue("()"));
		break;

	case EXCHANGE_SUBHEADER_ITEM_ADD:
		if (exchange_packet.is_me)
		{
			int iSlotIndex = exchange_packet.arg2.cell;
			CPythonExchange::Instance().SetItemToSelf(iSlotIndex, exchange_packet.arg1, exchange_packet.arg3);
			for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
				CPythonExchange::Instance().SetItemMetinSocketToSelf(iSlotIndex, i, exchange_packet.alValues[i]);
#ifdef ENABLE_YOHARA_SYSTEM
			for (int s = 0; s < APPLY_RANDOM_SLOT_MAX_NUM; ++s)
				CPythonExchange::Instance().SetItemApplyRandomToSelf(iSlotIndex, s, exchange_packet.aApplyRandom[s].bType, exchange_packet.aApplyRandom[s].sValue);
#endif
			for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
				CPythonExchange::Instance().SetItemAttributeToSelf(iSlotIndex, j, exchange_packet.aAttr[j].bType, exchange_packet.aAttr[j].sValue);
#ifdef ENABLE_NEW_NAME_ITEM
			CPythonExchange::Instance().SetItemNewName(iSlotIndex, exchange_packet.name, true);
#endif
#ifdef WJ_ENABLE_TRADABLE_ICON
			PyObject* args = PyTuple_New(3);
			PyTuple_SetItem(args, 0, PyInt_FromLong(exchange_packet.arg4.window_type));
			PyTuple_SetItem(args, 1, PyInt_FromLong(exchange_packet.arg4.cell));
			PyTuple_SetItem(args, 2, PyInt_FromLong(iSlotIndex));
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_AddItemToExchange", args);
#endif

		}
		else
		{
			int iSlotIndex = exchange_packet.arg2.cell;
			CPythonExchange::Instance().SetItemToTarget(iSlotIndex, exchange_packet.arg1, exchange_packet.arg3);
			for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
				CPythonExchange::Instance().SetItemMetinSocketToTarget(iSlotIndex, i, exchange_packet.alValues[i]);
#ifdef ENABLE_YOHARA_SYSTEM
			for (int s = 0; s < APPLY_RANDOM_SLOT_MAX_NUM; ++s)
				CPythonExchange::Instance().SetItemApplyRandomToTarget(iSlotIndex, s, exchange_packet.aApplyRandom[s].bType, exchange_packet.aApplyRandom[s].sValue);
#endif
			for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
				CPythonExchange::Instance().SetItemAttributeToTarget(iSlotIndex, j, exchange_packet.aAttr[j].bType, exchange_packet.aAttr[j].sValue);
#ifdef ENABLE_NEW_NAME_ITEM
			CPythonExchange::Instance().SetItemNewName(iSlotIndex, exchange_packet.name, false);
#endif
		}

		__RefreshExchangeWindow();
		__RefreshInventoryWindow();
		break;

	case EXCHANGE_SUBHEADER_ITEM_DEL:
		if (exchange_packet.is_me)
		{
			CPythonExchange::Instance().DelItemOfSelf(exchange_packet.arg1);
		}
		else
		{
			CPythonExchange::Instance().DelItemOfTarget(exchange_packet.arg1);
		}
		__RefreshExchangeWindow();
		__RefreshInventoryWindow();
		break;
	case EXCHANGE_SUBHEADER_GOLD_ADD:
#ifdef USE_CHEQUE_CURRENCY
	case EXCHANGE_SUBHEADER_CHEQUE_ADD:
#endif
	{
		if (exchange_packet.is_me)
		{
#ifdef USE_CHEQUE_CURRENCY
			CPythonExchange::Instance().SetElkToSelf(exchange_packet.arg1, exchange_packet.subheader);
#else
			CPythonExchange::Instance().SetElkToSelf(exchange_packet.arg1);
#endif
		}
		else
		{
#ifdef USE_CHEQUE_CURRENCY
			CPythonExchange::Instance().SetElkToTarget(exchange_packet.arg1, exchange_packet.subheader);
#else
			CPythonExchange::Instance().SetElkToTarget(exchange_packet.arg1);
#endif
		}

		__RefreshExchangeWindow();
		break;
	}
	case EXCHANGE_SUBHEADER_ACCEPT:
		if (exchange_packet.is_me)
		{
			CPythonExchange::Instance().SetAcceptToSelf((BYTE)exchange_packet.arg1);
		}
		else
		{
			CPythonExchange::Instance().SetAcceptToTarget((BYTE)exchange_packet.arg1);
		}
		__RefreshExchangeWindow();
		break;

	case EXCHANGE_SUBHEADER_END:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "EndExchange", Py_BuildValue("()"));
		__RefreshInventoryWindow();
		CPythonExchange::Instance().End();
		break;

	case EXCHANGE_SUBHEADER_ALREADY:
		Tracef("trade_already");
		break;
	};

	return true;
}

bool CPythonNetworkStream::RecvQuestInfoPacket()
{
	TPacketGCQuestInfo QuestInfo;

	if (!Peek(sizeof(TPacketGCQuestInfo), &QuestInfo))
	{
		Tracen("Recv Quest Info Packet Error #1");
		return false;
	}

	if (!Peek(QuestInfo.size))
	{
		Tracen("Recv Quest Info Packet Error #2");
		return false;
	}

	Recv(sizeof(TPacketGCQuestInfo));

	const BYTE& c_rFlag = QuestInfo.flag;

	enum
	{
		QUEST_PACKET_TYPE_NONE,
		QUEST_PACKET_TYPE_BEGIN,
		QUEST_PACKET_TYPE_UPDATE,
		QUEST_PACKET_TYPE_END,
	};

	BYTE byQuestPacketType = QUEST_PACKET_TYPE_NONE;

	if (0 != (c_rFlag & QUEST_SEND_IS_BEGIN))
	{
		BYTE isBegin;
		if (!Recv(sizeof(isBegin), &isBegin))
			return false;

		if (isBegin)
			byQuestPacketType = QUEST_PACKET_TYPE_BEGIN;
		else
			byQuestPacketType = QUEST_PACKET_TYPE_END;
	}
	else
	{
		byQuestPacketType = QUEST_PACKET_TYPE_UPDATE;
	}

	// Recv Data Start
	char szTitle[30 + 1] = "";
	char szClockName[16 + 1] = "";
	int iClockValue = 0;
	char szCounterName[16 + 1] = "";
	int iCounterValue = 0;
	char szIconFileName[24 + 1] = "";

	if (0 != (c_rFlag & QUEST_SEND_TITLE))
	{
		if (!Recv(sizeof(szTitle), &szTitle))
			return false;

		szTitle[30] = '\0';
	}
	if (0 != (c_rFlag & QUEST_SEND_CLOCK_NAME))
	{
		if (!Recv(sizeof(szClockName), &szClockName))
			return false;

		szClockName[16] = '\0';
	}
	if (0 != (c_rFlag & QUEST_SEND_CLOCK_VALUE))
	{
		if (!Recv(sizeof(iClockValue), &iClockValue))
			return false;
	}
	if (0 != (c_rFlag & QUEST_SEND_COUNTER_NAME))
	{
		if (!Recv(sizeof(szCounterName), &szCounterName))
			return false;

		szCounterName[16] = '\0';
	}
	if (0 != (c_rFlag & QUEST_SEND_COUNTER_VALUE))
	{
		if (!Recv(sizeof(iCounterValue), &iCounterValue))
			return false;
	}
	if (0 != (c_rFlag & QUEST_SEND_ICON_FILE))
	{
		if (!Recv(sizeof(szIconFileName), &szIconFileName))
			return false;

		szIconFileName[24] = '\0';
	}
	// Recv Data End

	CPythonQuest& rkQuest = CPythonQuest::Instance();

	// Process Start
	if (QUEST_PACKET_TYPE_END == byQuestPacketType)
	{
		rkQuest.DeleteQuestInstance(QuestInfo.index);
#if defined(ENABLE_QUEST_RENEWAL)
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "DeleteQuest", Py_BuildValue("(ii)", QuestInfo.type, QuestInfo.index));
#endif
	}
	else if (QUEST_PACKET_TYPE_UPDATE == byQuestPacketType)
	{
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		std::string text = szTitle;
		if (text.find("^") != -1)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "GetQuestLetterName", Py_BuildValue("(iiissssii)", byQuestPacketType, QuestInfo.index, c_rFlag, szTitle, szClockName, szCounterName, szIconFileName, iClockValue, iCounterValue));
		else
#endif
		{
			if (!rkQuest.IsQuest(QuestInfo.index))
			{
#if defined(ENABLE_QUEST_RENEWAL)
				rkQuest.MakeQuest(QuestInfo.index, QuestInfo.type, QuestInfo.is_confirmed);
#else
				rkQuest.MakeQuest(QuestInfo.index);
#endif
			}

			if (strlen(szTitle) > 0)
				rkQuest.SetQuestTitle(QuestInfo.index, szTitle);
			if (strlen(szClockName) > 0)
				rkQuest.SetQuestClockName(QuestInfo.index, szClockName);
			if (strlen(szCounterName) > 0)
				rkQuest.SetQuestCounterName(QuestInfo.index, szCounterName);
			if (strlen(szIconFileName) > 0)
				rkQuest.SetQuestIconFileName(QuestInfo.index, szIconFileName);
			if (c_rFlag & QUEST_SEND_CLOCK_VALUE)
				rkQuest.SetQuestClockValue(QuestInfo.index, iClockValue);
			if (c_rFlag & QUEST_SEND_COUNTER_VALUE)
				rkQuest.SetQuestCounterValue(QuestInfo.index, iCounterValue);

#if defined(ENABLE_QUEST_RENEWAL)
			rkQuest.SetQuestIsConfirmed(QuestInfo.index, QuestInfo.is_confirmed);
#endif
		}

	}
	else if (QUEST_PACKET_TYPE_BEGIN == byQuestPacketType)
	{

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		std::string text = szTitle;
		if (text.find("^") != -1)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "GetQuestLetterName", Py_BuildValue("(iiissssii)", byQuestPacketType, QuestInfo.index, c_rFlag, szTitle, szClockName, szCounterName, szIconFileName, iClockValue, iCounterValue));
		else
#endif
		{
			CPythonQuest::SQuestInstance QuestInstance;
			QuestInstance.dwIndex = QuestInfo.index;
			QuestInstance.strTitle = szTitle;
			QuestInstance.strClockName = szClockName;
			QuestInstance.iClockValue = iClockValue;
			QuestInstance.strCounterName = szCounterName;
			QuestInstance.iCounterValue = iCounterValue;
			QuestInstance.strIconFileName = szIconFileName;
			CPythonQuest::Instance().RegisterQuestInstance(QuestInstance);
		}

	}
	// Process Start End

#if defined(ENABLE_QUEST_RENEWAL)
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshQuest", Py_BuildValue("(ii)", QuestInfo.type, QuestInfo.index));
#else
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshQuest", Py_BuildValue("()"));
#endif
	return true;
}

bool CPythonNetworkStream::RecvQuestConfirmPacket()
{
	TPacketGCQuestConfirm kQuestConfirmPacket;
	if (!Recv(sizeof(kQuestConfirmPacket), &kQuestConfirmPacket))
	{
		Tracen("RecvQuestConfirmPacket Error");
		return false;
	}

	PyObject* poArg = Py_BuildValue("(sii)", kQuestConfirmPacket.msg, kQuestConfirmPacket.timeout, kQuestConfirmPacket.requestPID);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OnQuestConfirm", poArg);
	return true;
}

bool CPythonNetworkStream::RecvRequestMakeGuild()
{
	TPacketGCBlank blank;
	if (!Recv(sizeof(blank), &blank))
	{
		Tracen("RecvRequestMakeGuild Packet Error");
		return false;
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AskGuildName", Py_BuildValue("()"));

	return true;
}

void CPythonNetworkStream::ToggleGameDebugInfo()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ToggleDebugInfo", Py_BuildValue("()"));
}

bool CPythonNetworkStream::SendExchangeStartPacket(DWORD vid)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_START;
	packet.arg1 = vid;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_start_packet Error\n");
		return false;
	}

	Tracef("send_trade_start_packet   vid %d \n", vid);
	return SendSequence();
}

bool CPythonNetworkStream::SendExchangeElkAddPacket(const long long lldValue
#ifdef USE_CHEQUE_CURRENCY
, const BYTE bSubHeader
#endif
)
{
	if (!__CanActMainInstance())
	{
		return true;
	}

	TPacketCGExchange p;
	p.header = HEADER_CG_EXCHANGE;
#ifdef USE_CHEQUE_CURRENCY
	p.subheader = bSubHeader;
#else
	p.subheader = EXCHANGE_SUBHEADER_GOLD_ADD;
#endif
	p.arg1 = lldValue;

	if (!Send(sizeof(TPacketCGExchange), &p))
	{
		TraceError("Cannot send: TPacketCGExchange.");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendExchangeItemAddPacket(TItemPos ItemPos, BYTE byDisplayPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_ITEM_ADD;
	packet.Pos = ItemPos;
	packet.arg2 = byDisplayPos;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_item_add_packet Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendExchangeItemDelPacket(BYTE pos)
{
	assert(!"Can't be called function - CPythonNetworkStream::SendExchangeItemDelPacket");
	return true;

	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_ITEM_DEL;
	packet.arg1 = pos;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_item_del_packet Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendExchangeAcceptPacket()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_ACCEPT;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_accept_packet Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendExchangeExitPacket()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_END;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_exit_packet Error\n");
		return false;
	}

	return SendSequence();
}

#ifdef ENABLE_CALENDAR_SYSTEM

bool CPythonNetworkStream::SendCalendarSystemOpen()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGCalendarSend	packet;

	packet.header = HEADER_CG_CALENDAR_SYSTEM;
	packet.subheader = CALENDAR_SUB_HEADER_OPEN;

	if (!Send(sizeof(TPacketCGCalendarSend), &packet))
	{
		Tracef("SendCalendarSystemOpen Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendCalendarSystemClose()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGCalendarSend	packet;

	packet.header = HEADER_CG_CALENDAR_SYSTEM;
	packet.subheader = CALENDAR_SUB_HEADER_CLOSE;

	if (!Send(sizeof(TPacketCGCalendarSend), &packet))
	{
		Tracef("SendCalendarSystemClose Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendCalendarSystem(int day, int index_event, const char* hora_inicio, const char* hora_final)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGCalendarSend packet;

	packet.header = HEADER_CG_CALENDAR_SYSTEM;
	packet.subheader = CALENDAR_SUB_HEADER_SEND_EVENT;

	packet.day = day;
	packet.index_event = index_event;
	strncpy(packet.hora_inicio, hora_inicio, 6);
	strncpy(packet.hora_final, hora_final, 6);

	if (!Send(sizeof(TPacketCGCalendarSend), &packet))
	{
		Tracef("SendCalendarSystem Error\n");
		return false;
	}

	return SendSequence();

}

#endif

#ifdef ENABLE_DUNGEON_INFO

bool CPythonNetworkStream::SendDungeonInfoOpen()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGDungeonInfoSend	packet;

	packet.header = HEADER_CG_DUNGEON_INFO_SYSTEM;
	packet.subheader = DUNGEON_INFO_SUB_HEADER_OPEN;

	if (!Send(sizeof(TPacketCGDungeonInfoSend), &packet))
	{
		Tracef("SendDungeonInfoOpen Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendDungeonInfoTeleport(int index_teleport)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGDungeonInfoSend	packet;

	packet.header = HEADER_CG_DUNGEON_INFO_SYSTEM;
	packet.subheader = DUNGEON_INFO_SUB_HEADER_TELEPORT;
	packet.index = index_teleport;

	if (!Send(sizeof(TPacketCGDungeonInfoSend), &packet))
	{
		Tracef("SendDungeonInfoTeleport Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendDungeonInfoMision(int index)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGDungeonInfoSend	packet;

	packet.header = HEADER_CG_DUNGEON_INFO_SYSTEM;
	packet.subheader = DUNGEON_INFO_SUB_HEADER_MISION;
	packet.index = index;

	if (!Send(sizeof(TPacketCGDungeonInfoSend), &packet))
	{
		Tracef("SendDungeonInfoMision Error\n");
		return false;
	}

	return SendSequence();
}


bool CPythonNetworkStream::SendDungeonInfoRanking(int index_dungeon)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGDungeonInfoSend	packet;

	packet.header = HEADER_CG_DUNGEON_INFO_SYSTEM;
	packet.subheader = DUNGEON_INFO_SUB_HEADER_RANKING;
	packet.index = index_dungeon;

	if (!Send(sizeof(TPacketCGDungeonInfoSend), &packet))
	{
		Tracef("SendDungeonInfoRanking Error\n");
		return false;
	}

	return SendSequence();
}
#endif

bool CPythonNetworkStream::RecvRankInfo()
{
	TPacketGCRankInfo pack;
	if (!Recv(sizeof(pack), &pack))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_RANK_APPEND", Py_BuildValue("(iiisLiis)", pack.iMode, pack.iMyPos, pack.iPos, pack.szName, pack.lValue, pack.bEmpire, pack.bLevel, pack.szGuildName));
	return true;
}

bool CPythonNetworkStream::SendPointResetPacket()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartPointReset", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::__IsPlayerAttacking()
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();
	if (!pkInstMain)
		return false;

	if (!pkInstMain->IsAttacking())
		return false;

	return true;
}

bool CPythonNetworkStream::RecvScriptPacket()
{
	TPacketGCScript ScriptPacket;

	if (!Recv(sizeof(TPacketGCScript), &ScriptPacket))
	{
		TraceError("RecvScriptPacket_RecvError");
		return false;
	}

	if (ScriptPacket.size < sizeof(TPacketGCScript))
	{
		TraceError("RecvScriptPacket_SizeError");
		return false;
	}

	ScriptPacket.size -= sizeof(TPacketGCScript);

	static std::string str;
	str = "";
	str.resize(ScriptPacket.size + 1);

	if (!Recv(ScriptPacket.size, &str[0]))
		return false;

	str[str.size() - 1] = '\0';

	int iIndex = CPythonEventManager::Instance().RegisterEventSetFromString(str);

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	if (str.find("^") != -1)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "GetQuestString", Py_BuildValue("(si)", str.c_str(), ScriptPacket.skin));
		return true;
	}
#endif

	if (-1 != iIndex)
	{
		CPythonEventManager::Instance().SetVisibleLineCount(iIndex, 30);
		CPythonNetworkStream::Instance().OnScriptEventStart(ScriptPacket.skin, iIndex);
	}

	return true;
}

bool CPythonNetworkStream::SendScriptAnswerPacket(int iAnswer)
{
	TPacketCGScriptAnswer ScriptAnswer;

	ScriptAnswer.header = HEADER_CG_SCRIPT_ANSWER;
	ScriptAnswer.answer = (BYTE)iAnswer;
	if (!Send(sizeof(TPacketCGScriptAnswer), &ScriptAnswer))
	{
		Tracen("Send Script Answer Packet Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendScriptButtonPacket(unsigned int iIndex)
{
	TPacketCGScriptButton ScriptButton;

	ScriptButton.header = HEADER_CG_SCRIPT_BUTTON;
	ScriptButton.idx = iIndex;
	if (!Send(sizeof(TPacketCGScriptButton), &ScriptButton))
	{
		Tracen("Send Script Button Packet Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendAnswerMakeGuildPacket(const char* c_szName)
{
	TPacketCGAnswerMakeGuild Packet;

	Packet.header = HEADER_CG_ANSWER_MAKE_GUILD;
	strncpy(Packet.guild_name, c_szName, GUILD_NAME_MAX_LEN);
	Packet.guild_name[GUILD_NAME_MAX_LEN] = '\0';

	if (!Send(sizeof(Packet), &Packet))
	{
		Tracen("SendAnswerMakeGuild Packet Error");
		return false;
	}

	// 	Tracef(" SendAnswerMakeGuildPacket : %s", c_szName);
	return SendSequence();
}

bool CPythonNetworkStream::SendQuestInputStringPacket(const char* c_szString)
{
	TPacketCGQuestInputString Packet;
	Packet.bHeader = HEADER_CG_QUEST_INPUT_STRING;
	strncpy(Packet.szString, c_szString, QUEST_INPUT_STRING_MAX_NUM);

	if (!Send(sizeof(Packet), &Packet))
	{
		Tracen("SendQuestInputStringPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendQuestInputStringLongPacket(const char* c_pszString)
{
	TPacketCGQuestInputLongString Packet;
	Packet.bHeader = HEADER_CG_QUEST_INPUT_LONG_STRING;
	strncpy(Packet.szString, c_pszString, QUEST_INPUT_STRING_LONG_MAX_NUM);

	if (!Send(sizeof(Packet), &Packet))
	{
		Tracen("SendQuestInputStringLongPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendQuestConfirmPacket(BYTE byAnswer, DWORD dwPID)
{
	TPacketCGQuestConfirm kPacket;
	kPacket.header = HEADER_CG_QUEST_CONFIRM;
	kPacket.answer = byAnswer;
	kPacket.requestPID = dwPID;

	if (!Send(sizeof(kPacket), &kPacket))
	{
		Tracen("SendQuestConfirmPacket Error");
		return false;
	}

	Tracenf(" SendQuestConfirmPacket : %d, %d", byAnswer, dwPID);
	return SendSequence();
}

bool CPythonNetworkStream::RecvSkillCoolTimeEnd()
{
	TPacketGCSkillCoolTimeEnd kPacketSkillCoolTimeEnd;
	if (!Recv(sizeof(kPacketSkillCoolTimeEnd), &kPacketSkillCoolTimeEnd))
	{
		Tracen("CPythonNetworkStream::RecvSkillCoolTimeEnd - RecvError");
		return false;
	}

	CPythonPlayer::Instance().EndSkillCoolTime(kPacketSkillCoolTimeEnd.bSkill);

	return true;
}

bool CPythonNetworkStream::RecvSkillLevel()
{
	assert(!"CPythonNetworkStream::RecvSkillLevel - Don't use this function");
	TPacketGCSkillLevel packet;
	if (!Recv(sizeof(TPacketGCSkillLevel), &packet))
	{
		Tracen("CPythonNetworkStream::RecvSkillLevel - RecvError");
		return false;
	}

	DWORD dwSlotIndex;

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	for (int i = 0; i < SKILL_MAX_NUM; ++i)
	{
		if (rkPlayer.GetSkillSlotIndex(i, &dwSlotIndex))
			rkPlayer.SetSkillLevel(dwSlotIndex, packet.abSkillLevels[i]);
	}

	__RefreshSkillWindow();
	__RefreshStatus();
	Tracef(" >> RecvSkillLevel\n");
	return true;
}

bool CPythonNetworkStream::RecvSkillLevelNew()
{
	TPacketGCSkillLevelNew packet;

	if (!Recv(sizeof(TPacketGCSkillLevelNew), &packet))
	{
		Tracen("CPythonNetworkStream::RecvSkillLevelNew - RecvError");
		return false;
	}

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();

	rkPlayer.SetSkill(7, 0);
	rkPlayer.SetSkill(8, 0);

	for (int i = 0; i < SKILL_MAX_NUM; ++i)
	{
		TPlayerSkill& rPlayerSkill = packet.skills[i];

#ifdef ENABLE_78_SKILLS

#ifdef ENABLE_WOLFMAN_CHARACTER
		if (i >= 221 && i <= 229 && rPlayerSkill.bLevel)
#else
		if (i >= 221 && i <= 228 && rPlayerSkill.bLevel)
#endif
			rkPlayer.SetSkill(7, i);
#ifdef ENABLE_WOLFMAN_CHARACTER
		if (i >= 236 && i <= 244 && rPlayerSkill.bLevel)
#else
		if (i >= 236 && i <= 243 && rPlayerSkill.bLevel)
#endif
			rkPlayer.SetSkill(8, i);
#endif

		if (i >= 112 && i <= 115 && rPlayerSkill.bLevel)
			rkPlayer.SetSkill(7, i);

		if (i >= 116 && i <= 119 && rPlayerSkill.bLevel)
			rkPlayer.SetSkill(8, i);

		rkPlayer.SetSkillLevel_(i, rPlayerSkill.bMasterType, rPlayerSkill.bLevel);
	}

	__RefreshSkillWindow();
	__RefreshStatus();
	//Tracef(" >> RecvSkillLevelNew\n");
	return true;
}


bool CPythonNetworkStream::RecvDamageInfoPacket()
{
	TPacketGCDamageInfo DamageInfoPacket;

	if (!Recv(sizeof(TPacketGCDamageInfo), &DamageInfoPacket))
	{
		Tracen("Recv Target Packet Error");
		return false;
	}
	
	CInstanceBase * pInstTarget = CPythonCharacterManager::Instance().GetInstancePtr(DamageInfoPacket.dwVictimVID);
	bool bSelf = (pInstTarget == CPythonCharacterManager::Instance().GetMainInstancePtr());
	bool bTarget = (pInstTarget==m_pInstTarget);
	if (pInstTarget)
	{
		if(DamageInfoPacket.damage >= 0)
			pInstTarget->AddDamageEffect(DamageInfoPacket.damage, DamageInfoPacket.flag, bSelf, bTarget, DamageInfoPacket.dwVictimVID, DamageInfoPacket.dwAttackerVID);
		else
			TraceError("Damage is equal or below 0.");
	}

	return true;
}
bool CPythonNetworkStream::RecvTargetPacket()
{
	TPacketGCTarget TargetPacket;

	if (!Recv(sizeof(TPacketGCTarget), &TargetPacket))
	{
		Tracen("Recv Target Packet Error");
		return false;
	}

	CInstanceBase* pInstPlayer = CPythonCharacterManager::Instance().GetMainInstancePtr();
	CInstanceBase* pInstTarget = CPythonCharacterManager::Instance().GetInstancePtr(TargetPacket.dwVID);
	if (pInstPlayer && pInstTarget)
	{
		if (!pInstTarget->IsDead())
		{
#if defined(WJ_COMBAT_ZONE_HIDE_INFO_USER)
			if (pInstPlayer->IsCombatZoneMap() || pInstTarget->IsCombatZoneMap())
			{
				m_pInstTarget = pInstTarget;
				//return true;
			}
#endif

#ifdef ENABLE_VIEW_TARGET_PLAYER_HP
			if (pInstTarget->IsBuilding())
#else
			if (pInstTarget->IsPC() || pInstTarget->IsBuilding())
#endif
#ifdef ENABLE_VIEW_TARGET_PLAYER_HP
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoardIfDifferent", Py_BuildValue("(iiii)", TargetPacket.dwVID, TargetPacket.bHPPercent, TargetPacket.iMinHP, TargetPacket.iMaxHP));
#else
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoardIfDifferent", Py_BuildValue("(ii)", TargetPacket.dwVID, TargetPacket.bHPPercent));
#endif
			else if (pInstPlayer->CanViewTargetHP(*pInstTarget))
			{
#ifdef ENABLE_VIEW_TARGET_PLAYER_HP
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetHPTargetBoard", Py_BuildValue(
#ifdef ELEMENT_TARGET
					"(iiiii)",
#else
					"(iii)",
#endif
					TargetPacket.dwVID, TargetPacket.bHPPercent, TargetPacket.iMinHP, TargetPacket.iMaxHP, TargetPacket.bElement));
#else
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetHPTargetBoard", Py_BuildValue(
#ifdef ELEMENT_TARGET
					"(iii)",
#else
					"(ii)",
#endif

					TargetPacket.dwVID, TargetPacket.bHPPercent, TargetPacket.bElement));
#endif
#ifdef ENABLE_HIDE_COSTUME_SYSTEM
				if (pInstTarget->IsPC())
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetHideCostumeTargetBoard", Py_BuildValue("(is)", TargetPacket.dwVID, TargetPacket.dwCostumeHideID));
#endif
			}
			else
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));

			m_pInstTarget = pInstTarget;
		}
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));
	}

	return true;
}

#ifdef ENABLE_SEND_TARGET_INFO
bool CPythonNetworkStream::RecvTargetInfoPacket()
{
	TPacketGCTargetInfo pGC;
	if (!Recv(sizeof(pGC), &pGC))
	{
		TraceError("RecvTargetInfoPacket >> cannot recv (1)");
		return false;
	}

	if (pGC.count > 0)
	{
		std::vector<STargetItemInfo> vecRes(static_cast<size_t>(pGC.count));

		if (!Recv(vecRes.size() * sizeof(vecRes[0]), vecRes.data()))
		{
			TraceError("RecvTargetInfoPacket >> cannot recv (2)");
			return false;
		}

		for (const auto& item : vecRes)
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
									"BINARY_AddTargetMonsterDropInfo",
									Py_BuildValue("(iii)",
									pGC.race,
									item.vnum,
									item.count));
		}
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
								"BINARY_AddTargetMonsterDropInfo",
								Py_BuildValue("(iii)",
								pGC.race,
								-1,
								-1));
	}

	const auto* pMainInst = CPythonCharacterManager::Instance().GetMainInstancePtr();
	if (pMainInst)
	{
		auto* pTargetInst = CPythonCharacterManager::Instance().GetInstancePtr(pGC.vid);
		if (!pTargetInst || pTargetInst->IsDead())
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));
			return true;
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
								"BINARY_RefreshTargetMonsterDropInfo",
								Py_BuildValue("(i)",
								pGC.race));
	}

	return true;
}
#endif

bool CPythonNetworkStream::RecvMountPacket()
{
	TPacketGCMount MountPacket;

	if (!Recv(sizeof(TPacketGCMount), &MountPacket))
	{
		Tracen("Recv Mount Packet Error");
		return false;
	}

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(MountPacket.vid);

	if (pInstance)
	{
		// Mount
		if (0 != MountPacket.mount_vid)
		{
			//			pInstance->Ride(MountPacket.pos, MountPacket.mount_vid);
		}
		// Unmount
		else
		{
			//			pInstance->Unride(MountPacket.pos, MountPacket.x, MountPacket.y);
		}
	}

	if (CPythonPlayer::Instance().IsMainCharacterIndex(MountPacket.vid))
	{
		//		CPythonPlayer::Instance().SetRidingVehicleIndex(MountPacket.mount_vid);
	}

	return true;
}
#ifdef ENABLE_SHOW_CHEST_DROP
#include "../GameLib/ItemManager.h"
bool CPythonNetworkStream::RecvChestDrop()
{
	TPacketGCChestDrop p;
	if (!Recv(sizeof(TPacketGCChestDrop), &p))
		return false;

	DWORD chestItemVnum;
	if (!Recv(sizeof(DWORD), &chestItemVnum))
		return false;

	DWORD chestItemCount;
	if (!Recv(sizeof(DWORD), &chestItemCount))
		return false;

	if (chestItemCount > 0)
	{
		CItemManager::TChestDropItemInfoVec drop_vec;
		drop_vec.clear();
		for (DWORD j = 0; j < chestItemCount; ++j)
		{
			DWORD itemVnum;
			if (!Recv(sizeof(DWORD), &itemVnum))
				return false;
			DWORD itemCount;
			if (!Recv(sizeof(DWORD), &itemCount))
				return false;
			CItemManager::SDropItemInfo drop;
			drop.dwDropVnum = itemVnum;
			drop.iCount = itemCount;
			drop_vec.emplace_back(drop);
		}
		CItemManager::Instance().LoadChestDropFromServer(chestItemVnum, drop_vec);

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenChestDrop", Py_BuildValue("(i)", chestItemVnum));
	}
	return true;

}
#endif

bool CPythonNetworkStream::RecvChangeSpeedPacket()
{
	TPacketGCChangeSpeed SpeedPacket;

	if (!Recv(sizeof(TPacketGCChangeSpeed), &SpeedPacket))
	{
		Tracen("Recv Speed Packet Error");
		return false;
	}

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(SpeedPacket.vid);

	if (!pInstance)
		return true;

	//	pInstance->SetWalkSpeed(SpeedPacket.walking_speed);
	//	pInstance->SetRunSpeed(SpeedPacket.running_speed);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Recv

bool CPythonNetworkStream::SendAttackPacket(UINT uMotAttack, DWORD dwVIDVictim)
{
	if (!__CanActMainInstance())
		return true;

#ifdef ATTACK_TIME_LOG
	static DWORD prevTime = timeGetTime();
	DWORD curTime = timeGetTime();
	TraceError("TIME: %.4f(%.4f) ATTACK_PACKET: %d TARGET: %d", curTime / 1000.0f, (curTime - prevTime) / 1000.0f, uMotAttack, dwVIDVictim);
	prevTime = curTime;
#endif

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	if (!__IsPlayerAttacking())
		return true;

	if (CheckValues())
	{
		clientCode = 46;
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)NULL);
		__asm {
			mov eax, 0x001
			jmp eax
		}
	}
#endif

	TPacketCGAttack kPacketAtk;

	kPacketAtk.header = HEADER_CG_ATTACK;
	kPacketAtk.bType = uMotAttack;
	kPacketAtk.dwVictimVID = dwVIDVictim;

	if (!SendSpecial(sizeof(kPacketAtk), &kPacketAtk))
	{
#ifdef _DEBUG
		Tracen("Send Battle Attack Packet Error");
#endif
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendSpecial(int nLen, void* pvBuf)
{
	BYTE bHeader = *(BYTE*)pvBuf;

	switch (bHeader)
	{
	case HEADER_CG_ATTACK:
	{
		TPacketCGAttack* pkPacketAtk = (TPacketCGAttack*)pvBuf;
		pkPacketAtk->bCRCMagicCubeProcPiece = GetProcessCRCMagicCubePiece();
		pkPacketAtk->bCRCMagicCubeFilePiece = GetProcessCRCMagicCubePiece();
		return Send(nLen, pvBuf);
	}
	break;
	}

	return Send(nLen, pvBuf);
}

bool CPythonNetworkStream::RecvAddFlyTargetingPacket()
{
	TPacketGCFlyTargeting kPacket;
	if (!Recv(sizeof(kPacket), &kPacket))
		return false;

	__GlobalPositionToLocalPosition(kPacket.lX, kPacket.lY);

	Tracef("VID [%d] Added to target settings\n", kPacket.dwShooterVID);

	CPythonCharacterManager& rpcm = CPythonCharacterManager::Instance();

	CInstanceBase* pShooter = rpcm.GetInstancePtr(kPacket.dwShooterVID);

	if (!pShooter)
	{
#ifndef _DEBUG
		TraceError("CPythonNetworkStream::RecvFlyTargetingPacket() - dwShooterVID[%d] NOT EXIST", kPacket.dwShooterVID);
#endif
		return true;
	}

	CInstanceBase* pTarget = rpcm.GetInstancePtr(kPacket.dwTargetVID);

	if (kPacket.dwTargetVID && pTarget)
	{
		pShooter->GetGraphicThingInstancePtr()->AddFlyTarget(pTarget->GetGraphicThingInstancePtr());
	}
	else
	{
		float h = CPythonBackground::Instance().GetHeight(kPacket.lX, kPacket.lY) + 60.0f; // TEMPORARY HEIGHT
		pShooter->GetGraphicThingInstancePtr()->AddFlyTarget(D3DXVECTOR3(kPacket.lX, kPacket.lY, h));
		//pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(kPacket.kPPosTarget.x,kPacket.kPPosTarget.y,);
	}

	return true;
}

bool CPythonNetworkStream::RecvFlyTargetingPacket()
{
	TPacketGCFlyTargeting kPacket;
	if (!Recv(sizeof(kPacket), &kPacket))
		return false;

	__GlobalPositionToLocalPosition(kPacket.lX, kPacket.lY);

	//Tracef("CPythonNetworkStream::RecvFlyTargetingPacket - VID [%d]\n",kPacket.dwShooterVID);

	CPythonCharacterManager& rpcm = CPythonCharacterManager::Instance();

	CInstanceBase* pShooter = rpcm.GetInstancePtr(kPacket.dwShooterVID);

	if (!pShooter)
	{
#ifdef _DEBUG
		TraceError("CPythonNetworkStream::RecvFlyTargetingPacket() - dwShooterVID[%d] NOT EXIST", kPacket.dwShooterVID);
#endif
		return true;
	}

	CInstanceBase* pTarget = rpcm.GetInstancePtr(kPacket.dwTargetVID);

	if (kPacket.dwTargetVID && pTarget)
	{
		pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(pTarget->GetGraphicThingInstancePtr());
	}
	else
	{
		float h = CPythonBackground::Instance().GetHeight(kPacket.lX, kPacket.lY) + 60.0f; // TEMPORARY HEIGHT
		pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(D3DXVECTOR3(kPacket.lX, kPacket.lY, h));
		//pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(kPacket.kPPosTarget.x,kPacket.kPPosTarget.y,);
	}

	return true;
}

bool CPythonNetworkStream::SendShootPacket(UINT uSkill)
{
	TPacketCGShoot kPacketShoot;
	kPacketShoot.bHeader = HEADER_CG_SHOOT;
	kPacketShoot.bType = uSkill;

	if (!Send(sizeof(kPacketShoot), &kPacketShoot))
	{
		Tracen("SendShootPacket Error");
		return false;
	}

	return SendSequence();
}
#ifdef ENABLE_MANAGER_BANK_SYSTEM
bool CPythonNetworkStream::RecvAccountBankPacket()
{
	TPacketCGBank p;
	if (!Recv(sizeof(TPacketCGBank), &p))
	{
		Tracen("RecvAccountBankPacket Error");
		return false;
	}

	CPythonManagerBank::instance()->Append(p);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_Account_Bank_Logs", Py_BuildValue("()"));
	return true;
}
#endif
bool CPythonNetworkStream::SendAddFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget)
{
	TPacketCGFlyTargeting packet;

	//CPythonCharacterManager & rpcm = CPythonCharacterManager::Instance();

	packet.bHeader = HEADER_CG_ADD_FLY_TARGETING;
	packet.dwTargetVID = dwTargetVID;
	packet.lX = kPPosTarget.x;
	packet.lY = kPPosTarget.y;

	__LocalPositionToGlobalPosition(packet.lX, packet.lY);

	if (!Send(sizeof(packet), &packet))
	{
		Tracen("Send FlyTargeting Packet Error");
		return false;
	}

	return SendSequence();
}


bool CPythonNetworkStream::SendFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget)
{
	TPacketCGFlyTargeting packet;

	//CPythonCharacterManager & rpcm = CPythonCharacterManager::Instance();

	packet.bHeader = HEADER_CG_FLY_TARGETING;
	packet.dwTargetVID = dwTargetVID;
	packet.lX = kPPosTarget.x;
	packet.lY = kPPosTarget.y;

	__LocalPositionToGlobalPosition(packet.lX, packet.lY);

	if (!Send(sizeof(packet), &packet))
	{
		Tracen("Send FlyTargeting Packet Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::RecvCreateFlyPacket()
{
	TPacketGCCreateFly kPacket;
	if (!Recv(sizeof(TPacketGCCreateFly), &kPacket))
		return false;

	CFlyingManager& rkFlyMgr = CFlyingManager::Instance();
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();

	CInstanceBase* pkStartInst = rkChrMgr.GetInstancePtr(kPacket.dwStartVID);
	CInstanceBase* pkEndInst = rkChrMgr.GetInstancePtr(kPacket.dwEndVID);
	if (!pkStartInst || !pkEndInst)
		return true;

	rkFlyMgr.CreateIndexedFly(kPacket.bType, pkStartInst->GetGraphicThingInstancePtr(), pkEndInst->GetGraphicThingInstancePtr());

	return true;
}

bool CPythonNetworkStream::SendTargetPacket(DWORD dwVID)
{
	TPacketCGTarget packet;
	packet.header = HEADER_CG_TARGET;
	packet.dwVID = dwVID;

	if (!Send(sizeof(packet), &packet))
	{
		Tracen("Send Target Packet Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendSyncPositionElementPacket(DWORD dwVictimVID, DWORD dwVictimX, DWORD dwVictimY)
{
	TPacketCGSyncPositionElement kSyncPos;
	kSyncPos.dwVID = dwVictimVID;
	kSyncPos.lX = dwVictimX;
	kSyncPos.lY = dwVictimY;

	__LocalPositionToGlobalPosition(kSyncPos.lX, kSyncPos.lY);

	if (!Send(sizeof(kSyncPos), &kSyncPos))
	{
		Tracen("CPythonNetworkStream::SendSyncPositionElementPacket - ERROR");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::RecvMessenger()
{
	TPacketGCMessenger p;
	if (!Recv(sizeof(p), &p))
		return false;

	int iSize = p.size - sizeof(p);
	char char_name[24 + 1];

	switch (p.subheader)
	{
	case MESSENGER_SUBHEADER_GC_LIST:
	{
		TPacketGCMessengerListOnline on;
		while (iSize)
		{
			if (!Recv(sizeof(TPacketGCMessengerListOffline), &on))
				return false;

			if (!Recv(on.length, char_name))
				return false;

			char_name[on.length] = 0;

			if (on.connected & MESSENGER_CONNECTED_STATE_ONLINE)
				CPythonMessenger::Instance().OnFriendLogin(char_name);
			else
				CPythonMessenger::Instance().OnFriendLogout(char_name);

			if (on.connected & MESSENGER_CONNECTED_STATE_MOBILE)
				CPythonMessenger::Instance().SetMobile(char_name, TRUE);

			iSize -= sizeof(TPacketGCMessengerListOffline);
			iSize -= on.length;
		}
		break;
	}

	case MESSENGER_SUBHEADER_GC_LOGIN:
	{
		TPacketGCMessengerLogin p;
		if (!Recv(sizeof(p), &p))
			return false;
		if (!Recv(p.length, char_name))
			return false;
		char_name[p.length] = 0;
		CPythonMessenger::Instance().OnFriendLogin(char_name);
		__RefreshTargetBoardByName(char_name);
		break;
	}

	case MESSENGER_SUBHEADER_GC_LOGOUT:
	{
		TPacketGCMessengerLogout logout;
		if (!Recv(sizeof(logout), &logout))
			return false;
		if (!Recv(logout.length, char_name))
			return false;
		char_name[logout.length] = 0;
		CPythonMessenger::Instance().OnFriendLogout(char_name);
		break;
	}

	case MESSENGER_SUBHEADER_GC_MOBILE:
	{
		BYTE byState; // 모바일 번호가 없어졌는지 플래그
		BYTE byLength;
		if (!Recv(sizeof(byState), &byState))
			return false;
		if (!Recv(sizeof(byLength), &byLength))
			return false;
		if (!Recv(byLength, char_name))
			return false;
		char_name[byLength] = 0;
		CPythonMessenger::Instance().SetMobile(char_name, byState);
		break;
	}
#ifdef ENABLE_MESSENGER_BLOCK
		case MESSENGER_SUBHEADER_GC_BLOCK_LIST:
		{
			TPacketGCMessengerBlockListOnline block_on;
			while (iSize)
			{
				if (!Recv(sizeof(TPacketGCMessengerBlockListOffline), &block_on))
					return false;

				if (!Recv(block_on.length, char_name))
					return false;

				char_name[block_on.length] = 0;

				if (block_on.connected & MESSENGER_CONNECTED_STATE_ONLINE)
					CPythonMessenger::Instance().OnBlockLogin(char_name);
				else
					CPythonMessenger::Instance().OnBlockLogout(char_name);

				iSize -= sizeof(TPacketGCMessengerBlockListOffline);
				iSize -= block_on.length;
			}
			break;
		}

		case MESSENGER_SUBHEADER_GC_BLOCK_LOGIN:
		{
			TPacketGCMessengerLogin block_p;
			if (!Recv(sizeof(block_p), &block_p))
				return false;
			if (!Recv(block_p.length, char_name))
				return false;
			char_name[block_p.length] = 0;
			CPythonMessenger::Instance().OnBlockLogin(char_name);
			__RefreshTargetBoardByName(char_name);
			break;
		}

		case MESSENGER_SUBHEADER_GC_BLOCK_LOGOUT:
		{
			TPacketGCMessengerLogout block_logout;
			if (!Recv(sizeof(block_logout), &block_logout))
				return false;
			if (!Recv(block_logout.length, char_name))
				return false;
			char_name[block_logout.length] = 0;
			CPythonMessenger::Instance().OnBlockLogout(char_name);
			break;
		}

		case MESSENGER_SUBHEADER_GC_REMOVE_BLOCK:
		{
			BYTE bLength;
			if (!Recv(sizeof(bLength), &bLength))
				return false;

			if (!Recv(bLength, char_name))
				return false;

			char_name[bLength] = 0;
			CPythonMessenger::Instance().RemoveBlock(char_name);
			__RefreshTargetBoardByName(char_name);
			break;
		}
#endif

	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Party

bool CPythonNetworkStream::SendPartyInvitePacket(DWORD dwVID)
{
	TPacketCGPartyInvite kPartyInvitePacket;
	kPartyInvitePacket.header = HEADER_CG_PARTY_INVITE;
	kPartyInvitePacket.vid = dwVID;

	if (!Send(sizeof(kPartyInvitePacket), &kPartyInvitePacket))
	{
		Tracenf("CPythonNetworkStream::SendPartyInvitePacket [%ud] - PACKET SEND ERROR", dwVID);
		return false;
	}

	Tracef(" << SendPartyInvitePacket : %d\n", dwVID);
	return SendSequence();
}

bool CPythonNetworkStream::SendPartyInviteAnswerPacket(DWORD dwLeaderVID, BYTE byAnswer)
{
	TPacketCGPartyInviteAnswer kPartyInviteAnswerPacket;
	kPartyInviteAnswerPacket.header = HEADER_CG_PARTY_INVITE_ANSWER;
	kPartyInviteAnswerPacket.leader_pid = dwLeaderVID;
	kPartyInviteAnswerPacket.accept = byAnswer;

	if (!Send(sizeof(kPartyInviteAnswerPacket), &kPartyInviteAnswerPacket))
	{
		Tracenf("CPythonNetworkStream::SendPartyInviteAnswerPacket [%ud %ud] - PACKET SEND ERROR", dwLeaderVID, byAnswer);
		return false;
	}

	Tracef(" << SendPartyInviteAnswerPacket : %d, %d\n", dwLeaderVID, byAnswer);
	return SendSequence();
}

bool CPythonNetworkStream::SendPartyRemovePacket(DWORD dwPID)
{
	if (m_strPhase != "Game")
		return false;

	TPacketCGPartyRemove kPartyInviteRemove;
	kPartyInviteRemove.header = HEADER_CG_PARTY_REMOVE;
	kPartyInviteRemove.pid = dwPID;

	if (!Send(sizeof(kPartyInviteRemove), &kPartyInviteRemove))
	{
		Tracenf("CPythonNetworkStream::SendPartyRemovePacket [%ud] - PACKET SEND ERROR", dwPID);
		return false;
	}

	Tracef(" << SendPartyRemovePacket : %d\n", dwPID);
	return SendSequence();
}

bool CPythonNetworkStream::SendPartySetStatePacket(DWORD dwVID, BYTE byState, BYTE byFlag)
{
	TPacketCGPartySetState kPartySetState;
	kPartySetState.byHeader = HEADER_CG_PARTY_SET_STATE;
	kPartySetState.dwVID = dwVID;
	kPartySetState.byState = byState;
	kPartySetState.byFlag = byFlag;

	if (!Send(sizeof(kPartySetState), &kPartySetState))
	{
		Tracenf("CPythonNetworkStream::SendPartySetStatePacket(%ud, %ud) - PACKET SEND ERROR", dwVID, byState);
		return false;
	}

	Tracef(" << SendPartySetStatePacket : %d, %d, %d\n", dwVID, byState, byFlag);
	return SendSequence();
}

bool CPythonNetworkStream::SendPartyUseSkillPacket(BYTE bySkillIndex, DWORD dwVID)
{
	TPacketCGPartyUseSkill kPartyUseSkill;
	kPartyUseSkill.byHeader = HEADER_CG_PARTY_USE_SKILL;
	kPartyUseSkill.bySkillIndex = bySkillIndex;
	kPartyUseSkill.dwTargetVID = dwVID;

	if (!Send(sizeof(kPartyUseSkill), &kPartyUseSkill))
	{
		Tracenf("CPythonNetworkStream::SendPartyUseSkillPacket(%ud, %ud) - PACKET SEND ERROR", bySkillIndex, dwVID);
		return false;
	}

	Tracef(" << SendPartyUseSkillPacket : %d, %d\n", bySkillIndex, dwVID);
	return SendSequence();
}

bool CPythonNetworkStream::SendPartyParameterPacket(BYTE byDistributeMode)
{
	TPacketCGPartyParameter kPartyParameter;
	kPartyParameter.bHeader = HEADER_CG_PARTY_PARAMETER;
	kPartyParameter.bDistributeMode = byDistributeMode;

	if (!Send(sizeof(kPartyParameter), &kPartyParameter))
	{
		Tracenf("CPythonNetworkStream::SendPartyParameterPacket(%d) - PACKET SEND ERROR", byDistributeMode);
		return false;
	}

	Tracef(" << SendPartyParameterPacket : %d\n", byDistributeMode);
	return SendSequence();
}

#ifdef ENABLE_SWITCHBOT
bool CPythonNetworkStream::RecvSwitchbotPacket()
{
	TPacketGCSwitchbot pack;
	if (!Recv(sizeof(pack), &pack))
	{
		return false;
	}

	size_t packet_size = int(pack.size) - sizeof(TPacketGCSwitchbot);
	if (pack.subheader == SUBHEADER_GC_SWITCHBOT_UPDATE)
	{
		if (packet_size != sizeof(CPythonSwitchbot::TSwitchbotTable))
		{
			return false;
		}

		CPythonSwitchbot::TSwitchbotTable table;
		if (!Recv(sizeof(table), &table))
		{
			return false;
		}

		CPythonSwitchbot::Instance().Update(table);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSwitchbotWindow", Py_BuildValue("()"));
	}
	else if (pack.subheader == SUBHEADER_GC_SWITCHBOT_UPDATE_ITEM)
	{
		if (packet_size != sizeof(TSwitchbotUpdateItem))
		{
			return false;
		}

		TSwitchbotUpdateItem update;
		if (!Recv(sizeof(update), &update))
		{
			return false;
		}

		TItemPos pos(SWITCHBOT, update.slot);
#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
		TItemPos posInven(INVENTORY, update.CellInven);
#endif

		IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
		rkPlayer.SetItemCount(pos, update.count);

		for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
		{
			rkPlayer.SetItemMetinSocket(pos, i, update.alSockets[i]);

		}

		for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
		{
			rkPlayer.SetItemAttribute(pos, j, update.aAttr[j].bType, update.aAttr[j].sValue);
#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
			rkPlayer.SetItemAttribute(posInven, j, update.aAttr[j].bType, update.aAttr[j].sValue);
#endif
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSwitchbotItem", Py_BuildValue("(i)", update.slot));
		return true;
	}
	else if (pack.subheader == SUBHEADER_GC_SWITCHBOT_SEND_ATTRIBUTE_INFORMATION)
	{
		CPythonSwitchbot::Instance().ClearAttributeMap();

		size_t table_size = sizeof(CPythonSwitchbot::TSwitchbottAttributeTable);
		while (packet_size >= table_size)
		{
			const int test = sizeof(CPythonSwitchbot::TSwitchbottAttributeTable);

			CPythonSwitchbot::TSwitchbottAttributeTable table;
			if (!Recv(table_size, &table))
			{
				return false;
			}

			CPythonSwitchbot::Instance().AddAttributeToMap(table);
			packet_size -= table_size;
		}
	}

	return true;
}
bool CPythonNetworkStream::SendSwitchbotStartPacket(BYTE slot, std::vector<CPythonSwitchbot::TSwitchbotAttributeAlternativeTable> alternatives)
{
	TPacketCGSwitchbot pack;
	pack.header = HEADER_CG_SWITCHBOT;
	pack.subheader = SUBHEADER_CG_SWITCHBOT_START;
	pack.size = sizeof(TPacketCGSwitchbot) + sizeof(CPythonSwitchbot::TSwitchbotAttributeAlternativeTable) * SWITCHBOT_ALTERNATIVE_COUNT;
	pack.slot = slot;

	if (!Send(sizeof(pack), &pack))
	{
		return false;
	}

	for (const auto& it : alternatives)
	{
		if (!Send(sizeof(it), &it))
		{
			return false;
		}
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendSwitchbotStopPacket(BYTE slot)
{
	TPacketCGSwitchbot pack;
	pack.header = HEADER_CG_SWITCHBOT;
	pack.subheader = SUBHEADER_CG_SWITCHBOT_STOP;
	pack.size = sizeof(TPacketCGSwitchbot);
	pack.slot = slot;

	if (!Send(sizeof(pack), &pack))
	{
		return false;
	}

	return SendSequence();
}
#endif

bool CPythonNetworkStream::RecvPartyInvite()
{
	TPacketGCPartyInvite kPartyInvitePacket;
	if (!Recv(sizeof(kPartyInvitePacket), &kPartyInvitePacket))
		return false;

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(kPartyInvitePacket.leader_pid);
	if (!pInstance)
	{
		TraceError(" CPythonNetworkStream::RecvPartyInvite - Failed to find leader instance [%d]\n", kPartyInvitePacket.leader_pid);
		return true;
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RecvPartyInviteQuestion", Py_BuildValue("(is)", kPartyInvitePacket.leader_pid, pInstance->GetNameString()));
	Tracef(" >> RecvPartyInvite : %d, %s\n", kPartyInvitePacket.leader_pid, pInstance->GetNameString());

	return true;
}

bool CPythonNetworkStream::RecvPartyAdd()
{
	TPacketGCPartyAdd kPartyAddPacket;
	if (!Recv(sizeof(kPartyAddPacket), &kPartyAddPacket))
		return false;

	CPythonPlayer::Instance().AppendPartyMember(kPartyAddPacket.pid, kPartyAddPacket.name);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AddPartyMember", Py_BuildValue("(is)", kPartyAddPacket.pid, kPartyAddPacket.name));
	Tracef(" >> RecvPartyAdd : %d, %s\n", kPartyAddPacket.pid, kPartyAddPacket.name);

	return true;
}

bool CPythonNetworkStream::RecvPartyUpdate()
{
	TPacketGCPartyUpdate kPartyUpdatePacket;
	if (!Recv(sizeof(kPartyUpdatePacket), &kPartyUpdatePacket))
		return false;

	CPythonPlayer::TPartyMemberInfo* pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(kPartyUpdatePacket.pid, &pPartyMemberInfo))
		return true;

	BYTE byOldState = pPartyMemberInfo->byState;

	CPythonPlayer::Instance().UpdatePartyMemberInfo(kPartyUpdatePacket.pid, kPartyUpdatePacket.state, kPartyUpdatePacket.percent_hp);
	for (int i = 0; i < PARTY_AFFECT_SLOT_MAX_NUM; ++i)
	{
		CPythonPlayer::Instance().UpdatePartyMemberAffect(kPartyUpdatePacket.pid, i, kPartyUpdatePacket.affects[i]);
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "UpdatePartyMemberInfo", Py_BuildValue("(i)", kPartyUpdatePacket.pid));

	// 만약 리더가 바뀌었다면, TargetBoard 의 버튼을 업데이트 한다.
	DWORD dwVID;
	if (CPythonPlayer::Instance().PartyMemberPIDToVID(kPartyUpdatePacket.pid, &dwVID))
		if (byOldState != kPartyUpdatePacket.state)
		{
			__RefreshTargetBoardByVID(dwVID);
		}

	// 	Tracef(" >> RecvPartyUpdate : %d, %d, %d\n", kPartyUpdatePacket.pid, kPartyUpdatePacket.state, kPartyUpdatePacket.percent_hp);

	return true;
}

bool CPythonNetworkStream::RecvPartyRemove()
{
	TPacketGCPartyRemove kPartyRemovePacket;
	if (!Recv(sizeof(kPartyRemovePacket), &kPartyRemovePacket))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RemovePartyMember", Py_BuildValue("(i)", kPartyRemovePacket.pid));
	Tracef(" >> RecvPartyRemove : %d\n", kPartyRemovePacket.pid);

	return true;
}

bool CPythonNetworkStream::RecvPartyLink()
{
	TPacketGCPartyLink kPartyLinkPacket;
	if (!Recv(sizeof(kPartyLinkPacket), &kPartyLinkPacket))
		return false;

	CPythonPlayer::Instance().LinkPartyMember(kPartyLinkPacket.pid, kPartyLinkPacket.vid);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "LinkPartyMember", Py_BuildValue("(ii)", kPartyLinkPacket.pid, kPartyLinkPacket.vid));
	Tracef(" >> RecvPartyLink : %d, %d\n", kPartyLinkPacket.pid, kPartyLinkPacket.vid);

	return true;
}

bool CPythonNetworkStream::RecvPartyUnlink()
{
	TPacketGCPartyUnlink kPartyUnlinkPacket;
	if (!Recv(sizeof(kPartyUnlinkPacket), &kPartyUnlinkPacket))
		return false;

	CPythonPlayer::Instance().UnlinkPartyMember(kPartyUnlinkPacket.pid);

	if (CPythonPlayer::Instance().IsMainCharacterIndex(kPartyUnlinkPacket.vid))
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "UnlinkAllPartyMember", Py_BuildValue("()"));
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "UnlinkPartyMember", Py_BuildValue("(i)", kPartyUnlinkPacket.pid));
	}

	Tracef(" >> RecvPartyUnlink : %d, %d\n", kPartyUnlinkPacket.pid, kPartyUnlinkPacket.vid);

	return true;
}

bool CPythonNetworkStream::RecvPartyParameter()
{
	TPacketGCPartyParameter kPartyParameterPacket;
	if (!Recv(sizeof(kPartyParameterPacket), &kPartyParameterPacket))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ChangePartyParameter", Py_BuildValue("(i)", kPartyParameterPacket.bDistributeMode));
	Tracef(" >> RecvPartyParameter : %d\n", kPartyParameterPacket.bDistributeMode);

	return true;
}

// Party
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Guild

bool CPythonNetworkStream::SendGuildAddMemberPacket(DWORD dwVID)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_ADD_MEMBER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwVID), &dwVID))
		return false;

#ifdef _DEBUG
	Tracef(" SendGuildAddMemberPacket\n", dwVID);
#endif
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildRemoveMemberPacket(DWORD dwPID)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_REMOVE_MEMBER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwPID), &dwPID))
		return false;

	Tracef(" SendGuildRemoveMemberPacket %d\n", dwPID);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildChangeGradeNamePacket(BYTE byGradeNumber, const char* c_szName)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(byGradeNumber), &byGradeNumber))
		return false;

	char szName[GUILD_GRADE_NAME_MAX_LEN + 1];
	strncpy(szName, c_szName, GUILD_GRADE_NAME_MAX_LEN);
	szName[GUILD_GRADE_NAME_MAX_LEN] = '\0';

	if (!Send(sizeof(szName), &szName))
		return false;

	Tracef(" SendGuildChangeGradeNamePacket %d, %s\n", byGradeNumber, c_szName);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildChangeGradeAuthorityPacket(BYTE byGradeNumber, BYTE byAuthority)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(byGradeNumber), &byGradeNumber))
		return false;
	if (!Send(sizeof(byAuthority), &byAuthority))
		return false;

	Tracef(" SendGuildChangeGradeAuthorityPacket %d, %d\n", byGradeNumber, byAuthority);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildOfferPacket(DWORD dwExperience)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_OFFER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwExperience), &dwExperience))
		return false;

#ifdef _DEBUG
	Tracef(" SendGuildOfferPacket %d\n", dwExperience);
#endif

	return SendSequence();
}

bool CPythonNetworkStream::SendGuildPostCommentPacket(const char* c_szMessage)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_POST_COMMENT;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	BYTE bySize = BYTE(strlen(c_szMessage)) + 1;
	if (!Send(sizeof(bySize), &bySize))
		return false;
	if (!Send(bySize, c_szMessage))
		return false;

#ifdef _DEBUG
	Tracef(" SendGuildPostCommentPacket %d, %s\n", bySize, c_szMessage);
#endif
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildDeleteCommentPacket(DWORD dwIndex)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_DELETE_COMMENT;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwIndex), &dwIndex))
		return false;

	Tracef(" SendGuildDeleteCommentPacket %d\n", dwIndex);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildRefreshCommentsPacket(DWORD dwHighestIndex)
{
	static DWORD s_LastTime = timeGetTime() - 1001;

	if (timeGetTime() - s_LastTime < 1000)
		return true;
	s_LastTime = timeGetTime();

	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_REFRESH_COMMENT;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

#ifdef _DEBUG
	Tracef(" SendGuildRefreshCommentPacket %d\n", dwHighestIndex);
#endif
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildChangeMemberGradePacket(DWORD dwPID, BYTE byGrade)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwPID), &dwPID))
		return false;
	if (!Send(sizeof(byGrade), &byGrade))
		return false;

	Tracef(" SendGuildChangeMemberGradePacket %d, %d\n", dwPID, byGrade);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildUseSkillPacket(DWORD dwSkillID, DWORD dwTargetVID)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_USE_SKILL;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwSkillID), &dwSkillID))
		return false;
	if (!Send(sizeof(dwTargetVID), &dwTargetVID))
		return false;

	Tracef(" SendGuildUseSkillPacket %d, %d\n", dwSkillID, dwTargetVID);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildChangeMemberGeneralPacket(DWORD dwPID, BYTE byFlag)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwPID), &dwPID))
		return false;
	if (!Send(sizeof(byFlag), &byFlag))
		return false;

	Tracef(" SendGuildChangeMemberGeneralFlagPacket %d, %d\n", dwPID, byFlag);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildInviteAnswerPacket(DWORD dwGuildID, BYTE byAnswer)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwGuildID), &dwGuildID))
		return false;
	if (!Send(sizeof(byAnswer), &byAnswer))
		return false;

	Tracef(" SendGuildInviteAnswerPacket %d, %d\n", dwGuildID, byAnswer);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildChargeGSPPacket(DWORD dwMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHARGE_GSP;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwMoney), &dwMoney))
		return false;

	Tracef(" SendGuildChargeGSPPacket %d\n", dwMoney);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildDepositMoneyPacket(DWORD dwMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_DEPOSIT_MONEY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwMoney), &dwMoney))
		return false;

	Tracef(" SendGuildDepositMoneyPacket %d\n", dwMoney);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildWithdrawMoneyPacket(DWORD dwMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_WITHDRAW_MONEY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwMoney), &dwMoney))
		return false;

	Tracef(" SendGuildWithdrawMoneyPacket %d\n", dwMoney);
	return SendSequence();
}

bool CPythonNetworkStream::RecvGuild()
{
	TPacketGCGuild GuildPacket;
	if (!Recv(sizeof(GuildPacket), &GuildPacket))
		return false;

	switch (GuildPacket.subheader)
	{
	case GUILD_SUBHEADER_GC_LOGIN:
	{
		DWORD dwPID;
		if (!Recv(sizeof(DWORD), &dwPID))
			return false;

		// Messenger
		CPythonGuild::TGuildMemberData* pGuildMemberData;
		if (CPythonGuild::Instance().GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
			if (0 != pGuildMemberData->strName.compare(CPythonPlayer::Instance().GetName()))
				CPythonMessenger::Instance().LoginGuildMember(pGuildMemberData->strName.c_str());

		//Tracef(" <Login> %d\n", dwPID);
		break;
	}
	case GUILD_SUBHEADER_GC_LOGOUT:
	{
		DWORD dwPID;
		if (!Recv(sizeof(DWORD), &dwPID))
			return false;

		// Messenger
		CPythonGuild::TGuildMemberData* pGuildMemberData;
		if (CPythonGuild::Instance().GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
			if (0 != pGuildMemberData->strName.compare(CPythonPlayer::Instance().GetName()))
				CPythonMessenger::Instance().LogoutGuildMember(pGuildMemberData->strName.c_str());

		//Tracef(" <Logout> %d\n", dwPID);
		break;
	}
	case GUILD_SUBHEADER_GC_REMOVE:
	{
		DWORD dwPID;
		if (!Recv(sizeof(dwPID), &dwPID))
			return false;

		// Main Player 일 경우 DeleteGuild
		if (CPythonGuild::Instance().IsMainPlayer(dwPID))
		{
			CPythonGuild::Instance().Destroy();
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "DeleteGuild", Py_BuildValue("()"));
			CPythonMessenger::Instance().RemoveAllGuildMember();
			__SetGuildID(0);
			__RefreshMessengerWindow();
			__RefreshTargetBoard();
			__RefreshCharacterWindow();
		}
		else
		{
			// Get Member Name
			std::string strMemberName = "";
			CPythonGuild::TGuildMemberData* pData;
			if (CPythonGuild::Instance().GetMemberDataPtrByPID(dwPID, &pData))
			{
				strMemberName = pData->strName;
				CPythonMessenger::Instance().RemoveGuildMember(pData->strName.c_str());
			}

			CPythonGuild::Instance().RemoveMember(dwPID);

			// Refresh
			__RefreshTargetBoardByName(strMemberName.c_str());
			__RefreshGuildWindowMemberPage();
		}

		Tracef(" <Remove> %d\n", dwPID);
		break;
	}
	case GUILD_SUBHEADER_GC_LIST:
	{
		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);

		for (; iPacketSize > 0;)
		{
			TPacketGCGuildSubMember memberPacket;
			if (!Recv(sizeof(memberPacket), &memberPacket))
				return false;

			char szName[CHARACTER_NAME_MAX_LEN + 1] = "";
			if (memberPacket.byNameFlag)
			{
				if (!Recv(sizeof(szName), &szName))
					return false;

				iPacketSize -= CHARACTER_NAME_MAX_LEN + 1;
			}
			else
			{
				CPythonGuild::TGuildMemberData* pMemberData;
				if (CPythonGuild::Instance().GetMemberDataPtrByPID(memberPacket.pid, &pMemberData))
				{
					strncpy(szName, pMemberData->strName.c_str(), CHARACTER_NAME_MAX_LEN);
				}
			}

			//Tracef(" <List> %d : %s, %d (%d, %d, %d)\n", memberPacket.pid, szName, memberPacket.byGrade, memberPacket.byJob, memberPacket.byLevel, memberPacket.dwOffer);

			CPythonGuild::SGuildMemberData GuildMemberData;
			GuildMemberData.dwPID = memberPacket.pid;
			GuildMemberData.byGrade = memberPacket.byGrade;
			GuildMemberData.strName = szName;
			GuildMemberData.byJob = memberPacket.byJob;
			GuildMemberData.byLevel = memberPacket.byLevel;
			GuildMemberData.dwOffer = memberPacket.dwOffer;
			GuildMemberData.byGeneralFlag = memberPacket.byIsGeneral;
			CPythonGuild::Instance().RegisterMember(GuildMemberData);

			// Messenger
			if (strcmp(szName, CPythonPlayer::Instance().GetName()))
				CPythonMessenger::Instance().AppendGuildMember(szName);

			__RefreshTargetBoardByName(szName);

			iPacketSize -= sizeof(memberPacket);
		}

		__RefreshGuildWindowInfoPage();
		__RefreshGuildWindowMemberPage();
		__RefreshMessengerWindow();
		__RefreshCharacterWindow();
		break;
	}
	case GUILD_SUBHEADER_GC_GRADE:
	{
		BYTE byCount;
		if (!Recv(sizeof(byCount), &byCount))
			return false;

		for (BYTE i = 0; i < byCount; ++i)
		{
			BYTE byIndex;
			if (!Recv(sizeof(byCount), &byIndex))
				return false;
			TPacketGCGuildSubGrade GradePacket;
			if (!Recv(sizeof(GradePacket), &GradePacket))
				return false;

			auto data = CPythonGuild::SGuildGradeData(GradePacket.auth_flag, GradePacket.grade_name);
			CPythonGuild::Instance().SetGradeData(byIndex, data);
			//Tracef(" <Grade> [%d/%d] : %s, %d\n", byIndex, byCount, GradePacket.grade_name, GradePacket.auth_flag);
		}
		__RefreshGuildWindowGradePage();
		__RefreshGuildWindowMemberPageGradeComboBox();
		break;
	}
	case GUILD_SUBHEADER_GC_GRADE_NAME:
	{
		BYTE byGradeNumber;
		if (!Recv(sizeof(byGradeNumber), &byGradeNumber))
			return false;

		char szGradeName[GUILD_GRADE_NAME_MAX_LEN + 1] = "";
		if (!Recv(sizeof(szGradeName), &szGradeName))
			return false;

		CPythonGuild::Instance().SetGradeName(byGradeNumber, szGradeName);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildGrade", Py_BuildValue("()"));

		Tracef(" <Change Grade Name> %d, %s\n", byGradeNumber, szGradeName);
		__RefreshGuildWindowGradePage();
		__RefreshGuildWindowMemberPageGradeComboBox();
		break;
	}
	case GUILD_SUBHEADER_GC_GRADE_AUTH:
	{
		BYTE byGradeNumber;
		if (!Recv(sizeof(byGradeNumber), &byGradeNumber))
			return false;
		BYTE byAuthorityFlag;
		if (!Recv(sizeof(byAuthorityFlag), &byAuthorityFlag))
			return false;

		CPythonGuild::Instance().SetGradeAuthority(byGradeNumber, byAuthorityFlag);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildGrade", Py_BuildValue("()"));

		Tracef(" <Change Grade Authority> %d, %d\n", byGradeNumber, byAuthorityFlag);
		__RefreshGuildWindowGradePage();
		break;
	}
	case GUILD_SUBHEADER_GC_INFO:
	{
		TPacketGCGuildInfo GuildInfo;
		if (!Recv(sizeof(GuildInfo), &GuildInfo))
			return false;

		CPythonGuild::Instance().EnableGuild();
		CPythonGuild::TGuildInfo& rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
		strncpy(rGuildInfo.szGuildName, GuildInfo.name, GUILD_NAME_MAX_LEN);
		rGuildInfo.szGuildName[GUILD_NAME_MAX_LEN] = '\0';

		rGuildInfo.dwGuildID = GuildInfo.guild_id;
		rGuildInfo.dwMasterPID = GuildInfo.master_pid;
		rGuildInfo.dwGuildLevel = GuildInfo.level;
		rGuildInfo.dwCurrentExperience = GuildInfo.exp;
		rGuildInfo.dwCurrentMemberCount = GuildInfo.member_count;
		rGuildInfo.dwMaxMemberCount = GuildInfo.max_member_count;
		rGuildInfo.dwGuildMoney = GuildInfo.gold;
		rGuildInfo.bHasLand = GuildInfo.hasLand;

		//Tracef(" <Info> %s, %d, %d : %d\n", GuildInfo.name, GuildInfo.master_pid, GuildInfo.level, rGuildInfo.bHasLand);
		__RefreshGuildWindowInfoPage();
		break;
	}
	case GUILD_SUBHEADER_GC_COMMENTS:
	{
		BYTE byCount;
		if (!Recv(sizeof(byCount), &byCount))
			return false;

		CPythonGuild::Instance().ClearComment();
		//Tracef(" >>> Comments Count : %d\n", byCount);

		for (BYTE i = 0; i < byCount; ++i)
		{
			DWORD dwCommentID;
			if (!Recv(sizeof(dwCommentID), &dwCommentID))
				return false;

			char szName[CHARACTER_NAME_MAX_LEN + 1] = "";
			if (!Recv(sizeof(szName), &szName))
				return false;

			char szComment[GULID_COMMENT_MAX_LEN + 1] = "";
			if (!Recv(sizeof(szComment), &szComment))
				return false;

			//Tracef(" [Comment-%d] : %s, %s\n", dwCommentID, szName, szComment);
			CPythonGuild::Instance().RegisterComment(dwCommentID, szName, szComment);
		}

		__RefreshGuildWindowBoardPage();
		break;
	}
	case GUILD_SUBHEADER_GC_CHANGE_EXP:
	{
		BYTE byLevel;
		if (!Recv(sizeof(byLevel), &byLevel))
			return false;
		DWORD dwEXP;
		if (!Recv(sizeof(dwEXP), &dwEXP))
			return false;
		CPythonGuild::Instance().SetGuildEXP(byLevel, dwEXP);
		Tracef(" <ChangeEXP> %d, %d\n", byLevel, dwEXP);
		__RefreshGuildWindowInfoPage();
		break;
	}
	case GUILD_SUBHEADER_GC_CHANGE_MEMBER_GRADE:
	{
		DWORD dwPID;
		if (!Recv(sizeof(dwPID), &dwPID))
			return false;
		BYTE byGrade;
		if (!Recv(sizeof(byGrade), &byGrade))
			return false;
		CPythonGuild::Instance().ChangeGuildMemberGrade(dwPID, byGrade);
		Tracef(" <ChangeMemberGrade> %d, %d\n", dwPID, byGrade);
		__RefreshGuildWindowMemberPage();
		break;
	}
	case GUILD_SUBHEADER_GC_SKILL_INFO:
	{
		CPythonGuild::TGuildSkillData& rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
		if (!Recv(sizeof(rSkillData.bySkillPoint), &rSkillData.bySkillPoint))
			return false;
		if (!Recv(sizeof(rSkillData.bySkillLevel), rSkillData.bySkillLevel))
			return false;
		if (!Recv(sizeof(rSkillData.wGuildPoint), &rSkillData.wGuildPoint))
			return false;
		if (!Recv(sizeof(rSkillData.wMaxGuildPoint), &rSkillData.wMaxGuildPoint))
			return false;

		Tracef(" <SkillInfo> %d / %d, %d\n", rSkillData.bySkillPoint, rSkillData.wGuildPoint, rSkillData.wMaxGuildPoint);
		__RefreshGuildWindowSkillPage();
		break;
	}
	case GUILD_SUBHEADER_GC_CHANGE_MEMBER_GENERAL:
	{
		DWORD dwPID;
		if (!Recv(sizeof(dwPID), &dwPID))
			return false;
		BYTE byFlag;
		if (!Recv(sizeof(byFlag), &byFlag))
			return false;

		CPythonGuild::Instance().ChangeGuildMemberGeneralFlag(dwPID, byFlag);
		Tracef(" <ChangeMemberGeneralFlag> %d, %d\n", dwPID, byFlag);
		__RefreshGuildWindowMemberPage();
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_INVITE:
	{
		DWORD dwGuildID;
		if (!Recv(sizeof(dwGuildID), &dwGuildID))
			return false;
		char szGuildName[GUILD_NAME_MAX_LEN + 1];
		if (!Recv(GUILD_NAME_MAX_LEN, &szGuildName))
			return false;

		szGuildName[GUILD_NAME_MAX_LEN] = 0;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RecvGuildInviteQuestion", Py_BuildValue("(is)", dwGuildID, szGuildName));
		Tracef(" <Guild Invite> %d, %s\n", dwGuildID, szGuildName);
		break;
	}
	case GUILD_SUBHEADER_GC_WAR:
	{
		TPacketGCGuildWar kGuildWar;
		if (!Recv(sizeof(kGuildWar), &kGuildWar))
			return false;

		switch (kGuildWar.bWarState)
		{
		case GUILD_WAR_SEND_DECLARE:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_SEND_DECLARE\n");
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnSendDeclare",
				Py_BuildValue("(i)", kGuildWar.dwGuildOpp)
			);
			break;
		case GUILD_WAR_RECV_DECLARE:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_RECV_DECLARE\n");
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnRecvDeclare",
				Py_BuildValue("(ii)", kGuildWar.dwGuildOpp, kGuildWar.bType)
			);
			break;
		case GUILD_WAR_ON_WAR:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_ON_WAR : %d, %d\n", kGuildWar.dwGuildSelf, kGuildWar.dwGuildOpp);
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnStart",
				Py_BuildValue("(ii)", kGuildWar.dwGuildSelf, kGuildWar.dwGuildOpp)
			);
			CPythonGuild::Instance().StartGuildWar(kGuildWar.dwGuildOpp);
			break;
		case GUILD_WAR_END:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_END\n");
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnEnd",
				Py_BuildValue("(ii)", kGuildWar.dwGuildSelf, kGuildWar.dwGuildOpp)
			);
			CPythonGuild::Instance().EndGuildWar(kGuildWar.dwGuildOpp);
			break;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_NAME:
	{
		DWORD dwID;
		char szGuildName[GUILD_NAME_MAX_LEN + 1];

		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);

		int nItemSize = sizeof(dwID) + GUILD_NAME_MAX_LEN;

		assert(iPacketSize % nItemSize == 0 && "GUILD_SUBHEADER_GC_GUILD_NAME");

		for (; iPacketSize > 0;)
		{
			if (!Recv(sizeof(dwID), &dwID))
				return false;

			if (!Recv(GUILD_NAME_MAX_LEN, &szGuildName))
				return false;

			szGuildName[GUILD_NAME_MAX_LEN] = 0;

			//Tracef(" >> GulidName [%d : %s]\n", dwID, szGuildName);
			CPythonGuild::Instance().RegisterGuildName(dwID, szGuildName);
			iPacketSize -= nItemSize;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_WAR_LIST:
	{
		DWORD dwSrcGuildID;
		DWORD dwDstGuildID;

		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);
		int nItemSize = sizeof(dwSrcGuildID) + sizeof(dwDstGuildID);

		assert(iPacketSize % nItemSize == 0 && "GUILD_SUBHEADER_GC_GUILD_WAR_LIST");

		for (; iPacketSize > 0;)
		{
			if (!Recv(sizeof(dwSrcGuildID), &dwSrcGuildID))
				return false;

			if (!Recv(sizeof(dwDstGuildID), &dwDstGuildID))
				return false;

			Tracef(" >> GulidWarList [%d vs %d]\n", dwSrcGuildID, dwDstGuildID);
			CInstanceBase::InsertGVGKey(dwSrcGuildID, dwDstGuildID);
			CPythonCharacterManager::Instance().ChangeGVG(dwSrcGuildID, dwDstGuildID);
			iPacketSize -= nItemSize;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST:
	{
		DWORD dwSrcGuildID;
		DWORD dwDstGuildID;

		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);
		int nItemSize = sizeof(dwSrcGuildID) + sizeof(dwDstGuildID);

		assert(iPacketSize % nItemSize == 0 && "GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST");

		for (; iPacketSize > 0;)
		{

			if (!Recv(sizeof(dwSrcGuildID), &dwSrcGuildID))
				return false;

			if (!Recv(sizeof(dwDstGuildID), &dwDstGuildID))
				return false;

			Tracef(" >> GulidWarEndList [%d vs %d]\n", dwSrcGuildID, dwDstGuildID);
			CInstanceBase::RemoveGVGKey(dwSrcGuildID, dwDstGuildID);
			CPythonCharacterManager::Instance().ChangeGVG(dwSrcGuildID, dwDstGuildID);
			iPacketSize -= nItemSize;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_WAR_POINT:
	{
		TPacketGuildWarPoint GuildWarPoint;
		if (!Recv(sizeof(GuildWarPoint), &GuildWarPoint))
			return false;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
			"BINARY_GuildWar_OnRecvPoint",
			Py_BuildValue("(iii)", GuildWarPoint.dwGainGuildID, GuildWarPoint.dwOpponentGuildID, GuildWarPoint.lPoint)
		);
		break;
	}
	case GUILD_SUBHEADER_GC_MONEY_CHANGE:
	{
		DWORD dwMoney;
		if (!Recv(sizeof(dwMoney), &dwMoney))
			return false;

		CPythonGuild::Instance().SetGuildMoney(dwMoney);

		__RefreshGuildWindowInfoPage();
		Tracef(" >> Guild Money Change : %d\n", dwMoney);
		break;
	}
	}

	return true;
}

// Guild
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
// Fishing

bool CPythonNetworkStream::SendFishingPacket(int iRotation)
{
	TPacketCGFishing Packet;
	Packet.header = HEADER_CG_FISHING;
	Packet.dir = iRotation / 5;
	Packet.bFishingGame = true;
	Packet.fishing_key = 0;

	if (!Send(sizeof(Packet), &Packet))
		return false;

	return SendSequence();
}
#if defined(ENABLE_FISHING_GAME)
bool CPythonNetworkStream::SendFishingGamePacket(BYTE bSubHeader, BYTE bGoals, uint32_t fishing_key)
{
	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	if (!rkPlayer.GetFishingGameWindowOpen())
		return false;

	TPacketCGFishingGame Packet;
	Packet.bHeader = HEADER_CG_FISHING_GAME;
	Packet.bSubHeader = bSubHeader;
	Packet.bGoals = bGoals;
	Packet.fishing_key = fishing_key;

	if (!Send(sizeof(Packet), &Packet))
		return false;

	return SendSequence();
}
#endif

bool CPythonNetworkStream::SendGiveItemPacket(DWORD dwTargetVID, TItemPos ItemPos, int iItemCount)
{
	TPacketCGGiveItem GiveItemPacket;
	GiveItemPacket.byHeader = HEADER_CG_GIVE_ITEM;
	GiveItemPacket.dwTargetVID = dwTargetVID;
	GiveItemPacket.ItemPos = ItemPos;
	GiveItemPacket.byItemCount = iItemCount;

	if (!Send(sizeof(GiveItemPacket), &GiveItemPacket))
		return false;

	return SendSequence();
}
#if defined(WJ_COMBAT_ZONE)
bool CPythonNetworkStream::__RecvCombatZoneRankingPacket()
{
	TPacketGCCombatZoneRankingData p;
	if (!Recv(sizeof(TPacketGCCombatZoneRankingData), &p))
	{
		Tracen("Recv PacketGCCombatZoneRankingData Error");
		return false;
	}

	CPythonCombatZone::instance()->Initialize(p);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CombatZone_Manager", Py_BuildValue("(s)", "RegisterRank"));
	return true;
}

bool CPythonNetworkStream::SendCombatZoneRequestActionPacket(int iAction, int iValue)
{
	TPacketCGCombatZoneRequestAction p;
	p.header = HEADER_CG_COMBAT_ZONE_REQUEST_ACTION;
	p.action = iAction;
	p.value = iValue;

	if (!Send(sizeof(p), &p))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::__RecvCombatZonePacket()
{
	TPacketGCSendCombatZone p;

	if (!Recv(sizeof(TPacketGCSendCombatZone), &p))
	{
		Tracen("Recv CombatZone Packet Error");
		return false;
	}

	switch (p.sub_header)
	{
		/*
		case COMBAT_ZONE_SUB_HEADER_ADD_LEAVING_TARGET:
			CPythonBackground::Instance().CreateCombatZoneTargetsLeaving(p.m_pInfoData[0], p.m_pInfoData[1]);
			break;

		case COMBAT_ZONE_SUB_HEADER_REMOVE_LEAVING_TARGET:
			CPythonBackground::Instance().DeleteCombatZoneTargetsLeaving(0);
			break;
		*/

	case COMBAT_ZONE_SUB_HEADER_BONUS: {
		DWORD dwBonusType = p.m_pInfoData[0];
		DWORD dwBonusValue = p.m_pInfoData[1];
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CombatZone_Manager", Py_BuildValue("(sii)", "Bonus", dwBonusType, dwBonusValue));
	}
									 break;

	case COMBAT_ZONE_SUB_HEADER_TIME:
	{
		DWORD dwTime = p.m_pInfoData[0];
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CombatZone_Manager", Py_BuildValue("(si)", "Time", dwTime));

	}
	break;

	case COMBAT_ZONE_SUB_HEADER_BONUS_CLEAR:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CombatZone_Manager", Py_BuildValue("(s)", "BonusClear"));
		break;

	case COMBAT_ZONE_SUB_HEADER_MP:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CombatZone_Manager", Py_BuildValue("(s)", "StartMp"));
		break;

	case COMBAT_ZONE_SUB_HEADER_FLASH_ON_MINIMAP:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CombatZone_Manager", Py_BuildValue("(s)", "StartFlashing"));
		break;


	case COMBAT_ZONE_SUB_HEADER_OPEN_RANKING:
	{
		DWORD dwPoints = p.m_pInfoData[0];
		DWORD dwTimeRemaining = p.m_pInfoData[1];
		DWORD dwCurMobsKills = p.m_pInfoData[2];
		DWORD dwMaxMobsKills = p.m_pInfoData[3];

		CPythonCombatZone::instance()->SendDataDays(p.m_pDataDays, p.m_pDataDaysCheck, p.isRunning);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CombatZone_Manager", Py_BuildValue("(siiii)", "OpenWindow", dwPoints, dwTimeRemaining, dwCurMobsKills, dwMaxMobsKills));
	}
	break;

	case COMBAT_ZONE_SUB_HEADER_REFRESH_SHOP:
	{
		DWORD dwPoints = p.m_pInfoData[0];
		DWORD dwCurLimit = p.m_pInfoData[1];
		DWORD dwMaxLimit = p.m_pInfoData[2];
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CombatZone_Manager", Py_BuildValue("(siii)", "RefreshShop", dwPoints, dwCurLimit, dwMaxLimit));
	}
	break;
	default:
		return false;
	}
	return true;
}
#endif
bool CPythonNetworkStream::RecvFishing()
{
	TPacketGCFishing FishingPacket;
	if (!Recv(sizeof(FishingPacket), &FishingPacket))
		return false;

#if defined(ENABLE_FISHING_GAME)
	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
#endif

	CInstanceBase * pFishingInstance = NULL;
	if (FISHING_SUBHEADER_GC_FISH != FishingPacket.subheader)
	{
		pFishingInstance = CPythonCharacterManager::Instance().GetInstancePtr(FishingPacket.info);
		if (!pFishingInstance)
			return true;
	}

	switch (FishingPacket.subheader)
	{
		case FISHING_SUBHEADER_GC_START:
#if defined(ENABLE_FISHING_GAME)
			if (FishingPacket.bFishingGame)
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingGameStart", Py_BuildValue("(ii)", FishingPacket.bFishingGame, FishingPacket.fishing_key));
				rkPlayer.SetFishingGameWindowOpen(true);
			}
#endif
			break;
		case FISHING_SUBHEADER_GC_STOP:
			{
#if defined(ENABLE_FISHING_GAME)
				if (FishingPacket.bFishingGame)
				{
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingStopGame", Py_BuildValue("()"));
					rkPlayer.SetFishingGameWindowOpen(false);
				}
#endif
			}
			break;
		case FISHING_SUBHEADER_GC_REACT:
			if (pFishingInstance->IsFishing())
			{
				pFishingInstance->SetFishEmoticon(); // Fish Emoticon
				pFishingInstance->ReactFishing();
			}
			break;
		case FISHING_SUBHEADER_GC_SUCCESS:
#if defined(ENABLE_FISHING_GAME)
			if (FishingPacket.bFishingGame)
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingStopGame", Py_BuildValue("()"));
				rkPlayer.SetFishingGameWindowOpen(false);
			}
#endif
			break;
		case FISHING_SUBHEADER_GC_FAIL:
#if defined(ENABLE_FISHING_GAME)
			if (FishingPacket.bFishingGame)
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingStopGame", Py_BuildValue("()"));
				rkPlayer.SetFishingGameWindowOpen(false);
			}
#endif

			if (pFishingInstance == CPythonCharacterManager::Instance().GetMainInstancePtr())
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingFailure", Py_BuildValue("()"));
			}
			break;
		case FISHING_SUBHEADER_GC_FISH:
		{
			DWORD dwFishID = FishingPacket.info;

			if (0 == FishingPacket.info)
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingNotifyUnknown", Py_BuildValue("()"));
				return true;
			}

			CItemData * pItemData;
			if (!CItemManager::Instance().GetItemDataPointer(dwFishID, &pItemData))
				return true;

			CInstanceBase * pMainInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
			if (!pMainInstance)
				return true;

			if (pMainInstance->IsFishing())
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingNotify", Py_BuildValue("(is)", CItemData::ITEM_TYPE_FISH == pItemData->GetType(), pItemData->GetName()));
			}
			else
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingSuccess", Py_BuildValue("(is)", CItemData::ITEM_TYPE_FISH == pItemData->GetType(), pItemData->GetName()));
			}
			break;
		}
		case FISHING_SUBHEADER_GC_START_ANIM:
			if (pFishingInstance)
				pFishingInstance->StartFishing(float(FishingPacket.dir) * 5.0f);
			break;
		case FISHING_SUBHEADER_GC_START_ANIM_END:
			if (pFishingInstance && pFishingInstance->IsFishing())
				pFishingInstance->StopFishing();
			break;
		case FISHING_SUBHEADER_GC_SUCCESS_ANIM:
			if (pFishingInstance)
				pFishingInstance->CatchSuccess();
			break;
		case FISHING_SUBHEADER_GC_FAIL_ANIM:
			if (pFishingInstance)
				pFishingInstance->CatchFail();
	}

	return true;
}
// Fishing
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Dungeon
bool CPythonNetworkStream::RecvDungeon()
{
	TPacketGCDungeon DungeonPacket;
	if (!Recv(sizeof(DungeonPacket), &DungeonPacket))
		return false;

	switch (DungeonPacket.subheader)
	{
	case DUNGEON_SUBHEADER_GC_TIME_ATTACK_START:
	{
		break;
	}
	case DUNGEON_SUBHEADER_GC_DESTINATION_POSITION:
	{
		unsigned long ulx, uly;
		if (!Recv(sizeof(ulx), &ulx))
			return false;
		if (!Recv(sizeof(uly), &uly))
			return false;

		CPythonPlayer::Instance().SetDungeonDestinationPosition(ulx, uly);
		break;
	}
	}

	return true;
}
// Dungeon
/////////////////////////////////////////////////////////////////////////

bool CPythonNetworkStream::RecvTimePacket()
{
	TPacketGCTime TimePacket;
	if (!Recv(sizeof(TimePacket), &TimePacket))
		return false;

	IAbstractApplication& rkApp = IAbstractApplication::GetSingleton();
	rkApp.SetServerTime(TimePacket.time);

	return true;
}

bool CPythonNetworkStream::RecvWalkModePacket()
{
	TPacketGCWalkMode WalkModePacket;
	if (!Recv(sizeof(WalkModePacket), &WalkModePacket))
		return false;

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(WalkModePacket.vid);
	if (pInstance)
	{
		if (WALKMODE_RUN == WalkModePacket.mode)
		{
			pInstance->SetRunMode();
		}
		else
		{
			pInstance->SetWalkMode();
		}
	}

	return true;
}

#ifdef ENABLE_RANKING
bool CPythonNetworkStream::RecvRankingTable()
{
	TPacketGCRankingTable p;
	if (!Recv(sizeof(p), &p))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RankingClearData", Py_BuildValue("()"));
	for (int i = 0; i < MAX_RANKING_LIST; i++) {
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RankingAddRank", Py_BuildValue("(iiisi)", p.list[i].iPosition, p.list[i].iLevel, p.list[i].iPoints, p.list[i].szName, p.list[i].iRealPosition));
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RankingRefresh", Py_BuildValue("()"));
	return true;
}
#endif

bool CPythonNetworkStream::RecvChangeSkillGroupPacket()
{
	TPacketGCChangeSkillGroup ChangeSkillGroup;
	if (!Recv(sizeof(ChangeSkillGroup), &ChangeSkillGroup))
		return false;

	m_dwMainActorSkillGroup = ChangeSkillGroup.skill_group;

	CPythonPlayer::Instance().NEW_ClearSkillData();
	__RefreshCharacterWindow();
	return true;
}
#ifdef TOURNAMENT_PVP_SYSTEM
bool CPythonNetworkStream::RecvTournamentAdd()
{
	TPacketGCTournamentAdd dwPacket;
	if (!Recv(sizeof(dwPacket), &dwPacket))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_Tournament_OnRecvData", Py_BuildValue("(iiiiii)", dwPacket.dwTimeRemained, dwPacket.membersOnline_A, dwPacket.membersOnline_B, dwPacket.membersDead_A, dwPacket.membersDead_B, dwPacket.memberLives));
	return true;
}
#endif
void CPythonNetworkStream::__TEST_SetSkillGroupFake(int iIndex)
{
	m_dwMainActorSkillGroup = DWORD(iIndex);

	CPythonPlayer::Instance().NEW_ClearSkillData();
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshCharacter", Py_BuildValue("()"));
}

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
bool CPythonNetworkStream::RequestFlag(const char* szName)
{
	TPacketCGRequestFlag kRequestFlagPacket;
	kRequestFlagPacket.header = HEADER_CG_REQUEST_FLAG;
	strncpy(kRequestFlagPacket.name, szName, sizeof(kRequestFlagPacket.name));

	if (!Send(sizeof(kRequestFlagPacket), &kRequestFlagPacket))
		return false;

	return SendSequence();
}
#endif

bool CPythonNetworkStream::SendRefinePacket(WORD wPos, BYTE byType, BYTE bySpecialStorage
#ifdef ENABLE_PITTY_REFINE
, bool bUseSealOfGod
#endif
)
{
	TPacketCGRefine p;
	p.header = HEADER_CG_REFINE;
	p.pos = wPos;
	p.type = byType;
	p.special_storage = bySpecialStorage;
#ifdef ENABLE_PITTY_REFINE
	p.seal_of_god = bUseSealOfGod;
#endif

	if (!Send(sizeof(p), &p))
	{
		Tracef(">> SendRefinePacket: cannot be sended.\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendSelectItemPacket(DWORD dwItemPos)
{
	TPacketCGScriptSelectItem kScriptSelectItem;
	kScriptSelectItem.header = HEADER_CG_SCRIPT_SELECT_ITEM;
	kScriptSelectItem.selection = dwItemPos;

	if (!Send(sizeof(kScriptSelectItem), &kScriptSelectItem))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::RecvRefineInformationPacket()
{
	TPacketGCRefineInformation p;
	if (!Recv(sizeof(p), &p))
	{
		Tracef("RecvRefineInformationPacket -> cannot recv: TPacketGCRefineInformation.\n");
		return false;
	}

	auto& rkRefineTable = p.refine_table;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
							"OpenRefineDialog",
							Py_BuildValue("(iiiiii)",
							p.pos,
							p.refine_table.result_vnum,
							rkRefineTable.cost,
							rkRefineTable.prob,
							p.refine_table.special_storage,
							p.type)
	);

	for (int i = 0; i < rkRefineTable.material_count; ++i)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AppendMaterialToRefineDialog", Py_BuildValue("(ii)", rkRefineTable.materials[i].vnum, rkRefineTable.materials[i].count));
	}

#ifdef _DEBUG
	Tracef(" >> RecvRefineInformationPacket(pos=%d, result_vnum=%d, cost=%d, prob=%d, type=%d)\n",
		p.pos,
		p.refine_table.result_vnum,
		rkRefineTable.cost,
		rkRefineTable.prob,
		p.type);
#endif

	return true;
}

#ifndef USE_MARK_ATLAS_BINARY
bool CPythonNetworkStream::RecvNPCList()
{
	TPacketGCNPCPosition kNPCPosition;
	if (!Recv(sizeof(kNPCPosition), &kNPCPosition))
		return false;

	assert(int(kNPCPosition.size) - sizeof(kNPCPosition) == kNPCPosition.count * sizeof(TNPCPosition) && "HEADER_GC_NPC_POSITION");

	CPythonMiniMap::Instance().ClearAtlasMarkInfo();

	for (int i = 0; i < kNPCPosition.count; ++i)
	{
		TNPCPosition NPCPosition;
		if (!Recv(sizeof(TNPCPosition), &NPCPosition))
			return false;

		const char* c_szName;
		CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
		rkNonPlayer.GetName(NPCPosition.mobVnum, &c_szName);

		CPythonMiniMap::Instance().RegisterAtlasMark(NPCPosition.bType, NPCPosition.mobVnum, c_szName, NPCPosition.x, NPCPosition.y, NPCPosition.regenTime);
	}

	return true;
}
#endif

bool CPythonNetworkStream::__SendCRCReportPacket()
{
	/*
	DWORD dwProcessCRC = 0;
	DWORD dwFileCRC = 0;
	CFilename exeFileName;
	//LPCVOID c_pvBaseAddress = NULL;

	GetExeCRC(dwProcessCRC, dwFileCRC);

	CFilename strRootPackFileName = CEterPackManager::Instance().GetRootPacketFileName();
	strRootPackFileName.ChangeDosPath();

	TPacketCGCRCReport kReportPacket;

	kReportPacket.header = HEADER_CG_CRC_REPORT;
	kReportPacket.byPackMode = CEterPackManager::Instance().GetSearchMode();
	kReportPacket.dwBinaryCRC32 = dwFileCRC;
	kReportPacket.dwProcessCRC32 = dwProcessCRC;
	kReportPacket.dwRootPackCRC32 = GetFileCRC32(strRootPackFileName.c_str());

	if (!Send(sizeof(kReportPacket), &kReportPacket))
		Tracef("SendClientReportPacket Error");

	return SendSequence();
	*/
	return true;
}

bool CPythonNetworkStream::SendClientVersionPacket()
{
	std::string filename;

	GetExcutedFileName(filename);

	filename = CFileNameHelper::NoPath(filename);
	CFileNameHelper::ChangeDosPath(filename);

	if (LocaleService_IsEUROPE() && false == LocaleService_IsYMIR())
	{
		TPacketCGClientVersion2 kVersionPacket;
		kVersionPacket.header = HEADER_CG_CLIENT_VERSION2;
		strncpy(kVersionPacket.filename, filename.c_str(), sizeof(kVersionPacket.filename) - 1);
		strncpy(kVersionPacket.timestamp, "1215955205", sizeof(kVersionPacket.timestamp) - 1); // # python time.time 앞자리
		//strncpy(kVersionPacket.timestamp, __TIMESTAMP__, sizeof(kVersionPacket.timestamp)-1); // old_string_ver
		//strncpy(kVersionPacket.timestamp, "1218055205", sizeof(kVersionPacket.timestamp)-1); // new_future
		//strncpy(kVersionPacket.timestamp, "1214055205", sizeof(kVersionPacket.timestamp)-1); // old_past

		if (!Send(sizeof(kVersionPacket), &kVersionPacket))
			Tracef("SendClientReportPacket Error");
	}
	else
	{
		TPacketCGClientVersion kVersionPacket;
		kVersionPacket.header = HEADER_CG_CLIENT_VERSION;
		strncpy(kVersionPacket.filename, filename.c_str(), sizeof(kVersionPacket.filename) - 1);
		strncpy(kVersionPacket.timestamp, __TIMESTAMP__, sizeof(kVersionPacket.timestamp) - 1);

		if (!Send(sizeof(kVersionPacket), &kVersionPacket))
			Tracef("SendClientReportPacket Error");
	}
	return SendSequence();
}

bool CPythonNetworkStream::RecvAffectAddPacket()
{
	TPacketGCAffectAdd p;
	if (!Recv(sizeof(TPacketGCAffectAdd), &p))
	{
		TraceError("Cannot recv: TPacketGCAffectAdd.");
		return false;
	}

	TPacketAffectElement& rkElement = p.elem;
	if (rkElement.bApplyOn == POINT_ENERGY)
	{
		CPythonPlayer::instance().SetStatus(POINT_ENERGY_END_TIME, CPythonApplication::Instance().GetServerTimeStamp() + rkElement.lDuration);
		__RefreshStatus();
	}

#if defined(__BL_BATTLE_ROYALE__)
	if (rkElement.dwType == CInstanceBase::NEW_AFFECT_BATTLE_ROYALE_FIELD_DAMAGE)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenElectricFieldOnScreenImage", Py_BuildValue("(b)", true));
	}
#endif

	CPythonPlayer::Instance().AddAffect(rkElement.dwType, rkElement);
#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_NEW_AddAffect", Py_BuildValue("(iiiii)", rkElement.dwType, rkElement.bApplyOn, rkElement.lApplyValue, rkElement.lDuration, rkElement.dwFlag2));
#else
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_NEW_AddAffect", Py_BuildValue("(iiii)", rkElement.dwType, rkElement.bApplyOn, rkElement.lApplyValue, rkElement.lDuration));
#endif
	__RefreshAffectWindow();
	return true;
}

bool CPythonNetworkStream::RecvAffectRemovePacket()
{
	TPacketGCAffectRemove p;
	if (!Recv(sizeof(p), &p))
	{
		return false;
	}

#if defined(__BL_BATTLE_ROYALE__)
	if (p.dwType == CInstanceBase::NEW_AFFECT_BATTLE_ROYALE_FIELD_DAMAGE)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenElectricFieldOnScreenImage", Py_BuildValue("(b)", false));
	}
#endif

	CPythonPlayer::Instance().RemoveAffect(p.dwType, p.bApplyOn);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_NEW_RemoveAffect", Py_BuildValue("(ii)", p.dwType, p.bApplyOn));
	__RefreshAffectWindow();
	return true;
}

bool CPythonNetworkStream::RecvChannelPacket()
{
	TPacketGCChannel kChannelPacket;
	if (!Recv(sizeof(kChannelPacket), &kChannelPacket))
		return false;
#ifdef ENABLE_CHANNEL_CHANGE
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetChannelIndex", Py_BuildValue("(i)", kChannelPacket.channel));
#endif
	//Tracef(" >> CPythonNetworkStream::RecvChannelPacket(channel=%d)\n", kChannelPacket.channel);
#ifdef ENABLE_ANTI_EXP
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetAntiExp", Py_BuildValue("(i)", kChannelPacket.anti_exp));
#endif
	return true;
}

bool CPythonNetworkStream::RecvViewEquipPacket()
{
	TPacketGCViewEquip kViewEquipPacket;
	if (!Recv(sizeof(kViewEquipPacket), &kViewEquipPacket))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenEquipmentDialog", Py_BuildValue("(i)", kViewEquipPacket.dwVID));

	for (int i = 0; i < WEAR_MAX_NUM; ++i)
	{
		TEquipmentItemSet& rItemSet = kViewEquipPacket.equips[i];
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetEquipmentDialogItem", Py_BuildValue("(iiii)", kViewEquipPacket.dwVID, i, rItemSet.vnum, rItemSet.count));

		for (int j = 0; j < ITEM_SOCKET_SLOT_MAX_NUM; ++j)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetEquipmentDialogSocket", Py_BuildValue("(iiii)", kViewEquipPacket.dwVID, i, j, rItemSet.alSockets[j]));

		for (int k = 0; k < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++k)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetEquipmentDialogAttr", Py_BuildValue("(iiiii)", kViewEquipPacket.dwVID, i, k, rItemSet.aAttr[k].bType, rItemSet.aAttr[k].sValue));

#ifdef ENABLE_YOHARA_SYSTEM
		// for (int s = 0; s < APPLY_RANDOM_SLOT_MAX_NUM; ++s)
			// PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetEquipmentDialogApplyRandom", Py_BuildValue("(iiiii)", kViewEquipPacket.dwVID, i, s, rItemSet.aApplyRandom[s].bType, rItemSet.aApplyRandom[s].sValue));
#endif
	}

	return true;
}

bool CPythonNetworkStream::RecvLandPacket()
{
	TPacketGCLandList kLandList;
	if (!Recv(sizeof(kLandList), &kLandList))
		return false;

	std::vector<DWORD> kVec_dwGuildID;

	CPythonMiniMap& rkMiniMap = CPythonMiniMap::Instance();
	CPythonBackground& rkBG = CPythonBackground::Instance();
	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();

	rkMiniMap.ClearGuildArea();
	rkBG.ClearGuildArea();

	int iPacketSize = (kLandList.size - sizeof(TPacketGCLandList));
	for (; iPacketSize > 0; iPacketSize -= sizeof(TLandPacketElement))
	{
		TLandPacketElement kElement;
		if (!Recv(sizeof(TLandPacketElement), &kElement))
			return false;

		rkMiniMap.RegisterGuildArea(kElement.dwID,
			kElement.dwGuildID,
			kElement.x,
			kElement.y,
			kElement.width,
			kElement.height);

		if (pMainInstance)
			if (kElement.dwGuildID == pMainInstance->GetGuildID())
			{
				rkBG.RegisterGuildArea(kElement.x,
					kElement.y,
					kElement.x + kElement.width,
					kElement.y + kElement.height);
			}

		if (0 != kElement.dwGuildID)
			kVec_dwGuildID.push_back(kElement.dwGuildID);
	}
	// @fixme006
	if (kVec_dwGuildID.size() > 0)
		__DownloadSymbol(kVec_dwGuildID);

	return true;
}

bool CPythonNetworkStream::RecvTargetCreatePacket()
{
	TPacketGCTargetCreate kTargetCreate;
	if (!Recv(sizeof(kTargetCreate), &kTargetCreate))
		return false;

	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	rkpyMiniMap.CreateTarget(kTargetCreate.lID, kTargetCreate.szTargetName);

	//#ifdef _DEBUG
	//	char szBuf[256+1];
	//	_snprintf(szBuf, sizeof(szBuf), "타겟이 생성 되었습니다 [%d:%s]", kTargetCreate.lID, kTargetCreate.szTargetName);
	//	CPythonChat::Instance().AppendChat(CHAT_TYPE_NOTICE, szBuf);
	//	Tracef(" >> RecvTargetCreatePacket %d : %s\n", kTargetCreate.lID, kTargetCreate.szTargetName);
	//#endif

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OpenAtlasWindow", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::RecvTargetCreatePacketNew()
{
	TPacketGCTargetCreateNew kTargetCreate;
	if (!Recv(sizeof(kTargetCreate), &kTargetCreate))
		return false;

	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	CPythonBackground& rkpyBG = CPythonBackground::Instance();
	if (CREATE_TARGET_TYPE_LOCATION == kTargetCreate.byType)
	{
		rkpyMiniMap.CreateTarget(kTargetCreate.lID, kTargetCreate.szTargetName);
	}
#if defined(WJ_COMBAT_ZONE)
	else if (CREATE_TARGET_TYPE_COMBAT_ZONE == kTargetCreate.byType)
	{
		rkpyBG.CreateCombatZoneTargetsLeaving(kTargetCreate.lID, kTargetCreate.dwVID);
		rkpyMiniMap.CreateTarget(kTargetCreate.lID,
#if defined(WJ_COMBAT_ZONE_HIDE_INFO_USER)
			""
#else
			kTargetCreate.szTargetName
#endif
			, kTargetCreate.dwVID);
	}
#endif
	else
	{
		rkpyMiniMap.CreateTarget(kTargetCreate.lID, kTargetCreate.szTargetName, kTargetCreate.dwVID);
		rkpyBG.CreateTargetEffect(kTargetCreate.lID, kTargetCreate.dwVID);
	}

	//#ifdef _DEBUG
	//	char szBuf[256+1];
	//	_snprintf(szBuf, sizeof(szBuf), "캐릭터 타겟이 생성 되었습니다 [%d:%s:%d]", kTargetCreate.lID, kTargetCreate.szTargetName, kTargetCreate.dwVID);
	//	CPythonChat::Instance().AppendChat(CHAT_TYPE_NOTICE, szBuf);
	//	Tracef(" >> RecvTargetCreatePacketNew %d : %d/%d\n", kTargetCreate.lID, kTargetCreate.byType, kTargetCreate.dwVID);
	//#endif

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OpenAtlasWindow", Py_BuildValue("()"));
	return true;
}
//search offline remplace
bool CPythonNetworkStream::RecvTargetUpdatePacket()
{
	TPacketGCTargetUpdate kTargetUpdate;
	if (!Recv(sizeof(kTargetUpdate), &kTargetUpdate))
		return false;


	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	CPythonBackground& rkpyBG = CPythonBackground::Instance();

	rkpyMiniMap.UpdateTarget(kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);



	if (5 == kTargetUpdate.lType) {
		rkpyBG.CreateTargetEffectShop(kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);
	}
	else {
		rkpyBG.CreateTargetEffect(kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);
	}

	return true;
}
//search offline end

bool CPythonNetworkStream::RecvTargetDeletePacket()
{
	TPacketGCTargetDelete kTargetDelete;
	if (!Recv(sizeof(kTargetDelete), &kTargetDelete))
		return false;

	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	rkpyMiniMap.DeleteTarget(kTargetDelete.lID);

	CPythonBackground& rkpyBG = CPythonBackground::Instance();
	rkpyBG.DeleteTargetEffect(kTargetDelete.lID);

	//#ifdef _DEBUG
	//	Tracef(" >> RecvTargetDeletePacket %d\n", kTargetDelete.lID);
	//#endif

	return true;
}

#ifdef ENABLE_YOHARA_SYSTEM
bool CPythonNetworkStream::RecvSungmaAttrUpdatePacket()
{
	TPacketGCSungmaAttrUpdate kSungmaAttrUpdate;
	if (!Recv(sizeof(kSungmaAttrUpdate), &kSungmaAttrUpdate))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SungMaAttr", Py_BuildValue("(iiii)", kSungmaAttrUpdate.bSTR, kSungmaAttrUpdate.bHP, kSungmaAttrUpdate.bMOVE, kSungmaAttrUpdate.bINMUNE));
#ifdef _DEBUG
	Tracef("RECV SUNGMA ATTR UPDATE : %d\n", kSungmaAttrUpdate.bSTR, kSungmaAttrUpdate.bHP, kSungmaAttrUpdate.bMOVE, kSungmaAttrUpdate.bINMUNE);
#endif
	return true;
}
#endif


bool CPythonNetworkStream::RecvLoverInfoPacket()
{
	TPacketGCLoverInfo kLoverInfo;
	if (!Recv(sizeof(kLoverInfo), &kLoverInfo))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_LoverInfo", Py_BuildValue("(si)", kLoverInfo.szName, kLoverInfo.byLovePoint));
#ifdef _DEBUG
	Tracef("RECV LOVER INFO : %s, %d\n", kLoverInfo.szName, kLoverInfo.byLovePoint);
#endif
	return true;
}

bool CPythonNetworkStream::RecvLovePointUpdatePacket()
{
	TPacketGCLovePointUpdate kLovePointUpdate;
	if (!Recv(sizeof(kLovePointUpdate), &kLovePointUpdate))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_UpdateLovePoint", Py_BuildValue("(i)", kLovePointUpdate.byLovePoint));
#ifdef _DEBUG
	Tracef("RECV LOVE POINT UPDATE : %d\n", kLovePointUpdate.byLovePoint);
#endif
	return true;
}

bool CPythonNetworkStream::RecvDigMotionPacket()
{
	TPacketGCDigMotion kDigMotion;
	if (!Recv(sizeof(kDigMotion), &kDigMotion))
		return false;

#ifdef _DEBUG
	Tracef(" Dig Motion [%d/%d]\n", kDigMotion.vid, kDigMotion.count);
#endif

	IAbstractCharacterManager& rkChrMgr = IAbstractCharacterManager::GetSingleton();
	CInstanceBase* pkInstMain = rkChrMgr.GetInstancePtr(kDigMotion.vid);
	CInstanceBase* pkInstTarget = rkChrMgr.GetInstancePtr(kDigMotion.target_vid);
	if (NULL == pkInstMain)
		return true;

	if (pkInstTarget)
		pkInstMain->NEW_LookAtDestInstance(*pkInstTarget);

	for (int i = 0; i < kDigMotion.count; ++i)
		pkInstMain->PushOnceMotion(CRaceMotionData::NAME_DIG);

	return true;
}

#ifdef ENABLE_KINGDOMS_WAR
bool CPythonNetworkStream::RecvKingdomsWarPacket(bool bReturn)
{
	TPacketKingdomWar sPacket;
	if (!Recv(sizeof(sPacket), &sPacket))
		return bReturn;

	bReturn = true;
	switch (sPacket.bSubHeader)
	{
	case KINGDOMSWAR_SUBHEADER_GC_OPEN:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActKingdomsWar", Py_BuildValue("(iiiiiiii)", 1, sPacket.iKills[0], sPacket.iKills[1], sPacket.iKills[2], sPacket.iLimitKills, sPacket.iDeads, sPacket.iLimitDeads, sPacket.dwTimeRemained));
	}
	break;
	case KINGDOMSWAR_SUBHEADER_GC_REFRESH:
	{
		if (sPacket.iLimitDeads > 0)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActKingdomsWar", Py_BuildValue("(iiiiiiii)", 3, sPacket.iKills[0], sPacket.iKills[1], sPacket.iKills[2], sPacket.iLimitKills, sPacket.iDeads, sPacket.iLimitDeads, sPacket.dwTimeRemained));
		else
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActKingdomsWar", Py_BuildValue("(iiiiiiii)", 2, sPacket.iKills[0], sPacket.iKills[1], sPacket.iKills[2], sPacket.iLimitKills, sPacket.iDeads, sPacket.iLimitDeads, sPacket.dwTimeRemained));
	}
	break;
	default:
		TraceError("CPythonNetworkStream::RecvKingdomsWarPacket: unknown subheader %d\n.", sPacket.bSubHeader);
		break;
	}

	return bReturn;
}
#endif

// 용혼석 강화
bool CPythonNetworkStream::SendDragonSoulRefinePacket(BYTE bRefineType, TItemPos* pos)
{
	TPacketCGDragonSoulRefine pk;
	pk.header = HEADER_CG_DRAGON_SOUL_REFINE;
	pk.bSubType = bRefineType;
	memcpy(pk.ItemGrid, pos, sizeof(TItemPos) * DS_REFINE_WINDOW_MAX_NUM);
	if (!Send(sizeof(pk), &pk))
	{
		return false;
	}
	return true;
}
#ifdef ENABLE_SASH_SYSTEM
bool CPythonNetworkStream::RecvSashPacket(bool bReturn)
{
	TPacketSash sPacket;
	if (!Recv(sizeof(sPacket), &sPacket))
		return bReturn;

	bReturn = true;
	switch (sPacket.subheader)
	{
	case SASH_SUBHEADER_GC_OPEN:
	{
		CPythonSash::Instance().Clear();
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActSash", Py_BuildValue("(ib)", 1, sPacket.bWindow));
	}
	break;
	case SASH_SUBHEADER_GC_CLOSE:
	{
		CPythonSash::Instance().Clear();
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActSash", Py_BuildValue("(ib)", 2, sPacket.bWindow));
	}
	break;
	case SASH_SUBHEADER_GC_ADDED:
	{
		CPythonSash::Instance().AddMaterial(sPacket.dwPrice, sPacket.bPos, sPacket.tPos);
		if (sPacket.bPos == 1)
		{
			CPythonSash::Instance().AddResult(sPacket.dwItemVnum, sPacket.dwMinAbs, sPacket.dwMaxAbs);
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AlertSash", Py_BuildValue("(b)", sPacket.bWindow));
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActSash", Py_BuildValue("(ib)", 3, sPacket.bWindow));
	}
	break;
	case SASH_SUBHEADER_GC_REMOVED:
	{
		CPythonSash::Instance().RemoveMaterial(sPacket.dwPrice, sPacket.bPos);
		if (sPacket.bPos == 0)
			CPythonSash::Instance().RemoveMaterial(sPacket.dwPrice, 1);

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActSash", Py_BuildValue("(ib)", 4, sPacket.bWindow));
	}
	break;
	case SASH_SUBHEADER_CG_REFINED:
	{
		if (sPacket.dwMaxAbs == 0)
			CPythonSash::Instance().RemoveMaterial(sPacket.dwPrice, 1);
		else
		{
			CPythonSash::Instance().RemoveMaterial(sPacket.dwPrice, 0);
			CPythonSash::Instance().RemoveMaterial(sPacket.dwPrice, 1);
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActSash", Py_BuildValue("(ib)", 4, sPacket.bWindow));
	}
	break;
	default:
		TraceError("CPythonNetworkStream::RecvSashPacket: unknown subheader %d\n.", sPacket.subheader);
		break;
	}

	return bReturn;
}

bool CPythonNetworkStream::SendSashClosePacket()
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketSash sPacket;
	sPacket.header = HEADER_CG_SASH;
	sPacket.subheader = SASH_SUBHEADER_CG_CLOSE;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;
	return SendSequence();
}

bool CPythonNetworkStream::SendSashAddPacket(TItemPos tPos, BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketSash sPacket;
	sPacket.header = HEADER_CG_SASH;
	sPacket.subheader = SASH_SUBHEADER_CG_ADD;
	sPacket.dwPrice = 0;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendSashRemovePacket(BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketSash sPacket;
	sPacket.header = HEADER_CG_SASH;
	sPacket.subheader = SASH_SUBHEADER_CG_REMOVE;
	sPacket.dwPrice = 0;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendSashRefinePacket()
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketSash sPacket;
	sPacket.header = HEADER_CG_SASH;
	sPacket.subheader = SASH_SUBHEADER_CG_REFINE;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return SendSequence();
}
#endif


#ifdef ENABLE_DUNGEON_INFO
bool CPythonNetworkStream::RecvDungeonInfoPacket()
{

	TPacketGCDungeonInfoReceive KDungeonInfo;

	if (!Recv(sizeof(KDungeonInfo), &KDungeonInfo))
		return false;

	switch (KDungeonInfo.bSubType)
	{

	case DUNGEON_INFO_SUB_HEADER_OPEN_SEND:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_TABLE_DUNGEON_INFO_OPEN", Py_BuildValue("()"));
	}
	break;

	case DUNGEON_INFO_SUB_HEADER_DATES_CLEAR:
	{
		CPythonDungeonInfo::Instance().Clear();
	}
	break;

	case DUNGEON_INFO_SUB_HEADER_DATES_MISION_CLEAR:
	{
		CPythonDungeonInfo::Instance().ClearMision();

	}
	break;

	case DUNGEON_INFO_SUB_HEADER_DATES_SEND:
	{

		CPythonDungeonInfo::Instance().DateInfoList(KDungeonInfo.dateinfo);
	}
	break;

	case DUNGEON_INFO_SUB_HEADER_DATES_MISION_SEND:
	{
		CPythonDungeonInfo::Instance().DateInfoMisionList(KDungeonInfo.dateinfom);
	}

	case DUNGEON_INFO_SUB_HEADER_DATES_RANKING_CLEAR:
	{
		CPythonDungeonInfo::Instance().ClearRanking();
	}
	break;

	case DUNGEON_INFO_SUB_HEADER_DATES_RANKING_SEND:
	{

		CPythonDungeonInfo::Instance().DateInfoRankingList(KDungeonInfo.dateinfo_ranking);
	}
	break;
	case DUNGEON_INFO_SUB_HEADER_DATES_RANKING_LOAD:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_TABLE_DUNGEON_RANKING_LOAD", Py_BuildValue("()"));
	}
	break;
	case DUNGEON_INFO_SUB_HEADER_DATES_MISION_LOAD:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_TABLE_DUNGEON_MISION_LOAD", Py_BuildValue("()"));
	}
	break;
	}

	return true;
}
#endif


#ifdef ENABLE_DROP_ITEM_WORLDARD

bool CPythonNetworkStream::SendDropItemOpen()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGDropItemSend	packet;

	packet.header = HEADER_CG_DROP_ITEM_SYSTEM;
	packet.subheader = DROP_ITEM_SUB_HEADER_OPEN;

	if (!Send(sizeof(TPacketCGDropItemSend), &packet))
	{
		Tracef("SendDropItemOpen Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SearchDropItem(int vnum_item)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGDropItemSend	packet;

	packet.header = HEADER_CG_DROP_ITEM_SYSTEM;
	packet.subheader = DROP_ITEM_SUB_HEADER_SEARCH;
	packet.vnum_item = vnum_item;

	if (!Send(sizeof(TPacketCGDropItemSend), &packet))
	{
		Tracef("SearchDropItem Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::RecvDropItemPacket()
{

	TPacketGCDropItemReceive WDropItem;

	if (!Recv(sizeof(WDropItem), &WDropItem))
		return false;

	switch (WDropItem.subheader)
	{

	case DROP_ITEM_SUB_HEADER_OPEN_RECEIVE:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_DROP_ITEM_OPEN", Py_BuildValue("()"));
	}
	break;

	case DROP_ITEM_SUB_HEADER_DATES_RECEIVE:
	{
		CPythonDropItem::Instance().ReceiveList(WDropItem.info_drop);
	}
	break;


	case DROP_ITEM_SUB_HEADER_DATES_LOADING:
	{
		CPythonDropItem::Instance().LoadingList();
	}
	break;

	case DROP_ITEM_SUB_HEADER_DATES_CLEAR_RECEIVE:
	{
		CPythonDropItem::Instance().ClearList();
	}
	break;
	}

	return true;
}

#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD

bool CPythonNetworkStream::CubeRenewalMakeItem(int index_item, int count_item)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGCubeRenewalSend	packet;

	packet.header = HEADER_CG_CUBE_RENEWAL;
	packet.subheader = CUBE_RENEWAL_SUB_HEADER_MAKE_ITEM;
	packet.index_item = index_item;
	packet.count_item = count_item;

	if (!Send(sizeof(TPacketCGCubeRenewalSend), &packet))
	{
		Tracef("CPythonNetworkStream::CubeRenewalMakeItem Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::CubeRenewalClose()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGCubeRenewalSend	packet;

	packet.header = HEADER_CG_CUBE_RENEWAL;
	packet.subheader = CUBE_RENEWAL_SUB_HEADER_CLOSE;

	if (!Send(sizeof(TPacketCGCubeRenewalSend), &packet))
	{
		Tracef("CPythonNetworkStream::CubeRenewalClose Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::RecvCubeRenewalPacket()
{

	TPacketGCCubeRenewalReceive CubeRenewalPacket;

	if (!Recv(sizeof(CubeRenewalPacket), &CubeRenewalPacket))
		return false;

	switch (CubeRenewalPacket.subheader)
	{

	case CUBE_RENEWAL_SUB_HEADER_OPEN_RECEIVE:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CUBE_RENEWAL_OPEN", Py_BuildValue("()"));
	}
	break;

	case CUBE_RENEWAL_SUB_HEADER_DATES_RECEIVE:
	{
		CPythonCubeRenewal::Instance().ReceiveList(CubeRenewalPacket.date_cube_renewal);
	}
	break;

	case CUBE_RENEWAL_SUB_HEADER_DATES_LOADING:
	{
		CPythonCubeRenewal::Instance().LoadingList();
	}
	break;

	case CUBE_RENEWAL_SUB_HEADER_CLEAR_DATES_RECEIVE:
	{
		CPythonCubeRenewal::Instance().ClearList();
	}
	break;
	}

	return true;
}

#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD

bool CPythonNetworkStream::SendSwitchBotOpen()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGSwitchBotSend	packet;

	packet.header = HEADER_CG_SWITCHBOT;
	packet.subheader = SWITCH_SUB_HEADER_OPEN;

	if (!Send(sizeof(TPacketCGSwitchBotSend), &packet))
	{
		Tracef("SendSwitchBotOpen Error\n");
		return false;
	}

	return SendSequence();
}


bool CPythonNetworkStream::SendSwitchBotClose()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGSwitchBotSend	packet;

	packet.header = HEADER_CG_SWITCHBOT;
	packet.subheader = SWITCH_SUB_HEADER_CLOSE;

	if (!Send(sizeof(TPacketCGSwitchBotSend), &packet))
	{
		Tracef("SendSwitchBotClose Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendSwitchBotChange(PyObject* attr)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGSwitchBotSend	packet;

	packet.header = HEADER_CG_SWITCHBOT;
	packet.subheader = SWITCH_SUB_HEADER_CHANGE;


	PyObject* w0;
	PyObject* wa_type;
	PyObject* wa_value;

	int size_list = PyList_Size(attr);

	if (size_list < 0 || size_list > 5)
	{
		return true;
	}

	for (int i = 0; i < size_list; ++i)
	{
		w0 = PyList_GetItem(attr, i);
		wa_type = PyList_GetItem(w0, 0);
		wa_value = PyList_GetItem(w0, 1);

		packet.aAttr[i].bType = PyInt_AsLong(wa_type);
		packet.aAttr[i].sValue = PyInt_AsLong(wa_value);
	}

	if (!Send(sizeof(TPacketCGSwitchBotSend), &packet))
	{
		Tracef("SendSwitchBotChange Error\n");
		return false;
	}

	return SendSequence();
}


#endif

#ifdef ENABLE_DISCORD_RPC
static int64_t StartTime;

void CPythonNetworkStream::Discord_Start()
{
	StartTime = time(0);
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize(Discord::DiscordClientID, &handlers, 1, nullptr);
	Discord_Update(false);
}
void CPythonNetworkStream::Discord_Update(const bool ingame)
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.startTimestamp = StartTime;


	discordPresence.state = "Ameria.to";
	discordPresence.details = "Battle Royal";
	discordPresence.largeImageKey = "dracarwe";
	discordPresence.largeImageText = "Ameria";
	//discordPresence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
	//discordPresence.joinSecret = "MTI4NzM0OjFpMmhuZToxMjMxMjM= ";


	if (!ingame) {
		Discord_UpdatePresence(&discordPresence);
		return;
	}

	/*Name*/
	//auto NameData = Discord::GetNameData();
	//discordPresence.state = std::get<0>(NameData).c_str();
	//discordPresence.details = std::get<1>(NameData).c_str();

	/*Race*/
	//auto RaceData = Discord::GetRaceData();
	//discordPresence.largeImageKey = std::get<0>(RaceData).c_str();
	//discordPresence.largeImageText = std::get<1>(RaceData).c_str();

	///*Empire*/
	//auto EmpireData = Discord::GetEmpireData();
	//discordPresence.smallImageKey = std::get<0>(EmpireData).c_str();
	//discordPresence.smallImageText = std::get<1>(EmpireData).c_str();

	Discord_UpdatePresence(&discordPresence);
}
void CPythonNetworkStream::Discord_Close()
{
	Discord_Shutdown();
}
#endif



#ifdef BATTLEPASS_WORLDARD

bool CPythonNetworkStream::BattlePassOpen()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGBattlePass	packet;

	packet.header = HEADER_CG_BATTLEPASS;
	packet.subheader = BATTLEPASS_SUB_HEADER_OPEN;

	if (!Send(sizeof(TPacketCGBattlePass), &packet))
	{
		Tracef("CPythonNetworkStream::BattlePassOpen Error\n");
		return false;
	}

	return SendSequence();
}


bool CPythonNetworkStream::BattlePassReceiveReward(int index)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGBattlePass	packet;

	packet.header = HEADER_CG_BATTLEPASS;
	packet.subheader = BATTLEPASS_SUB_HEADER_REWARD;
	packet.index = index;

	if (!Send(sizeof(TPacketCGBattlePass), &packet))
	{
		Tracef("CPythonNetworkStream::BattlePassReceiveReward Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::RecvBattlePassPacket()
{

	TPacketGCBattlePass packet;

	if (!Recv(sizeof(packet), &packet))
		return false;

	switch (packet.subheader)
	{

	case BATTLEPASS_SUB_HEADER_OPEN:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BATTLEPASS_OPEN", Py_BuildValue("()"));
	}
	break;

	case BATTLEPASS_SUB_HEADER_MP:
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_BATTLEPASS_MP", Py_BuildValue("()"));
	}
	break;

	case BATTLEPASS_SUB_HEADER_INFO_EXTRA:
	{
		CPythonBattlePass::Instance().ReceiveDataExtra(packet.time);
	}
	break;

	case BATTLEPASS_SUB_HEADER_UPDATE:
	case BATTLEPASS_SUB_HEADER_INFO:
	{
		CPythonBattlePass::Instance().ReceiveData(packet.subheader, packet.data);
	}
	break;

	case BATTLEPASS_SUB_HEADER_UPDATE_LOADING:
	case BATTLEPASS_SUB_HEADER_LOADING:
	{
		CPythonBattlePass::Instance().LoadingData(packet.subheader);
	}
	break;

	case BATTLEPASS_SUB_HEADER_UPDATE_CLEAR:
	case BATTLEPASS_SUB_HEADER_CLEAR:
	{
		CPythonBattlePass::Instance().ClearData(packet.subheader);
	}
	break;



	}

	return true;
}

#endif

#ifdef ENABLE_ITEMSHOP
bool CPythonNetworkStream::RecvItemShop()
{
	TPacketGCItemShop p;
	if (!Recv(sizeof(p), &p))
	{
#ifdef _DEBUG
		Tracenf("RecvItemShop -> cannot recv: TPacketGCItemShop.");
#endif
		return false;
	}

	BYTE subHeader;
	if (!Recv(sizeof(subHeader), &subHeader))
	{
#ifdef _DEBUG
		Tracenf("RecvItemShop -> cannot recv: subHeader.");
#endif
		return false;
	}

	switch (subHeader)
	{
		case ITEMSHOP_DRAGONCOIN:
		{
			long long act_lldCoins;
			if (!Recv(sizeof(act_lldCoins), &act_lldCoins))
			{
#ifdef _DEBUG
				Tracenf("RecvItemShop -> cannot recv: act_lldCoins (subHeader: %u).", subHeader);
#endif
				return false;
			}

#ifdef USE_ITEMSHOP_RENEWED
			long long act_lldJCoins;
			if (!Recv(sizeof(act_lldJCoins), &act_lldJCoins))
			{
#ifdef _DEBUG
				Tracenf("RecvItemShop -> cannot recv: act_lldJCoins (subHeader: %u).", subHeader);
#endif
				return false;
			}
#endif

			bool isLogOpen;
			if (!Recv(sizeof(isLogOpen), &isLogOpen))
			{
#ifdef _DEBUG
				Tracenf("RecvItemShop -> cannot recv: isLogOpen (subHeader: %u).", subHeader);
#endif
				return false;
			}

			if (isLogOpen)
			{
				TIShopLogData logData;
				if (!Recv(sizeof(logData), &logData))
				{
#ifdef _DEBUG
					Tracenf("RecvItemShop -> cannot recv: logData (subHeader: %u).", subHeader);
#endif
					return false;
				}

#ifdef USE_ITEMSHOP_RENEWED
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopAppendLog",
										Py_BuildValue("(sissiiLL)",
										logData.buyDate,
										logData.buyTime,
										logData.playerName,
										logData.ipAdress,
										logData.itemVnum,
										logData.itemCount,
										logData.itemPrice,
										logData.itemPriceJD));
#else
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopAppendLog",
										Py_BuildValue("(sissiiL)",
										logData.buyDate,
										logData.buyTime,
										logData.playerName,
										logData.ipAdress,
										logData.itemVnum,
										logData.itemCount,
										logData.itemPrice));
#endif
			}

#ifdef USE_ITEMSHOP_RENEWED
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopSetDragonCoin",
									Py_BuildValue("(LL)",
									act_lldCoins,
									act_lldJCoins));
#else
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopSetDragonCoin",
									Py_BuildValue("(L)",
									act_lldCoins));
#endif

			return true;
		}
		case ITEMSHOP_LOG:
		{
			int logCount;
			if (!Recv(sizeof(int), &logCount))
			{
#ifdef _DEBUG
				Tracenf("RecvItemShop -> cannot recv: logCount (subHeader: %u).", subHeader);
#endif
				return false;
			}

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopHideLoading",
									Py_BuildValue("()"));

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopLogClear",
									Py_BuildValue("()"));

			if (logCount > 0)
			{
				std::vector<TIShopLogData> m_vec;
				m_vec.resize(logCount);

				if (!Recv(sizeof(TIShopLogData) * logCount, &m_vec[0]))
				{
#ifdef _DEBUG
					Tracenf("RecvItemShop -> cannot recv: m_vec (subHeader: %u).", subHeader);
#endif
					return false;
				}

				for (auto j = 0; j < m_vec.size(); ++j)
				{
#ifdef USE_ITEMSHOP_RENEWED
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopAppendLog",
											Py_BuildValue("(sissiiLL)",
											m_vec[j].buyDate,
											m_vec[j].buyTime,
											m_vec[j].playerName,
											m_vec[j].ipAdress,
											m_vec[j].itemVnum,
											m_vec[j].itemCount,
											m_vec[j].itemPrice,
											m_vec[j].itemPriceJD));
#else
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopAppendLog",
											Py_BuildValue("(sissiiL)",
											m_vec[j].buyDate,
											m_vec[j].buyTime,
											m_vec[j].playerName,
											m_vec[j].ipAdress,
											m_vec[j].itemVnum,
											m_vec[j].itemCount,
											m_vec[j].itemPrice));
#endif
				}
			}

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopPurchasesWindow",
									Py_BuildValue("()"));

			return true;
		}
		case ITEMSHOP_UPDATE_ITEM:
		{
			TIShopData updateItem;
			if (!Recv(sizeof(updateItem), &updateItem))
			{
#ifdef _DEBUG
				Tracenf("RecvItemShop -> cannot recv: updateItem (subHeader: %u).", subHeader);
#endif
				return false;
			}

#ifdef USE_ITEMSHOP_RENEWED
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopUpdateItem",
									Py_BuildValue("(iiLiiiiLiL)",
									updateItem.id,
									updateItem.itemVnum,
									updateItem.itemPrice,
									updateItem.discount,
									updateItem.offerTime,
									updateItem.topSellingIndex,
									updateItem.addedTime,
									updateItem.sellCount,
									updateItem.maxSellCount,
									updateItem.itemPriceJD));
#else
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopUpdateItem",
									Py_BuildValue("(iiLiiiiLi)",
									updateItem.id,
									updateItem.itemVnum,
									updateItem.itemPrice,
									updateItem.discount,
									updateItem.offerTime,
									updateItem.topSellingIndex,
									updateItem.addedTime,
									updateItem.sellCount,
									updateItem.maxSellCount));
#endif

			return true;
		}
		case ITEMSHOP_LOAD:
		{
			long long act_lldCoins;
			if (!Recv(sizeof(act_lldCoins), &act_lldCoins))
			{
#ifdef _DEBUG
				Tracenf("RecvItemShop -> cannot recv: act_lldCoins (subHeader: %u).", subHeader);
#endif
				return false;
			}

#ifdef USE_ITEMSHOP_RENEWED
			long long act_lldJCoins;
			if (!Recv(sizeof(act_lldJCoins), &act_lldJCoins))
			{
#ifdef _DEBUG
				Tracenf("RecvItemShop -> cannot recv: act_lldJCoins (subHeader: %u).", subHeader);
#endif
				return false;
			}
#endif

			int updateTime;
			if (!Recv(sizeof(updateTime), &updateTime))
			{
#ifdef _DEBUG
				Tracenf("RecvItemShop -> cannot recv: updateTime (subHeader: %u).", subHeader);
#endif
				return false;
			}

#ifdef USE_ITEMSHOP_RENEWED
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopSetDragonCoin",
									Py_BuildValue("(LL)",
									act_lldCoins,
									act_lldJCoins));
#else
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopSetDragonCoin",
									Py_BuildValue("(L)",
									act_lldCoins));
#endif

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopHideLoading",
									Py_BuildValue("()"));

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopClear",
									Py_BuildValue("(i)", updateTime));

			int categoryTotalSize;
			if (!Recv(sizeof(categoryTotalSize), &categoryTotalSize))
			{
#ifdef _DEBUG
				Tracenf("RecvItemShop -> cannot recv: categoryTotalSize (subHeader: %u).", subHeader);
#endif
				return false;
			}

			if (categoryTotalSize == 9999)
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopOpenMainPage",
										Py_BuildValue("()"));

				return true;
			}


			for (auto j = 0; j < categoryTotalSize; ++j)
			{
				BYTE categoryIndex;
				if (!Recv(sizeof(categoryIndex), &categoryIndex))
				{
#ifdef _DEBUG
					Tracenf("RecvItemShop -> cannot recv: categoryIndex (subHeader: %u).", subHeader);
#endif
					return false;
				}

				BYTE categorySize;
				if (!Recv(sizeof(categorySize), &categorySize))
				{
#ifdef _DEBUG
					Tracenf("RecvItemShop -> cannot recv: categorySize (subHeader: %u).", subHeader);
#endif
					return false;
				}

				for (auto x = 0; x < categorySize; ++x)
				{
					BYTE categorySubHeader;
					if (!Recv(sizeof(categorySubHeader), &categorySubHeader))
					{
#ifdef _DEBUG
						Tracenf("RecvItemShop -> cannot recv: categorySubHeader (subHeader: %u).", subHeader);
#endif
						return false;
					}

					BYTE categorySubSize;
					if (!Recv(sizeof(categorySubSize), &categorySubSize))
					{
#ifdef _DEBUG
						Tracenf("RecvItemShop -> cannot recv: categorySubSize (subHeader: %u).", subHeader);
#endif
						return false;
					}

					if (categorySubSize > 0)
					{
						for (auto k = 0; k < categorySubSize; ++k)
						{
							TIShopData shopData;
							if (!Recv(sizeof(shopData), &shopData))
							{
#ifdef _DEBUG
								Tracenf("RecvItemShop -> cannot recv: shopData (subHeader: %u).", subHeader);
#endif
								return false;
							}

#ifdef USE_ITEMSHOP_RENEWED
							PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopAppendItem",
													Py_BuildValue("(iiiiLiiiiLiL)",
													categoryIndex,
													categorySubHeader,
													shopData.id,
													shopData.itemVnum,
													shopData.itemPrice,
													shopData.discount,
													shopData.offerTime,
													shopData.topSellingIndex,
													shopData.addedTime,
													shopData.sellCount,
													shopData.maxSellCount,
													shopData.itemPriceJD));
#else
							PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopAppendItem",
													Py_BuildValue("(iiiiLiiiiLi)",
													categoryIndex,
													categorySubHeader,
													shopData.id,
													shopData.itemVnum,
													shopData.itemPrice,
													shopData.discount,
													shopData.offerTime,
													shopData.topSellingIndex,
													shopData.addedTime,
													shopData.sellCount,
													shopData.maxSellCount));
#endif
						}
					}
				}

				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ItemShopOpenMainPage",
										Py_BuildValue("()"));
			}

			return true;
		}
		default:
		{
#ifdef _DEBUG
			TraceError("RecvItemShop -> invalid subHeader: %u.", subHeader);
#endif
			return false;
		}
	}
}
#endif

#ifdef ENABLE_EVENT_MANAGER
bool CPythonNetworkStream::RecvEventManager()
{
	TPacketGCEventManager p;
	if (!Recv(sizeof(TPacketGCEventManager), &p))
		return false;
	BYTE subHeader;
	if (!Recv(sizeof(BYTE), &subHeader))
		return false;
	if (subHeader == EVENT_MANAGER_LOAD)
	{
		BYTE dayCount;
		if (!Recv(sizeof(BYTE), &dayCount))
			return false;
		int serverTime;
		if (!Recv(sizeof(int), &serverTime))
			return false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ClearEventManager", Py_BuildValue("()"));
		for (BYTE j = 0; j < dayCount; ++j)
		{
			BYTE dayIndex;
			if (!Recv(sizeof(BYTE), &dayIndex))
				return false;
			BYTE dayEventCount;
			if (!Recv(sizeof(BYTE), &dayEventCount))
				return false;
			if (dayEventCount > 0)
			{
				std::vector<TEventManagerData> m_vec;
				m_vec.resize(dayEventCount);
				if (!Recv(dayEventCount * sizeof(TEventManagerData), &m_vec[0]))
					return false;
				for (const auto& eventPtr : m_vec)
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AppendEvent", Py_BuildValue("(iiissiiiiiiiii)", dayIndex, eventPtr.eventID, eventPtr.eventIndex, eventPtr.startTimeText, eventPtr.endTimeText, eventPtr.empireFlag, eventPtr.channelFlag, eventPtr.value[0], eventPtr.value[1], eventPtr.value[2], eventPtr.value[3], eventPtr.startTime - serverTime, eventPtr.endTime == 0 ? 0 : eventPtr.endTime - serverTime, eventPtr.eventStatus));
			}
		}
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshEventManager", Py_BuildValue("()"));
	}
	else if (subHeader == EVENT_MANAGER_EVENT_STATUS)
	{
		WORD eventID;
		if (!Recv(sizeof(WORD), &eventID))
			return false;
		bool eventStatus;
		if (!Recv(sizeof(bool), &eventStatus))
			return false;
		int endTime;
		if (!Recv(sizeof(int), &endTime))
			return false;
		char endTimeText[25];
		if (!Recv(sizeof(endTimeText), &endTimeText))
			return false;
		int serverTime;
		if (!Recv(sizeof(int), &serverTime))
			return false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshEventStatus", Py_BuildValue("(iiis)", eventID, eventStatus, endTime == 0 ? 0 : endTime - serverTime, endTimeText));
	}
	return true;
}
#endif

#ifdef ENABLE_NEW_DETAILS_GUI
bool CPythonNetworkStream::RecvKillLOG()
{
	TPacketGCKillLOG log;
	if (!Recv(sizeof(TPacketGCKillLOG), &log))
		return false;
	CPythonPlayer::Instance().SetKillLOG(log.kill_log);
	//PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshCharacter", Py_BuildValue("()"));
	return true;
}
#endif

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
bool CPythonNetworkStream::RecvCShieldPacket()
{
	TPacketGCCShield pCShieldPacket;

	if (!Recv(sizeof(TPacketGCCShield), &pCShieldPacket))
	{
		Tracen("Recv CShield Packet Error");
		return false;
	}

	SendCShieldPacket(pCShieldPacket.start, pCShieldPacket.num);

	return true;
}

bool CPythonNetworkStream::SendCShieldPacket(bool start, DWORD num)
{
	TPacketCGCShield pCShieldPacket;
	pCShieldPacket.header = HEADER_CG_CSHIELD;

	strncpy(pCShieldPacket.key, GenKey(start, num).c_str(), sizeof(pCShieldPacket.key) - 1);

	if (!Send(sizeof(pCShieldPacket), &pCShieldPacket))
		return false;

	return true;
}
#endif


#ifdef ENABLE_EXCHANGE_LOG
bool CPythonNetworkStream::RecvExchangeLog()
{
	TPacketGCExchangeLog p;
	if (!Recv(sizeof(TPacketGCExchangeLog), &p))
		return false;
	BYTE subHeader;
	if (!Recv(sizeof(BYTE), &subHeader))
		return false;

	if (subHeader == SUB_EXCHANGELOG_LOAD)
	{

		char playerCode[19];
		if (!Recv(sizeof(playerCode), &playerCode))
			return false;


		bool isNeedClean;
		if (!Recv(sizeof(bool), &isNeedClean))
			return false;


		WORD logCount;
		if (!Recv(sizeof(WORD), &logCount))
			return false;


		if (isNeedClean)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ExchangeLogClear", Py_BuildValue("(s)", playerCode));


		for (DWORD j = 0; j < logCount; ++j)
		{
			DWORD logID;
			if (!Recv(sizeof(DWORD), &logID))
				return false;
			TExchangeLog logData;
			if (!Recv(sizeof(TExchangeLog), &logData))
				return false;
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ExchangeLogAppend", Py_BuildValue("(isLssLss)", logID, logData.owner, logData.ownerGold, logData.ownerIP, logData.target, logData.targetGold, logData.targetIP, logData.date));
		}
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ExchangeLogRefresh", Py_BuildValue("(i)", 0));
	}
	else if (subHeader == SUB_EXCHANGELOG_LOAD_ITEM)
	{
		WORD logItemCount;
		if (!Recv(sizeof(WORD), &logItemCount))
			return false;
		DWORD logID;
		if (!Recv(sizeof(DWORD), &logID))
			return false;
		if (logItemCount)
		{
			std::vector<TExchangeLogItem> m_vecLogItems;
			m_vecLogItems.resize(logItemCount);
			if (!Recv(logItemCount * sizeof(TExchangeLogItem), m_vecLogItems.data()))
				return false;
			for (DWORD j = 0; j < m_vecLogItems.size(); ++j)
			{
				const TExchangeLogItem& logItem = m_vecLogItems[j];
				PyObject* poSockets = PyList_New(0);
				for (DWORD x = 0; x < ITEM_SOCKET_SLOT_MAX_NUM; ++x)
					PyList_Append(poSockets, PyInt_FromLong(logItem.alSockets[x]));
				PyObject* poAttrType = PyList_New(0);
				PyObject* poAttrValue = PyList_New(0);
				for (DWORD x = 0; x < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++x)
				{
					PyList_Append(poAttrType, PyInt_FromLong(logItem.aAttr[x].bType));
					PyList_Append(poAttrValue, PyInt_FromLong(logItem.aAttr[x].sValue));
				}
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ExchangeLogItemAppend", Py_BuildValue("(iiiiOOOi)", logID, logItem.pos, logItem.vnum, logItem.count, poSockets, poAttrType, poAttrValue, logItem.isOwnerItem));

				Py_DECREF(poSockets);
				Py_DECREF(poAttrType);
				Py_DECREF(poAttrValue);
			}
		}
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ExchangeLogRefresh", Py_BuildValue("(i)", logID));
	}
	return true;
}
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
bool CPythonNetworkStream::SendMiniGameCatchKingStart(int ibetNumber)
{
	TPacketCGMiniGameCatchKing kMiniGamePacket;
	kMiniGamePacket.wSize = sizeof(TPacketCGMiniGameCatchKing) + sizeof(TSubPacketCGMiniGameCatchKingStart);
	kMiniGamePacket.bSubheader = SUBHEADER_CG_CATCH_KING_START;

	TSubPacketCGMiniGameCatchKingStart kMiniGameSubPacket;
	kMiniGameSubPacket.betNumber = ibetNumber;

	if (!Send(sizeof(TPacketCGMiniGameCatchKing), &kMiniGamePacket))
		return false;

	if (!Send(sizeof(TSubPacketCGMiniGameCatchKingStart), &kMiniGameSubPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendMiniGameCatchKingDeckCardClick()
{
	TPacketCGMiniGameCatchKing kMiniGamePacket;
	kMiniGamePacket.wSize = sizeof(TPacketCGMiniGameCatchKing);
	kMiniGamePacket.bSubheader = SUBHEADER_CG_CATCH_KING_DECKCARD_CLICK;

	if (!Send(sizeof(TPacketCGMiniGameCatchKing), &kMiniGamePacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendMiniGameCatchKingFieldCardClick(int icardNumber)
{
	TPacketCGMiniGameCatchKing kMiniGamePacket;
	kMiniGamePacket.wSize = sizeof(TPacketCGMiniGameCatchKing) + sizeof(TSubPacketCGMiniGameCatchKingFieldCardClick);
	kMiniGamePacket.bSubheader = SUBHEADER_CG_CATCH_KING_FIELDCARD_CLICK;

	TSubPacketCGMiniGameCatchKingFieldCardClick kMiniGameSubPacket;
	kMiniGameSubPacket.cardNumber = icardNumber;

	if (!Send(sizeof(TPacketCGMiniGameCatchKing), &kMiniGamePacket))
		return false;

	if (!Send(sizeof(TSubPacketCGMiniGameCatchKingFieldCardClick), &kMiniGameSubPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendMiniGameCatchKingReward()
{
	TPacketCGMiniGameCatchKing kMiniGamePacket;
	kMiniGamePacket.wSize = sizeof(TPacketCGMiniGameCatchKing);
	kMiniGamePacket.bSubheader = SUBHEADER_CG_CATCH_KING_REWARD;

	if (!Send(sizeof(TPacketCGMiniGameCatchKing), &kMiniGamePacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::RecvMiniGameCatchKingPacket()
{
	TPacketGCMiniGameCatchKing kMiniGamePacket;
	if (!Recv(sizeof(TPacketGCMiniGameCatchKing), &kMiniGamePacket))
	{
		Tracef("MiniGame Packet Error SubHeader %u\n", kMiniGamePacket.bSubheader);
		return false;
	}

	int iPacketSize = int(kMiniGamePacket.wSize) - sizeof(TPacketGCMiniGameCatchKing);

	switch (kMiniGamePacket.bSubheader)
	{
	case SUBHEADER_GC_CATCH_KING_EVENT_INFO:
	{
		if (iPacketSize > 0)
		{
			TSubPacketGCCatchKingEventInfo kSubPacket;
			assert(iPacketSize % sizeof(TSubPacketGCCatchKingEventInfo) == 0 && "MINI_GAME_RUMI_CARD_REWARD");
			if (!Recv(sizeof(TSubPacketGCCatchKingEventInfo), &kSubPacket))
				return false;

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingEvent", Py_BuildValue("(b)", kSubPacket.bIsEnable));
		}
		else
			TraceError(" RecvMiniGamePacket Error 0x040%uBEBC1", kMiniGamePacket.bSubheader);
		break;
	}

	case SUBHEADER_GC_CATCH_KING_START:
	{
		if (iPacketSize > 0)
		{
			TSubPacketGCCatchKingStart kSubPacket;
			assert(iPacketSize % sizeof(TSubPacketGCCatchKingStart) == 0 && "MINI_GAME_RUMI_CARD_REWARD");
			if (!Recv(sizeof(TSubPacketGCCatchKingStart), &kSubPacket))
				return false;

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingEventStart", Py_BuildValue("(i)", kSubPacket.dwBigScore));
		}
		else
			TraceError(" RecvMiniGamePacket Error 0x040%uBEBC2", kMiniGamePacket.bSubheader);
		break;
	}

	case SUBHEADER_GC_CATCH_KING_SET_CARD:
	{
		if (iPacketSize > 0)
		{
			TSubPacketGCCatchKingSetCard kSubPacket;
			assert(iPacketSize % sizeof(TSubPacketGCCatchKingSetCard) == 0 && "MINI_GAME_RUMI_CARD_REWARD");
			if (!Recv(sizeof(TSubPacketGCCatchKingSetCard), &kSubPacket))
				return false;

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingSetHandCard", Py_BuildValue("(i)", kSubPacket.bCardInHand));
		}
		else
			TraceError(" RecvMiniGamePacket Error 0x040%uBEBC3", kMiniGamePacket.bSubheader);
		break;
	}

	case SUBHEADER_GC_CATCH_KING_RESULT_FIELD:
	{
		if (iPacketSize > 0)
		{
			TSubPacketGCCatchKingResult kSubPacket;
			assert(iPacketSize % sizeof(TSubPacketGCCatchKingResult) == 0 && "MINI_GAME_RUMI_CARD_REWARD");
			if (!Recv(sizeof(TSubPacketGCCatchKingResult), &kSubPacket))
				return false;

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingResultField", Py_BuildValue("(iiiibbbb)",
				kSubPacket.dwPoints, kSubPacket.bRowType, kSubPacket.bCardPos, kSubPacket.bCardValue,
				kSubPacket.bKeepFieldCard, kSubPacket.bDestroyHandCard, kSubPacket.bGetReward, kSubPacket.bIsFiveNearBy));
		}
		else
			TraceError(" RecvMiniGamePacket Error 0x040%uBEBC4", kMiniGamePacket.bSubheader);
		break;
	}

	case SUBHEADER_GC_CATCH_KING_SET_END_CARD:
	{
		if (iPacketSize > 0)
		{
			TSubPacketGCCatchKingSetEndCard kSubPacket;
			assert(iPacketSize % sizeof(TSubPacketGCCatchKingSetEndCard) == 0 && "MINI_GAME_RUMI_CARD_REWARD");
			if (!Recv(sizeof(TSubPacketGCCatchKingSetEndCard), &kSubPacket))
				return false;

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingSetEndCard", Py_BuildValue("(ii)", kSubPacket.bCardPos, kSubPacket.bCardValue));
		}
		else
			TraceError(" RecvMiniGamePacket Error 0x040%uBEBC5", kMiniGamePacket.bSubheader);
		break;
	}

	case SUBHEADER_GC_CATCH_KING_REWARD:
	{
		if (iPacketSize > 0)
		{
			TSubPacketGCCatchKingReward kSubPacket;
			assert(iPacketSize % sizeof(TSubPacketGCCatchKingReward) == 0 && "MINI_GAME_RUMI_CARD_REWARD");
			if (!Recv(sizeof(TSubPacketGCCatchKingReward), &kSubPacket))
				return false;

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingReward", Py_BuildValue("(i)", kSubPacket.bReturnCode));
		}
		else
			TraceError(" RecvMiniGamePacket Error 0x040%uBEBC6", kMiniGamePacket.bSubheader);
		break;
	}
	}

	return true;
}

bool CPythonNetworkStream::SendOpenCatchKing()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartCatchKing", Py_BuildValue("()"));
	return true;
}
#endif

#ifdef __BUFFI_SUPPORT__
bool CPythonNetworkStream::RecvBuffiSkill()
{
	TPacketGCBuffiSkill p;
	if (!Recv(sizeof(TPacketGCBuffiSkill), &p))
	{
		Tracen("CPythonNetworkStream::RecvBuffiSkill - PACKET READ ERROR");
		return false;
	}
	CInstanceBase* pkTarget = CPythonCharacterManager::Instance().GetInstancePtr(p.dwTargetVID);
	CInstanceBase* pkInstance = CPythonCharacterManager::Instance().GetInstancePtr(p.dwVID);
	if (pkInstance && pkTarget)
	{
		const DWORD dwSkillIndex = p.dwSkillIdx;
		CPythonSkill::TSkillData* pSkillData;
		if (CPythonSkill::Instance().GetSkillData(dwSkillIndex, &pSkillData))
		{
			DWORD value = 0;
			if (p.bLevel > 39)
				value = 3;
			else if (p.bLevel > 30)
				value = 2;
			else if (p.bLevel > 17)
				value = 1;
			//CPythonPlayer::Instance().__ProcessEnemySkillTargetRange(*pkInstance, *pkTarget, *pSkillData, dwSkillIndex);//
			pkInstance->NEW_LookAtDestInstance(*pkTarget);
			if (!pkInstance->NEW_UseSkill(dwSkillIndex, pSkillData->GetSkillMotionIndex(value), 1, false))
				Tracenf("CPythonNetworkStream::RecvBuffiSkill(%d) - pkInstance->NEW_UseSkill - ERROR", dwSkillIndex);
		}
	}
	return true;
}
#endif

#ifdef ENABLE_NINJEYE
bool CPythonNetworkStream::RecvNinjEyeRequest()
{
	TPacketGCNinjEyeRequest request = { 0 };
	if (!Recv(sizeof(request), &request))
	{
		return false;
	}

	TPacketCGNinjEyeResponse response = { 0 };
	response.Header = HEADER_CG_NINJEYE_RESPONSE;
	response.Message = NinjEye::Internals::CallInternalFunction(request.Message);

	if (!Send(sizeof(response), &response))
	{
		return false;
	}

	return SendSequence();
}
#endif

#ifdef USE_CAPTCHA_SYSTEM
bool CPythonNetworkStream::RecvCaptcha()
{
    TPacketGCCaptcha p;
    if (!Recv(sizeof(p), &p))
    {
        return false;
    }

    if (p.code == 0 && p.limit == 0)
    {
        CPythonApplication::Instance().SetWindowInvisible(false);
    }
    else
    {
        if (p.code > 0 && CPythonApplication::Instance().SaveImage("c.bmp", p.code) == true)
        {
            CPythonApplication::Instance().SetWindowInvisible(true);
        }
    }

    PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
                            "BINARY_RecvCaptcha",
                            Py_BuildValue("(ii)",
                            p.code > 0 ? -1 : 0,
                            p.limit));

    return true;
}

bool CPythonNetworkStream::SendCaptcha(uint16_t code)
{
    TPacketCGCaptcha p;
    p.bHeader = HEADER_CG_CAPTCHA;
    p.code = code;

    if (!Send(sizeof(p), &p))
    {
        return false;
    }

    return SendSequence();
}

void CPythonNetworkStream::RecvCloseClient()
{
    SetOffLinePhase();
    Disconnect();

    CPythonApplication::Instance().Exit();
}
#endif
#ifdef ENABLE_AURA_SYSTEM
bool CPythonNetworkStream::RecvAuraPacket()
{
	TPacketGCAura kAuraPacket;
	if (!Recv(sizeof(TPacketGCAura), &kAuraPacket))
	{
		Tracef("Aura Packet Error SubHeader %u\n", kAuraPacket.bSubHeader);
		return false;
	}

	int iPacketSize = int(kAuraPacket.wSize) - sizeof(TPacketGCAura);

	switch (kAuraPacket.bSubHeader)
	{
	case AURA_SUBHEADER_GC_OPEN:
	case AURA_SUBHEADER_GC_CLOSE:
	{
		if (iPacketSize > 0)
		{
			TSubPacketGCAuraOpenClose kSubPacket;
			assert(iPacketSize % sizeof(TSubPacketGCAuraOpenClose) == 0 && "AURA_SUBHEADER_GC_OPENCLOSE");
			if (!Recv(sizeof(TSubPacketGCAuraOpenClose), &kSubPacket))
				return false;

			if (kAuraPacket.bSubHeader == AURA_SUBHEADER_GC_OPEN)
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AuraWindowOpen", Py_BuildValue("(i)", kSubPacket.bAuraWindowType));

			else if (kAuraPacket.bSubHeader == AURA_SUBHEADER_GC_CLOSE)
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AuraWindowClose", Py_BuildValue("()"));
				CPythonPlayer::instance().DelAuraItemData(AURA_SLOT_MAIN);
				CPythonPlayer::instance().DelAuraItemData(AURA_SLOT_SUB);
				CPythonPlayer::instance().DelAuraItemData(AURA_SLOT_RESULT);
			}

			CPythonPlayer::instance().SetAuraRefineWindowOpen(kSubPacket.bAuraWindowType);
		}
		else
			TraceError(" RecvAuraPacket Error 0x04100%u0F", kAuraPacket.bSubHeader);

		break;
	}
	case AURA_SUBHEADER_GC_SET_ITEM:
	{
		if (iPacketSize > 0)
		{
			TSubPacketGCAuraSetItem kSubPacket;
			assert(iPacketSize % sizeof(TSubPacketGCAuraSetItem) == 0 && "AURA_SUBHEADER_GC_SET_ITEM");
			if (!Recv(sizeof(TSubPacketGCAuraSetItem), &kSubPacket))
				return false;

			TItemData kItemData;
			kItemData.vnum = kSubPacket.pItem.vnum;
			kItemData.count = kSubPacket.pItem.count;
			for (int iSocket = 0; iSocket < ITEM_SOCKET_SLOT_MAX_NUM; ++iSocket)
				kItemData.alSockets[iSocket] = kSubPacket.pItem.alSockets[iSocket];
			for (int iAttr = 0; iAttr < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++iAttr)
				kItemData.aAttr[iAttr] = kSubPacket.pItem.aAttr[iAttr];

			if (kSubPacket.Cell.IsValidCell() && !kSubPacket.Cell.IsEquipCell())
				CPythonPlayer::instance().SetActivatedAuraSlot(BYTE(kSubPacket.AuraCell.cell), kSubPacket.Cell);

			CPythonPlayer::instance().SetAuraItemData(BYTE(kSubPacket.AuraCell.cell), kItemData);
		}
		else
			TraceError(" RecvAuraPacket Error 0x040%uBABE", kAuraPacket.bSubHeader);

		break;
	}
	case AURA_SUBHEADER_GC_CLEAR_SLOT:
	{
		if (iPacketSize > 0)
		{
			TSubPacketGCAuraClearSlot kSubPacket;
			assert(iPacketSize % sizeof(TSubPacketGCAuraClearSlot) == 0 && "AURA_SUBHEADER_GC_CLEAR_SLOT");
			if (!Recv(sizeof(TSubPacketGCAuraClearSlot), &kSubPacket))
				return false;

			CPythonPlayer::instance().DelAuraItemData(kSubPacket.bAuraSlotPos);
		}
		else
			TraceError(" RecvAuraPacket Error 0x04FF0%uAA", kAuraPacket.bSubHeader);

		break;
	}
	case AURA_SUBHEADER_GC_CLEAR_ALL:
	{
		CPythonPlayer::instance().DelAuraItemData(AURA_SLOT_MAIN);
		CPythonPlayer::instance().DelAuraItemData(AURA_SLOT_SUB);
		CPythonPlayer::instance().DelAuraItemData(AURA_SLOT_RESULT);
		break;
	}
	case AURA_SUBHEADER_GC_CLEAR_RIGHT:
	{
		if (iPacketSize == 0)
			CPythonPlayer::instance().DelAuraItemData(AURA_SLOT_SUB);
		else
			TraceError("invalid packet size %d", iPacketSize);
		break;
	}
	case AURA_SUBHEADER_GC_REFINE_INFO:
	{
		if (iPacketSize > 0)
		{
			for (size_t i = 0; iPacketSize > 0; ++i)
			{
				assert(iPacketSize % sizeof(TSubPacketGCAuraRefineInfo) == 0 && "AURA_SUBHEADER_GC_REFINE_INFO");
				TSubPacketGCAuraRefineInfo kSubPacket;
				if (!Recv(sizeof(TSubPacketGCAuraRefineInfo), &kSubPacket))
					return false;

				CPythonPlayer::instance().SetAuraRefineInfo(kSubPacket.bAuraRefineInfoType, kSubPacket.bAuraRefineInfoLevel, kSubPacket.bAuraRefineInfoExpPercent);
				iPacketSize -= sizeof(TSubPacketGCAuraRefineInfo);
			}

		}
		else
			TraceError(" RecvAuraPacket Error 0x04000%FF", kAuraPacket.bSubHeader);

		break;
	}
	}

	__RefreshInventoryWindow();

	return true;
}

bool CPythonNetworkStream::SendAuraRefineCheckIn(TItemPos InventoryCell, TItemPos AuraCell, BYTE byAuraRefineWindowType)
{
	__PlayInventoryItemDropSound(InventoryCell);

	TPacketCGAura kAuraPacket;
	kAuraPacket.wSize = sizeof(TPacketCGAura) + sizeof(TSubPacketCGAuraRefineCheckIn);
	kAuraPacket.bSubHeader = AURA_SUBHEADER_CG_REFINE_CHECKIN;

	TSubPacketCGAuraRefineCheckIn kAuraSubPacket;
	kAuraSubPacket.ItemCell = InventoryCell;
	kAuraSubPacket.AuraCell = AuraCell;
	kAuraSubPacket.byAuraRefineWindowType = byAuraRefineWindowType;

	if (!Send(sizeof(TPacketCGAura), &kAuraPacket))
		return false;

	if (!Send(sizeof(TSubPacketCGAuraRefineCheckIn), &kAuraSubPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendAuraRefineCheckOut(TItemPos AuraCell, BYTE byAuraRefineWindowType)
{
	TPacketCGAura kAuraPacket;
	kAuraPacket.wSize = sizeof(TPacketCGAura) + sizeof(TSubPacketCGAuraRefineCheckOut);
	kAuraPacket.bSubHeader = AURA_SUBHEADER_CG_REFINE_CHECKOUT;

	TSubPacketCGAuraRefineCheckOut kAuraSubPacket;
	kAuraSubPacket.AuraCell = AuraCell;
	kAuraSubPacket.byAuraRefineWindowType = byAuraRefineWindowType;

	if (!Send(sizeof(TPacketCGAura), &kAuraPacket))
		return false;

	if (!Send(sizeof(TSubPacketCGAuraRefineCheckOut), &kAuraSubPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendAuraRefineAccept(BYTE byAuraRefineWindowType)
{
	TPacketCGAura kAuraPacket;
	kAuraPacket.wSize = sizeof(TPacketCGAura) + sizeof(TSubPacketCGAuraRefineAccept);
	kAuraPacket.bSubHeader = AURA_SUBHEADER_CG_REFINE_ACCEPT;

	TSubPacketCGAuraRefineAccept kAuraSubPacket;
	kAuraSubPacket.byAuraRefineWindowType = byAuraRefineWindowType;

	if (!Send(sizeof(TPacketCGAura), &kAuraPacket))
		return false;

	if (!Send(sizeof(TSubPacketCGAuraRefineAccept), &kAuraSubPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendAuraRefineCancel()
{
	TPacketCGAura kAuraPacket;
	kAuraPacket.wSize = sizeof(TPacketCGAura);
	kAuraPacket.bSubHeader = AURA_SUBHEADER_CG_REFINE_CANCEL;

	if (!Send(sizeof(TPacketCGAura), &kAuraPacket))
		return false;

	return SendSequence();
}
#endif

#ifdef USE_CURRENCY_EXCHANGE
bool CPythonNetworkStream::SendCurrencyExchange(const long long exchangeAmount, const uint8_t exchangeType)
{
	if (!__CanActMainInstance(true))
	{
		return true;
	}

	if (exchangeType >= SUBHEADER_CURRENCY_EXCHANGE_MAX)
	{
		return true;
	}

	TPacketCGCurrencyExchange p;
	p.bHeader = HEADER_CG_CURRENCY_EXCHANGE;
	p.bSubHeader = exchangeType;
	p.llAmount = exchangeAmount;

	if (!Send(sizeof(TPacketCGCurrencyExchange), &p))
	{
		TraceError("Cannot send: TPacketCGCurrencyExchange.");
		return false;
	}

	return SendSequence();
}
#endif

#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
bool CPythonNetworkStream::RecvHitCountInfo()
{
	TPacketGCHitCountInfo hitCountPacket;
	if (!Recv(sizeof(hitCountPacket), &hitCountPacket))
		return false;

	CPythonPlayer::Instance().SetAccumulateDamage(hitCountPacket.dwVid, hitCountPacket.dwCount);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshAccumulateCount", Py_BuildValue("(i)", hitCountPacket.dwVid));

	return true;
}
#endif

#if defined(__BL_BATTLE_ROYALE__)
bool CPythonNetworkStream::RecvBattleRoyale()
{
	TPacketGCBattleRoyale p;

	if (!Peek(sizeof(p), &p))
		return false;

	if (!Peek(p.size))
		return false;

	Recv(sizeof(p));

	switch (p.subheader)
	{
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_INSERT_APPLICANT:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii)", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_MINI_GAME, p.subheader));
		return true;
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_REMOVE_APPLICANT:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii)", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_MINI_GAME, p.subheader));
		return true;
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_STARTABLE:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii)", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_MINI_GAME, p.subheader));
		return true;
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_ENTRY_CLOSED:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii)", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_MINI_GAME, p.subheader));
		return true;
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_START:
		PythonBattleRoyaleManager::Instance().SetPlaying(true);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii)", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_MINI_GAME, p.subheader));
		return true;
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_EXIT:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii)", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_MINI_GAME, p.subheader));
		PythonBattleRoyaleManager::Instance().BATTLE_ROYALE_Unknown_6();
		return true;
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_OPEN_KILL_INFO:
	{
		bool bOpen;
		if (!Recv(sizeof(bOpen), &bOpen))
			return false;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii(b))", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_BATTLE_ROYALE_INTERFACE, p.subheader, bOpen));
		return true;
	}
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_KILL_INFO:
	{
		uint32_t uAliveCount;
		if (!Recv(sizeof(uAliveCount), &uAliveCount))
			return false;

		uint32_t uKillCount;
		if (!Recv(sizeof(uKillCount), &uKillCount))
			return false;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii(ii))", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_BATTLE_ROYALE_INTERFACE, p.subheader, uAliveCount, uKillCount));
		return true;
	}
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_DEAD:
	{
		uint8_t bPersonalRank;
		if (!Recv(sizeof(bPersonalRank), &bPersonalRank))
			return false;
		
		char szKillerName[CHARACTER_NAME_MAX_LEN + 1];
		if (!Recv(sizeof(szKillerName), &szKillerName))
			return false;
		
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii(si))", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_BATTLE_ROYALE_INTERFACE, p.subheader, szKillerName, bPersonalRank));
		return true;
	}
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_WHITE_CIRCLE:
	{
		int iX;
		if (!Recv(sizeof(iX), &iX))
			return false;

		int iY;
		if (!Recv(sizeof(iY), &iY))
			return false;

		int iRadius;
		if (!Recv(sizeof(iRadius), &iRadius))
			return false;

		PythonBattleRoyaleManager::Instance().SetWhiteCirclePos(iX, iY, iRadius);
		return true;
	}
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_RED_CIRCLE:
	{
		int iX;
		if (!Recv(sizeof(iX), &iX))
			return false;

		int iY;
		if (!Recv(sizeof(iY), &iY))
			return false;

		int iRadius;
		if (!Recv(sizeof(iRadius), &iRadius))
			return false;

		PythonBattleRoyaleManager::Instance().SetRedCirclePos(iX, iY, iRadius);
		return true;
	}
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_SHRINK:
	{
		int iTime;
		if (!Recv(sizeof(iTime), &iTime))
			return false;

		PythonBattleRoyaleManager::Instance().Shrink(iTime);
		return true;
	}
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_UNKNOWN:
		PythonBattleRoyaleManager::Instance().BATTLE_ROYALE_Unknown_5();
		return true;
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_WINNER:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii)", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_BATTLE_ROYALE_INTERFACE, p.subheader));
		return true;
	case PythonBattleRoyaleManager::BATTLE_ROYALE_GC_HAS_REWARD:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BattleRoyaleProcess", Py_BuildValue("(ii)", PythonBattleRoyaleManager::BATTLE_ROYALE_GC_PROCESS_CATEGORY_MINI_GAME, p.subheader));
		return true;
	}

	return false;
}

bool CPythonNetworkStream::SendBattleRoyaleApplication()
{
	TPacketCGBattleRoyale p;
	p.header = HEADER_CG_BATTLE_ROYALE;
	p.subheader = PythonBattleRoyaleManager::BATTLE_ROYALE_CG_APPLICATION;
	p.arg = false;

	if (!Send(sizeof(p), &p))
	{
		Tracen("CPythonNetworkStream::SendBattleRoyaleApplication Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendBattleRoyaleApplicationCancel()
{
	TPacketCGBattleRoyale p;
	p.header = HEADER_CG_BATTLE_ROYALE;
	p.subheader = PythonBattleRoyaleManager::BATTLE_ROYALE_CG_APPLICATION_CANCEL;
	p.arg = false;

	if (!Send(sizeof(p), &p))
	{
		Tracen("CPythonNetworkStream::SendBattleRoyaleApplicationCancel Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendBattleRoyaleExit()
{
	TPacketCGBattleRoyale p;
	p.header = HEADER_CG_BATTLE_ROYALE;
	p.subheader = PythonBattleRoyaleManager::BATTLE_ROYALE_CG_EXIT;
	p.arg = false;

	if (!Send(sizeof(p), &p))
	{
		Tracen("CPythonNetworkStream::SendBattleRoyaleExit Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendBattleRoyaleRanking()
{
	TPacketCGBattleRoyale p;
	p.header = HEADER_CG_BATTLE_ROYALE;
	p.subheader = PythonBattleRoyaleManager::BATTLE_ROYALE_CG_RANKING;
	p.arg = false;

	if (!Send(sizeof(p), &p))
	{
		Tracen("CPythonNetworkStream::SendBattleRoyaleRanking Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendBattleRoyaleStart(bool bUseSpecialItem)
{
	TPacketCGBattleRoyale p;
	p.header = HEADER_CG_BATTLE_ROYALE;
	p.subheader = PythonBattleRoyaleManager::BATTLE_ROYALE_CG_START;
	p.arg = bUseSpecialItem;

	if (!Send(sizeof(p), &p))
	{
		Tracen("CPythonNetworkStream::SendBattleRoyaleStart Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendBattleRoyaleClaimReward()
{
	TPacketCGBattleRoyale p;
	p.header = HEADER_CG_BATTLE_ROYALE;
	p.subheader = PythonBattleRoyaleManager::BATTLE_ROYALE_CG_CLAIM_REWARD;
	p.arg = 0;

	if (!Send(sizeof(p), &p))
	{
		TraceError("CPythonNetworkStream::SendBattleRoyaleClaimReward Error");
		return false;
	}

	return SendSequence();
}
#endif

#if defined(ENABLE_VOICE_CHAT)
bool CPythonNetworkStream::RecvVoiceChatPacket()
{
	static auto& rkVoice = VoiceChat::Instance();
	TVoiceChatPacket packet{};

	if (!Recv(sizeof(packet), &packet))
	{
		TraceError("FAILED TO RECEIVE VOICE CHAT PACKET");
		return false;
	}

	const uint32_t bufSize = sizeof(int16_t) * std::max<uint32_t>(4800, rkVoice.GetSampleRate()) * std::max<uint8_t>(8, rkVoice.GetChannelCount());
	static std::vector<char> buf(bufSize, 0/*silence*/); // Default to enough for 48k frequency on 8 channels
	if (buf.size() < bufSize)
		buf.resize(bufSize); // Tf are you doing to fill this?

	if (!Recv(packet.dataSize, buf.data()))
	{
		TraceError("FAILED TO RECEIVE AUDIO DATA! size %u, recvBufSize %d(out %d, in %d, max %d)",
			packet.dataSize, GetRecvBufferSize(), m_recvBufOutputPos, m_recvBufInputPos, m_recvBufSize);
		return false;
	}

	if (auto inst = CPythonCharacterManager::Instance().GetInstancePtr(packet.vid))
	{
		inst->AttachSpecialEffect(CInstanceBase::EFFECT_VOICE_CHAT);
	}

	rkVoice.OnRecvVoiceChatPacket(packet, buf.data());
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OnVoice",
		Py_BuildValue("(si)", packet.name, packet.type));

	return true;
}
#endif

#ifdef USE_CRYSTAL_SYSTEM
bool CPythonNetworkStream::SendShardCraft(const uint8_t bSubHeader,
					int32_t iSourceSlot/* = -1*/,
					int32_t iTargetSlot/* = -1*/)
{
	switch (bSubHeader)
	{
		case SUBHEADER_SHARD_CRAFT_CLOSE:
		case SUBHEADER_SHARD_CRAFT_REFINE:
		{
			TPacketCGShardCraft p;
			p.bHeader = HEADER_CG_SHARD_CRAFT;
			p.bSubHeader = bSubHeader;

			if (!Send(sizeof(p), &p))
			{
				TraceError("SendShardCraft >> cannot be sended: subHeader (%u).", bSubHeader);
				return false;
			}

			return SendSequence();
		}
		case SUBHEADER_SHARD_CRAFT_ADD:
		{
			TPacketCGShardCraft p;
			p.bHeader = HEADER_CG_SHARD_CRAFT;
			p.bSubHeader = bSubHeader;

			TShardCraftAdd p2;
			p2.iSourceSlot = iSourceSlot;
			p2.iTargetSlot = iTargetSlot;

			if (!Send(sizeof(p), &p))
			{
				TraceError("SendShardCraft >> cannot be sended: subHeader (%u) (1).", bSubHeader);
				return false;
			}

			if (!Send(sizeof(p2), &p2))
			{
				TraceError("SendShardCraft >> cannot be sended: subHeader (%u) (2).", bSubHeader);
				return false;
			}

			return SendSequence();
		}
		case SUBHEADER_SHARD_CRAFT_OUT:
		{
			TPacketCGShardCraft p;
			p.bHeader = HEADER_CG_SHARD_CRAFT;
			p.bSubHeader = bSubHeader;

			TShardCraftOut p2;
			p2.iTargetSlot = iTargetSlot;

			if (!Send(sizeof(p), &p))
			{
				TraceError("SendShardCraft >> cannot be sended: subHeader (%u) (1).", bSubHeader);
				return false;
			}

			if (!Send(sizeof(p2), &p2))
			{
				TraceError("SendShardCraft >> cannot be sended: subHeader (%u) (2).", bSubHeader);
				return false;
			}

			return SendSequence();
		}
		default:
		{
			TraceError("SendShardCraft >> invalid subHeader (%u).", bSubHeader);
			return false;
		}
	}
}

bool CPythonNetworkStream::RecvShardCraft()
{
	TPacketGCShardCraft p;

	if (!Recv(sizeof(p), &p))
	{
		TraceError("RecvShardCraft >> cannot recv TPacketGCShardCraft.");
		return false;
	}

	switch (p.bSubHeader)
	{
		case SUBHEADER_SHARD_CRAFT_OPEN:
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
									"BINARY_OpenShardCraft",
									Py_BuildValue("()"));

			break;
		}
		case SUBHEADER_SHARD_CRAFT_CLOSE:
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
									"BINARY_CloseShardCraft",
									Py_BuildValue("()"));

			break;
		}
		case SUBHEADER_SHARD_CRAFT_ADD:
		{
			WORD wSize = p.wSize;
			wSize -= sizeof(p);

			if (wSize < sizeof(TShardCraftAdd))
			{
				TraceError("RecvShardCraft >> invalid size: left %u, total %u.", p.wSize, wSize);
				return false;
			}

			TShardCraftAdd p2;

			if (!Recv(sizeof(p2), &p2))
			{
				TraceError("RecvShardCraft >> cannot recv TShardCraftAdd.");
				return false;
			}

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
									"BINARY_AddShardCraft",
									Py_BuildValue("(ii)",
									p2.iSourceSlot,
									p2.iTargetSlot));

			break;
		}
		case SUBHEADER_SHARD_CRAFT_OUT:
		{
			WORD wSize = p.wSize;
			wSize -= sizeof(p);

			if (wSize < sizeof(TShardCraftOut))
			{
				TraceError("RecvShardCraft >> invalid size: left %u, total %u.", p.wSize, wSize);
				return false;
			}

			TShardCraftOut p2;

			if (!Recv(sizeof(p2), &p2))
			{
				TraceError("RecvShardCraft >> cannot recv TShardCraftOut.");
				return false;
			}

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
									"BINARY_OutShardCraft",
									Py_BuildValue("(i)",
									p2.iTargetSlot));

			break;
		}
		case SUBHEADER_SHARD_CRAFT_OUT_ALL:
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
									"BINARY_OutAllShardCraft",
									Py_BuildValue("()"));

			break;
		}
		default:
		{
			TraceError("RecvShardCraft >> invalid subHeader %u.", p.bSubHeader);
			return false;
		}
	}

	return true;
}
#endif
