#ifndef __INC_PACKET_H__
#define __INC_PACKET_H__
#include "stdafx.h"

#ifdef USE_OFFLINESHOP_SEARCH_V2
#include <tuple>
#endif

enum
{
	HEADER_CG_HANDSHAKE				= 0xff,
	HEADER_CG_PONG				= 0xfe,
	HEADER_CG_TIME_SYNC				= 0xfc,
	HEADER_CG_KEY_AGREEMENT			= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_

	HEADER_CG_LOGIN				= 1,
	HEADER_CG_ATTACK				= 225,
	HEADER_CG_CHAT					= 231,
	HEADER_CG_CHARACTER_CREATE			= 4,
	HEADER_CG_CHARACTER_DELETE			= 5,
	HEADER_CG_CHARACTER_SELECT			= 6,
	HEADER_CG_MOVE				= 7,
	HEADER_CG_SYNC_POSITION			= 8,
	HEADER_CG_ENTERGAME				= 10,

	HEADER_CG_ITEM_USE				= 11,
	HEADER_CG_ITEM_DROP				= 224,
	HEADER_CG_ITEM_MOVE				= 13,
	HEADER_CG_ITEM_PICKUP			= 15,

	HEADER_CG_QUICKSLOT_ADD			= 16,
	HEADER_CG_QUICKSLOT_DEL			= 17,
	HEADER_CG_QUICKSLOT_SWAP			= 18,
	HEADER_CG_WHISPER				= 19,
	HEADER_CG_ITEM_DROP2			= 20,
	HEADER_CG_PLAYER_HAIR_CHANGE	= 21,
#ifdef ENABLE_SELL_ITEM
	HEADER_CG_ITEM_SELL				= 210,
#endif	
	HEADER_CG_ON_CLICK				= 26,
	HEADER_CG_EXCHANGE				= 27,
	HEADER_CG_CHARACTER_POSITION		= 28,
	HEADER_CG_SCRIPT_ANSWER			= 29,
	HEADER_CG_QUEST_INPUT_STRING		= 30,
	HEADER_CG_QUEST_CONFIRM			= 31,

	HEADER_CG_SHOP				= 50,
	HEADER_CG_FLY_TARGETING			= 51,
	HEADER_CG_USE_SKILL				= 233,
	HEADER_CG_ADD_FLY_TARGETING			= 53,
	HEADER_CG_SHOOT				= 54,

	HEADER_CG_ITEM_USE_TO_ITEM			= 60,
	HEADER_CG_TARGET			 	= 61,

	HEADER_CG_TEXT				= 64,	// @ ·Î ½ÃÀÛµÇ¸é ÅØ½ºÆ®¸¦ ÆÄ½ÌÇÑ´Ù.
	HEADER_CG_WARP				= 65,
	HEADER_CG_SCRIPT_BUTTON			= 66,
	HEADER_CG_MESSENGER				= 67,
#ifdef ENABLE_NEW_PET_SYSTEM
	HEADER_GC_PET_POINT_CHANGE=68,
#endif

	HEADER_CG_MALL_CHECKOUT			= 69,
	HEADER_CG_SAFEBOX_CHECKIN			= 70,	// ¾ÆÀÌÅÛÀ» Ã¢°í¿¡ ³Ö´Â´Ù.
	HEADER_CG_SAFEBOX_CHECKOUT			= 71,	// ¾ÆÀÌÅÛÀ» Ã¢°í·Î ºÎÅÍ »©¿Â´Ù.

	HEADER_CG_PARTY_INVITE			= 72,
	HEADER_CG_PARTY_INVITE_ANSWER		= 73,
	HEADER_CG_PARTY_REMOVE			= 74,
	HEADER_CG_PARTY_SET_STATE                   = 75,
	HEADER_CG_PARTY_USE_SKILL			= 76,
	HEADER_CG_SAFEBOX_ITEM_MOVE			= 77,
	HEADER_CG_PARTY_PARAMETER			= 78,

	HEADER_CG_GUILD				= 80,
	HEADER_CG_ANSWER_MAKE_GUILD			= 81,

	HEADER_CG_FISHING				= 222,

	HEADER_CG_ITEM_GIVE				= 83,
#if defined(ENABLE_FISHING_GAME)
	HEADER_CG_FISHING_GAME					= 85,
#endif
	HEADER_CG_EMPIRE				= 90,
	HEADER_CG_REFINE				= 96,

#ifdef __AURA_SYSTEM__
	HEADER_CG_AURA					= 97,
#endif

	HEADER_CG_MARK_LOGIN			= 100,
	HEADER_CG_MARK_CRCLIST			= 101,
	HEADER_CG_MARK_UPLOAD			= 102,
	HEADER_CG_MARK_IDXLIST			= 104,

	HEADER_CG_HACK					= 105,
	HEADER_CG_CHANGE_NAME			= 106,
	HEADER_CG_LOGIN2				= 109,
	HEADER_CG_DUNGEON				= 110,
	HEADER_CG_LOGIN3				= 111,

	HEADER_CG_GUILD_SYMBOL_UPLOAD	= 112,
	HEADER_CG_SYMBOL_CRC			= 113,

	// SCRIPT_SELECT_ITEM
	HEADER_CG_SCRIPT_SELECT_ITEM	= 114,

#ifdef ENABLE_SWITCHBOT
	HEADER_CG_SWITCHBOT				= 170,
#endif
	
#ifdef ENABLE_IKASHOP_RENEWAL
	HEADER_CG_NEW_OFFLINESHOP		= 119,
#endif
	// END_OF_SCRIPT_SELECT_ITEM

#ifdef ENABLE_MINI_GAME_CATCH_KING
	HEADER_CG_MINI_GAME_CATCH_KING	= 163,
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
	HEADER_CG_SWITCHBOT 			= 171,
#endif

#ifdef ENABLE_NINJEYE
	HEADER_CG_NINJEYE_RESPONSE		= 172,
#endif

#if defined(WJ_COMBAT_ZONE)
	HEADER_CG_COMBAT_ZONE_REQUEST_ACTION	= 117,
#endif
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	HEADER_CG_REQUEST_FLAG		= 118,
#endif

//	HEADER_CG_ROULETTE				= 200,
//	HEADER_CG_RUNUP_MATRIX_ANSWER	= 201,

	//
	HEADER_CG_PASSPOD_ANSWER		= 202,
#ifdef ENABLE_VOICE_CHAT
	HEADER_CG_VOICE_CHAT = 207,
#endif

	HEADER_CG_QUEST_INPUT_LONG_STRING		= 214,
	
	HEADER_CG_DRAGON_SOUL_REFINE			= 205,
	HEADER_CG_STATE_CHECKER					= 94,
#if defined(__BL_BATTLE_ROYALE__)
	HEADER_CG_BATTLE_ROYALE					= 236,
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	HEADER_GC_SUNGMA_ATTR_UPDATE = 231,
#endif

#if defined(__BL_67_ATTR__)
	HEADER_CG_67_ATTR 						= 208,
	HEADER_CG_CLOSE_67_ATTR 				= 209,
#endif

	HEADER_CG_CLIENT_VERSION			= 0xfd,
	HEADER_CG_CLIENT_VERSION2			= 0xf1,
	
#ifdef ENABLE_DUNGEON_INFO
    HEADER_CG_DUNGEON_INFO_SYSTEM 			= 213,
#endif

#ifdef ENABLE_CALENDAR_SYSTEM
    HEADER_CG_CALENDAR_SYSTEM 			= 220,
#endif
	HEADER_GC_RANK_INFO		= 223,


#ifdef ENABLE_DROP_ITEM_WORLDARD
	HEADER_CG_DROP_ITEM_SYSTEM					= 215,
#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_CG_CUBE_RENEWAL 						= 216,
#endif

#ifdef BATTLEPASS_WORLDARD
	HEADER_CG_BATTLEPASS						= 230,
#endif
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	HEADER_CG_ITEMS_PICKUP = 235,
#endif

	/********************************************************/
	HEADER_GC_KEY_AGREEMENT_COMPLETED = 0xfa, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_KEY_AGREEMENT			= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
#ifdef ENABLE_CSHIELD
	HEADER_CG_CSHIELD				= 232,
#endif
	HEADER_GC_TIME_SYNC				= 0xfc,
	HEADER_GC_PHASE					= 0xfd,
	HEADER_GC_BINDUDP				= 0xfe,
#ifdef ENABLE_CSHIELD
	HEADER_GC_CSHIELD				= 232,
#endif
	HEADER_GC_HANDSHAKE				= 0xff,

	HEADER_GC_CHARACTER_ADD			= 1,
	HEADER_GC_CHARACTER_DEL			= 2,
	HEADER_GC_MOVE					= 3,
	HEADER_GC_CHAT					= 4,
	HEADER_GC_SYNC_POSITION			= 5,

	HEADER_GC_LOGIN_SUCCESS			= 6,
	HEADER_GC_LOGIN_SUCCESS_NEWSLOT	= 32,
	HEADER_GC_LOGIN_FAILURE			= 7,

	HEADER_GC_CHARACTER_CREATE_SUCCESS		= 8,
	HEADER_GC_CHARACTER_CREATE_FAILURE		= 9,
	HEADER_GC_CHARACTER_DELETE_SUCCESS		= 10,
	HEADER_GC_CHARACTER_DELETE_WRONG_SOCIAL_ID	= 11,

	HEADER_GC_ATTACK				= 12,
	HEADER_GC_STUN				= 13,
	HEADER_GC_DEAD				= 14,

	HEADER_GC_MAIN_CHARACTER_OLD		= 15,
	HEADER_GC_CHARACTER_POINTS			= 16,
	HEADER_GC_CHARACTER_POINT_CHANGE		= 17,
	HEADER_GC_CHANGE_SPEED			= 18,
	HEADER_GC_CHARACTER_UPDATE			= 19,
	HEADER_GC_CHARACTER_UPDATE_NEW		= 24,

	HEADER_GC_ITEM_DEL				= 20,
	HEADER_GC_ITEM_SET				= 21,
	HEADER_GC_ITEM_USE				= 22,
	HEADER_GC_ITEM_DROP				= 23,
	HEADER_GC_ITEM_UPDATE			= 25,

	HEADER_GC_ITEM_GROUND_ADD			= 26,
	HEADER_GC_ITEM_GROUND_DEL			= 27,

	HEADER_GC_QUICKSLOT_ADD			= 28,
	HEADER_GC_QUICKSLOT_DEL			= 29,
	HEADER_GC_QUICKSLOT_SWAP			= 30,

	HEADER_GC_ITEM_OWNERSHIP			= 31,

	HEADER_GC_WHISPER				= 34,

	HEADER_GC_MOTION				= 36,
	HEADER_GC_PARTS				= 37,

	HEADER_GC_SHOP				= 38,

	HEADER_GC_DUEL_START				= 40,
	HEADER_GC_PVP                               = 41,
	HEADER_GC_EXCHANGE				= 42,
	HEADER_GC_CHARACTER_POSITION		= 43,

	HEADER_GC_PING				= 44,
	HEADER_GC_SCRIPT				= 45,
	HEADER_GC_QUEST_CONFIRM			= 46,

#ifdef USE_EXPANDED_ITEM_REFRESH
    HEADER_GC_ITEM_UPDATE_COUNT = 49,
#endif

#ifdef ENABLE_EVENT_MANAGER
	HEADER_GC_EVENT_MANAGER = 39,
#endif

#ifdef __NEWPET_SYSTEM__
	HEADER_CG_PetSetName = 147,
#endif

#ifdef ENABLE_SUPPORT_SYSTEM
	HEADER_GC_SUPPORT_SKILL			= 148,
#endif

	HEADER_GC_MOUNT				= 61,
	HEADER_GC_OWNERSHIP				= 62,
	HEADER_GC_TARGET			 	= 63,
#ifdef __SEND_TARGET_INFO__
	HEADER_GC_TARGET_INFO			= 58,
	HEADER_CG_TARGET_INFO_LOAD		= 59,
#endif
	HEADER_GC_WARP				= 65,
#ifdef ENABLE_SHOW_CHEST_DROP
	HEADER_GC_CHESTDROP				= 66,
#endif
#ifdef ENABLE_NEW_DETAILS_GUI
	HEADER_GC_KILL_LOG			= 68,
#endif
	HEADER_GC_ADD_FLY_TARGETING			= 69,
	HEADER_GC_CREATE_FLY			= 70,
	HEADER_GC_FLY_TARGETING			= 71,
	HEADER_GC_SKILL_LEVEL_OLD			= 72,
	HEADER_GC_SKILL_LEVEL			= 76,

	HEADER_GC_MESSENGER				= 74,
	HEADER_GC_GUILD				= 75,

	HEADER_GC_PARTY_INVITE			= 77,
	HEADER_GC_PARTY_ADD				= 78,
	HEADER_GC_PARTY_UPDATE			= 79,
	HEADER_GC_PARTY_REMOVE			= 80,
	HEADER_GC_QUEST_INFO			= 81,
	HEADER_GC_REQUEST_MAKE_GUILD		= 82,
	HEADER_GC_PARTY_PARAMETER			= 83,

	HEADER_GC_SAFEBOX_SET			= 85,
	HEADER_GC_SAFEBOX_DEL			= 86,
	HEADER_GC_SAFEBOX_WRONG_PASSWORD		= 87,
	HEADER_GC_SAFEBOX_SIZE			= 88,

	HEADER_GC_FISHING				= 89,

	HEADER_GC_EMPIRE				= 90,

	HEADER_GC_PARTY_LINK			= 91,
	HEADER_GC_PARTY_UNLINK			= 92,
#ifdef ENABLE_ITEMSHOP
	HEADER_GC_ITEMSHOP = 93,
#endif

	HEADER_GC_REFINE_INFORMATION_OLD		= 95,

	HEADER_GC_VIEW_EQUIP			= 99,

	HEADER_GC_MARK_BLOCK			= 100,
	HEADER_GC_MARK_IDXLIST			= 102,

	HEADER_GC_TIME					= 106,
	HEADER_GC_CHANGE_NAME			= 107,

	HEADER_GC_DUNGEON				= 110,

	HEADER_GC_WALK_MODE				= 111,
	HEADER_GC_SKILL_GROUP			= 112,
	HEADER_GC_MAIN_CHARACTER			= 113,

	//	HEADER_GC_USE_POTION			= 114,
	HEADER_GC_SEPCIAL_EFFECT		= 114,
//#ifndef USE_MARK_ATLAS_BINARY
	HEADER_GC_NPC_POSITION			= 115,
//#endif
	HEADER_GC_MATRIX_CARD			= 116,
	HEADER_GC_LOGIN_KEY				= 118,
	HEADER_GC_REFINE_INFORMATION		= 119,
	HEADER_GC_CHANNEL				= 121,

	// 122 HEADER_GC_MALL_OPEN
	HEADER_GC_TARGET_UPDATE			= 123,
	HEADER_GC_TARGET_DELETE			= 124,
	HEADER_GC_TARGET_CREATE			= 125,

	HEADER_GC_AFFECT_ADD			= 126,
	HEADER_GC_AFFECT_REMOVE			= 127,

	HEADER_GC_MALL_OPEN				= 122,
	HEADER_GC_MALL_SET				= 128,
	HEADER_GC_MALL_DEL				= 129,

	HEADER_GC_LAND_LIST				= 130,
	HEADER_GC_LOVER_INFO			= 131,
	HEADER_GC_LOVE_POINT_UPDATE			= 132,

	HEADER_GC_SYMBOL_DATA			= 133,

	// MINING
	HEADER_GC_DIG_MOTION			= 134,
	// END_OF_MINING

	HEADER_GC_DAMAGE_INFO           = 135,
	HEADER_GC_CHAR_ADDITIONAL_INFO	= 136,

	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER3_BGM		= 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL	= 138,
	// END_OF_SUPPORT_BGM
#if defined(WJ_COMBAT_ZONE)
	HEADER_GC_RANKING_COMBAT_ZONE		= 139,
	HEADER_GC_SEND_COMBAT_ZONE			= 140,
#endif
#ifdef ENABLE_MANAGER_BANK_SYSTEM
	HEADER_GC_BANK						= 141,
#endif
#ifdef __AURA_SYSTEM__
	HEADER_GC_AURA					= 143,
#endif

	HEADER_GC_AUTH_SUCCESS			= 150,
#ifdef ENABLE_SWITCHBOT
	HEADER_GC_SWITCHBOT						= 171,
#endif
	
#ifdef ENABLE_SWITCHBOT_WORLDARD
	HEADER_GC_SWITCHBOT 					= 171,
#endif

#ifdef ENABLE_NINJEYE
	HEADER_GC_NINJEYE_REQUEST				= 172,
#endif
#ifdef ENABLE_VOICE_CHAT
	HEADER_GC_VOICE_CHAT = 182,
#endif
	// ROULETTE
	HEADER_GC_ROULETTE					= 200,
	// END_ROULETTE
	//
	HEADER_GC_REQUEST_PASSPOD				= 202,
	HEADER_GC_REQUEST_PASSPOD_FAILED		= 203,

	HEADER_GC_SPECIFIC_EFFECT				= 208,

	HEADER_GC_DRAGON_SOUL_REFINE			= 209,
	HEADER_GC_RESPOND_CHANNELSTATUS			= 210,

#if defined(__BL_67_ATTR__)
	HEADER_GC_OPEN_67_ATTR 					= 242,
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
	HEADER_GC_MINI_GAME_CATCH_KING			= 241,
#endif
#if defined(__BL_BATTLE_ROYALE__)
	HEADER_GC_BATTLE_ROYALE					= 244,
#endif
//#ifdef ENABLE_ENTITY_LOADING
	HEADER_GC_ENTITY = 245,
//#endif
#ifdef ENABLE_CALENDAR_SYSTEM
    HEADER_GC_CALENDAR_SYSTEM					= 214,
    HEADER_GC_CALENDAR_SYSTEM_ADM 				= 216,
#endif

    HEADER_GC_CHEAT_BLACKLIST					= 249,
#ifdef ENABLE_DUNGEON_INFO
     HEADER_GC_DUNGEON_INFO_SYSTEM 				= 217,
#endif


#ifdef ENABLE_DROP_ITEM_WORLDARD
    HEADER_GC_DROP_ITEM_SYSTEM 					= 220,
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_GC_CUBE_RENEWAL 						= 221,
#endif
#ifdef BATTLEPASS_WORLDARD
	HEADER_GC_BATTLEPASS						= 230,
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
	HEADER_GC_TOURNAMENT_ADD					= 142,
#endif
#ifdef ENABLE_EXCHANGE_LOG
	HEADER_GC_EXCHANGE_LOG = 235,
#endif
#ifdef __BUFFI_SUPPORT__
	HEADER_GC_BUFFI_SKILL = 240,
#endif
#ifdef ENABLE_IKASHOP_RENEWAL
	HEADER_GC_NEW_OFFLINESHOP				= 222,
#endif
#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
	HEADER_GC_HIT_COUNT_INFO					= 243,
#endif
	/////////////////////////////////////////////////////////////////////////////

	HEADER_GG_LOGIN				= 1,
	HEADER_GG_LOGOUT				= 2,
	HEADER_GG_RELAY				= 3,
	HEADER_GG_NOTICE				= 4,
	HEADER_GG_SHUTDOWN				= 5,
	HEADER_GG_GUILD				= 6,
	HEADER_GG_DISCONNECT			= 7,	// ´©±º°¡ÀÇ Á¢¼ÓÀ» °­Á¦·Î ²÷À» ¶§
	HEADER_GG_SHOUT				= 8,
	HEADER_GG_SETUP				= 9,
	HEADER_GG_MESSENGER_ADD                     = 10,
	HEADER_GG_MESSENGER_REMOVE                  = 11,
	HEADER_GG_FIND_POSITION			= 12,
	HEADER_GG_WARP_CHARACTER			= 13,
	HEADER_GG_MESSENGER_MOBILE			= 14,
	HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX		= 15,
	HEADER_GG_TRANSFER				= 16,
	HEADER_GG_XMAS_WARP_SANTA			= 17,
	HEADER_GG_XMAS_WARP_SANTA_REPLY		= 18,
	HEADER_GG_RELOAD_CRC_LIST			= 19,
	HEADER_GG_LOGIN_PING			= 20,
	HEADER_GG_CHECK_CLIENT_VERSION		= 21,
	HEADER_GG_BLOCK_CHAT			= 22,

	HEADER_GG_BLOCK_EXCEPTION		= 24,
	HEADER_GG_SIEGE					= 25,
	HEADER_GG_MONARCH_NOTICE		= 26,
	HEADER_GG_MONARCH_TRANSFER		= 27,
	HEADER_GG_PCBANG_UPDATE			= 28,

	HEADER_GG_CHECK_AWAKENESS		= 29,
#ifdef ENABLE_FULL_NOTICE
	HEADER_GG_BIG_NOTICE			= 30,
#endif
	//headerggswitch=31
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	HEADER_GG_LOGIN_EX	= 32,
#endif
#ifdef ENABLE_MULTI_FARM_BLOCK
	HEADER_GG_MULTI_FARM = 99,
#endif
#ifdef ENABLE_ULTIMATE_REGEN
	HEADER_GG_NEW_REGEN = 33,
#endif
#ifdef ENABLE_REWARD_SYSTEM
	HEADER_GG_REWARD_INFO = 34,
#endif
	HEADER_GG_DISCONNECT_NAME	= 35,
	HEADER_GG_PLAYER_RANK = 36,
#ifdef ENABLE_MESSENGER_BLOCK
	HEADER_GG_MESSENGER_BLOCK_ADD 				= 37,
	HEADER_GG_MESSENGER_BLOCK_REMOVE 			= 38,
#endif
#ifdef ENABLE_SWITCHBOT
	HEADER_GG_SWITCHBOT					= 39,
#endif
#if defined(__BL_BATTLE_ROYALE__)
	HEADER_GG_BATTLE_ROYALE			= 40,
#endif
	HEADER_GG_PLAYER_RANK_RELOAD = 41,
};

#pragma pack(1)
typedef struct SPacketGGSetup
{
	BYTE	bHeader;
	WORD	wPort;
	BYTE	bChannel;
} TPacketGGSetup;
#if defined(WJ_COMBAT_ZONE)
typedef struct SPacketCGCombatZoneAction
{
	BYTE	header;
	int		action;
	int		value;
} TPacketCGCombatZoneRequestAction;

typedef struct SPacketGCCombatZone
{
	int		rank;
	char	name[12 + 1];
	int		empire;
	int		points;
} TPacketGCCombatZoneRanking;

typedef struct SPacketGCCombatZoneData
{
	BYTE header;
	TPacketGCCombatZoneRanking rankingData[11];
} TPacketGCCombatZoneRankingData;

typedef struct SPacketGCSendCombatZone
{
	BYTE	header;
	DWORD	sub_header;
	DWORD	m_pInfoData[4];
	DWORD	m_pDataDays[7][8];
	bool	m_pDataDaysCheck[7];
	bool	isRunning;
} TPacketGCSendCombatZone;
#endif
typedef struct SPacketGGDisconnectName
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGDisconnectName;

typedef struct SPacketGGLogin
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	dwPID;
	BYTE	bEmpire;
	long	lMapIndex;
	BYTE	bChannel;
} TPacketGGLogin;

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
typedef struct SPacketGGLoginEx
{
	BYTE	bHeader;
	DWORD	dwPID;
	BYTE	bLanguage;
} TPacketGGLoginEx;
#endif

typedef struct SPacketGGLogout
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGLogout;

typedef struct SPacketGGRelay
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lSize;
} TPacketGGRelay;

typedef struct SPacketGGNotice
{
	BYTE	bHeader;
	long	lSize;
} TPacketGGNotice;

typedef struct SPacketGGMonarchNotice
{
	BYTE	bHeader;
	BYTE	bEmpire;
	long	lSize;
} TPacketGGMonarchNotice;

//FORKED_ROAD
typedef struct SPacketGGForkedMapInfo
{
	BYTE	bHeader;
	BYTE	bPass;
	BYTE	bSungzi;
} TPacketGGForkedMapInfo;
//END_FORKED_ROAD
typedef struct SPacketGGShutdown
{
	BYTE	bHeader;
} TPacketGGShutdown;

typedef struct SPacketGGGuild
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	DWORD	dwGuild;
} TPacketGGGuild;

enum
{
	GUILD_SUBHEADER_GG_CHAT,
	GUILD_SUBHEADER_GG_SET_MEMBER_COUNT_BONUS,
};

typedef struct SPacketGGGuildChat
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	DWORD	dwGuild;
	char	szText[CHAT_MAX_LEN + 1];
} TPacketGGGuildChat;

typedef struct SPacketGGParty
{
	BYTE	header;
	BYTE	subheader;
	DWORD	pid;
	DWORD	leaderpid;
} TPacketGGParty;

enum
{
	PARTY_SUBHEADER_GG_CREATE,
	PARTY_SUBHEADER_GG_DESTROY,
	PARTY_SUBHEADER_GG_JOIN,
	PARTY_SUBHEADER_GG_QUIT,
};

#ifdef ENABLE_SWITCHBOT
struct TPacketGGSwitchbot
{
	BYTE bHeader;
	WORD wPort;
	TSwitchbotTable table;

	TPacketGGSwitchbot() : bHeader(HEADER_GG_SWITCHBOT), wPort(0)
	{
		table = {};
	}
};

enum ECGSwitchbotSubheader
{
	SUBHEADER_CG_SWITCHBOT_START,
	SUBHEADER_CG_SWITCHBOT_STOP,
};

struct TPacketCGSwitchbot
{
	BYTE header;
	int size;
	BYTE subheader;
	BYTE slot;
};

enum EGCSwitchbotSubheader
{
	SUBHEADER_GC_SWITCHBOT_UPDATE,
	SUBHEADER_GC_SWITCHBOT_UPDATE_ITEM,
	SUBHEADER_GC_SWITCHBOT_SEND_ATTRIBUTE_INFORMATION,
};

struct TPacketGCSwitchbot
{
	BYTE header;
	int size;
	BYTE subheader;
	BYTE slot;
};

struct TSwitchbotUpdateItem
{
	BYTE	slot;
	BYTE	vnum;
	BYTE	count;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	WORD	CellInven;
#endif
};
#endif


typedef struct SPacketGGDisconnect
{
	BYTE	bHeader;
	char	szLogin[LOGIN_MAX_LEN + 1];
} TPacketGGDisconnect;

typedef struct SPacketGGShout
{
	BYTE	bHeader;
	BYTE	bEmpire;
	char	szText[CHAT_MAX_LEN + 1];
} TPacketGGShout;

typedef struct SPacketGGXmasWarpSanta
{
	BYTE	bHeader;
	BYTE	bChannel;
	long	lMapIndex;
} TPacketGGXmasWarpSanta;

typedef struct SPacketGGXmasWarpSantaReply
{
	BYTE	bHeader;
	BYTE	bChannel;
} TPacketGGXmasWarpSantaReply;

typedef struct SMessengerData
{
	char        szMobile[MOBILE_MAX_LEN + 1];
} TMessengerData;

typedef struct SPacketGGMessenger
{
	BYTE        bHeader;
	char        szAccount[CHARACTER_NAME_MAX_LEN + 1];
	char        szCompanion[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGMessenger;

typedef struct SPacketGGMessengerMobile
{
	BYTE        bHeader;
	char        szName[CHARACTER_NAME_MAX_LEN + 1];
	char        szMobile[MOBILE_MAX_LEN + 1];
} TPacketGGMessengerMobile;

typedef struct SPacketGGFindPosition
{
	BYTE header;
	DWORD dwFromPID; // Àú À§Ä¡·Î ¿öÇÁÇÏ·Á´Â »ç¶÷
	DWORD dwTargetPID; // Ã£´Â »ç¶÷
} TPacketGGFindPosition;

typedef struct SPacketGGWarpCharacter
{
	BYTE header;
	DWORD pid;
	long x;
	long y;
#ifdef __RENEWAL_WARP__
	DWORD mapIndex;
	WORD port;
#endif
} TPacketGGWarpCharacter;

//  HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX	    = 15,

typedef struct SPacketGGGuildWarMapIndex
{
	BYTE bHeader;
	DWORD dwGuildID1;
	DWORD dwGuildID2;
	long lMapIndex;
} TPacketGGGuildWarMapIndex;

typedef struct SPacketGGTransfer
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lX, lY;
#ifdef __RENEWAL_WARP__
	DWORD mapIndex;
	WORD port;
#endif

} TPacketGGTransfer;

typedef struct SPacketGGLoginPing
{
	BYTE	bHeader;
	char	szLogin[LOGIN_MAX_LEN + 1];
} TPacketGGLoginPing;

typedef struct SPacketGGBlockChat
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lBlockDuration;
} TPacketGGBlockChat;

/* Å¬¶óÀÌ¾ðÆ® Ãø¿¡¼­ º¸³»´Â ÆÐÅ¶ */

typedef struct command_text
{
	BYTE	bHeader;
} TPacketCGText;

/* ·Î±×ÀÎ (1) */
typedef struct command_handshake
{
	BYTE	bHeader;
	DWORD	dwHandshake;
	DWORD	dwTime;
	long	lDelta;
} TPacketCGHandshake;

typedef struct command_login
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
} TPacketCGLogin;

typedef struct command_login2
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	DWORD	dwLoginKey;
	DWORD	adwClientKey[4];
} TPacketCGLogin2;

typedef struct command_login3
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
	DWORD	adwClientKey[4];
#ifdef ENABLE_VERSION_SYSTEM
	WORD	versionKey;
#endif
#ifdef RENEWAL_HWID_BLOCK
	char	hwid_router[18+1];
#endif
} TPacketCGLogin3;

typedef struct command_matrix_card
{
	BYTE	bHeader;
	char	szAnswer[MATRIX_ANSWER_MAX_LEN + 1];
} TPacketCGMatrixCard;
#ifdef __NEWPET_SYSTEM__
typedef struct packet_RequestPetName
{
	BYTE byHeader;
	char petname[13];

}TPacketCGRequestPetName;
#endif
typedef struct packet_matrix_card
{
	BYTE	bHeader;
	DWORD	dwRows;
	DWORD	dwCols;
} TPacketGCMatrixCard;

typedef struct packet_login_key
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
} TPacketGCLoginKey;

typedef struct command_player_select
{
	BYTE	header;
	BYTE	index;
} TPacketCGPlayerSelect;

typedef struct command_player_delete
{
	BYTE	header;
	BYTE	index;
	char	private_code[8];
} TPacketCGPlayerDelete;

typedef struct command_player_hair_change
{
	BYTE        header;
	BYTE        index;
	int			hairIndex;
} TPacketCGChangeCharacterHair;

typedef struct command_player_create
{
	BYTE        header;
	BYTE        index;
	char        name[CHARACTER_NAME_MAX_LEN + 1];
	WORD        job;
	BYTE	shape;
	BYTE	Con;
	BYTE	Int;
	BYTE	Str;
	BYTE	Dex;
} TPacketCGPlayerCreate;

typedef struct command_player_create_success
{
	BYTE		header;
	BYTE		bAccountCharacterIndex;
	TSimplePlayer	player;
} TPacketGCPlayerCreateSuccess;

// °ø°Ý
typedef struct command_attack
{
	BYTE	bHeader;
	BYTE	bType;
	DWORD	dwVID;
	BYTE	bCRCMagicCubeProcPiece;
	BYTE	bCRCMagicCubeFilePiece;
} TPacketCGAttack;

enum EMoveFuncType
{
	FUNC_WAIT,
	FUNC_MOVE,
	FUNC_ATTACK,
	FUNC_COMBO,
	FUNC_MOB_SKILL,
	_FUNC_SKILL,
	FUNC_MAX_NUM,
	FUNC_SKILL = 0x80,
};

// ÀÌµ¿
typedef struct command_move
{
	BYTE	bHeader;
	BYTE	bFunc;
	BYTE	bArg;
	BYTE	bRot;
	long	lX;
	long	lY;
	DWORD	dwTime;
} TPacketCGMove;

typedef struct command_sync_position_element
{
	DWORD	dwVID;
	long	lX;
	long	lY;
} TPacketCGSyncPositionElement;

// À§Ä¡ µ¿±âÈ­
typedef struct command_sync_position	// °¡º¯ ÆÐÅ¶
{
	BYTE	bHeader;
	WORD	wSize;
} TPacketCGSyncPosition;

/* Ã¤ÆÃ (3) */
typedef struct command_chat	// °¡º¯ ÆÐÅ¶
{
	BYTE	header;
	WORD	size;
	BYTE	type;
} TPacketCGChat;

/* ±Ó¼Ó¸» */
typedef struct command_whisper
{
	BYTE	bHeader;
	WORD	wSize;
	char 	szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisper;

typedef struct command_entergame
{
	BYTE	header;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	BYTE	lang;
#endif
} TPacketCGEnterGame;

typedef struct command_item_use
{
	BYTE 	header;
	TItemPos 	Cell;
} TPacketCGItemUse;

typedef struct command_item_use_to_item
{
	BYTE	header;
	TItemPos	Cell;
	TItemPos	TargetCell;
} TPacketCGItemUseToItem;

typedef struct command_item_drop
{
	BYTE 	header;
	TItemPos 	Cell;
	DWORD	gold;
} TPacketCGItemDrop;

typedef struct command_item_drop2
{
	BYTE 	header;
	TItemPos 	Cell;
	DWORD	gold;
	DWORD	count;
} TPacketCGItemDrop2;

typedef struct command_item_move
{
	BYTE 	header;
	TItemPos	Cell;
	TItemPos	CellTo;
	DWORD	count;
} TPacketCGItemMove;

#ifdef ENABLE_SELL_ITEM
typedef struct command_item_sell
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		gold;
} TPacketCGItemSell;
#endif

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
typedef struct command_items_pickup
{
	uint8_t header;
	DWORD count;
} TPacketCGItemsPickUp;
#endif
typedef struct command_item_pickup
{
	BYTE 	header;
	DWORD	vid;
} TPacketCGItemPickup;

typedef struct command_quickslot_add
{
	BYTE	header;
	BYTE	pos;
	TQuickslot	slot;
} TPacketCGQuickslotAdd;

typedef struct command_quickslot_del
{
	BYTE	header;
	BYTE	pos;
} TPacketCGQuickslotDel;

typedef struct command_quickslot_swap
{
	BYTE	header;
	BYTE	pos;
	BYTE	change_pos;
} TPacketCGQuickslotSwap;

enum
{
	SHOP_SUBHEADER_CG_END,
	SHOP_SUBHEADER_CG_BUY,
	SHOP_SUBHEADER_CG_SELL,
	SHOP_SUBHEADER_CG_SELL2,
};

typedef struct command_shop_buy
{
	BYTE	count;
} TPacketCGShopBuy;//NEED LOK non use

typedef struct command_shop_sell
{
	BYTE	pos;
	BYTE	count;
} TPacketCGShopSell;//NEED LOK non use

typedef struct command_shop
{
	BYTE	header;
	BYTE	subheader;
} TPacketCGShop;

typedef struct command_on_click
{
	BYTE	header;
	DWORD	vid;
} TPacketCGOnClick;

typedef struct command_exchange
{
	BYTE	header;
	BYTE	sub_header;
	long long	arg1;
	BYTE	arg2;
	TItemPos	Pos;
} TPacketCGExchange;

typedef struct command_position
{
	BYTE	header;
	BYTE	position;
} TPacketCGPosition;

typedef struct command_script_answer
{
	BYTE	header;
	BYTE	answer;
	//char	file[32 + 1];
	//BYTE	answer[16 + 1];
} TPacketCGScriptAnswer;


typedef struct command_script_button
{
	BYTE        header;
	unsigned int	idx;
} TPacketCGScriptButton;

typedef struct command_quest_input_string
{
	BYTE header;
	char msg[64+1];
} TPacketCGQuestInputString;

typedef struct command_quest_input_long_string
{
	BYTE header;
	char szMsg[512];
} TPacketCGQuestInputLongString;

typedef struct command_quest_confirm
{
	BYTE header;
	BYTE answer;
	DWORD requestPID;
} TPacketCGQuestConfirm;

/*
 * ¼­¹ö Ãø¿¡¼­ º¸³»´Â ÆÐÅ¶
 */
typedef struct packet_quest_confirm
{
	BYTE header;
	char msg[64+1];
	long timeout;
	DWORD requestPID;
} TPacketGCQuestConfirm;

typedef struct packet_handshake
{
	BYTE	bHeader;
	DWORD	dwHandshake;
	DWORD	dwTime;
	long	lDelta;
} TPacketGCHandshake;

enum EPhase
{
	PHASE_CLOSE,
	PHASE_HANDSHAKE,
	PHASE_LOGIN,
	PHASE_SELECT,
	PHASE_LOADING,
	PHASE_GAME,
	PHASE_DEAD,

	PHASE_CLIENT_CONNECTING,
	PHASE_DBCLIENT,
	PHASE_P2P,
	PHASE_AUTH,
	PHASE_TEEN,
	PHASE_PASSPOD,
};

typedef struct packet_phase
{
	BYTE	header;
	BYTE	phase;
} TPacketGCPhase;

typedef struct packet_bindudp
{
	BYTE	header;
	DWORD	addr;
	WORD	port;
} TPacketGCBindUDP;

enum
{
	LOGIN_FAILURE_ALREADY	= 1,
	LOGIN_FAILURE_ID_NOT_EXIST	= 2,
	LOGIN_FAILURE_WRONG_PASS	= 3,
	LOGIN_FAILURE_FALSE		= 4,
	LOGIN_FAILURE_NOT_TESTOR	= 5,
	LOGIN_FAILURE_NOT_TEST_TIME	= 6,
	LOGIN_FAILURE_FULL		= 7
};

typedef struct packet_login_success
{
	BYTE		bHeader;
	TSimplePlayer	players[PLAYER_PER_ACCOUNT];
	DWORD		guild_id[PLAYER_PER_ACCOUNT];
	char		guild_name[PLAYER_PER_ACCOUNT][GUILD_NAME_MAX_LEN+1];

	DWORD		handle;
	DWORD		random_key;
} TPacketGCLoginSuccess;

typedef struct packet_auth_success
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
	BYTE	bResult;
} TPacketGCAuthSuccess;

typedef struct packet_login_failure
{
	BYTE	header;
	char	szStatus[ACCOUNT_STATUS_MAX_LEN + 1];
} TPacketGCLoginFailure;

typedef struct packet_create_failure
{
	BYTE	header;
	BYTE	bType;
} TPacketGCCreateFailure;

enum
{
	ADD_CHARACTER_STATE_DEAD		= (1 << 0),
	ADD_CHARACTER_STATE_SPAWN		= (1 << 1),
	ADD_CHARACTER_STATE_GUNGON		= (1 << 2),
	ADD_CHARACTER_STATE_KILLER		= (1 << 3),
	ADD_CHARACTER_STATE_PARTY		= (1 << 4),
};

enum ECharacterEquipmentPart
{
	CHR_EQUIPPART_ARMOR,
	CHR_EQUIPPART_WEAPON,
	CHR_EQUIPPART_HEAD,
	CHR_EQUIPPART_HAIR,
	#ifdef __SASH_SYSTEM__
	CHR_EQUIPPART_SASH,
	#endif
	CHR_PART_ARROW_TYPE,
#ifdef __AURA_SYSTEM__
	CHR_EQUIPPART_AURA,
#endif

	CHR_EQUIPPART_NUM,
};

#ifdef __ITEM_SHINING__
enum EShiningParts
{
	CHR_SHINING_WEAPON_1,
	// CHR_SHINING_WEAPON_2,
	// CHR_SHINING_WEAPON_3,
	CHR_SHINING_ARMOR_1,
	// CHR_SHINING_ARMOR_2,
	// CHR_SHINING_ARMOR_3,
	CHR_SHINING_SPECIAL,
	CHR_SHINING_NUM,
};
#endif


typedef struct packet_add_char
{
	BYTE	header;
	DWORD	dwVID;

	float	angle;
	long	x;
	long	y;
	long	z;

	BYTE	bType;
	WORD	wRaceNum;
	WORD	bMovingSpeed;
	BYTE	bAttackSpeed;

	BYTE	bStateFlag;
	DWORD	dwAffectFlag[3];
} TPacketGCCharacterAdd;

typedef struct packet_char_additional_info
{
	BYTE    header;
	DWORD   dwVID;
	char    name[CHARACTER_NAME_MAX_LEN + 1];
	uint32_t    awPart[CHR_EQUIPPART_NUM];

	BYTE	bEmpire;
	DWORD   dwGuildID;
	DWORD   dwLevel;
#ifdef ENABLE_YOHARA_SYSTEM
	DWORD	dwConquerorLevel;
#endif
	short	sAlignment;
	BYTE	bPKMode;
	DWORD	dwMountVnum;
#if defined(WJ_COMBAT_ZONE)
	BYTE	combat_zone_rank;
	DWORD	combat_zone_points;
#endif
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	BYTE	language;
#endif
#ifdef __ITEM_SHINING__
	DWORD	adwShining[CHR_SHINING_NUM];
#endif
} TPacketGCCharacterAdditionalInfo;

/*
   typedef struct packet_update_char_old
   {
   BYTE	header;
   DWORD	dwVID;

   uint32_t        awPart[CHR_EQUIPPART_NUM];
   BYTE	bMovingSpeed;
   BYTE	bAttackSpeed;

   BYTE	bStateFlag;
   DWORD	dwAffectFlag[3];

   DWORD	dwGuildID;
   short	sAlignment;
   BYTE	bPKMode;
   DWORD	dwMountVnum;
   } TPacketGCCharacterUpdateOld;
 */

typedef struct packet_update_char
{
	BYTE	header;
	DWORD	dwVID;

	uint32_t        awPart[CHR_EQUIPPART_NUM];

	WORD	bMovingSpeed;
	BYTE	bAttackSpeed;

	BYTE	bStateFlag;
	DWORD	dwAffectFlag[3];

	DWORD	dwGuildID;
	short	sAlignment;
#ifdef __NEWPET_SYSTEM__
	DWORD   dwLevel;
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	DWORD	dwConquerorLevel;
#endif
	BYTE	bPKMode;
	DWORD	dwMountVnum;
#if defined(WJ_COMBAT_ZONE)
	BYTE	combat_zone_rank;
	DWORD	combat_zone_points;
#endif
	//WORD	wRaceNum;
#ifdef __ITEM_SHINING__
	DWORD	adwShining[CHR_SHINING_NUM];
#endif
} TPacketGCCharacterUpdate;

#ifdef ENABLE_ENTITY_LOADING
using TPacketGCEntity = struct SPacketGCEntity
{
	BYTE bHeader;
	WORD wSize;
};

using TPacketEntityInfo = struct SPacketEntityInfo
{
	DWORD dwVID;
	DWORD dwRaceVNum;
	DWORD wPart[CHR_EQUIPPART_NUM];
	long xPos, yPos;
};
#endif

#ifdef ENABLE_SUPPORT_SYSTEM
typedef struct support_use_skill
{
	BYTE	bHeader;
	DWORD	dwVnum;
	DWORD	dwVid;
	DWORD	dwLevel;
}TPacketGCSupportUseSkill;
#endif

typedef struct packet_del_char
{
	BYTE	header;
	DWORD	id;
} TPacketGCCharacterDelete;

typedef struct packet_chat	// °¡º¯ ÆÐÅ¶
{
	BYTE	header;
	WORD	size;
	BYTE	type;
	DWORD	id;
	BYTE	bEmpire;
} TPacketGCChat;

typedef struct packet_whisper	// °¡º¯ ÆÐÅ¶
{
	BYTE	bHeader;
	WORD	wSize;
	BYTE	bType;
	char	szNameFrom[CHARACTER_NAME_MAX_LEN + 1];
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
    BYTE	bLanguage;
    BYTE	bEmpire;
#endif
} TPacketGCWhisper;

typedef struct packet_main_character
{
	BYTE        header;
	DWORD	dwVID;
	WORD	wRaceNum;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter;

// SUPPORT_BGM
typedef struct packet_main_character3_bgm
{
	enum
	{
		MUSIC_NAME_LEN = 24,
	};

	BYTE    header;
	DWORD	dwVID;
	WORD	wRaceNum;
	char	szChrName[CHARACTER_NAME_MAX_LEN + 1];
	char	szBGMName[MUSIC_NAME_LEN + 1];
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter3_BGM;

typedef struct packet_main_character4_bgm_vol
{
	enum
	{
		MUSIC_NAME_LEN = 24,
	};

	BYTE    header;
	DWORD	dwVID;
	WORD	wRaceNum;
	char	szChrName[CHARACTER_NAME_MAX_LEN + 1];
	char	szBGMName[MUSIC_NAME_LEN + 1];
	float	fBGMVol;
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter4_BGM_VOL;
// END_OF_SUPPORT_BGM

typedef struct packet_points
{
	BYTE	header;
	long long	points[POINT_MAX_NUM];

} TPacketGCPoints;

typedef struct packet_skill_level
{
	BYTE		bHeader;
	TPlayerSkill	skills[SKILL_MAX_NUM];
} TPacketGCSkillLevel;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
typedef struct SPacketCGRequestFlag
{
	BYTE	header;
	char	name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGRequestFlag;
#endif

typedef struct packet_point_change
{
	BYTE		header;
	DWORD		dwVID;
	BYTE		type;
	long long	amount;
	long long	value;
} TPacketGCPointChange;

typedef struct packet_stun
{
	BYTE	header;
	DWORD	vid;
} TPacketGCStun;

#ifdef RENEWAL_DEAD_PACKET
enum EReviveTypes
{
	REVIVE_TYPE_HERE,
	REVIVE_TYPE_TOWN,
	REVIVE_TYPE_AUTO_TOWN,
	REVIVE_TYPE_MAX
};
#endif
typedef struct packet_dead
{
#ifdef RENEWAL_DEAD_PACKET
	packet_dead() {	memset(&t_d, 0, sizeof(t_d)); }
#endif
	BYTE	header;
	DWORD	vid;
#ifdef RENEWAL_DEAD_PACKET
	BYTE	t_d[REVIVE_TYPE_MAX];
#endif
} TPacketGCDead;

struct TPacketGCItemDelDeprecated
{
	BYTE	header;
	TItemPos Cell;
	DWORD	vnum;
	DWORD	count;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
#ifdef __SOULBINDING_SYSTEM__
	long		bind;
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char		name[ITEM_NAME_MAX_LEN+1];
#endif
};

typedef struct packet_item_set
{
	BYTE	header;
	TItemPos Cell;
	DWORD	vnum;
	DWORD	count;
	DWORD	flags;
	DWORD	anti_flags;
	bool	highlight;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
#ifdef __SOULBINDING_SYSTEM__
	long		bind;
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char		name[ITEM_NAME_MAX_LEN+1];
#endif
} TPacketGCItemSet;

#ifdef __SASH_SYSTEM__
enum
{
	HEADER_CG_SASH = 211,
	HEADER_GC_SASH = 215,
	SASH_SUBHEADER_GC_OPEN = 0,
	SASH_SUBHEADER_GC_CLOSE,
	SASH_SUBHEADER_GC_ADDED,
	SASH_SUBHEADER_GC_REMOVED,
	SASH_SUBHEADER_CG_REFINED,
	SASH_SUBHEADER_CG_CLOSE = 0,
	SASH_SUBHEADER_CG_ADD,
	SASH_SUBHEADER_CG_REMOVE,
	SASH_SUBHEADER_CG_REFINE,
};

typedef struct SPacketSash
{
	BYTE	header;
	BYTE	subheader;
	bool	bWindow;
	DWORD	dwPrice;
	BYTE	bPos;
	TItemPos	tPos;
	DWORD	dwItemVnum;
	DWORD	dwMinAbs;
	DWORD	dwMaxAbs;
} TPacketSash;
#endif

typedef struct packet_item_del
{
	BYTE	header;
	BYTE	pos;
} TPacketGCItemDel;

struct packet_item_use
{
	BYTE	header;
	TItemPos Cell;
	DWORD	ch_vid;
	DWORD	victim_vid;
	DWORD	vnum;
};

struct packet_item_move
{
	BYTE	header;
	TItemPos Cell;
	TItemPos CellTo;
};
#ifdef ENABLE_MANAGER_BANK_SYSTEM
typedef struct packet_bankmanager
{
	int user_action;
	long long user_money;
	char user_datetime[19 + 1];
	char user_ip[29 + 1];
	char user_recvmoney[16 + 1];
} TAccountBank;

typedef struct packet_bankmanager_logs
{
	BYTE header;
	TAccountBank logs[1000];
} TPacketCGBank;
#endif

typedef struct packet_item_update
{
	BYTE	header;
	TItemPos Cell;
	DWORD	count;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
#ifdef __SOULBINDING_SYSTEM__
	long		bind;
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char		name[ITEM_NAME_MAX_LEN+1];
#endif
} TPacketGCItemUpdate;

#ifdef USE_EXPANDED_ITEM_REFRESH
typedef struct SPacketGCItemUpdateCount
{
    BYTE        header;
    TItemPos    pos;
    DWORD       count;
} TPacketGCItemUpdateCount;
#endif

typedef struct packet_item_ground_add
{
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	packet_item_ground_add()
	{
		memset(&alSockets, 0, sizeof(alSockets));
		memset(&aAttrs, 0, sizeof(aAttrs));
	}
#endif

	BYTE	bHeader;
	long 	x, y, z;
	DWORD	dwVID;
	DWORD	dwVnum;
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	long	alSockets[ITEM_SOCKET_MAX_NUM];//3
	TPlayerItemAttribute aAttrs[ITEM_ATTRIBUTE_MAX_NUM];//7
	unsigned short itemCount;
	int		ownerTime;
#endif
	
} TPacketGCItemGroundAdd;

typedef struct packet_item_ownership
{
	BYTE	bHeader;
	DWORD	dwVID;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCItemOwnership;

typedef struct packet_item_ground_del
{
	BYTE	bHeader;
	DWORD	dwVID;
} TPacketGCItemGroundDel;

struct packet_quickslot_add
{
	BYTE	header;
	BYTE	pos;
	TQuickslot	slot;
};

struct packet_quickslot_del
{
	BYTE	header;
	BYTE	pos;
};

struct packet_quickslot_swap
{
	BYTE	header;
	BYTE	pos;
	BYTE	pos_to;
};

struct packet_motion
{
	BYTE	header;
	DWORD	vid;
	DWORD	victim_vid;
	WORD	motion;
};

enum EPacketShopSubHeaders
{
	SHOP_SUBHEADER_GC_START,
	SHOP_SUBHEADER_GC_END,
	SHOP_SUBHEADER_GC_UPDATE_ITEM,
	SHOP_SUBHEADER_GC_UPDATE_PRICE,
	SHOP_SUBHEADER_GC_OK,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY,
	SHOP_SUBHEADER_GC_SOLDOUT,
	SHOP_SUBHEADER_GC_INVENTORY_FULL,
	SHOP_SUBHEADER_GC_INVALID_POS,
	SHOP_SUBHEADER_GC_SOLD_OUT,
	SHOP_SUBHEADER_GC_START_EX,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX,
#if defined(WJ_COMBAT_ZONE)
	SHOP_SUBHEADER_GC_NOT_ENOUGH_POINTS,
	SHOP_SUBHEADER_GC_MAX_LIMIT_POINTS,
	SHOP_SUBHEADER_GC_OVERFLOW_LIMIT_POINTS,
#endif
#ifdef ENABLE_ZODIAC_MISSION
	SHOP_SUBHEADER_GC_NOT_ENOUGH_LUCKY,
#endif
};

struct packet_shop_item
{   
	DWORD       vnum;
#ifdef FULL_YANG
	long long	price;
#else
	DWORD       price;
#endif
    DWORD        count;
#ifdef ENABLE_BUY_WITH_ITEM
	TShopItemPrice	itemprice[MAX_SHOP_PRICES];
#endif
	BYTE		display_pos;

	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
};

typedef struct packet_shop_start
{
	DWORD   owner_vid;
	struct packet_shop_item	items[SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCShopStart;

typedef struct packet_shop_start_ex // ´ÙÀ½¿¡ TSubPacketShopTab* shop_tabs ÀÌ µû¶ó¿È.
{
	typedef struct sub_packet_shop_tab
	{
		char name[SHOP_TAB_NAME_MAX];
		BYTE coin_type;
		packet_shop_item items[SHOP_HOST_ITEM_MAX_NUM];
	} TSubPacketShopTab;
	DWORD owner_vid;
	BYTE shop_tab_count;
#if defined(WJ_COMBAT_ZONE)
	DWORD points;
	DWORD curLimit;
	DWORD maxLimit;
#endif
} TPacketGCShopStartEx;

typedef struct packet_shop_update_item
{
	BYTE			pos;
	struct packet_shop_item	item;
} TPacketGCShopUpdateItem;

typedef struct packet_shop_update_price
{
	long long	iPrice;

} TPacketGCShopUpdatePrice;

typedef struct packet_shop	// °¡º¯ ÆÐÅ¶
{
	BYTE        header;
	WORD	size;
	BYTE        subheader;
} TPacketGCShop;

typedef struct SPacketExchange
{
	BYTE					header;
	BYTE					sub_header;
	BYTE					is_me;
	long long				arg1;
	TItemPos				arg2;
	DWORD					arg3;
#ifdef WJ_ENABLE_TRADABLE_ICON
	TItemPos				arg4;
#endif
	long					alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute	aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom	aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char					name[ITEM_NAME_MAX_LEN+1];
#endif
} TPacketExchange;

struct packet_position
{
	BYTE	header;
	DWORD	vid;
	BYTE	position;
};

typedef struct packet_ping
{
	BYTE	header;
} TPacketGCPing;

struct packet_script
{
	BYTE	header;
	WORD	size;
	BYTE	skin;
	WORD	src_size;
#ifdef ENABLE_QUEST_CATEGORY
	BYTE	quest_flag;
#endif
};

typedef struct packet_change_speed
{
	BYTE		header;
	DWORD		vid;
	WORD		moving_speed;
} TPacketGCChangeSpeed;

struct packet_mount
{
	BYTE	header;
	DWORD	vid;
	DWORD	mount_vid;
	BYTE	pos;
	DWORD	x, y;
};

typedef struct packet_move
{
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	DWORD		dwVID;
	long		lX;
	long		lY;
	DWORD		dwTime;
	DWORD		dwDuration;
} TPacketGCMove;

// ¼ÒÀ¯±Ç
typedef struct packet_ownership
{
	BYTE		bHeader;
	DWORD		dwOwnerVID;
	DWORD		dwVictimVID;
} TPacketGCOwnership;

// À§Ä¡ µ¿±âÈ­ ÆÐÅ¶ÀÇ bCount ¸¸Å­ ºÙ´Â ´ÜÀ§
typedef struct packet_sync_position_element
{
	DWORD	dwVID;
	long	lX;
	long	lY;
} TPacketGCSyncPositionElement;

// À§Ä¡ µ¿±âÈ­
typedef struct packet_sync_position	// °¡º¯ ÆÐÅ¶
{
	BYTE	bHeader;
	WORD	wSize;	// °³¼ö = (wSize - sizeof(TPacketGCSyncPosition)) / sizeof(TPacketGCSyncPositionElement)
} TPacketGCSyncPosition;

typedef struct packet_fly
{
	BYTE	bHeader;
	BYTE	bType;
	DWORD	dwStartVID;
	DWORD	dwEndVID;
} TPacketGCCreateFly;

typedef struct command_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwTargetVID;
	long		x, y;
} TPacketCGFlyTargeting;

typedef struct packet_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwShooterVID;
	DWORD		dwTargetVID;
	long		x, y;
} TPacketGCFlyTargeting;

typedef struct packet_shoot
{
	BYTE		bHeader;
	BYTE		bType;
} TPacketCGShoot;

typedef struct packet_duel_start
{
	BYTE	header;
	WORD	wSize;	// DWORD°¡ ¸î°³? °³¼ö = (wSize - sizeof(TPacketGCPVPList)) / 4
} TPacketGCDuelStart;

enum EPVPModes
{
	PVP_MODE_NONE,
	PVP_MODE_AGREE,
	PVP_MODE_FIGHT,
	PVP_MODE_REVENGE
};

typedef struct packet_pvp
{
	BYTE        bHeader;
	DWORD       dwVIDSrc;
	DWORD       dwVIDDst;
	BYTE        bMode;	// 0 ÀÌ¸é ²û, 1ÀÌ¸é ÄÔ
} TPacketGCPVP;

typedef struct command_use_skill
{
	BYTE	bHeader;
	DWORD	dwVnum;
	DWORD	dwVID;
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
	bool	bFromAutoHunt;
#endif
} TPacketCGUseSkill;

typedef struct command_target
{
	BYTE	header;
	DWORD	dwVID;
} TPacketCGTarget;

typedef struct packet_target
{
	BYTE	header;
	DWORD	dwVID;
	BYTE	bHPPercent;
#ifdef ENABLE_HIDE_COSTUME_SYSTEM
    char		dwCostumeHideID[255 + 1];
#endif
#ifdef __VIEW_TARGET_DECIMAL_HP__
	int		iMinHP;
	int		iMaxHP;
#endif
#ifdef ELEMENT_TARGET
	BYTE	bElement;
#endif
} TPacketGCTarget;

#ifdef __SEND_TARGET_INFO__
struct STargetItemInfo
{
	DWORD	vnum;
	DWORD	count;
};

typedef struct SPacketGCTargetInfo
{
	BYTE	header;
#ifdef ENABLE_LARGE_DYNAMIC_PACKETS
	int		size;
#else
	WORD	size;
#endif
	WORD	race;
	DWORD	vid;
	int		count;
} TPacketGCTargetInfo;

typedef struct SPacketCGTargetInfoLoad
{
	BYTE	header;
	DWORD	vid;
} TPacketCGTargetInfoLoad;
#endif

typedef struct packet_warp
{
	BYTE	bHeader;
	long	lX;
	long	lY;
	long	lAddr;
	WORD	wPort;
} TPacketGCWarp;

typedef struct command_warp
{
	BYTE	bHeader;
} TPacketCGWarp;

typedef struct SPacketGCQuestInfo
{
	BYTE header;
#ifdef ENABLE_LARGE_DYNAMIC_PACKETS
	int size;
#else
	WORD size;
#endif
	WORD index;
	BYTE flag;
#ifdef __QUEST_RENEWAL__
	BYTE type;
	bool is_confirmed;
#endif
} TPacketGCQuestInfo;

enum
{
	MESSENGER_SUBHEADER_GC_LIST,
	MESSENGER_SUBHEADER_GC_LOGIN,
	MESSENGER_SUBHEADER_GC_LOGOUT,
	MESSENGER_SUBHEADER_GC_INVITE,
	MESSENGER_SUBHEADER_GC_MOBILE,
#ifdef ENABLE_MESSENGER_BLOCK
	MESSENGER_SUBHEADER_GC_BLOCK_LIST,
	MESSENGER_SUBHEADER_GC_BLOCK_LOGIN,
	MESSENGER_SUBHEADER_GC_BLOCK_LOGOUT,
	MESSENGER_SUBHEADER_GC_REMOVE_BLOCK,
#endif
};

typedef struct packet_messenger
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCMessenger;

typedef struct packet_messenger_guild_list
{
	BYTE connected;
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildList;

typedef struct packet_messenger_guild_login
{
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildLogin;

#ifdef ENABLE_MESSENGER_BLOCK
typedef struct packet_messenger_block_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerBlockListOffline;

typedef struct packet_messenger_block_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerBlockListOnline;
#endif

typedef struct packet_messenger_guild_logout
{
	BYTE length;

	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildLogout;

typedef struct packet_messenger_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerListOffline;

typedef struct packet_messenger_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerListOnline;

enum
{
	MESSENGER_SUBHEADER_CG_ADD_BY_VID,
	MESSENGER_SUBHEADER_CG_ADD_BY_NAME,
	MESSENGER_SUBHEADER_CG_REMOVE,
	MESSENGER_SUBHEADER_CG_INVITE_ANSWER,
#ifdef ENABLE_MESSENGER_BLOCK
	MESSENGER_SUBHEADER_CG_BLOCK_ADD_BY_VID,
	MESSENGER_SUBHEADER_CG_BLOCK_ADD_BY_NAME,
	MESSENGER_SUBHEADER_CG_BLOCK_REMOVE,
	MESSENGER_SUBHEADER_CG_BLOCK_REMOVE_BY_VID,
#endif
};

typedef struct command_messenger
{
	BYTE header;
	BYTE subheader;
} TPacketCGMessenger;

typedef struct command_messenger_add_by_vid
{
	DWORD vid;
} TPacketCGMessengerAddByVID;

typedef struct command_messenger_add_by_name
{
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketCGMessengerAddByName;

typedef struct command_messenger_remove
{
	char login[LOGIN_MAX_LEN+1];
	//DWORD account;
} TPacketCGMessengerRemove;

#ifdef ENABLE_MESSENGER_BLOCK
typedef struct command_messenger_add_block_by_vid
{
	DWORD vid;
} TPacketCGMessengerAddBlockByVID;

typedef struct command_messenger_remove_block_by_vid
{
	DWORD vid;
} TPacketCGMessengerRemoveBlockByVID;

typedef struct command_messenger_add_block_by_name
{
	BYTE length;
} TPacketCGMessengerAddBlockByName;
#endif

typedef struct command_safebox_checkout
{
	BYTE	bHeader;
	BYTE	bSafePos;
	TItemPos	ItemPos;
} TPacketCGSafeboxCheckout;

typedef struct command_safebox_checkin
{
	BYTE	bHeader;
	BYTE	bSafePos;
	TItemPos	ItemPos;
} TPacketCGSafeboxCheckin;

///////////////////////////////////////////////////////////////////////////////////
// Party

typedef struct command_party_parameter
{
	BYTE	bHeader;
	BYTE	bDistributeMode;
} TPacketCGPartyParameter;

typedef struct paryt_parameter
{
	BYTE	bHeader;
	BYTE	bDistributeMode;
} TPacketGCPartyParameter;

#ifdef TOURNAMENT_PVP_SYSTEM
typedef struct packet_tournament_add
{
	BYTE	header;
	int		membersOnline_A;
	int		membersOnline_B;
	int		membersDead_A;
	int		membersDead_B;
	int		memberLives;
	int		dwTimeRemained;
} TPacketGCTournamentAdd;
#endif

typedef struct packet_party_add
{
	BYTE	header;
	DWORD	pid;
	char	name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCPartyAdd;

typedef struct command_party_invite
{
	BYTE	header;
	DWORD	vid;
} TPacketCGPartyInvite;

typedef struct packet_party_invite
{
	BYTE	header;
	DWORD	leader_vid;
} TPacketGCPartyInvite;

typedef struct command_party_invite_answer
{
	BYTE	header;
	DWORD	leader_vid;
	BYTE	accept;
} TPacketCGPartyInviteAnswer;

typedef struct packet_party_update
{
	BYTE	header;
	DWORD	pid;
	BYTE	role;
	BYTE	percent_hp;
	short	affects[7];
} TPacketGCPartyUpdate;

typedef struct packet_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketGCPartyRemove;

typedef struct packet_party_link
{
	BYTE header;
	DWORD pid;
	DWORD vid;
} TPacketGCPartyLink;

typedef struct packet_party_unlink
{
	BYTE header;
	DWORD pid;
	DWORD vid;
} TPacketGCPartyUnlink;

typedef struct command_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketCGPartyRemove;

typedef struct command_party_set_state
{
	BYTE header;
	DWORD pid;
	BYTE byRole;
	BYTE flag;
} TPacketCGPartySetState;

enum
{
	PARTY_SKILL_HEAL = 1,
	PARTY_SKILL_WARP = 2
};

typedef struct command_party_use_skill
{
	BYTE header;
	BYTE bySkillIndex;
	DWORD vid;
} TPacketCGPartyUseSkill;

typedef struct packet_safebox_size
{
	BYTE bHeader;
	BYTE bSize;
} TPacketCGSafeboxSize;

typedef struct packet_safebox_wrong_password
{
	BYTE	bHeader;
} TPacketCGSafeboxWrongPassword;

typedef struct command_empire
{
	BYTE	bHeader;
	BYTE	bEmpire;
} TPacketCGEmpire;

typedef struct packet_empire
{
	BYTE	bHeader;
	BYTE	bEmpire;
} TPacketGCEmpire;

enum
{
	SAFEBOX_MONEY_STATE_SAVE,
	SAFEBOX_MONEY_STATE_WITHDRAW,
};

typedef struct command_safebox_money
{
	BYTE        bHeader;
	BYTE        bState;
	long	lMoney;
} TPacketCGSafeboxMoney;

typedef struct packet_safebox_money_change
{
	BYTE	bHeader;
	long	lMoney;
} TPacketGCSafeboxMoneyChange;

// Guild

enum
{
	GUILD_SUBHEADER_GC_LOGIN,
	GUILD_SUBHEADER_GC_LOGOUT,
	GUILD_SUBHEADER_GC_LIST,
	GUILD_SUBHEADER_GC_GRADE,
	GUILD_SUBHEADER_GC_ADD,
	GUILD_SUBHEADER_GC_REMOVE,
	GUILD_SUBHEADER_GC_GRADE_NAME,
	GUILD_SUBHEADER_GC_GRADE_AUTH,
	GUILD_SUBHEADER_GC_INFO,
	GUILD_SUBHEADER_GC_COMMENTS,
	GUILD_SUBHEADER_GC_CHANGE_EXP,
	GUILD_SUBHEADER_GC_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_GC_SKILL_INFO,
	GUILD_SUBHEADER_GC_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_GC_GUILD_INVITE,
	GUILD_SUBHEADER_GC_WAR,
	GUILD_SUBHEADER_GC_GUILD_NAME,
	GUILD_SUBHEADER_GC_GUILD_WAR_LIST,
	GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST,
	GUILD_SUBHEADER_GC_WAR_SCORE,
	GUILD_SUBHEADER_GC_MONEY_CHANGE,
};

enum GUILD_SUBHEADER_CG
{
	GUILD_SUBHEADER_CG_ADD_MEMBER,
	GUILD_SUBHEADER_CG_REMOVE_MEMBER,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY,
	GUILD_SUBHEADER_CG_OFFER,
	GUILD_SUBHEADER_CG_POST_COMMENT,
	GUILD_SUBHEADER_CG_DELETE_COMMENT,
	GUILD_SUBHEADER_CG_REFRESH_COMMENT,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_CG_USE_SKILL,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER,
	GUILD_SUBHEADER_CG_CHARGE_GSP,
	GUILD_SUBHEADER_CG_DEPOSIT_MONEY,
	GUILD_SUBHEADER_CG_WITHDRAW_MONEY,
};

typedef struct packet_guild
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCGuild;

typedef struct packet_guild_name_t
{
	BYTE header;
	WORD size;
	BYTE subheader;
	DWORD	guildID;
	char	guildName[GUILD_NAME_MAX_LEN];
} TPacketGCGuildName;

typedef struct packet_guild_war
{
	DWORD	dwGuildSelf;
	DWORD	dwGuildOpp;
	BYTE	bType;
	BYTE 	bWarState;
} TPacketGCGuildWar;

typedef struct command_guild
{
	BYTE header;
	BYTE subheader;
} TPacketCGGuild;

typedef struct command_guild_answer_make_guild
{
	BYTE header;
	char guild_name[GUILD_NAME_MAX_LEN+1];
} TPacketCGAnswerMakeGuild;

typedef struct command_guild_use_skill
{
	DWORD	dwVnum;
	DWORD	dwPID;
} TPacketCGGuildUseSkill;

// Guild Mark
typedef struct command_mark_login
{
	BYTE    header;
	DWORD   handle;
	DWORD   random_key;
} TPacketCGMarkLogin;

typedef struct command_mark_upload
{
	BYTE	header;
	DWORD	gid;
	BYTE	image[16*12*4];
} TPacketCGMarkUpload;

typedef struct command_mark_idxlist
{
	BYTE	header;
} TPacketCGMarkIDXList;

typedef struct command_mark_crclist
{
	BYTE	header;
	BYTE	imgIdx;
	DWORD	crclist[80];
} TPacketCGMarkCRCList;

typedef struct packet_mark_idxlist
{
	BYTE    header;
	DWORD	bufSize;
	WORD	count;
	//µÚ¿¡ size * (WORD + WORD)¸¸Å­ µ¥ÀÌÅÍ ºÙÀ½
} TPacketGCMarkIDXList;

typedef struct packet_mark_block
{
	BYTE	header;
	DWORD	bufSize;
	BYTE	imgIdx;
	DWORD	count;
	// µÚ¿¡ 64 x 48 x ÇÈ¼¿Å©±â(4¹ÙÀÌÆ®) = 12288¸¸Å­ µ¥ÀÌÅÍ ºÙÀ½
} TPacketGCMarkBlock;

typedef struct command_symbol_upload
{
	BYTE	header;
	WORD	size;
	DWORD	guild_id;
} TPacketCGGuildSymbolUpload;

typedef struct command_symbol_crc
{
	BYTE header;
	DWORD guild_id;
	DWORD crc;
	DWORD size;
} TPacketCGSymbolCRC;

typedef struct packet_symbol_data
{
	BYTE header;
	WORD size;
	DWORD guild_id;
} TPacketGCGuildSymbolData;

// Fishing

typedef struct command_fishing
{
	BYTE header;
	BYTE dir;
#if defined(ENABLE_FISHING_GAME)
	BOOL bFishingGame;
#endif
	uint32_t fishing_key;
} TPacketCGFishing;

typedef struct packet_fishing
{
	BYTE header;
	BYTE subheader;
	DWORD info;
	BYTE dir;
#if defined(ENABLE_FISHING_GAME)
	BOOL bFishingGame;
#endif
	uint32_t fishing_key;
} TPacketGCFishing;

enum
{
	FISHING_SUBHEADER_GC_START,
	FISHING_SUBHEADER_GC_STOP,
	FISHING_SUBHEADER_GC_REACT,
	FISHING_SUBHEADER_GC_SUCCESS,
	FISHING_SUBHEADER_GC_FAIL,
	FISHING_SUBHEADER_GC_FISH,
	FISHING_SUBHEADER_GC_START_ANIM,
	FISHING_SUBHEADER_GC_START_ANIM_END,
	FISHING_SUBHEADER_GC_SUCCESS_ANIM,
	FISHING_SUBHEADER_GC_FAIL_ANIM,
};

typedef struct command_give_item
{
	BYTE byHeader;
	DWORD dwTargetVID;
	TItemPos ItemPos;
	DWORD byItemCount;
} TPacketCGGiveItem;

#if defined(ENABLE_FISHING_GAME)
enum EFishingGameSubHeader
{
	FISHING_GAME_SUBHEADER_GOAL,
	FISHING_GAME_SUBHEADER_QUIT,
};
using TPacketCGFishingGame = struct SPacketCGFishingGame
{
	BYTE bHeader;
	BYTE bSubHeader;
	BYTE bGoals;
	uint32_t fishing_key;
};
#endif

typedef struct SPacketCGHack
{
	BYTE	bHeader;
	char	szBuf[255 + 1];
	char	szInfo[255 + 1];
} TPacketCGHack;

typedef struct packet_cheat_blacklist
{
	BYTE	header;
	char	content[1024];
} TPacketGCCheatBlacklist;

// SubHeader - Dungeon
enum
{
	DUNGEON_SUBHEADER_GC_TIME_ATTACK_START = 0,
	DUNGEON_SUBHEADER_GC_DESTINATION_POSITION = 1,
};

typedef struct packet_dungeon
{
	BYTE bHeader;
	WORD size;
	BYTE subheader;
} TPacketGCDungeon;

typedef struct packet_dungeon_dest_position
{
	long x;
	long y;
} TPacketGCDungeonDestPosition;

typedef struct SPacketCGMyShop
{
	BYTE	bHeader;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
	BYTE	bCount;
} TPacketCGMyShop;

typedef struct SPacketGCTime
{
	BYTE	bHeader;
	time_t	time;
} TPacketGCTime;

enum
{
	WALKMODE_RUN,
	WALKMODE_WALK,
};

typedef struct SPacketGCWalkMode
{
	BYTE	header;
	DWORD	vid;
	BYTE	mode;
} TPacketGCWalkMode;

typedef struct SPacketGCChangeSkillGroup
{
	BYTE        header;
	BYTE        skill_group;
} TPacketGCChangeSkillGroup;

typedef struct SPacketCGRefine
{
	BYTE	header;
	WORD	pos;
	BYTE	type;
	BYTE	special_storage;
#ifdef ENABLE_PITTY_REFINE
	bool	seal_of_god;
#endif
} TPacketCGRefine;

typedef struct SPacketGCRefineInformaion
{
	BYTE	header;
	BYTE	type;
	WORD	pos;
	DWORD	src_vnum;
	DWORD	result_vnum;
	BYTE	material_count;
	int		cost;
	int		prob;
	BYTE	special_storage;
	TRefineMaterial materials[REFINE_MATERIAL_MAX_NUM];
} TPacketGCRefineInformation;

#ifndef USE_MARK_ATLAS_BINARY
struct TNPCPosition
{
	BYTE bType;
	DWORD mobVnum;
	long x;
	long y;
	int regenTime;
};

typedef struct SPacketGCNPCPosition
{
	BYTE header;
	WORD size;
	WORD count;

	// array of TNPCPosition
} TPacketGCNPCPosition;
#endif

typedef struct SPacketGCSpecialEffect
{
	BYTE header;
	BYTE type;
	DWORD vid;
} TPacketGCSpecialEffect;

typedef struct SPacketCGChangeName
{
	BYTE header;
	BYTE index;
	char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketCGChangeName;

typedef struct SPacketGCChangeName
{
	BYTE header;
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCChangeName;


typedef struct command_client_version
{
	BYTE header;
	char filename[32+1];
	char timestamp[32+1];
} TPacketCGClientVersion;

typedef struct command_client_version2
{
	BYTE header;
	char filename[32+1];
	char timestamp[32+1];
} TPacketCGClientVersion2;

typedef struct packet_channel
{
	BYTE header;
	BYTE channel;
#ifdef ENABLE_ANTI_EXP
	bool anti_exp;
#endif
} TPacketGCChannel;

typedef struct SEquipmentItemSet
{
	DWORD   vnum;
	DWORD    count;
	long    alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif

} TEquipmentItemSet;

typedef struct pakcet_view_equip
{
	BYTE  header;
	DWORD vid;
	TEquipmentItemSet equips[WEAR_MAX_NUM];
} TPacketViewEquip;

typedef struct
{
	DWORD	dwID;
	long	x, y;
	long	width, height;
	DWORD	dwGuildID;
} TLandPacketElement;

typedef struct packet_land_list
{
	BYTE	header;
	WORD	size;
} TPacketGCLandList;

typedef struct
{
	BYTE	bHeader;
	long	lID;
	char	szName[32+1];
	DWORD	dwVID;
	BYTE	bType;
} TPacketGCTargetCreate;
//search offline
typedef struct
{
	BYTE	bHeader;
	long	lID;
	long	lX, lY;
	BYTE	lType;
} TPacketGCTargetUpdate;
//search offline end
typedef struct
{
	BYTE	bHeader;
	long	lID;
} TPacketGCTargetDelete;

#if defined(__BL_67_ATTR__)
typedef struct command_67_attr
{
	BYTE			bHeader;
	BYTE			bMaterialCount;
	BYTE			bSupportCount;
	short			sSupportPos;
	short			sItemPos;
} TPacketCG67Attr;

typedef struct command_67_attr_open_close
{
	BYTE			bHeader;
} TPacket67AttrOpenClose;
#endif

typedef struct SPacketGCAffectAdd
{
	BYTE					bHeader;
	TPacketAffectElement	elem;
} TPacketGCAffectAdd;

typedef struct
{
	BYTE	bHeader;
	DWORD	dwType;
	BYTE	bApplyOn;
} TPacketGCAffectRemove;

#ifdef ENABLE_YOHARA_SYSTEM
typedef struct packet_sungma_attr_update
{
	BYTE bHeader;
	BYTE bSTR;
	BYTE bHP;
	BYTE bMOVE;
	BYTE bINMUNE;
} TPacketGCSungmaAttrUpdate;
#endif

typedef struct packet_lover_info
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	BYTE love_point;
} TPacketGCLoverInfo;

typedef struct packet_love_point_update
{
	BYTE header;
	BYTE love_point;
} TPacketGCLovePointUpdate;

// MINING
typedef struct packet_dig_motion
{
	BYTE header;
	DWORD vid;
	DWORD target_vid;
	BYTE count;
} TPacketGCDigMotion;
// END_OF_MINING

// SCRIPT_SELECT_ITEM
typedef struct command_script_select_item
{
	BYTE header;
	DWORD selection;
} TPacketCGScriptSelectItem;
// END_OF_SCRIPT_SELECT_ITEM

typedef struct packet_damage_info
{
	BYTE header;
	DWORD dwVictimVID;
	DWORD dwAttackerVID;
	BYTE flag;
	int damage;
} TPacketGCDamageInfo;

enum
{
	RUNUP_MATRIX_ANSWER_MAX_LEN = 4,
	RUNUP_MATRIX_QUIZ_MAX_LEN = 8 ,

};

typedef struct packet_runup_matrix_quiz
{
	BYTE    bHeader;
	char    szQuiz[RUNUP_MATRIX_QUIZ_MAX_LEN + 1];
} TPacketGCRunupMatrixQuiz;

typedef struct command_runup_matrix_answer
{
	BYTE    bHeader;
	char    szAnswer[RUNUP_MATRIX_ANSWER_MAX_LEN + 1];
} TPacketCGRunupMatrixAnswer;

typedef struct packet_passpod
{
	BYTE    bHeader;
	char    szAnswer[8+1];
} TPacketCGPasspod;

typedef struct packet_passpod_failed
{
	BYTE    bHeader;
	char    szMessage[128];
} TPacketCGPasspodFailed;

typedef struct tag_GGSiege
{
	BYTE	bHeader;
	BYTE	bEmpire;
	BYTE	bTowerCount;
} TPacketGGSiege;

typedef struct SPacketGGMonarchTransfer
{
	BYTE	bHeader;
	DWORD	dwTargetPID;
	long	x;
	long	y;
} TPacketMonarchGGTransfer;

typedef struct SPacketGGPCBangUpdate
{
	BYTE bHeader;
	unsigned long ulPCBangID;
} TPacketPCBangUpdate;

typedef struct SPacketGGCheckAwakeness
{
	BYTE bHeader;
} TPacketGGCheckAwakeness;

#ifdef _IMPROVED_PACKET_ENCRYPTION_
struct TPacketKeyAgreement
{
	static const int MAX_DATA_LEN = 256;
	BYTE bHeader;
	WORD wAgreedLength;
	WORD wDataLength;
	BYTE data[MAX_DATA_LEN];
};

struct TPacketKeyAgreementCompleted
{
	BYTE bHeader;
	BYTE data[3]; // dummy (not used)
};

#endif // _IMPROVED_PACKET_ENCRYPTION_

#define MAX_EFFECT_FILE_NAME 128
typedef struct SPacketGCSpecificEffect
{
	BYTE header;
	DWORD vid;
	char effect_file[MAX_EFFECT_FILE_NAME];
} TPacketGCSpecificEffect;

// ¿ëÈ¥¼®
enum EDragonSoulRefineWindowRefineType
{
	DragonSoulRefineWindow_UPGRADE,
	DragonSoulRefineWindow_IMPROVEMENT,
	DragonSoulRefineWindow_REFINE,
};

enum EPacketCGDragonSoulSubHeaderType
{
	DS_SUB_HEADER_OPEN,
	DS_SUB_HEADER_CLOSE,
	DS_SUB_HEADER_DO_REFINE_GRADE,
	DS_SUB_HEADER_DO_REFINE_STEP,
	DS_SUB_HEADER_DO_REFINE_STRENGTH,
	DS_SUB_HEADER_REFINE_FAIL,
	DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE,
	DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL,
	DS_SUB_HEADER_REFINE_SUCCEED,
};
typedef struct SPacketCGDragonSoulRefine
{
	SPacketCGDragonSoulRefine() : header (HEADER_CG_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos ItemGrid[DRAGON_SOUL_REFINE_GRID_SIZE];
} TPacketCGDragonSoulRefine;

typedef struct SPacketGCDragonSoulRefine
{
	SPacketGCDragonSoulRefine() : header(HEADER_GC_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos Pos;
} TPacketGCDragonSoulRefine;

typedef struct SPacketCGStateCheck
{
	BYTE header;
	unsigned long key;
	unsigned long index;
} TPacketCGStateCheck;

typedef struct SPacketGCStateCheck
{
	BYTE header;
	unsigned long key;
	unsigned long index;
	unsigned char state;
} TPacketGCStateCheck;
#ifdef ENABLE_IKASHOP_RENEWAL
//ACTIONS PACKETS
struct TPacketGCNewIkarusShop
{
	BYTE header;
#ifdef ENABLE_LARGE_DYNAMIC_PACKETS
	int size;
#else
	WORD size;
#endif
	BYTE subheader;
};

struct TPacketCGNewIkarusShop {
	BYTE header;
	WORD size;
	BYTE subheader;
};

namespace ikashop
{
    //common
    struct TFilterInfo 
    {
#ifdef USE_OFFLINESHOP_SEARCH_V2
        int32_t type;
        int32_t subtype;
        char name[ITEM_NAME_MAX_LEN];
        bool exact;
        uint32_t vnum;
        bool player;
        uint32_t pid;
        uint32_t antiflags;
#else
        BYTE type;
        BYTE subtype;
        char name[ITEM_NAME_MAX_LEN];
#endif
        TPriceInfo pricestart;
        TPriceInfo pricend;
        int levelstart;
        int levelend;
        TPlayerItemAttribute attrs[ITEM_ATTRIBUTE_NORM_NUM];
        int sashGrade;
        int alchemyGrade;
    };

	struct TShopItemInfo 
	{
		TItemPos pos;
		TPriceInfo price;
	};

#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TShopItemView : public ikashop::TShopItem
	{
		long long priceavg;
	};
#endif

#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TSearchResultInfo : public ikashop::TShopItemView
#else
	struct TSearchResultInfo : public ikashop::TShopItem 
#endif
	{
		bool auction;
		char seller_name[OFFLINE_SHOP_NAME_MAX_LEN];
		int duration;
	};

	//AUCTION
	struct TAuctionListElement 
	{
		TAuctionInfo auction;
		TPriceInfo best;
		DWORD offercount;
#ifdef EXTEND_IKASHOP_ULTIMATE
		long long priceavg;
#endif
	};

	//ikashop-updated 03/08/19
	struct TOfferListElement : public TOfferInfo 
	{
		TShopItem item;
		char shopname[OFFLINE_SHOP_NAME_MAX_LEN];
		bool incoming;
#ifdef EXTEND_IKASHOP_ULTIMATE
		long long priceavg;
#endif
	};

	//GAME TO CLIENT
	enum eSubHeaderGC
	{
		SUBHEADER_GC_SHOP_OPEN,
		SUBHEADER_GC_SHOP_OPEN_OWNER,
		SUBHEADER_GC_SHOP_OPEN_OWNER_NO_SHOP,
		SUBHEADER_GC_SHOP_EXPIRED_GUESTING,
		SUBHEADER_GC_SHOP_REMOVE_ITEM_OWNER,
		SUBHEADER_GC_SHOP_REMOVE_ITEM_GUEST,
		SUBHEADER_GC_SHOP_EDIT_ITEM_OWNER,
		SUBHEADER_GC_SHOP_EDIT_ITEM_GUEST,

		SUBHEADER_GC_OFFER_LIST,

		SUBHEADER_GC_SHOP_FILTER_RESULT,
		SUBHEADER_GC_SHOP_SAFEBOX_REFRESH,
#ifdef USE_IKASHOP_UI_RENEWED
		SUBHEADER_GC_SHOP_SAFEBOX_REFRESH_VALUTES,
#endif
		SUBHEADER_GC_SHOP_SAFEBOX_REMOVE_ITEM,
		SUBHEADER_GC_SHOP_SAFEBOX_ADD_ITEM,

		//AUCTION
		SUBHEADER_GC_AUCTION_LIST,
		SUBHEADER_GC_OPEN_MY_AUCTION,
		SUBHEADER_GC_OPEN_MY_AUCTION_NO_AUCTION,
		SUBHEADER_GC_OPEN_AUCTION,

		// ERROR HANDLING
		SUBHEADER_GC_POPUP_MESSAGE,

#ifdef ENABLE_IKASHOP_ENTITIES
		SUBHEADER_GC_INSERT_SHOP_ENTITY,
		SUBHEADER_GC_REMOVE_SHOP_ENTITY,
#endif
		SUBHEADER_GC_BOARD_COUNTERS,
#ifdef EXTEND_IKASHOP_PRO
		SUBHEADER_GC_NOTIFICATION_LIST,
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		SUBHEADER_GC_PRICE_AVERAGE_RESPONSE,
#endif
		SUBHEADER_GC_SEARCH_RESULT_DELETE,
#ifdef USE_OFFLINESHOP_SEARCH_V2
        SUBHEADER_GC_SHOP_SEARCH_RESULT,
#endif
	};

#ifdef USE_OFFLINESHOP_SEARCH_V2
    typedef struct SGCShopSearchPacket
    {
        int32_t     page;
        size_t      itemcount;
        size_t      pagecount;
    } TGCShopSearchPacket;
#endif

	struct TSubPacketGCShopOpen 
	{
		TShopInfo shop{};
	};

	struct TSubPacketGCShopOpenOwner
	{
		TShopInfo shop{};
#ifdef USE_IKASHOP_UI_RENEWED
		TValutesInfo valute;
#endif
	};

	struct TSubPacketGCShopExpiredGuesting 
	{
		DWORD ownerid;
	};

	struct TSubPacketGCShopRemoveItem
	{
		DWORD itemid;
	};

	struct TSubPacketGCShopEditItem
	{
		DWORD itemid;
		TPriceInfo price;
	};

	struct TSubPacketGCShopFilterResult 
	{
		DWORD count;
	};

	struct TSubPacketGCShopOfferList 
	{
		DWORD offercount;
	};

	struct TSubPacketGCShopSafeboxRefresh
	{
		TValutesInfo valute;
		DWORD itemcount;
	};

#ifdef USE_IKASHOP_UI_RENEWED
	struct TSubPacketGCShopSafeboxRefreshValutes
	{
		TValutesInfo valute;
	};
#endif

	struct TSubPacketGCShopSafeboxRemoveItem
	{
		DWORD itemid;
	};

	struct TSubPacketGCShoppingSafeboxAddItem
	{
		TShopPlayerItem item;
	};

	//AUCTION
	struct TSubPacketGCAuctionList 
	{
		DWORD count;
	};

	struct TSubPacketGCAuctionOpen 
	{
		TAuctionInfo auction;
		DWORD offercount;
#ifdef EXTEND_IKASHOP_ULTIMATE
		long long priceavg;
#endif
	};

	struct TSubPacketGCPopupMessage
	{
		static constexpr inline auto MESSAGE_LEN = 80;
		char localeString[MESSAGE_LEN];
	};

#ifdef ENABLE_IKASHOP_ENTITIES
	struct TSubPacketGCInsertShopEntity 
	{
		DWORD vid;
		char name[OFFLINE_SHOP_NAME_MAX_LEN];
		int type;
		int x;
		int y;
		int z;
	};

	struct TSubPacketGCRemoveShopEntity 
	{
		DWORD vid;
	};
#endif

	struct TSubPacketGCBoardCounters
	{
		int safebox;
		int offers;
		int auction;
#ifdef EXTEND_IKASHOP_PRO
		int notification;
#endif
	};

#ifdef EXTEND_IKASHOP_PRO
	struct TSubPacketGCNotificationList
	{
		DWORD count;
	};
#endif

	// CLIENT TO GAME
	enum eSubHeaderCG
	{
		SUBHEADER_CG_SHOP_CREATE_NEW,
		SUBHEADER_CG_SHOP_FORCE_CLOSE,
		SUBHEADER_CG_SHOP_REQUEST_SHOPLIST,
		SUBHEADER_CG_SHOP_OPEN,
		SUBHEADER_CG_SHOP_OPEN_OWNER,
		SUBHEADER_CG_SHOP_BUY_ITEM,

		SUBHEADER_CG_SHOP_ADD_ITEM,
		SUBHEADER_CG_SHOP_REMOVE_ITEM,
		SUBHEADER_CG_SHOP_EDIT_ITEM,

		SUBHEADER_CG_SHOP_FILTER_REQUEST,
		SUBHEADER_CG_SHOP_SEARCH_FILL_REQUEST,

		SUBHEADER_CG_SHOP_OFFER_CREATE,
		SUBHEADER_CG_SHOP_OFFER_ACCEPT,
		SUBHEADER_CG_SHOP_OFFER_CANCEL,
		SUBHEADER_CG_SHOP_REQUEST_OFFER_LIST,

		SUBHEADER_CG_SHOP_SAFEBOX_OPEN,
		SUBHEADER_CG_SHOP_SAFEBOX_GET_ITEM,
		SUBHEADER_CG_SHOP_SAFEBOX_GET_VALUTES,
		SUBHEADER_CG_SHOP_SAFEBOX_CLOSE,

		//AUCTION
		SUBHEADER_CG_AUCTION_LIST_REQUEST,
		SUBHEADER_CG_AUCTION_OPEN_REQUEST,
		SUBHEADER_CG_MY_AUCTION_OPEN_REQUEST,
		SUBHEADER_CG_MY_AUCTION_CLOSE,
		SUBHEADER_CG_MY_AUCTION_CANCEL,
		SUBHEADER_CG_CREATE_AUCTION,
		SUBHEADER_CG_AUCTION_ADD_OFFER,
		SUBHEADER_CG_EXIT_FROM_AUCTION,

		SUBHEADER_CG_CLOSE_MY_SHOP_BOARD,
		SUBHEADER_CG_CLOSE_OFFER_LIST,
		SUBHEADER_CG_CLOSE_SHOP_GUEST,
#ifdef ENABLE_IKASHOP_ENTITIES
		SUBHEADER_CG_CLICK_ENTITY,
#if defined(ENABLE_IKASHOP_ENTITIES) && !defined(USE_IKASHOP_UI_RENEWED)
		SUBHEADER_CG_MOVE_SHOP_ENTITY,
#endif
#ifdef USE_IKASHOP_UI_RENEWED
		SUBHEADER_CG_TELEPORT_TO_SHOP_ENTITY,
#endif
#endif
#ifdef EXTEND_IKASHOP_PRO
		SUBHEADER_CG_NOTIFICATION_LIST_REQUEST,
		SUBHEADER_CG_NOTIFICATION_LIST_CLOSE,
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		SUBHEADER_CG_PRICE_AVERAGE_REQUEST,
		SUBHEADER_CG_SHOP_DECORATION_USE,
		SUBHEADER_CG_SHOP_MOVE_ITEM,
#endif
#ifdef USE_OFFLINESHOP_SEARCH_V2
        SUBHEADER_CG_SHOP_SEARCH_OPEN,
        SUBHEADER_CG_SHOP_SEARCH_CHANGE_PAGE,
        SUBHEADER_CG_SHOP_SEARCH_BUY_ITEM,
#endif
	};

#ifdef USE_OFFLINESHOP_SEARCH_V2
    typedef struct SCGShopSearchPacket
    {
        int32_t itemType;
        int32_t itemSubType;
    } TCGShopSearchPacket;
#endif

	struct TSubPacketCGShopOpen 
	{
		DWORD ownerid;
	};

	struct TSubPacketCGAddItem 
	{
		TItemPos pos;
		TPriceInfo price;
		int destpos;
	};

	struct TSubPacketCGRemoveItem 
	{
		DWORD itemid;
	};

	struct TSubPacketCGEditItem	
	{
		DWORD itemid;
		TPriceInfo price;
	};

	struct TSubPacketCGFilterRequest 
	{
		TFilterInfo filter;
	};

	struct TSubPacketCGOfferCreate 
	{
		TOfferInfo offer;
	};

	struct TSubPacketCGOfferAccept 
	{
		DWORD offerid;
	};

	struct TSubPacketCGOfferCancel 
	{
		DWORD offerid;
		DWORD ownerid;
	};

	struct TSubPacketCGShopSafeboxGetItem 
	{
		DWORD itemid;
	};

	struct TSubPacketCGShopBuyItem 
	{
		DWORD ownerid;
		DWORD itemid;
		bool searching;
		long long seenprice;
	};

	//AUCTION
	struct TSubPacketCGAuctionOpenRequest 
	{
		DWORD ownerid;
	};

	struct TSubPacketCGAuctionCreate 
	{
		TItemPos pos;
		TPriceInfo startprice;
	};

	struct TSubPacketCGAuctionAddOffer 
	{
		DWORD ownerid;
		TPriceInfo price;
	};

	struct TSubPacketCGAuctionExitFrom 
	{
		DWORD ownerid;
	};

#ifdef ENABLE_IKASHOP_ENTITIES
	struct TSubPacketCGShopClickEntity 
	{
		DWORD vid;
	};
#endif

#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TSubPacketCGPriceAverageRequest
	{
		DWORD vnum;
		DWORD count;
	};

	struct TSubPacketCGShopMoveItem
	{
		int srcPos;
		int destPos;
	};
#endif
}


#endif

#ifdef ENABLE_CALENDAR_SYSTEM

enum
{
	CALENDAR_SUB_HEADER_OPEN,
	CALENDAR_SUB_HEADER_CLOSE,
	CALENDAR_SUB_HEADER_SEND_EVENT,

	CALENDAR_RECEIVE_EVENTS,
	CALENDAR_RECEIVE_OPEN,
	CALENDAR_RECEIVE_CLEAR_EVENTS,
	CALENDAR_RECEIVE_LOAD_EVENTS,
	CALENDAR_RECEIVE_LOADING_EVENTS,

	CALENDAR_RECEIVE_OPEN_ADM,
	CALENDAR_RECEIVE_CLEAR_EVENTS_ADM,
	CALENDAR_RECEIVE_LOAD_EVENTS_ADM,
	CALENDAR_RECEIVE_LOADING_EVENTS_ADM,
};

typedef struct packet_send_calendar_system
{
	BYTE header;
	BYTE subheader;
	int  day;
	int  index_event;
	char hora_inicio[6];
	char hora_final[6];

} TPacketCGCalendarSend;

typedef struct SPacketGCCalendarReceive
{
	SPacketGCCalendarReceive() : header(HEADER_GC_CALENDAR_SYSTEM)
	{}
	BYTE header;
	BYTE bSubType;
	/*int  day;
	char event[100];
	char event_descript[250];
	char hora_inicio[6];
	char hora_final[6];
	char hora_inicio_2[6];
	char hora_final_2[6];
	char banner[100];*/

	BYTE bDay;
	char szName[100];
	char szImage[100];
	char szStartAt[100];
	char szEndAt[100];
	DWORD dwDuration;
	
	DWORD dwCurrentTimeStamp;

} TPacketGCCalendarReceive;

typedef struct SPacketGCCalendarReceiveADM
{
	SPacketGCCalendarReceiveADM() : header(HEADER_GC_CALENDAR_SYSTEM_ADM)
	{}
	BYTE header;
	BYTE bSubType;
	int  index_event;
	char event[100];
	char event_descript[100];

} TPacketGCCalendarReceiveADM;

#endif

#ifdef ENABLE_DUNGEON_INFO
enum 
{
	DUNGEON_INFO_SUB_HEADER_OPEN,
	DUNGEON_INFO_SUB_HEADER_TELEPORT,
	DUNGEON_INFO_SUB_HEADER_MISION,
	DUNGEON_INFO_SUB_HEADER_OPEN_SEND,
	DUNGEON_INFO_SUB_HEADER_DATES_SEND,
	DUNGEON_INFO_SUB_HEADER_DATES_CLEAR,
	DUNGEON_INFO_SUB_HEADER_DATES_RANKING_CLEAR,
	DUNGEON_INFO_SUB_HEADER_DATES_RANKING_SEND,
	DUNGEON_INFO_SUB_HEADER_DATES_RANKING_LOAD,
	DUNGEON_INFO_SUB_HEADER_RANKING,

	DUNGEON_INFO_SUB_HEADER_DATES_MISION_CLEAR,
	DUNGEON_INFO_SUB_HEADER_DATES_MISION_SEND,
	DUNGEON_INFO_SUB_HEADER_DATES_MISION_LOAD,


};

typedef struct packet_send_dungeon_info_system
{
	BYTE header;
	BYTE subheader;
	int  index;

} TPacketCGDungeonInfoSend;

typedef struct SPacketGCDungeonInfoReceive
{
	SPacketGCDungeonInfoReceive() : header(HEADER_GC_DUNGEON_INFO_SYSTEM)
	{}

	BYTE header;
	BYTE bSubType;

	SFDateInfoPacket 	dateinfo;
	SFDateRanking 		dateinfo_ranking;
	SFDateMision		dateinfom;

} TPacketGCDungeonInfoReceive;
#endif

#ifdef __KINGDOMS_WAR__
enum
{
	HEADER_GC_KINGDOMSWAR = 218,
	KINGDOMSWAR_SUBHEADER_GC_OPEN = 0,
	KINGDOMSWAR_SUBHEADER_GC_REFRESH,
};

typedef struct SPacketKingdomWar
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	int		iKills[EMPIRE_MAX_NUM - 1];
	int		iLimitKills;
	int		iDeads;
	int		iLimitDeads;
	DWORD	dwTimeRemained;
} TPacketKingdomWar;
#endif

#ifdef ENABLE_DROP_ITEM_WORLDARD
enum
{
	DROP_ITEM_SUB_HEADER_OPEN,
	DROP_ITEM_SUB_HEADER_SEARCH,
	DROP_ITEM_SUB_HEADER_OPEN_RECEIVE,
	DROP_ITEM_SUB_HEADER_DATES_RECEIVE,
	DROP_ITEM_SUB_HEADER_DATES_CLEAR_RECEIVE,
	DROP_ITEM_SUB_HEADER_DATES_LOADING,
};

typedef struct packet_send_drop_item_system
{
	BYTE header;
	BYTE subheader;
	int vnum_item;

}TPacketCGDropItemSend;

typedef struct info_drop_item_system
{
	char name_mob[100];
	DWORD id_mob;
	DWORD prob_item;
	int count_item;
}TInfoDropItemSystem;

typedef struct packet_receive_drop_item_system
{
	packet_receive_drop_item_system(): header(HEADER_GC_DROP_ITEM_SYSTEM)
	{}
	BYTE header;
	BYTE subheader;
	TInfoDropItemSystem info_drop;

}TPacketGCDropItemReceive;

#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD

enum
{
	CUBE_RENEWAL_SUB_HEADER_OPEN_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_CLEAR_DATES_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_DATES_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_DATES_LOADING,

	CUBE_RENEWAL_SUB_HEADER_MAKE_ITEM,
	CUBE_RENEWAL_SUB_HEADER_CLOSE,
};

typedef struct  packet_send_cube_renewal
{
	BYTE header;
	BYTE subheader;
	DWORD index_item;
	DWORD count_item;

}TPacketCGCubeRenewalSend;


typedef struct dates_cube_renewal
{
	DWORD npc_vnum;
	DWORD index;

	DWORD	vnum_reward;
	int		count_reward;

	bool 	item_reward_stackable;

	DWORD	vnum_material_1;
	int		count_material_1;

	DWORD	vnum_material_2;
	int		count_material_2;

	DWORD	vnum_material_3;
	int		count_material_3;

	DWORD	vnum_material_4;
	int		count_material_4;

	DWORD	vnum_material_5;
	int		count_material_5;

	int 	gold;
	int 	percent;

	char 	category[100];
}TInfoDateCubeRenewal;

typedef struct packet_receive_cube_renewal
{
	packet_receive_cube_renewal(): header(HEADER_GC_CUBE_RENEWAL)
	{}

	BYTE header;
	BYTE subheader;
	TInfoDateCubeRenewal	date_cube_renewal;
}TPacketGCCubeRenewalReceive;
#endif

#ifdef BATTLEPASS_WORLDARD
enum
{
	BATTLEPASS_SUB_HEADER_OPEN,
	BATTLEPASS_SUB_HEADER_CLEAR,
	BATTLEPASS_SUB_HEADER_INFO,
	BATTLEPASS_SUB_HEADER_INFO_EXTRA,
	BATTLEPASS_SUB_HEADER_LOADING,
	BATTLEPASS_SUB_HEADER_REWARD,
	BATTLEPASS_SUB_HEADER_UPDATE_CLEAR,
	BATTLEPASS_SUB_HEADER_UPDATE,
	BATTLEPASS_SUB_HEADER_UPDATE_LOADING,
	BATTLEPASS_SUB_HEADER_MP,
};


typedef struct info_item_battlepass
{
	DWORD	vnum;
	DWORD	count;
}TInfoBattlePassItem;

typedef struct info_battlepass
{
	DWORD 	type;
	DWORD 	vnum;
	int 	count;
	int 	count_actual;
	DWORD   reward_status;

	TInfoBattlePassItem data_item[4];

}TInfoBattlePass;

typedef struct  packet_cg_battlepass
{
	BYTE header;
	BYTE subheader;
	int  index;
}TPacketCGBattlePass;

typedef struct packet_gc_battlepass
{
	packet_gc_battlepass(): header(HEADER_GC_BATTLEPASS)
	{}
	BYTE header;
	BYTE subheader;
	int time;
	TInfoBattlePass data;

}TPacketGCBattlePass;
#endif

#ifdef ENABLE_ITEMSHOP
typedef struct SPacketGCItemShop
{
	BYTE	header;
#ifdef ENABLE_LARGE_DYNAMIC_PACKETS
	int		size;
#else
	WORD	size;
#endif
} TPacketGCItemShop;
#endif

#ifdef ENABLE_EVENT_MANAGER
typedef struct SPacketGCEventManager
{
	BYTE	header;
	WORD	size;
} TPacketGCEventManager;
#endif


#ifdef ENABLE_SHOW_CHEST_DROP
typedef struct SPacketGCChestDrop
{
	BYTE	header;
	DWORD	size;
} TPacketGCChestDrop;
#endif

#ifdef ENABLE_NEW_DETAILS_GUI
typedef struct SPacketGCKillLOG
{
	BYTE	header;
	long	kill_log[KILL_MAX_NUM];
} TPacketGCKillLOG;
#endif

#ifdef ENABLE_MULTI_FARM_BLOCK
enum
{
	MULTI_FARM_SET,
	MULTI_FARM_REMOVE,
};
typedef struct SPacketGGMultiFarm
{
	BYTE	header;
	DWORD	size;
	BYTE	subHeader;
	char	playerIP[IP_ADDRESS_LENGTH + 1];
	DWORD	playerID;
	char	playerName[CHARACTER_NAME_MAX_LEN+1];
	bool	farmStatus;
	BYTE	affectType;
	int		affectTime;
} TPacketGGMultiFarm;
#endif

#ifdef ENABLE_CSHIELD
typedef struct SPacketGCCShield
{
	BYTE	header;
	bool	start;
	DWORD	num;
} TPacketGCCShield;

typedef struct SPacketCGCShield
{
	BYTE	header;
	char	key[128];
} TPacketCGCShield;
#endif


#ifdef ENABLE_ULTIMATE_REGEN
typedef struct SGGPacketNewRegen
{
	BYTE header;
	BYTE subHeader;
	WORD id;
	bool isAlive;
} TGGPacketNewRegen;
enum
{
	NEW_REGEN_LOAD,
	NEW_REGEN_REFRESH,
};
#endif
#ifdef ENABLE_EXCHANGE_LOG
typedef struct SPacketGCExchangeLog
{
	BYTE	header;
	WORD	size;
} TPacketGCExchangeLog;
enum
{
	SUB_EXCHANGELOG_LOAD,
	SUB_EXCHANGELOG_LOAD_ITEM,
};
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
typedef struct SCatchKingCard
{
	SCatchKingCard()
	{
		bIndex = 0;
		bIsExposed = false;
	}

	SCatchKingCard(uint8_t index, bool isExposed)
	{
		bIndex = index;
		bIsExposed = isExposed;
	}

	uint8_t bIndex;
	bool bIsExposed;
} TCatchKingCard;

/*Client -> Server*/
enum EPacketCGMiniGameSubHeaderCatchKing
{
	SUBHEADER_CG_CATCH_KING_START,
	SUBHEADER_CG_CATCH_KING_DECKCARD_CLICK,
	SUBHEADER_CG_CATCH_KING_FIELDCARD_CLICK,
	SUBHEADER_CG_CATCH_KING_REWARD,
};

typedef struct SPacketCGMiniGameCatchKing
{
	SPacketCGMiniGameCatchKing() : bHeader(HEADER_CG_MINI_GAME_CATCH_KING) {}
	uint8_t bHeader;
	uint16_t wSize;
	uint8_t bSubheader;
} TPacketCGMiniGameCatchKing;

typedef struct SSubPacketCGMiniGameCatchKingStart {
	uint8_t betNumber;
} TSubPacketCGMiniGameCatchKingStart;

typedef struct SSubPacketCGMiniGameCatchKingFieldCardClick {
	uint8_t cardNumber;
} TSubPacketCGMiniGameCatchKingFieldCardClick;

/*Server -> Client*/
enum EPacketGCMiniGameSubHeaderCatchKing
{
	SUBHEADER_GC_CATCH_KING_EVENT_INFO,
	SUBHEADER_GC_CATCH_KING_START,
	SUBHEADER_GC_CATCH_KING_SET_CARD,
	SUBHEADER_GC_CATCH_KING_RESULT_FIELD,
	SUBHEADER_GC_CATCH_KING_SET_END_CARD,
	SUBHEADER_GC_CATCH_KING_REWARD,
};

typedef struct SPacketGCMiniGameCatchKing
{
	SPacketGCMiniGameCatchKing() : bHeader(HEADER_GC_MINI_GAME_CATCH_KING) {}
	uint8_t bHeader;
	uint16_t wSize;
	uint8_t bSubheader;
} TPacketGCMiniGameCatchKing;

typedef struct SSubPacketGCCatchKingStart
{
	uint32_t dwBigScore;
} TSubPacketGCCatchKingStart;

typedef struct SSubPacketGCCatchKingSetCard
{
	uint8_t bCardInHand;
} TSubPacketGCCatchKingSetCard;

typedef struct SSubPacketGCCatchKingResult
{
	uint32_t dwPoints;
	uint8_t bRowType;
	uint8_t bCardPos;
	uint8_t bCardValue;
	bool bKeepFieldCard;
	bool bDestroyHandCard;
	bool bGetReward;
	bool bIsFiveNearBy;
} TSubPacketGCCatchKingResult;

typedef struct SSubPacketGCCatchKingSetEndCard
{
	uint8_t bCardPos;
	uint8_t bCardValue;
} TSubPacketGCCatchKingSetEndCard;

typedef struct SSubPacketGCCatchKingReward
{
	uint8_t bReturnCode;
} TSubPacketGCCatchKingReward;

typedef struct SSubPacketGCCatchKingEventInfo
{
	uint8_t bIsEnable;
} TSubPacketGCCatchKingEventInfo;
#endif

#ifdef ENABLE_RANKING
enum
{
	MAX_RANKING_LIST = 51,
	HEADER_GC_RANKING_SEND = 236,
};

typedef struct SPacketGCList
{
	int		iPosition, iRealPosition, iLevel, iPoints;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCList;

typedef struct SPacketGCRankingTable {
	SPacketGCRankingTable() : bHeader(HEADER_GC_RANKING_SEND)
	{}
	BYTE 			bHeader;
	TPacketGCList	list[MAX_RANKING_LIST];
} TPacketGCRankingTable;
#endif

#ifdef __BUFFI_SUPPORT__
typedef struct SPacketGCBuffiSkill
{
	BYTE	header;
	DWORD	dwSkillIdx;
	DWORD	dwVID;
	DWORD	dwTargetVID;
	DWORD	bLevel;
}TPacketGCBuffiSkill;
#endif

#ifdef ENABLE_REWARD_SYSTEM
#include <vector>
typedef struct reward_infos
{
	char lc_text[100];
	char playerName[CHARACTER_NAME_MAX_LEN+1];
	std::vector<std::pair<DWORD, DWORD>> m_rewardItems;
}TRewardInfo;
enum
{
	REWARD_120 = 1,
	REWARD_115WAR,
	REWARD_115ASSASSIN,
	REWARD_115SURA,
	REWARD_115SHAMAN,
	REWARD_CUSTOM_SASH,
	REWARD_FIRSTAURA,
	REWARD_FIRSTENERGY,
	REWARD_PSKILL,
	REWARD_FIRSTBIO,
	REWARD_FIRSTZODIACWEAPON,
	REWARD_FIRSTZODIACARMOR,
	REWARD_75WEAPONAVG,
	REWARD_105WEAPONAVG,
	REWARD_JOTUN,
	REWARD_HYDRA,
	REWARD_ZODIAC,
	REWARD_MELEY,
	REWARD_AKZADUR,
	REWARD_FIRSTPENDANTWHEEL,
	REWARD_MAX,
};
typedef struct SPacketGGRewardInfo
{
	BYTE	bHeader;
	BYTE	rewardIndex;
	char	playerName[CHARACTER_NAME_MAX_LEN+1];
} TPacketGGRewardInfo;
#endif

#ifdef ENABLE_NINJEYE
struct TPacketGCNinjEyeRequest
{
    uint8_t  Header;
    uint32_t Message;
};

struct TPacketCGNinjEyeResponse
{
    uint8_t  Header;
    uint32_t Message;
};
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
enum
{
	SWITCH_SUB_HEADER_CHANGE,
	SWITCH_SUB_HEADER_CLOSE,
	SWITCH_SUB_HEADER_OPEN,

	SWITCH_RECEIVE_OPEN,
	SWITCH_RECEIVE_CLEAR_INFO,
	SWITCH_RECEIVE_INFO_BONUS_ITEM,
	SWITCH_RECEIVE_INFO_EXTRA,
};

typedef struct packet_send_switchbot
{
	BYTE header;
	BYTE subheader;
	TPlayerItemAttribute aAttr[5];

} TPacketCGSwitchBotSend;

typedef struct SPacketGCSwitchBotReceive
{
	SPacketGCSwitchBotReceive() : header(HEADER_GC_SWITCHBOT)
	{}
	BYTE header;
	BYTE bSubType;
	int  id_bonus;
	int  value_bonus_0;
	int  value_bonus_1;
	int  value_bonus_2;
	int  value_bonus_3;
	int  value_bonus_4;

} TPacketGCSwitchBotReceive;
#endif

typedef struct SGCRankInfo
{
	uint8_t	bHeader;
	char 	szName[CHARACTER_NAME_MAX_LEN + 1];
	int		iMode, iPos, bEmpire, iMyPos;
	int64_t	lValue;
	BYTE	bLevel;
	char	szGuildName[GUILD_NAME_MAX_LEN + 1];
} TPacketGCRankInfo;

typedef struct SPacketGGPlayerRank
{
	uint8_t		bHeader;
	char		szName[CHARACTER_NAME_MAX_LEN + 1];
	uint8_t		bEmpire;
	int64_t		lValue;
	int			iMode;
	uint32_t	dwPid;
	char		szGuildName[GUILD_NAME_MAX_LEN + 1];
} TPacketGGPlayerRank;

typedef struct SPacketGGPlayerRankReload
{
	BYTE	bHeader;
	DWORD	dwPID;
} TPacketGGPlayerRankReload;

typedef struct SItemData
{
	// DWORD	vnum;
	// BYTE	count;
	// long	alSockets[ITEM_SOCKET_MAX_NUM];
	// TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
	
	DWORD	vnum;
	DWORD	count;
	DWORD	flags;
	DWORD	anti_flags;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute    aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif

#ifdef __SOULBINDING_SYSTEM__
	long	bind;
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char	name[ITEM_NAME_MAX_LEN+1];
#endif
} TItemData;

#ifdef __AURA_SYSTEM__
enum EPacketGCAuraSubHeader
{
	AURA_SUBHEADER_GC_OPEN,
	AURA_SUBHEADER_GC_CLOSE,
	AURA_SUBHEADER_GC_SET_ITEM,
	AURA_SUBHEADER_GC_CLEAR_SLOT,
	AURA_SUBHEADER_GC_CLEAR_ALL,
	AURA_SUBHEADER_GC_CLEAR_RIGHT,
	AURA_SUBHEADER_GC_REFINE_INFO,
};

typedef struct SSubPacketGCAuraOpenClose
{
	BYTE	bAuraWindowType;
} TSubPacketGCAuraOpenClose;

typedef struct SSubPacketGCAuraSetItem
{
	TItemPos	Cell;
	TItemPos	AuraCell;
	TItemData	pItem;
} TSubPacketGCAuraSetItem;

typedef struct SSubPacketGCAuraClearSlot
{
	BYTE	bAuraSlotPos;
} TSubPacketGCAuraClearSlot;

typedef struct SSubPacketGCAuraRefineInfo
{
	BYTE	bAuraRefineInfoType;
	BYTE	bAuraRefineInfoLevel;
	BYTE	bAuraRefineInfoExpPercent;
} TSubPacketGCAuraRefineInfo;

enum EPacketCGAuraSubHeader
{
	AURA_SUBHEADER_CG_REFINE_CHECKIN,
	AURA_SUBHEADER_CG_REFINE_CHECKOUT,
	AURA_SUBHEADER_CG_REFINE_ACCEPT,
	AURA_SUBHEADER_CG_REFINE_CANCEL,
};

typedef struct SSubPacketCGAuraRefineCheckIn
{
	TItemPos	ItemCell;
	TItemPos	AuraCell;
	BYTE		byAuraRefineWindowType;
} TSubPacketCGAuraRefineCheckIn;

typedef struct SSubPacketCGAuraRefineCheckOut
{
	TItemPos	AuraCell;
	BYTE		byAuraRefineWindowType;
} TSubPacketCGAuraRefineCheckOut;

typedef struct SSubPacketCGAuraRefineAccept
{
	BYTE		byAuraRefineWindowType;
} TSubPacketCGAuraRefineAccept;

typedef struct SPacketGCAura
{
	SPacketGCAura() : bHeader(HEADER_GC_AURA) {}
	BYTE bHeader;
	WORD wSize;
	BYTE bSubHeader;
} TPacketGCAura;

typedef struct SPacketCGAura
{
	SPacketCGAura() : bHeader(HEADER_CG_AURA) {}
	BYTE bHeader;
	WORD wSize;
	BYTE bSubHeader;
} TPacketCGAura;
#endif

#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
typedef struct SHitCountInfo
{
	SHitCountInfo() { dwVid = dwCount = 0; }
	SHitCountInfo(uint32_t vid, uint32_t count)
	{
		dwVid = vid;
		dwCount = count;
	}
	uint32_t dwVid;
	uint32_t dwCount;
} THitCountInfo;

typedef struct SPacketGCHitCountInfo {
	uint8_t bHeader;
	uint32_t dwVid;
	uint32_t dwCount;
} TPacketGCHitCountInfo;
#endif

#ifdef ENABLE_BATTLEPASS
inline namespace net_battlepass
{
	struct packet_battlepass
	{
		uint8_t header;
		uint16_t size;
		uint8_t subheader;
	};

	struct packet_battlepass_info
	{
		uint32_t tier_count;
		uint32_t mission_count;
	};

	struct BattlePassCharacterInfo
	{
		uint32_t current_level = 0;
		uint32_t current_exp = 0;
		bool	 premium_status = false;
	};

	struct BattlePassCharacterMissionInfo
	{
		uint32_t mission_index;
		uint32_t progress;
		bool	 completed;
        bool     collected;
	};
	
	struct BattlePassCharacterTierInfo
	{
		uint16_t mission_index;
        bool	 reward_collected;
        bool	 reward_premium_collected;
	};

	enum BattlePassPacket
    {
        HEADER_GC_BATTLEPASS = 173
    };

    enum BattlePassSubPackets
    {
		SUBHEADER_SEND_CHARACTER_DATA, // tier level, tier exp, premium status etc
		SUBHEADER_SEND_CHARACTER_PROGRESS_UPDATE, // tier level, tier exp, premium status etc
		SUBHEADER_SEND_MISSION_PROGRESS_UPDATE, // updating progess of a mission
        SUBHEADER_SEND_REWARD_UPDATE,             // updating progess of a rewards

	};

    } // namespace net_battlepass
#endif


#if defined(__BL_BATTLE_ROYALE__)
typedef struct SPacketGGBattleRoyale
{
	uint8_t		header;
	uint8_t		subheader;
	uint8_t		state;
	uint32_t	dwPID;
} TPacketGGBattleRoyale;

typedef struct SPacketGCBattleRoyale
{
	uint8_t		header;
	uint16_t	size;
	uint8_t		subheader;
} TPacketGCBattleRoyale;

typedef struct SPacketCGBattleRoyale
{
	uint8_t		header;
	uint8_t		subheader;
	bool		arg;
} TPacketCGBattleRoyale;
#endif

#ifdef ENABLE_VOICE_CHAT
enum EVoiceChatTypes : uint8_t
{
	VOICE_CHAT_TYPE_NONE,
	VOICE_CHAT_TYPE_LOCAL,
	VOICE_CHAT_TYPE_PARTY,
	VOICE_CHAT_TYPE_GUILD,
	VOICE_CHAT_TYPE_MAX_NUM,
};

typedef struct SVoiceChatPacket
{
	uint8_t header;
	uint16_t size;
	uint16_t dataSize;
	uint16_t distance;
	uint8_t type;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	uint32_t vid;
} TVoiceChatPacket;
#endif

#pragma pack()
#endif
