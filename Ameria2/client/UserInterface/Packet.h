#pragma once

#include "Locale.h"
#include "../gamelib/RaceData.h"
#include "../gamelib/ItemData.h"

typedef BYTE TPacketHeader;

enum
{
	/////////////////////////////////////////////////
	// To Server
	// HEADER_BLANK is the not use(for future use)
	HEADER_CG_LOGIN								= 1,
	HEADER_CG_ATTACK							= 225,
	HEADER_CG_CHAT								= 231,
	HEADER_CG_PLAYER_CREATE						= 4,		// 새로운 플래이어를 생성
	HEADER_CG_PLAYER_DESTROY					= 5,		// 플래이어를 삭제.
	HEADER_CG_PLAYER_SELECT						= 6,
	HEADER_CG_CHARACTER_MOVE					= 7,
	HEADER_CG_SYNC_POSITION  					= 8,
	HEADER_CG_DIRECT_ENTER						= 9,
	HEADER_CG_ENTERGAME							= 10,
	HEADER_CG_ITEM_USE							= 11,
	HEADER_CG_ITEM_DROP							= 224,
	HEADER_CG_ITEM_MOVE							= 13,
	HEADER_CG_ITEM_PICKUP						= 15,
	HEADER_CG_QUICKSLOT_ADD					 = 16,
	HEADER_CG_QUICKSLOT_DEL					 = 17,
	HEADER_CG_QUICKSLOT_SWAP					= 18,
	HEADER_CG_WHISPER							= 19,
	HEADER_CG_ITEM_DROP2						= 20,
	HEADER_CG_PLAYER_HAIR_CHANGE				= 21,
#ifdef ENABLE_SELL_ITEM
	HEADER_CG_ITEM_SELL							= 210,
#endif
	//HEADER_BLANK22								= 23,
	//HEADER_BLANK24								= 24,
	//HEADER_BLANK25								= 25,
	HEADER_CG_ON_CLICK							= 26,
	HEADER_CG_EXCHANGE							= 27,
	HEADER_CG_CHARACTER_POSITION				= 28,
	HEADER_CG_SCRIPT_ANSWER						= 29,
	HEADER_CG_QUEST_INPUT_STRING				= 30,
	HEADER_CG_QUEST_CONFIRM					 = 31,
	//HEADER_BLANK32								= 32,
	//HEADER_BLANK33								= 33,
	//HEADER_BLANK34								= 34,
	//HEADER_BLANK35								= 35,
	//HEADER_BLANK36								= 36,
	//HEADER_BLANK37								= 37,
	//HEADER_BLANK38								= 38,
	//HEADER_BLANK39								= 39,
	//HEADER_BLANK40								= 40,
	HEADER_CG_PVP								= 41,
	//HEADER_BLANK42								= 42,
	//HEADER_BLANK43								= 43,
	//HEADER_BLANK44								= 44,
	//HEADER_BLANK45								= 45,
	//HEADER_BLANK46								= 46,
	//HEADER_BLANK47								= 47,
	//HEADER_BLANK48								= 48,
	//HEADER_BLANK49								= 49,
	HEADER_CG_SHOP								= 50,
	HEADER_CG_FLY_TARGETING						= 51,
	HEADER_CG_USE_SKILL							= 233,
	HEADER_CG_ADD_FLY_TARGETING				 = 53,
	HEADER_CG_SHOOT								= 54,
	HEADER_CG_MYSHOP							= 55,

	//HEADER_BLANK56								= 56,
	//HEADER_BLANK57								= 57,
	//HEADER_BLANK58								= 58,
	//HEADER_BLANK59								= 59,
	HEADER_CG_ITEM_USE_TO_ITEM					= 60,
	HEADER_CG_TARGET							= 61,
	//HEADER_BLANK62								= 62,
	//HEADER_BLANK63								= 63,
	//HEADER_BLANK64								= 64,
	HEADER_CG_WARP								= 65,
	HEADER_CG_SCRIPT_BUTTON						= 66,
	HEADER_CG_MESSENGER						 = 67,
	//HEADER_BLANK68								= 68,
	HEADER_CG_MALL_CHECKOUT					 = 69,
	HEADER_CG_SAFEBOX_CHECKIN				   = 70,   // 아이템을 창고에 넣는다.
	HEADER_CG_SAFEBOX_CHECKOUT				  = 71,   // 아이템을 창고로 부터 빼온다.
	HEADER_CG_PARTY_INVITE					  = 72,
	HEADER_CG_PARTY_INVITE_ANSWER			   = 73,
	HEADER_CG_PARTY_REMOVE					  = 74,
	HEADER_CG_PARTY_SET_STATE				   = 75,
	HEADER_CG_PARTY_USE_SKILL				   = 76,
	HEADER_CG_SAFEBOX_ITEM_MOVE				 = 77,
	HEADER_CG_PARTY_PARAMETER				   = 78,
	//HEADER_BLANK68								= 79,
	HEADER_CG_GUILD								= 80,
	HEADER_CG_ANSWER_MAKE_GUILD					= 81,
	HEADER_CG_GIVE_ITEM						 = 83,
#if defined(ENABLE_FISHING_GAME)
	HEADER_CG_FISHING_GAME 						= 85,
#endif
	//HEADER_BLANK84								= 84,
	//HEADER_BLANK85								= 85,
	//HEADER_BLANK86								= 86,
	//HEADER_BLANK87								= 87,
	//HEADER_BLANK88								= 88,
	//HEADER_BLANK89								= 89,
	HEADER_CG_EMPIRE							= 90,
	//HEADER_BLANK91								= 91,
	//HEADER_BLANK92								= 92,
	//HEADER_BLANK93								= 93,
	//HEADER_BLANK94								= 94,
	//HEADER_BLANK95								= 95,
	HEADER_CG_REFINE							= 96,
	//HEADER_BLANK97								= 97,
	//HEADER_BLANK98								= 98,
	//HEADER_BLANK99								= 99,
#ifdef ENABLE_AURA_SYSTEM
	HEADER_CG_AURA								= 97,
#endif
	HEADER_CG_MARK_LOGIN						= 100,
	HEADER_CG_MARK_CRCLIST						= 101,
	HEADER_CG_MARK_UPLOAD						= 102,
	HEADER_CG_MARK_IDXLIST						= 104,

	HEADER_CG_CRC_REPORT						= 103,

	HEADER_CG_HACK								= 105,
	HEADER_CG_CHANGE_NAME					   = 106,
	HEADER_CG_SMS							   = 107,
	HEADER_CG_CHINA_MATRIX_CARD				 = 108,
	HEADER_CG_LOGIN2							= 109,
	HEADER_CG_DUNGEON							= 110,
	HEADER_CG_LOGIN3							= 111,
	HEADER_CG_GUILD_SYMBOL_UPLOAD				= 112,
	HEADER_CG_GUILD_SYMBOL_CRC					= 113,
	HEADER_CG_SCRIPT_SELECT_ITEM				= 114,
	HEADER_CG_LOGIN4							= 115,
#ifdef ENABLE_IKASHOP_RENEWAL
	HEADER_CG_NEW_OFFLINESHOP					= 119,
#endif
#ifdef ENABLE_MINI_GAME_CATCH_KING
	HEADER_CG_MINI_GAME_CATCH_KING				= 163,
#endif
#ifdef ENABLE_SWITCHBOT
	HEADER_CG_SWITCHBOT							= 170,
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
	HEADER_CG_SWITCHBOT							= 171,
#endif

#ifdef ENABLE_NINJEYE
	HEADER_CG_NINJEYE_RESPONSE					= 172,
#endif
#if defined(WJ_COMBAT_ZONE)
	HEADER_CG_COMBAT_ZONE_REQUEST_ACTION		= 117,
#endif
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	HEADER_CG_REQUEST_FLAG				= 118,
#endif
	HEADER_CG_RUNUP_MATRIX_ANSWER			   = 201,
	HEADER_CG_NEWCIBN_PASSPOD_ANSWER			= 202,

	HEADER_CG_DRAGON_SOUL_REFINE			= 205,
	HEADER_CG_STATE_CHECKER					= 94,

#if defined(ENABLE_VOICE_CHAT)
	HEADER_CG_VOICE_CHAT = 207,
#endif

#if defined(__BL_67_ATTR__)
	HEADER_CG_67_ATTR 						= 208,
	HEADER_CG_CLOSE_67_ATTR 				= 209,
#endif

	HEADER_CG_QUEST_INPUT_LONG_STRING				= 214,
		
	HEADER_CG_FISHING						   = 222,
#if defined(__BL_BATTLE_ROYALE__)
	HEADER_CG_BATTLE_ROYALE					= 236,
#endif

	HEADER_CG_KEY_AGREEMENT						= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	HEADER_CG_CSHIELD							= 232,
#endif
	HEADER_CG_TIME_SYNC							= 0xfc,
	HEADER_CG_CLIENT_VERSION					= 0xfd,
	HEADER_CG_CLIENT_VERSION2					= 0xf1,
	HEADER_CG_PONG								= 0xfe,
#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	HEADER_GC_CSHIELD							= 232,
#endif
	HEADER_CG_HANDSHAKE						 = 0xff,
	/////////////////////////////////////////////////
	// From Server
#ifdef ENABLE_CALENDAR_SYSTEM
	HEADER_CG_CALENDAR_SYSTEM 					= 220,
#endif

#ifdef ENABLE_DUNGEON_INFO
	HEADER_CG_DUNGEON_INFO_SYSTEM 				= 213,
#endif
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	HEADER_CG_ITEMS_PICKUP = 235,
#endif
#ifdef ENABLE_DROP_ITEM_WORLDARD
	HEADER_CG_DROP_ITEM_SYSTEM					= 215,
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_CG_CUBE_RENEWAL 						= 216,
#endif
#ifdef BATTLEPASS_WORLDARD
	HEADER_CG_BATTLEPASS						= 230,
#endif

	HEADER_GC_CHARACTER_ADD						= 1,
	HEADER_GC_CHARACTER_DEL						= 2,
	HEADER_GC_CHARACTER_MOVE					= 3,
	HEADER_GC_CHAT								= 4,
	HEADER_GC_SYNC_POSITION 					= 5,
	HEADER_GC_LOGIN_SUCCESS3					= 6,
	HEADER_GC_LOGIN_FAILURE						= 7,
	HEADER_GC_PLAYER_CREATE_SUCCESS				= 8,
	HEADER_GC_PLAYER_CREATE_FAILURE				= 9,
	HEADER_GC_PLAYER_DELETE_SUCCESS				= 10,
	HEADER_GC_PLAYER_DELETE_WRONG_SOCIAL_ID		= 11,
	// 12
	HEADER_GC_STUN								= 13,
	HEADER_GC_DEAD								= 14,

	HEADER_GC_MAIN_CHARACTER					= 15,
	HEADER_GC_PLAYER_POINTS						= 16,
	HEADER_GC_PLAYER_POINT_CHANGE				= 17,
	HEADER_GC_CHANGE_SPEED						= 18,
	HEADER_GC_CHARACTER_UPDATE				  = 19,
#if defined(GAIDEN)
	HEADER_GC_ITEM_DEL							= 20, // 아이템 창에 추가
	HEADER_GC_ITEM_SET							= 21, // 아이템 창에 추가
#else
	HEADER_GC_ITEM_SET							= 20, // 아이템 창에 추가
	HEADER_GC_ITEM_SET2							= 21, // 아이템 창에 추가
#endif
	HEADER_GC_ITEM_USE							= 22, // 아이템 사용 (주위 사람들에게 보여주기 위해)
	HEADER_GC_ITEM_DROP							= 23, // 아이템 버리기
	HEADER_GC_ITEM_UPDATE						= 25, // 아이템 수치 업데이트
	HEADER_GC_ITEM_GROUND_ADD					= 26, // 바닥에 아이템 추가
	HEADER_GC_ITEM_GROUND_DEL					= 27, // 바닥에서 아이템 삭제
	HEADER_GC_QUICKSLOT_ADD					 = 28,
	HEADER_GC_QUICKSLOT_DEL					 = 29,
	HEADER_GC_QUICKSLOT_SWAP					= 30,
	HEADER_GC_ITEM_OWNERSHIP					= 31,
	HEADER_GC_LOGIN_SUCCESS4					= 32,
	HEADER_GC_ITEM_UNBIND_TIME					= 33,
	HEADER_GC_WHISPER							= 34,
	HEADER_GC_ALERT								= 35,

	HEADER_GC_MOTION							= 36,

	HEADER_GC_SHOP								= 38,
#ifdef ENABLE_EVENT_MANAGER
	HEADER_GC_EVENT_MANAGER = 39,
#endif
	// 39 ~ 41 Balnk
	HEADER_GC_DUEL_START						= 40,
	HEADER_GC_PVP								= 41,
	HEADER_GC_EXCHANGE							= 42,
	HEADER_GC_CHARACTER_POSITION				= 43,

	HEADER_GC_PING								= 44,

	HEADER_GC_SCRIPT							= 45,
	HEADER_GC_QUEST_CONFIRM					 = 46,
#ifdef USE_EXPANDED_ITEM_REFRESH
	HEADER_GC_ITEM_UPDATE_COUNT = 49,
#endif
#ifdef NEW_PET_SYSTEM
	HEADER_CG_PetSetName						= 147,
#endif

	HEADER_GC_MOUNT								= 61,
	HEADER_GC_OWNERSHIP						 = 62,
	HEADER_GC_TARGET							= 63,
	#ifdef ENABLE_SEND_TARGET_INFO
	HEADER_GC_TARGET_INFO = 58,
	HEADER_CG_TARGET_INFO_LOAD = 59,
	#endif
	HEADER_GC_WARP								= 65,

#ifdef ENABLE_SHOW_CHEST_DROP
	HEADER_GC_CHESTDROP = 66,
#endif
#ifdef ENABLE_NEW_DETAILS_GUI
	HEADER_GC_KILL_LOG = 68,
#endif

	HEADER_GC_ADD_FLY_TARGETING				 = 69,

	HEADER_GC_CREATE_FLY						= 70,
	HEADER_GC_FLY_TARGETING						= 71,
	HEADER_GC_SKILL_LEVEL						= 72,
	HEADER_GC_SKILL_COOLTIME_END				= 73,
	HEADER_GC_MESSENGER						 = 74,
	HEADER_GC_GUILD								= 75,
	HEADER_GC_SKILL_LEVEL_NEW					= 76,

	HEADER_GC_PARTY_INVITE					  = 77,
	HEADER_GC_PARTY_ADD						 = 78,
	HEADER_GC_PARTY_UPDATE					  = 79,
	HEADER_GC_PARTY_REMOVE					  = 80,

	HEADER_GC_QUEST_INFO						= 81,
	HEADER_GC_REQUEST_MAKE_GUILD				= 82,
	HEADER_GC_PARTY_PARAMETER				   = 83,

	HEADER_GC_SAFEBOX_MONEY_CHANGE			  = 84,
	HEADER_GC_SAFEBOX_SET					   = 85,
	HEADER_GC_SAFEBOX_DEL					   = 86,
	HEADER_GC_SAFEBOX_WRONG_PASSWORD			= 87,
	HEADER_GC_SAFEBOX_SIZE					  = 88,

	HEADER_GC_FISHING						   = 89,

	HEADER_GC_EMPIRE							= 90,

	HEADER_GC_PARTY_LINK						= 91,
	HEADER_GC_PARTY_UNLINK					  = 92,

#ifdef ENABLE_ITEMSHOP
	HEADER_GC_ITEMSHOP = 93,
#endif

	HEADER_GC_REFINE_INFORMATION				= 95,

	HEADER_GC_OBSERVER_ADD						= 96,
	HEADER_GC_OBSERVER_REMOVE					= 97,
	HEADER_GC_OBSERVER_MOVE						= 98,
	HEADER_GC_VIEW_EQUIP						= 99,

	HEADER_GC_MARK_BLOCK						= 100,
	HEADER_GC_MARK_DIFF_DATA					= 101,
	HEADER_GC_MARK_IDXLIST						= 102,

	//HEADER_GC_SLOW_TIMER						= 105,
	HEADER_GC_TIME							  = 106,
	HEADER_GC_CHANGE_NAME					   = 107,

	HEADER_GC_DUNGEON							= 110,
	HEADER_GC_WALK_MODE							= 111,
	HEADER_GC_CHANGE_SKILL_GROUP				= 112,

#if defined(GAIDEN)
	HEADER_GC_MAIN_CHARACTER					= 113,
	HEADER_GC_MAIN_CHARACTER3_BGM				= 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL			= 138,
#else
	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER2_EMPIRE			= 113,
	// END_OF_SUPPORT_BGM
#endif
#ifdef ENABLE_MANAGER_BANK_SYSTEM
	HEADER_GC_BANK								= 141,
#endif
#ifdef ENABLE_AURA_SYSTEM
	HEADER_GC_AURA								= 143,
#endif
	HEADER_GC_SEPCIAL_EFFECT					= 114,
//#ifndef USE_MARK_ATLAS_BINARY
	HEADER_GC_NPC_POSITION = 115,
//#endif
	HEADER_GC_CHINA_MATRIX_CARD				 = 116,
	HEADER_GC_CHARACTER_UPDATE2				 = 117,
	HEADER_GC_LOGIN_KEY						 = 118,
	HEADER_GC_REFINE_INFORMATION_NEW			= 119,
	HEADER_GC_CHARACTER_ADD2					= 120,
	HEADER_GC_CHANNEL						   = 121,

	HEADER_GC_MALL_OPEN						 = 122,
	HEADER_GC_TARGET_UPDATE					 = 123,
	HEADER_GC_TARGET_DELETE					 = 124,
	HEADER_GC_TARGET_CREATE_NEW				 = 125,
#ifdef ENABLE_SUPPORT_SYSTEM
	HEADER_GC_SUPPORT_SKILL						= 148,
#endif

	HEADER_GC_AFFECT_ADD						= 126,
	HEADER_GC_AFFECT_REMOVE					 = 127,

	HEADER_GC_MALL_SET						  = 128,
	HEADER_GC_MALL_DEL						  = 129,
	HEADER_GC_LAND_LIST						 = 130,
	HEADER_GC_LOVER_INFO						= 131,
	HEADER_GC_LOVE_POINT_UPDATE					= 132,
	HEADER_GC_GUILD_SYMBOL_DATA					= 133,
	HEADER_GC_DIG_MOTION						= 134,

	HEADER_GC_DAMAGE_INFO						= 135,
	HEADER_GC_CHAR_ADDITIONAL_INFO				= 136,

	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER3_BGM				= 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL			= 138,
	// END_OF_SUPPORT_BGM
#if defined(WJ_COMBAT_ZONE)
	HEADER_GC_RANKING_COMBAT_ZONE				= 139, 
	HEADER_GC_SEND_COMBAT_ZONE					= 140,
#endif
	HEADER_GC_AUTH_SUCCESS					  = 150,
#ifdef ENABLE_SWITCHBOT_WORLDARD
	HEADER_GC_SWITCHBOT							= 171,
#endif
#ifdef ENABLE_SWITCHBOT
	HEADER_GC_SWITCHBOT							= 171,
#endif

#ifdef ENABLE_NINJEYE
	HEADER_GC_NINJEYE_REQUEST					= 172,
#endif
#if defined(ENABLE_VOICE_CHAT)
	HEADER_GC_VOICE_CHAT = 182,
#endif

	HEADER_GC_RUNUP_MATRIX_QUIZ				 = 201,
	HEADER_GC_NEWCIBN_PASSPOD_REQUEST			= 202,
	HEADER_GC_NEWCIBN_PASSPOD_FAILURE			= 203,
#if defined(GAIDEN)
	HEADER_GC_ONTIME							= 204,
	HEADER_GC_RESET_ONTIME						= 205,

	// AUTOBAN
	HEADER_GC_AUTOBAN_QUIZ						= 206,
	// END_OF_AUTOBAN
#endif

	HEADER_GC_SPECIFIC_EFFECT					= 208,
	HEADER_GC_DRAGON_SOUL_REFINE				= 209,
	HEADER_GC_RESPOND_CHANNELSTATUS				= 210,
	// @fixme007
	HEADER_GC_UNK_213							= 213,
		
#ifdef ENABLE_CALENDAR_SYSTEM
	HEADER_GC_CALENDAR_SYSTEM = 214,
	HEADER_GC_CALENDAR_SYSTEM_ADM = 216,
#endif

#ifdef ENABLE_DUNGEON_INFO
	HEADER_GC_DUNGEON_INFO_SYSTEM = 217,
#endif

#ifdef ENABLE_DROP_ITEM_WORLDARD
	HEADER_GC_DROP_ITEM_SYSTEM = 220,
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_GC_CUBE_RENEWAL = 221,
#endif
#ifdef ENABLE_IKASHOP_RENEWAL
	HEADER_GC_NEW_OFFLINESHOP = 222,
#endif
	HEADER_GC_RANK_INFO		= 223,
#ifdef ENABLE_YOHARA_SYSTEM
	HEADER_GC_SUNGMA_ATTR_UPDATE = 231,
#endif
#ifdef ENABLE_EXCHANGE_LOG
	HEADER_GC_EXCHANGE_LOG = 235,
#endif
#ifdef __BUFFI_SUPPORT__
	HEADER_GC_BUFFI_SKILL = 240,
#endif
#ifdef ENABLE_MINI_GAME_CATCH_KING
	HEADER_GC_MINI_GAME_CATCH_KING = 241,
#endif
#if defined(__BL_67_ATTR__)
	HEADER_GC_OPEN_67_ATTR  = 242,
#endif
//#ifdef ENABLE_ENTITY_LOADING
	HEADER_GC_ENTITY = 245,
//#endif
#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
	HEADER_GC_HIT_COUNT_INFO					= 243,
#endif
#if defined(__BL_BATTLE_ROYALE__)
	HEADER_GC_BATTLE_ROYALE						= 244,
#endif

	HEADER_GC_CHEAT_BLACKLIST					= 249,
	HEADER_GC_KEY_AGREEMENT_COMPLETED			= 0xfa, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_KEY_AGREEMENT						= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_HANDSHAKE_OK						= 0xfc, // 252
	HEADER_GC_PHASE								= 0xfd,	// 253
	HEADER_GC_BINDUDP						   = 0xfe, // 254
	HEADER_GC_HANDSHAKE						 = 0xff, // 255


	/////////////////////////////////////////////////
	// Client To Client for UDP
	/*
	HEADER_CC_STATE_WAITING						= 1,
	HEADER_CC_STATE_WALKING						= 2,
	HEADER_CC_STATE_GOING						= 3,
	HEADER_CC_EVENT_NORMAL_ATTACKING			= 4,
	HEADER_CC_EVENT_COMBO_ATTACKING				= 5,
	HEADER_CC_EVENT_HIT							= 6,
	*/
#ifdef TOURNAMENT_PVP_SYSTEM
	HEADER_GC_TOURNAMENT_ADD			= 142,
#endif
};

enum
{
	ID_MAX_NUM = 30,
	PASS_MAX_NUM = 16,
	CHAT_MAX_NUM = 128,
	PATH_NODE_MAX_NUM = 64,
	

	PLAYER_PER_ACCOUNT3 = 3,
#ifndef ENABLE_PLAYER_PER_ACCOUNT5
	PLAYER_PER_ACCOUNT4 = 4,
#else
	PLAYER_PER_ACCOUNT4 = 5,
	PLAYER_PER_ACCOUNT5 = 5,
#endif

	PLAYER_ITEM_SLOT_MAX_NUM = 20,		// 플래이어의 슬롯당 들어가는 갯수.

	QUICKSLOT_MAX_LINE = 4,
	QUICKSLOT_MAX_COUNT_PER_LINE = 8, // 클라이언트 임의 결정값
	QUICKSLOT_MAX_COUNT = QUICKSLOT_MAX_LINE * QUICKSLOT_MAX_COUNT_PER_LINE,

	QUICKSLOT_MAX_NUM = 36, // 서버와 맞춰져 있는 값

	SHOP_HOST_ITEM_MAX_NUM = 40,
	SHOP_SIGN_MAX_LEN = 32,
	METIN_SOCKET_COUNT = 6,

	PARTY_AFFECT_SLOT_MAX_NUM = 7,

	GUILD_GRADE_NAME_MAX_LEN = 8,
	GUILD_NAME_MAX_LEN = 12,
	GUILD_GRADE_COUNT = 15,
	GULID_COMMENT_MAX_LEN = 50,

	MARK_CRC_NUM = 8*8,
	MARK_DATA_SIZE = 16*12,
	SYMBOL_DATA_SIZE = 128*256,
	QUEST_INPUT_STRING_MAX_NUM = 64,
	QUEST_INPUT_STRING_LONG_MAX_NUM = 512,

	PRIVATE_CODE_LENGTH = 8,

	REFINE_MATERIAL_MAX_NUM = 5,

	CHINA_MATRIX_ANSWER_MAX_LEN	= 8,

	RUNUP_MATRIX_QUIZ_MAX_LEN	= 8,
	RUNUP_MATRIX_ANSWER_MAX_LEN = 4,
	NEWCIBN_PASSPOD_ANSWER_MAX_LEN = 8,
	NEWCIBN_PASSPOD_FAILURE_MAX_LEN = 128,

	WEAR_MAX_NUM = CItemData::WEAR_MAX_NUM,

	SHOP_TAB_NAME_MAX = 32,
	SHOP_TAB_COUNT_MAX = 3,
};

#pragma pack(push)
#pragma pack(1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mark

#ifdef ENABLE_SWITCHBOT
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
	long	alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
	#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	WORD	CellInven;
#endif
};
#endif


typedef struct command_mark_login
{
	BYTE	header;
	DWORD   handle;
	DWORD   random_key;
} TPacketCGMarkLogin;

typedef struct command_mark_upload
{
	BYTE	header;
	DWORD   gid;
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
	DWORD   crclist[80];
} TPacketCGMarkCRCList;

typedef struct packet_mark_idxlist
{
	BYTE	header;
	DWORD	bufSize;
	WORD	count;
	//뒤에 size * (WORD + WORD)만큼 데이터 붙음
} TPacketGCMarkIDXList;

typedef struct packet_mark_block
{
	BYTE	header;
	DWORD   bufSize;
	BYTE	imgIdx;
	DWORD   count;
	// 뒤에 64 x 48 x 픽셀크기(4바이트) = 12288만큼 데이터 붙음
} TPacketGCMarkBlock;

typedef struct command_symbol_upload
{
	BYTE	header;
	WORD	size;
	DWORD	handle;
} TPacketCGSymbolUpload;

typedef struct command_symbol_crc
{
	BYTE	header;
	DWORD	dwGuildID;
	DWORD	dwCRC;
	DWORD	dwSize;
} TPacketCGSymbolCRC;

typedef struct packet_symbol_data
{
	BYTE header;
	WORD size;
	DWORD guild_id;
} TPacketGCGuildSymbolData;

//
//
//
typedef struct packet_observer_add
{
	BYTE	header;
	DWORD	vid;
	WORD	x;
	WORD	y;
} TPacketGCObserverAdd;

typedef struct packet_observer_move
{
	BYTE	header;
	DWORD	vid;
	WORD	x;
	WORD	y;
} TPacketGCObserverMove;


typedef struct packet_observer_remove
{
	BYTE	header;
	DWORD	vid;
} TPacketGCObserverRemove;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// To Server

typedef struct command_checkin
{
	BYTE header;
	char name[ID_MAX_NUM+1];
	char pwd[PASS_MAX_NUM+1];
} TPacketCGCheckin;

typedef struct command_login
{
	BYTE header;
	char name[ID_MAX_NUM + 1];
	char pwd[PASS_MAX_NUM + 1];
} TPacketCGLogin;

// start - 권한 서버 접속을 위한 패킷들
typedef struct command_login2
{
	BYTE	header;
	char	name[ID_MAX_NUM + 1];
	DWORD	login_key;
	DWORD	adwClientKey[4];
} TPacketCGLogin2;

typedef struct command_login3
{
	BYTE	header;
	char	name[ID_MAX_NUM + 1];
	char	pwd[PASS_MAX_NUM + 1];
	DWORD	adwClientKey[4];
#ifdef ENABLE_VERSION_SYSTEM
	WORD	versionKey;
#endif
#ifdef RENEWAL_HWID_BLOCK
	char	hwid_router[18+1];
#endif
} TPacketCGLogin3;

typedef struct command_direct_enter
{
	BYTE		bHeader;
	char		login[ID_MAX_NUM + 1];
	char		passwd[PASS_MAX_NUM + 1];
	BYTE		index;
} TPacketCGDirectEnter;
#ifdef NEW_PET_SYSTEM
typedef struct packet_RequestPetName
{
	BYTE byHeader;
	char petname[13];
} TPacketCGRequestPetName;
#endif

typedef struct command_player_select
{
	BYTE	header;
	BYTE	player_index;
} TPacketCGSelectCharacter;

typedef struct command_attack
{
	BYTE	header;
	BYTE	bType;			// 공격 유형
	DWORD	dwVictimVID;	// 적 VID
	BYTE	bCRCMagicCubeProcPiece;
	BYTE	bCRCMagicCubeFilePiece;
} TPacketCGAttack;

typedef struct command_chat
{
	BYTE	header;
	WORD	length;
	BYTE	type;
} TPacketCGChat;

typedef struct command_whisper
{
	BYTE		bHeader;
	WORD		wSize;
	char		szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisper;

typedef struct command_sms
{
	BYTE		bHeader;
	WORD		wSize;
	char		szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGSMS;

enum EBattleMode
{
	BATTLEMODE_ATTACK = 0,
	BATTLEMODE_DEFENSE = 1,
};

typedef struct command_EnterFrontGame
{
	BYTE header;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	BYTE lang;
#endif
} TPacketCGEnterFrontGame;

typedef struct command_item_use
{
	BYTE header;
	TItemPos pos;
} TPacketCGItemUse;

typedef struct command_item_use_to_item
{
	BYTE header;
	TItemPos source_pos;
	TItemPos target_pos;
} TPacketCGItemUseToItem;

typedef struct command_item_drop
{
	BYTE  header;
	TItemPos pos;
	DWORD elk;
} TPacketCGItemDrop;

typedef struct command_item_drop2
{
	BYTE		header;
	TItemPos pos;
	DWORD	   gold;
	DWORD		count;
} TPacketCGItemDrop2;

typedef struct command_item_move
{
	BYTE header;
	TItemPos pos;
	TItemPos change_pos;
	DWORD num;
} TPacketCGItemMove;

#ifdef ENABLE_SELL_ITEM
typedef struct command_item_sell
{
	BYTE		header;
	TItemPos	pos;
	DWORD		gold;
} TPacketCGItemSell;
#endif

typedef struct command_item_pickup
{
	BYTE header;
	DWORD vid;
} TPacketCGItemPickUp;

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
typedef struct command_items_pickup
{
	BYTE header;
	DWORD count;
} TPacketCGItemsPickUp;
#endif

typedef struct command_quickslot_add
{
	BYTE		header;
	BYTE		pos;
	TQuickSlot	slot;
}TPacketCGQuickSlotAdd;

typedef struct command_quickslot_del
{
	BYTE		header;
	BYTE		pos;
}TPacketCGQuickSlotDel;

typedef struct command_quickslot_swap
{
	BYTE		header;
	BYTE		pos;
	BYTE		change_pos;
}TPacketCGQuickSlotSwap;

typedef struct command_on_click
{
	BYTE		header;
	DWORD		vid;
} TPacketCGOnClick;


enum
{
	SHOP_SUBHEADER_CG_END,
	SHOP_SUBHEADER_CG_BUY,
	SHOP_SUBHEADER_CG_SELL,
	SHOP_SUBHEADER_CG_SELL2,
};

typedef struct command_shop
{
	BYTE		header;
	BYTE		subheader;
} TPacketCGShop;

typedef struct command_exchange
{
	BYTE		header;
	BYTE		subheader;
	long long	arg1;
	BYTE		arg2;
	TItemPos	Pos;
} TPacketCGExchange;

typedef struct command_position
{
	BYTE		header;
	BYTE		position;
} TPacketCGPosition;

typedef struct command_script_answer
{
	BYTE		header;
	BYTE		answer;
} TPacketCGScriptAnswer;

typedef struct command_script_button
{
	BYTE		header;
	unsigned int			idx;
} TPacketCGScriptButton;

typedef struct command_target
{
	BYTE		header;
	DWORD	   dwVID;
} TPacketCGTarget;

typedef struct command_move
{
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	long		lX;
	long		lY;
	DWORD		dwTime;
} TPacketCGMove;

typedef struct command_sync_position_element
{
	DWORD	   dwVID;
	long		lX;
	long		lY;
} TPacketCGSyncPositionElement;

typedef struct command_sync_position
{
	BYTE		bHeader;
	WORD		wSize;
} TPacketCGSyncPosition;

typedef struct command_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwTargetVID;
	long		lX;
	long		lY;
} TPacketCGFlyTargeting;

typedef struct packet_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwShooterVID;
	DWORD		dwTargetVID;
	long		lX;
	long		lY;
} TPacketGCFlyTargeting;

typedef struct packet_shoot
{
	BYTE		bHeader;
	BYTE		bType;
} TPacketCGShoot;

typedef struct command_warp
{
	BYTE			bHeader;
} TPacketCGWarp;

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

typedef struct packet_messenger_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerListOffline;

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

enum
{
	MESSENGER_CONNECTED_STATE_OFFLINE,
	MESSENGER_CONNECTED_STATE_ONLINE,
	MESSENGER_CONNECTED_STATE_MOBILE,
};

typedef struct packet_messenger_list_online
{
	BYTE connected;
	BYTE length;
	//BYTE length_char_name;
} TPacketGCMessengerListOnline;

typedef struct packet_messenger_login
{
	//BYTE length_login;
	//BYTE length_char_name;
	BYTE length;
} TPacketGCMessengerLogin;

typedef struct packet_messenger_logout
{
	BYTE length;
} TPacketGCMessengerLogout;

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

typedef struct command_messenger_remove
{
	BYTE length;
} TPacketCGMessengerRemove;

enum
{
	SAFEBOX_MONEY_STATE_SAVE,
	SAFEBOX_MONEY_STATE_WITHDRAW,
};

typedef struct command_safebox_money
{
	BYTE		bHeader;
	BYTE		bState;
	DWORD	   dwMoney;
} TPacketCGSafeboxMoney;

typedef struct command_safebox_checkout
{
	BYTE		bHeader;
	BYTE		bSafePos;
	TItemPos	ItemPos;
} TPacketCGSafeboxCheckout;

typedef struct command_safebox_checkin
{
	BYTE		bHeader;
	BYTE		bSafePos;
	TItemPos	ItemPos;
} TPacketCGSafeboxCheckin;

typedef struct command_mall_checkout
{
	BYTE		bHeader;
	BYTE		bMallPos;
	TItemPos	ItemPos;
} TPacketCGMallCheckout;

///////////////////////////////////////////////////////////////////////////////////
// Party

typedef struct command_use_skill
{
	BYTE	bHeader;
	DWORD	dwVnum;
	DWORD	dwTargetVID;
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
	bool	bFromAutoHunt;
#endif
} TPacketCGUseSkill;

typedef struct command_party_invite
{
	BYTE header;
	DWORD vid;
} TPacketCGPartyInvite;

typedef struct command_party_invite_answer
{
	BYTE header;
	DWORD leader_pid;
	BYTE accept;
} TPacketCGPartyInviteAnswer;

typedef struct command_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketCGPartyRemove;

typedef struct command_party_set_state
{
	BYTE byHeader;
	DWORD dwVID;
	BYTE byState;
	BYTE byFlag;
} TPacketCGPartySetState;

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

typedef struct command_party_use_skill
{
	BYTE byHeader;
	BYTE bySkillIndex;
	DWORD dwTargetVID;
} TPacketCGPartyUseSkill;

enum
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

typedef struct command_guild
{
	BYTE byHeader;
	BYTE bySubHeader;
} TPacketCGGuild;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
typedef struct SPacketCGRequestFlag
{
	BYTE	header;
	char	name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGRequestFlag;
#endif
typedef struct command_guild_answer_make_guild
{
	BYTE header;
	char guild_name[GUILD_NAME_MAX_LEN+1];
} TPacketCGAnswerMakeGuild;

typedef struct command_give_item
{
	BYTE byHeader;
	DWORD dwTargetVID;
	TItemPos ItemPos;
	DWORD byItemCount;
} TPacketCGGiveItem;

typedef struct SPacketCGHack
{
	BYTE		bHeader;
	char		szBuf[255 + 1];
	char		szInfo[255 + 1];
} TPacketCGHack;

typedef struct packet_cheat_blacklist
{
	BYTE	header;
	char	content[1024];
} TPacketGCCheatBlacklist;

typedef struct command_dungeon
{
	BYTE		bHeader;
	WORD		size;
} TPacketCGDungeon;

#ifdef ENABLE_BUY_WITH_ITEM
typedef struct SShopItemPrice
{
	DWORD		vnum;
	DWORD		count;
} TShopItemPrice;
#endif

// Private Shop
typedef struct SShopItemTable
{
	DWORD		vnum;
	DWORD		count;

#ifdef ENABLE_BUY_WITH_ITEM
	TShopItemPrice	itemprice[MAX_SHOP_PRICES];
#endif

	TItemPos	pos;			// PC 상점에만 이용
#ifdef ENABLE_FULL_YANG
	long long	price;			// PC 상점에만 이용
#else
	DWORD		price;
#endif
	BYTE		display_pos;	//	PC 상점에만 이용, 보일 위치.
 	
} TShopItemTable;

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

typedef struct SPacketCGChangeName
{
	BYTE header;
	BYTE index;
	char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketCGChangeName;

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

typedef struct command_crc_report
{
	BYTE header;
	BYTE byPackMode;
	DWORD dwBinaryCRC32;
	DWORD dwProcessCRC32;
	DWORD dwRootPackCRC32;
} TPacketCGCRCReport;

typedef struct command_china_matrix_card
{
	BYTE	bHeader;
	char	szAnswer[CHINA_MATRIX_ANSWER_MAX_LEN + 1];
} TPacketCGChinaMatrixCard;

typedef struct command_runup_matrix_answer
{
	BYTE	bHeader;
	char	szAnswer[RUNUP_MATRIX_ANSWER_MAX_LEN + 1];
} TPacketCGRunupMatrixAnswer;

typedef struct command_newcibn_passpod_answer
{
	BYTE	bHeader;
	char	szAnswer[NEWCIBN_PASSPOD_ANSWER_MAX_LEN + 1];
} TPacketCGNEWCIBNPasspodAnswer;

enum EPartyExpDistributionType
{
	PARTY_EXP_DISTRIBUTION_NON_PARITY,
	PARTY_EXP_DISTRIBUTION_PARITY,
};

typedef struct command_party_parameter
{
	BYTE		bHeader;
	BYTE		bDistributeMode;
} TPacketCGPartyParameter;

typedef struct command_quest_input_string
{
	BYTE		bHeader;
	char		szString[QUEST_INPUT_STRING_MAX_NUM+1];
} TPacketCGQuestInputString;

typedef struct command_quest_input_long_string
{
	BYTE		bHeader;
	char		szString[QUEST_INPUT_STRING_LONG_MAX_NUM];
} TPacketCGQuestInputLongString;

typedef struct command_quest_confirm
{
	BYTE header;
	BYTE answer;
	DWORD requestPID;
} TPacketCGQuestConfirm;

typedef struct command_script_select_item
{
	BYTE header;
	DWORD selection;
} TPacketCGScriptSelectItem;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// From Server
enum EPhase
{
	PHASE_CLOSE,				// 끊기는 상태 (또는 끊기 전 상태)
	PHASE_HANDSHAKE,			// 악수..;;
	PHASE_LOGIN,				// 로그인 중
	PHASE_SELECT,				// 캐릭터 선택 화면
	PHASE_LOADING,				// 선택 후 로딩 화면
	PHASE_GAME,					// 게임 화면
	PHASE_DEAD,					// 죽었을 때.. (게임 안에 있는 것일 수도..)

	PHASE_DBCLIENT_CONNECTING,	// 서버용
	PHASE_DBCLIENT,				// 서버용
	PHASE_P2P,					// 서버용
	PHASE_AUTH,					// 로그인 인증 용
};

typedef struct packet_phase
{
	BYTE		header;
	BYTE		phase;
} TPacketGCPhase;

typedef struct packet_blank		// 공백패킷.
{
	BYTE		header;
} TPacketGCBlank;

typedef struct packet_blank_dynamic
{
	BYTE		header;
	WORD		size;
} TPacketGCBlankDynamic;

typedef struct packet_header_handshake
{
	BYTE		header;
	DWORD		dwHandshake;
	DWORD		dwTime;
	long		lDelta;
} TPacketGCHandshake;

typedef struct packet_header_bindudp
{
	BYTE		header;
	DWORD		addr;
	WORD		port;
} TPacketGCBindUDP;

typedef struct packet_header_dynamic_size
{
	BYTE		header;
	WORD		size;
} TDynamicSizePacketHeader;

#ifdef ENABLE_LARGE_DYNAMIC_PACKETS
typedef struct packet_header_large_dynamic_size
{
	BYTE		header;
	int			size;
} TLargeDynamicSizePacketHeader;
#endif

typedef struct SSimplePlayerInformation
{
	DWORD			   dwID;
	char				szName[CHARACTER_NAME_MAX_LEN + 1];
	BYTE				byJob;
	BYTE				byLevel;
	DWORD			   dwPlayMinutes;
	BYTE				byST, byHT, byDX, byIQ;
//	DWORD				wParts[CRaceData::PART_MAX_NUM];
	DWORD				wMainPart;
	BYTE				bChangeName;
	DWORD				wHairPart;
	#ifdef ENABLE_SASH_SYSTEM
	DWORD	wSashPart;
	#endif
	BYTE				bDummy[4];
	long				x, y;
	long				lAddr;
	WORD				wPort;
	BYTE				bySkillGroup;
#ifdef ENABLE_YOHARA_SYSTEM
	BYTE				byConquerorLevel;
	BYTE				bySungmaST, bySungmaHP, bySungmaMove, bySungmaInmune;
#endif

} TSimplePlayerInformation;

typedef struct packet_login_success3
{
	BYTE						header;
	TSimplePlayerInformation	akSimplePlayerInformation[PLAYER_PER_ACCOUNT3];
	DWORD						guild_id[PLAYER_PER_ACCOUNT3];
	char						guild_name[PLAYER_PER_ACCOUNT3][GUILD_NAME_MAX_LEN+1];
	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess3;

typedef struct packet_login_success4
{
	BYTE						header;
	TSimplePlayerInformation	akSimplePlayerInformation[PLAYER_PER_ACCOUNT4];
	DWORD						guild_id[PLAYER_PER_ACCOUNT4];
	char						guild_name[PLAYER_PER_ACCOUNT4][GUILD_NAME_MAX_LEN+1];
	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess4;
#ifdef ENABLE_PLAYER_PER_ACCOUNT5
typedef struct packet_login_success5
{
	BYTE						header;
	TSimplePlayerInformation	akSimplePlayerInformation[PLAYER_PER_ACCOUNT5];
	DWORD						guild_id[PLAYER_PER_ACCOUNT5];
	char						guild_name[PLAYER_PER_ACCOUNT5][GUILD_NAME_MAX_LEN+1];
	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess5;
#endif

enum { LOGIN_STATUS_MAX_LEN = 8 };
typedef struct packet_login_failure
{
	BYTE	header;
	char	szStatus[LOGIN_STATUS_MAX_LEN + 1];
} TPacketGCLoginFailure;

typedef struct command_player_create
{
	BYTE		header;
	BYTE		index;
	char		name[CHARACTER_NAME_MAX_LEN + 1];
	WORD		job;
	BYTE		shape;
	BYTE		CON;
	BYTE		INT;
	BYTE		STR;
	BYTE		DEX;
} TPacketCGCreateCharacter;
#ifdef ENABLE_SUPPORT_SYSTEM
typedef struct support_use_skill
{
	BYTE	bHeader;
	DWORD	dwVnum;
	DWORD	dwVid;
	DWORD	dwLevel;
}TPacketGCSupportUseSkill;
#endif
typedef struct command_player_create_success
{
	BYTE						header;
	BYTE						bAccountCharacterSlot;
	TSimplePlayerInformation	kSimplePlayerInfomation;
} TPacketGCPlayerCreateSuccess;

typedef struct command_create_failure
{
	BYTE	header;
	BYTE	bType;
} TPacketGCCreateFailure;

typedef struct command_player_delete
{
	BYTE		header;
	BYTE		index;
	char		szPrivateCode[PRIVATE_CODE_LENGTH];
} TPacketCGDestroyCharacter;

typedef struct command_player_hair_change
{
	BYTE		header;
	BYTE		index;
	int			hairIndex;
} TPacketCGChangeCharacterHair;

typedef struct packet_player_delete_success
{
	BYTE		header;
	BYTE		account_index;
} TPacketGCDestroyCharacterSuccess;

enum
{
	ADD_CHARACTER_STATE_DEAD   = (1 << 0),
	ADD_CHARACTER_STATE_SPAWN  = (1 << 1),
	ADD_CHARACTER_STATE_GUNGON = (1 << 2),
	ADD_CHARACTER_STATE_KILLER = (1 << 3),
	ADD_CHARACTER_STATE_PARTY  = (1 << 4),
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

	PK_MODE_MAX_NUM,
};

// 2004.11.20.myevan.CRaceData::PART_MAX_NUM 사용안하게 수정 - 서버에서 사용하는것과 일치하지 않음
enum ECharacterEquipmentPart
{
	CHR_EQUIPPART_ARMOR,
	CHR_EQUIPPART_WEAPON,
	CHR_EQUIPPART_HEAD,
	CHR_EQUIPPART_HAIR,
	#ifdef ENABLE_SASH_SYSTEM
	CHR_EQUIPPART_SASH,
	#endif
	CHR_PART_ARROW_TYPE,
#ifdef ENABLE_AURA_SYSTEM
	CHR_EQUIPPART_AURA,
#endif
	CHR_EQUIPPART_NUM,
};

#ifdef ENABLE_SHINING_SYSTEM
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

typedef struct packet_char_additional_info
{
	BYTE	header;
	DWORD   dwVID;
	char	name[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	awPart[CHR_EQUIPPART_NUM];
	BYTE	bEmpire;
	DWORD   dwGuildID;
	DWORD   dwLevel;
#ifdef ENABLE_YOHARA_SYSTEM
	DWORD	dwConquerorLevel;
#endif

	short   sAlignment; //선악치
	BYTE	bPKMode;
	DWORD   dwMountVnum;
#if defined(WJ_COMBAT_ZONE)
	BYTE	combat_zone_rank;
	DWORD	combat_zone_points;
#endif
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	BYTE	language;
#endif
#ifdef ENABLE_SHINING_SYSTEM
	DWORD	adwShining[CHR_SHINING_NUM];
#endif

} TPacketGCCharacterAdditionalInfo;

typedef struct packet_add_char
{
	BYTE		header;
	DWORD		dwVID;
	float		angle;
	long		x;
	long		y;
	long		z;
	BYTE		bType;
	WORD		wRaceNum;
	WORD		bMovingSpeed;
	BYTE		bAttackSpeed;
	BYTE		bStateFlag;
	DWORD		dwAffectFlag[3];
} TPacketGCCharacterAdd;

typedef struct packet_add_char2
{
	BYTE		header;

	DWORD	   dwVID;

	char		name[CHARACTER_NAME_MAX_LEN + 1];

	float	   angle;
	long		x;
	long		y;
	long		z;

	BYTE		bType;
	WORD		wRaceNum;
	DWORD		awPart[CHR_EQUIPPART_NUM];
	WORD		bMovingSpeed;
	BYTE		bAttackSpeed;

	BYTE		bStateFlag;
	DWORD	   dwAffectFlag[3];		// ??
	BYTE		bEmpire;

	DWORD	   dwGuild;
	short	   sAlignment;
	BYTE		bPKMode;
	DWORD		dwMountVnum;
} TPacketGCCharacterAdd2;

typedef struct packet_update_char
{
	BYTE		header;
	DWORD	   dwVID;

	DWORD		awPart[CHR_EQUIPPART_NUM];
	short		bMovingSpeed;
	BYTE		bAttackSpeed;

	BYTE		bStateFlag;
	DWORD	   dwAffectFlag[3];

	DWORD		dwGuildID;
	short	   sAlignment;
#ifdef NEW_PET_SYSTEM
	DWORD		dwLevel;
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	DWORD	dwConquerorLevel;
#endif

	BYTE		bPKMode;
	DWORD		dwMountVnum;
#if defined(WJ_COMBAT_ZONE)
	BYTE	combat_zone_rank;
	DWORD	combat_zone_points;
#endif
#ifdef ENABLE_SHINING_SYSTEM
	DWORD	adwShining[CHR_SHINING_NUM];
#endif
} TPacketGCCharacterUpdate;

typedef struct packet_update_char2
{
	BYTE		header;
	DWORD	   dwVID;

	DWORD		awPart[CHR_EQUIPPART_NUM];
	short		bMovingSpeed;
	BYTE		bAttackSpeed;

	BYTE		bStateFlag;
	DWORD	   dwAffectFlag[3];

	DWORD		dwGuildID;
	short	   sAlignment;
	BYTE		bPKMode;
	DWORD		dwMountVnum;

#ifdef ENABLE_SHINING_SYSTEM
	DWORD	adwShining[CHR_SHINING_NUM];
#endif
	
} TPacketGCCharacterUpdate2;

typedef struct packet_del_char
{
	BYTE	header;
	DWORD	dwVID;
} TPacketGCCharacterDelete;

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

typedef struct packet_GlobalTime
{
	BYTE	header;
	float	GlobalTime;
} TPacketGCGlobalTime;

enum EChatType
{
	CHAT_TYPE_TALKING,  /* 그냥 채팅 */
	CHAT_TYPE_INFO,	 /* 정보 (아이템을 집었다, 경험치를 얻었다. 등) */
	CHAT_TYPE_NOTICE,   /* 공지사항 */
	CHAT_TYPE_PARTY,	/* 파티말 */
	CHAT_TYPE_GUILD,	/* 길드말 */
	CHAT_TYPE_COMMAND,	/* 명령 */
	CHAT_TYPE_SHOUT,	/* 외치기 */
	CHAT_TYPE_WHISPER,	// 서버와는 연동되지 않는 Only Client Enum
	CHAT_TYPE_BIG_NOTICE,
	CHAT_TYPE_MONARCH_NOTICE,
	// CHAT_TYPE_UNK_10,
#ifdef ENABLE_DICE_SYSTEM
	CHAT_TYPE_DICE_INFO, //11
#endif
#if defined(__CHAT_SETTINGS__)
	CHAT_TYPE_EXP_INFO,
	CHAT_TYPE_ITEM_INFO,
	CHAT_TYPE_MONEY_INFO,
#endif
#ifdef dracaryS_DUNGEON_LIB
	CHAT_TYPE_MISSION,
#endif
	CHAT_TYPE_MAX_NUM,
};

typedef struct packet_chatting
{
	BYTE	header;
	WORD	size;
	BYTE	type;
	DWORD	dwVID;
	BYTE	bEmpire;
} TPacketGCChat;

typedef struct packet_whisper   // 가변 패킷
{
	BYTE		bHeader;
	WORD		wSize;
	BYTE		bType;
	char		szNameFrom[CHARACTER_NAME_MAX_LEN + 1];
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	BYTE		bLanguage;
	BYTE		bEmpire;
#endif
} TPacketGCWhisper;

typedef struct packet_stun
{
	BYTE		header;
	DWORD		vid;
} TPacketGCStun;
#if defined(WJ_COMBAT_ZONE)
enum EPacketCGCombatZoneSubHeaderType
{
	COMBAT_ZONE_SUB_HEADER_NONE,
	COMBAT_ZONE_SUB_HEADER_ADD_LEAVING_TARGET,
	COMBAT_ZONE_SUB_HEADER_REMOVE_LEAVING_TARGET,	
	COMBAT_ZONE_SUB_HEADER_FLASH_ON_MINIMAP,
	COMBAT_ZONE_SUB_HEADER_OPEN_RANKING,
	COMBAT_ZONE_SUB_HEADER_REFRESH_SHOP,
	COMBAT_ZONE_SUB_HEADER_MP,
	COMBAT_ZONE_SUB_HEADER_BONUS_CLEAR,
	COMBAT_ZONE_SUB_HEADER_BONUS,
	COMBAT_ZONE_SUB_HEADER_TIME,

};

typedef struct SPacketCGCombatZoneRequestAction
{
	BYTE	header;
	int		action;
	int		value;
} TPacketCGCombatZoneRequestAction;

typedef struct SPacketGCCombatZoneRanking
{
	int		rank;
	char	name[12 + 1];
	int		empire;
	int		points;
} TPacketGCCombatZoneRanking;

typedef struct SPacketGCCombatZoneRankingData
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

typedef struct packet_main_character
{
	BYTE		header;
	DWORD	   dwVID;
	WORD		wRaceNum;
	char		szName[CHARACTER_NAME_MAX_LEN + 1];
	long		lX, lY, lZ;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter;

// SUPPORT_BGM
typedef struct packet_main_character2_empire
{
	BYTE		header;
	DWORD	   dwVID;
	WORD		wRaceNum;
	char		szName[CHARACTER_NAME_MAX_LEN + 1];
	long		lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter2_EMPIRE;

typedef struct packet_main_character3_bgm
{
	enum
	{
		MUSIC_NAME_MAX_LEN = 24,
	};
	BYTE		header;
	DWORD	   dwVID;
	WORD		wRaceNum;
	char		szUserName[CHARACTER_NAME_MAX_LEN + 1];
	char		szBGMName[MUSIC_NAME_MAX_LEN + 1];
	long		lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter3_BGM;

typedef struct packet_main_character4_bgm_vol
{
	enum
	{
		MUSIC_NAME_MAX_LEN = 24,
	};
	BYTE		header;
	DWORD	   dwVID;
	WORD		wRaceNum;
	char		szUserName[CHARACTER_NAME_MAX_LEN + 1];
	char		szBGMName[MUSIC_NAME_MAX_LEN + 1];
	float		fBGMVol;
	long		lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter4_BGM_VOL;
// END_OF_SUPPORT_BGM

enum EPointTypes
{
	POINT_NONE,				 // 0
	POINT_LEVEL,				// 1
	POINT_VOICE,				// 2
	POINT_EXP,				  // 3
	POINT_NEXT_EXP,			 // 4
	POINT_HP,				   // 5
	POINT_MAX_HP,			   // 6
	POINT_SP,				   // 7
	POINT_MAX_SP,			   // 8
	POINT_STAMINA,			  // 9  스테미너
	POINT_MAX_STAMINA,		  // 10 최대 스테미너

	POINT_GOLD,				 // 11
	POINT_ST,				   // 12 근력
	POINT_HT,				   // 13 체력
	POINT_DX,				   // 14 민첩성
	POINT_IQ,				   // 15 정신력
	POINT_ATT_POWER,			// 16 공격력
	POINT_ATT_SPEED,			// 17 공격속도
	POINT_EVADE_RATE,		   // 18 회피율
	POINT_MOV_SPEED,			// 19 이동속도
	POINT_DEF_GRADE,			// 20 방어등급
	POINT_CASTING_SPEED,		// 21 주문속도 (쿨다운타임*100) / (100 + 이값) = 최종 쿨다운 타임
	POINT_MAGIC_ATT_GRADE,	  // 22 마법공격력
	POINT_MAGIC_DEF_GRADE,	  // 23 마법방어력
	POINT_EMPIRE_POINT,		 // 24 제국점수
	POINT_LEVEL_STEP,		   // 25 한 레벨에서의 단계.. (1 2 3 될 때 보상, 4 되면 레벨 업)
	POINT_STAT,				 // 26 능력치 올릴 수 있는 개수
	POINT_SUB_SKILL,			// 27 보조 스킬 포인트
	POINT_SKILL,				// 28 액티브 스킬 포인트
//	POINT_SKILL_PASV,		   // 27 패시브 기술 올릴 수 있는 개수
//	POINT_SKILL_ACTIVE,		 // 28 액티브 스킬 포인트
	POINT_MIN_ATK,				// 29 최소 파괴력
	POINT_MAX_ATK,				// 30 최대 파괴력
	POINT_PLAYTIME,			 // 31 플레이시간
	POINT_HP_REGEN,			 // 32 HP 회복률
	POINT_SP_REGEN,			 // 33 SP 회복률

	POINT_BOW_DISTANCE,		 // 34 활 사정거리 증가치 (meter)

	POINT_HP_RECOVERY,		  // 35 체력 회복 증가량
	POINT_SP_RECOVERY,		  // 36 정신력 회복 증가량

	POINT_POISON_PCT,		   // 37 독 확률
	POINT_STUN_PCT,			 // 38 기절 확률
	POINT_SLOW_PCT,			 // 39 슬로우 확률
	POINT_CRITICAL_PCT,		 // 40 크리티컬 확률
	POINT_PENETRATE_PCT,		// 41 관통타격 확률
	POINT_CURSE_PCT,			// 42 저주 확률

	POINT_ATTBONUS_HUMAN,	   // 43 인간에게 강함
	POINT_ATTBONUS_ANIMAL,	  // 44 동물에게 데미지 % 증가
	POINT_ATTBONUS_ORC,		 // 45 웅귀에게 데미지 % 증가
	POINT_ATTBONUS_MILGYO,	  // 46 밀교에게 데미지 % 증가
	POINT_ATTBONUS_UNDEAD,	  // 47 시체에게 데미지 % 증가
	POINT_ATTBONUS_DEVIL,	   // 48 마귀(악마)에게 데미지 % 증가
	POINT_ATTBONUS_INSECT,	  // 49 벌레족
	POINT_ATTBONUS_FIRE,		// 50 화염족
	POINT_ATTBONUS_ICE,		 // 51 빙설족
	POINT_ATTBONUS_DESERT,	  // 52 사막족
	POINT_ATTBONUS_MONSTER,	 // 53 UNUSED0
	POINT_ATTBONUS_WARRIOR,	 // 54 UNUSED1
	POINT_ATTBONUS_ASSASSIN,	 // 55 UNUSED2
	POINT_ATTBONUS_SURA,	 // 56 UNUSED3
	POINT_ATTBONUS_SHAMAN,	 // 57 UNUSED4
	POINT_ATTBONUS_TREE,	 // 58 UNUSED5
	POINT_RESIST_WARRIOR,	 // 59 UNUSED6
	POINT_RESIST_ASSASSIN,	 // 60 UNUSED7
	POINT_RESIST_SURA,	 // 61 UNUSED8
	POINT_RESIST_SHAMAN,	 // 62 UNUSED9

	POINT_STEAL_HP,			 // 63 생명력 흡수
	POINT_STEAL_SP,			 // 64 정신력 흡수

	POINT_MANA_BURN_PCT,		// 65 마나 번

	/// 피해시 보너스 ///

	POINT_DAMAGE_SP_RECOVER,	// 66 공격당할 시 정신력 회복 확률

	POINT_BLOCK,				// 67 블럭율
	POINT_DODGE,				// 68 회피율

	POINT_RESIST_SWORD,		 // 69
	POINT_RESIST_TWOHAND,	   // 70
	POINT_RESIST_DAGGER,		// 71
	POINT_RESIST_BELL,		  // 72
	POINT_RESIST_FAN,		   // 73
	POINT_RESIST_BOW,		   // 74  화살   저항   : 대미지 감소
	POINT_RESIST_FIRE,		  // 75  화염   저항   : 화염공격에 대한 대미지 감소
	POINT_RESIST_ELEC,		  // 76  전기   저항   : 전기공격에 대한 대미지 감소
	POINT_RESIST_MAGIC,		 // 77  술법   저항   : 모든술법에 대한 대미지 감소
	POINT_RESIST_WIND,		  // 78  바람   저항   : 바람공격에 대한 대미지 감소

	POINT_REFLECT_MELEE,		// 79 공격 반사

	/// 특수 피해시 ///
	POINT_REFLECT_CURSE,		// 80 저주 반사
	POINT_POISON_REDUCE,		// 81 독데미지 감소

	/// 적 소멸시 ///
	POINT_KILL_SP_RECOVER,	  // 82 적 소멸시 MP 회복
	POINT_EXP_DOUBLE_BONUS,	 // 83
	POINT_GOLD_DOUBLE_BONUS,	// 84
	POINT_ITEM_DROP_BONUS,	  // 85

	/// 회복 관련 ///
	POINT_POTION_BONUS,		 // 86
	POINT_KILL_HP_RECOVERY,	  // 87

	POINT_IMMUNE_STUN,		  // 88
	POINT_IMMUNE_SLOW,		  // 89
	POINT_IMMUNE_FALL,		  // 90
	//////////////////

	POINT_PARTY_ATT_GRADE,	  // 91
	POINT_PARTY_DEF_GRADE,	  // 92

	POINT_ATT_BONUS,			// 93
	POINT_DEF_BONUS,			// 94

	POINT_ATT_GRADE_BONUS,			// 95
	POINT_DEF_GRADE_BONUS,			// 96
	POINT_MAGIC_ATT_GRADE_BONUS,	// 97
	POINT_MAGIC_DEF_GRADE_BONUS,	// 98

	POINT_RESIST_NORMAL_DAMAGE,		// 99

	POINT_STAT_RESET_COUNT = 112,
	POINT_HORSE_SKILL = 113,

	POINT_MALL_ATTBONUS,		// 114 공격력 +x%
	POINT_MALL_DEFBONUS,		// 115 방어력 +x%
	POINT_MALL_EXPBONUS,		// 116 경험치 +x%
	POINT_MALL_ITEMBONUS,		// 117 아이템 드롭율 x/10배
	POINT_MALL_GOLDBONUS,		// 118 돈 드롭율 x/10배
	POINT_MAX_HP_PCT,			// 119 최대생명력 +x%
	POINT_MAX_SP_PCT,			// 120 최대정신력 +x%

	POINT_SKILL_DAMAGE_BONUS,	   // 121 스킬 데미지 *(100+x)%
	POINT_NORMAL_HIT_DAMAGE_BONUS,  // 122 평타 데미지 *(100+x)%

	POINT_SKILL_DEFEND_BONUS,	   // 123 스킬 방어 데미지
	POINT_NORMAL_HIT_DEFEND_BONUS,  // 124 평타 방어 데미지
	POINT_PC_BANG_EXP_BONUS,		// 125
	POINT_PC_BANG_DROP_BONUS,	   // 126 PC방 전용 드롭률 보너스

	POINT_ENERGY = 128,				// 128 기력

	// 기력 ui 용.
	// 이렇게 하고 싶지 않았지만,
	// uiTaskBar에서는 affect에 접근할 수 없고,
	// 더구나 클라리언트에서는 blend_affect는 관리하지 않아,
	// 임시로 이렇게 둔다.
	POINT_ENERGY_END_TIME = 129,	// 129 기력 종료 시간
	POINT_COSTUME_ATTR_BONUS = 130,
	POINT_MAGIC_ATT_BONUS_PER = 131,
	POINT_MELEE_MAGIC_ATT_BONUS_PER = 132,

	POINT_RESIST_ICE = 133,
	POINT_RESIST_EARTH = 134,
	POINT_RESIST_DARK = 135,

	POINT_RESIST_CRITICAL = 136,
	POINT_RESIST_PENETRATE = 137,

#ifdef ELEMENT_NEW_BONUSES
	POINT_ENCHANT_ELEC = 182,
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
//#ifdef ENABLE_WOLFMAN_CHARACTER
	POINT_ATTBONUS_CLAW,
//#endif
#endif
#ifdef NEW_BONUS
	POINT_ATTBONUS_STONE = 196,
	POINT_ATTBONUS_BOSS = 197,
#endif
	// 클라이언트 포인트
	POINT_MIN_WEP = 200,
	POINT_MAX_WEP,
	POINT_MIN_MAGIC_WEP,
	POINT_MAX_MAGIC_WEP,
	POINT_HIT_RATE,
#ifdef ENABLE_YOHARA_SYSTEM	
	POINT_CONQUEROR_LEVEL,
	POINT_CONQUEROR_LEVEL_STEP,
	POINT_SUNGMA_STR,
	POINT_SUNGMA_HP,
	POINT_SUNGMA_MOVE,
	POINT_SUNGMA_IMMUNE,
	POINT_CONQUEROR_POINT,
	POINT_CONQUEROR_EXP,
	POINT_CONQUEROR_NEXT_EXP,
	POINT_HIT_PCT,
	POINT_RESIST_MOUNT_FALL,
	POINT_MONSTER_DEFEND_BONUS,
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

#ifdef ENABLE_GEM_SYSTEM
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
	//POINT_MAX_NUM = 255,=>stdafx.h 로/
};

typedef struct packet_points
{
	BYTE		header;
   	long long	points[POINT_MAX_NUM];

} TPacketGCPoints;

typedef struct packet_point_change
{
	BYTE		header;
	DWORD		dwVID;
	BYTE		Type;
	long long	amount;
	long long	value;
} TPacketGCPointChange;

typedef struct packet_motion
{
	BYTE		header;
	DWORD		vid;
	DWORD		victim_vid;
	WORD		motion;
} TPacketGCMotion;

#if defined(GAIDEN)
struct TPacketGCItemDelDeprecate
{
	TPacketGCItemDelDeprecate() :
		header(HEADER_GC_ITEM_DEL),
		pos(0),
		vnum(0),
		count(0)
	{
		memset(&alSockets, 0, sizeof(alSockets));
		memset(&aAttr, 0, sizeof(aAttr));
#ifdef ENABLE_YOHARA_SYSTEM
	memset(&randomAttr, 0, sizeof(randomAttr));
#endif

	}

	BYTE	header;
	WORD	pos;
	DWORD   vnum;
	BYTE	count;
	long	alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemAttribute randomAttr[GLOVE_ATTR_MAX_NUM];
#endif
};

typedef struct packet_set_item
{
	BYTE		header;
	WORD		pos;
	DWORD		vnum;
	BYTE		count;
	DWORD		flags;	// 플래그 추가
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemAttribute randomAttr[GLOVE_ATTR_MAX_NUM];
#endif
#ifdef ENABLE_SOULBIND_SYSTEM
	long		bind;
#endif
} TPacketGCItemSet;

typedef struct packet_item_del
{
	BYTE		header;
	BYTE		pos;
} TPacketGCItemDel;
#else
typedef struct packet_set_item
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		vnum;
	DWORD		count;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
#ifdef ENABLE_SOULBIND_SYSTEM
	long		bind;
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char		name[CItemData::ITEM_NAME_MAX_LEN+1];
#endif
} TPacketGCItemSet;

typedef struct packet_set_item2
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		vnum;
	DWORD		count;
	DWORD		flags;
	DWORD		anti_flags;
	bool		highlight;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
#ifdef ENABLE_SOULBIND_SYSTEM
	long		bind;
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char		name[CItemData::ITEM_NAME_MAX_LEN+1];
#endif
} TPacketGCItemSet2;
#endif

typedef struct packet_item_del
{
	BYTE		header;
	BYTE		pos;
} TPacketGCItemDel;

typedef struct packet_use_item
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		ch_vid;
	DWORD		victim_vid;

	DWORD		vnum;
} TPacketGCItemUse;
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
typedef struct packet_update_item
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		count;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
#ifdef ENABLE_SOULBIND_SYSTEM
	long		bind;
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char		name[CItemData::ITEM_NAME_MAX_LEN+1];
#endif
} TPacketGCItemUpdate;

#ifdef USE_EXPANDED_ITEM_REFRESH
typedef struct SPacketGCItemUpdateCount
{
	BYTE		header;
	TItemPos	pos;
	DWORD	   count;
} TPacketGCItemUpdateCount;
#endif

typedef struct packet_ground_add_item
{
	BYTE	bHeader;
	long	lX;
	long	lY;
	long	lZ;
	DWORD	dwVID;
	DWORD	dwVnum;
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	long	alSockets[ITEM_SOCKET_SLOT_MAX_NUM];//3 
	TPlayerItemAttribute aAttrs[ITEM_ATTRIBUTE_SLOT_MAX_NUM];//7
	unsigned short itemCount;
	int		ownerTime;
#endif
	
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	packet_ground_add_item()
	{
		memset(&alSockets, 0, sizeof(alSockets));
		memset(&aAttrs, 0, sizeof(aAttrs));
	}
#endif
} TPacketGCItemGroundAdd;

typedef struct packet_ground_del_item
{
	BYTE		header;
	DWORD		vid;
} TPacketGCItemGroundDel;

typedef struct packet_item_ownership
{
	BYTE		bHeader;
	DWORD	   dwVID;
	char		szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCItemOwnership;

typedef struct packet_quickslot_add
{
	BYTE		header;
	BYTE		pos;
	TQuickSlot	slot;
} TPacketGCQuickSlotAdd;

typedef struct packet_quickslot_del
{
	BYTE		header;
	BYTE		pos;
} TPacketGCQuickSlotDel;

typedef struct packet_quickslot_swap
{
	BYTE		header;
	BYTE		pos;
	BYTE		change_pos;
} TPacketGCQuickSlotSwap;

typedef struct packet_shop_start
{
	//DWORD   owner_vid;//missing variable.
	struct packet_shop_item		items[SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCShopStart;

typedef struct packet_shop_start_ex // 다음에 TSubPacketShopTab* shop_tabs 이 따라옴.
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
	BYTE						pos;
	struct packet_shop_item		item;
} TPacketGCShopUpdateItem;

typedef struct packet_shop_update_price
{
	long long iElkAmount;	

} TPacketGCShopUpdatePrice;

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

#ifdef ENABLE_SPECIAL_STORAGE
	SHOP_SUBHEADER_GC_NOT_ENOUGH_COUNT,
#endif
#ifdef ENABLE_ZODIAC_MISSION
	SHOP_SUBHEADER_GC_NOT_ENOUGH_LUCKY,
#endif
};

typedef struct packet_shop
{
	BYTE		header;
	WORD		size;
	BYTE		subheader;
} TPacketGCShop;

typedef struct SPacketExchange
{
	BYTE					header;
	BYTE					subheader;
	BYTE					is_me;
	long long				arg1;
	TItemPos				arg2;
	DWORD					arg3;
#ifdef WJ_ENABLE_TRADABLE_ICON
	TItemPos				arg4;
#endif
	long					alValues[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute	aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom	aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
#ifdef ENABLE_NEW_NAME_ITEM
	char					name[CItemData::ITEM_NAME_MAX_LEN+1];
#endif
} TPacketExchange;

typedef struct packet_position
{
	BYTE		header;
	DWORD		vid;
	BYTE		position;
} TPacketGCPosition;

typedef struct packet_ping
{
	BYTE		header;
} TPacketGCPing;

typedef struct packet_pong
{
	BYTE		bHeader;
} TPacketCGPong;

typedef struct packet_script
{
	BYTE		header;
	WORD		size;
	BYTE		skin;
	WORD		src_size;
} TPacketGCScript;

typedef struct packet_target
{
	BYTE		header;
	DWORD	   dwVID;
	BYTE		bHPPercent;
#ifdef ENABLE_HIDE_COSTUME_SYSTEM
	char		dwCostumeHideID[255 + 1];
#endif
#ifdef ENABLE_VIEW_TARGET_DECIMAL_HP
	int		iMinHP;
	int		iMaxHP;
#endif
#ifdef ELEMENT_TARGET
	BYTE	bElement;
#endif

} TPacketGCTarget;

#ifdef ENABLE_SEND_TARGET_INFO
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

typedef struct packet_damage_info
{
	BYTE header;
	DWORD dwVictimVID;
	DWORD dwAttackerVID;
	BYTE flag;
	int  damage;
} TPacketGCDamageInfo;

typedef struct packet_mount
{
	BYTE		header;
	DWORD	   vid;
	DWORD	   mount_vid;
	BYTE		pos;
	DWORD		_x, _y;
} TPacketGCMount;

typedef struct packet_change_speed
{
	BYTE		header;
	DWORD		vid;
	WORD		moving_speed;
} TPacketGCChangeSpeed;

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

enum
{
	QUEST_SEND_IS_BEGIN		 = 1 << 0,
	QUEST_SEND_TITLE			= 1 << 1,  // 28자 까지
	QUEST_SEND_CLOCK_NAME	   = 1 << 2,  // 16자 까지
	QUEST_SEND_CLOCK_VALUE	  = 1 << 3,
	QUEST_SEND_COUNTER_NAME	 = 1 << 4,  // 16자 까지
	QUEST_SEND_COUNTER_VALUE	= 1 << 5,
	QUEST_SEND_ICON_FILE		= 1 << 6,  // 24자 까지
};

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
#ifdef ENABLE_QUEST_RENEWAL
	BYTE type;
	bool is_confirmed;
#endif
} TPacketGCQuestInfo;

typedef struct packet_quest_confirm
{
	BYTE header;
	char msg[64+1];
	long timeout;
	DWORD requestPID;
} TPacketGCQuestConfirm;

typedef struct packet_attack
{
	BYTE		header;
	DWORD	   dwVID;
	DWORD	   dwVictimVID;	// 적 VID
	BYTE		bType;		  // 공격 유형
} TPacketGCAttack;

typedef struct packet_c2c
{
	BYTE		header;
	WORD		wSize;
} TPacketGCC2C;

typedef struct packetd_sync_position_element
{
	DWORD	   dwVID;
	long		lX;
	long		lY;
} TPacketGCSyncPositionElement;

typedef struct packetd_sync_position
{
	BYTE		bHeader;
	WORD		wSize;
} TPacketGCSyncPosition;

typedef struct packet_ownership
{
	BYTE				bHeader;
	DWORD			   dwOwnerVID;
	DWORD			   dwVictimVID;
} TPacketGCOwnership;

#define	SKILL_MAX_NUM 255

typedef struct packet_skill_level
{
	BYTE		bHeader;
	BYTE		abSkillLevels[SKILL_MAX_NUM];
} TPacketGCSkillLevel;

typedef struct SPlayerSkill
{
	BYTE bMasterType;
	BYTE bLevel;
	time_t tNextRead;
} TPlayerSkill;

typedef struct packet_skill_level_new
{
	BYTE bHeader;
	TPlayerSkill skills[SKILL_MAX_NUM];
} TPacketGCSkillLevelNew;

// fly
typedef struct packet_fly
{
	BYTE		bHeader;
	BYTE		bType;
	DWORD	   dwStartVID;
	DWORD	   dwEndVID;
} TPacketGCCreateFly;

enum EPVPModes
{
	PVP_MODE_NONE,
	PVP_MODE_AGREE,
	PVP_MODE_FIGHT,
	PVP_MODE_REVENGE,
};

typedef struct packet_duel_start
{
	BYTE	header ;
	WORD	wSize ;	// DWORD가 몇개? 개수 = (wSize - sizeof(TPacketGCPVPList)) / 4
} TPacketGCDuelStart ;

typedef struct packet_pvp
{
	BYTE		header;
	DWORD		dwVIDSrc;
	DWORD		dwVIDDst;
	BYTE		bMode;
} TPacketGCPVP;

typedef struct packet_skill_cooltime_end
{
	BYTE		header;
	BYTE		bSkill;
} TPacketGCSkillCoolTimeEnd;

typedef struct packet_warp
{
	BYTE			bHeader;
	long			lX;
	long			lY;
	long			lAddr;
	WORD			wPort;
} TPacketGCWarp;

typedef struct packet_party_invite
{
	BYTE header;
	DWORD leader_pid;
} TPacketGCPartyInvite;

typedef struct packet_party_add
{
	BYTE header;
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCPartyAdd;

typedef struct packet_party_update
{
	BYTE header;
	DWORD pid;
	BYTE state;
	BYTE percent_hp;
	short affects[PARTY_AFFECT_SLOT_MAX_NUM];
} TPacketGCPartyUpdate;

typedef struct packet_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketGCPartyRemove;

typedef TPacketCGSafeboxCheckout TPacketGCSafeboxCheckout;
typedef TPacketCGSafeboxCheckin TPacketGCSafeboxCheckin;

typedef struct packet_safebox_wrong_password
{
	BYTE		bHeader;
} TPacketGCSafeboxWrongPassword;

typedef struct packet_safebox_size
{
	BYTE bHeader;
	BYTE bSize;
} TPacketGCSafeboxSize;

typedef struct packet_safebox_money_change
{
	BYTE bHeader;
	DWORD dwMoney;
} TPacketGCSafeboxMoneyChange;

typedef struct command_empire
{
	BYTE		bHeader;
	BYTE		bEmpire;
} TPacketCGEmpire;

typedef struct packet_empire
{
	BYTE		bHeader;
	BYTE		bEmpire;
} TPacketGCEmpire;

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

typedef struct paryt_parameter
{
	BYTE		bHeader;
	BYTE		bDistributeMode;
} TPacketGCPartyParameter;

//////////////////////////////////////////////////////////////////////////
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
	GUILD_SUBHEADER_GC_WAR_POINT,
	GUILD_SUBHEADER_GC_MONEY_CHANGE,
};

typedef struct packet_guild
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCGuild;

// SubHeader - Grade
enum
{
	GUILD_AUTH_ADD_MEMBER	   = (1 << 0),
	GUILD_AUTH_REMOVE_MEMBER	= (1 << 1),
	GUILD_AUTH_NOTICE		   = (1 << 2),
	GUILD_AUTH_SKILL			= (1 << 3),
};

typedef struct packet_guild_sub_grade
{
	char grade_name[GUILD_GRADE_NAME_MAX_LEN+1]; // 8+1 길드장, 길드원 등의 이름
	BYTE auth_flag;
} TPacketGCGuildSubGrade;

typedef struct packet_guild_sub_member
{
	DWORD pid;
	BYTE byGrade;
	BYTE byIsGeneral;
	BYTE byJob;
	BYTE byLevel;
	DWORD dwOffer;
	BYTE byNameFlag;
// if NameFlag is TRUE, name is sent from server.
//	char szName[CHARACTER_ME_MAX_LEN+1];
} TPacketGCGuildSubMember;

typedef struct packet_guild_sub_info
{
	WORD member_count;
	WORD max_member_count;
	DWORD guild_id;
	DWORD master_pid;
	DWORD exp;
	BYTE level;
	char name[GUILD_NAME_MAX_LEN+1];
	DWORD gold;
	BYTE hasLand;
} TPacketGCGuildInfo;

enum EGuildWarState
{
	GUILD_WAR_NONE,
	GUILD_WAR_SEND_DECLARE,
	GUILD_WAR_REFUSE,
	GUILD_WAR_RECV_DECLARE,
	GUILD_WAR_WAIT_START,
	GUILD_WAR_CANCEL,
	GUILD_WAR_ON_WAR,
	GUILD_WAR_END,

	GUILD_WAR_DURATION = 2*60*60, // 2시간
};

typedef struct packet_guild_war
{
	DWORD	   dwGuildSelf;
	DWORD	   dwGuildOpp;
	BYTE		bType;
	BYTE		bWarState;
} TPacketGCGuildWar;

typedef struct SPacketGuildWarPoint
{
	DWORD dwGainGuildID;
	DWORD dwOpponentGuildID;
	long lPoint;
} TPacketGuildWarPoint;

// SubHeader - Dungeon
enum
{
	DUNGEON_SUBHEADER_GC_TIME_ATTACK_START = 0,
	DUNGEON_SUBHEADER_GC_DESTINATION_POSITION = 1,
};

typedef struct packet_dungeon
{
	BYTE		bHeader;
	WORD		size;
	BYTE		subheader;
} TPacketGCDungeon;



typedef struct SPacketGCTime
{
	BYTE		bHeader;
	time_t	  time;
} TPacketGCTime;

enum
{
	WALKMODE_RUN,
	WALKMODE_WALK,
};

typedef struct SPacketGCWalkMode
{
	BYTE		header;
	DWORD	   vid;
	BYTE		mode;
} TPacketGCWalkMode;

typedef struct SPacketGCChangeSkillGroup
{
	BYTE		header;
	BYTE		skill_group;
} TPacketGCChangeSkillGroup;

struct TMaterial
{
	DWORD vnum;
	int count;
};

typedef struct SRefineTable
{
	DWORD		src_vnum;
	DWORD		result_vnum;
	BYTE		material_count;
	int			cost;
	int			prob;
	BYTE		special_storage;
	TMaterial	materials[REFINE_MATERIAL_MAX_NUM];
} TRefineTable;

typedef struct STPacketGCRefineInformation
{
	BYTE			header;
	BYTE			type;
	WORD			pos;
	TRefineTable	refine_table;
} TPacketGCRefineInformation;

enum SPECIAL_EFFECT
{
	SE_NONE,
	SE_HPUP_RED,
	SE_SPUP_BLUE,
	SE_SPEEDUP_GREEN,
	SE_DXUP_PURPLE,
	SE_CRITICAL,
	SE_PENETRATE,
	SE_BLOCK,
	SE_DODGE,
	SE_CHINA_FIREWORK,
	SE_SPIN_TOP,
	SE_SUCCESS,
	SE_FAIL,
	SE_FR_SUCCESS,
	SE_LEVELUP_ON_14_FOR_GERMANY,	//레벨업 14일때 ( 독일전용 )
	SE_LEVELUP_UNDER_15_FOR_GERMANY,//레벨업 15일때 ( 독일전용 )
	SE_PERCENT_DAMAGE1,
	SE_PERCENT_DAMAGE2,
	SE_PERCENT_DAMAGE3,
	SE_AUTO_HPUP,
	SE_AUTO_SPUP,
	SE_EQUIP_RAMADAN_RING,			// 초승달의 반지를 착용하는 순간에 발동하는 이펙트
	SE_EQUIP_HALLOWEEN_CANDY,		// 할로윈 사탕을 착용(-_-;)한 순간에 발동하는 이펙트
	SE_EQUIP_HAPPINESS_RING,		// 크리스마스 행복의 반지를 착용하는 순간에 발동하는 이펙트
	SE_EQUIP_LOVE_PENDANT,		// 발렌타인 사랑의 팬던트(71145) 착용할 때 이펙트 (발동이펙트임, 지속이펙트 아님)
#if defined(WJ_COMBAT_ZONE)
	SE_COMBAT_ZONE_POTION,
#endif
	#ifdef ENABLE_SASH_SYSTEM
	SE_EFFECT_SASH_SUCCEDED,
	SE_EFFECT_SASH_EQUIP,
	#endif
#ifdef VERSION_162_ENABLED
	SE_EFFECT_HEALER,
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
	SE_EFFECT_SNAKE_REGEN,
#endif
#ifdef ENABLE_VOICE_CHAT
	SE_VOICE_CHAT,
#endif
};

typedef struct SPacketGCSpecialEffect
{
	BYTE header;
	BYTE type;
	DWORD vid;
} TPacketGCSpecialEffect;

#ifndef USE_MARK_ATLAS_BINARY
typedef struct SPacketGCNPCPosition
{
	BYTE header;
	WORD size;
	WORD count;
} TPacketGCNPCPosition;

struct TNPCPosition
{
	BYTE bType;
	DWORD mobVnum;
	long x;
	long y;
	int regenTime;
};
#endif

typedef struct SPacketGCChangeName
{
	BYTE header;
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCChangeName;

enum EBlockAction
{
	BLOCK_EXCHANGE			  = (1 << 0),
	BLOCK_PARTY_INVITE		  = (1 << 1),
	BLOCK_GUILD_INVITE		  = (1 << 2),
	BLOCK_WHISPER			   = (1 << 3),
	BLOCK_MESSENGER_INVITE	  = (1 << 4),
	BLOCK_PARTY_REQUEST		 = (1 << 5),
};

typedef struct packet_china_matrixd_card
{
	BYTE	bHeader;
	DWORD	dwRows;
	DWORD	dwCols;
} TPacketGCChinaMatrixCard;

typedef struct packet_runup_matrixd_quiz
{
	BYTE	bHeader;
	char	szQuiz[RUNUP_MATRIX_QUIZ_MAX_LEN + 1];
} TPacketGCRunupMatrixQuiz;

typedef struct packet_newcibn_passpod_request
{
	BYTE	bHeader;
} TPacketGCNEWCIBNPasspodRequest;

typedef struct packet_newcibn_passpod_failure
{
	BYTE	bHeader;
	char	szMessage[NEWCIBN_PASSPOD_FAILURE_MAX_LEN + 1];
} TPacketGCNEWCIBNPasspodFailure;

typedef struct packet_login_key
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
} TPacketGCLoginKey;

typedef struct packet_auth_success
{
	BYTE		bHeader;
	DWORD	   dwLoginKey;
	BYTE		bResult;
} TPacketGCAuthSuccess;

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
	DWORD	count;
	long	alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
	TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif
} TEquipmentItemSet;

typedef struct pakcet_view_equip
{
	BYTE header;
	DWORD dwVID;
	TEquipmentItemSet equips[WEAR_MAX_NUM];
} TPacketGCViewEquip;

typedef struct
{
	DWORD	   dwID;
	long		x, y;
	long		width, height;
	DWORD	   dwGuildID;
} TLandPacketElement;

typedef struct packet_land_list
{
	BYTE		header;
	WORD		size;
} TPacketGCLandList;

typedef struct
{
	BYTE		bHeader;
	long		lID;
	char		szTargetName[32+1];
} TPacketGCTargetCreate;

enum
{
	CREATE_TARGET_TYPE_NONE,
	CREATE_TARGET_TYPE_LOCATION,
	CREATE_TARGET_TYPE_CHARACTER,
#if defined(WJ_COMBAT_ZONE)	
	CREATE_TARGET_TYPE_COMBAT_ZONE = 4,
#endif
};

typedef struct
{
	BYTE		bHeader;
	long		lID;
	char		szTargetName[32+1];
	DWORD		dwVID;
	BYTE		byType;
} TPacketGCTargetCreateNew;

typedef struct
{
	BYTE		bHeader;
	long		lID;
	long		lX, lY;
	BYTE		lType;
} TPacketGCTargetUpdate;

typedef struct
{
	BYTE		bHeader;
	long		lID;
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

typedef struct SPacketAffectElement
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

typedef struct SPacketGCAffectAdd
{
	BYTE 					bHeader;
	TPacketAffectElement	elem;
} TPacketGCAffectAdd;

typedef struct SPacketGCAffectRemove
{
	BYTE	bHeader;
	DWORD	dwType;
	BYTE	bApplyOn;
} TPacketGCAffectRemove;

typedef struct packet_mall_open
{
	BYTE bHeader;
	BYTE bSize;
} TPacketGCMallOpen;

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
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	BYTE byLovePoint;
} TPacketGCLoverInfo;

typedef struct packet_love_point_update
{
	BYTE bHeader;
	BYTE byLovePoint;
} TPacketGCLovePointUpdate;

typedef struct packet_dig_motion
{
	BYTE header;
	DWORD vid;
	DWORD target_vid;
	BYTE count;
} TPacketGCDigMotion;

typedef struct SPacketGCOnTime
{
	BYTE header;
	int ontime;	 // sec
} TPacketGCOnTime;

typedef struct SPacketGCResetOnTime
{
	BYTE header;
} TPacketGCResetOnTime;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Client To Client

typedef struct packet_state
{
	BYTE			bHeader;
	BYTE			bFunc;
	BYTE			bArg;
	BYTE			bRot;
	DWORD			dwVID;
	DWORD			dwTime;
	TPixelPosition	kPPos;
} TPacketCCState;

// AUTOBAN
typedef struct packet_autoban_quiz
{
	BYTE bHeader;
	BYTE bDuration;
	BYTE bCaptcha[64*32];
	char szQuiz[256];
} TPacketGCAutoBanQuiz;
// END_OF_AUTOBAN

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

typedef struct SPacketGCSpecificEffect
{
	BYTE header;
	DWORD vid;
	char effect_file[128];
} TPacketGCSpecificEffect;

// 용혼석
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
	DS_SUB_HEADER_DO_UPGRADE,
	DS_SUB_HEADER_DO_IMPROVEMENT,
	DS_SUB_HEADER_DO_REFINE,
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
	TItemPos ItemGrid[DS_REFINE_WINDOW_MAX_NUM];
} TPacketCGDragonSoulRefine;

typedef struct SPacketGCDragonSoulRefine
{
	SPacketGCDragonSoulRefine() : header(HEADER_GC_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos Pos;
} TPacketGCDragonSoulRefine;

typedef struct SChannelStatus
{
	short nPort;
	BYTE bStatus;
} TChannelStatus;

#ifdef ENABLE_SASH_SYSTEM
enum ESashInfo
{
	SASH_ABSORPTION_SOCKET = 0,
	SASH_ABSORBED_SOCKET = 1,
	SASH_CLEAN_ATTR_VALUE0 = 7,
	SASH_WINDOW_MAX_MATERIALS = 2,
};

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

typedef struct SSashMaterial
{
	BYTE	bHere;
	WORD	wCell;
} TSashMaterial;

typedef struct SSashResult
{
	DWORD	dwItemVnum;
	DWORD	dwMinAbs;
	DWORD	dwMaxAbs;
} TSashResult;
#endif
// @fixme007 length 2
typedef struct packet_unk_213
{
	BYTE bHeader;
	BYTE bUnk2;
} TPacketGCUnk213;

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

typedef struct  SFDateRanking
{
	int 	vnum_mob_r;
	int 	type;
	char 	name_r[24];
	int 	extra_r;

}TFDateInfoRanking;

typedef struct SFDateMision
{
	
	int 	vnum_mob_mision;
	int 	count_mob_mision;
	int 	count_mob_a_mision;
	int 	status_mision;
}TFDateMision;

typedef struct SPacketGCDungeonInfoReceive
{
	SPacketGCDungeonInfoReceive() : header(HEADER_GC_DUNGEON_INFO_SYSTEM)
	{}
	BYTE header;
	BYTE bSubType;

	SFDateInfoPacket dateinfo;
	SFDateRanking dateinfo_ranking;
	SFDateMision		dateinfom;

} TPacketGCDungeonInfoReceive;
#endif

#ifdef ENABLE_KINGDOMS_WAR
enum
{
	KW_EMPIRE_MAX_NUM = 4,
	HEADER_GC_KINGDOMSWAR = 218,
	KINGDOMSWAR_SUBHEADER_GC_OPEN = 0,
	KINGDOMSWAR_SUBHEADER_GC_REFRESH,
};

typedef struct SPacketKingdomWar
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	int		iKills[KW_EMPIRE_MAX_NUM - 1];
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

typedef struct SPacketGCShopSign
{
	BYTE		bHeader;
	DWORD	   dwVID;
	char		szSign[SHOP_SIGN_MAX_LEN + 1];
} TPacketGCShopSign;

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
	char		playerName[CHARACTER_NAME_MAX_LEN + 1];
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
	EVENT_BATTLEROYALE = 20,
};
#endif
#ifdef ENABLE_SHOW_CHEST_DROP
typedef struct SPacketGCChestDrop
{
	BYTE	header;
	DWORD	size;
} TPacketGCChestDrop;
#endif

#ifdef ENABLE_NEW_DETAILS_GUI
enum
{
	KILL_SHINSO,
	KILL_JINNO,
	KILL_CHUNJO,
	KILL_ALLPLAYER,
	KILL_DUELWON,
	KILL_DUELLOST,
	KILL_MONSTER,
	KILL_METINSTONE,
	KILL_BOSSES,
	KILL_MAX_NUM,
};
typedef struct SPacketGCKillLOG
{
	BYTE	header;
	long	kill_log[KILL_MAX_NUM];
} TPacketGCKillLOG;
#endif

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
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
typedef struct SNewRegen
{
	long x, y;
	BYTE day, hour, minute, second;
	long safeRange;
	DWORD mob_vnum;
	BYTE hours_range;
	BYTE minute_range;
	int leftTime;
} TNewRegen;
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
	long	alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
	bool	isOwnerItem;
}TExchangeLogItem;
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
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
#endif

#ifdef ENABLE_IKASHOP_RENEWAL
//ACTIONS PACKETS
typedef struct
{
	BYTE header;
#ifdef ENABLE_LARGE_DYNAMIC_PACKETS
	int size;
#else
	WORD size;
#endif
	BYTE subheader;
} TPacketGCNewOfflineshop;

typedef struct {
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketCGNewOfflineShop;

namespace ikashop
{
	typedef struct SPlayerItem
	{
		DWORD	id;
		BYTE	window;
		WORD	pos;
		DWORD	count;

		DWORD	vnum;
		long	alSockets[ITEM_SOCKET_SLOT_MAX_NUM];

		TPlayerItemAttribute	aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
		TPlayerItemApplyRandom aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
#endif


		DWORD	owner;

#ifdef __SOULBINDING_SYSTEM__
		long	bind;
#endif

#ifdef ENABLE_NEW_NAME_ITEM
		char	name[CItemData::ITEM_NAME_MAX_LEN + 1];
#endif
	} TPlayerItem;

	//patch 08-03-2020
	enum class ExpirationType
	{
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
		long long GetTotalYangAmount() const {
			return yang
#ifdef ENABLE_CHEQUE_SYSTEM
				+ static_cast<long long>(cheque) * YANG_PER_CHEQUE
#endif
				;
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

	struct TOfferInfo
	{
		DWORD offerid;
		DWORD ownerid;
		DWORD itemid;
		DWORD buyerid;
		TPriceInfo price;
		bool notified;
		bool accepted;
		char buyername[CHARACTER_NAME_MAX_LEN + 1];
		int64_t datetime;
	};

#ifdef ENABLE_IKASHOP_ENTITIES
	struct TShopSpawn
	{
		int x;
		int y;
		int map;
	};
#endif

	//AUCTION
	struct TAuctionInfo : public TShopItem
	{
		char  ownername[CHARACTER_NAME_MAX_LEN + 1];
		DWORD duration;
#ifdef ENABLE_IKASHOP_ENTITIES
#ifdef EXTEND_IKASHOP_ULTIMATE
		TShopSpawn spawn;
#endif
#endif
	};


	struct TAuctionOfferInfo
	{
		TPriceInfo	price;
		DWORD ownerid;
		DWORD buyerid;
		char buyername[CHARACTER_NAME_MAX_LEN + 1];
		bool seenflag;
		int64_t datetime;
	};

	struct TValutesInfo
	{
		long long yang = 0;

		void operator +=(const TValutesInfo& r)
		{
			yang += r.yang;
		}

		void operator -=(const TValutesInfo& r)
		{
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

	struct TFilterInfo
	{
#ifdef USE_OFFLINESHOP_SEARCH_V2
		int32_t type;
		int32_t subtype;
		char name[CItemData::ITEM_NAME_MAX_LEN];
		bool exact;
		uint32_t vnum;
		bool player;
		uint32_t pid;
		uint32_t antiflags;
#else
		BYTE type;
		BYTE subtype;
		char name[CItemData::ITEM_NAME_MAX_LEN];
#endif
		TPriceInfo pricestart;
		TPriceInfo pricend;
		int levelstart;
		int levelend;
		TPlayerItemAttribute attrs[ITEM_ATTRIBUTE_SLOT_NORM_NUM];
		int sashGrade;
		int alchemyGrade;
	};

	struct TShopItemInfo
	{
		TItemPos pos;
		int destpos;
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
		TPriceInfo actual_best;
		DWORD offercount;
#ifdef EXTEND_IKASHOP_ULTIMATE
		long long priceavg;
#endif
	};

	struct TOfferListElement : public TOfferInfo
	{
		TShopItem item;
		char shopname[OFFLINE_SHOP_NAME_MAX_LEN];
		bool incoming;
#ifdef EXTEND_IKASHOP_ULTIMATE
		long long priceavg;
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

		SUBHEDAER_GC_POPUP_MESSAGE,

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
		int32_t	 page;
		size_t	  itemcount;
		size_t	  pagecount;
	} TGCShopSearchPacket;
#endif

	struct TSubPacketGCShopOpen
	{
		TShopInfo shop;
	};

	struct TSubPacketGCShopOpenOwner
	{
		TShopInfo shop;
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
		int x, y, z;
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
#ifdef EXTEND_IKASHOP_ULTIMATE
		bool isAuction = false;
#endif
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

typedef struct SGCRankInfo
{
	BYTE	bHeader;
	char 	szName[CHARACTER_NAME_MAX_LEN + 1];
	int		iMode, iPos, bEmpire, iMyPos;
	int64_t	lValue;
	BYTE	bLevel;
	char	szGuildName[GUILD_NAME_MAX_LEN + 1];
} TPacketGCRankInfo;

#ifdef ENABLE_AURA_SYSTEM
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

#if defined(__BL_BATTLE_ROYALE__)
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

#if defined(ENABLE_VOICE_CHAT)
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

#pragma pack(pop)
