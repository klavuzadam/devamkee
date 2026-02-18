#include "stdafx.h"

#include <stack>

#include "utils.h"
#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "item_manager.h"
#include "desc.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "packet.h"
#include "affect.h"
#include "skill.h"
#include "start_position.h"
#include "mob_manager.h"
#include "db.h"
#include "log.h"
#include "vector.h"
#include "buffer_manager.h"
#include "questmanager.h"
#include "fishing.h"
#include "party.h"
#include "dungeon.h"
#include "refine.h"
#include "unique_item.h"
#include "war_map.h"
#include "xmas_event.h"
#include "marriage.h"
#include "monarch.h"
#include "polymorph.h"
#include "blend_item.h"
#include "castle.h"
#include "BattleArena.h"
#include "arena.h"
#include "dev_log.h"
#include "pcbang.h"
#include "threeway_war.h"
#include "itemblock.h"
#include "safebox.h"
#include "shop.h"
#ifdef __BUFFI_SUPPORT__
#include "buffi.h"
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
#include "switchbonus.h"
#endif
#ifdef ENABLE_NEWSTUFF
#include "pvp.h"
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
#include "PetSystem.h"
#endif
#ifdef ENABLE_QUEEN_NETHIS
#	include "SnakeLair.h"
#endif

#ifdef ENABLE_SWITCHBOT
#include "new_switchbot.h"
#endif

#include "../common/VnumHelper.h"
#include "DragonSoul.h"
#include "buff_on_attributes.h"
#include "belt_inventory_helper.h"
#include "../common/CommonDefines.h"
//auction_temp
const int ITEM_BROKEN_METIN_VNUM = 28960;
#define ENABLE_EFFECT_EXTRAPOT
#define ENABLE_BOOKS_STACKFIX
#if defined(WJ_COMBAT_ZONE)	
#include "combat_zone.h"
#endif
#ifdef __NEWPET_SYSTEM__
#include "New_PetSystem.h"
#endif
#include "RankPlayer.h"

#include "block_change_costume.h"
#ifdef __NEW_EVENTS__
#include "new_events.h"
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
#include "tournament.h"
#endif
#include "MountSystem.h"
#ifdef BATTLEPASS_WORLDARD
#include "battlepass.h"
#endif
#if defined(ENABLE_YOHARA_SYSTEM) && defined(ENABLE_RANDOM_ATTR)
# include "RandomHelper.h"
#endif

#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_ULTIMATE
#include "ikarus_shop_manager.h"
#endif
#endif
#ifdef USE_DAY_CHANGE_MGR
#include "day_change_mgr.hpp"
#endif

// CHANGE_ITEM_ATTRIBUTES
// const DWORD CHARACTER::msc_dwDefaultChangeItemAttrCycle = 10;
const char CHARACTER::msc_szLastChangeItemAttrFlag[] = "Item.LastChangeItemAttr";
// const char CHARACTER::msc_szChangeItemAttrCycleFlag[] = "change_itemattr_cycle";
// END_OF_CHANGE_ITEM_ATTRIBUTES
const BYTE g_aBuffOnAttrPoints[] = { POINT_ENERGY, POINT_COSTUME_ATTR_BONUS };

struct FFindStone
{
	std::map<DWORD, LPCHARACTER> m_mapStone;

	void operator()(LPENTITY pEnt)
	{
		if (pEnt->IsType(ENTITY_CHARACTER) == true)
		{
			LPCHARACTER pChar = (LPCHARACTER)pEnt;

			if (pChar->IsStone() == true)
			{
				m_mapStone[(DWORD)pChar->GetVID()] = pChar;
			}
		}
	}
};


//Â±??Â¯Âº?, Â±??Â¯Â±Ã¢Â¾Ã¯Âº?, Â°Ã¡?Â¥Â¹??Ã¶
static bool IS_SUMMON_ITEM(int vnum)
{
	switch (vnum)
	{
	case 22000:
	case 22010:
	case 22011:
	case 22020:
	case ITEM_MARRIAGE_RING:
		return true;
	}

	return false;
}

static bool IS_MONKEY_DUNGEON(int map_index)
{
	switch (map_index)
	{
	case 5:
	case 25:
	case 45:
	case 108:
	case 109:
		return true;;
	}

	return false;
}

bool IS_SUMMONABLE_ZONE(int map_index)
{
	// Â¸Ã¹?Â°Â´Ã¸?Ã¼
	if (IS_MONKEY_DUNGEON(map_index))
		return false;
	// Â¼Âº
	if (IS_CASTLE_MAP(map_index))
		return false;

	switch (map_index)
	{
	case 66: // Â»Ã§Â±??Â¸Â¿Ã¶
	case 71: // Â°?Â¹? Â´Ã¸?Ã¼ 2?Ã¾
	case 72: // ?Âµ?? ÂµÂ¿Â±Â¼
	case 73: // ?Âµ?? ÂµÂ¿Â±Â¼ 2?Ã¾
	case 193: // Â°?Â¹? Â´Ã¸?Ã¼ 2-1?Ã¾
#if 0
	case 184: // ?Âµ?? ÂµÂ¿Â±Â¼(Â½?Â¼Ã¶)
	case 185: // ?Âµ?? ÂµÂ¿Â±Â¼ 2?Ã¾(Â½?Â¼Ã¶)
	case 186: // ?Âµ?? ÂµÂ¿Â±Â¼(?Âµ?Â¶)
	case 187: // ?Âµ?? ÂµÂ¿Â±Â¼ 2?Ã¾(?Âµ?Â¶)
	case 188: // ?Âµ?? ÂµÂ¿Â±Â¼(?Ã¸Â³Ã«)
	case 189: // ?Âµ?? ÂµÂ¿Â±Â¼ 2?Ã¾(?Ã¸Â³Ã«)
#endif
		//		case 206 : // Â¾?Â±?ÂµÂ¿Â±Â¼
	case 216: // Â¾?Â±?ÂµÂ¿Â±Â¼
	case 217: // Â°?Â¹? Â´Ã¸?Ã¼ 3?Ã¾
	case 208: // ?Âµ?? ÂµÂ¿Â±Â¼ (Â¿Ã«Â¹Ã¦)

	case 113: // OX Event Â¸?
		return false;
	}

	if (CBattleArena::IsBattleArenaMap(map_index)) return false;

	// Â¸Ã°ÂµÃ§ private Â¸??Â¸Â·? Â¿Ã¶?? Âº?Â°Â¡Â´?
	if (map_index > 10000) return false;

	return true;
}

bool IS_BOTARYABLE_ZONE(int nMapIndex)
{
	if (!g_bEnableBootaryCheck) return true;

	switch (nMapIndex)
	{
	case 1:
	case 3:
	case 21:
	case 23:
	case 41:
	case 43:
		return true;
	}

	return false;
}

#ifdef ENABLE_SUNG_MAHI_TOWER
bool IS_SUNG_MAHI_ENABLE_ITEM(int iMapIndex, DWORD dwVnum)
{
	bool isDungeonMap = (iMapIndex >= 3540000 && iMapIndex < 3550000);
	if (isDungeonMap && !(dwVnum >= 79707 && dwVnum <= 79712))
		return false;

	if ((dwVnum >= 79707 && dwVnum <= 79712) && !isDungeonMap)
		return false;

	return true;
}
#endif

static bool FN_check_item_socket(LPITEM item)
{
#ifdef ENABLE_SORT_INVEN
	if (item->GetVnum() >= 71124 && item->GetVnum() <= 71177)
		return true;

	if (item->GetType() == ITEM_USE && item->GetSubType() == USE_AFFECT)
		return true;

#endif
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		if (item->GetSocket(i) != item->GetProto()->alSockets[i])
			return false;
	}

	return true;
}

// item socket ÂºÂ¹Â»Ã§ -- by mhh
static void FN_copy_item_socket(LPITEM dest, LPITEM src)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		dest->SetSocket(i, src->GetSocket(i));
	}
}
static bool FN_check_item_sex(LPCHARACTER ch, LPITEM item)
{
#ifdef __BUFFI_SUPPORT__
	if (ch->IsLookingBuffiPage() && item->GetType() == ITEM_COSTUME && (item->GetSubType() == COSTUME_BODY || item->GetSubType() == COSTUME_HAIR))
	{
		if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_MALE))
			return true;
		else
			return false;
	}
#endif
	// Â³Â²?? Â±??Ã¶
	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_MALE))
	{
		if (SEX_MALE == GET_SEX(ch))
			return false;
	}
	// Â¿Â©??Â±??Ã¶
	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_FEMALE))
	{
		if (SEX_FEMALE == GET_SEX(ch))
			return false;
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ITEM HANDLING
/////////////////////////////////////////////////////////////////////////////
bool CHARACTER::CanHandleItem(bool bSkipCheckRefine, bool bSkipObserver, bool bSkipDS
//#ifdef USE_CRYSTAL_SYSTEM
, bool bSkipShardCraft/* = false*/
//#endif
)
{
	if (!bSkipObserver)
	{
		if (m_bIsObserver)
		{
			return false;
		}
	}

	if (!bSkipCheckRefine)
	{
		if (m_bUnderRefine)
		{
			return false;
		}
	}

	if (!bSkipDS)
	{
		if (NULL != DragonSoul_RefineWindow_GetOpener())
		{
			return false;
		}
	}

	if (IsCubeOpen())
	{
		return false;
	}

	if (IsWarping())
	{
		return false;
	}

#ifdef __SASH_SYSTEM__
	if (isSashOpened(true)
		 || isSashOpened(false)
	)
	{
		return false;
	}
#endif

#ifdef __BL_67_ATTR__
	if (Is67AttrOpen())
	{
		return false;
	}
#endif

#ifdef __AURA_SYSTEM__
	if (IsAuraRefineWindowOpen() || NULL != GetAuraRefineWindowOpener())
	{
		return false;
	}
#endif

#ifdef USE_CRYSTAL_SYSTEM
	if (!bSkipShardCraft && IsShardCraftOpened())
	{
		return false;
	}
#endif

	return true;
}

LPITEM CHARACTER::GetInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(INVENTORY, wCell));
}

#ifdef ENABLE_SPECIAL_STORAGE
LPITEM CHARACTER::GetUpgradeInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(UPGRADE_INVENTORY, wCell));
}
LPITEM CHARACTER::GetBookInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(BOOK_INVENTORY, wCell));
}
LPITEM CHARACTER::GetStoneInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(STONE_INVENTORY, wCell));
}
LPITEM CHARACTER::GetChangeInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(CHANGE_INVENTORY, wCell));
}
LPITEM CHARACTER::GetCostumeInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(COSTUME_INVENTORY, wCell));
}
LPITEM CHARACTER::GetChestInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(CHEST_INVENTORY, wCell));
}
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
LPITEM CHARACTER::GetSwitchBotItem(WORD wCell) const
{
	return GetItem(TItemPos(SWITCHBOT, wCell));
}
#endif

#ifdef CHANGE_EQUIP_WORLDARD 
LPITEM CHARACTER::GetChangeEquipItem(WORD wCell) const
{
	return GetItem(TItemPos(CHANGE_EQUIP, wCell));
}
#endif

LPITEM CHARACTER::GetItem(TItemPos Cell) const
{
	if (!m_playerData)
	{
		return NULL;
	}

	if (!IsValidItemPosition(Cell))
	{
		return NULL;
	}

	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;

	switch (window_type)
	{
		case INVENTORY:
		case EQUIPMENT:
		{
			if (wCell >= INVENTORY_AND_EQUIP_SLOT_MAX)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid item cell %u", wCell);
				return NULL;
			}

			return m_playerData->pItems[wCell];
		}
		case DRAGON_SOUL_INVENTORY:
		{
			if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid DS item cell %u", wCell);
				return NULL;
			}

			return m_playerData->pDSItems[wCell];
		}
#ifdef ENABLE_SPECIAL_STORAGE
		case UPGRADE_INVENTORY:
		{
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid SSU item cell %u", wCell);
				return NULL;
			}

			return m_playerData->pSSUItems[wCell];
		}
		case BOOK_INVENTORY:
		{
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid SSB item cell %u", wCell);
				return NULL;
			}

			return m_playerData->pSSBItems[wCell];
		}
		case STONE_INVENTORY:
		{
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid SSS item cell %u", wCell);
				return NULL;
			}

			return m_playerData->pSSSItems[wCell];
		}
		case CHANGE_INVENTORY:
		{
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid SSC item cell %u", wCell);
				return NULL;
			}

			return m_playerData->pSSCItems[wCell];
		}
		case COSTUME_INVENTORY:
		{
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid SSC item cell %u", wCell);
				return NULL;
			}

			return m_playerData->pCostumeItems[wCell];
		}
		case CHEST_INVENTORY:
		{
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid SSC item cell %u", wCell);
				return NULL;
			}

			return m_playerData->pChestItems[wCell];
		}
#endif
#if defined(ENABLE_SWITCHBOT) && !defined(ENABLE_SWITCHBOT_RENEWAL)
		case SWITCHBOT:
		{
			if (wCell >= SWITCHBOT_SLOT_COUNT)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid switchbot item cell %d", wCell);
				return NULL;
			}

			return m_playerData->pSwitchbotItems[wCell];
		}
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
		case SWITCHBOT:
		{
			if (wCell >= SWITCHBOT_SLOT_MAX)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid SWITCHBOT item cell %d", wCell);
				return NULL;
			}

			return m_playerData->pSBItems[wCell];
		}
#endif
#ifdef CHANGE_EQUIP_WORLDARD
		case CHANGE_EQUIP:
		{
			if (wCell >= CHANGE_EQUIP_SLOT_COUNT)
			{
				sys_err("CHARACTER::GetInventoryItem: invalid change_equip item cell %d", wCell);
				return NULL;
			}

			return m_playerData->pChangeEquipItem[wCell];
		}
#endif
		default:
		{
			return NULL;
		}
	}

	return NULL;
}

void CHARACTER::SetItem(TItemPos Cell, LPITEM pItem, bool highlight)
{
	if (!m_playerData)
	{
		return;
	}

	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;

	if ((unsigned long)((CItem*)pItem) == 0xff || (unsigned long)((CItem*)pItem) == 0xffffffff)
	{
		sys_err("!!! FATAL ERROR !!! item == 0xff (char: %s cell: %u)", GetName(), wCell);
		core_dump();
		return;
	}

	if (pItem && pItem->GetOwner())
	{
		assert(!"GetOwner exist");
		return;
	}

#ifdef __BL_BATTLE_ROYALE__
	if (pItem && IsBattleRoyale())
	{
		// This is to make sure that nothing leaves the battle royale,
		// in case you have alternative drops(like quests) that you forgot to block.
		pItem->SetNeverSave();
	}
#endif

	switch (window_type)
	{
		case INVENTORY:
		case EQUIPMENT:
		{
			if (wCell >= INVENTORY_AND_EQUIP_SLOT_MAX)
			{
				sys_err("CHARACTER::SetItem: invalid item cell %u", wCell);
				return;
			}

			LPITEM pOld = m_playerData->pItems[wCell];
			if (pOld)
			{
#ifdef __BL_BATTLE_ROYALE__
				if (wCell < GetInventorySize())
#else
				if (wCell < INVENTORY_MAX_NUM)
#endif
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

#ifdef __BL_BATTLE_ROYALE__
						if (p >= GetInventorySize())
#else
						if (p >= INVENTORY_MAX_NUM)
#endif
						{
							continue;
						}

						if (m_playerData->pItems[p] && m_playerData->pItems[p] != pOld)
						{
							continue;
						}

						m_playerData->wItemGrid[p] = 0;
					}
				}
				else
				{
					m_playerData->wItemGrid[wCell] = 0;
				}
			}

			if (pItem)
			{
#ifdef __BL_BATTLE_ROYALE__
				if (wCell < GetInventorySize())
#else
				if (wCell < INVENTORY_MAX_NUM)
#endif
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

#ifdef __BL_BATTLE_ROYALE__
						if (p >= GetInventorySize())
#else
						if (p >= INVENTORY_MAX_NUM)
#endif
						{
							continue;
						}

						m_playerData->wItemGrid[p] = wCell + 1;
					}
				}
				else
				{
					m_playerData->wItemGrid[wCell] = wCell + 1;
				}
			}

			m_playerData->pItems[wCell] = pItem;
			break;
		}
		case DRAGON_SOUL_INVENTORY:
		{
			LPITEM pOld = m_playerData->pDSItems[wCell];
			if (pOld)
			{
				if (wCell < DRAGON_SOUL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * DRAGON_SOUL_BOX_COLUMN_NUM);

						if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						if (m_playerData->pDSItems[p] && m_playerData->pDSItems[p] != pOld)
						{
							continue;
						}

						m_playerData->wDSItemGrid[p] = 0;
					}
				}
				else
				{
					m_playerData->wDSItemGrid[wCell] = 0;
				}
			}

			if (pItem)
			{
				if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid DS item cell %u", wCell);
					return;
				}

				if (wCell < DRAGON_SOUL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * DRAGON_SOUL_BOX_COLUMN_NUM);

						if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						m_playerData->wDSItemGrid[p] = wCell + 1;
					}
				}
				else
				{
					m_playerData->wDSItemGrid[wCell] = wCell + 1;
				}
			}

			m_playerData->pDSItems[wCell] = pItem;
			break;
		}
#if defined(ENABLE_SWITCHBOT) && !defined(ENABLE_SWITCHBOT_RENEWAL)
		case SWITCHBOT:
		{
			if (wCell >= SWITCHBOT_SLOT_COUNT)
			{
				sys_err("CHARACTER::SetItem: invalid switchbot item cell %u", wCell);
				return;
			}

			LPITEM pOld = m_playerData->pSwitchbotItems[wCell];
			if (pItem && pOld)
			{
				return;
			}

			if (pItem)
			{
				CSwitchbotManager::Instance().RegisterItem(GetPlayerID(), pItem->GetID(), wCell);
			}
			else
			{
				CSwitchbotManager::Instance().UnregisterItem(GetPlayerID(), wCell);
			}

			m_playerData->pSwitchbotItems[wCell] = pItem;
			break;
		}
#endif
#ifdef ENABLE_SPECIAL_STORAGE
		case UPGRADE_INVENTORY:
		{
			LPITEM pOld = m_playerData->pSSUItems[wCell];
			if (pOld)
			{
				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						if (m_playerData->pSSUItems[p] && m_playerData->pSSUItems[p] != pOld)
						{
							continue;
						}

						m_playerData->wSSUItemGrid[p] = 0;
					}
				}
				else
				{
					m_playerData->wSSUItemGrid[wCell] = 0;
				}
			}

			if (pItem)
			{
				if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid SSU item cell %d", wCell);
					return;
				}

				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						m_playerData->wSSUItemGrid[p] = wCell + 1;
					}
				}
				else
				{
					m_playerData->wSSUItemGrid[wCell] = wCell + 1;
				}
			}

			m_playerData->pSSUItems[wCell] = pItem;
			break;
		}
		case BOOK_INVENTORY:
		{
			LPITEM pOld = m_playerData->pSSBItems[wCell];
			if (pOld)
			{
				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						if (m_playerData->pSSBItems[p] && m_playerData->pSSBItems[p] != pOld)
						{
							continue;
						}

						m_playerData->wSSBItemGrid[p] = 0;
					}
				}
				else
				{
					m_playerData->wSSBItemGrid[wCell] = 0;
				}
			}

			if (pItem)
			{
				if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid SSB item cell %d", wCell);
					return;
				}

				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						m_playerData->wSSBItemGrid[p] = wCell + 1;
					}
				}
				else
				{
					m_playerData->wSSBItemGrid[wCell] = wCell + 1;
				}
			}

			m_playerData->pSSBItems[wCell] = pItem;
			break;
		}
		case STONE_INVENTORY:
		{
			LPITEM pOld = m_playerData->pSSSItems[wCell];
			if (pOld)
			{
				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						if (m_playerData->pSSSItems[p] && m_playerData->pSSSItems[p] != pOld)
						{
							continue;
						}

						m_playerData->wSSSItemGrid[p] = 0;
					}
				}
				else
				{
					m_playerData->wSSSItemGrid[wCell] = 0;
				}
			}

			if (pItem)
			{
				if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid SSB item cell %d", wCell);
					return;
				}

				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						m_playerData->wSSSItemGrid[p] = wCell + 1;
					}
				}
				else
				{
					m_playerData->wSSSItemGrid[wCell] = wCell + 1;
				}
			}

			m_playerData->pSSSItems[wCell] = pItem;
			break;
		}
		case CHANGE_INVENTORY:
		{
			LPITEM pOld = m_playerData->pSSCItems[wCell];
			if (pOld)
			{
				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						if (m_playerData->pSSCItems[p] && m_playerData->pSSCItems[p] != pOld)
						{
							continue;
						}

						m_playerData->wSSCItemGrid[p] = 0;
					}
				}
				else
				{
					m_playerData->wSSCItemGrid[wCell] = 0;
				}
			}

			if (pItem)
			{
				if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid SSC item cell %d", wCell);
					return;
				}

				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						m_playerData->wSSCItemGrid[p] = wCell + 1;
					}
				}
				else
				{
					m_playerData->wSSCItemGrid[wCell] = wCell + 1;
				}
			}

			m_playerData->pSSCItems[wCell] = pItem;
			break;
		}
		case COSTUME_INVENTORY:
		{
			LPITEM pOld = m_playerData->pCostumeItems[wCell];
			if (pOld)
			{
				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						if (m_playerData->pCostumeItems[p] && m_playerData->pCostumeItems[p] != pOld)
						{
							continue;
						}

						m_playerData->wCostumeItemGrid[p] = 0;
					}
				}
				else
				{
					m_playerData->wCostumeItemGrid[wCell] = 0;
				}
			}

			if (pItem)
			{
				if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid SSC item cell %d", wCell);
					return;
				}

				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						m_playerData->wCostumeItemGrid[p] = wCell + 1;
					}
				}
				else
				{
					m_playerData->wCostumeItemGrid[wCell] = wCell + 1;
				}
			}

			m_playerData->pCostumeItems[wCell] = pItem;
			break;
		}
		case CHEST_INVENTORY:
		{
			LPITEM pOld = m_playerData->pChestItems[wCell];
			if (pOld)
			{
				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						if (m_playerData->pChestItems[p] && m_playerData->pChestItems[p] != pOld)
						{
							continue;
						}

						m_playerData->wChestItemGrid[p] = 0;
					}
				}
				else
				{
					m_playerData->wChestItemGrid[wCell] = 0;
				}
			}

			if (pItem)
			{
				if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid SSC item cell %d", wCell);
					return;
				}

				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							continue;
						}

						m_playerData->wChestItemGrid[p] = wCell + 1;
					}
				}
				else
				{
					m_playerData->wChestItemGrid[wCell] = wCell + 1;
				}
			}

			m_playerData->pChestItems[wCell] = pItem;
			break;
		}
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
		case SWITCHBOT:
		{
			if (wCell >= SWITCHBOT_SLOT_MAX)
			{
				sys_err("CHARACTER::SetItem: invalid SWITCHBOT item cell %d", wCell);
				return;
			}

			LPITEM pOld = m_playerData->pSBItems[wCell];

			if (pOld && pItem)
			{
				return;
			}

			if (pItem)
			{
				CSwitchBonus::Instance().SendSBItem(this, pItem);
			}
			else
			{
				CSwitchBonus::Instance().ClearSB(this);
			}


			m_playerData->pSBItems[wCell] = pItem;
			break;
		}
#endif
#ifdef CHANGE_EQUIP_WORLDARD
		case CHANGE_EQUIP:
		{
			if (wCell >= CHANGE_EQUIP_SLOT_COUNT)
			{
				sys_err("CHARACTER::SetItem: invalid CHANGE_EQUIP item cell %d", wCell);
				return;
			}

			LPITEM pOld = m_playerData->pChangeEquipItem[wCell];
			if (pOld && pItem)
			{
				return;
			}

			m_playerData->pChangeEquipItem[wCell] = pItem;
			break;
		}
#endif
		default:
		{
			sys_err("Invalid Inventory type %u", window_type);
			return;
		}
	}

	LPDESC d = GetDesc();

	if (d)
	{
		if (pItem)
		{
			TPacketGCItemSet p;
			p.header = HEADER_GC_ITEM_SET;
			p.Cell = Cell;

			p.count = pItem->GetCount();
#ifdef __SOULBINDING_SYSTEM__
			p.bind = pItem->GetBind();
#endif
			p.vnum = pItem->GetVnum();
#ifdef ENABLE_NEW_NAME_ITEM
			strlcpy(p.name, pItem->GetNewName(), sizeof(p.name));
#endif
			p.flags = pItem->GetFlag();
			p.anti_flags = pItem->GetAntiFlag();
#ifdef ENABLE_NEWITEM_AFFECT
			p.highlight = highlight;
#else
			p.highlight = (Cell.window_type == DRAGON_SOUL_INVENTORY);
#endif
			std::memcpy(p.alSockets, pItem->GetSockets(), sizeof(p.alSockets));
			std::memcpy(p.aAttr, pItem->GetAttributes(), sizeof(p.aAttr));
#ifdef ENABLE_YOHARA_SYSTEM
			std::memcpy(p.aApplyRandom, pItem->GetApplysRandom(), sizeof(p.aApplyRandom));
#endif

			d->Packet(&p, sizeof(p));
		}
		else
		{
			TPacketGCItemDelDeprecated p;
			p.header = HEADER_GC_ITEM_DEL;
			p.Cell = Cell;
			p.count = 0;
#ifdef __SOULBINDING_SYSTEM__
			p.bind = 0;
#endif
			p.vnum = 0;

			memset(p.alSockets, 0, sizeof(p.alSockets));
			memset(p.aAttr, 0, sizeof(p.aAttr));
#ifdef ENABLE_YOHARA_SYSTEM
			memset(p.aApplyRandom, 0, sizeof(p.aApplyRandom));
#endif

			d->Packet(&p, sizeof(p));
		}
	}

	if (pItem)
	{
		pItem->SetCell(this, wCell);

		switch (window_type)
		{
			case INVENTORY:
			case EQUIPMENT:
			{
				if ((wCell < INVENTORY_MAX_NUM) || (BELT_INVENTORY_SLOT_START <= wCell && BELT_INVENTORY_SLOT_END > wCell))
				{
					pItem->SetWindow(INVENTORY);
				}
				else
				{
					pItem->SetWindow(EQUIPMENT);
				}

				break;
			}
			case DRAGON_SOUL_INVENTORY:
			{
				pItem->SetWindow(DRAGON_SOUL_INVENTORY);
				break;
			}
#ifdef ENABLE_SWITCHBOT
			case SWITCHBOT:
			{
				pItem->SetWindow(SWITCHBOT);
				break;
			}
#endif
#ifdef ENABLE_SPECIAL_STORAGE
			case UPGRADE_INVENTORY:
			{
				pItem->SetWindow(UPGRADE_INVENTORY);
				break;
			}
			case BOOK_INVENTORY:
			{
				pItem->SetWindow(BOOK_INVENTORY);
				break;
			}
			case STONE_INVENTORY:
			{
				pItem->SetWindow(STONE_INVENTORY);
				break;
			}
			case CHANGE_INVENTORY:
			{
				pItem->SetWindow(CHANGE_INVENTORY);
				break;
			}
			case COSTUME_INVENTORY:
			{
				pItem->SetWindow(COSTUME_INVENTORY);
				break;
			}
			case CHEST_INVENTORY:
			{
				pItem->SetWindow(CHEST_INVENTORY);
				break;
			}
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
			case SWITCHBOT:
			{
				pItem->SetWindow(SWITCHBOT);
				break;
			}
#endif
#ifdef CHANGE_EQUIP_WORLDARD
			case CHANGE_EQUIP:
			{
				pItem->SetWindow(CHANGE_EQUIP);
				break;
			}
#endif
			default:
			{
				break;
			}
		}
	}
}

LPITEM CHARACTER::GetWear(WORD wCell) const
{
	if (!m_playerData)
	{
		return NULL;
	}

	if (wCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
	{
		sys_err("CHARACTER::GetWear: invalid wear cell %u", wCell);
		return NULL;
	}

	return m_playerData->pItems[INVENTORY_MAX_NUM + wCell];
}

void CHARACTER::SetWear(WORD wCell, LPITEM item)
{
	if (wCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
	{
		sys_err("CHARACTER::SetItem: invalid item cell %u", wCell);
		return;
	}

	SetItem(TItemPos(INVENTORY, INVENTORY_MAX_NUM + wCell), item);
}

void CHARACTER::ClearItem()
{
	WORD i;
	LPITEM item;

	for (i = 0; i < INVENTORY_AND_EQUIP_SLOT_MAX; ++i)
	{
		if ((item = GetInventoryItem(i)))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);

			SyncQuickslot(QUICKSLOT_TYPE_ITEM, i, WORD_MAX);
		}
	}
	for (i = 0; i < DRAGON_SOUL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(DRAGON_SOUL_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
#ifdef ENABLE_SPECIAL_STORAGE
	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(UPGRADE_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(BOOK_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(STONE_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(CHANGE_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(COSTUME_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(CHEST_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
	for (i = 0; i < SWITCHBOT_SLOT_MAX; ++i)
	{
		if ((item = GetItem(TItemPos(SWITCHBOT, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
#endif

#ifdef ENABLE_SWITCHBOT
	for (i = 0; i < SWITCHBOT_SLOT_COUNT; ++i)
	{
		if ((item = GetItem(TItemPos(SWITCHBOT, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
#endif

#ifdef CHANGE_EQUIP_WORLDARD
	for (i = 0; i < CHANGE_EQUIP_SLOT_COUNT; ++i)
	{
		if ((item = GetItem(TItemPos(CHANGE_EQUIP, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}

#endif
}

bool CHARACTER::IsEmptyItemGrid(TItemPos Cell, BYTE bSize, int iExceptionCell) const
{
	if (!m_playerData)
	{
		return false;
	}

	switch (Cell.window_type)
	{
		case INVENTORY:
		{
			WORD bCell = Cell.cell;

			++iExceptionCell;

			if (Cell.IsBeltInventoryPosition())
			{
				LPITEM beltItem = GetWear(WEAR_BELT);

				if (!beltItem)
				{
					return false;
				}

				if (!CBeltInventoryHelper::IsAvailableCell(bCell - BELT_INVENTORY_SLOT_START, beltItem->GetValue(0)))
				{
					return false;
				}

				if (m_playerData->wItemGrid[bCell])
				{
					if (m_playerData->wItemGrid[bCell] == iExceptionCell)
					{
						return true;
					}

					return false;
				}

				if (bSize == 1)
				{
					return true;
				}
			}
#ifdef UNLOCK_INVENTORY_WORLDARD
			else if (bCell >= GetInventorySize())
			{
				return false;
			}
			else if (bSize == 2)
			{
				if (bCell + 5 >= GetInventorySize())
				{
					return false;
				}
			}
			else if (bSize == 3)
			{
				if (bCell + 10 >= GetInventorySize())
				{
					return false;
				}
			}
#else
			else if (bCell >= INVENTORY_MAX_NUM)
			{
				return false;
			}
#endif

			if (m_playerData->wItemGrid[bCell])
			{
				if (m_playerData->wItemGrid[bCell] == iExceptionCell)
				{
					if (bSize == 1)
					{
						return true;
					}

					int j = 1;
					BYTE bPage = bCell / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT);

					do
					{
						BYTE p = bCell + (5 * j);

#ifdef __BL_BATTLE_ROYALE__
						if (IsBattleRoyale() && p >= GetInventorySize())
						{
							return false;
						}
#endif

#ifdef UNLOCK_INVENTORY_WORLDARD
						if (p >= GetInventorySize())
						{
							return false;
						}
#else
						if (p >= INVENTORY_MAX_NUM)
						{
							return false;
						}
#endif

						if (p / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT) != bPage)
						{
							return false;
						}

						if (m_playerData->wItemGrid[p])
						{
							if (m_playerData->wItemGrid[p] != iExceptionCell)
							{
								return false;
							}
						}
					} while (++j < bSize);

					return true;
				}
				else
				{
					return false;
				}
			}

			if (bSize == 1)
			{
				return true;
			}
			else
			{
				int j = 1;
				BYTE bPage = bCell / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT);

				do
				{
					BYTE p = bCell + (5 * j);

					if (p >= INVENTORY_MAX_NUM)
					{
						return false;
					}

					if (p / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT) != bPage)
					{
						return false;
					}

					if (m_playerData->wItemGrid[p])
					{
						if (m_playerData->wItemGrid[p] != iExceptionCell)
						{
							return false;
						}
					}
				} while (++j < bSize);

				return true;
			}

			break;
		}
		case DRAGON_SOUL_INVENTORY:
		{
			WORD wCell = Cell.cell;
			if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
			{
				return false;
			}

			iExceptionCell++;

			if (m_playerData->wDSItemGrid[wCell])
			{
				if (m_playerData->wDSItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
					{
						return true;
					}

					int j = 1;

					do
					{
						int p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

						if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						{
							return false;
						}

						if (m_playerData->wDSItemGrid[p])
						{
							if (m_playerData->wDSItemGrid[p] != iExceptionCell)
							{
								return false;
							}
						}
					} while (++j < bSize);

					return true;
				}
				else
				{
					return false;
				}
			}

			if (bSize == 1)
			{
				return true;
			}
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

					if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
					{
						return false;
					}

					if (m_playerData->wItemGrid[p])
					{
						if (m_playerData->wDSItemGrid[p] != iExceptionCell)
						{
							return false;
						}
					}
				} while (++j < bSize);

				return true;
			}

			break;
		}
#if defined(ENABLE_SWITCHBOT) && !defined(ENABLE_SWITCHBOT_RENEWAL)
		case SWITCHBOT:
		{
			WORD wCell = Cell.cell;
			if (wCell >= SWITCHBOT_SLOT_COUNT)
			{
				return false;
			}

			if (m_playerData->pSwitchbotItems[wCell])
			{
				return false;
			}

			return true;
		}
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
		case SWITCHBOT:
		{
			WORD wCell = Cell.cell;
			if (wCell >= SWITCHBOT_SLOT_MAX)
			{
				return false;
			}

			if (m_playerData->pSBItems[wCell])
			{
				return false;
			}

			return true;
		}
#endif
#ifdef ENABLE_SPECIAL_STORAGE
		case UPGRADE_INVENTORY:
		{
			WORD wCell = Cell.cell;

#ifdef UNLOCK_INVENTORY_WORLDARD
			if (wCell >= GetInventorySize(1))
			{
				return false;
			}
			else if (bSize == 2)
			{
				if (wCell + 5 >= GetInventorySize(1))
				{
					return false;
				}
			}
			else if (bSize == 3)
			{
				if (wCell + 10 >= GetInventorySize(1))
				{
					return false;
				}
			}
#else
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				return false;
			}
#endif

			iExceptionCell++;

			if (m_playerData->wSSUItemGrid[wCell])
			{
				if (m_playerData->wSSUItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
					{
						return true;
					}

					int j = 1;

					do
					{
						int p = wCell + (5 * j);

#ifdef UNLOCK_INVENTORY_WORLDARD
						if (p >= GetInventorySize(1))
						{
							return false;
						}
#else
						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							return false;
						}
#endif

						if (m_playerData->wSSUItemGrid[p])
						{
							if (m_playerData->wSSUItemGrid[p] != iExceptionCell)
							{
								return false;
							}
						}
					} while (++j < bSize);

					return true;
				}
				else
				{
					return false;
				}
			}

			if (bSize == 1)
			{
				return true;
			}
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
					{
						return false;
					}

					if (m_playerData->wItemGrid[p]) // old wItemGrid
					{
						if (m_playerData->wSSUItemGrid[p] != iExceptionCell)
						{
							return false;
						}
					}
				} while (++j < bSize);

				return true;
			}

			break;
		}
		case BOOK_INVENTORY:
		{
			WORD wCell = Cell.cell;
#ifdef UNLOCK_INVENTORY_WORLDARD
			if (wCell >= GetInventorySize(2))
			{
				return false;
			}
			else if (bSize == 2)
			{
				if (wCell + 5 >= GetInventorySize(2))
				{
					return false;
				}
			}
			else if (bSize == 3)
			{
				if (wCell + 10 >= GetInventorySize(2))
				{
					return false;
				}
			}
#else
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				return false;
			}
#endif

			iExceptionCell++;

			if (m_playerData->wSSBItemGrid[wCell])
			{
				if (m_playerData->wSSBItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
					{
						return true;
					}

					int j = 1;

					do
					{
						int p = wCell + (5 * j);

#ifdef UNLOCK_INVENTORY_WORLDARD
						if (p >= GetInventorySize(2))
						{
							return false;
						}
#else
						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							return false;
						}
#endif

						if (m_playerData->wSSBItemGrid[p])
						{
							if (m_playerData->wSSBItemGrid[p] != iExceptionCell)
							{
								return false;
							}
						}
					} while (++j < bSize);

					return true;
				}
				else
				{
					return false;
				}
			}

			if (bSize == 1)
			{
				return true;
			}
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
					{
						return false;
					}

					if (m_playerData->wItemGrid[p]) // old wItemGrid
					{
						if (m_playerData->wSSBItemGrid[p] != iExceptionCell)
						{
							return false;
						}
					}
				} while (++j < bSize);

				return true;
			}

			break;
		}
		case STONE_INVENTORY:
		{
			WORD wCell = Cell.cell;
#ifdef UNLOCK_INVENTORY_WORLDARD
			if (wCell >= GetInventorySize(3))
			{
				return false;
			}
			else if (bSize == 2)
			{
				if (wCell + 5 >= GetInventorySize(3))
				{
					return false;
				}
			}
			else if (bSize == 3)
			{
				if (wCell + 10 >= GetInventorySize(3))
				{
					return false;
				}
			}
#else
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				return false;
			}
#endif

			iExceptionCell++;

			if (m_playerData->wSSSItemGrid[wCell])
			{
				if (m_playerData->wSSSItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
					{
						return true;
					}

					int j = 1;

					do
					{
						int p = wCell + (5 * j);

#ifdef UNLOCK_INVENTORY_WORLDARD
						if (p >= GetInventorySize(3))
						{
							return false;
						}
#else
						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							return false;
						}
#endif

						if (m_playerData->wSSSItemGrid[p])
						{
							if (m_playerData->wSSSItemGrid[p] != iExceptionCell)
							{
								return false;
							}
						}
					} while (++j < bSize);

					return true;
				}
				else
				{
					return false;
				}
			}

			if (bSize == 1)
			{
				return true;
			}
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
					{
						return false;
					}

					if (m_playerData->wItemGrid[p]) // old wItemGrid
					{
						if (m_playerData->wSSSItemGrid[p] != iExceptionCell)
						{
							return false;
						}
					}
				} while (++j < bSize);

				return true;
			}

			break;
		}
		case CHANGE_INVENTORY:
		{
			WORD wCell = Cell.cell;
#ifdef UNLOCK_INVENTORY_WORLDARD
			if (wCell >= GetInventorySize(4))
			{
				return false;
			}
			else if (bSize == 2)
			{
				if (wCell + 5 >= GetInventorySize(4))
				{
					return false;
				}
			}
			else if (bSize == 3)
			{
				if (wCell + 10 >= GetInventorySize(4))
				{
					return false;
				}
			}
#else
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				return false;
			}
#endif

			iExceptionCell++;

			if (m_playerData->wSSCItemGrid[wCell])
			{
				if (m_playerData->wSSCItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
					{
						return true;
					}

					int j = 1;

					do
					{
						int p = wCell + (5 * j);

#ifdef UNLOCK_INVENTORY_WORLDARD
						if (p >= GetInventorySize(4))
						{
							return false;
						}
#else
						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							return false;
						}
#endif

						if (m_playerData->wSSCItemGrid[p])
						{
							if (m_playerData->wSSCItemGrid[p] != iExceptionCell)
							{
								return false;
							}
						}
					} while (++j < bSize);

					return true;
				}
				else
				{
					return false;
				}
			}

			if (bSize == 1)
			{
				return true;
			}
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
					{
						return false;
					}

					if (m_playerData->wItemGrid[p]) // old wItemGrid
					{
						if (m_playerData->wSSCItemGrid[p] != iExceptionCell)
						{
							return false;
						}
					}
				} while (++j < bSize);

				return true;
			}

			break;
		}
		case COSTUME_INVENTORY:
		{
			WORD wCell = Cell.cell;
#ifdef UNLOCK_INVENTORY_WORLDARD
			if (wCell >= GetInventorySize(5))
			{
				return false;
			}
			else if (bSize == 2)
			{
				if (wCell + 5 >= GetInventorySize(5))
				{
					return false;
				}
			}
			else if (bSize == 3)
			{
				if (wCell + 10 >= GetInventorySize(5))
				{
					return false;
				}
			}
#else
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				return false;
			}
#endif

			iExceptionCell++;

			if (m_playerData->wCostumeItemGrid[wCell])
			{
				if (m_playerData->wCostumeItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
					{
						return true;
					}

					int j = 1;

					do
					{
						int p = wCell + (5 * j);

#ifdef UNLOCK_INVENTORY_WORLDARD
						if (p >= GetInventorySize(5))
						{
							return false;
						}
#else
						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							return false;
						}
#endif

						if (m_playerData->wCostumeItemGrid[p])
						{
							if (m_playerData->wCostumeItemGrid[p] != iExceptionCell)
							{
								return false;
							}
						}
					} while (++j < bSize);

					return true;
				}
				else
				{
					return false;
				}
			}

			if (bSize == 1)
			{
				return true;
			}
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
					{
						return false;
					}

					if (m_playerData->wItemGrid[p]) // old wItemGrid
					{
						if (m_playerData->wCostumeItemGrid[p] != iExceptionCell)
						{
							return false;
						}
					}
				} while (++j < bSize);

				return true;
			}

			break;
		}
		case CHEST_INVENTORY:
		{
			WORD wCell = Cell.cell;
#ifdef UNLOCK_INVENTORY_WORLDARD
			if (wCell >= GetInventorySize(6))
			{
				return false;
			}
			else if (bSize == 2)
			{
				if (wCell + 5 >= GetInventorySize(6))
				{
					return false;
				}
			}
			else if (bSize == 3)
			{
				if (wCell + 10 >= GetInventorySize(6))
				{
					return false;
				}
			}
#else
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				return false;
			}
#endif

			iExceptionCell++;

			if (m_playerData->wChestItemGrid[wCell])
			{
				if (m_playerData->wChestItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
					{
						return true;
					}

					int j = 1;

					do
					{
						int p = wCell + (5 * j);

#ifdef UNLOCK_INVENTORY_WORLDARD
						if (p >= GetInventorySize(6))
						{
							return false;
						}
#else
						if (p >= SPECIAL_INVENTORY_MAX_NUM)
						{
							return false;
						}
#endif

						if (m_playerData->wChestItemGrid[p])
						{
							if (m_playerData->wChestItemGrid[p] != iExceptionCell)
							{
								return false;
							}
						}
					} while (++j < bSize);

					return true;
				}
				else
				{
					return false;
				}
			}

			if (bSize == 1)
			{
				return true;
			}
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
					{
						return false;
					}

					if (m_playerData->wItemGrid[p]) // old wItemGrid
					{
						if (m_playerData->wChestItemGrid[p] != iExceptionCell)
						{
							return false;
						}
					}
				} while (++j < bSize);

				return true;
			}

			break;
		}
#endif
#ifdef CHANGE_EQUIP_WORLDARD
		case CHANGE_EQUIP:
		{
			WORD wCell = Cell.cell;
			if (wCell >= CHANGE_EQUIP_SLOT_COUNT)
			{
				return false;
			}

			if (m_playerData->pChangeEquipItem[wCell])
			{
				return false;
			}

			return true;
		}
#endif
		default:
		{
			break;
		}
	}

	return false;
}

int CHARACTER::GetEmptyInventory(BYTE size) const
{
	// NOTE: ?Ã¶?Ã§ ?? ??Â¼Ã¶Â´? Â¾????? ?Ã¶Â±?, ?Â¹ÂµÃ¦ ÂµÃ®?? ???Â§Â¸Â¦ ?? Â¶Â§ ??ÂºÂ¥?Ã¤Â¸Â®?? ÂºÃ³ ?Â­?Â» ?Â£Â±Ã¢ ?Â§?? Â»Ã§Â¿Ã«Âµ?Â°Ã­ ??Â´?ÂµÂ¥,
	//		ÂºÂ§?Â® ??ÂºÂ¥?Ã¤Â¸Â®Â´? ?Â¯Â¼Ã¶ ??ÂºÂ¥?Ã¤Â¸Â®??Â¹?Â·? Â°?Â»Ã§???Ã¶ Â¾?ÂµÂµÂ·? ??Â´?. (Â±Ã¢ÂºÂ» ??ÂºÂ¥?Ã¤Â¸Â®: INVENTORY_MAX_NUM Â±Ã®?Ã¶Â¸Â¸ Â°?Â»Ã§)
#if defined(__BL_BATTLE_ROYALE__) || defined(UNLOCK_INVENTORY_WORLDARD)
	for (int i = 0; i < GetInventorySize(); ++i)
#else
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (IsEmptyItemGrid(TItemPos(INVENTORY, i), size))
			return i;
	}

	return -1;
}

int CHARACTER::GetEmptyDragonSoulInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsDragonSoul())
		return -1;
	if (!DragonSoul_IsQualified())
	{
		return -1;
	}
	BYTE bSize = pItem->GetSize();
	WORD wBaseCell = DSManager::instance().GetBasePosition(pItem);

	if (WORD_MAX == wBaseCell)
		return -1;

	for (int i = 0; i < DRAGON_SOUL_BOX_SIZE; ++i)
		if (IsEmptyItemGrid(TItemPos(DRAGON_SOUL_INVENTORY, i + wBaseCell), bSize))
			return i + wBaseCell;

	return -1;
}

void CHARACTER::CopyDragonSoulItemGrid(std::vector<WORD>& vDragonSoulItemGrid) const
{
	vDragonSoulItemGrid.resize(DRAGON_SOUL_INVENTORY_MAX_NUM);

	if (m_playerData)
	{
		std::fill(vDragonSoulItemGrid.begin(), vDragonSoulItemGrid.end(), 0);
		return;
	}

	std::copy(m_playerData->wDSItemGrid, m_playerData->wDSItemGrid + DRAGON_SOUL_INVENTORY_MAX_NUM, vDragonSoulItemGrid.begin());
}

#ifdef ENABLE_SPECIAL_STORAGE
int CHARACTER::GetEmptyUpgradeInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsUpgradeItem())
		return -1;

	BYTE bSize = pItem->GetSize();


#ifdef UNLOCK_INVENTORY_WORLDARD
	for (WORD i = 0; i < GetInventorySize(1); ++i)
		if (IsEmptyItemGrid(TItemPos(UPGRADE_INVENTORY, i), bSize))
			return i;
	return -1;
#else
	for (WORD i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(UPGRADE_INVENTORY, i), bSize))
			return i;
	return -1;
#endif

}
int CHARACTER::GetEmptyBookInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsBook())
		return -1;

	BYTE bSize = pItem->GetSize();

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (WORD i = 0; i < GetInventorySize(2); ++i)
		if (IsEmptyItemGrid(TItemPos(BOOK_INVENTORY, i), bSize))
			return i;
	return -1;
#else
	for (WORD i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(BOOK_INVENTORY, i), bSize))
			return i;
	return -1;
#endif

}
int CHARACTER::GetEmptyStoneInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsStone())
		return -1;

	BYTE bSize = pItem->GetSize();

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (WORD i = 0; i < GetInventorySize(3); ++i)
		if (IsEmptyItemGrid(TItemPos(STONE_INVENTORY, i), bSize))
			return i;
	return -1;
#else
	for (WORD i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(STONE_INVENTORY, i), bSize))
			return i;
	return -1;
#endif

}
int CHARACTER::GetEmptyChangeInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsChange())
		return -1;

	BYTE bSize = pItem->GetSize();

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (WORD i = 0; i < GetInventorySize(4); ++i)
		if (IsEmptyItemGrid(TItemPos(CHANGE_INVENTORY, i), bSize))
			return i;
	return -1;
#else
	for (WORD i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(CHANGE_INVENTORY, i), bSize))
			return i;
	return -1;
#endif

}

int CHARACTER::GetEmptyCostumeInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsCostume() || !pItem->IsCostumeSkin())
		return -1;

	BYTE bSize = pItem->GetSize();

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (WORD i = 0; i < GetInventorySize(5); ++i)
		if (IsEmptyItemGrid(TItemPos(COSTUME_INVENTORY, i), bSize))
			return i;
	return -1;
#else
	for (WORD i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(COSTUME_INVENTORY, i), bSize))
			return i;
	return -1;
#endif

}

int CHARACTER::GetEmptyChestInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsChest())
		return -1;

	BYTE bSize = pItem->GetSize();

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (WORD i = 0; i < GetInventorySize(6); ++i)
		if (IsEmptyItemGrid(TItemPos(CHEST_INVENTORY, i), bSize))
			return i;
	return -1;
#else
	for (WORD i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(CHEST_INVENTORY, i), bSize))
			return i;
	return -1;
#endif

}
#endif

int CHARACTER::CountEmptyInventory() const
{
	int count = 0;

#if defined(__BL_BATTLE_ROYALE__) || defined(UNLOCK_INVENTORY_WORLDARD)
	for (WORD i = 0; i < GetInventorySize(); ++i)
#else
	for (WORD i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (GetInventoryItem(i))
			count += GetInventoryItem(i)->GetSize();
	}

#if defined(__BL_BATTLE_ROYALE__) || defined(UNLOCK_INVENTORY_WORLDARD)
	return (GetInventorySize() - count);
#else
	return (INVENTORY_MAX_NUM - count);
#endif
}

void TransformRefineItem(LPITEM pkOldItem, LPITEM pkNewItem)
{
	// ACCESSORY_REFINE
	if (pkOldItem->IsAccessoryForSocket())
	{
		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			pkNewItem->SetSocket(i, pkOldItem->GetSocket(i));
		}
		//pkNewItem->StartAccessorySocketExpireEvent();
	}
	// END_OF_ACCESSORY_REFINE
	else
	{
		// Â¿Â©Â±Ã¢Â¼Â­ Â±Ãº?Ã¸Â¼Â®?? ??ÂµÂ¿?Ã»?Â¸Â·? ?Â»Â¼? Âµ?
		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			if (!pkOldItem->GetSocket(i))
				break;
			else
				pkNewItem->SetSocket(i, 1);
		}

		// Â¼??? Â¼Â³?Â¤
		int slot = 0;

		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			long socket = pkOldItem->GetSocket(i);

			if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
				pkNewItem->SetSocket(slot++, socket);
		}

	}

	// Â¸??Ã· Â¾????? Â¼Â³?Â¤
	pkOldItem->CopyAttributeTo(pkNewItem);
}

void NotifyRefineSuccess(LPCHARACTER ch, LPITEM item, const char* way)
{
	if (NULL != ch && item != NULL)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "RefineSuceeded");

		LogManager::instance().RefineLog(ch->GetPlayerID(), item->GetName(), item->GetID(), item->GetRefineLevel(), 1, way);
	}
}

void NotifyRefineFail(LPCHARACTER ch, LPITEM item, const char* way, int success = 0)
{
	if (NULL != ch && NULL != item)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "RefineFailed");

		LogManager::instance().RefineLog(ch->GetPlayerID(), item->GetName(), item->GetID(), item->GetRefineLevel(), success, way);
	}
}

void CHARACTER::SetRefineNPC(LPCHARACTER ch)
{
	if (ch != NULL)
	{
		m_dwRefineNPCVID = ch->GetVID();
	}
	else
	{
		m_dwRefineNPCVID = 0;
	}
}

bool CHARACTER::DoRefine(LPITEM item, bool bMoneyOnly/* = false*/
#ifdef ENABLE_PITTY_REFINE
, bool bUseSealOfGod/* = false*/
#endif
)
{
	if (!item || !CanHandleItem(true))
	{
		ClearRefineMode();
		return false;
	}

#ifdef USE_CRYSTAL_SYSTEM
	if (item->GetType() == ITEM_CRYSTAL)
	{
		const auto* pProto = ITEM_MANAGER::instance().GetTable(MAGIC_SPIRAL_VNUM);
		if (pProto)
		{
			char szItemName[ITEM_NAME_MAX_LEN + 1] = {};
			snprintf(szItemName, sizeof(szItemName), "I%u", MAGIC_SPIRAL_VNUM);
			ChatPacket(CHAT_TYPE_INFO, "1518 %s %s", item->GetName(), szItemName);
			// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s can be improved just through %s."), item->GetName(), pProto->szLocaleName);
		}

		ClearRefineMode();
		return false;
	}
#endif

	if (quest::CQuestManager::instance().GetEventFlag("update_refine_time") != 0)
	{
		if (get_global_time() < quest::CQuestManager::instance().GetEventFlag("update_refine_time") + (60 * 5))
		{
			sys_log(0, "can't refine %d %s", GetPlayerID(), GetName());
			return false;
		}
	}

	const auto* prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());
	if (!prt)
	{
		return false;
	}

	if (item->GetCount() > 1)
	{
		int iEmptyPos;
		if (item->IsDragonSoul())
		{
			iEmptyPos = GetEmptyDragonSoulInventory(item);
		}
#ifdef ENABLE_SPECIAL_STORAGE
		else if (item->IsUpgradeItem())
		{
			iEmptyPos = GetEmptyUpgradeInventory(item);
		}
		else if (item->IsBook())
		{
			iEmptyPos = GetEmptyBookInventory(item);
		}
		else if (item->IsStone())
		{
			iEmptyPos = GetEmptyStoneInventory(item);
		}
#endif
		else
		{
			iEmptyPos = GetEmptyInventory(item->GetSize());
		}

		if (iEmptyPos < 0)
		{
			ChatPacket(CHAT_TYPE_INFO, "You do not have space in your inventory");
			return false;
		}
	}

	DWORD result_vnum = item->GetRefinedVnum();
	if (result_vnum == 0)
	{
		sys_err("itemVnum: %u have resultVnum (%u)", item->GetVnum(), result_vnum);
		ChatPacket(CHAT_TYPE_INFO, "116");
		return false;
	}

	// REFINE_COST
	int cost = ComputeRefineFee(prt->cost);

	int RefineChance = GetQuestFlag("main_quest_lv7.refine_chance");

	if (RefineChance > 0)
	{
		if (!item->CheckItemUseLevel(20
#ifdef ENABLE_YOHARA_SYSTEM
, 0
#endif
		) || item->GetType() != ITEM_WEAPON)
		{
			ChatPacket(CHAT_TYPE_INFO, "115");
			return false;
		}

		cost = 0;
		SetQuestFlag("main_quest_lv7.refine_chance", RefineChance - 1);
	}
	// END_OF_REFINE_COST

	if (item->GetType() == ITEM_USE && item->GetSubType() == USE_TUNING)
	{
		return false;
	}

	const auto* pProto = ITEM_MANAGER::instance().GetTable(item->GetRefinedVnum());
	if (!pProto)
	{
		sys_err("DoRefine NOT GET ITEM PROTO %u", item->GetRefinedVnum());
		ChatPacket(CHAT_TYPE_INFO, "117");
		return false;
	}

	// REFINE_COST
	if (GetGold() < cost)
	{
		ChatPacket(CHAT_TYPE_INFO, "118");
		return false;
	}

	if (!bMoneyOnly && !RefineChance)
	{
		for (int i = 0; i < prt->material_count; ++i)
		{
			if (CountSpecifyItem(prt->materials[i].vnum) < prt->materials[i].count)
			{
				if (test_server)
				{
					ChatPacket(CHAT_TYPE_INFO, "Find %d, count %d, require %d", prt->materials[i].vnum, CountSpecifyItem(prt->materials[i].vnum), prt->materials[i].count);
				}

				ChatPacket(CHAT_TYPE_INFO, "119");
				return false;
			}
		}

		for (int i = 0; i < prt->material_count; ++i)
		{
			RemoveSpecifyItem(prt->materials[i].vnum, prt->materials[i].count);
		}
	}

	int prob = number(1, 100);

	if (IsRefineThroughGuild() || bMoneyOnly)
	{
		prob -= 10;
	}

#ifdef ENABLE_PITTY_REFINE
	if (bUseSealOfGod && CountSpecifyItem(ITEM_VNUM_SEAL_OF_GOD) > 0)
	{
		RemoveSpecifyItem(ITEM_VNUM_SEAL_OF_GOD, 1);
		prob -= EXTRA_CHANCE_SEAL_OF_GOD;
		if (prob <= 0)
		{
			prob = 1;
		}
	}
#endif

#ifdef ENABLE_BATTLE_PASS
	CHARACTER_MANAGER::Instance().DoMission(this, MISSION_CRAFT_ITEM, 1, item->GetVnum());
#endif

	// END_OF_REFINE_COST

	if (prob <= prt->prob)
	{
		// Â¼ÂºÂ°Ã¸! Â¸Ã°ÂµÃ§ Â¾??????? Â»Ã§Â¶Ã³?Ã¶Â°Ã­, Â°Â°?Âº Â¼?Â¼Âº?? Â´?Â¸Â¥ Â¾????? ?Â¹ÂµÃ¦
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());



			// DETAIL_REFINE_LOG
			NotifyRefineSuccess(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -cost);

			if (item->GetCount() == 1)
			{
				WORD bCell = item->GetCell();

#if defined(ENABLE_YOHARA_SYSTEM) && defined(ENABLE_RANDOM_ATTR)
				if (IsConquerorItem(item))
					CRandomHelper::RefineRandomAttr(item, pkNewItem, true);
#endif

				ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE SUCCESS)");


				if (pkNewItem->IsUpgradeItem())
				{
					pkNewItem->AddToCharacter(this, TItemPos(UPGRADE_INVENTORY, bCell));
				}
				else if (pkNewItem->IsBook())
				{
					pkNewItem->AddToCharacter(this, TItemPos(BOOK_INVENTORY, bCell));
				}
				else if (pkNewItem->IsStone())
				{
					pkNewItem->AddToCharacter(this, TItemPos(STONE_INVENTORY, bCell));
				}
				else
				{
					pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));
				}
			}
			else
			{

				item->SetCount(item->GetCount() - 1);

				if (pkNewItem->IsUpgradeItem())
				{
					int iEmptyPos = pkNewItem->IsDragonSoul() ? GetEmptyDragonSoulInventory(pkNewItem) : GetEmptyUpgradeInventory(pkNewItem);
					pkNewItem->AddToCharacter(this, TItemPos(pkNewItem->IsDragonSoul() ? DRAGON_SOUL_INVENTORY : UPGRADE_INVENTORY, iEmptyPos));
				}
				else if (pkNewItem->IsBook())
				{
					int iEmptyPos = pkNewItem->IsDragonSoul() ? GetEmptyDragonSoulInventory(pkNewItem) : GetEmptyBookInventory(pkNewItem);
					pkNewItem->AddToCharacter(this, TItemPos(pkNewItem->IsDragonSoul() ? DRAGON_SOUL_INVENTORY : BOOK_INVENTORY, iEmptyPos));
				}
				else if (pkNewItem->IsStone())
				{
					int iEmptyPos = pkNewItem->IsDragonSoul() ? GetEmptyDragonSoulInventory(pkNewItem) : GetEmptyStoneInventory(pkNewItem);
					pkNewItem->AddToCharacter(this, TItemPos(pkNewItem->IsDragonSoul() ? DRAGON_SOUL_INVENTORY : STONE_INVENTORY, iEmptyPos));
				}
				else
				{
					int iEmptyPos = pkNewItem->IsDragonSoul() ? GetEmptyDragonSoulInventory(pkNewItem) : GetEmptyInventory(pkNewItem->GetSize());
					pkNewItem->AddToCharacter(this, TItemPos(pkNewItem->IsDragonSoul() ? DRAGON_SOUL_INVENTORY : INVENTORY, iEmptyPos));
				}
			}

			// END_OF_DETAIL_REFINE_LOG

			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

			if (result_vnum == 3229 || result_vnum == 5169 || result_vnum == 7309 || result_vnum == 1189 || result_vnum == 319 || result_vnum == 309 || result_vnum == 2209)
			{
				CHARACTER_MANAGER::Instance().SetRewardData(REWARD_FIRSTZODIACWEAPON, GetName(), true);
			}

			if (result_vnum == 19499 || result_vnum == 19899 || result_vnum == 19699 || result_vnum == 19299)
			{
				CHARACTER_MANAGER::Instance().SetRewardData(REWARD_FIRSTZODIACARMOR, GetName(), true);
			}


			sys_log(0, "Refine Success %d", cost);
			pkNewItem->AttrLog();
			//PointChange(POINT_GOLD, -cost);
			sys_log(0, "PayPee %d", cost);
			PayRefineFee(cost);

#ifdef ENABLE_BATTLEPASS
			UpdateMissionProgress(MISSION_UPGRADE_OBJECT, 1, item->GetVnum());
#endif
			RankPlayer::instance().SendInfoPlayer(this, RANK_BY_ITEM_IMPROVED, RankPlayer::instance().GetProgressByPID(GetPlayerID(), RANK_BY_ITEM_IMPROVED) + 1, false);

			sys_log(0, "PayPee End %d", cost);
		}
		else
		{
			// DETAIL_REFINE_LOG
			// Â¾????? Â»Ã½Â¼ÂºÂ¿Â¡ Â½??? -> Â°Â³Â·Â® Â½???Â·? Â°Â£??
			sys_err("cannot create item %u", result_vnum);
			NotifyRefineFail(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
			// END_OF_DETAIL_REFINE_LOG
		}
	}
	else
	{
		// Â½???! Â¸Ã°ÂµÃ§ Â¾??????? Â»Ã§Â¶Ã³?Ã¼.
		DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -cost);
		NotifyRefineFail(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
		item->AttrLog();
		if (item->GetCount() == 1)
		{
			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE FAIL)");
		}
		else
		{
			item->SetCount(item->GetCount() - 1);
		}

		//PointChange(POINT_GOLD, -cost);
		PayRefineFee(cost);
	}

	return true;
}

bool CHARACTER::DoRefineWithScroll(LPITEM item
#ifdef ENABLE_PITTY_REFINE
, bool bUseSealOfGod/* = false*/
#endif
)
{
	if (!item || !CanHandleItem(true))
	{
		ClearRefineMode();
		return false;
	}

	ClearRefineMode();
	//Â°Â³Â·Â® Â½?Â°Â£?Â¦?? : upgrade_refine_scroll.quest Â¿Â¡Â¼Â­ Â°Â³Â·Â®?? 5Âº???Â³Â»Â¿Â¡ ??Â¹? Â°Â³Â·Â®?Â»
	//?Ã¸????Â¼Ã¶ Â¾Ã¸?Â½
	if (quest::CQuestManager::instance().GetEventFlag("update_refine_time") != 0)
	{
		if (get_global_time() < quest::CQuestManager::instance().GetEventFlag("update_refine_time") + (60 * 5))
		{
			sys_log(0, "can't refine %d %s", GetPlayerID(), GetName());
			return false;
		}
	}

	const auto* prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());
	if (!prt)
	{
		return false;
	}

	if (m_iRefineAdditionalCell < 0)
		return false;

	LPITEM pkItemScroll = GetInventoryItem(m_iRefineAdditionalCell);

	if (!pkItemScroll)
		return false;

	if (!(pkItemScroll->GetType() == ITEM_USE && pkItemScroll->GetSubType() == USE_TUNING))
		return false;

	if (pkItemScroll->GetVnum() == item->GetVnum())
		return false;

	DWORD result_vnum = item->GetRefinedVnum();
	DWORD result_fail_vnum = item->GetRefineFromVnum();

	if (result_vnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, "116");
		return false;
	}

#ifdef USE_CRYSTAL_SYSTEM
	if (item->GetType() == ITEM_CRYSTAL)
	{
		if (pkItemScroll->GetValue(0) == SPIRAL_SCROLL)
		{
			if (item->isLocked() || item->GetSocket(CRYSTAL_STATUS_SOCKET_SLOT) != 0)
			{
				ChatPacket(CHAT_TYPE_INFO, "1546 %s", item->GetName());
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot improve %s because it is activated."), item->GetName());
				return false;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, "1519 %s %s", pkItemScroll->GetName(), item->GetName());
			// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s cannot be used to improve on %s."), pkItemScroll->GetName(), item->GetName());
			return false;
		}
	}
	else
	{
		if (pkItemScroll->GetValue(0) == SPIRAL_SCROLL)
		{
			ChatPacket(CHAT_TYPE_INFO, "1520 %s", pkItemScroll->GetName());
			// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s can be used just to improve Crystals."), pkItemScroll->GetName());
			return false;
		}
	}
#endif

	// MUSIN_SCROLL
	if (pkItemScroll->GetValue(0) == MUSIN_SCROLL)
	{
		if (item->GetRefineLevel() >= 4)
		{
			ChatPacket(CHAT_TYPE_INFO, "120");
			return false;
		}
	}
	// END_OF_MUSIC_SCROLL

	else if (pkItemScroll->GetValue(0) == MEMO_SCROLL)
	{
		if (item->GetRefineLevel() != pkItemScroll->GetValue(1))
		{
			ChatPacket(CHAT_TYPE_INFO, "121");
			return false;
		}
	}
	else if (pkItemScroll->GetValue(0) == BDRAGON_SCROLL)
	{
		if (item->GetType() != ITEM_METIN || item->GetRefineLevel() != 4)
		{
			ChatPacket(CHAT_TYPE_INFO, "122");
			return false;
		}
	}

	TItemTable* pProto = ITEM_MANAGER::instance().GetTable(item->GetRefinedVnum());

	if (!pProto)
	{
		sys_err("DoRefineWithScroll NOT GET ITEM PROTO %d", item->GetRefinedVnum());
		ChatPacket(CHAT_TYPE_INFO, "117");
		return false;
	}

	if (GetGold() < prt->cost)
	{
		ChatPacket(CHAT_TYPE_INFO, "118");
		return false;
	}

	for (int i = 0; i < prt->material_count; ++i)
	{
		if (CountSpecifyItem(prt->materials[i].vnum) < prt->materials[i].count)
		{
			if (test_server)
			{
				ChatPacket(CHAT_TYPE_INFO, "Find %d, count %d, require %d", prt->materials[i].vnum, CountSpecifyItem(prt->materials[i].vnum), prt->materials[i].count);
			}
			ChatPacket(CHAT_TYPE_INFO, "119");
			return false;
		}
	}

	for (int i = 0; i < prt->material_count; ++i)
		RemoveSpecifyItem(prt->materials[i].vnum, prt->materials[i].count);

	int prob = number(1, 100);
	int success_prob = prt->prob;
	bool bDestroyWhenFail = false;

	const char* szRefineType = "SCROLL";

	if (pkItemScroll->GetValue(0) == HYUNIRON_CHN ||
		pkItemScroll->GetValue(0) == YONGSIN_SCROLL ||
		pkItemScroll->GetValue(0) == YAGONG_SCROLL
		|| pkItemScroll->GetValue(0) == STONE_OF_BLESS)
	{
		const char hyuniron_prob[9] = { 100, 75, 65, 55, 45, 40, 35, 25, 20 };
		const char yagong_prob[9] = { 100, 100, 90, 80, 70, 60, 50, 30, 20 };

		if (pkItemScroll->GetValue(0) == YONGSIN_SCROLL)
		{
			success_prob = hyuniron_prob[MINMAX(0, item->GetRefineLevel(), 8)];
		}
		else if (pkItemScroll->GetValue(0) == YAGONG_SCROLL)
		{
			success_prob = yagong_prob[MINMAX(0, item->GetRefineLevel(), 8)];
		}
		else if (pkItemScroll->GetValue(0) == STONE_OF_BLESS)
		{
			success_prob = 100;
		}

		else if (pkItemScroll->GetValue(0) == HYUNIRON_CHN) {} // @fixme121
		else
		{
			sys_err("REFINE : Unknown refine scroll item. Value0: %d", pkItemScroll->GetValue(0));
		}

		if (pkItemScroll->GetValue(0) == HYUNIRON_CHN) // ?Ã¶?Â¶?Âº Â¾??????? Âº?Â¼Â­?Â®Â¾? ??Â´?.
		{
			bDestroyWhenFail = true;
		}
		else if (pkItemScroll->GetValue(0) == STONE_OF_BLESS)
		{
			bDestroyWhenFail = true;
		}


		// DETAIL_REFINE_LOG
		if (pkItemScroll->GetValue(0) == HYUNIRON_CHN)
		{
			szRefineType = "HYUNIRON";
		}
		else if (pkItemScroll->GetValue(0) == YONGSIN_SCROLL)
		{
			szRefineType = "GOD_SCROLL";
		}
		else if (pkItemScroll->GetValue(0) == YAGONG_SCROLL)
		{
			szRefineType = "YAGONG_SCROLL";
		}
		else if (pkItemScroll->GetValue(0) == STONE_OF_BLESS)
		{
			szRefineType = "STONE_OF_BLESS";
		}
		// END_OF_DETAIL_REFINE_LOG
	}
#ifdef USE_CRYSTAL_SYSTEM
	else if (pkItemScroll->GetValue(0) == SPIRAL_SCROLL)
	{
		bDestroyWhenFail = true;
	}
#endif

	// DETAIL_REFINE_LOG
	if (pkItemScroll->GetValue(0) == MUSIN_SCROLL) // Â¹Â«Â½??? ??ÂºÂ¹Â¼Â­Â´? 100% Â¼ÂºÂ°Ã¸ (+4Â±Ã®?Ã¶Â¸Â¸)
	{
		success_prob = 100;

		szRefineType = "MUSIN_SCROLL";
	}
	// END_OF_DETAIL_REFINE_LOG
	else if (pkItemScroll->GetValue(0) == MEMO_SCROLL)
	{
		success_prob = 100;
		szRefineType = "MEMO_SCROLL";
	}
	else if (pkItemScroll->GetValue(0) == BDRAGON_SCROLL)
	{
		success_prob = 80;
		szRefineType = "BDRAGON_SCROLL";
	}

	// fix pergaminos
	if (pkItemScroll->GetVnum() == 39022 || pkItemScroll->GetVnum() == 71032 || pkItemScroll->GetVnum() == 76009)
		success_prob = prt->prob + 10;

	if (test_server)
	{
		ChatPacket(CHAT_TYPE_INFO, "[Only Test] Success_Prob %d, RefineLevel %d ", success_prob, item->GetRefineLevel());
	}

#ifdef ENABLE_BATTLE_PASS
	CHARACTER_MANAGER::Instance().DoMission(this, MISSION_CRAFT_ITEM, 1, item->GetVnum());
	CHARACTER_MANAGER::Instance().DoMission(this, MISSION_REFINE_ITEM, 1, pkItemScroll->GetVnum());
#endif


	pkItemScroll->SetCount(pkItemScroll->GetCount() - 1);

#ifdef ENABLE_PITTY_REFINE
	if (bUseSealOfGod && CountSpecifyItem(ITEM_VNUM_SEAL_OF_GOD) > 0)
	{
		RemoveSpecifyItem(ITEM_VNUM_SEAL_OF_GOD, 1);

		success_prob = success_prob + EXTRA_CHANCE_SEAL_OF_GOD;
		if (success_prob > 100)
		{
			success_prob = 100;
		}
	}

	int iMaxPittyLevel = item->GetLimitPittyRefine();
	int iGetPittyRefineLevel = GetPittyRefineLevel(item->GetVnum(), item->GetRefineLevel());

	if (iMaxPittyLevel != 0)
	{
		// If you are pitty level, then make 100% chance of success...
		if (iMaxPittyLevel == iGetPittyRefineLevel)
			prob = success_prob;

		if (prob <= success_prob) // succesed
			SetPittyRefineLevel(item->GetVnum(), item->GetRefineLevel(), 0);
		else if (!bDestroyWhenFail && result_fail_vnum) // should downgrade item-1
			SetPittyRefineLevel(item->GetVnum(), item->GetRefineLevel(), 0);
		else
		{
			if (iGetPittyRefineLevel < iMaxPittyLevel)
				SetPittyRefineLevel(item->GetVnum(), item->GetRefineLevel(), iGetPittyRefineLevel + 1);
		}

		SendPittyInfoClient(item->GetVnum(), item->GetRefineLevel());
	}
#endif

	if (prob <= success_prob)
	{
		// Â¼ÂºÂ°Ã¸! Â¸Ã°ÂµÃ§ Â¾??????? Â»Ã§Â¶Ã³?Ã¶Â°Ã­, Â°Â°?Âº Â¼?Â¼Âº?? Â´?Â¸Â¥ Â¾????? ?Â¹ÂµÃ¦
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());

			BYTE bWindow = item->GetWindow();
			WORD bCell = item->GetCell();


			NotifyRefineSuccess(this, item, szRefineType);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -prt->cost);

#if defined(ENABLE_YOHARA_SYSTEM) && defined(ENABLE_RANDOM_ATTR)
			if (IsConquerorItem(item))
				CRandomHelper::RefineRandomAttr(item, pkNewItem, true);
#endif

			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE SUCCESS)");

			pkNewItem->AddToCharacter(this, TItemPos(bWindow, bCell));
			//if (pkNewItem->IsUpgradeItem()){
			//	pkNewItem->AddToCharacter(this, TItemPos(UPGRADE_INVENTORY, bCell));
			//}
			//else if (pkNewItem->IsBook()){
			//	pkNewItem->AddToCharacter(this, TItemPos(BOOK_INVENTORY, bCell));
			//}
			//else if (pkNewItem->IsStone()){
			//	pkNewItem->AddToCharacter(this, TItemPos(STONE_INVENTORY, bCell));
			//}
			//else{
			//	pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));
			//}

			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

			if (result_vnum == 3229 || result_vnum == 5169 || result_vnum == 7309 || result_vnum == 1189 || result_vnum == 319 || result_vnum == 309 || result_vnum == 2209)
			{
				CHARACTER_MANAGER::Instance().SetRewardData(REWARD_FIRSTZODIACWEAPON, GetName(), true);
			}

			if (result_vnum == 19499 || result_vnum == 19899 || result_vnum == 19699 || result_vnum == 19299)
			{
				CHARACTER_MANAGER::Instance().SetRewardData(REWARD_FIRSTZODIACARMOR, GetName(), true);
			}

			pkNewItem->AttrLog();
			//PointChange(POINT_GOLD, -prt->cost);
			PayRefineFee(prt->cost);

#ifdef ENABLE_BATTLEPASS
			UpdateMissionProgress(MISSION_UPGRADE_OBJECT, 1, item->GetVnum());
#endif
			RankPlayer::instance().SendInfoPlayer(this, RANK_BY_ITEM_IMPROVED, RankPlayer::instance().GetProgressByPID(GetPlayerID(), RANK_BY_ITEM_IMPROVED) + 1, false);

			// if (pkNewItem->GetVnum() == targetVnum)
			// {
				// CHARACTER_MANAGER::Instance().SetRewardData(REWARD_PET_115,m_pkOwner->GetName(), true);
			// }
		}
		else
		{
			// Â¾????? Â»Ã½Â¼ÂºÂ¿Â¡ Â½??? -> Â°Â³Â·Â® Â½???Â·? Â°Â£??
			sys_err("cannot create item %u", result_vnum);
			NotifyRefineFail(this, item, szRefineType);
		}
	}
	else if (!bDestroyWhenFail && result_fail_vnum)
	{
		// Â½???! Â¸Ã°ÂµÃ§ Â¾??????? Â»Ã§Â¶Ã³?Ã¶Â°Ã­, Â°Â°?Âº Â¼?Â¼Âº?? Â³Â·?Âº ÂµÃ®Â±??? Â¾????? ?Â¹ÂµÃ¦
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_fail_vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE FAIL", pkNewItem->GetName());

			BYTE bWindow = item->GetWindow();
			WORD bCell = item->GetCell();

			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -prt->cost);
			NotifyRefineFail(this, item, szRefineType, -1);

#if defined(ENABLE_YOHARA_SYSTEM) && defined(ENABLE_RANDOM_ATTR)
			if (IsConquerorItem(item))
				CRandomHelper::RefineRandomAttr(item, pkNewItem, false);
#endif

			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE FAIL)");

			pkNewItem->AddToCharacter(this, TItemPos(bWindow, bCell));

			//if (pkNewItem->IsUpgradeItem()){
			//	pkNewItem->AddToCharacter(this, TItemPos(UPGRADE_INVENTORY, bCell));
			//}
			//else if (pkNewItem->IsBook()){
			//	pkNewItem->AddToCharacter(this, TItemPos(BOOK_INVENTORY, bCell));
			//}
			//else if (pkNewItem->IsStone()){
			//	pkNewItem->AddToCharacter(this, TItemPos(STONE_INVENTORY, bCell));
			//}
			//else{
			//	pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));
			//}

			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

			pkNewItem->AttrLog();

			//PointChange(POINT_GOLD, -prt->cost);
			PayRefineFee(prt->cost);
		}
		else
		{
			// Â¾????? Â»Ã½Â¼ÂºÂ¿Â¡ Â½??? -> Â°Â³Â·Â® Â½???Â·? Â°Â£??
			sys_err("cannot create item %u", result_fail_vnum);
			NotifyRefineFail(this, item, szRefineType);
		}
	}
	else
	{
		NotifyRefineFail(this, item, szRefineType); // Â°Â³Â·Â®Â½? Â¾????? Â»Ã§Â¶Ã³?Ã¶?Ã¶ Â¾??Â½

		PayRefineFee(prt->cost);
	}

	return true;
}

#ifdef ENABLE_QUEEN_NETHIS
bool CHARACTER::DoRefineSerpent(LPITEM item
#ifdef ENABLE_PITTY_REFINE
, bool bUseSealOfGod/* = false*/
#endif

)
{
	if (!item || !CanHandleItem(true))
	{
		ClearRefineMode();
		return false;
	}

#ifdef USE_CRYSTAL_SYSTEM
	if (item->GetType() == ITEM_CRYSTAL)
	{
		const auto* pProto = ITEM_MANAGER::instance().GetTable(MAGIC_SPIRAL_VNUM);
		if (pProto)
		{
			char szItemName[ITEM_NAME_MAX_LEN + 1] = {};
			snprintf(szItemName, sizeof(szItemName), "I%u", MAGIC_SPIRAL_VNUM);
			ChatPacket(CHAT_TYPE_INFO, "1518 %s %s", item->GetName(), szItemName);
			// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s can be improved just through %s."), item->GetName(), pProto->szLocaleName);
		}

		ClearRefineMode();
		return false;
	}
#endif

	const TRefineTable* prt = CRefineManager::Instance().GetRefineRecipe(item->GetRefineSet());
	if (!prt)
		return false;

	const uint32_t result_vnum = item->GetRefinedVnum();

	// REFINE_COST
	const int cost = ComputeRefineFee(prt->cost);
	// END_OF_REFINE_COST

	if (result_vnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, "[LS;991]");
		return false;
	}

	if (item->GetType() == ITEM_USE && item->GetSubType() == USE_TUNING)
		return false;

	const TItemTable* pProto = ITEM_MANAGER::Instance().GetTable(item->GetRefinedVnum());
	if (!pProto)
	{
		sys_err("DoRefineSerpent NOT GET ITEM PROTO %d", item->GetRefinedVnum());
		ChatPacket(CHAT_TYPE_INFO, "[LS;1002]");
		return false;
	}

	// REFINE_COST
	if (GetGold() < cost)
	{
		ChatPacket(CHAT_TYPE_INFO, "[LS;67]");
		return false;
	}

	int prob = number(1, 100);
	prob -= 10;
	// END_OF_REFINE_COST

#ifdef ENABLE_PITTY_REFINE
	if (bUseSealOfGod && CountSpecifyItem(ITEM_VNUM_SEAL_OF_GOD) > 0)
	{
		RemoveSpecifyItem(ITEM_VNUM_SEAL_OF_GOD, 1);
		prob -= EXTRA_CHANCE_SEAL_OF_GOD;
		if (prob <= 0)
			prob = 1;
	}
#endif

	if (prob <= prt->prob)
	{
		// Success! All items disappear, and other items with the same attribute are acquired
		LPITEM pkNewItem = ITEM_MANAGER::Instance().CreateItem(result_vnum, 1, 0, false);

		if (pkNewItem)
		{
#ifdef ENABLE_SEALBIND_SYSTEM
			pkNewItem->SetSealDate(item->GetSealDate());
#endif
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			LogManager::Instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());

			const uint16_t wCell = item->GetCell();	//@fixme519

			// DETAIL_REFINE_LOG
			NotifyRefineSuccess(this, item, "SERPENT");
			DBManager::Instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -cost);

#if defined(ENABLE_YOHARA_SYSTEM) && defined(ENABLE_RANDOM_ATTR)
			if (IsConquerorItem(item))
				CRandomHelper::RefineRandomAttr(item, pkNewItem, true);
#endif

			ITEM_MANAGER::Instance().RemoveItem(item, "REMOVE (REFINE SUCCESS)");
			// END_OF_DETAIL_REFINE_LOG

			pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, wCell));
			ITEM_MANAGER::Instance().FlushDelayedSave(pkNewItem);

			sys_log(0, "Refine Success %d", cost);
			pkNewItem->AttrLog();
			//PointChange(POINT_GOLD, -cost);
			sys_log(0, "PayPee %d", cost);
			PayRefineFee(cost);
			sys_log(0, "PayPee End %d", cost);
		}
		else
		{
			// DETAIL_REFINE_LOG
			// Failed to create item -> Considered as improvement failure
			// sys_err("cannot create item %u", result_vnum);
			NotifyRefineFail(this, item, "SERPENT");
			// END_OF_DETAIL_REFINE_LOG
		}
	}
	else
	{
		// Failure! All items disappear.
		DBManager::Instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -cost);
		NotifyRefineFail(this, item, "SERPENT");
		PayRefineFee(cost);
	}

	return true;
}
#endif

bool CHARACTER::RefineInformation(WORD wCell, BYTE bType, int iAdditionalCell
#ifdef ENABLE_SPECIAL_STORAGE
, BYTE bSpecialStorage
#endif
)
{
	if (wCell > INVENTORY_MAX_NUM)
	{
		return false;
	}

	LPITEM item = nullptr;

	switch (bSpecialStorage)
	{
		case INVENTORY:
		{
			item = GetInventoryItem(wCell);
			break;
		}
#ifdef ENABLE_SPECIAL_STORAGE
		case UPGRADE_INVENTORY:
		{
			item = GetUpgradeInventoryItem(wCell);
			break;
		}
		case BOOK_INVENTORY:
		{
			item = GetBookInventoryItem(wCell);
			break;
		}
		case STONE_INVENTORY:
		{
			item = GetStoneInventoryItem(wCell);
			break;
		}
#endif
		default:
		{
			break;
		}
	}

	if (!item)
	{
		return false;
	}

	// REFINE_COST
	if (bType == REFINE_TYPE_MONEY_ONLY)
	{
#ifdef ENABLE_QUEEN_NETHIS
		if (SnakeLair::CSnk::Instance().IsSnakeMap(GetMapIndex()))
		{
			if (get_global_time() < GetQuestFlag("snake_lair.refine_time"))
			{
				ChatPacket(CHAT_TYPE_INFO, "You can only be rewarded each 24h for the Serpent Temple Smith."); // Custom
				return false;
			}
		}
		else
#endif
		{
			if (!GetQuestFlag("deviltower_zone.can_refine"))
			{
				ChatPacket(CHAT_TYPE_INFO, "123");
				return false;
			}
		}
	}
	// END_OF_REFINE_COST

	if (item->GetRefinedVnum() == 0)
	{
		sys_err("RefineInformation p.result_vnum == 0 (%u)", item->GetVnum());
		ChatPacket(CHAT_TYPE_INFO, "117");
		return false;
	}

#ifdef USE_CRYSTAL_SYSTEM
	LPITEM itemScroll = nullptr;

	if (item->GetType() == ITEM_CRYSTAL)
	{
		LPITEM itemScroll = GetInventoryItem(iAdditionalCell);
		if (!itemScroll || !(itemScroll->GetType() == ITEM_USE && itemScroll->GetSubType() == USE_TUNING && itemScroll->GetValue(0) == SPIRAL_SCROLL))
		{
			const auto* pProto = ITEM_MANAGER::instance().GetTable(MAGIC_SPIRAL_VNUM);
			if (pProto)
			{
				char szItemName[ITEM_NAME_MAX_LEN + 1] = {};
				snprintf(szItemName, sizeof(szItemName), "I%u", MAGIC_SPIRAL_VNUM);
				ChatPacket(CHAT_TYPE_INFO, "1518 %s %s", item->GetName(), szItemName);
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s can be improved just through %s."), item->GetName(), pProto->szLocaleName);
			}

			return false;
		}
		else
		{
			if (item->isLocked() || item->GetSocket(CRYSTAL_STATUS_SOCKET_SLOT) != 0)
			{
				ChatPacket(CHAT_TYPE_INFO, "1546 %s", item->GetName());
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot improve %s because it is activated."), item->GetName());
				return false;
			}
		}
	}
	else
	{
		itemScroll = GetInventoryItem(iAdditionalCell);
		if (itemScroll && itemScroll->GetType() == ITEM_USE && itemScroll->GetSubType() == USE_TUNING && itemScroll->GetValue(0) == SPIRAL_SCROLL)
		{
			ChatPacket(CHAT_TYPE_INFO, "1520 %s", itemScroll->GetName());
			// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s can be used just to improve Crystals."), itemScroll->GetName());
			return false;
		}
	}
#endif

	TPacketGCRefineInformation p;
	p.header = HEADER_GC_REFINE_INFORMATION;
	p.pos = wCell;
	p.src_vnum = item->GetVnum();
	p.result_vnum = item->GetRefinedVnum();
	p.type = bType;
	p.special_storage = bSpecialStorage;

	if (item->GetType() == ITEM_USE && item->GetSubType() == USE_TUNING)
	{
		if (bType == 0)
		{
			ChatPacket(CHAT_TYPE_INFO, "124");
			return false;
		}
		else
		{

			itemScroll = GetInventoryItem(iAdditionalCell);
			if (!itemScroll || item->GetVnum() == itemScroll->GetVnum())
			{
				ChatPacket(CHAT_TYPE_INFO, "125");
				ChatPacket(CHAT_TYPE_INFO, "126");
				return false;
			}
		}
	}

	CRefineManager& rm = CRefineManager::instance();

	//const TRefineTable* prt = rm.GetRefineRecipe(item->GetRefineSet());

	const TRefineTable* prt = NULL;

	if (item->GetRefinedVnum() != 0)
	{
		prt = rm.GetRefineRecipe(item->GetRefineSet());
	}

	if (!prt)
	{
		sys_err("RefineInformation NOT GET REFINE SET refineSet: %d itemVnum: %d", item->GetRefineSet(), item->GetVnum());
		ChatPacket(CHAT_TYPE_INFO, "117");
		return false;
	}

	//MAIN_QUEST_LV7
	if (GetQuestFlag("main_quest_lv7.refine_chance") > 0)
	{
		// ??ÂºÂ»?Âº ?Â¦Â¿?
		if (!item->CheckItemUseLevel(20
#ifdef ENABLE_YOHARA_SYSTEM
, 0
#endif
		) || item->GetType() != ITEM_WEAPON)
		{
			ChatPacket(CHAT_TYPE_INFO, "115");
			return false;
		}
		p.cost = 0;
	}
	else
		p.cost = ComputeRefineFee(prt->cost);

	//END_MAIN_QUEST_LV7
	p.prob = prt->prob;
	if (bType == REFINE_TYPE_MONEY_ONLY)
	{
		p.material_count = 0;
		memset(p.materials, 0, sizeof(p.materials));
	}
	else
	{
		p.material_count = prt->material_count;
		std::memcpy(&p.materials, prt->materials, sizeof(prt->materials));
	}
	// END_OF_REFINE_COST

	GetDesc()->Packet(&p, sizeof(TPacketGCRefineInformation));

	SetRefineMode(iAdditionalCell);

#ifdef ENABLE_PITTY_REFINE
	int iMaxPittyLevel = item->GetLimitPittyRefine();

	if (iMaxPittyLevel != 0)
	{
		SendPittyInfoClient(item->GetVnum(), item->GetRefineLevel());
	}
#endif

	return true;
}

bool CHARACTER::RefineItem(LPITEM pkItem, LPITEM pkTarget)
{
	if (!CanHandleItem())
		return false;

	if (pkTarget->GetCount() != 1)
	{
		ChatPacket(CHAT_TYPE_INFO, "1077");
		return false;
	}

	if (pkItem->GetSubType() == USE_TUNING)
	{
		// MUSIN_SCROLL
		if (pkItem->GetValue(0) == MUSIN_SCROLL)
		{
			if (pkTarget->IsUpgradeItem())
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_MUSIN, pkItem->GetCell(), UPGRADE_INVENTORY);
			}
			else if (pkTarget->IsBook())
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_MUSIN, pkItem->GetCell(), BOOK_INVENTORY);
			}
			//else if (pkTarget->IsStone()){
			//	RefineInformation(pkTarget->GetCell(), REFINE_TYPE_MUSIN, pkItem->GetCell(),STONE_INVENTORY);
			//}
			else
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_MUSIN, pkItem->GetCell(), INVENTORY);
			}
		}
		// END_OF_MUSIN_SCROLL
		else if (pkItem->GetValue(0) == HYUNIRON_CHN)
		{
			if (pkTarget->IsBook())
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_HYUNIRON, pkItem->GetCell(), BOOK_INVENTORY);
			}
			else
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_HYUNIRON, pkItem->GetCell(), INVENTORY);
			}

		}
		else if (pkItem->GetValue(0) == STONE_OF_BLESS)
		{
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_STONE_OF_BLESS, pkItem->GetCell(), INVENTORY);
		}

		else if (pkItem->GetValue(0) == BDRAGON_SCROLL)
		{
			if (pkTarget->GetRefineSet() != 702) return false;
			if (pkTarget->IsUpgradeItem())
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_BDRAGON, pkItem->GetCell(), UPGRADE_INVENTORY);
			}
			else if (pkTarget->IsBook())
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_BDRAGON, pkItem->GetCell(), BOOK_INVENTORY);
			}
			else
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_BDRAGON, pkItem->GetCell(), INVENTORY);
			}
		}
		else
		{
			if (pkTarget->GetRefineSet() == 501) return false;
			if (pkTarget->IsUpgradeItem())
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_SCROLL, pkItem->GetCell(), UPGRADE_INVENTORY);
			}
			else if (pkTarget->IsBook())
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_SCROLL, pkItem->GetCell(), BOOK_INVENTORY);
			}
			//else if (pkTarget->IsStone()){
			//	RefineInformation(pkTarget->GetCell(), REFINE_TYPE_SCROLL, pkItem->GetCell(),STONE_INVENTORY);
			//}
			else
			{
				RefineInformation(pkTarget->GetCell(), REFINE_TYPE_SCROLL, pkItem->GetCell(), INVENTORY);
			}
		}
	}
	else if (pkItem->GetSubType() == USE_DETACHMENT && IS_SET(pkTarget->GetFlag(), ITEM_FLAG_REFINEABLE))
	{
		LogManager::instance().ItemLog(this, pkTarget, "USE_DETACHMENT", pkTarget->GetName());

		bool bHasMetinStone = false;

		const BYTE maxSocketforMetin = 3;

		for (int i = 0; i < ITEM_STONES_MAX_NUM; i++)
		{
			long socket = pkTarget->GetSocket(i);
			if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
			{
				bHasMetinStone = true;
				break;
			}
		}

		if (bHasMetinStone)
		{
			for (int i = 0; i < ITEM_STONES_MAX_NUM; ++i)
			{
				long socket = pkTarget->GetSocket(i);
				if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
				{
					AutoGiveItem(socket);
					//TItemTable* pTable = ITEM_MANAGER::instance().GetTable(pkTarget->GetSocket(i));
					//pkTarget->SetSocket(i, pTable->alValues[2]);
					// Â±Ãº?Ã¸ÂµÂ¹Â·? Â´Ã«?Â¼????Â´?
					pkTarget->SetSocket(i, ITEM_BROKEN_METIN_VNUM);
				}
			}
			pkItem->SetCount(pkItem->GetCount() - 1);
			return true;
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, "127");
			return false;
		}
	}

	return false;
}

EVENTFUNC(kill_campfire_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);

	if (info == NULL)
	{
		sys_err("kill_campfire_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (ch == NULL)
	{ // <Factor>
		return 0;
	}
	ch->m_pkMiningEvent = NULL;
	M2_DESTROY_CHARACTER(ch);
	return 0;
}

bool CHARACTER::GiveRecallItem(LPITEM item)
{
	int idx = GetMapIndex();
	int iEmpireByMapIndex = -1;

	if (idx < 20)
		iEmpireByMapIndex = 1;
	else if (idx < 40)
		iEmpireByMapIndex = 2;
	else if (idx < 60)
		iEmpireByMapIndex = 3;
	else if (idx < 10000)
		iEmpireByMapIndex = 0;

	switch (idx)
	{
	case 66:
	case 216:
		iEmpireByMapIndex = -1;
		break;
	}

	if (iEmpireByMapIndex && GetEmpire() != iEmpireByMapIndex)
	{
		ChatPacket(CHAT_TYPE_INFO, "128");
		return false;
	}

	int pos;

	if (item->GetCount() == 1)	// Â¾??????? ??Â³ÂªÂ¶Ã³Â¸Ã© Â±?Â³? Â¼???.
	{
		item->SetSocket(0, GetX());
		item->SetSocket(1, GetY());
	}
	else if ((pos = GetEmptyInventory(item->GetSize())) != -1) // Â±?Â·Â¸?Ã¶ Â¾?Â´?Â¸Ã© Â´?Â¸Â¥ ??ÂºÂ¥?Ã¤Â¸Â® Â½Â½Â·??Â» ?Â£Â´?Â´?.
	{
		LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), 1);

		if (NULL != item2)
		{
			item2->SetSocket(0, GetX());
			item2->SetSocket(1, GetY());
			item2->AddToCharacter(this, TItemPos(INVENTORY, pos));

			item->SetCount(item->GetCount() - 1);
		}
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, "129");
		return false;
	}

	return true;
}

void CHARACTER::ProcessRecallItem(LPITEM item)
{
	int idx;

	if ((idx = SECTREE_MANAGER::instance().GetMapIndex(item->GetSocket(0), item->GetSocket(1))) == 0)
		return;

	int iEmpireByMapIndex = -1;

	if (idx < 20)
		iEmpireByMapIndex = 1;
	else if (idx < 40)
		iEmpireByMapIndex = 2;
	else if (idx < 60)
		iEmpireByMapIndex = 3;
	else if (idx < 10000)
		iEmpireByMapIndex = 0;

	switch (idx)
	{
	case 66:
	case 216:
		iEmpireByMapIndex = -1;
		break;
		// Â¾?Â·Ã¦Â±ÂºÂµÂµ ??Â¶Â§
	case 301:
	case 302:
	case 303:
	case 304:
		if (GetLevel() < 90)
		{
			ChatPacket(CHAT_TYPE_INFO, "130");
			return;
		}
		else
			break;
	}

	if (iEmpireByMapIndex && GetEmpire() != iEmpireByMapIndex)
	{
		ChatPacket(CHAT_TYPE_INFO, "131");
		item->SetSocket(0, 0);
		item->SetSocket(1, 0);
	}
	else
	{
		sys_log(1, "Recall: %s %d %d -> %d %d", GetName(), GetX(), GetY(), item->GetSocket(0), item->GetSocket(1));
		WarpSet(item->GetSocket(0), item->GetSocket(1));
		item->SetCount(item->GetCount() - 1);
	}
}

#ifdef ENABLE_YOHARA_SYSTEM
bool CHARACTER::IsConquerorItem(LPITEM pItem)
{
	if (pItem->ItemHasApplyRandom())
		return true;

	return false;
}
#endif

int CalculateConsume(LPCHARACTER ch)
{
	static const int WARP_NEED_LIFE_PERCENT = 30;
	static const int WARP_MIN_LIFE_PERCENT = 10;
	// CONSUME_LIFE_WHEN_USE_WARP_ITEM
	int consumeLife = 0;
	{
		// CheckNeedLifeForWarp
		const int curLife = ch->GetHP();
		const int needPercent = WARP_NEED_LIFE_PERCENT;
		const int needLife = ch->GetMaxHP() * needPercent / 100;
		if (curLife < needLife)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "132");
			return -1;
		}

		consumeLife = needLife;


		// CheckMinLifeForWarp: ÂµÂ¶Â¿Â¡ ????Â¼Â­ ???Â¸Â¸Ã© Â¾?Âµ?Â¹?Â·? Â»Ã½Â¸Ã­Â·? ??Â¼?Â·Â®Â´? Â³Â²Â°???Â´?
		const int minPercent = WARP_MIN_LIFE_PERCENT;
		const int minLife = ch->GetMaxHP() * minPercent / 100;
		if (curLife - needLife < minLife)
			consumeLife = curLife - minLife;

		if (consumeLife < 0)
			consumeLife = 0;
	}
	// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM
	return consumeLife;
}

int CalculateConsumeSP(LPCHARACTER lpChar)
{
	static const int NEED_WARP_SP_PERCENT = 30;

	const int curSP = lpChar->GetSP();
	const int needSP = lpChar->GetMaxSP() * NEED_WARP_SP_PERCENT / 100;

	if (curSP < needSP)
	{
		lpChar->ChatPacket(CHAT_TYPE_INFO, "133");
		return -1;
	}

	return needSP;
}

// #define ENABLE_FIREWORK_STUN
#define ENABLE_ADDSTONE_FAILURE
bool CHARACTER::UseItemEx(LPITEM item, TItemPos DestCell)
{
	int iLimitRealtimeStartFirstUseFlagIndex = -1;
	//int iLimitTimerBasedOnWearFlagIndex = -1;

	WORD wDestCell = DestCell.cell;
	BYTE bDestInven = DestCell.window_type;
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		long limitValue = item->GetProto()->aLimits[i].lValue;

		switch (item->GetProto()->aLimits[i].bType)
		{
			case LIMIT_LEVEL:
			{
				if (GetLevel() < limitValue)
				{
					ChatPacket(CHAT_TYPE_INFO, "130");
					return false;
				}

				break;
			}
			case LIMIT_REAL_TIME_START_FIRST_USE:
			{
				iLimitRealtimeStartFirstUseFlagIndex = i;
				break;
			}
			case LIMIT_TIMER_BASED_ON_WEAR:
			{
				//iLimitTimerBasedOnWearFlagIndex = i;
				break;
			}
#ifdef ENABLE_YOHARA_SYSTEM
			case LIMIT_CONQUEROR_LEVEL:
			{
				if (GetConquerorLevel() < limitValue)
				{
					ChatPacket(CHAT_TYPE_INFO, "130");
					return false;
				}

				break;
			}
#endif
			default:
			{
				break;
			}
		}
	}

	if (test_server)
	{
		sys_log(0, "USE_ITEM %s, Inven %d, Cell %d, ItemType %d, SubType %d", item->GetName(), bDestInven, wDestCell, item->GetType(), item->GetSubType());
	}

	if (CArenaManager::instance().IsLimitedItem(GetMapIndex(), item->GetVnum()) == true)
	{
		ChatPacket(CHAT_TYPE_INFO, "134");
		return false;
	}
#ifdef ENABLE_NEWSTUFF
	else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && IsLimitedPotionOnPVP(item->GetVnum()))
	{
		ChatPacket(CHAT_TYPE_INFO, "134");
		return false;
	}
#endif

	// @fixme402 (IsLoadedAffect to block affect hacking)
	if (!IsLoadedAffect())
	{
		ChatPacket(CHAT_TYPE_INFO, "Affects are not loaded yet!");
		return false;
	}

	// @fixme141 BEGIN
	if (TItemPos(item->GetWindow(), item->GetCell()).IsBeltInventoryPosition())
	{
		LPITEM beltItem = GetWear(WEAR_BELT);

		if (NULL == beltItem)
		{
			ChatPacket(CHAT_TYPE_INFO, "<Belt> You can't use this item if you have no equipped belt.");
			return false;
		}

		if (false == CBeltInventoryHelper::IsAvailableCell(item->GetCell() - BELT_INVENTORY_SLOT_START, beltItem->GetValue(0)))
		{
			ChatPacket(CHAT_TYPE_INFO, "<Belt> You can't use this item if you don't upgrade your belt.");
			return false;
		}
	}
	// @fixme141 END


	// Â¾????? ???? Â»Ã§Â¿Ã« ????Âº???Â´? Â»Ã§Â¿Ã«???Ã¶ Â¾?Â¾?ÂµÂµ Â½?Â°Â£?? ?Ã·Â°Â¨Âµ?Â´? Â¹Ã¦Â½? ?Â³Â¸Â®.
	if (-1 != iLimitRealtimeStartFirstUseFlagIndex)
	{
		// ?? Â¹Ã¸??Â¶Ã³ÂµÂµ Â»Ã§Â¿Ã«?? Â¾????????Ã¶ Â¿Â©Âº?Â´? Socket1?Â» ÂºÂ¸Â°Ã­ ??Â´???Â´?. (Socket1Â¿Â¡ Â»Ã§Â¿Ã«?Â½Â¼Ã¶ Â±Ã¢Â·?)
		if (0 == item->GetSocket(1))
		{
			// Â»Ã§Â¿Ã«Â°Â¡Â´?Â½?Â°Â£?Âº Default Â°Âª?Â¸Â·? Limit Value Â°Âª?Â» Â»Ã§Â¿Ã«??Âµ?, Socket0Â¿Â¡ Â°Âª?? ???Â¸Â¸Ã© Â±? Â°Âª?Â» Â»Ã§Â¿Ã«??ÂµÂµÂ·? ??Â´?. (Â´??Â§Â´? ??)
			long duration = (0 != item->GetSocket(0)) ? item->GetSocket(0) : item->GetProto()->aLimits[iLimitRealtimeStartFirstUseFlagIndex].lValue;

			if (0 == duration)
				duration = 60 * 60 * 24 * 7;

			item->SetSocket(0, time(0) + duration);
			item->StartRealTimeExpireEvent();
		}

		if (false == item->IsEquipped())
			item->SetSocket(1, item->GetSocket(1) + 1);
	}


	if (item->GetVnum() == 79714)
	{
		ChatPacket(CHAT_TYPE_COMMAND, "ShowNameWindow %d", 1);
		return true;
	}

	if (item->GetVnum() == 79715)
	{
		ChatPacket(CHAT_TYPE_COMMAND, "ShowNameWindow %d", 2);
		return true;
	}

#ifdef __NEWPET_SYSTEM__

	if (item->GetVnum() == 55001)
	{

		LPITEM item2;

		if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
			return false;

		if (item2->IsExchanging())
			return false;

		if (item2->GetVnum() > 55704 || item2->GetVnum() < 55701)
			return false;


		char szQuery1[1024];
		snprintf(szQuery1, sizeof(szQuery1), "SELECT duration FROM new_petsystem WHERE id = %lu LIMIT 1", item2->GetID());
		std::unique_ptr<SQLMsg> pmsg2(DBManager::instance().DirectQuery(szQuery1));
		if (pmsg2->Get()->uiNumRows > 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pmsg2->Get()->pSQLResult);
			if (atoi(row[0]) > 0)
			{
				if (GetNewPetSystem()->IsActivePet())
				{
					ChatPacket(CHAT_TYPE_INFO, "Devi mandare via il tuo pet prima di proseguire.");
					return false;
				}

				std::unique_ptr<SQLMsg> pMsgUpdate(DBManager::instance().DirectQuery("UPDATE new_petsystem SET duration =(tduration) WHERE id = %d", item2->GetID()));
				ChatPacket(CHAT_TYPE_INFO, "La durata del tuo pet e stata ripristinata.");
			}
			else
			{
				std::unique_ptr<SQLMsg> pMsgUpdate(DBManager::instance().DirectQuery("UPDATE new_petsystem SET duration =(tduration/2) WHERE id = %d", item2->GetID()));
				ChatPacket(CHAT_TYPE_INFO, "La durata del tuo pet e stata ripristinata.");
			}
			item->SetCount(item->GetCount() - 1);
			return true;
		}
		else
			return false;
	}

	if (item->GetVnum() >= 55701 && item->GetVnum() <= 55704)
	{
		LPITEM item2;

		if (item2 = GetItem(DestCell))
		{
			if (item2->GetVnum() == 55002)
			{
				if (item2->GetAttributeValue(0) > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "Il trasportino contiene gia un Pet.");
				}
				else
				{
					if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
						return false;

					if (item2->IsExchanging())
						return false;

					if (GetNewPetSystem()->IsActivePet())
					{
						ChatPacket(CHAT_TYPE_INFO, "Devi mandare via il tuo Pet prima di proseguire.");
						return false;
					}


					for (int b = 0; b < 3; b++)
					{
						item2->SetForceAttribute(b, 1, item->GetAttributeValue(b));
					}

					item2->SetForceAttribute(3, 1, item->GetAttributeValue(3));
					item2->SetForceAttribute(4, 1, item->GetAttributeValue(4));
					DWORD vnum1 = item->GetVnum() - 55700;
					item2->SetSocket(0, vnum1);
					item2->SetSocket(1, item->GetSocket(1));
					std::unique_ptr<SQLMsg> pMsgUpdate(DBManager::instance().DirectQuery("UPDATE new_petsystem SET id =%d WHERE id = %d", item2->GetID(), item->GetID()));
					ITEM_MANAGER::instance().RemoveItem(item);
					return true;
				}
			}
		}
	}


	if (item->GetVnum() == 55002 && item->GetAttributeValue(0) > 0)
	{

		int pos = GetEmptyInventory(item->GetSize());
		if (pos == -1)
		{
			ChatPacket(CHAT_TYPE_INFO, "Non hai abbastanza spazio.");
			return false;
		}

		if (item->IsExchanging())
			return false;
		DWORD vnum2 = 55700 + item->GetSocket(0);
		LPITEM item2 = AutoGiveItem(vnum2, 1);
		for (int b = 0; b < 3; b++)
		{
			item2->SetForceAttribute(b, 1, item->GetAttributeValue(b));
		}
		item2->SetForceAttribute(3, 1, item->GetAttributeValue(3));
		item2->SetForceAttribute(4, 1, item->GetAttributeValue(4));
		item2->SetSocket(1, item->GetSocket(1));
		std::unique_ptr<SQLMsg> pMsgUpdate(DBManager::instance().DirectQuery("UPDATE new_petsystem SET id =%d WHERE id = %d", item2->GetID(), item->GetID()));
		ITEM_MANAGER::instance().RemoveItem(item);
		return true;

	}
#endif

	switch (item->GetType())
	{


	case ITEM_HAIR:
		return ItemProcess_Hair(item, wDestCell);

	case ITEM_POLYMORPH:
		return ItemProcess_Polymorph(item);

#ifdef __BUFFI_SUPPORT__
	case ITEM_BUFFI:
		switch (item->GetSubType())
		{
		case BUFFI_NAME_SCROLL:
		{
			if (item->GetSIGVnum() == 0)
				quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
			else
				quest::CQuestManager::instance().SIGUse(GetPlayerID(), item->GetSIGVnum(), item, false);
		}
		break;
		case BUFFI_SCROLL:
		{
			const int now = get_global_time();
			if (GetProtectTime("buffi_next_level") > now)
			{
				ChatPacket(CHAT_TYPE_INFO, "You need little bit slow. You need wait %d second.", GetProtectTime("buffi_next_level") - now);
				return false;
			}
			SetProtectTime("buffi_next_level", now + 1);
			BUFFI_MANAGER::Instance().Summon(!item->GetSocket(1), item);
		}
		break;
		}
		break;
#endif

	case ITEM_QUEST:
	{
#ifdef __AUTOMATIC_HUNTING_SYSTEM__
		if (item->GetVnum() == 49324)
		{
#ifdef USE_PREMIUM_AFFECT
			return false;
#else
			if (FindAffect(AFFECT_AUTO_HUNT))
			{
				CAffect* pkAff = FindAffect(AFFECT_AUTO_HUNT);

				// ChatPacket(CHAT_TYPE_INFO, "You already has affect");
				AddAffect(AFFECT_AUTO_HUNT, 0, 1, AFF_NONE, pkAff->lDuration + 604800, 0, true/*bOverride*/);

				ChatPacket(CHAT_TYPE_INFO, "Affect added for 7 day.");
				item->SetCount(item->GetCount() - 1);

				return false;
			}

			AddAffect(AFFECT_AUTO_HUNT, 0, 1, AFF_NONE, 604800, 0, false/*bOverride*/);

			ChatPacket(CHAT_TYPE_INFO, "Affect added for 7 day.");
			item->SetCount(item->GetCount() - 1);
			return true;
#endif
		}
#endif

#ifdef __RENEWAL_BRAVE_CAPE__
		if (item->GetVnum() >= 20570 && item->GetVnum() <= 20575)
		{
			if (FindAffect(AFFECT_BRAVE_CAPE))
			{
				ChatPacket(CHAT_TYPE_INFO, "You already has this affect!");
				return false;
			}
			const int timeAdd = item->GetValue(0) > 30 ? INFINITE_AFFECT_DURATION : 60 * 60 * 24 * item->GetValue(0);
			if (!timeAdd)
			{
				ChatPacket(CHAT_TYPE_INFO, "[SYSTEM] Contact game master. This item not has time data!");
				return false;
			}

			AddAffect(AFFECT_BRAVE_CAPE, POINT_NONE, 0, AFF_NONE, timeAdd, 0, 0, false);
			item->SetCount(item->GetCount() - 1);
			return true;
		}
#endif
		if (GetArena() != NULL || IsObserverMode() == true)
		{
			if (item->GetVnum() == 50051 || item->GetVnum() == 50052 || item->GetVnum() == 50053)
			{
				ChatPacket(CHAT_TYPE_INFO, "134");
				return false;
			}
		}
#ifdef RENEWAL_MISSION_BOOKS
		if (item->GetVnum() >= 50307 && item->GetVnum() <= 50310)
		{
			CHARACTER_MANAGER::Instance().GiveNewMission(item, this);
			return true;
		}
#endif

		if (!IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE | ITEM_FLAG_QUEST_USE_MULTIPLE))
		{
			if (item->GetSIGVnum() == 0)
			{
				quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
			}
			else
			{
				quest::CQuestManager::instance().SIGUse(GetPlayerID(), item->GetSIGVnum(), item, false);
			}
		}
	}
	break;

	case ITEM_CAMPFIRE:
	{
		float fx, fy;
		GetDeltaByDegree(GetRotation(), 100.0f, &fx, &fy);

		LPSECTREE tree = SECTREE_MANAGER::instance().Get(GetMapIndex(), (long)(GetX() + fx), (long)(GetY() + fy));

		if (!tree)
		{
			ChatPacket(CHAT_TYPE_INFO, "135");
			return false;
		}

		if (tree->IsAttr((long)(GetX() + fx), (long)(GetY() + fy), ATTR_WATER))
		{
			ChatPacket(CHAT_TYPE_INFO, "136");
			return false;
		}

		LPCHARACTER campfire = CHARACTER_MANAGER::instance().SpawnMob(fishing::CAMPFIRE_MOB, GetMapIndex(), (long)(GetX() + fx), (long)(GetY() + fy), 0, false, number(0, 359));

		char_event_info* info = AllocEventInfo<char_event_info>();

		info->ch = campfire;

		campfire->m_pkMiningEvent = event_create(kill_campfire_event, info, PASSES_PER_SEC(40));

		item->SetCount(item->GetCount() - 1);
	}
	break;

	case ITEM_UNIQUE:
	{
		switch (item->GetSubType())
		{
		case USE_ABILITY_UP:
		{
			switch (item->GetValue(0))
			{
			case APPLY_MOV_SPEED:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_MOV_SPEED, item->GetValue(2), AFF_MOV_SPEED_POTION, item->GetValue(1), 0, true, true);
				break;

			case APPLY_ATT_SPEED:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ATT_SPEED, item->GetValue(2), AFF_ATT_SPEED_POTION, item->GetValue(1), 0, true, true);
				break;

			case APPLY_STR:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ST, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_DEX:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_DX, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_CON:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_HT, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_INT:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_IQ, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_CAST_SPEED:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_CASTING_SPEED, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_RESIST_MAGIC:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_RESIST_MAGIC, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_ATT_GRADE_BONUS:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ATT_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_DEF_GRADE_BONUS:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_DEF_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;
			}
		}

		if (GetDungeon())
			GetDungeon()->UsePotion(this);

		if (GetWarMap())
			GetWarMap()->UsePotion(this, item);

		item->SetCount(item->GetCount() - 1);
		break;

		default:
		{
			if (item->GetSubType() == USE_SPECIAL)
			{
				sys_log(0, "ITEM_UNIQUE: USE_SPECIAL %u", item->GetVnum());
#ifdef ENABLE_MULTI_FARM_BLOCK
				if (item->GetVnum() >= 55610 && item->GetVnum() <= 55615)
				{
					if (FindAffect(AFFECT_MULTI_FARM_PREMIUM))
					{
						ChatPacket(CHAT_TYPE_INFO, "1074");
						return false;
					}
					else
					{
						AddAffect(AFFECT_MULTI_FARM_PREMIUM, POINT_NONE, item->GetValue(1), AFF_NONE, item->GetValue(0), 0, false);
						item->SetCount(item->GetCount() - 1);
						CHARACTER_MANAGER::Instance().CheckMultiFarmAccount(GetDesc()->GetHostName(), GetPlayerID(), GetName(), GetRewardStatus());
						ChatPacket(CHAT_TYPE_INFO, "1075");
						ChatPacket(CHAT_TYPE_INFO, "1076");
					}
				}
#endif
			}
			else
			{
				if (!item->IsEquipped())
					EquipItem(item);
				else
					UnequipItem(item);
			}
		}
		break;
		}
	}
	break;
#ifdef __ITEM_SHINING__
	case ITEM_SHINING:
#endif
	case ITEM_NEW_RINGS:
	case ITEM_COSTUME:
	case ITEM_WEAPON:
	case ITEM_ARMOR:
	case ITEM_ROD:
	case ITEM_RING:		// Â½?Â±? Â¹??Ã¶ Â¾?????
	case ITEM_BELT:		// Â½?Â±? ÂºÂ§?Â® Â¾?????
	case ITEM_RING_SPECIAL:		// Â½?Â±? ÂºÂ§?Â® Â¾?????
#ifdef ENABLE_NEWEQ		
	case ITEM_NEWRING:
#endif		
		// MINING
	case ITEM_PICK:
	case ITEM_BOOSTER:
		// END_OF_MINING
		if (!item->IsEquipped())
			EquipItem(item);
		else
			UnequipItem(item);
		break;
		// ?Ã¸Â¿Ã«???Ã¶ Â¾??Âº Â¿Ã«?Â¥Â¼Â®?Âº Â»Ã§Â¿Ã«?? Â¼Ã¶ Â¾Ã¸Â´?.
		// ?Â¤Â»Ã³?Ã»?? ?Â¬Â¶Ã³Â¶Ã³Â¸Ã©, Â¿Ã«?Â¥Â¼Â®Â¿Â¡ Â°Ã¼??Â¿Â© item use ???Â¶?Â» ÂºÂ¸Â³Â¾ Â¼Ã¶ Â¾Ã¸Â´?.
		// Â¿Ã«?Â¥Â¼Â® ?Ã¸Â¿Ã«?Âº item move ???Â¶?Â¸Â·? ??Â´?.
		// ?Ã¸Â¿Ã«?? Â¿Ã«?Â¥Â¼Â®?Âº ???Ã¢??Â´?.
	case ITEM_DS:
	{
		if (!item->IsEquipped())
			return false;
		return DSManager::instance().PullOut(this, NPOS, item);
		break;
	}
	case ITEM_SPECIAL_DS:
		if (!item->IsEquipped())
			EquipItem(item);
		else
			UnequipItem(item);
		break;
#ifdef ENABLE_NEW_PET_SYSTEM
	case ITEM_PET:
	{
		switch (item->GetSubType())
		{
		case PET_LEVELABLE:
		{
			time_t now = get_global_time();
			if (GetProtectTime("newpet.ride") > now)
			{
				ChatPacket(CHAT_TYPE_INFO, "139 %d", GetProtectTime("newpet.ride") - now);
				return false;
			}
			SetProtectTime("newpet.ride", now + 1);

			if (time(0) > item->GetSocket(POINT_PET_DURATION))
			{
				ChatPacket(CHAT_TYPE_INFO, "140 I%d", 55001);
				if (item->IsEquipped())
					UnequipItem(item);
				return false;
			}

			if (!item->IsEquipped())
				EquipItem(item);
			else
				UnequipItem(item);
		}
		break;

		case PET_PROTEIN:
		{
			LPITEM item2 = GetItem(DestCell);
			if (item2 != NULL)
			{
				if (!(item2->GetType() == ITEM_PET && item2->GetSubType() == PET_LEVELABLE))
					return false;
				long oldTime = item2->GetSocket(POINT_PET_DURATION) - time(0);
				if (oldTime < 0)
					oldTime = 0;
				if (oldTime >= ((60 * 60 * 24 * 20) * 6) - 100)
				{
					ChatPacket(CHAT_TYPE_INFO, "141");
					return false;
				}
				else
				{
					long newTime = MIN((oldTime + 60 * 60 * 24 * 7), (60 * 60 * 24 * 20) * 6);
					item2->SetSocket(POINT_PET_DURATION, time(0) + newTime);
					item->SetCount(item->GetCount() - 1);
					ChatPacket(CHAT_TYPE_INFO, "142 %d", 7);
				}
			}
			else
			{
				CPetSystem* pet = GetPetSystem();
				if (pet)
				{
					LPPET petActor = pet->GetNewPet();
					if (petActor)
					{
						if (petActor->PointChange(POINT_PET_DURATION, 60 * 60 * 24 * 7))
						{
							ChatPacket(CHAT_TYPE_INFO, "142 %d", 7);
							item->SetCount(item->GetCount() - 1);
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, "141");
							return false;
						}
					}
					else
						ChatPacket(CHAT_TYPE_INFO, "143");
				}
			}
		}
		break;

		case PET_DEW:
		{
			LPITEM item2 = GetItem(DestCell);
			if (item2 != NULL)
			{
				if (!(item2->GetType() == ITEM_PET && item2->GetSubType() == PET_LEVELABLE))
					return false;

				BYTE bonusType = item->GetValue(0);
				BYTE bonusStep = item->GetValue(1);

				DWORD petLevel = item2->GetSocket(POINT_PET_LEVEL);
				long bonusLevel = item2->GetSocket(POINT_PET_BONUS_1 + bonusType);

				if (bonusStep == 1)
				{
					if (bonusLevel >= 0 && bonusLevel <= 4)
					{
						item2->SetSocket(POINT_PET_BONUS_1 + bonusType, bonusLevel + 1);
						//item2->UpdatePacket();
						item->SetCount(item->GetCount() - 1);
						return true;
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, "144");
						return false;
					}
				}
				else if (bonusStep == 2)
				{
					if (petLevel < 40)
					{
						ChatPacket(CHAT_TYPE_INFO, "145 %d", 40);
						return false;
					}

					if (bonusLevel >= 5 && bonusLevel <= 9)
					{
						item2->SetSocket(POINT_PET_BONUS_1 + bonusType, bonusLevel + 1);
						//item2->UpdatePacket();
						item->SetCount(item->GetCount() - 1);
						return true;
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, "144");
						return false;
					}

				}
				else if (bonusStep == 3)
				{
					if (petLevel < 75)
					{
						ChatPacket(CHAT_TYPE_INFO, "145 %d", 75);
						return false;
					}

					if (bonusLevel >= 10 && bonusLevel <= 14)
					{
						item2->SetSocket(POINT_PET_BONUS_1 + bonusType, bonusLevel + 1);
						//item2->UpdatePacket();
						item->SetCount(item->GetCount() - 1);
						return true;
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, "144");
						return false;
					}
				}
				else if (bonusStep == 4)
				{
					if (petLevel < 100)
					{
						ChatPacket(CHAT_TYPE_INFO, "145 %d", 100);
						return false;
					}

					if (bonusLevel >= 15 && bonusLevel <= 19)
					{
						item2->SetSocket(POINT_PET_BONUS_1 + bonusType, bonusLevel + 1);
						//item2->UpdatePacket();
						item->SetCount(item->GetCount() - 1);
						return true;
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, "144");
						return false;
					}
				}
			}
			else
			{
				CPetSystem* pet = GetPetSystem();
				if (pet)
				{
					LPPET petActor = pet->GetNewPet();
					if (petActor)
					{
						if (petActor->IncreaseBonus(item->GetValue(0), item->GetValue(1)))
							item->SetCount(item->GetCount() - 1);
					}
					else
						ChatPacket(CHAT_TYPE_INFO, "143");
				}
			}
		}
		break;

		case PET_SKILL:
		{
			LPITEM item2 = GetItem(DestCell);
			if (item2 != NULL)
			{
				if (!(item2->GetType() == ITEM_PET && item2->GetSubType() == PET_LEVELABLE))
					return false;

				BYTE skillIndex = item->GetValue(0);

				BYTE bySlotIndex = 99;
				for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
				{
					if (item2->GetAttributeType(j) == skillIndex)
					{
						bySlotIndex = j;
						break;
					}
				}
				if (bySlotIndex == 99)
				{
					BYTE emptyIndex = 99;
					for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
					{
						if (item2->GetAttributeType(j) == 0)
						{
							emptyIndex = j;
							break;
						}
					}
					if (emptyIndex == 99)
					{
						ChatPacket(CHAT_TYPE_INFO, "146");
						return false;
					}

					item2->SetForceAttribute(emptyIndex, skillIndex, 0);
					ChatPacket(CHAT_TYPE_INFO, "147 I%d", 55009 + skillIndex);
					item->SetCount(item->GetCount() - 1);
					return true;
				}
				else
				{
					BYTE type = item2->GetAttributeType(bySlotIndex);
					long value = item2->GetAttributeValue(bySlotIndex);
					if (value > 19)
					{
						ChatPacket(CHAT_TYPE_INFO, "148 I%d", 55009 + skillIndex);
						return false;
					}
					item2->SetForceAttribute(bySlotIndex, type, value + 1);
					ChatPacket(CHAT_TYPE_INFO, "149 I%d", 55009 + skillIndex);
					item->SetCount(item->GetCount() - 1);
				}
			}
			else
			{
				CPetSystem* pet = GetPetSystem();
				if (pet)
				{
					LPPET petActor = pet->GetNewPet();
					if (petActor)
					{
						if (petActor->IncreaseSkill(item->GetValue(0)))
							item->SetCount(item->GetCount() - 1);
					}
					else
						ChatPacket(CHAT_TYPE_INFO, "143");
				}
			}
		}
		break;

		case PET_SKILL_DEL_BOOK:
		{
			WORD index = DestCell.cell;
			if (index >= 0 && index <= 15)
			{
				CPetSystem* pet = GetPetSystem();
				if (pet)
				{
					LPPET petActor = pet->GetNewPet();
					if (petActor)
					{
						LPITEM petItem = petActor->GetSummonItem();
						if (!petItem)
							return false;
						BYTE type = petItem->GetAttributeType(index);
						// if (type >= 1 && type <= 25)
						if (type >= 1)
						{
							petActor->PointChange(POINT_PET_SKILL_INDEX_1 + index, 0);
							petActor->PointChange(POINT_PET_SKILL_LEVEL_1 + index, 0);
							ChatPacket(CHAT_TYPE_INFO, "150 I%d", 55009 + type);
							item->SetCount(item->GetCount() - 1);
						}
					}
					else
						ChatPacket(CHAT_TYPE_INFO, "143");
				}
			}
		}
		break;
		case PET_EXPFOOD_PER:
		{
			CPetSystem* pet = GetPetSystem();
			if (pet)
			{
				LPPET petActor = pet->GetNewPet();
				if (petActor)
				{
					DWORD petExp = petActor->GetNextExp();
					float realExp = (float(petActor->GetNextExp()) / 100.0) * 50.0;
					if (petActor->PointChange(POINT_PET_EXP, int(realExp)))
					{
						ChatPacket(CHAT_TYPE_INFO, "151");
						item->SetCount(item->GetCount() - 1);
					}
					else
						ChatPacket(CHAT_TYPE_INFO, "152");
				}
				else
					ChatPacket(CHAT_TYPE_INFO, "143");
			}
		}
		break;
		case PET_SKILL_ALL_DEL_BOOK:
		{
			LPITEM item2 = GetItem(DestCell);
			if (item2 != NULL)
			{
				if (!(item2->GetType() == ITEM_PET && item2->GetSubType() == PET_LEVELABLE))
					return false;
				bool isHaveSkill = false;
				for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
				{
					BYTE type = item2->GetAttributeType(j);
					if (type != 0 && type != 99)
					{
						isHaveSkill = true;
						break;
					}
				}
				if (!isHaveSkill)
				{
					ChatPacket(CHAT_TYPE_INFO, "153");
					return false;
				}
				else
				{
					for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
					{
						BYTE type = item2->GetAttributeType(j);
						if (type != 0 && type != 99)
							item2->SetForceAttribute(j, 0, 0);
					}
					ChatPacket(CHAT_TYPE_INFO, "154");
					item->SetCount(item->GetCount() - 1);
				}
			}
			else
			{
				CPetSystem* pet = GetPetSystem();
				if (pet)
				{
					LPPET petActor = pet->GetNewPet();
					if (petActor)
					{
						LPITEM petItem = petActor->GetSummonItem();
						if (!petItem)
							return false;
						bool isHaveSkill = false;
						for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
						{
							BYTE type = petItem->GetAttributeType(j);
							if (type != 0 && type != 99)
							{
								isHaveSkill = true;
								break;
							}
						}
						if (!isHaveSkill)
						{
							ChatPacket(CHAT_TYPE_INFO, "153");
							return false;
						}
						else
						{
							for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
							{
								BYTE type = petItem->GetAttributeType(j);
								if (type != 0 && type != 99)
									petItem->SetForceAttribute(j, 0, 0);
							}
							ChatPacket(CHAT_TYPE_COMMAND, "UpdatePet %d", POINT_PET_SKILL_INDEX_1);
							ChatPacket(CHAT_TYPE_INFO, "154");
							item->SetCount(item->GetCount() - 1);
						}
					}
					else
						ChatPacket(CHAT_TYPE_INFO, "143");
				}
			}
		}
		break;
		}
	}
	break;
#endif
	case ITEM_FISH:
	{
		if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
		{
			ChatPacket(CHAT_TYPE_INFO, "134");
			return false;
		}
#ifdef ENABLE_NEWSTUFF
		else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(item->GetVnum()))
		{
			ChatPacket(CHAT_TYPE_INFO, "134");
			return false;
		}
#endif

		if (item->GetSubType() == FISH_ALIVE)
			fishing::UseFish(this, item);
	}
	break;

	case ITEM_TREASURE_BOX:
	{
		return false;
		ChatPacket(CHAT_TYPE_TALKING, "155");
	}
	break;

	case ITEM_TREASURE_KEY:
	{
		LPITEM item2;

		if (!GetItem(DestCell) || !(item2 = GetItem(DestCell)))
			return false;

		if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
			return false;

		if (item2->GetType() != ITEM_TREASURE_BOX)
		{
			ChatPacket(CHAT_TYPE_TALKING, "156");
			return false;
		}

		if (item->GetValue(0) == item2->GetValue(0))
		{
			ChatPacket(CHAT_TYPE_TALKING, "157");
			DWORD dwBoxVnum = item2->GetVnum();
			std::vector <DWORD> dwVnums;
			std::vector <DWORD> dwCounts;
			std::vector <LPITEM> item_gets(0);
			int count = 0;

			if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
			{
				item->SetCount(item->GetCount() - 1);
				item2->SetCount(item2->GetCount() - 1);

				for (int i = 0; i < count; i++)
				{
					switch (dwVnums[i])
					{
					case CSpecialItemGroup::GOLD:
#ifdef __CHAT_SETTINGS__
						ChatPacket(CHAT_TYPE_MONEY_INFO, "158 %d", dwCounts[i]);
#else
						ChatPacket(CHAT_TYPE_INFO, "158 %d", dwCounts[i]);
#endif
						break;
					case CSpecialItemGroup::EXP:
						ChatPacket(CHAT_TYPE_INFO, "159");
#ifdef __CHAT_SETTINGS__
						ChatPacket(CHAT_TYPE_EXP_INFO, "160 %d", dwCounts[i]);
#else
						ChatPacket(CHAT_TYPE_INFO, "160 %d", dwCounts[i]);
#endif
						break;
					case CSpecialItemGroup::MOB:
						ChatPacket(CHAT_TYPE_INFO, "161");
						break;
					case CSpecialItemGroup::SLOW:
						ChatPacket(CHAT_TYPE_INFO, "162");
						break;
					case CSpecialItemGroup::DRAIN_HP:
						ChatPacket(CHAT_TYPE_INFO, "163");
						break;
					case CSpecialItemGroup::POISON:
						ChatPacket(CHAT_TYPE_INFO, "164");
						break;
#ifdef ENABLE_WOLFMAN_CHARACTER
					case CSpecialItemGroup::BLEEDING:
						ChatPacket(CHAT_TYPE_INFO, "164");
						break;
#endif
					case CSpecialItemGroup::MOB_GROUP:
						ChatPacket(CHAT_TYPE_INFO, "161");
						break;
					default:
						if (item_gets[i])
						{
#ifdef __CHAT_SETTINGS__
							if (dwCounts[i] > 1)
								ChatPacket(CHAT_TYPE_ITEM_INFO, "165 %s %d", item_gets[i]->GetName(), dwCounts[i]);
							else
								ChatPacket(CHAT_TYPE_ITEM_INFO, "166 %s", item_gets[i]->GetName());
#else
							if (dwCounts[i] > 1)
								ChatPacket(CHAT_TYPE_INFO, "165 %s %d", item_gets[i]->GetName(), dwCounts[i]);
							else
								ChatPacket(CHAT_TYPE_INFO, "166 %s", item_gets[i]->GetName());
#endif
						}
					}
				}
			}
			else
			{
				ChatPacket(CHAT_TYPE_TALKING, "167");
				return false;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_TALKING, "167");
			return false;
		}
	}
	break;

	case ITEM_GIFTBOX:
	{
#ifdef ENABLE_NEWSTUFF
		if (0 != g_BoxUseTimeLimitValue)
		{
			if (get_dword_time() < m_dwLastBoxUseTime + g_BoxUseTimeLimitValue)
			{
				ChatPacket(CHAT_TYPE_INFO, "168");
				return false;
			}
		}

		m_dwLastBoxUseTime = get_dword_time();
#endif
		DWORD dwBoxVnum = item->GetVnum();
		std::vector <DWORD> dwVnums;
		std::vector <DWORD> dwCounts;
		std::vector <LPITEM> item_gets(0);
		int count = 0;

		if (dwBoxVnum > 51500 && dwBoxVnum < 52000)	// Â¿Ã«?Â¥Â¿Ã¸Â¼Â®ÂµÃ©
		{
			if (!(this->DragonSoul_IsQualified()))
			{
				ChatPacket(CHAT_TYPE_INFO, "169");
				return false;
			}
		}

#if defined(__BL_67_ATTR__) && !defined(USE_BL_67_ATTR_RENEWED)
		switch (dwBoxVnum)
		{
			case POWERSHARD_CHEST:
			{
				// The Powershard Chest can be purchased from Seon-Hae in exchange for 10 Skill Books.(From Wiki)
				// It can contain Powershards of any kind or a Skill Book.(From Wiki)
				// You can edit here for skill books(From black)
				if (number(1, 100) <= 30)
				{
					AutoGiveItem(CItemVnumHelper::Get67MaterialVnum(number(0, gPlayerMaxLevel)
#ifdef ENABLE_YOHARA_SYSTEM
, 0
#endif
					));
				}
				else
				{
					ChatPacket(CHAT_TYPE_TALKING, "170");
				}

				item->SetCount(item->GetCount() - 1);
				return true;
			}
			case ELEGANT_POWERSHARD_CHEST:
			{
				if (number(1, 100) <= 60)
				{
					AutoGiveItem(CItemVnumHelper::Get67MaterialVnum(number(0, gPlayerMaxLevel)
#ifdef ENABLE_YOHARA_SYSTEM
, 0
#endif
					));
				}
				else
				{
					ChatPacket(CHAT_TYPE_TALKING, "170");
				}

				item->SetCount(item->GetCount() - 1);
				return true;
			}
			case LUCENT_POWERSHARD_CHEST:
			{
				for (BYTE _i = 0; _i < 5; _i++)
				{
					AutoGiveItem(CItemVnumHelper::Get67MaterialVnum(number(0, gPlayerMaxLevel)
#ifdef ENABLE_YOHARA_SYSTEM
, 0
#endif
					));
				}

				item->SetCount(item->GetCount() - 1);
				return true;
			}
			default:
			{
				break;
			}
		}
#endif

#ifdef ENABLE_MULTI_CHEST
		OpenChest(item, 1);
#ifdef ENABLE_RANKING
		SetRankPoints(17, GetRankPoints(17) + 1);
#endif
		return true;
#else

		if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
		{

			item->SetCount(item->GetCount() - 1);
#ifdef ENABLE_RANKING
			SetRankPoints(17, GetRankPoints(17) + 1);
#endif
#ifdef BATTLEPASS_WORLDARD
			CBattlePass::instance().UpdateMisionBattlePass(this, OPEN_CHEST_BP, 1);
#endif

			for (int i = 0; i < count; i++)
			{
				switch (dwVnums[i])
				{
				case CSpecialItemGroup::GOLD:
#ifdef __CHAT_SETTINGS__
					ChatPacket(CHAT_TYPE_MONEY_INFO, "158 %d", dwCounts[i]);
#else
					ChatPacket(CHAT_TYPE_INFO, "158 %d", dwCounts[i]);
#endif
					break;
				case CSpecialItemGroup::EXP:
					ChatPacket(CHAT_TYPE_INFO, "159");
#ifdef __CHAT_SETTINGS__
					ChatPacket(CHAT_TYPE_EXP_INFO, "160 %d", dwCounts[i]);
#else
					ChatPacket(CHAT_TYPE_INFO, "160 %d", dwCounts[i]);
#endif
					break;
				case CSpecialItemGroup::MOB:
					ChatPacket(CHAT_TYPE_INFO, "161");
					break;
				case CSpecialItemGroup::SLOW:
					ChatPacket(CHAT_TYPE_INFO, "162");
					break;
				case CSpecialItemGroup::DRAIN_HP:
					ChatPacket(CHAT_TYPE_INFO, "163");
					break;
				case CSpecialItemGroup::POISON:
					ChatPacket(CHAT_TYPE_INFO, "164");
					break;
#ifdef ENABLE_WOLFMAN_CHARACTER
				case CSpecialItemGroup::BLEEDING:
					ChatPacket(CHAT_TYPE_INFO, "164");
					break;
#endif
				case CSpecialItemGroup::MOB_GROUP:
					ChatPacket(CHAT_TYPE_INFO, "161");
					break;
				default:
					if (item_gets[i])
					{
#ifdef __CHAT_SETTINGS__
						if (dwCounts[i] > 1)
							ChatPacket(CHAT_TYPE_ITEM_INFO, "165 %s %d", item_gets[i]->GetName(), dwCounts[i]);
						else
							ChatPacket(CHAT_TYPE_ITEM_INFO, "166 %s", item_gets[i]->GetName());
#else
						if (dwCounts[i] > 1)
							ChatPacket(CHAT_TYPE_INFO, "165 %s %d", item_gets[i]->GetName(), dwCounts[i]);
						else
							ChatPacket(CHAT_TYPE_INFO, "166 %s", item_gets[i]->GetName());
#endif
					}
				}
			}
			RankPlayer::instance().SendInfoPlayer(this, RANK_BY_BOX_OPENED, RankPlayer::instance().GetProgressByPID(GetPlayerID(), RANK_BY_BOX_OPENED) + 1, false);
		}
		else
		{
			ChatPacket(CHAT_TYPE_TALKING, "171");
			return false;
		}
#endif
	}
	break;

	case ITEM_SKILLFORGET:
	{
		if (!item->GetSocket(0))
		{
			ITEM_MANAGER::instance().RemoveItem(item);
			return false;
		}

		DWORD dwVnum = item->GetSocket(0);

		if (SkillLevelDown(dwVnum))
		{
			ITEM_MANAGER::instance().RemoveItem(item);
			ChatPacket(CHAT_TYPE_INFO, "172");
		}
		else
			ChatPacket(CHAT_TYPE_INFO, "173");
	}
	break;

	case ITEM_SKILLBOOK:
	{
		if (IsPolymorphed())
		{
			ChatPacket(CHAT_TYPE_INFO, "174");
			return false;
		}

		DWORD dwVnum = 0;

		if (item->GetVnum() == 50300)
		{
			dwVnum = item->GetSocket(0);
		}
		else
		{
			// Â»ÃµÂ·?Â¿Ã® Â¼Ã¶Â·?Â¼Â­Â´? value 0 Â¿Â¡ Â½Âº?Â³ Â¹Ã¸?Â£Â°Â¡ ???Â¸Â¹?Â·? Â±?Â°??Â» Â»Ã§Â¿Ã«.
			dwVnum = item->GetValue(0);
		}

		if (0 == dwVnum)
		{
			ITEM_MANAGER::instance().RemoveItem(item);

			return false;
		}

		if (true == LearnSkillByBook(dwVnum))
		{
#ifdef ENABLE_BOOKS_STACKFIX
			item->SetCount(item->GetCount() - 1);
#else
			ITEM_MANAGER::instance().RemoveItem(item);
#endif

			int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);

			if (distribution_test_server)
				iReadDelay /= 3;

			SetSkillNextReadTime(dwVnum, get_global_time() + iReadDelay);
		}
	}
	break;
//#ifdef USE_CRYSTAL_SYSTEM
	case ITEM_CRYSTAL:
	{
#ifdef USE_CRYSTAL_SYSTEM
		if (item->ModifyBoostCrystal(item->GetSocket(CRYSTAL_STATUS_SOCKET_SLOT) != 0 ? false : true, false, true))
		{
			
		}

		return true;
#else
		return false;
#endif
	}
	break;
//#endif
	case ITEM_USE:
	{
//ifdef USE_CRYSTAL_SYSTEM
		switch (item->GetSubType())
		{
			case USE_ADD_ATTR_CRYSTAL:
			{
#ifdef USE_CRYSTAL_SYSTEM
				if (!IsValidItemPosition(DestCell))
				{
					return false;
				}

				LPITEM item2 = GetItem(DestCell);
				if (!item2)
				{
					return false;
				}

				if (item2->GetType() != ITEM_CRYSTAL)
				{
					ChatPacket(CHAT_TYPE_INFO, "1521 %s", item->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s can be used just on Crystals."), item->GetName());
					return false;
				}

				if (item2->IsExchanging()/* || item2->IsEquipped()*/)
				{
					return false;
				}

#ifdef ENABLE_SOULBIND_SYSTEM
				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, "1522 %s", item2->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot add a bonus on %s because is soul bounded."), item2->GetName());
					return false;
				}
#endif

				if (item2->isLocked() || item2->GetSocket(CRYSTAL_STATUS_SOCKET_SLOT) != 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "1523 %s", item2->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot add a bonus on %s because is activated."), item2->GetName());
					return false;
				}

				item->Lock(true);
				item2->Lock(true);

				BYTE addBnsType = APPLY_NONE;

				for (int i = 0; i < ITEM_APPLY_MAX_NUM; i++)
				{
					addBnsType = item->GetApplyType(i);
					if (addBnsType != APPLY_NONE)
					{
						break;
					}
				}

				const auto it = m_mapCrystalBonuses.find(addBnsType);
				if (it == m_mapCrystalBonuses.end() || it->second.empty())
				{
					ChatPacket(CHAT_TYPE_INFO, "1524 %s %d", item->GetName(), 4);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with %s, please contact the staff. Err code: %d."), item->GetName(), 4);

					item->Lock(false);
					item2->Lock(false);

					return false;
				}

				const auto crystalBnsType = item2->GetSocket(CRYSTAL_BONUS_TYPE_SOCKET_SLOT);
				const auto sharpBnsType = item->GetValue(CRYSTAL_ADD_BONUS_TYPE_VALUE_SLOT);

				if (crystalBnsType != CRYSTAL_BONUS_TYPE_NONE)
				{
					if (crystalBnsType != CRYSTAL_BONUS_TYPE_PVM && crystalBnsType != CRYSTAL_BONUS_TYPE_PVP)
					{
						ChatPacket(CHAT_TYPE_INFO, "1524 %s %d", item->GetName(), 5);
						// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with %s, please contact the staff. Err code: %d."), item->GetName(), 5);

						item->Lock(false);
						item2->Lock(false);

						return false;
					}

					if (crystalBnsType != sharpBnsType)
					{
						ChatPacket(CHAT_TYPE_INFO, "1525 %s %s", crystalBnsType == CRYSTAL_BONUS_TYPE_PVM ? "PvM" : "PvP", item2->GetName());
						// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can add just %s bonuses on %s."), crystalBnsType == CRYSTAL_BONUS_TYPE_PVM ? "PvM" : "PvP", item2->GetName());

						item->Lock(false);
						item2->Lock(false);

						return false;
					}
				}

				const auto attrCount = item2->GetAttributeCount();
				if (attrCount == ITEM_ATTRIBUTE_NORM_NUM)
				{
					ChatPacket(CHAT_TYPE_INFO, "1526 %s %d", item2->GetName(), ITEM_ATTRIBUTE_NORM_NUM);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s cannot have more than %d bonuses."), item2->GetName(), ITEM_ATTRIBUTE_NORM_NUM);

					item->Lock(false);
					item2->Lock(false);

					return false;
				}

				for (int i = 0; i < ITEM_ATTRIBUTE_NORM_NUM; i++)
				{
					if (item2->GetAttributeType(i) == addBnsType)
					{
						ChatPacket(CHAT_TYPE_INFO, "1527 %s", item->GetName());
						// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s already have this shard added."), item->GetName());

						item->Lock(false);
						item2->Lock(false);

						return false;
					}
				}

				const auto slotValue = item->GetValue(CRYSTAL_ADD_BONUS_SLOT_VALUE_SLOT);
				if (slotValue < 1 || slotValue > ITEM_ATTRIBUTE_NORM_NUM)
				{
					ChatPacket(CHAT_TYPE_INFO, "1524 %s %d", item->GetName(), 6);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with %s, please contact the staff. Err code: %d."), item->GetName(), 6);

					item->Lock(false);
					item2->Lock(false);

					return false;
				}

				if (slotValue > item2->GetValue(CRYSTAL_ADD_BONUS_SLOT_VALUE_SLOT))
				{
					ChatPacket(CHAT_TYPE_INFO, "1528 %s %s %s", item->GetName(), item2->GetName(), item2->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s cannot be added on %s. %s must be upgraded first."), item->GetName(), item2->GetName(), item2->GetName());

					item->Lock(false);
					item2->Lock(false);

					return false;
				}

				const char* cpszItemName = item->GetName();

				item->Lock(false);
				item->SetCount(item->GetCount() - 1);

				if (attrCount == 0)
				{
					item2->SetSocket(CRYSTAL_BONUS_TYPE_SOCKET_SLOT, sharpBnsType);
				}

				item2->SetForceAttribute(slotValue - 1, it->first, it->second[0]);

				ChatPacket(CHAT_TYPE_INFO, "1529 %s %s", cpszItemName, item2->GetName());
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s was succesfully added on %s."), cpszItemName, item2->GetName());

				item2->Lock(false);

				return true;
#else
				return false;
#endif
			}
			case USE_CHANGE_ATTR_CRYSTAL:
			{
#ifdef USE_CRYSTAL_SYSTEM
				if (!IsValidItemPosition(DestCell))
				{
					return false;
				}

				LPITEM item2 = GetItem(DestCell);
				if (!item2)
				{
					return false;
				}

				if (item2->IsExchanging()/* || item2->IsEquipped()*/)
				{
					return false;
				}

				if (item2->GetType() != ITEM_CRYSTAL)
				{
					ChatPacket(CHAT_TYPE_INFO, "1521 %s", item->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s can be used just on Crystals."), item->GetName());
					return false;
				}

#ifdef ENABLE_SOULBIND_SYSTEM
				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, "1530 %s", item2->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot change the bonuses of %s because is soul bounded."), item2->GetName());
					return false;
				}
#endif

				if (item2->isLocked() || item2->GetSocket(CRYSTAL_STATUS_SOCKET_SLOT) != 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "1531 %s", item2->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot change the bonuses of %s because is activated."), item2->GetName());
					return false;
				}

				item->Lock(true);
				item2->Lock(true);

				if (item2->GetAttributeCount() == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "1532 %s", item2->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s doesn't have any bonus."), item2->GetName());

					item->Lock(false);
					item2->Lock(false);

					return false;
				}

				bool bChanged = false;

				for (int i = 0; i < ITEM_ATTRIBUTE_NORM_NUM; i++)
				{
					const auto bnsType = item2->GetAttributeType(i);
					if (bnsType == APPLY_NONE)
					{
						continue;
					}

					const auto it = m_mapCrystalBonuses.find(bnsType);
					if (it != m_mapCrystalBonuses.end() && !it->second.empty())
					{
						const auto itSize = it->second.size();
						item2->SetForceAttribute(i, it->first, it->second[number(0, itSize - 1)]);

						if (!bChanged)
						{
							bChanged = true;
						}
					}
				}

				if (!bChanged)
				{
					ChatPacket(CHAT_TYPE_INFO, "1524 %s %d", item->GetName(), 7);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with %s, please contact the staff. Err code: %d."), item->GetName(), 7);

					item->Lock(false);
					item2->Lock(false);

					return false;
				}

				item->Lock(false);
				item->SetCount(item->GetCount() - 1);

				ChatPacket(CHAT_TYPE_INFO, "1533 %s", item2->GetName());
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The bonuses of %s has succesfully been changed."), item2->GetName());

				item2->Lock(false);

				return true;
#else
				return false;
#endif
			}
			case USE_RECHARGE_CRYSTAL:
			{
#ifdef USE_CRYSTAL_SYSTEM
				if (!IsValidItemPosition(DestCell))
				{
					return false;
				}

				LPITEM item2 = GetItem(DestCell);
				if (!item2)
				{
					return false;
				}

				if (item2->GetType() != ITEM_CRYSTAL)
				{
					ChatPacket(CHAT_TYPE_INFO, "1521 %s", item->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s can be used just on Crystals."), item->GetName());
					return false;
				}

				if (item2->IsExchanging()/* || item2->IsEquipped()*/)
				{
					return false;
				}

				if (item2->isLocked() || item2->GetSocket(CRYSTAL_STATUS_SOCKET_SLOT) != 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "1547 %s %s", item->GetName(), item2->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot use %s on %s because is activated."), item->GetName(), item2->GetName());
					return false;
				}

				long maxRechargeTime = 0;

				if (item->GetSocket(CRYSTAL_LEFT_TIME_SOCKET_SLOT) == 0)
				{
					maxRechargeTime = item->GetValue(CRYSTAL_ADD_TIME_SLOT_VALUE_SLOT);
				}
				else
				{
					maxRechargeTime = item->GetSocket(CRYSTAL_LEFT_TIME_SOCKET_SLOT);
				}

				if (maxRechargeTime == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "1548 %s", item->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with %s, please contact the staff."), item->GetName());
					return false;
				}

				long maxCrystalTime = 0;

				const auto* proto = item2->GetProto();
				if (!proto)
				{
					ChatPacket(CHAT_TYPE_INFO, "1549");
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something went wrong, please contact the staff."));
					return false;
				}

				for (const auto& limit : proto->aLimits)
				{
					if (limit.bType == LIMIT_TIMER_BASED_ON_WEAR && limit.lValue > 0)
					{
						maxCrystalTime = limit.lValue;
						break;
					}
				}

				if (maxCrystalTime == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "1548 %s", item->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with %s, please contact the staff."), item->GetName());
					return false;
				}

				const auto leftTime = item2->GetSocket(ITEM_SOCKET_REMAIN_SEC);
				if (leftTime == maxCrystalTime)
				{
					ChatPacket(CHAT_TYPE_INFO, "1550 %s", item2->GetName());
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The time limit of %s is already full."), item2->GetName());
					return false;
				}

				if (item->GetSocket(CRYSTAL_LEFT_TIME_SOCKET_SLOT) == 0)
				{
					item->SetSocket(CRYSTAL_LEFT_TIME_SOCKET_SLOT, maxRechargeTime);
				}

				item->Lock(true);
				item2->Lock(true);

				const auto usedTime = maxCrystalTime - leftTime;

				if (usedTime >= maxRechargeTime)
				{
					item->Lock(false);
					item->SetCount(item->GetCount() - 1);

					item2->SetSocket(ITEM_SOCKET_REMAIN_SEC, leftTime + maxRechargeTime);
				}
				else
				{
					item->Lock(false);
					item->SetSocket(CRYSTAL_LEFT_TIME_SOCKET_SLOT, maxRechargeTime - usedTime);

					item2->SetSocket(ITEM_SOCKET_REMAIN_SEC, leftTime + usedTime);
				}

				ChatPacket(CHAT_TYPE_INFO, "1551 %s", item2->GetName());
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The time limit of %s was increased."), item2->GetName());

				item2->Lock(false);

				return true;
#else
				return false;
#endif
			}
			default:
			{
				break;
			}
		}
// #endif

		if (!CanUseItem(GetMapIndex(), item->GetVnum()))
		{
			ChatPacket(CHAT_TYPE_INFO, "You can't use this");
			return false;
		}

		if (item->GetVnum() > 50800 && item->GetVnum() <= 50820)
		{
			if (test_server)
				sys_log(0, "ADD addtional effect : vnum(%d) subtype(%d)", item->GetOriginalVnum(), item->GetSubType());


			int affect_type = AFFECT_EXP_BONUS_EURO_FREE;
			int apply_type = aApplyInfo[item->GetValue(0)].bPointType;
			int apply_value = item->GetValue(2);
			int apply_duration = item->GetValue(1);

			switch (item->GetSubType())
			{
			case USE_ABILITY_UP:
				if (FindAffect(affect_type, apply_type))
				{
					ChatPacket(CHAT_TYPE_INFO, "175");
					return false;
				}

				{
					switch (item->GetValue(0))
					{
					case APPLY_MOV_SPEED:
						AddAffect(affect_type, apply_type, apply_value, AFF_MOV_SPEED_POTION, apply_duration, 0, true, true);
						break;

					case APPLY_ATT_SPEED:
						AddAffect(affect_type, apply_type, apply_value, AFF_ATT_SPEED_POTION, apply_duration, 0, true, true);
						break;

					case APPLY_STR:
					case APPLY_DEX:
					case APPLY_CON:
					case APPLY_INT:
					case APPLY_CAST_SPEED:
					case APPLY_RESIST_MAGIC:
					case APPLY_ATT_GRADE_BONUS:
					case APPLY_DEF_GRADE_BONUS:
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
						break;
					}
				}

				if (GetDungeon())
					GetDungeon()->UsePotion(this);

				if (GetWarMap())
					GetWarMap()->UsePotion(this, item);

				item->SetCount(item->GetCount() - 1);
				break;

			case USE_AFFECT:
			{
				if (FindAffect(AFFECT_EXP_BONUS_EURO_FREE, aApplyInfo[item->GetValue(1)].bPointType))
				{
					ChatPacket(CHAT_TYPE_INFO, "175");
				}
				else
				{
					// PC_BANG_ITEM_ADD
					if (item->IsPCBangItem() == true)
					{
						// PCÂ¹Ã¦???Ã¶ ?Â¼?Â©??Â¼Â­ ?Â³Â¸Â®
						if (CPCBangManager::instance().IsPCBangIP(GetDesc()->GetHostName()) == false)
						{
							// PCÂ¹Ã¦?? Â¾?Â´?!
							ChatPacket(CHAT_TYPE_INFO, "176");
							return false;
						}
					}
					// END_PC_BANG_ITEM_ADD

					AddAffect(AFFECT_EXP_BONUS_EURO_FREE, aApplyInfo[item->GetValue(1)].bPointType, item->GetValue(2), 0, item->GetValue(3), 0, false, true);
					item->SetCount(item->GetCount() - 1);
				}
			}
			break;

			case USE_POTION_NODELAY:
			{
#ifdef ENABLE_SUNG_MAHI_TOWER
				if (!IS_SUNG_MAHI_ENABLE_ITEM(GetMapIndex(), item->GetVnum()))
				{
					ChatPacket(CHAT_TYPE_INFO, "You cannot use this type of potion here.");
					return false;
				}
#endif


				if (!CanUseItem(GetMapIndex(), item->GetVnum()))
				{
					ChatPacket(CHAT_TYPE_INFO, "You can't use this");
					return false;
				}

				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
					{
						ChatPacket(CHAT_TYPE_INFO, "177");
						return false;
					}

					switch (item->GetVnum())
					{
					case 70020:
					case 71018:
					case 71019:
					case 71020:
						if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
						{
							if (m_nPotionLimit <= 0)
							{
								ChatPacket(CHAT_TYPE_INFO, "178");
								return false;
							}
						}
						break;

					default:
						ChatPacket(CHAT_TYPE_INFO, "177");
						return false;
						break;
					}
				}
#ifdef ENABLE_NEWSTUFF
				else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(item->GetVnum()))
				{
					ChatPacket(CHAT_TYPE_INFO, "134");
					return false;
				}
#endif

				bool used = false;

				if (item->GetValue(0) != 0) // HP ?Ã½Â´Ã«Â°Âª ?Â¸ÂºÂ¹
				{
					if (GetHP() < GetMaxHP())
					{
						PointChange(POINT_HP, item->GetValue(0) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
						EffectPacket(SE_HPUP_RED);
						used = TRUE;
					}
				}

				if (item->GetValue(1) != 0)	// SP ?Ã½Â´Ã«Â°Âª ?Â¸ÂºÂ¹
				{
					if (GetSP() < GetMaxSP())
					{
						PointChange(POINT_SP, item->GetValue(1) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
						EffectPacket(SE_SPUP_BLUE);
						used = TRUE;
					}
				}

				if (item->GetValue(3) != 0) // HP % ?Â¸ÂºÂ¹
				{
					if (GetHP() < GetMaxHP())
					{
						PointChange(POINT_HP, item->GetValue(3) * GetMaxHP() / 100);
						EffectPacket(SE_HPUP_RED);
						used = TRUE;
					}
				}

				if (item->GetValue(4) != 0) // SP % ?Â¸ÂºÂ¹
				{
					if (GetSP() < GetMaxSP())
					{
						PointChange(POINT_SP, item->GetValue(4) * GetMaxSP() / 100);
						EffectPacket(SE_SPUP_BLUE);
						used = TRUE;
					}
				}

				if (used)
				{
					if (item->GetVnum() == 50085 || item->GetVnum() == 50086)
					{
						if (test_server)
							ChatPacket(CHAT_TYPE_INFO, "179");
						SetUseSeedOrMoonBottleTime();
					}
					if (GetDungeon())
						GetDungeon()->UsePotion(this);

					if (GetWarMap())
						GetWarMap()->UsePotion(this, item);

					m_nPotionLimit--;

					//RESTRICT_USE_SEED_OR_MOONBOTTLE
					item->SetCount(item->GetCount() - 1);
					//END_RESTRICT_USE_SEED_OR_MOONBOTTLE
				}
			}
			break;
			}

			return true;
		}


		if (item->GetVnum() >= 27863 && item->GetVnum() <= 27883)
		{

			if (!CanUseItem(GetMapIndex(), item->GetVnum()))
			{
				ChatPacket(CHAT_TYPE_INFO, "You can't use this");
				return false;
			}

			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				ChatPacket(CHAT_TYPE_INFO, "134");
				return false;
			}
#ifdef ENABLE_NEWSTUFF
			else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(item->GetVnum()))
			{
				ChatPacket(CHAT_TYPE_INFO, "134");
				return false;
			}
#endif
		}

		if (test_server)
		{
			sys_log(0, "USE_ITEM %s Type %d SubType %d vnum %d", item->GetName(), item->GetType(), item->GetSubType(), item->GetOriginalVnum());
		}

		switch (item->GetSubType())
		{
		case USE_TIME_CHARGE_PER:
		{
			LPITEM pDestItem = GetItem(DestCell);
			if (NULL == pDestItem)
			{
				return false;
			}
			// Â¿Ã¬Â¼Â± Â¿Ã«?Â¥Â¼Â®Â¿Â¡ Â°Ã¼??Â¼Â­Â¸Â¸ ??ÂµÂµÂ·? ??Â´?.
			if (pDestItem->IsDragonSoul())
			{
				int ret;
				char buf[128];
				if (item->GetVnum() == DRAGON_HEART_VNUM)
				{
					ret = pDestItem->GiveMoreTime_Per((float)item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
				}
				else
				{
					ret = pDestItem->GiveMoreTime_Per((float)item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
				}
				if (ret > 0)
				{
					if (item->GetVnum() == DRAGON_HEART_VNUM)
					{
						sprintf(buf, "Inc %ds by item{VN:%d SOC%d:%ld}", ret, item->GetVnum(), ITEM_SOCKET_CHARGING_AMOUNT_IDX, item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
					}
					else
					{
						sprintf(buf, "Inc %ds by item{VN:%d VAL%d:%ld}", ret, item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					}

					ChatPacket(CHAT_TYPE_INFO, "180 %d", ret);
					item->SetCount(item->GetCount() - 1);
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_SUCCESS", buf);
					return true;
				}
				else
				{
					if (item->GetVnum() == DRAGON_HEART_VNUM)
					{
						sprintf(buf, "No change by item{VN:%d SOC%d:%ld}", item->GetVnum(), ITEM_SOCKET_CHARGING_AMOUNT_IDX, item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
					}
					else
					{
						sprintf(buf, "No change by item{VN:%d VAL%d:%ld}", item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					}

					ChatPacket(CHAT_TYPE_INFO, "181");
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_FAILED", buf);
					return false;
				}
			}
			else
				return false;
		}
		break;
		case USE_TIME_CHARGE_FIX:
		{
			LPITEM pDestItem = GetItem(DestCell);
			if (NULL == pDestItem)
			{
				return false;
			}
			// Â¿Ã¬Â¼Â± Â¿Ã«?Â¥Â¼Â®Â¿Â¡ Â°Ã¼??Â¼Â­Â¸Â¸ ??ÂµÂµÂ·? ??Â´?.
			if (pDestItem->IsDragonSoul())
			{
				int ret = pDestItem->GiveMoreTime_Fix(item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
				char buf[128];
				if (ret)
				{
					ChatPacket(CHAT_TYPE_INFO, "180 %d", ret);
					sprintf(buf, "Increase %ds by item{VN:%d VAL%d:%ld}", ret, item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_SUCCESS", buf);
					item->SetCount(item->GetCount() - 1);
					return true;
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, "181");
					sprintf(buf, "No change by item{VN:%d VAL%d:%ld}", item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_FAILED", buf);
					return false;
				}
			}
			else
				return false;
		}
		break;
		case USE_SPECIAL:
#ifdef USE_DROP_AFFECT_BONUSES
			if (ItIsDropAffectBonus(item->GetVnum()))
			{
				if (!IsLoadedAffect())
				{
					ChatPacket(CHAT_TYPE_INFO, "1401");
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Try again in few seconds."));
					return false;
				}

				const auto* proto = item->GetProto();
				if (!proto)
				{
					ChatPacket(CHAT_TYPE_INFO, "1564 %d", 1);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this glove, please contact the staff (err code: %d)."), 1);
					return false;
				}

				const DWORD affectType = static_cast<DWORD>(item->GetValue(DOUBLE_DROP_BONUSES_AFFECT_VALUE_SLOT));
				if (!IS_DROP_AFFECT(affectType))
				{
					ChatPacket(CHAT_TYPE_INFO, "1564 %d", 2);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this glove, please contact the staff (err code: %d)."), 2);
					return false;
				}

				const long timeLimit = item->GetValue(DOUBLE_DROP_BONUSES_TIME_VALUE_SLOT);
				if (timeLimit < 1)
				{
					ChatPacket(CHAT_TYPE_INFO, "1564 %d", 3);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this glove, please contact the staff (err code: %d)."), 3);
					return false;
				}

				if (item->GetWindow() != INVENTORY)
				{
					ChatPacket(CHAT_TYPE_INFO, "1564 %d", 4);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this glove, please contact the staff (err code: %d)."), 4);
					return false;
				}

				BYTE affectApply = APPLY_NONE;
				long affectValue = 0;

				for (const auto& apply : proto->aApplies)
				{
					if (apply.bType != APPLY_NONE && apply.lValue != 0)
					{
						affectApply = apply.bType;
						affectValue = apply.lValue;
						break;
					}
				}

				if (affectApply == APPLY_NONE || affectValue == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "1564 %d", 5);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this glove, please contact the staff (err code: %d)."), 5);
					return false;
				}

				CAffect* affect = FindAffect(affectType, aApplyInfo[affectApply].bPointType);
				if (affect && affect->lDuration + timeLimit > 5184000)
				{
					ChatPacket(CHAT_TYPE_INFO, "1565");
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot have more than 60 days left time on this glove affect."));
					return false;
				}

				item->SetCount(item->GetCount() - 1);

				AddAffect(affectType, aApplyInfo[affectApply].bPointType, affectValue, 0, affect ? affect->lDuration + timeLimit : timeLimit, 0, true);

				ChatPacket(CHAT_TYPE_INFO, "1566");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The affect of the glove has been added."));

				return true;
			}
#endif

#ifdef USE_PREMIUM_AFFECT
			if (IsPremiumUserItem(item->GetVnum()))
			{
				if (!IsLoadedAffect())
				{
					ChatPacket(CHAT_TYPE_INFO, "1401");
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Try again in few seconds."));
					return false;
				}

				const auto* proto = item->GetProto();
				if (!proto)
				{
					ChatPacket(CHAT_TYPE_INFO, "1567 %d", 1);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this item, please contact the staff (err code: %d)."), 1);
					return false;
				}

				const DWORD affectType = static_cast<DWORD>(item->GetValue(PREMIUM_USER_AFFECT_VALUE_SLOT));
				if (!IS_PREMIUM_USER_AFFECT(affectType))
				{
					ChatPacket(CHAT_TYPE_INFO, "1567 %d", 2);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this item, please contact the staff (err code: %d)."), 2);
					return false;
				}

				const long timeLimit = item->GetValue(PREMIUM_USER_TIME_VALUE_SLOT);
				if (timeLimit < 1)
				{
					ChatPacket(CHAT_TYPE_INFO, "1567 %d", 3);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this item, please contact the staff (err code: %d)."), 3);
					return false;
				}

				if (item->GetWindow() != INVENTORY)
				{
					ChatPacket(CHAT_TYPE_INFO, "1567 %d", 4);
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this item, please contact the staff (err code: %d)."), 4);
					return false;
				}

				CAffect* affect = FindAffect(affectType, POINT_PREMIUM_USER);
				if (affect && affect->lDuration + timeLimit > 5184000)
				{
					ChatPacket(CHAT_TYPE_INFO, "1568");
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot have more than 60 days left time of premium user."));
					return false;
				}

				item->SetCount(item->GetCount() - 1);

				AddAffect(affectType, POINT_PREMIUM_USER, 1, 0, affect ? affect->lDuration + timeLimit : timeLimit, 0, true);

#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
#ifdef USE_DAY_CHANGE_MGR
				const int iToday = CDayChangeMgr::Instance().GetTodayNum();
#else
				const int iToday = GetTodayNum();
#endif
				const int iLastDay = GetAutoHuntLastDay();

				if (iLastDay == 0
					 || iToday != iLastDay)
				{
					SetAutoHuntLastDay(iToday);
					SetAutoHuntUsedTime(0);
				}

				ChatPacket(CHAT_TYPE_COMMAND, "autohunt_stat %d", GetAutoHuntUsedTime());
#endif

				ChatPacket(CHAT_TYPE_INFO, "1569");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The premium user affect was activated."));

				return true;
			}
#endif

			switch (item->GetVnum())
			{
			case 70071:
			{
				LPITEM item2;

				if (NULL == item2)
					return false;

				if (item2->IsEquipped())
				{
					return false;
				}
				if (item2->GetType() != ITEM_BOOSTER)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can only use this item on boosters."));
					return false;
				}

				// Adjust check if you are weird
				if (item2->GetLimitType(0) < LIMIT_REAL_TIME || item2->GetLimitType(0) > LIMIT_TIMER_BASED_ON_WEAR)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can only use this item on boosters with time limit."));
					return false;
				}

				auto existing_duration = item2->GetSocket(0);
				auto limit_time = item2->GetLimitValue(0);
				auto additional_duration = item2->GetLimitValue(0) / 4;
				auto new_duration = 0;

				if (existing_duration < get_global_time() - additional_duration)
					new_duration = get_global_time() + additional_duration;
				else
					new_duration = existing_duration + additional_duration;

				if (existing_duration == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "You don't have time that has run out.");
					return false;
				}

				if ((new_duration - time(0) >= limit_time))
				{
					ChatPacket(CHAT_TYPE_INFO, "Your time has been fully restored.");
					item2->SetSocket(0, get_global_time() + limit_time);
					item->SetCount(item->GetCount() - 1);
					return false;
				}

				item2->SetSocket(0, new_duration);

				item->SetCount(item->GetCount() - 1);
			}
			break;
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
			case 70002:
			{
				if (FindAffect(AFFECT_AUTO_PICK_UP))
				{
					ChatPacket(CHAT_TYPE_INFO, "1099");
					return false;
				}
				const int affectime = 60 * 60 * 24 * 30; // 30 DAY
				AddAffect(AFFECT_AUTO_PICK_UP, POINT_NONE, 0, AFF_NONE, affectime, 0, false, true);
				item->SetCount(item->GetCount() - 1);
				ChatPacket(CHAT_TYPE_INFO, "1100 %d", 30);
				return true;
			}
			break;
			case 53290:
			{
				if (FindAffect(AFFECT_AUTO_PICK_UP))
				{
					ChatPacket(CHAT_TYPE_INFO, "1099");
					return false;
				}
				const int affectime = 60 * 60 * 24 * 7; // 30 DAY
				AddAffect(AFFECT_AUTO_PICK_UP, POINT_NONE, 0, AFF_NONE, affectime, 0, false, true);
				item->SetCount(item->GetCount() - 1);
				ChatPacket(CHAT_TYPE_INFO, "1100 %d", 7);
				return true;
			}
			break;
#endif
#ifdef ENABLE_SOULBIND_SYSTEM
			case 50263:
			{
				LPITEM item2;
				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsEquipped() || item2->IsExchanging())
					return false;

				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, "182");
					return false;
				}

				if (item2->GetType() != ITEM_WEAPON && item2->GetType() != ITEM_ARMOR && item2->GetType() != ITEM_COSTUME)
					return false;

				item2->SetSealBind();
				ChatPacket(CHAT_TYPE_INFO, "183");
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "SET_SEALBIND_SUCCESS", buf);
				}
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50264:
			{
				LPITEM item2;
				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->isLocked() || item2->IsEquipped() || item2->GetSealBindTime() >= 0)
					return false;

				long duration = 72 * 60 * 60;
				item2->SetSealBind(time(0) + duration);
				item2->StartUnSealBindTimerExpireEvent();
				ChatPacket(CHAT_TYPE_INFO, "184");
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "REMOVE_SEALBIND_TIME_BEGIN", buf);
				}
				item->SetCount(item->GetCount() - 1);
			}
			break;
#endif
#ifdef ENABLE_ZODIAC_MISSION
			case 33031: // 12 Bead 
			{
				int bCount = GetQuestFlag("Bead.Count");
				SetQuestFlag("Bead.Count", GetQuestFlag("Bead.Count") + 12);
				bCount = GetQuestFlag("Bead.Count"); // Update
				ChatPacket(CHAT_TYPE_COMMAND, "SetBeadCount %d", bCount);
				item->SetCount(item->GetCount() - 1);
				ChatPacket(CHAT_TYPE_INFO, ("%d Adet Animasfer eklendi. "), 12);
			}
			break;
			case 33032: // 24 Bead 
			{
				int bCount = GetQuestFlag("Bead.Count");
				SetQuestFlag("Bead.Count", GetQuestFlag("Bead.Count") + 24);
				bCount = GetQuestFlag("Bead.Count"); // Update
				ChatPacket(CHAT_TYPE_COMMAND, "SetBeadCount %d", bCount);
				item->SetCount(item->GetCount() - 1);
				ChatPacket(CHAT_TYPE_INFO, ("%d Adet Animasfer eklendi. "), 24);
			}
			break;
			case 33033: // 36 Bead 
			{
				int bCount = GetQuestFlag("Bead.Count");
				SetQuestFlag("Bead.Count", GetQuestFlag("Bead.Count") + 36);
				bCount = GetQuestFlag("Bead.Count"); // Update
				ChatPacket(CHAT_TYPE_COMMAND, "SetBeadCount %d", bCount);
				item->SetCount(item->GetCount() - 1);
				ChatPacket(CHAT_TYPE_INFO, ("%d Adet Animasfer eklendi. "), 36);
			}
			break;

			case 33034: // 48 Bead 
			{
				int bCount = GetQuestFlag("Bead.Count");
				SetQuestFlag("Bead.Count", GetQuestFlag("Bead.Count") + 12);
				bCount = GetQuestFlag("Bead.Count"); // Update
				ChatPacket(CHAT_TYPE_COMMAND, "SetBeadCount %d", bCount);
				item->SetCount(item->GetCount() - 1);
				ChatPacket(CHAT_TYPE_INFO, ("%d Adet Animasfer eklendi. "), 12);
			}
			break;

			case 33035: // 60 Bead 
			{
				int bCount = GetQuestFlag("Bead.Count");
				SetQuestFlag("Bead.Count", GetQuestFlag("Bead.Count") + 60);
				bCount = GetQuestFlag("Bead.Count"); // Update
				ChatPacket(CHAT_TYPE_COMMAND, "SetBeadCount %d", bCount);
				item->SetCount(item->GetCount() - 1);
				ChatPacket(CHAT_TYPE_INFO, ("%d Adet Animasfer eklendi. "), 60);
			}
			break;
#endif
			//?Â©Â¸Â®Â½ÂºÂ¸Â¶Â½Âº Â¶Ãµ??
			case ITEM_NOG_POCKET:
			{
				/*
				Â¶Ãµ??Â´?Â·??Â¡ : item_proto value ??Â¹?
					??ÂµÂ¿Â¼?ÂµÂµ  value 1
					Â°Ã¸Â°?Â·?	  value 2
					Â°Ã¦?Ã¨?Â¡	value 3
					?Ã¶Â¼?Â½?Â°Â£  value 0 (Â´??Â§ ??)

				*/
				if (FindAffect(AFFECT_NOG_ABILITY))
				{
					ChatPacket(CHAT_TYPE_INFO, "175");
					return false;
				}
				long time = item->GetValue(0);
				long moveSpeedPer = item->GetValue(1);
				long attPer = item->GetValue(2);
				long expPer = item->GetValue(3);
				AddAffect(AFFECT_NOG_ABILITY, POINT_MOV_SPEED, moveSpeedPer, AFF_MOV_SPEED_POTION, time, 0, true, true);
				AddAffect(AFFECT_NOG_ABILITY, POINT_MALL_ATTBONUS, attPer, AFF_NONE, time, 0, true, true);
				AddAffect(AFFECT_NOG_ABILITY, POINT_MALL_EXPBONUS, expPer, AFF_NONE, time, 0, true, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			//Â¶Ã³Â¸Â¶Â´?Â¿Ã« Â»Ã§??
			case ITEM_RAMADAN_CANDY:
			{
				/*
				Â»Ã§??Â´?Â·??Â¡ : item_proto value ??Â¹?
					??ÂµÂ¿Â¼?ÂµÂµ  value 1
					Â°Ã¸Â°?Â·?	  value 2
					Â°Ã¦?Ã¨?Â¡	value 3
					?Ã¶Â¼?Â½?Â°Â£  value 0 (Â´??Â§ ??)

				*/
				// @fixme147 BEGIN
				if (FindAffect(AFFECT_RAMADAN_ABILITY))
				{
					ChatPacket(CHAT_TYPE_INFO, "175");
					return false;
				}
				// @fixme147 END
				long time = item->GetValue(0);
				long moveSpeedPer = item->GetValue(1);
				long attPer = item->GetValue(2);
				long expPer = item->GetValue(3);
				AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MOV_SPEED, moveSpeedPer, AFF_MOV_SPEED_POTION, time, 0, true, true);
				AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MALL_ATTBONUS, attPer, AFF_NONE, time, 0, true, true);
				AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MALL_EXPBONUS, expPer, AFF_NONE, time, 0, true, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;
			case ITEM_MARRIAGE_RING:
			{
				marriage::TMarriage* pMarriage = marriage::CManager::instance().Get(GetPlayerID());
				if (pMarriage)
				{
					if (pMarriage->ch1 != NULL)
					{
						if (CArenaManager::instance().IsArenaMap(pMarriage->ch1->GetMapIndex()) == true)
						{
							ChatPacket(CHAT_TYPE_INFO, "134");
							break;
						}
#if defined(WJ_COMBAT_ZONE)	
						if (CCombatZoneManager::Instance().IsCombatZoneMap(pMarriage->ch1->GetMapIndex()))
						{
							ChatPacket(CHAT_TYPE_INFO, "185");
							break;
						}
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
						if (CTournamentPvP::instance().IsTournamentMap(pMarriage->ch1, TOURNAMENT_BLOCK_RING_MARRIAGE))
						{
							return false;
						}
#endif
					}

					if (pMarriage->ch2 != NULL)
					{
						if (CArenaManager::instance().IsArenaMap(pMarriage->ch2->GetMapIndex()) == true)
						{
							ChatPacket(CHAT_TYPE_INFO, "134");
							break;
						}
#if defined(WJ_COMBAT_ZONE)	
						if (CCombatZoneManager::Instance().IsCombatZoneMap(pMarriage->ch2->GetMapIndex()))
						{
							ChatPacket(CHAT_TYPE_INFO, "185");
							break;
						}
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
						if (CTournamentPvP::instance().IsTournamentMap(pMarriage->ch2, TOURNAMENT_BLOCK_RING_MARRIAGE))
						{
							return false;
						}
#endif
					}

					int consumeSP = CalculateConsumeSP(this);

					if (consumeSP < 0)
						return false;

					PointChange(POINT_SP, -consumeSP, false);

					WarpToPID(pMarriage->GetOther(GetPlayerID()));
				}
				else
					ChatPacket(CHAT_TYPE_INFO, "186");
			}
			break;

			//Â±Ã¢?Â¸ Â¿Ã«Â±Ã¢?? Â¸??Ã¤
			case UNIQUE_ITEM_CAPE_OF_COURAGE:
				//Â¶Ã³Â¸Â¶Â´? ÂºÂ¸Â»Ã³Â¿Ã« Â¿Ã«Â±Ã¢?? Â¸??Ã¤
			case 70057:
			case REWARD_BOX_UNIQUE_ITEM_CAPE_OF_COURAGE:
				AggregateMonster();
				// item->SetCount(item->GetCount()-1);
				break;
#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_ULTIMATE
			case 51998:
				if (ikashop::GetManager().UsePrivateShopKey(this, item))
					item->SetCount(item->GetCount() - 1);
				break;

			case 51999:
				if (ikashop::GetManager().UseSearchShopPremium(this, item))
					item->SetCount(item->GetCount() - 1);
				break;
#endif
#endif

			case UNIQUE_ITEM_WHITE_FLAG:
				ForgetMyAttacker();
				item->SetCount(item->GetCount() - 1);
				break;

			case UNIQUE_ITEM_TREASURE_BOX:
				break;

			case 30093:
			case 30094:
			case 30095:
			case 30096:
				// ÂºÂ¹??Â¸?Â´?
			{
				const int MAX_BAG_INFO = 26;
				static struct LuckyBagInfo
				{
					DWORD count;
					int prob;
					DWORD vnum;
				} b1[MAX_BAG_INFO] =
				{
					{ 1000,	302,	1 },
					{ 10,	150,	27002 },
					{ 10,	75,	27003 },
					{ 10,	100,	27005 },
					{ 10,	50,	27006 },
					{ 10,	80,	27001 },
					{ 10,	50,	27002 },
					{ 10,	80,	27004 },
					{ 10,	50,	27005 },
					{ 1,	10,	50300 },
					{ 1,	6,	92 },
					{ 1,	2,	132 },
					{ 1,	6,	1052 },
					{ 1,	2,	1092 },
					{ 1,	6,	2082 },
					{ 1,	2,	2122 },
					{ 1,	6,	3082 },
					{ 1,	2,	3122 },
					{ 1,	6,	5052 },
					{ 1,	2,	5082 },
					{ 1,	6,	7082 },
					{ 1,	2,	7122 },
					{ 1,	1,	11282 },
					{ 1,	1,	11482 },
					{ 1,	1,	11682 },
					{ 1,	1,	11882 },
				};

				LuckyBagInfo* bi = NULL;
				bi = b1;

				int pct = number(1, 1000);

				int i;
				for (i = 0; i < MAX_BAG_INFO; i++)
				{
					if (pct <= bi[i].prob)
						break;
					pct -= bi[i].prob;
				}
				if (i >= MAX_BAG_INFO)
					return false;

				if (bi[i].vnum == 50300)
				{
					// Â½Âº?Â³Â¼Ã¶Â·?Â¼Â­Â´? ?Â¯Â¼Ã¶??Â°? ??Â´?.
					GiveRandomSkillBook();
				}
				else if (bi[i].vnum == 1)
				{
					PointChange(POINT_GOLD, 1000, true);
				}
				else
				{
					AutoGiveItem(bi[i].vnum, bi[i].count);
				}
				ITEM_MANAGER::instance().RemoveItem(item);
			}
			break;

#ifdef BATTLEPASS_WORLDARD
			case 93101:
			case 93102:
			case 93103:
			case 93104:
			case 93105:
			case 93106:
			case 93107:
			case 93108:
			case 93109:
			case 93110:
			case 93111:
			case 93112:
			{
				int iSeconds = GetSecondsTillNextMonth();

				if (iSeconds < 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "Error BattlePass 01");
					return false;
				}

				if (FindAffect(AFFECT_BATTLE_PASS))
				{
					ChatPacket(CHAT_TYPE_INFO, "Usted ya tiene activado el battlepass");
					return false;
				}
				else
				{

					time_t iTime;
					time(&iTime);
					struct tm endTime = *localtime(&iTime);
					int iCurrentMonth = endTime.tm_mon;

					if (iCurrentMonth + 1 != item->GetValue(0))
					{
						ChatPacket(CHAT_TYPE_INFO, "Ese battlepass dejo de funcionar");
						return false;
					}

					m_dwBattlePassEndTime = get_global_time() + iSeconds;
					CBattlePass::instance().ResetMisionBattlePass(this);
					AddAffect(AFFECT_BATTLE_PASS, POINT_BATTLE_PASS_ID, item->GetValue(0), 0, iSeconds, 0, true);
					ITEM_MANAGER::instance().RemoveItem(item);
					ChatPacket(CHAT_TYPE_INFO, "Acabas de activar el Pase de Batalla.");

				}

			}
			break;

#endif

			case 50004: // ??ÂºÂ¥?Â®Â¿Ã« Â°Â¨?Ã¶Â±Ã¢
			{
				if (item->GetSocket(0))
				{
					item->SetSocket(0, item->GetSocket(0) + 1);
				}
				else
				{
					// ?Â³?Â½ Â»Ã§Â¿Ã«Â½?
					int iMapIndex = GetMapIndex();

					PIXEL_POSITION pos;

					if (SECTREE_MANAGER::instance().GetRandomLocation(iMapIndex, pos, 700))
					{
						item->SetSocket(0, 1);
						item->SetSocket(1, pos.x);
						item->SetSocket(2, pos.y);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, "187");
						return false;
					}
				}

				int dist = 0;
				float distance = (DISTANCE_SQRT(GetX() - item->GetSocket(1), GetY() - item->GetSocket(2)));

				if (distance < 1000.0f)
				{
					// Â¹?Â°?!
					ChatPacket(CHAT_TYPE_INFO, "188");

					// Â»Ã§Â¿Ã«?Â½Â¼Ã¶Â¿Â¡ ÂµÃ»Â¶Ã³ ??Â´? Â¾??????Â» Â´?Â¸Â£Â°? ??Â´?.
					struct TEventStoneInfo
					{
						DWORD dwVnum;
						int count;
						int prob;
					};
					const int EVENT_STONE_MAX_INFO = 15;
					TEventStoneInfo info_10[EVENT_STONE_MAX_INFO] =
					{
						{ 27001, 10,  8 },
						{ 27004, 10,  6 },
						{ 27002, 10, 12 },
						{ 27005, 10, 12 },
						{ 27100,  1,  9 },
						{ 27103,  1,  9 },
						{ 27101,  1, 10 },
						{ 27104,  1, 10 },
						{ 27999,  1, 12 },

						{ 25040,  1,  4 },

						{ 27410,  1,  0 },
						{ 27600,  1,  0 },
						{ 25100,  1,  0 },

						{ 50001,  1,  0 },
						{ 50003,  1,  1 },
					};
					TEventStoneInfo info_7[EVENT_STONE_MAX_INFO] =
					{
						{ 27001, 10,  1 },
						{ 27004, 10,  1 },
						{ 27004, 10,  9 },
						{ 27005, 10,  9 },
						{ 27100,  1,  5 },
						{ 27103,  1,  5 },
						{ 27101,  1, 10 },
						{ 27104,  1, 10 },
						{ 27999,  1, 14 },

						{ 25040,  1,  5 },

						{ 27410,  1,  5 },
						{ 27600,  1,  5 },
						{ 25100,  1,  5 },

						{ 50001,  1,  0 },
						{ 50003,  1,  5 },

					};
					TEventStoneInfo info_4[EVENT_STONE_MAX_INFO] =
					{
						{ 27001, 10,  0 },
						{ 27004, 10,  0 },
						{ 27002, 10,  0 },
						{ 27005, 10,  0 },
						{ 27100,  1,  0 },
						{ 27103,  1,  0 },
						{ 27101,  1,  0 },
						{ 27104,  1,  0 },
						{ 27999,  1, 25 },

						{ 25040,  1,  0 },

						{ 27410,  1,  0 },
						{ 27600,  1,  0 },
						{ 25100,  1, 15 },

						{ 50001,  1, 10 },
						{ 50003,  1, 50 },

					};

					{
						TEventStoneInfo* info;
						if (item->GetSocket(0) <= 4)
							info = info_4;
						else if (item->GetSocket(0) <= 7)
							info = info_7;
						else
							info = info_10;

						int prob = number(1, 100);

						for (int i = 0; i < EVENT_STONE_MAX_INFO; ++i)
						{
							if (!info[i].prob)
								continue;

							if (prob <= info[i].prob)
							{
								if (info[i].dwVnum == 50001)
								{
									DWORD* pdw = M2_NEW DWORD[2];

									pdw[0] = info[i].dwVnum;
									pdw[1] = info[i].count;

									// ???Â·Â¼Â­Â´? Â¼????Â» Â¼Â³?Â¤??Â´?
									DBManager::instance().ReturnQuery(QID_LOTTO, GetPlayerID(), pdw,
										"INSERT INTO lotto_list VALUES(0, 'server%s', %u, NOW())",
										get_table_postfix(), GetPlayerID());
								}
								else
									AutoGiveItem(info[i].dwVnum, info[i].count);

								break;
							}
							prob -= info[i].prob;
						}
					}

					char chatbuf[CHAT_MAX_LEN + 1];
					int len = snprintf(chatbuf, sizeof(chatbuf), "StoneDetect %u 0 0", (DWORD)GetVID());

					if (len < 0 || len >= (int)sizeof(chatbuf))
						len = sizeof(chatbuf) - 1;

					++len;  // \0 Â¹Â®??Â±Ã®?Ã¶ ÂºÂ¸Â³Â»Â±Ã¢

					TPacketGCChat pack_chat;
					pack_chat.header = HEADER_GC_CHAT;
					pack_chat.size = sizeof(TPacketGCChat) + len;
					pack_chat.type = CHAT_TYPE_COMMAND;
					pack_chat.id = 0;
					pack_chat.bEmpire = GetDesc()->GetEmpire();
					//pack_chat.id	= vid;

					TEMP_BUFFER buf;
					buf.write(&pack_chat, sizeof(TPacketGCChat));
					buf.write(chatbuf, len);

					PacketAround(buf.read_peek(), buf.size());

					ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (DETECT_EVENT_STONE) 1");
					return true;
				}
				else if (distance < 20000)
					dist = 1;
				else if (distance < 70000)
					dist = 2;
				else
					dist = 3;

				// Â¸Â¹?? Â»Ã§Â¿Ã«???Â¸Â¸Ã© Â»Ã§Â¶Ã³?Ã¸Â´?.
				const int STONE_DETECT_MAX_TRY = 10;
				if (item->GetSocket(0) >= STONE_DETECT_MAX_TRY)
				{
					ChatPacket(CHAT_TYPE_INFO, "189");
					ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (DETECT_EVENT_STONE) 0");
					AutoGiveItem(27002);
					return true;
				}

				if (dist)
				{
					char chatbuf[CHAT_MAX_LEN + 1];
					int len = snprintf(chatbuf, sizeof(chatbuf),
						"StoneDetect %u %d %d",
						(DWORD)GetVID(), dist, (int)GetDegreeFromPositionXY(GetX(), item->GetSocket(2), item->GetSocket(1), GetY()));

					if (len < 0 || len >= (int)sizeof(chatbuf))
						len = sizeof(chatbuf) - 1;

					++len;  // \0 Â¹Â®??Â±Ã®?Ã¶ ÂºÂ¸Â³Â»Â±Ã¢

					TPacketGCChat pack_chat;
					pack_chat.header = HEADER_GC_CHAT;
					pack_chat.size = sizeof(TPacketGCChat) + len;
					pack_chat.type = CHAT_TYPE_COMMAND;
					pack_chat.id = 0;
					pack_chat.bEmpire = GetDesc()->GetEmpire();
					//pack_chat.id		= vid;

					TEMP_BUFFER buf;
					buf.write(&pack_chat, sizeof(TPacketGCChat));
					buf.write(chatbuf, len);

					PacketAround(buf.read_peek(), buf.size());
				}

			}
			break;

			case 27989: // Â¿ÂµÂ¼Â®Â°Â¨?Ã¶Â±Ã¢
			case 76006: // Â¼Â±Â¹Â°Â¿Ã« Â¿ÂµÂ¼Â®Â°Â¨?Ã¶Â±Ã¢
			{
				LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(GetMapIndex());

				if (pMap != NULL)
				{
					item->SetSocket(0, item->GetSocket(0) + 1);

					FFindStone f;

					// <Factor> SECTREE::for_each -> SECTREE::for_each_entity
					pMap->for_each(f);

					if (f.m_mapStone.size() > 0)
					{
						std::map<DWORD, LPCHARACTER>::iterator stone = f.m_mapStone.begin();

						DWORD max = UINT_MAX;
						LPCHARACTER pTarget = stone->second;

						while (stone != f.m_mapStone.end())
						{
							DWORD dist = (DWORD)DISTANCE_SQRT(GetX() - stone->second->GetX(), GetY() - stone->second->GetY());

							if (dist != 0 && max > dist)
							{
								max = dist;
								pTarget = stone->second;
							}
							stone++;
						}

						if (pTarget != NULL)
						{
							int val = 3;

							if (max < 10000) val = 2;
							else if (max < 70000) val = 1;

							ChatPacket(CHAT_TYPE_COMMAND, "StoneDetect %u %d %d", (DWORD)GetVID(), val,
								(int)GetDegreeFromPositionXY(GetX(), pTarget->GetY(), pTarget->GetX(), GetY()));
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, "190");
						}
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, "190");
					}

					if (item->GetSocket(0) >= 6)
					{
						ChatPacket(CHAT_TYPE_COMMAND, "StoneDetect %u 0 0", (DWORD)GetVID());
						ITEM_MANAGER::instance().RemoveItem(item);
					}
				}
				break;
			}
			break;

			case 27996: // ÂµÂ¶ÂºÂ´
				item->SetCount(item->GetCount() - 1);
				AttackedByPoison(NULL); // @warme008
				break;

			case 27987: // ?Â¶Â°Â³
				// 50  ÂµÂ¹?Â¶Â°Â¢ 47990
				// 30  Â²?
				// 10  Â¹Ã©?Ã¸?? 47992
				// 7   ?Â»?Ã¸?? 47993
				// 3   ???Ã¸?? 47994
			{

				item->SetCount(item->GetCount() - 1);

				int r = number(1, 100);

				if (r <= 50)
				{
					ChatPacket(CHAT_TYPE_INFO, "191");
					AutoGiveItem(27990);
				}
				else
				{
					const int prob_table_gb2312[] =
					{
						95, 97, 99
					};

					const int* prob_table = prob_table_gb2312;

					if (r <= prob_table[0])
					{
						ChatPacket(CHAT_TYPE_INFO, "192");
					}
					else if (r <= prob_table[1])
					{
						ChatPacket(CHAT_TYPE_INFO, "193");
						AutoGiveItem(27992);
					}
					else if (r <= prob_table[2])
					{
						ChatPacket(CHAT_TYPE_INFO, "194");
						AutoGiveItem(27993);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, "195");
						AutoGiveItem(27994);
					}
				}
			}
			break;

			case 71013: // ???Â¦Â¿Ã«?Ã¸??
				CreateFly(number(FLY_FIREWORK1, FLY_FIREWORK6), this);
				item->SetCount(item->GetCount() - 1);
				break;

			case 50100: // ?Ã¸??
			case 50101:
			case 50102:
			case 50103:
			case 50104:
			case 50105:
			case 50106:
				CreateFly(item->GetVnum() - 50100 + FLY_FIREWORK1, this);
				item->SetCount(item->GetCount() - 1);
				break;

			case fishing::FISH_MIND_PILL_VNUM:
				AddAffect(AFFECT_FISH_MIND_PILL, POINT_NONE, 0, AFF_FISH_MIND, 20 * 60, 0, true);
				item->SetCount(item->GetCount() - 1);
				break;

			case 50301: // ?Ã«Â¼?Â·? Â¼Ã¶Â·?Â¼Â­
			case 50302:
			case 50303:
			{
				if (IsPolymorphed() == true)
				{
					ChatPacket(CHAT_TYPE_INFO, "196");
					return false;
				}

				int lv = GetSkillLevel(SKILL_LEADERSHIP);

				if (lv < item->GetValue(0))
				{
					ChatPacket(CHAT_TYPE_INFO, "197");
					return false;
				}

				if (lv >= item->GetValue(1))
				{
					ChatPacket(CHAT_TYPE_INFO, "198");
					return false;
				}

				if (LearnSkillByBook(SKILL_LEADERSHIP))
				{
#ifdef ENABLE_BOOKS_STACKFIX
					item->SetCount(item->GetCount() - 1);
#else
					ITEM_MANAGER::instance().RemoveItem(item);
#endif

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(SKILL_LEADERSHIP, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50304: // Â¿Â¬Â°Ã¨Â±Ã¢ Â¼Ã¶Â·?Â¼Â­
			case 50305:
			case 50306:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, "174");
					return false;

				}
				if (GetSkillLevel(SKILL_COMBO) == 0 && GetLevel() < 30)
				{
					ChatPacket(CHAT_TYPE_INFO, "199");
					return false;
				}

				if (GetSkillLevel(SKILL_COMBO) == 1 && GetLevel() < 50)
				{
					ChatPacket(CHAT_TYPE_INFO, "200");
					return false;
				}

				if (GetSkillLevel(SKILL_COMBO) >= 2)
				{
					ChatPacket(CHAT_TYPE_INFO, "201");
					return false;
				}

				int iPct = item->GetValue(0);

				if (LearnSkillByBook(SKILL_COMBO, iPct))
				{
#ifdef ENABLE_BOOKS_STACKFIX
					item->SetCount(item->GetCount() - 1);
#else
					ITEM_MANAGER::instance().RemoveItem(item);
#endif

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(SKILL_COMBO, get_global_time() + iReadDelay);
				}
			}
			break;
			case 50311: // Â¾Ã°Â¾Ã® Â¼Ã¶Â·?Â¼Â­
			case 50312:
			case 50313:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, "174");
					return false;

				}
				DWORD dwSkillVnum = item->GetValue(0);
				int iPct = MINMAX(0, item->GetValue(1), 100);
				if (GetSkillLevel(dwSkillVnum) >= 20 || dwSkillVnum - SKILL_LANGUAGE1 + 1 == GetEmpire())
				{
					ChatPacket(CHAT_TYPE_INFO, "202");
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
#ifdef ENABLE_BOOKS_STACKFIX
					item->SetCount(item->GetCount() - 1);
#else
					ITEM_MANAGER::instance().RemoveItem(item);
#endif

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50061: // ??ÂºÂ» Â¸Â» Â¼??Â¯ Â½Âº?Â³ Â¼Ã¶Â·?Â¼Â­
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, "174");
					return false;

				}
				DWORD dwSkillVnum = item->GetValue(0);
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetSkillLevel(dwSkillVnum) >= 10)
				{
					ChatPacket(CHAT_TYPE_INFO, "203");
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
#ifdef ENABLE_BOOKS_STACKFIX
					item->SetCount(item->GetCount() - 1);
#else
					ITEM_MANAGER::instance().RemoveItem(item);
#endif

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;
#ifdef USE_PASSIVE_ABILITY_TAMER
			case 50940:
			case 50941:
			case 50942:
			case 50950:
			case 50951:
			case 50952:
			case 50960:
			case 50961:
			case 50962:
			case 50970:
			case 50971:
			case 50972:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, "196");
					return false;
				}

				const auto skillVnum = item->GetValue(0);
				if (skillVnum == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "Something is wrong with this book, please contact the staff.");
					return false;
				}

				const auto skillLevel = GetSkillLevel(skillVnum);
				if (skillLevel >= 40)
				{
					ChatPacket(CHAT_TYPE_INFO, "You cannot train this ability anymore.");
					return false;
				}
				else if ((item->GetValue(1) > 0 && skillLevel < item->GetValue(1)) || skillLevel >= item->GetValue(2))
				{
					ChatPacket(CHAT_TYPE_INFO, "You cannot train this ability using this book.");
					return false;
				}

				if (LearnSkillByBook(skillVnum, item->GetValue(3), true))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server)
					{
						iReadDelay /= 3;
					}

					SetSkillNextReadTime(skillVnum, get_global_time() + iReadDelay);
				}
			}
			break;
#endif
			case 50314: case 50315: case 50316: // ÂºÂ¯Â½? Â¼Ã¶Â·?Â¼Â­
			case 50323: case 50324: // ?Ãµ?Ã· Â¼Ã¶Â·?Â¼Â­
			case 50325: case 50326: // ?Â¶?Ã« Â¼Ã¶Â·?Â¼Â­
			{
				if (IsPolymorphed() == true)
				{
					ChatPacket(CHAT_TYPE_INFO, "196");
					return false;
				}

				int iSkillLevelLowLimit = item->GetValue(0);
				int iSkillLevelHighLimit = item->GetValue(1);
				int iPct = MINMAX(0, item->GetValue(2), 100);
				int iLevelLimit = item->GetValue(3);
				DWORD dwSkillVnum = 0;

				switch (item->GetVnum())
				{
				case 50314: case 50315: case 50316:
					dwSkillVnum = SKILL_POLYMORPH;
					break;

				case 50323: case 50324:
					dwSkillVnum = SKILL_ADD_HP;
					break;

				case 50325: case 50326:
					dwSkillVnum = SKILL_RESIST_PENETRATE;
					break;

				default:
					return false;
				}

				if (0 == dwSkillVnum)
					return false;

				if (GetLevel() < iLevelLimit)
				{
					ChatPacket(CHAT_TYPE_INFO, "204");
					return false;
				}

				if (GetSkillLevel(dwSkillVnum) >= 40)
				{
					ChatPacket(CHAT_TYPE_INFO, "203");
					return false;
				}

				if (GetSkillLevel(dwSkillVnum) < iSkillLevelLowLimit)
				{
					ChatPacket(CHAT_TYPE_INFO, "197");
					return false;
				}

				if (GetSkillLevel(dwSkillVnum) >= iSkillLevelHighLimit)
				{
					ChatPacket(CHAT_TYPE_INFO, "205");
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
#ifdef ENABLE_BOOKS_STACKFIX
					item->SetCount(item->GetCount() - 1);
#else
					ITEM_MANAGER::instance().RemoveItem(item);
#endif

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50902:
			case 50903:
			case 50904:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, "174");
					return false;

				}
				DWORD dwSkillVnum = SKILL_CREATE;
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetSkillLevel(dwSkillVnum) >= 40)
				{
					ChatPacket(CHAT_TYPE_INFO, "203");
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
#ifdef ENABLE_BOOKS_STACKFIX
					item->SetCount(item->GetCount() - 1);
#else
					ITEM_MANAGER::instance().RemoveItem(item);
#endif

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);

					if (test_server)
					{
						ChatPacket(CHAT_TYPE_INFO, "[TEST_SERVER] Success to learn skill ");
					}
				}
				else
				{
					if (test_server)
					{
						ChatPacket(CHAT_TYPE_INFO, "[TEST_SERVER] Failed to learn skill ");
					}
				}
			}
			break;

			// MINING
			case ITEM_MINING_SKILL_TRAIN_BOOK:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, "174");
					return false;

				}
				DWORD dwSkillVnum = SKILL_MINING;
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetSkillLevel(dwSkillVnum) >= 40)
				{
					ChatPacket(CHAT_TYPE_INFO, "203");
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
#ifdef ENABLE_BOOKS_STACKFIX
					item->SetCount(item->GetCount() - 1);
#else
					ITEM_MANAGER::instance().RemoveItem(item);
#endif

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;
			// END_OF_MINING

			case ITEM_HORSE_SKILL_TRAIN_BOOK:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, "174");
					return false;

				}
				DWORD dwSkillVnum = SKILL_HORSE;
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetLevel() < 50)
				{
					ChatPacket(CHAT_TYPE_INFO, "206");
					return false;
				}

				if (!test_server && get_global_time() < GetSkillNextReadTime(dwSkillVnum))
				{
					if (FindAffect(AFFECT_SKILL_NO_BOOK_DELAY))
					{
						// ??Â¾?Â¼ÃºÂ¼Â­ Â»Ã§Â¿Ã«??Â¿Â¡Â´? Â½?Â°Â£ ?Â¦?? Â¹Â«Â½?
						RemoveAffect(AFFECT_SKILL_NO_BOOK_DELAY);
						ChatPacket(CHAT_TYPE_INFO, "207");
					}
					else
					{
						SkillLearnWaitMoreTimeMessage(GetSkillNextReadTime(dwSkillVnum) - get_global_time());
						return false;
					}
				}

				if (GetPoint(POINT_HORSE_SKILL) >= 20 ||
					GetSkillLevel(SKILL_HORSE_WILDATTACK) + GetSkillLevel(SKILL_HORSE_CHARGE) + GetSkillLevel(SKILL_HORSE_ESCAPE) >= 60 ||
					GetSkillLevel(SKILL_HORSE_WILDATTACK_RANGE) + GetSkillLevel(SKILL_HORSE_CHARGE) + GetSkillLevel(SKILL_HORSE_ESCAPE) >= 60)
				{
					ChatPacket(CHAT_TYPE_INFO, "208");
					return false;
				}

				if (number(1, 100) <= iPct)
				{
					ChatPacket(CHAT_TYPE_INFO, "209");
					ChatPacket(CHAT_TYPE_INFO, "210");
					PointChange(POINT_HORSE_SKILL, 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					if (!test_server)
						SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, "211");
				}
#ifdef ENABLE_BOOKS_STACKFIX
				item->SetCount(item->GetCount() - 1);
#else
				ITEM_MANAGER::instance().RemoveItem(item);
#endif
			}
			break;

			case 70102: // Â¼Â±Âµ?
			case 70103: // Â¼Â±Âµ?
			{
				if (GetAlignment() >= 0)
					return false;

				int delta = MIN(-GetAlignment(), item->GetValue(0));

				sys_log(0, "%s ALIGNMENT ITEM %d", GetName(), delta);

				UpdateAlignment(delta);
				item->SetCount(item->GetCount() - 1);

				if (delta / 10 > 0)
				{
					ChatPacket(CHAT_TYPE_TALKING, "212");
					ChatPacket(CHAT_TYPE_INFO, "213 %d", delta / 10);
				}
			}
			break;

			case 71107: // ?ÂµÂµÂµÂºÂ¹Â¼Ã¾Â¾?
			{
				int val = item->GetValue(0);
				int interval = item->GetValue(1);
				quest::PC* pPC = quest::CQuestManager::instance().GetPC(GetPlayerID());
				int last_use_time = pPC->GetFlag("mythical_peach.last_use_time");

				if (get_global_time() - last_use_time < interval * 60 * 60)
				{
					if (test_server == false)
					{
						ChatPacket(CHAT_TYPE_INFO, "214");
						return false;
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, "215");
					}
				}

				if (GetAlignment() == 200000)
				{
					ChatPacket(CHAT_TYPE_INFO, "216");
					return false;
				}

				if (200000 - GetAlignment() < val * 10)
				{
					val = (200000 - GetAlignment()) / 10;
				}

				int old_alignment = GetAlignment() / 10;

				UpdateAlignment(val * 10);

				item->SetCount(item->GetCount() - 1);
				pPC->SetFlag("mythical_peach.last_use_time", get_global_time());

				ChatPacket(CHAT_TYPE_TALKING, "212");
				ChatPacket(CHAT_TYPE_INFO, "213 %d", val);

				char buf[256 + 1];
				snprintf(buf, sizeof(buf), "%d %d", old_alignment, GetAlignment() / 10);
				LogManager::instance().CharLog(this, val, "MYTHICAL_PEACH", buf);
			}
			break;

			case 71109: // ?Â»Â¼Â®Â¼Â­
			case 72719:
			{
				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
					return false;

				if (item2->GetSocketCount() == 0)
					return false;

				switch (item2->GetType())
				{
				case ITEM_WEAPON:
					break;
				case ITEM_ARMOR:
					switch (item2->GetSubType())
					{
					case ARMOR_EAR:
					case ARMOR_WRIST:
					case ARMOR_NECK:
#ifdef ENABLE_YOHARA_SYSTEM
					case ARMOR_GLOVE:
#endif
						ChatPacket(CHAT_TYPE_INFO, "217");
						return false;
					}
					break;

				default:
					return false;
				}

				std::stack<long> socket;

				const BYTE maxSocketforMetin = 3;

				for (int i = 0; i < maxSocketforMetin; ++i)
					socket.push(item2->GetSocket(i));

				int idx = ITEM_STONES_MAX_NUM - 1;

				while (socket.size() > 0)
				{
					if (socket.top() > 2 && socket.top() != ITEM_BROKEN_METIN_VNUM)
						break;

					idx--;
					socket.pop();
				}

				if (socket.size() == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "217");
					return false;
				}

				LPITEM pItemReward = AutoGiveItem(socket.top());

				if (pItemReward != NULL)
				{
					item2->SetSocket(idx, 1);

					char buf[256 + 1];
					snprintf(buf, sizeof(buf), "%s(%u) %s(%u)",
						item2->GetName(), item2->GetID(), pItemReward->GetName(), pItemReward->GetID());
					LogManager::instance().ItemLog(this, item, "USE_DETACHMENT_ONE", buf);

					item->SetCount(item->GetCount() - 1);
				}
			}
			break;

			case 70201:   // ?Â»Â»Ã¶?Â¦
			case 70202:   // Â¿Â°Â»Ã¶Â¾?(?Ã²Â»Ã¶)
			case 70203:   // Â¿Â°Â»Ã¶Â¾?(Â±?Â»Ã¶)
			case 70204:   // Â¿Â°Â»Ã¶Â¾?(Â»Â¡Â°Â£Â»Ã¶)
			case 70205:   // Â¿Â°Â»Ã¶Â¾?(Â°Â¥Â»Ã¶)
			case 70206:   // Â¿Â°Â»Ã¶Â¾?(Â°??ÂºÂ»Ã¶)
			{
				// NEW_HAIR_STYLE_ADD
				if (GetPart(PART_HAIR) >= 1001)
				{
					ChatPacket(CHAT_TYPE_INFO, "218");
				}
				// END_NEW_HAIR_STYLE_ADD
				else
				{
					quest::CQuestManager& q = quest::CQuestManager::instance();
					quest::PC* pPC = q.GetPC(GetPlayerID());

					if (pPC)
					{
						int last_dye_level = pPC->GetFlag("dyeing_hair.last_dye_level");

						if (last_dye_level == 0 ||
							last_dye_level + 3 <= GetLevel() ||
							item->GetVnum() == 70201)
						{
							SetPart(PART_HAIR, item->GetVnum() - 70201);

							if (item->GetVnum() == 70201)
								pPC->SetFlag("dyeing_hair.last_dye_level", 0);
							else
								pPC->SetFlag("dyeing_hair.last_dye_level", GetLevel());

							item->SetCount(item->GetCount() - 1);
							UpdatePacket();
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, "219 %d", last_dye_level + 3);
						}
					}
				}
			}
			break;

			case ITEM_NEW_YEAR_GREETING_VNUM:
			{
#ifdef ENABLE_MULTI_CHEST
				OpenChest(item, 1);
				return true;
#else

				DWORD dwBoxVnum = ITEM_NEW_YEAR_GREETING_VNUM;
				std::vector <DWORD> dwVnums;
				std::vector <DWORD> dwCounts;
				std::vector <LPITEM> item_gets;
				int count = 0;

				if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
				{
					for (int i = 0; i < count; i++)
					{
						if (dwVnums[i] == CSpecialItemGroup::GOLD)
#ifdef __CHAT_SETTINGS__
							ChatPacket(CHAT_TYPE_MONEY_INFO, "158 %d", dwCounts[i]);
#else
							ChatPacket(CHAT_TYPE_INFO, "158 %d", dwCounts[i]);
#endif
					}

					item->SetCount(item->GetCount() - 1);
				}
#endif
			}
			break;

			case ITEM_VALENTINE_ROSE:
			case ITEM_VALENTINE_CHOCOLATE:
			{
#ifdef ENABLE_MULTI_CHEST
				OpenChest(item, 1);
				return true;
#else
				DWORD dwBoxVnum = item->GetVnum();
				std::vector <DWORD> dwVnums;
				std::vector <DWORD> dwCounts;
				std::vector <LPITEM> item_gets(0);
				int count = 0;
				if (((item->GetVnum() == ITEM_VALENTINE_ROSE) && (SEX_MALE == GET_SEX(this))) ||
					((item->GetVnum() == ITEM_VALENTINE_CHOCOLATE) && (SEX_FEMALE == GET_SEX(this))))
				{
					// Â¼ÂºÂºÂ°?? Â¸??Ã¶Â¾?Â¾? Â¾Âµ Â¼Ã¶ Â¾Ã¸Â´?.
					ChatPacket(CHAT_TYPE_INFO, "220");
					return false;
				}
				if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
					item->SetCount(item->GetCount() - 1);
#endif
			}
			break;

			case ITEM_WHITEDAY_CANDY:
			case ITEM_WHITEDAY_ROSE:
			{
#ifdef ENABLE_MULTI_CHEST
				OpenChest(item, 1);
				return true;
#else
				DWORD dwBoxVnum = item->GetVnum();
				std::vector <DWORD> dwVnums;
				std::vector <DWORD> dwCounts;
				std::vector <LPITEM> item_gets(0);
				int count = 0;
				if (((item->GetVnum() == ITEM_WHITEDAY_CANDY) && (SEX_MALE == GET_SEX(this))) ||
					((item->GetVnum() == ITEM_WHITEDAY_ROSE) && (SEX_FEMALE == GET_SEX(this))))
				{
					// Â¼ÂºÂºÂ°?? Â¸??Ã¶Â¾?Â¾? Â¾Âµ Â¼Ã¶ Â¾Ã¸Â´?.
					ChatPacket(CHAT_TYPE_INFO, "220");
					return false;
				}
				if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
					item->SetCount(item->GetCount() - 1);
#endif
			}
			break;

			case 50011: // Â¿Ã¹Â±Â¤ÂºÂ¸??
			{
#ifdef ENABLE_MULTI_CHEST
				OpenChest(item, 1);
				return true;
#else
				DWORD dwBoxVnum = 50011;
				std::vector <DWORD> dwVnums;
				std::vector <DWORD> dwCounts;
				std::vector <LPITEM> item_gets(0);
				int count = 0;

				if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
				{
					for (int i = 0; i < count; i++)
					{
						char buf[50 + 1];
						snprintf(buf, sizeof(buf), "%u %u", dwVnums[i], dwCounts[i]);
						LogManager::instance().ItemLog(this, item, "MOONLIGHT_GET", buf);

						//ITEM_MANAGER::instance().RemoveItem(item);
						item->SetCount(item->GetCount() - 1);

						switch (dwVnums[i])
						{
						case CSpecialItemGroup::GOLD:
#ifdef __CHAT_SETTINGS__
							ChatPacket(CHAT_TYPE_MONEY_INFO, "158 %d", dwCounts[i]);
#else
							ChatPacket(CHAT_TYPE_INFO, "158 %d", dwCounts[i]);
#endif
							break;

						case CSpecialItemGroup::EXP:
							ChatPacket(CHAT_TYPE_INFO, "159");
#ifdef __CHAT_SETTINGS__
							ChatPacket(CHAT_TYPE_EXP_INFO, "160 %d", dwCounts[i]);
#else
							ChatPacket(CHAT_TYPE_INFO, "160 %d", dwCounts[i]);
#endif
							break;

						case CSpecialItemGroup::MOB:
							ChatPacket(CHAT_TYPE_INFO, "161");
							break;

						case CSpecialItemGroup::SLOW:
							ChatPacket(CHAT_TYPE_INFO, "162");
							break;

						case CSpecialItemGroup::DRAIN_HP:
							ChatPacket(CHAT_TYPE_INFO, "163");
							break;

						case CSpecialItemGroup::POISON:
							ChatPacket(CHAT_TYPE_INFO, "164");
							break;
#ifdef ENABLE_WOLFMAN_CHARACTER
						case CSpecialItemGroup::BLEEDING:
							ChatPacket(CHAT_TYPE_INFO, "164");
							break;
#endif
						case CSpecialItemGroup::MOB_GROUP:
							ChatPacket(CHAT_TYPE_INFO, "161");
							break;

						default:
							if (item_gets[i])
							{
#ifdef __CHAT_SETTINGS__
								if (dwCounts[i] > 1)
									ChatPacket(CHAT_TYPE_ITEM_INFO, "165 %s %d", item_gets[i]->GetName(), dwCounts[i]);
								else
									ChatPacket(CHAT_TYPE_ITEM_INFO, "166 %s", item_gets[i]->GetName());
#else
								if (dwCounts[i] > 1)
									ChatPacket(CHAT_TYPE_INFO, "165 %s %d", item_gets[i]->GetName(), dwCounts[i]);
								else
									ChatPacket(CHAT_TYPE_INFO, "166 %s", item_gets[i]->GetName());
#endif
							}
							break;
						}
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_TALKING, "171");
					return false;
				}
#endif
			}
			break;

			case ITEM_GIVE_STAT_RESET_COUNT_VNUM:
			{
				//PointChange(POINT_GOLD, -iCost);
				PointChange(POINT_STAT_RESET_COUNT, 1);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50107:
			{
				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					ChatPacket(CHAT_TYPE_INFO, "134");
					return false;
				}
#ifdef ENABLE_NEWSTUFF
				else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(item->GetVnum()))
				{
					ChatPacket(CHAT_TYPE_INFO, "134");
					return false;
				}
#endif

				EffectPacket(SE_CHINA_FIREWORK);
#ifdef ENABLE_FIREWORK_STUN
				// Â½Âº?? Â°Ã¸Â°??Â» Â¿?Â·???Â´?
				AddAffect(AFFECT_CHINA_FIREWORK, POINT_STUN_PCT, 30, AFF_CHINA_FIREWORK, 5 * 60, 0, true);
#endif
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50108:
			{
				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					ChatPacket(CHAT_TYPE_INFO, "134");
					return false;
				}
#ifdef ENABLE_NEWSTUFF
				else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(item->GetVnum()))
				{
					ChatPacket(CHAT_TYPE_INFO, "134");
					return false;
				}
#endif

				EffectPacket(SE_SPIN_TOP);
#ifdef ENABLE_FIREWORK_STUN
				// Â½Âº?? Â°Ã¸Â°??Â» Â¿?Â·???Â´?
				AddAffect(AFFECT_CHINA_FIREWORK, POINT_STUN_PCT, 30, AFF_CHINA_FIREWORK, 5 * 60, 0, true);
#endif
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_WONSO_BEAN_VNUM:
				PointChange(POINT_HP, GetMaxHP() - GetHP());
				item->SetCount(item->GetCount() - 1);
				break;

			case ITEM_WONSO_SUGAR_VNUM:
				PointChange(POINT_SP, GetMaxSP() - GetSP());
				item->SetCount(item->GetCount() - 1);
				break;

			case ITEM_WONSO_FRUIT_VNUM:
				PointChange(POINT_STAMINA, GetMaxStamina() - GetStamina());
				item->SetCount(item->GetCount() - 1);
				break;

			case 90008: // VCARD
			case 90009: // VCARD
				VCardUse(this, this, item);
				break;

			case ITEM_ELK_VNUM: // ÂµÂ·Â²?Â·Â¯Â¹?
			{
				int iGold = item->GetSocket(0);
				ITEM_MANAGER::instance().RemoveItem(item);
				ChatPacket(CHAT_TYPE_INFO, "158 %d", iGold);
				PointChange(POINT_GOLD, iGold);
			}
			break;

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
			case ITEM_VNUM_SWITCHBOT_PREMIUM:
				if (FindAffect(AFFECT_SWITCHBOT))
				{
					ChatPacket(CHAT_TYPE_INFO, "You have already this affect!");
					return false;
				}

				AddAffect(AFFECT_SWITCHBOT, POINT_NONE, get_global_time() + item->GetValue(0), 0, item->GetValue(0), 1, true);
				item->SetCount(item->GetCount() - 1);
				break;
#endif

				//Â±Âº???? ?Ãµ?Â¥
			case 70021:
			{
				int HealPrice = quest::CQuestManager::instance().GetEventFlag("MonarchHealGold");
				if (HealPrice == 0)
					HealPrice = 2000000;

				if (CMonarch::instance().HealMyEmpire(this, HealPrice))
				{
					char szNotice[256];
					snprintf(szNotice, sizeof(szNotice), "221 %s", EMPIRE_NAME(GetEmpire()));
					SendNoticeMap(szNotice, GetMapIndex(), false);

					ChatPacket(CHAT_TYPE_INFO, "222");
				}
			}
			break;

			case 27995:
			{
			}
			break;
#ifdef ENABLE_BATTLEPASS						
			case BATTLEPASS_PREMIUM_ITEM_VNUM:
			{
				if (!ActivateBattlePassPremium())
				{
					ChatPacket(CHAT_TYPE_INFO, "You've already purchased BattlePass this season!");
					return false;
				}
				item->SetCount(item->GetCount() - 1);
			}
			break;
#endif
			case 71092: // ÂºÂ¯Â½? ???Â¼Âº? ??Â½?
			{
				if (m_pkChrTarget != NULL)
				{
					if (m_pkChrTarget->IsPolymorphed())
					{
						m_pkChrTarget->SetPolymorph(0);
						m_pkChrTarget->RemoveAffect(AFFECT_POLYMORPH);
					}
				}
				else
				{
					if (IsPolymorphed())
					{
						SetPolymorph(0);
						RemoveAffect(AFFECT_POLYMORPH);
					}
				}
			}
			break;
#ifdef CHANGE_DS_ATRIBUTES
			case 79001:
			{
				LPITEM item2;

				if (!DSManager::instance().IsValidCellForThisItem(item2, DestCell) || !item2->IsDragonSoul())
					return false;

				if (ITEM_DS != item2->GetType())
				{
					ChatPacket(CHAT_TYPE_INFO, "223");
					return false;
				}

				if (item2->IsExchanging()) // @fixme114
					return false;

				item->SetCount(item->GetCount() - 1);
				// if (DSManager::PutAttributes(item2)){
					// ChatPacket(CHAT_TYPE_INFO, "224");
				// }
				// else{
					// ChatPacket(CHAT_TYPE_INFO, "225");
				// }
			}
			break;
#endif
			case 71068:
			{
				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				{
					ChatPacket(CHAT_TYPE_INFO, "Not valid item position.");
					return false;
				}
				if (item2->IsExchanging() == true)
				{
					ChatPacket(CHAT_TYPE_INFO, "Item in exchange.");
					return false;
				}

				if (item2->IsEquipped())
				{
					ChatPacket(CHAT_TYPE_INFO, "Item is equipped.");
					return false;
				}

				if ((item2->GetType() == ITEM_WEAPON) || (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY))
				{
					if (item2->GetSocketCount() == 4)
					{
						ChatPacket(CHAT_TYPE_INFO, "The item already has full sockets.");
						return false;
					}
					item2->SetSocket(3, 1);
					item2->UpdatePacket();
					// item2->Save();
					ChatPacket(CHAT_TYPE_INFO, "The socket was succesfully added.");
					item->SetCount(item->GetCount() - 1);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, "Cannot unlock another socket on this item.");
					break;
				}
			}
			break;

#if !defined(__BL_67_ATTR__)
			case 71051: // ì§?Â¬ê¿
			{
				if (LC_IsEurope() || LC_IsSingapore() || LC_IsVietnam())
					return false;

				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetInventoryItem(wDestCell)))
					return false;

				if (item2->IsExchanging() == true)
					return false;

				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, "226");
					return false;
				}

				if (item2->AddRareAttribute() == true)
				{
					ChatPacket(CHAT_TYPE_INFO, "227");

					int iAddedIdx = item2->GetRareAttrCount() + 4;
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());

					LogManager::instance().ItemLog(
						GetPlayerID(),
						item2->GetAttributeType(iAddedIdx),
						item2->GetAttributeValue(iAddedIdx),
						item->GetID(),
						"ADD_RARE_ATTR",
						buf,
						GetDesc()->GetHostName(),
						item->GetOriginalVnum());

					item->SetCount(item->GetCount() - 1);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, "228");
				}
			}
			break;
#endif
#if !defined(__BL_67_ATTR__)
			case 71052: // ì§?Â¬ê²½
			{
				// ?Â ?? ?Â±ê¿¿? ë² ?Â¸ë¿ì§?Â¬ê¿ ?Â¬ì©ê¿¬Â§Â
				if (LC_IsEurope() || LC_IsSingapore() || LC_IsVietnam())
					return false;

				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging() == true)
					return false;

				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, "229");
					return false;
				}

				if (item2->ChangeRareAttribute() == true)
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "CHANGE_RARE_ATTR", buf);

					item->SetCount(item->GetCount() - 1);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, "230");
				}
			}
			break;
#endif

			case ITEM_AUTO_HP_RECOVERY_S:
			case ITEM_AUTO_HP_RECOVERY_M:
			case ITEM_AUTO_HP_RECOVERY_L:
			case ITEM_AUTO_HP_RECOVERY_X:
			case ITEM_AUTO_SP_RECOVERY_S:
			case ITEM_AUTO_SP_RECOVERY_M:
			case ITEM_AUTO_SP_RECOVERY_L:
			case ITEM_AUTO_SP_RECOVERY_X:
				// Â¹Â«Â½?Â¹Â«Â½????Ã¶Â¸Â¸ ???Ã¼Â¿Â¡ ??Â´Ã¸ Â°? Â°Ã­?Â¡Â±Ã¢Â´? Â¹Â«Â¼Â·Â°Ã­...
				// Â±?Â·Â¡Â¼Â­ Â±?Â³? ??ÂµÃ¥ ??ÂµÃ¹. Â¼Â±Â¹Â° Â»Ã³??Â¿Ã« ??ÂµÂ¿Â¹Â°Â¾? Â¾?????ÂµÃ©.
			case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_XS:
			case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_S:
			case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_XS:
			case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_S:
			case FUCKING_BRAZIL_ITEM_AUTO_SP_RECOVERY_S:
			case FUCKING_BRAZIL_ITEM_AUTO_HP_RECOVERY_S:
			{
				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					ChatPacket(CHAT_TYPE_INFO, "177");
					return false;
				}
#ifdef ENABLE_NEWSTUFF
				else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(item->GetVnum()))
				{
					ChatPacket(CHAT_TYPE_INFO, "134");
					return false;
				}
#endif

				EAffectTypes type = AFFECT_NONE;
				bool isSpecialPotion = false;

				switch (item->GetVnum())
				{
				case ITEM_AUTO_HP_RECOVERY_X:
					isSpecialPotion = true;

				case ITEM_AUTO_HP_RECOVERY_S:
				case ITEM_AUTO_HP_RECOVERY_M:
				case ITEM_AUTO_HP_RECOVERY_L:
				case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_XS:
				case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_S:
				case FUCKING_BRAZIL_ITEM_AUTO_HP_RECOVERY_S:
					type = AFFECT_AUTO_HP_RECOVERY;
					break;

				case ITEM_AUTO_SP_RECOVERY_X:
					isSpecialPotion = true;

				case ITEM_AUTO_SP_RECOVERY_S:
				case ITEM_AUTO_SP_RECOVERY_M:
				case ITEM_AUTO_SP_RECOVERY_L:
				case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_XS:
				case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_S:
				case FUCKING_BRAZIL_ITEM_AUTO_SP_RECOVERY_S:
					type = AFFECT_AUTO_SP_RECOVERY;
					break;
				}

				if (AFFECT_NONE == type)
					break;

				if (item->GetCount() > 1)
				{
					int pos = GetEmptyInventory(item->GetSize());

					if (-1 == pos)
					{
						ChatPacket(CHAT_TYPE_INFO, "129");
						break;
					}

					item->SetCount(item->GetCount() - 1);

					LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), 1);
					item2->AddToCharacter(this, TItemPos(INVENTORY, pos));

					if (item->GetSocket(1) != 0)
					{
						item2->SetSocket(1, item->GetSocket(1));
					}

					item = item2;
				}

				CAffect* pAffect = FindAffect(type);

				if (NULL == pAffect)
				{
					EPointTypes bonus = POINT_NONE;

					if (true == isSpecialPotion)
					{
						if (type == AFFECT_AUTO_HP_RECOVERY)
						{
							bonus = POINT_MAX_HP_PCT;
						}
						else if (type == AFFECT_AUTO_SP_RECOVERY)
						{
							bonus = POINT_MAX_SP_PCT;
						}
					}

					AddAffect(type, bonus, 4, item->GetID(), INFINITE_AFFECT_DURATION, 0, true, false);

					item->Lock(true);
					item->SetSocket(0, true);

					AutoRecoveryItemProcess(type);
				}
				else
				{
					if (item->GetID() == pAffect->dwFlag)
					{
						RemoveAffect(pAffect);

						item->Lock(false);
						item->SetSocket(0, false);
					}
					else
					{
						LPITEM old = FindItemByID(pAffect->dwFlag);

						if (NULL != old)
						{
							old->Lock(false);
							old->SetSocket(0, false);
						}

						RemoveAffect(pAffect);

						EPointTypes bonus = POINT_NONE;

						if (true == isSpecialPotion)
						{
							if (type == AFFECT_AUTO_HP_RECOVERY)
							{
								bonus = POINT_MAX_HP_PCT;
							}
							else if (type == AFFECT_AUTO_SP_RECOVERY)
							{
								bonus = POINT_MAX_SP_PCT;
							}
						}

						AddAffect(type, bonus, 4, item->GetID(), INFINITE_AFFECT_DURATION, 0, true, false);

						item->Lock(true);
						item->SetSocket(0, true);

						AutoRecoveryItemProcess(type);
					}
				}
			}
			break;

#ifdef __AURA_SYSTEM__
			case ITEM_AURA_BOOST_ITEM_VNUM_BASE + ITEM_AURA_BOOST_ERASER:
			{
				LPITEM item2;
				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging() || item2->IsEquipped())
					return false;

#ifdef ENABLE_SEALBIND_SYSTEM
				if (item2->IsBound())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ìì°ë¼> ë¹ì ì ìí¼ì ë¬¶ì¬ ìê¸° ëë¬¸ì ë¹ì ì ìì°ë¼ ìììì ë¶ì¤í¸ë¥¼ ì§ì¸ ì ììµëë¤."));
					return false;
				}
#endif

				if (IS_SET(item->GetFlag(), ITEM_FLAG_STACKABLE) && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK) && item->GetCount() > 1)
					item->SetCount(item->GetCount() - 1);
				else
					ITEM_MANAGER::instance().RemoveItem(item);

				for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
					item2->SetForceAttribute(i, 0, 0);

				item2->SetSocket(0, 0);
				item2->SetSocket(ITEM_SOCKET_AURA_BOOST, 0);
			}
			break;
#endif


			}
			break;

		case USE_CLEAR:
		{
			switch (item->GetVnum())
			{
#ifdef ENABLE_WOLFMAN_CHARACTER
			case 27124: // Bandage
				RemoveBleeding();
				break;
#endif
			case 27874: // Grilled Perch
			default:
				RemoveBadAffect();
				break;
			}
			item->SetCount(item->GetCount() - 1);
		}
		break;

		case USE_INVISIBILITY:
		{
			if (item->GetVnum() == 70026)
			{
				quest::CQuestManager& q = quest::CQuestManager::instance();
				quest::PC* pPC = q.GetPC(GetPlayerID());

				if (pPC != NULL)
				{
					int last_use_time = pPC->GetFlag("mirror_of_disapper.last_use_time");

					if (get_global_time() - last_use_time < 10 * 60)
					{
						ChatPacket(CHAT_TYPE_INFO, "214");
						return false;
					}

					pPC->SetFlag("mirror_of_disapper.last_use_time", get_global_time());
				}
			}

			AddAffect(AFFECT_INVISIBILITY, POINT_NONE, 0, AFF_INVISIBILITY, 300, 0, true);
			item->SetCount(item->GetCount() - 1);
		}
		break;

		case USE_POTION_NODELAY:
		{
#ifdef ENABLE_SUNG_MAHI_TOWER
			if (!IS_SUNG_MAHI_ENABLE_ITEM(GetMapIndex(), item->GetVnum()))
			{
				ChatPacket(CHAT_TYPE_INFO, "You cannot use this type of potion here.");
				return false;
			}
#endif


			if (!CanUseItem(GetMapIndex(), item->GetVnum()))
			{
				ChatPacket(CHAT_TYPE_INFO, "You can't use this");
				return false;
			}

			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "177");
					return false;
				}

				switch (item->GetVnum())
				{
				case 70020:
				case 71018:
				case 71019:
				case 71020:
					if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
					{
						if (m_nPotionLimit <= 0)
						{
							ChatPacket(CHAT_TYPE_INFO, "178");
							return false;
						}
					}
					break;

				default:
					ChatPacket(CHAT_TYPE_INFO, "177");
					return false;
				}
			}
#ifdef ENABLE_NEWSTUFF
			else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(item->GetVnum()))
			{
				ChatPacket(CHAT_TYPE_INFO, "134");
				return false;
			}
#endif

			bool used = false;

			if (item->GetValue(0) != 0) // HP ?Ã½Â´Ã«Â°Âª ?Â¸ÂºÂ¹
			{
				if (GetHP() < GetMaxHP())
				{
					PointChange(POINT_HP, item->GetValue(0) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
					EffectPacket(SE_HPUP_RED);
					used = TRUE;
				}
			}

			if (item->GetValue(1) != 0)	// SP ?Ã½Â´Ã«Â°Âª ?Â¸ÂºÂ¹
			{
				if (GetSP() < GetMaxSP())
				{
					PointChange(POINT_SP, item->GetValue(1) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
					EffectPacket(SE_SPUP_BLUE);
					used = TRUE;
				}
			}

			if (item->GetValue(3) != 0) // HP % ?Â¸ÂºÂ¹
			{
				if (GetHP() < GetMaxHP())
				{
					PointChange(POINT_HP, item->GetValue(3) * GetMaxHP() / 100);
					EffectPacket(SE_HPUP_RED);
					used = TRUE;
				}
			}

			if (item->GetValue(4) != 0) // SP % ?Â¸ÂºÂ¹
			{
				if (GetSP() < GetMaxSP())
				{
					PointChange(POINT_SP, item->GetValue(4) * GetMaxSP() / 100);
					EffectPacket(SE_SPUP_BLUE);
					used = TRUE;
				}
			}

			if (used)
			{
				if (item->GetVnum() == 50085 || item->GetVnum() == 50086)
				{
					if (test_server)
						ChatPacket(CHAT_TYPE_INFO, "179");
					SetUseSeedOrMoonBottleTime();
				}
				if (GetDungeon())
					GetDungeon()->UsePotion(this);

				if (GetWarMap())
					GetWarMap()->UsePotion(this, item);

				m_nPotionLimit--;

				//RESTRICT_USE_SEED_OR_MOONBOTTLE
				item->SetCount(item->GetCount() - 1);
				//END_RESTRICT_USE_SEED_OR_MOONBOTTLE
			}
		}
		break;

		case USE_POTION:
#ifdef ENABLE_SUNG_MAHI_TOWER
			if (!IS_SUNG_MAHI_ENABLE_ITEM(GetMapIndex(), item->GetVnum()))
			{
				ChatPacket(CHAT_TYPE_INFO, "You cannot use this type of potion here.");
				return false;
			}
#endif

			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "177");
					return false;
				}

				switch (item->GetVnum())
				{
				case 27001:
				case 27002:
				case 27003:
				case 27004:
				case 27005:
				case 27006:
					if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
					{
						if (m_nPotionLimit <= 0)
						{
							ChatPacket(CHAT_TYPE_INFO, "178");
							return false;
						}
					}
					break;

				default:
					ChatPacket(CHAT_TYPE_INFO, "177");
					return false;
				}
			}
#ifdef ENABLE_NEWSTUFF
			else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(item->GetVnum()))
			{
				ChatPacket(CHAT_TYPE_INFO, "134");
				return false;
			}
#endif

			if (item->GetValue(1) != 0)
			{
				if (GetPoint(POINT_SP_RECOVERY) + GetSP() >= GetMaxSP())
				{
					return false;
				}

				PointChange(POINT_SP_RECOVERY, item->GetValue(1) * MIN(200, (100 + GetPoint(POINT_POTION_BONUS))) / 100);
				StartAffectEvent();
				EffectPacket(SE_SPUP_BLUE);
			}

			if (item->GetValue(0) != 0)
			{
				if (GetPoint(POINT_HP_RECOVERY) + GetHP() >= GetMaxHP())
				{
					return false;
				}

				PointChange(POINT_HP_RECOVERY, item->GetValue(0) * MIN(200, (100 + GetPoint(POINT_POTION_BONUS))) / 100);
				StartAffectEvent();
				EffectPacket(SE_HPUP_RED);
			}

			if (GetDungeon())
				GetDungeon()->UsePotion(this);

			if (GetWarMap())
				GetWarMap()->UsePotion(this, item);

			item->SetCount(item->GetCount() - 1);
			m_nPotionLimit--;
			break;

		case USE_POTION_CONTINUE:
		{
			if (item->GetValue(0) != 0)
			{
				AddAffect(AFFECT_HP_RECOVER_CONTINUE, POINT_HP_RECOVER_CONTINUE, item->GetValue(0), 0, item->GetValue(2), 0, true);
			}
			else if (item->GetValue(1) != 0)
			{
				AddAffect(AFFECT_SP_RECOVER_CONTINUE, POINT_SP_RECOVER_CONTINUE, item->GetValue(1), 0, item->GetValue(2), 0, true);
			}
			else
				return false;
		}

		if (GetDungeon())
			GetDungeon()->UsePotion(this);

		if (GetWarMap())
			GetWarMap()->UsePotion(this, item);

		item->SetCount(item->GetCount() - 1);
		break;

		case USE_ABILITY_UP:
		{
			switch (item->GetValue(0))
			{
			case APPLY_MOV_SPEED:
				AddAffect(AFFECT_MOV_SPEED, POINT_MOV_SPEED, item->GetValue(2), AFF_MOV_SPEED_POTION, item->GetValue(1), 0, true);
#ifdef ENABLE_EFFECT_EXTRAPOT
				EffectPacket(SE_DXUP_PURPLE);
#endif
				break;

			case APPLY_ATT_SPEED:
				AddAffect(AFFECT_ATT_SPEED, POINT_ATT_SPEED, item->GetValue(2), AFF_ATT_SPEED_POTION, item->GetValue(1), 0, true);
#ifdef ENABLE_EFFECT_EXTRAPOT
				EffectPacket(SE_SPEEDUP_GREEN);
#endif
				break;

			case APPLY_STR:
				AddAffect(AFFECT_STR, POINT_ST, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_DEX:
				AddAffect(AFFECT_DEX, POINT_DX, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_CON:
				AddAffect(AFFECT_CON, POINT_HT, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_INT:
				AddAffect(AFFECT_INT, POINT_IQ, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_CAST_SPEED:
				AddAffect(AFFECT_CAST_SPEED, POINT_CASTING_SPEED, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_ATT_GRADE_BONUS:
				AddAffect(AFFECT_ATT_GRADE, POINT_ATT_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_DEF_GRADE_BONUS:
				AddAffect(AFFECT_DEF_GRADE, POINT_DEF_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;
			}
		}

		if (GetDungeon())
			GetDungeon()->UsePotion(this);

		if (GetWarMap())
			GetWarMap()->UsePotion(this, item);

		item->SetCount(item->GetCount() - 1);
		break;

		case USE_TALISMAN:
		{
			const int TOWN_PORTAL = 1;
			const int MEMORY_PORTAL = 2;


			// gm_guild_build, oxevent Â¸?Â¿Â¡Â¼Â­ Â±??Â¯Âº? Â±??Â¯Â±Ã¢Â¾Ã¯Âº? Â¸Â¦ Â»Ã§Â¿Ã«Â¸Ã¸??Â°? Â¸Â·?Â½
			if (GetMapIndex() == 200 || GetMapIndex() == 113)
			{
				ChatPacket(CHAT_TYPE_INFO, "231");
				return false;
			}

			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				ChatPacket(CHAT_TYPE_INFO, "134");
				return false;
			}
#ifdef ENABLE_NEWSTUFF
			else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(item->GetVnum()))
			{
				ChatPacket(CHAT_TYPE_INFO, "134");
				return false;
			}
#endif

			if (m_pkWarpEvent)
			{
				ChatPacket(CHAT_TYPE_INFO, "232");
				return false;
			}

			// CONSUME_LIFE_WHEN_USE_WARP_ITEM
			int consumeLife = CalculateConsume(this);

			if (consumeLife < 0)
				return false;
			// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

			if (item->GetValue(0) == TOWN_PORTAL) // Â±??Â¯Âº?
			{
				if (item->GetSocket(0) == 0)
				{
					if (!GetDungeon())
						if (!GiveRecallItem(item))
							return false;

					PIXEL_POSITION posWarp;

					if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire(GetMapIndex(), GetEmpire(), posWarp))
					{
						// CONSUME_LIFE_WHEN_USE_WARP_ITEM
						PointChange(POINT_HP, -consumeLife, false);
						// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

						WarpSet(posWarp.x, posWarp.y);
					}
					else
					{
						sys_err("CHARACTER::UseItem : cannot find spawn position (name %s, %d x %d)", GetName(), GetX(), GetY());
					}
				}
				else
				{
					if (test_server)
						ChatPacket(CHAT_TYPE_INFO, "233");

					ProcessRecallItem(item);
				}
			}
			else if (item->GetValue(0) == MEMORY_PORTAL) // Â±??Â¯Â±Ã¢Â¾Ã¯Âº?
			{
				if (item->GetSocket(0) == 0)
				{
					if (GetDungeon())
					{
						ChatPacket(CHAT_TYPE_INFO, "234 %s%s",
							item->GetName(),
							"");
						return false;
					}

					if (!GiveRecallItem(item))
						return false;
				}
				else
				{
					// CONSUME_LIFE_WHEN_USE_WARP_ITEM
					PointChange(POINT_HP, -consumeLife, false);
					// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

					ProcessRecallItem(item);
				}
			}
		}
		break;

		case USE_TUNING:
		case USE_DETACHMENT:
		{
			LPITEM item2;

			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
				return false;

#ifdef __SASH_SYSTEM__
			if (item->GetValue(0) == SASH_CLEAN_ATTR_VALUE0)
			{
				if (!CleanSashAttr(item, item2))
					return false;

				return true;
			}
#endif

			RefineItem(item, item2);
		}
		break;

		case USE_CHANGE_BOOSTER_ATTR:
		case USE_ADD_BOOSTER_ATTR:
		{
			CItem* targetItem;
			if (!IsValidItemPosition(DestCell) ||
				!(targetItem = GetItem(DestCell)))
				return false;

			switch (item->GetSubType())
			{
			case USE_ADD_BOOSTER_ATTR:
				if (!UseItemBoosterAddAttribute(*item, *targetItem))
					return false;

				break;

			case USE_CHANGE_BOOSTER_ATTR:
				if (!UseItemBoosterChangeAttribute(*item, *targetItem))
					return false;

				break;
			}
		}
		break;

		case USE_CHANGE_COSTUME_ATTR:
		case USE_RESET_COSTUME_ATTR:
		{
			LPITEM item2;
			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsEquipped())
			{
				BuffOnAttr_RemoveBuffsFromItem(item2);
			}

			if (ITEM_COSTUME != item2->GetType())
			{
				ChatPacket(CHAT_TYPE_INFO, "223");
				return false;
			}
#ifdef ENABLE_SOULBIND_SYSTEM
			if (item2->IsSealed())
			{
				ChatPacket(CHAT_TYPE_INFO, "237");
				return false;
			}
#endif
			if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
				return false;

			if (item2->GetAttributeSetIndex() == -1)
			{
				ChatPacket(CHAT_TYPE_INFO, "223");
				return false;
			}

			if (CheckVnumCostumeBlock(item2->GetVnum()))
			{
				ChatPacket(CHAT_TYPE_INFO, "A este objeto no se le puede a?dir o cambiar el bonus.");
				return false;
			}


#ifdef ADD_BONUS_INTO_COSTUMES


			item2->SetAttrRings(false);

			if (item->GetVnum() == 70063)
			{
				DWORD bonus_count = 3;

				std::vector<DWORD> restrictedVnums = { 40457, 40460, 40458, 40461, 40462, 45160, 45161, 40459, 40576, 41324, 41325, 40578, 40579, 49042, 49037, 49038, 49039, 49040, 49041, 49290, 49291, 49292, 49293, 49294, 49295, 49296, 49297, 49298, 49299 };

				if (std::find(restrictedVnums.begin(), restrictedVnums.end(), item2->GetVnum()) != restrictedVnums.end())
					return false;

				if (item2->GetVnum() >= 73001 and item2->GetVnum() <= 73012 or item2->GetVnum() >= 73251 and item2->GetVnum() <= 73262 or item2->GetVnum() >= 73501 and item2->GetVnum() <= 73512 or item2->GetVnum() >= 73751 and item2->GetVnum() <= 73762)
					return false;

				if (item2->GetAttributeCount() < bonus_count)
				{
					if (item2->GetSubType() == COSTUME_HAIR)
						item2->AddAttrCostumeHair();
					else
						item2->AddAttrCostume();
					ChatPacket(CHAT_TYPE_INFO, "238"); // se ha agregado con exito
#ifdef BATTLEPASS_WORLDARD
					CBattlePass::instance().UpdateMisionBattlePass(this, ADD_COSTUME_ATTR_BP, 1);
#endif	

					item->SetCount(item->GetCount() - 1);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, "239");
				}
			}
			else if (item->GetVnum() == 70064)
			{
				if (item2->GetAttributeCount() == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "240");
					return false;
				}

				if (item2->GetSubType() == COSTUME_HAIR)
					item2->ChangeAttrCostumeHair();
				else
					item2->ChangeAttrCostume();
#ifdef BATTLEPASS_WORLDARD
				CBattlePass::instance().UpdateMisionBattlePass(this, CHANGE_COSTUME_ATTR_BP, 1);
#endif	

				item->SetCount(item->GetCount() - 1);
				ChatPacket(CHAT_TYPE_INFO, "241");
			}
			break;
#else
			switch (item->GetSubType())
			{
			case USE_CHANGE_COSTUME_ATTR:
				if (item2->GetSubType() == COSTUME_HAIR)
				{

					item2->ChangeAttrCostumeHair();
				}
				else
				{
					item2->ChangeAttrCostume();
				}
#ifdef BATTLEPASS_WORLDARD
				CBattlePass::instance().UpdateMisionBattlePass(this, CHANGE_COSTUME_ATTR_BP, 1);
#endif
				//item2->ChangeAttribute();
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "CHANGE_COSTUME_ATTR", buf);
				}
				break;
			case USE_RESET_COSTUME_ATTR:
				item2->ClearAttribute();
				item2->AlterToMagicItem();
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "RESET_COSTUME_ATTR", buf);
				}
				break;
			}
			ChatPacket(CHAT_TYPE_INFO, "241");

			item->SetCount(item->GetCount() - 1);
			break;
#endif
		}
		//  ACCESSORY_REFINE & ADD/CHANGE_ATTRIBUTES
		case USE_PUT_INTO_BELT_SOCKET:
		case USE_PUT_INTO_RING_SOCKET:
		case USE_PUT_INTO_ACCESSORY_SOCKET:
		case USE_ADD_ACCESSORY_SOCKET:
		case USE_CLEAN_SOCKET:
		case USE_CHANGE_ATTRIBUTE:

		case USE_CHANGE_ATTRIBUTE2:
		case USE_ADD_ATTRIBUTE:
		case USE_ADD_ATTRIBUTE2:
		case USE_ADD_BOOSTER_TIME:
#ifdef __AURA_SYSTEM__
		case USE_PUT_INTO_AURA_SOCKET:
#endif

		{
#ifdef ENABLE_LOCK_ATTR
			if (item->GetVnum() == 50348 || item->GetVnum() == 50349)
			{
				LPITEM item3;
				if (!IsValidItemPosition(DestCell) || !(item3 = GetItem(DestCell)))
				{
					quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
					return true;
				}
			}
#endif

#ifdef ENABLE_COPY_ATTR_ITEM
			if (item->GetVnum() == 77927)
			{
				LPITEM item2;
				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				{
					for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
						item->SetForceAttribute(j, 0, 0);
					return false;
				}
				if (item2->IsExchanging() || item2->IsEquipped())
					return false;
				if (item2->CanCopyAttrItem())
				{
					bool isHasAlreadyAttr = false;
					for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
					{
						if (item->GetAttribute(j).bType > 0)
						{
							isHasAlreadyAttr = true;
							break;
						}
					}
					if (isHasAlreadyAttr)
					{
						if (item->GetSocket(1) == ITEM_WEAPON)
						{
							if (!(item->GetSocket(1) == item2->GetType() && item->GetSocket(3) == item2->GetProto()->sAddonType))
							{
								ChatPacket(CHAT_TYPE_INFO, "You can't add attr for this item!");
								return false;
							}
						}
						else
						{
							if (!(item->GetSocket(1) == item2->GetType() && item->GetSocket(2) == item2->GetSubType() && item->GetSocket(3) == item2->GetProto()->sAddonType))
							{
								ChatPacket(CHAT_TYPE_INFO, "You can't add attr for this item!");
								return false;
							}
						}

						item2->SetAttributes(item->GetAttributes());
						item2->UpdatePacket();
						item->SetCount(item->GetCount() - 1);
						ChatPacket(CHAT_TYPE_INFO, "Succesfully added attr.");
					}
					else
					{
						isHasAlreadyAttr = false;
						for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
						{
							if (item2->GetAttribute(j).bType > 0)
							{
								isHasAlreadyAttr = true;
								break;
							}
						}
						if (!isHasAlreadyAttr)
						{
							ChatPacket(CHAT_TYPE_INFO, "This item not has any attribute. Please try with has attr item.");
							return false;

						}
						item->SetSocket(1, item2->GetType());
						item->SetSocket(2, item2->GetSubType());
						item->SetSocket(3, item2->GetProto()->sAddonType);
						item->SetAttributes(item2->GetAttributes());
						item->UpdatePacket();
						for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
							item2->SetForceAttribute(j, 0, 0);
						ChatPacket(CHAT_TYPE_INFO, "Succesfully saving attr in item");
					}
				}
				else
					ChatPacket(CHAT_TYPE_INFO, "You can't use on this!");

				return true;
			}
#endif							

			LPITEM item2;
			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			// if (item->GetVnum() == 71068)
			// {
					// if (item2->GetSocketCount() != 3 && item2->GetSocketCount() < 5)
					// {
						// ChatPacket(1, "The item already has %d sockets.", item2->GetSocketCount());
						// return false;
					// }

					// ChatPacket(1, "DEBUG: Item2 has %d sockets", item2->GetSocketCount());

					// item2->SetSocket(3, 1);

					// ChatPacket(1, "AFTER: Item2 has %d sockets", item2->GetSocketCount());

					// item->SetCount(item->GetCount() - 1);
					// ChatPacket(CHAT_TYPE_INFO, "Succesfully added 4th stone socket.");
				// return true;
			// }


			if (item2->IsEquipped())
			{
				BuffOnAttr_RemoveBuffsFromItem(item2);
			}
#ifdef __SOULBINDING_SYSTEM__
			if (item2->IsBind() && item->GetSubType() != USE_PUT_INTO_BELT_SOCKET && item->GetSubType() != USE_PUT_INTO_RING_SOCKET && item->GetSubType() != USE_PUT_INTO_ACCESSORY_SOCKET && item->GetSubType() != USE_ADD_ACCESSORY_SOCKET)
			{
				ChatPacket(CHAT_TYPE_INFO, "242");
				return false;
			}

			if (item2->IsUntilBind() && item->GetSubType() != USE_PUT_INTO_BELT_SOCKET && item->GetSubType() != USE_PUT_INTO_RING_SOCKET && item->GetSubType() != USE_PUT_INTO_ACCESSORY_SOCKET && item->GetSubType() != USE_ADD_ACCESSORY_SOCKET)
			{
				ChatPacket(CHAT_TYPE_INFO, "243");
				return false;
			}
#endif
			// [NOTE] ??Â½Âº?Â¬ Â¾?????Â¿Â¡Â´? Â¾????? ???? Â»Ã½Â¼ÂºÂ½? Â·Â£Â´Ã½ Â¼?Â¼Âº?Â» Âº?Â¿Â©??Âµ?, ?Ã§Â°Ã¦?Ã§Â°Â¡ ÂµÃ®ÂµÃ®?Âº Â¸Â·Â¾?Â´?Â¶Ã³Â´? Â¿Ã¤?Â»?? ??Â¾Ãº?Â½.
			// Â¿Ã¸Â·Â¡ ANTI_CHANGE_ATTRIBUTE Â°Â°?Âº Â¾????? FlagÂ¸Â¦ ??Â°Â¡??Â¿Â© Â±Ã¢?Â¹ Â·Â¹ÂºÂ§Â¿Â¡Â¼Â­ ?Â¯Â¿Â¬??Â°? ???Â®Â·? ?? Â¼Ã¶ ??ÂµÂµÂ·? ?? Â¿Â¹?Â¤??Â¾Ãº?Â¸Â³Âª
			// Â±?ÂµÃ½Â°? ??Â¿Ã¤Â¾Ã¸?Â¸Â´? Â´??Â¡Â°Ã­ Â»Â¡Â¸Â® ??Â´?Â·Â¡Â¼Â­ Â±?Â³? Â¿Â©Â±Ã¢Â¼Â­ Â¸Â·?Â½... -_-
#ifndef ENABLE_SPECIAL_COSTUME_ATTR
#if defined(__BL_67_ATTR__)
			if (ITEM_COSTUME == item2->GetType() && item->GetSubType() != USE_CHANGE_ATTRIBUTE2 && !item2->IsBooster())
#else
			if (ITEM_COSTUME == item2->GetType())
#endif
#ifdef __AURA_SYSTEM__
				if (item->GetSubType() != USE_PUT_INTO_AURA_SOCKET)
#endif
				{
					ChatPacket(CHAT_TYPE_INFO, "223");
					return false;
				}
#endif
			if (item2->IsBooster() &&
				item->GetSubType() != USE_ADD_BOOSTER_TIME)
			{
				ChatPacket(CHAT_TYPE_INFO, "223");
				return false;
			}

			if (item2->IsExchanging() || (item2->IsEquipped() && !item2->IsPendantSoul()))
				return false;
			if (!item->CheckEnchantToItem(item2))
			{
				ChatPacket(CHAT_TYPE_INFO, "You can't use on this item!");
				return false;
			}

			item2->SetAttrRings(false);

			switch (item->GetSubType())
			{
			case USE_ADD_BOOSTER_TIME:
			{
				if (!item2->IsRealTimeItem() && item2->GetProto()->cLimitRealTimeFirstUseIndex < 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "You cannot charge the time of this item.");
					return false;
				}

				if (item2->GetType() != ITEM_BOOSTER) {
					ChatPacket(CHAT_TYPE_INFO, "It can be used only on Boosters.");
					return false;
				}

				if (!item2->IsRealTimeItem() && item2->GetSocket(1) == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "You have to use it at least one time before charging this item.");
					return false;
				}

				// begin real time check
				bool bHasRealTime = false;
				for (BYTE i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
				{
					const BYTE bType = item2->GetProto()->aLimits[i].bType;
					if (bType == LIMIT_REAL_TIME || bType == LIMIT_REAL_TIME_START_FIRST_USE)
					{
						bHasRealTime = true;
						break;
					}
				}

				auto remained_time = item2->GetSocket(ITEM_SOCKET_REMAIN_SEC);

				// Check if the current time plus the added time would exceed the limit
				const DWORD added_time = item->GetValue(1);
				// end real time check

				if (remained_time <= get_global_time()) {
					item2->SetSocket(ITEM_SOCKET_REMAIN_SEC, get_global_time());
					remained_time = get_global_time();
				}

				auto new_time = remained_time + added_time;

				// Update the socket with the new time
				item2->SetSocket(ITEM_SOCKET_REMAIN_SEC, new_time);
				if (new_time > get_global_time()) {
					item2->Lock(false);
				}
				item->SetCount(item->GetCount() - 1);

				ChatPacket(CHAT_TYPE_INFO, "The time of %s was charged.", item2->GetName());
			}
			break;

			case USE_CLEAN_SOCKET:
			{
				//const BYTE maxSocketforMetin = ITEM_SOCKET_MAX_NUM;
				const BYTE maxSocketforMetin = 3;
				bool isHasBrokeStone = false;
				std::vector<long> metinSlot;
				for (BYTE i = 0; i < ITEM_STONES_MAX_NUM; ++i)
				{
					if (item2->GetSocket(i) == ITEM_BROKEN_METIN_VNUM)
						isHasBrokeStone = true;
					else
						metinSlot.push_back(item2->GetSocket(i));
				}
				if (!isHasBrokeStone)
				{
					ChatPacket(CHAT_TYPE_INFO, "244");
					return false;
				}
				for (BYTE i = 0; i < ITEM_STONES_MAX_NUM; ++i)
				{
					if (metinSlot.size() >= i + 1)
						item2->SetSocket(i, metinSlot[i]);
					else
						item2->SetSocket(i, 1);
				}

				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "CLEAN_SOCKET", buf);
				}

				item->SetCount(item->GetCount() - 1);

			}
			break;

			case USE_CHANGE_ATTRIBUTE:
				//case USE_CHANGE_ATTRIBUTE2 : // @fixme123

#ifdef ENABLE_LOCK_ATTR
				if (item->GetVnum() == 50348 || item->GetVnum() == 50349)
				{
					if (item->GetVnum() == 50348)// lock algorithm
					{
						const int lockItemIndex = item->GetSocket(0);
						if (lockItemIndex == 0 || lockItemIndex > ITEM_ATTRIBUTE_MAX_LEVEL) // test pe norm num
						{
							ChatPacket(CHAT_TYPE_INFO, "[Attr Lock]Uncorrect lock attr item index. Please select your want index!");
							return false;
						}
						const BYTE attrCount = item2->GetAttributeCount();
						if (attrCount != 5)
						{
							ChatPacket(CHAT_TYPE_INFO, "[Attr Lock]Item need 5 attr count!");
							return false;
						}
						BYTE attrIndex = 0;
						if (item2->GetAttrLockIndex(0) == 0)
							attrIndex = 0;
						else if (item2->GetAttrLockIndex(1) == 0)
							attrIndex = 1;
						else
						{
							ChatPacket(CHAT_TYPE_INFO, "[Attr Lock]This item already have max lock attr index!");
							return false;
						}
						if (item2->GetAttrLockIndex(0) == lockItemIndex)
						{
							ChatPacket(CHAT_TYPE_INFO, "You can't add same index!");
							return false;
						}
						if (item2->IsAddonItem() && (lockItemIndex == 1 || lockItemIndex == 2) && (item2->GetAttrLockIndex(0) == 1 || item2->GetAttrLockIndex(0) == 2))
						{
							ChatPacket(CHAT_TYPE_INFO, "Addon item already blocked 1 and 2 index attr!");
							return false;
						}
						item2->SetForceAttribute(LOCK_ATTR_DATA_INDEX + attrIndex, 0, lockItemIndex);
						item->SetCount(item->GetCount() - 1);
						ChatPacket(CHAT_TYPE_INFO, "[Attr Lock]Lock attr index successfully!");
					}
					else//remove lock
					{
						const int removeIndex = item->GetSocket(0);
						if (removeIndex == 0)
						{
							ChatPacket(CHAT_TYPE_INFO, "[Attr Lock]You need select your want remove index!");
							return false;
						}
						BYTE attrIndex = 0;
						if (item2->GetAttrLockIndex(0) == removeIndex)
							attrIndex = 0;
						else if (item2->GetAttrLockIndex(1) == removeIndex)
							attrIndex = 1;
						else
						{
							ChatPacket(CHAT_TYPE_INFO, "[Attr Lock]This item don't have lock attr index!");
							return false;
						}
						item2->SetForceAttribute(LOCK_ATTR_DATA_INDEX + attrIndex, 0, 0);
						item->SetCount(item->GetCount() - 1);
						ChatPacket(CHAT_TYPE_INFO, "[Attr Lock]Removing Lock attr index successfully!");
					}
					return true;
				}
#endif

				{
					DWORD dwTargetVnum = item2->GetVnum(); //zodiac
					bool bZodiacItem = (
						((dwTargetVnum >= 19290) && (dwTargetVnum <= 19312)) ||
						((dwTargetVnum >= 19490) && (dwTargetVnum <= 19512)) ||
						((dwTargetVnum >= 19690) && (dwTargetVnum <= 19712)) ||
						((dwTargetVnum >= 19890) && (dwTargetVnum <= 19912)) ||
						((dwTargetVnum >= 300) && (dwTargetVnum <= 319)) ||
						(dwTargetVnum == 329) ||
						(dwTargetVnum == 339) ||
						(dwTargetVnum == 349) ||
						(dwTargetVnum == 359) ||
						(dwTargetVnum == 369) ||
						(dwTargetVnum == 379) ||
						(dwTargetVnum == 389) ||
						(dwTargetVnum == 399) ||
						((dwTargetVnum >= 1180) && (dwTargetVnum <= 1189)) ||
						(dwTargetVnum == 1199) ||
						(dwTargetVnum == 1209) ||
						(dwTargetVnum == 1219) ||
						(dwTargetVnum == 1229) ||
						((dwTargetVnum >= 2200) && (dwTargetVnum <= 2209)) ||
						(dwTargetVnum == 2219) ||
						(dwTargetVnum == 2229) ||
						(dwTargetVnum == 2239) ||
						(dwTargetVnum == 2249) ||
						((dwTargetVnum >= 3220) && (dwTargetVnum <= 3229)) ||
						(dwTargetVnum == 3239) ||
						(dwTargetVnum == 3249) ||
						(dwTargetVnum == 3259) ||
						(dwTargetVnum == 3269) ||
						((dwTargetVnum >= 5160) && (dwTargetVnum <= 5169)) ||
						(dwTargetVnum == 5179) ||
						(dwTargetVnum == 5189) ||
						(dwTargetVnum == 5199) ||
						(dwTargetVnum == 5209) ||
						((dwTargetVnum >= 7300) && (dwTargetVnum <= 7309)) ||
						(dwTargetVnum == 7319) ||
						(dwTargetVnum == 7329) ||
						(dwTargetVnum == 7339) ||
						(dwTargetVnum == 7349)
						) ? true : false;
					if (item->GetVnum() != 94912)
					{
						if (bZodiacItem)
						{
							ChatPacket(CHAT_TYPE_INFO, "You can't use this item on %s, it is just for normal items.", item2->GetName());
							return false;
						}
					}
					else if (!bZodiacItem)
					{
						ChatPacket(CHAT_TYPE_INFO, "You can't use this item on %s, it is just for zodiac items.", item2->GetName());
						return false;
					}
				}

				if ((item2->GetType() == ITEM_DS && item->GetVnum() != 71097) || (item2->GetType() != ITEM_DS && item->GetVnum() == 71097))
				{
					ChatPacket(CHAT_TYPE_INFO, "1055");
					return false;
				}

#ifdef ENABLE_SPECIAL_COSTUME_ATTR
				{
					const DWORD bonusItemVnum = item->GetVnum();
					std::vector<DWORD> restrictedVnums = { 40457, 40460, 40458, 40461, 40462, 45160, 45161, 40459, 41324, 41325, 40576, 40577, 40578, 40579, 49042, 49037, 49038, 49039, 49040, 49041, 49290, 49291, 49292, 49293, 49294, 49295, 49296, 49297, 49298, 49299 };

					if (std::find(restrictedVnums.begin(), restrictedVnums.end(), item2->GetVnum()) != restrictedVnums.end())
						return false;

					bool isPendant = (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_GLOVE);
					if ((isPendant == true && bonusItemVnum != 71302) || (isPendant == false && bonusItemVnum == 71302))
					{
						ChatPacket(CHAT_TYPE_INFO, "1056");
						return false;
					}
					if (bonusItemVnum == 71302)
					{
						item2->ChangeAttribute();
						item->SetCount(item->GetCount() - 1);
						ChatPacket(CHAT_TYPE_INFO, "1067");
						return true;
					}

					bool itsCostumeAttr = (bonusItemVnum >= 53998 && bonusItemVnum <= 54500);
					if ((itsCostumeAttr == true && !item2->CanBonusCostume()) || (itsCostumeAttr == false && item2->CanBonusCostume()))
					{
						ChatPacket(CHAT_TYPE_INFO, "1056");
						return false;
					}

					if (bonusItemVnum == 53998)
					{
						bool isChange = false;
						for (int j = ITEM_ATTRIBUTE_MAX_LEVEL - 1; j >= 0; --j)
						{
							if (item2->GetAttributeType(j) != APPLY_NONE)
							{
								isChange = true;
								item->SetCount(item->GetCount() - 1);
								item2->SetForceAttribute(j, 0, 0);
								break;
							}
						}
						if (isChange)
							ChatPacket(CHAT_TYPE_INFO, "1058");
						else
							ChatPacket(CHAT_TYPE_INFO, "1060");
						return true;
					}
					else if (bonusItemVnum == 53999)
					{
						bool isChange = false;
						for (int j = 0; j < ITEM_ATTRIBUTE_MAX_LEVEL; ++j)
						{
							if (item2->GetAttributeType(j) != APPLY_NONE)
							{
								isChange = true;
								item2->SetForceAttribute(j, 0, 0);
							}
						}
						if (isChange)
						{
							item->SetCount(item->GetCount() - 1);
							ChatPacket(CHAT_TYPE_INFO, "1059");
						}
						else
							ChatPacket(CHAT_TYPE_INFO, "1060");
						return true;
					}
					else if (bonusItemVnum >= 53998 && bonusItemVnum <= 54500)
					{
						const BYTE applyType = item->GetValue(0);
						const long applyValue = item->GetValue(1);
						if (applyType == APPLY_NONE)
						{
							ChatPacket(CHAT_TYPE_INFO, "1061");
							return false;
						}
						const BYTE subTypeFlag = item2->GetBonusFlag();
						if (item->GetValue(2) != 0 && subTypeFlag != 0)
						{
							if (!IS_SET(item->GetValue(2), 1 << subTypeFlag))
							{
								ChatPacket(CHAT_TYPE_INFO, "1056");
								return false;
							}
						}
						const BYTE maxAttrCount = item2->GetBonusMaxAttr();
						if (item2->GetAttributeCount() >= maxAttrCount)
						{
							ChatPacket(CHAT_TYPE_INFO, "1062");
							return false;
						}
						else if (item2->HasAttr(applyType))
						{
							ChatPacket(CHAT_TYPE_INFO, "1063");
							return false;
						}
						item2->AddAttribute(applyType, applyValue);
						ChatPacket(CHAT_TYPE_INFO, "1057");
						item->SetCount(item->GetCount() - 1);
						return true;
					}
				}
#endif

				if (item2->GetAttributeSetIndex() == -1 && item2->GetType() != ITEM_DS)
				{
					ChatPacket(CHAT_TYPE_INFO, "223");
					return false;
				}

				if (item2->GetAttributeCount() == 0 && item2->GetType() != ITEM_DS)
				{
					ChatPacket(CHAT_TYPE_INFO, "240");
					return false;
				}



				if ((GM_PLAYER == GetGMLevel()) && (false == test_server) && (g_dwItemBonusChangeTime > 0))
				{
					//
					// Event Flag Â¸Â¦ ?Ã«?? ???Ã¼Â¿Â¡ Â¾????? Â¼?Â¼Âº ÂºÂ¯Â°Ã¦?Â» ?? Â½?Â°Â£?Â¸Â·? Âº??? ?Ã¦Âº??? Â½?Â°Â£?? ?ÃªÂ·Â¶Â´??Ã¶ Â°?Â»Ã§??Â°Ã­
					// Â½?Â°Â£?? ?Ã¦Âº??Ã· ?ÃªÂ·Â¶Â´?Â¸Ã© ?Ã¶?Ã§ Â¼?Â¼ÂºÂºÂ¯Â°Ã¦Â¿Â¡ Â´Ã«?? Â½?Â°Â£?Â» Â¼Â³?Â¤?? ??Â´?.
					//

					// DWORD dwChangeItemAttrCycle = quest::CQuestManager::instance().GetEventFlag(msc_szChangeItemAttrCycleFlag);
					// if (dwChangeItemAttrCycle < msc_dwDefaultChangeItemAttrCycle)
						// dwChangeItemAttrCycle = msc_dwDefaultChangeItemAttrCycle;
					DWORD dwChangeItemAttrCycle = g_dwItemBonusChangeTime;

					quest::PC* pPC = quest::CQuestManager::instance().GetPC(GetPlayerID());

					if (pPC)
					{
						DWORD dwNowSec = get_global_time();

						DWORD dwLastChangeItemAttrSec = pPC->GetFlag(msc_szLastChangeItemAttrFlag);

						if (dwLastChangeItemAttrSec + dwChangeItemAttrCycle > dwNowSec)
						{
							ChatPacket(CHAT_TYPE_INFO, "245 %d %d",
								dwChangeItemAttrCycle, dwChangeItemAttrCycle - (dwNowSec - dwLastChangeItemAttrSec));
							return false;
						}

						pPC->SetFlag(msc_szLastChangeItemAttrFlag, dwNowSec);
					}
				}

				if (item->GetSubType() == USE_CHANGE_ATTRIBUTE2)
				{
					int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
					{
						0, 0, 30, 40, 3
					};
					if (item2->GetType() == ITEM_NEW_RINGS)
					{
						item2->SetAttrRings(true);
					}
					else
					{
						item2->SetAttrRings(false);
					}
					item2->ChangeAttribute(aiChangeProb);

				}
				else if (item->GetVnum() == 76014)
				{
					int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
					{
						0, 10, 50, 39, 1
					};
					if (item2->GetType() == ITEM_NEW_RINGS)
					{
						item2->SetAttrRings(true);
					}
					else
					{
						item2->SetAttrRings(false);
					}
					item2->ChangeAttribute(aiChangeProb);
				}
				else if (item->GetVnum() == 76514)
				{
					int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
					{
						0, 10, 50, 39, 1
					};
					if (item2->GetType() == ITEM_NEW_RINGS)
					{
						item2->SetAttrRings(true);
					}
					else
					{
						item2->SetAttrRings(false);
					}
					item2->ChangeAttributeSpecial(aiChangeProb);
				}

				else
				{
					// Â¿Â¬?Ã§Â°Ã¦ ?Â¯Â¼Ã¶?Â³Â¸Â®
					// ?Ã½Â´Ã«Â·? Â¿Â¬?Ã§Â°Â¡ ??Â°Â¡ Â¾?Âµ?Â°?Â¶Ã³ ??Â¿Â© ??ÂµÃ¥ ??ÂµÃ¹??.
					if (item->GetVnum() == 71151 || item->GetVnum() == 76023)
					{
						if ((item2->GetType() == ITEM_WEAPON)
							|| (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY))
						{
							bool bCanUse = true;
							for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
							{
#ifdef ENABLE_YOHARA_SYSTEM
								if (item2->GetLimitType(i) == LIMIT_CONQUEROR_LEVEL && item2->GetLimitValue(i) > 0)
								{
									bCanUse = false;
									break;
								}
#endif

								if (item2->GetLimitType(i) == LIMIT_LEVEL && item2->GetLimitValue(i) > 40)
								{
									bCanUse = false;
									break;
								}
							}
							if (false == bCanUse)
							{
								ChatPacket(CHAT_TYPE_INFO, "246");
								break;
							}
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, "247");
							break;
						}
					}

				}


				if (item2->GetType() == ITEM_DS && item->GetVnum() == 71097)
				{
					//ChatPacket(CHAT_TYPE_INFO, "prueba funcion exitosa");
					if (DSManager::instance().ChangeAttributes(this, item2))
					{
						item->SetCount(item->GetCount() - 1);
					}
					break;
				}

				else
				{
					if (item2->GetType() == ITEM_NEW_RINGS)
					{
						item2->SetAttrRings(true);
					}
					else
					{
						item2->SetAttrRings(false);
					}
					item2->ChangeAttribute();
#ifdef BATTLEPASS_WORLDARD
					CBattlePass::instance().UpdateMisionBattlePass(this, CHANGE_ATTR_BP, 1);
#endif	
					ChatPacket(CHAT_TYPE_INFO, "241");
					{
						char buf[21];
						snprintf(buf, sizeof(buf), "%u", item2->GetID());
						LogManager::instance().ItemLog(this, item, "CHANGE_ATTRIBUTE", buf);
					}
					item->SetCount(item->GetCount() - 1);
#ifdef ENABLE_RANKING
					if (item->GetVnum() == 86051 || item->GetVnum() == 88965)
						SetRankPoints(13, GetRankPoints(13) + 1);
					else
						SetRankPoints(12, GetRankPoints(12) + 1);
#endif
					break;
				}



				break;

			case USE_ADD_ATTRIBUTE:
				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, "223");
					return false;
				}

#ifdef ENABLE_SOULBIND_SYSTEM
				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, "237");
					return false;
				}
#endif

				if (item2->GetAttributeCount() < 5)
				{
#ifdef ENABLE_SPECIAL_COSTUME_ATTR
					{
						const DWORD bonusItemVnum = item->GetVnum();
						bool isPendant = (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_GLOVE);
						if ((isPendant == true && bonusItemVnum != 71301) || (isPendant == false && bonusItemVnum == 71301))
						{
							ChatPacket(CHAT_TYPE_INFO, "1056");
							return false;
						}
						if (bonusItemVnum == 71301)
						{
							item2->AddAttribute();
							item->SetCount(item->GetCount() - 1);
							ChatPacket(CHAT_TYPE_INFO, "1057");
							return true;
						}
					}
#endif

					// Â¿Â¬?Ã§Â°Â¡ ?Â¯Â¼Ã¶?Â³Â¸Â®
					// ?Ã½Â´Ã«Â·? Â¿Â¬?Ã§Â°Â¡ ??Â°Â¡ Â¾?Âµ?Â°?Â¶Ã³ ??Â¿Â© ??ÂµÃ¥ ??ÂµÃ¹??.
					if (item->GetVnum() == 71152 || item->GetVnum() == 76024)
					{
#ifdef ITEM_TALISMAN_EQUIPMENT
						if ((item2->GetType() == ITEM_WEAPON)
							|| ((item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY) || (item2->GetType() == ITEM_COSTUME && item2->GetSubType() == COSTUME_SASH)))
#else
						if ((item2->GetType() == ITEM_WEAPON)
							|| (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY))
#endif
						{
							bool bCanUse = true;
							for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
							{
#ifdef ENABLE_YOHARA_SYSTEM
								if (item2->GetLimitType(i) == LIMIT_CONQUEROR_LEVEL && item2->GetLimitValue(i) > 0)
								{
									bCanUse = false;
									break;
								}
#endif

								if (item2->GetLimitType(i) == LIMIT_LEVEL && item2->GetLimitValue(i) > 40)
								{
									bCanUse = false;
									break;
								}
							}
							if (false == bCanUse)
							{
								ChatPacket(CHAT_TYPE_INFO, "246");
								break;
							}
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, "247");
							break;
						}
					}
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());

					if (item2->GetType() == ITEM_NEW_RINGS)
					{
						item2->SetAttrRings(true);
					}
					else
					{
						item2->SetAttrRings(false);
					}
					if (number(1, 100) <= aiItemAttributeAddPercent[item2->GetAttributeCount()])
					{
#ifdef ENABLE_MAX_ADD_ATTRIBUTE
						short AttributeCount = abs(5 - item->GetAttributeCount());
						for (int i = 0; i < AttributeCount; i++)
							item2->AddAttribute();
#else
						item2->AddAttribute();
#endif
						ChatPacket(CHAT_TYPE_INFO, "248");

						int iAddedIdx = item2->GetAttributeCount() - 1;
						LogManager::instance().ItemLog(
							GetPlayerID(),
							item2->GetAttributeType(iAddedIdx),
							item2->GetAttributeValue(iAddedIdx),
							item->GetID(),
							"ADD_ATTRIBUTE_SUCCESS",
							buf,
							GetDesc()->GetHostName(),
							item->GetOriginalVnum());
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, "249");
						LogManager::instance().ItemLog(this, item, "ADD_ATTRIBUTE2_FAIL", buf);
					}
					// item->SetCount(item->GetCount() - 1);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, "239");
				}
				break;

#if defined(__BL_67_ATTR__)
			case USE_CHANGE_ATTRIBUTE2:
				if (item2->GetAttributeSetIndex() == -1 || item2->GetRareAttrCount() == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "229");
					return false;
				}

				if (item2->IsEquipped())
					return false;

				if ((item->GetVnum() == SMALL_ORISON && number(1, 100) >= 10) == false
					&& item2->ChangeRareAttribute2() == true)
					ChatPacket(CHAT_TYPE_INFO, "227");
				else
					ChatPacket(CHAT_TYPE_INFO, "228");

				item->SetCount(item->GetCount() - 1);
				break;
#endif

			case USE_ADD_ACCESSORY_SOCKET:
			{
				char buf[21];
				snprintf(buf, sizeof(buf), "%u", item2->GetID());

				if (item2->IsAccessoryForSocket())
				{
					if (item2->GetAccessorySocketMaxGrade() < ITEM_ACCESSORY_SOCKET_MAX_NUM)
					{
#ifdef ENABLE_ADDSTONE_FAILURE
						if (number(1, 100) <= 100)
#else
						if (1)
#endif
						{
							item2->SetAccessorySocketMaxGrade(item2->GetAccessorySocketMaxGrade() + 1);
							ChatPacket(CHAT_TYPE_INFO, "238");
							LogManager::instance().ItemLog(this, item, "ADD_SOCKET_SUCCESS", buf);
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, "249");
							LogManager::instance().ItemLog(this, item, "ADD_SOCKET_FAIL", buf);
						}

						item->SetCount(item->GetCount() - 1);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, "250");
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, "251");
				}
			}
			break;

			case USE_PUT_INTO_BELT_SOCKET:
			case USE_PUT_INTO_ACCESSORY_SOCKET:
				if (item2->IsAccessoryForSocket() && item->CanPutInto(item2))
				{

					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());

					if (item2->GetAccessorySocketGrade() < item2->GetAccessorySocketMaxGrade())
					{
						if (number(1, 100) <= aiAccessorySocketPutPct[item2->GetAccessorySocketGrade()])
						{
							item2->SetAccessorySocketGrade(item2->GetAccessorySocketGrade() + 1);
							ChatPacket(CHAT_TYPE_INFO, "252");
							LogManager::instance().ItemLog(this, item, "PUT_SOCKET_SUCCESS", buf);
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, "253");
							LogManager::instance().ItemLog(this, item, "PUT_SOCKET_FAIL", buf);
						}

						item->SetCount(item->GetCount() - 1);
					}
					else
					{
						if (item2->GetAccessorySocketMaxGrade() == 0)
							ChatPacket(CHAT_TYPE_INFO, "254");
						else if (item2->GetAccessorySocketMaxGrade() < ITEM_ACCESSORY_SOCKET_MAX_NUM)
						{
							ChatPacket(CHAT_TYPE_INFO, "255");
							ChatPacket(CHAT_TYPE_INFO, "256");
						}
						else
							ChatPacket(CHAT_TYPE_INFO, "257");
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, "258");
				}
				break;
#ifdef __AURA_SYSTEM__
			case USE_PUT_INTO_AURA_SOCKET:
			{
				if (item2->IsAuraBoosterForSocket() && item->CanPutInto(item2))
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());

					const BYTE c_bAuraBoostIndex = item->GetOriginalVnum() - ITEM_AURA_BOOST_ITEM_VNUM_BASE;
					item2->SetSocket(ITEM_SOCKET_AURA_BOOST, c_bAuraBoostIndex * 100000000 + item->GetValue(ITEM_AURA_BOOST_TIME_VALUE));

					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Aura> The aura booster has successfully connected."));

					LogManager::instance().ItemLog(this, item, "PUT_AURA_SOCKET", buf);

					if (IS_SET(item->GetFlag(), ITEM_FLAG_STACKABLE) && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK) && item->GetCount() > 1)
						item->SetCount(item->GetCount() - 1);
					else
						ITEM_MANAGER::instance().RemoveItem(item, "PUT_AURA_SOCKET_REMOVE");
				}
				else
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Aura> You cannot add an aura boost to this item."));
			}
			break;
#endif

			}
			if (item2->IsEquipped())
			{
				BuffOnAttr_AddBuffsFromItem(item2);
			}
		}
		break;
		//  END_OF_ACCESSORY_REFINE & END_OF_ADD_ATTRIBUTES & END_OF_CHANGE_ATTRIBUTES

		case USE_BAIT:
		{

			if (m_pkFishingEvent)
			{
				ChatPacket(CHAT_TYPE_INFO, "259");
				return false;
			}

			LPITEM weapon = GetWear(WEAR_WEAPON);

			if (!weapon || weapon->GetType() != ITEM_ROD)
				return false;

			if (weapon->GetSocket(2))
			{
				ChatPacket(CHAT_TYPE_INFO, "260 %s", item->GetName());
			}
			else
			{
				ChatPacket(CHAT_TYPE_INFO, "261 %s", item->GetName());
			}

			weapon->SetSocket(2, item->GetValue(0));
			item->SetCount(item->GetCount() - 1);
		}
		break;
#ifdef __SOULBINDING_SYSTEM__
		case USE_BIND:
		{
			LPITEM item2;
			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsEquipped())
			{
				BuffOnAttr_RemoveBuffsFromItem(item2);
			}

			if (item2->IsExchanging() || item2->IsEquipped())
				return false;

			if (item2->IsBind())
			{
				ChatPacket(CHAT_TYPE_INFO, "262");
				return false;
			}

			if (item2->IsUntilBind())
			{
				ChatPacket(CHAT_TYPE_INFO, "263");
				return false;
			}

			if (IS_SET(item2->GetAntiFlag(), ITEM_ANTIFLAG_BIND))
			{
				ChatPacket(CHAT_TYPE_INFO, "264");
				return false;
			}

			long time_proc = item->GetValue(3);
			item2->Bind(time_proc);
			ChatPacket(CHAT_TYPE_INFO, "265");
			item->SetCount(item->GetCount() - 1);
		}
		break;
		case USE_UNBIND:
		{
			LPITEM item2;
			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsEquipped())
			{
				BuffOnAttr_RemoveBuffsFromItem(item2);
			}

			if (item2->IsExchanging() || item2->IsEquipped())
				return false;

			if (item2->IsUntilBind())
			{
				ChatPacket(CHAT_TYPE_INFO, "263");
				return false;
			}

			if (!item2->IsBind())
			{
				ChatPacket(CHAT_TYPE_INFO, "266");
				return false;
			}

			long time_proc = item->GetValue(3);
			item2->Bind(time_proc);
			item2->StartUnBindingExpireEvent();
			ChatPacket(CHAT_TYPE_INFO, "267");
			item->SetCount(item->GetCount() - 1);
		}
		break;
#endif
		case USE_MOVE:
		case USE_TREASURE_BOX:
		case USE_MONEYBAG:
			break;

		case USE_AFFECT:
		{
			if (FindAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType))
			{
				ChatPacket(CHAT_TYPE_INFO, "175");
			}
			else
			{
				// PC_BANG_ITEM_ADD
				if (item->IsPCBangItem() == true)
				{
					// PCÂ¹Ã¦???Ã¶ ?Â¼?Â©??Â¼Â­ ?Â³Â¸Â®
					if (CPCBangManager::instance().IsPCBangIP(GetDesc()->GetHostName()) == false)
					{
						// PCÂ¹Ã¦?? Â¾?Â´?!
						ChatPacket(CHAT_TYPE_INFO, "176");
						return false;
					}
				}
				// END_PC_BANG_ITEM_ADD

				AddAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType, item->GetValue(2), 0, item->GetValue(3), 0, false);
				item->SetCount(item->GetCount() - 1);
			}
		}
		break;

		case USE_CREATE_STONE:
			AutoGiveItem(number(28000, 28013));
			item->SetCount(item->GetCount() - 1);
			break;

			// Â¹Â°Â¾? ?Â¦?Â¶ Â½Âº?Â³Â¿Ã« Â·Â¹Â½??? ?Â³Â¸Â®
		case USE_RECIPE:
		{
			LPITEM pSource1 = FindSpecifyItem(item->GetValue(1));
			DWORD dwSourceCount1 = item->GetValue(2);

			LPITEM pSource2 = FindSpecifyItem(item->GetValue(3));
			DWORD dwSourceCount2 = item->GetValue(4);

			if (dwSourceCount1 != 0)
			{
				if (pSource1 == NULL)
				{
					ChatPacket(CHAT_TYPE_INFO, "268");
					return false;
				}
			}

			if (dwSourceCount2 != 0)
			{
				if (pSource2 == NULL)
				{
					ChatPacket(CHAT_TYPE_INFO, "268");
					return false;
				}
			}

			if (pSource1 != NULL)
			{
				if (pSource1->GetCount() < dwSourceCount1)
				{
					ChatPacket(CHAT_TYPE_INFO, "269 %s", pSource1->GetName());
					return false;
				}

				pSource1->SetCount(pSource1->GetCount() - dwSourceCount1);
			}

			if (pSource2 != NULL)
			{
				if (pSource2->GetCount() < dwSourceCount2)
				{
					ChatPacket(CHAT_TYPE_INFO, "269 %s", pSource2->GetName());
					return false;
				}

				pSource2->SetCount(pSource2->GetCount() - dwSourceCount2);
			}

			LPITEM pBottle = FindSpecifyItem(50901);

			if (!pBottle || pBottle->GetCount() < 1)
			{
				ChatPacket(CHAT_TYPE_INFO, "270");
				return false;
			}

			pBottle->SetCount(pBottle->GetCount() - 1);

			if (number(1, 100) > item->GetValue(5))
			{
				ChatPacket(CHAT_TYPE_INFO, "271");
				return false;
			}

			AutoGiveItem(item->GetValue(0));
		}
		break;
		case USE_SPECIAL_BOOST:
		{
#ifdef USE_BOOST_POTIONS
			if (!IsLoadedAffect())
			{
				ChatPacket(CHAT_TYPE_INFO, "1401");
				return false;
			}

			const auto* proto = item->GetProto();
			if (!proto)
			{
				ChatPacket(CHAT_TYPE_INFO, "1400");
				return false;
			}

			const DWORD affectType = static_cast<DWORD>(item->GetValue(ITEM_BOOST_POTION_AFFECT_VALUE));
			if (!IS_BOOST_POTION_AFFECT(affectType))
			{
				ChatPacket(CHAT_TYPE_INFO, "1400");
				return false;
			}

			BYTE affectApply = APPLY_NONE;
			long affectValue = 0;

			for (const auto& apply : proto->aApplies)
			{
				if (apply.bType != APPLY_NONE && apply.lValue != 0)
				{
					affectApply = apply.bType;
					affectValue = apply.lValue;
					break;
				}
			}

			if (affectApply == APPLY_NONE || affectValue == 0)
			{
				ChatPacket(CHAT_TYPE_INFO, "1400");
				return false;
			}

			if (item->GetWindow() != INVENTORY)
			{
				ChatPacket(CHAT_TYPE_INFO, "1400");
				return false;
			}

			if (CArenaManager::instance().IsArenaMap(GetMapIndex()))
			{
				ChatPacket(CHAT_TYPE_INFO, "177");
				return false;
			}
#ifdef ENABLE_NEWSTUFF
			else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(item->GetVnum()))
			{
				ChatPacket(CHAT_TYPE_INFO, "134");
				return false;
			}
#endif
#endif

			affectApply = aApplyInfo[affectApply].bPointType;
			CAffect* affect = NULL;

			const auto isActivated = item->GetSocket(ITEM_BOOST_POTION_STATUS);
			if (isActivated != 0)
			{
				affect = FindAffect(affectType, affectApply);
				if (affect)
				{
					if (RemoveAffect(affect))
					{
						affect = NULL;

						item->Lock(false);
						item->SetSocket(ITEM_BOOST_POTION_STATUS, 0);
						item->StopTimerBasedOnWearExpireEvent();
						break;
					}
				}

				item->Lock(false);
				item->SetSocket(ITEM_BOOST_POTION_STATUS, 0);
				item->StopTimerBasedOnWearExpireEvent();
				break;
			}

			if (IS_SET(item->GetFlag(), ITEM_FLAG_STACKABLE) && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
			{
				if (item->GetCount() > 1)
				{
					int emptyCell = GetEmptyInventory(item->GetSize());
					if (emptyCell == -1)
					{
						ChatPacket(CHAT_TYPE_INFO, "129");
						break;
					}

					LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), 1);
					if (item2)
					{
						item->SetCount(item->GetCount() - 1);

						item2->AddToCharacter(this, TItemPos(INVENTORY, emptyCell));

						/* append
															if (item->GetSocket(1) != 0)
															{
																item2->SetSocket(1, item->GetSocket(1));
															}
						*/

						item = item2;
					}
				}
			}

			const long affectDuration = item->IsTimerBasedOnWear() ? item->GetSocket(ITEM_SOCKET_REMAIN_SEC) : INFINITE_AFFECT_DURATION;

			affect = FindAffect(affectType, affectApply);
			if (affect)
			{
				LPITEM item2 = FindItemByID(affect->dwFlag);
				if (item2)
				{
					item2->Lock(false);
					item2->SetSocket(ITEM_BOOST_POTION_STATUS, 0);
					item2->StopTimerBasedOnWearExpireEvent();
				}

				if (RemoveAffect(affect))
				{
					affect = NULL;
				}
			}

			if (!affect && AddAffect(affectType, affectApply, affectValue, item->GetID(), affectDuration, 0, false, false, item->GetCell()))
			{
				item->Lock(true);
				item->SetSocket(ITEM_BOOST_POTION_STATUS, 1);
				item->StartTimerBasedOnWearExpireEvent();
			}

			break;
		}
		}
	}
	break;

	case ITEM_METIN:
	{
		LPITEM item2;

		if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
			return false;

		if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
			return false;

		if (item2->GetType() == ITEM_PICK) return false;
		if (item2->GetType() == ITEM_ROD) return false;

		int i;

		for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			DWORD dwVnum;

			if ((dwVnum = item2->GetSocket(i)) <= 2)
				continue;

			TItemTable* p = ITEM_MANAGER::instance().GetTable(dwVnum);

			if (!p)
				continue;

			if (item->GetValue(5) == p->alValues[5])
			{
				ChatPacket(CHAT_TYPE_INFO, "272");
				return false;
			}
		}

		if (item2->GetType() == ITEM_ARMOR)
		{
			if (!IS_SET(item->GetWearFlag(), WEARABLE_BODY) || !IS_SET(item2->GetWearFlag(), WEARABLE_BODY))
			{
				ChatPacket(CHAT_TYPE_INFO, "273");
				return false;
			}
		}
		else if (item2->GetType() == ITEM_WEAPON)
		{
			if (!IS_SET(item->GetWearFlag(), WEARABLE_WEAPON))
			{
				ChatPacket(CHAT_TYPE_INFO, "274");
				return false;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, "275");
			return false;
		}

		for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			if (item2->GetSocket(i) >= 1 && item2->GetSocket(i) <= 2 && item2->GetSocket(i) >= item->GetValue(2))
			{
#ifdef ENABLE_BATTLE_PASS
				CHARACTER_MANAGER::Instance().DoMission(this, MISSION_SPRITE_STONE, 1, item->GetVnum());
#endif
#ifdef ENABLE_ADDSTONE_FAILURE
				if (number(1, 100) <= 100)
#else
				if (1)
#endif
				{
					ChatPacket(CHAT_TYPE_INFO, "276");
					item2->SetSocket(i, item->GetVnum());
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, "277");
					item2->SetSocket(i, ITEM_BROKEN_METIN_VNUM);
				}

				LogManager::instance().ItemLog(this, item2, "SOCKET", item->GetName());

				if (item->GetCount() == 1)
				{
					ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (METIN)");
				}
				else
				{
					item->SetCount(item->GetCount() - 1);
				}
				break;
			}

		if (i == ITEM_SOCKET_MAX_NUM)
			ChatPacket(CHAT_TYPE_INFO, "275");
	}
	break;

	case ITEM_AUTOUSE:
	case ITEM_MATERIAL:
	case ITEM_SPECIAL:
	case ITEM_TOOL:
	case ITEM_LOTTERY:
		break;

	case ITEM_TOTEM:
	{
		if (!item->IsEquipped())
			EquipItem(item);
	}
	break;

	case ITEM_BLEND:
		// Â»ÃµÂ·?Â¿Ã® Â¾???ÂµÃ©
		sys_log(0, "ITEM_BLEND!!");
		if (Blend_Item_find(item->GetVnum()))
		{
			int		affect_type = AFFECT_BLEND;
			int		apply_type = aApplyInfo[item->GetSocket(0)].bPointType;
			int		apply_value = item->GetSocket(1);
			int		apply_duration = item->GetSocket(2);

			if (FindAffect(affect_type, apply_type))
			{
				ChatPacket(CHAT_TYPE_INFO, "175");
			}
			else
			{
				//if (FindAffect(AFFECT_EXP_BONUS_EURO_FREE, POINT_RESIST_MAGIC))
				//{
				//	ChatPacket(CHAT_TYPE_INFO, "175");
				//}
				//else
				//{
					// ChatPacket(CHAT_TYPE_INFO, "<test1 Affect Potion> Set icon ingame for item: [%s]", item->GetName());
				AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, false);
				item->SetCount(item->GetCount() - 1);
				//}
			}
		}
		break;
	case ITEM_EXTRACT:
	{
		LPITEM pDestItem = GetItem(DestCell);
		if (NULL == pDestItem)
		{
			return false;
		}
		switch (item->GetSubType())
		{
		case EXTRACT_DRAGON_SOUL:
			if (pDestItem->IsDragonSoul())
			{
				return DSManager::instance().PullOut(this, NPOS, pDestItem, item);
			}
			return false;
		case EXTRACT_DRAGON_HEART:
			if (pDestItem->IsDragonSoul())
			{
				return DSManager::instance().ExtractDragonHeart(this, pDestItem, item);
			}
			return false;
		default:
			return false;
		}
	}
	break;

	case ITEM_NONE:
		sys_err("Item type NONE %s", item->GetName());
		break;

	default:
		sys_log(0, "UseItemEx: Unknown type %s %d", item->GetName(), item->GetType());
		return false;
	}

	return true;
}

int g_nPortalLimitTime = 10;

bool CHARACTER::UseItem(TItemPos Cell, TItemPos DestCell)
{
	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;
	//WORD wDestCell = DestCell.cell;
	//BYTE bDestInven = DestCell.window_type;
	LPITEM item;

	if (!CanHandleItem(false, false, true))
		return false;

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	if (Cell.IsSwitchbotPosition())
	{
		CSwitchbot* pkSwitchbot = CSwitchbotManager::Instance().FindSwitchbot(GetPlayerID());
		if (pkSwitchbot && pkSwitchbot->IsActive(Cell.cell))
		{
			return false;
		}

		SetSwitchbotItem(Cell.cell, 0);
		return false;
	}
#endif

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	sys_log(0, "%s: USE_ITEM %s (inven %d, cell: %d)", GetName(), item->GetName(), window_type, wCell);

	if (item->IsExchanging())
		return false;

	if (item->IsPendantSoul())
		return false;

	if (!item->CanUsedBy(this))
	{
		ChatPacket(CHAT_TYPE_INFO, "279");
		return false;
	}

	if (IsStun())
		return false;

	if (false == FN_check_item_sex(this, item))
	{
		ChatPacket(CHAT_TYPE_INFO, "280");
		return false;
	}
#if defined(WJ_COMBAT_ZONE)
	if (!CCombatZoneManager::instance().CanUseItem(this, item))
		return false;
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsLimitedItem(this, item->GetVnum()))
		return false;

	if (!CTournamentPvP::instance().CanUseItem(this, item))
		return false;
#endif

#ifdef __KINGDOMS_WAR__
	if ((GetMapIndex() == KingdomsWar::MAP_INDEX) && (KingdomsWar::IS_UNACCPETABLE_ITEM(item->GetVnum()) == 1) && (GetGMLevel() == GM_PLAYER))
	{
		ChatPacket(CHAT_TYPE_INFO, KingdomsWar::MSG[4]);
		return false;
	}
#endif


	if (!CanUseItem(GetMapIndex(), item->GetVnum()))
	{
		ChatPacket(CHAT_TYPE_INFO, "You can't use this");
		return false;
	}

	//PREVENT_TRADE_WINDOW
	if (IS_SUMMON_ITEM(item->GetVnum()))
	{
		if (false == IS_SUMMONABLE_ZONE(GetMapIndex()))
		{
			ChatPacket(CHAT_TYPE_INFO, "281");
			return false;
		}

		// Â°Ã¦?Â¥Â¹??Ã¶ Â»Ã§Â¿Ã«?Ã¶ Â»Ã³Â´Ã«Â¹Ã¦?? SUMMONABLE_ZONEÂ¿Â¡ ??Â´?Â°Â¡Â´? WarpToPC()Â¿Â¡Â¼Â­ ?Â¼?Â©

		//Â»Ã¯Â°?Â¸Â® Â°Ã¼Â·? Â¸?Â¿Â¡Â¼Â­Â´? Â±??Â¯Âº?Â¸Â¦ Â¸Â·Â¾?Â¹Ã¶Â¸Â°Â´?.
		if (CThreeWayWar::instance().IsThreeWayWarMapIndex(GetMapIndex()))
		{
			ChatPacket(CHAT_TYPE_INFO, "282");
			return false;
		}
		int iPulse = thecore_pulse();

		//?Â¢Â°Ã­ Â¿Â¬?? ?Â¼?Â©
		if (iPulse - GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			ChatPacket(CHAT_TYPE_INFO, "283 %d", g_nPortalLimitTime);

			if (test_server)
				ChatPacket(CHAT_TYPE_INFO, "[TestOnly]Pulse %d LoadTime %d PASS %d", iPulse, GetSafeboxLoadTime(), PASSES_PER_SEC(g_nPortalLimitTime));
			return false;
		}

		//Â°?Â·Â¡Â°Ã¼Â·? ?Â¢ ?Â¼?Â©
		if (GetExchange()
			 || GetShop()
			 || IsOpenSafebox()
			 || IsCubeOpen()
#ifdef __BL_67_ATTR__
			 || Is67AttrOpen()
#endif
#ifdef __AURA_SYSTEM__
			 || IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
			 || isSashOpened(true)
			 || isSashOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
			 || IsShardCraftOpened()
#endif
			)
		{
			ChatPacket(CHAT_TYPE_INFO, "284");
			return false;
		}

		//PREVENT_REFINE_HACK
		//Â°Â³Â·Â®?? Â½?Â°Â£?Â¼?Â©
		{
			if (iPulse - GetRefineTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, "285 %d", g_nPortalLimitTime);
				return false;
			}
		}
		//END_PREVENT_REFINE_HACK


		//PREVENT_ITEM_COPY
		{
			if (iPulse - GetMyShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, "286 %d", g_nPortalLimitTime);
				return false;
			}

		}
		//END_PREVENT_ITEM_COPY


		//Â±??Â¯Âº? Â°?Â¸Â®?Â¼?Â©
		if (item->GetVnum() != 70302)
		{
			PIXEL_POSITION posWarp;

			int x = 0;
			int y = 0;

			double nDist = 0;
			const double nDistant = 5000.0;
			//Â±??Â¯Â±Ã¢Â¾Ã¯Âº?
			if (item->GetVnum() == 22010)
			{
				x = item->GetSocket(0) - GetX();
				y = item->GetSocket(1) - GetY();
			}
			//Â±??Â¯Âº?
			else if (item->GetVnum() == 22000)
			{
				SECTREE_MANAGER::instance().GetRecallPositionByEmpire(GetMapIndex(), GetEmpire(), posWarp);

				if (item->GetSocket(0) == 0)
				{
					x = posWarp.x - GetX();
					y = posWarp.y - GetY();
				}
				else
				{
					x = item->GetSocket(0) - GetX();
					y = item->GetSocket(1) - GetY();
				}
			}

			nDist = sqrt(pow((float)x, 2) + pow((float)y, 2));

			if (nDistant > nDist)
			{
				ChatPacket(CHAT_TYPE_INFO, "287");
				if (test_server)
					ChatPacket(CHAT_TYPE_INFO, "PossibleDistant %f nNowDist %f", nDistant, nDist);
				return false;
			}
		}

		//PREVENT_PORTAL_AFTER_EXCHANGE
		//Â±Â³?Â¯ ?? Â½?Â°Â£?Â¼?Â©
		if (iPulse - GetExchangeTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			ChatPacket(CHAT_TYPE_INFO, "288 %d", g_nPortalLimitTime);
			return false;
		}
		//END_PREVENT_PORTAL_AFTER_EXCHANGE

	}

	//ÂºÂ¸ÂµÃ»Â¸Â® ÂºÃ±Â´? Â»Ã§Â¿Ã«Â½? Â°?Â·Â¡?Â¢ ?Â¦?? ?Â¼?Â©
	if ((item->GetVnum() == 50200) || (item->GetVnum() == 71049))
	{
		if (GetExchange()
			 || GetShop()
			 || IsOpenSafebox()
			 || IsCubeOpen()
#ifdef __BL_67_ATTR__
			|| Is67AttrOpen()
#endif
#ifdef __AURA_SYSTEM__
			|| IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
			 || isSashOpened(true)
			 || isSashOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
			 || IsShardCraftOpened()
#endif
			)
		{
			ChatPacket(CHAT_TYPE_INFO, "289");
			return false;
		}

	}
	//END_PREVENT_TRADE_WINDOW

	// @fixme150 BEGIN
	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, "290");
		return false;
	}
	// @fixme150 END
#ifdef __SOULBINDING_SYSTEM__
	if (item->IsBind() || item->IsUntilBind())
	{
		ChatPacket(CHAT_TYPE_INFO, "291");
		return false;
	}
#endif
#ifdef __ANTI_EXP__
	if (item->GetVnum() == 72501)
	{
		int so = item->GetSocket(0);
		if (so == 0 && GetQuestFlag("anti_exp.block_exp") == 0)
		{
			item->Lock(true);
			item->SetSocket(0, 1);
			SetQuestFlag("anti_exp.block_exp", 1);
		}
		else
		{
			item->Lock(false);
			item->SetSocket(0, 0);
			SetQuestFlag("anti_exp.block_exp", 0);
		}
	}
#endif

	if (IS_SET(item->GetFlag(), ITEM_FLAG_LOG)) // Â»Ã§Â¿Ã« Â·?Â±?Â¸Â¦ Â³Â²Â±Ã¢Â´? Â¾????? ?Â³Â¸Â®
	{
		DWORD vid = item->GetVID();
		DWORD oldCount = item->GetCount();
		DWORD vnum = item->GetVnum();

		char hint[ITEM_NAME_MAX_LEN + 32 + 1];
		int len = snprintf(hint, sizeof(hint) - 32, "%s", item->GetName());

		if (len < 0 || len >= (int)sizeof(hint) - 32)
			len = (sizeof(hint) - 32) - 1;

#ifdef ENABLE_BATTLEPASS
		bool bpRet = (item->GetType() == ITEM_USE
						|| item->GetType() == ITEM_SKILLBOOK
						|| CItemVnumHelper::IsExtraBook(item->GetVnum())
						|| item->GetType() == ITEM_GIFTBOX);
#endif

		bool ret = UseItemEx(item, DestCell);

#ifdef ENABLE_BATTLEPASS
		if (ret && bpRet)
		{
			UpdateMissionProgress(MISSION_USE_ITEM, 1, item->GetVnum());
		}
#endif

		if (NULL == ITEM_MANAGER::instance().FindByVID(vid)) // UseItemExÂ¿Â¡Â¼Â­ Â¾??????? Â»Ã¨?Â¦ Âµ?Â¾ÃºÂ´?. Â»Ã¨?Â¦ Â·?Â±?Â¸Â¦ Â³Â²Â±Ã¨
		{
			LogManager::instance().ItemLog(this, vid, vnum, "REMOVE", hint);
		}
		else if (oldCount != item->GetCount())
		{
			snprintf(hint + len, sizeof(hint) - len, " %u", oldCount - 1);
			LogManager::instance().ItemLog(this, vid, vnum, "USE_ITEM", hint);
		}

		return ret;
	}
	else
	{
#ifdef ENABLE_BATTLEPASS
		bool bpRet = (item->GetType() == ITEM_USE
						|| item->GetType() == ITEM_SKILLBOOK
						|| CItemVnumHelper::IsExtraBook(item->GetVnum())
						|| item->GetType() == ITEM_GIFTBOX);
#endif

		bool ret = UseItemEx(item, DestCell);

#ifdef ENABLE_BATTLEPASS
		if (ret && bpRet)
		{
			UpdateMissionProgress(MISSION_USE_ITEM, 1, item->GetVnum());
		}
#endif

		return ret;
	}
}

bool CHARACTER::DropItem(TItemPos Cell, DWORD bCount)
{
	LPITEM item = NULL;

	if (!CanHandleItem())
	{
		if (NULL != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, "292");
		return false;
	}
#ifdef ENABLE_NEWSTUFF
	if (0 != g_ItemDropTimeLimitValue)
	{
		if (get_dword_time() < m_dwLastItemDropTime + g_ItemDropTimeLimitValue)
		{
			ChatPacket(CHAT_TYPE_INFO, "168");
			return false;
		}
	}

	m_dwLastItemDropTime = get_dword_time();
#endif
	if (IsDead())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	if (item->IsExchanging())
		return false;

	if (true == item->isLocked())
		return false;
#ifdef ENABLE_SOULBIND_SYSTEM 
	if (item->IsSealed())
	{
		ChatPacket(CHAT_TYPE_INFO, "293");
		return false;
	}
#endif
	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_DROP | ITEM_ANTIFLAG_GIVE))
	{
		ChatPacket(CHAT_TYPE_INFO, "294");
		return false;
	}

	if (bCount == 0 || bCount > item->GetCount())
		bCount = item->GetCount();

	SyncQuickslot(QUICKSLOT_TYPE_ITEM, Cell.cell, WORD_MAX);	// Quickslot Â¿Â¡Â¼Â­ ?Ã¶Â¿Ã²

	LPITEM pkItemToDrop;

	if (bCount == item->GetCount())
	{
		item->RemoveFromCharacter();
		pkItemToDrop = item;
	}
	else
	{
		if (bCount == 0)
		{
			if (test_server)
				sys_log(0, "[DROP_ITEM] drop item count == 0");
			return false;
		}

		item->SetCount(item->GetCount() - bCount);
		ITEM_MANAGER::instance().FlushDelayedSave(item);

		pkItemToDrop = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), bCount);

		// copy item socket -- by mhh
		FN_copy_item_socket(pkItemToDrop, item);

		char szBuf[51 + 1];
		snprintf(szBuf, sizeof(szBuf), "%u %u", pkItemToDrop->GetID(), pkItemToDrop->GetCount());
		LogManager::instance().ItemLog(this, item, "ITEM_SPLIT", szBuf);
	}

	PIXEL_POSITION pxPos = GetXYZ();

	if (pkItemToDrop->AddToGround(GetMapIndex(), pxPos))
	{
		ChatPacket(CHAT_TYPE_INFO, "295");
#ifdef ENABLE_NEWSTUFF
		pkItemToDrop->StartDestroyEvent(g_aiItemDestroyTime[ITEM_DESTROY_TIME_DROPITEM]);
#else
		pkItemToDrop->StartDestroyEvent();
#endif

		ITEM_MANAGER::instance().FlushDelayedSave(pkItemToDrop);

		char szHint[32 + 1];
		snprintf(szHint, sizeof(szHint), "%s %u %u", pkItemToDrop->GetName(), pkItemToDrop->GetCount(), pkItemToDrop->GetOriginalVnum());
		LogManager::instance().ItemLog(this, pkItemToDrop, "DROP", szHint);
		//Motion(MOTION_PICKUP);
	}

	return true;
}

#ifdef ENABLE_SELL_ITEM
bool CHARACTER::SellItem(TItemPos Cell)
{
	LPITEM item = NULL;

	if (!CanHandleItem())
	{
		if (NULL != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, "Unknown message, please report to a administrator!");
		return false;
	}

	if (IsDead())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	if (item->IsExchanging() || item->IsEquipped())
		return false;

	if (true == item->isLocked())
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_SELL))
		return false;

	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
		return false;

	if (item->GetCount() <= 0)
		return false;

#ifdef ENABLE_FISH_EVENT
	if (IsOpenJigsaw())
	{
		if (item->GetVnum() == 25106 || item->GetVnum() == 25107)
		{
			ChatPacket(CHAT_TYPE_INFO, "Cannot sell puzzle items from your inventory when Jigsaw Window is open.");
			return false;
		}
	}
#endif

	long long dwPrice = item->GetShopBuyPrice();
	dwPrice *= item->GetCount();

	long long dwTax = 0;
	int iVal = 3;

	{
		dwTax = dwPrice * static_cast<long long>(iVal) / 100;
		dwPrice -= dwTax;
	}

	const long long nTotalMoney = static_cast<long long>(GetGold()) + static_cast<long long>(dwPrice);
	if (nTotalMoney < 0)
	{
		return false;
	}
	else if (nTotalMoney > GOLD_MAX)
	{
		ChatPacket(CHAT_TYPE_INFO, "1312");
		ChatPacket(CHAT_TYPE_INFO, "1311 %lld", GOLD_MAX);
		return false;
	}

	BYTE bBlockAttr = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (item->GetAttributeType(i) > 0)
			++bBlockAttr;
	}

	if (bBlockAttr >= 5)
	{
		ChatPacket(CHAT_TYPE_INFO, "You cannot sell this item because it has 5 bonuses!");
		return false;
	}

	if (item->GetRefineLevel() >= 7)
	{
		ChatPacket(CHAT_TYPE_INFO, "You cannot quick sell +7, +8 and +9 equipment.");
		return false;
	}

	char buf[1024];
	char itemlink[256];
	int len;
	len = snprintf(itemlink, sizeof(itemlink), "item:%x:%lx", item->GetVnum(), item->GetFlag());

	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		len += snprintf(itemlink + len, sizeof(itemlink) - len, ":%lx", item->GetSocket(i));

	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		if (item->GetAttributeType(i) != 0)
			len += snprintf(itemlink + len, sizeof(itemlink) - len, ":%x:%d", item->GetAttributeType(i), item->GetAttributeValue(i));

	snprintf(buf, sizeof(buf), "|cffffc700|H%s|h[%s]|h|r", itemlink, item->GetName());

	item->SetCount(0);
	PointChange(POINT_GOLD, dwPrice, false);

	ChatPacket(CHAT_TYPE_INFO, "You succesfuly sold the following item with a 3 percent comission: %s ", buf);
	return true;
}
#endif

bool CHARACTER::DropGold(int gold)
{
	if (gold <= 0 || gold > GetGold())
		return false;

	if (!CanHandleItem())
		return false;

	if (0 != g_GoldDropTimeLimitValue)
	{
		if (get_dword_time() < m_dwLastGoldDropTime + g_GoldDropTimeLimitValue)
		{
			ChatPacket(CHAT_TYPE_INFO, "168");
			return false;
		}
	}

	m_dwLastGoldDropTime = get_dword_time();

	LPITEM item = ITEM_MANAGER::instance().CreateItem(1, gold);

	if (item)
	{
		PIXEL_POSITION pos = GetXYZ();

		if (item->AddToGround(GetMapIndex(), pos))
		{
			//Motion(MOTION_PICKUP);
			PointChange(POINT_GOLD, -gold, true);

			if (gold > 1000) // ?ÂµÂ¿Ã¸ ??Â»Ã³Â¸Â¸ Â±Ã¢Â·???Â´?.
				LogManager::instance().CharLog(this, gold, "DROP_GOLD", "");

#ifdef ENABLE_NEWSTUFF
			item->StartDestroyEvent(g_aiItemDestroyTime[ITEM_DESTROY_TIME_DROPGOLD]);
#else
			item->StartDestroyEvent();
#endif
			ChatPacket(CHAT_TYPE_INFO, "296 %d", 150 / 60);
		}

		Save();
		return true;
	}

	return false;
}

bool CHARACTER::IsValidItemChangeEquip(WORD cell, LPITEM item)
{
	int iWearCell = item->FindEquipCell(this);


	if (cell < 0)
	{
		return false;
	}

	if (cell > CHANGE_EQUIP_SLOT_COUNT)
	{
		return false;
	}

	if (GetChangeEquipItem(cell))
	{
		return false;
	}

	if (iWearCell == WEAR_ARROW)
	{
		return false;
	}


	BYTE page_index_ce = 1;

	for (int i = 1; i < CHANGE_EQUIP_PAGE_EXTRA; ++i)
	{
		if (cell >= CHANGE_EQUIP_SLOT_COUNT / CHANGE_EQUIP_PAGE_EXTRA * i && cell < (CHANGE_EQUIP_SLOT_COUNT / CHANGE_EQUIP_PAGE_EXTRA) * (i + 1))
		{
			page_index_ce = i;
			cell = cell - ((CHANGE_EQUIP_SLOT_COUNT / CHANGE_EQUIP_PAGE_EXTRA) * i);
		}
	}

	if (item->GetType() == ITEM_NEW_RINGS)
	{
		if (cell < WEAR_RINGS_1 || cell > WEAR_RINGS_2)
		{
			return false;
		}

		if (iWearCell < WEAR_RINGS_1 || iWearCell > WEAR_RINGS_2)
		{
			return false;
		}

		DWORD index_old = CHANGE_EQUIP_SLOT_COUNT - (CHANGE_EQUIP_SLOT_COUNT / page_index_ce);


		if (page_index_ce > 1)
		{
			index_old = CHANGE_EQUIP_SLOT_COUNT / CHANGE_EQUIP_PAGE_EXTRA * page_index_ce;
		}


		for (int i = index_old; i < (CHANGE_EQUIP_SLOT_COUNT / CHANGE_EQUIP_PAGE_EXTRA) * (page_index_ce + 1); ++i)
		{
			LPITEM item_check = GetChangeEquipItem(i);
			if (item_check)
			{
				if (item_check->GetType() == ITEM_NEW_RINGS)
				{
					if (item_check)
					{
						if (item_check->GetAccessorySocketVnumRings(item_check) == item->GetAccessorySocketVnumRings(item))
						{
							return false;
						}
					}
				}
			}
		}

	}
	else
	{
		if (iWearCell != cell)
		{
			return false;
		}
	}

	switch (GetJob())
	{
	case JOB_WARRIOR:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR)
			return false;
		break;

	case JOB_ASSASSIN:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN)
			return false;
		break;

	case JOB_SHAMAN:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
			return false;
		break;

	case JOB_SURA:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_SURA)
			return false;
		break;
	}

	if (false == FN_check_item_sex(this, item))
	{
		return false;
	}

	return true;
}


#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
DWORD CHARACTER::GetSwitchbotItemSwitcherUsed(WORD wCell)
{
	return m_playerData ? m_playerData->m_dwSwitchbotUsed[wCell] : 0;
}

void CHARACTER::SetSwitchbotItemSwitcherUsed(WORD wCell, DWORD dwCount)
{
	if (!m_playerData)
	{
		return;
	}

	if (m_playerData->m_dwSwitchbotUsed[wCell] == dwCount)
	{
		return;
	}

	m_playerData->m_dwSwitchbotUsed[wCell] = dwCount;
	m_bIsNeedSwitchbotSave = true;
}

void CHARACTER::SetSwitchbotItem(WORD wCell, WORD wCellInven)
{
	if (!m_playerData)
	{
		return;
	}

	if (!GetDesc() || wCell >= SWITCHBOT_SLOT_COUNT)
	{
		return;
	}

	LPITEM pItem = nullptr;
	if (wCellInven > 0) // 0==NONE_POS
		pItem = GetInventoryItem(wCellInven - 1);

	// Refresh Window
	if (pItem && SwitchbotHelper::IsValidItem(pItem))
	{
		pItem->Lock(true);

		TPacketGCItemSet pack = {};
		pack.header = HEADER_GC_ITEM_SET;
		pack.Cell = TItemPos(SWITCHBOT, wCell);

		pack.count = pItem->GetCount();
		pack.vnum = pItem->GetVnum();
		pack.flags = pItem->GetFlag();
		pack.anti_flags = pItem->GetAntiFlag();

		std::memcpy(pack.alSockets, pItem->GetSockets(), sizeof(pack.alSockets));
		std::memcpy(pack.aAttr, pItem->GetAttributes(), sizeof(pack.aAttr));
		GetDesc()->Packet(&pack, sizeof(TPacketGCItemSet));

		m_playerData->m_wSwitchbotCell[wCell] = wCellInven;
		m_playerData->m_dwSwitchbotUsed[wCell] = 0;

		CSwitchbotManager::Instance().RegisterItem(GetPlayerID(), pItem->GetID(), wCell);
	}
	else
	{
		TPacketGCItemDelDeprecated pack;
		pack.header = HEADER_GC_ITEM_DEL;
		pack.Cell = TItemPos(SWITCHBOT, wCell);
		pack.count = 0;
		pack.vnum = 0;
		memset(pack.alSockets, 0, sizeof(pack.alSockets));
		memset(pack.aAttr, 0, sizeof(pack.aAttr));
		GetDesc()->Packet(&pack, sizeof(TPacketGCItemDelDeprecated));

		// UNLOCK_ITEM
		auto wCellInvenOld = m_playerData->m_wSwitchbotCell[wCell];
		if (wCellInvenOld != 0)
		{
			pItem = GetInventoryItem(wCellInvenOld - 1);
			if (pItem)
				pItem->Lock(false);
		}

		m_playerData->m_wSwitchbotCell[wCell] = 0;
		m_playerData->m_dwSwitchbotUsed[wCell] = 0;

		CSwitchbotManager::Instance().UnregisterItem(GetPlayerID(), wCell);
	}

	m_bIsNeedSwitchbotSave = true;
}

void CHARACTER::ItemSaveSwitchbot()
{
	if (m_bIsNeedSwitchbotSave == false)
		return;

	if (!m_playerData)
	{
		return;
	}

	FILE* fileID;
	char file_name[256 + 1];

	snprintf(file_name, sizeof(file_name), "%s/local_database/switchbot/pid_%d.txt", LocaleService_GetBasePath().c_str(), GetPlayerID());
	fileID = fopen(file_name, "w");

	if (NULL == fileID)
		return;

	for (int i = 0; i < SWITCHBOT_SLOT_COUNT; ++i)
	{
		fprintf(fileID, "CELL	%d	%d	%d\n", i, m_playerData->m_wSwitchbotCell[i], m_playerData->m_dwSwitchbotUsed[i]);
	}

	fclose(fileID);
}

void CHARACTER::ItemLoadSwitchbot()
{
	if (!m_playerData)
	{
		return;
	}

	if (!GetDesc())
	{
		return;
	}

	// READ_FROM_FILE
	FILE* fp;
	char	one_line[256]; char* v, * token_string; char file_name[256 + 1];
	int		value1, value2, value3;
	const char* delim = " \t\r\n";
	snprintf(file_name, sizeof(file_name), "%s/local_database/switchbot/pid_%d.txt", LocaleService_GetBasePath().c_str(), GetPlayerID());
	fp = fopen(file_name, "r");
	if (NULL == fp)
		return;

	while (fgets(one_line, 256, fp))
	{
		value1 = 0; value2 = 0; value3 = 0;
		token_string = strtok(one_line, delim);
		if (NULL == token_string)
			continue;

		if ((v = strtok(NULL, delim)))
			str_to_number(value1, v);
		if ((v = strtok(NULL, delim)))
			str_to_number(value2, v);
		if ((v = strtok(NULL, delim)))
			str_to_number(value3, v);

		TOKEN("CELL")
		{
			if (value1 >= 0 && value1 < SWITCHBOT_SLOT_COUNT)
				m_playerData->m_wSwitchbotCell[value1] = value2;

			if (value1 >= 0 && value1 < SWITCHBOT_SLOT_COUNT)
				m_playerData->m_dwSwitchbotUsed[value1] = value3;
		}
	}
	fclose(fp);
	// READ_FROM_FILE_END

	for (int i = 0; i < SWITCHBOT_SLOT_COUNT; ++i)
	{
		auto wCell = i;
		auto wCellInven = m_playerData->m_wSwitchbotCell[i];
		if (wCellInven == 0)
		{
			m_playerData->m_dwSwitchbotUsed[wCell] = 0;
			continue;
		}

		auto pItem = GetInventoryItem(wCellInven - 1);
		if (pItem && SwitchbotHelper::IsValidItem(pItem))
		{
			pItem->Lock(true);

			TPacketGCItemSet pack = {};
			pack.header = HEADER_GC_ITEM_SET;
			pack.Cell = TItemPos(SWITCHBOT, wCell);

			pack.count = pItem->GetCount();
			pack.vnum = pItem->GetVnum();
			pack.flags = pItem->GetFlag();
			pack.anti_flags = pItem->GetAntiFlag();

			std::memcpy(pack.alSockets, pItem->GetSockets(), sizeof(pack.alSockets));
			std::memcpy(pack.aAttr, pItem->GetAttributes(), sizeof(pack.aAttr));
			GetDesc()->Packet(&pack, sizeof(TPacketGCItemSet));

			CSwitchbotManager::Instance().RegisterItem(GetPlayerID(), pItem->GetID(), wCell);
		}
		else
		{
			TPacketGCItemDelDeprecated pack;
			pack.header = HEADER_GC_ITEM_DEL;
			pack.Cell = TItemPos(SWITCHBOT, wCell);
			pack.count = 0;
			pack.vnum = 0;
			memset(pack.alSockets, 0, sizeof(pack.alSockets));
			memset(pack.aAttr, 0, sizeof(pack.aAttr));
			GetDesc()->Packet(&pack, sizeof(TPacketGCItemDelDeprecated));

			CSwitchbotManager::Instance().UnregisterItem(GetPlayerID(), wCell);
			m_playerData->m_dwSwitchbotUsed[wCell] = 0;
		}
	}
}
#endif

bool CHARACTER::MoveItem(TItemPos Cell, TItemPos DestCell, DWORD count, bool isFromCommand)
{
#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	if (Cell.IsSwitchbotPosition()) // remove item from switchbot
	{
		if (!IsValidItemPosition(Cell))
			return false;

		CSwitchbot* pkSwitchbot = CSwitchbotManager::Instance().FindSwitchbot(GetPlayerID());
		if (pkSwitchbot && pkSwitchbot->IsActive(Cell.cell))
		{
			return false;
		}

		SetSwitchbotItem(Cell.cell, 0);
		return true;
	}
#endif

	LPITEM item = NULL;

	if (!IsValidItemPosition(Cell))
		return false;

	if (!(item = GetItem(Cell)))
		return false;

	if (item->IsPendantSoul())
		return false;

	if (item->IsExchanging())
		return false;

	if (item->GetCount() < count)
		return false;

	if (INVENTORY == Cell.window_type && Cell.cell >= INVENTORY_MAX_NUM && IS_SET(item->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		return false;

	if (true == item->isLocked())
		return false;

	if (!IsValidItemPosition(DestCell))
	{
		return false;
	}

	if (Cell.cell == DestCell.cell && Cell.window_type == DestCell.window_type)
		return false;

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	if (item->isLocked() && CSwitchbotManager::Instance().IsItemIDInSwitchbot(GetPlayerID(), item->GetID()))
	{
		ChatPacket(CHAT_TYPE_INFO, "Remove item from switchbot first!");
		return false;
	}
#endif

	// if ((SWITCHBOT != Cell.window_type) && (Cell.cell == DestCell.cell))
		// return false;

	if (!CanHandleItem())
	{
		if (NULL != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, "292");
#ifdef __AURA_SYSTEM__
		if (IsAuraRefineWindowOpen())
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ìì°ë¼> ì¤ë¼ ì°½ì´ ì´ë ¤ìì ëê¹ì§ í­ëª©ì ì´ëí  ì ììµëë¤."));
#endif
		return false;
	}

	// Â±Ã¢?Â¹???? Â¿Ã¤?Â»?Â¸Â·? ÂºÂ§?Â® ??ÂºÂ¥?Ã¤Â¸Â®Â¿Â¡Â´? ?Â¯?Â¤ ?Â¸???? Â¾?????Â¸Â¸ Â³??Â» Â¼Ã¶ ??Â´?.
	if (DestCell.IsBeltInventoryPosition() && false == CBeltInventoryHelper::CanMoveIntoBeltInventory(item))
	{
		ChatPacket(CHAT_TYPE_INFO, "297");
		return false;
	}


#ifdef ENABLE_MOUNT_COSTUME_SYSTEM

	if (DestCell.IsDefaultInventoryPosition() && Cell.IsEquipPosition())
	{
		if (!CanUnequipNow(item))
			return false;

		if (item->IsCostumeMount() && GetHorse())
			HorseSummon(false);
	}
#endif

#ifdef CHANGE_EQUIP_WORLDARD
	if (DestCell.IsChangeEquipPosition())
	{
		if (!IsValidItemChangeEquip(DestCell.cell, item))
		{
			ChatPacket(CHAT_TYPE_INFO, "No puedes mover el item a ese slot");
			return false;
		}
	}
#endif

	if (IsLastMoveItemTime() && !isFromCommand)
	{
		ChatPacket(CHAT_TYPE_INFO, "You cant move this item yet.");
		return false;
	}

	SetLastMoveItemTime(get_dword_time());

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	if (DestCell.IsSwitchbotPosition())
	{
		if (!IsValidItemPosition(DestCell))
			return false;

		CSwitchbot* pkSwitchbot = CSwitchbotManager::Instance().FindSwitchbot(GetPlayerID());
		if (pkSwitchbot && pkSwitchbot->IsActive(DestCell.cell))
		{
			return false;
		}

		if (item->IsEquipped())
		{
			ChatPacket(CHAT_TYPE_INFO, "Transfer type not allowed.");
			return false;
		}

		if (!SwitchbotHelper::IsValidItem(item))
		{
			ChatPacket(CHAT_TYPE_INFO, "Invalid item type for switchbot.");
			return false;
		}

		SetSwitchbotItem(DestCell.cell, Cell.cell + 1);
		return true;
	}
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
	if (DestCell.IsSwitchBotPosition())
	{
		if (CSwitchBonus::Instance().CanMoveIntoSB(item) == false)
		{
			return false;
		}

		if (GetItem(TItemPos(SWITCHBOT, 0)))
		{
			return false;
		}

	}
#endif

	if (Cell.IsEquipPosition())
	{
		if (!CanUnequipNow(item))
			return false;

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		int iWearCell = item->FindEquipCell(this);
		if (iWearCell == WEAR_WEAPON)
		{
			LPITEM costumeWeapon = GetWear(WEAR_COSTUME_WEAPON);
			if (costumeWeapon && !UnequipItem(costumeWeapon))
			{
				ChatPacket(CHAT_TYPE_INFO, "300");
				return false;
			}

			if (!IsEmptyItemGrid(DestCell, item->GetSize(), Cell.cell))
				return UnequipItem(item);
		}
#endif
	}

	if (DestCell.IsEquipPosition())
	{
		if (GetItem(DestCell))	// ?Ã¥ÂºÃ±?? Â°Ã¦Â¿Ã¬ ?? Â°Ã·Â¸Â¸ Â°?Â»Ã§??ÂµÂµ Âµ?Â´?.
		{
			ChatPacket(CHAT_TYPE_INFO, "301");

			return false;
		}
		EquipItem(item, DestCell.cell - INVENTORY_MAX_NUM);
	}
	else
	{
		if (item->IsDragonSoul())
		{
			if (item->IsEquipped())
			{
				return DSManager::instance().PullOut(this, DestCell, item);
			}
			else
			{
				if (DestCell.window_type != DRAGON_SOUL_INVENTORY)
				{
					return false;
				}
				if (!DSManager::instance().IsValidCellForThisItem(item, DestCell))
					return false;
			}
		}
		// Â¿Ã«?Â¥Â¼Â®?? Â¾?Â´? Â¾??????Âº Â¿Ã«?Â¥Â¼Â® ??ÂºÂ¥Â¿Â¡ ÂµÃ©Â¾Ã®Â°Â¥ Â¼Ã¶ Â¾Ã¸Â´?.
		else if (DRAGON_SOUL_INVENTORY == DestCell.window_type)
			return false;
#ifdef ENABLE_SPECIAL_STORAGE
		if (item->IsUpgradeItem())
		{
			if (DestCell.window_type != UPGRADE_INVENTORY && DestCell.window_type != INVENTORY && DestCell.window_type != SAFEBOX)
				return false;

			// if (!DSManager::instance().IsValidCellForThisItem2(item, DestCell))
				// return false;
		}
		else if (UPGRADE_INVENTORY == DestCell.window_type)
			return false;

		if (item->IsBook())
		{
			if (DestCell.window_type != BOOK_INVENTORY && DestCell.window_type != INVENTORY && DestCell.window_type != SAFEBOX)
				return false;

			//if (!DSManager::instance().IsValidCellForThisItem2(item, DestCell))
			//	return false;
		}
		else if (BOOK_INVENTORY == DestCell.window_type)
			return false;

		if (item->IsStone())
		{
			if (DestCell.window_type != STONE_INVENTORY && DestCell.window_type != INVENTORY && DestCell.window_type != SAFEBOX)
				return false;

			// if (!DSManager::instance().IsValidCellForThisItem2(item, DestCell)){
				// ChatPacket(CHAT_TYPE_INFO, "302");
				// return false;
			// }
		}
		else if (STONE_INVENTORY == DestCell.window_type)
		{
			return false;
		}

		if (item->IsChange())
		{
			if (DestCell.window_type != CHANGE_INVENTORY && DestCell.window_type != INVENTORY && DestCell.window_type != SAFEBOX)
				return false;

			// if (!DSManager::instance().IsValidCellForThisItem2(item, DestCell)){
				// ChatPacket(CHAT_TYPE_INFO, "302");
				// return false;
			// }
		}
		else if (CHANGE_INVENTORY == DestCell.window_type)
		{
			return false;
		}

		if (item->IsCostume() || item->IsCostumeSkin())
		{
			if (DestCell.window_type != COSTUME_INVENTORY && DestCell.window_type != INVENTORY && DestCell.window_type != SAFEBOX && DestCell.window_type != CHANGE_EQUIP)
				return false;

			// if (!DSManager::instance().IsValidCellForThisItem2(item, DestCell)){
				// ChatPacket(CHAT_TYPE_INFO, "302");
				// return false;
			// }
		}
		else if (COSTUME_INVENTORY == DestCell.window_type)
		{
			return false;
		}

		if (item->IsChest())
		{
			if (DestCell.window_type != CHEST_INVENTORY && DestCell.window_type != INVENTORY && DestCell.window_type != SAFEBOX)
				return false;
		}
		else if (CHEST_INVENTORY == DestCell.window_type)
		{
			return false;
		}
#endif
		LPITEM item2;

		if ((item2 = GetItem(DestCell)) && item != item2 && item2->IsStackable() &&
			!IS_SET(item2->GetAntiFlag(), ITEM_ANTIFLAG_STACK) &&
			item2->GetVnum() == item->GetVnum()) // ???Â¥ Â¼Ã¶ ??Â´? Â¾??????? Â°Ã¦Â¿Ã¬
		{
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
				if (item2->GetSocket(i) != item->GetSocket(i))
					return false;

			if (count == 0)
				count = item->GetCount();

			sys_log(0, "%s: ITEM_STACK %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell,
				DestCell.window_type, DestCell.cell, count);

			count = MIN(g_bItemCountLimit - item2->GetCount(), count);

			item->SetCount(item->GetCount() - count);
			item2->SetCount(item2->GetCount() + count);

#ifdef ENABLE_MINI_GAME_CATCH_KING
			if (item2->GetVnum() == CATCH_KING_DROP_ITEM && item2->GetCount() >= 25)
			{
				const LPITEM& KingDeck = AutoGiveItem(CATCH_KING_PLAY_ITEM);
				if (KingDeck != nullptr)
					item2->SetCount(item2->GetCount() - 25);
			}
#endif

			return true;
		}
#ifdef USE_ITEM_FLAG_TIME_STACKABLE
		else if ((item2 = GetItem(DestCell))
			&& item != item2
			&& item2->IsTimeStackable()
			&& item2->GetVnum() == item->GetVnum())
		{
			if (count == 0)
			{
				count = item->GetCount();
			}

			if (count == 0 || count > 1)
			{
				return false;
			}

#ifdef USE_BOOST_POTIONS
			if (item->IsBoostPotion() && item->IsTimerBasedOnWear())
			{
				if (item->GetSocket(ITEM_BOOST_POTION_STATUS != 0) || item2->GetSocket(ITEM_BOOST_POTION_STATUS) != 0)
				{
					ChatPacket(CHAT_TYPE_INFO, "1402");
					return false;
				}
				else if (item->isLocked() || item2->isLocked())
				{
					return false;
				}
				else if (item->IsExchanging() || item2->IsExchanging())
				{
					return false;
				}
				else if (item->IsEquipped() || item2->IsEquipped())
				{
					return false;
				}

				const auto bWindow = item->GetWindow();
				if (bWindow != INVENTORY || bWindow != item2->GetWindow())
				{
					return false;
				}

				const long lSrcDuration = item->GetSocket(ITEM_SOCKET_REMAIN_SEC);
				const long lTrgDuration = item2->GetSocket(ITEM_SOCKET_REMAIN_SEC);

				const long lDuration = lSrcDuration + lTrgDuration;
				if (lDuration <= 0)
				{
					return false;
				}

				if (lDuration > ITEM_BOOST_POTION_MAX_STACK_TIME)
				{
					ChatPacket(CHAT_TYPE_INFO, "1403");
					return false;
				}

				sys_log(0, "%s: ITEM_STACK_TIME %s (window: %u, cell : %u) -> (window : %u, cell %u) count %u srcDuration %ld trgDuration %ld newDuration %ld"
					, GetName()
					, item->GetName()
					, Cell.window_type
					, Cell.cell
					, DestCell.window_type
					, DestCell.cell
					, count
					, lSrcDuration
					, lTrgDuration
					, lDuration);

				ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (STACK TIME)");
				item2->SetSocket(ITEM_SOCKET_REMAIN_SEC, lDuration);
				return true;
			}
#endif

			return false;
		}
#endif

		if (!IsEmptyItemGrid(DestCell, item->GetSize(), Cell.cell))
			return false;

		if (count == 0 || count >= item->GetCount() || !item->IsStackable() || IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
		{
			sys_log(0, "%s: ITEM_MOVE %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell,
				DestCell.window_type, DestCell.cell, count);

			item->RemoveFromCharacter();
#ifdef ENABLE_NEWITEM_AFFECT
			SetItem(DestCell, item, false);
#else
			SetItem(DestCell, item);
#endif

			if (INVENTORY == Cell.window_type && INVENTORY == DestCell.window_type)
				SyncQuickslot(QUICKSLOT_TYPE_ITEM, Cell.cell, DestCell.cell);
		}
		else if (count < item->GetCount())
		{

#ifdef ENABLE_PREVENT_FLOOD_PACKETS
			const int iPulse = thecore_pulse();
			if (iPulse - GetLastItemSplitTime() < PASSES_PER_SEC(3)) // 3 sec
			{
				ChatPacket(CHAT_TYPE_INFO, ("[1554]Please try again later."));
				return false;
			}
#endif


			sys_log(0, "%s: ITEM_SPLIT %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell,
				DestCell.window_type, DestCell.cell, count);

			item->SetCount(item->GetCount() - count);
			LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), count);

			// copy socket -- by mhh
			FN_copy_item_socket(item2, item);

			item2->AddToCharacter(this, DestCell);
#ifdef ENABLE_PREVENT_FLOOD_PACKETS
			SetLastItemSplitTime();
#endif
			char szBuf[51 + 1];
			snprintf(szBuf, sizeof(szBuf), "%u %u %u %u ", item2->GetID(), item2->GetCount(), item->GetCount(), item->GetCount() + item2->GetCount());
			LogManager::instance().ItemLog(this, item, "ITEM_SPLIT", szBuf);
		}
	}

#ifdef ENABLE_MOUNT_SKIN
	if (DestCell.IsDefaultInventoryPosition() && Cell.IsEquipPosition())
	{
		if (item->IsCostumeMountSkin() && GetHorse())
		{
			HorseSummon(false);
			HorseSummon(true);
		}
	}
#endif
	return true;
}

namespace NPartyPickupDistribute
{
	struct FFindOwnership
	{
		LPITEM item;
		LPCHARACTER owner;
		FFindOwnership(LPITEM item) : item(item), owner(NULL) {}
		void operator () (LPCHARACTER ch)
		{
			if (item->IsOwnership(ch))
				owner = ch;
		}
	};

	struct FCountNearMember
	{
		int		total;
		int		x, y;
		FCountNearMember(LPCHARACTER center) : total(0), x(center->GetX()), y(center->GetY()) {}

		void operator () (LPCHARACTER ch)
		{
			if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				total += 1;
		}
	};

	struct FMoneyDistributor
	{
		int		total;
		LPCHARACTER	c;
		int		x, y;
		int		iMoney;

		FMoneyDistributor(LPCHARACTER center, int iMoney)
			: total(0), c(center), x(center->GetX()), y(center->GetY()), iMoney(iMoney)
		{}

		void operator ()(LPCHARACTER ch)
		{
			if (ch != c)
				if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				{
					ch->PointChange(POINT_GOLD, iMoney, true);

					if (iMoney > 1000) // ?ÂµÂ¿Ã¸ ??Â»Ã³Â¸Â¸ Â±Ã¢Â·???Â´?.
					{
						LOG_LEVEL_CHECK(LOG_LEVEL_MAX, LogManager::instance().CharLog(ch, iMoney, "GET_GOLD", ""));
					}
				}
		}
	};
}

void CHARACTER::GiveGold(long long iAmount)
{
	if (iAmount <= 0)
		return;




	sys_log(0, "GIVE_GOLD: %s %lld", GetName(), iAmount);

	if (GetParty())
	{
		LPPARTY pParty = GetParty();

		// ???Â¼Â°Â¡ ??Â´? Â°Ã¦Â¿Ã¬ Â³ÂªÂ´Â©Â¾Ã® Â°Â¡?Ã¸Â´?.
		long long dwTotal = iAmount;
		long long dwMyAmount = dwTotal;

		NPartyPickupDistribute::FCountNearMember funcCountNearMember(this);
		pParty->ForEachOnMapMember(funcCountNearMember, GetMapIndex());

		if (funcCountNearMember.total > 1)
		{
			DWORD dwShare = dwTotal / funcCountNearMember.total;
			dwMyAmount -= dwShare * (funcCountNearMember.total - 1);

			NPartyPickupDistribute::FMoneyDistributor funcMoneyDist(this, dwShare);

			pParty->ForEachOnMapMember(funcMoneyDist, GetMapIndex());
		}

		PointChange(POINT_GOLD, dwMyAmount, true);

		if (dwMyAmount > 1000) // ?ÂµÂ¿Ã¸ ??Â»Ã³Â¸Â¸ Â±Ã¢Â·???Â´?.
		{
			LOG_LEVEL_CHECK(LOG_LEVEL_MAX, LogManager::instance().CharLog(this, dwMyAmount, "GET_GOLD", ""));
		}
	}
	else
	{
		PointChange(POINT_GOLD, iAmount, true);

		if (iAmount > 1000) // ?ÂµÂ¿Ã¸ ??Â»Ã³Â¸Â¸ Â±Ã¢Â·???Â´?.
		{
			LOG_LEVEL_CHECK(LOG_LEVEL_MAX, LogManager::instance().CharLog(this, iAmount, "GET_GOLD", ""));
		}
	}
}

bool CHARACTER::CanGetGroundItem(LPITEM item, LPCHARACTER partyOwner)
{
	if (item->IsOwnership(this))
		return true;
	else if (!IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_DROP) && GetParty())
	{
		NPartyPickupDistribute::FFindOwnership funcFindOwnership(item);
		GetParty()->ForEachOnMapMember(funcFindOwnership, GetMapIndex());
		LPCHARACTER owner = funcFindOwnership.owner;
		if (!owner)
			return false;
		if (item->IsDragonSoul())
		{
			if (owner->GetEmptyDragonSoulInventory(item) == -1)
			{
				if (GetEmptyDragonSoulInventory(item) != -1)
					return true;
			}
			else
			{
				partyOwner = owner;
				return true;
			}
		}
#ifdef ENABLE_SPECIAL_STORAGE
		else if (item->IsUpgradeItem())
		{
			if (owner->GetEmptyUpgradeInventory(item) == -1)
			{
				if (GetEmptyUpgradeInventory(item) != -1)
					return true;
			}
			else
			{
				partyOwner = owner;
				return true;
			}
		}
		else if (item->IsBook())
		{
			if (owner->GetEmptyBookInventory(item) == -1)
			{
				if (GetEmptyBookInventory(item) != -1)
					return true;
			}
			else
			{
				partyOwner = owner;
				return true;
			}
		}
		else if (item->IsStone())
		{
			if (owner->GetEmptyStoneInventory(item) == -1)
			{
				if (GetEmptyStoneInventory(item) != -1)
					return true;
			}
			else
			{
				partyOwner = owner;
				return true;
			}
		}
		else if (item->IsChange())
		{
			if (owner->GetEmptyChangeInventory(item) == -1)
			{
				if (GetEmptyChangeInventory(item) != -1)
					return true;
			}
			else
			{
				partyOwner = owner;
				return true;
			}
		}
		else if (item->IsCostume())
		{
			if (owner->GetEmptyCostumeInventory(item) == -1)
			{
				if (GetEmptyCostumeInventory(item) != -1)
					return true;
			}
			else
			{
				partyOwner = owner;
				return true;
			}
		}
		else if (item->IsChest())
		{
			if (owner->GetEmptyChestInventory(item) == -1)
			{
				if (GetEmptyChestInventory(item) != -1)
					return true;
			}
			else
			{
				partyOwner = owner;
				return true;
			}
		}
#endif
		else
		{
			if (owner->GetEmptyInventory(item->GetSize()) == -1)
			{
				if (GetEmptyInventory(item->GetSize()) != -1)
					return true;
			}
			else
			{
				partyOwner = owner;
				return true;
			}
		}
	}
	return false;
}

bool CHARACTER::PickupItem(DWORD dwVID)
{
	if (dwVID == 0
		|| !IsPC()
		|| IsDead()
		|| IsObserverMode())
	{
		return false;
	}

	LPITEM pItem = ITEM_MANAGER::Instance().FindByVID(dwVID);
	if (!pItem
		|| !pItem->GetSectree())
	{
		return false;
	}

	if (!pItem->DistanceValid(this, false))
	{
		return false;
	}

	if (pItem->isLocked())
	{
		return false;
	}

	DWORD dwCount = pItem->GetCount();
	if (dwCount == 0)
	{
		return false;
	}

	if (pItem->GetType() == ITEM_QUEST)
	{
		auto* pPC = quest::CQuestManager::instance().GetPCForce(GetPlayerID());
		if (!pPC
			|| pPC->IsRunning())
		{
			ChatPacket(CHAT_TYPE_INFO, "1552");
			// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot pickup this item while you have a quest instance opened!"));
			return false;
		}
	}

	LPCHARACTER pOwner = pItem->IsOwnership(this) ?
		this : nullptr;

	if (!pOwner/* && !IS_SET(pItem->GetAntiFlag(), ITEM_ANTIFLAG_GET)*/)
	{
		LPPARTY pParty = GetParty();
		if (!pParty)
		{
			return false;
		}

/*
		if (IS_SET(pItem->GetAntiFlag(), ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_DROP))
		{
			return false;
		}
*/
		NPartyPickupDistribute::FFindOwnership funcFindOwnership(pItem);
		pParty->ForEachOnMapMember(funcFindOwnership, GetMapIndex());

		pOwner = funcFindOwnership.owner;
		if (!pOwner)
		{
			return false;
		}

		if (!pOwner->IsPC()
			|| pOwner->IsDead()
			|| pOwner->IsObserverMode())
		{
			return false;
		}

		if (!pItem->DistanceValid(pOwner, true))
		{
			return false;
		}

		if (pItem->GetType() == ITEM_QUEST)
		{
			auto* pPC = quest::CQuestManager::instance().GetPCForce(pOwner->GetPlayerID());
			if (!pPC ||
				pPC->IsRunning())
			{
				ChatPacket(CHAT_TYPE_INFO, "1553 %s", pOwner->GetName());
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot pickup this item because %s have a quest instance opened!"), pOwner->GetName());
				return false;
			}
		}
	}

	if (!pOwner)
	{
		return false;
	}

	if (pOwner->m_pkTimedEvent)
	{
		event_cancel(&pOwner->m_pkTimedEvent);
		pOwner->m_pkTimedEvent = nullptr;

		ChatPacket(CHAT_TYPE_INFO, "1554");
		// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The logout / switch CH action was cancelled."));
	}

	DWORD dwCountMsg = dwCount;

	if (pItem->IsOwnership(pOwner))
	{
		if (pItem->GetType() == ITEM_ELK)
		{
			pItem->RemoveFromGround();
			M2_DESTROY_ITEM(pItem);

			pOwner->GiveGold(dwCount);
			Save();
		}
		else
		{
			const uint32_t dwItemVnum = pItem->GetVnum();

			pItem->Lock(true);

			if (pItem->IsStackable()
				&& pItem->GetCount() < g_bItemCountLimit
				&& !IS_SET(pItem->GetAntiFlag(), ITEM_ANTIFLAG_STACK)
				&& !pItem->IsDragonSoul())
			{
				DWORD wMin = 0;

#ifdef ENABLE_SPECIAL_STORAGE
				DWORD wMax = SPECIAL_INVENTORY_MAX_NUM;
#else
				DWORD wMax = INVENTORY_MAX_NUM;
#endif

				LPITEM pItem2 = nullptr;
				for (DWORD i = wMin; i < wMax; ++i)
				{
#ifdef ENABLE_SPECIAL_STORAGE
					if (pItem->IsUpgradeItem())
					{
						pItem2 = pOwner->GetUpgradeInventoryItem(i);
					}
					else if (pItem->IsBook())
					{
						pItem2 = pOwner->GetBookInventoryItem(i);
					}
					else if (pItem->IsStone())
					{
						pItem2 = pOwner->GetStoneInventoryItem(i);
					}
					else if (pItem->IsChange())
					{
						pItem2 = pOwner->GetChangeInventoryItem(i);
					}
					else if (pItem->IsCostume())
					{
						pItem2 = pOwner->GetCostumeInventoryItem(i);
					}
					else if (pItem->IsChest())
					{
						pItem2 = pOwner->GetChestInventoryItem(i);
					}
					else
					{
#endif
					pItem2 = pOwner->GetInventoryItem(i);
#ifdef ENABLE_SPECIAL_STORAGE
					}
#endif

					if (!pItem2)
					{
						continue;
					}

					if (pItem2 == pItem)
					{
						break;
					}

					if (pItem2->IsEquipped()
						|| pItem2->IsExchanging()
						|| pItem2->isLocked())
					{
						continue;
					}

					if (dwItemVnum != pItem2->GetVnum())
					{
						continue;
					}

					if (pItem2->GetCount() == g_bItemCountLimit)
					{
						continue;
					}

					uint8_t j = 0;
					for (; j < ITEM_SOCKET_MAX_NUM; ++j)
					{
						if (pItem2->GetSocket(j) != pItem->GetSocket(j))
						{
							break;
						}
					}

					if (j != ITEM_SOCKET_MAX_NUM)
					{
						continue;
					}

					pItem2->Lock(true);

					DWORD dwCount2 = MIN(g_bItemCountLimit - pItem2->GetCount(), dwCount);
					dwCount -= dwCount2;
					pItem2->SetCount(pItem2->GetCount() + dwCount2);

					pItem2->Lock(false);

					if (dwCount == 0)
					{
						quest::CQuestManager::instance().PickupItem(pOwner->GetPlayerID(), pItem);

						char szHint[128];
						snprintf(szHint, sizeof(szHint), "%s %u %u", pItem->GetName(), pItem->GetCount(), pItem->GetOriginalVnum());
						LogManager::instance().ItemLog(pOwner, pItem, "PICKUP", szHint);

						pItem->Lock(false);
						pItem->RemoveFromGround();
						ITEM_MANAGER::instance().DestroyItem(pItem);
						pItem = nullptr;
						break;
					}
				}

				if (pItem == nullptr)
				{
					const auto* pTable = ITEM_MANAGER::instance().GetTable(dwItemVnum);
					if (pTable)
					{
						char szItemName[ITEM_NAME_MAX_LEN + 1] = {};
						snprintf(szItemName, sizeof(szItemName), "I%u", dwItemVnum);

						if (dwCountMsg > 1)
						{
							// pOwner->ChatPacket(
// #ifdef __CHAT_SETTINGS__
												// CHAT_TYPE_ITEM_INFO
// #else
												// CHAT_TYPE_INFO
// #endif
												// , LC_TEXT("You received: %s (x%u).")
												// , pTable->szLocaleName
												// , dwCountMsg);

							pOwner->ChatPacket(
#ifdef __CHAT_SETTINGS__
												CHAT_TYPE_ITEM_INFO
#else
												CHAT_TYPE_INFO
#endif
												, "1555 %s %u"
												, szItemName
												, dwCountMsg);

							if (this != pOwner)
							{
								// ChatPacket(
// #ifdef __CHAT_SETTINGS__
											// CHAT_TYPE_ITEM_INFO
// #else
											// CHAT_TYPE_INFO
// #endif
											// , LC_TEXT("%s received: %s (x%u).")
											// , pOwner->GetName()
											// , pTable->szLocaleName
											// , dwCountMsg);
							// }

								ChatPacket(
#ifdef __CHAT_SETTINGS__
											CHAT_TYPE_ITEM_INFO
#else
											CHAT_TYPE_INFO
#endif
											, "1556 %s %s %u"
											, pOwner->GetName()
											, szItemName
											, dwCountMsg);
							}
						}
						else
						{
							// pOwner->ChatPacket(
// #ifdef __CHAT_SETTINGS__
												// CHAT_TYPE_ITEM_INFO
// #else
												// CHAT_TYPE_INFO
// #endif
												// , LC_TEXT("You received: %s.")
												// , pTable->szLocaleName);

							pOwner->ChatPacket(
#ifdef __CHAT_SETTINGS__
												CHAT_TYPE_ITEM_INFO
#else
												CHAT_TYPE_INFO
#endif
												, "1557 %s"
												, szItemName);

							if (this != pOwner)
							{
								// ChatPacket(
// #ifdef __CHAT_SETTINGS__
											// CHAT_TYPE_ITEM_INFO
// #else
											// CHAT_TYPE_INFO
// #endif
											// , LC_TEXT("%s received: %s.")
											// , pOwner->GetName()
											// , pTable->szLocaleName);

								ChatPacket(
#ifdef __CHAT_SETTINGS__
											CHAT_TYPE_ITEM_INFO
#else
											CHAT_TYPE_INFO
#endif
											, "1558 %s %s"
											, pOwner->GetName()
											, szItemName);
							}
						}
					}

#ifdef ENABLE_MINI_GAME_CATCH_KING
					if (dwItemVnum == CATCH_KING_DROP_ITEM)
					{
						if (pOwner->CountSpecifyItem(CATCH_KING_DROP_ITEM) >= 25)
						{
							pOwner->RemoveSpecifyItem(CATCH_KING_DROP_ITEM, 25);
							pOwner->AutoGiveItem(CATCH_KING_PLAY_ITEM, 1);
						}
					}
#endif

					return true;
				}
			}

			uint8_t bWindow = INVENTORY;
			int32_t iEmptyCell;

			if (pItem->IsDragonSoul())
			{
				iEmptyCell = pOwner->GetEmptyDragonSoulInventory(pItem);
				bWindow = DRAGON_SOUL_INVENTORY;
			}
#ifdef ENABLE_SPECIAL_STORAGE
			else if (pItem->IsUpgradeItem())
			{
				iEmptyCell = pOwner->GetEmptyUpgradeInventory(pItem);
				bWindow = UPGRADE_INVENTORY;
			}
			else if (pItem->IsBook())
			{
				iEmptyCell = pOwner->GetEmptyBookInventory(pItem);
				bWindow = BOOK_INVENTORY;
			}
			else if (pItem->IsStone())
			{
				iEmptyCell = pOwner->GetEmptyStoneInventory(pItem);
				bWindow = STONE_INVENTORY;
			}
			else if (pItem->IsChange())
			{
				iEmptyCell = pOwner->GetEmptyChangeInventory(pItem);
				bWindow = CHANGE_INVENTORY;
			}
			else if (pItem->IsCostume())
			{
				iEmptyCell = pOwner->GetEmptyCostumeInventory(pItem);
				bWindow = COSTUME_INVENTORY;
			}
			else if (pItem->IsChest())
			{
				iEmptyCell = pOwner->GetEmptyChestInventory(pItem);
				bWindow = CHEST_INVENTORY;
			}
#endif
			else
			{
				iEmptyCell = pOwner->GetEmptyInventory(pItem->GetSize());
			}

			pItem->Lock(false);

			if (iEmptyCell == -1)
			{
				if (pOwner != this)
				{
					// ChatPacket(CHAT_TYPE_INFO
										// , LC_TEXT("%s don't have enough empty space.")
										// , pOwner->GetName());

					ChatPacket(CHAT_TYPE_INFO
										, "1559 %s"
										, pOwner->GetName());
				}
				else
				{
					// pOwner->ChatPacket(CHAT_TYPE_INFO
										// , LC_TEXT("You don't have enough empty space."));

					pOwner->ChatPacket(CHAT_TYPE_INFO
										, "1560");
				}

				return false;
			}

			pItem->RemoveFromGround();
			pItem->AddToCharacter(pOwner, TItemPos(bWindow, iEmptyCell));

			if (dwCountMsg > 1)
			{
				// pOwner->ChatPacket(
// #ifdef __CHAT_SETTINGS__
									// CHAT_TYPE_ITEM_INFO
// #else
									// CHAT_TYPE_INFO
// #endif
									// , LC_TEXT("You received: %s (x%u).")
									// , pItem->GetName()
									// , dwCountMsg);

				pOwner->ChatPacket(
#ifdef __CHAT_SETTINGS__
									CHAT_TYPE_ITEM_INFO
#else
									CHAT_TYPE_INFO
#endif
									, "1555 %s %u"
									, pItem->GetName()
									, dwCountMsg);

				if (pOwner != this)
				{
					// ChatPacket(
// #ifdef __CHAT_SETTINGS__
								// CHAT_TYPE_ITEM_INFO
// #else
								// CHAT_TYPE_INFO
// #endif
								// , LC_TEXT("%s received: %s (x%u).")
								// , pOwner->GetName()
								// , pItem->GetName()
								// , dwCountMsg);

					ChatPacket(
#ifdef __CHAT_SETTINGS__
								CHAT_TYPE_ITEM_INFO
#else
								CHAT_TYPE_INFO
#endif
								, "1556 %s %s %u"
								, pOwner->GetName()
								, pItem->GetName()
								, dwCountMsg);
				}
			}
			else
			{
				// pOwner->ChatPacket(
// #ifdef __CHAT_SETTINGS__
									// CHAT_TYPE_ITEM_INFO
// #else
									// CHAT_TYPE_INFO
// #endif
									// , LC_TEXT("You received: %s.")
									// , pItem->GetName()
									// , dwCountMsg);

				pOwner->ChatPacket(
#ifdef __CHAT_SETTINGS__
									CHAT_TYPE_ITEM_INFO
#else
									CHAT_TYPE_INFO
#endif
									, "1557 %s"
									, pItem->GetName());

				if (pOwner != this)
				{
					// ChatPacket(
// #ifdef __CHAT_SETTINGS__
								// CHAT_TYPE_ITEM_INFO
// #else
								// CHAT_TYPE_INFO
// #endif
								// , LC_TEXT("%s received: %s.")
								// , pOwner->GetName()
								// , pItem->GetName()
								// , dwCountMsg);

					ChatPacket(
#ifdef __CHAT_SETTINGS__
								CHAT_TYPE_ITEM_INFO
#else
								CHAT_TYPE_INFO
#endif
								, "1558 %s %s"
								, pOwner->GetName()
								, pItem->GetName());
				}
			}

#ifdef ENABLE_MINI_GAME_CATCH_KING
			if (dwItemVnum == CATCH_KING_DROP_ITEM)
			{
				if (pOwner->CountSpecifyItem(CATCH_KING_DROP_ITEM) >= 25)
				{
					pOwner->RemoveSpecifyItem(CATCH_KING_DROP_ITEM, 25);
					pOwner->AutoGiveItem(CATCH_KING_PLAY_ITEM, 1);
				}
			}
#endif

			quest::CQuestManager::instance().PickupItem(pOwner->GetPlayerID(), pItem);

			char szHint[128];
			snprintf(szHint, sizeof(szHint), "%s %u %u", pItem->GetName(), pItem->GetCount(), pItem->GetOriginalVnum());
			LogManager::instance().ItemLog(pOwner, pItem, "PICKUP", szHint);
		}
	}

	return true;
}

bool CHARACTER::SwapItem(BYTE bWindow, WORD wCell, BYTE bDestWindow, WORD wDestCell)
{
	if (!CanHandleItem())
	{
		return false;
	}

	TItemPos srcCell(bWindow, wCell), destCell(bDestWindow, wDestCell);
	if (srcCell.IsDragonSoulEquipPosition() || destCell.IsDragonSoulEquipPosition())
	{
		return false;
	}

	if (wCell == wDestCell)
	{
		return false;
	}

	if (srcCell.IsEquipPosition() && destCell.IsEquipPosition())
	{
		return false;
	}

	LPITEM item1, item2;
	if (srcCell.IsEquipPosition())
	{
		item1 = GetItem(destCell);
		item2 = GetItem(srcCell);
	}
	else
	{
		item1 = GetItem(srcCell);
		item2 = GetItem(destCell);
	}

	if (!item1 || !item2)
	{
		return false;
	}

	if (item1 == item2)
	{
		sys_log(0, "[WARNING][WARNING][HACK USER!] : %s %u %u", m_stName.c_str(), wCell, wDestCell);
		return false;
	}

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	if (item1->isLocked() || item2->isLocked())
	{
		return false;
	}
#endif

	if (!IsEmptyItemGrid(TItemPos(item1->GetWindow(), item1->GetCell()), item2->GetSize(), item1->GetCell()))
	{
		return false;
	}

	// Â¹?Â²? Â¾??????? ?Ã¥ÂºÃ±?Â¢Â¿Â¡ ???Â¸Â¸Ã©
	if (TItemPos(EQUIPMENT, item2->GetCell()).IsEquipPosition())
	{
		WORD wEquipCell = item2->GetCell() - INVENTORY_MAX_NUM;
		WORD wInvenCell = item1->GetCell();

		if (item2->IsDragonSoul() || item2->GetType() == ITEM_BELT) 
		{
			if (false == CanUnequipNow(item2) || false == CanEquipNow(item1))
			{
				return false;
			}
		}

		if (wEquipCell != item1->FindEquipCell(this))
		{
			return false;
		}

		item2->RemoveFromCharacter();

		if (item1->EquipTo(this, wEquipCell))
		{
			item2->AddToCharacter(this, TItemPos(bWindow, wInvenCell));
		}
		else
		{
			sys_err("SwapItem cannot equip %s! item1 %s", item2->GetName(), item1->GetName());
		}
	}
	else
	{
		WORD wCell1 = item1->GetCell();
		WORD wCell2 = item2->GetCell();

		item1->RemoveFromCharacter();
		item2->RemoveFromCharacter();

		item1->AddToCharacter(this, TItemPos(bDestWindow, wCell2));
		item2->AddToCharacter(this, TItemPos(bWindow, wCell1));
	}

	return true;
}

bool CHARACTER::UnequipItem(LPITEM item)
{
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	int iWearCell = item->FindEquipCell(this);
	if (iWearCell == WEAR_WEAPON)
	{
		LPITEM costumeWeapon = GetWear(WEAR_COSTUME_WEAPON);
		if (costumeWeapon && !UnequipItem(costumeWeapon))
		{
			ChatPacket(CHAT_TYPE_INFO, "300");
			return false;
		}
	}
#endif
	const time_t current = get_global_time();

	if (item->GetType() == ITEM_BOOSTER && current > item->GetSocket(0)) {
		ChatPacket(CHAT_TYPE_INFO, "socket0 %d", item->GetSocket(0));
		item->SetSocket(0, 0);
	}

	if (iWearCell == WEAR_COSTUME_SASH)
	{
		LPITEM costumeAcce = GetWear(WEAR_COSTUME_NEW_SASH);
		if (costumeAcce && !UnequipItem(costumeAcce))
		{
			ChatPacket(CHAT_TYPE_INFO, "You cannot unequip your sash now. De-equip Costume sash Skin first.");
			return false;
		}
	}

	if (iWearCell == WEAR_WEAPON && getWeaponStatus() == 1)
	{
		ChatPacket(CHAT_TYPE_INFO, "You cannot unequip your weapon now.");
		return false;
	}

	if (false == CanUnequipNow(item))
		return false;

	int pos;
	if (item->IsDragonSoul())
		pos = GetEmptyDragonSoulInventory(item);
	else
		pos = GetEmptyInventory(item->GetSize());

	// HARD CODING
	if (item->GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		ShowAlignment(true);

	item->RemoveFromCharacter();

	if (item->IsDragonSoul())
	{
		item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, pos));
	}
	else
		item->AddToCharacter(this, TItemPos(INVENTORY, pos));


#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (item->IsCostumeMount())
	{
		if (GetHorse())
			HorseSummon(false);
	}
#endif

#ifdef ENABLE_MOUNT_SKIN
	if (item->IsCostumeMountSkin())
	{
		if (GetHorse())
			HorseSummon(false);
		HorseSummon(true);
	}
#endif

#ifdef ENABLE_MOUNT_SYSTEM
	if (item->IsRideItem())
	{
		CMountSystem* MountSystem = GetMountSystem();
		if (MountSystem)
		{
			if (MountSystem->CountSummoned() != 0)
			{
				MountSystem->Unsummon(item->GetValue(2));
				ChatPacket(CHAT_TYPE_INFO, "Skin Montura Guardada.");
			}
		}
	}
#endif


	CheckMaximumPoints();

	return true;
}

//
// @version	05/07/05 Bang2ni - Skill Â»Ã§Â¿Ã«?? 1.5 ?? ??Â³Â»Â¿Â¡ ?Ã¥ÂºÃ± ?Ã¸Â¿Ã« Â±??Ã¶
//
bool CHARACTER::EquipItem(LPITEM item, int iCandidateCell)
{
	if (item->IsExchanging())
		return false;
	if (false == item->IsEquipable())
		return false;
	if (false == CanEquipNow(item))
		return false;
	int iWearCell = item->FindEquipCell(this, iCandidateCell);

	if (iWearCell < 0)
		return false;

	if (iWearCell == WEAR_WEAPON && getWeaponStatus() == 1)
	{
		ChatPacket(CHAT_TYPE_INFO, "You cannot change weapon now.");
		return false;
	}

	if (iWearCell == WEAR_BODY && IsRiding() && (item->GetVnum() >= 11901 && item->GetVnum() <= 11904))
	{
		ChatPacket(CHAT_TYPE_INFO, "309");
		return false;
	}

	if (iWearCell != WEAR_ARROW && IsPolymorphed())
	{
		ChatPacket(CHAT_TYPE_INFO, "310");
		return false;
	}

	if (FN_check_item_sex(this, item) == false)
	{
		ChatPacket(CHAT_TYPE_INFO, "280");
		return false;
	}

	//Â½?Â±? ?Â»Â°? Â»Ã§Â¿Ã«Â½? Â±Ã¢?Â¸ Â¸Â» Â»Ã§Â¿Ã«Â¿Â©Âº? ?Â¼?Â©
	if (item->IsRideItem() && IsRiding())
	{
		ChatPacket(CHAT_TYPE_INFO, "311");// you can modify here...
		return false;
	}

	DWORD dwCurTime = get_dword_time();

	if ((iWearCell != WEAR_ARROW) && (iWearCell != WEAR_PENDANT_SOUL_FIRST) && (iWearCell != WEAR_PENDANT_SOUL_SECOND) &&
		(dwCurTime - GetLastAttackTime() <= 1500 || dwCurTime - m_dwLastSkillTime <= 1500))
	{
		ChatPacket(CHAT_TYPE_INFO, "312");
		return false;
	}

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	if (iWearCell == WEAR_WEAPON)
	{
		if (item->GetType() == ITEM_WEAPON)
		{
			LPITEM costumeWeapon = GetWear(WEAR_COSTUME_WEAPON);
			if (costumeWeapon && costumeWeapon->GetValue(3) != item->GetSubType() && !UnequipItem(costumeWeapon))
			{
				ChatPacket(CHAT_TYPE_INFO, "300");
				return false;
			}
		}
		else //fishrod/pickaxe
		{
			LPITEM costumeWeapon = GetWear(WEAR_COSTUME_WEAPON);
			if (costumeWeapon && !UnequipItem(costumeWeapon))
			{
				ChatPacket(CHAT_TYPE_INFO, "300");
				return false;
			}
		}
	}
	else if (iWearCell == WEAR_COSTUME_WEAPON)
	{
		if (item->GetType() == ITEM_COSTUME && item->GetSubType() == COSTUME_WEAPON)
		{
			LPITEM pkWeapon = GetWear(WEAR_WEAPON);
			if (!pkWeapon || pkWeapon->GetType() != ITEM_WEAPON || item->GetValue(3) != pkWeapon->GetSubType())
			{
				ChatPacket(CHAT_TYPE_INFO, "313");
				return false;
			}
		}
	}
#endif

	if ((item->GetType() == ITEM_BOOSTER) && (item->GetSocket(0) <= get_global_time()))
	{
		ChatPacket(CHAT_TYPE_INFO, "Item expired. Use Booster Time Extend to add more.");

		item->Lock(true);
		return false;
	}

	if (item->GetType() == ITEM_NEW_RINGS)
	{

		LPITEM rings1 = GetWear(WEAR_RINGS_1);
		LPITEM rings2 = GetWear(WEAR_RINGS_2);

		if (rings1)
		{

			if (rings1->GetAccessorySocketVnumRings(rings1) == item->GetAccessorySocketVnumRings(item))
			{
				ChatPacket(CHAT_TYPE_INFO, "No puede equipar el mismo anillo.");
				return false;
			}
		}

		if (rings2)
		{
			if (rings2->GetAccessorySocketVnumRings(rings2) == item->GetAccessorySocketVnumRings(item))
			{
				ChatPacket(CHAT_TYPE_INFO, "No puede equipar el mismo anillo.");
				return false;
			}
		}

	}

	// Unstack objects when equip[ITEM_UNIQUE]
	if (item->IsEquipable() && item->GetType() == ITEM_UNIQUE && item->GetCount() > 1)
	{
		auto pos = GetEmptyInventory(item->GetSize());
		if (pos == -1)
		{
			return false;
		}

		auto newItem = ITEM_MANAGER::Instance().CreateItem(item->GetVnum());
		if (!newItem)
			return false;

		item->SetCount(item->GetCount() - 1);
		newItem->AddToCharacter(this, TItemPos(INVENTORY, pos));

		item = newItem;
	}

	// Â¿Ã«?Â¥Â¼Â® ?Â¯Â¼Ã¶ ?Â³Â¸Â®
	if (item->IsDragonSoul())
	{
		// Â°Â°?Âº ?Â¸???? Â¿Ã«?Â¥Â¼Â®?? ??Â¹? ÂµÃ©Â¾Ã®Â°Â¡ ??Â´?Â¸Ã© ?Ã¸Â¿Ã«?? Â¼Ã¶ Â¾Ã¸Â´?.
		// Â¿Ã«?Â¥Â¼Â®?Âº swap?Â» ?Ã¶Â¿Ã¸??Â¸Ã© Â¾?Âµ?.
		if (GetInventoryItem(INVENTORY_MAX_NUM + iWearCell))
		{
			ChatPacket(CHAT_TYPE_INFO, "??Â¹? Â°Â°?Âº ?Â¾Â·Ã¹?? Â¿Ã«?Â¥Â¼Â®?Â» ?Ã¸Â¿Ã«??Â°Ã­ ??Â½?Â´?Â´?.");
			return false;
		}

		if (!item->EquipTo(this, iWearCell))
		{
			return false;
		}
	}
	// Â¿Ã«?Â¥Â¼Â®?? Â¾?Â´?.
	else
	{
		// ?Ã¸Â¿Ã«?? Â°Ã·Â¿Â¡ Â¾??????? ??Â´?Â¸Ã©,
		if (GetWear(iWearCell) && !IS_SET(GetWear(iWearCell)->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		{
			// ?? Â¾??????Âº ??Â¹Ã¸ Â¹??Ã·Â¸Ã© ÂºÂ¯Â°Ã¦ Âº?Â°Â¡. swap Â¿ÂªÂ½? Â¿??Ã¼ Âº?Â°Â¡
			if (item->GetWearFlag() == WEARABLE_ABILITY)
				return false;

			if (false == SwapItem(item->GetWindow(), item->GetCell(), INVENTORY, INVENTORY_MAX_NUM + iWearCell))
			{
				return false;
			}
		}
		else
		{
			WORD wOldCell = item->GetCell();

			if (item->EquipTo(this, iWearCell))
			{
				SyncQuickslot(QUICKSLOT_TYPE_ITEM, wOldCell, iWearCell);
			}
		}
	}

	if (true == item->IsEquipped())
	{
		// Â¾????? ???? Â»Ã§Â¿Ã« ????Âº???Â´? Â»Ã§Â¿Ã«???Ã¶ Â¾?Â¾?ÂµÂµ Â½?Â°Â£?? ?Ã·Â°Â¨Âµ?Â´? Â¹Ã¦Â½? ?Â³Â¸Â®.
		if (-1 != item->GetProto()->cLimitRealTimeFirstUseIndex)
		{
			// ?? Â¹Ã¸??Â¶Ã³ÂµÂµ Â»Ã§Â¿Ã«?? Â¾????????Ã¶ Â¿Â©Âº?Â´? Socket1?Â» ÂºÂ¸Â°Ã­ ??Â´???Â´?. (Socket1Â¿Â¡ Â»Ã§Â¿Ã«?Â½Â¼Ã¶ Â±Ã¢Â·?)
			if (0 == item->GetSocket(1))
			{
				// Â»Ã§Â¿Ã«Â°Â¡Â´?Â½?Â°Â£?Âº Default Â°Âª?Â¸Â·? Limit Value Â°Âª?Â» Â»Ã§Â¿Ã«??Âµ?, Socket0Â¿Â¡ Â°Âª?? ???Â¸Â¸Ã© Â±? Â°Âª?Â» Â»Ã§Â¿Ã«??ÂµÂµÂ·? ??Â´?. (Â´??Â§Â´? ??)
				long duration = (0 != item->GetSocket(0)) ? item->GetSocket(0) : item->GetProto()->aLimits[(unsigned char)(item->GetProto()->cLimitRealTimeFirstUseIndex)].lValue;

				if (0 == duration)
					duration = 60 * 60 * 24 * 7;

				item->SetSocket(0, time(0) + duration);
				item->StartRealTimeExpireEvent();
			}

			item->SetSocket(1, item->GetSocket(1) + 1);
		}

		if (item->GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
			ShowAlignment(false);

		const DWORD& dwVnum = item->GetVnum();

		// Â¶Ã³Â¸Â¶Â´? ??ÂºÂ¥?Â® ??Â½?Â´??? Â¹??Ã¶(71135) ?Ã¸Â¿Ã«Â½? ???Ã¥?Â® Â¹?ÂµÂ¿
		if (true == CItemVnumHelper::IsRamadanMoonRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_RAMADAN_RING);
		}
		// ??Â·??Â© Â»Ã§??(71136) ?Ã¸Â¿Ã«Â½? ???Ã¥?Â® Â¹?ÂµÂ¿
		else if (true == CItemVnumHelper::IsHalloweenCandy(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HALLOWEEN_CANDY);
		}
		// ??ÂºÂ¹?? Â¹??Ã¶(71143) ?Ã¸Â¿Ã«Â½? ???Ã¥?Â® Â¹?ÂµÂ¿
		else if (true == CItemVnumHelper::IsHappinessRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HAPPINESS_RING);
		}
		// Â»Ã§Â¶Ã»?? ??Â´Ã¸?Â®(71145) ?Ã¸Â¿Ã«Â½? ???Ã¥?Â® Â¹?ÂµÂ¿
		else if (true == CItemVnumHelper::IsLovePendant(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_LOVE_PENDANT);
		}
		// ITEM_UNIQUE?? Â°Ã¦Â¿Ã¬, SpecialItemGroupÂ¿Â¡ ?Â¤??Âµ?Â¾Ã® ??Â°Ã­, (item->GetSIGVnum() != NULL)
		//
		else if (ITEM_UNIQUE == item->GetType() && 0 != item->GetSIGVnum())
		{
			const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(item->GetSIGVnum());
			if (NULL != pGroup)
			{
				const CSpecialAttrGroup* pAttrGroup = ITEM_MANAGER::instance().GetSpecialAttrGroup(pGroup->GetAttrVnum(item->GetVnum()));
				if (NULL != pAttrGroup)
				{
					const std::string& std = pAttrGroup->m_stEffectFileName;
					SpecificEffectPacket(std.c_str());
				}
			}
		}
#ifdef __SASH_SYSTEM__
		else if ((item->GetType() == ITEM_COSTUME) && (item->GetSubType() == COSTUME_SASH))
			this->EffectPacket(SE_EFFECT_SASH_EQUIP);
#endif
		if (
			(ITEM_UNIQUE == item->GetType() && UNIQUE_SPECIAL_RIDE == item->GetSubType() && IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE))
			|| (ITEM_UNIQUE == item->GetType() && UNIQUE_SPECIAL_MOUNT_RIDE == item->GetSubType() && IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE))

			)
		{
			quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
		}

#if defined(WJ_COMBAT_ZONE)
#ifdef ENABLE_MOUNT_SYSTEM
		if (item->IsRideItem() && !CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		{
			CMountSystem* MountSystem = GetMountSystem();
			if (MountSystem)
			{
				if (MountSystem->CountSummoned() <= 0)
				{
					CMountActor* MState = MountSystem->Summon(item->GetValue(2), GetName(), item);
					if (MState)
					{
						ChatPacket(CHAT_TYPE_INFO, "Skin Montura Invocada.");
					}
				}
			}
		}
#endif
#endif

	}
#ifdef __ITEM_SHINING__
	if (item->GetType() == ITEM_SHINING)
		this->UpdatePacket();
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (item->IsCostumeMount()
#ifdef ENABLE_MOUNT_SKIN
		|| item->IsCostumeMountSkin()
#endif
		)
	{
		if (GetHorse())
			HorseSummon(false);
		HorseSummon(true);
	}
#endif

	bool allPendantsEquipped = true;
	for (BYTE i = WEAR_PENDANT_FIRE; i <= WEAR_PENDANT_EARTH; ++i)
	{
		LPITEM pendantItem = GetWear(i);

		// If any pendant is missing or its refine level is not 25, set the flag to false
		if (!pendantItem || pendantItem->GetRefineLevel() != 25)
		{
			allPendantsEquipped = false;
			break;
		}
	}

	// If all pendants are equipped and refined to 25, grant the reward
	if (allPendantsEquipped)
	{
		CHARACTER_MANAGER::Instance().SetRewardData(REWARD_FIRSTPENDANTWHEEL, GetName(), true);
	}

	return true;
}

void CHARACTER::BuffOnAttr_AddBuffsFromItem(LPITEM pItem)
{
	for (size_t i = 0; i < sizeof(g_aBuffOnAttrPoints) / sizeof(g_aBuffOnAttrPoints[0]); i++)
	{
		TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(g_aBuffOnAttrPoints[i]);
		if (it != m_map_buff_on_attrs.end())
		{
			it->second->AddBuffFromItem(pItem);
		}
	}
}

void CHARACTER::BuffOnAttr_RemoveBuffsFromItem(LPITEM pItem)
{
	for (size_t i = 0; i < sizeof(g_aBuffOnAttrPoints) / sizeof(g_aBuffOnAttrPoints[0]); i++)
	{
		TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(g_aBuffOnAttrPoints[i]);
		if (it != m_map_buff_on_attrs.end())
		{
			it->second->RemoveBuffFromItem(pItem);
		}
	}
}

void CHARACTER::BuffOnAttr_ClearAll()
{
	for (TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.begin(); it != m_map_buff_on_attrs.end(); it++)
	{
		CBuffOnAttributes* pBuff = it->second;
		if (pBuff)
		{
			pBuff->Initialize();
		}
	}
}

void CHARACTER::BuffOnAttr_ValueChange(BYTE bType, BYTE bOldValue, BYTE bNewValue)
{
	TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(bType);

	if (0 == bNewValue)
	{
		if (m_map_buff_on_attrs.end() == it)
			return;
		else
			it->second->Off();
	}
	else if (0 == bOldValue)
	{
		CBuffOnAttributes* pBuff = NULL;
		if (m_map_buff_on_attrs.end() == it)
		{
			switch (bType)
			{
			case POINT_ENERGY:
			{
#ifdef ENABLE_YOHARA_SYSTEM
				static WORD abSlot[] = { WEAR_BODY, WEAR_HEAD, WEAR_FOOTS, WEAR_WRIST, WEAR_WEAPON, WEAR_NECK, WEAR_EAR, WEAR_SHIELD, WEAR_GLOVE };
#else
				static WORD abSlot[] = { WEAR_BODY, WEAR_HEAD, WEAR_FOOTS, WEAR_WRIST, WEAR_WEAPON, WEAR_NECK, WEAR_EAR, WEAR_SHIELD };
#endif
				static std::vector <WORD> vec_slots(abSlot, abSlot + _countof(abSlot));
				pBuff = M2_NEW CBuffOnAttributes(this, bType, &vec_slots);
			}
			break;
			case POINT_COSTUME_ATTR_BONUS:
			{
				static WORD abSlot[] = {
					WEAR_COSTUME_BODY,
					WEAR_COSTUME_HAIR,
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
						WEAR_COSTUME_WEAPON,
#endif
						WEAR_COSTUME_NEW_SASH,
				};
				static std::vector <WORD> vec_slots(abSlot, abSlot + _countof(abSlot));
				pBuff = M2_NEW CBuffOnAttributes(this, bType, &vec_slots);
			}
			break;
			default:
				break;
			}
			m_map_buff_on_attrs.insert(TMapBuffOnAttrs::value_type(bType, pBuff));

		}
		else
			pBuff = it->second;
		if (pBuff != NULL)
			pBuff->On(bNewValue);
	}
	else
	{
		assert(m_map_buff_on_attrs.end() != it);
		it->second->ChangeBuffValue(bNewValue);
	}
}



LPITEM CHARACTER::FindSpecifyItem(DWORD vnum) const
{
#if defined(__BL_BATTLE_ROYALE__) || defined(UNLOCK_INVENTORY_WORLDARD)
	for (UINT i = 0; i < GetInventorySize(); ++i)
#else
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (GetInventoryItem(i) && GetInventoryItem(i)->GetVnum() == vnum)
			return GetInventoryItem(i);
	}

#ifdef ENABLE_SPECIAL_STORAGE
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(4)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (GetChangeInventoryItem(i) && GetChangeInventoryItem(i)->GetVnum() == vnum)
			return GetChangeInventoryItem(i);
	}

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(5)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (GetCostumeInventoryItem(i) && GetCostumeInventoryItem(i)->GetVnum() == vnum)
			return GetCostumeInventoryItem(i);
	}

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(6)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (GetChestInventoryItem(i) && GetChestInventoryItem(i)->GetVnum() == vnum)
			return GetChestInventoryItem(i);
	}

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(2)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (GetBookInventoryItem(i) && GetBookInventoryItem(i)->GetVnum() == vnum)
			return GetBookInventoryItem(i);
	}
#endif

	return NULL;
}


LPITEM CHARACTER::FindItemByID(DWORD id) const
{
#if defined(__BL_BATTLE_ROYALE__) || defined(UNLOCK_INVENTORY_WORLDARD)
	for (UINT i = 0; i < GetInventorySize(); ++i)
#else
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (NULL != GetInventoryItem(i) && GetInventoryItem(i)->GetID() == id)
			return GetInventoryItem(i);
	}

	for (WORD i = BELT_INVENTORY_SLOT_START; i < BELT_INVENTORY_SLOT_END; ++i)
	{
		if (NULL != GetInventoryItem(i) && GetInventoryItem(i)->GetID() == id)
			return GetInventoryItem(i);
	}

	return NULL;
}


#ifdef ENABLE_BUY_ITEMS_WORLDARD
int CHARACTER::CountSpecifyItemBuy(DWORD vnum) const
{
	int	count = 0;
	LPITEM item;

#if defined(__BL_BATTLE_ROYALE__) || defined(UNLOCK_INVENTORY_WORLDARD)
	for (UINT i = 0; i < GetInventorySize(); ++i)
#else
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			// Â°Â³?? Â»Ã³?Â¡Â¿Â¡ ÂµÃ®Â·?Âµ? Â¹Â°Â°???Â¸Ã© Â³?Â¾Ã®Â°Â£Â´?.
			count += item->GetCount();
		}
	}
#ifdef ENABLE_SPECIAL_STORAGE
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(1)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetUpgradeInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(2)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetBookInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(3)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetStoneInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(4)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetChangeInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(5)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetCostumeInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(6)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetChestInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}
#endif
	return count;
}
#endif

int CHARACTER::CountSpecifyItem(DWORD vnum) const
{
	int	count = 0;
	LPITEM item;

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize()); ++i)
#else
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}

#ifdef ENABLE_SPECIAL_STORAGE
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(1)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetUpgradeInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(2)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetBookInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(3)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetStoneInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(4)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetChangeInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(5)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetCostumeInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < (GetInventorySize(6)); ++i)
#else
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		item = GetChestInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}
#endif
	return count;
}

void CHARACTER::RemoveSpecifyItem(DWORD vnum, DWORD count)
{
	if (0 == count)
		return;

#if defined(__BL_BATTLE_ROYALE__) || defined(UNLOCK_INVENTORY_WORLDARD)
	for (UINT i = 0; i < GetInventorySize(); ++i)
#else
	for (UINT i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (NULL == GetInventoryItem(i))
			continue;

		if (GetInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetInventoryItem(i)->GetCount())
		{
			count -= GetInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetInventoryItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}
#ifdef ENABLE_SPECIAL_STORAGE
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (UINT i = 0; i < (GetInventorySize(1)); ++i)
#else
	for (UINT i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (NULL == GetUpgradeInventoryItem(i))
			continue;

		if (GetUpgradeInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (count >= GetUpgradeInventoryItem(i)->GetCount())
		{
			count -= GetUpgradeInventoryItem(i)->GetCount();
			GetUpgradeInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetUpgradeInventoryItem(i)->SetCount(GetUpgradeInventoryItem(i)->GetCount() - count);
			return;
		}
	}
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (UINT i = 0; i < (GetInventorySize(2)); ++i)
#else
	for (UINT i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (NULL == GetBookInventoryItem(i))
			continue;

		if (GetBookInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (count >= GetBookInventoryItem(i)->GetCount())
		{
			count -= GetBookInventoryItem(i)->GetCount();
			GetBookInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetBookInventoryItem(i)->SetCount(GetBookInventoryItem(i)->GetCount() - count);
			return;
		}
	}
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (UINT i = 0; i < (GetInventorySize(3)); ++i)
#else
	for (UINT i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (NULL == GetStoneInventoryItem(i))
			continue;

		if (GetStoneInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (count >= GetStoneInventoryItem(i)->GetCount())
		{
			count -= GetStoneInventoryItem(i)->GetCount();
			GetStoneInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetStoneInventoryItem(i)->SetCount(GetStoneInventoryItem(i)->GetCount() - count);
			return;
		}
	}

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (UINT i = 0; i < (GetInventorySize(4)); ++i)
#else
	for (UINT i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (NULL == GetChangeInventoryItem(i))
			continue;

		if (GetChangeInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (count >= GetChangeInventoryItem(i)->GetCount())
		{
			count -= GetChangeInventoryItem(i)->GetCount();
			GetChangeInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetChangeInventoryItem(i)->SetCount(GetChangeInventoryItem(i)->GetCount() - count);
			return;
		}
	}
#ifdef UNLOCK_INVENTORY_WORLDARD
	for (UINT i = 0; i < (GetInventorySize(5)); ++i)
#else
	for (UINT i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (NULL == GetCostumeInventoryItem(i))
			continue;

		if (GetCostumeInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (count >= GetCostumeInventoryItem(i)->GetCount())
		{
			count -= GetCostumeInventoryItem(i)->GetCount();
			GetCostumeInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetCostumeInventoryItem(i)->SetCount(GetCostumeInventoryItem(i)->GetCount() - count);
			return;
		}
	}

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (UINT i = 0; i < (GetInventorySize(6)); ++i)
#else
	for (UINT i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (NULL == GetChestInventoryItem(i))
			continue;

		if (GetChestInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (count >= GetChestInventoryItem(i)->GetCount())
		{
			count -= GetChestInventoryItem(i)->GetCount();
			GetChestInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetChestInventoryItem(i)->SetCount(GetChestInventoryItem(i)->GetCount() - count);
			return;
		}
	}

#endif
	// Â¿Â¹Â¿??Â³Â¸Â®Â°Â¡ Â¾???Â´?.
	if (count)
		sys_log(0, "CHARACTER::RemoveSpecifyItem cannot remove enough item vnum %u, still remain %d", vnum, count);
}

int CHARACTER::CountSpecifyTypeItem(BYTE type) const
{
	int count = 0;

#if defined(__BL_BATTLE_ROYALE__) || defined(UNLOCK_INVENTORY_WORLDARD)
	for (WORD i = 0; i < GetInventorySize(); ++i)
#else
	for (WORD i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		LPITEM pItem = GetInventoryItem(i);
		if (pItem != NULL && pItem->GetType() == type)
		{
			count += pItem->GetCount();
		}
	}

	return count;
}

void CHARACTER::RemoveSpecifyTypeItem(BYTE type, DWORD count)
{
	if (0 == count)
		return;

#if defined(__BL_BATTLE_ROYALE__) || defined(UNLOCK_INVENTORY_WORLDARD)
	for (UINT i = 0; i < GetInventorySize(); ++i)
#else
	for (UINT i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (NULL == GetInventoryItem(i))
			continue;

		if (GetInventoryItem(i)->GetType() != type)
			continue;

		if (count >= GetInventoryItem(i)->GetCount())
		{
			count -= GetInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetInventoryItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}
}

void CHARACTER::AutoGiveItem(LPITEM item, bool longOwnerShip)
{
	if (!item)
	{
		sys_err("NULL point.");
		return;
	}

	if (item->GetOwner() && item->GetOwner() != this)
	{
		sys_err("item %d 's owner exists!", item->GetID());
		return;
	}

	auto proto = ITEM_MANAGER::instance().GetTable(item->GetVnum());
	if (!proto)
	{
		M2_DESTROY_ITEM(item);
		return;
	}

	const auto origCount = item->GetCount();
	DWORD bCount = item->GetCount();

	if (item->GetType() == ITEM_BLEND)
	{
		for (int i = 0; i < INVENTORY_MAX_NUM; i++)
		{
			LPITEM inv_item = GetInventoryItem(i);
			if (!inv_item)
			{
				continue;
			}

			if (inv_item->GetType() == ITEM_BLEND && inv_item->GetVnum() == item->GetVnum())
			{
				if (inv_item->GetSocket(0) == item->GetSocket(0) && inv_item->GetSocket(1) == item->GetSocket(1) && inv_item->GetSocket(2) == item->GetSocket(2) && inv_item->GetCount() < g_bItemCountLimit)
				{
					inv_item->SetCount(inv_item->GetCount() + item->GetCount());
					M2_DESTROY_ITEM(item);
					return;
				}
			}
		}
	}
	else
	{
		if (item->IsStackable()
			&& !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK)
			&& item->GetCount() < g_bItemCountLimit)
		{
#ifdef ENABLE_SPECIAL_STORAGE
			if (item->IsUpgradeItem())
			{
				for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
				{
					LPITEM item2 = GetUpgradeInventoryItem(i);
					if (!item2)
					{
						continue;
					}

					if (item2->GetVnum() == item->GetVnum())
					{
						DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
						bCount -= bCount2;
						item2->SetCount(item2->GetCount() + bCount2);
						if (bCount == 0)
						{
							if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
							{
								quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
							}

							M2_DESTROY_ITEM(item);
							return;
						}
					}
				}
			}
			else if (item->IsBook())
			{
				for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
				{
					LPITEM item2 = GetBookInventoryItem(i);
					if (!item2)
					{
						continue;
					}

					if (item2->GetVnum() == item->GetVnum())
					{
						int mithras;
						for (mithras = 0; mithras < ITEM_SOCKET_MAX_NUM; ++mithras)
						{
							if (item2->GetSocket(mithras) != item->GetSocket(mithras))
							{
								break;
							}
						}

						if (mithras != ITEM_SOCKET_MAX_NUM)
						{
							continue;
						}

						const DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
						bCount -= bCount2;
						item2->SetCount(item2->GetCount() + bCount2);
						if (bCount == 0)
						{
							if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
							{
								quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
							}

							M2_DESTROY_ITEM(item);
							return;
						}
					}
				}
			}
			else if (item->IsStone())
			{
				for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
				{
					LPITEM item2 = GetStoneInventoryItem(i);
					if (!item2)
					{
						continue;
					}

					if (item2->GetVnum() == item->GetVnum())
					{
						const DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
						bCount -= bCount2;
						item2->SetCount(item2->GetCount() + bCount2);
						if (bCount == 0)
						{
							if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
							{
								quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
							}

							M2_DESTROY_ITEM(item);
							return;
						}
					}
				}
			}
			else if (item->IsChange())
			{
				for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
				{
					LPITEM item2 = GetChangeInventoryItem(i);
					if (!item2)
					{
						continue;
					}

					if (item2->GetVnum() == item->GetVnum())
					{
						const DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
						bCount -= bCount2;
						item2->SetCount(item2->GetCount() + bCount2);
						if (bCount == 0)
						{
							if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
							{
								quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
							}

							M2_DESTROY_ITEM(item);
							return;
						}
					}
				}
			}
			else if (item->IsCostume())
			{
				for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
				{
					LPITEM item2 = GetCostumeInventoryItem(i);
					if (!item2)
					{
						continue;
					}

					if (item2->GetVnum() == item->GetVnum())
					{
						const DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
						bCount -= bCount2;
						item2->SetCount(item2->GetCount() + bCount2);
						if (bCount == 0)
						{
							if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
							{
								quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
							}

							M2_DESTROY_ITEM(item);
							return;
						}
					}
				}
			}
			else if (item->IsChest())
			{
				for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
				{
					LPITEM item2 = GetChestInventoryItem(i);
					if (!item2)
					{
						continue;
					}

					if (item2->GetVnum() == item->GetVnum())
					{
						const DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
						bCount -= bCount2;
						item2->SetCount(item2->GetCount() + bCount2);
						if (bCount == 0)
						{
							if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
							{
								quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
							}

							M2_DESTROY_ITEM(item);
							return;
						}
					}
				}
			}
			else
			{
#endif
				for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
				{
					LPITEM invItem = GetInventoryItem(i);
					if (!invItem)
						continue;

					if (item == invItem)
						continue;

					if (invItem->GetCount() >= g_bItemCountLimit)
						continue;

// #ifdef ENABLE_SORT_INVEN
					// if (invItem->GetOriginalVnum() == item->GetVnum() && FN_check_item_socket(invItem))
// #else
					// if (invItem->GetVnum() == item->GetVnum() && FN_check_item_socket(invItem))
// #endif
					if (invItem->GetVnum() == item->GetVnum())
					{
						int j = 0;
						for (; j < ITEM_SOCKET_MAX_NUM; ++j)
							if (invItem->GetSocket(j) != item->GetSocket(j))
								break;

						if (j != ITEM_SOCKET_MAX_NUM)
							continue;

						auto bCount2 = MIN(g_bItemCountLimit - invItem->GetCount(), bCount);
						bCount -= bCount2;
						invItem->SetCount(invItem->GetCount() + bCount2);
						// if (IS_SET(proto->dwFlags, ITEM_FLAG_MAKECOUNT))
						// {
							// if (bCount < proto->alValues[1])
							// {
								// bCount = proto->alValues[1];
							// }
						// }

						// const DWORD bCount2 = MIN(g_bItemCountLimit - invItem->GetCount(), bCount);
						// bCount -= bCount2;
						// invItem->SetCount(invItem->GetCount() + bCount2);

						if (bCount == 0)
						{
							if (invItem->GetType() == ITEM_QUEST || invItem->GetType() == ITEM_MATERIAL)
							{
								quest::CQuestManager::instance().PickupItem(GetPlayerID(), invItem);
							}

							M2_DESTROY_ITEM(item);
							return;
						}
					}
				}
#ifdef ENABLE_SPECIAL_STORAGE
			}
#endif

			if (origCount != bCount)
			{
				item->SetCount(bCount);
			}
		}
	}

	int cell;
	if (item->IsDragonSoul())
	{
		cell = GetEmptyDragonSoulInventory(item);
	}
#ifdef ENABLE_SPECIAL_STORAGE
	else if (item->IsUpgradeItem())
	{
		cell = GetEmptyUpgradeInventory(item);
	}
	else if (item->IsBook())
	{
		cell = GetEmptyBookInventory(item);
	}
	else if (item->IsStone())
	{
		cell = GetEmptyStoneInventory(item);
	}
	else if (item->IsChange())
	{
		cell = GetEmptyChangeInventory(item);
	}
	else if (item->IsCostume())
	{
		cell = GetEmptyCostumeInventory(item);
	}
	else if (item->IsChest())
	{
		cell = GetEmptyChestInventory(item);
	}
#endif
	else
	{
		cell = GetEmptyInventory(item->GetSize());
	}

	if (cell != -1)
	{
		if (item->IsDragonSoul())
		{
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, cell));
		}
#ifdef ENABLE_SPECIAL_STORAGE
		else if (item->IsUpgradeItem())
		{
			item->AddToCharacter(this, TItemPos(UPGRADE_INVENTORY, cell));
		}
		else if (item->IsBook())
		{
			item->AddToCharacter(this, TItemPos(BOOK_INVENTORY, cell));
		}
		else if (item->IsStone())
		{
			item->AddToCharacter(this, TItemPos(STONE_INVENTORY, cell));
		}
		else if (item->IsChange())
		{
			item->AddToCharacter(this, TItemPos(CHANGE_INVENTORY, cell));
		}
		else if (item->IsCostume())
		{
			item->AddToCharacter(this, TItemPos(COSTUME_INVENTORY, cell));
		}
		else if (item->IsChest())
		{
			item->AddToCharacter(this, TItemPos(CHEST_INVENTORY, cell));
		}
#endif
		else
		{
			item->AddToCharacter(this, TItemPos(INVENTORY, cell));
		}


		LogManager::instance().ItemLog(this, item, "SYSTEM", item->GetName());

		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION)
		{
			TQuickslot* pSlot;
			if (GetQuickslot(0, &pSlot) && pSlot->type == QUICKSLOT_TYPE_NONE)
			{
				TQuickslot slot;
				slot.type = QUICKSLOT_TYPE_ITEM;
				slot.pos = cell;
				SetQuickslot(0, slot);
			}
		}

		if (item->GetType() == ITEM_QUEST || item->GetType() == ITEM_MATERIAL)
		{
			quest::CQuestManager::instance().PickupItem(GetPlayerID(), item);
		}
	}
	else
	{
		item->AddToGround(GetMapIndex(), GetXYZ());
#ifdef ENABLE_NEWSTUFF
		item->StartDestroyEvent(g_aiItemDestroyTime[ITEM_DESTROY_TIME_AUTOGIVE]);
#else
		item->StartDestroyEvent();
#endif

		if (longOwnerShip)
		{
			item->SetOwnership(this, 300);
		}
		else
		{
			item->SetOwnership(this, 60);
		}

		LogManager::instance().ItemLog(this, item, "SYSTEM_DROP", item->GetName());
	}
}

LPITEM CHARACTER::AutoGiveItem(DWORD dwItemVnum, DWORD bCount, int iRarePct, bool bMsg)
{
	TItemTable* p = ITEM_MANAGER::instance().GetTable(dwItemVnum);

	if (!p)
		return NULL;

	DBManager::instance().SendMoneyLog(MONEY_LOG_DROP, dwItemVnum, bCount);

	if (p->dwFlags & ITEM_FLAG_STACKABLE && p->bType != ITEM_BLEND)
	{
		for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item = GetInventoryItem(i);

			if (!item)
				continue;

#ifdef ENABLE_SORT_INVEN
			if (item->GetOriginalVnum() == dwItemVnum && FN_check_item_socket(item))
#else
			if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
#endif
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}

				DWORD bCount2 = MIN(g_bItemCountLimit - item->GetCount(), bCount);
				bCount -= bCount2;

				item->SetCount(item->GetCount() + bCount2);

				if (bCount == 0)
				{
					if (bMsg)
					{
#ifdef __CHAT_SETTINGS__
						ChatPacket(CHAT_TYPE_ITEM_INFO, "304 %s", item->GetName());
#else
						ChatPacket(CHAT_TYPE_INFO, "305 %s", item->GetName());
#endif
					}
					if (item->GetType() == ITEM_QUEST || item->GetType() == ITEM_MATERIAL)
						quest::CQuestManager::instance().PickupItem(GetPlayerID(), item);
					return item;
				}
			}
		}
	}

	LPITEM item = ITEM_MANAGER::instance().CreateItem(dwItemVnum, bCount, 0, true);
	if (!item)
	{
		sys_err("cannot create item by vnum %u (name: %s)", dwItemVnum, GetName());
		return NULL;
	}

	if (item->GetType() == ITEM_BLEND)
	{
		for (int i = 0; i < INVENTORY_MAX_NUM; i++)
		{
			LPITEM inv_item = GetInventoryItem(i);

			if (inv_item == NULL) continue;

			if (inv_item->GetType() == ITEM_BLEND)
			{
				if (inv_item->GetVnum() == item->GetVnum())
				{
					if (inv_item->GetSocket(0) == item->GetSocket(0) &&
						inv_item->GetSocket(1) == item->GetSocket(1) &&
						inv_item->GetSocket(2) == item->GetSocket(2) &&
						inv_item->GetCount() < g_bItemCountLimit)
					{
						inv_item->SetCount(inv_item->GetCount() + item->GetCount());
						return inv_item;
					}
				}
			}
		}
	}
#ifdef ENABLE_SPECIAL_STORAGE
	else if (item->IsUpgradeItem() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
	{
		DWORD bCount = item->GetCount();

		for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item2 = GetUpgradeInventoryItem(i);

			if (!item2)
				continue;

			if (item2->GetVnum() == item->GetVnum())
			{

				DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
				bCount -= bCount2;

				item2->SetCount(item2->GetCount() + bCount2);

				if (bCount == 0)
				{
#ifdef __CHAT_SETTINGS__
					ChatPacket(CHAT_TYPE_ITEM_INFO, "304 %s", item2->GetName());
#else
					ChatPacket(CHAT_TYPE_INFO, "305 %s", item2->GetName());
#endif
					M2_DESTROY_ITEM(item);
					if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
						quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
					return item2;
				}
			}
		}

		item->SetCount(bCount);
	}
	else if (item->IsBook() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
	{
		DWORD bCount = item->GetCount();

		for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item2 = GetBookInventoryItem(i);

			if (!item2)
				continue;

			if (item2->GetVnum() == item->GetVnum())
			{

				int mithras;

				for (mithras = 0; mithras < ITEM_SOCKET_MAX_NUM; ++mithras)
					if (item2->GetSocket(mithras) != item->GetSocket(mithras))
						break;

				if (mithras != ITEM_SOCKET_MAX_NUM)
					continue;

				DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
				bCount -= bCount2;

				item2->SetCount(item2->GetCount() + bCount2);

				if (bCount == 0)
				{
#ifdef __CHAT_SETTINGS__
					ChatPacket(CHAT_TYPE_ITEM_INFO, "304 %s", item2->GetName());
#else
					ChatPacket(CHAT_TYPE_INFO, "305 %s", item2->GetName());
#endif
					M2_DESTROY_ITEM(item);
					if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
						quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
					return item2;
				}
			}
		}

		item->SetCount(bCount);
	}
	else if (item->IsStone() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
	{
		DWORD bCount = item->GetCount();

		for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item2 = GetStoneInventoryItem(i);

			if (!item2)
				continue;

			if (item2->GetVnum() == item->GetVnum())
			{

				DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
				bCount -= bCount2;

				item2->SetCount(item2->GetCount() + bCount2);

				if (bCount == 0)
				{
#ifdef __CHAT_SETTINGS__
					ChatPacket(CHAT_TYPE_ITEM_INFO, "304 %s", item2->GetName());
#else
					ChatPacket(CHAT_TYPE_INFO, "305 %s", item2->GetName());
#endif
					M2_DESTROY_ITEM(item);
					if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
						quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
					return item2;
				}
			}
		}
		item->SetCount(bCount);
	}
	else if (item->IsChange() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
	{
		DWORD bCount = item->GetCount();

		for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item2 = GetChangeInventoryItem(i);

			if (!item2)
				continue;

			if (item2->GetVnum() == item->GetVnum())
			{

				DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
				bCount -= bCount2;

				item2->SetCount(item2->GetCount() + bCount2);

				if (bCount == 0)
				{
#ifdef __CHAT_SETTINGS__
					ChatPacket(CHAT_TYPE_ITEM_INFO, "304 %s", item2->GetName());
#else
					ChatPacket(CHAT_TYPE_INFO, "305 %s", item2->GetName());
#endif
					M2_DESTROY_ITEM(item);
					if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
						quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
					return item2;
				}
			}
		}

		item->SetCount(bCount);
	}
	else if (item->IsCostume() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
	{
		DWORD bCount = item->GetCount();

		for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item2 = GetCostumeInventoryItem(i);
			if (!item2)
				continue;
			if (item2->GetVnum() == item->GetVnum())
			{

				DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
				bCount -= bCount2;

				item2->SetCount(item2->GetCount() + bCount2);

				if (bCount == 0)
				{
#ifdef __CHAT_SETTINGS__
					ChatPacket(CHAT_TYPE_ITEM_INFO, "304 %s", item2->GetName());
#else
					ChatPacket(CHAT_TYPE_INFO, "305 %s", item2->GetName());
#endif
					M2_DESTROY_ITEM(item);
					if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
						quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
					return item2;
				}
			}
		}
		item->SetCount(bCount);
	}

	else if (item->IsChest() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
	{
		DWORD bCount = item->GetCount();

		for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item2 = GetChestInventoryItem(i);
			if (!item2)
				continue;
			if (item2->GetVnum() == item->GetVnum())
			{

				DWORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
				bCount -= bCount2;

				item2->SetCount(item2->GetCount() + bCount2);

				if (bCount == 0)
				{
#ifdef __CHAT_SETTINGS__
					ChatPacket(CHAT_TYPE_ITEM_INFO, "304 %s", item2->GetName());
#else
					ChatPacket(CHAT_TYPE_INFO, "305 %s", item2->GetName());
#endif
					M2_DESTROY_ITEM(item);
					if (item2->GetType() == ITEM_QUEST || item2->GetType() == ITEM_MATERIAL)
						quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
					return item2;
				}
			}
		}
		item->SetCount(bCount);
	}

#endif
#ifdef USE_BOOST_POTIONS
	else if (item->IsTimeStackable() && item->IsBoostPotion() && item->IsTimerBasedOnWear() && item->GetCount() == 1)
	{
		LPITEM item2 = nullptr;
		for (uint16_t i = 0; i < INVENTORY_MAX_NUM; i++)
		{
			item2 = GetInventoryItem(i);
			if (!item2)
			{
				continue;
			}

			if (item->GetVnum() == item2->GetVnum())
			{
				if (item2->GetSocket(ITEM_BOOST_POTION_STATUS != 0))
				{
					continue;
				}
				else if (item2->isLocked())
				{
					continue;
				}
				else if (item2->IsExchanging())
				{
					continue;
				}
				else if (item2->IsEquipped())
				{
					continue;
				}

				const long lSrcDuration = item->GetSocket(ITEM_SOCKET_REMAIN_SEC);
				const long lTrgDuration = item2->GetSocket(ITEM_SOCKET_REMAIN_SEC);

				const long lDuration = lSrcDuration + lTrgDuration;
				if (lDuration > 0 && lDuration <= ITEM_BOOST_POTION_MAX_STACK_TIME)
				{
					M2_DESTROY_ITEM(item);

					item2->SetSocket(ITEM_SOCKET_REMAIN_SEC, lDuration);
					return item2;
				}
			}
		}
	}
#endif

	int iEmptyCell;
	if (item->IsDragonSoul())
	{
		iEmptyCell = GetEmptyDragonSoulInventory(item);
	}
#ifdef ENABLE_SPECIAL_STORAGE
	else if (item->IsUpgradeItem())
	{
		iEmptyCell = GetEmptyUpgradeInventory(item);
	}
	else if (item->IsBook())
	{
		iEmptyCell = GetEmptyBookInventory(item);
	}
	else if (item->IsStone())
	{
		iEmptyCell = GetEmptyStoneInventory(item);
	}
	else if (item->IsChange())
	{
		iEmptyCell = GetEmptyChangeInventory(item);
	}
	else if (item->IsCostume())
	{
		iEmptyCell = GetEmptyCostumeInventory(item);
	}
	else if (item->IsChest())
	{
		iEmptyCell = GetEmptyChestInventory(item);
	}
#endif
	else
		iEmptyCell = GetEmptyInventory(item->GetSize());

	if (iEmptyCell != -1)
	{
		if (bMsg)
		{
#ifdef __CHAT_SETTINGS__
			ChatPacket(CHAT_TYPE_ITEM_INFO, "304 %s", item->GetName());
#else
			ChatPacket(CHAT_TYPE_INFO, "305 %s", item->GetName());
#endif
		}

		if (item->IsDragonSoul())
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
#ifdef ENABLE_SPECIAL_STORAGE
		else if (item->IsUpgradeItem())
			item->AddToCharacter(this, TItemPos(UPGRADE_INVENTORY, iEmptyCell));
		else if (item->IsBook())
			item->AddToCharacter(this, TItemPos(BOOK_INVENTORY, iEmptyCell));
		else if (item->IsStone())
			item->AddToCharacter(this, TItemPos(STONE_INVENTORY, iEmptyCell));
		else if (item->IsChange())
			item->AddToCharacter(this, TItemPos(CHANGE_INVENTORY, iEmptyCell));
		else if (item->IsCostume())
			item->AddToCharacter(this, TItemPos(COSTUME_INVENTORY, iEmptyCell));
		else if (item->IsChest())
			item->AddToCharacter(this, TItemPos(CHEST_INVENTORY, iEmptyCell));
#endif
		else
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
		LogManager::instance().ItemLog(this, item, "SYSTEM", item->GetName());

		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION)
		{
			TQuickslot* pSlot;

			if (GetQuickslot(0, &pSlot) && pSlot->type == QUICKSLOT_TYPE_NONE)
			{
				TQuickslot slot;
				slot.type = QUICKSLOT_TYPE_ITEM;
				slot.pos = iEmptyCell;
				SetQuickslot(0, slot);
			}
		}
	}
	else
	{
		item->AddToGround(GetMapIndex(), GetXYZ());
#ifdef ENABLE_NEWSTUFF
		item->StartDestroyEvent(g_aiItemDestroyTime[ITEM_DESTROY_TIME_AUTOGIVE]);
#else
		item->StartDestroyEvent();
#endif
		// Â¾??Â¼ ÂµÃ¥Â¶Ã¸ flagÂ°Â¡ Â°?Â·???Â´? Â¾??????? Â°Ã¦Â¿Ã¬,
		// ??ÂºÂ¥Â¿Â¡ ÂºÃ³ Â°Ã¸Â°Â£?? Â¾Ã¸Â¾Ã®Â¼Â­ Â¾Ã®?Â¿ Â¼Ã¶ Â¾Ã¸?? Â¶Â³Â¾Ã®?Â®Â¸Â®Â°? Âµ?Â¸Ã©,
		// ownership?Â» Â¾??????? Â»Ã§Â¶Ã³?Ãº Â¶Â§Â±Ã®?Ã¶(300??) ?Â¯?Ã¶??Â´?.
		if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_DROP))
			item->SetOwnership(this, 300);
		else
			item->SetOwnership(this, 60);
		LogManager::instance().ItemLog(this, item, "SYSTEM_DROP", item->GetName());
	}

	sys_log(0,
		"7: %d %d", dwItemVnum, bCount);
	return item;
}

bool CHARACTER::GiveItem(LPCHARACTER victim, TItemPos Cell)
{
	if (!CanHandleItem())
		return false;

	// @fixme150 BEGIN
	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, "314");
		return false;
	}
	// @fixme150 END

	LPITEM item = GetItem(Cell);

	if (item && !item->IsExchanging())
	{
		if (victim->CanReceiveItem(this, item))
		{
			victim->ReceiveItem(this, item);
			return true;
		}
	}

	return false;
}

bool CHARACTER::CanReceiveItem(LPCHARACTER from, LPITEM item) const
{
	if (IsPC())
		return false;

	// TOO_LONG_DISTANCE_EXCHANGE_BUG_FIX
	if (DISTANCE_APPROX(GetX() - from->GetX(), GetY() - from->GetY()) > 2000)
		return false;
	// END_OF_TOO_LONG_DISTANCE_EXCHANGE_BUG_FIX
#ifdef ENABLE_SOULBIND_SYSTEM
	if (item->IsSealed())
	{
		from->ChatPacket(CHAT_TYPE_INFO, "315");
		return false;
	}
#endif

	switch (GetRaceNum())
	{
		case fishing::CAMPFIRE_MOB:
		{
			if (item->GetType() == ITEM_FISH &&
				 (item->GetSubType() == FISH_ALIVE || item->GetSubType() == FISH_DEAD))
			{
				return true;
			}

			break;
		}
		case fishing::FISHER_MOB:
		{
			if (item->GetType() == ITEM_ROD)
			{
				return true;
			}

			break;
		}
		case BLACKSMITH_WEAPON_MOB:
		// case DEVILTOWER_BLACKSMITH_WEAPON_MOB:
		{
			if (item->GetType() == ITEM_WEAPON &&
				 item->GetRefinedVnum())
			{
				return true;
			}
			else
			{
				return false;
			}

			break;
		}
		case BLACKSMITH_ARMOR_MOB:
		// case DEVILTOWER_BLACKSMITH_ARMOR_MOB:
		{
			if (item->GetType() == ITEM_ARMOR &&
				 (item->GetSubType() == ARMOR_BODY || item->GetSubType() == ARMOR_SHIELD || item->GetSubType() == ARMOR_HEAD) && item->GetRefinedVnum())
			{
				return true;
			}
			else
			{
				return false;
			}

			break;
		}
		case BLACKSMITH_ACCESSORY_MOB:
		// case DEVILTOWER_BLACKSMITH_ACCESSORY_MOB:
		{
			if (item->GetType() == ITEM_ARMOR &&
				!(item->GetSubType() == ARMOR_BODY || item->GetSubType() == ARMOR_SHIELD || item->GetSubType() == ARMOR_HEAD) &&
				item->GetRefinedVnum())
			{
				return true;
			}
			else
			{
				return false;
			}

			break;
		}
		case BLACKSMITH_MOB:
		{
			// if (item->GetRefinedVnum() && item->GetRefineSet() < 500)
			if (item->GetRefinedVnum())
			{
#ifdef USE_CRYSTAL_SYSTEM
				if (item->GetType() == ITEM_CRYSTAL)
				{
					return false;
				}
#endif

				return true;
			}
			else
			{
				return false;
			}
		}
		case BLACKSMITH2_MOB:
		{
			if (item->GetRefineSet() >= 500)
			{
#ifdef USE_CRYSTAL_SYSTEM
				if (item->GetType() == ITEM_CRYSTAL)
				{
					return false;
				}
#endif

				return true;
			}
			else
			{
				return false;
			}
		}
		case ALCHEMIST_MOB:
		{
			if (item->GetRefinedVnum())
			{
#ifdef USE_CRYSTAL_SYSTEM
				if (item->GetType() == ITEM_CRYSTAL)
				{
					return false;
				}
#endif

				return true;
			}

			break;
		}
		case 20101:
		case 20102:
		case 20103:
		{
			if (item->GetVnum() == ITEM_REVIVE_HORSE_1)
			{
				if (!IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, "316");
					return false;
				}

				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_1)
			{
				if (IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, "317");
					return false;
				}

				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_2 || item->GetVnum() == ITEM_HORSE_FOOD_3)
			{
				return false;
			}

			break;
		}
		case 20104:
		case 20105:
		case 20106:
		{
			if (item->GetVnum() == ITEM_REVIVE_HORSE_2)
			{
				if (!IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, "316");
					return false;
				}

				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_2)
			{
				if (IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, "317");
					return false;
				}

				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_1 || item->GetVnum() == ITEM_HORSE_FOOD_3)
			{
				return false;
			}

			break;
		}
		case 20107:
		case 20108:
		case 20109:
		{
			if (item->GetVnum() == ITEM_REVIVE_HORSE_3)
			{
				if (!IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, "316");
					return false;
				}

				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_3)
			{
				if (IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, "317");
					return false;
				}

				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_1 || item->GetVnum() == ITEM_HORSE_FOOD_2)
			{
				return false;
			}

			break;
		}
		default:
		{
			break;
		}
	}

	//if (IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_GIVE))
	{
		return true;
	}

	return false;
}

void CHARACTER::ReceiveItem(LPCHARACTER from, LPITEM item)
{
	if (IsPC())
		return;

	switch (GetRaceNum())
	{
	case fishing::CAMPFIRE_MOB:
		if (item->GetType() == ITEM_FISH && (item->GetSubType() == FISH_ALIVE || item->GetSubType() == FISH_DEAD))
			fishing::Grill(from, item);
		else
		{
			// TAKE_ITEM_BUG_FIX
			from->SetQuestNPCID(GetVID());
			// END_OF_TAKE_ITEM_BUG_FIX
			quest::CQuestManager::instance().TakeItem(from->GetPlayerID(), GetRaceNum(), item);
		}
		break;

#ifdef ENABLE_QUEEN_NETHIS
	case SnakeLair::PILAR_STEP_4:
	{
		if (from->IsPC())
		{
			if (SnakeLair::CSnk::instance().IsSnakeMap(from->GetMapIndex()))
				if (item || from || this)
					SnakeLair::CSnk::instance().OnKillPilar(item, from, this);
		}
	}
	break;

	case SnakeLair::BLACKSMITH_5:
	{
		if (from->IsPC())
		{
			if (SnakeLair::CSnk::instance().IsSnakeMap(from->GetMapIndex()))
				if (item || from || this)
					SnakeLair::CSnk::instance().OnKillBlackSmith(item, from, this);
		}
	}
	break;

	case SnakeLair::SNAKE_STATUE1:
	case SnakeLair::SNAKE_STATUE2:
	case SnakeLair::SNAKE_STATUE3:
	case SnakeLair::SNAKE_STATUE4:
	{
		if (from->IsPC())
		{
			if (SnakeLair::CSnk::instance().IsSnakeMap(from->GetMapIndex()))
				if (item || from || this)
					SnakeLair::CSnk::instance().OnStatueSetRotation(item, from, this);
		}
	}
	break;

	case SnakeLair::VNUM_BLACKSMITH_SERPENT:
	{
#ifdef ENABLE_YOHARA_SYSTEM
		if (item->GetType() == ITEM_ARMOR && item->GetSubType() == ARMOR_GLOVE)
		{
			from->SetRefineNPC(this);
			from->RefineInformation(item->GetCell(), REFINE_TYPE_MONEY_ONLY);
		}
		else
#endif
		{
			from->ChatPacket(CHAT_TYPE_INFO, "[LS;1002]");
		}
	}
	break;
#endif

	case BLACKSMITH_MOB:
	case BLACKSMITH2_MOB:
	case BLACKSMITH_WEAPON_MOB:
	case BLACKSMITH_ARMOR_MOB:
	case BLACKSMITH_ACCESSORY_MOB:
		if (item->GetRefinedVnum())
		{
			from->SetRefineNPC(this);

			//if (item->IsUpgradeItem() || ){
			//	from->RefineInformation(item->GetCell(), REFINE_TYPE_NORMAL, -1,UPGRADE_INVENTORY);
			//}
			//else if (item->IsBook()){
			//	from->RefineInformation(item->GetCell(), REFINE_TYPE_NORMAL, -1,BOOK_INVENTORY);
			//}
			if (item->IsStone() && item->GetWindow() == STONE_INVENTORY)
			{
				from->ChatPacket(CHAT_TYPE_INFO, "2508");
				return;
			}
			else
			{
				from->RefineInformation(item->GetCell(), REFINE_TYPE_NORMAL, -1, INVENTORY);
			}


		}
		else
		{
			from->ChatPacket(CHAT_TYPE_INFO, "117");
		}
		break;

	case 20101:
	case 20102:
	case 20103:
	case 20104:
	case 20105:
	case 20106:
	case 20107:
	case 20108:
	case 20109:
		if (item->GetVnum() == ITEM_REVIVE_HORSE_1 ||
			item->GetVnum() == ITEM_REVIVE_HORSE_2 ||
			item->GetVnum() == ITEM_REVIVE_HORSE_3)
		{
			from->ReviveHorse();
			item->SetCount(item->GetCount() - 1);
			from->ChatPacket(CHAT_TYPE_INFO, "318");
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_1 ||
			item->GetVnum() == ITEM_HORSE_FOOD_2 ||
			item->GetVnum() == ITEM_HORSE_FOOD_3)
		{
			from->FeedHorse();
			from->ChatPacket(CHAT_TYPE_INFO, "319");
			item->SetCount(item->GetCount() - 1);
			EffectPacket(SE_HPUP_RED);
		}
		break;

	default:
		sys_log(0, "TakeItem %s %d %s", from->GetName(), GetRaceNum(), item->GetName());
		from->SetQuestNPCID(GetVID());
		quest::CQuestManager::instance().TakeItem(from->GetPlayerID(), GetRaceNum(), item);
		break;
	}
}

bool CHARACTER::IsEquipUniqueItem(DWORD dwItemVnum) const
{
	{
		LPITEM u = GetWear(WEAR_UNIQUE1);

		if (u && u->GetVnum() == dwItemVnum)
			return true;
	}

	{
		LPITEM u = GetWear(WEAR_UNIQUE2);

		if (u && u->GetVnum() == dwItemVnum)
			return true;
	}

	// Â¾Ã°Â¾Ã®Â¹??Ã¶?? Â°Ã¦Â¿Ã¬ Â¾Ã°Â¾Ã®Â¹??Ã¶(Â°?ÂºÂ») ???Ã¶ÂµÂµ ?Â¼?Â©??Â´?.
	if (dwItemVnum == UNIQUE_ITEM_RING_OF_LANGUAGE)
		return IsEquipUniqueItem(UNIQUE_ITEM_RING_OF_LANGUAGE_SAMPLE);

	return false;
}

// CHECK_UNIQUE_GROUP
bool CHARACTER::IsEquipUniqueGroup(DWORD dwGroupVnum) const
{
	{
		LPITEM u = GetWear(WEAR_UNIQUE1);

		if (u && u->GetSpecialGroup() == (int)dwGroupVnum)
			return true;
	}

	{
		LPITEM u = GetWear(WEAR_UNIQUE2);

		if (u && u->GetSpecialGroup() == (int)dwGroupVnum)
			return true;
	}

	return false;
}
// END_OF_CHECK_UNIQUE_GROUP

void CHARACTER::SetRefineMode(int iAdditionalCell)
{
	m_iRefineAdditionalCell = iAdditionalCell;
	m_bUnderRefine = true;
}

void CHARACTER::ClearRefineMode()
{
	m_bUnderRefine = false;
	SetRefineNPC(NULL);
}

bool CHARACTER::GiveItemFromSpecialItemGroup(DWORD dwGroupNum, std::vector<DWORD>& dwItemVnums,
	std::vector<DWORD>& dwItemCounts, std::vector <LPITEM>& item_gets, int& count)
{
	const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(dwGroupNum);

	if (!pGroup)
	{
		sys_err("cannot find special item group %d", dwGroupNum);
		return false;
	}

	std::vector <int> idxes;
	int n = pGroup->GetMultiIndex(idxes);

	bool bSuccess = true;
	int pos_check = 0;



	for (int i = 0; i < n; i++)
	{

		int idx = idxes[i];
		DWORD dwVnum = pGroup->GetVnum(idx);
		DWORD dwCount = pGroup->GetCount(idx);
		if (dwVnum)
		{
			LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(dwVnum, dwCount);
			if (pkNewItem)
			{
				int pos = -1;

#ifdef ENABLE_SPECIAL_STORAGE
				if (pkNewItem->IsUpgradeItem())
				{
					pos = GetEmptyUpgradeInventory(pkNewItem);
				}
				else if (pkNewItem->IsBook())
				{
					pos = GetEmptyBookInventory(pkNewItem);
				}
				else if (pkNewItem->IsStone())
				{
					pos = GetEmptyStoneInventory(pkNewItem);
				}
				else if (pkNewItem->IsChange())
				{
					pos = GetEmptyChangeInventory(pkNewItem);
				}
				else if (pkNewItem->IsCostume())
				{
					pos = GetEmptyCostumeInventory(pkNewItem);
				}
				else if (pkNewItem->IsChest())
				{
					pos = GetEmptyChestInventory(pkNewItem);
				}
				else
				{
					pos = GetEmptyInventory(pkNewItem->GetSize());
				}
#endif
				if (pos != -1)
				{
					pos_check += 1;
				}
			}
		}
	}

	/*
	if (CountEmptyInventory() < pos_check || pos_check <= 0)
	{
		ChatPacket(CHAT_TYPE_INFO, "No tiene suficiente espacio en su inventario");
		return false;
	}
*/

	for (int i = 0; i < n; i++)
	{
		bSuccess = false;
		int idx = idxes[i];
		DWORD dwVnum = pGroup->GetVnum(idx);
		DWORD dwCount = pGroup->GetCount(idx);
		int	iRarePct = pGroup->GetRarePct(idx);
		LPITEM item_get = NULL;


		switch (dwVnum)
		{
		case CSpecialItemGroup::GOLD:
			PointChange(POINT_GOLD, dwCount);
			LogManager::instance().CharLog(this, dwCount, "TREASURE_GOLD", "");

			bSuccess = true;
			break;
		case CSpecialItemGroup::EXP:
		{
			PointChange(POINT_EXP, dwCount);
			LogManager::instance().CharLog(this, dwCount, "TREASURE_EXP", "");

			bSuccess = true;
		}
		break;

		case CSpecialItemGroup::MOB:
		{
			sys_log(0, "CSpecialItemGroup::MOB %d", dwCount);
			int x = GetX() + number(-500, 500);
			int y = GetY() + number(-500, 500);

			LPCHARACTER ch = CHARACTER_MANAGER::instance().SpawnMob(dwCount, GetMapIndex(), x, y, 0, true, -1);
			if (ch)
				ch->SetAggressive();
			bSuccess = true;
		}
		break;
		case CSpecialItemGroup::SLOW:
		{
			sys_log(0, "CSpecialItemGroup::SLOW %d", -(int)dwCount);
			AddAffect(AFFECT_SLOW, POINT_MOV_SPEED, -(int)dwCount, AFF_SLOW, 300, 0, true);
			bSuccess = true;
		}
		break;
		case CSpecialItemGroup::DRAIN_HP:
		{
			int iDropHP = GetMaxHP() * dwCount / 100;
			sys_log(0, "CSpecialItemGroup::DRAIN_HP %d", -iDropHP);
			iDropHP = MIN(iDropHP, GetHP() - 1);
			sys_log(0, "CSpecialItemGroup::DRAIN_HP %d", -iDropHP);
			PointChange(POINT_HP, -iDropHP);
			bSuccess = true;
		}
		break;
		case CSpecialItemGroup::POISON:
		{
			AttackedByPoison(NULL);
			bSuccess = true;
		}
		break;
#ifdef ENABLE_WOLFMAN_CHARACTER
		case CSpecialItemGroup::BLEEDING:
		{
			AttackedByBleeding(NULL);
			bSuccess = true;
		}
		break;
#endif
		case CSpecialItemGroup::MOB_GROUP:
		{
			int sx = GetX() - number(300, 500);
			int sy = GetY() - number(300, 500);
			int ex = GetX() + number(300, 500);
			int ey = GetY() + number(300, 500);
			CHARACTER_MANAGER::instance().SpawnGroup(dwCount, GetMapIndex(), sx, sy, ex, ey, NULL, true);

			bSuccess = true;
		}
		break;
		default:
		{

			item_get = AutoGiveItem(dwVnum, dwCount, iRarePct);

			if (item_get)
			{
				bSuccess = true;
			}
		}
		break;
		}

		if (bSuccess)
		{
			dwItemVnums.push_back(dwVnum);
			dwItemCounts.push_back(dwCount);
			item_gets.push_back(item_get);
			count++;

		}
		else
		{
			return false;
		}
	}
	return bSuccess;
}

// NEW_HAIR_STYLE_ADD
bool CHARACTER::ItemProcess_Hair(LPITEM item, int iDestCell)
{
	if (item->CheckItemUseLevel(GetLevel()
#ifdef ENABLE_YOHARA_SYSTEM
, GetConquerorLevel()
#endif
	) == false)
	{
		// Â·Â¹ÂºÂ§ ?Â¦??Â¿Â¡ Â°?Â¸Â²
		ChatPacket(CHAT_TYPE_INFO, "320");
		return false;
	}

	DWORD hair = item->GetVnum();

	switch (GetJob())
	{
	case JOB_WARRIOR:
		hair -= 72000; // 73001 - 72000 = 1001 Âº??? ?Ã¬Â¾Ã® Â¹Ã¸?Â£ Â½???
		break;

	case JOB_ASSASSIN:
		hair -= 71250;
		break;

	case JOB_SURA:
		hair -= 70500;
		break;

	case JOB_SHAMAN:
		hair -= 69750;
		break;
#ifdef ENABLE_WOLFMAN_CHARACTER
	case JOB_WOLFMAN:
		break; // NOTE: ?? ?Ã¬Â¾Ã®??ÂµÃ¥Â´? Â¾? Â¾Â²??Â¹?Â·? ??Â½Âº. (?Ã¶?Ã§ ?Ã¬Â¾Ã®Â½?Â½Âº???Âº ??Â¹? ??Â½Âº?Â¬?Â¸Â·? Â´Ã«?Â¼ Âµ? Â»Ã³????)
#endif
	default:
		return false;
		break;
	}

	if (hair == GetPart(PART_HAIR))
	{
		ChatPacket(CHAT_TYPE_INFO, "321");
		return true;
	}

	item->SetCount(item->GetCount() - 1);

	SetPart(PART_HAIR, hair);
	UpdatePacket();

	return true;
}
// END_NEW_HAIR_STYLE_ADD

bool CHARACTER::ItemProcess_Polymorph(LPITEM item)
{
#if defined(WJ_COMBAT_ZONE)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, "322");
		return false;
	}
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(this, TOURNAMENT_BLOCK_POLY))
		return false;
#endif
	if (IsPolymorphed())
	{
		ChatPacket(CHAT_TYPE_INFO, "323");
		return false;
	}

	if (true == IsRiding())
	{
		ChatPacket(CHAT_TYPE_INFO, "324");
		return false;
	}

	DWORD dwVnum = item->GetSocket(0);

	if (dwVnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, "325");
		item->SetCount(item->GetCount() - 1);
		return false;
	}

	const CMob* pMob = CMobManager::instance().Get(dwVnum);

	if (pMob == NULL)
	{
		ChatPacket(CHAT_TYPE_INFO, "325");
		item->SetCount(item->GetCount() - 1);
		return false;
	}

	switch (item->GetVnum())
	{
	case 70104:
	case 70105:
	case 70106:
	case 70107:
	case 71093:
	{
		// Âµ?Â°Â©Â±Â¸ ?Â³Â¸Â®
		sys_log(0, "USE_POLYMORPH_BALL PID(%d) vnum(%d)", GetPlayerID(), dwVnum);

		// Â·Â¹ÂºÂ§ ?Â¦?? ?Â¼?Â©
		int iPolymorphLevelLimit = MAX(0, 20 - GetLevel() * 3 / 10);
		if (pMob->m_table.bLevel >= GetLevel() + iPolymorphLevelLimit)
		{
			ChatPacket(CHAT_TYPE_INFO, "326");
			return false;
		}

		int iDuration = GetSkillLevel(POLYMORPH_SKILL_ID) == 0 ? 5 : (5 + (5 + GetSkillLevel(POLYMORPH_SKILL_ID) / 40 * 25));
		iDuration *= 60;

		DWORD dwBonus = 0;

		dwBonus = (2 + GetSkillLevel(POLYMORPH_SKILL_ID) / 40) * 100;

		AddAffect(AFFECT_POLYMORPH, POINT_POLYMORPH, dwVnum, AFF_POLYMORPH, iDuration, 0, true);
		AddAffect(AFFECT_POLYMORPH, POINT_ATT_BONUS, dwBonus, AFF_POLYMORPH, iDuration, 0, false);
		item->SetCount(item->GetCount() - 1);
	}
	break;

	case 50322:
	{
		// ÂºÂ¸Â·Ã¹

		// Âµ?Â°Â©Â¼Â­ ?Â³Â¸Â®
		// Â¼???0				Â¼???1		   Â¼???2
		// Âµ?Â°Â©?? Â¸Ã³Â½Âº?? Â¹Ã¸?Â£   Â¼Ã¶Â·??Â¤ÂµÂµ		Âµ?Â°Â©Â¼Â­ Â·Â¹ÂºÂ§
		sys_log(0, "USE_POLYMORPH_BOOK: %s(%u) vnum(%u)", GetName(), GetPlayerID(), dwVnum);

		if (CPolymorphUtils::instance().PolymorphCharacter(this, item, pMob) == true)
		{
			CPolymorphUtils::instance().UpdateBookPracticeGrade(this, item);
		}
		else
		{
		}
	}
	break;

	default:
		sys_err("POLYMORPH invalid item passed PID(%d) vnum(%d)", GetPlayerID(), item->GetOriginalVnum());
		return false;
	}

	return true;
}

bool CHARACTER::CanDoCube() const
{
	if (m_bIsObserver)	return false;
	if (GetShop())		return false;
	if (m_bUnderRefine)	return false;
	if (IsWarping())	return false;
#ifdef __AURA_SYSTEM__
	if (IsAuraRefineWindowOpen())	return false;
#endif
	return true;
}

bool CHARACTER::UnEquipSpecialRideUniqueItem()
{
	/*
	LPITEM Unique1 = GetWear(WEAR_UNIQUE1);
	LPITEM Unique2 = GetWear(WEAR_UNIQUE2);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	LPITEM MountCostume = GetWear(WEAR_COSTUME_MOUNT);
#endif

	if( NULL != Unique1 )
	{
		if( UNIQUE_GROUP_SPECIAL_RIDE == Unique1->GetSpecialGroup() )
		{
			return UnequipItem(Unique1);
		}
	}

	if( NULL != Unique2 )
	{
		if( UNIQUE_GROUP_SPECIAL_RIDE == Unique2->GetSpecialGroup() )
		{
			return UnequipItem(Unique2);
		}
	}
	*/

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	LPITEM MountCostume = GetWear(WEAR_COSTUME_MOUNT);
	if (MountCostume)
		return UnequipItem(MountCostume);
#endif

	return true;
}

void CHARACTER::AutoRecoveryItemProcess(const EAffectTypes type)
{
	if (true == IsDead() || true == IsStun())
		return;

	if (false == IsPC())
		return;

	if (AFFECT_AUTO_HP_RECOVERY != type && AFFECT_AUTO_SP_RECOVERY != type)
		return;

#ifdef __DOJANG_SRC_FUNCTIONS__
	if (type == AFFECT_AUTO_HP_RECOVERY && GetMapIndex() == DOJANG_MAPINDEX)
		return;
#endif
#ifdef ENABLE_SUNG_MAHI_TOWER
	bool isDungeonMap = (GetMapIndex() >= 3540000 && GetMapIndex() < 3550000);
	if (isDungeonMap)
		return;
#endif

	if (NULL != FindAffect(AFFECT_STUN))
		return;

	{
		const DWORD stunSkills[] = { SKILL_TANHWAN, SKILL_GEOMPUNG, SKILL_BYEURAK, SKILL_GIGUNG };

		for (size_t i = 0; i < sizeof(stunSkills) / sizeof(DWORD); ++i)
		{
			const CAffect* p = FindAffect(stunSkills[i]);

			if (NULL != p && AFF_STUN == p->dwFlag)
				return;
		}
	}

	const CAffect* pAffect = FindAffect(type);
	const size_t idx_of_amount_of_used = 1;
	const size_t idx_of_amount_of_full = 2;

	if (NULL != pAffect)
	{
		LPITEM pItem = FindItemByID(pAffect->dwFlag);

		if (NULL != pItem && true == pItem->GetSocket(0))
		{
			if (!CArenaManager::instance().IsArenaMap(GetMapIndex())
#ifdef ENABLE_NEWSTUFF
				&& !(g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(GetPlayerID()) && !IsAllowedPotionOnPVP(pItem->GetVnum()))
#endif
				)
			{
				const long amount_of_used = pItem->GetSocket(idx_of_amount_of_used);
				const long amount_of_full = pItem->GetSocket(idx_of_amount_of_full);

				const int32_t avail = amount_of_full - amount_of_used;

				int32_t amount = 0;

				if (AFFECT_AUTO_HP_RECOVERY == type)
				{
					amount = GetMaxHP() - (GetHP() + GetPoint(POINT_HP_RECOVERY));
				}
				else if (AFFECT_AUTO_SP_RECOVERY == type)
				{
					amount = GetMaxSP() - (GetSP() + GetPoint(POINT_SP_RECOVERY));
				}

				if (amount > 0)
				{
					if (avail > amount)
					{
						const int pct_of_used = amount_of_used * 100 / amount_of_full;
						const int pct_of_will_used = (amount_of_used + amount) * 100 / amount_of_full;

						bool bLog = false;
						if ((pct_of_will_used / 10) - (pct_of_used / 10) >= 1)
						{
							bLog = true;
						}

						pItem->SetSocket(idx_of_amount_of_used, amount_of_used + amount, bLog);
					}
					else
					{
						amount = avail;

						ITEM_MANAGER::instance().RemoveItem(pItem);
					}

					if (AFFECT_AUTO_HP_RECOVERY == type)
					{
						PointChange(POINT_HP_RECOVERY, amount);
						EffectPacket(SE_AUTO_HPUP);
					}
					else if (AFFECT_AUTO_SP_RECOVERY == type)
					{
						PointChange(POINT_SP_RECOVERY, amount);
						EffectPacket(SE_AUTO_SPUP);
					}
				}
			}
			else
			{
				pItem->Lock(false);
				pItem->SetSocket(0, false);
				RemoveAffect(const_cast<CAffect*>(pAffect));
			}
		}
		else
		{
			RemoveAffect(const_cast<CAffect*>(pAffect));
		}
	}
}

bool CHARACTER::IsValidItemPosition(TItemPos Pos) const
{
	BYTE window_type = Pos.window_type;
	WORD cell = Pos.cell;

	switch (window_type)
	{
	case RESERVED_WINDOW:
		return false;

	case INVENTORY:
	case EQUIPMENT:
		return cell < (INVENTORY_AND_EQUIP_SLOT_MAX);

	case DRAGON_SOUL_INVENTORY:
		return cell < (DRAGON_SOUL_INVENTORY_MAX_NUM);

#ifdef ENABLE_SWITCHBOT
	case SWITCHBOT:
		return cell < SWITCHBOT_SLOT_COUNT;
#endif

#ifdef ENABLE_SPECIAL_STORAGE
	case UPGRADE_INVENTORY:
	case BOOK_INVENTORY:
	case STONE_INVENTORY:
	case CHANGE_INVENTORY:
	case COSTUME_INVENTORY:
	case CHEST_INVENTORY:
		return cell < (SPECIAL_INVENTORY_MAX_NUM);
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
	case SWITCHBOT:
		return cell < SWITCHBOT_SLOT_MAX;
#endif

#ifdef CHANGE_EQUIP_WORLDARD
	case CHANGE_EQUIP:
		return cell < CHANGE_EQUIP_SLOT_COUNT;
#endif

	case SAFEBOX:
		if (NULL != m_pkSafebox)
			return m_pkSafebox->IsValidPosition(cell);
		else
			return false;

	case MALL:
		if (NULL != m_pkMall)
			return m_pkMall->IsValidPosition(cell);
		else
			return false;
	default:
		return false;
	}
}


// Â±??ÃºÂ¾?Â¼Â­ Â¸Â¸ÂµÃ§ Â¸??Â©Â·?.. expÂ°Â¡ trueÂ¸Ã© msgÂ¸Â¦ ?Ã¢Â·???Â°Ã­ return false ??Â´? Â¸??Â©Â·? (??Â¹??Ã»?? verify Â¿Ã«ÂµÂµÂ¶Ã»?Âº return Â¶Â§Â¹Â®Â¿Â¡ Â¾?Â°Â£ Â¹?Â´Ã«Â¶Ã³ ??Â¸Â§Â¶Â§Â¹Â®Â¿Â¡ ?Ã²Â°Â¥Â¸Â± Â¼Ã¶ÂµÂµ ??Â°?Â´?..)
#define VERIFY_MSG(exp, msg)  \
	if (true == (exp)) { \
			ChatPacket(CHAT_TYPE_INFO, (msg)); \
			return false; \
	}

/// ?Ã¶?Ã§ ?Â³Â¸Â¯???? Â»Ã³??Â¸Â¦ Â¹????Â¸Â·? ??Â¾Ã®?Ã¸ item?Â» ?Ã¸Â¿Ã«?? Â¼Ã¶ ??Â´? ?Ã¶ ?Â®????Â°Ã­, Âº?Â°Â¡Â´? ??Â´?Â¸Ã© ?Â³Â¸Â¯??Â¿Â¡Â°? ???Â¯Â¸Â¦ Â¾?Â·???Â´? ??Â¼Ã¶
bool CHARACTER::CanEquipNow(const LPITEM item, const TItemPos& srcCell, const TItemPos& destCell) /*const*/
{
#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	if (item->isLocked() && CSwitchbotManager::Instance().IsItemIDInSwitchbot(GetPlayerID(), item->GetID()))
	{
		ChatPacket(CHAT_TYPE_INFO, "Remove item from switchbot first!");
		return false;
	}

	if (item->isLocked())
		return false;
#endif

	const TItemTable* itemTable = item->GetProto();
	//BYTE itemType = item->GetType();
	//BYTE itemSubType = item->GetSubType();

#ifdef __BUFFI_SUPPORT__
	if (IsLookingBuffiPage() && item->GetType() == ITEM_COSTUME && item->GetSubType() == COSTUME_WEAPON)
	{
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
			return false;
	}
	else
#endif
	{
		switch (GetJob())
		{
		case JOB_WARRIOR:
			if (item->GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR)
				return false;
			break;

		case JOB_ASSASSIN:
			if (item->GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN)
				return false;
			break;

		case JOB_SHAMAN:
			if (item->GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
				return false;
			break;

		case JOB_SURA:
			if (item->GetAntiFlag() & ITEM_ANTIFLAG_SURA)
				return false;
			break;

#ifdef ENABLE_WOLFMAN
		case JOB_WOLFMAN:
			if (item->GetAntiFlag() & ITEM_ANTIFLAG_WOLFMAN)
			{
				return false;
			}
			break;
#endif
		}
	}

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		long limit = itemTable->aLimits[i].lValue;
		switch (itemTable->aLimits[i].bType)
		{
			case LIMIT_LEVEL:
			{
				if (GetLevel() < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, "327");
					return false;
				}

				break;
			}
			case LIMIT_STR:
			{
				if (GetPoint(POINT_ST) < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, "328");
					return false;
				}

				break;
			}
			case LIMIT_INT:
			{
				if (GetPoint(POINT_IQ) < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, "329");
					return false;
				}

				break;
			}
			case LIMIT_DEX:
			{
				if (GetPoint(POINT_DX) < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, "330");
					return false;
				}

				break;
			}
			case LIMIT_CON:
			{
				if (GetPoint(POINT_HT) < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, "331");
					return false;
				}

				break;
			}
#ifdef ENABLE_YOHARA_SYSTEM
			case LIMIT_CONQUEROR_LEVEL:
			{
				if (GetConquerorLevel() < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, "327");
					return false;
				}

				break;
			}
#endif
			default:
			{
				break;
			}
		}
	}

	if (item->GetWearFlag() & WEARABLE_UNIQUE)
	{
		if ((GetWear(WEAR_UNIQUE1) && GetWear(WEAR_UNIQUE1)->IsSameSpecialGroup(item)) ||
			(GetWear(WEAR_UNIQUE2) && GetWear(WEAR_UNIQUE2)->IsSameSpecialGroup(item)))
		{
			ChatPacket(CHAT_TYPE_INFO, "332");
			return false;
		}

		if (marriage::CManager::instance().IsMarriageUniqueItem(item->GetVnum()) &&
			!marriage::CManager::instance().IsMarried(GetPlayerID()))
		{
			ChatPacket(CHAT_TYPE_INFO, "333");
			return false;
		}

	}

	if (item->IsSashSkin() && !GetWear(WEAR_COSTUME_SASH))
	{
		ChatPacket(CHAT_TYPE_INFO, "You can't equip a sash skin without a proper sash.");
		return false;
	}

#ifdef ENABLE_MOUNT_SKIN
	if (item->IsCostumeMountSkin())
	{
		if (IsRiding())
		{
			ChatPacket(CHAT_TYPE_INFO, "334");
			return false;
		}

		LPITEM normalMount = GetWear(WEAR_COSTUME_MOUNT);
		if (!normalMount)
		{
			ChatPacket(CHAT_TYPE_INFO, "1098");
			return false;
		}
	}
#endif

#ifdef ENABLE_DS_SET
	if ((DragonSoul_IsDeckActivated()) && (item->IsDragonSoul()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You must deactivate alchemy to can do this."));
		return false;
	}
#endif

	return true;
}

/// ?Ã¶?Ã§ ?Â³Â¸Â¯???? Â»Ã³??Â¸Â¦ Â¹????Â¸Â·? ?Ã¸Â¿Ã« ???? item?Â» Â¹Ã¾?Â» Â¼Ã¶ ??Â´? ?Ã¶ ?Â®????Â°Ã­, Âº?Â°Â¡Â´? ??Â´?Â¸Ã© ?Â³Â¸Â¯??Â¿Â¡Â°? ???Â¯Â¸Â¦ Â¾?Â·???Â´? ??Â¼Ã¶
bool CHARACTER::CanUnequipNow(const LPITEM item, const TItemPos& srcCell, const TItemPos& destCell) /*const*/
{
	if (item->GetType() == ITEM_WEAPON)
	{
		if (IsAffectFlag(AFF_GWIGUM))
			RemoveAffect(SKILL_GWIGEOM);

		if (IsAffectFlag(AFF_GEOMGYEONG))
			RemoveAffect(SKILL_GEOMKYUNG);
	}
	if (ITEM_BELT == item->GetType())
		VERIFY_MSG(CBeltInventoryHelper::IsExistItemInBeltInventory(this), "ÂºÂ§?Â® ??ÂºÂ¥?Ã¤Â¸Â®Â¿Â¡ Â¾??????? ?Â¸?Ã§??Â¸Ã© ???Â¦?? Â¼Ã¶ Â¾Ã¸Â½?Â´?Â´?.");

	// Â¿ÂµÂ¿Ã¸?Ã· ???Â¦?? Â¼Ã¶ Â¾Ã¸Â´? Â¾?????
	if (IS_SET(item->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		return false;
#ifdef __AURA_SYSTEM__
	if (IsAuraRefineWindowOpen())
		return false;
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (item->IsCostumeMount())
	{
		if (IsRiding())
		{
			ChatPacket(CHAT_TYPE_INFO, "334");
			return false;
		}

#ifdef ENABLE_MOUNT_SKIN
		LPITEM skinMount = GetWear(WEAR_COSTUME_MOUNT_SKIN);
		if (skinMount)
		{
			ChatPacket(CHAT_TYPE_INFO, "1097");
			return false;
		}
#endif

	}
#endif

#ifdef ENABLE_MOUNT_SKIN
	if (item->IsCostumeMountSkin())
	{
		if (IsRiding())
		{
			ChatPacket(CHAT_TYPE_INFO, "334");
			return false;
		}
	}
#endif

	// Â¾????? unequipÂ½? ??ÂºÂ¥?Ã¤Â¸Â®Â·? Â¿?Â±Ã¦ Â¶Â§ ÂºÃ³ ??Â¸Â®Â°Â¡ ??Â´? ?Ã¶ ?Â®??
	{
		int pos = -1;

		if (item->IsDragonSoul())
			pos = GetEmptyDragonSoulInventory(item);
		else
			pos = GetEmptyInventory(item->GetSize());

		VERIFY_MSG(-1 == pos, "Â¼??Ã¶?Â°Â¿Â¡ ÂºÃ³ Â°Ã¸Â°Â£?? Â¾Ã¸Â½?Â´?Â´?.");
	}

#ifdef ENABLE_DS_SET
	if ((DragonSoul_IsDeckActivated()) && (item->IsDragonSoul()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You must deactivate alchemy to can do this."));
		return false;
	}
#endif

	return true;
}

#ifdef ENABLE_SHOW_CHEST_DROP
void CHARACTER::SendChestData(DWORD chestIndex)
{
	const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(chestIndex);
	if (pGroup)
	{
		DWORD chestItemCount = pGroup->GetGroupSize();
		if (chestItemCount > 0)
		{
			TEMP_BUFFER buf;
			TPacketGCChestDrop p;
			p.header = HEADER_GC_CHESTDROP;
			p.size = sizeof(p) + sizeof(DWORD) + sizeof(DWORD) + (chestItemCount * (sizeof(DWORD) * 2));

			buf.write(&p, sizeof(TPacketGCChestDrop));
			buf.write(&chestIndex, sizeof(DWORD));
			buf.write(&chestItemCount, sizeof(DWORD));
			for (int i = 0; i < pGroup->GetGroupSize(); i++)
			{
				DWORD itemVnum = pGroup->GetVnum(i);
				DWORD itemCount = pGroup->GetCount(i);
				buf.write(&itemVnum, sizeof(DWORD));
				buf.write(&itemCount, sizeof(DWORD));
			}
			GetDesc()->Packet(buf.read_peek(), buf.size());
		}
	}
}
#endif

#ifdef ENABLE_MULTI_CHEST
void CHARACTER::OpenChest(LPITEM item, WORD openCount)
{
	if (openCount > item->GetCount())
		openCount = item->GetCount();

	if (openCount > 1000)
		openCount = 1000;

	int cell = -1;
	if (item->IsDragonSoul())
		cell = GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_STORAGE
	else if (item->IsUpgradeItem())
		cell = GetEmptyUpgradeInventory(item);
	else if (item->IsBook())
		cell = GetEmptyBookInventory(item);
	else if (item->IsStone())
		cell = GetEmptyStoneInventory(item);
	else if (item->IsChange())
		cell = GetEmptyChangeInventory(item);
	else if (item->IsCostume())
		cell = GetEmptyCostumeInventory(item);
	else if (item->IsChest())
		cell = GetEmptyChestInventory(item);
#endif
	else
		cell = GetEmptyInventory(item->GetSize());

	if (cell < 0)
	{
		ChatPacket(CHAT_TYPE_INFO, "You do not have space in your inventory");
		return;
	}

	std::map<DWORD, DWORD> dwItemVnums;
	const WORD openCountCache = openCount;
	if (GiveItemFromSpecialItemGroupNew(item->GetVnum(), openCount))
	{
		if (openCount != 0)
		{
			ChatPacket(CHAT_TYPE_INFO, "1096 %d", (openCountCache - openCount));
			RankPlayer::instance().SendInfoPlayer(this, RANK_BY_BOX_OPENED, RankPlayer::instance().GetProgressByPID(GetPlayerID(), RANK_BY_BOX_OPENED) + (openCountCache - openCount), false);
			item->SetCount(item->GetCount() - (openCountCache - openCount));

		}
		else
		{
			RankPlayer::instance().SendInfoPlayer(this, RANK_BY_BOX_OPENED, RankPlayer::instance().GetProgressByPID(GetPlayerID(), RANK_BY_BOX_OPENED) + openCountCache, false);
			item->SetCount(item->GetCount() - openCountCache);

		}
	}
	else
		ChatPacket(CHAT_TYPE_INFO, "976");
}
bool CHARACTER::AutoGiveItemChest(LPITEM item, DWORD& itemCount, std::map<LPITEM, WORD>& vecUpdateItems)
{
	int cell = -1;
	if (item->IsDragonSoul())
		cell = GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_STORAGE
	else if (item->IsUpgradeItem())
		cell = GetEmptyUpgradeInventory(item);
	else if (item->IsBook())
		cell = GetEmptyBookInventory(item);
	else if (item->IsStone())
		cell = GetEmptyStoneInventory(item);
	else if (item->IsChange())
		cell = GetEmptyChangeInventory(item);
	else if (item->IsCostume())
		cell = GetEmptyCostumeInventory(item);
	else if (item->IsChest())
		cell = GetEmptyChestInventory(item);
#endif
	else
		cell = GetEmptyInventory(item->GetSize());

	if (cell == -1)
		return false;

	const DWORD itemVnum = item->GetVnum();
	WORD wCount = item->GetCount();

	if (item->IsStackable() && item->GetType() != ITEM_BLEND)
	{
		for (auto it = vecUpdateItems.begin(); it != vecUpdateItems.end(); ++it)
		{
			LPITEM item2 = it->first;
			if (itemVnum == item2->GetVnum())
			{
				const DWORD item2Count = item2->GetCount();
				const WORD bCount2 = MIN(g_bItemCountLimit - item2Count, wCount);
				if (bCount2 > 0)
				{
					wCount -= bCount2;
					item2->SetCount(item2Count + bCount2);
					if (wCount == 0)
					{
						itemCount = 0;
						return true;
					}
				}
			}
		}
#ifdef ENABLE_SPECIAL_STORAGE
		if (item->IsUpgradeItem())
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = GetUpgradeInventoryItem(i);
				if (!item2)
					continue;
				if (item2->GetVnum() == itemVnum)
				{
					BYTE j;
					for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
						if (item2->GetSocket(j) != item->GetSocket(j))
							break;
					if (j != ITEM_SOCKET_MAX_NUM)
						continue;
					const DWORD item2Count = item2->GetCount();
					const WORD bCount2 = MIN(g_bItemCountLimit - item2Count, wCount);
					if (bCount2 > 0)
					{
						const auto itItem = vecUpdateItems.find(item2);
						if (itItem == vecUpdateItems.end())
							vecUpdateItems.emplace(item2, item2Count);
						wCount -= bCount2;
						item2->SetUpdateStatus(true);
						item2->SetCount(item2Count + bCount2);
						if (wCount == 0)
						{
							itemCount = 0;
							return true;
						}
					}
				}
			}
		}
		else if (item->IsBook())
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = GetBookInventoryItem(i);
				if (!item2)
					continue;
				if (item2->GetVnum() == itemVnum)
				{
					BYTE j;
					for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
						if (item2->GetSocket(j) != item->GetSocket(j))
							break;
					if (j != ITEM_SOCKET_MAX_NUM)
						continue;
					const DWORD item2Count = item2->GetCount();
					const WORD bCount2 = MIN(g_bItemCountLimit - item2Count, wCount);
					if (bCount2 > 0)
					{
						const auto itItem = vecUpdateItems.find(item2);
						if (itItem == vecUpdateItems.end())
							vecUpdateItems.emplace(item2, item2Count);
						wCount -= bCount2;
						item2->SetUpdateStatus(true);
						item2->SetCount(item2Count + bCount2);
						if (wCount == 0)
						{
							itemCount = 0;
							return true;
						}
					}
				}
			}
		}
		else if (item->IsStone())
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = GetStoneInventoryItem(i);
				if (!item2)
					continue;
				if (item2->GetVnum() == itemVnum)
				{
					BYTE j;
					for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
						if (item2->GetSocket(j) != item->GetSocket(j))
							break;
					if (j != ITEM_SOCKET_MAX_NUM)
						continue;
					const DWORD item2Count = item2->GetCount();
					const WORD bCount2 = MIN(g_bItemCountLimit - item2Count, wCount);
					if (bCount2 > 0)
					{
						const auto itItem = vecUpdateItems.find(item2);
						if (itItem == vecUpdateItems.end())
							vecUpdateItems.emplace(item2, item2Count);
						wCount -= bCount2;
						item2->SetUpdateStatus(true);
						item2->SetCount(item2Count + bCount2);
						if (wCount == 0)
						{
							itemCount = 0;
							return true;
						}
					}
				}
			}
		}
		else if (item->IsChange())
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = GetChangeInventoryItem(i);
				if (!item2)
					continue;
				if (item2->GetVnum() == itemVnum)
				{
					BYTE j;
					for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
						if (item2->GetSocket(j) != item->GetSocket(j))
							break;
					if (j != ITEM_SOCKET_MAX_NUM)
						continue;
					const DWORD item2Count = item2->GetCount();
					const WORD bCount2 = MIN(g_bItemCountLimit - item2Count, wCount);
					if (bCount2 > 0)
					{
						const auto itItem = vecUpdateItems.find(item2);
						if (itItem == vecUpdateItems.end())
							vecUpdateItems.emplace(item2, item2Count);
						wCount -= bCount2;
						item2->SetUpdateStatus(true);
						item2->SetCount(item2Count + bCount2);
						if (wCount == 0)
						{
							itemCount = 0;
							return true;
						}
					}
				}
			}
		}
		else if (item->IsChest())
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = GetChestInventoryItem(i);
				if (!item2)
					continue;
				if (item2->GetVnum() == itemVnum)
				{
					BYTE j;
					for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
						if (item2->GetSocket(j) != item->GetSocket(j))
							break;
					if (j != ITEM_SOCKET_MAX_NUM)
						continue;
					const DWORD item2Count = item2->GetCount();
					const WORD bCount2 = MIN(g_bItemCountLimit - item2Count, wCount);
					if (bCount2 > 0)
					{
						const auto itItem = vecUpdateItems.find(item2);
						if (itItem == vecUpdateItems.end())
							vecUpdateItems.emplace(item2, item2Count);
						wCount -= bCount2;
						item2->SetUpdateStatus(true);
						item2->SetCount(item2Count + bCount2);
						if (wCount == 0)
						{
							itemCount = 0;
							return true;
						}
					}
				}
			}
		}
#endif
		else
		{
			for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = GetInventoryItem(i);
				if (!item2)
					continue;
				if (item2->GetVnum() == itemVnum)
				{
					BYTE j;
					for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
						if (item2->GetSocket(j) != item->GetSocket(j))
							break;
					if (j != ITEM_SOCKET_MAX_NUM)
						continue;
					const DWORD item2Count = item2->GetCount();
					const WORD bCount2 = MIN(g_bItemCountLimit - item2Count, wCount);
					if (bCount2 > 0)
					{
						const auto itItem = vecUpdateItems.find(item2);
						if (itItem == vecUpdateItems.end())
							vecUpdateItems.emplace(item2, item2Count);
						wCount -= bCount2;
						item2->SetUpdateStatus(true);
						item2->SetCount(item2Count + bCount2);
						if (wCount == 0)
						{
							itemCount = 0;
							return true;
						}
					}
				}
			}
		}
	}
	if (wCount > 0)
	{
		item->SetCount(wCount);
		if (item->IsDragonSoul())
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, cell));
#ifdef ENABLE_SPECIAL_STORAGE
		else if (item->IsUpgradeItem())
			item->AddToCharacter(this, TItemPos(UPGRADE_INVENTORY, cell));
		else if (item->IsBook())
			item->AddToCharacter(this, TItemPos(BOOK_INVENTORY, cell));
		else if (item->IsStone())
			item->AddToCharacter(this, TItemPos(STONE_INVENTORY, cell));
		else if (item->IsChange())
			item->AddToCharacter(this, TItemPos(CHANGE_INVENTORY, cell));
		else if (item->IsCostume())
			item->AddToCharacter(this, TItemPos(COSTUME_INVENTORY, cell));
		else if (item->IsChest())
			item->AddToCharacter(this, TItemPos(CHEST_INVENTORY, cell));
#endif
		else
			item->AddToCharacter(this, TItemPos(INVENTORY, cell));

		if (item->IsStackable())
		{
			const auto itItem = vecUpdateItems.find(item);
			if (itItem == vecUpdateItems.end())
				vecUpdateItems.emplace(item, wCount);
		}
		itemCount = wCount;
		return true;
	}
	return false;
}

bool CHARACTER::GiveItemFromSpecialItemGroupNew(DWORD dwGroupNum, WORD& loopCount)
{
	const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(dwGroupNum);
	if (!pGroup)
	{
		sys_err("cannot find special item group %d", dwGroupNum);
		return false;
	}
	std::map<DWORD, WORD> vecItemCounts;
	std::map<LPITEM, WORD> vecUpdateItems;
	const WORD constCount = loopCount;
	WORD workCount = loopCount;

	int returnCount = 5;

	for (DWORD j = 0; j < constCount; ++j)
	{
		if (returnCount <= 0 || workCount <= 0)
			break;
		std::vector <int> idxes;
		const int n = pGroup->GetMultiIndex(idxes);

		bool isOkey = false;

		for (int i = 0; i < n; i++)
		{
			if (returnCount <= 0 || (workCount <= 0 && !isOkey))
				break;

			const int idx = idxes[i];
			const DWORD dwVnum = pGroup->GetVnum(idx);
			const DWORD dwEarnCount = pGroup->GetCount(idx);
			DWORD dwCount = pGroup->GetCount(idx);

			switch (dwVnum)
			{
			case CSpecialItemGroup::GOLD:
				PointChange(POINT_GOLD, dwCount);
				if (isOkey == false)
				{
					isOkey = true;
					workCount -= 1;
				}
				break;
			case CSpecialItemGroup::POISON:
				AttackedByPoison(NULL);
				if (isOkey == false)
				{
					isOkey = true;
					workCount -= 1;
				}
				break;
			case CSpecialItemGroup::EXP:
				PointChange(POINT_EXP, dwCount);
				if (isOkey == false)
				{
					isOkey = true;
					workCount -= 1;
				}
				break;
			case CSpecialItemGroup::MOB:
			{
				LPCHARACTER ch = CHARACTER_MANAGER::instance().SpawnMob(dwCount, GetMapIndex(), GetX() + number(-500, 500), GetY() + number(-500, 500), 0, true, -1);
				if (ch)
					ch->SetAggressive();

				if (isOkey == false)
				{
					isOkey = true;
					workCount -= 1;
				}
			}
			break;
			case CSpecialItemGroup::MOB_GROUP:
				CHARACTER_MANAGER::instance().SpawnGroup(dwCount, GetMapIndex(), GetX() - number(300, 500), GetY() - number(300, 500), GetX() + number(300, 500), GetY() + number(300, 500), NULL, true);
				if (isOkey == false)
				{
					isOkey = true;
					workCount -= 1;
				}
				break;
			case CSpecialItemGroup::SLOW:
				AddAffect(AFFECT_SLOW, POINT_MOV_SPEED, -(int)dwCount, AFF_SLOW, 300, 0, true);
				if (isOkey == false)
				{
					isOkey = true;
					workCount -= 1;
				}
				break;
			case CSpecialItemGroup::DRAIN_HP:
			{
				int iDropHP = GetMaxHP() * dwCount / 100;
				iDropHP = MIN(iDropHP, GetHP() - 1);
				PointChange(POINT_HP, -iDropHP);
				if (isOkey == false)
				{
					isOkey = true;
					workCount -= 1;
				}
			}
			break;
#ifdef ENABLE_WOLFMAN
			case CSpecialItemGroup::BLEEDING:
			{
				AttackedByBleeding(NULL);
				if (isOkey == false)
				{
					isOkey = true;
					workCount -= 1;
				}
			}
			break;
#endif
			default:
			{
				LPITEM item = ITEM_MANAGER::Instance().CreateItem(dwVnum, dwCount);
				if (item)
				{
					if (AutoGiveItemChest(item, dwCount, vecUpdateItems))
					{
						if (isOkey == false)
						{
							isOkey = true;
							workCount -= 1;
						}

						auto itCount = vecItemCounts.find(dwVnum);
						if (itCount != vecItemCounts.end())
							itCount->second += dwEarnCount;
						else
							vecItemCounts.emplace(dwVnum, dwEarnCount);
					}
					else
					{
						// item->AddToGround(GetMapIndex(), GetXYZ());
						// item->StartDestroyEvent();
						// item->SetOwnership(this, 300);

						M2_DESTROY_ITEM(item);
						returnCount -= 1;
					}
				}
			}
			}
		}
	}

	for (auto it = vecUpdateItems.begin(); it != vecUpdateItems.end(); ++it)
	{
		LPITEM item = it->first;
		item->SetUpdateStatus(false);
		if (item->GetCount() != it->second)
			item->UpdatePacket();

		if (item->GetType() == ITEM_QUEST || item->GetType() == ITEM_MATERIAL)
			quest::CQuestManager::instance().PickupItem(GetPlayerID(), item);
	}

	for (auto it = vecItemCounts.begin(); it != vecItemCounts.end(); ++it)
		ChatPacket(CHAT_TYPE_ITEM_INFO, "165 I%d %d", it->first, it->second);

	loopCount = workCount;
	return (workCount != constCount);
}
#endif

LPITEM CHARACTER::GetSpecialStorageItem(const WORD& wCell, int invType) const
{
	return GetItem(TItemPos(invType, wCell));
}

int CHARACTER::GetEmptySpecialStorageSlotFromIndex(WORD index, BYTE itemSize, int invType) const
{
	// #ifdef ENABLE_EXTEND_INVEN_SYSTEM
		// BYTE bWindow = -1;

		// if (invType == UPGRADE_INVENTORY)
			// bWindow = 0;
		// else if (invType == BOOK_INVENTORY)
			// bWindow = 1;
		// else if (invType == STONE_INVENTORY)
			// bWindow = 2;
		// else if (invType == GENERAL_INVENTORY)
			// bWindow = 3;
		// else if (invType == CHANGE_INVENTORY)
			// bWindow = 4;
		// else if (invType == COSTUME_INVENTORY)
			// bWindow = 5;
		// else if (invType == CHEST_INVENTORY)
			// bWindow = 6;

		// if (bWindow == -1 || bWindow >= 7)
			// return -1;

		// if (index > GetStorageSize(bWindow))
			// return -1;

		// for (WORD i = index; i < GetStorageSize(bWindow); ++i)
	// #else
	if (index > SPECIAL_INVENTORY_MAX_NUM)
		return -1;

	for (WORD i = index; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		// #endif
		if (IsEmptyItemGrid(TItemPos(invType, i), itemSize))
			return i;

	return -1;
}

int CHARACTER::GetEmptyInventoryFromIndex(WORD index, BYTE itemSize) const
{
#if defined(__BL_BATTLE_ROYALE__)
	if (IsBattleRoyale() && index >= GetInventorySize())
		return false;
#endif
	// #ifdef ENABLE_EXTEND_INVEN_SYSTEM
		// if (index > GetInventorySize())
	// #else
	if (index > INVENTORY_MAX_NUM)
		// #endif
		return -1;

	// #ifdef ENABLE_EXTEND_INVEN_SYSTEM
		// for (WORD i = index; i < GetInventorySize(); ++i)
	// #else
	for (WORD i = index; i < INVENTORY_MAX_NUM; ++i)
		// #endif
		if (IsEmptyItemGrid(TItemPos(INVENTORY, i), itemSize))
			return i;

	return -1;
}

bool CHARACTER::UseItemBoosterAddAttribute(CItem& item, CItem& targetItem)
{
	if (!targetItem.IsBooster() || targetItem.IsExchanging() ||
		targetItem.IsEquipped() || targetItem.GetAttributeSetIndex() == -1)
	{
		ChatPacket(CHAT_TYPE_INFO, "You cannot add another attribute.");

		return false;
	}

	auto maxAttrCount = 3;
	if (targetItem.GetAttributeCount() >= maxAttrCount)
	{
		ChatPacket(CHAT_TYPE_INFO, "You cannot add another attribute.");
		return false;
	}

	if (number(1, 100) <= aiItemAttributeAddPercent[targetItem.GetAttributeCount()])
	{
		targetItem.AddAttribute();
		ChatPacket(CHAT_TYPE_INFO, "Upgrade successfully added. ");

		auto addedIndex = targetItem.GetAttributeCount() - 1;
		const std::string hint = std::to_string(targetItem.GetID());
		LogManager::instance().ItemLog(
			GetPlayerID(), targetItem.GetAttributeType(addedIndex),
			targetItem.GetAttributeValue(addedIndex), item.GetID(),
			"ADD_ATTRIBUTE_BOOSTER", hint.c_str(), GetDesc()->GetHostName(),
			item.GetOriginalVnum());
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ìì± ì¶ê°ì ì¤í¨íììµëë¤."));
	}

	if (targetItem.IsEquipped())
		BuffOnAttr_RemoveBuffsFromItem(&targetItem);

	item.DecrementCount();
	return true;
}

bool CHARACTER::UseItemBoosterChangeAttribute(CItem& item, CItem& targetItem)
{
	if (!targetItem.IsBooster() || targetItem.IsExchanging() ||
		targetItem.IsEquipped() || targetItem.GetAttributeSetIndex() == -1 ||
		targetItem.GetAttributeCount() == 0)
		return false;

	if (targetItem.IsEquipped())
		BuffOnAttr_RemoveBuffsFromItem(&targetItem);

	targetItem.ChangeAttribute();

	const std::string hint = std::to_string(targetItem.GetID());
	LogManager::Instance().ItemLog(this, &item, "CHANGE_ATTRIBUTE_BOOSTER",
		hint.c_str());

	item.DecrementCount();
	return true;
}

UINT CHARACTER::GetInventorySize(uint32_t inventoryType) const
{
#ifdef __BL_BATTLE_ROYALE__
	if (IsBattleRoyale())
	{
		if (inventoryType > 0)
			return 0;
		return BattleRoyale::BR_INVENTORY_SLOTS;
	}
#endif
#ifdef UNLOCK_INVENTORY_WORLDARD
	//return GetInventorySize(inventoryType); // worldard or smth
	return 90 + (5 * GetUnlockSlotsW(inventoryType)); // worldard or smth
#else
	return INVENTORY_MAX_NUM;
#endif
}
