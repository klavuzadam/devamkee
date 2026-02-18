#pragma once

// NOTE : Item의 통합 관리 클래스다.
//        Icon, Model (droped on ground), Game Data

#include "../UserInterface/Locale_inc.h"
#include "../eterLib/GrpSubImage.h"
#include "../eterGrnLib/Thing.h"
#include "GameLibDefines.h"
#include <optional>

#include "../UserInterface/Locale_inc.h"

class CItemData
{
	public:
		enum
		{
			ITEM_NAME_MAX_LEN = 24,
			ITEM_LIMIT_MAX_NUM = 2,
			ITEM_VALUES_MAX_NUM = 6,
			ITEM_SMALL_DESCR_MAX_LEN = 256,
			ITEM_APPLY_MAX_NUM = 3,
			ITEM_SOCKET_MAX_NUM = 8,
#ifdef __UNIMPLEMENTED__
			ITEM_PICK_MAX_LEVEL = 9,
			ITEM_ROD_MAX_LEVEL = 20,
#endif
		};

#ifdef __UNIMPLEMENTED__
		enum EPetData
		{
			EGG_USE_SUCCESS = 0,
			EGG_USE_FAILED_TIMEOVER = 2,
			EGG_USE_FAILED_BECAUSE_NAME = 1,
			GROWTH_PET_ITEM_USE_COOL_TIME = 1,
			PET_EGG_USE_TRUE = 0,
			PET_EGG_USE_FAILED_BECAUSE_TRADING = 1,
			PET_EGG_USE_FAILED_BECAUSE_SHOP_OPEN = 2,
			PET_EGG_USE_FAILED_BECAUSE_MALL_OPEN = 3,
			PET_EGG_USE_FAILED_BECAUSE_SAFEBOX_OPEN = 4,
			PET_HATCHING_MONEY = 100000,
			PET_NAME_MAX_SIZE = 20,
			PET_NAME_MIN_SIZE = 4,
		};
#endif

#ifdef ENABLE_SPECIAL_COSTUME_ATTR
		enum ECostumeAttrLimit
		{
			SPECIAL_ATTR_COSTUME_BODY_LIMIT = 3,
			SPECIAL_ATTR_COSTUME_HAIR_LIMIT = 3,
			SPECIAL_ATTR_COSTUME_WEAPON_LIMIT = 3,
			SPECIAL_ATTR_COSTUME_MOUNT_SKIN_LIMIT = 3,
			
			SPECIAL_ATTR_COSTUME_SASH_SKIN_LIMIT=3,
			SPECIAL_ATTR_COSTUME_SHINING_LIMIT=3,
		};
#endif
		enum EItemType
		{
			ITEM_TYPE_NONE,					// 0
			ITEM_TYPE_WEAPON,				// 1
			ITEM_TYPE_ARMOR,				// 2
			ITEM_TYPE_USE,					// 3
			ITEM_TYPE_AUTOUSE,				// 4
			ITEM_TYPE_MATERIAL,				// 5
			ITEM_TYPE_SPECIAL,				// 6
			ITEM_TYPE_TOOL,					// 7
			ITEM_TYPE_LOTTERY,				// 8
			ITEM_TYPE_ELK,					// 9
			ITEM_TYPE_METIN,				// 10
			ITEM_TYPE_CONTAINER,			// 11
			ITEM_TYPE_FISH,					// 12
			ITEM_TYPE_ROD,					// 13
			ITEM_TYPE_RESOURCE,				// 14
			ITEM_TYPE_CAMPFIRE,				// 15
			ITEM_TYPE_UNIQUE,				// 16
			ITEM_TYPE_SKILLBOOK,			// 17
			ITEM_TYPE_QUEST,				// 18
			ITEM_TYPE_POLYMORPH,			// 19
			ITEM_TYPE_TREASURE_BOX,			// 20
			ITEM_TYPE_TREASURE_KEY,			// 21
			ITEM_TYPE_SKILLFORGET,			// 22
			ITEM_TYPE_GIFTBOX,				// 23
			ITEM_TYPE_PICK,					// 24
			ITEM_TYPE_HAIR,					// 25
			ITEM_TYPE_TOTEM,				// 26
			ITEM_TYPE_BLEND,				// 27
			ITEM_TYPE_COSTUME,				// 28
			ITEM_TYPE_DS,					// 29
			ITEM_TYPE_SPECIAL_DS,			// 30
			ITEM_TYPE_EXTRACT,				// 31
			ITEM_TYPE_SECONDARY_COIN,		// 32
			ITEM_TYPE_RING,					// 33
			ITEM_TYPE_BELT,					// 34
//#ifdef ENABLE_SHINING_SYSTEM
			ITEM_TYPE_SHINING,				// 35
//#endif
			ITEM_TYPE_RINGS,				// 36
//#ifdef ENABLE_NEW_PET_SYSTEM
			ITEM_TYPE_PET,					// 37
//#endif
//#ifdef __BUFFI_SUPPORT__
			ITEM_TYPE_BUFFI,				// 38
//#endif
//#ifdef ENABLE_NEWEQ
			ITEM_TYPE_NEWRING,				// 39
//#endif
			ITEM_TYPE_SPECIAL_RING,			// 40
			ITEM_TYPE_BOOSTER,				// 41
//#ifdef USE_CRYSTAL_SYSTEM
			ITEM_TYPE_CRYSTAL,				// 42
//#endif
			ITEM_TYPE_MAX_NUM,
		};

	enum EBoosterSubTypes
	{
		BOOSTER_HEAD,
		BOOSTER_ARMOR,
		BOOSTER_WEAPON,
	};

	enum ESpecialRingSubTypes
	{
		SPECIAL_RING_SLOT1,
		SPECIAL_RING_SLOT2,
	};
	
#ifdef ENABLE_NEW_PET_SYSTEM
	enum EPetSubTypes
	{
		PET_LEVELABLE,
		PET_EGG,
		PET_TRANSPORTBOX,
		PET_PROTEIN,
		PET_CHANGE_NAME_ITEM,
		PET_DEW,
		PET_SKILL,
		PET_PRIMIUM_FEEDSTUFF,
		PET_ATTR_CHANGE,
		PET_ATTR_DETERMINE,
		PET_EXPFOOD_PER,
		PET_SKILL_DEL_BOOK,
		PET_SKILL_ALL_DEL_BOOK,
		PET_EXPFOOD,
		PET_FEEDSTUFF,

	};
#endif

	enum ENewRingSubTypes
	{
		RING_SLOT1,
		RING_SLOT2,
		RING_SLOT3,
		RING_SLOT4,
		RING_SLOT5, //marriage 1
		RING_SLOT6,	//marriage 2
	};

		enum EResourceSubTypes
		{
			RESOURCE_FISHBONE,
			RESOURCE_WATERSTONEPIECE,
			RESOURCE_WATERSTONE,
			RESOURCE_BLOOD_PEARL,
			RESOURCE_BLUE_PEARL,
			RESOURCE_WHITE_PEARL,
			RESOURCE_BUCKET,
			RESOURCE_CRYSTAL,
			RESOURCE_GEM,
			RESOURCE_STONE,
			RESOURCE_METIN,
			RESOURCE_ORE,
#ifdef ENABLE_AURA_SYSTEM
			RESOURCE_AURA,
#endif
		};
		
		enum EWeddingItem
		{
			WEDDING_TUXEDO1 = 11901,
			WEDDING_TUXEDO2 = 11902,
			WEDDING_BRIDE_DRESS1 = 11903,
			WEDDING_BRIDE_DRESS2 = 11904,
			WEDDING_TUXEDO3 = 11911,
			WEDDING_TUXEDO4 = 11912,
			WEDDING_BRIDE_DRESS3 = 11913,
			WEDDING_BRIDE_DRESS4 = 11914,
			WEDDING_BOUQUET1 = 50201,
			WEDDING_BOUQUET2 = 50202,
		};

#ifdef __UNIMPLEMENTED__
		enum EPetSubTypes
		{
			PET_EGG = 0,
			PET_UPBRINGING = 1,
			PET_BAG = 2,
			PET_FEEDSTUFF = 3,
			PET_SKILL = 4,
			PET_SKILL_DEL_BOOK = 5,
		};

		enum EMediumSubTypes
		{
			MEDIUM_MOVE_COSTUME_ATTR = 0,
		};
#endif

		enum EWeaponSubTypes
		{
			WEAPON_SWORD, //0
			WEAPON_DAGGER, //1	//이도류
			WEAPON_BOW, //2
			WEAPON_TWO_HANDED, //3
			WEAPON_BELL, //4
			WEAPON_FAN, //5
			WEAPON_ARROW, //6
			WEAPON_MOUNT_SPEAR, //7
#ifdef ENABLE_WOLFMAN_CHARACTER
			WEAPON_CLAW = 8, //8
#endif
			WEAPON_QUIVER = 9, //9
			
#ifdef __UNIMPLEMENTED__
			WEAPON_BOUQUET = 10, //10
#endif
			WEAPON_NUM_TYPES, //11 2015/11/12

			WEAPON_NONE = WEAPON_NUM_TYPES+1,
		};

#ifdef __BUFFI_SUPPORT__
		enum EBuffiSubTypes
		{
			BUFFI_SCROLL,
			BUFFI_NAME_SCROLL,
		};
#endif

		enum EMaterialSubTypes
		{
			MATERIAL_LEATHER,
			MATERIAL_BLOOD,
			MATERIAL_ROOT,
			MATERIAL_NEEDLE,
			MATERIAL_JEWEL,
			MATERIAL_DS_REFINE_NORMAL,
			MATERIAL_DS_REFINE_BLESSED,
			MATERIAL_DS_REFINE_HOLLY,
		};

		enum EArmorSubTypes
		{
			ARMOR_BODY,
			ARMOR_HEAD,
			ARMOR_SHIELD,
			ARMOR_WRIST,
			ARMOR_FOOTS,
		    ARMOR_NECK,
			ARMOR_EAR,
#ifdef ENABLE_YOHARA_SYSTEM
			ARMOR_GLOVE,
#endif
			ARMOR_PENDANT_FIRE,
			ARMOR_PENDANT_ICE,
			ARMOR_PENDANT_THUNDER,
			ARMOR_PENDANT_WIND,
			ARMOR_PENDANT_EARTH,
			ARMOR_PENDANT_DARK,
			ARMOR_PENDANT_SOUL,

			ARMOR_NUM_TYPES
		};

		enum ECostumeSubTypes
		{
			COSTUME_BODY,				//0	갑옷(main look)
			COSTUME_HAIR,				//1	헤어(탈착가능)
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
			COSTUME_MOUNT	= 2,		//2
#endif
#ifdef ENABLE_SASH_SYSTEM
			COSTUME_SASH,
#endif

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
			COSTUME_WEAPON	= 4,		//4
#endif
#ifdef ENABLE_SASH_COSTUME_SYSTEM
			COSTUME_NEW_SASH	= 5,
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
			COSTUME_PET = 6,
#endif
#ifdef ENABLE_MOUNT_SKIN
			COSTUME_MOUNT_SKIN = 7,
#endif
#ifdef ENABLE_AURA_SYSTEM
			COSTUME_AURA = 8,
#endif
			COSTUME_NUM_TYPES,
		};

		enum EDragonSoulSubType
		{
			DS_SLOT1,
			DS_SLOT2,
			DS_SLOT3,
			DS_SLOT4,
			DS_SLOT5,
			DS_SLOT6,
			DS_SLOT_NUM_TYPES = 6,
		};

		enum EMetinSubTypes
		{
			METIN_NORMAL,
			METIN_GOLD,
		};

#ifdef ENABLE_SHINING_SYSTEM
		enum EShiningSubTypes
		{
			SHINING_WEAPON,
			SHINING_ARMOR,
			SHINING_SPECIAL,
		};
#endif

		enum ELimitTypes
		{
			LIMIT_NONE,
			LIMIT_LEVEL,
			LIMIT_STR,
			LIMIT_DEX,
			LIMIT_INT,
			LIMIT_CON,
			LIMIT_PCBANG,
			LIMIT_REAL_TIME,
			LIMIT_REAL_TIME_START_FIRST_USE,
			LIMIT_TIMER_BASED_ON_WEAR,
//#ifdef ENABLE_PITTY_REFINE
			LIMIT_PITTY_REFINE,
//#endif
//#ifdef ENABLE_YOHARA_SYSTEM
			LIMIT_CONQUEROR_LEVEL,
//#endif
			LIMIT_MAX_NUM
		};

		enum EPendantList
		{
			PENDANT_FIRE,
			PENDANT_ICE,
			PENDANT_EARTH,
			PENDANT_DARK,
			PENDANT_WIND,
			PENDANT_LIGHTNING,

			PENDANT_MAX_NUM,
		};

		enum EWearPositions
		{
			WEAR_BODY,          // 0
			WEAR_HEAD,          // 1
			WEAR_FOOTS,         // 2
			WEAR_WRIST,         // 3
			WEAR_WEAPON,        // 4
			WEAR_NECK,          // 5
			WEAR_EAR,           // 6
			WEAR_UNIQUE1,       // 7
			WEAR_UNIQUE2,       // 8
			WEAR_ARROW,         // 9
			WEAR_SHIELD,        // 10
			WEAR_ABILITY1,  // 11
			WEAR_ABILITY2,  // 12
			WEAR_ABILITY3,  // 13
			WEAR_ABILITY4,  // 14
			WEAR_ABILITY5,  // 15
			WEAR_ABILITY6,  // 16
			WEAR_ABILITY7,  // 17
			WEAR_ABILITY8,  // 18
			WEAR_COSTUME_BODY,	// 19
			WEAR_COSTUME_HAIR,	// 20

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
			WEAR_COSTUME_MOUNT,
#else
			WEAR_RING1,			// 21	: 신규 반지슬롯1 (왼쪽)
#endif
#ifdef ENABLE_SASH_SYSTEM
			WEAR_COSTUME_SASH,
#else
			WEAR_RING2,			// 22	: 신규 반지슬롯2 (오른쪽)
#endif

			WEAR_BELT,			// 23	: 신규 벨트슬롯

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
			WEAR_COSTUME_WEAPON,
#endif
			WEAR_RINGS_1,
			WEAR_RINGS_2,

#ifdef ENABLE_SASH_COSTUME_SYSTEM
			WEAR_COSTUME_NEW_SASH,  //29
#endif
#ifdef ENABLE_YOHARA_SYSTEM
			WEAR_GLOVE,
#endif

#ifdef ITEM_TALISMAN_EQUIPMENT
			WEAR_PENDANT,
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
			WEAR_OLD_PET,
			WEAR_PET,
#endif
#ifdef ENABLE_SHINING_SYSTEM
			WEAR_SHINING_WEAPON_1,
			WEAR_SHINING_ARMOR_1,
			WEAR_SHINING_SPECIAL,
#endif 
#ifdef ENABLE_MOUNT_SKIN
			WEAR_COSTUME_MOUNT_SKIN,//35
#endif
#ifdef __BUFFI_SUPPORT__
			WEAR_BUFFI_BODY,
			WEAR_BUFFI_HEAD,
			WEAR_BUFFI_WEAPON,
			WEAR_BUFFI_SASH,
#ifdef __BUFFI_SHINING__
			WEAR_BUFFI_SHINING_WEAPON,
			WEAR_BUFFI_SHINING_ARMOR,
#endif
			WEAR_BUFFI_RING_0,
			WEAR_BUFFI_RING_1,
			WEAR_BUFFI_RING_2,
#endif
			WEAR_RING_SLOT1,
			WEAR_RING_SLOT2,
			WEAR_RING_SLOT3,
			WEAR_RING_SLOT4,
			WEAR_RING_SLOT5,
			WEAR_RING_SLOT6,
			WEAR_RING_SLOT7,
			WEAR_COSTUME_AURA,
			WEAR_PENDANT_FIRE,
			WEAR_PENDANT_ICE,
			WEAR_PENDANT_THUNDER,
			WEAR_PENDANT_WIND,
			WEAR_PENDANT_DARK,
			WEAR_PENDANT_EARTH,
			WEAR_SPECIAL_RING1,
			WEAR_SPECIAL_RING2,
			WEAR_BOOST_ARMOR,
			WEAR_BOOST_HEAD,
			WEAR_BOOST_WEAPON,
			WEAR_PENDANT_SOUL_FIRST,
			WEAR_PENDANT_SOUL_SECOND,
			WEAR_MAX_NUM = 65,
		};


		enum EItemWearableFlag
		{
			WEARABLE_BODY       = (1 << 0),
			WEARABLE_HEAD       = (1 << 1),
			WEARABLE_FOOTS      = (1 << 2),
			WEARABLE_WRIST      = (1 << 3),
			WEARABLE_WEAPON     = (1 << 4),
			WEARABLE_NECK       = (1 << 5),
			WEARABLE_EAR        = (1 << 6),
			WEARABLE_UNIQUE     = (1 << 7),
			WEARABLE_SHIELD     = (1 << 8),
			WEARABLE_ARROW      = (1 << 9),
			WEARABLE_HAIR	= (1 << 10),
			WEARABLE_ABILITY		= (1 << 11),
			WEARABLE_COSTUME_BODY	= (1 << 12),
#ifdef ENABLE_SASH_SYSTEM
			WEARABLE_COSTUME_SASH = (1 << 14),
#endif
#ifdef ENABLE_SASH_COSTUME_SYSTEM
			WEARABLE_COSTUME_NEW_SASH	= (1 << 15),
#endif
#ifdef ENABLE_YOHARA_SYSTEM
			WEARABLE_GLOVE			= (1 << 16),
#endif
#ifdef ITEM_TALISMAN_EQUIPMENT
			WEARABLE_PENDANT		= (1 << 17),
#endif

		};

		enum EApplyTypes
		{
			APPLY_NONE,                 // 0
			APPLY_MAX_HP,               // 1
			APPLY_MAX_SP,               // 2
			APPLY_CON,                  // 3
			APPLY_INT,                  // 4
			APPLY_STR,                  // 5
			APPLY_DEX,                  // 6
			APPLY_ATT_SPEED,            // 7
			APPLY_MOV_SPEED,            // 8
			APPLY_CAST_SPEED,           // 9
			APPLY_HP_REGEN,             // 10
			APPLY_SP_REGEN,             // 11
			APPLY_POISON_PCT,           // 12
			APPLY_STUN_PCT,             // 13
			APPLY_SLOW_PCT,             // 14
			APPLY_CRITICAL_PCT,         // 15
			APPLY_PENETRATE_PCT,        // 16
			APPLY_ATTBONUS_HUMAN,       // 17
			APPLY_ATTBONUS_ANIMAL,      // 18
			APPLY_ATTBONUS_ORC,         // 19
			APPLY_ATTBONUS_MILGYO,      // 20
			APPLY_ATTBONUS_UNDEAD,      // 21
			APPLY_ATTBONUS_DEVIL,       // 22
			APPLY_STEAL_HP,             // 23
			APPLY_STEAL_SP,             // 24
			APPLY_MANA_BURN_PCT,        // 25
			APPLY_DAMAGE_SP_RECOVER,    // 26
			APPLY_BLOCK,                // 27
			APPLY_DODGE,                // 28
			APPLY_RESIST_SWORD,         // 29
			APPLY_RESIST_TWOHAND,       // 30
			APPLY_RESIST_DAGGER,        // 31
			APPLY_RESIST_BELL,          // 32
			APPLY_RESIST_FAN,           // 33
			APPLY_RESIST_BOW,           // 34
			APPLY_RESIST_FIRE,          // 35
			APPLY_RESIST_ELEC,          // 36
			APPLY_RESIST_MAGIC,         // 37
			APPLY_RESIST_WIND,          // 38
			APPLY_REFLECT_MELEE,        // 39
			APPLY_REFLECT_CURSE,        // 40
			APPLY_POISON_REDUCE,        // 41
			APPLY_KILL_SP_RECOVER,      // 42
			APPLY_EXP_DOUBLE_BONUS,     // 43
			APPLY_GOLD_DOUBLE_BONUS,    // 44
			APPLY_ITEM_DROP_BONUS,      // 45
			APPLY_POTION_BONUS,         // 46
			APPLY_KILL_HP_RECOVER,      // 47
			APPLY_IMMUNE_STUN,          // 48
			APPLY_IMMUNE_SLOW,          // 49
			APPLY_IMMUNE_FALL,          // 50
			APPLY_SKILL,                // 51
			APPLY_BOW_DISTANCE,         // 52
			APPLY_ATT_GRADE_BONUS,            // 53
			APPLY_DEF_GRADE_BONUS,            // 54
			APPLY_MAGIC_ATT_GRADE,      // 55
			APPLY_MAGIC_DEF_GRADE,      // 56
			APPLY_CURSE_PCT,            // 57
			APPLY_MAX_STAMINA,			// 58
			APPLY_ATT_BONUS_TO_WARRIOR, // 59
			APPLY_ATT_BONUS_TO_ASSASSIN,// 60
			APPLY_ATT_BONUS_TO_SURA,    // 61
			APPLY_ATT_BONUS_TO_SHAMAN,  // 62
			APPLY_ATT_BONUS_TO_MONSTER, // 63
			APPLY_MALL_ATTBONUS,        // 64 공격력 +x%
			APPLY_MALL_DEFBONUS,        // 65 방어력 +x%
			APPLY_MALL_EXPBONUS,        // 66 경험치 +x%
			APPLY_MALL_ITEMBONUS,       // 67 아이템 드롭율 x/10배
			APPLY_MALL_GOLDBONUS,       // 68 돈 드롭율 x/10배
			APPLY_MAX_HP_PCT,           // 69 최대 생명력 +x%
			APPLY_MAX_SP_PCT,           // 70 최대 정신력 +x%
			APPLY_SKILL_DAMAGE_BONUS,   // 71 스킬 데미지 * (100+x)%
			APPLY_NORMAL_HIT_DAMAGE_BONUS,      // 72 평타 데미지 * (100+x)%
			APPLY_SKILL_DEFEND_BONUS,   // 73 스킬 데미지 방어 * (100-x)%
			APPLY_NORMAL_HIT_DEFEND_BONUS,      // 74 평타 데미지 방어 * (100-x)%
			APPLY_PC_BANG_EXP_BONUS,		//75
			APPLY_PC_BANG_DROP_BONUS,		//76
			APPLY_EXTRACT_HP_PCT,			//77
			APPLY_RESIST_WARRIOR,			//78
			APPLY_RESIST_ASSASSIN ,			//79
			APPLY_RESIST_SURA,				//80
			APPLY_RESIST_SHAMAN,			//81
			APPLY_ENERGY,					//82
			APPLY_DEF_GRADE,				// 83 방어력. DEF_GRADE_BONUS는 클라에서 두배로 보여지는 의도된 버그(...)가 있다.
			APPLY_COSTUME_ATTR_BONUS,		// 84 코스튬 아이템에 붙은 속성치 보너스
			APPLY_MAGIC_ATTBONUS_PER,		// 85 마법 공격력 +x%
			APPLY_MELEE_MAGIC_ATTBONUS_PER,			// 86 마법 + 밀리 공격력 +x%

			APPLY_RESIST_ICE,		// 87 냉기 저항
			APPLY_RESIST_EARTH,		// 88 대지 저항
			APPLY_RESIST_DARK,		// 89 어둠 저항

			APPLY_ANTI_CRITICAL_PCT,	//90 크리티컬 저항
			APPLY_ANTI_PENETRATE_PCT,	//91 관통타격 저항

// #ifdef ENABLE_WOLFMAN_CHARACTER
			APPLY_BLEEDING_REDUCE			= 92,	//92
			APPLY_BLEEDING_PCT				= 93,	//93
			APPLY_ATT_BONUS_TO_WOLFMAN		= 94,	//94
			APPLY_RESIST_WOLFMAN			= 95,	//95
			APPLY_RESIST_CLAW				= 96,	//96
// #endif

// #ifdef ENABLE_ACCE_COSTUME_SYSTEM
			APPLY_ACCEDRAIN_RATE			= 97,	//97
// #endif

// #ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
			APPLY_RESIST_MAGIC_REDUCTION	= 98,	//98
// #endif

// #ifdef ENABLE_NEW_BONUS_WORLDARD
			APPLY_RESIST_HUMAN				= 99,
// #endif	

// #ifdef ELEMENT_NEW_BONUSES
			APPLY_ATTBONUS_ELEC 	 = 100,
			APPLY_ATTBONUS_FIRE 	 = 101,
			APPLY_ATTBONUS_ICE	 	= 102,
			APPLY_ATTBONUS_WIND  	= 103,
			APPLY_ATTBONUS_EARTH 	= 104,
			APPLY_ATTBONUS_DARK  	= 105,
// #endif
// #ifdef ENABLE_YOHARA_SYSTEM
			APPLY_RANDOM = 118,
			APPLY_SUNGMA_STR = 119,
			APPLY_SUNGMA_HP = 120,
			APPLY_SUNGMA_MOVE = 121,
			APPLY_SUNGMA_IMMUNE = 122,
// #endif
// #ifdef ELEMENT_NEW_BONUSES
			APPLY_ENCHANT_ELEC = 123,
			APPLY_ENCHANT_FIRE = 124,
			APPLY_ENCHANT_ICE = 125,
			APPLY_ENCHANT_WIND  = 126,
			APPLY_ENCHANT_EARTH = 127,
			APPLY_ENCHANT_DARK = 128,
			APPLY_ATTBONUS_SWORD = 129,		
			APPLY_ATTBONUS_TWOHAND = 130,	
			APPLY_ATTBONUS_DAGGER = 131,	
			APPLY_ATTBONUS_BELL = 132,		
			APPLY_ATTBONUS_FAN = 133,		
			APPLY_ATTBONUS_BOW = 134,

			APPLY_ATTBONUS_ZODIAC = 135,
			APPLY_ATTBONUS_DESERT = 136,
			APPLY_ATTBONUS_INSECT = 137,	
// #ifdef ENABLE_WOLFMAN_CHARACTER
			APPLY_ATTBONUS_CLAW = 138,
// #endif	
// #endif
// #ifdef NEW_BONUS
			APPLY_ATTBONUS_STONE = 139,
			APPLY_ATTBONUS_BOSS = 140,
// #endif
// #ifdef ENABLE_YOHARA_SYSTEM
			APPLY_HIT_PCT = 141,
			APPLY_RESIST_MOUNT_FALL = 142,
			APPLY_MONSTER_DEFEND_BONUS = 143,
// #endif
// #ifdef USE_DROP_AFFECT_BONUSES
			APPLY_DOUBLE_DROP_METIN = 144,
			APPLY_DOUBLE_DROP_BOSS = 145,
// #endif
			MAX_APPLY_NUM			= 150,
		};

		enum EImmuneFlags
		{
			IMMUNE_PARA         = (1 << 0),
			IMMUNE_CURSE        = (1 << 1),
			IMMUNE_STUN         = (1 << 2),
			IMMUNE_SLEEP        = (1 << 3),
			IMMUNE_SLOW         = (1 << 4),
			IMMUNE_POISON       = (1 << 5),
			IMMUNE_TERROR       = (1 << 6),
		};

#pragma pack(push)
#pragma pack(1)
		typedef struct SItemLimit
		{
			BYTE        bType;
			long        lValue;
		} TItemLimit;

		typedef struct SItemApply
		{
			BYTE        bType;
			long        lValue;
		} TItemApply;

		typedef struct SItemTable
		{
			DWORD		dwVnum;
			DWORD		dwVnumRange;

			char		szName[ITEM_NAME_MAX_LEN + 1];
			char		szLocaleName[ITEM_NAME_MAX_LEN + 1];

			BYTE		bType;
			BYTE		bSubType;

			BYTE		bWeight;
			BYTE		bSize;

			DWORD		dwAntiFlags;
			DWORD		dwFlags;
			DWORD		dwWearFlags;
			DWORD		dwImmuneFlag;

			long long	dwIBuyItemPrice;
			long long	dwISellItemPrice;

			TItemLimit	aLimits[ITEM_LIMIT_MAX_NUM];
			TItemApply	aApplies[ITEM_APPLY_MAX_NUM];
			long		alValues[ITEM_VALUES_MAX_NUM];
			long		alSockets[ITEM_SOCKET_MAX_NUM];

			DWORD		dwRefinedVnum;
			WORD		wRefineSet;
			BYTE		bAlterToMagicItemPct;
			BYTE		bSpecular;
			BYTE		bGainSocketPct;
		} TItemTable;

#ifdef ENABLE_SASH_SYSTEM
		struct SScaleInfo
		{
			float	fScaleX, fScaleY, fScaleZ;
			float	fPositionX, fPositionY, fPositionZ;
		};

		typedef struct SScaleTable
		{
			SScaleInfo	tInfo[10];
		} TScaleTable;
#endif

#pragma pack(pop)

	public:
		CItemData();
		virtual ~CItemData();

		void Clear();
		void SetSummary(const std::string& c_rstSumm);
		void SetDescription(const std::string& c_rstDesc);

		CGraphicThing * GetModelThing();
		CGraphicThing * GetSubModelThing();
		CGraphicThing * GetDropModelThing();
		CGraphicSubImage * GetIconImage();

		DWORD GetLODModelThingCount();
		BOOL GetLODModelThingPointer(DWORD dwIndex, CGraphicThing ** ppModelThing);

		DWORD GetAttachingDataCount();
		BOOL GetCollisionDataPointer(DWORD dwIndex, const NRaceData::TAttachingData ** c_ppAttachingData);
		BOOL GetAttachingDataPointer(DWORD dwIndex, const NRaceData::TAttachingData ** c_ppAttachingData);

		/////
		const TItemTable*	GetTable() const;
		DWORD GetIndex() const;
		const char * GetName() const;
		const char * GetDescription() const;
		const char * GetSummary() const;
		BYTE GetType() const;
		BYTE GetSubType() const;
		UINT GetRefine() const;
		const char* GetUseTypeString() const;
		DWORD GetWeaponType() const;
		BYTE GetSize() const;
		BOOL IsAntiFlag(DWORD dwFlag) const;
		BOOL IsFlag(DWORD dwFlag) const;
		BOOL IsWearableFlag(DWORD dwFlag) const;
		BOOL HasNextGrade() const;
#if defined(ENABLE_WIKI) || defined(INSIDE_RENDER)
		WORD GetRefinedSet() const;
		WORD GetRefinedVnum() const;
#endif
		DWORD GetWearFlags() const;
		long long GetIBuyItemPrice() const;
		long long GetISellItemPrice() const;
		BOOL GetLimit(BYTE byIndex, TItemLimit * pItemLimit) const;
		BOOL GetApply(BYTE byIndex, TItemApply * pItemApply) const;
		long GetValue(BYTE byIndex) const;
		long GetSocket(BYTE byIndex) const;
		long SetSocket(BYTE byIndex,DWORD value);
		int GetSocketCount() const;
		DWORD GetIconNumber() const;

		UINT	GetSpecularPoweru() const;
		float	GetSpecularPowerf() const;

		/////

		BOOL IsEquipment() const;

		/////

		//BOOL LoadItemData(const char * c_szFileName);
		void SetDefaultItemData(const char * c_szIconFileName, const char * c_szModelFileName  = NULL);
		void SetItemTableData(TItemTable * pItemTable);
		#ifdef ENABLE_SASH_SYSTEM
		void	SetItemScale(const std::string strJob, const std::string strSex, const std::string strScaleX, const std::string strScaleY, const std::string strScaleZ, const std::string strPositionX, const std::string strPositionY, const std::string strPositionZ);
		bool	GetItemScale(DWORD dwPos, float & fScaleX, float & fScaleY, float & fScaleZ, float & fPositionX, float & fPositionY, float & fPositionZ);
		#endif
	protected:
		void __LoadFiles();
		void __SetIconImage(const char * c_szFileName);

	protected:
		std::string m_strModelFileName;
		std::string m_strSubModelFileName;
		std::string m_strDropModelFileName;
		std::string m_strIconFileName;
		std::string m_strDescription;
		std::string m_strSummary;
		std::vector<std::string> m_strLODModelFileNameVector;

		CGraphicThing * m_pModelThing;
		CGraphicThing * m_pSubModelThing;
		CGraphicThing * m_pDropModelThing;
		CGraphicSubImage * m_pIconImage;
		std::vector<CGraphicThing *> m_pLODModelThingVector;

		NRaceData::TAttachingDataVector m_AttachingDataVector;
		DWORD		m_dwVnum;
		TItemTable m_ItemTable;
		#ifdef ENABLE_SASH_SYSTEM
		TScaleTable	m_ScaleTable;
		#endif
	public:
		static void DestroySystem();

		static CItemData* New();
		static void Delete(CItemData* pkItemData);

		static CDynamicPool<CItemData>		ms_kPool;
#ifdef ENABLE_AURA_SYSTEM
	public:
		enum EAuraGradeType
		{
			AURA_GRADE_NONE,
			AURA_GRADE_ORDINARY,
			AURA_GRADE_SIMPLE,
			AURA_GRADE_NOBLE,
			AURA_GRADE_SPARKLING,
			AURA_GRADE_MAGNIFICENT,
			AURA_GRADE_RADIANT,
			AURA_GRADE_MAX_NUM,
		};
		enum EAuraItem
		{
			AURA_BOOST_ITEM_VNUM_BASE = 49980
		};
		enum EAuraBoostIndex
		{
			ITEM_AURA_BOOST_ERASER,
			ITEM_AURA_BOOST_WEAK,
			ITEM_AURA_BOOST_NORMAL,
			ITEM_AURA_BOOST_STRONG,
			ITEM_AURA_BOOST_ULTIMATE,
			ITEM_AURA_BOOST_MAX,
		};

	protected:
		typedef struct SAuraScaleTable
		{
			D3DXVECTOR3 v3MeshScale[NRaceData::SEX_MAX_NUM][NRaceData::JOB_MAX_NUM];
			float fParticleScale[NRaceData::SEX_MAX_NUM][NRaceData::JOB_MAX_NUM];
		} TAuraScaleTable;

		TAuraScaleTable m_AuraScaleTable;
		DWORD m_dwAuraEffectID;

	public:
		void SetAuraScaleTableData(BYTE byJob, BYTE bySex, float fMeshScaleX, float fMeshScaleY, float fMeshScaleZ, float fParticleScale);
		D3DXVECTOR3& GetAuraMeshScaleVector(BYTE byJob, BYTE bySex);
		float GetAuraParticleScale(BYTE byJob, BYTE bySex);

		void SetAuraEffectID(const char* szAuraEffectPath);
		DWORD GetAuraEffectID() const { return m_dwAuraEffectID; }
#endif

};
