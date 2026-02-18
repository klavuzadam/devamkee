
#include "stdafx.h"

#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "affect.h"
#include "packet.h"
#include "buffer_manager.h"
#include "desc_client.h"
#include "battle.h"
#include "guild.h"
#include "utils.h"
#include "locale_service.h"
#include "lua_incl.h"
#include "arena.h"
#include "horsename_manager.h"
#include "item.h"
#include "DragonSoul.h"
#include "../common/CommonDefines.h"
#if defined(WJ_COMBAT_ZONE)
	#include "combat_zone.h"
#endif
#ifdef USE_DAY_CHANGE_MGR
#include "day_change_mgr.hpp"
#endif

static bool IS_NO_SAVE_AFFECT(const DWORD type)
{
	if (type >= AFFECT_PREMIUM_START && type <= AFFECT_PREMIUM_END)
	{
		return true;
	}

	switch (type)
	{
		case AFFECT_WAR_FLAG:
		case AFFECT_REVIVE_INVISIBLE:
		case AFFECT_MOUNT_BONUS:
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_PREMIUM_AFFECT)
		case AFFECT_AUTO_USE:
#endif
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}

static bool IS_NO_CLEAR_ON_DEATH_AFFECT(const DWORD type)
{
	if (type >= 500 && type < 600)
	{
		return true;
	}

	switch (type)
	{
		case AFFECT_BLOCK_CHAT:
#ifdef __RENEWAL_BRAVE_CAPE__
		case AFFECT_BRAVE_CAPE:
#endif
		case AFFECT_THIEF:
		case AFFECT_EXP:
#ifdef USE_DROP_AFFECT_BONUSES
		case AFFECT_DROP_GREEN:
		case AFFECT_DROP_BLUE:
#endif
#ifdef USE_PREMIUM_AFFECT
		case AFFECT_PREMIUM_USER:
#ifdef __AUTOMATIC_HUNTING_SYSTEM__
		case AFFECT_AUTO_USE:
#endif
#endif
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}

#ifdef ENABLE_ADD_REALTIME_AFFECT
bool IsRealTimeAffect(DWORD affectIndex)
{
	switch(affectIndex)
	{
		case AFFECT_AUTO_PICK_UP:
		case AFFECT_NAME_PET:
		case AFFECT_NAME_MOUNT:
		case AFFECT_NAME_BUFFI:
#ifdef ENABLE_VOTE4BUFF
		case AFFECT_VOTE4BUFF:
#endif
#ifdef VOTE_BUFF_RENEWAL
		case AFFECT_VOTEFORBONUS:
#endif

			return true;
			break;
	}
	return false;
}
#endif

#ifdef USE_BOOST_POTIONS
bool IS_BOOST_POTION_AFFECT(const DWORD type)
{
	switch (type)
	{
		case AFFECT_BOOST_POTION1:
		case AFFECT_BOOST_POTION2:
		case AFFECT_BOOST_POTION3:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}
#endif

#ifdef USE_CRYSTAL_SYSTEM
bool IS_CRYSTAL_AFFECT(const DWORD type)
{
	switch (type)
	{
		case AFFECT_BOOST_CRYSTAL:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}
#endif

#ifdef USE_DROP_AFFECT_BONUSES
bool IS_DROP_AFFECT(const DWORD type)
{
	switch (type)
	{
		case AFFECT_DROP_GREEN:
		case AFFECT_DROP_BLUE:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}
#endif

#ifdef USE_PREMIUM_AFFECT
bool IS_PREMIUM_USER_AFFECT(const DWORD type)
{
	switch (type)
	{
		case AFFECT_PREMIUM_USER:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}
#endif

void SendAffectRemovePacket(LPDESC d, DWORD pid, DWORD type, BYTE point)
{
	if (d)
	{
		TPacketGCAffectRemove ptoc;
		ptoc.bHeader	= HEADER_GC_AFFECT_REMOVE;
		ptoc.dwType		= type;
		ptoc.bApplyOn	= point;

		d->Packet(&ptoc, sizeof(TPacketGCAffectRemove));
	}

	TPacketGDRemoveAffect ptod;
	ptod.dwPID		= pid;
	ptod.dwType		= type;
	ptod.bApplyOn	= point;

	db_clientdesc->DBPacket(HEADER_GD_REMOVE_AFFECT, 0, &ptod, sizeof(ptod));
}

void SendAffectAddPacket(LPDESC d, CAffect * pkAff)
{
	if (!d)
	{
		return;
	}

	TPacketGCAffectAdd p;
	p.bHeader = HEADER_GC_AFFECT_ADD;
	p.elem.dwType = pkAff->dwType;
	p.elem.bApplyOn = pkAff->bApplyOn;
	p.elem.lApplyValue = pkAff->lApplyValue;
	p.elem.dwFlag = pkAff->dwFlag;
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
	p.elem.dwFlag2 = pkAff->dwFlag2;
//#endif
#ifdef ENABLE_ADD_REALTIME_AFFECT
	p.elem.lDuration = IsRealTimeAffect(p.elem.dwType) ? pkAff->lDuration - time(0): pkAff->lDuration;
#else
	p.elem.lDuration = pkAff->lDuration;
#endif
	p.elem.lSPCost = pkAff->lSPCost;

	d->Packet(&p, sizeof(TPacketGCAffectAdd));
}
////////////////////////////////////////////////////////////////////
// Affect
CAffect* CHARACTER::FindAffect(DWORD dwType, BYTE bApply) const
{
    auto it = m_list_pkAffect.begin();
    while (it != m_list_pkAffect.end())
    {
        CAffect* pkAffect = *it++;
        if (pkAffect->dwType == dwType && (bApply == APPLY_NONE || bApply == pkAffect->bApplyOn))
            return pkAffect;
    }
    return NULL;
}

EVENTFUNC(affect_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info );

	if (!info)
	{
		sys_err( "affect_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER ch = info->ch;

	if (!ch) { // <Factor>
		return 0;
	}

	if (!ch->UpdateAffect())
		return 0;
	else
		return passes_per_sec; // 1ÃÊ
}

bool CHARACTER::UpdateAffect()
{
	// affect_event ¿¡¼­ Ã³¸®ÇÒ ÀÏÀº ¾Æ´ÏÁö¸¸, 1ÃÊÂ¥¸® ÀÌº¥Æ®¿¡¼­ Ã³¸®ÇÏ´Â °ÍÀÌ
	// ÀÌ°Í »ÓÀÌ¶ó ¿©±â¼­ ¹°¾à Ã³¸®¸¦ ÇÑ´Ù.
	if (GetPoint(POINT_HP_RECOVERY) > 0)
	{
		if (GetMaxHP() <= GetHP())
		{
			PointChange(POINT_HP_RECOVERY, -GetPoint(POINT_HP_RECOVERY));
		}
		else
		{
			int iVal = MIN(GetPoint(POINT_HP_RECOVERY), GetMaxHP() * 7 / 100);

			PointChange(POINT_HP, iVal);
			PointChange(POINT_HP_RECOVERY, -iVal);
		}
	}

	if (GetPoint(POINT_SP_RECOVERY) > 0)
	{
		if (GetMaxSP() <= GetSP())
			PointChange(POINT_SP_RECOVERY, -GetPoint(POINT_SP_RECOVERY));
		else
		{
			int iVal = MIN(GetPoint(POINT_SP_RECOVERY), GetMaxSP() * 7 / 100);

			PointChange(POINT_SP, iVal);
			PointChange(POINT_SP_RECOVERY, -iVal);
		}
	}

	if (GetPoint(POINT_HP_RECOVER_CONTINUE) > 0)
	{
		PointChange(POINT_HP, GetPoint(POINT_HP_RECOVER_CONTINUE));
	}

	if (GetPoint(POINT_SP_RECOVER_CONTINUE) > 0)
	{
		PointChange(POINT_SP, GetPoint(POINT_SP_RECOVER_CONTINUE));
	}

	AutoRecoveryItemProcess( AFFECT_AUTO_HP_RECOVERY );
	AutoRecoveryItemProcess( AFFECT_AUTO_SP_RECOVERY );

#ifdef __LEADERSHIP__BONUS__
	if (GetParty())
	{
		RemoveAffect(AFF_LEADERSHIP);
	}
#endif

	// ½ºÅ×¹Ì³ª È¸º¹
	if (GetMaxStamina() > GetStamina())
	{
		int iSec = (get_dword_time() - GetStopTime()) / 3000;
		if (iSec)
			PointChange(POINT_STAMINA, GetMaxStamina()/1);
	}


	// ProcessAffect´Â affect°¡ ¾øÀ¸¸é true¸¦ ¸®ÅÏÇÑ´Ù.
	if (ProcessAffect())
		if (GetPoint(POINT_HP_RECOVERY) == 0 && GetPoint(POINT_SP_RECOVERY) == 0 && GetStamina() == GetMaxStamina())
		{
			m_pkAffectEvent = NULL;
			return false;
		}

	return true;
}

void CHARACTER::StartAffectEvent()
{
	if (m_pkAffectEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();
	info->ch = this;
	m_pkAffectEvent = event_create(affect_event, info, passes_per_sec);
	sys_log(1, "StartAffectEvent %s %p %p", GetName(), this, get_pointer(m_pkAffectEvent));
}

void CHARACTER::ClearAffect(bool bSave, bool bSomeAffect)
{
	TAffectFlag afOld = m_afAffectFlag;
	WORD	wMovSpd = GetPoint(POINT_MOV_SPEED);
	WORD	wAttSpd = GetPoint(POINT_ATT_SPEED);

	itertype(m_list_pkAffect) it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		CAffect * pkAff = *it;

		if (bSave)
		{
#ifdef ENABLE_MULTI_FARM_BLOCK
			if(pkAff->dwType == AFFECT_MULTI_FARM_PREMIUM)
			{
				++it;
				continue;
			}
#endif

#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && !defined(USE_PREMIUM_AFFECT)
			if (pkAff->dwType == AFFECT_AUTO_USE
				 || pkAff->dwType == AFFECT_AUTO_HUNT)
			{
				++it;
				continue;
			}
#endif

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
			if(IsRealTimeAffect(pkAff->dwType))
			{
				++it;
				continue;
			}
#endif

			if ( IS_NO_CLEAR_ON_DEATH_AFFECT(pkAff->dwType) || IS_NO_SAVE_AFFECT(pkAff->dwType) )
			{
				++it;
				continue;
			}

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
			if (pkAff->dwType == AFFECT_SWITCHBOT)
			{
				++it;
				continue;
			}
#endif

#ifdef __LEADERSHIP__BONUS__
			if (pkAff->dwType == AFF_LEADERSHIP)
			{
				++it;
				continue;
			}
#endif

			if (bSomeAffect)
			{
				switch (pkAff->dwType)
				{
					case (SKILL_JEONGWI):
					case (SKILL_GEOMKYUNG):
					case (SKILL_CHUNKEON):
					case (SKILL_GWIGEOM):
					case (SKILL_TERROR):
					case (SKILL_JUMAGAP):
					case (SKILL_HOSIN):
					case (SKILL_REFLECT):
					case (SKILL_GICHEON):
					case (SKILL_KWAESOK):
					case (SKILL_JEUNGRYEOK):
					case (SKILL_JEOKRANG):
					case (SKILL_CHEONGRANG):
					{
						++it;
						continue;
					}
				}
			}

			if (IsPC())
			{
				SendAffectRemovePacket(GetDesc(), GetPlayerID(), pkAff->dwType, pkAff->bApplyOn);
#if defined(WJ_COMBAT_ZONE)
				CCombatZoneManager::instance().RemoveAffectMob(this, pkAff->dwType);
#endif
			}
		}

		ComputeAffect(pkAff, false);

		it = m_list_pkAffect.erase(it);
		CAffect::Release(pkAff);
	}

	if (afOld != m_afAffectFlag ||
			wMovSpd != GetPoint(POINT_MOV_SPEED) ||
			wAttSpd != GetPoint(POINT_ATT_SPEED))
		UpdatePacket();

	CheckMaximumPoints();

	if (m_list_pkAffect.empty())
		event_cancel(&m_pkAffectEvent);
}

int CHARACTER::ProcessAffect()
{
	bool	bDiff	= false;
	CAffect	*pkAff	= NULL;

	//
	// ÇÁ¸®¹Ì¾ö Ã³¸®
	//
	for (int i = 0; i <= PREMIUM_MAX_NUM; ++i)
	{
		int aff_idx = i + AFFECT_PREMIUM_START;

		pkAff = FindAffect(aff_idx);

		if (!pkAff)
			continue;

		int remain = GetPremiumRemainSeconds(i);

		if (remain < 0)
		{
			RemoveAffect(aff_idx);
			bDiff = true;
		}
		else
			pkAff->lDuration = remain + 1;
	}


#ifdef BATTLEPASS_WORLDARD
	pkAff = FindAffect(AFFECT_BATTLE_PASS);
	if (pkAff)
	{
		int remain = GetBattlePassEndTime();
		
		if (remain < 0)
		{
			RemoveAffect(AFFECT_BATTLE_PASS);
			m_dwBattlePassEndTime = 0;
			bDiff = true;
			this->ChatPacket(CHAT_TYPE_INFO,"Ya has finalizado el pase de batalla, debes esperar al siguiente mes.");

		}
		else
			pkAff->lDuration = remain + 1;
	}
#endif
	
	////////// HAIR_AFFECT
	pkAff = FindAffect(AFFECT_HAIR);
	if (pkAff)
	{
		// IF HAIR_LIMIT_TIME() < CURRENT_TIME()
		if ( this->GetQuestFlag("hair.limit_time") < get_global_time())
		{
			// SET HAIR NORMAL
			this->SetPart(PART_HAIR, 0);
			// REMOVE HAIR AFFECT
			RemoveAffect(AFFECT_HAIR);
		}
		else
		{
			// INCREASE AFFECT DURATION
			++(pkAff->lDuration);
		}
	}
	////////// HAIR_AFFECT
	//

	CHorseNameManager::instance().Validate(this);

	TAffectFlag afOld = m_afAffectFlag;
	long lMovSpd = GetPoint(POINT_MOV_SPEED);
	long lAttSpd = GetPoint(POINT_ATT_SPEED);

#if defined(USE_PREMIUM_AFFECT) && defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
	bool bRemoveAffectAutoUse = false;
#endif

	auto it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		pkAff = *it;

		bool bEnd = false;

		if (pkAff->dwType >= GUILD_SKILL_START && pkAff->dwType <= GUILD_SKILL_END)
		{
			if (!GetGuild() || !GetGuild()->UnderAnyWar())
				bEnd = true;
		}

		if (pkAff->lSPCost > 0
#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
		&& pkAff->dwType != AFFECT_SWITCHBOT
#endif
		
		)
		{
			if (GetSP() < pkAff->lSPCost)
				bEnd = true;
			else
				PointChange(POINT_SP, -pkAff->lSPCost);
		}

		// AFFECT_DURATION_BUG_FIX
		// ¹«ÇÑ È¿°ú ¾ÆÀÌÅÛµµ ½Ã°£À» ÁÙÀÎ´Ù.
		// ½Ã°£À» ¸Å¿ì Å©°Ô Àâ±â ¶§¹®¿¡ »ó°ü ¾øÀ» °ÍÀÌ¶ó »ý°¢µÊ.

#ifdef ENABLE_ADD_REALTIME_AFFECT
		if(IsRealTimeAffect(pkAff->dwType))
		{
			if(time(0) > pkAff->lDuration)
				bEnd = true;
		}
		else
#endif
		{
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
			if (pkAff->dwType == AFFECT_AUTO_USE)
			{
#ifdef USE_DAY_CHANGE_MGR
				const int iToday = CDayChangeMgr::Instance().GetTodayNum();
#else
				const int iToday = GetTodayNum();
#endif
				const int iLastDay = GetAutoHuntLastDay();

				if (iToday == iLastDay)
				{
					int iTimeUsed = GetAutoHuntUsedTime();
					iTimeUsed += 1;

					SetAutoHuntUsedTime(iTimeUsed < AUTO_HUNT_DAILY_TIME_LIMIT_SEC ? iTimeUsed : AUTO_HUNT_DAILY_TIME_LIMIT_SEC, true);
				}
			}
#endif

			if ( --pkAff->lDuration <= 0 )
			{
				bEnd = true;
			}
		}

		// END_AFFECT_DURATION_BUG_FIX

		if (bEnd)
		{
			it = m_list_pkAffect.erase(it);
			ComputeAffect(pkAff, false);
			bDiff = true;
			if (IsPC())
			{
				SendAffectRemovePacket(GetDesc(), GetPlayerID(), pkAff->dwType, pkAff->bApplyOn);
#if defined(WJ_COMBAT_ZONE)
				CCombatZoneManager::instance().RemoveAffectMob(this, pkAff->dwType);
#endif

#if defined(USE_PREMIUM_AFFECT) && defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
				if (IS_PREMIUM_USER_AFFECT(pkAff->dwType))
				{
					bRemoveAffectAutoUse = true;

					SetAutoHuntUsedTime(AUTO_HUNT_DAILY_TIME_LIMIT_SEC);

					for (uint8_t uiTypeIndex = AUTO_ONOFF_POTION; uiTypeIndex < AUTO_ONOFF_MAX; uiTypeIndex++)
					{
						SetAutoUseType(uiTypeIndex, false);
					}

					ChatPacket(CHAT_TYPE_COMMAND, "auto_onoff 0");
					ChatPacket(CHAT_TYPE_COMMAND, "autohunt_stat %d", AUTO_HUNT_DAILY_TIME_LIMIT_SEC);

					Save();
				}
#endif

#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
				if (pkAff->dwType == AFFECT_AUTO_USE)
				{
					ChatPacket(CHAT_TYPE_COMMAND, "auto_onoff 0");
					ChatPacket(CHAT_TYPE_COMMAND, "autohunt_stat %d", AUTO_HUNT_DAILY_TIME_LIMIT_SEC);
				}
#endif
			}

			CAffect::Release(pkAff);

			continue;
		}

		++it;
	}

	if (bDiff)
	{
		if (afOld != m_afAffectFlag ||
				lMovSpd != GetPoint(POINT_MOV_SPEED) ||
				lAttSpd != GetPoint(POINT_ATT_SPEED))
		{
			UpdatePacket();
		}

		CheckMaximumPoints();
	}

#if defined(USE_PREMIUM_AFFECT) && defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
	if (bRemoveAffectAutoUse)
	{
		RemoveAffect(AFFECT_AUTO_USE);
	}
#endif

	if (m_list_pkAffect.empty())
		return true;

	return false;
}

void CHARACTER::SaveAffect()
{
#ifdef __BL_BATTLE_ROYALE__
	if (IsBattleRoyale())
		return;
#endif

	TPacketGDAddAffect p;

	auto it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		CAffect * pkAff = *it++;

		if (IS_NO_SAVE_AFFECT(pkAff->dwType))
			continue;

		sys_log(1, "AFFECT_SAVE: %u %u %d %d", pkAff->dwType, pkAff->bApplyOn, pkAff->lApplyValue, pkAff->lDuration);

		p.dwPID				= GetPlayerID();
		p.elem.dwType		= pkAff->dwType;
		p.elem.bApplyOn		= pkAff->bApplyOn;
		p.elem.lApplyValue	= pkAff->lApplyValue;
		p.elem.dwFlag		= pkAff->dwFlag;
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
		p.elem.dwFlag2		= pkAff->dwFlag2;
//#endif
		p.elem.lDuration	= pkAff->lDuration;
		p.elem.lSPCost		= pkAff->lSPCost;

		db_clientdesc->DBPacket(HEADER_GD_ADD_AFFECT, 0, &p, sizeof(p));
	}
}

EVENTINFO(load_affect_login_event_info)
{
	DWORD pid;
	DWORD count;
	char* data;

	load_affect_login_event_info()
	: pid( 0 )
	, count( 0 )
	, data( 0 )
	{
	}
};

EVENTFUNC(load_affect_login_event)
{
	load_affect_login_event_info* info = dynamic_cast<load_affect_login_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "load_affect_login_event_info> <Factor> Null pointer" );
		return 0;
	}

	DWORD dwPID = info->pid;
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(dwPID);

	if (!ch)
	{
		M2_DELETE_ARRAY(info->data);
		return 0;
	}

	LPDESC d = ch->GetDesc();

	if (!d)
	{
		M2_DELETE_ARRAY(info->data);
		return 0;
	}

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
		M2_DELETE_ARRAY(info->data);
		return 0;
	}
	else if (d->IsPhase(PHASE_GAME))
	{
		sys_log(1, "Affect Load by Event");
		ch->LoadAffect(info->count, (TPacketAffectElement*)info->data);
		M2_DELETE_ARRAY(info->data);
		return 0;
	}
	else
	{
		sys_err("input_db.cpp:quest_login_event INVALID PHASE pid %d", ch->GetPlayerID());
		M2_DELETE_ARRAY(info->data);
		return 0;
	}
}

void CHARACTER::LoadAffect(DWORD dwCount, TPacketAffectElement * pElements)
{
#ifdef __BL_BATTLE_ROYALE__
	if (IsBattleRoyale())
	{
		m_bIsLoadedAffect = true;
		return;
	}
#endif

	m_bIsLoadedAffect = false;

	if (!GetDesc()->IsPhase(PHASE_GAME))
	{
		if (test_server)
			sys_log(0, "LOAD_AFFECT: Creating Event", GetName(), dwCount);

		load_affect_login_event_info* info = AllocEventInfo<load_affect_login_event_info>();

		info->pid = GetPlayerID();
		info->count = dwCount;
		info->data = M2_NEW char[sizeof(TPacketAffectElement) * dwCount];
		std::memcpy(info->data, pElements, sizeof(TPacketAffectElement) * dwCount);

		event_create(load_affect_login_event, info, PASSES_PER_SEC(1));

		return;
	}

	ClearAffect(true);

	if (test_server)
		sys_log(0, "LOAD_AFFECT: %s count %d", GetName(), dwCount);

	TAffectFlag afOld = m_afAffectFlag;

	long lMovSpd = GetPoint(POINT_MOV_SPEED);
	long lAttSpd = GetPoint(POINT_ATT_SPEED);

	for (DWORD i = 0; i < dwCount; ++i, ++pElements)
	{
		// ¹«¿µÁøÀº ·ÎµåÇÏÁö¾Ê´Â´Ù.
		if (pElements->dwType == SKILL_MUYEONG)
			continue;
#ifdef ENABLE_YOHARA_SYSTEM
		if (pElements->dwType == SKILL_CHEONUN)
			continue;
#endif

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
		if (pElements->dwType == AFFECT_SWITCHBOT)
		{
			pElements->lDuration = pElements->lApplyValue - get_global_time();

			if (pElements->lDuration < 0 || get_global_time() > pElements->lApplyValue)
				continue;
		}
#endif

		if (AFFECT_AUTO_HP_RECOVERY == pElements->dwType || AFFECT_AUTO_SP_RECOVERY == pElements->dwType)
		{
			LPITEM item = FindItemByID(pElements->dwFlag);
			if (!item)
			{
				continue;
			}

			item->Lock(true);
		}
#ifdef USE_BOOST_POTIONS
		else if (IS_BOOST_POTION_AFFECT(pElements->dwType))
		{
			LPITEM item = FindItemByID(pElements->dwFlag);
			if (!item)
			{
				continue;
			}

			item->Lock(true);
//			item->SetSocket(ITEM_BOOST_POTION_STATUS, 1);
			item->StartTimerBasedOnWearExpireEvent();
		}
#endif
#ifdef USE_CRYSTAL_SYSTEM
		else if (IS_CRYSTAL_AFFECT(pElements->dwType))
		{
			LPITEM item = FindItemByID(pElements->dwFlag);
			if (!item)
			{
				continue;
			}

			if (!item->ModifyBoostCrystal(true, true, false))
			{
				item->Lock(false);
				item->SetSocket(CRYSTAL_STATUS_SOCKET_SLOT, 0);
				item->StopTimerBasedOnWearExpireEvent();
			}
		}
#endif

		if (pElements->bApplyOn >= POINT_MAX_NUM)
		{
			sys_err("invalid affect data %s ApplyOn %u ApplyValue %d",
					GetName(), pElements->bApplyOn, pElements->lApplyValue);
			continue;
		}

		if (test_server)
		{
			sys_log(0, "Load Affect : Affect %s %d %d", GetName(), pElements->dwType, pElements->bApplyOn );
		}

		CAffect* pkAff = CAffect::Acquire();
		m_list_pkAffect.push_back(pkAff);

		pkAff->dwType		= pElements->dwType;
		pkAff->bApplyOn		= pElements->bApplyOn;
		pkAff->lApplyValue	= pElements->lApplyValue;
		pkAff->dwFlag		= pElements->dwFlag;
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
		pkAff->dwFlag2		= pElements->dwFlag2;
//#endif
		pkAff->lDuration	= pElements->lDuration;
		pkAff->lSPCost		= pElements->lSPCost;

		SendAffectAddPacket(GetDesc(), pkAff);

		ComputeAffect(pkAff, true);
	}

	if ( CArenaManager::instance().IsArenaMap(GetMapIndex()) == true )
	{
		RemoveGoodAffect();
	}
#ifdef __DOJANG_SRC_FUNCTIONS__
	if (GetMapIndex() == DOJANG_MAPINDEX)
	{
		RemoveAffect(SKILL_JEONGWI);
		RemoveAffect(SKILL_GEOMKYUNG);
		RemoveAffect(SKILL_CHUNKEON);
		RemoveAffect(SKILL_EUNHYUNG);
		RemoveAffect(SKILL_GYEONGGONG);
		RemoveAffect(SKILL_GWIGEOM);
		RemoveAffect(SKILL_TERROR);
		RemoveAffect(SKILL_JUMAGAP);
		RemoveAffect(SKILL_MANASHILED);
		RemoveAffect(SKILL_HOSIN);
		RemoveAffect(SKILL_REFLECT);
		RemoveAffect(SKILL_KWAESOK);
		RemoveAffect(SKILL_JEUNGRYEOK);
		RemoveAffect(SKILL_GICHEON);
	}
#endif
	if (afOld != m_afAffectFlag || lMovSpd != GetPoint(POINT_MOV_SPEED) || lAttSpd != GetPoint(POINT_ATT_SPEED))
	{
		UpdatePacket();
	}

	StartAffectEvent();

	m_bIsLoadedAffect = true;

	// ¿ëÈ¥¼® ¼ÂÆÃ ·Îµå ¹× ÃÊ±âÈ­
	DragonSoul_Initialize();

#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
	AutoHunt_Initialize();
#endif

	// @fixme118 (regain affect hp/mp)
	if (!IsDead())
	{
		PointChange(POINT_HP, GetMaxHP() - GetHP());
		PointChange(POINT_SP, GetMaxSP() - GetSP());
	}
}

bool CHARACTER::AddAffect(DWORD dwType, BYTE bApplyOn, long lApplyValue, DWORD dwFlag, long lDuration, long lSPCost, bool bOverride, bool IsCube
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
, DWORD dwFlag2/* = 0*/
//#endif
)
{
	// CHAT_BLOCK
	if (dwType == AFFECT_BLOCK_CHAT && lDuration > 1)
	{
		ChatPacket(CHAT_TYPE_INFO, "96");
	}
	// END_OF_CHAT_BLOCK

	if (lDuration == 0)
	{
		sys_err("Character::AddAffect lDuration == 0 type %d", lDuration, dwType);
		lDuration = 1;
	}
#if defined(ENABLE_IGNORE_LOWER_BUFFS)
	switch (dwType)
	{
		case SKILL_HOSIN:
		case SKILL_REFLECT:
		case SKILL_GICHEON:
		case SKILL_JEONGEOP:
		case SKILL_KWAESOK:
		case SKILL_JEUNGRYEOK:
#if defined(ENABLE_WOLFMAN_CHARACTER)
		case SKILL_CHEONGRANG:
#endif
		{
			const CAffect * pkAffect = FindAffect(dwType);
			if (!pkAffect)
				break;
				
			if (lApplyValue < pkAffect->lApplyValue)
			{
				ChatPacket(CHAT_TYPE_INFO, "<AddAffect> has blocked receiving skill (%s) because power is (%ld%%) more small then current one (%ld%%).", CSkillManager::instance().Get(dwType)->szName, lApplyValue, pkAffect->lApplyValue);
				return false;
			}
		}
		break;
		default:
			break;
	}
#endif

	CAffect* pkAff = NULL;

	if (IsCube)
	{
		pkAff = FindAffect(dwType, bApplyOn);
	}
	else
	{
		pkAff = FindAffect(dwType);
	}

	if (dwFlag == AFF_STUN)
	{
		if (m_posDest.x != GetX() || m_posDest.y != GetY())
		{
			m_posDest.x = m_posStart.x = GetX();
			m_posDest.y = m_posStart.y = GetY();
			battle_end(this);

			SyncPacket();
		}
	}

	// ÀÌ¹Ì ÀÖ´Â È¿°ú¸¦ µ¤¾î ¾²´Â Ã³¸®
	if (pkAff && bOverride)
	{
		ComputeAffect(pkAff, false); // ÀÏ´Ü È¿°ú¸¦ »èÁ¦ÇÏ°í

		if (GetDesc())
			SendAffectRemovePacket(GetDesc(), GetPlayerID(), pkAff->dwType, pkAff->bApplyOn);
	}
	else
	{
		//
		// »õ ¿¡Æå¸¦ Ãß°¡
		//
		// NOTE: µû¶ó¼­ °°Àº type À¸·Îµµ ¿©·¯ ¿¡ÆåÆ®¸¦ ºÙÀ» ¼ö ÀÖ´Ù.
		//
		pkAff = CAffect::Acquire();
		m_list_pkAffect.push_back(pkAff);
	}

	sys_log(1, "AddAffect %s type %d apply %d %d flag %u duration %d", GetName(), dwType, bApplyOn, lApplyValue, dwFlag, lDuration);
	sys_log(0, "AddAffect %s type %d apply %d %d flag %u duration %d", GetName(), dwType, bApplyOn, lApplyValue, dwFlag, lDuration);

	pkAff->dwType		= dwType;
	pkAff->bApplyOn		= bApplyOn;
	pkAff->lApplyValue	= lApplyValue;
	pkAff->dwFlag		= dwFlag;
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
	pkAff->dwFlag2		= dwFlag2;
//#endif
#ifdef ENABLE_ADD_REALTIME_AFFECT
	pkAff->lDuration	= IsRealTimeAffect(dwType) ? lDuration + time(0) : lDuration;
#else
	pkAff->lDuration	= lDuration;
#endif
	pkAff->lSPCost		= lSPCost;

	WORD wMovSpd = GetPoint(POINT_MOV_SPEED);
	WORD wAttSpd = GetPoint(POINT_ATT_SPEED);

	ComputeAffect(pkAff, true);

	if (pkAff->dwFlag || wMovSpd != GetPoint(POINT_MOV_SPEED) || wAttSpd != GetPoint(POINT_ATT_SPEED))
	{
		UpdatePacket();
	}

	StartAffectEvent();

	if (IsPC())
	{
		SendAffectAddPacket(GetDesc(), pkAff);

#ifdef __BL_BATTLE_ROYALE__
		if (IsBattleRoyale() && dwType != AFFECT_BATTLE_ROYALE_CROWN)
			return true;
#endif

		if (IS_NO_SAVE_AFFECT(pkAff->dwType))
			return true;

		TPacketGDAddAffect p;
		p.dwPID			= GetPlayerID();
		p.elem.dwType		= pkAff->dwType;
		p.elem.bApplyOn		= pkAff->bApplyOn;
		p.elem.lApplyValue	= pkAff->lApplyValue;
		p.elem.dwFlag		= pkAff->dwFlag;
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
		p.elem.dwFlag2		= pkAff->dwFlag2;
//#endif
		p.elem.lDuration	= pkAff->lDuration;
		p.elem.lSPCost		= pkAff->lSPCost;

		db_clientdesc->DBPacket(HEADER_GD_ADD_AFFECT, 0, &p, sizeof(p));
	}

	return true;
}

void CHARACTER::RefreshAffect()
{
	auto it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		CAffect* pkAff = *it++;
		ComputeAffect(pkAff, true);
	}
}

void CHARACTER::ComputeAffect(CAffect * pkAff, bool bAdd)
{
	if (bAdd && pkAff->dwType >= GUILD_SKILL_START && pkAff->dwType <= GUILD_SKILL_END)
	{
		if (!GetGuild())
			return;

		if (!GetGuild()->UnderAnyWar())
			return;
	}

	if (pkAff->dwFlag)
	{
#ifdef ENABLE_PERFECT_BUFF
		if(pkAff->dwFlag == AFF_HOSIN && pkAff->lApplyValue > 68) // 50 OR 200, OR 300, 30, ETC
		{
			pkAff->dwFlag = AFF_HOSIN_PERFECT;
		}
		else if(pkAff->dwFlag == AFF_BOHO && pkAff->lApplyValue > 68)
		{
			pkAff->dwFlag = AFF_BOHO_PERFECT;
		}
		else if(pkAff->dwFlag == AFF_KWAESOK && pkAff->lApplyValue > 68)
		{
			pkAff->dwFlag = AFF_KWAESOK_PERFECT;
		}
		else if(pkAff->dwFlag == AFF_GICHEON && pkAff->lApplyValue > 68)
		{
			pkAff->dwFlag = AFF_GICHEON_PERFECT;
		}
		else if(pkAff->dwFlag == AFF_JEUNGRYEOK && pkAff->lApplyValue > 68)
		{
			pkAff->dwFlag = AFF_JEUNGRYEOK_PERFECT;
		}
#endif

		if (!bAdd)
		{
			m_afAffectFlag.Reset(pkAff->dwFlag);
		}
		else
		{
			m_afAffectFlag.Set(pkAff->dwFlag);
		}
	}

#ifdef __LEADERSHIP__BONUS__
	if (pkAff->dwType == AFF_LEADERSHIP)
	{
		if (!bAdd)
		{
			PointChange(POINT_PARTY_ATTACKER_BONUS, -GetPoint(POINT_PARTY_ATTACKER_BONUS));
			PointChange(POINT_PARTY_TANKER_BONUS, -GetPoint(POINT_PARTY_TANKER_BONUS));
			PointChange(POINT_PARTY_BUFFER_BONUS, -GetPoint(POINT_PARTY_BUFFER_BONUS));
			PointChange(POINT_PARTY_SKILL_MASTER_BONUS, -GetPoint(POINT_PARTY_SKILL_MASTER_BONUS));
			PointChange(POINT_PARTY_DEFENDER_BONUS, -GetPoint(POINT_PARTY_DEFENDER_BONUS));
			PointChange(POINT_PARTY_HASTE_BONUS, -GetPoint(POINT_PARTY_HASTE_BONUS));

			ComputeBattlePoints();
			
			SetPoint(POINT_PARTY_ATTACKER_BONUS, 0);
			SetPoint(POINT_PARTY_TANKER_BONUS, 0);
			SetPoint(POINT_PARTY_BUFFER_BONUS, 0);
			SetPoint(POINT_PARTY_SKILL_MASTER_BONUS, 0);
			SetPoint(POINT_PARTY_DEFENDER_BONUS, 0);
			SetPoint(POINT_PARTY_HASTE_BONUS, 0);
		
			ComputePoints();
		}
		else
		{
			if (GetPoint(pkAff->bApplyOn) <= 0)
				PointChange(pkAff->bApplyOn, pkAff->lApplyValue);
		}
		
		return;
	}
#endif

	if (bAdd)
		PointChange(pkAff->bApplyOn, pkAff->lApplyValue);
	else
		PointChange(pkAff->bApplyOn, -pkAff->lApplyValue);
		
	if (pkAff->dwType == SKILL_MUYEONG)
	{
		if (bAdd)
			StartMuyeongEvent();
		else
			StopMuyeongEvent();
	}

#ifdef ENABLE_YOHARA_SYSTEM
    	if (pkAff->dwType == SKILL_CHEONUN)
    	{
        	if (bAdd)
            		StartCheonunEvent(pkAff->lApplyValue);
        	else
            		StopCheonunEvent();
    	}
#endif

}

bool CHARACTER::RemoveAffect(CAffect* pkAff)
{
	if (!pkAff)
	{
		return false;
	}

	// AFFECT_BUF_FIX
	m_list_pkAffect.remove(pkAff);
	// END_OF_AFFECT_BUF_FIX

	ComputeAffect(pkAff, false);

	if (AFFECT_REVIVE_INVISIBLE != pkAff->dwType)
		ComputePoints();
	else  // @fixme110
		UpdatePacket();

	CheckMaximumPoints();

	if (test_server)
	{
		sys_log(0, "AFFECT_REMOVE: %s (flag %u apply: %u)", GetName(), pkAff->dwFlag, pkAff->bApplyOn);
	}

	if (IsPC())
	{
		SendAffectRemovePacket(GetDesc(), GetPlayerID(), pkAff->dwType, pkAff->bApplyOn);

#if defined(USE_PREMIUM_AFFECT) && defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
		if (IS_PREMIUM_USER_AFFECT(pkAff->dwType))
		{
			RemoveAffect(AFFECT_AUTO_USE);

			SetAutoHuntUsedTime(AUTO_HUNT_DAILY_TIME_LIMIT_SEC);

			for (uint8_t uiTypeIndex = AUTO_ONOFF_POTION; uiTypeIndex < AUTO_ONOFF_MAX; uiTypeIndex++)
			{
				SetAutoUseType(uiTypeIndex, false);
			}

			ChatPacket(CHAT_TYPE_COMMAND, "auto_onoff 0");
			ChatPacket(CHAT_TYPE_COMMAND, "autohunt_stat %d", AUTO_HUNT_DAILY_TIME_LIMIT_SEC);

			Save();
		}
#endif
	}

	CAffect::Release(pkAff);
	return true;
}

bool CHARACTER::RemoveAffect(DWORD dwType)
{
	// CHAT_BLOCK
	if (dwType == AFFECT_BLOCK_CHAT)
	{
		ChatPacket(CHAT_TYPE_INFO, "97");
	}
	// END_OF_CHAT_BLOCK

	bool flag = false;

	CAffect * pkAff;

	while ((pkAff = FindAffect(dwType)))
	{
		RemoveAffect(pkAff);
		flag = true;
	}

	return flag;
}

bool CHARACTER::IsAffectFlag(DWORD dwAff) const
{
	return m_afAffectFlag.IsSet(dwAff);
}

void CHARACTER::RemoveGoodAffect()
{
	RemoveAffect(AFFECT_MOV_SPEED);
	RemoveAffect(AFFECT_ATT_SPEED);
	RemoveAffect(AFFECT_STR);
	RemoveAffect(AFFECT_DEX);
	RemoveAffect(AFFECT_INT);
	RemoveAffect(AFFECT_CON);
	RemoveAffect(AFFECT_CHINA_FIREWORK);

	RemoveAffect(SKILL_JEONGWI);
	RemoveAffect(SKILL_GEOMKYUNG);
	RemoveAffect(SKILL_CHUNKEON);
	RemoveAffect(SKILL_EUNHYUNG);
	RemoveAffect(SKILL_GYEONGGONG);
	RemoveAffect(SKILL_GWIGEOM);
	RemoveAffect(SKILL_TERROR);
	RemoveAffect(SKILL_JUMAGAP);
	RemoveAffect(SKILL_MANASHILED);
	RemoveAffect(SKILL_HOSIN);
	RemoveAffect(SKILL_REFLECT);
	RemoveAffect(SKILL_KWAESOK);
	RemoveAffect(SKILL_JEUNGRYEOK);
	RemoveAffect(SKILL_GICHEON);
#ifdef ENABLE_WOLFMAN_CHARACTER
	// ¼öÀÎÁ·(WOLFMEN) ¹öÇÁ Ãß°¡
	RemoveAffect(SKILL_JEOKRANG);
	RemoveAffect(SKILL_CHEONGRANG);
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	RemoveAffect(SKILL_CHEONUN);
#endif

}

bool CHARACTER::IsGoodAffect(BYTE bAffectType) const
{
	switch (bAffectType)
	{
		case (AFFECT_MOV_SPEED):
		case (AFFECT_ATT_SPEED):
		case (AFFECT_STR):
		case (AFFECT_DEX):
		case (AFFECT_INT):
		case (AFFECT_CON):
		case (AFFECT_CHINA_FIREWORK):

		case (SKILL_JEONGWI):
		case (SKILL_GEOMKYUNG):
		case (SKILL_CHUNKEON):
		case (SKILL_EUNHYUNG):
		case (SKILL_GYEONGGONG):
		case (SKILL_GWIGEOM):
		case (SKILL_TERROR):
		case (SKILL_JUMAGAP):
		case (SKILL_MANASHILED):
		case (SKILL_HOSIN):
		case (SKILL_REFLECT):
		case (SKILL_KWAESOK):
		case (SKILL_JEUNGRYEOK):
		case (SKILL_GICHEON):
#ifdef ENABLE_WOLFMAN_CHARACTER
		// ¼öÀÎÁ·(WOLFMEN) ¹öÇÁ Ãß°¡
		case (SKILL_JEOKRANG):
		case (SKILL_CHEONGRANG):
#endif
#ifdef ENABLE_YOHARA_SYSTEM
		case (SKILL_CHEONUN):
#endif

			return true;
	}
	return false;
}

void CHARACTER::RemoveBadAffect()
{
	sys_log(0, "RemoveBadAffect %s", GetName());
	// µ¶
	RemovePoison();
#ifdef ENABLE_WOLFMAN_CHARACTER
	RemoveBleeding();
#endif
	RemoveFire();

	// ½ºÅÏ           : Value%·Î »ó´ë¹æÀ» 5ÃÊ°£ ¸Ó¸® À§¿¡ º°ÀÌ µ¹¾Æ°£´Ù. (¶§¸®¸é 1/2 È®·ü·Î Ç®¸²)               AFF_STUN
	RemoveAffect(AFFECT_STUN);

	// ½½·Î¿ì         : Value%·Î »ó´ë¹æÀÇ °ø¼Ó/ÀÌ¼Ó ¸ðµÎ ´À·ÁÁø´Ù. ¼ö·Ãµµ¿¡ µû¶ó ´Þ¶óÁü ±â¼ú·Î »ç¿ë ÇÑ °æ¿ì¿¡   AFF_SLOW
	RemoveAffect(AFFECT_SLOW);

	// Åõ¼Ó¸¶·É
	RemoveAffect(SKILL_TUSOK);

	// ÀúÁÖ
	//RemoveAffect(SKILL_CURSE);

	// ÆÄ¹ý¼ú
	//RemoveAffect(SKILL_PABUP);

	// ±âÀý           : Value%·Î »ó´ë¹æÀ» ±âÀý½ÃÅ²´Ù. 2ÃÊ                                                       AFF_FAINT
	//RemoveAffect(AFFECT_FAINT);

	// ´Ù¸®¹­ÀÓ       : Value%·Î »ó´ë¹æÀÇ ÀÌµ¿¼Óµµ¸¦ ¶³¾îÆ®¸°´Ù. 5ÃÊ°£ -40                                      AFF_WEB
	//RemoveAffect(AFFECT_WEB);

	// Àáµé±â         : Value%·Î »ó´ë¹æÀ» 10ÃÊ°£ ÀáÀç¿î´Ù. (¶§¸®¸é Ç®¸²)                                        AFF_SLEEP
	//RemoveAffect(AFFECT_SLEEP);

	// ÀúÁÖ           : Value%·Î »ó´ë¹æÀÇ °øµî/¹æµî ¸ðµÎ ¶³¾îÆ®¸°´Ù. ¼ö·Ãµµ¿¡ µû¶ó ´Þ¶óÁü ±â¼ú·Î »ç¿ë ÇÑ °æ¿ì¿¡ AFF_CURSE
	//RemoveAffect(AFFECT_CURSE);

	// ¸¶ºñ           : Value%·Î »ó´ë¹æÀ» 4ÃÊ°£ ¸¶ºñ½ÃÅ²´Ù.                                                     AFF_PARA
	//RemoveAffect(AFFECT_PARALYZE);

	// ºÎµ¿¹ÚºÎ       : ¹«´ç ±â¼ú
	//RemoveAffect(SKILL_BUDONG);
}
