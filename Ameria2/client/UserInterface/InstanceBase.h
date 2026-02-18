#pragma once

#include "../gamelib/RaceData.h"
#include "../gamelib/ActorInstance.h"
#include "../gamelib/GameLibDefines.h"

#include "AffectFlagContainer.h"
#ifdef ENABLE_SASH_SYSTEM
#include "../eterlib/GrpObjectInstance.h"
#endif
class CInstanceBase
{
	public:
		struct SCreateData
		{
			BYTE	m_bType;
			DWORD	m_dwStateFlags;
			DWORD	m_dwEmpireID;
			DWORD	m_dwGuildID;
			DWORD	m_dwLevel;
#ifdef ENABLE_YOHARA_SYSTEM
			DWORD	m_dwConquerorLevel;
#endif

			DWORD	m_dwVID;
			DWORD	m_dwRace;
			DWORD	m_dwMovSpd;
			DWORD	m_dwAtkSpd;
			long	m_lPosX;
			long	m_lPosY;
			FLOAT	m_fRot;
			DWORD	m_dwArmor;
			DWORD	m_dwWeapon;
			DWORD	m_dwHair;
			DWORD	m_dwMountVnum;
			#ifdef ENABLE_SASH_SYSTEM
			DWORD	m_dwSash;
			#endif

			DWORD	m_dwArrowType;


			short	m_sAlignment;
			BYTE	m_byPKMode;
#if defined(WJ_COMBAT_ZONE)
			BYTE	combat_zone_rank;
			DWORD	combat_zone_points;
#endif
			CAffectFlagContainer	m_kAffectFlags;

			std::string m_stName;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
			BYTE m_stLanguage;
#endif
#ifdef ENABLE_SHINING_SYSTEM
			DWORD	m_dwShining[7];
#endif
#ifdef ENABLE_AURA_SYSTEM
			DWORD	m_dwAura;
#endif

			bool	m_isMain;
		};

	public:
		typedef DWORD TType;

#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
		enum EAutoType
		{
			AUTO_ONOFF_START,
			AUTO_ONOFF_ATTACK,
			AUTO_ONOFF_RANGE,
			AUTO_ONOFF_POTION,
			AUTO_ONOFF_SKILL,
			AUTO_ONOFF_RESTART,
			AUTO_ONOFF_ATTACK_STONE,
			AUTO_ONOFF_MAX,
		};
#endif

		enum EDirection
		{
			DIR_NORTH,
			DIR_NORTHEAST,
			DIR_EAST,
			DIR_SOUTHEAST,
			DIR_SOUTH,
			DIR_SOUTHWEST,
			DIR_WEST,
			DIR_NORTHWEST,
			DIR_MAX_NUM,
		};

		enum
		{
			FUNC_WAIT,
			FUNC_MOVE,
			FUNC_ATTACK,
			FUNC_COMBO,
			FUNC_MOB_SKILL,
			FUNC_EMOTION,
			FUNC_SKILL = 0x80,
		};

		enum
		{
//			AFFECT_NONE,
			AFFECT_YMIR,
			AFFECT_INVISIBILITY,
			AFFECT_SPAWN,

			AFFECT_POISON,
			AFFECT_SLOW,
			AFFECT_STUN,

			AFFECT_DUNGEON_READY,			// 던전에서 준비 상태
			AFFECT_SHOW_ALWAYS,				// AFFECT_DUNGEON_UNIQUE 에서 변경(클라이언트에서 컬링되지않음)

			AFFECT_BUILDING_CONSTRUCTION_SMALL,
			AFFECT_BUILDING_CONSTRUCTION_LARGE,
			AFFECT_BUILDING_UPGRADE,

			AFFECT_MOV_SPEED_POTION,		// 11
			AFFECT_ATT_SPEED_POTION,		// 12

			AFFECT_FISH_MIND,				// 13

			AFFECT_JEONGWI,					// 14 전귀혼
			AFFECT_GEOMGYEONG,				// 15 검경
			AFFECT_CHEONGEUN,				// 16 천근추
			AFFECT_GYEONGGONG,				// 17 경공술
			AFFECT_EUNHYEONG,				// 18 은형법
			AFFECT_GWIGEOM,					// 19 귀검
			AFFECT_GONGPO,					// 20 공포
			AFFECT_JUMAGAP,					// 21 주마갑
			AFFECT_HOSIN,					// 22 호신
			AFFECT_BOHO,					// 23 보호
			AFFECT_KWAESOK,					// 24 쾌속
		    AFFECT_HEUKSIN,					// 25 흑신수호
			AFFECT_MUYEONG,					// 26 무영진
			AFFECT_REVIVE_INVISIBILITY,		// 27 부활 무적
			AFFECT_FIRE,					// 28 지속 불
			AFFECT_GICHEON,					// 29 기천 대공
			AFFECT_JEUNGRYEOK,				// 30 증력술
			AFFECT_DASH,					// 31 대쉬
			AFFECT_PABEOP,					// 32 파법술
			AFFECT_FALLEN_CHEONGEUN,		// 33 다운 그레이드 천근추
			AFFECT_POLYMORPH,				// 34 폴리모프
			AFFECT_WAR_FLAG1,				// 35
			AFFECT_WAR_FLAG2,				// 36
			AFFECT_WAR_FLAG3,				// 37
			AFFECT_CHINA_FIREWORK,			// 38
// #ifdef __BUFFI_SUPPORT__
			AFFECT_BUFFI_HOSIN,
			AFFECT_BUFFI_GICHEON,
			AFFECT_BUFFI_KWAESOK,
			AFFECT_BUFFI_JEUNGRYEO,
// #endif
			AFFECT_PREMIUM_SILVER,
			AFFECT_PREMIUM_GOLD,
			AFFECT_RAMADAN_RING,			// 41 초승달 반지 착용 Affect
// #ifdef ENABLE_WOLFMAN_CHARACTER
			AFFECT_BLEEDING,				// 42
			AFFECT_RED_POSSESSION,			// 43
			AFFECT_BLUE_POSSESSION,			// 44
// #endif
// #ifdef ENABLE_YOHARA_SYSTEM
			AFFECT_CHEONUN,
			AFFECT_CHUNWOON,
// #endif

// #ifdef ENABLE_MELEY_LAIR_DUNGEON
			AFFECT_STATUE1,
			AFFECT_STATUE2,
			AFFECT_STATUE3,
			AFFECT_STATUE4,
// #endif
			AFFECT_HOSIN_PERFECT,					// 54
			AFFECT_BOHO_PERFECT,                    // 55
			AFFECT_KWAESOK_PERFECT,                 // 56
			AFFECT_GICHEON_PERFECT,                 // 57 // sunt afectele prost sync sv client
			AFFECT_JEUNGRYEOK_PERFECT,              // 58
// #if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
			AFFECT_AUTO,
// #endif
#if defined(__BL_BATTLE_ROYALE__)
			AFFECT_BATTLE_ROYALE_CROWN,
			AFFECT_BATTLE_ROYALE_SLOW_1,
			AFFECT_BATTLE_ROYALE_SLOW_2,
			AFFECT_BATTLE_ROYALE_SLOW_3,
#endif

			AFFECT_NUM,

			AFFECT_HWAYEOM = AFFECT_GEOMGYEONG,
		};
		
		enum
		{
			NEW_AFFECT_MOV_SPEED            = 200,
			NEW_AFFECT_ATT_SPEED,
			NEW_AFFECT_ATT_GRADE,
			NEW_AFFECT_INVISIBILITY,
			NEW_AFFECT_STR,
			NEW_AFFECT_DEX,                 // 205
			NEW_AFFECT_CON,
			NEW_AFFECT_INT,
			NEW_AFFECT_FISH_MIND_PILL,

			NEW_AFFECT_POISON,
			NEW_AFFECT_STUN,                // 210
			NEW_AFFECT_SLOW,
			NEW_AFFECT_DUNGEON_READY,
			NEW_AFFECT_DUNGEON_UNIQUE,

			NEW_AFFECT_BUILDING,
			NEW_AFFECT_REVIVE_INVISIBLE,    // 215
			NEW_AFFECT_FIRE,
			NEW_AFFECT_CAST_SPEED,
			NEW_AFFECT_HP_RECOVER_CONTINUE,
			NEW_AFFECT_SP_RECOVER_CONTINUE,

			NEW_AFFECT_POLYMORPH,           // 220
			NEW_AFFECT_MOUNT,

			NEW_AFFECT_WAR_FLAG,            // 222

			NEW_AFFECT_BLOCK_CHAT,          // 223
			NEW_AFFECT_CHINA_FIREWORK,

			NEW_AFFECT_BOW_DISTANCE,        // 225

			NEW_AFFECT_EXP_BONUS         = 500, // 경험의 반지
			NEW_AFFECT_ITEM_BONUS        = 501, // 도둑의 장갑
			NEW_AFFECT_SAFEBOX           = 502, // PREMIUM_SAFEBOX,
			NEW_AFFECT_AUTOLOOT          = 503, // PREMIUM_AUTOLOOT,
			NEW_AFFECT_FISH_MIND         = 504, // PREMIUM_FISH_MIND,
			NEW_AFFECT_MARRIAGE_FAST     = 505, // 원앙의 깃털 (금슬),
			NEW_AFFECT_GOLD_BONUS        = 506,

		    NEW_AFFECT_MALL              = 510, // 몰 아이템 에펙트
			NEW_AFFECT_NO_DEATH_PENALTY  = 511, // 용신의 가호 (경험치 패널티를 한번 막아준다)
			NEW_AFFECT_SKILL_BOOK_BONUS  = 512, // 선인의 교훈 (책 수련 성공 확률이 50% 증가)
			NEW_AFFECT_SKILL_BOOK_NO_DELAY  = 513, // 주안 술서 (책 수련 딜레이 없음)

			NEW_AFFECT_EXP_BONUS_EURO_FREE = 516, // 경험의 반지 (유럽 버전 14 레벨 이하 기본 효과)
			NEW_AFFECT_EXP_BONUS_EURO_FREE_UNDER_15 = 517,

			NEW_AFFECT_AUTO_HP_RECOVERY		= 534,		// 자동물약 HP
			NEW_AFFECT_AUTO_SP_RECOVERY		= 535,		// 자동물약 SP


			NEW_AFFECT_DRAGON_SOUL_QUALIFIED = 540,
			NEW_AFFECT_DRAGON_SOUL_DECK1 = 541,
			NEW_AFFECT_DRAGON_SOUL_DECK2 = 542,
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
			AFFECT_CRYSTAL = 583,
#endif
			NEW_AFFECT_RAMADAN_ABILITY = 300,
			NEW_AFFECT_RAMADAN_RING    = 301,			// 라마단 이벤트용 특수아이템 초승달의 반지 착용 유무

			NEW_AFFECT_NOG_POCKET_ABILITY = 302,

#if defined(__BL_BATTLE_ROYALE__)
			NEW_AFFECT_BATTLE_ROYALE_CROWN = 688,
			NEW_AFFECT_BATTLE_ROYALE_FIELD_DAMAGE = 689,
			NEW_AFFECT_BATTLE_ROYALE_SLOW = 690,
			NEW_AFFECT_BATTLE_ROYALE_MOVE_SPEED = 691,
			NEW_AFFECT_BATTLE_ROYALE_INFINITE_STAMINA = 692,
#endif

#ifdef ENABLE_MULTI_FARM_BLOCK
			NEW_AFFECT_MULTI_FARM = 704,
#endif

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
			NEW_AFFECT_AUTO_PICK_UP = 705,
#endif
#ifdef ENABLE_DS_SET_RENEWAL
			NEW_AFFECT_DS_SET = 706,
			NEW_AFFECT_DS_BNS1 = 707,
			NEW_AFFECT_DS_BNS2 = 708,
			NEW_AFFECT_DS_BNS3 = 709,
#endif
			NEW_AFFECT_THIEF = 710,
			NEW_AFFECT_EXP = 711,
#ifdef USE_DROP_AFFECT_BONUSES
			AFFECT_DROP_GREEN = 712,
			AFFECT_DROP_BLUE = 713,
#endif
#ifdef USE_PREMIUM_AFFECT
			AFFECT_PREMIUM_USER = 714,
#endif
#ifdef ENABLE_VOTE4BUFF
			NEW_AFFECT_VOTE4BUFF = 809,
#endif
#ifdef __RENEWAL_BRAVE_CAPE__
			NEW_AFFECT_BRAVE_CAPE = 813,
#endif
#ifdef ENABLE_QUEEN_NETHIS
			NEW_AFFECT_SUNGMA_MOVE_DEBUFF = 815,
			NEW_AFFECT_SUNGMA_STR_DEBUFF = 816,
#endif

// #if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM) && !defined(USE_PREMIUM_AFFECT)
			NEW_AFFECT_AUTO_HUNT = 840,
// #endif
// #if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
			NEW_AFFECT_AUTO_USE = 841,
// #endif
#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_ULTIMATE
			NEW_AFFECT_SEARCH_SHOP_PREMIUM = 550,
#endif
#endif
#ifdef VOTE_BUFF_RENEWAL
			AFFECT_VOTEFORBONUS,
#endif
#ifdef ENABLE_SUNG_MAHI_TOWER
			NEW_AFFECT_SUNG_MAHI_BP,
			NEW_AFFECT_SUNG_MAHI_CURSE,
#endif
			AFFECT_NAME_PET,
			AFFECT_NAME_MOUNT,
			AFFECT_NAME_BUFFI,

			NEW_AFFECT_QUEST_START_IDX   = 1000,
		};

		enum
		{
			STONE_SMOKE1 = 0,	// 99%
			STONE_SMOKE2 = 1,	// 85%
			STONE_SMOKE3 = 2,	// 80%
			STONE_SMOKE4 = 3,	// 60%
			STONE_SMOKE5 = 4,	// 45%
			STONE_SMOKE6 = 5,	// 40%
			STONE_SMOKE7 = 6,	// 20%
			STONE_SMOKE8 = 7,	// 10%
			STONE_SMOKE_NUM = 4,
		};

		enum EBuildingAffect
		{
			BUILDING_CONSTRUCTION_SMALL = 0,
			BUILDING_CONSTRUCTION_LARGE = 1,
			BUILDING_UPGRADE = 2,
		};

		enum
		{
			WEAPON_DUALHAND,
			WEAPON_ONEHAND,
			WEAPON_TWOHAND,
			WEAPON_NUM,
		};

		enum
		{
			EMPIRE_NONE,
			EMPIRE_A,
			EMPIRE_B,
			EMPIRE_C,
			EMPIRE_NUM,
		};

		enum
		{
			NAMECOLOR_MOB,
			NAMECOLOR_NPC,
			NAMECOLOR_PC,
			NAMECOLOR_PC_END = NAMECOLOR_PC + EMPIRE_NUM,
			NAMECOLOR_NORMAL_MOB,
			NAMECOLOR_NORMAL_NPC,
			NAMECOLOR_NORMAL_PC,
			NAMECOLOR_NORMAL_PC_END = NAMECOLOR_NORMAL_PC + EMPIRE_NUM,
			NAMECOLOR_EMPIRE_MOB,
			NAMECOLOR_EMPIRE_NPC,
			NAMECOLOR_EMPIRE_PC,
			NAMECOLOR_EMPIRE_PC_END = NAMECOLOR_EMPIRE_PC + EMPIRE_NUM,
			NAMECOLOR_FUNC,
			NAMECOLOR_PK,
			NAMECOLOR_PVP,
			NAMECOLOR_PARTY,
			NAMECOLOR_WARP,
			NAMECOLOR_WAYPOINT,
#ifdef NAMECOLOR_BOSS_CLIENT
			NAMECOLOR_BOSS,
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
			NAMECOLOR_TOURNAMENT_TEAM_A,
			NAMECOLOR_TOURNAMENT_TEAM_B,
#endif	
#ifdef ENABLE_YOHARA_SYSTEM
			NAMECOLOR_CONQUEROR,
#endif
#ifdef ENABLE_STONEMINIMAP
			NAMECOLOR_METIN,
#endif

			NAMECOLOR_EXTRA = NAMECOLOR_FUNC + 10,
			NAMECOLOR_NUM = NAMECOLOR_EXTRA + 10,
		};

		enum ETitleColor
		{
			TITLE_RGB_GOOD_4,
			TITLE_RGB_GOOD_3,
			TITLE_RGB_GOOD_2,
			TITLE_RGB_GOOD_1,
			TITLE_RGB_NORMAL,
			TITLE_RGB_EVIL_1,
			TITLE_RGB_EVIL_2,
			TITLE_RGB_EVIL_3,
			TITLE_RGB_EVIL_4,
		};

		enum
		{
			ALIGNMENT_TYPE_WHITE,
			ALIGNMENT_TYPE_NORMAL,
			ALIGNMENT_TYPE_DARK,
		};

		enum
		{
			EMOTICON_EXCLAMATION	= 1,
			EMOTICON_FISH			= 11,
			EMOTICON_NUM			= 128,

			TITLE_NUM				= 9,
			TITLE_NONE				= 4,
		};

		enum	//아래 번호가 바뀌면 registerEffect 쪽도 바꾸어 줘야 한다.
		{
			EFFECT_REFINED_NONE,

/*---------------------------------------------------*/
			/*---------------SWORD---------------*/
			EFFECT_SWORD_REFINED7,
			EFFECT_SWORD_REFINED8,
			EFFECT_SWORD_REFINED9,
#ifdef ENABLE_LVL96_WEAPON_EFFECT
			EFFECT_SWORD_REFINED10,
#endif

			/*---------------BOW---------------*/
			EFFECT_BOW_REFINED7,
			EFFECT_BOW_REFINED8,
			EFFECT_BOW_REFINED9,
#ifdef ENABLE_LVL96_WEAPON_EFFECT
			EFFECT_BOW_REFINED10,
#endif

			/*---------------FAN---------------*/
			EFFECT_FANBELL_REFINED7,
			EFFECT_FANBELL_REFINED8,
			EFFECT_FANBELL_REFINED9,
#ifdef ENABLE_LVL96_WEAPON_EFFECT
			EFFECT_FANBELL_REFINED10,
#endif

			/*---------------DAGGER---------------*/
			//RIGHT
			EFFECT_SMALLSWORD_REFINED7,
			EFFECT_SMALLSWORD_REFINED8,
			EFFECT_SMALLSWORD_REFINED9,
#ifdef ENABLE_LVL96_WEAPON_EFFECT
			EFFECT_SMALLSWORD_REFINED10,
#endif
			//LEFT
			EFFECT_SMALLSWORD_REFINED7_LEFT,
			EFFECT_SMALLSWORD_REFINED8_LEFT,
			EFFECT_SMALLSWORD_REFINED9_LEFT,
#ifdef ENABLE_LVL96_WEAPON_EFFECT
			EFFECT_SMALLSWORD_REFINED10_LEFT,
#endif

			/*---------------CLAW---------------*/
#ifdef ENABLE_WOLFMAN_CHARACTER
			//RIGHT
			EFFECT_SWORD_REFINED7_W,
			EFFECT_SWORD_REFINED8_W,
			EFFECT_SWORD_REFINED9_W,
#	ifdef ENABLE_LVL96_WEAPON_EFFECT
			EFFECT_SWORD_REFINED10_W,
#	endif
			//LEFT
			EFFECT_SWORD_REFINED7_W_LEFT,
			EFFECT_SWORD_REFINED8_W_LEFT,
			EFFECT_SWORD_REFINED9_W_LEFT,
#	ifdef ENABLE_LVL96_WEAPON_EFFECT
			EFFECT_SWORD_REFINED10_W_LEFT,
#	endif
#endif

/*---------------------------------------------------*/
			/*---------------ARMOR---------------*/
			EFFECT_BODYARMOR_REFINED7,
			EFFECT_BODYARMOR_REFINED8,
			EFFECT_BODYARMOR_REFINED9,
#ifdef ENABLE_LVL96_ARMOR_EFFECT
			EFFECT_BODYARMOR_REFINED10,
#endif
			EFFECT_BODYARMOR_SPECIAL,	// 4-2-1	Bubbles-Effekt
			EFFECT_BODYARMOR_SPECIAL2,	// 4-2-2	Blue-Shining-Effekt
#ifdef ENABLE_LVL115_ARMOR_EFFECT
			EFFECT_BODYARMOR_SPECIAL3,	// 5-1		Blitze-Effekt
#endif

#ifdef ENABLE_SASH_SYSTEM
			EFFECT_SASH,
#endif
			EFFECT_REFINED_NUM,
		};

#ifdef ENABLE_SHINING_SYSTEM
		enum
		{
			EFFECT_SHINING_WEAPON_SWORD,
			EFFECT_SHINING_WEAPON_SWORD_END = EFFECT_SHINING_WEAPON_SWORD + 1999,
			EFFECT_SHINING_WEAPON_TWO_HANDED,
			EFFECT_SHINING_WEAPON_TWO_HANDED_END = EFFECT_SHINING_WEAPON_TWO_HANDED + 1999,
			EFFECT_SHINING_WEAPON_BOW,
			EFFECT_SHINING_WEAPON_BOW_END = EFFECT_SHINING_WEAPON_BOW + 1999,
			EFFECT_SHINING_WEAPON_BELL,
			EFFECT_SHINING_WEAPON_BELL_END = EFFECT_SHINING_WEAPON_BELL + 1999,
			EFFECT_SHINING_WEAPON_FAN,
			EFFECT_SHINING_WEAPON_FAN_END = EFFECT_SHINING_WEAPON_FAN + 1999,
			EFFECT_SHINING_WEAPON_DAGGER_R,
			EFFECT_SHINING_WEAPON_DAGGER_R_END = EFFECT_SHINING_WEAPON_DAGGER_R + 1999,
			EFFECT_SHINING_WEAPON_DAGGER_L,
			EFFECT_SHINING_WEAPON_DAGGER_L_END = EFFECT_SHINING_WEAPON_DAGGER_L + 1999,
			EFFECT_SHINING_WEAPON_NUM,
		};
#endif

		enum DamageFlag
		{
			DAMAGE_NORMAL	= (1<<0),
			DAMAGE_POISON	= (1<<1),
			DAMAGE_DODGE	= (1<<2),
			DAMAGE_BLOCK	= (1<<3),
			DAMAGE_PENETRATE= (1<<4),
			DAMAGE_CRITICAL = (1<<5),
			// 반-_-사
		};

		enum
		{
			EFFECT_DUST,
			EFFECT_STUN,
			EFFECT_HIT,
			EFFECT_FLAME_ATTACK,
			EFFECT_FLAME_HIT,
			EFFECT_FLAME_ATTACH,
			EFFECT_ELECTRIC_ATTACK,
			EFFECT_ELECTRIC_HIT,
			EFFECT_ELECTRIC_ATTACH,
			EFFECT_SPAWN_APPEAR,
			EFFECT_SPAWN_DISAPPEAR,
			EFFECT_LEVELUP,
			EFFECT_SKILLUP,
			EFFECT_HPUP_RED,
			EFFECT_SPUP_BLUE,
			EFFECT_SPEEDUP_GREEN,
			EFFECT_DXUP_PURPLE,
			EFFECT_CRITICAL,
			EFFECT_PENETRATE,
			EFFECT_BLOCK,
			EFFECT_DODGE,
			EFFECT_FIRECRACKER,
			EFFECT_SPIN_TOP,
			EFFECT_WEAPON,
			EFFECT_WEAPON_END = EFFECT_WEAPON + WEAPON_NUM,
			EFFECT_AFFECT,
			EFFECT_AFFECT_GYEONGGONG = EFFECT_AFFECT + AFFECT_GYEONGGONG,
			EFFECT_AFFECT_KWAESOK = EFFECT_AFFECT + AFFECT_KWAESOK,
#ifdef ENABLE_PERFECT_BUFF
			EFFECT_AFFECT_KWAESOK_PERFECT = EFFECT_AFFECT + AFFECT_KWAESOK_PERFECT,
#endif		
			EFFECT_AFFECT_END = EFFECT_AFFECT + AFFECT_NUM,
			EFFECT_EMOTICON,
			EFFECT_EMOTICON_END = EFFECT_EMOTICON + EMOTICON_NUM,
			EFFECT_SELECT,
			EFFECT_TARGET,
			EFFECT_EMPIRE,
			EFFECT_EMPIRE_END = EFFECT_EMPIRE + EMPIRE_NUM,
			EFFECT_HORSE_DUST,
			EFFECT_REFINED,
			EFFECT_REFINED_END = EFFECT_REFINED + EFFECT_REFINED_NUM,
			EFFECT_DAMAGE_TARGET,
			EFFECT_DAMAGE_NOT_TARGET,
			EFFECT_DAMAGE_SELFDAMAGE,
			EFFECT_DAMAGE_SELFDAMAGE2,
			EFFECT_DAMAGE_POISON,
			EFFECT_DAMAGE_MISS,
			EFFECT_DAMAGE_TARGETMISS,
			EFFECT_DAMAGE_CRITICAL,
			EFFECT_SUCCESS,
			EFFECT_FAIL,
			EFFECT_FR_SUCCESS,
			EFFECT_LEVELUP_ON_14_FOR_GERMANY,	//레벨업 14일때 ( 독일전용 )
			EFFECT_LEVELUP_UNDER_15_FOR_GERMANY,//레벨업 15일때 ( 독일전용 )
			EFFECT_PERCENT_DAMAGE1,
			EFFECT_PERCENT_DAMAGE2,
			EFFECT_PERCENT_DAMAGE3,
			EFFECT_AUTO_HPUP,
			EFFECT_AUTO_SPUP,
			EFFECT_RAMADAN_RING_EQUIP,			// 초승달 반지 착용 순간에 발동하는 이펙트
			EFFECT_HALLOWEEN_CANDY_EQUIP,		// 할로윈 사탕 착용 순간에 발동하는 이펙트
			EFFECT_HAPPINESS_RING_EQUIP,				// 행복의 반지 착용 순간에 발동하는 이펙트
			EFFECT_LOVE_PENDANT_EQUIP,				// 행복의 반지 착용 순간에 발동하는 이펙트
			EFFECT_TEMP,
#if defined(WJ_COMBAT_ZONE)
			EFFECT_COMBAT_ZONE_POTION,
#endif
			#ifdef ENABLE_SASH_SYSTEM
			EFFECT_SASH_SUCCEDED,
			EFFECT_SASH_EQUIP,
			#endif
#ifdef VERSION_162_ENABLED
			EFFECT_HEALER,
#endif
#ifdef ENABLE_SHINING_SYSTEM
			EFFECT_SHINING_WEAPON,
			EFFECT_SHINING_WEAPON_END = EFFECT_SHINING_WEAPON + EFFECT_SHINING_WEAPON_NUM,
			EFFECT_SHINING_ARMOR,
			EFFECT_SHINING_ARMOR_END = EFFECT_SHINING_ARMOR + 20,
			EFFECT_SHINING_SPECIAL,
			EFFECT_SHINING_SPECIAL_END = EFFECT_SHINING_SPECIAL + 10,
#endif
#ifdef ENABLE_ZODIAC_MISSION
			EFFECT_SKILL_DAMAGE_ZONE,
			EFFECT_SKILL_DAMAGE_ZONE_BUYUK,
			EFFECT_SKILL_DAMAGE_ZONE_ORTA,
			EFFECT_SKILL_DAMAGE_ZONE_KUCUK,
			EFFECT_SKILL_SAFE_ZONE,
			EFFECT_SKILL_SAFE_ZONE_BUYUK,
			EFFECT_SKILL_SAFE_ZONE_ORTA,
			EFFECT_SKILL_SAFE_ZONE_KUCUK,
			EFFECT_METEOR,
			EFFECT_BEAD_RAIN,
			EFFECT_FALL_ROCK,
			EFFECT_ARROW_RAIN,
			EFFECT_HORSE_DROP,
			EFFECT_EGG_DROP,
			EFFECT_DEAPO_BOOM,
#endif
#ifdef ENABLE_QUEEN_NETHIS
			EFFECT_SNAKE_REGEN,
#endif
			EFFECT_DAMAGE_PENETRATE,
			EFFECT_METIN_QUEUE,
			EFFECT_BOSS,
//#ifdef ENABLE_VOICE_CHAT
			EFFECT_VOICE_CHAT,
//#endif
			EFFECT_NUM,
		};

		enum
		{
			DUEL_NONE,
			DUEL_CANNOTATTACK,
			DUEL_START,
		};

#ifdef __BUFFI_SUPPORT__
	public:
		bool	IsBuffi() { return m_bIsBuffi; }
		void	SetBuffi(bool bFlag) { m_bIsBuffi = bFlag; }
	protected:
		bool	m_bIsBuffi;
#endif

	public:
		static void DestroySystem();
		static void CreateSystem(UINT uCapacity);
		static bool RegisterEffect(UINT eEftType, const char* c_szEftAttachBone, const char* c_szEftName, bool isCache);
		static void RegisterTitleName(int iIndex, const char * c_szTitleName);
		static bool RegisterNameColor(UINT uIndex, UINT r, UINT g, UINT b);
		static bool RegisterTitleColor(UINT uIndex, UINT r, UINT g, UINT b);
		static bool ChangeEffectTexture(UINT eEftType, const char* c_szSrcFileName, const char* c_szDstFileName);

		static void SetDustGap(float fDustGap);
		static void SetHorseDustGap(float fDustGap);

		static void SetEmpireNameMode(bool isEnable);
		static const D3DXCOLOR& GetIndexedNameColor(UINT eNameColor);

	public:
		void SetMainInstance();

		void OnSelected();
		void OnUnselected();
		void OnTargeted();
		void OnUntargeted();

#ifdef __AUTO_QUQUE_ATTACK__
		void SetAutoFarmAffect(const bool bStatus);
#endif

	protected:
		bool __IsExistMainInstance();
		bool __IsMainInstance();
		bool __MainCanSeeHiddenThing();
		float __GetBowRange();

	protected:
		DWORD	__AttachEffect(UINT eEftType, float fScale = 1.0f);
		DWORD	__AttachEffect(char filename[128]);
		void	__DetachEffect(DWORD dwEID);

	public:
		void CreateSpecialEffect(DWORD iEffectIndex, float fScale = 1.0f);
		void AttachSpecialEffect(DWORD effect, float fScale = 1.0f);

	protected:
		static std::string ms_astAffectEffectAttachBone[EFFECT_NUM];
		static DWORD ms_adwCRCAffectEffect[EFFECT_NUM];
		static float ms_fDustGap;
		static float ms_fHorseDustGap;

	public:
		CInstanceBase();
		virtual ~CInstanceBase();

		bool LessRenderOrder(CInstanceBase* pkInst);

		void MountHorse(UINT eRace);
		void DismountHorse();
#ifdef RACE_HEIGHT
		float					GetBaseHeight();
#endif
		// 스크립트용 테스트 함수. 나중에 없에자
		void SCRIPT_SetAffect(UINT eAffect, bool isVisible);

		float CalculateDistanceSq3d(const TPixelPosition& c_rkPPosDst);

		// Instance Data
		bool IsFlyTargetObject();
		void ClearFlyTargetInstance();
		void SetFlyTargetInstance(CInstanceBase& rkInstDst);
		void AddFlyTargetInstance(CInstanceBase& rkInstDst);
		void AddFlyTargetPosition(const TPixelPosition& c_rkPPosDst);

		float GetFlyTargetDistance();

		void SetAlpha(float fAlpha);

		void DeleteBlendOut();

		void					AttachTextTail();
		void					UpdateStoneScale();
		void					DetachTextTail();
		void					UpdateTextTailLevel(DWORD level);
#ifdef ENABLE_YOHARA_SYSTEM
		void					UpdateTextTailConquerorLevel(DWORD level);
		DWORD GetConquerorLevel();
		void SetConquerorLevel(DWORD dwcLevel);
#endif

#ifdef ENABLE_SHINING_SYSTEM
		void					SetShining(BYTE num, DWORD dwShining);
#endif

		void					RefreshTextTail();
		void					RefreshTextTailTitle();

		bool					Create(const SCreateData& c_rkCreateData);

		bool					CreateDeviceObjects();
		void					DestroyDeviceObjects();
		void					__AttachBossEffect();
		void					Destroy();

		void					Update();
		bool					UpdateDeleting();

		void					Transform();
		void					Deform();
		void					Render();
		void					RenderTrace();
		void					RenderToShadowMap();
		void					RenderCollision();
		void					RegisterBoundingSphere();

		// Temporary
		void					GetBoundBox(D3DXVECTOR3 * vtMin, D3DXVECTOR3 * vtMax);

		void					SetNameString(const char* c_szName, int len);
		bool					SetRace(DWORD dwRaceIndex);
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		void					SetLanguage(BYTE c_szName);
#endif
		void					SetVirtualID(DWORD wVirtualNumber);
		void					SetVirtualNumber(DWORD dwVirtualNumber);
		void					SetInstanceType(int iInstanceType);
		void					SetAlignment(short sAlignment);
		void					SetLevelText(int mLevel);
#ifdef ENABLE_YOHARA_SYSTEM
		void					SetConquerorLevelText(int iLevel);
#endif

		void					SetPKMode(BYTE byPKMode);
		void					SetKiller(bool bFlag);
		void					SetPartyMemberFlag(bool bFlag);
		void					SetStateFlags(DWORD dwStateFlags);

		void					SetArmor(DWORD dwArmor);
		void					SetShape(DWORD eShape, float fSpecular=0.0f);
		void					SetHair(DWORD eHair);
		#ifdef ENABLE_SASH_SYSTEM
		void	SetSash(DWORD dwSash);
		void	ChangeSash(DWORD dwSash);
		#endif
		bool					SetWeapon(DWORD eWeapon, DWORD eArrow = 0);

		//bool					SetWeapon(DWORD eWeapon);
		bool					ChangeArmor(DWORD dwArmor);

		void					ChangeWeapon(DWORD eWeapon, DWORD eArrow = 0);


	//	void					ChangeWeapon(DWORD eWeapon);
		void					ChangeHair(DWORD eHair);
		void					ChangeGuild(DWORD dwGuildID);
		DWORD					GetWeaponType();

		void					SetComboType(UINT uComboType);
		void					SetAttackSpeed(UINT uAtkSpd);
		void					SetMoveSpeed(UINT uMovSpd);
		void					SetRotationSpeed(float fRotSpd);

		const char *			GetNameString();
#if defined(WJ_COMBAT_ZONE)
		bool					IsCombatZoneMap();
		void					SetCombatZonePoints(DWORD dwValue);
		DWORD					GetCombatZonePoints();
		void					SetCombatZoneRank(BYTE bValue);
		BYTE					GetCombatZoneRank();
#endif
#if defined(ENABLE_LEVEL_IN_TRADE) || defined(ENABLE_NEW_EXCHANGE_WINDOW)
		DWORD					GetLevel();
#endif
		int						GetInstanceType();
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		BYTE					GetLanguage();
#endif

		DWORD					GetPart(CRaceData::EParts part);
		DWORD					GetShape();
		DWORD					GetRace();
		DWORD					GetVirtualID();
		DWORD					GetVirtualNumber();
		DWORD					GetEmpireID();
		DWORD					GetGuildID();
		int						GetAlignment();
		UINT					GetAlignmentGrade();
		int						GetAlignmentType();
		BYTE					GetPKMode();
		bool					IsKiller();
		bool					IsPartyMember();

#ifdef TOURNAMENT_PVP_SYSTEM
		bool					IsTournamentMap();
#endif

		void					ActDualEmotion(CInstanceBase & rkDstInst, WORD dwMotionNumber1, WORD dwMotionNumber2);
		void					ActEmotion(DWORD dwMotionNumber);
		void					LevelUp();
		void					SkillUp();
		void					UseSpinTop();
		void					Revive();
		void					Stun();
		void					Die();
		void					Hide();
		void					Show();

		bool					CanAct();
		bool					CanMove();
		bool					CanAttack();
		bool					CanUseSkill();
		bool					CanFishing();
		bool					IsConflictAlignmentInstance(CInstanceBase& rkInstVictim);
		bool					IsAttackableInstance(CInstanceBase& rkInstVictim);
		bool					IsTargetableInstance(CInstanceBase& rkInstVictim);
		bool					IsPVPInstance(CInstanceBase& rkInstVictim);
		bool					CanChangeTarget();
		bool					CanPickInstance();
		bool					CanViewTargetHP(CInstanceBase& rkInstVictim);


		// Movement
		BOOL					IsGoing();
		bool					NEW_Goto(const TPixelPosition& c_rkPPosDst, float fDstRot);
		void					EndGoing();

		void					SetRunMode();
		void					SetWalkMode();

		bool					IsAffect(UINT uAffect);
		BOOL					IsInvisibility();
		BOOL					IsParalysis();
		BOOL					IsGameMaster();
		BOOL					IsSameEmpire(CInstanceBase& rkInstDst);
		BOOL					IsBowMode();
		BOOL					IsHandMode();
		BOOL					IsFishingMode();
		BOOL					IsFishing();

		BOOL					IsWearingDress();
		BOOL					IsHoldingPickAxe();
		BOOL					IsMountingHorse();
		BOOL					IsNewMount();
		BOOL					IsForceVisible();
		BOOL					IsInSafe();
		BOOL					IsEnemy();
#ifdef NEW_PET_SYSTEM
		BOOL					IsNewPet();
#endif
#ifdef NAMECOLOR_BOSS_CLIENT
		BOOL                    IsBoss();
#endif
		BOOL					IsStone();
		BOOL					IsResource();
		BOOL					IsNPC();
		BOOL					IsPC();
#ifdef ENABLE_DEFENSAWESHIP
		BOOL					IsHydraNPC();
#endif
		BOOL					IsMount();
#ifdef ENABLE_NEW_PET_SYSTEM
		BOOL					IsPet();
		
		BOOL					IsNewPet();
		void					RefreshModelScale();
#endif
		BOOL					IsPoly();
		BOOL					IsWarp();
		BOOL					IsGoto();
		BOOL					IsObject();
		BOOL					IsDoor();
		BOOL					IsBuilding();
		BOOL					IsWoodenDoor();
		BOOL					IsStoneDoor();
		BOOL					IsFlag();
		BOOL					IsGuildWall();

		BOOL					IsDead();
		BOOL					IsStun();
		BOOL					IsSleep();
		BOOL					__IsSyncing();
		BOOL					IsWaiting();
		BOOL					IsWalking();
		BOOL					IsPushing();
		BOOL					IsAttacking();
		BOOL					IsActingEmotion();
		BOOL					IsAttacked();
		BOOL					IsKnockDown();
		BOOL					IsUsingSkill();
		BOOL					IsUsingMovingSkill();
		BOOL					CanCancelSkill();
		BOOL					CanAttackHorseLevel();

#ifdef __MOVIE_MODE__
		BOOL					IsMovieMode(); // 운영자용 완전히 안보이는거
#endif
		bool					NEW_CanMoveToDestPixelPosition(const TPixelPosition& c_rkPPosDst);

		void					NEW_SetAdvancingRotationFromPixelPosition(const TPixelPosition& c_rkPPosSrc, const TPixelPosition& c_rkPPosDst);
		void					NEW_SetAdvancingRotationFromDirPixelPosition(const TPixelPosition& c_rkPPosDir);
		bool					NEW_SetAdvancingRotationFromDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		void					SetAdvancingRotation(float fRotation);

		void					EndWalking(float fBlendingTime=0.15f);
		void					EndWalkingWithoutBlending();

		// Battle
		void					SetEventHandler(CActorInstance::IEventHandler* pkEventHandler);

		void					PushUDPState(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg);
		void					PushTCPState(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg);
		void					PushTCPStateExpanded(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg, UINT uTargetVID);

		void					NEW_Stop();

		bool					NEW_UseSkill(UINT uSkill, UINT uMot, UINT uMotLoopCount, bool isMovingSkill);
		void					NEW_Attack();
		void					NEW_Attack(float fDirRot);
		void					NEW_AttackToDestPixelPositionDirection(const TPixelPosition& c_rkPPosDst);
		bool					NEW_AttackToDestInstanceDirection(CInstanceBase& rkInstDst, IFlyEventHandler* pkFlyHandler);
		bool					NEW_AttackToDestInstanceDirection(CInstanceBase& rkInstDst);

		bool					NEW_MoveToDestPixelPositionDirection(const TPixelPosition& c_rkPPosDst);
		void					NEW_MoveToDestInstanceDirection(CInstanceBase& rkInstDst);
		void					NEW_MoveToDirection(float fDirRot);

		float					NEW_GetDistanceFromDirPixelPosition(const TPixelPosition& c_rkPPosDir);
		float					NEW_GetDistanceFromDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		float					NEW_GetDistanceFromDestInstance(CInstanceBase& rkInstDst);

		float					NEW_GetRotation();
		float					NEW_GetRotationFromDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		float					NEW_GetRotationFromDirPixelPosition(const TPixelPosition& c_rkPPosDir);
		float					NEW_GetRotationFromDestInstance(CInstanceBase& rkInstDst);

		float					NEW_GetAdvancingRotationFromDirPixelPosition(const TPixelPosition& c_rkPPosDir);
		float					NEW_GetAdvancingRotationFromDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		float					NEW_GetAdvancingRotationFromPixelPosition(const TPixelPosition& c_rkPPosSrc, const TPixelPosition& c_rkPPosDst);

		BOOL					NEW_IsClickableDistanceDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		BOOL					NEW_IsClickableDistanceDestInstance(CInstanceBase& rkInstDst);

		bool					NEW_GetFrontInstance(CInstanceBase ** ppoutTargetInstance, float fDistance);
		void					NEW_GetRandomPositionInFanRange(CInstanceBase& rkInstTarget, TPixelPosition* pkPPosDst);
		bool					NEW_GetInstanceVectorInFanRange(float fSkillDistance, CInstanceBase& rkInstTarget, std::vector<CInstanceBase*>* pkVct_pkInst);
		bool					NEW_GetInstanceVectorInCircleRange(float fSkillDistance, std::vector<CInstanceBase*>* pkVct_pkInst);

		void					NEW_SetOwner(DWORD dwOwnerVID);
		void					NEW_SyncPixelPosition(long & nPPosX, long & nPPosY);
		void					NEW_SyncCurrentPixelPosition();

		void					NEW_SetPixelPosition(const TPixelPosition& c_rkPPosDst);

		bool					NEW_IsLastPixelPosition();
		const TPixelPosition&	NEW_GetLastPixelPositionRef();


		// Battle
		BOOL					isNormalAttacking();
		BOOL					isComboAttacking();
		MOTION_KEY				GetNormalAttackIndex();
		DWORD					GetComboIndex();
		float					GetAttackingElapsedTime();
		void					InputNormalAttack(float fAtkDirRot);
		void					InputComboAttack(float fAtkDirRot);

		void					RunNormalAttack(float fAtkDirRot);
		void					RunComboAttack(float fAtkDirRot, DWORD wMotionIndex);

		CInstanceBase*			FindNearestVictim();
		BOOL					CheckAdvancing();


		bool					AvoidObject(const CGraphicObjectInstance& c_rkBGObj);
		bool					IsBlockObject(const CGraphicObjectInstance& c_rkBGObj);
		void					BlockMovement();
	public:
		BOOL					CheckAttacking(CInstanceBase& rkInstVictim);
		void					ProcessHitting(DWORD dwMotionKey, CInstanceBase * pVictimInstance);
		void					ProcessHitting(DWORD dwMotionKey, BYTE byEventIndex, CInstanceBase * pVictimInstance);
		void					GetBlendingPosition(TPixelPosition * pPixelPosition);
		void					SetBlendingPosition(const TPixelPosition & c_rPixelPosition);

		// Fishing
		void					StartFishing(float frot);
		void					StopFishing();
		void					ReactFishing();
		void					CatchSuccess();
		void					CatchFail();
		BOOL					GetFishingRot(int * pirot);

		// Render Mode
		void					RestoreRenderMode();
		void					SetAddRenderMode();
		void					SetModulateRenderMode();
		void					SetRenderMode(int iRenderMode);
		void					SetAddColor(const D3DXCOLOR & c_rColor);

		// Position
		void					SCRIPT_SetPixelPosition(float fx, float fy);
		void					NEW_GetPixelPosition(TPixelPosition * pPixelPosition);

		// Rotation
		void					NEW_LookAtFlyTarget();
		void					NEW_LookAtDestInstance(CInstanceBase& rkInstDst);
		void					NEW_LookAtDestPixelPosition(const TPixelPosition& c_rkPPosDst);

		float					GetRotation();
		float					GetAdvancingRotation();
		void					SetRotation(float fRotation);
		void					BlendRotation(float fRotation, float fBlendTime = 0.1f);

		void					SetDirection(int dir);
		void					BlendDirection(int dir, float blendTime);
		float					GetDegreeFromDirection(int dir);

		// Motion
		//	Motion Deque
		BOOL					isLock();

		void					SetMotionMode(int iMotionMode);
		int						GetMotionMode(DWORD dwMotionIndex);

		// Motion
		//	Pushing Motion
		void					ResetLocalTime();
		void					SetLoopMotion(WORD wMotion, float fBlendTime=0.1f, float fSpeedRatio=1.0f);
		void					PushOnceMotion(WORD wMotion, float fBlendTime=0.1f, float fSpeedRatio=1.0f);
		void					PushLoopMotion(WORD wMotion, float fBlendTime=0.1f, float fSpeedRatio=1.0f);
		void					SetEndStopMotion();

		// Intersect
		bool					IntersectDefendingSphere();
		bool					IntersectBoundingBox();

		// Part
		//void					SetParts(const WORD * c_pParts);
		void					Refresh(DWORD dwMotIndex, bool isLoop);

		//void					AttachEffectByID(DWORD dwParentPartIndex, const char * c_pszBoneName, DWORD dwEffectID, int dwLife = CActorInstance::EFFECT_LIFE_INFINITE ); // 수명은 ms단위입니다.
		//void					AttachEffectByName(DWORD dwParentPartIndex, const char * c_pszBoneName, const char * c_pszEffectName, int dwLife = CActorInstance::EFFECT_LIFE_INFINITE ); // 수명은 ms단위입니다.

		float					GetDistance(CInstanceBase * pkTargetInst);
		float					GetDistance(const TPixelPosition & c_rPixelPosition);

		// ETC
		CActorInstance&			GetGraphicThingInstanceRef();
		CActorInstance*			GetGraphicThingInstancePtr();

		bool __Background_IsWaterPixelPosition(const TPixelPosition& c_rkPPos);
		bool __Background_GetWaterHeight(const TPixelPosition& c_rkPPos, float* pfHeight);

		// 2004.07.25.myevan.이펙트 안나오는 문제
		/////////////////////////////////////////////////////////////
		void __ClearAffectFlagContainer();
		void __ClearAffects();
		/////////////////////////////////////////////////////////////

		void __SetAffect(UINT eAffect, bool isVisible);
		int	HasAffect(DWORD dwIndex);
		void SetAffectFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer);

		void __SetNormalAffectFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer);
		void __SetStoneSmokeFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer);

		void SetEmoticon(UINT eEmoticon);
		void SetFishEmoticon();
		bool IsPossibleEmoticon();

	public:
		bool __CanSkipCollision();
		
	protected:
		UINT					__LessRenderOrder_GetLODLevel();
		void					__Initialize();
		void					__InitializeRotationSpeed();

		void					__Create_SetName(const SCreateData& c_rkCreateData);
		void					__Create_SetWarpName(const SCreateData& c_rkCreateData);

		CInstanceBase*			__GetMainInstancePtr();
		CInstanceBase*			__FindInstancePtr(DWORD dwVID);

		bool  __FindRaceType(DWORD dwRace, BYTE* pbType);
		DWORD __GetRaceType();

		bool __IsShapeAnimalWear();
		BOOL __IsChangableWeapon(int iWeaponID);

		void __EnableSkipCollision();
		void __DisableSkipCollision();

		void __ClearMainInstance();

		void __Shaman_SetParalysis(bool isParalysis);
		void __Warrior_SetGeomgyeongAffect(bool isVisible);
		void __Assassin_SetEunhyeongAffect(bool isVisible);
		void __SetReviveInvisibilityAffect(bool isVisible);

		BOOL __CanProcessNetworkStatePacket();

		bool __IsInDustRange();

		// Emotion
		void __ProcessFunctionEmotion(DWORD dwMotionNumber, DWORD dwTargetVID, const TPixelPosition & c_rkPosDst);
		void __EnableChangingTCPState();
		void __DisableChangingTCPState();
		BOOL __IsEnableTCPProcess(UINT eCurFunc);

		// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
		bool __CanRender();
		bool __IsInViewFrustum();

		// HORSE
		void __AttachHorseSaddle();
		void __DetachHorseSaddle();

		struct SHORSE
		{
			bool m_isMounting;
			CActorInstance* m_pkActor;

			SHORSE();
			~SHORSE();

			void Destroy();
			void Create(const TPixelPosition& c_rkPPos, UINT eRace, UINT eHitEffect);

			void SetAttackSpeed(UINT uAtkSpd);
			void SetMoveSpeed(UINT uMovSpd);
			void Deform();
			void Render();
			CActorInstance& GetActorRef();
			CActorInstance* GetActorPtr();

			bool IsMounting();
			bool CanAttack();
			bool CanUseSkill();

			UINT GetLevel();
			bool IsNewMount();

			void __Initialize();
		} m_kHorse;


	protected:
		// Blend Mode
		void					__SetBlendRenderingMode();
		void					__SetAlphaValue(float fAlpha);
		float					__GetAlphaValue();

		void					__ComboProcess();
		void					MovementProcess();
		void					TodoProcess();
		void					StateProcess();
		void					AttackProcess();

		void					StartWalking();
		float					GetLocalTime();

		
		void					RefreshActorInstance();
	public:
		void					RefreshState(DWORD dwMotIndex, bool isLoop);

	protected:
		void					OnSyncing();
		void					OnWaiting();
		void					OnMoving();

		void					NEW_SetCurPixelPosition(const TPixelPosition& c_rkPPosDst);
		void					NEW_SetSrcPixelPosition(const TPixelPosition& c_rkPPosDst);
		void					NEW_SetDstPixelPosition(const TPixelPosition& c_rkPPosDst);
		void					NEW_SetDstPixelPositionZ(FLOAT z);

		const TPixelPosition&	NEW_GetCurPixelPositionRef();
		const TPixelPosition&	NEW_GetSrcPixelPositionRef();

	public:
		const TPixelPosition&	NEW_GetDstPixelPositionRef();

	protected:
		BOOL m_isTextTail;

		// Instance Data
		std::string				m_stName;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		BYTE			m_stLanguage;
#endif
		DWORD					m_awPart[CRaceData::PART_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
		DWORD					m_dwConquerorLevel;
#endif
		DWORD					m_dwLevel;
		DWORD					m_dwEmpireID;
		DWORD					m_dwGuildID;

	protected:
		CAffectFlagContainer	m_kAffectFlagContainer;
		DWORD					m_adwCRCAffectEffect[AFFECT_NUM];

#ifdef ENABLE_SHINING_SYSTEM
		INT		__GetShiningEffect(WORD slot);
		bool	ClearShiningEffect();
		bool	AttachShiningEffect(DWORD dwWeapon);
#endif
#ifdef ENABLE_SHINING_TABLE_SYSTEM
		void	_GetShiningTableEffect(CItemData* pItem);
		void	__ClearWeaponShiningEffect(bool detaching = true);
		void	__ClearArmorShiningEffect(bool detaching = true);
		void	__ClearAcceShiningEffect(bool detaching = true);
		void	__AttachWeaponShiningEffect(const char* effectFileName, const char* boneName = "Bip01");
		void	__AttachArmorShiningEffect(const char* effectFileName, const char* boneName = "Bip01");
		void	__AttachAcceShiningEffect(const char* effectFileName, const char* boneName = "Bip01 Spine2");
#endif
		UINT	__GetRefinedEffect(CItemData* pItem);
		void	__ClearWeaponRefineEffect();
		void	__ClearArmorRefineEffect();
		#ifdef ENABLE_SASH_SYSTEM
		void	ClearSashEffect();
		#endif
	protected:
		void __AttachSelectEffect();
		void __DetachSelectEffect();

		void __AttachTargetEffect();
		void __DetachTargetEffect();

		void __AttachEmpireEffect(DWORD eEmpire);

	protected:
		struct SEffectContainer
		{
			typedef std::map<DWORD, DWORD> Dict;
			Dict m_kDct_dwEftID;
		} m_kEffectContainer;

		void __EffectContainer_Initialize();
		void __EffectContainer_Destroy();

		DWORD __EffectContainer_AttachEffect(DWORD eEffect);
		void __EffectContainer_DetachEffect(DWORD eEffect);
#ifdef ENABLE_NO_COLLISION
		bool __EffectContainer_IsEffect(DWORD eEffect);
#endif

		SEffectContainer::Dict& __EffectContainer_GetDict();

	protected:
		struct SStoneSmoke
		{
			DWORD m_dwEftID;
		} m_kStoneSmoke;

		void __StoneSmoke_Inialize();
		void __StoneSmoke_Destroy();
		void __StoneSmoke_Create(DWORD eSmoke);

	protected:
		// Emoticon
		//DWORD					m_adwCRCEmoticonEffect[EMOTICON_NUM];

		BYTE					m_eType;
		BYTE					m_eRaceType;
		DWORD					m_eShape;
		DWORD					m_dwRace;
		DWORD					m_dwVirtualNumber;
		short					m_sAlignment;
		BYTE					m_byPKMode;

#if defined(WJ_COMBAT_ZONE)
		BYTE					combat_zone_rank;
		DWORD					combat_zone_points;
#endif
		bool					m_isKiller;
		bool					m_isPartyMember;

		// Movement
		int						m_iRotatingDirection;

		DWORD					m_dwAdvActorVID;
		DWORD					m_dwLastDmgActorVID;

		long					m_nAverageNetworkGap;
		DWORD					m_dwNextUpdateHeightTime;

		bool					m_isGoing;

		TPixelPosition			m_kPPosDust;
		DWORD					m_dwLastComboIndex;

		DWORD					m_swordRefineEffectRight;
		DWORD					m_swordRefineEffectLeft;
		DWORD					m_armorRefineEffect[2];
		#ifdef ENABLE_SASH_SYSTEM
		DWORD	m_dwSashEffect;
#endif

#ifdef ENABLE_SHINING_SYSTEM
		DWORD					m_dwShining[3];
		DWORD					m_armorShining;
		DWORD					m_swordShiningRight;
		DWORD					m_swordShiningLeft;
		DWORD					m_specialShining;
#endif
#ifdef ENABLE_SHINING_TABLE_SYSTEM
		DWORD					m_weaponShiningEffects[2];
		DWORD					m_armorShiningEffects;
		DWORD					m_AcceShiningEffects;
#endif
		struct SMoveAfterFunc
		{
			UINT eFunc;
			UINT uArg;

			// For Emotion Function
			UINT uArgExpanded;
			TPixelPosition kPosDst;
		};

		SMoveAfterFunc m_kMovAfterFunc;

		float m_fDstRot;
		float m_fAtkPosTime;
		float m_fRotSpd;
		float m_fMaxRotSpd;

		BOOL m_bEnableTCPState;

		// Graphic Instance
		CActorInstance m_GraphicThingInstance;


	protected:
		struct SCommand
		{
			DWORD	m_dwChkTime;
			DWORD	m_dwCmdTime;
			float	m_fDstRot;
			UINT 	m_eFunc;
			UINT 	m_uArg;
			UINT	m_uTargetVID;
			TPixelPosition m_kPPosDst;
		};

		typedef std::list<SCommand> CommandQueue;

		DWORD		m_dwBaseChkTime;
		DWORD		m_dwBaseCmdTime;

		DWORD		m_dwSkipTime;

		CommandQueue m_kQue_kCmdNew;

		BOOL		m_bDamageEffectType;

		struct SEffectDamage
		{
			DWORD	damage;
			BYTE	flag;
			BOOL	bSelf;
			BOOL	bTarget;
			DWORD	dwVictimVID;
			DWORD	dwAttackerVID;
		};

		typedef std::list<SEffectDamage> CommandDamageQueue;
		CommandDamageQueue m_DamageQueue;

		void ProcessDamage();
		void ProcessRemoveOldDamage();

	public:
		void AddDamageEffect(DWORD damage, BYTE flag, BOOL bSelf, BOOL bTarget, DWORD dwVictimVID, DWORD dwAttackerVID);

	protected:
		struct SWarrior
		{
			DWORD m_dwGeomgyeongEffect;
		};

		SWarrior m_kWarrior;

		void __Warrior_Initialize();

	public:
		static void ClearPVPKeySystem();

		static void InsertPVPKey(DWORD dwSrcVID, DWORD dwDstVID);
		static void InsertPVPReadyKey(DWORD dwSrcVID, DWORD dwDstVID);
		static void RemovePVPKey(DWORD dwSrcVID, DWORD dwDstVID);

		static void InsertGVGKey(DWORD dwSrcGuildVID, DWORD dwDstGuildVID);
		static void RemoveGVGKey(DWORD dwSrcGuildVID, DWORD dwDstGuildVID);

		static void InsertDUELKey(DWORD dwSrcVID, DWORD dwDstVID);

		UINT GetNameColorIndex();

		const D3DXCOLOR& GetNameColor();
		const D3DXCOLOR& GetTitleColor();

	protected:
		static DWORD __GetPVPKey(DWORD dwSrcVID, DWORD dwDstVID);
		static bool __FindPVPKey(DWORD dwSrcVID, DWORD dwDstVID);
		static bool __FindPVPReadyKey(DWORD dwSrcVID, DWORD dwDstVID);
		static bool __FindGVGKey(DWORD dwSrcGuildID, DWORD dwDstGuildID);
		static bool __FindDUELKey(DWORD dwSrcGuildID, DWORD dwDstGuildID);

	protected:
		CActorInstance::IEventHandler* GetEventHandlerPtr();
		CActorInstance::IEventHandler& GetEventHandlerRef();

	protected:
		static float __GetBackgroundHeight(float x, float y);
		static DWORD __GetShadowMapColor(float x, float y);

	public:
		static void ResetPerformanceCounter();
		static void GetInfo(std::string* pstInfo);

	public:
		static CInstanceBase* New();
		static void Delete(CInstanceBase* pkInst);

		static CDynamicPool<CInstanceBase>	ms_kPool;

	protected:
		static DWORD ms_dwUpdateCounter;
		static DWORD ms_dwRenderCounter;
		static DWORD ms_dwDeformCounter;

	public:
		DWORD					GetDuelMode();
		void					SetDuelMode(DWORD type);
	protected:
		DWORD					m_dwDuelMode;
		DWORD					m_dwEmoticonTime;

#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
	public:
		void					MobInfoAiFlagRefresh();
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
	public:
		void					MobInfoLevelRefresh();
#endif
	public:
		void SetQueueEntity(DWORD state) { m_GraphicThingInstance.SetQueueElement(state); }

	protected:
		bool m_IsAlwaysRender;
		std::vector<DWORD>		m_vecWikiEffects;
	public:
		bool IsAlwaysRender(){return m_IsAlwaysRender;}
		void SetAlwaysRender(bool val);
		bool					SetMotionIndex(DWORD motionIndex);
		void					AttachWikiAffect(DWORD effectIndex);
		void					RemoveWikiAffect(DWORD effectIndex);
		void					ClearWikiAffect();
		DWORD					GetPartData(BYTE partIndex);
#ifdef ENABLE_SHINING_SYSTEM
		void					SetShiningRender(DWORD slot, DWORD itemIndex);
		DWORD					GetShiningData(BYTE slot);
#endif

#ifdef ENABLE_GRAPHIC_ON_OFF
		bool	GetRenderStatus();
		void	SetRenderStatus(bool bStatus);
		bool	GetEffectStatus();
		void	SetEffectStatus(bool bStatus);
#endif
#ifdef ENABLE_AURA_SYSTEM
	public:
		bool	SetAura(DWORD eAura);
		void	ChangeAura(DWORD eAura);
	protected:
		DWORD	m_auraRefineEffect;
#endif

};

inline int RaceToJob(int race)
{
#ifdef ENABLE_WOLFMAN_CHARACTER
	if (race==8)
		return 4;
#endif
	const int JOB_NUM = 4;
	return race % JOB_NUM;
}

inline int RaceToSex(int race)
{
	switch (race)
	{
		case 0:
		case 2:
		case 5:
		case 7:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case 8:
#endif
			return 1;
		case 1:
		case 3:
		case 4:
		case 6:
			return 0;

	}
	return 0;
}