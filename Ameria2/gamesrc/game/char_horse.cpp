#include "stdafx.h"
#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "packet.h"
#include "guild.h"
#include "vector.h"
#include "questmanager.h"
#include "item.h"
#include "horsename_manager.h"
#include "locale_service.h"
#include "arena.h"

#include "../common/VnumHelper.h"
#if defined(WJ_COMBAT_ZONE)	
	#include "combat_zone.h"
#endif
bool CHARACTER::StartRiding()
{
#ifdef ENABLE_NEWSTUFF
	if (g_NoMountAtGuildWar && GetWarMap())
	{
		RemoveAffect(AFFECT_MOUNT);
		RemoveAffect(AFFECT_MOUNT_BONUS);
		if (IsRiding())
			StopRiding();
		return false;
	}
#endif
	if (IsDead() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, "106");
		return false;
	}

	if (IsPolymorphed())
	{
		ChatPacket(CHAT_TYPE_INFO, "107");
		return false;
	}
#if defined(WJ_COMBAT_ZONE)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
	{
		RemoveAffect(AFFECT_MOUNT);
		RemoveAffect(AFFECT_MOUNT_BONUS);
		
		if (IsRiding())
			StopRiding();
		
		ChatPacket(CHAT_TYPE_INFO, "108");	
		return false;
	}
#endif
	// ÅÎ½Ãµµ ÀÔÀº »óÅÂÀÇ ¸» Å¸±â ±ÝÁö
	LPITEM armor = GetWear(WEAR_BODY);

	if (armor && (armor->GetVnum() >= 11901 && armor->GetVnum() <= 11904))
	{
		ChatPacket(CHAT_TYPE_INFO, "109");
		return false;
	}

	// @warme005
	if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
		return false;


	DWORD dwMountVnum = m_chHorse ? m_chHorse->GetRaceNum() : GetMyHorseVnum();

	if (false == CHorseRider::StartRiding())
	{
		if (GetHorseLevel() <= 0)
			ChatPacket(CHAT_TYPE_INFO, "110");
		else if (GetHorseHealth() <= 0)
			ChatPacket(CHAT_TYPE_INFO, "111");
		else if (GetHorseStamina() <= 0)
			ChatPacket(CHAT_TYPE_INFO, "112");

		return false;
	}

	// ¼ÒÈ¯ÇÑ ¸» ¾ø¾Ö°í
	HorseSummon(false);

	MountVnum(dwMountVnum);

	if(test_server)
		sys_log(0, "Ride Horse : %s ", GetName());

	return true;
}

bool CHARACTER::StopRiding()
{
	if (CHorseRider::StopRiding())
	{
		quest::CQuestManager::instance().Unmount(GetPlayerID());

		if (!IsDead() && !IsStun())
		{
			DWORD dwOldVnum = GetMountVnum();
			MountVnum(0);

			// [NOTE] ¸»¿¡¼­ ³»¸± ¶© ÀÚ±â°¡ ÅÀ´ø°É ¼ÒÈ¯ÇÏµµ·Ï ¼öÁ¤
			HorseSummon(true, false, dwOldVnum);
		}
		else
		{
			m_dwMountVnum = 0;
			ComputePoints();
			UpdatePacket();
		}

		PointChange(POINT_ST, 0);
		PointChange(POINT_DX, 0);
		PointChange(POINT_HT, 0);
		PointChange(POINT_IQ, 0);

		return true;
	}

	return false;
}

EVENTFUNC(horse_dead_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "horse_dead_event> <Factor> Null pointer" );
		return 0;
	}

	// <Factor>
	LPCHARACTER ch = info->ch;
	if (ch == NULL) {
		return 0;
	}
	ch->HorseSummon(false);
	return 0;
}

void CHARACTER::SetRider(LPCHARACTER ch)
{
	if (m_chRider)
		m_chRider->ClearHorseInfo();

	m_chRider = ch;

	if (m_chRider)
		m_chRider->SendHorseInfo();
}

LPCHARACTER CHARACTER::GetRider() const
{
	return m_chRider;
}


void CHARACTER::HorseSummon(bool bSummon, bool bFromFar, DWORD dwVnum, const char* pPetName)
{
	if ( bSummon )
	{
		//NOTE : summonÇß´Âµ¥ ÀÌ¹Ì horse°¡ ÀÖÀ¸¸é ¾Æ¹«°Íµµ ¾ÈÇÑ´Ù.
		if( m_chHorse != NULL )
			return;

#ifdef __FAKE_PLAYER__
		if (!IsFakePlayer())
		{
			if (GetHorseLevel() <= 0)
				return;
		}
#else
		if (GetHorseLevel() <= 0)
			return;
#endif
		// ¹«¾ð°¡¸¦ Å¸°í ÀÖ´Ù¸é ½ÇÆÐ
		//if (IsRiding())
		//	return;
#if defined(WJ_COMBAT_ZONE)	
		if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		{
			ChatPacket(CHAT_TYPE_INFO, "108");	
			return;
		}
#endif
		sys_log(0, "HorseSummon : %s lv:%d bSummon:%d fromFar:%d", GetName(), GetLevel(), bSummon, bFromFar);

		long x = GetX();
		long y = GetY();

		if (GetHorseHealth() <= 0)
			bFromFar = false;

		if (bFromFar)
		{
			x += (number(0, 1) * 2 - 1) * number(2000, 2500);
			y += (number(0, 1) * 2 - 1) * number(2000, 2500);
		}
		else
		{
			x += number(-100, 100);
			y += number(-100, 100);
		}

		m_chHorse = CHARACTER_MANAGER::instance().SpawnMob(
				(0 == dwVnum) ? GetMyHorseVnum() : dwVnum,
				GetMapIndex(),
				x, y,
				GetZ(), false, (int)(GetRotation()+180), false);

		if (!m_chHorse)
		{
			ChatPacket(CHAT_TYPE_INFO, "113");
			return;
		}

		m_chHorse->m_stateIdle.Set(m_chHorse, &CHARACTER::BeginStateEmpty, &CHARACTER::StateHorse, &CHARACTER::EndStateEmpty);
		m_chHorse->m_stateMove.Set(m_chHorse, &CHARACTER::BeginStateEmpty, &CHARACTER::StateMove, &CHARACTER::EndStateEmpty);
		m_chHorse->m_stateBattle.Set(m_chHorse, &CHARACTER::BeginStateEmpty, &CHARACTER::StateHorse, &CHARACTER::EndStateEmpty);

#ifdef __FAKE_PLAYER__
		if (!IsFakePlayer())
		{
			if (GetHorseHealth() <= 0)
			{
				m_chHorse->SetPosition(POS_DEAD);

				char_event_info* info = AllocEventInfo<char_event_info>();
				info->ch = this;
				m_chHorse->m_pkDeadEvent = event_create(horse_dead_event, info, PASSES_PER_SEC(60));
			}
		}
#else
		if (GetHorseHealth() <= 0)
		{
			// Á×Àº°ÅÃ³·³ ÀÖ°Ô ÇÏ´Â Ã³¸®
			m_chHorse->SetPosition(POS_DEAD);

			// ÀÏÁ¤½Ã°£ÀÖ´Ù »ç¶óÁö°Ô ÇÏÀÚ.
			char_event_info* info = AllocEventInfo<char_event_info>();
			info->ch = this;
			m_chHorse->m_pkDeadEvent = event_create(horse_dead_event, info, PASSES_PER_SEC(60));
		}
#endif
		m_chHorse->SetLevel(GetHorseLevel());

		const char* pHorseName = CHorseNameManager::instance().GetHorseName(GetPlayerID());

		if ( pHorseName != NULL && strlen(pHorseName) != 0 )
		{
			m_chHorse->m_stName = pHorseName;
		}
		else
		{
			m_chHorse->m_stName = GetName();
			m_chHorse->m_stName += " Mount";
		}

		m_chHorse->SetName(m_chHorse->m_stName.c_str());
		
		if (!m_chHorse->Show(GetMapIndex(), x, y, GetZ()))
		{
			M2_DESTROY_CHARACTER(m_chHorse);
			sys_err("cannot show monster");
			m_chHorse = NULL;
			return;
		}

		if ((GetHorseHealth() <= 0))
		{
			TPacketGCDead pack;
			pack.header	= HEADER_GC_DEAD;
			pack.vid    = m_chHorse->GetVID();
			PacketAround(&pack, sizeof(pack));
		}

		m_chHorse->SetRider(this);
#ifdef ENABLE_BATTLE_PASS
		if (m_dwMountStartTime > 0)
		{
			DWORD dwMountPlayedTime = (get_dword_time() - m_dwMountStartTime);
			if (dwMountPlayedTime > 1000)
				CHARACTER_MANAGER::Instance().DoMission(this, MISSION_MOUNT_TIME, dwMountPlayedTime / 1000, 0);
			m_dwMountStartTime = 0;
		}
#endif
	}
	else
	{
		if (!m_chHorse)
			return;

		LPCHARACTER chHorse = m_chHorse;

		chHorse->SetRider(NULL); // m_chHorse assign to NULL

		// ¸»½ÃÃ¼°¡ ¼ÒÈ¯µÇ¾î ÀÖÀ»¶§ »óÁ¡ ¿­¸é bFromFar¸¦ false·Î ¸¸µé¾î ¸» ½ÃÃ¼¸¦ »ç¶óÁö°Ô ÇÑ´Ù.
		if ((GetHorseHealth() <= 0))
			bFromFar = false;

		if (!bFromFar)
		{
			M2_DESTROY_CHARACTER(chHorse);
		}
		else
		{
			// ¸Ö¾îÁö¸é¼­ »ç¶óÁö´Â Ã³¸® ÇÏ±â
			chHorse->SetNowWalking(false);
			float fx, fy;
			chHorse->SetRotation(GetDegreeFromPositionXY(chHorse->GetX(), chHorse->GetY(), GetX(), GetY())+180);
			GetDeltaByDegree(chHorse->GetRotation(), 3500, &fx, &fy);
			chHorse->Goto((long)(chHorse->GetX()+fx), (long) (chHorse->GetY()+fy));
			chHorse->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
		}

		m_chHorse = NULL;
#ifdef ENABLE_BATTLE_PASS
		if (IsRiding())
			m_dwMountStartTime = get_dword_time();
#endif

	}
}

DWORD CHARACTER::GetMyHorseVnum() const
{
	int delta = 0;

	if (GetGuild())
	{
		++delta;

		if (GetGuild()->GetMasterPID() == GetPlayerID())
			++delta;
	}

#ifdef ENABLE_MOUNT_SKIN
	LPITEM pkCostumeMountSkin = GetWear(WEAR_COSTUME_MOUNT_SKIN);
	if (pkCostumeMountSkin)
		return pkCostumeMountSkin->GetValue(0);
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	LPITEM pkCostumeMount = GetWear(WEAR_COSTUME_MOUNT);
	if (pkCostumeMount)
		return pkCostumeMount->GetValue(0);
#endif

	return c_aHorseStat[GetHorseLevel()].iNPCRace + delta;
}

void CHARACTER::HorseDie()
{
	CHorseRider::HorseDie();
	HorseSummon(false);
}

bool CHARACTER::ReviveHorse()
{
	if (CHorseRider::ReviveHorse())
	{
		HorseSummon(false);
		HorseSummon(true);
		return true;
	}
	return false;
}

void CHARACTER::ClearHorseInfo()
{
	if (!IsHorseRiding())
	{
		ChatPacket(CHAT_TYPE_COMMAND, "hide_horse_state");

		m_bSendHorseLevel = 0;
		m_bSendHorseHealthGrade = 0;
		m_bSendHorseStaminaGrade = 0;
	}

	m_chHorse = NULL;

}

void CHARACTER::SendHorseInfo()
{
	if (m_chHorse || IsHorseRiding())
	{
		int iHealthGrade;
		int iStaminaGrade;
		/*
		   HP
3: 70% < ~ <= 100%
2: 30% < ~ <= 70%
1:  0% < ~ <= 30%
0: »ç¸Á

STM

3: 71% < ~ <= 100%
2: 31% < ~ <= 70%
1: 10% < ~ <= 30%
0:	 ~ <= 10%
		 */
		if (GetHorseHealth() == 0)
			iHealthGrade = 0;
		else if (GetHorseHealth() * 10 <= GetHorseMaxHealth() * 3)
			iHealthGrade = 1;
		else if (GetHorseHealth() * 10 <= GetHorseMaxHealth() * 7)
			iHealthGrade = 2;
		else
			iHealthGrade = 3;

		if (GetHorseStamina() * 10 <= GetHorseMaxStamina())
			iStaminaGrade = 0;
		else if (GetHorseStamina() * 10 <= GetHorseMaxStamina() * 3)
			iStaminaGrade = 1;
		else if (GetHorseStamina() * 10 <= GetHorseMaxStamina() * 7)
			iStaminaGrade = 2;
		else
			iStaminaGrade = 3;

		if (m_bSendHorseLevel != GetHorseLevel() ||
				m_bSendHorseHealthGrade != iHealthGrade ||
				m_bSendHorseStaminaGrade != iStaminaGrade)
		{
			ChatPacket(CHAT_TYPE_COMMAND, "horse_state %d %d %d", GetHorseLevel(), iHealthGrade, iStaminaGrade);

			// FIX : Å¬¶óÀÌ¾ðÆ®¿¡ "¸» »óÅÂ ¹öÇÁ" ¾ÆÀÌÄÜÀ» Ç¥½ÃÇÏÁö ¾ÊÀ» ¸ñÀûÀ¸·Î ÇÔ¼ö ÃÊÀÔ¿¡ returnÇÔÀ¸·Î½á ¾Æ·¡ ÄÚµå¸¦ ¹«½ÃÇÑ´Ù¸é
			// ¸»À» ¹«ÇÑ´ë·Î ¼ÒÈ¯ÇÏ´Â ¹«½Ã¹«½ÃÇÑ ¹ö±×°¡ »ý±è.. Á¤È®ÇÑ ¿øÀÎÀº ÆÄ¾Ç ¾ÈÇØºÁ¼­ ¸ð¸§.
			m_bSendHorseLevel = GetHorseLevel();
			m_bSendHorseHealthGrade = iHealthGrade;
			m_bSendHorseStaminaGrade = iStaminaGrade;
		}
	}
}

bool CHARACTER::CanUseHorseSkill()
{
	if(IsRiding())
	{
		if (GetHorseGrade() == 3)
			return true;
		else
			return false;

		if(GetMountVnum())
		{
			if (GetMountVnum() >= 20209 && GetMountVnum() <= 20212)
				return true;

			//¶ó¸¶´Ü Èæ¸¶
			if (CMobVnumHelper::IsRamadanBlackHorse(GetMountVnum()))
				return true;
		}
		else
			return false;

	}

	return false;
}

void CHARACTER::SetHorseLevel(int iLevel)
{
	CHorseRider::SetHorseLevel(iLevel);
	SetSkillLevel(SKILL_HORSE, GetHorseLevel());
}

