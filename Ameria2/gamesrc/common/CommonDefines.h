#pragma once

enum eCommonDefines
{
	//DEFINES						//VALUE			//DESCRIPTION								// DEFAULT
	MAP_ALLOW_LIMIT					= 40,			// Maps each Core							// 32
	SUNGMA_ATTR_PROB				= 40,			// Sungma Items Change of Attr				// 40
};

/*
#define ENABLE_CSHIELD
#ifdef ENABLE_CSHIELD
	//#define ENABLE_AUTOBAN // enables autoban if player has too many CShield Async reports # no activar

	#define ENABLE_CHECK_ATTACKSPEED_HACK //checks if player is attacking too fast
	#define ENABLE_CHECK_MOVESPEED_HACK //checks if player is moving too fast
	#define ENABLE_CHECK_WAIT_HACK // checks if player is using waithack


	// enable this checks if you have time to test it

	//#define ENABLE_CHECK_PICKUP_HACK 100 //checks if player using pickup hack //important: check CPythonPlayer::SendClickItemPacket for pickup time. if you dont have that just disable this define # no activar
	#define ENABLE_CHECK_GHOSTMODE //checks if player is using ghostmode
	//#define ENABLE_CHECK_WALLHACK //checks player position for wallhack # no activar

#endif
*/
#define VOTE_BUFF_RENEWAL
#define ENABLE_FISHING_GAME

#define HAPPY_HOUR_EVENT
#define ENABLE_LOCK_ATTR
#define ENABLE_COPY_ATTR_ITEM
#define RENEWAL_MISSION_BOOKS
#define ENABLE_MINI_GAME_CATCH_KING					// Catch the King
#define ENABLE_VOTE4BUFF
#define ENABLE_DUNGEON_BOSS_ICON_IN_MAP
#define __GEM_SYSTEM__
#define ENABLE_MOB_MOVE_SPEED
#define ENABLE_EXCHANGE_LOG
// #define ENABLE_FISH_GAME
#define ENABLE_CHECK_GHOSTMODE
#define ENABLE_ADD_REALTIME_AFFECT
#define ENABLE_ULTIMATE_REGEN
#define ENABLE_TRACK_WINDOW
#define __HIT_LIMITER_ENABLE__
#define ENABLE_MELEY_LAIR_DUNGEON
#define ENABLE_ZODIAC_MISSION
#define ENABLE_MOUNT_SKIN
#define ENABLE_MULTI_CHEST
//#define ENABLE_BOSS_KILL_FLOOD
#define ENABLE_ANTI_PACKET_FLOOD
#define ENABLE_FLOOD_PRETECTION
#define ENABLE_DS_SET
#define ENABLE_SPECIAL_COSTUME_ATTR
//#define ENABLE_VERSION_SYSTEM
#define ENABLE_NEW_DETAILS_GUI
#define FULL_MAX_YANG
#define ENABLE_NEW_NAME_ITEM
#define ENABLE_NEW_PET_SYSTEM
#define NEW_COMMON_DROP
#define ENABLE_MULTI_FARM_BLOCK
#define ENABLE_SHOW_CHEST_DROP
#define NEW_BONUS
#define ENABLE_BIYOLOG
#define ENABLE_DEFENSAWE_SHIP
#define __CHAT_SETTINGS__
#define ENABLE_EVENT_MANAGER
//#define ENABLE_BATTLE_PASS
#define ENABLE_AUTO_SELECT_SKILL
#define ENABLE_DRAGON_LAIR
#define ENABLE_CUBE_RENEWAL_COPY
#define ENABLE_ANTI_EXP
#define ENABLE_ITEMSHOP
#ifdef ENABLE_ITEMSHOP
#define USE_ITEMSHOP_RENEWED
#endif
#define __FAKE_PLAYER__


#define DISABLE_OLD_MINUS
#define ENABLE_CHANNEL_CHANGE

#define dracaryS_DUNGEON_LIB

#define ENABLE_DS_GRADE_MYTH

//////////////////////////////////////////////////////////////////////////
// ### General Features ###
#define ENABLE_D_NJGUILD
#define ENABLE_FULL_NOTICE
#define ENABLE_NEWSTUFF
#define ENABLE_PORT_SECURITY
#define ENABLE_NEW_AUTOPOTION
#define ENABLE_BELT_INVENTORY_EX

// ### General Features ###
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ### CommonDefines Systems ###
#define ENABLE_WOLFMAN_CHARACTER
#ifdef ENABLE_WOLFMAN_CHARACTER
#define USE_MOB_BLEEDING_AS_POISON
#define USE_MOB_CLAW_AS_DAGGER
// #define USE_ITEM_BLEEDING_AS_POISON
// #define USE_ITEM_CLAW_AS_DAGGER
//#define USE_WOLFMAN_STONES
//#define USE_WOLFMAN_BOOKS
#endif

#define ENABLE_PLAYER_PER_ACCOUNT5
#define ENABLE_DICE_SYSTEM
#define ENABLE_EXTEND_INVEN_SYSTEM
#define ENABLE_2LOOT

#define ENABLE_MOUNT_COSTUME_SYSTEM
#define ENABLE_WEAPON_COSTUME_SYSTEM

// #define ENABLE_MAGIC_REDUCTION_SYSTEM
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
// #define USE_MAGIC_REDUCTION_STONES
#endif
#define ENABLE_MULTILANGUAGE_SYSTEM
#define __SEND_TARGET_INFO__
#define ENABLE_EXTENDED_ITEMNAME_ON_GROUND

//#define WJ_COMBAT_ZONE // Enable the combat zone
//#define WJ_COMBAT_ZONE_HIDE_INFO_USER // If you have this enabled on server, players can't use chat and whisper on combat zone map. (Disable with "//")

#define ENABLE_BIOLOG_SYSTEM
#define WJ_ENABLE_TRADABLE_ICON
#define ENABLE_BUY_WITH_ITEM
#define FULL_YANG // Enable support for yang type long long

/* OFFLINE SHOPS  
#define OFFLINE_SHOP // Offline shops system
#define GIFT_SYSTEM // gift system enable
#define SHOP_TIME_REFRESH 1*60 // time for cycle checking older shops
#define SHOP_BLOCK_GAME99 //Blocking create shops on channel 99
//#define SHOP_DISTANCE // Show shops in pos distance like WoM2
#define SHOP_AUTO_CLOSE //Enable auto closing shop after sell last item
#define SHOP_ONLY_ALLOWED_INDEX //Enable limiting for other map index which is not defined in player.shop_limit
//#define SHOP_HIDE_NAME // Enable hidding shop npc names like "Player's shop"
#define SHOP_GM_PRIVILEGES GM_IMPLEMENTOR //Minimum GM privileges to using Shop GM Panel
// #define NO_YANG_ON_GIFT_SYSTEM // no recoge el yang del sistema de gift, solo del banco
*/

// #define ENABLE_SWITCHBOT_WORLDARD
//#define UNLOCK_INVENTORY // item para inventario
//#define CHANGE_CHANNEL
#define __HIGHLIGHT_SYSTEM__ // disabled because its bug
#define ENABLE_NEWITEM_AFFECT
//#define ENABLE_MOUNT_SYSTEM
#define MOUNT_SYSTEM_BONUS
//#define SEACH_PRIVATE_SHOP
//#define STONE_INTO_COSTUMES // piedras en atuendos (no funciona)
#define ADD_BONUS_INTO_COSTUMES
#ifdef ADD_BONUS_INTO_COSTUMES
	#define ADD_NEW_BONUS_INTO_COSTUME
#endif
#define HORSE_FIX
#define ENABLE_IGNORE_LOWER_BUFFS
//#define ENABLE_SORT_INVEN
#define ENABLE_PLAYERS_ONLINE
//#define __NEWPET_SYSTEM__
#define __SASH_SYSTEM__
#ifdef __SASH_SYSTEM__
	#define FIX_19_PERCENT
#endif
#define __ANTI_EXP__
#define FIX_SyncPosition
//#define ENABLE_MANAGER_BANK_SYSTEM
#define ENABLE_SPECIAL_STORAGE

#define CHANGE_DS_ATRIBUTES
#define ENABLE_SORT_INVENTORY
// #define ENABLE_SUPPORT_SYSTEM
// ### CommonDefines Systems ###
#define ENABLE_CALENDAR_SYSTEM
#define EXCHANGE_LOG
#define SHOW_DROPS_IN_DB
#define SHOUT_LOG_EXPANDED 
#define ENABLE_BUY_ITEMS_WORLDARD
#define ENABLE_DUNGEON_INFO
//////////////////////////////////////////////////////////////////////////
#define __VERSION_162__
#ifdef __VERSION_162__
	#define HEALING_SKILL_VNUM 265
#endif

#define __NEW_EVENTS__
#ifdef __NEW_EVENTS__
	#define __KINGDOMS_WAR__
#endif
#define ENABLE_AUTOMATIC_EVENT
#define ENABLE_NEW_OX_FEATURES
#define ENABLE_AUTOMATIC_PICK_UP_SYSTEM

#define ENABLE_HIDE_COSTUME_SYSTEM
#define ENABLE_HIDE_COSTUME_SYSTEM_ACCE
#define ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME
//#define ENABLE_HIDE_COSTUME_SYSTEM_TRANSMUTACION
#define ENABLE_CHANGE_FONT_SYSTEMS

#define LSS_SECURITY_KEY		"1234abcd5678efgh"
#define ENABLE_NEW_BONUS_WORLDARD
#define ENABLE_DROP_ITEM_WORLDARD

#define ENABLE_CUBE_RENEWAL_WORLDARD

//#define ENABLE_VOTE_COINS_WHILE_PLAYING
#define ELEMENT_NEW_BONUSES

#define UNLOCK_INVENTORY_WORLDARD
#define __ITEM_SHINING__
#define ENABLE_NEW_RINGS
#define ENABLE_MAINTENANCE_SYSTEM
#define ENABLE_SASH_COSTUME_SYSTEM
#define RENEWAL_DEAD_PACKET
//#define BATTLEPASS_WORLDARD
#define CHANGE_EQUIP_WORLDARD
//#define TOURNAMENT_PVP_SYSTEM

//Installed Systems
#define ITEM_TALISMAN_EQUIPMENT
#define ELEMENT_TARGET
#define ENABLE_AFFECT_BUFF_REMOVE
#define ENABLE_78_SKILLS

#define __BL_67_ATTR__
#define __RENEWAL_BRAVE_CAPE__
#define ENABLE_SELL_ITEM
#define __VIEW_TARGET_PLAYER_HP__
#define __VIEW_TARGET_DECIMAL_HP__
// #define ENABLE_RANKING
#define ENABLE_MAX_ADD_ATTRIBUTE
#define __BUFFI_SUPPORT__
//#ifdef __BUFFI_SUPPORT__
//	#define __BUFFI_SHINING__
//#endif
#define ENABLE_REWARD_SYSTEM
#define __NEW_EXCHANGE_WINDOW__
#define __DUNGEON_INFO__
#define __AUTO_SKILL_READER__

// Systems by @JaZ
#define ENABLE_YOHARA_SYSTEM
	#ifdef ENABLE_YOHARA_SYSTEM
	//#	define ENABLE_RANDOM_ATTR				// Random Attr at Items
	#	define ENABLE_RANDOM_ATTR_ADD_MAX 1		// If refine, attr will increase to max [1 = enabled | 0 = disabled]
	#	define ENABLE_YOHARA_PROTO_EXT			// sungma-exp
	#	define ENABLE_PRECISION					// Precision passive skill
	//#	define ENABLE_PRECISION_SKILL

	// Dungeons
	#	define ENABLE_WHITE_DRAGON				// Alastor
	//#	define ENABLE_WHITE_DRAGON_EX			// Alastor Monster Event
//	#	define ENABLE_QUEEN_NETHIS				// Serpent
	#endif

#define ENABLE_PERFECT_BUFF
#define RENEWAL_HWID_BLOCK
#define __RENEWAL_WARP__
#define __PERMA_ACCESSORY__
#define __AUTO_HUNT__
#define __ROULETTE__
#define __NEW_BANWORD__
#define __SPIN_WHEEL__
#define /* @author: Owsap */ __AUTOMATIC_HUNTING_SYSTEM__ // Automatic Hunting
#define ENABLE_NEWEQ

#define ENABLE_IKASHOP_RENEWAL
#ifdef ENABLE_IKASHOP_RENEWAL
#define USE_IKASHOP_UI_RENEWED
#endif
#define ENABLE_IKASHOP_ENTITIES
#define EXTEND_IKASHOP_PRO
#define EXTEND_IKASHOP_ULTIMATE
#define ENABLE_IKASHOP_LOGS
#define ENABLE_LARGE_DYNAMIC_PACKETS
#define ENABLE_DB_SQL_LOG
#define ENABLE_SUNG_MAHI_TOWER
#define ENABLE_MESSENGER_BLOCK

#define __DOJANG_SRC_FUNCTIONS__
#ifdef __DOJANG_SRC_FUNCTIONS__
	#define DOJANG_MAPINDEX 92
#endif
#define __AURA_SYSTEM__
#ifdef __AURA_SYSTEM__
#define USE_AURA_SYSTEM_RENEWED
#endif
#define ENABLE_PENDANT
#define ENABLE_PENDANT_SYSTEM_RENEWAL
#define __GAME_OPTION_ESCAPE__
// #define ENABLE_NINJEYE
#define ENABLE_PITTY_REFINE
#define __LEADERSHIP__BONUS__
#define ENABLE_ACCUMULATE_DAMAGE_DISPLAY
#define ENABLE_SWITCHBOT_RENEWAL
#define ENABLE_SWITCHBOT
#define __NEW_SET_BONUS__
#define ENABLE_BATTLEPASS
#ifdef ENABLE_BATTLEPASS
    #define BATTLEPASS_PREMIUM_ITEM_VNUM 25152 // useitem vnum
#endif

 #define __BL_BATTLE_ROYALE__
#define __QUEST_RENEWAL__ // Quest Page Renewal
#if defined(__QUEST_RENEWAL__)
#	define __QUEST_EVENT_FISH__ // Practice Fishing
#	define __QUEST_EVENT_MINE__ // Practice Mining
#	define __QUEST_EVENT_BUY_SELL__ // NPC Buy/Sell
#	define __QUEST_EVENT_CRAFT__ // Craft Item
#	define __QUEST_EVENT_EMOTION__ // Emoting
#	define __QUEST_EVENT_RESTART_HERE__ // Restart Here
#endif
#define ENABLE_VOICE_CHAT
//#define ENABLE_ENTITY_LOADING
