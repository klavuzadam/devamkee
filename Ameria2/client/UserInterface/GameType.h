#pragma once
#include "../GameLib/ItemData.h"

struct SAffects
{
	enum
	{
		AFFECT_MAX_NUM = 32,
	};

	SAffects() : dwAffects(0) {}
	SAffects(const DWORD & c_rAffects)
	{
		__SetAffects(c_rAffects);
	}
	int operator = (const DWORD & c_rAffects)
	{
		__SetAffects(c_rAffects);
	}

	BOOL IsAffect(BYTE byIndex)
	{
		return dwAffects & (1 << byIndex);
	}

	void __SetAffects(const DWORD & c_rAffects)
	{
		dwAffects = c_rAffects;
	}

	DWORD dwAffects;
};

extern std::string g_strGuildSymbolPathName;

const DWORD c_Name_Max_Length = 64;
const DWORD c_FileName_Max_Length = 128;
const DWORD c_Short_Name_Max_Length = 32;

const DWORD c_Inventory_Page_Column = 5;
const DWORD c_Inventory_Page_Row = 9;
const DWORD c_Inventory_Page_Size = c_Inventory_Page_Column*c_Inventory_Page_Row; // x*y
#ifdef ENABLE_EXTEND_INVEN_SYSTEM
const DWORD c_Inventory_Page_Count = 4;
#else
const DWORD c_Inventory_Page_Count = 2;
#endif
const DWORD c_ItemSlot_Count = c_Inventory_Page_Size * c_Inventory_Page_Count;
const DWORD c_Equipment_Count = 12;

const DWORD c_Equipment_Start = c_ItemSlot_Count;

const DWORD c_Equipment_Body	= c_Equipment_Start + 0;
const DWORD c_Equipment_Head	= c_Equipment_Start + 1;
const DWORD c_Equipment_Shoes	= c_Equipment_Start + 2;
const DWORD c_Equipment_Wrist	= c_Equipment_Start + 3;
const DWORD c_Equipment_Weapon	= c_Equipment_Start + 4;
const DWORD c_Equipment_Neck	= c_Equipment_Start + 5;
const DWORD c_Equipment_Ear		= c_Equipment_Start + 6;
const DWORD c_Equipment_Unique1	= c_Equipment_Start + 7;
const DWORD c_Equipment_Unique2	= c_Equipment_Start + 8;
const DWORD c_Equipment_Arrow	= c_Equipment_Start + 9;
const DWORD c_Equipment_Shield	= c_Equipment_Start + 10;

// 새로 추가된 신규 반지 & 벨트
// 장착형 아이템에 할당할 수 있는 위치가 기존 장비, 채기랍 퀘스트 보상, 코스튬 시스템 등으로 인해서 공간이 잘려있다.
// 이게 다 채기랍 보상 버프를 장착아이템처럼 구현한 ㅅㄲ 때문에 난리났따... ㅆㅂ
//
// 정리하면, 기존 장비창들은 서버DB상 아이템 포지션이 90 ~ 102 이고,
// 2013년 초에 새로 추가되는 슬롯들은 111 ~ 부터 시작한다. 착용 장비에서 최대로 사용할 수 있는 값은 121 까지이고, 122부터는 용혼석에서 사용한다.
#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
#ifdef ENABLE_SASH_SYSTEM
	const DWORD c_New_Equipment_Start = c_Equipment_Start + 21 + 1;
#else
	const DWORD c_New_Equipment_Start = c_Equipment_Start + 21+1;
#endif
	const DWORD c_New_Equipment_Count = 10;
	const DWORD c_Equipment_Ring1 = c_New_Equipment_Start + 0;
	const DWORD c_Equipment_Ring2 = c_New_Equipment_Start + 1;
	const DWORD c_Equipment_Belt  = c_New_Equipment_Start + 2;
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	const DWORD c_Equipment_Glove = c_New_Equipment_Start + 6; //In my example this number = 27, because it's 21 + 6 = 27, you will need check what is your number. 
#endif

#ifdef ITEM_TALISMAN_EQUIPMENT
	const DWORD c_Equipment_Pendant = c_New_Equipment_Start + 7;
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
	const DWORD c_Equipment_OldPet = c_New_Equipment_Start + 8;
	const DWORD c_Equipment_Pet = c_New_Equipment_Start + 9;
#endif
const DWORD c_Equipment_New_Ring1 = c_Equipment_Start + CItemData::WEAR_RING_SLOT1;
const DWORD c_Equipment_New_Ring2 = c_Equipment_Start + CItemData::WEAR_RING_SLOT2;
const DWORD c_Equipment_New_Ring3 = c_Equipment_Start + CItemData::WEAR_RING_SLOT3;
const DWORD c_Equipment_New_Ring4 = c_Equipment_Start + CItemData::WEAR_RING_SLOT4;
const DWORD c_Equipment_New_Ring5 = c_Equipment_Start + CItemData::WEAR_RING_SLOT5;
const DWORD c_Equipment_New_Ring6 = c_Equipment_Start + CItemData::WEAR_RING_SLOT6;
const DWORD c_Equipment_New_Ring7 = c_Equipment_Start + CItemData::WEAR_RING_SLOT7;
#ifdef ENABLE_SHINING_SYSTEM
const DWORD c_Shining_Slot_Start = c_New_Equipment_Start+c_New_Equipment_Count;
const DWORD c_Shining_Slot_Count = 3;
const DWORD c_Shining_Slot_End = c_Shining_Slot_Start + c_Shining_Slot_Count;
#endif

const DWORD	c_EquipmentPendantFirst = c_Equipment_Start + CItemData::WEAR_PENDANT_SOUL_FIRST;
const DWORD	c_EquipmentPendantSecond = c_Equipment_Start + CItemData::WEAR_PENDANT_SOUL_SECOND;
#ifdef ENABLE_MOUNT_SKIN
const DWORD c_Equipment_MountSkin = c_Shining_Slot_End;
#endif

enum EDragonSoulDeckType
{
	DS_DECK_1,
	DS_DECK_2,
	DS_DECK_MAX_NUM = 2,
};

enum EDragonSoulGradeTypes
{
	DRAGON_SOUL_GRADE_NORMAL,
	DRAGON_SOUL_GRADE_BRILLIANT,
	DRAGON_SOUL_GRADE_RARE,
	DRAGON_SOUL_GRADE_ANCIENT,
	DRAGON_SOUL_GRADE_LEGENDARY,
#ifdef ENABLE_DS_GRADE_MYTH
	DRAGON_SOUL_GRADE_MYTH,
#endif
	DRAGON_SOUL_GRADE_MAX,

};

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
enum eAutomaticPickUP
{
	AUTOMATIC_PICK_UP_ACTIVATE = (1 << 0),

	AUTOMATIC_PICK_UP_WEAPON = (1 << 1),
	AUTOMATIC_PICK_UP_ARMOR = (1 << 2),
	AUTOMATIC_PICK_UP_SHIELD = (1 << 3),
	AUTOMATIC_PICK_UP_RINGS = (1 << 4),
	AUTOMATIC_PICK_UP_HELMETS = (1 << 5),
	AUTOMATIC_PICK_UP_BRACELETS = (1 << 6),
	AUTOMATIC_PICK_UP_NECKLACE = (1 << 7),
	AUTOMATIC_PICK_UP_EARRINGS = (1 << 8),
	AUTOMATIC_PICK_UP_COSTUMES = (1 << 9),
	AUTOMATIC_PICK_UP_PET_MOUNT = (1 << 10),
	AUTOMATIC_PICK_UP_SASH = (1 << 11),
	AUTOMATIC_PICK_UP_TALISMAN = (1 << 12),
	AUTOMATIC_PICK_UP_BOOKS = (1 << 13),
	AUTOMATIC_PICK_UP_STONES = (1 << 14),
	AUTOMATIC_PICK_UP_YANG = (1 << 15),
	AUTOMATIC_PICK_UP_CHESTS = (1 << 16),
};
#endif

enum EDragonSoulStepTypes
{
	DRAGON_SOUL_STEP_LOWEST,
	DRAGON_SOUL_STEP_LOW,
	DRAGON_SOUL_STEP_MID,
	DRAGON_SOUL_STEP_HIGH,
	DRAGON_SOUL_STEP_HIGHEST,
	DRAGON_SOUL_STEP_MAX,
};

#ifdef WJ_ENABLE_TRADABLE_ICON
enum ETopWindowTypes
{
	ON_TOP_WND_NONE,
	ON_TOP_WND_SHOP,
	ON_TOP_WND_EXCHANGE,
	ON_TOP_WND_SAFEBOX,
	ON_TOP_WND_PRIVATE_SHOP,
	ON_TOP_WND_ITEM_COMB,
	ON_TOP_WND_PET_FEED,
#if defined(__BL_67_ATTR__)
	ON_TOP_WND_ATTR_67,
#endif
	ON_TOP_WND_MAX,
};
#endif

#ifdef CHANGE_EQUIP_WORLDARD 

enum ChangeEquipConfig
{
	CHANGE_EQUIP_PAGE_EXTRA = 3,
	//CHANGE_EQUIP_SLOT_COUNT = 29*CHANGE_EQUIP_PAGE_EXTRA,
	CHANGE_EQUIP_SLOT_COUNT = 34*CHANGE_EQUIP_PAGE_EXTRA,
};
	
#endif

#ifdef ENABLE_COSTUME_SYSTEM
	const DWORD c_Costume_Slot_Start	= c_Equipment_Start + 19;	// [주의] 숫자(19) 하드코딩 주의. 현재 서버에서 코스츔 슬롯은 19부터임. 서버 common/length.h 파일의 EWearPositions 열거형 참고.
	const DWORD	c_Costume_Slot_Body		= c_Costume_Slot_Start + 0;
	const DWORD	c_Costume_Slot_Hair		= c_Costume_Slot_Start + 1;

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	const DWORD	c_Costume_Slot_Mount	= c_Costume_Slot_Start + 2;
#endif

#ifdef ENABLE_SASH_SYSTEM
	const DWORD	c_Costume_Slot_Sash = c_Costume_Slot_Start + 3;
#endif
#ifdef ENABLE_AURA_SYSTEM
	const DWORD c_Costume_Slot_Aura		= c_Costume_Slot_Start + 4;
#endif


#if defined(ENABLE_WEAPON_COSTUME_SYSTEM) || defined(ENABLE_SASH_SYSTEM)
	const DWORD c_Costume_Slot_Count	= 5;
#endif
	const DWORD c_Costume_Slot_End		= c_Costume_Slot_Start + c_Costume_Slot_Count;

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	const DWORD	c_Costume_Slot_Weapon	= c_Costume_Slot_End + 1;
#endif

#endif


const DWORD c_Rings_Slot_Start = c_Costume_Slot_Weapon+1;
const DWORD c_Rings_Slot_Count = 2;
const DWORD c_Rings_Slot_End = c_Rings_Slot_Start + c_Rings_Slot_Count;


const DWORD c_sash_skin_Slot_Start = c_Rings_Slot_End;
const DWORD c_sash_sking_Slot_Count = 1;
const DWORD c_sash_sking_Slot_End = c_sash_skin_Slot_Start + c_sash_sking_Slot_Count;


#ifdef CHANGE_EQUIP_WORLDARD 

const DWORD c_change_equip_Slot_Start = c_sash_sking_Slot_End;
const DWORD c_change_equip_Slot_End = c_change_equip_Slot_Start + CHANGE_EQUIP_SLOT_COUNT;

#endif

const DWORD c_Wear_Max = 65;
const DWORD c_DragonSoul_Equip_Start = c_ItemSlot_Count + c_Wear_Max;
const DWORD c_DragonSoul_Equip_Slot_Max = 6;
const DWORD c_DragonSoul_Equip_End = c_DragonSoul_Equip_Start + c_DragonSoul_Equip_Slot_Max * DS_DECK_MAX_NUM;

const DWORD c_DragonSoul_Equip_Reserved_Count = c_DragonSoul_Equip_Slot_Max * 3;

#ifdef __BUFFI_SUPPORT__
const DWORD	c_Buffi_Slot_Body = c_Equipment_Start + CItemData::WEAR_BUFFI_BODY;
const DWORD	c_Buffi_Slot_Head = c_Equipment_Start + CItemData::WEAR_BUFFI_HEAD;
const DWORD	c_Buffi_Slot_Weapon = c_Equipment_Start + CItemData::WEAR_BUFFI_WEAPON;
const DWORD	c_Buffi_Slot_Sash = c_Equipment_Start + CItemData::WEAR_BUFFI_SASH;
#ifdef __BUFFI_SHINING__
const DWORD	c_Buffi_Slot_ShiningWeapon = c_Equipment_Start + CItemData::WEAR_BUFFI_SHINING_WEAPON;
const DWORD	c_Buffi_Slot_ShiningArmor = c_Equipment_Start + CItemData::WEAR_BUFFI_SHINING_ARMOR;
#endif
const DWORD	c_Buffi_Slot_Ring0 = c_Equipment_Start + CItemData::WEAR_BUFFI_RING_0;
const DWORD	c_Buffi_Slot_Ring1 = c_Equipment_Start + CItemData::WEAR_BUFFI_RING_1;
const DWORD	c_Buffi_Slot_Ring2 = c_Equipment_Start + CItemData::WEAR_BUFFI_RING_2;
#endif

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	// 벨트 아이템이 제공하는 인벤토리
	const DWORD c_Belt_Inventory_Slot_Start = c_DragonSoul_Equip_End + c_DragonSoul_Equip_Reserved_Count;
	const DWORD c_Belt_Inventory_Width = 4;
	const DWORD c_Belt_Inventory_Height= 4;
	const DWORD c_Belt_Inventory_Slot_Count = c_Belt_Inventory_Width * c_Belt_Inventory_Height;
	const DWORD c_Belt_Inventory_Slot_End = c_Belt_Inventory_Slot_Start + c_Belt_Inventory_Slot_Count;

	const DWORD c_Inventory_Count	= c_Belt_Inventory_Slot_End;
#else
	const DWORD c_Inventory_Count	= c_DragonSoul_Equip_End;
#endif

// 용혼석 전용 인벤토리
const DWORD c_DragonSoul_Inventory_Start = 0;
const DWORD c_DragonSoul_Inventory_Box_Size = 32;
const DWORD c_DragonSoul_Inventory_Count = CItemData::DS_SLOT_NUM_TYPES * DRAGON_SOUL_GRADE_MAX * c_DragonSoul_Inventory_Box_Size;
const DWORD c_DragonSoul_Inventory_End = c_DragonSoul_Inventory_Start + c_DragonSoul_Inventory_Count;
#ifdef ENABLE_SPECIAL_STORAGE
const DWORD c_Special_Inventory_Page_Size = 5*9;
const DWORD c_Special_Inventory_Page_Count = 4;
const DWORD c_Special_ItemSlot_Count = c_Special_Inventory_Page_Size * c_Special_Inventory_Page_Count;
#endif

enum ESlotType
{
	SLOT_TYPE_NONE,
	SLOT_TYPE_INVENTORY,
	SLOT_TYPE_SKILL,
	SLOT_TYPE_EMOTION,
	SLOT_TYPE_SHOP,
	SLOT_TYPE_EXCHANGE_OWNER,
	SLOT_TYPE_EXCHANGE_TARGET,
	SLOT_TYPE_QUICK_SLOT,
	SLOT_TYPE_SAFEBOX,
	SLOT_TYPE_PRIVATE_SHOP,
	SLOT_TYPE_MALL,
	SLOT_TYPE_DRAGON_SOUL_INVENTORY,
#ifdef ENABLE_SPECIAL_STORAGE
	SLOT_TYPE_UPGRADE_INVENTORY,
	SLOT_TYPE_BOOK_INVENTORY,
	SLOT_TYPE_STONE_INVENTORY,
	SLOT_TYPE_CHANGE_INVENTORY,
	SLOT_TYPE_COSTUME_INVENTORY,
	SLOT_TYPE_CHEST_INVENTORY,
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
	SLOT_TYPE_SWITCHBOT,
#endif
#ifdef ENABLE_SWITCHBOT
	SLOT_TYPE_SWITCHBOT,
#endif
#ifdef CHANGE_EQUIP_WORLDARD
	SLOT_TYPE_CHANGE_EQUIP,
#endif
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
	SLOT_TYPE_AUTO,
#endif
#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_ULTIMATE
	SLOT_TYPE_OFFLINESHOP,
#endif
#endif
#ifdef ENABLE_AURA_SYSTEM
	SLOT_TYPE_AURA,
#endif
	SLOT_TYPE_MAX,
};
#ifdef ENABLE_AURA_SYSTEM
const BYTE c_AuraMaxLevel = 250;

enum EAuraRefineInfoSlot
{
	AURA_REFINE_INFO_SLOT_CURRENT,
	AURA_REFINE_INFO_SLOT_NEXT,
	AURA_REFINE_INFO_SLOT_EVOLVED,
	AURA_REFINE_INFO_SLOT_MAX
};

enum EAuraWindowType
{
	AURA_WINDOW_TYPE_ABSORB,
	AURA_WINDOW_TYPE_GROWTH,
	AURA_WINDOW_TYPE_EVOLVE,
	AURA_WINDOW_TYPE_MAX,
};

enum EAuraSlotType
{
	AURA_SLOT_MAIN,
	AURA_SLOT_SUB,
	AURA_SLOT_RESULT,
	AURA_SLOT_MAX
};

enum EAuraRefineInfoType
{
	AURA_REFINE_INFO_STEP,
	AURA_REFINE_INFO_LEVEL_MIN,
	AURA_REFINE_INFO_LEVEL_MAX,
	AURA_REFINE_INFO_NEED_EXP,
	AURA_REFINE_INFO_MATERIAL_VNUM,
	AURA_REFINE_INFO_MATERIAL_COUNT,
	AURA_REFINE_INFO_NEED_GOLD,
	AURA_REFINE_INFO_EVOLVE_PCT,
	AURA_REFINE_INFO_MAX
};
#endif


enum EWindows
{
	RESERVED_WINDOW,
	INVENTORY,				// 기본 인벤토리. (45칸 짜리가 2페이지 존재 = 90칸)
	EQUIPMENT,
	SAFEBOX,
	MALL,
	DRAGON_SOUL_INVENTORY,
#ifdef ENABLE_SPECIAL_STORAGE
	UPGRADE_INVENTORY,
	BOOK_INVENTORY,
	STONE_INVENTORY,
	CHANGE_INVENTORY,
	COSTUME_INVENTORY,
	CHEST_INVENTORY,
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
	SWITCHBOT,
#endif
#ifdef ENABLE_SWITCHBOT
	SWITCHBOT,
#endif
#ifdef CHANGE_EQUIP_WORLDARD
	CHANGE_EQUIP,
#endif
	GROUND,					// NOTE: 2013년 2월5일 현재까지 unused.. 왜 있는거지???
	BELT_INVENTORY,			// NOTE: W2.1 버전에 새로 추가되는 벨트 슬롯 아이템이 제공하는 벨트 인벤토리
#ifdef ENABLE_IKASHOP_RENEWAL
	IKASHOP_OFFLINESHOP,
	IKASHOP_SAFEBOX,
	IKASHOP_AUCTION,
#endif
#ifdef ENABLE_AURA_SYSTEM
	AURA_REFINE,
#endif
	WINDOW_TYPE_MAX,
};

enum EDSInventoryMaxNum
{
	DS_INVENTORY_MAX_NUM = c_DragonSoul_Inventory_Count,
	DS_REFINE_WINDOW_MAX_NUM = 15,
};

#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
enum EAutoSlotMax
{
	AUTO_SKILL_SLOT_MAX = 12,
	AUTO_POTION_SLOT_START = AUTO_SKILL_SLOT_MAX + 1,
	AUTO_POTION_SLOT_MAX = AUTO_SKILL_SLOT_MAX + AUTO_POTION_SLOT_START,
};
#endif

#pragma pack (push, 1)
#define WORD_MAX 0xffff

#ifdef ENABLE_SWITCHBOT
enum ESwitchbotValues
{
	SWITCHBOT_SLOT_COUNT = 5,
	SWITCHBOT_ALTERNATIVE_COUNT = 5,
	MAX_NORM_ATTR_NUM = 5,
};

enum EAttributeSet
{
	ATTRIBUTE_SET_WEAPON,
	ATTRIBUTE_SET_BODY,
	ATTRIBUTE_SET_WRIST,
	ATTRIBUTE_SET_FOOTS,
	ATTRIBUTE_SET_NECK,
	ATTRIBUTE_SET_HEAD,
	ATTRIBUTE_SET_SHIELD,
	ATTRIBUTE_SET_EAR,
	ATTRIBUTE_SET_MAX_NUM,
};

#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
enum SwitchBotMaxNum
{
	SWITCHBOT_COUNT = 1,
};
#endif

typedef struct SItemPos
{
	BYTE window_type;
	WORD cell;
    SItemPos ()
    {
		window_type =     INVENTORY;
		cell = WORD_MAX;
    }
	SItemPos (BYTE _window_type, WORD _cell)
    {
        window_type = _window_type;
        cell = _cell;
    }

	// 기존에 cell의 형을 보면 BYTE가 대부분이지만, oi
	// 어떤 부분은 int, 어떤 부분은 WORD로 되어있어,
	// 가장 큰 자료형인 int로 받는다.
  //  int operator=(const int _cell)
  //  {
		//window_type = INVENTORY;
  //      cell = _cell;
  //      return cell;
  //  }
	bool IsValidCell()
	{
		switch (window_type)
		{
		case INVENTORY:
			return cell < c_Inventory_Count;
			break;
		case EQUIPMENT:
			return cell < c_DragonSoul_Equip_End;
			break;
		case DRAGON_SOUL_INVENTORY:
			return cell < (DS_INVENTORY_MAX_NUM);
			break;
			
#ifdef ENABLE_SWITCHBOT
		case SWITCHBOT:
			return cell < SWITCHBOT_SLOT_COUNT;
			break;
#endif
			
#ifdef ENABLE_SPECIAL_STORAGE
		case UPGRADE_INVENTORY:
			return cell < c_Special_ItemSlot_Count;
			break;
		case BOOK_INVENTORY:
			return cell < c_Special_ItemSlot_Count;
			break;
		case STONE_INVENTORY:
			return cell < c_Special_ItemSlot_Count;
			break;
		case CHANGE_INVENTORY:
			return cell < c_Special_ItemSlot_Count;
			break;
		case COSTUME_INVENTORY:
			return cell < c_Special_ItemSlot_Count;
			break;
		case CHEST_INVENTORY:
			return cell < c_Special_ItemSlot_Count;
			break;
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
		case SWITCHBOT:
			return cell < SWITCHBOT_COUNT;
			break;
#endif
#ifdef CHANGE_EQUIP_WORLDARD
		case CHANGE_EQUIP:
			return cell < CHANGE_EQUIP_SLOT_COUNT;
			break;
#endif
		default:
			return false;
		}
	}
	bool IsEquipCell()
	{
		switch (window_type)
		{
		case INVENTORY:
		case EQUIPMENT:
			return (c_Equipment_Start + c_Wear_Max > cell) && (c_Equipment_Start <= cell);
			break;

		case BELT_INVENTORY:
		case DRAGON_SOUL_INVENTORY:
#ifdef ENABLE_SPECIAL_STORAGE
		case UPGRADE_INVENTORY:
		case BOOK_INVENTORY:
		case STONE_INVENTORY:
		case CHANGE_INVENTORY:
		case COSTUME_INVENTORY:
		case CHEST_INVENTORY:
#endif

			return false;
			break;

		default:
			return false;
		}
	}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	bool IsBeltInventoryCell()
	{
		bool bResult = c_Belt_Inventory_Slot_Start <= cell && c_Belt_Inventory_Slot_End > cell;
		return bResult;
	}
#endif
	bool IsNPOS()
	{
		return (window_type == RESERVED_WINDOW && cell == WORD_MAX);
	}
	bool operator==(const struct SItemPos& rhs) const
	{
		return (window_type == rhs.window_type) && (cell == rhs.cell);
	}

	bool operator<(const struct SItemPos& rhs) const
	{
		return (window_type < rhs.window_type) || ((window_type == rhs.window_type) && (cell < rhs.cell));
	}
} TItemPos;
const TItemPos NPOS(RESERVED_WINDOW, WORD_MAX);

#pragma pack(pop)

const DWORD c_QuickBar_Line_Count = 3;
const DWORD c_QuickBar_Slot_Count = 12;

const float c_Idle_WaitTime = 5.0f;

const int c_Monster_Race_Start_Number = 6;
const int c_Monster_Model_Start_Number = 20001;

const float c_fAttack_Delay_Time = 0.2f;
const float c_fHit_Delay_Time = 0.1f;
const float c_fCrash_Wave_Time = 0.2f;
const float c_fCrash_Wave_Distance = 3.0f;

const float c_fHeight_Step_Distance = 50.0f;

enum
{
	DISTANCE_TYPE_FOUR_WAY,
	DISTANCE_TYPE_EIGHT_WAY,
	DISTANCE_TYPE_ONE_WAY,
	DISTANCE_TYPE_MAX_NUM,
};

const float c_fMagic_Script_Version = 1.0f;
const float c_fSkill_Script_Version = 1.0f;
const float c_fMagicSoundInformation_Version = 1.0f;
const float c_fBattleCommand_Script_Version = 1.0f;
const float c_fEmotionCommand_Script_Version = 1.0f;
const float c_fActive_Script_Version = 1.0f;
const float c_fPassive_Script_Version = 1.0f;

// Used by PushMove
const float c_fWalkDistance = 175.0f;
const float c_fRunDistance = 310.0f;

#define FILE_MAX_LEN 128

enum
{
	ITEM_SOCKET_SLOT_MAX_NUM = 8,
	// refactored attribute slot begin
	ITEM_ATTRIBUTE_SLOT_NORM_NUM	= 5,
	ITEM_ATTRIBUTE_SLOT_RARE_NUM	= 2,
	ITEM_ATTRIBUTE_SLOT_NORM_START	= 0,
	ITEM_ATTRIBUTE_SLOT_NORM_END	= ITEM_ATTRIBUTE_SLOT_NORM_START + ITEM_ATTRIBUTE_SLOT_NORM_NUM,

	ITEM_ATTRIBUTE_SLOT_RARE_START	= ITEM_ATTRIBUTE_SLOT_NORM_END,
	ITEM_ATTRIBUTE_SLOT_RARE_END	= ITEM_ATTRIBUTE_SLOT_RARE_START + ITEM_ATTRIBUTE_SLOT_RARE_NUM,

	ITEM_ATTRIBUTE_SLOT_MAX_NUM		= 15, // 7
#ifdef ENABLE_YOHARA_SYSTEM
	APPLY_RANDOM_SLOT_MAX_NUM = 3,
#endif
};

#pragma pack(push)
#pragma pack(1)

typedef struct SQuickSlot
{
	BYTE Type;
	WORD Position;
} TQuickSlot;

typedef struct TPlayerItemAttribute
{
    BYTE        bType;
    short       sValue;
} TPlayerItemAttribute;

#ifdef ENABLE_YOHARA_SYSTEM
typedef struct TPlayerItemApplyRandom
{
	BYTE bType;
	short sValue;
	BYTE row;
} TPlayerItemApplyRandom;
#endif

typedef struct packet_item
{
    DWORD       vnum;
    DWORD        count;
	DWORD		flags;
	DWORD		anti_flags;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
#ifdef ENABLE_SOULBIND_SYSTEM
	long	bind;
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char		name[CItemData::ITEM_NAME_MAX_LEN+1];
#endif
} TItemData;

#ifdef ENABLE_BUY_WITH_ITEM
typedef struct SShopItemPriceData
{
	DWORD		vnum;
	DWORD		count;
} TShopItemPriceData;
#endif

typedef struct packet_shop_item
{
	DWORD       vnum;
#ifdef ENABLE_FULL_YANG
	long long	price;			// PC 상점에만 이용
#else
	DWORD		price;
#endif
	DWORD        count;
#ifdef ENABLE_BUY_WITH_ITEM
	TShopItemPriceData	itemprice[MAX_SHOP_PRICES];
#endif
	BYTE		display_pos;

	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];

#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif

} TShopItemData;


#pragma pack(pop)

inline float GetSqrtDistance(int ix1, int iy1, int ix2, int iy2) // By sqrt
{
	float dx, dy;

	dx = float(ix1 - ix2);
	dy = float(iy1 - iy2);

	return sqrtf(dx*dx + dy*dy);
}

// DEFAULT_FONT
void DefaultFont_Startup();
void DefaultFont_Cleanup();
void DefaultFont_SetName(const char * c_szFontName);
CResource* DefaultFont_GetResource();
CResource* DefaultItalicFont_GetResource();
// END_OF_DEFAULT_FONT

void SetGuildSymbolPath(const char * c_szPathName);
const char * GetGuildSymbolFileName(DWORD dwGuildID);
BYTE SlotTypeToInvenType(BYTE bSlotType);

BYTE ApplyTypeToPointType(BYTE bType);
BYTE PointTypeToApplyType(BYTE bType);

#ifdef ENABLE_AURA_SYSTEM
int* GetAuraRefineInfo(BYTE bLevel);
#endif
