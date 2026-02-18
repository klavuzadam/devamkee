#include "stdafx.h"
#include "constants.h"
#include "packet.h"
#include "desc.h"
#include "char.h"
#include "p2p.h"
#include "config.h"
#include "RankPlayer.h"
#include "db.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char_manager.h"
#include <boost/algorithm/string.hpp>
#include "guild.h"

RankPlayer::RankPlayer()
{
}

RankPlayer::~RankPlayer()
{
	// clear map while closing server
	m_info_player.clear();
}

void RankPlayer::Initialize() // Initalize (start server load)
{
	if (g_bAuthServer)
		return;

	m_info_player.clear();
	
	// Syntax :: rank|name_table|where_table|select_tables|order_by_tables
#ifdef ENABLE_YOHARA_SYSTEM
	LoadPlayerValue(RANK_BY_LEVEL, "player", "id >= 0", "id|level|conquerorlevel", "playtime");
#else
	LoadPlayerValue(RANK_BY_LEVEL, "player", "id >= 0", "id|level", "playtime");
#endif
	LoadPlayerValue(RANK_BY_YANG, "player", "id >= 0", "id|gold", "gold");
	LoadPlayerValue(RANK_BY_STONE, "player_ranking", "rank = 2", "dwPID|lValue", "lValue");
	LoadPlayerValue(RANK_BY_BOSS, "player_ranking", "rank = 3", "dwPID|lValue", "lValue");
	LoadPlayerValue(RANK_BY_MONSTER, "player_ranking", "rank = 4", "dwPID|lValue", "lValue");
	LoadPlayerValue(RANK_BY_DUNGEON_COMPLETE, "player_ranking", "rank = 5", "dwPID|lValue", "lValue");
	LoadPlayerValue(RANK_BY_FISH_CAUGHT, "player_ranking", "rank = 6", "dwPID|lValue", "lValue");
	LoadPlayerValue(RANK_BY_EXTRACTION, "player_ranking", "rank = 7", "dwPID|lValue", "lValue");
	LoadPlayerValue(RANK_BY_BONUS_CHANGED, "player_ranking", "rank = 8", "dwPID|lValue", "lValue");
	LoadPlayerValue(RANK_BY_BOX_OPENED, "player_ranking", "rank = 9", "dwPID|lValue", "lValue");
	LoadPlayerValue(RANK_BY_ITEM_IMPROVED, "player_ranking", "rank = 10", "dwPID|lValue", "lValue");
	LoadPlayerValue(RANK_BY_DEATH, "player_ranking", "rank = 11", "dwPID|lValue", "lValue");
}

void RankPlayer::LoadPlayerValue(int iMode, const char* szNameTable, const char* szWhereExcept, const char* szSelect, const char* szNameColumn)
{
	std::vector<std::string> stArgs;
	boost::split(stArgs, szSelect, boost::is_any_of("|"));

	char query[QUERY_MAX_LEN];
	sprintf(query, "SELECT %s", stArgs[0].c_str());

	for (size_t i = 1; i < stArgs.size(); ++i)
		sprintf(query, "%s, %s", query, stArgs[i].c_str());

	sprintf(query, "%s FROM %s WHERE %s ORDER BY %s DESC", query, szNameTable, szWhereExcept, szNameColumn);
    // sprintf(query, "%s FROM %s WHERE %s AND %s NOT IN (SELECT mName FROM common.gmlist) ORDER BY %s DESC", query, szNameTable, szWhereExcept, szNameColumn, szNameColumn);

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(query));

	if (!pMsg || pMsg->Get()->uiNumRows == 0)
		return;

	MYSQL_ROW row = NULL;
	while ((row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		uint32_t dwPid = 0;
		int iVal = 0;
		str_to_number(dwPid, row[0]);

		int64_t lYang = 0;
		
		// Check the account status in account.account
		bool isAccountBlocked = false;
		if (LoadAccountStatus(dwPid, isAccountBlocked) && !isAccountBlocked) {

			// Try Load Info About Player
			LoadInfoPlayer(dwPid);

			// Check if the player's name is in the common.gmlist table
			const char* playerName = m_info_player[dwPid].szName; // Get the player's name from LoadInfoPlayer
			if (IsPlayerInGmList(playerName)) {
				// Player is in the common.gmlist table, so consider them not ranked
				continue;
			}


			switch (iMode)
			{
				case RANK_BY_LEVEL:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].bLevel = iVal;

#ifdef ENABLE_YOHARA_SYSTEM
					str_to_number(iVal, row[2]);

					if (iVal > 0)
					{
						m_info_player[dwPid].bLevel = gPlayerMaxLevel + iVal;
					}
#endif

					break;
				}
				case RANK_BY_YANG:
				{
					str_to_number(lYang, row[1]);
					m_info_player[dwPid].lYang = lYang;
					break;
				}
				case RANK_BY_STONE:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].iStone = iVal;
					break;
				}
				case RANK_BY_BOSS:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].iBoss = iVal;
					break;
				}
				case RANK_BY_MONSTER:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].iMonster = iVal;
					break;
				}
				case RANK_BY_DUNGEON_COMPLETE:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].iDungeon = iVal;
					break;
				}
				case RANK_BY_FISH_CAUGHT:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].iFish = iVal;
					break;
				}
				case RANK_BY_EXTRACTION:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].iExtraction = iVal;
					break;
				}
				case RANK_BY_BONUS_CHANGED:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].iBonus = iVal;
					break;
				}
				case RANK_BY_BOX_OPENED:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].iBox = iVal;
					break;
				}
				case RANK_BY_ITEM_IMPROVED:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].iImprove = iVal;
					break;
				}
				case RANK_BY_DEATH:
				{
					str_to_number(iVal, row[1]);
					m_info_player[dwPid].iDeath = iVal;
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
}

void RankPlayer::LoadInfoPlayer(uint32_t dwPID, bool bReload/* = false*/)
{
	if (dwPID == 0)
	{
		return;
	}

	auto it = m_info_player.find(dwPID);
	if (it != m_info_player.end())
	{
		if (it->second.bLoaded && !bReload)
		{
			return;
		}
	}
	else if (bReload)
	{
		return;
	}

	LPCHARACTER ch = CHARACTER_MANAGER::Instance().FindByPID(dwPID);
	if (ch)
	{
		strlcpy(m_info_player[dwPID].szName, ch->GetName(), sizeof(m_info_player[dwPID].szName));
		m_info_player[dwPID].bLevel = ch->GetLevel();

#ifdef ENABLE_YOHARA_SYSTEM
		BYTE bConquerorLevel = ch->GetConquerorLevel();
		if (bConquerorLevel > 0)
		{
			m_info_player[dwPID].bLevel = gPlayerMaxLevel + bConquerorLevel;
		}
#endif

		m_info_player[dwPID].bEmpire = bReload ? GetEmpire(ch->GetAID()) : ch->GetEmpire();
		strlcpy(m_info_player[dwPID].szGuildName, ch->GetGuild() ? ch->GetGuild()->GetName() : "-", sizeof(m_info_player[dwPID].szName));
		m_info_player[dwPID].bLoaded = true;

		// if (bReload)
		// {
			// TPacketGGPlayerRankReload pRld;
			// pRld.bHeader = HEADER_GG_PLAYER_RANK_RELOAD;
			// pRld.dwPID = dwPID;

			// P2P_MANAGER::instance().Send(&pRld, sizeof(pRld));
		// }
	}
	else
	{
#ifdef ENABLE_YOHARA_SYSTEM
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT `account_id`, `name`, `level`, `conquerorlevel` FROM `player` WHERE `id` = %u LIMIT 1;", dwPID));
#else
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT `account_id`, `name`, `level` FROM `player` WHERE `id` = %u LIMIT 1;", dwPID));
#endif

		if (!pMsg->Get() || pMsg->Get()->uiNumRows == 0)
		{
			return;
		}

		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		if (!row)
		{
			return;
		}

		DWORD dwAID = 0;
		str_to_number(dwAID, row[0]);

		if (dwAID == 0)
		{
			return;
		}

		const auto iEmpire = GetEmpire(dwAID);
		if (iEmpire == -1)
		{
			return;
		}

		m_info_player[dwPID].bEmpire = iEmpire;
		strlcpy(m_info_player[dwPID].szName, row[1], sizeof(m_info_player[dwPID].szName));

		BYTE bLevel = 0;
		str_to_number(bLevel, row[2]);

		m_info_player[dwPID].bLevel = bLevel;

#ifdef ENABLE_YOHARA_SYSTEM
		BYTE bConquerorLevel = 0;
		str_to_number(bConquerorLevel, row[3]);

		if (bConquerorLevel > 0)
		{
			m_info_player[dwPID].bLevel = gPlayerMaxLevel + bConquerorLevel;
		}
#endif

		std::unique_ptr<SQLMsg> pMsgGuild(DBManager::instance().DirectQuery("SELECT `name` FROM `guild` WHERE `id` = (SELECT `guild_id` FROM `guild_member` WHERE `pid` = %u) LIMIT 1;", dwPID));

		if (!pMsgGuild->Get() || pMsgGuild->Get()->uiNumRows == 0)
		{
			strlcpy(m_info_player[dwPID].szGuildName, "-", sizeof(m_info_player[dwPID].szGuildName));
			return;
		}
		else
		{
			row = mysql_fetch_row(pMsgGuild->Get()->pSQLResult);
			strlcpy(m_info_player[dwPID].szGuildName, row[0], sizeof(m_info_player[dwPID].szGuildName));
		}

		m_info_player[dwPID].bLoaded = true;

		// if (bReload)
		// {
			// TPacketGGPlayerRankReload pRld;
			// pRld.bHeader = HEADER_GG_PLAYER_RANK_RELOAD;
			// pRld.dwPID = dwPID;

			// P2P_MANAGER::instance().Send(&pRld, sizeof(pRld));
		// }
	}
}

bool RankPlayer::IsPlayerInGmList(const char* playerName)
{
    char query[QUERY_MAX_LEN];
    sprintf(query, "SELECT mName FROM common.gmlist WHERE mName = '%s'", playerName);

    std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(query));

    if (!pMsg || pMsg->Get()->uiNumRows == 0)
        return false;

    return true;
}

bool RankPlayer::LoadAccountStatus(uint32_t dwPlayerID, bool& isBlocked)
{
    // Retrieve the account_id associated with the player ID
    std::unique_ptr<SQLMsg> pMsgAccountID(DBManager::instance().DirectQuery("SELECT account_id FROM player.player WHERE id = %d", dwPlayerID));

    if (pMsgAccountID->Get()->uiNumRows == 0)
        return false;

    MYSQL_ROW rowAccountID = NULL;
    if ((rowAccountID = mysql_fetch_row(pMsgAccountID->Get()->pSQLResult)))
    {
        uint32_t dwAccountID = 0;
        str_to_number(dwAccountID, rowAccountID[0]);

        // Query the status from the account.account table using the retrieved account_id
        std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT status FROM account.account WHERE id = %d", dwAccountID));

        if (pMsg->Get()->uiNumRows == 0)
            return false;

        MYSQL_ROW row = NULL;
        if ((row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
        {
            const char* szStatus = row[0]; // Assuming status is a string
            isBlocked = (strcmp(szStatus, "BLOCK") == 0);
            return true;
        }
    }
    return false;
}

int RankPlayer::GetEmpire(uint32_t dwAcc) const
{
	std::unique_ptr<SQLMsg> pMsgEmpire(DBManager::instance().DirectQuery("SELECT empire FROM player.player_index WHERE id = %u", dwAcc));
	
	if (pMsgEmpire->Get()->uiNumRows == 0)
		return -1;
	
	MYSQL_ROW row = NULL;
	int iEmpire = 0;
	while ((row = mysql_fetch_row(pMsgEmpire->Get()->pSQLResult)))
	{
		str_to_number(iEmpire, row[0]);
	}
	
	return iEmpire;
}

void RankPlayer::SetCachedStatusPlayer(uint32_t dwPID, bool bStatus)
{
	
}

bool RankPlayer::IsCachedPlayer(uint32_t dwPID)
{
	return false;
}

int64_t RankPlayer::GetProgressByPID(uint32_t dwPid, int iMode)
{
	auto it = m_info_player.find(dwPid);
	
	if (it == m_info_player.end())
		return 0;
	
	switch(iMode)
	{
		case RANK_BY_STONE:
			return it->second.iStone;
		case RANK_BY_BOSS:
			return it->second.iBoss;
		case RANK_BY_MONSTER:
			return it->second.iMonster;
		case RANK_BY_DUNGEON_COMPLETE:
			return it->second.iDungeon;
		case RANK_BY_FISH_CAUGHT:
			return it->second.iFish;
		case RANK_BY_EXTRACTION:
			return it->second.iExtraction;
		case RANK_BY_BONUS_CHANGED:
			return it->second.iBonus;
		case RANK_BY_BOX_OPENED:
			return it->second.iBox;
		case RANK_BY_ITEM_IMPROVED:
			return it->second.iImprove;
		case RANK_BY_DEATH:
			return it->second.iDeath;

		default:
			return -1; // SKIP_RANK_BECAUSE_ITS_NON_IN_LIST
	}

	return -1; // SKIP_RANK_BECAUSE_ITS_NON_IN_LIST
}

void RankPlayer::ReceiveP2P(int iMode, uint32_t dwPid, const char* szName, uint8_t bEmpire, int64_t lValue, const char* szGuildName)
{
	m_info_player[dwPid].bEmpire = bEmpire;
	
	switch(iMode)
	{
		case RANK_BY_LEVEL:
			m_info_player[dwPid].bLevel = lValue;
			break;
		case RANK_BY_YANG:
			m_info_player[dwPid].lYang = lValue;
			break;
		case RANK_BY_STONE:
			m_info_player[dwPid].iStone = lValue;
			break;
		case RANK_BY_BOSS:
			m_info_player[dwPid].iBoss = lValue;
			break;
		case RANK_BY_MONSTER:
			m_info_player[dwPid].iMonster = lValue;
			break;
		case RANK_BY_DUNGEON_COMPLETE:
			m_info_player[dwPid].iDungeon = lValue;
			break;
		case RANK_BY_FISH_CAUGHT:
			m_info_player[dwPid].iFish = lValue;
			break;
		case RANK_BY_EXTRACTION:
			m_info_player[dwPid].iExtraction = lValue;
			break;
		case RANK_BY_BONUS_CHANGED:
			m_info_player[dwPid].iBonus = lValue;
			break;
		case RANK_BY_BOX_OPENED:
			m_info_player[dwPid].iBox = lValue;
			break;
		case RANK_BY_ITEM_IMPROVED:
			m_info_player[dwPid].iImprove = lValue;
			break;
		case RANK_BY_DEATH:
			m_info_player[dwPid].iDeath = lValue;
			break;
	}

	strlcpy(m_info_player[dwPid].szName, szName, sizeof(m_info_player[dwPid].szName));
	strlcpy(m_info_player[dwPid].szGuildName, szGuildName, sizeof(m_info_player[dwPid].szGuildName));
}

void RankPlayer::SendInfoPlayer(LPCHARACTER ch, int iMode, int64_t lValue, bool bCanP2P)
{
	if (!ch)
		return;
	
	if (ch->GetGMLevel() != GM_PLAYER)
		return;
	
	ReceiveP2P(iMode, ch->GetPlayerID(), ch->GetName(), ch->GetEmpire(), lValue, ch->GetGuild() ? ch->GetGuild()->GetName() : "-");
	
	if (!bCanP2P)
		return;
	
	TPacketGGPlayerRank p2;

	p2.bHeader = HEADER_GG_PLAYER_RANK;
	strlcpy(p2.szName, ch->GetName(), sizeof(p2.szName));
	p2.bEmpire = ch->GetEmpire();
	p2.dwPid = ch->GetPlayerID();
	p2.lValue = lValue;
	p2.iMode = iMode;
	strlcpy(p2.szGuildName, ch->GetGuild() ? ch->GetGuild()->GetName() : "-", sizeof(p2.szGuildName));

	P2P_MANAGER::instance().Send(&p2, sizeof(TPacketGGPlayerRank));
}

bool CompareRankPlayerByLevel(SInfoPlayer i, SInfoPlayer j)
{
	return i.bLevel > j.bLevel;
}

bool CompareRankPlayerByYang(SInfoPlayer i, SInfoPlayer j)
{
	return i.lYang > j.lYang;
}

bool CompareRankPlayerByStone(SInfoPlayer i, SInfoPlayer j)
{
	return i.iStone > j.iStone;
}

bool CompareRankPlayerByBoss(SInfoPlayer i, SInfoPlayer j)
{
	return i.iBoss > j.iBoss;
}

bool CompareRankPlayerByMonster(SInfoPlayer i, SInfoPlayer j)
{
	return i.iMonster > j.iMonster;
}

bool CompareRankPlayerByDungeon(SInfoPlayer i, SInfoPlayer j)
{
	return i.iDungeon > j.iDungeon;
}

bool CompareRankPlayerByFish(SInfoPlayer i, SInfoPlayer j)
{
	return i.iFish > j.iFish;
}

bool CompareRankPlayerByExtraction(SInfoPlayer i, SInfoPlayer j)
{
	return i.iExtraction > j.iExtraction;
}

bool CompareRankPlayerByBonus(SInfoPlayer i, SInfoPlayer j)
{
	return i.iBonus > j.iBonus;
}

bool CompareRankPlayerByBox(SInfoPlayer i, SInfoPlayer j)
{
	return i.iBox > j.iBox;
}

bool CompareRankPlayerByImprove(SInfoPlayer i, SInfoPlayer j)
{
	return i.iImprove > j.iImprove;
}

bool CompareRankPlayerByDeath(SInfoPlayer i, SInfoPlayer j)
{
	return i.iDeath > j.iDeath;
}

void RankPlayer::RequestInfoRank(LPCHARACTER ch, int iMode, int iFromPos)
{
	if (!ch || !ch->GetDesc())
		return;

	int iMyPos = -1;
	
	std::vector<SInfoPlayer> vec_cache;

	for (std::map<uint32_t, SInfoPlayer>::iterator it = m_info_player.begin(); it != m_info_player.end(); ++it)
	{
		if (*it->second.szName != '\0' && strlen(it->second.szName) > 1)
		{
			vec_cache.push_back(it->second);
		}
	}

	if (vec_cache.empty())
	{
		return;
	}

	int iSize = vec_cache.size();

	if (iSize > SHOW_MAX_LIMIT + 1)
		iSize = SHOW_MAX_LIMIT + 1;

	TPacketGCRankInfo pack;
	pack.bHeader = HEADER_GC_RANK_INFO;
	pack.iMode = iMode;
	
	switch(iMode)
	{
		case RANK_BY_LEVEL:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByLevel);
	
			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].bLevel;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].bLevel;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_YANG:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByYang);
			for (int iPos = 0; iPos < iSize; ++iPos)
			{	
				pack.lValue = vec_cache[iPos].lYang;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].lYang;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_STONE:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByStone);
			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].iStone;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].iStone;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_BOSS:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByBoss);
			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].iBoss;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].iBoss;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_MONSTER:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByMonster);
			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].iMonster;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].iMonster;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_DUNGEON_COMPLETE:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByDungeon);
			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].iDungeon;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].iDungeon;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_FISH_CAUGHT:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByFish);
			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].iFish;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].iFish;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_EXTRACTION:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByExtraction);
			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].iExtraction;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].iExtraction;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_BONUS_CHANGED:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByBonus);
			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].iBonus;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].iBonus;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_BOX_OPENED:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByBox);
			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].iBox;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].iBox;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_ITEM_IMPROVED:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByImprove);
			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].iImprove;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].iImprove;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
			
		case RANK_BY_DEATH:
			std::stable_sort(vec_cache.begin(), vec_cache.end(), CompareRankPlayerByDeath);

			for (int iPos = 0; iPos < iSize; ++iPos)
			{
				pack.lValue = vec_cache[iPos].iDeath;
				strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
				pack.iMyPos = -1;
				pack.iPos = iPos + 1;
				pack.bEmpire = vec_cache[iPos].bEmpire;

				pack.bLevel = vec_cache[iPos].bLevel;
				strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
			
			for (int iPos = 0; iPos < vec_cache.size(); ++iPos)
			{
				if (!strcmp(vec_cache[iPos].szName, ch->GetName()))
				{
					pack.lValue = vec_cache[iPos].iDeath;
					strlcpy(pack.szName, vec_cache[iPos].szName, sizeof(pack.szName));
					pack.iMyPos = iPos + 1;
					pack.iPos = iPos + 1;
					pack.bEmpire = vec_cache[iPos].bEmpire;

					pack.bLevel = vec_cache[iPos].bLevel;
					strlcpy(pack.szGuildName, vec_cache[iPos].szGuildName, sizeof(pack.szGuildName));

					ch->GetDesc()->Packet(&pack, sizeof(pack));
					break;
				}
			}
			break;
	}
}
