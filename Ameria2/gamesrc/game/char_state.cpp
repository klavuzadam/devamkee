#include "stdafx.h"
#include "config.h"
#include "utils.h"
#include "vector.h"
#include "char.h"
#include "battle.h"
#include "char_manager.h"
#include "packet.h"
#include "motion.h"
#include "party.h"
#include "affect.h"
#include "buffer_manager.h"
#include "questmanager.h"
#include "p2p.h"
#include "item_manager.h"
#include "mob_manager.h"
#include "exchange.h"
#include "sectree_manager.h"
#include "xmas_event.h"
#include "guild_manager.h"
#include "war_map.h"
#include "locale_service.h"
#include "BlueDragon.h"
#ifdef ENABLE_MOUNT_SYSTEM
#include "MountSystem.h"
#endif
#include "../common/VnumHelper.h"
#ifdef ENABLE_ZODIAC_MISSION
	#include "dungeon.h"
#endif

extern LPCHARACTER FindVictim(LPCHARACTER pkChr, int iMaxDistance);
#ifdef ENABLE_DEFENSAWE_SHIP
extern LPCHARACTER FindVictimHydra(LPCHARACTER pkChr, int iMaxDistance);
#endif
namespace
{
	class FuncFindChrForFlag
	{
		public:
			FuncFindChrForFlag(LPCHARACTER pkChr) :
				m_pkChr(pkChr), m_pkChrFind(NULL), m_iMinDistance(INT_MAX)
				{
				}

			void operator () (LPENTITY ent)
			{
				if (!ent->IsType(ENTITY_CHARACTER))
					return;

				if (ent->IsObserverMode())
					return;

				LPCHARACTER pkChr = (LPCHARACTER) ent;

				if (!pkChr->IsPC())
					return;

				if (!pkChr->GetGuild())
					return;

				if (pkChr->IsDead())
					return;

				int iDist = DISTANCE_APPROX(pkChr->GetX()-m_pkChr->GetX(), pkChr->GetY()-m_pkChr->GetY());

				if (iDist <= 500 && m_iMinDistance > iDist &&
						!pkChr->IsAffectFlag(AFF_WAR_FLAG1) &&
						!pkChr->IsAffectFlag(AFF_WAR_FLAG2) &&
						!pkChr->IsAffectFlag(AFF_WAR_FLAG3))
				{
					// ¿ì¸®Æí ±ê¹ßÀÏ °æ¿ì
					if ((DWORD) m_pkChr->GetPoint(POINT_STAT) == pkChr->GetGuild()->GetID())
					{
						CWarMap * pMap = pkChr->GetWarMap();
						BYTE idx;

						if (!pMap || !pMap->GetTeamIndex(pkChr->GetGuild()->GetID(), idx))
							return;

						// ¿ì¸®Æí ±âÁö¿¡ ±ê¹ßÀÌ ¾øÀ» ¶§¸¸ ±ê¹ßÀ» »Ì´Â´Ù. ¾È±×·¯¸é ±âÁö¿¡ ÀÖ´Â ±ê¹ßÀ»
						// °¡¸¸È÷ µÎ°í ½ÍÀºµ¥µµ »ÌÈú¼ö°¡ ÀÖÀ¸¹Ç·Î..
						if (!pMap->IsFlagOnBase(idx))
						{
							m_pkChrFind = pkChr;
							m_iMinDistance = iDist;
						}
					}
					else
					{
						// »ó´ëÆí ±ê¹ßÀÎ °æ¿ì ¹«Á¶°Ç »Ì´Â´Ù.
						m_pkChrFind = pkChr;
						m_iMinDistance = iDist;
					}
				}
			}

			LPCHARACTER	m_pkChr;
			LPCHARACTER m_pkChrFind;
			int		m_iMinDistance;
	};

	class FuncFindChrForFlagBase
	{
		public:
			FuncFindChrForFlagBase(LPCHARACTER pkChr) : m_pkChr(pkChr)
			{
			}

			void operator () (LPENTITY ent)
			{
				if (!ent->IsType(ENTITY_CHARACTER))
					return;

				if (ent->IsObserverMode())
					return;

				LPCHARACTER pkChr = (LPCHARACTER) ent;

				if (!pkChr->IsPC())
					return;

				CGuild * pkGuild = pkChr->GetGuild();

				if (!pkGuild)
					return;

				int iDist = DISTANCE_APPROX(pkChr->GetX()-m_pkChr->GetX(), pkChr->GetY()-m_pkChr->GetY());

				if (iDist <= 500 &&
						(pkChr->IsAffectFlag(AFF_WAR_FLAG1) ||
						 pkChr->IsAffectFlag(AFF_WAR_FLAG2) ||
						 pkChr->IsAffectFlag(AFF_WAR_FLAG3)))
				{
					CAffect * pkAff = pkChr->FindAffect(AFFECT_WAR_FLAG);

					sys_log(0, "FlagBase %s dist %d aff %p flag gid %d chr gid %u",
							pkChr->GetName(), iDist, pkAff, m_pkChr->GetPoint(POINT_STAT),
							pkChr->GetGuild()->GetID());

					if (pkAff)
					{
						if ((DWORD) m_pkChr->GetPoint(POINT_STAT) == pkGuild->GetID() &&
								m_pkChr->GetPoint(POINT_STAT) != pkAff->lApplyValue)
						{
							CWarMap * pMap = pkChr->GetWarMap();
							BYTE idx;

							if (!pMap || !pMap->GetTeamIndex(pkGuild->GetID(), idx))
								return;

							//if (pMap->IsFlagOnBase(idx))
							{
								BYTE idx_opp = idx == 0 ? 1 : 0;

								SendGuildWarScore(m_pkChr->GetPoint(POINT_STAT), pkAff->lApplyValue, 1);
								//SendGuildWarScore(pkAff->lApplyValue, m_pkChr->GetPoint(POINT_STAT), -1);

								pMap->ResetFlag();
								//pMap->AddFlag(idx_opp);
								//pkChr->RemoveAffect(AFFECT_WAR_FLAG);

								char buf[256];
								snprintf(buf, sizeof(buf), "374 %s %s", pMap->GetGuild(idx)->GetName(), pMap->GetGuild(idx_opp)->GetName());
								pMap->Notice(buf);
							}
						}
					}
				}
			}

			LPCHARACTER m_pkChr;
	};

	class FuncFindGuardVictim
	{
		public:
			FuncFindGuardVictim(LPCHARACTER pkChr, int iMaxDistance) :
				m_pkChr(pkChr),
			m_iMinDistance(INT_MAX),
			m_iMaxDistance(iMaxDistance),
			m_lx(pkChr->GetX()),
			m_ly(pkChr->GetY()),
			m_pkChrVictim(NULL)
			{
			};

			void operator () (LPENTITY ent)
			{
				if (!ent->IsType(ENTITY_CHARACTER))
					return;

				LPCHARACTER pkChr = (LPCHARACTER) ent;

				// ÀÏ´Ü PC °ø°Ý¾ÈÇÔ
				if (pkChr->IsPC())
					return;


				if (pkChr->IsNPC() && !pkChr->IsMonster())
					return;

				if (pkChr->IsDead())
					return;

				if (pkChr->IsAffectFlag(AFF_EUNHYUNG) ||
						pkChr->IsAffectFlag(AFF_INVISIBILITY) ||
						pkChr->IsAffectFlag(AFF_REVIVE_INVISIBLE))
					return;

				// ¿Ö±¸´Â ÆÐ½º
				if (pkChr->GetRaceNum() == 5001)
					return;

				int iDistance = DISTANCE_APPROX(m_lx - pkChr->GetX(), m_ly - pkChr->GetY());

				if (iDistance < m_iMinDistance && iDistance <= m_iMaxDistance)
				{
					m_pkChrVictim = pkChr;
					m_iMinDistance = iDistance;
				}
			}

			LPCHARACTER GetVictim()
			{
				return (m_pkChrVictim);
			}

		private:
			LPCHARACTER	m_pkChr;

			int		m_iMinDistance;
			int		m_iMaxDistance;
			long	m_lx;
			long	m_ly;

			LPCHARACTER	m_pkChrVictim;
	};

}

bool CHARACTER::IsAggressive() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_AGGRESSIVE);
}

void CHARACTER::SetAggressive()
{
	SET_BIT(m_pointsInstant.dwAIFlag, AIFLAG_AGGRESSIVE);
}

bool CHARACTER::IsCoward() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_COWARD);
}

void CHARACTER::SetCoward()
{
	SET_BIT(m_pointsInstant.dwAIFlag, AIFLAG_COWARD);
}

bool CHARACTER::IsBerserker() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_BERSERK);
}

bool CHARACTER::IsStoneSkinner() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_STONESKIN);
}

bool CHARACTER::IsGodSpeeder() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_GODSPEED);
}

bool CHARACTER::IsDeathBlower() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_DEATHBLOW);
}

bool CHARACTER::IsReviver() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_REVIVE);
}

void CHARACTER::CowardEscape()
{
	int iDist[4] = {500, 1000, 3000, 5000};

	for (int iDistIdx = 2; iDistIdx >= 0; --iDistIdx)
		for (int iTryCount = 0; iTryCount < 8; ++iTryCount)
		{
			SetRotation(number(0, 359));        // ¹æÇâÀº ·£´ýÀ¸·Î ¼³Á¤

			float fx, fy;
			float fDist = number(iDist[iDistIdx], iDist[iDistIdx+1]);

			GetDeltaByDegree(GetRotation(), fDist, &fx, &fy);

			bool bIsWayBlocked = false;
			for (int j = 1; j <= 100; ++j)
			{
				if (!SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int) fx*j/100, GetY() + (int) fy*j/100))
				{
					bIsWayBlocked = true;
					break;
				}
			}

			if (bIsWayBlocked)
				continue;

			m_dwStateDuration = PASSES_PER_SEC(1);

			int iDestX = GetX() + (int) fx;
			int iDestY = GetY() + (int) fy;

			if (Goto(iDestX, iDestY))
				SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

			sys_log(0, "WAEGU move to %d %d (far)", iDestX, iDestY);
			return;
		}
}

void  CHARACTER::SetNoAttackShinsu()
{
	SET_BIT(m_pointsInstant.dwAIFlag, AIFLAG_NOATTACKSHINSU);
}
bool CHARACTER::IsNoAttackShinsu() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOATTACKSHINSU);
}

void CHARACTER::SetNoAttackChunjo()
{
	SET_BIT(m_pointsInstant.dwAIFlag, AIFLAG_NOATTACKCHUNJO);
}

bool CHARACTER::IsNoAttackChunjo() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOATTACKCHUNJO);
}

void CHARACTER::SetNoAttackJinno()
{
	SET_BIT(m_pointsInstant.dwAIFlag, AIFLAG_NOATTACKJINNO);
}

bool CHARACTER::IsNoAttackJinno() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOATTACKJINNO);
}

void CHARACTER::SetAttackMob()
{
	SET_BIT(m_pointsInstant.dwAIFlag, AIFLAG_ATTACKMOB);
}

bool CHARACTER::IsAttackMob() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_ATTACKMOB);
}

#ifdef ENABLE_SUNG_MAHI_TOWER
bool CHARACTER::IsNomove() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE);
}

void CHARACTER::SetNomove()
{
	SET_BIT(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE);
}

void CHARACTER::RemoveNomove()
{
	REMOVE_BIT(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE);
}

bool CHARACTER::IsNoattack() const
{
	return IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOATTACK);
}

void CHARACTER::SetNoattack()
{
	SET_BIT(m_pointsInstant.dwAIFlag, AIFLAG_NOATTACK);
}

void CHARACTER::RemoveNoattack()
{
	REMOVE_BIT(m_pointsInstant.dwAIFlag, AIFLAG_NOATTACK);
}
#endif


// STATE_IDLE_REFACTORING
void CHARACTER::StateIdle()
{
	if (IsStone())
	{
		__StateIdle_Stone();
		return;
	}
	else if (IsWarp() || IsGoto())
	{
		m_dwStateDuration = 60 * passes_per_sec;
		return;
	}

	if (IsPC()
#ifdef __FAKE_PLAYER__
		|| IsFakePlayer()
#endif			
	)
		return;

	if (!IsMonster())
	{
		__StateIdle_NPC();
		return;
	}

	__StateIdle_Monster();
}

void CHARACTER::__StateIdle_Stone()
{
	m_dwStateDuration = PASSES_PER_SEC(1);

	int iPercent = 0; // @fixme136
	if (GetMaxHP() >= 0)
		iPercent = (GetHP() * 100) / GetMaxHP();
	DWORD dwVnum = number(MIN(GetMobTable().sAttackSpeed, GetMobTable().sMovingSpeed ), MAX(GetMobTable().sAttackSpeed, GetMobTable().sMovingSpeed));

	if (iPercent <= 10 && GetMaxSP() < 10)
	{
		SetMaxSP(10);
		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

		CHARACTER_MANAGER::instance().SelectStone(this);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 500, GetY() - 500, GetX() + 500, GetY() + 500);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1500, GetY() - 1500, GetX() + 1500, GetY() + 1500);
		CHARACTER_MANAGER::instance().SelectStone(NULL);
	}
	else if (iPercent <= 20 && GetMaxSP() < 9)
	{
		SetMaxSP(9);
		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

		CHARACTER_MANAGER::instance().SelectStone(this);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 500, GetY() - 500, GetX() + 500, GetY() + 500);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1500, GetY() - 1500, GetX() + 1500, GetY() + 1500);
		CHARACTER_MANAGER::instance().SelectStone(NULL);
	}
	else if (iPercent <= 30 && GetMaxSP() < 8)
	{
		SetMaxSP(8);
		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

		CHARACTER_MANAGER::instance().SelectStone(this);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 500, GetY() - 500, GetX() + 500, GetY() + 500);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SelectStone(NULL);
	}
	else if (iPercent <= 40 && GetMaxSP() < 7)
	{
		SetMaxSP(7);
		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

		CHARACTER_MANAGER::instance().SelectStone(this);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SelectStone(NULL);
	}
	else if (iPercent <= 50 && GetMaxSP() < 6)
	{
		SetMaxSP(6);
		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

		CHARACTER_MANAGER::instance().SelectStone(this);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SelectStone(NULL);
	}
	else if (iPercent <= 60 && GetMaxSP() < 5)
	{
		SetMaxSP(5);
		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

		CHARACTER_MANAGER::instance().SelectStone(this);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 500, GetY() - 500, GetX() + 500, GetY() + 500);
		CHARACTER_MANAGER::instance().SelectStone(NULL);
	}
	else if (iPercent <= 70 && GetMaxSP() < 4)
	{
		SetMaxSP(4);
		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

		CHARACTER_MANAGER::instance().SelectStone(this);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 500, GetY() - 500, GetX() + 500, GetY() + 500);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SelectStone(NULL);
	}
	else if (iPercent <= 80 && GetMaxSP() < 3)
	{
		SetMaxSP(3);
		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

		CHARACTER_MANAGER::instance().SelectStone(this);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SelectStone(NULL);

#ifdef ENABLE_ZODIAC_MISSION
		if(GetMapIndex() >= 3570000 && GetMapIndex() <= 3579999 && GetDungeon() != NULL)
		{
			int PortalFlag = GetDungeon()->GetFlag("Portal");
			if(PortalFlag >= 1 && PortalFlag <= 12)
			{
				DWORD vnum[] = {0, 35740, 35730,35726,35736,35744,35734,35746,35732,35742,35748,35728,35738};
				SetMaxSP(3);
				SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);
				CHARACTER_MANAGER::instance().SelectStone(this);
				CHARACTER_MANAGER::instance().SpawnGroup(vnum[PortalFlag], GetMapIndex(), GetX() - 500, GetY() - 500, GetX() + 500, GetY() + 500);
				CHARACTER_MANAGER::instance().SelectStone(NULL);
			}
		}
#endif
	}
	else if (iPercent <= 90 && GetMaxSP() < 2)
	{
		SetMaxSP(2);
		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

		CHARACTER_MANAGER::instance().SelectStone(this);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 500, GetY() - 500, GetX() + 500, GetY() + 500);
		CHARACTER_MANAGER::instance().SelectStone(NULL);
	}
	else if (iPercent <= 99 && GetMaxSP() < 1)
	{
		SetMaxSP(1);
		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

		CHARACTER_MANAGER::instance().SelectStone(this);
		CHARACTER_MANAGER::instance().SpawnGroup(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000);
		CHARACTER_MANAGER::instance().SelectStone(NULL);
	}
	else
		return;

	UpdatePacket();
	return;
}

void CHARACTER::__StateIdle_NPC()
{
	MonsterChat(MONSTER_CHAT_WAIT);
	m_dwStateDuration = PASSES_PER_SEC(5);

	// Æê ½Ã½ºÅÛÀÇ Idle Ã³¸®´Â ±âÁ¸ °ÅÀÇ ¸ðµç Á¾·ùÀÇ Ä³¸¯ÅÍµéÀÌ °øÀ¯ÇØ¼­ »ç¿ëÇÏ´Â »óÅÂ¸Ó½ÅÀÌ ¾Æ´Ñ CPetActor::Update¿¡¼­ Ã³¸®ÇÔ.
#ifdef ENABLE_MOUNT_SYSTEM
	if (IsPet() || IsMountSystem() || IsNewPet() || IsBuffNPC())
#else
	if (IsPet())
#endif
		return;
	else if (IsGuardNPC())
	{
		if (!quest::CQuestManager::instance().GetEventFlag("noguard"))
		{
			FuncFindGuardVictim f(this, 50000);

			if (GetSectree())
				GetSectree()->ForEachAround(f);

			LPCHARACTER victim = f.GetVictim();

			if (victim)
			{
				m_dwStateDuration = passes_per_sec/2;

				if (CanBeginFight())
					BeginFight(victim);
			}
		}
	}
	else
	{
		if (GetRaceNum() == xmas::MOB_SANTA_VNUM) // »êÅ¸
		{
			if (get_dword_time() > m_dwPlayStartTime)
			{
				int	next_warp_time = 2 * 1000;	// 2ÃÊ

				m_dwPlayStartTime = get_dword_time() + next_warp_time;

				// ½Ã°£ÀÌ ³Ñ¾úÀ¸´Ï ¿öÇÁÇÕ½Ã´Ù.
				/*
				 * »êÅ¸¿ë
				const int WARP_MAP_INDEX_NUM = 4;
				static const long c_lWarpMapIndexs[WARP_MAP_INDEX_NUM] = {61, 62, 63, 64};
				*/
				// ½Å¼±ÀÚ ³ëÇØ¿ë
				const int WARP_MAP_INDEX_NUM = 7;
				static const long c_lWarpMapIndexs[WARP_MAP_INDEX_NUM] = { 61, 62, 63, 64, 3, 23, 43 };
				long lNextMapIndex;
				lNextMapIndex = c_lWarpMapIndexs[number(1, WARP_MAP_INDEX_NUM) - 1];

				if (map_allow_find(lNextMapIndex))
				{
					// ÀÌ°÷ÀÔ´Ï´Ù.
					M2_DESTROY_CHARACTER(this);
					int iNextSpawnDelay = 50 * 60;

					xmas::SpawnSanta(lNextMapIndex, iNextSpawnDelay);
				}
				else
				{
					// ´Ù¸¥ ¼­¹ö ÀÔ´Ï´Ù.
					TPacketGGXmasWarpSanta p;
					p.bHeader   = HEADER_GG_XMAS_WARP_SANTA;
					p.bChannel  = g_bChannel;
					p.lMapIndex = lNextMapIndex;
					P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGXmasWarpSanta));
				}
				return;
			}
		}

		if (!IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE))
		{
			//
			// ÀÌ °÷ Àú °÷ ÀÌµ¿ÇÑ´Ù.
			//
			LPCHARACTER pkChrProtege = GetProtege();

			if (pkChrProtege)
			{
				if (DISTANCE_APPROX(GetX() - pkChrProtege->GetX(), GetY() - pkChrProtege->GetY()) > 500)
				{
					if (Follow(pkChrProtege, number(100, 300)))
						return;
				}
			}

			if (!number(0, 6))
			{
				SetRotation(number(0, 359));        // ¹æÇâÀº ·£´ýÀ¸·Î ¼³Á¤

				float fx, fy;
				float fDist = number(200, 400);

				GetDeltaByDegree(GetRotation(), fDist, &fx, &fy);

				// ´À½¼ÇÑ ¸ø°¨ ¼Ó¼º Ã¼Å©; ÃÖÁ¾ À§Ä¡¿Í Áß°£ À§Ä¡°¡ °¥¼ö¾ø´Ù¸é °¡Áö ¾Ê´Â´Ù.
				if (!(SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int) fx, GetY() + (int) fy)
					&& SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int) fx / 2, GetY() + (int) fy / 2)))
					return;

				SetNowWalking(true);

				if (Goto(GetX() + (int) fx, GetY() + (int) fy))
					SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

				return;
			}
		}
	}
}

void CHARACTER::__StateIdle_Monster()
{
#ifdef ENABLE_MELEY_LAIR_DUNGEON
	if(IsMeley())
	{
		if(!IsStone() && time(0) > GetProtectTime("MeleyTime"))
		{
			if (HasMobSkill())
			{
				for (unsigned int iSkillIdx = 0; iSkillIdx < MOB_SKILL_MAX_NUM; ++iSkillIdx)
				{
					if (CanUseMobSkill(iSkillIdx))
					{
						if (UseMobSkill(iSkillIdx))
						{
							SendMovePacket(FUNC_MOB_SKILL, iSkillIdx, GetX(), GetY(), 0, get_dword_time());
							float fDuration = CMotionManager::instance().GetMotionDuration(GetRaceNum(), MAKE_MOTION_KEY(MOTION_MODE_GENERAL, MOTION_SPECIAL_1 + iSkillIdx));
							m_dwStateDuration = (DWORD) (fDuration == 0.0f ? PASSES_PER_SEC(2) : PASSES_PER_SEC(fDuration));
							return;
						}
					}
				}
			}
			SetProtectTime("MeleyTime",time(0)+5);
		}
		return;
	}
#endif
	
	if (IsStun())
		return;

	if (!CanMove())
		return;

	if (IsCoward())
	{
		// °ÌÀïÀÌ ¸ó½ºÅÍ´Â µµ¸Á¸¸ ´Ù´Õ´Ï´Ù.
		if (!IsDead())
			CowardEscape();

		return;
	}

	if (IsBerserker())
		if (IsBerserk())
			SetBerserk(false);

	if (IsGodSpeeder())
		if (IsGodSpeed())
			SetGodSpeed(false);

	LPCHARACTER victim = GetVictim();

	if (!victim || victim->IsDead())
	{
		SetVictim(NULL);
		victim = NULL;
		m_dwStateDuration = PASSES_PER_SEC(1);
	}

	if (!victim || victim->IsBuilding())
	{
		// µ¹ º¸È£ Ã³¸®
		if (m_pkChrStone)
		{
			victim = m_pkChrStone->GetNearestVictim(m_pkChrStone);
		}
		// ¼±°ø ¸ó½ºÅÍ Ã³¸®
		else if (!no_wander && IsAggressive())
		{
			if (GetMapIndex() == 61 && quest::CQuestManager::instance().GetEventFlag("xmas_tree"))
			{
				
			}
			else
			{
#ifdef ENABLE_DEFENSAWE_SHIP
				if (IsHydraMob() || IsHydra())
				{
					victim = FindVictimHydra(this, 40000);
				}
				else
				{
					victim = FindVictim(this, m_pkMobData->m_table.wAggressiveSight);
				}
#else
				victim = FindVictim(this, m_pkMobData->m_table.wAggressiveSight);
#endif
			}
		}
	}

   if (victim && !victim->IsDead())
   {
      SECTREE *tree = victim->GetSectree();
      if (tree && tree->IsAttr(victim->GetX(), victim->GetY(), ATTR_BANPK))
         return;
      
      if (CanBeginFight())
         BeginFight(victim);

      return;
   }

	if (IsAggressive() && !victim)
		m_dwStateDuration = PASSES_PER_SEC(number(1, 3));
	else
		m_dwStateDuration = PASSES_PER_SEC(number(3, 5));

	LPCHARACTER pkChrProtege = GetProtege();

	// º¸È£ÇÒ °Í(µ¹, ÆÄÆ¼Àå)¿¡°Ô·Î ºÎÅÍ ¸Ö´Ù¸é µû¶ó°£´Ù.
	if (pkChrProtege)
	{
		if (DISTANCE_APPROX(GetX() - pkChrProtege->GetX(), GetY() - pkChrProtege->GetY()) > 1000)
		{
			if (Follow(pkChrProtege, number(150, 400)))
			{
				MonsterLog("[IDLE] ¸®´õ·ÎºÎÅÍ ³Ê¹« ¸Ö¸® ¶³¾îÁ³´Ù! º¹±ÍÇÑ´Ù.");
				return;
			}
		}
	}

	//
	// ±×³É ¿Ô´Ù¸® °¬´Ù¸® ÇÑ´Ù.
	//
	if (!no_wander && !IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE))
	{
		if (!number(0, 6))
		{
			SetRotation(number(0, 359));        // ¹æÇâÀº ·£´ýÀ¸·Î ¼³Á¤

			float fx, fy;
			float fDist = number(300, 700);

			GetDeltaByDegree(GetRotation(), fDist, &fx, &fy);

			// ´À½¼ÇÑ ¸ø°¨ ¼Ó¼º Ã¼Å©; ÃÖÁ¾ À§Ä¡¿Í Áß°£ À§Ä¡°¡ °¥¼ö¾ø´Ù¸é °¡Áö ¾Ê´Â´Ù.
			if (!(SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int) fx, GetY() + (int) fy)
						&& SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int) fx/2, GetY() + (int) fy/2)))
				return;

			// NOTE: ¸ó½ºÅÍ°¡ IDLE »óÅÂ¿¡¼­ ÁÖº¯À» ¼­¼º°Å¸± ¶§, ÇöÀç ¹«Á¶°Ç ¶Ù¾î°¡°Ô µÇ¾î ÀÖÀ½. (Àý´ë·Î °ÈÁö ¾ÊÀ½)
			// ±×·¡ÇÈ ÆÀ¿¡¼­ ¸ó½ºÅÍ°¡ °È´Â ¸ð½Àµµ º¸°í½Í´Ù°í ÇØ¼­ ÀÓ½Ã·Î Æ¯Á¤È®·ü·Î °È°Å³ª ¶Ù°Ô ÇÔ. (°ÔÀÓÀÇ Àü¹ÝÀûÀÎ ´À³¦ÀÌ Æ²·ÁÁö±â ¶§¹®¿¡ ÀÏ´Ü Å×½ºÆ® ¸ðµå¿¡¼­¸¸ ÀÛµ¿)
			if (test_server) // @warme010
			{
				if (number(0, 100) < 60)
					SetNowWalking(false);
				else
					SetNowWalking(true);
			}

			if (Goto(GetX() + (int) fx, GetY() + (int) fy))
				SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

			return;
		}
	}

	MonsterChat(MONSTER_CHAT_WAIT);
}
// END_OF_STATE_IDLE_REFACTORING

bool __CHARACTER_GotoNearTarget(LPCHARACTER self, LPCHARACTER victim)
{
	if (IS_SET(self->GetAIFlag(), AIFLAG_NOMOVE))
		return false;

	switch (self->GetMobBattleType())
	{
		case BATTLE_TYPE_RANGE:
		case BATTLE_TYPE_MAGIC:
			// ¸¶¹ý»ç³ª ±Ã¼ö´Â °ø°Ý °Å¸®ÀÇ 80%±îÁö °¡¼­ °ø°ÝÀ» ½ÃÀÛÇÑ´Ù.
			if (self->Follow(victim, self->GetMobAttackRange() * 8 / 10))
				return true;
			break;

		default:
			// ³ª¸ÓÁö´Â 90%?
			if (self->Follow(victim, self->GetMobAttackRange() * 9 / 10))
				return true;
	}

	return false;
}

void CHARACTER::StateMove()
{
	float fRate = 1.0f;
	
	const DWORD dwElapsedTime = get_dword_time() - m_dwMoveStartTime;
	if (dwElapsedTime != 0 && m_dwMoveStartTime != 0)
		fRate = static_cast<float>(dwElapsedTime) / static_cast<float>(m_dwMoveDuration);

	if (fRate > 1.0f)
		fRate = 1.0f;

	const int x = static_cast<int>(static_cast<float>(m_posDest.x - m_posStart.x) * fRate + m_posStart.x);
	const int y = static_cast<int>(static_cast<float>(m_posDest.y - m_posStart.y) * fRate + m_posStart.y);

	Move(x, y);

	if (IsPC() && (thecore_pulse() & 15) == 0)
	{
		UpdateSectree();

		if (GetExchange())
		{
			LPCHARACTER victim = GetExchange()->GetCompany()->GetOwner();
			int iDist = DISTANCE_APPROX(GetX() - victim->GetX(), GetY() - victim->GetY());

			// °Å¸® Ã¼Å©
			if (iDist >= EXCHANGE_MAX_DISTANCE)
			{
				GetExchange()->Cancel();
			}
		}
	}

	if (IsPC()
#ifdef __FAKE_PLAYER__
		|| IsFakePlayer()
#endif		
	)
	{
		if (IsWalking() && GetStamina() < GetMaxStamina())
		{
			// 5ÃÊ ÈÄ ºÎÅÍ ½ºÅ×¹Ì³Ê Áõ°¡
			if (get_dword_time() - GetWalkStartTime() > 5000)
				PointChange(POINT_STAMINA, GetMaxStamina() / 1);
		}

		// ÀüÅõ ÁßÀÌ¸é¼­ ¶Ù´Â ÁßÀÌ¸é
		if (!IsWalking() && !IsRiding()){
			if ((get_dword_time() - GetLastAttackTime()) < 20000)
			{
				StartAffectEvent();

				if (IsStaminaHalfConsume())
				{
					if (thecore_pulse()&1)
						PointChange(POINT_STAMINA, -STAMINA_PER_STEP);
				}
				else
					PointChange(POINT_STAMINA, -STAMINA_PER_STEP);

				StartStaminaConsume();

				if (GetStamina() <= 0)
				{
					// ½ºÅ×¹Ì³ª°¡ ¸ðÀÚ¶ó °É¾î¾ßÇÔ
					SetStamina(0);
					SetNowWalking(true);
					StopStaminaConsume();
				}
			}
			else if (IsStaminaConsume())
			{
				StopStaminaConsume();
			}
		}
	}
	else
	{
		// XXX AGGRO
		if (IsMonster() && GetVictim())
		{
			LPCHARACTER victim = GetVictim();
			UpdateAggrPoint(victim, DAMAGE_TYPE_NORMAL, -(victim->GetLevel() / 3 + 1));

			if (test_server) // @warme010
			{
				// ¸ó½ºÅÍ°¡ ÀûÀ» ÂÑ¾Æ°¡´Â °ÍÀÌ¸é ¹«Á¶°Ç ¶Ù¾î°£´Ù.
				SetNowWalking(false);
			}
		}

		if (IsMonster() && GetMobRank() >= MOB_RANK_BOSS && GetVictim())
		{
			LPCHARACTER victim = GetVictim();

			// °Å´ë °ÅºÏ
			if (GetRaceNum() == 2191 && number(1, 20) == 1 && get_dword_time() - m_pkMobInst->m_dwLastWarpTime > 1000)
			{
				// ¿öÇÁ Å×½ºÆ®
				float fx, fy;
				GetDeltaByDegree(victim->GetRotation(), 400, &fx, &fy);
				long new_x = victim->GetX() + (long)fx;
				long new_y = victim->GetY() + (long)fy;
				SetRotation(GetDegreeFromPositionXY(new_x, new_y, victim->GetX(), victim->GetY()));
				Show(victim->GetMapIndex(), new_x, new_y, 0, true);
				GotoState(m_stateBattle);
				m_dwStateDuration = 1;
				ResetMobSkillCooltime();
				m_pkMobInst->m_dwLastWarpTime = get_dword_time();
				return;
			}

			// TODO ¹æÇâÀüÈ¯À» ÇØ¼­ ´ú ¹Ùº¸°¡ µÇÀÚ!
			if (number(0, 3) == 0)
			{
				if (__CHARACTER_GotoNearTarget(this, victim))
					return;
			}
		}
	}

	if (1.0f == fRate)
	{
		if (IsPC())
		{
			sys_log(1, "µµÂø %s %d %d", GetName(), x, y);
			GotoState(m_stateIdle);
			StopStaminaConsume();
		}
		else
		{
			if (GetVictim() && !IsCoward())
			{
				if (!IsState(m_stateBattle))
					MonsterLog("[BATTLE] ±ÙÃ³¿¡ ¿ÔÀ¸´Ï °ø°Ý½ÃÀÛ %s", GetVictim()->GetName());

				GotoState(m_stateBattle);
				m_dwStateDuration = 1;
			}
			else
			{
				if (!IsState(m_stateIdle))
					MonsterLog("[IDLE] ´ë»óÀÌ ¾øÀ¸´Ï ½¬ÀÚ");

				GotoState(m_stateIdle);

				//LPCHARACTER rider = GetRider();

				m_dwStateDuration = PASSES_PER_SEC(number(1, 3));
			}
		}
	}
}

void CHARACTER::StateBattle()
{
	if (IsStone())
	{
		sys_err("Stone must not use battle state (name %s)", GetName());
		return;
	}

	if (IsPC() 
#ifdef __FAKE_PLAYER__
		|| IsFakePlayer()	
#endif
	)
		return;

	if (!CanMove())
		return;

	if (IsStun())
		return;

	LPCHARACTER victim = GetVictim();

	if (IsCoward())
	{
		if (IsDead())
			return;

		SetVictim(NULL);

		if (number(1, 50) != 1)
		{
			GotoState(m_stateIdle);
			m_dwStateDuration = 1;
		}
		else
			CowardEscape();

		return;
	}

	if (!victim || (victim->IsStun() && IsGuardNPC()) || victim->IsDead())
	{
		if (victim && victim->IsDead() &&
				!no_wander && IsAggressive() && (!GetParty() || GetParty()->GetLeader() == this))
		{
			LPCHARACTER new_victim = NULL;
#ifdef ENABLE_DEFENSAWE_SHIP
			if(IsHydraMob() || IsHydra())
				new_victim = FindVictimHydra(this, 40000);
			else
				new_victim = FindVictim(this, m_pkMobData->m_table.wAggressiveSight);
#else
			LPCHARACTER new_victim = FindVictim(this, m_pkMobData->m_table.wAggressiveSight);
#endif
			SetVictim(new_victim);
			m_dwStateDuration = PASSES_PER_SEC(1);

			if (!new_victim)
			{
				switch (GetMobBattleType())
				{
					case BATTLE_TYPE_MELEE:
					case BATTLE_TYPE_SUPER_POWER:
					case BATTLE_TYPE_SUPER_TANKER:
					case BATTLE_TYPE_POWER:
					case BATTLE_TYPE_TANKER:
						{
							float fx, fy;
							float fDist = number(400, 1500);

							GetDeltaByDegree(number(0, 359), fDist, &fx, &fy);

							if (SECTREE_MANAGER::instance().IsMovablePosition(victim->GetMapIndex(),
										victim->GetX() + (int) fx,
										victim->GetY() + (int) fy) &&
									SECTREE_MANAGER::instance().IsMovablePosition(victim->GetMapIndex(),
										victim->GetX() + (int) fx/2,
										victim->GetY() + (int) fy/2))
							{
								float dx = victim->GetX() + fx;
								float dy = victim->GetY() + fy;

								SetRotation(GetDegreeFromPosition(dx, dy));

								if (Goto((long) dx, (long) dy))
								{
									sys_log(0, "KILL_AND_GO: %s distance %.1f", GetName(), fDist);
									SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
								}
							}
						}
				}
			}
			return;
		}

		SetVictim(NULL);

		if (IsGuardNPC())
			Return();

		m_dwStateDuration = PASSES_PER_SEC(1);
		return;
	}

	if (IsSummonMonster() && !IsDead() && !IsStun())
	{
		if (!GetParty())
		{
			// ¼­¸óÇØ¼­ Ã¤¿öµÑ ÆÄÆ¼¸¦ ¸¸µé¾î µÓ´Ï´Ù.
			CPartyManager::instance().CreateParty(this);
		}

		LPPARTY pParty = GetParty();
		bool bPct = !number(0, 3);

		if (bPct && pParty->CountMemberByVnum(GetSummonVnum()) < SUMMON_MONSTER_COUNT)
		{
			MonsterLog("ºÎÇÏ ¸ó½ºÅÍ ¼ÒÈ¯!");
			// ¸ðÀÚ¶ó´Â ³à¼®À» ºÒ·¯³» Ã¤¿ó½Ã´Ù.
			int sx = GetX() - 300;
			int sy = GetY() - 300;
			int ex = GetX() + 300;
			int ey = GetY() + 300;

			LPCHARACTER tch = CHARACTER_MANAGER::instance().SpawnMobRange(GetSummonVnum(), GetMapIndex(), sx, sy, ex, ey, true, true);

			if (tch)
			{
				pParty->Join(tch->GetVID());
				pParty->Link(tch);
			}
		}
	}

	LPCHARACTER pkChrProtege = GetProtege();

	float fDist = DISTANCE_APPROX(GetX() - victim->GetX(), GetY() - victim->GetY());
	float fDistCheck = 15000.0f;
	
#ifdef ENABLE_SUNG_MAHI_TOWER
	if (GetMapIndex() >= (354 * 10000) && GetMapIndex() < ((354 + 1) * 10000))
		fDistCheck = 16000.0f;
#endif

#ifdef __RENEWAL_BRAVE_CAPE__
	if (fDist >= fDistCheck)
#else
   if (fDist >= 100.0f)
   {
      int boss_no_follow[] = {5127, 2493, 2091, 2092, 1903, 2492, 1191, 3691, 2598, 1901, 3091, 3191, 3591 , 3691 , 3791 , 3891 , 3391 , 6228 , 6091 , 6009 , 6109 , 6151};
      for (int i = 0; i < _countof(boss_no_follow); i++)
      {
         if (m_pkMobData->m_table.dwVnum == boss_no_follow[i])
         {
            SetVictim(NULL);
            return;            
         }
      }
   }
	if (fDist >= 4000.0f)
#endif
	{
		MonsterLog("Å¸°ÙÀÌ ¸Ö¾î¼­ Æ÷±â");
		SetVictim(NULL);

		// º¸È£ÇÒ °Í(µ¹, ÆÄÆ¼Àå) ÁÖº¯À¸·Î °£´Ù.
		if (pkChrProtege)
			if (DISTANCE_APPROX(GetX() - pkChrProtege->GetX(), GetY() - pkChrProtege->GetY()) > 1000)
				Follow(pkChrProtege, number(150, 400));

		return;
	}

	if (fDist >= GetMobAttackRange() * 1.15)
	{
		__CHARACTER_GotoNearTarget(this, victim);
		return;
	}

	if (m_pkParty)
		m_pkParty->SendMessage(this, PM_ATTACKED_BY, 0, 0);

	if (2493 == m_pkMobData->m_table.dwVnum)
	{
		// ¼ö·æ(2493) Æ¯¼ö Ã³¸®
		m_dwStateDuration = BlueDragon_StateBattle(this);
		return;
	}

	DWORD dwCurTime = get_dword_time();
	DWORD dwDuration = CalculateDuration(GetLimitPoint(POINT_ATT_SPEED), 2000);

	if ((dwCurTime - m_dwLastAttackTime) < dwDuration) // 2ÃÊ ¸¶´Ù °ø°ÝÇØ¾ß ÇÑ´Ù.
	{
		m_dwStateDuration = MAX(1, (passes_per_sec * (dwDuration - (dwCurTime - m_dwLastAttackTime)) / 1000));
		return;
	}

	if (IsBerserker() == true)
		if (GetHPPct() < m_pkMobData->m_table.bBerserkPoint)
			if (IsBerserk() != true)
				SetBerserk(true);

	if (IsGodSpeeder() == true)
		if (GetHPPct() < m_pkMobData->m_table.bGodSpeedPoint)
			if (IsGodSpeed() != true)
				SetGodSpeed(true);

	//
	// ¸÷ ½ºÅ³ Ã³¸®
	//
	if (HasMobSkill())
	{
		for (unsigned int iSkillIdx = 0; iSkillIdx < MOB_SKILL_MAX_NUM; ++iSkillIdx)
		{
			if (CanUseMobSkill(iSkillIdx))
			{
				SetRotationToXY(victim->GetX(), victim->GetY());

				if (UseMobSkill(iSkillIdx))
				{
					SendMovePacket(FUNC_MOB_SKILL, iSkillIdx, GetX(), GetY(), 0, dwCurTime);

					float fDuration = CMotionManager::instance().GetMotionDuration(GetRaceNum(), MAKE_MOTION_KEY(MOTION_MODE_GENERAL, MOTION_SPECIAL_1 + iSkillIdx));
					m_dwStateDuration = (DWORD) (fDuration == 0.0f ? PASSES_PER_SEC(2) : PASSES_PER_SEC(fDuration));

					if (test_server)
						sys_log(0, "USE_MOB_SKILL: %s idx %u motion %u duration %.0f", GetName(), iSkillIdx, MOTION_SPECIAL_1 + iSkillIdx, fDuration);

					return;
				}
			}
		}
	}

	if (!Attack(victim))    // °ø°Ý ½ÇÆÐ¶ó¸é? ¿Ö ½ÇÆÐÇßÁö? TODO
		m_dwStateDuration = passes_per_sec / 2;
	else
	{
		// ÀûÀ» ¹Ù¶óº¸°Ô ¸¸µç´Ù.
		SetRotationToXY(victim->GetX(), victim->GetY());

		SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0, dwCurTime);

		float fDuration = CMotionManager::instance().GetMotionDuration(GetRaceNum(), MAKE_MOTION_KEY(MOTION_MODE_GENERAL, MOTION_NORMAL_ATTACK));
		m_dwStateDuration = (DWORD) (fDuration == 0.0f ? PASSES_PER_SEC(2) : PASSES_PER_SEC(fDuration));
	}
}

void CHARACTER::StateFlag()
{
	m_dwStateDuration = (DWORD) PASSES_PER_SEC(0.5);

	CWarMap * pMap = GetWarMap();

	if (!pMap)
		return;

	FuncFindChrForFlag f(this);
	GetSectree()->ForEachAround(f);

	if (!f.m_pkChrFind)
		return;

	if (NULL == f.m_pkChrFind->GetGuild())
		return;

	char buf[256];
	BYTE idx;

	if (!pMap->GetTeamIndex(GetPoint(POINT_STAT), idx))
		return;

	f.m_pkChrFind->AddAffect(AFFECT_WAR_FLAG, POINT_NONE, GetPoint(POINT_STAT), idx == 0 ? AFF_WAR_FLAG1 : AFF_WAR_FLAG2, INFINITE_AFFECT_DURATION, 0, false);
	f.m_pkChrFind->AddAffect(AFFECT_WAR_FLAG, POINT_MOV_SPEED, 50 - f.m_pkChrFind->GetPoint(POINT_MOV_SPEED), 0, INFINITE_AFFECT_DURATION, 0, false);

	pMap->RemoveFlag(idx);

	snprintf(buf, sizeof(buf), "375 %s %s", pMap->GetGuild(idx)->GetName(), f.m_pkChrFind->GetName());
	pMap->Notice(buf);
}

void CHARACTER::StateFlagBase()
{
	m_dwStateDuration = (DWORD) PASSES_PER_SEC(0.5);

	FuncFindChrForFlagBase f(this);
	GetSectree()->ForEachAround(f);
}

void CHARACTER::StateHorse()
{
	float	START_FOLLOW_DISTANCE = 400.0f;		// ÀÌ °Å¸® ÀÌ»ó ¶³¾îÁö¸é ÂÑ¾Æ°¡±â ½ÃÀÛÇÔ
	float	START_RUN_DISTANCE = 700.0f;		// ÀÌ °Å¸® ÀÌ»ó ¶³¾îÁö¸é ¶Ù¾î¼­ ÂÑ¾Æ°¨.
	int		MIN_APPROACH = 150;					// ÃÖ¼Ò Á¢±Ù °Å¸®
	int		MAX_APPROACH = 300;					// ÃÖ´ë Á¢±Ù °Å¸®
	float	RESPAWN_DISTANCE = 3000.0f;
	int		APPROACH = 200;

	DWORD	STATE_DURATION = (DWORD)PASSES_PER_SEC(0.5);	// »óÅÂ Áö¼Ó ½Ã°£

	bool bDoMoveAlone = true;					// Ä³¸¯ÅÍ¿Í °¡±îÀÌ ÀÖÀ» ¶§ È¥ÀÚ ¿©±âÀú±â ¿òÁ÷ÀÏ°ÇÁö ¿©ºÎ -_-;
	bool bRun = true;							// ¶Ù¾î¾ß ÇÏ³ª?

	if (IsDead())
		return;

	m_dwStateDuration = STATE_DURATION;

	LPCHARACTER victim = GetRider();

	// ! ¾Æ´Ô // ´ë»óÀÌ ¾ø´Â °æ¿ì ¼ÒÈ¯ÀÚ°¡ Á÷Á¢ ³ª¸¦ Å¬¸®¾îÇÒ °ÍÀÓ
	if (!victim)
	{
		M2_DESTROY_CHARACTER(this);
		return;
	}

	m_pkMobInst->m_posLastAttacked = GetXYZ();

	float fDist = DISTANCE_APPROX(GetX() - victim->GetX(), GetY() - victim->GetY());

	if (fDist >= RESPAWN_DISTANCE)
	{
		float fOwnerRot = victim->GetRotation() * 3.141592f / 180.f;
		float fx = -APPROACH * cos(fOwnerRot);
		float fy = -APPROACH * sin(fOwnerRot);
		Show(victim->GetMapIndex(), victim->GetX() + fx, victim->GetY() + fy);
	}
	else if (fDist >= START_FOLLOW_DISTANCE)
	{
		if (fDist > START_RUN_DISTANCE)
			SetNowWalking(!bRun);		// NOTE: ÇÔ¼ö ÀÌ¸§º¸°í ¸ØÃß´Â°ÇÁÙ ¾Ë¾Ò´Âµ¥ SetNowWalking(false) ÇÏ¸é ¶Ù´Â°ÅÀÓ.. -_-;

		Follow(victim, number(MIN_APPROACH, MAX_APPROACH));

		m_dwStateDuration = STATE_DURATION;
	}
	else if (bDoMoveAlone && (get_dword_time() > m_dwLastAttackTime))
	{
		// wondering-.-
		m_dwLastAttackTime = get_dword_time() + number(5000, 12000);

		SetRotation(number(0, 359));        // ¹æÇâÀº ·£´ýÀ¸·Î ¼³Á¤

		float fx, fy;
		float fDist = number(200, 400);

		GetDeltaByDegree(GetRotation(), fDist, &fx, &fy);

		// ´À½¼ÇÑ ¸ø°¨ ¼Ó¼º Ã¼Å©; ÃÖÁ¾ À§Ä¡¿Í Áß°£ À§Ä¡°¡ °¥¼ö¾ø´Ù¸é °¡Áö ¾Ê´Â´Ù.
		if (!(SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int) fx, GetY() + (int) fy)
					&& SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int) fx/2, GetY() + (int) fy/2)))
			return;

		SetNowWalking(true);

		if (Goto(GetX() + (int) fx, GetY() + (int) fy))
			SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	}
}


#ifdef ENABLE_NEW_PET_SYSTEM
void CHARACTER::StatePet()
{
	float	START_FOLLOW_DISTANCE = 200.0f;
	float	START_RUN_DISTANCE = 300.0f;
	float	RESPAWN_DISTANCE = 3000.0f;
	int		APPROACH = 200;

	bool bDoMoveAlone = true;
	bool bRun = true;

	m_dwStateDuration = 0;
	LPCHARACTER victim = GetOwner();

	if (!victim)
	{
		//M2_DESTROY_CHARACTER(this);
		return;
	}

	m_pkMobInst->m_posLastAttacked = GetXYZ();

	float fDist = DISTANCE_APPROX(GetX() - victim->GetX(), GetY() - victim->GetY());

	if (fDist >= RESPAWN_DISTANCE)
	{
		float fOwnerRot = victim->GetRotation() * 3.141592f / 180.f;
		float fx = -APPROACH * cos(fOwnerRot);
		float fy = -APPROACH * sin(fOwnerRot);
		Show(victim->GetMapIndex(), victim->GetX() + fx, victim->GetY() + fy);
	}
	else if (fDist >= START_FOLLOW_DISTANCE)
	{
		if (fDist > START_RUN_DISTANCE)
			SetNowWalking(!bRun);

		Follow(victim, APPROACH);
	}
	else if (bDoMoveAlone && (get_dword_time() > m_dwLastAttackTime))
	{
		m_dwLastAttackTime = get_dword_time() + number(5000, 12000);
		SetRotation(number(0, 359));
		float fx, fy;
		float fDist = number(200, 400);
		GetDeltaByDegree(GetRotation(), fDist, &fx, &fy);

		if (!(SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int)fx, GetY() + (int)fy)
			&& SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int)fx / 2, GetY() + (int)fy / 2)))
			return;

		SetNowWalking(true);

		if (Goto(GetX() + (int)fx, GetY() + (int)fy))
			SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	}
}
#endif

#ifdef __BUFFI_SUPPORT__
#include "buffi.h"
void CHARACTER::StateBuffi()
{
	float	START_FOLLOW_DISTANCE = 200.0f;
	float	START_RUN_DISTANCE = 300.0f;
	float	RESPAWN_DISTANCE = 3000.0f;
	int		APPROACH = 200;

	bool bDoMoveAlone = true;
	bool bRun = true;

	DWORD	STATE_DURATION = (DWORD)PASSES_PER_SEC(0.25);

	m_dwStateDuration = STATE_DURATION;
	LPCHARACTER victim = GetOwner();

	if (!victim)
	{
		//M2_DESTROY_CHARACTER(this);
		return;
	}

	m_pkMobInst->m_posLastAttacked = GetXYZ();

	float fDist = DISTANCE_APPROX(GetX() - victim->GetX(), GetY() - victim->GetY());

	if (fDist >= RESPAWN_DISTANCE)
	{
		float fOwnerRot = victim->GetRotation() * 3.141592f / 180.f;
		float fx = -APPROACH * cos(fOwnerRot);
		float fy = -APPROACH * sin(fOwnerRot);
		Show(victim->GetMapIndex(), victim->GetX() + fx, victim->GetY() + fy);
	}
	else if (fDist >= START_FOLLOW_DISTANCE)
	{
		if (fDist > START_RUN_DISTANCE)
			SetNowWalking(!bRun);

		Follow(victim, APPROACH);
	}
	else if (bDoMoveAlone && (get_dword_time() > m_dwLastAttackTime))
	{
		m_dwLastAttackTime = get_dword_time() + number(5000, 12000);
		SetRotation(number(0, 359));
		float fx, fy;
		float fDist = number(200, 400);
		GetDeltaByDegree(GetRotation(), fDist, &fx, &fy);

		if (!(SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int)fx, GetY() + (int)fy)
			&& SECTREE_MANAGER::instance().IsMovablePosition(GetMapIndex(), GetX() + (int)fx / 2, GetY() + (int)fy / 2)))
			return;

		SetNowWalking(true);

		if (Goto(GetX() + (int)fx, GetY() + (int)fy))
			SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	}
	BUFFI* buffi = BUFFI_MANAGER::Instance().GetBuffi(victim->GetPlayerID(), GetVID());
	if (buffi)
		buffi->CheckOwnerSupport();
}
#endif
