#include "stdafx.h"
#include "constants.h"
#include "config.h"
#include "utils.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "item.h"
#ifdef ENABLE_IKASHOP_RENEWAL
#include "ikarus_shop.h"
#include "ikarus_shop_manager.h"
#endif
#include "item_manager.h"
#include "packet.h"
#include "protocol.h"
#include "mob_manager.h"
#include "shop_manager.h"
#include "sectree_manager.h"
#include "skill.h"
#include "questmanager.h"
#include "p2p.h"
#include "guild.h"
#include "guild_manager.h"
#include "start_position.h"
#include "party.h"
#include "refine.h"
#include "banword.h"
#include "priv_manager.h"
#include "db.h"
#include "building.h"
#include "login_sim.h"
#include "wedding.h"
#include "login_data.h"
#include "unique_item.h"

#ifdef ENABLE_ULTIMATE_REGEN
#include "new_mob_timer.h"
#endif
#ifdef __FAKE_PLAYER__
	#include "FakePlayerManager.h"
#endif
#include "monarch.h"
#include "affect.h"
#include "castle.h"
#include "block_country.h"
#include "motion.h"

#include "dev_log.h"

#include "log.h"

#include "horsename_manager.h"
#include "pcbang.h"
#include "gm.h"
#include "map_location.h"

#include "DragonSoul.h"

#include "shutdown_manager.h"
#include "../common/CommonDefines.h"
#include "Anticheat_Manager.h"
#ifdef USE_DUNGEON_COOLDOWN_MANAGER
#include "dungeon_cooldown_mgr.hpp"
#endif
#ifdef USE_DAY_CHANGE_MGR
#include "day_change_mgr.hpp"
#endif

#define MAPNAME_DEFAULT	"none"

bool GetServerLocation(TAccountTable & rTab, BYTE bEmpire)
{
	bool bFound = false;

	for (int i = 0; i < PLAYER_PER_ACCOUNT; ++i)
	{
		if (0 == rTab.players[i].dwID)
			continue;

		bFound = true;
		long lIndex = 0;

		if (!CMapLocation::instance().Get(rTab.players[i].x,
					rTab.players[i].y,
					lIndex,
					rTab.players[i].lAddr,
					rTab.players[i].wPort))
		{
			sys_err("location error name %s mapindex %d %d x %d empire %d",
					rTab.players[i].szName, lIndex, rTab.players[i].x, rTab.players[i].y, rTab.bEmpire);

			rTab.players[i].x = EMPIRE_START_X(rTab.bEmpire);
			rTab.players[i].y = EMPIRE_START_Y(rTab.bEmpire);

			lIndex = 0;

			if (!CMapLocation::instance().Get(rTab.players[i].x, rTab.players[i].y, lIndex, rTab.players[i].lAddr, rTab.players[i].wPort))
			{
				sys_err("cannot find server for mapindex %d %d x %d (name %s)",
						lIndex,
						rTab.players[i].x,
						rTab.players[i].y,
						rTab.players[i].szName);

				continue;
			}
		}

		struct in_addr in;
		in.s_addr = rTab.players[i].lAddr;
		sys_log(0, "success to %s:%d", inet_ntoa(in), rTab.players[i].wPort);
	}

	return bFound;
}

extern std::map<DWORD, CLoginSim *> g_sim;
extern std::map<DWORD, CLoginSim *> g_simByPID;

void CInputDB::LoginSuccess(DWORD dwHandle, const char *data)
{
	sys_log(0, "LoginSuccess");

	TAccountTable * pTab = (TAccountTable *) data;

	itertype(g_sim) it = g_sim.find(pTab->id);
	if (g_sim.end() != it)
	{
		sys_log(0, "CInputDB::LoginSuccess - already exist sim [%s]", pTab->login);
		it->second->SendLoad();
		return;
	}

	LPDESC d = DESC_MANAGER::instance().FindByHandle(dwHandle);

	if (!d)
	{
		sys_log(0, "CInputDB::LoginSuccess - cannot find handle [%s]", pTab->login);

		TLogoutPacket pack;

		strlcpy(pack.login, pTab->login, sizeof(pack.login));
		db_clientdesc->DBPacket(HEADER_GD_LOGOUT, dwHandle, &pack, sizeof(pack));
		return;
	}

	if (strcmp(pTab->status, "OK")) // OK°¡ ¾Æ´Ï¸é
	{
		sys_log(0, "CInputDB::LoginSuccess - status[%s] is not OK [%s]", pTab->status, pTab->login);

		TLogoutPacket pack;

		strlcpy(pack.login, pTab->login, sizeof(pack.login));
		db_clientdesc->DBPacket(HEADER_GD_LOGOUT, dwHandle, &pack, sizeof(pack));

		LoginFailure(d, pTab->status);
		return;
	}

	for (int i = 0; i != PLAYER_PER_ACCOUNT; ++i)
	{
		TSimplePlayer& player = pTab->players[i];
		sys_log(0, "\tplayer(%s).job(%d)", player.szName, player.byJob);
	}

	bool bFound = GetServerLocation(*pTab, pTab->bEmpire);

	d->BindAccountTable(pTab);


	if (!bFound) // Ä³¸¯ÅÍ°¡ ¾øÀ¸¸é ·£´ýÇÑ Á¦±¹À¸·Î º¸³½´Ù.. -_-
	{
		TPacketGCEmpire pe;
		pe.bHeader = HEADER_GC_EMPIRE;
		pe.bEmpire = number(1, 3);
		d->Packet(&pe, sizeof(pe));
	}
	else
	{
		TPacketGCEmpire pe;
		pe.bHeader = HEADER_GC_EMPIRE;
		pe.bEmpire = d->GetEmpire();
		d->Packet(&pe, sizeof(pe));
	}

	d->SetPhase(PHASE_SELECT);
	d->SendLoginSuccessPacket();

	// __SHUTDOWN::Shutdown Register
	CShutdownManager::Instance().AddDesc(d);

	sys_log(0, "InputDB::login_success: %s", pTab->login);
}

void CInputDB::PlayerCreateFailure(LPDESC d, BYTE bType)
{
	if (!d)
		return;

	TPacketGCCreateFailure pack;

	pack.header	= HEADER_GC_CHARACTER_CREATE_FAILURE;
	pack.bType	= bType;

	d->Packet(&pack, sizeof(pack));
}

void CInputDB::PlayerCreateSuccess(LPDESC d, const char * data)
{
	if (!d)
		return;

	TPacketDGCreateSuccess * pPacketDB = (TPacketDGCreateSuccess *) data;

	if (pPacketDB->bAccountCharacterIndex >= PLAYER_PER_ACCOUNT)
	{
		d->Packet(encode_byte(HEADER_GC_CHARACTER_CREATE_FAILURE), 1);
		return;
	}

	long lIndex = 0;

	if (!CMapLocation::instance().Get(pPacketDB->player.x,
				pPacketDB->player.y,
				lIndex,
				pPacketDB->player.lAddr,
				pPacketDB->player.wPort))
	{
		sys_err("InputDB::PlayerCreateSuccess: cannot find server for mapindex %d %d x %d (name %s)",
				lIndex,
				pPacketDB->player.x,
				pPacketDB->player.y,
				pPacketDB->player.szName);
	}

	TAccountTable & r_Tab = d->GetAccountTable();
	r_Tab.players[pPacketDB->bAccountCharacterIndex] = pPacketDB->player;

	TPacketGCPlayerCreateSuccess pack;

	pack.header = HEADER_GC_CHARACTER_CREATE_SUCCESS;
	pack.bAccountCharacterIndex = pPacketDB->bAccountCharacterIndex;
	pack.player = pPacketDB->player;

	d->Packet(&pack, sizeof(TPacketGCPlayerCreateSuccess));

#ifdef USE_REWARD_AT_START
	TPlayerItem t = {};
	t.owner = r_Tab.players[pPacketDB->bAccountCharacterIndex].dwID;
#ifdef ENABLE_YOHARA_SYSTEM
	memset(&t.aApplyRandom, 0, sizeof(t.aApplyRandom));
#endif
#ifdef __SOULBINDING_SYSTEM__
	t.bind = 0;
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	snprintf(t.name, sizeof(t.name), "^");
#endif

	struct SInitialItem
	{
		BYTE					bWindow;
		WORD					wPos;
		DWORD					dwVnum;
		DWORD					dwCount;
		TPlayerItemAttribute	aAttr[ITEM_ATTRIBUTE_MAX_NUM];
		long					alSockets[ITEM_SOCKET_MAX_NUM];
	};

	static SInitialItem initialItems[MAIN_RACE_MAX_NUM][8] = 
	{
		/* MAIN_RACE_WARRIOR_M */
		{
			// Armor
			{
				EQUIPMENT, WEAR_BODY, 31259, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_ATT_GRADE_BONUS, 50},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Weapon
			{
				EQUIPMENT, WEAR_WEAPON, 3019, 1,
				{
					{APPLY_STR, 12},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					1,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Helm
			{
				EQUIPMENT, WEAR_HEAD, 31263, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shield
			{
				EQUIPMENT, WEAR_SHIELD, 31267, 1,
				{
					{APPLY_STR, 12},
					{APPLY_IMMUNE_STUN, 1},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Bracelet
			{
				EQUIPMENT, WEAR_WRIST, 31269, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_PENETRATE_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shoes
			{
				EQUIPMENT, WEAR_FOOTS, 31271, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Necklace
			{
				EQUIPMENT, WEAR_NECK, 31268, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Earrings
			{
				EQUIPMENT, WEAR_EAR, 31270, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_ITEM_DROP_BONUS, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
		},
		/* MAIN_RACE_ASSASSIN_W */
		{
			// Armor
			{
				EQUIPMENT, WEAR_BODY, 31260, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_ATT_GRADE_BONUS, 50},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Weapon
			{
				EQUIPMENT, WEAR_WEAPON, 29, 1,
				{
					{APPLY_DEX, 12},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					1,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Helm
			{
				EQUIPMENT, WEAR_HEAD, 31264, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shield
			{
				EQUIPMENT, WEAR_SHIELD, 31267, 1,
				{
					{APPLY_DEX, 12},
					{APPLY_IMMUNE_STUN, 1},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Bracelet
			{
				EQUIPMENT, WEAR_WRIST, 31269, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_PENETRATE_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shoes
			{
				EQUIPMENT, WEAR_FOOTS, 31271, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Necklace
			{
				EQUIPMENT, WEAR_NECK, 31268, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Earrings
			{
				EQUIPMENT, WEAR_EAR, 31270, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_ITEM_DROP_BONUS, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
		},
		/* MAIN_RACE_SURA_M */
		{
			// Armor
			{
				EQUIPMENT, WEAR_BODY, 31261, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_ATT_GRADE_BONUS, 50},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Weapon
			{
				EQUIPMENT, WEAR_WEAPON, 29, 1,
				{
					{APPLY_INT, 12},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					1,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Helm
			{
				EQUIPMENT, WEAR_HEAD, 31265, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shield
			{
				EQUIPMENT, WEAR_SHIELD, 31267, 1,
				{
					{APPLY_INT, 12},
					{APPLY_IMMUNE_STUN, 1},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Bracelet
			{
				EQUIPMENT, WEAR_WRIST, 31269, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_PENETRATE_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shoes
			{
				EQUIPMENT, WEAR_FOOTS, 31271, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Necklace
			{
				EQUIPMENT, WEAR_NECK, 31268, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Earrings
			{
				EQUIPMENT, WEAR_EAR, 31270, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_ITEM_DROP_BONUS, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
		},
		/* MAIN_RACE_SHAMAN_W */
		{
			// Armor
			{
				EQUIPMENT, WEAR_BODY, 31262, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_ATT_GRADE_BONUS, 50},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Weapon
			{
				EQUIPMENT, WEAR_WEAPON, 7029, 1,
				{
					{APPLY_INT, 12},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					1,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Helm
			{
				EQUIPMENT, WEAR_HEAD, 31266, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shield
			{
				EQUIPMENT, WEAR_SHIELD, 31267, 1,
				{
					{APPLY_INT, 12},
					{APPLY_IMMUNE_STUN, 1},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Bracelet
			{
				EQUIPMENT, WEAR_WRIST, 31269, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_PENETRATE_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shoes
			{
				EQUIPMENT, WEAR_FOOTS, 31271, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Necklace
			{
				EQUIPMENT, WEAR_NECK, 31268, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Earrings
			{
				EQUIPMENT, WEAR_EAR, 31270, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_ITEM_DROP_BONUS, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
		},
		/* MAIN_RACE_WARRIOR_W */
		{
			// Armor
			{
				EQUIPMENT, WEAR_BODY, 31259, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_ATT_GRADE_BONUS, 50},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Weapon
			{
				EQUIPMENT, WEAR_WEAPON, 29, 1,
				{
					{APPLY_STR, 12},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					1,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Helm
			{
				EQUIPMENT, WEAR_HEAD, 31263, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shield
			{
				EQUIPMENT, WEAR_SHIELD, 31267, 1,
				{
					{APPLY_STR, 12},
					{APPLY_IMMUNE_STUN, 1},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Bracelet
			{
				EQUIPMENT, WEAR_WRIST, 31269, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_PENETRATE_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shoes
			{
				EQUIPMENT, WEAR_FOOTS, 31271, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Necklace
			{
				EQUIPMENT, WEAR_NECK, 31268, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Earrings
			{
				EQUIPMENT, WEAR_EAR, 31270, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_ITEM_DROP_BONUS, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
		},
		/* MAIN_RACE_ASSASSIN_M */
		{
			// Armor
			{
				EQUIPMENT, WEAR_BODY, 31260, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_ATT_GRADE_BONUS, 50},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Weapon
			{
				EQUIPMENT, WEAR_WEAPON, 29, 1,
				{
					{APPLY_DEX, 12},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					1,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Helm
			{
				EQUIPMENT, WEAR_HEAD, 31264, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shield
			{
				EQUIPMENT, WEAR_SHIELD, 31267, 1,
				{
					{APPLY_DEX, 12},
					{APPLY_IMMUNE_STUN, 1},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Bracelet
			{
				EQUIPMENT, WEAR_WRIST, 31269, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_PENETRATE_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shoes
			{
				EQUIPMENT, WEAR_FOOTS, 31271, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Necklace
			{
				EQUIPMENT, WEAR_NECK, 31268, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Earrings
			{
				EQUIPMENT, WEAR_EAR, 31270, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_ITEM_DROP_BONUS, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
		},
		/* MAIN_RACE_SURA_W */
		{
			// Armor
			{
				EQUIPMENT, WEAR_BODY, 31261, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_ATT_GRADE_BONUS, 50},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Weapon
			{
				EQUIPMENT, WEAR_WEAPON, 29, 1,
				{
					{APPLY_INT, 12},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					1,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Helm
			{
				EQUIPMENT, WEAR_HEAD, 31265, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shield
			{
				EQUIPMENT, WEAR_SHIELD, 31267, 1,
				{
					{APPLY_INT, 12},
					{APPLY_IMMUNE_STUN, 1},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Bracelet
			{
				EQUIPMENT, WEAR_WRIST, 31269, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_PENETRATE_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shoes
			{
				EQUIPMENT, WEAR_FOOTS, 31271, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Necklace
			{
				EQUIPMENT, WEAR_NECK, 31268, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Earrings
			{
				EQUIPMENT, WEAR_EAR, 31270, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_ITEM_DROP_BONUS, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
		},
		/* MAIN_RACE_SHAMAN_M */
		{
			// Armor
			{
				EQUIPMENT, WEAR_BODY, 31262, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_ATT_GRADE_BONUS, 50},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Weapon
			{
				EQUIPMENT, WEAR_WEAPON, 7029, 1,
				{
					{APPLY_INT, 12},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					1,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Helm
			{
				EQUIPMENT, WEAR_HEAD, 31266, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shield
			{
				EQUIPMENT, WEAR_SHIELD, 31267, 1,
				{
					{APPLY_INT, 12},
					{APPLY_IMMUNE_STUN, 1},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Bracelet
			{
				EQUIPMENT, WEAR_WRIST, 31269, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_PENETRATE_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shoes
			{
				EQUIPMENT, WEAR_FOOTS, 31271, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Necklace
			{
				EQUIPMENT, WEAR_NECK, 31268, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Earrings
			{
				EQUIPMENT, WEAR_EAR, 31270, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_ITEM_DROP_BONUS, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
		},
#ifdef ENABLE_WOLFMAN_CHARACTER
		/* MAIN_RACE_WOLFMAN_M */
		{
			// Armor
			{
				EQUIPMENT, WEAR_BODY, 21009, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_ATT_GRADE_BONUS, 50},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Weapon
			{
				EQUIPMENT, WEAR_WEAPON, 6009, 1,
				{
					{APPLY_INT, 12},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					1,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Helm
			{
				EQUIPMENT, WEAR_HEAD, 21509, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shield
			{
				EQUIPMENT, WEAR_SHIELD, 31267, 1,
				{
					{APPLY_DEX, 12},
					{APPLY_IMMUNE_STUN, 1},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Bracelet
			{
				EQUIPMENT, WEAR_WRIST, 31269, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_PENETRATE_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Shoes
			{
				EQUIPMENT, WEAR_FOOTS, 31271, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Necklace
			{
				EQUIPMENT, WEAR_NECK, 31268, 1,
				{
					{APPLY_MAX_HP, 2000},
					{APPLY_CRITICAL_PCT, 10},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
			// Earrings
			{
				EQUIPMENT, WEAR_EAR, 31270, 1,
				{
					{APPLY_ATTBONUS_ANIMAL, 20},
					{APPLY_ATTBONUS_ORC, 20},
					{APPLY_ITEM_DROP_BONUS, 20},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0},
					{APPLY_NONE, 0}
				},
				{
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					0
				},
			},
		},
#endif
	};

	const BYTE byJob = pPacketDB->player.byJob;

	for (auto i = 0; i < _countof(initialItems[byJob]); i++)
	{
		t.id = ITEM_MANAGER::instance().GetNewID();
		t.window = initialItems[byJob][i].bWindow;
		t.pos = initialItems[byJob][i].wPos;
		t.vnum = initialItems[byJob][i].dwVnum;
		t.count = initialItems[byJob][i].dwCount;

		for (auto j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
		{
			t.aAttr[j].bType = initialItems[byJob][i].aAttr[j].bType;
			t.aAttr[j].sValue = initialItems[byJob][i].aAttr[j].sValue;
		}

		for (uint8_t j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
		{
			t.alSockets[j] = initialItems[byJob][i].alSockets[j];
		}

		db_clientdesc->DBPacketHeader(HEADER_GD_ITEM_SAVE, 0, sizeof(TPlayerItem));
		db_clientdesc->Packet(&t, sizeof(TPlayerItem));
	}
#endif

	LogManager::instance().CharLog(pack.player.dwID, 0, 0, 0, "CREATE PLAYER", "", d->GetHostName());
}

void CInputDB::PlayerDeleteSuccess(LPDESC d, const char * data)
{
	if (!d)
		return;

	BYTE account_index;
	account_index = decode_byte(data);
	d->BufferedPacket(encode_byte(HEADER_GC_CHARACTER_DELETE_SUCCESS),	1);
	d->Packet(encode_byte(account_index),			1);

	d->GetAccountTable().players[account_index].dwID = 0;
}

void CInputDB::PlayerDeleteFail(LPDESC d)
{
	if (!d)
		return;

	d->Packet(encode_byte(HEADER_GC_CHARACTER_DELETE_WRONG_SOCIAL_ID),	1);
	//d->Packet(encode_byte(account_index),			1);

	//d->GetAccountTable().players[account_index].dwID = 0;
}

void CInputDB::ChangeName(LPDESC d, const char * data)
{
	if (!d)
		return;

	TPacketDGChangeName * p = (TPacketDGChangeName *) data;

	TAccountTable & r = d->GetAccountTable();

	if (!r.id)
		return;

	for (size_t i = 0; i < PLAYER_PER_ACCOUNT; ++i)
		if (r.players[i].dwID == p->pid)
		{
			strlcpy(r.players[i].szName, p->name, sizeof(r.players[i].szName));
			r.players[i].bChangeName = 0;

			TPacketGCChangeName pgc;

			pgc.header = HEADER_GC_CHANGE_NAME;
			pgc.pid = p->pid;
			strlcpy(pgc.name, p->name, sizeof(pgc.name));

			d->Packet(&pgc, sizeof(TPacketGCChangeName));
			break;
		}
}

#define ENABLE_GOHOME_IF_MAP_NOT_EXIST
void CInputDB::PlayerLoad(LPDESC d, const char * data)
{
	TPlayerTable * pTab = (TPlayerTable *) data;

	if (!d)
		return;

	long lMapIndex = pTab->lMapIndex;
	PIXEL_POSITION pos;

	if (lMapIndex == 0)
	{
		lMapIndex = SECTREE_MANAGER::instance().GetMapIndex(pTab->x, pTab->y);

		if (lMapIndex == 0) // ÁÂÇ¥¸¦ Ã£À» ¼ö ¾ø´Ù.
		{
			lMapIndex = EMPIRE_START_MAP(d->GetAccountTable().bEmpire);
			pos.x = EMPIRE_START_X(d->GetAccountTable().bEmpire);
			pos.y = EMPIRE_START_Y(d->GetAccountTable().bEmpire);
		}
		else
		{
			pos.x = pTab->x;
			pos.y = pTab->y;
		}
	}
	pTab->lMapIndex = lMapIndex;

	// Private ¸Ê¿¡ ÀÖ¾ú´Âµ¥, Private ¸ÊÀÌ »ç¶óÁø »óÅÂ¶ó¸é Ãâ±¸·Î µ¹¾Æ°¡¾ß ÇÑ´Ù.
	// ----
	// ±Ùµ¥ Ãâ±¸·Î µ¹¾Æ°¡¾ß ÇÑ´Ù¸é¼­... ¿Ö Ãâ±¸°¡ ¾Æ´Ï¶ó private map »ó¿¡ ´ëÀÀµÇ´Â pulic mapÀÇ À§Ä¡¸¦ Ã£³Ä°í...
	// ¿ª»ç¸¦ ¸ð¸£´Ï... ¶Ç ÇÏµåÄÚµù ÇÑ´Ù.
	// ¾Æ±Íµ¿±¼ÀÌ¸é, Ãâ±¸·Î...
	// by rtsummit
#ifdef __BL_BATTLE_ROYALE__
	if (lMapIndex == BattleRoyale::Settings::BR_MAP_INDEX && (!BattleRoyale::Instance().IsBattleChannel() || !BattleRoyale::Instance().IsFighter(pTab->id)))
	{
		lMapIndex = EMPIRE_START_MAP(d->GetAccountTable().bEmpire);
		pos.x = EMPIRE_START_X(d->GetAccountTable().bEmpire);
		pos.y = EMPIRE_START_Y(d->GetAccountTable().bEmpire);
	}
	else
#endif
	if (!SECTREE_MANAGER::instance().GetValidLocation(pTab->lMapIndex, pTab->x, pTab->y, lMapIndex, pos, d->GetEmpire()))
	{
		sys_err("InputDB::PlayerLoad : cannot find valid location %d x %d (name: %s)", pTab->x, pTab->y, pTab->name);
#ifdef ENABLE_GOHOME_IF_MAP_NOT_EXIST
		lMapIndex = EMPIRE_START_MAP(d->GetAccountTable().bEmpire);
		pos.x = EMPIRE_START_X(d->GetAccountTable().bEmpire);
		pos.y = EMPIRE_START_Y(d->GetAccountTable().bEmpire);
#else
		d->SetPhase(PHASE_CLOSE);
		return;
#endif
	}

	pTab->x = pos.x;
	pTab->y = pos.y;
	pTab->lMapIndex = lMapIndex;

	if (d->GetCharacter() || d->IsPhase(PHASE_GAME))
	{
		LPCHARACTER p = d->GetCharacter();
		sys_err("login state already has main state (character %s %p)", p->GetName(), get_pointer(p));
		return;
	}

	if (NULL != CHARACTER_MANAGER::Instance().FindPC(pTab->name))
	{
		sys_err("InputDB: PlayerLoad : %s already exist in game", pTab->name);
		return;
	}

	LPCHARACTER ch = CHARACTER_MANAGER::instance().CreateCharacter(pTab->name, pTab->id);

	ch->BindDesc(d);
#ifdef __BL_BATTLE_ROYALE__
	// We should probably throw an error and crash the core if it fails in battle royale
	if (lMapIndex != BattleRoyale::Settings::BR_MAP_INDEX || !BattleRoyale::Instance().SetBattleRoyaleTableFromPlayerTable(ch, pTab))
#endif
	{
		ch->SetPlayerProto(pTab);
	}
	ch->SetEmpire(d->GetEmpire());

	d->BindCharacter(ch);
	
#ifdef VOTE_BUFF_RENEWAL
	long vcoins = ch->GetEp();
	ch->SetEp(vcoins);
#endif

	{
		// P2P Login
		TPacketGGLogin p;

		p.bHeader = HEADER_GG_LOGIN;
		strlcpy(p.szName, ch->GetName(), sizeof(p.szName));
		p.dwPID = ch->GetPlayerID();
		p.bEmpire = ch->GetEmpire();
		p.lMapIndex = SECTREE_MANAGER::instance().GetMapIndex(ch->GetX(), ch->GetY());
		p.bChannel = g_bChannel;

		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGLogin));

		char buf[51];
		snprintf(buf, sizeof(buf), "%s %lld %d %ld %d", 
				inet_ntoa(ch->GetDesc()->GetAddr().sin_addr), ch->GetGold(), g_bChannel, ch->GetMapIndex(), ch->GetAlignment());
		LogManager::instance().CharLog(ch, 0, "LOGIN", buf);

#ifdef ENABLE_PCBANG_FEATURE // @warme006
		{
			LogManager::instance().LoginLog(true,
					ch->GetDesc()->GetAccountTable().id, ch->GetPlayerID(), ch->GetLevel(), ch->GetJob(), ch->GetRealPoint(POINT_PLAYTIME));

			if (0)
				ch->SetPCBang(CPCBangManager::instance().IsPCBangIP(ch->GetDesc()->GetHostName()));
		}
#endif
	}

	d->SetPhase(PHASE_LOADING);

#ifdef ENABLE_ENTITY_LOADING
	SECTREE_MANAGER::Instance().SendEntity(ch);
#endif

	ch->MainCharacterPacket();

	long lPublicMapIndex = lMapIndex >= 10000 ? lMapIndex / 10000 : lMapIndex;

	//if (!map_allow_find(lMapIndex >= 10000 ? lMapIndex / 10000 : lMapIndex) || !CheckEmpire(ch, lMapIndex))
	if (!map_allow_find(lPublicMapIndex))
	{
		sys_err("InputDB::PlayerLoad : entering %d map is not allowed here (name: %s, empire %u)",
				lMapIndex, pTab->name, d->GetEmpire());

		ch->SetWarpLocation(EMPIRE_START_MAP(d->GetEmpire()),
				EMPIRE_START_X(d->GetEmpire()) / 100,
				EMPIRE_START_Y(d->GetEmpire()) / 100);

		d->SetPhase(PHASE_CLOSE);
		return;
	}

	quest::CQuestManager::instance().BroadcastEventFlagOnLogin(ch);

#ifdef __BL_BATTLE_ROYALE__
	bool isBattleRoyale = ch->IsBattleRoyale();
	TQuickslot emptySlot{};
	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
	{
		if (isBattleRoyale)
			ch->SetQuickslot(i, emptySlot);
		else
			ch->SetQuickslot(i, pTab->quickslot[i]);
	}
#else
	bool isBattleRoyale = false;
	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
		ch->SetQuickslot(i, pTab->quickslot[i]);
#endif

	ch->PointsPacket();
	ch->SkillLevelPacket();

	sys_log(0, "InputDB: player_load %s %dx%dx%d LEVEL %d MOV_SPEED %d JOB %d ATG %d DFG %d GMLv %d battle royale %u",
		pTab->name,
		ch->GetX(), ch->GetY(), ch->GetZ(),
		ch->GetLevel(),
		ch->GetPoint(POINT_MOV_SPEED),
		ch->GetJob(),
		ch->GetPoint(POINT_ATT_GRADE),
		ch->GetPoint(POINT_DEF_GRADE),
		ch->GetGMLevel(), isBattleRoyale);

	ch->QuerySafeboxSize();

	CAnticheatManager::instance().CreateClientHandle(ch);
}

void CInputDB::Boot(const char* data)
{
	signal_timer_disable();

	// ÆÐÅ¶ »çÀÌÁî Ã¼Å©
	DWORD dwPacketSize = decode_4bytes(data);
	data += 4;

	// ÆÐÅ¶ ¹öÀü Ã¼Å©
	BYTE bVersion = decode_byte(data);
	data += 1;

	sys_log(0, "BOOT: PACKET: %d", dwPacketSize);
	sys_log(0, "BOOT: VERSION: %d", bVersion);
	if (bVersion != 6)
	{
		sys_err("boot version error");
		thecore_shutdown();
	}

	sys_log(0, "sizeof(TMobTable) = %d", sizeof(TMobTable));
	sys_log(0, "sizeof(TItemTable) = %d", sizeof(TItemTable));
	sys_log(0, "sizeof(TShopTable) = %d", sizeof(TShopTable));
	sys_log(0, "sizeof(TSkillTable) = %d", sizeof(TSkillTable));
	sys_log(0, "sizeof(TRefineTable) = %d", sizeof(TRefineTable));

	sys_log(0, "sizeof(TItemAttrTable) = %d", sizeof(TItemAttrTable));
	sys_log(0, "sizeof(TItemRareTable) = %d", sizeof(TItemAttrTable));
	sys_log(0, "sizeof(TBanwordTable) = %d", sizeof(TBanwordTable));
	sys_log(0, "sizeof(TLand) = %d", sizeof(building::TLand));
	sys_log(0, "sizeof(TObjectProto) = %d", sizeof(building::TObjectProto));
	sys_log(0, "sizeof(TObject) = %d", sizeof(building::TObject));
#ifdef ENABLE_YOHARA_SYSTEM
	sys_log(0, "sizeof(TSungmaTable) = %d", sizeof(TSungmaTable));
#endif
	//ADMIN_MANAGER
	sys_log(0, "sizeof(TAdminManager) = %d", sizeof (TAdminInfo) );
	//END_ADMIN_MANAGER
#ifdef USE_DUNGEON_COOLDOWN_MANAGER
	sys_log(0, "sizeof(TPacketDungeonCooldownMgrSet) = %u", sizeof(TPacketDungeonCooldownMgrSet));
#endif

	WORD size;

	/*
	 * MOB
	 */

	if (decode_2bytes(data)!=sizeof(TMobTable))
	{
		sys_err("mob table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: MOB: %d", size);

	if (size)
	{
		CMobManager::instance().Initialize((TMobTable *) data, size);
		data += size * sizeof(TMobTable);
	}

	/*
	 * ITEM
	 */

	if (decode_2bytes(data) != sizeof(TItemTable))
	{
		sys_err("item table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: ITEM: %d", size);


	if (size)
	{
		ITEM_MANAGER::instance().Initialize((TItemTable *) data, size);
		data += size * sizeof(TItemTable);
	}

	/*
	 * SHOP
	 */

	if (decode_2bytes(data) != sizeof(TShopTable))
	{
		sys_err("shop table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: SHOP: %d", size);


	if (size)
	{
		if (!CShopManager::instance().Initialize((TShopTable *) data, size))
		{
			sys_err("shop table Initialize error");
			thecore_shutdown();
			return;
		}
		data += size * sizeof(TShopTable);
	}

	/*
	 * SKILL
	 */

	if (decode_2bytes(data) != sizeof(TSkillTable))
	{
		sys_err("skill table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: SKILL: %d", size);

	if (size)
	{
		if (!CSkillManager::instance().Initialize((TSkillTable *) data, size))
		{
			sys_err("cannot initialize skill table");
			thecore_shutdown();
			return;
		}

		data += size * sizeof(TSkillTable);
	}
	/*
	 * REFINE RECIPE
	 */
	if (decode_2bytes(data) != sizeof(TRefineTable))
	{
		sys_err("refine table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: REFINE: %d", size);

	if (size)
	{
		CRefineManager::instance().Initialize((TRefineTable*) data, size);
		data += size * sizeof(TRefineTable);
	}

	/*
	 * ITEM ATTR
	 */
	if (decode_2bytes(data) != sizeof(TItemAttrTable))
	{
		sys_err("item attr table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: ITEM_ATTR: %d", size);

	if (size)
	{
		TItemAttrTable * p = (TItemAttrTable *) data;

		for (int i = 0; i < size; ++i, ++p)
		{
			if (p->dwApplyIndex >= MAX_APPLY_NUM)
				continue;

			g_map_itemAttr[p->dwApplyIndex] = *p;
			sys_log(0, "ITEM_ATTR[%d]: %s %u", p->dwApplyIndex, p->szApply, p->dwProb);
		}
	}

	data += size * sizeof(TItemAttrTable);


	/*
     * ITEM RARE
     */
	if (decode_2bytes(data) != sizeof(TItemAttrTable))
	{
		sys_err("item rare table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: ITEM_RARE: %d", size);

	if (size)
	{
		TItemAttrTable * p = (TItemAttrTable *) data;

		for (int i = 0; i < size; ++i, ++p)
		{
			if (p->dwApplyIndex >= MAX_APPLY_NUM)
				continue;

			g_map_itemRare[p->dwApplyIndex] = *p;
			sys_log(0, "ITEM_RARE[%d]: %s %u", p->dwApplyIndex, p->szApply, p->dwProb);
		}
	}

	data += size * sizeof(TItemAttrTable);

	/*
     * ITEM COSTUME
     */
	if (decode_2bytes(data) != sizeof(TItemAttrTable))
	{
		sys_err("item costume table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: ITEM_COSTUME: %d", size);

	if (size)
	{
		TItemAttrTable * p = (TItemAttrTable *) data;

		for (int i = 0; i < size; ++i, ++p)
		{
			if (p->dwApplyIndex >= MAX_APPLY_NUM)
				continue;

			g_map_itemCostume[p->dwApplyIndex] = *p;
			sys_log(0, "ITEM_COSTUME[%d]: %s %u", p->dwApplyIndex, p->szApply, p->dwProb);
		}
	}

	data += size * sizeof(TItemAttrTable);
	

		/*
     * ITEM COSTUME HAIR
     */
	if (decode_2bytes(data) != sizeof(TItemAttrTable))
	{
		sys_err("item costume HAIR table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: ITEM_COSTUME_HAIR: %d", size);

	if (size)
	{
		TItemAttrTable * p = (TItemAttrTable *) data;

		for (int i = 0; i < size; ++i, ++p)
		{
			if (p->dwApplyIndex >= MAX_APPLY_NUM)
				continue;

			g_map_itemCostumeHair[p->dwApplyIndex] = *p;
			sys_log(0, "ITEM_COSTUME_HAIR[%d]: %s %u", p->dwApplyIndex, p->szApply, p->dwProb);
		}
	}

	data += size * sizeof(TItemAttrTable);


	/*
	 * ITEM ATTR RINGS
	 */
	if (decode_2bytes(data) != sizeof(TItemAttrTable))
	{
		sys_err("item attr rings table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: ITEM_ATTR: %d", size);

	if (size)
	{
		TItemAttrTable * p = (TItemAttrTable *) data;

		for (int i = 0; i < size; ++i, ++p)
		{
			if (p->dwApplyIndex >= MAX_APPLY_NUM)
				continue;

			g_map_itemRingsAttr[p->dwApplyIndex] = *p;
			sys_log(0, "ITEM_RINGS_ATTR[%d]: %s %u", p->dwApplyIndex, p->szApply, p->dwProb);
		}
	}

	data += size * sizeof(TItemAttrTable);


	/*
     * ITEM RARE RINGS
     */
	if (decode_2bytes(data) != sizeof(TItemAttrTable))
	{
		sys_err("item rare rings table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: ITEM_RINGS_RARE: %d", size);

	if (size)
	{
		TItemAttrTable * p = (TItemAttrTable *) data;

		for (int i = 0; i < size; ++i, ++p)
		{
			if (p->dwApplyIndex >= MAX_APPLY_NUM)
				continue;

			g_map_itemRingsRare[p->dwApplyIndex] = *p;
			sys_log(0, "ITEM_RINGS_RARE[%d]: %s %u", p->dwApplyIndex, p->szApply, p->dwProb);
		}
	}

	data += size * sizeof(TItemAttrTable);

	/*
	 * BANWORDS
	 */

	if (decode_2bytes(data) != sizeof(TBanwordTable))
	{
		sys_err("ban word table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;

	CBanwordManager::instance().Initialize((TBanwordTable *) data, size);
	data += size * sizeof(TBanwordTable);

	{
		using namespace building;

		/*
		 * LANDS
		 */

		if (decode_2bytes(data) != sizeof(TLand))
		{
			sys_err("land table size error");
			thecore_shutdown();
			return;
		}
		data += 2;

		size = decode_2bytes(data);
		data += 2;

		TLand * kLand = (TLand *) data;
		data += size * sizeof(TLand);

		for (WORD i = 0; i < size; ++i, ++kLand)
			CManager::instance().LoadLand(kLand);

		/*
		 * OBJECT PROTO
		 */

		if (decode_2bytes(data) != sizeof(TObjectProto))
		{
			sys_err("object proto table size error");
			thecore_shutdown();
			return;
		}
		data += 2;

		size = decode_2bytes(data);
		data += 2;

		if (size)
		{
			CManager::instance().LoadObjectProto((TObjectProto*)data, size);
			data += size * sizeof(TObjectProto);
		}

		/*
		 * OBJECT
		 */
		if (decode_2bytes(data) != sizeof(TObject))
		{
			sys_err("object table size error");
			thecore_shutdown();
			return;
		}
		data += 2;

		size = decode_2bytes(data);
		data += 2;

		TObject * kObj = (TObject *) data;
		data += size * sizeof(TObject);

		for (WORD i = 0; i < size; ++i, ++kObj)
			CManager::instance().LoadObject(kObj, true);
	}

#ifdef ENABLE_YOHARA_SYSTEM
	/*
	* SUNGMA_TABLE
	*/

	if (decode_2bytes(data) != sizeof(TSungmaTable))
	{
		sys_err("sungma table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: SUNGMA_TABLE: %d", size);

	if (size)
	{
		TSungmaTable* p = (TSungmaTable*)data;
		for (size_t i = 0; i < size; ++i, ++p)
		{
			g_map_SungmaTable[p->lMapIdx] = *p;
			sys_log(0, "SUNGMA_MAP[%d]: str %d hp %d move %d immune %d "
#ifdef ENABLE_PRECISION
				"hit_pct %d"
#endif
				, p->lMapIdx, p->bSungmaStr, p->bSungmaHp, p->bSungmaMove, p->bSungmaImmune
#ifdef ENABLE_PRECISION
				, p->bHitPct
#endif
			);
		}

		data += size * sizeof(TSungmaTable);
	}
#endif

	set_global_time(*(time_t *) data);
	data += sizeof(time_t);

	if (decode_2bytes(data) != sizeof(TItemIDRangeTable) )
	{
		sys_err("ITEM ID RANGE size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;

	TItemIDRangeTable* range = (TItemIDRangeTable*) data;
	data += size * sizeof(TItemIDRangeTable);

	TItemIDRangeTable* rangespare = (TItemIDRangeTable*) data;
	data += size * sizeof(TItemIDRangeTable);

	//ADMIN_MANAGER
	//°ü¸®ÀÚ µî·Ï
	int ChunkSize = decode_2bytes(data );
	data += 2;
	int HostSize = decode_2bytes(data );
	data += 2;
	sys_log(0, "GM Value Count %d %d", HostSize, ChunkSize  );
	for (int n = 0; n < HostSize; ++n )
	{
		gm_new_host_inert(data );
		sys_log(0, "GM HOST : IP[%s] ", data );
		data += ChunkSize;
	}


	data += 2;
	int adminsize = decode_2bytes(data );
	data += 2;

	for (int n = 0; n < adminsize; ++n )
	{
		tAdminInfo& rAdminInfo = *(tAdminInfo*)data;

		gm_new_insert(rAdminInfo );

		data += sizeof(rAdminInfo );
	}

	//END_ADMIN_MANAGER

	//MONARCH
	data += 2;
	data += 2;

	TMonarchInfo& p = *(TMonarchInfo *) data;
	data += sizeof(TMonarchInfo);

	CMonarch::instance().SetMonarchInfo(&p);

	for (int n = 1; n < 4; ++n)
	{
		if (p.name[n] && *p.name[n])
			sys_log(0, "[MONARCH] Empire %d Pid %d Money %d %s", n, p.pid[n], p.money[n], p.name[n]);
	}

	int CandidacySize = decode_2bytes(data);
	data += 2;

	int CandidacyCount = decode_2bytes(data);
	data += 2;

	if (test_server)
		sys_log (0, "[MONARCH] Size %d Count %d", CandidacySize, CandidacyCount);

	data += CandidacySize * CandidacyCount;


	//END_MONARCH

#ifdef USE_DUNGEON_COOLDOWN_MANAGER
	if (decode_2bytes(data) != sizeof(TPacketDungeonCooldownMgrSet))
	{
		sys_err("TPacketDungeonCooldownMgrSet table size error");
		thecore_shutdown();
		return;
	}

	data += 2;

	size = decode_4bytes(data);
	data += 4;

	auto& dungeonCooldownMgr = CDungeonCooldownMgr::Instance();
	dungeonCooldownMgr.Initialize((TPacketDungeonCooldownMgrSet*)data, size);
	data += size * sizeof(TPacketDungeonCooldownMgrSet);
#endif

#ifdef USE_DAY_CHANGE_MGR
	int iDayNum = decode_4bytes(data);
	CDayChangeMgr::Instance().Setup(iDayNum);

	data += 4;
#endif

	WORD endCheck=decode_2bytes(data);
	if (endCheck != 0xffff)
	{
		sys_err("boot packet end check error [%x]!=0xffff", endCheck);
		thecore_shutdown();
		return;
	}
	else
		sys_log(0, "boot packet end check ok [%x]==0xffff", endCheck );
	data +=2;

	if (!ITEM_MANAGER::instance().SetMaxItemID(*range))
	{
		sys_err("not enough item id contact your administrator!");
		thecore_shutdown();
		return;
	}

	if (!ITEM_MANAGER::instance().SetMaxSpareItemID(*rangespare))
	{
		sys_err("not enough item id for spare contact your administrator!");
		thecore_shutdown();
		return;
	}



	// LOCALE_SERVICE
	const int FILE_NAME_LEN = 256;
	char szCommonDropItemFileName[FILE_NAME_LEN];
	char szETCDropItemFileName[FILE_NAME_LEN];
	char szMOBDropItemFileName[FILE_NAME_LEN];
	char szDropItemGroupFileName[FILE_NAME_LEN];
	char szSpecialItemGroupFileName[FILE_NAME_LEN];
	char szMapIndexFileName[FILE_NAME_LEN];
	char szItemVnumMaskTableFileName[FILE_NAME_LEN];
	char szDragonSoulTableFileName[FILE_NAME_LEN];

	snprintf(szCommonDropItemFileName, sizeof(szCommonDropItemFileName),
			"%s/common_drop_item.txt", LocaleService_GetBasePath().c_str());
	snprintf(szETCDropItemFileName, sizeof(szETCDropItemFileName),
			"%s/etc_drop_item.txt", LocaleService_GetBasePath().c_str());
	snprintf(szMOBDropItemFileName, sizeof(szMOBDropItemFileName),
			"%s/mob_drop_item.txt", LocaleService_GetBasePath().c_str());
	snprintf(szSpecialItemGroupFileName, sizeof(szSpecialItemGroupFileName),
			"%s/special_item_group.txt", LocaleService_GetBasePath().c_str());
	snprintf(szDropItemGroupFileName, sizeof(szDropItemGroupFileName),
			"%s/drop_item_group.txt", LocaleService_GetBasePath().c_str());
	snprintf(szMapIndexFileName, sizeof(szMapIndexFileName),
			"%s/index", LocaleService_GetMapPath().c_str());
	snprintf(szItemVnumMaskTableFileName, sizeof(szItemVnumMaskTableFileName),
			"%s/ori_to_new_table.txt", LocaleService_GetBasePath().c_str());
	snprintf(szDragonSoulTableFileName, sizeof(szDragonSoulTableFileName),
			"%s/dragon_soul_table.txt", LocaleService_GetBasePath().c_str());

	sys_log(0, "Initializing Informations of Cube System");
	Cube_InformationInitialize();

	sys_log(0, "LoadLocaleFile: CommonDropItem: %s", szCommonDropItemFileName);
	if (!ITEM_MANAGER::instance().ReadCommonDropItemFile(szCommonDropItemFileName))
	{
		sys_err("cannot load CommonDropItem: %s", szCommonDropItemFileName);
		thecore_shutdown();
		return;
	}
#ifdef NEW_COMMON_DROP
	snprintf(szCommonDropItemFileName, sizeof(szCommonDropItemFileName), "%s/common_drop_new.txt", LocaleService_GetBasePath().c_str());
	if (!ITEM_MANAGER::instance().ReadCommonDropNewItemFile(szCommonDropItemFileName))
	{
		sys_err("cannot load CommonDropItemNew: %s", szCommonDropItemFileName);
		thecore_shutdown();
		return;
	}
#endif

	sys_log(0, "LoadLocaleFile: ETCDropItem: %s", szETCDropItemFileName);
	if (!ITEM_MANAGER::instance().ReadEtcDropItemFile(szETCDropItemFileName))
	{
		sys_err("cannot load ETCDropItem: %s", szETCDropItemFileName);
		thecore_shutdown();
		return;
	}

	sys_log(0, "LoadLocaleFile: DropItemGroup: %s", szDropItemGroupFileName);
	if (!ITEM_MANAGER::instance().ReadDropItemGroup(szDropItemGroupFileName))
	{
		sys_err("cannot load DropItemGroup: %s", szDropItemGroupFileName);
		thecore_shutdown();
		return;
	}

	sys_log(0, "LoadLocaleFile: SpecialItemGroup: %s", szSpecialItemGroupFileName);
	if (!ITEM_MANAGER::instance().ReadSpecialDropItemFile(szSpecialItemGroupFileName))
	{
		sys_err("cannot load SpecialItemGroup: %s", szSpecialItemGroupFileName);
		thecore_shutdown();
		return;
	}

	sys_log(0, "LoadLocaleFile: ItemVnumMaskTable : %s", szItemVnumMaskTableFileName);
	if (!ITEM_MANAGER::instance().ReadItemVnumMaskTable(szItemVnumMaskTableFileName))
	{
		sys_log(0, "Could not open MaskItemTable");
	}

	sys_log(0, "LoadLocaleFile: MOBDropItemFile: %s", szMOBDropItemFileName);
	if (!ITEM_MANAGER::instance().ReadMonsterDropItemGroup(szMOBDropItemFileName))
	{
		sys_err("cannot load MOBDropItemFile: %s", szMOBDropItemFileName);
		thecore_shutdown();
		return;
	}

	sys_log(0, "LoadLocaleFile: MapIndex: %s", szMapIndexFileName);
	if (!SECTREE_MANAGER::instance().Build(szMapIndexFileName, LocaleService_GetMapPath().c_str()))
	{
		sys_err("cannot load MapIndex: %s", szMapIndexFileName);
		thecore_shutdown();
		return;
	}

	sys_log(0, "LoadLocaleFile: DragonSoulTable: %s", szDragonSoulTableFileName);
	if (!DSManager::instance().ReadDragonSoulTableFile(szDragonSoulTableFileName))
	{
		sys_err("cannot load DragonSoulTable: %s", szDragonSoulTableFileName);
		//thecore_shutdown();
		//return;
	}

	// END_OF_LOCALE_SERVICE


	building::CManager::instance().FinalizeBoot();

	CMotionManager::instance().Build();

#ifdef ENABLE_BATTLE_PASS
	CHARACTER_MANAGER::Instance().InitializeBattlePass();
#endif

	signal_timer_enable(30);
#ifdef ENABLE_REWARD_SYSTEM
	CHARACTER_MANAGER::Instance().LoadRewardData();
#endif
#ifdef ENABLE_ULTIMATE_REGEN
	char buf[250];
	snprintf(buf, sizeof(buf), "%s/newregen.txt", LocaleService_GetBasePath().c_str());
	CNewMobTimer::Instance().LoadFile(buf);
#endif
#ifdef RENEWAL_MISSION_BOOKS
	CHARACTER_MANAGER::Instance().LoadMissionBook();
#endif
	if (test_server)
	{
		CMobManager::instance().DumpRegenCount("mob_count");
	}

	CPCBangManager::instance().RequestUpdateIPList(0);

	// castle_boot
	castle_boot();

	// request blocked_country_ip
	{
		db_clientdesc->DBPacket(HEADER_GD_BLOCK_COUNTRY_IP, 0, NULL, 0);
		dev_log(LOG_DEB0, "<sent HEADER_GD_BLOCK_COUNTRY_IP>");
	}
#if defined(ENABLE_2LOOT)
	Init2Loot();
#endif
#ifdef ENABLE_BATTLEPASS
	CHARACTER_MANAGER::Instance().LoadBattlePass();
#endif
}

EVENTINFO(quest_login_event_info)
{
	DWORD dwPID;

	quest_login_event_info()
	: dwPID( 0 )
	{
	}
};

EVENTFUNC(quest_login_event)
{
	quest_login_event_info* info = dynamic_cast<quest_login_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "quest_login_event> <Factor> Null pointer" );
		return 0;
	}

	DWORD dwPID = info->dwPID;

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(dwPID);

	if (!ch)
		return 0;

	LPDESC d = ch->GetDesc();

	if (!d)
		return 0;

	if (d->IsPhase(PHASE_HANDSHAKE) ||
		d->IsPhase(PHASE_LOGIN) ||
		d->IsPhase(PHASE_SELECT) ||
		d->IsPhase(PHASE_DEAD) ||
		d->IsPhase(PHASE_LOADING))
	{
		return PASSES_PER_SEC(1);
	}
	else if (d->IsPhase(PHASE_CLOSE))
	{
		return 0;
	}
	else if (d->IsPhase(PHASE_GAME))
	{
		sys_log(0, "QUEST_LOAD: Login pc %d by event", ch->GetPlayerID());
#ifdef __SPIN_WHEEL__
		if (quest::CQuestManager::instance().GetEventFlag("spin_wheel") == 1)
			ch->ChatPacket(CHAT_TYPE_COMMAND, "SetSpinWheel %d 1", ch->GetQuestFlag("spin_wheel.count"));
#endif
		quest::CQuestManager::instance().Login(ch->GetPlayerID());
		return 0;
	}
	else
	{
		sys_err(0, "input_db.cpp:quest_login_event INVALID PHASE pid %d", ch->GetPlayerID());
		return 0;
	}
}

void CInputDB::QuestLoad(LPDESC d, const char * c_pData)
{
	if (NULL == d)
		return;

	LPCHARACTER ch = d->GetCharacter();

	if (NULL == ch)
		return;

#ifdef __BL_BATTLE_ROYALE__
	if (ch->IsBattleRoyale())
		return;
#endif

	const DWORD dwCount = decode_4bytes(c_pData);

	const TQuestTable* pQuestTable = reinterpret_cast<const TQuestTable*>(c_pData+4);

	if (NULL != pQuestTable)
	{
		if (dwCount != 0)
		{
			if (ch->GetPlayerID() != pQuestTable[0].dwPID)
			{
				sys_err("PID differs %u %u", ch->GetPlayerID(), pQuestTable[0].dwPID);
				return;
			}
		}

		sys_log(0, "QUEST_LOAD: count %d", dwCount);

		quest::PC * pkPC = quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID());

		if (!pkPC)
		{
			sys_err("null quest::PC with id %u", pQuestTable[0].dwPID);
			return;
		}

		if (pkPC->IsLoaded())
			return;

		for (unsigned int i = 0; i < dwCount; ++i)
		{
			std::string st(pQuestTable[i].szName);

			st += ".";
			st += pQuestTable[i].szState;

			sys_log(0,  "            %s %d", st.c_str(), pQuestTable[i].lValue);
			pkPC->SetFlag(st.c_str(), pQuestTable[i].lValue, false);
		}

		pkPC->SetLoaded();
		pkPC->Build();

		if (ch->GetDesc()->IsPhase(PHASE_GAME))
		{
			sys_log(0, "QUEST_LOAD: Login pc %d", pQuestTable[0].dwPID);
#ifdef __SPIN_WHEEL__
			if (quest::CQuestManager::instance().GetEventFlag("spin_wheel") == 1)
				ch->ChatPacket(CHAT_TYPE_COMMAND, "SetSpinWheel %d 1", ch->GetQuestFlag("spin_wheel.count"));
#endif
			quest::CQuestManager::instance().Login(pQuestTable[0].dwPID);
		}
		else
		{
			quest_login_event_info* info = AllocEventInfo<quest_login_event_info>();
			info->dwPID = ch->GetPlayerID();

			event_create(quest_login_event, info, PASSES_PER_SEC(1));
		}
	}
}

void CInputDB::SafeboxLoad(LPDESC d, const char * c_pData)
{
	if (!d)
		return;

	TSafeboxTable * p = (TSafeboxTable *) c_pData;

	if (d->GetAccountTable().id != p->dwID)
	{
		sys_err("SafeboxLoad: safebox has different id %u != %u", d->GetAccountTable().id, p->dwID);
		return;
	}

	if (!d->GetCharacter())
		return;

	BYTE bSize = 4;

	LPCHARACTER ch = d->GetCharacter();

	//Fix Revive Time

	if(ch->IsDead())
	{
		d->GetCharacter()->CancelSafeboxLoad();
		return;
	}

	//PREVENT_TRADE_WINDOW
	if (ch->GetExchange()
		 || ch->GetShop()
		 || ch->IsCubeOpen()
#ifdef __BL_67_ATTR__
		 || ch->Is67AttrOpen()
#endif
#ifdef __AURA_SYSTEM__
		 || ch->IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
		 || ch->isSashOpened(true)
		 || ch->isSashOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
		 || ch->IsShardCraftOpened()
#endif
		)
	{
		d->GetCharacter()->ChatPacket(CHAT_TYPE_INFO, "721" );
		d->GetCharacter()->CancelSafeboxLoad();
		return;
	}


	//END_PREVENT_TRADE_WINDOW

	// ADD_PREMIUM
	//if (d->GetCharacter()->GetPremiumRemainSeconds(PREMIUM_SAFEBOX) > 0 || d->GetCharacter()->IsEquipUniqueGroup(UNIQUE_GROUP_LARGE_SAFEBOX))
	//	bSize = 4;
	// END_OF_ADD_PREMIUM

	//if (d->GetCharacter()->IsEquipUniqueItem(UNIQUE_ITEM_SAFEBOX_EXPAND))
	//bSize = 3; // Ã¢°íÈ®Àå±Ç

	//d->GetCharacter()->LoadSafebox(p->bSize * SAFEBOX_PAGE_SIZE, p->dwGold, p->wItemCount, (TPlayerItem *) (c_pData + sizeof(TSafeboxTable)));
	d->GetCharacter()->LoadSafebox(bSize * SAFEBOX_PAGE_SIZE, p->dwGold, p->wItemCount, (TPlayerItem *) (c_pData + sizeof(TSafeboxTable)));
}

void CInputDB::SafeboxChangeSize(LPDESC d, const char * c_pData)
{
	if (!d)
		return;

	BYTE bSize = *(BYTE *) c_pData;

	if (!d->GetCharacter())
		return;

	d->GetCharacter()->ChangeSafeboxSize(bSize);
}

//
// @version	05/06/20 Bang2ni - ReqSafeboxLoad ÀÇ Ãë¼Ò
//
void CInputDB::SafeboxWrongPassword(LPDESC d)
{
	if (!d)
		return;

	if (!d->GetCharacter())
		return;

	TPacketCGSafeboxWrongPassword p;
	p.bHeader = HEADER_GC_SAFEBOX_WRONG_PASSWORD;
	d->Packet(&p, sizeof(p));

	d->GetCharacter()->CancelSafeboxLoad();
}

void CInputDB::SafeboxChangePasswordAnswer(LPDESC d, const char* c_pData)
{
	if (!d)
		return;

	if (!d->GetCharacter())
		return;

	TSafeboxChangePasswordPacketAnswer* p = (TSafeboxChangePasswordPacketAnswer*) c_pData;
	if (p->flag)
	{
		d->GetCharacter()->ChatPacket(CHAT_TYPE_INFO, "722");
	}
	else
	{
		d->GetCharacter()->ChatPacket(CHAT_TYPE_INFO, "723");
	}
}

void CInputDB::MallLoad(LPDESC d, const char * c_pData)
{
	if (!d)
		return;

	TSafeboxTable * p = (TSafeboxTable *) c_pData;

	if (d->GetAccountTable().id != p->dwID)
	{
		sys_err("safebox has different id %u != %u", d->GetAccountTable().id, p->dwID);
		return;
	}

	if (!d->GetCharacter())
		return;

	d->GetCharacter()->LoadMall(p->wItemCount, (TPlayerItem *) (c_pData + sizeof(TSafeboxTable)));
}

void CInputDB::LoginAlready(LPDESC d, const char * c_pData)
{
	if (!d)
		return;

	// INTERNATIONAL_VERSION ÀÌ¹Ì Á¢¼ÓÁßÀÌ¸é Á¢¼Ó ²÷À½
	{
		TPacketDGLoginAlready * p = (TPacketDGLoginAlready *) c_pData;

		LPDESC d2 = DESC_MANAGER::instance().FindByLoginName(p->szLogin);

		if (d2)
			d2->DisconnectOfSameLogin();
		else
		{
			TPacketGGDisconnect pgg;

			pgg.bHeader = HEADER_GG_DISCONNECT;
			strlcpy(pgg.szLogin, p->szLogin, sizeof(pgg.szLogin));

			P2P_MANAGER::instance().Send(&pgg, sizeof(TPacketGGDisconnect));
		}
	}
	// END_OF_INTERNATIONAL_VERSION

	LoginFailure(d, "ALREADY");
}

void CInputDB::EmpireSelect(LPDESC d, const char * c_pData)
{
	sys_log(0, "EmpireSelect %p", get_pointer(d));

	if (!d)
		return;

	TAccountTable & rTable = d->GetAccountTable();
	rTable.bEmpire = *(BYTE *) c_pData;

	TPacketGCEmpire pe;
	pe.bHeader = HEADER_GC_EMPIRE;
	pe.bEmpire = rTable.bEmpire;
	d->Packet(&pe, sizeof(pe));

	for (int i = 0; i < PLAYER_PER_ACCOUNT; ++i)
		if (rTable.players[i].dwID)
		{
			rTable.players[i].x = EMPIRE_START_X(rTable.bEmpire);
			rTable.players[i].y = EMPIRE_START_Y(rTable.bEmpire);
		}

	GetServerLocation(d->GetAccountTable(), rTable.bEmpire);

	d->SendLoginSuccessPacket();
}

void CInputDB::MapLocations(const char * c_pData)
{
	BYTE bCount = *(BYTE *) (c_pData++);

	sys_log(0, "InputDB::MapLocations %d", bCount);

	TMapLocation * pLoc = (TMapLocation *) c_pData;

	while (bCount--)
	{
		for (int i = 0; i < MAP_ALLOW_LIMIT; ++i)
		{
			if (0 == pLoc->alMaps[i])
				break;

			CMapLocation::instance().Insert(pLoc->alMaps[i], pLoc->szHost, pLoc->wPort);
		}

		pLoc++;
	}
}

void CInputDB::P2P(const char * c_pData)
{
	extern LPFDWATCH main_fdw;

	TPacketDGP2P * p = (TPacketDGP2P *) c_pData;

	P2P_MANAGER& mgr = P2P_MANAGER::instance();

	if (false == DESC_MANAGER::instance().IsP2PDescExist(p->szHost, p->wPort))
	{
	    LPCLIENT_DESC pkDesc = NULL;
		sys_log(0, "InputDB:P2P %s:%u", p->szHost, p->wPort);
	    pkDesc = DESC_MANAGER::instance().CreateConnectionDesc(main_fdw, p->szHost, p->wPort, PHASE_P2P, false);
		mgr.RegisterConnector(pkDesc);
		pkDesc->SetP2P(p->szHost, p->wPort, p->bChannel);
	}
}

void CInputDB::GuildLoad(const char * c_pData)
{
	CGuildManager::instance().LoadGuild(*(DWORD *) c_pData);
}

void CInputDB::GuildSkillUpdate(const char* c_pData)
{
	TPacketGuildSkillUpdate * p = (TPacketGuildSkillUpdate *) c_pData;

	CGuild * g = CGuildManager::instance().TouchGuild(p->guild_id);

	if (g)
	{
		g->UpdateSkill(p->skill_point, p->skill_levels);
		g->GuildPointChange(POINT_SP, p->amount, p->save?true:false);
	}
}

void CInputDB::GuildWar(const char* c_pData)
{
	TPacketGuildWar * p = (TPacketGuildWar*) c_pData;

	sys_log(0, "InputDB::GuildWar %u %u state %d", p->dwGuildFrom, p->dwGuildTo, p->bWar);

	switch (p->bWar)
	{
		case GUILD_WAR_SEND_DECLARE:
		case GUILD_WAR_RECV_DECLARE:
			CGuildManager::instance().DeclareWar(p->dwGuildFrom, p->dwGuildTo, p->bType);
			break;

		case GUILD_WAR_REFUSE:
			CGuildManager::instance().RefuseWar(p->dwGuildFrom, p->dwGuildTo);
			break;

		case GUILD_WAR_WAIT_START:
			CGuildManager::instance().WaitStartWar(p->dwGuildFrom, p->dwGuildTo);
			break;

		case GUILD_WAR_CANCEL:
			CGuildManager::instance().CancelWar(p->dwGuildFrom, p->dwGuildTo);
			break;

		case GUILD_WAR_ON_WAR:
			CGuildManager::instance().StartWar(p->dwGuildFrom, p->dwGuildTo);
			break;

		case GUILD_WAR_END:
			CGuildManager::instance().EndWar(p->dwGuildFrom, p->dwGuildTo);
			break;

		case GUILD_WAR_OVER:
			CGuildManager::instance().WarOver(p->dwGuildFrom, p->dwGuildTo, p->bType);
			break;

		case GUILD_WAR_RESERVE:
			CGuildManager::instance().ReserveWar(p->dwGuildFrom, p->dwGuildTo, p->bType);
			break;

		default:
			sys_err("Unknown guild war state");
			break;
	}
}

void CInputDB::GuildWarScore(const char* c_pData)
{
	TPacketGuildWarScore* p = (TPacketGuildWarScore*) c_pData;
	CGuild * g = CGuildManager::instance().TouchGuild(p->dwGuildGainPoint);
	g->SetWarScoreAgainstTo(p->dwGuildOpponent, p->lScore);
}

void CInputDB::GuildSkillRecharge()
{
	CGuildManager::instance().SkillRecharge();
}

void CInputDB::GuildExpUpdate(const char* c_pData)
{
	TPacketGuildSkillUpdate * p = (TPacketGuildSkillUpdate *) c_pData;
	sys_log(1, "GuildExpUpdate %d", p->amount);

	CGuild * g = CGuildManager::instance().TouchGuild(p->guild_id);

	if (g)
		g->GuildPointChange(POINT_EXP, p->amount);
}

void CInputDB::GuildAddMember(const char* c_pData)
{
	TPacketDGGuildMember * p = (TPacketDGGuildMember *) c_pData;
	CGuild * g = CGuildManager::instance().TouchGuild(p->dwGuild);

	if (g)
		g->AddMember(p);
}

void CInputDB::GuildRemoveMember(const char* c_pData)
{
	TPacketGuild* p=(TPacketGuild*)c_pData;
	CGuild* g = CGuildManager::instance().TouchGuild(p->dwGuild);

	if (g)
		g->RemoveMember(p->dwInfo);
}

void CInputDB::GuildChangeGrade(const char* c_pData)
{
	TPacketGuild* p=(TPacketGuild*)c_pData;
	CGuild* g = CGuildManager::instance().TouchGuild(p->dwGuild);

	if (g)
		g->P2PChangeGrade((BYTE)p->dwInfo);
}

void CInputDB::GuildChangeMemberData(const char* c_pData)
{
	sys_log(0, "Recv GuildChangeMemberData");
	TPacketGuildChangeMemberData * p = (TPacketGuildChangeMemberData *) c_pData;
	CGuild * g = CGuildManager::instance().TouchGuild(p->guild_id);

	if (g)
		g->ChangeMemberData(p->pid, p->offer, p->level, p->grade);
}

void CInputDB::GuildDisband(const char* c_pData)
{
	TPacketGuild * p = (TPacketGuild*) c_pData;
	CGuildManager::instance().DisbandGuild(p->dwGuild);
}

void CInputDB::GuildLadder(const char* c_pData)
{
	TPacketGuildLadder* p = (TPacketGuildLadder*) c_pData;
	sys_log(0, "Recv GuildLadder %u %d / w %d d %d l %d", p->dwGuild, p->lLadderPoint, p->lWin, p->lDraw, p->lLoss);
	CGuild * g = CGuildManager::instance().TouchGuild(p->dwGuild);

	g->SetLadderPoint(p->lLadderPoint);
	g->SetWarData(p->lWin, p->lDraw, p->lLoss);
}

void CInputDB::ItemLoad(LPDESC d, const char * c_pData)
{
	LPCHARACTER ch;

	if (!d || !(ch = d->GetCharacter()))
		return;

	if (ch->IsItemLoaded())
		return;

#ifdef __BL_BATTLE_ROYALE__
	if (ch->IsBattleRoyale())
	{
		ch->SetItemLoaded();
		return;
	}
#endif

	DWORD dwCount = decode_4bytes(c_pData);
	c_pData += sizeof(DWORD);

	sys_log(0, "ITEM_LOAD: COUNT %s %u", ch->GetName(), dwCount);

	std::vector<LPITEM> v;

	TPlayerItem * p = (TPlayerItem *) c_pData;

	for (DWORD i = 0; i < dwCount; ++i, ++p)
	{
#ifdef __SOULBINDING_SYSTEM__
		LPITEM item = ITEM_MANAGER::instance().CreateItem(p->vnum, p->count, p->id, false, -1, false, p->bind);
#else
		LPITEM item = ITEM_MANAGER::instance().CreateItem(p->vnum, p->count, p->id);
#endif

		if (!item)
		{
			sys_err("cannot create item by vnum %u (name %s id %u)", p->vnum, ch->GetName(), p->id);
			continue;
		}

        item->SetSkipSave(true);
#ifdef USE_EXPANDED_ITEM_REFRESH
        item->SetSkipPacket(true);
#endif

		item->SetSockets(p->alSockets);

		item->SetAttributes(p->aAttr);
		item->SetLastOwnerPID(p->owner);
#ifdef ENABLE_NEW_NAME_ITEM
		item->SetNewName(p->name);
#endif
#ifdef ENABLE_YOHARA_SYSTEM
		item->SetRandomAttrs(p->aApplyRandom);
#endif
#ifdef ENABLE_BELT_INVENTORY_EX
		if (p->window == BELT_INVENTORY)
		{
			p->window = INVENTORY;
			p->pos = p->pos + BELT_INVENTORY_SLOT_START;
		}
#endif

		if ((p->window == INVENTORY && ch->GetInventoryItem(p->pos)) ||
				(p->window == EQUIPMENT && ch->GetWear(p->pos)))
		{
			sys_log(0, "ITEM_RESTORE: %s %s", ch->GetName(), item->GetName());
			v.push_back(item);
		}
		else
		{
			switch (p->window)
			{
				case INVENTORY:
				case DRAGON_SOUL_INVENTORY:
#ifdef ENABLE_SPECIAL_STORAGE
				case UPGRADE_INVENTORY:
				case BOOK_INVENTORY:
				case STONE_INVENTORY:
				case CHANGE_INVENTORY:
				case COSTUME_INVENTORY:
				case CHEST_INVENTORY:
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
				case SWITCHBOT:
#endif
#ifdef CHANGE_EQUIP_WORLDARD
				case CHANGE_EQUIP:
#endif
				{
					item->AddToCharacter(ch, TItemPos(p->window, p->pos));
					break;
				}
				case EQUIPMENT:
				{
					if (item->CheckItemUseLevel(ch->GetLevel()
#ifdef ENABLE_YOHARA_SYSTEM
, ch->GetConquerorLevel()
#endif
					) == true)
					{
						if (item->EquipTo(ch, p->pos) == false )
						{
							v.push_back(item);
						}
					}
					else
					{
						v.push_back(item);
					}

					break;
				}
				default:
				{
					break;
				}
			}
		}

		if (false == item->OnAfterCreatedItem())
			sys_err("Failed to call ITEM::OnAfterCreatedItem (vnum: %d, id: %d)", item->GetVnum(), item->GetID());

		item->SetSkipSave(false);
#ifdef USE_EXPANDED_ITEM_REFRESH
        item->SetSkipPacket(false);
#endif
	}

	itertype(v) it = v.begin();

	while (it != v.end())
	{
		LPITEM item = *(it++);

		int pos = ch->GetEmptyInventory(item->GetSize());

		if (pos < 0)
		{
			PIXEL_POSITION coord;
			coord.x = ch->GetX();
			coord.y = ch->GetY();

			item->AddToGround(ch->GetMapIndex(), coord);
			item->SetOwnership(ch, 180);
			item->StartDestroyEvent();
		}
		else
			item->AddToCharacter(ch, TItemPos(INVENTORY, pos));
	}

	ch->CheckMaximumPoints();
	ch->PointsPacket();

	ch->SetItemLoaded();
}



void CInputDB::AffectLoad(LPDESC d, const char * c_pData)
{
	if (!d)
		return;

	if (!d->GetCharacter())
		return;

	LPCHARACTER ch = d->GetCharacter();

	DWORD dwPID = decode_4bytes(c_pData);
	c_pData += sizeof(DWORD);

	DWORD dwCount = decode_4bytes(c_pData);
	c_pData += sizeof(DWORD);

	if (ch->GetPlayerID() != dwPID)
		return;

	ch->LoadAffect(dwCount, (TPacketAffectElement *) c_pData);
}



void CInputDB::PartyCreate(const char* c_pData)
{
	TPacketPartyCreate* p = (TPacketPartyCreate*) c_pData;
	CPartyManager::instance().P2PCreateParty(p->dwLeaderPID);
}

void CInputDB::PartyDelete(const char* c_pData)
{
	TPacketPartyDelete* p = (TPacketPartyDelete*) c_pData;
	CPartyManager::instance().P2PDeleteParty(p->dwLeaderPID);
}

void CInputDB::PartyAdd(const char* c_pData)
{
	TPacketPartyAdd* p = (TPacketPartyAdd*) c_pData;
	CPartyManager::instance().P2PJoinParty(p->dwLeaderPID, p->dwPID, p->bState);
}

void CInputDB::PartyRemove(const char* c_pData)
{
	TPacketPartyRemove* p = (TPacketPartyRemove*) c_pData;
	CPartyManager::instance().P2PQuitParty(p->dwPID);
}

void CInputDB::PartyStateChange(const char* c_pData)
{
	TPacketPartyStateChange * p = (TPacketPartyStateChange *) c_pData;
	LPPARTY pParty = CPartyManager::instance().P2PCreateParty(p->dwLeaderPID);

	if (!pParty)
		return;

	pParty->SetRole(p->dwPID, p->bRole, p->bFlag);
}

void CInputDB::PartySetMemberLevel(const char* c_pData)
{
	TPacketPartySetMemberLevel* p = (TPacketPartySetMemberLevel*) c_pData;
	LPPARTY pParty = CPartyManager::instance().P2PCreateParty(p->dwLeaderPID);

	if (!pParty)
		return;

	pParty->P2PSetMemberLevel(p->dwPID, p->bLevel);
}

void CInputDB::Time(const char * c_pData)
{
	set_global_time(*(time_t *) c_pData);
}

void CInputDB::ReloadProto(const char * c_pData)
{
	WORD wSize;

	/*
	 * Skill
	 */
	wSize = decode_2bytes(c_pData);
	c_pData += sizeof(WORD);
	if (wSize) CSkillManager::instance().Initialize((TSkillTable *) c_pData, wSize);
	c_pData += sizeof(TSkillTable) * wSize;

	/*
	 * Banwords
	 */

	wSize = decode_2bytes(c_pData);
	c_pData += sizeof(WORD);
	CBanwordManager::instance().Initialize((TBanwordTable *) c_pData, wSize);
	c_pData += sizeof(TBanwordTable) * wSize;

	/*
	 * ITEM
	 */
	wSize = decode_2bytes(c_pData);
	c_pData += 2;
	sys_log(0, "RELOAD: ITEM: %d", wSize);

	if (wSize)
	{
		ITEM_MANAGER::instance().Initialize((TItemTable *) c_pData, wSize);
		c_pData += wSize * sizeof(TItemTable);
	}

	/*
	 * MONSTER
	 */
	wSize = decode_2bytes(c_pData);
	c_pData += 2;
	sys_log(0, "RELOAD: MOB: %d", wSize);

	if (wSize)
	{
		CMobManager::instance().Initialize((TMobTable *) c_pData, wSize);
		c_pData += wSize * sizeof(TMobTable);
	}

	CMotionManager::instance().Build();

    CHARACTER_MANAGER::Instance().for_each_pc(std::mem_fn(&CHARACTER::ComputePoints));
}

void CInputDB::GuildSkillUsableChange(const char* c_pData)
{
	TPacketGuildSkillUsableChange* p = (TPacketGuildSkillUsableChange*) c_pData;

	CGuild* g = CGuildManager::instance().TouchGuild(p->dwGuild);

	g->SkillUsableChange(p->dwSkillVnum, p->bUsable?true:false);
}

void CInputDB::AuthLogin(LPDESC d, const char * c_pData)
{
	if (!d)
		return;

	BYTE bResult = *(BYTE *) c_pData;

	TPacketGCAuthSuccess ptoc;

	ptoc.bHeader = HEADER_GC_AUTH_SUCCESS;

	if (bResult)
	{
		ptoc.dwLoginKey = d->GetLoginKey();
	}
	else
	{
		ptoc.dwLoginKey = 0;
	}

	ptoc.bResult = bResult;

	d->Packet(&ptoc, sizeof(TPacketGCAuthSuccess));
	sys_log(0, "AuthLogin result %u key %u", bResult, d->GetLoginKey());
}

void CInputDB::ChangeEmpirePriv(const char* c_pData)
{
	TPacketDGChangeEmpirePriv* p = (TPacketDGChangeEmpirePriv*) c_pData;

	// ADD_EMPIRE_PRIV_TIME
	CPrivManager::instance().GiveEmpirePriv(p->empire, p->type, p->value, p->bLog, p->end_time_sec);
	// END_OF_ADD_EMPIRE_PRIV_TIME
}

/**
 * @version 05/06/08	Bang2ni - Áö¼Ó½Ã°£ Ãß°¡
 */
void CInputDB::ChangeGuildPriv(const char* c_pData)
{
	TPacketDGChangeGuildPriv* p = (TPacketDGChangeGuildPriv*) c_pData;

	// ADD_GUILD_PRIV_TIME
	CPrivManager::instance().GiveGuildPriv(p->guild_id, p->type, p->value, p->bLog, p->end_time_sec);
	// END_OF_ADD_GUILD_PRIV_TIME
}

void CInputDB::ChangeCharacterPriv(const char* c_pData)
{
	TPacketDGChangeCharacterPriv* p = (TPacketDGChangeCharacterPriv*) c_pData;
	CPrivManager::instance().GiveCharacterPriv(p->pid, p->type, p->value, p->bLog);
}

void CInputDB::MoneyLog(const char* c_pData)
{
	TPacketMoneyLog * p = (TPacketMoneyLog *) c_pData;

	if (p->type == 4) // QUEST_MONEY_LOG_SKIP
		return;

	if (g_bAuthServer ==true )
		return;

	LogManager::instance().MoneyLog(p->type, p->vnum, p->gold);
}

void CInputDB::GuildMoneyChange(const char* c_pData)
{
	TPacketDGGuildMoneyChange* p = (TPacketDGGuildMoneyChange*) c_pData;

	CGuild* g = CGuildManager::instance().TouchGuild(p->dwGuild);
	if (g)
	{
		g->RecvMoneyChange(p->iTotalGold);
	}
}

void CInputDB::GuildWithdrawMoney(const char* c_pData)
{
	TPacketDGGuildMoneyWithdraw* p = (TPacketDGGuildMoneyWithdraw*) c_pData;

	CGuild* g = CGuildManager::instance().TouchGuild(p->dwGuild);
	if (g)
	{
		g->RecvWithdrawMoneyGive(p->iChangeGold);
	}
}

void CInputDB::SetEventFlag(const char* c_pData)
{
	TPacketSetEventFlag* p = (TPacketSetEventFlag*) c_pData;
	quest::CQuestManager::instance().SetEventFlag(p->szFlagName, p->lValue);
}

void CInputDB::CreateObject(const char * c_pData)
{
	using namespace building;
	CManager::instance().LoadObject((TObject *) c_pData);
}

void CInputDB::DeleteObject(const char * c_pData)
{
	using namespace building;
	CManager::instance().DeleteObject(*(DWORD *) c_pData);
}

void CInputDB::UpdateLand(const char * c_pData)
{
	using namespace building;
	CManager::instance().UpdateLand((TLand *) c_pData);
}

////////////////////////////////////////////////////////////////////
// Billing
////////////////////////////////////////////////////////////////////
void CInputDB::BillingRepair(const char * c_pData)
{
	DWORD dwCount = *(DWORD *) c_pData;
	c_pData += sizeof(DWORD);

	TPacketBillingRepair * p = (TPacketBillingRepair *) c_pData;

	for (DWORD i = 0; i < dwCount; ++i, ++p)
	{
		CLoginData * pkLD = M2_NEW CLoginData;

		pkLD->SetKey(p->dwLoginKey);
		pkLD->SetLogin(p->szLogin);
		pkLD->SetIP(p->szHost);

		sys_log(0, "BILLING: REPAIR %s host %s", p->szLogin, p->szHost);
	}
}

void CInputDB::BillingExpire(const char * c_pData)
{
	TPacketBillingExpire * p = (TPacketBillingExpire *) c_pData;

	LPDESC d = DESC_MANAGER::instance().FindByLoginName(p->szLogin);

	if (!d)
		return;

	LPCHARACTER ch = d->GetCharacter();

	if (p->dwRemainSeconds <= 60)
	{
		int i = MAX(5, p->dwRemainSeconds);
		sys_log(0, "BILLING_EXPIRE: %s %u", p->szLogin, p->dwRemainSeconds);
		d->DelayedDisconnect(i);
	}
	else
	{
		if ((p->dwRemainSeconds - d->GetBillingExpireSecond()) > 60)
		{
			d->SetBillingExpireSecond(p->dwRemainSeconds);

			if (ch)
				ch->ChatPacket(CHAT_TYPE_INFO, "724 %d", (p->dwRemainSeconds / 60));
		}
	}
}

void CInputDB::BillingLogin(const char * c_pData)
{
	if (NULL == c_pData)
		return;

	TPacketBillingLogin * p;

	DWORD dwCount = *(DWORD *) c_pData;
	c_pData += sizeof(DWORD);

	p = (TPacketBillingLogin *) c_pData;

	for (DWORD i = 0; i < dwCount; ++i, ++p)
	{
		DBManager::instance().SetBilling(p->dwLoginKey, p->bLogin);
	}
}

void CInputDB::BillingCheck(const char * c_pData)
{
	DWORD size = *(DWORD *) c_pData;
	c_pData += sizeof(DWORD);

	for (DWORD i = 0; i < size; ++i)
	{
		DWORD dwKey = *(DWORD *) c_pData;
		c_pData += sizeof(DWORD);

		sys_log(0, "BILLING: NOT_LOGIN %u", dwKey);
		DBManager::instance().SetBilling(dwKey, 0, true);
	}
}

void CInputDB::Notice(const char * c_pData)
{
	char szBuf[256+1];
	strlcpy(szBuf, c_pData, sizeof(szBuf));

	sys_log(0, "InputDB:: Notice: %s", szBuf);

	//SendNotice(LC_TEXT(szBuf));
	SendNotice(szBuf);
}

void CInputDB::VCard(const char * c_pData)
{
	TPacketGDVCard * p = (TPacketGDVCard *) c_pData;

	sys_log(0, "VCARD: %u %s %s %s %s", p->dwID, p->szSellCharacter, p->szSellAccount, p->szBuyCharacter, p->szBuyAccount);

	std::unique_ptr<SQLMsg> pmsg(DBManager::instance().DirectQuery("SELECT sell_account, buy_account, time FROM vcard WHERE id=%u", p->dwID));
	if (pmsg->Get()->uiNumRows != 1)
	{
		sys_log(0, "VCARD_FAIL: no data");
		return;
	}

	MYSQL_ROW row = mysql_fetch_row(pmsg->Get()->pSQLResult);

	if (strcmp(row[0], p->szSellAccount))
	{
		sys_log(0, "VCARD_FAIL: sell account differ %s", row[0]);
		return;
	}

	if (!row[1] || *row[1])
	{
		sys_log(0, "VCARD_FAIL: buy account already exist");
		return;
	}

	int time = 0;
	str_to_number(time, row[2]);

	if (!row[2] || time < 0)
	{
		sys_log(0, "VCARD_FAIL: time null");
		return;
	}

	std::unique_ptr<SQLMsg> pmsg1(DBManager::instance().DirectQuery("UPDATE GameTime SET LimitTime=LimitTime+%d WHERE UserID='%s'", time, p->szBuyAccount));

	if (pmsg1->Get()->uiAffectedRows == 0 || pmsg1->Get()->uiAffectedRows == (uint32_t)-1)
	{
		sys_log(0, "VCARD_FAIL: cannot modify GameTime table");
		return;
	}

	std::unique_ptr<SQLMsg> pmsg2(DBManager::instance().DirectQuery("UPDATE vcard,GameTime SET sell_pid='%s', buy_pid='%s', buy_account='%s', sell_time=NOW(), new_time=GameTime.LimitTime WHERE vcard.id=%u AND GameTime.UserID='%s'", p->szSellCharacter, p->szBuyCharacter, p->szBuyAccount, p->dwID, p->szBuyAccount));

	if (pmsg2->Get()->uiAffectedRows == 0 || pmsg2->Get()->uiAffectedRows == (uint32_t)-1)
	{
		sys_log(0, "VCARD_FAIL: cannot modify vcard table");
		return;
	}

	sys_log(0, "VCARD_SUCCESS: %s %s", p->szBuyAccount, p->szBuyCharacter);
}

void CInputDB::GuildWarReserveAdd(TGuildWarReserve * p)
{
	CGuildManager::instance().ReserveWarAdd(p);
}

void CInputDB::GuildWarReserveDelete(DWORD dwID)
{
	CGuildManager::instance().ReserveWarDelete(dwID);
}

void CInputDB::GuildWarBet(TPacketGDGuildWarBet * p)
{
	CGuildManager::instance().ReserveWarBet(p);
}

void CInputDB::MarriageAdd(TPacketMarriageAdd * p)
{
	sys_log(0, "MarriageAdd %u %u %u %s %s", p->dwPID1, p->dwPID2, (DWORD)p->tMarryTime, p->szName1, p->szName2);
	marriage::CManager::instance().Add(p->dwPID1, p->dwPID2, p->tMarryTime, p->szName1, p->szName2);
}

void CInputDB::MarriageUpdate(TPacketMarriageUpdate * p)
{
	sys_log(0, "MarriageUpdate %u %u %d %d", p->dwPID1, p->dwPID2, p->iLovePoint, p->byMarried);
	marriage::CManager::instance().Update(p->dwPID1, p->dwPID2, p->iLovePoint, p->byMarried);
}

void CInputDB::MarriageRemove(TPacketMarriageRemove * p)
{
	sys_log(0, "MarriageRemove %u %u", p->dwPID1, p->dwPID2);
	marriage::CManager::instance().Remove(p->dwPID1, p->dwPID2);
}

void CInputDB::WeddingRequest(TPacketWeddingRequest* p)
{
	marriage::WeddingManager::instance().Request(p->dwPID1, p->dwPID2);
}

void CInputDB::WeddingReady(TPacketWeddingReady* p)
{
	sys_log(0, "WeddingReady %u %u %u", p->dwPID1, p->dwPID2, p->dwMapIndex);
	marriage::CManager::instance().WeddingReady(p->dwPID1, p->dwPID2, p->dwMapIndex);
}

void CInputDB::WeddingStart(TPacketWeddingStart* p)
{
	sys_log(0, "WeddingStart %u %u", p->dwPID1, p->dwPID2);
	marriage::CManager::instance().WeddingStart(p->dwPID1, p->dwPID2);
}

void CInputDB::WeddingEnd(TPacketWeddingEnd* p)
{
	sys_log(0, "WeddingEnd %u %u", p->dwPID1, p->dwPID2);
	marriage::CManager::instance().WeddingEnd(p->dwPID1, p->dwPID2);
}
//RELOAD_ADMIN
void CInputDB::ReloadAdmin(const char * c_pData )
{
	gm_new_clear();
	int ChunkSize = decode_2bytes(c_pData );
	c_pData += 2;
	int HostSize = decode_2bytes(c_pData );
	c_pData += 2;

	for (int n = 0; n < HostSize; ++n )
	{
		gm_new_host_inert(c_pData );
		c_pData += ChunkSize;
	}


	c_pData += 2;
	int size = 	decode_2bytes(c_pData );
	c_pData += 2;

	for (int n = 0; n < size; ++n )
	{
		tAdminInfo& rAdminInfo = *(tAdminInfo*)c_pData;

		gm_new_insert(rAdminInfo );

		c_pData += sizeof (tAdminInfo );

		LPCHARACTER pChar = CHARACTER_MANAGER::instance().FindPC(rAdminInfo.m_szName );
		if (pChar )
		{
			pChar->SetGMLevel();
		}
	}

}
#ifdef ENABLE_IKASHOP_RENEWAL
template <class T>
const T& Decode(const char*& data){
	auto obj = reinterpret_cast<const T*>(data);
	data += sizeof(T);
	return *obj;
}

void IkarusShopLoadTables(const char* data)
{
	// decoding subpacket
	const auto& subpack = Decode<ikashop::TSubPacketDGLoadTables>(data);	
	
	auto& manager = ikashop::GetManager();

	// decoding shop & items
	for (DWORD i = 0; i < subpack.shopcount; i++)
	{
		const auto& shopInfo = Decode<ikashop::TShopInfo>(data);
		auto shop = manager.PutsNewShop(shopInfo);

		for (DWORD j = 0; j < shopInfo.count; j++)
			shop->AddItem(Decode<ikashop::TShopItem>(data));
	}

	// decoding private offers
	for (DWORD i = 0; i < subpack.offercount; i++)
	{
		const auto& offerInfo = Decode<ikashop::TOfferInfo>(data);
		
		auto shop = manager.GetShopByOwnerID(offerInfo.ownerid);
		if (!shop)
		{
			sys_err("CANNOT FIND SHOP BY OWNERID (TOfferInfo) %d ", offerInfo.ownerid);
			continue;
		}

		auto offer = std::make_shared<ikashop::TOfferInfo>(offerInfo);
		shop->AddOffer(offer);
		manager.PutsNewOffer(offer);
	}

	// decoding auctions
	for (DWORD i = 0; i < subpack.auctioncount; i++)
	{
		const auto& auction = Decode<ikashop::TAuctionInfo>(data);
		manager.PutsAuction(auction);
		
	}

	// decoding auction offers
	for (DWORD i = 0; i < subpack.auctionoffercount; i++)
	{
		const auto& offer = Decode<ikashop::TAuctionOfferInfo>(data);
		manager.PutsAuctionOffer(offer);
		
	}

#ifdef EXTEND_IKASHOP_ULTIMATE
	auto saleHistoryCount = Decode<DWORD>(data);
	for(DWORD i=0; i < saleHistoryCount; i++)
		manager.RecvShopRegisterSaleHistoryDBPacket(Decode<ikashop::TSaleHistory>(data));
#endif
}

void IkarusShopBuyItemPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGBuyItem>(data);
	ikashop::GetManager().RecvShopBuyDBPacket(subpack.buyerid, subpack.ownerid, subpack.itemid);
}

void IkarusShopLockedBuyItemPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGLockedBuyItem>(data);
	ikashop::GetManager().RecvShopLockedBuyItemDBPacket(subpack.buyerid, subpack.ownerid, subpack.itemid);
}

void IkarusShopEditItemPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGEditItem>(data);
	ikashop::GetManager().RecvShopEditItemDBPacket(subpack.ownerid , subpack.itemid, subpack.price);
}

void IkarusShopRemoveItemPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGRemoveItem>(data);
	ikashop::GetManager().RecvShopRemoveItemDBPacket(subpack.ownerid , subpack.itemid, subpack.requester);
}

void IkarusShopAddItemPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGAddItem>(data);
	ikashop::GetManager().RecvShopAddItemDBPacket(subpack.ownerid, subpack.item);
}

void IkarusShopForceClosePacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGShopForceClose>(data);
	ikashop::GetManager().RecvShopForceCloseDBPacket(subpack.ownerid);
}

void IkarusShopShopCreateNewPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGShopCreateNew>(data);
	
	ikashop::GetManager().RecvShopCreateNewDBPacket(subpack.shop);
}

void IkarusShopShopChangeNamePacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGShopChangeName>(data);
	ikashop::GetManager().RecvShopChangeNameDBPacket(subpack.ownerid , subpack.name);
}

void IkarusShopOfferCreatePacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGOfferCreate>(data);
	ikashop::GetManager().RecvShopOfferNewDBPacket(subpack.offer);
}

void IkarusShopOfferNotifiedPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGOfferNotified>(data);
	ikashop::GetManager().RecvShopOfferNotifiedDBPacket(subpack.offerid , subpack.ownerid);
}

void IkarusShopOfferAcceptPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGOfferAccept>(data);
	ikashop::GetManager().RecvShopOfferAcceptDBPacket(subpack.offerid , subpack.ownerid);
}

void IkarusShopOfferCancelPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGOfferCancel>(data);
	ikashop::GetManager().RecvShopOfferCancelDBPacket(subpack.offerid , subpack.ownerid, subpack.removing);
}

void IkarusShopSafeboxAddItemPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGSafeboxAddItem>(data);
	ikashop::GetManager().RecvShopSafeboxAddItemDBPacket(subpack.ownerid , subpack.itemid , subpack.item);
}

void IkarusShopSafeboxAddValutesPacket(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGSafeboxAddValutes>(data);
	ikashop::GetManager().RecvShopSafeboxAddValutesDBPacket(subpack.ownerid , subpack.valute);
}

void IkarusShopSafeboxLoad(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGSafeboxLoad>(data);
	std::vector<ikashop::TShopPlayerItem> items;
	items.reserve(subpack.itemcount);

	for (DWORD i = 0; i < subpack.itemcount; i++)
		items.emplace_back(Decode<ikashop::TShopPlayerItem>(data));

	ikashop::GetManager().RecvShopSafeboxLoadDBPacket(subpack.ownerid , subpack.valute, items);
}

void IkarusShopSafeboxExpiredItem(const char* data) {
	const auto& subpack = Decode<ikashop::TSubPacketDGSafeboxExpiredItem>(data);
	ikashop::GetManager().RecvShopSafeboxExpiredItemDBPacket(subpack.ownerid, subpack.itemid);
}

void IkarusShopSafeboxGetItemConfirm(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGSafeboxGetItemConfirm>(data);
	ikashop::GetManager().RecvShopSafeboxGetItemConfirm(subpack.ownerid, subpack.itemid);
}

void IkarusShopAuctionCreate(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGAuctionCreate>(data);
	ikashop::GetManager().RecvAuctionCreateDBPacket(subpack.auction);
}

void IkarusShopAuctionAddOffer(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGAuctionAddOffer>(data);
	ikashop::GetManager().RecvAuctionAddOfferDBPacket(subpack.offer);
}

void IkarusShopAuctionExpired(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGAuctionExpired>(data);
	ikashop::GetManager().RecvAuctionExpiredDBPacket(subpack.ownerid);
}

void IkarusShopAuctionOfferSeen(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGAuctionOfferSeen>(data);
	ikashop::GetManager().RecvAuctionOfferSeenDBPacket(subpack.ownerid, subpack.buyerid, subpack.price);
}

void IkarusShopShopExpired(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGShopExpired>(data);
	ikashop::GetManager().RecvShopExpiredDBPacket(subpack.ownerid);
}

#ifdef EXTEND_IKASHOP_PRO
void IkarusShopNotificationLoad(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGNotificationLoad>(data);
	// reserving notifications vector
	std::vector<ikashop::TNotificationInfo> notifications;
	notifications.reserve(subpack.count);
	// fetching vector
	for(DWORD i=0; i < subpack.count; i++)
		notifications.emplace_back(Decode<ikashop::TNotificationInfo>(data));
	// registering to manager
	ikashop::GetManager().RecvNotificationLoadDBPacket(subpack.ownerid, notifications);
}

void IkarusShopNotificationForward(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGNotificationForward>(data);
	ikashop::GetManager().RecvNotificationForwardDBPacket(subpack.ownerid, subpack);
}

void IkarusShopRestoreDuration(const char* data)
{
	const auto& owner = Decode<DWORD>(data);
	ikashop::GetManager().RecvShopRestoreDurationDBPacket(owner);
}
#ifdef ENABLE_IKASHOP_ENTITIES
void IkarusShopMoveShopEntity(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGMoveShopEntity>(data);
	ikashop::GetManager().RecvMoveShopEntityDBPacket(subpack.owner, subpack.spawn);
}
#endif
#endif

#ifdef EXTEND_IKASHOP_ULTIMATE
void IkarusShopRegisterSaleHistory(const char* data)
{
	const auto& sale = Decode<ikashop::TSaleHistory>(data);
	ikashop::GetManager().RecvShopRegisterSaleHistoryDBPacket(sale);
}

void IkarusShopDecorationSet(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGShopDecorationSet>(data);
	ikashop::GetManager().RecvShopDecorationSetDBPacket(subpack.owner, subpack.index, subpack.time);
}

void IkarusShopMoveItem(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGShopMoveItem>(data);
	ikashop::GetManager().RecvShopMoveItemDBPacket(subpack.owner, subpack.itemid, subpack.destCell);
}

void IkarusShopUnlockCell(const char* data)
{
	const auto& subpack = Decode<ikashop::TSubPacketDGShopUnlockCell>(data);
	ikashop::GetManager().RecvShopUnlockCellDBPacket(subpack.owner, subpack.lockIndex);
}

#ifdef USE_OFFLINESHOP_SEARCH_V2
void IkarusShopSearchRefresh(const char* data)
{
    const auto& pid = Decode<uint32_t>(data);
    auto ch	= CHARACTER_MANAGER::instance().FindByPID(pid);
    if (ch)
    {
        ikashop::GetManager().ShopSearchUpdateResults(ch, true, true);
    }
}
#endif
#endif


void IkarusShopPacket(const char* data)
{
	const auto& pack = Decode<TPacketDGNewIkarusShop>(data);

	

	switch (pack.bSubHeader)
	{
	case ikashop::SUBHEADER_DG_LOAD_TABLES:
		IkarusShopLoadTables(data);
		return;

	case ikashop::SUBHEADER_DG_BUY_ITEM:
		IkarusShopBuyItemPacket(data);
		return;

	case ikashop::SUBHEADER_DG_LOCKED_BUY_ITEM:
		IkarusShopLockedBuyItemPacket(data);
		return;

	case ikashop::SUBHEADER_DG_EDIT_ITEM:
		IkarusShopEditItemPacket(data);
		return;
	case ikashop::SUBHEADER_DG_REMOVE_ITEM:
		IkarusShopRemoveItemPacket(data);
		return;

	case ikashop::SUBHEADER_DG_ADD_ITEM:
		IkarusShopAddItemPacket(data);
		return;

	case ikashop::SUBHEADER_DG_SHOP_FORCE_CLOSE:
		IkarusShopForceClosePacket(data);
		return;

	case ikashop::SUBHEADER_DG_SHOP_CREATE_NEW:
		IkarusShopShopCreateNewPacket(data);
		return;

	case ikashop::SUBHEADER_DG_SHOP_CHANGE_NAME:
		IkarusShopShopChangeNamePacket(data);
		return;

	case ikashop::SUBHEADER_DG_SHOP_EXPIRED:
		IkarusShopShopExpired(data);
		break;

	case ikashop::SUBHEADER_DG_OFFER_CREATE:
		IkarusShopOfferCreatePacket(data);
		return;

	case ikashop::SUBHEADER_DG_OFFER_NOTIFIED:
		IkarusShopOfferNotifiedPacket(data);
		return;

	case ikashop::SUBHEADER_DG_OFFER_ACCEPT:
		IkarusShopOfferAcceptPacket(data);
		return;
	
	case ikashop::SUBHEADER_DG_OFFER_CANCEL:
		IkarusShopOfferCancelPacket(data);
		return;

	case ikashop::SUBHEADER_DG_SAFEBOX_ADD_ITEM:
		IkarusShopSafeboxAddItemPacket(data);
		return;

	case ikashop::SUBHEADER_DG_SAFEBOX_ADD_VALUTES:
		IkarusShopSafeboxAddValutesPacket(data);
		return;

	case ikashop::SUBHEADER_DG_SAFEBOX_LOAD:
		IkarusShopSafeboxLoad(data);
		return;

	case ikashop::SUBHEADER_DG_SAFEBOX_EXPIRED_ITEM:
		IkarusShopSafeboxExpiredItem(data);
		return;

	case ikashop::SUBHEADER_DG_SAFEBOX_GET_ITEM_CONFIRM:
		IkarusShopSafeboxGetItemConfirm(data);
		return;

	case ikashop::SUBHEADER_DG_AUCTION_CREATE:
		IkarusShopAuctionCreate(data);
		return;

	case ikashop::SUBHEADER_DG_AUCTION_ADD_OFFER:
		IkarusShopAuctionAddOffer(data);
		return;

	case ikashop::SUBHEADER_DG_AUCTION_EXPIRED:
		IkarusShopAuctionExpired(data);
		return;

	case ikashop::SUBHEADER_DG_AUCTION_OFFER_SEEN:
		IkarusShopAuctionOfferSeen(data);
		return;

#ifdef EXTEND_IKASHOP_PRO
	case ikashop::SUBHEADER_DG_NOTIFICATION_LOAD:
		IkarusShopNotificationLoad(data);
		return;

	case ikashop::SUBHEADER_DG_NOTIFICATION_FORWARD:
		IkarusShopNotificationForward(data);
		return;

	case ikashop::SUBHEADER_DG_SHOP_RESTORE_DURATION:
		IkarusShopRestoreDuration(data);
		return;
#ifdef ENABLE_IKASHOP_ENTITIES
	case ikashop::SUBHEADER_DG_MOVE_SHOP_ENTITY:
		IkarusShopMoveShopEntity(data);
		return;
#endif
#endif

#ifdef EXTEND_IKASHOP_ULTIMATE
	case ikashop::SUBHEADER_DG_SALE_HISTORY_REGISTER:
		IkarusShopRegisterSaleHistory(data);
		return;

	case ikashop::SUBHEADER_DG_SHOP_DECORATION_SET:
		IkarusShopDecorationSet(data);
		return;

	case ikashop::SUBHEADER_DG_SHOP_MOVE_ITEM:
		IkarusShopMoveItem(data);
		return;

	case ikashop::SUBHEADER_DG_SHOP_UNLOCK_CELL:
		IkarusShopUnlockCell(data);
		return;
#endif
#ifdef USE_OFFLINESHOP_SEARCH_V2
    case ikashop::SUBHEADER_DG_REFRESH_SHOPSEARCH:
    {
        IkarusShopSearchRefresh(data);
        return;
    }
#endif
	default:
		sys_err("UKNOWN SUB HEADER %d ", pack.bSubHeader);
		return;
	}
}
#endif

#ifdef ENABLE_AUTOMATIC_EVENT
struct FWarpAllToVillage
{
	FWarpAllToVillage() {};
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->IsPC() && ch->IsGM() == false)
			{
				BYTE bEmpire = ch->GetEmpire();
				if (bEmpire == 0)
				{
					sys_err("Unkonwn Empire %s %d ", ch->GetName(), ch->GetPlayerID());
					return;
				}

				ch->WarpSet(g_start_position[bEmpire][0], g_start_position[bEmpire][1]);
			}
		}
	}
};

void WarpAllToVillage(DWORD dwMapIndex)
{
	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(dwMapIndex);
	if (NULL != pSecMap) {
		FWarpAllToVillage f;
		pSecMap->for_each(f);
	}
}

void CInputDB::AutomaticEvent(TPacketDGAutomaticEvent * data)
{
	if (data->bStatus == true)
	{
		
	}
	else if (data->bStatus == false)
	{
		if (data->dwVnum == 10)
		{
			if (map_allow_find(24) && map_allow_find(44)){
				WarpAllToVillage(24);
				WarpAllToVillage(44);
			}
		}
	}
}

void CInputDB::MessageAutomaticEvent(TPacketDGMessageAutomaticEvent * data)
{
	char strMessage[100];
	snprintf(strMessage, sizeof(strMessage), "MESSAGE_%d_%d", data->dwMessageID, data->dwMessageType);
	strlcpy(strMessage, LC_TEXT(strMessage), sizeof(strMessage));

	if (data->dwMessageType == 0 || data->dwMessageType == 1)
	{
		// SendNotice(strMessage);
	}
	else if (data->dwMessageType == 2)
	{	
		char strTime[100];
		if (data->dwDays > 0)
			snprintf(strTime, sizeof(strTime), "725 %d %d %d", data->dwDays, data->dwHours, data->dwMinutes);
		else if(data->dwDays == 0 && data->dwHours > 0)
			snprintf(strTime, sizeof(strTime), "726 %d %d", data->dwHours, data->dwMinutes);
		else if (data->dwDays == 0 && data->dwHours == 0 && data->dwMinutes > 0)
			snprintf(strTime, sizeof(strTime), "727 %d", data->dwMinutes);
		else if(data->dwMinutes <= 0)
			return;
	
		char strNotice[100];
		snprintf(strNotice, sizeof(strNotice), "%s %s", strMessage, strTime);
		SendNotice(strNotice);
	}
}

#endif

//END_RELOAD_ADMIN

#ifdef USE_DUNGEON_COOLDOWN_MANAGER
void CInputDB::DungeonCooldownAdd(const TPacketDungeonCooldownMgrSet* p)
{
	if (!p)
	{
		return;
	}

	auto& dungeonCooldownMgr = CDungeonCooldownMgr::Instance();
	dungeonCooldownMgr.RecvAdd(p);
}

void CInputDB::DungeonCooldownExpire(const char* c_pData)
{
	const int maxsize = decode_4bytes(c_pData);
	if (maxsize == 0)
	{
		sys_err("Something went wrong with the maxsize (%d).", maxsize);
		return;
	}

	const auto* p = reinterpret_cast<const TPacketDungeonCooldownMgrExpire*>(c_pData + sizeof(int));
	if (!p)
	{
		return;
	}

	auto& dungeonCooldownMgr = CDungeonCooldownMgr::Instance();
	for (int i = 0; i < maxsize; ++i)
	{
		dungeonCooldownMgr.RecvExpire(p[i].key);
	}
}
#endif

#ifdef USE_DAY_CHANGE_MGR
void CInputDB::DayChangeProcess(const int* p)
{
	if (!p)
	{
		return;
	}

	CDayChangeMgr::Instance().OnChange(*p);
}
#endif

////////////////////////////////////////////////////////////////////
// Analyze
// @version	05/06/10 Bang2ni - ¾ÆÀÌÅÛ °¡°ÝÁ¤º¸ ¸®½ºÆ® ÆÐÅ¶(HEADER_DG_MYSHOP_PRICELIST_RES) Ã³¸®·çÆ¾ Ãß°¡.
////////////////////////////////////////////////////////////////////
int CInputDB::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)
{
	switch (bHeader)
	{
	case HEADER_DG_BOOT:
		Boot(c_pData);
		break;

	case HEADER_DG_LOGIN_SUCCESS:
		LoginSuccess(m_dwHandle, c_pData);
		break;
	case HEADER_DG_LOGIN_NOT_EXIST:
		LoginFailure(DESC_MANAGER::instance().FindByHandle(m_dwHandle), "NOID");
		break;

	case HEADER_DG_LOGIN_WRONG_PASSWD:
		LoginFailure(DESC_MANAGER::instance().FindByHandle(m_dwHandle), "WRONGPWD");
		break;

	case HEADER_DG_LOGIN_ALREADY:
		LoginAlready(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_PLAYER_LOAD_SUCCESS:
		PlayerLoad(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_PLAYER_CREATE_SUCCESS:
		PlayerCreateSuccess(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_PLAYER_CREATE_FAILED:
		PlayerCreateFailure(DESC_MANAGER::instance().FindByHandle(m_dwHandle), 0);
		break;

	case HEADER_DG_PLAYER_CREATE_ALREADY:
		PlayerCreateFailure(DESC_MANAGER::instance().FindByHandle(m_dwHandle), 1);
		break;

	case HEADER_DG_PLAYER_DELETE_SUCCESS:
		PlayerDeleteSuccess(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_PLAYER_LOAD_FAILED:
		//sys_log(0, "PLAYER_LOAD_FAILED");
		break;

	case HEADER_DG_PLAYER_DELETE_FAILED:
		//sys_log(0, "PLAYER_DELETE_FAILED");
		PlayerDeleteFail(DESC_MANAGER::instance().FindByHandle(m_dwHandle));
		break;

	case HEADER_DG_ITEM_LOAD:
		ItemLoad(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_QUEST_LOAD:
		QuestLoad(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_AFFECT_LOAD:
		AffectLoad(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_SAFEBOX_LOAD:
		SafeboxLoad(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_SAFEBOX_CHANGE_SIZE:
		SafeboxChangeSize(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_SAFEBOX_WRONG_PASSWORD:
		SafeboxWrongPassword(DESC_MANAGER::instance().FindByHandle(m_dwHandle));
		break;

	case HEADER_DG_SAFEBOX_CHANGE_PASSWORD_ANSWER:
		SafeboxChangePasswordAnswer(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_MALL_LOAD:
		MallLoad(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_EMPIRE_SELECT:
		EmpireSelect(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_MAP_LOCATIONS:
		MapLocations(c_pData);
		break;

	case HEADER_DG_P2P:
		P2P(c_pData);
		break;

	case HEADER_DG_GUILD_SKILL_UPDATE:
		GuildSkillUpdate(c_pData);
		break;

	case HEADER_DG_GUILD_LOAD:
		GuildLoad(c_pData);
		break;

	case HEADER_DG_GUILD_SKILL_RECHARGE:
		GuildSkillRecharge();
		break;

	case HEADER_DG_GUILD_EXP_UPDATE:
		GuildExpUpdate(c_pData);
		break;

	case HEADER_DG_PARTY_CREATE:
		PartyCreate(c_pData);
		break;

	case HEADER_DG_PARTY_DELETE:
		PartyDelete(c_pData);
		break;

	case HEADER_DG_PARTY_ADD:
		PartyAdd(c_pData);
		break;

	case HEADER_DG_PARTY_REMOVE:
		PartyRemove(c_pData);
		break;

	case HEADER_DG_PARTY_STATE_CHANGE:
		PartyStateChange(c_pData);
		break;

	case HEADER_DG_PARTY_SET_MEMBER_LEVEL:
		PartySetMemberLevel(c_pData);
		break;

	case HEADER_DG_TIME:
		Time(c_pData);
		break;

	case HEADER_DG_GUILD_ADD_MEMBER:
		GuildAddMember(c_pData);
		break;

	case HEADER_DG_GUILD_REMOVE_MEMBER:
		GuildRemoveMember(c_pData);
		break;

	case HEADER_DG_GUILD_CHANGE_GRADE:
		GuildChangeGrade(c_pData);
		break;

	case HEADER_DG_GUILD_CHANGE_MEMBER_DATA:
		GuildChangeMemberData(c_pData);
		break;

	case HEADER_DG_GUILD_DISBAND:
		GuildDisband(c_pData);
		break;

	case HEADER_DG_RELOAD_PROTO:
		ReloadProto(c_pData);
		break;

	case HEADER_DG_GUILD_WAR:
		GuildWar(c_pData);
		break;

	case HEADER_DG_GUILD_WAR_SCORE:
		GuildWarScore(c_pData);
		break;

	case HEADER_DG_GUILD_LADDER:
		GuildLadder(c_pData);
		break;

	case HEADER_DG_GUILD_SKILL_USABLE_CHANGE:
		GuildSkillUsableChange(c_pData);
		break;

	case HEADER_DG_CHANGE_NAME:
		ChangeName(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_AUTH_LOGIN:
		AuthLogin(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;

	case HEADER_DG_CHANGE_EMPIRE_PRIV:
		ChangeEmpirePriv(c_pData);
		break;

	case HEADER_DG_CHANGE_GUILD_PRIV:
		ChangeGuildPriv(c_pData);
		break;

	case HEADER_DG_CHANGE_CHARACTER_PRIV:
		ChangeCharacterPriv(c_pData);
		break;

	case HEADER_DG_MONEY_LOG:
		MoneyLog(c_pData);
		break;

	case HEADER_DG_GUILD_WITHDRAW_MONEY_GIVE:
		GuildWithdrawMoney(c_pData);
		break;

	case HEADER_DG_GUILD_MONEY_CHANGE:
		GuildMoneyChange(c_pData);
		break;

	case HEADER_DG_SET_EVENT_FLAG:
		SetEventFlag(c_pData);
		break;

	case HEADER_DG_BILLING_REPAIR:
		BillingRepair(c_pData);
		break;

	case HEADER_DG_BILLING_EXPIRE:
		BillingExpire(c_pData);
		break;

	case HEADER_DG_BILLING_LOGIN:
		BillingLogin(c_pData);
		break;

	case HEADER_DG_BILLING_CHECK:
		BillingCheck(c_pData);
		break;

	case HEADER_DG_VCARD:
		VCard(c_pData);
		break;

	case HEADER_DG_CREATE_OBJECT:
		CreateObject(c_pData);
		break;

	case HEADER_DG_DELETE_OBJECT:
		DeleteObject(c_pData);
		break;

	case HEADER_DG_UPDATE_LAND:
		UpdateLand(c_pData);
		break;

	case HEADER_DG_NOTICE:
		Notice(c_pData);
		break;

	case HEADER_DG_GUILD_WAR_RESERVE_ADD:
		GuildWarReserveAdd((TGuildWarReserve *) c_pData);
		break;

	case HEADER_DG_GUILD_WAR_RESERVE_DEL:
		GuildWarReserveDelete(*(DWORD *) c_pData);
		break;

	case HEADER_DG_GUILD_WAR_BET:
		GuildWarBet((TPacketGDGuildWarBet *) c_pData);
		break;

	case HEADER_DG_MARRIAGE_ADD:
		MarriageAdd((TPacketMarriageAdd*) c_pData);
		break;

	case HEADER_DG_MARRIAGE_UPDATE:
		MarriageUpdate((TPacketMarriageUpdate*) c_pData);
		break;

	case HEADER_DG_MARRIAGE_REMOVE:
		MarriageRemove((TPacketMarriageRemove*) c_pData);
		break;

	case HEADER_DG_WEDDING_REQUEST:
		WeddingRequest((TPacketWeddingRequest*) c_pData);
		break;
#ifdef ENABLE_ITEMSHOP
	case HEADER_DG_ITEMSHOP:
	{
		ItemShop(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;
	}
#endif
	case HEADER_DG_WEDDING_READY:
		WeddingReady((TPacketWeddingReady*) c_pData);
		break;

	case HEADER_DG_WEDDING_START:
		WeddingStart((TPacketWeddingStart*) c_pData);
		break;

	case HEADER_DG_WEDDING_END:
		WeddingEnd((TPacketWeddingEnd*) c_pData);
		break;

#ifdef __FAKE_PLAYER__
	case HEADER_DG_FAKE_PLAYER:
		CFakePlayerManager::instance().DGFakePlayer(m_dwHandle, c_pData);
		break;
#endif

	// RELOAD_ADMIN
	case HEADER_DG_RELOAD_ADMIN:
		ReloadAdmin(c_pData );
		break;
	//END_RELOAD_ADMIN

#ifdef ENABLE_EVENT_MANAGER
	case HEADER_DG_EVENT_MANAGER:
		EventManager(c_pData);
		break;
#endif

	case HEADER_DG_ADD_MONARCH_MONEY:
		AddMonarchMoney(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData );
		break;

	case HEADER_DG_DEC_MONARCH_MONEY:
		DecMonarchMoney(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData );
		break;

	case HEADER_DG_TAKE_MONARCH_MONEY:
		TakeMonarchMoney(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData );
		break;

	case HEADER_DG_CHANGE_MONARCH_LORD_ACK :
		ChangeMonarchLord((TPacketChangeMonarchLordACK*)c_pData);
		break;

	case HEADER_DG_UPDATE_MONARCH_INFO :
		UpdateMonarchInfo((TMonarchInfo*)c_pData);
		break;

	case HEADER_DG_BLOCK_COUNTRY_IP:
		this->AddBlockCountryIp((TPacketBlockCountryIp *) c_pData);
		break;
	case HEADER_DG_BLOCK_EXCEPTION:
		this->BlockException((TPacketBlockException *) c_pData);
		break;

	case HEADER_DG_ACK_CHANGE_GUILD_MASTER :
		this->GuildChangeMaster((TPacketChangeGuildMaster*) c_pData);
		break;
	case HEADER_DG_ACK_SPARE_ITEM_ID_RANGE :
		ITEM_MANAGER::instance().SetMaxSpareItemID(*((TItemIDRangeTable*)c_pData) );
		break;

	case HEADER_DG_UPDATE_HORSE_NAME :
	case HEADER_DG_ACK_HORSE_NAME :
		CHorseNameManager::instance().UpdateHorseName(
				((TPacketUpdateHorseName*)c_pData)->dwPlayerID,
				((TPacketUpdateHorseName*)c_pData)->szHorseName);
		break;

	case HEADER_DG_NEED_LOGIN_LOG:
		DetailLog( (TPacketNeedLoginLogInfo*) c_pData );
		break;
	// µ¶ÀÏ ¼±¹° ±â´É Å×½ºÆ®
	case HEADER_DG_ITEMAWARD_INFORMER:
		ItemAwardInformer((TPacketItemAwardInfromer*) c_pData);
		break;
	case HEADER_DG_RESPOND_CHANNELSTATUS:
		RespondChannelStatus(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;
#ifdef ENABLE_IKASHOP_RENEWAL
	case HEADER_DG_NEW_OFFLINESHOP:
		IkarusShopPacket(c_pData);
		break;
#endif
#ifdef ENABLE_AUTOMATIC_EVENT
	case HEADER_DG_AUTOMATIC_EVENT:
		AutomaticEvent((TPacketDGAutomaticEvent*)c_pData);
		break;

	case HEADER_DG_MESSAGE_AUTOMATIC_EVENT:
		MessageAutomaticEvent((TPacketDGMessageAutomaticEvent*)c_pData);
		break;
#endif

#ifdef USE_DUNGEON_COOLDOWN_MANAGER
	case HEADER_DG_DUNGEON_COOLDOWN_MGR_ADD:
	{
		DungeonCooldownAdd(reinterpret_cast<const TPacketDungeonCooldownMgrSet*>(c_pData));
		break;
	}
	case HEADER_DG_DUNGEON_COOLDOWN_MGR_EXPIRE:
	{
		DungeonCooldownExpire(c_pData);
		break;
	}
#endif
#ifdef USE_DAY_CHANGE_MGR
	case HEADER_DG_DAY_CHANGE_PROCESS:
	{
		DayChangeProcess(reinterpret_cast<const int*>(c_pData));
		break;
	}
#endif
	default:
		return (-1);
	}

	return 0;
}

bool CInputDB::Process(LPDESC d, const void * orig, int bytes, int & r_iBytesProceed)
{
	const char *	c_pData = (const char *) orig;
	BYTE		bHeader, bLastHeader = 0;
	int			iSize;
	int			iLastPacketLen = 0;

	for (m_iBufferLeft = bytes; m_iBufferLeft > 0;)
	{
		if (m_iBufferLeft < 9)
			return true;

		bHeader		= *((BYTE *) (c_pData));	// 1
		m_dwHandle	= *((DWORD *) (c_pData + 1));	// 4
		iSize		= *((DWORD *) (c_pData + 5));	// 4

		sys_log(1, "DBCLIENT: header %d handle %d size %d bytes %d", bHeader, m_dwHandle, iSize, bytes);

		if (m_iBufferLeft - 9 < iSize)
			return true;

		const char * pRealData = (c_pData + 9);

		if (Analyze(d, bHeader, pRealData) < 0)
		{
			sys_err("in InputDB: UNKNOWN HEADER: %d, LAST HEADER: %d(%d), REMAIN BYTES: %d, DESC: %d",
					bHeader, bLastHeader, iLastPacketLen, m_iBufferLeft, d->GetSocket());

			//printdata((BYTE*) orig, bytes);
			//d->SetPhase(PHASE_CLOSE);
		}

		c_pData		+= 9 + iSize;
		m_iBufferLeft	-= 9 + iSize;
		r_iBytesProceed	+= 9 + iSize;

		iLastPacketLen	= 9 + iSize;
		bLastHeader	= bHeader;
	}

	return true;
}

void CInputDB::AddMonarchMoney(LPDESC d, const char * data )
{
	int Empire = *(int *) data;
	data += sizeof(int);

	int Money = *(int *) data;
	data += sizeof(int);

	CMonarch::instance().AddMoney(Money, Empire);

	DWORD pid = CMonarch::instance().GetMonarchPID(Empire);

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(pid);

	if (ch)
	{
		if (number(1, 100) > 95)
			ch->ChatPacket(CHAT_TYPE_INFO, "728 %s %u", EMPIRE_NAME(Empire), CMonarch::instance().GetMoney(Empire));
	}
}

void CInputDB::DecMonarchMoney(LPDESC d, const char * data)
{
	int Empire = *(int *) data;
	data += sizeof(int);

	int Money = *(int *) data;
	data += sizeof(int);

	CMonarch::instance().DecMoney(Money, Empire);

	DWORD pid = CMonarch::instance().GetMonarchPID(Empire);

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(pid);

	if (ch)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "729 %s %d", EMPIRE_NAME(Empire), CMonarch::instance().GetMoney(Empire));
	}
}

void CInputDB::TakeMonarchMoney(LPDESC d, const char * data)
{
	int Empire = *(int *) data;
	data += sizeof(int);

	int Money = *(int *) data;
	data += sizeof(int);

	if (!CMonarch::instance().DecMoney(Money, Empire))
	{
		if (!d)
			return;

		if (!d->GetCharacter())
			return;

		LPCHARACTER ch = d->GetCharacter();
		ch->ChatPacket(CHAT_TYPE_INFO, "730");
	}
}

void CInputDB::ChangeMonarchLord(TPacketChangeMonarchLordACK* info)
{
	char notice[256];
	snprintf(notice, sizeof(notice), "731 %s %s", EMPIRE_NAME(info->bEmpire), info->szName);
	SendNotice(notice);
}

void CInputDB::UpdateMonarchInfo(TMonarchInfo* info)
{
	CMonarch::instance().SetMonarchInfo(info);
	sys_log(0, "MONARCH INFO UPDATED");
}

void CInputDB::AddBlockCountryIp(TPacketBlockCountryIp * data)
{
	add_blocked_country_ip(data);
}

void CInputDB::BlockException(TPacketBlockException *data)
{
	block_exception(data);
}

void CInputDB::GuildChangeMaster(TPacketChangeGuildMaster* p)
{
	CGuildManager::instance().ChangeMaster(p->dwGuildID);
}

void CInputDB::DetailLog(const TPacketNeedLoginLogInfo* info)
{
	LPCHARACTER pChar = CHARACTER_MANAGER::instance().FindByPID( info->dwPlayerID );

	if (NULL != pChar)
	{
		LogManager::instance().DetailLoginLog(true, pChar);
	}
}

void CInputDB::ItemAwardInformer(TPacketItemAwardInfromer *data)
{
	LPDESC d = DESC_MANAGER::instance().FindByLoginName(data->login);	//loginÁ¤º¸

	if(d == NULL)
		return;
	else
	{
		if (d->GetCharacter())
		{
			LPCHARACTER ch = d->GetCharacter();
			ch->SetItemAward_vnum(data->vnum);	// ch ¿¡ ÀÓ½Ã ÀúÀåÇØ³ù´Ù°¡ QuestLoad ÇÔ¼ö¿¡¼­ Ã³¸®
			ch->SetItemAward_cmd(data->command);

			if(d->IsPhase(PHASE_GAME))			//°ÔÀÓÆäÀÌÁîÀÏ¶§
			{
				quest::CQuestManager::instance().ItemInformer(ch->GetPlayerID(),ch->GetItemAward_vnum());	//questmanager È£Ãâ
			}
		}
	}
}

void CInputDB::RespondChannelStatus(LPDESC desc, const char* pcData)
{
	if (!desc) {
		return;
	}
	const int nSize = decode_4bytes(pcData);
	pcData += sizeof(nSize);

	BYTE bHeader = HEADER_GC_RESPOND_CHANNELSTATUS;
	desc->BufferedPacket(&bHeader, sizeof(BYTE));
	desc->BufferedPacket(&nSize, sizeof(nSize));
	if (0 < nSize) {
		desc->BufferedPacket(pcData, sizeof(TChannelStatus)*nSize);
	}
	BYTE bSuccess = 1;
	desc->Packet(&bSuccess, sizeof(bSuccess));
	desc->SetChannelStatusRequested(false);
}

#ifdef ENABLE_ITEMSHOP
void CInputDB::ItemShop(LPDESC d, const char* c_pData)
{
	const BYTE subIndex = *(BYTE*)c_pData;
	c_pData += sizeof(BYTE);

	switch (subIndex)
	{
		case ITEMSHOP_LOAD:
		{
			CHARACTER_MANAGER::Instance().LoadItemShopData(c_pData);
			break;
		}
		case ITEMSHOP_LOG:
		{
			if (!d)
			{
				return;
			}

			CHARACTER_MANAGER::Instance().LoadItemShopLogReal(d->GetCharacter(), c_pData);
			break;
		}
		case ITEMSHOP_BUY:
		{
			if (!d)
			{
				return;
			}

			CHARACTER_MANAGER::Instance().LoadItemShopBuyReal(d->GetCharacter(), c_pData);
			break;
		}
		case ITEMSHOP_UPDATE_ITEM:
		{
			CHARACTER_MANAGER::Instance().UpdateItemShopItem(c_pData);
			break;
		}
		default:
		{
			sys_err("unknown subIndex %u", subIndex);
			return;
		}
	}
}
#endif

#ifdef ENABLE_EVENT_MANAGER
void CInputDB::EventManager(const char* c_pData)
{
	CHARACTER_MANAGER& chrMngr = CHARACTER_MANAGER::Instance();
	const BYTE subIndex = *(BYTE*)c_pData;
	c_pData += sizeof(BYTE);
	if (subIndex == EVENT_MANAGER_LOAD)
	{
		chrMngr.ClearEventData();
		const BYTE dayCount = *(BYTE*)c_pData;
		c_pData += sizeof(BYTE);

		const bool updateFromGameMaster = *(bool*)c_pData;
		c_pData += sizeof(bool);

		for (DWORD x = 0; x < dayCount; ++x)
		{
			const BYTE dayIndex = *(BYTE*)c_pData;
			c_pData += sizeof(BYTE);

			const BYTE dayEventCount = *(BYTE*)c_pData;
			c_pData += sizeof(BYTE);

			if (dayEventCount > 0)
			{
				std::vector<TEventManagerData> m_vec;
				m_vec.resize(dayEventCount);
				std::memcpy(&m_vec[0], c_pData, dayEventCount*sizeof(TEventManagerData));
				c_pData += dayEventCount * sizeof(TEventManagerData);
				chrMngr.SetEventData(dayIndex, m_vec);

			}
		}
		if (updateFromGameMaster)
			chrMngr.UpdateAllPlayerEventData();
	}
	else if (EVENT_MANAGER_EVENT_STATUS == subIndex)
	{
		const WORD& eventID = *(WORD*)c_pData;
		c_pData += sizeof(WORD);
		const bool& eventStatus = *(bool*)c_pData;
		c_pData += sizeof(bool);
		const int& endTime = *(int*)c_pData;
		c_pData += sizeof(int);
		char endTimeText[25];
		strlcpy(endTimeText, c_pData, sizeof(endTimeText));
		c_pData += sizeof(endTimeText);
		chrMngr.SetEventStatus(eventID, eventStatus, endTime, endTimeText);
	}
}
#endif

