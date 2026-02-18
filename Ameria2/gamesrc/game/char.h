#ifndef __INC_METIN_II_CHAR_H__
#define __INC_METIN_II_CHAR_H__

#include <boost/unordered_map.hpp>

#include "../common/stl.h"
#include "../common/CommonDefines.h"

#ifdef ENABLE_CSHIELD
#include "cshieldLib.h"
#endif

#include "entity.h"
#include "FSM.h"
#include "horse_rider.h"
#include "vid.h"
#include "constants.h"
#include "affect.h"
#include "affect_flag.h"

#ifndef ENABLE_CUBE_RENEWAL_WORLDARD
#include "cube.h"
#else
#include "cuberenewal.h"
#endif
#include "utils.h"

# ifdef ENABLE_MINI_GAME_CATCH_KING
#include "packet.h"
#endif
#ifdef __BL_BATTLE_ROYALE__
#include "battle_royale.h"
#endif
#include "mining.h"
#define ENABLE_ANTI_CMD_FLOOD
#define ENABLE_OPEN_SHOP_WITH_ARMOR
enum eMountType {MOUNT_TYPE_NONE=0, MOUNT_TYPE_NORMAL=1, MOUNT_TYPE_COMBAT=2, MOUNT_TYPE_MILITARY=3};
eMountType GetMountLevelByVnum(DWORD dwMountVnum, bool IsNew);
const DWORD GetRandomSkillVnum(BYTE bJob = JOB_MAX_NUM);

#ifdef __NEWPET_SYSTEM__
class CNewPetSystem;
#endif

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
	AUTOMATIC_PICK_UP_SHOES = (1 << 17),
	AUTOMATIC_PICK_UP_BELTS = (1 << 18),
};
#endif

class CBuffOnAttributes;
class CPetSystem;
#ifdef ENABLE_SUPPORT_SYSTEM
class CSupportSystem;
#endif
#ifdef ENABLE_MOUNT_SYSTEM
class CMountSystem;
#endif

#ifdef ENABLE_IKASHOP_RENEWAL
namespace ikashop
{
	class CShop;
	class CShopSafebox;
	class CAuction;
}
#endif


#define INSTANT_FLAG_DEATH_PENALTY		(1 << 0)
#define INSTANT_FLAG_SHOP			(1 << 1)
#define INSTANT_FLAG_EXCHANGE			(1 << 2)
#define INSTANT_FLAG_STUN			(1 << 3)
#define INSTANT_FLAG_NO_REWARD			(1 << 4)

#define AI_FLAG_NPC				(1 << 0)
#define AI_FLAG_AGGRESSIVE			(1 << 1)
#define AI_FLAG_HELPER				(1 << 2)
#define AI_FLAG_STAYZONE			(1 << 3)
#define MAX_CARDS_IN_HAND	5
#define MAX_CARDS_IN_FIELD	3


#define SET_OVER_TIME(ch, time)	(ch)->SetOverTime(time)

extern int g_nPortalLimitTime;

enum
{
	MAIN_RACE_WARRIOR_M,
	MAIN_RACE_ASSASSIN_W,
	MAIN_RACE_SURA_M,
	MAIN_RACE_SHAMAN_W,
	MAIN_RACE_WARRIOR_W,
	MAIN_RACE_ASSASSIN_M,
	MAIN_RACE_SURA_W,
	MAIN_RACE_SHAMAN_M,
#ifdef ENABLE_WOLFMAN_CHARACTER
	MAIN_RACE_WOLFMAN_M,
#endif
	MAIN_RACE_MAX_NUM,
};

enum
{
	POISON_LENGTH = 30,
#ifdef ENABLE_WOLFMAN_CHARACTER
	BLEEDING_LENGTH = 30,
#endif
	STAMINA_PER_STEP = 1,
	SAFEBOX_PAGE_SIZE = 9,
	AI_CHANGE_ATTACK_POISITION_TIME_NEAR = 10000,
	AI_CHANGE_ATTACK_POISITION_TIME_FAR = 1000,
	AI_CHANGE_ATTACK_POISITION_DISTANCE = 100,
	SUMMON_MONSTER_COUNT = 3,
};

enum
{
	FLY_NONE,
	FLY_EXP,
	FLY_HP_MEDIUM,
	FLY_HP_BIG,
	FLY_SP_SMALL,
	FLY_SP_MEDIUM,
	FLY_SP_BIG,
	FLY_FIREWORK1,
	FLY_FIREWORK2,
	FLY_FIREWORK3,
	FLY_FIREWORK4,
	FLY_FIREWORK5,
	FLY_FIREWORK6,
	FLY_FIREWORK_CHRISTMAS,
	FLY_CHAIN_LIGHTNING,
	FLY_HP_SMALL,
	FLY_SKILL_MUYEONG,
#ifdef ENABLE_YOHARA_SYSTEM
	FLY_CONQUEROR_EXP,
#endif

};

enum EDamageType
{
	DAMAGE_TYPE_NONE,
	DAMAGE_TYPE_NORMAL,
	DAMAGE_TYPE_NORMAL_RANGE,
	//½ºÅ³
	DAMAGE_TYPE_MELEE,
	DAMAGE_TYPE_RANGE,
	DAMAGE_TYPE_FIRE,
	DAMAGE_TYPE_ICE,
	DAMAGE_TYPE_ELEC,
	DAMAGE_TYPE_MAGIC,
	DAMAGE_TYPE_POISON,
	DAMAGE_TYPE_SPECIAL,
#ifdef ENABLE_WOLFMAN_CHARACTER
	DAMAGE_TYPE_BLEEDING,
#endif
};

enum DamageFlag
{
	DAMAGE_NORMAL	= (1 << 0),
	DAMAGE_POISON	= (1 << 1),
	DAMAGE_DODGE	= (1 << 2),
	DAMAGE_BLOCK	= (1 << 3),
	DAMAGE_PENETRATE= (1 << 4),
	DAMAGE_CRITICAL = (1 << 5),
#if defined(ENABLE_WOLFMAN_CHARACTER) && !defined(USE_MOB_BLEEDING_AS_POISON)
	DAMAGE_BLEEDING	= (1 << 6),
#endif
};

enum EPointTypes
{
	POINT_NONE,                 // 0
	POINT_LEVEL,                // 1
	POINT_VOICE,                // 2
	POINT_EXP,                  // 3
	POINT_NEXT_EXP,             // 4
	POINT_HP,                   // 5
	POINT_MAX_HP,               // 6
	POINT_SP,                   // 7
	POINT_MAX_SP,               // 8
	POINT_STAMINA,              // 9  ½ºÅ×¹Ì³Ê
	POINT_MAX_STAMINA,          // 10 ÃÖ´ë ½ºÅ×¹Ì³Ê

	POINT_GOLD,                 // 11
	POINT_ST,                   // 12 ±Ù·Â
	POINT_HT,                   // 13 Ã¼·Â
	POINT_DX,                   // 14 ¹ÎÃ¸¼º
	POINT_IQ,                   // 15 Á¤½Å·Â
	POINT_DEF_GRADE,		// 16 ...
	POINT_ATT_SPEED,            // 17 °ø°Ý¼Óµµ
	POINT_ATT_GRADE,		// 18 °ø°Ý·Â MAX
	POINT_MOV_SPEED,            // 19 ÀÌµ¿¼Óµµ
	POINT_CLIENT_DEF_GRADE,	// 20 ¹æ¾îµî±Þ
	POINT_CASTING_SPEED,        // 21 ÁÖ¹®¼Óµµ (Äð´Ù¿îÅ¸ÀÓ*100) / (100 + ÀÌ°ª) = ÃÖÁ¾ Äð´Ù¿î Å¸ÀÓ
	POINT_MAGIC_ATT_GRADE,      // 22 ¸¶¹ý°ø°Ý·Â
	POINT_MAGIC_DEF_GRADE,      // 23 ¸¶¹ý¹æ¾î·Â
	POINT_EMPIRE_POINT,         // 24 Á¦±¹Á¡¼ö
	POINT_LEVEL_STEP,           // 25 ÇÑ ·¹º§¿¡¼­ÀÇ ´Ü°è.. (1 2 3 µÉ ¶§ º¸»ó, 4 µÇ¸é ·¹º§ ¾÷)
	POINT_STAT,                 // 26 ´É·ÂÄ¡ ¿Ã¸± ¼ö ÀÖ´Â °³¼ö
	POINT_SUB_SKILL,		// 27 º¸Á¶ ½ºÅ³ Æ÷ÀÎÆ®
	POINT_SKILL,		// 28 ¾×Æ¼ºê ½ºÅ³ Æ÷ÀÎÆ®
	POINT_WEAPON_MIN,		// 29 ¹«±â ÃÖ¼Ò µ¥¹ÌÁö
	POINT_WEAPON_MAX,		// 30 ¹«±â ÃÖ´ë µ¥¹ÌÁö
	POINT_PLAYTIME,             // 31 ÇÃ·¹ÀÌ½Ã°£
	POINT_HP_REGEN,             // 32 HP È¸º¹·ü
	POINT_SP_REGEN,             // 33 SP È¸º¹·ü

	POINT_BOW_DISTANCE,         // 34 È° »çÁ¤°Å¸® Áõ°¡Ä¡ (meter)

	POINT_HP_RECOVERY,          // 35 Ã¼·Â È¸º¹ Áõ°¡·®
	POINT_SP_RECOVERY,          // 36 Á¤½Å·Â È¸º¹ Áõ°¡·®

	POINT_POISON_PCT,           // 37 µ¶ È®·ü
	POINT_STUN_PCT,             // 38 ±âÀý È®·ü
	POINT_SLOW_PCT,             // 39 ½½·Î¿ì È®·ü
	POINT_CRITICAL_PCT,         // 40 Å©¸®Æ¼ÄÃ È®·ü
	POINT_PENETRATE_PCT,        // 41 °üÅëÅ¸°Ý È®·ü
	POINT_CURSE_PCT,            // 42 ÀúÁÖ È®·ü

	POINT_ATTBONUS_HUMAN,       // 43 ÀÎ°£¿¡°Ô °­ÇÔ
	POINT_ATTBONUS_ANIMAL,      // 44 µ¿¹°¿¡°Ô µ¥¹ÌÁö % Áõ°¡
	POINT_ATTBONUS_ORC,         // 45 ¿õ±Í¿¡°Ô µ¥¹ÌÁö % Áõ°¡
	POINT_ATTBONUS_MILGYO,      // 46 ¹Ð±³¿¡°Ô µ¥¹ÌÁö % Áõ°¡
	POINT_ATTBONUS_UNDEAD,      // 47 ½ÃÃ¼¿¡°Ô µ¥¹ÌÁö % Áõ°¡
	POINT_ATTBONUS_DEVIL,       // 48 ¸¶±Í(¾Ç¸¶)¿¡°Ô µ¥¹ÌÁö % Áõ°¡
	POINT_ATTBONUS_INSECT,      // 49 ¹ú·¹Á·
	POINT_ATTBONUS_FIRE,        // 50 È­¿°Á·
	POINT_ATTBONUS_ICE,         // 51 ºù¼³Á·
	POINT_ATTBONUS_DESERT,      // 52 »ç¸·Á·
	POINT_ATTBONUS_MONSTER,     // 53 ¸ðµç ¸ó½ºÅÍ¿¡°Ô °­ÇÔ
	POINT_ATTBONUS_WARRIOR,     // 54 ¹«»ç¿¡°Ô °­ÇÔ
	POINT_ATTBONUS_ASSASSIN,	// 55 ÀÚ°´¿¡°Ô °­ÇÔ
	POINT_ATTBONUS_SURA,		// 56 ¼ö¶ó¿¡°Ô °­ÇÔ
	POINT_ATTBONUS_SHAMAN,		// 57 ¹«´ç¿¡°Ô °­ÇÔ
	POINT_ATTBONUS_TREE,     	// 58 ³ª¹«¿¡°Ô °­ÇÔ 20050729.myevan UNUSED5

	POINT_RESIST_WARRIOR,		// 59 ¹«»ç¿¡°Ô ÀúÇ×
	POINT_RESIST_ASSASSIN,		// 60 ÀÚ°´¿¡°Ô ÀúÇ×
	POINT_RESIST_SURA,			// 61 ¼ö¶ó¿¡°Ô ÀúÇ×
	POINT_RESIST_SHAMAN,		// 62 ¹«´ç¿¡°Ô ÀúÇ×

	POINT_STEAL_HP,             // 63 »ý¸í·Â Èí¼ö
	POINT_STEAL_SP,             // 64 Á¤½Å·Â Èí¼ö

	POINT_MANA_BURN_PCT,        // 65 ¸¶³ª ¹ø

	/// ÇÇÇØ½Ã º¸³Ê½º ///

	POINT_DAMAGE_SP_RECOVER,    // 66 °ø°Ý´çÇÒ ½Ã Á¤½Å·Â È¸º¹ È®·ü

	POINT_BLOCK,                // 67 ºí·°À²
	POINT_DODGE,                // 68 È¸ÇÇÀ²

	POINT_RESIST_SWORD,         // 69
	POINT_RESIST_TWOHAND,       // 70
	POINT_RESIST_DAGGER,        // 71
	POINT_RESIST_BELL,          // 72
	POINT_RESIST_FAN,           // 73
	POINT_RESIST_BOW,           // 74  È­»ì   ÀúÇ×   : ´ë¹ÌÁö °¨¼Ò
	POINT_RESIST_FIRE,          // 75  È­¿°   ÀúÇ×   : È­¿°°ø°Ý¿¡ ´ëÇÑ ´ë¹ÌÁö °¨¼Ò
	POINT_RESIST_ELEC,          // 76  Àü±â   ÀúÇ×   : Àü±â°ø°Ý¿¡ ´ëÇÑ ´ë¹ÌÁö °¨¼Ò
	POINT_RESIST_MAGIC,         // 77  ¼ú¹ý   ÀúÇ×   : ¸ðµç¼ú¹ý¿¡ ´ëÇÑ ´ë¹ÌÁö °¨¼Ò
	POINT_RESIST_WIND,          // 78  ¹Ù¶÷   ÀúÇ×   : ¹Ù¶÷°ø°Ý¿¡ ´ëÇÑ ´ë¹ÌÁö °¨¼Ò

	POINT_REFLECT_MELEE,        // 79 °ø°Ý ¹Ý»ç

	/// Æ¯¼ö ÇÇÇØ½Ã ///
	POINT_REFLECT_CURSE,		// 80 ÀúÁÖ ¹Ý»ç
	POINT_POISON_REDUCE,		// 81 µ¶µ¥¹ÌÁö °¨¼Ò

	/// Àû ¼Ò¸ê½Ã ///
	POINT_KILL_SP_RECOVER,		// 82 Àû ¼Ò¸ê½Ã MP È¸º¹
	POINT_EXP_DOUBLE_BONUS,		// 83
	POINT_GOLD_DOUBLE_BONUS,		// 84
	POINT_ITEM_DROP_BONUS,		// 85

	/// È¸º¹ °ü·Ã ///
	POINT_POTION_BONUS,			// 86
	POINT_KILL_HP_RECOVERY,		// 87

	POINT_IMMUNE_STUN,			// 88
	POINT_IMMUNE_SLOW,			// 89
	POINT_IMMUNE_FALL,			// 90
	//////////////////

	POINT_PARTY_ATTACKER_BONUS,		// 91
	POINT_PARTY_TANKER_BONUS,		// 92

	POINT_ATT_BONUS,			// 93
	POINT_DEF_BONUS,			// 94

	POINT_ATT_GRADE_BONUS,		// 95
	POINT_DEF_GRADE_BONUS,		// 96
	POINT_MAGIC_ATT_GRADE_BONUS,	// 97
	POINT_MAGIC_DEF_GRADE_BONUS,	// 98

	POINT_RESIST_NORMAL_DAMAGE,		// 99

	POINT_HIT_HP_RECOVERY,		// 100
	POINT_HIT_SP_RECOVERY, 		// 101
	POINT_MANASHIELD,			// 102 Èæ½Å¼öÈ£ ½ºÅ³¿¡ ÀÇÇÑ ¸¶³ª½¯µå È¿°ú Á¤µµ

	POINT_PARTY_BUFFER_BONUS,		// 103
	POINT_PARTY_SKILL_MASTER_BONUS,	// 104

	POINT_HP_RECOVER_CONTINUE,		// 105
	POINT_SP_RECOVER_CONTINUE,		// 106

	POINT_STEAL_GOLD,			// 107
	POINT_POLYMORPH,			// 108 º¯½ÅÇÑ ¸ó½ºÅÍ ¹øÈ£
	POINT_MOUNT,			// 109 Å¸°íÀÖ´Â ¸ó½ºÅÍ ¹øÈ£

	POINT_PARTY_HASTE_BONUS,		// 110
	POINT_PARTY_DEFENDER_BONUS,		// 111
	POINT_STAT_RESET_COUNT,		// 112 ÇÇÀÇ ´Ü¾à »ç¿ëÀ» ÅëÇÑ ½ºÅÝ ¸®¼Â Æ÷ÀÎÆ® (1´ç 1Æ÷ÀÎÆ® ¸®¼Â°¡´É)

	POINT_HORSE_SKILL,			// 113

	POINT_MALL_ATTBONUS,		// 114 °ø°Ý·Â +x%
	POINT_MALL_DEFBONUS,		// 115 ¹æ¾î·Â +x%
	POINT_MALL_EXPBONUS,		// 116 °æÇèÄ¡ +x%
	POINT_MALL_ITEMBONUS,		// 117 ¾ÆÀÌÅÛ µå·ÓÀ² x/10¹è
	POINT_MALL_GOLDBONUS,		// 118 µ· µå·ÓÀ² x/10¹è

	POINT_MAX_HP_PCT,			// 119 ÃÖ´ë»ý¸í·Â +x%
	POINT_MAX_SP_PCT,			// 120 ÃÖ´ëÁ¤½Å·Â +x%

	POINT_SKILL_DAMAGE_BONUS,		// 121 ½ºÅ³ µ¥¹ÌÁö *(100+x)%
	POINT_NORMAL_HIT_DAMAGE_BONUS,	// 122 ÆòÅ¸ µ¥¹ÌÁö *(100+x)%

	// DEFEND_BONUS_ATTRIBUTES
	POINT_SKILL_DEFEND_BONUS,		// 123 ½ºÅ³ ¹æ¾î µ¥¹ÌÁö
	POINT_NORMAL_HIT_DEFEND_BONUS,	// 124 ÆòÅ¸ ¹æ¾î µ¥¹ÌÁö
	// END_OF_DEFEND_BONUS_ATTRIBUTES

	// PC_BANG_ITEM_ADD
	POINT_PC_BANG_EXP_BONUS,		// 125 PC¹æ Àü¿ë °æÇèÄ¡ º¸³Ê½º
	POINT_PC_BANG_DROP_BONUS,		// 126 PC¹æ Àü¿ë µå·Ó·ü º¸³Ê½º
	// END_PC_BANG_ITEM_ADD
	POINT_RAMADAN_CANDY_BONUS_EXP,			// ¶ó¸¶´Ü »çÅÁ °æÇèÄ¡ Áõ°¡¿ë

	POINT_ENERGY = 128,					// 128 ±â·Â

	// ±â·Â ui ¿ë.
	// ¼­¹ö¿¡¼­ ¾²Áö ¾Ê±â¸¸, Å¬¶óÀÌ¾ðÆ®¿¡¼­ ±â·ÂÀÇ ³¡ ½Ã°£À» POINT·Î °ü¸®ÇÏ±â ¶§¹®¿¡ ÀÌ·¸°Ô ÇÑ´Ù.
	// ¾Æ ºÎ²ô·´´Ù
	POINT_ENERGY_END_TIME = 129,					// 129 ±â·Â Á¾·á ½Ã°£

	POINT_COSTUME_ATTR_BONUS = 130,
	POINT_MAGIC_ATT_BONUS_PER = 131,
	POINT_MELEE_MAGIC_ATT_BONUS_PER = 132,

	// Ãß°¡ ¼Ó¼º ÀúÇ×
	POINT_RESIST_ICE = 133,          //   ³Ã±â ÀúÇ×   : ¾óÀ½°ø°Ý¿¡ ´ëÇÑ ´ë¹ÌÁö °¨¼Ò
	POINT_RESIST_EARTH = 134,        //   ´ëÁö ÀúÇ×   : ¾óÀ½°ø°Ý¿¡ ´ëÇÑ ´ë¹ÌÁö °¨¼Ò
	POINT_RESIST_DARK = 135,         //   ¾îµÒ ÀúÇ×   : ¾óÀ½°ø°Ý¿¡ ´ëÇÑ ´ë¹ÌÁö °¨¼Ò

	POINT_RESIST_CRITICAL = 136,		// Å©¸®Æ¼ÄÃ ÀúÇ×	: »ó´ëÀÇ Å©¸®Æ¼ÄÃ È®·üÀ» °¨¼Ò
	POINT_RESIST_PENETRATE = 137,		// °üÅëÅ¸°Ý ÀúÇ×	: »ó´ëÀÇ °üÅëÅ¸°Ý È®·üÀ» °¨¼Ò

#ifdef ENABLE_WOLFMAN_CHARACTER
	POINT_BLEEDING_REDUCE = 138,
	POINT_BLEEDING_PCT = 139,

	POINT_ATTBONUS_WOLFMAN = 140,				// 140 ¼öÀÎÁ·¿¡°Ô °­ÇÔ
	POINT_RESIST_WOLFMAN = 141,				// 141 ¼öÀÎÁ·¿¡°Ô ÀúÇ×
	POINT_RESIST_CLAW = 142,					// 142 CLAW¿¡ ÀúÇ×
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	POINT_ACCEDRAIN_RATE = 143,
#endif
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	POINT_RESIST_MAGIC_REDUCTION = 144,
#endif

#ifdef ENABLE_NEW_BONUS_WORLDARD
	POINT_RESIST_HUMAN = 220,
#endif

#ifdef ELEMENT_NEW_BONUSES
	POINT_ATTBONUS_ELEC = 176,
	POINT_ATTBONUS_WIND = 177,
	POINT_ATTBONUS_EARTH  = 178,
	POINT_ATTBONUS_DARK  = 179,
#endif

#ifdef BATTLEPASS_WORLDARD
	POINT_BATTLE_PASS_ID	= 181,
#endif
#ifdef ELEMENT_NEW_BONUSES
	POINT_ENCHANT_ELEC,
	POINT_ENCHANT_FIRE,
	POINT_ENCHANT_ICE,
	POINT_ENCHANT_WIND,
	POINT_ENCHANT_EARTH,
	POINT_ENCHANT_DARK,

	POINT_ATTBONUS_SWORD,		
	POINT_ATTBONUS_TWOHAND,	
	POINT_ATTBONUS_DAGGER,	
	POINT_ATTBONUS_BELL,		
	POINT_ATTBONUS_FAN,		
	POINT_ATTBONUS_BOW,

	POINT_ATTBONUS_ZODIAC,
#ifdef ENABLE_WOLFMAN_CHARACTER
	POINT_ATTBONUS_CLAW,
#endif
#endif
#ifdef NEW_BONUS
	POINT_ATTBONUS_STONE = 196,
	POINT_ATTBONUS_BOSS = 197,
#endif

#ifdef ENABLE_YOHARA_SYSTEM
	POINT_CONQUEROR_LEVEL = 205,
	POINT_CONQUEROR_LEVEL_STEP = 206,
	POINT_SUNGMA_STR = 207,
	POINT_SUNGMA_HP = 208,
	POINT_SUNGMA_MOVE = 209,
	POINT_SUNGMA_IMMUNE = 210,
	POINT_CONQUEROR_POINT = 211,
	POINT_CONQUEROR_EXP = 212,
	POINT_CONQUEROR_NEXT_EXP = 213,
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	POINT_HIT_PCT,
	POINT_RESIST_MOUNT_FALL,
	POINT_MONSTER_DEFEND_BONUS,
#endif


#ifdef __GEM_SYSTEM__
	POINT_GEM = 240,
#endif
#ifdef VOTE_BUFF_RENEWAL
	POINT_VCOINS = 241,
#endif
#ifdef USE_CHEQUE_CURRENCY
	POINT_CHEQUE = 242,
#endif
// #ifdef USE_DROP_AFFECT_BONUSES
	POINT_DOUBLE_DROP_METIN = 243,
	POINT_DOUBLE_DROP_BOSS = 244,
// #endif
// #ifdef USE_PREMIUM_AFFECT
	POINT_PREMIUM_USER = 245,
// #endif
	//POINT_MAX_NUM = 129	common/length.h
};

enum EPKModes
{
	PK_MODE_PEACE,
	PK_MODE_REVENGE,
	PK_MODE_FREE,
	PK_MODE_PROTECT,
	PK_MODE_GUILD,
#ifdef TOURNAMENT_PVP_SYSTEM
	PK_MODE_TEAM_A,
	PK_MODE_TEAM_B,
#endif
	PK_MODE_FEAR,

	PK_MODE_MAX_NUM
};

enum EPositions
{
	POS_DEAD,
	POS_SLEEPING,
	POS_RESTING,
	POS_SITTING,
	POS_FISHING,
	POS_FIGHTING,
	POS_MOUNTING,
	POS_STANDING
};

enum EBlockAction
{
	BLOCK_EXCHANGE		= (1 << 0),
	BLOCK_PARTY_INVITE		= (1 << 1),
	BLOCK_GUILD_INVITE		= (1 << 2),
	BLOCK_WHISPER		= (1 << 3),
	BLOCK_MESSENGER_INVITE	= (1 << 4),
	BLOCK_PARTY_REQUEST		= (1 << 5),
};

// <Factor> Dynamically evaluated CHARACTER* equivalent.
// Referring to SCharDeadEventInfo.
struct DynamicCharacterPtr {
	DynamicCharacterPtr() : is_pc(false), id(0) {}
	DynamicCharacterPtr(const DynamicCharacterPtr& o)
		: is_pc(o.is_pc), id(o.id) {}

	// Returns the LPCHARACTER found in CHARACTER_MANAGER.
	LPCHARACTER Get() const;
	// Clears the current settings.
	void Reset() {
		is_pc = false;
		id = 0;
	}

	// Basic assignment operator.
	DynamicCharacterPtr& operator=(const DynamicCharacterPtr& rhs) {
		is_pc = rhs.is_pc;
		id = rhs.id;
		return *this;
	}
	// Supports assignment with LPCHARACTER type.
	DynamicCharacterPtr& operator=(LPCHARACTER character);
	// Supports type casting to LPCHARACTER.
	operator LPCHARACTER() const {
		return Get();
	}

	bool is_pc;
	uint32_t id;
};

/* ÀúÀåÇÏ´Â µ¥ÀÌÅÍ */
typedef struct character_point
{
	long long		points[POINT_MAX_NUM];

	BYTE			job;
	BYTE			voice;

	BYTE			level;

#ifdef ENABLE_YOHARA_SYSTEM
	BYTE	conquerorlevel;
	DWORD	conqueror_exp;
#endif

	DWORD			exp;
	long long		gold;
#ifdef USE_CHEQUE_CURRENCY
	uint16_t cheque;
#endif
#ifdef __GEM_SYSTEM__
	int			gem;
#endif

	int				hp;
	int				sp;

	int				iRandomHP;
	int				iRandomSP;

	int				stamina;

	BYTE			skill_group;
#ifdef ENABLE_NEW_DETAILS_GUI
	long			kill_log[KILL_MAX_NUM];
#endif
} CHARACTER_POINT;

typedef struct character_point_instant
{
	long long		points[POINT_MAX_NUM];

	float			fRot;

	int				iMaxHP;
	int				iMaxSP;

	long			position;

	long			instant_flag;

	DWORD			dwAIFlag;
	DWORD			dwImmuneFlag;
	DWORD			dwLastShoutPulse;

	uint32_t		parts[PART_MAX_NUM];

	LPCHARACTER		battle_victim;

	BYTE			gm_level;

	BYTE			bBasePart;

	int				iMaxStamina;

	BYTE			bBlockMode;

	int				iDragonSoulActiveDeck;

#ifdef ENABLE_ANTI_EXP
	bool			anti_exp;
#endif
} CHARACTER_POINT_INSTANT;

struct SPlayerData
{
	TQuickslot		pQuickSlot[QUICKSLOT_MAX_NUM];

	LPITEM			pItems[INVENTORY_AND_EQUIP_SLOT_MAX];
	WORD			wItemGrid[INVENTORY_AND_EQUIP_SLOT_MAX];

	LPITEM			pDSItems[DRAGON_SOUL_INVENTORY_MAX_NUM];
	WORD			wDSItemGrid[DRAGON_SOUL_INVENTORY_MAX_NUM];
	LPENTITY		m_pDragonSoulRefineWindowOpener;

#ifdef ENABLE_SPECIAL_STORAGE
	LPITEM			pSSUItems[SPECIAL_INVENTORY_MAX_NUM];
	WORD			wSSUItemGrid[SPECIAL_INVENTORY_MAX_NUM];

	LPITEM			pSSBItems[SPECIAL_INVENTORY_MAX_NUM];
	WORD			wSSBItemGrid[SPECIAL_INVENTORY_MAX_NUM];

	LPITEM			pSSSItems[SPECIAL_INVENTORY_MAX_NUM];
	WORD			wSSSItemGrid[SPECIAL_INVENTORY_MAX_NUM];

	LPITEM			pSSCItems[SPECIAL_INVENTORY_MAX_NUM];
	WORD			wSSCItemGrid[SPECIAL_INVENTORY_MAX_NUM];

	LPITEM			pCostumeItems[SPECIAL_INVENTORY_MAX_NUM];
	WORD			wCostumeItemGrid[SPECIAL_INVENTORY_MAX_NUM];

	LPITEM			pChestItems[SPECIAL_INVENTORY_MAX_NUM];
	WORD			wChestItemGrid[SPECIAL_INVENTORY_MAX_NUM];
#endif

	LPCHARACTER		pCubeNpc;

#ifdef ENABLE_SWITCHBOT
#ifdef ENABLE_SWITCHBOT_RENEWAL
	WORD			m_wSwitchbotCell[SWITCHBOT_SLOT_COUNT];
	DWORD			m_dwSwitchbotUsed[SWITCHBOT_SLOT_COUNT];
#else
	LPITEM			pSwitchbotItems[SWITCHBOT_SLOT_COUNT];
#endif
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
	LPITEM			pSBItems[SWITCHBOT_SLOT_MAX];
#endif

#ifdef CHANGE_EQUIP_WORLDARD
	LPITEM 			pChangeEquipItem[CHANGE_EQUIP_SLOT_COUNT];
#endif

#ifdef __SASH_SYSTEM__
	LPITEM			pSashMaterials[SASH_WINDOW_MAX_MATERIALS];
#endif

#ifdef __AURA_SYSTEM__
	LPENTITY		m_pAuraRefineWindowOpener;
#endif

#ifdef USE_CRYSTAL_SYSTEM
	bool			m_bShardCraftWindowStat;
	SShardCraft		m_pShardCraft[SHARD_CRAFT_SLOT_MAX];
#endif

#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
	int				m_iAutoHuntLastDay;
	int				m_iAutoHuntUsedTime;
	int				m_iAutoHuntLastUpdate;
#endif

#ifdef USE_BRAVE_CAPE_RENEWED
	DWORD			m_dwReviveTime;
#endif
};

#define TRIGGERPARAM		LPCHARACTER ch, LPCHARACTER causer

typedef struct trigger
{
	BYTE	type;
	int		(*func) (TRIGGERPARAM);
	long	value;
} TRIGGER;

class CTrigger
{
	public:
		CTrigger() : bType(0), pFunc(NULL)
		{
		}

		BYTE	bType;
		int	(*pFunc) (TRIGGERPARAM);
};

EVENTINFO(char_event_info)
{
	DynamicCharacterPtr ch;
};

typedef std::map<VID, size_t> target_map;
struct TSkillUseInfo
{
	int	    iHitCount;
	int	    iMaxHitCount;
	int	    iSplashCount;
	DWORD   dwNextSkillUsableTime;
	int	    iRange;
	bool    bSkillCD;
	DWORD   dwHitCount;
	bool    bUsed;
	DWORD   dwVID;
	bool    isGrandMaster;

	target_map TargetVIDMap;

	TSkillUseInfo()
		: iHitCount(0), iMaxHitCount(0), iSplashCount(0), dwNextSkillUsableTime(0), iRange(0), bSkillCD(false), dwHitCount(0), bUsed(false),
		dwVID(0), isGrandMaster(false)
   	{}

	bool    HitOnce(DWORD dwVnum = 0);
	bool    IsSkillCooldown(DWORD dwVnum, float fSkillPower);

	bool    UseSkill(bool isGrandMaster, DWORD vid, DWORD dwCooltime, int splashcount = 1, int hitcount = -1, int range = -1);
	DWORD   GetMainTargetVID() const	{ return dwVID; }
	void    SetMainTargetVID(DWORD vid) { dwVID=vid; }
	void    ResetHitCount() { if (iSplashCount) { iHitCount = iMaxHitCount; iSplashCount--; } }
};

typedef struct packet_party_update TPacketGCPartyUpdate;
class CExchange;
class CSkillProto;
class CParty;
class CDungeon;
class CWarMap;
class CAffect;
class CGuild;
class CSafebox;
class CArena;
class CShop;
typedef class CShop * LPSHOP;

class CMob;
class CMobInstance;
typedef struct SMobSkillInfo TMobSkillInfo;

//SKILL_POWER_BY_LEVEL
extern int GetSkillPowerByLevelFromType(int job, int skillgroup, int skilllevel);
//END_SKILL_POWER_BY_LEVEL

namespace marriage
{
	class WeddingMap;
}
enum e_overtime
{
	OT_NONE,
	OT_3HOUR,
	OT_5HOUR,
};

// #define NEW_ICEDAMAGE_SYSTEM
class CHARACTER : public CEntity, public CFSM, public CHorseRider
{
	protected:
		//////////////////////////////////////////////////////////////////////////////////
		// Entity °ü·Ã
		virtual void	EncodeInsertPacket(LPENTITY entity);
		virtual void	EncodeRemovePacket(LPENTITY entity);
		//////////////////////////////////////////////////////////////////////////////////

	public:
		LPCHARACTER			FindCharacterInView(const char * name, bool bFindPCOnly);
		void				UpdatePacket();

		//////////////////////////////////////////////////////////////////////////////////
		// FSM (Finite State Machine) °ü·Ã
	protected:
		CStateTemplate<CHARACTER>	m_stateMove;
		CStateTemplate<CHARACTER>	m_stateBattle;
		CStateTemplate<CHARACTER>	m_stateIdle;

	public:
		virtual void		StateMove();
		virtual void		StateBattle();
		virtual void		StateIdle();
		virtual void		StateFlag();
		virtual void		StateFlagBase();
		void				StateHorse();

	protected:
		// STATE_IDLE_REFACTORING
		void				__StateIdle_Monster();
		void				__StateIdle_Stone();
		void				__StateIdle_NPC();
		// END_OF_STATE_IDLE_REFACTORING

	public:
		DWORD GetAIFlag() const	{ return m_pointsInstant.dwAIFlag; }

		void				SetAggressive();
		bool				IsAggressive() const;

		void				SetCoward();
		bool				IsCoward() const;
		void				CowardEscape();

		void				SetNoAttackShinsu();
		bool				IsNoAttackShinsu() const;

		void				SetNoAttackChunjo();
		bool				IsNoAttackChunjo() const;

		void				SetNoAttackJinno();
		bool				IsNoAttackJinno() const;

		void				SetAttackMob();
		bool				IsAttackMob() const;

		virtual void			BeginStateEmpty();
		virtual void			EndStateEmpty() {}

		void				RestartAtSamePos();
		
#ifdef ENABLE_SUNG_MAHI_TOWER
		void				SetNomove();
		void				RemoveNomove();
		bool				IsNomove() const;
		
		void				SetNoattack();
		void				RemoveNoattack();
		bool				IsNoattack() const;
#endif

	protected:
		DWORD				m_dwStateDuration;
		//////////////////////////////////////////////////////////////////////////////////

	public:
		CHARACTER();
		virtual ~CHARACTER();

		void			Create(const char * c_pszName, DWORD vid, bool isPC);
		void			Destroy();

		void			Disconnect(const char * c_pszReason);

	protected:
		void			Initialize();

		//////////////////////////////////////////////////////////////////////////////////
		// Basic Points
#ifdef __SEND_TARGET_INFO__
	private:
		DWORD			dwLastTargetInfoPulse;

	public:
		DWORD			GetLastTargetInfoPulse() const	{ return dwLastTargetInfoPulse; }
		void			SetLastTargetInfoPulse(DWORD pulse) { dwLastTargetInfoPulse = pulse; }
#endif
	public:
		DWORD			GetPlayerID() const	{ return m_dwPlayerID; }

		void			SetPlayerProto(const TPlayerTable * table);
		void			CreatePlayerProto(TPlayerTable & tab);	// ÀúÀå ½Ã »ç¿ë

		void			SetProto(const CMob * c_pkMob);
		WORD			GetRaceNum() const;

		void			Save();		// DelayedSave
		void			SaveReal();	// ½ÇÁ¦ ÀúÀå
		void			FlushDelayedSaveItem();

		const char *	GetName() const;
		const VID &		GetVID() const		{ return m_vid;		}

		void			SetName(const std::string& name) { m_stName = name; }

		void			SetRace(BYTE race);
		bool			ChangeSex();

		DWORD			GetAID() const;
		int				GetChangeEmpireCount() const;
		void			SetChangeEmpireCount();
		int				ChangeEmpire(BYTE empire);

		BYTE			GetJob() const;
		BYTE			GetCharType() const;

		bool			IsPC() const		{ return GetDesc() ? true : false; }
		bool			IsNPC()	const		{ return m_bCharType != CHAR_TYPE_PC; }
		bool			IsMonster()	const	{ return m_bCharType == CHAR_TYPE_MONSTER; }
		bool			IsStone() const		{ return m_bCharType == CHAR_TYPE_STONE; }
		bool			IsDoor() const		{ return m_bCharType == CHAR_TYPE_DOOR; }
		bool			IsBuilding() const	{ return m_bCharType == CHAR_TYPE_BUILDING;  }
		bool			IsWarp() const		{ return m_bCharType == CHAR_TYPE_WARP; }
		bool			IsGoto() const		{ return m_bCharType == CHAR_TYPE_GOTO; }
//		bool			IsPet() const		{ return m_bCharType == CHAR_TYPE_PET; }

		DWORD			GetLastShoutPulse() const	{ return m_pointsInstant.dwLastShoutPulse; }
		void			SetLastShoutPulse(DWORD pulse) { m_pointsInstant.dwLastShoutPulse = pulse; }
		int				GetLevel() const		{ return m_points.level;	}
		void			SetLevel(BYTE level);

#ifdef ENABLE_YOHARA_SYSTEM
		int				GetConquerorLevel() const { return m_points.conquerorlevel; }
		void			SetConquerorLevel(BYTE level);

		DWORD			GetConquerorExp() const { return m_points.conqueror_exp; }
		void			SetConquerorExp(DWORD exp) { m_points.conqueror_exp = exp; }
		DWORD			GetConquerorNextExp() const;

		void ResetConquerorPoint(int iLv);
		bool IsSungmaMap() const;

		uint8_t GetSungmaMapAttribute(BYTE wPoint) const;

		bool IsConquerorItem(LPITEM pItem);
#endif


		BYTE			GetGMLevel() const;
		BOOL 			IsGM() const;
		void			SetGMLevel();

		DWORD			GetExp() const		{ return m_points.exp;	}
		void			SetExp(DWORD exp)	{ m_points.exp = exp;	}
		DWORD			GetNextExp() const;
#ifdef __ANTI_EXP__
		bool            block_exp;  
#endif

#ifdef __NEWPET_SYSTEM__
		DWORD			PetGetNextExp() const;
#endif
		LPCHARACTER		DistributeExp();	// Á¦ÀÏ ¸¹ÀÌ ¶§¸° »ç¶÷À» ¸®ÅÏÇÑ´Ù.
		void			DistributeHP(LPCHARACTER pkKiller);
		void			DistributeSP(LPCHARACTER pkKiller, int iMethod=0);

		void			SetPosition(int pos);
		bool			IsPosition(int pos) const	{ return m_pointsInstant.position == pos ? true : false; }
		int				GetPosition() const		{ return m_pointsInstant.position; }

		void			SetPart(BYTE bPartPos, uint32_t wVal);
		uint32_t			GetPart(BYTE bPartPos) const;
		uint32_t			GetOriginalPart(BYTE bPartPos) const;

		void			SetHP(int hp)		{ m_points.hp = hp; }
		int				GetHP() const		{ return m_points.hp; }

		void			SetSP(int sp)		{ m_points.sp = sp; }
		int				GetSP() const		{ return m_points.sp; }

		void			SetStamina(int stamina)	{ m_points.stamina = stamina; }
		int				GetStamina() const		{ return m_points.stamina; }

		void			SetMaxHP(int iVal)	{ m_pointsInstant.iMaxHP = iVal; }
		int				GetMaxHP() const	{ return m_pointsInstant.iMaxHP; }

		void			SetMaxSP(int iVal)	{ m_pointsInstant.iMaxSP = iVal; }
		int				GetMaxSP() const	{ return m_pointsInstant.iMaxSP; }

		void			SetMaxStamina(int iVal)	{ m_pointsInstant.iMaxStamina = iVal; }
		int				GetMaxStamina() const	{ return m_pointsInstant.iMaxStamina; }

		void			SetRandomHP(int v)	{ m_points.iRandomHP = v; }
		void			SetRandomSP(int v)	{ m_points.iRandomSP = v; }

		int				GetRandomHP() const	{ return m_points.iRandomHP; }
		int				GetRandomSP() const	{ return m_points.iRandomSP; }

		int				GetHPPct() const;

		void			SetRealPoint(BYTE idx, int val);
		int				GetRealPoint(BYTE idx) const;

		void			SetPoint(BYTE idx, long long val);

		bool Test() const;

		long long		GetPoint(BYTE idx) const;
		int				GetLimitPoint(BYTE idx) const;
		int				GetPolymorphPoint(BYTE idx) const;

		const TMobTable &	GetMobTable() const;
		BYTE				GetMobRank() const;
		BYTE				GetMobBattleType() const;
		BYTE				GetMobSize() const;
		DWORD				GetMobDamageMin() const;
		DWORD				GetMobDamageMax() const;
		WORD				GetMobAttackRange() const;
		DWORD				GetMobDropItemVnum() const;
		float				GetMobDamageMultiply() const;

		// NEWAI
		bool			IsBerserker() const;
		bool			IsBerserk() const;
		void			SetBerserk(bool mode);

		bool			IsStoneSkinner() const;

		bool			IsGodSpeeder() const;
		bool			IsGodSpeed() const;
		void			SetGodSpeed(bool mode);

		bool			IsDeathBlower() const;
		bool			IsDeathBlow() const;

		bool			IsReviver() const;
		bool			HasReviverInParty() const;
		bool			IsRevive() const;
		void			SetRevive(bool mode);
		// NEWAI END

		bool			IsRaceFlag(DWORD dwBit) const;
		bool			IsSummonMonster() const;
		DWORD			GetSummonVnum() const;

		DWORD			GetPolymorphItemVnum() const;
		DWORD			GetMonsterDrainSPPoint() const;

		void			MainCharacterPacket();	// ³»°¡ ¸ÞÀÎÄ³¸¯ÅÍ¶ó°í º¸³»ÁØ´Ù.

		void			ComputePoints();
		void			ComputeBattlePoints();
		void			PointChange(BYTE type, long long amount, bool bAmount = false, bool bBroadcast = false);
		void			PointsPacket();
		void			ApplyPoint(BYTE bApplyType, int iVal);
		void			CheckMaximumPoints();	// HP, SP µîÀÇ ÇöÀç °ªÀÌ ÃÖ´ë°ª º¸´Ù ³ôÀºÁö °Ë»çÇÏ°í ³ô´Ù¸é ³·Ãá´Ù.
#ifdef __NEWPET_SYSTEM__
		void			SendPetLevelUpEffect(int vid, int type, int value, int amount);
#endif
		bool			Show(long lMapIndex, long x, long y, long z = LONG_MAX, bool bShowSpawnMotion = false);

		void			Sitdown(int is_ground);
		void			Standup();

		void			SetRotation(float fRot);
		void			SetRotationToXY(long x, long y);
		float			GetRotation() const	{ return m_pointsInstant.fRot; }

		void			MotionPacketEncode(BYTE motion, LPCHARACTER victim, struct packet_motion * packet);
		void			Motion(BYTE motion, LPCHARACTER victim = NULL);

		void			ChatPacket(BYTE type, const char *format, ...);
		void			MonsterChat(BYTE bMonsterChatType);
		void			SendGreetMessage();

		void			ResetPoint(int iLv);

		void			SetBlockMode(BYTE bFlag);
		void			SetBlockModeForce(BYTE bFlag);
		bool			IsBlockMode(BYTE bFlag) const	{ return (m_pointsInstant.bBlockMode & bFlag)?true:false; }

		bool			IsPolymorphed() const		{ return m_dwPolymorphRace>0; }
		bool			IsPolyMaintainStat() const	{ return m_bPolyMaintainStat; } // ÀÌÀü ½ºÅÝÀ» À¯ÁöÇÏ´Â Æú¸®¸ðÇÁ.
		void			SetPolymorph(DWORD dwRaceNum, bool bMaintainStat = false);
		DWORD			GetPolymorphVnum() const	{ return m_dwPolymorphRace; }
		int				GetPolymorphPower() const;

		void			StartAnticheatCommunication();


		// FISING
#ifdef ENABLE_CSHIELD
		bool			IsNearWater() const;
#endif
		void			fishing(uint32_t fishing_key = 0);
		void			fishing_take(uint32_t fishing_key = 0);
		// END_OF_FISHING

		// MINING
		void			mining(LPCHARACTER chLoad);
		void			mining_cancel();
		void			mining_take();
		// END_OF_MINING

		void			ResetPlayTime(DWORD dwTimeRemain = 0);

		void			CreateFly(BYTE bType, LPCHARACTER pkVictim);

		void			ResetChatCounter();
		BYTE			IncreaseChatCounter();
		BYTE			GetChatCounter() const;

	protected:
		DWORD			m_dwPolymorphRace;
		bool			m_bPolyMaintainStat;
		DWORD			m_dwLoginPlayTime;
		DWORD			m_dwPlayerID;
		VID				m_vid;
		std::string		m_stName;
#ifdef __NEWPET_SYSTEM__
		BYTE			m_stImmortalSt;
#endif
		BYTE			m_bCharType;
#ifdef __NEWPET_SYSTEM__
		DWORD			m_newpetskillcd[3];
#endif
		CHARACTER_POINT		m_points;
		CHARACTER_POINT_INSTANT	m_pointsInstant;
		std::unique_ptr<SPlayerData> m_playerData;

		int	waitHackCounter;
		LPEVENT			m_pkAnticheatEvent;
		
		int				m_iMoveCount;
		DWORD			m_dwPlayStartTime;
		BYTE			m_bAddChrState;
		bool			m_bSkipSave;
		std::string		m_stMobile;
		char			m_szMobileAuth[5];
		BYTE			m_bChatCounter;
#ifdef UNLOCK_INVENTORY
		DWORD			m_dwUnlockSlots;
#endif
		// End of Basic Points

		//////////////////////////////////////////////////////////////////////////////////
		// Move & Synchronize Positions
		//////////////////////////////////////////////////////////////////////////////////
	public:
		bool			IsStateMove() const			{ return IsState((CState&)m_stateMove); }
		bool			IsStateIdle() const			{ return IsState((CState&)m_stateIdle); }
		bool			IsWalking() const			{ return m_bNowWalking || GetStamina()<=0; }
		void			SetWalking(bool bWalkFlag)	{ m_bWalking=bWalkFlag; }
		void			SetNowWalking(bool bWalkFlag);
		void			ResetWalking()			{ SetNowWalking(m_bWalking); }

		bool			Goto(long x, long y);	// ¹Ù·Î ÀÌµ¿ ½ÃÅ°Áö ¾Ê°í ¸ñÇ¥ À§Ä¡·Î BLENDING ½ÃÅ²´Ù.
		void			Stop();

		bool			CanMove() const;		// ÀÌµ¿ÇÒ ¼ö ÀÖ´Â°¡?

		void			SyncPacket();
		bool			Sync(long x, long y);	// ½ÇÁ¦ ÀÌ ¸Þ¼Òµå·Î ÀÌµ¿ ÇÑ´Ù (°¢ Á¾ Á¶°Ç¿¡ ÀÇÇÑ ÀÌµ¿ ºÒ°¡°¡ ¾øÀ½)
		bool			Move(long x, long y);	// Á¶°ÇÀ» °Ë»çÇÏ°í Sync ¸Þ¼Òµå¸¦ ÅëÇØ ÀÌµ¿ ÇÑ´Ù.
		void			OnMove(bool bIsAttack = false);	// ¿òÁ÷ÀÏ¶§ ºÒ¸°´Ù. Move() ¸Þ¼Òµå ÀÌ¿Ü¿¡¼­µµ ºÒ¸± ¼ö ÀÖ´Ù.
		DWORD			GetMotionMode() const;
		float			GetMoveMotionSpeed() const;
		float			GetMoveSpeed() const;
		void			CalculateMoveDuration();
		void			SendMovePacket(BYTE bFunc, BYTE bArg, DWORD x, DWORD y, DWORD dwDuration, DWORD dwTime=0, int iRot=-1);
#ifdef ENABLE_SUPPORT_SYSTEM
		void			SendSupportSkillPacket(DWORD skill_vnum, DWORD skill_level);
		DWORD			SupportGetNextExp() const;
#endif
		DWORD			GetCurrentMoveDuration() const	{ return m_dwMoveDuration; }
		DWORD			GetWalkStartTime() const	{ return m_dwWalkStartTime; }
		DWORD			GetLastMoveTime() const		{ return m_dwLastMoveTime; }
		DWORD			GetLastAttackTime() const	{ return m_dwLastAttackTime; }
		DWORD 			GetLastSkillTime() const {return m_dwLastSkillTime;}
		int				GetLastFishingTime() const	{ return m_iLastFishingTime; }
		void			SetLastFishingTime() { m_iLastFishingTime = thecore_pulse(); }

		void			SetLastAttacked(DWORD time);	// ¸¶Áö¸·À¸·Î °ø°Ý¹ÞÀº ½Ã°£ ¹× À§Ä¡¸¦ ÀúÀåÇÔ

		DWORD			GetLastAttackedTime() const	{ return m_dwLastAttackedTime;}

		bool			SetSyncOwner(LPCHARACTER ch, bool bRemoveFromList = true);
		bool			IsSyncOwner(LPCHARACTER ch) const;

		bool			WarpSet(long x, long y, long lRealMapIndex = 0, WORD port = 0);
		void			SetWarpLocation(long lMapIndex, long x, long y);
		void			WarpEnd();

		const PIXEL_POSITION & GetWarpPosition() const { return m_posWarp; }
		bool			WarpToPID(DWORD dwPID);

		void			SaveExitLocation();
		void			ExitToSavedLocation();

		void			StartStaminaConsume();
		void			StopStaminaConsume();
		bool			IsStaminaConsume() const;
		bool			IsStaminaHalfConsume() const;

		void			ResetStopTime();
		DWORD			GetStopTime() const;

	protected:
		void			ClearSync();

		float			m_fSyncTime;
		LPCHARACTER		m_pkChrSyncOwner;
		CHARACTER_LIST	m_kLst_pkChrSyncOwned;	// ³»°¡ SyncOwnerÀÎ ÀÚµé

		PIXEL_POSITION	m_posDest;
		PIXEL_POSITION	m_posStart;
		PIXEL_POSITION	m_posWarp;
		long			m_lWarpMapIndex;

		PIXEL_POSITION	m_posExit;
		long			m_lExitMapIndex;

		DWORD			m_dwMoveStartTime;
		DWORD			m_dwMoveDuration;

		DWORD			m_dwLastMoveTime;
		DWORD			m_dwLastAttackTime;
		DWORD			m_dwWalkStartTime;
		DWORD			m_dwLastAttackedTime;
		DWORD			m_dwStopTime;
		int				m_iLastFishingTime;

		bool			m_bWalking;
		bool			m_bNowWalking;
		bool			m_bStaminaConsume;
		// End

		// Quickslot °ü·Ã
	public:
		void			SyncQuickslot(BYTE bType, WORD bOldPos, WORD bNewPos);
		bool			GetQuickslot(BYTE pos, TQuickslot ** ppSlot);
		bool			SetQuickslot(BYTE pos, TQuickslot & rSlot);
		bool			DelQuickslot(BYTE pos);
		bool			SwapQuickslot(BYTE a, BYTE b);
		void			ChainQuickslotItem(LPITEM pItem, BYTE bType, WORD bOldPos);
#ifdef ENABLE_IKASHOP_RENEWAL
	private:
		// watcher
		using ShopActionWeight = uint16_t;
		using ShopActionTimestamp = double;
		using ShopActionData = std::pair<ShopActionWeight, ShopActionTimestamp>;
		using ShopActionRegister = std::deque<ShopActionData>;


	public:
		using SHOP_HANDLE = std::shared_ptr<ikashop::CShop>;
		using SAFEBOX_HANDLE = std::shared_ptr<ikashop::CShopSafebox>;
		using AUCTION_HANDLE = std::shared_ptr<ikashop::CAuction>;

		SHOP_HANDLE GetIkarusShop() {return m_pkIkarusShop;}
		void SetIkarusShop(SHOP_HANDLE pkShop) {m_pkIkarusShop = pkShop;}
		
		SHOP_HANDLE	GetIkarusShopGuest() const {return m_pkIkarusShopGuest;}
		void SetIkarusShopGuest(SHOP_HANDLE pkShop);
		
		SAFEBOX_HANDLE GetIkarusShopSafebox() {return m_pkIkarusShopSafebox;}
		void SetIkarusShopSafebox(SAFEBOX_HANDLE pk);

		void SetIkarusAuction(AUCTION_HANDLE pk) {m_pkIkarusAuction = pk;}
		void SetIkarusAuctionGuest(AUCTION_HANDLE pk);

		AUCTION_HANDLE GetIkarusAuction() {return m_pkIkarusAuction;}
		AUCTION_HANDLE GetIkarusAuctionGuest() const {return m_pkIkarusAuctionGuest;}

		// can warp check
		int GetIkarusShopUseTime() const { return m_iIkarusShopUseTime; }
		void SetIkarusShopUseTime() { m_iIkarusShopUseTime = thecore_pulse(); }

#ifdef USE_IKASHOP_UI_RENEWED
		int GetIkarusOpenShopTime() const { return m_iIkarusOpenShopTime; }
		void SetIkarusOpenShopTime() { m_iIkarusOpenShopTime = thecore_pulse(); }
#endif

		// looking offerlist
		void SetLookingIkarusShopOfferList(bool state) { m_bIsLookingIkarusShopOfferList = state; }
		bool IsLookingIkarusShopOfferList() const { return m_bIsLookingIkarusShopOfferList; }
		
		// looking shop
		void SetLookingShopOwner(bool state) { m_bIsLookingShopOwner  = state;}
		bool IsLookingShopOwner() const { return m_bIsLookingShopOwner; }
		
		// looking auction
		void SetLookingAuctionOwner(bool state) { m_bIsLookingAuctionOwner = state;}
		bool IsLookingAuctionOwner() const { return m_bIsLookingAuctionOwner; }

#ifdef EXTEND_IKASHOP_PRO
		void SetLookingNotificationList(bool state) { m_bIsLookingNotificationList = state; }
		bool IsLookingNotificationList() const { return m_bIsLookingNotificationList; }
#endif

		bool CanTakeInventoryItem(LPITEM item, TItemPos* pos);

		//watcher
		bool IkarusShopFloodCheck(ShopActionWeight weight);

	private:
		//watcher
		ShopActionRegister m_IkarusShopActionRegister;

		// handles
		SHOP_HANDLE	m_pkIkarusShop;
		SHOP_HANDLE	m_pkIkarusShopGuest;
		SAFEBOX_HANDLE m_pkIkarusShopSafebox;
		AUCTION_HANDLE m_pkIkarusAuction;
		AUCTION_HANDLE m_pkIkarusAuctionGuest;

		int m_iIkarusShopUseTime = 0;
#ifdef USE_IKASHOP_UI_RENEWED
		int m_iIkarusOpenShopTime = 0;
#endif
		bool m_bIsLookingIkarusShopOfferList = false;
		bool m_bIsLookingShopOwner = false;
		bool m_bIsLookingAuctionOwner = false;
#ifdef EXTEND_IKASHOP_PRO
		bool m_bIsLookingNotificationList = false;
#endif
#endif

		////////////////////////////////////////////////////////////////////////////////////////
		// Affect
	public:
		void			StartAffectEvent();
		void			ClearAffect(bool bSave=false, bool bSomeAffect = false);
		void			ComputeAffect(CAffect * pkAff, bool bAdd);
		bool			AddAffect(DWORD dwType, BYTE bApplyOn, long lApplyValue, DWORD dwFlag, long lDuration, long lSPCost, bool bOverride, bool IsCube = false
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
, DWORD dwFlag2 = 0
//#endif
		);
		void			RefreshAffect();
		bool			RemoveAffect(DWORD dwType);
		bool			IsAffectFlag(DWORD dwAff) const;

		bool			UpdateAffect();	// called from EVENT
		int				ProcessAffect();

		void			LoadAffect(DWORD dwCount, TPacketAffectElement * pElements);
		void			SaveAffect();

		// Affect loadingÀÌ ³¡³­ »óÅÂÀÎ°¡?
		bool			IsLoadedAffect() const	{ return m_bIsLoadedAffect; }

		bool			IsGoodAffect(BYTE bAffectType) const;

		void			RemoveGoodAffect();
		void			RemoveBadAffect();

		CAffect *		FindAffect(DWORD dwType, BYTE bApply=APPLY_NONE) const;
		const std::list<CAffect *> & GetAffectContainer() const	{ return m_list_pkAffect; }
		bool RemoveAffect(CAffect* pkAff);

	protected:
		bool			m_bIsLoadedAffect;
		TAffectFlag		m_afAffectFlag;
		std::list<CAffect *>	m_list_pkAffect;

	public:
		// PARTY_JOIN_BUG_FIX
		void			SetParty(LPPARTY pkParty);
		LPPARTY			GetParty() const	{ return m_pkParty; }

		bool			RequestToParty(LPCHARACTER leader);
		void			DenyToParty(LPCHARACTER member);
		void			AcceptToParty(LPCHARACTER member);

		/// ÀÚ½ÅÀÇ ÆÄÆ¼¿¡ ´Ù¸¥ character ¸¦ ÃÊ´ëÇÑ´Ù.
		/**
		 * @param	pchInvitee ÃÊ´ëÇÒ ´ë»ó character. ÆÄÆ¼¿¡ Âü¿© °¡´ÉÇÑ »óÅÂÀÌ¾î¾ß ÇÑ´Ù.
		 *
		 * ¾çÃø character ÀÇ »óÅÂ°¡ ÆÄÆ¼¿¡ ÃÊ´ëÇÏ°í ÃÊ´ë¹ÞÀ» ¼ö ÀÖ´Â »óÅÂ°¡ ¾Æ´Ï¶ó¸é ÃÊ´ëÇÏ´Â Ä³¸¯ÅÍ¿¡°Ô ÇØ´çÇÏ´Â Ã¤ÆÃ ¸Þ¼¼Áö¸¦ Àü¼ÛÇÑ´Ù.
		 */
		void			PartyInvite(LPCHARACTER pchInvitee);

		/// ÃÊ´ëÇß´ø character ÀÇ ¼ö¶ôÀ» Ã³¸®ÇÑ´Ù.
		/**
		 * @param	pchInvitee ÆÄÆ¼¿¡ Âü¿©ÇÒ character. ÆÄÆ¼¿¡ Âü¿©°¡´ÉÇÑ »óÅÂÀÌ¾î¾ß ÇÑ´Ù.
		 *
		 * pchInvitee °¡ ÆÄÆ¼¿¡ °¡ÀÔÇÒ ¼ö ÀÖ´Â »óÈ²ÀÌ ¾Æ´Ï¶ó¸é ÇØ´çÇÏ´Â Ã¤ÆÃ ¸Þ¼¼Áö¸¦ Àü¼ÛÇÑ´Ù.
		 */
		void			PartyInviteAccept(LPCHARACTER pchInvitee);

		/// ÃÊ´ëÇß´ø character ÀÇ ÃÊ´ë °ÅºÎ¸¦ Ã³¸®ÇÑ´Ù.
		/**
		 * @param [in]	dwPID ÃÊ´ë Çß´ø character ÀÇ PID
		 */
		void			PartyInviteDeny(DWORD dwPID);

		bool			BuildUpdatePartyPacket(TPacketGCPartyUpdate & out);
		int				GetLeadershipSkillLevel() const;

		bool			CanSummon(int iLeaderShip);

		void			SetPartyRequestEvent(LPEVENT pkEvent) { m_pkPartyRequestEvent = pkEvent; }

	protected:

		/// ÆÄÆ¼¿¡ °¡ÀÔÇÑ´Ù.
		/**
		 * @param	pkLeader °¡ÀÔÇÒ ÆÄÆ¼ÀÇ ¸®´õ
		 */
		void			PartyJoin(LPCHARACTER pkLeader);

		/**
		 * ÆÄÆ¼ °¡ÀÔÀ» ÇÒ ¼ö ¾øÀ» °æ¿ìÀÇ ¿¡·¯ÄÚµå.
		 * Error code ´Â ½Ã°£¿¡ ÀÇÁ¸ÀûÀÎ°¡¿¡ µû¶ó º¯°æ°¡´ÉÇÑ(mutable) type °ú Á¤Àû(static) type À¸·Î ³ª´¶´Ù.
		 * Error code ÀÇ °ªÀÌ PERR_SEPARATOR º¸´Ù ³·À¸¸é º¯°æ°¡´ÉÇÑ type ÀÌ°í ³ôÀ¸¸é Á¤Àû type ÀÌ´Ù.
		 */
		enum PartyJoinErrCode {
			PERR_NONE		= 0,	///< Ã³¸®¼º°ø
			PERR_SERVER,			///< ¼­¹ö¹®Á¦·Î ÆÄÆ¼°ü·Ã Ã³¸® ºÒ°¡
			PERR_DUNGEON,			///< Ä³¸¯ÅÍ°¡ ´øÀü¿¡ ÀÖÀ½
			PERR_OBSERVER,			///< °üÀü¸ðµåÀÓ
			PERR_LVBOUNDARY,		///< »ó´ë Ä³¸¯ÅÍ¿Í ·¹º§Â÷ÀÌ°¡ ³²
			PERR_LOWLEVEL,			///< »ó´ëÆÄÆ¼ÀÇ ÃÖ°í·¹º§º¸´Ù 30·¹º§ ³·À½
			PERR_HILEVEL,			///< »ó´ëÆÄÆ¼ÀÇ ÃÖÀú·¹º§º¸´Ù 30·¹º§ ³ôÀ½
			PERR_ALREADYJOIN,		///< ÆÄÆ¼°¡ÀÔ ´ë»ó Ä³¸¯ÅÍ°¡ ÀÌ¹Ì ÆÄÆ¼Áß
			PERR_PARTYISFULL,		///< ÆÄÆ¼ÀÎ¿ø Á¦ÇÑ ÃÊ°ú
			PERR_SEPARATOR,			///< Error type separator.
			PERR_DIFFEMPIRE,		///< »ó´ë Ä³¸¯ÅÍ¿Í ´Ù¸¥ Á¦±¹ÀÓ
			PERR_MAX				///< Error code ÃÖ°íÄ¡. ÀÌ ¾Õ¿¡ Error code ¸¦ Ãß°¡ÇÑ´Ù.
		};

		/// ÆÄÆ¼ °¡ÀÔÀÌ³ª °á¼º °¡´ÉÇÑ Á¶°ÇÀ» °Ë»çÇÑ´Ù.
		/**
		 * @param 	pchLeader ÆÄÆ¼ÀÇ leader ÀÌ°Å³ª ÃÊ´ëÇÑ character
		 * @param	pchGuest ÃÊ´ë¹Þ´Â character
		 * @return	¸ðµç PartyJoinErrCode °¡ ¹ÝÈ¯µÉ ¼ö ÀÖ´Ù.
		 */
		static PartyJoinErrCode	IsPartyJoinableCondition(const LPCHARACTER pchLeader, const LPCHARACTER pchGuest);

		/// ÆÄÆ¼ °¡ÀÔÀÌ³ª °á¼º °¡´ÉÇÑ µ¿ÀûÀÎ Á¶°ÇÀ» °Ë»çÇÑ´Ù.
		/**
		 * @param 	pchLeader ÆÄÆ¼ÀÇ leader ÀÌ°Å³ª ÃÊ´ëÇÑ character
		 * @param	pchGuest ÃÊ´ë¹Þ´Â character
		 * @return	mutable type ÀÇ code ¸¸ ¹ÝÈ¯ÇÑ´Ù.
		 */
		static PartyJoinErrCode	IsPartyJoinableMutableCondition(const LPCHARACTER pchLeader, const LPCHARACTER pchGuest);

		LPPARTY			m_pkParty;
		DWORD			m_dwLastDeadTime;
		LPEVENT			m_pkPartyRequestEvent;

		/**
		 * ÆÄÆ¼ÃÊÃ» Event map.
		 * key: ÃÊ´ë¹ÞÀº Ä³¸¯ÅÍÀÇ PID
		 * value: eventÀÇ pointer
		 *
		 * ÃÊ´ëÇÑ Ä³¸¯ÅÍµé¿¡ ´ëÇÑ event map.
		 */
		typedef std::map< DWORD, LPEVENT >	EventMap;
		EventMap		m_PartyInviteEventMap;

		// END_OF_PARTY_JOIN_BUG_FIX

		////////////////////////////////////////////////////////////////////////////////////////
		// Dungeon
	public:
		void			SetDungeon(LPDUNGEON pkDungeon);
		LPDUNGEON		GetDungeon() const	{ return m_pkDungeon; }
		LPDUNGEON		GetDungeonForce() const;
	protected:
		LPDUNGEON	m_pkDungeon;
		int			m_iEventAttr;

		////////////////////////////////////////////////////////////////////////////////////////
		// Guild
	public:
		void			SetGuild(CGuild * pGuild);
		CGuild*			GetGuild() const	{ return m_pGuild; }

		void			SetWarMap(CWarMap* pWarMap);
		CWarMap*		GetWarMap() const	{ return m_pWarMap; }

	protected:
		CGuild *		m_pGuild;
		DWORD			m_dwUnderGuildWarInfoMessageTime;
		CWarMap *		m_pWarMap;

		////////////////////////////////////////////////////////////////////////////////////////
		// Item related
	public:
		bool CanHandleItem(bool bSkipRefineCheck = false, bool bSkipObserver = false, bool bSkipDS = false
//#ifdef USE_CRYSTAL_SYSTEM
, bool bSkipShardCraft = false
//#endif
		);

		bool			IsItemLoaded() const	{ return m_bItemLoaded; }
		void			SetItemLoaded()	{ m_bItemLoaded = true; }

		void			ClearItem();
		
		void			SetItem(TItemPos Cell, LPITEM item, bool highlight = false);

		LPITEM			GetItem(TItemPos Cell) const;
		LPITEM			GetInventoryItem(WORD wCell) const;
		bool			IsEmptyItemGrid(TItemPos Cell, BYTE size, int iExceptionCell = -1) const;
#ifdef ENABLE_SPECIAL_STORAGE
		LPITEM			GetUpgradeInventoryItem(WORD wCell) const;
		LPITEM			GetBookInventoryItem(WORD wCell) const;
		LPITEM			GetStoneInventoryItem(WORD wCell) const;
		LPITEM			GetChangeInventoryItem(WORD wCell) const;
		LPITEM			GetCostumeInventoryItem(WORD wCell) const;
		LPITEM			GetChestInventoryItem(WORD wCell) const;
#endif
		void			SetWear(WORD wCell, LPITEM item);
		LPITEM			GetWear(WORD wCell) const;


		bool			UseItemEx(LPITEM item, TItemPos DestCell);
		bool			UseItem(TItemPos Cell, TItemPos DestCell = NPOS);

#ifdef ENABLE_SHOW_CHEST_DROP
		void			SendChestData(DWORD chestIndex);
#endif

		// ADD_REFINE_BUILDING
		bool			IsRefineThroughGuild() const;
		CGuild *		GetRefineGuild() const;
		int				ComputeRefineFee(int iCost, int iMultiply = 5) const;
		void			PayRefineFee(int iTotalMoney);
		void			SetRefineNPC(LPCHARACTER ch);
		// END_OF_ADD_REFINE_BUILDING

		bool			RefineItem(LPITEM pkItem, LPITEM pkTarget);
		bool			DropItem(TItemPos Cell,  DWORD bCount=0);
#ifdef ENABLE_SELL_ITEM
		bool			SellItem(TItemPos Cell);
#endif
		bool			GiveRecallItem(LPITEM item);
		void			ProcessRecallItem(LPITEM item);

		//	void			PotionPacket(int iPotionType);
		void			EffectPacket(int32_t enumEffectType, LPDESC specificDesc = nullptr);
		void			SpecificEffectPacket(const std::string& stEffectName);

		bool DoRefine(LPITEM item, bool bMoneyOnly = false
#ifdef ENABLE_PITTY_REFINE
, bool bUseSealOfGod = false
#endif
		);

		bool DoRefineWithScroll(LPITEM item
#ifdef ENABLE_PITTY_REFINE
, bool bUseSealOfGod = false
#endif
		);

#ifdef ENABLE_QUEEN_NETHIS
		bool DoRefineSerpent(LPITEM item
#ifdef ENABLE_PITTY_REFINE
, bool bUseSealOfGod = false
#endif
		);
#endif

		bool RefineInformation(WORD wCell, BYTE bType, int iAdditionalCell = -1
#ifdef ENABLE_SPECIAL_STORAGE
, BYTE bSpecialStorage = INVENTORY
#endif
		);

		void			SetRefineMode(int iAdditionalCell = -1);
		void			ClearRefineMode();

		bool			GiveItem(LPCHARACTER victim, TItemPos Cell);
		bool			CanReceiveItem(LPCHARACTER from, LPITEM item) const;
		void			ReceiveItem(LPCHARACTER from, LPITEM item);
		bool			GiveItemFromSpecialItemGroup(DWORD dwGroupNum, std::vector <DWORD> &dwItemVnums,
							std::vector <DWORD> &dwItemCounts, std::vector <LPITEM> &item_gets, int &count);
		
#ifdef ENABLE_SHOW_CHEST_DROP
		void			OpenChest(LPITEM item, WORD openCount);
		bool			GiveItemFromSpecialItemGroupNew(DWORD dwGroupNum, WORD& loopCount);
		bool			AutoGiveItemChest(LPITEM item, DWORD& itemCount, std::map<LPITEM, WORD>& vecUpdateItems);
#endif

		bool			MoveItem(TItemPos pos, TItemPos change_pos, DWORD num, bool isFromCommand = false);
		bool			CanGetGroundItem(LPITEM item, LPCHARACTER partyOwner);
		bool			PickupItem(DWORD vid);
		bool			EquipItem(LPITEM item, int iCandidateCell = -1);
		bool			UnequipItem(LPITEM item);

		// ÇöÀç itemÀ» Âø¿ëÇÒ ¼ö ÀÖ´Â Áö È®ÀÎÇÏ°í, ºÒ°¡´É ÇÏ´Ù¸é Ä³¸¯ÅÍ¿¡°Ô ÀÌÀ¯¸¦ ¾Ë·ÁÁÖ´Â ÇÔ¼ö
		bool			CanEquipNow(const LPITEM item, const TItemPos& srcCell = NPOS, const TItemPos& destCell = NPOS);

		// Âø¿ëÁßÀÎ itemÀ» ¹þÀ» ¼ö ÀÖ´Â Áö È®ÀÎÇÏ°í, ºÒ°¡´É ÇÏ´Ù¸é Ä³¸¯ÅÍ¿¡°Ô ÀÌÀ¯¸¦ ¾Ë·ÁÁÖ´Â ÇÔ¼ö
		bool			CanUnequipNow(const LPITEM item, const TItemPos& srcCell = NPOS, const TItemPos& destCell = NPOS);

		bool			SwapItem(BYTE bWindow, WORD wCell, BYTE bDestWindow, WORD wDestCell);
		LPITEM			AutoGiveItem(DWORD dwItemVnum, DWORD bCount=1, int iRarePct = -1, bool bMsg = true);
		void			AutoGiveItem(LPITEM item, bool longOwnerShip = false);

		int				GetEmptyInventory(BYTE size) const;
		int				GetEmptyDragonSoulInventory(LPITEM pItem) const;
		void			CopyDragonSoulItemGrid(std::vector<WORD>& vDragonSoulItemGrid) const;
#ifdef ENABLE_SPECIAL_STORAGE
		int				GetEmptyUpgradeInventory(LPITEM pItem) const;
		int				GetEmptyBookInventory(LPITEM pItem) const;
		int				GetEmptyStoneInventory(LPITEM pItem) const;
		int				GetEmptyChangeInventory(LPITEM pItem) const;
		int				GetEmptyCostumeInventory(LPITEM pItem) const;
		int				GetEmptyChestInventory(LPITEM pItem) const;
#endif
		int				CountEmptyInventory() const;
#ifdef ENABLE_BUY_ITEMS_WORLDARD
		int				CountSpecifyItemBuy(DWORD vnum) const;	
#endif
		int				CountSpecifyItem(DWORD vnum) const;
		void			RemoveSpecifyItem(DWORD vnum, DWORD count = 1);
		LPITEM			FindSpecifyItem(DWORD vnum) const;
		LPITEM			FindItemByID(DWORD id) const;

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
		DWORD			GetSwitchbotItemSwitcherUsed(WORD wCell);
		void			SetSwitchbotItemSwitcherUsed(WORD wCell, DWORD dwCount);

		void			SetSwitchbotItem(WORD wCell, WORD wCellInven);
		
		void			ItemSaveSwitchbot();
		void			ItemLoadSwitchbot();
		
		bool			m_bIsNeedSwitchbotSave;
#endif

		int				CountSpecifyTypeItem(BYTE type) const;
		void			RemoveSpecifyTypeItem(BYTE type, DWORD count = 1);

		bool			IsEquipUniqueItem(DWORD dwItemVnum) const;

		// CHECK_UNIQUE_GROUP
		bool			IsEquipUniqueGroup(DWORD dwGroupVnum) const;
		// END_OF_CHECK_UNIQUE_GROUP

		void			SendEquipment(LPCHARACTER ch);
		// End of Item

	protected:

		bool			m_bItemLoaded;
		int				m_iRefineAdditionalCell;
		bool			m_bUnderRefine;
		DWORD			m_dwRefineNPCVID;

	public:
		////////////////////////////////////////////////////////////////////////////////////////
		// Money related
		/*
		INT				GetGold() const		{ return m_points.gold;	}
		void			SetGold(INT gold)	{ m_points.gold = gold;	}
		bool			DropGold(INT gold);
		INT				GetAllowedGold() const;
		void			GiveGold(INT iAmount);	// ÆÄÆ¼°¡ ÀÖÀ¸¸é ÆÄÆ¼ ºÐ¹è, ·Î±× µîÀÇ Ã³¸®
		// End of Money*/

		long long		GetGold() const		{ return m_points.gold;	}
		void			SetGold(long long gold)	{ m_points.gold = gold;	}
		bool			DropGold(INT gold);
		long long		GetAllowedGold() const;
		void			GiveGold(long long iAmount);	// ??? ??? ?? ??, ?? ?? ??

#ifdef ENABLE_SORT_INVENTORY
        void            SetNextSortInventoryPulse(int pulse) { m_sortInventoryPulse = pulse; }
        int             GetSortInventoryPulse() { return m_sortInventoryPulse; }
       
        void            SetNextSortSpecialStoragePulse(int pulse) { m_sortSpecialStoragePulse = pulse; }
        int             GetSortSpecialStoragePulse() { return m_sortSpecialStoragePulse; }
#endif
#ifdef UNLOCK_INVENTORY
		DWORD 			GetUnlockSlots() const { return m_dwUnlockSlots;}
		void			SetUnlockSlots(DWORD slots) {m_dwUnlockSlots = slots;}
#endif
#ifdef ENABLE_MOUNT_SYSTEM
		////////////////////////////////
		// Mount system
	public:
		void SetMountingM(bool bValue) { bMounting = bValue; }
		bool GetMountVnumM() { return bMounting; }
		DWORD GetMountingVnumM() const { return bMountVnumM; }
		void SetMountVnumM(DWORD bVnum) { bMountVnumM = bVnum; }
		void SetSummonMounting(bool bValue) {bMountingS = bValue;}
		bool GetSummonMounting() {return bMountingS;}
		
	private:
		bool bMounting;
		bool bMountingS;
		DWORD bMountVnumM;
	
#endif
		////////////////////////////////////////////////////////////////////////////////////////
		// Shop related
	public:
		void			SetShop(LPSHOP pkShop);
		LPSHOP			GetShop() const { return m_pkShop; }

	protected:
		LPSHOP			m_pkShop;

		// End of shop
#ifdef ENABLE_SORT_INVENTORY
        int     m_sortInventoryPulse;
        int     m_sortSpecialStoragePulse;
#endif
		////////////////////////////////////////////////////////////////////////////////////////
		// Exchange related
	public:
		bool			ExchangeStart(LPCHARACTER victim);
		void			SetExchange(CExchange * pkExchange);
		CExchange *		GetExchange() const	{ return m_pkExchange;	}

	protected:
		CExchange *		m_pkExchange;
		// End of Exchange

		////////////////////////////////////////////////////////////////////////////////////////
		// Battle
	public:
		struct TBattleInfo
		{
			int iTotalDamage;
			int iAggro;

			TBattleInfo(int iTot, int iAggr)
				: iTotalDamage(iTot), iAggro(iAggr)
				{}
		};
		typedef std::map<VID, TBattleInfo>	TDamageMap;

		typedef struct SAttackLog
		{
			DWORD	dwVID;
			DWORD	dwTime;
		} AttackLog;

		bool				Damage(LPCHARACTER pAttacker, int dam, EDamageType type = DAMAGE_TYPE_NORMAL);
		void 				SetDamageMonsterHp(LPCHARACTER pAttacker, int dam, EDamageType type = DAMAGE_TYPE_NORMAL);
		bool 				GetDamageMonsterHp(LPCHARACTER pAttacker);

		bool				__Profile__Damage(LPCHARACTER pAttacker, int dam, EDamageType type = DAMAGE_TYPE_NORMAL);
		void				DeathPenalty(BYTE bExpLossPercent);
		void				ReviveInvisible(int iDur);

		bool				Attack(LPCHARACTER pkVictim, BYTE bType = 0);
		bool				IsAlive() const		{ return m_pointsInstant.position == POS_DEAD ? false : true; }
		bool				CanFight() const;

		bool				CanBeginFight() const;
		void				BeginFight(LPCHARACTER pkVictim); // pkVictimr°ú ½Î¿ì±â ½ÃÀÛÇÑ´Ù. (°­Á¦ÀûÀÓ, ½ÃÀÛÇÒ ¼ö ÀÖ³ª Ã¼Å©ÇÏ·Á¸é CanBeginFightÀ» »ç¿ë)

		bool				CounterAttack(LPCHARACTER pkChr); // ¹Ý°ÝÇÏ±â (¸ó½ºÅÍ¸¸ »ç¿ë)

		void				ClearWaitHackCounter(void) { waitHackCounter = 0; }
		void				SetWaitHackCounter(void) { waitHackCounter = 1; }
		int					GetWaitHackCounter(void) const { return waitHackCounter; }

		bool				IsStun() const;
		void				Stun();
		bool				IsDead() const;
		void				Dead(LPCHARACTER pkKiller = NULL, bool bImmediateDead=false);
#ifdef __NEWPET_SYSTEM__
		void				SetImmortal(int st) { m_stImmortalSt = st; };
		bool				IsImmortal() { return 1 == m_stImmortalSt; };
		void				SetNewPetSkillCD(int s, DWORD time) { m_newpetskillcd[s] = time; };
		DWORD				GetNewPetSkillCD(int s) { return m_newpetskillcd[s]; };
#endif
		void				Reward(bool bItemDrop);
#if defined(ENABLE_2LOOT)
		void				RewardGold(LPCHARACTER pkAttacker, DWORD dwMultiple);
#else
		void				RewardGold(LPCHARACTER pkAttacker);
#endif

		bool				Shoot(BYTE bType);
		void				FlyTarget(DWORD dwTargetVID, long x, long y, BYTE bHeader);

		void				ForgetMyAttacker(bool invisible = true);
		void				AggregateMonster();
		void				AttractRanger();
		void				PullMonster();
#ifdef ENABLE_SUNG_MAHI_TOWER
		void				AggregateMonsterByMaster();
#endif
		int					GetArrowAndBow(LPITEM * ppkBow, LPITEM * ppkArrow, int iArrowCount = 1);
		void				UseArrow(LPITEM pkArrow, DWORD dwArrowCount);

		void				AttackedByPoison(LPCHARACTER pkAttacker);
		void				RemovePoison();
#ifdef ENABLE_WOLFMAN_CHARACTER
		void				AttackedByBleeding(LPCHARACTER pkAttacker);
		void				RemoveBleeding();
#endif
		void				AttackedByFire(LPCHARACTER pkAttacker, int amount, int count);
		void				RemoveFire();

		void				UpdateAlignment(int iAmount);
		int					GetAlignment() const;

		//¼±¾ÇÄ¡ ¾ò±â
		int					GetRealAlignment() const;
		void				ShowAlignment(bool bShow);

		void				SetKillerMode(bool bOn);
		bool				IsKillerMode() const;
		void				UpdateKillerMode();

		BYTE				GetPKMode() const;
		void				SetPKMode(BYTE bPKMode);

		void				ItemDropPenalty(LPCHARACTER pkKiller);

		void				UpdateAggrPoint(LPCHARACTER ch, EDamageType type, int dam);

#ifdef ENABLE_NINJEYE
    private:
        void OnNinjEyeChecksFailed();
        void OnNinjEyeChecksSuccess();
        void InvalidateNinjEyeSession_Unsafe();

    public:
        void InvalidateNinjEyeSession();
        void StartNinjEyeSession();
        void OnNinjEyeResponse(struct TPacketCGNinjEyeResponse* Response);
        bool IsNinjEyeBlocked();

        void NinjEyeSendHeartbeatRequest(uint32_t EventSessionID);
        void NinjEyeCheckTimeout(uint32_t EventSessionID);

    private:
#ifdef ENABLE_NINJEYE_MULTITHREADED
        std::mutex NinjEyeMutex;
#endif
        bool       NinjEyeSessionStatus{ false };
        bool       NinjEyeBlockStatus{ false };
        bool       NinjEyeResponseStatus{ false };
        uint32_t   NinjEyeSessionID{ 0 };
        uint32_t   NinjEyeKey{ 0 };
        uint32_t   NinjEyeValidationRetryCount{ 0 };
        LPEVENT    NinjEyeDelayEvent{ nullptr };
        LPEVENT    NinjEyeTimeoutEvent{ nullptr };
#endif

		//
		// HACK
		//
	public:
		void SetComboSequence(BYTE seq);
		BYTE GetComboSequence() const;

		void SetLastComboTime(DWORD time);
		DWORD GetLastComboTime() const;

		int GetValidComboInterval() const;
		void SetValidComboInterval(int interval);

		BYTE GetComboIndex() const;

		void IncreaseComboHackCount(int k = 1);
		void ResetComboHackCount();
		void SkipComboAttackByTime(int interval);
		DWORD GetSkipComboAttackByTime() const;

	protected:
		BYTE m_bComboSequence;
		DWORD m_dwLastComboTime;
		int m_iValidComboInterval;
		BYTE m_bComboIndex;
		int m_iComboHackCount;
		DWORD m_dwSkipComboAttackByTime;

	protected:
		void				UpdateAggrPointEx(LPCHARACTER ch, EDamageType type, int dam, TBattleInfo & info);
		void				ChangeVictimByAggro(int iNewAggro, LPCHARACTER pNewVictim);

		DWORD				m_dwFlyTargetID;
		std::vector<DWORD>	m_vec_dwFlyTargets;
		std::set<DWORD>	m_dwFlyTargets;
		TDamageMap			m_map_kDamage;	// ¾î¶² Ä³¸¯ÅÍ°¡ ³ª¿¡°Ô ¾ó¸¶¸¸Å­ÀÇ µ¥¹ÌÁö¸¦ ÁÖ¾ú´Â°¡?
//		AttackLog			m_kAttackLog;
		DWORD				m_dwKillerPID;

		int					m_iAlignment;		// Lawful/Chaotic value -200000 ~ 200000
		int					m_iRealAlignment;
		int					m_iKillerModePulse;
		BYTE				m_bPKMode;

		// Aggro
		DWORD				m_dwLastVictimSetTime;
		int					m_iMaxAggro;
		// End of Battle

		// Stone
	public:
		void				SetStone(LPCHARACTER pkChrStone);
		void				ClearStone();
		void				DetermineDropMetinStone();
		DWORD				GetDropMetinStoneVnum() const { return m_dwDropMetinStone; }
		BYTE				GetDropMetinStonePct() const { return m_bDropMetinStonePct; }
		void				DetermineDropChestBooksMetin();
		DWORD				GetDropChestBookVnum() const { return m_dropChestBook; }
		BYTE				GetDropChestBookPct() const { return m_bDropChestBookPct; }

	protected:
		LPCHARACTER			m_pkChrStone;		// ³ª¸¦ ½ºÆùÇÑ µ¹
		CHARACTER_SET		m_set_pkChrSpawnedBy;	// ³»°¡ ½ºÆùÇÑ ³ðµé
		DWORD				m_dwDropMetinStone;
		BYTE				m_bDropMetinStonePct;
		DWORD				m_dropChestBook;
		BYTE				m_bDropChestBookPct;
		// End of Stone

	public:
		enum
		{
			SKILL_UP_BY_POINT,
			SKILL_UP_BY_BOOK,
			SKILL_UP_BY_TRAIN,

			// ADD_GRANDMASTER_SKILL
			SKILL_UP_BY_QUEST,
			// END_OF_ADD_GRANDMASTER_SKILL
		};

		void				SkillLevelPacket();
		void				SkillLevelUp(DWORD dwVnum, BYTE bMethod = SKILL_UP_BY_POINT);
		bool				SkillLevelDown(DWORD dwVnum);
		// ADD_GRANDMASTER_SKILL
		bool UseSkill(DWORD dwVnum, LPCHARACTER pkVictim, bool bUseGrandMaster = true
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, bool bFromAutoHunt = false
#endif
		);
		bool				IsSkillCooldown(DWORD dwVnum, float fSkillPower) { return m_SkillUseInfo[dwVnum].IsSkillCooldown(dwVnum, fSkillPower) ? true : false; }
		void				ResetSkill();
		void				SetSkillLevel(DWORD dwVnum, BYTE bLev);
		int					GetUsedSkillMasterType(DWORD dwVnum);

		bool				IsLearnableSkill(DWORD dwSkillVnum) const;
		// END_OF_ADD_GRANDMASTER_SKILL

		bool				CheckSkillHitCount(const BYTE SkillID, const VID dwTargetVID);
		bool CanUseSkill(DWORD dwSkillVnum
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, bool bFromAutoHunt = false
#endif
		) const;
		bool				IsUsableSkillMotion(DWORD dwMotionIndex) const;
		int					GetSkillLevel(DWORD dwVnum) const;
		int					GetSkillMasterType(DWORD dwVnum) const;
		int					GetSkillPower(DWORD dwVnum, BYTE bLevel = 0) const;

		time_t				GetSkillNextReadTime(DWORD dwVnum) const;
		void				SetSkillNextReadTime(DWORD dwVnum, time_t time);
		void				SkillLearnWaitMoreTimeMessage(DWORD dwVnum);

		void				ComputePassiveSkill(DWORD dwVnum);
		int ComputeSkill(DWORD dwVnum, LPCHARACTER pkVictim, BYTE bSkillLevel = 0
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, bool bFromAutoHunt = false
#endif
		);
// #ifdef ENABLE_WOLFMAN_CHARACTER
		int ComputeSkillParty(DWORD dwVnum, LPCHARACTER pkVictim, BYTE bSkillLevel = 0
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, bool bFromAutoHunt = false
#endif
		);
// #endif
		int					ComputeSkillAtPosition(DWORD dwVnum, const PIXEL_POSITION& posTarget, BYTE bSkillLevel = 0);
		void				ComputeSkillPoints();

		void				SetSkillGroup(BYTE bSkillGroup);
		BYTE				GetSkillGroup() const		{ return m_points.skill_group; }

		int					ComputeCooltime(int time);

		void				GiveRandomSkillBook();

		void				DisableCooltime();
		bool LearnSkillByBook(DWORD dwSkillVnum, BYTE bProb = 0
#ifdef USE_PASSIVE_ABILITY_TAMER
, bool bIsTamerSkill = false
#endif
		);
		bool				LearnGrandMasterSkill(DWORD dwSkillVnum);

	private:
		bool				m_bDisableCooltime;
		DWORD				m_dwLastSkillTime;	///< ¸¶Áö¸·À¸·Î skill À» ¾´ ½Ã°£(millisecond).
		// End of Skill

		// MOB_SKILL
	public:
		bool				HasMobSkill() const;
		size_t				CountMobSkill() const;
		const TMobSkillInfo * GetMobSkill(unsigned int idx) const;
		bool				CanUseMobSkill(unsigned int idx) const;
		bool				UseMobSkill(unsigned int idx);
		void				ResetMobSkillCooltime();
	protected:
		DWORD				m_adwMobSkillCooltime[MOB_SKILL_MAX_NUM];
		// END_OF_MOB_SKILL

		// for SKILL_MUYEONG
	public:
		void				StartMuyeongEvent();
		void				StopMuyeongEvent();
#ifdef ENABLE_YOHARA_SYSTEM
		void				StartCheonunEvent(long lApplyValue);
		void				StopCheonunEvent();
#endif

	private:
		LPEVENT				m_pkMuyeongEvent;
#ifdef ENABLE_YOHARA_SYSTEM
		LPEVENT				m_pkCheonunEvent;
#endif
		// for SKILL_CHAIN lighting
	public:
		int					GetChainLightningIndex() const { return m_iChainLightingIndex; }
		void				IncChainLightningIndex() { ++m_iChainLightingIndex; }
		void				AddChainLightningExcept(LPCHARACTER ch) { m_setExceptChainLighting.insert(ch); }
		void				ResetChainLightningIndex() { m_iChainLightingIndex = 0; m_setExceptChainLighting.clear(); }
		int					GetChainLightningMaxCount() const;
		const CHARACTER_SET& GetChainLightingExcept() const { return m_setExceptChainLighting; }

	private:
		int					m_iChainLightingIndex;
		CHARACTER_SET m_setExceptChainLighting;

		// for SKILL_EUNHYUNG
	public:
		void				SetAffectedEunhyung();
		void				ClearAffectedEunhyung() { m_dwAffectedEunhyungLevel = 0; }
		bool				GetAffectedEunhyung() const { return m_dwAffectedEunhyungLevel; }

	private:
		DWORD				m_dwAffectedEunhyungLevel;

		//
		// Skill levels
		//
	protected:
		TPlayerSkill*					m_pSkillLevels;
		boost::unordered_map<BYTE, int>		m_SkillDamageBonus;
		std::map<int, TSkillUseInfo>	m_SkillUseInfo;

		////////////////////////////////////////////////////////////////////////////////////////
		// AI related
	public:
		void			AssignTriggers(const TMobTable * table);
		LPCHARACTER		GetVictim() const;	// °ø°ÝÇÒ ´ë»ó ¸®ÅÏ
		void			SetVictim(LPCHARACTER pkVictim);
		LPCHARACTER		GetNearestVictim(LPCHARACTER pkChr);
		LPCHARACTER		GetProtege() const;	// º¸È£ÇØ¾ß ÇÒ ´ë»ó ¸®ÅÏ

		bool			Follow(LPCHARACTER pkChr, float fMinimumDistance = 150.0f);
		bool			Return();
		bool			IsGuardNPC() const;
		bool			IsChangeAttackPosition(LPCHARACTER target) const;
		void			ResetChangeAttackPositionTime() { m_dwLastChangeAttackPositionTime = get_dword_time() - AI_CHANGE_ATTACK_POISITION_TIME_NEAR;}
		void			SetChangeAttackPositionTime() { m_dwLastChangeAttackPositionTime = get_dword_time();}
#ifdef ENABLE_DEFENSAWE_SHIP
		bool			IsHydraMob() const;
		bool			IsHydraMobLP(LPCHARACTER ch) const;
		bool			IsHydraNPC() const;
		bool			IsHydra() const;
#endif

		bool			OnIdle();

		void			OnAttack(LPCHARACTER pkChrAttacker);
		void			OnClick(LPCHARACTER pkChrCauser);

		VID				m_kVIDVictim;

	protected:
		DWORD			m_dwLastChangeAttackPositionTime;
		CTrigger		m_triggerOnClick;
		// End of AI

		////////////////////////////////////////////////////////////////////////////////////////
		// Target
	protected:
		LPCHARACTER				m_pkChrTarget;		// ³» Å¸°Ù
		CHARACTER_SET	m_set_pkChrTargetedBy;	// ³ª¸¦ Å¸°ÙÀ¸·Î °¡Áö°í ÀÖ´Â »ç¶÷µé

	public:
		void				SetTarget(LPCHARACTER pkChrTarget);
		void				BroadcastTargetPacket();
		void				ClearTarget();
		void				CheckTarget();
		LPCHARACTER			GetTarget() const { return m_pkChrTarget; }

		////////////////////////////////////////////////////////////////////////////////////////
		// Safebox
	public:
		int					GetSafeboxSize() const;
		void				QuerySafeboxSize();
		void				SetSafeboxSize(int size);

		CSafebox *			GetSafebox() const;
		void				LoadSafebox(int iSize, DWORD dwGold, int iItemCount, TPlayerItem * pItems);
		void				ChangeSafeboxSize(BYTE bSize);
		void				CloseSafebox();

		/// Ã¢°í ¿­±â ¿äÃ»
		/**
		 * @param [in]	pszPassword 1ÀÚ ÀÌ»ó 6ÀÚ ÀÌÇÏÀÇ Ã¢°í ºñ¹Ð¹øÈ£
		 *
		 * DB ¿¡ Ã¢°í¿­±â¸¦ ¿äÃ»ÇÑ´Ù.
		 * Ã¢°í´Â Áßº¹À¸·Î ¿­Áö ¸øÇÏ¸ç, ÃÖ±Ù Ã¢°í¸¦ ´ÝÀº ½Ã°£À¸·Î ºÎÅÍ 10ÃÊ ÀÌ³»¿¡´Â ¿­ Áö ¸øÇÑ´Ù.
		 */
		void				ReqSafeboxLoad(const char* pszPassword);

		/// Ã¢°í ¿­±â ¿äÃ»ÀÇ Ãë¼Ò
		/**
		 * ReqSafeboxLoad ¸¦ È£ÃâÇÏ°í CloseSafebox ÇÏÁö ¾Ê¾ÒÀ» ¶§ ÀÌ ÇÔ¼ö¸¦ È£ÃâÇÏ¸é Ã¢°í¸¦ ¿­ ¼ö ÀÖ´Ù.
		 * Ã¢°í¿­±âÀÇ ¿äÃ»ÀÌ DB ¼­¹ö¿¡¼­ ½ÇÆÐÀÀ´äÀ» ¹Þ¾ÒÀ» °æ¿ì ÀÌ ÇÔ¼ö¸¦ »ç¿ëÇØ¼­ ¿äÃ»À» ÇÒ ¼ö ÀÖ°Ô ÇØÁØ´Ù.
		 */
		void				CancelSafeboxLoad( void ) { m_bOpeningSafebox = false; }

		void				SetMallLoadTime(int t) { m_iMallLoadTime = t; }
		int					GetMallLoadTime() const { return m_iMallLoadTime; }

		CSafebox *			GetMall() const;
		void				LoadMall(int iItemCount, TPlayerItem * pItems);
		void				CloseMall();

		void				SetSafeboxOpenPosition();
		float				GetDistanceFromSafeboxOpen() const;

	protected:
		CSafebox *			m_pkSafebox;
		int					m_iSafeboxSize;
		int					m_iSafeboxLoadTime;
		bool				m_bOpeningSafebox;	///< Ã¢°í°¡ ¿­±â ¿äÃ» ÁßÀÌ°Å³ª ¿­·ÁÀÖ´Â°¡ ¿©ºÎ, true ÀÏ °æ¿ì ¿­±â¿äÃ»ÀÌ°Å³ª ¿­·ÁÀÖÀ½.

		CSafebox *			m_pkMall;
		int					m_iMallLoadTime;

		PIXEL_POSITION		m_posSafeboxOpen;

		////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////
		// Mounting
	public:
		void				MountVnum(DWORD vnum);
		DWORD				GetMountVnum() const { return m_dwMountVnum; }
		DWORD				GetLastMountTime() const { return m_dwMountTime; }

		bool				CanUseHorseSkill();

		// Horse
		virtual	void		SetHorseLevel(int iLevel);

		virtual	bool		StartRiding();
		virtual	bool		StopRiding();

		virtual	DWORD		GetMyHorseVnum() const;

		virtual	void		HorseDie();
		virtual bool		ReviveHorse();

		virtual void		SendHorseInfo();
		virtual	void		ClearHorseInfo();

		void				HorseSummon(bool bSummon, bool bFromFar = false, DWORD dwVnum = 0, const char* name = 0);

		LPCHARACTER			GetHorse() const			{ return m_chHorse; }	 // ÇöÀç ¼ÒÈ¯ÁßÀÎ ¸»
		LPCHARACTER			GetRider() const; // rider on horse
		void				SetRider(LPCHARACTER ch);

		bool				IsRiding() const;
#ifdef ENABLE_SUPPORT_SYSTEM
	public:
		CSupportSystem*			GetSupportSystem()				{ return m_supportSystem; }

	protected:
		CSupportSystem*			m_supportSystem;

	public:
#endif 
#ifdef __AUTO_HUNT__
public:
	void			GetAutoHuntCommand(const char* szArgument);
	void			SetAutoHuntStatus(bool bStatus, bool fromEvent = false);
	bool			IsAutoHuntAffectHas();
#endif
#ifdef __PET_SYSTEM__
	public:
		CPetSystem*			GetPetSystem()				{ return m_petSystem; }

#ifdef ENABLE_NEW_PET_SYSTEM
		bool IsPet() const { return m_bCharType == CHAR_TYPE_PET; }
		void SetNewPet() { m_bIsNewPet = true; }
		bool IsNewPet() const { return m_bIsNewPet; }
		void StatePet();
		bool PetBlockMap();
#endif

	protected:
		CPetSystem*			m_petSystem;
#ifdef ENABLE_NEW_PET_SYSTEM
		bool m_bIsNewPet;
#endif
	public:
#endif
#ifdef __NEWPET_SYSTEM__
	public:
		CNewPetSystem*			GetNewPetSystem() { return m_newpetSystem; }

	protected:
		CNewPetSystem*			m_newpetSystem;

	public:
#endif
#ifdef ENABLE_MOUNT_SYSTEM
	public:
		CMountSystem*			GetMountSystem() { return m_MountSystem; }

	protected:
		CMountSystem*			m_MountSystem;

	public:
#endif
	protected:
		LPCHARACTER			m_chHorse;
		LPCHARACTER			m_chRider;

		DWORD				m_dwMountVnum;
		DWORD				m_dwMountTime;

		BYTE				m_bSendHorseLevel;
		BYTE				m_bSendHorseHealthGrade;
		BYTE				m_bSendHorseStaminaGrade;

		////////////////////////////////////////////////////////////////////////////////////////
		// Detailed Log
	public:
		void				DetailLog() { m_bDetailLog = !m_bDetailLog; }
		void				ToggleMonsterLog();
		void				MonsterLog(const char* format, ...);
	private:
		bool				m_bDetailLog;
		bool				m_bMonsterLog;

		////////////////////////////////////////////////////////////////////////////////////////
		// Empire

	public:
		void 				SetEmpire(BYTE bEmpire);
		BYTE				GetEmpire() const { return m_bEmpire; }

	protected:
		BYTE				m_bEmpire;

		////////////////////////////////////////////////////////////////////////////////////////
		// Regen
	public:
		void				SetRegen(LPREGEN pkRegen);

	protected:
		PIXEL_POSITION			m_posRegen;
		float				m_fRegenAngle;
		LPREGEN				m_pkRegen;
		size_t				regen_id_; // to help dungeon regen identification
		// End of Regen

		////////////////////////////////////////////////////////////////////////////////////////
		// Resists & Proofs
	public:
		bool				CannotMoveByAffect() const;	// Æ¯Á¤ È¿°ú¿¡ ÀÇÇØ ¿òÁ÷ÀÏ ¼ö ¾ø´Â »óÅÂÀÎ°¡?
		bool				IsImmune(DWORD dwImmuneFlag);
		void				SetImmuneFlag(DWORD dw) { m_pointsInstant.dwImmuneFlag = dw; }

	protected:
		void				ApplyMobAttribute(const TMobTable* table);
		// End of Resists & Proofs

		////////////////////////////////////////////////////////////////////////////////////////
		// QUEST
		//
	public:
		void				SetQuestNPCID(DWORD vid);
		DWORD				GetQuestNPCID() const { return m_dwQuestNPCVID; }
		LPCHARACTER			GetQuestNPC() const;

		void				SetQuestItemPtr(LPITEM item);
		void				ClearQuestItemPtr();
		LPITEM				GetQuestItemPtr() const;
#ifdef ENABLE_BIOLOG_SYSTEM
		int					GetBiologState(const std::string& state) const;
		void				SetBiologState(const std::string& state, int szValue);
#endif

		void				SetQuestBy(DWORD dwQuestVnum)	{ m_dwQuestByVnum = dwQuestVnum; }
		DWORD				GetQuestBy() const			{ return m_dwQuestByVnum; }

		int					GetQuestFlag(const std::string& flag) const;
		void				SetQuestFlag(const std::string& flag, int value);

		void				ConfirmWithMsg(const char* szMsg, int iTimeout, DWORD dwRequestPID);

	private:
		DWORD				m_dwQuestNPCVID;
		DWORD				m_dwQuestByVnum;
		LPITEM				m_pQuestItem;

		// Events
	public:
		bool				StartStateMachine(int iPulse = 1);
		void				StopStateMachine();
		void				UpdateStateMachine(DWORD dwPulse);
		void				SetNextStatePulse(int iPulseNext);

		// Ä³¸¯ÅÍ ÀÎ½ºÅÏ½º ¾÷µ¥ÀÌÆ® ÇÔ¼ö. ±âÁ¸¿£ ÀÌ»óÇÑ »ó¼Ó±¸Á¶·Î CFSM::Update ÇÔ¼ö¸¦ È£ÃâÇÏ°Å³ª UpdateStateMachine ÇÔ¼ö¸¦ »ç¿ëÇß´Âµ¥, º°°³ÀÇ ¾÷µ¥ÀÌÆ® ÇÔ¼ö Ãß°¡ÇÔ.
		void				UpdateCharacter(DWORD dwPulse);

	protected:
		DWORD				m_dwNextStatePulse;

		// Marriage
	public:
		LPCHARACTER			GetMarryPartner() const;
		void				SetMarryPartner(LPCHARACTER ch);
		int					GetMarriageBonus(DWORD dwItemVnum, bool bSum = true);

		void				SetWeddingMap(marriage::WeddingMap* pMap);
		marriage::WeddingMap* GetWeddingMap() const { return m_pWeddingMap; }

	private:
		marriage::WeddingMap* m_pWeddingMap;
		LPCHARACTER			m_pkChrMarried;

		// Warp Character
	public:
		void				StartWarpNPCEvent();
#ifdef CHANGE_CHANNEL
		void				ChannelSwitch(int new_ch);
#endif

	public:
		void				StartSaveEvent();
		void				StartRecoveryEvent();
		void				StartCheckSpeedHackEvent();
		void				StartDestroyWhenIdleEvent();

		LPEVENT				m_pkDeadEvent;
		LPEVENT				m_pkStunEvent;
		LPEVENT				m_pkSaveEvent;
		LPEVENT				m_pkRecoveryEvent;
		LPEVENT				m_pkTimedEvent;
		LPEVENT				m_pkFishingEvent;
		LPEVENT				m_pkAffectEvent;
		LPEVENT				m_pkPoisonEvent;
#ifdef ENABLE_WOLFMAN_CHARACTER
		LPEVENT				m_pkBleedingEvent;
#endif
		LPEVENT				m_pkFireEvent;
		LPEVENT				m_pkWarpNPCEvent;
		//DELAYED_WARP
		//END_DELAYED_WARP

		// MINING
		LPEVENT				m_pkMiningEvent;
		// END_OF_MINING
		LPEVENT				m_pkWarpEvent;
		LPEVENT				m_pkCheckSpeedHackEvent;
		LPEVENT				m_pkDestroyWhenIdleEvent;
		LPEVENT				m_pkPetSystemUpdateEvent;
#ifdef __NEWPET_SYSTEM__
		LPEVENT				m_pkNewPetSystemUpdateEvent;
		LPEVENT				m_pkNewPetSystemExpireEvent;
#endif

#ifdef ENABLE_MOUNT_SYSTEM
		LPEVENT				m_pkMountSystemUpdateEvent;
#endif
		bool IsWarping() const { return m_pkWarpEvent ? true : false; }

		bool				m_bHasPoisoned;
#ifdef ENABLE_WOLFMAN_CHARACTER
		bool				m_bHasBled;
#endif

		const CMob *		m_pkMobData;
		CMobInstance *		m_pkMobInst;

		std::map<int, LPEVENT> m_mapMobSkillEvent;

		friend struct FuncSplashDamage;
		friend struct FuncSplashAffect;
		friend class CFuncShoot;

	public:
		int				GetPremiumRemainSeconds(BYTE bType) const;

	private:
		int				m_aiPremiumTimes[PREMIUM_MAX_NUM];

		// CHANGE_ITEM_ATTRIBUTES
		// static const DWORD		msc_dwDefaultChangeItemAttrCycle;	///< µðÆúÆ® ¾ÆÀÌÅÛ ¼Ó¼ºº¯°æ °¡´É ÁÖ±â
		static const char		msc_szLastChangeItemAttrFlag[];		///< ÃÖ±Ù ¾ÆÀÌÅÛ ¼Ó¼ºÀ» º¯°æÇÑ ½Ã°£ÀÇ Quest Flag ÀÌ¸§
		// static const char		msc_szChangeItemAttrCycleFlag[];		///< ¾ÆÀÌÅÛ ¼Ó¼ºº´°æ °¡´É ÁÖ±âÀÇ Quest Flag ÀÌ¸§
		// END_OF_CHANGE_ITEM_ATTRIBUTES

		// PC_BANG_ITEM_ADD
	private :
		bool m_isinPCBang;

	public :
		bool SetPCBang(bool flag) { m_isinPCBang = flag; return m_isinPCBang; }
		bool IsPCBang() const { return m_isinPCBang; }
		// END_PC_BANG_ITEM_ADD

		// NEW_HAIR_STYLE_ADD
	public :
		bool ItemProcess_Hair(LPITEM item, int iDestCell);
		// END_NEW_HAIR_STYLE_ADD

	public :
		void ClearSkill();
		void ClearSubSkill();

		// RESET_ONE_SKILL
		bool ResetOneSkill(DWORD dwVnum);
		// END_RESET_ONE_SKILL

#ifdef ENABLE_PITTY_REFINE
	public:
		int					GetPittyRefineLevel(DWORD dwItemVnum, int iRefineLevel);
		void				SetPittyRefineLevel(DWORD dwItemVnum, int iRefineLevel, int iValue);

		void				SendPittyInfoClient(DWORD dwItemVnum, int iRefineLevel);
		
		void				SaveRefineFlags();
		void				LoadRefineFlags();

	protected:
		std::map<std::pair<int, int>, int> m_mapRefineFlags;
#endif

	private :
		void SendDamagePacket(LPCHARACTER pAttacker, int Damage, BYTE DamageFlag);

	// ARENA
	private :
		CArena *m_pArena;
		bool m_ArenaObserver;
		int m_nPotionLimit;

	public :
		void 	SetArena(CArena* pArena) { m_pArena = pArena; }
		void	SetArenaObserverMode(bool flag) { m_ArenaObserver = flag; }

		CArena* GetArena() const { return m_pArena; }
		bool	GetArenaObserverMode() const { return m_ArenaObserver; }

		void	SetPotionLimit(int count) { m_nPotionLimit = count; }
		int		GetPotionLimit() const { return m_nPotionLimit; }
	// END_ARENA

		//PREVENT_TRADE_WINDOW
	public:
		bool	IsOpenSafebox() const { return m_isOpenSafebox ? true : false; }
		void 	SetOpenSafebox(bool b) { m_isOpenSafebox = b; }

		int		GetSafeboxLoadTime() const { return m_iSafeboxLoadTime; }
		void	SetSafeboxLoadTime() { m_iSafeboxLoadTime = thecore_pulse(); }
		//END_PREVENT_TRADE_WINDOW
	private:
		bool	m_isOpenSafebox;

	public:
		int		GetSkillPowerByLevel(int level, bool bMob = false) const;

		//PREVENT_REFINE_HACK
		int		GetRefineTime() const { return m_iRefineTime; }
		void	SetRefineTime() { m_iRefineTime = thecore_pulse(); }
		int		m_iRefineTime;
		//END_PREVENT_REFINE_HACK

		//RESTRICT_USE_SEED_OR_MOONBOTTLE
		int 	GetUseSeedOrMoonBottleTime() const { return m_iSeedTime; }
		void  	SetUseSeedOrMoonBottleTime() { m_iSeedTime = thecore_pulse(); }
		int 	m_iSeedTime;
		//END_RESTRICT_USE_SEED_OR_MOONBOTTLE

		//PREVENT_PORTAL_AFTER_EXCHANGE
		int		GetExchangeTime() const { return m_iExchangeTime; }
		void	SetExchangeTime() { m_iExchangeTime = thecore_pulse(); }
		int		m_iExchangeTime;
		//END_PREVENT_PORTAL_AFTER_EXCHANGE

		int 	m_iMyShopTime;
		int		GetMyShopTime() const	{ return m_iMyShopTime; }
		void	SetMyShopTime() { m_iMyShopTime = thecore_pulse(); }

		// Hack ¹æÁö¸¦ À§ÇÑ Ã¼Å©.
		bool IsHack(bool bSendMsg = true, bool bCheckShopOwner = true, int limittime = g_nPortalLimitTime
//#ifdef USE_CRYSTAL_SYSTEM
, bool bSkipShardCraft = false
//#endif
		);

		// MONARCH
		BOOL	IsMonarch() const;
		// END_MONARCH
		void Say(const std::string & s);

		enum MONARCH_COOLTIME
		{
			MC_HEAL = 10,
			MC_WARP	= 60,
			MC_TRANSFER = 60,
			MC_TAX = (60 * 60 * 24 * 7),
			MC_SUMMON = (60 * 60),
		};

		enum MONARCH_INDEX
		{
			MI_HEAL = 0,
			MI_WARP,
			MI_TRANSFER,
			MI_TAX,
			MI_SUMMON,
			MI_MAX
		};

		DWORD m_dwMonarchCooltime[MI_MAX];
		DWORD m_dwMonarchCooltimelimit[MI_MAX];

		void  InitMC();
		DWORD GetMC(enum MONARCH_INDEX e) const;
		void SetMC(enum MONARCH_INDEX e);
		bool IsMCOK(enum MONARCH_INDEX e) const;
		DWORD GetMCL(enum MONARCH_INDEX e) const;
		DWORD GetMCLTime(enum MONARCH_INDEX e) const;

	public:
		bool ItemProcess_Polymorph(LPITEM item);

		bool IsCubeOpen () const
		{
			return (m_playerData && m_playerData->pCubeNpc) ? true : false;
		}

		void SetCubeNpc(LPCHARACTER npc)
		{
			if (m_playerData)
			{
				m_playerData->pCubeNpc = npc;
			}
		}

		bool CanDoCube() const;

	public:
		bool IsSiegeNPC() const;

	private:
		//Áß±¹ Àü¿ë
		//18¼¼ ¹Ì¸¸ Àü¿ë
		//3½Ã°£ : 50 % 5 ½Ã°£ 0%
		e_overtime m_eOverTime;

	public:
		bool IsOverTime(e_overtime e) const { return (e == m_eOverTime); }
		void SetOverTime(e_overtime e) { m_eOverTime = e; }

	private:
		int		m_deposit_pulse;

	public:
		void	UpdateDepositPulse();
		bool	CanDeposit() const;

	public:
		struct AttackedLog
		{
			DWORD 	dwPID;
			DWORD	dwAttackedTime;

			AttackedLog() : dwPID(0), dwAttackedTime(0)
			{
			}
		};

		AttackLog	m_kAttackLog;
		AttackedLog m_AttackedLog;
		int			m_speed_hack_count;

	private :
		std::string m_strNewName;

	public :
		const std::string GetNewName() const { return this->m_strNewName; }
		void SetNewName(const std::string name) { this->m_strNewName = name; }

	public :
		void GoHome();

	private :
		std::set<DWORD>	m_known_guild;

	public :
		void SendGuildName(CGuild* pGuild);
		void SendGuildName(DWORD dwGuildID);

	private :
		DWORD m_dwLogOffInterval;

	public :
		DWORD GetLogOffInterval() const { return m_dwLogOffInterval; }

	public:
		bool UnEquipSpecialRideUniqueItem ();

		bool CanWarp(bool isfromStorage = false
#ifdef USE_IKASHOP_UI_RENEWED
, bool bFromIkaShop = false
#endif
		) const;

	private:
		DWORD m_dwLastGoldDropTime;
#ifdef ENABLE_NEWSTUFF
		DWORD m_dwLastItemDropTime;
		DWORD m_dwLastBoxUseTime;
		DWORD m_dwLastBuySellTime;
	public:
		DWORD GetLastBuySellTime() const { return m_dwLastBuySellTime; }
		void SetLastBuySellTime(DWORD dwLastBuySellTime) { m_dwLastBuySellTime = dwLastBuySellTime; }
#endif

	public:
		void AutoRecoveryItemProcess (const EAffectTypes);

	public:
		void BuffOnAttr_AddBuffsFromItem(LPITEM pItem);
		void BuffOnAttr_RemoveBuffsFromItem(LPITEM pItem);

	private:
		void BuffOnAttr_ValueChange(BYTE bType, BYTE bOldValue, BYTE bNewValue);
		void BuffOnAttr_ClearAll();

		typedef std::map <BYTE, CBuffOnAttributes*> TMapBuffOnAttrs;
		TMapBuffOnAttrs m_map_buff_on_attrs;
		// ¹«Àû : ¿øÈ°ÇÑ Å×½ºÆ®¸¦ À§ÇÏ¿©.
	public:
		void SetArmada() { cannot_dead = true; }
		void ResetArmada() { cannot_dead = false; }
	private:
		bool cannot_dead;
#ifdef ENABLE_SUPPORT_SYSTEM
	private:
		DWORD m_dwSupportCostume;
		DWORD m_dwSupportHair;
		DWORD m_dwSupportWeapon;
		DWORD m_dwSupportInt;
	public:
		void SetSupport() { m_bIsSupport = true; }
		bool IsSupport() { return m_bIsSupport; }
		void SetSupportArmor(int vnum) { m_dwSupportCostume = vnum; }
		void SetSupportHair(int vnum) { m_dwSupportHair = vnum; }
		void SetSupportWeapon(int vnum) { m_dwSupportWeapon = vnum; }
		DWORD GetSupportArmor() { return m_dwSupportCostume; }
		DWORD GetSupportHair() { return m_dwSupportHair; }
		DWORD GetSupportWeapon() { return m_dwSupportWeapon; }
		void SetSupportInt(int count) { m_dwSupportInt = count; }
		DWORD GetSupportInt() { return m_dwSupportInt; }
#endif
#if defined(__BL_67_ATTR__)
	public:
		void Open67Attr();
		void Set67Attr(bool b) { b67Attr = b; }
		bool Is67AttrOpen() const { return b67Attr; }
	private:
		bool b67Attr;
#endif
#ifdef __PET_SYSTEM__
	private:
		bool m_bIsPet;
	public:
		void SetPet() { m_bIsPet = true; }
#ifdef ENABLE_NEW_PET_SYSTEM
		bool IsPet() { return m_bCharType == CHAR_TYPE_PET; }
#else
		bool IsPet() { return m_bIsPet; }
#endif
#endif
#ifdef __NEWPET_SYSTEM__
	private:
		// Nuovo PetSystem
		bool m_bIsNewPet;
		int m_eggvid;
	public:
		// Nuovo PetSystem
		void SetNewPet() { m_bIsNewPet = true; }
		bool IsNewPet() const { return m_bIsNewPet ? true : false; }
		void SetEggVid(int vid) { m_eggvid = vid; }
		int GetEggVid() { return m_eggvid; }
#endif
#ifdef ENABLE_MOUNT_SYSTEM
	private:
		bool m_bIsMountSystem;
	public:
		void SetMount() { m_bIsMountSystem = true; }
		bool IsMountSystem() const { return m_bIsMountSystem;}
#endif
#ifdef NEW_ICEDAMAGE_SYSTEM
	private:
		DWORD m_dwNDRFlag;
		std::set<DWORD> m_setNDAFlag;
	public:
		const DWORD GetNoDamageRaceFlag();
		void SetNoDamageRaceFlag(DWORD dwRaceFlag);
		void UnsetNoDamageRaceFlag(DWORD dwRaceFlag);
		void ResetNoDamageRaceFlag();
		const std::set<DWORD> & GetNoDamageAffectFlag();
		void SetNoDamageAffectFlag(DWORD dwAffectFlag);
		void UnsetNoDamageAffectFlag(DWORD dwAffectFlag);
		void ResetNoDamageAffectFlag();
#endif

	//ÃÖÁ¾ µ¥¹ÌÁö º¸Á¤.
	private:
		float m_fAttMul;
		float m_fDamMul;
	public:
		float GetAttMul() { return this->m_fAttMul; }
		void SetAttMul(float newAttMul) {this->m_fAttMul = newAttMul; }
		float GetDamMul() { return this->m_fDamMul; }
		void SetDamMul(float newDamMul) {this->m_fDamMul = newDamMul; }

	private:
		bool IsValidItemPosition(TItemPos Pos) const;

	public:
		//¿ëÈ¥¼®

		// Ä³¸¯ÅÍÀÇ affect, quest°¡ load µÇ±â Àü¿¡ DragonSoul_Initialize¸¦ È£ÃâÇÏ¸é ¾ÈµÈ´Ù.
		// affect°¡ °¡Àå ¸¶Áö¸·¿¡ ·ÎµåµÇ¾î LoadAffect¿¡¼­ È£ÃâÇÔ.
		void	DragonSoul_Initialize();

		bool	DragonSoul_IsQualified() const;
		void	DragonSoul_GiveQualification();

		int		DragonSoul_GetActiveDeck() const;
		bool	DragonSoul_IsDeckActivated() const;
		bool	DragonSoul_ActivateDeck(int deck_idx);

		void	DragonSoul_DeactivateAll();
		// ¹Ýµå½Ã ClearItem Àü¿¡ ºÒ·¯¾ß ÇÑ´Ù.
		// ¿Ö³ÄÇÏ¸é....
		// ¿ëÈ¥¼® ÇÏ³ª ÇÏ³ª¸¦ deactivateÇÒ ¶§¸¶´Ù µ¦¿¡ activeÀÎ ¿ëÈ¥¼®ÀÌ ÀÖ´ÂÁö È®ÀÎÇÏ°í,
		// activeÀÎ ¿ëÈ¥¼®ÀÌ ÇÏ³ªµµ ¾ø´Ù¸é, Ä³¸¯ÅÍÀÇ ¿ëÈ¥¼® affect¿Í, È°¼º »óÅÂ¸¦ Á¦°ÅÇÑ´Ù.
		//
		// ÇÏÁö¸¸ ClearItem ½Ã, Ä³¸¯ÅÍ°¡ Âø¿ëÇÏ°í ÀÖ´Â ¸ðµç ¾ÆÀÌÅÛÀ» unequipÇÏ´Â ¹Ù¶÷¿¡,
		// ¿ëÈ¥¼® Affect°¡ Á¦°ÅµÇ°í, °á±¹ ·Î±×ÀÎ ½Ã, ¿ëÈ¥¼®ÀÌ È°¼ºÈ­µÇÁö ¾Ê´Â´Ù.
		// (UnequipÇÒ ¶§¿¡´Â ·Î±×¾Æ¿ô »óÅÂÀÎÁö, ¾Æ´ÑÁö ¾Ë ¼ö ¾ø´Ù.)
		// ¿ëÈ¥¼®¸¸ deactivate½ÃÅ°°í Ä³¸¯ÅÍÀÇ ¿ëÈ¥¼® µ¦ È°¼º »óÅÂ´Â °Çµå¸®Áö ¾Ê´Â´Ù.
		void	DragonSoul_CleanUp();
		// ¿ëÈ¥¼® °­È­Ã¢
		
#ifdef ENABLE_NEW_DETAILS_GUI
		void	SendKillLog();
#endif
	public:
		bool		DragonSoul_RefineWindow_Open(LPENTITY pEntity);
		bool		DragonSoul_RefineWindow_Close();

		LPENTITY DragonSoul_RefineWindow_GetOpener()
		{
			return m_playerData ? m_playerData->m_pDragonSoulRefineWindowOpener : nullptr;
		}

		bool		DragonSoul_RefineWindow_CanRefine();
#if defined(WJ_COMBAT_ZONE)
	private:
		DWORD m_iCombatZonePoints;
		DWORD m_iCombatZoneDeaths;

	public:
		LPEVENT m_pkCombatZoneLeaveEvent;
		LPEVENT m_pkCombatZoneWarpEvent;

		BYTE	GetCombatZoneRank();
	
		DWORD	GetRealCombatZonePoints();
		void	SetRealCombatZonePoints(DWORD dwValue);
		
		void	UpdateCombatZoneRankings(const char* memberName, DWORD memberEmpire, DWORD memberPoints);
	
		DWORD	GetCombatZonePoints() { return m_iCombatZonePoints; }
		void	SetCombatZonePoints(DWORD dwValue) { m_iCombatZonePoints = dwValue; }
		DWORD	GetCombatZoneDeaths() { return m_iCombatZoneDeaths; }
		void	SetCombatZoneDeaths(DWORD dwValue) { m_iCombatZoneDeaths = dwValue; }
#endif
		//µ¶ÀÏ ¼±¹° ±â´É ÆÐÅ¶ ÀÓ½Ã ÀúÀå
	private:
		unsigned int itemAward_vnum;
		char		 itemAward_cmd[20];
		//bool		 itemAward_flag;

	public:
		unsigned int GetItemAward_vnum() { return itemAward_vnum; }
		char*		 GetItemAward_cmd() { return itemAward_cmd;	  }
		//bool		 GetItemAward_flag() { return itemAward_flag; }
		void		 SetItemAward_vnum(unsigned int vnum) { itemAward_vnum = vnum; }
		void		 SetItemAward_cmd(char* cmd) { strcpy(itemAward_cmd,cmd); }
		//void		 SetItemAward_flag(bool flag) { itemAward_flag = flag; }
#ifdef ENABLE_ANTI_CMD_FLOOD
	private:
		int m_dwCmdAntiFloodPulse;
		DWORD m_dwCmdAntiFloodCount;
	public:
		int GetCmdAntiFloodPulse(){return m_dwCmdAntiFloodPulse;}
		DWORD GetCmdAntiFloodCount(){return m_dwCmdAntiFloodCount;}
		DWORD IncreaseCmdAntiFloodCount(){return ++m_dwCmdAntiFloodCount;}
		void SetCmdAntiFloodPulse(int dwPulse){m_dwCmdAntiFloodPulse=dwPulse;}
		void SetCmdAntiFloodCount(DWORD dwCount){m_dwCmdAntiFloodCount=dwCount;}
#endif
	private:
		// SyncPositionÀ» ¾Ç¿ëÇÏ¿© Å¸À¯Àú¸¦ ÀÌ»óÇÑ °÷À¸·Î º¸³»´Â ÇÙ ¹æ¾îÇÏ±â À§ÇÏ¿©,
		// SyncPositionÀÌ ÀÏ¾î³¯ ¶§¸¦ ±â·Ï.
		timeval		m_tvLastSyncTime;
		int			m_iSyncHackCount;
	public:
		void			SetLastSyncTime(const timeval &tv) { memcpy(&m_tvLastSyncTime, &tv, sizeof(timeval)); }
		const timeval&	GetLastSyncTime() { return m_tvLastSyncTime; }
		void			SetSyncHackCount(int iCount) { m_iSyncHackCount = iCount;}
		int				GetSyncHackCount() { return m_iSyncHackCount; }

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	public:
		DWORD			GetPickUPMode() const { return dwPickUPMode; }
		void			SetPickUPMode(DWORD dwMode) { dwPickUPMode = dwMode; }
		DWORD			GetRarityMode() const { return dwRarityMode; }
		void			SetRarityMode(DWORD dwMode) { dwRarityMode = dwMode; }

		void			ChangePickUPMode(DWORD dwMode, DWORD dwRMode);
		bool			CheckItemCanGet(const LPITEM item);

	private:
		DWORD dwPickUPMode;
		DWORD dwRarityMode;
#endif

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	private:
		BYTE m_language;
	public:
		BYTE GetCharacterLanguage(){return m_language;}
		void SetCharacterLanguage(BYTE newLanguage){m_language = newLanguage;}
#endif

#ifdef __SASH_SYSTEM__
	protected:
		bool	m_bSashCombination, m_bSashAbsorption;
	
	public:
		bool isSashOpened(bool bCombination)
		{
			return bCombination ? m_bSashCombination : m_bSashAbsorption;
		}

		bool isSashWindowOpened(bool bCombination) const
		{
			return bCombination ? m_bSashCombination : m_bSashAbsorption;
		}

		void	OpenSash(bool bCombination);
		void	CloseSash();
		void	ClearSashMaterials();
		bool	CleanSashAttr(LPITEM pkItem, LPITEM pkTarget);

		LPITEM* GetSashMaterials()
		{
			return m_playerData ? m_playerData->pSashMaterials : nullptr;
		}

		bool	SashIsSameGrade(long lGrade);
		DWORD	GetSashCombinePrice(long lGrade);
		void	GetSashCombineResult(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs);
		BYTE	CheckEmptyMaterialSlot();
		void	AddSashMaterial(TItemPos tPos, BYTE bPos);
		void	RemoveSashMaterial(BYTE bPos);
		BYTE	CanRefineSashMaterials();
		void	RefineSashMaterials();
#endif

#ifdef ENABLE_CALENDAR_SYSTEM
	public:

		bool	IsOpenCalendar() const { return m_isOpenCalendar ? true : false; }
		void 	SetOpenCalendarSystem(bool b) { m_isOpenCalendar = b; }
		
		void 	OpenCalendarSystem();
		bool 	GetEventsInfo(int day);
		void 	SendEvents(int day, int index_event, const char * hora_inicio, const char * hora_final);
		void 	LoadEvents();

		void 	OpenCalendarADM();
		void 	LoadEventsADM();

		void 	SendCalendarPackets(BYTE subheader, BYTE bDay = 0,  const char* name = NULL, const char* image = NULL, const char* szStartAt = NULL, const char* szEndAt = NULL, DWORD dwDuration = 0);
		//void 	SendCalendarPackets(BYTE subheader, int day=0 , char event_escape[100]="", char event_descrip[250] ="", char banner[100] ="", char hora_inicio[6]="", char hora_final[6]="", char hora_inicio_2[6]="", char hora_final_2[6]="");
		void 	SendCalendarPacketsADM(BYTE subheader, int index_event=0 , const char* event_escape = NULL, const char* event_descrip = NULL);
	private:
		bool	m_isOpenCalendar;
		DWORD   m_dwLastOpenCalendar;

#endif

#ifdef ENABLE_HIDE_COSTUME_SYSTEM
	public:
		void 	FuncHideCostume(int slot);
		void 	FuncHideCostumeLoad();
		int 	GetHideCostume(int wear) const ;
		void 	SetHideCostumeUpdate();
		int		GetSlotsCostume();
		int 	GetSlotDefault(int slot);
		DWORD 	SetHideCostumeUpdateShining(DWORD id);
#endif

#ifdef ENABLE_VOTE_COINS_WHILE_PLAYING
	private:
		LPEVENT			m_pkVoteCoinsWhilePlaying;
	public:
		void			StartVoteCoinsEvent();
		void			SetVoteCoinsEventLastTime();
#endif

#ifdef ENABLE_DUNGEON_INFO
	public:
		void 	SetDamageDoneDungeonInfo(int dam){damage_done_dungeon_info = dam;}
		int 	GetDamageDoneDungeonInfo(){return damage_done_dungeon_info;}


		void 	SetDamageReceivedDungeonInfo(int dam){damage_received_dungeon_info = dam;}
		int 	GetDamageReceivedDungeonInfo(){return damage_received_dungeon_info;}

	private:
		int damage_done_dungeon_info;
		int damage_received_dungeon_info;
#endif

#ifdef UNLOCK_INVENTORY_WORLDARD
	public:
		DWORD GetUnlockSlotsW(DWORD type = 0) const {
			return inventory_unlock[type];
		}
		void			SetUnlockSlotsW(DWORD slots,DWORD type) {inventory_unlock[type] = slots;}
	private:
		DWORD 			inventory_unlock[UNLOCK_INVENTORY_MAX];
#endif


	public:
		void 			StartNoticeMapCombatZone();
	private:
#ifdef WJ_COMBAT_ZONE
		LPEVENT			m_pkNoticeCombatZoneEvent;
#endif

#ifdef RENEWAL_DEAD_PACKET
		DWORD			CalculateDeadTime(BYTE type);
#endif


#ifdef BATTLEPASS_WORLDARD
	public:
		BYTE 			GetBattlePassId();
		int 			GetSecondsTillNextMonth();
		int 			GetBattlePassEndTime() { return (m_dwBattlePassEndTime - get_global_time()); };
	protected:
		DWORD			m_dwBattlePassEndTime;
#endif

#ifdef CHANGE_EQUIP_WORLDARD
	public:
		bool 			IsValidItemChangeEquip(WORD wCell, LPITEM item);
		LPITEM			GetChangeEquipItem(WORD wCell) const;
#endif

#ifdef ENABLE_BIYOLOG
	public:
		void			CheckBio();
#endif

#ifdef ENABLE_CHANNEL_CHANGE
		void			MoveChannel(DWORD channelIndex);
#endif

#ifdef ENABLE_BATTLE_PASS
public:
	int		GetBattlePassStatus();
	bool	IsBattlePassActive();

	void	ClearBattlePassData();
	void	SetBattlePassMissonValue(BYTE missionIndex, long long missionValue);
	long long	GetBattlePassMissonValue(BYTE missionIndex);
protected:
	std::map<BYTE, long long> m_battlePassData;
	DWORD m_dwMountStartTime;
#endif

#ifdef ENABLE_ANTI_EXP
	bool			GetAntiExp() { return m_pointsInstant.anti_exp; }
	void			SetAntiExp(bool flag) { m_pointsInstant.anti_exp = flag; }
#endif
#ifdef ENABLE_MULTI_FARM_BLOCK
public:
	bool				GetRewardStatus() { return m_bmultiFarmStatus; }
	void				SetRewardStatus(bool bValue) { m_bmultiFarmStatus = bValue; }
protected:
	bool				m_bmultiFarmStatus;
#endif

#ifdef ENABLE_CHECK_PICKUP_HACK
	private:
		DWORD m_dwLastPickupTime;
	public:
		void SetLastPickupTime() { m_dwLastPickupTime = get_dword_time(); }
		DWORD GetLastPickupTime() { return m_dwLastPickupTime; }
#endif
#ifdef ENABLE_CHECK_GHOSTMODE
	private:
		DWORD m_dwCountGhostmodePoint;
	public:
		DWORD GetGhostmodeCount() { return m_dwCountGhostmodePoint; }
		void AddGhostmodeCount() { m_dwCountGhostmodePoint += 1; }
		void ResetGhostmodeCount() { m_dwCountGhostmodePoint = 0; }
#endif
#ifdef ENABLE_CHECK_WALLHACK
	private:
		DWORD m_dwCountWallhackPoint;
	public:
		DWORD GetWallhackCount() { return m_dwCountWallhackPoint; }
		void AddWallhackCount() { m_dwCountWallhackPoint += 1; }
		void ResetWallhackCount() { m_dwCountWallhackPoint = 0; }
#endif
#ifdef ENABLE_CSHIELD
	private:
		LPEVENT m_pkCShieldEvent;
		LPEVENT m_pkCShieldDataEvent;
		spCShield m_cshield;
	public:
		void StartCShieldEvent();
		void StartCShieldDataEvent();
		void SendCShieldPacket(bool start);
		spCShield GetCShield() const { return m_cshield; }
#endif


#ifdef ENABLE_ANTI_PACKET_FLOOD
private:
	int m_dwPacketAntiFloodPulse;
	uint32_t m_dwPacketAntiFloodCount;
public:
    int GetPacketAntiFloodPulse() noexcept { return m_dwPacketAntiFloodPulse; }
    DWORD GetPacketAntiFloodCount() noexcept { return m_dwPacketAntiFloodCount; }
    DWORD IncreasePacketAntiFloodCount() noexcept { return ++m_dwPacketAntiFloodCount; }
    void SetPacketAntiFloodPulse(int dwPulse) noexcept { m_dwPacketAntiFloodPulse = dwPulse; }
    void SetPacketAntiFloodCount(DWORD dwCount) noexcept { m_dwPacketAntiFloodCount = dwCount; }
#endif
#ifdef ENABLE_FLOOD_PRETECTION
public:
	int analyze_protect = 0;
	int analyze_protect_count = 0;
#endif

#ifdef ENABLE_PREVENT_FLOOD_PACKETS
public:
    int GetLastExchangeTime() const noexcept { return m_dwLastExchangeTime; }
    void SetLastExchangeTime() noexcept { m_dwLastExchangeTime = thecore_pulse(); }

    int GetLastBuyTime() const noexcept { return m_dwLastBuyTime; }
    void SetLastBuyTime() noexcept { m_dwLastBuyTime = thecore_pulse(); }

    int GetLastSafeboxCheckOutTime() const noexcept { return m_dwLastSafeBoxCheckOutTime; }
    void SetLastSafeboxCheckOutTime() noexcept { m_dwLastSafeBoxCheckOutTime = thecore_pulse(); }

    int GetLastItemSplitTime() const noexcept { return m_dwLastItemSplitTime; }
    void SetLastItemSplitTime() noexcept { m_dwLastItemSplitTime = thecore_pulse(); }

    int GetLastOfferExpTime() const noexcept { return m_dwLastOfferExpime; }
    void SetLastOfferExpTime() noexcept { m_dwLastOfferExpime = thecore_pulse(); }

    uint32_t GetLastBraveCapePulse() const noexcept { return dwLastBraveryCapePulse; }
    void SetLastBraveCapePulse(uint32_t time) noexcept { dwLastBraveryCapePulse = time; }

#ifdef ENABLE_GUILD_DONATE_ATTENDANCE
    int GetLastOfferNewExpTime() const noexcept { return m_dwLastOfferNewExpTime; }
    void SetLastOfferNewExpTime() noexcept { m_dwLastOfferNewExpTime = thecore_pulse(); }
#endif

#ifdef ENABLE_SEARCH_ITEM_DROP_ON_MOB
    int GetLastDropItemSearchTime() const noexcept { return m_dwLastDropItemSearchTime; }
    void SetLastDropItemSearchTime() noexcept { m_dwLastDropItemSearchTime = thecore_pulse(); }
#endif

private:
    int m_dwLastExchangeTime;
    int m_dwLastBuyTime;
    int m_dwLastSafeBoxCheckOutTime;
    int m_dwLastItemSplitTime;
    int m_dwLastOfferExpime;
    uint32_t dwLastBraveryCapePulse;
#ifdef ENABLE_GUILD_DONATE_ATTENDANCE
    int m_dwLastOfferNewExpTime;
#endif
#ifdef ENABLE_SEARCH_ITEM_DROP_ON_MOB
    int m_dwLastDropItemSearchTime;
#endif
#endif
public:
	void StopAttacksOnLogout(); 

#ifdef ENABLE_ZODIAC_MISSION
public:
	void				BeadCount(LPCHARACTER ch);
#endif

#ifdef __RENEWAL_BRAVE_CAPE__
public:
	void			GetBraveCapeCMDCompare(const char* c_pszArgument);
	LPEVENT			m_pkBraveCapeEvent;
#endif

#ifdef ENABLE_MELEY_LAIR_DUNGEON
public:
	bool			IsMeley();
	void			SetQuestNPCIDAttack(DWORD vid){ m_dwQuestNPCVIDAttack = vid;}
	DWORD			GetQuestNPCIDAttack() const { return m_dwQuestNPCVIDAttack; }
	LPCHARACTER		GetQuestNPCAttack() const;
private:
	DWORD			m_dwQuestNPCVIDAttack;
#endif

#ifdef ENABLE_TRACK_WINDOW
public:
	void	GetDungeonCooldown(WORD bossIndex);
	void	GetDungeonCooldownTest(WORD bossIndex, int value, bool isCooldown);
#endif

#ifdef ENABLE_EXCHANGE_LOG
public:
	void	LoadExchangeLog();
	bool	LoadExchangeLogItem(DWORD logID);
	void	DeleteExchangeLog(DWORD logID);
	void	SendExchangeLogPacket(BYTE subHeader, DWORD id = 0, const TExchangeLog* exchangeLog = NULL);

protected:
	std::map<DWORD, std::pair<TExchangeLog, std::vector<TExchangeLogItem>>> m_mapExchangeLog;
#endif

#ifdef __GEM_SYSTEM__
public:
	int				GetGem() const { return m_points.gem; }
	void			SetGem(int gem) { m_points.gem = gem; }

	void			LoadGemItems();
	void			SaveGemItems();
	void			BuyGemItem(BYTE slotIndex);
	void			RefreshGemPlayer();
	void 			RefreshGemItems();

protected:
	std::map<BYTE, TGemItem> m_mapGemItems;
#endif

# ifdef ENABLE_MINI_GAME_CATCH_KING
public:
	void MiniGameCatchKingSetFieldCards(std::vector<TCatchKingCard> vec) { m_vecCatchKingFieldCards = vec; }

	uint32_t MiniGameCatchKingGetScore() const noexcept { return dwCatchKingTotalScore; }
	void MiniGameCatchKingSetScore(uint32_t dwScore) noexcept { dwCatchKingTotalScore = dwScore; }

	uint32_t MiniGameCatchKingGetBetNumber() const noexcept { return bCatchKingBetSetNumber; }
	void MiniGameCatchKingSetBetNumber(uint8_t bSetNr) noexcept { bCatchKingBetSetNumber = bSetNr; }

	uint8_t MiniGameCatchKingGetHandCard() const noexcept { return bCatchKingHandCard; }
	void MiniGameCatchKingSetHandCard(uint8_t bKingCard) noexcept { bCatchKingHandCard = bKingCard; }

	uint8_t MiniGameCatchKingGetHandCardLeft() const noexcept { return bCatchKingHandCardLeft; }
	void MiniGameCatchKingSetHandCardLeft(uint8_t bHandCard) noexcept { bCatchKingHandCardLeft = bHandCard; }

	bool MiniGameCatchKingGetGameStatus() const noexcept { return dwCatchKingGameStatus; }
	void MiniGameCatchKingSetGameStatus(bool bStatus) noexcept { dwCatchKingGameStatus = bStatus; }

	std::vector<TCatchKingCard> m_vecCatchKingFieldCards;

protected:
	LPCHARACTER m_Owner;

public:
	LPCHARACTER m_PetOwner;
	LPCHARACTER GetOwner() {return m_PetOwner;}
	void SetOwner(LPCHARACTER ch) { m_PetOwner = ch; }

protected:
	uint8_t bCatchKingHandCard;
	uint8_t bCatchKingHandCardLeft;
	bool dwCatchKingGameStatus;
	uint8_t bCatchKingBetSetNumber;
	uint32_t dwCatchKingTotalScore;
# endif // ENABLE_MINI_GAME_CATCH_KING

#ifdef ENABLE_VOTE4BUFF
public:
	long long	GetVoteCoin();
	void		SetVoteCoin(long long amount);
#endif
	public:
		struct S_CARD
		{
			DWORD	type;
			DWORD	value;
		};

		struct CARDS_INFO
		{
			S_CARD cards_in_hand[MAX_CARDS_IN_HAND];
			S_CARD cards_in_field[MAX_CARDS_IN_FIELD];
			DWORD	cards_left;
			DWORD	field_points;
			DWORD	points;
		};
		
		void			Cards_open(DWORD safemode);
		void			Cards_clean_list();
		DWORD			GetEmptySpaceInHand();
		void			Cards_pullout();
		void			RandomizeCards();
		bool			CardWasRandomized(DWORD type, DWORD value);
		void			SendUpdatedInformations();
		void			SendReward();
		void			CardsDestroy(DWORD reject_index);
		void			CardsAccept(DWORD accept_index);
		void			CardsRestore(DWORD restore_index);
		DWORD			GetEmptySpaceInField();
		DWORD			GetAllCardsCount();
		bool			TypesAreSame();
		bool			ValuesAreSame();
		bool			CardsMatch();
		DWORD			GetLowestCard();
		bool			CheckReward();
		void			CheckCards();
		void			RestoreField();
		void			ResetField();
		void			CardsEnd();
		void			GetGlobalRank(char * buffer, size_t buflen);
		void			GetRundRank(char * buffer, size_t buflen);
	protected:
		CARDS_INFO	character_cards;
		S_CARD	randomized_cards[24];
#ifdef RENEWAL_MISSION_BOOKS
public:
	void		SetMissionBook(BYTE missionType, BYTE value, DWORD arg, WORD level);
	void		RewardMissionBook(WORD missionID);
	void		DeleteBookMission(WORD missionID);
	void		LoadMissionData();
	void		SendMissionData();
	void		SaveMissionData();
	void		GiveNewMission(WORD missionID);
	bool		IsMissionHas(WORD missionID);
	BYTE		MissionCount();
	void		ModifySetMissionCMD(WORD missionID, std::string& cmdText);
protected:
	std::map<WORD, TMissionBook> m_mapMissionData;
#endif

#ifdef ENABLE_RANKING
	protected:
		long long	m_lRankPoints[RANKING_MAX_CATEGORIES];

	public:
		long long	GetRankPoints(int iArg);
		void		SetRankPoints(int iArg, long long lPoint);
		void		RankingSubcategory(int iArg);
#endif

#ifdef __BUFFI_SUPPORT__
	public:
		bool		IsBuffi() { return m_bIsSupport; }
		void		SetBuffi(bool bFlag);
		void		StateBuffi();
		void		SendBuffiSkillAffect(DWORD targetVID, DWORD skillIdx, BYTE skillLevel);
		void		SetLookingBuffiPage(bool bFlag) { m_bIsLookingBuffiPage = bFlag; }
		bool		IsLookingBuffiPage() { return m_bIsLookingBuffiPage; }
	protected:
		bool		m_bIsSupport;
		bool		m_bIsLookingBuffiPage;
#endif


	public:
		bool			IsSBOpen() const { return m_isOpenSB ? true : false; }
		void 			SetSBOpen(bool b) { m_isOpenSB = b; }
		LPITEM 			GetSwitchBotItem(WORD wCell) const;

	private:
		bool 			m_isOpenSB;

#ifdef __HIT_LIMITER_ENABLE__
	public:
		const DWORD & GetHitCount() { return dw_hit_count; }
		const DWORD & GetHitNextUpdate() { return dw_hit_next_update; }
		void SetHitCount(const DWORD & dwHitCount) { dw_hit_count = dwHitCount; }
		void SetHitNextUpdate(const DWORD & dwHitNextUpdate) { dw_hit_next_update = dwHitNextUpdate; }

		bool FindEnemyVID(const DWORD & dwVID) { return (um_enemy_vids.find(dwVID) != um_enemy_vids.end()); }
		void AppendEnemyVID(const DWORD & dwVID) { um_enemy_vids.insert(std::make_pair(dwVID, 0)); }
		void UpdateEnemyHitCount(const DWORD & dwVID, const WORD & wCount) { um_enemy_vids[dwVID] = wCount; }
		const WORD & GetEnemyVIDHitCount(const DWORD & dwVID) { return um_enemy_vids[dwVID]; }
		const WORD GetEnemyVIDCount() { return um_enemy_vids.size(); }
		void ClearEnemyVids() { return um_enemy_vids.clear(); }

	private:
		DWORD dw_hit_count;
		DWORD dw_hit_next_update;
		boost::unordered_map<DWORD, WORD> um_enemy_vids;
#endif

	public:
		uint32_t GetLastMoveItemTime() const { return m_dwLastMoveItemTime; }
		bool IsLastMoveItemTime() const;
		void SetLastMoveItemTime(uint32_t dwLastMoveItemTime) { m_dwLastMoveItemTime = dwLastMoveItemTime; }
	private:
		uint32_t m_dwLastMoveItemTime;

#ifdef __DUNGEON_INFO__
public:
	void			SendDungeonCooldown(DWORD bossIdx);
	int				GetQuestFlagSpecial(const char* szQuestFlag, ...);
#endif

#ifdef __AUTO_SKILL_READER__
public:
	void			GetAutoSkill(BYTE skillIdx, bool status, bool isFromEvent = false);
	bool			ReadSkill(BYTE skillIdx);
	bool			SkillToBook(BYTE skillIdx, DWORD& itemIdx, DWORD& socket0, DWORD& exorcismIdx, DWORD& concentratedIdx);
	void			SetAutoSkillEvent(LPEVENT _event) { m_pkAutoSkill = _event; }
	BYTE			GetSelectedSkillIndex() { return m_bSelectedSkillIdx; }
	BYTE			GetSkillGradeByLevel(int iSkillLevel);
protected:
	enum EAutoSkillRead
	{
		// ! Don't touch !
		SKILL_GRADE_MASTER = 0,
		SKILL_GRADE_GRAND_MASTER = 1,
		SKILL_GRADE_PERFECT_MASTER = 2,
		SKILL_GRADE_L_MASTER = 3,
		// ! Don't touch !

		USE_YMIR_50300_SKILLBOOK = 0,// if the 0 system will search the manuel books example: 50401
		
		USE_COOLTIME_ON_BOOKS = 1,
		USE_COOLTIME_ON_SOULSTONE = 1,

		USE_DECREASE_ALIGNMENT_SOULSTONE = 1,
		USE_ZEN_BEAN_SOULSTONE = 1,

	};
	bool			m_bAutoSkillStatus;
	BYTE			m_bSelectedSkillIdx;
	LPEVENT			m_pkAutoSkill;
#endif

#ifdef ENABLE_QUEEN_NETHIS
public:
	int ComputeSnakeSkill(uint32_t dwVnum, LPCHARACTER pkVictim, uint8_t bSkillLevel);
private:
	LPEVENT m_pkSnakeSkillEvent;
#endif

#if defined(__AUTOMATIC_HUNTING_SYSTEM__)
	// Automatic Hunting
public:
	void SetAutoUseType(const uint8_t c_uiType, const bool c_bOnOff) { m_abAutoUseType[c_uiType] = c_bOnOff; }
	bool GetAutoUseType(const uint8_t c_uiType) { return m_abAutoUseType[c_uiType]; }

#ifdef USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED
	bool GetAutoUseTypeCst(const uint8_t c_uiType) const
	{
		return m_abAutoUseType[c_uiType];
	}
#endif

#ifdef USE_AUTOMATIC_HUNTING_RENEWED
	int GetAutoHuntLastDay() const;
	void SetAutoHuntLastDay(const int val);

	int GetAutoHuntUsedTime() const;
	void SetAutoHuntUsedTime(const int val, bool update = false);

	void AutoHunt_Initialize();
#endif

private:
	bool m_abAutoUseType[AUTO_ONOFF_MAX];
#endif

public:
	LPITEM			GetSpecialStorageItem(const WORD &wCell, int invType) const;
	int				GetEmptySpecialStorageSlotFromIndex(WORD index, BYTE itemSize, int invType) const;
	int				GetEmptyInventoryFromIndex(WORD index, BYTE itemSize) const;

#ifdef __FAKE_PLAYER__
	public:
		bool				IsFakePlayer() const { return m_bIsFakePlayer; }
		void				SetFakePlayer(bool bValue) { m_bIsFakePlayer = bValue; }
		void				SetLanguage(uint32_t stLanguage) { m_stFakePlayerLanguage = stLanguage; }
		void 				SetFakePlayerGuildID(uint32_t dwID) { m_dwFakePlayerGuildID = dwID; }
		void 				SetFakePlayerBlockEQ(uint8_t bBlock) { m_bFakePlayerBlockEQ = bBlock; }
		uint8_t				GetFakePlayerBlockEQ() { return m_bFakePlayerBlockEQ; }
		uint8_t				GetFakePlayerLanguage() { return m_stFakePlayerLanguage; }
		void 				FakePlayerCreatePetSystem();
		void 				FakePlayerCreateMountSystem();
		LPEVENT				m_pFakePlayerDeadEvent;
		void				SetCharType(BYTE type)  {m_bCharType = type; };

	protected:
		bool				m_bIsFakePlayer;
		uint32_t			m_stFakePlayerLanguage;
		uint32_t			m_dwFakePlayerGuildID;
		uint8_t				m_bFakePlayerBlockEQ;
#endif
#ifdef VOTE_BUFF_RENEWAL
	public:
		long			GetVoteCoins() const		{ return m_coinsvote;	}
		void			SetVoteCoins(long votecoins);
		long			GetEp()	const;
		void			SetEp(long votecoins);
	protected:
		long			m_coinsvote;
#endif
	public:
		const char* GetHardwareID();
#ifdef USE_CAPTCHA_SYSTEM
    protected:
        LPEVENT         m_CaptchaTimer;

    private:
        bool        m_isWaitingForCaptcha;
        uint16_t    m_Captcha;

    public:
        void        LoadCaptchaTimer();
        bool        IsWaitingForCaptcha() { return m_isWaitingForCaptcha; };
        void        SetWaitingForCaptcha(bool bValue) { m_isWaitingForCaptcha = bValue; };
        uint16_t    GetCaptcha() { return m_Captcha; };
        void        ValidateCaptcha(uint16_t);
        void        CleanCaptchaTimer();
#endif

    private:
        std::map<std::string, int>  m_protection_Time;

    public:
        void        SetProtectTime(const std::string& flagname, int value);
        int         GetProtectTime(const std::string& flagname) const;

#ifdef USE_OFFLINESHOP_SEARCH_V2
    private:
        int32_t     m_iShopSearchPageCount;
        int32_t     m_iShopSearchPage;
        ikashop::TFilterInfo m_ShopSearchFilterInfo;

    public:
        std::vector<ikashop::TSearchResultInfo> m_vecShopSearchItem;
        std::unordered_set<uint32_t> m_setShopSearchItemIds;

        int32_t     GetShopSearchPage() const;
        void        SetShopSearchPage(const int32_t v);

        bool        RefreshShopSearchPageCount();
        size_t      GetShopSearchMaxPageCount() const;

        bool                    SetShopSearchFilterInfo(const ikashop::TFilterInfo& filter);
        ikashop::TFilterInfo    GetShopSearchFilterInfo() const;

        bool SetShopSearchEraseItem(const uint32_t id);
        bool SetShopSearchIds(const uint32_t id);
        bool GetShopSearchIds(const uint32_t id);
        bool UpdateShopSearchItemPrice(const uint32_t id, const long long price);
#endif
#ifdef ENABLE_SUNG_MAHI_TOWER
	protected:
		bool m_bIsUniqueMaster;
		
	public:
		void SetUniqueMaster(bool bInfo) { m_bIsUniqueMaster = bInfo; }
		bool GetUniqueMaster() const { return m_bIsUniqueMaster; }
		void SetDungeonMultipliers(BYTE dungeonLevel);
		bool IsSungMahiDungeon(int mapIdx) const;
#endif
	public:
		int 	m_iRequestRankTimer;
		int		GetRequestRankTimer() const	{ return m_iRequestRankTimer; }
		void	SetRequestRankTimer() { m_iRequestRankTimer = thecore_pulse(); }
		void			SaveRankingInfo();
	protected:
		std::map<BYTE, long long> m_map_RankInfo;
#if defined(ENABLE_FISHING_GAME)
	public:
		void SetFishingGameGoals(BYTE bCount) { m_bFishingGameGoals = bCount; }
		BYTE GetFishingGameGoals() { return m_bFishingGameGoals; }
		void SetFishingKey(uint32_t key) { fishing_key = key; };
		uint32_t GetFishingKey() { return fishing_key; };
		uint32_t fishing_key;

	private:
		BYTE m_bFishingGameGoals;
#endif

	public:
		BYTE			bWeaponStatus;
		int				getWeaponStatus() { return bWeaponStatus; };

#ifdef __AURA_SYSTEM__
	private:
		BYTE		m_bAuraRefineWindowType;
		bool		m_bAuraRefineWindowOpen;
		TItemPos	m_pAuraRefineWindowItemSlot[AURA_SLOT_MAX];
		TAuraRefineInfo m_bAuraRefineInfo[AURA_REFINE_INFO_SLOT_MAX];

	protected:
		BYTE		__GetAuraAbsorptionRate(BYTE bLevel, BYTE bBoostIndex) const;
		TAuraRefineInfo __GetAuraRefineInfo(TItemPos Cell);
		TAuraRefineInfo __CalcAuraRefineInfo(TItemPos Cell, TItemPos MaterialCell);
		TAuraRefineInfo __GetAuraEvolvedRefineInfo(TItemPos Cell);

	public:
		void		OpenAuraRefineWindow(LPENTITY pOpener, EAuraWindowType type);
		bool		IsAuraRefineWindowOpen() const { return  m_bAuraRefineWindowOpen; }
		BYTE		GetAuraRefineWindowType() const { return  m_bAuraRefineWindowType; }

		LPENTITY GetAuraRefineWindowOpener()
		{
			return m_playerData ? m_playerData->m_pAuraRefineWindowOpener : nullptr;
		}

		bool		IsAuraRefineWindowCanRefine();

		void		AuraRefineWindowCheckIn(BYTE bAuraRefineWindowType, TItemPos AuraCell, TItemPos ItemCell);
		void		AuraRefineWindowCheckOut(BYTE bAuraRefineWindowType, TItemPos AuraCell);
		void		AuraRefineWindowAccept(BYTE bAuraRefineWindowType);
		void		AuraRefineWindowClose();
#endif

#ifdef ENABLE_MESSENGER_BLOCK
	private:
		int m_iBlockAntiFloodPulse;
		DWORD m_dwBlockAntiFloodCount;
	public:
		int GetBlockAntiFloodPulse() const { return m_iBlockAntiFloodPulse; }
		DWORD GetBlockAntiFloodCount() const { return m_dwBlockAntiFloodCount; }
		DWORD IncreaseBlockAntiFloodCount() { return ++m_dwBlockAntiFloodCount; }
		void SetBlockAntiFloodPulse(int dwPulse) { m_iBlockAntiFloodPulse = dwPulse; }
		void SetBlockAntiFloodCount(DWORD dwCount) { m_dwBlockAntiFloodCount = dwCount; }
#endif

#if defined(__GAME_OPTION_ESCAPE__)
public:
	void SetEscapeCooltime(const DWORD dwTime) { m_dwEscapeCooltime = dwTime; }
	DWORD GetEscapeCooltime() const { return m_dwEscapeCooltime; }
private:
	DWORD m_dwEscapeCooltime;
#endif

#ifdef USE_CHEQUE_CURRENCY
public:
	uint16_t GetCheque() const;
	void SetCheque(const uint16_t value);
#endif

#ifdef USE_DELAY_MGR
private:
	uint32_t m_dwDelayTime[M2_DELAY_MAX];

public:
	void SetDelay(const uint8_t bType, const uint32_t dwValue);
	bool IsDelay(const uint8_t bType, bool bMsg = false);
#endif

#ifdef USE_CURRENCY_EXCHANGE
public:
	void OnCurrencyExchange(long long exchangeAmount, uint8_t exchangeType);
#endif

#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
public:
	std::vector<THitCountInfo> m_hitCount;
	void RewardAttendance();
#endif

public:
	bool UseItemBoosterAddAttribute(CItem& item, CItem& targetItem);
	bool UseItemBoosterChangeAttribute(CItem& item, CItem& targetItem);

#ifdef __NEW_SET_BONUS__
public:
	bool			IsNewSetNeedRefresh(DWORD itemIdx);
#endif
#ifdef ENABLE_BATTLEPASS
	public:
		void LoadBattlePassProgress();
		void SaveBattlePassProgress();
		
        bool ActivateBattlePassPremium();

        void RedeemBattlePassReward(size_t tier_index, bool premium_item);
        void RedeemBattlePassMissionExp(size_t mission_index);

        void SendBattlePassCurrentProgress();

		void UpdateMissionProgress(EBattlePassMissions mission, int32_t update_amount = 1, uint32_t target_vnum = 0);
        void InitializeBattlePassData();
        void UpdateBattlePassCurrentCharacterInfo();
        void UpdateBattlePassMissionProgress(size_t mission_index, size_t progress, bool completed, bool collected = false);
        void UpdateBattlePassRewardInfo(size_t tier_index);

	private:
		std::unordered_map<uint32_t /*mission id*/, BattlePassMyMissionInfo> m_player_battle_pass_stats;
		std::unordered_map<uint32_t /*mission id*/, BattlePassMyTierInfo> m_player_battle_pass_tier_info;

		uint16_t m_player_battle_pass_current_level;
		uint32_t m_player_battle_pass_current_exp;
		bool	 m_player_battle_pass_premium_status;

#endif

#ifdef __BL_BATTLE_ROYALE__
public:
	bool IsBattleRoyale() const { return m_lMapIndex == BattleRoyale::Settings::BR_MAP_INDEX; }
	void SetSkipSave(bool flag) { m_bSkipSave = flag; }
#endif
	UINT GetInventorySize(uint32_t inventoryType = 0) const;

#ifdef ENABLE_VOICE_CHAT
public:
	void VoiceChatSetConfig(uint8_t type, bool flag);
	bool VoiceChatIsBlock(uint8_t type) const;
	int VoiceChatGetConfig() const;
#endif

//#ifdef ENABLE_ITEMSHOP
private:
	bool m_bAccountMoneyLoaded;

	long long m_lldAccountCoins;
#ifdef USE_ITEMSHOP_RENEWED
	long long m_lldAccountJCoins;
#endif

public:
#ifdef USE_ITEMSHOP_RENEWED
	void GetAccountMoney(long long& lldCoins, long long& lldJCoins, bool bReloadForced = false);
	void SetAccountMoney(long long lldCoins, long long lldJCoins, bool bPlus);
#else
	void GetAccountMoney(long long& lldCoins, bool bReloadForced = false);
	void SetAccountMoney(long long lldCoins, bool bPlus);
#endif
	void RefreshAccountMoney();
//#endif

#ifdef USE_CRYSTAL_SYSTEM
public:
	void ClearShardCraft(int32_t iTargetSlot = -1);
	void SendOutShardCraft(int32_t iTargetSlot = -1);
	void SendShardCraft(const uint8_t bSubHeader,
							int32_t iSourceSlot = -1,
							int32_t iTargetSlot = -1);
	bool IsShardCraftOpened() const
	{
		return m_playerData ? m_playerData->m_bShardCraftWindowStat : false;
	}
#endif

#ifdef USE_BRAVE_CAPE_RENEWED
public:
	DWORD GetLastReviveTime() const
	{
		return m_playerData ? m_playerData->m_dwReviveTime : 0;
	}
#endif
};

ESex GET_SEX(LPCHARACTER ch);

#ifdef USE_CAPTCHA_SYSTEM
EVENTINFO(captcha_event_info)
{
    DynamicCharacterPtr ch;
    bool                disconnect;
};
#endif

#endif
