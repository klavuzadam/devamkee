#include "StdAfx.h"
#include "InstanceBase.h"
#include "PythonBackground.h"
#include "PythonNonPlayer.h"
#include "PythonPlayer.h"
#include "PythonCharacterManager.h"
#include "AbstractPlayer.h"
#include "AbstractApplication.h"
#include "packet.h"

#include "../eterlib/StateManager.h"
#include "../gamelib/ItemManager.h"
#include "../gamelib/GameLibDefines.h"

#ifdef WJ_SHOW_MOB_INFO
#include "PythonSystem.h"
#include "PythonTextTail.h"
#endif
#ifdef RACE_HEIGHT
#include "../gamelib/RaceManager.h"
#endif
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
#include "PythonApplication.h"
#endif

#if defined(__BL_BATTLE_ROYALE__)
#include "PythonBattleRoyaleManager.h"
#endif

BOOL HAIR_COLOR_ENABLE=FALSE;
BOOL USE_ARMOR_SPECULAR=FALSE;
BOOL RIDE_HORSE_ENABLE=TRUE;
const float c_fDefaultRotationSpeed = 1200.0f;
const float c_fDefaultHorseRotationSpeed = 1000.0f;
// #define ENABLE_NO_MOUNT_CHECK

bool IsWall(unsigned race)
{
	switch (race)
	{
		case 14201:
		case 14202:
		case 14203:
		case 14204:
			return true;
			break;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////

CInstanceBase::SHORSE::SHORSE()
{
	__Initialize();
}

CInstanceBase::SHORSE::~SHORSE()
{
	assert(m_pkActor==NULL);
}

void CInstanceBase::SHORSE::__Initialize()
{
	m_isMounting=false;
	m_pkActor=NULL;
}
#ifdef RACE_HEIGHT
float CInstanceBase::GetBaseHeight()
{
	CActorInstance* pkHorse = m_kHorse.GetActorPtr();
	if (!m_kHorse.IsMounting() || !pkHorse)
		return 0.0f;

	DWORD dwHorseVnum = m_kHorse.m_pkActor->GetRace();
	if ((dwHorseVnum >= 20101 && dwHorseVnum <= 20109) ||
		(dwHorseVnum == 20029 || dwHorseVnum == 20030))
		return 100.0f;

	float fRaceHeight = CRaceManager::instance().GetRaceHeight(dwHorseVnum);
	if (fRaceHeight == 0.0f)
		return 100.0f;
	else
		return fRaceHeight;
}
#endif
void CInstanceBase::SHORSE::SetAttackSpeed(UINT uAtkSpd)
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.SetAttackSpeed(uAtkSpd/100.0f);
}

void CInstanceBase::SHORSE::SetMoveSpeed(UINT uMovSpd)
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.SetMoveSpeed(uMovSpd/100.0f);
}

void CInstanceBase::SHORSE::Create(const TPixelPosition& c_rkPPos, UINT eRace, UINT eHitEffect)
{
	assert(NULL==m_pkActor && "CInstanceBase::SHORSE::Create - ALREADY MOUNT");

	m_pkActor=new CActorInstance;

	CActorInstance& rkActor=GetActorRef();
	rkActor.SetEventHandler(CActorInstance::IEventHandler::GetEmptyPtr());
	if (!rkActor.SetRace(eRace))
	{
		delete m_pkActor;
		m_pkActor=NULL;
		return;
	}

	rkActor.SetShape(0);
	rkActor.SetBattleHitEffect(eHitEffect);
	rkActor.SetAlphaValue(0.0f);
	rkActor.BlendAlphaValue(1.0f, 0.5f);
	rkActor.SetMoveSpeed(1.0f);
	rkActor.SetAttackSpeed(1.0f);
	rkActor.SetMotionMode(CRaceMotionData::MODE_GENERAL);
	rkActor.Stop();
	rkActor.RefreshActorInstance();

	rkActor.SetCurPixelPosition(c_rkPPos);

	m_isMounting=true;
}

void CInstanceBase::SHORSE::Destroy()
{
	if (m_pkActor)
	{
		m_pkActor->Destroy();
		delete m_pkActor;
	}

	__Initialize();
}

CActorInstance& CInstanceBase::SHORSE::GetActorRef()
{
	assert(NULL!=m_pkActor && "CInstanceBase::SHORSE::GetActorRef");
	return *m_pkActor;
}

CActorInstance* CInstanceBase::SHORSE::GetActorPtr()
{
	return m_pkActor;
}

enum eMountType {MOUNT_TYPE_NONE=0, MOUNT_TYPE_NORMAL=1, MOUNT_TYPE_COMBAT=2, MOUNT_TYPE_MILITARY=3};
eMountType GetMountLevelByVnum(DWORD dwMountVnum, bool IsNew)
{
	if (!dwMountVnum)
		return MOUNT_TYPE_NONE;

	switch (dwMountVnum)
	{
		// ### YES SKILL
		// @fixme116 begin
		case 20107: // normal military horse (no guild)
		case 20108: // normal military horse (guild member)
		//case 20109: // normal military horse (guild master)
			if (IsNew)
				return MOUNT_TYPE_NONE;
		// @fixme116 end
		// Classic
		case 20110: // Classic Boar
		case 20111: // Classic Wolf
		case 20112: // Classic Tiger
		case 20113: // Classic Lion
		case 20114: // White Lion
		// Special Lv2
		case 20115: // Wild Battle Boar
		case 20116: // Fight Wolf
		case 20117: // Storm Tiger
		case 20118: // Battle Lion (bugged)
		case 20205: // Wild Battle Boar (alternative)
		case 20206: // Fight Wolf (alternative)
		case 20207: // Storm Tiger (alternative)
		case 20208: // Battle Lion (bugged) (alternative)
		// Royal Tigers
		case 20120: // blue
		case 20121: // dark red
		case 20122: // gold
		case 20123: // green
		case 20124: // pied
		case 20125: // white
		// Royal mounts (Special Lv3)
		case 20209: // Royal Boar
		case 20210: // Royal Wolf
		case 20211: // Royal Tiger
		case 20212: // Royal Lion
		//
		case 20215: // Rudolph m Lv3 (yes skill, yes atk)
		case 20218: // Rudolph f Lv3 (yes skill, yes atk)
		case 20225: // Dyno Lv3 (yes skill, yes atk)
		case 20230: // Turkey Lv3 (yes skill, yes atk)
		case 20231: // Leopard (no skill, yes atk)
		case 20232: // Black Panther (no skill, yes atk)
		case 20224: // Dyno Lv2 (no skill, yes atk)
		case 20221: // Polar Predator (no skill, yes atk)
		case 20219: // Equus Porphyreus (no skill, yes atk)
		
		//
		case 32000:
		case 32001:
		case 32002:
		case 32003:
		case 32004:
		case 32005:
		case 32006:
		case 32007:
		case 32009:
		case 32010:
		case 32011:
		case 32012:
		case 32013:
		case 32014:
		case 32015:
		case 32016:
		case 32017:
		case 32018:
		case 32019:
		case 32020:
		case 32021:
		case 32022:
		case 32023:
		case 32024:
		case 32025:
		case 32028:
		case 65002:
		case 65003:
		case 65004:
		case 65005:
		case 20244:
		case 20245:
		case 32026:
		case 32027:
		case 32029:
		case 32030:
		case 32031:
		case 32032:
		case 32033:
		case 32034:
		case 32035:
		case 32036:
		case 20109:
		case 20226: // Nightmare (no skill, yes atk)
		case 20227: // Unicorn (no skill, yes atk)
		case 20228: // Turkey Lv1 (no skill, no atk)
		case 20229: // Turkey Lv2 (no skill, yes atk)
		case 20142:
		case 20143:
		case 20144:
		case 20145:
		case 20146:

			return MOUNT_TYPE_MILITARY;
			break;
		// ### NO SKILL YES ATK
		// @fixme116 begin
		case 20104: // normal combat horse (no guild)
		case 20105: // normal combat horse (guild member)
		case 20106: // normal combat horse (guild master)
			if (IsNew)
				return MOUNT_TYPE_NONE;
		// @fixme116 end
		case 20119: // Black Horse (no skill, yes atk)
		case 20214: // Rudolph m Lv2 (no skill, yes atk)
		case 20217: // Rudolph f Lv2 (no skill, yes atk)
		case 20220: // Comet (no skill, yes atk)
		case 20222: // Armoured Panda (no skill, yes atk)
			return MOUNT_TYPE_COMBAT;
			break;
		// ### NO SKILL NO ATK
		// @fixme116 begin
		case 20101: // normal beginner horse (no guild)
		case 20102: // normal beginner horse (guild member)
		case 20103: // normal beginner horse (guild master)
			if (IsNew)
				return MOUNT_TYPE_NONE;
		// @fixme116 end
		case 20213: // Rudolph m Lv1 (no skill, no atk)
		case 20216: // Rudolph f Lv1 (no skill, no atk)
		// Special Lv1
		case 20201: // Boar Lv1 (no skill, no atk)
		case 20202: // Wolf Lv1 (no skill, no atk)
		case 20203: // Tiger Lv1 (no skill, no atk)
		case 20204: // Lion Lv1 (no skill, no atk)
		//
		case 20223: // Dyno Lv1 (no skill, no atk)
			return MOUNT_TYPE_NORMAL;
			break;
		default:
			return MOUNT_TYPE_NONE;
			break;
	}
}

UINT CInstanceBase::SHORSE::GetLevel()
{
	if (m_pkActor)
	{
#ifndef ENABLE_NO_MOUNT_CHECK
		return static_cast<UINT>(GetMountLevelByVnum(m_pkActor->GetRace(), false));
#else
		return (m_pkActor->GetRace()) ? MOUNT_TYPE_MILITARY : MOUNT_TYPE_NONE;
#endif
	}
	return 0;
}

bool CInstanceBase::SHORSE::IsNewMount()
{
#ifndef ENABLE_NO_MOUNT_CHECK
	if (m_pkActor)
	{
		DWORD dwMountVnum = m_pkActor->GetRace();
		eMountType mountType = GetMountLevelByVnum(dwMountVnum, true);
		return (mountType != MOUNT_TYPE_NONE) && (mountType != MOUNT_TYPE_NORMAL);
	}
#endif
	return false;
}
bool CInstanceBase::SHORSE::CanUseSkill()
{
	// 마상스킬은 말의 레벨이 3 이상이어야만 함.
	if (IsMounting())
		return 2 < GetLevel();

	return true;
}

bool CInstanceBase::SHORSE::CanAttack()
{
	if (IsMounting())
		if (GetLevel()<=1)
			return false;

	return true;
}

bool CInstanceBase::SHORSE::IsMounting()
{
	return m_isMounting;
}

void CInstanceBase::SHORSE::Deform()
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.INSTANCEBASE_Deform();
}

void CInstanceBase::SHORSE::Render()
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.Render();
}

void CInstanceBase::__AttachHorseSaddle()
{
	if (!IsMountingHorse())
		return;
	m_kHorse.m_pkActor->AttachModelInstance(CRaceData::PART_MAIN, "saddle", m_GraphicThingInstance, CRaceData::PART_MAIN);
}

void CInstanceBase::__DetachHorseSaddle()
{
	if (!IsMountingHorse())
		return;
	m_kHorse.m_pkActor->DetachModelInstance(CRaceData::PART_MAIN, m_GraphicThingInstance, CRaceData::PART_MAIN);
}

//////////////////////////////////////////////////////////////////////////////////////

void CInstanceBase::BlockMovement()
{
	m_GraphicThingInstance.BlockMovement();
}

bool CInstanceBase::IsBlockObject(const CGraphicObjectInstance& c_rkBGObj)
{
	return m_GraphicThingInstance.IsBlockObject(c_rkBGObj);
}

bool CInstanceBase::AvoidObject(const CGraphicObjectInstance& c_rkBGObj)
{
	return m_GraphicThingInstance.AvoidObject(c_rkBGObj);
}

///////////////////////////////////////////////////////////////////////////////////

bool __ArmorVnumToShape(int iVnum, DWORD * pdwShape)
{
	*pdwShape = iVnum;

	/////////////////////////////////////////

	if (0 == iVnum || 1 == iVnum)
		return false;

	if (!USE_ARMOR_SPECULAR)
		return false;

	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(iVnum, &pItemData))
		return false;

	enum
	{
		SHAPE_VALUE_SLOT_INDEX = 3,
	};

	*pdwShape = pItemData->GetValue(SHAPE_VALUE_SLOT_INDEX);

	return true;
}

// 2004.07.05.myevan.궁신탄영 끼이는 문제
class CActorInstanceBackground : public IBackground
{
	public:
		CActorInstanceBackground() {}
		virtual ~CActorInstanceBackground() {}
		bool IsBlock(int x, int y)
		{
			CPythonBackground& rkBG=CPythonBackground::Instance();
			return rkBG.isAttrOn(x, y, CTerrainImpl::ATTRIBUTE_BLOCK);
		}
};

static CActorInstanceBackground gs_kActorInstBG;

bool CInstanceBase::LessRenderOrder(CInstanceBase* pkInst)
{
	int nMainAlpha=(__GetAlphaValue() < 1.0f) ? 1 : 0;
	int nTestAlpha=(pkInst->__GetAlphaValue() < 1.0f) ? 1 : 0;
	if (nMainAlpha < nTestAlpha)
		return true;
	if (nMainAlpha > nTestAlpha)
		return false;

	if (GetRace()<pkInst->GetRace())
		return true;
	if (GetRace()>pkInst->GetRace())
		return false;

	if (GetShape()<pkInst->GetShape())
		return true;

	if (GetShape()>pkInst->GetShape())
		return false;

	UINT uLeftLODLevel=__LessRenderOrder_GetLODLevel();
	UINT uRightLODLevel=pkInst->__LessRenderOrder_GetLODLevel();
	if (uLeftLODLevel<uRightLODLevel)
		return true;
	if (uLeftLODLevel>uRightLODLevel)
		return false;

	if (m_awPart[CRaceData::PART_WEAPON]<pkInst->m_awPart[CRaceData::PART_WEAPON])
		return true;

	return false;
}

UINT CInstanceBase::__LessRenderOrder_GetLODLevel()
{
	CGrannyLODController* pLODCtrl=m_GraphicThingInstance.GetLODControllerPointer(0);
	if (!pLODCtrl)
		return 0;

	return pLODCtrl->GetLODLevel();
}

bool CInstanceBase::__Background_GetWaterHeight(const TPixelPosition& c_rkPPos, float* pfHeight)
{
	long lHeight;
	if (!CPythonBackground::Instance().GetWaterHeight(int(c_rkPPos.x), int(c_rkPPos.y), &lHeight))
		return false;

	*pfHeight = float(lHeight);

	return true;
}

bool CInstanceBase::__Background_IsWaterPixelPosition(const TPixelPosition& c_rkPPos)
{
	return CPythonBackground::Instance().isAttrOn(c_rkPPos.x, c_rkPPos.y, CTerrainImpl::ATTRIBUTE_WATER);
}

const float PC_DUST_RANGE = 2000.0f;
const float NPC_DUST_RANGE = 1000.0f;

DWORD CInstanceBase::ms_dwUpdateCounter=0;
DWORD CInstanceBase::ms_dwRenderCounter=0;
DWORD CInstanceBase::ms_dwDeformCounter=0;

CDynamicPool<CInstanceBase> CInstanceBase::ms_kPool;

bool CInstanceBase::__IsInDustRange()
{
	if (!__IsExistMainInstance())
		return false;

	CInstanceBase* pkInstMain=__GetMainInstancePtr();

	float fDistance=NEW_GetDistanceFromDestInstance(*pkInstMain);

	if (IsPC())
	{
		if (fDistance<=PC_DUST_RANGE)
			return true;
	}

	if (fDistance<=NPC_DUST_RANGE)
		return true;

	return false;
}

void CInstanceBase::__EnableSkipCollision()
{
	if (__IsMainInstance())
	{
		TraceError("CInstanceBase::__EnableSkipCollision - You should not skip your own collisions!!");
		return;
	}
	m_GraphicThingInstance.EnableSkipCollision();
}

void CInstanceBase::__DisableSkipCollision()
{
	m_GraphicThingInstance.DisableSkipCollision();
}

bool CInstanceBase::__CanSkipCollision()
{
	return m_GraphicThingInstance.CanSkipCollision();
}

DWORD CInstanceBase::__GetShadowMapColor(float x, float y)
{
	CPythonBackground& rkBG=CPythonBackground::Instance();
	return rkBG.GetShadowMapColor(x, y);
}

float CInstanceBase::__GetBackgroundHeight(float x, float y)
{
	CPythonBackground& rkBG=CPythonBackground::Instance();
	return rkBG.GetHeight(x, y);
}

#ifdef __MOVIE_MODE__

BOOL CInstanceBase::IsMovieMode()
{
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	if (IsAffect(AFFECT_INVISIBILITY) && !__MainCanSeeHiddenThing())
		return true;
#else
	if (IsAffect(AFFECT_INVISIBILITY))
		return true;
#endif

	return false;
}

#endif

BOOL CInstanceBase::IsInvisibility()
{
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	if (IsAffect(AFFECT_INVISIBILITY) && !__MainCanSeeHiddenThing())
		return true;
#else
	if (IsAffect(AFFECT_INVISIBILITY))
		return true;
#endif

	return false;
}

BOOL CInstanceBase::IsParalysis()
{
	return m_GraphicThingInstance.IsParalysis();
}

BOOL CInstanceBase::IsGameMaster()
{
	const std::string playerName = GetNameString();
	if (playerName.find("[") != std::string::npos)
		return TRUE;
	if (m_kAffectFlagContainer.IsSet(AFFECT_YMIR))
		return TRUE;
	return FALSE;
}


BOOL CInstanceBase::IsSameEmpire(CInstanceBase& rkInstDst)
{
//combat zone vegas
#if defined(WJ_COMBAT_ZONE)
	if (IsCombatZoneMap() || rkInstDst.IsCombatZoneMap())
		return FALSE;
#endif
//combat zone vegas end
	if (0 == rkInstDst.m_dwEmpireID)
		return TRUE;

	if (IsGameMaster())
		return TRUE;

	if (rkInstDst.IsGameMaster())
		return TRUE;

	if (rkInstDst.m_dwEmpireID==m_dwEmpireID)
		return TRUE;

	return FALSE;
}

DWORD CInstanceBase::GetEmpireID()
{
	return m_dwEmpireID;
}

DWORD CInstanceBase::GetGuildID()
{
	return m_dwGuildID;
}

int CInstanceBase::GetAlignment()
{
	return m_sAlignment;
}

UINT CInstanceBase::GetAlignmentGrade()
{
	if (m_sAlignment >= 12000)
		return 0;
	else if (m_sAlignment >= 8000)
		return 1;
	else if (m_sAlignment >= 4000)
		return 2;
	else if (m_sAlignment >= 1000)
		return 3;
	else if (m_sAlignment >= 0)
		return 4;
	else if (m_sAlignment > -4000)
		return 5;
	else if (m_sAlignment > -8000)
		return 6;
	else if (m_sAlignment > -12000)
		return 7;

	return 8;
}

int CInstanceBase::GetAlignmentType()
{
	switch (GetAlignmentGrade())
	{
		case 0:
		case 1:
		case 2:
		case 3:
		{
			return ALIGNMENT_TYPE_WHITE;
			break;
		}

		case 5:
		case 6:
		case 7:
		case 8:
		{
			return ALIGNMENT_TYPE_DARK;
			break;
		}
	}

	return ALIGNMENT_TYPE_NORMAL;
}

BYTE CInstanceBase::GetPKMode()
{
	return m_byPKMode;
}

bool CInstanceBase::IsKiller()
{
	return m_isKiller;
}

bool CInstanceBase::IsPartyMember()
{
	return m_isPartyMember;
}

BOOL CInstanceBase::IsInSafe()
{
	const TPixelPosition& c_rkPPosCur=m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
	if (CPythonBackground::Instance().isAttrOn(c_rkPPosCur.x, c_rkPPosCur.y, CTerrainImpl::ATTRIBUTE_BANPK))
		return TRUE;

	return FALSE;
}

float CInstanceBase::CalculateDistanceSq3d(const TPixelPosition& c_rkPPosDst)
{
	const TPixelPosition& c_rkPPosSrc=m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
	return SPixelPosition_CalculateDistanceSq3d(c_rkPPosSrc, c_rkPPosDst);
}

void CInstanceBase::OnSelected()
{
#ifdef __MOVIE_MODE__
	if (!__IsExistMainInstance())
		return;
#endif

	if (IsStoneDoor())
		return;

	if (IsDead())
		return;

	__AttachSelectEffect();
}

void CInstanceBase::OnUnselected()
{
	__DetachSelectEffect();
}

void CInstanceBase::OnTargeted()
{
#ifdef __MOVIE_MODE__
	if (!__IsExistMainInstance())
		return;
#endif

	if (IsStoneDoor())
		return;

	if (IsDead())
		return;

	__AttachTargetEffect();
}

void CInstanceBase::OnUntargeted()
{
	__DetachTargetEffect();
}

void CInstanceBase::DestroySystem()
{
	ms_kPool.Clear();
}

void CInstanceBase::CreateSystem(UINT uCapacity)
{
	ms_kPool.Create(uCapacity);

	memset(ms_adwCRCAffectEffect, 0, sizeof(ms_adwCRCAffectEffect));

	ms_fDustGap=250.0f;
	ms_fHorseDustGap=500.0f;
}

CInstanceBase* CInstanceBase::New()
{
	return ms_kPool.Alloc();
}

void CInstanceBase::Delete(CInstanceBase* pkInst)
{
	pkInst->Destroy();
	ms_kPool.Free(pkInst);
}

void CInstanceBase::SetMainInstance()
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();

	DWORD dwVID=GetVirtualID();
	rkChrMgr.SetMainInstance(dwVID);

	m_GraphicThingInstance.SetMainInstance();
}

CInstanceBase* CInstanceBase::__GetMainInstancePtr()
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	return rkChrMgr.GetMainInstancePtr();
}

void CInstanceBase::__ClearMainInstance()
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	rkChrMgr.ClearMainInstance();
}

/* 실제 플레이어 캐릭터인지 조사.*/
bool CInstanceBase::__IsMainInstance()
{
	if (this==__GetMainInstancePtr())
		return true;

	return false;
}

bool CInstanceBase::__IsExistMainInstance()
{
	if(__GetMainInstancePtr())
		return true;
	else
		return false;
}

bool CInstanceBase::__MainCanSeeHiddenThing()
{
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	CInstanceBase * pInstance = __GetMainInstancePtr();
	return (pInstance) ? TRUE == pInstance->IsGameMaster() : false;
#else
	return false;
#endif
}

float CInstanceBase::__GetBowRange()
{
	float fRange = 2500.0f - 100.0f;

	if (__IsMainInstance())
	{
		IAbstractPlayer& rPlayer=IAbstractPlayer::GetSingleton();
		fRange += float(rPlayer.GetStatus(POINT_BOW_DISTANCE));
	}

	return fRange;
}

CInstanceBase* CInstanceBase::__FindInstancePtr(DWORD dwVID)
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	return rkChrMgr.GetInstancePtr(dwVID);
}

bool CInstanceBase::__FindRaceType(DWORD dwRace, BYTE* pbType)
{
	CPythonNonPlayer& rkNonPlayer=CPythonNonPlayer::Instance();
	return rkNonPlayer.GetInstanceType(dwRace, pbType);
}

bool CInstanceBase::Create(const SCreateData& c_rkCreateData)
{
	IAbstractApplication::GetSingleton().SkipRenderBuffering(300);

	SetInstanceType(c_rkCreateData.m_bType);

	const auto npcRaceIndex = CPythonNonPlayer::Instance().GetNPCWear(c_rkCreateData.m_dwRace, NPC_WEAR_RACE);
#ifdef __BUFFI_SUPPORT__
	if (65000 == c_rkCreateData.m_dwRace)
	{
		SetInstanceType(CActorInstance::TYPE_PC);
		SetBuffi(true);

		if (!SetRace(3))
			return false;
	}
	else
	{
#endif
#ifdef ENABLE_NPC_WEAR_ITEM
		if (npcRaceIndex != -1)
		{
			if (!SetRace(npcRaceIndex))
				return false;
		}
		else
		{
			if (!SetRace(c_rkCreateData.m_dwRace))
				return false;
		}
#else
		if (!SetRace(c_rkCreateData.m_dwRace))
			return false;
#endif
	}

	SetVirtualID(c_rkCreateData.m_dwVID);

	if (c_rkCreateData.m_isMain)
		SetMainInstance();

	if (IsGuildWall())
	{
		unsigned center_x;
		unsigned center_y;

		c_rkCreateData.m_kAffectFlags.ConvertToPosition(&center_x, &center_y);

		float center_z = __GetBackgroundHeight(center_x, center_y);
		NEW_SetPixelPosition(TPixelPosition(float(c_rkCreateData.m_lPosX), float(c_rkCreateData.m_lPosY), center_z));
	}
	else
	{
		SCRIPT_SetPixelPosition(float(c_rkCreateData.m_lPosX), float(c_rkCreateData.m_lPosY));
	}

#ifdef ENABLE_NPC_WEAR_ITEM
	if (npcRaceIndex != -1)
	{
		CPythonNonPlayer& nonPlayer = CPythonNonPlayer::Instance();

		const auto mount = nonPlayer.GetNPCWear(c_rkCreateData.m_dwRace, NPC_WEAR_MOUNT);
		if (mount != 0)
			MountHorse(mount);
		SetArmor(nonPlayer.GetNPCWear(c_rkCreateData.m_dwRace, NPC_WEAR_ARMOR));
		SetWeapon(nonPlayer.GetNPCWear(c_rkCreateData.m_dwRace, NPC_WEAR_WEAPON));
		SetHair(nonPlayer.GetNPCWear(c_rkCreateData.m_dwRace, NPC_WEAR_HAIR));
		SetSash(nonPlayer.GetNPCWear(c_rkCreateData.m_dwRace, NPC_WEAR_ACCE)-85000);
		for (int i = 0; i < CHR_SHINING_NUM; i++)
			SetShining(i, nonPlayer.GetNPCWear(c_rkCreateData.m_dwRace, NPC_WEAR_SHINING_WEAPON1 + i));
	}
	else
	{
		if (0 != c_rkCreateData.m_dwMountVnum)
			MountHorse(c_rkCreateData.m_dwMountVnum);
		SetArmor(c_rkCreateData.m_dwArmor);
	}
#else
	if (0 != c_rkCreateData.m_dwMountVnum)
		MountHorse(c_rkCreateData.m_dwMountVnum);

	SetArmor(c_rkCreateData.m_dwArmor);
#endif

	if (IsPC() || IsBuffi())
	{
		SetHair(c_rkCreateData.m_dwHair);
#ifdef ENABLE_SHINING_SYSTEM
		for (int i = 0; i < CHR_SHINING_NUM; i++)
			SetShining(i, c_rkCreateData.m_dwShining[i]);
#endif
		#ifdef ENABLE_SASH_SYSTEM
		SetSash(c_rkCreateData.m_dwSash);
		#endif
#ifdef ELEMENT_SPELL_WORLDARD
		SetElementEffect(c_rkCreateData.m_dwElementsEffect);
#endif
#ifdef ENABLE_AURA_SYSTEM
		SetAura(c_rkCreateData.m_dwAura);
#endif
		//SetWeapon(c_rkCreateData.m_dwWeapon);
		SetWeapon(c_rkCreateData.m_dwWeapon, c_rkCreateData.m_dwArrowType);

#ifdef ENABLE_SHINING_SYSTEM
		if (!AttachShiningEffect(c_rkCreateData.m_dwWeapon))
			TraceError("Error on shining attachment!");
#endif

#if defined(WJ_COMBAT_ZONE)	
		SetCombatZoneRank(c_rkCreateData.combat_zone_rank);
		SetCombatZonePoints(c_rkCreateData.combat_zone_points);
#endif
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		SetLanguage(c_rkCreateData.m_stLanguage);
#endif
	}

	__Create_SetName(c_rkCreateData);
#ifdef ENABLE_YOHARA_SYSTEM
	if(IsPC())
		m_dwConquerorLevel = c_rkCreateData.m_dwConquerorLevel;
#endif


#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
	if (IsEnemy() && CPythonSystem::Instance().IsShowMobLevel())
		m_dwLevel = CPythonNonPlayer::Instance().GetMonsterLevel(GetRace());
	else
		m_dwLevel = c_rkCreateData.m_dwLevel;
#else
	m_dwLevel = c_rkCreateData.m_dwLevel;
#endif


	

	m_dwGuildID = c_rkCreateData.m_dwGuildID;
	m_dwEmpireID = c_rkCreateData.m_dwEmpireID;

	SetVirtualNumber(c_rkCreateData.m_dwRace);
	SetRotation(c_rkCreateData.m_fRot);

	SetAlignment(c_rkCreateData.m_sAlignment);
#ifdef NEW_PET_SYSTEM
	SetLevelText(c_rkCreateData.m_dwLevel);
#endif
	SetPKMode(c_rkCreateData.m_byPKMode);

	SetMoveSpeed(c_rkCreateData.m_dwMovSpd);
	SetAttackSpeed(c_rkCreateData.m_dwAtkSpd);
// #ifdef NEW_PET_SYSTEM
	// if(m_dwRace == 34041 ||m_dwRace == 34045 || m_dwRace == 34049 ||m_dwRace == 34053 ||m_dwRace == 34042 ||m_dwRace == 34046 ||m_dwRace == 34050 ||m_dwRace == 34054){
		// float scale = c_rkCreateData.m_dwLevel * 0.005f + 0.75f;
		// m_GraphicThingInstance.SetScaleNew(scale, scale, scale);
	// }
	// else
		// m_GraphicThingInstance.SetScaleNew(1.0f,1.0f,1.0f);
// #endif
	// NOTE : Dress 를 입고 있으면 Alpha 를 넣지 않는다.
	if (!IsWearingDress())
	{
		// NOTE : 반드시 Affect 셋팅 윗쪽에 있어야 함
		m_GraphicThingInstance.SetAlphaValue(0.0f);
		m_GraphicThingInstance.BlendAlphaValue(1.0f, 0.5f);
	}

#ifdef ENABLE_MOB_SCALE_SYSTEM
	float fx,fy,fz = 1.0f;
	if(!IsPC() && !IsMount()){
		if (CPythonNonPlayer::Instance().GetScale(c_rkCreateData.m_dwRace, fx, fy, fz))
		{
			m_GraphicThingInstance.SetScale(fx, fy, fz, true);
		}
	}
#endif


	if(IsMount())
	{
		float fScale = static_cast<float>(50.0 / 100.0f); // 20.0 100/20 
		m_GraphicThingInstance.SetScale(fScale,fScale,fScale, true);
	}

	if (!IsGuildWall())
	{
		SetAffectFlagContainer(c_rkCreateData.m_kAffectFlags);
	}

	// NOTE : 반드시 Affect 셋팅 후에 해야 함
#if defined(__BL_BATTLE_ROYALE__)
	if (!CPythonBackground::Instance().IsBattleRoyaleMap() || !PythonBattleRoyaleManager::Instance().IsPCTextTailHided() || !IsPC())
	{
		AttachTextTail();
		RefreshTextTail();
	}
#else
	AttachTextTail();
	RefreshTextTail();
#endif

#ifdef STONE_SCALE
	UpdateStoneScale();
#endif

	if (c_rkCreateData.m_dwStateFlags & ADD_CHARACTER_STATE_SPAWN)
	{
		// if (IsAffect(AFFECT_SPAWN))
			// __AttachEffect(EFFECT_SPAWN_APPEAR);

		if (IsPC())
		{
			Refresh(CRaceMotionData::NAME_WAIT, true);
		}
		else
		{
			Refresh(CRaceMotionData::NAME_SPAWN, false);
		}
	}
	else
	{
		Refresh(CRaceMotionData::NAME_WAIT, true);
	}

	__AttachEmpireEffect(c_rkCreateData.m_dwEmpireID);

	RegisterBoundingSphere();

	auto bossRank = CPythonNonPlayer::Instance().GetMonsterRank(c_rkCreateData.m_dwRace);
	auto bossType = CPythonNonPlayer::Instance().GetMonsterType(c_rkCreateData.m_dwRace);
	if (bossRank >= 4 && bossType == 0)
		__AttachBossEffect();

	if (c_rkCreateData.m_dwStateFlags & ADD_CHARACTER_STATE_DEAD)
		m_GraphicThingInstance.DieEnd();

	SetStateFlags(c_rkCreateData.m_dwStateFlags);

	m_GraphicThingInstance.SetBattleHitEffect(ms_adwCRCAffectEffect[EFFECT_HIT]);

#ifdef ENABLE_NEW_PET_SYSTEM
	RefreshModelScale();
#endif
	if (!IsPC())
	{
		DWORD dwBodyColor = CPythonNonPlayer::Instance().GetMonsterColor(c_rkCreateData.m_dwRace);
		if (0 != dwBodyColor)
		{
			SetModulateRenderMode();
			SetAddColor(dwBodyColor);
		}
	}

	__AttachHorseSaddle();

	// 길드 심볼을 위한 임시 코드, 적정 위치를 찾는 중
	const int c_iGuildSymbolRace = 14200;
	if (c_iGuildSymbolRace == GetRace())
	{
		std::string strFileName = GetGuildSymbolFileName(m_dwGuildID);
		if (IsFile(strFileName.c_str()))
			m_GraphicThingInstance.ChangeMaterial(strFileName.c_str());
	}

#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	if (IsAffect(AFFECT_INVISIBILITY) && __MainCanSeeHiddenThing())
		m_GraphicThingInstance.BlendAlphaValue(0.5f, 0.5f);
#endif

	const CPythonNonPlayer::TMobTable* pMobTable = CPythonNonPlayer::Instance().GetTable(GetRace());
	if (pMobTable)
		m_GraphicThingInstance.SetRank(pMobTable->bRank);
	return true;
}


void CInstanceBase::__Create_SetName(const SCreateData& c_rkCreateData)
{
	if (IsGoto())
	{
		SetNameString("", 0);
		return;
	}
	if (IsWarp())
	{
		__Create_SetWarpName(c_rkCreateData);
		return;
	}
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
	if (IsEnemy() && CPythonSystem::Instance().IsShowMobAIFlag() && CPythonNonPlayer::Instance().IsAggressive(GetRace()))
	{
		std::string strName = c_rkCreateData.m_stName;
		strName += "*";
		SetNameString(strName.c_str(), strName.length());
	}
	else
		SetNameString(c_rkCreateData.m_stName.c_str(), c_rkCreateData.m_stName.length());
#else
	SetNameString(c_rkCreateData.m_stName.c_str(), c_rkCreateData.m_stName.length());
#endif
}

void CInstanceBase::__Create_SetWarpName(const SCreateData& c_rkCreateData)
{
	const char * c_szName;
	if (CPythonNonPlayer::Instance().GetName(c_rkCreateData.m_dwRace, &c_szName))
	{
		std::string strName = c_szName;
		int iFindingPos = strName.find_first_of(" ", 0);
		if (iFindingPos > 0)
		{
			strName.resize(iFindingPos);
		}
		SetNameString(strName.c_str(), strName.length());
	}
	else
	{
		SetNameString(c_rkCreateData.m_stName.c_str(), c_rkCreateData.m_stName.length());
	}
}

void CInstanceBase::SetNameString(const char* c_szName, int len)
{
	m_stName.assign(c_szName, len);
}
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
void CInstanceBase::SetLanguage(BYTE c_szLang)
{
	m_stLanguage = c_szLang;
}
#endif

bool CInstanceBase::SetRace(DWORD eRace)
{
	m_dwRace = eRace;

	if (!m_GraphicThingInstance.SetRace(eRace))
		return false;

	if (!__FindRaceType(m_dwRace, &m_eRaceType))
		m_eRaceType=CActorInstance::TYPE_PC;

	return true;
}

BOOL CInstanceBase::__IsChangableWeapon(int iWeaponID)
{
	// 드레스 입고 있을때는 부케외의 장비는 나오지 않게..
	if (IsWearingDress())
	{
		const int c_iBouquets[] =
		{
			50201,	// Bouquet for Assassin
			50202,	// Bouquet for Shaman
			50203,
			50204,
			0, // #0000545: [M2CN] 웨딩 드레스와 장비 착용 문제
		};

		for (int i = 0; c_iBouquets[i] != 0; ++i)
			if (iWeaponID == c_iBouquets[i])
				return true;

		return false;
	}
	else
		return true;
}

BOOL CInstanceBase::IsWearingDress()
{
	const int c_iWeddingDressShape = 201;
	return c_iWeddingDressShape == m_eShape;
}

BOOL CInstanceBase::IsHoldingPickAxe()
{
	const int c_iPickAxeStart = 29101;
	const int c_iPickAxeEnd = 29110;
	return m_awPart[CRaceData::PART_WEAPON] >= c_iPickAxeStart && m_awPart[CRaceData::PART_WEAPON] <= c_iPickAxeEnd;
}

BOOL CInstanceBase::IsNewMount()
{
	return m_kHorse.IsNewMount();
}

BOOL CInstanceBase::IsMountingHorse()
{
	return m_kHorse.IsMounting();
}

void CInstanceBase::MountHorse(UINT eRace)
{
	m_kHorse.Destroy();
	m_kHorse.Create(m_GraphicThingInstance.NEW_GetCurPixelPositionRef(), eRace, ms_adwCRCAffectEffect[EFFECT_HIT]);

	SetMotionMode(CRaceMotionData::MODE_HORSE);
	SetRotationSpeed(c_fDefaultHorseRotationSpeed);

	m_GraphicThingInstance.MountHorse(m_kHorse.GetActorPtr());
	m_GraphicThingInstance.Stop();
	m_GraphicThingInstance.RefreshActorInstance();
}

void CInstanceBase::DismountHorse()
{
	m_kHorse.Destroy();
}

void CInstanceBase::GetInfo(std::string* pstInfo)
{
	char szInfo[256];
	sprintf(szInfo, "Inst - UC %d, RC %d Pool - %d ",
		ms_dwUpdateCounter,
		ms_dwRenderCounter,
		ms_kPool.GetCapacity()
	);

	pstInfo->append(szInfo);
}

void CInstanceBase::ResetPerformanceCounter()
{
	ms_dwUpdateCounter=0;
	ms_dwRenderCounter=0;
	ms_dwDeformCounter=0;
}

bool CInstanceBase::NEW_IsLastPixelPosition()
{
	return m_GraphicThingInstance.IsPushing();
}

const TPixelPosition& CInstanceBase::NEW_GetLastPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetLastPixelPositionRef();
}

void CInstanceBase::NEW_SetDstPixelPositionZ(FLOAT z)
{
	m_GraphicThingInstance.NEW_SetDstPixelPositionZ(z);
}

void CInstanceBase::NEW_SetDstPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	m_GraphicThingInstance.NEW_SetDstPixelPosition(c_rkPPosDst);
}

void CInstanceBase::NEW_SetSrcPixelPosition(const TPixelPosition& c_rkPPosSrc)
{
	m_GraphicThingInstance.NEW_SetSrcPixelPosition(c_rkPPosSrc);
}

const TPixelPosition& CInstanceBase::NEW_GetCurPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
}

const TPixelPosition& CInstanceBase::NEW_GetDstPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetDstPixelPositionRef();
}

const TPixelPosition& CInstanceBase::NEW_GetSrcPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetSrcPixelPositionRef();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void CInstanceBase::OnSyncing()
{
	m_GraphicThingInstance.__OnSyncing();
}

void CInstanceBase::OnWaiting()
{
	m_GraphicThingInstance.__OnWaiting();
}

void CInstanceBase::OnMoving()
{
	m_GraphicThingInstance.__OnMoving();
}

void CInstanceBase::ChangeGuild(DWORD dwGuildID)
{
	m_dwGuildID=dwGuildID;

	DetachTextTail();
#if defined(__BL_BATTLE_ROYALE__)
	if (!CPythonBackground::Instance().IsBattleRoyaleMap() || !PythonBattleRoyaleManager::Instance().IsPCTextTailHided() || !IsPC())
	{
		AttachTextTail();
		RefreshTextTail();
	}
#else
	AttachTextTail();
	RefreshTextTail();
#endif
}

DWORD CInstanceBase::GetPart(CRaceData::EParts part)
{
	assert(part >= 0 && part < CRaceData::PART_MAX_NUM);
	return m_awPart[part];
}

DWORD CInstanceBase::GetShape()
{
	return m_eShape;
}

bool CInstanceBase::CanAct()
{
	return m_GraphicThingInstance.CanAct();
}

bool CInstanceBase::CanMove()
{
	return m_GraphicThingInstance.CanMove();
}

bool CInstanceBase::CanUseSkill()
{
	if (IsPoly())
		return false;

	if (IsWearingDress())
		return false;

	if (IsHoldingPickAxe())
		return false;

	if (!m_kHorse.CanUseSkill())
		return false;

	if (!m_GraphicThingInstance.CanUseSkill())
		return false;

	return true;
}

bool CInstanceBase::CanAttack()
{
	if (!m_kHorse.CanAttack())
		return false;

	if (IsWearingDress())
		return false;

	if (IsHoldingPickAxe())
		return false;

	return m_GraphicThingInstance.CanAttack();
}



bool CInstanceBase::CanFishing()
{
	return m_GraphicThingInstance.CanFishing();
}


BOOL CInstanceBase::IsBowMode()
{
	return m_GraphicThingInstance.IsBowMode();
}

BOOL CInstanceBase::IsHandMode()
{
	return m_GraphicThingInstance.IsHandMode();
}

BOOL CInstanceBase::IsFishingMode()
{
	if (CRaceMotionData::MODE_FISHING == m_GraphicThingInstance.GetMotionMode())
		return true;

	return false;
}

BOOL CInstanceBase::IsFishing()
{
	return m_GraphicThingInstance.IsFishing();
}

BOOL CInstanceBase::IsDead()
{
	return m_GraphicThingInstance.IsDead();
}

BOOL CInstanceBase::IsStun()
{
	return m_GraphicThingInstance.IsStun();
}

BOOL CInstanceBase::IsSleep()
{
	return m_GraphicThingInstance.IsSleep();
}


BOOL CInstanceBase::__IsSyncing()
{
	return m_GraphicThingInstance.__IsSyncing();
}



void CInstanceBase::NEW_SetOwner(DWORD dwVIDOwner)
{
	m_GraphicThingInstance.SetOwner(dwVIDOwner);
}

float CInstanceBase::GetLocalTime()
{
	return m_GraphicThingInstance.GetLocalTime();
}


void CInstanceBase::PushUDPState(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{
}

DWORD	ELTimer_GetServerFrameMSec();

void CInstanceBase::PushTCPStateExpanded(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg, UINT uTargetVID)
{
	SCommand kCmdNew;
	kCmdNew.m_kPPosDst = c_rkPPosDst;
	kCmdNew.m_dwChkTime = dwCmdTime+100;
	kCmdNew.m_dwCmdTime = dwCmdTime;
	kCmdNew.m_fDstRot = fDstRot;
	kCmdNew.m_eFunc = eFunc;
	kCmdNew.m_uArg = uArg;
	kCmdNew.m_uTargetVID = uTargetVID;
	m_kQue_kCmdNew.push_back(kCmdNew);
}

void CInstanceBase::PushTCPState(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{
	if (__IsMainInstance())
	{
		//assert(!"CInstanceBase::PushTCPState 플레이어 자신에게 이동패킷은 오면 안된다!");
		TraceError("CInstanceBase::PushTCPState You can't send move packets to yourself!");
		return;
	}

	int nNetworkGap=ELTimer_GetServerFrameMSec()-dwCmdTime;

	m_nAverageNetworkGap=(m_nAverageNetworkGap*70+nNetworkGap*30)/100;

	/*
	if (m_dwBaseCmdTime == 0)
	{
		m_dwBaseChkTime = ELTimer_GetFrameMSec()-nNetworkGap;
		m_dwBaseCmdTime = dwCmdTime;

		Tracenf("VID[%d] 네트웍갭 [%d]", GetVirtualID(), nNetworkGap);
	}
	*/

	//m_dwBaseChkTime-m_dwBaseCmdTime+ELTimer_GetServerMSec();

	SCommand kCmdNew;
	kCmdNew.m_kPPosDst = c_rkPPosDst;
	kCmdNew.m_dwChkTime = dwCmdTime+m_nAverageNetworkGap;//m_dwBaseChkTime + (dwCmdTime - m_dwBaseCmdTime);// + nNetworkGap;
	kCmdNew.m_dwCmdTime = dwCmdTime;
	kCmdNew.m_fDstRot = fDstRot;
	kCmdNew.m_eFunc = eFunc;
	kCmdNew.m_uArg = uArg;
	m_kQue_kCmdNew.push_back(kCmdNew);

	//int nApplyGap=kCmdNew.m_dwChkTime-ELTimer_GetServerFrameMSec();

	//if (nApplyGap<-500 || nApplyGap>500)
	//	Tracenf("VID[%d] NAME[%s] 네트웍갭 [cur:%d ave:%d] 작동시간 (%d)", GetVirtualID(), GetNameString(), nNetworkGap, m_nAverageNetworkGap, nApplyGap);
}

/*
CInstanceBase::TStateQueue::iterator CInstanceBase::FindSameState(TStateQueue& rkQuekStt, DWORD dwCmdTime, UINT eFunc, UINT uArg)
{
	TStateQueue::iterator i=rkQuekStt.begin();
	while (rkQuekStt.end()!=i)
	{
		SState& rkSttEach=*i;
		if (rkSttEach.m_dwCmdTime==dwCmdTime)
			if (rkSttEach.m_eFunc==eFunc)
				if (rkSttEach.m_uArg==uArg)
					break;
		++i;
	}

	return i;
}
*/

BOOL CInstanceBase::__CanProcessNetworkStatePacket()
{
	if (m_GraphicThingInstance.IsDead())
		return FALSE;
	if (m_GraphicThingInstance.IsKnockDown())
		return FALSE;
	if (m_GraphicThingInstance.IsUsingSkill())
		if (!m_GraphicThingInstance.CanCancelSkill())
			return FALSE;

	return TRUE;
}

BOOL CInstanceBase::__IsEnableTCPProcess(UINT eCurFunc)
{
	if (m_GraphicThingInstance.IsActEmotion())
	{
		return FALSE;
	}

	if (!m_bEnableTCPState)
	{
		if (FUNC_EMOTION != eCurFunc)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CInstanceBase::StateProcess()
{
	while (1)
	{
		if (m_kQue_kCmdNew.empty())
			return;

		DWORD dwDstChkTime = m_kQue_kCmdNew.front().m_dwChkTime;
		DWORD dwCurChkTime = ELTimer_GetServerFrameMSec();

		if (dwCurChkTime < dwDstChkTime)
			return;

		SCommand kCmdTop = m_kQue_kCmdNew.front();
		m_kQue_kCmdNew.pop_front();

		TPixelPosition kPPosDst = kCmdTop.m_kPPosDst;
		//DWORD dwCmdTime = kCmdTop.m_dwCmdTime;
		FLOAT fRotDst = kCmdTop.m_fDstRot;
		UINT eFunc = kCmdTop.m_eFunc;
		UINT uArg = kCmdTop.m_uArg;
		UINT uVID = GetVirtualID();
		UINT uTargetVID = kCmdTop.m_uTargetVID;

		TPixelPosition kPPosCur;
		NEW_GetPixelPosition(&kPPosCur);

		/*
		if (IsPC())
			Tracenf("%d cmd: vid=%d[%s] func=%d arg=%d  curPos=(%f, %f) dstPos=(%f, %f) rot=%f (time %d)",
			ELTimer_GetMSec(),
			uVID, m_stName.c_str(), eFunc, uArg,
			kPPosCur.x, kPPosCur.y,
			kPPosDst.x, kPPosDst.y, fRotDst, dwCmdTime-m_dwBaseCmdTime);
		*/

		TPixelPosition kPPosDir = kPPosDst - kPPosCur;
		float fDirLen = (float)sqrt(kPPosDir.x * kPPosDir.x + kPPosDir.y * kPPosDir.y);

		//Tracenf("거리 %f", fDirLen);

		if (!__CanProcessNetworkStatePacket())
		{
			Lognf(0, "vid=%d Skip State as unable to process IsDead=%d, IsKnockDown=%d", uVID, m_GraphicThingInstance.IsDead(), m_GraphicThingInstance.IsKnockDown());
			return;
		}

		if (!__IsEnableTCPProcess(eFunc))
		{
			return;
		}

		switch (eFunc)
		{
			case FUNC_WAIT:
			{
				//Tracenf("%s (%f, %f) -> (%f, %f) 남은거리 %f", GetNameString(), kPPosCur.x, kPPosCur.y, kPPosDst.x, kPPosDst.y, fDirLen);
				if (fDirLen > 1.0f)
				{
					//NEW_GetSrcPixelPositionRef() = kPPosCur;
					//NEW_GetDstPixelPositionRef() = kPPosDst;
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);

					__EnableSkipCollision();

					m_fDstRot = fRotDst;
					m_isGoing = TRUE;

					m_kMovAfterFunc.eFunc = FUNC_WAIT;

					if (!IsWalking())
						StartWalking();

					//Tracen("목표정지");
				}
				else
				{
					//Tracen("현재 정지");

					m_isGoing = FALSE;

					if (!IsWaiting())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					SetAdvancingRotation(fRotDst);
					SetRotation(fRotDst);
				}
				break;
			}

			case FUNC_MOVE:
			{
				//NEW_GetSrcPixelPositionRef() = kPPosCur;
				//NEW_GetDstPixelPositionRef() = kPPosDst;
				NEW_SetSrcPixelPosition(kPPosCur);
				NEW_SetDstPixelPosition(kPPosDst);
				m_fDstRot = fRotDst;
				m_isGoing = TRUE;
				__EnableSkipCollision();
				//m_isSyncMov = TRUE;

				m_kMovAfterFunc.eFunc = FUNC_MOVE;

				if (!IsWalking())
				{
					//Tracen("걷고 있지 않아 걷기 시작");
					StartWalking();
				}
				else
				{
					//Tracen("이미 걷는중 ");
				}
				break;
			}

			case FUNC_COMBO:
			{
				if (fDirLen >= 50.0f)
				{
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot=fRotDst;
					m_isGoing = TRUE;
					__EnableSkipCollision();

					m_kMovAfterFunc.eFunc = FUNC_COMBO;
					m_kMovAfterFunc.uArg = uArg;

					if (!IsWalking())
						StartWalking();
				}
				else
				{
					//Tracen("대기 공격 정지");

					m_isGoing = FALSE;

					if (IsWalking())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					RunComboAttack(fRotDst, uArg);
				}
				break;
			}

			case FUNC_ATTACK:
			{
				if (fDirLen>=50.0f)
				{
					//NEW_GetSrcPixelPositionRef() = kPPosCur;
					//NEW_GetDstPixelPositionRef() = kPPosDst;
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot = fRotDst;
					m_isGoing = TRUE;
					__EnableSkipCollision();
					//m_isSyncMov = TRUE;

					m_kMovAfterFunc.eFunc = FUNC_ATTACK;

					if (!IsWalking())
						StartWalking();

					//Tracen("너무 멀어서 이동 후 공격");
				}
				else
				{
					//Tracen("노말 공격 정지");

					m_isGoing = FALSE;

					if (IsWalking())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					BlendRotation(fRotDst);

					RunNormalAttack(fRotDst);

					//Tracen("가깝기 때문에 워프 공격");
				}
				break;
			}

			case FUNC_MOB_SKILL:
			{
				if (fDirLen >= 50.0f)
				{
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot = fRotDst;
					m_isGoing = TRUE;
					__EnableSkipCollision();

					m_kMovAfterFunc.eFunc = FUNC_MOB_SKILL;
					m_kMovAfterFunc.uArg = uArg;

					if (!IsWalking())
						StartWalking();
				}
				else
				{
					m_isGoing = FALSE;

					if (IsWalking())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					BlendRotation(fRotDst);

					m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_SPECIAL_1 + uArg);
				}
				break;
			}

			case FUNC_EMOTION:
			{
				if (fDirLen>100.0f)
				{
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot = fRotDst;
					m_isGoing = TRUE;

					if (__IsMainInstance())
						__EnableSkipCollision();

					m_kMovAfterFunc.eFunc = FUNC_EMOTION;
					m_kMovAfterFunc.uArg = uArg;
					m_kMovAfterFunc.uArgExpanded = uTargetVID;
					m_kMovAfterFunc.kPosDst = kPPosDst;

					if (!IsWalking())
						StartWalking();
				}
				else
				{
					__ProcessFunctionEmotion(uArg, uTargetVID, kPPosDst);
				}
				break;
			}

			default:
			{
				if (eFunc & FUNC_SKILL)
				{
					if (fDirLen >= 50.0f)
					{
						//NEW_GetSrcPixelPositionRef() = kPPosCur;
						//NEW_GetDstPixelPositionRef() = kPPosDst;
						NEW_SetSrcPixelPosition(kPPosCur);
						NEW_SetDstPixelPosition(kPPosDst);
						m_fDstRot = fRotDst;
						m_isGoing = TRUE;
						//m_isSyncMov = TRUE;
						__EnableSkipCollision();

						m_kMovAfterFunc.eFunc = eFunc;
						m_kMovAfterFunc.uArg = uArg;

						if (!IsWalking())
							StartWalking();

						//Tracen("너무 멀어서 이동 후 공격");
					}
					else
					{
						//Tracen("스킬 정지");

						m_isGoing = FALSE;

						if (IsWalking())
							EndWalking();

						SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
						SetAdvancingRotation(fRotDst);
						SetRotation(fRotDst);

						NEW_UseSkill(0, eFunc & 0x7f, uArg&0x0f, (uArg>>4) ? true : false);
						//Tracen("가깝기 때문에 워프 공격");
					}
				}
				break;
			}
		}
	}
}


void CInstanceBase::MovementProcess()
{
	TPixelPosition kPPosCur;
	NEW_GetPixelPosition(&kPPosCur);

	// 렌더링 좌표계이므로 y를 -화해서 더한다.

	TPixelPosition kPPosNext;
	{
		const D3DXVECTOR3 & c_rkV3Mov = m_GraphicThingInstance.GetMovementVectorRef();

		kPPosNext.x = kPPosCur.x + (+c_rkV3Mov.x);
		kPPosNext.y = kPPosCur.y + (-c_rkV3Mov.y);
		kPPosNext.z = kPPosCur.z + (+c_rkV3Mov.z);
	}

	TPixelPosition kPPosDeltaSC = kPPosCur - NEW_GetSrcPixelPositionRef();
	TPixelPosition kPPosDeltaSN = kPPosNext - NEW_GetSrcPixelPositionRef();
	TPixelPosition kPPosDeltaSD = NEW_GetDstPixelPositionRef() - NEW_GetSrcPixelPositionRef();

	float fCurLen = sqrtf(kPPosDeltaSC.x * kPPosDeltaSC.x + kPPosDeltaSC.y * kPPosDeltaSC.y);
	float fNextLen = sqrtf(kPPosDeltaSN.x * kPPosDeltaSN.x + kPPosDeltaSN.y * kPPosDeltaSN.y);
	float fTotalLen = sqrtf(kPPosDeltaSD.x * kPPosDeltaSD.x + kPPosDeltaSD.y * kPPosDeltaSD.y);
	float fRestLen = fTotalLen - fCurLen;

	if (__IsMainInstance())
	{
		if (m_isGoing && IsWalking())
		{
			float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(NEW_GetSrcPixelPositionRef(), NEW_GetDstPixelPositionRef());

			SetAdvancingRotation(fDstRot);

			if (fRestLen<=0.0)
			{
				if (IsWalking())
					EndWalking();

				//Tracen("목표 도달 정지");

				m_isGoing = FALSE;

				BlockMovement();

				if (FUNC_EMOTION == m_kMovAfterFunc.eFunc)
				{
					DWORD dwMotionNumber = m_kMovAfterFunc.uArg;
					DWORD dwTargetVID = m_kMovAfterFunc.uArgExpanded;
					__ProcessFunctionEmotion(dwMotionNumber, dwTargetVID, m_kMovAfterFunc.kPosDst);
					m_kMovAfterFunc.eFunc = FUNC_WAIT;
					return;
				}
			}
		}
	}
	else
	{
		if (m_isGoing && IsWalking())
		{
			float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(NEW_GetSrcPixelPositionRef(), NEW_GetDstPixelPositionRef());

			SetAdvancingRotation(fDstRot);

			// 만약 렌턴시가 늦어 너무 많이 이동했다면..
			if (fRestLen < -100.0f)
			{
				NEW_SetSrcPixelPosition(kPPosCur);

				float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(kPPosCur, NEW_GetDstPixelPositionRef());
				SetAdvancingRotation(fDstRot);
				//Tracenf("VID %d 오버 방향설정 (%f, %f) %f rest %f", GetVirtualID(), kPPosCur.x, kPPosCur.y, fDstRot, fRestLen);

				// 이동중이라면 다음번에 멈추게 한다
				if (FUNC_MOVE == m_kMovAfterFunc.eFunc)
				{
					m_kMovAfterFunc.eFunc = FUNC_WAIT;
				}
			}
			// 도착했다면...
			else if (fCurLen <= fTotalLen && fTotalLen <= fNextLen)
			{
				if (m_GraphicThingInstance.IsDead() || m_GraphicThingInstance.IsKnockDown())
				{
					__DisableSkipCollision();

					//Tracen("사망 상태라 동작 스킵");

					m_isGoing = FALSE;

					//Tracen("행동 불능 상태라 이후 동작 스킵");
				}
				else
				{
					switch (m_kMovAfterFunc.eFunc)
					{
						case FUNC_ATTACK:
						{
							if (IsWalking())
								EndWalking();

							__DisableSkipCollision();
							m_isGoing = FALSE;

							BlockMovement();
							SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
							SetAdvancingRotation(m_fDstRot);
							SetRotation(m_fDstRot);

							RunNormalAttack(m_fDstRot);
							break;
						}

						case FUNC_COMBO:
						{
							if (IsWalking())
								EndWalking();

							__DisableSkipCollision();
							m_isGoing = FALSE;

							BlockMovement();
							SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
							RunComboAttack(m_fDstRot, m_kMovAfterFunc.uArg);
							break;
						}

						case FUNC_EMOTION:
						{
							m_isGoing = FALSE;
							m_kMovAfterFunc.eFunc = FUNC_WAIT;
							__DisableSkipCollision();
							BlockMovement();

							DWORD dwMotionNumber = m_kMovAfterFunc.uArg;
							DWORD dwTargetVID = m_kMovAfterFunc.uArgExpanded;
							__ProcessFunctionEmotion(dwMotionNumber, dwTargetVID, m_kMovAfterFunc.kPosDst);
							break;
						}

						case FUNC_MOVE:
						{
							break;
						}

						case FUNC_MOB_SKILL:
						{
							if (IsWalking())
								EndWalking();

							__DisableSkipCollision();
							m_isGoing = FALSE;

							BlockMovement();
							SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
							SetAdvancingRotation(m_fDstRot);
							SetRotation(m_fDstRot);

							m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_SPECIAL_1 + m_kMovAfterFunc.uArg);
							break;
						}

						default:
						{
							if (m_kMovAfterFunc.eFunc & FUNC_SKILL)
							{
								SetAdvancingRotation(m_fDstRot);
								BlendRotation(m_fDstRot);
								NEW_UseSkill(0, m_kMovAfterFunc.eFunc & 0x7f, m_kMovAfterFunc.uArg&0x0f, (m_kMovAfterFunc.uArg>>4) ? true : false);
							}
							else
							{
								//Tracenf("VID %d 스킬 공격 (%f, %f) rot %f", GetVirtualID(), NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y, m_fDstRot);

								__DisableSkipCollision();
								m_isGoing = FALSE;

								BlockMovement();
								SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
								SetAdvancingRotation(m_fDstRot);
								BlendRotation(m_fDstRot);
								if (!IsWaiting())
								{
									EndWalking();
								}

								//Tracenf("VID %d 정지 (%f, %f) rot %f IsWalking %d", GetVirtualID(), NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y, m_fDstRot, IsWalking());
							}
							break;
						}
					}

				}
			}

		}
	}

	if (IsWalking() || m_GraphicThingInstance.IsUsingMovingSkill())
	{
		float fRotation = m_GraphicThingInstance.GetRotation();
		float fAdvancingRotation = m_GraphicThingInstance.GetAdvancingRotation();
		int iDirection = GetRotatingDirection(fRotation, fAdvancingRotation);

		if (DEGREE_DIRECTION_SAME != m_iRotatingDirection)
		{
			if (DEGREE_DIRECTION_LEFT == iDirection)
			{
				fRotation = fmodf(fRotation + m_fRotSpd*m_GraphicThingInstance.GetSecondElapsed(), 360.0f);
			}
			else if (DEGREE_DIRECTION_RIGHT == iDirection)
			{
				fRotation = fmodf(fRotation - m_fRotSpd*m_GraphicThingInstance.GetSecondElapsed() + 360.0f, 360.0f);
			}

			if (m_iRotatingDirection != GetRotatingDirection(fRotation, fAdvancingRotation))
			{
				m_iRotatingDirection = DEGREE_DIRECTION_SAME;
				fRotation = fAdvancingRotation;
			}

			m_GraphicThingInstance.SetRotation(fRotation);
		}

		if (__IsInDustRange())
		{
			float fDustDistance = NEW_GetDistanceFromDestPixelPosition(m_kPPosDust);
			if (IsMountingHorse())
			{
				if (fDustDistance > ms_fHorseDustGap)
				{
					NEW_GetPixelPosition(&m_kPPosDust);
					__AttachEffect(EFFECT_HORSE_DUST);
				}
			}
			else
			{
				if (fDustDistance > ms_fDustGap)
				{
					NEW_GetPixelPosition(&m_kPPosDust);
					__AttachEffect(EFFECT_DUST);
				}
			}
		}
	}
}

void CInstanceBase::__ProcessFunctionEmotion(DWORD dwMotionNumber, DWORD dwTargetVID, const TPixelPosition & c_rkPosDst)
{
	if (IsWalking())
		EndWalkingWithoutBlending();

	__EnableChangingTCPState();
	SCRIPT_SetPixelPosition(c_rkPosDst.x, c_rkPosDst.y);

	CInstanceBase * pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(dwTargetVID);
	if (pTargetInstance)
	{
		pTargetInstance->__EnableChangingTCPState();

		if (pTargetInstance->IsWalking())
			pTargetInstance->EndWalkingWithoutBlending();

		WORD wMotionNumber1 = HIWORD(dwMotionNumber);
		WORD wMotionNumber2 = LOWORD(dwMotionNumber);

		int src_job = RaceToJob(GetRace());
		int dst_job = RaceToJob(pTargetInstance->GetRace());

		NEW_LookAtDestInstance(*pTargetInstance);
		m_GraphicThingInstance.InterceptOnceMotion(wMotionNumber1 + dst_job);
		m_GraphicThingInstance.SetRotation(m_GraphicThingInstance.GetTargetRotation());
		m_GraphicThingInstance.SetAdvancingRotation(m_GraphicThingInstance.GetTargetRotation());

		pTargetInstance->NEW_LookAtDestInstance(*this);
		pTargetInstance->m_GraphicThingInstance.InterceptOnceMotion(wMotionNumber2 + src_job);
		pTargetInstance->m_GraphicThingInstance.SetRotation(pTargetInstance->m_GraphicThingInstance.GetTargetRotation());
		pTargetInstance->m_GraphicThingInstance.SetAdvancingRotation(pTargetInstance->m_GraphicThingInstance.GetTargetRotation());

		if (pTargetInstance->__IsMainInstance())
		{
			IAbstractPlayer & rPlayer=IAbstractPlayer::GetSingleton();
			rPlayer.EndEmotionProcess();
		}
	}

	if (__IsMainInstance())
	{
		IAbstractPlayer & rPlayer=IAbstractPlayer::GetSingleton();
		rPlayer.EndEmotionProcess();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Update & Deform & Render

int g_iAccumulationTime = 0;

void CInstanceBase::Update()
{
	++ms_dwUpdateCounter;

	StateProcess();
	m_GraphicThingInstance.PhysicsProcess();
	m_GraphicThingInstance.RotationProcess();
	m_GraphicThingInstance.ComboProcess();
	m_GraphicThingInstance.AccumulationMovement();

	if (m_GraphicThingInstance.IsMovement())
	{
		TPixelPosition kPPosCur;
		NEW_GetPixelPosition(&kPPosCur);

		DWORD dwCurTime=ELTimer_GetFrameMSec();
		//if (m_dwNextUpdateHeightTime<dwCurTime)
		{
			m_dwNextUpdateHeightTime=dwCurTime;
			kPPosCur.z = __GetBackgroundHeight(kPPosCur.x, kPPosCur.y);
			NEW_SetPixelPosition(kPPosCur);
		}

		// SetMaterialColor
		{
			DWORD dwMtrlColor=__GetShadowMapColor(kPPosCur.x, kPPosCur.y);
			m_GraphicThingInstance.SetMaterialColor(dwMtrlColor);
		}
	}

	m_GraphicThingInstance.UpdateAdvancingPointInstance();

	AttackProcess();
	MovementProcess();

	m_GraphicThingInstance.MotionProcess(IsPC());
	if (IsMountingHorse())
	{
		m_kHorse.m_pkActor->HORSE_MotionProcess(FALSE);
	}

	__ComboProcess();

	ProcessRemoveOldDamage();
	ProcessDamage();
#ifdef STONE_SCALE
	UpdateStoneScale();
#endif

}

void CInstanceBase::Transform()
{
	if (__IsSyncing())
	{
		//OnSyncing();
	}
	else
	{
		if (IsWalking() || m_GraphicThingInstance.IsUsingMovingSkill())
		{
			const D3DXVECTOR3& c_rv3Movment=m_GraphicThingInstance.GetMovementVectorRef();

			float len=(c_rv3Movment.x*c_rv3Movment.x)+(c_rv3Movment.y*c_rv3Movment.y);
			if (len>1.0f)
				OnMoving();
			else
				OnWaiting();
		}
	}

	m_GraphicThingInstance.INSTANCEBASE_Transform();
}


void CInstanceBase::Deform()
{
	// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
	if (!__CanRender())
		return;

	++ms_dwDeformCounter;

	m_GraphicThingInstance.INSTANCEBASE_Deform();

	m_kHorse.Deform();
}

void CInstanceBase::RenderTrace()
{
	if (!__CanRender())
		return;

	m_GraphicThingInstance.RenderTrace();
}




void CInstanceBase::Render()
{
	// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
	if (!__CanRender())
		return;

	++ms_dwRenderCounter;

	m_kHorse.Render();
	m_GraphicThingInstance.Render();

	if (CActorInstance::IsDirLine())
	{
		if (NEW_GetDstPixelPositionRef().x != 0.0f)
		{
			static CScreen s_kScreen;

			STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_DIFFUSE);
			STATEMANAGER.SetTextureStageState(0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
			STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP,	D3DTOP_DISABLE);
			STATEMANAGER.SaveRenderState(D3DRS_ZENABLE, FALSE);
			STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, FALSE);
			STATEMANAGER.SetRenderState(D3DRS_LIGHTING, FALSE);

			TPixelPosition px;
			m_GraphicThingInstance.GetPixelPosition(&px);
			D3DXVECTOR3 kD3DVt3Cur(px.x, px.y, px.z);
			//D3DXVECTOR3 kD3DVt3Cur(NEW_GetSrcPixelPositionRef().x, -NEW_GetSrcPixelPositionRef().y, NEW_GetSrcPixelPositionRef().z);
			D3DXVECTOR3 kD3DVt3Dest(NEW_GetDstPixelPositionRef().x, -NEW_GetDstPixelPositionRef().y, NEW_GetDstPixelPositionRef().z);

			//printf("%s %f\n", GetNameString(), kD3DVt3Cur.y - kD3DVt3Dest.y);
			//float fdx = NEW_GetDstPixelPositionRef().x - NEW_GetSrcPixelPositionRef().x;
			//float fdy = NEW_GetDstPixelPositionRef().y - NEW_GetSrcPixelPositionRef().y;

			s_kScreen.SetDiffuseColor(0.0f, 0.0f, 1.0f);
			s_kScreen.RenderLine3d(kD3DVt3Cur.x, kD3DVt3Cur.y, px.z, kD3DVt3Dest.x, kD3DVt3Dest.y, px.z);
			STATEMANAGER.RestoreRenderState(D3DRS_ZENABLE);
			STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, TRUE);
			STATEMANAGER.SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

void CInstanceBase::RenderToShadowMap()
{
	if (IsDoor())
		return;

	if (IsBuilding())
		return;

	if (!__CanRender())
		return;

	if (!__IsExistMainInstance())
		return;

	CInstanceBase* pkInstMain=__GetMainInstancePtr();

	const float SHADOW_APPLY_DISTANCE = 2500.0f;

	float fDistance=NEW_GetDistanceFromDestInstance(*pkInstMain);
	if (fDistance>=SHADOW_APPLY_DISTANCE)
		return;

	m_GraphicThingInstance.RenderToShadowMap();
}

void CInstanceBase::RenderCollision()
{
	m_GraphicThingInstance.RenderCollisionData();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Setting & Getting Data

void CInstanceBase::SetVirtualID(DWORD dwVirtualID)
{
	m_GraphicThingInstance.SetVirtualID(dwVirtualID);
}

void CInstanceBase::SetVirtualNumber(DWORD dwVirtualNumber)
{
	m_dwVirtualNumber = dwVirtualNumber;
}

void CInstanceBase::SetInstanceType(int iInstanceType)
{
	m_GraphicThingInstance.SetActorType(iInstanceType);
}

void CInstanceBase::SetAlignment(short sAlignment)
{
	m_sAlignment = sAlignment;
	RefreshTextTailTitle();
}

#ifdef ENABLE_YOHARA_SYSTEM
void CInstanceBase::SetConquerorLevelText(int sLevel)
{
	m_dwConquerorLevel = sLevel;
	UpdateTextTailConquerorLevel(sLevel);
}

DWORD CInstanceBase::GetConquerorLevel()
{
	return m_dwConquerorLevel;
}

void CInstanceBase::SetConquerorLevel(DWORD dwcLevel)
{
	m_dwConquerorLevel = dwcLevel;
}
#endif

void CInstanceBase::SetPKMode(BYTE byPKMode)
{
	if (m_byPKMode == byPKMode)
		return;

	m_byPKMode = byPKMode;

	if (__IsMainInstance())
	{
		IAbstractPlayer& rPlayer=IAbstractPlayer::GetSingleton();
		rPlayer.NotifyChangePKMode();
	}
#ifdef TOURNAMENT_PVP_SYSTEM	
	RefreshTextTail();
#endif
}

void CInstanceBase::SetLevelText(int sLevel)
{
	m_dwLevel = sLevel;
	UpdateTextTailLevel(sLevel);
}

void CInstanceBase::SetKiller(bool bFlag)
{
	if (m_isKiller == bFlag)
		return;

	m_isKiller = bFlag;
	RefreshTextTail();
}

void CInstanceBase::SetPartyMemberFlag(bool bFlag)
{
	m_isPartyMember = bFlag;
}

void CInstanceBase::SetStateFlags(DWORD dwStateFlags)
{
	if (dwStateFlags & ADD_CHARACTER_STATE_KILLER)
		SetKiller(TRUE);
	else
		SetKiller(FALSE);

	if (dwStateFlags & ADD_CHARACTER_STATE_PARTY)
		SetPartyMemberFlag(TRUE);
	else
		SetPartyMemberFlag(FALSE);
}

void CInstanceBase::SetComboType(UINT uComboType)
{
	m_GraphicThingInstance.SetComboType(uComboType);
}
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
BYTE CInstanceBase::GetLanguage()
{
	return m_stLanguage;
}
#endif
const char * CInstanceBase::GetNameString()
{
	return m_stName.c_str();
}
#if defined(WJ_COMBAT_ZONE)
bool CInstanceBase::IsCombatZoneMap()
{
	if (!strcmp(CPythonBackground::Instance().GetWarpMapName(), "metin2_map_battlefied"))
		return true;
	return false;
}

void CInstanceBase::SetCombatZonePoints(DWORD dwValue)
{
	combat_zone_points = dwValue;
}

DWORD CInstanceBase::GetCombatZonePoints()
{
	return combat_zone_points;
}

void CInstanceBase::SetCombatZoneRank(BYTE bValue)
{
	combat_zone_rank = bValue;
}

BYTE CInstanceBase::GetCombatZoneRank()
{
	return combat_zone_rank;
}
#endif
#if defined(ENABLE_LEVEL_IN_TRADE) || defined(ENABLE_NEW_EXCHANGE_WINDOW)
DWORD CInstanceBase::GetLevel()
{
	return m_dwLevel;
}
#endif

#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
void CInstanceBase::MobInfoAiFlagRefresh()
{
	// set
	std::string strName = CPythonNonPlayer::Instance().GetMonsterName(GetRace());
	if (CPythonSystem::Instance().IsShowMobAIFlag() && CPythonNonPlayer::Instance().IsAggressive(GetRace()))
		strName += "*";
	SetNameString(strName.c_str(), strName.length());
	// refresh
	DetachTextTail();
	AttachTextTail();
	RefreshTextTail();
}
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
void CInstanceBase::MobInfoLevelRefresh()
{
	// set
	if (CPythonSystem::Instance().IsShowMobLevel())
		m_dwLevel = CPythonNonPlayer::Instance().GetMonsterLevel(GetRace());
	else
		m_dwLevel = 0;
	// refresh
	if (m_dwLevel)
		UpdateTextTailLevel(m_dwLevel);
	else
		CPythonTextTail::Instance().DetachLevel(GetVirtualID());
}
#endif

DWORD CInstanceBase::GetRace()
{
	return m_dwRace;
}


bool CInstanceBase::IsConflictAlignmentInstance(CInstanceBase& rkInstVictim)
{
	if (PK_MODE_PROTECT == rkInstVictim.GetPKMode())
		return false;

	switch (GetAlignmentType())
	{
		case ALIGNMENT_TYPE_NORMAL:
		case ALIGNMENT_TYPE_WHITE:
			if (ALIGNMENT_TYPE_DARK == rkInstVictim.GetAlignmentType())
				return true;
			break;
		case ALIGNMENT_TYPE_DARK:
			if (GetAlignmentType() != rkInstVictim.GetAlignmentType())
				return true;
			break;
	}

	return false;
}

void CInstanceBase::SetDuelMode(DWORD type)
{
	m_dwDuelMode = type;
}

DWORD CInstanceBase::GetDuelMode()
{
	return m_dwDuelMode;
}
#ifdef TOURNAMENT_PVP_SYSTEM
bool CInstanceBase::IsTournamentMap()
{
	const std::string& c_rstrMapFileName = CPythonBackground::Instance().GetWarpMapName();
	return c_rstrMapFileName == "metin2_map_colosseum";
}
#endif
bool CInstanceBase::IsAttackableInstance(CInstanceBase& rkInstVictim)
{
	if (__IsMainInstance())
	{
		CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
		if(rkPlayer.IsObserverMode())
			return false;
	}

	if (GetVirtualID() == rkInstVictim.GetVirtualID())
		return false;

	if (IsStone())
	{
		if (rkInstVictim.IsPC())
			return true;
	}
	else if (IsPC())
	{
		if (rkInstVictim.IsStone())
			return true;

		if (rkInstVictim.IsPC())
		{
#ifdef ENABLE_ULTIMATE_REGEN
			CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
			if (rkPlayer.CheckBossSafeRange())
				return false;
#endif
			if (GetDuelMode())
			{
				switch(GetDuelMode())
				{
				case DUEL_CANNOTATTACK:
					return false;
				case DUEL_START:
					if(__FindDUELKey(GetVirtualID(),rkInstVictim.GetVirtualID()))
						return true;
					else
						return false;
				}
			}
			
			if (PK_MODE_GUILD == GetPKMode())
				if (GetGuildID() == rkInstVictim.GetGuildID())
					return false;
				
			if (PK_MODE_FEAR == GetPKMode())
				return false;
			
			if (PK_MODE_FEAR == rkInstVictim.GetPKMode())
				return false;

#ifdef TOURNAMENT_PVP_SYSTEM
			if (IsTournamentMap())
			{
				if (GetPKMode() == rkInstVictim.GetPKMode())
					return false;
				
				if (GetPKMode() == PK_MODE_TEAM_A && rkInstVictim.GetPKMode() == PK_MODE_TEAM_B)
					return true;
				
				if (GetPKMode() == PK_MODE_TEAM_B && rkInstVictim.GetPKMode() == PK_MODE_TEAM_A)
					return true;
			}
#endif
			if (rkInstVictim.IsKiller())
				if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
					return true;

			if (PK_MODE_PROTECT != GetPKMode())
			{
				if (PK_MODE_FREE == GetPKMode())
				{
					if (PK_MODE_PROTECT != rkInstVictim.GetPKMode())
						if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
							return true;
				}
				if (PK_MODE_GUILD == GetPKMode())
				{
					if (PK_MODE_PROTECT != rkInstVictim.GetPKMode())
						if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
							if (GetGuildID() != rkInstVictim.GetGuildID())
								return true;
				}
			}

#ifdef TOURNAMENT_PVP_SYSTEM
			if (IsSameEmpire(rkInstVictim) && !IsTournamentMap())
#else
			if (IsSameEmpire(rkInstVictim))	
#endif
			{
				if (IsPVPInstance(rkInstVictim))
					return true;

				if (PK_MODE_REVENGE == GetPKMode())
					if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
						if (IsConflictAlignmentInstance(rkInstVictim))
							return true;
			}
			else
			{
#ifdef TOURNAMENT_PVP_SYSTEM
				if (!IsTournamentMap())
					return true;
#else
				return true;
#endif
			}
		}

		if (rkInstVictim.IsEnemy())
			return true;

		if (rkInstVictim.IsWoodenDoor())
			return true;
	}
	else if (IsEnemy())
	{
		if (rkInstVictim.IsPC())
			return true;

		if (rkInstVictim.IsBuilding())
			return true;

	}
	else if (IsPoly())
	{
		if (rkInstVictim.IsPC())
			return true;

		if (rkInstVictim.IsEnemy())
			return true;
	}
	return false;
}

bool CInstanceBase::IsTargetableInstance(CInstanceBase& rkInstVictim)
{
	return rkInstVictim.CanPickInstance();
}

// 2004. 07. 07. [levites] - 스킬 사용중 타겟이 바뀌는 문제 해결을 위한 코드
bool CInstanceBase::CanChangeTarget()
{
	return m_GraphicThingInstance.CanChangeTarget();
}

// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
bool CInstanceBase::CanPickInstance()
{
	if (!__IsInViewFrustum())
		return false;

	if (IsDoor())
	{
		if (IsDead())
			return false;
	}

	if (IsPC())
	{
		if (IsAffect(AFFECT_EUNHYEONG))
		{
			if (!__MainCanSeeHiddenThing())
				return false;
		}
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
		if (IsAffect(AFFECT_REVIVE_INVISIBILITY) && !__MainCanSeeHiddenThing())
			return false;
#else
		if (IsAffect(AFFECT_REVIVE_INVISIBILITY))
			return false;
#endif
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
		if (IsAffect(AFFECT_INVISIBILITY) && !__MainCanSeeHiddenThing())
			return false;
#else
		if (IsAffect(AFFECT_INVISIBILITY))
			return false;
#endif

	}

	if (IsDead())
		return false;

	return true;
}

bool CInstanceBase::CanViewTargetHP(CInstanceBase& rkInstVictim)
{
	if (rkInstVictim.IsStone())
		return true;
	if (rkInstVictim.IsWoodenDoor())
		return true;
	if (rkInstVictim.IsEnemy())
		return true;
	if (rkInstVictim.IsPC())
		return true;
#ifdef ENABLE_DEFENSAWESHIP
	if (rkInstVictim.IsHydraNPC())
		return true;
#endif
	return false;
}

BOOL CInstanceBase::IsPoly()
{
	return m_GraphicThingInstance.IsPoly();
}

BOOL CInstanceBase::IsPC()
{
	return m_GraphicThingInstance.IsPC();
}

BOOL CInstanceBase::IsNPC()
{
	return m_GraphicThingInstance.IsNPC();
}

#ifdef ENABLE_GRAPHIC_ON_OFF
bool CInstanceBase::GetRenderStatus()
{
	return m_GraphicThingInstance.GetRenderStatus();
}
void CInstanceBase::SetRenderStatus(bool bStatus)
{
	return m_GraphicThingInstance.SetRenderStatus(bStatus);
}
bool CInstanceBase::GetEffectStatus()
{
	return m_GraphicThingInstance.GetEffectStatus();
}
void CInstanceBase::SetEffectStatus(bool bStatus)
{
	return m_GraphicThingInstance.SetEffectStatus(bStatus);
}
#endif

BOOL CInstanceBase::IsEnemy()
{
	return m_GraphicThingInstance.IsEnemy();
}
#ifdef NEW_PET_SYSTEM
BOOL CInstanceBase::IsNewPet()
{
	return m_GraphicThingInstance.IsNewPet();
}
#endif
BOOL CInstanceBase::IsStone()
{
	return m_GraphicThingInstance.IsStone();
}


BOOL CInstanceBase::IsGuildWall()	//IsBuilding 길드건물전체 IsGuildWall은 담장벽만(문은 제외)
{
	return IsWall(m_dwRace);
}


BOOL CInstanceBase::IsResource()
{
	switch (m_dwVirtualNumber)
	{
		case 20047:
		case 20048:
		case 20049:
		case 20050:
		case 20051:
		case 20052:
		case 20053:
		case 20054:
		case 20055:
		case 20056:
		case 20057:
		case 20058:
		case 20059:
		case 30301:
		case 30302:
		case 30303:
		case 30304:
		case 30305:
		case 30306:
			return TRUE;
	}

	return FALSE;
}

BOOL CInstanceBase::IsWarp()
{
	return m_GraphicThingInstance.IsWarp();
}

BOOL CInstanceBase::IsGoto()
{
	return m_GraphicThingInstance.IsGoto();
}

BOOL CInstanceBase::IsObject()
{
	return m_GraphicThingInstance.IsObject();
}

BOOL CInstanceBase::IsBuilding()
{
	return m_GraphicThingInstance.IsBuilding();
}

BOOL CInstanceBase::IsDoor()
{
	return m_GraphicThingInstance.IsDoor();
}

BOOL CInstanceBase::IsWoodenDoor()
{
	if (m_GraphicThingInstance.IsDoor())
	{
		int vnum = GetVirtualNumber();
		if (vnum == 13000) // 나무문
			return true;
		else if (vnum >= 30111 && vnum <= 30119) // 사귀문
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

BOOL CInstanceBase::IsStoneDoor()
{
	return m_GraphicThingInstance.IsDoor() && 13001 == GetVirtualNumber();
}
#ifdef NAMECOLOR_BOSS_CLIENT
BOOL CInstanceBase::IsBoss()
{
    if (GetRace() == 691)
        return TRUE;
    if (GetRace() == 692)
        return TRUE;
    if (GetRace() == 693)
        return TRUE;
    if (GetRace() == 791)
        return TRUE;
    if (GetRace() == 991)
        return TRUE;
    if (GetRace() == 992)
        return TRUE;
    if (GetRace() == 993)
        return TRUE;
    if (GetRace() == 1091)
        return TRUE;
    if (GetRace() == 1092)
        return TRUE;
    if (GetRace() == 1093)
        return TRUE;
    if (GetRace() == 1094)
        return TRUE;
    if (GetRace() == 1095)
        return TRUE;
    if (GetRace() == 2191)
        return TRUE;
    if (GetRace() == 1191)
        return TRUE;
    if (GetRace() == 1192)
        return TRUE;
    if (GetRace() == 1304)
        return TRUE;
    if (GetRace() == 1306)
        return TRUE;
    if (GetRace() == 1307)
        return TRUE;
    if (GetRace() == 1901)
        return TRUE;
    if (GetRace() == 1902)
        return TRUE;
    if (GetRace() == 1903)
        return TRUE;
    if (GetRace() == 2206)
        return TRUE;
    if (GetRace() == 2207)
        return TRUE;
    if (GetRace() == 2291)
        return TRUE;
    if (GetRace() == 2306)
        return TRUE;
    if (GetRace() == 2307)
        return TRUE;
    if (GetRace() == 2492)
        return TRUE;
    if (GetRace() == 2493)
        return TRUE;
    if (GetRace() == 2494)
        return TRUE;
    if (GetRace() == 2598)
        return TRUE;
    if (GetRace() == 3090)
        return TRUE;
    if (GetRace() == 3091)
        return TRUE;
    if (GetRace() == 3190)
        return TRUE;
    if (GetRace() == 3191)
        return TRUE;
    if (GetRace() == 3290)
        return TRUE;
    if (GetRace() == 3291)
        return TRUE;
    if (GetRace() == 3390)
        return TRUE;
    if (GetRace() == 3391)
        return TRUE;
    if (GetRace() == 3490)
        return TRUE;
    if (GetRace() == 3491)
        return TRUE;
    if (GetRace() == 3590)
        return TRUE;
    if (GetRace() == 3591)
        return TRUE;
    if (GetRace() == 3690)
        return TRUE;
    if (GetRace() == 3691)
        return TRUE;
    if (GetRace() == 3790)
        return TRUE;
    if (GetRace() == 3791)
        return TRUE;
    if (GetRace() == 3890)
        return TRUE;
    if (GetRace() == 3891)
        return TRUE;
    if (GetRace() == 5001)
        return TRUE;
    if (GetRace() == 5004)
        return TRUE;
    if (GetRace() == 5002)
        return TRUE;
    if (GetRace() == 5161)
        return TRUE;
    if (GetRace() == 5162)
        return TRUE;
    if (GetRace() == 5163)
        return TRUE;
    if (GetRace() == 6091)
        return TRUE;
    if (GetRace() == 6191)
        return TRUE;
    return FALSE;
}
#endif
BOOL CInstanceBase::IsFlag()
{
	if (GetRace() == 20035)
		return TRUE;
	if (GetRace() == 20036)
		return TRUE;
	if (GetRace() == 20037)
		return TRUE;

	return FALSE;
}

BOOL CInstanceBase::IsForceVisible()
{
	if (IsAffect(AFFECT_SHOW_ALWAYS))
		return TRUE;

	if (IsObject() || IsBuilding() || IsDoor() )
		return TRUE;

	return FALSE;
}

int	CInstanceBase::GetInstanceType()
{
	return m_GraphicThingInstance.GetActorType();
}

DWORD CInstanceBase::GetVirtualID()
{
	return m_GraphicThingInstance.GetVirtualID();
}

DWORD CInstanceBase::GetVirtualNumber()
{
	return m_dwVirtualNumber;
}

// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
bool CInstanceBase::__IsInViewFrustum()
{
	return m_GraphicThingInstance.isShow();
}

/*
bool CInstanceBase::__CanRender()
{
	if (!__IsInViewFrustum())
		return false;

#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	if (IsAffect(AFFECT_INVISIBILITY) && !__MainCanSeeHiddenThing())
		return false;
#else
	if (IsAffect(AFFECT_INVISIBILITY))
		return false;
#endif

	return true;
}*/


bool CInstanceBase::__CanRender()
{
	if (IsAlwaysRender())
		return true;
	if (!__IsInViewFrustum())
		return false;
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	if (IsAffect(AFFECT_INVISIBILITY) && !__MainCanSeeHiddenThing())
		return false;
#else
	if (IsAffect(AFFECT_INVISIBILITY))
		return false;
#endif

	// Graphic Check Part!/
#ifdef ENABLE_GRAPHIC_ON_OFF
	CPythonSystem& pythonSystem = CPythonSystem::Instance();
	CInstanceBase* pkInstMain = __GetMainInstancePtr();
	const BYTE effectLevelIndex = pythonSystem.GetEffectLevel();
	bool effectShow = true;

	if (pkInstMain != NULL && pkInstMain != this)
	{
		const float fDistance = NEW_GetDistanceFromDestInstance(*pkInstMain);
/*
		if (IsShop())
		{
			constexpr WORD shopRange[] = { 10000,6000,4000,2000,0 };
			BYTE shopRangeIndex = pythonSystem.GetShopNamesRange();
			if (shopRangeIndex >= 5)
				shopRangeIndex = 0;
			if (fDistance > shopRange[shopRangeIndex])
			{
				m_GraphicThingInstance.HideAllAttachingEffectEx();
				SetRenderStatus(false);
				return false;
			}
		}
*/
		if (IsPet() && !pythonSystem.GetPetRender())
		{
			m_GraphicThingInstance.HideAllAttachingEffectEx();
			SetRenderStatus(false);
			return false;
		}
		else if (IsMount() && !pythonSystem.GetMountRender())
		{
			m_GraphicThingInstance.HideAllAttachingEffectEx();
			SetRenderStatus(false);
			return false;
		}

		if (effectLevelIndex == 1 && !(IsEnemy() || IsStone()))
			effectShow = false;
		else if (effectLevelIndex == 2 && !IsPC())
			effectShow = false;
		else if (effectLevelIndex == 3)
			effectShow = false;
	}
	if (effectLevelIndex == 4)
		effectShow = false;
	if (effectShow != GetEffectStatus())
	{
		if(effectShow)
			m_GraphicThingInstance.ShowAllAttachingEffectEx();
		else
			m_GraphicThingInstance.HideAllAttachingEffectEx();
	}
	SetRenderStatus(true);
#endif

	return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Graphic Control

bool CInstanceBase::IntersectBoundingBox()
{
	float u, v, t;
	return m_GraphicThingInstance.Intersect(&u, &v, &t);
}

bool CInstanceBase::IntersectDefendingSphere()
{
	return m_GraphicThingInstance.IntersectDefendingSphere();
}

float CInstanceBase::GetDistance(CInstanceBase * pkTargetInst)
{
	TPixelPosition TargetPixelPosition;
	pkTargetInst->m_GraphicThingInstance.GetPixelPosition(&TargetPixelPosition);
	return GetDistance(TargetPixelPosition);
}

float CInstanceBase::GetDistance(const TPixelPosition & c_rPixelPosition)
{
	TPixelPosition PixelPosition;
	m_GraphicThingInstance.GetPixelPosition(&PixelPosition);

	float fdx = PixelPosition.x - c_rPixelPosition.x;
	float fdy = PixelPosition.y - c_rPixelPosition.y;

	return sqrtf((fdx*fdx) + (fdy*fdy));
}

CActorInstance& CInstanceBase::GetGraphicThingInstanceRef()
{
	return m_GraphicThingInstance;
}

CActorInstance* CInstanceBase::GetGraphicThingInstancePtr()
{
	return &m_GraphicThingInstance;
}

void CInstanceBase::RefreshActorInstance()
{
	m_GraphicThingInstance.RefreshActorInstance();
}

void CInstanceBase::Refresh(DWORD dwMotIndex, bool isLoop)
{
	RefreshState(dwMotIndex, isLoop);
}

void CInstanceBase::RestoreRenderMode()
{
	m_GraphicThingInstance.RestoreRenderMode();
}

void CInstanceBase::SetAddRenderMode()
{
	m_GraphicThingInstance.SetAddRenderMode();
}

void CInstanceBase::SetModulateRenderMode()
{
	m_GraphicThingInstance.SetModulateRenderMode();
}

void CInstanceBase::SetRenderMode(int iRenderMode)
{
	m_GraphicThingInstance.SetRenderMode(iRenderMode);
}

void CInstanceBase::SetAddColor(const D3DXCOLOR & c_rColor)
{
	m_GraphicThingInstance.SetAddColor(c_rColor);
}

void CInstanceBase::__SetBlendRenderingMode()
{
	m_GraphicThingInstance.SetBlendRenderMode();
}

void CInstanceBase::__SetAlphaValue(float fAlpha)
{
	m_GraphicThingInstance.SetAlphaValue(fAlpha);
}

float CInstanceBase::__GetAlphaValue()
{
	return m_GraphicThingInstance.GetAlphaValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Part

void CInstanceBase::SetHair(DWORD eHair)
{
	if (!HAIR_COLOR_ENABLE)
		return;

	if (IsPC()==false && GetRace() > 8)
		return;
	
	// if (IsBuffi())
		// return;

	m_awPart[CRaceData::PART_HAIR] = eHair;
	m_GraphicThingInstance.SetHair(eHair);
}

void CInstanceBase::ChangeHair(DWORD eHair)
{
	if (!HAIR_COLOR_ENABLE)
		return;

	if (IsPC()==false && GetRace() > 8)
		return;
	
	// if (IsBuffi())
		// return;

	if (GetPart(CRaceData::PART_HAIR)==eHair)
		return;

	SetHair(eHair);

	//int type = m_GraphicThingInstance.GetMotionMode();

	RefreshState(CRaceMotionData::NAME_WAIT, true);
	//RefreshState(type, true);
}

void CInstanceBase::SetArmor(DWORD dwArmor)
{
	DWORD dwShape;
	if (__ArmorVnumToShape(dwArmor, &dwShape))
	{
		CItemData * pItemData;
		if (CItemManager::Instance().GetItemDataPointer(dwArmor, &pItemData))
		{
			float fSpecularPower=pItemData->GetSpecularPowerf();
			SetShape(dwShape, fSpecularPower);
			__GetRefinedEffect(pItemData);
#ifdef ENABLE_SHINING_TABLE_SYSTEM
			_GetShiningTableEffect(pItemData);
#endif
			return;
		}
		else
		{
			__ClearArmorRefineEffect();
#ifdef ENABLE_SHINING_TABLE_SYSTEM
			__ClearArmorShiningEffect();
#endif
		}

	}

	SetShape(dwArmor);
}
#ifdef ENABLE_SASH_SYSTEM
void CInstanceBase::SetSash(DWORD dwSash)
{
	if ((IsPC() && GetRace() > 7) || IsPoly() || __IsShapeAnimalWear())
	{
#ifdef ENABLE_SHINING_TABLE_SYSTEM
		__ClearAcceShiningEffect();
#endif
		return;
	}
		

	if (IsWearingDress())
	{
#ifdef ENABLE_SHINING_TABLE_SYSTEM
		__ClearAcceShiningEffect();
#endif
		return;
	}

	if (IsPoly())
	{
#ifdef ENABLE_SHINING_TABLE_SYSTEM
		__ClearAcceShiningEffect();
#endif
		return;
	}

	m_awPart[CRaceData::PART_SASH] = dwSash;

	dwSash += 85000;
	
	ClearSashEffect();
	
	float fSpecular = 65.0f;
	if (dwSash > 86000)
	{
		// dwSash -= 1000;
		fSpecular += 35;
		
		m_dwSashEffect = EFFECT_REFINED + EFFECT_SASH;
		__EffectContainer_AttachEffect(m_dwSashEffect);
	}
	
	fSpecular /= 100.0f;
	
	
	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(dwSash, &pItemData))
	{
#ifdef ENABLE_SHINING_TABLE_SYSTEM
		__ClearAcceShiningEffect();
#endif
		return;
	}

#ifdef ENABLE_SHINING_TABLE_SYSTEM
	__ClearAcceShiningEffect();
#endif

#ifdef ENABLE_SHINING_TABLE_SYSTEM
	if (pItemData)
		_GetShiningTableEffect(pItemData);
#endif

	m_GraphicThingInstance.AttachSash(pItemData, fSpecular);
#ifdef ENABLE_OBJ_SCALLING
	DWORD dwRace = GetRace(), dwPos = RaceToJob(dwRace), dwSex = RaceToSex(dwRace);
	dwPos += 1;
	if (dwSex == 0)
		dwPos += 5;
	
	float fScaleX, fScaleY, fScaleZ, fPositionX, fPositionY, fPositionZ;
	if (pItemData->GetItemScale(dwPos, fScaleX, fScaleY, fScaleZ, fPositionX, fPositionY, fPositionZ))
	{
		m_GraphicThingInstance.SetScale(fScaleX, fScaleY, fScaleZ, true);
		if (m_kHorse.IsMounting())
			fPositionZ += 10.0f;
		
		m_GraphicThingInstance.SetScalePosition(fPositionX, fPositionY, fPositionZ);
	}
#endif
}

void CInstanceBase::ChangeSash(DWORD dwSash)
{
	if (!IsPC() && !IsBuffi())
		return;
	
	SetSash(dwSash);
}

void CInstanceBase::ClearSashEffect()
{
	if (!m_dwSashEffect)
		return;
	
	__EffectContainer_DetachEffect(m_dwSashEffect);
	m_dwSashEffect = 0;
}
#endif
void CInstanceBase::SetShape(DWORD eShape, float fSpecular)
{
	if (IsPoly())
	{
		m_GraphicThingInstance.SetShape(0);
	}
	else
	{
		m_GraphicThingInstance.SetShape(eShape, fSpecular);
	}

	m_eShape = eShape;
	m_awPart[CRaceData::PART_MAIN] = eShape;
}



DWORD CInstanceBase::GetWeaponType()
{
	DWORD dwWeapon = GetPart(CRaceData::PART_WEAPON);
	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(dwWeapon, &pItemData))
		return CItemData::WEAPON_NONE;

	return pItemData->GetWeaponType();
}

void CInstanceBase::__ClearWeaponRefineEffect()
{
	if (m_swordRefineEffectRight)
	{
		__DetachEffect(m_swordRefineEffectRight);
		m_swordRefineEffectRight = 0;
	}
	if (m_swordRefineEffectLeft)
	{
		__DetachEffect(m_swordRefineEffectLeft);
		m_swordRefineEffectLeft = 0;
	}
}

void CInstanceBase::__ClearArmorRefineEffect()
{
	if (m_armorRefineEffect[0])
	{
		__DetachEffect(m_armorRefineEffect[0]);
		m_armorRefineEffect[0] = 0;
	}

	if (m_armorRefineEffect[1])
	{
		__DetachEffect(m_armorRefineEffect[1]);
		m_armorRefineEffect[1] = 0;
	}
}

// #define USE_WEAPON_COSTUME_WITH_EFFECT
// #define USE_BODY_COSTUME_WITH_EFFECT
UINT CInstanceBase::__GetRefinedEffect(CItemData* pItem)
{
#ifdef ENABLE_SHINING_SYSTEM
	if (pItem->GetType() == CItemData::ITEM_TYPE_WEAPON || pItem->GetType() == CItemData::ITEM_TYPE_COSTUME)
	{
		if (!AttachShiningEffect(pItem->GetIndex()))
			TraceError("Error with Shining attachment");
	}
	else {
		if (!AttachShiningEffect(m_awPart[CRaceData::PART_WEAPON]))
			TraceError("Error with Shining attachment");
	}
#endif

#ifdef ENABLE_SIMPLE_REFINED_EFFECT_CHECK
	DWORD refine = pItem->GetRefine();
#else
	DWORD refine = max(pItem->GetRefine() + pItem->GetSocketCount()+2,CItemData::ITEM_SOCKET_MAX_NUM) - CItemData::ITEM_SOCKET_MAX_NUM;
#endif

	DWORD vnum = pItem->GetIndex();

	switch (pItem->GetType())
	{
		case CItemData::ITEM_TYPE_WEAPON:
		{
			__ClearWeaponRefineEffect();
#ifdef ELEMENT_SPELL_WORLDARD
			AttachElementEffect(pItem);
#endif

			bool enable_refine = true;

			switch (pItem->GetSubType())
			{
				case CItemData::WEAPON_SWORD:
				case CItemData::WEAPON_TWO_HANDED:
				{
					bool default_effect = true;

#ifdef ENABLE_LVL96_WEAPON_EFFECT
					if ((330 <= vnum && vnum <= 335) ||		// Dunkeldrachenschwert
						(350 <= vnum && vnum <= 355) ||		// Dunkeldrachenschneide
						(3240 <= vnum && vnum <= 3245) ||	// Dunkeldrachenklinge
						(370 <= vnum && vnum <= 375) ||		// Schlangenschwert
						(390 <= vnum && vnum <= 395) ||		// Schlangenklinge
						(3260 <= vnum && vnum <= 3265))		// Schlangengro?chwert
					{
						default_effect = false;
						enable_refine = false;
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SWORD_REFINED10;
					}
#endif

					if (default_effect)
					{
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SWORD_REFINED7 + refine - 7;
					}
				}
				break;

				case CItemData::WEAPON_DAGGER:
				{
					bool default_effect = true;

#ifdef ENABLE_LVL96_WEAPON_EFFECT
					if ((1200 <= vnum && vnum <= 1205) ||	// Dunkeldrachendolch
						(1220 <= vnum && vnum <= 1225))		// Schlangendolch
					{
						default_effect = false;
						enable_refine = false;
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED10;
						m_swordRefineEffectLeft = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED10_LEFT;
					}
#endif

					if (default_effect)
					{
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED7 + refine - 7;
						m_swordRefineEffectLeft = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED7_LEFT + refine - 7;
					}
				}
				break;

				case CItemData::WEAPON_BOW:
				{
					bool default_effect = true;

#ifdef ENABLE_LVL96_WEAPON_EFFECT
					if ((2220 <= vnum && vnum <= 2225) ||	// Dunkeldrachenbogen
						(2240 <= vnum && vnum <= 2245))		// Schlangenbogen
					{
						default_effect = false;
						enable_refine = false;
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_BOW_REFINED10;
					}
#endif

					if (default_effect)
					{
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_BOW_REFINED7 + refine - 7;
					}
				}
				break;

				case CItemData::WEAPON_BELL:
				case CItemData::WEAPON_ARROW:
				case CItemData::WEAPON_QUIVER:
				{
					bool default_effect = true;

#ifdef ENABLE_LVL96_WEAPON_EFFECT
					if ((5180 <= vnum && vnum <= 5185) ||	// Dunkeldrachenglocke
						(5210 <= vnum && vnum <= 5215))		// Schlangenglocke
					{
						default_effect = false;
						enable_refine = false;
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED10;
					}
#endif

					if (default_effect)
					{
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED7 + refine - 7;
					}
				}
				break;

				case CItemData::WEAPON_FAN:
				{
					bool default_effect = true;

#ifdef ENABLE_LVL96_WEAPON_EFFECT
					if ((7320 <= vnum && vnum <= 7325) ||	// Dunkeldrachenf?her
						(7340 <= vnum && vnum <= 7345))		// Schlangenf?her
					{
						default_effect = false;
						enable_refine = false;
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_FANBELL_REFINED10;
					}
#endif

					if (default_effect)
					{
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_FANBELL_REFINED7 + refine - 7;
					}
				}
				break;

#ifdef ENABLE_WOLFMAN_CHARACTER
				case CItemData::WEAPON_CLAW:
					m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED7 + refine - 7;
					m_swordRefineEffectLeft = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED7_LEFT + refine - 7;
					break;
#endif

				default:
					TraceError("Weapon set Default!");
					m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SWORD_REFINED7 + refine - 7;
					break;
			}

			if ((refine < 7) && enable_refine)
				return 0;

			if (m_swordRefineEffectRight)
				m_swordRefineEffectRight = __AttachEffect(m_swordRefineEffectRight);
			if (m_swordRefineEffectLeft)
				m_swordRefineEffectLeft = __AttachEffect(m_swordRefineEffectLeft);
		}
		break;

		case CItemData::ITEM_TYPE_ARMOR:
		{
			__ClearArmorRefineEffect();

			if (pItem->GetSubType() == CItemData::ARMOR_BODY)
			{
				bool default_effect = true;
				bool enable_refine = true;

	/*----------Bubble-Armor----------*/
				if ((12010 <= vnum && vnum <= 12019) ||	// Blaustahl-Panzer
					(12020 <= vnum && vnum <= 12029) ||	// Blauer Drachenanzug
					(12030 <= vnum && vnum <= 12039) ||	// Auraplattenpanzer
					(12040 <= vnum && vnum <= 12049)	// Kleidung des Drachen
#ifdef ENABLE_WOLFMAN_CHARACTER
					|| (21080 <= vnum && vnum <= 21089)	// Waldl?ferpanzer
#endif
					)
				{
					default_effect = false;
					m_armorRefineEffect[0] = EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL;	// Bubbles-Effekt
					m_armorRefineEffect[1] = EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL2;	// Blue-Shining-Effekt
				}

#ifdef ENABLE_LVL96_ARMOR_EFFECT
	/*----------96-Armor----------*/
				if ((21220 <= vnum && vnum <= 21225) ||	// Dunkeldrachenr?tung		Warrior
					(21240 <= vnum && vnum <= 21245) ||	// Winddrachenr?tung		Assassin
					(21260 <= vnum && vnum <= 21265) ||	// Magiedrachenr?tung		Sura
					(21280 <= vnum && vnum <= 21285)	// G?terdrachenr?tung		Shaman
#	ifdef ENABLE_WOLFMAN_CHARACTER
					|| (21300 <= vnum && vnum <= 21305)	// Himmelsdrachenr?tung	Wolfman
#	endif
					|| (21320 <= vnum && vnum <= 21325)	// Schlangenmantel
					|| (21340 <= vnum && vnum <= 21345)	// Schlangenwams
					|| (21350 <= vnum && vnum <= 21365)	// B?er Schlangenmantel
					|| (21370 <= vnum && vnum <= 21385)	// Rein. Schlangenmantel
					|| (21390 <= vnum && vnum <= 21405)	// Him. Schlangenmantel
					)
				{
					default_effect = false;
					enable_refine = false;
					m_armorRefineEffect[0] = EFFECT_REFINED + EFFECT_BODYARMOR_REFINED10;
				}
#endif

#ifdef ENABLE_LVL115_ARMOR_EFFECT
	/*----------Blitz[105-115]-Armor----------*/
				if (
					// Warrior
					(20760 <= vnum && vnum <= 20769) ||	// Polarlichtpanzer
					(20860 <= vnum && vnum <= 20869) ||	// Walknochenpanzer
					(20810 <= vnum && vnum <= 20819) ||	// Magmaglutpanzer
					(20910 <= vnum && vnum <= 20919) ||	// Ascheregenpanzer

					// Sura
					(20780 <= vnum && vnum <= 20789) ||	// Teufelskrallenpanzer
					(20880 <= vnum && vnum <= 20889) ||	// Irrlichterpanzer
					(20830 <= vnum && vnum <= 20839) ||	// H?lenrittpanzer
					(20930 <= vnum && vnum <= 20930) ||	// Schwarzfeuerpanzer

					// Assassin
					(20770 <= vnum && vnum <= 20770) ||	// Anzug des Hinterhalts
					(20870 <= vnum && vnum <= 20870) ||	// Verwandlungsanzug
					(20820 <= vnum && vnum <= 20820) ||	// Blutmondanzug
					(20920 <= vnum && vnum <= 20920) ||	// Todesnachtanzug

					// Shaman
					(20790 <= vnum && vnum <= 20790) ||	// Kleidung des Glaubens
					(20890 <= vnum && vnum <= 20890) ||	// Kleidung der Harmonie
					(20840 <= vnum && vnum <= 20840) ||	// Kleidung des Feuers
					(20940 <= vnum && vnum <= 20940)	// Kleidung des Mondes

#	ifdef ENABLE_WOLFMAN_CHARACTER
				// Wolfman
					|| (20800 <= vnum && vnum <= 20800)	// Schattenk?pferpanzer
					|| (20900 <= vnum && vnum <= 20900)	// Mondsteinplattenpanzer
					|| (20850 <= vnum && vnum <= 20850)	// Achatplattenpanzer
					|| (20950 <= vnum && vnum <= 20950)	// Onyxplattenpanzer
#	endif
					)
				{
					enable_refine = false;
					default_effect = false;
					m_armorRefineEffect[0] = EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL3;
					if (refine > 6)
						m_armorRefineEffect[1] = EFFECT_REFINED + EFFECT_BODYARMOR_REFINED7 + refine - 7;
				}
#endif

				if (default_effect)
				{
					m_armorRefineEffect[0] = EFFECT_REFINED + EFFECT_BODYARMOR_REFINED7 + refine - 7;
				}

				if ((refine < 7) && enable_refine)
					return 0;

				if (m_armorRefineEffect[0])
					m_armorRefineEffect[0] = __AttachEffect(m_armorRefineEffect[0]);
				if (m_armorRefineEffect[1])
					m_armorRefineEffect[1] = __AttachEffect(m_armorRefineEffect[1]);
			}

			break;
		}	//END ITEM_TYPE_ARMOR

		case CItemData::ITEM_TYPE_COSTUME:
	#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
			if (pItem->GetSubType() == CItemData::COSTUME_WEAPON)
			{
				__ClearWeaponRefineEffect();

	#ifdef USE_WEAPON_COSTUME_WITH_EFFECT
				switch(pItem->GetValue(3))
				{
					case CItemData::WEAPON_DAGGER:
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED9;
						m_swordRefineEffectLeft = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED9_LEFT;
						break;
					case CItemData::WEAPON_FAN:
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_FANBELL_REFINED9;
						break;
					case CItemData::WEAPON_ARROW:
					case CItemData::WEAPON_QUIVER:
					case CItemData::WEAPON_BELL:
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED9;
						break;
					case CItemData::WEAPON_BOW:
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_BOW_REFINED9;
						break;
	#ifdef ENABLE_WOLFMAN_CHARACTER
					case CItemData::WEAPON_CLAW:
						m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED9;
						m_swordRefineEffectLeft = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED9_LEFT;
						break;
	#endif
					default:
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SWORD_REFINED9;
				}
				if (m_swordRefineEffectRight)
					m_swordRefineEffectRight = __AttachEffect(m_swordRefineEffectRight);
				if (m_swordRefineEffectLeft)
					m_swordRefineEffectLeft = __AttachEffect(m_swordRefineEffectLeft);
	#endif //USE_WEAPON_COSTUME_WITH_EFFECT
				break;
			}
	#endif
	#ifdef USE_BODY_COSTUME_WITH_EFFECT
			if (pItem->GetSubType() == CItemData::COSTUME_BODY)
			{
				__ClearArmorRefineEffect();

				// lvl80 armor's effect (blue smoke+bubbles)
				// {
					// __AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL);
					// __AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL2);
				// }

				// lvl105 armor's effect (sparkles)
				// {
					// __AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL3);
				// }

				m_armorRefineEffect = EFFECT_REFINED+EFFECT_BODYARMOR_REFINED9;
				__AttachEffect(m_armorRefineEffect);
				break;
			}
	#endif //USE_BODY_COSTUME_WITH_EFFECT
			break;
	}
	return 0;
}

bool CInstanceBase::SetWeapon(DWORD eWeapon, DWORD eArrow)
{
	if (IsPoly())
		return false;
	
	if (__IsShapeAnimalWear())
		return false;
	
	m_awPart[CRaceData::PART_ARROW_TYPE] = eArrow;
	if (__IsChangableWeapon(eWeapon) == false)
		eWeapon = 0;
	
	m_GraphicThingInstance.AttachWeapon(eWeapon);
	m_awPart[CRaceData::PART_WEAPON] = eWeapon;

	//Weapon Effect
	CItemData * pItemData;
	if (CItemManager::Instance().GetItemDataPointer(eWeapon, &pItemData))
	{
		__GetRefinedEffect(pItemData);
#ifdef ENABLE_SHINING_TABLE_SYSTEM
		_GetShiningTableEffect(pItemData);
#endif
	}
	else{
		__ClearWeaponRefineEffect();
#ifdef ENABLE_SHINING_TABLE_SYSTEM
		__ClearWeaponShiningEffect();
#endif
#ifdef ENABLE_SHINING_SYSTEM
		if (!AttachShiningEffect(0))
			TraceError("Error with Shining attachment");
#endif	
	}

	return true;
}

void CInstanceBase::ChangeWeapon(DWORD eWeapon, DWORD eArrow)
{
	CItemData * pItemData;
	m_awPart[CRaceData::PART_ARROW_TYPE] = eArrow;
	
	if (CItemManager::Instance().GetItemDataPointer(eWeapon, &pItemData))
	{
		if (!m_kHorse.IsMounting() && pItemData->GetSubType() == CItemData::WEAPON_BOW)
		{
			if (eArrow == CItemData::WEAPON_QUIVER)
				SetMotionMode(CRaceMotionData::MODE_BOW_SPECIAL);
			else
				SetMotionMode(CRaceMotionData::MODE_BOW);
		}
		else if (!m_kHorse.IsMounting() && pItemData->GetType() == CItemData::ITEM_TYPE_COSTUME && pItemData->GetValue(3) == CItemData::WEAPON_BOW)
		{
			if (eArrow == CItemData::WEAPON_QUIVER)
				SetMotionMode(CRaceMotionData::MODE_BOW_SPECIAL);
			else
				SetMotionMode(CRaceMotionData::MODE_BOW);
		}
	}

	if (eWeapon == m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON))
		return;
	
	if (SetWeapon(eWeapon, eArrow))
		RefreshState(CRaceMotionData::NAME_WAIT, true);
}

bool CInstanceBase::ChangeArmor(DWORD dwArmor)
{
	DWORD eShape;
	__ArmorVnumToShape(dwArmor, &eShape);

	if (GetShape()==eShape)
		return false;

	CAffectFlagContainer kAffectFlagContainer;
	kAffectFlagContainer.CopyInstance(m_kAffectFlagContainer);

	DWORD dwVID = GetVirtualID();
	DWORD dwRace = GetRace();
	DWORD eHair = GetPart(CRaceData::PART_HAIR);
	#ifdef ENABLE_SASH_SYSTEM
	DWORD dwSash = GetPart(CRaceData::PART_SASH);
	#endif

	DWORD eWeapon = GetPart(CRaceData::PART_WEAPON);
	float fRot = GetRotation();
	float fAdvRot = GetAdvancingRotation();

	if (IsWalking())
		EndWalking();

	// 2004.07.25.myevan.이펙트 안 붙는 문제
	//////////////////////////////////////////////////////
	__ClearAffects();
	//////////////////////////////////////////////////////

	if (!SetRace(dwRace))
	{
		TraceError("CPythonCharacterManager::ChangeArmor - SetRace VID[%d] Race[%d] ERROR", dwVID, dwRace);
		return false;
	}

	SetArmor(dwArmor);
	SetHair(eHair);
	#ifdef ENABLE_SASH_SYSTEM
	SetSash(dwSash);
	#endif
	SetWeapon(eWeapon);
#ifdef ENABLE_AURA_SYSTEM
	DWORD eAura = GetPart(CRaceData::PART_AURA);
	SetAura(eAura);
#endif
	SetRotation(fRot);
	SetAdvancingRotation(fAdvRot);

	__AttachHorseSaddle();

	RefreshState(CRaceMotionData::NAME_WAIT, TRUE);

	// 2004.07.25.myevan.이펙트 안 붙는 문제
	/////////////////////////////////////////////////
	SetAffectFlagContainer(kAffectFlagContainer);
	/////////////////////////////////////////////////

	CActorInstance::IEventHandler& rkEventHandler=GetEventHandlerRef();
	rkEventHandler.OnChangeShape();

	return true;
}

bool CInstanceBase::__IsShapeAnimalWear()
{
	if (100 == GetShape() ||
		101 == GetShape() ||
		102 == GetShape() ||
		103 == GetShape())
		return true;

	return false;
}

DWORD CInstanceBase::__GetRaceType()
{
	return m_eRaceType;
}


void CInstanceBase::RefreshState(DWORD dwMotIndex, bool isLoop)
{
	DWORD dwPartItemID = m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON);

	BYTE byItemType = 0xff;
	BYTE bySubType = 0xff;

	CItemManager & rkItemMgr = CItemManager::Instance();
	CItemData * pItemData;

	if (rkItemMgr.GetItemDataPointer(dwPartItemID, &pItemData))
	{
		byItemType = pItemData->GetType();
		bySubType = pItemData->GetWeaponType();
	}

	if (IsPoly())
	{
		SetMotionMode(CRaceMotionData::MODE_GENERAL);
	}
	else if (IsWearingDress())
	{
		SetMotionMode(CRaceMotionData::MODE_WEDDING_DRESS);
	}
	else if (IsHoldingPickAxe())
	{
		if (m_kHorse.IsMounting())
		{
			SetMotionMode(CRaceMotionData::MODE_HORSE);
		}
		else
		{
			SetMotionMode(CRaceMotionData::MODE_GENERAL);
		}
	}
	else if (CItemData::ITEM_TYPE_ROD == byItemType)
	{
		if (m_kHorse.IsMounting())
		{
			SetMotionMode(CRaceMotionData::MODE_HORSE);
		}
		else
		{
			SetMotionMode(CRaceMotionData::MODE_FISHING);
		}
	}
	else if (m_kHorse.IsMounting())
	{
		switch (bySubType)
		{
			case CItemData::WEAPON_SWORD:
				SetMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
				break;

			case CItemData::WEAPON_TWO_HANDED:
				SetMotionMode(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD); // Only Warrior
				break;

			case CItemData::WEAPON_DAGGER:
				SetMotionMode(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD); // Only Assassin
				break;

			case CItemData::WEAPON_FAN:
				SetMotionMode(CRaceMotionData::MODE_HORSE_FAN); // Only Shaman
				break;

			case CItemData::WEAPON_BELL:
				SetMotionMode(CRaceMotionData::MODE_HORSE_BELL); // Only Shaman
				break;

			case CItemData::WEAPON_BOW:
				SetMotionMode(CRaceMotionData::MODE_HORSE_BOW); // Only Shaman
				break;
#ifdef ENABLE_WOLFMAN_CHARACTER
			case CItemData::WEAPON_CLAW:
				SetMotionMode(CRaceMotionData::MODE_HORSE_CLAW); // Only Wolfman
				break;
#endif
			default:
				SetMotionMode(CRaceMotionData::MODE_HORSE);
				break;
		}
	}
	else
	{
		switch (bySubType)
		{
			case CItemData::WEAPON_SWORD:
				SetMotionMode(CRaceMotionData::MODE_ONEHAND_SWORD);
				break;

			case CItemData::WEAPON_TWO_HANDED:
				SetMotionMode(CRaceMotionData::MODE_TWOHAND_SWORD); // Only Warrior
				break;

			case CItemData::WEAPON_DAGGER:
				SetMotionMode(CRaceMotionData::MODE_DUALHAND_SWORD); // Only Assassin
				break;

			case CItemData::WEAPON_BOW:
				if (m_awPart[CRaceData::PART_ARROW_TYPE] == CItemData::WEAPON_QUIVER )
					SetMotionMode(CRaceMotionData::MODE_BOW_SPECIAL);
				else
					SetMotionMode(CRaceMotionData::MODE_BOW);

				//SetMotionMode(CRaceMotionData::MODE_BOW); // Only Assassin
				break;

			case CItemData::WEAPON_FAN:
				SetMotionMode(CRaceMotionData::MODE_FAN); // Only Shaman
				break;

			case CItemData::WEAPON_BELL:
				SetMotionMode(CRaceMotionData::MODE_BELL); // Only Shaman
				break;
#ifdef ENABLE_WOLFMAN_CHARACTER
			case CItemData::WEAPON_CLAW:
				SetMotionMode(CRaceMotionData::MODE_CLAW); // Only Wolfman
				break;
#endif
			case CItemData::WEAPON_ARROW:
			case CItemData::WEAPON_QUIVER:
			default:
				SetMotionMode(CRaceMotionData::MODE_GENERAL);
				break;
		}
	}

	if (isLoop)
		m_GraphicThingInstance.InterceptLoopMotion(dwMotIndex);
	else
		m_GraphicThingInstance.InterceptOnceMotion(dwMotIndex);

	RefreshActorInstance();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Device

void CInstanceBase::RegisterBoundingSphere()
{
	// Stone 일 경우 DeforomNoSkin 을 하면
	// 낙하하는 애니메이션 같은 경우 애니메이션이
	// 바운드 박스에 영향을 미쳐 컬링이 제대로 이루어지지 않는다.
	if (!IsStone())
	{
		m_GraphicThingInstance.DeformNoSkin();
	}

	m_GraphicThingInstance.RegisterBoundingSphere();
}

bool CInstanceBase::CreateDeviceObjects()
{
	return m_GraphicThingInstance.CreateDeviceObjects();
}

void CInstanceBase::DestroyDeviceObjects()
{
	m_GraphicThingInstance.DestroyDeviceObjects();
}

void CInstanceBase::__AttachBossEffect()
{
	if (!__IsExistMainInstance())
		return;

	CInstanceBase* pkInstMain = __GetMainInstancePtr();

	if (IsWarp())
		return;
	if (IsObject())
		return;
	if (IsFlag())
		return;
	if (IsResource())
		return;

	__EffectContainer_AttachEffect(EFFECT_BOSS);
}

void CInstanceBase::Destroy()
{
	DetachTextTail();

	DismountHorse();

	m_kQue_kCmdNew.clear();

	__EffectContainer_Destroy();
	__StoneSmoke_Destroy();

	if (__IsMainInstance())
		__ClearMainInstance();

	m_GraphicThingInstance.Destroy();

	__Initialize();
}

void CInstanceBase::__InitializeRotationSpeed()
{
	SetRotationSpeed(c_fDefaultRotationSpeed);
}

void CInstanceBase::__Warrior_Initialize()
{
	m_kWarrior.m_dwGeomgyeongEffect=0;
}

void CInstanceBase::__Initialize()
{
	__Warrior_Initialize();
	__StoneSmoke_Inialize();
	__EffectContainer_Initialize();
	__InitializeRotationSpeed();

	SetEventHandler(CActorInstance::IEventHandler::GetEmptyPtr());

	m_kAffectFlagContainer.Clear();

	m_dwLevel = 0;
	m_dwGuildID = 0;
#ifdef ENABLE_YOHARA_SYSTEM
	m_dwConquerorLevel = 0;
#endif
	m_dwEmpireID = 0;
#ifdef ENABLE_AURA_SYSTEM
	m_auraRefineEffect = 0;
#endif
	m_eType = 0;
	m_eRaceType = 0;
	m_eShape = 0;
	m_dwRace = 0;
	m_dwVirtualNumber = 0;
#ifdef __BUFFI_SUPPORT__
	m_bIsBuffi = false;
#endif
	m_dwBaseCmdTime=0;
	m_dwBaseChkTime=0;
	m_dwSkipTime=0;

	m_GraphicThingInstance.Initialize();

	m_dwAdvActorVID=0;
	m_dwLastDmgActorVID=0;

	m_nAverageNetworkGap=0;
	m_dwNextUpdateHeightTime=0;

	// Moving by keyboard
	m_iRotatingDirection = DEGREE_DIRECTION_SAME;

	// Moving by mouse
	m_isTextTail = FALSE;
	m_isGoing = FALSE;
	NEW_SetSrcPixelPosition(TPixelPosition(0, 0, 0));
	NEW_SetDstPixelPosition(TPixelPosition(0, 0, 0));

	m_kPPosDust = TPixelPosition(0, 0, 0);


	m_kQue_kCmdNew.clear();

	m_dwLastComboIndex = 0;

	m_swordRefineEffectRight = 0;
	m_swordRefineEffectLeft = 0;
	m_armorRefineEffect[0] = 0;
	m_armorRefineEffect[1] = 0;
	#ifdef ENABLE_SASH_SYSTEM
	m_dwSashEffect = 0;
	#endif
	m_sAlignment = 0;
	m_byPKMode = 0;
	m_isKiller = false;
	m_isPartyMember = false;

	m_bEnableTCPState = TRUE;

	m_stName = "";
#if defined(WJ_COMBAT_ZONE)
	combat_zone_rank = 0;
	combat_zone_points = 0;
#endif
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	m_stLanguage = CPythonApplication::LOCALE_EN;
#endif
	memset(m_awPart, 0, sizeof(m_awPart));
	memset(m_adwCRCAffectEffect, 0, sizeof(m_adwCRCAffectEffect));
	//memset(m_adwCRCEmoticonEffect, 0, sizeof(m_adwCRCEmoticonEffect));
	memset(&m_kMovAfterFunc, 0, sizeof(m_kMovAfterFunc));

	m_bDamageEffectType = false;
	m_dwDuelMode = DUEL_NONE;
	m_dwEmoticonTime = 0;
	m_IsAlwaysRender = false;
#ifdef ELEMENT_SPELL_WORLDARD
	m_dwElementsEffect = 0;
	m_swordElementEffectRight = 0;
	m_swordElementEffectLeft = 0;
#endif
#ifdef ENABLE_SHINING_SYSTEM
	for (int i = 0; i < CHR_SHINING_NUM; i++) {
		m_dwShining[i] = 0;
	}
	m_armorShining = 0;
	m_swordShiningRight = 0;
	m_swordShiningLeft = 0;
	m_specialShining = 0;
#endif

#ifdef ENABLE_SHINING_TABLE_SYSTEM
	__ClearWeaponShiningEffect(false);
	__ClearArmorShiningEffect(false);
	__ClearAcceShiningEffect(false);
#endif

}

CInstanceBase::CInstanceBase()
{
	__Initialize();
}

CInstanceBase::~CInstanceBase()
{
	Destroy();
}


void CInstanceBase::GetBoundBox(D3DXVECTOR3 * vtMin, D3DXVECTOR3 * vtMax)
{
	m_GraphicThingInstance.GetBoundBox(vtMin, vtMax);
}

#ifdef ELEMENT_SPELL_WORLDARD

void CInstanceBase::SetElementEffect(BYTE wElementsEffect)
{
	if (m_dwElementsEffect < 0)
		return;

	m_dwElementsEffect = wElementsEffect;
}

BYTE CInstanceBase::GetElementEffect()
{
	if (m_dwElementsEffect < 0)
		return 0;

	return m_dwElementsEffect;
}

void CInstanceBase::ClearElementEffect()
{
	if (m_swordElementEffectRight)
	{
		__DetachEffect(m_swordElementEffectRight);
		m_swordElementEffectRight = 0;
	}
	if (m_swordElementEffectLeft)
	{
		__DetachEffect(m_swordElementEffectLeft);
		m_swordElementEffectLeft = 0;
	}
}


void CInstanceBase::AttachElementEffect(CItemData* pItem)
{

	ClearElementEffect();


	if (GetElementEffect() != 0){
		switch(pItem->GetSubType())
		{
			case CItemData::WEAPON_DAGGER:
				m_swordElementEffectRight = EFFECT_ELEMENTS_WEAPON+EFFECT_ELEMENTS_SMALLSWORD+GetElementEffect();
				m_swordElementEffectLeft = EFFECT_ELEMENTS_WEAPON+EFFECT_ELEMENTS_SMALLSWORD_LEFT+GetElementEffect();
				break;
			case CItemData::WEAPON_FAN:
				m_swordElementEffectRight = EFFECT_ELEMENTS_WEAPON+EFFECT_ELEMENTS_FANBELL+GetElementEffect();
				break;
			case CItemData::WEAPON_ARROW:
			case CItemData::WEAPON_QUIVER:
			case CItemData::WEAPON_BELL:
				m_swordElementEffectRight = EFFECT_ELEMENTS_WEAPON+EFFECT_ELEMENTS_SMALLSWORD+GetElementEffect();
				break;
			case CItemData::WEAPON_BOW:
				m_swordElementEffectRight = EFFECT_ELEMENTS_WEAPON+EFFECT_ELEMENTS_BOW+GetElementEffect();
				break;
#ifdef ENABLE_WOLFMAN_CHARACTER
			case CItemData::WEAPON_CLAW:
				m_swordElementEffectRight = EFFECT_ELEMENTS_WEAPON + EFFECT_ELEMENTS_CLAW + GetElementEffect();
				m_swordElementEffectLeft = EFFECT_ELEMENTS_WEAPON + EFFECT_ELEMENTS_CLAW_LEFT + GetElementEffect();
				break;
#endif
			default:
				m_swordElementEffectRight = EFFECT_ELEMENTS_WEAPON+EFFECT_ELEMENTS_SWORD+GetElementEffect();
		}

		if (m_swordElementEffectRight)
			m_swordElementEffectRight = __AttachEffect(m_swordElementEffectRight);
		if (m_swordElementEffectLeft)
			m_swordElementEffectLeft = __AttachEffect(m_swordElementEffectLeft);
	}
}


#endif

BOOL CInstanceBase::IsMount()
{
	return m_GraphicThingInstance.IsMount();
}

#ifdef ENABLE_NEW_PET_SYSTEM
BOOL CInstanceBase::IsPet()
{
	return m_GraphicThingInstance.IsPet();
}
BOOL CInstanceBase::IsNewPet()
{
	return m_GraphicThingInstance.IsNewPet();
}
void CInstanceBase::RefreshModelScale()
{
	if (IsNewPet())
	{
		float fScale = static_cast<float>((float(m_dwLevel + 70) * 0.8f) / 100.0f);
		m_GraphicThingInstance.SetScale(fScale, fScale, fScale, true);
		//m_GraphicThingInstance.SetScaleWorld(fScale, fScale, fScale);
	}
}
#endif
#ifdef ENABLE_AURA_SYSTEM
void CInstanceBase::ChangeAura(DWORD eAura)
{
	if (m_GraphicThingInstance.GetPartItemID(CRaceData::PART_AURA) != eAura)
		SetAura(eAura);
}

bool CInstanceBase::SetAura(DWORD eAura)
{
	if (!IsPC() || IsPoly() || __IsShapeAnimalWear())
		return false;

	m_GraphicThingInstance.ChangePart(CRaceData::PART_AURA, eAura);
	if (!eAura)
	{
		if (m_auraRefineEffect)
		{
			__DetachEffect(m_auraRefineEffect);
			m_auraRefineEffect = 0;
		}
		m_awPart[CRaceData::PART_AURA] = 0;
		return true;
	}

	CItemData* pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(eAura, &pItemData))
	{
		if (m_auraRefineEffect)
		{
			__DetachEffect(m_auraRefineEffect);
			m_auraRefineEffect = 0;
		}
		m_awPart[CRaceData::PART_AURA] = 0;
		return true;
	}

	BYTE byRace = (BYTE)GetRace();
	BYTE byJob = (BYTE)RaceToJob(byRace);
	BYTE bySex = (BYTE)RaceToSex(byRace);

	D3DXVECTOR3 v3MeshScale = pItemData->GetAuraMeshScaleVector(byJob, bySex);
	float fParticleScale = pItemData->GetAuraParticleScale(byJob, bySex);
	m_auraRefineEffect = m_GraphicThingInstance.AttachEffectByID(0, "Bip01 Spine2", pItemData->GetAuraEffectID(), NULL, fParticleScale, &v3MeshScale);
	m_awPart[CRaceData::PART_AURA] = eAura;
	return true;
}
#endif

#ifdef ENABLE_DEFENSAWESHIP
BOOL CInstanceBase::IsHydraNPC()
{
	switch (m_dwVirtualNumber)
	{
		case 6801:
			return TRUE;
	}
	return FALSE;
}
#endif
#ifdef ENABLE_SHINING_SYSTEM
void CInstanceBase::SetShining(BYTE num, DWORD dwShining)
{
	if (m_dwShining[num] == dwShining || dwShining < 0)
		return;

	CItemManager::Instance().SelectItemData(dwShining);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();

	if (pItemData && pItemData->GetType() == CItemData::ITEM_TYPE_SHINING || dwShining == 0)
	{
		m_dwShining[num] = dwShining;
		if (!AttachShiningEffect(m_awPart[CRaceData::PART_WEAPON]))
			TraceError("Error with shining attachment");
	}
}
INT CInstanceBase::__GetShiningEffect(WORD slot)
{
	if (m_dwShining[slot] == 0)
		return -1;

	CItemManager::Instance().SelectItemData(m_dwShining[slot]);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();

	if (pItemData && pItemData->GetType() == CItemData::ITEM_TYPE_SHINING)
	{
		return pItemData->GetValue(0);
	}
	return -1;
}

bool CInstanceBase::ClearShiningEffect()
{

	if (m_swordShiningLeft)
		__DetachEffect(m_swordShiningLeft);
	if (m_swordShiningRight)
		__DetachEffect(m_swordShiningRight);
	if (m_armorShining)
		__DetachEffect(m_armorShining);
	if (m_specialShining)
		__DetachEffect(m_specialShining);

	m_swordShiningLeft = 0;
	m_swordShiningRight = 0;
	m_armorShining = 0;
	m_specialShining = 0;
	return true;
}

bool CInstanceBase::AttachShiningEffect(DWORD dwWeapon)
{
	CItemData* pItemData = NULL;
	if (!ClearShiningEffect())
		TraceError("Error while clear shining");

	DWORD weaponVnum = m_awPart[CRaceData::PART_WEAPON];
	CItemData* pItem;
	if (weaponVnum > 0)
	{
		CItemManager::Instance().SelectItemData(weaponVnum);
		pItem = CItemManager::Instance().GetSelectedItemDataPointer();
	}
	else
		pItem = NULL;

	for (int i = 0; i < CHR_SHINING_NUM; i++)
	{
		if (__GetShiningEffect(i) >= 0)
		{
			if (i == 0)
			{
				if (pItem)
				{

					if (pItem->GetType() == CItemData::ITEM_TYPE_WEAPON)
					{
						switch (pItem->GetSubType())
						{
						case CItemData::WEAPON_DAGGER:
						//case CItemData::WEAPON_POLE://elf
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_DAGGER_R + __GetShiningEffect(i);
							m_swordShiningLeft = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_DAGGER_L + __GetShiningEffect(i);
							break;
						//case CItemData::WEAPON_CLAW:
						//	m_swordShiningRight[i] = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_FAN + __GetShiningEffect(i);
						//	m_swordShiningLeft[i] = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_FAN + 10 + __GetShiningEffect(i);
						case CItemData::WEAPON_FAN:
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_FAN + __GetShiningEffect(i);
							break;
						case CItemData::WEAPON_BELL:
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_BELL + __GetShiningEffect(i);
							break;
						case CItemData::WEAPON_TWO_HANDED:
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_TWO_HANDED + __GetShiningEffect(i);
							break;
						case CItemData::WEAPON_BOW:
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_BOW + __GetShiningEffect(i);
							break;
						default:
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_SWORD + __GetShiningEffect(i);
						}
					}
					else if (pItem->GetType() == CItemData::ITEM_TYPE_COSTUME)
					{
						switch (pItem->GetValue(3))
						{
						case CItemData::WEAPON_DAGGER:
						//case CItemData::WEAPON_POLE://elf
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_DAGGER_R + __GetShiningEffect(i);
							m_swordShiningLeft = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_DAGGER_L + __GetShiningEffect(i);
							break;
						//case CItemData::WEAPON_CLAW:
						//	m_swordShiningRight[i] = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_FAN + __GetShiningEffect(i);
						//	m_swordShiningLeft[i] = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_FAN + 10 + __GetShiningEffect(i);
						case CItemData::WEAPON_FAN:
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_FAN + __GetShiningEffect(i);
							break;
						case CItemData::WEAPON_BELL:
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_BELL + __GetShiningEffect(i);
							break;
						case CItemData::WEAPON_TWO_HANDED:
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_TWO_HANDED + __GetShiningEffect(i);
							break;
						case CItemData::WEAPON_BOW:
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_BOW + __GetShiningEffect(i);
							break;
						default:
							m_swordShiningRight = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_SWORD + __GetShiningEffect(i);
						}
					}
					if (m_swordShiningRight)
						m_swordShiningRight = __AttachEffect(m_swordShiningRight);

					if (m_swordShiningLeft)
						m_swordShiningLeft = __AttachEffect(m_swordShiningLeft);
				}

			}
			else if (i == 1)
			{
				m_armorShining = EFFECT_SHINING_ARMOR + __GetShiningEffect(i);
				if (m_armorShining)
					m_armorShining = __AttachEffect(m_armorShining);
			}
			else if (i == 2)
			{
				m_specialShining = EFFECT_SHINING_SPECIAL + __GetShiningEffect(i);
				if (m_specialShining)
					m_specialShining = __AttachEffect(m_specialShining);
			}

		}
	}
	return true;
}
#endif


void CInstanceBase::AttachWikiAffect(DWORD effectIndex)
{	
	const DWORD newAffect = __AttachEffect(effectIndex);
	m_vecWikiEffects.push_back(newAffect);
}
#ifdef ENABLE_SHINING_SYSTEM
void CInstanceBase::SetShiningRender(DWORD slot, DWORD itemIndex)
{
	m_dwShining[slot] = itemIndex;
	AttachShiningEffect(m_awPart[CRaceData::PART_WEAPON]);
}
DWORD CInstanceBase::GetShiningData(BYTE slot)
{
	if (slot >= _countof(m_dwShining))
		return 0;
	return m_dwShining[slot];
}
#endif
void CInstanceBase::RemoveWikiAffect(DWORD effectIndex)
{
	const auto it = std::find(m_vecWikiEffects.begin(), m_vecWikiEffects.end(), effectIndex);
	if (it != m_vecWikiEffects.end())
	{
		__DetachEffect(effectIndex);
		m_vecWikiEffects.erase(it);
	}
}
void CInstanceBase::ClearWikiAffect()
{
#ifdef ENABLE_SHINING_SYSTEM
	memset(&m_dwShining, 0, sizeof(m_dwShining));
#endif
	for (const auto& effectIndex : m_vecWikiEffects)
		__DetachEffect(effectIndex);
	m_vecWikiEffects.clear();
}
bool CInstanceBase::SetMotionIndex(DWORD motionIndex)
{
	return m_GraphicThingInstance.InterceptOnceMotion(motionIndex);
}
void CInstanceBase::SetAlwaysRender(bool val)
{
	m_IsAlwaysRender = val;
	m_GraphicThingInstance.SetScale(1.0, 1.0, 1.0, true);
}
DWORD CInstanceBase::GetPartData(BYTE partIndex)
{
	if (partIndex >= _countof(m_awPart))
		return 0;
	return m_awPart[partIndex];
}

#ifdef STONE_SCALE
void CInstanceBase::UpdateStoneScale()
{
	if (!IsStone())
		return;

	const float StoneScale = CPythonSystem::Instance().GetStoneScale();
	m_GraphicThingInstance.SetScale(StoneScale, StoneScale, StoneScale, true);
	DetachTextTail();
	AttachTextTail();
	RefreshTextTail();
}
#endif

#ifdef ENABLE_SHINING_TABLE_SYSTEM
void CInstanceBase::_GetShiningTableEffect(CItemData* pItem)
{
	bool removeRefineEffect = false; // fixed lightwork shining

	const auto& shiningPath = CItemManager::Instance().GetItemShiningPath(pItem->GetIndex());

	if (!shiningPath.has_value())
		return;

	if (pItem->GetType() == CItemData::ITEM_TYPE_WEAPON)
	{
		__ClearWeaponShiningEffect();

		if (removeRefineEffect)
		{
			__ClearWeaponRefineEffect();
		}

		if (pItem->GetSubType() == CItemData::WEAPON_BOW)
		{
			__AttachWeaponShiningEffect(shiningPath.value().c_str(), "PART_WEAPON_LEFT");
		}

		else
		{
			//Check for double sided weapons or fan which is attached on both sides when mounted.
			bool twoSidedWeapon = pItem->GetSubType() == CItemData::WEAPON_DAGGER || (IsMountingHorse() && pItem->GetSubType() == CItemData::WEAPON_FAN);
			if (twoSidedWeapon)
			{
				__AttachWeaponShiningEffect(shiningPath.value().c_str(), "PART_WEAPON_LEFT");
			}
			__AttachWeaponShiningEffect(shiningPath.value().c_str(), "PART_WEAPON");
		}
	}
#ifdef ENABLE_COSTUME_SYSTEM
	else if (pItem->GetType() == CItemData::ITEM_TYPE_COSTUME)
	{
#ifdef ENABLE_SASH_SYSTEM
		if (pItem->GetSubType() == CItemData::COSTUME_SASH
			|| pItem->GetSubType() == CItemData::COSTUME_NEW_SASH)
		{
			if (pItem->GetIndex() == 86057 || pItem->GetIndex() == 49073 || pItem->GetIndex() == 49074 || pItem->GetIndex() == 49067 || pItem->GetIndex() == 49068 || pItem->GetIndex() == 49069 || pItem->GetIndex() == 49070 || pItem->GetIndex() == 49071 || pItem->GetIndex() == 49072 || 
			pItem->GetIndex() == 49106 || pItem->GetIndex() == 49107 || pItem->GetIndex() == 49100 || pItem->GetIndex() == 49101 || pItem->GetIndex() == 49102 || pItem->GetIndex() == 49103 || pItem->GetIndex() == 49104 || pItem->GetIndex() == 49105
				)
			{
				__AttachAcceShiningEffect(shiningPath.value().c_str(), "Bip01 Spine2");
			}
			else
			{
				__ClearAcceShiningEffect();
			}
		}
#endif
		else if (pItem->GetSubType() == CItemData::COSTUME_BODY)
		{
			if (removeRefineEffect)
			{
				__ClearArmorRefineEffect();
			}

			__AttachArmorShiningEffect(shiningPath.value().c_str());
		}
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		else if (pItem->GetSubType() == CItemData::COSTUME_WEAPON)
		{
			__ClearWeaponShiningEffect();
			if (removeRefineEffect)
			{
				__ClearWeaponRefineEffect();
			}

			if (pItem->GetSubType() == CItemData::WEAPON_BOW)
			{
				__AttachWeaponShiningEffect(shiningPath.value().c_str(), "PART_WEAPON_LEFT");
			}
			else
			{
				bool twoSidedWeapon = pItem->GetValue(3) == CItemData::WEAPON_DAGGER || (IsMountingHorse() && pItem->GetValue(3) == CItemData::WEAPON_FAN);
				if (twoSidedWeapon)
				{
					__AttachWeaponShiningEffect(shiningPath.value().c_str(), "PART_WEAPON_LEFT");
				}
				if (pItem->GetValue(3) == CItemData::WEAPON_BOW)
					__AttachWeaponShiningEffect(shiningPath.value().c_str(), "PART_WEAPON_LEFT");
				else
					__AttachWeaponShiningEffect(shiningPath.value().c_str(), "PART_WEAPON");
			}
		}
#endif
	}
#endif
	else
	{
		if (removeRefineEffect)
		{
			__ClearArmorRefineEffect();
		}

		__AttachArmorShiningEffect(shiningPath.value().c_str());
	}
}

void CInstanceBase::__AttachWeaponShiningEffect(const char* effectFileName, const char* boneName)
{
	if (IsAffect(AFFECT_INVISIBILITY))
	{
		return;
	}


	CEffectManager::Instance().RegisterEffect(effectFileName, false, false);

	if (!strcmp(boneName, "PART_WEAPON"))
	{
		const char* c_szRightBoneName;
		m_GraphicThingInstance.GetAttachingBoneName(CRaceData::PART_WEAPON, &c_szRightBoneName);

		if (c_szRightBoneName == NULL)
			return;

		if (strcmp(c_szRightBoneName, ""))
		{
			m_weaponShiningEffects[0] = m_GraphicThingInstance.AttachEffectByName(0, c_szRightBoneName, effectFileName);
		}
	}
	else if (!strcmp(boneName, "PART_WEAPON_LEFT"))
	{
		const char* c_szLeftBoneName;
		m_GraphicThingInstance.GetAttachingBoneName(CRaceData::PART_WEAPON_LEFT, &c_szLeftBoneName);

		if (c_szLeftBoneName == NULL)
			return;

		if (strcmp(c_szLeftBoneName, ""))
		{
			m_weaponShiningEffects[1] = m_GraphicThingInstance.AttachEffectByName(0, c_szLeftBoneName, effectFileName);
		}
	}
	else
	{
		Tracef("Invalid partname for getting attaching bone name. %s - %s", effectFileName, boneName);
	}
}
void CInstanceBase::__AttachArmorShiningEffect(const char* effectFileName, const char* boneName)
{
	if (IsAffect(AFFECT_INVISIBILITY))
	{
		return;
	}

	if (!strcmp(boneName, ""))
	{
		Tracef("Empty bone name for attaching armor shining. Effect Index: %i, EffectFileName: %s", 0, effectFileName);
		return;
	}

	CEffectManager::Instance().RegisterEffect(effectFileName, false, false);
	m_armorShiningEffects = m_GraphicThingInstance.AttachEffectByName(0, boneName, effectFileName);
}

#ifdef ENABLE_SASH_SYSTEM
void CInstanceBase::__AttachAcceShiningEffect(const char* effectFileName, const char* boneName)
{
	if (IsAffect(AFFECT_INVISIBILITY))
	{
		return;
	}

	if (!strcmp(boneName, ""))
	{
		Tracef("Empty bone name for attaching acce shining. Effect Index: %i, EffectFileName: %s", 0, effectFileName);
		return;
	}

	CEffectManager::Instance().RegisterEffect(effectFileName, false, false);
	m_AcceShiningEffects = m_GraphicThingInstance.AttachEffectByName(0, boneName, effectFileName); // maybe related with parent index?
}
#endif

void CInstanceBase::__ClearWeaponShiningEffect(bool detaching)
{
	if (detaching)
	{
		if (m_weaponShiningEffects[0])
		{
			__DetachEffect(m_weaponShiningEffects[0]);
		}
		if (m_weaponShiningEffects[1])
		{
			__DetachEffect(m_weaponShiningEffects[1]);
		}
	}
	memset(&m_weaponShiningEffects, 0, sizeof(m_weaponShiningEffects));
}

void CInstanceBase::__ClearArmorShiningEffect(bool detaching)
{
	if (detaching)
	{
		__DetachEffect(m_armorShiningEffects);
	}
	m_armorShiningEffects = 0;
}

#ifdef ENABLE_SASH_SYSTEM
void CInstanceBase::__ClearAcceShiningEffect(bool detaching)
{
	if (detaching)
	{
		__DetachEffect(m_AcceShiningEffects);
	}

	m_AcceShiningEffects = 0;
}
#endif
#endif