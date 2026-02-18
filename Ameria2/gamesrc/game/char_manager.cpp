#include "stdafx.h"
#include "constants.h"
#include "utils.h"
#include "desc.h"
#include "char.h"
#include "char_manager.h"
#include "mob_manager.h"
#include "party.h"
#include "regen.h"
#include "p2p.h"
#include "dungeon.h"
#include "db.h"
#include "config.h"
#include "xmas_event.h"
#include "questmanager.h"
#include "questlua.h"
#include "locale_service.h"
#include "shutdown_manager.h"
#ifdef ENABLE_ITEMSHOP
#include "desc_client.h"
#include "desc_manager.h"
#endif
#ifdef ENABLE_ITEMSHOP
#include "item_manager.h"
#endif

#ifndef __GNUC__
#include <boost/bind.hpp>
#endif

CHARACTER_MANAGER::CHARACTER_MANAGER() :
	m_iVIDCount(0),
	m_pkChrSelectedStone(NULL),
	m_bUsePendingDestroy(false)
{
	RegisterRaceNum(xmas::MOB_XMAS_FIRWORK_SELLER_VNUM);
	RegisterRaceNum(xmas::MOB_SANTA_VNUM);
	RegisterRaceNum(xmas::MOB_XMAS_TREE_VNUM);

	m_iMobItemRate = 100;
	m_iMobDamageRate = 100;
	m_iMobGoldAmountRate = 100;
	m_iMobGoldDropRate = 100;
	m_iMobExpRate = 100;

	m_iMobItemRatePremium = 100;
	m_iMobGoldAmountRatePremium = 100;
	m_iMobGoldDropRatePremium = 100;
	m_iMobExpRatePremium = 100;

	m_iUserDamageRate = 100;
	m_iUserDamageRatePremium = 100;
#ifdef ENABLE_BATTLE_PASS
	m_BattlePassData.clear();
#endif
#ifdef ENABLE_EVENT_MANAGER
	m_eventData.clear();
#endif
#ifdef ENABLE_REWARD_SYSTEM
	m_rewardData.clear();
#endif
}

CHARACTER_MANAGER::~CHARACTER_MANAGER()
{
	Destroy();
}

void CHARACTER_MANAGER::Destroy()
{
#ifdef ENABLE_EVENT_MANAGER
	m_eventData.clear();
#endif

#ifdef ENABLE_ITEMSHOP
	m_IShopManager.clear();
#endif

#ifdef ENABLE_BATTLE_PASS
	m_BattlePassData.clear();
#endif

	itertype(m_map_pkChrByVID) it = m_map_pkChrByVID.begin();
	while (it != m_map_pkChrByVID.end()) {
		LPCHARACTER ch = it->second;
		M2_DESTROY_CHARACTER(ch); // m_map_pkChrByVID is changed here
		it = m_map_pkChrByVID.begin();
	}
#ifdef ENABLE_REWARD_SYSTEM
	m_rewardData.clear();
#endif
}

void CHARACTER_MANAGER::GracefulShutdown()
{
	auto it =m_map_pkPCChr.begin();

	while (it != m_map_pkPCChr.end())
		(it++)->second->Disconnect("GracefulShutdown");
}

DWORD CHARACTER_MANAGER::AllocVID()
{
	++m_iVIDCount;
	return m_iVIDCount;
}

LPCHARACTER CHARACTER_MANAGER::CreateCharacter(const char * name, DWORD dwPID)
{
	DWORD dwVID = AllocVID();

#ifdef M2_USE_POOL
	LPCHARACTER ch = pool_.Construct();
#else
	LPCHARACTER ch = M2_NEW CHARACTER;
#endif
	ch->Create(name, dwVID, dwPID ? true : false);

	m_map_pkChrByVID.insert(std::make_pair(dwVID, ch));

	if (dwPID)
	{
		char szName[CHARACTER_NAME_MAX_LEN + 1];
		str_lower(name, szName, sizeof(szName));

		m_map_pkPCChr.insert(NAME_MAP::value_type(szName, ch));
		m_map_pkChrByPID.insert(std::make_pair(dwPID, ch));
	}

	return (ch);
}

#ifndef DEBUG_ALLOC
void CHARACTER_MANAGER::DestroyCharacter(LPCHARACTER ch)
#else
void CHARACTER_MANAGER::DestroyCharacter(LPCHARACTER ch, const char* file, size_t line)
#endif
{
	if (!ch)
		return;

	// <Factor> Check whether it has been already deleted or not.
	itertype(m_map_pkChrByVID) it = m_map_pkChrByVID.find(ch->GetVID());
	if (it == m_map_pkChrByVID.end()) {
		sys_err("[CHARACTER_MANAGER::DestroyCharacter] <Factor> %d not found", (long)(ch->GetVID()));
		return; // prevent duplicated destrunction
	}

	// ´øÀü¿¡ ¼Ò¼ÓµÈ ¸ó½ºÅÍ´Â ´øÀü¿¡¼­µµ »èÁ¦ÇÏµµ·Ï.
#ifdef ENABLE_MOUNT_SYSTEM
	if (ch->IsNPC() && !ch->IsPet() && !ch->IsNewPet() && !ch->IsBuffNPC() && !ch->IsMountSystem() && ch->GetRider() == NULL)
#else
	if (ch->IsNPC() && !ch->IsPet() && ch->GetRider() == NULL)
#endif
	{
		if (ch->GetDungeon())
		{
			ch->GetDungeon()->DeadCharacter(ch);
		}
	}

	if (m_bUsePendingDestroy)
	{
		m_set_pkChrPendingDestroy.insert(ch);
		return;
	}

	m_map_pkChrByVID.erase(it);

	if (true == ch->IsPC() && false == ch->IsPet())
	{
		char szName[CHARACTER_NAME_MAX_LEN + 1];
		str_lower(ch->GetName(), szName, sizeof(szName));
		auto it = m_map_pkPCChr.find(szName);

		if (m_map_pkPCChr.end() != it)
			m_map_pkPCChr.erase(it);
	}

	if (0 != ch->GetPlayerID())
	{
		itertype(m_map_pkChrByPID) it = m_map_pkChrByPID.find(ch->GetPlayerID());

		if (m_map_pkChrByPID.end() != it)
		{
			m_map_pkChrByPID.erase(it);
		}
	}

	UnregisterRaceNumMap(ch);

	RemoveFromStateList(ch);

#ifdef M2_USE_POOL
	pool_.Destroy(ch);
#else
#ifndef DEBUG_ALLOC
	M2_DELETE(ch);
#else
	M2_DELETE_EX(ch, file, line);
#endif
#endif
}

LPCHARACTER CHARACTER_MANAGER::Find(DWORD dwVID)
{
	itertype(m_map_pkChrByVID) it = m_map_pkChrByVID.find(dwVID);

	if (m_map_pkChrByVID.end() == it)
		return NULL;

	// <Factor> Added sanity check
	LPCHARACTER found = it->second;
	if (found != NULL && dwVID != (DWORD)found->GetVID()) {
		sys_err("[CHARACTER_MANAGER::Find] <Factor> %u != %u", dwVID, (DWORD)found->GetVID());
		return NULL;
	}
	return found;
}

LPCHARACTER CHARACTER_MANAGER::Find(const VID & vid)
{
	LPCHARACTER tch = Find((DWORD) vid);

	if (!tch || tch->GetVID() != vid)
		return NULL;

	return tch;
}

LPCHARACTER CHARACTER_MANAGER::FindByPID(DWORD dwPID)
{
	itertype(m_map_pkChrByPID) it = m_map_pkChrByPID.find(dwPID);

	if (m_map_pkChrByPID.end() == it)
		return NULL;

	// <Factor> Added sanity check
	LPCHARACTER found = it->second;
	if (found != NULL && dwPID != found->GetPlayerID()) {
		sys_err("[CHARACTER_MANAGER::FindByPID] <Factor> %u != %u", dwPID, found->GetPlayerID());
		return NULL;
	}
	return found;
}

LPCHARACTER CHARACTER_MANAGER::FindPC(const char * name)
{
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	str_lower(name, szName, sizeof(szName));
	auto it =m_map_pkPCChr.find(szName);

	if (it == m_map_pkPCChr.end())
		return NULL;

	// <Factor> Added sanity check
	LPCHARACTER found = it->second;
	if (found != NULL && strncasecmp(szName, found->GetName(), CHARACTER_NAME_MAX_LEN) != 0) {
		sys_err("[CHARACTER_MANAGER::FindPC] <Factor> %s != %s", name, found->GetName());
		return NULL;
	}
	return found;
}

LPCHARACTER CHARACTER_MANAGER::SpawnMobRandomPosition(DWORD dwVnum, long lMapIndex)
{
	// ¿Ö±¸ ½ºÆùÇÒÁö¸»Áö¸¦ °áÁ¤ÇÒ ¼ö ÀÖ°ÔÇÔ
	{
		if (dwVnum == 5001 && !quest::CQuestManager::instance().GetEventFlag("japan_regen"))
		{
			sys_log(1, "WAEGU[5001] regen disabled.");
			return NULL;
		}
	}

	// ÇØÅÂ¸¦ ½ºÆùÇÒÁö ¸»Áö¸¦ °áÁ¤ÇÒ ¼ö ÀÖ°Ô ÇÔ
	{
		if (dwVnum == 5002 && !quest::CQuestManager::instance().GetEventFlag("newyear_mob"))
		{
			sys_log(1, "HAETAE (new-year-mob) [5002] regen disabled.");
			return NULL;
		}
	}

	// ±¤º¹Àý ÀÌº¥Æ®
	{
		if (dwVnum == 5004 && !quest::CQuestManager::instance().GetEventFlag("independence_day"))
		{
			sys_log(1, "INDEPENDECE DAY [5004] regen disabled.");
			return NULL;
		}
	}

	const CMob * pkMob = CMobManager::instance().Get(dwVnum);

	if (!pkMob)
	{
		sys_err("No mob data for vnum: %u (map index: %ld)", dwVnum, lMapIndex);
		return NULL;
	}

	if (!map_allow_find(lMapIndex))
	{
		sys_err("not allowed map %u", lMapIndex);
		return NULL;
	}

	LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(lMapIndex);
	if (pkSectreeMap == NULL) {
		return NULL;
	}

	int i;
	long x, y;
	for (i=0; i<2000; i++)
	{
		x = number(1, (pkSectreeMap->m_setting.iWidth / 100)  - 1) * 100 + pkSectreeMap->m_setting.iBaseX;
		y = number(1, (pkSectreeMap->m_setting.iHeight / 100) - 1) * 100 + pkSectreeMap->m_setting.iBaseY;
		//LPSECTREE tree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);
		LPSECTREE tree = pkSectreeMap->Find(x, y);

		if (!tree)
			continue;

		DWORD dwAttr = tree->GetAttribute(x, y);

		if (IS_SET(dwAttr, ATTR_BLOCK | ATTR_OBJECT))
			continue;

		if (IS_SET(dwAttr, ATTR_BANPK))
			continue;

		break;
	}

	if (i == 2000)
	{
		sys_err("cannot find valid location");
		return NULL;
	}

	LPSECTREE sectree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);

	if (!sectree)
	{
		sys_log(0, "SpawnMobRandomPosition: cannot create monster at non-exist sectree %d x %d (map %d)", x, y, lMapIndex);
		return NULL;
	}

	LPCHARACTER ch = CHARACTER_MANAGER::instance().CreateCharacter(pkMob->m_table.szLocaleName);

	if (!ch)
	{
		sys_log(0, "SpawnMobRandomPosition: cannot create new character");
		return NULL;
	}

	ch->SetProto(pkMob);

	// if mob is npc with no empire assigned, assign to empire of map
	if (pkMob->m_table.bType == CHAR_TYPE_NPC)
		if (ch->GetEmpire() == 0)
			ch->SetEmpire(SECTREE_MANAGER::instance().GetEmpireFromMapIndex(lMapIndex));

	ch->SetRotation(number(0, 360));

	if (!ch->Show(lMapIndex, x, y, 0, false))
	{
		M2_DESTROY_CHARACTER(ch);
		sys_err(0, "SpawnMobRandomPosition: cannot show monster");
		return NULL;
	}

	//char buf[512+1];
	//long local_x = x - pkSectreeMap->m_setting.iBaseX;
	//long local_y = y - pkSectreeMap->m_setting.iBaseY;
	// snprintf(buf, sizeof(buf), "spawn %s[%d] random position at %ld %ld %ld %ld (time: %d)", ch->GetName(), dwVnum, x, y, local_x, local_y, get_global_time());

	//if (test_server)
	//	SendNotice(buf);

	//sys_log(0, buf);
	return (ch);
}

LPCHARACTER CHARACTER_MANAGER::SpawnMob(DWORD dwVnum, long lMapIndex, long x, long y, long z, bool bSpawnMotion, int iRot, bool bShow)
{
	const CMob * pkMob = CMobManager::instance().Get(dwVnum);
	if (!pkMob)
	{
		sys_err("SpawnMob: no mob data for vnum %u", dwVnum);
		return NULL;
	}

	if (!(pkMob->m_table.bType == CHAR_TYPE_NPC || pkMob->m_table.bType == CHAR_TYPE_WARP || pkMob->m_table.bType == CHAR_TYPE_GOTO
#ifdef ENABLE_NEW_PET_SYSTEM
	|| pkMob->m_table.bType == CHAR_TYPE_PET
#endif
	
	) || mining::IsVeinOfOre (dwVnum))
	{
		LPSECTREE tree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);

		if (!tree)
		{
			sys_log(0, "no sectree for spawn at %d %d mobvnum %d mapindex %d", x, y, dwVnum, lMapIndex);
			return NULL;
		}

		DWORD dwAttr = tree->GetAttribute(x, y);

		bool is_set = false;

		if ( mining::IsVeinOfOre (dwVnum) ) is_set = IS_SET(dwAttr, ATTR_BLOCK);
		else is_set = IS_SET(dwAttr, ATTR_BLOCK | ATTR_OBJECT);

		if ( is_set )
		{
			// SPAWN_BLOCK_LOG
			static bool s_isLog=quest::CQuestManager::instance().GetEventFlag("spawn_block_log");
			static DWORD s_nextTime=get_global_time()+10000;

			DWORD curTime=get_global_time();

			if (curTime>s_nextTime)
			{
				s_nextTime=curTime;
				s_isLog=quest::CQuestManager::instance().GetEventFlag("spawn_block_log");

			}

			if (s_isLog)
				sys_log(0, "SpawnMob: BLOCKED position for spawn %s %u at %d %d (attr %u)", pkMob->m_table.szName, dwVnum, x, y, dwAttr);
			// END_OF_SPAWN_BLOCK_LOG
			return NULL;
		}

		if (IS_SET(dwAttr, ATTR_BANPK))
		{
			sys_log(0, "SpawnMob: BAN_PK position for mob spawn %s %u at %d %d", pkMob->m_table.szName, dwVnum, x, y);
			return NULL;
		}
	}

	LPSECTREE sectree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);

	if (!sectree)
	{
		sys_log(0, "SpawnMob: cannot create monster at non-exist sectree %d x %d (map %d)", x, y, lMapIndex);
		return NULL;
	}

	LPCHARACTER ch = CHARACTER_MANAGER::instance().CreateCharacter(pkMob->m_table.szLocaleName);

	if (!ch)
	{
		sys_log(0, "SpawnMob: cannot create new character");
		return NULL;
	}

	if (iRot == -1)
		iRot = number(0, 360);

	ch->SetProto(pkMob);

	// if mob is npc with no empire assigned, assign to empire of map
	if (pkMob->m_table.bType == CHAR_TYPE_NPC)
		if (ch->GetEmpire() == 0)
			ch->SetEmpire(SECTREE_MANAGER::instance().GetEmpireFromMapIndex(lMapIndex));

	ch->SetRotation(iRot);

	if (bShow && !ch->Show(lMapIndex, x, y, z, bSpawnMotion))
	{
		M2_DESTROY_CHARACTER(ch);
		sys_log(0, "SpawnMob: cannot show monster");
		return NULL;
	}

	return (ch);
}

LPCHARACTER CHARACTER_MANAGER::SpawnMobRange(DWORD dwVnum, long lMapIndex, int sx, int sy, int ex, int ey, bool bIsException, bool bSpawnMotion, bool bAggressive )
{
	const CMob * pkMob = CMobManager::instance().Get(dwVnum);

	if (!pkMob)
		return NULL;

	if (pkMob->m_table.bType == CHAR_TYPE_STONE
#ifdef ENABLE_DEFENSAWE_SHIP
	|| dwVnum == 6512
#endif
	)	// µ¹Àº ¹«Á¶°Ç SPAWN ¸ð¼ÇÀÌ ÀÖ´Ù.
		bSpawnMotion = true;

	int i = 16;

	while (i--)
	{
		int x = number(sx, ex);
		int y = number(sy, ey);
		/*
		   if (bIsException)
		   if (is_regen_exception(x, y))
		   continue;
		 */
		LPCHARACTER ch = SpawnMob(dwVnum, lMapIndex, x, y, 0, bSpawnMotion);

		if (ch)
		{
			sys_log(1, "MOB_SPAWN: %s(%d) %dx%d", ch->GetName(), (DWORD) ch->GetVID(), ch->GetX(), ch->GetY());
			if ( bAggressive )
				ch->SetAggressive();
			return (ch);
		}
	}

	return NULL;
}

void CHARACTER_MANAGER::SelectStone(LPCHARACTER pkChr)
{
	m_pkChrSelectedStone = pkChr;
}

bool CHARACTER_MANAGER::SpawnMoveGroup(DWORD dwVnum, long lMapIndex, int sx, int sy, int ex, int ey, int tx, int ty, LPREGEN pkRegen, bool bAggressive_)
{
	CMobGroup * pkGroup = CMobManager::Instance().GetGroup(dwVnum);

	if (!pkGroup)
	{
		sys_err("NOT_EXIST_GROUP_VNUM(%u) Map(%u) ", dwVnum, lMapIndex);
		return false;
	}

	LPCHARACTER pkChrMaster = NULL;
	LPPARTY pkParty = NULL;

	const std::vector<DWORD> & c_rdwMembers = pkGroup->GetMemberVector();

	bool bSpawnedByStone = false;
	bool bAggressive = bAggressive_;

	if (m_pkChrSelectedStone)
	{
		bSpawnedByStone = true;
		if (m_pkChrSelectedStone->GetDungeon())
			bAggressive = true;
	}

	for (DWORD i = 0; i < c_rdwMembers.size(); ++i)
	{
		LPCHARACTER tch = SpawnMobRange(c_rdwMembers[i], lMapIndex, sx, sy, ex, ey, true, bSpawnedByStone);

		if (!tch)
		{
			if (i == 0)	// ¸ø¸¸µç ¸ó½ºÅÍ°¡ ´ëÀåÀÏ °æ¿ì¿¡´Â ±×³É ½ÇÆÐ
				return false;

			continue;
		}

		sx = tch->GetX() - number(300, 500);
		sy = tch->GetY() - number(300, 500);
		ex = tch->GetX() + number(300, 500);
		ey = tch->GetY() + number(300, 500);

		if (m_pkChrSelectedStone)
			tch->SetStone(m_pkChrSelectedStone);
		else if (pkParty)
		{
			pkParty->Join(tch->GetVID());
			pkParty->Link(tch);
		}
		else if (!pkChrMaster)
		{
			pkChrMaster = tch;
			pkChrMaster->SetRegen(pkRegen);

			pkParty = CPartyManager::instance().CreateParty(pkChrMaster);
		}
		if (bAggressive)
			tch->SetAggressive();

		if (tch->Goto(tx, ty))
			tch->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	}

	return true;
}

bool CHARACTER_MANAGER::SpawnGroupGroup(DWORD dwVnum, long lMapIndex, int sx, int sy, int ex, int ey, LPREGEN pkRegen, bool bAggressive_, LPDUNGEON pDungeon)
{
	const DWORD dwGroupID = CMobManager::Instance().GetGroupFromGroupGroup(dwVnum);

	if( dwGroupID != 0 )
	{
		return SpawnGroup(dwGroupID, lMapIndex, sx, sy, ex, ey, pkRegen, bAggressive_, pDungeon);
	}
	else
	{
		sys_err( "NOT_EXIST_GROUP_GROUP_VNUM(%u) MAP(%ld)", dwVnum, lMapIndex );
		return false;
	}
}

LPCHARACTER CHARACTER_MANAGER::SpawnGroup(DWORD dwVnum, long lMapIndex, int sx, int sy, int ex, int ey, LPREGEN pkRegen, bool bAggressive_, LPDUNGEON pDungeon)
{
	CMobGroup * pkGroup = CMobManager::Instance().GetGroup(dwVnum);

	if (!pkGroup)
	{
		sys_err("NOT_EXIST_GROUP_VNUM(%u) Map(%u) ", dwVnum, lMapIndex);
		return NULL;
	}

	LPCHARACTER pkChrMaster = NULL;
	LPPARTY pkParty = NULL;

	const std::vector<DWORD> & c_rdwMembers = pkGroup->GetMemberVector();

	bool bSpawnedByStone = false;
	bool bAggressive = bAggressive_;

	if (m_pkChrSelectedStone)
	{
		bSpawnedByStone = true;

		if (m_pkChrSelectedStone->GetDungeon())
			bAggressive = true;
	}

	LPCHARACTER chLeader = NULL;

	for (DWORD i = 0; i < c_rdwMembers.size(); ++i)
	{
		LPCHARACTER tch = SpawnMobRange(c_rdwMembers[i], lMapIndex, sx, sy, ex, ey, true, bSpawnedByStone);

		if (!tch)
		{
			if (i == 0)	// ¸ø¸¸µç ¸ó½ºÅÍ°¡ ´ëÀåÀÏ °æ¿ì¿¡´Â ±×³É ½ÇÆÐ
				return NULL;

			continue;
		}

		if (i == 0)
			chLeader = tch;

		tch->SetDungeon(pDungeon);

		sx = tch->GetX() - number(300, 500);
		sy = tch->GetY() - number(300, 500);
		ex = tch->GetX() + number(300, 500);
		ey = tch->GetY() + number(300, 500);

		if (m_pkChrSelectedStone)
			tch->SetStone(m_pkChrSelectedStone);
		else if (pkParty)
		{
			pkParty->Join(tch->GetVID());
			pkParty->Link(tch);
		}
		else if (!pkChrMaster)
		{
			pkChrMaster = tch;
			pkChrMaster->SetRegen(pkRegen);

			pkParty = CPartyManager::instance().CreateParty(pkChrMaster);
		}

		if (bAggressive)
			tch->SetAggressive();
		
#ifdef ENABLE_SUNG_MAHI_TOWER
		if (tch && pDungeon)
		{
			BYTE bDungeonLevel = pDungeon->GetDungeonDifficulty();
			tch->SetDungeonMultipliers(bDungeonLevel);
		}
#endif
	}

	return chLeader;
}

struct FuncUpdateAndResetChatCounter
{
	void operator () (LPCHARACTER ch)
	{
		ch->ResetChatCounter();
		ch->CFSM::Update();
	}
};

void CHARACTER_MANAGER::Update(int iPulse)
{
    BeginPendingDestroy();

    bool resetChatCounter = !(iPulse % PASSES_PER_SEC(5));

    std::for_each(m_map_pkPCChr.begin(), m_map_pkPCChr.end(),
                    [resetChatCounter, iPulse](const NAME_MAP::value_type &v)
                    {
                        LPCHARACTER ch = v.second;
                        if (ch)
                        {
                          if (resetChatCounter)
                          {
                              ch->ResetChatCounter();
                              ch->CFSM::Update();
                          }

                          ch->UpdateCharacter(iPulse);
                        }
                    }
    );

    std::for_each(m_set_pkChrState.begin(), m_set_pkChrState.end(),
                    [iPulse](LPCHARACTER ch)
                    {
                        if (ch)
                        {
                            ch->UpdateStateMachine(iPulse);
                        }
                    }
    );

    {
        CharacterVectorInteractor i;
        if (GetCharactersByRaceNum(xmas::MOB_SANTA_VNUM, i))
        {
            std::for_each(i.begin(), i.end(),
                            [iPulse](LPCHARACTER ch)
                            {
                                if (ch)
                                {
                                    ch->UpdateStateMachine(iPulse);
                                }
                            }
            );
        }
    }

    if (0 == (iPulse % PASSES_PER_SEC(3600)))
    {
        for (auto it = m_map_dwMobKillCount.begin(); it != m_map_dwMobKillCount.end(); ++it)
        {
            DBManager::instance().SendMoneyLog(MONEY_LOG_MONSTER_KILL, it->first, it->second);
        }

        m_map_dwMobKillCount.clear();
    }

    if (test_server && 0 == (iPulse % PASSES_PER_SEC(60)))
    {
        sys_log(0, "CHARACTER COUNT vid %zu pid %zu", m_map_pkChrByVID.size(), m_map_pkChrByPID.size());
    }

    FlushPendingDestroy();

    CShutdownManager::Instance().Update();
}

void CHARACTER_MANAGER::ProcessDelayedSave()
{
	auto it =m_set_pkChrForDelayedSave.begin();

	while (it != m_set_pkChrForDelayedSave.end())
	{
		LPCHARACTER pkChr = *it++;
		pkChr->SaveReal();
	}

	m_set_pkChrForDelayedSave.clear();
}

bool CHARACTER_MANAGER::AddToStateList(LPCHARACTER ch)
{
	assert(ch != NULL);

	auto it =m_set_pkChrState.find(ch);

	if (it == m_set_pkChrState.end())
	{
		m_set_pkChrState.insert(ch);
		return true;
	}

	return false;
}

void CHARACTER_MANAGER::RemoveFromStateList(LPCHARACTER ch)
{
	auto it =m_set_pkChrState.find(ch);

	if (it != m_set_pkChrState.end())
	{
		//sys_log(0, "RemoveFromStateList %p", ch);
		m_set_pkChrState.erase(it);
	}
}

void CHARACTER_MANAGER::DelayedSave(LPCHARACTER ch)
{
	m_set_pkChrForDelayedSave.insert(ch);
}

bool CHARACTER_MANAGER::FlushDelayedSave(LPCHARACTER ch)
{
	auto it =m_set_pkChrForDelayedSave.find(ch);

	if (it == m_set_pkChrForDelayedSave.end())
		return false;

	m_set_pkChrForDelayedSave.erase(it);
	ch->SaveReal();
	return true;
}

void CHARACTER_MANAGER::RegisterForMonsterLog(LPCHARACTER ch)
{
	m_set_pkChrMonsterLog.insert(ch);
}

void CHARACTER_MANAGER::UnregisterForMonsterLog(LPCHARACTER ch)
{
	m_set_pkChrMonsterLog.erase(ch);
}

void CHARACTER_MANAGER::PacketMonsterLog(LPCHARACTER ch, const void* buf, int size)
{
	for (auto it = m_set_pkChrMonsterLog.begin(); it!=m_set_pkChrMonsterLog.end();++it)
	{
		LPCHARACTER c = *it;

		if (ch && DISTANCE_APPROX(c->GetX()-ch->GetX(), c->GetY()-ch->GetY())>6000)
			continue;

		LPDESC d = c->GetDesc();

		if (d)
			d->Packet(buf, size);
	}
}

void CHARACTER_MANAGER::KillLog(DWORD dwVnum)
{
	const DWORD SEND_LIMIT = 10000;

	itertype(m_map_dwMobKillCount) it = m_map_dwMobKillCount.find(dwVnum);

	if (it == m_map_dwMobKillCount.end())
		m_map_dwMobKillCount.insert(std::make_pair(dwVnum, 1));
	else
	{
		++it->second;

		if (it->second > SEND_LIMIT)
		{
			DBManager::instance().SendMoneyLog(MONEY_LOG_MONSTER_KILL, it->first, it->second);
			m_map_dwMobKillCount.erase(it);
		}
	}
}

void CHARACTER_MANAGER::RegisterRaceNum(DWORD dwVnum)
{
	m_set_dwRegisteredRaceNum.insert(dwVnum);
}

void CHARACTER_MANAGER::RegisterRaceNumMap(LPCHARACTER ch)
{
	DWORD dwVnum = ch->GetRaceNum();

	if (m_set_dwRegisteredRaceNum.find(dwVnum) != m_set_dwRegisteredRaceNum.end()) // µî·ÏµÈ ¹øÈ£ ÀÌ¸é
	{
		sys_log(0, "RegisterRaceNumMap %s %u", ch->GetName(), dwVnum);
		m_map_pkChrByRaceNum[dwVnum].insert(ch);
	}
}

void CHARACTER_MANAGER::UnregisterRaceNumMap(LPCHARACTER ch)
{
	DWORD dwVnum = ch->GetRaceNum();

	itertype(m_map_pkChrByRaceNum) it = m_map_pkChrByRaceNum.find(dwVnum);

	if (it != m_map_pkChrByRaceNum.end())
		it->second.erase(ch);
}

bool CHARACTER_MANAGER::GetCharactersByRaceNum(DWORD dwRaceNum, CharacterVectorInteractor & i)
{
	auto it = m_map_pkChrByRaceNum.find(dwRaceNum);

	if (it == m_map_pkChrByRaceNum.end())
		return false;

	// ÄÁÅ×ÀÌ³Ê º¹»ç
	i = it->second;
	return true;
}

#define FIND_JOB_WARRIOR_0	(1 << 3)
#define FIND_JOB_WARRIOR_1	(1 << 4)
#define FIND_JOB_WARRIOR_2	(1 << 5)
#define FIND_JOB_WARRIOR	(FIND_JOB_WARRIOR_0 | FIND_JOB_WARRIOR_1 | FIND_JOB_WARRIOR_2)
#define FIND_JOB_ASSASSIN_0	(1 << 6)
#define FIND_JOB_ASSASSIN_1	(1 << 7)
#define FIND_JOB_ASSASSIN_2	(1 << 8)
#define FIND_JOB_ASSASSIN	(FIND_JOB_ASSASSIN_0 | FIND_JOB_ASSASSIN_1 | FIND_JOB_ASSASSIN_2)
#define FIND_JOB_SURA_0		(1 << 9)
#define FIND_JOB_SURA_1		(1 << 10)
#define FIND_JOB_SURA_2		(1 << 11)
#define FIND_JOB_SURA		(FIND_JOB_SURA_0 | FIND_JOB_SURA_1 | FIND_JOB_SURA_2)
#define FIND_JOB_SHAMAN_0	(1 << 12)
#define FIND_JOB_SHAMAN_1	(1 << 13)
#define FIND_JOB_SHAMAN_2	(1 << 14)
#define FIND_JOB_SHAMAN		(FIND_JOB_SHAMAN_0 | FIND_JOB_SHAMAN_1 | FIND_JOB_SHAMAN_2)
#ifdef ENABLE_WOLFMAN_CHARACTER
#define FIND_JOB_WOLFMAN_0	(1 << 15)
#define FIND_JOB_WOLFMAN_1	(1 << 16)
#define FIND_JOB_WOLFMAN_2	(1 << 17)
#define FIND_JOB_WOLFMAN		(FIND_JOB_WOLFMAN_0 | FIND_JOB_WOLFMAN_1 | FIND_JOB_WOLFMAN_2)
#endif

//
// (job+1)*3+(skill_group)
//
LPCHARACTER CHARACTER_MANAGER::FindSpecifyPC(unsigned int uiJobFlag, long lMapIndex, LPCHARACTER except, int iMinLevel, int iMaxLevel)
{
	LPCHARACTER chFind = NULL;
	int n = 0;

	for (auto it = m_map_pkChrByPID.begin(); it != m_map_pkChrByPID.end(); ++it)
	{
		LPCHARACTER ch = it->second;

		if (ch == except)
			continue;

		if (ch->GetLevel() < iMinLevel)
			continue;

		if (ch->GetLevel() > iMaxLevel)
			continue;

		if (ch->GetMapIndex() != lMapIndex)
			continue;

		if (uiJobFlag)
		{
			unsigned int uiChrJob = (1 << ((ch->GetJob() + 1) * 3 + ch->GetSkillGroup()));

			if (!IS_SET(uiJobFlag, uiChrJob))
				continue;
		}

		if (!chFind || number(1, ++n) == 1)
			chFind = ch;
	}

	return chFind;
}

int CHARACTER_MANAGER::GetMobItemRate(LPCHARACTER ch)
{
	//PREVENT_TOXICATION_FOR_CHINA
	if (g_bChinaIntoxicationCheck)
	{
		if ( ch->IsOverTime( OT_3HOUR ) )
		{
			if (ch && ch->GetPremiumRemainSeconds(PREMIUM_ITEM) > 0)
				return m_iMobItemRatePremium/2;
			return m_iMobItemRate/2;
		}
		else if ( ch->IsOverTime( OT_5HOUR ) )
		{
			return 0;
		}
	}
	//END_PREVENT_TOXICATION_FOR_CHINA
	if (ch && ch->GetPremiumRemainSeconds(PREMIUM_ITEM) > 0)
		return m_iMobItemRatePremium;
	return m_iMobItemRate;
}

int CHARACTER_MANAGER::GetMobDamageRate(LPCHARACTER ch)
{
	return m_iMobDamageRate;
}

int CHARACTER_MANAGER::GetMobGoldAmountRate(LPCHARACTER ch)
{
	if ( !ch )
		return m_iMobGoldAmountRate;

	//PREVENT_TOXICATION_FOR_CHINA
	if (g_bChinaIntoxicationCheck)
	{
		if ( ch->IsOverTime( OT_3HOUR ) )
		{
			if (ch && ch->GetPremiumRemainSeconds(PREMIUM_GOLD) > 0)
				return m_iMobGoldAmountRatePremium/2;
			return m_iMobGoldAmountRate/2;
		}
		else if ( ch->IsOverTime( OT_5HOUR ) )
		{
			return 0;
		}
	}
	//END_PREVENT_TOXICATION_FOR_CHINA
	if (ch && ch->GetPremiumRemainSeconds(PREMIUM_GOLD) > 0)
		return m_iMobGoldAmountRatePremium;
	return m_iMobGoldAmountRate;
}

int CHARACTER_MANAGER::GetMobGoldDropRate(LPCHARACTER ch)
{
	if ( !ch )
		return m_iMobGoldDropRate;

	//PREVENT_TOXICATION_FOR_CHINA
	if (g_bChinaIntoxicationCheck)
	{
		if ( ch->IsOverTime( OT_3HOUR ) )
		{
			if (ch && ch->GetPremiumRemainSeconds(PREMIUM_GOLD) > 0)
				return m_iMobGoldDropRatePremium/2;
			return m_iMobGoldDropRate/2;
		}
		else if ( ch->IsOverTime( OT_5HOUR ) )
		{
			return 0;
		}
	}
	//END_PREVENT_TOXICATION_FOR_CHINA
	if (ch && ch->GetPremiumRemainSeconds(PREMIUM_GOLD) > 0)
		return m_iMobGoldDropRatePremium;
	return m_iMobGoldDropRate;
}

int CHARACTER_MANAGER::GetMobExpRate(LPCHARACTER ch)
{
	if ( !ch )
		return m_iMobExpRate;

	//PREVENT_TOXICATION_FOR_CHINA
	if (g_bChinaIntoxicationCheck)
	{
		if ( ch->IsOverTime( OT_3HOUR ) )
		{
			if (ch && ch->GetPremiumRemainSeconds(PREMIUM_EXP) > 0)
				return m_iMobExpRatePremium/2;
			return m_iMobExpRate/2;
		}
		else if ( ch->IsOverTime( OT_5HOUR ) )
		{
			return 0;
		}
	}
	//END_PREVENT_TOXICATION_FOR_CHINA
	if (ch && ch->GetPremiumRemainSeconds(PREMIUM_EXP) > 0)
		return m_iMobExpRatePremium;
	return m_iMobExpRate;
}

int	CHARACTER_MANAGER::GetUserDamageRate(LPCHARACTER ch)
{
	if (!ch)
		return m_iUserDamageRate;

	if (ch && ch->GetPremiumRemainSeconds(PREMIUM_EXP) > 0)
		return m_iUserDamageRatePremium;

	return m_iUserDamageRate;
}

void CHARACTER_MANAGER::SendScriptToMap(long lMapIndex, const std::string & s)
{
	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(lMapIndex);

	if (NULL == pSecMap)
		return;

	struct packet_script p;

	p.header = HEADER_GC_SCRIPT;
	p.skin = 1;
	p.src_size = s.size();

	quest::FSendPacket f;
	p.size = p.src_size + sizeof(struct packet_script);
	f.buf.write(&p, sizeof(struct packet_script));
	f.buf.write(&s[0], s.size());

	pSecMap->for_each(f);
}

bool CHARACTER_MANAGER::BeginPendingDestroy()
{
	// Begin ÀÌ ÈÄ¿¡ BeginÀ» ¶Ç ÇÏ´Â °æ¿ì¿¡ Flush ÇÏÁö ¾Ê´Â ±â´É Áö¿øÀ» À§ÇØ
	// ÀÌ¹Ì ½ÃÀÛµÇ¾îÀÖÀ¸¸é false ¸®ÅÏ Ã³¸®
	if (m_bUsePendingDestroy)
		return false;

	m_bUsePendingDestroy = true;
	return true;
}

void CHARACTER_MANAGER::FlushPendingDestroy()
{
	using namespace std;

	m_bUsePendingDestroy = false; // ÇÃ·¡±×¸¦ ¸ÕÀú ¼³Á¤ÇØ¾ß ½ÇÁ¦ Destroy Ã³¸®°¡ µÊ

	if (!m_set_pkChrPendingDestroy.empty())
	{
		sys_log(0, "FlushPendingDestroy size %d", m_set_pkChrPendingDestroy.size());

		auto it =m_set_pkChrPendingDestroy.begin(),
			end = m_set_pkChrPendingDestroy.end();
		for ( ; it != end; ++it) {
			M2_DESTROY_CHARACTER(*it);
		}

		m_set_pkChrPendingDestroy.clear();
	}
}

CharacterVectorInteractor::CharacterVectorInteractor(const CHARACTER_SET & r)
{
    reserve(r.size());
    insert(end(), r.begin(), r.end());

    if (CHARACTER_MANAGER::instance().BeginPendingDestroy())
    {
        m_bMyBegin = true;
    }
}

CharacterVectorInteractor::~CharacterVectorInteractor()
{
    if (m_bMyBegin)
    {
        CHARACTER_MANAGER::instance().FlushPendingDestroy();
    }
}


#ifdef ENABLE_BATTLE_PASS
BYTE GetBattlePassMissionIndex(const char* szMissionName)
{
	std::map<std::string, BYTE> eventNames = {
		{"MISSION_NONE",MISSION_NONE},
		{"MISSION_BOSS",MISSION_BOSS},
		{"MISSION_CATCH_FISH",MISSION_CATCH_FISH},
		{"MISSION_CRAFT_ITEM",MISSION_CRAFT_ITEM},
		{"MISSION_CRAFT_GAYA",MISSION_CRAFT_GAYA},
		{"MISSION_DESTROY_ITEM",MISSION_DESTROY_ITEM},
		{"MISSION_DUNGEON",MISSION_DUNGEON},
		{"MISSION_EARN_MONEY",MISSION_EARN_MONEY},
		{"MISSION_FEED_PET",MISSION_FEED_PET},
		{"MISSION_LEVEL_UP",MISSION_LEVEL_UP},
		{"MISSION_MONSTER",MISSION_MONSTER},
		{"MISSION_MOUNT_TIME",MISSION_MOUNT_TIME},
		{"MISSION_OPEN_OFFLINESHOP",MISSION_OPEN_OFFLINESHOP},
		{"MISSION_PLAYTIME",MISSION_PLAYTIME},
		{"MISSION_REFINE_ITEM",MISSION_REFINE_ITEM},
		{"MISSION_REFINE_ALCHEMY",MISSION_REFINE_ALCHEMY},
		{"MISSION_SASH",MISSION_SASH},
		{"MISSION_SELL_ITEM",MISSION_SELL_ITEM},
		{"MISSION_SPEND_MONEY",MISSION_SPEND_MONEY},
		{"MISSION_SPRITE_STONE",MISSION_SPRITE_STONE},
		{"MISSION_STONE",MISSION_STONE},
		{"MISSION_USE_EMOTICON",MISSION_USE_EMOTICON},
		{"MISSION_WHISPER",MISSION_WHISPER},
		{"MISSION_SHOUT_CHAT",MISSION_SHOUT_CHAT},
		{"MISSION_KILLPLAYER",MISSION_KILLPLAYER},
	};
	auto it = eventNames.find(szMissionName);
	if (it != eventNames.end())
		return it->second;
	return MISSION_NONE;
}
void CHARACTER_MANAGER::DoMission(LPCHARACTER ch, BYTE missionIndex, long long value, DWORD subValue)
{
	if (!ch->IsBattlePassActive())
		return;
	else if (!IsBattlePassMissionHave(missionIndex))
		return;

	DWORD missionSubValue = GetMissionSubValue(missionIndex);
	if (missionSubValue != 0)
		if (subValue != missionSubValue)
			return;

	long long missionMaxValue = GetMissionMaxValue(missionIndex);
	long long selfValue = ch->GetBattlePassMissonValue(missionIndex);
	if (selfValue >= missionMaxValue)
		return;

	long long newValue = selfValue + std::abs(value);
	if (newValue >= missionMaxValue)
	{
		newValue = missionMaxValue;
		ch->SetBattlePassMissonValue(missionIndex, newValue);
		ch->ChatPacket(CHAT_TYPE_INFO, "1045");
		GiveLowMissionReward(ch, missionIndex);
		if (CheckBattlePassDone(ch))
		{
			char buf[254];
#ifdef ENABLE_REWARD_SYSTEM
			if(ch)
				CHARACTER_MANAGER::Instance().SetRewardData(REWARD_BATTLEPASS,ch->GetName(), true);
#endif
			snprintf(buf, sizeof(buf), "1044 %s", ch->GetName());
			BroadcastNotice(buf);
			ch->ChatPacket(CHAT_TYPE_INFO, "1046");
		}
	}
	else
		ch->SetBattlePassMissonValue(missionIndex, newValue);
	if (ch->GetProtectTime("battlePassOpen") == 1)
		ch->ChatPacket(CHAT_TYPE_COMMAND, "BattlePassSetMission %d %lld", missionIndex, newValue);
}
bool CHARACTER_MANAGER::IsBattlePassMissionHave(BYTE missionIndex)
{
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);
	auto it = m_BattlePassData.find(vKey.tm_mon);
	if (it != m_BattlePassData.end())
	{
		if (it->second.second.missionData.find(missionIndex) != it->second.second.missionData.end())
			return true;
	}
	return false;
}
void CHARACTER_MANAGER::GiveLowMissionReward(LPCHARACTER ch, BYTE missionIndex)
{
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);
	auto it = m_BattlePassData.find(vKey.tm_mon);
	if (it != m_BattlePassData.end())
	{
		auto lowReward = it->second.second.subReward.find(missionIndex);
		if (lowReward != it->second.second.subReward.end())
		{
			for (DWORD j = 0; j < lowReward->second.size(); ++j)
				ch->AutoGiveItem(lowReward->second[j].first, lowReward->second[j].second);
		}
	}
}
long long CHARACTER_MANAGER::GetMissionMaxValue(BYTE missionIndex)
{
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);
	auto it = m_BattlePassData.find(vKey.tm_mon);
	if (it != m_BattlePassData.end())
	{
		auto itEx = it->second.second.missionData.find(missionIndex);
		if (itEx != it->second.second.missionData.end())
			return itEx->second.first;
	}
	return 0;
}
DWORD CHARACTER_MANAGER::GetMissionSubValue(BYTE missionIndex)
{
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);
	auto it = m_BattlePassData.find(vKey.tm_mon);
	if (it != m_BattlePassData.end())
	{
		auto itEx = it->second.second.missionData.find(missionIndex);
		if (itEx != it->second.second.missionData.end())
			return itEx->second.second;
	}
	return 0;
}
bool CHARACTER_MANAGER::CheckBattlePassDone(LPCHARACTER ch)
{
	if (!ch->IsBattlePassActive())
		return false;
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);
	auto it = m_BattlePassData.find(vKey.tm_mon);
	if (it != m_BattlePassData.end())
	{
		const auto missionData = it->second.second.missionData;
		if (missionData.size())
		{
			for (auto itMission = missionData.begin(); itMission != missionData.end(); ++itMission)
			{
				if (ch->GetBattlePassMissonValue(itMission->first) < itMission->second.first)
					return false;
			}
		}
	}
	return true;
}
void CHARACTER_MANAGER::CheckBattlePassReward(LPCHARACTER ch)
{
	if(!CheckBattlePassDone(ch))
		return;
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);
	auto it = m_BattlePassData.find(vKey.tm_mon);
	if (it != m_BattlePassData.end())
	{
		const auto rewardData = it->second.second.rewardData;
		if (rewardData.size())
		{
			for (auto itReward = rewardData.begin(); itReward != rewardData.end(); ++itReward)
				ch->AutoGiveItem(itReward->first, itReward->second);
		}
	}
	ch->ClearBattlePassData();
	ch->SetQuestFlag("battlePass.status",2);
	ch->ChatPacket(CHAT_TYPE_COMMAND, "BattlePassSetStatusEx 2");
}
void CHARACTER_MANAGER::LoadBattlePassData(LPCHARACTER ch)
{
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);
	auto it = m_BattlePassData.find(vKey.tm_mon);
	if(it != m_BattlePassData.end())
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "BattlePassClear");
		std::string rewardText = "";
		const auto missionData = it->second.second.missionData;
		if (missionData.size())
		{
			for (auto itMission = missionData.begin(); itMission != missionData.end(); ++itMission)
			{
				const auto subReward = it->second.second.subReward;
				auto rewardList = subReward.find(itMission->first);
				if (rewardList != subReward.end())
				{
					const auto itRewardList = rewardList->second;
					if (itRewardList.size() == 0)
						rewardText = "Empty";
					else
					{
						for (BYTE j = 0; j < itRewardList.size(); ++j)
						{
							rewardText += std::to_string(itRewardList[j].first);
							rewardText += "|";
							rewardText += std::to_string(itRewardList[j].second);
							rewardText += "#";
						}
					}
				}

				int vecList = 0;
				const auto missinvecIndexList = it->second.second.missionIndex;
				auto missinvecIndex = missinvecIndexList.find(itMission->first);
				if (missinvecIndex != missinvecIndexList.end())
					vecList = missinvecIndex->second;

				ch->ChatPacket(CHAT_TYPE_COMMAND, "BattlePassAppendMission %u %s %lld %lld %u %d", itMission->first, rewardText.c_str(), ch->GetBattlePassMissonValue(itMission->first), itMission->second.first, itMission->second.second, vecList);
				rewardText = "";
			}
		}
		const auto rewardData = it->second.second.rewardData;
		for (DWORD j = 0; j < rewardData.size(); ++j)
		{
			rewardText += std::to_string(rewardData[j].first);
			rewardText += "|";
			rewardText += std::to_string(rewardData[j].second);
			rewardText += "#";
		}
		ch->ChatPacket(CHAT_TYPE_COMMAND, "BattlePassSetStatus %d %d %s", ch->GetBattlePassStatus(), it->second.first-time(0), rewardText.c_str());
	}
}
void CHARACTER_MANAGER::InitializeBattlePass()
{
	m_BattlePassData.clear();
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT * FROM player.battle_pass"));
	if (pMsg->Get()->uiNumRows != 0)
	{
		time_t cur_Time = time(NULL);
		struct tm vKey = *localtime(&cur_Time);
		MYSQL_ROW row = NULL;
		
		for (int n = 0; (row = mysql_fetch_row(pMsg->Get()->pSQLResult)) != NULL; ++n)
		{
			int c = 0;
			int monthIndex;
			str_to_number(monthIndex, row[c++]);

			int year = vKey.tm_year+1900;
			int month = monthIndex;
			if (month >= 12)
			{
				year += 1;
				month = 0;
			}

			struct tm t;
			t.tm_year = year - 1900;
			t.tm_mon = month;
			t.tm_mday = 1;
			t.tm_isdst = 0;
			t.tm_hour = 0;
			t.tm_min = 0;
			t.tm_sec = 0;
			time_t lastTime = mktime(&t);

			//sys_err("year %d month %d lasttime %d realTime %d", year, month, lastTime, lastTime-time(0));

			BattlePassData p;
			for (DWORD j = 0; j < BATTLE_MISSION_MAX; ++j)
			{
				char missionName[100];
				strlcpy(missionName, row[c++], sizeof(missionName));
				BYTE missionIndex = GetBattlePassMissionIndex(missionName);
				DWORD missionMaxValue, missionSubValue;
				str_to_number(missionMaxValue, row[c++]);
				str_to_number(missionSubValue, row[c++]);

				std::vector<std::pair<DWORD, DWORD>> m_vec;
				m_vec.clear();
				for (DWORD j = 0; j < BATTLE_SUB_REWARD; ++j)
				{
					DWORD itemVnum, itemCount;
					str_to_number(itemVnum, row[c++]);
					str_to_number(itemCount, row[c++]);
					if (itemVnum > 0)
						m_vec.emplace_back(itemVnum, itemCount);
				}

				if (missionIndex > 0)
				{
					auto it = p.missionData.find(missionIndex);
					if (it == p.missionData.end())
					{
						p.subReward.emplace(missionIndex, m_vec);
						p.missionIndex.emplace(missionIndex, j + 1);
						p.missionData.emplace(missionIndex, std::make_pair(missionMaxValue, missionSubValue));
					}
					else
						sys_err("battlepass: duplicate mission in same month! monthIndex %d missionIndex %d", monthIndex, missionIndex);
				}
			}
			for (DWORD j = 0; j < BATTLE_REWARD_MAX; ++j)
			{
				DWORD itemVnum, itemCount;
				str_to_number(itemVnum, row[c++]);
				str_to_number(itemCount, row[c++]);
				if (itemVnum != 0 && itemCount != 0)
					p.rewardData.emplace_back(itemVnum, itemCount);
			}
			m_BattlePassData.emplace(monthIndex-1, std::make_pair(lastTime,p));
		}
	}
}
#endif

#ifdef ENABLE_ITEMSHOP
void CHARACTER_MANAGER::LoadItemShopLogReal(LPCHARACTER ch, const char* c_pData)
{
	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return;
	}

	BYTE subIndex = ITEMSHOP_LOG;

	const int logCount = *(int*)c_pData;
	c_pData += sizeof(int);

	std::vector<TIShopLogData> m_vec;

	if (logCount)
	{
		for (DWORD j = 0; j < logCount; ++j)
		{
			const TIShopLogData logData = *(TIShopLogData*)c_pData;
			m_vec.emplace_back(logData);

			c_pData += sizeof(TIShopLogData);
		}
	}

	TPacketGCItemShop p;
	p.header = HEADER_GC_ITEMSHOP;
	p.size = sizeof(TPacketGCItemShop) + sizeof(BYTE) + sizeof(int) + (sizeof(TIShopLogData) * logCount);

	TEMP_BUFFER buf;
	buf.write(&p, sizeof(TPacketGCItemShop));
	buf.write(&subIndex, sizeof(BYTE));
	buf.write(&logCount, sizeof(int));

	if (logCount > 0)
	{
		buf.write(m_vec.data(), sizeof(TIShopLogData)* logCount);
	}

	d->Packet(buf.read_peek(), buf.size());
}
void CHARACTER_MANAGER::LoadItemShopLog(LPCHARACTER ch)
{
	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return;
	}

	BYTE subIndex = ITEMSHOP_LOG;
	DWORD accountID = d->GetAccountTable().id;

	db_clientdesc->DBPacketHeader(HEADER_GD_ITEMSHOP, ch->GetDesc()->GetHandle(), sizeof(BYTE)+sizeof(DWORD));
	db_clientdesc->Packet(&subIndex, sizeof(BYTE));
	db_clientdesc->Packet(&accountID, sizeof(DWORD));
}

void CHARACTER_MANAGER::LoadItemShopData(LPCHARACTER ch, bool isAll)
{
	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return;
	}

	TEMP_BUFFER buf;

	TPacketGCItemShop p;
	p.header = HEADER_GC_ITEMSHOP;

	long long act_lldCoins;
#ifdef USE_ITEMSHOP_RENEWED
	long long act_lldJCoins;
	ch->GetAccountMoney(act_lldCoins, act_lldJCoins);
#else
	ch->GetAccountMoney(act_lldCoins);
#endif

	if (isAll)
	{
		int calculateSize = 0;
		BYTE subIndex = ITEMSHOP_LOAD;
		calculateSize += sizeof(BYTE);

		calculateSize += sizeof(long long); // dragon coins
#ifdef USE_ITEMSHOP_RENEWED
		calculateSize += sizeof(long long); // dragon jetons
#endif
		calculateSize += sizeof(int); // updatetime

		int categoryTotalSize = m_IShopManager.size();
		calculateSize += sizeof(int);

		if (m_IShopManager.size())
		{
			for (auto it = m_IShopManager.begin(); it != m_IShopManager.end(); ++it)
			{
				BYTE categoryIndex = it->first;
				calculateSize += sizeof(BYTE);

				BYTE categorySize = it->second.size();
				calculateSize += sizeof(BYTE);

				if (it->second.size())
				{
					for (auto itEx = it->second.begin(); itEx != it->second.end(); ++itEx)
					{
						BYTE categorySubIndex = itEx->first;
						calculateSize += sizeof(BYTE);

						BYTE categorySubSize = itEx->second.size();
						calculateSize += sizeof(BYTE);

						if (categorySubSize)
						{
							calculateSize += sizeof(TIShopData) * categorySubSize;
						}
					}
				}
			}
		}

		p.size = sizeof(TPacketGCItemShop) + calculateSize;

		buf.write(&p, sizeof(TPacketGCItemShop));
		buf.write(&subIndex, sizeof(BYTE));
		buf.write(&act_lldCoins, sizeof(act_lldCoins));
#ifdef USE_ITEMSHOP_RENEWED
		buf.write(&act_lldJCoins, sizeof(act_lldJCoins));
#endif
		buf.write(&itemshopUpdateTime, sizeof(int));
		buf.write(&categoryTotalSize, sizeof(int));

		if (m_IShopManager.size())
		{
			for (auto it = m_IShopManager.begin(); it != m_IShopManager.end(); ++it)
			{
				BYTE categoryIndex = it->first;
				buf.write(&categoryIndex, sizeof(BYTE));

				BYTE categorySize = it->second.size();
				buf.write(&categorySize, sizeof(BYTE));

				if (it->second.size())
				{
					for (auto itEx = it->second.begin(); itEx != it->second.end(); ++itEx)
					{
						BYTE categorySubIndex = itEx->first;
						buf.write(&categorySubIndex, sizeof(BYTE));

						BYTE categorySubSize = itEx->second.size();
						buf.write(&categorySubSize, sizeof(BYTE));

						if (categorySubSize)
						{
							buf.write(itEx->second.data(), sizeof(TIShopData) * categorySubSize);
						}
					}
				}
			}
		}
	}
	else
	{
		p.size = sizeof(TPacketGCItemShop) + sizeof(BYTE)+sizeof(int)+sizeof(int);

		buf.write(&p, sizeof(TPacketGCItemShop));

		BYTE subIndex = ITEMSHOP_LOAD;
		buf.write(&subIndex, sizeof(BYTE));

		buf.write(&act_lldCoins, sizeof(act_lldCoins));
#ifdef USE_ITEMSHOP_RENEWED
		buf.write(&act_lldJCoins, sizeof(act_lldJCoins));
#endif
		buf.write(&itemshopUpdateTime, sizeof(int));

		int categoryTotalSize = 9999;
		buf.write(&categoryTotalSize, sizeof(int));
	}

	d->Packet(buf.read_peek(), buf.size());
}

void CHARACTER_MANAGER::LoadItemShopBuyReal(LPCHARACTER ch, const char* c_pData)
{
	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return;
	}

	const BYTE returnType = *(BYTE*)c_pData;
	c_pData += sizeof(BYTE);

	if (returnType == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "991");
		return;
	}
	else if (returnType == 1)
	{
		const int weekMaxCount = *(int*)c_pData;
		c_pData += sizeof(int);
		ch->ChatPacket(CHAT_TYPE_INFO, "994 %d", weekMaxCount);
		return;
	}
	else if (returnType == 2)
	{
		const int monthMaxCount = *(int*)c_pData;
		c_pData += sizeof(int);
		ch->ChatPacket(CHAT_TYPE_INFO, "995 %d", monthMaxCount);
		return;
	}
	else if (returnType == 4)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "1000");
		return;
	}
	else if (returnType == 5)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "1500");
		return;
	}
#ifdef USE_ITEMSHOP_RENEWED
	else if (returnType == 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "1501");
		return;
	}
	else if (returnType == 7)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "1502");
		return;
	}
#endif

	const bool isOpenLog = *(bool*)c_pData;
	c_pData += sizeof(bool);

	const DWORD itemVnum = *(DWORD*)c_pData;
	c_pData += sizeof(DWORD);

	const int itemCount = *(int*)c_pData;
	c_pData += sizeof(int);

	const long long itemPrice = *(long long*)c_pData;
	c_pData += sizeof(long long);

#ifdef USE_ITEMSHOP_RENEWED
	const long long itemPriceJD = *(long long*)c_pData;
	c_pData += sizeof(long long);
#endif

	TEMP_BUFFER buf;
	TPacketGCItemShop p;
	p.header = HEADER_GC_ITEMSHOP;
	p.size = sizeof(TPacketGCItemShop) + sizeof(BYTE) + sizeof(long long) + sizeof(bool);

	if (isOpenLog)
	{
		p.size += sizeof(TIShopLogData);
	}

	long long act_lldCoins;
#ifdef USE_ITEMSHOP_RENEWED
	long long act_lldJCoins;
	ch->GetAccountMoney(act_lldCoins, act_lldJCoins, true);
#else
	ch->GetAccountMoney(act_lldCoins, true);
#endif

	ch->AutoGiveItem(itemVnum, itemCount, -1, false);

	BYTE subIndex = ITEMSHOP_DRAGONCOIN;

	buf.write(&p, sizeof(TPacketGCItemShop));
	buf.write(&subIndex, sizeof(BYTE));
	buf.write(&act_lldCoins, sizeof(act_lldCoins));
#ifdef USE_ITEMSHOP_RENEWED
	buf.write(&act_lldJCoins, sizeof(act_lldJCoins));
#endif
	buf.write(&isOpenLog, sizeof(bool));

	if (isOpenLog)
	{
		const TIShopLogData logData = *(TIShopLogData*)c_pData;
		c_pData += sizeof(TIShopLogData);

		buf.write(&logData, sizeof(TIShopLogData));
	}

	d->Packet(buf.read_peek(), buf.size());

#ifdef USE_ITEMSHOP_RENEWED
	if (itemPrice > 0 && itemPriceJD > 0)
	{
		if (itemCount > 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "1508 %d I%d %lld %lld", itemCount, itemVnum, itemPrice, itemPriceJD);
			ch->ChatPacket(CHAT_TYPE_INFO, "1503");
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "1509 I%d %lld %lld", itemVnum, itemPrice, itemPriceJD);
			ch->ChatPacket(CHAT_TYPE_INFO, "1503");
		}
	}
	else if (itemPrice > 0)
	{
		if (itemCount > 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "1504 %d I%d %lld", itemCount, itemVnum, itemPrice);
			ch->ChatPacket(CHAT_TYPE_INFO, "1503");
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "1505 I%d %lld", itemVnum, itemPrice);
			ch->ChatPacket(CHAT_TYPE_INFO, "1503");
		}
	}
	else if (itemPriceJD > 0)
	{
		if (itemCount > 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "1506 %d I%d %lld", itemCount, itemVnum, itemPriceJD);
			ch->ChatPacket(CHAT_TYPE_INFO, "1503");
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "1507 I%d %lld", itemVnum, itemPriceJD);
			ch->ChatPacket(CHAT_TYPE_INFO, "1503");
		}
	}
	else
	{
		if (itemCount > 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "1508 %d I%d %lld %lld", itemCount, itemVnum, itemPrice, itemPriceJD);
			ch->ChatPacket(CHAT_TYPE_INFO, "1503");
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "1509 I%d %lld %lld", itemVnum, itemPrice, itemPriceJD);
			ch->ChatPacket(CHAT_TYPE_INFO, "1503");
		}
	}
#else
	if (itemCount > 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "992 I%d %d %lld", itemVnum, itemCount, itemPrice);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "993 I%d %lld", itemVnum, itemPrice);
	}
#endif
}

void CHARACTER_MANAGER::LoadItemShopBuy(LPCHARACTER ch, int itemID, int itemCount)
{
	if (itemID < 0 || itemCount < 1 || itemCount > 20)
	{
		return;
	}

	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return;
	}

	if (!m_IShopManager.empty())
	{
		for (auto it = m_IShopManager.begin(); it != m_IShopManager.end(); ++it)
		{
			if (!it->second.empty())
			{
				for (auto itEx = it->second.begin(); itEx != it->second.end(); ++itEx)
				{
					if (!itEx->second.empty())
					{
						for (auto itReal = itEx->second.begin(); itReal != itEx->second.end(); ++itReal)
						{
							const TIShopData& itemData = *itReal;
							if (itemData.id == itemID)
							{
								const TItemTable* pItemProto = ITEM_MANAGER::Instance().GetTable(itemData.itemVnum);
								if (!pItemProto)
								{
									sys_err("Item vnum: %u doesn't exists!", itemData.itemVnum);
									return;
								}

								if (itemCount > 1)
								{
									if (pItemProto->dwAntiFlags & ITEM_ANTIFLAG_STACK || !(pItemProto->dwFlags & ITEM_FLAG_STACKABLE))
									{
										sys_err("Item vnum: %u is not stackable!", itemData.itemVnum);
										return;
									}
								}

								long long act_lldCoins;
#ifdef USE_ITEMSHOP_RENEWED
								long long act_lldJCoins;
								ch->GetAccountMoney(act_lldCoins, act_lldJCoins);
#else
								ch->GetAccountMoney(act_lldCoins);
#endif

								long long itemPrice = itemData.itemPrice * itemCount;
#ifdef USE_ITEMSHOP_RENEWED
								long long itemPriceJD = itemData.itemPriceJD * itemCount;
#endif

								if (itemData.discount > 0)
								{
									itemPrice = static_cast<long long>((float(itemData.itemPrice) / 100.0) * float(100 - itemData.discount));
#ifdef USE_ITEMSHOP_RENEWED
									itemPriceJD = static_cast<long long>((float(itemData.itemPriceJD) / 100.0) * float(100 - itemData.discount));
#endif
								}

								if (itemPrice < 0)
								{
									return;
								}

								if (act_lldCoins < itemPrice)
								{
									ch->ChatPacket(CHAT_TYPE_INFO, "991");
									return;
								}

#ifdef USE_ITEMSHOP_RENEWED
								if (itemPriceJD < 0)
								{
									return;
								}

								if (act_lldJCoins < itemPriceJD)
								{
									ch->ChatPacket(CHAT_TYPE_INFO, "1502");
									return;
								}
#endif

								DWORD accountID = d->GetAccountTable().id;
								if (accountID == 0)
								{
									return;
								}

								BYTE subIndex = ITEMSHOP_BUY;

								char szName[CHARACTER_NAME_MAX_LEN + 1] = {};
								strlcpy(szName, ch->GetName(), sizeof(szName));

								char szIP[16];
								strlcpy(szIP, d->GetHostName(), sizeof(szIP));

								TEMP_BUFFER buf;
								buf.write(&subIndex, sizeof(BYTE));
								buf.write(&accountID, sizeof(DWORD));
								buf.write(&szName, sizeof(szName));
								buf.write(&szIP, sizeof(szIP));
								buf.write(&itemID, sizeof(int));
								buf.write(&itemCount, sizeof(int));
								bool isLogOpen = (ch->GetProtectTime("itemshop.log") == 1) ? true : false;
								buf.write(&isLogOpen, sizeof(bool));

								db_clientdesc->DBPacketHeader(HEADER_GD_ITEMSHOP, d->GetHandle(), buf.size());
								db_clientdesc->Packet(buf.read_peek(), buf.size());
								return;
							}
						}
					}
				}
			}
		}
	}
}

void CHARACTER_MANAGER::UpdateItemShopItem(const char* c_pData)
{
	const TIShopData& updateItem = *(TIShopData*)c_pData;
	c_pData += sizeof(TIShopData);

	bool sendPacketProcess = false;

	if (!m_IShopManager.empty())
	{
		for (auto it = m_IShopManager.begin(); it != m_IShopManager.end(); ++it)
		{
			if (sendPacketProcess)
			{
				break;
			}

			if (!it->second.empty())
			{
				for (auto itEx = it->second.begin(); itEx != it->second.end(); ++itEx)
				{
					if (sendPacketProcess)
					{
						break;
					}

					if (!itEx->second.empty())
					{
						for (auto itReal = itEx->second.begin(); itReal != itEx->second.end(); ++itReal)
						{
							TIShopData& itemData = *itReal;
							if (itemData.id == updateItem.id)
							{
								itemData.maxSellCount = updateItem.maxSellCount;
								sendPacketProcess = true;
								break;
							}
						}
					}
				}
			}
		}
	}

	if (sendPacketProcess)
	{
		TEMP_BUFFER buf;

		TPacketGCItemShop p;
		p.header = HEADER_GC_ITEMSHOP;
		p.size = sizeof(TPacketGCItemShop) + sizeof(BYTE) + sizeof(TIShopData);

		BYTE subIndex = ITEMSHOP_UPDATE_ITEM;
		buf.write(&p, sizeof(TPacketGCItemShop));
		buf.write(&subIndex, sizeof(BYTE));
		buf.write(&updateItem, sizeof(TIShopData));

		const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();
		if (c_ref_set.size())
		{
			for (auto it = c_ref_set.begin(); it != c_ref_set.end(); ++it)
			{
				auto desc = *it;
				if (desc)
				{
					LPCHARACTER ch = desc->GetCharacter();
					if (ch)
					{
						if (ch->GetProtectTime("itemshop.load") == 1)
						{
							desc->Packet(buf.read_peek(), buf.size());
						}
					}
				}
			}
		}
	}
}

void RefreshItemShop(LPDESC d)
{
	LPCHARACTER ch = d ? d->GetCharacter() : nullptr;
	if (!ch)
	{
		return;
	}

	if (ch->GetProtectTime("itemshop.load") == 1)
	{
		CHARACTER_MANAGER::Instance().LoadItemShopData(ch, true);

		ch->ChatPacket(CHAT_TYPE_INFO, "990");
	}
}
void CHARACTER_MANAGER::LoadItemShopData(const char* c_pData)
{
	m_IShopManager.clear();

	const int updateTime = *(int*)c_pData;
	c_pData += sizeof(int);

	const bool isManuelUpdate = *(bool*)c_pData;
	c_pData += sizeof(bool);

	const int categoryTotalSize = *(int*)c_pData;
	c_pData += sizeof(int);

	itemshopUpdateTime = updateTime;

	for (auto j = 0; j < categoryTotalSize; ++j)
	{
		const BYTE categoryIndex = *(BYTE*)c_pData;
		c_pData += sizeof(BYTE);

		const BYTE categorySize = *(BYTE*)c_pData;
		c_pData += sizeof(BYTE);

		std::map<BYTE, std::vector<TIShopData>> m_map;
		m_map.clear();

		for (auto x = 0; x < categorySize; ++x)
		{
			const BYTE categorySubIndex = *(BYTE*)c_pData;
			c_pData += sizeof(BYTE);

			const BYTE categorySubSize = *(BYTE*)c_pData;
			c_pData += sizeof(BYTE);

			std::vector<TIShopData> m_vec;
			m_vec.clear();

			for (auto b = 0; b < categorySubSize; ++b)
			{
				const TIShopData itemData = *(TIShopData*)c_pData;

				m_vec.emplace_back(itemData);
				c_pData += sizeof(TIShopData);
			}

			if (!m_vec.empty())
			{
				m_map.emplace(categorySubIndex, m_vec);
			}
		}

		if (!m_map.empty())
		{
			m_IShopManager.emplace(categoryIndex, m_map);
		}
	}

	if (isManuelUpdate)
	{
		const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();
		std::for_each(c_ref_set.begin(), c_ref_set.end(), RefreshItemShop);
	}
}
#endif

#ifdef ENABLE_EVENT_MANAGER
#include "item_manager.h"
#include "item.h"
#include "desc_client.h"
#include "desc_manager.h"
void CHARACTER_MANAGER::ClearEventData()
{
	m_eventData.clear();
}
void CHARACTER_MANAGER::CheckBonusEvent(LPCHARACTER ch)
{
	const TEventManagerData* eventPtr = CheckEventIsActive(BONUS_EVENT, ch->GetEmpire());
	if (eventPtr)
		ch->ApplyPoint(eventPtr->value[0], eventPtr->value[1]);
}
const TEventManagerData* CHARACTER_MANAGER::CheckEventIsActive(BYTE eventIndex, BYTE empireIndex)
{
	const time_t cur_Time = time(NULL);
	const struct tm vKey = *localtime(&cur_Time);

	for(auto it = m_eventData.begin();it!=m_eventData.end();++it)
	{
		const auto& dayIndex = it->first;
		const auto& dayVector = it->second;
		for (DWORD j=0;j<dayVector.size();++j)
		{
			const auto & eventData = dayVector[j];
			if (eventData.eventIndex == eventIndex)
			{
				if (eventData.channelFlag != 0)
					if (eventData.channelFlag != g_bChannel)
						continue;
				if (eventData.empireFlag != 0 && empireIndex != 0)
					if (eventData.empireFlag != empireIndex)
						continue;

				if(eventData.eventStatus == true)
					return &eventData;
				//if (cur_Time >= eventData.startTime && cur_Time <= eventData.endTime)
				//	return &eventData;
			}
		}
	}
	return NULL;
}
void CHARACTER_MANAGER::CheckEventForDrop(LPCHARACTER pkChr, LPCHARACTER pkKiller, std::vector<LPITEM>& vec_item)
{
	const BYTE killerEmpire = pkKiller->GetEmpire();
	const TEventManagerData* eventPtr = NULL;
	LPITEM rewardItem = NULL;

	if (pkChr->IsStone())
	{
#ifdef USE_DROP_AFFECT_BONUSES
		bool bDoubleDrop = (number(1, 100) <= pkKiller->GetPoint(POINT_DOUBLE_DROP_METIN)) ? true : false;
		if (!bDoubleDrop)
		{
			eventPtr = CheckEventIsActive(DOUBLE_METIN_LOOT_EVENT, killerEmpire);
		}
#else
		eventPtr = CheckEventIsActive(DOUBLE_METIN_LOOT_EVENT, killerEmpire);
#endif
		if (
		(
#ifdef USE_DROP_AFFECT_BONUSES
 bDoubleDrop || 
#endif
		eventPtr) && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 20))
		{
			const int prob = number(1, 100);

#ifdef USE_DROP_AFFECT_BONUSES
			const int success_prob = bDoubleDrop ? 100 : eventPtr->value[3];
#else
			const int success_prob = eventPtr->value[3];
#endif

			if (success_prob >= prob)
			{
				std::vector<LPITEM> m_cache;
				for (DWORD j=0;j<vec_item.size();++j)
				{
					const auto vItem = vec_item[j];
					rewardItem = ITEM_MANAGER::Instance().CreateItem(vItem->GetVnum(), vItem->GetCount(), 0, true);
					if (rewardItem) m_cache.emplace_back(rewardItem);
				}
				for (DWORD j=0;j<m_cache.size();++j)
					vec_item.emplace_back(m_cache[j]);
			}
		}
	}
	else if (pkChr->GetMobRank() >= MOB_RANK_BOSS)
	{
#ifdef USE_DROP_AFFECT_BONUSES
		bool bDoubleDrop = (number(1, 100) <= pkKiller->GetPoint(POINT_DOUBLE_DROP_BOSS)) ? true : false;
		if (!bDoubleDrop)
		{
			eventPtr = CheckEventIsActive(DOUBLE_BOSS_LOOT_EVENT, killerEmpire);
		}
#else
		eventPtr = CheckEventIsActive(DOUBLE_BOSS_LOOT_EVENT, killerEmpire);
#endif

		if (
		(
#ifdef USE_DROP_AFFECT_BONUSES
 bDoubleDrop || 
#endif
		eventPtr) && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 20))
		{
			const int prob = number(1, 100);

#ifdef USE_DROP_AFFECT_BONUSES
			const int success_prob = bDoubleDrop ? 100 : eventPtr->value[3];
#else
			const int success_prob = eventPtr->value[3];
#endif
			if (success_prob >= prob)
			{
				std::vector<LPITEM> m_cache;
				for (DWORD j=0;j<vec_item.size();++j)
				{
					const auto vItem = vec_item[j];
					rewardItem = ITEM_MANAGER::Instance().CreateItem(vItem->GetVnum(), vItem->GetCount(), 0, true);
					if (rewardItem) m_cache.emplace_back(rewardItem);
				}
				for (DWORD j=0;j<m_cache.size();++j)
					vec_item.emplace_back(m_cache[j]);
			}
		}
	}

	eventPtr = CheckEventIsActive(DOUBLE_MISSION_BOOK_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 20))
	{
		const int prob = number(1, 100);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			// If your moonlight item vnum is different change 50011!
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(77917, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}
	// if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 20))
	// {
		// const int prob = number(1, 100);
		// const int success_prob = eventPtr->value[3];
		// if (success_prob >= prob)
		// {
			//If you have different book index put here!
			// constexpr DWORD m_lbookItems[] = { 50307, 50308, 50309, 50310 };
			// std::vector<LPITEM> m_cache;
			// for (DWORD j=0;j<vec_item.size();++j)
			// {
				// const auto vItem = vec_item[j];

				// const DWORD itemVnum = vItem->GetVnum();
				// for (DWORD x=0;x<_countof(m_lbookItems);++x)
				// {
					// if (m_lbookItems[x] == itemVnum)
					// {
						// rewardItem = ITEM_MANAGER::Instance().CreateItem(itemVnum, vItem->GetCount(), 0, true);
						// if (rewardItem) m_cache.emplace_back(rewardItem);
						// break;
					// }
				// }
				// for (DWORD j=0;j<m_cache.size();++j)
					// vec_item.emplace_back(m_cache[j]);
			// }
		// }
	// }

	eventPtr = CheckEventIsActive(DUNGEON_TICKET_LOOT_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 20))
	{
		const int prob = number(1, 100);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			// If you have different book index put here!
			constexpr DWORD m_lticketItems[] = { 25163, 25164, 25165, 25166 , 25167 , 25168 , 25169 };
			std::vector<LPITEM> m_cache;
			for (DWORD j=0;j<vec_item.size();++j)
			{
				const auto vItem = vec_item[j];
				const DWORD itemVnum = vItem->GetVnum();
				for (DWORD x=0;x<_countof(m_lticketItems);++x)
				{
					if (m_lticketItems[x] == itemVnum)
					{
						rewardItem = ITEM_MANAGER::Instance().CreateItem(itemVnum, vItem->GetCount(), 0, true);
						if (rewardItem) m_cache.emplace_back(rewardItem);
						break;
					}
				}
				for (DWORD j=0;j<m_cache.size();++j)
					vec_item.emplace_back(m_cache[j]);
			}
		}
	}
	eventPtr = CheckEventIsActive(MOONLIGHT_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 20))
	{
		const int prob = number(1, 100);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			// If your moonlight item vnum is different change 50011!
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(50011, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}

#ifdef ENABLE_MINI_GAME_CATCH_KING
	eventPtr = CheckEventIsActive(CATCH_KING_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 20))
	{
		const int prob = number(1, 100);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(CATCH_KING_DROP_ITEM, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}
#endif

	eventPtr = CheckEventIsActive(CARD_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 20))
	{
		const int prob = number(1, 100);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(79505, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}
	eventPtr = CheckEventIsActive(TREASURE_KEY, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 20))
	{
		const int prob = number(1, 100);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(77924, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}
}
void CHARACTER_MANAGER::CompareEventSendData(TEMP_BUFFER* buf)
{
	const BYTE dayCount = m_eventData.size();
	const BYTE subIndex = EVENT_MANAGER_LOAD;
	const int cur_Time = time(NULL);
	TPacketGCEventManager p;
	p.header = HEADER_GC_EVENT_MANAGER;
	p.size = sizeof(TPacketGCEventManager) + sizeof(BYTE)+sizeof(BYTE)+sizeof(int);
	for(auto it = m_eventData.begin();it != m_eventData.end();++it)
	{
		const auto& dayIndex = it->first;
		const auto& dayData = it->second;
		const BYTE dayEventCount = dayData.size();
		p.size += sizeof(BYTE) + sizeof(BYTE) + (dayEventCount * sizeof(TEventManagerData));
	}
	buf->write(&p, sizeof(TPacketGCEventManager));
	buf->write(&subIndex, sizeof(BYTE));
	buf->write(&dayCount, sizeof(BYTE));
	buf->write(&cur_Time, sizeof(int));
	for(auto it = m_eventData.begin();it != m_eventData.end();++it)
	{
		const auto& dayIndex = it->first;
		const auto& dayData = it->second;
		const BYTE dayEventCount = dayData.size();
		buf->write(&dayIndex, sizeof(BYTE));
		buf->write(&dayEventCount, sizeof(BYTE));
		if (dayEventCount > 0)
			buf->write(dayData.data(), dayEventCount * sizeof(TEventManagerData));
	}
}
void CHARACTER_MANAGER::UpdateAllPlayerEventData()
{
	TEMP_BUFFER buf;
	CompareEventSendData(&buf);
	const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();
	for (auto it = c_ref_set.begin(); it != c_ref_set.end(); ++it)
	{
		LPCHARACTER ch = (*it)->GetCharacter();
		if (!ch)
			continue;
		(*it)->Packet(buf.read_peek(), buf.size());
	}
}
void CHARACTER_MANAGER::SendDataPlayer(LPCHARACTER ch)
{
	auto desc = ch->GetDesc();
	if (!desc)
		return;
	TEMP_BUFFER buf;
	CompareEventSendData(&buf);
	desc->Packet(buf.read_peek(), buf.size());

	const auto event = CheckEventIsActive(BEGGINER_EVENT, ch->GetEmpire());
	if (event != NULL && ch->GetQuestFlag("event_manager.is_new_player") == 0 && ch->GetLevel() == 0)
	{
		//value0 - itemvnum / value1 - itemcount
		ch->AutoGiveItem(event->value[0],event->value[1]);
		ch->ChatPacket(CHAT_TYPE_INFO,"1117");
		ch->SetQuestFlag("event_manager.is_new_player",1);
	}
	if(ch->GetLevel() == 0)
		ch->PointChange(POINT_LEVEL, 1);
}
bool CHARACTER_MANAGER::CloseEventManuel(BYTE eventIndex)
{
	auto eventPtr = CheckEventIsActive(eventIndex);
	if (eventPtr != NULL)
	{
		const BYTE subHeader = EVENT_MANAGER_REMOVE_EVENT;
		db_clientdesc->DBPacketHeader(HEADER_GD_EVENT_MANAGER, 0, sizeof(BYTE)+sizeof(WORD));
		db_clientdesc->Packet(&subHeader, sizeof(BYTE));
		db_clientdesc->Packet(&eventPtr->eventID, sizeof(WORD));
		return true;
	}
	return false;
}
void CHARACTER_MANAGER::SetEventStatus(const WORD eventID, const bool eventStatus, const int endTime, const char* endTimeText)
{
	//eventStatus - 0-deactive  // 1-active

	TEventManagerData* eventData = NULL;
	for (auto it = m_eventData.begin(); it != m_eventData.end(); ++it)
	{
		if (it->second.size())
		{
			for (DWORD j = 0; j < it->second.size(); ++j)
			{
				TEventManagerData& pData = it->second[j];
				if (pData.eventID == eventID)
				{
					eventData = &pData;
					break;
				}
			}
		}
	}
	if (eventData == NULL)
		return;
	eventData->eventStatus = eventStatus;
	eventData->endTime = endTime;
	strlcpy(eventData->endTimeText, endTimeText, sizeof(eventData->endTimeText));

	// Auto open&close notice
	const std::map<BYTE, std::pair<std::string, std::string>> m_eventText = {
		{BONUS_EVENT,{"1078","1079"}},
		{DOUBLE_BOSS_LOOT_EVENT,{"1080","1081"}},
		{DOUBLE_METIN_LOOT_EVENT,{"1082","1083"}},
		{DOUBLE_MISSION_BOOK_EVENT,{"1084","1085"}},
		{DUNGEON_COOLDOWN_EVENT,{"1086","1087"}},
		{DUNGEON_TICKET_LOOT_EVENT,{"1088","1089"}},
		{MOONLIGHT_EVENT,{"1090","1091"}},
	
		{CARD_EVENT,{"1111","1112"}},//AFTERSET
#ifdef ENABLE_MINI_GAME_CATCH_KING
		{CATCH_KING_EVENT,{"1113.","1114"}},//AFTERSET
#endif
		{TREASURE_KEY,{"1115","1116"}},
		{BEGGINER_EVENT,{"1118","1119"}},
		{EMPIRE_WAR_EVENT,{"Battleroyale has begin","Battleroyale has ended"}},
	};

	const auto it = m_eventText.find(eventData->eventIndex);
	if (it != m_eventText.end())
	{
		if (eventStatus)
			SendNotice(it->second.first.c_str());
		else
			SendNotice(it->second.second.c_str());
	}

	const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();

	// Bonus event update status
	if (eventData->eventIndex == BONUS_EVENT)
	{
		for (auto itDesc = c_ref_set.begin(); itDesc != c_ref_set.end(); ++itDesc)
		{
			LPCHARACTER ch = (*itDesc)->GetCharacter();
			if (!ch)
				continue;
			if (eventData->empireFlag != 0)
				if (eventData->empireFlag != ch->GetEmpire())
					continue;
			if (eventData->channelFlag != 0)
				if (eventData->channelFlag != g_bChannel)
					return;
			if (!eventStatus)
			{
				const long value = eventData->value[1];
				ch->ApplyPoint(eventData->value[0], -value);
			}
			ch->ComputePoints();
		}
	}

	if (eventData->eventIndex == EMPIRE_WAR_EVENT)
	{
		for (auto itDesc = c_ref_set.begin(); itDesc != c_ref_set.end(); ++itDesc)
		{
			LPCHARACTER ch = (*itDesc)->GetCharacter();
			if (!ch)
				continue;
			if (eventStatus)
			{
#ifdef __BL_BATTLE_ROYALE__
				BattleRoyale::Instance().RequestStateChange(ch, BattleRoyale::BR_STATE_REGISTER);
#endif
			}
			else {
#ifdef __BL_BATTLE_ROYALE__
				BattleRoyale::Instance().RequestStateChange(ch, BattleRoyale::BR_STATE_CLOSED);
#endif
			}
		}
	}

	const int now = time(0);
	const BYTE subIndex = EVENT_MANAGER_EVENT_STATUS;
	
	TPacketGCEventManager p;
	p.header = HEADER_GC_EVENT_MANAGER;
	p.size = sizeof(TPacketGCEventManager)+sizeof(BYTE)+sizeof(WORD)+sizeof(bool)+sizeof(int)+sizeof(int)+sizeof(eventData->endTimeText);
	
	TEMP_BUFFER buf;
	buf.write(&p, sizeof(TPacketGCEventManager));
	buf.write(&subIndex, sizeof(BYTE));
	buf.write(&eventData->eventID, sizeof(WORD));
	buf.write(&eventData->eventStatus, sizeof(bool));
	buf.write(&eventData->endTime, sizeof(int));
	buf.write(&eventData->endTimeText, sizeof(eventData->endTimeText));
	buf.write(&now, sizeof(int));

	for (auto it = c_ref_set.begin(); it != c_ref_set.end(); ++it)
	{
		LPCHARACTER ch = (*it)->GetCharacter();
		if (!ch)
			continue;
		(*it)->Packet(buf.read_peek(), buf.size());
	}
}
void CHARACTER_MANAGER::SetEventData(BYTE dayIndex, const std::vector<TEventManagerData>& m_data)
{
	const auto it = m_eventData.find(dayIndex);
	if (it == m_eventData.end())
		m_eventData.emplace(dayIndex, m_data);
	else
	{
		it->second.clear();
		for (DWORD j=0;j<m_data.size();++j)
			it->second.emplace_back(m_data[j]);
	}
}
#endif



#ifdef ENABLE_MULTI_FARM_BLOCK
void CHARACTER_MANAGER::CheckMultiFarmAccounts(const char* szIP)
{
	auto it = m_mapmultiFarm.find(szIP);
	if (it != m_mapmultiFarm.end())
	{
		auto itVec = it->second.begin();
		while(itVec != it->second.end())
		{
			LPCHARACTER ch = FindByPID(itVec->playerID);
			CCI* chP2P = P2P_MANAGER::Instance().FindByPID(itVec->playerID);
			if(!ch && !chP2P)
				itVec = it->second.erase(itVec);
			else
				++itVec;
		}
		if(!it->second.size())
			m_mapmultiFarm.erase(szIP);
	}
}
void CHARACTER_MANAGER::RemoveMultiFarm(const char* szIP, const DWORD playerID, const bool isP2P)
{
	if (!isP2P)
	{
		TPacketGGMultiFarm p;
		p.header = HEADER_GG_MULTI_FARM;
		p.subHeader = MULTI_FARM_REMOVE;
		p.playerID = playerID;
		strlcpy(p.playerIP, szIP, sizeof(p.playerIP));
		P2P_MANAGER::Instance().Send(&p, sizeof(TPacketGGMultiFarm));
	}
	
	auto it = m_mapmultiFarm.find(szIP);
	if (it != m_mapmultiFarm.end())
	{
		for (auto itVec = it->second.begin(); itVec != it->second.end(); ++itVec)
		{
			if (itVec->playerID == playerID)
			{
				it->second.erase(itVec);
				break;
			}
		}
		if (!it->second.size())
			m_mapmultiFarm.erase(szIP);
	}
}
void CHARACTER_MANAGER::SetMultiFarm(const char* szIP, const DWORD playerID, const char* playerName, const bool bStatus, const BYTE affectType, const int affectTime)
{
	const auto it = m_mapmultiFarm.find(szIP);
	if (it != m_mapmultiFarm.end())
	{
		for (auto itVec = it->second.begin(); itVec != it->second.end(); ++itVec)
		{
			if (itVec->playerID == playerID)
			{
				itVec->farmStatus = bStatus;
				itVec->affectType = affectType;
				itVec->affectTime = affectTime;
				return;
			}
		}
		it->second.emplace_back(TMultiFarm(playerID, playerName, bStatus, affectType, affectTime));
	}
	else
	{
		std::vector<TMultiFarm> m_vecFarmList;
		m_vecFarmList.emplace_back(TMultiFarm(playerID, playerName, bStatus, affectType, affectTime));
		m_mapmultiFarm.emplace(szIP, m_vecFarmList);
	}
}
int CHARACTER_MANAGER::GetMultiFarmCount(const char* playerIP, std::map<DWORD, std::pair<std::string, bool>>& m_mapNames)
{
	int accCount = 0;
	bool affectTimeHas = false;
	BYTE affectType = 0;
	const auto it = m_mapmultiFarm.find(playerIP);
	if (it != m_mapmultiFarm.end())
	{
		for (auto itVec = it->second.begin(); itVec != it->second.end(); ++itVec)
		{
			if (itVec->farmStatus)
				accCount++;
			if (itVec->affectTime > get_global_time())
				affectTimeHas = true;
			if (itVec->affectType > affectType)
				affectType = itVec->affectType;
			m_mapNames.emplace(itVec->playerID, std::make_pair(itVec->playerName, itVec->farmStatus));
		}
	}

	if (affectTimeHas && affectType > 0)
		accCount -= affectType;
	if (accCount < 0)
		accCount = 0;

	return accCount;
}
void CHARACTER_MANAGER::CheckMultiFarmAccount(const char* szIP, const DWORD playerID, const char* playerName, const bool bStatus, BYTE affectType, int affectDuration, bool isP2P)
{
	CheckMultiFarmAccounts(szIP);

	LPCHARACTER ch = FindByPID(playerID);
	if (ch && bStatus)
	{
		affectDuration = ch->FindAffect(AFFECT_MULTI_FARM_PREMIUM) ? get_global_time() + ch->FindAffect(AFFECT_MULTI_FARM_PREMIUM)->lDuration : 0;
		affectType = ch->FindAffect(AFFECT_MULTI_FARM_PREMIUM) ? ch->FindAffect(AFFECT_MULTI_FARM_PREMIUM)->lApplyValue : 0;
	}

	std::map<DWORD, std::pair<std::string, bool>> m_mapNames;
	int farmPlayerCount = GetMultiFarmCount(szIP, m_mapNames);
	if (bStatus)
	{
		if (farmPlayerCount >= 2)
		{
			CheckMultiFarmAccount(szIP, playerID, playerName, false);
			return;
		}
	}

	if (!isP2P)
	{
		TPacketGGMultiFarm p;
		p.header = HEADER_GG_MULTI_FARM;
		p.subHeader = MULTI_FARM_SET;
		p.playerID = playerID;
		strlcpy(p.playerIP, szIP, sizeof(p.playerIP));
		strlcpy(p.playerName, playerName, sizeof(p.playerIP));
		p.farmStatus = bStatus;
		p.affectType = affectType;
		p.affectTime = affectDuration;
		P2P_MANAGER::Instance().Send(&p, sizeof(TPacketGGMultiFarm));
	}

	SetMultiFarm(szIP, playerID, playerName, bStatus, affectType, affectDuration);
	if(ch)
	{
		ch->SetRewardStatus(bStatus);
	}

	m_mapNames.clear();
	farmPlayerCount = GetMultiFarmCount(szIP, m_mapNames);
	
	for (auto it = m_mapNames.begin(); it != m_mapNames.end(); ++it)
	{
		LPCHARACTER newCh = FindByPID(it->first);
		if (newCh)
		{
			newCh->ChatPacket(CHAT_TYPE_COMMAND, "UpdateMultiFarmAffect %d %d", newCh->GetRewardStatus(), newCh == ch ? true : false);
			for (auto itEx = m_mapNames.begin(); itEx != m_mapNames.end(); ++itEx)
			{
				if (itEx->second.second)
					newCh->ChatPacket(CHAT_TYPE_COMMAND, "UpdateMultiFarmPlayer %s", itEx->second.first.c_str());
			}
		}
	}
}
#endif


#ifdef RENEWAL_MISSION_BOOKS
void CHARACTER_MANAGER::GiveNewMission(LPITEM missionBook, LPCHARACTER ch)
{
	if (!missionBook || !ch)
		return;
	const DWORD missionBookItem = missionBook->GetVnum();
	if (ch->MissionCount() >= MISSION_BOOK_MAX )
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You can't read new mission book.");
		return;
	}
	std::vector<WORD> m_vecCanGetMissions;
#if __cplusplus < 199711L
	for (itertype(m_mapMissionData) it = m_mapMissionData.begin(); it != m_mapMissionData.end(); ++it)
#else
	for (auto it = m_mapMissionData.begin(); it != m_mapMissionData.end(); ++it)
#endif
	{
		if (it->second.missionItemIndex == missionBookItem)
		{
			if (!ch->IsMissionHas(it->second.id))
#if __cplusplus < 199711L
				m_vecCanGetMissions.push_back(it->second.id);
#else
				m_vecCanGetMissions.emplace_back(it->second.id);
#endif
		}
	}
	const int luckyIndex = m_vecCanGetMissions.size() > 1 ? number(0, m_vecCanGetMissions.size() - 1) : 0;
	if (luckyIndex + 1 > m_vecCanGetMissions.size())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You can't get more mission on this book!");
		return;
	}
	ch->GiveNewMission(m_vecCanGetMissions[luckyIndex]);
	missionBook->SetCount(missionBook->GetCount() - 1);
}
const TMissionBookData* CHARACTER_MANAGER::GetMissionData(WORD id)
{
#if __cplusplus < 199711L
	const itertype(m_mapMissionData) it = m_mapMissionData.find(id);
#else
	const auto it = m_mapMissionData.find(id);
#endif
	if (it != m_mapMissionData.end())
		return &it->second;
	return NULL;
}
void CHARACTER_MANAGER::LoadMissionBook()
{
	m_mapMissionData.clear();
	char filename[124];
	snprintf(filename, sizeof(filename), "locale/missionbook/data");
	FILE* fp;
	if ((fp = fopen(filename, "r")) != NULL)
	{
		char	one_line[256];
		TMissionBookData mission;
		while (fgets(one_line, 256, fp))
		{
			std::vector<std::string> m_vec;
			split_argument(one_line, m_vec);
			if (!m_vec.size())
				continue;

			if (m_vec[0].find("#") != std::string::npos)
				continue;

			if (m_vec[0].find("start") != std::string::npos)
			{
				memset(&mission, 0, sizeof(mission));
				memset(&mission.gold, 0, sizeof(mission.gold));
				memset(&mission.exp, 0, sizeof(mission.exp));
				memset(&mission.rewardItems, 0, sizeof(mission.rewardItems));
				memset(&mission.rewardCount, 0, sizeof(mission.rewardCount));
			}
			else if (m_vec[0] == "id")
			{
				if (m_vec.size() != 2)
					continue;
				str_to_number(mission.id, m_vec[1].c_str());
			}
			else if (m_vec[0] == "book")
			{
				if (m_vec.size() != 2)
					continue;
				str_to_number(mission.missionItemIndex, m_vec[1].c_str());
			}
			else if (m_vec[0] == "type")
			{
				if (m_vec.size() != 2)
					continue;
				else if (m_vec[1].find("monster") != std::string::npos)
					mission.type = MISSION_BOOK_TYPE_MONSTER;
				else if (m_vec[1].find("metin") != std::string::npos)
					mission.type = MISSION_BOOK_TYPE_METINSTONE;
				else if (m_vec[1].find("boss") != std::string::npos)
					mission.type = MISSION_BOOK_TYPE_BOSS;
			}
			else if (m_vec[0] == "levelrange")
			{
				if (m_vec.size() != 2)
					continue;
				str_to_number(mission.levelRange, m_vec[1].c_str());
			}
			else if (m_vec[0] == "subtype")
			{
				if (m_vec.size() != 2)
					continue;
				if (m_vec[1] != "all")
					str_to_number(mission.subtype, m_vec[1].c_str());
			}
			else if (m_vec[0] == "max")
			{
				if (m_vec.size() != 2)
					continue;
				str_to_number(mission.max, m_vec[1].c_str());
			}
			else if (m_vec[0] == "timer")
			{
				if (m_vec.size() != 2)
					continue;
				str_to_number(mission.maxTime, m_vec[1].c_str());
			}
			else if (m_vec[0] == "gold")
			{
				if (m_vec.size() != 3)
					continue;
				str_to_number(mission.gold[0], m_vec[1].c_str());
				str_to_number(mission.gold[1], m_vec[2].c_str());
			}
			else if (m_vec[0] == "exp")
			{
				if (m_vec.size() != 3)
					continue;
				str_to_number(mission.exp[0], m_vec[1].c_str());
				str_to_number(mission.exp[1], m_vec[2].c_str());
			}
			else if (m_vec[0] == "reward")
			{
				if (m_vec.size() != 3)
					continue;
				for (BYTE j = 0; j < 6; ++j)
				{
					if (mission.rewardItems[j] == 0)
					{
						str_to_number(mission.rewardItems[j], m_vec[1].c_str());
						str_to_number(mission.rewardCount[j], m_vec[2].c_str());
						break;
					}
				}
			}
			if (m_vec[0].find("end") != std::string::npos)
			{
				if (mission.id != 0)
#if __cplusplus < 199711L
					m_mapMissionData.insert(std::make_pair(mission.id, mission));
#else
					m_mapMissionData.emplace(mission.id, mission);
#endif
				memset(&mission, 0, sizeof(mission));
				memset(&mission.gold, 0, sizeof(mission.gold));
				memset(&mission.exp, 0, sizeof(mission.exp));
				memset(&mission.rewardItems, 0, sizeof(mission.rewardItems));
				memset(&mission.rewardCount, 0, sizeof(mission.rewardCount));
			}

		}
	}
}
#endif

#ifdef ENABLE_REWARD_SYSTEM
const char* GetRewardIndexToString(BYTE rewardIndex)
{
	std::map<BYTE, std::string> rewardNames = {
		{REWARD_120,"REWARD_120"},
		{REWARD_115WAR,"REWARD_115WAR"},
		{REWARD_115ASSASSIN,"REWARD_115ASSASSIN"},
		{REWARD_115SURA,"REWARD_115SURA"},
		{REWARD_115SHAMAN,"REWARD_115SHAMAN"},
		{REWARD_CUSTOM_SASH,"REWARD_CUSTOM_SASH"},
		{REWARD_FIRSTAURA,"REWARD_FIRSTAURA"},
		{REWARD_FIRSTENERGY,"REWARD_FIRSTENERGY"},
		{REWARD_PSKILL,"REWARD_PSKILL"},
		{REWARD_FIRSTBIO,"REWARD_FIRSTBIO"},
		{REWARD_FIRSTZODIACWEAPON,"REWARD_FIRSTZODIACWEAPON"},
		{REWARD_FIRSTZODIACARMOR,"REWARD_FIRSTZODIACARMOR"},
		{REWARD_75WEAPONAVG,"REWARD_75WEAPONAVG"},
		{REWARD_105WEAPONAVG,"REWARD_105WEAPONAVG"},
		{REWARD_JOTUN,"REWARD_JOTUN"},
		{REWARD_HYDRA,"REWARD_HYDRA"},
		{REWARD_ZODIAC,"REWARD_ZODIAC"},
		{REWARD_MELEY,"REWARD_MELEY"},
		{REWARD_AKZADUR,"REWARD_AKZADUR"},
		{REWARD_FIRSTPENDANTWHEEL,"REWARD_FIRSTPENDANTWHEEL"},
	};
	auto it = rewardNames.find(rewardIndex);
	if (it != rewardNames.end())
		return it->second.c_str();
	return 0;
}
BYTE GetRewardIndex(const char* szRewardName)
{
	std::map<std::string, BYTE> rewardNames = {
		{"REWARD_120",REWARD_120},
		{"REWARD_115WAR",REWARD_115WAR},
		{"REWARD_115ASSASSIN",REWARD_115ASSASSIN},
		{"REWARD_115SURA",REWARD_115SURA},
		{"REWARD_115SHAMAN",REWARD_115SHAMAN},
		{"REWARD_CUSTOM_SASH",REWARD_CUSTOM_SASH},
		{"REWARD_FIRSTAURA",REWARD_FIRSTAURA},
		{"REWARD_FIRSTENERGY",REWARD_FIRSTENERGY},
		{"REWARD_PSKILL",REWARD_PSKILL},
		{"REWARD_FIRSTBIO",REWARD_FIRSTBIO},
		{"REWARD_FIRSTZODIACWEAPON",REWARD_FIRSTZODIACWEAPON},
		{"REWARD_FIRSTZODIACARMOR",REWARD_FIRSTZODIACARMOR},
		{"REWARD_75WEAPONAVG",REWARD_75WEAPONAVG},
		{"REWARD_105WEAPONAVG",REWARD_105WEAPONAVG},
		{"REWARD_JOTUN",REWARD_JOTUN},
		{"REWARD_HYDRA",REWARD_HYDRA},
		{"REWARD_ZODIAC",REWARD_ZODIAC},
		{"REWARD_MELEY",REWARD_MELEY},
		{"REWARD_AKZADUR",REWARD_AKZADUR},
		{"REWARD_FIRSTPENDANTWHEEL",REWARD_FIRSTPENDANTWHEEL},
	};
	auto it = rewardNames.find(szRewardName);
	if (it != rewardNames.end())
		return it->second;
	return 0;
}
void CHARACTER_MANAGER::LoadRewardData()
{
	m_rewardData.clear();

	char szQuery[QUERY_MAX_LEN];
	snprintf(szQuery, sizeof(szQuery), "SELECT rewardIndex, lc_text, playerName, itemVnum0, itemCount0, itemVnum1, itemCount1, itemVnum2, itemCount2 FROM player.reward_table");
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));

	if (pMsg->Get()->uiNumRows != 0)
	{
		MYSQL_ROW row = NULL;
		for (int n = 0; (row = mysql_fetch_row(pMsg->Get()->pSQLResult)) != NULL; ++n)
		{
			int col = 0;
			TRewardInfo p;
			p.m_rewardItems.clear();

			char rewardName[50];
			DWORD rewardIndex;
			strlcpy(rewardName, row[col++], sizeof(rewardName));
			rewardIndex = GetRewardIndex(rewardName);

			strlcpy(p.lc_text, row[col++], sizeof(p.lc_text));
			strlcpy(p.playerName, row[col++], sizeof(p.playerName));
			for (BYTE j = 0; j < 3; ++j)
			{
				DWORD itemVnum, itemCount;
				str_to_number(itemVnum, row[col++]);
				str_to_number(itemCount, row[col++]);
				p.m_rewardItems.emplace_back(itemVnum, itemCount);
			}
			m_rewardData.emplace(rewardIndex, p);
		}
	}
}
bool CHARACTER_MANAGER::IsRewardEmpty(BYTE rewardIndex)
{
	auto it = m_rewardData.find(rewardIndex);
	if (it != m_rewardData.end())
	{
		if (strcmp(it->second.playerName, "") == 0)
			return true;
	}
	return false;
}
void CHARACTER_MANAGER::SendRewardInfo(LPCHARACTER ch)
{
	ch->SetProtectTime("RewardInfo", 1);
	std::string cmd="";//12
	if (m_rewardData.size())
	{
		for (auto it = m_rewardData.begin(); it != m_rewardData.end(); ++it)
		{
			if (strlen(it->second.playerName) > 1)
			{
				char text[60];
				snprintf(text, sizeof(text), "%d|%s#", it->first, it->second.playerName);
				cmd += text;
			}
			if (strlen(cmd.c_str()) + 12 > CHAT_MAX_LEN - 30)
			{
				ch->ChatPacket(CHAT_TYPE_COMMAND, "RewardInfo %s", cmd.c_str());
				cmd = "";
			}
		}
		if (strlen(cmd.c_str()) > 1)
			ch->ChatPacket(CHAT_TYPE_COMMAND, "RewardInfo %s", cmd.c_str());
	}
}

struct RewardForEach
{
	void operator() (LPDESC d)
	{
		LPCHARACTER ch = d->GetCharacter();
		if (ch == NULL)
			return;
		else if (ch->GetProtectTime("RewardInfo") != 1)
			return;
		CHARACTER_MANAGER::Instance().SendRewardInfo(ch);
	}
};
void CHARACTER_MANAGER::SetRewardData(BYTE rewardIndex, const char* playerName, bool isP2P)
{
	if (!IsRewardEmpty(rewardIndex))
		return;

	auto it = m_rewardData.find(rewardIndex);
	if (it == m_rewardData.end())
		return;
	TRewardInfo& rewardInfo = it->second;
	strlcpy(rewardInfo.playerName, playerName, sizeof(rewardInfo.playerName));

	if (isP2P)
	{
		LPCHARACTER ch = FindPC(playerName);
		if (ch)
		{
			for (DWORD j = 0; j < rewardInfo.m_rewardItems.size(); ++j)
				ch->AutoGiveItem(rewardInfo.m_rewardItems[j].first, rewardInfo.m_rewardItems[j].second);
		}

		char msg[CHAT_MAX_LEN+1];
		snprintf(msg, sizeof(msg), LC_TEXT(it->second.lc_text), playerName);
		// snprintf(msg, sizeof(msg), rewardInfo.lc_text, playerName);
		BroadcastNotice("New reward has been completed");

		TPacketGGRewardInfo p;
		p.bHeader = HEADER_GG_REWARD_INFO;
		p.rewardIndex = rewardIndex;
		strlcpy(p.playerName, playerName, sizeof(p.playerName));
		P2P_MANAGER::Instance().Send(&p, sizeof(p));

		char szQuery[1024];
		snprintf(szQuery, sizeof(szQuery), "UPDATE player.reward_table SET playerName = '%s' WHERE lc_text = '%s'", playerName, rewardInfo.lc_text);
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
	}
	const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();
	std::for_each(c_ref_set.begin(), c_ref_set.end(), RewardForEach());
}
#endif

#ifdef __DUNGEON_INFO__
bool sortByTime(const TDungeonRank& a, const TDungeonRank& b){return a.value < b.value;}
bool sortByVal(const TDungeonRank& a, const TDungeonRank& b){return a.value > b.value;}
void CHARACTER_MANAGER::SendDungeonRank(LPCHARACTER ch, DWORD mobIdx, BYTE rankIdx)
{
	const auto it = m_mapDungeonList.find(mobIdx);
	if (it == m_mapDungeonList.end())
		return;

	bool reLoad = false;

	auto itMob = m_mapDungeonRank.find(mobIdx);
	if (itMob == m_mapDungeonRank.end())
	{
		std::map<BYTE, std::pair<std::vector<TDungeonRank>, int>> m_data;
		m_mapDungeonRank.emplace(mobIdx, m_data);
		itMob = m_mapDungeonRank.find(mobIdx);
		reLoad = true;
	}

	auto itRank = itMob->second.find(rankIdx);
	if (itRank == itMob->second.end())
	{
		reLoad = true;
		std::pair<std::vector<TDungeonRank>, int> m_vec;
		itMob->second.emplace(rankIdx, m_vec);
		itRank = itMob->second.find(rankIdx);
	}

	if (!reLoad && itRank->second.second < time(0))
		reLoad = true;


	if (reLoad)
	{
		itRank->second.first.clear();
		char szQuery[1024];
		if(rankIdx == 0)
			snprintf(szQuery, sizeof(szQuery), "SELECT dwPID, lValue FROM player.quest WHERE szName = 'dungeon' and szState = '%u_completed' and lValue > 0 ORDER BY lValue DESC LIMIT 10", mobIdx);
		else if(rankIdx == 1)
			snprintf(szQuery, sizeof(szQuery), "SELECT dwPID, lValue FROM player.quest WHERE szName = 'dungeon' and szState = '%u_fastest' and lValue > 0 ORDER BY lValue ASC LIMIT 10", mobIdx);
		else if (rankIdx == 2)
			snprintf(szQuery, sizeof(szQuery), "SELECT dwPID, lValue FROM player.quest WHERE szName = 'dungeon' and szState = '%u_damage' and lValue > 0 ORDER BY lValue DESC LIMIT 10", mobIdx);

		std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));
		if (pMsg->Get()->pSQLResult)
		{
			MYSQL_ROW mRow;
			while (NULL != (mRow = mysql_fetch_row(pMsg->Get()->pSQLResult)))
			{
				TDungeonRank dungeonRank;
				memset(&dungeonRank, 0, sizeof(dungeonRank));
				DWORD playerID;
				str_to_number(playerID, mRow[0]);
				str_to_number(dungeonRank.value, mRow[1]);
				snprintf(szQuery, sizeof(szQuery), "SELECT name, level FROM player.player WHERE id = %d", playerID);
				std::unique_ptr<SQLMsg> pMsg2(DBManager::instance().DirectQuery(szQuery));
				MYSQL_ROW  playerRow = mysql_fetch_row(pMsg2->Get()->pSQLResult);
				if (playerRow)
				{
					strlcpy(dungeonRank.name, playerRow[0], sizeof(dungeonRank.name));
					str_to_number(dungeonRank.level, playerRow[1]);
				}
				itRank->second.first.emplace_back(dungeonRank);
			}
			if (itRank->second.first.size())
			{
				if(rankIdx == 1)
					std::sort(itRank->second.first.begin(), itRank->second.first.end(), sortByTime);
				else
					std::sort(itRank->second.first.begin(), itRank->second.first.end(), sortByVal);
			}
		}
		itRank->second.second = time(0) + (60 * 5);
	}

	std::string cmd("");

	for (BYTE j = 0; j < itRank->second.first.size(); ++j)
	{
		const auto& rank = itRank->second.first[j];

		cmd += rank.name;
		cmd += "|";
		cmd += std::to_string(rank.value);
		cmd += "|";
		cmd += std::to_string(rank.level);
		cmd += "#";
	}
	if (cmd == "")
		cmd = "-";
	ch->ChatPacket(CHAT_TYPE_COMMAND, "dungeon_log_info %u %u %s", mobIdx, rankIdx, cmd.c_str());
}
#endif

#ifdef __NEW_BANWORD__
void replace_new(std::string& str, const std::string& from, const std::string& to) {
	while (true)
	{
		const size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			break;
		str.replace(start_pos, from.length(), to);
	}
}
void CHARACTER_MANAGER::LoadBanword()
{
	m_mapBanwords.clear();
	char szQuery[QUERY_MAX_LEN];
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT * FROM player.banword_new"));
	if (pMsg && pMsg->Get()->uiNumRows != 0)
	{
		MYSQL_ROW row;

		char banText[250];
		int iChatBan, iHwidBan;
		while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
		{
			strlcpy(banText, row[0], sizeof(banText));
			str_to_number(iChatBan, row[1]);
			str_to_number(iHwidBan, row[2]);
			m_mapBanwords.emplace(banText, std::make_pair(iChatBan, iHwidBan == 1 ? true : false));
		}
	}
}
bool CHARACTER_MANAGER::IsBanword(LPCHARACTER ch, const char* szChat)
{
	std::string szText(szChat);
	std::transform(szText.begin(), szText.end(), szText.begin(), [](unsigned char c) { return std::tolower(c); });
	for (auto it = m_mapBanwords.begin(); it != m_mapBanwords.end(); ++it)
	{
		const std::string strText = it->first;
		if (szText.find(strText.c_str()) != std::string::npos)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "This text is forbidden in chat: %s", strText.c_str());
			if (it->second.first)
			{
				ch->AddAffect(AFFECT_BLOCK_CHAT, POINT_NONE, 0, AFF_NONE, it->second.first, 0, true);
				ch->ChatPacket(CHAT_TYPE_INFO, "You got block chat.");
			}
			if (it->second.second)//hwid ban
			{
				char chName[CHARACTER_NAME_MAX_LEN + 1];
				strlcpy(chName, ch->GetName(), sizeof(chName));

				DWORD account_id;
				char szEscapeName[CHARACTER_NAME_MAX_LEN * 2 + 1];
				DBManager::instance().EscapeString(szEscapeName, sizeof(szEscapeName), chName, strlen(chName));
				std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT player.account_id FROM player WHERE LOWER(name) like LOWER('%s') LIMIT 1", szEscapeName));
				if (pMsg->Get()->uiNumRows == 0)
					return true;

				MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
				str_to_number(account_id, row[0]);
				std::unique_ptr<SQLMsg> pMsg2(DBManager::instance().DirectQuery("SELECT hwid FROM account.account WHERE id = '%d' LIMIT 1", account_id));
				if (pMsg2->Get()->uiNumRows == 0)
					return true;

				MYSQL_ROW row2 = mysql_fetch_row(pMsg2->Get()->pSQLResult);
				if (strcmp(row2[0], "") == 0)
					return true;
				
				std::unique_ptr<SQLMsg> pMsgUpdate(DBManager::instance().DirectQuery("INSERT INTO account.hwid_ban (hwid, player, gm) VALUES ('%s', '%s', '%s')", row2[0], szEscapeName, "AUTO_BANWORD"));
				DESC_MANAGER::instance().DestroyDesc(ch->GetDesc());
			}

			return true;
		}
	}
	return false;
}

#endif

#ifdef ENABLE_BATTLEPASS
#include <fstream>
#include "locale_service.h"

#define ENUM_ENTRY(enum_val) {#enum_val, enum_val}

int32_t CHARACTER_MANAGER::GetBattlePassMissionIndex(const std::string& mission_text)
{
    static std::unordered_map<std::string, uint8_t> mission_index_map = {
        ENUM_ENTRY(MISSION_NONE),
        ENUM_ENTRY(MISSION_REACH_LEVEL),
        ENUM_ENTRY(MISSION_COLLECT_GOLD),
        ENUM_ENTRY(MISSION_COLLECT_EXP),
        ENUM_ENTRY(MISSION_KILL_PLAYER),
        ENUM_ENTRY(MISSION_KILL_MOB),
        ENUM_ENTRY(MISSION_CATCH_FISH),
        ENUM_ENTRY(MISSION_MINE),
        ENUM_ENTRY(MISSION_SEND_SHOUT_MESSAGE),
        ENUM_ENTRY(MISSION_UPGRADE_OBJECT),
        ENUM_ENTRY(MISSION_USE_ITEM),
        ENUM_ENTRY(MISSION_BATTLEPASS_MAX)
    };

    auto it = mission_index_map.find(mission_text);

    if (it != mission_index_map.end())
        return it->second;

	return -1;
}

void CHARACTER_MANAGER::LoadBattlePass()
{
    m_battle_pass_missions.clear();
    m_battle_pass_tier_map.clear();

    using js = nlohmann::json;
    char file_name[256 + 1];
    snprintf(file_name, sizeof(file_name), "locale/germany/battlepass/battlepass_config.json");

    std::ifstream ifs(file_name);
    if (!ifs.is_open())
    {
        sys_err("File named %s is not located!", file_name);
        return;
    }

    try
    {
        sys_log(0, "----Loading Battlepass Config ------");
        js jf = js::parse(ifs);

        if (jf.contains("general"))
        {
            js& general = jf["general"];

            if (general.contains("id") && general.contains("start_date") 
				&& general.contains("end_date"))
            {
				m_battlepass_info.id = general["id"];
                m_battlepass_info.start_date = general["start_date"];
				m_battlepass_info.end_date = general["end_date"];
            }
            else
            {
                sys_err("Error: Missing id or start_date or end_date in 'general' key");
            }
        }
        if (jf.contains("missions"))
        {
            js& missions = jf["missions"];

            if (missions.is_array())
            {
                for (const auto& mission : missions)
                {
                    if (mission.contains("mission_index") && mission.contains("mission_type") &&
                        mission.contains("target_object") && mission.contains("target_count") &&
                        mission.contains("mission_exp_reward"))
                    {
						const int32_t mission_type = GetBattlePassMissionIndex(mission["mission_type"].get<std::string>());

						if (mission_type == -1)
						{
                            sys_err("Invalid Mission_Type %s", mission["mission_type"].get<std::string>().c_str());
                            continue;
						}
                        uint16_t min_level = 1;
                        uint16_t max_level = gPlayerMaxLevel;

						if (mission.contains("min_level"))
                            min_level = mission["min_level"].get<uint16_t>();
                            
						if (mission.contains("max_level"))
                            max_level = mission["max_level"].get<uint16_t>();
                            
						m_battle_pass_missions.emplace(mission["mission_index"].get<uint32_t>(),
													 BattlePassMission(mission_type,
                                                                       mission["target_object"].get<uint32_t>(),
                                                                       mission["target_count"].get<uint64_t>(),
                                              mission["mission_exp_reward"].get<uint32_t>(), min_level, max_level));
                    }
                    else
                    {
                        sys_err("Error: Missing required fields in one of the missions");
                    }
                }
            }
            else
            {
                sys_err("Error: 'missions' is not an array");
            }
        }
        else
        {
            sys_err("Error: 'missions' key not found in JSON");
        }

		if (jf.contains("tiers"))
        {
            js& tiers = jf["tiers"];

            if (tiers.is_array())
            {
                for (const auto& tier : tiers)
                {
                    if (tier.contains("tier") && tier.contains("needed_exp") && tier.contains("reward_vnum") &&
                        tier.contains("reward_count") && tier.contains("reward_vnum_premium") &&
                        tier.contains("reward_premium_count"))
                    {
                        uint8_t tier_index = tier["tier"].get<uint8_t>();
                        uint32_t needed_exp = tier["needed_exp"].get<uint32_t>();
                        uint32_t reward_vnum = tier["reward_vnum"].get<uint32_t>();
                        uint32_t reward_count = tier["reward_count"].get<uint32_t>();
                        uint32_t reward_vnum_premium = tier["reward_vnum_premium"].get<uint32_t>();
                        uint32_t reward_premium_count = tier["reward_premium_count"].get<uint32_t>();

                        TierInfo tier_info(needed_exp, reward_vnum, reward_count, reward_vnum_premium,
                                           reward_premium_count);

                        m_battle_pass_tier_map.emplace(tier_index, tier_info);
                    }
                    else
                    {
                        sys_err("Error: Missing required fields in one of the tiers");
                    }
                }
            }
            else
            {
                sys_err("Error: 'tiers' is not an array");
            }
        }
        else
        {
            sys_err("Error: 'tiers' key not found in JSON");
        }
    }

    catch (const nlohmann::json::exception& e)
    {
        sys_err("%s", e.what());
    }
}
int32_t CHARACTER_MANAGER::GetMaxBattlePassLevel() const
{
    if (m_battle_pass_tier_map.empty())
        return 0;

	const int32_t battlePassLevelLimitFlag = quest::CQuestManager::instance().GetEventFlag("battle_pass_level_limit");

	if (battlePassLevelLimitFlag > 0)
		return battlePassLevelLimitFlag;

    auto max_tier_it = std::max_element(
        m_battle_pass_tier_map.begin(), 
        m_battle_pass_tier_map.end(),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        }
    );

    return static_cast<int32_t>(max_tier_it->first) + 1;
}
#endif
