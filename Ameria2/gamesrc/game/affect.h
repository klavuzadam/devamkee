#ifndef __INC_AFFECT_H
#define __INC_AFFECT_H

class CAffect
{
	public:
		DWORD	dwType;
		BYTE	bApplyOn;
		long	lApplyValue;
		DWORD	dwFlag;
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
		DWORD	dwFlag2;
//#endif
		long	lDuration;
		long	lSPCost;

		static CAffect* Acquire();
		static void Release(CAffect* p);
};

enum EAffectTypes
{
	AFFECT_NONE,

	AFFECT_MOV_SPEED		= 200,
	AFFECT_ATT_SPEED,
	AFFECT_ATT_GRADE,
	AFFECT_INVISIBILITY,
	AFFECT_STR,
	AFFECT_DEX,			// 205
	AFFECT_CON,
	AFFECT_INT,
	AFFECT_FISH_MIND_PILL,

	AFFECT_POISON,
	AFFECT_STUN,		// 210
	AFFECT_SLOW,
	AFFECT_DUNGEON_READY,
	AFFECT_DUNGEON_UNIQUE,

	AFFECT_BUILDING,
	AFFECT_REVIVE_INVISIBLE,	// 215
	AFFECT_FIRE,
	AFFECT_CAST_SPEED,
	AFFECT_HP_RECOVER_CONTINUE,
	AFFECT_SP_RECOVER_CONTINUE,

	AFFECT_POLYMORPH,		// 220
	AFFECT_MOUNT,

	AFFECT_WAR_FLAG,		// 222

	AFFECT_BLOCK_CHAT,		// 223
	AFFECT_CHINA_FIREWORK,

	AFFECT_BOW_DISTANCE,	// 225
	AFFECT_DEF_GRADE,		// 226
#ifdef ENABLE_WOLFMAN_CHARACTER
	AFFECT_BLEEDING,		// 227
#endif
#ifdef __NEWPET_SYSTEM__
	AFFECT_PET,				// 228
#endif
	AFFECT_PREMIUM_START	= 500,
	AFFECT_EXP_BONUS		= 500,	// Â°Ã¦ÃÃ¨ÃÃ Â¹ÃÃÃ¶
	AFFECT_ITEM_BONUS		= 501,	// ÂµÂµÂµÃÃÃ ÃÃ¥Â°Â©
	AFFECT_SAFEBOX		= 502,  // PREMIUM_SAFEBOX,
	AFFECT_AUTOLOOT		= 503,	// PREMIUM_AUTOLOOT,
	AFFECT_FISH_MIND		= 504,	// PREMIUM_FISH_MIND,
	AFFECT_MARRIAGE_FAST	= 505,	// Â¿Ã¸Â¾ÃÃÃ Â±ÃªÃÃ
	AFFECT_GOLD_BONUS		= 506,	// ÂµÂ· ÂµÃ¥Â·ÃÃÂ®Â·Ã¼ 50%ÃÃµÂ°Â¡
	AFFECT_PREMIUM_END		= 509,

	AFFECT_MALL			= 510,	// Â¸Ã´ Â¾ÃÃÃÃÃ Â¿Â¡ÃÃ¥ÃÂ®
	AFFECT_NO_DEATH_PENALTY	= 511,	// Â¿Ã«Â½ÃÃÃ Â°Â¡ÃÂ£ (Â°Ã¦ÃÃ¨ÃÂ¡Â°Â¡ ÃÃÂ³ÃÃÂ¼Â¸Â¦ ÃÃÂ¹Ã¸ Â¸Â·Â¾ÃÃÃÂ´Ã)
	AFFECT_SKILL_BOOK_BONUS	= 512,	// Â¼Â±ÃÃÃÃ Â±Â³ÃÃ (ÃÂ¥ Â¼Ã¶Â·Ã Â¼ÂºÂ°Ã¸ ÃÂ®Â·Ã¼ÃÃ 50% ÃÃµÂ°Â¡)
	AFFECT_SKILL_NO_BOOK_DELAY	= 513,	// ÃÃÂ¾ÃÂ¼ÃºÂ¼Â­

	AFFECT_HAIR	= 514,	// ÃÃ¬Â¾Ã® ÃÂ¿Â°Ãº
	AFFECT_COLLECT = 515, //Â¼Ã¶ÃÃ½ÃÃ¹Â½ÂºÃÂ®
	AFFECT_EXP_BONUS_EURO_FREE = 516, // Â°Ã¦ÃÃ¨ÃÃ Â¹ÃÃÃ¶ (ÃÂ¯Â·Â´ Â¹Ã¶ÃÃ¼ 14 Â·Â¹ÂºÂ§ ÃÃÃÃ Â±Ã¢ÂºÂ» ÃÂ¿Â°Ãº)
	AFFECT_EXP_BONUS_EURO_FREE_UNDER_15 = 517,
	AFFECT_UNIQUE_ABILITY = 518,

	AFFECT_CUBE_1,
	AFFECT_CUBE_2,
	AFFECT_CUBE_3,
	AFFECT_CUBE_4,
	AFFECT_CUBE_5,
	AFFECT_CUBE_6,
	AFFECT_CUBE_7,
	AFFECT_CUBE_8,
	AFFECT_CUBE_9,
	AFFECT_CUBE_10,
	AFFECT_CUBE_11,
	AFFECT_CUBE_12,

	AFFECT_BLEND,

	AFFECT_HORSE_NAME,
	AFFECT_MOUNT_BONUS,

	AFFECT_AUTO_HP_RECOVERY = 534,
	AFFECT_AUTO_SP_RECOVERY = 535,


	AFFECT_DRAGON_SOUL_QUALIFIED = 540,
	AFFECT_DRAGON_SOUL_DECK_0 = 541,
	AFFECT_DRAGON_SOUL_DECK_1 = 542,

#ifdef ENABLE_BIOLOG_SYSTEM
	AFF_BIO_1 = 543,	AFF_BIO_2 = 544,	AFF_BIO_3 = 545,	AFF_BIO_4 = 546,
	AFF_BIO_5 = 547,	AFF_BIO_6 = 548,	AFF_BIO_7 = 549,	AFF_BIO_8 = 550,
	AFF_BIO_9 = 551,	AFF_BIO_10 = 552,	AFF_BIO_11 = 553,	AFF_BIO_12 = 554,
	AFF_BIO_13 = 555,	AFF_BIO_14 = 556,	AFF_BIO_15 = 557,	AFF_BIO_16 = 558,
	AFF_BIO_17 = 559,	AFF_BIO_18 = 560,	AFF_BIO_19 = 561,	AFF_BIO_20 = 562,
#endif
#ifdef __NEWPET_SYSTEM__
	AFFECT_NEW_PET = 563,
#endif
#ifdef USE_BOOST_POTIONS
	AFFECT_BOOST_POTION1 = 564,
	AFFECT_BOOST_POTION2 = 565,
	AFFECT_BOOST_POTION3 = 566,
#endif
#ifdef USE_PASSIVE_ABILITY_TAMER
	AFFECT_SKILL_TAMER_BOOK_BONUS = 581,
	AFFECT_SKILL_TAMER_NO_BOOK_DELAY = 582,
#endif
#ifdef USE_CRYSTAL_SYSTEM
	AFFECT_BOOST_CRYSTAL = 583,
#endif
	AFFECT_RAMADAN_ABILITY = 300,
	AFFECT_RAMADAN_RING	   = 301,

	AFFECT_NOG_ABILITY = 302,
	

	AFFECT_HOLLY_STONE_POWER = 303,

#if defined(WJ_COMBAT_ZONE)	
	AFFECT_COMBAT_ZONE_POTION = 585,
	AFFECT_COMBAT_ZONE_DEFENSE = 586,
	AFFECT_COMBAT_ZONE_MOVEMENT = 587,
	AFFECT_COMBAT_ZONE_MOB = 588,
#endif

#ifdef BATTLEPASS_WORLDARD
	AFFECT_BATTLE_PASS = 600,
#endif

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	AFFECT_SWITCHBOT = 605,
#endif

#if defined(__BL_BATTLE_ROYALE__)
	AFFECT_BATTLE_ROYALE_CROWN = 688,
	AFFECT_BATTLE_ROYALE_FIELD_DAMAGE = 689,
	AFFECT_BATTLE_ROYALE_SLOW = 690,
	AFFECT_BATTLE_ROYALE_MOVE_SPEED = 691,
	AFFECT_BATTLE_ROYALE_INFINITE_STAMINA = 692,
#endif

#ifdef ENABLE_YOHARA_SYSTEM
	AFFECT_SUNGMA_STR	= 907,
	AFFECT_SUNGMA_HP	= 908,
	AFFECT_SUNGMA_MOVE	= 909,
	AFFECT_SUNGMA_INMUNE	= 910,
#endif
#ifdef ENABLE_MELEY_LAIR_DUNGEON
	AFFECT_STATUE = 700,
#endif
#ifdef ENABLE_MULTI_FARM_BLOCK
	AFFECT_MULTI_FARM_PREMIUM = 704,
#endif
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	AFFECT_AUTO_PICK_UP = 705,
#endif
#ifdef ENABLE_DS_SET
	AFFECT_DS_SET = 706,
	AFFECT_DS_BNS1 = 707,
	AFFECT_DS_BNS2 = 708,
	AFFECT_DS_BNS3 = 709,
#endif

	AFFECT_THIEF = 710,
	AFFECT_EXP = 711,
#ifdef USE_DROP_AFFECT_BONUSES
	AFFECT_DROP_GREEN = 712,
	AFFECT_DROP_BLUE = 713,
#endif
#ifdef USE_PREMIUM_AFFECT
	AFFECT_PREMIUM_USER = 714,
#endif
#ifdef ENABLE_VOTE4BUFF
	AFFECT_VOTE4BUFF = 809,
#endif
#ifdef __RENEWAL_BRAVE_CAPE__
	AFFECT_BRAVE_CAPE = 813,
#endif
#ifdef ENABLE_QUEEN_NETHIS
	AFFECT_SUNGMA_MOVE_DEBUFF = 815,
	AFFECT_SUNGMA_STR_DEBUFF = 816,
#endif
// #if defined(__AUTOMATIC_HUNTING_SYSTEM__) && !defined(USE_PREMIUM_AFFECT)
	AFFECT_AUTO_HUNT = 840,
// #endif
// #if defined(__AUTOMATIC_HUNTING_SYSTEM__)
	AFFECT_AUTO_USE = 841,
// #endif
#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_ULTIMATE
	AFFECT_SEARCH_SHOP_PREMIUM = 550,
#endif
#endif

#ifdef VOTE_BUFF_RENEWAL
	AFFECT_VOTEFORBONUS,
#endif
#ifdef ENABLE_SUNG_MAHI_TOWER
	AFFECT_SUNG_MAHI_BP,
	AFFECT_SUNG_MAHI_CURSE,
#endif
	AFFECT_NAME_PET,
	AFFECT_NAME_MOUNT,
	AFFECT_NAME_BUFFI,

#ifdef __LEADERSHIP__BONUS__
	AFF_LEADERSHIP = 901,
#endif
	AFFECT_QUEST_START_IDX = 1000,

};

enum EAffectBits
{
	AFF_NONE,

	AFF_YMIR,
	AFF_INVISIBILITY,
	AFF_SPAWN,

	AFF_POISON,
	AFF_SLOW,
	AFF_STUN,

	AFF_DUNGEON_READY,		// Â´Ã¸ÃÃ¼Â¿Â¡Â¼Â­ ÃÃÂºÃ± Â»Ã³ÃÃ
	AFF_DUNGEON_UNIQUE,		// Â´Ã¸ÃÃ¼ ÃÂ¯Â´ÃÃÂ© (ÃÂ¬Â¶Ã³ÃÃÂ¾Ã°ÃÂ®Â¿Â¡Â¼Â­ ÃÃÂ¸ÂµÂµÃÃÃ¶Â¾ÃÃÂ½)

	AFF_BUILDING_CONSTRUCTION_SMALL,
	AFF_BUILDING_CONSTRUCTION_LARGE,
	AFF_BUILDING_UPGRADE,

	AFF_MOV_SPEED_POTION,
	AFF_ATT_SPEED_POTION,

	AFF_FISH_MIND,

	AFF_JEONGWIHON,		// ÃÃ¼Â±ÃÃÂ¥
	AFF_GEOMGYEONG,		// Â°ÃÂ°Ã¦
	AFF_CHEONGEUN,		// ÃÂµÂ±ÃÃÃ
	AFF_GYEONGGONG,		// Â°Ã¦Â°Ã¸Â¼Ãº
	AFF_EUNHYUNG,		// ÃÂºÃÃ¼Â¹Ã½
	AFF_GWIGUM,			// Â±ÃÂ°Ã
	AFF_TERROR,			// Â°Ã¸ÃÃ·
	AFF_JUMAGAP,		// ÃÃÂ¸Â¶Â°Â©
	AFF_HOSIN,			// ÃÂ£Â½Ã
	AFF_BOHO,			// ÂºÂ¸ÃÂ£
	AFF_KWAESOK,		// ÃÃ¨Â¼Ã
	AFF_MANASHIELD,		// Â¸Â¶Â³ÂªÂ½Â¯ÂµÃ¥
	AFF_MUYEONG,		// Â¹Â«Â¿ÂµÃÃ¸ affect
	AFF_REVIVE_INVISIBLE,	// ÂºÃÃÂ°Â½Ã ÃÃ¡Â½ÃÂµÂ¿Â¾Ã Â¹Â«ÃÃ»
	AFF_FIRE,			// ÃÃ¶Â¼Ã ÂºÃ ÂµÂ¥Â¹ÃÃÃ¶
	AFF_GICHEON,		// Â±Ã¢ÃÂµÂ´Ã«Â°Ã¸
	AFF_JEUNGRYEOK,		// ÃÃµÂ·ÃÂ¼Ãº
	AFF_TANHWAN_DASH,		// ÃÂºÃÂ¯Â°ÃÂ¿Ã« Â´ÃÂ¸Â®Â±Ã¢Â¾Ã®ÃÃ¥ÃÂ®
	AFF_PABEOP,			// ÃÃÂ¹Ã½Â¼Ãº
	AFF_CHEONGEUN_WITH_FALL,	// ÃÂµÂ±ÃÃÃ
	AFF_POLYMORPH,
	AFF_WAR_FLAG1,
	AFF_WAR_FLAG2,
	AFF_WAR_FLAG3,

	AFF_CHINA_FIREWORK,
// #ifdef __BUFFI_SUPPORT__
	AFF_BUFFI_HOSIN,
	AFF_BUFFI_GICHEON,
	AFF_BUFFI_KWAESOK,
	AFF_BUFFI_JEUNGRYEOK,
// #endif
	AFF_HAIR,	// ÃÃ¬Â¾Ã®
	AFF_GERMANY, // ÂµÂ¶ÃÃ
	AFF_RAMADAN_RING,

// #ifdef ENABLE_WOLFMAN_CHARACTER
	AFF_BLEEDING,			// 42
	AFF_RED_POSSESSION,		// 44
	AFF_BLUE_POSSESSION,	// 43
// #endif
// #ifdef ENABLE_YOHARA_SYSTEM
	AFF_CHEONUN,
	AFF_CHUNWOON_MOOJUK,
// #endif
// #ifdef ENABLE_MELEY_LAIR_DUNGEON
	AFF_STATUE1,
	AFF_STATUE2,
	AFF_STATUE3,
	AFF_STATUE4,
// #endif
// #ifdef ENABLE_PERFECT_BUFF
	AFF_HOSIN_PERFECT,							// 55
	AFF_BOHO_PERFECT,                           // 56
	AFF_KWAESOK_PERFECT,                        // 57
	AFF_GICHEON_PERFECT,                        // 58
	AFF_JEUNGRYEOK_PERFECT,                     // 59
// #endif
// #if defined(__AUTOMATIC_HUNTING_SYSTEM__) && !defined(USE_PREMIUM_AFFECT)
	AFFECT_AUTO,
// #endif
#if defined(__BL_BATTLE_ROYALE__)
	AFF_BATTLE_ROYALE_CROWN,
	AFF_BATTLE_ROYALE_SLOW_1,
	AFF_BATTLE_ROYALE_SLOW_2,
	AFF_BATTLE_ROYALE_SLOW_3,
#endif

	AFF_BITS_MAX
};

extern void SendAffectAddPacket(LPDESC d, CAffect * pkAff);

// AFFECT_DURATION_BUG_FIX
enum AffectVariable
{
	// AffectÂ°Â¡ Â¹Â«ÃÃÂ´Ã«Â·Ã ÂµÃ©Â¾Ã®Â°Â¡ ÃÃÂ¾Ã®Â¾Ã ÃÃ Â°Ã¦Â¿Ã¬ Â»Ã§Â¿Ã«.
	// Â½ÃÂ°Â£ÃÂ» Â°Ã¨Â¼Ã ÃÃÃÃÂ±Ã¢ Â¶Â§Â¹Â®Â¿Â¡ Â¸ÃÂ¿Ã¬ ÃÂ«Â°ÂªÃÂ¸Â·Ã Â¹Â«ÃÃÂ´Ã«Â¸Â¦ Â¿Â¡Â¹ÃÂ·Â¹ÃÃÂ¼ÃÃÃ.
	//// 24ÂºÃ±ÃÂ®Â´Ã ÃÃ»ÃÂ¸Â¹ÃÂ·Ã 25ÂºÃ±ÃÂ®Â¸Â¦ Â»Ã§Â¿Ã«.
	// ... 25ÂºÃ±ÃÂ® Â»Ã§Â¿Ã«ÃÃÂ´ÃÂ°Ã­ ÃÃÂ³ÃµÂ°Ã­Â¼Â± 29bit Â»Ã§Â¿Ã«ÃÃÂ°Ã­ ÃÃÂ´Ã Â¾Ã¶ÃÂ»Â³Â­ ÃÃÂ¼Â®ÃÃÂ¶Ãµ...
	// collect questÂ¿Â¡Â¼Â­ Â¹Â«ÃÃ Â½ÃÂ°Â£ÃÂ» 60Â³Ã¢ÃÂ¸Â·Ã Â»Ã§Â¿Ã«ÃÃÂ°Ã­ ÃÃÃÂ¸Â¹ÃÂ·Ã, Â¿Â©Â±Ã¢ÂµÂµ 60Â³Ã¢ÃÂ¸Â·Ã ÃÃÃÃ.

	INFINITE_AFFECT_DURATION = 60 * 365 * 24 * 60 * 60
};
// END_AFFECT_DURATION_BUG_FIX

#ifdef USE_BOOST_POTIONS
extern bool IS_BOOST_POTION_AFFECT(const DWORD type);
#endif
#ifdef USE_CRYSTAL_SYSTEM
extern bool IS_CRYSTAL_AFFECT(const DWORD type);
#endif
#ifdef USE_DROP_AFFECT_BONUSES
extern bool IS_DROP_AFFECT(const DWORD type);
#endif
#ifdef USE_PREMIUM_AFFECT
extern bool IS_PREMIUM_USER_AFFECT(const DWORD type);
#endif
#endif
