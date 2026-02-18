#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "char.h"
#include "desc.h"
#include "sectree_manager.h"
#include "packet.h"
#include "protocol.h"
#include "log.h"
#include "skill.h"
#include "unique_item.h"
#include "profiler.h"
#include "marriage.h"
#include "item_addon.h"
#include "char_manager.h"
#include "dev_log.h"
#include "locale_service.h"
#include "item.h"
#include "item_manager.h"
#include "affect.h"
#include "DragonSoul.h"
#include "questmanager.h"
#include "buff_on_attributes.h"
#include "belt_inventory_helper.h"
#include "../common/VnumHelper.h"
#include "../common/CommonDefines.h"
#include "MountSystem.h"
#ifdef ENABLE_NEW_PET_SYSTEM
#include "PetSystem.h"
#endif
#ifdef __BUFFI_SUPPORT__
#include "buffi.h"
#endif
#ifdef USE_CRYSTAL_SYSTEM
#include "arena.h"
#ifdef ENABLE_NEWSTUFF
#include "pvp.h"
#endif
#endif

CItem::CItem(DWORD dwVnum)
	: m_dwVnum(dwVnum), m_bWindow(0), m_dwID(0), m_bEquipped(false), m_dwVID(0), m_wCell(0), m_dwCount(0), 
#ifdef __SOULBINDING_SYSTEM__
		m_lBind(0),
		m_pkBindingExpireEvent(NULL), 
#endif
	m_lFlag(0), m_dwLastOwnerPID(0),
	m_bExchanging(false), m_pkDestroyEvent(NULL), m_pkExpireEvent(NULL), m_pkUniqueExpireEvent(NULL),
	m_pkTimerBasedOnWearExpireEvent(NULL), m_pkRealTimeExpireEvent(NULL),
   	m_pkAccessorySocketExpireEvent(NULL), m_pkOwnershipEvent(NULL), m_dwOwnershipPID(0), m_bSkipSave(false), m_isLocked(false),
	m_dwMaskVnum(0), m_dwSIGVnum (0)
#ifdef ENABLE_MULTI_CHEST
	, m_dwUpdateStatus (false)
#endif
#ifdef __AURA_SYSTEM__
	, m_pkAuraBoostSocketExpireEvent(NULL)
#endif
{
	memset( &m_alSockets, 0, sizeof(m_alSockets) );
	memset( &m_aAttr, 0, sizeof(m_aAttr) );
#ifdef ENABLE_YOHARA_SYSTEM
	memset(&m_aApplyRandom, 0, sizeof(m_aApplyRandom));
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	strlcpy(nameex,"^",sizeof(nameex));
#endif
#ifdef USE_EXPANDED_ITEM_REFRESH
     m_bSkipPacket = false;
#endif
#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	m_bSkipUpdatePacket = false;
#endif
}

CItem::~CItem()
{
	Destroy();
}

void CItem::Initialize()
{
	CEntity::Initialize(ENTITY_ITEM);
#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	m_bSkipUpdatePacket = false;
#endif

	m_bWindow = RESERVED_WINDOW;
	m_pOwner = NULL;
	m_dwID = 0;
	m_bEquipped = false;
	m_dwVID = m_wCell = m_dwCount = m_lFlag = 0;
	m_pProto = NULL;
	m_bExchanging = false;

#ifdef __AURA_SYSTEM__
	m_pkAuraBoostSocketExpireEvent = NULL;
#endif
	memset(&m_alSockets, 0, sizeof(m_alSockets));
	memset(&m_aAttr, 0, sizeof(m_aAttr));
#ifdef ENABLE_NEW_NAME_ITEM
	strlcpy(nameex,"^",sizeof(nameex));
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	memset(&m_aApplyRandom, 0, sizeof(m_aApplyRandom));
#endif

#ifdef ENABLE_MULTI_CHEST
	m_dwUpdateStatus = false;
#endif

#ifdef __SOULBINDING_SYSTEM__
	m_lBind = 0;
	m_pkBindingExpireEvent = NULL;
#endif

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	m_stName = "I" + std::to_string(GetVnum());
#endif

	m_pkDestroyEvent = NULL;
	m_pkOwnershipEvent = NULL;
	m_dwOwnershipPID = 0;
	m_pkUniqueExpireEvent = NULL;

	m_pkTimerBasedOnWearExpireEvent = NULL;
	m_pkRealTimeExpireEvent = NULL;

	m_pkAccessorySocketExpireEvent = NULL;

	m_bSkipSave = false;
#ifdef USE_EXPANDED_ITEM_REFRESH
     m_bSkipPacket = false;
#endif

	m_dwLastOwnerPID = 0;
#ifdef __BL_BATTLE_ROYALE__
	m_NeverSave = false;
#endif
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	destroyTime = -1;
#endif
}

void CItem::Destroy()
{
	event_cancel(&m_pkDestroyEvent);
	event_cancel(&m_pkOwnershipEvent);
	event_cancel(&m_pkUniqueExpireEvent);
	event_cancel(&m_pkTimerBasedOnWearExpireEvent);
	event_cancel(&m_pkRealTimeExpireEvent);
#ifdef __AURA_SYSTEM__
	event_cancel(&m_pkAuraBoostSocketExpireEvent);
#endif
#ifdef __SOULBINDING_SYSTEM__
	event_cancel(&m_pkBindingExpireEvent);
#endif
	CEntity::Destroy();

	if (GetSectree())
		GetSectree()->RemoveEntity(this);
}

EVENTFUNC(item_destroy_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "item_destroy_event> <Factor> Null pointer" );
		return 0;
	}

	LPITEM pkItem = info->item;

	if (pkItem->GetOwner())
		sys_err("item_destroy_event: Owner exist. (item %s owner %s)", pkItem->GetName(), pkItem->GetOwner()->GetName());

	pkItem->SetDestroyEvent(NULL);
	M2_DESTROY_ITEM(pkItem);
	return 0;
}

void CItem::SetDestroyEvent(LPEVENT pkEvent)
{
	m_pkDestroyEvent = pkEvent;
}

void CItem::StartDestroyEvent(int iSec)
{
	if (m_pkDestroyEvent)
		return;
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	destroyTime = time(0)+iSec;
	AddToGround(dropMapIndex, dropPos, dropskipOwnerCheck);
#endif
	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetDestroyEvent(event_create(item_destroy_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::EncodeInsertPacket(LPENTITY ent)
{
	LPDESC d;

	if (!(d = ent->GetDesc()))
		return;

	const PIXEL_POSITION & c_pos = GetXYZ();

	struct packet_item_ground_add pack;

	pack.bHeader	= HEADER_GC_ITEM_GROUND_ADD;
	pack.x		= c_pos.x;
	pack.y		= c_pos.y;
	pack.z		= c_pos.z;
	pack.dwVnum		= GetVnum();
	pack.dwVID		= m_dwVID;
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	pack.itemCount = GetCount();
	pack.ownerTime = destroyTime-time(0);
	
	for (size_t i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		pack.alSockets[i] = GetSocket(i);

	std::memcpy(pack.aAttrs, GetAttributes(), sizeof(pack.aAttrs));
#endif
	//pack.count	= m_dwCount;


	d->Packet(&pack, sizeof(pack));

	if (m_pkOwnershipEvent != NULL)
	{
		item_event_info * info = dynamic_cast<item_event_info *>(m_pkOwnershipEvent->info);

		if ( info == NULL )
		{
			sys_err( "CItem::EncodeInsertPacket> <Factor> Null pointer" );
			return;
		}

		TPacketGCItemOwnership p;

		p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
		p.dwVID = m_dwVID;
		strlcpy(p.szName, info->szOwnerName, sizeof(p.szName));

		d->Packet(&p, sizeof(TPacketGCItemOwnership));
	}
}

void CItem::EncodeRemovePacket(LPENTITY ent)
{
	LPDESC d;

	if (!(d = ent->GetDesc()))
		return;

	struct packet_item_ground_del pack;

	pack.bHeader	= HEADER_GC_ITEM_GROUND_DEL;
	pack.dwVID		= m_dwVID;

	d->Packet(&pack, sizeof(pack));
	sys_log(2, "Item::EncodeRemovePacket %s to %s", GetName(), ((LPCHARACTER) ent)->GetName());
}

void CItem::SetProto(const TItemTable * table)
{
	assert(table != NULL);
	m_pProto = table;
	SetFlag(m_pProto->dwFlags);
}

void CItem::UsePacketEncode(LPCHARACTER ch, LPCHARACTER victim, struct packet_item_use *packet)
{
	if (!GetVnum())
		return;

	packet->header 	= HEADER_GC_ITEM_USE;
	packet->ch_vid 	= ch->GetVID();
	packet->victim_vid 	= victim->GetVID();
	packet->Cell = TItemPos(GetWindow(), m_wCell);
	packet->vnum	= GetVnum();
}

void CItem::RemoveFlag(long bit)
{
	REMOVE_BIT(m_lFlag, bit);
}

void CItem::AddFlag(long bit)
{
	SET_BIT(m_lFlag, bit);
}

#ifdef USE_EXPANDED_ITEM_REFRESH
void CItem::UpdatePacket(bool bJustCount/* = false*/)
#else
void CItem::UpdatePacket()
#endif
{
	
#ifdef ENABLE_SWITCHBOT
	if (m_bWindow == SWITCHBOT)
		return;
#endif
	
#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	if (m_bSkipUpdatePacket)
		return;
#endif

// #ifdef ENABLE_MULTI_CHEST
    if (GetUpdateStatus())
    {
        return;
    }
// #endif

#ifdef USE_EXPANDED_ITEM_REFRESH
    if (GetSkipSave())
    {
        return;
    }
#endif

    LPDESC d = m_pOwner ? m_pOwner->GetDesc() : nullptr;
    if (!d)
    {
        return;
    }

#ifdef USE_EXPANDED_ITEM_REFRESH
    if (bJustCount)
    {
        TPacketGCItemUpdateCount p2;
        p2.header = HEADER_GC_ITEM_UPDATE_COUNT;
        p2.pos = TItemPos(GetWindow(), m_wCell);
        p2.count = m_dwCount;

        d->Packet(&p2, sizeof(TPacketGCItemUpdateCount));
        sys_log(2, "UpdatePacketCount %s -> %s", GetName(), m_pOwner->GetName());
        return;
    }
#endif

    TPacketGCItemUpdate p;
    p.header = HEADER_GC_ITEM_UPDATE;
    p.Cell = TItemPos(GetWindow(), m_wCell);
    p.count = m_dwCount;
#ifdef ENABLE_NEW_NAME_ITEM
    strlcpy(p.name,nameex,sizeof(p.name));
#endif

    for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
    {
        p.alSockets[i] = m_alSockets[i];
    }

    std::memcpy(p.aAttr, GetAttributes(), sizeof(p.aAttr));

#ifdef ENABLE_YOHARA_SYSTEM
    std::memcpy(p.aApplyRandom, GetApplysRandom(), sizeof(p.aApplyRandom));
#endif

#ifdef __SOULBINDING_SYSTEM__
    p.bind = m_lBind;
#endif

    d->Packet(&p, sizeof(TPacketGCItemUpdate));
    sys_log(2, "UpdatePacket %s -> %s", GetName(), m_pOwner->GetName());
}

#ifdef __SOULBINDING_SYSTEM__
void CItem::Bind(long lTime)
{
	if (lTime >= 0)
	{
		m_lBind = lTime;
		UpdatePacket();
		Save();
	}
}

bool CItem::IsBind()
{
	if (GetBind() == 1)
		return true;
	
	return false;
}

bool CItem::IsUntilBind()
{
	if (GetBind() >= 2)
		return true;
	
	return false;
}

EVENTFUNC(unbinding_expire_event)
{
	const item_vid_event_info * pInfo = reinterpret_cast<item_vid_event_info*>(event->info);
	if (!pInfo)
		return 0;
	
	const LPITEM pItem = ITEM_MANAGER::instance().FindByVID(pInfo->item_vid);
	if (!pItem)
		return 0;
	
	int iNextExpire = pItem->GetBind() - processing_time / passes_per_sec;
	if (iNextExpire <= 2)
	{
		pItem->Bind(0);
		pItem->SetUnBindingExpireEvent(NULL);
		return 0;
	}
	
	pItem->Bind(iNextExpire);
	return PASSES_PER_SEC(MIN(60, iNextExpire));
}

void CItem::SetUnBindingExpireEvent(LPEVENT pkEvent)
{
	m_pkBindingExpireEvent = pkEvent;
}

void CItem::StartUnBindingExpireEvent()
{
	if (m_pkBindingExpireEvent)
		return;
	
	item_vid_event_info * pInfo = AllocEventInfo<item_vid_event_info>();
	pInfo->item_vid = GetVID();
	SetUnBindingExpireEvent(event_create(unbinding_expire_event, pInfo, PASSES_PER_SEC(60)));
}
#endif


DWORD CItem::GetCount()
{
	if (GetType() == ITEM_ELK) return MIN(m_dwCount, INT_MAX);
	else
	{
		return MIN(m_dwCount, g_bItemCountLimit);
	}
}

bool CItem::SetCount(DWORD count)
{
#ifdef USE_EXPANDED_ITEM_REFRESH
    const auto oldcount = m_dwCount;
#endif

	if (GetType() == ITEM_ELK)
	{
		m_dwCount = MIN(count, INT_MAX);
	}
	else
	{
		m_dwCount = MIN(count, g_bItemCountLimit);
	}

	if (count == 0 && m_pOwner)
	{
		if (GetSubType() == USE_ABILITY_UP || GetSubType() == USE_POTION || GetVnum() == 70020)
		{
			LPCHARACTER pOwner = GetOwner();
			WORD wCell = GetCell();

			RemoveFromCharacter();

			if (!IsDragonSoul())
			{
				LPITEM pItem = pOwner->FindSpecifyItem(GetVnum());

				if (NULL != pItem)
				{
					pOwner->ChainQuickslotItem(pItem, QUICKSLOT_TYPE_ITEM, wCell);
				}
				else
				{
					pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, wCell, WORD_MAX);
				}
			}

			M2_DESTROY_ITEM(this);
		}
		else
		{
			if (!IsDragonSoul())
			{
				m_pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, m_wCell, WORD_MAX);
			}
			M2_DESTROY_ITEM(RemoveFromCharacter());
		}

		return true;
	}

#ifdef USE_EXPANDED_ITEM_REFRESH
    if (oldcount != m_dwCount)
    {
        UpdatePacket(true);
    }
#else
    UpdatePacket();
#endif

	Save();
	return true;
}

LPITEM CItem::RemoveFromCharacter()
{
	if (!m_pOwner)
	{
		sys_err("Item::RemoveFromCharacter owner null");
		return (this);
	}

	LPCHARACTER pOwner = m_pOwner;

	if (m_bEquipped)	// ÀåÂøµÇ¾ú´Â°¡?
	{
		Unequip();
		//pOwner->UpdatePacket();

		SetWindow(RESERVED_WINDOW);
		Save();
		return (this);
	}
	else
	{
		if (GetWindow() != SAFEBOX && GetWindow() != MALL
#ifdef ENABLE_IKASHOP_RENEWAL
			&& GetWindow() != IKASHOP_OFFLINESHOP 
			&& GetWindow() != IKASHOP_AUCTION
			&& GetWindow() != IKASHOP_SAFEBOX
#endif
		)
		{
			if (IsDragonSoul())
			{
				if (m_wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= DRAGON_SOUL_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), NULL);
			}
#ifdef ENABLE_SWITCHBOT_WORLDARD
			else if (GetWindow() == SWITCHBOT)
			{
				if (m_wCell >= SWITCHBOT_SLOT_MAX)
					sys_err("CItem::RemoveFromCharacter: pos >= SWITCHBOT_SLOT_MAX");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), NULL);
			}
#endif
#ifdef CHANGE_EQUIP_WORLDARD
			else if (m_bWindow == CHANGE_EQUIP)
			{
				if (m_wCell >= CHANGE_EQUIP_SLOT_COUNT)
				{
					sys_err("CItem::RemoveFromCharacter: pos >= CHANGE_EQUIP_SLOT_COUNT");
				}
				else
				{
					pOwner->SetItem(TItemPos(CHANGE_EQUIP, m_wCell), NULL);
				}
			}
#endif
#ifdef ENABLE_SPECIAL_STORAGE
			else if (IsUpgradeItem() || IsBook() || IsStone() || IsChange() || (IsCostume() || IsCostumeSkin()) || IsChest())
			{
				if (m_wCell >= SPECIAL_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= SPECIAL_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), NULL);
			}
#endif
			else
			{
				TItemPos cell(INVENTORY, m_wCell);

				if (false == cell.IsDefaultInventoryPosition() && false == cell.IsBeltInventoryPosition()) // ¾Æ´Ï¸é ¼ÒÁöÇ°¿¡?
					sys_err("CItem::RemoveFromCharacter: Invalid Item Position");
				else
				{
					pOwner->SetItem(cell, NULL);
				}
			}
		}

		m_pOwner = NULL;
		m_wCell = 0;

		SetWindow(RESERVED_WINDOW);
		Save();
		return (this);
	}
}

bool CItem::AddToCharacter(LPCHARACTER ch, TItemPos Cell)
{
	assert(GetSectree() == NULL);
	assert(m_pOwner == NULL);
	WORD pos = Cell.cell;
	BYTE window_type = Cell.window_type;

	if (INVENTORY == window_type)
	{
		if (m_wCell >= INVENTORY_MAX_NUM && BELT_INVENTORY_SLOT_START > m_wCell)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
	else if (DRAGON_SOUL_INVENTORY == window_type)
	{
		if (m_wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}

#ifdef ENABLE_SWITCHBOT_WORLDARD
	else if (SWITCHBOT == window_type)
	{
		if (m_wCell >= SWITCHBOT_SLOT_MAX){
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
#endif
#ifdef CHANGE_EQUIP_WORLDARD
	else if (CHANGE_EQUIP == window_type)
	{
		if (m_wCell >= CHANGE_EQUIP_SLOT_COUNT)
		{
			sys_err("CItem::AddToCharacter:switchbot cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
#endif

#ifdef ENABLE_SPECIAL_STORAGE
	else if (UPGRADE_INVENTORY == window_type || BOOK_INVENTORY == window_type || STONE_INVENTORY == window_type || CHANGE_INVENTORY == window_type || COSTUME_INVENTORY == window_type || CHEST_INVENTORY == window_type)
	{
		if (m_wCell >= SPECIAL_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
#endif
#ifdef ENABLE_NEWITEM_AFFECT
	bool bHighlight = false;
	if (ch->GetDesc())
	{
		bHighlight = m_dwLastOwnerPID != ch->GetPlayerID();
		m_dwLastOwnerPID = ch->GetPlayerID();
	}
#endif

#ifdef __SASH_SYSTEM__
	if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH) && (GetSocket(SASH_ABSORPTION_SOCKET) == 0))
	{
		long lVal = GetValue(SASH_GRADE_VALUE_FIELD);
		switch (lVal)
		{
			case 2:
				{
					lVal = SASH_GRADE_2_ABS;
				}
				break;
			case 3:
				{
					lVal = SASH_GRADE_3_ABS;
				}
				break;
			case 4:
				{
					lVal = number(SASH_GRADE_4_ABS_MIN, SASH_GRADE_4_ABS_MAX_COMB);
				}
				break;
			default:
				{
					lVal = SASH_GRADE_1_ABS;
				}
				break;
		}
		
		SetSocket(SASH_ABSORPTION_SOCKET, lVal);
	}
	#endif
	event_cancel(&m_pkDestroyEvent);

#ifdef ENABLE_NEWITEM_AFFECT
	ch->SetItem(TItemPos(window_type, pos), this, bHighlight);
#else
	ch->SetItem(TItemPos(window_type, pos), this);
#endif

	m_pOwner = ch;

	Save();
	return true;
}

LPITEM CItem::RemoveFromGround()
{
	if (GetSectree())
	{
		SetOwnership(NULL);

		GetSectree()->RemoveEntity(this);

		ViewCleanup();

		Save();
	}

	return (this);
}

bool CItem::AddToGround(long lMapIndex, const PIXEL_POSITION & pos, bool skipOwnerCheck)
{
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	if(destroyTime == -1)
	{
		std::memcpy(&dropPos, &pos, sizeof(dropPos));
		dropMapIndex = lMapIndex;
		dropskipOwnerCheck = skipOwnerCheck;
		return true;
	}
#endif

	if (0 == lMapIndex)
	{
		sys_err("wrong map index argument: %d", lMapIndex);
		return false;
	}

	if (GetSectree())
	{
		sys_err("sectree already assigned vnum: %d mapIndex: %ld",GetVnum(), lMapIndex);

		UpdateSectree();

		return false;
	}

	if (!skipOwnerCheck && m_pOwner)
	{
		sys_err("owner pointer not null");
		return false;
	}

	LPSECTREE tree = SECTREE_MANAGER::instance().Get(lMapIndex, pos.x, pos.y);

	if (!tree)
	{
		sys_err("cannot find sectree by %dx%d", pos.x, pos.y);
		return false;
	}

	//tree->Touch();

	SetWindow(GROUND);
	SetXYZ(pos.x, pos.y, pos.z);
	tree->InsertEntity(this);
	UpdateSectree();
	Save();
	return true;
}

bool CItem::DistanceValid(LPCHARACTER ch, bool bIsParty)
{
	if (!ch || !GetSectree())
	{
		return false;
	}

	const int iLimit = bIsParty ? 3600 : 1800;
	const int iDist = DISTANCE_APPROX(GetX() - ch->GetX(), GetY() - ch->GetY());
	if (iDist > iLimit)
	{
		return false;
	}

/*
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	const WORD distance = ch->FindAffect(AFFECT_AUTO_PICK_UP) != NULL ? 1800 :  1800;
	if (iDist > distance)
#else
	if (iDist > 1800)
#endif
		return false;
*/

	return true;
}

bool CItem::CanUsedBy(LPCHARACTER ch)
{
#ifdef __BUFFI_SUPPORT__
	if (ch->IsLookingBuffiPage() && GetType() == ITEM_COSTUME && GetSubType() == COSTUME_WEAPON)
	{
		if (GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
			return false;
		return true;
	}
#endif

	// Anti flag check
	switch (ch->GetJob())
	{
		case JOB_WARRIOR:
			if (GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR)
				return false;
			break;

		case JOB_ASSASSIN:
			if (GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN)
				return false;
			break;

		case JOB_SHAMAN:
			if (GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
				return false;
			break;

		case JOB_SURA:
			if (GetAntiFlag() & ITEM_ANTIFLAG_SURA)
				return false;
			break;
#ifdef ENABLE_WOLFMAN_CHARACTER
		case JOB_WOLFMAN:
			if (GetAntiFlag() & ITEM_ANTIFLAG_WOLFMAN)
				return false;
			break;
#endif
	}

	return true;
}

int CItem::FindEquipCell(LPCHARACTER ch, int iCandidateCell)
{
	// ÄÚ½ºÃõ ¾ÆÀÌÅÛ(ITEM_COSTUME)Àº WearFlag ¾ø¾îµµ µÊ. (sub typeÀ¸·Î Âø¿ëÀ§Ä¡ ±¸ºÐ. ±ÍÂú°Ô ¶Ç wear flag ÁÙ ÇÊ¿ä°¡ ÀÖ³ª..)
	// ¿ëÈ¥¼®(ITEM_DS, ITEM_SPECIAL_DS)µµ  SUB_TYPEÀ¸·Î ±¸ºÐ. ½Å±Ô ¹ÝÁö, º§Æ®´Â ITEM_TYPEÀ¸·Î ±¸ºÐ -_-

	if ((0 == GetWearFlag() || ITEM_TOTEM == GetType()) && ITEM_COSTUME != GetType() && ITEM_DS != GetType() && ITEM_SPECIAL_DS != GetType() && ITEM_RING != GetType() && ITEM_BELT != GetType() && ITEM_NEW_RINGS != GetType()
#ifdef __ITEM_SHINING__
		&& ITEM_SHINING != GetType()
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
		&& ITEM_PET != GetType()
#endif
		&& 	ITEM_NEWRING != GetType()
		&& 	ITEM_RING_SPECIAL != GetType()
		&& 	ITEM_BOOSTER != GetType()
	)
		return -1;

	// ¿ëÈ¥¼® ½½·ÔÀ» WEAR·Î Ã³¸®ÇÒ ¼ö°¡ ¾ø¾î¼­(WEAR´Â ÃÖ´ë 32°³±îÁö °¡´ÉÇÑµ¥ ¿ëÈ¥¼®À» Ãß°¡ÇÏ¸é 32°¡ ³Ñ´Â´Ù.)
	// ÀÎº¥Åä¸®ÀÇ Æ¯Á¤ À§Ä¡((INVENTORY_MAX_NUM + WEAR_MAX_NUM)ºÎÅÍ (INVENTORY_MAX_NUM + WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX - 1)±îÁö)¸¦
	// ¿ëÈ¥¼® ½½·ÔÀ¸·Î Á¤ÇÔ.
	// return ÇÒ ¶§¿¡, INVENTORY_MAX_NUMÀ» »« ÀÌÀ¯´Â,
	// º»·¡ WearCellÀÌ INVENTORY_MAX_NUM¸¦ »©°í return ÇÏ±â ¶§¹®.
	if (GetType() == ITEM_DS || GetType() == ITEM_SPECIAL_DS)
	{
		if (iCandidateCell < 0)
		{
			return WEAR_MAX_NUM + GetSubType();
		}
		else
		{
			for (int i = 0; i < DRAGON_SOUL_DECK_MAX_NUM; i++)
			{
				if (WEAR_MAX_NUM + i * DS_SLOT_MAX + GetSubType() == iCandidateCell)
				{
					return iCandidateCell;
				}
			}
			return -1;
		}
	}

#ifdef __ITEM_SHINING__
	else if (GetType() == ITEM_SHINING)
	{
#ifdef __BUFFI_SHINING__
		LPCHARACTER ch = GetOwner();
		if (ch && ch->IsLookingBuffiPage())
		{
			if (GetSubType() == SHINING_WEAPON)
				return WEAR_BUFFI_SHINING_WEAPON;
			else if (GetSubType() == SHINING_ARMOR)
				return WEAR_BUFFI_SHINING_ARMOR;
		}
		else
#endif
		{
			if (GetSubType() == SHINING_WEAPON)
				return WEAR_SHINING_WEAPON_1;
			else if (GetSubType() == SHINING_ARMOR)
				return WEAR_SHINING_ARMOR_1;
		}
		if (GetSubType() == SHINING_SPECIAL)
			return WEAR_SHINING_SPECIAL;
	}
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
	else if (IsNewPetItem())
		return WEAR_PET;
#endif
	else if (GetType() == ITEM_NEW_RINGS)
	{
		if (ch->GetWear(WEAR_RINGS_1))
			return WEAR_RINGS_2;
		else
			return WEAR_RINGS_1;
	}

	else if (GetType() == ITEM_COSTUME)
	{
#ifdef __BUFFI_SUPPORT__
		LPCHARACTER ch = GetOwner();
		if (ch && ch->IsLookingBuffiPage())
		{
			//not teach these method
			if (GetSubType() == COSTUME_BODY)
			{
				if (!IS_SET(GetAntiFlag(), ITEM_ANTIFLAG_MALE))
					return -1;
				return WEAR_BUFFI_BODY;
			}
			else if (GetSubType() == COSTUME_HAIR)
			{
				if (!IS_SET(GetAntiFlag(), ITEM_ANTIFLAG_MALE))
					return -1;
				return WEAR_BUFFI_HEAD;
			}
			else if (GetSubType() == COSTUME_WEAPON)
				return WEAR_BUFFI_WEAPON;
			else if (GetSubType() == COSTUME_SASH)
				return WEAR_BUFFI_SASH;
		}
		else
#endif
		{
			if (GetSubType() == COSTUME_BODY)
				return WEAR_COSTUME_BODY;
			else if (GetSubType() == COSTUME_HAIR)
				return WEAR_COSTUME_HAIR;
#ifdef __SASH_SYSTEM__
			else if (GetSubType() == COSTUME_SASH)
				return WEAR_COSTUME_SASH;
#endif
		}
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
			if (GetSubType() == COSTUME_MOUNT)
				return WEAR_COSTUME_MOUNT;
#endif
#ifdef ENABLE_SASH_COSTUME_SYSTEM
			else if (GetSubType() == COSTUME_NEW_SASH)
				return WEAR_COSTUME_NEW_SASH;
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
			else if (GetSubType() == COSTUME_WEAPON)
				return WEAR_COSTUME_WEAPON;
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
			else if (GetSubType() == COSTUME_PET)
				return WEAR_OLD_PET;
#endif
#ifdef ENABLE_MOUNT_SKIN
			else if (GetSubType() == COSTUME_MOUNT_SKIN)
				return WEAR_COSTUME_MOUNT_SKIN;
#endif
#ifdef __AURA_SYSTEM__
		else if (GetSubType() == COSTUME_AURA)
			return WEAR_COSTUME_AURA;
#endif
	}
#if !defined(ENABLE_MOUNT_COSTUME_SYSTEM) && !defined(__SASH_SYSTEM__)
	else if (GetType() == ITEM_RING)
	{
		if (ch->GetWear(WEAR_RING1))
			return WEAR_RING2;
		else
			return WEAR_RING1;
	}
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	else if (GetType() == ITEM_ARMOR && GetSubType() == ARMOR_GLOVE)
		return WEAR_GLOVE;
#endif

	else if (GetType() == ITEM_RING_SPECIAL)
	{
		// if (ch->GetWear(WEAR_SPECIAL_RING1))
			// return WEAR_SPECIAL_RING2;
		// else
			// return WEAR_SPECIAL_RING1;

		if (GetSubType() == SPECIAL_RING_SLOT1)
			return WEAR_SPECIAL_RING1;
		else if (GetSubType() == SPECIAL_RING_SLOT2)
			return WEAR_SPECIAL_RING2;
	}


#ifdef ENABLE_NEWEQ
	else if (GetType() == ITEM_NEWRING)
	{
		if (GetSubType() == RING_SLOT1)
			return WEAR_RING_SLOT1;
		else if (GetSubType() == RING_SLOT2)
			return WEAR_RING_SLOT2;
		else if (GetSubType() == RING_SLOT3)
			return WEAR_RING_SLOT3;
		else if (GetSubType() == RING_SLOT4)
			return WEAR_RING_SLOT4;
		else if (GetSubType() == RING_SLOT5)
			return WEAR_RING_SLOT5; //marriage 1
		else if (GetSubType() == RING_SLOT6)
			return WEAR_RING_SLOT6; //marriage 2
		else if (GetSubType() == RING_SLOT7)
			return WEAR_RING_SLOT7; //marriage 2
	}
#endif
	else if (GetType() == ITEM_BELT)
		return WEAR_BELT;
	else if (GetWearFlag() & WEARABLE_BODY)
		return WEAR_BODY;
	else if (GetWearFlag() & WEARABLE_HEAD)
		return WEAR_HEAD;
	else if (GetWearFlag() & WEARABLE_FOOTS)
		return WEAR_FOOTS;
	else if (GetWearFlag() & WEARABLE_WRIST)
		return WEAR_WRIST;
	else if (GetWearFlag() & WEARABLE_WEAPON)
		return WEAR_WEAPON;
	else if (GetWearFlag() & WEARABLE_SHIELD)
		return WEAR_SHIELD;
	else if (GetWearFlag() & WEARABLE_NECK)
		return WEAR_NECK;
	else if (GetWearFlag() & WEARABLE_EAR)
		return WEAR_EAR;
	else if (GetWearFlag() & WEARABLE_ARROW)
		return WEAR_ARROW;

	else if (GetWearFlag() & WEARABLE_UNIQUE)
	{
		if (ch->GetWear(WEAR_UNIQUE1))
			return WEAR_UNIQUE2;
		else
			return WEAR_UNIQUE1;
	}

	else if (GetType() == ITEM_BOOSTER)
	{
		if (GetSubType() == BOOSTER_HEAD)
			return WEAR_BOOST_HEAD;
		else if (GetSubType() == BOOSTER_ARMOR)
			return WEAR_BOOST_ARMOR;
		else if (GetSubType() == BOOSTER_WEAPON)
			return WEAR_BOOST_WEAPON;
	}

	else if (GetType() == ITEM_ARMOR && GetSubType() >= ARMOR_PENDANT_FIRE && GetSubType() <= ARMOR_PENDANT_SOUL)
	{
		switch (GetSubType())
		{
			case ARMOR_PENDANT_FIRE:
				return WEAR_PENDANT_FIRE;
			case ARMOR_PENDANT_ICE:
				return WEAR_PENDANT_ICE;
			case ARMOR_PENDANT_THUNDER:
				return WEAR_PENDANT_THUNDER;
			case ARMOR_PENDANT_WIND:
				return WEAR_PENDANT_WIND;
			case ARMOR_PENDANT_DARK:
				return WEAR_PENDANT_DARK;
			case ARMOR_PENDANT_EARTH:
				return WEAR_PENDANT_EARTH;
			case ARMOR_PENDANT_SOUL:
				if (GetVnum() == 91066)
					return WEAR_PENDANT_SOUL_FIRST;
				else if (GetVnum() == 91067)
					return WEAR_PENDANT_SOUL_SECOND;
		}
		return -1;
	}

	else if (GetWearFlag() & WEARABLE_ABILITY)
	{
		if (!ch->GetWear(WEAR_ABILITY1))
		{
			return WEAR_ABILITY1;
		}
		else if (!ch->GetWear(WEAR_ABILITY2))
		{
			return WEAR_ABILITY2;
		}
		else if (!ch->GetWear(WEAR_ABILITY3))
		{
			return WEAR_ABILITY3;
		}
		else if (!ch->GetWear(WEAR_ABILITY4))
		{
			return WEAR_ABILITY4;
		}
		else if (!ch->GetWear(WEAR_ABILITY5))
		{
			return WEAR_ABILITY5;
		}
		else if (!ch->GetWear(WEAR_ABILITY6))
		{
			return WEAR_ABILITY6;
		}
		else if (!ch->GetWear(WEAR_ABILITY7))
		{
			return WEAR_ABILITY7;
		}
		else if (!ch->GetWear(WEAR_ABILITY8))
		{
			return WEAR_ABILITY8;
		}
		else
		{
			return -1;
		}
	}
	return -1;
}
// #ifdef ENABLE_PENDANT_SYSTEM_RENEWAL
// BYTE CItem::GetTalismanType()
// {
	// if (GetVnum() >= 9600 && GetVnum() <= 9800)
		// return PENDANT_FIRE;

	// else if (GetVnum() >= 9830 && GetVnum() <= 10030)
		// return PENDANT_ICE;

	// else if (GetVnum() >= 10060 && GetVnum() <= 10260)
		// return PENDANT_EARTH;

	// else if (GetVnum() >= 10290 && GetVnum() <= 10490)
		// return PENDANT_DARK;	

	// else if (GetVnum() >= 10520 && GetVnum() <= 10720)
		// return PENDANT_WIND;		

	// else if (GetVnum() >= 10750 && GetVnum() <= 10950)
		// return PENDANT_LIGHTNING;	

	// return PENDANT_FIRE;				

// }
// #endif

void CItem::ModifyPoints(bool bAdd)
{
	const time_t current = get_global_time();

	if (IsBooster() && current > GetSocket(0))
	{
		return;
	}

#ifdef ENABLE_NEW_PET_SYSTEM
	if (GetType() == ITEM_PET && GetSubType() == PET_LEVELABLE)
	{
		int bonusTypes[][2] = { {APPLY_MAX_HP,4000},{APPLY_ATTBONUS_MONSTER,20},{APPLY_ATT_GRADE_BONUS,100} };
		for (BYTE i = 0; i < 3; ++i)
		{
			long bonusLevel = GetSocket(POINT_PET_BONUS_1+i);
			if (bonusLevel == 0)
				continue;
			int value = bonusLevel == 20 ? bonusTypes[i][1]:bonusLevel*(float(bonusTypes[i][1])/20.0);
			m_pOwner->ApplyPoint(bonusTypes[i][0], bAdd ? value : -value);
		}

		for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j)
		{
			BYTE skillType = GetAttributeType(j);
			if (skillType == 0 || skillType == 99)
				continue;
			auto it = petSkillBonus.find(skillType);
			if (it != petSkillBonus.end())
			{
				BYTE skillLevel = GetAttributeValue(j);
				for (BYTE i = 0; i < it->second.size(); ++i)
				{
					auto bonus = it->second[i];
					int value = skillLevel == 20 ? bonus.second : skillLevel * (float(bonus.second) / 20.0);
					m_pOwner->ApplyPoint(bonus.first, bAdd ? value : -value);
				}
			}
		}
		return;
	}
#endif

#ifdef __BUFFI_SUPPORT__
	if (IsEquipped() && GetCell()-INVENTORY_MAX_NUM >= WEAR_BUFFI_BODY && GetCell()-INVENTORY_MAX_NUM < WEAR_BUFFI_RING_0)
		return;
#endif

	int accessoryGrade;

	if (IsPendantSoul())
	{
		for (BYTE i = WEAR_PENDANT_FIRE; i <= WEAR_PENDANT_EARTH; ++i)
		{
			LPITEM item = m_pOwner->GetWear(i);
			if (!item)
				return;
			if (item->GetRefineLevel() != 25)
				return;
		}
		if (GetCell() - (180 + WEAR_PENDANT_SOUL_FIRST) != m_pOwner->GetQuestFlag("pendant.idx"))
			return;
	}

	if (!IsAccessoryForSocket())
	{
		if (m_pProto->bType == ITEM_WEAPON || m_pProto->bType == ITEM_ARMOR || m_pProto->bType == ITEM_NEW_RINGS)
		{
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			{
				DWORD dwVnum;

				if ((dwVnum = GetSocket(i)) <= 2)
					continue;

				TItemTable * p = ITEM_MANAGER::instance().GetTable(dwVnum);

				if (!p)
				{
					sys_err("cannot find table by vnum %u", dwVnum);
					continue;
				}

				if (ITEM_METIN == p->bType)
				{
					//m_pOwner->ApplyPoint(p->alValues[0], bAdd ? p->alValues[1] : -p->alValues[1]);
					for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
					{
						if (p->aApplies[i].bType == APPLY_NONE)
							continue;

						if (p->aApplies[i].bType == APPLY_SKILL)
							m_pOwner->ApplyPoint(p->aApplies[i].bType, bAdd ? p->aApplies[i].lValue : p->aApplies[i].lValue ^ 0x00800000);
						else
							m_pOwner->ApplyPoint(p->aApplies[i].bType, bAdd ? p->aApplies[i].lValue : -p->aApplies[i].lValue);
					}
				}
			}
		}

		accessoryGrade = 0;
	}
	else
	{
		accessoryGrade = MIN(GetAccessorySocketGrade(), ITEM_ACCESSORY_SOCKET_MAX_NUM);
	}
	#ifdef __SASH_SYSTEM__
	if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH) && (GetSocket(SASH_ABSORBED_SOCKET)))
	{
		TItemTable * pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(SASH_ABSORBED_SOCKET));
		if (pkItemAbsorbed)
		{
			if ((pkItemAbsorbed->bType == ITEM_ARMOR) && (pkItemAbsorbed->bSubType == ARMOR_BODY))
			{
				long lDefGrade = pkItemAbsorbed->alValues[1] + long(pkItemAbsorbed->alValues[5] * 2);
				double dValue = lDefGrade * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lDefGrade = (long) dValue;
				if ((pkItemAbsorbed->alValues[1] > 0) && (lDefGrade <= 0) || (pkItemAbsorbed->alValues[5] > 0) && (lDefGrade < 1))
					lDefGrade += 1;
				else if ((pkItemAbsorbed->alValues[1] > 0) || (pkItemAbsorbed->alValues[5] > 0))
					lDefGrade += 1;
				
				m_pOwner->ApplyPoint(APPLY_DEF_GRADE_BONUS, bAdd ? lDefGrade : -lDefGrade);
				
				long lDefMagicBonus = pkItemAbsorbed->alValues[0];
				dValue = lDefMagicBonus * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lDefMagicBonus = (long) dValue;
				if ((pkItemAbsorbed->alValues[0] > 0) && (lDefMagicBonus < 1))
					lDefMagicBonus += 1;
				else if (pkItemAbsorbed->alValues[0] > 0)
					lDefMagicBonus += 1;
				
				m_pOwner->ApplyPoint(APPLY_MAGIC_DEF_GRADE, bAdd ? lDefMagicBonus : -lDefMagicBonus);
			}

			/*
			else if (pkItemAbsorbed->bType == ITEM_WEAPON)
			{
				long lAttGrade = pkItemAbsorbed->alValues[4] + pkItemAbsorbed->alValues[5];
				if (pkItemAbsorbed->alValues[3] > pkItemAbsorbed->alValues[4])
					lAttGrade = pkItemAbsorbed->alValues[3] + pkItemAbsorbed->alValues[5];
				
				double dValue = lAttGrade * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lAttGrade = (long) dValue;
				if (((pkItemAbsorbed->alValues[3] > 0) && (lAttGrade < 1)) || ((pkItemAbsorbed->alValues[4] > 0) && (lAttGrade < 1)))
					lAttGrade += 1;
				else if ((pkItemAbsorbed->alValues[3] > 0) || (pkItemAbsorbed->alValues[4] > 0))
					lAttGrade += 1;
				
				m_pOwner->ApplyPoint(APPLY_ATT_GRADE_BONUS, bAdd ? lAttGrade : -lAttGrade);
				
				long lAttMagicGrade = pkItemAbsorbed->alValues[2] + pkItemAbsorbed->alValues[5];
				if (pkItemAbsorbed->alValues[1] > pkItemAbsorbed->alValues[2])
					lAttMagicGrade = pkItemAbsorbed->alValues[1] + pkItemAbsorbed->alValues[5];
				
				dValue = lAttMagicGrade * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lAttMagicGrade = (long) dValue;
				if (((pkItemAbsorbed->alValues[1] > 0) && (lAttMagicGrade < 1)) || ((pkItemAbsorbed->alValues[2] > 0) && (lAttMagicGrade < 1)))
					lAttMagicGrade += 1;
				else if ((pkItemAbsorbed->alValues[1] > 0) || (pkItemAbsorbed->alValues[2] > 0))
					lAttMagicGrade += 1;
				
				m_pOwner->ApplyPoint(APPLY_MAGIC_ATT_GRADE, bAdd ? lAttMagicGrade : -lAttMagicGrade);
			}*/
		}
	}
	#endif
	
/*
	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{

		if (m_pProto->aApplies[i].bType == APPLY_NONE)
			continue;
		
		long value = m_pProto->aApplies[i].lValue;

		if (m_pProto->aApplies[i].bType == APPLY_SKILL)
		{
			m_pOwner->ApplyPoint(m_pProto->aApplies[i].bType, bAdd ? value : value ^ 0x00800000);
		}
		else
		{
			if (0 != accessoryGrade)
				value += MAX(accessoryGrade, value * aiAccessorySocketEffectivePct[accessoryGrade] / 100);

			m_pOwner->ApplyPoint(m_pProto->aApplies[i].bType, bAdd ? value : -value);
		}
	}*/
	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
		#ifdef __SASH_SYSTEM__
		if ((m_pProto->aApplies[i].bType == APPLY_NONE) && (GetType() != ITEM_COSTUME) && (GetSubType() != COSTUME_SASH))
		#else
		if (m_pProto->aApplies[i].bType == APPLY_NONE)
		#endif
			continue;
		BYTE bType = m_pProto->aApplies[i].bType;
		long value = m_pProto->aApplies[i].lValue;
		#ifdef __SASH_SYSTEM__
		if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH))
		{
			TItemTable * pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(SASH_ABSORBED_SOCKET));
			if (pkItemAbsorbed)
			{
				if (pkItemAbsorbed->aApplies[i].bType == APPLY_NONE)
					continue;
				
				bType = pkItemAbsorbed->aApplies[i].bType;
				value = pkItemAbsorbed->aApplies[i].lValue;
				if (value < 0)
					continue;
				
				double dValue = value * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				value = (long) dValue;
				if ((pkItemAbsorbed->aApplies[i].lValue > 0) && (value <= 0))
					value += 1;
			}
			else
				continue;
		}
		#endif
		
		if (bType != APPLY_SKILL)
		{
#ifdef ENABLE_YOHARA_SYSTEM
			if (!ItemHasApplyRandom())
#endif
			{
				if (accessoryGrade != 0)
					value += MAX(accessoryGrade, value * aiAccessorySocketEffectivePct[accessoryGrade] / 100);
			}
			
			m_pOwner->ApplyPoint(bType, bAdd ? value : -value);
		}
		else
			m_pOwner->ApplyPoint(bType, bAdd ? value : value ^ 0x00800000);
	}
	
#ifdef __AURA_SYSTEM__
	float fAuraDrainPer = 0.0f;
	if (m_pProto->bType == ITEM_COSTUME && m_pProto->bSubType == COSTUME_AURA)
	{
		const long c_lLevelSocket = GetSocket(ITEM_SOCKET_AURA_CURRENT_LEVEL);
		const long c_lDrainSocket = GetSocket(ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM);
		const long c_lBoostSocket = GetSocket(ITEM_SOCKET_AURA_BOOST);

		BYTE bCurLevel = (c_lLevelSocket / 100000) - 1000;
		BYTE bBoostIndex = c_lBoostSocket / 100000000;

		TItemTable* pBoosterProto = ITEM_MANAGER::instance().GetTable(ITEM_AURA_BOOST_ITEM_VNUM_BASE + bBoostIndex);
		fAuraDrainPer = (1.0f * bCurLevel / 10.0f) / 100.0f;
		if (pBoosterProto)
			fAuraDrainPer += 1.0f * pBoosterProto->alValues[ITEM_AURA_BOOST_PERCENT_VALUE] / 100.0f;

		TItemTable* p = NULL;
		if (c_lDrainSocket != 0)
			p = ITEM_MANAGER::instance().GetTable(c_lDrainSocket);

		if (p != NULL && (ITEM_ARMOR == p->bType && (ARMOR_SHIELD == p->bSubType || ARMOR_WRIST == p->bSubType || ARMOR_NECK == p->bSubType || ARMOR_EAR == p->bSubType)))
		{
			for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
			{
				if (p->aApplies[i].bType == APPLY_NONE || /*p->aApplies[i].bType == APPLY_MOUNT || */p->aApplies[i].bType == APPLY_MOV_SPEED || p->aApplies[i].lValue <= 0)
					continue;

				float fValue = p->aApplies[i].lValue * fAuraDrainPer;
				int iValue = static_cast<int>((fValue < 1.0f) ? ceilf(fValue) : truncf(fValue));
				if (p->aApplies[i].bType == APPLY_SKILL)
					m_pOwner->ApplyPoint(p->aApplies[i].bType, bAdd ? iValue : iValue ^ 0x00800000);
				else
					m_pOwner->ApplyPoint(p->aApplies[i].bType, bAdd ? iValue : -iValue);
			}
		}
	}
#endif
	

	// ÃÊ½Â´ÞÀÇ ¹ÝÁö, ÇÒ·ÎÀ© »çÅÁ, Çàº¹ÀÇ ¹ÝÁö, ¿µ¿øÇÑ »ç¶ûÀÇ Ææ´øÆ®ÀÇ °æ¿ì
	// ±âÁ¸ÀÇ ÇÏµå ÄÚµùÀ¸·Î °­Á¦·Î ¼Ó¼ºÀ» ºÎ¿©ÇßÁö¸¸,
	// ±× ºÎºÐÀ» Á¦°ÅÇÏ°í special item group Å×ÀÌºí¿¡¼­ ¼Ó¼ºÀ» ºÎ¿©ÇÏµµ·Ï º¯°æÇÏ¿´´Ù.
	// ÇÏÁö¸¸ ÇÏµå ÄÚµùµÇ¾îÀÖÀ» ¶§ »ý¼ºµÈ ¾ÆÀÌÅÛÀÌ ³²¾ÆÀÖÀ» ¼öµµ ÀÖ¾î¼­ Æ¯¼öÃ³¸® ÇØ³õ´Â´Ù.
	// ÀÌ ¾ÆÀÌÅÛµéÀÇ °æ¿ì, ¹Ø¿¡ ITEM_UNIQUEÀÏ ¶§ÀÇ Ã³¸®·Î ¼Ó¼ºÀÌ ºÎ¿©µÇ±â ¶§¹®¿¡,
	// ¾ÆÀÌÅÛ¿¡ ¹ÚÇôÀÖ´Â attribute´Â Àû¿ëÇÏÁö ¾Ê°í ³Ñ¾î°£´Ù.
	if (true == CItemVnumHelper::IsRamadanMoonRing(GetVnum()) || true == CItemVnumHelper::IsHalloweenCandy(GetVnum())
		|| true == CItemVnumHelper::IsHappinessRing(GetVnum()) || true == CItemVnumHelper::IsLovePendant(GetVnum()))
	{
		// Do not anything.
	}
	/*
	else
	{
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			if (GetAttributeType(i))
			{
				const TPlayerItemAttribute& ia = GetAttribute(i);
				long sValue = ia.sValue;

				
				if (ia.bType == APPLY_SKILL)
					m_pOwner->ApplyPoint(ia.bType, bAdd ? sValue : sValue ^ 0x00800000);
				else
					m_pOwner->ApplyPoint(ia.bType, bAdd ? sValue : -sValue);
			}
		}



		
	}*/


	else
	{
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			if (GetAttributeType(i))
			{
				// const TPlayerItemAttribute& ia = GetAttribute(i);
				// long sValue = ia.sValue;
				// #ifdef __SASH_SYSTEM__
				// if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH))
				// {
					// double dValue = sValue * GetSocket(SASH_ABSORPTION_SOCKET);
					// dValue = (double)dValue / 100;
					// dValue = (double)dValue + .5;
					// sValue = (long) dValue;
					// if ((ia.sValue > 0) && (sValue <= 0))
						// sValue += 1;
				// }
				// #endif
				
				// if (ia.bType == APPLY_SKILL)
					// m_pOwner->ApplyPoint(ia.bType, bAdd ? sValue : sValue ^ 0x00800000);
				// else
					// m_pOwner->ApplyPoint(ia.bType, bAdd ? sValue : -sValue);
				
				const TPlayerItemAttribute& ia = GetAttribute(i);
				long lValue = ia.sValue;
#ifdef __AURA_SYSTEM__
				if (ia.bType == APPLY_SKILL)
				{
					if (m_pProto->bType == ITEM_COSTUME && m_pProto->bSubType == COSTUME_AURA)
					{
						if (ia.sValue <= 0)
							continue;

						float fValue = ia.sValue * fAuraDrainPer;
						lValue = static_cast<int>((fValue < 1.0f) ? ceilf(fValue) : truncf(fValue));
					}
				}
				else
				{
					if (m_pProto->bType == ITEM_COSTUME && m_pProto->bSubType == COSTUME_AURA)
					{
						if (ia.sValue <= 0)
							continue;

						float fValue = ia.sValue * fAuraDrainPer;
						lValue = static_cast<int>((fValue < 1.0f) ? ceilf(fValue) : truncf(fValue));
					}
				}
#endif
#ifdef __SASH_SYSTEM__
				if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH))
				{
					double dValue = lValue * GetSocket(SASH_ABSORPTION_SOCKET);
					dValue = (double)dValue / 100;
					dValue = (double)dValue + .5;
					lValue = (long) dValue;
					if ((ia.sValue > 0) && (lValue <= 0))
						lValue += 1;
				}
#endif
				if (ia.bType == APPLY_SKILL)
					m_pOwner->ApplyPoint(ia.bType, bAdd ? lValue : lValue ^ 0x00800000);
				else
					m_pOwner->ApplyPoint(ia.bType, bAdd ? lValue : -lValue);

				
			}
		}
	}

#ifdef ENABLE_YOHARA_SYSTEM
	for (int s = 0; s < APPLY_RANDOM_SLOT_MAX_NUM; ++s)
	{
		if (GetApplyRandomType(s))
		{
			const TPlayerItemApplyRandom& ia = GetApplyRandom(s);
			long randomVal = ia.sValue;

			if (ia.bType == APPLY_SKILL)
			{
				m_pOwner->ApplyPoint(ia.bType, bAdd ? randomVal : randomVal ^ 0x00800000);
			}
			else
			{
				if (0 != accessoryGrade)
					randomVal += MAX(accessoryGrade, randomVal * aiAccessorySocketEffectivePct[accessoryGrade] / 100);

				m_pOwner->ApplyPoint(ia.bType, bAdd ? randomVal : -randomVal);
			}
		}
	}
#endif

	switch (m_pProto->bType)
	{
		case ITEM_PICK:
		case ITEM_ROD:
			{
				if (bAdd)
				{
					if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
						m_pOwner->SetPart(PART_WEAPON, GetVnum());
				}
				else
				{
					if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
						m_pOwner->SetPart(PART_WEAPON, 0);
				}
			}
			break;

		case ITEM_WEAPON:
			{

				if (m_pProto->bSubType == WEAPON_ARROW || m_pProto->bSubType == WEAPON_QUIVER)
				{
					if (bAdd)
					{
						if (m_wCell == INVENTORY_MAX_NUM + WEAR_ARROW)
						{
							m_pOwner->SetPart(PART_ARROW_TYPE, m_pProto->bSubType);
							const CItem* pWeapon = m_pOwner->GetWear(WEAR_WEAPON);
							if (pWeapon != NULL && pWeapon->GetSubType() == WEAPON_BOW)
							{
								const CItem* pCostumeWeapon = m_pOwner->GetWear(WEAR_COSTUME_WEAPON);
								if (pCostumeWeapon != NULL)
								{
									m_pOwner->SetPart(PART_WEAPON, pCostumeWeapon->GetVnum());
								}
								else
								{
									m_pOwner->SetPart(PART_WEAPON, pWeapon->GetVnum());
								}
							}
						}
					}
					else
					{
						if (m_wCell == INVENTORY_MAX_NUM + WEAR_ARROW)
							m_pOwner->SetPart(PART_ARROW_TYPE, m_pOwner->GetOriginalPart(PART_ARROW_TYPE));
					}
					
					break;
				}
				const CItem* pArrow = m_pOwner->GetWear(WEAR_ARROW);

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
				if (0 != m_pOwner->GetWear(WEAR_COSTUME_WEAPON))
					break;
#endif

				if (bAdd)
				{
					if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
						if (pArrow != NULL)
							m_pOwner->SetPart(PART_ARROW_TYPE, pArrow->GetSubType());

					if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
						m_pOwner->SetPart(PART_WEAPON, GetVnum());
				}
				else
				{
					if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
						m_pOwner->SetPart(PART_WEAPON, 0);
				}
			}
			break;

		case ITEM_ARMOR:
			{
				// ÄÚ½ºÃõ body¸¦ ÀÔ°íÀÖ´Ù¸é armor´Â ¹þ´ø ÀÔ´ø »ó°ü ¾øÀÌ ºñÁÖ¾ó¿¡ ¿µÇâÀ» ÁÖ¸é ¾È µÊ.
				if (0 != m_pOwner->GetWear(WEAR_COSTUME_BODY))
					break;

#ifdef ENABLE_YOHARA_SYSTEM
#ifdef ITEM_TALISMAN_EQUIPMENT
				if (GetSubType() == ARMOR_BODY || GetSubType() == ARMOR_HEAD || GetSubType() == ARMOR_FOOTS || GetSubType() == ARMOR_SHIELD || GetSubType() == ARMOR_GLOVE)
#else
				if (GetSubType() == ARMOR_BODY || GetSubType() == ARMOR_HEAD || GetSubType() == ARMOR_FOOTS || GetSubType() == ARMOR_SHIELD || GetSubType() == ARMOR_GLOVE)
#endif
#else
#ifdef ITEM_TALISMAN_EQUIPMENT
				if (GetSubType() == ARMOR_BODY || GetSubType() == ARMOR_HEAD || GetSubType() == ARMOR_FOOTS || GetSubType() == ARMOR_SHIELD)
#else
				if (GetSubType() == ARMOR_BODY || GetSubType() == ARMOR_HEAD || GetSubType() == ARMOR_FOOTS || GetSubType() == ARMOR_SHIELD)
#endif
#endif
				{
					if (bAdd)
					{
						if (GetProto()->bSubType == ARMOR_BODY)
							m_pOwner->SetPart(PART_MAIN, GetVnum());
					}
					else
					{
						if (GetProto()->bSubType == ARMOR_BODY)
							m_pOwner->SetPart(PART_MAIN, m_pOwner->GetOriginalPart(PART_MAIN));
					}
				}
			}
			break;

		// ÄÚ½ºÃõ ¾ÆÀÌÅÛ ÀÔ¾úÀ» ¶§ Ä³¸¯ÅÍ parts Á¤º¸ ¼¼ÆÃ. ±âÁ¸ ½ºÅ¸ÀÏ´ë·Î Ãß°¡ÇÔ..
		case ITEM_COSTUME:
			{
				DWORD toSetValue = this->GetVnum();
				EParts toSetPart = PART_MAX_NUM;

				// °©¿Ê ÄÚ½ºÃõ
				if (GetSubType() == COSTUME_BODY)
				{
					toSetPart = PART_MAIN;

					if (false == bAdd)
					{
						// ÄÚ½ºÃõ °©¿ÊÀ» ¹þ¾úÀ» ¶§ ¿ø·¡ °©¿ÊÀ» ÀÔ°í ÀÖ¾ú´Ù¸é ±× °©¿ÊÀ¸·Î look ¼¼ÆÃ, ÀÔÁö ¾Ê¾Ò´Ù¸é default look
						const CItem* pArmor = m_pOwner->GetWear(WEAR_BODY);
						toSetValue = (NULL != pArmor) ? pArmor->GetVnum() : m_pOwner->GetOriginalPart(PART_MAIN);
					}

				}

				// Çì¾î ÄÚ½ºÃõ
				else if (GetSubType() == COSTUME_HAIR)
				{
					toSetPart = PART_HAIR;

					// ÄÚ½ºÃõ Çì¾î´Â shape°ªÀ» item protoÀÇ value3¿¡ ¼¼ÆÃÇÏµµ·Ï ÇÔ. Æ¯º°ÇÑ ÀÌÀ¯´Â ¾ø°í ±âÁ¸ °©¿Ê(ARMOR_BODY)ÀÇ shape°ªÀÌ ÇÁ·ÎÅäÀÇ value3¿¡ ÀÖ¾î¼­ Çì¾îµµ °°ÀÌ value3À¸·Î ÇÔ.
					// [NOTE] °©¿ÊÀº ¾ÆÀÌÅÛ vnumÀ» º¸³»°í Çì¾î´Â shape(value3)°ªÀ» º¸³»´Â ÀÌÀ¯´Â.. ±âÁ¸ ½Ã½ºÅÛÀÌ ±×·¸°Ô µÇ¾îÀÖÀ½...
					toSetValue = (true == bAdd) ? this->GetValue(3) : 0;
				}
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
				else if (GetSubType() == COSTUME_WEAPON)
				{
					toSetPart = PART_WEAPON;
					if (false == bAdd)
					{
						const CItem* pWeapon = m_pOwner->GetWear(WEAR_WEAPON);
						toSetValue = (NULL != pWeapon) ? pWeapon->GetVnum() : m_pOwner->GetOriginalPart(PART_WEAPON);
					}
				}
#endif
#ifdef __AURA_SYSTEM__
				else if (GetSubType() == COSTUME_AURA)
				{
					if (m_wCell == INVENTORY_MAX_NUM + WEAR_COSTUME_AURA)
						m_pOwner->SetPart(PART_AURA, bAdd ? GetVnum() : m_pOwner->GetOriginalPart(PART_AURA));
				}
#endif

#ifdef __SASH_SYSTEM__
				else if (GetSubType() == COSTUME_SASH || GetSubType() == COSTUME_NEW_SASH)
				{
					if (GetSubType() == COSTUME_SASH)
					{
						if (m_pOwner->GetWear(WEAR_COSTUME_NEW_SASH))
							break;
					}

					toSetValue -= 85000;
					// if (GetSocket(SASH_ABSORPTION_SOCKET) >= SASH_EFFECT_FROM_ABS)
						// toSetValue += 1000;
					
					if (GetSubType() == COSTUME_NEW_SASH)
					{
						auto itemSash = m_pOwner->GetWear(WEAR_COSTUME_SASH);
						if (itemSash && !bAdd)
						{
							toSetValue = itemSash->GetVnum() - 85000;

							// if (itemSash->GetSocket(SASH_ABSORPTION_SOCKET) >= SASH_EFFECT_FROM_ABS) 
								// toSetValue += 1000;
						}
					}
					else
						toSetValue = (bAdd == true) ? toSetValue : 0;

					toSetPart = PART_SASH;
				}
#endif


				if (PART_MAX_NUM != toSetPart)
				{
					m_pOwner->SetPart((BYTE)toSetPart, toSetValue);
					m_pOwner->UpdatePacket();
				}
			}
			break;
		case ITEM_UNIQUE:
			{
				if (0 != GetSIGVnum())
				{
					const CSpecialItemGroup* pItemGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(GetSIGVnum());
					if (NULL == pItemGroup)
						break;
					DWORD dwAttrVnum = pItemGroup->GetAttrVnum(GetVnum());
					const CSpecialAttrGroup* pAttrGroup = ITEM_MANAGER::instance().GetSpecialAttrGroup(dwAttrVnum);
					if (NULL == pAttrGroup)
						break;
					for (itertype (pAttrGroup->m_vecAttrs) it = pAttrGroup->m_vecAttrs.begin(); it != pAttrGroup->m_vecAttrs.end(); it++)
					{
						m_pOwner->ApplyPoint(it->apply_type, bAdd ? it->apply_value : -it->apply_value);
					}
				}
			}
			break;
	}
}

bool CItem::IsEquipable() const
{
	switch (this->GetType())
	{
	case ITEM_COSTUME:
	case ITEM_ARMOR:
	case ITEM_WEAPON:
	case ITEM_ROD:
	case ITEM_PICK:
	case ITEM_UNIQUE:
	case ITEM_DS:
	case ITEM_SPECIAL_DS:
	case ITEM_RING:
	case ITEM_BELT:
#ifdef __ITEM_SHINING__
	case ITEM_SHINING:
#endif
	case ITEM_NEW_RINGS:
#ifdef ENABLE_NEW_PET_SYSTEM
	case ITEM_PET:
#endif
	case ITEM_NEWRING:
	case ITEM_RING_SPECIAL:
	case ITEM_BOOSTER:
		return true;
	}

	return false;
}

#define ENABLE_IMMUNE_FIX
// return false on error state
bool CItem::EquipTo(LPCHARACTER ch, WORD wWearCell)
{
	if (!ch)
	{
		sys_err("EquipTo: nil character");
		return false;
	}

	// ¿ëÈ¥¼® ½½·Ô index´Â WEAR_MAX_NUM º¸´Ù Å­.
	if (IsDragonSoul())
	{
		if (wWearCell < WEAR_MAX_NUM || wWearCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
		{
			sys_err("EquipTo: invalid dragon soul cell (this: #%d %s wearflag: %d cell: %u)", GetOriginalVnum(), GetName(), GetSubType(), wWearCell - WEAR_MAX_NUM);
			return false;
		}
	}
	else
	{
		if (wWearCell >= WEAR_MAX_NUM)
		{
			sys_err("EquipTo: invalid wear cell (this: #%d %s wearflag: %d cell: %u)", GetOriginalVnum(), GetName(), GetWearFlag(), wWearCell);
			return false;
		}
	}

	if (ch->GetWear(wWearCell))
	{
		sys_err("EquipTo: item already exist (this: #%d %s cell: %u %s)", GetOriginalVnum(), GetName(), wWearCell, ch->GetWear(wWearCell)->GetName());
		return false;
	}

	if (GetOwner())
		RemoveFromCharacter();

	ch->SetWear(wWearCell, this); // ¿©±â¼­ ÆÐÅ¶ ³ª°¨

	m_pOwner = ch;
	m_bEquipped = true;
	m_wCell = INVENTORY_MAX_NUM + wWearCell;

#ifndef ENABLE_IMMUNE_FIX
	DWORD dwImmuneFlag = 0;

	for (int i = 0; i < WEAR_MAX_NUM; ++i)
	{
		if (m_pOwner->GetWear(i))
		{
			// m_pOwner->ChatPacket(CHAT_TYPE_INFO, "unequip immuneflag(%u)", m_pOwner->GetWear(i)->m_pProto->dwImmuneFlag); // always 0
			SET_BIT(dwImmuneFlag, m_pOwner->GetWear(i)->m_pProto->dwImmuneFlag);
		}
	}

	m_pOwner->SetImmuneFlag(dwImmuneFlag);
#endif

#ifdef __BUFFI_SUPPORT__
	if (wWearCell >= WEAR_BUFFI_BODY && wWearCell < WEAR_BUFFI_RING_0 && BUFFI_MANAGER::Instance().SummonCount(m_pOwner))
	{
		BUFFI* buffi = BUFFI_MANAGER::Instance().GetBuffi(m_pOwner->GetPlayerID(), 0);
		if (buffi)
			buffi->CheckSupportWear(false);
	}
#endif

	if (IsDragonSoul())
	{
		DSManager::instance().ActivateDragonSoul(this);
	}
	else
	{
		ModifyPoints(true);
		StartUniqueExpireEvent();
		if (-1 != GetProto()->cLimitTimerBasedOnWearIndex)
			StartTimerBasedOnWearExpireEvent();

		// ACCESSORY_REFINE
		StartAccessorySocketExpireEvent();
		// END_OF_ACCESSORY_REFINE
#ifdef __AURA_SYSTEM__
		StartAuraBoosterSocketExpireEvent();
#endif
	}
#ifdef ENABLE_NEW_PET_SYSTEM
	if (GetOwner())
	{
		if (IsCostumePetItem())
		{
			CPetSystem* petSystem = GetOwner()->GetPetSystem();
			if (petSystem)
				petSystem->HandlePetCostumeItem();
		}
		if (IsNewPetItem())
		{
			CPetSystem* petSystem = GetOwner()->GetPetSystem();
			if (petSystem)
				petSystem->HandleNewPetItem();
		}
	}
#endif
	ch->BuffOnAttr_AddBuffsFromItem(this);

#ifdef __NEW_SET_BONUS__
	if (m_pOwner->IsNewSetNeedRefresh(GetVnum()))
		m_pOwner->ComputePoints();
	else
	{
		m_pOwner->ComputeBattlePoints();
		m_pOwner->UpdatePacket();
	}
#else
	m_pOwner->ComputeBattlePoints();
	m_pOwner->UpdatePacket();
#endif

	Save();

	return (true);
}

bool CItem::Unequip()
{
	if (!m_pOwner || GetCell() < INVENTORY_MAX_NUM)
	{
		// ITEM_OWNER_INVALID_PTR_BUG
		sys_err("%s %u m_pOwner %p, GetCell %d",
				GetName(), GetID(), get_pointer(m_pOwner), GetCell());
		// END_OF_ITEM_OWNER_INVALID_PTR_BUG
		return false;
	}

	if (this != m_pOwner->GetWear(GetCell() - INVENTORY_MAX_NUM))
	{
		sys_err("m_pOwner->GetWear() != this");
		return false;
	}

	//½Å±Ô ¸» ¾ÆÀÌÅÛ Á¦°Å½Ã Ã³¸®
	if (IsRideItem())
		ClearMountAttributeAndAffect();

#ifdef ENABLE_NEW_PET_SYSTEM
	if (GetOwner())
	{
		if (IsCostumePetItem() || IsNewPetItem())
		{
			CPetSystem* petSystem = GetOwner()->GetPetSystem();
			if (petSystem)
				petSystem->DeletePet(GetID());
		}
	}
#endif

	if (IsDragonSoul())
	{
		DSManager::instance().DeactivateDragonSoul(this);
	}
	else
	{
		ModifyPoints(false);
	}

	StopUniqueExpireEvent();

	if (-1 != GetProto()->cLimitTimerBasedOnWearIndex)
		StopTimerBasedOnWearExpireEvent();

	// ACCESSORY_REFINE
	StopAccessorySocketExpireEvent();
	// END_OF_ACCESSORY_REFINE
#ifdef __AURA_SYSTEM__
	StopAuraBoosterSocketExpireEvent();
#endif

	m_pOwner->BuffOnAttr_RemoveBuffsFromItem(this);

	m_pOwner->SetWear(GetCell() - INVENTORY_MAX_NUM, NULL);

#ifdef __BUFFI_SUPPORT__
	if (GetCell() - INVENTORY_MAX_NUM >= WEAR_BUFFI_BODY && GetCell() - INVENTORY_MAX_NUM < WEAR_BUFFI_RING_0 && BUFFI_MANAGER::Instance().SummonCount(m_pOwner))
	{
		BUFFI* buffi = BUFFI_MANAGER::Instance().GetBuffi(m_pOwner->GetPlayerID(), 0);
		if (buffi)
			buffi->CheckSupportWear(false);
	}
#endif

#ifndef ENABLE_IMMUNE_FIX
	DWORD dwImmuneFlag = 0;

	for (int i = 0; i < WEAR_MAX_NUM; ++i)
	{
		if (m_pOwner->GetWear(i))
		{
			// m_pOwner->ChatPacket(CHAT_TYPE_INFO, "unequip immuneflag(%u)", m_pOwner->GetWear(i)->m_pProto->dwImmuneFlag); // always 0
			SET_BIT(dwImmuneFlag, m_pOwner->GetWear(i)->m_pProto->dwImmuneFlag);
		}
	}

	m_pOwner->SetImmuneFlag(dwImmuneFlag);
#endif

	quest::CQuestManager& q = quest::CQuestManager::instance();
	quest::PC* pPC = q.GetPC(m_pOwner->GetPlayerID());

#ifdef __NEW_SET_BONUS__
	if (m_pOwner->IsNewSetNeedRefresh(GetVnum()) || (pPC))
		m_pOwner->ComputePoints();
	else
	{
		m_pOwner->ComputeBattlePoints();
		m_pOwner->UpdatePacket();
	}
#else
	m_pOwner->ComputeBattlePoints();
	m_pOwner->UpdatePacket();
#endif

	m_pOwner = NULL;
	m_wCell = 0;
	m_bEquipped	= false;

	return true;
}

long CItem::GetValue(DWORD idx)
{
	assert(idx < ITEM_VALUES_MAX_NUM);
	return GetProto()->alValues[idx];
}

void CItem::SetExchanging(bool bOn)
{
	m_bExchanging = bOn;
}

void CItem::Save()
{
	if (m_bSkipSave)
		return;

#ifdef __BL_BATTLE_ROYALE__
	if (m_NeverSave)
		return;
#endif

	ITEM_MANAGER::instance().DelayedSave(this);
}

bool CItem::CreateSocket(BYTE bSlot, BYTE bGold)
{
	assert(bSlot < ITEM_SOCKET_MAX_NUM);

	if (m_alSockets[bSlot] != 0)
	{
		sys_err("Item::CreateSocket : socket already exist %s %d", GetName(), bSlot);
		return false;
	}

	if (bGold)
		m_alSockets[bSlot] = 2;
	else
		m_alSockets[bSlot] = 1;

	UpdatePacket();

	Save();
	return true;
}

void CItem::SetSockets(const long * c_al)
{
	std::memcpy(m_alSockets, c_al, sizeof(m_alSockets));
	Save();
}

void CItem::SetSocket(int i, long v, bool bLog)
{
	assert(i < ITEM_SOCKET_MAX_NUM);

// #ifdef USE_CRYSTAL_SYSTEM
	if (m_alSockets[i] == v)
	{
		return;
	}
// #endif

	m_alSockets[i] = v;

	UpdatePacket();
	Save();

	if (bLog)
	{
#ifdef ENABLE_NEWSTUFF
		if (g_iDbLogLevel >= LOG_LEVEL_MAX)
		{
#endif
		LogManager::instance().ItemLog(i, v, 0, GetID(), "SET_SOCKET", "", "", GetOriginalVnum());
#ifdef ENABLE_NEWSTUFF
		}
#endif
	}
}

#ifdef ENABLE_YOHARA_SYSTEM
// Random Attr
bool CItem::ItemHasApplyRandom()
{
	if (!m_pProto)
		return false;

	for (int i = 0; i < APPLY_RANDOM_SLOT_MAX_NUM; ++i)
	{
		if (m_pProto->aApplies[i].bType == APPLY_RANDOM)
			return true;
	}

	return false;
}

bool CItem::GetApplyRandomData(DWORD vnum)	//need to be reworked, to use APPLY_RANDOM instead of fixed vnums!
{
	if (!vnum)
		return 0;

	int variable = 0;

	if (vnum >= 81000 && vnum <= 81559)
		variable = 1;

	else if ((vnum >= 14580 && vnum <= 14619) || (vnum >= 16580 && vnum <= 16619) || (vnum >= 17570 && vnum <= 17609))
		variable = 2;

	else if (vnum >= 23000 && vnum <= 23039)
		variable = 3;

	else if (vnum >= 360 && vnum <= 375)	//Schlangenwaffen
		variable = 4;
	else if ((vnum >= 21310 && vnum <= 21325) || (vnum >= 21330 && vnum <= 21345))	//SchlangenR?sis
		variable = 4;

	return variable;
}

WORD CItem::GetSungmaUniqueRandomSocket()
{
	std::vector<WORD> vSungmaSocketAttr =
	{
		APPLY_SUNGMA_STR, // STR
		APPLY_SUNGMA_HP, // VIT
		APPLY_SUNGMA_MOVE, // RES
		APPLY_SUNGMA_IMMUNE, // INT
	};

	for (int i = 3; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		auto lApplyType = GetSocket(i);

		/*
		* Because there are no more sockets to store the information, I was forced to think
		* of a solution to find out which socket is "boosted" and which is not.
		* So I decided that the "boost" sockets will be the applyType x 10
		*/
		const auto wBoostApplyType = lApplyType / 10;
		if (wBoostApplyType >= APPLY_SUNGMA_STR && wBoostApplyType <= APPLY_SUNGMA_IMMUNE)
			lApplyType = wBoostApplyType;

		vSungmaSocketAttr.erase(std::remove(vSungmaSocketAttr.begin(), vSungmaSocketAttr.end(), lApplyType), vSungmaSocketAttr.end());
	}

	if (vSungmaSocketAttr.empty())
		return 0;

	const int randomSocket = number(0, (vSungmaSocketAttr.size() - 1));
	return vSungmaSocketAttr[randomSocket];
}
#endif

long long CItem::GetGold()
{
	if (IS_SET(GetFlag(), ITEM_FLAG_COUNT_PER_1GOLD))
	{
		if (GetProto()->dwGold == 0)
			return GetCount();
		else
			return GetCount() / GetProto()->dwGold;
	}
	else
		return GetProto()->dwGold;
}

long long CItem::GetShopBuyPrice()
{
	return GetProto()->dwShopBuyPrice;
}

bool CItem::IsOwnership(LPCHARACTER ch)
{
	if (!m_pkOwnershipEvent)
		return true;

	return m_dwOwnershipPID == ch->GetPlayerID() ? true : false;
}

EVENTFUNC(ownership_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "ownership_event> <Factor> Null pointer" );
		return 0;
	}

	LPITEM pkItem = info->item;

	pkItem->SetOwnershipEvent(NULL);

	TPacketGCItemOwnership p;

	p.bHeader	= HEADER_GC_ITEM_OWNERSHIP;
	p.dwVID	= pkItem->GetVID();
	p.szName[0]	= '\0';

	pkItem->PacketAround(&p, sizeof(p));
	return 0;
}

void CItem::SetOwnershipEvent(LPEVENT pkEvent)
{
	m_pkOwnershipEvent = pkEvent;
}

void CItem::SetOwnership(LPCHARACTER ch, int iSec)
{
#if defined(__BL_BATTLE_ROYALE__)
	if (ch && ch->IsBattleRoyale())
		ch = NULL;
#endif

	if (!ch)
	{
		if (m_pkOwnershipEvent)
		{
			event_cancel(&m_pkOwnershipEvent);
			m_dwOwnershipPID = 0;

			TPacketGCItemOwnership p;

			p.bHeader	= HEADER_GC_ITEM_OWNERSHIP;
			p.dwVID	= m_dwVID;
			p.szName[0]	= '\0';

			PacketAround(&p, sizeof(p));
		}
		return;
	}

	if (m_pkOwnershipEvent)
		return;

	if (iSec <= 10)
		iSec = 30;

	m_dwOwnershipPID = ch->GetPlayerID();

	item_event_info* info = AllocEventInfo<item_event_info>();
	strlcpy(info->szOwnerName, ch->GetName(), sizeof(info->szOwnerName));
	info->item = this;

	SetOwnershipEvent(event_create(ownership_event, info, PASSES_PER_SEC(iSec)));

	TPacketGCItemOwnership p;

	p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
	p.dwVID = m_dwVID;
	strlcpy(p.szName, ch->GetName(), sizeof(p.szName));

	PacketAround(&p, sizeof(p));
}

int CItem::GetSocketCount()
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		if (GetSocket(i) == 0)
			return i;
	}
	return ITEM_SOCKET_MAX_NUM;
}

bool CItem::AddSocket()
{
	int count = GetSocketCount();
	if (count == ITEM_SOCKET_MAX_NUM)
		return false;
	m_alSockets[count] = 1;
	return true;
}

void CItem::AlterToSocketItem(int iSocketCount)
{
	if (iSocketCount >= ITEM_SOCKET_MAX_NUM)
	{
		sys_log(0, "Invalid Socket Count %d, set to maximum", ITEM_SOCKET_MAX_NUM);
		iSocketCount = ITEM_SOCKET_MAX_NUM;
	}

	for (int i = 0; i < iSocketCount; ++i)
		SetSocket(i, 1);
}

void CItem::AlterToMagicItem()
{
	int idx = GetAttributeSetIndex();

	if (idx < 0)
		return;

	//      Appeariance Second Third
	// Weapon 50        20     5
	// Armor  30        10     2
	// Acc    20        10     1

	int iSecondPct;
	int iThirdPct;

	switch (GetType())
	{
		case ITEM_WEAPON:
			iSecondPct = 20;
			iThirdPct = 5;
			break;

		case ITEM_ARMOR:
		case ITEM_COSTUME:
			if (GetSubType() == ARMOR_BODY)
			{
				iSecondPct = 10;
				iThirdPct = 2;
			}
#ifdef __AURA_SYSTEM__
			else if (GetType() == ITEM_COSTUME && GetSubType() == COSTUME_AURA)
			{
				iSecondPct = 0;
				iThirdPct = 0;
			}
#endif
			else
			{
				iSecondPct = 10;
				iThirdPct = 1;
			}
			break;

		default:
			return;
	}

	if(GetType() == ITEM_COSTUME)
	{
		if(GetSubType() == COSTUME_HAIR)
		{
			PutAttributeCostumeHair(aiItemMagicAttributePercentHigh);
		}else{
			PutAttributeCostume(aiItemMagicAttributePercentHigh);
		}
	}else{
		PutAttribute(aiItemMagicAttributePercentHigh);
	}	

	if (number(1, 100) <= iSecondPct){
		if(GetType() == ITEM_COSTUME)
		{
			if(GetSubType() == COSTUME_HAIR)
			{
				PutAttributeCostumeHair(aiItemMagicAttributePercentLow);
			}else{
				PutAttributeCostume(aiItemMagicAttributePercentLow);
			}
		}else{
			PutAttribute(aiItemMagicAttributePercentLow);
		}
	}

	if (number(1, 100) <= iThirdPct){
		if(GetType() == ITEM_COSTUME)
		{
			if(GetSubType() == COSTUME_HAIR)
			{
				PutAttributeCostumeHair(aiItemMagicAttributePercentLow);
			}else{
				PutAttributeCostume(aiItemMagicAttributePercentLow);
			}
		}else{
			PutAttribute(aiItemMagicAttributePercentLow);
		}
	}
}

DWORD CItem::GetRefineFromVnum()
{
	return ITEM_MANAGER::instance().GetRefineFromVnum(GetVnum());
}

int CItem::GetRefineLevel()
{
	const char* name = GetBaseName();
	char* p = const_cast<char*>(strrchr(name, '+'));

	if (!p)
		return 0;

	int	rtn = 0;
	str_to_number(rtn, p+1);

	const char* locale_name = GetName();
	p = const_cast<char*>(strrchr(locale_name, '+'));

	if (p)
	{
		int	locale_rtn = 0;
		str_to_number(locale_rtn, p+1);
		if (locale_rtn != rtn)
		{
			sys_err("refine_level_based_on_NAME(%d) is not equal to refine_level_based_on_LOCALE_NAME(%d).", rtn, locale_rtn);
		}
	}

	return rtn;
}

bool CItem::IsPolymorphItem()
{
	return GetType() == ITEM_POLYMORPH;
}

EVENTFUNC(unique_expire_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "unique_expire_event> <Factor> Null pointer" );
		return 0;
	}

	LPITEM pkItem = info->item;

	if (pkItem->GetValue(2) == 0)
	{
		if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) <= 1)
		{
			sys_log(0, "UNIQUE_ITEM: expire %s %u", pkItem->GetName(), pkItem->GetID());
			pkItem->SetUniqueExpireEvent(NULL);
			ITEM_MANAGER::instance().RemoveItem(pkItem, "UNIQUE_EXPIRE");
			return 0;
		}
		else
		{
			pkItem->SetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME, pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - 1);
			return PASSES_PER_SEC(60);
		}
	}
	else
	{
		time_t cur = get_global_time();

		if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) <= cur)
		{
			pkItem->SetUniqueExpireEvent(NULL);
			ITEM_MANAGER::instance().RemoveItem(pkItem, "UNIQUE_EXPIRE");
			return 0;
		}
		else
		{
			// °ÔÀÓ ³»¿¡ ½Ã°£Á¦ ¾ÆÀÌÅÛµéÀÌ ºü¸´ºü¸´ÇÏ°Ô »ç¶óÁöÁö ¾Ê´Â ¹ö±×°¡ ÀÖ¾î
			// ¼öÁ¤
			// by rtsummit
			if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - cur < 600)
				return PASSES_PER_SEC(pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - cur);
			else
				return PASSES_PER_SEC(600);
		}
	}
}

// ½Ã°£ ÈÄºÒÁ¦
// timer¸¦ ½ÃÀÛÇÒ ¶§¿¡ ½Ã°£ Â÷°¨ÇÏ´Â °ÍÀÌ ¾Æ´Ï¶ó,
// timer°¡ ¹ßÈ­ÇÒ ¶§¿¡ timer°¡ µ¿ÀÛÇÑ ½Ã°£ ¸¸Å­ ½Ã°£ Â÷°¨À» ÇÑ´Ù.
EVENTFUNC(timer_based_on_wear_expire_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "expire_event <Factor> Null pointer" );
		return 0;
	}

	LPITEM pkItem = info->item;
	int remain_time = pkItem->GetSocket(ITEM_SOCKET_REMAIN_SEC) - processing_time/passes_per_sec;
	if (remain_time <= 0)
	{
		sys_log(0, "ITEM EXPIRED : expired %s %u", pkItem->GetName(), pkItem->GetID());
		pkItem->SetTimerBasedOnWearExpireEvent(NULL);
		pkItem->SetSocket(ITEM_SOCKET_REMAIN_SEC, 0);

		// ÀÏ´Ü timer based on wear ¿ëÈ¥¼®Àº ½Ã°£ ´Ù µÇ¾ú´Ù°í ¾ø¾ÖÁö ¾Ê´Â´Ù.
		if (pkItem->IsDragonSoul())
		{
			DSManager::instance().DeactivateDragonSoul(pkItem);
		}
#ifdef USE_CRYSTAL_SYSTEM
		else if (pkItem->ModifyBoostCrystal(false, false, true))
		{
			return 0;
		}
#endif
		else
		{
#ifdef USE_BOOST_POTIONS
			if (pkItem->DeactivateBoostPotion())
			{
				
			}
#endif

			ITEM_MANAGER::instance().RemoveItem(pkItem, "TIMER_BASED_ON_WEAR_EXPIRE");
		}

		return 0;
	}

	pkItem->SetSocket(ITEM_SOCKET_REMAIN_SEC, remain_time);
	return PASSES_PER_SEC (MIN (60, remain_time));
}

void CItem::SetUniqueExpireEvent(LPEVENT pkEvent)
{
	m_pkUniqueExpireEvent = pkEvent;
}

void CItem::SetTimerBasedOnWearExpireEvent(LPEVENT pkEvent)
{
	m_pkTimerBasedOnWearExpireEvent = pkEvent;
}

EVENTFUNC(real_time_expire_event)
{
	const item_vid_event_info* info = reinterpret_cast<const item_vid_event_info*>(event->info);

	if (NULL == info)
		return 0;

	const LPITEM item = ITEM_MANAGER::instance().FindByVID( info->item_vid );

	if (NULL == item)
		return 0;

	const time_t current = get_global_time();

	if (current > item->GetSocket(0))
	{
		switch (item->GetVnum())
		{
			if(item->IsNewMountItem())
			{
				if (item->GetSocket(2) != 0)
					item->ClearMountAttributeAndAffect();
			}
			break;
		}
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		if(item->IsCostumeMount())
		{
			LPCHARACTER owner = item->GetOwner();
			if (owner)
			{
				owner->StopRiding();
				owner->HorseSummon(false);
			}
		}
#endif

		if (item->GetType() == ITEM_BOOSTER)
		{
			item->Lock(true);

			return 0;
		}

		if(item->IsNewPetItem())
			return 0;


#ifdef ENABLE_MOUNT_SKIN
		const bool isNeedRefresh = item->IsCostumeMountSkin() ? true : false;
		LPCHARACTER itOwner = item->GetOwner();
#endif
		ITEM_MANAGER::instance().RemoveItem(item, "REAL_TIME_EXPIRE");

#ifdef ENABLE_MOUNT_SKIN
		if(isNeedRefresh && itOwner)
		{
			itOwner->StopRiding();
			itOwner->HorseSummon(false);
		}
#endif

		return 0;
	}

	return PASSES_PER_SEC(1);
}

void CItem::StartRealTimeExpireEvent()
{
	if (m_pkRealTimeExpireEvent)
		return;

	for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType || LIMIT_REAL_TIME_START_FIRST_USE == GetProto()->aLimits[i].bType)
		{
			item_vid_event_info* info = AllocEventInfo<item_vid_event_info>();
			info->item_vid = GetVID();

			m_pkRealTimeExpireEvent = event_create( real_time_expire_event, info, PASSES_PER_SEC(1));

			sys_log(0, "REAL_TIME_EXPIRE: StartRealTimeExpireEvent");

			return;
		}
	}
}

bool CItem::IsRealTimeItem()
{
	if(!GetProto())
		return false;
	for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType)
			return true;
	}
	return false;
}

#ifdef ENABLE_PITTY_REFINE
int CItem::GetLimitPittyRefine()
{
	if(!GetProto())
		return 0;

	for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
	{
		if (LIMIT_PITTY_REFINE == GetProto()->aLimits[i].bType)
			return GetProto()->aLimits[i].lValue;
	}
	return 0;
}
#endif

void CItem::StartUniqueExpireEvent()
{
	if (GetType() != ITEM_UNIQUE)
		return;

	if (m_pkUniqueExpireEvent)
		return;

	//±â°£Á¦ ¾ÆÀÌÅÛÀÏ °æ¿ì ½Ã°£Á¦ ¾ÆÀÌÅÛÀº µ¿ÀÛÇÏÁö ¾Ê´Â´Ù
	if (IsRealTimeItem())
		return;

	// HARD CODING
	if (GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		m_pOwner->ShowAlignment(false);

	int iSec = GetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME);

	if (iSec == 0)
		iSec = 60;
	else
		iSec = MIN(iSec, 60);

	SetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME, 0);

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetUniqueExpireEvent(event_create(unique_expire_event, info, PASSES_PER_SEC(iSec)));
}

// ½Ã°£ ÈÄºÒÁ¦
// timer_based_on_wear_expire_event ¼³¸í ÂüÁ¶
void CItem::StartTimerBasedOnWearExpireEvent()
{
	if (m_pkTimerBasedOnWearExpireEvent)
		return;

	//±â°£Á¦ ¾ÆÀÌÅÛÀÏ °æ¿ì ½Ã°£Á¦ ¾ÆÀÌÅÛÀº µ¿ÀÛÇÏÁö ¾Ê´Â´Ù
	if (IsRealTimeItem())
		return;

	if (-1 == GetProto()->cLimitTimerBasedOnWearIndex)
		return;

	int iSec = GetSocket(0);

	// ³²Àº ½Ã°£À» ºÐ´ÜÀ§·Î ²÷±â À§ÇØ...
	if (0 != iSec)
	{
		iSec %= 60;
		if (0 == iSec)
			iSec = 60;
	}

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetTimerBasedOnWearExpireEvent(event_create(timer_based_on_wear_expire_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::StopUniqueExpireEvent()
{
	if (!m_pkUniqueExpireEvent)
		return;

	if (GetValue(2) != 0) // °ÔÀÓ½Ã°£Á¦ ÀÌ¿ÜÀÇ ¾ÆÀÌÅÛÀº UniqueExpireEvent¸¦ Áß´ÜÇÒ ¼ö ¾ø´Ù.
		return;

	// HARD CODING
	if (GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		m_pOwner->ShowAlignment(true);

	SetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME, event_time(m_pkUniqueExpireEvent) / passes_per_sec);
	event_cancel(&m_pkUniqueExpireEvent);

	ITEM_MANAGER::instance().SaveSingleItem(this);
}

void CItem::StopTimerBasedOnWearExpireEvent()
{
	if (!m_pkTimerBasedOnWearExpireEvent)
		return;

	int remain_time = GetSocket(ITEM_SOCKET_REMAIN_SEC) - event_processing_time(m_pkTimerBasedOnWearExpireEvent) / passes_per_sec;

	SetSocket(ITEM_SOCKET_REMAIN_SEC, remain_time);
	event_cancel(&m_pkTimerBasedOnWearExpireEvent);

	ITEM_MANAGER::instance().SaveSingleItem(this);
}

#ifdef ENABLE_SWITCHBOT_WORLDARD
void CItem::ApplyAddon(int iAddonType, bool switchbot)
#else
void CItem::ApplyAddon(int iAddonType)
#endif
{
#ifdef ENABLE_SWITCHBOT_WORLDARD
	CItemAddonManager::instance().ApplyAddonTo(iAddonType, this, switchbot);
#else
	CItemAddonManager::instance().ApplyAddonTo(iAddonType, this);
#endif
}

int CItem::GetSpecialGroup() const
{
	return ITEM_MANAGER::instance().GetSpecialGroupFromItem(GetVnum());
}

//
// ¾Ç¼¼¼­¸® ¼ÒÄÏ Ã³¸®.
//
bool CItem::IsAccessoryForSocket()
{
#ifdef STONE_INTO_COSTUMES
	return (m_pProto->bType == ITEM_ARMOR && (m_pProto->bSubType == ARMOR_WRIST || m_pProto->bSubType == ARMOR_NECK || m_pProto->bSubType == ARMOR_EAR)) ||
		(m_pProto->bType == ITEM_BELT) || (m_pProto->bType == ITEM_COSTUME && m_pProto->bSubType == COSTUME_BODY) || (m_pProto->bType == ITEM_NEW_RINGS);			// 2013³â 2¿ù »õ·Î Ãß°¡µÈ 'º§Æ®' ¾ÆÀÌÅÛÀÇ °æ¿ì ±âÈ¹ÆÀ¿¡¼­ ¾Ç¼¼¼­¸® ¼ÒÄÏ ½Ã½ºÅÛÀ» ±×´ë·Î ÀÌ¿ëÇÏÀÚ°í ÇÔ.
#else
	return (m_pProto->bType == ITEM_ARMOR && (m_pProto->bSubType == ARMOR_WRIST || m_pProto->bSubType == ARMOR_NECK || m_pProto->bSubType == ARMOR_EAR )) ||
		(m_pProto->bType == ITEM_BELT) || (m_pProto->bType == ITEM_NEW_RINGS);				// 2013³â 2¿ù »õ·Î Ãß°¡µÈ 'º§Æ®' ¾ÆÀÌÅÛÀÇ °æ¿ì ±âÈ¹ÆÀ¿¡¼­ ¾Ç¼¼¼­¸® ¼ÒÄÏ ½Ã½ºÅÛÀ» ±×´ë·Î ÀÌ¿ëÇÏÀÚ°í ÇÔ.
#endif
}

void CItem::SetAccessorySocketGrade(int iGrade)
{
	SetSocket(0, MINMAX(0, iGrade, GetAccessorySocketMaxGrade()));

	int iDownTime = aiAccessorySocketDegradeTime[GetAccessorySocketGrade()];

	//if (test_server)
	//	iDownTime /= 60;

	SetAccessorySocketDownGradeTime(iDownTime);
}

void CItem::SetAccessorySocketMaxGrade(int iMaxGrade)
{
	SetSocket(1, MINMAX(0, iMaxGrade, ITEM_ACCESSORY_SOCKET_MAX_NUM));
}

void CItem::SetAccessorySocketDownGradeTime(DWORD time)
{
	SetSocket(2, time);

	if (test_server && GetOwner())
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, "822 %s %d", GetName(), time);
}

EVENTFUNC(accessory_socket_expire_event)
{
	item_vid_event_info* info = dynamic_cast<item_vid_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "accessory_socket_expire_event> <Factor> Null pointer" );
		return 0;
	}

	LPITEM item = ITEM_MANAGER::instance().FindByVID(info->item_vid);
	if(!item)
		return 0;

	if (item->GetAccessorySocketDownGradeTime() <= 1)
	{
degrade:
		item->SetAccessorySocketExpireEvent(NULL);
		item->AccessorySocketDegrade();
		return 0;
	}
	else
	{
		int iTime = item->GetAccessorySocketDownGradeTime() - 60;

		if (iTime <= 1)
			goto degrade;

		item->SetAccessorySocketDownGradeTime(iTime);

		if (iTime > 60)
			return PASSES_PER_SEC(60);
		else
			return PASSES_PER_SEC(iTime);
	}
}

void CItem::StartAccessorySocketExpireEvent()
{
	if (!IsAccessoryForSocket())
		return;

	if (m_pkAccessorySocketExpireEvent)
		return;

	if (GetAccessorySocketMaxGrade() == 0)
		return;

	if (GetAccessorySocketGrade() == 0)
		return;

	int iSec = GetAccessorySocketDownGradeTime();
	SetAccessorySocketExpireEvent(NULL);

	if (iSec <= 1)
		iSec = 5;
	else
		iSec = MIN(iSec, 60);

	item_vid_event_info* info = AllocEventInfo<item_vid_event_info>();
	info->item_vid = GetVID();

	SetAccessorySocketExpireEvent(event_create(accessory_socket_expire_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::StopAccessorySocketExpireEvent()
{
	if (!m_pkAccessorySocketExpireEvent)
		return;

	if (!IsAccessoryForSocket())
		return;

	int new_time = GetAccessorySocketDownGradeTime() - (60 - event_time(m_pkAccessorySocketExpireEvent) / passes_per_sec);

	event_cancel(&m_pkAccessorySocketExpireEvent);

	if (new_time <= 1)
	{
		AccessorySocketDegrade();
	}
	else
	{
		SetAccessorySocketDownGradeTime(new_time);
	}
}

bool CItem::IsRideItem()
{
	if (ITEM_UNIQUE == GetType() && UNIQUE_SPECIAL_RIDE == GetSubType())
		return true;
	if (ITEM_UNIQUE == GetType() && UNIQUE_SPECIAL_MOUNT_RIDE == GetSubType())
		return true;
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (ITEM_COSTUME == GetType() && COSTUME_MOUNT == GetSubType())
		return true;
#endif
#ifdef ENABLE_MOUNT_SKIN
	if (ITEM_COSTUME == GetType() && COSTUME_MOUNT_SKIN == GetSubType())
		return true;
#endif
	return false;
}

bool CItem::IsRamadanRing()
{
	if (GetVnum() == UNIQUE_ITEM_RAMADAN_RING)
		return true;
	return false;
}

void CItem::ClearMountAttributeAndAffect()
{
	LPCHARACTER ch = GetOwner();

	ch->RemoveAffect(AFFECT_MOUNT);
	ch->RemoveAffect(AFFECT_MOUNT_BONUS);

	ch->MountVnum(0);

	ch->PointChange(POINT_ST, 0);
	ch->PointChange(POINT_DX, 0);
	ch->PointChange(POINT_HT, 0);
	ch->PointChange(POINT_IQ, 0);
}

// fixme
// ÀÌ°Å Áö±ÝÀº ¾È¾´µ¥... ±Ùµ¥ È¤½Ã³ª ½Í¾î¼­ ³²°ÜµÒ.
// by rtsummit
bool CItem::IsNewMountItem()
{
	switch(GetVnum())
	{
		case 76000: case 76001: case 76002: case 76003:
		case 76004: case 76005: case 76006: case 76007:
		case 76008: case 76009: case 76010: case 76011:
		case 76012: case 76013: case 76014:
			return true;
	}
	return false;
}

void CItem::SetAccessorySocketExpireEvent(LPEVENT pkEvent)
{
	m_pkAccessorySocketExpireEvent = pkEvent;
}

void CItem::AccessorySocketDegrade()
{
	if (GetAccessorySocketGrade() > 0)
	{
		LPCHARACTER ch = GetOwner();

		if (ch)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "823 %s", GetName());
		}

		ModifyPoints(false);
		SetAccessorySocketGrade(GetAccessorySocketGrade()-1);
		ModifyPoints(true);

		int iDownTime = aiAccessorySocketDegradeTime[GetAccessorySocketGrade()];

		if (test_server)
			iDownTime /= 60;

		SetAccessorySocketDownGradeTime(iDownTime);

		if (iDownTime)
			StartAccessorySocketExpireEvent();
	}
}

// ring¿¡ itemÀ» ¹ÚÀ» ¼ö ÀÖ´ÂÁö ¿©ºÎ¸¦ Ã¼Å©ÇØ¼­ ¸®ÅÏ
static const bool CanPutIntoRing(LPITEM ring, LPITEM item)
{
	//const DWORD vnum = item->GetVnum();
	return false;
}

bool CItem::CanPutInto(LPITEM item)
{
	if (item->GetType() == ITEM_BELT)
		return this->GetSubType() == USE_PUT_INTO_BELT_SOCKET;

	else if(item->GetType() == ITEM_RING)
		return CanPutIntoRing(item, this);

#ifdef __AURA_SYSTEM__
	else if (item->GetType() == ITEM_COSTUME && item->GetSubType() == COSTUME_AURA)
		return this->GetSubType() == USE_PUT_INTO_AURA_SOCKET;
#endif
	if (item->GetType() != ITEM_NEW_RINGS && item->GetType() != ITEM_ARMOR)
	{
		return false;
	}

	DWORD vnum = item->GetVnum();

	struct JewelAccessoryInfo
	{
		DWORD jewel;
		DWORD wrist;
		DWORD neck;
		DWORD ear;
		DWORD rings;
	};
	const static JewelAccessoryInfo infos[] = {
		{ 50634, 14420, 16220, 17220, 19220},
		{ 50635, 14500, 16500, 17500, 19500},
		{ 50636, 14520, 16520, 19520},
		{ 50637, 14540, 16540, 17540, 19540},
		{ 50638, 14560, 16560, 17560, 19560},
		{ 50639, 14570, 16570, 17520},
		{ 50662, 14600, 16600, 17590 },
	};

	DWORD item_type = (item->GetVnum() / 10) * 10;
	for (size_t i = 0; i < sizeof(infos) / sizeof(infos[0]); i++)
	{
		const JewelAccessoryInfo& info = infos[i];
		switch(item->GetSubType())
		{
		case ARMOR_WRIST:
			if (info.wrist == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_NECK:
			if (info.neck == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_EAR:
			if (info.ear == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;

		}


		if (item->GetType() == ITEM_NEW_RINGS){
			if (info.rings == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}

	if (item->GetType() == ITEM_NEW_RINGS){
		vnum -= 19000;
	}
	else
	{
		if (item->GetSubType() == ARMOR_WRIST)
			vnum -= 14000;
		else if (item->GetSubType() == ARMOR_NECK)
			vnum -= 16000;
		else if (item->GetSubType() == ARMOR_EAR)
			vnum -= 17000;	
		else
			return false;
	}

	DWORD type = vnum / 20;

	if (type < 0 || type > 11)
	{
		type = (vnum - 170) / 20;

		if (50623 + type != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16210 && item->GetVnum() <= 16219)
	{
		if (50625 != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16230 && item->GetVnum() <= 16239)
	{
		if (50626 != GetVnum())
			return false;
		else
			return true;
	}

	return 50623 + type == GetVnum();
}

#ifdef __AURA_SYSTEM__
bool CItem::IsAuraBoosterForSocket()
{
	if (m_pProto->bType == ITEM_COSTUME && m_pProto->bSubType == COSTUME_AURA)
		return this->GetSocket(ITEM_SOCKET_AURA_BOOST) == 0;

	return false;
}

EVENTFUNC(aura_boost_socket_expire_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>(event->info);
	if (info == NULL)
	{
		sys_err("aura_boost_socket_expire_event> <Factor> Null pointer");
		return 0;
	}

	LPITEM pkAura = info->item;
	const long c_lBoosterSocket = pkAura->GetSocket(ITEM_SOCKET_AURA_BOOST);
	const long c_lBoosterIndex = c_lBoosterSocket / 100000000;
	if (c_lBoosterIndex <= ITEM_AURA_BOOST_ERASER || c_lBoosterIndex >= ITEM_AURA_BOOST_MAX)
		return 0;

	DWORD dwBoosterItemVnum = ITEM_AURA_BOOST_ITEM_VNUM_BASE + c_lBoosterIndex;
	TItemTable* pBoosterProto = ITEM_MANAGER::instance().GetTable(dwBoosterItemVnum);
	if (!pBoosterProto)
		return 0;

	if (pBoosterProto->alValues[ITEM_AURA_BOOST_UNLIMITED_VALUE] == 1)
		return 0;

	long lBoostTimeSec = (c_lBoosterSocket % 100000000) - processing_time / passes_per_sec;
	if (lBoostTimeSec <= 0)
	{
		sys_log(0, "AURA BOOST EXPIRED : expired %s %lu", pkAura->GetName(), pkAura->GetID());
		pkAura->SetAuraBoosterSocketExpireEvent(NULL);
		pkAura->ModifyPoints(false);
		pkAura->SetSocket(ITEM_SOCKET_AURA_BOOST, 0);
		pkAura->ModifyPoints(true);
		if (pkAura->GetOwner())
		{
			pkAura->GetOwner()->ComputeBattlePoints();
			pkAura->GetOwner()->UpdatePacket();
		}

		LogManager::instance().ItemLog(pkAura->GetOwner(), pkAura, "AURA_BOOST_EXPIRED", pkAura->GetName());

		return 0;
	}

	pkAura->SetSocket(ITEM_SOCKET_AURA_BOOST, c_lBoosterIndex * 100000000 + lBoostTimeSec);
	return PASSES_PER_SEC(MIN(60, lBoostTimeSec));
}

void CItem::StartAuraBoosterSocketExpireEvent()
{
	if (!(m_pProto->bType == ITEM_COSTUME && m_pProto->bSubType == COSTUME_AURA))
		return;

	if (GetSocket(ITEM_SOCKET_AURA_BOOST) == 0)
		return;

	if (m_pkAuraBoostSocketExpireEvent)
		return;

	const long c_lBoosterSocket = GetSocket(ITEM_SOCKET_AURA_BOOST);
	const long c_lBoosterIndex = c_lBoosterSocket / 100000000;
	if (c_lBoosterIndex <= ITEM_AURA_BOOST_ERASER || c_lBoosterIndex >= ITEM_AURA_BOOST_MAX)
		return;

	DWORD dwBoosterItemVnum = ITEM_AURA_BOOST_ITEM_VNUM_BASE + c_lBoosterIndex;
	TItemTable* pBoosterProto = ITEM_MANAGER::instance().GetTable(dwBoosterItemVnum);
	if (!pBoosterProto)
		return;

	if (pBoosterProto->alValues[ITEM_AURA_BOOST_UNLIMITED_VALUE] == 1)
		return;

	long lBoostTimeSec = c_lBoosterSocket % 100000000;
	if (0 != lBoostTimeSec)
	{
		lBoostTimeSec %= 60;
		if (0 == lBoostTimeSec)
			lBoostTimeSec = 60;
	}

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetAuraBoosterSocketExpireEvent(event_create(aura_boost_socket_expire_event, info, PASSES_PER_SEC(lBoostTimeSec)));
}

void CItem::StopAuraBoosterSocketExpireEvent()
{
	if (!m_pkAuraBoostSocketExpireEvent)
		return;

	const long c_lBoosterSocket = GetSocket(ITEM_SOCKET_AURA_BOOST);
	const long c_lBoosterIndex = c_lBoosterSocket / 100000000;
	if (c_lBoosterIndex <= ITEM_AURA_BOOST_ERASER || c_lBoosterIndex >= ITEM_AURA_BOOST_MAX)
		return;

	DWORD dwBoosterItemVnum = ITEM_AURA_BOOST_ITEM_VNUM_BASE + c_lBoosterIndex;
	TItemTable* pBoosterProto = ITEM_MANAGER::instance().GetTable(dwBoosterItemVnum);
	if (!pBoosterProto)
		return;

	long lBoostTimeSec = (c_lBoosterSocket % 100000000) - event_processing_time(m_pkAuraBoostSocketExpireEvent) / passes_per_sec;
	SetSocket(ITEM_SOCKET_AURA_BOOST, c_lBoosterIndex * 100000000 + lBoostTimeSec);
	event_cancel(&m_pkAuraBoostSocketExpireEvent);

	ITEM_MANAGER::instance().SaveSingleItem(this);
}

void CItem::SetAuraBoosterSocketExpireEvent(LPEVENT pkEvent)
{
	m_pkAuraBoostSocketExpireEvent = pkEvent;
}
#endif

int CItem::GetAccessorySocketVnumRings(LPITEM item)
{

	struct RingsAccessoryInfo
	{
		DWORD jewel;
		DWORD rings;
	};

	const int ACCESSORY_MATERIAL_LIST[] = {50623, 50624, 50625, 50626, 50627, 50628, 50629, 50630, 50631, 50632, 50633, 50634, 50635, 50636, 50637, 50638, 50639, 50662};
	const static RingsAccessoryInfo infos[] = {
		{ 50634,  19220},
		{ 50635,  19500},
		{ 50636,  19520},
		{ 50637,  19540},
		{ 50638,  19560},
		{ 50639,  14570},
		{ 50639,  16570},
		{ 50639,  17520},
		{ 50662,  14600},
		{ 50662,  16600},
		{ 50662,  17590},
	};

	DWORD vnum = item->GetVnum();

	DWORD item_type = (item->GetVnum() / 10) * 10;
	for (size_t i = 0; i < sizeof(infos) / sizeof(infos[0]); i++)
	{
		const RingsAccessoryInfo& info = infos[i];
		if (item->GetType() == ITEM_NEW_RINGS){
			if (info.rings == item_type)
			{
				return info.jewel;
			}
		}
	}

	if (item->GetType() == ITEM_NEW_RINGS){
		vnum -= 19000;
	}

	DWORD type = vnum / 20;

	if (type < 0 || type >= (sizeof(ACCESSORY_MATERIAL_LIST) / sizeof(ACCESSORY_MATERIAL_LIST[0])))
	{
		type = (vnum - 170) / 20;

		if (type < 0 || type >= (sizeof(ACCESSORY_MATERIAL_LIST) / sizeof(ACCESSORY_MATERIAL_LIST[0])))
			return 0;
	}

	return ACCESSORY_MATERIAL_LIST[type];

}

// PC_BANG_ITEM_ADD
bool CItem::IsPCBangItem()
{
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (m_pProto->aLimits[i].bType == LIMIT_PCBANG)
			return true;
	}
	return false;
}
// END_PC_BANG_ITEM_ADD

bool CItem::CheckItemUseLevel(int nLevel
#ifdef ENABLE_YOHARA_SYSTEM
, int nConquerorLevel
#endif
)
{
	if (!m_pProto)
	{
		return false;
	}

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (m_pProto->aLimits[i].bType == LIMIT_LEVEL)
		{
			if (m_pProto->aLimits[i].lValue > nLevel)
			{
				return false;
			}
		}
#ifdef ENABLE_YOHARA_SYSTEM
		else if (m_pProto->aLimits[i].bType == LIMIT_CONQUEROR_LEVEL)
		{
			if (m_pProto->aLimits[i].lValue > nConquerorLevel)
			{
				return false;
			}
		}
#endif
	}

	return true;
}

long CItem::FindApplyValue(BYTE bApplyType)
{
	if (m_pProto == NULL)
		return 0;

	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
		if (m_pProto->aApplies[i].bType == bApplyType)
			return m_pProto->aApplies[i].lValue;
	}

	return 0;
}

void CItem::CopySocketTo(LPITEM pItem)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		pItem->m_alSockets[i] = m_alSockets[i];
	}
}

int CItem::GetAccessorySocketGrade()
{
   	return MINMAX(0, GetSocket(0), GetAccessorySocketMaxGrade());
}

int CItem::GetAccessorySocketMaxGrade()
{
   	return MINMAX(0, GetSocket(1), ITEM_ACCESSORY_SOCKET_MAX_NUM);
}

int CItem::GetAccessorySocketDownGradeTime()
{
	return MINMAX(0, GetSocket(2), aiAccessorySocketDegradeTime[GetAccessorySocketGrade()]);
}

void CItem::AttrLog()
{
	const char * pszIP = NULL;

	if (GetOwner() && GetOwner()->GetDesc())
		pszIP = GetOwner()->GetDesc()->GetHostName();

	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		if (m_alSockets[i])
		{
#ifdef ENABLE_NEWSTUFF
			if (g_iDbLogLevel>=LOG_LEVEL_MAX)
#endif
			LogManager::instance().ItemLog(i, m_alSockets[i], 0, GetID(), "INFO_SOCKET", "", pszIP ? pszIP : "", GetOriginalVnum());
		}
	}

	for (int i = 0; i<ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		int	type	= m_aAttr[i].bType;
		int value	= m_aAttr[i].sValue;

		if (type)
		{
#ifdef ENABLE_NEWSTUFF
			if (g_iDbLogLevel>=LOG_LEVEL_MAX)
#endif
			LogManager::instance().ItemLog(i, type, value, GetID(), "INFO_ATTR", "", pszIP ? pszIP : "", GetOriginalVnum());
		}
	}
}

int CItem::GetLevelLimit()
{
	if (!m_pProto)
	{
		return 0;
	}

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (m_pProto->aLimits[i].bType == LIMIT_LEVEL)
		{
			return this->m_pProto->aLimits[i].lValue;
		}
	}

	return 0;
}

#ifdef ENABLE_YOHARA_SYSTEM
int CItem::GetConquerorLevelLimit()
{
	if (!m_pProto)
	{
		return 0;
	}

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (m_pProto->aLimits[i].bType == LIMIT_CONQUEROR_LEVEL)
		{
			return this->m_pProto->aLimits[i].lValue;
		}
	}

	return 0;
}
#endif

bool CItem::OnAfterCreatedItem()
{
	// ¾ÆÀÌÅÛÀ» ÇÑ ¹øÀÌ¶óµµ »ç¿ëÇß´Ù¸é, ±× ÀÌÈÄ¿£ »ç¿ë ÁßÀÌÁö ¾Ê¾Æµµ ½Ã°£ÀÌ Â÷°¨µÇ´Â ¹æ½Ä
	if (-1 != this->GetProto()->cLimitRealTimeFirstUseIndex)
	{
		// Socket1¿¡ ¾ÆÀÌÅÛÀÇ »ç¿ë È½¼ö°¡ ±â·ÏµÇ¾î ÀÖÀ¸´Ï, ÇÑ ¹øÀÌ¶óµµ »ç¿ëÇÑ ¾ÆÀÌÅÛÀº Å¸ÀÌ¸Ó¸¦ ½ÃÀÛÇÑ´Ù.
		if (0 != GetSocket(1))
		{
			StartRealTimeExpireEvent();
		}
	}
#ifdef __SOULBINDING_SYSTEM__
	if (IsUntilBind())
		StartUnBindingExpireEvent();
#endif
	return true;
}

bool CItem::IsDragonSoul()
{
	return GetType() == ITEM_DS;
}

#ifdef ENABLE_SPECIAL_STORAGE
bool CItem::IsUpgradeItem()
{
	return ((GetType() == ITEM_MATERIAL && GetSubType() == MATERIAL_LEATHER) || (GetVnum() == 27987 || GetVnum() == 27799 || GetVnum() == 70031 || (GetVnum() >= 27992 && GetVnum() <= 27994)));
}

bool CItem::IsBook()
{
	return (GetType() == ITEM_SKILLBOOK || CItemVnumHelper::IsExtraBook(GetVnum()));
}

bool CItem::IsStone()
{
	return (GetType() == ITEM_METIN && GetSubType() == METIN_NORMAL);
}

bool CItem::IsChange()
{
	if (GetType() == ITEM_USE)
	{
		switch (GetSubType())
		{
			case USE_CHANGE_ATTRIBUTE:
#ifdef USE_CRYSTAL_SYSTEM
			case USE_CHANGE_ATTR_CRYSTAL:
#endif
			{
				return true;
			}
			default:
			{
				break;
			}
		}
	}

	switch (GetVnum())
	{
		case 72346:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}

bool CItem::IsCostumeSkin()
{
	return GetType() == ITEM_COSTUME && GetSubType() != COSTUME_MOUNT && GetSubType() != COSTUME_PET && GetSubType() != COSTUME_SASH;
}
bool CItem::IsCostume()
{
	return false;//when create false for to inv! bro this function last line return <-

	switch(GetVnum())
	{
		//case 85005:
		case 41004:
		case 40101:
		case 45005:
		case 87012:
			return false;
	}
	return GetType() == ITEM_COSTUME && GetSubType() != COSTUME_MOUNT && GetSubType() != COSTUME_PET;
}

bool CItem::IsChest()
{
	return (GetType() == ITEM_GIFTBOX);
}
#endif

int CItem::GiveMoreTime_Per(float fPercent)
{
	if (IsDragonSoul())
	{
		DWORD duration = DSManager::instance().GetDuration(this);
		DWORD remain_sec = GetSocket(ITEM_SOCKET_REMAIN_SEC);
		DWORD given_time = fPercent * duration / 100u;
		if (remain_sec == duration)
			return false;
		if ((given_time + remain_sec) >= duration)
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, duration);
			return duration - remain_sec;
		}
		else
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, given_time + remain_sec);
			return given_time;
		}
	}
	// ¿ì¼± ¿ëÈ¥¼®¿¡ °üÇØ¼­¸¸ ÇÏµµ·Ï ÇÑ´Ù.
	else
		return 0;
}

int CItem::GiveMoreTime_Fix(DWORD dwTime)
{
	if (IsDragonSoul())
	{
		DWORD duration = DSManager::instance().GetDuration(this);
		DWORD remain_sec = GetSocket(ITEM_SOCKET_REMAIN_SEC);
		if (remain_sec == duration)
			return false;
		if ((dwTime + remain_sec) >= duration)
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, duration);
			return duration - remain_sec;
		}
		else
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, dwTime + remain_sec);
			return dwTime;
		}
	}
	// ¿ì¼± ¿ëÈ¥¼®¿¡ °üÇØ¼­¸¸ ÇÏµµ·Ï ÇÑ´Ù.
	else
		return 0;
}


int	CItem::GetDuration()
{
	if(!GetProto())
		return -1;

	for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType)
			return GetProto()->aLimits[i].lValue;
	}

	if (GetProto()->cLimitTimerBasedOnWearIndex >= 0)
	{
		BYTE cLTBOWI = GetProto()->cLimitTimerBasedOnWearIndex;
		return GetProto()->aLimits[cLTBOWI].lValue;
	}

	return -1;
}

bool CItem::IsSameSpecialGroup(const LPITEM item) const
{
	// ¼­·Î VNUMÀÌ °°´Ù¸é °°Àº ±×·ìÀÎ °ÍÀ¸·Î °£ÁÖ
	if (this->GetVnum() == item->GetVnum())
		return true;

	if (GetSpecialGroup() && (item->GetSpecialGroup() == GetSpecialGroup()))
		return true;

	return false;
}

#ifdef ENABLE_NEW_NAME_ITEM
void CItem::SetNewName(const char* name)
{
	strlcpy(nameex,name,sizeof(nameex));
}
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
bool CItem::IsCostumePetItem()
{
	return GetType() == ITEM_COSTUME && GetSubType() == COSTUME_PET;
}
bool CItem::IsNewPetItem()
{
	return GetType() == ITEM_PET && GetSubType() == PET_LEVELABLE;
}
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
bool CItem::IsCostumeMount()
{
	return GetType() == ITEM_COSTUME && GetSubType() == COSTUME_MOUNT;
}
#endif

#ifdef ENABLE_MOUNT_SKIN
bool CItem::IsCostumeMountSkin()
{
	return GetType() == ITEM_COSTUME && GetSubType() == COSTUME_MOUNT_SKIN;
}
#endif


#ifdef ENABLE_SPECIAL_COSTUME_ATTR
bool CItem::CanBonusCostume()
{
#ifdef __ITEM_SHINING__
	if(GetType() == ITEM_SHINING)
	{
		switch (GetSubType())
		{
			case SHINING_WEAPON:
			case SHINING_ARMOR:
			case SHINING_SPECIAL:
				return true;
		}
	}
#endif
	else if (GetType() == ITEM_COSTUME)
	{
		switch (GetSubType())
		{
			case COSTUME_BODY:
				case COSTUME_HAIR:
				case COSTUME_WEAPON:
#ifdef ENABLE_MOUNT_SKIN
				case COSTUME_MOUNT_SKIN:
#endif
#ifdef ENABLE_SASH_COSTUME_SYSTEM
				case COSTUME_NEW_SASH:
#endif
					return true;
		}
	}
	return false;
}

BYTE CItem::GetBonusFlag()
{
#ifdef __ITEM_SHINING__
	if(GetType() == ITEM_SHINING)
	{
		switch (GetSubType())
		{
			case SHINING_WEAPON:
			case SHINING_ARMOR:
			case SHINING_SPECIAL:
				return 6;
		}
	}
#endif
	else if (GetType() == ITEM_COSTUME)
	{
		switch (GetSubType())
		{
			case COSTUME_BODY:
				return 1;
			case COSTUME_HAIR:
				return 2;
			case COSTUME_WEAPON:
				return 3;
#ifdef ENABLE_MOUNT_SKIN
			case COSTUME_MOUNT_SKIN:
				return 4;
#endif
#ifdef ENABLE_SASH_COSTUME_SYSTEM
			case COSTUME_NEW_SASH:
				return 5;
#endif
		}
	}
	return 0;
}
BYTE CItem::GetBonusMaxAttr()
{
#ifdef __ITEM_SHINING__
	if(GetType() == ITEM_SHINING)
	{
		switch (GetSubType())
		{
			case SHINING_WEAPON:
			case SHINING_ARMOR:
			case SHINING_SPECIAL:
				return SPECIAL_ATTR_COSTUME_SHINING_LIMIT;
		}
	}
#endif
	else if (GetType() == ITEM_COSTUME)
	{
		switch (GetSubType())
		{
			case COSTUME_BODY:
				return SPECIAL_ATTR_COSTUME_BODY_LIMIT;
			case COSTUME_HAIR:
				return SPECIAL_ATTR_COSTUME_HAIR_LIMIT;
			case COSTUME_WEAPON:
				return SPECIAL_ATTR_COSTUME_WEAPON_LIMIT;
#ifdef ENABLE_MOUNT_SKIN
			case COSTUME_MOUNT_SKIN:
				return SPECIAL_ATTR_COSTUME_MOUNT_SKIN_LIMIT;
#endif
#ifdef ENABLE_SASH_COSTUME_SYSTEM
			case COSTUME_NEW_SASH:
				return SPECIAL_ATTR_COSTUME_SASH_SKIN_LIMIT;
#endif
		}
	}
	return 0;
}
#endif

#ifdef ENABLE_COPY_ATTR_ITEM
bool CItem::CanCopyAttrItem()
{
	switch(GetType())
	{
		//case ITEM_WEAPON:
		//	if(GetSubType() != WEAPON_ARROW && GetSubType() != WEAPON_QUIVER)
		//		return true;
		//	break;
		case ITEM_COSTUME:
		//case ITEM_ARMOR:
			return true;
	}
	return false;
}
#endif

#ifdef ENABLE_NEWEQ
bool CItem::IsNewEqItem()
{
	if (GetType() == ITEM_NEWRING && GetSubType() == RING_SLOT1)
		return true;
	else if (GetType() == ITEM_NEWRING && GetSubType() == RING_SLOT2)
		return true;
	else if (GetType() == ITEM_NEWRING && GetSubType() == RING_SLOT3)
		return true;
	else if (GetType() == ITEM_NEWRING && GetSubType() == RING_SLOT4)
		return true;
	else if (GetType() == ITEM_NEWRING && GetSubType() == RING_SLOT5)
		return true;
	else if (GetType() == ITEM_NEWRING && GetSubType() == RING_SLOT6)
		return true;
	else if (GetType() == ITEM_NEWRING && GetSubType() == RING_SLOT7)
		return true;
	else
		return false;
}
#endif

#ifdef __FAKE_PLAYER__
void CItem::StopExpireEvents()
{
	if (m_pkUniqueExpireEvent)
		event_cancel(&m_pkUniqueExpireEvent);

	if (m_pkTimerBasedOnWearExpireEvent)
		event_cancel(&m_pkTimerBasedOnWearExpireEvent);

	if (m_pkRealTimeExpireEvent)
		event_cancel(&m_pkRealTimeExpireEvent);

	if (m_pkAccessorySocketExpireEvent)
		event_cancel(&m_pkAccessorySocketExpireEvent);
}
#endif

#ifdef USE_BOOST_POTIONS
bool CItem::IsBoostPotion()
{
	const auto* proto = GetProto();
	return (proto && proto->bType == ITEM_USE && proto->bSubType == USE_SPECIAL_BOOST) ? true : false;
}

bool CItem::DeactivateBoostPotion()
{
	// CHECK IS ALREADY ON: EVENTFUNC(timer_based_on_wear_expire_event)
	// if (!IsBoostPotion())
	// {
		// return true;
	// }

	if (GetSocket(ITEM_BOOST_POTION_STATUS) != 0)
	{
		LPCHARACTER pOwner = GetOwner();
		if (!pOwner)
		{
			return true;
		}

		BYTE affectApply = APPLY_NONE;

		const auto* pProto = GetProto();
		if (pProto)
		{
			const DWORD affectType = static_cast<DWORD>(GetValue(ITEM_BOOST_POTION_AFFECT_VALUE));
			if (IS_BOOST_POTION_AFFECT(affectType))
			{
				for (const auto& apply : pProto->aApplies)
				{
					if (apply.bType != APPLY_NONE && apply.lValue != 0)
					{
						affectApply = apply.bType;
						break;
					}
				}

				if (affectApply != APPLY_NONE)
				{
					affectApply = aApplyInfo[affectApply].bPointType;

					CAffect* affect = pOwner->FindAffect(affectType, affectApply);
					if (affect)
					{
						if (pOwner->RemoveAffect(affect))
						{
							
						}
					}
				}
			}
		}

		// SetSocket(ITEM_BOOST_POTION_STATUS, 0);
	}

	return true;
}

bool CItem::IsTimeStackable()
{
	return (GetFlag() & ITEM_FLAG_TIME_STACKABLE) ? true : false;
}
#endif

#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
bool CItem::IsTimerBasedOnWear()
{
	const auto* proto = GetProto();
	if (proto)
	{
		for (const auto& limit : proto->aLimits)
		{
			if (limit.bType == LIMIT_TIMER_BASED_ON_WEAR && limit.lValue > 0)
			{
				return true;
			}
		}
	}

	return false;
}
#endif

#ifdef USE_CRYSTAL_SYSTEM
bool CItem::IsBoostCrystal()
{
	const auto* proto = GetProto();
	return (proto && proto->bType == ITEM_CRYSTAL) ? true : false;
}

bool CItem::ModifyBoostCrystal(bool bAdd, bool bAffectLoading, bool bMsg)
{
	if (!IsBoostCrystal())
	{
		return false;
	}

	LPCHARACTER pOwner = GetOwner();
	if (!pOwner)
	{
		return false;
	}

	if (!IsTimerBasedOnWear())
	{
		if (bMsg)
		{
			pOwner->ChatPacket(CHAT_TYPE_INFO, "1510 %d", 1);
			// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this crystal, please contact the staff. Err code: %d."), 1);
		}

		return false;
	}

	const auto crystalBnsType = GetSocket(CRYSTAL_BONUS_TYPE_SOCKET_SLOT);

	switch (crystalBnsType)
	{
		case CRYSTAL_BONUS_TYPE_PVM:
		case CRYSTAL_BONUS_TYPE_PVP:
		{
			const DWORD affectType = static_cast<DWORD>(AFFECT_BOOST_CRYSTAL);

			if (bAdd)
			{
				if (GetWindow() != INVENTORY)
				{
					if (bMsg)
					{
						pOwner->ChatPacket(CHAT_TYPE_INFO, "1510 %d", 2);
						// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this crystal, please contact the staff. Err code: %d."), 2);
					}

					return false;
				}

				const auto leftTime = GetSocket(ITEM_SOCKET_REMAIN_SEC);

				if (leftTime < 1)
				{
					if (bMsg)
					{
						pOwner->ChatPacket(CHAT_TYPE_INFO, "1511 %s", GetName());
						// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s doesn't have any left time."), GetName());
					}

					return false;
				}

				if (CArenaManager::instance().IsArenaMap(pOwner->GetMapIndex()))
				{
					if (bMsg)
					{
						pOwner->ChatPacket(CHAT_TYPE_INFO, "1512 %s", GetName());
						// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s cannot be used when in this map."), GetName());
					}

					return false;
				}
#ifdef ENABLE_NEWSTUFF
				else if (g_NoPotionsOnPVP && CPVPManager::instance().IsFighting(pOwner->GetPlayerID()) && !IsAllowedPotionOnPVP(GetVnum()))
				{
					if (bMsg)
					{
						pOwner->ChatPacket(CHAT_TYPE_INFO, "1513 %s", GetName());
						// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s cannot be used when you're in a duel."), GetName());
					}

					return false;
				}
#endif

				if (!bAffectLoading)
				{
					if (!pOwner->IsLoadedAffect())
					{
						if (bMsg)
						{
							pOwner->ChatPacket(CHAT_TYPE_INFO, "1514");
							// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Try again in few seconds."));
						}

						return false;
					}

					LPITEM item2 = nullptr;

					CAffect* affect = pOwner->FindAffect(affectType);
					if (affect)
					{
						item2 = pOwner->FindItemByID(affect->dwFlag);
					}

					pOwner->RemoveAffect(affectType);

					if (item2)
					{
						item2->Lock(false);
						item2->SetSocket(CRYSTAL_STATUS_SOCKET_SLOT, 0);
						item2->StopTimerBasedOnWearExpireEvent();
					}

					bool bEmpty = true;
					bool bAdded = true;

					for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
					{
						const TPlayerItemAttribute& ia = GetAttribute(i);
						if (bEmpty
							 && ia.bType != APPLY_NONE
							 && ia.sValue != 0)
						{
							bEmpty = false;
						}

						if (ia.bType != APPLY_NONE
							 && ia.sValue != 0)
						{
							if (!pOwner->AddAffect(affectType, aApplyInfo[ia.bType].bPointType, ia.sValue, GetID(), leftTime, 0, true, true, GetCell()))
							{
								bAdded = false;
								break;
							}
						}
					}

					if (bEmpty)
					{
						if (bMsg)
						{
							pOwner->ChatPacket(CHAT_TYPE_INFO, "1517 %s", GetName());
							// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s doesn't have any bonus so you cannot activate it."), GetName());
						}

						return false;
					}
					else if (!bAdded)
					{
						pOwner->RemoveAffect(affectType);

						if (bMsg)
						{
							pOwner->ChatPacket(CHAT_TYPE_INFO, "1510 %d", 3);
							// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with this crystal, please contact the staff. Err code: %d."), 3);
						}

						return false;
					}
				}

				Lock(true);
				SetSocket(CRYSTAL_STATUS_SOCKET_SLOT, 1);
				StartTimerBasedOnWearExpireEvent();

				if (!bAffectLoading && bMsg)
				{
					pOwner->ChatPacket(CHAT_TYPE_INFO, "1515 %s", GetName());
					// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s was activated."), GetName());
				}
			}
			else
			{
				pOwner->RemoveAffect(affectType);

				Lock(false);
				SetSocket(CRYSTAL_STATUS_SOCKET_SLOT, 0);
				StopTimerBasedOnWearExpireEvent();

				if (!bAffectLoading && bMsg)
				{
					if (GetSocket(ITEM_SOCKET_REMAIN_SEC) > 0)
					{
						pOwner->ChatPacket(CHAT_TYPE_INFO, "1516 %s", GetName());
						// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s was deactivated."), GetName());
					}
					else
					{
						pOwner->ChatPacket(CHAT_TYPE_INFO, "1576 %s", GetName());
						// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The limit time of %s expired."), GetName());
					}
				}
			}

			return true;
		}
		default:
		{
			if (bAdd && bMsg)
			{
				pOwner->ChatPacket(CHAT_TYPE_INFO, "1517 %s", GetName());
				// pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s doesn't have any bonus so you cannot activate it."), GetName());
			}

			return false;
		}
	}
}
#endif

bool CItem::CheckEnchantToItem(LPITEM item2)
{
	const BYTE enchantType = GetSubType();
	const DWORD itemIdx = GetVnum();

	if (enchantType == USE_CHANGE_ATTRIBUTE)
	{
		if ((item2->IsPendantSoul() && itemIdx != 91063) || (!item2->IsPendantSoul() && itemIdx == 91063))
			return false;
	}
	else if (enchantType == USE_ADD_ATTRIBUTE)
	{
		if ((item2->IsPendantSoul() && itemIdx != 91064) || (!item2->IsPendantSoul() && itemIdx == 91064))
			return false;
	}
	else if (enchantType == USE_ADD_ATTRIBUTE2)
	{
		if ((item2->IsPendantSoul() && itemIdx != 91065) || (!item2->IsPendantSoul() && itemIdx == 91065))
			return false;
	}
	return true;
}
bool CItem::IsPendantSoul()
{
	return (GetType() == ITEM_ARMOR && GetSubType() == ARMOR_PENDANT_SOUL) ? true : false;
}

bool CItem::IsSashSkin()
{
	return (GetType() == ITEM_COSTUME && GetSubType() == COSTUME_NEW_SASH) ? true : false;
}
