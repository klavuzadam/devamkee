#include "stdafx.h"
#ifdef __FreeBSD__
#include <md5.h>
#else
#include "../libthecore/xmd5.h"
#endif
#include "utils.h"
#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "motion.h"
#include "packet.h"
#include "affect.h"
#include "pvp.h"
#include "start_position.h"
#include "party.h"
#include "guild_manager.h"
#include "p2p.h"
#include "dungeon.h"
#include "messenger_manager.h"
#include "war_map.h"
#include "questmanager.h"
#include "item_manager.h"
#include "monarch.h"
#include "mob_manager.h"
#include "dev_log.h"
#include "item.h"
#include "arena.h"
#include "buffer_manager.h"
#include "unique_item.h"
#include "threeway_war.h"
#include "log.h"
#include "../common/VnumHelper.h"
#if defined(WJ_COMBAT_ZONE)
	#include "combat_zone.h"
#endif
#ifdef ENABLE_BIOLOG_SYSTEM
	#include "biolog.h"
#endif
#ifdef ENABLE_MOUNT_SYSTEM
#include "MountSystem.h"
#endif
#ifdef __NEWPET_SYSTEM__
#include "New_PetSystem.h"
#endif

#ifdef ENABLE_SUPPORT_SYSTEM
#include "SupportSystem.h"
#endif
//search offline
#include "target.h"
//search offline end
#ifdef ENABLE_MAINTENANCE_SYSTEM	
	#include "maintenance.h"
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
	#include "tournament.h"
#endif
#ifdef ENABLE_QUEEN_NETHIS
#	include "SnakeLair.h"
#endif
#include "RankPlayer.h"

#include "itemblock.h"
#ifdef USE_DAY_CHANGE_MGR
#include "day_change_mgr.hpp"
#endif

#ifdef ENABLE_HIDE_COSTUME_SYSTEM
ACMD (do_costume_hide)
{
	char arg1[256];
	one_argument (argument, arg1, sizeof(arg1));
	
	if (0 == arg1[0])
		return;

	int slot = atoi(arg1);

	ch->FuncHideCostume(slot);
}
#endif

#ifdef __AUTO_HUNT__
ACMD(do_auto_hunt)
{
	ch->GetAutoHuntCommand(argument);
}
#endif

#ifdef ENABLE_SUPPORT_SYSTEM
ACMD(do_support_system_clear)
{

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;

	int type = atoi(arg1);

	if (type < 1 || type >6){
		return;
	}
	
	if (ch->GetSupportSystem() && ch->GetSupportSystem()->IsActiveSupport())
	{
		LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(ch->GetSupportSystem()->GetActiveSupport()->GetSummonItemVID());
		if (pSummonItem != NULL)
		{
			//Clear Body Costume Buff
			if (type == 1)
			{
				TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(pSummonItem->GetSocket(1));
				if (!pItemTable)
				{
					sys_err("[do_support_system_clear] Item vnum: %u doesn't exists -> playerID %u.", pSummonItem->GetSocket(1), ch->GetPlayerID());
					return;
				}

				int iEmptyPos = ch->GetEmptyInventory(pItemTable->bSize);
				if (iEmptyPos == -1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Usted no cuenta con espacio suficiente en su inventario.");
					return;
				}

				LPITEM item_give = ITEM_MANAGER::instance().CreateItem(pSummonItem->GetSocket(1), 1);
				if (!item_give)
				{
					sys_err("[do_support_system_clear] Item vnum: %u cannot be created -> playerID %u.", pSummonItem->GetSocket(1), ch->GetPlayerID());
					return;
				}

				int iLimitRealtimeStartFirstUseFlagIndex = -1;
				for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
				{
					if (LIMIT_REAL_TIME_START_FIRST_USE == item_give->GetProto()->aLimits[i].bType){
						iLimitRealtimeStartFirstUseFlagIndex = i;
					}
				}

				if (ch->GetQuestFlag("costume_support.costume_body_permanente") == 0)
				{
					item_give->SetSocket(0,  ch->GetQuestFlag("costume_support.costume_body"));
				}
				

				if (iLimitRealtimeStartFirstUseFlagIndex != -1){
					item_give->SetSocket(1, item_give->GetSocket(1) + 1);
				}

				item_give->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

				pSummonItem->SetSocket(1,0);
				ch->SetQuestFlag("costume_support.costume_body",time(0));
				ch->SetQuestFlag("costume_support.costume_body_permanentem",0);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportClear %d",1);
			}
			//Clear Hair Costume Buff
			else if (type == 2)
			{
				TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(ch->GetQuestFlag("costume_support.costume_hair_vnum"));
				if (!pItemTable)
				{
					sys_err("[do_support_system_clear] Item vnum: %u doesn't exists -> playerID %u.", ch->GetQuestFlag("costume_support.costume_hair_vnum"), ch->GetPlayerID());
					return;
				}

				int iEmptyPos = ch->GetEmptyInventory(item_give->GetSize());
				if (iEmptyPos == -1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Usted no cuenta con espacio suficiente en su inventario.");
					return;
				}

				LPITEM item_give = ITEM_MANAGER::instance().CreateItem(ch->GetQuestFlag("costume_support.costume_hair_vnum"), 1);
				if (!item_give)
				{
					sys_err("[do_support_system_clear] Item vnum: %u cannot be created -> playerID %u.", ch->GetQuestFlag("costume_support.costume_hair_vnum"), ch->GetPlayerID());
					return;
				}

				int iLimitRealtimeStartFirstUseFlagIndex = -1;
				for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
				{
					if (LIMIT_REAL_TIME_START_FIRST_USE == item_give->GetProto()->aLimits[i].bType){
						iLimitRealtimeStartFirstUseFlagIndex = i;
					}
				}

				if (ch->GetQuestFlag("costume_support.costume_hair_permanente") == 0)
				{
					item_give->SetSocket(0, ch->GetQuestFlag("costume_support.costume_hair"));
				}
				
				if (iLimitRealtimeStartFirstUseFlagIndex != -1){
					item_give->SetSocket(1, item_give->GetSocket(1) + 1);
				}
				item_give->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

				pSummonItem->SetSocket(2,0);
				ch->SetQuestFlag("costume_support.costume_hair",time(0));
				ch->SetQuestFlag("costume_support.costume_hair_vnum",0);
				ch->SetQuestFlag("costume_support.costume_hair_permanente",0);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportClear %d",2);
			}
			//Clear Weapon Costume Buff
			else if (type == 3)
			{
				TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(pSummonItem->GetSocket(0));
				if (!pItemTable)
				{
					sys_err("[do_support_system_clear] Item vnum: %u doesn't exists -> playerID %u.", pSummonItem->GetSocket(0), ch->GetPlayerID());
					return;
				}

				int iEmptyPos = ch->GetEmptyInventory(item_give->GetSize());
				if (iEmptyPos == -1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Usted no cuenta con espacio suficiente en su inventario.");
					return;
				}

				LPITEM item_give = ITEM_MANAGER::instance().CreateItem(pSummonItem->GetSocket(0), 1);
				if (!item_give)
				{
					sys_err("[do_support_system_clear] Item vnum: %u cannot be created -> playerID %u.", pSummonItem->GetSocket(0), ch->GetPlayerID());
					return;
				}

				int iLimitRealtimeStartFirstUseFlagIndex = -1;
				for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
				{
					if (LIMIT_REAL_TIME_START_FIRST_USE == item_give->GetProto()->aLimits[i].bType){
						iLimitRealtimeStartFirstUseFlagIndex = i;
					}
				}

				if (ch->GetQuestFlag("costume_support.costume_weapon_permanente") == 0)
				{
					item_give->SetSocket(0, ch->GetQuestFlag("costume_support.costume_weapon"));
				}
				

				if (iLimitRealtimeStartFirstUseFlagIndex != -1){
					item_give->SetSocket(1, item_give->GetSocket(1) + 1);
				}

				item_give->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

				pSummonItem->SetSocket(0,0);
				ch->SetQuestFlag("costume_support.costume_weapon",time(0));
				ch->SetQuestFlag("costume_support.costume_weapon_permanente",0);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportClear %d",3);
			}
			//Clear Item 1 Extra Buff
			else if (type == 4)
			{
				TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(pSummonItem->GetAttributeValue(1) + 55500);
				if (!pItemTable)
				{
					sys_err("[do_support_system_clear] Item vnum: %u doesn't exists -> playerID %u.", pSummonItem->GetAttributeValue(1) + 55500, ch->GetPlayerID());
					return;
				}

				int iEmptyPos = ch->GetEmptyInventory(item_give->GetSize());
				if (iEmptyPos == -1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Usted no cuenta con espacio suficiente en su inventario.");
					return;
				}

				LPITEM item_give = ITEM_MANAGER::instance().CreateItem(pSummonItem->GetAttributeValue(1) + 55500, 1);
				if (!item_give)
				{
					sys_err("[do_support_system_clear] Item vnum: %u cannot be created -> playerID %u.", pSummonItem->GetAttributeValue(1) + 55500, ch->GetPlayerID());
					return;
				}

				item_give->SetSocket(0, ch->GetQuestFlag("items_support.item_support_extra_1"));
				item_give->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

				pSummonItem->SetForceAttribute(1, 0, 0);
				ch->SetQuestFlag("items_support.item_support_extra_1",time(0));
				ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportClear %d",4);
			}
			//Clear Item 2 Extra Buff
			else if (type == 5)
			{
				TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(pSummonItem->GetAttributeValue(2) + 55500);
				if (!pItemTable)
				{
					sys_err("[do_support_system_clear] Item vnum: %u doesn't exists -> playerID %u.", pSummonItem->GetAttributeValue(2) + 55500, ch->GetPlayerID());
					return;
				}

				int iEmptyPos = ch->GetEmptyInventory(item_give->GetSize());
				if (iEmptyPos == -1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Usted no cuenta con espacio suficiente en su inventario.");
					return;
				}

				LPITEM item_give = ITEM_MANAGER::instance().CreateItem(pSummonItem->GetAttributeValue(2) + 55500, 1);
				if (!item_give)
				{
					sys_err("[do_support_system_clear] Item vnum: %u cannot be created -> playerID %u.", pSummonItem->GetAttributeValue(2) + 55500, ch->GetPlayerID());
					return;
				}

				item_give->SetSocket(0, ch->GetQuestFlag("items_support.item_support_extra_2"));
				item_give->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

				pSummonItem->SetForceAttribute(2, 0, 0);
				ch->SetQuestFlag("items_support.item_support_extra_2",time(0));
				ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportClear %d",5);
			}
			//Clear Item 3 Extra Buff
			else if (type == 6)
			{
				TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(pSummonItem->GetAttributeValue(3) + 55500);
				if (!pItemTable)
				{
					sys_err("[do_support_system_clear] Item vnum: %u doesn't exists -> playerID %u.", pSummonItem->GetAttributeValue(3) + 55500, ch->GetPlayerID());
					return;
				}

				int iEmptyPos = ch->GetEmptyInventory(item_give->GetSize());
				if (iEmptyPos == -1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Usted no cuenta con espacio suficiente en su inventario.");
					return;
				}

				LPITEM item_give = ITEM_MANAGER::instance().CreateItem(pSummonItem->GetAttributeValue(3) + 55500, 1);
				if (!item_give)
				{
					sys_err("[do_support_system_clear] Item vnum: %u cannot be created -> playerID %u.", pSummonItem->GetAttributeValue(3) + 55500, ch->GetPlayerID());
					return;
				}

				item_give->SetSocket(0, ch->GetQuestFlag("items_support.item_support_extra_3"));
				item_give->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

				pSummonItem->SetForceAttribute(3, 0, 0);
				ch->SetQuestFlag("items_support.item_support_extra_3",time(0));
				ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportClear %d",6);
			}
		}
	}
}
#endif

#ifdef ENABLE_SUPPORT_SYSTEM
ACMD(do_support_system)
{
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	
	if (!*arg1 || !*arg2)
		return;

	int slot = atoi(arg1);
	int type = atoi(arg2);

	if (type < 1 || type >6){
		return;
	}

	if (ch->GetSupportSystem()->IsActiveSupport())
	{
		CSupportSystem* supportSystem = ch->GetSupportSystem();

		LPITEM item = ch->GetItem(TItemPos(INVENTORY, slot));
		LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(ch->GetSupportSystem()->GetActiveSupport()->GetSummonItemVID());
		if (item != NULL && pSummonItem != NULL){

			if (item->IsExchanging())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "<Support System> You can not do this when you are exchanging!");
				return;
			}

			if (type >= 1 && type <=3){
				if (item->GetType() == ITEM_COSTUME){
					
					int iLimitRealtimeStartFirstUseFlagIndex = -1;
					for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
					{
						if (LIMIT_REAL_TIME_START_FIRST_USE == item->GetProto()->aLimits[i].bType){
							iLimitRealtimeStartFirstUseFlagIndex = i;
						}
					}

					int duration = 0;
					bool permanente = false;

					if (iLimitRealtimeStartFirstUseFlagIndex != -1){
						if  (item->GetSocket(1) == 0)
						{
							if (item->GetSocket(0) != 0)
							{
								duration = item->GetSocket(0);	
							}else{
								duration = time(0) + item->GetProto()->aLimits[iLimitRealtimeStartFirstUseFlagIndex].lValue;
							}

							if (0 == duration){
								duration = time(0) + 60 * 60 * 24 * 7;
							}
						}
						else{
							duration = item->GetSocket(0);
						}
					}else{
						//duration = time(0) + 60 * 60 * 24 * 30;
						permanente = true;
						//duration = item->GetSocket(0);
					}

					//Costume Body Add
					if (type == 1){
						if(item->GetSubType() == COSTUME_BODY)
						{
							pSummonItem->SetSocket(1,item->GetVnum());
							if (permanente == false){
								ch->SetQuestFlag("costume_support.costume_body",duration);
								ch->SetQuestFlag("costume_support.costume_body_permanente",0);
								ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportArmor %u %d %d", item->GetVnum(),ch->GetQuestFlag("costume_support.costume_body")-get_global_time(), 0);
							}
							else{
								ch->SetQuestFlag("costume_support.costume_body",0);
								ch->SetQuestFlag("costume_support.costume_body_permanente",1);
								ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportArmor %u %d %d", item->GetVnum(),0, 1);
							}
							item->RemoveFromCharacter();
						}
						else
						{
							ch->ChatPacket(CHAT_TYPE_INFO, "<Support System> You can not put this item!");
							return;
						}
					}

					//Costume Hair Add
					if(type == 2){
						if(item->GetSubType() == COSTUME_HAIR)
						{
							pSummonItem->SetSocket(2,item->GetValue(3));


							ch->SetQuestFlag("costume_support.costume_hair_vnum",item->GetVnum());

							if (permanente == false){
								ch->SetQuestFlag("costume_support.costume_hair",duration);
								ch->SetQuestFlag("costume_support.costume_hair_permanente",0);
								ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportHair %u %d %d", item->GetVnum(),ch->GetQuestFlag("costume_support.costume_hair")-get_global_time(),0);
							}else{

								ch->SetQuestFlag("costume_support.costume_hair",0);
								ch->SetQuestFlag("costume_support.costume_hair_permanente",1);
								ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportHair %u %d %d", item->GetVnum(),0,1);
							}

							item->RemoveFromCharacter();
						}
						else
						{
							ch->ChatPacket(CHAT_TYPE_INFO, "<Support System> You can not put this item!");
							return;
						}	
					}

					//Costume Weapon Add
					if(type == 3){
						if(item->GetSubType() == COSTUME_WEAPON)
						{
							pSummonItem->SetSocket(0,item->GetVnum());

							if (permanente == false){

								ch->SetQuestFlag("costume_support.costume_weapon",duration);
								ch->SetQuestFlag("costume_support.costume_weapon_permanente",0);
								ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportWeapon %u %d %d", item->GetVnum(), ch->GetQuestFlag("costume_support.costume_weapon")-get_global_time(),0);

							}else{

								ch->SetQuestFlag("costume_support.costume_weapon",0);
								ch->SetQuestFlag("costume_support.costume_weapon_permanente",1);
								ch->ChatPacket(CHAT_TYPE_COMMAND, "SupportWeapon %u %d %d", item->GetVnum(), 0,1);
							}

							item->RemoveFromCharacter();
						}
						else
						{
							ch->ChatPacket(CHAT_TYPE_INFO, "<Support System> You can not put this item!");
							return;
						}	
					}
				}
			}
			if(type >= 4 && type <= 6)
			{
				if(type == 4)
				{
					if (item->GetVnum() >= 55502 && item->GetVnum() <= 55504){

						pSummonItem->SetForceAttribute(1, 1, item->GetVnum()-55500);
						ch->SetQuestFlag("items_support.item_support_extra_1",item->GetSocket(0));
						ch->ChatPacket(CHAT_TYPE_COMMAND, "SetSupportSkill 4 %u %d", item->GetVnum(), ch->GetQuestFlag("items_support.item_support_extra_1")-get_global_time());
						item->RemoveFromCharacter();

					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "<Support System> You can not put this item!");
						return;
					}	
				}
				if(type == 5)
				{
					if (item->GetVnum() >= 55505 && item->GetVnum() <= 55507){

						pSummonItem->SetForceAttribute(2, 1, item->GetVnum()-55500);
						ch->SetQuestFlag("items_support.item_support_extra_2",item->GetSocket(0));
						ch->ChatPacket(CHAT_TYPE_COMMAND, "SetSupportSkill 5 %u %d", item->GetVnum(), ch->GetQuestFlag("items_support.item_support_extra_2")-get_global_time());
						item->RemoveFromCharacter();

					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "<Support System> You can not put this item!");
						return;
					}	
				}
				if(type == 6)
				{
					if (item->GetVnum() >= 55508 && item->GetVnum() <= 55510){

						pSummonItem->SetForceAttribute(3, 1, item->GetVnum()-55500);
						ch->SetQuestFlag("items_support.item_support_extra_3",item->GetSocket(0));
						ch->ChatPacket(CHAT_TYPE_COMMAND, "SetSupportSkill 6 %u %d", item->GetVnum(), ch->GetQuestFlag("items_support.item_support_extra_3")-get_global_time());
						item->RemoveFromCharacter();
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "<Support System> You can not put this item!");
						return;
					}	
				}
			}
		}
		ch->GetSupportSystem()->GetActiveSupport()->RefreshCostume();
	}
	else
		ch->ChatPacket(CHAT_TYPE_INFO, "<Support System> You have to call your support for use this function.");
}
#endif

ACMD(do_real_sort_items)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		return;
	}

	int page = atoi(arg1);
	
    int lastSortInventoryPulse = ch->GetSortInventoryPulse();
    int currentPulse = thecore_pulse();
   
    if (lastSortInventoryPulse > currentPulse) {
        int deltaInSeconds = ((lastSortInventoryPulse / PASSES_PER_SEC(1)) - (currentPulse / PASSES_PER_SEC(1)));
        int minutes = deltaInSeconds / 60;
        int seconds = (deltaInSeconds - (minutes * 60));
		
		ch->ChatPacket(CHAT_TYPE_INFO, "394 %d", seconds);
        return;
    }

	if (ch->IsDead() || !ch->CanHandleItem() || ch->GetExchange() || ch->m_pkTimedEvent)
		return;

	int SortStart = 0, SortEnd = 0;

	switch (page)
	{
		case 0:
			SortStart = 0;
			SortEnd = INVENTORY_MAX_NUM;
			break;
			
		default:
			SortStart = 0;
			SortEnd = SPECIAL_INVENTORY_MAX_NUM;
	}

	std::vector<LPITEM> TempInvVec;

	for (WORD i = SortStart; i < SortEnd; ++i)
	{
		LPITEM CurrItem = nullptr;

		switch(page)
		{
			case 0:
				CurrItem = ch->GetInventoryItem(i);
				break;
			case 1:
				CurrItem = ch->GetUpgradeInventoryItem(i);
				break;
			case 2:
				CurrItem = ch->GetBookInventoryItem(i);
				break;
			case 3:
				CurrItem = ch->GetStoneInventoryItem(i);
				break;
			case 4:
				CurrItem = ch->GetChangeInventoryItem(i);
				break;
			case 5:
				CurrItem = ch->GetCostumeInventoryItem(i);
				break;
			case 6:
				CurrItem = ch->GetChestInventoryItem(i);
				break;
			default:
				return;
		}

		if (!CurrItem)
			continue;

		if (CurrItem->isLocked())
			continue;

		TempInvVec.push_back(CurrItem);
		CurrItem->RemoveFromCharacter();
		ch->SyncQuickslot(QUICKSLOT_TYPE_ITEM, i, WORD_MAX);
	}

	if (TempInvVec.empty())
		return;
	
	BYTE sortType = 0;
	std::sort(TempInvVec.begin(), TempInvVec.end(), [&sortType](const LPITEM a, const LPITEM b) {
		// switch (sortType)
		// {
			// case ESortInventoryType::SORT_BY_TYPE:
				return a->GetType() == b->GetType() ? a->GetSubType() < b->GetSubType() : a->GetType() < b->GetType();
		// }
		});

	int ItemIndex = 0;
	for (WORD i = SortStart; i < SortEnd; ++i)
	{
		if (ItemIndex >= TempInvVec.size())
			break;

		TItemPos pos;

		switch(page)
		{
			case 0:
				pos= TItemPos(INVENTORY, i);
				break;
			case 1:
				pos= TItemPos(UPGRADE_INVENTORY, i);
				break;
			case 2:
				pos= TItemPos(BOOK_INVENTORY, i);
				break;
			case 3:
				pos= TItemPos(STONE_INVENTORY, i);
				break;
			case 4:
				pos= TItemPos(CHANGE_INVENTORY, i);
				break;
			case 5:
				pos= TItemPos(COSTUME_INVENTORY, i);
				break;
			case 6:
				pos = TItemPos(CHEST_INVENTORY, i);
				break;
		}

		if (!ch->IsEmptyItemGrid(pos, TempInvVec[ItemIndex]->GetSize()))
			continue;

		TempInvVec[ItemIndex++]->AddToCharacter(ch, pos);
	}
	
	ch->SetNextSortInventoryPulse(thecore_pulse() + PASSES_PER_SEC(60));
}

#ifdef ENABLE_SORT_INVENTORY
ACMD (do_sort_items)
{
	if (ch->IsDead()
		 || ch->IsObserverMode()
/* checked in: ch->IsHack()
		 || ch->GetExchange()
		 || ch->IsOpenSafebox()
		 || ch->IsCubeOpen()
		 || ch->GetShop()
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
*/
	)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "393");
		return;
	}

	if (ch->IsHack() || !ch->CanWarp())
		return;
	
	if (ch->m_pkTimedEvent)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "407");
		event_cancel(&ch->m_pkTimedEvent);

		return;
	}
	
    int lastSortInventoryPulse = ch->GetSortInventoryPulse();
    int currentPulse = thecore_pulse();
   
    if (lastSortInventoryPulse > currentPulse) {
        int deltaInSeconds = ((lastSortInventoryPulse / PASSES_PER_SEC(1)) - (currentPulse / PASSES_PER_SEC(1)));
        int minutes = deltaInSeconds / 60;
        int seconds = (deltaInSeconds - (minutes * 60));
		
		ch->ChatPacket(CHAT_TYPE_INFO, "394 %d", seconds);
        return;
    }
   
    for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
    {
        LPITEM item = ch->GetInventoryItem(i);
       
        if(!item)
            continue;
       
        if(item->isLocked())
            continue;
       
        if(item->GetCount() == g_bItemCountLimit)
            continue;
       
        if (item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
        {
            for (int j = i; j < INVENTORY_MAX_NUM; ++j)
            {
                LPITEM item2 = ch->GetInventoryItem(j);
               
                if(!item2)
                    continue;
               
                if(item2->isLocked())
                    continue;
   
                if (item2->GetVnum() == item->GetVnum())
                {
                    bool bStopSockets = false;
                   
                    for (int k = 0; k < ITEM_SOCKET_MAX_NUM; ++k)
                    {
                        if (item2->GetSocket(k) != item->GetSocket(k))
                        {
                            bStopSockets = true;
                            break;
                        }
                    }
                   
                    if(bStopSockets)
                        continue;
   
                    DWORD bAddCount = MIN(g_bItemCountLimit - item->GetCount(), item2->GetCount());
   
                    item->SetCount(item->GetCount() + bAddCount);
                    item2->SetCount(item2->GetCount() - bAddCount);
                   
                    continue;
                }
            }
        }
    }
   
    ch->SetNextSortInventoryPulse(thecore_pulse() + PASSES_PER_SEC(60));
}
 
ACMD (do_sort_special_storage)
{
	if (ch->IsDead()
		 || ch->IsObserverMode()
/* checked in: ch->IsHack()
		 || ch->GetExchange()
		 || ch->IsOpenSafebox()
		 || ch->IsCubeOpen()
		 || ch->GetShop()
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
*/
	)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "393");
		return;
	}
 
 	if (ch->IsHack() || !ch->CanWarp())
		return;
	
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		return;
	}

	int page = atoi(arg1);

	if(page < 0 || page > 4)
	{
		return;
	}

    int lastSortSpecialStoragePulse = ch->GetSortSpecialStoragePulse();
    int currentPulse = thecore_pulse();
   
    if (lastSortSpecialStoragePulse > currentPulse) {
        int deltaInSeconds = ((lastSortSpecialStoragePulse / PASSES_PER_SEC(1)) - (currentPulse / PASSES_PER_SEC(1)));
        int minutes = deltaInSeconds / 60;
        int seconds = (deltaInSeconds - (minutes * 60));
 
        ch->ChatPacket(CHAT_TYPE_INFO, "394 %d", seconds);
        return;
    }
   
   

	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM item;
           
		switch(page)
		{
			case 1:
				item = ch->GetUpgradeInventoryItem(i);
				break;
			case 2:
				item = ch->GetBookInventoryItem(i);
				break;
			case 3:
				item = ch->GetStoneInventoryItem(i);
				break;
			case 4:
				item = ch->GetChangeInventoryItem(i);
				break;
			default:
				item = ch->GetUpgradeInventoryItem(i);
				break;
		}
           
		if(!item)
			continue;
           
		if(item->isLocked())
			continue;
           
		if(item->GetCount() == g_bItemCountLimit)
			continue;
           
		if (item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
		{
			for (int j = i; j < SPECIAL_INVENTORY_MAX_NUM; ++j)
			{
                   
				LPITEM item2;
                   
				switch(page)
				{
					case 1:
						item2 = ch->GetUpgradeInventoryItem(j);
						break;
					case 2:
						item2 = ch->GetBookInventoryItem(j);
						break;
					case 3:
						item2 = ch->GetStoneInventoryItem(j);
						break;
					case 4:
						item2 = ch->GetChangeInventoryItem(j);
						break;
					default:
						item2 = ch->GetUpgradeInventoryItem(j);
						break;
				}
           
				if(!item2)
					continue;
                   
				if(item2->isLocked())
					continue;
       
				if (item2->GetVnum() == item->GetVnum())
				{
					bool bStopSockets = false;
                       
					for (int k = 0; k < ITEM_SOCKET_MAX_NUM; ++k)
					{
						if (item2->GetSocket(k) != item->GetSocket(k))
						{
							bStopSockets = true;
							break;
						}
					}
                       
					if(bStopSockets)
						continue;
       
					DWORD bAddCount = MIN(g_bItemCountLimit - item->GetCount(), item2->GetCount());
       
					item->SetCount(item->GetCount() + bAddCount);
					item2->SetCount(item2->GetCount() - bAddCount);
                       
					continue;
				}
			}
		}
	}
    
    ch->SetNextSortSpecialStoragePulse(thecore_pulse() + PASSES_PER_SEC(60));
}
#endif

#ifdef ENABLE_MANAGER_BANK_SYSTEM	
	#include "bank.h"
#endif
#ifdef ENABLE_MANAGER_BANK_SYSTEM	
ACMD(do_bank_create)
{
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	BankManager::instance().Send_Create(ch, arg1, arg2);
}
ACMD(do_bank_open)
{
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	BankManager::instance().Send_Open(ch, arg1, arg2);
}
ACMD(do_bank_add)
{
	const char *r;
	char arg1[256], arg2[256], arg3[256];
	r = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(r, arg3, sizeof(arg3));
	BankManager::instance().Send_AddMoney(ch, arg1, arg2, arg3);
}
ACMD(do_bank_withdraw)
{
	const char *r;
	char arg1[256], arg2[256], arg3[256];
	r = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(r, arg3, sizeof(arg3));
	BankManager::instance().Send_WithdrawMoney(ch, arg1, arg2, arg3);
}
ACMD(do_bank_send)
{
	char arg1[256], arg2[256], arg3[256], arg4[256];
	two_arguments (two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3), arg4, sizeof(arg4));
	BankManager::instance().Send_Money(ch, arg1, arg2, arg3, arg4);
}
ACMD(do_bank_changepw)
{
	char arg1[256], arg2[256], arg3[256], arg4[256];
	two_arguments (two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3), arg4, sizeof(arg4));
	BankManager::instance().Send_ChangePassword(ch, arg1, arg2, arg3, arg4);
}
#endif
ACMD(do_destroy_item){
	if (!ch)
		return;
	
	char arg1[20];
	one_argument(argument, arg1, sizeof(arg1));
	
	DWORD Slot;
	str_to_number(Slot, arg1);
	
	LPITEM item = ch->GetInventoryItem(Slot);
	if (item){
		
		if (CanDestroyItem(item->GetVnum())) {
			ch->ChatPacket(CHAT_TYPE_COMMAND, "sCantDestroyItemDialog");
			return;
		}

#ifdef ENABLE_BATTLE_PASS
		CHARACTER_MANAGER::Instance().DoMission(ch, MISSION_DESTROY_ITEM, item->GetCount(), item->GetVnum());
#endif

		LogManager::instance().ItemLog(ch, item, "DESTROY_ITEM", item->GetName());
		item->RemoveFromCharacter();
	}
}
#ifdef ENABLE_PLAYERS_ONLINE
ACMD(do_players_online){
	if (!ch)
		return;
	
	int iTotal;
	int * paiEmpireUserCount;
	int iLocal;

	DESC_MANAGER::instance().GetUserCount(iTotal, &paiEmpireUserCount, iLocal);
	
	ch->ChatPacket(CHAT_TYPE_COMMAND, "SetPlayersOnline %d", iTotal);
}
#endif

#ifdef ENABLE_SORT_INVEN
struct s_sort {
	bool operator() (LPITEM src, LPITEM dst) { return (src->GetVnum()<dst->GetVnum()); }
} _sort;

ACMD(do_sort_inventory)
{
	if (!ch)
		return;
	std::vector<LPITEM> collectItems;
	std::vector<WORD> oldCells;
	// std::vector<DWORD> oldTime;
	int totalSize = 0;
	#ifdef UNLOCK_INVENTORY_WORLDARD
	int SLOTS = ch->GetInventorySize();
	#else
	int SLOTS = INVENTORY_MAX_NUM;
	#endif
	for (WORD i = 0; i < SLOTS; ++i)
	{
		LPITEM item = ch->GetInventoryItem(i);
		if (item)
		{
			totalSize += item->GetSize();
			oldCells.push_back(item->GetCell());
			collectItems.push_back(item);
			// if (item->GetVnum() == 71124)
				// oldTime.push_back(item->GetValue(0));
				
		}
	}
	if (totalSize - 3 >= SLOTS)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "395");
		return;
	}

	std::vector<LPITEM>::iterator it = collectItems.begin(), end = collectItems.end();
	for (; it != end; ++it)
		((LPITEM)*it)->RemoveFromCharacter();

	std::sort(collectItems.begin(), collectItems.end(), _sort);

	std::vector<LPITEM>::iterator it1 = collectItems.begin(), end1 = collectItems.end();
	for (; it1 != end1; ++it1)
	{
		// new here
		TItemTable * p = ITEM_MANAGER::instance().GetTable(((LPITEM)*it1)->GetVnum());
		if (p && p->dwFlags & ITEM_FLAG_STACKABLE && p->bType != ITEM_BLEND){
			ch->AutoGiveItem(((LPITEM)*it1)->GetVnum(), ((LPITEM)*it1)->GetCount(), -1, false); // create new item for stackable items
		}
		else{
			ch->AutoGiveItem((LPITEM)*it1); // copy orginal items
			// if (((LPITEM)*it1)->GetVnum() == 71124)
				// ((LPITEM)*it1)->SetForceAttribute(0,1,it1);
		}
		
		// WORD cell = ch->GetEmptyInventory(((LPITEM)*it1)->GetSize());
		// ((LPITEM)*it1)->AddToCharacter(ch, TItemPos(INVENTORY, cell), false);
	}
}
#endif

//search offline
ACMD(do_search_shop)
{
	const char *var;
	char arg1[255],arg2[255],arg3[255],arg4[255];

	var = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	two_arguments(var, arg3, sizeof(arg3), arg4, sizeof(arg4));

	if (0 == arg1[0] || 0 == arg2[0] || 0 == arg3[0] || 0 == arg4[0])
		return;

	int id_item = atoi(arg1);
	int vnum_item = atoi(arg2);
	int gold = atoi(arg3);
	int check_all = atoi(arg4);

	char szQuery[1024];
	char szQuery2[1024];
	char szQuery3[1024];
	char InsertSql[1024];


	snprintf(szQuery3, sizeof(szQuery3),"SELECT * FROM player.offline_shop_search WHERE iid=%d",ch->GetPlayerID());
	std::unique_ptr<SQLMsg> pMsg3(DBManager::Instance().DirectQuery(szQuery3));
	if (pMsg3->Get()->uiNumRows > 0)
	{ 
		while (MYSQL_ROW row_vid = mysql_fetch_row(pMsg3->Get()->pSQLResult))
		{
			DWORD ID_Vid = 0;
			const char * Id_Vid_0 = row_vid[2];
			str_to_number(ID_Vid, Id_Vid_0);

			LPEVENT pkEvent = CTargetManager::instance().GetTargetEvent(ch->GetPlayerID(), ID_Vid, "_WorldArd_");
			if (pkEvent){
				CTargetManager::instance().DeleteTarget(ch->GetPlayerID(), ID_Vid, "_WorldArd_");
			}
		}
		std::unique_ptr<SQLMsg> pMsgUpdate(DBManager::instance().DirectQuery("DELETE FROM player.offline_shop_search WHERE iid = %d", ch->GetPlayerID()));
	}

	if (check_all == 0){snprintf(szQuery, sizeof(szQuery),"SELECT shop_id,price FROM player.player_shop_items WHERE vnum=%d",vnum_item);}
	else if (check_all == 1){snprintf(szQuery, sizeof(szQuery),"SELECT shop_id,price FROM player.player_shop_items WHERE id=%d",id_item);}

	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));
	if (pMsg->Get()->uiNumRows > 0) 
	{	
		DWORD owner_id = 0;
		int gold_id = 0;
		DWORD position_x = 0;
		DWORD position_y = 0;
		for (int i = 0; i < mysql_num_rows(pMsg->Get()->pSQLResult); ++i){
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			int w_0 = 0;
			str_to_number(owner_id, row[w_0++]);
			str_to_number(gold_id, row[w_0++]);

			if (gold > 0){
				if (gold_id <= gold){
					snprintf(szQuery2, sizeof(szQuery2),"SELECT x,y FROM player.player_shop WHERE id=%d and channel = %d",owner_id,g_bChannel);
					std::unique_ptr<SQLMsg> pMsg2(DBManager::Instance().DirectQuery(szQuery2));
					if (pMsg2->Get()->uiNumRows > 0){
						MYSQL_ROW row2 = mysql_fetch_row(pMsg2->Get()->pSQLResult);
						int w_1 = 0;
						str_to_number(position_x, row2[w_1++]);
						str_to_number(position_y, row2[w_1++]);

						CTargetManager::instance().CreateTarget(ch->GetPlayerID(),
						owner_id,
						"_WorldArd_",
						TARGET_TYPE_SHOP,
						position_x,position_y,
						ch->GetMapIndex());
													
						snprintf(InsertSql, sizeof(InsertSql), "INSERT INTO player.offline_shop_search (iid,vid) VALUES(%d,%d)", ch->GetPlayerID() , owner_id);
						std::unique_ptr<SQLMsg> pmsg4(DBManager::instance().DirectQuery(InsertSql));
					}
				}
			}
			else{
				snprintf(szQuery2, sizeof(szQuery2),"SELECT x,y FROM player.player_shop WHERE id=%d and channel=%d",owner_id,g_bChannel);
				std::unique_ptr<SQLMsg> pMsg2(DBManager::Instance().DirectQuery(szQuery2));
				if (pMsg2->Get()->uiNumRows > 0){
					MYSQL_ROW row2 = mysql_fetch_row(pMsg2->Get()->pSQLResult);
					int w_1 = 0;
					str_to_number(position_x, row2[w_1++]);
					str_to_number(position_y, row2[w_1++]);

					CTargetManager::instance().CreateTarget(ch->GetPlayerID(),
					owner_id,
					"_WorldArd_",
					TARGET_TYPE_SHOP,
					position_x,position_y,
					ch->GetMapIndex());								
					snprintf(InsertSql, sizeof(InsertSql), "INSERT INTO player.offline_shop_search (iid,vid) VALUES(%d,%d)", ch->GetPlayerID() , owner_id);
					std::unique_ptr<SQLMsg> pmsg4(DBManager::instance().DirectQuery(InsertSql));
				}
			}

		}
	}
	else{
		ch->ChatPacket(CHAT_TYPE_INFO, "[Search]Items no encontrado en tienda.");
		return;
	}
}

ACMD(do_search_item)
{
	char arg1[255];
	char arg2[255];
	two_arguments (argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (0 == arg1[0] || 0 == arg2[0])
		return;

	int item_vnum = atoi(arg1);
	int Item_Gold = atoi(arg2);

	if (item_vnum == 0){
		ch->ChatPacket(CHAT_TYPE_INFO,"[Shop Search] No ha colocado ninguno item.");
		return;
	}
	if (Item_Gold < 0){
		ch->ChatPacket(CHAT_TYPE_INFO,"Seta¨Ï¡©i o valoare mai mare de 0 Yang.");
		return;
	}
	if (Item_Gold > 2000000000){
		ch->ChatPacket(CHAT_TYPE_INFO,"Seta¨Ï¡©i o valoare mai mica de 2kkk Yang.");
		return;
	}

	int check = 0;
	int check_0 = 0;

    char szQuery[1024];
    char szQuery1[1024];
	snprintf(szQuery, sizeof(szQuery),"SELECT "
	"id,shop_id,vnum,price, " 
	"attrtype0, attrvalue0, "
	"attrtype1, attrvalue1, "
	"attrtype2, attrvalue2, "
	"attrtype3, attrvalue3, "
	"attrtype4, attrvalue4, "
	"attrtype5, attrvalue5, "
	"attrtype6, attrvalue6 "
	"FROM player.player_shop_items WHERE vnum=%d",item_vnum);
	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));

	if (pMsg->Get()->uiNumRows > 0) 
	{

		DWORD owner_id;
		int gold;
		TPlayerItem item;
		for (int i = 0; i < mysql_num_rows(pMsg->Get()->pSQLResult); ++i)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			int w = 0;
			str_to_number(item.id, row[w++]);
			str_to_number(owner_id, row[w++]);
			str_to_number(item.vnum, row[w++]);
			str_to_number(gold,row[w++]);

			for (BYTE n = 0; n < 7; n++)
			{
				str_to_number(item.aAttr[n].bType, row[w++]);
				str_to_number(item.aAttr[n].sValue, row[w++]);
			}


			snprintf(szQuery1, sizeof(szQuery1),"SELECT * FROM player.player_shop WHERE map_index=%ld and id = %d and channel = %d",ch->GetMapIndex(),owner_id,g_bChannel);
			std::unique_ptr<SQLMsg> pMsg1(DBManager::Instance().DirectQuery(szQuery1));
			if (pMsg1->Get()->uiNumRows > 0)
			{

				if (Item_Gold > 0){
					if (gold <= Item_Gold){
						ch->ChatPacket(CHAT_TYPE_COMMAND, "PrivateSearch_ItemsSearch %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
							item.id,item.vnum,gold,
							item.aAttr[0].bType,item.aAttr[0].sValue,
							item.aAttr[1].bType,item.aAttr[1].sValue,
							item.aAttr[2].bType,item.aAttr[2].sValue,
							item.aAttr[3].bType,item.aAttr[3].sValue,
							item.aAttr[4].bType,item.aAttr[4].sValue,
							item.aAttr[5].bType,item.aAttr[5].sValue,
							item.aAttr[6].bType,item.aAttr[6].sValue);
							check++;
					}
				}else{
						ch->ChatPacket(CHAT_TYPE_COMMAND, "PrivateSearch_ItemsSearch %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
							item.id,item.vnum,gold,
							item.aAttr[0].bType,item.aAttr[0].sValue,
							item.aAttr[1].bType,item.aAttr[1].sValue,
							item.aAttr[2].bType,item.aAttr[2].sValue,
							item.aAttr[3].bType,item.aAttr[3].sValue,
							item.aAttr[4].bType,item.aAttr[4].sValue,
							item.aAttr[5].bType,item.aAttr[5].sValue,
							item.aAttr[6].bType,item.aAttr[6].sValue);
							check++;

				}
			}
		}
	}
	else{
		ch->ChatPacket(CHAT_TYPE_INFO,"[Shop Search] No se encuentra ningun item con ese nombre.");
		return;
	}

	if (Item_Gold > 0){
		if(check == 0){
			ch->ChatPacket(CHAT_TYPE_INFO,"[Shop Search] No se encuentra ningun item con ese valor.");
			return;
		}
	}

	ch->ChatPacket(CHAT_TYPE_COMMAND, "PrivateSearch_RefreshItems");
}
//search offline end

#ifdef UNLOCK_INVENTORY_WORLDARD
ACMD(do_reload_inventory){
	if (!ch)
		return;

	for (int i = 0; i < UNLOCK_INVENTORY_MAX; ++i)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ManagerInventoryUnlock Hide|%d|%d", ch->GetUnlockSlotsW(i),i);
	}
}

ACMD(do_unlock_inventory){
	if (!ch)
		return;
	
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		return;
	}

	DWORD type_inventory;
	str_to_number(type_inventory, arg1);

	DWORD inv = ch->GetUnlockSlotsW(type_inventory);
	int cant;
	if (inv > 0){ 
		cant = (inv+1)*2;
		if (ch->CountSpecifyItem(72320) >= cant){
			ch->RemoveSpecifyItem(72320,cant);
			ch->ChatPacket(CHAT_TYPE_COMMAND, "ManagerInventoryUnlock Hide|%d|%d", inv+1, type_inventory);
			ch->SetUnlockSlotsW(inv+1,type_inventory);
		}else{
			ch->ChatPacket(CHAT_TYPE_INFO, "396 %d", cant);
		}
	}else{
		cant = 2;
		if (ch->CountSpecifyItem(72320) >= cant){
			ch->RemoveSpecifyItem(72320,cant);
			ch->ChatPacket(CHAT_TYPE_COMMAND, "ManagerInventoryUnlock Hide|%d|%d", 1, type_inventory);
			ch->SetUnlockSlotsW(1,type_inventory);
		}else{
			ch->ChatPacket(CHAT_TYPE_INFO, "396 %d", cant);
		}
	}

}
#endif

ACMD(do_user_horse_ride)
{
	if (ch->IsObserverMode())
		return;

	if (ch->IsDead() || ch->IsStun())
		return;

	if (ch->IsHorseRiding() == false)
	{
		// ¸»ÀÌ ¾Æ´Ñ ´Ù¸¥Å»°ÍÀ» Å¸°íÀÖ´Ù.
		if (ch->GetMountVnum())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "397");
			return;
		}

		if (ch->GetHorse() == NULL)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "398");
			return;
		}

		ch->StartRiding();
	}
	else
	{
		ch->StopRiding();
	}
}

ACMD(do_user_horse_back)
{
	if (ch->GetHorse() != NULL)
	{
		ch->HorseSummon(false);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		LPITEM pkCostumeMount = ch->GetWear(WEAR_COSTUME_MOUNT);
		if (pkCostumeMount)
			ch->UnequipItem(pkCostumeMount);
#endif
		ch->ChatPacket(CHAT_TYPE_INFO, "399");
	}
	else if (ch->IsHorseRiding() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "400");
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "398");
	}
}

ACMD(do_user_horse_feed)
{

	if (ch->GetHorse() == NULL)
	{
		if (ch->IsHorseRiding() == false)
			ch->ChatPacket(CHAT_TYPE_INFO, "398");
		else
			ch->ChatPacket(CHAT_TYPE_INFO, "401");
		return;
	}

	DWORD dwFood = ch->GetHorseGrade() + 50054 - 1;

	if (ch->CountSpecifyItem(dwFood) > 0)
	{
		ch->RemoveSpecifyItem(dwFood, 1);
		ch->FeedHorse();
		ch->ChatPacket(CHAT_TYPE_INFO, "402 %s%s",
				ITEM_MANAGER::instance().GetTable(dwFood)->szLocaleName,
				"");
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "403 %s", ITEM_MANAGER::instance().GetTable(dwFood)->szLocaleName);
	}
}

#define MAX_REASON_LEN		128

EVENTINFO(TimedEventInfo)
{
	DynamicCharacterPtr ch;
	int		subcmd;
	int         	left_second;
	char		szReason[MAX_REASON_LEN];
#ifdef ENABLE_CHANNEL_CHANGE
	short		channel;
#endif

	TimedEventInfo()
	: ch()
	, subcmd( 0 )
	, left_second( 0 )
#ifdef ENABLE_CHANNEL_CHANGE
	,channel(0)
#endif
	{
		::memset( szReason, 0, MAX_REASON_LEN );
	}
};

struct SendDisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetCharacter())
		{
			if (d->GetCharacter()->GetGMLevel() == GM_PLAYER)
				d->GetCharacter()->ChatPacket(CHAT_TYPE_COMMAND, "quit Shutdown(SendDisconnectFunc)");
		}
	}
};

struct DisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetType() == DESC_TYPE_CONNECTOR)
			return;

		if (d->IsPhase(PHASE_P2P))
			return;

		if (d->GetCharacter())
			d->GetCharacter()->Disconnect("Shutdown(DisconnectFunc)");

		d->SetPhase(PHASE_CLOSE);
	}
};

EVENTINFO(shutdown_event_data)
{
	int seconds;

	shutdown_event_data()
	: seconds( 0 )
	{
	}
};

EVENTFUNC(shutdown_event)
{
	shutdown_event_data* info = dynamic_cast<shutdown_event_data*>( event->info );

	if ( info == NULL )
	{
		sys_err( "shutdown_event> <Factor> Null pointer" );
		return 0;
	}

	int * pSec = & (info->seconds);

	if (*pSec < 0)
	{
		sys_log(0, "shutdown_event sec %d", *pSec);

		if (--*pSec == -10)
		{
			const DESC_MANAGER::DESC_SET & c_set_desc = DESC_MANAGER::instance().GetClientSet();
			std::for_each(c_set_desc.begin(), c_set_desc.end(), DisconnectFunc());
			return passes_per_sec;
		}
		else if (*pSec < -10)
			return 0;

		return passes_per_sec;
	}
	else if (*pSec == 0)
	{
		const DESC_MANAGER::DESC_SET & c_set_desc = DESC_MANAGER::instance().GetClientSet();
		std::for_each(c_set_desc.begin(), c_set_desc.end(), SendDisconnectFunc());
		g_bNoMoreClient = true;
		--*pSec;
		return passes_per_sec;
	}
	else
	{
		char buf[64];
		snprintf(buf, sizeof(buf), "404 %d", *pSec);
		SendNotice(buf);

		--*pSec;
		return passes_per_sec;
	}
}

void Shutdown(int iSec)
{
	if (g_bNoMoreClient)
	{
		thecore_shutdown();
		return;
	}

	CWarMapManager::instance().OnShutdown();

	char buf[64];
	snprintf(buf, sizeof(buf), "405 %d", iSec);

	SendNotice(buf);

	shutdown_event_data* info = AllocEventInfo<shutdown_event_data>();
	info->seconds = iSec;

	event_create(shutdown_event, info, 1);
}

#ifdef ENABLE_MAINTENANCE_SYSTEM
ACMD(do_maintenance_text)
{
	char arg1[256];
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));	
	
	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_NOTICE, "<Syntax> The arguments available for this command are:");
		ch->ChatPacket(CHAT_TYPE_NOTICE, "<Syntax> /m_text disable");
		ch->ChatPacket(CHAT_TYPE_NOTICE, "<Syntax> /m_text enable <text>");
		return;
	}

	if (*arg1 && !strcmp(arg1, "disable"))
	{			
		MaintenanceManager::instance().Send_Text(ch, "rmf");
	}

	else if (*arg1 && !strcmp(arg1, "enable"))		
	{			
		const char* sReason = one_argument(argument, arg2, sizeof(arg2));
		MaintenanceManager::instance().Send_Text(ch, sReason);
	}	
}


ACMD(do_maintenance)
{
	char arg1[256];
	char arg2[256];
	
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	
	if (*arg1 && !strcmp(arg1, "force_stop"))
	{			
		MaintenanceManager::instance().Send_DisableSecurity(ch);
	}
	else
	{			
		long time_maintenance = parse_time_str(arg1);
		long duration_maintenance = parse_time_str(arg2);

		MaintenanceManager::instance().Send_ActiveMaintenance(ch, time_maintenance, duration_maintenance);
	}
}	
#endif

ACMD(do_shutdown)
{
	if (NULL == ch)
	{
		sys_err("Accept shutdown command from %s.", ch->GetName());
	}
	TPacketGGShutdown p;
	p.bHeader = HEADER_GG_SHUTDOWN;
	P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGShutdown));

	Shutdown(10);
}

EVENTFUNC(timed_event)
{
	TimedEventInfo * info = dynamic_cast<TimedEventInfo *>( event->info );

	if ( info == NULL )
	{
		sys_err( "timed_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;
	if (ch == NULL) { // <Factor>
		return 0;
	}
	LPDESC d = ch->GetDesc();

	if (info->left_second <= 0)
	{
		ch->m_pkTimedEvent = NULL;

		switch (info->subcmd)
		{
			case SCMD_LOGOUT:
			case SCMD_QUIT:
			case SCMD_PHASE_SELECT:
			case SCMD_CHANGE_INVENTORY:
				{
					TPacketNeedLoginLogInfo acc_info;
					acc_info.dwPlayerID = ch->GetDesc()->GetAccountTable().id;

					db_clientdesc->DBPacket( HEADER_GD_VALID_LOGOUT, 0, &acc_info, sizeof(acc_info) );

					LogManager::instance().DetailLoginLog( false, ch );
				}
				break;
		}

		switch (info->subcmd)
		{
			case SCMD_LOGOUT:
				if (d)
					d->SetPhase(PHASE_CLOSE);
				break;

			case SCMD_QUIT:
				ch->ChatPacket(CHAT_TYPE_COMMAND, "quit");
				break;

			case SCMD_PHASE_SELECT:
				{
					ch->Disconnect("timed_event - SCMD_PHASE_SELECT");

					if (d)
					{
						d->SetPhase(PHASE_SELECT);
					}
				}
				break;
#ifdef ENABLE_CHANNEL_CHANGE
			case SCMD_CHANGE_CHANNEL:
				ch->MoveChannel(info->channel);
				break;
#endif
			case SCMD_CHANGE_INVENTORY:
				ch->Disconnect("timed_event - SCMD_CHANGE_INVENTORY");
				if (d)
					d->SetPhase(PHASE_SELECT);
				break;
		}

		return 0;
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "406 %d", info->left_second);
		--info->left_second;
	}

	return PASSES_PER_SEC(1);
}

ACMD(do_cmd)
{
	ch->ForgetMyAttacker(false) ;

	/* RECALL_DELAY
	   if (ch->m_pkRecallEvent != NULL)
	   {
	   ch->ChatPacket(CHAT_TYPE_INFO, "407");
	   event_cancel(&ch->m_pkRecallEvent);
	   return;
	   }
	// END_OF_RECALL_DELAY */

	if (ch->m_pkTimedEvent)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "407");
		event_cancel(&ch->m_pkTimedEvent);
		return;
	}
	
#ifdef ENABLE_CHANNEL_CHANGE
	short channel = 0;
	if(subcmd == SCMD_CHANGE_CHANNEL)
	{
		char arg1[256];
		one_argument(argument, arg1, sizeof(arg1));
		if (!*arg1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "554");
			return;
		}
		str_to_number(channel, arg1);
		if (channel < 1 || channel > 4) {
			ch->ChatPacket(CHAT_TYPE_INFO, "555");
			return;
		}
		if (channel == g_bChannel) {
			ch->ChatPacket(CHAT_TYPE_INFO, "556 %d", g_bChannel);
			return;
		}
		if (g_bChannel == 99) {
			ch->ChatPacket(CHAT_TYPE_INFO, "557");
			return;
		}
		if (ch->GetDungeon()) {
			ch->ChatPacket(CHAT_TYPE_INFO, "558");
			return;
		}
		//check mapindex
		switch(ch->GetMapIndex())
		{
			case 113://ox
			case 81:
			case 110:
			case 111:
			case 114:
			case 118:
			case 119:
			case 120:
			case 121:
			case 122:
			case 123:
			case 124:
			case 125:
			case 126:
			case 127:
			case 128:
			case 181:
			case 182:
			case 183:
			case 24:
			case 44:
			case 211:
			case 90:
			case 91:
			case 92:
				ch->ChatPacket(CHAT_TYPE_INFO, "999");
				return;
			break;
		}
	}
#endif

	switch (subcmd)
	{
		case SCMD_LOGOUT:
			ch->ChatPacket(CHAT_TYPE_INFO, "408");
			break;

		case SCMD_QUIT:
			ch->ChatPacket(CHAT_TYPE_INFO, "409");
			break;

		case SCMD_PHASE_SELECT:
			ch->ChatPacket(CHAT_TYPE_INFO, "410");
			break;

		case SCMD_CHANGE_INVENTORY:
			ch->ChatPacket(CHAT_TYPE_INFO, "410");
			break;
	}

	int nExitLimitTime = 3;

	if (ch->IsHack(false, true, nExitLimitTime) &&
		false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()) &&
	   	(!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
	{
		return;
	}

	switch (subcmd)
	{
		case SCMD_LOGOUT:
		case SCMD_QUIT:
		case SCMD_PHASE_SELECT:
#ifdef ENABLE_CHANNEL_CHANGE
		case SCMD_CHANGE_CHANNEL:
#endif
		case SCMD_CHANGE_INVENTORY:
			{
				TimedEventInfo* info = AllocEventInfo<TimedEventInfo>();
				{
					if (ch->IsPosition(POS_FIGHTING))
						info->left_second = 0;
					else
						info->left_second = 0;
				}

				info->ch		= ch;
				info->subcmd		= subcmd;
				strlcpy(info->szReason, argument, sizeof(info->szReason));
#ifdef ENABLE_CHANNEL_CHANGE
				info->channel		= channel;
#endif
				ch->m_pkTimedEvent	= event_create(timed_event, info, 1);
			}
			break;
	}
}

ACMD(do_mount)
{
	/*
	   char			arg1[256];
	   struct action_mount_param	param;

	// ÀÌ¹Ì Å¸°í ÀÖÀ¸¸é
	if (ch->GetMountingChr())
	{
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
	return;

	param.x		= atoi(arg1);
	param.y		= atoi(arg2);
	param.vid	= ch->GetMountingChr()->GetVID();
	param.is_unmount = true;

	float distance = DISTANCE_SQRT(param.x - (DWORD) ch->GetX(), param.y - (DWORD) ch->GetY());

	if (distance > 600.0f)
	{
	ch->ChatPacket(CHAT_TYPE_INFO, "411");
	return;
	}

	action_enqueue(ch, ACTION_TYPE_MOUNT, &param, 0.0f, true);
	return;
	}

	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	return;

	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(atoi(arg1));

	if (!tch->IsNPC() || !tch->IsMountable())
	{
	ch->ChatPacket(CHAT_TYPE_INFO, "412");
	return;
	}

	float distance = DISTANCE_SQRT(tch->GetX() - ch->GetX(), tch->GetY() - ch->GetY());

	if (distance > 600.0f)
	{
	ch->ChatPacket(CHAT_TYPE_INFO, "413");
	return;
	}

	param.vid		= tch->GetVID();
	param.is_unmount	= false;

	action_enqueue(ch, ACTION_TYPE_MOUNT, &param, 0.0f, true);
	 */
}
#ifdef ENABLE_BIOLOG_SYSTEM
ACMD(do_biolog)
{
	
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	
	if (!*arg1)
	{
		BiologManager::instance().SendButton(ch);
		return;
	}	
	
	if (!strcmp(arg1, "92_reward_1")) {
		BiologManager::instance().SelectBonusType(ch, "92_reward_1"); return; }		
	if (!strcmp(arg1, "92_reward_2")) {
		BiologManager::instance().SelectBonusType(ch, "92_reward_2"); return; }				
	if (!strcmp(arg1, "92_reward_3")) {
		BiologManager::instance().SelectBonusType(ch, "92_reward_3"); return; }		
	if (!strcmp(arg1, "94_reward_1")) {
		BiologManager::instance().SelectBonusType(ch, "94_reward_1"); return; }			
	if (!strcmp(arg1, "94_reward_2")) {
		BiologManager::instance().SelectBonusType(ch, "94_reward_2"); return; }					
	if (!strcmp(arg1, "94_reward_3")) {
		BiologManager::instance().SelectBonusType(ch, "94_reward_3"); return; }			
}	
#endif
ACMD(do_fishing)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	ch->SetRotation(atof(arg1));
	ch->fishing();
}

ACMD(do_console)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ConsoleEnable");
}

ACMD(do_restart)
{
#ifdef __BL_BATTLE_ROYALE__
	if (ch->IsBattleRoyale())
	{
		auto& brMgr = BattleRoyale::Instance();
		if (!ch->IsDead() && !brMgr.BattleCore_IsWinner(ch))
		{
			ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");
			return;
		}

		if (subcmd == SCMD_RESTART_TOWN)
			brMgr.BattleCore_Leave(ch);

		return;
	}
#endif

#ifdef ENABLE_SUNG_MAHI_TOWER
	LPDUNGEON dungeonInstance = ch->GetDungeon();
	if (dungeonInstance && dungeonInstance->GetFlag("isSungMahiDungeon") > 0)
	{
		ch->WarpSet((9216 * 100), (6144*100));
		return;
	}
#endif
	if (false == ch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");
		ch->StartRecoveryEvent();
		return;
	}

	if (NULL == ch->m_pkDeadEvent)
		return;

	int iTimeToDead = (event_time(ch->m_pkDeadEvent) / passes_per_sec);
#if defined(WJ_COMBAT_ZONE)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(ch->GetMapIndex()))
	{
		CCombatZoneManager::Instance().OnRestart(ch, subcmd);
		return;
	}	
#endif
	if (subcmd != SCMD_RESTART_TOWN && (!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
	{
		if (!test_server)
		{
			if (ch->IsHack())
			{
				//¼ºÁö ¸ÊÀÏ°æ¿ì¿¡´Â Ã¼Å© ÇÏÁö ¾Ê´Â´Ù.
				if (false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "414 %d", iTimeToDead - (180 - g_nPortalLimitTime));
					return;
				}
			}
#define eFRS_HERESEC	170
			if (iTimeToDead > eFRS_HERESEC)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "414 %d", iTimeToDead - eFRS_HERESEC);
				return;
			}
		}
	}

	//PREVENT_HACK
	//DESC : Ã¢°í, ±³È¯ Ã¢ ÈÄ Æ÷Å»À» »ç¿ëÇÏ´Â ¹ö±×¿¡ ÀÌ¿ëµÉ¼ö ÀÖ¾î¼­
	//		ÄðÅ¸ÀÓÀ» Ãß°¡
	if (subcmd == SCMD_RESTART_TOWN)
	{

		if (ch->GetDungeon())
		{
			ch->ChatPacket(CHAT_TYPE_INFO,"Esta funcion esta desactivada para las mazmorras.");
			return;
		}

		if (ch->IsHack())
		{
			//±æµå¸Ê, ¼ºÁö¸Ê¿¡¼­´Â Ã¼Å© ÇÏÁö ¾Ê´Â´Ù.
			if ((!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG) ||
			   	false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "414 %d", iTimeToDead - (180 - g_nPortalLimitTime));
				return;
			}
		}

#define eFRS_TOWNSEC	173
		if (iTimeToDead > eFRS_TOWNSEC)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "415 %d", iTimeToDead - eFRS_TOWNSEC);
			return;
		}
	}
	//END_PREVENT_HACK

	ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");

	ch->GetDesc()->SetPhase(PHASE_GAME);
	ch->SetPosition(POS_STANDING);
	ch->StartRecoveryEvent();

	//FORKED_LOAD
	//DESC: »ï°Å¸® ÀüÅõ½Ã ºÎÈ°À» ÇÒ°æ¿ì ¸ÊÀÇ ÀÔ±¸°¡ ¾Æ´Ñ »ï°Å¸® ÀüÅõÀÇ ½ÃÀÛÁöÁ¡À¸·Î ÀÌµ¿ÇÏ°Ô µÈ´Ù.
	if (1 == quest::CQuestManager::instance().GetEventFlag("threeway_war"))
	{
		if (subcmd == SCMD_RESTART_TOWN || subcmd == SCMD_RESTART_HERE)
		{
			if (true == CThreeWayWar::instance().IsThreeWayWarMapIndex(ch->GetMapIndex()) &&
					false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));

				ch->ReviveInvisible(5);
				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());

				return;
			}

			//¼ºÁö
			if (true == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				if (CThreeWayWar::instance().GetReviveTokenForPlayer(ch->GetPlayerID()) <= 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "416");
					ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
				}
				else
				{
					ch->Show(ch->GetMapIndex(), GetSungziStartX(ch->GetEmpire()), GetSungziStartY(ch->GetEmpire()));
				}

				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
				ch->ReviveInvisible(5);

				return;
			}
		}
	}
	//END_FORKED_LOAD

	if (ch->GetDungeon())
		ch->GetDungeon()->UseRevive(ch);

	if (ch->GetWarMap() && !ch->IsObserverMode())
	{
		CWarMap * pMap = ch->GetWarMap();
		DWORD dwGuildOpponent = pMap ? pMap->GetGuildOpponent(ch) : 0;

		if (dwGuildOpponent)
		{
			switch (subcmd)
			{
				case SCMD_RESTART_TOWN:
					sys_log(0, "do_restart: restart town");
					PIXEL_POSITION pos;

					if (CWarMapManager::instance().GetStartPosition(ch->GetMapIndex(), ch->GetGuild()->GetID() < dwGuildOpponent ? 0 : 1, pos))
						ch->Show(ch->GetMapIndex(), pos.x, pos.y);
					else
						ch->ExitToSavedLocation();

					ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
					ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
					ch->ReviveInvisible(5);
					break;

				case SCMD_RESTART_HERE:
					sys_log(0, "do_restart: restart here");
					ch->RestartAtSamePos();
					//ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY());
					ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
					ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
					ch->ReviveInvisible(5);
					break;
			}

			return;
		}
	}

#ifdef ENABLE_QUEEN_NETHIS
	if (SnakeLair::CSnk::instance().IsSnakeMap(ch->GetMapIndex()))
	{
		switch (subcmd)
		{
			case SCMD_RESTART_TOWN:
				sys_log(0, "do_restart: restart town");
				//PIXEL_POSITION pos;

				ch->RestartAtSamePos();

				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
				ch->ReviveInvisible(3);
				break;

			case SCMD_RESTART_HERE:
				sys_log(0, "do_restart: restart here");
				ch->RestartAtSamePos();
				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
				ch->ReviveInvisible(3);
				break;

			default:
				break;
		}

		return;
	}
#endif

	switch (subcmd)
	{
		case SCMD_RESTART_TOWN:
			sys_log(0, "do_restart: restart town");
			PIXEL_POSITION pos;

			if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire(ch->GetMapIndex(), ch->GetEmpire(), pos))
				ch->WarpSet(pos.x, pos.y);
			else
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
			ch->PointChange(POINT_HP, 50 - ch->GetHP());
			ch->DeathPenalty(1);
			break;

		case SCMD_RESTART_HERE:
			sys_log(0, "do_restart: restart here");
			ch->RestartAtSamePos();
			//ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY());
			ch->PointChange(POINT_HP, 50 - ch->GetHP());
			ch->DeathPenalty(0);
			ch->ReviveInvisible(5);
			break;
	}
}

#define MAX_STAT g_iStatusPointSetMaxValue

ACMD(do_stat_reset)
{
	ch->PointChange(POINT_STAT_RESET_COUNT, 12 - ch->GetPoint(POINT_STAT_RESET_COUNT));
}

ACMD(do_stat_minus)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "353");
		return;
	}

	if (ch->GetPoint(POINT_STAT_RESET_COUNT) <= 0)
		return;

	if (!strcmp(arg1, "st"))
	{
		if (ch->GetRealPoint(POINT_ST) <= JobInitialPoints[ch->GetJob()].st)
			return;

		ch->SetRealPoint(POINT_ST, ch->GetRealPoint(POINT_ST) - 1);
		ch->SetPoint(POINT_ST, ch->GetPoint(POINT_ST) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_ST, 0);
	}
	else if (!strcmp(arg1, "dx"))
	{
		if (ch->GetRealPoint(POINT_DX) <= JobInitialPoints[ch->GetJob()].dx)
			return;

		ch->SetRealPoint(POINT_DX, ch->GetRealPoint(POINT_DX) - 1);
		ch->SetPoint(POINT_DX, ch->GetPoint(POINT_DX) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_DX, 0);
	}
	else if (!strcmp(arg1, "ht"))
	{
		if (ch->GetRealPoint(POINT_HT) <= JobInitialPoints[ch->GetJob()].ht)
			return;

		ch->SetRealPoint(POINT_HT, ch->GetRealPoint(POINT_HT) - 1);
		ch->SetPoint(POINT_HT, ch->GetPoint(POINT_HT) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_HT, 0);
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (!strcmp(arg1, "iq"))
	{
		if (ch->GetRealPoint(POINT_IQ) <= JobInitialPoints[ch->GetJob()].iq)
			return;

		ch->SetRealPoint(POINT_IQ, ch->GetRealPoint(POINT_IQ) - 1);
		ch->SetPoint(POINT_IQ, ch->GetPoint(POINT_IQ) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_IQ, 0);
		ch->PointChange(POINT_MAX_SP, 0);
	}
	else
		return;

	ch->PointChange(POINT_STAT, +1);
	ch->PointChange(POINT_STAT_RESET_COUNT, -1);
	ch->ComputePoints();
}

ACMD(do_stat)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "353");
		return;
	}

	BYTE REAL_POINT = POINT_STAT;

#ifdef ENABLE_YOHARA_SYSTEM
	if (!strcmp(arg1, "sstr") || !strcmp(arg1, "shp") || !strcmp(arg1, "smove") || !strcmp(arg1, "simmune"))
		REAL_POINT = POINT_CONQUEROR_POINT;
#endif

	if (ch->GetPoint(REAL_POINT) <= 0)
		return;

	BYTE idx = 0;

	if (!strcmp(arg1, "st"))
		idx = POINT_ST;
	else if (!strcmp(arg1, "dx"))
		idx = POINT_DX;
	else if (!strcmp(arg1, "ht"))
		idx = POINT_HT;
	else if (!strcmp(arg1, "iq"))
		idx = POINT_IQ;
#ifdef ENABLE_YOHARA_SYSTEM
	else if (!strcmp(arg1, "sstr"))
		idx = POINT_SUNGMA_STR;
	else if (!strcmp(arg1, "shp"))
		idx = POINT_SUNGMA_HP;
	else if (!strcmp(arg1, "smove"))
		idx = POINT_SUNGMA_MOVE;
	else if (!strcmp(arg1, "simmune"))
		idx = POINT_SUNGMA_IMMUNE;
#endif
	else
		return;

	// ch->ChatPacket(CHAT_TYPE_INFO, "%s GRP(%d) idx(%u), MAX_STAT(%d), expr(%d)", __FUNCTION__, ch->GetRealPoint(idx), idx, MAX_STAT, ch->GetRealPoint(idx) >= MAX_STAT);
	if (ch->GetRealPoint(idx) >= MAX_STAT)
		return;

	ch->SetRealPoint(idx, ch->GetRealPoint(idx) + 1);
	ch->SetPoint(idx, ch->GetPoint(idx) + 1);
	ch->ComputePoints();
	ch->PointChange(idx, 0);

	if (idx == POINT_IQ)
	{
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (idx == POINT_HT)
	{
		ch->PointChange(POINT_MAX_SP, 0);
	}
#ifdef ENABLE_YOHARA_SYSTEM
	else if (idx == POINT_SUNGMA_STR)
	{
		ch->PointChange(POINT_SUNGMA_STR, 0);
		ch->PointChange(POINT_ST, 0);
	}
	else if (idx == POINT_SUNGMA_HP)
	{
		ch->PointChange(POINT_SUNGMA_HP, 0);
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (idx == POINT_SUNGMA_MOVE)
	{
		ch->PointChange(POINT_SUNGMA_MOVE, 0);
		ch->PointChange(POINT_MOV_SPEED, 0);
	}
	else if (idx == POINT_SUNGMA_IMMUNE)
		ch->PointChange(POINT_SUNGMA_IMMUNE, 0);
#endif

	ch->PointChange(REAL_POINT, -1);
	ch->ComputePoints();
}

ACMD(do_pvp)
{

#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_DUEL))
		return;
#endif
	
#if defined(WJ_COMBAT_ZONE)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(ch->GetMapIndex()))
		return;
#endif
	if (ch->GetArena() != NULL || CArenaManager::instance().IsArenaMap(ch->GetMapIndex()) == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "383");
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER pkVictim = CHARACTER_MANAGER::instance().Find(vid);

	if (!pkVictim)
		return;

	if (pkVictim->IsNPC())
		return;

	if (pkVictim->GetArena() != NULL)
	{
		pkVictim->ChatPacket(CHAT_TYPE_INFO, "419");
		return;
	}
#ifdef ENABLE_MESSENGER_BLOCK
	if (MessengerManager::instance().IsBlocked(ch->GetName(), pkVictim->GetName()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You need to unblock %s do to that.", pkVictim->GetName());
		return;
	}
	if (MessengerManager::instance().IsBlocked(pkVictim->GetName(), ch->GetName()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "%s has blocked you.", pkVictim->GetName());
		return;
	}
#endif
	CPVPManager::instance().Insert(ch, pkVictim);
}

ACMD(do_guildskillup)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch->GetGuild())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "420");
		return;
	}

	CGuild* g = ch->GetGuild();
	TGuildMember* gm = g->GetMember(ch->GetPlayerID());
	if (gm->grade == GUILD_LEADER_GRADE)
	{
		DWORD vnum = 0;
		str_to_number(vnum, arg1);
		g->SkillLevelUp(vnum);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "421");
	}
}

ACMD(do_skillup)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vnum = 0;
	str_to_number(vnum, arg1);

	if (true == ch->CanUseSkill(vnum))
	{
		ch->SkillLevelUp(vnum);
	}
	else
	{
		switch(vnum)
		{
			case SKILL_HORSE_WILDATTACK:
			case SKILL_HORSE_CHARGE:
			case SKILL_HORSE_ESCAPE:
			case SKILL_HORSE_WILDATTACK_RANGE:

			case SKILL_7_A_ANTI_TANHWAN:
			case SKILL_7_B_ANTI_AMSEOP:
			case SKILL_7_C_ANTI_SWAERYUNG:
			case SKILL_7_D_ANTI_YONGBI:

			case SKILL_8_A_ANTI_GIGONGCHAM:
			case SKILL_8_B_ANTI_YEONSA:
			case SKILL_8_C_ANTI_MAHWAN:
			case SKILL_8_D_ANTI_BYEURAK:

//#ifdef ENABLE_78_SKILLS
			case SKILL_P_PALBANG:
			case SKILL_P_AMSEUP:
			case SKILL_P_SWAERYEONG:
			case SKILL_P_YONGPA:
			case SKILL_P_GINGONGCHAM:
			case SKILL_P_HWAJO:
			case SKILL_P_MARYEONG:
			case SKILL_P_BYEORAK:
#ifdef ENABLE_WOLFMAN_CHARACTER
			case SKILL_P_SALPOONG:
#endif	
			case SKILL_PP_PALBANG:
			case SKILL_PP_AMSEUP:
			case SKILL_PP_SWAERYONG:
			case SKILL_PP_YONGPA:
			case SKILL_PP_GIGONGCHAM:
			case SKILL_PP_HWAJO:
			case SKILL_PP_MARYEONG:
			case SKILL_PP_BYEORAK:
#ifdef ENABLE_WOLFMAN_CHARACTER
			case SKILL_PP_SALPOONG:
#endif
//#endif
			case SKILL_ADD_HP:
			case SKILL_RESIST_PENETRATE:
				ch->SkillLevelUp(vnum);
				break;
		}
	}
}

//
// @version	05/06/20 Bang2ni - Ä¿¸Çµå Ã³¸® Delegate to CHARACTER class
//
ACMD(do_safebox_close)
{
	ch->CloseSafebox();
}

//
// @version	05/06/20 Bang2ni - Ä¿¸Çµå Ã³¸® Delegate to CHARACTER class
//
ACMD(do_safebox_password)
{
#ifdef __BL_BATTLE_ROYALE__
	if (ch->IsBattleRoyale())
		return;
#endif

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	ch->ReqSafeboxLoad(arg1);
}

ACMD(do_safebox_change_password)
{
	char arg1[256];
	char arg2[256];

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || strlen(arg1)>6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "65");
		return;
	}

	if (!*arg2 || strlen(arg2)>6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "65");
		return;
	}

	TSafeboxChangePasswordPacket p;

	p.dwID = ch->GetDesc()->GetAccountTable().id;
	strlcpy(p.szOldPassword, arg1, sizeof(p.szOldPassword));
	strlcpy(p.szNewPassword, arg2, sizeof(p.szNewPassword));

	db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_CHANGE_PASSWORD, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}

ACMD(do_mall_password)
{
#ifdef __BL_BATTLE_ROYALE__
	if (ch->IsBattleRoyale())
		return;
#endif

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1 || strlen(arg1) > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "65");
		return;
	}

	int iPulse = thecore_pulse();

	if (ch->GetMall())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "66");
		return;
	}

	if (iPulse - ch->GetMallLoadTime() < passes_per_sec * 10) // 10ÃÊ¿¡ ÇÑ¹ø¸¸ ¿äÃ» °¡´É
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "67");
		return;
	}

	ch->SetMallLoadTime(iPulse);

	TSafeboxLoadPacket p;
	p.dwID = ch->GetDesc()->GetAccountTable().id;
	strlcpy(p.szLogin, ch->GetDesc()->GetAccountTable().login, sizeof(p.szLogin));
	strlcpy(p.szPassword, arg1, sizeof(p.szPassword));

	db_clientdesc->DBPacket(HEADER_GD_MALL_LOAD, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}

ACMD(do_mall_close)
{
	if (ch->GetMall())
	{
		ch->SetMallLoadTime(thecore_pulse());
		ch->CloseMall();
		ch->Save();
	}
}

ACMD(do_ungroup)
{
	if (!ch->GetParty())
		return;

	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "46");
		return;
	}

	if (ch->GetDungeon())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "422");
		return;
	}

#ifdef ENABLE_QUEEN_NETHIS
	if (SnakeLair::CSnk::instance().IsSnakeMap(ch->GetMapIndex()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "[LS;676]");
		return;
	}
#endif

	LPPARTY pParty = ch->GetParty();

	if (pParty->GetMemberCount() == 2)
	{
		// party disband
		CPartyManager::instance().DeleteParty(pParty);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "423");
		//pParty->SendPartyRemoveOneToAll(ch);
		pParty->Quit(ch->GetPlayerID());
		//pParty->SendPartyRemoveAllToOne(ch);
	}
}

ACMD(do_set_walk_mode)
{
	ch->SetNowWalking(true);
	ch->SetWalking(true);
}

ACMD(do_set_run_mode)
{
	ch->SetNowWalking(false);
	ch->SetWalking(false);
}

ACMD(do_war)
{
	CGuild* g = ch->GetGuild();
	if (!g)
	{
		return;
	}

	if (g->UnderAnyWar())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "424");
		return;
	}

	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	DWORD type = GUILD_WAR_TYPE_FIELD;

	if (!*arg1)
	{
		return;
	}

	if (*arg2)
	{
		str_to_number(type, arg2);

		if (type >= GUILD_WAR_TYPE_MAX_NUM)
		{
			type = GUILD_WAR_TYPE_FIELD;
		}
	}

	//±æµåÀÇ ¸¶½ºÅÍ ¾ÆÀÌµð¸¦ ¾ò¾î¿ÂµÚ
	DWORD gm_pid = g->GetMasterPID();

	//¸¶½ºÅÍÀÎÁö Ã¼Å©(±æÀüÀº ±æµåÀå¸¸ÀÌ °¡´É)
	if (gm_pid != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "425");
		return;
	}

	//»ó´ë ±æµå¸¦ ¾ò¾î¿À°í
	CGuild* opp_g = CGuildManager::instance().FindGuildByName(arg1);
	if (!opp_g)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "426");
		return;
	}

	//»ó´ë±æµå¿ÍÀÇ »óÅÂ Ã¼Å©
	switch (g->GetGuildWarState(opp_g->GetID()))
	{
		case GUILD_WAR_NONE:
			{
				if (opp_g->UnderAnyWar())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "427");
					return;
				}

				int iWarPrice = KOR_aGuildWarInfo[type].iWarPrice;

				if (g->GetGuildMoney() < iWarPrice)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "428");
					return;
				}

				if (opp_g->GetGuildMoney() < iWarPrice)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "429");
					return;
				}
			}
			break;

		case GUILD_WAR_SEND_DECLARE:
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "430");
				return;
			}
			break;

		case GUILD_WAR_RECV_DECLARE:
			{
				if (opp_g->UnderAnyWar())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "427");
					g->RequestRefuseWar(opp_g->GetID());
					return;
				}
			}
			break;

		case GUILD_WAR_RESERVE:
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "431");
				return;
			}
			break;

		case GUILD_WAR_END:
			return;

		default:
			ch->ChatPacket(CHAT_TYPE_INFO, "432");
			g->RequestRefuseWar(opp_g->GetID());
			return;
	}

	if (!g->CanStartWar(type))
	{
		// ±æµåÀüÀ» ÇÒ ¼ö ÀÖ´Â Á¶°ÇÀ» ¸¸Á·ÇÏÁö¾Ê´Â´Ù.
		if (g->GetLadderPoint() == 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "433");
			sys_log(0, "GuildWar.StartError.NEED_LADDER_POINT");
		}
		else if (g->GetMemberCount() < GUILD_WAR_MIN_MEMBER_COUNT)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "434 %d", GUILD_WAR_MIN_MEMBER_COUNT);
			sys_log(0, "GuildWar.StartError.NEED_MINIMUM_MEMBER[%d]", GUILD_WAR_MIN_MEMBER_COUNT);
		}
		else
		{
			sys_log(0, "GuildWar.StartError.UNKNOWN_ERROR");
		}
		return;
	}

	// ÇÊµåÀü Ã¼Å©¸¸ ÇÏ°í ¼¼¼¼ÇÑ Ã¼Å©´Â »ó´ë¹æÀÌ ½Â³«ÇÒ¶§ ÇÑ´Ù.
	if (!opp_g->CanStartWar(GUILD_WAR_TYPE_FIELD))
	{
		if (opp_g->GetLadderPoint() == 0)
			ch->ChatPacket(CHAT_TYPE_INFO, "435");
		else if (opp_g->GetMemberCount() < GUILD_WAR_MIN_MEMBER_COUNT)
			ch->ChatPacket(CHAT_TYPE_INFO, "436");
		return;
	}

	do
	{
		if (g->GetMasterCharacter() != NULL)
			break;

		CCI *pCCI = P2P_MANAGER::instance().FindByPID(g->GetMasterPID());

		if (pCCI != NULL)
			break;

		ch->ChatPacket(CHAT_TYPE_INFO, "437");
		g->RequestRefuseWar(opp_g->GetID());
		return;

	} while (false);

	do
	{
		if (opp_g->GetMasterCharacter() != NULL)
			break;

		CCI *pCCI = P2P_MANAGER::instance().FindByPID(opp_g->GetMasterPID());

		if (pCCI != NULL)
			break;

		ch->ChatPacket(CHAT_TYPE_INFO, "437");
		g->RequestRefuseWar(opp_g->GetID());
		return;

	} while (false);

	g->RequestDeclareWar(opp_g->GetID(), type);
}

ACMD(do_nowar)
{
	CGuild* g = ch->GetGuild();
	if (!g)
	{
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		return;
	}

	DWORD gm_pid = g->GetMasterPID();

	if (gm_pid != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "425");
		return;
	}

	CGuild* opp_g = CGuildManager::instance().FindGuildByName(arg1);

	if (!opp_g)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "426");
		return;
	}

	g->RequestRefuseWar(opp_g->GetID());
}

ACMD(do_detaillog)
{
	ch->DetailLog();
}

ACMD(do_monsterlog)
{
	ch->ToggleMonsterLog();
}

ACMD(do_pkmode)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	BYTE mode = 0;
	str_to_number(mode, arg1);

	if (mode == PK_MODE_PROTECT)
		return;

	if (ch->GetLevel() < PK_PROTECT_LEVEL && mode != 0)
		return;

#ifdef TOURNAMENT_PVP_SYSTEM
	if (ch->GetMapIndex() == TOURNAMENT_MAP_INDEX)
		return;

	if (mode == PK_MODE_TEAM_A || mode == PK_MODE_TEAM_B)
		return;
#endif

#if defined(WJ_COMBAT_ZONE)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(ch->GetMapIndex()))
		return;
#endif
	
	if (mode == PK_MODE_FEAR && (ch->GetMapIndex() == 91 || ch->GetMapIndex() == 90 || ch->GetMapIndex() == 357))
	{
		ch->ChatPacket(1, "You can't use fear mode in this map.");
		return;
	}
	
	if (mode == PK_MODE_FEAR &&
#ifdef USE_PREMIUM_AFFECT
		ch->GetPoint(POINT_PREMIUM_USER) < 1
#else
		 !ch->FindAffect(AFFECT_AUTO_HUNT)
#endif
	)
	{
		ch->ChatPacket(1, "You can't use fear mode if you don't have Auto-Hunting.");
		return;
	}
	
	ch->SetPKMode(mode);
}

ACMD(do_messenger_auth)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "383");
		return;
	}

	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
		return;

	char answer = LOWER(*arg1);
	// @fixme130 AuthToAdd void -> bool
	bool bIsDenied = answer != 'y';
	bool bIsAdded = MessengerManager::instance().AuthToAdd(ch->GetName(), arg2, bIsDenied); // DENY
	if (bIsAdded && bIsDenied)
	{
		LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg2);

		if (tch)
			tch->ChatPacket(CHAT_TYPE_INFO, "438 %s", ch->GetName());
	}

}
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
ACMD(do_setpickupmode)
{
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1 || !*arg2)
		return;
	DWORD flag = 0;
	str_to_number(flag, arg1);
	DWORD flagRarity = 0;
	str_to_number(flagRarity, arg2);
	ch->ChangePickUPMode(flag, flagRarity);
}
#endif

ACMD(do_setblockmode)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (*arg1)
	{
		BYTE flag = 0;
		str_to_number(flag, arg1);
		ch->SetBlockMode(flag);
	}
}

ACMD(do_unmount)
{
	if (true == ch->UnEquipSpecialRideUniqueItem())
	{
		ch->RemoveAffect(AFFECT_MOUNT);
		ch->RemoveAffect(AFFECT_MOUNT_BONUS);

		if (ch->IsHorseRiding())
		{
			ch->StopRiding();
		}
	}
	else
	{
		ch->ChatPacket( CHAT_TYPE_INFO, "439");
	}

}

ACMD(do_observer_exit)
{
#ifdef ENABLE_DRAGON_LAIR
	if(ch->GetMapIndex() == 208)
	{
		if (ch->IsObserverMode())
		{
			ch->SetObserverMode(false);
			ch->WarpSet(180000,1220000);
			return;
		}
	}
#endif
	if (ch->IsObserverMode())
	{

#ifdef TOURNAMENT_PVP_SYSTEM
		if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_EXIT_OBSERVER_MODE))
			return;
#endif

		if (ch->GetWarMap())
			ch->SetWarMap(NULL);

		if (ch->GetArena() != NULL || ch->GetArenaObserverMode() == true)
		{
			ch->SetArenaObserverMode(false);

			if (ch->GetArena() != NULL)
				ch->GetArena()->RemoveObserver(ch->GetPlayerID());

			ch->SetArena(NULL);
			ch->WarpSet(ARENA_RETURN_POINT_X(ch->GetEmpire()), ARENA_RETURN_POINT_Y(ch->GetEmpire()));
		}
		else
		{
			ch->ExitToSavedLocation();
		}
		ch->SetObserverMode(false);
	}
}

ACMD(do_view_equip)
{
	if (ch->GetGMLevel() <= GM_PLAYER)
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (*arg1)
	{
		DWORD vid = 0;
		str_to_number(vid, arg1);
		LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

		if (!tch)
			return;

		if (!tch->IsPC())
			return;
		/*
		   int iSPCost = ch->GetMaxSP() / 3;

		   if (ch->GetSP() < iSPCost)
		   {
		   ch->ChatPacket(CHAT_TYPE_INFO, "440");
		   return;
		   }
		   ch->PointChange(POINT_SP, -iSPCost);
		 */
		tch->SendEquipment(ch);
	}
}

ACMD(do_party_request)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "383");
		return;
	}

	if (ch->GetParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "441");
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		if (!ch->RequestToParty(tch))
			ch->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

ACMD(do_party_request_accept)
{

#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_PARTY))
		return;
#endif

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->AcceptToParty(tch);
}

ACMD(do_party_request_deny)
{

#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_PARTY))
		return;
#endif

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->DenyToParty(tch);
}

ACMD(do_monarch_warpto)
{
	if (!CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "442");
		return;
	}

	//±ºÁÖ ÄðÅ¸ÀÓ °Ë»ç
	if (!ch->IsMCOK(CHARACTER::MI_WARP))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "443 %d", ch->GetMCLTime(CHARACTER::MI_WARP));
		return;
	}

	//±ºÁÖ ¸÷ ¼ÒÈ¯ ºñ¿ë
	const int WarpPrice = 10000;

	//±ºÁÖ ±¹°í °Ë»ç
	if (!CMonarch::instance().IsMoneyOk(WarpPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, "444 %u %u", NationMoney, WarpPrice);
		return;
	}

	int x = 0, y = 0;
	char arg1[256];

	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "445");
		return;
	}

	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg1);

	if (!tch)
	{
		CCI * pkCCI = P2P_MANAGER::instance().Find(arg1);

		if (pkCCI)
		{
			if (pkCCI->bEmpire != ch->GetEmpire())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "446");
				return;
			}

			if (pkCCI->bChannel != g_bChannel)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "447 %d %d", pkCCI->bChannel, g_bChannel);
				return;
			}
			if (!IsMonarchWarpZone(pkCCI->lMapIndex))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, "448");
				return;
			}

			PIXEL_POSITION pos;

			if (!SECTREE_MANAGER::instance().GetCenterPositionOfMap(pkCCI->lMapIndex, pos))
				ch->ChatPacket(CHAT_TYPE_INFO, "Cannot find map (index %d)", pkCCI->lMapIndex);
			else
			{
				//ch->ChatPacket(CHAT_TYPE_INFO, "You warp to (%d, %d)", pos.x, pos.y);
				ch->ChatPacket(CHAT_TYPE_INFO, "449 %s", arg1);
				ch->WarpSet(pos.x, pos.y);

				//±ºÁÖ µ· »è°¨
				CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);

				//ÄðÅ¸ÀÓ ÃÊ±âÈ­
				ch->SetMC(CHARACTER::MI_WARP);
			}
		}
		else if (NULL == CHARACTER_MANAGER::instance().FindPC(arg1))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "There is no one by that name");
		}

		return;
	}
	else
	{
		if (tch->GetEmpire() != ch->GetEmpire())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "446");
			return;
		}
		if (!IsMonarchWarpZone(tch->GetMapIndex()))
		{
			ch->ChatPacket (CHAT_TYPE_INFO, "448");
			return;
		}
		x = tch->GetX();
		y = tch->GetY();
	}

	ch->ChatPacket(CHAT_TYPE_INFO, "449 %s", arg1);
	ch->WarpSet(x, y);
	ch->Stop();

	//±ºÁÖ µ· »è°¨
	CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);

	//ÄðÅ¸ÀÓ ÃÊ±âÈ­
	ch->SetMC(CHARACTER::MI_WARP);
}

ACMD(do_monarch_transfer)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "450");
		return;
	}

	if (!CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "442");
		return;
	}

	//±ºÁÖ ÄðÅ¸ÀÓ °Ë»ç
	if (!ch->IsMCOK(CHARACTER::MI_TRANSFER))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "443 %d", ch->GetMCLTime(CHARACTER::MI_TRANSFER));
		return;
	}

	//±ºÁÖ ¿öÇÁ ºñ¿ë
	const int WarpPrice = 10000;

	//±ºÁÖ ±¹°í °Ë»ç
	if (!CMonarch::instance().IsMoneyOk(WarpPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, "444 %u %u", NationMoney, WarpPrice);
		return;
	}


	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg1);

	if (!tch)
	{
		CCI * pkCCI = P2P_MANAGER::instance().Find(arg1);

		if (pkCCI)
		{
			if (pkCCI->bEmpire != ch->GetEmpire())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "451");
				return;
			}
			if (pkCCI->bChannel != g_bChannel)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "452 %s %s %d", arg1, pkCCI->bChannel, g_bChannel);
				return;
			}
			if (!IsMonarchWarpZone(pkCCI->lMapIndex))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, "448");
				return;
			}
			if (!IsMonarchWarpZone(ch->GetMapIndex()))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, "453");
				return;
			}

			TPacketGGTransfer pgg;

			pgg.bHeader = HEADER_GG_TRANSFER;
			strlcpy(pgg.szName, arg1, sizeof(pgg.szName));
			pgg.lX = ch->GetX();
			pgg.lY = ch->GetY();

			P2P_MANAGER::instance().Send(&pgg, sizeof(TPacketGGTransfer));
			ch->ChatPacket(CHAT_TYPE_INFO, "454 %s", arg1);

			//±ºÁÖ µ· »è°¨
			CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);
			//ÄðÅ¸ÀÓ ÃÊ±âÈ­
			ch->SetMC(CHARACTER::MI_TRANSFER);
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "455");
		}

		return;
	}


	if (ch == tch)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "456");
		return;
	}

	if (tch->GetEmpire() != ch->GetEmpire())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "451");
		return;
	}
	if (!IsMonarchWarpZone(tch->GetMapIndex()))
	{
		ch->ChatPacket (CHAT_TYPE_INFO, "448");
		return;
	}
	if (!IsMonarchWarpZone(ch->GetMapIndex()))
	{
		ch->ChatPacket (CHAT_TYPE_INFO, "453");
		return;
	}

	//tch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
	tch->WarpSet(ch->GetX(), ch->GetY(), ch->GetMapIndex());

	//±ºÁÖ µ· »è°¨
	CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);
	//ÄðÅ¸ÀÓ ÃÊ±âÈ­
	ch->SetMC(CHARACTER::MI_TRANSFER);
}

ACMD(do_monarch_info)
{
	if (CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "457");
		TMonarchInfo * p = CMonarch::instance().GetMonarch();
		for (int n = 1; n < 4; ++n)
		{
			if (n == ch->GetEmpire())
				ch->ChatPacket(CHAT_TYPE_INFO, "458 %s %s %lld", EMPIRE_NAME(n), p->name[n], p->money[n]);
			else
				ch->ChatPacket(CHAT_TYPE_INFO, "459 %s %s", EMPIRE_NAME(n), p->name[n]);

		}
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "460");
		TMonarchInfo * p = CMonarch::instance().GetMonarch();
		for (int n = 1; n < 4; ++n)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "459 %s %s", EMPIRE_NAME(n), p->name[n]);

		}
	}

}

ACMD(do_elect)
{
	db_clientdesc->DBPacketHeader(HEADER_GD_COME_TO_VOTE, ch->GetDesc()->GetHandle(), 0);
}

// LUA_ADD_GOTO_INFO
struct GotoInfo
{
	std::string 	st_name;

	BYTE 	empire;
	int 	mapIndex;
	DWORD 	x, y;

	GotoInfo()
	{
		st_name 	= "";
		empire 		= 0;
		mapIndex 	= 0;

		x = 0;
		y = 0;
	}

	GotoInfo(const GotoInfo& c_src)
	{
		__copy__(c_src);
	}

	void operator = (const GotoInfo& c_src)
	{
		__copy__(c_src);
	}

	void __copy__(const GotoInfo& c_src)
	{
		st_name 	= c_src.st_name;
		empire 		= c_src.empire;
		mapIndex 	= c_src.mapIndex;

		x = c_src.x;
		y = c_src.y;
	}
};

ACMD(do_monarch_tax)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: monarch_tax <1-50>");
		return;
	}

	// ±ºÁÖ °Ë»ç
	if (!ch->IsMonarch())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "461");
		return;
	}

	// ¼¼±Ý¼³Á¤
	int tax = 0;
	str_to_number(tax,  arg1);

	if (tax < 1 || tax > 50)
		ch->ChatPacket(CHAT_TYPE_INFO, "462");

	quest::CQuestManager::instance().SetEventFlag("trade_tax", tax);

	// ±ºÁÖ¿¡°Ô ¸Þ¼¼Áö ÇÏ³ª
	ch->ChatPacket(CHAT_TYPE_INFO, "463 %d");

	// °øÁö
	char szMsg[1024];

	snprintf(szMsg, sizeof(szMsg), "±ºÁÖÀÇ ¸íÀ¸·Î ¼¼±ÝÀÌ %d %% ·Î º¯°æµÇ¾ú½À´Ï´Ù", tax);
	BroadcastNotice(szMsg);

	snprintf(szMsg, sizeof(szMsg), "¾ÕÀ¸·Î´Â °Å·¡ ±Ý¾×ÀÇ %d %% °¡ ±¹°í·Î µé¾î°¡°ÔµË´Ï´Ù.", tax);
	BroadcastNotice(szMsg);

	// ÄðÅ¸ÀÓ ÃÊ±âÈ­
	ch->SetMC(CHARACTER::MI_TAX);
}

static const DWORD cs_dwMonarchMobVnums[] =
{
	191, //	»ê°ß½Å
	192, //	Àú½Å
	193, //	¿õ½Å
	194, //	È£½Å
	391, //	¹ÌÁ¤
	392, //	ÀºÁ¤
	393, //	¼¼¶û
	394, //	ÁøÈñ
	491, //	¸ÍÈ¯
	492, //	º¸¿ì
	493, //	±¸ÆÐ
	494, //	ÃßÈç
	591, //	ºñ·ù´Ü´ëÀå
	691, //	¿õ±Í Á·Àå
	791, //	¹Ð±³±³ÁÖ
	1304, // ´©··¹ü±Í
	1901, // ±¸¹ÌÈ£
	2091, // ¿©¿Õ°Å¹Ì
	2191, // °Å´ë»ç¸·°ÅºÏ
	2206, // È­¿°¿Õi
	0,
};

ACMD(do_monarch_mob)
{
	char arg1[256];
	LPCHARACTER	tch;

	one_argument(argument, arg1, sizeof(arg1));

	if (!ch->IsMonarch())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "461");
		return;
	}

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: mmob <mob name>");
		return;
	}

#ifdef ENABLE_MONARCH_MOB_CMD_MAP_CHECK // @warme006
	BYTE pcEmpire = ch->GetEmpire();
	BYTE mapEmpire = SECTREE_MANAGER::instance().GetEmpireFromMapIndex(ch->GetMapIndex());
	if (mapEmpire != pcEmpire && mapEmpire != 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "464");
		return;
	}
#endif

	// ±ºÁÖ ¸÷ ¼ÒÈ¯ ºñ¿ë
	const int SummonPrice = 5000000;

	// ±ºÁÖ ÄðÅ¸ÀÓ °Ë»ç
	if (!ch->IsMCOK(CHARACTER::MI_SUMMON))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "443 %d", ch->GetMCLTime(CHARACTER::MI_SUMMON));
		return;
	}

	// ±ºÁÖ ±¹°í °Ë»ç
	if (!CMonarch::instance().IsMoneyOk(SummonPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, "444 %u %u", NationMoney, SummonPrice);
		return;
	}

	const CMob * pkMob;
	DWORD vnum = 0;

	if (isdigit(*arg1))
	{
		str_to_number(vnum, arg1);

		if ((pkMob = CMobManager::instance().Get(vnum)) == NULL)
			vnum = 0;
	}
	else
	{
		pkMob = CMobManager::Instance().Get(arg1, true);

		if (pkMob)
			vnum = pkMob->m_table.dwVnum;
	}

	DWORD count;

	// ¼ÒÈ¯ °¡´É ¸÷ °Ë»ç
	for (count = 0; cs_dwMonarchMobVnums[count] != 0; ++count)
		if (cs_dwMonarchMobVnums[count] == vnum)
			break;

	if (0 == cs_dwMonarchMobVnums[count])
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "465");
		return;
	}

	tch = CHARACTER_MANAGER::instance().SpawnMobRange(vnum,
			ch->GetMapIndex(),
			ch->GetX() - number(200, 750),
			ch->GetY() - number(200, 750),
			ch->GetX() + number(200, 750),
			ch->GetY() + number(200, 750),
			true,
			pkMob->m_table.bType == CHAR_TYPE_STONE,
			true);

	if (tch)
	{
		// ±ºÁÖ µ· »è°¨
		CMonarch::instance().SendtoDBDecMoney(SummonPrice, ch->GetEmpire(), ch);

		// ÄðÅ¸ÀÓ ÃÊ±âÈ­
		ch->SetMC(CHARACTER::MI_SUMMON);
	}
}

static const char* FN_point_string(int apply_number)
{
	switch (apply_number)
	{
#ifdef ENABLE_YOHARA_SYSTEM
		case POINT_SUNGMA_STR:           return "466";
		case POINT_SUNGMA_HP:           return "466";
		case POINT_SUNGMA_MOVE:           return "466";
		case POINT_SUNGMA_IMMUNE:           return "466";
		case POINT_CONQUEROR_POINT:           return "466";
#endif

		case POINT_MAX_HP:	return "467 %d";
		case POINT_MAX_SP:	return "468 %d";
		case POINT_HT:		return "469 %d";
		case POINT_IQ:		return "470 %d";
		case POINT_ST:		return "471 %d";
		case POINT_DX:		return "472 %d";
		case POINT_ATT_SPEED:	return "473 %d";
		case POINT_MOV_SPEED:	return "474 %d";
		case POINT_CASTING_SPEED:	return "475 %d";
		case POINT_HP_REGEN:	return "476 %d";
		case POINT_SP_REGEN:	return "477 %d";
		case POINT_POISON_PCT:	return "478 %d";
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_BLEEDING_PCT:	return "478 %d";
#endif
		case POINT_STUN_PCT:	return "479 %d";
		case POINT_SLOW_PCT:	return "480 %d";
		case POINT_CRITICAL_PCT:	return "481 %d%%";
		case POINT_RESIST_CRITICAL:	return "482 %d%%";
		case POINT_PENETRATE_PCT:	return "483 %d%%";
		case POINT_RESIST_PENETRATE: return "484 %d%%";
		case POINT_ATTBONUS_HUMAN:	return "485 %d%%";
		case POINT_ATTBONUS_ANIMAL:	return "486 %d%%";
		case POINT_ATTBONUS_ORC:	return "487 %d%%";
		case POINT_ATTBONUS_MILGYO:	return "488 %d%%";
		case POINT_ATTBONUS_UNDEAD:	return "489 %d%%";
		case POINT_ATTBONUS_DEVIL:	return "490 %d%%";
		case POINT_STEAL_HP:		return "491 %d%%";
		case POINT_STEAL_SP:		return "492 %d%%";
		case POINT_MANA_BURN_PCT:	return "493 %d%%";
		case POINT_DAMAGE_SP_RECOVER:	return "494 %d%%";
		case POINT_BLOCK:			return "495 %d%%";
		case POINT_DODGE:			return "496 %d%%";
		case POINT_RESIST_SWORD:	return "497 %d%%";
		case POINT_RESIST_TWOHAND:	return "498 %d%%";
		case POINT_RESIST_DAGGER:	return "499 %d%%";
		case POINT_RESIST_BELL:		return "500 %d%%";
		case POINT_RESIST_FAN:		return "501 %d%%";
		case POINT_RESIST_BOW:		return "502 %d%%";
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_RESIST_CLAW:		return "499 %d%%";
#endif
		case POINT_RESIST_FIRE:		return "503 %d%%";
		case POINT_RESIST_ELEC:		return "504 %d%%";
		case POINT_RESIST_MAGIC:	return "505 %d%%";
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
		case POINT_RESIST_MAGIC_REDUCTION:	return "505 %d%%";
#endif
		case POINT_RESIST_WIND:		return "506 %d%%";
		case POINT_RESIST_ICE:		return "507 %d%%";
		case POINT_RESIST_EARTH:	return "508 %d%%";
		case POINT_RESIST_DARK:		return "509 %d%%";
		case POINT_REFLECT_MELEE:	return "510 %d%%";
		case POINT_REFLECT_CURSE:	return "511 %d%%";
		case POINT_POISON_REDUCE:	return "512 %d%%";
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_BLEEDING_REDUCE:	return "512 %d%%";
#endif
		case POINT_KILL_SP_RECOVER:	return "513 %d%%";
		case POINT_EXP_DOUBLE_BONUS:	return "514 %d%%";
		case POINT_GOLD_DOUBLE_BONUS:	return "515 %d%%";
		case POINT_ITEM_DROP_BONUS:	return "516 %d%%";
		case POINT_POTION_BONUS:	return "517 %d%%";
		case POINT_KILL_HP_RECOVERY:	return "518 %d%%";
//		case POINT_IMMUNE_STUN:	return "519 %d%%";
//		case POINT_IMMUNE_SLOW:	return "520 %d%%";
//		case POINT_IMMUNE_FALL:	return "521 %d%%";
//		case POINT_SKILL:	return "466";
//		case POINT_BOW_DISTANCE:	return "466";
		case POINT_ATT_GRADE_BONUS:	return "522 %d";
		case POINT_DEF_GRADE_BONUS:	return "523 %d";
		case POINT_MAGIC_ATT_GRADE:	return "524 %d";
		case POINT_MAGIC_DEF_GRADE:	return "525 %d";
//		case POINT_CURSE_PCT:	return "466";
		case POINT_MAX_STAMINA:	return "526 %d";
		case POINT_ATTBONUS_WARRIOR:	return "527 %d%%";
		case POINT_ATTBONUS_ASSASSIN:	return "528 %d%%";
		case POINT_ATTBONUS_SURA:		return "529 %d%%";
		case POINT_ATTBONUS_SHAMAN:		return "530 %d%%";
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_ATTBONUS_WOLFMAN:	return "530 %d%%";
#endif
		case POINT_ATTBONUS_MONSTER:	return "531 %d%%";
		case POINT_MALL_ATTBONUS:		return "532 %d%%";
		case POINT_MALL_DEFBONUS:		return "533 %d%%";
		case POINT_MALL_EXPBONUS:		return "534 %d%%";
		case POINT_MALL_ITEMBONUS:		return "535";
		case POINT_MALL_GOLDBONUS:		return "536";
		case POINT_MAX_HP_PCT:			return "537 %d%%";
		case POINT_MAX_SP_PCT:			return "538 %d%%";
		case POINT_SKILL_DAMAGE_BONUS:	return "539 %d%%";
		case POINT_NORMAL_HIT_DAMAGE_BONUS:	return "540 %d%%";
		case POINT_SKILL_DEFEND_BONUS:		return "541 %d%%";
		case POINT_NORMAL_HIT_DEFEND_BONUS:	return "542 %d%%";
//		case POINT_PC_BANG_EXP_BONUS:	return "466";
//		case POINT_PC_BANG_DROP_BONUS:	return "466";
//		case POINT_EXTRACT_HP_PCT:	return "466";
		case POINT_RESIST_WARRIOR:	return "543 %d%%";
		case POINT_RESIST_ASSASSIN:	return "544 %d%%";
		case POINT_RESIST_SURA:		return "545 %d%%";
		case POINT_RESIST_SHAMAN:	return "546 %d%%";
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_RESIST_WOLFMAN:	return "546 %d%%";
#endif
		default:					return NULL;
	}
}

static bool FN_hair_affect_string(LPCHARACTER ch, char *buf, size_t bufsiz)
{
	if (NULL == ch || NULL == buf)
		return false;

	CAffect* aff = NULL;
	time_t expire = 0;
	struct tm ltm;
	int	year, mon, day;
	int	offset = 0;

	aff = ch->FindAffect(AFFECT_HAIR);

	if (NULL == aff)
		return false;

	expire = ch->GetQuestFlag("hair.limit_time");

	if (expire < get_global_time())
		return false;

	// set apply string
	offset = snprintf(buf, bufsiz, FN_point_string(aff->bApplyOn), aff->lApplyValue);

	if (offset < 0 || offset >= (int) bufsiz)
		offset = bufsiz - 1;

	localtime_r(&expire, &ltm);

	year	= ltm.tm_year + 1900;
	mon		= ltm.tm_mon + 1;
	day		= ltm.tm_mday;

	snprintf(buf + offset, bufsiz - offset, "547 %d %d %d", year, mon, day);

	return true;
}

ACMD(do_costume)
{
	char buf[512];
	const size_t bufferSize = sizeof(buf);

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	CItem* pBody = ch->GetWear(WEAR_COSTUME_BODY);
	CItem* pHair = ch->GetWear(WEAR_COSTUME_HAIR);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	CItem* pMount = ch->GetWear(WEAR_COSTUME_MOUNT);
#endif
	#ifdef __SASH_SYSTEM__
	CItem * pSash = ch->GetWear(WEAR_COSTUME_SASH);
	#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	CItem* pWeapon = ch->GetWear(WEAR_COSTUME_WEAPON);
#endif

	ch->ChatPacket(CHAT_TYPE_INFO, "COSTUME status:");

	if (pHair)
	{
		const char* itemName = pHair->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  HAIR : %s", itemName);

		for (int i = 0; i < pHair->GetAttributeCount(); ++i)
		{
			const TPlayerItemAttribute& attr = pHair->GetAttribute(i);
			if (0 < attr.bType)
			{
				snprintf(buf, bufferSize, FN_point_string(attr.bType), attr.sValue);
				ch->ChatPacket(CHAT_TYPE_INFO, "     %s", buf);
			}
		}

		if (pHair->IsEquipped() && arg1[0] == 'h')
			ch->UnequipItem(pHair);
	}

	if (pBody)
	{
		const char* itemName = pBody->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  BODY : %s", itemName);

		if (pBody->IsEquipped() && arg1[0] == 'b')
			ch->UnequipItem(pBody);
	}

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (pMount)
	{
		const char* itemName = pMount->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  MOUNT : %s", itemName);

		if (pMount->IsEquipped() && arg1[0] == 'm')
			ch->UnequipItem(pMount);
	}
#endif

	#ifdef __SASH_SYSTEM__
	if (pSash)
	{
		const char * itemName = pSash->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  SASH: %s", itemName);
		for (int i = 0; i < pSash->GetAttributeCount(); ++i)
		{
			const TPlayerItemAttribute& attr = pSash->GetAttribute(i);
			if (attr.bType > 0)
			{
				const char * pAttrName = FN_point_string(attr.bType);
				if (pAttrName == NULL)
					continue;
				
				snprintf(buf, sizeof(buf), FN_point_string(attr.bType), attr.sValue);
				ch->ChatPacket(CHAT_TYPE_INFO, "     %s", buf);
			}
		}

		if (pSash->IsEquipped() && arg1[0] == 's')
			ch->UnequipItem(pSash);
	}
	#endif

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	if (pWeapon)
	{
		const char* itemName = pWeapon->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  WEAPON : %s", itemName);

		if (pWeapon->IsEquipped() && arg1[0] == 'w')
			ch->UnequipItem(pWeapon);
	}
#endif
}

ACMD(do_hair)
{
	char buf[256];

	if (false == FN_hair_affect_string(ch, buf, sizeof(buf)))
		return;

	ch->ChatPacket(CHAT_TYPE_INFO, buf);
}

ACMD(do_inventory)
{
	int	index = 0;
	int	count		= 1;

	char arg1[256];
	char arg2[256];

	LPITEM	item;

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: inventory <start_index> <count>");
		return;
	}

	if (!*arg2)
	{
		index = 0;
		str_to_number(count, arg1);
	}
	else
	{
		str_to_number(index, arg1); index = MIN(index, INVENTORY_MAX_NUM);
		str_to_number(count, arg2); count = MIN(count, INVENTORY_MAX_NUM);
	}

	for (int i = 0; i < count; ++i)
	{
		if (index >= INVENTORY_MAX_NUM)
			break;

		item = ch->GetInventoryItem(index);

		ch->ChatPacket(CHAT_TYPE_INFO, "inventory [%d] = %s",
						index, item ? item->GetName() : "<NONE>");
		++index;
	}
}

//gift notify quest command
ACMD(do_gift)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "gift");
}
#ifdef __NEWPET_SYSTEM__
ACMD(do_CubePetAdd) {

	int pos = 0;
	int invpos = 0;

	const char *line;
	char arg1[256], arg2[256], arg3[256];

	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));

	if (0 == arg1[0])
		return;
	const std::string& strArg1 = std::string(arg1);
	switch (LOWER(arg1[0]))
	{
	case 'a':	// add cue_index inven_index
	{
		if (0 == arg2[0] || !isdigit(*arg2) ||
			0 == arg3[0] || !isdigit(*arg3))
			return;

		str_to_number(pos, arg2);
		str_to_number(invpos, arg3);

	}
	break;

	default:
		return;
	}

	if (ch->GetNewPetSystem()->IsActivePet())
		ch->GetNewPetSystem()->SetItemCube(pos, invpos);
	else
		return;

}

ACMD(do_PetSkill) {
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;

	DWORD skillslot = 0;
	str_to_number(skillslot, arg1);
	if (skillslot > 2 || skillslot < 0)
		return;

	if (ch->GetNewPetSystem()->IsActivePet())
		ch->GetNewPetSystem()->DoPetSkill(skillslot);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, "548");
}

ACMD(do_FeedCubePet) {
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;

	DWORD feedtype = 0;
	str_to_number(feedtype, arg1);
	if (ch->GetNewPetSystem()->IsActivePet())
		ch->GetNewPetSystem()->ItemCubeFeed(feedtype);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, "548");
}

ACMD(do_PetEvo) {

	if (ch->GetExchange()
		 || ch->GetShop()
		 || ch->IsOpenSafebox()
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
		ch->ChatPacket(CHAT_TYPE_INFO, "549");
		return;
	}

	CNewPetSystem* pPetSystem = ch->GetNewPetSystem();
	if (!pPetSystem)
	{
		return;
	}

	if (pPetSystem->IsActivePet()) {

		int it[3][4] = { 
						{ 55003,31107 }, //Here Modify Items to request for 1 evo
						{ 55004,31107,31108 }, //Here Modify Items to request for 2 evo
						{ 55005,31107,31108,31114,31109 }  //Here Modify Items to request for 3 evo
		};
		int ic[3][4] = {{ 15,200 },
						{ 15,200,200 },
						{ 15,500,500,500,500 }
		};
		int tmpevo = pPetSystem->GetEvolution();

		//* Here you have evolution step */ now should be ok
		if (pPetSystem->GetLevel() == 41 && tmpevo == 0 ||
			pPetSystem->GetLevel() == 81 && tmpevo == 1 ||
			pPetSystem->GetLevel() == 82 && tmpevo == 2) {

			for (int b = 0; b < 4; b++) {
				if (ch->CountSpecifyItem(it[tmpevo][b]) < ic[tmpevo][b]) {
					ch->ChatPacket(CHAT_TYPE_INFO, "550");
					for (int c = 0; c < 4; c++) {
						DWORD vnum = it[tmpevo][c];
						ch->ChatPacket(CHAT_TYPE_INFO, "%s X%d", ITEM_MANAGER::instance().GetTable(vnum)->szLocaleName , ic[tmpevo][c]);
					}
					return;
				}
			}
			for (int c = 0; c < 4; c++) {
				ch->RemoveSpecifyItem(it[tmpevo][c], ic[tmpevo][c]);
			}

			pPetSystem->IncreasePetEvolution();
		}
		else {
			ch->ChatPacket(CHAT_TYPE_INFO, "549");
			return;
		}

	}else
		ch->ChatPacket(CHAT_TYPE_INFO, "548");

}

#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
ACMD(do_cube)
{

	const char *line;
	char arg1[256], arg2[256], arg3[256];
	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));

	if (0 == arg1[0])
	{
		return;
	}

	switch (LOWER(arg1[0]))
	{
		case 'o':	// open
			Cube_open(ch);
			break;

		default:
			return;
	}
}
#else

ACMD(do_cube)
{
	if (!ch->CanDoCube())
		return;

	dev_log(LOG_DEB0, "CUBE COMMAND <%s>: %s", ch->GetName(), argument);
	int cube_index = 0, inven_index = 0;
#ifdef ENABLE_SPECIAL_STORAGE
	int inven_type = 0;
	
	char arg1[256], arg2[256], arg3[256], arg4[256];
	two_arguments (two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3), arg4, sizeof(arg4));
#else
	const char *line;
	char arg1[256], arg2[256], arg3[256];

	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));
#endif


	if (0 == arg1[0])
	{
		// print usage
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: cube open");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube close");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube add <inveltory_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube delete <cube_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube list");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube cancel");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube make [all]");
		return;
	}

	const std::string& strArg1 = std::string(arg1);

	// r_info (request information)
	// /cube r_info     ==> (Client -> Server) ÇöÀç NPC°¡ ¸¸µé ¼ö ÀÖ´Â ·¹½ÃÇÇ ¿äÃ»
	//					    (Server -> Client) /cube r_list npcVNUM resultCOUNT 123,1/125,1/128,1/130,5
	//
	// /cube r_info 3   ==> (Client -> Server) ÇöÀç NPC°¡ ¸¸µé¼ö ÀÖ´Â ·¹½ÃÇÇ Áß 3¹øÂ° ¾ÆÀÌÅÛÀ» ¸¸µå´Â µ¥ ÇÊ¿äÇÑ Á¤º¸¸¦ ¿äÃ»
	// /cube r_info 3 5 ==> (Client -> Server) ÇöÀç NPC°¡ ¸¸µé¼ö ÀÖ´Â ·¹½ÃÇÇ Áß 3¹øÂ° ¾ÆÀÌÅÛºÎÅÍ ÀÌÈÄ 5°³ÀÇ ¾ÆÀÌÅÛÀ» ¸¸µå´Â µ¥ ÇÊ¿äÇÑ Àç·á Á¤º¸¸¦ ¿äÃ»
	//					   (Server -> Client) /cube m_info startIndex count 125,1|126,2|127,2|123,5&555,5&555,4/120000@125,1|126,2|127,2|123,5&555,5&555,4/120000
	//
	if (strArg1 == "r_info")
	{
		if (0 == arg2[0])
			Cube_request_result_list(ch);
		else
		{
			if (isdigit(*arg2))
			{
				int listIndex = 0, requestCount = 1;
				str_to_number(listIndex, arg2);

				if (0 != arg3[0] && isdigit(*arg3))
					str_to_number(requestCount, arg3);

				Cube_request_material_info(ch, listIndex, requestCount);
			}
		}

		return;
	}

	switch (LOWER(arg1[0]))
	{
		case 'o':	// open
			Cube_open(ch);
			break;

		case 'c':	// close
			Cube_close(ch);
			break;

		case 'l':	// list
			Cube_show_list(ch);
			break;

		case 'a':	// add cue_index inven_index
			{

#ifdef ENABLE_SPECIAL_STORAGE
				if (0 == arg2[0] || !isdigit(*arg2) || 0 == arg3[0] || !isdigit(*arg3) || 0 == arg4[0] || !isdigit(*arg4))
#else
				if (0 == arg2[0] || !isdigit(*arg2) || 0 == arg3[0] || !isdigit(*arg3))
#endif

					return;

				str_to_number(cube_index, arg2);
				str_to_number(inven_index, arg3);
#ifdef ENABLE_SPECIAL_STORAGE
				str_to_number(inven_type, arg4);
				Cube_add_item (ch, cube_index, inven_index, inven_type);
#else
				Cube_add_item (ch, cube_index, inven_index);
#endif

			}
			break;

		case 'd':	// delete
			{
				if (0 == arg2[0] || !isdigit(*arg2))
					return;

				str_to_number(cube_index, arg2);
				Cube_delete_item (ch, cube_index);
			}
			break;

		case 'm':	// make
			if (0 != arg2[0])
			{
				while (true == Cube_make(ch))
					dev_log (LOG_DEB0, "cube make success");
			}
			else
				Cube_make(ch);
			break;

		default:
			return;
	}
}
#endif

ACMD(do_in_game_mall)
{
#ifdef ENABLE_ITEMSHOP
	ch->ChatPacket(CHAT_TYPE_COMMAND, "open_ishop");
#else
	if (LC_IsEurope() == true)
	{
		char country_code[3];

		switch (LC_GetLocalType())
		{
			case LC_GERMANY:	country_code[0] = 'd'; country_code[1] = 'e'; country_code[2] = '\0'; break;
			case LC_FRANCE:		country_code[0] = 'f'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_ITALY:		country_code[0] = 'i'; country_code[1] = 't'; country_code[2] = '\0'; break;
			case LC_SPAIN:		country_code[0] = 'e'; country_code[1] = 's'; country_code[2] = '\0'; break;
			case LC_UK:			country_code[0] = 'e'; country_code[1] = 'n'; country_code[2] = '\0'; break;
			case LC_TURKEY:		country_code[0] = 't'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_POLAND:		country_code[0] = 'p'; country_code[1] = 'l'; country_code[2] = '\0'; break;
			case LC_PORTUGAL:	country_code[0] = 'p'; country_code[1] = 't'; country_code[2] = '\0'; break;
			case LC_GREEK:		country_code[0] = 'g'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_RUSSIA:		country_code[0] = 'r'; country_code[1] = 'u'; country_code[2] = '\0'; break;
			case LC_DENMARK:	country_code[0] = 'd'; country_code[1] = 'k'; country_code[2] = '\0'; break;
			case LC_BULGARIA:	country_code[0] = 'b'; country_code[1] = 'g'; country_code[2] = '\0'; break;
			case LC_CROATIA:	country_code[0] = 'h'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_MEXICO:		country_code[0] = 'm'; country_code[1] = 'x'; country_code[2] = '\0'; break;
			case LC_ARABIA:		country_code[0] = 'a'; country_code[1] = 'e'; country_code[2] = '\0'; break;
			case LC_CZECH:		country_code[0] = 'c'; country_code[1] = 'z'; country_code[2] = '\0'; break;
			case LC_ROMANIA:	country_code[0] = 'r'; country_code[1] = 'o'; country_code[2] = '\0'; break;
			case LC_HUNGARY:	country_code[0] = 'h'; country_code[1] = 'u'; country_code[2] = '\0'; break;
			case LC_NETHERLANDS: country_code[0] = 'n'; country_code[1] = 'l'; country_code[2] = '\0'; break;
			case LC_USA:		country_code[0] = 'u'; country_code[1] = 's'; country_code[2] = '\0'; break;
			case LC_CANADA:	country_code[0] = 'c'; country_code[1] = 'a'; country_code[2] = '\0'; break;
			default:
				if (test_server == true)
				{
					country_code[0] = 'd'; country_code[1] = 'e'; country_code[2] = '\0';
				}
				break;
		}

		char buf[512+1];
		char sas[33];
		MD5_CTX ctx;
		const char sas_key[] = "GF9001";

		snprintf(buf, sizeof(buf), "%u%u%s", ch->GetPlayerID(), ch->GetAID(), sas_key);

		MD5Init(&ctx);
		MD5Update(&ctx, (const unsigned char *) buf, strlen(buf));
#ifdef __FreeBSD__
		MD5End(&ctx, sas);
#else
		static const char hex[] = "0123456789abcdef";
		unsigned char digest[16];
		MD5Final(digest, &ctx);
		int i;
		for (i = 0; i < 16; ++i) {
			sas[i+i] = hex[digest[i] >> 4];
			sas[i+i+1] = hex[digest[i] & 0x0f];
		}
		sas[i+i] = '\0';
#endif

		snprintf(buf, sizeof(buf), "mall http://%s/ishop?pid=%u&c=%s&sid=%d&sas=%s",
				g_strWebMallURL.c_str(), ch->GetPlayerID(), country_code, g_server_id, sas);

		ch->ChatPacket(CHAT_TYPE_COMMAND, buf);
	}
#endif
}

// ÁÖ»çÀ§
ACMD(do_dice)
{
	char arg1[256], arg2[256];
	int start = 1, end = 100;

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (*arg1 && *arg2)
	{
		start = atoi(arg1);
		end = atoi(arg2);
	}
	else if (*arg1 && !*arg2)
	{
		start = 1;
		end = atoi(arg1);
	}

	end = MAX(start, end);
	start = MIN(start, end);

	int n = number(start, end);

#ifdef ENABLE_DICE_SYSTEM
	if (ch->GetParty())
		ch->GetParty()->ChatPacketToAllMember(CHAT_TYPE_DICE_INFO, "551 %s %d %d %d", ch->GetName(), n, start, end);
	else
		ch->ChatPacket(CHAT_TYPE_DICE_INFO, "552 %d %d %d", n, start, end);
#else
	if (ch->GetParty())
		ch->GetParty()->ChatPacketToAllMember(CHAT_TYPE_INFO, "551 %s %d %d %d", ch->GetName(), n, start, end);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, "552 %d %d %d", n, start, end);
#endif
}

#ifdef ENABLE_NEWSTUFF
ACMD(do_click_safebox)
{
	if ((ch->GetGMLevel() <= GM_PLAYER) && (ch->GetDungeon() || ch->GetWarMap()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "553");
		return;
	}

	ch->SetSafeboxOpenPosition();
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeSafeboxPassword");
}
ACMD(do_force_logout)
{
	LPDESC pDesc=DESC_MANAGER::instance().FindByCharacterName(ch->GetName());
	if (!pDesc)
		return;
	pDesc->DelayedDisconnect(0);
}
#endif

ACMD(do_click_mall)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeMallPassword");
}

// ACMD(do_click_safebox)
// {
	// ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeSafeboxPassword");
// }

ACMD(do_ride)
{
    dev_log(LOG_DEB0, "[DO_RIDE] start");

    if (ch->IsDead() || ch->IsStun() || ch->IsPolymorphed())
	return;

    int CalcLastAttackSec = (get_dword_time() - ch->GetLastAttackedTime()) / 1000 + 0.5;

    if (CalcLastAttackSec < 10)
    {
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have to wait a few seconds after being attacked!"));
        return;
    }

    // ³»¸®±â
#ifdef ENABLE_MOUNT_SYSTEM
	if(ch->GetMountingVnumM() != 0)
	{
		CMountSystem* MountSystem = ch->GetMountSystem();
		if(MountSystem != NULL)
		{
			bool MState = MountSystem->IsMounting(ch->GetMountingVnumM());
			if(MState == true)
			{
				MountSystem->Unmount(ch->GetMountingVnumM());
				return;
			}
			else
			{
				do_unmount(ch, NULL, 0, 0);
				if (ch->IsHorseRiding())
				    ch->StopRiding(); 
				MountSystem->Mount(ch->GetMountingVnumM());
				return;
			}
		} 
	}
#endif
    {
	if (ch->IsHorseRiding())
	{
	    dev_log(LOG_DEB0, "[DO_RIDE] stop riding");
	    ch->StopRiding();
	    return;
	}

	if (ch->GetMountVnum())
	{
	    dev_log(LOG_DEB0, "[DO_RIDE] unmount");
	    do_unmount(ch, NULL, 0, 0);
	    return;
	}
    }

    // Å¸±â
    {
	if (ch->GetHorse() != NULL)
	{
	    dev_log(LOG_DEB0, "[DO_RIDE] start riding");
	    ch->StartRiding();
	    return;
	}

	for (WORD i=0; i<INVENTORY_MAX_NUM; ++i)
	{
	    LPITEM item = ch->GetInventoryItem(i);
	    if (NULL == item)
		continue;

	    // À¯´ÏÅ© Å»°Í ¾ÆÀÌÅÛ
	/*
		if (item->IsRideItem())
		{
			if (
				NULL==ch->GetWear(WEAR_UNIQUE1)
				|| NULL==ch->GetWear(WEAR_UNIQUE2)
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
				|| NULL==ch->GetWear(WEAR_COSTUME_MOUNT)
#endif
			)
			{
				dev_log(LOG_DEB0, "[DO_RIDE] USE UNIQUE ITEM");
				//ch->EquipItem(item);
				ch->UseItem(TItemPos (INVENTORY, i));
				return;
			}
		}*/

	    // ÀÏ¹Ý Å»°Í ¾ÆÀÌÅÛ
	    // TODO : Å»°Í¿ë SubType Ãß°¡
	    switch (item->GetVnum())
	    {
		case 71114:	// Àú½ÅÀÌ¿ë±Ç
		case 71116:	// »ê°ß½ÅÀÌ¿ë±Ç
		case 71118:	// ÅõÁö¹üÀÌ¿ë±Ç
		case 71120:	// »çÀÚ¿ÕÀÌ¿ë±Ç
		    dev_log(LOG_DEB0, "[DO_RIDE] USE QUEST ITEM");
		    ch->UseItem(TItemPos (INVENTORY, i));
		    return;
	    }

		// GF mantis #113524, 52001~52090 ¹ø Å»°Í
		if( (item->GetVnum() > 52000) && (item->GetVnum() < 52091) )	{
			dev_log(LOG_DEB0, "[DO_RIDE] USE QUEST ITEM");
			ch->UseItem(TItemPos (INVENTORY, i));
		    return;
		}
	}
    }


    // Å¸°Å³ª ³»¸± ¼ö ¾øÀ»¶§
    ch->ChatPacket(CHAT_TYPE_INFO, "398");
}

/*
#ifdef CHANGE_CHANNEL
ACMD(do_change_channel)
{
	if (!ch)
		return;

	if (ch->m_pkTimedEvent)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "407");
		event_cancel(&ch->m_pkTimedEvent);
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "554");
		return;
	}

	short channel;
	str_to_number(channel, arg1);

	if (channel < 1 || channel > 4) {
		ch->ChatPacket(CHAT_TYPE_INFO, "555");
		return;
	}

	if (channel == g_bChannel) {
		ch->ChatPacket(CHAT_TYPE_INFO, "556 %d", g_bChannel);
		return;
	}

	if (g_bChannel == 99) {
		ch->ChatPacket(CHAT_TYPE_INFO, "557");
		return;
	}

	if (ch->GetDungeon()) {
		ch->ChatPacket(CHAT_TYPE_INFO, "558");
		return;
	}
	
	

	ch->ChannelSwitch(channel);
}
#endif
*/

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>
static void PassArguments(std::string inlineargument, std::vector <std::string> & container)
{
	boost::split(container,inlineargument,boost::is_any_of(" "));
}

int text_warning = 0;

struct PickupItems
{
	LPCHARACTER ch;

	bool Weapon;
	bool Armor;
	bool Book;
	bool Stone;
	bool Costume;
	bool Upgrade;
	PickupItems(LPCHARACTER ch, bool Weapon, bool Armor, bool Book, bool Stone, bool Costume, bool Upgrade) : ch(ch), 
	Weapon(Weapon),
	Armor(Armor),
	Book(Book),
	Stone(Stone),
	Costume(Costume),
	Upgrade(Upgrade) {}

	void operator () (LPENTITY ent)
	{
		if (!ent->IsType(ENTITY_ITEM))
			return;

		LPITEM pkItem = (LPITEM) ent;

		if (!pkItem->IsOwnership(ch))
		{
			return;
		}

		if (pkItem)
		{
			if(Weapon && pkItem->GetType() == ITEM_WEAPON){
				ch->PickupItem(pkItem->GetVID());
			}
			if(Armor && pkItem->GetType() == ITEM_ARMOR){
				ch->PickupItem(pkItem->GetVID());
			}

			if (Book && (pkItem->GetType() == ITEM_SKILLBOOK || CItemVnumHelper::IsExtraBook(pkItem->GetVnum())))
			{
				ch->PickupItem(pkItem->GetVID());
			}

			if(Stone && pkItem->GetType() == ITEM_METIN && pkItem->GetSubType() == METIN_NORMAL){
				ch->PickupItem(pkItem->GetVID());
			}
			if(Costume && pkItem->GetType() == ITEM_COSTUME){
				ch->PickupItem(pkItem->GetVID());
			}
			if(Upgrade && pkItem->GetType() == ITEM_MATERIAL && pkItem->GetSubType() == MATERIAL_LEATHER){
				ch->PickupItem(pkItem->GetVID());
			}
		}

	}
};
// tell me item type from book, stone, upgrade, costume

// stone ITEM_METIN	METIN_NORMAL
// costume ITEM_COSTUME	COSTUME_BODY  ITEM_COSTUME	COSTUME_HAIR 
// upgrade ITEM_MATERIAL	MATERIAL_LEATHER
// book ITEM_SKILLBOOK
ACMD(do_pickup_all)
{
	if (ch->IsObserverMode())
		return;

	if (ch->IsDead() || ch->IsStun())
		return;

	std::vector<std::string> row;
	PassArguments(argument, row);
	if((size_t)row.size() != 7) return;

	PickupItems func(ch, 
		strcmp(row[1].c_str(), "Weapon") == 0, 
		strcmp(row[2].c_str(), "Armor") == 0, 
		strcmp(row[3].c_str(), "Book") == 0,
		strcmp(row[4].c_str(), "Stone") == 0, 
		strcmp(row[5].c_str(), "Costume") == 0, 
		strcmp(row[6].c_str(), "Upgrade") == 0
	);

	LPSECTREE sectree = ch->GetSectree();

	if (sectree)
		sectree->ForEachAround(func);
}


ACMD(do_stat_val)
{
	char	arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	int val = 0;
	str_to_number(val, arg2);
	
	if (!*arg1 || val <= 0)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "353");
		return;
	}

	if (ch->GetPoint(POINT_STAT) <= 0)
		return;

	BYTE idx = 0;
	
	if (!strcmp(arg1, "st"))
		idx = POINT_ST;
	else if (!strcmp(arg1, "dx"))
		idx = POINT_DX;
	else if (!strcmp(arg1, "ht"))
		idx = POINT_HT;
	else if (!strcmp(arg1, "iq"))
		idx = POINT_IQ;
	else
		return;

	if (ch->GetRealPoint(idx) >= MAX_STAT)
		return;
	
	if (val > ch->GetPoint(POINT_STAT))
		val = ch->GetPoint(POINT_STAT);
	
	if (ch->GetRealPoint(idx) + val > MAX_STAT)
		val = MAX_STAT - ch->GetRealPoint(idx);

	ch->SetRealPoint(idx, ch->GetRealPoint(idx) + val);
	ch->SetPoint(idx, ch->GetPoint(idx) + val);
	ch->ComputePoints();
	ch->PointChange(idx, 0);

	if (idx == POINT_IQ)
		ch->PointChange(POINT_MAX_HP, 0);
	else if (idx == POINT_HT)
		ch->PointChange(POINT_MAX_SP, 0);

	ch->PointChange(POINT_STAT, -val);
	ch->ComputePoints();
}


ACMD(do_open_change_equip)
{

	char arg1[256];
	one_argument (argument, arg1, sizeof(arg1));
	
	if (0 == arg1[0])
		return;

	int page_index = atoi(arg1);

	if(page_index <= 0 || page_index > CHANGE_EQUIP_PAGE_EXTRA){
		return;
	}

	if(!ch->CanHandleItem()){
		return;
	}

	if (ch->IsDead()){
		return;
	}

	if (ch->IsStun()){
		return;
	}

	if (ch->GetExchange()
		 || ch->GetShop()
		 || ch->IsOpenSafebox()
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
		return;
	}

	DWORD dwCurTime = get_dword_time();

	if (dwCurTime - ch->GetLastAttackTime() <= 1500 || dwCurTime - ch->GetLastSkillTime() <= 1500)
	{
		return;
	}

	DWORD index_old = CHANGE_EQUIP_SLOT_COUNT-(CHANGE_EQUIP_SLOT_COUNT/page_index);

	if(page_index > 1){
		index_old = CHANGE_EQUIP_SLOT_COUNT/CHANGE_EQUIP_PAGE_EXTRA*(page_index-1);
	}

	LPITEM item_change_equip;
	LPITEM item_inv;

	item_inv = ch->GetWear(WEAR_COSTUME_WEAPON);
	if(item_inv){
		item_change_equip = ch->GetChangeEquipItem(index_old+WEAR_WEAPON);
		if(item_change_equip)
		{
			if (item_change_equip->GetType() != ITEM_WEAPON || item_inv->GetValue(3) != item_change_equip->GetSubType())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "Este cambio de equipo no se puede realizar, tu skin de arma actual es incompatible.");
				return;
			}
		}
	}

	for (int i = index_old; i < CHANGE_EQUIP_SLOT_COUNT/CHANGE_EQUIP_PAGE_EXTRA*page_index; ++i)
	{
		item_change_equip = ch->GetChangeEquipItem(i);

		WORD cell = i;

		if(page_index > 1){
			if(cell >= CHANGE_EQUIP_SLOT_COUNT/CHANGE_EQUIP_PAGE_EXTRA*(page_index-1) && cell < (CHANGE_EQUIP_SLOT_COUNT/CHANGE_EQUIP_PAGE_EXTRA)*(page_index))
			{
				cell = cell - ((CHANGE_EQUIP_SLOT_COUNT/CHANGE_EQUIP_PAGE_EXTRA)*(page_index-1));
			}
		}

		item_inv = ch->GetWear(cell);

		if(item_change_equip && item_inv == NULL)
		{
			item_change_equip->EquipTo(ch, item_change_equip->FindEquipCell(ch));
		}

		if(item_change_equip && item_inv)
		{
			item_inv->RemoveFromCharacter();
			if(item_change_equip->EquipTo(ch, item_change_equip->FindEquipCell(ch)))
				item_inv->AddToCharacter(ch, TItemPos(CHANGE_EQUIP, i));
			
		}
	}

}

#ifdef ENABLE_NEW_PET_SYSTEM
#include "PetSystem.h"
ACMD(do_change_name)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 3) { return; }
	int scrollPos = -1;
	int petPos = -1;
	str_to_number(scrollPos, vecArgs[1].c_str());
	str_to_number(petPos, vecArgs[2].c_str());
	if (scrollPos == -1 || petPos == -1 || vecArgs[3].empty() || vecArgs[3].size() < 3 || ch->GetGold() < 100000)
		return;
	LPITEM scroll = ch->GetInventoryItem(scrollPos);
	if (scroll)
	{
		//if ()
		{
			LPITEM pet = ch->GetInventoryItem(petPos);
			if (pet)
			{
				if (pet->IsEquipped())
					return;
				if (pet->GetType() == ITEM_PET && pet->GetSubType() == PET_LEVELABLE && scroll->GetVnum() == 55008)
				{
					ch->SetPoint(POINT_GOLD, -100000);
					scroll->SetCount(scroll->GetCount() - 1);
					//str_replace(vecArgs[3], ";", " ");

					if (!check_name(vecArgs[3].c_str()))
					{
						return;
					}

					pet->SetNewName(vecArgs[3].c_str());
					pet->UpdatePacket();
					ch->ChatPacket(CHAT_TYPE_INFO, "559");
				}
			}
		}
	}
}
ACMD(do_pet_egg)
{
	if (ch->GetGold() < 100000)
		return;

	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	int petPos = -1;
	str_to_number(petPos, vecArgs[1].c_str());

	LPITEM item = ch->GetInventoryItem(petPos);
	if (!item)
	{
		return;
	}
	else if (!(item->GetType() == ITEM_PET && item->GetSubType() == PET_EGG))
	{
		return;
	}
	else if (!check_name(vecArgs[2].c_str()))
	{
		return;
	}

	LPITEM petItem = ch->AutoGiveItem(item->GetValue(0));
	if (!petItem)
		return;

	ch->PointChange(POINT_GOLD, -100000);

	item->SetCount(item->GetCount()-1);
	petItem->SetNewName(vecArgs[2].c_str());
	petItem->SetSocket(0,time(0)+60*60*24*30);
	petItem->SetSocket(1, 1);
	for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
		petItem->SetForceAttribute(j, 99, 0);
	petItem->Save();
	petItem->UpdatePacket();
}
ACMD(do_pet_evolve)
{
	LPITEM item = ch->GetWear(WEAR_PET);
	if(!item)
		return;
	long level = item->GetSocket(1);
	if (level != 40 && level != 75 && level != 100)
		return;
	CPetSystem* pet = ch->GetPetSystem();
	if (!pet)
		return;
	LPPET petActor = pet->GetNewPet();
	if (!petActor)
		return;
	petActor->IncreaseEvolve();
}
ACMD(do_pet_add_slot)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	int skillSlot = -1;
	str_to_number(skillSlot, vecArgs[1].c_str());
	if (skillSlot >= 0 && skillSlot <= 14)
	{
		CPetSystem* pet = ch->GetPetSystem();
		if (!pet)
			return;
		LPPET petActor = pet->GetNewPet();
		if (!petActor)
			return;
		LPITEM item = petActor->GetSummonItem();
		if (!item)
			return;
		if (item->GetSocket(1) < 115)
			return;

		BYTE petSlotMax = pet->PetItemSocketMax(item->GetVnum());

		if (!(skillSlot >= 0 && skillSlot <= petSlotMax))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "1001");
			return;
		}

		int nextIndex = -1;
		for (int j = 3; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
		{
			if (item->GetAttributeType(j) == 99) {
				nextIndex = j;
				break;
			}
		}

		if (nextIndex < 0)
			return;

		DWORD slotItems[][2] = {
			{55036,1},
		};
		for (BYTE j=0;j<_countof(slotItems);++j)
		{
			if (ch->CountSpecifyItem(slotItems[j][0]) < slotItems[j][1])
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "560 I%d %d", slotItems[j][0], slotItems[j][1]);
				return;
			}
		}
		for (BYTE j = 0; j < _countof(slotItems); ++j)
			ch->RemoveSpecifyItem(slotItems[j][0],slotItems[j][1]);
		ch->ChatPacket(CHAT_TYPE_INFO, "561");
		petActor->PointChange(POINT_PET_SKILL_INDEX_1 + nextIndex, 0);
	}
}
#endif

//#ifdef ENABLE_CHANNEL_CHANGE
//ACMD(do_move_channel)
//{
//	std::vector<std::string> vecArgs;
//	split_argument(argument, vecArgs);
//	if (vecArgs.size() < 2) { return; }
//	DWORD channelIndex = 0;
//	str_to_number(channelIndex, vecArgs[1].c_str());
//
//	//check mapindex
//	switch(ch->GetMapIndex())
//	{
//		// you need modify here. example you don't want dont change channel mapindex
//		//81 113 110 111 114 118 119 120 121 122 123 124 125 126 127 128 181 182 183 24 44 211 92 90 91
//		case 113://ox
//		case 81:
//		case 110:
//		case 111:
//		case 114:
//		case 118:
//		case 119:
//		case 120:
//		case 121:
//		case 122:
//		case 123:
//		case 124:
//		case 125:
//		case 126:
//		case 127:
//		case 128:
//		case 181:
//		case 182:
//		case 183:
//		case 24:
//		case 44:
//		case 211:
//		case 90:
//		case 91:
//		case 92:
//			ch->ChatPacket(CHAT_TYPE_INFO, "999");
//			return;
//		break;
//	}
//	if(channelIndex > 8)
//		return;
//	ch->MoveChannel(channelIndex);
//}
//#endif


#ifdef ENABLE_BIYOLOG
#include "shop_manager.h"
ACMD(do_open_shop)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }

	DWORD shopVnum = 0;
	str_to_number(shopVnum, vecArgs[1].c_str());
	CShopManager::Instance().StartShopping(ch, NULL, shopVnum);
}
ACMD(do_bio)
{
	if (ch->GetLevel() < 30)
		return;

	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }

	int level = ch->GetQuestFlag("bio.level");

	if (level >= bio_max)
		return;
	else if (level < 1)
		return;
	else if (ch->GetLevel() < bio_data[level][0])
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "562", bio_data[level][0]);
		return;
	}

	int count = ch->GetQuestFlag("bio.count");
	int time = ch->GetQuestFlag("bio.time");

	if (vecArgs[1] == "mission")
	{
		if (vecArgs.size() < 4) { return; }
		BYTE isOzut = 0;
		BYTE isUnutkanlik = 0;

		str_to_number(isOzut, vecArgs[2].c_str());
		str_to_number(isUnutkanlik, vecArgs[3].c_str());

		if (count < bio_data[level][2])
		{
			if (ch->CountSpecifyItem(bio_data[level][1]) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "563 I%d", bio_data[level][1]);
				return;
			}

			//bool isUnutkanlikDrink = false;

			if (isUnutkanlik)
			{
				if (ch->CountSpecifyItem(bio_data[level][16]) > 0 && time > get_global_time())//fix
				//if (ch->CountSpecifyItem(bio_data[level][16]) > 0)
				{
					ch->RemoveSpecifyItem(bio_data[level][16], 1);
					//isUnutkanlikDrink = true;
				}
				else
				{
					if (time > get_global_time())
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "564 I%d", bio_data[level][16]);
						return;
					}
				}
			}
			else
			{
				if (time > get_global_time())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "564 I%d",bio_data[level][16]);
					return;
				}
			}


			bool isOzutDrink = false;
			if (isOzut)
			{
				if (ch->CountSpecifyItem(bio_data[level][15]) > 0)
				{
					ch->RemoveSpecifyItem(bio_data[level][15], 1);
					isOzutDrink = true;
				}
			}
			
			int prob = isOzutDrink ? bio_data[level][4] + 50 : bio_data[level][4];
			ch->RemoveSpecifyItem(bio_data[level][1], 1);
			if (prob >= number(1, 100))
			{
				count += 1;
				time = get_global_time() + bio_data[level][3];
				ch->SetQuestFlag("bio.count", count);
				ch->SetQuestFlag("bio.time", time);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biodata %d %d %d", level, count, time);

				ch->ChatPacket(CHAT_TYPE_INFO, "565 I%d", bio_data[level][1]);
			}
			else
			{
				time = get_global_time() + bio_data[level][3];
				ch->SetQuestFlag("bio.time", time);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biodata %d %d %d", level, count, time);

				ch->ChatPacket(CHAT_TYPE_INFO, "566 I%d", bio_data[level][1]);
			}

			if (bio_data[level][5] != 0)
			{
				if (count == bio_data[level][2])
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biostone %d", level);
				return;
			}
			
			
		}

		if (bio_data[level][5] != 0)
		{
			if (count == bio_data[level][2])
			{
				if (ch->CountSpecifyItem(bio_data[level][5]) < 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "567 I%d", bio_data[level][5]);
					return;
				}
				else
				{
					ch->RemoveSpecifyItem(bio_data[level][5], 1);
					ch->SetQuestFlag("bio.count", count+1);

					if (bio_data[level][14] == 0)
					{
						ch->SetQuestFlag("bio.count", 0);
						ch->SetQuestFlag("bio.level", level + 1);
						ch->SetQuestFlag("bio.time", 0);

						if (bio_data[level][6] != 0)
						{
							long value = bio_data[level][7];
							CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][6]);
							if (affect) {
								value += affect->lApplyValue;
								ch->RemoveAffect(affect);
							}
							ch->AddAffect(AFFECT_COLLECT, bio_data[level][6], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

							ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][6]), bio_data[level][7]);
							//ch->ChatPacket(CHAT_TYPE_INFO, "569");
						}

						if (bio_data[level][8] != 0)
						{
							long value = bio_data[level][9];
							CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][8]);
							if (affect) {
								value += affect->lApplyValue;
								ch->RemoveAffect(affect);
							}
							ch->AddAffect(AFFECT_COLLECT, bio_data[level][8], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

							ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][8]), bio_data[level][9]);
							//ch->ChatPacket(CHAT_TYPE_INFO, "569");
						}

						if (bio_data[level][10] != 0)
						{
							long value = bio_data[level][11];
							CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][10]);
							if (affect) {
								value += affect->lApplyValue;
								ch->RemoveAffect(affect);
							}
							ch->AddAffect(AFFECT_COLLECT, bio_data[level][10], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

							ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][10]), bio_data[level][11]);
							//ch->ChatPacket(CHAT_TYPE_INFO, "569");
						}

						if (bio_data[level][12] != 0)
						{
							long value = bio_data[level][13];
							CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][12]);
							if (affect) {
								value += affect->lApplyValue;
								ch->RemoveAffect(affect);
							}
								
							ch->AddAffect(AFFECT_COLLECT, bio_data[level][12], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

							ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][12]), bio_data[level][13]);
							//ch->ChatPacket(CHAT_TYPE_INFO, "569");
						}

						int newLevel = level + 1;
						if (newLevel >= bio_max)
						{
							ch->ChatPacket(CHAT_TYPE_INFO, "570");
							ch->ChatPacket(CHAT_TYPE_COMMAND, "bioempty");
							return;
						}
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biodata %d %d %d", newLevel,0,0);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_COMMAND, "bioodul %d", level);
						return;
					}
				}
			}
			else if (count == bio_data[level][2]+1)
				return;
		}
		else
		{
			if (count == bio_data[level][2])
			{
				if (bio_data[level][14] == 0)
				{
					ch->SetQuestFlag("bio.count", 0);
					ch->SetQuestFlag("bio.level", level + 1);
					ch->SetQuestFlag("bio.time", 0);

					if (bio_data[level][6] != 0)
					{
						long value = bio_data[level][7];
						CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][6]);
						if (affect) {
							value += affect->lApplyValue;
							ch->RemoveAffect(affect);
						}
						ch->AddAffect(AFFECT_COLLECT, bio_data[level][6], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

						ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][6]), bio_data[level][7]);
						//ch->ChatPacket(CHAT_TYPE_INFO, "569");
					}

					if (bio_data[level][8] != 0)
					{
						long value = bio_data[level][9];
						CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][8]);
						if (affect) {
							value += affect->lApplyValue;
							ch->RemoveAffect(affect);
						}
						ch->AddAffect(AFFECT_COLLECT, bio_data[level][8], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

						ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][8]), bio_data[level][9]);
						//ch->ChatPacket(CHAT_TYPE_INFO, "569");
					}

					if (bio_data[level][10] != 0)
					{
						long value = bio_data[level][11];
						CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][10]);
						if (affect) {
							value += affect->lApplyValue;
							ch->RemoveAffect(affect);
						}
						ch->AddAffect(AFFECT_COLLECT, bio_data[level][10], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

						ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][10]), bio_data[level][11]);
						//ch->ChatPacket(CHAT_TYPE_INFO, "569");
					}

					if (bio_data[level][12] != 0)
					{
						long value = bio_data[level][13];
						CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][12]);
						if (affect) {
							value += affect->lApplyValue;
							ch->RemoveAffect(affect);
						}
						ch->AddAffect(AFFECT_COLLECT, bio_data[level][12], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

						ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][12]), bio_data[level][13]);
						//ch->ChatPacket(CHAT_TYPE_INFO, "569");
					}

					int newLevel = level + 1;
					if (newLevel >= bio_max)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "570");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "bioempty");
						return;
					}
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biodata %d %d %d", newLevel, 0, 0);
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_COMMAND, "bioodul %d", level);
					return;
				}
			}
		}
	}
	else if (vecArgs[1] == "gift")
	{
		if (vecArgs.size() < 3) { return; }
		BYTE index = 0;
		str_to_number(index, vecArgs[2].c_str());

		if (index > 3)
			return;

		if (bio_data[level][5] != 0)
		{
			if (count != bio_data[level][2] + 1)
				return;
		}
		else
		{
			if (count != bio_data[level][2])
				return;
		}

		ch->SetQuestFlag("bio.count", 0);
		ch->SetQuestFlag("bio.level", level + 1);
		ch->SetQuestFlag("bio.time", 0);

		if (bio_data[level][6] != 0 && index == 0)
		{
			long value = bio_data[level][7];
			CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][6]);
			if (affect) {
				value += affect->lApplyValue;
				ch->RemoveAffect(affect);
			}
			ch->AddAffect(AFFECT_COLLECT, bio_data[level][6], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

			ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][6]), bio_data[level][7]);
			//ch->ChatPacket(CHAT_TYPE_INFO, "569");
		}

		if (bio_data[level][8] != 0 && index == 1)
		{
			long value = bio_data[level][9];
			CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][8]);
			if (affect) {
				value += affect->lApplyValue;
				ch->RemoveAffect(affect);
			}
			ch->AddAffect(AFFECT_COLLECT, bio_data[level][8], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

			ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][8]), bio_data[level][9]);
			//ch->ChatPacket(CHAT_TYPE_INFO, "569");
		}

		if (bio_data[level][10] != 0 && index == 2)
		{
			long value = bio_data[level][11];
			CAffect* affect = ch->FindAffect(AFFECT_COLLECT, bio_data[level][10]);
			if (affect) {
				value += affect->lApplyValue;
				ch->RemoveAffect(affect);
			}
			ch->AddAffect(AFFECT_COLLECT, bio_data[level][10], value, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);

			ch->ChatPacket(CHAT_TYPE_INFO, "568 E%d:%d", pointToApply(bio_data[level][10]), bio_data[level][11]);
			//ch->ChatPacket(CHAT_TYPE_INFO, "569");
		}

		char flag[100];
		sprintf(flag, "bio.bonus%d", level);
		ch->SetQuestFlag(flag, index + 1);

		int newLevel = level + 1;
		if (newLevel >= bio_max)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "570");
			ch->ChatPacket(CHAT_TYPE_COMMAND, "bioempty");
			CHARACTER_MANAGER::Instance().SetRewardData(REWARD_FIRSTBIO, ch->GetName(), true);

			return;
		}

		ch->ChatPacket(CHAT_TYPE_COMMAND, "biodata %d %d %d", newLevel, 0, 0);
	}
}
#endif

#ifdef DISABLE_OLD_MINUS
ACMD(do_stat_minus_new)
{
	char	arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	int val = 0;
	str_to_number(val, arg2);

	if (!*arg1 || val <= 0)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "353");
		return;
	}

	BYTE idx = 0;

	if (!strcmp(arg1, "st"))
	{
		idx = POINT_ST;
		if (ch->GetRealPoint(idx) - val <= JobInitialPoints[ch->GetJob()].st)
			val = 1;
		if (ch->GetRealPoint(idx) <= JobInitialPoints[ch->GetJob()].st)
			return;
	}
	else if (!strcmp(arg1, "dx"))
	{
		idx = POINT_DX;
		if (ch->GetRealPoint(idx) - val <= JobInitialPoints[ch->GetJob()].dx)
			val = 1;
		if (ch->GetRealPoint(idx) <= JobInitialPoints[ch->GetJob()].dx)
			return;
	}
	else if (!strcmp(arg1, "ht"))
	{
		idx = POINT_HT;
		if (ch->GetRealPoint(idx) - val <= JobInitialPoints[ch->GetJob()].ht)
			val = 1;
		if (ch->GetRealPoint(idx) <= JobInitialPoints[ch->GetJob()].ht)
			return;
	}
	else if (!strcmp(arg1, "iq"))
	{
		idx = POINT_IQ;
		if (ch->GetRealPoint(idx) - val <= JobInitialPoints[ch->GetJob()].iq)
			val = 1;
		if (ch->GetRealPoint(idx) <= JobInitialPoints[ch->GetJob()].iq)
			return;
	}
	else
		return;

	ch->SetRealPoint(idx, ch->GetRealPoint(idx) - val);
	ch->SetPoint(idx, ch->GetPoint(idx) - val);
	ch->ComputePoints();
	ch->PointChange(idx, 0);

	ch->PointChange(POINT_STAT, val);
	ch->ComputePoints();
}
#endif

#ifdef ENABLE_BATTLE_PASS
ACMD(do_battle_pass)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "info")
	{
		ch->SetProtectTime("battlePassOpen", 1);
		CHARACTER_MANAGER::Instance().LoadBattlePassData(ch);
	}
	else if (vecArgs[1] == "reward")
		CHARACTER_MANAGER::Instance().CheckBattlePassReward(ch);
}
#endif


#ifdef ENABLE_ITEMSHOP
ACMD(do_ishop)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);

	if (vecArgs.size() < 2)
	{
		return;
	}
	else if (vecArgs[1] == "data")
	{
		if (ch->GetProtectTime("itemshop.load") == 1)
		{
			return;
		}

		ch->SetProtectTime("itemshop.load", 1);

		if (vecArgs.size() < 3)
		{
			return;
		}

		int updateTime;
		str_to_number(updateTime, vecArgs[2].c_str());

		CHARACTER_MANAGER::Instance().LoadItemShopData(ch, CHARACTER_MANAGER::Instance().GetItemShopUpdateTime() != updateTime);
	}
	else if (vecArgs[1] == "log")
	{
		if (ch->GetProtectTime("itemshop.log") == 1)
		{
			return;
		}

		ch->SetProtectTime("itemshop.log", 1);

		CHARACTER_MANAGER::Instance().LoadItemShopLog(ch);
	}
	else if (vecArgs[1] == "buy")
	{
		if (vecArgs.size() < 4)
		{
			return;
		}

		int itemID;
		str_to_number(itemID, vecArgs[2].c_str());

		int itemCount;
		str_to_number(itemCount, vecArgs[3].c_str());

/* check already in: LoadItemShopBuy
		if (itemCount < 1 || itemCount > 20)
		{
			return;
		}
*/
		CHARACTER_MANAGER::Instance().LoadItemShopBuy(ch, itemID, itemCount);
	}
	else if (vecArgs[1] == "wheel")
	{
		if (vecArgs.size() < 3)
		{
			return;
		}
		else if (vecArgs[2] == "start")
		{
			if (vecArgs.size() < 4)
			{
				return;
			}

			BYTE ticketType;
			if (!str_to_number(ticketType, vecArgs[3].c_str()))
			{
				return;
			}

			if (ticketType > 1)
			{
				return;
			}
			else if (ch->GetProtectTime("WheelWorking") != 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "998");
				return;
			}

			if (ticketType == 0)
			{
				if (ch->CountSpecifyItem(80013) <= 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "996");
					return;
				}

				ch->RemoveSpecifyItem(80013, 1);
			}
			else if (ticketType == 1)
			{
				long long act_lldCoins;
#ifdef USE_ITEMSHOP_RENEWED
				long long act_lldJCoins;
				ch->GetAccountMoney(act_lldCoins, act_lldJCoins);
#else
				ch->GetAccountMoney(act_lldCoins);
#endif

				if (act_lldCoins < 10)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "997");
					return;
				}

#ifdef USE_ITEMSHOP_RENEWED
				ch->SetAccountMoney(10, 0, false);
				ch->GetAccountMoney(act_lldCoins, act_lldJCoins);
#else
				ch->SetAccountMoney(10, false);
				ch->GetAccountMoney(act_lldCoins);
#endif

				LPDESC d = ch->GetDesc();
				if (d)
				{
					LogManager::Instance().WheelOfFortuneLog(d->GetAccountTable().id, ch->GetPlayerID(), ch->GetMapIndex(), ch->GetX(), ch->GetY(), 10);

					if (ch->GetProtectTime("itemshop.log") == 1)
					{
						char szIP[16] = {};
						strlcpy(szIP, d->GetHostName(), sizeof(szIP));

						char szTime[21];
						time_t now = time(0);
						struct tm tstruct = *localtime(&now);
						strftime(szTime, sizeof(szTime), "%Y-%m-%d %X", &tstruct);

						ch->ChatPacket(CHAT_TYPE_COMMAND, "ItemShopAppendLog %s %d %s %s 1 1 5", szTime, time(0), ch->GetName(), szIP);
					}
				}
			}

			// Important items
			std::vector<std::pair<long, long>> m_important_item = {
				{25042,1},
				{53011,1},
				{53999,1},
				{54715,1},
				{60301,1},
				{30618,1},
				{33031,10},
				{71509,1},
				{60300,1},
				{20574,1},
				{71097,1},
			};

			// normal items
			std::map<std::pair<long, long>, int> m_normal_item = {
				{{71084,50},30},
				 {{71084,100},30},
				 {{71084,100},30},
				 {{71084,200},30},
				 {{71084,200},30},
				 {{71084,1000},30},
				 {{6884,1},30},
				 {{6884,3},30},
				 {{6884,5},30},
				 {{6884,7},30},
				 {{6884,11},30},
				 {{70005,1},30},
				 {{55035,1},30},
				 {{25041,1},30},
				 {{25041,1},10},
				 {{72346,1},30},
				 {{72346,2},15},
				 {{55009,1},20},
				 {{72320,1},30},
				 {{72320,2},30},
				 {{72320,1},30},
				 {{72320,2},30},
				 {{72320,5},30},
				 {{55001,1},30},
				 {{71035,1},30},
				 {{72346,3},10},
				 {{6884,8},30},
				 {{6884,10},30},
				 {{50255,50},10},
				 {{50255,30},20},
				 {{50255,20},30},
				 {{51001,100},30},
				 {{72067,1},30},
				 {{72067,2},30},
				 {{72067,3},10},
				 {{6884,12},30},
			};

			std::vector<std::pair<long, long>> m_send_items;
			if (m_important_item.size())
			{
				int random = number(0,m_important_item.size()-1);
				m_send_items.emplace_back(m_important_item[random].first, m_important_item[random].second);
			}

			while (true)
			{
				for (auto it = m_normal_item.begin(); it != m_normal_item.end(); ++it)
				{
					int randomEx = number(0,4);
					if (randomEx == 4)
					{
						int random = number(0,100);
						if (it->second >= random)
						{
							auto itFind = std::find(m_send_items.begin(), m_send_items.end(), it->first);
							if (itFind == m_send_items.end())
							{
								m_send_items.emplace_back(it->first.first, it->first.second);
								if (m_send_items.size() >= 10)
								{
									break;
								}
							}
						}
					}
				}

				if (m_send_items.size() >= 10)
				{
					break;
				}
			}

			std::string cmd_wheel = "";

			if (!m_send_items.empty())
			{
				for (auto it = m_send_items.begin(); it != m_send_items.end(); ++it)
				{
					cmd_wheel += std::to_string(it->first);
					cmd_wheel += "|";
					cmd_wheel += std::to_string(it->second);
					cmd_wheel += "#";
				}
			}

			int luckyWheel = number(0, 9);
			{
				if (luckyWheel == 0)
				{
					if (number(0, 1) == 0)
					{
						luckyWheel = number(0, 9);
					}
				}
			}

			ch->SetProtectTime("WheelLuckyIndex", luckyWheel);
			ch->SetProtectTime("WheelLuckyItemVnum", m_send_items[luckyWheel].first);
			ch->SetProtectTime("WheelLuckyItemCount", m_send_items[luckyWheel].second);
			ch->SetProtectTime("WheelWorking", 1);

			ch->ChatPacket(CHAT_TYPE_COMMAND, "SetWheelItemData %s", cmd_wheel.c_str());
			ch->ChatPacket(CHAT_TYPE_COMMAND, "OnSetWhell %d", luckyWheel);
		}
		else if (vecArgs[2] == "done")
		{
			if (ch->GetProtectTime("WheelWorking") == 0)
			{
				return;
			}

			ch->AutoGiveItem(ch->GetProtectTime("WheelLuckyItemVnum"), ch->GetProtectTime("WheelLuckyItemCount"));

			ch->ChatPacket(CHAT_TYPE_COMMAND, "GetWheelGiftData %d %d", ch->GetProtectTime("WheelLuckyItemVnum"), ch->GetProtectTime("WheelLuckyItemCount"));

			ch->SetProtectTime("WheelLuckyIndex", 0);
			ch->SetProtectTime("WheelLuckyItemVnum", 0);
			ch->SetProtectTime("WheelLuckyItemCount", 0);
			ch->SetProtectTime("WheelWorking", 0);
		}
	}
	else if (vecArgs[1] == "currency")
	{
		ch->RefreshAccountMoney();
	}
}
#endif

#ifdef ENABLE_EVENT_MANAGER
ACMD(do_event_manager)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "info")
	{
		CHARACTER_MANAGER::Instance().SendDataPlayer(ch);
	}
	else if (vecArgs[1] == "remove")
	{
		if (!ch->IsGM())
			return;

		if (vecArgs.size() < 3) { 
			
			ch->ChatPacket(CHAT_TYPE_INFO, "put the event index!!");
			return; 
		}

		BYTE removeIndex;
		str_to_number(removeIndex, vecArgs[2].c_str());

		if(CHARACTER_MANAGER::Instance().CloseEventManuel(removeIndex))
			ch->ChatPacket(CHAT_TYPE_INFO, "successfuly remove!");
		else
			ch->ChatPacket(CHAT_TYPE_INFO, "dont has any event!");
	}
	else if (vecArgs[1] == "update")
	{
		if (!ch->IsGM())
			return;
		const BYTE subHeader = EVENT_MANAGER_UPDATE;
		//db_clientdesc->DBPacketHeader(HEADER_GD_EVENT_MANAGER, 0, sizeof(BYTE));
		//db_clientdesc->Packet(&subHeader, sizeof(BYTE));
		db_clientdesc->DBPacket(HEADER_GD_EVENT_MANAGER, 0, &subHeader, sizeof(BYTE));

		ch->ChatPacket(CHAT_TYPE_INFO, "successfully update!");
	}
}
#endif

#ifdef ENABLE_AUTO_SELECT_SKILL
ACMD(do_skillauto)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "select")
	{
		if (vecArgs.size() < 3) { return; }
		if (ch->GetSkillGroup() != 0)
			return;
		BYTE skillIndex;
		str_to_number(skillIndex, vecArgs[2].c_str());
		if(skillIndex > 2)
			return;
		ch->RemoveGoodAffect();
		ch->SetSkillGroup(skillIndex);
		ch->ClearSkill();
		ch->SetSkillLevel(122,2);
		ch->SetSkillLevel(124,50);
		ch->SetSkillLevel(126,50);
		ch->SetSkillLevel(127,50);
		ch->SetSkillLevel(128,50);
		ch->SetSkillLevel(129,50);
		ch->SetSkillLevel(130,21);
		ch->SetSkillLevel(131,20);
		ch->SetSkillLevel(137,20);
		ch->SetSkillLevel(138,20);
		ch->SetSkillLevel(139,20);
		ch->SkillLevelPacket();
	}
}
#endif

#ifdef ENABLE_SHOW_CHEST_DROP
ACMD(do_chestdrop)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	DWORD chestIndex;
	str_to_number(chestIndex, vecArgs[1].c_str());
	ch->SendChestData(chestIndex);
}
#endif


#ifdef ENABLE_ANTI_EXP
ACMD(do_anti_exp)
{
	time_t real_time = time(0);
	if (ch->GetProtectTime("anti.exp") > real_time)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "1068 %d", ch->GetProtectTime("anti.exp") - real_time);
		return;
	}
	ch->SetProtectTime("anti.exp", real_time + 3);
	ch->SetAntiExp(!ch->GetAntiExp());
	ch->ChatPacket(CHAT_TYPE_COMMAND, "SetAntiExp %d", ch->GetAntiExp()?1:0);
}
#endif

#ifdef ENABLE_MULTI_FARM_BLOCK
ACMD(do_multi_farm)
{
	if (!ch->GetDesc())
		return;
	if (ch->GetProtectTime("multi-farm") > get_global_time())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You need be slow! You can try after %d second.", ch->GetProtectTime("multi-farm") - get_global_time());
		return;
	}
	ch->SetProtectTime("multi-farm", get_global_time() + 10);
	CHARACTER_MANAGER::Instance().CheckMultiFarmAccount(ch->GetDesc()->GetHostName(), ch->GetPlayerID(),ch->GetName(), !ch->GetRewardStatus());
}
#endif


#ifdef ENABLE_MULTI_CHEST
ACMD(do_chest_drop)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 4) { return; }
	BYTE invWindow;
	str_to_number(invWindow, vecArgs[1].c_str());
	WORD invCell;
	str_to_number(invCell, vecArgs[2].c_str());
	WORD openCount;
	str_to_number(openCount, vecArgs[3].c_str());

	LPITEM item = ch->GetItem(TItemPos(invWindow, invCell));
	if (!item
		 || item->GetType() != ITEM_GIFTBOX
		 || item->isLocked()
		 || item->IsExchanging())
	{
		return;
	}

	item->Lock(true);

	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		item->Lock(false);
		return;
	}

	if (ch->m_pkTimedEvent
		 || ch->IsDead()
		 || ch->IsObserverMode())
	{
		item->Lock(false);

		ch->ChatPacket(CHAT_TYPE_INFO, "1570");
		// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this when you're dead or in observer mode."));
		return;
	}

	if (ch->IsWarping()
		 || !ch->CanWarp()
		 || ch->GetSafebox()
/* checked in: CanWarp
		 || ch->GetShop()
		 || ch->GetExchange()
		 || ch->IsCubeOpen()
		 || ch->IsOpenSafebox()
#ifdef ENABLE_IKASHOP_RENEWAL
		 || ch->GetIkarusShopGuest()
		 || ch->GetIkarusAuctionGuest()
#endif
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
		 || (!bSkipShardCraft && IsShardCraftOpened())
#endif
*/
	)
	{
		item->Lock(false);

		ch->ChatPacket(CHAT_TYPE_INFO, "1571");
		ch->ChatPacket(CHAT_TYPE_INFO, "1572");
		// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this right now, if you just had opened another system"));
		// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("or you just exchanged, please wait 10 seconds and retry."));
		return;
	}

	quest::PC* pPC = quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID());
	if (!pPC
		 || pPC->IsRunning())
	{
		item->Lock(false);

		ch->ChatPacket(CHAT_TYPE_INFO, "1573");
		ch->ChatPacket(CHAT_TYPE_INFO, "1574");
		// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this right now because you have a quest opened."));
		// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("If you don't, please try to re-login."));
		return;
	}

	item->Lock(false);
	ch->OpenChest(item, openCount);
}
#endif

#ifdef ENABLE_ZODIAC_MISSION
ACMD(do_cz_reward)
{
	if (ch->GetProtectTime("Zodiac12ZiReward") > get_global_time())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Biraz beklemelisin.");
		return;
	}
	ch->SetProtectTime("Zodiac12ZiReward",get_global_time()+1);

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Eksik islem uygulandi!");
		return;
	}

	BYTE type = 0;
	str_to_number(type, arg1);
	int FYellow = ch->GetQuestFlag("Quest_ZodiacTemple.YellowReward");
	int FGreen = ch->GetQuestFlag("Quest_ZodiacTemple.GreenReward");

	if (type == 1)
	{
		if (ch->GetQuestFlag("Quest_ZodiacTemple.YellowMark") == 1073741823)
		{
			ch->AutoGiveItem(33026, 1);
			ch->SetQuestFlag("Quest_ZodiacTemple.YellowMark",0);
			ch->SetQuestFlag("Quest_ZodiacTemple.YellowReward",FYellow+1);
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince sari sanduka puanin yok!");
			return;
		}
	}
	else if (type == 2)
	{
		if (ch->GetQuestFlag("Quest_ZodiacTemple.GreenMark") == 1073741823)
		{
			ch->AutoGiveItem(33027, 1);
			ch->SetQuestFlag("Quest_ZodiacTemple.GreenReward",FGreen+1);
			ch->SetQuestFlag("Quest_ZodiacTemple.GreenMark",0);
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince Yesil sanduka puanin yok!");
			return;
		}
	}
	else if (type == 3)
	{
		if(FYellow >= 1 && FGreen >= 1)
		{
			ch->AutoGiveItem(33028, 1);
			ch->SetQuestFlag("Quest_ZodiacTemple.YellowReward",FYellow-1);
			ch->SetQuestFlag("Quest_ZodiacTemple.GreenReward",FGreen-1);
		}
	}
	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenUI12zi %d %d %d %d", ch->GetQuestFlag("Quest_ZodiacTemple.YellowMark"), ch->GetQuestFlag("Quest_ZodiacTemple.GreenMark"), ch->GetQuestFlag("Quest_ZodiacTemple.YellowReward"), ch->GetQuestFlag("Quest_ZodiacTemple.GreenReward"));
}

ACMD(do_cz_check_box)
{
	if (!ch)
		return;


	if (ch->GetProtectTime("Zodiac12ZiTable") > get_global_time())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Biraz beklemelisin.");
		return;
	}
	ch->SetProtectTime("Zodiac12ZiTable",get_global_time()+1);

	char arg1[256];
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Eksik islem uygulandi!");
		return;
	}

	BYTE type = 0, value = 0, zero = 0;
	str_to_number(type, arg1);
	str_to_number(value, arg2);

	DWORD column_item_list_yellow[] = { 33001, 33003, 33005, 33007, 33009, 33011 };
	DWORD column_item_list_green[] = { 33002, 33004, 33006, 33008, 33010, 33012 };

	DWORD row_item_list_yellow[] = { 33013, 33015, 33017, 33019, 33021 };
	DWORD row_item_list_green[] = { 33014, 33016, 33018, 33020, 33022 };

	{
		if (type == 0)
		{
			if (value == 0 || value == 6 || value == 12 || value == 18 || value == 24)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[0]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[0], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value == 1 || value == 7 || value == 13 || value == 19 || value == 25)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[1]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[1], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value == 2 || value == 8 || value == 14 || value == 20 || value == 26)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[2]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[2], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value == 3 || value == 9 || value == 15 || value == 21 || value == 27)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[3]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[3], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value == 4 || value == 10 || value == 16 || value == 22 || value == 28)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[4]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[4], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value == 5 || value == 11 || value == 17 || value == 23 || value == 29)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[5]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[5], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}

			/////////////////////////////////////////////////////////////////////////////////
			if (value >= zero && value <= 5)
			{
				if (ch->CountSpecifyItem(row_item_list_yellow[0]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_yellow[0], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value >= 6 && value <= 11)
			{
				if (ch->CountSpecifyItem(row_item_list_yellow[1]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_yellow[1], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value >= 12 && value <= 17)
			{
				if (ch->CountSpecifyItem(row_item_list_yellow[2]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_yellow[2], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value >= 18 && value <= 23)
			{
				if (ch->CountSpecifyItem(row_item_list_yellow[3]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_yellow[3], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value >= 24 && value <= 29)
			{
				if (ch->CountSpecifyItem(row_item_list_yellow[4]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_yellow[4], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
		}
		else///////////////////////////////////////////////////////////////////////////////////
		{
			if (value == 0 || value == 6 || value == 12 || value == 18 || value == 24)
			{
				if (ch->CountSpecifyItem(column_item_list_green[0]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[0], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value == 1 || value == 7 || value == 13 || value == 19 || value == 25)
			{
				if (ch->CountSpecifyItem(column_item_list_green[1]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[1], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value == 2 || value == 8 || value == 14 || value == 20 || value == 26)
			{
				if (ch->CountSpecifyItem(column_item_list_green[2]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[2], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value == 3 || value == 9 || value == 15 || value == 21 || value == 27)
			{
				if (ch->CountSpecifyItem(column_item_list_green[3]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[3], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value == 4 || value == 10 || value == 16 || value == 22 || value == 28)
			{
				if (ch->CountSpecifyItem(column_item_list_green[4]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[4], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value == 5 || value == 11 || value == 17 || value == 23 || value == 29)
			{
				if (ch->CountSpecifyItem(column_item_list_green[5]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[5], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}

			/////////////////////////////////////////////////////////////////////////////////
			if (value >= zero && value <= 5)
			{
				if (ch->CountSpecifyItem(row_item_list_green[0]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_green[0], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value >= 6 && value <= 11)
			{
				if (ch->CountSpecifyItem(row_item_list_green[1]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_green[1], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value >= 12 && value <= 17)
			{
				if (ch->CountSpecifyItem(row_item_list_green[2]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_green[2], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value >= 18 && value <= 23)
			{
				if (ch->CountSpecifyItem(row_item_list_green[3]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_green[3], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
			else if (value >= 24 && value <= 29)
			{
				if (ch->CountSpecifyItem(row_item_list_green[4]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_green[4], 50);
				else
				{
					 ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterince zodyak tilsimin yok!"));
					return;
				}
			}
		}
	}

	int size = 1;
	for (BYTE b = 0; b < value; ++b)
		size *= 2;

	if (type == 0)
		ch->SetQuestFlag("Quest_ZodiacTemple.YellowMark",ch->GetQuestFlag("Quest_ZodiacTemple.YellowMark")+size);
	else
		ch->SetQuestFlag("Quest_ZodiacTemple.GreenMark",ch->GetQuestFlag("Quest_ZodiacTemple.GreenMark")+size);

	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenUI12zi %d %d %d %d", ch->GetQuestFlag("Quest_ZodiacTemple.YellowMark"), ch->GetQuestFlag("Quest_ZodiacTemple.GreenMark"), ch->GetQuestFlag("Quest_ZodiacTemple.YellowReward"), ch->GetQuestFlag("Quest_ZodiacTemple.GreenReward"));
}
#endif

#ifdef ENABLE_TRACK_WINDOW
#include "new_mob_timer.h"
ACMD(do_track_window)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "load")
	{
		if (ch->GetProtectTime("track_dungeon")==1)
			return;
		ch->GetDungeonCooldown(WORD_MAX);
		for (BYTE i = 2; i < vecArgs.size(); ++i)
		{
			WORD globalBossID;
			if (!str_to_number(globalBossID, vecArgs[i].c_str()))
				continue;
			CNewMobTimer::Instance().GetTrackData(ch, globalBossID);
		}
		ch->SetProtectTime("track_dungeon", 1);
	}
	else if (vecArgs[1] == "reenter")
	{
		if (!ch->IsGM())
			return;
		if (vecArgs.size() < 4) { return; }
		WORD testVnum;
		if (!str_to_number(testVnum, vecArgs[2].c_str()))
			return;
		int testTime;
		if (!str_to_number(testTime, vecArgs[3].c_str()))
			return;
		ch->GetDungeonCooldownTest(testVnum, testTime, false);
	}
	else if (vecArgs[1] == "cooldown")
	{
		if (!ch->IsGM())
			return;
		if (vecArgs.size() < 4) { return; }
		WORD testVnum;
		if (!str_to_number(testVnum, vecArgs[2].c_str()))
			return;
		int testTime;
		if (!str_to_number(testTime, vecArgs[3].c_str()))
			return;
		ch->GetDungeonCooldownTest(testVnum, testTime, true);
	}
	else if (vecArgs[1] == "teleport")
	{
		if (vecArgs.size() < 3) { return; }
		WORD mobIndex;
		if (!str_to_number(mobIndex, vecArgs[2].c_str()))
			return;

		//PORTAL WARP I PUT ONLY FOR FLAME B
		const std::map<WORD, std::pair<std::pair<long, long>,std::pair<WORD, std::pair<BYTE,BYTE>>>> m_TeleportData = {
			//{mobindex - {{X, Y}, {PORT, {MINLVL,MAXLVL},}}},
			{9844, { {3840, 14323}, {0, {40, 60}} }},
			{9836, { {3327, 14848}, {0, {65, 85}} }},
			{9838, { {3840, 14861}, {0, {80, 100}} }},
			{9840, { {4350, 14850}, {0, {95, 115}} }},
			{9842, { {3327, 15384}, {0, {105, 120}} }},
			// {4140, { {9341, 4134}, {0, {105, 120}} }},
			{1093, { {5905, 1105}, {0, {40, 120}} }},
			{2092, { {689, 6111}, {0, {70, 120}} }},
			{2493, { {1800, 12199}, {0, {75, 120}} }},
			{2598, { {5918, 993}, {0, {80, 120}} }},
			{6091, { {5984, 7073}, {0, {90, 120}} }},
			{6191, { {4319, 1647}, {0, {90, 120}} }},
			{6192, { {8277, 14187}, {0, {105, 120}} }},
			{9018, { {11082, 17824}, {0, {105, 120}} }},
			{20442, { {7358, 6237}, {0, {110, 120}} }},
		};
		const auto it = m_TeleportData.find(mobIndex);
		if (it != m_TeleportData.end())
		{
			if (ch->GetLevel() < it->second.second.second.first || ch->GetLevel() > it->second.second.second.second)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "You don't has enought level for teleport!");
				return;
			}
			ch->WarpSet(it->second.first.first * 100, it->second.first.second * 100, it->second.second.first);
		}
	}
}
#endif

#ifdef ENABLE_FISH_GAME
#include "fishing.h"
ACMD(do_fish_game)
{
	if (!ch->m_pkFishingEvent)
		return;
	else if (quest::CQuestManager::instance().GetEventFlag("fishgame_event") != 1)
		return;

	LPITEM rod = ch->GetWear(WEAR_WEAPON);
	if (!rod)
		return;

	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 3) { return; }

	int gameKey;
	if(!str_to_number(gameKey, vecArgs[1].c_str()))
		return;
	if (ch->GetProtectTime("fish_game_key") != gameKey)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseFishGame");
		return;
	}
	else if (vecArgs[2] == "close")
	{
		fishing::FishingFail(ch);
	}
	else if (vecArgs[2] == "score")
	{
		if (vecArgs.size() < 4) { return; }
		int scoreType;
		if(!str_to_number(scoreType, vecArgs[3].c_str()))
			return;

		int totalClick = ch->GetProtectTime("fish_game_total_click");
		totalClick += 1;
		ch->SetProtectTime("fish_game_total_click", totalClick);

		int totalScore = ch->GetProtectTime("fish_game_total_score");
		if (scoreType == 1)
		{
			totalScore += 1;
			ch->SetProtectTime("fish_game_total_score", totalScore);
		}

		if (totalClick == 3)
		{
			if(totalScore == 3)
				ch->fishing_take();
			else
			{
				event_cancel(&ch->m_pkFishingEvent);
				fishing::FishingFail(ch);
			}
		}
		else
			ch->ChatPacket(CHAT_TYPE_COMMAND, "SetFishGameGoal %d", totalScore);
	}
}
#endif

#ifdef ENABLE_EXCHANGE_LOG
ACMD(do_ex_log)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "load")
		ch->SendExchangeLogPacket(SUB_EXCHANGELOG_LOAD);
	else if (vecArgs[1] == "load_item")
	{
		if (vecArgs.size() < 3) { return; }
		DWORD logID;
		if(!str_to_number(logID, vecArgs[2].c_str()))
			return;
		ch->SendExchangeLogPacket(SUB_EXCHANGELOG_LOAD_ITEM, logID);
	}
	else if (vecArgs[1] == "delete")
	{
		if (vecArgs.size() < 4) { return; }
		const std::string playerCode(ch->GetDesc()->GetAccountTable().social_id);
		if (playerCode != vecArgs[2])
			return;
		if (vecArgs[3] == "all")
			ch->DeleteExchangeLog(0);
		else
		{
			for (DWORD j = 3; j < vecArgs.size(); ++j)
			{
				DWORD logID;
				if(!str_to_number(logID, vecArgs[j].c_str()))
					return;
				ch->DeleteExchangeLog(logID);
			}
		}
	}
}
#endif


#ifdef __GEM_SYSTEM__
ACMD(do_gem)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "load")
	{
		ch->RefreshGemPlayer();
	}
	else if (vecArgs[1] == "buy")
	{
		if (vecArgs.size() < 3) { return; }
		BYTE slotIndex;
		if (!str_to_number(slotIndex, vecArgs[2].c_str()))
			return;
		ch->BuyGemItem(slotIndex);
	}
}
#endif
ACMD(do_cards)
{
	const char *line;

	char arg1[256], arg2[256];

	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	switch (LOWER(arg1[0]))
	{
		case 'o':	// open
			if (isdigit(*arg2))
			{
				DWORD safemode;
				str_to_number(safemode, arg2);
				ch->Cards_open(safemode);
			}
			break;
		case 'p':	// open
			ch->Cards_pullout();
			break;
		case 'e':	// open
			ch->CardsEnd();
			break;
		case 'd':	// open
			if (isdigit(*arg2))
			{
				DWORD destroy_index;
				str_to_number(destroy_index, arg2);
				ch->CardsDestroy(destroy_index);
			}
			break;
		case 'a':	// open
			if (isdigit(*arg2))
			{
				DWORD accpet_index;
				str_to_number(accpet_index, arg2);
				ch->CardsAccept(accpet_index);
			}
			break;
		case 'r':	// open
			if (isdigit(*arg2))
			{
				DWORD restore_index;
				str_to_number(restore_index, arg2);
				ch->CardsRestore(restore_index);
			}
			break;
		default:
			return;
	}
}

#ifdef RENEWAL_MISSION_BOOKS
ACMD(do_missionbooks)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "load")
	{
		ch->SendMissionData();
	}
	else if (vecArgs[1] == "delete")
	{
		if (vecArgs.size() < 3) { return; }
		WORD missionID;
		str_to_number(missionID, vecArgs[2].c_str());
		ch->DeleteBookMission(missionID);
	}
	else if (vecArgs[1] == "reward")
	{
		if (vecArgs.size() < 3) { return; }
		WORD missionID;
		str_to_number(missionID, vecArgs[2].c_str());
		ch->RewardMissionBook(missionID);
	}
}
#endif

#ifdef __RENEWAL_BRAVE_CAPE__
ACMD(do_brave_cape)
{
	ch->GetBraveCapeCMDCompare(argument);
}
#endif

#ifdef __BUFFI_SUPPORT__
ACMD(do_buffi_costume_page)
{
	char arg[256];
	one_argument(argument, arg, sizeof(arg));
	if (!*arg)
		return;
	int iPageIdx;
	if (!str_to_number(iPageIdx, arg))
		return;
	ch->SetLookingBuffiPage(iPageIdx == 1 ? true : false);
}
#endif

#ifdef ENABLE_REWARD_SYSTEM
ACMD(do_update_reward_data)
{
	CHARACTER_MANAGER::Instance().SendRewardInfo(ch);
}
#endif

#ifdef __DUNGEON_INFO__
ACMD(do_dungeon_info)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "rank")
	{
		if (vecArgs.size() < 4) { return; }
		DWORD mobIdx, rankIdx;
		if (!str_to_number(mobIdx, vecArgs[2].c_str()) || !str_to_number(rankIdx, vecArgs[3].c_str()))
			return;
		CHARACTER_MANAGER::Instance().SendDungeonRank(ch, mobIdx, rankIdx);
	}
	else if (vecArgs[1] == "test_cooldown" && ch->IsGM())
	{
		ch->SetQuestFlag("jotun.cooldown", time(0) + 90);
		ch->SetQuestFlag("deviltower.cooldown", time(0) + 30);
		ch->SetQuestFlag("hydra.cooldown", time(0) + 90);
		ch->SetQuestFlag("MeleyDungeon.cooldown", time(0) + 90);
		ch->SetQuestFlag("spider.cooldown", time(0) + 60);
		ch->SetQuestFlag("devil.cooldown", time(0) + 30);
		ch->SetQuestFlag("nemere.cooldown", time(0) + 60);
		ch->SetQuestFlag("azrael.cooldown", time(0) + 60);
		ch->SetQuestFlag("dragonlair.cooldown", time(0) + 60);
		ch->SetQuestFlag("nightmare.cooldown", time(0) + 60);
		ch->SetQuestFlag("razador.cooldown", time(0) + 60);
		ch->SetQuestFlag("duratus.cooldown", time(0) + 90);
		ch->SetQuestFlag("alastoreasy.cooldown", time(0) + 90);
		ch->SetQuestFlag("alastorhard.cooldown", time(0) + 90);
		ch->SetQuestFlag("serpent.cooldown", time(0) + 90);
		ch->SetQuestFlag("zodiactemple.cooldown", time(0) + 90);
		ch->SetQuestFlag("rxdragonlair.cooldown", time(0) + 240);

		ch->SendDungeonCooldown(0);
	}
	else if (vecArgs[1] == "update" && ch->IsGM())
		ch->SendDungeonCooldown(0);
}
#endif

#ifdef __AUTO_SKILL_READER__
ACMD(do_auto_skill_reader)
{
	char szArg1[256], szArg2[256];
	two_arguments(argument, szArg1, sizeof(szArg1), szArg2, sizeof(szArg2));

	if (!*szArg1 || !*szArg2)
	{
		return;
	}

	BYTE bSkillIndex;
	str_to_number(bSkillIndex, szArg1);

	BYTE bStatus;
	str_to_number(bStatus, szArg2);

	ch->GetAutoSkill(bSkillIndex, bStatus == 0 ? false : true);
}

#ifdef ENABLE_AFFECT_BUFF_REMOVE
ACMD(do_remove_buff)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch)
		return;

	int affect = 0;
	str_to_number(affect, arg1);
	CAffect* pAffect = ch->FindAffect(affect);

	if (pAffect)
		ch->RemoveAffect(affect);
}
#endif
#ifdef __ROULETTE__
ACMD(do_roulette)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "start")
	{
		constexpr DWORD TICKET_ITEM_VNUM = 70313;

		if (quest::CQuestManager::instance().GetEventFlag("roulette_event") != 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Roulette event in not active!");
			return;
		}

		if (!ch->CountSpecifyItem(TICKET_ITEM_VNUM))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "You don't have enought ticket.");
			return;
		}
		else if (ch->GetProtectTime("roulette_game_key") != 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "You already in spin..");
			return;
		}

		const std::vector<std::pair<DWORD, WORD>> m_vecItemList = { {71084, 250}, {50255, 50}, {25041, 1}, {72346, 2}, {30624, 1}, {51001, 15}, {6884, 15}, {30400, 100}, {33031, 1}, {30630, 1},
			{30621, 1}, {30620, 1}, {30604, 1}, {30612, 1}, {30622, 1}, {55009, 1}, {30186, 1}, {30189, 1}, {30183, 1}, {49274, 1}, {54705, 10},
			{30627, 10}, {31233, 3}, {70251, 3}, {70429, 1}, {31258, 1}, {31257, 1}, {70428, 1}, {31242, 3}, {31243, 3}, {70411, 1}, {70412, 1}, {31235, 1}, {31236, 1}, {31237, 1},
			{50266, 1}, {51116, 1}, {30524, 1}, {55001, 1}, {30525, 1}, {30616, 1}, {30617, 1}, {30628, 1}, {50261, 5}, {50262, 5}, {31109, 50}, {31114, 50}, {30610, 10}, 
			{30626, 10}, };
			

		std::vector<std::pair<DWORD, WORD>> m_vecSelectedList;

		while (true)
		{
			const WORD idxList = m_vecItemList.size() == 1 ? 0 : number(0, m_vecItemList.size() - 1);
			if (std::find(m_vecSelectedList.begin(), m_vecSelectedList.end(), m_vecItemList[idxList]) == m_vecSelectedList.end())
				m_vecSelectedList.emplace_back(m_vecItemList[idxList]);
			if (m_vecSelectedList.size() == 19)
				break;
		}

		const std::vector<std::pair<DWORD, WORD>> m_vecSpecialItemList = { {53011, 1}, {71509, 1}, {20574, 1}, {50512, 1}, {54713, 1} };
		const WORD idxList = m_vecSpecialItemList.size() == 1 ? 0 : number(0, m_vecSpecialItemList.size() - 1);
		m_vecSelectedList.emplace_back(m_vecSpecialItemList[idxList]);

		const WORD gameKey = number(0, 65000);
		const BYTE selectedIdx = number(0, 19);

		ch->SetProtectTime("roulette_game_key", gameKey);
		ch->SetProtectTime("roulette_itemIdx", m_vecSelectedList[selectedIdx].first);
		ch->SetProtectTime("roulette_itemCount", m_vecSelectedList[selectedIdx].second);

		std::string cmd("");
		for (BYTE i = 0; i < 20; ++i)
		{
			cmd += std::to_string(m_vecSelectedList[i].first);
			cmd += "?";
			cmd += std::to_string(m_vecSelectedList[i].second);
			cmd += "#";
		}
		if (cmd == "")
			cmd = "Empty";

		ch->RemoveSpecifyItem(TICKET_ITEM_VNUM);
		ch->ChatPacket(CHAT_TYPE_COMMAND, "RouletteStart %u %s %u", gameKey, cmd.c_str(), selectedIdx);
	}
	else if (vecArgs[1] == "stop")
	{
		if (vecArgs.size() < 3) { return; }
		WORD wGameKey;
		if (!str_to_number(wGameKey, vecArgs[2].c_str()))
			return;
		if (ch->GetProtectTime("roulette_game_key") == wGameKey && ch->GetProtectTime("roulette_itemIdx") != 0 && ch->GetProtectTime("roulette_itemCount") != 0)
		{
			ch->AutoGiveItem(ch->GetProtectTime("roulette_itemIdx"), ch->GetProtectTime("roulette_itemCount"));
			ch->ChatPacket(CHAT_TYPE_INFO, "[Roulette] Successfully u earned reward.");

			ch->SetProtectTime("roulette_game_key", 0);
			ch->SetProtectTime("roulette_itemIdx", 0);
			ch->SetProtectTime("roulette_itemCount", 0);
		}
	}
}
#endif
#ifdef __SPIN_WHEEL__
ACMD(do_spin_wheel)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "spin")
	{
		if (ch->IsHack() || !ch->CanHandleItem() || ch->GetQuestFlag("spin_wheel.count") < 15 || quest::CQuestManager::instance().GetEventFlag("spin_wheel") != 1)
			return;
		ch->SetQuestFlag("spin_wheel.count", 0);
		ch->ChatPacket(CHAT_TYPE_COMMAND, "SetSpinWheel 0 1");

		struct spinItem
		{
			BYTE rareType;
			DWORD itemIdx;
			WORD itemCount;
			spinItem(const BYTE _rareType, const DWORD _itemIdx, const WORD _itemCount) : rareType(_rareType), itemIdx(_itemIdx), itemCount(_itemCount) {}
			spinItem() {}
		};
		struct spinStruct
		{
			WORD wStartLevel, wEndLevel;
			std::vector<spinItem> vecItems;
			spinItem jackPotItem;
			spinStruct(const WORD _wStartLevel, const WORD _wEndLevel, const std::vector<spinItem>& _vecItems, const spinItem& _jackPotItem) : wStartLevel(_wStartLevel), wEndLevel(_wEndLevel), vecItems(_vecItems), jackPotItem(){
				std::memcpy(&jackPotItem, &_jackPotItem, sizeof(jackPotItem));
			}
		};

		const static std::vector<spinStruct> vecSpinData = {
			spinStruct(
				1, 
				80, 
				{
					spinItem(0, 25041, 5), spinItem(0, 50255, 70), spinItem(0, 50255, 60), spinItem(0, 50255, 50), spinItem(0, 50255, 40), spinItem(0, 50255, 30), spinItem(0, 50255, 20), spinItem(0, 50255, 10), spinItem(0, 6884, 20), spinItem(0, 30400, 500), spinItem(0, 71084, 500), spinItem(0, 31108, 500), spinItem(0, 31107, 500), spinItem(0, 25041, 10), spinItem(0, 50513, 50), spinItem(0, 30005, 50), spinItem(0, 51001, 100), spinItem(0, 71084, 200), spinItem(0, 49273, 5), spinItem(0, 6884, 15), spinItem(0, 80030, 5), spinItem(0, 51112, 5), spinItem(0, 50186, 5), spinItem(0, 6884, 10), spinItem(0, 50255, 80)
				},
				spinItem(3, 50512, 1)
			),
			spinStruct(
				80,
				90,
				{
					spinItem(0, 25041, 5), spinItem(0, 50255, 70), spinItem(0, 50255, 60), spinItem(0, 50255, 50), spinItem(0, 50255, 40), spinItem(0, 50255, 30), spinItem(0, 50255, 20), spinItem(0, 50255, 10), spinItem(0, 6884, 20), spinItem(0, 30400, 500), spinItem(0, 71084, 500), spinItem(0, 31108, 500), spinItem(0, 31107, 500), spinItem(0, 25041, 10), spinItem(0, 50513, 50), spinItem(0, 30005, 50), spinItem(0, 51001, 100), spinItem(0, 71084, 200), spinItem(0, 51110, 5), spinItem(0, 6884, 15), spinItem(0, 51111, 5), spinItem(0, 51112, 5), spinItem(0, 50186, 5), spinItem(0, 6884, 10), spinItem(0, 50255, 80)
				},
				spinItem(3, 20572, 1)
			),
			spinStruct(
				100,
				110,
				{
					spinItem(0, 25041, 5), spinItem(0, 50255, 70), spinItem(0, 50255, 60), spinItem(0, 50255, 50), spinItem(0, 50255, 40), spinItem(0, 51116, 3), spinItem(0, 54705, 3), spinItem(0, 50266, 3), spinItem(0, 6884, 20), spinItem(0, 30400, 500), spinItem(0, 71084, 500), spinItem(0, 31108, 500), spinItem(0, 31107, 500), spinItem(0, 25041, 10), spinItem(0, 50513, 50), spinItem(0, 30005, 50), spinItem(0, 51001, 100), spinItem(0, 71084, 200), spinItem(0, 49273, 5), spinItem(0, 6884, 15), spinItem(0, 80030, 5), spinItem(0, 51112, 5), spinItem(0, 50186, 5), spinItem(0, 6884, 10), spinItem(0, 50255, 80)
				},
				spinItem(3, 60301, 1)
			),
			spinStruct(
				110,
				120,
				{
					spinItem(0, 25041, 5), spinItem(0, 70429, 1), spinItem(0, 70412, 1), spinItem(0, 70411, 1), spinItem(0, 49274, 3), spinItem(0, 51116, 3), spinItem(0, 54705, 3), spinItem(0, 50266, 3), spinItem(0, 6884, 20), spinItem(0, 30400, 500), spinItem(0, 71084, 500), spinItem(0, 31108, 500), spinItem(0, 31107, 500), spinItem(0, 25041, 10), spinItem(0, 50513, 50), spinItem(0, 30005, 50), spinItem(0, 51001, 100), spinItem(0, 71084, 200), spinItem(0, 49273, 5), spinItem(0, 6884, 15), spinItem(0, 80030, 5), spinItem(0, 51112, 5), spinItem(0, 50186, 5), spinItem(0, 6884, 10), spinItem(0, 50255, 80)
				},
				spinItem(3, 60300, 1)
			),
		};

		for (auto it = vecSpinData.begin(); it != vecSpinData.end(); ++it)
		{
			const spinStruct& spinData = *it;
			if (ch->GetLevel() >= spinData.wStartLevel && ch->GetLevel() <= spinData.wEndLevel)
			{
				DWORD selectedItemIdx = 0, selectedItemCount = 0;

				if (number(0, 5) == 1)
				{
					selectedItemIdx = spinData.jackPotItem.itemIdx;
					selectedItemCount = spinData.jackPotItem.itemCount;
				}
				else
				{
					const WORD idx = spinData.vecItems.size() <= 1 ? 0 : number(0, spinData.vecItems.size() - 1);
					selectedItemIdx = spinData.vecItems[idx].itemIdx;
					selectedItemCount = spinData.vecItems[idx].itemCount;

				}

				std::string cmd("");
				for (BYTE j = 0; j < spinData.vecItems.size(); ++j)
				{
					cmd += std::to_string(spinData.vecItems[j].rareType);
					cmd += "|";
					cmd += std::to_string(spinData.vecItems[j].itemIdx);
					cmd += "|";
					cmd += std::to_string(spinData.vecItems[j].itemCount);
					cmd += "?";
				}

				cmd += std::to_string(spinData.jackPotItem.rareType);
				cmd += "|";
				cmd += std::to_string(spinData.jackPotItem.itemIdx);
				cmd += "|";
				cmd += std::to_string(spinData.jackPotItem.itemCount);

				if (cmd == "")
					cmd = "-";
				ch->ChatPacket(CHAT_TYPE_COMMAND, "SetSpinReward %u %u %s", selectedItemIdx, selectedItemCount, cmd.c_str());
				ch->SetProtectTime("spint_itemidx", selectedItemIdx);
				ch->SetProtectTime("spint_itemcount", selectedItemCount);
				return;
			}
		}
	}
	else if (vecArgs[1] == "ani_done")
	{
		if (!ch->GetProtectTime("spint_itemidx") || !ch->GetProtectTime("spint_itemcount"))
			return;
		ch->AutoGiveItem(ch->GetProtectTime("spint_itemidx"), ch->GetProtectTime("spint_itemcount"));
		ch->SetProtectTime("spint_itemidx", 0);
		ch->SetProtectTime("spint_itemcount", 0);
		ch->ChatPacket(CHAT_TYPE_INFO, "Successfuly gived spin reward.");
	}
}
#endif
#endif

#if defined(__AUTOMATIC_HUNTING_SYSTEM__)
ACMD(do_auto_onoff)
{
	char arg1[256], arg2[256], arg3[256], arg4[256], arg5[256], arg6[256], arg7[256];
	one_argument(
		one_argument(
			one_argument(
				one_argument(
					one_argument(
						one_argument(
							one_argument(argument,
								arg1, sizeof(arg1)),
							arg2, sizeof(arg2)),
						arg3, sizeof(arg3)),
					arg4, sizeof(arg4)),
				arg5, sizeof(arg5)),
			arg6, sizeof(arg6)),
		arg7, sizeof(arg7)
	);
	if (!*arg1 || !*arg2 || !*arg3 || !*arg4 || !*arg5 || !*arg6 || !*arg7)
		return; // invalid syntax

	uint8_t uiStart, uiAttack, uiRange, uiPotion, uiSkill, uiRestart, uiAttackStone;
	str_to_number(uiStart, arg1);
	str_to_number(uiAttack, arg2);
	str_to_number(uiRange, arg3);
	str_to_number(uiPotion, arg4);
	str_to_number(uiSkill, arg5);
	str_to_number(uiRestart, arg6);
	str_to_number(uiAttackStone, arg7);

	bool bAutoUse = false;

#ifdef USE_AUTOMATIC_HUNTING_RENEWED
	bool bAutoHuntAff = false;
#endif

#ifdef USE_PREMIUM_AFFECT
	if (ch->FindAffect(AFFECT_PREMIUM_USER))
#else
	if (ch->FindAffect(AFFECT_AUTO_HUNT))
#endif
	{
#ifdef USE_AUTOMATIC_HUNTING_RENEWED
		bAutoHuntAff = true;
		bAutoUse = static_cast<bool>(uiStart);

		if (bAutoUse)
		{
#ifdef USE_DAY_CHANGE_MGR
			const int iToday = CDayChangeMgr::Instance().GetTodayNum();
#else
			const int iToday = GetTodayNum();
#endif
			const int iLastDay = ch->GetAutoHuntLastDay();

			if (iToday == iLastDay)
			{
				if (ch->GetAutoHuntUsedTime() >= AUTO_HUNT_DAILY_TIME_LIMIT_SEC)
				{
					bAutoUse = false;
				}
			}
			else
			{
				ch->SetAutoHuntLastDay(iToday + 1);
				ch->SetAutoHuntUsedTime(0);
			}
		}
		if (bAutoUse)
		{
#endif
		ch->SetAutoUseType(AUTO_ONOFF_START, static_cast<bool>(uiStart));
		ch->SetAutoUseType(AUTO_ONOFF_ATTACK, static_cast<bool>(uiAttack));
		ch->SetAutoUseType(AUTO_ONOFF_RANGE, static_cast<bool>(uiRange));
		ch->SetAutoUseType(AUTO_ONOFF_POTION, static_cast<bool>(uiPotion));
		ch->SetAutoUseType(AUTO_ONOFF_SKILL, static_cast<bool>(uiSkill));
		ch->SetAutoUseType(AUTO_ONOFF_RESTART, static_cast<bool>(uiRestart));
		ch->SetAutoUseType(AUTO_ONOFF_ATTACK_STONE, static_cast<bool>(uiAttackStone));
#ifdef USE_AUTOMATIC_HUNTING_RENEWED
		}
		else
		{
			for (uint8_t uiTypeIndex = AUTO_ONOFF_POTION; uiTypeIndex < AUTO_ONOFF_MAX; uiTypeIndex++)
			{
				ch->SetAutoUseType(uiTypeIndex, false);
			}
		}
#else
		bAutoUse = ch->GetAutoUseType(AUTO_ONOFF_START);
#endif
	}
	else
	{
		for (uint8_t uiTypeIndex = AUTO_ONOFF_POTION; uiTypeIndex < AUTO_ONOFF_MAX; uiTypeIndex++)
		{
			ch->SetAutoUseType(uiTypeIndex, false);
		}
	}

	if (bAutoUse)
	{
#ifdef USE_AUTOMATIC_HUNTING_RENEWED
		ch->AddAffect(AFFECT_AUTO_USE, POINT_NONE, 0, AFFECT_AUTO, AUTO_HUNT_DAILY_TIME_LIMIT_SEC - ch->GetAutoHuntUsedTime(), 0, true);
#else
		ch->AddAffect(AFFECT_AUTO_USE, POINT_NONE, 0, AFFECT_AUTO, INFINITE_AFFECT_DURATION, 0, true);
#endif
	}
	else
	{
		ch->RemoveAffect(AFFECT_AUTO_USE);
	}

#ifdef USE_AUTOMATIC_HUNTING_RENEWED
	ch->ChatPacket(CHAT_TYPE_COMMAND, "auto_onoff %u", bAutoUse ? 1 : 0);
#else
	ch->ChatPacket(CHAT_TYPE_COMMAND, "auto_onoff %d", uiStart);
#endif

#ifdef USE_AUTOMATIC_HUNTING_RENEWED
	if (static_cast<bool>(uiStart) && static_cast<bool>(uiStart) != bAutoUse)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "1575");
		// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have no left usage time for today, please retry tommorow."));
	}

	ch->ChatPacket(CHAT_TYPE_COMMAND, "autohunt_stat %d", bAutoHuntAff ? ch->GetAutoHuntUsedTime() : AUTO_HUNT_DAILY_TIME_LIMIT_SEC);
#endif
}
#endif

ACMD(do_split_items)
{
	if (!ch)
		return;
	
	const char *line;
	char arg1[256], arg2[256], arg3[256];
	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));
	
	if (!*arg1 || !*arg2 || !*arg3)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Wrong command use.");
		return;
	}
	
	if (!ch->CanWarp())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Close all windows and wait a few seconds, before using this.");
		return;
	}
	
	WORD count = 0;
	WORD cell = 0;
	WORD destCell = 0;
	
	str_to_number(cell, arg1);
	str_to_number(count, arg2);
	str_to_number(destCell, arg3);
	
	if (count <= 0)
		return;

	LPITEM item = ch->GetInventoryItem(cell);
	if (item != NULL)
	{
		WORD itemCount = item->GetCount();
		while (itemCount > 0)
		{
			if (count > itemCount)
				count = itemCount;
			
			int iEmptyPosition = ch->GetEmptyInventoryFromIndex(destCell, item->GetSize());
			if (iEmptyPosition == -1)
				break;
			
			itemCount -= count;
			ch->MoveItem(TItemPos(INVENTORY, cell), TItemPos(INVENTORY, iEmptyPosition), count, true);
		}
	}
}

ACMD(do_split_storage_items)
{
	if (!ch)
		return;
	
	char arg1[256], arg2[256], arg3[256], arg4[256];
	two_arguments (two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3), arg4, sizeof(arg4));
	
	if (!*arg1 || !*arg2 || !*arg3 || !*arg4)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Wrong command use.");
		return;
	}
	
	if (!ch->CanWarp(true))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Close all windows and wait a few seconds, before using this.");
		return;
	}
	
	WORD count = 0;
	WORD cell = 0;
	WORD destCell = 0;
	
	str_to_number(cell, arg1);
	str_to_number(count, arg2);
	str_to_number(destCell, arg3);
	
	if (count <= 0)
		return;

	switch (LOWER(arg4[0]))
	{
		case 'a':
		{
			LPITEM item = ch->GetSpecialStorageItem(cell, UPGRADE_INVENTORY);
			
			if (item != NULL)
			{
				WORD itemCount = item->GetCount();
				while (itemCount > 0)
				{
					if (count > itemCount)
						count = itemCount;
					
					int iEmptyPosition = ch->GetEmptySpecialStorageSlotFromIndex(destCell, item->GetSize(), UPGRADE_INVENTORY);
					if (iEmptyPosition == -1)
						break;
					
					itemCount -= count;
					ch->MoveItem(TItemPos(UPGRADE_INVENTORY, cell), TItemPos(UPGRADE_INVENTORY, iEmptyPosition), count, true);
				}
			}
			
			break;
		}
		
		case 'b':
		{
			LPITEM item = ch->GetSpecialStorageItem(cell, BOOK_INVENTORY);
			
			if (item != NULL)
			{
				WORD itemCount = item->GetCount();
				while (itemCount > 0)
				{
					if (count > itemCount)
						count = itemCount;
					
					int iEmptyPosition = ch->GetEmptySpecialStorageSlotFromIndex(destCell, item->GetSize(), BOOK_INVENTORY);
					if (iEmptyPosition == -1)
						break;
					
					itemCount -= count;
					ch->MoveItem(TItemPos(BOOK_INVENTORY, cell), TItemPos(BOOK_INVENTORY, iEmptyPosition), count, true);
				}
			}
			
			break;
		}
		
		case 'c':
		{
			LPITEM item = ch->GetSpecialStorageItem(cell, STONE_INVENTORY);
			
			if (item != NULL)
			{
				WORD itemCount = item->GetCount();
				while (itemCount > 0)
				{
					if (count > itemCount)
						count = itemCount;
					
					int iEmptyPosition = ch->GetEmptySpecialStorageSlotFromIndex(destCell, item->GetSize(), STONE_INVENTORY);
					if (iEmptyPosition == -1)
						break;
					
					itemCount -= count;
					ch->MoveItem(TItemPos(STONE_INVENTORY, cell), TItemPos(STONE_INVENTORY, iEmptyPosition), count, true);
				}
			}
			
			break;
		}
		
		case 'd':
		{
			LPITEM item = ch->GetSpecialStorageItem(cell, CHANGE_INVENTORY);
			
			if (item != NULL)
			{
				WORD itemCount = item->GetCount();
				while (itemCount > 0)
				{
					if (count > itemCount)
						count = itemCount;
					
					int iEmptyPosition = ch->GetEmptySpecialStorageSlotFromIndex(destCell, item->GetSize(), CHANGE_INVENTORY);
					if (iEmptyPosition == -1)
						break;
					
					itemCount -= count;
					ch->MoveItem(TItemPos(CHANGE_INVENTORY, cell), TItemPos(CHANGE_INVENTORY, iEmptyPosition), count, true);
				}
			}
			
			break;
		}
		
		case 'e':
		{
			LPITEM item = ch->GetSpecialStorageItem(cell, COSTUME_INVENTORY);
			
			if (item != NULL)
			{
				WORD itemCount = item->GetCount();
				while (itemCount > 0)
				{
					if (count > itemCount)
						count = itemCount;
					
					int iEmptyPosition = ch->GetEmptySpecialStorageSlotFromIndex(destCell, item->GetSize(), COSTUME_INVENTORY);
					if (iEmptyPosition == -1)
						break;
					
					itemCount -= count;
					ch->MoveItem(TItemPos(COSTUME_INVENTORY, cell), TItemPos(COSTUME_INVENTORY, iEmptyPosition), count, true);
				}
			}
			
			break;
		}		
		case 'm':
		{
			LPITEM item = ch->GetSpecialStorageItem(cell, CHEST_INVENTORY);
			
			if (item != NULL)
			{
				WORD itemCount = item->GetCount();
				while (itemCount > 0)
				{
					if (count > itemCount)
						count = itemCount;
					
					int iEmptyPosition = ch->GetEmptySpecialStorageSlotFromIndex(destCell, item->GetSize(), CHEST_INVENTORY);
					if (iEmptyPosition == -1)
						break;
					
					itemCount -= count;
					ch->MoveItem(TItemPos(CHEST_INVENTORY, cell), TItemPos(CHEST_INVENTORY, iEmptyPosition), count, true);
				}
			}
			
			break;
		}
		
		default:
			return;
	}
}

ACMD(do_vote_coins)
{
	if (!ch)
		return;

	if (ch->IsOpenSafebox()
		 || ch->GetShop()
		 || ch->IsCubeOpen()
		 || ch->IsDead()
		 || ch->GetExchange()
		 || !ch->CanWarp()
		 || !ch->CanHandleItem()
#ifdef __ENABLE_NEW_OFFLINESHOP__
		|| ch->GetOfflineShopGuest() || ch->GetAuctionGuest()
#endif
		)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You cannot do this action while having opened windows.");
		return;
	}

	if (!ch->IsLoadedAffect() || !ch->IsItemLoaded())
		return;

	quest::PC * pPC = quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID());

	if (!pPC)
		return;

	if (pPC && pPC->IsRunning())
		return;
	
	char arg1[256];
	one_argument (argument, arg1, sizeof(arg1));
	
	if (!*arg1)
		return;
	
	int iChoice = 0;
	str_to_number(iChoice, arg1);
	
	if (iChoice < 1 || iChoice > 4)
		return;

	if (ch->GetEp() == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You don't have any votecoins.");
		return;
	}
	
	if (ch->FindAffect(AFFECT_VOTEFORBONUS))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You already have an active bonus.");
		return;
	}
	
	if (iChoice == 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You have chosen strong against metinstones for 24 hours.");
		ch->AddAffect(AFFECT_VOTEFORBONUS, POINT_EXP_DOUBLE_BONUS, 20, 0, V4B_AFFECT_DURATION, 0, true);
	}
	else if (iChoice == 2)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You have chosen strong against monsters for 24 hours.");
		ch->AddAffect(AFFECT_VOTEFORBONUS, POINT_ATTBONUS_MONSTER, 10, 0, V4B_AFFECT_DURATION, 0, true);
	}
	else if (iChoice == 3)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You have chosen strong against bosses for 24 hours.");
		ch->AddAffect(AFFECT_VOTEFORBONUS, POINT_GOLD_DOUBLE_BONUS, 20, 0, V4B_AFFECT_DURATION, 0, true);
	}
	else if (iChoice == 4)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You have chosen strong against humans for 24 hours.");
		ch->AddAffect(AFFECT_VOTEFORBONUS, POINT_ATTBONUS_HUMAN, 10, 0, V4B_AFFECT_DURATION, 0, true);
	}

	long vcoins = ch->GetEp();
	
	ch->SetVoteCoins(vcoins - 1);
	ch->ChatPacket(CHAT_TYPE_COMMAND, "UpdateVoteCoins %d", vcoins - 1);
}

ACMD(do_ban_test)
{
	if (!ch)
		return;

	if (ch->IsOpenSafebox()
		 || ch->GetShop()
		 || ch->IsCubeOpen()
		 || ch->IsDead()
		 || ch->GetExchange()
		 || !ch->CanWarp()
		 || !ch->CanHandleItem()
#ifdef __ENABLE_NEW_OFFLINESHOP__
		|| ch->GetOfflineShopGuest() || ch->GetAuctionGuest()
#endif
		)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You cannot do this action while having opened windows.");
		return;
	}

	if (!ch->IsLoadedAffect() || !ch->IsItemLoaded())
		return;

	quest::PC * pPC = quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID());

	if (!pPC)
		return;

	if (pPC && pPC->IsRunning())
		return;
	
	uint32_t count_limit = 700;
	uint32_t bannedCount = 0;
	
	std::unique_ptr<SQLMsg> pmsg(DBManager::Instance().DirectQuery("SELECT name, login, hwid FROM log.hacker_detections GROUP BY name HAVING COUNT(*) > %d LIMIT 30;", count_limit));
	SQLResult* resSelect = pmsg->Get();
	MYSQL_ROW row;

	std::map<std::string, std::pair<std::string, std::string>> bannedAccounts = {};

	while ((row = mysql_fetch_row(resSelect->pSQLResult)))
	{
		std::string name = row[0];
		std::string login = row[1];
		std::string hwid = row[2];
		bannedAccounts[name] = std::pair(login, hwid);
	}

	for (auto& it : bannedAccounts)
	{
		char banAccount[512];
		sprintf(banAccount, 
				"UPDATE account.account AS a \
				 JOIN ( \
					 SELECT hwid_routher FROM account.account WHERE login = '%s' \
				 ) AS b ON a.hwid_routher = b.hwid_routher \
				 SET a.status = 'BLOCK'",
				it.second.first.c_str());

		DBManager::Instance().Query(banAccount);
		
		char bannedShoutNew[256];
		sprintf(bannedShoutNew, "[HackShield] Banned a player that used cheats!");
		BroadcastNotice(bannedShoutNew, false);

		LPDESC desc = DESC_MANAGER::Instance().FindByCharacterName(it.first.c_str());

		if (desc)
			desc->SetPhase(PHASE_CLOSE);
		else
		{
			TPacketGGDisconnectName p;
			p.bHeader = HEADER_GG_DISCONNECT_NAME;
			snprintf(p.szName, sizeof(p.szName), "%s", it.first.c_str());
			P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGDisconnectName));
		}
		
		char insertQuery[512];
		sprintf(insertQuery, 
				"INSERT INTO account.hwid_routher (hwid, reason, date) \
				 SELECT hwid_routher, 'AUTO_BANNED', NOW() \
				 FROM account.account \
				 WHERE LENGTH(hwid_routher) > 4 AND hwid_routher = ( \
					 SELECT hwid_routher FROM account.account WHERE login = '%s')",
				it.second.first.c_str());

		DBManager::Instance().Query(insertQuery);

		
		char banQuery[256];
		sprintf(banQuery, "INSERT INTO `log`.`hacker_bans` (`dateTime`, `playerName`, `accountName`, `playerHWID`, `detectionCount`) SELECT NOW(), '%s', '%s', '%s', COUNT(*) AS detectionCount FROM log.hacker_detections WHERE `name` = '%s';", it.first.c_str(), it.second.first.c_str(), it.second.second.c_str(), it.first.c_str());
		DBManager::Instance().Query(banQuery);

		char deleteQuery[256];
		sprintf(deleteQuery, "DELETE FROM `log`.`hacker_detections` WHERE name = '%s';", it.first.c_str());
		DBManager::Instance().Query(deleteQuery);
		
		bannedCount++;
	}
	
	char bannedShout[256];
	sprintf(bannedShout, "[HackShield] %d players were banned, play fair and don't use cheats!", bannedCount);
	BroadcastNotice(bannedShout, false);

	bannedAccounts.clear();
}

ACMD(do_request_rank_info)
{
	int iPulse = thecore_pulse();
	
	if (iPulse - ch->GetRequestRankTimer() < PASSES_PER_SEC(1))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You need to wait.");
		return;
	}
	
	char arg1[256];
	char arg2[256];

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
		return;
	
	BYTE i = 0;
	str_to_number(i, arg1);

	int iIter = 0;
	str_to_number(iIter, arg2);

	if (i >= 0 && i < RANK_MAX)
		RankPlayer::instance().RequestInfoRank(ch, i, iIter);
	
	ch->SetRequestRankTimer();
}
#include "banword.h"
#include <boost/algorithm/string.hpp>
#include "horsename_manager.h"
ACMD(do_pet_name)
{
	if (!ch)
		return;

	if (ch->IsOpenSafebox()
		 || ch->GetShop()
		 || ch->IsCubeOpen()
		 || ch->IsDead()
		 || ch->GetExchange()
		 || !ch->CanWarp()
		 || !ch->CanHandleItem()
#ifdef __ENABLE_NEW_OFFLINESHOP__
		 || ch->GetOfflineShopGuest() || ch->GetAuctionGuest()
#endif
		)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You cannot do this.");
		return;
	}

	if (!ch->IsLoadedAffect() || !ch->IsItemLoaded())
		return;

	quest::PC * pPC = quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID());

	if (!pPC)
		return;

	if (pPC && pPC->IsRunning())
		return;
	
	char arg1[256];
	one_argument (argument, arg1, sizeof(arg1));
	
	if (!*arg1)
		return;
	
	std::string petName(arg1);
		
	if (petName.length() > 14)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "The name is too long. Use 14 characters only.");
		return;
	}
	
	if (ch->CountSpecifyItem(79714) < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You don't have the required item");
		return;
	}
	
	if (CBanwordManager::instance().CheckString(petName.c_str(), petName.length()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You can't use that name.");
		return;
	}
	
	if (ch->FindAffect(AFFECT_NAME_PET))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Your pet already has a name bonus.");
		return;
	}

	char szName[256];

	DBManager::instance().EscapeString(szName, 256, arg1, strlen(arg1));
	std::string petName_escape(szName);
	boost::replace_all(petName_escape, "_", " ");
	boost::replace_all(petName_escape, "%", "%%");
	
	const LPITEM pet = ch->GetWear(WEAR_OLD_PET);
	if (!pet)
	{
		ch->ChatPacket(1, "You need to have a pet equipped.");
		return;
	}

	if (pet != NULL)
	{
		pet->SetNewName(petName_escape.c_str());
		
		CPetSystem* petSystem = ch->GetPetSystem();
		if (petSystem)
		{
			petSystem->DeletePet(pet->GetID());
			petSystem->HandlePetCostumeItem();
		}
		
	}
	ch->RemoveSpecifyItem(79714, 1);

	ch->AddAffect(AFFECT_NAME_PET, POINT_ATTBONUS_STONE, 10, 0, 604800, 0, true);
}

ACMD(do_mount_name)
{
	if (!ch)
		return;

	if (ch->IsOpenSafebox()
		 || ch->GetShop()
		 || ch->IsCubeOpen()
		 || ch->IsDead()
		 || ch->GetExchange()
		 || !ch->CanWarp()
		 || !ch->CanHandleItem()
#ifdef __ENABLE_NEW_OFFLINESHOP__
		|| ch->GetOfflineShopGuest() || ch->GetAuctionGuest()
#endif
		)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You cannot do this.");
		return;
	}

	if (!ch->IsLoadedAffect() || !ch->IsItemLoaded())
		return;

	quest::PC * pPC = quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID());

	if (!pPC)
		return;

	if (pPC && pPC->IsRunning())
		return;
	
	char arg1[256];
	one_argument (argument, arg1, sizeof(arg1));
	
	if (!*arg1)
		return;
	
	std::string petName(arg1);
		
	if (petName.length() > 14)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "The name is too long. Use 14 characters only.");
		return;
	}
	
	if (ch->CountSpecifyItem(79715) < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You don't have the required item");
		return;
	}
	
	if (CBanwordManager::instance().CheckString(petName.c_str(), petName.length()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You can't use that name.");
		return;
	}
	if (ch->FindAffect(AFFECT_NAME_MOUNT))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Your mount already has a name bonus.");
		return;
	}
	
	char szName[256];
	DBManager::instance().EscapeString(szName, 256, arg1, strlen(arg1));
	std::string petName_escape(szName);
	boost::replace_all(petName_escape, "_", " ");
	boost::replace_all(petName_escape, "%", "%%");
	
	CHorseNameManager::instance().UpdateHorseName(ch->GetPlayerID(), petName_escape.c_str(), true);

	const LPITEM mount = ch->GetWear(WEAR_COSTUME_MOUNT);
	if (!mount)
	{
		ch->ChatPacket(1, "You need to have a mount equipped.");
		return;
	}
	
	if (mount != NULL)
	{
		if (ch->GetHorse())
			ch->HorseSummon(false);
		ch->HorseSummon(true);
	}
	ch->RemoveSpecifyItem(79715, 1);
	
	ch->SetQuestFlag("horse_name.valid_till", get_global_time() + 604800);
	ch->AddAffect(AFFECT_NAME_MOUNT, POINT_ATTBONUS_MONSTER, 10, 0, 604800, 0, false);
}

#if defined(__GAME_OPTION_ESCAPE__)
ACMD(do_escape)
{
	if (ch->IsDead() || ch->IsPolymorphed())
		return;

	if (ch->GetEscapeCooltime() > thecore_pulse() && !ch->IsGM())
		return;

	const BYTE bEmpire = ch->GetEmpire();
	const long lMapIndex = ch->GetMapIndex();
	const PIXEL_POSITION& rkPos = ch->GetXYZ();

	const LPSECTREE_MAP pSectreeMap = SECTREE_MANAGER::instance().GetMap(lMapIndex);
	if (pSectreeMap == nullptr)
	{
		ch->WarpSet(g_start_position[bEmpire][0], g_start_position[bEmpire][1]);
		return;
	}

	const LPSECTREE pSectree = pSectreeMap->Find(rkPos.x, rkPos.y);
	const DWORD dwAttr = pSectree->GetAttribute(rkPos.x, rkPos.y);

	int iEscapeDistance = quest::CQuestManager::instance().GetEventFlag("escape_distance");
	iEscapeDistance = (iEscapeDistance > 0) ? iEscapeDistance : 300;

	int iEscapeCooltime = quest::CQuestManager::instance().GetEventFlag("escape_cooltime");
	iEscapeCooltime = (iEscapeCooltime > 0) ? iEscapeCooltime : 10;

	if (IS_SET(dwAttr, ATTR_BLOCK /*| ATTR_OBJECT*/))
	{
		/*
		* NOTE : If an object doesn't have a blocked area, players can still be blocked if they get inside it.
		* The problem is that bridges are treated as objects too, and we don't want players to use the escape feature through them.
		* The tricky part is figuring out whether a specific object is a bridge or not.
		* In the current state, we are only checking blocked areas.
		*
		* 2021.01.17.Owsap
		*/

		PIXEL_POSITION kNewPos;
		if (SECTREE_MANAGER::instance().GetRandomLocation(lMapIndex, kNewPos, rkPos.x, rkPos.y, iEscapeDistance))
		{
			char szBuf[255 + 1];
			snprintf(szBuf, sizeof(szBuf), "%ld, (%d, %d) -> (%d, %d)",
				lMapIndex, rkPos.x, rkPos.y, kNewPos.x, kNewPos.y);
			LogManager::instance().CharLog(ch, lMapIndex, "ESCAPE", szBuf);

			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have successfully freed yourself."));
			ch->Show(lMapIndex, kNewPos.x, kNewPos.y, rkPos.z);
		}
		else
		{
			char szBuf[255 + 1];
			snprintf(szBuf, sizeof(szBuf), "%ld, (%d, %d) -> EMPIRE START POSITION",
				lMapIndex, rkPos.x, rkPos.y);
			LogManager::instance().CharLog(ch, lMapIndex, "ESCAPE", szBuf);

			ch->WarpSet(g_start_position[bEmpire][0], g_start_position[bEmpire][1]);
		}
	}

	ch->SetEscapeCooltime(thecore_pulse() + PASSES_PER_SEC(10));
}
#endif

#ifdef __LEADERSHIP__BONUS__
#include "party.h"
ACMD(do_leadership_bonus)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	BYTE bRole = 0;
	str_to_number(bRole, arg1);

	if (!ch->GetDesc() || !ch->CanWarp() || !ch->IsLoadedAffect())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You need to wait 10 seconds before using this command.");
		return;
	}
	
	if (ch->GetParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You cant make this while party.");
		return;
	}
	
	if (ch->GetLeadershipSkillLevel() == 0)
		return;
	
	if (ch->GetQuestFlag("wait_leadership") > get_global_time())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Wait few seconds to using again this command.");
		return;
	}
	
	ch->SetQuestFlag("wait_leadership", get_global_time() + 2);

	int iBonus = 0;
	float k = (float) ch->GetSkillPowerByLevel( MIN(SKILL_MAX_LEVEL, ch->GetLeadershipSkillLevel() ) )/ 100.0f;
	
	int iPointActived = 0;
	
	CAffect* aff = ch->FindAffect(AFF_LEADERSHIP);
	if (aff)
	{
		iPointActived = aff->bApplyOn;
	}

	ch->PointChange(POINT_PARTY_ATTACKER_BONUS, -ch->GetPoint(POINT_PARTY_ATTACKER_BONUS));
	ch->PointChange(POINT_PARTY_TANKER_BONUS, -ch->GetPoint(POINT_PARTY_TANKER_BONUS));
	ch->PointChange(POINT_PARTY_BUFFER_BONUS, -ch->GetPoint(POINT_PARTY_BUFFER_BONUS));
	ch->PointChange(POINT_PARTY_SKILL_MASTER_BONUS, -ch->GetPoint(POINT_PARTY_SKILL_MASTER_BONUS));
	ch->PointChange(POINT_PARTY_DEFENDER_BONUS, -ch->GetPoint(POINT_PARTY_DEFENDER_BONUS));
	ch->PointChange(POINT_PARTY_HASTE_BONUS, -ch->GetPoint(POINT_PARTY_HASTE_BONUS));
	ch->ComputeBattlePoints();

	ch->SetPoint(POINT_PARTY_ATTACKER_BONUS, 0);
	ch->SetPoint(POINT_PARTY_TANKER_BONUS, 0);
	ch->SetPoint(POINT_PARTY_BUFFER_BONUS, 0);
	ch->SetPoint(POINT_PARTY_SKILL_MASTER_BONUS, 0);
	ch->SetPoint(POINT_PARTY_DEFENDER_BONUS, 0);
	ch->SetPoint(POINT_PARTY_HASTE_BONUS, 0);

	ch->RemoveAffect(AFF_LEADERSHIP);
	ch->ComputePoints();
	
	switch(bRole)
	{
		case 1:
			{
				if (iPointActived == POINT_PARTY_ATTACKER_BONUS)
					break;
				
				iBonus = (int)(10 + 60 * k);
				ch->AddAffect(AFF_LEADERSHIP, POINT_PARTY_ATTACKER_BONUS, iBonus, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);
			}
			break;

		case 2:
			{
				if (iPointActived == POINT_PARTY_TANKER_BONUS)
					break;
				
				iBonus = (int)(50 + 1450 * k);

				ch->AddAffect(AFF_LEADERSHIP, POINT_PARTY_TANKER_BONUS, iBonus, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);
			}
			break;
		
		case 3:
			{
				if (iPointActived == POINT_PARTY_BUFFER_BONUS)
					break;
				
				iBonus = (int)(5 + 45 * k);

				ch->AddAffect(AFF_LEADERSHIP, POINT_PARTY_BUFFER_BONUS, iBonus, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);
			}
			break;
			
		case 4:
			{
				if (iPointActived == POINT_PARTY_SKILL_MASTER_BONUS)
					break;
				
				iBonus = (int)(25 + 600 * k);

				ch->AddAffect(AFF_LEADERSHIP, POINT_PARTY_SKILL_MASTER_BONUS, iBonus, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);
			}
			break;
			
		case 5:
			{
				if (iPointActived == POINT_PARTY_HASTE_BONUS)
					break;
				
				iBonus = (int)(1 + 5 * k);

				ch->AddAffect(AFF_LEADERSHIP, POINT_PARTY_HASTE_BONUS, iBonus, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);
			}
			break;
			
		case 6:
			{
				if (iPointActived == POINT_PARTY_DEFENDER_BONUS)
					break;
				
				iBonus = (int)(5 + 30 * k);

				ch->AddAffect(AFF_LEADERSHIP, POINT_PARTY_DEFENDER_BONUS, iBonus, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);
			}
			break;
			
		default:
		{
			break;
		}
	}
	
	ch->ComputePoints();
	ch->PointsPacket();
}
#endif
#ifdef ENABLE_BATTLEPASS

enum BattlePassCommand
{
    REQUEST_CURRENT_PROGRESS_DATA = 1,
    REDEEM_MISSION_REWARD,
    REDEEM_MISSION_EXP,
};


ACMD(do_battlepass_command)
{
    char arg1[256], arg2[256], arg3[256];
    three_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2), arg3, sizeof(arg3));

    if (!*arg1)
        return;
	
    uint32_t command;
    str_to_number(command, arg1);

	switch (command)
    {
        case REQUEST_CURRENT_PROGRESS_DATA:
            ch->SendBattlePassCurrentProgress();
            break;
			
        case REDEEM_MISSION_REWARD:
            if (!*arg2)
                return;

            uint32_t tier_index;
            str_to_number(tier_index, arg2);
			
            bool is_premium_reward;
            str_to_number(is_premium_reward, arg3);

			ch->RedeemBattlePassReward(tier_index, is_premium_reward);
            break;
			
        case REDEEM_MISSION_EXP:
            if (!*arg2)
                return;

            uint32_t mission_index;
            str_to_number(mission_index, arg2);

			ch->RedeemBattlePassMissionExp(mission_index);
            break;

        default:
            break;
    }

}
#endif

ACMD(do_pendant)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "set")
	{
		if (vecArgs.size() < 3) { return; }
		WORD uPageIdx;
		if (!str_to_number(uPageIdx, vecArgs[2].c_str()))
			return;
		else if (uPageIdx > 1)
			return;
		else if (ch->GetProtectTime("lastPendantClick") > time(0))
			return;
		else if (ch->GetQuestFlag("pendant.idx") == uPageIdx)
			return;
		ch->SetProtectTime("lastPendantClick", time(0) + 2);
		if (ch->GetQuestFlag("pendant.opened") == 0)
		{
			if (ch->CountSpecifyItem(51501) < 200)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "You don't has enought item");
				return;
			}
			ch->RemoveSpecifyItem(51501, 200);
			ch->SetQuestFlag("pendant.opened", 1);
		}
		ch->SetQuestFlag("pendant.idx", uPageIdx);
		ch->ChatPacket(CHAT_TYPE_COMMAND, "SetPendantPageIdx %d %d", uPageIdx, 1);
		ch->ComputePoints();
	}
}

#ifdef ENABLE_VOICE_CHAT
ACMD(do_voice_chat_config)
{
	char arg[256];
	one_argument(argument, arg, sizeof(arg));

	uint8_t type;
	str_to_number(type, arg);

	if (!type || type >= VOICE_CHAT_TYPE_MAX_NUM)
		return;

	ch->VoiceChatSetConfig(type, !ch->VoiceChatIsBlock(type));
}
#endif
