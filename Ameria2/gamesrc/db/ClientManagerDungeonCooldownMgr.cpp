#include "stdafx.h"

#ifdef USE_DUNGEON_COOLDOWN_MANAGER
#include "ClientManager.h"

bool CClientManager::InitializeDungeonCooldownMgrTable()
{
	vecDungeonCooldown.clear();

	time_t timenow = time(0);

	char szQuery[512 + 1] = {};
	snprintf(szQuery, sizeof(szQuery), "DELETE FROM `dungeon_cooldown` WHERE `cooldown` <= %d;", timenow);

	std::unique_ptr<SQLMsg> pMsgClearExpired(CDBManager::Instance().DirectQuery(szQuery));

	if (!pMsgClearExpired || !pMsgClearExpired->Get())
	{
		sys_err("something went wrong (1).");
		return false;
	}

	if (pMsgClearExpired->uiSQLErrno != 0)
	{
		sys_err("errcode (%d)", pMsgClearExpired->uiSQLErrno);
		return false;
	}

	std::unique_ptr<SQLMsg> pMsg(CDBManager::Instance().DirectQuery("SELECT * FROM `dungeon_cooldown`;"));

	if (!pMsg || !pMsg->Get())
	{
		sys_err("something went wrong (2).");
		return false;
	}

	if (pMsg->uiSQLErrno != 0)
	{
		sys_err("errcode (%d)", pMsg->uiSQLErrno);
		return false;
	}

	m_mapDungeonCooldown.clear();

	auto pRes = pMsg->Get();
	if (pRes && pRes->uiNumRows > 0)
	{
		uint8_t c = 0;

		char szKey[32 + 1];
		TPacketDungeonCooldownMgr tmp;
		TPacketDungeonCooldownMgrSet tmp2;

		MYSQL_ROW mRow = nullptr;
		while ((mRow = mysql_fetch_row(pRes->pSQLResult)))
		{
			tmp = {};
			tmp2 = {};

			snprintf(szKey, sizeof(szKey), "%s", mRow[c]);
			strlcpy(tmp2.key, mRow[c], sizeof(tmp2.key));
			c++;

			strlcpy(tmp.hwid, mRow[c], sizeof(tmp.hwid));
			strlcpy(tmp2.info.hwid, mRow[c], sizeof(tmp2.info.hwid));
			c++;

			str_to_number(tmp.map_index, mRow[c]);
			str_to_number(tmp2.info.map_index, mRow[c]);
			c++;

			str_to_number(tmp.cooldown, mRow[c]);
			str_to_number(tmp2.info.cooldown, mRow[c]);
			c++;

			str_to_number(tmp.slot, mRow[c]);
			str_to_number(tmp2.info.slot, mRow[c]);
			c++;

			str_to_number(tmp.pid, mRow[c]);
			str_to_number(tmp2.info.pid, mRow[c]);

			m_mapDungeonCooldown.emplace(szKey, tmp);
			vecDungeonCooldown.push_back(tmp2);

			c = 0;
		}
	}

	return true;
}

void CClientManager::DungeonCooldownMgrUpdate()
{
	time_t timenow = time(0);

	std::vector<TPacketDungeonCooldownMgrExpire> tmpVec;

	TPacketDungeonCooldownMgrExpire tmp = {};

	auto it = m_mapDungeonCooldown.begin();
	while (it != m_mapDungeonCooldown.end())
	{
		auto v = it->second;
		if (v.cooldown <= timenow)
		{
			if (v.cooldown != 0)
			{
				strlcpy(tmp.key, it->first.c_str(), sizeof(tmp.key));
				tmpVec.push_back(tmp);
			}

			it = m_mapDungeonCooldown.erase(it);
			continue;
		}

		++it;
	}

	const int maxsize = tmpVec.size();
	if (maxsize > 0)
	{
		for (auto peer : m_peerList)
		{
			if (!peer || peer->IsAuth())
			{
				continue;
			}

			peer->EncodeHeader(HEADER_DG_DUNGEON_COOLDOWN_MGR_EXPIRE, 0, sizeof(int) + (sizeof(TPacketDungeonCooldownMgrExpire) * maxsize));
			peer->Encode(&maxsize, sizeof(int));
			peer->Encode(&tmpVec[0], sizeof(TPacketDungeonCooldownMgrExpire) * maxsize);
		}
	}
}

void CClientManager::DungeonCooldownMgrSet(CPeer* pPeer, const TPacketDungeonCooldownMgrSet* p)
{
	if (strlen(p->key) == 0)
	{
		sys_err("Something went wrong with the key.");
		return;
	}
	else if (p->info.cooldown == 0)
	{
		sys_err("Something went wrong with the cooldown == 0.");
		return;
	}

	bool exists = false;

	auto it = m_mapDungeonCooldown.find(p->key);
	if (it != m_mapDungeonCooldown.end())
	{
		it->second.cooldown = p->info.cooldown;
		exists = true;
	}

	char szQuery[1024 + 1] = {};
	snprintf(szQuery, sizeof(szQuery), "REPLACE INTO `dungeon_cooldown` (`key`, `hwid`, `mapindex`, `cooldown`, `slot`, `pid`) VALUES ('%s', '%s', %d, %u, %d, %u);", p->key, p->info.hwid, p->info.map_index, p->info.cooldown, p->info.slot, p->info.pid);

	std::unique_ptr<SQLMsg> pMsg(CDBManager::Instance().DirectQuery(szQuery));

	if (!pMsg || !pMsg->Get())
	{
		sys_err("cannot add cooldown key (%s), hwid (%s), map index (%d), cooldown (%d), slot (%d), pid (%u).", p->key, p->info.hwid, p->info.map_index, p->info.cooldown, p->info.slot, p->info.pid);
		return;
	}

	if (pMsg->uiSQLErrno != 0)
	{
		sys_err("errcode (%d) while adding cooldown key (%s), hwid (%s), map index (%d), cooldown (%d), slot (%d), pid (%u).", pMsg->uiSQLErrno, p->key, p->info.hwid, p->info.map_index, p->info.cooldown, p->info.slot, p->info.pid);
		return;
	}

	if (!exists)
	{
		TPacketDungeonCooldownMgr tmp = {};
		strlcpy(tmp.hwid, p->info.hwid, sizeof(tmp.hwid));
		tmp.map_index = p->info.map_index;
		tmp.cooldown = p->info.cooldown;

		m_mapDungeonCooldown.emplace(p->key, tmp);
	}

	TPacketDungeonCooldownMgrSet tmp2 = {};
	strlcpy(tmp2.key, p->key, sizeof(tmp2.key));
	strlcpy(tmp2.info.hwid, p->info.hwid, sizeof(tmp2.info.hwid));
	tmp2.info.map_index = p->info.map_index;
	tmp2.info.cooldown = p->info.cooldown;
	tmp2.info.slot = p->info.slot;
	tmp2.info.pid = p->info.pid;

	for (auto peer : m_peerList)
	{
		if (!peer || peer->IsAuth())
		{
			continue;
		}

		peer->EncodeHeader(HEADER_DG_DUNGEON_COOLDOWN_MGR_ADD, 0, sizeof(TPacketDungeonCooldownMgrSet));
		peer->Encode(&tmp2, sizeof(TPacketDungeonCooldownMgrSet));
	}
}

void CClientManager::DungeonCooldownMgrExpire(CPeer* pPeer, const TPacketDungeonCooldownMgrExpire* p, uint32_t dwLength)
{
	if ((dwLength % sizeof(TPacketDungeonCooldownMgrExpire)) != 0)
	{
		sys_err("invalid packet size %u, sizeof(TPacketDungeonCooldownMgrExpire) == %d", dwLength, sizeof(TPacketDungeonCooldownMgrExpire));
		return;
	}

	const int maxsize = dwLength / sizeof(TPacketDungeonCooldownMgrExpire);
	if (maxsize < 1)
	{
		sys_err("Something went wrong with the maxsize (%d).", maxsize);
		return;
	}

	std::vector<TPacketDungeonCooldownMgrExpire> tmpVec;

	TPacketDungeonCooldownMgrExpire tmp = {};

	char szQuery[512 + 1] = {};

	for (int i = 0; i < maxsize; ++i, ++p)
	{
		auto it = m_mapDungeonCooldown.find(p->key);
		if (it != m_mapDungeonCooldown.end())
		{
			it->second.cooldown = 0;
		}

		strlcpy(tmp.key, p->key, sizeof(tmp.key));
		tmpVec.push_back(tmp);

		snprintf(szQuery, sizeof(szQuery), "DELETE FROM `dungeon_cooldown` WHERE `key` = '%s';", p->key);

		std::unique_ptr<SQLMsg> pMsg(CDBManager::Instance().DirectQuery(szQuery));

		if (!pMsg || !pMsg->Get())
		{
			sys_err("cannot remove cooldown key (%s).", p->key);
			continue;
		}

		if (pMsg->uiSQLErrno != 0)
		{
			sys_err("errcode (%d) while removing cooldown key (%s).", pMsg->uiSQLErrno, p->key);
			continue;
		}
	}

	const auto ressize = tmpVec.size();
	if (ressize > 0)
	{
		for (auto peer : m_peerList)
		{
			if (!peer || peer->IsAuth())
			{
				continue;
			}

			peer->EncodeHeader(HEADER_DG_DUNGEON_COOLDOWN_MGR_EXPIRE, 0, sizeof(int) + (sizeof(TPacketDungeonCooldownMgrExpire) * maxsize));
			peer->Encode(&maxsize, sizeof(int));
			peer->Encode(&tmpVec[0], sizeof(TPacketDungeonCooldownMgrExpire) * ressize);
		}
	}
}
#endif
