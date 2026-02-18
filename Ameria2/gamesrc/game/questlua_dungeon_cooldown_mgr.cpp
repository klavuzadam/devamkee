#include "stdafx.h"

#include "dungeon_cooldown_mgr.hpp"

#include "char.h"
#include "char_manager.h"
#include "dungeon.h"
#include "party.h"
#include "sectree_manager.h"
#include "questmanager.h"

#include "utils.h"

namespace quest
{
#ifdef USE_DUNGEON_COOLDOWN_MANAGER
	struct FPartyPIDCollectorDungeonCooldown
	{
		std::vector<uint32_t> m_vecPIDs;

		FPartyPIDCollectorDungeonCooldown()
		{
			
		}

		void operator () (LPCHARACTER ch)
		{
			if (ch && ch->IsPC())
			{
				m_vecPIDs.push_back(ch->GetPlayerID());
			}
		}
	};

	struct FDungeonManagerReplaceCooldown
	{
		int32_t m_iMapIndex;
		int32_t m_iCooldown;

		FDungeonManagerReplaceCooldown(int iCooldown, int iMapIndex) : m_iMapIndex(iMapIndex), m_iCooldown(iCooldown)
		{
			
		}

		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER) ent;
				if (ch->IsPC())
				{
					auto& dungeonCooldownMgr = CDungeonCooldownMgr::Instance();
					const auto bRes = dungeonCooldownMgr.Replace(ch, m_iMapIndex, m_iCooldown + get_global_time());

					if (!bRes)
					{
						sys_err("Cannot set dungeon hwid cooldown for player: name -> %s, id -> %u.", ch->GetName(), ch->GetPlayerID());
					}
				}
			}
		}
	};
#endif

	int dungeon_cooldown_mgr_check_cooldown(lua_State* L)
	{
#ifdef USE_DUNGEON_COOLDOWN_MANAGER
		if (!lua_isnumber(L, 1) || !lua_isboolean(L, 2))
		{
			sys_err("not enough arguments or invalid arguments given.");
			lua_pushnumber(L, 2);
			lua_pushstring(L, "");
			lua_pushnumber(L, 0);
			lua_pushboolean(L, false);
			return 4;
		}

		int32_t idx = static_cast<int32_t>(lua_tonumber(L, 1));
		bool bPartyCheck = static_cast<bool>(lua_toboolean(L, 2));

		auto& questMgr = CQuestManager::Instance();
		LPCHARACTER ch = questMgr.GetCurrentCharacterPtr();
		if (!ch)
		{
			sys_err("not char pointer.");
			lua_pushnumber(L, 2);
			lua_pushstring(L, "");
			lua_pushnumber(L, 0);
			lua_pushboolean(L, false);
			return 4;
		}

		const uint32_t timenow = get_global_time();
		uint32_t cooldownTime = 0;

		auto& dungeonCooldownMgr = CDungeonCooldownMgr::Instance();

		LPPARTY party = ch->GetParty();
		if (party && bPartyCheck)
		{
			FPartyPIDCollectorDungeonCooldown f;
			party->ForEachOnMapMember(f, ch->GetMapIndex());

			bool bReturn = false;
			std::vector<std::string> vecHwid;

			LPCHARACTER tch = nullptr;

			for (const auto& pid : f.m_vecPIDs)
			{
				tch = CHARACTER_MANAGER::Instance().FindByPID(pid);
				if (!tch)
				{
					sys_err("Player ID: %u wasn't founded on this core.", pid);
					continue;
				}

				std::string stHwid(tch->GetHardwareID());
				if (stHwid.empty())
				{
					lua_pushnumber(L, 3);
					lua_pushstring(L, tch->GetName());
					lua_pushnumber(L, 0);
					lua_pushboolean(L, true);
					bReturn = true;
					break;
				}

				const uint32_t cooldownTime = dungeonCooldownMgr.Get(tch, idx);
				if (cooldownTime > timenow)
				{
					lua_pushnumber(L, 4);
					lua_pushstring(L, tch->GetName());
					lua_pushnumber(L, cooldownTime - timenow);
					lua_pushboolean(L, true);
					bReturn = true;
					break;
				}

				if (!dungeonCooldownMgr.GetExcept(stHwid, idx, 1 + count(vecHwid.begin(), vecHwid.end(), stHwid)))
				{
					lua_pushnumber(L, 5);
					lua_pushstring(L, tch->GetName());
					lua_pushnumber(L, 0);
					lua_pushboolean(L, true);
					bReturn = true;
					break;
				}

				vecHwid.emplace_back(std::move(stHwid));
			}

			vecHwid.clear();

			if (bReturn)
			{
				return 4;
			}
		}
		else
		{
			const char* cpszHwid = ch->GetHardwareID();
			if (strlen(cpszHwid) == 0)
			{
				lua_pushnumber(L, 3);
				lua_pushstring(L, ch->GetName());
				lua_pushnumber(L, 0);
				lua_pushboolean(L, false);
				return 4;
			}

			std::string stHwid = cpszHwid;

			const uint32_t cooldownTime = dungeonCooldownMgr.Get(ch, idx);
			if (cooldownTime > timenow)
			{
				lua_pushnumber(L, 4);
				lua_pushstring(L, ch->GetName());
				lua_pushnumber(L, cooldownTime - timenow);
				lua_pushboolean(L, false);
				return 4;
			}

			if (!dungeonCooldownMgr.GetExcept(stHwid, idx, 1))
			{
				lua_pushnumber(L, 5);
				lua_pushstring(L, ch->GetName());
				lua_pushnumber(L, 0);
				lua_pushboolean(L, false);
				return 4;
			}
		}

		lua_pushnumber(L, 1);
		lua_pushstring(L, "");
		lua_pushnumber(L, 0);
		lua_pushboolean(L, party ? true : false);
#else
		lua_pushnumber(L, 1);
		lua_pushstring(L, "");
		lua_pushnumber(L, 0);
		lua_pushboolean(L, false);
#endif
		return 4;
	}

	int dungeon_cooldown_mgr_enter_cooldown(lua_State* L)
	{
#ifdef USE_DUNGEON_COOLDOWN_MANAGER
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isboolean(L, 3))
		{
			sys_err("not enough arguments or invalid arguments given.");
			lua_pushboolean(L, false);
			return 1;
		}

		int32_t idx = static_cast<int32_t>(lua_tonumber(L, 1));
		uint32_t cooldown = static_cast<uint32_t>(lua_tonumber(L, 2));
		bool bPartyCheck = static_cast<bool>(lua_toboolean(L, 3));

		auto& questMgr = CQuestManager::Instance();
		LPCHARACTER ch = questMgr.GetCurrentCharacterPtr();
		if (!ch)
		{
			sys_err("not char pointer.");
			lua_pushboolean(L, false);
			return 1;
		}

		const uint32_t timenow = get_global_time();
		uint32_t timedif = 0;

		auto& dungeonCooldownMgr = CDungeonCooldownMgr::Instance();

		LPPARTY party = ch->GetParty();
		if (party && bPartyCheck)
		{
			FPartyPIDCollectorDungeonCooldown f;
			party->ForEachOnMapMember(f, ch->GetMapIndex());

			bool bReturn = false;
			std::vector<std::string> vecHwid;

			LPCHARACTER tch = nullptr;

			bool bRes1 = true;
			int8_t bRes2 = true;

			for (const auto& pid : f.m_vecPIDs)
			{
				tch = CHARACTER_MANAGER::Instance().FindByPID(pid);
				if (!tch)
				{
					sys_err("Player ID: %u wasn't founded on this core.", pid);
					continue;
				}

				bRes2 = dungeonCooldownMgr.Set(tch, idx, timenow + cooldown);

				if (bRes1 && bRes2 != 0)
				{
					bRes1 = false;
				}
			}

			lua_pushboolean(L, bRes1);
		}
		else
		{
			lua_pushboolean(L, (dungeonCooldownMgr.Set(ch, idx, timenow + cooldown) == 0) ? true : false);
		}
#else
		lua_pushboolean(L, true);
#endif
		return 1;
	}

	int dungeon_cooldown_mgr_set_cooldown(lua_State* L)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			sys_err("not enough arguments or invalid arguments given.");
			lua_pushnumber(L, -1);
			return 1;
		}

#ifdef USE_DUNGEON_COOLDOWN_MANAGER
		auto& questMgr = CQuestManager::Instance();
		LPCHARACTER ch = questMgr.GetCurrentCharacterPtr();
		if (!ch)
		{
			sys_err("not char pointer.");
			lua_pushnumber(L, 0);
			return 1;
		}

		auto& dungeonCooldownMgr = CDungeonCooldownMgr::Instance();

		lua_pushnumber(L, dungeonCooldownMgr.Set(ch, static_cast<int32_t>(lua_tonumber(L, 1)), static_cast<uint32_t>(lua_tonumber(L, 2)) + get_global_time()));
#else
		lua_pushnumber(L, 0);
#endif
		return 1;
	}

	int dungeon_cooldown_mgr_get_cooldown(lua_State* L)
	{
		if (!lua_isnumber(L, 1))
		{
			sys_err("not enough arguments or invalid arguments given.");
			lua_pushnumber(L, -1);
			return 1;
		}

#ifdef USE_DUNGEON_COOLDOWN_MANAGER
		auto& questMgr = CQuestManager::Instance();
		LPCHARACTER ch = questMgr.GetCurrentCharacterPtr();
		if (!ch)
		{
			sys_err("not char pointer.");
			lua_pushnumber(L, 0);
			return 1;
		}

		auto& dungeonCooldownMgr = CDungeonCooldownMgr::Instance();

		lua_pushnumber(L, dungeonCooldownMgr.Get(ch, static_cast<int32_t>(lua_tonumber(L, 1))));
#else
		lua_pushnumber(L, 0);
#endif
		return 1;
	}

	int dungeon_cooldown_mgr_reset_cooldown(lua_State* L)
	{
#ifdef USE_DUNGEON_COOLDOWN_MANAGER
		if (!lua_isnumber(L, 1) || !lua_isboolean(L, 2) || !lua_isboolean(L, 3))
		{
			sys_err("not enough arguments or invalid arguments given.");
			lua_pushnumber(L, -1);
			return 1;
		}

		auto& questMgr = CQuestManager::Instance();
		LPCHARACTER ch = questMgr.GetCurrentCharacterPtr();
		if (!ch)
		{
			sys_err("not char pointer.");
			lua_pushnumber(L, -1);
			return 1;
		}

		bool bResetAll = static_cast<bool>(lua_toboolean(L, 2));
		bool bResetParty = static_cast<bool>(lua_toboolean(L, 3));

		auto& dungeonCooldownMgr = CDungeonCooldownMgr::Instance();

		LPPARTY party = ch->GetParty();

		if (bResetParty && party)
		{
			FPartyPIDCollectorDungeonCooldown f;
			party->ForEachOnMapMember(f, ch->GetMapIndex());

			bool bReturn = false;
			std::vector<std::string> vecHwid;

			LPCHARACTER tch = nullptr;

			int8_t bRes1 = 0;
			int8_t bRes2 = 0;

			for (const auto& pid : f.m_vecPIDs)
			{
				tch = CHARACTER_MANAGER::Instance().FindByPID(pid);
				if (!tch)
				{
					sys_err("Player ID: %u wasn't founded on this core.", pid);
					continue;
				}

				if (bResetAll)
				{
					bRes2 = dungeonCooldownMgr.Reset(tch, static_cast<int32_t>(lua_tonumber(L, 1)));
				}
				else
				{
					bRes2 = dungeonCooldownMgr.ResetSingle(tch, static_cast<int32_t>(lua_tonumber(L, 1)));
				}

				if (bRes1 == 0 && bRes2 != 0)
				{
					bRes1 = bRes2;
				}
			}

			lua_pushnumber(L, bRes1);
		}
		else
		{
			if (bResetAll)
			{
				lua_pushnumber(L, dungeonCooldownMgr.Reset(ch, static_cast<int32_t>(lua_tonumber(L, 1))));
			}
			else
			{
				lua_pushnumber(L, dungeonCooldownMgr.ResetSingle(ch, static_cast<int32_t>(lua_tonumber(L, 1))));
			}
		}
#else
		lua_pushnumber(L, 0);
#endif

		return 1;
	}

	int dungeon_cooldown_mgr_replace_cooldown(lua_State* L)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			sys_err("not enough arguments or invalid arguments given.");
			lua_pushboolean(L, false);
			return 1;
		}

#ifdef USE_DUNGEON_COOLDOWN_MANAGER
		auto& questMgr = CQuestManager::Instance();
		LPCHARACTER ch = questMgr.GetCurrentCharacterPtr();
		if (!ch)
		{
			sys_err("not char pointer.");
			lua_pushboolean(L, false);
			return 1;
		}

		bool bPartyCheck = false;

		if (lua_isboolean(L, 3))
		{
			bPartyCheck = static_cast<bool>(lua_toboolean(L, 3));
		}

		auto& dungeonCooldownMgr = CDungeonCooldownMgr::Instance();

		LPPARTY party = ch->GetParty();
		if (party && bPartyCheck)
		{
			FPartyPIDCollectorDungeonCooldown f;
			party->ForEachOnMapMember(f, ch->GetMapIndex());

			LPCHARACTER tch = nullptr;

			bool bRes1 = true;
			bool bRes2 = true;

			for (const auto& pid : f.m_vecPIDs)
			{
				tch = CHARACTER_MANAGER::Instance().FindByPID(pid);
				if (!tch)
				{
					sys_err("Player ID: %u wasn't founded on this core.", pid);
					continue;
				}

				bRes2 = dungeonCooldownMgr.Replace(tch, static_cast<int32_t>(lua_tonumber(L, 1)), static_cast<std::time_t>(lua_tonumber(L, 2)) + get_global_time());

				if (bRes1 && !bRes2)
				{
					bRes1 = false;
				}
			}

			lua_pushboolean(L, bRes1);
		}
		else
		{
			lua_pushboolean(L, dungeonCooldownMgr.Replace(ch, static_cast<int32_t>(lua_tonumber(L, 1)), static_cast<std::time_t>(lua_tonumber(L, 2)) + get_global_time()));
		}
#else
		lua_pushboolean(L, true);
#endif
		return 1;
	}

	int dungeon_cooldown_mgr_replace_idx_cooldown(lua_State* L)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
		{
			sys_err("not enough arguments or invalid arguments given.");
			return 0;
		}

#ifdef USE_DUNGEON_COOLDOWN_MANAGER
		int32_t realidx = static_cast<int32_t>(lua_tonumber(L, 1));
		uint32_t cooldown = static_cast<uint32_t>(lua_tonumber(L, 2));
		int32_t idx = static_cast<int32_t>(lua_tonumber(L, 3));

		LPDUNGEON pDungeon = CDungeonManager::instance().FindByMapIndex(realidx);
		if (!pDungeon)
		{
			sys_err("dungeon instance with id: %d wasn't founded.", realidx);
			return 0;
		}

		int32_t checkidx = idx / 10000;
		if (checkidx != realidx)
		{
			sys_err("map realidx doesn't match (realidx: %d, idx: %d, checkidx: %d).", realidx, idx, checkidx);
			return 0;
		}

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(idx);
		if (!pSecMap)
		{
			sys_err("map instance with id: %d wasn't founded.", idx);
			return 0;
		}

		FDungeonManagerReplaceCooldown func(realidx, cooldown);
		pSecMap->for_each(func);
#endif
		return 0;
	}

	void RegisterDungeonCooldownMgrFunctionTable()
	{
		luaL_reg dungeon_cooldown_mgr_functions[] =
		{
			{ "check", dungeon_cooldown_mgr_check_cooldown },
			{ "enter", dungeon_cooldown_mgr_enter_cooldown },
			{ "set", dungeon_cooldown_mgr_set_cooldown },
			{ "get", dungeon_cooldown_mgr_get_cooldown },
			{ "reset", dungeon_cooldown_mgr_reset_cooldown },
			{ "replace", dungeon_cooldown_mgr_replace_cooldown },
			{ "replace_idx", dungeon_cooldown_mgr_replace_idx_cooldown },
			{ nullptr, nullptr }
		};

		CQuestManager::instance().AddLuaFunctionTable("dungeon_cooldown_mgr", dungeon_cooldown_mgr_functions);
	}
};
