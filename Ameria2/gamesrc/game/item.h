#ifndef __INC_METIN_II_GAME_ITEM_H__
#define __INC_METIN_II_GAME_ITEM_H__

#include "entity.h"

class CItem : public CEntity
{
	protected:
		// override methods from ENTITY class
		virtual void	EncodeInsertPacket(LPENTITY entity);
		virtual void	EncodeRemovePacket(LPENTITY entity);

	public:
		CItem(DWORD dwVnum);
		virtual ~CItem();

#ifdef ENABLE_PITTY_REFINE
		int			GetLimitPittyRefine();
#endif

		int			GetLevelLimit();
#ifdef ENABLE_YOHARA_SYSTEM
		int GetConquerorLevelLimit();
#endif
		bool CheckItemUseLevel(int nLevel
#ifdef ENABLE_YOHARA_SYSTEM
, int nConquerorLevel
#endif
		);

		bool		IsPCBangItem();

		long		FindApplyValue(BYTE bApplyType);

		bool		IsStackable()		{ return (GetFlag() & ITEM_FLAG_STACKABLE)?true:false; }

		void		Initialize();
		void		Destroy();

		void		Save();

		void		SetWindow(BYTE b)	{ m_bWindow = b; }
		BYTE		GetWindow()		{ return m_bWindow; }

		void		SetID(DWORD id)		{ m_dwID = id;	}
		DWORD		GetID()			{ return m_dwID; }

		void			SetProto(const TItemTable * table);
		TItemTable const *	GetProto()	{ return m_pProto; }

		long long	GetGold();

		long long	GetShopBuyPrice();

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		const char * GetName() {return m_pProto ? m_stName.c_str() : "";}
#else
		const char * GetName()			{ return m_pProto ? m_pProto->szLocaleName : NULL; }
#endif

		const char *	GetBaseName()		{ return m_pProto ? m_pProto->szName : NULL; }
		BYTE		GetSize()		{ return m_pProto ? m_pProto->bSize : 0;	}

		void		SetFlag(long flag)	{ m_lFlag = flag;	}
		long		GetFlag()		{ return m_lFlag;	}

		void		AddFlag(long bit);
		void		RemoveFlag(long bit);

		inline 		void IncrementCount() { SetCount(GetCount() + 1); }
		inline 		bool DecrementCount() { return SetCount(GetCount() - 1); }
		bool 		IsBooster() const { return GetType() == ITEM_BOOSTER;}

		DWORD		GetWearFlag()		{ return m_pProto ? m_pProto->dwWearFlags : 0; }
		DWORD		GetAntiFlag()		{ return m_pProto ? m_pProto->dwAntiFlags : 0; }
		DWORD		GetImmuneFlag()		{ return m_pProto ? m_pProto->dwImmuneFlag : 0; }
#ifdef ENABLE_SPECIAL_COSTUME_ATTR
		bool		CanBonusCostume();
		BYTE		GetBonusFlag();
		BYTE		GetBonusMaxAttr();
#endif

		void		SetVID(DWORD vid)	{ m_dwVID = vid;	}
		DWORD		GetVID()		{ return m_dwVID;	}

#ifdef ENABLE_NEW_NAME_ITEM
		const char*	GetNewName() const	{return nameex;}
		void		SetNewName(const char* name);
#endif

		bool		SetCount(DWORD count);
		DWORD		GetCount();
#ifdef __SOULBINDING_SYSTEM__
		void	Bind(long lTime);
		long	GetBind()	{return m_lBind;}
		bool	IsBind();
		bool	IsUntilBind();
		void	SetUnBindingExpireEvent(LPEVENT pkEvent);
		void	StartUnBindingExpireEvent();
#endif
		// GetVnum°ú GetOriginalVnum¿¡ ´ëÇÑ comment
		// GetVnumÀº Masking µÈ VnumÀÌ´Ù. ÀÌ¸¦ »ç¿ëÇÔÀ¸·Î½á, ¾ÆÀÌÅÛÀÇ ½ÇÁ¦ VnumÀº 10ÀÌÁö¸¸, VnumÀÌ 20ÀÎ °ÍÃ³·³ µ¿ÀÛÇÒ ¼ö ÀÖ´Â °ÍÀÌ´Ù.
		// Masking °ªÀº ori_to_new.txt¿¡¼­ Á¤ÀÇµÈ °ªÀÌ´Ù.
		// GetOriginalVnumÀº ¾ÆÀÌÅÛ °íÀ¯ÀÇ VnumÀ¸·Î, ·Î±× ³²±æ ¶§, Å¬¶óÀÌ¾ðÆ®¿¡ ¾ÆÀÌÅÛ Á¤º¸ º¸³¾ ¶§, ÀúÀåÇÒ ¶§´Â ÀÌ VnumÀ» »ç¿ëÇÏ¿©¾ß ÇÑ´Ù.
		//
		DWORD		GetVnum() const		{ return m_dwMaskVnum ? m_dwMaskVnum : m_dwVnum;	}
		DWORD		GetOriginalVnum() const		{ return m_dwVnum;	}
		BYTE		GetType() const		{ return m_pProto ? m_pProto->bType : 0;	}
		BYTE		GetSubType() const	{ return m_pProto ? m_pProto->bSubType : 0;	}
		BYTE		GetLimitType(DWORD idx) const { return m_pProto ? m_pProto->aLimits[idx].bType : 0;	}
		long		GetLimitValue(DWORD idx) const { return m_pProto ? m_pProto->aLimits[idx].lValue : 0;	}

		long		GetValue(DWORD idx);

		void		SetCell(LPCHARACTER ch, WORD pos)	{ m_pOwner = ch, m_wCell = pos;	}
		WORD		GetCell()				{ return m_wCell;	}

		LPITEM		RemoveFromCharacter();
		
		bool		AddToCharacter(LPCHARACTER ch, TItemPos Cell);

		LPCHARACTER	GetOwner()		{ return m_pOwner; }

		LPITEM		RemoveFromGround();
		bool		AddToGround(long lMapIndex, const PIXEL_POSITION & pos, bool skipOwnerCheck = false);

		int			FindEquipCell(LPCHARACTER ch, int bCandidateCell = -1);
		bool		IsEquipped() const		{ return m_bEquipped;	}
		bool		EquipTo(LPCHARACTER ch, WORD wWearCell);
		bool		IsEquipable() const;

		bool		CanUsedBy(LPCHARACTER ch);

		bool DistanceValid(LPCHARACTER ch, bool bIsParty);

#ifdef USE_EXPANDED_ITEM_REFRESH
        void UpdatePacket(bool bJustCount = false);
#else
        void UpdatePacket();
#endif
		void		UsePacketEncode(LPCHARACTER ch, LPCHARACTER victim, struct packet_item_use * packet);

		void		SetExchanging(bool isOn = true);
		bool		IsExchanging()		{ return m_bExchanging;	}

		bool		IsTwohanded();

		bool		IsPolymorphItem();

		void		ModifyPoints(bool bAdd);	// ¾ÆÀÌÅÛÀÇ È¿°ú¸¦ Ä³¸¯ÅÍ¿¡ ºÎ¿© ÇÑ´Ù. bAdd°¡ falseÀÌ¸é Á¦°ÅÇÔ

		bool		CreateSocket(BYTE bSlot, BYTE bGold);
		const long *	GetSockets()		{ return &m_alSockets[0];	}
		long		GetSocket(int i)	{ return m_alSockets[i];	}

		void		SetSockets(const long * al);
		void		SetSocket(int i, long v, bool bLog = true);

		int		GetSocketCount();
		bool		AddSocket();

		const TPlayerItemAttribute* GetAttributes()		{ return m_aAttr;	}
		const TPlayerItemAttribute& GetAttribute(int i)	{ return m_aAttr[i];	}

		BYTE		GetAttributeType(int i)	{ return m_aAttr[i].bType;	}
		short		GetAttributeValue(int i){ return m_aAttr[i].sValue;	}

#if defined(ENABLE_YOHARA_SYSTEM) || defined(USE_CRYSTAL_SYSTEM)
		BYTE GetApplyType(int i) const
		{
			return m_pProto ? m_pProto->aApplies[i].bType : 0;
		}
#endif

#ifdef ENABLE_YOHARA_SYSTEM
		long GetApplyValue(int i) { return m_pProto ? m_pProto->aApplies[i].lValue : 0; }

		// Random Attr
		const TPlayerItemApplyRandom* GetApplysRandom() { return m_aApplyRandom; }
		const TPlayerItemApplyRandom& GetApplyRandom(int i) { return m_aApplyRandom[i]; }

		BYTE GetApplyRandomType(int i) { return m_aApplyRandom[i].bType; }
		short GetApplyRandomValue(int i) { return m_aApplyRandom[i].sValue; }
		short GetApplyRandomRow(int i) { return m_aApplyRandom[i].row; }

		WORD GetSungmaUniqueRandomSocket();
		bool GetApplyRandomData(DWORD vnum);
		bool ItemHasApplyRandom();

		// Defaultfunctions
		int GetRandomAttrCount();
		void ClearRandomAttribute();
		void SetRandomAttrs(const TPlayerItemApplyRandom* c_pAttribute);
		void CopyApplyRandomTo(LPITEM pItem);

		// Refine
		void SetItemApplyRandom(int i, BYTE wType, long sValue, BYTE bRow);
		void PutRandomAttributeWithLevel(BYTE bLevel);
		void AddRandomAttr(BYTE wApply, BYTE bLevel);
		bool HasRandomAttr(BYTE wApply);
#endif

		void		SetAttributes(const TPlayerItemAttribute* c_pAttribute);

		int		FindAttribute(BYTE bType);
		bool		RemoveAttributeAt(int index);
		bool		RemoveAttributeType(BYTE bType);

		bool		HasAttr(BYTE bApply);
		bool		HasRareAttr(BYTE bApply);

		void		SetDestroyEvent(LPEVENT pkEvent);
		void		StartDestroyEvent(int iSec=300);

		DWORD		GetRefinedVnum()	{ return m_pProto ? m_pProto->dwRefinedVnum : 0; }
		DWORD		GetRefineFromVnum();
		int		GetRefineLevel();

		void		SetSkipSave(bool b)	{ m_bSkipSave = b; }
#ifdef __BL_BATTLE_ROYALE__
	bool		GetSkipSave() const { return m_bSkipSave || m_NeverSave; }
#else
	bool		GetSkipSave() { return m_bSkipSave; }
#endif

#ifdef USE_EXPANDED_ITEM_REFRESH
        bool GetSkipPacket() const
        {
            return m_bSkipPacket;
        }

        void SetSkipPacket(const bool v)
        {
            m_bSkipPacket = v;
        }
#endif

		bool		IsOwnership(LPCHARACTER ch);
		void		SetOwnership(LPCHARACTER ch, int iSec = 10);
		void		SetOwnershipEvent(LPEVENT pkEvent);

		void		SetLastOwnerPID(DWORD pid) { m_dwLastOwnerPID = pid; }
		DWORD		GetLastOwnerPID()	{ return m_dwLastOwnerPID; }

		int		GetAttributeSetIndex(); // ¼Ó¼º ºÙ´Â°ÍÀ» ÁöÁ¤ÇÑ ¹è¿­ÀÇ ¾î´À ÀÎµ¦½º¸¦ »ç¿ëÇÏ´ÂÁö µ¹·ÁÁØ´Ù.
		void		AlterToMagicItem();
		void		AlterToSocketItem(int iSocketCount);

		WORD		GetRefineSet()		{ return m_pProto ? m_pProto->wRefineSet : 0;	}

		void		StartUniqueExpireEvent();
		void		SetUniqueExpireEvent(LPEVENT pkEvent);

		void		StartTimerBasedOnWearExpireEvent();
		void		SetTimerBasedOnWearExpireEvent(LPEVENT pkEvent);

		void		StartRealTimeExpireEvent();
		bool		IsRealTimeItem();

		void		StopUniqueExpireEvent();
		void		StopTimerBasedOnWearExpireEvent();
		void		StopAccessorySocketExpireEvent();

		//			ÀÏ´Ü REAL_TIME°ú TIMER_BASED_ON_WEAR ¾ÆÀÌÅÛ¿¡ ´ëÇØ¼­¸¸ Á¦´ë·Î µ¿ÀÛÇÔ.
		int			GetDuration();

		int		GetAttributeCount();
		void		ClearAttribute();
		void		ClearAttributeDragonSoul();
		void		ChangeAttribute(const int* aiChangeProb=NULL);
		void		ChangeAttributeSpecial(const int* aiChangeProb=NULL);
		void		AddAttribute();
		void		AddAttribute(BYTE bType, short sValue);

		bool 		IsAttrRings(){return is_attr_rings;}
		void 		SetAttrRings(bool rings){is_attr_rings = rings;}

#ifdef ENABLE_SWITCHBOT_WORLDARD
		void 		ApplyAddon(int iAddonType, bool switchbot = false);
#else
		void 		ApplyAddon(int iAddonType);
#endif
		bool		CheckEnchantToItem(LPITEM item2);
		bool		IsPendantSoul();
		int		GetSpecialGroup() const;
		bool	IsSameSpecialGroup(const LPITEM item) const;

		// ACCESSORY_REFINE
		// ¾×¼¼¼­¸®¿¡ ±¤»êÀ» ÅëÇØ ¼ÒÄÏÀ» Ãß°¡
		bool		IsAccessoryForSocket();

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
		void	SetSkipUpdatePacket(bool bState) {m_bSkipUpdatePacket = bState;}
		bool	m_bSkipUpdatePacket;
#endif

		int		GetAccessorySocketGrade();
		int		GetAccessorySocketMaxGrade();
		int		GetAccessorySocketDownGradeTime();

		void		SetAccessorySocketGrade(int iGrade);
		void		SetAccessorySocketMaxGrade(int iMaxGrade);
		void		SetAccessorySocketDownGradeTime(DWORD time);

		void		AccessorySocketDegrade();

		// ¾Ç¼¼»ç¸® ¸¦ ¾ÆÀÌÅÛ¿¡ ¹Û¾ÒÀ»¶§ Å¸ÀÌ¸Ó µ¹¾Æ°¡´Â°Í( ±¸¸®, µî )
		void		StartAccessorySocketExpireEvent();
		void		SetAccessorySocketExpireEvent(LPEVENT pkEvent);

		bool		CanPutInto(LPITEM item);
		// END_OF_ACCESSORY_REFINE

		int 		GetAccessorySocketVnumRings(LPITEM item);


		void		CopyAttributeTo(LPITEM pItem);
		void		CopySocketTo(LPITEM pItem);

		int			GetRareAttrCount();
		bool		AddRareAttribute();
		bool		ChangeRareAttribute();
		bool		ChangeRareAttribute2();

		void 		ChangeAttrCostume();
		void 		AddAttrCostume();
		void		AddAttrCostumeNew(BYTE bApply, BYTE bLevel);
		void		PutAttributeCostume(const int * aiAttrPercentTable);
		void		PutAttributeWithLevelCostume(BYTE bLevel);


		void 		ChangeAttrCostumeHair();
		void 		AddAttrCostumeHair();
		void		AddAttrCostumeHairNew(BYTE bApply, BYTE bLevel);
		void		PutAttributeCostumeHair(const int * aiAttrPercentTable);
		void		PutAttributeWithLevelCostumeHair(BYTE bLevel);



		void		AttrLog();

		void		Lock(bool f) { m_isLocked = f; }
		bool		isLocked() const { return m_isLocked; }

	private :
		void		SetAttribute(int i, BYTE bType, short sValue);

	public:
		void		SetForceAttribute(int i, BYTE bType, short sValue);

	protected:
		bool		EquipEx(bool is_equip);
		bool		Unequip();

		void		AddAttr(BYTE bApply, BYTE bLevel);
		void		PutAttribute(const int * aiAttrPercentTable);
		void		PutAttributeWithLevel(BYTE bLevel);

	public:
		void		AddRareAttribute2(const int * aiAttrPercentTable = NULL);
	protected:
		void		AddRareAttr(BYTE bApply, BYTE bLevel);
		void		PutRareAttribute(const int * aiAttrPercentTable);
		void		PutRareAttributeWithLevel(BYTE bLevel);

	protected:
		friend class CInputDB;
		bool		OnAfterCreatedItem();			// ¼­¹ö»ó¿¡ ¾ÆÀÌÅÛÀÌ ¸ðµç Á¤º¸¿Í ÇÔ²² ¿ÏÀüÈ÷ »ý¼º(·Îµå)µÈ ÈÄ ºÒ¸®¿ì´Â ÇÔ¼ö.

	public:
		bool		IsRideItem();
		bool		IsRamadanRing();
		bool		IsNewEqItem();

		void		ClearMountAttributeAndAffect();
		bool		IsNewMountItem();

		// µ¶ÀÏ¿¡¼­ ±âÁ¸ Ä³½Ã ¾ÆÀÌÅÛ°ú °°Áö¸¸, ±³È¯ °¡´ÉÇÑ Ä³½Ã ¾ÆÀÌÅÛÀ» ¸¸µç´Ù°í ÇÏ¿©,
		// ¿À¸®Áö³Î ¾ÆÀÌÅÛ¿¡, ±³È¯ ±ÝÁö ÇÃ·¡±×¸¸ »èÁ¦ÇÑ »õ·Î¿î ¾ÆÀÌÅÛµéÀ» »õ·Î¿î ¾ÆÀÌÅÛ ´ë¿ª¿¡ ÇÒ´çÇÏ¿´´Ù.
		// ¹®Á¦´Â »õ·Î¿î ¾ÆÀÌÅÛµµ ¿À¸®Áö³Î ¾ÆÀÌÅÛ°ú °°Àº È¿°ú¸¦ ³»¾ßÇÏ´Âµ¥,
		// ¼­¹ö°Ç, Å¬¶ó°Ç, vnum ±â¹ÝÀ¸·Î µÇ¾îÀÖ¾î
		// »õ·Î¿î vnumÀ» ÁË´Ù ¼­¹ö¿¡ »õ·Î ´Ù ¹Ú¾Æ¾ßÇÏ´Â ¾ÈÅ¸±î¿î »óÈ²¿¡ ¸Â´ê¾Ò´Ù.
		// ±×·¡¼­ »õ vnumÀÇ ¾ÆÀÌÅÛÀÌ¸é, ¼­¹ö¿¡¼­ µ¹¾Æ°¥ ¶§´Â ¿À¸®Áö³Î ¾ÆÀÌÅÛ vnumÀ¸·Î ¹Ù²ã¼­ µ¹°í ÇÏ°í,
		// ÀúÀåÇÒ ¶§¿¡ º»·¡ vnumÀ¸·Î ¹Ù²ãÁÖµµ·Ï ÇÑ´Ù.

		// Mask vnumÀº ¾î¶² ÀÌÀ¯(ex. À§ÀÇ »óÈ²)·Î ÀÎÇØ vnumÀÌ ¹Ù²î¾î µ¹¾Æ°¡´Â ¾ÆÀÌÅÛÀ» À§ÇØ ÀÖ´Ù.
		void		SetMaskVnum(DWORD vnum)	{	m_dwMaskVnum = vnum; }
		DWORD		GetMaskVnum()			{	return m_dwMaskVnum; }
		bool		IsMaskedItem()	{	return m_dwMaskVnum != 0;	}
		// bool		IsTalisman();
		bool IsArmorGlove() const { return GetType() == ITEM_ARMOR && GetSubType() == ARMOR_GLOVE; }

#ifdef ENABLE_NEW_PET_SYSTEM
		bool		IsCostumePetItem();
		bool		IsNewPetItem();

#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		bool		IsCostumeMount();
#endif
		bool		IsSashSkin();
#ifdef ENABLE_MOUNT_SKIN
		bool		IsCostumeMountSkin();
#endif

		// ¿ëÈ¥¼®
		bool		IsDragonSoul();
		int			GiveMoreTime_Per(float fPercent);
		int			GiveMoreTime_Fix(DWORD dwTime);
#ifdef ENABLE_LOCK_ATTR
		bool		IsAddonItem() { return (GetProto() && GetProto()->sAddonType) ? true : false; }
		BYTE		GetAttrLockIndex(BYTE index);
		int			GetEmptyAttrIndex();
#endif
#ifdef ENABLE_SPECIAL_STORAGE
		bool 		IsUpgradeItem();
		bool 		IsBook();
		bool 		IsStone();
		bool 		IsChange();
		bool 		IsCostume();
		bool 		IsCostumeSkin();
		bool 		IsChest();
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
	public:
		int		ChangeAttributeSwtichBot(int count,const TPlayerItemAttribute* aAttr,bool*check_add);
		void	SetAttributeSwtichBot(int i, BYTE bType, short sValue);
		int		GetCountSelect(const TPlayerItemAttribute* aAttr);
		bool 	CheckBonusSwitchBot(const TPlayerItemAttribute* aAttr);
		void 	AddAttributeSwitchBot(BYTE bApply, short sValue);

#endif

	private:
		TItemTable const * m_pProto;		// ÇÁ·ÎÅä Å¸ÀÙ

		DWORD		m_dwVnum;
		LPCHARACTER	m_pOwner;

		BYTE		m_bWindow;		// ÇöÀç ¾ÆÀÌÅÛÀÌ À§Ä¡ÇÑ À©µµ¿ì
		DWORD		m_dwID;			// °íÀ¯¹øÈ£
		bool		m_bEquipped;	// ÀåÂø µÇ¾ú´Â°¡?
		DWORD		m_dwVID;		// VID
		WORD		m_wCell;		// À§Ä¡
		DWORD		m_dwCount;		// °³¼ö
#ifdef __SOULBINDING_SYSTEM__
		long		m_lBind;
		LPEVENT		m_pkBindingExpireEvent;
#endif
		long		m_lFlag;		// Ãß°¡ flag
		DWORD		m_dwLastOwnerPID;	// ¸¶Áö¸· °¡Áö°í ÀÖ¾ú´ø »ç¶÷ÀÇ PID

		bool		m_bExchanging;	///< ÇöÀç ±³È¯Áß »óÅÂ

		long		m_alSockets[ITEM_SOCKET_MAX_NUM];	// ¾ÆÀÌÅÛ ¼ÒÄ¹
		TPlayerItemAttribute	m_aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
		TPlayerItemApplyRandom m_aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		std::string m_stName;
#endif

		LPEVENT		m_pkDestroyEvent;
		LPEVENT		m_pkExpireEvent;
		LPEVENT		m_pkUniqueExpireEvent;
		LPEVENT		m_pkTimerBasedOnWearExpireEvent;
		LPEVENT		m_pkRealTimeExpireEvent;

		LPEVENT		m_pkAccessorySocketExpireEvent;
		LPEVENT		m_pkOwnershipEvent;

		DWORD		m_dwOwnershipPID;

		bool		m_bSkipSave;

#ifdef USE_EXPANDED_ITEM_REFRESH
        bool m_bSkipPacket;
#endif

		bool		m_isLocked;

		DWORD		m_dwMaskVnum;
		DWORD		m_dwSIGVnum;

#ifdef ENABLE_NEW_NAME_ITEM
		char		nameex[ITEM_NAME_MAX_LEN+1];
#endif
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
		int			destroyTime;
		long		dropMapIndex;
		PIXEL_POSITION		dropPos;
		bool		dropskipOwnerCheck;
#endif
		bool		is_attr_rings;
	public:
		void SetSIGVnum(DWORD dwSIG)
		{
			m_dwSIGVnum = dwSIG;
		}
		DWORD	GetSIGVnum() const
		{
			return m_dwSIGVnum;
		}
#ifdef ENABLE_PENDANT_SYSTEM_RENEWAL
	public:
		// BYTE		GetTalismanType();
		bool		IsTalisman() { return (GetVnum() >= 9600 && GetVnum() <= 10950); }
#endif
// #ifdef ENABLE_MULTI_CHEST
public:
	bool	GetUpdateStatus(){return m_dwUpdateStatus;}
	void	SetUpdateStatus(bool bFlag){m_dwUpdateStatus=bFlag;}
protected:
	bool	m_dwUpdateStatus;
// #endif

#ifdef ENABLE_COPY_ATTR_ITEM
public:
	bool	CanCopyAttrItem();
#endif
#ifdef __FAKE_PLAYER__
	public:
		void StopExpireEvents(); 
#endif	
#ifdef __AURA_SYSTEM__
	private:
		LPEVENT m_pkAuraBoostSocketExpireEvent;

	public:
		bool	IsAuraBoosterForSocket();

		void	StartAuraBoosterSocketExpireEvent();
		void	StopAuraBoosterSocketExpireEvent();
		void	SetAuraBoosterSocketExpireEvent(LPEVENT pkEvent);
#endif

#ifdef USE_BOOST_POTIONS
	public:
		bool IsBoostPotion();
		bool DeactivateBoostPotion();
		bool IsTimeStackable();
#endif

#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
	public:
		bool IsTimerBasedOnWear();
#endif

#ifdef USE_CRYSTAL_SYSTEM
	public:
		bool IsBoostCrystal();
		bool ModifyBoostCrystal(bool bAdd, bool bAffectLoading, bool bMsg);
#endif

#ifdef __BL_BATTLE_ROYALE__
public:
	// Can not be unset once it's being set.
	void SetNeverSave() { m_NeverSave = true; }
	bool IsNeverSave() const { return m_NeverSave; }
private:
	bool m_NeverSave;
#endif
};

EVENTINFO(item_event_info)
{
	LPITEM item;
	char szOwnerName[CHARACTER_NAME_MAX_LEN];

	item_event_info()
	: item( 0 )
	{
		::memset( szOwnerName, 0, CHARACTER_NAME_MAX_LEN );
	}
};

EVENTINFO(item_vid_event_info)
{
	DWORD item_vid;

	item_vid_event_info()
	: item_vid( 0 )
	{
	}
};

#endif
