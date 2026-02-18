#pragma once

#include "AbstractPlayer.h"
#include "Packet.h"
#include "PythonSkill.h"

class CInstanceBase;

/*
 *	메인 캐릭터 (자신이 조정하는 캐릭터) 가 가진 정보들을 관리한다.
 *
 * 2003-01-12 Levites	본래는 CPythonCharacter가 가지고 있었지만 규모가 너무 커져 버린데다
 *						위치도 애매해서 따로 분리
 * 2003-07-19 Levites	메인 캐릭터의 이동 처리 CharacterInstance에서 떼어다 붙임
 *						기존의 데이타 보존의 역할에서 완벽한 메인 플레이어 제어 클래스로
 *						탈바꿈 함.
 */

class CPythonPlayer : public CSingleton<CPythonPlayer>, public IAbstractPlayer
{
	public:
       	 	void* ptrti;
        	void* ptrit;
		enum
		{
			CATEGORY_NONE		= 0,
			CATEGORY_ACTIVE		= 1,
			CATEGORY_PASSIVE	= 2,
			CATEGORY_MAX_NUM	= 3,

			STATUS_INDEX_ST = 1,
			STATUS_INDEX_DX = 2,
			STATUS_INDEX_IQ = 3,
			STATUS_INDEX_HT = 4,
		};

		enum
		{
			MBT_LEFT,
			MBT_RIGHT,
			MBT_MIDDLE,
			MBT_NUM,
		};

		enum
		{
			MBF_SMART,
			MBF_MOVE,
			MBF_CAMERA,
			MBF_ATTACK,
			MBF_SKILL,
			MBF_AUTO,
#ifdef __AUTO_QUQUE_ATTACK__
			MBF_QUEUE_ON,
#endif
		};

		enum
		{
			MBS_CLICK,
			MBS_PRESS,
		};

		enum EMode
		{
			MODE_NONE,
			MODE_CLICK_POSITION,
			MODE_CLICK_ITEM,
			MODE_CLICK_ACTOR,
			MODE_USE_SKILL,
		};

		enum EEffect
		{
			EFFECT_PICK,
			EFFECT_NUM,
		};

		enum EMetinSocketType
		{
			METIN_SOCKET_TYPE_NONE,
			METIN_SOCKET_TYPE_SILVER,
			METIN_SOCKET_TYPE_GOLD,
		};

		enum
		{
			PHASE_WINDOW_LOGO,
			PHASE_WINDOW_LOGIN,
			PHASE_WINDOW_SELECT,
			PHASE_WINDOW_CREATE,
			PHASE_WINDOW_LOAD,
			PHASE_WINDOW_GAME,
			PHASE_WINDOW_EMPIRE,
			PHASE_WINDOW_NUM,
		};

		typedef struct SSkillInstance
		{
			DWORD dwIndex;
			int iType;
			int iGrade;
			int iLevel;
			float fcurEfficientPercentage;
			float fnextEfficientPercentage;
			BOOL isCoolTime;

			float fCoolTime;			// NOTE : 쿨타임 중인 스킬 슬롯을
			float fLastUsedTime;		//        퀵창에 등록할 때 사용하는 변수
			BOOL bActive;
		} TSkillInstance;

		enum EKeyBoard_UD
		{
			KEYBOARD_UD_NONE,
			KEYBOARD_UD_UP,
			KEYBOARD_UD_DOWN,
		};

		enum EKeyBoard_LR
		{
			KEYBOARD_LR_NONE,
			KEYBOARD_LR_LEFT,
			KEYBOARD_LR_RIGHT,
		};

		enum
		{
			DIR_UP,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
		};

		typedef struct SPlayerStatus
		{
			TItemData			aItem[c_Inventory_Count];
			TItemData			aDSItem[c_DragonSoul_Inventory_Count];
#ifdef ENABLE_SWITCHBOT
			TItemData			aSwitchbotItem[SWITCHBOT_SLOT_COUNT];
#endif
#ifdef ENABLE_SPECIAL_STORAGE
			TItemData			aSSUItem[c_Special_ItemSlot_Count];
			TItemData			aSSBItem[c_Special_ItemSlot_Count];
			TItemData			aSSSItem[c_Special_ItemSlot_Count];
			TItemData			aSSCItem[c_Special_ItemSlot_Count];
			TItemData			aCostumeItem[c_Special_ItemSlot_Count];
			TItemData			aChestItem[c_Special_ItemSlot_Count];
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
			TItemData			aSBItem[SWITCHBOT_COUNT];
#endif
#ifdef CHANGE_EQUIP_WORLDARD
			TItemData 			aChangeEquipItem[CHANGE_EQUIP_SLOT_COUNT];
#endif
			TQuickSlot			aQuickSlot[QUICKSLOT_MAX_NUM];
			TSkillInstance		aSkill[SKILL_MAX_NUM];
			long long			m_alPoint[POINT_MAX_NUM];

			long				lQuickPageIndex;

			void SetPoint(UINT ePoint, long long lPoint);

			long long GetPoint(UINT ePoint);

		} TPlayerStatus;

		typedef struct SPartyMemberInfo
		{
			SPartyMemberInfo(DWORD _dwPID, const char * c_szName) : dwPID(_dwPID), strName(c_szName), dwVID(0) {}

			DWORD dwVID;
			DWORD dwPID;
			std::string strName;
			BYTE byState;
			BYTE byHPPercentage;
			short sAffects[PARTY_AFFECT_SLOT_MAX_NUM];
		} TPartyMemberInfo;

		enum EPartyRole
		{
			PARTY_ROLE_NORMAL,
			PARTY_ROLE_LEADER,
			PARTY_ROLE_ATTACKER,
			PARTY_ROLE_TANKER,
			PARTY_ROLE_BUFFER,
			PARTY_ROLE_SKILL_MASTER,
			PARTY_ROLE_BERSERKER,
			PARTY_ROLE_DEFENDER,
			PARTY_ROLE_MAX_NUM,
		};

		enum
		{
			SKILL_NORMAL,
			SKILL_MASTER,
			SKILL_GRAND_MASTER,
			SKILL_PERFECT_MASTER,
		};

		// 자동물약 상태 관련 특화 구조체.. 이런식의 특화 처리 작업을 안 하려고 최대한 노력했지만 실패하고 결국 특화처리.
		struct SAutoPotionInfo
		{
			SAutoPotionInfo() : bActivated(false), totalAmount(0), currentAmount(0) {}

			bool bActivated;					// 활성화 되었는가?
			long currentAmount;					// 현재 남은 양
			long totalAmount;					// 전체 양
			long inventorySlotIndex;			// 사용중인 아이템의 인벤토리상 슬롯 인덱스
		};

		enum EAutoPotionType
		{
			AUTO_POTION_TYPE_HP = 0,
			AUTO_POTION_TYPE_SP = 1,
			AUTO_POTION_TYPE_NUM
		};

	public:
		CPythonPlayer(void);
		virtual ~CPythonPlayer(void);

		void	PickCloseMoney();
		void	PickCloseItem();

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
		void 	PickCloseItemVector();
#endif
		void	SetGameWindow(PyObject * ppyObject);

		void	SetObserverMode(bool isEnable);
		bool	IsObserverMode();

		void	SetQuickCameraMode(bool isEnable);

		void	SetAttackKeyState(bool isPress);

		void	NEW_GetMainActorPosition(TPixelPosition* pkPPosActor);

		bool	RegisterEffect(DWORD dwEID, const char* c_szEftFileName, bool isCache);

		bool	NEW_SetMouseState(int eMBType, int eMBState);
		bool	NEW_SetMouseFunc(int eMBType, int eMBFunc);
		int		NEW_GetMouseFunc(int eMBT);
		void	NEW_SetMouseMiddleButtonState(int eMBState);

		void	NEW_SetAutoCameraRotationSpeed(float fRotSpd);
		void	NEW_ResetCameraRotation();

		void	NEW_SetSingleDirKeyState(int eDirKey, bool isPress);
		void	NEW_SetSingleDIKKeyState(int eDIKKey, bool isPress);
		void	NEW_SetMultiDirKeyState(bool isLeft, bool isRight, bool isUp, bool isDown);

		void	NEW_Attack();
		void	NEW_Fishing();
		bool	NEW_CancelFishing();

		void	NEW_LookAtFocusActor();
		bool	NEW_IsAttackableDistanceFocusActor();


		bool	NEW_MoveToDestPixelPositionDirection(const TPixelPosition& c_rkPPosDst);
		bool	NEW_MoveToMousePickedDirection();
		bool	NEW_MoveToMouseScreenDirection();
		bool	NEW_MoveToDirection(float fDirRot);
		void	NEW_Stop();


		// Reserved
		bool	NEW_IsEmptyReservedDelayTime(float fElapsedtime);	// 네이밍 교정 논의 필요 - [levites]


		// Dungeon
		void	SetDungeonDestinationPosition(int ix, int iy);
		void	AlarmHaveToGo();


		CInstanceBase* NEW_FindActorPtr(DWORD dwVID);
		CInstanceBase* NEW_GetMainActorPtr();

		// flying target set
		void	Clear();
		void	ClearSkillDict(); // 없어지거나 ClearGame 쪽으로 포함될 함수
		void	NEW_ClearSkillData(bool bAll = false);

		void	Update();


		// Play Time
		DWORD	GetPlayTime();
		void	SetPlayTime(DWORD dwPlayTime);

#ifdef ENABLE_NEW_DETAILS_GUI
		long	kill_log[KILL_MAX_NUM];
		long	GetKillLOG(int index) { return kill_log[index]; }
		void	SetKillLOG(long* value) { memcpy(kill_log, value, sizeof(kill_log)); }
#endif


		// System
		void	SetMainCharacterIndex(int iIndex);

		DWORD	GetMainCharacterIndex();
		bool	IsMainCharacterIndex(DWORD dwIndex);
		DWORD	GetGuildID();
		void	NotifyDeletingCharacterInstance(DWORD dwVID);
		void	NotifyCharacterDead(DWORD dwVID);
		void	NotifyCharacterUpdate(DWORD dwVID);
		void	NotifyDeadMainCharacter();
		void	NotifyChangePKMode();


		// Player Status
		const char *	GetName();
		void	SetName(const char *name);

		void	SetRace(DWORD dwRace);
		DWORD	GetRace();

		void	SetWeaponPower(DWORD dwMinPower, DWORD dwMaxPower, DWORD dwMinMagicPower, DWORD dwMaxMagicPower, DWORD dwAddPower);
		void	SetStatus(DWORD dwType, long long lValue);

		long long	GetStatus(DWORD dwType);


		// Item
		void	MoveItemData(TItemPos SrcCell, TItemPos DstCell);
		void	SetItemData(TItemPos Cell, const TItemData & c_rkItemInst);
		const TItemData * GetItemData(TItemPos Cell) const;
		void	SetItemCount(TItemPos Cell, DWORD byCount);
		void	SetItemMetinSocket(TItemPos Cell, DWORD dwMetinSocketIndex, DWORD dwMetinNumber);
		void	SetItemAttribute(TItemPos Cell, DWORD dwAttrIndex, BYTE byType, short sValue);
#ifdef ENABLE_YOHARA_SYSTEM
		void SetSungmaAttribute(TItemPos Cell, DWORD dwSungmaAttrIndex, BYTE byType, short sValue);
		void GetItemApplyRandom(TItemPos Cell, DWORD dwSungmaAttrIndex, BYTE* pwType, short* psValue);
#endif
#ifdef ENABLE_NEW_NAME_ITEM
		void	SetItemNewName(TItemPos itemPos, const char* name);
		const char*	GetItemNewName(TItemPos itemPos);
#endif

		DWORD	GetItemIndex(TItemPos Cell);
		DWORD	GetItemFlags(TItemPos Cell);
		DWORD	GetItemCount(TItemPos Cell);
		DWORD	GetItemCountByVnum(DWORD dwVnum);

#ifdef __AUTO_SKILL_READER__
		DWORD	GetItemCountByVnumNew(DWORD dwVnum, int socket0);
		int		GetSkillLevelNew(DWORD dwSkillIndex);
		int		GetSkillGradeNew(DWORD dwSkillIndex);
#endif

		DWORD	GetItemAntiFlags(TItemPos Cell);
		BYTE	GetItemTypeBySlot(TItemPos Cell);
		BYTE	GetItemSubTypeBySlot(TItemPos Cell);
		
#ifdef ENABLE_SPECIAL_STORAGE
		DWORD	GetItemCountByTT(DWORD dwVnum);
		DWORD	GetSpecialChestItemCount(DWORD dwVnum);
		DWORD	GetSpecialBookItemCount(DWORD dwVnum);
#endif
		DWORD	GetItemMetinSocket(TItemPos Cell, DWORD dwMetinSocketIndex);
		void	GetItemAttribute(TItemPos Cell, DWORD dwAttrSlotIndex, BYTE * pbyType, short * psValue);
		void	SendClickItemPacket(DWORD dwIID);

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
		void 	SendPickUPItemPacket(std::vector<DWORD> itemVIDVector);
#endif
#ifdef ENABLE_SOULBIND_SYSTEM
		void	SetItemBind(TItemPos Cell, long lBindTime);
		long	GetItemBind(TItemPos Cell);
#endif
		void	RequestAddLocalQuickSlot(DWORD dwLocalSlotIndex, DWORD dwWndType, WORD wWndItemPos);
		void	RequestAddToEmptyLocalQuickSlot(DWORD dwWndType, WORD wWndItemPos);
		void	RequestMoveGlobalQuickSlotToLocalQuickSlot(DWORD dwGlobalSrcSlotIndex, DWORD dwLocalDstSlotIndex);
		void	RequestDeleteGlobalQuickSlot(DWORD dwGlobalSlotIndex);
		void	RequestUseLocalQuickSlot(DWORD dwLocalSlotIndex);
		DWORD	LocalQuickSlotIndexToGlobalQuickSlotIndex(DWORD dwLocalSlotIndex);

		void	GetGlobalQuickSlotData(DWORD dwGlobalSlotIndex, DWORD* pdwWndType, DWORD* pdwWndItemPos);
		void	GetLocalQuickSlotData(DWORD dwSlotPos, DWORD* pdwWndType, DWORD* pdwWndItemPos);
		void	RemoveQuickSlotByValue(int iType, int iPosition);

		char	IsItem(TItemPos SlotIndex);

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
		bool    IsBeltInventorySlot(TItemPos Cell);
#endif
		bool	IsInventorySlot(TItemPos SlotIndex);
		bool	IsEquipmentSlot(TItemPos SlotIndex);

		bool	IsEquipItemInSlot(TItemPos iSlotIndex);


		// Quickslot
		int		GetQuickPage();
		void	SetQuickPage(int nPageIndex);
		void	AddQuickSlot(int QuickslotIndex, char IconType, char IconPosition);
		void	DeleteQuickSlot(int QuickslotIndex);
		void	MoveQuickSlot(int Source, int Target);


		// Skill
		void	SetSkill(DWORD dwSlotIndex, DWORD dwSkillIndex);
		bool	GetSkillSlotIndex(DWORD dwSkillIndex, DWORD* pdwSlotIndex);
		int		GetSkillIndex(DWORD dwSlotIndex);
		int		GetSkillGrade(DWORD dwSlotIndex);
		int		GetSkillLevel(DWORD dwSlotIndex);
		float	GetSkillCurrentEfficientPercentage(DWORD dwSlotIndex);
		float	GetSkillNextEfficientPercentage(DWORD dwSlotIndex);
		void	SetSkillLevel(DWORD dwSlotIndex, DWORD dwSkillLevel);
		void	SetSkillLevel_(DWORD dwSkillIndex, DWORD dwSkillGrade, DWORD dwSkillLevel);
		BOOL	IsToggleSkill(DWORD dwSlotIndex);
		void	ClickSkillSlot(DWORD dwSlotIndex);
		void	ChangeCurrentSkillNumberOnly(DWORD dwSlotIndex);
		bool	FindSkillSlotIndexBySkillIndex(DWORD dwSkillIndex, DWORD * pdwSkillSlotIndex);

		void	SetSkillCoolTime(DWORD dwSkillIndex);
		void	EndSkillCoolTime(DWORD dwSkillIndex);

		float	GetSkillCoolTime(DWORD dwSlotIndex);
		float	GetSkillElapsedCoolTime(DWORD dwSlotIndex);
		BOOL	IsSkillActive(DWORD dwSlotIndex);
		BOOL	IsSkillCoolTime(DWORD dwSlotIndex);
		void	UseGuildSkill(DWORD dwSkillSlotIndex);
		bool	AffectIndexToSkillSlotIndex(UINT uAffect, DWORD* pdwSkillSlotIndex);
		bool	AffectIndexToSkillIndex(DWORD dwAffectIndex, DWORD * pdwSkillIndex);

		void	SetAffect(UINT uAffect);
		void	ResetAffect(UINT uAffect);
		void	ClearAffects();
#if defined(ENABLE_FISHING_GAME)
		// Fish Game
		void SetFishingGameWindowOpen(bool bFlag) { m_bIsOpenFishingGameWindow = bFlag; };
		bool GetFishingGameWindowOpen() { return m_bIsOpenFishingGameWindow; }
#endif

		// Target
		void	SetTarget(DWORD dwVID, BOOL bForceChange = TRUE);
		void	OpenCharacterMenu(DWORD dwVictimActorID);
		DWORD	GetTargetVID();


		// Party
		void	ExitParty();
		void	AppendPartyMember(DWORD dwPID, const char * c_szName);
		void	LinkPartyMember(DWORD dwPID, DWORD dwVID);
		void	UnlinkPartyMember(DWORD dwPID);
		void	UpdatePartyMemberInfo(DWORD dwPID, BYTE byState, BYTE byHPPercentage);
		void	UpdatePartyMemberAffect(DWORD dwPID, BYTE byAffectSlotIndex, short sAffectNumber);
		void	RemovePartyMember(DWORD dwPID);
		bool	IsPartyMemberByVID(DWORD dwVID);
		bool	IsPartyMemberByName(const char * c_szName);
		bool	GetPartyMemberPtr(DWORD dwPID, TPartyMemberInfo ** ppPartyMemberInfo);
		bool	PartyMemberPIDToVID(DWORD dwPID, DWORD * pdwVID);
		bool	PartyMemberVIDToPID(DWORD dwVID, DWORD * pdwPID);
		bool	IsSamePartyMember(DWORD dwVID1, DWORD dwVID2);


		// Fight
		void	RememberChallengeInstance(DWORD dwVID);
		void	RememberRevengeInstance(DWORD dwVID);
		void	RememberCantFightInstance(DWORD dwVID);
		void	ForgetInstance(DWORD dwVID);
		bool	IsChallengeInstance(DWORD dwVID);
		bool	IsRevengeInstance(DWORD dwVID);
		bool	IsCantFightInstance(DWORD dwVID);


		// Private Shop
		void	OpenPrivateShop();
		void	ClosePrivateShop();
		bool	IsOpenPrivateShop();
		bool	IsDead();
		bool	IsPoly();



		// Stamina
		void	StartStaminaConsume(DWORD dwConsumePerSec, DWORD dwCurrentStamina);
		void	StopStaminaConsume(DWORD dwCurrentStamina);


		// PK Mode
		DWORD	GetPKMode();


		// Mobile
		void	SetMobileFlag(BOOL bFlag);
		BOOL	HasMobilePhoneNumber();


		// Combo
		void	SetComboSkillFlag(BOOL bFlag);


		// System
		void	SetMovableGroundDistance(float fDistance);


		// Emotion
		void	ActEmotion(DWORD dwEmotionID);
		void	StartEmotionProcess();
		void	EndEmotionProcess();


		// Function Only For Console System
		BOOL	__ToggleCoolTime();
		BOOL	__ToggleLevelLimit();

		__inline const	SAutoPotionInfo& GetAutoPotionInfo(int type) const	{ return m_kAutoPotionInfo[type]; }
		__inline		SAutoPotionInfo& GetAutoPotionInfo(int type)		{ return m_kAutoPotionInfo[type]; }
		__inline void					 SetAutoPotionInfo(int type, const SAutoPotionInfo& info)	{ m_kAutoPotionInfo[type] = info; }
#ifdef ENABLE_ULTIMATE_REGEN
		bool	LoadNewRegen();
		bool	CheckBossSafeRange();
#endif
#ifdef __AUTO_HUNT__
public:
	void	ClearAutoHunt();
	void	AutoHuntLoop();
	void	AutoHuntSettings(const char* szArgument);

protected:
	std::vector<std::pair<BYTE, int>> m_vecAutoHuntItems;
	std::vector<BYTE> m_vecAutoSkills;
	TPixelPosition	m_bAutoHuntStartPoint;
#endif
#ifdef __AUTO_QUQUE_ATTACK__
	public:
		void		AutoFarmLoop();
		bool		AutoFarmQuqueSet(const bool isAdd, const DWORD dwVirtualID);
		DWORD		GetAutoFarmTarget();
		void		SetTotalAutoFarmCount(const BYTE bCount) { bTotalQuqueAutoAttack = bCount; }
		void		SetMetinstonesQueueCount2(const BYTE bCount) {
			if (bTotalQuqueAutoAttack < bCount)
				bTotalQuqueAutoAttack = bCount;
		}

	protected:
		BYTE		bTotalQuqueAutoAttack;
		std::vector<DWORD> m_vecQuqueAutoAttack;
#endif

#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
	// Automatic Hunting
	public:
		void SetAutoUseOnOff(const bool c_bOnOff);
		bool GetAutoUseOnOff() { return m_bAutoUseOnOff; }
	
		void __AutoProcess();
	
		void SetAutoAttachedSlotIndex(const BYTE c_bSlotIndex, const int c_dwAttachedSlotIndex);
		int GetAutoAttachedSlotIndex(const BYTE c_bSlotIndex);
	
		void SetAutoSlotCoolTime(const BYTE c_bSlotIndex, const DWORD c_dwCooldown);
		DWORD GetAutoSlotCoolTime(const BYTE c_bSlotIndex);
	
		DWORD CheckAutoSlotCoolTime(const BYTE c_bSlotIndex, const DWORD c_dwAttachedSlotIndex = 0, const DWORD c_dwEditCooldown = 0);
	
		void ClearAutoSkillSlot();
		void ClearAutoPotionSlot();
		void ClearAutoAllSlot();
	
		void SetAutoSlotLastUsedTime(const BYTE c_bSlotIndex);
		float GetAutoSlotLastUsedTime(const BYTE c_bSlotIndex) { return m_afAutoSlotLastUsedTime[c_bSlotIndex]; }
	
		using TAutoSlotMap = std::unordered_map<BYTE, int>;
	
	private:
		void __AutoAttack();
		void __AutoPotion();
		void __AutoSkill();
	
		TAutoSlotMap m_kMap_dwAutoAttachedSlotIndex;
		TAutoSlotMap m_kMap_dwAutoCooldownSlotIndex;
	
		bool m_bAutoUseOnOff;
		float m_afAutoSlotLastUsedTime[AUTO_POTION_SLOT_MAX];
#endif

	protected:
		TQuickSlot &	__RefLocalQuickSlot(int SlotIndex);
		TQuickSlot &	__RefGlobalQuickSlot(int SlotIndex);


		DWORD	__GetLevelAtk();
		DWORD	__GetStatAtk();
		DWORD	__GetWeaponAtk(DWORD dwWeaponPower);
		DWORD	__GetTotalAtk(DWORD dwWeaponPower, DWORD dwRefineBonus);
		DWORD	__GetRaceStat();
		DWORD	__GetHitRate();
		DWORD	__GetEvadeRate();

		void	__UpdateBattleStatus();

		void	__DeactivateSkillSlot(DWORD dwSlotIndex);
		void	__ActivateSkillSlot(DWORD dwSlotIndex);

		void	__OnPressSmart(CInstanceBase& rkInstMain, bool isAuto);
		void	__OnClickSmart(CInstanceBase& rkInstMain, bool isAuto);

		void	__OnPressItem(CInstanceBase& rkInstMain, DWORD dwPickedItemID);
		void	__OnPressActor(CInstanceBase& rkInstMain, DWORD dwPickedActorID, bool isAuto);
		void	__OnPressGround(CInstanceBase& rkInstMain, const TPixelPosition& c_rkPPosPickedGround);
		void	__OnPressScreen(CInstanceBase& rkInstMain);

		void	__OnClickActor(CInstanceBase& rkInstMain, DWORD dwPickedActorID, bool isAuto);
		void	__OnClickItem(CInstanceBase& rkInstMain, DWORD dwPickedItemID);
		void	__OnClickGround(CInstanceBase& rkInstMain, const TPixelPosition& c_rkPPosPickedGround);

		bool	__IsMovableGroundDistance(CInstanceBase& rkInstMain, const TPixelPosition& c_rkPPosPickedGround);

		bool	__GetPickedActorPtr(CInstanceBase** pkInstPicked);

		bool	__GetPickedActorID(DWORD* pdwActorID);
		bool	__GetPickedItemID(DWORD* pdwItemID);
		bool	__GetPickedGroundPos(TPixelPosition* pkPPosPicked);

		void	__ClearReservedAction();
		void	__ReserveClickItem(DWORD dwItemID);
		void	__ReserveClickActor(DWORD dwActorID);
		void	__ReserveClickGround(const TPixelPosition& c_rkPPosPickedGround);
		void	__ReserveUseSkill(DWORD dwActorID, DWORD dwSkillSlotIndex, DWORD dwRange);

		void	__ReserveProcess_ClickActor();

		void	__ShowPickedEffect(const TPixelPosition& c_rkPPosPickedGround);
		void	__SendClickActorPacket(CInstanceBase& rkInstVictim);

		void	__ClearAutoAttackTargetActorID();
		void	__SetAutoAttackTargetActorID(DWORD dwActorID);

		void	NEW_ShowEffect(int dwEID, TPixelPosition kPPosDst);

		void	NEW_SetMouseSmartState(int eMBS, bool isAuto);
		void	NEW_SetMouseMoveState(int eMBS);
		void	NEW_SetMouseCameraState(int eMBS);
		void	NEW_GetMouseDirRotation(float fScrX, float fScrY, float* pfDirRot);
		void	NEW_GetMultiKeyDirRotation(bool isLeft, bool isRight, bool isUp, bool isDown, float* pfDirRot);

		float	GetDegreeFromDirection(int iUD, int iLR);
		float	GetDegreeFromPosition(int ix, int iy, int iHalfWidth, int iHalfHeight);

		bool	CheckCategory(int iCategory);
		bool	CheckAbilitySlot(int iSlotIndex);

		void	RefreshKeyWalkingDirection();
		void	NEW_RefreshMouseWalkingDirection();


		// Instances
		void	RefreshInstances();

		bool	__CanShot(CInstanceBase& rkInstMain, CInstanceBase& rkInstTarget);
		bool	__CanUseSkill();

		bool	__CanMove();

		bool	__CanAttack();
		bool	__CanChangeTarget();

		bool __CheckSkillUsable(DWORD dwSlotIndex
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, bool bFromAutoHunt = false
#endif
		);
		void	__UseCurrentSkill();
		void	__UseChargeSkill(DWORD dwSkillSlotIndex);
		bool __UseSkill(DWORD dwSlotIndex
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, bool bFromAutoHunt = false
#endif
		);
		bool	__CheckSpecialSkill(DWORD dwSkillIndex);

		bool	__CheckRestSkillCoolTime(DWORD dwSkillSlotIndex);
		bool	__CheckShortLife(TSkillInstance & rkSkillInst, CPythonSkill::TSkillData& rkSkillData);
		bool	__CheckShortMana(TSkillInstance & rkSkillInst, CPythonSkill::TSkillData& rkSkillData);
		bool	__CheckShortArrow(TSkillInstance & rkSkillInst, CPythonSkill::TSkillData& rkSkillData);
		bool	__CheckDashAffect(CInstanceBase& rkInstMain);

		void __SendUseSkill(DWORD dwSkillSlotIndex, DWORD dwTargetVID
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, bool bFromAutoHunt = false
#endif
		);
		void	__RunCoolTime(DWORD dwSkillSlotIndex);

		BYTE	__GetSkillType(DWORD dwSkillSlotIndex);

		bool	__IsReservedUseSkill(DWORD dwSkillSlotIndex);
		bool	__IsMeleeSkill(CPythonSkill::TSkillData& rkSkillData);
		bool	__IsChargeSkill(CPythonSkill::TSkillData& rkSkillData);
		DWORD	__GetSkillTargetRange(CPythonSkill::TSkillData& rkSkillData);
		bool	__SearchNearTarget();
		bool	__IsUsingChargeSkill();

		bool	__ProcessEnemySkillTargetRange(CInstanceBase& rkInstMain, CInstanceBase& rkInstTarget, CPythonSkill::TSkillData& rkSkillData, DWORD dwSkillSlotIndex);


		// Item
		bool	__HasEnoughArrow();
		bool	__HasItem(DWORD dwItemID);
		DWORD	__GetPickableDistance();


		// Target
		CInstanceBase*		__GetTargetActorPtr();
		void				__ClearTarget();
		DWORD				__GetTargetVID();
		void				__SetTargetVID(DWORD dwVID);
		bool				__IsSameTargetVID(DWORD dwVID);
		bool				__IsTarget();
		bool				__ChangeTargetToPickedInstance();

		CInstanceBase *		__GetSkillTargetInstancePtr(CPythonSkill::TSkillData& rkSkillData);
		CInstanceBase *		__GetAliveTargetInstancePtr();
		CInstanceBase *		__GetDeadTargetInstancePtr();

		BOOL				__IsRightButtonSkillMode();
#ifdef __AUTO_QUQUE_ATTACK__
		bool __IsLeftButtonQueueMode();
#endif

		// Update
		void				__Update_AutoAttack();
		void				__Update_NotifyGuildAreaEvent();



		// Emotion
		BOOL				__IsProcessingEmotion();


	protected:
		PyObject *				m_ppyGameWindow;

		// Client Player Data
		std::map<DWORD, DWORD>	m_skillSlotDict;
		std::string				m_stName;
		DWORD					m_dwMainCharacterIndex;
		DWORD					m_dwRace;
		DWORD					m_dwWeaponMinPower;
		DWORD					m_dwWeaponMaxPower;
		DWORD					m_dwWeaponMinMagicPower;
		DWORD					m_dwWeaponMaxMagicPower;
		DWORD					m_dwWeaponAddPower;

		// Todo
		DWORD					m_dwSendingTargetVID;
		float					m_fTargetUpdateTime;

		// Attack
		DWORD					m_dwAutoAttackTargetVID;

		// NEW_Move
		EMode					m_eReservedMode;
		float					m_fReservedDelayTime;

		float					m_fMovDirRot;

		bool					m_isUp;
		bool					m_isDown;
		bool					m_isLeft;
		bool					m_isRight;
		bool					m_isAtkKey;
		bool					m_isDirKey;
		bool					m_isCmrRot;
		bool					m_isSmtMov;
		bool					m_isDirMov;

		float					m_fCmrRotSpd;

		TPlayerStatus			m_playerStatus;

		UINT					m_iComboOld;
		DWORD					m_dwVIDReserved;
		DWORD					m_dwIIDReserved;

		DWORD					m_dwcurSkillSlotIndex;
		DWORD					m_dwSkillSlotIndexReserved;
		DWORD					m_dwSkillRangeReserved;

		TPixelPosition			m_kPPosInstPrev;
		TPixelPosition			m_kPPosReserved;

		// Emotion
		BOOL					m_bisProcessingEmotion;

		// Dungeon
		BOOL					m_isDestPosition;
		int						m_ixDestPos;
		int						m_iyDestPos;
		int						m_iLastAlarmTime;

		// Party
		std::map<DWORD, TPartyMemberInfo>	m_PartyMemberMap;

		// PVP
		std::set<DWORD>			m_ChallengeInstanceSet;
		std::set<DWORD>			m_RevengeInstanceSet;
		std::set<DWORD>			m_CantFightInstanceSet;

		// Private Shop
		bool					m_isOpenPrivateShop;
		bool					m_isObserverMode;
#if defined(ENABLE_FISHING_GAME)
		bool					m_bIsOpenFishingGameWindow;
#endif
		// Stamina
		BOOL					m_isConsumingStamina;
		float					m_fCurrentStamina;
		float					m_fConsumeStaminaPerSec;

		// Guild
		DWORD					m_inGuildAreaID;

		// Mobile
		BOOL					m_bMobileFlag;

		// System
		BOOL					m_sysIsCoolTime;
		BOOL					m_sysIsLevelLimit;

	protected:
		// Game Cursor Data
		TPixelPosition			m_MovingCursorPosition;
		float					m_fMovingCursorSettingTime;
		DWORD					m_adwEffect[EFFECT_NUM];

		DWORD					m_dwVIDPicked;
		DWORD					m_dwIIDPicked;
		int						m_aeMBFButton[MBT_NUM];

		DWORD					m_dwTargetVID;
		DWORD					m_dwTargetEndTime;
		DWORD					m_dwPlayTime;
#ifdef ENABLE_ULTIMATE_REGEN
		std::vector<TNewRegen> m_eventData;
#endif
		SAutoPotionInfo			m_kAutoPotionInfo[AUTO_POTION_TYPE_NUM];

	protected:
		float					MOVABLE_GROUND_DISTANCE;

	private:
		std::map<DWORD, DWORD> m_kMap_dwAffectIndexToSkillIndex;

#if defined(__BL_BATTLE_ROYALE__)
	public:
		void SetBattleRoyaleEnable(bool enable) { m_bIsBattleRoyaleEnabled = enable; }
		bool GetBattleRoyaleEnable() const { return m_bIsBattleRoyaleEnabled; }

	private:
		bool					m_bIsBattleRoyaleEnabled;
#endif

	public:
		void AddAffect(DWORD dwType, TPacketAffectElement kElem);
		void RemoveAffect(DWORD dwType, DWORD dwApplyOn);

		int GetAffectDataIndex(DWORD dwType, DWORD dwApplyOn);
		long GetAffectDuration(DWORD dwType);

		using TAffectDataVector = std::vector<TPacketAffectElement>;
		TAffectDataVector GetAffectDataVector(DWORD dwType);
		TPacketAffectElement GetAffectData(DWORD dwType, BYTE bApplyOn);

	protected:
		TAffectDataVector m_vecAffectData;

#ifdef ENABLE_MINI_GAME_CATCH_KING
	public:
		BYTE GetEventGame() { return bEventStatus; }
		void SetEventGame(BYTE status) { bEventStatus = status; }

	private:
		BYTE bEventStatus;
#endif
#ifdef ENABLE_AURA_SYSTEM
	public:
		enum EItemAuraSockets
		{
			ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM,
			ITEM_SOCKET_AURA_CURRENT_LEVEL,
			ITEM_SOCKET_AURA_BOOST,
		};

		enum EItemAuraMaterialValues
		{
			ITEM_VALUE_AURA_MATERIAL_EXP,
		};

		enum EItemAuraBoostValues
		{
			ITEM_VALUE_AURA_BOOST_PERCENT,
			ITEM_VALUE_AURA_BOOST_TIME,
			ITEM_VALUE_AURA_BOOST_UNLIMITED,
		};

	private:
		typedef struct SAuraRefineInfo
		{
			BYTE bAuraRefineInfoLevel;
			BYTE bAuraRefineInfoExpPercent;
		} TAuraRefineInfo;

		std::vector<TItemData> m_AuraItemInstanceVector;

	protected:
		bool				m_bAuraWindowOpen;
		BYTE				m_bOpenedAuraWindowType;
		TItemPos			m_AuraRefineActivatedCell[AURA_SLOT_MAX];
		TAuraRefineInfo		m_bAuraRefineInfo[AURA_REFINE_INFO_SLOT_MAX];

		void				__ClearAuraRefineWindow();

	public:
		void				SetAuraRefineWindowOpen(BYTE wndType);
		BYTE				GetAuraRefineWindowType() const { return m_bOpenedAuraWindowType; };

		bool				IsAuraRefineWindowOpen() const { return m_bAuraWindowOpen; }
		bool				IsAuraRefineWindowEmpty();

		void				SetAuraRefineInfo(BYTE bAuraRefineInfoSlot, BYTE bAuraRefineInfoLevel, BYTE bAuraRefineInfoExpPercent);
		BYTE				GetAuraRefineInfoLevel(BYTE bAuraRefineInfoSlot);
		BYTE				GetAuraRefineInfoExpPct(BYTE bAuraRefineInfoSlot);

		void				SetAuraItemData(BYTE bSlotIndex, const TItemData& rItemInstance);
		void				DelAuraItemData(BYTE bSlotIndex);

		BYTE				FineMoveAuraItemSlot();
		BYTE				GetAuraCurrentItemSlotCount();

		BOOL				GetAuraItemDataPtr(BYTE bSlotIndex, TItemData** ppInstance);

		void				SetActivatedAuraSlot(BYTE bSlotIndex, TItemPos ItemCell);
		BYTE				FindActivatedAuraSlot(TItemPos ItemCell);
		TItemPos			FindUsingAuraSlot(BYTE bSlotIndex);
#endif

#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
public:
	void		SetAccumulateDamage(uint32_t dwVid, uint32_t dwCount);
	uint32_t 	GetAccumulateDamage(uint32_t dwVid);

protected:
	std::vector<THitCountInfo> m_hitCount;
#endif
};

extern const int c_iFastestSendingCount;
extern const int c_iSlowestSendingCount;
extern const float c_fFastestSendingDelay;
extern const float c_fSlowestSendingDelay;
extern const float c_fRotatingStepTime;

extern const float c_fComboDistance;
extern const float c_fPickupDistance;
extern const float c_fClickDistance;
