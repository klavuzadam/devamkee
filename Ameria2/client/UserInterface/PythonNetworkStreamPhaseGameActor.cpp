#include "StdAfx.h"
#include "PythonNetworkStream.h"
#include "NetworkActorManager.h"
#include "PythonBackground.h"

#include "PythonApplication.h"
#include "AbstractPlayer.h"
#include "../gamelib/ActorInstance.h"





void CPythonNetworkStream::__GlobalPositionToLocalPosition(long& rGlobalX, long& rGlobalY)
{
	CPythonBackground&rkBgMgr=CPythonBackground::Instance();
	rkBgMgr.GlobalPositionToLocalPosition(rGlobalX, rGlobalY);
}

void CPythonNetworkStream::__LocalPositionToGlobalPosition(long& rLocalX, long& rLocalY)
{
	CPythonBackground&rkBgMgr=CPythonBackground::Instance();
	rkBgMgr.LocalPositionToGlobalPosition(rLocalX, rLocalY);
}

bool CPythonNetworkStream::__CanActMainInstance(bool bNoAct/* = false*/)
{
	auto& charMgr = CPythonCharacterManager::Instance();
	auto* pInstMain = charMgr.GetMainInstancePtr();
	if (!pInstMain)
	{
		return false;
	}

//#common fix in most of packets
//#ifdef USE_CURRENCY_EXCHANGE
	if (m_strPhase != "Game")
	{
		return false;
	}
//#endif

	if (bNoAct)
	{
		return true;
	}

	return pInstMain->CanAct();
}

void CPythonNetworkStream::__ClearNetworkActorManager()
{
	m_rokNetActorMgr->Destroy();
}

//MARTY-SAMA VERSION
/*
void __SetWeaponPower(IAbstractPlayer& rkPlayer, DWORD dwWeaponID)
{
	DWORD minPower=0;
	DWORD maxPower=0;
	DWORD minMagicPower=0;
	DWORD maxMagicPower=0;
	DWORD addPower=0;

	CItemData* pkWeapon;
	if (CItemManager::Instance().GetItemDataPointer(dwWeaponID, &pkWeapon))
	{
		if (pkWeapon->GetType()==CItemData::ITEM_TYPE_WEAPON)
		{
			minPower=pkWeapon->GetValue(3);
			maxPower=pkWeapon->GetValue(4);
			minMagicPower=pkWeapon->GetValue(1);
			maxMagicPower=pkWeapon->GetValue(2);
			addPower=pkWeapon->GetValue(5);
		}
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		else if (pkWeapon->GetType()==CItemData::ITEM_TYPE_COSTUME && pkWeapon->GetSubType()==CItemData::COSTUME_WEAPON)//who put that? non know i think marty is base marty i non change costume skins
		{
			CItemData* pkRealWeapon;
			if (CItemManager::Instance().GetItemDataPointer(CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Equipment_Weapon)), &pkRealWeapon))
			{
				minPower=pkRealWeapon->GetValue(3);
				maxPower=pkRealWeapon->GetValue(4);
				minMagicPower=pkRealWeapon->GetValue(1);
				maxMagicPower=pkRealWeapon->GetValue(2);
				addPower=pkRealWeapon->GetValue(5);
			}
		}
#endif
	}

	rkPlayer.SetWeaponPower(minPower, maxPower, minMagicPower, maxMagicPower, addPower);
}
*/

void __SetWeaponPower(IAbstractPlayer& rkPlayer)
{
	DWORD minPower=0;
	DWORD maxPower=0;
	DWORD minMagicPower=0;
	DWORD maxMagicPower=0;
	DWORD addPower=0;

	CItemData* pkWeapon;
	if (CItemManager::Instance().GetItemDataPointer(CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Equipment_Weapon)), &pkWeapon))
	{
		if (pkWeapon->GetType() == CItemData::ITEM_TYPE_WEAPON)
		{
			minPower = pkWeapon->GetValue(3);
			maxPower = pkWeapon->GetValue(4);
			minMagicPower = pkWeapon->GetValue(1);
			maxMagicPower = pkWeapon->GetValue(2);
			addPower = pkWeapon->GetValue(5);
		}
	}
	rkPlayer.SetWeaponPower(minPower, maxPower, minMagicPower, maxMagicPower, addPower);
}

//테이블에서 이름이 "." 인 것들
//차후에 서버에서 보내주지 않게 되면 없어질 함수..(서버님께 꼭!!협박; )
bool IsInvisibleRace(WORD raceNum)
{
	switch(raceNum)
	{
	case 20025:
	case 20038:
	case 20039:
		return true;
	default:
		return false;
	}
}

static SNetworkActorData s_kNetActorData;


bool CPythonNetworkStream::RecvCharacterAppendPacket()
{
	TPacketGCCharacterAdd chrAddPacket;
	if (!Recv(sizeof(chrAddPacket), &chrAddPacket))
		return false;

	__GlobalPositionToLocalPosition(chrAddPacket.x, chrAddPacket.y);

	SNetworkActorData kNetActorData;
	kNetActorData.m_bType=chrAddPacket.bType;
	kNetActorData.m_dwMovSpd=chrAddPacket.bMovingSpeed;
	kNetActorData.m_dwAtkSpd=chrAddPacket.bAttackSpeed;
	kNetActorData.m_dwRace=chrAddPacket.wRaceNum;

	kNetActorData.m_dwStateFlags=chrAddPacket.bStateFlag;
	kNetActorData.m_dwVID=chrAddPacket.dwVID;
	kNetActorData.m_fRot=chrAddPacket.angle;

	kNetActorData.m_stName="";

	kNetActorData.m_stName="";
	kNetActorData.m_kAffectFlags.CopyData(0, sizeof(chrAddPacket.dwAffectFlag[0]), &chrAddPacket.dwAffectFlag[0]);
	kNetActorData.m_kAffectFlags.CopyData(32, sizeof(chrAddPacket.dwAffectFlag[1]), &chrAddPacket.dwAffectFlag[1]);
	kNetActorData.m_kAffectFlags.CopyData(64, sizeof(chrAddPacket.dwAffectFlag[2]), &chrAddPacket.dwAffectFlag[2]);

	kNetActorData.SetPosition(chrAddPacket.x, chrAddPacket.y);

	kNetActorData.m_sAlignment=0;/*chrAddPacket.sAlignment*/;
	kNetActorData.m_byPKMode=0;/*chrAddPacket.bPKMode*/;
	kNetActorData.m_dwGuildID=0;/*chrAddPacket.dwGuild*/;
	kNetActorData.m_dwEmpireID=0;/*chrAddPacket.bEmpire*/;
	kNetActorData.m_dwArmor=0;/*chrAddPacket.awPart[CHR_EQUIPPART_ARMOR]*/;
	kNetActorData.m_dwWeapon=0;/*chrAddPacket.awPart[CHR_EQUIPPART_WEAPON]*/;
	kNetActorData.m_dwHair=0;/*chrAddPacket.awPart[CHR_EQUIPPART_HAIR]*/;
	#ifdef ENABLE_SASH_SYSTEM
	kNetActorData.m_dwSash = 0;
	#endif
	kNetActorData.m_dwArrowType = 0;

	kNetActorData.m_dwMountVnum=0;/*chrAddPacket.dwMountVnum*/;
#ifdef ENABLE_YOHARA_SYSTEM
	kNetActorData.m_dwConquerorLevel = 0;
#endif

	kNetActorData.m_dwLevel = 0; // 몬스터 레벨 표시 안함
#ifdef ENABLE_AURA_SYSTEM
	kNetActorData.m_dwAura=0;/* chrAddPacket.awPart[CHR_EQUIPPART_AURA]*/
#endif
	if(kNetActorData.m_bType != CActorInstance::TYPE_PC 
#ifdef ENABLE_NEW_PET_SYSTEM
		&& kNetActorData.m_bType != CActorInstance::TYPE_PET
#endif
		&& kNetActorData.m_bType != CActorInstance::TYPE_NPC
		&& kNetActorData.m_dwRace != 65000
		)
	{
		const char * c_szName;
		CPythonNonPlayer& rkNonPlayer=CPythonNonPlayer::Instance();
		if (rkNonPlayer.GetName(kNetActorData.m_dwRace, &c_szName))
			kNetActorData.m_stName = c_szName;
		//else
		//	kNetActorData.m_stName=chrAddPacket.name;

		__RecvCharacterAppendPacket(&kNetActorData);
	}
	else
	{
		s_kNetActorData = kNetActorData;
	}

	return true;
}

bool CPythonNetworkStream::RecvCharacterAdditionalInfo()
{
	TPacketGCCharacterAdditionalInfo chrInfoPacket;
	if (!Recv(sizeof(chrInfoPacket), &chrInfoPacket))
		return false;


	SNetworkActorData kNetActorData = s_kNetActorData;
	if (IsInvisibleRace(kNetActorData.m_dwRace))
		return true;

	if(kNetActorData.m_dwVID == chrInfoPacket.dwVID)
	{
		if (kNetActorData.m_bType == CActorInstance::TYPE_NPC && isAllowedNpc(kNetActorData.m_dwRace))
		{
			const char * c_szName;
			CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
			if (rkNonPlayer.GetName(kNetActorData.m_dwRace, &c_szName))
				kNetActorData.m_stName = c_szName;
		}
		else
			kNetActorData.m_stName = chrInfoPacket.name;
		if (10 == kNetActorData.m_dwRace || 11 == kNetActorData.m_dwRace)
		{
			kNetActorData.m_stName = chrInfoPacket.name;
		}

		kNetActorData.m_dwGuildID = chrInfoPacket.dwGuildID;
#if defined(WJ_COMBAT_ZONE)
		kNetActorData.combat_zone_rank = chrInfoPacket.combat_zone_rank;
		kNetActorData.combat_zone_points = chrInfoPacket.combat_zone_points;
#endif
		kNetActorData.m_dwLevel = chrInfoPacket.dwLevel;
#ifdef ENABLE_YOHARA_SYSTEM
		kNetActorData.m_dwConquerorLevel = chrInfoPacket.dwConquerorLevel;
#endif

		kNetActorData.m_sAlignment=chrInfoPacket.sAlignment;
		kNetActorData.m_byPKMode=chrInfoPacket.bPKMode;
		kNetActorData.m_dwGuildID=chrInfoPacket.dwGuildID;
		kNetActorData.m_dwEmpireID=chrInfoPacket.bEmpire;
		kNetActorData.m_dwArmor=chrInfoPacket.awPart[CHR_EQUIPPART_ARMOR];
		kNetActorData.m_dwWeapon=chrInfoPacket.awPart[CHR_EQUIPPART_WEAPON];
		kNetActorData.m_dwHair=chrInfoPacket.awPart[CHR_EQUIPPART_HAIR];
		kNetActorData.m_dwArrowType = chrInfoPacket.awPart[CHR_PART_ARROW_TYPE];

#ifdef ENABLE_SASH_SYSTEM
		kNetActorData.m_dwSash = chrInfoPacket.awPart[CHR_EQUIPPART_SASH];
#endif
		kNetActorData.m_dwMountVnum=chrInfoPacket.dwMountVnum;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		kNetActorData.m_stLanguage=chrInfoPacket.language;
#endif
#ifdef ENABLE_AURA_SYSTEM
		kNetActorData.m_dwAura=chrInfoPacket.awPart[CHR_EQUIPPART_AURA];
#endif
#ifdef ENABLE_SHINING_SYSTEM
		for (int i = 0; i < CHR_SHINING_NUM; i++) {
			kNetActorData.m_dwShining[i] = chrInfoPacket.adwShining[i];
		}
#endif
		__RecvCharacterAppendPacket(&kNetActorData);
	}
	else
	{
		TraceError("TPacketGCCharacterAdditionalInfo name=%s vid=%d race=%d Error",chrInfoPacket.name,chrInfoPacket.dwVID,kNetActorData.m_dwRace);
	}
	return true;
}

bool CPythonNetworkStream::RecvCharacterAppendPacketNew()
{
	TraceError("TPacketGCCharacterAdd2 is packet that doesn't write.");
	TPacketGCCharacterAdd2 chrAddPacket;
	if (!Recv(sizeof(chrAddPacket), &chrAddPacket))
		return false;
	if(IsInvisibleRace(chrAddPacket.wRaceNum))
		return true;

	__GlobalPositionToLocalPosition(chrAddPacket.x, chrAddPacket.y);

	SNetworkActorData kNetActorData;
	kNetActorData.m_dwLevel = 0;
#ifdef ENABLE_YOHARA_SYSTEM
	kNetActorData.m_dwConquerorLevel = 0;
#endif

	kNetActorData.m_bType=chrAddPacket.bType;
	kNetActorData.m_dwGuildID=chrAddPacket.dwGuild;
	kNetActorData.m_dwEmpireID=chrAddPacket.bEmpire;
	kNetActorData.m_dwMovSpd=chrAddPacket.bMovingSpeed;
	kNetActorData.m_dwAtkSpd=chrAddPacket.bAttackSpeed;
	kNetActorData.m_dwRace=chrAddPacket.wRaceNum;
	kNetActorData.m_dwArmor=chrAddPacket.awPart[CHR_EQUIPPART_ARMOR];
	kNetActorData.m_dwWeapon=chrAddPacket.awPart[CHR_EQUIPPART_WEAPON];
	kNetActorData.m_dwHair=chrAddPacket.awPart[CHR_EQUIPPART_HAIR];
#ifdef ENABLE_SASH_SYSTEM
	kNetActorData.m_dwSash = chrAddPacket.awPart[CHR_EQUIPPART_SASH];
#endif
	kNetActorData.m_dwArrowType = chrAddPacket.awPart[CHR_PART_ARROW_TYPE];

	kNetActorData.m_dwStateFlags=chrAddPacket.bStateFlag;
	kNetActorData.m_dwVID=chrAddPacket.dwVID;
	kNetActorData.m_dwMountVnum=chrAddPacket.dwMountVnum;
	kNetActorData.m_fRot=chrAddPacket.angle;
	kNetActorData.m_kAffectFlags.CopyData(0, sizeof(chrAddPacket.dwAffectFlag[0]), &chrAddPacket.dwAffectFlag[0]);
	kNetActorData.m_kAffectFlags.CopyData(32, sizeof(chrAddPacket.dwAffectFlag[1]), &chrAddPacket.dwAffectFlag[1]);
	kNetActorData.m_kAffectFlags.CopyData(64, sizeof(chrAddPacket.dwAffectFlag[2]), &chrAddPacket.dwAffectFlag[2]);

	kNetActorData.SetPosition(chrAddPacket.x, chrAddPacket.y);
	kNetActorData.m_sAlignment=chrAddPacket.sAlignment;
	kNetActorData.m_byPKMode=chrAddPacket.bPKMode;
	if (kNetActorData.m_bType == CActorInstance::TYPE_NPC  && isAllowedNpc(kNetActorData.m_dwRace)) {
		const char * c_szName;
		CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
		if (rkNonPlayer.GetName(kNetActorData.m_dwRace, &c_szName))
			kNetActorData.m_stName = c_szName;
	}
	else
		kNetActorData.m_stName = chrAddPacket.name;
	__RecvCharacterAppendPacket(&kNetActorData);

	return true;
}

//in mynpc arry insert just the value you want to be loaded from mob_proto server like pet or horse
bool CPythonNetworkStream::isAllowedNpc(DWORD race){
	if (race >= 30000 && race <= 30020)
		return false;
	
	if(race == 65000)
		return false;
	
	switch (race)
	{
	case 20109:
	case 20110:
	case 20111:
	case 20112:
	case 20113:
	case 20244:
	case 20245:
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
	case 32026:
	case 32027:
	case 32028:
	case 32029:
	case 32030:
	case 32031:
	case 32032:
	case 65002:
	case 65003:
	case 65004:
	case 65005:
	case 32033:
	case 32034:
	case 32035:
	case 32036:
	case 20142:
	case 20143:
		return false;
	}

	int mynpc[] = { 20101, 20102, 20103, 20104, 20105, 20106, 20107, 20108, 20109, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 34041, 34042, 34043, 34044, 34045, 34046, 34047, 34048, 34049, 34050, 34051, 34052, 34053, 34054, 34001, 34003, 34009, 34010, 34011, 34012, 34008, 34007, 34005, 34006, 34016, 34023, 34024, 34025, 34026, 34027, 34028, 34021, 34022, 34015 };
	// int mynpc[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for (int i = 0; i < (sizeof(mynpc) / sizeof(mynpc[0])); ++i){
		if (mynpc[i] == race)
			return false;
	}
	return true;
}

bool CPythonNetworkStream::RecvCharacterUpdatePacket()
{
	TPacketGCCharacterUpdate chrUpdatePacket;
	if (!Recv(sizeof(chrUpdatePacket), &chrUpdatePacket))
		return false;

	SNetworkUpdateActorData kNetUpdateActorData;
	kNetUpdateActorData.m_dwGuildID=chrUpdatePacket.dwGuildID;
#if defined(WJ_COMBAT_ZONE)
	kNetUpdateActorData.combat_zone_rank = chrUpdatePacket.combat_zone_rank;
	kNetUpdateActorData.combat_zone_points = chrUpdatePacket.combat_zone_points;
#endif
	kNetUpdateActorData.m_dwMovSpd=chrUpdatePacket.bMovingSpeed;
	kNetUpdateActorData.m_dwAtkSpd=chrUpdatePacket.bAttackSpeed;
	kNetUpdateActorData.m_dwArmor=chrUpdatePacket.awPart[CHR_EQUIPPART_ARMOR];
	kNetUpdateActorData.m_dwWeapon=chrUpdatePacket.awPart[CHR_EQUIPPART_WEAPON];
	kNetUpdateActorData.m_dwHair=chrUpdatePacket.awPart[CHR_EQUIPPART_HAIR];
	#ifdef ENABLE_SASH_SYSTEM
	kNetUpdateActorData.m_dwSash = chrUpdatePacket.awPart[CHR_EQUIPPART_SASH];
	#endif
	kNetUpdateActorData.m_dwArrowType = chrUpdatePacket.awPart[CHR_PART_ARROW_TYPE];

	kNetUpdateActorData.m_dwVID=chrUpdatePacket.dwVID;
	kNetUpdateActorData.m_kAffectFlags.CopyData(0, sizeof(chrUpdatePacket.dwAffectFlag[0]), &chrUpdatePacket.dwAffectFlag[0]);
	kNetUpdateActorData.m_kAffectFlags.CopyData(32, sizeof(chrUpdatePacket.dwAffectFlag[1]), &chrUpdatePacket.dwAffectFlag[1]);
	kNetUpdateActorData.m_kAffectFlags.CopyData(64, sizeof(chrUpdatePacket.dwAffectFlag[2]), &chrUpdatePacket.dwAffectFlag[2]);
	kNetUpdateActorData.m_sAlignment=chrUpdatePacket.sAlignment;
#ifdef NEW_PET_SYSTEM
	kNetUpdateActorData.m_dwLevel=chrUpdatePacket.dwLevel;
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	kNetUpdateActorData.m_dwConquerorLevel=chrUpdatePacket.dwConquerorLevel;
#endif
#ifdef ENABLE_SHINING_SYSTEM
	for (int i = 0; i < CHR_SHINING_NUM; i++) {
		kNetUpdateActorData.m_dwShining[i] = chrUpdatePacket.adwShining[i];
	}
#endif
	kNetUpdateActorData.m_byPKMode=chrUpdatePacket.bPKMode;
	kNetUpdateActorData.m_dwStateFlags=chrUpdatePacket.bStateFlag;
	kNetUpdateActorData.m_dwMountVnum=chrUpdatePacket.dwMountVnum;
#ifdef ENABLE_AURA_SYSTEM
	kNetUpdateActorData.m_dwAura=chrUpdatePacket.awPart[CHR_EQUIPPART_AURA];
#endif

	__RecvCharacterUpdatePacket(&kNetUpdateActorData);

	return true;
}

bool CPythonNetworkStream::RecvCharacterUpdatePacketNew()
{
	TPacketGCCharacterUpdate2 chrUpdatePacket;
	if (!Recv(sizeof(chrUpdatePacket), &chrUpdatePacket))
		return false;

	SNetworkUpdateActorData kNetUpdateActorData;
	kNetUpdateActorData.m_dwGuildID=chrUpdatePacket.dwGuildID;
	kNetUpdateActorData.m_dwMovSpd=chrUpdatePacket.bMovingSpeed;
	kNetUpdateActorData.m_dwAtkSpd=chrUpdatePacket.bAttackSpeed;
	kNetUpdateActorData.m_dwArmor=chrUpdatePacket.awPart[CHR_EQUIPPART_ARMOR];
	kNetUpdateActorData.m_dwWeapon=chrUpdatePacket.awPart[CHR_EQUIPPART_WEAPON];
	kNetUpdateActorData.m_dwHair=chrUpdatePacket.awPart[CHR_EQUIPPART_HAIR];
#ifdef ENABLE_SASH_SYSTEM
	kNetUpdateActorData.m_dwSash = chrUpdatePacket.awPart[CHR_EQUIPPART_SASH];
#endif
	kNetUpdateActorData.m_dwArrowType = chrUpdatePacket.awPart[CHR_PART_ARROW_TYPE];

#ifdef ENABLE_SHINING_SYSTEM
	for (int i = 0; i < CHR_SHINING_NUM; i++) {
		kNetUpdateActorData.m_dwShining[i] = chrUpdatePacket.adwShining[i];
	}
#endif
	
	kNetUpdateActorData.m_dwVID=chrUpdatePacket.dwVID;
	kNetUpdateActorData.m_kAffectFlags.CopyData(0, sizeof(chrUpdatePacket.dwAffectFlag[0]), &chrUpdatePacket.dwAffectFlag[0]);
	kNetUpdateActorData.m_kAffectFlags.CopyData(32, sizeof(chrUpdatePacket.dwAffectFlag[1]), &chrUpdatePacket.dwAffectFlag[1]);
	kNetUpdateActorData.m_kAffectFlags.CopyData(64, sizeof(chrUpdatePacket.dwAffectFlag[2]), &chrUpdatePacket.dwAffectFlag[2]);
	kNetUpdateActorData.m_sAlignment=chrUpdatePacket.sAlignment;
	kNetUpdateActorData.m_byPKMode=chrUpdatePacket.bPKMode;
	kNetUpdateActorData.m_dwStateFlags=chrUpdatePacket.bStateFlag;
	kNetUpdateActorData.m_dwMountVnum=chrUpdatePacket.dwMountVnum;


	__RecvCharacterUpdatePacket(&kNetUpdateActorData);

	return true;
}

void CPythonNetworkStream::__RecvCharacterAppendPacket(SNetworkActorData * pkNetActorData)
{
	// NOTE : 카메라가 땅에 묻히는 문제의 해결을 위해 메인 캐릭터가 지형에 올려지기
	//        전에 맵을 업데이트 해 높이를 구할 수 있도록 해놓아야 합니다.
	//        단, 게임이 들어갈때가 아닌 이미 캐릭터가 추가 된 이후에만 합니다.
	//        헌데 이동인데 왜 Move로 안하고 Append로 하는지..? - [levites]
	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	if (rkPlayer.IsMainCharacterIndex(pkNetActorData->m_dwVID))
	{
		rkPlayer.SetRace(pkNetActorData->m_dwRace);

		//__SetWeaponPower(rkPlayer, pkNetActorData->m_dwWeapon);
		__SetWeaponPower(rkPlayer);

		if (rkPlayer.NEW_GetMainActorPtr())
		{
			CPythonBackground::Instance().Update(pkNetActorData->m_lCurX, pkNetActorData->m_lCurY, 0.0f);
			CPythonCharacterManager::Instance().Update();

			// NOTE : 사귀 타워일 경우 GOTO 로 이동시에도 맵 이름을 출력하도록 처리
			{
				std::string strMapName = CPythonBackground::Instance().GetWarpMapName();
				if (strMapName == "metin2_map_deviltower1")
					__ShowMapName(pkNetActorData->m_lCurX, pkNetActorData->m_lCurY);
			}
		}
		else
		{
			__ShowMapName(pkNetActorData->m_lCurX, pkNetActorData->m_lCurY);
		}
	}

	m_rokNetActorMgr->AppendActor(*pkNetActorData);

	if (GetMainActorVID()==pkNetActorData->m_dwVID)
	{
		rkPlayer.SetTarget(0);
		if (m_bComboSkillFlag)
			rkPlayer.SetComboSkillFlag(m_bComboSkillFlag);

		__SetGuildID(pkNetActorData->m_dwGuildID);
		//CPythonApplication::Instance().SkipRenderBuffering(10000);
	}
}

void CPythonNetworkStream::__RecvCharacterUpdatePacket(SNetworkUpdateActorData * pkNetUpdateActorData)
{
	m_rokNetActorMgr->UpdateActor(*pkNetUpdateActorData);

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	if (rkPlayer.IsMainCharacterIndex(pkNetUpdateActorData->m_dwVID))
	{
		__SetGuildID(pkNetUpdateActorData->m_dwGuildID);
		__SetWeaponPower(rkPlayer);
		//__SetWeaponPower(rkPlayer, pkNetUpdateActorData->m_dwWeapon);

		__RefreshStatus();
		__RefreshAlignmentWindow();
		__RefreshEquipmentWindow();
		__RefreshInventoryWindow();
	}
	else
	{
		rkPlayer.NotifyCharacterUpdate(pkNetUpdateActorData->m_dwVID);
	}
}

bool CPythonNetworkStream::RecvCharacterDeletePacket()
{
	TPacketGCCharacterDelete chrDelPacket;

	if (!Recv(sizeof(chrDelPacket), &chrDelPacket))
	{
		TraceError("CPythonNetworkStream::RecvCharacterDeletePacket - Recv Error");
		return false;
	}

	m_rokNetActorMgr->RemoveActor(chrDelPacket.dwVID);

	// 캐릭터가 사라질때 개인 상점도 없애줍니다.
	// Key Check 를 하기때문에 없어도 상관은 없습니다.
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
		"BINARY_PrivateShop_Disappear",
		Py_BuildValue("(i)", chrDelPacket.dwVID)
	);

	return true;
}

#ifdef ENABLE_SUPPORT_SYSTEM
bool CPythonNetworkStream::RecvSupportUseSkill()
{
	TPacketGCSupportUseSkill packet;
	//TraceError("Skill0");
	if (!Recv(sizeof(TPacketGCSupportUseSkill), &packet))
	{
		Tracen("CPythonNetworkStream::RecvCharacterMovePacket - PACKET READ ERROR");
		return false;
	}
	CInstanceBase* pkInstClone = CPythonCharacterManager::Instance().GetInstancePtr(packet.dwVid);
	DWORD dwSkillIndex = packet.dwVnum;
	CPythonSkill::TSkillData * pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(dwSkillIndex, &pSkillData))
		return false;
	//TraceError("Skill1");
	DWORD value;
	if(packet.dwLevel <=17)
		value = 0;
	else if(packet.dwLevel <=30 && packet.dwLevel >17)
		value = 1;
	else if(packet.dwLevel > 30 && packet.dwLevel <= 39)
		value = 2;
	else if(packet.dwLevel >= 40)
		value = 3;
	
	DWORD dwMotionIndex = pSkillData->GetSkillMotionIndex(value);
	if (!pkInstClone->NEW_UseSkill(dwSkillIndex, dwMotionIndex, 1, false))
	{
		Tracenf("CPythonPlayer::UseGuildSkill(%d) - pkInstMain->NEW_UseSkill - ERROR", dwSkillIndex);
	}
	
	return true;
}
#endif

bool CPythonNetworkStream::RecvCharacterMovePacket()
{
	TPacketGCMove kMovePacket;
	if (!Recv(sizeof(TPacketGCMove), &kMovePacket))
	{
		Tracen("CPythonNetworkStream::RecvCharacterMovePacket - PACKET READ ERROR");
		return false;
	}

	__GlobalPositionToLocalPosition(kMovePacket.lX, kMovePacket.lY);

	SNetworkMoveActorData kNetMoveActorData;
	kNetMoveActorData.m_dwArg=kMovePacket.bArg;
	kNetMoveActorData.m_dwFunc=kMovePacket.bFunc;
	kNetMoveActorData.m_dwTime=kMovePacket.dwTime;
	kNetMoveActorData.m_dwVID=kMovePacket.dwVID;
	kNetMoveActorData.m_fRot=kMovePacket.bRot*5.0f;
	kNetMoveActorData.m_lPosX=kMovePacket.lX;
	kNetMoveActorData.m_lPosY=kMovePacket.lY;
	kNetMoveActorData.m_dwDuration=kMovePacket.dwDuration;

	m_rokNetActorMgr->MoveActor(kNetMoveActorData);

	return true;
}

bool CPythonNetworkStream::RecvOwnerShipPacket()
{
	TPacketGCOwnership kPacketOwnership;

	if (!Recv(sizeof(kPacketOwnership), &kPacketOwnership))
		return false;

	m_rokNetActorMgr->SetActorOwner(kPacketOwnership.dwOwnerVID, kPacketOwnership.dwVictimVID);

	return true;
}

bool CPythonNetworkStream::RecvSyncPositionPacket()
{
	TPacketGCSyncPosition kPacketSyncPos;
	if (!Recv(sizeof(kPacketSyncPos), &kPacketSyncPos))
		return false;

	TPacketGCSyncPositionElement kSyncPos;

	UINT uSyncPosCount=(kPacketSyncPos.wSize-sizeof(kPacketSyncPos))/sizeof(kSyncPos);
	for (UINT iSyncPos=0; iSyncPos<uSyncPosCount; ++iSyncPos)
	{
		if (!Recv(sizeof(TPacketGCSyncPositionElement), &kSyncPos))
			return false;

#ifdef __MOVIE_MODE__
		return true;
#endif __MOVIE_MODE__

		//Tracenf("CPythonNetworkStream::RecvSyncPositionPacket %d (%d, %d)", kSyncPos.dwVID, kSyncPos.lX, kSyncPos.lY);

		__GlobalPositionToLocalPosition(kSyncPos.lX, kSyncPos.lY);
		m_rokNetActorMgr->SyncActor(kSyncPos.dwVID, kSyncPos.lX, kSyncPos.lY);

		/*
		CPythonCharacterManager & rkChrMgr = CPythonCharacterManager::Instance();
		CInstanceBase * pkChrInst = rkChrMgr.GetInstancePtr(kSyncPos.dwVID);

		if (pkChrInst)
		{
			pkChrInst->NEW_SyncPixelPosition(kSyncPos.lX, kSyncPos.lY);
		}
		*/
}

	return true;
}

/*
bool CPythonNetworkStream::RecvCharacterAppendPacket()
{
	TPacketGCCharacterAdd chrAddPacket;

	if (!Recv(sizeof(chrAddPacket), &chrAddPacket))
		return false;

	__GlobalPositionToLocalPosition(chrAddPacket.lX, chrAddPacket.lY);

	SNetworkActorData kNetActorData;
	kNetActorData.m_dwGuildID=chrAddPacket.dwGuild;
	kNetActorData.m_dwEmpireID=chrAddPacket.bEmpire;
	kNetActorData.m_dwMovSpd=chrAddPacket.bMovingSpeed;
	kNetActorData.m_dwAtkSpd=chrAddPacket.bAttackSpeed;
	kNetActorData.m_dwRace=chrAddPacket.wRaceNum;
	kNetActorData.m_dwShape=chrAddPacket.parts[CRaceData::PART_MAIN];
	kNetActorData.m_dwStateFlags=chrAddPacket.bStateFlag;
	kNetActorData.m_dwVID=chrAddPacket.dwVID;
	kNetActorData.m_dwWeapon=chrAddPacket.parts[CRaceData::PART_WEAPON];
	kNetActorData.m_fRot=chrAddPacket.angle;
	kNetActorData.m_kAffectFlags.CopyData(0, sizeof(chrAddPacket.dwAffectFlag[0]), &chrAddPacket.dwAffectFlag[0]);
	kNetActorData.m_kAffectFlags.CopyData(32, sizeof(chrAddPacket.dwAffectFlag[1]), &chrAddPacket.dwAffectFlag[1]);
	kNetActorData.m_kAffectFlags.CopyData(64, sizeof(chrAddPacket.dwAffectFlag[2]), &chrAddPacket.dwAffectFlag[2]);
	kNetActorData.SetPosition(chrAddPacket.lX, chrAddPacket.lY);
	kNetActorData.m_stName=chrAddPacket.name;
	__RecvCharacterAppendPacket(&kNetActorData);
	return true;
}
*/

