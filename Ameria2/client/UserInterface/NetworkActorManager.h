#pragma once

#include "InstanceBase.h"
#include "Packet.h"

struct SNetworkActorData
{
	std::string m_stName;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	BYTE m_stLanguage;
#endif
	CAffectFlagContainer	m_kAffectFlags;

	BYTE	m_bType;
	DWORD	m_dwVID;
	DWORD	m_dwStateFlags;
	DWORD	m_dwEmpireID;
	DWORD	m_dwRace;
	DWORD	m_dwMovSpd;
	DWORD	m_dwAtkSpd;
	FLOAT	m_fRot;
	long	m_lCurX;
	long	m_lCurY;
	long	m_lSrcX;
	long	m_lSrcY;
	long	m_lDstX;
	long	m_lDstY;


	DWORD	m_dwServerSrcTime;
	DWORD	m_dwClientSrcTime;
	DWORD	m_dwDuration;

	DWORD	m_dwArmor;
	DWORD	m_dwWeapon;
	DWORD	m_dwHair;
	#ifdef ENABLE_SASH_SYSTEM
	DWORD	m_dwSash;
	#endif
	DWORD	m_dwArrowType;

	DWORD	m_dwOwnerVID;

	short	m_sAlignment;
	BYTE	m_byPKMode;
	DWORD	m_dwMountVnum;
#if defined(WJ_COMBAT_ZONE)
	BYTE combat_zone_rank;
	DWORD combat_zone_points;
#endif
#ifdef ENABLE_SHINING_SYSTEM
	DWORD	m_dwShining[CHR_SHINING_NUM];
#endif
	DWORD	m_dwGuildID;
	DWORD	m_dwLevel;
#ifdef ENABLE_YOHARA_SYSTEM
	DWORD	m_dwConquerorLevel;
#endif
#ifdef ENABLE_AURA_SYSTEM
	DWORD	m_dwAura;
#endif


	

	SNetworkActorData();

	void SetDstPosition(DWORD dwServerTime, long lDstX, long lDstY, DWORD dwDuration);
	void SetPosition(long lPosX, long lPosY);
	void UpdatePosition();

	// NETWORK_ACTOR_DATA_COPY
	SNetworkActorData(const SNetworkActorData& src);
	void operator=(const SNetworkActorData& src);
	void __copy__(const SNetworkActorData& src);
	// END_OF_NETWORK_ACTOR_DATA_COPY
};

struct SNetworkMoveActorData
{
	DWORD	m_dwVID;
	DWORD	m_dwTime;
	long	m_lPosX;
	long	m_lPosY;
	float	m_fRot;
	DWORD	m_dwFunc;
	DWORD	m_dwArg;
	DWORD	m_dwDuration;

	SNetworkMoveActorData()
	{
		m_dwVID=0;
		m_dwTime=0;
		m_fRot=0.0f;
		m_lPosX=0;
		m_lPosY=0;
		m_dwFunc=0;
		m_dwArg=0;
		m_dwDuration=0;
	}
};

struct SNetworkUpdateActorData
{
	DWORD m_dwVID;
	DWORD m_dwGuildID;
	DWORD m_dwArmor;
	DWORD m_dwWeapon;
	DWORD m_dwHair;
	#ifdef ENABLE_SASH_SYSTEM
	DWORD	m_dwSash;
	#endif
	DWORD m_dwArrowType;

	DWORD m_dwMovSpd;
	DWORD m_dwAtkSpd;
	short m_sAlignment;
#ifdef NEW_PET_SYSTEM
	DWORD	m_dwLevel;
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	DWORD	m_dwConquerorLevel;
#endif
	BYTE m_byPKMode;
	DWORD m_dwMountVnum;
#if defined(WJ_COMBAT_ZONE)
	BYTE combat_zone_rank;
	DWORD combat_zone_points;
#endif
#ifdef ENABLE_SHINING_SYSTEM
	DWORD m_dwShining[CHR_SHINING_NUM];
#endif
#ifdef ENABLE_AURA_SYSTEM
	DWORD m_dwAura;
#endif
	DWORD m_dwStateFlags; // 본래 Create 때만 쓰이는 변수임
	CAffectFlagContainer m_kAffectFlags;

	SNetworkUpdateActorData()
	{
		m_dwGuildID=0;
		m_dwVID=0;
		m_dwArmor=0;
		m_dwWeapon=0;
		m_dwHair=0;
		#ifdef ENABLE_SASH_SYSTEM
		m_dwSash = 0;
		#endif
		m_dwArrowType=0;

		m_dwMovSpd=0;
		m_dwAtkSpd=0;
		m_sAlignment=0;
		m_byPKMode=0;
		m_dwMountVnum=0;
#if defined(WJ_COMBAT_ZONE)
		combat_zone_rank = 0;
		combat_zone_points = 0;
#endif
#ifdef ENABLE_AURA_SYSTEM
		m_dwAura=0;
#endif
		m_dwStateFlags=0;
		m_kAffectFlags.Clear();
	}
};

class CPythonCharacterManager;

class CNetworkActorManager : public CReferenceObject
{
	public:
		CNetworkActorManager();
		virtual ~CNetworkActorManager();

		void Destroy();

		void SetMainActorVID(DWORD dwVID);

		void RemoveActor(DWORD dwVID);
		void AppendActor(const SNetworkActorData& c_rkNetActorData);
		void UpdateActor(const SNetworkUpdateActorData& c_rkNetUpdateActorData);
		void MoveActor(const SNetworkMoveActorData& c_rkNetMoveActorData);

		void SyncActor(DWORD dwVID, long lPosX, long lPosY);
		void SetActorOwner(DWORD dwOwnerVID, DWORD dwVictimVID);

		void Update();

	protected:
		void __OLD_Update();

		void __UpdateMainActor();

		bool __IsVisiblePos(long lPosX, long lPosY);
		bool __IsVisibleActor(const SNetworkActorData& c_rkNetActorData);
		bool __IsMainActorVID(DWORD dwVID);

		void __RemoveAllGroundItems();
		void __RemoveAllActors();
		void __RemoveDynamicActors();
		void __RemoveCharacterManagerActor(SNetworkActorData& rkNetActorData);

		SNetworkActorData* __FindActorData(DWORD dwVID);

		CInstanceBase* __AppendCharacterManagerActor(SNetworkActorData& rkNetActorData);
		CInstanceBase* __FindActor(SNetworkActorData& rkNetActorData);
		CInstanceBase* __FindActor(SNetworkActorData& rkNetActorData, long lDstX, long lDstY);

		CPythonCharacterManager& __GetCharacterManager();

	protected:
		DWORD m_dwMainVID;

		long m_lMainPosX;
		long m_lMainPosY;

		std::map<DWORD, SNetworkActorData> m_kNetActorDict;
};
