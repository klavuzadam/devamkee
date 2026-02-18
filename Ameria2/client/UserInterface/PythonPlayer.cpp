#include "StdAfx.h"
#include "PythonPlayerEventHandler.h"
#include "PythonApplication.h"
#include "PythonItem.h"
#include "../eterbase/Timer.h"

#include "AbstractPlayer.h"
#include "../gamelib/GameLibDefines.h"

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
#include "CShield.h"
#endif

const long long POINT_MAGIC_NUMBER = 0xe73ac1da;

void CPythonPlayer::SPlayerStatus::SetPoint(UINT ePoint, long long lPoint)
{
	m_alPoint[ePoint]=lPoint ^ POINT_MAGIC_NUMBER;
}

long long CPythonPlayer::SPlayerStatus::GetPoint(UINT ePoint)
{
	return m_alPoint[ePoint] ^ POINT_MAGIC_NUMBER;
}

bool CPythonPlayer::AffectIndexToSkillIndex(DWORD dwAffectIndex, DWORD * pdwSkillIndex)
{
	if (m_kMap_dwAffectIndexToSkillIndex.end() == m_kMap_dwAffectIndexToSkillIndex.find(dwAffectIndex))
		return false;

	*pdwSkillIndex = m_kMap_dwAffectIndexToSkillIndex[dwAffectIndex];
	return true;
}

bool CPythonPlayer::AffectIndexToSkillSlotIndex(UINT uAffect, DWORD* pdwSkillSlotIndex)
{
	DWORD dwSkillIndex=m_kMap_dwAffectIndexToSkillIndex[uAffect];

	return GetSkillSlotIndex(dwSkillIndex, pdwSkillSlotIndex);
}

bool CPythonPlayer::__GetPickedActorPtr(CInstanceBase** ppkInstPicked)
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	CInstanceBase* pkInstPicked=rkChrMgr.OLD_GetPickedInstancePtr();
	if (!pkInstPicked)
		return false;

	*ppkInstPicked=pkInstPicked;
	return true;
}

bool CPythonPlayer::__GetPickedActorID(DWORD* pdwActorID)
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	return rkChrMgr.OLD_GetPickedInstanceVID(pdwActorID);
}

bool CPythonPlayer::__GetPickedItemID(DWORD* pdwItemID)
{
	CPythonItem& rkItemMgr=CPythonItem::Instance();
	return rkItemMgr.GetPickedItemID(pdwItemID);
}

bool CPythonPlayer::__GetPickedGroundPos(TPixelPosition* pkPPosPicked)
{
	CPythonBackground& rkBG=CPythonBackground::Instance();

	TPixelPosition kPPosPicked;
	if (rkBG.GetPickingPoint(pkPPosPicked))
	{
		pkPPosPicked->y=-pkPPosPicked->y;
		return true;
	}

	return false;
}

void CPythonPlayer::NEW_GetMainActorPosition(TPixelPosition* pkPPosActor)
{
	TPixelPosition kPPosMainActor;

	IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
	CInstanceBase * pInstance = rkPlayer.NEW_GetMainActorPtr();
	if (pInstance)
	{
		pInstance->NEW_GetPixelPosition(pkPPosActor);
	}
	else
	{
		CPythonApplication::Instance().GetCenterPosition(pkPPosActor);
	}
}



bool CPythonPlayer::RegisterEffect(DWORD dwEID, const char* c_szFileName, bool isCache)
{
	if (dwEID>=EFFECT_NUM)
		return false;

	CEffectManager& rkEftMgr=CEffectManager::Instance();
	rkEftMgr.RegisterEffect2(c_szFileName, &m_adwEffect[dwEID], isCache);
	return true;
}

void CPythonPlayer::NEW_ShowEffect(int dwEID, TPixelPosition kPPosDst)
{
	if (dwEID>=EFFECT_NUM)
		return;

	D3DXVECTOR3 kD3DVt3Pos(kPPosDst.x, -kPPosDst.y, kPPosDst.z);
	D3DXVECTOR3 kD3DVt3Dir(0.0f, 0.0f, 1.0f);

	CEffectManager& rkEftMgr=CEffectManager::Instance();
	rkEftMgr.CreateEffect(m_adwEffect[dwEID], kD3DVt3Pos, kD3DVt3Dir);
}

CInstanceBase* CPythonPlayer::NEW_FindActorPtr(DWORD dwVID)
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	return rkChrMgr.GetInstancePtr(dwVID);
}

CInstanceBase* CPythonPlayer::NEW_GetMainActorPtr()
{
	return NEW_FindActorPtr(m_dwMainCharacterIndex);
}

///////////////////////////////////////////////////////////////////////////////////////////

void CPythonPlayer::Update()
{
#ifdef __AUTO_HUNT__
	AutoHuntLoop();
#endif
	NEW_RefreshMouseWalkingDirection();
	CPythonPlayer::Instance().SetMetinstonesQueueCount2(5);
#ifdef __AUTO_QUQUE_ATTACK__
	AutoFarmLoop();
#endif
	CPythonPlayerEventHandler& rkPlayerEventHandler=CPythonPlayerEventHandler::GetSingleton();
	rkPlayerEventHandler.FlushVictimList();

	if (m_isDestPosition)
	{
		CInstanceBase * pInstance = NEW_GetMainActorPtr();
		if (pInstance)
		{
			TPixelPosition PixelPosition;
			pInstance->NEW_GetPixelPosition(&PixelPosition);

			if (abs(int(PixelPosition.x) - m_ixDestPos) + abs(int(PixelPosition.y) - m_iyDestPos) < 10000)
			{
				m_isDestPosition = FALSE;
			}
			else
			{
				if (CTimer::Instance().GetCurrentMillisecond() - m_iLastAlarmTime > 20000)
				{
					AlarmHaveToGo();
				}
			}
		}
	}

	if (m_isConsumingStamina)
	{
		float fElapsedTime = CTimer::Instance().GetElapsedSecond();
		m_fCurrentStamina -= (fElapsedTime * m_fConsumeStaminaPerSec);

		SetStatus(POINT_STAMINA, DWORD(m_fCurrentStamina));

		PyCallClassMemberFunc(m_ppyGameWindow, "RefreshStamina", Py_BuildValue("()"));
	}

	__Update_AutoAttack();
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
	__AutoProcess();
#endif
	__Update_NotifyGuildAreaEvent();
}

bool CPythonPlayer::__IsUsingChargeSkill()
{
	CInstanceBase * pkInstMain = NEW_GetMainActorPtr();
	if (!pkInstMain)
		return false;

	if (__CheckDashAffect(*pkInstMain))
		return true;

	if (MODE_USE_SKILL != m_eReservedMode)
		return false;

	if (m_dwSkillSlotIndexReserved >= SKILL_MAX_NUM)
		return false;

	TSkillInstance & rkSkillInst = m_playerStatus.aSkill[m_dwSkillSlotIndexReserved];

	CPythonSkill::TSkillData * pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(rkSkillInst.dwIndex, &pSkillData))
		return false;

	return pSkillData->IsChargeSkill() ? true : false;
}

void CPythonPlayer::__Update_AutoAttack()
{
	if (0 == m_dwAutoAttackTargetVID)
		return;

	CInstanceBase * pkInstMain = NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;

	// 탄환격 쓰고 달려가는 도중에는 스킵
	if (__IsUsingChargeSkill())
		return;

	CInstanceBase* pkInstVictim=NEW_FindActorPtr(m_dwAutoAttackTargetVID);
	if (!pkInstVictim)
	{
		__ClearAutoAttackTargetActorID();
	}
	else
	{
		if (pkInstVictim->IsDead())
		{
			__ClearAutoAttackTargetActorID();
		}
		else if (pkInstMain->IsMountingHorse() && !pkInstMain->CanAttackHorseLevel())
		{
			__ClearAutoAttackTargetActorID();
		}
		else if (pkInstMain->IsAttackableInstance(*pkInstVictim))
		{
			if (pkInstMain->IsSleep())
			{
				//TraceError("SKIP_AUTO_ATTACK_IN_SLEEPING");
			}
			else
			{
				__ReserveClickActor(m_dwAutoAttackTargetVID);
			}
		}
	}
}

void CPythonPlayer::__Update_NotifyGuildAreaEvent()
{
	CInstanceBase * pkInstMain = NEW_GetMainActorPtr();
	if (pkInstMain)
	{
		TPixelPosition kPixelPosition;
		pkInstMain->NEW_GetPixelPosition(&kPixelPosition);

		DWORD dwAreaID = CPythonMiniMap::Instance().GetGuildAreaID(
			ULONG(kPixelPosition.x), ULONG(kPixelPosition.y));

		if (dwAreaID != m_inGuildAreaID)
		{
			if (0xffffffff != dwAreaID)
			{
				PyCallClassMemberFunc(m_ppyGameWindow, "BINARY_Guild_EnterGuildArea", Py_BuildValue("(i)", dwAreaID));
			}
			else
			{
				PyCallClassMemberFunc(m_ppyGameWindow, "BINARY_Guild_ExitGuildArea", Py_BuildValue("(i)", dwAreaID));
			}

			m_inGuildAreaID = dwAreaID;
		}
	}
}

void CPythonPlayer::SetMainCharacterIndex(int iIndex)
{
	m_dwMainCharacterIndex = iIndex;

	CInstanceBase* pkInstMain=NEW_GetMainActorPtr();
	if (pkInstMain)
	{
		CPythonPlayerEventHandler& rkPlayerEventHandler=CPythonPlayerEventHandler::GetSingleton();
		pkInstMain->SetEventHandler(&rkPlayerEventHandler);
	}
}

DWORD CPythonPlayer::GetMainCharacterIndex()
{
	return m_dwMainCharacterIndex;
}

bool CPythonPlayer::IsMainCharacterIndex(DWORD dwIndex)
{
	return (m_dwMainCharacterIndex == dwIndex);
}

DWORD CPythonPlayer::GetGuildID()
{
	CInstanceBase* pkInstMain=NEW_GetMainActorPtr();
	if (!pkInstMain)
		return 0xffffffff;

	return pkInstMain->GetGuildID();
}

void CPythonPlayer::SetWeaponPower(DWORD dwMinPower, DWORD dwMaxPower, DWORD dwMinMagicPower, DWORD dwMaxMagicPower, DWORD dwAddPower)
{
	m_dwWeaponMinPower=dwMinPower;
	m_dwWeaponMaxPower=dwMaxPower;
	m_dwWeaponMinMagicPower=dwMinMagicPower;
	m_dwWeaponMaxMagicPower=dwMaxMagicPower;
	m_dwWeaponAddPower=dwAddPower;

	__UpdateBattleStatus();
}

void CPythonPlayer::SetRace(DWORD dwRace)
{
	m_dwRace=dwRace;
}

DWORD CPythonPlayer::GetRace()
{
	return m_dwRace;
}

DWORD CPythonPlayer::__GetRaceStat()
{
	switch (GetRace())
	{
		case MAIN_RACE_WARRIOR_M:
		case MAIN_RACE_WARRIOR_W:
			return GetStatus(POINT_ST);
			break;
		case MAIN_RACE_ASSASSIN_M:
		case MAIN_RACE_ASSASSIN_W:
			return GetStatus(POINT_DX);
			break;
		case MAIN_RACE_SURA_M:
		case MAIN_RACE_SURA_W:
			return GetStatus(POINT_ST);
			break;
		case MAIN_RACE_SHAMAN_M:
		case MAIN_RACE_SHAMAN_W:
			return GetStatus(POINT_IQ);
			break;
#ifdef ENABLE_WOLFMAN_CHARACTER
		case MAIN_RACE_WOLFMAN_M:
			return GetStatus(POINT_DX);
			break;
#endif
	}
	return GetStatus(POINT_ST);
}

DWORD CPythonPlayer::__GetLevelAtk()
{
	return 2*GetStatus(POINT_LEVEL);
}

DWORD CPythonPlayer::__GetStatAtk()
{
	return (4*GetStatus(POINT_ST)+2*__GetRaceStat())/3;
}

DWORD CPythonPlayer::__GetWeaponAtk(DWORD dwWeaponPower)
{
	return 2*dwWeaponPower;
}

DWORD CPythonPlayer::__GetTotalAtk(DWORD dwWeaponPower, DWORD dwRefineBonus)
{
	DWORD dwLvAtk=__GetLevelAtk();
	DWORD dwStAtk=__GetStatAtk();

	/////

	DWORD dwWepAtk;
	DWORD dwTotalAtk;

	if (LocaleService_IsCHEONMA())
	{
		dwWepAtk = __GetWeaponAtk(dwWeaponPower+dwRefineBonus);
		dwTotalAtk = dwLvAtk+(dwStAtk+dwWepAtk)*(GetStatus(POINT_DX)+210)/300;
	}
	else
	{
		int hr = __GetHitRate();
		dwWepAtk = __GetWeaponAtk(dwWeaponPower+dwRefineBonus);
		dwTotalAtk = dwLvAtk+(dwStAtk+dwWepAtk)*hr/100;
	}

	return dwTotalAtk;
}

DWORD CPythonPlayer::__GetHitRate()
{
	int src = 0;

	if (LocaleService_IsCHEONMA())
	{
		src = GetStatus(POINT_DX);
	}
	else
	{
		src = (GetStatus(POINT_DX) * 4 + GetStatus(POINT_LEVEL) * 2)/6;
	}

	return 100*(min(150, src)+210)/300; //bug visual de stats
}

DWORD CPythonPlayer::__GetEvadeRate()
{
	return 30*(2*GetStatus(POINT_DX)+5)/(GetStatus(POINT_DX)+95);
}

void CPythonPlayer::__UpdateBattleStatus()
{
	m_playerStatus.SetPoint(POINT_NONE, 0);
	m_playerStatus.SetPoint(POINT_EVADE_RATE, __GetEvadeRate());
	m_playerStatus.SetPoint(POINT_HIT_RATE, __GetHitRate());
	m_playerStatus.SetPoint(POINT_MIN_WEP, m_dwWeaponMinPower+m_dwWeaponAddPower);
	m_playerStatus.SetPoint(POINT_MAX_WEP, m_dwWeaponMaxPower+m_dwWeaponAddPower);
	m_playerStatus.SetPoint(POINT_MIN_MAGIC_WEP, m_dwWeaponMinMagicPower+m_dwWeaponAddPower);
	m_playerStatus.SetPoint(POINT_MAX_MAGIC_WEP, m_dwWeaponMaxMagicPower+m_dwWeaponAddPower);
	m_playerStatus.SetPoint(POINT_MIN_ATK, __GetTotalAtk(m_dwWeaponMinPower, m_dwWeaponAddPower));
	m_playerStatus.SetPoint(POINT_MAX_ATK, __GetTotalAtk(m_dwWeaponMaxPower, m_dwWeaponAddPower));

#ifdef ENABLE_SASH_SYSTEM //change it with your sash define

	const TItemData * PItemData = GetItemData(TItemPos(INVENTORY, c_Costume_Slot_Sash));
	if (PItemData)
	{
		auto AbsorbedVnum = PItemData->alSockets[1];
		if (AbsorbedVnum != 0 && CItemManager::Instance().SelectItemData(AbsorbedVnum))
		{
			auto SelectedItemData = CItemManager::Instance().GetSelectedItemDataPointer();
			if (SelectedItemData->GetType() == CItemData::ITEM_TYPE_WEAPON || (SelectedItemData->GetType() == CItemData::ITEM_TYPE_COSTUME && SelectedItemData->GetSubType() == CItemData::COSTUME_WEAPON))
			{
				auto oldMinWep = m_playerStatus.GetPoint(POINT_MIN_WEP);
				auto oldMaxWep = m_playerStatus.GetPoint(POINT_MAX_WEP);

				auto oldMinMag = m_playerStatus.GetPoint(POINT_MIN_MAGIC_WEP);
				auto oldMaxMag = m_playerStatus.GetPoint(POINT_MAX_MAGIC_WEP);

				auto oldMinAtt = m_playerStatus.GetPoint(POINT_MIN_ATK);
				auto oldMaxAtt = m_playerStatus.GetPoint(POINT_MAX_ATK);

				double sashAbsorbPerc = static_cast<double>(PItemData->alSockets[0]);
				auto sashMinWep = static_cast<double>(SelectedItemData->GetValue(3) + SelectedItemData->GetValue(5)) * (sashAbsorbPerc/100.0) + 0.5;
				auto sashMaxWep = static_cast<double>(SelectedItemData->GetValue(4) + SelectedItemData->GetValue(5)) * (sashAbsorbPerc/100.0) + 0.5;
				auto sashMinMag = static_cast<double>(SelectedItemData->GetValue(1) + SelectedItemData->GetValue(5)) * (sashAbsorbPerc/100.0) + 0.5;
				auto sashMaxMag = static_cast<double>(SelectedItemData->GetValue(2) + SelectedItemData->GetValue(5)) * (sashAbsorbPerc/100.0) + 0.5;


				m_playerStatus.SetPoint(POINT_MIN_WEP, oldMinWep + sashMinWep);
				m_playerStatus.SetPoint(POINT_MAX_WEP, oldMaxWep + sashMaxWep);

				m_playerStatus.SetPoint(POINT_MIN_MAGIC_WEP, oldMinMag + sashMinMag);
				m_playerStatus.SetPoint(POINT_MAX_MAGIC_WEP, oldMaxMag + sashMaxMag);

				m_playerStatus.SetPoint(POINT_MIN_ATK, oldMinAtt + sashMinWep);
				m_playerStatus.SetPoint(POINT_MAX_ATK, oldMaxAtt + sashMaxWep);
			}
		}
	}
#endif

}

void CPythonPlayer::SetStatus(DWORD dwType, long long lValue)
{
	if (dwType >= POINT_MAX_NUM)
	{
		assert(!" CPythonPlayer::SetStatus - Strange Status Type!");
		Tracef("CPythonPlayer::SetStatus - Set Status Type Error\n");
		return;
	}

	if (dwType == POINT_LEVEL)
	{
		CInstanceBase* pkPlayer = NEW_GetMainActorPtr();

		if (pkPlayer)
			pkPlayer->UpdateTextTailLevel(lValue);
	}
#ifdef ENABLE_YOHARA_SYSTEM
	else if (dwType == POINT_CONQUEROR_LEVEL)
	{
		CInstanceBase* pkPlayer = NEW_GetMainActorPtr();

		if (pkPlayer)
		{
			pkPlayer->UpdateTextTailConquerorLevel(lValue);
		}
	}
#endif
#ifdef USE_PREMIUM_AFFECT
	else if (dwType == POINT_PREMIUM_USER)
	{
		SetTotalAutoFarmCount(lValue > 0 ? 7 : 5);
	}
#endif

	switch (dwType)
	{
		case POINT_MIN_WEP:
		case POINT_MAX_WEP:
		case POINT_MIN_ATK:
		case POINT_MAX_ATK:
		case POINT_HIT_RATE:
		case POINT_EVADE_RATE:
		case POINT_LEVEL:
		case POINT_ST:
		case POINT_DX:
		case POINT_IQ:
#ifdef ENABLE_YOHARA_SYSTEM
		case POINT_CONQUEROR_LEVEL:
		case POINT_SUNGMA_STR:
		case POINT_SUNGMA_HP:
		case POINT_SUNGMA_MOVE:
		case POINT_SUNGMA_IMMUNE:
#endif
			m_playerStatus.SetPoint(dwType, lValue);
			__UpdateBattleStatus();
			break;
		default:
			m_playerStatus.SetPoint(dwType, lValue);
			break;
	}
}

long long CPythonPlayer::GetStatus(DWORD dwType)
{
	if (dwType >= POINT_MAX_NUM)
	{
		assert(!" CPythonPlayer::GetStatus - Strange Status Type!");
		Tracef("CPythonPlayer::GetStatus - Get Status Type Error\n");
		return 0;
	}

	//TraceError("SUNGMA POINT : %d : %d", dwType, m_playerStatus.GetPoint(dwType));
	return m_playerStatus.GetPoint(dwType);
}

const char* CPythonPlayer::GetName()
{
	return m_stName.c_str();
}

void CPythonPlayer::SetName(const char *name)
{
#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	strncpy(charName, name, sizeof(charName));
#endif
	m_stName = name;
}

void CPythonPlayer::NotifyDeletingCharacterInstance(DWORD dwVID)
{
	if (m_dwMainCharacterIndex == dwVID)
		m_dwMainCharacterIndex = 0;
}

void CPythonPlayer::NotifyCharacterDead(DWORD dwVID)
{
	if (__IsSameTargetVID(dwVID))
	{
		SetTarget(0);
	}
}

void CPythonPlayer::NotifyCharacterUpdate(DWORD dwVID)
{
	if (__IsSameTargetVID(dwVID))
	{
		CInstanceBase * pMainInstance = NEW_GetMainActorPtr();
		CInstanceBase * pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(dwVID);
		if (pMainInstance && pTargetInstance)
		{
			if (!pMainInstance->IsTargetableInstance(*pTargetInstance))
			{
				SetTarget(0);
				PyCallClassMemberFunc(m_ppyGameWindow, "CloseTargetBoard", Py_BuildValue("()"));
			}
			else
			{
				PyCallClassMemberFunc(m_ppyGameWindow, "RefreshTargetBoardByVID", Py_BuildValue("(i)", dwVID));
			}
		}
	}
}

void CPythonPlayer::NotifyDeadMainCharacter()
{
	__ClearAutoAttackTargetActorID();
}

void CPythonPlayer::NotifyChangePKMode()
{
	PyCallClassMemberFunc(m_ppyGameWindow, "OnChangePKMode", Py_BuildValue("()"));
}


void CPythonPlayer::MoveItemData(TItemPos SrcCell, TItemPos DstCell)
{
	if (!SrcCell.IsValidCell() || !DstCell.IsValidCell())
		return;

	TItemData src_item(*GetItemData(SrcCell));
	TItemData dst_item(*GetItemData(DstCell));
	SetItemData(DstCell, src_item);
	SetItemData(SrcCell, dst_item);
}

const TItemData * CPythonPlayer::GetItemData(TItemPos Cell) const
{
	if (!Cell.IsValidCell())
		return NULL;

	switch (Cell.window_type)
	{
	case INVENTORY:
	case EQUIPMENT:
		return &m_playerStatus.aItem[Cell.cell];
	case DRAGON_SOUL_INVENTORY:
		return &m_playerStatus.aDSItem[Cell.cell];
#ifdef ENABLE_SWITCHBOT
	case SWITCHBOT:
		return &m_playerStatus.aSwitchbotItem[Cell.cell];
#endif
#ifdef ENABLE_SPECIAL_STORAGE
	case UPGRADE_INVENTORY:
		return &m_playerStatus.aSSUItem[Cell.cell];
	case BOOK_INVENTORY:
		return &m_playerStatus.aSSBItem[Cell.cell];
	case STONE_INVENTORY:
		return &m_playerStatus.aSSSItem[Cell.cell];
	case CHANGE_INVENTORY:
		return &m_playerStatus.aSSCItem[Cell.cell];
	case COSTUME_INVENTORY:
		return &m_playerStatus.aCostumeItem[Cell.cell];
	case CHEST_INVENTORY:
		return &m_playerStatus.aChestItem[Cell.cell];
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
	case SWITCHBOT:
		return &m_playerStatus.aSBItem[Cell.cell];
#endif		
#ifdef CHANGE_EQUIP_WORLDARD
	case CHANGE_EQUIP:
		return &m_playerStatus.aChangeEquipItem[Cell.cell];
#endif

	default:
		return NULL;
	}
}

void CPythonPlayer::SetItemData(TItemPos Cell, const TItemData & c_rkItemInst)
{
	if (!Cell.IsValidCell())
		return;

	if (c_rkItemInst.vnum != 0)
	{
		CItemData * pItemData;
		if (!CItemManager::Instance().GetItemDataPointer(c_rkItemInst.vnum, &pItemData))
		{
			TraceError("CPythonPlayer::SetItemData(window_type : %d, dwSlotIndex=%d, itemIndex=%d) - Failed to item data\n", Cell.window_type, Cell.cell, c_rkItemInst.vnum);
			return;
		}
	}

	switch (Cell.window_type)
	{
	case INVENTORY:
	case EQUIPMENT:
		m_playerStatus.aItem[Cell.cell] = c_rkItemInst;
		break;
	case DRAGON_SOUL_INVENTORY:
		m_playerStatus.aDSItem[Cell.cell] = c_rkItemInst;
		break;
#ifdef ENABLE_SWITCHBOT
	case SWITCHBOT:
		m_playerStatus.aSwitchbotItem[Cell.cell] = c_rkItemInst;
		break;
#endif
#ifdef ENABLE_SPECIAL_STORAGE
	case UPGRADE_INVENTORY:
		m_playerStatus.aSSUItem[Cell.cell] = c_rkItemInst;
		break;
	case BOOK_INVENTORY:
		m_playerStatus.aSSBItem[Cell.cell] = c_rkItemInst;
		break;
	case STONE_INVENTORY:
		m_playerStatus.aSSSItem[Cell.cell] = c_rkItemInst;
		break;
	case CHANGE_INVENTORY:
		m_playerStatus.aSSCItem[Cell.cell] = c_rkItemInst;
		break;
	case COSTUME_INVENTORY:
		m_playerStatus.aCostumeItem[Cell.cell] = c_rkItemInst;
		break;
	case CHEST_INVENTORY:
		m_playerStatus.aChestItem[Cell.cell] = c_rkItemInst;
		break;
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
	case SWITCHBOT:
		m_playerStatus.aSBItem[Cell.cell] = c_rkItemInst;
		break;
#endif
#ifdef CHANGE_EQUIP_WORLDARD
	case CHANGE_EQUIP:
		m_playerStatus.aChangeEquipItem[Cell.cell] = c_rkItemInst;
		break;
#endif
	}
}

DWORD CPythonPlayer::GetItemIndex(TItemPos Cell)
{
	if (!Cell.IsValidCell())
		return 0;

	return GetItemData(Cell)->vnum;
}

#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
void CPythonPlayer::SetAccumulateDamage(uint32_t dwVid, uint32_t dwCount)
{
	bool bContain = false;

	if (!m_hitCount.empty())
	{
		for (uint32_t i = 0; i < m_hitCount.size(); i++)
		{
			if (m_hitCount[i].dwVid == dwVid)
			{
				bContain = true;
				m_hitCount[i].dwCount = dwCount;
				break;
			}
		}
	}

	if (!bContain)
	{
		m_hitCount.push_back(THitCountInfo(dwVid, dwCount));
	}
}

uint32_t CPythonPlayer::GetAccumulateDamage(uint32_t dwVid)
{
	uint32_t dwCount = 0;

	for (uint32_t i = 0; i < m_hitCount.size(); i++)
	{
		if (m_hitCount[i].dwVid == dwVid)
		{
			dwCount = m_hitCount[i].dwCount;
			break;
		}
	}

	return dwCount;
}
#endif

DWORD CPythonPlayer::GetItemFlags(TItemPos Cell)
{
	if (!Cell.IsValidCell())
		return 0;
	const TItemData * pItem = GetItemData(Cell);
	assert (pItem != NULL);
	return pItem->flags;
}

DWORD CPythonPlayer::GetItemAntiFlags(TItemPos Cell)
{
	if (!Cell.IsValidCell())
		return 0;

	const TItemData * pItem = GetItemData(Cell);
	assert(pItem != NULL);
	return pItem->anti_flags;
}

BYTE CPythonPlayer::GetItemTypeBySlot(TItemPos Cell)
{
	if (!Cell.IsValidCell())
		return 0;

	CItemData * pItemDataPtr = NULL;
	if (CItemManager::Instance().GetItemDataPointer(GetItemIndex(Cell), &pItemDataPtr))
		return pItemDataPtr->GetType();
	else
	{
		TraceError("FAILED\t\tCPythonPlayer::GetItemTypeBySlot()\t\tCell(%d, %d) item is null", Cell.window_type, Cell.cell);
		return 0;
	}
}

BYTE CPythonPlayer::GetItemSubTypeBySlot(TItemPos Cell)
{
	if (!Cell.IsValidCell())
		return 0;

	CItemData * pItemDataPtr = NULL;
	if (CItemManager::Instance().GetItemDataPointer(GetItemIndex(Cell), &pItemDataPtr))
		return pItemDataPtr->GetSubType();
	else
	{
		TraceError("FAILED\t\tCPythonPlayer::GetItemSubTypeBySlot()\t\tCell(%d, %d) item is null", Cell.window_type, Cell.cell);
		return 0;
	}
}

DWORD CPythonPlayer::GetItemCount(TItemPos Cell)
{
	if (!Cell.IsValidCell())
		return 0;
	const TItemData * pItem = GetItemData(Cell);
	if (pItem == NULL)
		return 0;
	else
		return pItem->count;
}

DWORD CPythonPlayer::GetItemCountByVnum(DWORD dwVnum)
{
	DWORD dwCount = 0;

	for (int i = 0; i < c_Inventory_Count; ++i)
	{
		auto& c_rItemData = m_playerStatus.aItem[i];
		if (c_rItemData.vnum == dwVnum)
		{
			dwCount += c_rItemData.count;
		}
	}

#ifdef ENABLE_SPECIAL_STORAGE
	for (int i = 0; i < c_Special_ItemSlot_Count; ++i)
	{
		auto& c_rItemData = m_playerStatus.aSSUItem[i];
		if (c_rItemData.vnum == dwVnum)
		{
			dwCount += c_rItemData.count;
		}
	}

	for (int i = 0; i < c_Special_ItemSlot_Count; ++i)
	{
		auto& c_rItemData = m_playerStatus.aSSBItem[i];
		if (c_rItemData.vnum == dwVnum)
		{
			dwCount += c_rItemData.count;
		}
	}

	for (int i = 0; i < c_Special_ItemSlot_Count; ++i)
	{
		auto& c_rItemData = m_playerStatus.aSSSItem[i];
		if (c_rItemData.vnum == dwVnum)
		{
			dwCount += c_rItemData.count;
		}
	}

	for (int i = 0; i < c_Special_ItemSlot_Count; ++i)
	{
		auto& c_rItemData = m_playerStatus.aSSCItem[i];
		if (c_rItemData.vnum == dwVnum)
		{
			dwCount += c_rItemData.count;
		}
	}

	for (int i = 0; i < c_Special_ItemSlot_Count; ++i)
	{
		auto& c_rItemData = m_playerStatus.aCostumeItem[i];
		if (c_rItemData.vnum == dwVnum)
		{
			dwCount += c_rItemData.count;
		}
	}

	for (int i = 0; i < c_Special_ItemSlot_Count; ++i)
	{
		auto& c_rItemData = m_playerStatus.aChestItem[i];
		if (c_rItemData.vnum == dwVnum)
		{
			dwCount += c_rItemData.count;
		}
	}
#endif

	return dwCount;
}

#ifdef __AUTO_SKILL_READER__
DWORD CPythonPlayer::GetItemCountByVnumNew(DWORD dwVnum, int socket0)
{
	DWORD dwCount = 0;
	for (int i = 0; i < c_Inventory_Count; ++i)
	{
		//const TItemData& c_rItemData = m_playerStatus.aItem[i];
		const TItemData& c_rItemData = m_playerStatus.aSSBItem[i];
		if (c_rItemData.vnum == dwVnum && c_rItemData.alSockets[0] == socket0)
			dwCount += c_rItemData.count;
	}
	return dwCount;
}
int CPythonPlayer::GetSkillLevelNew(DWORD dwSkillIndex)
{
	DWORD dwSlotIndex;
	if (!GetSkillSlotIndex(dwSkillIndex, &dwSlotIndex))
		return 0;

	if (dwSlotIndex >= SKILL_MAX_NUM)
		return 0;
	return m_playerStatus.aSkill[dwSlotIndex].iLevel;
}

int CPythonPlayer::GetSkillGradeNew(DWORD dwSkillIndex)
{
	DWORD dwSlotIndex;
	if (!GetSkillSlotIndex(dwSkillIndex, &dwSlotIndex))
		return 0;

	if (dwSlotIndex >= SKILL_MAX_NUM)
		return 0;
	return m_playerStatus.aSkill[dwSlotIndex].iGrade;
}
#endif

#ifdef ENABLE_SPECIAL_STORAGE
DWORD CPythonPlayer::GetItemCountByTT(DWORD dwVnum)
{
	DWORD dwCount = 0;

	for (int i = 0; i < c_Special_ItemSlot_Count; ++i)
	{
		const TItemData & c_rItemData = m_playerStatus.aSSUItem[i];
		if (c_rItemData.vnum == dwVnum)
		{
			dwCount += c_rItemData.count;
		}
	}

	return dwCount;
}

DWORD CPythonPlayer::GetSpecialBookItemCount(DWORD dwVnum)
{
	DWORD dwCount = 0;

	for (int i = 0; i < c_Special_ItemSlot_Count; ++i)
	{
		const TItemData& c_rItemData = m_playerStatus.aSSBItem[i];
		if (c_rItemData.vnum == dwVnum)
		{
			dwCount += c_rItemData.count;
		}
	}

	return dwCount;
}
DWORD CPythonPlayer::GetSpecialChestItemCount(DWORD dwVnum)
{
	DWORD dwCount = 0;

	for (int i = 0; i < c_Special_ItemSlot_Count; ++i)
	{
		const TItemData& c_rItemData = m_playerStatus.aChestItem[i];
		if (c_rItemData.vnum == dwVnum)
		{
			dwCount += c_rItemData.count;
		}
	}

	return dwCount;
}
#endif
DWORD CPythonPlayer::GetItemMetinSocket(TItemPos Cell, DWORD dwMetinSocketIndex)
{
	if (!Cell.IsValidCell())
		return 0;

	if (dwMetinSocketIndex >= ITEM_SOCKET_SLOT_MAX_NUM)
		return 0;

	return GetItemData(Cell)->alSockets[dwMetinSocketIndex];
}

void CPythonPlayer::GetItemAttribute(TItemPos Cell, DWORD dwAttrSlotIndex, BYTE * pbyType, short * psValue)
{
	*pbyType = 0;
	*psValue = 0;

	if (!Cell.IsValidCell())
		return;

	if (dwAttrSlotIndex >= ITEM_ATTRIBUTE_SLOT_MAX_NUM)
		return;

	*pbyType = GetItemData(Cell)->aAttr[dwAttrSlotIndex].bType;
	*psValue = GetItemData(Cell)->aAttr[dwAttrSlotIndex].sValue;
}

#ifdef ENABLE_YOHARA_SYSTEM
void CPythonPlayer::SetSungmaAttribute(TItemPos Cell, DWORD dwSungmaAttrIndex, BYTE wType, short sValue)	//@fixme436
{
	if (!Cell.IsValidCell())
		return;
	if (dwSungmaAttrIndex >= APPLY_RANDOM_SLOT_MAX_NUM)
		return;

	(const_cast <TItemData*>(GetItemData(Cell)))->aApplyRandom[dwSungmaAttrIndex].bType = wType;
	(const_cast <TItemData*>(GetItemData(Cell)))->aApplyRandom[dwSungmaAttrIndex].sValue = sValue;
}

void CPythonPlayer::GetItemApplyRandom(TItemPos Cell, DWORD dwSungmaAttrIndex, BYTE* pwType, short* psValue)
{
	*pwType = 0;
	*psValue = 0;

	if (!Cell.IsValidCell())
		return;

	if (dwSungmaAttrIndex >= APPLY_RANDOM_SLOT_MAX_NUM)
		return;

	*pwType = GetItemData(Cell)->aApplyRandom[dwSungmaAttrIndex].bType;
	*psValue = GetItemData(Cell)->aApplyRandom[dwSungmaAttrIndex].sValue;
}
#endif

void CPythonPlayer::SetItemCount(TItemPos Cell, DWORD byCount)
{
	if (!Cell.IsValidCell())
		return;

	(const_cast <TItemData *>(GetItemData(Cell)))->count = byCount;
	PyCallClassMemberFunc(m_ppyGameWindow, "RefreshInventory", Py_BuildValue("()"));
}
#ifdef ENABLE_SOULBIND_SYSTEM
void CPythonPlayer::SetItemBind(TItemPos Cell, long lBindTime)
{
	if (!Cell.IsValidCell())
		return;
	
	(const_cast <TItemData *>(GetItemData(Cell)))->bind = lBindTime;
	PyCallClassMemberFunc(m_ppyGameWindow, "RefreshInventory", Py_BuildValue("()"));
}

long CPythonPlayer::GetItemBind(TItemPos Cell)
{
	if (!Cell.IsValidCell())
		return 0;
	
	const TItemData * pItem = GetItemData(Cell);
	if (pItem == NULL)
		return 0;
	else
		return pItem->bind;
}
#endif
void CPythonPlayer::SetItemMetinSocket(TItemPos Cell, DWORD dwMetinSocketIndex, DWORD dwMetinNumber)
{
	if (!Cell.IsValidCell())
		return;
	if (dwMetinSocketIndex >= ITEM_SOCKET_SLOT_MAX_NUM)
		return;

	(const_cast <TItemData *>(GetItemData(Cell)))->alSockets[dwMetinSocketIndex] = dwMetinNumber;
}

void CPythonPlayer::SetItemAttribute(TItemPos Cell, DWORD dwAttrIndex, BYTE byType, short sValue)
{
	if (!Cell.IsValidCell())
		return;
	if (dwAttrIndex >= ITEM_ATTRIBUTE_SLOT_MAX_NUM)
		return;

	(const_cast <TItemData *>(GetItemData(Cell)))->aAttr[dwAttrIndex].bType = byType;
	(const_cast <TItemData *>(GetItemData(Cell)))->aAttr[dwAttrIndex].sValue = sValue;
}

int CPythonPlayer::GetQuickPage()
{
	return m_playerStatus.lQuickPageIndex;
}

void CPythonPlayer::SetQuickPage(int nQuickPageIndex)
{
	if (nQuickPageIndex<0)
		m_playerStatus.lQuickPageIndex=QUICKSLOT_MAX_LINE+nQuickPageIndex;
	else if (nQuickPageIndex>=QUICKSLOT_MAX_LINE)
		m_playerStatus.lQuickPageIndex=nQuickPageIndex%QUICKSLOT_MAX_LINE;
	else
		m_playerStatus.lQuickPageIndex=nQuickPageIndex;

	PyCallClassMemberFunc(m_ppyGameWindow, "RefreshInventory", Py_BuildValue("()"));
}

DWORD	CPythonPlayer::LocalQuickSlotIndexToGlobalQuickSlotIndex(DWORD dwLocalSlotIndex)
{
	return m_playerStatus.lQuickPageIndex*QUICKSLOT_MAX_COUNT_PER_LINE+dwLocalSlotIndex;
}

void	CPythonPlayer::GetGlobalQuickSlotData(DWORD dwGlobalSlotIndex, DWORD* pdwWndType, DWORD* pdwWndItemPos)
{
	TQuickSlot& rkQuickSlot=__RefGlobalQuickSlot(dwGlobalSlotIndex);
	*pdwWndType=rkQuickSlot.Type;
	*pdwWndItemPos=rkQuickSlot.Position;
}

void	CPythonPlayer::GetLocalQuickSlotData(DWORD dwSlotPos, DWORD* pdwWndType, DWORD* pdwWndItemPos)
{
	TQuickSlot& rkQuickSlot=__RefLocalQuickSlot(dwSlotPos);
	*pdwWndType=rkQuickSlot.Type;
	*pdwWndItemPos=rkQuickSlot.Position;
}

TQuickSlot & CPythonPlayer::__RefLocalQuickSlot(int SlotIndex)
{
	return __RefGlobalQuickSlot(LocalQuickSlotIndexToGlobalQuickSlotIndex(SlotIndex));
}

TQuickSlot & CPythonPlayer::__RefGlobalQuickSlot(int SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= QUICKSLOT_MAX_NUM)
	{
		static TQuickSlot s_kQuickSlot;
		s_kQuickSlot.Type = 0;
		s_kQuickSlot.Position = 0;
		return s_kQuickSlot;
	}

	return m_playerStatus.aQuickSlot[SlotIndex];
}

void CPythonPlayer::RemoveQuickSlotByValue(int iType, int iPosition)
{
	for (BYTE i = 0; i < QUICKSLOT_MAX_NUM; ++i)
	{
		if (iType == m_playerStatus.aQuickSlot[i].Type)
			if (iPosition == m_playerStatus.aQuickSlot[i].Position)
				CPythonNetworkStream::Instance().SendQuickSlotDelPacket(i);
	}
}

char CPythonPlayer::IsItem(TItemPos Cell)
{
	if (!Cell.IsValidCell())
		return 0;

	return 0 != GetItemData(Cell)->vnum;
}

void CPythonPlayer::RequestMoveGlobalQuickSlotToLocalQuickSlot(DWORD dwGlobalSrcSlotIndex, DWORD dwLocalDstSlotIndex)
{
	//DWORD dwGlobalSrcSlotIndex=LocalQuickSlotIndexToGlobalQuickSlotIndex(dwLocalSrcSlotIndex);
	DWORD dwGlobalDstSlotIndex=LocalQuickSlotIndexToGlobalQuickSlotIndex(dwLocalDstSlotIndex);

	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SendQuickSlotMovePacket((BYTE) dwGlobalSrcSlotIndex, (BYTE)dwGlobalDstSlotIndex);
}

void CPythonPlayer::RequestAddLocalQuickSlot(DWORD dwLocalSlotIndex, DWORD dwWndType, WORD wWndItemPos)
{
	if (dwLocalSlotIndex>=QUICKSLOT_MAX_COUNT_PER_LINE)
		return;

	DWORD dwGlobalSlotIndex=LocalQuickSlotIndexToGlobalQuickSlotIndex(dwLocalSlotIndex);

	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SendQuickSlotAddPacket((BYTE)dwGlobalSlotIndex, (BYTE)dwWndType, wWndItemPos);
}

void CPythonPlayer::RequestAddToEmptyLocalQuickSlot(DWORD dwWndType, WORD wWndItemPos)
{
	for (int i = 0; i < QUICKSLOT_MAX_COUNT_PER_LINE; ++i)
	{
		TQuickSlot& rkQuickSlot=__RefLocalQuickSlot(i);

		if (0 == rkQuickSlot.Type)
		{
			DWORD dwGlobalQuickSlotIndex=LocalQuickSlotIndexToGlobalQuickSlotIndex(i);
			CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
			rkNetStream.SendQuickSlotAddPacket((BYTE)dwGlobalQuickSlotIndex, (BYTE)dwWndType, wWndItemPos);
			return;
		}
	}

}

void CPythonPlayer::RequestDeleteGlobalQuickSlot(DWORD dwGlobalSlotIndex)
{
	if (dwGlobalSlotIndex>=QUICKSLOT_MAX_COUNT)
		return;

	//if (dwLocalSlotIndex>=QUICKSLOT_MAX_SLOT_PER_LINE)
	//	return;

	//DWORD dwGlobalSlotIndex=LocalQuickSlotIndexToGlobalQuickSlotIndex(dwLocalSlotIndex);

	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SendQuickSlotDelPacket((BYTE)dwGlobalSlotIndex);
}

void CPythonPlayer::RequestUseLocalQuickSlot(DWORD dwLocalSlotIndex)
{
	if (dwLocalSlotIndex>=QUICKSLOT_MAX_COUNT_PER_LINE)
		return;

	DWORD dwRegisteredType;
	DWORD dwRegisteredItemPos;
	GetLocalQuickSlotData(dwLocalSlotIndex, &dwRegisteredType, &dwRegisteredItemPos);

	switch (dwRegisteredType)
	{
		case SLOT_TYPE_INVENTORY:
		{
			CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
			rkNetStream.SendItemUsePacket(TItemPos(INVENTORY, (WORD)dwRegisteredItemPos));
			break;
		}
		case SLOT_TYPE_SKILL:
		{
			ClickSkillSlot(dwRegisteredItemPos);
			break;
		}
		case SLOT_TYPE_EMOTION:
		{
			PyCallClassMemberFunc(m_ppyGameWindow, "BINARY_ActEmotion", Py_BuildValue("(i)", dwRegisteredItemPos));
			break;
		}
	}
}

void CPythonPlayer::AddQuickSlot(int QuickSlotIndex, char IconType, char IconPosition)
{
	if (QuickSlotIndex < 0 || QuickSlotIndex >= QUICKSLOT_MAX_NUM)
		return;

	m_playerStatus.aQuickSlot[QuickSlotIndex].Type = IconType;
	m_playerStatus.aQuickSlot[QuickSlotIndex].Position = IconPosition;
}

void CPythonPlayer::DeleteQuickSlot(int QuickSlotIndex)
{
	if (QuickSlotIndex < 0 || QuickSlotIndex >= QUICKSLOT_MAX_NUM)
		return;

	m_playerStatus.aQuickSlot[QuickSlotIndex].Type = 0;
	m_playerStatus.aQuickSlot[QuickSlotIndex].Position = 0;
}

void CPythonPlayer::MoveQuickSlot(int Source, int Target)
{
	if (Source < 0 || Source >= QUICKSLOT_MAX_NUM)
		return;

	if (Target < 0 || Target >= QUICKSLOT_MAX_NUM)
		return;

	TQuickSlot& rkSrcSlot=__RefGlobalQuickSlot(Source);
	TQuickSlot& rkDstSlot=__RefGlobalQuickSlot(Target);

	std::swap(rkSrcSlot, rkDstSlot);
}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
bool CPythonPlayer::IsBeltInventorySlot(TItemPos Cell)
{
	return Cell.IsBeltInventoryCell();
}
#endif

bool CPythonPlayer::IsInventorySlot(TItemPos Cell)
{
	return !Cell.IsEquipCell() && Cell.IsValidCell();
}

bool CPythonPlayer::IsEquipmentSlot(TItemPos Cell)
{
	return Cell.IsEquipCell();
}

bool CPythonPlayer::IsEquipItemInSlot(TItemPos Cell)
{
	if (!Cell.IsEquipCell())
	{
		return false;
	}

	const TItemData * pData = GetItemData(Cell);

	if (NULL == pData)
	{
		return false;
	}

	DWORD dwItemIndex = pData->vnum;

	CItemManager::Instance().SelectItemData(dwItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
	{
		TraceError("Failed to find ItemData - CPythonPlayer::IsEquipItem(window_type=%d, iSlotindex=%d)\n", Cell.window_type, Cell.cell);
		return false;
	}

	return pItemData->IsEquipment() ? true : false;
}


void CPythonPlayer::SetSkill(DWORD dwSlotIndex, DWORD dwSkillIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return;

	m_playerStatus.aSkill[dwSlotIndex].dwIndex = dwSkillIndex;
	m_skillSlotDict[dwSkillIndex] = dwSlotIndex;
}

int CPythonPlayer::GetSkillIndex(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return 0;

	return m_playerStatus.aSkill[dwSlotIndex].dwIndex;
}

bool CPythonPlayer::GetSkillSlotIndex(DWORD dwSkillIndex, DWORD* pdwSlotIndex)
{
	std::map<DWORD, DWORD>::iterator f=m_skillSlotDict.find(dwSkillIndex);
	if (m_skillSlotDict.end()==f)
	{
		return false;
	}

	*pdwSlotIndex=f->second;

	return true;
}

int CPythonPlayer::GetSkillGrade(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return 0;

	return m_playerStatus.aSkill[dwSlotIndex].iGrade;
}

int CPythonPlayer::GetSkillLevel(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return 0;

	return m_playerStatus.aSkill[dwSlotIndex].iLevel;
}

float CPythonPlayer::GetSkillCurrentEfficientPercentage(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return 0;

	return m_playerStatus.aSkill[dwSlotIndex].fcurEfficientPercentage;
}

float CPythonPlayer::GetSkillNextEfficientPercentage(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return 0;

	return m_playerStatus.aSkill[dwSlotIndex].fnextEfficientPercentage;
}

void CPythonPlayer::SetSkillLevel(DWORD dwSlotIndex, DWORD dwSkillLevel)
{
	assert(!"CPythonPlayer::SetSkillLevel - Don't use this function");
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return;

	m_playerStatus.aSkill[dwSlotIndex].iGrade = -1;
	m_playerStatus.aSkill[dwSlotIndex].iLevel = dwSkillLevel;
}

void CPythonPlayer::SetSkillLevel_(DWORD dwSkillIndex, DWORD dwSkillGrade, DWORD dwSkillLevel)
{
	DWORD dwSlotIndex;
	if (!GetSkillSlotIndex(dwSkillIndex, &dwSlotIndex))
		return;

	if (dwSlotIndex >= SKILL_MAX_NUM)
		return;

	switch (dwSkillGrade)
	{
		case 0:
			m_playerStatus.aSkill[dwSlotIndex].iGrade = dwSkillGrade;
			m_playerStatus.aSkill[dwSlotIndex].iLevel = dwSkillLevel;
			break;
		case 1:
			m_playerStatus.aSkill[dwSlotIndex].iGrade = dwSkillGrade;
			m_playerStatus.aSkill[dwSlotIndex].iLevel = dwSkillLevel-20+1;
			break;
		case 2:
			m_playerStatus.aSkill[dwSlotIndex].iGrade = dwSkillGrade;
			m_playerStatus.aSkill[dwSlotIndex].iLevel = dwSkillLevel-30+1;
			break;
		case 3:
			m_playerStatus.aSkill[dwSlotIndex].iGrade = dwSkillGrade;
			m_playerStatus.aSkill[dwSlotIndex].iLevel = dwSkillLevel-40+1;
			break;
	}

	const DWORD SKILL_MAX_LEVEL = 40;





	if (dwSkillLevel>SKILL_MAX_LEVEL)
	{
		m_playerStatus.aSkill[dwSlotIndex].fcurEfficientPercentage = 0.0f;
		m_playerStatus.aSkill[dwSlotIndex].fnextEfficientPercentage = 0.0f;

		TraceError("CPythonPlayer::SetSkillLevel(SlotIndex=%d, SkillLevel=%d)", dwSlotIndex, dwSkillLevel);
		return;
	}

	m_playerStatus.aSkill[dwSlotIndex].fcurEfficientPercentage	= LocaleService_GetSkillPower(dwSkillLevel)/100.0f;
	m_playerStatus.aSkill[dwSlotIndex].fnextEfficientPercentage = LocaleService_GetSkillPower(dwSkillLevel+1)/100.0f;

}

void CPythonPlayer::SetSkillCoolTime(DWORD dwSkillIndex)
{
	DWORD dwSlotIndex;
	if (!GetSkillSlotIndex(dwSkillIndex, &dwSlotIndex))
	{
		Tracenf("CPythonPlayer::SetSkillCoolTime(dwSkillIndex=%d) - FIND SLOT ERROR", dwSkillIndex);
		return;
	}

	if (dwSlotIndex>=SKILL_MAX_NUM)
	{
		Tracenf("CPythonPlayer::SetSkillCoolTime(dwSkillIndex=%d) - dwSlotIndex=%d/%d OUT OF RANGE", dwSkillIndex, dwSlotIndex, SKILL_MAX_NUM);
		return;
	}

	m_playerStatus.aSkill[dwSlotIndex].isCoolTime=true;
}

void CPythonPlayer::EndSkillCoolTime(DWORD dwSkillIndex)
{
	DWORD dwSlotIndex;
	if (!GetSkillSlotIndex(dwSkillIndex, &dwSlotIndex))
	{
		Tracenf("CPythonPlayer::EndSkillCoolTime(dwSkillIndex=%d) - FIND SLOT ERROR", dwSkillIndex);
		return;
	}

	if (dwSlotIndex>=SKILL_MAX_NUM)
	{
		Tracenf("CPythonPlayer::EndSkillCoolTime(dwSkillIndex=%d) - dwSlotIndex=%d/%d OUT OF RANGE", dwSkillIndex, dwSlotIndex, SKILL_MAX_NUM);
		return;
	}

	m_playerStatus.aSkill[dwSlotIndex].isCoolTime=false;
}

float CPythonPlayer::GetSkillCoolTime(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return 0.0f;

	return m_playerStatus.aSkill[dwSlotIndex].fCoolTime;
}

float CPythonPlayer::GetSkillElapsedCoolTime(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return 0.0f;

	return CTimer::Instance().GetCurrentSecond() - m_playerStatus.aSkill[dwSlotIndex].fLastUsedTime;
}

void CPythonPlayer::__ActivateSkillSlot(DWORD dwSlotIndex)
{
	if (dwSlotIndex>=SKILL_MAX_NUM)
	{
		Tracenf("CPythonPlayer::ActivavteSkill(dwSlotIndex=%d/%d) - OUT OF RANGE", dwSlotIndex, SKILL_MAX_NUM);
		return;
	}

	m_playerStatus.aSkill[dwSlotIndex].bActive = TRUE;
	PyCallClassMemberFunc(m_ppyGameWindow, "ActivateSkillSlot", Py_BuildValue("(i)", dwSlotIndex));
}

void CPythonPlayer::__DeactivateSkillSlot(DWORD dwSlotIndex)
{
	if (dwSlotIndex>=SKILL_MAX_NUM)
	{
		Tracenf("CPythonPlayer::DeactivavteSkill(dwSlotIndex=%d/%d) - OUT OF RANGE", dwSlotIndex, SKILL_MAX_NUM);
		return;
	}

	m_playerStatus.aSkill[dwSlotIndex].bActive = FALSE;
	PyCallClassMemberFunc(m_ppyGameWindow, "DeactivateSkillSlot", Py_BuildValue("(i)", dwSlotIndex));
}

BOOL CPythonPlayer::IsSkillCoolTime(DWORD dwSlotIndex)
{
	if (!__CheckRestSkillCoolTime(dwSlotIndex))
		return FALSE;

	return TRUE;
}

BOOL CPythonPlayer::IsSkillActive(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return FALSE;

	return m_playerStatus.aSkill[dwSlotIndex].bActive;
}

BOOL CPythonPlayer::IsToggleSkill(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return FALSE;

	DWORD dwSkillIndex = m_playerStatus.aSkill[dwSlotIndex].dwIndex;

	CPythonSkill::TSkillData * pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(dwSkillIndex, &pSkillData))
		return FALSE;

	return pSkillData->IsToggleSkill();
}

void CPythonPlayer::SetPlayTime(DWORD dwPlayTime)
{
	m_dwPlayTime = dwPlayTime;
}

DWORD CPythonPlayer::GetPlayTime()
{
	return m_dwPlayTime;
}

/*
void CPythonPlayer::SendClickItemPacket(DWORD dwIID)
{
	if (IsObserverMode())
		return;

	static DWORD s_dwNextTCPTime = 0;

	DWORD dwCurTime = ELTimer_GetMSec();

	if (dwCurTime >= s_dwNextTCPTime)
	{
		s_dwNextTCPTime = dwCurTime + 100;

		const char * c_szOwnerName;
		if (!CPythonItem::Instance().GetOwnership(dwIID, &c_szOwnerName))
			return;

		if (strlen(c_szOwnerName) > 0)
		if (0 != strcmp(c_szOwnerName, GetName()))
		{
			CItemData * pItemData;
			if (!CItemManager::Instance().GetItemDataPointer(CPythonItem::Instance().GetVirtualNumberOfGroundItem(dwIID), &pItemData))
			{
				Tracenf("CPythonPlayer::SendClickItemPacket(dwIID=%d) : Non-exist item.", dwIID);
				return;
			}
			if (!IsPartyMemberByName(c_szOwnerName) || pItemData->IsAntiFlag(ITEM_ANTIFLAG_DROP | ITEM_ANTIFLAG_GIVE))
			{
				PyCallClassMemberFunc(m_ppyGameWindow, "OnCannotPickItem", Py_BuildValue("()"));
				return;
			}
		}

		CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
		rkNetStream.SendItemPickUpPacket(dwIID);
	}
}
*/

void CPythonPlayer::SendClickItemPacket(const DWORD vid)
{
	static DWORD s_dwNextClickItemTime = 0;
	const auto timenow = ELTimer_GetMSec();
	if (timenow < s_dwNextClickItemTime)
	{
		return;
	}

	s_dwNextClickItemTime = timenow + 100;

	if (IsObserverMode())
	{
		return;
	}

	auto pInstMain = NEW_GetMainActorPtr();
	if (!pInstMain || pInstMain->IsDead())
	{
		return;
	}

	auto& pyItemMgr = CPythonItem::Instance();

	const char* c_szOwnerName;
	if (!pyItemMgr.GetOwnership(vid, &c_szOwnerName))
	{
		return;
	}

	bool can = (strlen(c_szOwnerName) == 0) ? true : false;
	if (!can)
	{
		if (strcmp(c_szOwnerName, GetName()) == 0 || IsPartyMemberByName(c_szOwnerName))
		{
			can = true;
		}
		else
		{
			PyCallClassMemberFunc(m_ppyGameWindow, "OnCannotPickItem", Py_BuildValue("()"));
			return;
		}
	}

	if (can)
	{
		auto& itemMgr = CItemManager::Instance();

		CItemData* pItemData;
		if (!itemMgr.GetItemDataPointer(CPythonItem::Instance().GetVirtualNumberOfGroundItem(vid), &pItemData))
		{
			Tracenf("CPythonPlayer::SendClickItemPacket(vid = %u) : Non-exist item.", vid);
			return;
		}

/*
		if (pItemData->IsAntiFlag(ITEM_ANTIFLAG_GET))// pItemData->IsAntiFlag(ITEM_ANTIFLAG_DROP | ITEM_ANTIFLAG_GIVE))
		{
			PyCallClassMemberFunc(m_ppyGameWindow, "OnCannotPickItem", Py_BuildValue("()"));
			return;
		}
*/

		auto& netMgr = CPythonNetworkStream::Instance();
		netMgr.SendItemPickUpPacket(vid);

		s_dwNextClickItemTime = timenow + 100;
	}
}

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
void CPythonPlayer::SendPickUPItemPacket(std::vector<DWORD> itemVIDVector)
{
	if (IsObserverMode())
		return;

	static uint32_t s_dwNextTCPTime = 0;
	uint32_t dwCurTime = ELTimer_GetMSec();
	std::vector<DWORD> itemVector;
	if (dwCurTime >= s_dwNextTCPTime)
	{
		s_dwNextTCPTime = dwCurTime + 500;
		for (size_t i = 0; i < itemVIDVector.size(); i++)
		{
			DWORD dwIID = itemVIDVector[i];
			const char * c_szOwnerName;
			if (!CPythonItem::Instance().GetOwnership(dwIID, &c_szOwnerName))
				continue;

			if (strlen(c_szOwnerName) > 0)
			{
				if (0 != strcmp(c_szOwnerName, GetName()))
				{
					CItemData * pItemData;
					if (!CItemManager::Instance().GetItemDataPointer(CPythonItem::Instance().GetVirtualNumberOfGroundItem(dwIID), &pItemData)){
						continue;
					}
					if (!IsPartyMemberByName(c_szOwnerName) || pItemData->IsAntiFlag(ITEM_ANTIFLAG_DROP | ITEM_ANTIFLAG_GIVE)){
						continue;
					}
				}
			}
			itemVector.push_back(dwIID);
		}
		if (itemVector.empty())
			return;

		CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
		rkNetStream.SendItemsPickUpPacket(itemVector);
	}
}
#endif

void CPythonPlayer::__SendClickActorPacket(CInstanceBase& rkInstVictim)
{
	// 말을 타고 광산을 캐는 것에 대한 예외 처리
	CInstanceBase* pkInstMain=NEW_GetMainActorPtr();
	if (pkInstMain)
	if (pkInstMain->IsHoldingPickAxe())
	if (pkInstMain->IsMountingHorse())
	if (rkInstVictim.IsResource())
	{
		PyCallClassMemberFunc(m_ppyGameWindow, "OnCannotMining", Py_BuildValue("()"));
		return;
	}

	static DWORD s_dwNextTCPTime = 0;

	DWORD dwCurTime=ELTimer_GetMSec();

	if (dwCurTime >= s_dwNextTCPTime)
	{
		s_dwNextTCPTime=dwCurTime+1000;

		CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();

		DWORD dwVictimVID=rkInstVictim.GetVirtualID();
		rkNetStream.SendOnClickPacket(dwVictimVID);
	}
}

void CPythonPlayer::ActEmotion(DWORD dwEmotionID)
{
	CInstanceBase * pkInstTarget = __GetAliveTargetInstancePtr();
	if (!pkInstTarget)
	{
		PyCallClassMemberFunc(m_ppyGameWindow, "OnCannotShotError", Py_BuildValue("(is)", GetMainCharacterIndex(), "NEED_TARGET"));
		return;
	}

	CPythonNetworkStream::Instance().SendChatPacket(_getf("/kiss %s", pkInstTarget->GetNameString()));
}

void CPythonPlayer::StartEmotionProcess()
{
	__ClearReservedAction();
	__ClearAutoAttackTargetActorID();

	m_bisProcessingEmotion = TRUE;
}

void CPythonPlayer::EndEmotionProcess()
{
	m_bisProcessingEmotion = FALSE;
}

BOOL CPythonPlayer::__IsProcessingEmotion()
{
	return m_bisProcessingEmotion;
}

// Dungeon
void CPythonPlayer::SetDungeonDestinationPosition(int ix, int iy)
{
	m_isDestPosition = TRUE;
	m_ixDestPos = ix;
	m_iyDestPos = iy;

	AlarmHaveToGo();
}

void CPythonPlayer::AlarmHaveToGo()
{
	m_iLastAlarmTime = CTimer::Instance().GetCurrentMillisecond();

	/////

	CInstanceBase * pInstance = NEW_GetMainActorPtr();
	if (!pInstance)
		return;

	TPixelPosition PixelPosition;
	pInstance->NEW_GetPixelPosition(&PixelPosition);

	float fAngle = GetDegreeFromPosition2(PixelPosition.x, PixelPosition.y, float(m_ixDestPos), float(m_iyDestPos));
	fAngle = fmod(540.0f - fAngle, 360.0f);
	D3DXVECTOR3 v3Rotation(0.0f, 0.0f, fAngle);

	PixelPosition.y *= -1.0f;

	CEffectManager::Instance().RegisterEffect("d:/ymir work/effect/etc/compass/appear_middle.mse");
	CEffectManager::Instance().CreateEffect("d:/ymir work/effect/etc/compass/appear_middle.mse", PixelPosition, v3Rotation);
}

// Party
void CPythonPlayer::ExitParty()
{
	m_PartyMemberMap.clear();

	CPythonCharacterManager::Instance().RefreshAllPCTextTail();
}

void CPythonPlayer::AppendPartyMember(DWORD dwPID, const char * c_szName)
{
	m_PartyMemberMap.insert(std::make_pair(dwPID, TPartyMemberInfo(dwPID, c_szName)));
}

void CPythonPlayer::LinkPartyMember(DWORD dwPID, DWORD dwVID)
{
	TPartyMemberInfo * pPartyMemberInfo;
	if (!GetPartyMemberPtr(dwPID, &pPartyMemberInfo))
	{
		TraceError(" CPythonPlayer::LinkPartyMember(dwPID=%d, dwVID=%d) - Failed to find party member", dwPID, dwVID);
		return;
	}

	pPartyMemberInfo->dwVID = dwVID;

	CInstanceBase * pInstance = NEW_FindActorPtr(dwVID);
	if (pInstance)
		pInstance->RefreshTextTail();
}

void CPythonPlayer::UnlinkPartyMember(DWORD dwPID)
{
	TPartyMemberInfo * pPartyMemberInfo;
	if (!GetPartyMemberPtr(dwPID, &pPartyMemberInfo))
	{
		TraceError(" CPythonPlayer::UnlinkPartyMember(dwPID=%d) - Failed to find party member", dwPID);
		return;
	}

	pPartyMemberInfo->dwVID = 0;
}

void CPythonPlayer::UpdatePartyMemberInfo(DWORD dwPID, BYTE byState, BYTE byHPPercentage)
{
	TPartyMemberInfo * pPartyMemberInfo;
	if (!GetPartyMemberPtr(dwPID, &pPartyMemberInfo))
	{
		TraceError(" CPythonPlayer::UpdatePartyMemberInfo(dwPID=%d, byState=%d, byHPPercentage=%d) - Failed to find character", dwPID, byState, byHPPercentage);
		return;
	}

	pPartyMemberInfo->byState = byState;
	pPartyMemberInfo->byHPPercentage = byHPPercentage;
}

void CPythonPlayer::UpdatePartyMemberAffect(DWORD dwPID, BYTE byAffectSlotIndex, short sAffectNumber)
{
	if (byAffectSlotIndex >= PARTY_AFFECT_SLOT_MAX_NUM)
	{
		TraceError(" CPythonPlayer::UpdatePartyMemberAffect(dwPID=%d, byAffectSlotIndex=%d, sAffectNumber=%d) - Strange affect slot index", dwPID, byAffectSlotIndex, sAffectNumber);
		return;
	}

	TPartyMemberInfo * pPartyMemberInfo;
	if (!GetPartyMemberPtr(dwPID, &pPartyMemberInfo))
	{
		TraceError(" CPythonPlayer::UpdatePartyMemberAffect(dwPID=%d, byAffectSlotIndex=%d, sAffectNumber=%d) - Failed to find character", dwPID, byAffectSlotIndex, sAffectNumber);
		return;
	}

	pPartyMemberInfo->sAffects[byAffectSlotIndex] = sAffectNumber;
}

void CPythonPlayer::RemovePartyMember(DWORD dwPID)
{
	DWORD dwVID = 0;
	TPartyMemberInfo * pPartyMemberInfo;
	if (GetPartyMemberPtr(dwPID, &pPartyMemberInfo))
	{
		dwVID = pPartyMemberInfo->dwVID;
	}

	m_PartyMemberMap.erase(dwPID);

	if (dwVID > 0)
	{
		CInstanceBase * pInstance = NEW_FindActorPtr(dwVID);
		if (pInstance)
			pInstance->RefreshTextTail();
	}
}

bool CPythonPlayer::IsPartyMemberByVID(DWORD dwVID)
{
	std::map<DWORD, TPartyMemberInfo>::iterator itor = m_PartyMemberMap.begin();
	for (; itor != m_PartyMemberMap.end(); ++itor)
	{
		TPartyMemberInfo & rPartyMemberInfo = itor->second;
		if (dwVID == rPartyMemberInfo.dwVID)
			return true;
	}

	return false;
}

bool CPythonPlayer::IsPartyMemberByName(const char * c_szName)
{
	std::map<DWORD, TPartyMemberInfo>::iterator itor = m_PartyMemberMap.begin();
	for (; itor != m_PartyMemberMap.end(); ++itor)
	{
		TPartyMemberInfo & rPartyMemberInfo = itor->second;
		if (0 == rPartyMemberInfo.strName.compare(c_szName))
			return true;
	}

	return false;
}

bool CPythonPlayer::GetPartyMemberPtr(DWORD dwPID, TPartyMemberInfo ** ppPartyMemberInfo)
{
	std::map<DWORD, TPartyMemberInfo>::iterator itor = m_PartyMemberMap.find(dwPID);

	if (m_PartyMemberMap.end() == itor)
		return false;

	*ppPartyMemberInfo = &(itor->second);

	return true;
}

bool CPythonPlayer::PartyMemberPIDToVID(DWORD dwPID, DWORD * pdwVID)
{
	std::map<DWORD, TPartyMemberInfo>::iterator itor = m_PartyMemberMap.find(dwPID);

	if (m_PartyMemberMap.end() == itor)
		return false;

	const TPartyMemberInfo & c_rPartyMemberInfo = itor->second;
	*pdwVID = c_rPartyMemberInfo.dwVID;

	return true;
}

bool CPythonPlayer::PartyMemberVIDToPID(DWORD dwVID, DWORD * pdwPID)
{
	std::map<DWORD, TPartyMemberInfo>::iterator itor = m_PartyMemberMap.begin();
	for (; itor != m_PartyMemberMap.end(); ++itor)
	{
		TPartyMemberInfo & rPartyMemberInfo = itor->second;
		if (dwVID == rPartyMemberInfo.dwVID)
		{
			*pdwPID = rPartyMemberInfo.dwPID;
			return true;
		}
	}

	return false;
}

bool CPythonPlayer::IsSamePartyMember(DWORD dwVID1, DWORD dwVID2)
{
	return (IsPartyMemberByVID(dwVID1) && IsPartyMemberByVID(dwVID2));
}

// PVP
void CPythonPlayer::RememberChallengeInstance(DWORD dwVID)
{
	m_RevengeInstanceSet.erase(dwVID);
	m_ChallengeInstanceSet.insert(dwVID);
}
void CPythonPlayer::RememberRevengeInstance(DWORD dwVID)
{
	m_ChallengeInstanceSet.erase(dwVID);
	m_RevengeInstanceSet.insert(dwVID);
}
void CPythonPlayer::RememberCantFightInstance(DWORD dwVID)
{
	m_CantFightInstanceSet.insert(dwVID);
}
void CPythonPlayer::ForgetInstance(DWORD dwVID)
{
	m_ChallengeInstanceSet.erase(dwVID);
	m_RevengeInstanceSet.erase(dwVID);
	m_CantFightInstanceSet.erase(dwVID);
}

bool CPythonPlayer::IsChallengeInstance(DWORD dwVID)
{
	return m_ChallengeInstanceSet.end() != m_ChallengeInstanceSet.find(dwVID);
}
bool CPythonPlayer::IsRevengeInstance(DWORD dwVID)
{
	return m_RevengeInstanceSet.end() != m_RevengeInstanceSet.find(dwVID);
}
bool CPythonPlayer::IsCantFightInstance(DWORD dwVID)
{
	return m_CantFightInstanceSet.end() != m_CantFightInstanceSet.find(dwVID);
}

void CPythonPlayer::OpenPrivateShop()
{
	m_isOpenPrivateShop = TRUE;
}
void CPythonPlayer::ClosePrivateShop()
{
	m_isOpenPrivateShop = FALSE;
}

bool CPythonPlayer::IsOpenPrivateShop()
{
	return m_isOpenPrivateShop;
}

bool CPythonPlayer::IsDead()
{
	CInstanceBase* pMainInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	if (!pMainInstance)
		return false;

	return pMainInstance->IsDead();
}

bool CPythonPlayer::IsPoly()
{
	CInstanceBase* pMainInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	if (!pMainInstance)
		return false;

	return pMainInstance->IsPoly();
}


void CPythonPlayer::SetObserverMode(bool isEnable)
{
	m_isObserverMode=isEnable;
}

bool CPythonPlayer::IsObserverMode()
{
	return m_isObserverMode;
}


BOOL CPythonPlayer::__ToggleCoolTime()
{
	m_sysIsCoolTime = 1 - m_sysIsCoolTime;
	return m_sysIsCoolTime;
}

BOOL CPythonPlayer::__ToggleLevelLimit()
{
	m_sysIsLevelLimit = 1 - m_sysIsLevelLimit;
	return m_sysIsLevelLimit;
}

void CPythonPlayer::StartStaminaConsume(DWORD dwConsumePerSec, DWORD dwCurrentStamina)
{
	m_isConsumingStamina = TRUE;
	m_fConsumeStaminaPerSec = float(dwConsumePerSec);
	m_fCurrentStamina = float(dwCurrentStamina);

	SetStatus(POINT_STAMINA, dwCurrentStamina);
}

void CPythonPlayer::StopStaminaConsume(DWORD dwCurrentStamina)
{
	m_isConsumingStamina = FALSE;
	m_fConsumeStaminaPerSec = 0.0f;
	m_fCurrentStamina = float(dwCurrentStamina);

	SetStatus(POINT_STAMINA, dwCurrentStamina);
}

DWORD CPythonPlayer::GetPKMode()
{
	CInstanceBase * pInstance = NEW_GetMainActorPtr();
	if (!pInstance)
		return 0;

	return pInstance->GetPKMode();
}

void CPythonPlayer::SetMobileFlag(BOOL bFlag)
{
	m_bMobileFlag = bFlag;
	PyCallClassMemberFunc(m_ppyGameWindow, "RefreshMobile", Py_BuildValue("()"));
}

BOOL CPythonPlayer::HasMobilePhoneNumber()
{
	return m_bMobileFlag;
}

void CPythonPlayer::SetGameWindow(PyObject * ppyObject)
{
	m_ppyGameWindow = ppyObject;
}

void CPythonPlayer::NEW_ClearSkillData(bool bAll)
{
	std::map<DWORD, DWORD>::iterator it;

	for (it = m_skillSlotDict.begin(); it != m_skillSlotDict.end();)
	{
		if (bAll || __GetSkillType(it->first) == CPythonSkill::SKILL_TYPE_ACTIVE)
			it = m_skillSlotDict.erase(it);
		else
			++it;
	}

	for (int i = 0; i < SKILL_MAX_NUM; ++i)
	{
		ZeroMemory(&m_playerStatus.aSkill[i], sizeof(TSkillInstance));
	}

	for (int j = 0; j < SKILL_MAX_NUM; ++j)
	{
		// 2004.09.30.myevan.스킬갱신시 스킬 포인트업[+] 버튼이 안나와 처리
		m_playerStatus.aSkill[j].iGrade = 0;
		m_playerStatus.aSkill[j].fcurEfficientPercentage=0.0f;
		m_playerStatus.aSkill[j].fnextEfficientPercentage=0.05f;
	}

	if (m_ppyGameWindow)
		PyCallClassMemberFunc(m_ppyGameWindow, "BINARY_CheckGameButton", Py_BuildNone());
}

void CPythonPlayer::ClearSkillDict()
{
	// ClearSkillDict
	m_skillSlotDict.clear();
#ifdef __AUTO_QUQUE_ATTACK__
#ifdef USE_PREMIUM_AFFECT
	bTotalQuqueAutoAttack = 0;
#else
	bTotalQuqueAutoAttack = 5;
#endif
	m_vecQuqueAutoAttack.clear();
#endif

	// Game End - Player Data Reset
	m_isOpenPrivateShop = false;
	m_isObserverMode = false;
#if defined(ENABLE_FISHING_GAME)
	m_bIsOpenFishingGameWindow = false;
#endif

	m_isConsumingStamina = FALSE;
	m_fConsumeStaminaPerSec = 0.0f;
	m_fCurrentStamina = 0.0f;

	m_bMobileFlag = FALSE;

	__ClearAutoAttackTargetActorID();

	m_vecAffectData.clear();
}

void CPythonPlayer::Clear()
{
	memset(&m_playerStatus, 0, sizeof(m_playerStatus));
	NEW_ClearSkillData(true);

	m_bisProcessingEmotion = FALSE;

	m_dwSendingTargetVID = 0;
	m_fTargetUpdateTime = 0.0f;
#ifdef ENABLE_AURA_SYSTEM
	m_AuraItemInstanceVector.clear();
	m_AuraItemInstanceVector.resize(AURA_SLOT_MAX);
	for (size_t i = 0; i < m_AuraItemInstanceVector.size(); ++i)
	{
		TItemData& rkAuraItemInstance = m_AuraItemInstanceVector[i];
		ZeroMemory(&rkAuraItemInstance, sizeof(TItemData));
	}

	m_bAuraWindowOpen = false;
	m_bOpenedAuraWindowType = AURA_WINDOW_TYPE_MAX;

	for (BYTE j = 0; j < AURA_SLOT_MAX; ++j)
		m_AuraRefineActivatedCell[j] = NPOS;

	ZeroMemory(&m_bAuraRefineInfo, sizeof(TAuraRefineInfo));
#endif

#if defined(__BL_BATTLE_ROYALE__)
	m_bIsBattleRoyaleEnabled = false;
#endif

	// Test Code for Status Interface
	m_stName = "";
	m_dwMainCharacterIndex = 0;
	m_dwRace = 0;
	m_dwWeaponMinPower = 0;
	m_dwWeaponMaxPower = 0;
	m_dwWeaponMinMagicPower = 0;
	m_dwWeaponMaxMagicPower = 0;
	m_dwWeaponAddPower = 0;

	/////
	m_MovingCursorPosition = TPixelPosition(0, 0, 0);
	m_fMovingCursorSettingTime = 0.0f;

	m_eReservedMode = MODE_NONE;
	m_fReservedDelayTime = 0.0f;
	m_kPPosReserved = TPixelPosition(0, 0, 0);
	m_dwVIDReserved = 0;
	m_dwIIDReserved = 0;
	m_dwSkillSlotIndexReserved = 0;
	m_dwSkillRangeReserved = 0;

	m_isUp = false;
	m_isDown = false;
	m_isLeft = false;
	m_isRight = false;
	m_isSmtMov = false;
	m_isDirMov = false;
	m_isDirKey = false;
	m_isAtkKey = false;

	m_isCmrRot = true;
	m_fCmrRotSpd = 20.0f;

	m_iComboOld = 0;

	m_dwVIDPicked=0;
	m_dwIIDPicked=0;

	m_dwcurSkillSlotIndex = DWORD(-1);

#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
	m_hitCount.clear();
#endif

	m_dwTargetVID = 0;
	m_dwTargetEndTime = 0;

	m_PartyMemberMap.clear();

	m_ChallengeInstanceSet.clear();
	m_RevengeInstanceSet.clear();

	m_isOpenPrivateShop = false;
	m_isObserverMode = false;

	m_isConsumingStamina = FALSE;
	m_fConsumeStaminaPerSec = 0.0f;
	m_fCurrentStamina = 0.0f;

	m_inGuildAreaID = 0xffffffff;

	m_bMobileFlag = FALSE;

	__ClearAutoAttackTargetActorID();
}

void CPythonPlayer::AddAffect(DWORD dwType, TPacketAffectElement kElem)
{
	int iAffIndex = GetAffectDataIndex(dwType, kElem.bApplyOn);
	if (iAffIndex != -1)
		m_vecAffectData.at(iAffIndex) = kElem;
	else
		m_vecAffectData.emplace_back(kElem);
}
#ifdef ENABLE_AURA_SYSTEM
void CPythonPlayer::__ClearAuraRefineWindow()
{
	for (BYTE i = 0; i < AURA_SLOT_MAX; ++i)
	{
		if (!m_AuraRefineActivatedCell[i].IsNPOS())
		{
			//PyCallClassMemberFunc(m_ppyGameWindow, "DeactivateSlot", Py_BuildValue("(iii)", m_AuraRefineActivatedCell[i].window_type, m_AuraRefineActivatedCell[i].cell, SLOT_HIGHLIGHT_AURA));
			m_AuraRefineActivatedCell[i] = NPOS;
		}
	}
}

void CPythonPlayer::SetAuraRefineWindowOpen(BYTE wndType)
{
	m_bOpenedAuraWindowType = wndType;
	m_bAuraWindowOpen = AURA_WINDOW_TYPE_MAX != wndType;
	if (!m_bAuraWindowOpen)
		__ClearAuraRefineWindow();
}

bool CPythonPlayer::IsAuraRefineWindowEmpty()
{
	for (size_t i = 0; i < m_AuraItemInstanceVector.size(); ++i)
	{
		if (m_AuraItemInstanceVector[i].vnum)
			return false;
	}

	return true;
}

void CPythonPlayer::SetAuraRefineInfo(BYTE bAuraRefineInfoSlot, BYTE bAuraRefineInfoLevel, BYTE bAuraRefineInfoExpPercent)
{
	if (bAuraRefineInfoSlot >= AURA_REFINE_INFO_SLOT_MAX)
		return;

	m_bAuraRefineInfo[bAuraRefineInfoSlot].bAuraRefineInfoLevel = bAuraRefineInfoLevel;
	m_bAuraRefineInfo[bAuraRefineInfoSlot].bAuraRefineInfoExpPercent = bAuraRefineInfoExpPercent;
}

BYTE CPythonPlayer::GetAuraRefineInfoLevel(BYTE bAuraRefineInfoSlot)
{
	if (bAuraRefineInfoSlot >= AURA_REFINE_INFO_SLOT_MAX)
		return 0;

	return m_bAuraRefineInfo[bAuraRefineInfoSlot].bAuraRefineInfoLevel;
}

BYTE CPythonPlayer::GetAuraRefineInfoExpPct(BYTE bAuraRefineInfoSlot)
{
	if (bAuraRefineInfoSlot >= AURA_REFINE_INFO_SLOT_MAX)
		return 0;

	return m_bAuraRefineInfo[bAuraRefineInfoSlot].bAuraRefineInfoExpPercent;
}

void CPythonPlayer::SetAuraItemData(BYTE bySlotIndex, const TItemData& rItemInstance)
{
	if (bySlotIndex >= m_AuraItemInstanceVector.size())
	{
		TraceError("CPythonPlayer::SetAuraItemData(bySlotIndex=%u) - Strange slot index", bySlotIndex);
		return;
	}

	m_AuraItemInstanceVector[bySlotIndex] = rItemInstance;

	//if (bySlotIndex != AURA_SLOT_RESULT)
	//	PyCallClassMemberFunc(m_ppyGameWindow, "ActivateSlot", Py_BuildValue("(iii)", m_AuraRefineActivatedCell[bySlotIndex].window_type, m_AuraRefineActivatedCell[bySlotIndex].cell, SLOT_HIGHLIGHT_AURA));
}

void CPythonPlayer::DelAuraItemData(BYTE bySlotIndex)
{
	if (bySlotIndex >= AURA_SLOT_MAX || bySlotIndex >= (BYTE)m_AuraItemInstanceVector.size())
	{
		TraceError("CPythonPlayer::DelAuraItemData(bySlotIndex=%u) - Strange slot index", bySlotIndex);
		return;
	}

	TItemData& rInstance = m_AuraItemInstanceVector[bySlotIndex];
	CItemData* pItemData;
	if (CItemManager::instance().GetItemDataPointer(rInstance.vnum, &pItemData))
	{
		if (bySlotIndex == AURA_SLOT_MAIN || bySlotIndex == AURA_SLOT_SUB)
			DelAuraItemData(AURA_SLOT_RESULT);

		if (bySlotIndex != AURA_SLOT_RESULT)
		{
			//PyCallClassMemberFunc(m_ppyGameWindow, "DeactivateSlot", Py_BuildValue("(iii)", m_AuraRefineActivatedCell[bySlotIndex].window_type, m_AuraRefineActivatedCell[bySlotIndex].cell, SLOT_HIGHLIGHT_AURA));
			m_AuraRefineActivatedCell[bySlotIndex] = NPOS;
		}

		ZeroMemory(&rInstance, sizeof(TItemData));
	}
}

BYTE CPythonPlayer::FineMoveAuraItemSlot()
{
	for (size_t i = 0; i < m_AuraItemInstanceVector.size(); ++i)
	{
		if (!m_AuraItemInstanceVector[i].vnum)
			return (BYTE)i;
	}

	return AURA_SLOT_MAX;
}

BYTE CPythonPlayer::GetAuraCurrentItemSlotCount()
{
	BYTE bCurCount = 0;
	for (BYTE i = 0; i < AURA_SLOT_MAX; ++i)
	{
		if (m_AuraItemInstanceVector[i].vnum)
			++bCurCount;
	}

	return bCurCount;
}

BOOL CPythonPlayer::GetAuraItemDataPtr(BYTE bySlotIndex, TItemData** ppInstance)
{
	if (bySlotIndex >= m_AuraItemInstanceVector.size())
	{
		TraceError("CPythonPlayer::GetAuraItemDataPtr(bySlotIndex=%u) - Strange slot index", bySlotIndex);
		return FALSE;
	}

	*ppInstance = &m_AuraItemInstanceVector[bySlotIndex];

	return TRUE;
}

void CPythonPlayer::SetActivatedAuraSlot(BYTE bySlotIndex, TItemPos ItemCell)
{
	if (bySlotIndex >= AURA_SLOT_MAX)
		return;

	m_AuraRefineActivatedCell[bySlotIndex] = ItemCell;
}

BYTE CPythonPlayer::FindActivatedAuraSlot(TItemPos ItemCell)
{
	for (BYTE i = AURA_SLOT_MAIN; i < AURA_SLOT_MAX; ++i)
	{
		if (m_AuraRefineActivatedCell[i] == ItemCell)
			return BYTE(i);
	}

	return AURA_SLOT_MAX;
}

TItemPos CPythonPlayer::FindUsingAuraSlot(BYTE bySlotIndex)
{
	if (bySlotIndex >= AURA_SLOT_MAX)
		return NPOS;

	return m_AuraRefineActivatedCell[bySlotIndex];
}
#endif

void CPythonPlayer::RemoveAffect(DWORD dwType, DWORD dwApplyOn)
{
	TAffectDataVector::iterator it = m_vecAffectData.begin();
	for (; it != m_vecAffectData.end(); ++it)
	{
		TPacketAffectElement kElem = *it;
		if (kElem.dwType == dwType && (dwApplyOn == 0 || dwApplyOn == kElem.bApplyOn))
		{
			m_vecAffectData.erase(it);
			break;
		}
	}
}

int CPythonPlayer::GetAffectDataIndex(DWORD dwType, DWORD dwApplyOn)
{
	int ret = -1, i = 0;
	TAffectDataVector::iterator it = m_vecAffectData.begin();
	for (; it != m_vecAffectData.end(); ++it, ++i)
	{
		TPacketAffectElement kElem = *it;
		if (kElem.dwType == dwType && (dwApplyOn == 0 || dwApplyOn == kElem.bApplyOn))
		{
			ret = i;
			break;
		}
	}
	return ret;
}

long CPythonPlayer::GetAffectDuration(DWORD dwType)
{
	TAffectDataVector::iterator it = m_vecAffectData.begin();
	for (; it != m_vecAffectData.end(); ++it)
	{
		TPacketAffectElement kElem = *it;
		if (kElem.dwType == dwType)
			return kElem.lDuration;
	}
	return 0;
}

TPacketAffectElement CPythonPlayer::GetAffectData(DWORD dwType, BYTE bApplyOn)
{
	TPacketAffectElement ret;
	memset(&ret, 0, sizeof(TPacketAffectElement));
	for (TAffectDataVector::iterator it = m_vecAffectData.begin(); it != m_vecAffectData.end(); ++it)
	{
		TPacketAffectElement elem = *it;
		if (elem.dwType == dwType && (bApplyOn == 0 || bApplyOn == elem.bApplyOn))
		{
			ret = elem;
			break;
		}
	}
	return ret;
}

CPythonPlayer::TAffectDataVector CPythonPlayer::GetAffectDataVector(DWORD dwType)
{
	TAffectDataVector vAffect;
	TAffectDataVector::iterator it = m_vecAffectData.begin();
	for (; it != m_vecAffectData.end(); ++it)
	{
		const TPacketAffectElement kElem = *it;
		if (kElem.dwType == dwType)
			vAffect.push_back(kElem);
	}
	return vAffect;
}

#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
void CPythonPlayer::SetAutoUseOnOff(const bool c_bOnOff)
{
#ifdef USE_PREMIUM_AFFECT
	if (GetStatus(POINT_PREMIUM_USER) < 1)
#else
	if (GetAffectDataIndex(CInstanceBase::NEW_AFFECT_AUTO_HUNT, 0) == -1)
#endif
	{
		m_bAutoUseOnOff = false;
		return;
	}

	if (m_bAutoUseOnOff)
	{
		CInstanceBase* pMainInst = NEW_GetMainActorPtr();
		if (pMainInst)
		{
			__ClearReservedAction();
			__ClearTarget();
			__ClearAutoAttackTargetActorID();

			pMainInst->NEW_Stop();
		}
	}

	m_bAutoUseOnOff = c_bOnOff;
}

void CPythonPlayer::__AutoProcess()
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	if (rkChrMgr.GetAutoOnOff() != true)
		return;

	if (GetAutoUseOnOff() != true)
		return;

#ifdef USE_PREMIUM_AFFECT
	if (GetStatus(POINT_PREMIUM_USER) > 0)
#else
	if (GetAffectDataIndex(CInstanceBase::NEW_AFFECT_AUTO_HUNT, 0) != -1)
#endif
	{
		if (rkChrMgr.GetAutoSystemByType(CInstanceBase::AUTO_ONOFF_ATTACK) == true)
			__AutoAttack();

		if (rkChrMgr.GetAutoSystemByType(CInstanceBase::AUTO_ONOFF_POTION) == true)
			__AutoPotion();

		if (rkChrMgr.GetAutoSystemByType(CInstanceBase::AUTO_ONOFF_SKILL) == true)
			__AutoSkill();
	}
}

void CPythonPlayer::__AutoAttack()
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();

	CInstanceBase* pNearInst = nullptr;
	CInstanceBase* pMainInst = rkChrMgr.GetInstancePtr(m_dwMainCharacterIndex);
	if (pMainInst == nullptr)
		return;

	if (pMainInst->IsDead() || pMainInst->IsAffect(CInstanceBase::AFFECT_REVIVE_INVISIBILITY))
		return;

// START ASD
	if (pMainInst->IsAttacking())
	{
		return;
	}

	TPixelPosition rkCharLastPixelPos = rkChrMgr.GetLastPixelPosition();

	if (rkChrMgr.GetAutoSystemByType(CInstanceBase::AUTO_ONOFF_RANGE))
	{
		TPixelPosition kInstPos;
		pMainInst->NEW_GetPixelPosition(&kInstPos);

		if (fabs(rkCharLastPixelPos.x - kInstPos.x) > 3200.0f ||
			fabs(rkCharLastPixelPos.y - kInstPos.y) > 3200.0f)
		{
			__ClearReservedAction();
			__ClearTarget();
			__ClearAutoAttackTargetActorID();

			pMainInst->NEW_Stop();

			pMainInst->NEW_MoveToDestPixelPositionDirection(rkCharLastPixelPos);
			return;
		}
	}
// END ASD

	float fMinDistance = 4100.0f; //4100.0f;
	float fMinFocusRadius = 4000.0f;

	CPythonCharacterManager::CharacterIterator it;
	for (it = rkChrMgr.CharacterInstanceBegin(); it != rkChrMgr.CharacterInstanceEnd(); ++it)
	{
		TPixelPosition rkLastPixelPos, kMainPixelPos, rkTargetPos;
		rkLastPixelPos = rkChrMgr.GetLastPixelPosition();
		pMainInst->NEW_GetPixelPosition(&kMainPixelPos);

		CInstanceBase* pkInstEach = *it;
		if (!pkInstEach || pkInstEach == pMainInst || (!pkInstEach->IsEnemy() &&!pkInstEach->IsStone()))
		{
			if (rkChrMgr.GetAutoSystemByType(CInstanceBase::AUTO_ONOFF_RANGE) == true)
			{
				if (fabs(rkLastPixelPos.x - kMainPixelPos.x) > 600.0f ||
					fabs(rkLastPixelPos.y - kMainPixelPos.y) > 600.0f)
				{
					if (pMainInst->GetAttackingElapsedTime() > 2.0f /*8*/)
					{
						if (!pMainInst->IsAttacking())
						{
// START ASD
							__ClearReservedAction();
							__ClearTarget();
							__ClearAutoAttackTargetActorID();

							pMainInst->NEW_Stop();

							pMainInst->NEW_MoveToDestPixelPositionDirection(rkCharLastPixelPos);
							return;
// END ASD
							// pMainInst->NEW_MoveToDestPixelPositionDirection(rkLastPixelPos);
						}
					}
				}
			}

			continue;
		}

		if (rkChrMgr.GetAutoSystemByType(CInstanceBase::AUTO_ONOFF_ATTACK_STONE) == true)
			if (!pkInstEach->IsStone())
				continue;

		if (pkInstEach->IsDead())
			continue;

		if (rkChrMgr.GetAutoSystemByType(CInstanceBase::AUTO_ONOFF_RANGE) == true)
		{
			pkInstEach->NEW_GetPixelPosition(&rkTargetPos);

			if (fabs(rkLastPixelPos.x - rkTargetPos.x) < fMinFocusRadius && fabs(rkLastPixelPos.y - rkTargetPos.y) < fMinFocusRadius)
			{
				float fDistance = pMainInst->GetDistance(pkInstEach);
				if (fDistance < fMinDistance)
				{
					fMinDistance = fDistance;
					pNearInst = pkInstEach;
				}
			}

			if (fabs(rkLastPixelPos.x - kMainPixelPos.x) > fMinFocusRadius ||
				fabs(rkLastPixelPos.y - kMainPixelPos.y) > fMinFocusRadius)
			{
				if (!pMainInst->IsAttacking())
					pMainInst->NEW_MoveToDestPixelPositionDirection(rkLastPixelPos);
				continue;
			}
		}
		else
		{
			float fDistance = pMainInst->GetDistance(pkInstEach);
			if (fDistance < fMinDistance)
			{
				fMinDistance = fDistance;
				pNearInst = pkInstEach;
			}
		}

		if (pNearInst)
		{
			if (!pNearInst->IsDead())
				__OnPressActor(*pMainInst, pNearInst->GetVirtualID(), true);
		}
	}

	if (pNearInst)
	{
		if (!pNearInst->IsDead())
			__OnPressActor(*pMainInst, pNearInst->GetVirtualID(), true);
	}
}

void CPythonPlayer::__AutoPotion()
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	CItemData* pItemData = nullptr;

	for (BYTE bSlotIndex = AUTO_POTION_SLOT_START; bSlotIndex < AUTO_POTION_SLOT_MAX; bSlotIndex++)
	{
		int c_dwItemSlotIndex = GetAutoAttachedSlotIndex(bSlotIndex);
		if (c_dwItemSlotIndex == -1)
			continue;

		DWORD dwItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_dwItemSlotIndex));
		if (dwItemIndex > 0)
		{
			if (!CItemManager::Instance().GetItemDataPointer(dwItemIndex, &pItemData))
			{
				TraceError(" Cannot find item data %d", dwItemIndex);
				return;
			}

			const DWORD c_dwSlotCooldown = GetAutoSlotCoolTime(bSlotIndex);
			const float c_fCurrentSec = CTimer::Instance().GetCurrentSecond();
			const float c_fLastUsedTime = GetAutoSlotLastUsedTime(bSlotIndex);

			switch (pItemData->GetSubType())
			{
			case USE_POTION:
			{
				BYTE bPointType = POINT_NONE;
				DWORD dwValuePct = 0;

				if (pItemData->GetValue(0) != 0)
				{
					bPointType = POINT_HP;
					dwValuePct = (GetStatus(POINT_MAX_HP) / 100) * c_dwSlotCooldown;
				}
				else if (pItemData->GetValue(1) != 0)
				{
					bPointType = POINT_SP;
					dwValuePct = (GetStatus(POINT_MAX_SP) / 100) * c_dwSlotCooldown;
				}

				if (GetStatus(bPointType) < dwValuePct)
				{
					float fElapsedTime = c_fCurrentSec - c_fLastUsedTime;
					if (c_fLastUsedTime == 0.0f || fElapsedTime >= 0.1f)
					{
						rkNetStream.SendItemUsePacket(TItemPos(INVENTORY, c_dwItemSlotIndex));
						SetAutoSlotLastUsedTime(bSlotIndex);
					}
				}
			} break;

			/*
			case CItemData::USE_CALL:
			{
				float fElapsedTime = c_fCurrentSec - c_fLastUsedTime;
				if (!c_fLastUsedTime || fElapsedTime >= static_cast<float>(c_dwSlotCooldown))
				{
					rkNetStream.SendItemUsePacket(TItemPos(INVENTORY, c_dwItemSlotIndex));
					SetAutoSlotLastUsedTime(bSlotIndex);
				}
			} break;
			*/

			case USE_ABILITY_UP:
			case USE_AFFECT:
			{
				BYTE bValueIndex = 0;

				if (pItemData->GetSubType() == USE_ABILITY_UP)
					bValueIndex = 1;
				else if (pItemData->GetSubType() == USE_AFFECT)
					bValueIndex = 3;

				float fElapsedTime = c_fCurrentSec - c_fLastUsedTime;
				if ((c_fLastUsedTime == 0.0f) || (fElapsedTime >= static_cast<float>(pItemData->GetValue(bValueIndex))))
				{
					if ((c_fLastUsedTime == 0.0f) || (fElapsedTime >= static_cast<float>(c_dwSlotCooldown)))
					{
						rkNetStream.SendItemUsePacket(TItemPos(INVENTORY, c_dwItemSlotIndex));
						SetAutoSlotLastUsedTime(bSlotIndex);
					}
				}
			} break;

			default:
				return;
			}
		}
	}
}

void CPythonPlayer::__AutoSkill()
{
	for (BYTE bSlotIndex = 0; bSlotIndex < AUTO_SKILL_SLOT_MAX; bSlotIndex++)
	{
		DWORD dwSkillIndex = GetAutoAttachedSlotIndex(bSlotIndex);
		if (dwSkillIndex > 0 && dwSkillIndex < SKILL_MAX_NUM)
		{
			const float c_fCurrentSec = CTimer::Instance().GetCurrentSecond();

			if (!__CheckRestSkillCoolTime(dwSkillIndex))
			{
				float fElapsedTime = c_fCurrentSec - m_playerStatus.aSkill[dwSkillIndex].fLastUsedTime;
				if (fElapsedTime >= GetAutoSlotCoolTime(bSlotIndex))
				{
					__UseSkill(dwSkillIndex
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, true
#endif
					);
				}
			}
		}
	}
}

void CPythonPlayer::SetAutoAttachedSlotIndex(const BYTE c_bSlotIndex, const int c_dwAttachedSlotIndex)
{
	if (c_bSlotIndex >= AUTO_POTION_SLOT_MAX)
		return;

	if (c_bSlotIndex < AUTO_SKILL_SLOT_MAX)
	{
		if (c_dwAttachedSlotIndex == -1 || c_dwAttachedSlotIndex >= c_Equipment_Start)
		{
			m_kMap_dwAutoAttachedSlotIndex[c_bSlotIndex] = 0;
			return;
		}

		for (BYTE bSlotIndex = 0; bSlotIndex < AUTO_SKILL_SLOT_MAX; bSlotIndex++)
		{
			if (m_kMap_dwAutoAttachedSlotIndex[bSlotIndex] == c_dwAttachedSlotIndex)
				m_kMap_dwAutoAttachedSlotIndex[bSlotIndex] = 0;
		}
	}
	else
	{
		if (c_dwAttachedSlotIndex == -1)
		{
			m_kMap_dwAutoAttachedSlotIndex[c_bSlotIndex] = -1;
			return;
		}

		for (BYTE bSlotIndex = AUTO_POTION_SLOT_START; bSlotIndex < AUTO_POTION_SLOT_MAX; bSlotIndex++)
		{
			if (m_kMap_dwAutoAttachedSlotIndex[bSlotIndex] == c_dwAttachedSlotIndex)
				m_kMap_dwAutoAttachedSlotIndex[bSlotIndex] = -1;
		}
	}

	m_kMap_dwAutoAttachedSlotIndex[c_bSlotIndex] = c_dwAttachedSlotIndex;
	m_kMap_dwAutoCooldownSlotIndex[c_bSlotIndex] = 0;
}

void CPythonPlayer::ClearAutoSkillSlot()
{
	for (BYTE bSlotIndex = 0; bSlotIndex < AUTO_SKILL_SLOT_MAX; bSlotIndex++)
	{
		m_kMap_dwAutoAttachedSlotIndex[bSlotIndex] = 0;
		m_kMap_dwAutoCooldownSlotIndex[bSlotIndex] = 0;
		m_afAutoSlotLastUsedTime[bSlotIndex] = 0;
	}
}

void CPythonPlayer::ClearAutoPotionSlot()
{
	for (BYTE bSlotIndex = AUTO_POTION_SLOT_START; bSlotIndex < AUTO_POTION_SLOT_MAX; bSlotIndex++)
	{
		m_kMap_dwAutoAttachedSlotIndex[bSlotIndex] = -1;
		m_kMap_dwAutoCooldownSlotIndex[bSlotIndex] = 0;
		m_afAutoSlotLastUsedTime[bSlotIndex] = 0;
	}
}

void CPythonPlayer::ClearAutoAllSlot()
{
	ClearAutoSkillSlot();
	ClearAutoPotionSlot();
}

void CPythonPlayer::SetAutoSlotLastUsedTime(const BYTE c_bSlotIndex)
{
	m_afAutoSlotLastUsedTime[c_bSlotIndex] = CTimer::Instance().GetCurrentSecond();
}

int CPythonPlayer::GetAutoAttachedSlotIndex(const BYTE c_bSlotIndex)
{
	TAutoSlotMap::const_iterator it = m_kMap_dwAutoAttachedSlotIndex.find(c_bSlotIndex);
	if (it != m_kMap_dwAutoAttachedSlotIndex.end())
		return it->second;
	return -1;
}

void CPythonPlayer::SetAutoSlotCoolTime(const BYTE c_bSlotIndex, const DWORD c_dwEditCooldown)
{
	m_kMap_dwAutoCooldownSlotIndex[c_bSlotIndex] = c_dwEditCooldown;
}

DWORD CPythonPlayer::GetAutoSlotCoolTime(const BYTE c_bSlotIndex)
{
	DWORD dwEditCooldown = 0;
	TAutoSlotMap::const_iterator it = m_kMap_dwAutoCooldownSlotIndex.find(c_bSlotIndex);
	if (it != m_kMap_dwAutoCooldownSlotIndex.end())
		dwEditCooldown = it->second;

	const DWORD c_dwAttachedSlotIndex = GetAutoAttachedSlotIndex(c_bSlotIndex);
	return CheckAutoSlotCoolTime(c_bSlotIndex, c_dwAttachedSlotIndex, dwEditCooldown);
}

DWORD CPythonPlayer::CheckAutoSlotCoolTime(const BYTE c_bSlotIndex, const DWORD c_dwAttachedSlotIndex, const DWORD c_dwEditCooldown)
{
	if (c_bSlotIndex < AUTO_SKILL_SLOT_MAX)
	{
		if (c_dwAttachedSlotIndex > 0 && c_dwAttachedSlotIndex < SKILL_MAX_NUM)
		{
			TSkillInstance& rkSkillInst = m_playerStatus.aSkill[c_dwAttachedSlotIndex];

			CPythonSkill::TSkillData* pSkillData;
			if (!CPythonSkill::Instance().GetSkillData(rkSkillInst.dwIndex, &pSkillData))
			{
				TraceError(" Cannot find skill data %d", rkSkillInst.dwIndex);
				return 0;
			}

			const DWORD dwSkillCooldown = pSkillData->GetSkillCoolTime(float(rkSkillInst.fcurEfficientPercentage));
			if (c_dwEditCooldown < dwSkillCooldown)
				return dwSkillCooldown;
			else
				return c_dwEditCooldown;
		}
	}
	else
	{
		if (c_dwAttachedSlotIndex == -1)
			return 0;

		DWORD dwItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_dwAttachedSlotIndex));
		if (dwItemIndex > 0)
		{
			CItemData* pItemData = nullptr;
			if (!CItemManager::Instance().GetItemDataPointer(dwItemIndex, &pItemData))
			{
				TraceError(" Cannot find item data %d", dwItemIndex);
				return 0;
			}

			switch (pItemData->GetSubType())
			{
			case USE_POTION:
			{
				if (1 <= c_dwEditCooldown && c_dwEditCooldown <= 100)
					return c_dwEditCooldown;
				else if (c_dwEditCooldown > 100)
					return 100;
				else
					return 50;
			}

			/*
			case CItemData::USE_CALL:
			{
				if (c_dwEditCooldown == 0)
					return 60;
				else
					return c_dwEditCooldown;
			} break;
			*/

			case USE_ABILITY_UP:
			case USE_AFFECT:
			{
				BYTE bValueIndex = 0;

				if (pItemData->GetSubType() == USE_ABILITY_UP)
					bValueIndex = 1;
				else if (pItemData->GetSubType() == USE_AFFECT)
					bValueIndex = 3;

				if (c_dwEditCooldown < pItemData->GetValue(bValueIndex))
					return pItemData->GetValue(bValueIndex);
				else
					return c_dwEditCooldown;
			} break;

			default:
				return 0;
			}
		}
	}

	return 0;
}
#endif

CPythonPlayer::CPythonPlayer(void)
{
	SetMovableGroundDistance(40.0f);

	// AffectIndex To SkillIndex
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_JEONGWI), 3));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_GEOMGYEONG), 4));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_CHEONGEUN), 19));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_GYEONGGONG), 49));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_EUNHYEONG), 34));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_GONGPO), 64));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_JUMAGAP), 65));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_HOSIN), 94));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_BOHO), 95));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_KWAESOK), 110));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_GICHEON), 96));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_JEUNGRYEOK), 111));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_PABEOP), 66));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_FALLEN_CHEONGEUN), 19));
	/////
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_GWIGEOM), 63));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_MUYEONG), 78));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_HEUKSIN), 79));

#ifdef __BUFFI_SUPPORT__
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_BUFFI_HOSIN), 164));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_BUFFI_GICHEON), 165));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_BUFFI_KWAESOK), 166));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_BUFFI_JEUNGRYEO), 167));
#endif

#ifdef ENABLE_WOLFMAN_CHARACTER
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_RED_POSSESSION), 174));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_BLUE_POSSESSION), 175));
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_CHEONUN), 182));
#endif
#ifdef ENABLE_PERFECT_BUFF
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_HOSIN_PERFECT), 94));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_BOHO_PERFECT), 95));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_KWAESOK_PERFECT), 110));
	
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_GICHEON_PERFECT), 96));
	m_kMap_dwAffectIndexToSkillIndex.insert(std::make_pair(int(CInstanceBase::AFFECT_JEUNGRYEOK_PERFECT), 111));
#endif

#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
	ClearAutoAllSlot();
	m_bAutoUseOnOff = false;
#endif

	m_ppyGameWindow = NULL;

	m_sysIsCoolTime = TRUE;
	m_sysIsLevelLimit = TRUE;
	m_dwPlayTime = 0;

	m_aeMBFButton[MBT_LEFT]=CPythonPlayer::MBF_SMART;
	m_aeMBFButton[MBT_RIGHT]=CPythonPlayer::MBF_CAMERA;
	m_aeMBFButton[MBT_MIDDLE]=CPythonPlayer::MBF_CAMERA;

	memset(m_adwEffect, 0, sizeof(m_adwEffect));

	m_isDestPosition = FALSE;
	m_ixDestPos = 0;
	m_iyDestPos = 0;
	m_iLastAlarmTime = 0;

	Clear();
}

CPythonPlayer::~CPythonPlayer(void)
{
}

#ifdef ENABLE_NEW_NAME_ITEM
void CPythonPlayer::SetItemNewName(TItemPos itemPos, const char* name)
{
	if (!itemPos.IsValidCell())
		return;
	sprintf((const_cast <TItemData*>(GetItemData(itemPos)))->name, "%s", name);
	PyCallClassMemberFunc(m_ppyGameWindow, "RefreshInventory", Py_BuildValue("()"));
}
const char* CPythonPlayer::GetItemNewName(TItemPos itemPos)
{
	if (itemPos.IsValidCell())
	{
		const TItemData* pkItem = GetItemData(itemPos);
		if (pkItem)
			return pkItem->name;
	}
	return "^";
}
#endif

#ifdef ENABLE_ULTIMATE_REGEN
#include "../eterPack/EterPackManager.h"
void localToGlobal(long& X, long& Y)
{
	CPythonBackground::Instance().LocalPositionToGlobalPosition(X, Y);
	X *= 100;
	Y *= 100;
}
void positionToGlobal(const float& eX, const float& eY, long& X, long& Y)
{
	PR_FLOAT_TO_INT(eX, X);
	PR_FLOAT_TO_INT(eY, Y);
	X /= 100;
	Y /= 100;
	localToGlobal(X, Y);
}
int DISTANCE_APPROX_SERVER(int dx, int dy)
{
	int min, max;
	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;
	if (dx < dy)
	{
		min = dx;
		max = dy;
	}
	else
	{
		min = dy;
		max = dx;
	}
	return (((max << 8) + (max << 3) - (max << 4) - (max << 1) +(min << 7) - (min << 5) + (min << 3) - (min << 1)) >> 8);
}
bool CPythonPlayer::CheckBossSafeRange()
{
	if (m_eventData.size())
	{
		auto mainInstance = NEW_GetMainActorPtr();
		if (mainInstance)
		{
			TPixelPosition kPPosCur;
			mainInstance->NEW_GetPixelPosition(&kPPosCur);
			long mainX, mainY;
			positionToGlobal(kPPosCur.x, kPPosCur.y, mainX, mainY);

			const time_t cur_Time = time(NULL);
			const struct tm vKey = *localtime(&cur_Time);
			int myear = vKey.tm_year;
			int mmon = vKey.tm_mon;
			int mday = vKey.tm_mday;
			int yday = vKey.tm_yday;
			int day = vKey.tm_wday;
			int hour = vKey.tm_hour;
			int minute = vKey.tm_min;
			int second = vKey.tm_sec;

			for (auto it = m_eventData.begin(); it != m_eventData.end(); ++it)
			{
				TNewRegen& newRegen = *it;
				if (newRegen.day != 0)
				{
					const int day = vKey.tm_wday + 1;
					if (day != newRegen.day)
						continue;
				}
				long posGlobalX = newRegen.x, posGlobalY = newRegen.y;
				localToGlobal(posGlobalX, posGlobalY);
				int fDist = DISTANCE_APPROX_SERVER(mainX - posGlobalX, mainY - posGlobalY);
				if (fDist <= newRegen.safeRange)
				{
					std::vector<CInstanceBase*> m_Data;
					CPythonCharacterManager::Instance().GetMobWithVnum(newRegen.mob_vnum, m_Data);
					if (m_Data.size())
					{
						for (auto itBoss = m_Data.begin(); itBoss != m_Data.end(); ++itBoss)
						{
							CInstanceBase* bossInstance = *itBoss;
							if (bossInstance)
							{
								TPixelPosition kPPosBoss;
								bossInstance->NEW_GetPixelPosition(&kPPosBoss);
								long bossX, bossY;
								positionToGlobal(kPPosBoss.x, kPPosBoss.y, bossX, bossY);
								fDist = DISTANCE_APPROX_SERVER(mainX - bossX, mainY - bossY);
								if (fDist <= newRegen.safeRange)
									return true;
							}
						}
					}
					else
					{
						const int nowTime = time(0);
						if (newRegen.leftTime < 0)
						{
							bool isSucces = false;
							const int now = time(0) + 2;
							const BYTE regenHour = newRegen.hour;
							const BYTE regenMinute = newRegen.minute;
							const BYTE regenSecond = newRegen.second;
							while (true)
							{
								++second;
								if (second == 60)
								{
									minute += 1;
									second = 0;
									if (minute == 60)
									{
										minute = 0;
										hour += 1;
										if (hour == 24)
										{
											hour = 0;
											day += 1;
											if (day == 7)
												day = 0;
											mday += 1;
											yday += 1;
											if (mday == 32)
											{
												mday = 1;
												mmon += 1;
												if (mmon == 12)
												{
													mmon = 0;
													myear += 1;
													yday = 0;
												}
											}
										}
									}
								}
								if (second == 0)
								{
									if (newRegen.hours_range)
									{
										if ((hour % newRegen.hours_range) == 0 && regenMinute == minute)
										{
											isSucces = true;
											break;
										}
									}
									if (newRegen.minute_range)
									{
										if ((minute % newRegen.minute_range) == 0)
										{
											isSucces = true;
											break;
										}
									}
								}
								if (regenHour == hour && regenMinute == minute && regenSecond == second)
								{
									if (newRegen.day)
									{
										if (newRegen.day - 1 == day)
										{
											isSucces = true;
											break;
										}
									}
									else
									{
										isSucces = true;
										break;
									}
								}
								if (time(0) > now)
									break;
							}
							if (isSucces)
							{
								std::tm tm{};
								tm.tm_year = myear;
								tm.tm_mon = mmon;
								tm.tm_mday = mday;
								tm.tm_isdst = 0;
								tm.tm_wday = day;
								tm.tm_hour = hour;
								tm.tm_min = minute;
								tm.tm_sec = second;
								tm.tm_yday = yday;
								const time_t t = mktime(&tm);
								newRegen.leftTime = t;

								if (newRegen.leftTime-nowTime >= 0 && newRegen.leftTime-nowTime <= 30)
									return true;
							}
							else
								return false;
						}
						else
						{
							if (newRegen.leftTime-nowTime >= 0 && newRegen.leftTime-nowTime <= 30)
								return true;
						}
					}
				}
			}
		}
	}
	return false;
}
bool CPythonPlayer::LoadNewRegen()
{
	m_eventData.clear();
	CMappedFile File;
	LPCVOID pData;
	if (!CEterPackManager::Instance().Get(File, "locale/newregen.txt", &pData, __FUNCTION__))
		return false;
	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);
	const std::string mapName = CPythonBackground::Instance().GetWarpMapName();
	CTokenVector TokenVector;
	for (DWORD i = 0; i < textFileLoader.GetLineCount(); ++i)
	{
		if (!textFileLoader.SplitLine(i, &TokenVector, "\t"))
			continue;
		else if (TokenVector.size() != 16)
			continue;
		else if (mapName != TokenVector[2])
			continue;

		TNewRegen data;
		data.x = atoi(TokenVector[3].c_str());
		data.y = atoi(TokenVector[4].c_str());
		data.day = atoi(TokenVector[7].c_str());
		data.hour = atoi(TokenVector[8].c_str());
		data.minute = atoi(TokenVector[9].c_str());
		data.second = atoi(TokenVector[10].c_str());
		data.safeRange = atoi(TokenVector[12].c_str());
		if (data.safeRange == 0)
			continue;
		data.hours_range = atoi(TokenVector[13].c_str());
		data.minute_range = atoi(TokenVector[14].c_str());
		data.mob_vnum = atoi(TokenVector[15].c_str());
		data.leftTime = -1;
		m_eventData.emplace_back(data);
	}
	return true;
}
#endif

#ifdef __AUTO_HUNT__
#include "PythonMiniMap.h"
void CPythonPlayer::ClearAutoHunt()
{
	CPythonMiniMap::Instance().SetAutoHuntRange(false);
	m_vecAutoSkills.clear();
	m_vecAutoHuntItems.clear();
}
void CPythonPlayer::AutoHuntSettings(const char* szArgument)
{
	std::vector<std::string> vecArgs;
	split_argument(szArgument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "end")
	{
		ClearAutoHunt();
	}
	else if (vecArgs[1] == "start")
	{
		ClearAutoHunt();
		if (vecArgs.size() < 4) { return; }
		if (vecArgs[2] != "empty")
		{
			std::vector<std::string> vecArgsEx;
			split_argument(vecArgs[2].c_str(), vecArgsEx, "?");
			for (BYTE j = 0; j < vecArgsEx.size(); ++j)
			{
				if (vecArgsEx[j] != "1")
					continue;
				m_vecAutoHuntItems.emplace_back(j, 0);
			}
		}
		if (vecArgs[3] != "empty")
		{
			std::vector<std::string> vecArgsEx;
			split_argument(vecArgs[3].c_str(), vecArgsEx, "?");
			int i;
			for (BYTE j = 0; j < vecArgsEx.size(); ++j)
			{
				if (j > 5)
					break;
				i = atoi(vecArgsEx[j].c_str());
				m_vecAutoSkills.emplace_back(i);
			}
		}

		CInstanceBase* pkInstMain = NEW_GetMainActorPtr();
		if(pkInstMain)
			pkInstMain->NEW_GetPixelPosition(&m_bAutoHuntStartPoint);
	
		CPythonMiniMap::Instance().SetAutoHuntRange(true);
	}
}
void CPythonPlayer::AutoHuntLoop()
{
	if (!CPythonMiniMap::Instance().GetAutoHuntStatus())
		return;

	CInstanceBase* pkInstMain = NEW_GetMainActorPtr();
	if (pkInstMain)
	{
		CInstanceBase* pkInstVictim = __GetTargetActorPtr();

		const int now = time(0);

		const std::map<BYTE, std::pair<DWORD, int>> m_vecItemData = {
			{
				0,
				{
					70038,//itemIdx
					5//itemcooldown
				}
			},
			{
				1,
				{
					72049,//itemIdx
					60 * 5//itemcooldown
				}
			},
			{
				2,
				{
					71016,//itemIdx
					60 * 5//itemcooldown
				}
			}
		};

		for (auto it = m_vecAutoHuntItems.begin(); it != m_vecAutoHuntItems.end(); ++it)
		{
			if (it->second > now)
				continue;
			const auto itData = m_vecItemData.find(it->first);
			if (itData != m_vecItemData.end())
			{
				const DWORD itemVnum = itData->second.first;
		
				for (int i = 0; i < c_Inventory_Count; ++i)
				{
					if (m_playerStatus.aItem[i].vnum == itemVnum)
					{
						CPythonNetworkStream::Instance().SendItemUsePacket(TItemPos(INVENTORY, i));
						break;
					}
				}
				it->second = now + itData->second.second;
			}
		}

		for (const auto& skillSlotNumber : m_vecAutoSkills)
		{
			CPythonSkill::TSkillData* pSkillData;
			CPythonSkill::Instance().GetSkillData(GetSkillIndex(skillSlotNumber), &pSkillData);
			if (pSkillData)
			{
				const BYTE skillIdx = GetSkillIndex(skillSlotNumber);
				if (pSkillData->IsStandingSkill() && skillIdx != 62 && skillIdx != 76 && skillIdx != 93 && skillIdx != 17)
				{
					if (!IsSkillActive(skillSlotNumber) && GetSkillCoolTime(skillSlotNumber) == 0)
					{
						__UseSkill(skillSlotNumber);
						return;
					}
					continue;
				}
				//if (!pkInstVictim || pkInstMain->IsAttacking() || GetSkillCoolTime(skillSlotNumber) != 0)
				if (!pkInstVictim || GetSkillCoolTime(skillSlotNumber) != 0)
					continue;
				if (__ProcessEnemySkillTargetRange(*pkInstMain, *pkInstVictim, *pSkillData, skillSlotNumber))
				{
					const float fDistance = pkInstMain->GetDistance(pkInstVictim);
					if (fDistance > 200.0)
						continue;
					if (__UseSkill(skillSlotNumber))
						return;
				}
			}
		}

		if (pkInstMain->IsAttacking())
			return;

		if (!pkInstVictim)
		{
			pkInstVictim = CPythonCharacterManager::Instance().FindVictim(pkInstMain, (CPythonMiniMap::Instance().GetAutoHuntRadius() + 40.0) * (4000.0 / 100.0), &m_bAutoHuntStartPoint);
			if (!pkInstVictim)
			{
				TPixelPosition srcPos;
				pkInstMain->NEW_GetPixelPosition(&srcPos);
				const float fDistance = GetDistanceNew(srcPos, m_bAutoHuntStartPoint);
				if(fDistance > 200.0)
					pkInstMain->NEW_MoveToDestPixelPositionDirection(m_bAutoHuntStartPoint);
				return;
			}

			m_dwVIDReserved = pkInstVictim->GetVirtualID();
			m_dwTargetVID = pkInstVictim->GetVirtualID();
			__OnPressActor(*pkInstMain, m_dwTargetVID, true);
			return;
		}
		__ReserveProcess_ClickActor();
	}
}
#endif