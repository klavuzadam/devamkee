#include "stdafx.h"

#ifdef USE_DUNGEON_COOLDOWN_MANAGER
#include "char.h"
#include "config.h"
#include "desc.h"
#include "desc_client.h"
#include "desc_manager.h"

#include "dungeon_cooldown_mgr.hpp"

#include "new_md5.h"

void CDungeonHwidCooldown::Update()
{
	
}

uint32_t CDungeonHwidCooldown::GetCooldownTime() const
{
	return m_tCooldownTime;
}

bool CDungeonHwidCooldown::SetCooldownTime(const uint32_t cooldowntime)
{
	m_tCooldownTime = cooldowntime;
	Update();
	return true;
}

CDungeonHwidCooldown::CDungeonHwidCooldown(const char* cpszHwid, const int32_t mapindex, const uint32_t cooldowntime, const int slot, const uint32_t pid)
	: m_iMapIndex(mapindex), m_tCooldownTime(cooldowntime), m_iSlot(slot), m_dwPID(pid)
{
	m_strHwid = cpszHwid;
}

CDungeonHwidCooldown::~CDungeonHwidCooldown()
{
	m_strHwid.clear();
}

int8_t CDungeonCooldownMgr::Set(LPCHARACTER ch, const int32_t mapindex, const uint32_t cooldowntime)
{
	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return -2;
	}

	const char* cpszHwid = ch->GetHardwareID();
	if (strlen(cpszHwid) == 0)
	{
		return -3;
	}

	bool bFound = false;

	const auto timenow = get_global_time();

	char szText[HWID_MAX_LEN * 2] = {};
	char szTextMD5[MAX_MD5_LEN + 1] = {};

	uint32_t cooldownTimes[MAX_PID_PER_HWID_DUNGEON_COOLDOWN];

	for (int i = 0; i < MAX_PID_PER_HWID_DUNGEON_COOLDOWN; i++)
	{
		cooldownTimes[i] = 0;

		snprintf(szText, sizeof(szText), "%s%d%d", cpszHwid, mapindex, i);

		MD5 md5;
		snprintf(szTextMD5, sizeof(szTextMD5), "%s", md5.digestString((char*)szText));

		auto it = m_mapCooldown.find(szTextMD5);
		if (it != m_mapCooldown.end())
		{
			cooldownTimes[i] = it->second.get() ? it->second.get()->GetCooldownTime() : (timenow + 3600);

			if (!bFound)
			{
				bFound = true;
			}
		}
	}

	bool bDone = false;
	int slot = 0;
	uint32_t cooldownTimeNow = 0;

	if (bFound)
	{
		for (int i = 0; i < MAX_PID_PER_HWID_DUNGEON_COOLDOWN; i++)
		{
			if (!bDone)
			{
				slot = i;
				cooldownTimeNow = cooldownTimes[i];

				bDone = true;
				continue;
			}

			if (cooldownTimes[i] < cooldownTimeNow)
			{
				slot = i;
				cooldownTimeNow = cooldownTimes[i];
			}
		}
	}

	snprintf(szText, sizeof(szText), "%s%d%d", cpszHwid, mapindex, slot);

	MD5 md5;
	snprintf(szTextMD5, sizeof(szTextMD5), "%s", md5.digestString((char*)szText));

	auto it = m_mapCooldown.find(szTextMD5);

	if (cooldowntime == 0)
	{
		TPacketDungeonCooldownMgrExpire p = {};
		strlcpy(p.key, szTextMD5, sizeof(p.key));

		if (it != m_mapCooldown.end())
		{
			auto c = it->second ? it->second.get() : nullptr;
			if (c)
			{
				if (c->SetCooldownTime(0))
				{
					
				}
			}

			m_mapCooldown.erase(it);
		}

		db_clientdesc->DBPacketHeader(HEADER_GD_DUNGEON_COOLDOWN_MGR_EXPIRE, 0, sizeof(TPacketDungeonCooldownMgrExpire));
		db_clientdesc->Packet(&p, sizeof(TPacketDungeonCooldownMgrExpire));
	}
	else
	{
		const auto pid = ch->GetPlayerID();

		if (it != m_mapCooldown.end())
		{
			auto v = it->second.get();
			if (v)
			{
				if (v->SetCooldownTime(cooldowntime))
				{
					
				}
			}
			else
			{
				auto c = it->second ? it->second.get() : nullptr;
				if (c)
				{
					if (c->SetCooldownTime(0))
					{
						
					}
				}

				m_mapCooldown.erase(it);

				auto cooldown = std::make_unique<CDungeonHwidCooldown>(cpszHwid, mapindex, cooldowntime, slot, pid);
				m_mapCooldown.emplace(szTextMD5, std::move(cooldown));
			}
		}
		else
		{
			auto cooldown = std::make_unique<CDungeonHwidCooldown>(cpszHwid, mapindex, cooldowntime, slot, pid);
			m_mapCooldown.emplace(szTextMD5, std::move(cooldown));
		}

		TPacketDungeonCooldownMgrSet p = {};
		strlcpy(p.key, szTextMD5, sizeof(p.key));
		strlcpy(p.info.hwid, cpszHwid, sizeof(p.info.hwid));
		p.info.map_index = mapindex;
		p.info.cooldown = cooldowntime;
		p.info.slot = slot;
		p.info.pid = pid;

		db_clientdesc->DBPacketHeader(HEADER_GD_DUNGEON_COOLDOWN_MGR_SET, 0, sizeof(TPacketDungeonCooldownMgrSet));
		db_clientdesc->Packet(&p, sizeof(TPacketDungeonCooldownMgrSet));
	}

	return 0;
}

bool CDungeonCooldownMgr::Replace(LPCHARACTER ch, const int32_t mapindex, const uint32_t cooldowntime)
{
	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		Set(ch, mapindex, cooldowntime);

		sys_err("cooldown cannot be replaced (no pid): map index (%d), cooldown time (%u).", mapindex, cooldowntime);
		return false;
	}

	const auto pid = ch->GetPlayerID();

	const char* cpszHwid = ch->GetHardwareID();
	if (strlen(cpszHwid) == 0)
	{
		Set(ch, mapindex, cooldowntime);

		sys_err("cooldown cannot be replaced (invalid hwid): %s (%u), map index (%d), cooldown time (%u).", ch->GetName(), pid, mapindex, cooldowntime);
		return false;
	}

	int slot = -1;

	char szText[HWID_MAX_LEN * 2] = {};
	char szTextMD5[MAX_MD5_LEN + 1] = {};
	for (int i = 0; i < MAX_PID_PER_HWID_DUNGEON_COOLDOWN; i++)
	{
		snprintf(szText, sizeof(szText), "%s%d%d", cpszHwid, mapindex, i);

		MD5 md5;
		snprintf(szTextMD5, sizeof(szTextMD5), "%s", md5.digestString((char*)szText));

		auto it = m_mapCooldown.find(szTextMD5);
		if (it != m_mapCooldown.end())
		{
			auto v = it->second ? it->second.get() : nullptr;
			if (!v)
			{
				continue;
			}

			if (v->GetPID() == pid)
			{
				if (v->SetCooldownTime(cooldowntime))
				{
					slot = v->GetSlot();
					break;
				}
			}
		}
	}

	if (slot != -1)
	{
		TPacketDungeonCooldownMgrSet p = {};
		strlcpy(p.key, szTextMD5, sizeof(p.key));
		strlcpy(p.info.hwid, cpszHwid, sizeof(p.info.hwid));
		p.info.map_index = mapindex;
		p.info.cooldown = cooldowntime;
		p.info.slot = slot;
		p.info.pid = pid;

		db_clientdesc->DBPacketHeader(HEADER_GD_DUNGEON_COOLDOWN_MGR_SET, 0, sizeof(TPacketDungeonCooldownMgrSet));
		db_clientdesc->Packet(&p, sizeof(TPacketDungeonCooldownMgrSet));

		return true;
	}

	Set(ch, mapindex, cooldowntime);
	sys_err("cooldown cannot be replaced (wasn't found): %s (%u), map index (%d), cooldown time (%u).", ch->GetName(), pid, mapindex, cooldowntime);
	return false;
}

uint32_t CDungeonCooldownMgr::Get(LPCHARACTER ch, const int32_t mapindex)
{
	const auto timenow = get_global_time();

	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return (timenow + 3600);
	}

	const char* cpszHwid = ch->GetHardwareID();
	if (strlen(cpszHwid) == 0)
	{
		return (timenow + 3600);
	}

	char szText[HWID_MAX_LEN * 2] = {};
	char szTextMD5[MAX_MD5_LEN + 1] = {};

	std::vector<uint32_t> vecTimes;
	uint32_t cooldownTime = 0;

	for (int i = 0; i < MAX_PID_PER_HWID_DUNGEON_COOLDOWN; i++)
	{
		cooldownTime = 0;

		snprintf(szText, sizeof(szText), "%s%d%d", cpszHwid, mapindex, i);

		MD5 md5;
		snprintf(szTextMD5, sizeof(szTextMD5), "%s", md5.digestString((char*)szText));

		auto it = m_mapCooldown.find(szTextMD5);

		if (it != m_mapCooldown.end())
		{
			cooldownTime = it->second.get() ? it->second.get()->GetCooldownTime() : (timenow + 3600);
			cooldownTime = (cooldownTime <= timenow) ? cooldownTime + 60 : cooldownTime;
		}

		vecTimes.push_back(cooldownTime);
	}

	std::sort(vecTimes.begin(), vecTimes.end());

	return vecTimes.empty() ? 0 : vecTimes[0];
}

bool CDungeonCooldownMgr::GetExcept(const std::string& stHwid, const int32_t mapindex, const int32_t count)
{
	int32_t emptySlots = 0;

	char szText[HWID_MAX_LEN * 2] = {};
	char szTextMD5[MAX_MD5_LEN + 1] = {};
	for (int i = 0; i < MAX_PID_PER_HWID_DUNGEON_COOLDOWN; i++)
	{
		snprintf(szText, sizeof(szText), "%s%d%d", stHwid.c_str(), mapindex, i);

		MD5 md5;
		snprintf(szTextMD5, sizeof(szTextMD5), "%s", md5.digestString((char*)szText));

		auto it = m_mapCooldown.find(szTextMD5);
		if (it == m_mapCooldown.end())
		{
			emptySlots += 1;
		}
	}

	return (emptySlots >= count) ? true : false;
}

int8_t CDungeonCooldownMgr::Reset(LPCHARACTER ch, const int32_t mapindex)
{
	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return -2;
	}

	const char* cpszHwid = ch->GetHardwareID();
	if (strlen(cpszHwid) == 0)
	{
		return -3;
	}

	std::vector<TPacketDungeonCooldownMgrExpire> s_vecRemove;

	char szText[HWID_MAX_LEN * 2] = {};
	char szTextMD5[MAX_MD5_LEN + 1] = {};

	for (int i = 0; i < MAX_PID_PER_HWID_DUNGEON_COOLDOWN; i++)
	{
		snprintf(szText, sizeof(szText), "%s%d%d", cpszHwid, mapindex, i);

		MD5 md5;
		snprintf(szTextMD5, sizeof(szTextMD5), "%s", md5.digestString((char*)szText));

		auto it = m_mapCooldown.find(szTextMD5);
		if (it != m_mapCooldown.end())
		{
			TPacketDungeonCooldownMgrExpire p = {};
			strlcpy(p.key, szTextMD5, sizeof(p.key));

			s_vecRemove.emplace_back(std::move(p));

			auto c = it->second ? it->second.get() : nullptr;
			if (c)
			{
				if (c->SetCooldownTime(0))
				{
					
				}
			}

			m_mapCooldown.erase(it);
		}
	}

	const size_t i = s_vecRemove.size();
	if (i > 0)
	{
		db_clientdesc->DBPacketHeader(HEADER_GD_DUNGEON_COOLDOWN_MGR_EXPIRE, 0, sizeof(TPacketDungeonCooldownMgrExpire) * i);
		db_clientdesc->Packet(&s_vecRemove[0], sizeof(TPacketDungeonCooldownMgrExpire) * i);

		s_vecRemove.clear();
	}

	return 0;
}

int8_t CDungeonCooldownMgr::ResetSingle(LPCHARACTER ch, const int32_t mapindex)
{
	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return -2;
	}

	const char* cpszHwid = ch->GetHardwareID();
	if (strlen(cpszHwid) == 0)
	{
		return -3;
	}

	std::vector<TPacketDungeonCooldownMgrExpire> s_vecRemove;

	bool bFound = false;

	const auto timenow = get_global_time();

	char szText[HWID_MAX_LEN * 2] = {};
	char szTextMD5[MAX_MD5_LEN + 1] = {};

	uint32_t cooldownTimes[MAX_PID_PER_HWID_DUNGEON_COOLDOWN];

	for (int i = 0; i < MAX_PID_PER_HWID_DUNGEON_COOLDOWN; i++)
	{
		cooldownTimes[i] = 0;

		snprintf(szText, sizeof(szText), "%s%d%d", cpszHwid, mapindex, i);

		MD5 md5;
		snprintf(szTextMD5, sizeof(szTextMD5), "%s", md5.digestString((char*)szText));

		auto it = m_mapCooldown.find(szTextMD5);
		if (it != m_mapCooldown.end())
		{
			cooldownTimes[i] = it->second.get() ? it->second.get()->GetCooldownTime() : (timenow + 3600);
			cooldownTimes[i] = (cooldownTimes[i] <= timenow) ? cooldownTimes[i] + 60 : cooldownTimes[i];

			if (!bFound)
			{
				bFound = true;
			}
		}
	}

	if (!bFound)
	{
		return 0;
	}

	bool bDone = false;
	int slot = 0;
	uint32_t cooldownTimeNow = 0;

	for (int i = 0; i < MAX_PID_PER_HWID_DUNGEON_COOLDOWN; i++)
	{
		if (!bDone)
		{
			slot = i;
			cooldownTimeNow = cooldownTimes[i];

			bDone = true;
			continue;
		}

		if (cooldownTimes[i] > cooldownTimeNow)
		{
			slot = i;
			cooldownTimeNow = cooldownTimes[i];
		}
	}

	snprintf(szText, sizeof(szText), "%s%d%d", cpszHwid, mapindex, slot);

	MD5 md5;
	snprintf(szTextMD5, sizeof(szTextMD5), "%s", md5.digestString((char*)szText));

	auto it = m_mapCooldown.find(szTextMD5);
	if (it != m_mapCooldown.end())
	{
		TPacketDungeonCooldownMgrExpire p = {};
		strlcpy(p.key, szTextMD5, sizeof(p.key));

		s_vecRemove.emplace_back(std::move(p));

		auto c = it->second ? it->second.get() : nullptr;
		if (c)
		{
			if (c->SetCooldownTime(0))
			{
				
			}
		}

		m_mapCooldown.erase(it);
	}

	const size_t i = s_vecRemove.size();
	if (i > 0)
	{
		db_clientdesc->DBPacketHeader(HEADER_GD_DUNGEON_COOLDOWN_MGR_EXPIRE, 0, sizeof(TPacketDungeonCooldownMgrExpire) * i);
		db_clientdesc->Packet(&s_vecRemove[0], sizeof(TPacketDungeonCooldownMgrExpire) * i);

		s_vecRemove.clear();
	}

	return 0;
}

void CDungeonCooldownMgr::Initialize(TPacketDungeonCooldownMgrSet* data, size_t size)
{
	if (g_bAuthServer)
	{
		return;
	}

	auto t = data;
	for (size_t i = 0; i < size; i++, ++t)
	{
		auto it = m_mapCooldown.find(t->key);
		if (it != m_mapCooldown.end())
		{
			sys_err("dupe cooldown key (%s), hwid (%s), map index (%d), cooldown (%d), slot (%d), pid (%u).", t->key, t->info.hwid, t->info.map_index, t->info.cooldown, t->info.slot, t->info.pid);
			continue;
		}

		auto cooldown = std::make_unique<CDungeonHwidCooldown>(t->info.hwid, t->info.map_index, t->info.cooldown, t->info.slot, t->info.pid);
		m_mapCooldown.emplace(t->key, std::move(cooldown));
	}
}

void CDungeonCooldownMgr::RecvAdd(const TPacketDungeonCooldownMgrSet* p)
{
	if (g_bAuthServer)
	{
		return;
	}

	auto it = m_mapCooldown.find(p->key);
	if (it != m_mapCooldown.end())
	{
		auto v = it->second.get();
		if (v)
		{
			if (v->SetCooldownTime(p->info.cooldown))
			{
				
			}

			return;
		}
		else
		{
			auto c = it->second ? it->second.get() : nullptr;
			if (c)
			{
				if (c->SetCooldownTime(0))
				{
					
				}
			}

			m_mapCooldown.erase(it);
		}
	}

	auto cooldown = std::make_unique<CDungeonHwidCooldown>(p->info.hwid, p->info.map_index, p->info.cooldown, p->info.slot, p->info.pid);
	m_mapCooldown.emplace(p->key, std::move(cooldown));

	it = m_mapCooldown.find(p->key);
	if (it != m_mapCooldown.end())
	{
		auto cooldown = it->second ? it->second.get() : nullptr;
		if (cooldown)
		{
			cooldown->Update();
		}
	}
}

void CDungeonCooldownMgr::RecvExpire(const char* key)
{
	if (g_bAuthServer)
	{
		return;
	}

	auto it = m_mapCooldown.find(key);
	if (it != m_mapCooldown.end())
	{
		auto c = it->second ? it->second.get() : nullptr;
		if (c)
		{
			if (c->SetCooldownTime(0))
			{
				
			}
		}

		m_mapCooldown.erase(it);
	}
}

void CDungeonCooldownMgr::Clear()
{
	m_mapCooldown.clear();
}

CDungeonCooldownMgr::CDungeonCooldownMgr()
{
	Clear();
}

CDungeonCooldownMgr::~CDungeonCooldownMgr()
{
	Clear();
}
#endif // USE_DUNGEON_COOLDOWN_MANAGER
