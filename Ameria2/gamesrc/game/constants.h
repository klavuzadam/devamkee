#ifndef __INC_METIN_II_GAME_CONSTANTS_H__
#define __INC_METIN_II_GAME_CONSTANTS_H__

#include "../common/tables.h"

#ifdef USE_CRYSTAL_SYSTEM
extern std::map<BYTE, std::vector<WORD>> m_mapCrystalBonuses;
#endif

#ifdef USE_PASSIVE_ABILITY_TAMER
extern std::unordered_map<int, uint8_t> m_mapPassiveAbilityTamerBnsType;
extern uint8_t m_bPassiveAbilityTamerBookCount[SKILL_MAX_LEVEL + 1];
extern int64_t m_vPassiveAbilityTamerBnsValue[PASSIVE_ABILITY_VNUM_TAMER_MAX_COUNT][SKILL_MAX_LEVEL + 1];
#endif

#ifdef ENABLE_NEW_PET_SYSTEM
extern std::map<BYTE, std::vector<std::pair<DWORD, WORD>>> petEvolutionItems;
extern std::map<BYTE, std::vector<std::pair<BYTE, long>>> petSkillBonus;
extern BYTE petEvolutionLimits[PET_MAX_EVOLUTION_CONST + 1];
#endif

#ifdef ENABLE_BIYOLOG
extern const DWORD bio_data[][17];
extern const BYTE bio_max;
extern BYTE pointToApply(BYTE p);
#endif

#ifdef __NEW_SET_BONUS__
typedef struct SNewSetBonus
{
	std::vector<std::pair<BYTE, std::vector<DWORD>>> vecSlots; // Store multiple item IDs for each slot
	std::map<BYTE, BYTE> mapPartCountToBonusCount;
	std::map<BYTE, std::pair<BYTE, long>> mapBonus;

	SNewSetBonus(const std::vector<std::pair<BYTE, std::vector<DWORD>>>& _vecSlots, 
				 const std::map<BYTE, BYTE>& _mapPartCountToBonusCount, 
				 const std::map<BYTE, std::pair<BYTE, long>>& _mapBonus)
		: vecSlots(_vecSlots), 
		  mapPartCountToBonusCount(_mapPartCountToBonusCount), 
		  mapBonus(_mapBonus) {}
} TNewSetBonus;
extern const std::map<std::string, TNewSetBonus> m_mapNewSetBonus;
#endif

#ifdef __AURA_SYSTEM__
static int s_aiAuraRefineTable[AURA_GRADE_MAX_NUM][AURA_REFINE_INFO_MAX];
int* GetAuraRefineInfo(BYTE bLevel);
int GetAuraRefineInfo(BYTE bGrade, BYTE bInfo);
#endif

#if defined(ENABLE_2LOOT)
extern bool Is2Loot(DWORD dwMobVnum, BYTE bType);
extern void Init2Loot();
#endif

enum EMonsterChatState
{
	MONSTER_CHAT_WAIT,
	MONSTER_CHAT_ATTACK,
	MONSTER_CHAT_CHASE,
	MONSTER_CHAT_ATTACKED,
};

typedef struct SMobRankStat
{
	int iGoldPercent;   // µ·ÀÌ ³ª¿Ã È®·ü
} TMobRankStat;

typedef struct SMobStat
{
	BYTE	byLevel;
	WORD	HP;
	DWORD	dwExp;
	WORD	wDefGrade;
} TMobStat;

typedef struct SBattleTypeStat
{
	int		AttGradeBias;
	int		DefGradeBias;
	int		MagicAttGradeBias;
	int		MagicDefGradeBias;
} TBattleTypeStat;

#ifdef ENABLE_YOHARA_SYSTEM
typedef std::map<DWORD, TSungmaTable> TSungmaMap;
extern TSungmaMap g_map_SungmaTable;

typedef struct SRandomJewelry
{
	uint8_t byLevel;
	uint8_t byRandomValue[4];
} TRandomJewelry;

typedef struct SRandomMysterious
{
	uint8_t byLevel;
	uint8_t byRandomValue[30];
} TRandomMysterious;

typedef struct SRandomGloveInfo
{
	uint8_t byLevel;
	uint8_t byRandomValue[5];
} TRandomGloveInfo;

// Snake
typedef struct SRandomSnakeGloveInfo
{
	uint8_t byLevel;
	uint8_t byRandomValue[4];
} TRandomSnakeGloveInfo;

typedef struct SRandomSnakeArmorInfo
{
	DWORD refineNumber;
	int iRefineValues[16][5];
} TRandomSnakeArmorInfo;

typedef struct SRandomSnakeWeaponInfo
{
	DWORD refineNumber;
	int iRefineValues[24][3];
} TRandomSnakeWeaponInfo;

extern const std::vector<TRandomJewelry> aiItemApplyRandomTalisman;
extern const std::vector<TRandomMysterious> aiItemApplyRandomMysterious;
extern const std::vector<TRandomJewelry> aiItemApplyRandomJewelry;
extern const std::vector<TRandomGloveInfo> aiItemApplyRandomGlove;
extern const std::vector<TRandomSnakeGloveInfo> aiItemApplyRandomSnakeGlove;
extern const std::vector <TRandomSnakeArmorInfo> aiItemApplyRandomSnakeArmor;
extern const std::vector <TRandomSnakeWeaponInfo> aiItemApplyRandomSnakeWeapon;
#endif
#ifdef ENABLE_PRECISION_SKILL
extern const int aiSkillPrecision[SKILL_MAX_LEVEL + 1];
#endif

typedef struct SJobInitialPoints
{
	int		st, ht, dx, iq;
	int		max_hp, max_sp;
	int		hp_per_ht, sp_per_iq;
	int		hp_per_lv_begin, hp_per_lv_end;
	int		sp_per_lv_begin, sp_per_lv_end;
	int		max_stamina;
	int		stamina_per_con;
	int		stamina_per_lv_begin, stamina_per_lv_end;
} TJobInitialPoints;

typedef struct __coord
{
	int		x, y;
} Coord;

typedef struct SApplyInfo
{
	BYTE	bPointType;                          // APPLY -> POINT
} TApplyInfo;

enum {
	FORTUNE_BIG_LUCK,
	FORTUNE_LUCK,
	FORTUNE_SMALL_LUCK,
	FORTUNE_NORMAL,
	FORTUNE_SMALL_BAD_LUCK,
	FORTUNE_BAD_LUCK,
	FORTUNE_BIG_BAD_LUCK,
	FORTUNE_MAX_NUM,
};

const int CHEST_BOOK_INFO_MAX_NUM = 31;
const int CHEST_BOOK_MAX_NUM = 4;
const int STONE_INFO_MAX_NUM = 38;
const int STONE_LEVEL_MAX_NUM = 4;

struct SStoneDropInfo
{
	DWORD dwMobVnum;
	int iDropPct;
	int iLevelPct[STONE_LEVEL_MAX_NUM+1];
};

struct SChestDrop
{
	DWORD dwMobVnum;
	int iChance[CHEST_BOOK_MAX_NUM];
};

inline bool operator < (const SStoneDropInfo& l, DWORD r)
{
	return l.dwMobVnum < r;
}

inline bool operator < (const SChestDrop& l, DWORD r)
{
	return l.dwMobVnum < r;
}

inline bool operator < (DWORD l, const SStoneDropInfo& r)
{
	return l < r.dwMobVnum;
}

inline bool operator < (DWORD l, const SChestDrop& r)
{
	return l < r.dwMobVnum;
}

inline bool operator < (const SStoneDropInfo& l, const SStoneDropInfo& r)
{
	return l.dwMobVnum < r.dwMobVnum;
}

inline bool operator < (const SChestDrop& l, const SChestDrop& r)
{
	return l.dwMobVnum < r.dwMobVnum;
}

extern const TApplyInfo		aApplyInfo[MAX_APPLY_NUM];
extern const TMobRankStat       MobRankStats[MOB_RANK_MAX_NUM];

extern TBattleTypeStat		BattleTypeStats[BATTLE_TYPE_MAX_NUM];

extern const DWORD		party_exp_distribute_table[PLAYER_EXP_TABLE_MAX + 1];
#ifdef ENABLE_YOHARA_SYSTEM
extern const DWORD exp_table_conqueror_common[PLAYER_CONQUEROR_LEVEL + 1];
extern const DWORD* conqueror_exp_table;
#endif
extern const DWORD		exp_table_common[PLAYER_MAX_LEVEL_CONST + 1];

extern const DWORD*		exp_table;
#ifdef __NEWPET_SYSTEM__
//extern const DWORD pets;
extern DWORD		exppet_table_common[121];
extern DWORD*		exppet_table;
extern const DWORD		Pet_SKill_TablePerc[3][20];
extern const DWORD		Pet_Skill_Table[18][23];
extern const DWORD		Pet_Table[4][2];
#endif
extern const DWORD		guild_exp_table[GUILD_MAX_LEVEL + 1];
extern const DWORD		guild_exp_table2[GUILD_MAX_LEVEL + 1];

#define MAX_EXP_DELTA_OF_LEV	31
#define PERCENT_LVDELTA(me, victim) aiPercentByDeltaLev[MINMAX(0, (victim + 15) - me, MAX_EXP_DELTA_OF_LEV - 1)]
#define PERCENT_LVDELTA_BOSS(me, victim) aiPercentByDeltaLevForBoss[MINMAX(0, (victim + 15) - me, MAX_EXP_DELTA_OF_LEV - 1)]
#define CALCULATE_VALUE_LVDELTA(me, victim, val) ((val * PERCENT_LVDELTA(me, victim)) / 100)
extern const int		aiPercentByDeltaLev_euckr[MAX_EXP_DELTA_OF_LEV];
extern const int		aiPercentByDeltaLevForBoss_euckr[MAX_EXP_DELTA_OF_LEV];
extern const int *		aiPercentByDeltaLev;
extern const int *		aiPercentByDeltaLevForBoss;

#define ARROUND_COORD_MAX_NUM	161
extern Coord			aArroundCoords[ARROUND_COORD_MAX_NUM];
extern TJobInitialPoints	JobInitialPoints[JOB_MAX_NUM];

extern const int		aiMobEnchantApplyIdx[MOB_ENCHANTS_MAX_NUM];
extern const int		aiMobResistsApplyIdx[MOB_RESISTS_MAX_NUM];

extern const int		aSkillAttackAffectProbByRank[MOB_RANK_MAX_NUM];

extern const int aiItemMagicAttributePercentHigh[ITEM_ATTRIBUTE_MAX_LEVEL]; // 1°³±îÁö
extern const int aiItemMagicAttributePercentLow[ITEM_ATTRIBUTE_MAX_LEVEL];

extern const int aiItemAttributeAddPercent[ITEM_ATTRIBUTE_MAX_NUM];

extern const int aiWeaponSocketQty[WEAPON_NUM_TYPES];
extern const int aiArmorSocketQty[ARMOR_NUM_TYPES];
extern const int aiSocketPercentByQty[5][4];

extern const int aiExpLossPercents[PLAYER_EXP_TABLE_MAX + 1];

extern const int * aiSkillPowerByLevel;
extern const int aiSkillPowerByLevel_euckr[SKILL_MAX_LEVEL + 1];

extern const int aiPolymorphPowerByLevel[SKILL_MAX_LEVEL + 1];

extern const int aiSkillBookCountForLevelUp[10];
extern const int aiGrandMasterSkillBookCountForLevelUp[10];
extern const int aiGrandMasterSkillBookMinCount[10];
extern const int aiGrandMasterSkillBookMaxCount[10];
extern const int CHN_aiPartyBonusExpPercentByMemberCount[9];
extern const int KOR_aiPartyBonusExpPercentByMemberCount[9];
extern const int KOR_aiUniqueItemPartyBonusExpPercentByMemberCount[9];

typedef std::map<DWORD, TItemAttrTable> TItemAttrMap;
extern TItemAttrMap g_map_itemAttr;
extern TItemAttrMap g_map_itemRare;
extern TItemAttrMap g_map_itemCostume;
extern TItemAttrMap g_map_itemCostumeHair;
extern TItemAttrMap g_map_itemRingsAttr;
extern TItemAttrMap g_map_itemRingsRare;

extern const int * aiChainLightningCountBySkillLevel;
extern const int aiChainLightningCountBySkillLevel_euckr[SKILL_MAX_LEVEL + 1];

extern const char * c_apszEmpireNames[EMPIRE_MAX_NUM];
extern const char * c_apszPrivNames[MAX_PRIV_NUM];
extern const SStoneDropInfo aStoneDrop[STONE_INFO_MAX_NUM];
extern const SChestDrop aChestDrop[CHEST_BOOK_INFO_MAX_NUM];

typedef struct
{
	long lMapIndex;
	int iWarPrice;
	int iWinnerPotionRewardPctToWinner;
	int iLoserPotionRewardPctToWinner;
	int iInitialScore;
	int iEndScore;
} TGuildWarInfo;

extern TGuildWarInfo KOR_aGuildWarInfo[GUILD_WAR_TYPE_MAX_NUM];

// ACCESSORY_REFINE
enum
{
	ITEM_ACCESSORY_SOCKET_MAX_NUM = 3
};

extern const int aiAccessorySocketAddPct[ITEM_ACCESSORY_SOCKET_MAX_NUM];
extern const int aiAccessorySocketEffectivePct[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1];
extern const int aiAccessorySocketDegradeTime[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1];
extern const int aiAccessorySocketPutPct[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1];
long FN_get_apply_type(const char *apply_type_string);

// END_OF_ACCESSORY_REFINE

long FN_get_apply_type(const char *apply_type_string);

#ifdef __DUNGEON_INFO__
extern const std::map<DWORD, std::pair<std::pair<BYTE, BYTE>, std::string>> m_mapDungeonList;
#endif
#endif

