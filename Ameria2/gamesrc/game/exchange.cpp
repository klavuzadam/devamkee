#include "stdafx.h"
#include "../libgame/grid.h"
#include "utils.h"
#include "desc.h"
#include "desc_client.h"
#include "char.h"
#include "item.h"
#include "item_manager.h"
#include "packet.h"
#include "log.h"
#include "db.h"
#include "locale_service.h"
#include "../common/length.h"
#include "exchange.h"
#include "DragonSoul.h"
#include "questmanager.h" // @fixme150
#if defined(WJ_COMBAT_ZONE)	
	#include "combat_zone.h"
#endif
#ifdef ENABLE_EXCHANGE_LOG
#include "quest.h"
#include "questmanager.h"
#endif
#ifdef ENABLE_MESSENGER_BLOCK
#include "messenger_manager.h"
#endif

void exchange_packet(LPCHARACTER ch, BYTE sub_header, bool is_me, long long arg1, TItemPos arg2, DWORD arg3, void * pvData = NULL);

// ±³È¯ ÆÐÅ¶
void exchange_packet(LPCHARACTER ch, BYTE sub_header, bool is_me, long long arg1, TItemPos arg2, DWORD arg3, void * pvData)
{
	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return;
	}

	TPacketExchange p;
	p.header = HEADER_GC_EXCHANGE;
	p.sub_header = sub_header;
	p.is_me = is_me;
	p.arg1 = arg1;
	p.arg2 = arg2;
	p.arg3 = arg3;

	if (sub_header == EXCHANGE_SUBHEADER_ITEM_ADD && pvData)
	{
#ifdef WJ_ENABLE_TRADABLE_ICON
		p.arg4 = TItemPos(((LPITEM) pvData)->GetWindow(), ((LPITEM) pvData)->GetCell());
#endif
		std::memcpy(&p.alSockets, ((LPITEM) pvData)->GetSockets(), sizeof(p.alSockets));
		std::memcpy(&p.aAttr, ((LPITEM) pvData)->GetAttributes(), sizeof(p.aAttr));
#ifdef ENABLE_YOHARA_SYSTEM
		std::memcpy(&p.aApplyRandom, ((LPITEM)pvData)->GetApplysRandom(), sizeof(p.aApplyRandom));
#endif
#ifdef ENABLE_NEW_NAME_ITEM
		strlcpy(p.name,((LPITEM) pvData)->GetNewName(),sizeof(p.name));
#endif
	}
	else
	{
#ifdef WJ_ENABLE_TRADABLE_ICON
		p.arg4 = TItemPos(RESERVED_WINDOW, 0);
#endif
		memset(&p.alSockets, 0, sizeof(p.alSockets));
		memset(&p.aAttr, 0, sizeof(p.aAttr));
#ifdef ENABLE_YOHARA_SYSTEM
		memset(&p.aApplyRandom, 0, sizeof(p.aApplyRandom));
#endif
#ifdef ENABLE_NEW_NAME_ITEM
		strlcpy(p.name,"^",sizeof(p.name));
#endif
	}

	d->Packet(&p, sizeof(TPacketExchange));
}

bool CHARACTER::ExchangeStart(LPCHARACTER victim)
{
	if (this == victim)
		return false;

	if (IsObserverMode())
	{
		ChatPacket(CHAT_TYPE_INFO, "655");
		return false;
	}

	if (victim->IsNPC())
		return false;

#ifdef ENABLE_PREVENT_FLOOD_PACKETS
    const int iPulse = thecore_pulse();
    if (iPulse - GetLastExchangeTime() < PASSES_PER_SEC(3)) // 3 sec
    {
        ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[1554]Please try again later."));
        return false;
    }

    if (iPulse - victim->GetLastExchangeTime() < PASSES_PER_SEC(3)) // 3 sec
    {
        ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[1554]Please try again later."));
        return false;
    }
#endif


#if defined(WJ_COMBAT_ZONE)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()) || CCombatZoneManager::Instance().IsCombatZoneMap(victim->GetMapIndex()))
		return false;
#endif


	//Fix Revive Time
	if (victim->IsDead() || IsDead())
		return false;
	
	//PREVENT_TRADE_WINDOW
	if (IsOpenSafebox()
		 || GetShop()
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
		ChatPacket( CHAT_TYPE_INFO, "656" );
		return false;
	}

	if (victim->IsOpenSafebox()
		 || victim->GetShop()
		 || victim->IsCubeOpen()
#ifdef __BL_67_ATTR__
		 || victim->Is67AttrOpen()
#endif
#ifdef __AURA_SYSTEM__
		 || victim->IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
		 || victim->isSashOpened(true)
		 || victim->isSashOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
		 || victim->IsShardCraftOpened()
#endif
		)
	{
		ChatPacket( CHAT_TYPE_INFO, "657" );
		return false;
	}
	//END_PREVENT_TRADE_WINDOW
	int iDist = DISTANCE_APPROX(GetX() - victim->GetX(), GetY() - victim->GetY());

	// °Å¸® Ã¼Å©
	if (iDist >= EXCHANGE_MAX_DISTANCE)
		return false;

	if (GetExchange())
		return false;
	
#ifdef ENABLE_MESSENGER_BLOCK
	if (MessengerManager::instance().IsBlocked(GetName(), victim->GetName()))
	{
		ChatPacket(CHAT_TYPE_INFO, "You need to unblock %s do to that.", victim->GetName());
		return false;
	}
	if (MessengerManager::instance().IsBlocked(victim->GetName(), GetName()))
	{
		ChatPacket(CHAT_TYPE_INFO, "%s has blocked you.", victim->GetName());
		return false;
	}
#endif

	if (victim->GetExchange())
	{
		exchange_packet(this, EXCHANGE_SUBHEADER_ALREADY, 0, 0, NPOS, 0);
		return false;
	}

	if (victim->IsBlockMode(BLOCK_EXCHANGE))
	{
		ChatPacket(CHAT_TYPE_INFO, "658");
		return false;
	}

	SetExchange(M2_NEW CExchange(this));
	victim->SetExchange(M2_NEW CExchange(victim));

	victim->GetExchange()->SetCompany(GetExchange());
	GetExchange()->SetCompany(victim->GetExchange());

	//
	SetExchangeTime();
	victim->SetExchangeTime();

	exchange_packet(victim, EXCHANGE_SUBHEADER_START, 0, GetVID(), NPOS, 0);
	exchange_packet(this, EXCHANGE_SUBHEADER_START, 0, victim->GetVID(), NPOS, 0);

	return true;
}

CExchange::CExchange(LPCHARACTER pOwner)
{
	m_pCompany = NULL;

	m_bAccept = false;

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		m_apItems[i] = NULL;
		m_aItemPos[i] = NPOS;
		m_abItemDisplayPos[i] = 0;
	}

#ifdef ENABLE_EXCHANGE_LOG
	logIndex = -1;
#endif
	m_lldGold = 0;
#ifdef USE_CHEQUE_CURRENCY
	m_lldCheque = 0;
#endif

	m_pOwner = pOwner;
	pOwner->SetExchange(this);

	m_pGrid = M2_NEW CGrid(EXCHANGE_PAGE_COLUMN, EXCHANGE_PAGE_ROW);
}

CExchange::~CExchange()
{
	M2_DELETE(m_pGrid);
}

bool CExchange::AddItem(TItemPos item_pos, BYTE display_pos)
{
	assert(m_pOwner != NULL && GetCompany());

	if (!item_pos.IsValidItemPosition())
		return false;

	// Àåºñ´Â ±³È¯ÇÒ ¼ö ¾øÀ½
	if (item_pos.IsEquipPosition())
		return false;

	LPITEM item;

	if (!(item = m_pOwner->GetItem(item_pos)))
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_GIVE))
	{
		m_pOwner->ChatPacket(CHAT_TYPE_INFO, "659");
		return false;
	}

	if (true == item->isLocked())
	{
		return false;
	}

	// ÀÌ¹Ì ±³È¯Ã¢¿¡ Ãß°¡µÈ ¾ÆÀÌÅÛÀÎ°¡?
	if (item->IsExchanging())
	{
		sys_log(0, "EXCHANGE under exchanging");
		return false;
	}

	if (!m_pGrid->IsEmpty(display_pos, 1, item->GetSize()))
	{
		sys_log(0, "EXCHANGE not empty item_pos %d %d %d", display_pos, 1, item->GetSize());
		return false;
	}
#ifdef __SOULBINDING_SYSTEM__
	if (item->IsBind() || item->IsUntilBind())
	{
		m_pOwner->ChatPacket(CHAT_TYPE_INFO, "You can't exchange this item because is binded!");
		return false;
	}
#endif
	Accept(false);
	GetCompany()->Accept(false);

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (m_apItems[i])
			continue;

		m_apItems[i]		= item;
		m_aItemPos[i]		= item_pos;
		m_abItemDisplayPos[i]	= display_pos;
		m_pGrid->Put(display_pos, 1, item->GetSize());

		item->SetExchanging(true);

		exchange_packet(m_pOwner,
				EXCHANGE_SUBHEADER_ITEM_ADD,
				true,
				item->GetVnum(),
				TItemPos(RESERVED_WINDOW, display_pos),
				item->GetCount(),
				item);

		exchange_packet(GetCompany()->GetOwner(),
				EXCHANGE_SUBHEADER_ITEM_ADD,
				false,
				item->GetVnum(),
				TItemPos(RESERVED_WINDOW, display_pos),
				item->GetCount(),
				item);

		sys_log(0, "EXCHANGE AddItem success %s pos(%d, %d) %d", item->GetName(), item_pos.window_type, item_pos.cell, display_pos);

		return true;
	}

	// Ãß°¡ÇÒ °ø°£ÀÌ ¾øÀ½
	return false;
}

bool CExchange::RemoveItem(BYTE pos)
{
	if (pos >= EXCHANGE_ITEM_MAX_NUM)
		return false;

	if (!m_apItems[pos])
		return false;

	TItemPos PosOfInventory = m_aItemPos[pos];
	m_apItems[pos]->SetExchanging(false);

	m_pGrid->Get(m_abItemDisplayPos[pos], 1, m_apItems[pos]->GetSize());

	exchange_packet(GetOwner(),	EXCHANGE_SUBHEADER_ITEM_DEL, true, pos, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_ITEM_DEL, false, pos, PosOfInventory, 0);

	Accept(false);
	GetCompany()->Accept(false);

	m_apItems[pos]	    = NULL;
	m_aItemPos[pos]	    = NPOS;
	m_abItemDisplayPos[pos] = 0;
	return true;
}

bool CExchange::AddGold(const long long lldGold)
{
	if (lldGold <= 0)
	{
		return false;
	}

	auto owner = GetOwner();
	if (!owner)
	{
		return false;
	}

	auto target = GetCompany() ? GetCompany()->GetOwner() : nullptr;
	if (!target)
	{
		return false;
	}

	if (owner->GetPlayerID() == target->GetPlayerID())
	{
		return false;
	}

	if (m_lldGold > 0)
	{
		return false;
	}

	if (owner->GetGold() < lldGold)
	{
		owner->ChatPacket(CHAT_TYPE_INFO, "1315");
		return false;
	}

	const long long nTotalMoney = static_cast<long long>(target->GetGold()) + static_cast<long long>(lldGold);
	if (nTotalMoney > GOLD_MAX)
	{
		owner->ChatPacket(CHAT_TYPE_INFO, "1314 %lld %s", lldGold, target->GetName());
		owner->ChatPacket(CHAT_TYPE_INFO, "1313");
		return false;
	}

	Accept(false);
	GetCompany()->Accept(false);

	m_lldGold = lldGold;

	exchange_packet(owner, EXCHANGE_SUBHEADER_GOLD_ADD, true, m_lldGold, NPOS, 0);
	exchange_packet(target, EXCHANGE_SUBHEADER_GOLD_ADD, false, m_lldGold, NPOS, 0);
	return true;
}

#ifdef USE_CHEQUE_CURRENCY
bool CExchange::AddCheque(const long long lldCheque)
{
	if (lldCheque <= 0)
	{
		return false;
	}

	auto owner = GetOwner();
	if (!owner)
	{
		return false;
	}

	auto target = GetCompany() ? GetCompany()->GetOwner() : nullptr;
	if (!target)
	{
		return false;
	}

	if (owner->GetPlayerID() == target->GetPlayerID())
	{
		return false;
	}

	if (m_lldCheque > 0)
	{
		return false;
	}

	if (static_cast<long long>(owner->GetCheque()) < lldCheque)
	{
		owner->ChatPacket(CHAT_TYPE_INFO, "1316");
		return false;
	}

	const long long nTotalCheque = static_cast<long long>(target->GetCheque()) + static_cast<long long>(lldCheque);
	if (nTotalCheque > CHEQUE_MAX)
	{
		owner->ChatPacket(CHAT_TYPE_INFO, "1317 %lld %s", lldCheque, target->GetName());
		owner->ChatPacket(CHAT_TYPE_INFO, "1313");
		return false;
	}

	Accept(false);
	GetCompany()->Accept(false);

	m_lldCheque = lldCheque;

	exchange_packet(owner, EXCHANGE_SUBHEADER_CHEQUE_ADD, true, m_lldCheque, NPOS, 0);
	exchange_packet(target, EXCHANGE_SUBHEADER_CHEQUE_ADD, false, m_lldCheque, NPOS, 0);
	return true;
}
#endif

bool CExchange::Check(int* piItemCount)
{
	auto owner = GetOwner();
	if (!owner)
	{
		return false;
	}

	if (m_lldGold > 0 && owner->GetGold() < m_lldGold)
	{
		return false;
	}
#ifdef USE_CHEQUE_CURRENCY
	else if (m_lldCheque > 0 && static_cast<long long>(owner->GetCheque()) < m_lldCheque)
	{
		return false;
	}
#endif

	int item_count = 0;

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
		{
			continue;
		}

		if (!m_aItemPos[i].IsValidItemPosition())
		{
			return false;
		}

		if (m_apItems[i] != owner->GetItem(m_aItemPos[i]))
		{
			return false;
		}

		++item_count;
	}

	*piItemCount = item_count;
	return true;
}

#ifdef ENABLE_SPECIAL_STORAGE
bool CExchange::CheckSpecialStorageSpace(BYTE bStorageType)
{
	static CGrid s_grid1(SPECIAL_INVENTORY_PAGE_COLUMN, SPECIAL_INVENTORY_PAGE_ROW);
	static CGrid s_grid2(SPECIAL_INVENTORY_PAGE_COLUMN, SPECIAL_INVENTORY_PAGE_ROW);
	static CGrid s_grid3(SPECIAL_INVENTORY_PAGE_COLUMN, SPECIAL_INVENTORY_PAGE_ROW);
	static CGrid s_grid4(SPECIAL_INVENTORY_PAGE_COLUMN, SPECIAL_INVENTORY_PAGE_ROW);
	//static CGrid s_grid5(SPECIAL_INVENTORY_PAGE_COLUMN, SPECIAL_INVENTORY_PAGE_ROW);
	s_grid1.Clear();
	s_grid2.Clear();
	s_grid3.Clear();
	s_grid4.Clear();
	//s_grid5.Clear();

	LPCHARACTER	victim = GetCompany()->GetOwner();
	LPITEM item;

	int i;

	for (i = 0; i < SPECIAL_INVENTORY_PAGE_SIZE*1; ++i)
	{
		if(bStorageType == 0)
			item = victim->GetUpgradeInventoryItem(i);
		else if(bStorageType == 1)
			item = victim->GetBookInventoryItem(i);
		else if(bStorageType == 2)
			item = victim->GetStoneInventoryItem(i);
		else if(bStorageType == 3)
			item = victim->GetChangeInventoryItem(i);
		else if(bStorageType == 4)
			item = victim->GetCostumeInventoryItem(i);
		else if(bStorageType == 5)
			item = victim->GetChestInventoryItem(i);
		if(!item)
			continue;

		s_grid1.Put(i, 1, item->GetSize());
	}

	for (i = SPECIAL_INVENTORY_PAGE_SIZE*1; i < SPECIAL_INVENTORY_PAGE_SIZE*2; ++i)
	{
		if(bStorageType == 0)
			item = victim->GetUpgradeInventoryItem(i);
		else if(bStorageType == 1)
			item = victim->GetBookInventoryItem(i);
		else if(bStorageType == 2)
			item = victim->GetStoneInventoryItem(i);
		else if(bStorageType == 3)
			item = victim->GetChangeInventoryItem(i);
		else if(bStorageType == 4)
			item = victim->GetCostumeInventoryItem(i);
		else if(bStorageType == 5)
			item = victim->GetChestInventoryItem(i);
		if(!item)
			continue;

		s_grid2.Put(i - SPECIAL_INVENTORY_PAGE_SIZE*1, 1, item->GetSize());
	}

	for (i = SPECIAL_INVENTORY_PAGE_SIZE*2; i < SPECIAL_INVENTORY_PAGE_SIZE*3; ++i)
	{
		if (i >= victim->GetInventorySize(bStorageType+1))
		{
			s_grid3.Put(i - SPECIAL_INVENTORY_PAGE_SIZE*2, 1, 1);
			continue;
		}

		if(bStorageType == 0)
			item = victim->GetUpgradeInventoryItem(i);
		else if(bStorageType == 1)
			item = victim->GetBookInventoryItem(i);
		else if(bStorageType == 2)
			item = victim->GetStoneInventoryItem(i);
		else if(bStorageType == 3)
			item = victim->GetChangeInventoryItem(i);
		else if(bStorageType == 4)
			item = victim->GetCostumeInventoryItem(i);
		else if(bStorageType == 5)
			item = victim->GetChestInventoryItem(i);
		if(!item)
			continue;

		s_grid3.Put(i - SPECIAL_INVENTORY_PAGE_SIZE*2, 1, item->GetSize());
	}

	for (i = SPECIAL_INVENTORY_PAGE_SIZE*3; i < SPECIAL_INVENTORY_PAGE_SIZE*4; ++i)
	{
		if (i >= victim->GetInventorySize(bStorageType+1))
		{
			s_grid4.Put(i - SPECIAL_INVENTORY_PAGE_SIZE*3, 1, 1);
			continue;
		}

		if(bStorageType == 0)
			item = victim->GetUpgradeInventoryItem(i);
		else if(bStorageType == 1)
			item = victim->GetBookInventoryItem(i);
		else if(bStorageType == 2)
			item = victim->GetStoneInventoryItem(i);
		else if(bStorageType == 3)
			item = victim->GetChangeInventoryItem(i);
		else if(bStorageType == 4)
			item = victim->GetCostumeInventoryItem(i);
		else if(bStorageType == 5)
			item = victim->GetChestInventoryItem(i);
		if(!item)
			continue;

		s_grid4.Put(i - SPECIAL_INVENTORY_PAGE_SIZE*3, 1, item->GetSize());
	}
	
	/*
	for (i = SPECIAL_INVENTORY_PAGE_SIZE*4; i < SPECIAL_INVENTORY_PAGE_SIZE*5; ++i)
	{
		if(bStorageType == 0)
			item = victim->GetUpgradeInventoryItem(i);
		else if(bStorageType == 1)
			item = victim->GetBookInventoryItem(i);
		else if(bStorageType == 2)
			item = victim->GetStoneInventoryItem(i);
		else if(bStorageType == 3)
			item = victim->GetChangeInventoryItem(i);
		else if(bStorageType == 4)
			item = victim->GetCostumeInventoryItem(i);
		if(!item)
			continue;
		s_grid5.Put(i - SPECIAL_INVENTORY_PAGE_SIZE*4, 1, item->GetSize());
	}
	*/


	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!(item = m_apItems[i]))
			continue;
#ifdef UNLOCK_INVENTORY_WORLDARD
		int slots;
		if(item->IsUpgradeItem())
			slots = victim->GetEmptyUpgradeInventory(item);
		else if(item->IsBook())
			slots = victim->GetEmptyBookInventory(item);
		else if(item->IsStone())
			slots = victim->GetEmptyStoneInventory(item);
		else if(item->IsChange())
			slots = victim->GetEmptyChangeInventory(item);
		else if(item->IsCostume())
			slots = victim->GetEmptyCostumeInventory(item);
		else if(item->IsChest())
			slots = victim->GetEmptyChestInventory(item);
		if (slots < 0)
			return false;
#endif
		int iPos;
		if(bStorageType < 0 || bStorageType > 5)
			continue;
		if((bStorageType == 0 && (!item->IsUpgradeItem())) || (bStorageType == 1 && (!item->IsBook())) || (bStorageType == 2 && (!item->IsStone())) || (bStorageType == 3 && (!item->IsChange())) || (bStorageType == 4 && (!item->IsCostume())) || (bStorageType == 5 && (!item->IsChest())))
			continue;
		if ((iPos = s_grid1.FindBlank(1, item->GetSize())) >= 0)
			s_grid1.Put(iPos, 1, item->GetSize());
		else if((iPos = s_grid2.FindBlank(1, item->GetSize())) >= 0)
			s_grid2.Put(iPos, 1, item->GetSize());
		else if((iPos = s_grid3.FindBlank(1, item->GetSize())) >= 0)
			s_grid3.Put(iPos, 1, item->GetSize());
		else if((iPos = s_grid4.FindBlank(1, item->GetSize())) >= 0)
			s_grid4.Put(iPos, 1, item->GetSize());
		//else if((iPos = s_grid5.FindBlank(1, item->GetSize())) >= 0)
		//	s_grid5.Put(iPos, 1, item->GetSize());
		else{
			return false;
		}
	}

	return true;
}
#endif


bool CExchange::CheckSpace()
{
	static CGrid s_grid1(INVENTORY_PAGE_COLUMN, INVENTORY_PAGE_ROW); // inven page 1
	static CGrid s_grid2(INVENTORY_PAGE_COLUMN, INVENTORY_PAGE_ROW); // inven page 2
#ifdef ENABLE_EXTEND_INVEN_SYSTEM
	static CGrid s_grid3(INVENTORY_PAGE_COLUMN, INVENTORY_PAGE_ROW); // inven page 3
	static CGrid s_grid4(INVENTORY_PAGE_COLUMN, INVENTORY_PAGE_ROW); // inven page 4
#endif

	s_grid1.Clear();
	s_grid2.Clear();
#ifdef ENABLE_EXTEND_INVEN_SYSTEM
	s_grid3.Clear();
	s_grid4.Clear();
#endif

	LPCHARACTER	victim = GetCompany()->GetOwner();
	LPITEM item;
	int i;
	for (i = 0; i < INVENTORY_PAGE_SIZE*1; ++i)
	{
		if (!(item = victim->GetInventoryItem(i)))
			continue;
		s_grid1.Put(i, 1, item->GetSize());
	}
	for (i = INVENTORY_PAGE_SIZE*1; i < INVENTORY_PAGE_SIZE*2; ++i)
	{
		if (!(item = victim->GetInventoryItem(i)))
			continue;
		s_grid2.Put(i - INVENTORY_PAGE_SIZE*1, 1, item->GetSize());
	}
#ifdef ENABLE_EXTEND_INVEN_SYSTEM
	for (i = INVENTORY_PAGE_SIZE*2; i < INVENTORY_PAGE_SIZE*3; ++i)
	{
		if (i >= victim->GetInventorySize())
		{
			s_grid3.Put(i - INVENTORY_PAGE_SIZE*2, 1, 1);
			continue;
		}
		if (!(item = victim->GetInventoryItem(i)))
			continue;
		s_grid3.Put(i - INVENTORY_PAGE_SIZE*2, 1, item->GetSize());
	}
	for (i = INVENTORY_PAGE_SIZE*3; i < INVENTORY_PAGE_SIZE*4; ++i)
	{
		if (i >= victim->GetInventorySize())
		{
			s_grid4.Put(i - INVENTORY_PAGE_SIZE*3, 1, 1);
			continue;
		}
		if (!(item = victim->GetInventoryItem(i)))
			continue;
		s_grid4.Put(i - INVENTORY_PAGE_SIZE*3, 1, item->GetSize());
	}
#endif

	// ¾Æ... ¹º°¡ °³º´½Å °°Áö¸¸... ¿ëÈ¥¼® ÀÎº¥À» ³ë¸Ö ÀÎº¥ º¸°í µû¶ó ¸¸µç ³» Àß¸øÀÌ´Ù ¤Ð¤Ð
	static std::vector <WORD> s_vDSGrid(DRAGON_SOUL_INVENTORY_MAX_NUM);

	// ÀÏ´Ü ¿ëÈ¥¼®À» ±³È¯ÇÏÁö ¾ÊÀ» °¡´É¼ºÀÌ Å©¹Ç·Î, ¿ëÈ¥¼® ÀÎº¥ º¹»ç´Â ¿ëÈ¥¼®ÀÌ ÀÖÀ» ¶§ ÇÏµµ·Ï ÇÑ´Ù.
	bool bDSInitialized = false;

	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!(item = m_apItems[i]))
			continue;
		#ifdef UNLOCK_INVENTORY_WORLDARD
		int slots;
		if (item->IsDragonSoul())
			slots = victim->GetEmptyDragonSoulInventory(item);
		else
			slots = victim->GetEmptyInventory(item->GetSize());

		if (slots < 0)
			return false;
		#endif
		if (item->IsDragonSoul())
		{
			if (!victim->DragonSoul_IsQualified())
			{
				return false;
			}
			if (!bDSInitialized)
			{
				bDSInitialized = true;
				victim->CopyDragonSoulItemGrid(s_vDSGrid);
			}

			bool bExistEmptySpace = false;
			WORD wBasePos = DSManager::instance().GetBasePosition(item);
			if (wBasePos >= DRAGON_SOUL_INVENTORY_MAX_NUM)
				return false;

			for (int i = 0; i < DRAGON_SOUL_BOX_SIZE; i++)
			{
				WORD wPos = wBasePos + i;
				if (0 == s_vDSGrid[wPos])
				{
					bool bEmpty = true;
					for (int j = 1; j < item->GetSize(); j++)
					{
						if (s_vDSGrid[wPos + j * DRAGON_SOUL_BOX_COLUMN_NUM])
						{
							bEmpty = false;
							break;
						}
					}
					if (bEmpty)
					{
						for (int j = 0; j < item->GetSize(); j++)
						{
							s_vDSGrid[wPos + j * DRAGON_SOUL_BOX_COLUMN_NUM] =  wPos + 1;
						}
						bExistEmptySpace = true;
						break;
					}
				}
				if (bExistEmptySpace)
					break;
			}
			if (!bExistEmptySpace)
				return false;
		}
		else
		{
			int iPos;
			if ((iPos = s_grid1.FindBlank(1, item->GetSize())) >= 0)
				s_grid1.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid2.FindBlank(1, item->GetSize())) >= 0)
				s_grid2.Put(iPos, 1, item->GetSize());
#ifdef ENABLE_EXTEND_INVEN_SYSTEM
			else if ((iPos = s_grid3.FindBlank(1, item->GetSize())) >= 0)
				s_grid3.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid4.FindBlank(1, item->GetSize())) >= 0)
				s_grid4.Put(iPos, 1, item->GetSize());
#endif
			else
				return false;
		}
	}

	return true;
}

// ±³È¯ ³¡ (¾ÆÀÌÅÛ°ú µ· µîÀ» ½ÇÁ¦·Î ¿Å±ä´Ù)
bool CExchange::Done()
{
	int		empty_pos, i;
	LPITEM	item;

	LPCHARACTER	victim = GetCompany()->GetOwner();
#ifdef ENABLE_EXCHANGE_LOG
	char szQuery[QUERY_MAX_LEN];
#endif
	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!(item = m_apItems[i]))
			continue;

		if (item->IsDragonSoul())
			empty_pos = victim->GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_STORAGE
		else if(item->IsUpgradeItem())
			empty_pos = victim->GetEmptyUpgradeInventory(item);
		else if(item->IsBook())
			empty_pos = victim->GetEmptyBookInventory(item);
		else if(item->IsStone())
			empty_pos = victim->GetEmptyStoneInventory(item);
		else if(item->IsChange())
			empty_pos = victim->GetEmptyChangeInventory(item);
		else if(item->IsCostume())
			empty_pos = victim->GetEmptyCostumeInventory(item);
		else if(item->IsChest())
			empty_pos = victim->GetEmptyChestInventory(item);
#endif
		else
			empty_pos = victim->GetEmptyInventory(item->GetSize());

		if (empty_pos < 0)
		{
			sys_err("Exchange::Done : Cannot find blank position in inventory %s <-> %s item %s",
					m_pOwner->GetName(), victim->GetName(), item->GetName());
			continue;
		}

		assert(empty_pos >= 0);

#ifdef ENABLE_EXCHANGE_LOG
		int iLen = snprintf(szQuery, sizeof(szQuery), "INSERT INTO log.exchange_log_items(id, pid, item_id, pos, vnum, count");
		for (BYTE j=0;j< ITEM_SOCKET_MAX_NUM;++j)
			iLen += snprintf(szQuery+iLen, sizeof(szQuery)-iLen,", socket%u",j);
		for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
			iLen += snprintf(szQuery + iLen, sizeof(szQuery) - iLen, ", attrtype%u, attrvalue%u", j, j);
		iLen += snprintf(szQuery + iLen, sizeof(szQuery) - iLen, ") VALUES(%d, %u, %u, %d, %d, %d", logIndex, GetOwner()->GetPlayerID(), item->GetID(), m_abItemDisplayPos[i], item->GetVnum(), item->GetCount());
		for (BYTE j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
			iLen += snprintf(szQuery + iLen, sizeof(szQuery) - iLen, ", %ld", item->GetSocket(j));
		for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
			iLen += snprintf(szQuery + iLen, sizeof(szQuery) - iLen, ", %u, %d", item->GetAttributeType(j), item->GetAttributeValue(j));
		iLen += snprintf(szQuery + iLen, sizeof(szQuery) - iLen, ")");
		std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery(szQuery));
#endif

		if (item->GetVnum() == 90008 || item->GetVnum() == 90009) // VCARD
		{
			VCardUse(m_pOwner, victim, item);
			continue;
		}

		m_pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, item->GetCell(), WORD_MAX);

		item->RemoveFromCharacter();
		if (item->IsDragonSoul())
			item->AddToCharacter(victim, TItemPos(DRAGON_SOUL_INVENTORY, empty_pos));
#ifdef ENABLE_SPECIAL_STORAGE
		else if(item->IsUpgradeItem())
			item->AddToCharacter(victim, TItemPos(UPGRADE_INVENTORY, empty_pos));
		else if(item->IsBook())
			item->AddToCharacter(victim, TItemPos(BOOK_INVENTORY, empty_pos));
		else if(item->IsStone())
			item->AddToCharacter(victim, TItemPos(STONE_INVENTORY, empty_pos));
		else if(item->IsChange())
			item->AddToCharacter(victim, TItemPos(CHANGE_INVENTORY, empty_pos));
		else if(item->IsCostume())
			item->AddToCharacter(victim, TItemPos(COSTUME_INVENTORY, empty_pos));
		else if(item->IsChest())
			item->AddToCharacter(victim, TItemPos(CHEST_INVENTORY, empty_pos));
#endif
		else
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
		ITEM_MANAGER::instance().FlushDelayedSave(item);

		item->SetExchanging(false);
		{
			char exchange_buf[51];

			snprintf(exchange_buf, sizeof(exchange_buf), "%s %u %u", item->GetName(), GetOwner()->GetPlayerID(), item->GetCount());
			LogManager::instance().ItemLog(victim, item, "EXCHANGE_TAKE", exchange_buf);

			snprintf(exchange_buf, sizeof(exchange_buf), "%s %u %u", item->GetName(), victim->GetPlayerID(), item->GetCount());
			LogManager::instance().ItemLog(GetOwner(), item, "EXCHANGE_GIVE", exchange_buf);

			if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
			{
				LogManager::instance().GoldBarLog(victim->GetPlayerID(), item->GetID(), EXCHANGE_TAKE, "");
				LogManager::instance().GoldBarLog(GetOwner()->GetPlayerID(), item->GetID(), EXCHANGE_GIVE, "");
			}
#ifdef EXCHANGE_LOG
			bool firstPlayer = true;
			if (GetCompany())
			{
				firstPlayer = false;
			}

			DWORD tradeID = LogManager::instance().ExchangeLog(1, GetOwner()->GetPlayerID(), victim->GetPlayerID(), GetOwner()->GetX(), GetOwner()->GetY(), m_lldGold, GetCompany()->m_lldGold);
			LogManager::instance().ExchangeItemLog(tradeID, item, firstPlayer ? "A" : "B", empty_pos);
#endif
		}

		m_apItems[i] = NULL;
	}

	if (m_lldGold > 0)
	{
		GetOwner()->PointChange(POINT_GOLD, -m_lldGold, true);
		victim->PointChange(POINT_GOLD, m_lldGold, true);

		if (m_lldGold >= 1000000)
		{
			char szBuf[128] = {};
			snprintf(szBuf, sizeof(szBuf), "%u %s", GetOwner()->GetPlayerID(), GetOwner()->GetName());
			LogManager::instance().CharLog(victim, m_lldGold, "EXCHANGE_GOLD_TAKE", szBuf);

			snprintf(szBuf, sizeof(szBuf), "%u %s", victim->GetPlayerID(), victim->GetName());
			LogManager::instance().CharLog(GetOwner(), m_lldGold, "EXCHANGE_GOLD_GIVE", szBuf);
		}
	}

#ifdef USE_CHEQUE_CURRENCY
	if (m_lldCheque > 0)
	{
		GetOwner()->PointChange(POINT_CHEQUE, -m_lldCheque, true);
		victim->PointChange(POINT_CHEQUE, m_lldCheque, true);

		char szBuf[128] = {};
		snprintf(szBuf, sizeof(szBuf), "%u %s", GetOwner()->GetPlayerID(), GetOwner()->GetName());
		LogManager::instance().CharLog(victim, m_lldCheque, "EXCHANGE_CHEQUE_TAKE", szBuf);

		snprintf(szBuf, sizeof(szBuf), "%u %s", victim->GetPlayerID(), victim->GetName());
		LogManager::instance().CharLog(GetOwner(), m_lldCheque, "EXCHANGE_CHEQUE_GIVE", szBuf);
	}
#endif

#ifdef ENABLE_PREVENT_FLOOD_PACKETS
	GetOwner()->SetLastExchangeTime();
	victim->SetLastExchangeTime();
#endif

	m_pGrid->Clear();
	return true;
}

// ±³È¯À» µ¿ÀÇ
bool CExchange::Accept(bool bAccept)
{
	if (m_bAccept == bAccept)
		return true;

	m_bAccept = bAccept;

	// µÑ ´Ù µ¿ÀÇ ÇßÀ¸¹Ç·Î ±³È¯ ¼º¸³
	if (m_bAccept && GetCompany()->m_bAccept)
	{
		int	iItemCount;

		LPCHARACTER victim = GetCompany()->GetOwner();

		//PREVENT_PORTAL_AFTER_EXCHANGE
		GetOwner()->SetExchangeTime();
		victim->SetExchangeTime();
		//END_PREVENT_PORTAL_AFTER_EXCHANGE

		// @fixme150 BEGIN
		if (quest::CQuestManager::instance().GetPCForce(GetOwner()->GetPlayerID())->IsRunning() == true)
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "660");
			victim->ChatPacket(CHAT_TYPE_INFO, "661");
			goto EXCHANGE_END;
		}
		else if (quest::CQuestManager::instance().GetPCForce(victim->GetPlayerID())->IsRunning() == true)
		{
			victim->ChatPacket(CHAT_TYPE_INFO, "660");
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "661");
			goto EXCHANGE_END;
		}
		// @fixme150 END

		// exchange_check ¿¡¼­´Â ±³È¯ÇÒ ¾ÆÀÌÅÛµéÀÌ Á¦ÀÚ¸®¿¡ ÀÖ³ª È®ÀÎÇÏ°í,
		// ¿¤Å©µµ ÃæºÐÈ÷ ÀÖ³ª È®ÀÎÇÑ´Ù, µÎ¹øÂ° ÀÎÀÚ·Î ±³È¯ÇÒ ¾ÆÀÌÅÛ °³¼ö
		// ¸¦ ¸®ÅÏÇÑ´Ù.
		if (!Check(&iItemCount))
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "630");
			victim->ChatPacket(CHAT_TYPE_INFO, "662");
			goto EXCHANGE_END;
		}

		// ¸®ÅÏ ¹ÞÀº ¾ÆÀÌÅÛ °³¼ö·Î »ó´ë¹æÀÇ ¼ÒÁöÇ°¿¡ ³²Àº ÀÚ¸®°¡ ÀÖ³ª È®ÀÎÇÑ´Ù.
		if (!CheckSpace())
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "663");
			victim->ChatPacket(CHAT_TYPE_INFO, "638");
			goto EXCHANGE_END;
		}

		// »ó´ë¹æµµ ¸¶Âù°¡Áö·Î..
		if (!GetCompany()->Check(&iItemCount))
		{
			victim->ChatPacket(CHAT_TYPE_INFO, "630");
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "662");
			goto EXCHANGE_END;
		}

		if (!GetCompany()->CheckSpace())
		{
			victim->ChatPacket(CHAT_TYPE_INFO, "663");
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "638");
			goto EXCHANGE_END;
		}

#ifdef ENABLE_SPECIAL_STORAGE
		for(int i = 0; i < 5; i++)
		{
			if (!CheckSpecialStorageSpace(i))
			{
				GetOwner()->ChatPacket(CHAT_TYPE_INFO, "663");
				victim->ChatPacket(CHAT_TYPE_INFO, "638");
				goto EXCHANGE_END;
			}
			
			if (!GetCompany()->CheckSpecialStorageSpace(i))
			{
				victim->ChatPacket(CHAT_TYPE_INFO, "663");
				GetOwner()->ChatPacket(CHAT_TYPE_INFO, "638");
				goto EXCHANGE_END;
			}
		}
#endif
		
		if (db_clientdesc->GetSocket() == INVALID_SOCKET)
		{
			sys_err("Cannot use exchange feature while DB cache connection is dead.");
			victim->ChatPacket(CHAT_TYPE_INFO, "Unknown error");
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "Unknown error");
			goto EXCHANGE_END;
		}

		{
#ifdef ENABLE_EXCHANGE_LOG
			TExchangeLog p;
			strlcpy(p.owner, victim->GetName(), sizeof(p.owner));
			p.ownerPID = victim->GetPlayerID();
			p.ownerGold = GetCompany()->m_lldGold;
			strlcpy(p.ownerIP, victim->GetDesc()->GetHostName(), sizeof(p.ownerIP));

			strlcpy(p.target, GetOwner()->GetName(), sizeof(p.target));
			p.targetPID = GetOwner()->GetPlayerID();
			p.targetGold = m_lldGold;
			strlcpy(p.targetIP, GetOwner()->GetDesc()->GetHostName(), sizeof(p.targetIP));
			p.itemsLoaded = false;

			const time_t curr_time = time(0);
			const tm* curr_tm = localtime(&curr_time);
			strftime(p.date, 50, "%T - %d/%m/%y", curr_tm);

			//const bool isGameMasterExchange = (GetOwner()->IsGm() || victim->IsGm()) ? true: false;
			const bool isGameMasterExchange = false;//SHOW GAMEMASTER EXCHANGE IN GAME

			const int newLogID = quest::CQuestManager::instance().GetEventFlag("ex_log_index")+1;
			char szQuery[QUERY_MAX_LEN];
			snprintf(szQuery, sizeof(szQuery), "INSERT INTO log.exchange_log(id, owner, owner_pid, owner_gold, owner_ip, target, target_pid, target_gold, target_ip, date, owner_delete, target_delete) VALUES(%d, '%s', %u, %lld, '%s', '%s', %u, %lld, '%s', '%s', %d, %d)", newLogID, p.owner, p.ownerPID, p.ownerGold, p.ownerIP, p.target, p.targetPID, p.targetGold, p.targetIP, p.date, isGameMasterExchange, isGameMasterExchange);
			std::unique_ptr<SQLMsg> pMsgUpdate(DBManager::instance().DirectQuery(szQuery));
			quest::CQuestManager::instance().RequestSetEventFlag("ex_log_index", newLogID);
			logIndex = newLogID;
			GetCompany()->logIndex = newLogID;
			if(!isGameMasterExchange)
			{
				if (GetOwner()->GetProtectTime("ExchangeLogLoaded") == 1)
					GetOwner()->SendExchangeLogPacket(SUB_EXCHANGELOG_LOAD, newLogID, &p);
				if (victim->GetProtectTime("ExchangeLogLoaded") == 1)
					victim->SendExchangeLogPacket(SUB_EXCHANGELOG_LOAD, newLogID, &p);
			}
#endif
			if (Done())
			{
#ifdef USE_CHEQUE_CURRENCY
				bool bSaved = false;
#endif

			if (m_lldGold > 0)
			{
				GetOwner()->Save();
#ifdef USE_CHEQUE_CURRENCY
				bSaved = true;
#endif
			}

#ifdef USE_CHEQUE_CURRENCY
				if (!bSaved && m_lldCheque > 0)
				{
					GetOwner()->Save();
				}
#endif

				if (GetCompany()->Done())
				{
#ifdef USE_CHEQUE_CURRENCY
					bSaved = false;
#endif

					if (GetCompany()->m_lldGold > 0)
					{
						victim->Save();
#ifdef USE_CHEQUE_CURRENCY
						bSaved = true;
#endif
					}

#ifdef USE_CHEQUE_CURRENCY
					if (!bSaved && GetCompany()->m_lldCheque > 0)
					{
						victim->Save();
					}
#endif

					// INTERNATIONAL_VERSION
					GetOwner()->ChatPacket(CHAT_TYPE_INFO, "664 %s", victim->GetName());
					victim->ChatPacket(CHAT_TYPE_INFO, "664 %s", GetOwner()->GetName());
					// END_OF_INTERNATIONAL_VERSION
				}
			}
		}

EXCHANGE_END:
		Cancel();
		return false;
	}
	else
	{
		// ¾Æ´Ï¸é accept¿¡ ´ëÇÑ ÆÐÅ¶À» º¸³»ÀÚ.
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_ACCEPT, true, m_bAccept, NPOS, 0);
		exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_ACCEPT, false, m_bAccept, NPOS, 0);
		return true;
	}
}

// ±³È¯ Ãë¼Ò
void CExchange::Cancel()
{
	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_END, 0, 0, NPOS, 0);
	GetOwner()->SetExchange(NULL);

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (m_apItems[i])
			m_apItems[i]->SetExchanging(false);
	}

	if (GetCompany())
	{
		GetCompany()->SetCompany(NULL);
		GetCompany()->Cancel();
	}

	M2_DELETE(this);
}

