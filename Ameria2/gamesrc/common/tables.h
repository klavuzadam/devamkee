#ifndef __INC_TABLES_H__
#define __INC_TABLES_H__

#include "length.h"
#include "item_length.h"
#include "CommonDefines.h"
#include <string>
#include <vector>
typedef	DWORD IDENT;

/**
 * @version 05/06/10	Bang2ni - Myshop Pricelist °ü·Ã ÆÐÅ¶ HEADER_XX_MYSHOP_PRICELIST_XXX Ãß°¡
 */
enum
{
	HEADER_GD_LOGIN				= 1,
	HEADER_GD_LOGOUT			= 2,

	HEADER_GD_PLAYER_LOAD		= 3,
	HEADER_GD_PLAYER_SAVE		= 4,
	HEADER_GD_PLAYER_CREATE		= 5,
	HEADER_GD_PLAYER_DELETE		= 6,

	HEADER_GD_LOGIN_KEY			= 7,
	// 8 empty
	HEADER_GD_BOOT				= 9,
	HEADER_GD_PLAYER_COUNT		= 10,
	HEADER_GD_QUEST_SAVE		= 11,
	HEADER_GD_SAFEBOX_LOAD		= 12,
	HEADER_GD_SAFEBOX_SAVE		= 13,
	HEADER_GD_SAFEBOX_CHANGE_SIZE	= 14,
	HEADER_GD_EMPIRE_SELECT		= 15,

	HEADER_GD_SAFEBOX_CHANGE_PASSWORD		= 16,
	HEADER_GD_SAFEBOX_CHANGE_PASSWORD_SECOND	= 17, // Not really a packet, used internal
	HEADER_GD_DIRECT_ENTER		= 18,

	HEADER_GD_GUILD_SKILL_UPDATE	= 19,
	HEADER_GD_GUILD_EXP_UPDATE		= 20,
	HEADER_GD_GUILD_ADD_MEMBER		= 21,
	HEADER_GD_GUILD_REMOVE_MEMBER	= 22,
	HEADER_GD_GUILD_CHANGE_GRADE	= 23,
	HEADER_GD_GUILD_CHANGE_MEMBER_DATA	= 24,
	HEADER_GD_GUILD_DISBAND		= 25,
	HEADER_GD_GUILD_WAR			= 26,
	HEADER_GD_GUILD_WAR_SCORE		= 27,
	HEADER_GD_GUILD_CREATE		= 28,

	HEADER_GD_ITEM_SAVE			= 30,
	HEADER_GD_ITEM_DESTROY		= 31,

	HEADER_GD_ADD_AFFECT		= 32,
	HEADER_GD_REMOVE_AFFECT		= 33,

	HEADER_GD_HIGHSCORE_REGISTER	= 34,
	HEADER_GD_ITEM_FLUSH		= 35,

	HEADER_GD_PARTY_CREATE		= 36,
	HEADER_GD_PARTY_DELETE		= 37,
	HEADER_GD_PARTY_ADD			= 38,
	HEADER_GD_PARTY_REMOVE		= 39,
	HEADER_GD_PARTY_STATE_CHANGE	= 40,
	HEADER_GD_PARTY_HEAL_USE		= 41,

	HEADER_GD_FLUSH_CACHE		= 42,
	HEADER_GD_RELOAD_PROTO		= 43,

	HEADER_GD_CHANGE_NAME		= 44,
	HEADER_GD_SMS				= 45,

	HEADER_GD_GUILD_CHANGE_LADDER_POINT	= 46,
	HEADER_GD_GUILD_USE_SKILL		= 47,

	HEADER_GD_REQUEST_EMPIRE_PRIV	= 48,
	HEADER_GD_REQUEST_GUILD_PRIV	= 49,

	HEADER_GD_MONEY_LOG				= 50,

	HEADER_GD_GUILD_DEPOSIT_MONEY				= 51,
	HEADER_GD_GUILD_WITHDRAW_MONEY				= 52,
	HEADER_GD_GUILD_WITHDRAW_MONEY_GIVE_REPLY	= 53,

	HEADER_GD_REQUEST_CHARACTER_PRIV	= 54,

	HEADER_GD_SET_EVENT_FLAG			= 55,

	HEADER_GD_PARTY_SET_MEMBER_LEVEL	= 56,

	HEADER_GD_GUILD_WAR_BET		= 57,

	HEADER_GD_CREATE_OBJECT		= 60,
	HEADER_GD_DELETE_OBJECT		= 61,
	HEADER_GD_UPDATE_LAND		= 62,

	HEADER_GD_MARRIAGE_ADD		= 70,
	HEADER_GD_MARRIAGE_UPDATE	= 71,
	HEADER_GD_MARRIAGE_REMOVE	= 72,

	HEADER_GD_WEDDING_REQUEST	= 73,
	HEADER_GD_WEDDING_READY		= 74,
	HEADER_GD_WEDDING_END		= 75,


	HEADER_GD_AUTH_LOGIN		= 100,
	HEADER_GD_LOGIN_BY_KEY		= 101,
	HEADER_GD_BILLING_EXPIRE	= 104,
	HEADER_GD_VCARD				= 105,
	HEADER_GD_BILLING_CHECK		= 106,
	HEADER_GD_MALL_LOAD			= 107,

	HEADER_GD_BLOCK_CHAT				= 110,

	// PCBANG_IP_LIST_BY_AUTH
	HEADER_GD_PCBANG_REQUEST_IP_LIST	= 111,
	HEADER_GD_PCBANG_CLEAR_IP_LIST		= 112,
	HEADER_GD_PCBANG_INSERT_IP			= 113,
	// END_OF_PCBANG_IP_LIST_BY_AUTH

	HEADER_GD_HAMMER_OF_TOR			= 114,
	HEADER_GD_RELOAD_ADMIN			= 115,			///<¿î¿µÀÚ Á¤º¸ ¿äÃ»
	HEADER_GD_BREAK_MARRIAGE		= 116,			///< °áÈ¥ ÆÄ±â
	HEADER_GD_ELECT_MONARCH			= 117,			///< ±ºÁÖ ÅõÇ¥
	HEADER_GD_CANDIDACY				= 118,			///< ±ºÁÖ µî·Ï
	HEADER_GD_ADD_MONARCH_MONEY		= 119,			///< ±ºÁÖ µ· Áõ°¡
	HEADER_GD_TAKE_MONARCH_MONEY	= 120,			///< ±ºÁÖ µ· °¨¼Ò
	HEADER_GD_COME_TO_VOTE			= 121,			///< Ç¥°á
	HEADER_GD_RMCANDIDACY			= 122,			///< ÈÄº¸ Á¦°Å (¿î¿µÀÚ)
	HEADER_GD_SETMONARCH			= 123,			///<±ºÁÖ¼³Á¤ (¿î¿µÀÚ)
	HEADER_GD_RMMONARCH			= 124,			///<±ºÁÖ»èÁ¦
	HEADER_GD_DEC_MONARCH_MONEY = 125,

	HEADER_GD_CHANGE_MONARCH_LORD = 126,
	HEADER_GD_BLOCK_COUNTRY_IP		= 127,		// ±¤´ë¿ª IP-Block
	HEADER_GD_BLOCK_EXCEPTION		= 128,		// ±¤´ë¿ª IP-Block ¿¹¿Ü

	HEADER_GD_REQ_CHANGE_GUILD_MASTER	= 129,

	HEADER_GD_REQ_SPARE_ITEM_ID_RANGE	= 130,

	HEADER_GD_UPDATE_HORSE_NAME		= 131,
	HEADER_GD_REQ_HORSE_NAME		= 132,

	HEADER_GD_DC					= 133,		// Login Key¸¦ Áö¿ò

	HEADER_GD_VALID_LOGOUT			= 134,

	HEADER_GD_REQUEST_CHARGE_CASH	= 137,

	HEADER_GD_DELETE_AWARDID	= 138,	// delete gift notify icon

	HEADER_GD_UPDATE_CHANNELSTATUS	= 139,
	HEADER_GD_REQUEST_CHANNELSTATUS	= 140,
#if defined(WJ_COMBAT_ZONE)
	HEADER_GD_COMBAT_ZONE_RESET_RANKING	= 141,
	HEADER_GD_COMBAT_ZONE_SKILLS_CACHE	= 142,
#endif
	HEADER_GD_PLAYER_HAIR_CHANGE		= 147,
#ifdef ENABLE_IKASHOP_RENEWAL
	HEADER_GD_NEW_OFFLINESHOP		= 153,
#endif
#ifdef __FAKE_PLAYER__
	HEADER_GD_FAKE_PLAYER,
#endif
	// RANKING_SYSTEM
	HEADER_GD_ADD_RANKING,
	// END_OF_RANKING_SYSTEM

	HEADER_GD_SETUP			= 0xff,

	///////////////////////////////////////////////
	HEADER_DG_NOTICE			= 1,

	HEADER_DG_LOGIN_SUCCESS			= 30,
	HEADER_DG_LOGIN_NOT_EXIST		= 31,
	HEADER_DG_LOGIN_WRONG_PASSWD	= 33,
	HEADER_DG_LOGIN_ALREADY			= 34,

	HEADER_DG_PLAYER_LOAD_SUCCESS	= 35,
	HEADER_DG_PLAYER_LOAD_FAILED	= 36,
	HEADER_DG_PLAYER_CREATE_SUCCESS	= 37,
	HEADER_DG_PLAYER_CREATE_ALREADY	= 38,
	HEADER_DG_PLAYER_CREATE_FAILED	= 39,
	HEADER_DG_PLAYER_DELETE_SUCCESS	= 40,
	HEADER_DG_PLAYER_DELETE_FAILED	= 41,

	HEADER_DG_ITEM_LOAD			= 42,

	HEADER_DG_BOOT				= 43,
	HEADER_DG_QUEST_LOAD		= 44,

	HEADER_DG_SAFEBOX_LOAD					= 45,
	HEADER_DG_SAFEBOX_CHANGE_SIZE			= 46,
	HEADER_DG_SAFEBOX_WRONG_PASSWORD		= 47,
	HEADER_DG_SAFEBOX_CHANGE_PASSWORD_ANSWER = 48,

	HEADER_DG_EMPIRE_SELECT		= 49,

	HEADER_DG_AFFECT_LOAD		= 50,
	HEADER_DG_MALL_LOAD			= 51,
#ifdef USE_DAY_CHANGE_MGR
	HEADER_DG_DAY_CHANGE_PROCESS = 52,
#endif
	HEADER_DG_DIRECT_ENTER		= 55,

	HEADER_DG_GUILD_SKILL_UPDATE	= 56,
	HEADER_DG_GUILD_SKILL_RECHARGE	= 57,
	HEADER_DG_GUILD_EXP_UPDATE		= 58,

	HEADER_DG_PARTY_CREATE		= 59,
	HEADER_DG_PARTY_DELETE		= 60,
	HEADER_DG_PARTY_ADD			= 61,
	HEADER_DG_PARTY_REMOVE		= 62,
	HEADER_DG_PARTY_STATE_CHANGE	= 63,
	HEADER_DG_PARTY_HEAL_USE		= 64,
	HEADER_DG_PARTY_SET_MEMBER_LEVEL	= 65,
	HEADER_DG_TIME			= 90,
	HEADER_DG_ITEM_ID_RANGE		= 91,

	HEADER_DG_GUILD_ADD_MEMBER		= 92,
	HEADER_DG_GUILD_REMOVE_MEMBER	= 93,
	HEADER_DG_GUILD_CHANGE_GRADE	= 94,
	HEADER_DG_GUILD_CHANGE_MEMBER_DATA	= 95,
	HEADER_DG_GUILD_DISBAND		= 96,
	HEADER_DG_GUILD_WAR			= 97,
	HEADER_DG_GUILD_WAR_SCORE		= 98,
	HEADER_DG_GUILD_TIME_UPDATE		= 99,
	HEADER_DG_GUILD_LOAD		= 100,
	HEADER_DG_GUILD_LADDER		= 101,
	HEADER_DG_GUILD_SKILL_USABLE_CHANGE	= 102,
	HEADER_DG_GUILD_MONEY_CHANGE	= 103,
	HEADER_DG_GUILD_WITHDRAW_MONEY_GIVE	= 104,

	HEADER_DG_SET_EVENT_FLAG		= 105,

	HEADER_DG_GUILD_WAR_RESERVE_ADD	= 106,
	HEADER_DG_GUILD_WAR_RESERVE_DEL	= 107,
	HEADER_DG_GUILD_WAR_BET		= 108,

	HEADER_DG_RELOAD_PROTO		= 120,
	HEADER_DG_CHANGE_NAME		= 121,

#ifdef ENABLE_ITEMSHOP
	HEADER_DG_ITEMSHOP = 76,
	HEADER_GD_ITEMSHOP = 76,
#endif

	HEADER_DG_AUTH_LOGIN		= 122,

	HEADER_DG_CHANGE_EMPIRE_PRIV	= 124,
	HEADER_DG_CHANGE_GUILD_PRIV		= 125,

	HEADER_DG_MONEY_LOG			= 126,

	HEADER_DG_CHANGE_CHARACTER_PRIV	= 127,

	HEADER_DG_BILLING_REPAIR		= 128,
	HEADER_DG_BILLING_EXPIRE		= 129,
	HEADER_DG_BILLING_LOGIN		= 130,
	HEADER_DG_VCARD			= 131,
	HEADER_DG_BILLING_CHECK		= 132,

	HEADER_DG_CREATE_OBJECT		= 140,
	HEADER_DG_DELETE_OBJECT		= 141,
	HEADER_DG_UPDATE_LAND		= 142,

	HEADER_DG_MARRIAGE_ADD		= 150,
	HEADER_DG_MARRIAGE_UPDATE		= 151,
	HEADER_DG_MARRIAGE_REMOVE		= 152,

	HEADER_DG_WEDDING_REQUEST		= 153,
	HEADER_DG_WEDDING_READY		= 154,
	HEADER_DG_WEDDING_START		= 155,
	HEADER_DG_WEDDING_END		= 156,

	HEADER_DG_MYSHOP_PRICELIST_RES	= 157,		///< °¡°ÝÁ¤º¸ ¸®½ºÆ® ÀÀ´ä
	HEADER_DG_RELOAD_ADMIN = 158, 				///< ¿î¿µÀÚ Á¤º¸ ¸®·Îµå
	HEADER_DG_BREAK_MARRIAGE = 159,				///< °áÈ¥ ÆÄ±â
	HEADER_DG_ELECT_MONARCH			= 160,			///< ±ºÁÖ ÅõÇ¥
	HEADER_DG_CANDIDACY				= 161,			///< ±ºÁÖ µî·Ï
	HEADER_DG_ADD_MONARCH_MONEY		= 162,			///< ±ºÁÖ µ· Áõ°¡
	HEADER_DG_TAKE_MONARCH_MONEY	= 163,			///< ±ºÁÖ µ· °¨¼Ò
	HEADER_DG_COME_TO_VOTE			= 164,			///< Ç¥°á
	HEADER_DG_RMCANDIDACY			= 165,			///< ÈÄº¸ Á¦°Å (¿î¿µÀÚ)
	HEADER_DG_SETMONARCH			= 166,			///<±ºÁÖ¼³Á¤ (¿î¿µÀÚ)
	HEADER_DG_RMMONARCH			= 167,			///<±ºÁÖ»èÁ¦
	HEADER_DG_DEC_MONARCH_MONEY = 168,

	HEADER_DG_CHANGE_MONARCH_LORD_ACK = 169,
	HEADER_DG_UPDATE_MONARCH_INFO	= 170,
	HEADER_DG_BLOCK_COUNTRY_IP		= 171,		// ±¤´ë¿ª IP-Block
	HEADER_DG_BLOCK_EXCEPTION		= 172,		// ±¤´ë¿ª IP-Block ¿¹¿Ü account

	HEADER_DG_ACK_CHANGE_GUILD_MASTER = 173,

	HEADER_DG_ACK_SPARE_ITEM_ID_RANGE = 174,

	HEADER_DG_UPDATE_HORSE_NAME 	= 175,
	HEADER_DG_ACK_HORSE_NAME		= 176,

	HEADER_DG_NEED_LOGIN_LOG		= 177,

	HEADER_DG_RESULT_CHARGE_CASH	= 179,
	HEADER_DG_ITEMAWARD_INFORMER	= 180,	//gift notify
	HEADER_DG_RESPOND_CHANNELSTATUS		= 181,
#ifdef ENABLE_IKASHOP_RENEWAL
	HEADER_DG_NEW_OFFLINESHOP		= 182,
#endif
#ifdef ENABLE_EVENT_MANAGER
	HEADER_DG_EVENT_MANAGER						= 212,
	HEADER_GD_EVENT_MANAGER						= 212,
#endif
#ifdef __FAKE_PLAYER__
	HEADER_DG_FAKE_PLAYER,
#endif
	HEADER_DG_MAP_LOCATIONS		= 0xfe,
	HEADER_DG_P2P			= 0xff,

#ifdef ENABLE_AUTOMATIC_EVENT
	HEADER_DG_AUTOMATIC_EVENT = 200,
	HEADER_DG_MESSAGE_AUTOMATIC_EVENT = 201,
#endif

	HEADER_GP_CONFIRM_PASSPOD = 1,
	HEADER_PG_CONFIRM_PASSPOD = 2,

};

enum E_PASSPOD
{
	E_PASSPOD_SUCCESS = 0,
	E_PASSPOD_FAILED_PASSPOD_ERROR,
	E_PASSPOD_FAILED_USER_NOT_FOUND,
	E_PASSPOD_FAILED_SYSTEM_NOT_FOUND,
	E_PASSPOD_FAILED_TOKEN_DISABLED,
	E_PASSPOD_FAILED_EMPTY,
};

typedef struct SRequestConfirmPasspod
{
	int pid;
	char passpod[MAX_PASSPOD + 1];
	char login[LOGIN_MAX_LEN + 1];

} RequestConfirmPasspod;

typedef struct SResultConfirmPasspod
{
	int pid;
	int ret_code;
	char login[LOGIN_MAX_LEN + 1];
} ResultConfirmPasspod;
/* ----------------------------------------------
 * table
 * ----------------------------------------------
 */

/* game Server -> DB Server */
#pragma pack(1)
enum ERequestChargeType
{
	ERequestCharge_Cash = 0,
	ERequestCharge_Mileage,
};

typedef struct SRequestChargeCash
{
	DWORD		dwAID;		// id(primary key) - Account Table
	DWORD		dwAmount;
	ERequestChargeType	eChargeType;

} TRequestChargeCash;

typedef struct SSimplePlayer
{
	DWORD		dwID;
	char		szName[CHARACTER_NAME_MAX_LEN + 1];
	BYTE		byJob;
	BYTE		byLevel;
	DWORD		dwPlayMinutes;
	BYTE		byST, byHT, byDX, byIQ;
	uint32_t		wMainPart;
	BYTE		bChangeName;
	uint32_t		wHairPart;
	#ifdef __SASH_SYSTEM__
	uint32_t	wSashPart;
	#endif
	BYTE		bDummy[4];
	long		x, y;
	long		lAddr;
	WORD		wPort;
	BYTE		skill_group;

#ifdef ENABLE_YOHARA_SYSTEM
	BYTE		byConquerorLevel;
	BYTE		bySungmaST, bySungmaHP, bySungmaMove, bySungmaInmune;
#endif

} TSimplePlayer;

typedef struct SAccountTable
{
	DWORD		id;
	char		login[LOGIN_MAX_LEN + 1];
	char		passwd[PASSWD_MAX_LEN + 1];
	char		social_id[SOCIAL_ID_MAX_LEN + 1];
	char		status[ACCOUNT_STATUS_MAX_LEN + 1];
	BYTE		bEmpire;
	TSimplePlayer	players[PLAYER_PER_ACCOUNT];
} TAccountTable;

typedef struct SPacketDGCreateSuccess
{
	BYTE		bAccountCharacterIndex;
	TSimplePlayer	player;
} TPacketDGCreateSuccess;
#if defined(WJ_COMBAT_ZONE)
typedef struct SPacketGDCombatZoneResetRanking
{
	BYTE	bType;
} TPacketGDCombatZoneResetRanking;

typedef struct SPacketGDCombatZoneSkillsCache
{
	DWORD		dwPID;
	DWORD		dwSkillLevel1;
	DWORD		dwSkillLevel2;
	DWORD		dwSkillLevel3;
	DWORD		dwSkillLevel4;
	DWORD		dwSkillLevel5;
	DWORD		dwSkillLevel6;
} TPacketGDCombatZoneSkillsCache;
#endif
typedef struct TPlayerItemAttribute
{
	BYTE	bType;
	short	sValue;
} TPlayerItemAttribute;

#ifdef ENABLE_YOHARA_SYSTEM
typedef struct SSungmaTable
{
	long lMapIdx;
	uint8_t bSungmaStr;
	uint8_t bSungmaHp;
	uint8_t bSungmaMove;
	uint8_t bSungmaImmune;
#ifdef ENABLE_PRECISION
	uint8_t bHitPct;
#endif
} TSungmaTable;

typedef struct TPlayerItemApplyRandom
{
	BYTE bType;
	short sValue;
	BYTE row;
} TPlayerItemApplyRandom;
#endif

typedef struct SPlayerItem
{
	DWORD	id;
	BYTE	window;
	WORD	pos;
	DWORD	count;

	DWORD	vnum;
	long	alSockets[ITEM_SOCKET_MAX_NUM];	// ¼ÒÄÏ¹øÈ£

	TPlayerItemAttribute    aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif


	DWORD	owner;

#ifdef __SOULBINDING_SYSTEM__
	long	bind;
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char	name[ITEM_NAME_MAX_LEN+1];
#endif
} TPlayerItem;

typedef struct SQuickslot
{
	BYTE	type;
	WORD	pos;
} TQuickslot;

typedef struct SPlayerSkill
{
	BYTE	bMasterType;
	BYTE	bLevel;
	time_t	tNextRead;
} TPlayerSkill;

struct	THorseInfo
{
	BYTE	bLevel;
	BYTE	bRiding;
	short	sStamina;
	short	sHealth;
	DWORD	dwHorseHealthDropTime;
};

#ifdef ENABLE_BATTLE_PASS
enum
{
	MISSION_NONE,
	MISSION_BOSS,
	MISSION_CATCH_FISH,
	MISSION_CRAFT_ITEM,
	MISSION_CRAFT_GAYA,
	MISSION_DESTROY_ITEM,
	MISSION_DUNGEON,
	MISSION_EARN_MONEY,
	MISSION_FEED_PET,
	MISSION_LEVEL_UP,
	MISSION_MONSTER,
	MISSION_MOUNT_TIME,
	MISSION_OPEN_OFFLINESHOP,
	MISSION_PLAYTIME,
	MISSION_REFINE_ITEM,
	MISSION_REFINE_ALCHEMY,
	MISSION_SASH,
	MISSION_SELL_ITEM,
	MISSION_SPEND_MONEY,
	MISSION_SPRITE_STONE,
	MISSION_STONE,
	MISSION_USE_EMOTICON,
	MISSION_WHISPER,
	MISSION_SHOUT_CHAT,
	MISSION_KILLPLAYER,
	MISSION_MAX,

	BATTLE_MISSION_MAX = 10,
	BATTLE_SUB_REWARD = 3,
	BATTLE_REWARD_MAX = 6,
};
#include <vector>
#include <map>
typedef struct _battle_pass
{
	std::map<BYTE, std::pair<long long, DWORD>> missionData;
	std::map<BYTE, BYTE> missionIndex;
	std::map<BYTE, std::vector<std::pair<DWORD, DWORD>>> subReward;
	std::vector<std::pair<DWORD, DWORD>> rewardData;
}BattlePassData;
#endif

typedef struct SPlayerTable
{
	DWORD	id;

	char	name[CHARACTER_NAME_MAX_LEN + 1];
	char	ip[IP_ADDRESS_LENGTH + 1];

	WORD	job;
	BYTE	voice;

	BYTE	level;
	BYTE	level_step;
	short	st, ht, dx, iq;

	DWORD	exp;
	long long	gold;
#ifdef USE_CHEQUE_CURRENCY
	uint16_t cheque;
#endif
#ifdef UNLOCK_INVENTORY_WORLDARD
	DWORD	inventory_unlock[UNLOCK_INVENTORY_MAX];
#endif

#ifdef BATTLEPASS_WORLDARD
	DWORD	dwBattlePassEndTime;
#endif

	BYTE	dir;
	INT		x, y, z;
	INT		lMapIndex;

	long	lExitX, lExitY;
	long	lExitMapIndex;

	// @fixme301
	int		hp;
	int		sp;

	short	sRandomHP;
	short	sRandomSP;

	int         playtime;

	short	stat_point;
	short	skill_point;
	short	sub_skill_point;
	short	horse_skill_point;

	TPlayerSkill skills[SKILL_MAX_NUM];

	TQuickslot  quickslot[QUICKSLOT_MAX_NUM];

	BYTE	part_base;
	uint32_t	parts[PART_MAX_NUM];

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	DWORD	dwPickUPMode;
	DWORD	dwRarityMode;
#endif
	
	short	stamina;

	BYTE	skill_group;
	long	lAlignment;
	char	szMobile[MOBILE_MAX_LEN + 1];

	short	stat_reset_count;

	THorseInfo	horse;

	DWORD	logoff_interval;

	int		aiPremiumTimes[PREMIUM_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	BYTE	conquerorlevel;
	BYTE	conqueror_level_step;
	short	sungma_str, sungma_hp, sungma_move, sungma_inmune;
	DWORD	conqueror_exp;
	short	conqueror_point;	
#endif
#ifdef ENABLE_BATTLE_PASS
	long long	battlePassData[MISSION_MAX];
#endif
#ifdef ENABLE_NEW_DETAILS_GUI
	long	kill_log[KILL_MAX_NUM];
#endif
#ifdef ENABLE_ANTI_EXP
	bool	anti_exp;
#endif
#ifdef __GEM_SYSTEM__
	int			gem;
#endif
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
	int auto_hunt_last_day;
	int auto_hunt_used_time;
#endif
#ifdef ENABLE_RANKING
	long long	lRankPoints[RANKING_MAX_CATEGORIES];
#endif
} TPlayerTable;

typedef struct SMobSkillLevel
{
	DWORD	dwVnum;
	BYTE	bLevel;
} TMobSkillLevel;

typedef struct SEntityTable
{
	DWORD dwVnum;
} TEntityTable;

typedef struct SMobTable : public SEntityTable
{
	char			szName[CHARACTER_NAME_MAX_LEN + 1];
	char			szLocaleName[CHARACTER_NAME_MAX_LEN + 1];

	BYTE			bType;
	BYTE			bRank;
	BYTE			bBattleType;
	BYTE			bLevel;
	BYTE			bSize;

	DWORD			dwGoldMin;
	DWORD			dwGoldMax;
	DWORD			dwExp;
#ifdef ENABLE_YOHARA_PROTO_EXT
	DWORD			dwSungMaExp;
#endif
	DWORD			dwMaxHP;
	BYTE			bRegenCycle;
	BYTE			bRegenPercent;
	WORD			wDef;

	DWORD			dwAIFlag;
	DWORD			dwRaceFlag;
	DWORD			dwImmuneFlag;

	BYTE			bStr, bDex, bCon, bInt;
	DWORD			dwDamageRange[2];

	short			sAttackSpeed;
	short			sMovingSpeed;
	BYTE			bAggresiveHPPct;
	WORD			wAggressiveSight;
	WORD			wAttackRange;

	char			cEnchants[MOB_ENCHANTS_MAX_NUM];
	char			cResists[MOB_RESISTS_MAX_NUM];

	DWORD			dwResurrectionVnum;
	DWORD			dwDropItemVnum;

	BYTE			bMountCapacity;
	BYTE			bOnClickType;

	BYTE			bEmpire;
	char			szFolder[64 + 1];

	float			fDamMultiply;
	DWORD			dwSummonVnum;
	DWORD			dwDrainSP;
	DWORD			dwMobColor;
	DWORD			dwPolymorphItemVnum;

	TMobSkillLevel Skills[MOB_SKILL_MAX_NUM];

	BYTE			bBerserkPoint;
	BYTE			bStoneSkinPoint;
	BYTE			bGodSpeedPoint;
	BYTE			bDeathBlowPoint;
	BYTE			bRevivePoint;
} TMobTable;

typedef struct SSkillTable
{
	DWORD	dwVnum;
	char	szName[32 + 1];
	BYTE	bType;
	BYTE	bMaxLevel;
	DWORD	dwSplashRange;

	char	szPointOn[64];
	char	szPointPoly[100 + 1];
	char	szSPCostPoly[100 + 1];
	char	szDurationPoly[100 + 1];
	char	szDurationSPCostPoly[100 + 1];
	char	szCooldownPoly[100 + 1];
	char	szMasterBonusPoly[100 + 1];
	//char	szAttackGradePoly[100 + 1];
	char	szGrandMasterAddSPCostPoly[100 + 1];
	DWORD	dwFlag;
	DWORD	dwAffectFlag;

	// Data for secondary skill
	char 	szPointOn2[64];
	char 	szPointPoly2[100 + 1];
	char 	szDurationPoly2[100 + 1];
	DWORD 	dwAffectFlag2;

	// Data for grand master point
	char 	szPointOn3[64];
	char 	szPointPoly3[100 + 1];
	char 	szDurationPoly3[100 + 1];

	BYTE	bLevelStep;
	BYTE	bLevelLimit;
	DWORD	preSkillVnum;
	BYTE	preSkillLevel;

	long	lMaxHit;
	char	szSplashAroundDamageAdjustPoly[100 + 1];

	BYTE	bSkillAttrType;

	DWORD	dwTargetRange;
} TSkillTable;

#ifdef ENABLE_BUY_WITH_ITEM
typedef struct SShopItemPrice
{
	DWORD		vnum;
	DWORD		count;
} TShopItemPrice;
#endif

typedef struct SShopItemTable
{
	DWORD		vnum;
	DWORD		count;
#ifdef ENABLE_BUY_WITH_ITEM
	TShopItemPrice	itemprice[MAX_SHOP_PRICES];
#endif
    TItemPos	pos;			// PC ¡íoA¢§?¢§¢¬¢¬ ???e
	#ifdef FULL_YANG
	long long		price;	// PC, shop_table_ex.txt ¡íoA¢§?¢§¢¬¢¬ ???e
	#else
	DWORD		price;	// PC, shop_table_ex.txt ¡íoA¢§?¢§¢¬¢¬ ???e
	#endif
	BYTE		display_pos; // PC, shop_table_ex.txt ¡íoA¢§?¢§¢¬¢¬ ???e, ?¢¬?? ?¡×A¢§.
 	
} TShopItemTable;

typedef struct SShopTable
{
	DWORD		dwVnum;
	DWORD		dwNPCVnum;

	BYTE		byItemCount;
	TShopItemTable	items[SHOP_HOST_ITEM_MAX_NUM];
} TShopTable;

#define QUEST_NAME_MAX_LEN	32
#define QUEST_STATE_MAX_LEN	64

typedef struct SQuestTable
{
	DWORD		dwPID;
	char		szName[QUEST_NAME_MAX_LEN + 1];
	char		szState[QUEST_STATE_MAX_LEN + 1];
	long		lValue;
} TQuestTable;

typedef struct SItemLimit
{
	BYTE	bType;
	long	lValue;
} TItemLimit;

typedef struct SItemApply
{
	BYTE	bType;
	long	lValue;
} TItemApply;

typedef struct SItemTable : public SEntityTable
{
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

	long long	dwGold;
	long long	dwShopBuyPrice;

	TItemLimit	aLimits[ITEM_LIMIT_MAX_NUM];
	TItemApply	aApplies[ITEM_APPLY_MAX_NUM];
	long		alValues[ITEM_VALUES_MAX_NUM];
	long		alSockets[ITEM_SOCKET_MAX_NUM];

	DWORD		dwRefinedVnum;
	WORD		wRefineSet;
	BYTE		bAlterToMagicItemPct;
	BYTE		bSpecular;
	BYTE		bGainSocketPct;

	short int	sAddonType;
	char		cLimitRealTimeFirstUseIndex;
	char		cLimitTimerBasedOnWearIndex;
} TItemTable;

struct TItemAttrTable
{
	TItemAttrTable() :
		dwApplyIndex(0),
		dwProb(0)
	{
		szApply[0] = 0;
		memset(&lValues, 0, sizeof(lValues));
		memset(&bMaxLevelBySet, 0, sizeof(bMaxLevelBySet));
	}

	char    szApply[APPLY_NAME_MAX_LEN + 1];
	DWORD   dwApplyIndex;
	DWORD   dwProb;
	long    lValues[ITEM_ATTRIBUTE_MAX_LEVEL];
	BYTE    bMaxLevelBySet[ATTRIBUTE_SET_MAX_NUM];
};

typedef struct SConnectTable
{
	char	login[LOGIN_MAX_LEN + 1];
	IDENT	ident;
} TConnectTable;

typedef struct SLoginPacket
{
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
} TLoginPacket;

typedef struct SPlayerLoadPacket
{
	DWORD	account_id;
	DWORD	player_id;
	BYTE	account_index;	/* account ¿¡¼­ÀÇ À§Ä¡ */
} TPlayerLoadPacket;

typedef struct SPlayerCreatePacket
{
	char		login[LOGIN_MAX_LEN + 1];
	char		passwd[PASSWD_MAX_LEN + 1];
	DWORD		account_id;
	BYTE		account_index;
	TPlayerTable	player_table;
} TPlayerCreatePacket;

typedef struct SPlayerDeletePacket
{
	char	login[LOGIN_MAX_LEN + 1];
	DWORD	player_id;
	BYTE	account_index;
	//char	name[CHARACTER_NAME_MAX_LEN + 1];
	char	private_code[8];
} TPlayerDeletePacket;

typedef struct SPlayerChangeHairPacket
{
	char	login[LOGIN_MAX_LEN + 1];
	DWORD	player_index;
	int		hairIndex;
} TPlayerChangeHairPacket;

typedef struct SLogoutPacket
{
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
} TLogoutPacket;

typedef struct SPlayerCountPacket
{
	DWORD	dwCount;
} TPlayerCountPacket;

#define SAFEBOX_MAX_NUM			180
#define SAFEBOX_PASSWORD_MAX_LEN	6

typedef struct SSafeboxTable
{
	DWORD	dwID;
	BYTE	bSize;
	DWORD	dwGold;
	WORD	wItemCount;
} TSafeboxTable;

typedef struct SSafeboxChangeSizePacket
{
	DWORD	dwID;
	BYTE	bSize;
} TSafeboxChangeSizePacket;

typedef struct SSafeboxLoadPacket
{
	DWORD	dwID;
	char	szLogin[LOGIN_MAX_LEN + 1];
	char	szPassword[SAFEBOX_PASSWORD_MAX_LEN + 1];
} TSafeboxLoadPacket;

typedef struct SSafeboxChangePasswordPacket
{
	DWORD	dwID;
	char	szOldPassword[SAFEBOX_PASSWORD_MAX_LEN + 1];
	char	szNewPassword[SAFEBOX_PASSWORD_MAX_LEN + 1];
} TSafeboxChangePasswordPacket;

typedef struct SSafeboxChangePasswordPacketAnswer
{
	BYTE	flag;
} TSafeboxChangePasswordPacketAnswer;

typedef struct SEmpireSelectPacket
{
	DWORD	dwAccountID;
	BYTE	bEmpire;
} TEmpireSelectPacket;

typedef struct SPacketGDSetup
{
	char	szPublicIP[16];	// Public IP which listen to users
	BYTE	bChannel;	// Ã¤³Î
	WORD	wListenPort;	// Å¬¶óÀÌ¾ðÆ®°¡ Á¢¼ÓÇÏ´Â Æ÷Æ® ¹øÈ£
	WORD	wP2PPort;	// ¼­¹ö³¢¸® ¿¬°á ½ÃÅ°´Â P2P Æ÷Æ® ¹øÈ£
	long	alMaps[MAP_ALLOW_LIMIT];
	DWORD	dwLoginCount;
	BYTE	bAuthServer;
} TPacketGDSetup;

typedef struct SPacketDGMapLocations
{
	BYTE	bCount;
} TPacketDGMapLocations;

typedef struct SMapLocation
{
	long	alMaps[MAP_ALLOW_LIMIT];
	char	szHost[MAX_HOST_LENGTH + 1];
	WORD	wPort;
} TMapLocation;

typedef struct SPacketDGP2P
{
	char	szHost[MAX_HOST_LENGTH + 1];
	WORD	wPort;
	BYTE	bChannel;
} TPacketDGP2P;

typedef struct SPacketGDDirectEnter
{
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
	BYTE	index;
} TPacketGDDirectEnter;

typedef struct SPacketDGDirectEnter
{
	TAccountTable accountTable;
	TPlayerTable playerTable;
} TPacketDGDirectEnter;

typedef struct SPacketGuildSkillUpdate
{
	DWORD guild_id;
	int amount;
	BYTE skill_levels[12];
	BYTE skill_point;
	BYTE save;
} TPacketGuildSkillUpdate;

typedef struct SPacketGuildExpUpdate
{
	DWORD guild_id;
	int amount;
} TPacketGuildExpUpdate;

typedef struct SPacketGuildChangeMemberData
{
	DWORD guild_id;
	DWORD pid;
	DWORD offer;
	BYTE level;
	BYTE grade;
} TPacketGuildChangeMemberData;


typedef struct SPacketDGLoginAlready
{
	char	szLogin[LOGIN_MAX_LEN + 1];
} TPacketDGLoginAlready;

typedef struct TPacketAffectElement
{
	DWORD	dwType;
	BYTE	bApplyOn;
	long	lApplyValue;
	DWORD	dwFlag;
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
	DWORD	dwFlag2;
//#endif
	long	lDuration;
	long	lSPCost;
} TPacketAffectElement;

typedef struct SPacketGDAddAffect
{
	DWORD					dwPID;
	TPacketAffectElement	elem;
} TPacketGDAddAffect;

typedef struct SPacketGDRemoveAffect
{
	DWORD	dwPID;
	DWORD	dwType;
	BYTE	bApplyOn;
} TPacketGDRemoveAffect;

typedef struct SPacketGDHighscore
{
	DWORD	dwPID;
	long	lValue;
	char	cDir;
	char	szBoard[21];
} TPacketGDHighscore;

typedef struct SPacketPartyCreate
{
	DWORD	dwLeaderPID;
} TPacketPartyCreate;

typedef struct SPacketPartyDelete
{
	DWORD	dwLeaderPID;
} TPacketPartyDelete;

typedef struct SPacketPartyAdd
{
	DWORD	dwLeaderPID;
	DWORD	dwPID;
	BYTE	bState;
} TPacketPartyAdd;

typedef struct SPacketPartyRemove
{
	DWORD	dwLeaderPID;
	DWORD	dwPID;
} TPacketPartyRemove;

typedef struct SPacketPartyStateChange
{
	DWORD	dwLeaderPID;
	DWORD	dwPID;
	BYTE	bRole;
	BYTE	bFlag;
} TPacketPartyStateChange;

typedef struct SPacketPartySetMemberLevel
{
	DWORD	dwLeaderPID;
	DWORD	dwPID;
	BYTE	bLevel;
} TPacketPartySetMemberLevel;

typedef struct SPacketGDBoot
{
    DWORD	dwItemIDRange[2];
	char	szIP[16];
} TPacketGDBoot;

typedef struct SPacketGuild
{
	DWORD	dwGuild;
	DWORD	dwInfo;
} TPacketGuild;

typedef struct SPacketGDGuildAddMember
{
	DWORD	dwPID;
	DWORD	dwGuild;
	BYTE	bGrade;
} TPacketGDGuildAddMember;

typedef struct SPacketDGGuildMember
{
	DWORD	dwPID;
	DWORD	dwGuild;
	BYTE	bGrade;
	BYTE	isGeneral;
	BYTE	bJob;
	BYTE	bLevel;
	DWORD	dwOffer;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketDGGuildMember;

typedef struct SPacketGuildWar
{
	BYTE	bType;
	BYTE	bWar;
	DWORD	dwGuildFrom;
	DWORD	dwGuildTo;
	long	lWarPrice;
	long	lInitialScore;
} TPacketGuildWar;

// Game -> DB : »ó´ëÀû º¯È­°ª
// DB -> Game : ÅäÅ»µÈ ÃÖÁ¾°ª
typedef struct SPacketGuildWarScore
{
	DWORD dwGuildGainPoint;
	DWORD dwGuildOpponent;
	long lScore;
	long lBetScore;
} TPacketGuildWarScore;

typedef struct SRefineMaterial
{
	DWORD vnum;
	int count;
} TRefineMaterial;

typedef struct SRefineTable
{
	//DWORD src_vnum;
	//DWORD result_vnum;
	DWORD id;
	BYTE material_count;
	int cost; // ¼Ò¿ä ºñ¿ë
	int prob; // È®·ü
	TRefineMaterial materials[REFINE_MATERIAL_MAX_NUM];

} TRefineTable;

typedef struct SBanwordTable
{
	char szWord[BANWORD_MAX_LEN + 1];
} TBanwordTable;

typedef struct SPacketGDChangeName
{
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGDChangeName;

typedef struct SPacketDGChangeName
{
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketDGChangeName;

typedef struct SPacketGuildLadder
{
	DWORD dwGuild;
	long lLadderPoint;
	long lWin;
	long lDraw;
	long lLoss;
} TPacketGuildLadder;

typedef struct SPacketGuildLadderPoint
{
	DWORD dwGuild;
	long lChange;
} TPacketGuildLadderPoint;

typedef struct SPacketGDSMS
{
	char szFrom[CHARACTER_NAME_MAX_LEN + 1];
	char szTo[CHARACTER_NAME_MAX_LEN + 1];
	char szMobile[MOBILE_MAX_LEN + 1];
	char szMsg[SMS_MAX_LEN + 1];
} TPacketGDSMS;

typedef struct SPacketGuildUseSkill
{
	DWORD dwGuild;
	DWORD dwSkillVnum;
	DWORD dwCooltime;
} TPacketGuildUseSkill;

typedef struct SPacketGuildSkillUsableChange
{
	DWORD dwGuild;
	DWORD dwSkillVnum;
	BYTE bUsable;
} TPacketGuildSkillUsableChange;

typedef struct SPacketGDLoginKey
{
	DWORD dwAccountID;
	DWORD dwLoginKey;
} TPacketGDLoginKey;

typedef struct SPacketGDAuthLogin
{
	DWORD	dwID;
	DWORD	dwLoginKey;
	char	szLogin[LOGIN_MAX_LEN + 1];
	char	szSocialID[SOCIAL_ID_MAX_LEN + 1];
	DWORD	adwClientKey[4];
	BYTE	bBillType;
	DWORD	dwBillID;
	int		iPremiumTimes[PREMIUM_MAX_NUM];
} TPacketGDAuthLogin;

typedef struct SPacketGDLoginByKey
{
	char	szLogin[LOGIN_MAX_LEN + 1];
	DWORD	dwLoginKey;
	DWORD	adwClientKey[4];
	char	szIP[MAX_HOST_LENGTH + 1];
} TPacketGDLoginByKey;

/**
 * @version 05/06/08	Bang2ni - Áö¼Ó½Ã°£ Ãß°¡
 */
typedef struct SPacketGiveGuildPriv
{
	BYTE type;
	int value;
	DWORD guild_id;
	time_t duration_sec;	///< Áö¼Ó½Ã°£
} TPacketGiveGuildPriv;
typedef struct SPacketGiveEmpirePriv
{
	BYTE type;
	int value;
	BYTE empire;
	time_t duration_sec;
} TPacketGiveEmpirePriv;
typedef struct SPacketGiveCharacterPriv
{
	BYTE type;
	int value;
	DWORD pid;
} TPacketGiveCharacterPriv;
typedef struct SPacketRemoveGuildPriv
{
	BYTE type;
	DWORD guild_id;
} TPacketRemoveGuildPriv;
typedef struct SPacketRemoveEmpirePriv
{
	BYTE type;
	BYTE empire;
} TPacketRemoveEmpirePriv;

typedef struct SPacketDGChangeCharacterPriv
{
	BYTE type;
	int value;
	DWORD pid;
	BYTE bLog;
} TPacketDGChangeCharacterPriv;

/**
 * @version 05/06/08	Bang2ni - Áö¼Ó½Ã°£ Ãß°¡
 */
typedef struct SPacketDGChangeGuildPriv
{
	BYTE type;
	int value;
	DWORD guild_id;
	BYTE bLog;
	time_t end_time_sec;	///< Áö¼Ó½Ã°£
} TPacketDGChangeGuildPriv;

typedef struct SPacketDGChangeEmpirePriv
{
	BYTE type;
	int value;
	BYTE empire;
	BYTE bLog;
	time_t end_time_sec;
} TPacketDGChangeEmpirePriv;

typedef struct SPacketMoneyLog
{
	BYTE type;
	DWORD vnum;
	long long	gold;
} TPacketMoneyLog;

typedef struct SPacketGDGuildMoney
{
	DWORD dwGuild;
	INT iGold;
} TPacketGDGuildMoney;

typedef struct SPacketDGGuildMoneyChange
{
	DWORD dwGuild;
	INT iTotalGold;
} TPacketDGGuildMoneyChange;

typedef struct SPacketDGGuildMoneyWithdraw
{
	DWORD dwGuild;
	INT iChangeGold;
} TPacketDGGuildMoneyWithdraw;

typedef struct SPacketGDGuildMoneyWithdrawGiveReply
{
	DWORD dwGuild;
	INT iChangeGold;
	BYTE bGiveSuccess;
} TPacketGDGuildMoneyWithdrawGiveReply;

typedef struct SPacketSetEventFlag
{
	char	szFlagName[EVENT_FLAG_NAME_MAX_LEN + 1];
	long	lValue;
} TPacketSetEventFlag;

typedef struct SPacketBillingLogin
{
	DWORD	dwLoginKey;
	BYTE	bLogin;
} TPacketBillingLogin;

typedef struct SPacketBillingRepair
{
	DWORD	dwLoginKey;
	char	szLogin[LOGIN_MAX_LEN + 1];
	char	szHost[MAX_HOST_LENGTH + 1];
} TPacketBillingRepair;

typedef struct SPacketBillingExpire
{
	char	szLogin[LOGIN_MAX_LEN + 1];
	BYTE	bBillType;
	DWORD	dwRemainSeconds;
} TPacketBillingExpire;

typedef struct SPacketLoginOnSetup
{
	DWORD   dwID;
	char    szLogin[LOGIN_MAX_LEN + 1];
	char    szSocialID[SOCIAL_ID_MAX_LEN + 1];
	char    szHost[MAX_HOST_LENGTH + 1];
	DWORD   dwLoginKey;
	DWORD   adwClientKey[4];
} TPacketLoginOnSetup;

typedef struct SPacketGDCreateObject
{
	DWORD	dwVnum;
	DWORD	dwLandID;
	INT		lMapIndex;
	INT	 	x, y;
	float	xRot;
	float	yRot;
	float	zRot;
} TPacketGDCreateObject;

typedef struct SPacketGDHammerOfTor
{
	DWORD 	key;
	DWORD	delay;
} TPacketGDHammerOfTor;

typedef struct SPacketGDVCard
{
	DWORD	dwID;
	char	szSellCharacter[CHARACTER_NAME_MAX_LEN + 1];
	char	szSellAccount[LOGIN_MAX_LEN + 1];
	char	szBuyCharacter[CHARACTER_NAME_MAX_LEN + 1];
	char	szBuyAccount[LOGIN_MAX_LEN + 1];
} TPacketGDVCard;

typedef struct SGuildReserve
{
	DWORD       dwID;
	DWORD       dwGuildFrom;
	DWORD       dwGuildTo;
	DWORD       dwTime;
	BYTE        bType;
	long        lWarPrice;
	long        lInitialScore;
	bool        bStarted;
	DWORD	dwBetFrom;
	DWORD	dwBetTo;
	long	lPowerFrom;
	long	lPowerTo;
	long	lHandicap;
} TGuildWarReserve;

typedef struct
{
	DWORD	dwWarID;
	char	szLogin[LOGIN_MAX_LEN + 1];
	DWORD	dwGold;
	DWORD	dwGuild;
} TPacketGDGuildWarBet;

// Marriage

typedef struct
{
	DWORD dwPID1;
	DWORD dwPID2;
	time_t tMarryTime;
	char szName1[CHARACTER_NAME_MAX_LEN + 1];
	char szName2[CHARACTER_NAME_MAX_LEN + 1];
} TPacketMarriageAdd;

typedef struct
{
	DWORD dwPID1;
	DWORD dwPID2;
	INT  iLovePoint;
	BYTE  byMarried;
} TPacketMarriageUpdate;

typedef struct
{
	DWORD dwPID1;
	DWORD dwPID2;
} TPacketMarriageRemove;

typedef struct
{
	DWORD dwPID1;
	DWORD dwPID2;
} TPacketWeddingRequest;

typedef struct
{
	DWORD dwPID1;
	DWORD dwPID2;
	DWORD dwMapIndex;
} TPacketWeddingReady;

typedef struct
{
	DWORD dwPID1;
	DWORD dwPID2;
} TPacketWeddingStart;

typedef struct
{
	DWORD dwPID1;
	DWORD dwPID2;
} TPacketWeddingEnd;

/// °³ÀÎ»óÁ¡ °¡°ÝÁ¤º¸ÀÇ Çì´õ. °¡º¯ ÆÐÅ¶À¸·Î ÀÌ µÚ¿¡ byCount ¸¸Å­ÀÇ TItemPriceInfo °¡ ¿Â´Ù.
typedef struct SPacketMyshopPricelistHeader
{
	DWORD	dwOwnerID;	///< °¡°ÝÁ¤º¸¸¦ °¡Áø ÇÃ·¹ÀÌ¾î ID
	BYTE	byCount;	///< °¡°ÝÁ¤º¸ °¹¼ö
} TPacketMyshopPricelistHeader;

/// °³ÀÎ»óÁ¡ÀÇ ´ÜÀÏ ¾ÆÀÌÅÛ¿¡ ´ëÇÑ °¡°ÝÁ¤º¸
typedef struct SItemPriceInfo
{
	DWORD	dwVnum;		///< ¾ÆÀÌÅÛ vnum
	long long	dwPrice;
} TItemPriceInfo;

/// °³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ °¡°ÝÁ¤º¸ ¸®½ºÆ® Å×ÀÌºí
typedef struct SItemPriceListTable
{
	DWORD	dwOwnerID;	///< °¡°ÝÁ¤º¸¸¦ °¡Áø ÇÃ·¹ÀÌ¾î ID
	BYTE	byCount;	///< °¡°ÝÁ¤º¸ ¸®½ºÆ®ÀÇ °¹¼ö

	TItemPriceInfo	aPriceInfo[SHOP_PRICELIST_MAX_NUM];	///< °¡°ÝÁ¤º¸ ¸®½ºÆ®
} TItemPriceListTable;

typedef struct
{
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	long lDuration;
} TPacketBlockChat;

// PCBANG_IP_LIST
typedef struct SPacketPCBangIP
{
	DWORD id;
	DWORD ip;
} TPacketPCBangIP;
// END_OF_PCBANG_IP_LIST


//ADMIN_MANAGER
typedef struct TAdminInfo
{
	int m_ID;				//°íÀ¯ID
	char m_szAccount[32];	//°èÁ¤
	char m_szName[32];		//Ä³¸¯ÅÍÀÌ¸§
	char m_szContactIP[16];	//Á¢±Ù¾ÆÀÌÇÇ
	char m_szServerIP[16];  //¼­¹ö¾ÆÀÌÇÇ
	int m_Authority;		//±ÇÇÑ
} tAdminInfo;
//END_ADMIN_MANAGER

//BOOT_LOCALIZATION
struct tLocale
{
	char szValue[32];
	char szKey[32];
};
//BOOT_LOCALIZATION

//RELOAD_ADMIN
typedef struct SPacketReloadAdmin
{
	char szIP[16];
} TPacketReloadAdmin;
//END_RELOAD_ADMIN

typedef struct TMonarchInfo
{
	DWORD pid[4];  // ±ºÁÖÀÇ PID
	int64_t money[4];  // ±ºÁÖÀÇ º°°³ µ·
	char name[4][32];  // ±ºÁÖÀÇ ÀÌ¸§
	char date[4][32];  // ±ºÁÖ µî·Ï ³¯Â¥
} MonarchInfo;

typedef struct TMonarchElectionInfo
{
	DWORD pid;  // ÅõÇ¥ ÇÑ»ç¶÷ PID
	DWORD selectedpid; // ÅõÇ¥ ´çÇÑ PID ( ±ºÁÖ Âü°¡ÀÚ )
	char date[32]; // ÅõÇ¥ ³¯Â¥
} MonarchElectionInfo;

// ±ºÁÖ Ãâ¸¶ÀÚ
typedef struct tMonarchCandidacy
{
	DWORD pid;
	char name[32];
	char date[32];
} MonarchCandidacy;

typedef struct tChangeMonarchLord
{
	BYTE bEmpire;
	DWORD dwPID;
} TPacketChangeMonarchLord;

typedef struct tChangeMonarchLordACK
{
	BYTE bEmpire;
	DWORD dwPID;
	char szName[32];
	char szDate[32];
} TPacketChangeMonarchLordACK;

// Block Country Ip
typedef struct tBlockCountryIp
{
	DWORD	ip_from;
	DWORD	ip_to;
} TPacketBlockCountryIp;

enum EBlockExceptionCommand
{
	BLOCK_EXCEPTION_CMD_ADD = 1,
	BLOCK_EXCEPTION_CMD_DEL = 2,
};

// Block Exception Account
typedef struct tBlockException
{
	BYTE	cmd;	// 1 == add, 2 == delete
	char	login[LOGIN_MAX_LEN + 1];
}TPacketBlockException;

typedef struct tChangeGuildMaster
{
	DWORD dwGuildID;
	DWORD idFrom;
	DWORD idTo;
} TPacketChangeGuildMaster;

typedef struct tItemIDRange
{
	DWORD dwMin;
	DWORD dwMax;
	DWORD dwUsableItemIDMin;
} TItemIDRangeTable;

typedef struct tUpdateHorseName
{
	DWORD dwPlayerID;
	char szHorseName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketUpdateHorseName;

typedef struct tDC
{
	char	login[LOGIN_MAX_LEN + 1];
} TPacketDC;

typedef struct tNeedLoginLogInfo
{
	DWORD dwPlayerID;
} TPacketNeedLoginLogInfo;

//µ¶ÀÏ ¼±¹° ¾Ë¸² ±â´É Å×½ºÆ®¿ë ÆÐÅ¶ Á¤º¸
typedef struct tItemAwardInformer
{
	char	login[LOGIN_MAX_LEN + 1];
	char	command[20];		//¸í·É¾î
	unsigned int vnum;			//¾ÆÀÌÅÛ
} TPacketItemAwardInfromer;
// ¼±¹° ¾Ë¸² ±â´É »èÁ¦¿ë ÆÐÅ¶ Á¤º¸
typedef struct tDeleteAwardID
{
	DWORD dwID;
} TPacketDeleteAwardID;

typedef struct SChannelStatus
{
	short nPort;
	BYTE bStatus;
} TChannelStatus;

#ifdef ENABLE_SWITCHBOT
struct TSwitchbotAttributeAlternativeTable
{
	TPlayerItemAttribute attributes[MAX_NORM_ATTR_NUM];

	bool IsConfigured() const
	{
		for (const auto& it : attributes)
		{
			if (it.bType && it.sValue)
			{
				return true;
			}
		}

		return false;
	}
};

struct TSwitchbotTable
{
	DWORD player_id;
	bool active[SWITCHBOT_SLOT_COUNT];
	bool finished[SWITCHBOT_SLOT_COUNT];
	DWORD items[SWITCHBOT_SLOT_COUNT];
	TSwitchbotAttributeAlternativeTable alternatives[SWITCHBOT_SLOT_COUNT][SWITCHBOT_ALTERNATIVE_COUNT];

	TSwitchbotTable() : player_id(0)
	{
		memset(&items, 0, sizeof(items));
		memset(&alternatives, 0, sizeof(alternatives));
		memset(&active, false, sizeof(active));
		memset(&finished, false, sizeof(finished));
	}
};

struct TSwitchbottAttributeTable
{
	BYTE attribute_set;
	int apply_num;
	long max_value;
};
#endif


#ifdef ENABLE_DUNGEON_INFO
typedef struct SFDateInfoPacket
{
	int 	id_dungeon;
	char 	name[100];
	char 	img[100];
	int 	lv_min;
	int 	lv_max;
	int 	party_max;
	DWORD 	respawn;
	DWORD 	time_room;
	int   	time_respawn_dungeon;
	int 	time_room_dungeon;
	char 	entrance[100];
	char 	resistance[100];
	char 	force[100];
	int 	vnum_item;
	int 	count_item;
	int 	count_finish;
	int 	time_finish;
	int 	difficulty;
	int 	damage_done;
	int 	damage_received;
	int 	id_boss;


}TFDateInfoPacket;

typedef struct SFDateMision
{
	
	int 	vnum_mob_mision;
	int 	count_mob_mision;
	int 	count_mob_a_mision;
	int 	status_mision;
}TFDateMision;

typedef struct SFDateInfo
{
	int 	id;
	SFDateInfoPacket dateinfo;
	int 	index_map;
	int 	x_dungeon_join;
	int 	y_dungeon_join;
	int 	x_map_npc;
	int 	y_map_npc;
	SFDateMision	dateinfom;				
}TFDateInfo;

typedef struct  SFDateRanking
{
	int vnum_mob_r;
	int type;
	char name_r[24];
	int extra_r;

}TFDateInfoRanking;

#endif

#ifdef ENABLE_AUTOMATIC_EVENT
typedef struct SEventTable
{
	DWORD dwVnum;
	BYTE bType;
	bool IsActivated;
	int iDate;
	int iDuration;
	char szFlagName[EVENT_FLAG_NAME_MAX_LEN + 1];
	BYTE bPrivEmpire;
	BYTE bPrivType;
	int iPrivValue;
	DWORD dwMessageID;
	int iNextNotice;
	int iMessageInterval;
}TEventTable;

typedef struct SPacketDGAutomaticEvent
{
	DWORD dwVnum;
	bool bStatus;

	SPacketDGAutomaticEvent(DWORD vnum, bool status)
	{
		dwVnum = vnum;
		bStatus = status;
	}
}TPacketDGAutomaticEvent;

typedef struct SPacketDGMessageAutomaticEvent
{
	DWORD dwMessageID;
	DWORD dwMessageType;
	DWORD dwDays;
	DWORD dwHours;
	DWORD dwMinutes;

	SPacketDGMessageAutomaticEvent(DWORD messageid, DWORD messageType, DWORD days, DWORD hours, DWORD minutes)
	{
		dwMessageID = messageid;
		dwMessageType = messageType;
		dwDays = days;
		dwHours = hours;
		dwMinutes = minutes;
	}
}TPacketDGMessageAutomaticEvent;
#endif

#ifdef ENABLE_NEW_PET_SYSTEM
enum EPetOptions
{
	EPetOption_Followable = 1 << 0,
	EPetOption_Mountable = 1 << 1,
	EPetOption_Summonable = 1 << 2,
	EPetOption_Combatable = 1 << 3,
};
enum m_petMisc {
	NORMAL_PET,
	NORMAL_LEVEL_PET,
};
enum EPetPointTypes
{
	POINT_PET_DURATION = 0,
	POINT_PET_LEVEL = 1,
	POINT_PET_EXP = 2,
	POINT_PET_EVOLVE = 3,
	POINT_PET_SKILLCOUNT = 4,

	POINT_PET_BONUS_1 = 5,
	POINT_PET_BONUS_2 = 6,
	POINT_PET_BONUS_3 = 7,
	

	POINT_PET_SKILL_INDEX_1 = 100,
	POINT_PET_SKILL_INDEX_2,
	POINT_PET_SKILL_INDEX_3,
	POINT_PET_SKILL_INDEX_4,
	POINT_PET_SKILL_INDEX_5,
	POINT_PET_SKILL_INDEX_6,
	POINT_PET_SKILL_INDEX_7,
	POINT_PET_SKILL_INDEX_8,
	POINT_PET_SKILL_INDEX_9,
	POINT_PET_SKILL_INDEX_10,
	POINT_PET_SKILL_INDEX_11,
	POINT_PET_SKILL_INDEX_12,
	POINT_PET_SKILL_INDEX_13,
	POINT_PET_SKILL_INDEX_14,
	POINT_PET_SKILL_INDEX_15 = 114,

	POINT_PET_SKILL_LEVEL_1 = 150,
	POINT_PET_SKILL_LEVEL_2,
	POINT_PET_SKILL_LEVEL_3,
	POINT_PET_SKILL_LEVEL_4,
	POINT_PET_SKILL_LEVEL_5,
	POINT_PET_SKILL_LEVEL_6,
	POINT_PET_SKILL_LEVEL_7,
	POINT_PET_SKILL_LEVEL_8,
	POINT_PET_SKILL_LEVEL_9,
	POINT_PET_SKILL_LEVEL_10,
	POINT_PET_SKILL_LEVEL_11,
	POINT_PET_SKILL_LEVEL_12,
	POINT_PET_SKILL_LEVEL_13,
	POINT_PET_SKILL_LEVEL_14,
	POINT_PET_SKILL_LEVEL_15 = 164,
};
typedef struct SPetApply
{
	BYTE bType;
	long lValue;
} TPetApply;

typedef struct SPetSkill
{
	BYTE bType;
	short sLevel;
} TPetSkill;
#endif

#ifdef ENABLE_ITEMSHOP
enum EItemShopSubheaders : uint8_t
{
	ITEMSHOP_LOAD			= 0,
	ITEMSHOP_LOG			= 1,
	ITEMSHOP_BUY			= 2,
	ITEMSHOP_DRAGONCOIN		= 3,
	ITEMSHOP_RELOAD			= 4,
	ITEMSHOP_UPDATE_ITEM	= 5,
};

typedef struct SIShopData
{
	DWORD		id;
	DWORD		itemVnum;
	long long	itemPrice;
#ifdef USE_ITEMSHOP_RENEWED
	long long	itemPriceJD;
#endif
	int			topSellingIndex;
	BYTE		discount;
	int			offerTime;
	int			addedTime;
	long long	sellCount;
	int			week_limit;
	int			month_limit;
	int 		maxSellCount;
} TIShopData;

typedef struct SIShopLogData
{
	DWORD		accountID;
	char		playerName[CHARACTER_NAME_MAX_LEN+1];
	char		buyDate[21];
	int			buyTime;
	char		ipAdress[16];
	DWORD		itemID;
	DWORD		itemVnum;
	int			itemCount;
	long long	itemPrice;
#ifdef USE_ITEMSHOP_RENEWED
	long long	itemPriceJD;
#endif
} TIShopLogData;
#endif

#ifdef ENABLE_EVENT_MANAGER
typedef struct event_struct_
{
	WORD	eventID;
	BYTE	eventIndex;
	int		startTime;
	int		endTime;
	BYTE	empireFlag;
	BYTE	channelFlag;
	DWORD	value[4];
	bool	eventStatus;
	bool	eventTypeOnlyStart;
	char	startTimeText[25];
	char	endTimeText[25];
}TEventManagerData;
enum
{
	EVENT_MANAGER_LOAD,
	EVENT_MANAGER_EVENT_STATUS,
	EVENT_MANAGER_REMOVE_EVENT,
	EVENT_MANAGER_UPDATE,

	BONUS_EVENT = 1,
	DOUBLE_BOSS_LOOT_EVENT = 2,
	DOUBLE_METIN_LOOT_EVENT = 3,
	DOUBLE_MISSION_BOOK_EVENT = 4,
	DUNGEON_COOLDOWN_EVENT = 5,
	DUNGEON_TICKET_LOOT_EVENT = 6,
	EMPIRE_WAR_EVENT = 7,
	MOONLIGHT_EVENT = 8,
	TOURNAMENT_EVENT = 9,
	WHELL_OF_FORTUNE_EVENT = 10,
	HALLOWEEN_EVENT = 11,
	NPC_SEARCH_EVENT = 12,
	EXP_EVENT = 13,
	ITEM_DROP_EVENT = 14,
	YANG_DROP_EVENT = 15,
	CARD_EVENT = 16,
	CATCH_KING_EVENT = 17,
	TREASURE_KEY = 18,
	BEGGINER_EVENT = 19,
};
#endif
#ifdef ENABLE_MULTI_FARM_BLOCK
typedef struct SMultiFarm
{
	DWORD	playerID;
	bool	farmStatus;
	BYTE	affectType;
	int		affectTime;
	char	playerName[CHARACTER_NAME_MAX_LEN+1];
	SMultiFarm(DWORD id_, const char* playerName_, bool status_, BYTE type_, int time_) : playerID(id_), farmStatus(status_), affectType(type_), affectTime(time_){
		strlcpy(playerName, playerName_, sizeof(playerName));
	}
}TMultiFarm;
#endif

#ifdef ENABLE_EXCHANGE_LOG
typedef struct SExchangeLog
{
	char	owner[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	ownerPID;
	long long	ownerGold;
	char	ownerIP[15 + 1];
	char	target[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	targetPID;
	long long	targetGold;
	char	targetIP[15 + 1];
	char	date[25];
	bool	itemsLoaded;
}TExchangeLog;
typedef struct SExchangeLogItem
{
	WORD	pos;
	DWORD	vnum;
	WORD	count;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute    aAttr[ITEM_ATTRIBUTE_MAX_NUM];
	bool	isOwnerItem;
}TExchangeLogItem;
#endif

#ifdef __GEM_SYSTEM__
typedef struct SGemItem
{
	DWORD itemVnum;
	WORD itemCount;
	int itemPrice;
	bool itemBuyed;
} TGemItem;
#endif


#ifdef RENEWAL_MISSION_BOOKS
typedef struct SMissionBookData
{
	WORD	id;
	DWORD	missionItemIndex;
	BYTE type;
	DWORD subtype;
	BYTE levelRange;
	long long max;
	int maxTime;
	long long gold[2];
	DWORD exp[2];
	DWORD rewardItems[6];
	WORD rewardCount[6];
} TMissionBookData;
typedef struct SMissionBook
{
	long long value;
	int	leftTime;
	bool reward;
	int endTime;
}TMissionBook;
enum
{
	MISSION_BOOK_MAX = 5,

	MISSION_BOOK_TYPE_MONSTER = 0,
	MISSION_BOOK_TYPE_METINSTONE = 1,
	MISSION_BOOK_TYPE_BOSS = 2,
};
#endif

#ifdef __DUNGEON_INFO__
typedef struct SDungeonRank
{
	char name[CHARACTER_NAME_MAX_LEN + 1];
	BYTE level;
	int	value;
}TDungeonRank;
#endif
#ifdef ENABLE_IKASHOP_RENEWAL
//common
struct TPacketGDNewIkarusShop 
{
	BYTE bSubHeader;
};

struct TPacketDGNewIkarusShop 
{
	BYTE bSubHeader;
};


namespace ikashop
{
	//patch 08-03-2020
	enum class ExpirationType {
		EXPIRE_NONE,
		EXPIRE_REAL_TIME,
		EXPIRE_REAL_TIME_FIRST_USE,
	};

	struct TPriceInfo
	{
		long long yang = 0;
#ifdef ENABLE_CHEQUE_SYSTEM
		int cheque = 0;
#endif
		bool operator < (const TPriceInfo& rItem) const
		{
			return GetTotalYangAmount() < rItem.GetTotalYangAmount();
		}

		long long GetTotalYangAmount() const{
			long long total = yang;
#ifdef ENABLE_CHEQUE_SYSTEM
			total += (long long) YANG_PER_CHEQUE * (long long) cheque;
#endif
			return total;
		}
	};

	struct TShopPlayerItem : public TPlayerItem
	{
		ExpirationType expiration;
	};

	struct TShopItem : public TShopPlayerItem
	{
		TPriceInfo price;
	};

#ifdef ENABLE_IKASHOP_ENTITIES
	struct TShopSpawn
	{
		int x;
		int y;
		int map;
	};
#endif

	struct TOfferInfo 
	{
		DWORD offerid;
		DWORD ownerid;
		DWORD itemid;
		DWORD buyerid;
		TPriceInfo price;
		bool notified;
		bool accepted;
		char buyername[CHARACTER_NAME_MAX_LEN+1];
		int64_t datetime;
	};

	//AUCTION
	struct TAuctionInfo : public TShopItem {
		char ownername[CHARACTER_NAME_MAX_LEN + 1];
		DWORD duration;
#ifdef ENABLE_IKASHOP_ENTITIES
	#ifdef EXTEND_IKASHOP_ULTIMATE
		TShopSpawn spawn;
	#endif
#endif
	};


	struct TAuctionOfferInfo {
		TPriceInfo price;
		DWORD ownerid;
		DWORD buyerid;
		char buyername[CHARACTER_NAME_MAX_LEN + 1];
		bool seenflag;
		int64_t datetime;
	};

	struct TValutesInfo
	{
		long long yang = 0;

		void operator +=(const TValutesInfo& r){
			yang += r.yang;
		}

		void operator -=(const TValutesInfo& r){
			yang -= r.yang;
		}
	};

	struct TShopInfo 
	{
		DWORD ownerid;
		DWORD duration;
		char name[OFFLINE_SHOP_NAME_MAX_LEN];
		DWORD count;
#ifdef ENABLE_IKASHOP_ENTITIES
		TShopSpawn spawn;
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		int decoration;
		int decoration_time;
		int lock_index;
#endif
	};

#ifdef EXTEND_IKASHOP_PRO
	enum class ENotificationType : uint8_t
	{
		SELLER_SOLD_ITEM,
		SELLER_NEW_OFFER_ON_ITEM,
		SELLER_SHOP_EXPIRED,
		SELLER_ITEM_EXPIRED,
		SELLER_AUCTION_NEW_OFFER,
		SELLER_AUCTION_EXPIRED,
		SELLER_AUCTION_SOLD,
		SELLER_OFFER_CANCELLED,
		BUYER_OFFER_DENIED,
		BUYER_OFFER_ACCEPTED,
		BUYER_OFFER_EXPIRED,
		BUYER_AUCTION_BUY,
		BUYER_AUCTION_RAISE,
		BUYER_AUCTION_LOST,
#ifdef EXTEND_IKASHOP_ULTIMATE
		SELLER_DECORATION_EXPIRED,
#endif
	};

	struct TNotificationInfo
	{
		DWORD id;
		ENotificationType type;
		DWORD what;
		char who[OFFLINE_SHOP_NAME_MAX_LEN];
		char format[OFFLINESHOP_NOTIFICATION_FORMAT_LEN];
		int64_t datetime;
		bool seenflag;
	};
#endif

#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TSaleHistory 
	{
		DWORD account;
		DWORD vnum;
		DWORD count;
		long long price;
		int64_t datetime;
	};
#endif

	// ### GAME TO DB ###

	enum eNewIkarusShopSubHeaderGD
	{
		SUBHEADER_GD_BUY_ITEM = 0,
		SUBHEADER_GD_BUY_LOCK_ITEM,
#ifdef USE_OFFLINESHOP_SEARCH_V2
        SUBHEADER_GD_BUY_LOCK_ITEM_MULTI,
#endif
		SUBHEADER_GD_EDIT_ITEM,
		SUBHEADER_GD_REMOVE_ITEM,
		SUBHEADER_GD_ADD_ITEM,

		SUBHEADER_GD_SHOP_FORCE_CLOSE,
		SUBHEADER_GD_SHOP_CREATE_NEW,
		SUBHEADER_GD_SHOP_CHANGE_NAME,


		SUBHEADER_GD_OFFER_CREATE,
		SUBHEADER_GD_OFFER_NOTIFIED,
		SUBHEADER_GD_OFFER_ACCEPT,
		SUBHEADER_GD_OFFER_CANCEL,

		SUBHEADER_GD_SAFEBOX_GET_ITEM,
		SUBHEADER_GD_SAFEBOX_GET_VALUTES,
		SUBHEADER_GD_SAFEBOX_ADD_ITEM,
		SUBHEADER_GD_SAFEBOX_GET_TIME_FEEDBACK,

		//AUCTION
		SUBHEADER_GD_AUCTION_CREATE,
		SUBHEADER_GD_AUCTION_ADD_OFFER,
		SUBHEADER_GD_AUCTION_CANCEL,
		SUBHEADER_GD_AUCTION_OFFER_SEEN,

#ifdef EXTEND_IKASHOP_PRO
		SUBHEADER_GD_NOTIFICATION_SEEN,
		SUBHEADER_GD_SHOP_RESTORE_DURATION,
#ifdef ENABLE_IKASHOP_ENTITIES
		SUBHEADER_GD_MOVE_SHOP_ENTITY,
#endif
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		SUBHEADER_GD_SHOP_DECORATION_USE,
		SUBHEADER_GD_SHOP_MOVE_ITEM,
		SUBHEADER_GD_SHOP_UNLOCK_CELL,
#endif
	};

	struct TSubPacketGDBuyItem {
		DWORD ownerid;
		DWORD itemid;
		DWORD guestid;
		bool success;
	};

	struct TSubPacketGDLockBuyItem {
		DWORD ownerid;
		DWORD itemid;
		DWORD guestid;
#ifdef EXTEND_IKASHOP_ULTIMATE
		DWORD accountid;
		long long normalizedPrice;
#endif
		long long seenprice;
	};

	struct TSubPacketGDEditItem {
		DWORD ownerid;
		DWORD itemid;
		TPriceInfo price;
	};

	struct TSubPacketGDRemoveItem {
		DWORD ownerid;
		DWORD itemid;
	};

	struct TSubPacketGDAddItem {
		DWORD ownerid;
		TShopItem sitem;
	};

	struct TSubPacketGDShopForceClose {
		DWORD ownerid;
		bool gm;
	};

	struct TSubPacketGDShopCreateNew {
		TShopInfo shop;
	};

	struct TSubPacketGDShopChangeName {
		DWORD ownerid;
		char name[OFFLINE_SHOP_NAME_MAX_LEN];
	};

	struct TSubPacketGDOfferCreate {
		DWORD ownerid;
		DWORD itemid;
		DWORD vnum;
		DWORD count;
		TOfferInfo offer;
	};

	struct TSubPacketGDOfferCancel {
		DWORD offerid;
		DWORD ownerid;
		DWORD requester;
	};

	struct TSubPacketGDOfferNotified {
		DWORD ownerid;
		DWORD offerid;
	};

	struct TSubPacketGDOfferAccept {
		DWORD ownerid;
		DWORD offerid;
	};

	struct TSubPacketGDSafeboxGetItem {
		DWORD ownerid;
		DWORD itemid;
	};

	struct TSubPacketGDSafeboxAddItem {
		DWORD ownerid;
		DWORD itemid;
		TShopPlayerItem item;
	};

	struct TSubPacketGDSafeboxGetItemFeedback {
		DWORD ownerid;
		DWORD itemid;
		bool result;
	};

	struct TSubPacketGDSafeboxGetValutes {
		DWORD ownerid;
		TValutesInfo valute;
	};


	//AUCTION
	struct TSubPacketGDAuctionCreate {
		TAuctionInfo auction;
	};

	struct TSubPacketGDAuctionAddOffer {
		TAuctionOfferInfo offer;
	};

	struct TSubPacketGDAuctionOfferSeen {
		DWORD ownerid;
		DWORD buyerid;
		long long price;
	};

#ifdef EXTEND_IKASHOP_PRO
	struct TSubPacketGDNotificationSeen
	{
		DWORD owner;
		DWORD id;
	};
#ifdef ENABLE_IKASHOP_ENTITIES
	struct TSubPacketGDMoveShopEntity
	{
		DWORD owner;
		TShopSpawn spawn;
	};
#endif
#endif


#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TSubPacketGDShopDecorationUse 
	{
		DWORD owner;
		int index;
	};

	struct TSubPacketGDShopMoveItem
	{
		DWORD owner;
		DWORD itemid;
		int destCell;
	};
#endif

	// ### DB TO GAME

	enum eSubHeaderDGNewIkarusShop
	{
		SUBHEADER_DG_BUY_ITEM,
		SUBHEADER_DG_LOCKED_BUY_ITEM,
		SUBHEADER_DG_EDIT_ITEM,
		SUBHEADER_DG_REMOVE_ITEM,
		SUBHEADER_DG_ADD_ITEM,

		SUBHEADER_DG_SHOP_FORCE_CLOSE,
		SUBHEADER_DG_SHOP_CREATE_NEW,
		SUBHEADER_DG_SHOP_CHANGE_NAME,
		SUBHEADER_DG_SHOP_EXPIRED,


		SUBHEADER_DG_OFFER_CREATE,
		SUBHEADER_DG_OFFER_NOTIFIED,
		SUBHEADER_DG_OFFER_ACCEPT,
		SUBHEADER_DG_OFFER_CANCEL,

		SUBHEADER_DG_LOAD_TABLES,

		SUBHEADER_DG_SAFEBOX_ADD_ITEM,
		SUBHEADER_DG_SAFEBOX_ADD_VALUTES,
		SUBHEADER_DG_SAFEBOX_LOAD,
		SUBHEADER_DG_SAFEBOX_EXPIRED_ITEM,
		SUBHEADER_DG_SAFEBOX_GET_ITEM_CONFIRM,

		//AUCTION
		SUBHEADER_DG_AUCTION_CREATE,
		SUBHEADER_DG_AUCTION_ADD_OFFER,
		SUBHEADER_DG_AUCTION_EXPIRED,
		SUBHEADER_DG_AUCTION_OFFER_SEEN,

#ifdef EXTEND_IKASHOP_PRO
		SUBHEADER_DG_NOTIFICATION_LOAD,
		SUBHEADER_DG_NOTIFICATION_FORWARD,
		SUBHEADER_DG_SHOP_RESTORE_DURATION,
#ifdef ENABLE_IKASHOP_ENTITIES
		SUBHEADER_DG_MOVE_SHOP_ENTITY,
#endif
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		SUBHEADER_DG_SALE_HISTORY_REGISTER,
		SUBHEADER_DG_SHOP_DECORATION_SET,
		SUBHEADER_DG_SHOP_MOVE_ITEM,
		SUBHEADER_DG_SHOP_UNLOCK_CELL,
#endif
#ifdef USE_OFFLINESHOP_SEARCH_V2
        SUBHEADER_DG_REFRESH_SHOPSEARCH,
#endif
	};

	struct TSubPacketDGBuyItem {
		DWORD ownerid;
		DWORD itemid;
		DWORD buyerid;
	};

	struct TSubPacketDGLockedBuyItem {
		DWORD ownerid;
		DWORD itemid;
		DWORD buyerid;
	};

	struct TSubPacketDGEditItem {
		DWORD ownerid;
		DWORD itemid;
		TPriceInfo price;
	};

	struct TSubPacketDGRemoveItem {
		DWORD ownerid;
		DWORD itemid;
		bool requester = false;
	};

	struct TSubPacketDGAddItem {
		DWORD ownerid;
		DWORD itemid;
		TShopItem item;
	};

	struct TSubPacketDGShopForceClose {
		DWORD ownerid;
	};

	struct TSubPacketDGShopCreateNew {
		TShopInfo shop;
	};

	struct TSubPacketDGShopChangeName {
		DWORD ownerid;
		char name[OFFLINE_SHOP_NAME_MAX_LEN];
	};

	struct TSubPacketDGOfferCreate {
		DWORD ownerid;
		DWORD itemid;
		TOfferInfo offer;
	};

	struct TSubPacketDGOfferCancel {
		DWORD offerid;
		DWORD ownerid;
		bool removing;
	};

	struct TSubPacketDGOfferNotified {
		DWORD ownerid;
		DWORD offerid;
	};

	struct TSubPacketDGOfferAccept {
		DWORD ownerid;
		DWORD offerid;
	};

	struct TSubPacketDGLoadTables {
		DWORD shopcount;
		DWORD offercount;
		DWORD auctioncount;
		DWORD auctionoffercount;
	};

	struct TSubPacketDGShopExpired {
		DWORD ownerid;
	};

	struct TSubPacketDGSafeboxAddItem {
		DWORD ownerid;
		DWORD itemid;
		TShopPlayerItem item;
	};

	struct TSubPacketDGSafeboxAddValutes {
		DWORD ownerid;
		TValutesInfo valute;
	};

	struct TSubPacketDGSafeboxLoad {
		DWORD ownerid;
		TValutesInfo valute;
		DWORD itemcount;
	};

	struct TSubPacketDGSafeboxExpiredItem {
		DWORD ownerid;
		DWORD itemid;
	};

	struct TSubPacketDGSafeboxGetItemConfirm {
		DWORD ownerid;
		DWORD itemid;
	};

	//AUCTION
	struct TSubPacketDGAuctionCreate {
		TAuctionInfo auction;
	};

	struct TSubPacketDGAuctionAddOffer {
		TAuctionOfferInfo offer;
	};

	struct TSubPacketDGAuctionExpired {
		DWORD ownerid;
	};

	struct TSubPacketDGAuctionOfferSeen {
		DWORD ownerid;
		DWORD buyerid;
		long long price;
	};

#ifdef EXTEND_IKASHOP_PRO
	struct TSubPacketDGNotificationLoad 
	{
		DWORD ownerid;
		DWORD count;
	};

	struct TSubPacketDGNotificationForward : public TNotificationInfo
	{
		DWORD ownerid;
	};
#ifdef ENABLE_IKASHOP_ENTITIES
	struct TSubPacketDGMoveShopEntity
	{
		DWORD owner;
		TShopSpawn spawn;
	};
#endif
#endif


#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TSubPacketDGShopDecorationSet
	{
		DWORD owner;
		int index;
		int time;
	};

	struct TSubPacketDGShopMoveItem
	{
		DWORD owner;
		DWORD itemid;
		int destCell;
	};

	struct TSubPacketDGShopUnlockCell 
	{
		DWORD owner;
		int lockIndex;
	};
#endif

}

#endif
#ifdef __FAKE_PLAYER__
//Game -> DB
typedef struct SPacketGDFakePlayer
{
	uint8_t	 bSubheader;
} TPacketGDFakePlayer;

enum EFakePlayerGDSubheaders
{
	FAKE_PLAYER_SUBHEADER_GD_LOGIN,
	FAKE_PLAYER_SUBHEADER_GD_LOGOUT,
};

typedef struct SFakePlayerLoginData
{
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long 	lMapIndex;
	long 	lX;
	long 	lY;
	int32_t	iRotation;
} TFakePlayerLoginData;

typedef struct SFakePlayerLogoutData
{
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TFakePlayerLogoutData;

//DB -> Game
typedef struct SPacketDGFakePlayer
{
	uint8_t	 bSubheader;
} TPacketDGFakePlayer;

enum EFakePlayerDGSubheaders
{
	FAKE_PLAYER_SUBHEADER_DG_LOGIN,
	FAKE_PLAYER_SUBHEADER_DG_LOGOUT,
	FAKE_PLAYER_SUBHEADER_DG_ALREADY_EXISTS,
	FAKE_PLAYER_SUBHEADER_DG_DONT_EXISTS,
};

typedef struct SFakePlayerItemData
{
	uint8_t bWearPos;
	uint32_t dwVnum;
	long alSockets[ITEM_SOCKET_MAX_NUM];		
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
} TFakePlayerItemData;
#endif

typedef struct SPacketGDAddRanking
{
	uint32_t		dwPID;
	uint8_t		bTypeRank;
	int64_t	lValue;
} TPacketGDAddRanking;

#ifdef USE_CRYSTAL_SYSTEM
struct SShardCraft
{
	DWORD	dwID;
	WORD	wCell;
	BYTE	bWindow;
};
#endif

#pragma pack()
#endif
