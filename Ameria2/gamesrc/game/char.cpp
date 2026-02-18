#include "stdafx.h"

#include "../common/teen_packet.h"
#include "../common/VnumHelper.h"

#include "char.h"

#include "config.h"
#include "utils.h"
#include "crc32.h"
#include "char_manager.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "item_manager.h"
#include "motion.h"
#include "vector.h"
#include "packet.h"
#include "cmd.h"
#include "fishing.h"
#include "exchange.h"
#include "battle.h"
#include "affect.h"
#include "shop.h"
#include "shop_manager.h"
#include "safebox.h"
#include "regen.h"
#include "pvp.h"
#include "party.h"
#include "start_position.h"
#include "questmanager.h"
#include "log.h"
#include "p2p.h"
#include "guild.h"
#include "guild_manager.h"
#include "dungeon.h"
#include "messenger_manager.h"
#include "unique_item.h"
#include "priv_manager.h"
#include "war_map.h"
#include "xmas_event.h"
#include "banword.h"
#include "target.h"
#include "wedding.h"
#include "mob_manager.h"
#include "mining.h"
#include "monarch.h"
#include "castle.h"
#include "arena.h"
#include "dev_log.h"
#include "horsename_manager.h"
#include "pcbang.h"
#include "gm.h"
#include "map_location.h"
#include "BlueDragon_Binder.h"
#include "skill_power.h"
#include "buff_on_attributes.h"
#ifdef __BUFFI_SUPPORT__
#include "buffi.h"
#endif
#ifdef __PET_SYSTEM__
#include "PetSystem.h"
#endif
#include "DragonSoul.h"
#include "../common/CommonDefines.h"
#include <random>
#ifdef __SEND_TARGET_INFO__
#include <algorithm>
#include <iterator>
using namespace std;
#endif
#if defined(WJ_COMBAT_ZONE)
	#include "combat_zone.h"
#endif
#include <boost/algorithm/string.hpp>

#ifdef ENABLE_MOUNT_SYSTEM
#include "MountSystem.h"
#endif
#ifdef __NEWPET_SYSTEM__
#include "New_PetSystem.h"
#endif

#ifdef ENABLE_SUPPORT_SYSTEM
	#include "SupportSystem.h"
#endif
#include "RankPlayer.h"

#ifdef ENABLE_DUNGEON_INFO
#include "dungeon_info.h"
#endif
#include "Anticheat_Manager.h"
#ifdef TOURNAMENT_PVP_SYSTEM
	#include "tournament.h"
#endif

#ifdef ENABLE_WHITE_DRAGON
#	include "WhiteDragon.h"
#endif
#ifdef ENABLE_QUEEN_NETHIS
#	include "SnakeLair.h"
#endif
#ifdef ENABLE_SWITCHBOT
#include "new_switchbot.h"
#endif

#ifdef ENABLE_IKASHOP_RENEWAL
#include <numeric>
#include <chrono>
#include "ikarus_shop.h"
#include "ikarus_shop_manager.h"
#endif
#ifdef USE_DAY_CHANGE_MGR
#include "day_change_mgr.hpp"
#endif

extern const BYTE g_aBuffOnAttrPoints;
extern bool RaceToJob(unsigned race, unsigned *ret_job);

extern bool IS_SUMMONABLE_ZONE(int map_index); // char_item.cpp
bool CAN_ENTER_ZONE(const LPCHARACTER& ch, int map_index);

bool CAN_ENTER_ZONE(const LPCHARACTER& ch, int map_index)
{
	switch (map_index)
	{
	case 301:
	case 302:
	case 303:
	case 304:
		if (ch->GetLevel() < 80)
			return false;
	}
	return true;
}



#ifdef NEW_ICEDAMAGE_SYSTEM
const DWORD CHARACTER::GetNoDamageRaceFlag()
{
	return m_dwNDRFlag;
}

void CHARACTER::SetNoDamageRaceFlag(DWORD dwRaceFlag)
{
	if (dwRaceFlag>=MAIN_RACE_MAX_NUM) return;
	if (IS_SET(m_dwNDRFlag, 1<<dwRaceFlag)) return;
	SET_BIT(m_dwNDRFlag, 1<<dwRaceFlag);
}

void CHARACTER::UnsetNoDamageRaceFlag(DWORD dwRaceFlag)
{
	if (dwRaceFlag>=MAIN_RACE_MAX_NUM) return;
	if (!IS_SET(m_dwNDRFlag, 1<<dwRaceFlag)) return;
	REMOVE_BIT(m_dwNDRFlag, 1<<dwRaceFlag);
}

void CHARACTER::ResetNoDamageRaceFlag()
{
	m_dwNDRFlag = 0;
}

const std::set<DWORD> & CHARACTER::GetNoDamageAffectFlag()
{
	return m_setNDAFlag;
}

void CHARACTER::SetNoDamageAffectFlag(DWORD dwAffectFlag)
{
	m_setNDAFlag.insert(dwAffectFlag);
}

void CHARACTER::UnsetNoDamageAffectFlag(DWORD dwAffectFlag)
{
	m_setNDAFlag.erase(dwAffectFlag);
}

void CHARACTER::ResetNoDamageAffectFlag()
{
	m_setNDAFlag.clear();
}
#endif

// <Factor> DynamicCharacterPtr member function definitions

LPCHARACTER DynamicCharacterPtr::Get() const {
	LPCHARACTER p = NULL;
	if (is_pc) {
		p = CHARACTER_MANAGER::instance().FindByPID(id);
	} else {
		p = CHARACTER_MANAGER::instance().Find(id);
	}
	return p;
}

DynamicCharacterPtr& DynamicCharacterPtr::operator=(LPCHARACTER character) {
	if (character == NULL) {
		Reset();
		return *this;
	}
	if (character->IsPC()) {
		is_pc = true;
		id = character->GetPlayerID();
	} else {
		is_pc = false;
		id = character->GetVID();
	}
	return *this;
}

CHARACTER::CHARACTER()
{
	m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateIdle, &CHARACTER::EndStateEmpty);
	m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateMove, &CHARACTER::EndStateEmpty);
	m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateBattle, &CHARACTER::EndStateEmpty);

	Initialize();
}

CHARACTER::~CHARACTER()
{
	Destroy();
}

void CHARACTER::Initialize()
{
	CEntity::Initialize(ENTITY_CHARACTER);

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	m_bIsNeedSwitchbotSave = false;
#endif

#if defined(__AUTOMATIC_HUNTING_SYSTEM__)
	std::memset(m_abAutoUseType, false, sizeof(m_abAutoUseType));
#endif

	m_bOpeningSafebox = false;

	m_fSyncTime = get_float_time()-3;

#ifdef ENABLE_YOHARA_SYSTEM
	m_pkCheonunEvent = NULL;
#endif

#ifdef __RENEWAL_BRAVE_CAPE__
	m_pkBraveCapeEvent = NULL;
#endif
	m_dwLastMoveItemTime = 0;
#ifdef ENABLE_PREVENT_FLOOD_PACKETS
    m_dwLastExchangeTime = 0;
    m_dwLastBuyTime = 0;
    m_dwLastSafeBoxCheckOutTime = 0;
    m_dwLastItemSplitTime = 0;
    m_dwLastOfferExpime = 0;
    dwLastBraveryCapePulse = 0;
#ifdef ENABLE_GUILD_DONATE_ATTENDANCE
    m_dwLastOfferNewExpTime = 0;
#endif
#ifdef ENABLE_SEARCH_ITEM_DROP_ON_MOB
    m_dwLastDropItemSearchTime = 0;
#endif
#endif
	bWeaponStatus = 0;

#ifdef ENABLE_ANTI_PACKET_FLOOD
	m_dwPacketAntiFloodCount = 0;
	m_dwPacketAntiFloodPulse = 0;
#endif
#if defined(__GAME_OPTION_ESCAPE__)
	m_dwEscapeCooltime = 0;
#endif
	m_dwPlayerID = 0;
	m_dwKillerPID = 0;
#ifdef __NEWPET_SYSTEM__
	m_stImmortalSt = 0;

	m_newpetskillcd[0] = 0;
	m_newpetskillcd[1] = 0;
	m_newpetskillcd[2] = 0;
#endif

#ifdef __BUFFI_SUPPORT__
	m_bIsSupport = false;
	m_bIsLookingBuffiPage = false;
#endif

#ifdef ENABLE_CALENDAR_SYSTEM
	m_dwLastOpenCalendar = 0;
#endif
#ifdef __SEND_TARGET_INFO__
	dwLastTargetInfoPulse = 0;
#endif
	waitHackCounter = 0;
	m_iMoveCount = 0;

	m_pkRegen = NULL;
	regen_id_ = 0;
	m_posRegen.x = m_posRegen.y = m_posRegen.z = 0;
	m_posStart.x = m_posStart.y = 0;
	m_posDest.x = m_posDest.y = 0;
	m_fRegenAngle = 0.0f;

	m_pkMobData		= NULL;
	m_pkMobInst		= NULL;

	m_pkShop		= NULL;

	m_pkExchange	= NULL;
	m_pkParty		= NULL;
	m_pkPartyRequestEvent = NULL;
	//m_pkAnticheatEvent = NULL;

	m_pGuild = NULL;
#if defined(__BL_67_ATTR__)
	b67Attr = false;
#endif
	m_pkChrTarget = NULL;

#ifdef __HIT_LIMITER_ENABLE__
	dw_hit_count = 0;
	dw_hit_next_update = 0;
#endif

#ifdef BATTLEPASS_WORLDARD
	m_dwBattlePassEndTime = 0;
#endif

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	m_language = LOCALE_DEFAULT;//DEFAULT
#endif

	m_pkMuyeongEvent = NULL;
#if defined(WJ_COMBAT_ZONE)
	m_pkCombatZoneLeaveEvent = NULL;
	m_pkCombatZoneWarpEvent = NULL;
#endif
#ifdef ENABLE_QUEEN_NETHIS
	m_pkSnakeSkillEvent = NULL;
#endif
	m_pkWarpNPCEvent = NULL;
	m_pkDeadEvent = NULL;
	m_pkStunEvent = NULL;
	m_pkSaveEvent = NULL;
	m_pkRecoveryEvent = NULL;
	m_pkTimedEvent = NULL;
	m_pkFishingEvent = NULL;
	m_pkWarpEvent = NULL;

	// MINING
	m_pkMiningEvent = NULL;
	// END_OF_MINING

	m_pkPoisonEvent = NULL;
#ifdef ENABLE_WOLFMAN_CHARACTER
	m_pkBleedingEvent = NULL;
#endif
	m_pkFireEvent = NULL;
	m_pkCheckSpeedHackEvent	= NULL;
	m_speed_hack_count	= 0;

	m_pkAffectEvent = NULL;
	m_afAffectFlag = TAffectFlag(0, 0);

#ifdef ENABLE_NINJEYE
    NinjEyeSessionStatus = false;
    NinjEyeBlockStatus = false;
    NinjEyeResponseStatus = false;
    NinjEyeSessionID = 0;
    NinjEyeKey = 0;
    NinjEyeValidationRetryCount = 0;
    NinjEyeDelayEvent = nullptr;
    NinjEyeTimeoutEvent = nullptr;
#endif

	m_pkDestroyWhenIdleEvent = NULL;
#ifdef ENABLE_BATTLE_PASS
	m_battlePassData.clear();
	m_dwMountStartTime = 0;
#endif
	m_pkChrSyncOwner = NULL;

	m_points = {};
	m_pointsInstant = {};

	memset(&character_cards, 0, sizeof(character_cards));
	memset(&randomized_cards, 0, sizeof(randomized_cards));
	m_bCharType = CHAR_TYPE_MONSTER;

	SetPosition(POS_STANDING);

	m_dwPlayStartTime = m_dwLastMoveTime = get_dword_time();

	GotoState(m_stateIdle);
	m_dwStateDuration = 1;

	m_dwLastAttackTime = get_dword_time() - 20000;

	m_bAddChrState = 0;

	m_pkChrStone = NULL;

	m_pkSafebox = NULL;
	m_iSafeboxSize = -1;
	m_iSafeboxLoadTime = 0;

	m_pkMall = NULL;
	m_iMallLoadTime = 0;

	m_posWarp.x = m_posWarp.y = m_posWarp.z = 0;
	m_lWarpMapIndex = 0;

	m_posExit.x = m_posExit.y = m_posExit.z = 0;
	m_lExitMapIndex = 0;

	m_pSkillLevels = NULL;

	m_dwMoveStartTime = 0;
	m_dwMoveDuration = 0;

	m_dwFlyTargetID = 0;

	m_dwNextStatePulse = 0;

	m_dwLastDeadTime = get_dword_time()-180000;

	m_bSkipSave = false;

	m_bItemLoaded = false;

	m_bHasPoisoned = false;
#ifdef ENABLE_WOLFMAN_CHARACTER
	m_bHasBled = false;
#endif
	m_pkDungeon = NULL;
	m_iEventAttr = 0;

#ifdef ENABLE_MOUNT_SYSTEM
	bMounting = false;
	bMountingS = false;
	bMountVnumM = 0;
#endif

	m_kAttackLog.dwVID = 0;
	m_kAttackLog.dwTime = 0;

	m_bNowWalking = m_bWalking = false;
	ResetChangeAttackPositionTime();

	m_bDetailLog = false;
	m_bMonsterLog = false;

	m_bDisableCooltime = false;

	m_iAlignment = 0;
	m_iRealAlignment = 0;
#if defined(WJ_COMBAT_ZONE)
	m_iCombatZonePoints = 0;
	m_iCombatZoneDeaths = 0;
	m_pkNoticeCombatZoneEvent = NULL;
#endif
	m_iKillerModePulse = 0;
	m_bPKMode = PK_MODE_PEACE;

	m_dwQuestNPCVID = 0;
	m_dwQuestByVnum = 0;
	m_pQuestItem = NULL;

	m_szMobileAuth[0] = '\0';

	m_dwUnderGuildWarInfoMessageTime = get_dword_time()-60000;

	m_bUnderRefine = false;

	// REFINE_NPC
	m_dwRefineNPCVID = 0;
	// END_OF_REFINE_NPC

	m_dwPolymorphRace = 0;

	m_bStaminaConsume = false;

	ResetChainLightningIndex();

	m_dwMountVnum = 0;
	m_chHorse = NULL;
	m_chRider = NULL;
#ifdef ENABLE_IKASHOP_RENEWAL
	m_pkIkarusShop = NULL;
	m_pkIkarusShopSafebox	= NULL;
	m_pkIkarusAuction		= NULL;
	m_pkIkarusAuctionGuest= NULL;
	//ikashop-updated 03/08/19
	m_pkIkarusShopGuest = NULL;

	//ikashop-updated 05/08/19
	m_bIsLookingIkarusShopOfferList = false;
#endif
#ifdef ENABLE_MESSENGER_BLOCK
	m_iBlockAntiFloodPulse = 0;
	m_dwBlockAntiFloodCount = 0;
#endif
	m_pWarMap = NULL;
	m_pWeddingMap = NULL;
	m_bChatCounter = 0;

	ResetStopTime();
#if defined(ENABLE_FISHING_GAME)
	SetFishingGameGoals(0);
#endif
	m_dwLastVictimSetTime = get_dword_time() - 3000;
	m_iMaxAggro = -100;

	m_bSendHorseLevel = 0;
	m_bSendHorseHealthGrade = 0;
	m_bSendHorseStaminaGrade = 0;

	m_dwLoginPlayTime = 0;

	m_pkChrMarried = NULL;

	m_posSafeboxOpen.x = -1000;
	m_posSafeboxOpen.y = -1000;

	// EQUIP_LAST_SKILL_DELAY
	m_dwLastSkillTime = get_dword_time();
	// END_OF_EQUIP_LAST_SKILL_DELAY

	// MOB_SKILL_COOLTIME
	memset(m_adwMobSkillCooltime, 0, sizeof(m_adwMobSkillCooltime));
	// END_OF_MOB_SKILL_COOLTIME

	m_isinPCBang = false;

	// ARENA
	m_pArena = NULL;
	m_nPotionLimit = quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count");
	// END_ARENA

	//PREVENT_TRADE_WINDOW
	m_isOpenSafebox = 0;
	//END_PREVENT_TRADE_WINDOW

	//PREVENT_REFINE_HACK
	m_iRefineTime = 0;
	//END_PREVENT_REFINE_HACK

	//RESTRICT_USE_SEED_OR_MOONBOTTLE
	m_iSeedTime = 0;
	//END_RESTRICT_USE_SEED_OR_MOONBOTTLE
	//PREVENT_PORTAL_AFTER_EXCHANGE
	m_iExchangeTime = 0;
	//END_PREVENT_PORTAL_AFTER_EXCHANGE
	//
	m_iSafeboxLoadTime = 0;

	m_iMyShopTime = 0;
	m_iRequestRankTimer = 0;

	InitMC();

	m_deposit_pulse = 0;

	SET_OVER_TIME(this, OT_NONE);

	m_strNewName = "";

	m_known_guild.clear();

	m_dwLogOffInterval = 0;

	m_bComboSequence = 0;
	m_dwLastComboTime = 0;
	m_bComboIndex = 0;
	m_iComboHackCount = 0;
	m_dwSkipComboAttackByTime = 0;

#ifdef ENABLE_CHECK_PICKUP_HACK
	m_dwLastPickupTime = 0;
#endif
#ifdef ENABLE_CHECK_WALLHACK
	m_dwCountWallhackPoint = 0;
#endif
#ifdef ENABLE_CHECK_GHOSTMODE
	m_dwCountGhostmodePoint = 0;
#endif
#ifdef ENABLE_CSHIELD
	m_pkCShieldEvent = NULL;
	m_pkCShieldDataEvent = NULL;
	m_cshield = nullptr;
#endif

	m_dwMountTime = 0;

	m_dwLastGoldDropTime = 0;
#ifdef ENABLE_NEWSTUFF
	m_dwLastItemDropTime = 0;
	m_dwLastBoxUseTime = 0;
	m_dwLastBuySellTime = 0;
#endif

	m_bIsLoadedAffect = false;
	cannot_dead = false;

#ifdef ENABLE_SUPPORT_SYSTEM
	m_supportSystem = 0;
	m_dwSupportHair = 0;
	m_dwSupportWeapon = 0;
	m_dwSupportCostume = 0;
	m_Owner = NULL;
#endif
	m_PetOwner = NULL;
#ifdef ENABLE_RANKING
	for (int i = 0; i < RANKING_MAX_CATEGORIES; ++i)
		m_lRankPoints[i] = 0;
#endif

#ifdef __PET_SYSTEM__
	m_petSystem = 0;
	m_bIsPet = false;
#endif
#ifdef __NEWPET_SYSTEM__	
	m_newpetSystem = 0;
	m_bIsNewPet = false;
	m_eggvid = 0;
#endif
#ifdef ENABLE_MOUNT_SYSTEM
	m_MountSystem = 0;
	m_bIsMountSystem = false;
#endif
#ifdef NEW_ICEDAMAGE_SYSTEM
	m_dwNDRFlag = 0;
	m_setNDAFlag.clear();
#endif

	m_fAttMul = 1.0f;
	m_fDamMul = 1.0f;
#ifdef ENABLE_NEW_PET_SYSTEM
	m_bIsNewPet = false;
#endif
	m_pointsInstant.iDragonSoulActiveDeck = -1;
#ifdef ENABLE_SUNG_MAHI_TOWER
	m_bIsUniqueMaster = false;
#endif
#ifdef ENABLE_ANTI_CMD_FLOOD
	m_dwCmdAntiFloodCount = 0;
	m_dwCmdAntiFloodPulse = 0;
#endif
	memset(&m_tvLastSyncTime, 0, sizeof(m_tvLastSyncTime));
	m_iSyncHackCount = 0;
#ifdef ENABLE_SWITCHBOT_WORLDARD
	m_isOpenSB = false;
#endif
#ifdef __SASH_SYSTEM__
	m_bSashCombination = false;
	m_bSashAbsorption = false;
#endif
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	dwPickUPMode = 0;
	dwRarityMode = 0;
#endif
#ifdef UNLOCK_INVENTORY
	m_dwUnlockSlots = 0;
#endif
#ifdef ENABLE_SORT_INVENTORY
    m_sortInventoryPulse = 0;
    m_sortSpecialStoragePulse = 0;
#endif

#ifdef ENABLE_CALENDAR_SYSTEM
	m_isOpenCalendar = 0;
#endif

#ifdef ENABLE_DUNGEON_INFO
	CDungeonInfoExtern::instance().LoadDateDungeon();
	//CDungeonInfoExtern::instance().LoadDateDungeonRanking();
	damage_done_dungeon_info = 0;
	damage_received_dungeon_info = 0;
#endif
#ifdef __FAKE_PLAYER__
	m_bIsFakePlayer = false;
	m_pFakePlayerDeadEvent = nullptr;
	m_stFakePlayerLanguage = 0;
	m_dwFakePlayerGuildID = 0;
	m_bFakePlayerBlockEQ = 0;
#endif
#ifdef ENABLE_VOTE_COINS_WHILE_PLAYING
	m_pkVoteCoinsWhilePlaying = NULL;
#endif

#ifdef UNLOCK_INVENTORY_WORLDARD
	memset(inventory_unlock, 0, sizeof(inventory_unlock));
#endif
#ifdef ENABLE_MINI_GAME_CATCH_KING
	m_vecCatchKingFieldCards.clear();
	bCatchKingHandCard = 0;
	bCatchKingHandCardLeft = 0;
	bCatchKingBetSetNumber = 0;
	dwCatchKingTotalScore = 0;
	dwCatchKingGameStatus = false;
#endif
#ifdef __AUTO_SKILL_READER__
	m_pkAutoSkill = NULL;
	m_bSelectedSkillIdx = 0;
	m_bAutoSkillStatus = 0;
#endif
	m_iLastFishingTime = thecore_pulse();

#ifdef USE_CAPTCHA_SYSTEM
    m_CaptchaTimer = nullptr;
    m_isWaitingForCaptcha = false;
    m_Captcha = 0;
#endif

    m_protection_Time.clear();

#ifdef USE_OFFLINESHOP_SEARCH_V2
    m_iShopSearchPageCount = 0;
    m_iShopSearchPage = 1;

    m_ShopSearchFilterInfo = {};

    m_vecShopSearchItem.clear();
    m_setShopSearchItemIds.clear();
#endif
#ifdef __AURA_SYSTEM__
	m_bAuraRefineWindowType = AURA_WINDOW_TYPE_MAX;
	m_bAuraRefineWindowOpen = false;
	for (BYTE i = AURA_SLOT_MAIN; i < AURA_SLOT_MAX; i++)
		m_pAuraRefineWindowItemSlot[i] = NPOS;

	memset(&m_bAuraRefineInfo, 0, AURA_REFINE_INFO_SLOT_MAX * sizeof(TAuraRefineInfo));
#endif

#ifdef USE_DELAY_MGR
	memset(&m_dwDelayTime, 0, sizeof(m_dwDelayTime));
#endif
#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
	m_hitCount.clear();
#endif
#ifdef ENABLE_BATTLEPASS
	m_player_battle_pass_stats.clear();
    m_player_battle_pass_tier_info.clear();
    m_player_battle_pass_current_level = m_player_battle_pass_current_exp = m_player_battle_pass_premium_status = 0;
#endif

//#ifdef ENABLE_ITEMSHOP
	m_bAccountMoneyLoaded = false;

	m_lldAccountCoins = 0;
	m_lldAccountJCoins = 0;
//#endif
}

void CHARACTER::Create(const char * c_pszName, DWORD vid, bool isPC)
{
	static int s_crc = 172814;

	char crc_string[128+1];
	snprintf(crc_string, sizeof(crc_string), "%s%p%d", c_pszName, this, ++s_crc);
	m_vid = VID(vid, GetCRC32(crc_string, strlen(crc_string)));

	if (isPC)
	{
		m_stName = c_pszName;
#ifdef ENABLE_CSHIELD
		m_cshield = std::make_shared<CShield>();
#endif
	}
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	else
		m_stName = "M" + std::to_string(GetRaceNum());
#endif
}

void CHARACTER::Destroy()
{
	if (IsPC())
	{
		sys_log(0, "destroy char: name %s, id %u.", GetName(), GetPlayerID());
	}

#ifdef ENABLE_NINJEYE
    InvalidateNinjEyeSession();
#endif

//#ifdef ENABLE_ITEMSHOP
	m_bAccountMoneyLoaded = false;

	m_lldAccountCoins = 0;
	m_lldAccountJCoins = 0;
//#endif

#ifdef USE_OFFLINESHOP_SEARCH_V2
    m_vecShopSearchItem.clear();
    m_setShopSearchItemIds.clear();
#endif

#ifdef WJ_COMBAT_ZONE
	event_cancel(&m_pkNoticeCombatZoneEvent);
#endif

	if (m_pkRegen)
	{
		if (m_pkDungeon) {
			// Dungeon regen may not be valid at this point
			if (m_pkDungeon->IsValidRegen(m_pkRegen, regen_id_)) {
				--m_pkRegen->count;
			}
		} else {
			// Is this really safe?
			--m_pkRegen->count;
		}
		m_pkRegen = NULL;
	}

#ifdef __BUFFI_SUPPORT__
	BUFFI_MANAGER::Instance().CompareLoginStatus(false, this);
#endif

	if (m_pkDungeon)
	{
		SetDungeon(NULL);
	}
#ifdef ENABLE_SUPPORT_SYSTEM
	if (m_supportSystem)
	{
		m_supportSystem->Destroy();
		delete m_supportSystem;

		m_supportSystem = 0;
	}
#endif

#ifdef __PET_SYSTEM__
	if (m_petSystem)
	{
		m_petSystem->Destroy();
		delete m_petSystem;
		m_petSystem = NULL;
	}
#endif

#ifdef __NEWPET_SYSTEM__
	if (m_newpetSystem)
	{
		m_newpetSystem->Destroy();
		delete m_newpetSystem;
		m_newpetSystem = NULL;
	}
#endif
#ifdef ENABLE_MOUNT_SYSTEM
	if (m_MountSystem)
	{
		m_MountSystem->Destroy();
		delete m_MountSystem;

		m_MountSystem = 0;
	}
#endif
	HorseSummon(false);

	if (GetRider())
		GetRider()->ClearHorseInfo();

	
	if (IsPC())
	{
		CAnticheatManager::instance().DeleteClientHandle(GetPlayerID());
	}

	if (GetDesc())
	{
		GetDesc()->BindCharacter(NULL);
//		BindDesc(NULL);
	}

	if (m_pkExchange)
		m_pkExchange->Cancel();

	SetVictim(NULL);

	if (GetShop())
	{
		GetShop()->RemoveGuest(this);
		SetShop(NULL);
	}

#ifdef TOURNAMENT_PVP_SYSTEM
	if (IsPC())
	{	
		CTournamentPvP::instance().OnDisconnect(this);
	}
#endif

#if defined(WJ_COMBAT_ZONE)
	if (IsPC())
	{	
		CCombatZoneManager::instance().OnLogout(this);
	}
#endif
	ClearStone();
	ClearSync();
	ClearTarget();

	if (NULL == m_pkMobData)
	{
		DragonSoul_CleanUp();
		ClearItem();
	}

	// <Factor> m_pkParty becomes NULL after CParty destructor call!
	LPPARTY party = m_pkParty;
	if (party)
	{
		if (party->GetLeaderPID() == GetVID() && !IsPC())
		{
			M2_DELETE(party);
		}
		else
		{
			party->Unlink(this);

			if (!IsPC())
				party->Quit(GetVID());
		}

		SetParty(NULL); // ¾ÈÇØµµ µÇÁö¸¸ ¾ÈÀüÇÏ°Ô.
	}

	if (m_pkMobInst)
	{
		M2_DELETE(m_pkMobInst);
		m_pkMobInst = NULL;
	}

	m_pkMobData = NULL;
#ifdef ENABLE_MULTI_FARM_BLOCK
	m_bmultiFarmStatus = false;
#endif

	if (m_pkSafebox)
	{
		M2_DELETE(m_pkSafebox);
		m_pkSafebox = NULL;
	}

	if (m_pkMall)
	{
		M2_DELETE(m_pkMall);
		m_pkMall = NULL;
	}

	for (TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.begin();  it != m_map_buff_on_attrs.end(); it++)
	{
		if (NULL != it->second)
		{
			M2_DELETE(it->second);
		}
	}
	m_map_buff_on_attrs.clear();

	m_set_pkChrSpawnedBy.clear();

#ifdef __AUTO_SKILL_READER__
	if (m_pkAutoSkill)
	{
		event_cancel(&m_pkAutoSkill);
		m_pkAutoSkill = nullptr;
	}
#endif

	StopMuyeongEvent();
#ifdef ENABLE_YOHARA_SYSTEM
	StopCheonunEvent();
#endif

	event_cancel(&m_pkWarpNPCEvent);
	event_cancel(&m_pkRecoveryEvent);
	event_cancel(&m_pkDeadEvent);
	event_cancel(&m_pkSaveEvent);
#if defined(WJ_COMBAT_ZONE)
	event_cancel(&m_pkCombatZoneLeaveEvent);
	event_cancel(&m_pkCombatZoneWarpEvent);
#endif
	event_cancel(&m_pkTimedEvent);
	event_cancel(&m_pkStunEvent);
	event_cancel(&m_pkFishingEvent);
	event_cancel(&m_pkPoisonEvent);
#ifdef ENABLE_WOLFMAN_CHARACTER
	event_cancel(&m_pkBleedingEvent);
#endif
#ifdef __RENEWAL_BRAVE_CAPE__
	event_cancel(&m_pkBraveCapeEvent);
#endif
	event_cancel(&m_pkFireEvent);
	event_cancel(&m_pkPartyRequestEvent);
	//DELAYED_WARP
	event_cancel(&m_pkWarpEvent);
	event_cancel(&m_pkCheckSpeedHackEvent);
	//END_DELAYED_WARP

	event_cancel(&m_pkAnticheatEvent);


	// RECALL_DELAY
	//event_cancel(&m_pkRecallEvent);
	// END_OF_RECALL_DELAY

	// MINING
	event_cancel(&m_pkMiningEvent);
	// END_OF_MINING

#ifdef USE_CAPTCHA_SYSTEM
    if (m_CaptchaTimer)
    {
        event_cancel(&m_CaptchaTimer);
        m_CaptchaTimer = nullptr;
    }
#endif

#ifdef ENABLE_CSHIELD
	event_cancel(&m_pkCShieldEvent);
	event_cancel(&m_pkCShieldDataEvent);
#endif

#ifdef ENABLE_VOTE_COINS_WHILE_PLAYING
	event_cancel(&m_pkVoteCoinsWhilePlaying);
#endif
#ifdef __FAKE_PLAYER__
	event_cancel(&m_pFakePlayerDeadEvent);
#endif

	for (itertype(m_mapMobSkillEvent) it = m_mapMobSkillEvent.begin(); it != m_mapMobSkillEvent.end(); ++it)
	{
		LPEVENT pkEvent = it->second;
		event_cancel(&pkEvent);
	}
	m_mapMobSkillEvent.clear();

	ClearAffect();
	//event_cancel(&m_pkAffectEvent);

	event_cancel(&m_pkDestroyWhenIdleEvent);

	if (m_pSkillLevels)
	{
		M2_DELETE_ARRAY(m_pSkillLevels);
		m_pSkillLevels = NULL;
	}

	CEntity::Destroy();

	if (GetSectree())
		GetSectree()->RemoveEntity(this);

	if (m_bMonsterLog)
		CHARACTER_MANAGER::instance().UnregisterForMonsterLog(this);
#ifdef ENABLE_IKASHOP_RENEWAL
	SetIkarusShop(nullptr);
	SetIkarusShopGuest(nullptr);
	SetIkarusShopSafebox(nullptr);
	SetIkarusAuction(nullptr);
	SetIkarusAuctionGuest(nullptr);
#endif
}

const char * CHARACTER::GetName() const
{
	return (m_stName.empty() || (IsWarp() || IsGoto()))  ? (m_pkMobData ? m_pkMobData->m_table.szLocaleName : "") : m_stName.c_str();
}

void EncodeMovePacket(TPacketGCMove & pack, DWORD dwVID, BYTE bFunc, BYTE bArg, DWORD x, DWORD y, DWORD dwDuration, DWORD dwTime, BYTE bRot)
{
	pack.bHeader = HEADER_GC_MOVE;
	pack.bFunc   = bFunc;
	pack.bArg    = bArg;
	pack.dwVID   = dwVID;
	pack.dwTime  = dwTime ? dwTime : get_dword_time();
	pack.bRot    = bRot;
	pack.lX		= x;
	pack.lY		= y;
	pack.dwDuration	= dwDuration;
}

void CHARACTER::RestartAtSamePos()
{
	if (m_bIsObserver)
		return;

	EncodeRemovePacket(this);
	EncodeInsertPacket(this);

	ENTITY_MAP::iterator it = m_map_view.begin();

	while (it != m_map_view.end())
	{
		LPENTITY entity = (it++)->first;

		EncodeRemovePacket(entity);
		if (!m_bIsObserver)
			EncodeInsertPacket(entity);

		if( entity->IsType(ENTITY_CHARACTER) )
		{
			LPCHARACTER lpChar = (LPCHARACTER)entity;
			if( lpChar->IsPC() || lpChar->IsNPC() || lpChar->IsMonster() )
			{
				if (!entity->IsObserverMode())
					entity->EncodeInsertPacket(this);
			}
		}
		else
		{
			if( !entity->IsObserverMode())
			{
				entity->EncodeInsertPacket(this);
			}
		}
	}
}

// #define ENABLE_SHOWNPCLEVEL
// Entity¿¡ ³»°¡ ³ªÅ¸³µ´Ù°í ÆÐÅ¶À» º¸³½´Ù.
void CHARACTER::EncodeInsertPacket(LPENTITY entity)
{
    LPDESC d = entity ? entity->GetDesc() : nullptr;
    if (!d)
    {
        return;
    }

	// ±æµåÀÌ¸§ ¹ö±× ¼öÁ¤ ÄÚµå
	LPCHARACTER ch = (LPCHARACTER) entity;
	ch->SendGuildName(GetGuild());
#ifdef __FAKE_PLAYER__
	if (IsFakePlayer())
	{	
		if (m_dwFakePlayerGuildID)
			ch->SendGuildName(m_dwFakePlayerGuildID);
	}
#endif	
	TPacketGCCharacterAdd pack;

	pack.header		= HEADER_GC_CHARACTER_ADD;
	pack.dwVID		= m_vid;
	pack.bType		= GetCharType();
	pack.angle		= GetRotation();
	pack.x		= GetX();
	pack.y		= GetY();
	pack.z		= GetZ();
	pack.wRaceNum	= GetRaceNum();
#ifdef ENABLE_MOUNT_SYSTEM
	if (IsPet() || IsMountSystem() || IsNewPet() || IsSupport())
#else
	if (IsPet())
#endif
	{
		pack.bMovingSpeed	= 150;
	}
	else
	{
		pack.bMovingSpeed = GetLimitPoint(POINT_MOV_SPEED);
		//sys_err("MoveSpeed_1 %d", GetLimitPoint(POINT_MOV_SPEED));
	}
	pack.bAttackSpeed	= GetLimitPoint(POINT_ATT_SPEED);
	pack.dwAffectFlag[0] = m_afAffectFlag.bits[0];
	pack.dwAffectFlag[1] = m_afAffectFlag.bits[1];
	pack.dwAffectFlag[2] = m_afAffectFlag.bits[2];

	pack.bStateFlag = m_bAddChrState;

	int iDur = 0;

	if (m_posDest.x != pack.x || m_posDest.y != pack.y)
	{
		iDur = (m_dwMoveStartTime + m_dwMoveDuration) - get_dword_time();

		if (iDur <= 0)
		{
			pack.x = m_posDest.x;
			pack.y = m_posDest.y;
		}
	}

	d->Packet(&pack, sizeof(pack));


#ifdef ENABLE_NEW_PET_SYSTEM
	if (IsPC() == true || m_bCharType == CHAR_TYPE_NPC || m_bCharType == CHAR_TYPE_PET
#ifdef __FAKE_PLAYER__
		|| IsFakePlayer()
#endif
	)
#else
	if (IsPC() == true || m_bCharType == CHAR_TYPE_NPC
#ifdef __FAKE_PLAYER__
		|| IsFakePlayer()
#endif
	)
#endif
	{
		TPacketGCCharacterAdditionalInfo addPacket;
		memset(&addPacket, 0, sizeof(TPacketGCCharacterAdditionalInfo));

		addPacket.header = HEADER_GC_CHAR_ADDITIONAL_INFO;
		addPacket.dwVID = m_vid;
#ifdef ENABLE_HIDE_COSTUME_SYSTEM
		SetHideCostumeUpdate();
#endif
		addPacket.awPart[CHR_EQUIPPART_ARMOR] = GetPart(PART_MAIN);
		addPacket.awPart[CHR_EQUIPPART_WEAPON] = GetPart(PART_WEAPON);
		addPacket.awPart[CHR_EQUIPPART_HEAD] = GetPart(PART_HEAD);
		addPacket.awPart[CHR_EQUIPPART_HAIR] = GetPart(PART_HAIR);
		#ifdef __SASH_SYSTEM__
		addPacket.awPart[CHR_EQUIPPART_SASH] = GetPart(PART_SASH);
		#endif
		addPacket.awPart[CHR_PART_ARROW_TYPE] = GetPart(PART_ARROW_TYPE);
#ifdef __AURA_SYSTEM__
		addPacket.awPart[CHR_EQUIPPART_AURA] = GetPart(PART_AURA);
#endif

#ifdef __ITEM_SHINING__
		addPacket.adwShining[CHR_SHINING_WEAPON_1] = GetWear(WEAR_SHINING_WEAPON_1) ? GetWear(WEAR_SHINING_WEAPON_1)->GetVnum() : 0;
		// addPacket.adwShining[CHR_SHINING_WEAPON_2] = GetWear(WEAR_SHINING_WEAPON_2) ? GetWear(WEAR_SHINING_WEAPON_2)->GetVnum() : 0;
		// addPacket.adwShining[CHR_SHINING_WEAPON_3] = GetWear(WEAR_SHINING_WEAPON_3) ? GetWear(WEAR_SHINING_WEAPON_3)->GetVnum() : 0;
		addPacket.adwShining[CHR_SHINING_ARMOR_1] = GetWear(WEAR_SHINING_ARMOR_1) ? GetWear(WEAR_SHINING_ARMOR_1)->GetVnum() : 0;
		// addPacket.adwShining[CHR_SHINING_ARMOR_2] = GetWear(WEAR_SHINING_ARMOR_2) ? GetWear(WEAR_SHINING_ARMOR_2)->GetVnum() : 0;
		// addPacket.adwShining[CHR_SHINING_ARMOR_3] = GetWear(WEAR_SHINING_ARMOR_3) ? GetWear(WEAR_SHINING_ARMOR_3)->GetVnum() : 0;
		addPacket.adwShining[CHR_SHINING_SPECIAL] = GetWear(WEAR_SHINING_SPECIAL) ? GetWear(WEAR_SHINING_SPECIAL)->GetVnum() : 0;
#endif
		addPacket.bPKMode = m_bPKMode;
		addPacket.dwMountVnum = GetMountVnum();
		addPacket.bEmpire = m_bEmpire;
#ifdef ENABLE_SHOWNPCLEVEL
		if (1)
#else
		if (IsPC() == true || IsNewPet() == true
#ifdef __FAKE_PLAYER__
			|| IsFakePlayer()
#endif	
		)
#endif
		{
			addPacket.dwLevel = GetLevel();
#ifdef ENABLE_YOHARA_SYSTEM
			addPacket.dwConquerorLevel = GetConquerorLevel();
#endif
		}
		else
		{
			addPacket.dwLevel = 0;
#ifdef ENABLE_YOHARA_SYSTEM
			addPacket.dwConquerorLevel = 0;
#endif
		}

		if (false)
		{
			LPCHARACTER ch = (LPCHARACTER) entity;

			if (GetEmpire() == ch->GetEmpire() || ch->GetGMLevel() > GM_PLAYER || m_bCharType == CHAR_TYPE_NPC)
			{
				goto show_all_info;
			}
			else
			{
				memset(addPacket.name, 0, CHARACTER_NAME_MAX_LEN);
				addPacket.dwGuildID = 0;
				addPacket.sAlignment = 0;
			}
		}
		else
		{
		show_all_info:
			strlcpy(addPacket.name, GetName(), sizeof(addPacket.name));
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
			addPacket.language = GetCharacterLanguage();
			if (IsFakePlayer())
				addPacket.language = GetFakePlayerLanguage();
			if (IsBuffi())
				addPacket.language = 0;
#endif
			if (GetGuild() != NULL)
			{
				addPacket.dwGuildID = GetGuild()->GetID();
			}
#ifdef __FAKE_PLAYER__
			else if (IsFakePlayer())
			{
				addPacket.dwGuildID = m_dwFakePlayerGuildID;
		//		addPacket.dwNewIsGuildName = 0;
			}
#endif			
			else
				addPacket.dwGuildID = 0;

			addPacket.sAlignment = m_iAlignment / 10;
#if defined(WJ_COMBAT_ZONE)
			addPacket.combat_zone_rank = GetCombatZoneRank();
			addPacket.combat_zone_points = GetCombatZonePoints();
#endif
		}

		d->Packet(&addPacket, sizeof(TPacketGCCharacterAdditionalInfo));
	}

	if (iDur)
	{
		TPacketGCMove pack;
		EncodeMovePacket(pack, GetVID(), FUNC_MOVE, 0, m_posDest.x, m_posDest.y, iDur, 0, (BYTE) (GetRotation() / 5));
		d->Packet(&pack, sizeof(pack));

		TPacketGCWalkMode p;
		p.vid = GetVID();
		p.header = HEADER_GC_WALK_MODE;
		p.mode = m_bNowWalking ? WALKMODE_WALK : WALKMODE_RUN;

		d->Packet(&p, sizeof(p));
	}

	if (entity->IsType(ENTITY_CHARACTER) && GetDesc())
	{
		LPCHARACTER ch = (LPCHARACTER) entity;
		if (ch->IsWalking())
		{
			TPacketGCWalkMode p;
			p.vid = ch->GetVID();
			p.header = HEADER_GC_WALK_MODE;
			p.mode = ch->m_bNowWalking ? WALKMODE_WALK : WALKMODE_RUN;
			GetDesc()->Packet(&p, sizeof(p));
		}
	}

	if (entity->IsType(ENTITY_CHARACTER))
	{
		sys_log(3, "EntityInsert %s (RaceNum %d) (%d %d) TO %s",
				GetName(), GetRaceNum(), GetX() / SECTREE_SIZE, GetY() / SECTREE_SIZE, ((LPCHARACTER)entity)->GetName());
	}
}

void CHARACTER::EncodeRemovePacket(LPENTITY entity)
{
	if (entity->GetType() != ENTITY_CHARACTER)
		return;

	LPDESC d;

	if (!(d = entity->GetDesc()))
		return;

	TPacketGCCharacterDelete pack;

	pack.header	= HEADER_GC_CHARACTER_DEL;
	pack.id	= m_vid;

	d->Packet(&pack, sizeof(TPacketGCCharacterDelete));

	if (entity->IsType(ENTITY_CHARACTER))
		sys_log(3, "EntityRemove %s(%d) FROM %s", GetName(), (DWORD) m_vid, ((LPCHARACTER) entity)->GetName());
}

void CHARACTER::UpdatePacket()
{
    if (!GetSectree())
    {
        return;
    }

    LPDESC d = GetDesc();
    if (d && !d->GetCharacter())
    {
        return;
    }

    if (!d
/*
#ifdef ENABLE_SUPPORT_SYSTEM
        && !IsSupport()
#endif
*/
#ifdef __BUFFI_SUPPORT__
        && !IsBuffi()
#endif
    )
    {
        return;
    }

	TPacketGCCharacterUpdate pack;
	TPacketGCCharacterUpdate pack2;

	pack.header = HEADER_GC_CHARACTER_UPDATE;
	pack.dwVID = m_vid;
#ifdef ENABLE_HIDE_COSTUME_SYSTEM
	SetHideCostumeUpdate();
#endif
	pack.awPart[CHR_EQUIPPART_ARMOR] = GetPart(PART_MAIN);
	pack.awPart[CHR_EQUIPPART_WEAPON] = GetPart(PART_WEAPON);
	pack.awPart[CHR_EQUIPPART_HEAD] = GetPart(PART_HEAD);
	pack.awPart[CHR_EQUIPPART_HAIR] = GetPart(PART_HAIR);
#ifdef __SASH_SYSTEM__
	pack.awPart[CHR_EQUIPPART_SASH] = GetPart(PART_SASH);
#endif
#ifdef __AURA_SYSTEM__
	pack.awPart[CHR_EQUIPPART_AURA] = GetPart(PART_AURA);
#endif
#ifdef __ITEM_SHINING__
	pack.adwShining[CHR_SHINING_WEAPON_1] = GetWear(WEAR_SHINING_WEAPON_1) ? GetWear(WEAR_SHINING_WEAPON_1)->GetVnum() : 0;
	// pack.adwShining[CHR_SHINING_WEAPON_2] = GetWear(WEAR_SHINING_WEAPON_2) ? GetWear(WEAR_SHINING_WEAPON_2)->GetVnum() : 0;
	// pack.adwShining[CHR_SHINING_WEAPON_3] = GetWear(WEAR_SHINING_WEAPON_3) ? GetWear(WEAR_SHINING_WEAPON_3)->GetVnum() : 0;
	pack.adwShining[CHR_SHINING_ARMOR_1] = GetWear(WEAR_SHINING_ARMOR_1) ? GetWear(WEAR_SHINING_ARMOR_1)->GetVnum() : 0;
	// pack.adwShining[CHR_SHINING_ARMOR_2] = GetWear(WEAR_SHINING_ARMOR_2) ? GetWear(WEAR_SHINING_ARMOR_2)->GetVnum() : 0;
	// pack.adwShining[CHR_SHINING_ARMOR_3] = GetWear(WEAR_SHINING_ARMOR_3) ? GetWear(WEAR_SHINING_ARMOR_3)->GetVnum() : 0;
	pack.adwShining[CHR_SHINING_SPECIAL] = GetWear(WEAR_SHINING_SPECIAL) ? GetWear(WEAR_SHINING_SPECIAL)->GetVnum() : 0;
#endif

	pack.awPart[CHR_PART_ARROW_TYPE] = GetPart(PART_ARROW_TYPE);

	pack.bMovingSpeed = GetLimitPoint(POINT_MOV_SPEED);
	sys_err("MoveSpeed_2 %d", GetLimitPoint(POINT_MOV_SPEED));
	pack.bAttackSpeed	= GetLimitPoint(POINT_ATT_SPEED);
	pack.bStateFlag	= m_bAddChrState;
	pack.dwAffectFlag[0] = m_afAffectFlag.bits[0];
	pack.dwAffectFlag[1] = m_afAffectFlag.bits[1];
	pack.dwAffectFlag[2] = m_afAffectFlag.bits[2];
	pack.dwGuildID	= 0;
	pack.sAlignment	= m_iAlignment / 10;

#ifdef ENABLE_YOHARA_SYSTEM
	pack.dwConquerorLevel = GetConquerorLevel();
#endif

#ifdef ENABLE_SUPPORT_SYSTEM
	if(IsSupport())
	{
		pack.awPart[CHR_EQUIPPART_ARMOR] = GetSupportArmor();
		pack.awPart[CHR_EQUIPPART_WEAPON] = GetSupportWeapon();
		pack.awPart[CHR_EQUIPPART_HAIR] = GetSupportHair();
	}
#endif
#ifdef __NEWPET_SYSTEM__
	pack.dwLevel = GetLevel();
#endif
	pack.bPKMode	= m_bPKMode;
#if defined(WJ_COMBAT_ZONE)
	pack.combat_zone_rank = GetCombatZoneRank();
	pack.combat_zone_points	= GetCombatZonePoints();
#endif
	if (GetGuild())
		pack.dwGuildID = GetGuild()->GetID();
#ifdef __FAKE_PLAYER__
	else if (IsFakePlayer())
		pack.dwGuildID = m_dwFakePlayerGuildID;
#endif

	pack.dwMountVnum	= GetMountVnum();

	pack2 = pack;
	pack2.dwGuildID = 0;
	pack2.sAlignment = 0;
#ifdef __NEWPET_SYSTEM__
	pack2.dwLevel = 0;
#endif

	if (false)
	{
		if (m_bIsObserver != true)
		{
			for (ENTITY_MAP::iterator iter = m_map_view.begin(); iter != m_map_view.end(); iter++)
			{
				LPENTITY pEntity = iter->first;

				if (pEntity != NULL)
				{
					if (pEntity->IsType(ENTITY_CHARACTER) == true)
					{
						if (pEntity->GetDesc() != NULL)
						{
							LPCHARACTER pChar = (LPCHARACTER)pEntity;

							if (GetEmpire() == pChar->GetEmpire() || pChar->GetGMLevel() > GM_PLAYER)
							{
								pEntity->GetDesc()->Packet(&pack, sizeof(pack));
							}
							else
							{
								pEntity->GetDesc()->Packet(&pack2, sizeof(pack2));
							}
						}
					}
					else
					{
						if (pEntity->GetDesc() != NULL)
						{
							pEntity->GetDesc()->Packet(&pack, sizeof(pack));
						}
					}
				}
			}
		}

		if (GetDesc() != NULL)
		{
			GetDesc()->Packet(&pack, sizeof(pack));
		}
	}
	else
	{
		PacketAround(&pack, sizeof(pack));
	}
}

LPCHARACTER CHARACTER::FindCharacterInView(const char * c_pszName, bool bFindPCOnly)
{
	ENTITY_MAP::iterator it = m_map_view.begin();

	for (; it != m_map_view.end(); ++it)
	{
		if (!it->first->IsType(ENTITY_CHARACTER))
			continue;

		LPCHARACTER tch = (LPCHARACTER) it->first;

		if (bFindPCOnly && tch->IsNPC())
			continue;

		if (!strcasecmp(tch->GetName(), c_pszName))
			return (tch);
	}

	return NULL;
}

void CHARACTER::SetPosition(int pos)
{
	if (pos == POS_STANDING)
	{
		REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_DEAD);
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_STUN);

		event_cancel(&m_pkDeadEvent);
		event_cancel(&m_pkStunEvent);
	}
	else if (pos == POS_DEAD)
		SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_DEAD);

	if (!IsStone())
	{
		switch (pos)
		{
			case POS_FIGHTING:
				if (!IsState(m_stateBattle))
					MonsterLog("[BATTLE] ½Î¿ì´Â »óÅÂ");

				GotoState(m_stateBattle);
				break;

			default:
				if (!IsState(m_stateIdle))
					MonsterLog("[IDLE] ½¬´Â »óÅÂ");

				GotoState(m_stateIdle);
				break;
		}
	}

	m_pointsInstant.position = pos;
}

void CHARACTER::Save()
{
	if (!m_bSkipSave)
		CHARACTER_MANAGER::instance().DelayedSave(this);
}

void CHARACTER::CreatePlayerProto(TPlayerTable & tab)
{
	memset(&tab, 0, sizeof(TPlayerTable));

	if (GetNewName().empty())
	{
		strlcpy(tab.name, GetName(), sizeof(tab.name));
	}
	else
	{
		strlcpy(tab.name, GetNewName().c_str(), sizeof(tab.name));
	}

	strlcpy(tab.ip, GetDesc()->GetHostName(), sizeof(tab.ip));

	tab.id			= m_dwPlayerID;
	tab.voice		= GetPoint(POINT_VOICE);
	tab.level		= GetLevel();
	tab.level_step	= GetPoint(POINT_LEVEL_STEP);
	tab.exp			= GetExp();
	tab.gold		= GetGold();
#ifdef USE_CHEQUE_CURRENCY
	tab.cheque = GetCheque();
#endif
#ifdef __GEM_SYSTEM__
	tab.gem = GetGem();
#endif
	tab.job			= m_points.job;
	tab.part_base	= m_pointsInstant.bBasePart;
	tab.skill_group	= m_points.skill_group;
#ifdef ENABLE_NEW_DETAILS_GUI
	std::memcpy(tab.kill_log, m_points.kill_log, sizeof(tab.kill_log));
#endif
	DWORD dwPlayedTime = (get_dword_time() - m_dwPlayStartTime);

	if (dwPlayedTime > 60000)
	{
#ifdef ENABLE_BATTLE_PASS
		CHARACTER_MANAGER::Instance().DoMission(this, MISSION_PLAYTIME, dwPlayedTime / 1000, 0);
#endif
		if (GetSectree() && !GetSectree()->IsAttr(GetX(), GetY(), ATTR_BANPK))
		{
			if (GetRealAlignment() < 0)
			{
				if (IsEquipUniqueItem(UNIQUE_ITEM_FASTER_ALIGNMENT_UP_BY_TIME))
					UpdateAlignment(120 * (dwPlayedTime / 60000));
				else
					UpdateAlignment(60 * (dwPlayedTime / 60000));
			}
			else
				UpdateAlignment(5 * (dwPlayedTime / 60000));
		}

		SetRealPoint(POINT_PLAYTIME, GetRealPoint(POINT_PLAYTIME) + dwPlayedTime / 60000);
		SetPoint(POINT_PLAYTIME,GetRealPoint(POINT_PLAYTIME) + dwPlayedTime / 60000);
		ResetPlayTime(dwPlayedTime % 60000);
	}

#ifdef ENABLE_BATTLE_PASS
	if(m_dwMountStartTime > 0)
	{
		DWORD dwMountPlayedTime = (get_dword_time() - m_dwMountStartTime);
		if (dwMountPlayedTime > 1000)
		{
			CHARACTER_MANAGER::Instance().DoMission(this, MISSION_MOUNT_TIME, dwMountPlayedTime / 1000, 0);
			dwMountPlayedTime = get_dword_time();
		}
	}
#endif

	tab.playtime = GetRealPoint(POINT_PLAYTIME);
	tab.lAlignment = m_iRealAlignment;

	if (m_posWarp.x != 0 || m_posWarp.y != 0)
	{
		tab.x = m_posWarp.x;
		tab.y = m_posWarp.y;
		tab.z = 0;
		tab.lMapIndex = m_lWarpMapIndex;
	}
	else
	{
		tab.x = GetX();
		tab.y = GetY();
		tab.z = GetZ();
		tab.lMapIndex	= GetMapIndex();
	}

	if (m_lExitMapIndex == 0)
	{
		tab.lExitMapIndex	= tab.lMapIndex;
		tab.lExitX		= tab.x;
		tab.lExitY		= tab.y;
	}
	else
	{
		tab.lExitMapIndex	= m_lExitMapIndex;
		tab.lExitX		= m_posExit.x;
		tab.lExitY		= m_posExit.y;
	}

	sys_log(0, "SAVE: %s %dx%d", GetName(), tab.x, tab.y);

	tab.st = GetRealPoint(POINT_ST);
	tab.ht = GetRealPoint(POINT_HT);
	tab.dx = GetRealPoint(POINT_DX);
	tab.iq = GetRealPoint(POINT_IQ);

	tab.stat_point = GetPoint(POINT_STAT);
	tab.skill_point = GetPoint(POINT_SKILL);
	tab.sub_skill_point = GetPoint(POINT_SUB_SKILL);
	tab.horse_skill_point = GetPoint(POINT_HORSE_SKILL);

	tab.stat_reset_count = GetPoint(POINT_STAT_RESET_COUNT);

	tab.hp = GetHP();
	tab.sp = GetSP();

	tab.stamina = GetStamina();

	tab.sRandomHP = m_points.iRandomHP;
	tab.sRandomSP = m_points.iRandomSP;

	if (m_playerData)
	{
		for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
		{
			tab.quickslot[i] = m_playerData->pQuickSlot[i];
		}
	}

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	tab.dwPickUPMode = GetPickUPMode();
	tab.dwRarityMode = GetRarityMode();
#endif

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < UNLOCK_INVENTORY_MAX; ++i)
		tab.inventory_unlock[i] = GetUnlockSlotsW(i);
#endif


#ifdef BATTLEPASS_WORLDARD
	tab.dwBattlePassEndTime = m_dwBattlePassEndTime;
#endif

	if (m_stMobile.length() && !*m_szMobileAuth)
		strlcpy(tab.szMobile, m_stMobile.c_str(), sizeof(tab.szMobile));

	std::memcpy(tab.parts, m_pointsInstant.parts, sizeof(tab.parts));

	// REMOVE_REAL_SKILL_LEVLES
	std::memcpy(tab.skills, m_pSkillLevels, sizeof(TPlayerSkill) * SKILL_MAX_NUM);
	// END_OF_REMOVE_REAL_SKILL_LEVLES
#ifdef ENABLE_YOHARA_SYSTEM
	tab.conqueror_exp	= GetConquerorExp();
	tab.conquerorlevel = GetConquerorLevel();
	tab.conqueror_level_step = GetPoint(POINT_CONQUEROR_LEVEL_STEP);
	tab.sungma_str = GetRealPoint(POINT_SUNGMA_STR);
	tab.sungma_hp = GetRealPoint(POINT_SUNGMA_HP);
	tab.sungma_move = GetRealPoint(POINT_SUNGMA_MOVE);
	tab.sungma_inmune = GetRealPoint(POINT_SUNGMA_IMMUNE);
	tab.conqueror_point = GetPoint(POINT_CONQUEROR_POINT);
#endif

	//sys_err("SUNGMA: %d, %d, %d, %d", tab.sungma_str, tab.sungma_hp, tab.sungma_move, tab.sungma_inmune);
	tab.horse = GetHorseData();

#ifdef ENABLE_BATTLE_PASS
	memset(&tab.battlePassData, 0, sizeof(tab.battlePassData));
	//if (m_battlePassData.size() && GetBattlePassStatus())
	if (m_battlePassData.size())
	{
		for (auto it = m_battlePassData.begin(); it != m_battlePassData.end(); ++it)
		{
			if(it->second > 0)
				tab.battlePassData[it->first] = it->second;
		}
	}
#endif

#ifdef ENABLE_ANTI_EXP
	tab.anti_exp = GetAntiExp();
#endif
#ifdef ENABLE_RANKING
	for (int i = 0; i < RANKING_MAX_CATEGORIES; ++i) {
		tab.lRankPoints[i] = m_lRankPoints[i];
	}
#endif

#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
	if (m_playerData)
	{
		tab.auto_hunt_last_day = m_playerData->m_iAutoHuntLastDay;
		tab.auto_hunt_used_time = m_playerData->m_iAutoHuntUsedTime;
	}
#endif
}


void CHARACTER::SaveReal()
{
	if (m_bSkipSave)
		return;

	if (!GetDesc())
	{
		sys_err("Character::Save : no descriptor when saving (name: %s)", GetName());
		return;
	}

	TPlayerTable table;
	CreatePlayerProto(table);

	db_clientdesc->DBPacket(HEADER_GD_PLAYER_SAVE, GetDesc()->GetHandle(), &table, sizeof(TPlayerTable));

	quest::PC * pkQuestPC = quest::CQuestManager::instance().GetPCForce(GetPlayerID());

	if (!pkQuestPC)
		sys_err("CHARACTER::Save : null quest::PC pointer! (name %s)", GetName());
	else
	{
		pkQuestPC->Save();
	}

	marriage::TMarriage* pMarriage = marriage::CManager::instance().Get(GetPlayerID());
	if (pMarriage)
		pMarriage->Save();
	
	SaveRankingInfo();
}

void CHARACTER::FlushDelayedSaveItem()
{
	// ÀúÀå ¾ÈµÈ ¼ÒÁöÇ°À» ÀüºÎ ÀúÀå½ÃÅ²´Ù.
	LPITEM item;

	for (WORD i = 0; i < INVENTORY_AND_EQUIP_SLOT_MAX; ++i)
	{
		if ((item = GetInventoryItem(i)))
		{
			ITEM_MANAGER::instance().FlushDelayedSave(item);
		}
	}
}

void CHARACTER::Disconnect(const char * c_pszReason)
{
	assert(GetDesc() != NULL);

	sys_log(0, "DISCONNECT: %s (%s)", GetName(), c_pszReason ? c_pszReason : "unset");
#if defined(__BL_BATTLE_ROYALE__)
	BattleRoyale::Instance().OnLogout(this);
#endif
#ifdef __SPIN_WHEEL__
	if (GetProtectTime("spint_itemidx") && GetProtectTime("spint_itemcount"))
	{
		AutoGiveItem(GetProtectTime("spint_itemidx"), GetProtectTime("spint_itemcount"));
		SetProtectTime("spint_itemidx", 0);
		SetProtectTime("spint_itemcount", 0);
	}
#endif
#ifdef RENEWAL_MISSION_BOOKS
	SaveMissionData();
#endif
	if (GetShop())
	{
		GetShop()->RemoveGuest(this);
		SetShop(NULL);
	}

#if defined(WJ_COMBAT_ZONE)
	if (IsPC())
	{	
		CCombatZoneManager::instance().OnLogout(this);
	}
#endif
	if (GetArena() != NULL)
	{
		GetArena()->OnDisconnect(GetPlayerID());
	}

	if (GetParty() != NULL)
	{
		GetParty()->UpdateOfflineState(GetPlayerID());
	}
#ifdef __ROULETTE__
	if (GetProtectTime("roulette_itemIdx") != 0 && GetProtectTime("roulette_itemCount") != 0)
	{
		AutoGiveItem(GetProtectTime("roulette_itemIdx"), GetProtectTime("roulette_itemCount"));
		ChatPacket(CHAT_TYPE_INFO, "[Roulette] Successfully u earned reward.");
		SetProtectTime("roulette_game_key", 0);
		SetProtectTime("roulette_itemIdx", 0);
		SetProtectTime("roulette_itemCount", 0);
	}
#endif

	marriage::CManager::instance().Logout(this);
#ifdef ENABLE_MULTI_FARM_BLOCK
	CHARACTER_MANAGER::Instance().CheckMultiFarmAccount(GetDesc() ? GetDesc()->GetHostName() : "", GetPlayerID(), GetName(), false);
#endif
	// P2P Logout
	TPacketGGLogout p;
	p.bHeader = HEADER_GG_LOGOUT;
	strlcpy(p.szName, GetName(), sizeof(p.szName));
	P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGLogout));
	LogManager::instance().CharLog(this, 0, "LOGOUT", "");

#ifdef ENABLE_PCBANG_FEATURE // @warme006
	{
		long playTime = GetRealPoint(POINT_PLAYTIME) - m_dwLoginPlayTime;
		LogManager::instance().LoginLog(false, GetDesc()->GetAccountTable().id, GetPlayerID(), GetLevel(), GetJob(), playTime);

		if (0)
			CPCBangManager::instance().Log(GetDesc()->GetHostName(), GetPlayerID(), playTime);
	}
#endif

#ifdef ENABLE_PITTY_REFINE
	SaveRefineFlags();
#endif

	if (m_pWarMap)
		SetWarMap(NULL);

	if (m_pWeddingMap)
	{
		SetWeddingMap(NULL);
	}

	if (GetGuild())
		GetGuild()->LogoutMember(this);

	quest::CQuestManager::instance().LogoutPC(this);

	if (GetParty())
		GetParty()->Unlink(this);

	// Á×¾úÀ» ¶§ Á¢¼Ó²÷À¸¸é °æÇèÄ¡ ÁÙ°Ô ÇÏ±â
	if (IsStun() || IsDead())
	{
		DeathPenalty(0);
		PointChange(POINT_HP, 50 - GetHP());
	}

	if (m_petSystem)
	{
		LPITEM item = GetWear(WEAR_PET);
		if (item)
			m_petSystem->DeletePet(item->GetID());
		item = GetWear(WEAR_OLD_PET);
		if (item)
			m_petSystem->DeletePet(item->GetID());
	}

	if (!CHARACTER_MANAGER::instance().FlushDelayedSave(this))
	{
		SaveReal();
	}

	FlushDelayedSaveItem();

	SaveAffect();
	m_bIsLoadedAffect = false;

	m_bSkipSave = true; // ÀÌ ÀÌÈÄ¿¡´Â ´õÀÌ»ó ÀúÀåÇÏ¸é ¾ÈµÈ´Ù.

	quest::CQuestManager::instance().DisconnectPC(this);

	CloseSafebox();
#ifdef ENABLE_BATTLEPASS
    SaveBattlePassProgress();
#endif
	CloseMall();
#ifdef __AURA_SYSTEM__
	if (IsAuraRefineWindowOpen())
		AuraRefineWindowClose();
#endif
	CPVPManager::instance().Disconnect(this);

	CTargetManager::instance().Logout(GetPlayerID());

	MessengerManager::instance().Logout(GetName());

#if defined(ENABLE_SWITCHBOT) && defined(ENABLE_SWITCHBOT_RENEWAL)
	ItemSaveSwitchbot();
#endif

	if (g_TeenDesc)
	{
		int		offset = 0;
		char	buf[245] = {0};

		buf[0] = HEADER_GT_LOGOUT;
		offset += 1;

		memset(buf+offset, 0x00, 2);
		offset += 2;

		TAccountTable	&acc_table = GetDesc()->GetAccountTable();
		memcpy(buf+offset, &acc_table.id, 4);
		offset += 4;

		g_TeenDesc->Packet(buf, offset);
	}

	if (GetDesc())
	{
		GetDesc()->BindCharacter(NULL);
//		BindDesc(NULL);
	}

	M2_DESTROY_CHARACTER(this);
}

bool CHARACTER::Show(long lMapIndex, long x, long y, long z, bool bShowSpawnMotion/* = false */)
{
	LPSECTREE sectree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);

	if (!sectree)
	{
		sys_log(0, "cannot find sectree by %dx%d mapindex %d", x, y, lMapIndex);
		return false;
	}

	SetMapIndex(lMapIndex);

	bool bChangeTree = false;

	if (!GetSectree() || GetSectree() != sectree)
		bChangeTree = true;

	if (bChangeTree)
	{
		if (GetSectree())
			GetSectree()->RemoveEntity(this);

		ViewCleanup();
	}

	if (!IsNPC())
	{
		sys_log(0, "SHOW: %s %dx%dx%d", GetName(), x, y, z);
#ifdef ENABLE_CHECK_MOVESPEED_HACK
		GetCShield()->ResetMoveSpeedhack(get_dword_time());
#endif
		if (GetStamina() < GetMaxStamina())
			StartAffectEvent();
	}
	else if (m_pkMobData)
	{
		m_pkMobInst->m_posLastAttacked.x = x;
		m_pkMobInst->m_posLastAttacked.y = y;
		m_pkMobInst->m_posLastAttacked.z = z;
	}

	if (bShowSpawnMotion)
	{
		SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);
		// m_afAffectFlag.Set(AFF_SPAWN);
	}

	SetXYZ(x, y, z);

	m_posDest.x = x;
	m_posDest.y = y;
	m_posDest.z = z;

	m_posStart.x = x;
	m_posStart.y = y;
	m_posStart.z = z;

	if (bChangeTree)
	{
		EncodeInsertPacket(this);
		sectree->InsertEntity(this);

		UpdateSectree();
	}
	else
	{
		ViewReencode();
		sys_log(0, "      in same sectree");
	}

	REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);

	SetValidComboInterval(0);
	return true;
}

// BGM_INFO
struct BGMInfo
{
	std::string	name;
	float		vol;
};

typedef std::map<unsigned, BGMInfo> BGMInfoMap;

static BGMInfoMap 	gs_bgmInfoMap;
static bool		gs_bgmVolEnable = false;

void CHARACTER_SetBGMVolumeEnable()
{
	gs_bgmVolEnable = true;
	sys_log(0, "bgm_info.set_bgm_volume_enable");
}

void CHARACTER_AddBGMInfo(unsigned mapIndex, const char* name, float vol)
{
	BGMInfo newInfo;
	newInfo.name = name;
	newInfo.vol = vol;

	gs_bgmInfoMap[mapIndex] = newInfo;

	sys_log(0, "bgm_info.add_info(%d, '%s', %f)", mapIndex, name, vol);
}

const BGMInfo& CHARACTER_GetBGMInfo(unsigned mapIndex)
{
	BGMInfoMap::iterator f = gs_bgmInfoMap.find(mapIndex);
	if (gs_bgmInfoMap.end() == f)
	{
		static BGMInfo s_empty = {"", 0.0f};
		return s_empty;
	}
	return f->second;
}

bool CHARACTER_IsBGMVolumeEnable()
{
	return gs_bgmVolEnable;
}
// END_OF_BGM_INFO

#ifdef ENABLE_YOHARA_SYSTEM
bool CHARACTER::IsSungmaMap() const
{
	if (!IsPC())
		return false;

#ifdef ENABLE_QUEEN_NETHIS
	if (SnakeLair::CSnk::instance().IsSnakeMap(GetMapIndex()))
		return true;
#endif
#ifdef ENABLE_WHITE_DRAGON
	if (WhiteDragon::CWhDr::instance().IsWhiteMap(GetMapIndex()))
		return true;
#endif
#ifdef ENABLE_SUNG_MAHI_TOWER
	if (IsSungMahiDungeon(GetMapIndex()))
		return true;
#endif

	const TSungmaTable& r = g_map_SungmaTable[GetMapIndex()];
	if (r.lMapIdx == GetMapIndex())
		return true;

	return false;
}

uint8_t CHARACTER::GetSungmaMapAttribute(BYTE wPoint) const
{
	uint8_t sungma_points = 0;

	if (!IsPC() || !wPoint)
		return sungma_points;

	const TSungmaTable& r = g_map_SungmaTable[GetMapIndex()];
#ifdef ENABLE_QUEEN_NETHIS
	if (!SnakeLair::CSnk::instance().IsSnakeMap(GetMapIndex()))
#endif
#ifdef ENABLE_WHITE_DRAGON
	if (!WhiteDragon::CWhDr::instance().IsWhiteMap(GetMapIndex()))
#endif
#ifdef ENABLE_SUNG_MAHI_TOWER
	if (!IsSungMahiDungeon(GetMapIndex()))
#endif
	{
		if (r.lMapIdx == 0)
			return sungma_points;
	}

	switch (wPoint)
	{
		case POINT_SUNGMA_STR:
		{
			sungma_points = r.bSungmaStr;
#ifdef ENABLE_QUEEN_NETHIS
			if (SnakeLair::CSnk::instance().IsSnakeMap(GetMapIndex()))
				sungma_points = SnakeLair::CSnk::instance().GetSungmaQueenDungeonValue(this, POINT_SUNGMA_STR);
#endif
#ifdef ENABLE_WHITE_DRAGON
			if (WhiteDragon::CWhDr::instance().IsWhiteMap(GetMapIndex()))
				sungma_points = WhiteDragon::CWhDr::instance().GetSungmaWhiteDungeonValue(this, POINT_SUNGMA_STR);
#endif
// #ifdef ENABLE_SUNG_MAHI_TOWER
			// if (IsSungMahiDungeon(GetMapIndex()))
				// sungma_points = GetSungMahiTowerDungeonValue(0/*POINT_SUNGMA_STR*/);
// #endif
		}
		break;

		case POINT_SUNGMA_HP:
		{
			sungma_points = r.bSungmaHp;
#ifdef ENABLE_QUEEN_NETHIS
			if (SnakeLair::CSnk::instance().IsSnakeMap(GetMapIndex()))
				sungma_points = SnakeLair::CSnk::instance().GetSungmaQueenDungeonValue(this, POINT_SUNGMA_HP);
#endif
#ifdef ENABLE_WHITE_DRAGON
			if (WhiteDragon::CWhDr::instance().IsWhiteMap(GetMapIndex()))
				sungma_points = WhiteDragon::CWhDr::instance().GetSungmaWhiteDungeonValue(this, POINT_SUNGMA_HP);
#endif
// #ifdef ENABLE_SUNG_MAHI_TOWER
			// if (IsSungMahiDungeon(GetMapIndex()))
				// sungma_points = GetSungMahiTowerDungeonValue(1/*POINT_SUNGMA_HP*/);
// #endif
		}
		break;

		case POINT_SUNGMA_MOVE:
		{
			sungma_points = r.bSungmaMove;
#ifdef ENABLE_QUEEN_NETHIS
			if (SnakeLair::CSnk::instance().IsSnakeMap(GetMapIndex()))
				sungma_points = SnakeLair::CSnk::instance().GetSungmaQueenDungeonValue(this, POINT_SUNGMA_MOVE);
#endif
#ifdef ENABLE_WHITE_DRAGON
			if (WhiteDragon::CWhDr::instance().IsWhiteMap(GetMapIndex()))
				sungma_points = WhiteDragon::CWhDr::instance().GetSungmaWhiteDungeonValue(this, POINT_SUNGMA_MOVE);
#endif
// #ifdef ENABLE_SUNG_MAHI_TOWER
			// if (IsSungMahiDungeon(GetMapIndex()))
				// sungma_points = GetSungMahiTowerDungeonValue(2/*POINT_SUNGMA_MOVE*/);
// #endif
		}
		break;

		case POINT_SUNGMA_IMMUNE:
		{
			sungma_points = r.bSungmaImmune;
#ifdef ENABLE_QUEEN_NETHIS
			if (SnakeLair::CSnk::instance().IsSnakeMap(GetMapIndex()))
				sungma_points = SnakeLair::CSnk::instance().GetSungmaQueenDungeonValue(this, POINT_SUNGMA_IMMUNE);
#endif
#ifdef ENABLE_WHITE_DRAGON
			if (WhiteDragon::CWhDr::instance().IsWhiteMap(GetMapIndex()))
				sungma_points = WhiteDragon::CWhDr::instance().GetSungmaWhiteDungeonValue(this, POINT_SUNGMA_IMMUNE);
#endif
// #ifdef ENABLE_SUNG_MAHI_TOWER
			// if (IsSungMahiDungeon(GetMapIndex()))
				// sungma_points = GetSungMahiTowerDungeonValue(3/*POINT_SUNGMA_IMMUNE*/);
// #endif
		}
		break;

		default:
			return sungma_points;
			break;
	}

	return sungma_points;
}
#endif

void CHARACTER::MainCharacterPacket()
{
	const unsigned mapIndex = GetMapIndex();
	const BGMInfo& bgmInfo = CHARACTER_GetBGMInfo(mapIndex);

	// SUPPORT_BGM
	if (!bgmInfo.name.empty())
	{
		if (CHARACTER_IsBGMVolumeEnable())
		{
			sys_log(1, "bgm_info.play_bgm_vol(%d, name='%s', vol=%f)", mapIndex, bgmInfo.name.c_str(), bgmInfo.vol);
			TPacketGCMainCharacter4_BGM_VOL mainChrPacket;
			mainChrPacket.header = HEADER_GC_MAIN_CHARACTER4_BGM_VOL;
			mainChrPacket.dwVID = m_vid;
			mainChrPacket.wRaceNum = GetRaceNum();
			mainChrPacket.lx = GetX();
			mainChrPacket.ly = GetY();
			mainChrPacket.lz = GetZ();
			mainChrPacket.empire = GetDesc()->GetEmpire();
			mainChrPacket.skill_group = GetSkillGroup();
			strlcpy(mainChrPacket.szChrName, GetName(), sizeof(mainChrPacket.szChrName));

			mainChrPacket.fBGMVol = bgmInfo.vol;
			strlcpy(mainChrPacket.szBGMName, bgmInfo.name.c_str(), sizeof(mainChrPacket.szBGMName));
			GetDesc()->Packet(&mainChrPacket, sizeof(TPacketGCMainCharacter4_BGM_VOL));
		}
		else
		{
			sys_log(1, "bgm_info.play(%d, '%s')", mapIndex, bgmInfo.name.c_str());
			TPacketGCMainCharacter3_BGM mainChrPacket;
			mainChrPacket.header = HEADER_GC_MAIN_CHARACTER3_BGM;
			mainChrPacket.dwVID = m_vid;
			mainChrPacket.wRaceNum = GetRaceNum();
			mainChrPacket.lx = GetX();
			mainChrPacket.ly = GetY();
			mainChrPacket.lz = GetZ();
			mainChrPacket.empire = GetDesc()->GetEmpire();
			mainChrPacket.skill_group = GetSkillGroup();
			strlcpy(mainChrPacket.szChrName, GetName(), sizeof(mainChrPacket.szChrName));
			strlcpy(mainChrPacket.szBGMName, bgmInfo.name.c_str(), sizeof(mainChrPacket.szBGMName));
			GetDesc()->Packet(&mainChrPacket, sizeof(TPacketGCMainCharacter3_BGM));
		}
		//if (m_stMobile.length())
		//		ChatPacket(CHAT_TYPE_COMMAND, "sms");
	}
	// END_OF_SUPPORT_BGM
	else
	{
		sys_log(0, "bgm_info.play(%d, DEFAULT_BGM_NAME)", mapIndex);

		TPacketGCMainCharacter pack;
		pack.header = HEADER_GC_MAIN_CHARACTER;
		pack.dwVID = m_vid;
		pack.wRaceNum = GetRaceNum();
		pack.lx = GetX();
		pack.ly = GetY();
		pack.lz = GetZ();
		pack.empire = GetDesc()->GetEmpire();
		pack.skill_group = GetSkillGroup();
		strlcpy(pack.szName, GetName(), sizeof(pack.szName));
		GetDesc()->Packet(&pack, sizeof(TPacketGCMainCharacter));

		if (m_stMobile.length())
			ChatPacket(CHAT_TYPE_COMMAND, "sms");
	}
}

void CHARACTER::PointsPacket()
{
	LPDESC d = GetDesc();
	if (!d)
	{
#ifdef __BUFFI_SUPPORT__
		if (!IsBuffi())
		{
			return;
		}
#else
		return;
#endif
	}

	TPacketGCPoints p;
	p.header = HEADER_GC_CHARACTER_POINTS;
	p.points[POINT_LEVEL] = GetLevel();
	p.points[POINT_EXP] = GetExp();
	p.points[POINT_NEXT_EXP] = GetNextExp();
	p.points[POINT_HP] = GetHP();
	p.points[POINT_MAX_HP] = GetMaxHP();
	p.points[POINT_SP] = GetSP();
	p.points[POINT_MAX_SP] = GetMaxSP();
	p.points[POINT_GOLD] = GetGold();
	p.points[POINT_STAMINA] = GetStamina();
	p.points[POINT_MAX_STAMINA] = GetMaxStamina();

	for (int i = POINT_ST; i < POINT_MAX_NUM; ++i)
	{
#ifdef __GEM_SYSTEM__
		if (i == POINT_GEM)
		{
			p.points[POINT_GEM] = GetGem();
			continue;
		}
#endif

#ifdef ENABLE_YOHARA_SYSTEM
		if (i == POINT_CONQUEROR_LEVEL)
		{
			p.points[POINT_CONQUEROR_LEVEL] = GetConquerorLevel();
			continue;
		}

		if (i == POINT_CONQUEROR_EXP)
		{
			p.points[POINT_CONQUEROR_EXP] = GetConquerorExp();
			continue;
		}

		if (i == POINT_CONQUEROR_NEXT_EXP)
		{
			p.points[POINT_CONQUEROR_NEXT_EXP] = GetConquerorNextExp();
			continue;
		}
#endif

#ifdef __GEM_SYSTEM__
		if (i == POINT_VCOINS)
		{
			p.points[POINT_VCOINS] = GetVoteCoins();
			continue;
		}
#endif

#ifdef USE_CHEQUE_CURRENCY
		if (i == POINT_CHEQUE)
		{
			p.points[POINT_CHEQUE] = static_cast<long long>(GetCheque());
			continue;
		}
#endif

		p.points[i] = GetPoint(i);
	}

	if (d)
	{
		d->Packet(&p, sizeof(TPacketGCPoints));
	}
}

bool CHARACTER::ChangeSex()
{
	int src_race = GetRaceNum();

	switch (src_race)
	{
		case MAIN_RACE_WARRIOR_M:
			m_points.job = MAIN_RACE_WARRIOR_W;
			break;

		case MAIN_RACE_WARRIOR_W:
			m_points.job = MAIN_RACE_WARRIOR_M;
			break;

		case MAIN_RACE_ASSASSIN_M:
			m_points.job = MAIN_RACE_ASSASSIN_W;
			break;

		case MAIN_RACE_ASSASSIN_W:
			m_points.job = MAIN_RACE_ASSASSIN_M;
			break;

		case MAIN_RACE_SURA_M:
			m_points.job = MAIN_RACE_SURA_W;
			break;

		case MAIN_RACE_SURA_W:
			m_points.job = MAIN_RACE_SURA_M;
			break;

		case MAIN_RACE_SHAMAN_M:
			m_points.job = MAIN_RACE_SHAMAN_W;
			break;

		case MAIN_RACE_SHAMAN_W:
			m_points.job = MAIN_RACE_SHAMAN_M;
			break;
#ifdef ENABLE_WOLFMAN_CHARACTER
		case MAIN_RACE_WOLFMAN_M:
			m_points.job = MAIN_RACE_WOLFMAN_M;
			break;
#endif
		default:
			sys_err("CHANGE_SEX: %s unknown race %d", GetName(), src_race);
			return false;
	}

	sys_log(0, "CHANGE_SEX: %s (%d -> %d)", GetName(), src_race, m_points.job);
	return true;
}

WORD CHARACTER::GetRaceNum() const
{
	if (m_dwPolymorphRace)
		return m_dwPolymorphRace;

	if (m_pkMobData)
		return m_pkMobData->m_table.dwVnum;

	return m_points.job;
}

void CHARACTER::SetRace(BYTE race)
{
	if (race >= MAIN_RACE_MAX_NUM)
	{
		sys_err("CHARACTER::SetRace(name=%s, race=%d).OUT_OF_RACE_RANGE", GetName(), race);
		return;
	}

	m_points.job = race;
}

BYTE CHARACTER::GetJob() const
{
	unsigned race = m_points.job;
	unsigned job;

	if (RaceToJob(race, &job))
		return job;

	sys_err("CHARACTER::GetJob(name=%s, race=%d).OUT_OF_RACE_RANGE", GetName(), race);
	return JOB_WARRIOR;
}

void CHARACTER::SetLevel(BYTE level)
{
	m_points.level = level;
#if defined(WJ_COMBAT_ZONE)	
	if (IsPC() && CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		return;
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
	if (IsPC() && GetMapIndex() == TOURNAMENT_MAP_INDEX)
		return;
#endif
	if (IsPC())
	{
		if (level < PK_PROTECT_LEVEL)
			SetPKMode(PK_MODE_PROTECT);
		else if (GetGMLevel() != GM_PLAYER)
			SetPKMode(PK_MODE_PROTECT);
		else if (m_bPKMode == PK_MODE_PROTECT)
			SetPKMode(PK_MODE_PEACE);
	}
}

#ifdef ENABLE_YOHARA_SYSTEM
void CHARACTER::SetConquerorLevel(BYTE level)
{
	m_points.conquerorlevel = level;
}
#endif

void CHARACTER::SetEmpire(BYTE bEmpire)
{
	m_bEmpire = bEmpire;
}

#define ENABLE_GM_FLAG_IF_TEST_SERVER
#define ENABLE_GM_FLAG_FOR_LOW_WIZARD
void CHARACTER::SetPlayerProto(const TPlayerTable * t)
{
	if (!GetDesc() || !*GetDesc()->GetHostName())
	{
		sys_err("cannot get desc or hostname");
	}
	else
	{
		SetGMLevel();
	}

	m_playerData = std::make_unique<SPlayerData>();

	m_bCharType = CHAR_TYPE_PC;

	m_dwPlayerID = t->id;

	m_iAlignment = t->lAlignment;
	m_iRealAlignment = t->lAlignment;

	m_points.voice = t->voice;

	m_points.skill_group = t->skill_group;

	m_pointsInstant.bBasePart = t->part_base;
	SetPart(PART_HAIR, t->parts[PART_HAIR]);
#ifdef __AURA_SYSTEM__
	SetPart(PART_AURA, t->parts[PART_AURA]);
#endif
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	SetPickUPMode(t->dwPickUPMode);
	SetRarityMode(t->dwRarityMode);
#endif

#ifdef UNLOCK_INVENTORY_WORLDARD
	for (int i = 0; i < UNLOCK_INVENTORY_MAX; ++i)
		SetUnlockSlotsW(t->inventory_unlock[i],i);
#endif
#ifdef BATTLEPASS_WORLDARD
	m_dwBattlePassEndTime = t->dwBattlePassEndTime;
#endif
	#ifdef __SASH_SYSTEM__
	SetPart(PART_SASH, t->parts[PART_SASH]);
	#endif

	m_points.iRandomHP = t->sRandomHP;
	m_points.iRandomSP = t->sRandomSP;

	// REMOVE_REAL_SKILL_LEVLES
	if (m_pSkillLevels)
		M2_DELETE_ARRAY(m_pSkillLevels);

	m_pSkillLevels = M2_NEW TPlayerSkill[SKILL_MAX_NUM];
	std::memcpy(m_pSkillLevels, t->skills, sizeof(TPlayerSkill) * SKILL_MAX_NUM);
	// END_OF_REMOVE_REAL_SKILL_LEVLES


	if (t->lMapIndex >= 10000)
	{
		m_posWarp.x = t->lExitX;
		m_posWarp.y = t->lExitY;
		m_lWarpMapIndex = t->lExitMapIndex;
	}

	SetRealPoint(POINT_PLAYTIME, t->playtime);
	SetPoint(POINT_PLAYTIME, t->playtime);
	m_dwLoginPlayTime = t->playtime;
	SetRealPoint(POINT_ST, t->st);
	SetRealPoint(POINT_HT, t->ht);
	SetRealPoint(POINT_DX, t->dx);
	SetRealPoint(POINT_IQ, t->iq);

	SetPoint(POINT_ST, t->st);
	SetPoint(POINT_HT, t->ht);
	SetPoint(POINT_DX, t->dx);
	SetPoint(POINT_IQ, t->iq);

	SetPoint(POINT_STAT, t->stat_point);
	SetPoint(POINT_SKILL, t->skill_point);
	SetPoint(POINT_SUB_SKILL, t->sub_skill_point);
	SetPoint(POINT_HORSE_SKILL, t->horse_skill_point);

	SetPoint(POINT_STAT_RESET_COUNT, t->stat_reset_count);

	SetPoint(POINT_LEVEL_STEP, t->level_step);
	SetRealPoint(POINT_LEVEL_STEP, t->level_step);

	SetRace(t->job);

	SetLevel(t->level);
	SetExp(t->exp);
	SetGold(t->gold);
#ifdef USE_CHEQUE_CURRENCY
	SetCheque(t->cheque);
#endif
#ifdef __GEM_SYSTEM__
	SetGem(t->gem);
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	SetConquerorLevel(t->conquerorlevel);
	SetConquerorExp(t->conqueror_exp);

	SetPoint(POINT_CONQUEROR_LEVEL_STEP, t->conqueror_level_step);
	SetRealPoint(POINT_CONQUEROR_LEVEL_STEP, t->conqueror_level_step);

	SetRealPoint(POINT_SUNGMA_STR, t->sungma_str);
	SetRealPoint(POINT_SUNGMA_HP, t->sungma_hp);
	SetRealPoint(POINT_SUNGMA_MOVE, t->sungma_move);
	SetRealPoint(POINT_SUNGMA_IMMUNE, t->sungma_inmune);

	SetPoint(POINT_SUNGMA_STR, t->sungma_str);
	SetPoint(POINT_SUNGMA_HP, t->sungma_hp);
	SetPoint(POINT_SUNGMA_MOVE, t->sungma_move);
	SetPoint(POINT_SUNGMA_IMMUNE, t->sungma_inmune);
	SetPoint(POINT_CONQUEROR_POINT, t->conqueror_point);
#endif

	//sys_err("SUNGMA 2: %d, %d, %d, %d", t->sungma_str, t->sungma_hp, t->sungma_move, t->sungma_inmune);
	
	SetMapIndex(t->lMapIndex);
	SetXYZ(t->x, t->y, t->z);

	ComputePoints();
#ifdef ENABLE_ANTI_EXP
	SetAntiExp(t->anti_exp);
#endif
	SetHP(t->hp);
	SetSP(t->sp);
	SetStamina(t->stamina);

	//GMÀÏ¶§ º¸È£¸ðµå
#ifndef ENABLE_GM_FLAG_IF_TEST_SERVER
	if (!test_server)
#endif
	{
#ifdef ENABLE_GM_FLAG_FOR_LOW_WIZARD
		if (GetGMLevel() > GM_PLAYER)
#else
		if (GetGMLevel() > GM_LOW_WIZARD)
#endif
		{
			m_afAffectFlag.Set(AFF_YMIR);
			m_bPKMode = PK_MODE_PROTECT;
		}
	}

	if (GetLevel() < PK_PROTECT_LEVEL)
		m_bPKMode = PK_MODE_PROTECT;

	m_stMobile = t->szMobile;

	SetHorseData(t->horse);

	if (GetHorseLevel() > 0)
		UpdateHorseDataByLogoff(t->logoff_interval);

	std::memcpy(m_aiPremiumTimes, t->aiPremiumTimes, sizeof(t->aiPremiumTimes));
#ifdef ENABLE_BATTLE_PASS
	for (BYTE j = 0; j < MISSION_MAX; ++j)
	{
		//sys_err("load: index %d value %lld ",j,t->battlePassData[j]);
		if (t->battlePassData[j] > 0)
			m_battlePassData.emplace(j, t->battlePassData[j]);
	}
#endif
#ifdef ENABLE_NEW_DETAILS_GUI
	std::memcpy(m_points.kill_log, t->kill_log, sizeof(m_points.kill_log));
#endif
	m_dwLogOffInterval = t->logoff_interval;

	sys_log(0, "PLAYER_LOAD: %s PREMIUM %d %d, LOGGOFF_INTERVAL %u PTR: %p", t->name, m_aiPremiumTimes[0], m_aiPremiumTimes[1], t->logoff_interval, this);

	if (GetGMLevel() != GM_PLAYER)
	{
		LogManager::instance().CharLog(this, GetGMLevel(), "GM_LOGIN", "");
		sys_log(0, "GM_LOGIN(gmlevel=%d, name=%s(%d), pos=(%d, %d)", GetGMLevel(), GetName(), GetPlayerID(), GetX(), GetY());
	}

#ifdef ENABLE_SUPPORT_SYSTEM
	if (m_supportSystem)
	{
		m_supportSystem->Destroy();
		delete m_supportSystem;
	}

	m_supportSystem = M2_NEW CSupportSystem(this);
#endif

#ifdef ENABLE_RANKING
	for (int i = 0; i < RANKING_MAX_CATEGORIES; ++i)
		m_lRankPoints[i] = t->lRankPoints[i];
#endif

#ifdef __PET_SYSTEM__
	// NOTE: ÀÏ´Ü Ä³¸¯ÅÍ°¡ PCÀÎ °æ¿ì¿¡¸¸ PetSystemÀ» °®µµ·Ï ÇÔ. À¯·´ ¸Ó½Å´ç ¸Þ¸ð¸® »ç¿ë·ü¶§¹®¿¡ NPC±îÁö ÇÏ±ä Á»..
	if (m_petSystem)
	{
		m_petSystem->Destroy();
		delete m_petSystem;
	}

	m_petSystem = M2_NEW CPetSystem(this);
#endif
#ifdef ENABLE_ASLAN_BUFF_NPC_SYSTEM
	if (m_buffNPCSystem)
	{
		m_buffNPCSystem->Destroy();
		delete m_buffNPCSystem;
	}

	m_buffNPCSystem = M2_NEW CBuffNPCActor(this);
#endif
#ifdef __NEWPET_SYSTEM__
	// NOTE: AI¢¥U A©ø¢¬?AI¡Æ¢® PCAI ¡Æ©¡¢¯i¢¯¢®¢¬¢¬ PetSystemA¡í ¡Æ¢ç¥ì¥ì¡¤I CO. A?¡¤¢¥ ¢¬O¨öA¢¥c ¢¬¨­¢¬©£¢¬¢ç ¡íc¢¯e¡¤u¢Ò¡×©ö¢ç¢¯¢® NPC¡¾iAo CI¡¾a A¡í..
	if (m_newpetSystem)
	{
		m_newpetSystem->Destroy();
		delete m_newpetSystem;
	}

	m_newpetSystem = M2_NEW CNewPetSystem(this);
#endif
#ifdef ENABLE_MOUNT_SYSTEM
	if (m_MountSystem)
	{
		m_MountSystem->Destroy();
		delete m_MountSystem;
	}

	m_MountSystem = M2_NEW CMountSystem(this);
#endif
#ifdef ENABLE_BATTLEPASS
    LoadBattlePassProgress();
#endif

#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
	if (m_playerData)
	{
		m_playerData->m_iAutoHuntLastDay = t->auto_hunt_last_day;
		m_playerData->m_iAutoHuntUsedTime = t->auto_hunt_used_time;
		m_playerData->m_iAutoHuntLastUpdate = 0;
	}
#endif

#ifdef USE_BRAVE_CAPE_RENEWED
	if (m_playerData)
	{
		m_playerData->m_dwReviveTime = 0;
	}
#endif
}

EVENTFUNC(kill_ore_load_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info);
	if ( info == NULL )
	{
		sys_err( "kill_ore_load_even> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;
	if (ch == NULL) { // <Factor>
		return 0;
	}

	ch->m_pkMiningEvent = NULL;
	M2_DESTROY_CHARACTER(ch);
	return 0;
}

void CHARACTER::SetProto(const CMob * pkMob)
{
	if (m_pkMobInst)
		M2_DELETE(m_pkMobInst);

	m_pkMobData = pkMob;
	m_pkMobInst = M2_NEW CMobInstance;

	m_bPKMode = PK_MODE_FREE;

	const TMobTable * t = &m_pkMobData->m_table;

	m_bCharType = t->bType;

	SetLevel(t->bLevel);
	SetEmpire(t->bEmpire);

	SetExp(t->dwExp);
#ifdef ENABLE_YOHARA_PROTO_EXT
	SetConquerorExp(t->dwSungMaExp);
#endif
	SetRealPoint(POINT_ST, t->bStr);
	SetRealPoint(POINT_DX, t->bDex);
	SetRealPoint(POINT_HT, t->bCon);
	SetRealPoint(POINT_IQ, t->bInt);

	ComputePoints();

	SetHP(GetMaxHP());
	SetSP(GetMaxSP());

	////////////////////
	m_pointsInstant.dwAIFlag = t->dwAIFlag;
	SetImmuneFlag(t->dwImmuneFlag);

	AssignTriggers(t);

	ApplyMobAttribute(t);

	if (IsStone())
	{
		DetermineDropMetinStone();
		DetermineDropChestBooksMetin();
	}

	if (IsWarp() || IsGoto())
	{
		StartWarpNPCEvent();
	}

	CHARACTER_MANAGER::instance().RegisterRaceNumMap(this);

	// XXX X-mas santa hardcoding
	if (GetRaceNum() == xmas::MOB_SANTA_VNUM)
	{
		SetPoint(POINT_ATT_GRADE_BONUS, 10);
		SetPoint(POINT_DEF_GRADE_BONUS, 6);

		//»êÅ¸¿ë
		//m_dwPlayStartTime = get_dword_time() + 10 * 60 * 1000;
		//½Å¼±ÀÚ ³ëÇØ
		m_dwPlayStartTime = get_dword_time() + 30 * 1000;
		if (test_server)
			m_dwPlayStartTime = get_dword_time() + 30 * 1000;
	}

	// XXX CTF GuildWar hardcoding
	if (warmap::IsWarFlag(GetRaceNum()))
	{
		m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlag, &CHARACTER::EndStateEmpty);
		m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlag, &CHARACTER::EndStateEmpty);
		m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlag, &CHARACTER::EndStateEmpty);
	}

	if (warmap::IsWarFlagBase(GetRaceNum()))
	{
		m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlagBase, &CHARACTER::EndStateEmpty);
		m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlagBase, &CHARACTER::EndStateEmpty);
		m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlagBase, &CHARACTER::EndStateEmpty);
	}
#ifdef ENABLE_NEW_PET_SYSTEM
	if (m_bCharType == CHAR_TYPE_PET)
	{
		m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StatePet, &CHARACTER::EndStateEmpty);
		m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateMove, &CHARACTER::EndStateEmpty);
		m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StatePet, &CHARACTER::EndStateEmpty);
	}
#endif

	if (m_bCharType == CHAR_TYPE_HORSE ||
			GetRaceNum() == 20101 ||
			GetRaceNum() == 20102 ||
			GetRaceNum() == 20103 ||
			GetRaceNum() == 20104 ||
			GetRaceNum() == 20105 ||
			GetRaceNum() == 20106 ||
			GetRaceNum() == 20107 ||
			GetRaceNum() == 20108 ||
			GetRaceNum() == 20109
	  )
	{
		m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateHorse, &CHARACTER::EndStateEmpty);
		m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateMove, &CHARACTER::EndStateEmpty);
		m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateHorse, &CHARACTER::EndStateEmpty);
	}

	// MINING
	if (mining::IsVeinOfOre (GetRaceNum()))
	{
		char_event_info* info = AllocEventInfo<char_event_info>();

		info->ch = this;

		m_pkMiningEvent = event_create(kill_ore_load_event, info, PASSES_PER_SEC(number(7 * 60, 15 * 60)));
	}
	// END_OF_MINING
}

const TMobTable & CHARACTER::GetMobTable() const
{
	return m_pkMobData->m_table;
}

bool CHARACTER::IsRaceFlag(DWORD dwBit) const
{
	return m_pkMobData ? IS_SET(m_pkMobData->m_table.dwRaceFlag, dwBit) : 0;
}

DWORD CHARACTER::GetMobDamageMin() const
{
	return m_pkMobData->m_table.dwDamageRange[0];
}

DWORD CHARACTER::GetMobDamageMax() const
{
	return m_pkMobData->m_table.dwDamageRange[1];
}

float CHARACTER::GetMobDamageMultiply() const
{
	float fDamMultiply = GetMobTable().fDamMultiply;

	if (IsBerserk())
		fDamMultiply = fDamMultiply * 2.0f; // BALANCE: ±¤ÆøÈ­ ½Ã µÎ¹è

	return fDamMultiply;
}

DWORD CHARACTER::GetMobDropItemVnum() const
{
	return m_pkMobData->m_table.dwDropItemVnum;
}

bool CHARACTER::IsSummonMonster() const
{
	return GetSummonVnum() != 0;
}

DWORD CHARACTER::GetSummonVnum() const
{
	return m_pkMobData ? m_pkMobData->m_table.dwSummonVnum : 0;
}

DWORD CHARACTER::GetPolymorphItemVnum() const
{
	return m_pkMobData ? m_pkMobData->m_table.dwPolymorphItemVnum : 0;
}

DWORD CHARACTER::GetMonsterDrainSPPoint() const
{
	return m_pkMobData ? m_pkMobData->m_table.dwDrainSP : 0;
}

BYTE CHARACTER::GetMobRank() const
{
	if (!m_pkMobData)
		return MOB_RANK_KNIGHT;	// PCÀÏ °æ¿ì KNIGHT±Þ

	return m_pkMobData->m_table.bRank;
}

BYTE CHARACTER::GetMobSize() const
{
	if (!m_pkMobData)
		return MOBSIZE_MEDIUM;

	return m_pkMobData->m_table.bSize;
}

WORD CHARACTER::GetMobAttackRange() const
{
	switch (GetMobBattleType())
	{
		case BATTLE_TYPE_RANGE:
		case BATTLE_TYPE_MAGIC:
			return m_pkMobData->m_table.wAttackRange + GetPoint(POINT_BOW_DISTANCE);
		default:
			return m_pkMobData->m_table.wAttackRange;
	}
}

BYTE CHARACTER::GetMobBattleType() const
{
	if (!m_pkMobData)
		return BATTLE_TYPE_MELEE;

	return (m_pkMobData->m_table.bBattleType);
}

void CHARACTER::ComputeBattlePoints()
{
	if (IsPolymorphed())
	{
		DWORD dwMobVnum = GetPolymorphVnum();
		const CMob * pMob = CMobManager::instance().Get(dwMobVnum);
		int iAtt = 0;
		int iDef = 0;

		if (pMob)
		{
			iAtt = GetLevel() * 2 + GetPolymorphPoint(POINT_ST) * 2;
			// lev + con
			iDef = GetLevel() + GetPolymorphPoint(POINT_HT) + pMob->m_table.wDef;
		}

		SetPoint(POINT_ATT_GRADE, iAtt);
		SetPoint(POINT_DEF_GRADE, iDef);
		SetPoint(POINT_MAGIC_ATT_GRADE, GetPoint(POINT_ATT_GRADE));
		SetPoint(POINT_MAGIC_DEF_GRADE, GetPoint(POINT_DEF_GRADE));
	}
	else if (IsPC()
#ifdef __FAKE_PLAYER__
		|| IsFakePlayer()
#endif
	)
	{
		SetPoint(POINT_ATT_GRADE, 0);
		SetPoint(POINT_DEF_GRADE, 0);
		SetPoint(POINT_CLIENT_DEF_GRADE, 0);
		SetPoint(POINT_MAGIC_ATT_GRADE, GetPoint(POINT_ATT_GRADE));
		SetPoint(POINT_MAGIC_DEF_GRADE, GetPoint(POINT_DEF_GRADE));

		//
		// ±âº» ATK = 2lev + 2str, Á÷¾÷¿¡ ¸¶´Ù 2strÀº ¹Ù²ð ¼ö ÀÖÀ½
		//
		int iAtk = GetLevel() * 2;
		int iStatAtk = 0;

		switch (GetJob())
		{
			case JOB_WARRIOR:
			case JOB_SURA:
				iStatAtk = (2 * GetPoint(POINT_ST));
				break;

			case JOB_ASSASSIN:
				iStatAtk = (4 * GetPoint(POINT_ST) + 2 * GetPoint(POINT_DX)) / 3;
				break;

			case JOB_SHAMAN:
				iStatAtk = (4 * GetPoint(POINT_ST) + 2 * GetPoint(POINT_IQ)) / 3;
				break;
#ifdef ENABLE_WOLFMAN_CHARACTER
			case JOB_WOLFMAN:
				// TODO: ¼öÀÎÁ· °ø°Ý·Â °ø½Ä ±âÈ¹ÀÚ¿¡°Ô ¿äÃ»
				iStatAtk = (2 * GetPoint(POINT_ST));
				break;
#endif
			default:
				sys_err("invalid job %d", GetJob());
				iStatAtk = (2 * GetPoint(POINT_ST));
				break;
		}

		// ¸»À» Å¸°í ÀÖ°í, ½ºÅÈÀ¸·Î ÀÎÇÑ °ø°Ý·ÂÀÌ ST*2 º¸´Ù ³·À¸¸é ST*2·Î ÇÑ´Ù.
		// ½ºÅÈÀ» Àß¸ø ÂïÀº »ç¶÷ °ø°Ý·ÂÀÌ ´õ ³·Áö ¾Ê°Ô ÇÏ±â À§ÇØ¼­´Ù.
		if (GetMountVnum() && iStatAtk < 2 * GetPoint(POINT_ST))
			iStatAtk = (2 * GetPoint(POINT_ST));

		iAtk += iStatAtk;

		// ½Â¸¶(¸») : °Ë¼ö¶ó µ¥¹ÌÁö °¨¼Ò
		if (GetMountVnum())
		{
			if (GetJob() == JOB_SURA && GetSkillGroup() == 1)
			{
				iAtk += (iAtk * GetHorseLevel()) / 60;
			}
			else
			{
				iAtk += (iAtk * GetHorseLevel()) / 30;
			}
		}

		//
		// ATK Setting
		//
		iAtk += GetPoint(POINT_ATT_GRADE_BONUS);

		PointChange(POINT_ATT_GRADE, iAtk);

		// DEF = LEV + CON + ARMOR
		int iShowDef = GetLevel() + GetPoint(POINT_HT); // For Ymir(Ãµ¸¶)
		int iDef = GetLevel() + (int) (GetPoint(POINT_HT) / 1.25); // For Other
		int iArmor = 0;

		LPITEM pkItem;

		for (int i = 0; i < WEAR_MAX_NUM; ++i)
			if ((pkItem = GetWear(i)) && pkItem->GetType() == ITEM_ARMOR)
			{
				if (pkItem->GetSubType() == ARMOR_BODY || pkItem->GetSubType() == ARMOR_HEAD || pkItem->GetSubType() == ARMOR_FOOTS || pkItem->GetSubType() == ARMOR_SHIELD)
				{
					iArmor += pkItem->GetValue(1);
					iArmor += (2 * pkItem->GetValue(5));
				}
			}
#ifdef __AURA_SYSTEM__
			else if (pkItem && pkItem->GetType() == ITEM_COSTUME && pkItem->GetSubType() == COSTUME_AURA)
			{
				const long c_lLevelSocket = pkItem->GetSocket(ITEM_SOCKET_AURA_CURRENT_LEVEL);
				const long c_lDrainSocket = pkItem->GetSocket(ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM);
				const long c_lBoostSocket = pkItem->GetSocket(ITEM_SOCKET_AURA_BOOST);

				BYTE bCurLevel = (c_lLevelSocket / 100000) - 1000;
				BYTE bBoostIndex = c_lBoostSocket / 100000000;

				TItemTable* pBoosterProto = ITEM_MANAGER::instance().GetTable(ITEM_AURA_BOOST_ITEM_VNUM_BASE + bBoostIndex);
				float fAuraDrainPer = (1.0f * bCurLevel / 10.0f) / 100.0f;
				if (pBoosterProto)
					fAuraDrainPer += 1.0f * pBoosterProto->alValues[ITEM_AURA_BOOST_PERCENT_VALUE] / 100.0f;

				TItemTable* pDrainedItem = NULL;
				if (c_lDrainSocket != 0)
					pDrainedItem = ITEM_MANAGER::instance().GetTable(c_lDrainSocket);
				if (pDrainedItem != NULL && pDrainedItem->bType == ITEM_ARMOR && pDrainedItem->bSubType == ARMOR_SHIELD)
				{
					float fValue = (pDrainedItem->alValues[1] + (2 * pDrainedItem->alValues[5])) * fAuraDrainPer;
					iArmor += static_cast<int>((fValue < 1.0f) ? ceilf(fValue) : truncf(fValue));;
				}
			}
#endif

		// ¸» Å¸°í ÀÖÀ» ¶§ ¹æ¾î·ÂÀÌ ¸»ÀÇ ±âÁØ ¹æ¾î·Âº¸´Ù ³·À¸¸é ±âÁØ ¹æ¾î·ÂÀ¸·Î ¼³Á¤
		if( true == IsHorseRiding() )
		{
			if (iArmor < GetHorseArmor())
				iArmor = GetHorseArmor();

			const char* pHorseName = CHorseNameManager::instance().GetHorseName(GetPlayerID());

			if (pHorseName != NULL && strlen(pHorseName))
			{
				iArmor += 20;
			}
		}

		iArmor += GetPoint(POINT_DEF_GRADE_BONUS);
		iArmor += GetPoint(POINT_PARTY_DEFENDER_BONUS);

		// INTERNATIONAL_VERSION
		PointChange(POINT_DEF_GRADE, iDef + iArmor);
		PointChange(POINT_CLIENT_DEF_GRADE, (iShowDef + iArmor) - GetPoint(POINT_DEF_GRADE));
		// END_OF_INTERNATIONAL_VERSION

		PointChange(POINT_MAGIC_ATT_GRADE, GetLevel() * 2 + GetPoint(POINT_IQ) * 2 + GetPoint(POINT_MAGIC_ATT_GRADE_BONUS));
		PointChange(POINT_MAGIC_DEF_GRADE, GetLevel() + (GetPoint(POINT_IQ) * 3 + GetPoint(POINT_HT)) / 3 + iArmor / 2 + GetPoint(POINT_MAGIC_DEF_GRADE_BONUS));
	}
	else
	{
		// 2lev + str * 2
		int iAtt = GetLevel() * 2 + GetPoint(POINT_ST) * 2;
		// lev + con
		int iDef = GetLevel() + GetPoint(POINT_HT) + GetMobTable().wDef;

		SetPoint(POINT_ATT_GRADE, iAtt);
		SetPoint(POINT_DEF_GRADE, iDef);
		SetPoint(POINT_MAGIC_ATT_GRADE, GetPoint(POINT_ATT_GRADE));
		SetPoint(POINT_MAGIC_DEF_GRADE, GetPoint(POINT_DEF_GRADE));
	}
}

void CHARACTER::ComputePoints()
{
	long lStat = GetPoint(POINT_STAT);
	long lStatResetCount = GetPoint(POINT_STAT_RESET_COUNT);
	long lSkillActive = GetPoint(POINT_SKILL);
	long lSkillSub = GetPoint(POINT_SUB_SKILL);
	long lSkillHorse = GetPoint(POINT_HORSE_SKILL);
	long lLevelStep = GetPoint(POINT_LEVEL_STEP);

	long lAttackerBonus = GetPoint(POINT_PARTY_ATTACKER_BONUS);
	long lTankerBonus = GetPoint(POINT_PARTY_TANKER_BONUS);
	long lBufferBonus = GetPoint(POINT_PARTY_BUFFER_BONUS);
	long lSkillMasterBonus = GetPoint(POINT_PARTY_SKILL_MASTER_BONUS);
	long lHasteBonus = GetPoint(POINT_PARTY_HASTE_BONUS);
	long lDefenderBonus = GetPoint(POINT_PARTY_DEFENDER_BONUS);

	long lHPRecovery = GetPoint(POINT_HP_RECOVERY);
	long lSPRecovery = GetPoint(POINT_SP_RECOVERY);
#ifdef ENABLE_YOHARA_SYSTEM
	long lConquerorStat = GetPoint(POINT_CONQUEROR_POINT);
	long lConquerorLevelStep = GetPoint(POINT_CONQUEROR_LEVEL_STEP);
#endif


	memset(m_pointsInstant.points, 0, sizeof(m_pointsInstant.points));
	BuffOnAttr_ClearAll();
	m_SkillDamageBonus.clear();

	SetPoint(POINT_STAT, lStat);
	SetPoint(POINT_SKILL, lSkillActive);
	SetPoint(POINT_SUB_SKILL, lSkillSub);
	SetPoint(POINT_HORSE_SKILL, lSkillHorse);
	SetPoint(POINT_LEVEL_STEP, lLevelStep);
	SetPoint(POINT_STAT_RESET_COUNT, lStatResetCount);

	SetPoint(POINT_ST, GetRealPoint(POINT_ST));
	SetPoint(POINT_HT, GetRealPoint(POINT_HT));
	SetPoint(POINT_DX, GetRealPoint(POINT_DX));
	SetPoint(POINT_IQ, GetRealPoint(POINT_IQ));
#ifdef ENABLE_YOHARA_SYSTEM
	SetPoint(POINT_CONQUEROR_POINT, lConquerorStat);
	SetPoint(POINT_CONQUEROR_LEVEL_STEP, lConquerorLevelStep);

	SetPoint(POINT_SUNGMA_STR, GetRealPoint(POINT_SUNGMA_STR));
	SetPoint(POINT_SUNGMA_HP, GetRealPoint(POINT_SUNGMA_HP));
	SetPoint(POINT_SUNGMA_MOVE, GetRealPoint(POINT_SUNGMA_MOVE));
	SetPoint(POINT_SUNGMA_IMMUNE, GetRealPoint(POINT_SUNGMA_IMMUNE));
#endif

	SetPart(PART_MAIN, GetOriginalPart(PART_MAIN));
	SetPart(PART_WEAPON, GetOriginalPart(PART_WEAPON));
	SetPart(PART_HEAD, GetOriginalPart(PART_HEAD));
	SetPart(PART_HAIR, GetOriginalPart(PART_HAIR));
	#ifdef __SASH_SYSTEM__
	SetPart(PART_SASH, GetOriginalPart(PART_SASH));
	#endif
#ifdef __AURA_SYSTEM__
	SetPart(PART_AURA, GetOriginalPart(PART_AURA));
#endif

	SetPart(PART_ARROW_TYPE, GetOriginalPart(PART_ARROW_TYPE));
	
	SetPoint(POINT_PARTY_ATTACKER_BONUS, lAttackerBonus);
	SetPoint(POINT_PARTY_TANKER_BONUS, lTankerBonus);
	SetPoint(POINT_PARTY_BUFFER_BONUS, lBufferBonus);
	SetPoint(POINT_PARTY_SKILL_MASTER_BONUS, lSkillMasterBonus);
	SetPoint(POINT_PARTY_HASTE_BONUS, lHasteBonus);
	SetPoint(POINT_PARTY_DEFENDER_BONUS, lDefenderBonus);

	SetPoint(POINT_HP_RECOVERY, lHPRecovery);
	SetPoint(POINT_SP_RECOVERY, lSPRecovery);

	// PC_BANG_ITEM_ADD
	SetPoint(POINT_PC_BANG_EXP_BONUS, 0);
	SetPoint(POINT_PC_BANG_DROP_BONUS, 0);
	// END_PC_BANG_ITEM_ADD
#ifdef VOTE_BUFF_RENEWAL
	SetPoint(POINT_VCOINS, GetVoteCoins());
#endif

	int iMaxHP, iMaxSP;
	int iMaxStamina;

	if (IsPC()
#ifdef __FAKE_PLAYER__
		|| IsFakePlayer()
 #endif	
	)
	{
		// ÃÖ´ë »ý¸í·Â/Á¤½Å·Â
		iMaxHP = JobInitialPoints[GetJob()].max_hp + m_points.iRandomHP + GetPoint(POINT_HT) * JobInitialPoints[GetJob()].hp_per_ht;
		iMaxSP = JobInitialPoints[GetJob()].max_sp + m_points.iRandomSP + GetPoint(POINT_IQ) * JobInitialPoints[GetJob()].sp_per_iq;
		iMaxStamina = JobInitialPoints[GetJob()].max_stamina + GetPoint(POINT_HT) * JobInitialPoints[GetJob()].stamina_per_con;

		{
			CSkillProto* pkSk = CSkillManager::instance().Get(SKILL_ADD_HP);

			if (NULL != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(SKILL_ADD_HP) / 100.0f);

				iMaxHP += static_cast<int>(pkSk->kPointPoly.Eval());
			}
		}

#ifdef USE_PASSIVE_ABILITY_TAMER
		{
			for (uint8_t i = PASSIVE_ABILITY_VNUM_TAMER_1; i < PASSIVE_ABILITY_VNUM_TAMER_MAX; i++)
			{
				const auto it = m_mapPassiveAbilityTamerBnsType.find(i);
				if (it == m_mapPassiveAbilityTamerBnsType.end() || it->second == POINT_NONE)
				{
					continue;
				}

				int tamerLvl = GetSkillLevel(i);
				tamerLvl = tamerLvl > SKILL_MAX_LEVEL ? SKILL_MAX_LEVEL : tamerLvl;
				tamerLvl = tamerLvl < 0 ? 0 : tamerLvl;

				if (tamerLvl == 0)
				{
					continue;
				}

				PointChange(it->second, m_vPassiveAbilityTamerBnsValue[it->first - PASSIVE_ABILITY_VNUM_TAMER_1][tamerLvl]);
			}
		}
#endif

		// ±âº» °ªµé
		//SetPoint(POINT_MOV_SPEED,	100);
		SetPoint(POINT_MOV_SPEED,	230);
		SetPoint(POINT_ATT_SPEED,	162);
		PointChange(POINT_ATT_SPEED, GetPoint(POINT_PARTY_HASTE_BONUS));
		SetPoint(POINT_CASTING_SPEED,	100);
#ifdef __GEM_SYSTEM__
		SetPoint(POINT_GEM, GetGem());
#endif
	}
	else
	{
		iMaxHP = m_pkMobData->m_table.dwMaxHP;
		iMaxSP = 0;
		iMaxStamina = 0;

		SetPoint(POINT_ATT_SPEED, m_pkMobData->m_table.sAttackSpeed);
		SetPoint(POINT_MOV_SPEED, m_pkMobData->m_table.sMovingSpeed);
		SetPoint(POINT_CASTING_SPEED, m_pkMobData->m_table.sAttackSpeed);
	}

	if (IsPC())
	{
		// ¸» Å¸°í ÀÖÀ» ¶§´Â ±âº» ½ºÅÈÀÌ ¸»ÀÇ ±âÁØ ½ºÅÈº¸´Ù ³·À¸¸é ³ô°Ô ¸¸µç´Ù.
		// µû¶ó¼­ ¸»ÀÇ ±âÁØ ½ºÅÈÀÌ ¹«»ç ±âÁØÀÌ¹Ç·Î, ¼ö¶ó/¹«´çÀº ÀüÃ¼ ½ºÅÈ ÇÕÀÌ
		// ´ëÃ¤ÀûÀ¸·Î ´õ ¿Ã¶ó°¡°Ô µÉ °ÍÀÌ´Ù.
		if (GetMountVnum())
		{
			if (GetHorseST() > GetPoint(POINT_ST))
				PointChange(POINT_ST, GetHorseST() - GetPoint(POINT_ST));

			if (GetHorseDX() > GetPoint(POINT_DX))
				PointChange(POINT_DX, GetHorseDX() - GetPoint(POINT_DX));

			if (GetHorseHT() > GetPoint(POINT_HT))
				PointChange(POINT_HT, GetHorseHT() - GetPoint(POINT_HT));

			if (GetHorseIQ() > GetPoint(POINT_IQ))
				PointChange(POINT_IQ, GetHorseIQ() - GetPoint(POINT_IQ));
		}

	}

	ComputeBattlePoints();

	// ±âº» HP/SP ¼³Á¤
	if (iMaxHP != GetMaxHP())
	{
		SetRealPoint(POINT_MAX_HP, iMaxHP); // ±âº»HP¸¦ RealPoint¿¡ ÀúÀåÇØ ³õ´Â´Ù.
	}

	PointChange(POINT_MAX_HP, 0);

	if (iMaxSP != GetMaxSP())
	{
		SetRealPoint(POINT_MAX_SP, iMaxSP); // ±âº»SP¸¦ RealPoint¿¡ ÀúÀåÇØ ³õ´Â´Ù.
	}

	PointChange(POINT_MAX_SP, 0);

	SetMaxStamina(iMaxStamina);
	// @fixme118 part1
	int iCurHP = this->GetHP();
	int iCurSP = this->GetSP();

	m_pointsInstant.dwImmuneFlag = 0;

	for (int i = 0 ; i < WEAR_MAX_NUM; i++)
	{
		LPITEM pItem = GetWear(i);
		if (pItem)
		{
			pItem->ModifyPoints(true);
			SET_BIT(m_pointsInstant.dwImmuneFlag, GetWear(i)->GetImmuneFlag());
		}
	}

#ifdef ENABLE_EVENT_MANAGER
	CHARACTER_MANAGER::Instance().CheckBonusEvent(this);
#endif

#ifdef __NEW_SET_BONUS__
	for (auto it = m_mapNewSetBonus.begin(); it != m_mapNewSetBonus.end(); ++it)
	{
		const TNewSetBonus& setBonus = it->second;  // Use reference to avoid unnecessary copying
		BYTE bPartCount = 0;

		for (BYTE i = 0; i < setBonus.vecSlots.size(); ++i)
		{
			const LPITEM item = GetWear(setBonus.vecSlots[i].first);

			if (item)
			{
				// Check if the equipped item's Vnum is in the list of possible item IDs
				const std::vector<DWORD>& possibleItemIDs = setBonus.vecSlots[i].second;
				if (std::find(possibleItemIDs.begin(), possibleItemIDs.end(), item->GetVnum()) != possibleItemIDs.end())
				{
					bPartCount += 1;
				}
			}
		}

		if (bPartCount)
		{
			// Find the corresponding bonus count based on the part count
			const auto itPart = setBonus.mapPartCountToBonusCount.find(bPartCount);
			BYTE givedBonus = (itPart != setBonus.mapPartCountToBonusCount.end()) ? itPart->second : 0;

			// Apply the bonuses based on the given bonus count
			for (BYTE i = 1; i <= 5; ++i)  // i starts from 1 as per your original logic
			{
				if (!givedBonus)
					break;

				const auto bonusIt = setBonus.mapBonus.find(i);
				if (bonusIt != setBonus.mapBonus.end())
				{
					ApplyPoint(bonusIt->second.first, bonusIt->second.second);
				}
				givedBonus -= 1;
			}
		}
	}
#endif

	if (DragonSoul_IsDeckActivated())
	{
		for (int i = WEAR_MAX_NUM + DS_SLOT_MAX * DragonSoul_GetActiveDeck();
			i < WEAR_MAX_NUM + DS_SLOT_MAX * (DragonSoul_GetActiveDeck() + 1); i++)
		{
			LPITEM pItem = GetWear(i);
			if (pItem)
			{
				if (DSManager::instance().IsTimeLeftDragonSoul(pItem))
					pItem->ModifyPoints(true);
			}
		}
	}

	if (GetHP() > GetMaxHP())
		PointChange(POINT_HP, GetMaxHP() - GetHP());

	if (GetSP() > GetMaxSP())
		PointChange(POINT_SP, GetMaxSP() - GetSP());

	ComputeSkillPoints();

	RefreshAffect();
	// @fixme118 part2 (before petsystem stuff)
	if (IsPC())
	{
		if (this->GetHP() != iCurHP)
			this->PointChange(POINT_HP, iCurHP-this->GetHP());
		if (this->GetSP() != iCurSP)
			this->PointChange(POINT_SP, iCurSP-this->GetSP());
	}

#ifndef ENABLE_NEW_PET_SYSTEM
	CPetSystem* pPetSystem = GetPetSystem();
	if (NULL != pPetSystem)
		pPetSystem->RefreshBuff();
#endif
// #if defined(ENABLE_MOUNT_SYSTEM) and defined(MOUNT_SYSTEM_BONUS)
	// CMountSystem* pMountSystem = GetMountSystem();
	// if (NULL != pMountSystem)
	// {
		// pMountSystem->RefreshBuff();
	// }
// #endif

	UpdatePacket();
}

// m_dwPlayStartTimeÀÇ ´ÜÀ§´Â milisecond´Ù. µ¥ÀÌÅÍº£ÀÌ½º¿¡´Â ºÐ´ÜÀ§·Î ±â·ÏÇÏ±â
// ¶§¹®¿¡ ÇÃ·¹ÀÌ½Ã°£À» °è»êÇÒ ¶§ / 60000 À¸·Î ³ª´²¼­ ÇÏ´Âµ¥, ±× ³ª¸ÓÁö °ªÀÌ ³²¾Ò
// À» ¶§ ¿©±â¿¡ dwTimeRemainÀ¸·Î ³Ö¾î¼­ Á¦´ë·Î °è»êµÇµµ·Ï ÇØÁÖ¾î¾ß ÇÑ´Ù.
void CHARACTER::ResetPlayTime(DWORD dwTimeRemain)
{
	m_dwPlayStartTime = get_dword_time() - dwTimeRemain;
}

const int aiRecoveryPercents[10] = { 1, 5, 5, 5, 5, 5, 5, 5, 5, 5 };

EVENTFUNC(recovery_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info);
	if ( info == NULL )
	{
		sys_err( "recovery_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (ch == NULL) { // <Factor>
		return 0;
	}

	if (!ch->IsPC()
#ifdef __FAKE_PLAYER__
		&& !ch->IsFakePlayer()
#endif		
	)
	{
		//
		// ¸ó½ºÅÍ È¸º¹
		//
		if (ch->IsAffectFlag(AFF_POISON))
			return PASSES_PER_SEC(MAX(1, ch->GetMobTable().bRegenCycle));
#ifdef ENABLE_WOLFMAN_CHARACTER
		if (ch->IsAffectFlag(AFF_BLEEDING))
			return PASSES_PER_SEC(MAX(1, ch->GetMobTable().bRegenCycle));
#endif
		if (2493 == ch->GetMobTable().dwVnum)
		{
			int regenPct = BlueDragon_GetRangeFactor("hp_regen", ch->GetHPPct());
			regenPct += ch->GetMobTable().bRegenPercent;

			for (int i=1 ; i <= 4 ; ++i)
			{
				if (REGEN_PECT_BONUS == BlueDragon_GetIndexFactor("DragonStone", i, "effect_type"))
				{
					DWORD dwDragonStoneID = BlueDragon_GetIndexFactor("DragonStone", i, "vnum");
					size_t val = BlueDragon_GetIndexFactor("DragonStone", i, "val");
					size_t cnt = SECTREE_MANAGER::instance().GetMonsterCountInMap( ch->GetMapIndex(), dwDragonStoneID);

					regenPct += (val*cnt);

					break;
				}
			}

			ch->PointChange(POINT_HP, MAX(1, (ch->GetMaxHP() * regenPct) / 100));
		}
		else if (!ch->IsDoor())
		{
			ch->MonsterLog("HP_REGEN +%d", MAX(1, (ch->GetMaxHP() * ch->GetMobTable().bRegenPercent) / 100));
			ch->PointChange(POINT_HP, MAX(1, (ch->GetMaxHP() * ch->GetMobTable().bRegenPercent) / 100));
		}

		if (ch->GetHP() >= ch->GetMaxHP())
		{
			ch->m_pkRecoveryEvent = NULL;
			return 0;
		}

		if (2493 == ch->GetMobTable().dwVnum)
		{
			for (int i=1 ; i <= 4 ; ++i)
			{
				if (REGEN_TIME_BONUS == BlueDragon_GetIndexFactor("DragonStone", i, "effect_type"))
				{
					DWORD dwDragonStoneID = BlueDragon_GetIndexFactor("DragonStone", i, "vnum");
					size_t val = BlueDragon_GetIndexFactor("DragonStone", i, "val");
					size_t cnt = SECTREE_MANAGER::instance().GetMonsterCountInMap( ch->GetMapIndex(), dwDragonStoneID);

					return PASSES_PER_SEC(MAX(1, (ch->GetMobTable().bRegenCycle - (val*cnt))));
				}
			}
		}

		return PASSES_PER_SEC(MAX(1, ch->GetMobTable().bRegenCycle));
	}
	else
	{
		//
		// PC È¸º¹
		//
		ch->CheckTarget();
		//ch->UpdateSectree(); // ¿©±â¼­ ÀÌ°É ¿ÖÇÏÁö?
		ch->UpdateKillerMode();

		if (ch->IsAffectFlag(AFF_POISON) == true)
		{
			// Áßµ¶ÀÎ °æ¿ì ÀÚµ¿È¸º¹ ±ÝÁö
			// ÆÄ¹ý¼úÀÎ °æ¿ì ÀÚµ¿È¸º¹ ±ÝÁö
			return 3;
		}
#ifdef ENABLE_WOLFMAN_CHARACTER
		if (ch->IsAffectFlag(AFF_BLEEDING))
			return 3;
#endif
		int iSec = (get_dword_time() - ch->GetLastMoveTime()) / 3000;

		// SP È¸º¹ ·çÆ¾.
		// ¿Ö ÀÌ°É·Î ÇØ¼­ ÇÔ¼ö·Î »©³ù´Â°¡ ?!
		ch->DistributeSP(ch);

		if (ch->GetMaxHP() <= ch->GetHP())
			return PASSES_PER_SEC(3);

		int iPercent = 0;
		int iAmount = 0;

		{
			iPercent = aiRecoveryPercents[MIN(9, iSec)];
			iAmount = 15 + (ch->GetMaxHP() * iPercent) / 100;
		}

		iAmount += (iAmount * ch->GetPoint(POINT_HP_REGEN)) / 100;

		sys_log(1, "RECOVERY_EVENT: %s %d HP_REGEN %d HP +%d", ch->GetName(), iPercent, ch->GetPoint(POINT_HP_REGEN), iAmount);

		ch->PointChange(POINT_HP, iAmount, false);
		return PASSES_PER_SEC(3);
	}
}

void CHARACTER::StartRecoveryEvent()
{
	if (m_pkRecoveryEvent)
		return;

	if (IsDead() || IsStun())
		return;

	if (IsNPC() && GetHP() >= GetMaxHP()) // ¸ó½ºÅÍ´Â Ã¼·ÂÀÌ ´Ù Â÷ÀÖÀ¸¸é ½ÃÀÛ ¾ÈÇÑ´Ù.
		return;

#if defined(__BL_BATTLE_ROYALE__)
	if (IsBattleRoyale())
		return;
#endif

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;
	
	int iSec = 0;
	if (IsPC()
#ifdef __FAKE_PLAYER__
		|| IsFakePlayer()
#endif		
	)
		iSec = 3; 
	else
		iSec = (MAX(1, GetMobTable().bRegenCycle));

	m_pkRecoveryEvent = event_create(recovery_event, info, PASSES_PER_SEC(iSec));
}

void CHARACTER::Standup()
{
	struct packet_position pack_position;

	if (!IsPosition(POS_SITTING))
		return;

	SetPosition(POS_STANDING);

	sys_log(1, "STANDUP: %s", GetName());

	pack_position.header	= HEADER_GC_CHARACTER_POSITION;
	pack_position.vid		= GetVID();
	pack_position.position	= POSITION_GENERAL;

	PacketAround(&pack_position, sizeof(pack_position));
}

void CHARACTER::Sitdown(int is_ground)
{
	struct packet_position pack_position;

	if (IsPosition(POS_SITTING))
		return;

	SetPosition(POS_SITTING);
	sys_log(1, "SITDOWN: %s", GetName());

	pack_position.header	= HEADER_GC_CHARACTER_POSITION;
	pack_position.vid		= GetVID();
	pack_position.position	= POSITION_SITTING_GROUND;
	PacketAround(&pack_position, sizeof(pack_position));
}

void CHARACTER::SetRotation(float fRot)
{
	m_pointsInstant.fRot = fRot;
}

// x, y ¹æÇâÀ¸·Î º¸°í ¼±´Ù.
void CHARACTER::SetRotationToXY(long x, long y)
{
	SetRotation(GetDegreeFromPositionXY(GetX(), GetY(), x, y));
}

bool CHARACTER::CannotMoveByAffect() const
{
	return (IsAffectFlag(AFF_STUN));
}

bool CHARACTER::CanMove() const
{
	if (CannotMoveByAffect())
		return false;

#ifdef ENABLE_SUNG_MAHI_TOWER
	if (IsNoattack())
		return false;
#endif
	return true;
}

// ¹«Á¶°Ç x, y À§Ä¡·Î ÀÌµ¿ ½ÃÅ²´Ù.
bool CHARACTER::Sync(long x, long y)
{
	if (!GetSectree())
		return false;

	LPSECTREE new_tree = SECTREE_MANAGER::instance().Get(GetMapIndex(), x, y);

	if (!new_tree)
	{
		if (GetDesc())
		{
			sys_err("cannot find tree at %d %d (name: %s)", x, y, GetName());
			GetDesc()->SetPhase(PHASE_CLOSE);
		}
		else
		{
			sys_err("no tree: %s %d %d %d", GetName(), x, y, GetMapIndex());
			Dead();
		}

		return false;
	}

#ifdef ENABLE_CHECK_MOVESPEED_HACK
	if (IsPC())
	{
		if (GetCShield()->CheckMoveSpeedhack(x, y, get_dword_time(), IsRiding(), GetMoveSpeed()))
		{
			LogManager::instance().HackLog("CShield-ServerSide-ErrorCode: Movespeed Hack", this);
			Show(GetMapIndex(), GetX(), GetY(), 0);
			Stop();
		}
	}
#endif

	SetRotationToXY(x, y);
	SetXYZ(x, y, 0);

	if (GetDungeon())
	{
		// ´øÁ¯¿ë ÀÌº¥Æ® ¼Ó¼º º¯È­
		int iLastEventAttr = m_iEventAttr;
		m_iEventAttr = new_tree->GetEventAttribute(x, y);

		if (m_iEventAttr != iLastEventAttr)
		{
			if (GetParty())
			{
				quest::CQuestManager::instance().AttrOut(GetParty()->GetLeaderPID(), this, iLastEventAttr);
				quest::CQuestManager::instance().AttrIn(GetParty()->GetLeaderPID(), this, m_iEventAttr);
			}
			else
			{
				quest::CQuestManager::instance().AttrOut(GetPlayerID(), this, iLastEventAttr);
				quest::CQuestManager::instance().AttrIn(GetPlayerID(), this, m_iEventAttr);
			}
		}
	}

	if (GetSectree() != new_tree)
	{
		if (!IsNPC())
		{
			SECTREEID id = new_tree->GetID();
			SECTREEID old_id = GetSectree()->GetID();

			const float fDist = DISTANCE_SQRT(id.coord.x - old_id.coord.x, id.coord.y - old_id.coord.y);
			sys_log(0, "SECTREE DIFFER: %s %dx%d was %dx%d dist %.1fm",
					GetName(),
					id.coord.x,
					id.coord.y,
					old_id.coord.x,
					old_id.coord.y,
					fDist);
		}

		new_tree->InsertEntity(this);
	}

	return true;
}

void CHARACTER::Stop()
{
	if (!IsState(m_stateIdle))
		MonsterLog("[IDLE] Á¤Áö");

	GotoState(m_stateIdle);

	m_posDest.x = m_posStart.x = GetX();
	m_posDest.y = m_posStart.y = GetY();
}

bool CHARACTER::Goto(long x, long y)
{
	// TODO °Å¸®Ã¼Å© ÇÊ¿ä
	// °°Àº À§Ä¡¸é ÀÌµ¿ÇÒ ÇÊ¿ä ¾øÀ½ (ÀÚµ¿ ¼º°ø)
	if (GetX() == x && GetY() == y)
		return false;

	if (m_posDest.x == x && m_posDest.y == y)
	{
		if (!IsState(m_stateMove))
		{
			m_dwStateDuration = 4;
			GotoState(m_stateMove);
		}
		return false;
	}

	m_posDest.x = x;
	m_posDest.y = y;

	CalculateMoveDuration();

	m_dwStateDuration = 4;


	if (!IsState(m_stateMove))
	{
		MonsterLog("[MOVE] %s", GetVictim() ? "´ë»óÃßÀû" : "±×³ÉÀÌµ¿");

		if (GetVictim())
		{
			//MonsterChat(MONSTER_CHAT_CHASE);
			MonsterChat(MONSTER_CHAT_ATTACK);
		}
	}

	GotoState(m_stateMove);

	return true;
}


DWORD CHARACTER::GetMotionMode() const
{
	DWORD dwMode = MOTION_MODE_GENERAL;

	if (IsPolymorphed())
		return dwMode;

	LPITEM pkItem;

	if ((pkItem = GetWear(WEAR_WEAPON)))
	{
		switch (pkItem->GetProto()->bSubType)
		{
			case WEAPON_SWORD:
				dwMode = MOTION_MODE_ONEHAND_SWORD;
				break;

			case WEAPON_TWO_HANDED:
				dwMode = MOTION_MODE_TWOHAND_SWORD;
				break;

			case WEAPON_DAGGER:
				dwMode = MOTION_MODE_DUALHAND_SWORD;
				break;

			case WEAPON_BOW:
				dwMode = MOTION_MODE_BOW;
				break;

			case WEAPON_BELL:
				dwMode = MOTION_MODE_BELL;
				break;

			case WEAPON_FAN:
				dwMode = MOTION_MODE_FAN;
				break;
#ifdef ENABLE_WOLFMAN_CHARACTER
			case WEAPON_CLAW:
				dwMode = MOTION_MODE_CLAW;
				break;
#endif
		}
	}
	return dwMode;
}

float CHARACTER::GetMoveMotionSpeed() const
{
	DWORD dwMode = GetMotionMode();

	const CMotion * pkMotion = NULL;

	if (!GetMountVnum())
		pkMotion = CMotionManager::instance().GetMotion(GetRaceNum(), MAKE_MOTION_KEY(dwMode, (IsWalking() && IsPC()) ? MOTION_WALK : MOTION_RUN));
	else
	{
		pkMotion = CMotionManager::instance().GetMotion(GetMountVnum(), MAKE_MOTION_KEY(MOTION_MODE_GENERAL, (IsWalking() && IsPC()) ? MOTION_WALK : MOTION_RUN));

		if (!pkMotion)
			pkMotion = CMotionManager::instance().GetMotion(GetRaceNum(), MAKE_MOTION_KEY(MOTION_MODE_HORSE, (IsWalking() && IsPC()) ? MOTION_WALK : MOTION_RUN));
	}

	if (pkMotion)
		return -pkMotion->GetAccumVector().y / pkMotion->GetDuration();
	else
	{
		sys_err("cannot find motion (name %s race %d mode %d)", GetName(), GetRaceNum(), dwMode);
		return 300.0f;
	}
}

float CHARACTER::GetMoveSpeed() const
{
	return GetMoveMotionSpeed() * 10000 / CalculateDuration(GetLimitPoint(POINT_MOV_SPEED), 10000);
}

void CHARACTER::CalculateMoveDuration()
{
	m_posStart.x = GetX();
	m_posStart.y = GetY();

	float fDist = DISTANCE_SQRT(m_posStart.x - m_posDest.x, m_posStart.y - m_posDest.y);

	float motionSpeed = GetMoveMotionSpeed();

	m_dwMoveDuration = CalculateDuration(GetLimitPoint(POINT_MOV_SPEED),
			(int) ((fDist / motionSpeed) * 1000.0f));

	if (IsNPC())
		sys_log(1, "%s: GOTO: distance %f, spd %u, duration %u, motion speed %f pos %d %d -> %d %d",
				GetName(), fDist, GetLimitPoint(POINT_MOV_SPEED), m_dwMoveDuration, motionSpeed,
				m_posStart.x, m_posStart.y, m_posDest.x, m_posDest.y);

	m_dwMoveStartTime = get_dword_time();
}

// x y À§Ä¡·Î ÀÌµ¿ ÇÑ´Ù. (ÀÌµ¿ÇÒ ¼ö ÀÖ´Â °¡ ¾ø´Â °¡¸¦ È®ÀÎ ÇÏ°í Sync ¸Þ¼Òµå·Î ½ÇÁ¦ ÀÌµ¿ ÇÑ´Ù)
// ¼­¹ö´Â charÀÇ x, y °ªÀ» ¹Ù·Î ¹Ù²ÙÁö¸¸,
// Å¬¶ó¿¡¼­´Â ÀÌÀü À§Ä¡¿¡¼­ ¹Ù²Û x, y±îÁö interpolationÇÑ´Ù.
// °È°Å³ª ¶Ù´Â °ÍÀº charÀÇ m_bNowWalking¿¡ ´Þ·ÁÀÖ´Ù.
// Warp¸¦ ÀÇµµÇÑ °ÍÀÌ¶ó¸é Show¸¦ »ç¿ëÇÒ °Í.
bool CHARACTER::Move(long x, long y)
{
	// °°Àº À§Ä¡¸é ÀÌµ¿ÇÒ ÇÊ¿ä ¾øÀ½ (ÀÚµ¿ ¼º°ø)
	if (GetX() == x && GetY() == y)
		return true;

	if (test_server)
		if (m_bDetailLog)
			sys_log(0, "%s position %u %u", GetName(), x, y);

	OnMove();
	return Sync(x, y);
}
#ifdef ENABLE_SUPPORT_SYSTEM
void CHARACTER::SendSupportSkillPacket(DWORD skill_vnum, DWORD skill_level)
{
	TPacketGCSupportUseSkill pack;
	pack.bHeader = HEADER_GC_SUPPORT_SKILL;
	pack.dwVnum = skill_vnum;
	pack.dwVid = GetVID();
	pack.dwLevel = skill_level;
	
	PacketView(&pack, sizeof(TPacketGCSupportUseSkill), this);
}
#endif
void CHARACTER::SendMovePacket(BYTE bFunc, BYTE bArg, DWORD x, DWORD y, DWORD dwDuration, DWORD dwTime, int iRot)
{
	TPacketGCMove pack;

	if (bFunc == FUNC_WAIT)
	{
		x = m_posDest.x;
		y = m_posDest.y;
		dwDuration = m_dwMoveDuration;
	}

	EncodeMovePacket(pack, GetVID(), bFunc, bArg, x, y, dwDuration, dwTime, iRot == -1 ? (int) GetRotation() / 5 : iRot);
	PacketView(&pack, sizeof(TPacketGCMove), this);
}

int CHARACTER::GetRealPoint(BYTE type) const
{
	return m_points.points[type];
}

void CHARACTER::SetRealPoint(BYTE type, int val)
{
	m_points.points[type] = val;
#ifdef ENABLE_RANKING
	if (type == POINT_PLAYTIME)
		SetRankPoints(15, val);
#endif
}

int CHARACTER::GetPolymorphPoint(BYTE type) const
{
	if (IsPolymorphed() && !IsPolyMaintainStat())
	{
		DWORD dwMobVnum = GetPolymorphVnum();
		const CMob * pMob = CMobManager::instance().Get(dwMobVnum);
		int iPower = GetPolymorphPower();

		if (pMob)
		{
			switch (type)
			{
				case POINT_ST:
					if ((GetJob() == JOB_SHAMAN) || ((GetJob() == JOB_SURA) && (GetSkillGroup() == 2)))
						return pMob->m_table.bStr * iPower / 100 + GetPoint(POINT_IQ);
					return pMob->m_table.bStr * iPower / 100 + GetPoint(POINT_ST);

				case POINT_HT:
					return pMob->m_table.bCon * iPower / 100 + GetPoint(POINT_HT);

				case POINT_IQ:
					return pMob->m_table.bInt * iPower / 100 + GetPoint(POINT_IQ);

				case POINT_DX:
					return pMob->m_table.bDex * iPower / 100 + GetPoint(POINT_DX);
			}
		}
	}

	return GetPoint(type);
}

long long CHARACTER::GetPoint(BYTE type) const
{
	if (type >= POINT_MAX_NUM)
	{
		sys_err("Point type overflow (type %u)", type);
		return 0;
	}

	long long val = m_pointsInstant.points[type];
	long long max_val = LLONG_MAX;

	switch (type)
	{
		case POINT_STEAL_HP:
		case POINT_STEAL_SP:
		{
			max_val = 50;
			break;
		}
		case POINT_GOLD:
		{
			max_val = GOLD_MAX;
			break;
		}
#ifdef USE_CHEQUE_CURRENCY
		case POINT_CHEQUE:
		{
			max_val = CHEQUE_MAX;
			break;
		}
#endif
	}

	if (val > max_val)
	{
		sys_err("POINT_ERROR: %s type %d val %lld (max: %lld)", GetName(), val, max_val);
	}

	return (val > max_val) ? max_val : val;
}

int CHARACTER::GetLimitPoint(BYTE type) const
{
	if (type >= POINT_MAX_NUM)
	{
		sys_err("Point type overflow (type %u)", type);
		return 0;
	}

	int val = m_pointsInstant.points[type];
	
#ifdef ENABLE_MOB_MOVE_SPEED
	if (IsMonster() && GetMobRank() < MOB_RANK_KING && POINT_MOV_SPEED == type)
		val = GetProtectTime("is_aggregate_monster") == 1 ? 300 : 250;
#endif

	int max_val = INT_MAX;
	int limit = INT_MAX;
	int min_limit = -INT_MAX;

	switch (type)
	{
		case POINT_ATT_SPEED:
			min_limit = 0;

			if (IsPC())
				limit = 160;
			else
				limit = 250;
			break;

		case POINT_MOV_SPEED:
		{
			min_limit = 0;

#ifdef ENABLE_YOHARA_SYSTEM
			if (IsSungmaMap())
			{
				if (GetPoint(POINT_SUNGMA_MOVE) < GetSungmaMapAttribute(POINT_SUNGMA_MOVE))
					val /= 2;
			}
#endif

			if (IsPC())
				limit = 200;
			else
#ifdef ENABLE_MOB_MOVE_SPEED
				limit = (IsMonster() && GetMobRank() < MOB_RANK_KING) ? 350 : 250;
#else
				limit = 250;
#endif
		}
		break;

		case POINT_STEAL_HP:
		case POINT_STEAL_SP:
			limit = 50;
			max_val = 50;
			break;

		case POINT_MALL_ATTBONUS:
		case POINT_MALL_DEFBONUS:
			limit = 20;
			max_val = 50;
			break;

		//SERGIO-CRITICAL
		//case POINT_CRITICAL_PCT:
		//	if(IsPC())
		//		limit = 100;
		//		max_val = 100;
		//	break;
	}

	if (val > max_val)
		//sys_err("POINT_ERROR: %s type %d val %d (max: %d)", GetName(), val, max_val);
		sys_err("POINT_ERROR: %s type %d val %d (max: %d)", GetName(), type, val, max_val);

	if (val > limit)
		val = limit;

	if (val < min_limit)
		val = min_limit;

#ifdef __BUFFI_SUPPORT__
	if (m_bIsSupport && type == POINT_MOV_SPEED)
		val = 300;//you can set speed value like your want!
#endif

	return (val);
}

void CHARACTER::SetPoint(BYTE type, long long val)
{
	if (type >= POINT_MAX_NUM)
	{
		sys_err("Point type overflow (type %u)", type);
		return;
	}

	m_pointsInstant.points[type] = val;

	// ¾ÆÁ÷ ÀÌµ¿ÀÌ ´Ù ¾È³¡³µ´Ù¸é ÀÌµ¿ ½Ã°£ °è»êÀ» ´Ù½Ã ÇØ¾ß ÇÑ´Ù.
	if (type == POINT_MOV_SPEED && get_dword_time() < m_dwMoveStartTime + m_dwMoveDuration)
	{
		CalculateMoveDuration();
	}
}

long long CHARACTER::GetAllowedGold() const
{
	if (GetLevel() <= 10)
		return 100000;
	else if (GetLevel() <= 20)
		return 500000;
	else
		return 50000000;
}

void CHARACTER::CheckMaximumPoints()
{
	if (GetMaxHP() < GetHP())
		PointChange(POINT_HP, GetMaxHP() - GetHP());

	if (GetMaxSP() < GetSP())
		PointChange(POINT_SP, GetMaxSP() - GetSP());
}

void CHARACTER::PointChange(BYTE type, long long amount, bool bAmount, bool bBroadcast)
{
	long long val = 0;

	//sys_log(0, "PointChange %d %d | %d -> %d cHP %d mHP %d", type, amount, GetPoint(type), GetPoint(type)+amount, GetHP(), GetMaxHP());

	switch (type)
	{
		case POINT_NONE:
		//case POINT_BATTLE_PASS_ID:
			return;

#ifdef ENABLE_YOHARA_SYSTEM
		case POINT_CONQUEROR_LEVEL:
			if ((GetConquerorLevel() + amount) > gConquerorMaxLevel)
				return;

			SetConquerorLevel(GetConquerorLevel() + amount);
			val = GetConquerorLevel();
			
			RankPlayer::instance().SendInfoPlayer(this, RANK_BY_LEVEL, GetConquerorLevel(), false);

			sys_log(0, "CLEVELUP: %s %d CNEXT EXP %d", GetName(), GetConquerorLevel(), GetConquerorNextExp());
			PointChange(POINT_CONQUEROR_NEXT_EXP, GetConquerorNextExp(), false);
			break;

		case POINT_CONQUEROR_NEXT_EXP:
			val = GetConquerorNextExp();
			bAmount = false;
			break;

		case POINT_CONQUEROR_EXP:
			{
				auto conqueror_exp = GetConquerorExp();
				const auto conqueror_next_exp = GetConquerorNextExp();

				sys_err("Conqueror_exp: %d", amount);
				//Reduction will set us below zero, set zero directly
				if ((amount < 0) && (conqueror_exp < (-amount)))
				{
					sys_log(1, "%s AMOUNT < 0 %d, CUR EXP: %d", GetName(), -amount, conqueror_exp);
					amount = -conqueror_exp;

					SetConquerorExp(conqueror_exp + amount);
					val = GetConquerorExp();
				}
				else
				{
					if (gConquerorMaxLevel <= GetConquerorLevel())
						return;

					if (test_server)
						ChatPacket(CHAT_TYPE_INFO, "[LS;1290;%d]", amount);

					uint32_t iExpBalance = 0;

					if (conqueror_exp + amount >= conqueror_next_exp)
					{
						iExpBalance = (conqueror_exp + amount) - conqueror_next_exp;
						amount = conqueror_next_exp - conqueror_exp;

						SetConquerorExp(0);
						conqueror_exp = conqueror_next_exp;
					}
					else
					{
						SetConquerorExp(conqueror_exp + amount);
						conqueror_exp = GetConquerorExp();
					}

					const auto q = (conqueror_next_exp / 4.0f);
					int ciLevStep = GetRealPoint(POINT_CONQUEROR_LEVEL_STEP);

					if (ciLevStep >= 4)
					{
						sys_err("%s LEVEL_STEP bigger than 4! (%d)", GetName(), ciLevStep);
						ciLevStep = 4;
					}

					if (conqueror_exp >= conqueror_next_exp && ciLevStep < 4)
					{
						for (int i = 0; i < 4 - ciLevStep; ++i)
							PointChange(POINT_CONQUEROR_LEVEL_STEP, 1, false, true);
					}
					else if (conqueror_exp >= q * 3 && ciLevStep < 3)
					{
						for (int i = 0; i < 3 - ciLevStep; ++i)
							PointChange(POINT_CONQUEROR_LEVEL_STEP, 1, false, true);
					}
					else if (conqueror_exp >= q * 2 && ciLevStep < 2)
					{
						for (int i = 0; i < 2 - ciLevStep; ++i)
							PointChange(POINT_CONQUEROR_LEVEL_STEP, 1, false, true);
					}
					else if (conqueror_exp >= q && ciLevStep < 1)
						PointChange(POINT_CONQUEROR_LEVEL_STEP, 1);

					if (iExpBalance)
					{
						PointChange(POINT_CONQUEROR_EXP, iExpBalance);
					}

					val = GetConquerorExp();
				}
			}
			break;

		case POINT_CONQUEROR_LEVEL_STEP:
			if (amount > 0)
			{
				val = GetPoint(POINT_CONQUEROR_LEVEL_STEP) + amount;

				switch (val)
				{
					case 1:
					case 2:
					case 3:
						if ((GetConquerorLevel() <= g_iConquerorPointGetLevelLimit) &&
							(GetConquerorLevel() <= gConquerorMaxLevel) ) // @fixme104
							PointChange(POINT_CONQUEROR_POINT, 1);
						break;

					case 4:
						{
							const auto iHP = number(JobInitialPoints[GetJob()].hp_per_lv_begin, JobInitialPoints[GetJob()].hp_per_lv_end);
							const auto iSP = number(JobInitialPoints[GetJob()].sp_per_lv_begin, JobInitialPoints[GetJob()].sp_per_lv_end);

							m_points.iRandomHP += iHP;
							m_points.iRandomSP += iSP;

							PointChange(POINT_MAX_HP, iHP);
							PointChange(POINT_MAX_SP, iSP);
							PointChange(POINT_CONQUEROR_LEVEL, 1, false, true);
							val = 0;
						}
						break;

					default:
						break;
				}

				SetPoint(POINT_CONQUEROR_LEVEL_STEP, val);
				SetRealPoint(POINT_CONQUEROR_LEVEL_STEP, val);

				Save();
			}
			else
				val = GetPoint(POINT_CONQUEROR_LEVEL_STEP);

			break;

		case POINT_SUNGMA_STR:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			PointChange(POINT_ST, 0);
			break;

		case POINT_SUNGMA_HP:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			PointChange(POINT_MAX_HP, 0);
			break;

		case POINT_SUNGMA_MOVE:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			PointChange(POINT_MOV_SPEED, 0);
			break;

		case POINT_SUNGMA_IMMUNE:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;
#endif

		case POINT_LEVEL:
			if ((GetLevel() + amount) > gPlayerMaxLevel)
				return;

			SetLevel(GetLevel() + amount);
			val = GetLevel();

			sys_log(0, "LEVELUP: %s %d NEXT EXP %d", GetName(), GetLevel(), GetNextExp());
#ifdef ENABLE_WOLFMAN_CHARACTER
			if (GetJob() == JOB_WOLFMAN)
			{
				if ((5 <= val) && (GetSkillGroup()!=1))
				{
					ClearSkill();
					// set skill group
					SetSkillGroup(1);
					// set skill points
					SetRealPoint(POINT_SKILL, GetLevel()-1);
					SetPoint(POINT_SKILL, GetRealPoint(POINT_SKILL));
					PointChange(POINT_SKILL, 0);
					// update points (not required)
					// ComputePoints();
					// PointsPacket();
				}
			}
#endif

			if (val == 120)
			{
				CHARACTER_MANAGER::Instance().SetRewardData(REWARD_120, GetName(), true);
			}

			if (val == 115 && GetJob() == JOB_WARRIOR)
			{
				CHARACTER_MANAGER::Instance().SetRewardData(REWARD_115WAR, GetName(), true);
			}

			if (val == 115 && GetJob() == JOB_ASSASSIN)
			{
				CHARACTER_MANAGER::Instance().SetRewardData(REWARD_115ASSASSIN, GetName(), true);
			}

			if (val == 115 && GetJob() == JOB_SURA)
			{
				CHARACTER_MANAGER::Instance().SetRewardData(REWARD_115SURA, GetName(), true);
			}

			if (val == 115 && GetJob() == JOB_SHAMAN)
			{
				CHARACTER_MANAGER::Instance().SetRewardData(REWARD_115SHAMAN, GetName(), true);
			}

			PointChange(POINT_NEXT_EXP,	GetNextExp(), false);
			// RankPlayer::instance().SendInfoPlayer(this, RANK_BY_LEVEL, GetLevel(), false);

			if (amount)
			{
				quest::CQuestManager::instance().LevelUp(GetPlayerID());
#ifdef ENABLE_BIYOLOG
				CheckBio();
#endif
#ifdef ENABLE_BATTLE_PASS
				CHARACTER_MANAGER::Instance().DoMission(this, MISSION_LEVEL_UP, amount, 0);
#endif
#ifdef ENABLE_AUTO_SELECT_SKILL
				if(GetSkillGroup() == 0 && GetLevel() >= 5)
					ChatPacket(CHAT_TYPE_COMMAND, "OpenAutoSkill");
#endif
				LogManager::instance().LevelLog(this, val, GetRealPoint(POINT_PLAYTIME) + (get_dword_time() - m_dwPlayStartTime) / 60000);

				if (GetGuild())
				{
					GetGuild()->LevelChange(GetPlayerID(), GetLevel());
				}

				if (GetParty())
				{
					GetParty()->RequestSetMemberLevel(GetPlayerID(), GetLevel());
				}
#ifdef ENABLE_BATTLEPASS
                UpdateMissionProgress(MISSION_REACH_LEVEL, GetLevel());
#endif
			}
			break;

		case POINT_NEXT_EXP:
			val = GetNextExp();
			bAmount = false;	// ¹«Á¶°Ç bAmount´Â false ¿©¾ß ÇÑ´Ù.
			break;

		case POINT_EXP:
			{
#if defined(__BL_BATTLE_ROYALE__)
				if (IsBattleRoyale())
					amount = 0;
#endif

				DWORD exp = GetExp();
				DWORD next_exp = GetNextExp();

				// Ã»¼Ò³âº¸È£
				if (g_bChinaIntoxicationCheck)
				{
					if (IsOverTime(OT_NONE))
					{
						dev_log(LOG_DEB0, "<EXP_LOG> %s = NONE", GetName());
					}
					else if (IsOverTime(OT_3HOUR))
					{
						amount = (amount / 2);
						dev_log(LOG_DEB0, "<EXP_LOG> %s = 3HOUR", GetName());
					}
					else if (IsOverTime(OT_5HOUR))
					{
						amount = 0;
						dev_log(LOG_DEB0, "<EXP_LOG> %s = 5HOUR", GetName());
					}
				}

				// exp°¡ 0 ÀÌÇÏ·Î °¡Áö ¾Êµµ·Ï ÇÑ´Ù
				if ((amount < 0) && (exp < (DWORD)(-amount)))
				{
					sys_log(1, "%s AMOUNT < 0 %d, CUR EXP: %d", GetName(), -amount, exp);
					amount = -exp;

					SetExp(exp + amount);
					val = GetExp();
				}
				else
				{
					/*if (gPlayerMaxLevel <= GetLevel())
						return;*/

					if (gPlayerMaxLevel == GetLevel() && (next_exp / 4) <= exp)	//@fixme405
						return;

#ifdef __ANTI_EXP__
					if (GetQuestFlag("anti_exp.block_exp") == 1)
					{
						return;
					}
#endif

					DWORD iExpBalance = 0;

					if (gPlayerMaxLevel == GetLevel())	//@fixme405
					{
						if ((amount + exp) > (next_exp / 4))
							amount = ((next_exp / 4) - exp);
					}

					// Level up!
					if (exp + amount >= next_exp)
					{
						iExpBalance = (exp + amount) - next_exp;
						amount = next_exp - exp;

						SetExp(0);
						exp = next_exp;
					}
					else
					{
						SetExp(exp + amount);
						exp = GetExp();
					}

					DWORD q = DWORD(next_exp / 4.0f);
					int iLevStep = GetRealPoint(POINT_LEVEL_STEP);

					// iLevStepÀÌ 4 ÀÌ»óÀÌ¸é ·¹º§ÀÌ ¿Ã¶ú¾î¾ß ÇÏ¹Ç·Î ¿©±â¿¡ ¿Ã ¼ö ¾ø´Â °ªÀÌ´Ù.
					if (iLevStep >= 4)
					{
						sys_err("%s LEVEL_STEP bigger than 4! (%d)", GetName(), iLevStep);
						iLevStep = 4;
					}

					if (exp >= next_exp && iLevStep < 4)
					{
						for (int i = 0; i < 4 - iLevStep; ++i)
							PointChange(POINT_LEVEL_STEP, 1, false, true);
					}
					else if (exp >= q * 3 && iLevStep < 3)
					{
						for (int i = 0; i < 3 - iLevStep; ++i)
							PointChange(POINT_LEVEL_STEP, 1, false, true);
					}
					else if (exp >= q * 2 && iLevStep < 2)
					{
						for (int i = 0; i < 2 - iLevStep; ++i)
							PointChange(POINT_LEVEL_STEP, 1, false, true);
					}
					else if (exp >= q && iLevStep < 1)
						PointChange(POINT_LEVEL_STEP, 1);

					if (iExpBalance)
					{
						PointChange(POINT_EXP, iExpBalance);
					}

					val = GetExp();
				}
			}
			break;

		case POINT_LEVEL_STEP:
			if (amount > 0)
			{
				val = GetPoint(POINT_LEVEL_STEP) + amount;

				switch (val)
				{
					case 1:
					case 2:
					case 3:
						if ((GetLevel() <= g_iStatusPointGetLevelLimit) &&
							(GetLevel() <= gPlayerMaxLevel) ) // @fixme104
							PointChange(POINT_STAT, 1);
						break;

					case 4:
						{
							int iHP = number(JobInitialPoints[GetJob()].hp_per_lv_begin, JobInitialPoints[GetJob()].hp_per_lv_end);
							int iSP = number(JobInitialPoints[GetJob()].sp_per_lv_begin, JobInitialPoints[GetJob()].sp_per_lv_end);

							m_points.iRandomHP += iHP;
							m_points.iRandomSP += iSP;

							if (GetSkillGroup())
							{
								if (GetLevel() >= 5)
									PointChange(POINT_SKILL, 1);

								if (GetLevel() >= 9)
									PointChange(POINT_SUB_SKILL, 1);
							}

							PointChange(POINT_MAX_HP, iHP);
							PointChange(POINT_MAX_SP, iSP);
							PointChange(POINT_LEVEL, 1, false, true);

							val = 0;
						}
						break;
				}

				// if (GetLevel() <= 10)
					// AutoGiveItem(27001, 2);
				// else if (GetLevel() <= 30)
					// AutoGiveItem(27002, 2);
				// else
				// {
					// AutoGiveItem(27002, 2);
				// }

				PointChange(POINT_HP, GetMaxHP() - GetHP());
				PointChange(POINT_SP, GetMaxSP() - GetSP());
				PointChange(POINT_STAMINA, GetMaxStamina() - GetStamina());

				SetPoint(POINT_LEVEL_STEP, val);
				SetRealPoint(POINT_LEVEL_STEP, val);

				Save();
			}
			else
				val = GetPoint(POINT_LEVEL_STEP);

			break;

		case POINT_HP:
			{
				if (IsDead() || IsStun())
					return;

				int prev_hp = GetHP();

				amount = MIN(GetMaxHP() - GetHP(), amount);
				SetHP(GetHP() + amount);
				val = GetHP();

				BroadcastTargetPacket();

				if (GetParty() && IsPC() && val != prev_hp)
					GetParty()->SendPartyInfoOneToAll(this);
			}
			break;

		case POINT_SP:
			{
				if (IsDead() || IsStun())
					return;

				amount = MIN(GetMaxSP() - GetSP(), amount);
				SetSP(GetSP() + amount);
				val = GetSP();
			}
			break;

		case POINT_STAMINA:
			{
				if (IsDead() || IsStun())
					return;

				int prev_val = GetStamina();
				amount = MIN(GetMaxStamina() - GetStamina(), amount);
				SetStamina(GetStamina() + amount);
				val = GetStamina();

				if (val == 0)
				{
					// Stamina°¡ ¾øÀ¸´Ï °ÈÀÚ!
					SetNowWalking(true);
				}
				else if (prev_val == 0)
				{
					// ¾ø´ø ½ºÅ×¹Ì³ª°¡ »ý°åÀ¸´Ï ÀÌÀü ¸ðµå º¹±Í
					ResetWalking();
				}

				if (amount < 0 && val != 0) // °¨¼Ò´Â º¸³»Áö¾Ê´Â´Ù.
					return;
			}
			break;

		case POINT_MAX_HP:
			{
				SetPoint(type, GetPoint(type) + amount);

				//SetMaxHP(GetMaxHP() + amount);
				// ÃÖ´ë »ý¸í·Â = (±âº» ÃÖ´ë »ý¸í·Â + Ãß°¡) * ÃÖ´ë»ý¸í·Â%
				int hp = GetRealPoint(POINT_MAX_HP);
				int add_hp = MIN(3500, hp * GetPoint(POINT_MAX_HP_PCT) / 100);
				add_hp += GetPoint(POINT_MAX_HP);
				add_hp += GetPoint(POINT_PARTY_TANKER_BONUS);

#ifdef ENABLE_YOHARA_SYSTEM
				int max_hp_val = hp + add_hp;

				if (IsPC() && IsSungmaMap() && (GetPoint(POINT_SUNGMA_HP) < GetSungmaMapAttribute(POINT_SUNGMA_HP)))
					max_hp_val /= 2;

				SetMaxHP(max_hp_val);
#else
				SetMaxHP(hp + add_hp);
#endif
				val = GetMaxHP();
			}
			break;

		case POINT_MAX_SP:
			{
				SetPoint(type, GetPoint(type) + amount);

				//SetMaxSP(GetMaxSP() + amount);
				// ÃÖ´ë Á¤½Å·Â = (±âº» ÃÖ´ë Á¤½Å·Â + Ãß°¡) * ÃÖ´ëÁ¤½Å·Â%
				int sp = GetRealPoint(POINT_MAX_SP);
				int add_sp = MIN(800, sp * GetPoint(POINT_MAX_SP_PCT) / 100);
				add_sp += GetPoint(POINT_MAX_SP);
				add_sp += GetPoint(POINT_PARTY_SKILL_MASTER_BONUS);

				SetMaxSP(sp + add_sp);

				val = GetMaxSP();
			}
			break;

		case POINT_MAX_HP_PCT:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			PointChange(POINT_MAX_HP, 0);
			break;

		case POINT_MAX_SP_PCT:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			PointChange(POINT_MAX_SP, 0);
			break;

		case POINT_MAX_STAMINA:
			SetMaxStamina(GetMaxStamina() + amount);
			val = GetMaxStamina();
			break;
#ifdef __GEM_SYSTEM__
		case POINT_GEM:
		{
			const int64_t nTotalGem = static_cast<int64_t>(GetGem()) + static_cast<long long>(amount);
			if (GEM_MAX <= nTotalGem)
			{
				sys_err("[OVERFLOW_GEM] OriGem %d AddedGem %lld id %u Name %s ", GetGem(), amount, GetPlayerID(), GetName());
				return;
			}
			SetGem(GetGem() + amount);
			val = GetGem();
		}
		break;
#endif
		case POINT_GOLD:
		{
			const long long nTotalMoney = static_cast<long long>(GetGold()) + static_cast<long long>(amount);
			if (nTotalMoney > GOLD_MAX || nTotalMoney < 0)
			{
				sys_err("[GOLD_OVERFLOW] name (%s), pid (%u), yang (%lld), amount (%lld), total (%lld).", GetName(), GetPlayerID(), GetGold(), amount, nTotalMoney);
				return;
			}

#ifdef ENABLE_BATTLE_PASS
			if (amount > 0)
			{
				CHARACTER_MANAGER::Instance().DoMission(this, MISSION_EARN_MONEY, amount, 0);
			}
			else if (amount < 0)
			{
				CHARACTER_MANAGER::Instance().DoMission(this, MISSION_SPEND_MONEY, amount, 0);
			}
#endif

			SetGold(nTotalMoney);
			val = GetGold();
#ifdef ENABLE_BATTLEPASS
			UpdateMissionProgress(MISSION_COLLECT_GOLD, amount);
#endif
			RankPlayer::instance().SendInfoPlayer(this, RANK_BY_YANG, val, false);
			break;
		}
#ifdef USE_CHEQUE_CURRENCY
		case POINT_CHEQUE:
		{
			const long long totalCheque = static_cast<long long>(GetCheque()) + amount;
			if (totalCheque < 0 || totalCheque > CHEQUE_MAX)
			{
				sys_err("[CHEQUE_OVERFLOW] name (%s), pid (%u), cheque (%u), amount (%lld), total (%lld).", GetName(), GetPlayerID(), GetCheque(), amount, totalCheque);
				return;
			}

			SetCheque(static_cast<uint16_t>(totalCheque));
			val = GetCheque();
			break;
		}
#endif
		case POINT_SKILL:
		case POINT_STAT:
#ifdef ENABLE_YOHARA_SYSTEM
		case POINT_CONQUEROR_POINT:
#endif
		case POINT_SUB_SKILL:
		case POINT_STAT_RESET_COUNT:
		case POINT_HORSE_SKILL:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			SetRealPoint(type, val);
			break;

		case POINT_DEF_GRADE:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			PointChange(POINT_CLIENT_DEF_GRADE, amount);
			break;

		case POINT_CLIENT_DEF_GRADE:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;

		case POINT_MOV_SPEED:
		{
			#if defined(WJ_COMBAT_ZONE)	
			if (FindAffect(AFFECT_COMBAT_ZONE_MOVEMENT))
			{
				SetPoint(type, COMBAT_ZONE_REDUCTION_MAX_MOVEMENT_SPEED);
				val = GetPoint(type);
				break;
			}
			#endif

			
/*#ifdef ENABLE_YOHARA_SYSTEM
			const int move = GetRealPoint(POINT_MOV_SPEED);
			const int add_move = GetPoint(POINT_MOV_SPEED);
			int move_speed = move + add_move;
			if (IsPC() && IsSungmaMap() && (GetPoint(POINT_SUNGMA_MOVE) < GetSungmaMapAttribute(POINT_SUNGMA_MOVE)))
			{
				move_speed /= 2;

				if (move_speed >= 200)
					move_speed = 200;

				SetPoint(type, move_speed);
			}


			val = GetPoint(type);
#endif*/	
			
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			sys_err("POINT_MOVE_SPEED case %d", val);
		}
		break;

		case POINT_ST:
		case POINT_HT:
		case POINT_DX:
		case POINT_IQ:
		case POINT_HP_REGEN:
		case POINT_SP_REGEN:
		case POINT_ATT_SPEED:
		case POINT_ATT_GRADE:
		case POINT_CASTING_SPEED:
		case POINT_MAGIC_ATT_GRADE:
		case POINT_MAGIC_DEF_GRADE:
		case POINT_BOW_DISTANCE:
		case POINT_HP_RECOVERY:
		case POINT_SP_RECOVERY:

		case POINT_ATTBONUS_HUMAN:	// 42 ÀÎ°£¿¡°Ô °­ÇÔ
		case POINT_ATTBONUS_ANIMAL:	// 43 µ¿¹°¿¡°Ô µ¥¹ÌÁö % Áõ°¡
		case POINT_ATTBONUS_ORC:		// 44 ¿õ±Í¿¡°Ô µ¥¹ÌÁö % Áõ°¡
		case POINT_ATTBONUS_MILGYO:	// 45 ¹Ð±³¿¡°Ô µ¥¹ÌÁö % Áõ°¡
		case POINT_ATTBONUS_UNDEAD:	// 46 ½ÃÃ¼¿¡°Ô µ¥¹ÌÁö % Áõ°¡
		case POINT_ATTBONUS_DEVIL:	// 47 ¸¶±Í(¾Ç¸¶)¿¡°Ô µ¥¹ÌÁö % Áõ°¡

		case POINT_ATTBONUS_MONSTER:
		case POINT_ATTBONUS_SURA:
		case POINT_ATTBONUS_ASSASSIN:
		case POINT_ATTBONUS_WARRIOR:
		case POINT_ATTBONUS_SHAMAN:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_ATTBONUS_WOLFMAN:
#endif

		case POINT_POISON_PCT:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_BLEEDING_PCT:
#endif
		case POINT_STUN_PCT:
		case POINT_SLOW_PCT:

		case POINT_BLOCK:
		case POINT_DODGE:

		case POINT_CRITICAL_PCT:
		case POINT_RESIST_CRITICAL:
		case POINT_PENETRATE_PCT:
		case POINT_RESIST_PENETRATE:
		case POINT_CURSE_PCT:

		case POINT_STEAL_HP:		// 48 »ý¸í·Â Èí¼ö
		case POINT_STEAL_SP:		// 49 Á¤½Å·Â Èí¼ö

		case POINT_MANA_BURN_PCT:	// 50 ¸¶³ª ¹ø
		case POINT_DAMAGE_SP_RECOVER:	// 51 °ø°Ý´çÇÒ ½Ã Á¤½Å·Â È¸º¹ È®·ü
		case POINT_RESIST_NORMAL_DAMAGE:
		case POINT_RESIST_SWORD:
		case POINT_RESIST_TWOHAND:
		case POINT_RESIST_DAGGER:
		case POINT_RESIST_BELL:
		case POINT_RESIST_FAN:
		case POINT_RESIST_BOW:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_RESIST_CLAW:
#endif
		case POINT_RESIST_FIRE:
		case POINT_RESIST_ELEC:
		case POINT_RESIST_MAGIC:
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		case POINT_ACCEDRAIN_RATE:
#endif
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
		case POINT_RESIST_MAGIC_REDUCTION:
#endif
		case POINT_RESIST_WIND:
		case POINT_RESIST_ICE:
		case POINT_RESIST_EARTH:
		case POINT_RESIST_DARK:
		case POINT_REFLECT_MELEE:	// 67 °ø°Ý ¹Ý»ç
		case POINT_REFLECT_CURSE:	// 68 ÀúÁÖ ¹Ý»ç
		case POINT_POISON_REDUCE:	// 69 µ¶µ¥¹ÌÁö °¨¼Ò
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_BLEEDING_REDUCE:
#endif
		case POINT_KILL_SP_RECOVER:	// 70 Àû ¼Ò¸ê½Ã MP È¸º¹
		case POINT_KILL_HP_RECOVERY:	// 75
		case POINT_HIT_HP_RECOVERY:
		case POINT_HIT_SP_RECOVERY:
		case POINT_MANASHIELD:
		case POINT_ATT_BONUS:
		case POINT_DEF_BONUS:
		case POINT_SKILL_DAMAGE_BONUS:
		case POINT_NORMAL_HIT_DAMAGE_BONUS:
#ifdef ELEMENT_NEW_BONUSES
		case POINT_ATTBONUS_ELEC:
		case POINT_ATTBONUS_FIRE:
		case POINT_ATTBONUS_ICE:
		case POINT_ATTBONUS_WIND:
		case POINT_ATTBONUS_EARTH:
		case POINT_ATTBONUS_DARK:
#endif
			// DEPEND_BONUS_ATTRIBUTES
		case POINT_SKILL_DEFEND_BONUS:
		case POINT_NORMAL_HIT_DEFEND_BONUS:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;
			// END_OF_DEPEND_BONUS_ATTRIBUTES

		case POINT_PARTY_ATTACKER_BONUS:
		case POINT_PARTY_TANKER_BONUS:
		case POINT_PARTY_BUFFER_BONUS:
		case POINT_PARTY_SKILL_MASTER_BONUS:
		case POINT_PARTY_HASTE_BONUS:
		case POINT_PARTY_DEFENDER_BONUS:

		case POINT_RESIST_WARRIOR :
		case POINT_RESIST_ASSASSIN :
		case POINT_RESIST_SURA :
		case POINT_RESIST_SHAMAN :
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_RESIST_WOLFMAN :
#endif

#ifdef ENABLE_NEW_BONUS_WORLDARD
		case  POINT_RESIST_HUMAN:
#endif
#ifdef ELEMENT_NEW_BONUSES
		case POINT_ENCHANT_ELEC:
		case POINT_ENCHANT_FIRE:
		case POINT_ENCHANT_ICE:
		case POINT_ENCHANT_WIND:
		case POINT_ENCHANT_EARTH:
		case POINT_ENCHANT_DARK:

		case POINT_ATTBONUS_SWORD:		
		case POINT_ATTBONUS_TWOHAND:	
		case POINT_ATTBONUS_DAGGER:	
		case POINT_ATTBONUS_BELL:		
		case POINT_ATTBONUS_FAN:		
		case POINT_ATTBONUS_BOW:

		case POINT_ATTBONUS_ZODIAC:
		case POINT_ATTBONUS_DESERT:
		case POINT_ATTBONUS_INSECT:	
#endif

#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_ATTBONUS_CLAW:
#endif
#ifdef NEW_BONUS
		case POINT_ATTBONUS_STONE:
		case POINT_ATTBONUS_BOSS:
#endif
#ifdef USE_DROP_AFFECT_BONUSES
		case POINT_DOUBLE_DROP_METIN:
		case POINT_DOUBLE_DROP_BOSS:
#endif
#ifdef USE_PREMIUM_AFFECT
		case POINT_PREMIUM_USER:
#endif
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;

		case POINT_MALL_ATTBONUS:
		case POINT_MALL_DEFBONUS:
		case POINT_MALL_EXPBONUS:
		case POINT_MALL_ITEMBONUS:
		case POINT_MALL_GOLDBONUS:
		case POINT_MELEE_MAGIC_ATT_BONUS_PER:
			if (GetPoint(type) + amount > 100)
			{
				sys_err("MALL_BONUS exceeded over 100!! point type: %d name: %s amount %d", type, GetName(), amount);
				amount = 100 - GetPoint(type);
			}

			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;

			// PC_BANG_ITEM_ADD
		case POINT_PC_BANG_EXP_BONUS :
		case POINT_PC_BANG_DROP_BONUS :
		case POINT_RAMADAN_CANDY_BONUS_EXP:
			SetPoint(type, amount);
			val = GetPoint(type);
			break;
			// END_PC_BANG_ITEM_ADD

		case POINT_EXP_DOUBLE_BONUS:	// 71
		case POINT_GOLD_DOUBLE_BONUS:	// 72
		case POINT_ITEM_DROP_BONUS:	// 73
		case POINT_POTION_BONUS:	// 74
			//SERGIO-GOLD_MAX 
			if (GetPoint(type) + amount > 100)
			{
				sys_err("BONUS exceeded over 100!! point type: %d name: %s amount %d", type, GetName(), amount);
				amount = 100 - GetPoint(type);
			}

			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;

		case POINT_IMMUNE_STUN:		// 76
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			if (val)
			{
				// ChatPacket(CHAT_TYPE_INFO, "IMMUNE_STUN SET_BIT type(%u) amount(%d)", type, amount);
				SET_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_STUN);
			}
			else
			{
				// ChatPacket(CHAT_TYPE_INFO, "IMMUNE_STUN REMOVE_BIT type(%u) amount(%d)", type, amount);
				REMOVE_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_STUN);
			}
			break;

		case POINT_IMMUNE_SLOW:		// 77
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			if (val)
			{
				SET_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_SLOW);
			}
			else
			{
				REMOVE_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_SLOW);
			}
			break;

		case POINT_IMMUNE_FALL:	// 78
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			if (val)
			{
				SET_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_FALL);
			}
			else
			{
				REMOVE_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_FALL);
			}
			break;

		case POINT_ATT_GRADE_BONUS:
			SetPoint(type, GetPoint(type) + amount);
			PointChange(POINT_ATT_GRADE, amount);
			val = GetPoint(type);
			break;

		case POINT_DEF_GRADE_BONUS:
			SetPoint(type, GetPoint(type) + amount);
			PointChange(POINT_DEF_GRADE, amount);
			val = GetPoint(type);
			break;

		case POINT_MAGIC_ATT_GRADE_BONUS:
			SetPoint(type, GetPoint(type) + amount);
			PointChange(POINT_MAGIC_ATT_GRADE, amount);
			val = GetPoint(type);
			break;

		case POINT_MAGIC_DEF_GRADE_BONUS:
			SetPoint(type, GetPoint(type) + amount);
			PointChange(POINT_MAGIC_DEF_GRADE, amount);
			val = GetPoint(type);
			break;

		case POINT_VOICE:
		case POINT_EMPIRE_POINT:
			//sys_err("CHARACTER::PointChange: %s: point cannot be changed. use SetPoint instead (type: %d)", GetName(), type);
			val = GetRealPoint(type);
			break;

		case POINT_POLYMORPH:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			SetPolymorph(val);
			break;

		case POINT_MOUNT:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			MountVnum(val);
			break;

		case POINT_ENERGY:
		case POINT_COSTUME_ATTR_BONUS:
			{
				int old_val = GetPoint(type);
				SetPoint(type, old_val + amount);
				val = GetPoint(type);
				BuffOnAttr_ValueChange(type, old_val, val);
			}
			break;

		default:
			sys_err("CHARACTER::PointChange: %s: unknown point change type %d", GetName(), type);
			return;
	}

	switch (type)
	{
		case POINT_LEVEL:
		case POINT_ST:
		case POINT_DX:
		case POINT_IQ:
		case POINT_HT:
#ifdef ENABLE_YOHARA_SYSTEM
		case POINT_CONQUEROR_LEVEL:
		case POINT_SUNGMA_STR:
		case POINT_SUNGMA_HP:
		case POINT_SUNGMA_MOVE:
		case POINT_SUNGMA_IMMUNE:
#endif

			ComputeBattlePoints();
			break;
		case POINT_MAX_HP:
		case POINT_MAX_SP:
		case POINT_MAX_STAMINA:
			break;
	}

	if (type == POINT_HP && amount == 0)
		return;

	if (GetDesc())
	{
		struct packet_point_change pack;

		pack.header = HEADER_GC_CHARACTER_POINT_CHANGE;
		pack.dwVID = m_vid;
		pack.type = type;
		pack.value = val;

		if (bAmount)
			pack.amount = amount;
		else
			pack.amount = 0;

		if (!bBroadcast)
			GetDesc()->Packet(&pack, sizeof(struct packet_point_change));
		else
			PacketAround(&pack, sizeof(pack));
	}
}
#ifdef __NEWPET_SYSTEM__
void CHARACTER::SendPetLevelUpEffect(int vid, int type, int value, int amount) {
	struct packet_point_change pack;

	pack.header = HEADER_GC_CHARACTER_POINT_CHANGE;
	pack.dwVID = vid;
	pack.type = type;
	pack.value = value;
	pack.amount = amount;
	PacketAround(&pack, sizeof(pack));
}
#endif
void CHARACTER::ApplyPoint(BYTE bApplyType, int iVal)
{
	switch (bApplyType)
	{
		case APPLY_NONE:			// 0
			break;;

		case APPLY_CON:
			PointChange(POINT_HT, iVal);
			PointChange(POINT_MAX_HP, (iVal * JobInitialPoints[GetJob()].hp_per_ht));
			PointChange(POINT_MAX_STAMINA, (iVal * JobInitialPoints[GetJob()].stamina_per_con));
			break;

		case APPLY_INT:
			PointChange(POINT_IQ, iVal);
			PointChange(POINT_MAX_SP, (iVal * JobInitialPoints[GetJob()].sp_per_iq));
			break;

		case APPLY_SKILL:
			// SKILL_DAMAGE_BONUS
			{
				// ÃÖ»óÀ§ ºñÆ® ±âÁØÀ¸·Î 8ºñÆ® vnum, 9ºñÆ® add, 15ºñÆ® change
				// 00000000 00000000 00000000 00000000
				// ^^^^^^^^  ^^^^^^^^^^^^^^^^^^^^^^^^^
				// vnum     ^ add       change
				BYTE bSkillVnum = (BYTE) (((DWORD)iVal) >> 24);
				int iAdd = iVal & 0x00800000;
				int iChange = iVal & 0x007fffff;

				sys_log(1, "APPLY_SKILL skill %d add? %d change %d", bSkillVnum, iAdd ? 1 : 0, iChange);

				if (0 == iAdd)
					iChange = -iChange;

				boost::unordered_map<BYTE, int>::iterator iter = m_SkillDamageBonus.find(bSkillVnum);

				if (iter == m_SkillDamageBonus.end())
					m_SkillDamageBonus.insert(std::make_pair(bSkillVnum, iChange));
				else
					iter->second += iChange;
			}
			// END_OF_SKILL_DAMAGE_BONUS
			break;

		// NOTE: ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ÃÖ´ëHP º¸³Ê½º³ª Äù½ºÆ® º¸»ó º¸³Ê½º°¡ ¶È°°Àº ¹æ½ÄÀ» »ç¿ëÇÏ¹Ç·Î
		// ±×³É MAX_HP¸¸ °è»êÇÏ¸é Äù½ºÆ® º¸»óÀÇ °æ¿ì ¹®Á¦°¡ »ý±è. »ç½Ç ¿ø·¡ ÀÌÂÊÀÌ ÇÕ¸®ÀûÀÌ±âµµ ÇÏ°í..
		// ¹Ù²Û °ø½ÄÀº ÇöÀç ÃÖ´ë hp¿Í º¸À¯ hpÀÇ ºñÀ²À» ±¸ÇÑ µÚ ¹Ù²ð ÃÖ´ë hp¸¦ ±âÁØÀ¸·Î hp¸¦ º¸Á¤ÇÑ´Ù.
		// ¿ø·¡ PointChange¿¡¼­ ÇÏ´Â°Ô ÁÁÀ»°Í °°Àºµ¥ ¼³°è ¹®Á¦·Î ¾î·Á¿ö¼­ skip..
		// SPµµ ¶È°°ÀÌ °è»êÇÑ´Ù.
		// Mantis : 101460			~ ity ~
		case APPLY_MAX_HP:
		case APPLY_MAX_HP_PCT:
			{
				int i = GetMaxHP(); if(i == 0) break;
				PointChange(aApplyInfo[bApplyType].bPointType, iVal);
				float fRatio = (float)GetMaxHP() / (float)i;
				PointChange(POINT_HP, GetHP() * fRatio - GetHP());
			}
			break;

		case APPLY_MAX_SP:
		case APPLY_MAX_SP_PCT:
			{
				int i = GetMaxSP(); if(i == 0) break;
				PointChange(aApplyInfo[bApplyType].bPointType, iVal);
				float fRatio = (float)GetMaxSP() / (float)i;
				PointChange(POINT_SP, GetSP() * fRatio - GetSP());
			}
			break;

		case APPLY_STR:
		case APPLY_DEX:
		case APPLY_ATT_SPEED:
		case APPLY_MOV_SPEED:
		case APPLY_CAST_SPEED:
		case APPLY_HP_REGEN:
		case APPLY_SP_REGEN:
		case APPLY_POISON_PCT:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case APPLY_BLEEDING_PCT:
#endif
		case APPLY_STUN_PCT:
		case APPLY_SLOW_PCT:
		case APPLY_CRITICAL_PCT:
		case APPLY_PENETRATE_PCT:
		case APPLY_ATTBONUS_HUMAN:
		case APPLY_ATTBONUS_ANIMAL:
		case APPLY_ATTBONUS_ORC:
		case APPLY_ATTBONUS_MILGYO:
		case APPLY_ATTBONUS_UNDEAD:
		case APPLY_ATTBONUS_DEVIL:
		case APPLY_ATTBONUS_WARRIOR:	// 59
		case APPLY_ATTBONUS_ASSASSIN:	// 60
		case APPLY_ATTBONUS_SURA:	// 61
		case APPLY_ATTBONUS_SHAMAN:	// 62
#ifdef ENABLE_WOLFMAN_CHARACTER
		case APPLY_ATTBONUS_WOLFMAN:
#endif
		case APPLY_ATTBONUS_MONSTER:	// 63
		case APPLY_STEAL_HP:
		case APPLY_STEAL_SP:
		case APPLY_MANA_BURN_PCT:
		case APPLY_DAMAGE_SP_RECOVER:
		case APPLY_BLOCK:
		case APPLY_DODGE:
		case APPLY_RESIST_SWORD:
		case APPLY_RESIST_TWOHAND:
		case APPLY_RESIST_DAGGER:
		case APPLY_RESIST_BELL:
		case APPLY_RESIST_FAN:
		case APPLY_RESIST_BOW:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case APPLY_RESIST_CLAW:
#endif
		case APPLY_RESIST_FIRE:
		case APPLY_RESIST_ELEC:
		case APPLY_RESIST_MAGIC:
		case APPLY_RESIST_WIND:
		case APPLY_RESIST_ICE:
		case APPLY_RESIST_EARTH:
		case APPLY_RESIST_DARK:
		case APPLY_REFLECT_MELEE:
		case APPLY_REFLECT_CURSE:
		case APPLY_ANTI_CRITICAL_PCT:
		case APPLY_ANTI_PENETRATE_PCT:
		case APPLY_POISON_REDUCE:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case APPLY_BLEEDING_REDUCE:
#endif
		case APPLY_KILL_SP_RECOVER:
		case APPLY_EXP_DOUBLE_BONUS:
		case APPLY_GOLD_DOUBLE_BONUS:
		case APPLY_ITEM_DROP_BONUS:
		case APPLY_POTION_BONUS:
		case APPLY_KILL_HP_RECOVER:
		case APPLY_IMMUNE_STUN:
		case APPLY_IMMUNE_SLOW:
		case APPLY_IMMUNE_FALL:
		case APPLY_BOW_DISTANCE:
		case APPLY_ATT_GRADE_BONUS:
		case APPLY_DEF_GRADE_BONUS:
		case APPLY_MAGIC_ATT_GRADE:
		case APPLY_MAGIC_DEF_GRADE:
		case APPLY_CURSE_PCT:
		case APPLY_MAX_STAMINA:
		case APPLY_MALL_ATTBONUS:
		case APPLY_MALL_DEFBONUS:
		case APPLY_MALL_EXPBONUS:
		case APPLY_MALL_ITEMBONUS:
		case APPLY_MALL_GOLDBONUS:
		case APPLY_SKILL_DAMAGE_BONUS:
		case APPLY_NORMAL_HIT_DAMAGE_BONUS:
			// DEPEND_BONUS_ATTRIBUTES
		case APPLY_SKILL_DEFEND_BONUS:
		case APPLY_NORMAL_HIT_DEFEND_BONUS:
			// END_OF_DEPEND_BONUS_ATTRIBUTES

		case APPLY_PC_BANG_EXP_BONUS :
		case APPLY_PC_BANG_DROP_BONUS :

		case APPLY_RESIST_WARRIOR :
		case APPLY_RESIST_ASSASSIN :
		case APPLY_RESIST_SURA :
		case APPLY_RESIST_SHAMAN :
#ifdef ENABLE_WOLFMAN_CHARACTER
		case APPLY_RESIST_WOLFMAN :
#endif
		case APPLY_ENERGY:					// 82 ±â·Â
		case APPLY_DEF_GRADE:				// 83 ¹æ¾î·Â. DEF_GRADE_BONUS´Â Å¬¶ó¿¡¼­ µÎ¹è·Î º¸¿©Áö´Â ÀÇµµµÈ ¹ö±×(...)°¡ ÀÖ´Ù.
		case APPLY_COSTUME_ATTR_BONUS:		// 84 ÄÚ½ºÆ¬ ¾ÆÀÌÅÛ¿¡ ºÙÀº ¼Ó¼ºÄ¡ º¸³Ê½º
		case APPLY_MAGIC_ATTBONUS_PER:		// 85 ¸¶¹ý °ø°Ý·Â +x%
		case APPLY_MELEE_MAGIC_ATTBONUS_PER:			// 86 ¸¶¹ý + ¹Ð¸® °ø°Ý·Â +x%
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		case APPLY_ACCEDRAIN_RATE:			//97
#endif
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
		case APPLY_RESIST_MAGIC_REDUCTION:	//98
#endif

#ifdef ENABLE_NEW_BONUS_WORLDARD
		case APPLY_RESIST_HUMAN:
#endif
#ifdef ELEMENT_NEW_BONUSES
		case APPLY_ATTBONUS_ELEC:
		case APPLY_ATTBONUS_FIRE:
		case APPLY_ATTBONUS_ICE:
		case APPLY_ATTBONUS_WIND:
		case APPLY_ATTBONUS_EARTH:
		case APPLY_ATTBONUS_DARK:
#endif
#ifdef ENABLE_YOHARA_SYSTEM
		case APPLY_SUNGMA_STR:
		case APPLY_SUNGMA_HP:
		case APPLY_SUNGMA_MOVE:
		case APPLY_SUNGMA_IMMUNE:
#endif
#ifdef ELEMENT_NEW_BONUSES
		case APPLY_ENCHANT_ELEC: // 99
		case APPLY_ENCHANT_FIRE: // 100
		case APPLY_ENCHANT_ICE: // 101
		case APPLY_ENCHANT_WIND: // 102
		case APPLY_ENCHANT_EARTH: // 103
		case APPLY_ENCHANT_DARK: // 104

		case APPLY_ATTBONUS_SWORD: //106		
		case APPLY_ATTBONUS_TWOHAND: //107
		case APPLY_ATTBONUS_DAGGER:	//108
		case APPLY_ATTBONUS_BELL: //109		
		case APPLY_ATTBONUS_FAN: //110		
		case APPLY_ATTBONUS_BOW: //111

		case APPLY_ATTBONUS_ZODIAC: //112
		case APPLY_ATTBONUS_DESERT: //113
		case APPLY_ATTBONUS_INSECT: //114	
#endif
#ifdef ENABLE_WOLFMAN_CHARACTER
		case APPLY_ATTBONUS_CLAW: //115
#endif

#ifdef NEW_BONUS
		case APPLY_ATTBONUS_STONE:
		case APPLY_ATTBONUS_BOSS:
#endif
#ifdef USE_DROP_AFFECT_BONUSES
		case APPLY_DOUBLE_DROP_METIN:
		case APPLY_DOUBLE_DROP_BOSS:
#endif
			PointChange(aApplyInfo[bApplyType].bPointType, iVal);
			break;

		default:
			sys_err("Unknown apply type %d name %s", bApplyType, GetName());
			break;
	}
}

void CHARACTER::MotionPacketEncode(BYTE motion, LPCHARACTER victim, struct packet_motion * packet)
{
	packet->header	= HEADER_GC_MOTION;
	packet->vid		= m_vid;
	packet->motion	= motion;

	if (victim)
		packet->victim_vid = victim->GetVID();
	else
		packet->victim_vid = 0;
}

void CHARACTER::Motion(BYTE motion, LPCHARACTER victim)
{
	struct packet_motion pack_motion;
	MotionPacketEncode(motion, victim, &pack_motion);
	PacketAround(&pack_motion, sizeof(struct packet_motion));
}

EVENTFUNC(save_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info);
	if ( info == NULL )
	{
		sys_err( "save_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER ch = info->ch;

	if (ch == NULL) { // <Factor>
		return 0;
	}
	sys_log(1, "SAVE_EVENT: %s", ch->GetName());
	ch->Save();
	ch->FlushDelayedSaveItem();
	return (save_event_second_cycle);
}

void CHARACTER::StartSaveEvent()
{
	if (m_pkSaveEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;
	m_pkSaveEvent = event_create(save_event, info, save_event_second_cycle);
}

void CHARACTER::MonsterLog(const char* format, ...)
{
	if (!test_server)
		return;

	if (IsPC())
		return;

	char chatbuf[CHAT_MAX_LEN + 1];
	int len = snprintf(chatbuf, sizeof(chatbuf), "%u)", (DWORD)GetVID());

	if (len < 0 || len >= (int) sizeof(chatbuf))
		len = sizeof(chatbuf) - 1;

	va_list args;

	va_start(args, format);

	int len2 = vsnprintf(chatbuf + len, sizeof(chatbuf) - len, format, args);

	if (len2 < 0 || len2 >= (int) sizeof(chatbuf) - len)
		len += (sizeof(chatbuf) - len) - 1;
	else
		len += len2;

	// \0 ¹®ÀÚ Æ÷ÇÔ
	++len;

	va_end(args);

	TPacketGCChat pack_chat;

	pack_chat.header    = HEADER_GC_CHAT;
	pack_chat.size		= sizeof(TPacketGCChat) + len;
	pack_chat.type      = CHAT_TYPE_TALKING;
	pack_chat.id        = (DWORD)GetVID();
	pack_chat.bEmpire	= 0;

	TEMP_BUFFER buf;
	buf.write(&pack_chat, sizeof(TPacketGCChat));
	buf.write(chatbuf, len);

	CHARACTER_MANAGER::instance().PacketMonsterLog(this, buf.read_peek(), buf.size());
}

void CHARACTER::ChatPacket(BYTE type, const char * format, ...)
{
	LPDESC d = GetDesc();

	if (!d || !format)
		return;

	char chatbuf[CHAT_MAX_LEN + 1];
	va_list args;

	va_start(args, format);
	int len = vsnprintf(chatbuf, sizeof(chatbuf), format, args);
	va_end(args);

	struct packet_chat pack_chat;

	pack_chat.header    = HEADER_GC_CHAT;
	pack_chat.size      = sizeof(struct packet_chat) + len;
	pack_chat.type      = type;
	pack_chat.id        = 0;
	pack_chat.bEmpire   = d->GetEmpire();

	TEMP_BUFFER buf;
	buf.write(&pack_chat, sizeof(struct packet_chat));
	buf.write(chatbuf, len);

	d->Packet(buf.read_peek(), buf.size());

	if (type == CHAT_TYPE_COMMAND && test_server)
		sys_log(0, "SEND_COMMAND %s %s", GetName(), chatbuf);
}

// MINING
void CHARACTER::mining_take()
{
	m_pkMiningEvent = NULL;
}

void CHARACTER::mining_cancel()
{
	if (m_pkMiningEvent)
	{
		sys_log(0, "XXX MINING CANCEL");
		event_cancel(&m_pkMiningEvent);
		ChatPacket(CHAT_TYPE_INFO, "39");
	}
}

void CHARACTER::mining(LPCHARACTER chLoad)
{
	if (m_pkMiningEvent)
	{
		mining_cancel();
		return;
	}

	if (!chLoad)
		return;

	// @fixme128
	if (GetMapIndex() != chLoad->GetMapIndex() || DISTANCE_APPROX(GetX() - chLoad->GetX(), GetY() - chLoad->GetY()) > 1000)
		return;

	if (mining::GetRawOreFromLoad(chLoad->GetRaceNum()) == 0)
		return;

	LPITEM pick = GetWear(WEAR_WEAPON);

	if (!pick || pick->GetType() != ITEM_PICK)
	{
		ChatPacket(CHAT_TYPE_INFO, "40");
		return;
	}

	int count = number(5, 15); // µ¿ÀÛ È½¼ö, ÇÑ µ¿ÀÛ´ç 2ÃÊ

	// Ã¤±¤ µ¿ÀÛÀ» º¸¿©ÁÜ
	TPacketGCDigMotion p;
	p.header = HEADER_GC_DIG_MOTION;
	p.vid = GetVID();
	p.target_vid = chLoad->GetVID();
	p.count = count;

	PacketAround(&p, sizeof(p));

	m_pkMiningEvent = mining::CreateMiningEvent(this, chLoad, count);
}
// END_OF_MINING
#ifdef ENABLE_CSHIELD
bool CHARACTER::IsNearWater() const
{
	if (!GetSectree())
		return false;

	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			if (IS_SET(GetSectree()->GetAttribute(GetX() + x * 100, GetY() + y * 100), ATTR_WATER))
				return true;
		}
	}

	return false;
}
#endif
void CHARACTER::fishing(uint32_t fishing_key)
{
#ifdef ENABLE_CSHIELD
	if (!IsNearWater())
		return;
#endif

	if ((thecore_pulse() - GetLastFishingTime()) < PASSES_PER_SEC(4))
	{
		ChatPacket(CHAT_TYPE_INFO, "[LS;16]");
		return;
	}
	
	if (m_pkFishingEvent)
	{
		fishing_take(fishing_key);
		return;
	}

	{
		LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(GetMapIndex());

		int	x = GetX();
		int y = GetY();

		LPSECTREE tree = pkSectreeMap->Find(x, y);
		DWORD dwAttr = tree->GetAttribute(x, y);

		if (IS_SET(dwAttr, ATTR_BLOCK))
		{
			ChatPacket(CHAT_TYPE_INFO, "41");
			return;
		}
	}

	LPITEM rod = GetWear(WEAR_WEAPON);

	// ³¬½Ã´ë ÀåÂø
	if (!rod || rod->GetType() != ITEM_ROD)
	{
		ChatPacket(CHAT_TYPE_INFO, "42");
		return;
	}

	if (0 == rod->GetSocket(2))
	{
		ChatPacket(CHAT_TYPE_INFO, "43");
		return;
	}

	float fx, fy;
	GetDeltaByDegree(GetRotation(), 400.0f, &fx, &fy);
#if defined(ENABLE_FISHING_GAME)
	SetFishingGameGoals(0);
#endif

	m_pkFishingEvent = fishing::CreateFishingEvent(this);
}

void CHARACTER::fishing_take(uint32_t fishing_key)
{
	LPITEM rod = GetWear(WEAR_WEAPON);
	if (rod && rod->GetType() == ITEM_ROD)
	{
		using fishing::fishing_event_info;
		if (m_pkFishingEvent)
		{
			struct fishing_event_info* info = dynamic_cast<struct fishing_event_info*>(m_pkFishingEvent->info);

			if (info)
				fishing::Take(info, this, fishing_key);
		}
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, "44");
	}

	event_cancel(&m_pkFishingEvent);
}

bool CHARACTER::StartStateMachine(int iNextPulse)
{
	if (CHARACTER_MANAGER::instance().AddToStateList(this))
	{
		m_dwNextStatePulse = thecore_heart->pulse + iNextPulse;
		return true;
	}

	return false;
}

void CHARACTER::StopStateMachine()
{
	CHARACTER_MANAGER::instance().RemoveFromStateList(this);
}

void CHARACTER::UpdateStateMachine(DWORD dwPulse)
{
	if (dwPulse < m_dwNextStatePulse)
		return;

	if (IsDead())
		return;

	Update();
	m_dwNextStatePulse = dwPulse + m_dwStateDuration;
}

void CHARACTER::SetNextStatePulse(int iNextPulse)
{
	CHARACTER_MANAGER::instance().AddToStateList(this);
	m_dwNextStatePulse = iNextPulse;

	if (iNextPulse < 10)
		MonsterLog("´ÙÀ½»óÅÂ·Î¾î¼­°¡ÀÚ");
}


// Ä³¸¯ÅÍ ÀÎ½ºÅÏ½º ¾÷µ¥ÀÌÆ® ÇÔ¼ö.
void CHARACTER::UpdateCharacter(DWORD dwPulse)
{
	CFSM::Update();
}

void CHARACTER::SetShop(LPSHOP pkShop)
{
	if ((m_pkShop = pkShop))
		SET_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_SHOP);
	else
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_SHOP);
}

void CHARACTER::SetExchange(CExchange * pkExchange)
{
	m_pkExchange = pkExchange;
}

void CHARACTER::SetPart(BYTE bPartPos, uint32_t wVal)
{
	assert(bPartPos < PART_MAX_NUM);
	m_pointsInstant.parts[bPartPos] = wVal;
}

uint32_t CHARACTER::GetPart(BYTE bPartPos) const
{
	assert(bPartPos < PART_MAX_NUM);
	return m_pointsInstant.parts[bPartPos];
}

uint32_t CHARACTER::GetOriginalPart(BYTE bPartPos) const
{
	switch (bPartPos)
	{
		case PART_MAIN:
			if (!IsPC()) // PC°¡ ¾Æ´Ñ °æ¿ì ÇöÀç ÆÄÆ®¸¦ ±×´ë·Î ¸®ÅÏ
				return GetPart(PART_MAIN);
			else
				return m_pointsInstant.bBasePart;

		case PART_HAIR:
			return GetPart(PART_HAIR);
#ifdef __SASH_SYSTEM__
		case PART_SASH:
			return GetPart(PART_SASH);
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		case PART_WEAPON:
			return GetPart(PART_WEAPON);
#endif

		default:
			return 0;
	}
}

BYTE CHARACTER::GetCharType() const
{
	return m_bCharType;
}

bool CHARACTER::SetSyncOwner(LPCHARACTER ch, bool bRemoveFromList)
{
	// TRENT_MONSTER
	if (IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE))
		return false;
	// END_OF_TRENT_MONSTER
	
	if (ch) // @fixme131
	{
		if (!battle_is_attackable(ch, this))
		{
			SendDamagePacket(ch, 0, DAMAGE_BLOCK);
			return false;
		}
	}

	if (ch == this)
	{
		sys_err("SetSyncOwner owner == this (%p)", this);
		return false;
	}

	if (!ch)
	{
		if (bRemoveFromList && m_pkChrSyncOwner)
		{
			m_pkChrSyncOwner->m_kLst_pkChrSyncOwned.remove(this);
		}

		if (m_pkChrSyncOwner)
			sys_log(1, "SyncRelease %s %p from %s", GetName(), this, m_pkChrSyncOwner->GetName());

		// ¸®½ºÆ®¿¡¼­ Á¦°ÅÇÏÁö ¾Ê´õ¶óµµ Æ÷ÀÎÅÍ´Â NULL·Î ¼ÂÆÃµÇ¾î¾ß ÇÑ´Ù.
		m_pkChrSyncOwner = NULL;
	}
	else
	{
		if (!IsSyncOwner(ch))
			return false;

		// °Å¸®°¡ 200 ÀÌ»óÀÌ¸é SyncOwner°¡ µÉ ¼ö ¾ø´Ù.
		if (DISTANCE_APPROX(GetX() - ch->GetX(), GetY() - ch->GetY()) > 250)
		{
			sys_log(1, "SetSyncOwner distance over than 250 %s %s", GetName(), ch->GetName());

			// SyncOwnerÀÏ °æ¿ì Owner·Î Ç¥½ÃÇÑ´Ù.
			if (m_pkChrSyncOwner == ch)
				return true;

			return false;
		}

		if (m_pkChrSyncOwner != ch)
		{
			if (m_pkChrSyncOwner)
			{
				sys_log(1, "SyncRelease %s %p from %s", GetName(), this, m_pkChrSyncOwner->GetName());
				m_pkChrSyncOwner->m_kLst_pkChrSyncOwned.remove(this);
			}

			m_pkChrSyncOwner = ch;
			m_pkChrSyncOwner->m_kLst_pkChrSyncOwned.push_back(this);

			// SyncOwner°¡ ¹Ù²î¸é LastSyncTimeÀ» ÃÊ±âÈ­ÇÑ´Ù.
			static const timeval zero_tv = {0, 0};
			SetLastSyncTime(zero_tv);

			sys_log(1, "SetSyncOwner set %s %p to %s", GetName(), this, ch->GetName());
		}

		m_fSyncTime = get_float_time();
	}

	// TODO: Sync Owner°¡ °°´õ¶óµµ °è¼Ó ÆÐÅ¶À» º¸³»°í ÀÖÀ¸¹Ç·Î,
	//       µ¿±âÈ­ µÈ ½Ã°£ÀÌ 3ÃÊ ÀÌ»ó Áö³µÀ» ¶§ Ç®¾îÁÖ´Â ÆÐÅ¶À»
	//       º¸³»´Â ¹æ½ÄÀ¸·Î ÇÏ¸é ÆÐÅ¶À» ÁÙÀÏ ¼ö ÀÖ´Ù.
	TPacketGCOwnership pack;

	pack.bHeader	= HEADER_GC_OWNERSHIP;
	pack.dwOwnerVID	= ch ? ch->GetVID() : 0;
	pack.dwVictimVID	= GetVID();

	PacketAround(&pack, sizeof(TPacketGCOwnership));
	return true;
}

struct FuncClearSync
{
	void operator () (LPCHARACTER ch)
	{
		assert(ch != NULL);
		ch->SetSyncOwner(NULL, false);	// false ÇÃ·¡±×·Î ÇØ¾ß for_each °¡ Á¦´ë·Î µ·´Ù.
	}
};

void CHARACTER::ClearSync()
{
	SetSyncOwner(NULL);

	// ¾Æ·¡ for_each¿¡¼­ ³ª¸¦ m_pkChrSyncOwner·Î °¡Áø ÀÚµéÀÇ Æ÷ÀÎÅÍ¸¦ NULL·Î ÇÑ´Ù.
	std::for_each(m_kLst_pkChrSyncOwned.begin(), m_kLst_pkChrSyncOwned.end(), FuncClearSync());
	m_kLst_pkChrSyncOwned.clear();
}

bool CHARACTER::IsSyncOwner(LPCHARACTER ch) const
{
	if (m_pkChrSyncOwner == ch)
		return true;

	// ¸¶Áö¸·À¸·Î µ¿±âÈ­ µÈ ½Ã°£ÀÌ 3ÃÊ ÀÌ»ó Áö³µ´Ù¸é ¼ÒÀ¯±ÇÀÌ ¾Æ¹«¿¡°Ôµµ
	// ¾ø´Ù. µû¶ó¼­ ¾Æ¹«³ª SyncOwnerÀÌ¹Ç·Î true ¸®ÅÏ
	if (get_float_time() - m_fSyncTime >= 3.0f)
		return true;

	return false;
}

void CHARACTER::SetParty(LPPARTY pkParty)
{
	if (pkParty == m_pkParty)
		return;

	if (pkParty && m_pkParty)
		sys_err("%s is trying to reassigning party (current %p, new party %p)", GetName(), get_pointer(m_pkParty), get_pointer(pkParty));

	sys_log(1, "PARTY set to %p", get_pointer(pkParty));

	//if (m_pkDungeon && IsPC())
	//SetDungeon(NULL);
	m_pkParty = pkParty;

	if (IsPC())
	{
		if (m_pkParty)
			SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_PARTY);
		else
			REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_PARTY);

		UpdatePacket();
	}
}

// PARTY_JOIN_BUG_FIX
/// ÆÄÆ¼ °¡ÀÔ event Á¤º¸
EVENTINFO(TPartyJoinEventInfo)
{
	DWORD	dwGuestPID;		///< ÆÄÆ¼¿¡ Âü¿©ÇÒ Ä³¸¯ÅÍÀÇ PID
	DWORD	dwLeaderPID;		///< ÆÄÆ¼ ¸®´õÀÇ PID

	TPartyJoinEventInfo()
	: dwGuestPID( 0 )
	, dwLeaderPID( 0 )
	{
	}
} ;

EVENTFUNC(party_request_event)
{
	TPartyJoinEventInfo * info = dynamic_cast<TPartyJoinEventInfo *>(  event->info);

	if ( info == NULL )
	{
		sys_err( "party_request_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(info->dwGuestPID);

	if (ch)
	{
		sys_log(0, "PartyRequestEvent %s", ch->GetName());
		ch->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
		ch->SetPartyRequestEvent(NULL);
	}

	return 0;
}

bool CHARACTER::RequestToParty(LPCHARACTER leader)
{
	if (leader->GetParty())
		leader = leader->GetParty()->GetLeaderCharacter();

	if (!leader)
	{
		ChatPacket(CHAT_TYPE_INFO, "45");
		return false;
	}

	if (m_pkPartyRequestEvent)
		return false;

	if (!IsPC() || !leader->IsPC())
		return false;

	if (leader->IsBlockMode(BLOCK_PARTY_REQUEST))
		return false;
#if defined(WJ_COMBAT_ZONE)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		return false;
#endif
	PartyJoinErrCode errcode = IsPartyJoinableCondition(leader, this);

	switch (errcode)
	{
		case PERR_NONE:
			break;

		case PERR_SERVER:
			ChatPacket(CHAT_TYPE_INFO, "46");
			return false;

		case PERR_DIFFEMPIRE:
			ChatPacket(CHAT_TYPE_INFO, "47");
			return false;

		case PERR_DUNGEON:
			ChatPacket(CHAT_TYPE_INFO, "48");
			return false;

		case PERR_OBSERVER:
			ChatPacket(CHAT_TYPE_INFO, "49");
			return false;

		case PERR_LVBOUNDARY:
			ChatPacket(CHAT_TYPE_INFO, "50");
			return false;

		case PERR_LOWLEVEL:
			ChatPacket(CHAT_TYPE_INFO, "51");
			return false;

		case PERR_HILEVEL:
			ChatPacket(CHAT_TYPE_INFO, "52");
			return false;

		case PERR_ALREADYJOIN:
			return false;

		case PERR_PARTYISFULL:
			ChatPacket(CHAT_TYPE_INFO, "53");
			return false;

		default:
			sys_err("Do not process party join error(%d)", errcode);
			return false;
	}

	TPartyJoinEventInfo* info = AllocEventInfo<TPartyJoinEventInfo>();

	info->dwGuestPID = GetPlayerID();
	info->dwLeaderPID = leader->GetPlayerID();

	SetPartyRequestEvent(event_create(party_request_event, info, PASSES_PER_SEC(10)));

	leader->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequest %u", (DWORD) GetVID());
	ChatPacket(CHAT_TYPE_INFO, "54 %s", leader->GetName());
	return true;
}

void CHARACTER::DenyToParty(LPCHARACTER member)
{
	sys_log(1, "DenyToParty %s member %s %p", GetName(), member->GetName(), get_pointer(member->m_pkPartyRequestEvent));

	if (!member->m_pkPartyRequestEvent)
		return;

	TPartyJoinEventInfo * info = dynamic_cast<TPartyJoinEventInfo *>(member->m_pkPartyRequestEvent->info);

	if (!info)
	{
		sys_err( "CHARACTER::DenyToParty> <Factor> Null pointer");
		return;
	}

	if (info->dwGuestPID != member->GetPlayerID())
		return;

	if (info->dwLeaderPID != GetPlayerID())
		return;

	event_cancel(&member->m_pkPartyRequestEvent);

	member->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

void CHARACTER::AcceptToParty(LPCHARACTER member)
{
	sys_log(1, "AcceptToParty %s member %s %p", GetName(), member->GetName(), get_pointer(member->m_pkPartyRequestEvent));

	if (!member->m_pkPartyRequestEvent)
		return;

	TPartyJoinEventInfo * info = dynamic_cast<TPartyJoinEventInfo *>(member->m_pkPartyRequestEvent->info);

	if (!info)
	{
		sys_err( "CHARACTER::AcceptToParty> <Factor> Null pointer");
		return;
	}

	if (info->dwGuestPID != member->GetPlayerID())
		return;

	if (info->dwLeaderPID != GetPlayerID())
		return;

	event_cancel(&member->m_pkPartyRequestEvent);

	if (!GetParty())
		member->ChatPacket(CHAT_TYPE_INFO, "55");
	else
	{
		if (GetPlayerID() != GetParty()->GetLeaderPID())
			return;

		PartyJoinErrCode errcode = IsPartyJoinableCondition(this, member);
		switch (errcode)
		{
			case PERR_NONE: 		member->PartyJoin(this); return;
			case PERR_SERVER:		member->ChatPacket(CHAT_TYPE_INFO, "46"); break;
			case PERR_DUNGEON:		member->ChatPacket(CHAT_TYPE_INFO, "48"); break;
			case PERR_OBSERVER: 	member->ChatPacket(CHAT_TYPE_INFO, "49"); break;
			case PERR_LVBOUNDARY:	member->ChatPacket(CHAT_TYPE_INFO, "50"); break;
			case PERR_LOWLEVEL: 	member->ChatPacket(CHAT_TYPE_INFO, "51"); break;
			case PERR_HILEVEL: 		member->ChatPacket(CHAT_TYPE_INFO, "52"); break;
			case PERR_ALREADYJOIN: 	break;
			case PERR_PARTYISFULL: {
									   ChatPacket(CHAT_TYPE_INFO, "53");
									   member->ChatPacket(CHAT_TYPE_INFO, "56");
									   break;
								   }
			default: sys_err("Do not process party join error(%d)", errcode);
		}
	}

	member->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

/**
 * ÆÄÆ¼ ÃÊ´ë event callback ÇÔ¼ö.
 * event °¡ ¹ßµ¿ÇÏ¸é ÃÊ´ë °ÅÀý·Î Ã³¸®ÇÑ´Ù.
 */
EVENTFUNC(party_invite_event)
{
	TPartyJoinEventInfo * pInfo = dynamic_cast<TPartyJoinEventInfo *>(  event->info);

	if ( pInfo == NULL )
	{
		sys_err( "party_invite_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER pchInviter = CHARACTER_MANAGER::instance().FindByPID(pInfo->dwLeaderPID);

	if (pchInviter)
	{
		sys_log(1, "PartyInviteEvent %s", pchInviter->GetName());
		pchInviter->PartyInviteDeny(pInfo->dwGuestPID);
	}

	return 0;
}

void CHARACTER::PartyInvite(LPCHARACTER pchInvitee)
{
#if defined(WJ_COMBAT_ZONE)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		return;
#endif

#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(this, TOURNAMENT_BLOCK_PARTY))
		return;
#endif


#ifdef __DOJANG_SRC_FUNCTIONS__
	if (GetMapIndex() == DOJANG_MAPINDEX)
		return;

	if (pchInvitee->GetMapIndex() == DOJANG_MAPINDEX)
		return;
#endif

	if (GetParty() && GetParty()->GetLeaderPID() != GetPlayerID())
	{
		ChatPacket(CHAT_TYPE_INFO, "57");
		return;
	}
	else if (pchInvitee->IsBlockMode(BLOCK_PARTY_INVITE))
	{
		ChatPacket(CHAT_TYPE_INFO, "58 %s", pchInvitee->GetName());
		return;
	}

	PartyJoinErrCode errcode = IsPartyJoinableCondition(this, pchInvitee);

	switch (errcode)
	{
		case PERR_NONE:
			break;

		case PERR_SERVER:
			ChatPacket(CHAT_TYPE_INFO, "46");
			return;

		case PERR_DIFFEMPIRE:
			ChatPacket(CHAT_TYPE_INFO, "47");
			return;

		case PERR_DUNGEON:
			ChatPacket(CHAT_TYPE_INFO, "48");
			return;

		case PERR_OBSERVER:
			ChatPacket(CHAT_TYPE_INFO, "49");
			return;

		case PERR_LVBOUNDARY:
			ChatPacket(CHAT_TYPE_INFO, "50");
			return;

		case PERR_LOWLEVEL:
			ChatPacket(CHAT_TYPE_INFO, "51");
			return;

		case PERR_HILEVEL:
			ChatPacket(CHAT_TYPE_INFO, "52");
			return;

		case PERR_ALREADYJOIN:
			ChatPacket(CHAT_TYPE_INFO, "59 %s", pchInvitee->GetName());
			return;

		case PERR_PARTYISFULL:
			ChatPacket(CHAT_TYPE_INFO, "53");
			return;

		default:
			sys_err("Do not process party join error(%d)", errcode);
			return;
	}

	if (m_PartyInviteEventMap.end() != m_PartyInviteEventMap.find(pchInvitee->GetPlayerID()))
		return;

	//
	// EventMap ¿¡ ÀÌº¥Æ® Ãß°¡
	//
	TPartyJoinEventInfo* info = AllocEventInfo<TPartyJoinEventInfo>();

	info->dwGuestPID = pchInvitee->GetPlayerID();
	info->dwLeaderPID = GetPlayerID();

	m_PartyInviteEventMap.insert(EventMap::value_type(pchInvitee->GetPlayerID(), event_create(party_invite_event, info, PASSES_PER_SEC(10))));

	//
	// ÃÊ´ë ¹Þ´Â character ¿¡°Ô ÃÊ´ë ÆÐÅ¶ Àü¼Û
	//

	TPacketGCPartyInvite p;
	p.header = HEADER_GC_PARTY_INVITE;
	p.leader_vid = GetVID();
	pchInvitee->GetDesc()->Packet(&p, sizeof(p));
}

void CHARACTER::PartyInviteAccept(LPCHARACTER pchInvitee)
{
	EventMap::iterator itFind = m_PartyInviteEventMap.find(pchInvitee->GetPlayerID());

	if (itFind == m_PartyInviteEventMap.end())
	{
		sys_log(1, "PartyInviteAccept from not invited character(%s)", pchInvitee->GetName());
		return;
	}

	event_cancel(&itFind->second);
	m_PartyInviteEventMap.erase(itFind);

	if (GetParty() && GetParty()->GetLeaderPID() != GetPlayerID())
	{
		ChatPacket(CHAT_TYPE_INFO, "57");
		return;
	}

	PartyJoinErrCode errcode = IsPartyJoinableMutableCondition(this, pchInvitee);

	switch (errcode)
	{
		case PERR_NONE:
			break;

		case PERR_SERVER:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, "46");
			return;

		case PERR_DUNGEON:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, "60");
			return;

		case PERR_OBSERVER:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, "49");
			return;

		case PERR_LVBOUNDARY:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, "50");
			return;

		case PERR_LOWLEVEL:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, "51");
			return;

		case PERR_HILEVEL:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, "52");
			return;

		case PERR_ALREADYJOIN:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, "61");
			return;

		case PERR_PARTYISFULL:
			ChatPacket(CHAT_TYPE_INFO, "53");
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, "56");
			return;

		default:
			sys_err("ignore party join error(%d)", errcode);
			return;
	}

	//
	// ÆÄÆ¼ °¡ÀÔ Ã³¸®
	//

	if (GetParty())
		pchInvitee->PartyJoin(this);
	else
	{
		LPPARTY pParty = CPartyManager::instance().CreateParty(this);

		pParty->Join(pchInvitee->GetPlayerID());
		pParty->Link(pchInvitee);
		pParty->SendPartyInfoAllToOne(this);
	}
}

void CHARACTER::PartyInviteDeny(DWORD dwPID)
{
	EventMap::iterator itFind = m_PartyInviteEventMap.find(dwPID);

	if (itFind == m_PartyInviteEventMap.end())
	{
		sys_log(1, "PartyInviteDeny to not exist event(inviter PID: %d, invitee PID: %d)", GetPlayerID(), dwPID);
		return;
	}

	event_cancel(&itFind->second);
	m_PartyInviteEventMap.erase(itFind);

	LPCHARACTER pchInvitee = CHARACTER_MANAGER::instance().FindByPID(dwPID);
	if (pchInvitee)
		ChatPacket(CHAT_TYPE_INFO, "62", pchInvitee->GetName());
}

void CHARACTER::PartyJoin(LPCHARACTER pLeader)
{
	pLeader->ChatPacket(CHAT_TYPE_INFO, "63 %s", GetName());
	ChatPacket(CHAT_TYPE_INFO, "64 %s", pLeader->GetName());

	pLeader->GetParty()->Join(GetPlayerID());
	pLeader->GetParty()->Link(this);
}

CHARACTER::PartyJoinErrCode CHARACTER::IsPartyJoinableCondition(const LPCHARACTER pchLeader, const LPCHARACTER pchGuest)
{
	if (pchLeader->GetEmpire() != pchGuest->GetEmpire())
		return PERR_DIFFEMPIRE;

	return IsPartyJoinableMutableCondition(pchLeader, pchGuest);
}

static bool __party_can_join_by_level(LPCHARACTER leader, LPCHARACTER quest)
{
	int	level_limit = 30;
	return (abs(leader->GetLevel() - quest->GetLevel()) <= level_limit);
}

CHARACTER::PartyJoinErrCode CHARACTER::IsPartyJoinableMutableCondition(const LPCHARACTER pchLeader, const LPCHARACTER pchGuest)
{
#ifdef __BL_BATTLE_ROYALE__
	if (pchLeader->IsBattleRoyale() || pchGuest->IsBattleRoyale())
		return PERR_DIFFEMPIRE;
#endif
	if (!CPartyManager::instance().IsEnablePCParty())
		return PERR_SERVER;
	else if (pchLeader->GetDungeon())
		return PERR_DUNGEON;
	else if (pchGuest->IsObserverMode())
		return PERR_OBSERVER;
	else if (false == __party_can_join_by_level(pchLeader, pchGuest))
		return PERR_LVBOUNDARY;
	else if (pchGuest->GetParty())
		return PERR_ALREADYJOIN;
	else if (pchLeader->GetParty())
   	{
	   	if (pchLeader->GetParty()->GetMemberCount() == PARTY_MAX_MEMBER)
			return PERR_PARTYISFULL;
	}

	return PERR_NONE;
}
// END_OF_PARTY_JOIN_BUG_FIX

void CHARACTER::SetDungeon(LPDUNGEON pkDungeon)
{
	if (pkDungeon && m_pkDungeon)
		sys_err("%s is trying to reassigning dungeon (current %p, new party %p)", GetName(), get_pointer(m_pkDungeon), get_pointer(pkDungeon));

	if (m_pkDungeon == pkDungeon) {
		return;
	}

	if (m_pkDungeon)
	{
		if (IsPC())
		{
			if (GetParty())
				m_pkDungeon->DecPartyMember(GetParty(), this);
			else
				m_pkDungeon->DecMember(this);
		}
		else if (IsMonster() || IsStone())
		{
			m_pkDungeon->DecMonster();
		}
	}

	m_pkDungeon = pkDungeon;

	if (pkDungeon)
	{
		sys_log(0, "%s DUNGEON set to %p, PARTY is %p", GetName(), get_pointer(pkDungeon), get_pointer(m_pkParty));

		if (IsPC())
		{
			if (GetParty())
				m_pkDungeon->IncPartyMember(GetParty(), this);
			else
				m_pkDungeon->IncMember(this);
		}
		else if (IsMonster() || IsStone())
		{
			m_pkDungeon->IncMonster();
		}
	}
}

void CHARACTER::SetWarMap(CWarMap * pWarMap)
{
	if (m_pWarMap)
		m_pWarMap->DecMember(this);

	m_pWarMap = pWarMap;

	if (m_pWarMap)
		m_pWarMap->IncMember(this);
}

void CHARACTER::SetWeddingMap(marriage::WeddingMap* pMap)
{
	if (m_pWeddingMap)
		m_pWeddingMap->DecMember(this);

	m_pWeddingMap = pMap;

	if (m_pWeddingMap)
		m_pWeddingMap->IncMember(this);
}

void CHARACTER::SetRegen(LPREGEN pkRegen)
{
    SECTREE * sectree = GetSectree();
 
    if (sectree && sectree->IsAttr(GetX(), GetY(), ATTR_BANPK) && IsMonster())
        Return();
	m_pkRegen = pkRegen;
	if (pkRegen != NULL) {
		regen_id_ = pkRegen->id;
	}
	m_fRegenAngle = GetRotation();
	m_posRegen = GetXYZ();
}

bool CHARACTER::OnIdle()
{
	return false;
}

void CHARACTER::OnMove(bool bIsAttack)
{
	m_dwLastMoveTime = get_dword_time();

	if (bIsAttack)
	{
		m_dwLastAttackTime = m_dwLastMoveTime;

		if (IsAffectFlag(AFF_REVIVE_INVISIBLE))
			RemoveAffect(AFFECT_REVIVE_INVISIBLE);

		if (IsAffectFlag(AFF_EUNHYUNG))
		{
			RemoveAffect(SKILL_EUNHYUNG);
			SetAffectedEunhyung();
		}
		else
		{
			ClearAffectedEunhyung();
		}

		/*if (IsAffectFlag(AFF_JEONSIN))
		  RemoveAffect(SKILL_JEONSINBANGEO);*/
	}

	/*if (IsAffectFlag(AFF_GUNGON))
	  RemoveAffect(SKILL_GUNGON);*/

	// MINING
	mining_cancel();
	// END_OF_MINING
}

void CHARACTER::OnClick(LPCHARACTER pkChrCauser)
{
	if (!pkChrCauser)
	{
		sys_err("OnClick %s by NULL", GetName());
		return;
	}
	else if (pkChrCauser->IsPC() && pkChrCauser->GetExchange())
	{
		return;
	}

    DWORD vid = GetVID();
	sys_log(0, "OnClick %s[vnum: %d vid: %u] by %s", GetName(), GetRaceNum(), vid, pkChrCauser->GetName());

#ifdef ENABLE_QUEEN_NETHIS
	if ((IsNPC()) && (GetRaceNum() == (uint16_t)(SnakeLair::PORTAL_VNUM)) && (SnakeLair::CSnk::instance().IsSnakeMap(pkChrCauser->GetMapIndex())))
	{
		SnakeLair::CSnk::instance().Start(pkChrCauser);
		return;
	}
#endif

	pkChrCauser->SetQuestNPCID(GetVID());

	if (quest::CQuestManager::instance().Click(pkChrCauser->GetPlayerID(), this))
		return;

	if (!IsPC())
	{
		if (!m_triggerOnClick.pFunc)
			return;
		m_triggerOnClick.pFunc(this, pkChrCauser);
	}

}

BYTE CHARACTER::GetGMLevel() const
{
	if (test_server)
		return GM_IMPLEMENTOR;
	return m_pointsInstant.gm_level;
}

void CHARACTER::SetGMLevel()
{
	if (GetDesc())
	{
	    m_pointsInstant.gm_level =  gm_get_level(GetName(), GetDesc()->GetHostName(), GetDesc()->GetAccountTable().login);
	}
	else
	{
	    m_pointsInstant.gm_level = GM_PLAYER;
	}
}

BOOL CHARACTER::IsGM() const
{
	if (m_pointsInstant.gm_level != GM_PLAYER)
		return true;
	if (test_server)
		return true;
	return false;
}

void CHARACTER::SetStone(LPCHARACTER pkChrStone)
{
	m_pkChrStone = pkChrStone;

	if (m_pkChrStone)
	{
		if (pkChrStone->m_set_pkChrSpawnedBy.find(this) == pkChrStone->m_set_pkChrSpawnedBy.end())
			pkChrStone->m_set_pkChrSpawnedBy.insert(this);
	}
}

struct FuncDeadSpawnedByStone
{
	void operator () (LPCHARACTER ch)
	{
		ch->Dead(NULL);
		ch->SetStone(NULL);
	}
};

void CHARACTER::ClearStone()
{
	if (!m_set_pkChrSpawnedBy.empty())
	{
		// ³»°¡ ½ºÆù½ÃÅ² ¸ó½ºÅÍµéÀ» ¸ðµÎ Á×ÀÎ´Ù.
		FuncDeadSpawnedByStone f;
		std::for_each(m_set_pkChrSpawnedBy.begin(), m_set_pkChrSpawnedBy.end(), f);
		m_set_pkChrSpawnedBy.clear();
	}

	if (!m_pkChrStone)
		return;

	m_pkChrStone->m_set_pkChrSpawnedBy.erase(this);
	m_pkChrStone = NULL;
}

void CHARACTER::ClearTarget()
{
	if (m_pkChrTarget)
	{
		m_pkChrTarget->m_set_pkChrTargetedBy.erase(this);
		m_pkChrTarget = NULL;
	}

	TPacketGCTarget p{};

	p.header = HEADER_GC_TARGET;
	p.dwVID = 0;
	p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
	p.iMinHP = 0;
	p.iMaxHP = 0;
#endif
#ifdef ENABLE_HIDE_COSTUME_SYSTEM
	snprintf(p.dwCostumeHideID, sizeof(p.dwCostumeHideID), "");
#endif
	CHARACTER_SET::iterator it = m_set_pkChrTargetedBy.begin();

	while (it != m_set_pkChrTargetedBy.end())
	{
		LPCHARACTER pkChr = *(it++);
		pkChr->m_pkChrTarget = NULL;

		if (!pkChr->GetDesc())
		{
			sys_err("%s %p does not have desc", pkChr->GetName(), get_pointer(pkChr));
			abort();
		}

		pkChr->GetDesc()->Packet(&p, sizeof(TPacketGCTarget));
	}

	m_set_pkChrTargetedBy.clear();
}

void CHARACTER::SetTarget(LPCHARACTER pkChrTarget)
{
	if (m_pkChrTarget == pkChrTarget)
		return;

	// CASTLE
	if (IS_CASTLE_MAP(GetMapIndex()) && !IsGM())
		return;
	// CASTLE

	if (m_pkChrTarget)
		m_pkChrTarget->m_set_pkChrTargetedBy.erase(this);

	m_pkChrTarget = pkChrTarget;

	TPacketGCTarget p;

	p.header = HEADER_GC_TARGET;

	if (m_pkChrTarget)
	{
		m_pkChrTarget->m_set_pkChrTargetedBy.insert(this);

		p.dwVID	= m_pkChrTarget->GetVID();

#ifdef ENABLE_HIDE_COSTUME_SYSTEM
		if (m_pkChrTarget->IsPC())
		{
			char c_hide_show[255+1];
			snprintf(c_hide_show, sizeof(c_hide_show), "%d|%d|%d|%d|", m_pkChrTarget->GetQuestFlag("hide_costume_w.hide_costume_0"),m_pkChrTarget->GetQuestFlag("hide_costume_w.hide_costume_1"),m_pkChrTarget->GetQuestFlag("hide_costume_w.hide_costume_2"),m_pkChrTarget->GetQuestFlag("hide_costume_w.hide_costume_3"));
			memcpy (p.dwCostumeHideID, c_hide_show, sizeof(p.dwCostumeHideID));
		}
#endif

#ifdef __VIEW_TARGET_PLAYER_HP__
		if ((m_pkChrTarget->GetMaxHP() <= 0))
		{
			p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
			p.iMinHP = 0;
			p.iMaxHP = 0;
#endif
		}
		else if (m_pkChrTarget->IsPC() && !m_pkChrTarget->IsPolymorphed())
		{
			p.bHPPercent = MINMAX(0, (m_pkChrTarget->GetHP() * 100) / m_pkChrTarget->GetMaxHP(), 100);
#ifdef __VIEW_TARGET_DECIMAL_HP__
			p.iMinHP = m_pkChrTarget->GetHP();
			p.iMaxHP = m_pkChrTarget->GetMaxHP();
#endif
		}
#else
		if ((m_pkChrTarget->IsPC() && !m_pkChrTarget->IsPolymorphed()) || (m_pkChrTarget->GetMaxHP() <= 0))
			p.bHPPercent = 0;
#endif
		else
		{
			if (m_pkChrTarget->GetRaceNum() == 20101 ||
					m_pkChrTarget->GetRaceNum() == 20102 ||
					m_pkChrTarget->GetRaceNum() == 20103 ||
					m_pkChrTarget->GetRaceNum() == 20104 ||
					m_pkChrTarget->GetRaceNum() == 20105 ||
					m_pkChrTarget->GetRaceNum() == 20106 ||
					m_pkChrTarget->GetRaceNum() == 20107 ||
					m_pkChrTarget->GetRaceNum() == 20108 ||
					m_pkChrTarget->GetRaceNum() == 20109)
			{
				LPCHARACTER owner = m_pkChrTarget->GetVictim();

				if (owner)
				{
					int iHorseHealth = owner->GetHorseHealth();
					int iHorseMaxHealth = owner->GetHorseMaxHealth();
#ifdef __VIEW_TARGET_DECIMAL_HP__
					if (iHorseMaxHealth)
					{
						p.bHPPercent = MINMAX(0,  iHorseHealth * 100 / iHorseMaxHealth, 100);
						p.iMinHP = 100;
						p.iMaxHP = 100;
					}
					else
					{
						p.bHPPercent = 100;
						p.iMinHP = 100;
						p.iMaxHP = 100;
					}
				}
				else
				{
					p.bHPPercent = 100;
					p.iMinHP = 100;
						p.iMaxHP = 100;
				}
			}
			else
			{
				if (m_pkChrTarget->GetMaxHP() <= 0) // @fixme136
				{
					p.bHPPercent = 0;
					p.iMinHP = 0;
					p.iMaxHP = 0;
				}
				else
				{
					p.bHPPercent = int(float(m_pkChrTarget->GetHP()) * 100.0f) / float(m_pkChrTarget->GetMaxHP()) > 100.0f ? 100.0f : (float(m_pkChrTarget->GetHP()) * 100.0f) / float(m_pkChrTarget->GetMaxHP());
					p.iMinHP = m_pkChrTarget->GetHP();
					p.iMaxHP = m_pkChrTarget->GetMaxHP();
				}
			}
		}
	}
	else
	{
		p.dwVID = 0;
		p.bHPPercent = 0;
		p.iMinHP = 0;
		p.iMaxHP = 0;
	}
#else
					if (iHorseMaxHealth)
						p.bHPPercent = MINMAX(0,  iHorseHealth * 100 / iHorseMaxHealth, 100);

					else
						p.bHPPercent = 100;
				}
				else
					p.bHPPercent = 100;
			}
			else
			{
				if (m_pkChrTarget->GetMaxHP() <= 0) // @fixme136
					p.bHPPercent = 0;
				else
					p.bHPPercent = MINMAX(0, (m_pkChrTarget->GetHP() * 100) / m_pkChrTarget->GetMaxHP(), 100);
			}
		}
	}
	else
	{
		p.dwVID = 0;
		p.bHPPercent = 0;
	}
#endif

#ifdef ELEMENT_TARGET
	const int ELEMENT_BASE = 11;
	DWORD curElementBase = ELEMENT_BASE;
	DWORD raceFlag;
	if (m_pkChrTarget && m_pkChrTarget->IsMonster() && (raceFlag = m_pkChrTarget->GetMobTable().dwRaceFlag) >= RACE_FLAG_ATT_ELEC)
	{
		for (int i = RACE_FLAG_ATT_ELEC; i <= RACE_FLAG_ATT_DARK; i *= 2)
		{
			curElementBase++;
			int diff = raceFlag - i;
			if (abs(diff) <= 1024)
				break;
		}
		p.bElement = curElementBase - ELEMENT_BASE;
	}
	else if(m_pkChrTarget && m_pkChrTarget->IsPC())
	{
		LPITEM pkElement = m_pkChrTarget->GetWear(WEAR_PENDANT);
		if (!pkElement) // NONE
			p.bElement = 0;
		else if (pkElement && (pkElement->GetVnum() >= 9600 && pkElement->GetVnum() <= 9800))	
			p.bElement = 2;
		else if (pkElement && (pkElement->GetVnum() >= 9830 && pkElement->GetVnum() <= 10030))	
			p.bElement = 3;
		else if (pkElement && (pkElement->GetVnum() >= 10060 && pkElement->GetVnum() <= 10260))
			p.bElement = 5;
		else if (pkElement && (pkElement->GetVnum() >= 10290 && pkElement->GetVnum() <= 10490))	
			p.bElement = 6;
		else if (pkElement && (pkElement->GetVnum() >= 10520 && pkElement->GetVnum() <= 10720))	
			p.bElement = 4;
		else if (pkElement && (pkElement->GetVnum() >= 10750 && pkElement->GetVnum() <= 10950))
			p.bElement = 1;
	}
	else
	{
		p.bElement = 0;
	}

#endif

	GetDesc()->Packet(&p, sizeof(TPacketGCTarget));
}

void CHARACTER::BroadcastTargetPacket()
{
	if (m_set_pkChrTargetedBy.empty())
		return;

	TPacketGCTarget p;

	p.header = HEADER_GC_TARGET;
	p.dwVID = GetVID();


	if (GetMaxHP() <= 0)
	{
		p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
		p.iMinHP = 0;
		p.iMaxHP = 0;
#endif
	}
	else
	{
#ifdef __VIEW_TARGET_PLAYER_HP__
		p.bHPPercent = MINMAX(0, (GetHP() * 100) / GetMaxHP(), 100);
#ifdef __VIEW_TARGET_DECIMAL_HP__
		p.iMinHP = GetHP();
		p.iMaxHP = GetMaxHP();
#endif
#else
		if (IsPC())
		{
			p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
			p.iMinHP = 0;
			p.iMaxHP = 0;
#endif
		}
		else
		{
			p.bHPPercent = MINMAX(0, (GetHP() * 100) / GetMaxHP(), 100);
#ifdef __VIEW_TARGET_DECIMAL_HP__
			p.iMinHP = GetHP();
			p.iMaxHP = GetMaxHP();
#endif
		}
#endif
	}

	/*
	if (IsPC())
		p.bHPPercent = 0;
	else if (GetMaxHP() <= 0) // @fixme136
		p.bHPPercent = 0;
	else
		p.bHPPercent = MINMAX(0, (GetHP() * 100) / GetMaxHP(), 100);*/

#ifdef ENABLE_HIDE_COSTUME_SYSTEM
		if (IsPC()){
			char c_hide_show[255+1];
			snprintf(c_hide_show, sizeof(c_hide_show), "%d|%d|%d|%d|", GetQuestFlag("hide_costume_w.hide_costume_0"),GetQuestFlag("hide_costume_w.hide_costume_1"),GetQuestFlag("hide_costume_w.hide_costume_2"),GetQuestFlag("hide_costume_w.hide_costume_3"));
			memcpy (p.dwCostumeHideID, c_hide_show, sizeof(p.dwCostumeHideID));
		}
#endif

	CHARACTER_SET::iterator it = m_set_pkChrTargetedBy.begin();

	while (it != m_set_pkChrTargetedBy.end())
	{
		LPCHARACTER pkChr = *it++;

		if (!pkChr->GetDesc())
		{
			sys_err("%s %p does not have desc", pkChr->GetName(), get_pointer(pkChr));
			abort();
		}

		pkChr->GetDesc()->Packet(&p, sizeof(TPacketGCTarget));
	}
}

void CHARACTER::CheckTarget()
{
	if (!m_pkChrTarget)
		return;

	if (DISTANCE_APPROX(GetX() - m_pkChrTarget->GetX(), GetY() - m_pkChrTarget->GetY()) >= 4800)
		SetTarget(NULL);
}

void CHARACTER::SetWarpLocation(long lMapIndex, long x, long y)
{
	m_posWarp.x = x * 100;
	m_posWarp.y = y * 100;
	m_lWarpMapIndex = lMapIndex;
}

void CHARACTER::SaveExitLocation()
{
	m_posExit = GetXYZ();
	m_lExitMapIndex = GetMapIndex();
}

void CHARACTER::ExitToSavedLocation()
{
	sys_log (0, "ExitToSavedLocation");
	WarpSet(m_posWarp.x, m_posWarp.y, m_lWarpMapIndex);

	m_posExit.x = m_posExit.y = m_posExit.z = 0;
	m_lExitMapIndex = 0;
}

// fixme
// Áö±Ý±îÁø privateMapIndex °¡ ÇöÀç ¸Ê ÀÎµ¦½º¿Í °°ÀºÁö Ã¼Å© ÇÏ´Â °ÍÀ» ¿ÜºÎ¿¡¼­ ÇÏ°í,
// ´Ù¸£¸é warpsetÀ» ºÒ·¶´Âµ¥
// ÀÌ¸¦ warpset ¾ÈÀ¸·Î ³ÖÀÚ.
bool CHARACTER::WarpSet(long x, long y, long lPrivateMapIndex, WORD port)
{
	if (!IsPC())
		return false;

	long lAddr;
	long lMapIndex;
	WORD wPort;

	if (!CMapLocation::instance().Get(x, y, lMapIndex, lAddr, wPort))
	{
		sys_err("cannot find map location index %d x %d y %d name %s", lMapIndex, x, y, GetName());
		return false;
	}

	if (lPrivateMapIndex >= 10000)
	{
		if (lPrivateMapIndex / 10000 != lMapIndex)
		{
			sys_err("Invalid map index %d, must be child of %d", lPrivateMapIndex, lMapIndex);
			return false;
		}

		lMapIndex = lPrivateMapIndex;
	}

	Stop();
	Save();

	if (GetSectree())
	{
		GetSectree()->RemoveEntity(this);
		ViewCleanup();

		EncodeRemovePacket(this);
	}

	m_lWarpMapIndex = lMapIndex;
	m_posWarp.x = x;
	m_posWarp.y = y;

	sys_log(0, "WarpSet %s %d %d current map %d target map %d", GetName(), x, y, GetMapIndex(), lMapIndex);

	TPacketGCWarp p;

	p.bHeader	= HEADER_GC_WARP;
	p.lX	= x;
	p.lY	= y;
	p.lAddr	= lAddr;
	p.wPort = port?port:wPort;

#ifdef ENABLE_SWITCHBOT
	CSwitchbotManager::Instance().SetIsWarping(GetPlayerID(), true);

	if (p.wPort != mother_port)
	{
		CSwitchbotManager::Instance().P2PSendSwitchbot(GetPlayerID(), p.wPort);
	}
#endif

	GetDesc()->Packet(&p, sizeof(TPacketGCWarp));

	char buf[256];
	snprintf(buf, sizeof(buf), "%s MapIdx %ld DestMapIdx%ld DestX%ld DestY%ld Empire%d", GetName(), GetMapIndex(), lPrivateMapIndex, x, y, GetEmpire());
	LogManager::instance().CharLog(this, 0, "WARP", buf);
	
#ifdef ENABLE_VOTE_COINS_WHILE_PLAYING
	SetQuestFlag("vote_coins_event.warp_cache", get_global_time());
#endif

	return true;
}

#define ENABLE_GOHOME_IF_MAP_NOT_ALLOWED
void CHARACTER::WarpEnd()
{
	if (test_server)
		sys_log(0, "WarpEnd %s", GetName());

	if (m_posWarp.x == 0 && m_posWarp.y == 0)
		return;

	int index = m_lWarpMapIndex;

	if (index > 10000)
		index /= 10000;

	if (!map_allow_find(index))
	{
		// ÀÌ °÷À¸·Î ¿öÇÁÇÒ ¼ö ¾øÀ¸¹Ç·Î ¿öÇÁÇÏ±â Àü ÁÂÇ¥·Î µÇµ¹¸®ÀÚ.
		sys_err("location %d %d not allowed to login this server", m_posWarp.x, m_posWarp.y);
#ifdef ENABLE_GOHOME_IF_MAP_NOT_ALLOWED
		GoHome();
#else
		GetDesc()->SetPhase(PHASE_CLOSE);
#endif
		return;
	}

	sys_log(0, "WarpEnd %s %d %u %u", GetName(), m_lWarpMapIndex, m_posWarp.x, m_posWarp.y);

	Show(m_lWarpMapIndex, m_posWarp.x, m_posWarp.y, 0);
	Stop();

	m_lWarpMapIndex = 0;
	m_posWarp.x = m_posWarp.y = m_posWarp.z = 0;

	{
		// P2P Login
		TPacketGGLogin p;
		p.bHeader = HEADER_GG_LOGIN;
		strlcpy(p.szName, GetName(), sizeof(p.szName));
		p.dwPID = GetPlayerID();
		p.bEmpire = GetEmpire();
		p.lMapIndex = SECTREE_MANAGER::instance().GetMapIndex(GetX(), GetY());
		p.bChannel = g_bChannel;
		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGLogin));
	}
}

bool CHARACTER::Return()
{
	if (!IsNPC())
		return false;

	int x, y;
	/*
	   float fDist = DISTANCE_SQRT(m_pkMobData->m_posLastAttacked.x - GetX(), m_pkMobData->m_posLastAttacked.y - GetY());
	   float fx, fy;
	   GetDeltaByDegree(GetRotation(), fDist, &fx, &fy);
	   x = GetX() + (int) fx;
	   y = GetY() + (int) fy;
	 */
	SetVictim(NULL);

	x = m_pkMobInst->m_posLastAttacked.x;
	y = m_pkMobInst->m_posLastAttacked.y;

	SetRotationToXY(x, y);

	if (!Goto(x, y))
		return false;

	SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

	if (test_server)
		sys_log(0, "%s %p Æ÷±âÇÏ°í µ¹¾Æ°¡ÀÚ! %d %d", GetName(), this, x, y);

	if (GetParty())
		GetParty()->SendMessage(this, PM_RETURN, x, y);

	return true;
}

bool CHARACTER::Follow(LPCHARACTER pkChr, float fMinDistance)
{
	if (IsPC()
#ifdef __FAKE_PLAYER__
		|| IsFakePlayer()	
#endif		
	)
	{
		sys_err("CHARACTER::Follow : PC cannot use this method", GetName());
		return false;
	}

	// TRENT_MONSTER
	if (IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE))
	{
		if (pkChr->IsPC()) // ÂÑ¾Æ°¡´Â »ó´ë°¡ PCÀÏ ¶§
		{
			// If i'm in a party. I must obey party leader's AI.
			if (!GetParty() || !GetParty()->GetLeader() || GetParty()->GetLeader() == this)
			{
				if (get_dword_time() - m_pkMobInst->m_dwLastAttackedTime >= 15000) // ¸¶Áö¸·À¸·Î °ø°Ý¹ÞÀºÁö 15ÃÊ°¡ Áö³µ°í
				{
					// ¸¶Áö¸· ¸ÂÀº °÷À¸·Î ºÎÅÍ 50¹ÌÅÍ ÀÌ»ó Â÷ÀÌ³ª¸é Æ÷±âÇÏ°í µ¹¾Æ°£´Ù.
					if (m_pkMobData->m_table.wAttackRange < DISTANCE_APPROX(pkChr->GetX() - GetX(), pkChr->GetY() - GetY()))
						if (Return())
							return true;
				}
			}
		}
		return false;
	}
	// END_OF_TRENT_MONSTER

	long x = pkChr->GetX();
	long y = pkChr->GetY();

	if (pkChr->IsPC()) // ÂÑ¾Æ°¡´Â »ó´ë°¡ PCÀÏ ¶§
	{
		// If i'm in a party. I must obey party leader's AI.
		if (!GetParty() || !GetParty()->GetLeader() || GetParty()->GetLeader() == this)
		{
			if (get_dword_time() - m_pkMobInst->m_dwLastAttackedTime >= 15000) // ¸¶Áö¸·À¸·Î °ø°Ý¹ÞÀºÁö 15ÃÊ°¡ Áö³µ°í
			{
				// ¸¶Áö¸· ¸ÂÀº °÷À¸·Î ºÎÅÍ 50¹ÌÅÍ ÀÌ»ó Â÷ÀÌ³ª¸é Æ÷±âÇÏ°í µ¹¾Æ°£´Ù.
				if (5000 < DISTANCE_APPROX(m_pkMobInst->m_posLastAttacked.x - GetX(), m_pkMobInst->m_posLastAttacked.y - GetY()))
					if (Return())
						return true;
			}
		}
	}

	if (IsGuardNPC())
	{
		if (5000 < DISTANCE_APPROX(m_pkMobInst->m_posLastAttacked.x - GetX(), m_pkMobInst->m_posLastAttacked.y - GetY()))
			if (Return())
				return true;
	}

#ifdef ENABLE_MOUNT_SYSTEM
	if (pkChr->IsState(pkChr->m_stateMove) &&
		GetMobBattleType() != BATTLE_TYPE_RANGE &&
		GetMobBattleType() != BATTLE_TYPE_MAGIC &&
		false == IsPet() && false == IsMountSystem() && false == IsNewPet() && false == IsSupport())
#else
	if (pkChr->IsState(pkChr->m_stateMove) && 
		GetMobBattleType() != BATTLE_TYPE_RANGE && 
		GetMobBattleType() != BATTLE_TYPE_MAGIC &&
		false == IsPet())
#endif
	{
		// ´ë»óÀÌ ÀÌµ¿ÁßÀÌ¸é ¿¹Ãø ÀÌµ¿À» ÇÑ´Ù
		// ³ª¿Í »ó´ë¹æÀÇ ¼ÓµµÂ÷¿Í °Å¸®·ÎºÎÅÍ ¸¸³¯ ½Ã°£À» ¿¹»óÇÑ ÈÄ
		// »ó´ë¹æÀÌ ±× ½Ã°£±îÁö Á÷¼±À¸·Î ÀÌµ¿ÇÑ´Ù°í °¡Á¤ÇÏ¿© °Å±â·Î ÀÌµ¿ÇÑ´Ù.
		float rot = pkChr->GetRotation();
		float rot_delta = GetDegreeDelta(rot, GetDegreeFromPositionXY(GetX(), GetY(), pkChr->GetX(), pkChr->GetY()));

		float yourSpeed = pkChr->GetMoveSpeed();
		float mySpeed = GetMoveSpeed();

		float fDist = DISTANCE_SQRT(x - GetX(), y - GetY());
		float fFollowSpeed = mySpeed - yourSpeed * cos(rot_delta * M_PI / 180);

		if (fFollowSpeed >= 0.1f)
		{
			float fMeetTime = fDist / fFollowSpeed;
			float fYourMoveEstimateX, fYourMoveEstimateY;

			if( fMeetTime * yourSpeed <= 100000.0f )
			{
				GetDeltaByDegree(pkChr->GetRotation(), fMeetTime * yourSpeed, &fYourMoveEstimateX, &fYourMoveEstimateY);

				x += (long) fYourMoveEstimateX;
				y += (long) fYourMoveEstimateY;

				float fDistNew = sqrt(((double)x - GetX())*(x-GetX())+((double)y - GetY())*(y-GetY()));
				if (fDist < fDistNew)
				{
					x = (long)(GetX() + (x - GetX()) * fDist / fDistNew);
					y = (long)(GetY() + (y - GetY()) * fDist / fDistNew);
				}
			}
		}
	}

	// °¡·Á´Â À§Ä¡¸¦ ¹Ù¶óºÁ¾ß ÇÑ´Ù.
	SetRotationToXY(x, y);

	float fDist = DISTANCE_SQRT(x - GetX(), y - GetY());

	if (fDist <= fMinDistance)
		return false;

	float fx, fy;

	if (IsChangeAttackPosition(pkChr) && GetMobRank() < MOB_RANK_BOSS)
	{
		// »ó´ë¹æ ÁÖº¯ ·£´ýÇÑ °÷À¸·Î ÀÌµ¿
		SetChangeAttackPositionTime();

		int retry = 16;
		int dx, dy;
		int rot = (int) GetDegreeFromPositionXY(x, y, GetX(), GetY());

		while (--retry)
		{
			if (fDist < 500.0f)
				GetDeltaByDegree((rot + number(-90, 90) + number(-90, 90)) % 360, fMinDistance, &fx, &fy);
			else
				GetDeltaByDegree(number(0, 359), fMinDistance, &fx, &fy);

			dx = x + (int) fx;
			dy = y + (int) fy;

			LPSECTREE tree = SECTREE_MANAGER::instance().Get(GetMapIndex(), dx, dy);

			if (NULL == tree)
				break;

			if (0 == (tree->GetAttribute(dx, dy) & (ATTR_BLOCK | ATTR_OBJECT)))
				break;
		}

		//sys_log(0, "±ÙÃ³ ¾îµò°¡·Î ÀÌµ¿ %s retry %d", GetName(), retry);
		if (!Goto(dx, dy))
			return false;
	}
	else
	{
		// Á÷¼± µû¶ó°¡±â
		float fDistToGo = fDist - fMinDistance;
		GetDeltaByDegree(GetRotation(), fDistToGo, &fx, &fy);

		//sys_log(0, "Á÷¼±À¸·Î ÀÌµ¿ %s", GetName());
		if (!Goto(GetX() + (int) fx, GetY() + (int) fy))
			return false;
	}

	SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	//MonsterLog("ÂÑ¾Æ°¡±â; %s", pkChr->GetName());
	return true;
}

float CHARACTER::GetDistanceFromSafeboxOpen() const
{
	return DISTANCE_APPROX(GetX() - m_posSafeboxOpen.x, GetY() - m_posSafeboxOpen.y);
}

void CHARACTER::SetSafeboxOpenPosition()
{
	m_posSafeboxOpen = GetXYZ();
}

CSafebox * CHARACTER::GetSafebox() const
{
	return m_pkSafebox;
}

void CHARACTER::ReqSafeboxLoad(const char* pszPassword)
{
	if (!*pszPassword || strlen(pszPassword) > SAFEBOX_PASSWORD_MAX_LEN)
	{
		ChatPacket(CHAT_TYPE_INFO, "65");
		return;
	}
	else if (m_pkSafebox)
	{
		ChatPacket(CHAT_TYPE_INFO, "66");
		return;
	}

	int iPulse = thecore_pulse();

	if (iPulse - GetSafeboxLoadTime()  < PASSES_PER_SEC(10))// need timer.
	{
		ChatPacket(CHAT_TYPE_INFO, "67");
		return;
	}
	//else if (GetDistanceFromSafeboxOpen() > 1000)
	//{
	//	ChatPacket(CHAT_TYPE_INFO, "68");
	//	return;
	//}
	else if (m_bOpeningSafebox)
	{
		sys_log(0, "Overlapped safebox load request from %s", GetName());
		return;
	}

	SetSafeboxLoadTime();
	m_bOpeningSafebox = true;

	TSafeboxLoadPacket p;
	p.dwID = GetDesc()->GetAccountTable().id;
	strlcpy(p.szLogin, GetDesc()->GetAccountTable().login, sizeof(p.szLogin));
	strlcpy(p.szPassword, pszPassword, sizeof(p.szPassword));

	db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_LOAD, GetDesc()->GetHandle(), &p, sizeof(p));
}

void CHARACTER::LoadSafebox(int iSize, DWORD dwGold, int iItemCount, TPlayerItem * pItems)
{
	bool bLoaded = false;

	//PREVENT_TRADE_WINDOW
	SetOpenSafebox(true);
	//END_PREVENT_TRADE_WINDOW

	if (m_pkSafebox)
		bLoaded = true;

	if (!m_pkSafebox)
		m_pkSafebox = M2_NEW CSafebox(this, iSize, dwGold);
	else
		m_pkSafebox->ChangeSize(iSize);

	m_iSafeboxSize = iSize;

	TPacketCGSafeboxSize p;

	p.bHeader = HEADER_GC_SAFEBOX_SIZE;
	p.bSize = iSize;

	GetDesc()->Packet(&p, sizeof(TPacketCGSafeboxSize));

	if (!bLoaded)
	{
		for (int i = 0; i < iItemCount; ++i, ++pItems)
		{
			if (!m_pkSafebox->IsValidPosition(pItems->pos))
				continue;

#ifdef __SOULBINDING_SYSTEM__
			LPITEM item = ITEM_MANAGER::instance().CreateItem(pItems->vnum, pItems->count, pItems->id, false, -1, false, pItems->bind);
#else
			LPITEM item = ITEM_MANAGER::instance().CreateItem(pItems->vnum, pItems->count, pItems->id);
#endif

			if (!item)
			{
				sys_err("cannot create item vnum %d id %u (name: %s)", pItems->vnum, pItems->id, GetName());
				continue;
			}

			item->SetSkipSave(true);
			item->SetSockets(pItems->alSockets);
			item->SetAttributes(pItems->aAttr);
#ifdef ENABLE_YOHARA_SYSTEM
			item->SetRandomAttrs(pItems->aApplyRandom);
#endif
#ifdef ENABLE_NEW_NAME_ITEM
			item->SetNewName(pItems->name);
#endif
			if (!m_pkSafebox->Add(pItems->pos, item))
			{
				M2_DESTROY_ITEM(item);
			}
			else
				item->SetSkipSave(false);
		}
	}
}

void CHARACTER::ChangeSafeboxSize(BYTE bSize)
{
	//if (!m_pkSafebox)
	//return;

	TPacketCGSafeboxSize p;

	p.bHeader = HEADER_GC_SAFEBOX_SIZE;
	p.bSize = bSize;

	GetDesc()->Packet(&p, sizeof(TPacketCGSafeboxSize));

	if (m_pkSafebox)
		m_pkSafebox->ChangeSize(bSize);

	m_iSafeboxSize = bSize;
}

void CHARACTER::CloseSafebox()
{
	if (!m_pkSafebox)
		return;

	//PREVENT_TRADE_WINDOW
	SetOpenSafebox(false);
	//END_PREVENT_TRADE_WINDOW

	m_pkSafebox->Save();

	M2_DELETE(m_pkSafebox);
	m_pkSafebox = NULL;

	ChatPacket(CHAT_TYPE_COMMAND, "CloseSafebox");

	SetSafeboxLoadTime();
	m_bOpeningSafebox = false;

	Save();
}

CSafebox * CHARACTER::GetMall() const
{
	return m_pkMall;
}

void CHARACTER::LoadMall(int iItemCount, TPlayerItem * pItems)
{
	bool bLoaded = false;

	//if (m_pkMall)//i think here...
	//	bLoaded = true;

	if (!m_pkMall)
		m_pkMall = M2_NEW CSafebox(this, 3 * SAFEBOX_PAGE_SIZE, 0);
	else
		m_pkMall->ChangeSize(3 * SAFEBOX_PAGE_SIZE);

	m_pkMall->SetWindowMode(MALL);

	TPacketCGSafeboxSize p;

	p.bHeader = HEADER_GC_MALL_OPEN;
	p.bSize = 3 * SAFEBOX_PAGE_SIZE;

	GetDesc()->Packet(&p, sizeof(TPacketCGSafeboxSize));

	if (!bLoaded)
	{
		for (int i = 0; i < iItemCount; ++i, ++pItems)
		{
			if (!m_pkMall->IsValidPosition(pItems->pos))
				continue;

#ifdef __SOULBINDING_SYSTEM__
			LPITEM item = ITEM_MANAGER::instance().CreateItem(pItems->vnum, pItems->count, pItems->id, false, -1, false, pItems->bind);
#else
			LPITEM item = ITEM_MANAGER::instance().CreateItem(pItems->vnum, pItems->count, pItems->id);
#endif

			if (!item)
			{
				sys_err("cannot create item vnum %d id %u (name: %s)", pItems->vnum, pItems->id, GetName());
				continue;
			}

			item->SetSkipSave(true);
			item->SetSockets(pItems->alSockets);
			item->SetAttributes(pItems->aAttr);
#ifdef ENABLE_YOHARA_SYSTEM
			item->SetRandomAttrs(pItems->aApplyRandom);
#endif


			if (!m_pkMall->Add(pItems->pos, item))
				M2_DESTROY_ITEM(item);
			else
				item->SetSkipSave(false);
		}
	}
}

void CHARACTER::CloseMall()
{
	if (!m_pkMall)
		return;

	m_pkMall->Save();

	M2_DELETE(m_pkMall);
	m_pkMall = NULL;

	ChatPacket(CHAT_TYPE_COMMAND, "CloseMall");
}

bool CHARACTER::BuildUpdatePartyPacket(TPacketGCPartyUpdate & out)
{
	if (!GetParty())
		return false;

	memset(&out, 0, sizeof(out));

	out.header		= HEADER_GC_PARTY_UPDATE;
	out.pid		= GetPlayerID();
	if (GetMaxHP() <= 0) // @fixme136
		out.percent_hp	= 0;
	else
		out.percent_hp	= MINMAX(0, GetHP() * 100 / GetMaxHP(), 100);
	out.role		= GetParty()->GetRole(GetPlayerID());

	sys_log(1, "PARTY %s role is %d", GetName(), out.role);

	LPCHARACTER l = GetParty()->GetLeaderCharacter();

	if (l && DISTANCE_APPROX(GetX() - l->GetX(), GetY() - l->GetY()) < PARTY_DEFAULT_RANGE)
	{
		out.affects[0] = GetParty()->GetPartyBonusExpPercent();
		out.affects[1] = GetPoint(POINT_PARTY_ATTACKER_BONUS);
		out.affects[2] = GetPoint(POINT_PARTY_TANKER_BONUS);
		out.affects[3] = GetPoint(POINT_PARTY_BUFFER_BONUS);
		out.affects[4] = GetPoint(POINT_PARTY_SKILL_MASTER_BONUS);
		out.affects[5] = GetPoint(POINT_PARTY_HASTE_BONUS);
		out.affects[6] = GetPoint(POINT_PARTY_DEFENDER_BONUS);
	}

	return true;
}

int CHARACTER::GetLeadershipSkillLevel() const
{
	return GetSkillLevel(SKILL_LEADERSHIP);
}

void CHARACTER::QuerySafeboxSize()
{
	if (m_iSafeboxSize == -1)
	{
		DBManager::instance().ReturnQuery(QID_SAFEBOX_SIZE,
				GetPlayerID(),
				NULL,
				"SELECT size FROM safebox%s WHERE account_id = %u",
				get_table_postfix(),
				GetDesc()->GetAccountTable().id);
	}
}

void CHARACTER::SetSafeboxSize(int iSize)
{
	sys_log(1, "SetSafeboxSize: %s %d", GetName(), iSize);
	m_iSafeboxSize = iSize;
	DBManager::instance().Query("UPDATE safebox%s SET size = %d WHERE account_id = %u", get_table_postfix(), iSize / SAFEBOX_PAGE_SIZE, GetDesc()->GetAccountTable().id);
}

int CHARACTER::GetSafeboxSize() const
{
	return m_iSafeboxSize;
}

void CHARACTER::SetNowWalking(bool bWalkFlag)
{
	//if (m_bNowWalking != bWalkFlag || IsNPC())
	if (m_bNowWalking != bWalkFlag)
	{
		if (bWalkFlag)
		{
			m_bNowWalking = true;
			m_dwWalkStartTime = get_dword_time();
		}
		else
		{
			m_bNowWalking = false;
		}

		//if (m_bNowWalking)
		{
			TPacketGCWalkMode p;
			p.vid = GetVID();
			p.header = HEADER_GC_WALK_MODE;
			p.mode = m_bNowWalking ? WALKMODE_WALK : WALKMODE_RUN;

			PacketView(&p, sizeof(p));
		}

		if (IsNPC())
		{
			if (m_bNowWalking)
				MonsterLog("°È´Â´Ù");
			else
				MonsterLog("¶Ú´Ù");
		}

		//sys_log(0, "%s is now %s", GetName(), m_bNowWalking?"walking.":"running.");
	}
}

void CHARACTER::StartStaminaConsume()
{
	if (m_bStaminaConsume)
		return;
	PointChange(POINT_STAMINA, 0);
	m_bStaminaConsume = true;
	//ChatPacket(CHAT_TYPE_COMMAND, "StartStaminaConsume %d %d", STAMINA_PER_STEP * passes_per_sec, GetStamina());
	if (IsStaminaHalfConsume())
		ChatPacket(CHAT_TYPE_COMMAND, "StartStaminaConsume %d %d", STAMINA_PER_STEP * passes_per_sec / 2, GetStamina());
	else
		ChatPacket(CHAT_TYPE_COMMAND, "StartStaminaConsume %d %d", STAMINA_PER_STEP * passes_per_sec, GetStamina());
}

void CHARACTER::StopStaminaConsume()
{
	if (!m_bStaminaConsume)
		return;
	PointChange(POINT_STAMINA, 0);
	m_bStaminaConsume = false;
	ChatPacket(CHAT_TYPE_COMMAND, "StopStaminaConsume %d", GetStamina());
}

bool CHARACTER::IsStaminaConsume() const
{
	return m_bStaminaConsume;
}

bool CHARACTER::IsStaminaHalfConsume() const
{
	return IsEquipUniqueItem(UNIQUE_ITEM_HALF_STAMINA);
}

void CHARACTER::ResetStopTime()
{
	m_dwStopTime = get_dword_time();
}

DWORD CHARACTER::GetStopTime() const
{
	return m_dwStopTime;
}

void CHARACTER::ResetPoint(int iLv)
{
	BYTE bJob = GetJob();

	PointChange(POINT_LEVEL, iLv - GetLevel());

	SetRealPoint(POINT_ST, JobInitialPoints[bJob].st);
	SetPoint(POINT_ST, GetRealPoint(POINT_ST));

	SetRealPoint(POINT_HT, JobInitialPoints[bJob].ht);
	SetPoint(POINT_HT, GetRealPoint(POINT_HT));

	SetRealPoint(POINT_DX, JobInitialPoints[bJob].dx);
	SetPoint(POINT_DX, GetRealPoint(POINT_DX));

	SetRealPoint(POINT_IQ, JobInitialPoints[bJob].iq);
	SetPoint(POINT_IQ, GetRealPoint(POINT_IQ));

	SetRandomHP((iLv - 1) * number(JobInitialPoints[GetJob()].hp_per_lv_begin, JobInitialPoints[GetJob()].hp_per_lv_end));
	SetRandomSP((iLv - 1) * number(JobInitialPoints[GetJob()].sp_per_lv_begin, JobInitialPoints[GetJob()].sp_per_lv_end));

	// @fixme104
	PointChange(POINT_STAT, (MINMAX(1, iLv, g_iStatusPointGetLevelLimit) * 3) + GetPoint(POINT_LEVEL_STEP) - GetPoint(POINT_STAT));

	ComputePoints();

	// È¸º¹
	PointChange(POINT_HP, GetMaxHP() - GetHP());
	PointChange(POINT_SP, GetMaxSP() - GetSP());

	PointsPacket();

	LogManager::instance().CharLog(this, 0, "RESET_POINT", "");
}

#ifdef ENABLE_YOHARA_SYSTEM
void CHARACTER::ResetConquerorPoint(int iLv)
{
	PointChange(POINT_CONQUEROR_LEVEL, iLv - GetConquerorLevel());

	SetRealPoint(POINT_SUNGMA_STR, 0);
	SetPoint(POINT_SUNGMA_STR, GetRealPoint(POINT_SUNGMA_STR));

	SetRealPoint(POINT_SUNGMA_HP, 0);
	SetPoint(POINT_SUNGMA_HP, GetRealPoint(POINT_SUNGMA_HP));

	SetRealPoint(POINT_SUNGMA_MOVE, 0);
	SetPoint(POINT_SUNGMA_MOVE, GetRealPoint(POINT_SUNGMA_MOVE));

	SetRealPoint(POINT_SUNGMA_IMMUNE, 0);
	SetPoint(POINT_SUNGMA_IMMUNE, GetRealPoint(POINT_SUNGMA_IMMUNE));

	// @fixme104
	if (iLv > 1)
		PointChange(POINT_CONQUEROR_POINT, (MINMAX(0, iLv - 1, g_iConquerorPointGetLevelLimit) * 3) + GetPoint(POINT_CONQUEROR_LEVEL_STEP) - GetPoint(POINT_CONQUEROR_POINT));
	else
		PointChange(POINT_CONQUEROR_POINT, -GetPoint(POINT_CONQUEROR_POINT));

	ComputePoints();
	PointsPacket();

	LogManager::Instance().CharLog(this, 0, "RESET_CONQUEROR_POINT", "");
}
#endif

bool CHARACTER::IsChangeAttackPosition(LPCHARACTER target) const
{
	if (!IsNPC())
		return true;

	DWORD dwChangeTime = AI_CHANGE_ATTACK_POISITION_TIME_NEAR;

	if (DISTANCE_APPROX(GetX() - target->GetX(), GetY() - target->GetY()) >
		AI_CHANGE_ATTACK_POISITION_DISTANCE + GetMobAttackRange())
		dwChangeTime = AI_CHANGE_ATTACK_POISITION_TIME_FAR;

	return get_dword_time() - m_dwLastChangeAttackPositionTime > dwChangeTime;
}

void CHARACTER::GiveRandomSkillBook()
{
	LPITEM item = AutoGiveItem(50300);

	if (NULL != item)
	{
		extern const DWORD GetRandomSkillVnum(BYTE bJob = JOB_MAX_NUM);
		DWORD dwSkillVnum = 0;
		// 50% of getting random books or getting one of the same player's race
		if (!number(0, 1))
			dwSkillVnum = GetRandomSkillVnum(GetJob());
		else
			dwSkillVnum = GetRandomSkillVnum();
		item->SetSocket(0, dwSkillVnum);
	}
}

void CHARACTER::ReviveInvisible(int iDur)
{
#ifdef USE_BRAVE_CAPE_RENEWED
	if (m_playerData)
	{
		m_playerData->m_dwReviveTime = get_dword_time();
	}
#endif

	AddAffect(AFFECT_REVIVE_INVISIBLE, POINT_NONE, 0, AFF_REVIVE_INVISIBLE, iDur, 0, true);

#if defined(__AUTOMATIC_HUNTING_SYSTEM__)
#ifdef USE_PREMIUM_AFFECT
	if (GetPoint(POINT_PREMIUM_USER) > 0)
#else
	if (FindAffect(AFFECT_AUTO_HUNT))
#endif
	{
		if (!GetAutoUseType(AUTO_ONOFF_START))
		{
			RemoveAffect(AFFECT_AUTO_USE);

			for (uint8_t uiTypeIndex = AUTO_ONOFF_POTION; uiTypeIndex < AUTO_ONOFF_MAX; uiTypeIndex++)
			{
				SetAutoUseType(uiTypeIndex, false);
			}

			ChatPacket(CHAT_TYPE_COMMAND, "auto_onoff 0");
			ChatPacket(CHAT_TYPE_COMMAND, "autohunt_stat %d", GetAutoHuntUsedTime());
		}
	}
#endif
}

void CHARACTER::ToggleMonsterLog()
{
	m_bMonsterLog = !m_bMonsterLog;

	if (m_bMonsterLog)
	{
		CHARACTER_MANAGER::instance().RegisterForMonsterLog(this);
	}
	else
	{
		CHARACTER_MANAGER::instance().UnregisterForMonsterLog(this);
	}
}

void CHARACTER::SetGuild(CGuild* pGuild)
{
	if (m_pGuild != pGuild)
	{
		m_pGuild = pGuild;
		UpdatePacket();
	}
}

void CHARACTER::SendGreetMessage()
{
	auto& v = DBManager::instance().GetGreetMessage();

	for (auto it = v.begin(); it != v.end(); ++it)
	{
		ChatPacket(CHAT_TYPE_NOTICE, it->c_str());
	}
}

void CHARACTER::BeginStateEmpty()
{
	MonsterLog("!");
}

void CHARACTER::EffectPacket(int32_t enumEffectType, LPDESC specificDesc)
{
	TPacketGCSpecialEffect p{};

	p.header = HEADER_GC_SEPCIAL_EFFECT;
	p.type = enumEffectType;
	p.vid = GetVID();

	if (specificDesc)
		specificDesc->Packet(&p, sizeof(TPacketGCSpecialEffect));
	else
		PacketAround(&p, sizeof(TPacketGCSpecialEffect));
}

void CHARACTER::SpecificEffectPacket(const std::string& stEffectName)
{
	if (stEffectName.empty())
	{
		return;
	}

	TPacketGCSpecificEffect p;

	p.header = HEADER_GC_SPECIFIC_EFFECT;
	p.vid = GetVID();
	snprintf(p.effect_file, sizeof(p.effect_file), "%s", stEffectName.c_str());

	PacketAround(&p, sizeof(TPacketGCSpecificEffect));
}

void CHARACTER::MonsterChat(BYTE bMonsterChatType)
{
	if (IsPC())
		return;

	char sbuf[256+1];

	if (IsMonster())
	{
		if (number(0, 60))
			return;

		snprintf(sbuf, sizeof(sbuf),
				"(locale.monster_chat[%i] and locale.monster_chat[%i][%d] or '')",
				GetRaceNum(), GetRaceNum(), bMonsterChatType*3 + number(1, 3));
	}
	else
	{
		if (bMonsterChatType != MONSTER_CHAT_WAIT)
			return;

		if (IsGuardNPC())
		{
			if (number(0, 6))
				return;
		}
		else
		{
			if (number(0, 30))
				return;
		}

		snprintf(sbuf, sizeof(sbuf), "(locale.monster_chat[%i] and locale.monster_chat[%i][number(1, table.getn(locale.monster_chat[%i]))] or '')", GetRaceNum(), GetRaceNum(), GetRaceNum());
	}

	std::string text = quest::ScriptToString(sbuf);

	if (text.empty())
		return;

	struct packet_chat pack_chat;

	pack_chat.header    = HEADER_GC_CHAT;
	pack_chat.size	= sizeof(struct packet_chat) + text.size() + 1;
	pack_chat.type      = CHAT_TYPE_TALKING;
	pack_chat.id        = GetVID();
	pack_chat.bEmpire	= 0;

	TEMP_BUFFER buf;
	buf.write(&pack_chat, sizeof(struct packet_chat));
	buf.write(text.c_str(), text.size() + 1);

	PacketAround(buf.read_peek(), buf.size());
}

void CHARACTER::SetQuestNPCID(DWORD vid)
{
	m_dwQuestNPCVID = vid;
}

LPCHARACTER CHARACTER::GetQuestNPC() const
{
	return CHARACTER_MANAGER::instance().Find(m_dwQuestNPCVID);
}

void CHARACTER::SetQuestItemPtr(LPITEM item)
{
	m_pQuestItem = item;
}

void CHARACTER::ClearQuestItemPtr()
{
	m_pQuestItem = NULL;
}

LPITEM CHARACTER::GetQuestItemPtr() const
{
	return m_pQuestItem;
}

LPDUNGEON CHARACTER::GetDungeonForce() const
{
	if (m_lWarpMapIndex > 10000)
		return CDungeonManager::instance().FindByMapIndex(m_lWarpMapIndex);

	return m_pkDungeon;
}

void CHARACTER::SetBlockMode(BYTE bFlag)
{
	m_pointsInstant.bBlockMode = bFlag;

	ChatPacket(CHAT_TYPE_COMMAND, "setblockmode %d", m_pointsInstant.bBlockMode);

	SetQuestFlag("game_option.block_exchange", bFlag & BLOCK_EXCHANGE ? 1 : 0);
	SetQuestFlag("game_option.block_party_invite", bFlag & BLOCK_PARTY_INVITE ? 1 : 0);
	SetQuestFlag("game_option.block_guild_invite", bFlag & BLOCK_GUILD_INVITE ? 1 : 0);
	SetQuestFlag("game_option.block_whisper", bFlag & BLOCK_WHISPER ? 1 : 0);
	SetQuestFlag("game_option.block_messenger_invite", bFlag & BLOCK_MESSENGER_INVITE ? 1 : 0);
	SetQuestFlag("game_option.block_party_request", bFlag & BLOCK_PARTY_REQUEST ? 1 : 0);
}

void CHARACTER::SetBlockModeForce(BYTE bFlag)
{
	m_pointsInstant.bBlockMode = bFlag;
	ChatPacket(CHAT_TYPE_COMMAND, "setblockmode %d", m_pointsInstant.bBlockMode);
}

bool CHARACTER::IsGuardNPC() const
{
	return IsNPC() && (GetRaceNum() == 11000 || GetRaceNum() == 11002 || GetRaceNum() == 11004);
}

int CHARACTER::GetPolymorphPower() const
{
	if (test_server)
	{
		int value = quest::CQuestManager::instance().GetEventFlag("poly");
		if (value)
			return value;
	}
	return aiPolymorphPowerByLevel[MINMAX(0, GetSkillLevel(SKILL_POLYMORPH), 40)];
}

void CHARACTER::SetPolymorph(DWORD dwRaceNum, bool bMaintainStat)
{
#ifdef ENABLE_WOLFMAN_CHARACTER
	if (dwRaceNum < MAIN_RACE_MAX_NUM)
#else
	if (dwRaceNum < JOB_MAX_NUM)
#endif
	{
		dwRaceNum = 0;
		bMaintainStat = false;
	}

	if (m_dwPolymorphRace == dwRaceNum)
		return;

	m_bPolyMaintainStat = bMaintainStat;
	m_dwPolymorphRace = dwRaceNum;

	sys_log(0, "POLYMORPH: %s race %u ", GetName(), dwRaceNum);

	if (dwRaceNum != 0)
		StopRiding();

	SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);
	//m_afAffectFlag.Set(AFF_SPAWN);

	ViewReencode();

	REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);

	if (!bMaintainStat)
	{
		PointChange(POINT_ST, 0);
		PointChange(POINT_DX, 0);
		PointChange(POINT_IQ, 0);
		PointChange(POINT_HT, 0);
	}

	// Æú¸®¸ðÇÁ »óÅÂ¿¡¼­ Á×´Â °æ¿ì, Æú¸®¸ðÇÁ°¡ Ç®¸®°Ô µÇ´Âµ¥
	// Æú¸® ¸ðÇÁ ÀüÈÄ·Î valid combo intervalÀÌ ´Ù¸£±â ¶§¹®¿¡
	// Combo ÇÙ ¶Ç´Â Hacker·Î ÀÎ½ÄÇÏ´Â °æ¿ì°¡ ÀÖ´Ù.
	// µû¶ó¼­ Æú¸®¸ðÇÁ¸¦ Ç®°Å³ª Æú¸®¸ðÇÁ ÇÏ°Ô µÇ¸é,
	// valid combo intervalÀ» resetÇÑ´Ù.
	SetValidComboInterval(0);
	SetComboSequence(0);

	ComputeBattlePoints();
}

int CHARACTER::GetQuestFlag(const std::string& flag) const
{
	quest::CQuestManager& q = quest::CQuestManager::instance();
	quest::PC* pPC = q.GetPC(GetPlayerID());

	if(!pPC) {
        return 0;
    }  

	return pPC->GetFlag(flag);
}

void CHARACTER::SetQuestFlag(const std::string& flag, int value)
{
	quest::CQuestManager& q = quest::CQuestManager::instance();
	quest::PC* pPC = q.GetPC(GetPlayerID());
	if(!pPC) {
        return;
    } 

	pPC->SetFlag(flag, value);
}

void CHARACTER::DetermineDropMetinStone()
{
#ifdef ENABLE_NEWSTUFF
	if (g_NoDropMetinStone)
	{
		m_dwDropMetinStone = 0;
		return;
	}
#endif

	static const DWORD c_adwMetin[] =
	{
// #if defined(ENABLE_WOLFMAN_CHARACTER) && defined(USE_WOLFMAN_STONES)
		// 28012,
// #endif
		28030,
		28031,
		28032,
		28033,
		28034,
		28035,
		28036,
		28037,
		28038,
		28039,
		28040,
		28041,
		28042,
		28043,
		28044,
		28045,
		// 28046,
		// 28047,
		// 28048,
		// 28049,
// #if defined(ENABLE_MAGIC_REDUCTION_SYSTEM) && defined(USE_MAGIC_REDUCTION_STONES)
		// 28044,
		// 28045,
// #endif
	};
	DWORD stone_num = GetRaceNum();
	int idx = std::lower_bound(aStoneDrop, aStoneDrop+STONE_INFO_MAX_NUM, stone_num) - aStoneDrop;
	if (idx >= STONE_INFO_MAX_NUM || aStoneDrop[idx].dwMobVnum != stone_num)
	{
		m_dwDropMetinStone = 0;
	}
	else
	{
		const SStoneDropInfo & info = aStoneDrop[idx];
		m_bDropMetinStonePct = info.iDropPct;
		{
			m_dwDropMetinStone = c_adwMetin[number(0, sizeof(c_adwMetin)/sizeof(DWORD) - 1)];
			int iGradePct = number(1, 100);
			for (int iStoneLevel = 0; iStoneLevel < STONE_LEVEL_MAX_NUM; iStoneLevel ++)
			{
				int iLevelGradePortion = info.iLevelPct[iStoneLevel];
				if (iGradePct <= iLevelGradePortion)
				{
					break;
				}
				else
				{
					iGradePct -= iLevelGradePortion;
					m_dwDropMetinStone += 100; // µ¹ +a -> +(a+1)ÀÌ µÉ¶§¸¶´Ù 100¾¿ Áõ°¡
				}
			}
		}
	}
}

void CHARACTER::DetermineDropChestBooksMetin()
{
	static const DWORD c_adwMetin[] = {
										39054,
										39050,
										39052,
										39048,
	};

	DWORD stone_num = GetRaceNum();
	int idx = std::lower_bound(aChestDrop, aChestDrop+CHEST_BOOK_INFO_MAX_NUM, stone_num) - aChestDrop;
	if (idx >= CHEST_BOOK_INFO_MAX_NUM || aChestDrop[idx].dwMobVnum != stone_num) {
		m_dropChestBook = 0;
	} else {
		const SChestDrop & info = aChestDrop[idx];
		int random = number(0, sizeof(c_adwMetin) / sizeof(DWORD) - 1);
		m_dropChestBook = c_adwMetin[random];
		m_bDropChestBookPct = info.iChance[random];
	}
}


void CHARACTER::SendEquipment(LPCHARACTER ch)
{
	TPacketViewEquip p;
	p.header = HEADER_GC_VIEW_EQUIP;
	p.vid    = GetVID();
	for (int i = 0; i<WEAR_MAX_NUM; i++)
	{
		LPITEM item = GetWear(i);
		if (item)
		{
			p.equips[i].vnum = item->GetVnum();
			p.equips[i].count = item->GetCount();

			std::memcpy(p.equips[i].alSockets, item->GetSockets(), sizeof(p.equips[i].alSockets));
			std::memcpy(p.equips[i].aAttr, item->GetAttributes(), sizeof(p.equips[i].aAttr));
#ifdef ENABLE_YOHARA_SYSTEM
			std::memcpy(p.equips[i].aApplyRandom, item->GetApplysRandom(), sizeof(p.equips[i].aApplyRandom));
#endif

		}
		else
		{
			p.equips[i].vnum = 0;
		}
	}
	ch->GetDesc()->Packet(&p, sizeof(p));
}

bool CHARACTER::CanSummon(int iLeaderShip)
{
	return ((iLeaderShip >= 20) || ((iLeaderShip >= 12) && ((m_dwLastDeadTime + 180) > get_dword_time())));
}
#ifdef ENABLE_BIOLOG_SYSTEM
int CHARACTER::GetBiologState(const std::string& state) const
{
	quest::CQuestManager& q = quest::CQuestManager::instance();
	quest::PC* pPC = q.GetPC(GetPlayerID());
	if (!pPC)
		return 0;
	return pPC->GetFlag(state);
}

void CHARACTER::SetBiologState(const std::string& state, int szValue)
{
	quest::CQuestManager& q = quest::CQuestManager::instance();
	quest::PC* pPC = q.GetPC(GetPlayerID());
	if (!pPC)
		return;
	pPC->SetFlag(state, szValue);
}
#endif


void CHARACTER::MountVnum(DWORD vnum)
{
	if (m_dwMountVnum == vnum)
		return;
	if ((m_dwMountVnum != 0)&&(vnum!=0)) //@fixme108 set recursively to 0 for eventuality
		MountVnum(0);

	m_dwMountVnum = vnum;
	m_dwMountTime = get_dword_time();
#if defined(WJ_COMBAT_ZONE)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		return;
#endif
	if (m_bIsObserver)
		return;

	//NOTE : MountÇÑ´Ù°í ÇØ¼­ Client SideÀÇ °´Ã¼¸¦ »èÁ¦ÇÏÁø ¾Ê´Â´Ù.
	//±×¸®°í ¼­¹öSide¿¡¼­ ÅÀÀ»¶§ À§Ä¡ ÀÌµ¿Àº ÇÏÁö ¾Ê´Â´Ù. ¿Ö³ÄÇÏ¸é Client Side¿¡¼­ Coliision Adjust¸¦ ÇÒ¼ö ÀÖ´Âµ¥
	//°´Ã¼¸¦ ¼Ò¸ê½ÃÄ×´Ù°¡ ¼­¹öÀ§Ä¡·Î ÀÌµ¿½ÃÅ°¸é ÀÌ¶§ collision check¸¦ ÇÏÁö´Â ¾ÊÀ¸¹Ç·Î ¹è°æ¿¡ ³¢°Å³ª ¶Õ°í ³ª°¡´Â ¹®Á¦°¡ Á¸ÀçÇÑ´Ù.
	m_posDest.x = m_posStart.x = GetX();
	m_posDest.y = m_posStart.y = GetY();
	//EncodeRemovePacket(this);
	EncodeInsertPacket(this);

	ENTITY_MAP::iterator it = m_map_view.begin();

	while (it != m_map_view.end())
	{
		LPENTITY entity = (it++)->first;

		//MountÇÑ´Ù°í ÇØ¼­ Client SideÀÇ °´Ã¼¸¦ »èÁ¦ÇÏÁø ¾Ê´Â´Ù.
		//EncodeRemovePacket(entity);
		//if (!m_bIsObserver)
		EncodeInsertPacket(entity);

		//if (!entity->IsObserverMode())
		//	entity->EncodeInsertPacket(this);
	}

	SetValidComboInterval(0);
	SetComboSequence(0);

	ComputePoints();
}

namespace {
	class FuncCheckWarp
	{
		public:
			FuncCheckWarp(LPCHARACTER pkWarp)
			{
				m_lTargetY = 0;
				m_lTargetX = 0;

				m_lX = pkWarp->GetX();
				m_lY = pkWarp->GetY();

				m_bInvalid = false;
				m_bEmpire = pkWarp->GetEmpire();

				char szTmp[64];

				if (3 != sscanf(pkWarp->GetName(), " %s %ld %ld ", szTmp, &m_lTargetX, &m_lTargetY))
				{
					if (number(1, 100) < 5)
						sys_err("Warp NPC name wrong : vnum(%d) name(%s)", pkWarp->GetRaceNum(), pkWarp->GetName());

					m_bInvalid = true;

					return;
				}

				m_lTargetX *= 100;
				m_lTargetY *= 100;

				m_bUseWarp = true;

				if (pkWarp->IsGoto())
				{
					LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(pkWarp->GetMapIndex());
					m_lTargetX += pkSectreeMap->m_setting.iBaseX;
					m_lTargetY += pkSectreeMap->m_setting.iBaseY;
					m_bUseWarp = false;
				}
			}

			bool Valid()
			{
				return !m_bInvalid;
			}

			void operator () (LPENTITY ent)
			{
				if (!Valid())
					return;

				if (!ent->IsType(ENTITY_CHARACTER))
					return;

				LPCHARACTER pkChr = (LPCHARACTER) ent;

				if (!pkChr->IsPC())
					return;

				int iDist = DISTANCE_APPROX(pkChr->GetX() - m_lX, pkChr->GetY() - m_lY);

				if (iDist > 300)
					return;

				if (m_bEmpire && pkChr->GetEmpire() && m_bEmpire != pkChr->GetEmpire())
					return;

				if (pkChr->IsHack())
					return;

				if (!pkChr->CanHandleItem(false, true))
					return;

				if (m_bUseWarp)
					pkChr->WarpSet(m_lTargetX, m_lTargetY);
				else
				{
					pkChr->Show(pkChr->GetMapIndex(), m_lTargetX, m_lTargetY);
					pkChr->Stop();
				}
			}

			bool m_bInvalid;
			bool m_bUseWarp;

			long m_lX;
			long m_lY;
			long m_lTargetX;
			long m_lTargetY;

			BYTE m_bEmpire;
	};
}

EVENTFUNC(warp_npc_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info);
	if ( info == NULL )
	{
		sys_err( "warp_npc_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (ch == NULL) { // <Factor>
		return 0;
	}

	if (!ch->GetSectree())
	{
		ch->m_pkWarpNPCEvent = NULL;
		return 0;
	}

	FuncCheckWarp f(ch);
	if (f.Valid())
		ch->GetSectree()->ForEachAround(f);

	return passes_per_sec / 2;
}

#ifdef CHANGE_CHANNEL
void CHARACTER::ChannelSwitch(int new_ch){
	long lAddr;
	long lMapIndex;
	WORD wPort;
	long x = this->GetX();
	long y = this->GetY();

	if (!CMapLocation::instance().Get(x, y, lMapIndex, lAddr, wPort))
	{
			sys_err("cannot find map location index %d x %d y %d name %s", lMapIndex, x, y, GetName());
			return;
	}
	if(lMapIndex >= 10000){
		return;
	}
	
	std::map<WORD, int>micha;
	for(int i = 0; i < 4; i++){ //replace with maximum channels -1   actual 1300x - 1330x
		for(int i2 = 2; i2 < 6; i2++){ //replace with your core values actual  13x02 - 13x03
			micha[13*1000 + i*100 + i2] = i+1;
		}
	}

	int chan;
	if(micha.find(wPort) != micha.end()){
		chan = micha[wPort];
	}
	else{
		if (test_server)
			ChatPacket(CHAT_TYPE_INFO, "Change Channel cant find the new port %d, contact with the admin.", wPort);
		return;
	}
	if (test_server)
		ChatPacket(CHAT_TYPE_INFO, "new_ch: %d, lAddr: %d, lMapIndex: %d, wPort: %d, GetName: %s, NewwPort: %d, chan: %d", new_ch, lAddr, lMapIndex, wPort, GetName(), (wPort - 100*(chan-1) + 100*(new_ch-1)), chan);
	Stop();
	Save();
	
	if(GetSectree()){
		GetSectree()->RemoveEntity(this);
		ViewCleanup();
		EncodeRemovePacket(this);
	}
	
	
	TPacketGCWarp p;

	p.bHeader	= HEADER_GC_WARP;
	p.lX	= x;
	p.lY	= y;
	p.lAddr	= lAddr;
	p.wPort	= (wPort - 100*(chan-1) + 100*(new_ch-1));
	
#ifdef ENABLE_SWITCHBOT
	CSwitchbotManager::Instance().SetIsWarping(GetPlayerID(), true);

	if (p.wPort != mother_port)
	{
		CSwitchbotManager::Instance().P2PSendSwitchbot(GetPlayerID(), p.wPort);
	}
#endif
	
	sys_err("x : %d y: %d, addr: %d, port: %d , newPort: %d", x, y, lAddr, wPort, (wPort - 100*chan + 100*new_ch));

	GetDesc()->Packet(&p, sizeof(TPacketGCWarp));
}
#endif

void CHARACTER::StartWarpNPCEvent()
{
	if (m_pkWarpNPCEvent)
		return;

	if (!IsWarp() && !IsGoto())
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkWarpNPCEvent = event_create(warp_npc_event, info, passes_per_sec / 2);
}

void CHARACTER::SyncPacket()
{
	TEMP_BUFFER buf;

	TPacketCGSyncPositionElement elem;

	elem.dwVID = GetVID();
	elem.lX = GetX();
	elem.lY = GetY();

	TPacketGCSyncPosition pack;

	pack.bHeader = HEADER_GC_SYNC_POSITION;
	pack.wSize = sizeof(TPacketGCSyncPosition) + sizeof(elem);

	buf.write(&pack, sizeof(pack));
	buf.write(&elem, sizeof(elem));

	PacketAround(buf.read_peek(), buf.size());
}

LPCHARACTER CHARACTER::GetMarryPartner() const
{
	return m_pkChrMarried;
}

void CHARACTER::SetMarryPartner(LPCHARACTER ch)
{
	m_pkChrMarried = ch;
}

int CHARACTER::GetMarriageBonus(DWORD dwItemVnum, bool bSum)
{
	if (IsNPC())
		return 0;

	marriage::TMarriage* pMarriage = marriage::CManager::instance().Get(GetPlayerID());

	if (!pMarriage)
		return 0;

	return pMarriage->GetBonus(dwItemVnum, bSum, this);
}

void CHARACTER::ConfirmWithMsg(const char* szMsg, int iTimeout, DWORD dwRequestPID)
{
	if (!IsPC())
		return;

	TPacketGCQuestConfirm p;

	p.header = HEADER_GC_QUEST_CONFIRM;
	p.requestPID = dwRequestPID;
	p.timeout = iTimeout;
	strlcpy(p.msg, szMsg, sizeof(p.msg));

	GetDesc()->Packet(&p, sizeof(p));
}

int CHARACTER::GetPremiumRemainSeconds(BYTE bType) const
{
	if (bType >= PREMIUM_MAX_NUM)
		return 0;

	return m_aiPremiumTimes[bType] - get_global_time();
}

bool CHARACTER::WarpToPID(DWORD dwPID)
{
	LPCHARACTER victim;
	if ((victim = (CHARACTER_MANAGER::instance().FindByPID(dwPID))))
	{
		int mapIdx = victim->GetMapIndex();
		if (IS_SUMMONABLE_ZONE(mapIdx))
		{
			if (CAN_ENTER_ZONE(this, mapIdx))
			{
				WarpSet(victim->GetX(), victim->GetY());
			}
			else
			{
				ChatPacket(CHAT_TYPE_INFO, "69");
				return false;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, "69");
			return false;
		}
	}
	else
	{
		// ´Ù¸¥ ¼­¹ö¿¡ ·Î±×ÀÎµÈ »ç¶÷ÀÌ ÀÖÀ½ -> ¸Þ½ÃÁö º¸³» ÁÂÇ¥¸¦ ¹Þ¾Æ¿ÀÀÚ
		// 1. A.pid, B.pid ¸¦ »Ñ¸²
		// 2. B.pid¸¦ °¡Áø ¼­¹ö°¡ »Ñ¸°¼­¹ö¿¡°Ô A.pid, ÁÂÇ¥ ¸¦ º¸³¿
		// 3. ¿öÇÁ
		CCI * pcci = P2P_MANAGER::instance().FindByPID(dwPID);

		if (!pcci)
		{
			ChatPacket(CHAT_TYPE_INFO, "70");
			return false;
		}

#ifndef __RENEWAL_WARP__
		if (pcci->bChannel != g_bChannel)
		{
			ChatPacket(CHAT_TYPE_INFO, "71 %d %d", pcci->bChannel, g_bChannel);
			return false;
		}
		else if (false == IS_SUMMONABLE_ZONE(pcci->lMapIndex))
#else
		if (false == IS_SUMMONABLE_ZONE(pcci->lMapIndex))
#endif
		{
			ChatPacket(CHAT_TYPE_INFO, "69");
			return false;
		}
		else
		{
			if (!CAN_ENTER_ZONE(this, pcci->lMapIndex))
			{
				ChatPacket(CHAT_TYPE_INFO, "69");
				return false;
			}

			TPacketGGFindPosition p;
			p.header = HEADER_GG_FIND_POSITION;
			p.dwFromPID = GetPlayerID();
			p.dwTargetPID = dwPID;
			pcci->pkDesc->Packet(&p, sizeof(TPacketGGFindPosition));

			if (test_server)
				ChatPacket(CHAT_TYPE_PARTY, "sent find position packet for teleport");
		}
	}
	return true;
}

// ADD_REFINE_BUILDING
CGuild* CHARACTER::GetRefineGuild() const
{
	LPCHARACTER chRefineNPC = CHARACTER_MANAGER::instance().Find(m_dwRefineNPCVID);

	return (chRefineNPC ? chRefineNPC->GetGuild() : NULL);
}

bool CHARACTER::IsRefineThroughGuild() const
{
	return GetRefineGuild() != NULL;
}

int CHARACTER::ComputeRefineFee(int iCost, int iMultiply) const
{
	CGuild* pGuild = GetRefineGuild();
	if (pGuild)
	{
		if (pGuild == GetGuild())
			return iCost * iMultiply * 9 / 10;

		// ´Ù¸¥ Á¦±¹ »ç¶÷ÀÌ ½ÃµµÇÏ´Â °æ¿ì Ãß°¡·Î 3¹è ´õ
		LPCHARACTER chRefineNPC = CHARACTER_MANAGER::instance().Find(m_dwRefineNPCVID);
		if (chRefineNPC && chRefineNPC->GetEmpire() != GetEmpire())
			return iCost * iMultiply * 3;

		return iCost * iMultiply;
	}
	else
		return iCost;
}

void CHARACTER::PayRefineFee(int iTotalMoney)
{
	int iFee = iTotalMoney / 10;
	CGuild* pGuild = GetRefineGuild();

	int iRemain = iTotalMoney;

	if (pGuild)
	{
		// ÀÚ±â ±æµåÀÌ¸é iTotalMoney¿¡ ÀÌ¹Ì 10%°¡ Á¦¿ÜµÇ¾îÀÖ´Ù
		if (pGuild != GetGuild())
		{
			pGuild->RequestDepositMoney(this, iFee);
			iRemain -= iFee;
		}
	}

	PointChange(POINT_GOLD, -iRemain);
}
// END_OF_ADD_REFINE_BUILDING

//Hack ¹æÁö¸¦ À§ÇÑ Ã¼Å©.
bool CHARACTER::IsHack(bool bSendMsg, bool bCheckShopOwner, int limittime
//#ifdef USE_CRYSTAL_SYSTEM
, bool bSkipShardCraft/* = false*/
//#endif
)
{
	const int iPulse = thecore_pulse();

	if (test_server)
		bSendMsg = true;

	//Ã¢°í ¿¬ÈÄ Ã¼Å©
	if (iPulse - GetSafeboxLoadTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, "72 %d", limittime);

		if (test_server)
			ChatPacket(CHAT_TYPE_INFO, "[TestOnly]Pulse %d LoadTime %d PASS %d", iPulse, GetSafeboxLoadTime(), PASSES_PER_SEC(limittime));
		return true;
	}

	//°Å·¡°ü·Ã Ã¢ Ã¼Å©
	if (bCheckShopOwner)
	{
		if (GetExchange()
			 || GetShop()
			 || IsOpenSafebox()
			 || IsCubeOpen()
#ifdef __BL_67_ATTR__
			 || Is67AttrOpen()
#endif
#ifdef __AURA_SYSTEM__
			 || IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
			 || isSashOpened(true)
			 || isSashOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
			 || (!bSkipShardCraft && IsShardCraftOpened())
#endif
			)
		{
			if (bSendMsg)
				ChatPacket(CHAT_TYPE_INFO, "73");

			return true;
		}
	}
	else
	{
		if (GetExchange()
			 || GetShop()
			 || IsOpenSafebox()
			 || IsCubeOpen()
#ifdef __BL_67_ATTR__
			 || Is67AttrOpen()
#endif
#ifdef __AURA_SYSTEM__
			 || IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
			 || isSashOpened(true)
			 || isSashOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
			 || (!bSkipShardCraft && IsShardCraftOpened())
#endif
			)
		{
			if (bSendMsg)
				ChatPacket(CHAT_TYPE_INFO, "73");

			return true;
		}
	}

	//PREVENT_PORTAL_AFTER_EXCHANGE
	//±³È¯ ÈÄ ½Ã°£Ã¼Å©
	if (iPulse - GetExchangeTime()  < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, "74 %d", limittime);
		return true;
	}
	//END_PREVENT_PORTAL_AFTER_EXCHANGE

	//PREVENT_ITEM_COPY
	if (iPulse - GetMyShopTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, "74 %d", limittime);
		return true;
	}

	if (iPulse - GetRefineTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, "75", limittime);
		return true;
	}

#ifdef ENABLE_IKASHOP_RENEWAL
	if (iPulse - GetIkarusShopUseTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You will not be able to move within %d seconds of the transaction."), limittime);
		}

		return true;
	}

#ifdef USE_IKASHOP_UI_RENEWED
	if (iPulse - GetIkarusOpenShopTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You will not be able to move within %d seconds of the transaction."), limittime);
		}

		return true;
	}
#endif
#endif
	//END_PREVENT_ITEM_COPY

	if (IsLastMoveItemTime())
		return true;
	
	return false;
}

BOOL CHARACTER::IsMonarch() const
{
	//MONARCH_LIMIT
	if (CMonarch::instance().IsMonarch(GetPlayerID(), GetEmpire()))
		return true;

	return false;

	//END_MONARCH_LIMIT
}
void CHARACTER::Say(const std::string & s)
{
	struct ::packet_script packet_script;

	packet_script.header = HEADER_GC_SCRIPT;
	packet_script.skin = 1;
	packet_script.src_size = s.size();
	packet_script.size = packet_script.src_size + sizeof(struct packet_script);

	TEMP_BUFFER buf;

	buf.write(&packet_script, sizeof(struct packet_script));
	buf.write(&s[0], s.size());

	if (IsPC())
	{
		GetDesc()->Packet(buf.read_peek(), buf.size());
	}
}

//
// Monarch
//
void CHARACTER::InitMC()
{
	for (int n = 0; n < MI_MAX; ++n)
	{
		m_dwMonarchCooltime[n] = thecore_pulse();
	}

	m_dwMonarchCooltimelimit[MI_HEAL] = PASSES_PER_SEC(MC_HEAL);
	m_dwMonarchCooltimelimit[MI_WARP] = PASSES_PER_SEC(MC_WARP);
	m_dwMonarchCooltimelimit[MI_TRANSFER] = PASSES_PER_SEC(MC_TRANSFER);
	m_dwMonarchCooltimelimit[MI_TAX] = PASSES_PER_SEC(MC_TAX);
	m_dwMonarchCooltimelimit[MI_SUMMON] = PASSES_PER_SEC(MC_SUMMON);

	m_dwMonarchCooltime[MI_HEAL] -= PASSES_PER_SEC(GetMCL(MI_HEAL));
	m_dwMonarchCooltime[MI_WARP] -= PASSES_PER_SEC(GetMCL(MI_WARP));
	m_dwMonarchCooltime[MI_TRANSFER] -= PASSES_PER_SEC(GetMCL(MI_TRANSFER));
	m_dwMonarchCooltime[MI_TAX] -= PASSES_PER_SEC(GetMCL(MI_TAX));
	m_dwMonarchCooltime[MI_SUMMON] -= PASSES_PER_SEC(GetMCL(MI_SUMMON));
}

DWORD CHARACTER::GetMC(enum MONARCH_INDEX e) const
{
	return m_dwMonarchCooltime[e];
}

void CHARACTER::SetMC(enum MONARCH_INDEX e)
{
	m_dwMonarchCooltime[e] = thecore_pulse();
}

bool CHARACTER::IsMCOK(enum MONARCH_INDEX e) const
{
	int iPulse = thecore_pulse();

	if ((iPulse -  GetMC(e)) <  GetMCL(e))
	{
		if (test_server)
			sys_log(0, " Pulse %d cooltime %d, limit %d", iPulse, GetMC(e), GetMCL(e));

		return false;
	}

	if (test_server)
		sys_log(0, " Pulse %d cooltime %d, limit %d", iPulse, GetMC(e), GetMCL(e));

	return true;
}

DWORD CHARACTER::GetMCL(enum MONARCH_INDEX e) const
{
	return m_dwMonarchCooltimelimit[e];
}

DWORD CHARACTER::GetMCLTime(enum MONARCH_INDEX e) const
{
	int iPulse = thecore_pulse();

	if (test_server)
		sys_log(0, " Pulse %d cooltime %d, limit %d", iPulse, GetMC(e), GetMCL(e));

	return  (GetMCL(e)) / passes_per_sec   -  (iPulse - GetMC(e)) / passes_per_sec;
}

bool CHARACTER::IsSiegeNPC() const
{
	return IsNPC() && (GetRaceNum() == 11000 || GetRaceNum() == 11002 || GetRaceNum() == 11004);
}

//------------------------------------------------
void CHARACTER::UpdateDepositPulse()
{
	m_deposit_pulse = thecore_pulse() + PASSES_PER_SEC(60*5);	// 5ºÐ
}

bool CHARACTER::CanDeposit() const
{
	return (m_deposit_pulse == 0 || (m_deposit_pulse < thecore_pulse()));
}
//------------------------------------------------

ESex GET_SEX(LPCHARACTER ch)
{
	switch (ch->GetRaceNum())
	{
		case MAIN_RACE_WARRIOR_M:
		case MAIN_RACE_SURA_M:
		case MAIN_RACE_ASSASSIN_M:
		case MAIN_RACE_SHAMAN_M:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case MAIN_RACE_WOLFMAN_M:
#endif
			return SEX_MALE;

		case MAIN_RACE_ASSASSIN_W:
		case MAIN_RACE_SHAMAN_W:
		case MAIN_RACE_WARRIOR_W:
		case MAIN_RACE_SURA_W:
			return SEX_FEMALE;
	}

	/* default sex = male */
	return SEX_MALE;
}

int CHARACTER::GetHPPct() const
{
	if (GetMaxHP() <= 0) // @fixme136
		return 0;
	return (GetHP() * 100) / GetMaxHP();
}

bool CHARACTER::IsBerserk() const
{
	if (m_pkMobInst != NULL)
		return m_pkMobInst->m_IsBerserk;
	else
		return false;
}

void CHARACTER::SetBerserk(bool mode)
{
	if (m_pkMobInst != NULL)
		m_pkMobInst->m_IsBerserk = mode;
}

bool CHARACTER::IsGodSpeed() const
{
	if (m_pkMobInst != NULL)
	{
		return m_pkMobInst->m_IsGodSpeed;
	}
	else
	{
		return false;
	}
}

void CHARACTER::SetGodSpeed(bool mode)
{
	if (m_pkMobInst != NULL)
	{
		m_pkMobInst->m_IsGodSpeed = mode;

		if (mode == true)
		{
			SetPoint(POINT_ATT_SPEED, 250);
		}
		else
		{
			SetPoint(POINT_ATT_SPEED, m_pkMobData->m_table.sAttackSpeed);
		}
	}
}

bool CHARACTER::IsDeathBlow() const
{
	if (number(1, 100) <= m_pkMobData->m_table.bDeathBlowPoint)
	{
		return true;
	}
	else
	{
		return false;
	}
}

struct FFindReviver
{
	FFindReviver()
	{
		pChar = NULL;
		HasReviver = false;
	}

	void operator() (LPCHARACTER ch)
	{
		if (ch->IsMonster() != true)
		{
			return;
		}

		if (ch->IsReviver() == true && pChar != ch && ch->IsDead() != true)
		{
			if (number(1, 100) <= ch->GetMobTable().bRevivePoint)
			{
				HasReviver = true;
				pChar = ch;
			}
		}
	}

	LPCHARACTER pChar;
	bool HasReviver;
};

bool CHARACTER::HasReviverInParty() const
{
	LPPARTY party = GetParty();

	if (party != NULL)
	{
		if (party->GetMemberCount() == 1) return false;

		FFindReviver f;
		party->ForEachMemberPtr(f);
		return f.HasReviver;
	}

	return false;
}

bool CHARACTER::IsRevive() const
{
	if (m_pkMobInst != NULL)
	{
		return m_pkMobInst->m_IsRevive;
	}

	return false;
}

void CHARACTER::SetRevive(bool mode)
{
	if (m_pkMobInst != NULL)
	{
		m_pkMobInst->m_IsRevive = mode;
	}
}

#define IS_SPEED_HACK_PLAYER(ch) (ch->m_speed_hack_count > SPEEDHACK_LIMIT_COUNT)

EVENTFUNC(check_speedhack_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info);
	if ( info == NULL )
	{
		sys_err( "check_speedhack_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (NULL == ch || ch->IsNPC())
		return 0;

	if (IS_SPEED_HACK_PLAYER(ch))
	{
		// write hack log
		LogManager::instance().SpeedHackLog(ch->GetPlayerID(), ch->GetX(), ch->GetY(), ch->m_speed_hack_count);

		if (g_bEnableSpeedHackCrash)
		{
			// close connection
			LPDESC desc = ch->GetDesc();

			if (desc)
			{
				DESC_MANAGER::instance().DestroyDesc(desc);
				return 0;
			}
		}
	}

	ch->m_speed_hack_count = 0;

	ch->ResetComboHackCount();
	return PASSES_PER_SEC(60);
}

void CHARACTER::StartCheckSpeedHackEvent()
{
	if (m_pkCheckSpeedHackEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkCheckSpeedHackEvent = event_create(check_speedhack_event, info, PASSES_PER_SEC(60));	// 1ºÐ
}

void CHARACTER::GoHome()
{
	WarpSet(EMPIRE_START_X(GetEmpire()), EMPIRE_START_Y(GetEmpire()));
}

void CHARACTER::SendGuildName(CGuild* pGuild)
{
	if (NULL == pGuild) return;

	DESC	*desc = GetDesc();

	if (NULL == desc) return;
	if (m_known_guild.find(pGuild->GetID()) != m_known_guild.end()) return;

	m_known_guild.insert(pGuild->GetID());

	TPacketGCGuildName	pack;
	memset(&pack, 0x00, sizeof(pack));

	pack.header		= HEADER_GC_GUILD;
	pack.subheader	= GUILD_SUBHEADER_GC_GUILD_NAME;
	pack.size		= sizeof(TPacketGCGuildName);
	pack.guildID	= pGuild->GetID();
	memcpy(pack.guildName, pGuild->GetName(), GUILD_NAME_MAX_LEN);

	desc->Packet(&pack, sizeof(pack));
}

void CHARACTER::SendGuildName(DWORD dwGuildID)
{
	SendGuildName(CGuildManager::instance().FindGuild(dwGuildID));
}

EVENTFUNC(destroy_when_idle_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info);
	if ( info == NULL )
	{
		sys_err( "destroy_when_idle_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER ch = info->ch;
	if (ch == NULL) { // <Factor>
		return 0;
	}

	if (ch->GetVictim())
	{
		return PASSES_PER_SEC(300);
	}

	sys_log(1, "DESTROY_WHEN_IDLE: %s", ch->GetName());

	ch->m_pkDestroyWhenIdleEvent = NULL;
	M2_DESTROY_CHARACTER(ch);
	return 0;
}

void CHARACTER::StartDestroyWhenIdleEvent()
{
	if (m_pkDestroyWhenIdleEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkDestroyWhenIdleEvent = event_create(destroy_when_idle_event, info, PASSES_PER_SEC(300));
}

void CHARACTER::SetComboSequence(BYTE seq)
{
	m_bComboSequence = seq;
}

BYTE CHARACTER::GetComboSequence() const
{
	return m_bComboSequence;
}

void CHARACTER::SetLastComboTime(DWORD time)
{
	m_dwLastComboTime = time;
}

DWORD CHARACTER::GetLastComboTime() const
{
	return m_dwLastComboTime;
}

void CHARACTER::SetValidComboInterval(int interval)
{
	m_iValidComboInterval = interval;
}

int CHARACTER::GetValidComboInterval() const
{
	return m_iValidComboInterval;
}

BYTE CHARACTER::GetComboIndex() const
{
	return m_bComboIndex;
}

void CHARACTER::IncreaseComboHackCount(int k)
{
	m_iComboHackCount += k;

	if (m_iComboHackCount >= 10)
	{
		if (GetDesc())
			if (GetDesc()->DelayedDisconnect(number(2, 7)))
			{
				sys_log(0, "COMBO_HACK_DISCONNECT: %s count: %d", GetName(), m_iComboHackCount);
				LogManager::instance().HackLog("Combo", this);
			}
	}
}

void CHARACTER::ResetComboHackCount()
{
	m_iComboHackCount = 0;
}

void CHARACTER::SkipComboAttackByTime(int interval)
{
	m_dwSkipComboAttackByTime = get_dword_time() + interval;
}

DWORD CHARACTER::GetSkipComboAttackByTime() const
{
	return m_dwSkipComboAttackByTime;
}

void CHARACTER::ResetChatCounter()
{
	m_bChatCounter = 0;
}

BYTE CHARACTER::IncreaseChatCounter()
{
	return ++m_bChatCounter;
}

BYTE CHARACTER::GetChatCounter() const
{
	return m_bChatCounter;
}
#if defined(WJ_COMBAT_ZONE)
void CHARACTER::UpdateCombatZoneRankings(const char* memberName, DWORD memberEmpire, DWORD memberPoints)
{
	std::unique_ptr<SQLMsg> pMsg1(DBManager::instance().DirectQuery("INSERT INTO player.combat_zone_ranking_weekly (memberName, memberEmpire, memberPoints) VALUES('%s', '%d', '%d') ON DUPLICATE KEY UPDATE memberPoints = memberPoints + '%d'", memberName, memberEmpire, memberPoints, memberPoints));
	std::unique_ptr<SQLMsg> pMsg2(DBManager::instance().DirectQuery("INSERT INTO player.combat_zone_ranking_general (memberName, memberEmpire, memberPoints) VALUES('%s', '%d', '%d') ON DUPLICATE KEY UPDATE memberPoints = memberPoints + '%d'", memberName, memberEmpire, memberPoints, memberPoints));
}

BYTE CHARACTER::GetCombatZoneRank()
{
	if (GetDesc() != NULL)
	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT combat_zone_rank FROM player.player WHERE id = %u", GetPlayerID()));
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		return atoi(row[0]);
	}
	
	return 0;
}

DWORD CHARACTER::GetRealCombatZonePoints()
{
	return CCombatZoneManager::Instance().GetValueByType(this, COMBAT_ZONE_GET_POINTS);	
}

void CHARACTER::SetRealCombatZonePoints(DWORD iValue)
{
	DBManager::instance().Query("UPDATE player.player SET combat_zone_points = %d WHERE id = %u", iValue, GetPlayerID());
}
#endif
// ¸»ÀÌ³ª ´Ù¸¥°ÍÀ» Å¸°í ÀÖ³ª?
bool CHARACTER::IsRiding() const
{
	return IsHorseRiding() || GetMountVnum();
}

bool CHARACTER::CanWarp(bool isfromStorage/* = false*/
#ifdef USE_IKASHOP_UI_RENEWED
, bool bFromIkaShop/* = false*/
#endif
) const
{
	const int iPulse = thecore_pulse();
	const int limit_time = PASSES_PER_SEC(g_nPortalLimitTime);

	if ((iPulse - GetSafeboxLoadTime()) < limit_time)
		return false;

	if ((iPulse - GetExchangeTime()) < limit_time)
		return false;

	if ((iPulse - GetMyShopTime()) < limit_time)
		return false;

	if (!isfromStorage && IsLastMoveItemTime())
		return false;
	
	if ((iPulse - GetRefineTime()) < limit_time)
		return false;

#ifdef ENABLE_IKASHOP_RENEWAL
	if (iPulse - GetIkarusShopUseTime() < limit_time)
	{
		return false;
	}

#ifdef USE_IKASHOP_UI_RENEWED
	if (!bFromIkaShop && iPulse - GetIkarusOpenShopTime() < limit_time)
	{
		return false;
	}

	if ((!bFromIkaShop && GetIkarusShopGuest()) || GetIkarusAuctionGuest())
#else
	if (GetIkarusShopGuest() || GetIkarusAuctionGuest())
#endif
	{
		return false;
	}
#endif

	if (!isfromStorage && IsOpenSafebox())
		return false;
	
	if (GetExchange() || GetShop() || IsCubeOpen()
#ifdef __BL_67_ATTR__
		 || Is67AttrOpen()
#endif
#ifdef __AURA_SYSTEM__
		 || IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
		 || isSashWindowOpened(true)
		 || isSashWindowOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
		 || IsShardCraftOpened()
#endif
	)
	{
		return false;
	}

	return true;
}

DWORD CHARACTER::GetNextExp() const
{
	if (PLAYER_MAX_LEVEL_CONST < GetLevel())
		return 2500000000u;
	else
		return exp_table[GetLevel()];
}

#ifdef ENABLE_YOHARA_SYSTEM
DWORD CHARACTER::GetConquerorNextExp() const
{
	if (PLAYER_CONQUEROR_LEVEL + 1 < GetConquerorLevel())
		return 188537554;
	else
		return conqueror_exp_table[GetConquerorLevel()];
}
#endif

#ifdef ENABLE_SUPPORT_SYSTEM
DWORD CHARACTER::SupportGetNextExp() const
{
	return 0;
}
#endif


EVENTFUNC(anticheat_check_routine)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == NULL)
	{
		sys_err("anticheat_check_routine <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER Player = info->ch;

	if (!Player)
		return 0;

	LPDESC d = Player->GetDesc();

	if (!d)
		return 0;

	if (d->IsPhase(PHASE_HANDSHAKE) || d->IsPhase(PHASE_LOGIN) || d->IsPhase(PHASE_SELECT) || d->IsPhase(PHASE_DEAD) || d->IsPhase(PHASE_LOADING))
		return PASSES_PER_SEC(1);

	sys_log(2, "ANTICHEAT_CHECK: %s", Player->GetName());

	LPSECTREE CurrentTree = Player->GetSectree();
	if (!CurrentTree)
		return PASSES_PER_SEC(1);

	CAnticheatManager::Instance().SendCheckPacket(Player);

	return PASSES_PER_SEC(60);
}

void CHARACTER::StartAnticheatCommunication()
{
	if (m_pkAnticheatEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkAnticheatEvent = event_create(anticheat_check_routine, info, PASSES_PER_SEC(60));
}


#ifdef ENABLE_IKASHOP_RENEWAL
void CHARACTER::SetIkarusShopSafebox(SAFEBOX_HANDLE pk)
{
	if(m_pkIkarusShopSafebox && pk == nullptr)
		m_pkIkarusShopSafebox->SetOwner(nullptr);

	else if(m_pkIkarusShopSafebox == nullptr && pk)
		pk->SetOwner(this);

	m_pkIkarusShopSafebox  = pk;
}

void CHARACTER::SetIkarusShopGuest(SHOP_HANDLE pkShop)
{
	if(m_pkIkarusShopGuest)
		m_pkIkarusShopGuest->RemoveGuest(this);
	m_pkIkarusShopGuest = pkShop;
	if(pkShop)
		pkShop->AddGuest(this);
}

void CHARACTER::SetIkarusAuctionGuest(AUCTION_HANDLE pk)
{
	if(m_pkIkarusAuctionGuest)
		m_pkIkarusAuctionGuest->RemoveGuest(this);
	m_pkIkarusAuctionGuest = pk;
	if(pk)
		pk->AddGuest(this);
}

bool CHARACTER::CanTakeInventoryItem(LPITEM item, TItemPos* pos)
{
	// storing result in signed value
	int emptyPos = -1;

	if (item->IsDragonSoul())
	{
		pos->cell = emptyPos = GetEmptyDragonSoulInventory(item);
		pos->window_type = DRAGON_SOUL_INVENTORY;
	}

#ifdef ENABLE_SPECIAL_STORAGE
	else if (item->IsUpgradeItem())
	{
		pos->cell = emptyPos = GetEmptyUpgradeInventory(item);
		pos->window_type = UPGRADE_INVENTORY;
	}
	else if (item->IsBook())
	{
		pos->cell = emptyPos = GetEmptyBookInventory(item);
		pos->window_type = BOOK_INVENTORY;
	}
	else if (item->IsStone())
	{
		pos->cell = emptyPos = GetEmptyStoneInventory(item);
		pos->window_type = STONE_INVENTORY;
	}
	else if (item->IsChange())
	{
		pos->cell = emptyPos = GetEmptyChangeInventory(item);
		pos->window_type = CHANGE_INVENTORY;
	}
	else if (item->IsCostume())
	{
		pos->cell = emptyPos = GetEmptyCostumeInventory(item);
		pos->window_type = COSTUME_INVENTORY;
	}
	else if (item->IsChest())
	{
		pos->cell = emptyPos = GetEmptyChestInventory(item);
		pos->window_type = CHEST_INVENTORY;
	}
#endif
	else
	{
		pos->cell = emptyPos = GetEmptyInventory(item->GetSize());
		pos->window_type = INVENTORY;
	}

	return emptyPos != -1;
}

bool CHARACTER::IkarusShopFloodCheck(ShopActionWeight weight)
{
	// making timestamps
	using namespace std::chrono;
	const auto now = duration<double>(high_resolution_clock::now().time_since_epoch()).count();
	const auto elapsedTime = now - 60.0; // 1 minute ago

	// cleaning up register
	EraseIfContainer(m_IkarusShopActionRegister, [&](auto& pair){ return pair.second < elapsedTime; });

	// getting total weight
	const auto totalWeight = std::accumulate(m_IkarusShopActionRegister.begin(), m_IkarusShopActionRegister.end(), 0UL,
		[](auto total, auto& pair){ return total + pair.first; });

	// checking weight value
	if(totalWeight + weight > OFFLINESHOP_MAX_WEIGHT_PER_PLAYER){
		ikashop::GetManager().SendPopupMessage(this, "IKASHOP_SERVER_POPUP_MESSAGE_PLEASE_SLOW_DOWN");
		return false;
	}

	// registering the action
	m_IkarusShopActionRegister.emplace_back(std::make_pair(weight, now));
	return true;
}
#endif

int	CHARACTER::GetSkillPowerByLevel(int level, bool bMob) const
{
	return CTableBySkill::instance().GetSkillPowerByLevelFromType(GetJob(), GetSkillGroup(), MINMAX(0, level, SKILL_MAX_LEVEL), bMob);
}
#ifdef __NEWPET_SYSTEM__
DWORD CHARACTER::PetGetNextExp() const
{
	if (IsNewPet()) {
		if (120 < GetLevel())
			return 2500000000;
		else
			return exppet_table[GetLevel()];
	}
}
#endif

#ifdef __SASH_SYSTEM__
void CHARACTER::OpenSash(bool bCombination)
{
	if (isSashOpened(bCombination))
	{
		ChatPacket(CHAT_TYPE_INFO, "76");
		return;
	}
	
	if (bCombination)
	{
		if (m_bSashAbsorption)
		{
			ChatPacket(CHAT_TYPE_INFO, "77");
			return;
		}
		
		m_bSashCombination = true;
	}
	else
	{
		if (m_bSashCombination)
		{
			ChatPacket(CHAT_TYPE_INFO, "78");
			return;
		}
		
		m_bSashAbsorption = true;
	}
	
	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;
	
	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_OPEN;
	sPacket.bWindow = bCombination;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
	
	ClearSashMaterials();
}

void CHARACTER::CloseSash()
{
	if ((!m_bSashCombination) && (!m_bSashAbsorption))
		return;
	
	bool bWindow = (m_bSashCombination == true ? true : false);
	
	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;
	
	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_CLOSE;
	sPacket.bWindow = bWindow;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
	
	if (bWindow)
		m_bSashCombination = false;
	else
		m_bSashAbsorption = false;
	
	ClearSashMaterials();
}

void CHARACTER::ClearSashMaterials()
{
	LPITEM* pkItemMaterial = GetSashMaterials();
	if (!pkItemMaterial)
	{
		return;
	}

	for (uint8_t i = 0; i < SASH_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
		{
			continue;
		}

		pkItemMaterial[i]->Lock(false);
		pkItemMaterial[i] = NULL;
	}
}

bool CHARACTER::SashIsSameGrade(long lGrade)
{
	LPITEM* pkItemMaterial = GetSashMaterials();
	if (!pkItemMaterial)
	{
		return false;
	}

	if (!pkItemMaterial[0])
	{
		return false;
	}

	return (pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD) == lGrade) ? true : false;
}

DWORD CHARACTER::GetSashCombinePrice(long lGrade)
{
	DWORD dwPrice = 0;
	switch (lGrade)
	{
		case 2:
			{
				dwPrice = SASH_GRADE_2_PRICE;
			}
			break;
		case 3:
			{
				dwPrice = SASH_GRADE_3_PRICE;
			}
			break;
		case 4:
			{
				dwPrice = SASH_GRADE_4_PRICE;
			}
			break;
		default:
			{
				dwPrice = SASH_GRADE_1_PRICE;
			}
			break;
	}
	
	return dwPrice;
}

BYTE CHARACTER::CheckEmptyMaterialSlot()
{
	LPITEM* pkItemMaterial = GetSashMaterials();
	if (pkItemMaterial)
	{
		for (uint8_t i = 0; i < SASH_WINDOW_MAX_MATERIALS; ++i)
		{
			if (!pkItemMaterial[i])
			{
				return i;
			}
		}
	}

	return 255;
}

void CHARACTER::GetSashCombineResult(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs)
{
	LPITEM* pkItemMaterial = GetSashMaterials();
	if (!pkItemMaterial)
	{
		return;
	}

	if (m_bSashCombination)
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			long lVal = pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD);
			if (lVal == 4)
			{
				dwItemVnum = pkItemMaterial[0]->GetOriginalVnum();
				dwMinAbs = pkItemMaterial[0]->GetSocket(SASH_ABSORPTION_SOCKET);
				DWORD dwMaxAbsCalc = (dwMinAbs + SASH_GRADE_4_ABS_RANGE > SASH_GRADE_4_ABS_MAX ? SASH_GRADE_4_ABS_MAX : (dwMinAbs + SASH_GRADE_4_ABS_RANGE));
				dwMaxAbs = dwMaxAbsCalc;
			}
			else
			{
				DWORD dwMaskVnum = pkItemMaterial[0]->GetOriginalVnum();
				TItemTable * pTable = ITEM_MANAGER::instance().GetTable(dwMaskVnum + 1);
				if (pTable)
					dwMaskVnum += 1;
				
				dwItemVnum = dwMaskVnum;
				switch (lVal)
				{
					case 2:
						{
							dwMinAbs = SASH_GRADE_3_ABS;
							dwMaxAbs = SASH_GRADE_3_ABS;
						}
						break;
					case 3:
						{
							dwMinAbs = SASH_GRADE_4_ABS_MIN;
							dwMaxAbs = SASH_GRADE_4_ABS_MAX_COMB;
						}
						break;
					default:
						{
							dwMinAbs = SASH_GRADE_2_ABS;
							dwMaxAbs = SASH_GRADE_2_ABS;
						}
						break;
				}
			}
		}
		else
		{
			dwItemVnum = 0;
			dwMinAbs = 0;
			dwMaxAbs = 0;
		}
	}
	else
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			dwItemVnum = pkItemMaterial[0]->GetOriginalVnum();
			dwMinAbs = pkItemMaterial[0]->GetSocket(SASH_ABSORPTION_SOCKET);
			dwMaxAbs = dwMinAbs;
		}
		else
		{
			dwItemVnum = 0;
			dwMinAbs = 0;
			dwMaxAbs = 0;
		}
	}
}

void CHARACTER::AddSashMaterial(TItemPos tPos, BYTE bPos)
{
	if (bPos >= SASH_WINDOW_MAX_MATERIALS)
	{
		if (bPos == 255)
		{
			bPos = CheckEmptyMaterialSlot();
			if (bPos >= SASH_WINDOW_MAX_MATERIALS)
			{
				return;
			}
		}
		else
		{
			return;
		}
	}
	
	LPITEM pkItem = GetItem(tPos);
	if (!pkItem)
		return;
	else if ((pkItem->GetCell() >= INVENTORY_MAX_NUM) || (pkItem->IsEquipped()) || (tPos.IsBeltInventoryPosition()) || (pkItem->IsDragonSoul()))
		return;
	else if ((pkItem->GetType() != ITEM_COSTUME) && (m_bSashCombination))
		return;
	else if ((pkItem->GetType() != ITEM_COSTUME) && (bPos == 0) && (m_bSashAbsorption))
		return;
	else if (pkItem->isLocked())
	{
		ChatPacket(CHAT_TYPE_INFO, "79");
		return;
	}
	#ifdef __SOULBINDING_SYSTEM__
	else if ((pkItem->IsBind()) || (pkItem->IsUntilBind()))
	{
		ChatPacket(CHAT_TYPE_INFO, "80");
		return;
	}
	#endif
	else if ((m_bSashCombination) && (bPos == 1) && (!SashIsSameGrade(pkItem->GetValue(SASH_GRADE_VALUE_FIELD))))
	{
		ChatPacket(CHAT_TYPE_INFO, "81");
		return;
	}
	else if ((m_bSashCombination) && (pkItem->GetSocket(SASH_ABSORPTION_SOCKET) >= SASH_GRADE_4_ABS_MAX))
	{
		ChatPacket(CHAT_TYPE_INFO, "82");
		return;
	}
	else if ((bPos == 1) && (m_bSashAbsorption))
	{
		if ((pkItem->GetType() != ITEM_WEAPON) && (pkItem->GetType() != ITEM_ARMOR))
		{
			ChatPacket(CHAT_TYPE_INFO, "83");
			return;
		}
		else if ((pkItem->GetType() == ITEM_ARMOR) && (pkItem->GetSubType() != ARMOR_BODY))
		{
			ChatPacket(CHAT_TYPE_INFO, "83");
			return;
		}
	}
	else if ((pkItem->GetSubType() != COSTUME_SASH) && (m_bSashCombination))
		return;
	else if ((pkItem->GetSubType() != COSTUME_SASH) && (bPos == 0) && (m_bSashAbsorption))
		return;
	else if ((pkItem->GetSocket(SASH_ABSORBED_SOCKET) > 0) && (bPos == 0) && (m_bSashAbsorption))
		return;

	LPITEM* pkItemMaterial = GetSashMaterials();
	if (!pkItemMaterial)
	{
		return;
	}

	if ((bPos == 1) && (!pkItemMaterial[0]))
		return;

	if (pkItemMaterial[bPos])
		return;
	
	pkItemMaterial[bPos] = pkItem;
	pkItemMaterial[bPos]->Lock(true);
	
	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	GetSashCombineResult(dwItemVnum, dwMinAbs, dwMaxAbs);
	
	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_ADDED;
	sPacket.bWindow = m_bSashCombination == true ? true : false;
	sPacket.dwPrice = GetSashCombinePrice(pkItem->GetValue(SASH_GRADE_VALUE_FIELD));
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = dwItemVnum;
	sPacket.dwMinAbs = dwMinAbs;
	sPacket.dwMaxAbs = dwMaxAbs;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
}

void CHARACTER::RemoveSashMaterial(BYTE bPos)
{
	LPITEM* pkItemMaterial = GetSashMaterials();
	if (!pkItemMaterial)
	{
		return;
	}

	if (bPos >= SASH_WINDOW_MAX_MATERIALS)
	{
		return;
	}

	DWORD dwPrice = 0;
	
	if (bPos == 1)
	{
		if (pkItemMaterial[bPos])
		{
			pkItemMaterial[bPos]->Lock(false);
			pkItemMaterial[bPos] = NULL;
		}
		
		if (pkItemMaterial[0])
			dwPrice = GetSashCombinePrice(pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD));
	}
	else
		ClearSashMaterials();
	
	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;
	
	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_REMOVED;
	sPacket.bWindow = m_bSashCombination == true ? true : false;
	sPacket.dwPrice = dwPrice;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
}

BYTE CHARACTER::CanRefineSashMaterials()
{
#ifdef ENABLE_IKASHOP_RENEWAL
	if (GetIkarusShopGuest() || GetIkarusAuctionGuest())
	{
		return 0;
	}
#endif

	LPITEM* pkItemMaterial = GetSashMaterials();
	if (!pkItemMaterial)
	{
		return 0;
	}

	BYTE bReturn = 0;

	if (m_bSashCombination)
	{
		for (int i = 0; i < SASH_WINDOW_MAX_MATERIALS; ++i)
		{
			if (pkItemMaterial[i])
			{
				if ((pkItemMaterial[i]->GetType() == ITEM_COSTUME) && (pkItemMaterial[i]->GetSubType() == COSTUME_SASH))
					bReturn = 1;
				else
				{
					bReturn = 0;
					break;
				}
			}
			else
			{
				bReturn = 0;
				break;
			}
		}
	}
	else if (m_bSashAbsorption)
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			if ((pkItemMaterial[0]->GetType() == ITEM_COSTUME) && (pkItemMaterial[0]->GetSubType() == COSTUME_SASH))
				bReturn = 2;
			else
				bReturn = 0;
			
			if ((pkItemMaterial[1]->GetType() == ITEM_WEAPON) || ((pkItemMaterial[1]->GetType() == ITEM_ARMOR) && (pkItemMaterial[1]->GetSubType() == ARMOR_BODY)))
				bReturn = 2;
			else
				bReturn = 0;
			
			if (pkItemMaterial[0]->GetSocket(SASH_ABSORBED_SOCKET) > 0)
				bReturn = 0;
		}
		else
			bReturn = 0;
	}
	
	return bReturn;
}

void CHARACTER::RefineSashMaterials()
{
	LPITEM* pkItemMaterial = GetSashMaterials();
	if (!pkItemMaterial)
	{
		return;
	}

	BYTE bCan = CanRefineSashMaterials();
	if (bCan == 0)
	{
		return;
	}

	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	GetSashCombineResult(dwItemVnum, dwMinAbs, dwMaxAbs);
	long long dwPrice = GetSashCombinePrice(pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD));
	
	if (bCan == 1)
	{
		int iSuccessChance = 0;
		long lVal = pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD);
		switch (lVal)
		{
			case 2:
				{
					iSuccessChance = SASH_COMBINE_GRADE_2;
				}
				break;
			case 3:
				{
					iSuccessChance = SASH_COMBINE_GRADE_3;
				}
				break;
			case 4:
				{
					iSuccessChance = SASH_COMBINE_GRADE_4;
				}
				break;
			default:
				{
					iSuccessChance = SASH_COMBINE_GRADE_1;
				}
				break;
		}
		
		if (GetGold() < dwPrice)
		{
			ChatPacket(CHAT_TYPE_INFO, "84");
			return;
		}
		
		int iChance = number(1, 100);
		bool bSucces = (iChance <= iSuccessChance ? true : false);
		if (bSucces)
		{
			LPITEM pkItem = ITEM_MANAGER::instance().CreateItem(dwItemVnum, 1, 0, false);
			if (!pkItem)
			{
				sys_err("%d can't be created.", dwItemVnum);
				return;
			}
			
			ITEM_MANAGER::CopyAllAttrTo(pkItemMaterial[0], pkItem);
			LogManager::instance().ItemLog(this, pkItem, "COMBINE SUCCESS", pkItem->GetName());
			DWORD dwAbs = (dwMinAbs == dwMaxAbs ? dwMinAbs : number(dwMinAbs + 1, dwMaxAbs));
			pkItem->SetSocket(SASH_ABSORPTION_SOCKET, dwAbs);
			pkItem->SetSocket(SASH_ABSORBED_SOCKET, pkItemMaterial[0]->GetSocket(SASH_ABSORBED_SOCKET));
			
			PointChange(POINT_GOLD, -dwPrice);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);
			
			WORD wCell = pkItemMaterial[0]->GetCell();
			ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[0], "COMBINE (REFINE SUCCESS)");
			ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "COMBINE (REFINE SUCCESS)");
			
			pkItem->AddToCharacter(this, TItemPos(INVENTORY, wCell));
			ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
			pkItem->AttrLog();
			
			if (lVal == 4)
				ChatPacket(CHAT_TYPE_INFO, "New Absorption Rate: %d%", dwAbs);
			else
				ChatPacket(CHAT_TYPE_INFO, "86");

#ifdef ENABLE_REWARD_SYSTEM
			if (dwAbs == 25)
			{
				LPCHARACTER chrew = pkItem->GetOwner();
				if(chrew)
					CHARACTER_MANAGER::Instance().SetRewardData(REWARD_CUSTOM_SASH,chrew->GetName(), true);
			}
#endif

			EffectPacket(SE_EFFECT_SASH_SUCCEDED);
			LogManager::instance().SashLog(GetPlayerID(), GetX(), GetY(), dwItemVnum, pkItem->GetID(), 1, dwAbs, 1);

			for (uint8_t i = 0; i < SASH_WINDOW_MAX_MATERIALS; i++)
			{
				pkItemMaterial[i] = NULL;
			}
		}
		else
		{
			PointChange(POINT_GOLD, -dwPrice);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);
			
			ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "COMBINE (REFINE FAIL)");
			
			if (lVal == 4)
				ChatPacket(CHAT_TYPE_INFO, "85 %d%", pkItemMaterial[0]->GetSocket(SASH_ABSORPTION_SOCKET));
			else
				ChatPacket(CHAT_TYPE_INFO, "87");
			
			LogManager::instance().SashLog(GetPlayerID(), GetX(), GetY(), dwItemVnum, 0, 0, 0, 0);

			pkItemMaterial[1] = NULL;
		}
		
#ifdef ENABLE_BATTLE_PASS
		CHARACTER_MANAGER::Instance().DoMission(this, MISSION_SASH, 1, 0);
#endif
		
		TItemPos tPos;
		tPos.window_type = INVENTORY;
		tPos.cell = 0;
		
		TPacketSash sPacket;
		sPacket.header = HEADER_GC_SASH;
		sPacket.subheader = SASH_SUBHEADER_CG_REFINED;
		sPacket.bWindow = m_bSashCombination == true ? true : false;
		sPacket.dwPrice = dwPrice;
		sPacket.bPos = 0;
		sPacket.tPos = tPos;
		sPacket.dwItemVnum = 0;
		sPacket.dwMinAbs = 0;
		if (bSucces)
			sPacket.dwMaxAbs = 100;
		else
			sPacket.dwMaxAbs = 0;
		
		GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
	}
	else
	{
		pkItemMaterial[1]->CopyAttributeTo(pkItemMaterial[0]);

		LogManager::instance().ItemLog(this, pkItemMaterial[0], "ABSORB (REFINE SUCCESS)", pkItemMaterial[0]->GetName());
		pkItemMaterial[0]->SetSocket(SASH_ABSORBED_SOCKET, pkItemMaterial[1]->GetOriginalVnum());
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			if (pkItemMaterial[0]->GetAttributeValue(i) < 0)
				pkItemMaterial[0]->SetForceAttribute(i, pkItemMaterial[0]->GetAttributeType(i), 0);
		}
		
		ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "ABSORBED (REFINE SUCCESS)");
		pkItemMaterial[1] = NULL;

		ITEM_MANAGER::instance().FlushDelayedSave(pkItemMaterial[0]);
		pkItemMaterial[0]->AttrLog();
		
		ChatPacket(CHAT_TYPE_INFO, "86");
		
		ClearSashMaterials();
		
		TItemPos tPos;
		tPos.window_type = INVENTORY;
		tPos.cell = 0;
		
		TPacketSash sPacket;
		sPacket.header = HEADER_GC_SASH;
		sPacket.subheader = SASH_SUBHEADER_CG_REFINED;
		sPacket.bWindow = m_bSashCombination == true ? true : false;
		sPacket.dwPrice = dwPrice;
		sPacket.bPos = 255;
		sPacket.tPos = tPos;
		sPacket.dwItemVnum = 0;
		sPacket.dwMinAbs = 0;
		sPacket.dwMaxAbs = 1;
		GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
	}
}

bool CHARACTER::CleanSashAttr(LPITEM pkItem, LPITEM pkTarget)
{
	if (!CanHandleItem())
		return false;
	else if ((!pkItem) || (!pkTarget))
		return false;
	else if ((pkTarget->GetType() != ITEM_COSTUME) && (pkTarget->GetSubType() != COSTUME_SASH))
		return false;
	
	if (pkTarget->GetSocket(SASH_ABSORBED_SOCKET) <= 0)
		return false;
	
	pkTarget->SetSocket(SASH_ABSORBED_SOCKET, 0);
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		pkTarget->SetForceAttribute(i, 0, 0);
	
	pkItem->SetCount(pkItem->GetCount() - 1);
	LogManager::instance().ItemLog(this, pkTarget, "USE_DETACHMENT (CLEAN ATTR)", pkTarget->GetName());
	return true;
}
#endif

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
bool CHARACTER::CheckItemCanGet(const LPITEM item)
{
	if (!item)
	{
		return false;
	}

	const BYTE itemType = item->GetType();
	const BYTE itemSubType = item->GetSubType();

	if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_YANG) && item->GetVnum() == 1)
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_WEAPON) && (itemType == ITEM_WEAPON && itemSubType != WEAPON_ARROW))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_ARMOR) && (itemType == ITEM_ARMOR && itemSubType == ARMOR_BODY))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_SHIELD) && (itemType == ITEM_ARMOR && itemSubType == ARMOR_SHIELD))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_RINGS) && itemType == ITEM_NEW_RINGS)
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_HELMETS) && (itemType == ITEM_ARMOR && itemSubType == ARMOR_HEAD))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_BRACELETS) && (itemType == ITEM_ARMOR && itemSubType == ARMOR_WRIST))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_NECKLACE) && (itemType == ITEM_ARMOR && itemSubType == ARMOR_NECK))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_EARRINGS) && (itemType == ITEM_ARMOR && itemSubType == ARMOR_EAR))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_SHOES) && (itemType == ITEM_ARMOR && itemSubType == ARMOR_FOOTS))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_COSTUMES) && (itemType == ITEM_COSTUME && !(itemSubType == COSTUME_MOUNT_SKIN || itemSubType == COSTUME_PET || itemSubType == COSTUME_MOUNT || itemSubType == COSTUME_NEW_SASH || itemSubType == COSTUME_SASH)))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_PET_MOUNT) && (itemType == ITEM_COSTUME && (itemSubType == COSTUME_MOUNT_SKIN || itemSubType == COSTUME_PET || itemSubType == COSTUME_MOUNT)))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_SASH) && (itemType == ITEM_COSTUME && (itemSubType == COSTUME_NEW_SASH || itemSubType == COSTUME_SASH)))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_TALISMAN) && 
			 (itemType == ITEM_ARMOR && 
			 (itemSubType == ARMOR_PENDANT_FIRE || 
			  itemSubType == ARMOR_PENDANT_ICE || 
			  itemSubType == ARMOR_PENDANT_THUNDER || 
			  itemSubType == ARMOR_PENDANT_WIND || 
			  itemSubType == ARMOR_PENDANT_EARTH || 
			  itemSubType == ARMOR_PENDANT_DARK)))
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_BOOKS)
			&& (itemType == ITEM_SKILLBOOK || CItemVnumHelper::IsExtraBook(item->GetVnum()))
	)
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_STONES) && itemType == ITEM_METIN)
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_CHESTS) && itemType == ITEM_METIN)
	{
		return false;
	}
	else if (IS_SET(dwPickUPMode, AUTOMATIC_PICK_UP_BELTS) && itemType == ITEM_BELT)
	{
		return false;
	}

	return true;
}
void CHARACTER::ChangePickUPMode(DWORD dwMode, DWORD dwRMode)
{
	dwPickUPMode = dwMode;
	dwRarityMode = dwRMode;
	ChatPacket(CHAT_TYPE_COMMAND, "PickUPMode %u %u", dwPickUPMode, dwRarityMode);
}
#endif

#ifdef ENABLE_HIDE_COSTUME_SYSTEM

int CHARACTER::GetSlotsCostume(){
	int slot_default = 1;

#ifdef ENABLE_HIDE_COSTUME_SYSTEM_ACCE
	slot_default += 1;
#endif
#ifdef ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME
	slot_default += 1;
#endif

	slot_default += 1;

	slot_default += 2;

	return slot_default;
}

int CHARACTER::GetSlotDefault(int slot){
	int slot_amor = 0;
	int slot_hair = 1;
	int slot_weapon = 2;
	int slot_acce = 3;
	int slot_skin_acce = 4;
	int slot_shining_weapon = 5;
	int slot_shining_armor = 6;

	if(slot == 0){return slot_amor;}
	if(slot == 1){return slot_hair;}
	if(slot == 2){return slot_weapon;}
	if(slot == 3){return slot_acce;}
	if(slot == 4){return slot_skin_acce;}
	if(slot == 5){return slot_shining_weapon;}
	if(slot == 6){return slot_shining_armor;}

	return 0;
}

void CHARACTER::FuncHideCostume(int slot)
{

	if (slot <0 || slot > GetSlotsCostume())
		return;

	const int now = time(0);

	if (GetProtectTime("hide.last_click") > now)
	{
		ChatPacket(CHAT_TYPE_INFO, "You have to wait a few moments!");
		return;
	}

	SetProtectTime("hide.last_click", now + 3);

	// 0 = show, 1 = hide
	int part_value = 0;
	int part_acce = 0;
	bool bAdd = false;

	char costume_hide[1024];	
	snprintf(costume_hide, sizeof(costume_hide), "hide_costume_w.hide_costume_%d", slot);

	const CItem* pArmor;
	const CItem * pWeapon;
	LPITEM pkItem;


	if(slot == GetSlotDefault(0))
	{
		pkItem = GetWear(WEAR_COSTUME_BODY);
		if(!pkItem)
			return;

		if (GetQuestFlag(costume_hide) == 0) 
		{
			SetQuestFlag(costume_hide,1);
			pArmor = GetWear(WEAR_BODY);
		}
		else{
			SetQuestFlag(costume_hide,0);
			pArmor = GetWear(WEAR_COSTUME_BODY);
		}

#ifdef ENABLE_HIDE_COSTUME_SYSTEM_TRANSMUTACION
		if (NULL != pArmor){
			if (pArmor->GetTransmutation() > 0)
			{
				part_value = pArmor->GetTransmutation();
			}else{
				part_value = pArmor->GetVnum();
			}
		}else{
			part_value = GetOriginalPart(PART_MAIN);
		}
#else
		part_value = (NULL != pArmor) ? pArmor->GetVnum() : GetOriginalPart(PART_MAIN);
#endif

		SetPart(PART_MAIN, part_value);
	}

	else if(slot == GetSlotDefault(1))
	{
		pkItem = GetWear(WEAR_COSTUME_HAIR);
		if(!pkItem)
			return;

		if (GetQuestFlag(costume_hide) == 0) 
		{
			SetQuestFlag(costume_hide,1);
			bAdd = false;
		}
		else{
			SetQuestFlag(costume_hide,0);
			bAdd = true;
		}

		LPITEM pkItem = GetWear(WEAR_COSTUME_HAIR);
		if (pkItem){

#ifdef ENABLE_HIDE_COSTUME_SYSTEM_TRANSMUTACION
			if (pkItem->GetTransmutation() > 0){
				TItemTable * pHair = ITEM_MANAGER::instance().GetTable(pkItem->GetTransmutation());
				if (!pHair)
				{
					part_value = (true == bAdd) ? pkItem->GetValue(3) : 0;
				}
				else
				{
					part_value = (true == bAdd) ? pHair->alValues[3] : 0;
				}
			}else{
				part_value = (true == bAdd) ? pkItem->GetValue(3) : 0;
			}
#else
			part_value = (true == bAdd) ? pkItem->GetValue(3) : 0;
#endif
		}
		else{
			part_value = 0;
		}

		SetPart(PART_HAIR, part_value);
	}

	//Acce Slot 2
#ifdef ENABLE_HIDE_COSTUME_SYSTEM_ACCE
	else if(slot == GetSlotDefault(3))
	{

		pkItem = GetWear(WEAR_COSTUME_SASH);
		if(!pkItem)
			return;

		if (GetQuestFlag(costume_hide) == 0) 
		{
			SetQuestFlag(costume_hide,1);
			bAdd = false;
		}
		else{
			SetQuestFlag(costume_hide,0);
			bAdd = true;
		}

		if (pkItem){
			if(bAdd)
			{
				LPITEM sashSkin = GetWear(WEAR_COSTUME_NEW_SASH);
				if (sashSkin)
					part_value = sashSkin->GetVnum();
				else
					part_value = pkItem->GetVnum();
				
				part_value -= 85000;
			}else{
				part_value = 0;
			}
			
		}

		SetPart(PART_SASH, part_value);
	}
#endif

#ifdef	ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME
	//Weapon Slot 3
	else if(slot == GetSlotDefault(2))
	{

		pkItem = GetWear(WEAR_COSTUME_WEAPON);
		if(!pkItem)
			return;
		
		if (GetQuestFlag(costume_hide) == 0) 
		{
			SetQuestFlag(costume_hide,1);
			pWeapon = GetWear(WEAR_WEAPON);
		}
		else{
			SetQuestFlag(costume_hide,0);
			pWeapon = GetWear(WEAR_COSTUME_WEAPON);
		}

		if (NULL != pWeapon){

#ifdef ENABLE_HIDE_COSTUME_SYSTEM_TRANSMUTACION
			if (pWeapon->GetTransmutation() > 0)
			{
				part_value = pWeapon->GetTransmutation();
			}else{
				part_value = pWeapon->GetVnum();
			}
#else
			part_value = pWeapon->GetVnum();
#endif
		}else{
			part_value = GetOriginalPart(PART_WEAPON);
		}

		SetPart(PART_WEAPON, part_value);
	}
#endif

	else if(slot == GetSlotDefault(4))
	{
		pkItem = GetWear(WEAR_COSTUME_AURA);
		if(!pkItem)
			return;

		if (GetQuestFlag(costume_hide) == 0) 
		{
			SetQuestFlag(costume_hide,1);
			bAdd = false;
		}
		else{
			SetQuestFlag(costume_hide,0);
			bAdd = true;
		}

		LPITEM pkItem = GetWear(WEAR_COSTUME_AURA);
		if (pkItem){
			part_value = (true == bAdd) ? pkItem->GetVnum() : 0;
		}
		else{
			part_value = 0;
		}

		SetPart(PART_AURA, part_value);

		
		
		
		// pkItem = GetWear(WEAR_COSTUME_AURA);
		// if(!pkItem)
			// return;

		// if (GetQuestFlag(costume_hide) == 0) 
		// {
			// SetQuestFlag(costume_hide,1);
			// bAdd = false;
		// }
		// else{
			// SetQuestFlag(costume_hide,0);
			// bAdd = true;
		// }

		// if(!bAdd)
			// SetPart(PART_AURA, bAdd ? pkItem->GetVnum() : GetOriginalPart(PART_AURA));
		// else
			// SetPart(PART_AURA, 0);
		
		ComputePoints();
	}

	else if(slot == GetSlotDefault(5) || slot == GetSlotDefault(6))
	{
		pkItem = GetWear(WEAR_SHINING_WEAPON_1);
		if(!pkItem)
			return;

		if (GetQuestFlag(costume_hide) == 0) 
		{
			SetQuestFlag(costume_hide,1);
		}
		else{
			SetQuestFlag(costume_hide,0);
		}
	}

	BroadcastTargetPacket();
	FuncHideCostumeLoad();
	UpdatePacket();
	
}

void CHARACTER::FuncHideCostumeLoad()
{
	char costume_hide[1024];
	ChatPacket(CHAT_TYPE_COMMAND,"COSTUME_HIDE_CLEAR");

	for (int i = 0; i < GetSlotsCostume()+1; ++i)
	{
		snprintf(costume_hide, sizeof(costume_hide), "hide_costume_w.hide_costume_%d", i);
		ChatPacket(CHAT_TYPE_COMMAND,"COSTUME_HIDE_LIST %d %d",i,GetQuestFlag(costume_hide));
	}
	ChatPacket(CHAT_TYPE_COMMAND,"COSTUME_HIDE_LOAD");
}


DWORD CHARACTER::SetHideCostumeUpdateShining(DWORD id)
{
	CItem* shining_weapon = GetWear(WEAR_SHINING_WEAPON_1);
	CItem* shining_armor  = GetWear(WEAR_SHINING_ARMOR_1);
	if(id == 0)
	{
		if(shining_weapon)
		{
			if (GetHideCostume(GetSlotDefault(5)) == 1){
				return 0;
			}else{
				return shining_weapon->GetVnum();
			}
		}else{
			return 0;
		}

	}else{
		if(shining_armor)
		{
			if (GetHideCostume(GetSlotDefault(6)) == 1){
				return 0;
			}else{
				return shining_armor->GetVnum();
			}
		}else{
			return 0;
		}
	}

	return 0;
}

void CHARACTER::SetHideCostumeUpdate()
{
	CItem* body_costume 	= GetWear(WEAR_COSTUME_BODY);

#ifdef ENABLE_HIDE_COSTUME_SYSTEM_ACCE
	CItem* acce_costume 	= GetWear(WEAR_COSTUME_SASH);
#endif
	CItem* hair_costume 	= GetWear(WEAR_COSTUME_HAIR);

#ifdef ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME
	CItem* weapon_costume 	= GetWear(WEAR_COSTUME_WEAPON);
#endif

	CItem* aura_costume 	= GetWear(WEAR_COSTUME_AURA);

	if (GetHideCostume(GetSlotDefault(0)) == 1){
		CItem* body_armor = GetWear(WEAR_BODY);

		if (body_costume){
			if (body_armor){

#ifdef ENABLE_HIDE_COSTUME_SYSTEM_TRANSMUTACION
				if (body_armor->GetTransmutation() > 0){
					SetPart(PART_MAIN, body_armor->GetTransmutation());
				}else{
					SetPart(PART_MAIN, body_armor->GetVnum());
				}
#else
				SetPart(PART_MAIN, body_armor->GetVnum());
#endif
			}
			else
			{
				SetPart(PART_MAIN, GetOriginalPart(PART_MAIN));
			}
		}
	}

	if (GetHideCostume(GetSlotDefault(1)) == 1){
		if (hair_costume){
			SetPart(PART_HAIR, 0);
		}
	}

#ifdef ENABLE_HIDE_COSTUME_SYSTEM_ACCE
	if (GetHideCostume(GetSlotDefault(3)) == 1){
		if (acce_costume){
			SetPart(PART_SASH, 0);
		}
	}
#endif

#ifdef ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME
	if (GetHideCostume(GetSlotDefault(2)) == 1){
		CItem* body_weapon = GetWear(WEAR_WEAPON);
		if (weapon_costume){
			if (body_weapon){
#ifdef ENABLE_HIDE_COSTUME_SYSTEM_TRANSMUTACION
				if (body_weapon->GetTransmutation() > 0){
					SetPart(PART_WEAPON, body_weapon->GetTransmutation());
				}else{
					SetPart(PART_WEAPON, body_weapon->GetVnum());
				}
#else
				SetPart(PART_WEAPON, body_weapon->GetVnum());
#endif
			}
			else
			{
				SetPart(PART_WEAPON, GetOriginalPart(PART_WEAPON));
			}
		}
	}
#endif


	if (GetHideCostume(GetSlotDefault(4)) == 1){
		if (aura_costume){
			SetPart(PART_AURA, 0);
		}
	}


}

int CHARACTER::GetHideCostume(int wear) const
{
	char costume_hide[1024];
	snprintf(costume_hide, sizeof(costume_hide), "hide_costume_w.hide_costume_%d", wear);
	return GetQuestFlag(costume_hide);
}
#endif

#ifdef ENABLE_VOTE_COINS_WHILE_PLAYING
const int VOTE_COINS_WHILE_PLAYING_TIME = 60;

EVENTINFO(SVoteCoinsWhilePlaying)
{
	DWORD dwID;

	SVoteCoinsWhilePlaying() : dwID(0)
	{
	}
};

EVENTFUNC(VoteCoinsWhilePlayingEvent)
{
	SVoteCoinsWhilePlaying* info = dynamic_cast<SVoteCoinsWhilePlaying*>(event->info);

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(info->dwID);

	if (ch)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "88");

		int iAccountID = ch->GetDesc()->GetAccountTable().id;

		// Update coins.
		DBManager::instance().Query("UPDATE account.account SET drs = drs + 100 WHERE id = %d", ch->GetDesc()->GetAccountTable().id);
		
		// Send the coins to the client.
		std::unique_ptr<SQLMsg>pMsg (DBManager::instance().DirectQuery("SELECT coins, drs FROM account.account WHERE id = %d", iAccountID));
		if (pMsg->Get()->uiNumRows > 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			ch->ChatPacket(CHAT_TYPE_COMMAND, "ITEMSHOP COINS/%d/%d", atoi(row[0]), atoi(row[1]));
		}

		ch->SetVoteCoinsEventLastTime();

		int iLastTime = ch->GetQuestFlag("vote_coins_event.last_time_new");
		DWORD convert_time = (iLastTime-ch->GetRealPoint(POINT_PLAYTIME))*60;

		return PASSES_PER_SEC(convert_time);
	}

	return 0;
}

void CHARACTER::StartVoteCoinsEvent()
{
	if (m_pkVoteCoinsWhilePlaying)
		return;

	SVoteCoinsWhilePlaying* pEventInfo = AllocEventInfo<SVoteCoinsWhilePlaying>();
	pEventInfo->dwID = GetPlayerID();

	int iTime = VOTE_COINS_WHILE_PLAYING_TIME;
	int iLastTime = GetQuestFlag("vote_coins_event.last_time_new");

	if (iLastTime <= 0)
	{
		SetVoteCoinsEventLastTime();
		iLastTime = GetQuestFlag("vote_coins_event.last_time_new");
	}
	else if(GetRealPoint(POINT_PLAYTIME) >= iLastTime)
	{
		iLastTime = 5;
	}

	DWORD convert_time = (iLastTime-GetRealPoint(POINT_PLAYTIME))*60;

	m_pkVoteCoinsWhilePlaying = event_create(VoteCoinsWhilePlayingEvent, pEventInfo, PASSES_PER_SEC(convert_time));
	
}

void CHARACTER::SetVoteCoinsEventLastTime()
{
	SetQuestFlag("vote_coins_event.last_time_new", GetRealPoint(POINT_PLAYTIME) + VOTE_COINS_WHILE_PLAYING_TIME);
}
#endif

#if defined(WJ_COMBAT_ZONE)
EVENTFUNC(combat_zone_notice_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == NULL)
	{
		sys_err("combat_zone_notice_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (NULL == ch || ch->IsNPC())
		return 0;

	if(CCombatZoneManager::instance().IsCombatZoneMap(ch->GetMapIndex()) && CCombatZoneManager::instance().IsRunning())
	{
		ch->ChatPacket(CHAT_TYPE_BIG_NOTICE, "PARA GUARDAR LOS PUNTOS OBTENIDOS DEBES ABANDONAR LA TOTAL WAR ANTES DE QUE ACABE!");
		return PASSES_PER_SEC(60*10);
	}

	return 0;
}

void CHARACTER::StartNoticeMapCombatZone()
{
	if (m_pkNoticeCombatZoneEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();
	info->ch = this;
	m_pkNoticeCombatZoneEvent = event_create(combat_zone_notice_event, info, PASSES_PER_SEC(60*10));

}
#endif

#ifdef RENEWAL_DEAD_PACKET
#include "threeway_war.h"
DWORD CHARACTER::CalculateDeadTime(BYTE type)
{
	//generated from do_restart(cmd_general.cpp)
	if (!m_pkDeadEvent)
		return 0;
	int iTimeToDead = (event_time(m_pkDeadEvent) / passes_per_sec);

	if (type == REVIVE_TYPE_AUTO_TOWN)
		return iTimeToDead-7;
	if (!test_server && type == REVIVE_TYPE_HERE && (!GetWarMap() || GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG)) {
		if (IsHack(false) && !CThreeWayWar::instance().IsSungZiMapIndex(GetMapIndex()))
			return iTimeToDead - (180 - g_nPortalLimitTime);
		if (iTimeToDead > 170)
			return iTimeToDead - 170;
	}
	if (IsHack(false) && ((!GetWarMap() || GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG) || !CThreeWayWar::instance().IsSungZiMapIndex(GetMapIndex())))
		return iTimeToDead - (180 - g_nPortalLimitTime);
	if (iTimeToDead > 173) 
		return iTimeToDead - 173;
	return 0;
}
#endif

#ifdef BATTLEPASS_WORLDARD

BYTE CHARACTER::GetBattlePassId()
{
	CAffect* pAffect = FindAffect(AFFECT_BATTLE_PASS, POINT_BATTLE_PASS_ID);

	if (!pAffect)
		return 0;

	return pAffect->lApplyValue;
}

int CHARACTER::GetSecondsTillNextMonth()
{
	time_t iTime;
	time(&iTime);
	struct tm endTime = *localtime(&iTime);

	int iCurrentMonth = endTime.tm_mon;

	endTime.tm_hour = 0; 
	endTime.tm_min = 0; 
	endTime.tm_sec = 0;
	endTime.tm_mday = 1;

	if (iCurrentMonth == 11)
	{
		endTime.tm_mon = 0;
		endTime.tm_year = endTime.tm_year + 1;
	}
	else
	{
		endTime.tm_mon = iCurrentMonth + 1;
	}

	int seconds = difftime(mktime(&endTime), iTime);
	
	return seconds;
}

#endif

#ifdef ENABLE_NEW_PET_SYSTEM
bool CHARACTER::PetBlockMap()
{
	if (IsObserverMode())
		return true;
	switch (GetMapIndex())
	{
		case 999:
			return true;
	}
	return false;
}
#endif

#if defined(__BL_67_ATTR__)
void CHARACTER::Open67Attr()
{
	LPDESC d = GetDesc();
	if (!d)
		return;
	
	if (GetExchange()
		 || IsOpenSafebox()
		 || GetShop()
		 || IsCubeOpen()
#ifdef __AURA_SYSTEM__
		 || IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
		 || isSashOpened(true)
		 || isSashOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
		 || IsShardCraftOpened()
#endif
	)
	{
		ChatPacket(CHAT_TYPE_INFO, "89");
		return;
	}

	Set67Attr(true);

	TPacket67AttrOpenClose p;
	p.bHeader = HEADER_GC_OPEN_67_ATTR;
	d->Packet(&p, sizeof(p));
}
#endif

#ifdef ENABLE_BIYOLOG
void CHARACTER::CheckBio()
{
	int level = GetQuestFlag("bio.level");
	if (level == 0)
	{
		level += 1;
		SetQuestFlag("bio.level", level);
	}
	else if (level >= bio_max)
		return;

	int count = GetQuestFlag("bio.count");
	if (count >= bio_data[level][2])
	{
		if (count == bio_data[level][2])
		{
			if (bio_data[level][5] != 0)
				ChatPacket(CHAT_TYPE_COMMAND, "biostone %d", level);
			else
				if (bio_data[level][14] == 1)
					ChatPacket(CHAT_TYPE_COMMAND, "bioodul %d", level);
		}
		else
			if (bio_data[level][14] == 1)
				ChatPacket(CHAT_TYPE_COMMAND, "bioodul %d", level);
	}
	else
	{
		ChatPacket(CHAT_TYPE_COMMAND, "biodata %d %d %d", level, count, GetQuestFlag("bio.time"));
	}
}
#endif


#ifdef ENABLE_CHANNEL_CHANGE
void CHARACTER::MoveChannel(DWORD channelIndex)
{
	if (!IsPC())
		return;

	if (!CanWarp() || IsHack())
		return;

	if (!GetDesc())
		return;
	
	if(GetMapIndex() >= 10000)//dungeon check.
	{
		ChatPacket(CHAT_TYPE_INFO, "999");
		return;
	}

	int oldPort = mother_port-(g_bChannel > 1 ? (100*(g_bChannel-1)):0);
	int newPort = oldPort+(100*(channelIndex-1));
	long lAddr;
	long lMapIndex;
	WORD wPort;
	long x = this->GetX();
	long y = this->GetY();
	if (!CMapLocation::instance().Get(x, y, lMapIndex, lAddr, wPort))
		return;
	Stop();
	Save();
	if(GetSectree())
	{
		GetSectree()->RemoveEntity(this);
		ViewCleanup();
		EncodeRemovePacket(this);
	}
	
	ChatPacket(CHAT_TYPE_COMMAND, "UpdateServerInfo %d", channelIndex);
	TPacketGCWarp p;
	p.bHeader = HEADER_GC_WARP;
	p.lX = x;
	p.lY = y;
	p.lAddr = lAddr;
	p.wPort = newPort;
	
#ifdef ENABLE_SWITCHBOT
	CSwitchbotManager::Instance().SetIsWarping(GetPlayerID(), true);

	if (p.wPort != mother_port)
	{
		CSwitchbotManager::Instance().P2PSendSwitchbot(GetPlayerID(), p.wPort);
	}
#endif
	
	GetDesc()->Packet(&p, sizeof(TPacketGCWarp));
}
#endif

#ifdef ENABLE_BATTLE_PASS
int CHARACTER::GetBattlePassStatus()
{
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);
	if (GetQuestFlag("battlePass.monthIndex") != (vKey.tm_mon + 1))
		return 0;
	return GetQuestFlag("battlePass.status");
}
bool CHARACTER::IsBattlePassActive()
{
	return GetBattlePassStatus() == 1;
}
void CHARACTER::ClearBattlePassData()
{
	m_battlePassData.clear();
}
void CHARACTER::SetBattlePassMissonValue(BYTE missionIndex, long long missionValue)
{
	auto it = m_battlePassData.find(missionIndex);
	if (it != m_battlePassData.end())
		it->second = missionValue;
	else
		m_battlePassData.emplace(missionIndex, missionValue);
}
long long CHARACTER::GetBattlePassMissonValue(BYTE missionIndex)
{
	auto it = m_battlePassData.find(missionIndex);
	if (it != m_battlePassData.end())
		return it->second;
	return 0;
}
#endif

#ifdef ENABLE_DEFENSAWE_SHIP
bool CHARACTER::IsHydraMob() const
{
	switch(GetRaceNum())
	{
		case 6802:
		case 6803:
		case 6804:
		case 6805:
		case 6806:
		case 6807:
		case 6808:
		case 6809:
		case 6812:
			return true;
	}
	return false;
}
bool CHARACTER::IsHydraMobLP(LPCHARACTER ch) const
{
	switch(ch->GetRaceNum())
	{
		case 6802:
		case 6803:
		case 6804:
		case 6805:
		case 6806:
		case 6807:
		case 6808:
		case 6809:
		case 6812:
			return true;
	}
	return false;
}
bool CHARACTER::IsHydraNPC() const
{
	switch(GetRaceNum())
	{
		case 6801:
			return true;
	}
	return false;
}
bool CHARACTER::IsHydra() const
{
	switch(GetRaceNum())
	{
		case 6812:
			return true;
	}
	return false;
}
#endif


#ifdef ENABLE_NEW_DETAILS_GUI
void CHARACTER::SendKillLog()
{
	if(!GetDesc())
		return;
	TPacketGCKillLOG p;
	p.header = HEADER_GC_KILL_LOG;
	std::memcpy(p.kill_log, m_points.kill_log,sizeof(p.kill_log));
	GetDesc()->Packet(&p,sizeof(TPacketGCKillLOG));
}
#endif

#ifdef ENABLE_CSHIELD
EVENTFUNC(cshield_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == NULL)
	{
		sys_err("cshield_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (NULL == ch || ch->IsNPC())
		return 0;

	DWORD cShield = ch->GetCShield()->VerifyCShield(ch->GetName());

	if (cShield > 1)
	{
		LPDESC d = ch->GetDesc();
		if (d)
		{
			if (d->DelayedDisconnect(3))
			{
				if (cShield == 2)
					LogManager::instance().HackLog("CShield-ServerSide-ErrorCode: CShield Patched", ch);
				else
				{
					LogManager::instance().HackLog("CShield-ServerSide-ErrorCode: CShield Async", ch);
				}
				
#ifdef ENABLE_AUTOBAN
				std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT login FROM account.account WHERE id='%d' LIMIT 1", ch->GetAID()));
				if (pMsg->Get()->uiNumRows == 0)
					return 0;
				MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
				
				std::unique_ptr<SQLMsg> pMsg2(DBManager::instance().DirectQuery("SELECT count(*) FROM log.hack_log WHERE (why = 'CShield Async' OR why = 'CShield Patched') AND login = '%s' AND time BETWEEN DATE_ADD(NOW(), INTERVAL -20 MINUTE) AND NOW()", row[0]));
				if (pMsg2->Get()->uiNumRows == 0)
					return 0;

				MYSQL_ROW row2 = mysql_fetch_row(pMsg2->Get()->pSQLResult);
				if (atoi(row2[0]) >= 5)
				{
					std::unique_ptr<SQLMsg> pMsgUpdate(DBManager::instance().DirectQuery("UPDATE account.account set status='BLOCK' where id='%u'", ch->GetAID()));
					LogManager::instance().HackLog("CShield-ServerSide-ErrorCode: CShield Autoban", ch);
				}
#endif
			}
		}
	}

	return PASSES_PER_SEC(20);
}

EVENTFUNC(cshield_data_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == NULL)
	{
		sys_err("cshield_data_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (NULL == ch || ch->IsNPC())
		return 0;

	if (!ch->GetCShield()->GetCShieldInit())
	{
		ch->GetCShield()->SetCShieldTime();
		ch->SendCShieldPacket(true);
		ch->GetCShield()->SetCShieldIinit();
	}
	else
		ch->SendCShieldPacket(false);

	return PASSES_PER_SEC(3);
}

void CHARACTER::StartCShieldEvent()
{
	if (m_pkCShieldEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkCShieldEvent = event_create(cshield_event, info, PASSES_PER_SEC(15));
}

void CHARACTER::StartCShieldDataEvent()
{
	if (m_pkCShieldDataEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkCShieldDataEvent = event_create(cshield_data_event, info, PASSES_PER_SEC(7));
}

void CHARACTER::SendCShieldPacket(bool start)
{
	LPDESC d = GetDesc();
	if (!d)
		return;

	SPacketGCCShield packet;
	packet.header = HEADER_GC_CSHIELD;
	packet.start = start;
	packet.num = GetCShield()->GetCShieldNumber();
	d->Packet(&packet, sizeof(SPacketGCCShield));
}
#endif
void CHARACTER::StopAttacksOnLogout()
{
	//ChatPacket(CHAT_TYPE_INFO, "WTF1");
    for (auto it : m_map_kDamage)
    {
        auto vid = it.first;
        CHARACTER * mob = CHARACTER_MANAGER::instance().Find(vid);
		//ChatPacket(CHAT_TYPE_INFO, "The mob vid is %s, name" , GetVID(), GetName()); 
        if (mob)
        {
            mob->SetVictim(NULL); 
        }

    }
}

#ifdef ENABLE_ZODIAC_MISSION
void CHARACTER::BeadCount(LPCHARACTER ch)
{
	int bCount = ch->GetQuestFlag("Bead.Count");
	ch->ChatPacket(CHAT_TYPE_COMMAND, "SetBeadCount %d",bCount);
}
#endif

#ifdef ENABLE_MELEY_LAIR_DUNGEON
bool CHARACTER::IsMeley()
{
	switch(GetRaceNum())
	{
		case 6231:
		case 9563:
		case 6230:
		case 6119:
			return true;
			break;
	}
	return false;
}
LPCHARACTER CHARACTER::GetQuestNPCAttack() const
{
	return CHARACTER_MANAGER::instance().Find(m_dwQuestNPCVIDAttack);
}
#endif


#ifdef ENABLE_TRACK_WINDOW
void CHARACTER::GetDungeonCooldownTest(WORD bossIndex, int value, bool isCooldown)
{
	//HERE SET DUNGEONS FLAGS
	//BossVnum - (cooldownFlag.backDungeonFlag)
	const std::map<WORD, std::string> m_vecQuestCooldowns = {
		{1093,isCooldown ? "deviltower_zone.cooldown" : "deviltower_zone.time"},
		{2598,isCooldown ? "devilcatacomb_zone.cooldown" : "devilcatacomb_zone.time"},
		{2493,isCooldown ? "dragonlair.cooldown" : "dragonlair.time"},
		{6091,isCooldown ? "flame_dungeon.cooldown" : "flame_dungeon.time"},
		{9018,isCooldown ? "ShipDefense.cooldown" : "ShipDefense.time"},
		{6191,isCooldown ? "snow_dungeon.cooldown" : "snow_dungeon.time"},
		{20442,isCooldown ? "MeleyDungeon.cooldown" : "MeleyDungeon.time"},
		{2092,isCooldown ? "SpiderDungeon.cooldown" : "SpiderDungeon.time"},
		{6192,isCooldown ? "jotunDungeon.cooldown" : "jotunDungeon.time"},
	};
	const auto it = m_vecQuestCooldowns.find(bossIndex);
	if (it != m_vecQuestCooldowns.end())
	{
		SetQuestFlag(it->second.c_str(), time(0) + value);
		GetDungeonCooldown(bossIndex);
	}
}
void CHARACTER::GetDungeonCooldown(WORD bossIndex)
{
	//HERE SET DUNGEONS FLAGS
	//BossVnum - (cooldownFlag.backDungeonFlag)
	const std::map<WORD, std::pair<std::string, std::string>> m_vecQuestCooldowns = {
		{1093,{"dungeonManager.demonTowerTime", "demonTower_zone.demonTime"}},
		{2598,{"dungeonManager.devilDungeonTime", "devilCatacomb_zone.devilTime"}},
		{2493,{"dungeonManager.dragonDungeonTime", "dragonLair_zone.dragonTime"}},
		{6091,{"dungeonManager.blazingDungeonTime", "blazingPurgatory_zone.blazingTime"}},
		{9018,{"ShipDefense.cooldown", "ShipDefense.time"}},
		{6191,{"dungeonManager.nemereDungeonTime", "nemeresTower_zone.nemereTime"}},
		{6192,{"dungeonManager.jotunDungeonTime", "jotunDungeon_zone.jotunTime"}},
		{20442,{"MeleyDungeon.cooldown", "MeleyDungeon.time"}},
		{2092,{"dungeonManager.spiderDungeonTime", "spiderQueen_zone.spiderTime"}},
	};
	std::string cmdText("");
	if (bossIndex == WORD_MAX)
	{
#if __cplusplus < 199711L
		for (const auto& [bossVnum, cooldown] : m_vecQuestCooldowns)
		{
			const int leftTime = GetQuestFlag(cooldown.c_str()) - time(0);
#else
		for (auto it = m_vecQuestCooldowns.begin(); it != m_vecQuestCooldowns.end(); ++it)
		{
			const WORD bossVnum = it->first;
			int leftTime = GetQuestFlag(it->second.first.c_str()) - time(0);
#endif
			if (leftTime > 0)
			{
				cmdText += std::to_string(bossVnum);
				cmdText += "|";
				cmdText += std::to_string(leftTime);
				cmdText += "|";
				cmdText += "0";
				cmdText += "#";

				if (cmdText.length() + 50 > CHAT_MAX_LEN)
				{
					ChatPacket(CHAT_TYPE_COMMAND, "TrackDungeonInfo %s", cmdText.c_str());
					cmdText.clear();
				}
				continue;
			}

			leftTime = GetQuestFlag(it->second.second.c_str()) - time(0);
			if (leftTime > 0)
			{
				cmdText += std::to_string(bossVnum);
				cmdText += "|";
				cmdText += std::to_string(leftTime);
				cmdText += "|";
				cmdText += "1";
				cmdText += "#";
				if (cmdText.length() + 50 > CHAT_MAX_LEN)
				{
					ChatPacket(CHAT_TYPE_COMMAND, "TrackDungeonInfo %s", cmdText.c_str());
					cmdText.clear();
				}

				continue;
			}
		}

		
	}
	else
	{
		const auto it = m_vecQuestCooldowns.find(bossIndex);
		if (it != m_vecQuestCooldowns.end())
		{
			int leftTime = GetQuestFlag(it->second.first.c_str()) - time(0);
			if (leftTime > 0)
			{
				cmdText += std::to_string(it->first);
				cmdText += "|";
				cmdText += std::to_string(leftTime);
				cmdText += "|";
				cmdText += "0";
				cmdText += "#";
			}
			leftTime = GetQuestFlag(it->second.second.c_str()) - time(0);
			if (leftTime > 0)
			{
				cmdText += std::to_string(it->first);
				cmdText += "|";
				cmdText += std::to_string(leftTime);
				cmdText += "|";
				cmdText += "1";
				cmdText += "#";
			}
		}
	}
	if (cmdText.length())
		ChatPacket(CHAT_TYPE_COMMAND, "TrackDungeonInfo %s", cmdText.c_str());
}
#endif

#ifdef ENABLE_EXCHANGE_LOG
bool CHARACTER::LoadExchangeLogItem(DWORD logID)
{
	const auto it = m_mapExchangeLog.find(logID);
	if (it != m_mapExchangeLog.end())
	{
		if (!it->second.first.itemsLoaded)
		{
			it->second.second.clear();
			char szQuery[124];
			snprintf(szQuery, sizeof(szQuery), "SELECT * FROM log.exchange_log_items WHERE id = %u AND (pid = %u OR pid = %u)", logID, it->second.first.ownerPID, it->second.first.targetPID);
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
			if (pMsg && pMsg->Get()->uiNumRows != 0)
			{
				std::vector<TExchangeLogItem> logItemVector;
				MYSQL_ROW row;
				while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
				{
					DWORD itemOwnerPID;
					TExchangeLogItem logItem;
					BYTE col = 0;
					col++;//log_id
					str_to_number(itemOwnerPID, row[col++]);
					col++;//item_id
					str_to_number(logItem.pos, row[col++]);
					str_to_number(logItem.vnum, row[col++]);
					str_to_number(logItem.count, row[col++]);
					for (BYTE j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
						str_to_number(logItem.alSockets[j], row[col++]);
					for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; ++j) {
						str_to_number(logItem.aAttr[j].bType, row[col++]);
						str_to_number(logItem.aAttr[j].sValue, row[col++]);
					}
					logItem.isOwnerItem = it->second.first.ownerPID == itemOwnerPID;
					it->second.second.emplace_back(logItem);
				}
			}

			const WORD logItemCount = it->second.second.size();
			const BYTE subHeader = SUB_EXCHANGELOG_LOAD_ITEM;

			TPacketGCExchangeLog p;
			p.header = HEADER_GC_EXCHANGE_LOG;
			p.size = sizeof(TPacketGCExchangeLog) + sizeof(BYTE) + sizeof(WORD) + sizeof(DWORD) + (logItemCount*sizeof(TExchangeLogItem));

			TEMP_BUFFER buf;
			buf.write(&p, sizeof(TPacketGCExchangeLog));
			buf.write(&subHeader, sizeof(BYTE));
			buf.write(&logItemCount, sizeof(WORD));
			buf.write(&logID, sizeof(DWORD));
			if(logItemCount)
				buf.write(it->second.second.data(), logItemCount * sizeof(TExchangeLogItem));
			GetDesc()->Packet(buf.read_peek(), buf.size());

			return true;
		}
	}
	return false;
}
void CHARACTER::DeleteExchangeLog(DWORD logID)
{
	if (logID == 0)
	{
		for (auto it = m_mapExchangeLog.begin(); it != m_mapExchangeLog.end(); ++it)
		{
			char szQuery[124];
			snprintf(szQuery, sizeof(szQuery), "UPDATE log.exchange_log SET %s = 1 WHERE id = %u", it->second.first.ownerPID == GetPlayerID() ? "owner_delete" : "target_delete", it->first);
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
		}
		m_mapExchangeLog.clear();
	}
	else
	{
		auto it = m_mapExchangeLog.find(logID);
		if (it != m_mapExchangeLog.end())
		{
			char szQuery[124];
			snprintf(szQuery, sizeof(szQuery), "UPDATE log.exchange_log SET %s = 1 WHERE id = %u", it->second.first.ownerPID == GetPlayerID() ? "owner_delete" : "target_delete", logID);
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
			m_mapExchangeLog.erase(it);
		}
	}
}
void CHARACTER::LoadExchangeLog()
{
	if (GetProtectTime("ExchangeLogLoaded") == 1)
		return;
	m_mapExchangeLog.clear();

	char szQuery[256];
	snprintf(szQuery, sizeof(szQuery), "SELECT * FROM log.exchange_log WHERE (owner_pid = %d AND owner_delete = 0) OR (target_pid = %d AND target_delete = 0)", GetPlayerID(), GetPlayerID());
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));

	if (pMsg && pMsg->Get()->uiNumRows != 0)
	{
		std::vector<TExchangeLogItem> logItemVector;
		MYSQL_ROW row;
		while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
		{
			TExchangeLog exchangeLog;
			BYTE col = 0;
			DWORD logID;
			str_to_number(logID, row[col++]);
			strlcpy(exchangeLog.owner, row[col++], sizeof(exchangeLog.owner));
			str_to_number(exchangeLog.ownerPID, row[col++]);
			str_to_number(exchangeLog.ownerGold, row[col++]);
			strlcpy(exchangeLog.ownerIP, row[col++], sizeof(exchangeLog.ownerIP));

			strlcpy(exchangeLog.target, row[col++], sizeof(exchangeLog.target));
			str_to_number(exchangeLog.targetPID, row[col++]);
			str_to_number(exchangeLog.targetGold, row[col++]);
			strlcpy(exchangeLog.targetIP, row[col++], sizeof(exchangeLog.targetIP));
			strlcpy(exchangeLog.date, row[col++], sizeof(exchangeLog.date));
			col++;
			col++;
			exchangeLog.itemsLoaded = false;
			m_mapExchangeLog.emplace(logID, std::make_pair(exchangeLog, logItemVector));
		}
	}

	const WORD logCount = m_mapExchangeLog.size();
	const BYTE subHeader = SUB_EXCHANGELOG_LOAD;
	const bool isNeedClean = true;
	char playerCode[19];
	snprintf(playerCode, sizeof(playerCode), "%s", GetDesc()->GetAccountTable().social_id);

	TPacketGCExchangeLog p;
	p.header = HEADER_GC_EXCHANGE_LOG;
	p.size = sizeof(TPacketGCExchangeLog) + sizeof(BYTE) + sizeof(WORD) + (logCount*(sizeof(DWORD) + sizeof(TExchangeLog)));

	TEMP_BUFFER buf;
	buf.write(&p, sizeof(TPacketGCExchangeLog));
	buf.write(&subHeader, sizeof(BYTE));
	buf.write(&playerCode, sizeof(playerCode));
	buf.write(&isNeedClean, sizeof(bool));
	buf.write(&logCount, sizeof(WORD));
	for (auto it = m_mapExchangeLog.begin(); it != m_mapExchangeLog.end(); ++it)
	{
		buf.write(&it->first, sizeof(DWORD));
		buf.write(&it->second.first, sizeof(TExchangeLog));
	}
	GetDesc()->Packet(buf.read_peek(), buf.size());

	SetProtectTime("ExchangeLogLoaded", 1);
}
void CHARACTER::SendExchangeLogPacket(BYTE subHeader, DWORD id, const TExchangeLog* exchangeLog)
{
	if (!GetDesc())
		return;

	if (SUB_EXCHANGELOG_LOAD_ITEM == subHeader)
		LoadExchangeLogItem(id);
	else if (SUB_EXCHANGELOG_LOAD == subHeader)
	{
		if (exchangeLog)
		{
			if (GetProtectTime("ExchangeLogLoaded") != 1)
				return;
			std::vector<TExchangeLogItem> logItemVector;
			TExchangeLog exchangeLogEx;
			std::memcpy(&exchangeLogEx, exchangeLog, sizeof(exchangeLogEx));
			m_mapExchangeLog.emplace(id, std::make_pair(exchangeLogEx, logItemVector));

			const WORD logCount = 1;
			const bool isNeedClean = false;
			char playerCode[19];
			snprintf(playerCode, sizeof(playerCode),"%s", GetDesc()->GetAccountTable().social_id);

			TPacketGCExchangeLog p;
			p.header = HEADER_GC_EXCHANGE_LOG;
			p.size = sizeof(TPacketGCExchangeLog) + sizeof(BYTE) + sizeof(WORD) + (logCount*(sizeof(DWORD) + sizeof(TExchangeLog)));

			TEMP_BUFFER buf;
			buf.write(&p, sizeof(TPacketGCExchangeLog));
			buf.write(&subHeader, sizeof(BYTE));
			buf.write(&playerCode, sizeof(playerCode));
			buf.write(&isNeedClean, sizeof(bool));
			buf.write(&logCount, sizeof(WORD));
			buf.write(&id, sizeof(DWORD));
			buf.write(&exchangeLogEx, sizeof(TExchangeLog));
			GetDesc()->Packet(buf.read_peek(), buf.size());
		}
		else
			LoadExchangeLog();
	}
}
#endif

#ifdef __GEM_SYSTEM__
void CHARACTER::RefreshGemPlayer()
{
	if (!m_mapGemItems.size())
	{
		LoadGemItems();
		if (!m_mapGemItems.size())
			RefreshGemItems();
	}

	ChatPacket(CHAT_TYPE_COMMAND, "GemClear");	
	std::string cmdString("");
	
#if __cplusplus < 199711L
	for (itertype(m_mapGemItems) it = m_mapGemItems.begin(); it != m_mapGemItems.end(); ++it)
	{
		const BYTE& slotPos = it->first;
		const TGemItem& itemData = it->second;
#else
	for (const auto& [slotPos, itemData] : m_mapGemItems)
	{
#endif
		cmdString += std::to_string(slotPos).c_str();
		cmdString += "#";
		cmdString += std::to_string(itemData.itemVnum).c_str();
		cmdString += "#";
		cmdString += std::to_string(itemData.itemCount).c_str();
		cmdString += "#";
		cmdString += std::to_string(itemData.itemPrice).c_str();
		cmdString += "#";
		cmdString += std::to_string(itemData.itemBuyed).c_str();
		cmdString += "|";
		if (cmdString.size() + 50 > CHAT_MAX_LEN)
		{
			ChatPacket(CHAT_TYPE_COMMAND, "GemSetItemsWithString %s", cmdString.c_str());
			cmdString.clear();
		}
	}
	if(cmdString.size())
		ChatPacket(CHAT_TYPE_COMMAND, "GemSetItemsWithString %s", cmdString.c_str());
}

void CHARACTER::BuyGemItem(BYTE slotIndex)
{
#if __cplusplus < 199711L
	itertype(m_mapGemItems) it = m_mapGemItems.find(slotIndex);
#else
	auto it = m_mapGemItems.find(slotIndex);
#endif
	if (it != m_mapGemItems.end())
	{		
		if (it->second.itemPrice > GetGem())
			return;
		
		PointChange(POINT_GEM, -it->second.itemPrice);
		AutoGiveItem(it->second.itemVnum, it->second.itemCount);
		SaveGemItems();
	}
}

void CHARACTER::SaveGemItems()
{
	char filename[45];
	snprintf(filename, sizeof(filename), "locale/germany/gem/%u", GetPlayerID());

	FILE* fp = fopen(filename, "w+");
	if (fp)
	{
		fprintf(fp, "index-vnum-count-price-buyed\n");
#if __cplusplus < 199711L
		for (itertype(m_mapGemItems) it = m_mapGemItems.begin(); it != m_mapGemItems.end(); ++it)
		{
			const BYTE& slotPos = it->first;
			const TGemItem& itemData = it->second;
			fprintf(fp, "%u %u %u %d %u\n", slotPos, itemData.itemVnum, itemData.itemCount, itemData.itemPrice, itemData.itemBuyed);
		}
#else
		for (const auto& [slotPos, itemData] : m_mapGemItems)
			fprintf(fp, "%u %u %u %d %u\n", slotPos, itemData.itemVnum, itemData.itemCount, itemData.itemPrice, itemData.itemBuyed);
#endif
		fclose(fp);
	}
}

void CHARACTER::LoadGemItems()
{
	m_mapGemItems.clear();
	char filename[45];
	snprintf(filename, sizeof(filename), "locale/germany/gem/%u", GetPlayerID());
	FILE* fp;
	if ((fp = fopen(filename, "r")) != NULL)
	{
		char	one_line[256];
		while (fgets(one_line, 256, fp))
		{
			std::vector<std::string> m_vec;
			split_argument(one_line, m_vec);
			if (m_vec.size() != 5)
				continue;
			TGemItem gemItem;
			BYTE slotIndex;
			str_to_number(slotIndex, m_vec[0].c_str());
			str_to_number(gemItem.itemVnum, m_vec[1].c_str());
			str_to_number(gemItem.itemCount, m_vec[2].c_str());
			str_to_number(gemItem.itemPrice, m_vec[3].c_str());
			str_to_number(gemItem.itemBuyed, m_vec[4].c_str());
#if __cplusplus < 199711L
			m_mapGemItems.insert(std::make_pair(slotIndex, gemItem));
#else
			m_mapGemItems.emplace(slotIndex, gemItem);
#endif
		}
		fclose(fp);
	}
}

void CHARACTER::RefreshGemItems()
{
	m_mapGemItems.clear();

#if __cplusplus < 199711L
	const DWORD gemItems[][3] = {
#else
	constexpr DWORD gemItems[][4] = {
#endif
	//	{item,count,price,lucky}
		{49331,1,250,100},
		{54160,1,5,100},
		{49330,1,250,100},
		{49334,1,3,100},
		{49333,1,3,100},
		{53008,1,10,100},
		{61030,1,100,100},
		{79714,1,25,100},
		{91116,100,100,100},
		{70071,1,30,100},
		{91116,25,25,100},
		{53290,1,25,100},
		{54157,1,5,100},
		{30750,1,400,100},
		{54154,1,5,100},
		{71224,1,10,100},
		{91116,1,1,100},
		{71501,1,10,100},
		{79715,1,30,100},
		{49332,1,250,100},
		{61025,1,25,100},
		{59101,1,5,100},
		{59102,1,5,100},
		{49335,1,3,100},
		{91116,10,10,100},
	};

	const size_t listLen = _countof(gemItems);
	std::vector<BYTE> m_vecListIndex;
	while (1)
	{
		if (m_vecListIndex.size() >= listLen)
			break;
		for (WORD i = 0; i < listLen; ++i)
		{
			if (number(1, 100) < gemItems[i][3])
				continue;
			if (std::find(m_vecListIndex.begin(), m_vecListIndex.end(), i) != m_vecListIndex.end())
				continue;
#if __cplusplus < 199711L
			m_vecListIndex.push_back(i);
#else
			m_vecListIndex.emplace_back(i);
#endif
		}
	}

#if __cplusplus < 199711L
	for (auto it = m_vecListIndex.begin(); it != m_vecListIndex.end(); ++it)
	{
		const BYTE& listIndex = *it;
#else
	for(const auto& listIndex : m_vecListIndex)
	{
#endif
		TGemItem gemItem;
		gemItem.itemBuyed = false;
		gemItem.itemVnum = gemItems[listIndex][0];
		gemItem.itemCount = gemItems[listIndex][1];
		gemItem.itemPrice = gemItems[listIndex][2];
#if __cplusplus < 199711L
		m_mapGemItems.insert(std::make_pair(m_mapGemItems.size(), gemItem));
#else
		m_mapGemItems.emplace(m_mapGemItems.size(), gemItem);
#endif
	}
	SaveGemItems();
}
#endif

#ifdef ENABLE_VOTE4BUFF
long long CHARACTER::GetVoteCoin()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT votebuff FROM account.account WHERE id = '%d';", GetDesc()->GetAccountTable().id));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;
	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	long long coin = 0;
	str_to_number(coin, row[0]);
	return coin;
}
void CHARACTER::SetVoteCoin(long long amount)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE account.account SET votebuff = '%lld' WHERE hwid_routher = '%s';", amount, GetHardwareID()));
}
#endif

#ifdef RENEWAL_MISSION_BOOKS
void CHARACTER::SetMissionBook(BYTE missionType, BYTE value, DWORD arg, WORD level)
{
#if __cplusplus < 199711L
	for (itertype(m_mapMissionData) it = m_mapMissionData.begin(); it != m_mapMissionData.end(); ++it)
#else
	for (auto it = m_mapMissionData.begin(); it != m_mapMissionData.end(); ++it)
#endif
	{
#if __cplusplus < 199711L
		const TMissionBookData* missionData = CHARACTER_MANAGER::Instance().GetMissionData(it->first);
#else
		auto missionData = CHARACTER_MANAGER::Instance().GetMissionData(it->first);
#endif
		if (missionData)
		{
			if (missionData->type != missionType)
				continue;
			if (missionData->subtype != 0)
			{
				if (missionData->subtype != arg)
					continue;
			}
			else
			{
				if (!LEVEL_DELTA(level, GetLevel(), missionData->levelRange))
					continue;
			}
			if (it->second.value >= missionData->max || time(0) > it->second.leftTime)
				continue;
			it->second.value += value;
			
			if (it->second.value >= missionData->max)
			{
				it->second.endTime = time(0);
				it->second.value = missionData->max;

				char endTime[34];
				const time_t cur_Time = it->second.endTime;
				const struct tm vKey = *localtime(&cur_Time);
				snprintf(endTime, sizeof(endTime), "%02d:%02d:%02d-%02d/%02d/%02d", vKey.tm_hour, vKey.tm_min, vKey.tm_sec, vKey.tm_mday, vKey.tm_mon, vKey.tm_year + 1900);

				ChatPacket(CHAT_TYPE_COMMAND, "UpdateMissionEndTime %u %s", it->first, endTime);
				
				ChatPacket(CHAT_TYPE_INFO, "Your mission done!");
			}
			if (GetProtectTime("mission_load") == 1)
				ChatPacket(CHAT_TYPE_COMMAND, "UpdateMissionValue %u %lld", it->first, it->second.value);
		}
	}
}
void CHARACTER::RewardMissionBook(WORD missionID)
{
#if __cplusplus < 199711L
	const itertype(m_mapMissionData) it = m_mapMissionData.find(missionID);
#else
	const auto it = m_mapMissionData.find(missionID);
#endif
	if (it != m_mapMissionData.end())
	{
#if __cplusplus < 199711L
		const TMissionBookData* missionData = CHARACTER_MANAGER::Instance().GetMissionData(missionID);
#else
		auto missionData = CHARACTER_MANAGER::Instance().GetMissionData(missionID);
#endif
		if (missionData)
		{
			if(it->second.reward)
			{
				ChatPacket(CHAT_TYPE_INFO, "You already get reward!");
				return;
			}

			if (it->second.value >= missionData->max)
			{
				it->second.reward = true;
				SaveMissionData();
				for (BYTE j = 0; j < 6; ++j)
				{
					if (missionData->rewardItems[j] != 0)
						AutoGiveItem(missionData->rewardItems[j], missionData->rewardCount[j]);
				}
				PointChange(POINT_GOLD, number(missionData->gold[0], missionData->gold[1]));
				PointChange(POINT_EXP, number(missionData->exp[0], missionData->exp[1]));
				ChatPacket(CHAT_TYPE_INFO, "Succesfully get reward!");
				ChatPacket(CHAT_TYPE_COMMAND, "RewardMissionData %u %d", missionID, true);
			}
			else
				ChatPacket(CHAT_TYPE_INFO, "You need first finish mission!");
		}
	}
}
void CHARACTER::DeleteBookMission(WORD missionID)
{
#if __cplusplus < 199711L
	itertype(m_mapMissionData) it = m_mapMissionData.find(missionID);
#else
	auto it = m_mapMissionData.find(missionID);
#endif
	if (it != m_mapMissionData.end())
	{
		m_mapMissionData.erase(it);
		SaveMissionData();
		ChatPacket(CHAT_TYPE_COMMAND, "RemoveMissionData %u", missionID);
		ChatPacket(CHAT_TYPE_INFO, "Mission succesfully removed!");
	}
}
BYTE CHARACTER::MissionCount()
{
	return m_mapMissionData.size();
}
bool CHARACTER::IsMissionHas(WORD missionID)
{
#if __cplusplus < 199711L
	const itertype(m_mapMissionData) it = m_mapMissionData.find(missionID);
#else
	const auto it = m_mapMissionData.find(missionID);
#endif
	if (it != m_mapMissionData.end())
		return true;
	return false;
}
void CHARACTER::GiveNewMission(WORD missionID)
{
#if __cplusplus < 199711L
	const TMissionBookData* missionData = CHARACTER_MANAGER::Instance().GetMissionData(missionID);
#else
	auto missionData = CHARACTER_MANAGER::Instance().GetMissionData(missionID);
#endif
	if (!missionData)
		return;
	TMissionBook misson;
	memset(&misson,0,sizeof(misson));
	misson.leftTime = time(0)+ missionData->maxTime;
#if __cplusplus < 199711L
	m_mapMissionData.insert(std::make_pair(missionID, misson));
#else
	m_mapMissionData.emplace(missionID, misson);
#endif
	SaveMissionData();

	std::string cmdText("");
	ModifySetMissionCMD(missionID, cmdText);
	if (cmdText.length())
		ChatPacket(CHAT_TYPE_COMMAND, "UpdateMissionInfo %s", cmdText.c_str());
}
void CHARACTER::SaveMissionData()
{
	char filename[124];
	snprintf(filename, sizeof(filename), "locale/missionbook/%u", GetPlayerID());
	FILE* fp = fopen(filename, "w+");
	if (fp)
	{
		fprintf(fp, "missionid-value-lefttime-reward\n");
#if __cplusplus < 199711L
		for (itertype(m_mapMissionData) it = m_mapMissionData.begin(); it != m_mapMissionData.end(); ++it)
		{
			const BYTE& missionID = it->first;
			const TMissionBook& missionData = it->second;
			fprintf(fp, "%u %lld %d %u %d\n", missionID, missionData.value, missionData.leftTime, missionData.reward, missionData.endTime);
		}
#else
		for (const auto& [missionID, missionData] : m_mapMissionData)
			fprintf(fp, "%u %lld %d %u %d\n", missionID, missionData.value, missionData.leftTime, missionData.reward, missionData.endTime);
#endif
		fclose(fp);
	}
}

void CHARACTER::ModifySetMissionCMD(WORD missionID, std::string& cmdText)
{
#if __cplusplus < 199711L
	const itertype(m_mapMissionData) it = m_mapMissionData.find(missionID);
#else
	const auto it = m_mapMissionData.find(missionID);
#endif
	if (it != m_mapMissionData.end())
	{
#if __cplusplus < 199711L
		const TMissionBookData* missionData = CHARACTER_MANAGER::Instance().GetMissionData(it->first);
#else
		auto missionData = CHARACTER_MANAGER::Instance().GetMissionData(it->first);
#endif
		if (missionData)
		{
			char endTime[34];
			if (it->second.endTime != 0)
			{
				const time_t cur_Time = it->second.endTime;
				const struct tm vKey = *localtime(&cur_Time);
				snprintf(endTime, sizeof(endTime), "%02d:%02d:%02d-%02d/%02d/%02d", vKey.tm_hour, vKey.tm_min, vKey.tm_sec, vKey.tm_mday, vKey.tm_mon, vKey.tm_year + 1900);
			}
			else
				snprintf(endTime, sizeof(endTime), "-");
			cmdText += std::to_string(it->first);
			cmdText += "#";
			cmdText += std::to_string(missionData->missionItemIndex);
			cmdText += "#";
			cmdText += std::to_string(missionData->type);
			cmdText += "#";
			cmdText += std::to_string(missionData->subtype);
			cmdText += "#";
			cmdText += std::to_string(missionData->levelRange);
			cmdText += "#";
			cmdText += std::to_string(it->second.value);
			cmdText += "#";
			cmdText += std::to_string(missionData->max);
			cmdText += "#";
			cmdText += std::to_string(it->second.leftTime-time(0));
			cmdText += "#";
			cmdText += std::to_string(missionData->maxTime);
			cmdText += "#";
			cmdText += endTime;
			cmdText += "#";
			cmdText += std::to_string(missionData->gold[0]);
			cmdText += "#";
			cmdText += std::to_string(missionData->gold[1]);
			cmdText += "#";
			cmdText += std::to_string(missionData->exp[0]);
			cmdText += "#";
			cmdText += std::to_string(missionData->exp[1]);
			cmdText += "#";
			cmdText += std::to_string(it->second.reward);
			cmdText += "#";
			BYTE rewardCount = 0;
			for (BYTE j = 0; j < 6; ++j)
			{
				if ((missionData->rewardItems[j] == 0 && j != 0) || j == 5)
				{
					rewardCount = j + 1;
					break;
				}
			}
			cmdText += std::to_string(rewardCount);
			cmdText += "#";
			for (BYTE j = 0; j < rewardCount; ++j)
			{
				cmdText += std::to_string(missionData->rewardItems[j]);
				cmdText += "#";
				cmdText += std::to_string(missionData->rewardCount[j]);
				cmdText += "#";
			}
		}
		else
		{
			sys_err("Player has %d id mission but not has remove in data!", it->first);
		}
	}
}
void CHARACTER::SendMissionData()
{
	if (GetProtectTime("mission_load") == 1)
		return;
	ChatPacket(CHAT_TYPE_COMMAND, "ClearBookMission");
	std::string cmdText("");
#if __cplusplus < 199711L
	for (itertype(m_mapMissionData) it = m_mapMissionData.begin(); it != m_mapMissionData.end(); ++it)
#else
	for (auto it = m_mapMissionData.begin(); it != m_mapMissionData.end(); ++it)
#endif
	{
		ModifySetMissionCMD(it->first, cmdText);
		if (cmdText.length())
		{
			ChatPacket(CHAT_TYPE_COMMAND, "UpdateMissionInfo %s", cmdText.c_str());
			cmdText.clear();
		}
	}
	SetProtectTime("mission_load", 1);
}
void CHARACTER::LoadMissionData()
{
	m_mapMissionData.clear();
	char filename[124];
	snprintf(filename, sizeof(filename), "locale/missionbook/%u", GetPlayerID());
	FILE* fp;
	if ((fp = fopen(filename, "r")) != NULL)
	{
		char	one_line[256];
		while (fgets(one_line, 256, fp))
		{
			std::vector<std::string> m_vec;
			split_argument(one_line, m_vec);
			if (m_vec.size() != 5)
				continue;
			TMissionBook missionItem;
			WORD missionID;
			str_to_number(missionID, m_vec[0].c_str());
			str_to_number(missionItem.value, m_vec[1].c_str());
			str_to_number(missionItem.leftTime, m_vec[2].c_str());
			str_to_number(missionItem.reward, m_vec[3].c_str());
			str_to_number(missionItem.endTime, m_vec[4].c_str());
#if __cplusplus < 199711L
			m_mapMissionData.insert(std::make_pair(missionID, missionItem));
#else
			m_mapMissionData.emplace(missionID, missionItem);
#endif
		}
		fclose(fp);
	}
	
}
#endif
#ifdef __FAKE_PLAYER__
void CHARACTER::FakePlayerCreatePetSystem()
{
	if (m_petSystem)
	{
		m_petSystem->Destroy();
		delete m_petSystem;
	}

	m_petSystem = M2_NEW CPetSystem(this);
}
void CHARACTER::FakePlayerCreateMountSystem()
{
	return;
	// if (m_mountSystem)
	// {
		// m_mountSystem->Destroy();
		// delete m_mountSystem;
	// }

	// m_mountSystem = M2_NEW CMountSystem(this);
}

#endif

#ifdef __RENEWAL_BRAVE_CAPE__
struct FuncAggregateMonsterNew
{
	LPCHARACTER m_ch;
	int m_dwRange;
	FuncAggregateMonsterNew(LPCHARACTER ch, int range) : m_ch(ch), m_dwRange(range){}
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (!ch->IsMonster() || ch->GetVictim())
				return;
			if (DISTANCE_APPROX(ch->GetX() - m_ch->GetX(), ch->GetY() - m_ch->GetY()) < m_dwRange)
			{
				if (ch->CanBeginFight())
					ch->BeginFight(m_ch);
			}
		}
	}
};
EVENTFUNC(brave_cape_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == NULL)
	{
		sys_err("<brave_cape_event> <Factor> Null pointer");
		return 0;
	}
	LPCHARACTER	ch = info->ch;
	if (ch == NULL)
		return 0;
	if (!ch->FindAffect(AFFECT_BRAVE_CAPE))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Brave affect is done loop event closed.");
		ch->m_pkBraveCapeEvent = NULL;
		return 0;
	}

	LPSECTREE pSec = ch->GetSectree();
	if (pSec)
	{
#ifdef USE_BRAVE_CAPE_RENEWED
		if (ch->IsDead() || ch->IsObserverMode())
		{
			return PASSES_PER_SEC(1);
		}

		if (get_dword_time() - ch->GetLastReviveTime() <= 7500)
		{
			return PASSES_PER_SEC(1);
		}
#endif

		FuncAggregateMonsterNew f(ch, ch->GetProtectTime("brave_cape_range"));
		pSec->ForEachAround(f);
	}

	return PASSES_PER_SEC(ch->GetProtectTime("brave_cape_time"));
}
void CHARACTER::GetBraveCapeCMDCompare(const char* c_pszArgument)
{
	std::vector<std::string> vecArgs;
	split_argument(c_pszArgument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "active")
	{
		if (vecArgs.size() < 4) { return; }
		if (!FindAffect(AFFECT_BRAVE_CAPE))
		{
			ChatPacket(CHAT_TYPE_INFO, "You don't has brave cape affect.");
			return;
		}
		if (m_pkBraveCapeEvent)
		{
			ChatPacket(CHAT_TYPE_INFO, "Already brave cape working..");
			return;
		}
		const std::vector<WORD> m_dwBlockedMapIdx = { 1, 21, 41 };
		if (std::find(m_dwBlockedMapIdx.begin(), m_dwBlockedMapIdx.end(), GetMapIndex()) != m_dwBlockedMapIdx.end())
		{
			ChatPacket(CHAT_TYPE_INFO, "You can't use brave cape on this map!");
			return;
		}

		int loopTime, eventRange;
		if (!str_to_number(loopTime, vecArgs[2].c_str()) || !str_to_number(eventRange, vecArgs[3].c_str()))
			return;

		if (loopTime < 2 || loopTime > 60 || eventRange < 1000 || eventRange > 15000)
			return;

		SetProtectTime("brave_cape_time", loopTime);
		SetProtectTime("brave_cape_range", eventRange);

		char_event_info* info = AllocEventInfo<char_event_info>();
		info->ch = this;
		m_pkBraveCapeEvent = event_create(brave_cape_event, info, 0);
		ChatPacket(CHAT_TYPE_COMMAND, "SetBraveCapeStatus 1");
		ChatPacket(CHAT_TYPE_INFO, "Brave cape event started..");

	}
	else if (vecArgs[1] == "deactive")
	{
		if (!m_pkBraveCapeEvent)
		{
			ChatPacket(CHAT_TYPE_INFO, "You don't has brave cape loop event!");
			return;
		}
		SetProtectTime("brave_cape_time", 0);
		SetProtectTime("brave_cape_range", 0);
		event_cancel(&m_pkBraveCapeEvent);
		m_pkBraveCapeEvent = NULL;
		ChatPacket(CHAT_TYPE_INFO, "Brave cape event closed..");
		ChatPacket(CHAT_TYPE_COMMAND, "SetBraveCapeStatus 0");
	}
}
#endif

#ifdef ENABLE_RANKING
long long CHARACTER::GetRankPoints(int iArg)
{
	if ((iArg < 0) || (iArg >= RANKING_MAX_CATEGORIES))
		return 0;
	
	return m_lRankPoints[iArg];
}

void CHARACTER::SetRankPoints(int iArg, long long lPoint)
{
	if ((iArg < 0) || (iArg >= RANKING_MAX_CATEGORIES))
		return;
	
	/*
	iArg : 0	- chi ha vinto piu duelli -
	iArg : 1	- chi ha perso piu duelli -
	iArg : 2	- m. m. di uccisioni in guerra -
	iArg : 3	- d. medi maggiori vs player -
	iArg : 4	- d. abilita maggiore vs player -
	iArg : 5	- chi ha distrutto piu metin -
	iArg : 6	- chi ha ucciso piu mostri -
	iArg : 7	- chi ha ucciso piu boss -
	iArg : 8	- d. medi maggiori vs boss -
	iArg : 9	- d. abilita maggiori vs boss -
	iArg : 10	- chi ha raccolto piu yang -
	iArg : 11	- chi ha raccolto piu gaya -
	iArg : 12	- chi ha usato piu i. oggetto -
	iArg : 13	- chi ha usato piu i. talismani -
	iArg : 14	- chi ha pescato piu pesci -
	iArg : 15	- m. numero di tempo in game -
	iArg : 16	- piu dungeon completati -
	iArg : 17	- chi ha aperto piu forzieri -
	iArg : 18	- d. medio mas. vs mob -
	iArg : 19	- d. abilita mas. vs mob -
	*/
	m_lRankPoints[iArg] = lPoint;
	Save();
}

void CHARACTER::RankingSubcategory(int iArg)
{
	if (!GetDesc())
		return;
	
	if ((iArg < 0) || (iArg >= RANKING_MAX_CATEGORIES))
		return;
	
	TPacketGCRankingTable p;
	int j = 0;
	
	char szQuery1[1024] = {0};
	snprintf(szQuery1, sizeof(szQuery1), "SELECT account_id, level, name, r%d FROM player.player%s WHERE account_id=(SELECT id FROM account.account%s WHERE status='OK' AND id=account_id) AND name not in(SELECT mName FROM common.gmlist%s) ORDER BY r%d desc, level desc, name asc LIMIT 50", iArg, get_table_postfix(), get_table_postfix(), get_table_postfix(), iArg);
	std::unique_ptr<SQLMsg> pRes1(DBManager::instance().DirectQuery(szQuery1));
	int iRes = pRes1->Get()->uiNumRows;
	if (iRes > 0) {
		MYSQL_ROW data;
		while ((data = mysql_fetch_row(pRes1->Get()->pSQLResult))) {
			int col = 1;
			p.list[j].iPosition = j;
			p.list[j].iRealPosition = 0;
			p.list[j].iLevel = atoi(data[col++]);
			strlcpy(p.list[j].szName, data[col++], sizeof(p.list[j].szName));
			p.list[j].iPoints = atoi(data[col]);
			j += 1;
		}
	}
	
	if (j < MAX_RANKING_LIST) {
		for (int i = j; i < MAX_RANKING_LIST; i++) {
			p.list[i].iPosition = i;
			p.list[i].iRealPosition = 0;
			p.list[i].iLevel = 0;
			p.list[i].iPoints = 0;
			strlcpy(p.list[i].szName, "", sizeof(p.list[i].szName));
		}
	}
	
	char szQuery2[1024] = {0};
	if (GetGMLevel() > GM_PLAYER) {
		snprintf(szQuery2, sizeof(szQuery2), "SELECT * FROM (SELECT @rank:=0) a, (SELECT @rank:=@rank+1 r, r%d, name, level FROM player.player%s AS res ORDER BY r%d desc, level desc, name asc) as custom WHERE name='%s'", iArg, get_table_postfix(), iArg, GetName());
	} else {
		snprintf(szQuery2, sizeof(szQuery2), "SELECT * FROM (SELECT @rank:=0) a, (SELECT @rank:=@rank+1 r, r%d, name, level FROM player.player%s AS res WHERE name not in(SELECT mName FROM common.gmlist) ORDER BY r%d desc, level desc, name asc) as custom WHERE name='%s'", iArg, get_table_postfix(), iArg, GetName());
	}
	std::unique_ptr<SQLMsg> pRes2(DBManager::instance().DirectQuery(szQuery2));
	iRes = pRes2->Get()->uiNumRows;
	if (iRes > 0) {
		j = MAX_RANKING_LIST-1;
		MYSQL_ROW data = mysql_fetch_row(pRes2->Get()->pSQLResult);
		p.list[j].iPosition = j;
		p.list[j].iRealPosition = atoi(data[1]);
		p.list[j].iLevel = atoi(data[4]);
		p.list[j].iPoints = atoi(data[2]);
		strlcpy(p.list[j].szName, GetName(), sizeof(p.list[j].szName));
	}
	
	GetDesc()->Packet(&p, sizeof(p));
}
#endif

#ifdef __BUFFI_SUPPORT__
void CHARACTER::SendBuffiSkillAffect(DWORD targetVID, DWORD skillIdx, BYTE skillLevel)
{
	TPacketGCBuffiSkill pack;
	pack.header = HEADER_GC_BUFFI_SKILL;
	pack.dwSkillIdx = skillIdx;
	pack.dwVID = GetVID();
	pack.dwTargetVID = targetVID;
	pack.bLevel = skillLevel;
	PacketView(&pack, sizeof(TPacketGCBuffiSkill), this);
}
void CHARACTER::SetBuffi(bool bFlag)
{
	m_bIsSupport = bFlag;
	m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateBuffi, &CHARACTER::EndStateEmpty);
	m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateMove, &CHARACTER::EndStateEmpty);
	m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateBuffi, &CHARACTER::EndStateEmpty);
}
#endif

bool CHARACTER::IsLastMoveItemTime() const
{
	const auto timeval = 0.5f;
	return (get_dword_time() < m_dwLastMoveItemTime + static_cast<uint32_t>(timeval * 1000.0f));
}

#ifdef __DUNGEON_INFO__
int CHARACTER::GetQuestFlagSpecial(const char* szQuestFlag, ...)
{
	char szBuff[254];
	va_list args;
	va_start(args, szQuestFlag);
	vsnprintf(szBuff, sizeof(szBuff), szQuestFlag, args);
	va_end(args);
	return GetQuestFlag(szBuff);
}

void CHARACTER::SendDungeonCooldown(DWORD bossIdx)
{
	std::string cmd("");
	
	std::vector<DWORD> m_dwBossListIdx;

	if (!bossIdx)
	{
		const int playerLevel = GetLevel();
		for (auto it = m_mapDungeonList.begin(); it != m_mapDungeonList.end(); ++it)
			if (playerLevel >= it->second.first.first  && playerLevel <= it->second.first.second)
				m_dwBossListIdx.emplace_back(it->first);
	}
	else
	{
		if (m_mapDungeonList.find(bossIdx) == m_mapDungeonList.end())
			return;
		m_dwBossListIdx.emplace_back(bossIdx);
	}

	for (BYTE j = 0; j < m_dwBossListIdx.size(); ++j)
	{
		const DWORD& bossIdx = m_dwBossListIdx[j];
		const auto it = m_mapDungeonList.find(bossIdx);
		if (it == m_mapDungeonList.end())
			continue;

		cmd += std::to_string(bossIdx);
		cmd += "|";

		cmd += std::to_string(GetQuestFlagSpecial("%s.cooldown", it->second.second.c_str()) - time(0));
		cmd += "|";

		cmd += std::to_string(GetQuestFlagSpecial("dungeon.%u_completed", bossIdx));
		cmd += "|";

		cmd += std::to_string(GetQuestFlagSpecial("dungeon.%u_fastest", bossIdx));
		cmd += "|";

		cmd += std::to_string(GetQuestFlagSpecial("dungeon.%u_damage", bossIdx));
		cmd += "#";
	}
	if (cmd == "")
		cmd = "-";
	ChatPacket(CHAT_TYPE_COMMAND, "dungeon_info_cooldown %d %s", (bossIdx == 0) ? 1 : 0, cmd.c_str());
}
#endif

#ifdef __AUTO_SKILL_READER__
BYTE CHARACTER::GetSkillGradeByLevel(int iSkillLevel)
{
	int grade = 0;

	if (iSkillLevel >= 40)
	{
		grade = SKILL_PERFECT_MASTER;
	}
	else if (iSkillLevel >= 30 && iSkillLevel <= 39)
	{
		grade = SKILL_GRAND_MASTER;
	}
	else if (iSkillLevel >= 20 && iSkillLevel <= 29)
	{
		grade = SKILL_MASTER;
	}
	else if (iSkillLevel >= 1 && iSkillLevel <= 19)
	{
		grade = SKILL_NORMAL;
	}

	return grade;
}

bool CHARACTER::SkillToBook(BYTE skillIdx, DWORD& itemIdx, DWORD& socket0, DWORD& exorcismIdx, DWORD& concentratedIdx)
{
	const BYTE skillLevel = GetSkillLevel(skillIdx);
	const auto skillGrade = GetSkillGradeByLevel(skillLevel);

	if (skillIdx <= 111)
	{
		if (skillGrade <= 2)
		{
			exorcismIdx = 71001;
			concentratedIdx = 71094;
		}

		if (skillGrade == 1)
		{
			itemIdx = USE_YMIR_50300_SKILLBOOK ? 50300 : 50400 + skillIdx;
			socket0 = skillIdx;
			return true;
		}

		else if (skillGrade == 2)
		{
			itemIdx = 50513;
			return true;
		}
	}
	else
	{
		const static std::map<BYTE, std::pair<DWORD, DWORD>> m_mapExorcismandConcentrated = {
			//{SKILL_IDX, {exorcismIdx, exorcismIdx}},
			{SKILL_COMBO, {71001, 71094}},
			{SKILL_LEADERSHIP, {71001, 71094}},
			{SKILL_POLYMORPH, {71001, 71094}},
			{SKILL_MINING, {71001, 71094}},
#ifdef __BUFFI_SUPPORT__
			{SKILL_BUFFI_1, {71001, 71094}},
			{SKILL_BUFFI_2, {71001, 71094}},
			{SKILL_BUFFI_3, {71001, 71094}},
			{SKILL_BUFFI_4, {71001, 71094}},
#endif
#ifdef USE_PASSIVE_ABILITY_TAMER
			{PASSIVE_ABILITY_VNUM_TAMER_1, {50973, 50974}},
			{PASSIVE_ABILITY_VNUM_TAMER_2, {50973, 50974}},
			{PASSIVE_ABILITY_VNUM_TAMER_3, {50973, 50974}},
			{PASSIVE_ABILITY_VNUM_TAMER_4, {50973, 50974}},
#endif
		};

		const auto itNew = m_mapExorcismandConcentrated.find(skillIdx);
		if (itNew != m_mapExorcismandConcentrated.end())
		{
			exorcismIdx = itNew->second.first;
			concentratedIdx = itNew->second.second;
		}

		//if skill has 1 book put here
		const static std::map<BYTE, DWORD> __skillBookItemList ={
			{ SKILL_COMBO, 50304 },
			{ SKILL_MINING, 50600 },
		};
		const auto it = __skillBookItemList.find(skillIdx);
		if (it != __skillBookItemList.end())
		{
			itemIdx = it->second;
			return true;
		}

		//#if skill different book for every stage(M | G | P)
		const static std::map<BYTE, std::map<BYTE, DWORD>> __skillBookItemListEx = {
			{
				SKILL_LEADERSHIP, //skillIdx
				{
					//book via grade
					{SKILL_GRADE_MASTER, 50301},
					{SKILL_GRADE_GRAND_MASTER, 50302},
					{SKILL_GRADE_PERFECT_MASTER, 50303},
				}
			},
			{
				SKILL_POLYMORPH,//skillIdx
				{
					//book via grade
					{SKILL_GRADE_MASTER, 50314},
					{SKILL_GRADE_GRAND_MASTER, 50315},
					{SKILL_GRADE_PERFECT_MASTER, 50316},
				}
			},
#ifdef __BUFFI_SUPPORT__
			{
				SKILL_BUFFI_1,
				{
					{SKILL_GRADE_MASTER, 0},
					{SKILL_GRADE_GRAND_MASTER, 61035},
					{SKILL_GRADE_PERFECT_MASTER, 61031},
				}
			},
			{
				SKILL_BUFFI_2,
				{
					{SKILL_GRADE_MASTER, 0},
					{SKILL_GRADE_GRAND_MASTER, 61036},
					{SKILL_GRADE_PERFECT_MASTER, 61031},
				}
			},
			{
				SKILL_BUFFI_3,
				{
					{SKILL_GRADE_MASTER, 0},
					{SKILL_GRADE_GRAND_MASTER, 61037},
					{SKILL_GRADE_PERFECT_MASTER, 61031},
				}
			},
			{
				SKILL_BUFFI_4,
				{
					{SKILL_GRADE_MASTER, 0},
					{SKILL_GRADE_GRAND_MASTER, 61038},
					{SKILL_GRADE_PERFECT_MASTER, 61031},
				}
			},
#endif
#ifdef USE_PASSIVE_ABILITY_TAMER
			{
				PASSIVE_ABILITY_VNUM_TAMER_1,
				{
					{SKILL_GRADE_MASTER, 50940},
					{SKILL_GRADE_GRAND_MASTER, 50941},
					{SKILL_GRADE_PERFECT_MASTER, 50942},
				}
			},
			{
				PASSIVE_ABILITY_VNUM_TAMER_2,
				{
					{SKILL_GRADE_MASTER, 50950},
					{SKILL_GRADE_GRAND_MASTER, 50951},
					{SKILL_GRADE_PERFECT_MASTER, 50952},
				}
			},
			{
				PASSIVE_ABILITY_VNUM_TAMER_3,
				{
					{SKILL_GRADE_MASTER, 50960},
					{SKILL_GRADE_GRAND_MASTER, 50961},
					{SKILL_GRADE_PERFECT_MASTER, 50962},
				}
			},
			{
				PASSIVE_ABILITY_VNUM_TAMER_4,
				{
					{SKILL_GRADE_MASTER, 50970},
					{SKILL_GRADE_GRAND_MASTER, 50971},
					{SKILL_GRADE_PERFECT_MASTER, 50972},
				}
			},
#endif
		};

		const auto itEx = __skillBookItemListEx.find(skillIdx);
		if (itEx != __skillBookItemListEx.end())
		{
			const auto itExEx = itEx->second.find(skillGrade);
			if (itExEx != itEx->second.end())
			{
#ifdef __BUFFI_SUPPORT__
				if (itExEx->second == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot learn yet this skill."));
					return false;
				}
#endif

				itemIdx = itExEx->second;
				return true;
			}
		}
	}
	return false;
}
bool CHARACTER::ReadSkill(BYTE skillIdx)
{
	if (!GetSkillGroup() || IsPolymorphed())
	{
		return false;
	}

	const CSkillProto* pkSk = CSkillManager::instance().Get(skillIdx);
	if (!pkSk)
	{
		return false;
	}

	DWORD itemIdx = 0, exorcismIdx = 0, concentratedIdx = 0, socket0 = 0;
	if (!SkillToBook(skillIdx, itemIdx, socket0, exorcismIdx, concentratedIdx))
	{
		return false;
	}

	LPITEM book = NULL;

	if (itemIdx == 50300)
	{
		for (WORD i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			book = GetBookInventoryItem(i);
			if (!book || book->GetVnum() != itemIdx || (socket0 != 0 && socket0 != book->GetSocket(0)))
				continue;
			break;
		}
		
		if (!book)
		{
			for (WORD i = 0; i < INVENTORY_MAX_NUM; ++i)
			{
				book = GetInventoryItem(i);
				if (!book || book->GetVnum() != itemIdx || (socket0 != 0 && socket0 != book->GetSocket(0)))
					continue;
				break;
			}
		}
	}
	else
	{
		book = FindSpecifyItem(itemIdx);

		/*for (WORD i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			book = GetBookInventoryItem(i);
			if (!book || book->GetVnum() != itemIdx) // || (socket0 != 0 && socket0 != book->GetSocket(0)))
				continue;
			break;
		}*/
	}
		

	if (!book)
	{
		ChatPacket(CHAT_TYPE_INFO, "Not has enought specific item!");
		return false;
	}

	const time_t now = get_global_time();
	const BYTE skillLevel = GetSkillLevel(skillIdx);
	const BYTE skillGrade = GetSkillGradeByLevel(skillLevel);

	// SOUL_STONE
	if (itemIdx == 50513
#ifdef __BUFFI_SUPPORT__
 || itemIdx == 61031
#endif
	)
	{
		if (now < GetSkillNextReadTime(skillIdx))
		{
			if (!FindAffect(AFFECT_SKILL_NO_BOOK_DELAY))
			{
				const TItemTable* itemTable = ITEM_MANAGER::Instance().GetTable(exorcismIdx);
				if (!itemTable || CountSpecifyItem(exorcismIdx) <= 0)
				{
					SkillLearnWaitMoreTimeMessage(GetSkillNextReadTime(skillIdx) - now);
					return false;
				}
				RemoveSpecifyItem(exorcismIdx);
				AddAffect(itemTable->alValues[0], aApplyInfo[itemTable->alValues[1]].bPointType, itemTable->alValues[2], 0, itemTable->alValues[3], 0, false);
			}
			else
				RemoveAffect(AFFECT_SKILL_NO_BOOK_DELAY);
		}

		int needAlignment = (1000 + 500 * (skillLevel - 30));
		const int currentAlignment = (GetRealAlignment() / 10);
		int resultAlignment = (-19000 + needAlignment);

		if (USE_DECREASE_ALIGNMENT_SOULSTONE)
		{
			if (currentAlignment < 0)
			{
				needAlignment *= 2;
				resultAlignment = (-19000 - needAlignment);
			}

			if (currentAlignment > resultAlignment)
			{
				int needZenBeanCount = needAlignment / 500;
				if (needZenBeanCount < 0)
					needZenBeanCount = 0 - needZenBeanCount;

				if (currentAlignment < 0)
				{
					if (!USE_ZEN_BEAN_SOULSTONE)
					{
						ChatPacket(1, "You can't do that with this alignment.");
						return false;
					}

					if (CountSpecifyItem(70102) < needZenBeanCount)
					{
						ChatPacket(1, "You not has enought zen bean.");
						return false;
					}

					UpdateAlignment(MIN(-GetAlignment(), 500) * needZenBeanCount);
					RemoveSpecifyItem(70102, needZenBeanCount);
				}
			}
		}
		
		if (!FindAffect(AFFECT_SKILL_BOOK_BONUS))
		{
			const TItemTable* itemTable = ITEM_MANAGER::Instance().GetTable(concentratedIdx);
			if (itemTable)
			{
				if (CountSpecifyItem(concentratedIdx))
				{
					RemoveSpecifyItem(concentratedIdx);
					AddAffect(itemTable->alValues[0], aApplyInfo[itemTable->alValues[1]].bPointType, itemTable->alValues[2], 0, itemTable->alValues[3], 0, false);
				}
			}
		}
		book->SetCount(book->GetCount() - 1);
		if (USE_DECREASE_ALIGNMENT_SOULSTONE)
			UpdateAlignment((needAlignment < 0) ? needAlignment * 10 : -needAlignment * 10);
		LearnGrandMasterSkill(skillIdx);
		SetSkillNextReadTime(skillIdx, USE_COOLTIME_ON_SOULSTONE ?  now + number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX) : 0);
		return true;
	}

#ifdef USE_PASSIVE_ABILITY_TAMER
	bool bIsTamerSkill = false;
#endif

	int iPct = 0;
	switch (skillIdx)
	{
		case SKILL_COMBO:
		{
			const int playerLevel = GetLevel();
			if (skillLevel == 0 && playerLevel < 30)
			{
				ChatPacket(CHAT_TYPE_INFO, "You need to have a minimum level of 30 to understand this book.");
				return false;
			}
			else if (skillLevel == 1 && playerLevel < 50)
			{
				ChatPacket(CHAT_TYPE_INFO, "You need a minimum level of 50 to understand this book.");
				return false;
			}
			else if (skillLevel >= 2)
			{
				ChatPacket(CHAT_TYPE_INFO, "You can't train any more Combos.");
				return false;
			}
			iPct = book->GetValue(0);
		}
		break;
		case SKILL_LEADERSHIP:
		{
			if (skillLevel < book->GetValue(0))
			{
				ChatPacket(CHAT_TYPE_INFO, "It isn't easy to understand this book.");
				return false;
			}
			else if (skillLevel >= book->GetValue(1))
			{
				ChatPacket(CHAT_TYPE_INFO, "This book will not help you.");
				return false;
			}
		}
		break;
		case SKILL_POLYMORPH:
		{
			if (GetLevel() < book->GetValue(3))
			{
				ChatPacket(CHAT_TYPE_INFO, "You have to improve your Level to read this Book.");
				return false;
			}
			else if (skillLevel >= 40)
			{
				ChatPacket(CHAT_TYPE_INFO, "You cannot train this skill.");
				return false;
			}
			else if (skillLevel < book->GetValue(0))
			{
				ChatPacket(CHAT_TYPE_INFO, "It isn't easy to understand this book.");
				return false;
			}
			else if (skillLevel >= book->GetValue(1))
			{
				ChatPacket(CHAT_TYPE_INFO, "You cannot train with this Book any more.");
				return false;
			}
			iPct = MINMAX(0, book->GetValue(2), 100);
		}
		break;
		case SKILL_MINING:
		{
			if (skillLevel >= 40)
			{
				ChatPacket(CHAT_TYPE_INFO, "You cannot train this skill.");
				return false;
			}
			iPct = MINMAX(0, book->GetValue(1), 100);
		}
		break;
#ifdef USE_PASSIVE_ABILITY_TAMER
		case PASSIVE_ABILITY_VNUM_TAMER_1:
		case PASSIVE_ABILITY_VNUM_TAMER_2:
		case PASSIVE_ABILITY_VNUM_TAMER_3:
		case PASSIVE_ABILITY_VNUM_TAMER_4:
		{
			if (skillLevel >= 40)
			{
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot train this ability anymore."));
				ChatPacket(CHAT_TYPE_INFO, "1561");
				return false;
			}
			else if ((book->GetValue(1) > 0 && skillLevel < book->GetValue(1)) || skillLevel >= book->GetValue(2))
			{
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot train this ability using this book."));
				ChatPacket(CHAT_TYPE_INFO, "1562");
				return false;
			}

			iPct = MINMAX(0, book->GetValue(3), 100);
			bIsTamerSkill = true;
		}
		break;
#endif
	}

	if (iPct < 0)
	{
		ChatPacket(CHAT_TYPE_INFO, "1549");
		// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something went wrong, please contact the staff."));
		return false;
	}
	else if (iPct > 100)
	{
		iPct = 100;
	}

	if (now < GetSkillNextReadTime(skillIdx))
	{
#ifdef USE_PASSIVE_ABILITY_TAMER
		DWORD dwSkillBonusDelay = bIsTamerSkill ? AFFECT_SKILL_TAMER_NO_BOOK_DELAY : AFFECT_SKILL_NO_BOOK_DELAY;

		if (!FindAffect(dwSkillBonusDelay))
#else
		if (!FindAffect(AFFECT_SKILL_NO_BOOK_DELAY))
#endif
		{
			const TItemTable* itemTable = ITEM_MANAGER::Instance().GetTable(exorcismIdx);
			if (!itemTable || CountSpecifyItem(exorcismIdx) <= 0)
			{
				SkillLearnWaitMoreTimeMessage(GetSkillNextReadTime(skillIdx) - now);
				return false;
			}

			RemoveSpecifyItem(exorcismIdx);
			AddAffect(itemTable->alValues[0], aApplyInfo[itemTable->alValues[1]].bPointType, itemTable->alValues[2], 0, itemTable->alValues[3], 0, false);
		}
	}

#ifdef USE_PASSIVE_ABILITY_TAMER
	DWORD dwSkillBonusAffect = bIsTamerSkill ? AFFECT_SKILL_TAMER_BOOK_BONUS : AFFECT_SKILL_BOOK_BONUS;

	if (iPct != 100 && !FindAffect(dwSkillBonusAffect))
#else
	if (iPct != 100 && !FindAffect(AFFECT_SKILL_BOOK_BONUS))
#endif
	{
		const TItemTable* itemTable = ITEM_MANAGER::Instance().GetTable(concentratedIdx);
		if (itemTable)
		{
			if (CountSpecifyItem(concentratedIdx))
			{
				RemoveSpecifyItem(concentratedIdx);
				AddAffect(itemTable->alValues[0], aApplyInfo[itemTable->alValues[1]].bPointType, itemTable->alValues[2], 0, itemTable->alValues[3], 0, false);
			}
		}
	}

	if (LearnSkillByBook(skillIdx, iPct
#ifdef USE_PASSIVE_ABILITY_TAMER
, bIsTamerSkill
#endif
	))
	{
		book->SetCount(book->GetCount() - 1);
		SetSkillNextReadTime(skillIdx, USE_COOLTIME_ON_BOOKS ? now + number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX) : 0);
		return true;
	}

	return false;
}

EVENTFUNC(auto_skill_read_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (!info)
	{
		sys_err("auto_skill_read_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER ch = info->ch;
	if (!ch)
	{
		sys_err("auto_skill_read_event> <Character> Null pointer");
		return 0;
	}

	if (ch->ReadSkill(ch->GetSelectedSkillIndex()))
	{
		return PASSES_PER_SEC(0.5);
	}

	ch->GetAutoSkill(ch->GetSelectedSkillIndex(), false, true);
	return 0;
}

void CHARACTER::GetAutoSkill(BYTE skillIdx, bool status, bool isFromEvent)
{
	const CSkillProto* pkSk = CSkillManager::instance().Get(skillIdx);
	if (!pkSk)
	{
		return;
	}

	if (status)
	{
		if (m_pkAutoSkill)
		{
			event_cancel(&m_pkAutoSkill);
			m_pkAutoSkill = NULL;
		}

		ChatPacket(CHAT_TYPE_COMMAND, "AutoSkillStatus 1");

		m_bSelectedSkillIdx = skillIdx;
		char_event_info* info = AllocEventInfo<char_event_info>();
		info->ch = this;

		m_pkAutoSkill = event_create(auto_skill_read_event, info, PASSES_PER_SEC(0.1));
	}
	else
	{
		m_bSelectedSkillIdx = 0;

		if (!isFromEvent && m_pkAutoSkill)
		{
			event_cancel(&m_pkAutoSkill);
			m_pkAutoSkill = NULL;
		}

		ChatPacket(CHAT_TYPE_COMMAND, "AutoSkillStatus 0");
	}
}
#endif

#ifdef __AUTO_HUNT__
void CHARACTER::SetAutoHuntStatus(bool bStatus, bool fromEvent)
{
	if (bStatus)
	{
		//future
	}
	else
	{
		//future
	}

	if(GetDesc())
		ChatPacket(CHAT_TYPE_COMMAND, "AutoHuntStatus %d", bStatus);
}
void CHARACTER::GetAutoHuntCommand(const char* szArgument)
{
	std::vector<std::string> vecArgs;
	split_argument(szArgument, vecArgs);
	if (vecArgs.size() < 2) { return; }
	else if (vecArgs[1] == "end")
	{
		SetAutoHuntStatus(false);
	}
	else if (vecArgs[1] == "start")
	{
		if (!IsAutoHuntAffectHas())
		{
			ChatPacket(CHAT_TYPE_INFO, "You don't have auto hunt affect.");
			return;
		}
		SetAutoHuntStatus(true);
	}
	ChatPacket(CHAT_TYPE_COMMAND, "auto_hunt %s", szArgument);
}
bool CHARACTER::IsAutoHuntAffectHas()
{
#ifdef USE_PREMIUM_AFFECT
	return (GetPoint(POINT_PREMIUM_USER) > 0) ? true : false;
#else
	return FindAffect(AFFECT_AUTO_HUNT) != NULL ? true : false;
#endif
}
#endif

#ifdef VOTE_BUFF_RENEWAL
long CHARACTER::GetEp() const
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT votebuff FROM account.account WHERE id = '%d';", GetDesc()->GetAccountTable().id));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;
	
	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	DWORD votecoins = 0;
	str_to_number(votecoins, row[0]);
	return votecoins;
}
void CHARACTER::SetVoteCoins(long votecoins)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE account.account SET votebuff = '%d' WHERE id = '%d';", votecoins, GetDesc()->GetAccountTable().id));
	m_coinsvote = votecoins;
}
void CHARACTER::SetEp(long votecoins)
{
	m_coinsvote = votecoins;
	SetPoint(POINT_VCOINS, votecoins);
}
#endif

const char* CHARACTER::GetHardwareID()
{
	char szQuery[512 + 1] = {};
	snprintf(szQuery, sizeof(szQuery), "SELECT hwid_routher FROM account.account WHERE id = '%d' LIMIT 1", GetDesc()->GetAccountTable().id);
	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));

	if (pMsg->Get() && pMsg->Get()->uiNumRows > 0)
	{
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		return row[0];
	}

	return "";
}

#ifdef USE_CAPTCHA_SYSTEM
EVENTFUNC(captcha_event)
{
    captcha_event_info* info = dynamic_cast<captcha_event_info*>(event->info);
    if (!info)
    {
        sys_err("<captcha_event> event is null.");
        return 0;
    }

    LPCHARACTER ch = info->ch;
    if (!ch)
    {
        sys_err("<captcha_event> ch is null.");
        return 0;
    }

    LPDESC d = ch->GetDesc();
    if (!d)
    {
        sys_err("<captcha_event> %s have no desc connector.", ch->GetName());
        return 0;
    }

    if (info->disconnect == true)
    {
        if (ch->IsWaitingForCaptcha() == true)
        {
            TPacketEmpty p;
            p.bHeader = HEADER_GC_CLOSECLIENT;
            d->Packet(&p, sizeof(p));

            d->DelayedDisconnect(3);
        }

        ch->CleanCaptchaTimer();
        return 0;
    }
    else
    {
        if (ch->IsWaitingForCaptcha() == false)
        {
            if (get_dword_time() - ch->GetLastAttackTime() <= 800)
            {
                info->disconnect = true;

                ch->SetWaitingForCaptcha(true);

                TPacketGCCaptcha p;
                p.bHeader = HEADER_GC_CAPTCHA;
                p.code = ch->GetCaptcha();
                p.limit = CAPTCHA_MAX_ALLOWED_TIME;
                d->Packet(&p, sizeof(p));
            }
            else
            {
                return PASSES_PER_SEC(60);
            }
        }

        return PASSES_PER_SEC(CAPTCHA_MAX_ALLOWED_TIME);
    }
}

void CHARACTER::LoadCaptchaTimer()
{
    if (m_CaptchaTimer)
    {
        event_cancel(&m_CaptchaTimer);
        m_CaptchaTimer = nullptr;
    }

    if (IsGM())
    {
        return;
    }

    SetWaitingForCaptcha(false);

    uint32_t dwMapIndex = GetMapIndex();
    if (dwMapIndex >= 10000)
    {
        dwMapIndex /= 10000;
    }

    if (std::count(std::begin(st_dwActivateCaptchaForMapIndex), std::end(st_dwActivateCaptchaForMapIndex), dwMapIndex) > 0)
    {
        m_Captcha = number(1000, 9999);
        uint32_t NextTimeCheck = MINMAX(1, number(CAPTCHA_TIME_MIN, CAPTCHA_TIME_MAX), 720);

        captcha_event_info* info = AllocEventInfo<captcha_event_info>();
        info->ch = this;
        info->disconnect = false;
        m_CaptchaTimer = event_create(captcha_event, info, PASSES_PER_SEC(NextTimeCheck));
    }
}

void CHARACTER::ValidateCaptcha(uint16_t code)
{
    if (IsWaitingForCaptcha() == false)
    {
        return;
    }

    LPDESC d = GetDesc();
    if (d)
    {
        TPacketGCCaptcha p;
        p.bHeader = HEADER_GC_CAPTCHA;
        p.code = 0;
        if (code == GetCaptcha())
        {
            p.limit = 0;

            LoadCaptchaTimer();
        }
        else
        {
            p.limit = 1;
        }

        d->Packet(&p, sizeof(p));
    }
}

void CHARACTER::CleanCaptchaTimer()
{
    if (m_CaptchaTimer)
    {
        event_cancel(&m_CaptchaTimer);
        m_CaptchaTimer = nullptr;
    }
}
#endif

void CHARACTER::SetProtectTime(const std::string& flagname, int value)
{
    auto it = m_protection_Time.find(flagname);
    if (it != m_protection_Time.end())
    {
        it->second = value;
        return;
    }

    m_protection_Time.insert(make_pair(flagname, value));
}

int CHARACTER::GetProtectTime(const std::string& flagname) const
{
    auto it = m_protection_Time.find(flagname);
    if (it != m_protection_Time.end())
    {
        return it->second;
    }

    return 0;
}

#ifdef USE_OFFLINESHOP_SEARCH_V2
int32_t CHARACTER::GetShopSearchPage() const
{
    return m_iShopSearchPage;
}

void CHARACTER::SetShopSearchPage(const int32_t v)
{
    m_iShopSearchPage = v;
}

bool CHARACTER::RefreshShopSearchPageCount()
{
    m_iShopSearchPageCount = ceilf(static_cast<float>(m_vecShopSearchItem.size()) / static_cast<float>(OFFLINESHOP_MAX_ITEM_PER_SEARCH_PAGE));
    return true;
}

size_t CHARACTER::GetShopSearchMaxPageCount() const
{
    return m_iShopSearchPageCount;
}

bool CHARACTER::SetShopSearchFilterInfo(const ikashop::TFilterInfo& filter)
{
    m_vecShopSearchItem.clear();
    m_setShopSearchItemIds.clear();

    m_ShopSearchFilterInfo = filter;
    return true;
}

ikashop::TFilterInfo CHARACTER::GetShopSearchFilterInfo() const
{
    return m_ShopSearchFilterInfo;
}

bool CHARACTER::SetShopSearchEraseItem(const uint32_t id)
{
    bool ret = false;
    auto it = m_vecShopSearchItem.begin();
    while (it != m_vecShopSearchItem.end())
    {
        if (it->id == id)
        {
            m_vecShopSearchItem.erase(it);

            ret = true;
            break;
        }

        ++it;
    }

    return ret;
}

bool CHARACTER::SetShopSearchIds(const uint32_t id)
{
    if (id == 0)
    {
        return false;
    }

    m_setShopSearchItemIds.insert(id);
    return true;
}

bool CHARACTER::GetShopSearchIds(const uint32_t id)
{
    auto it = m_setShopSearchItemIds.find(id);
    if (it != m_setShopSearchItemIds.end())
    {
        m_setShopSearchItemIds.erase(id);
        return true;
    }

    return false;
}

bool CHARACTER::UpdateShopSearchItemPrice(const uint32_t id, const long long price)
{
    bool ret = false;

    auto it = m_vecShopSearchItem.begin();
    while (it != m_vecShopSearchItem.end())
    {
        if (it->id == id)
        {
            it->price.yang = price;
            ret = true;
            break;
        }

        ++it;
    }

    return ret;
}
#endif
#ifdef ENABLE_SUNG_MAHI_TOWER
void CHARACTER::SetDungeonMultipliers(BYTE dungeonLevel)
{
	if (dungeonLevel > 50)
		return;

	PointChange(POINT_DEF_GRADE, GetPoint(POINT_DEF_GRADE) + (dungeonLevel * SUNG_MAHI_DAMANGE_MULTIPLIER));
	PointChange(POINT_ATT_GRADE, GetPoint(POINT_ATT_GRADE) + (dungeonLevel * SUNG_MAHI_DEFENCE_MULTIPLIER));
	
	SetMaxHP(GetMaxHP() + (dungeonLevel * SUNG_MAHI_HP_MULTIPLIER));
	SetHP(GetMaxHP());
}
bool CHARACTER::IsSungMahiDungeon(int mapIdx) const
{
	switch(mapIdx)
	{
		case 353:
		case 354:
			return true;
	}
	return false;
}
#endif
void CHARACTER::SaveRankingInfo()
{
	std::vector<TPacketGDAddRanking> s_table;
	s_table.clear();

	int i = 0;
	for (int index = 0; index < RANK_MAX; ++index)
	{
		long long lValue = RankPlayer::instance().GetProgressByPID(GetPlayerID(), index);

		// SKIP_TYPE_RANK_FROM_ANOTHER_TABLE
		if (lValue == -1 || lValue == 0)
			continue;
		// SKIP_TYPE_RANK_FROM_ANOTHER_TABLE
		
		TPacketGDAddRanking r;
		r.bTypeRank	= index;
		r.lValue	= lValue;
		r.dwPID		= GetPlayerID();
		
		s_table.push_back(r);
		i++;
	}
	
	if (!s_table.empty() && i > 0)
	{
		db_clientdesc->DBPacketHeader(HEADER_GD_ADD_RANKING, 0, sizeof(TPacketGDAddRanking) * i);
		db_clientdesc->Packet(&s_table[0], sizeof(TPacketGDAddRanking) * i);
	}

	RankPlayer::instance().SendInfoPlayer(this, RANK_BY_BONUS_CHANGED, RankPlayer::instance().GetProgressByPID(GetPlayerID(), RANK_BY_BONUS_CHANGED));
	RankPlayer::instance().SendInfoPlayer(this, RANK_BY_BOX_OPENED, RankPlayer::instance().GetProgressByPID(GetPlayerID(), RANK_BY_BOX_OPENED));
	RankPlayer::instance().SendInfoPlayer(this, RANK_BY_ITEM_IMPROVED, RankPlayer::instance().GetProgressByPID(GetPlayerID(), RANK_BY_ITEM_IMPROVED));
	RankPlayer::instance().SendInfoPlayer(this, RANK_BY_YANG, GetGold());
	RankPlayer::instance().SendInfoPlayer(this, RANK_BY_LEVEL, GetConquerorLevel());
	RankPlayer::instance().SendInfoPlayer(this, RANK_BY_MONSTER, RankPlayer::instance().GetProgressByPID(GetPlayerID(), RANK_BY_MONSTER));
}

#ifdef ENABLE_PITTY_REFINE
int CHARACTER::GetPittyRefineLevel(DWORD dwItemVnum, int iRefineLevel)
{
	auto iter = m_mapRefineFlags.find(std::make_pair(dwItemVnum, iRefineLevel));
	if (iter != m_mapRefineFlags.end())
		return iter->second;
	
	return 0;
}

void CHARACTER::SetPittyRefineLevel(DWORD dwItemVnum, int iRefineLevel, int iValue)
{
	if (iValue == 0) // Remove From map if no needed anymore..
	{
		auto iter = m_mapRefineFlags.find(std::make_pair(dwItemVnum, iRefineLevel));
		if (iter != m_mapRefineFlags.end())
			m_mapRefineFlags.erase(iter);
		return;
	}

	m_mapRefineFlags[std::make_pair(dwItemVnum, iRefineLevel)] = iValue;
}

void CHARACTER::SendPittyInfoClient(DWORD dwItemVnum, int iRefineLevel)
{
	int iPittyValue = GetPittyRefineLevel(dwItemVnum, iRefineLevel);
	
	TItemTable* p = ITEM_MANAGER::instance().GetTable(dwItemVnum);
	if (p)
	{
		// SKIP_GET_max_lvFROM_CLIENTSIDE
		int iMaxPittyLevel = 0;
		for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
		{
			if (LIMIT_PITTY_REFINE == p->aLimits[i].bType)
				iMaxPittyLevel = p->aLimits[i].lValue;
		}
		// SKIP_GET_max_lvFROM_CLIENTSIDE

		ChatPacket(CHAT_TYPE_COMMAND, "RefinePitty %d %d %d", dwItemVnum, iPittyValue, iMaxPittyLevel);
	}
}

void CHARACTER::SaveRefineFlags()
{
	if (m_mapRefineFlags.empty())
		return;
	
	FILE 	*fileID;
	char file_name[256+1];

	snprintf(file_name, sizeof(file_name), "%s/local_database/refine_pitty/pid_%d.txt", LocaleService_GetBasePath().c_str(), GetPlayerID());
	
	fileID = fopen(file_name, "w");
	if (NULL == fileID)
		return;
	
	for (auto &it : m_mapRefineFlags)
	{
		auto iterFirst = it.first;
		auto refineLevel = it.second;
		
		fprintf(fileID,"REFINE	%d	%d	%d\n", iterFirst.first, iterFirst.second, refineLevel);
	}
	
	fclose(fileID);
}

void CHARACTER::LoadRefineFlags()
{
	m_mapRefineFlags.clear();
	
	FILE 	*fp;
	char	one_line[256];
	int		value1, value2, value3;
	const char	*delim = " \t\r\n";
	char	*v, *token_string;
	char file_name[256+1];

	snprintf(file_name, sizeof(file_name), "%s/local_database/refine_pitty/pid_%d.txt", LocaleService_GetBasePath().c_str(), GetPlayerID());
	fp = fopen(file_name, "r");

	if (NULL == fp)
		return;

	while (fgets(one_line, 256, fp))
	{
		value1 = 0; value2 = 0; value3 = 0;
		token_string = strtok(one_line, delim);
		if (NULL == token_string)
			continue;
		
		if ((v = strtok(NULL, delim)))
			str_to_number(value1, v);

		if ((v = strtok(NULL, delim)))
			str_to_number(value2, v);

		if ((v = strtok(NULL, delim)))
			str_to_number(value3, v);

		TOKEN("REFINE")
		{
			m_mapRefineFlags[std::make_pair(value1, value2)] = value3;
		}
	}

	fclose(fp);
}
#endif

#ifdef USE_CHEQUE_CURRENCY
void CHARACTER::SetCheque(const uint16_t value)
{
	if (value > CHEQUE_MAX)
	{
		sys_err("[CHEQUE_OVERFLOW] name (%s), pid (%u), value (%u).", GetName(), GetPlayerID(), value);
		return;
	}

	m_points.cheque = value;
}

uint16_t CHARACTER::GetCheque() const
{
	return m_points.cheque;
}
#endif

#ifdef USE_DELAY_MGR
void CHARACTER::SetDelay(const uint8_t bType, const uint32_t dwValue)
{
	if (!IsPC())
	{
		return;
	}

	if (bType >= M2_DELAY_MAX)
	{
		return;
	}

	m_dwDelayTime[bType] = get_dword_time() + dwValue;
}

bool CHARACTER::IsDelay(const uint8_t bType, bool bMsg/* = false*/)
{
	if (!IsPC())
	{
		return false;
	}

	if (bType >= M2_DELAY_MAX)
	{
		return false;
	}

	const auto dwTimeNow = get_dword_time();
	if (dwTimeNow < m_dwDelayTime[bType])
	{
		if (bMsg)
		{
			const auto dwTimeLeft = m_dwDelayTime[bType] - dwTimeNow;
			ChatPacket(CHAT_TYPE_INFO, "1310 %.2f", (float)((float(dwTimeLeft)) / 1000.0f));
		}

		return true;
	}

	return false;
}
#endif

#ifdef USE_CURRENCY_EXCHANGE
void CHARACTER::OnCurrencyExchange(long long exchangeAmount, uint8_t exchangeType)
{
	if (!IsPC())
	{
		return;
	}

	switch (exchangeType)
	{
		case SUBHEADER_CURRENCY_EXCHANGE_YANG:
		{
			if (exchangeAmount < 1)
			{
				return;
			}

#ifdef USE_DELAY_MGR
			if (IsDelay(M2_DELAY_CURRENCY_EXCHANGE, true))
			{
				return;
			}

			SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 5000);
#endif

/*
todo security checks
*/

			const long long llWonValue = static_cast<long long>(ONE_CHEQUE_EQUAL_TO_YANG);

			long long llExchangeLimit = static_cast<long long>(GOLD_MAX);
			llExchangeLimit /= llWonValue;

			if (exchangeAmount > llExchangeLimit)
			{
				ChatPacket(CHAT_TYPE_INFO, "1309", llExchangeLimit);
#ifdef USE_DELAY_MGR
				SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 250);
#endif
				return;
			}

			const long long llExchangeAmountYang = exchangeAmount * llWonValue;
			if (llExchangeAmountYang == 0)
			{
				ChatPacket(CHAT_TYPE_INFO, "1300");
#ifdef USE_DELAY_MGR
				SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 250);
#endif
				return;
			}

			long long llInventoryValue = static_cast<long long>(GetGold());
			if (llInventoryValue < llExchangeAmountYang)
			{
				ChatPacket(CHAT_TYPE_INFO, "1308", llExchangeAmountYang);
#ifdef USE_DELAY_MGR
				SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 250);
#endif
				return;
			}

			long long llActualCheque = static_cast<long long>(GetCheque());
			llActualCheque += exchangeAmount;

			if (llActualCheque > static_cast<long long>(CHEQUE_MAX))
			{
				ChatPacket(CHAT_TYPE_INFO, "1307", exchangeAmount);
				ChatPacket(CHAT_TYPE_INFO, "1306");
#ifdef USE_DELAY_MGR
				SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 250);
#endif
				return;
			}

			PointChange(POINT_GOLD, -static_cast<long long>(llExchangeAmountYang), true);
			PointChange(POINT_CHEQUE, static_cast<long long>(exchangeAmount), true);

			Save();
#ifdef USE_DELAY_MGR
			SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 250);
#endif

			break;
		}
		case SUBHEADER_CURRENCY_EXCHANGE_WON:
		{
			if (exchangeAmount < 1)
			{
				return;
			}

#ifdef USE_DELAY_MGR
			if (IsDelay(M2_DELAY_CURRENCY_EXCHANGE, true))
			{
				return;
			}

			SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 5000);
#endif

/*
todo security checks
*/

			const long long llWonValue = static_cast<long long>(ONE_CHEQUE_EQUAL_TO_YANG);

			long long llExchangeLimit = static_cast<long long>(GOLD_MAX);
			llExchangeLimit /= llWonValue;

			if (exchangeAmount > llExchangeLimit)
			{
				ChatPacket(CHAT_TYPE_INFO, "1305", llExchangeLimit);
#ifdef USE_DELAY_MGR
				SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 250);
#endif
				return;
			}

			long long llInventoryValue = static_cast<long long>(GetCheque());
			if (llInventoryValue < exchangeAmount)
			{
				ChatPacket(CHAT_TYPE_INFO, "1304", exchangeAmount);
#ifdef USE_DELAY_MGR
				SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 250);
#endif
				return;
			}

			const long long llAmountYang = llWonValue * exchangeAmount;
			if (llAmountYang == 0)
			{
				ChatPacket(CHAT_TYPE_INFO, "1301");
#ifdef USE_DELAY_MGR
				SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 250);
#endif
				return;
			}

			long long llActualYang = static_cast<long long>(GetGold());
			llActualYang += llAmountYang;

			if (llActualYang > static_cast<long long>(GOLD_MAX))
			{
				ChatPacket(CHAT_TYPE_INFO, "1302", exchangeAmount);
				ChatPacket(CHAT_TYPE_INFO, "1303");
#ifdef USE_DELAY_MGR
				SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 250);
#endif
				return;
			}

			PointChange(POINT_CHEQUE, -static_cast<long long>(exchangeAmount), true);
			PointChange(POINT_GOLD, static_cast<long long>(llAmountYang), true);

			Save();
#ifdef USE_DELAY_MGR
			SetDelay(M2_DELAY_CURRENCY_EXCHANGE, 250);
#endif
			break;
		}
		default:
		{
			sys_err("Unknown exchangeType (%u), playerName (%s), playerID (%u)", exchangeType, GetName(), GetPlayerID());
			return;
		}
	}
}
#endif

#ifdef __NEW_SET_BONUS__
bool CHARACTER::IsNewSetNeedRefresh(DWORD itemIdx)
{
	for (auto it = m_mapNewSetBonus.begin(); it != m_mapNewSetBonus.end(); ++it)
	{
		const TNewSetBonus& setBonus = it->second;

		for (BYTE i = 0; i < setBonus.vecSlots.size(); ++i)
		{
			const std::vector<DWORD>& possibleItemIDs = setBonus.vecSlots[i].second;

			if (std::find(possibleItemIDs.begin(), possibleItemIDs.end(), itemIdx) != possibleItemIDs.end())
			{
				return true;
			}
		}
	}
	return false;
}
#endif

#ifdef ENABLE_BATTLEPASS
#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;

void CHARACTER::UpdateMissionProgress(EBattlePassMissions mission_type, int32_t update_amount, uint32_t target_vnum)
{
    const auto& missionMap = CHARACTER_MANAGER::Instance().GetBattlePassMissionMap();

    for (auto& [player_mission_index, player_mission_info] : m_player_battle_pass_stats)
    {
        auto mission_info_it = missionMap->find(player_mission_index);
        if (mission_info_it == missionMap->end())
            continue;

		if (player_mission_info.completed)
			continue;

        const auto& mission_info = mission_info_it->second;

        if (mission_info.mission_type == mission_type)
        {
            if (BP_IsReachMission(mission_type))
            {
                if (update_amount >= mission_info.target_count)
                {
                    player_mission_info.total_count = update_amount;
                    player_mission_info.completed = true;
                }
            }
            else
            {
				bool update = false;

                const bool is_target_valid =(mission_info.target_object == 0 || target_vnum == mission_info.target_object);
                const bool is_level_in_range = (GetLevel() >= mission_info.min_level && GetLevel() <= mission_info.max_level);

                if (is_target_valid && is_level_in_range)
                {
                    player_mission_info.total_count += update_amount;
                    update = true;
                }

                if (player_mission_info.total_count >= mission_info.target_count)
                {
                    player_mission_info.total_count = mission_info.target_count;
                    player_mission_info.completed = true;
                }
				if (update)
				{
                    UpdateBattlePassMissionProgress(player_mission_index, player_mission_info.total_count,
                                                    player_mission_info.completed);
				}

            }
        }
    }
}

void CHARACTER::InitializeBattlePassData()
{
    const auto& mission_map_ptr = CHARACTER_MANAGER::Instance().GetBattlePassMissionMap();
    if (mission_map_ptr && !mission_map_ptr->empty())
    {
        for (const auto& [mission_id, mission_info] : *mission_map_ptr)
        {
            m_player_battle_pass_stats[mission_id] = BattlePassMyMissionInfo{false, 0, false};
        }
    }
    else
    {
        sys_err("Mission map is empty or not available");
    }

    const auto& tier_map = CHARACTER_MANAGER::Instance().GetBattlePassTierMap();
    if (tier_map && !tier_map->empty())
    {
        for (const auto& [tier_id, tier_info] : *tier_map)
        {
            m_player_battle_pass_tier_info[tier_id] = BattlePassMyTierInfo{false, false};
        }
    }
    else
    {
        sys_err("Tier map is empty or not available");
    }

    m_player_battle_pass_current_level = 0;
    m_player_battle_pass_current_exp = 0;
    m_player_battle_pass_premium_status = false;
}

void CHARACTER::LoadBattlePassProgress()
{
    const uint8_t battle_pass_id = CHARACTER_MANAGER::Instance().GetBattlePassInfo()->id;
    char filepath[128 + 1];
    snprintf(filepath, sizeof(filepath), "locale/germany/battlepass/user_data/%u_%s.json", battle_pass_id, GetName());

    std::ifstream file(filepath);

    // Check if file exists and is open
    if (!file.is_open())
    {
        // Initialize with default values if file does not exist
        sys_err("No saved battle pass data found for [%s]. Initializing with default values.", GetName());
        InitializeBattlePassData();
        return;
    }

    json j;
    try
    {
        file >> j;
    }
    catch (const std::exception& e)
    {
        sys_err("Failed to parse JSON from file [%s]: %s", filepath, e.what());
        file.close();
        InitializeBattlePassData();
        return;
    }

    file.close(); // Close the file after reading

    m_player_battle_pass_stats.clear();
    m_player_battle_pass_tier_info.clear();

    const auto& mission_map_ptr = CHARACTER_MANAGER::Instance().GetBattlePassMissionMap();
    if (mission_map_ptr && !mission_map_ptr->empty())
    {
        for (const auto& [mission_id, mission_info] : *mission_map_ptr)
        {
            auto it = j["stats"].find(std::to_string(mission_id));
            if (it != j["stats"].end())
            {
                BattlePassMyMissionInfo mission;
                mission.completed = it.value().value("completed", false);
                mission.total_count = it.value().value("total_count", uint32_t{0});
                mission.collected = it.value().value("collected", false);
                m_player_battle_pass_stats[mission_id] = mission;
            }
            else
            {
                m_player_battle_pass_stats[mission_id] = BattlePassMyMissionInfo{false, 0, false};
            }
        }
    }
    else
    {
        sys_err("Mission map is empty or not available");
    }

    const auto& tier_map = CHARACTER_MANAGER::Instance().GetBattlePassTierMap();
    if (tier_map && !tier_map->empty())
    {
        for (const auto& [tier_id, tier_info] : *tier_map)
        {
            auto it = j["tiers"].find(std::to_string(tier_id));
            if (it != j["tiers"].end())
            {
                BattlePassMyTierInfo tier;
                tier.reward_collected = it.value().value("reward_collected", false);
                tier.reward_premium_collected = it.value().value("reward_premium_collected", false);
                m_player_battle_pass_tier_info[tier_id] = tier;
            }
            else
            {
                m_player_battle_pass_tier_info[tier_id] = BattlePassMyTierInfo{false, false};
            }
        }
    }
    else
    {
        sys_err("Tier map is empty or not available");
    }

    m_player_battle_pass_current_level = j.value("current_level", uint16_t{0});
    m_player_battle_pass_current_exp = j.value("current_exp", uint32_t{0});
    m_player_battle_pass_premium_status = j.value("premium_status", false);

}

void CHARACTER::SaveBattlePassProgress()
{
    json j;

    // Save player battle pass stats
    json stats_json;
    for (const auto& [mission_id, mission_stat] : m_player_battle_pass_stats)
    {
        stats_json[std::to_string(mission_id)] = {
            {"completed",   mission_stat.completed  },
            {"total_count", mission_stat.total_count},
            {"collected",   mission_stat.collected  }
        };
    }
    j["stats"] = stats_json;

    json tiers_json;
    for (const auto& [tier_id, tier_info] : m_player_battle_pass_tier_info)
    {
        tiers_json[std::to_string(tier_id)] = {
            {"reward_collected",         tier_info.reward_collected        },
            {"reward_premium_collected", tier_info.reward_premium_collected}
        };
    }
    j["tiers"] = tiers_json;

    j["current_level"] = m_player_battle_pass_current_level;
    j["current_exp"] = m_player_battle_pass_current_exp;
    j["premium_status"] = m_player_battle_pass_premium_status;

	const uint8_t battle_pass_id = CHARACTER_MANAGER::Instance().GetBattlePassInfo()->id;
    char filepath[128 + 1];
    snprintf(filepath, sizeof(filepath), "locale/germany/battlepass/user_data/%u_%s.json", battle_pass_id, GetName());
    std::ofstream file(filepath);
    if (!file.is_open())
    {
        sys_err("Failed to open file for saving battle pass progress for [%s]", GetName());
        return;
    }

    file << j.dump(4);
    file.close();
    UpdateBattlePassCurrentCharacterInfo();
}

void CHARACTER::SendBattlePassCurrentProgress()
{
    auto desc = GetDesc();
    if (!desc)
        return;

	UpdateBattlePassCurrentCharacterInfo();

    /* Sending Dynamic Packets to the Client Side*/
    auto packet_size = sizeof(packet_battlepass) + sizeof(packet_battlepass_info) + sizeof(BattlePassCharacterInfo)
                       + sizeof(BattlePassCharacterMissionInfo) * m_player_battle_pass_stats.size() 
					   + sizeof(BattlePassCharacterTierInfo) * m_player_battle_pass_tier_info.size();

    TEMP_BUFFER buffer(packet_size);

    packet_battlepass packet = {
        HEADER_GC_BATTLEPASS,
        static_cast<uint16_t>(packet_size),
        SUBHEADER_SEND_CHARACTER_DATA,
    };
    buffer.write(&packet, sizeof(packet));
	
    packet_battlepass_info size_packet;
	size_packet.mission_count = m_player_battle_pass_stats.size();
    size_packet.tier_count = m_player_battle_pass_tier_info.size();
    buffer.write(&size_packet, sizeof(size_packet));

	BattlePassCharacterInfo chr_info;
    chr_info.current_level = m_player_battle_pass_current_level;
    chr_info.current_exp = m_player_battle_pass_current_exp;
    chr_info.premium_status = m_player_battle_pass_premium_status;
    buffer.write(&chr_info, sizeof(chr_info));

    for (const auto& [mission_index, mission_stat] : m_player_battle_pass_stats)
    {
        BattlePassCharacterMissionInfo mission_info;
        mission_info.mission_index = mission_index;
        mission_info.progress = mission_stat.total_count;
        mission_info.completed = mission_stat.completed;
        mission_info.collected = mission_stat.collected;
        buffer.write(&mission_info, sizeof(mission_info));
    }

    for (const auto& [tier_index, tier_stat] : m_player_battle_pass_tier_info)
    {
        BattlePassCharacterTierInfo tier_info;
        tier_info.mission_index =tier_index;
        tier_info.reward_collected = tier_stat.reward_collected;
        tier_info.reward_premium_collected = tier_stat.reward_premium_collected;
        buffer.write(&tier_info, sizeof(tier_info));
    }
    desc->Packet(buffer.read_peek(), buffer.size());
}

bool CHARACTER::ActivateBattlePassPremium()
{
    if (m_player_battle_pass_premium_status)
        return false;

    m_player_battle_pass_premium_status = true;
    UpdateBattlePassCurrentCharacterInfo();
    return true;
}

void CHARACTER::RedeemBattlePassReward(size_t tier_index, bool is_premium_item)
{
    if (is_premium_item && !m_player_battle_pass_premium_status)
    {
        ChatPacket(1, "You have to buy premium battlepass to collect premium items!");
        return;
    }

    auto player_tier_info_it = m_player_battle_pass_tier_info.find(tier_index);
    if (player_tier_info_it == m_player_battle_pass_tier_info.end())
    {
        ChatPacket(1, "Invalid tier index.");
        return;
    }

	if (m_player_battle_pass_current_level < tier_index + 1)
    {
        ChatPacket(1, "You need to have %u tier level to redeem this!", tier_index + 1);
        return;
    }

    auto& tier_info = player_tier_info_it->second;

    if ((is_premium_item && tier_info.reward_premium_collected) 
		|| (!is_premium_item && tier_info.reward_collected))
    {
        ChatPacket(1, "Reward already redeemed!");
        return;
    }

    auto tier_map = CHARACTER_MANAGER::Instance().GetBattlePassTierMap();
    if (tier_map->empty())
    {
        ChatPacket(1, "Battle Pass tier map is empty.");
        return;
    }

    auto tier_data_it = tier_map->find(tier_index);
    if (tier_data_it == tier_map->end())
    {
        ChatPacket(1, "Tier data not found.");
        return;
    }

    uint32_t reward_vnum =
        is_premium_item ? tier_data_it->second.reward_vnum_premium : tier_data_it->second.reward_vnum;
    uint32_t reward_count =
        is_premium_item ? tier_data_it->second.reward_premium_count : tier_data_it->second.reward_count;

    if (reward_count == 0)
        reward_count = 1; 

	TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(reward_vnum);
	if (!pItemTable)
	{
		sys_err("[RedeemBattlePassReward] Item vnum: %u doesn't exists -> playerID %u.", reward_vnum, GetPlayerID());
		return;
	}

	int iPos = GetEmptyInventory(pItemTable->bSize);
	if (iPos == -1)
	{
		ChatPacket(CHAT_TYPE_INFO, "[LS;1130]");
		return;
	}

	LPITEM pItem = ITEM_MANAGER::instance().CreateItem(reward_vnum, reward_count);
	if (!pItem)
	{
		sys_err("[RedeemBattlePassReward] Item vnum: %u cannot be created -> playerID %u.", reward_vnum, GetPlayerID());
		return;
	}

	if (is_premium_item)
	{
		tier_info.reward_premium_collected = true;
	}
	else
	{
		tier_info.reward_collected = true;
	}

	UpdateBattlePassRewardInfo(tier_index);

	pItem->AddToCharacter(this, TItemPos(INVENTORY, iPos));
}

void CHARACTER::RedeemBattlePassMissionExp(size_t mission_index)
{
    const uint32_t battlePassLevelLimit = CHARACTER_MANAGER::Instance().GetMaxBattlePassLevel();

    if (m_player_battle_pass_current_level >= battlePassLevelLimit)
    {
        ChatPacket(1, "You have reached battle-pass level limit.");
        return;
    }
	
    auto player_mission_it = m_player_battle_pass_stats.find(mission_index);
    if (player_mission_it == m_player_battle_pass_stats.end())
        return;

    if (!player_mission_it->second.completed)
    {
        ChatPacket(1, "You have to complete the mission to redeem the reward!");
        return;
    }

    if (player_mission_it->second.collected)
    {
        ChatPacket(1, "EXP already collected!");
        return;
    }

    const auto& mission_map_ptr = CHARACTER_MANAGER::Instance().GetBattlePassMissionMap();
    if (!mission_map_ptr || mission_map_ptr->empty())
        return;

    auto mission_it = mission_map_ptr->find(mission_index);
    if (mission_it == mission_map_ptr->end())
        return;

    const auto& tier_map_ptr = CHARACTER_MANAGER::Instance().GetBattlePassTierMap();
    if (!tier_map_ptr || tier_map_ptr->empty())
        return;

    auto tier_map_it = tier_map_ptr->find(m_player_battle_pass_current_level);
    if (tier_map_it == tier_map_ptr->end())
        return;

    player_mission_it->second.collected = true;

    uint32_t totalExp = m_player_battle_pass_current_exp + mission_it->second.exp_reward;
	const uint32_t nextLevelNeededExp = tier_map_it->second.needed_exp;

    while (totalExp >= nextLevelNeededExp && m_player_battle_pass_current_level < battlePassLevelLimit)
    {
        m_player_battle_pass_current_level++;
        totalExp -= nextLevelNeededExp;

        auto next_tier_map_it = std::next(tier_map_it);
        if (next_tier_map_it == tier_map_ptr->end())
        {
            m_player_battle_pass_current_exp = totalExp;
            break;
        }
        tier_map_it = next_tier_map_it;
    }

    if (tier_map_it != tier_map_ptr->end() && totalExp == nextLevelNeededExp)
    {
        m_player_battle_pass_current_level++;
        m_player_battle_pass_current_exp = 0;
    }
    else
    {
        m_player_battle_pass_current_exp = totalExp;
    }

    UpdateBattlePassMissionProgress(mission_index, player_mission_it->second.total_count,
                                    player_mission_it->second.completed, player_mission_it->second.collected);
    UpdateBattlePassCurrentCharacterInfo();

    ChatPacket(1, "EXP Redeemed!");
}

void CHARACTER::UpdateBattlePassCurrentCharacterInfo()
{
    auto desc = GetDesc();
    if (!desc)
        return;

    /* Sending Dynamic Packets to the Client Side*/
    auto packet_size = sizeof(packet_battlepass) + sizeof(BattlePassCharacterInfo);

    packet_battlepass packet = {
        HEADER_GC_BATTLEPASS,
        static_cast<uint16_t>(packet_size),
        SUBHEADER_SEND_CHARACTER_PROGRESS_UPDATE,
    };
    
    BattlePassCharacterInfo info;
    info.current_level = m_player_battle_pass_current_level;
    info.current_exp = m_player_battle_pass_current_exp;
    info.premium_status = m_player_battle_pass_premium_status;

	desc->BufferedPacket(&packet, sizeof(packet));
    desc->Packet(&info, sizeof info);
}

void CHARACTER::UpdateBattlePassMissionProgress(size_t mission_index, size_t progress, bool completed, bool collected)
{
    auto desc = GetDesc();
    if (!desc)
        return;

    /* Sending Dynamic Packets to the Client Side*/
    auto packet_size = sizeof(packet_battlepass) + sizeof(BattlePassCharacterMissionInfo);

    packet_battlepass packet = {
        HEADER_GC_BATTLEPASS,
        static_cast<uint16_t>(packet_size),
        SUBHEADER_SEND_MISSION_PROGRESS_UPDATE,
    };

    BattlePassCharacterMissionInfo info;
    info.mission_index = mission_index;
    info.progress = progress;
    info.completed = completed;
    info.collected = collected;

    desc->BufferedPacket(&packet, sizeof(packet));
    desc->Packet(&info, sizeof info);
}

void CHARACTER::UpdateBattlePassRewardInfo(size_t tier_index)
{
    auto desc = GetDesc();
    if (!desc)
        return;

    auto it = m_player_battle_pass_tier_info.find(tier_index);
    if (it == m_player_battle_pass_tier_info.end())
        return;

    /* Sending Dynamic Packets to the Client Side*/
    auto packet_size = sizeof(packet_battlepass) + sizeof(BattlePassCharacterInfo);

    packet_battlepass packet = {
        HEADER_GC_BATTLEPASS,
        static_cast<uint16_t>(packet_size),
        SUBHEADER_SEND_REWARD_UPDATE,
    };

    BattlePassCharacterTierInfo info;
    info.mission_index = tier_index;
    info.reward_collected = it->second.reward_collected;
    info.reward_premium_collected = it->second.reward_premium_collected;

    desc->BufferedPacket(&packet, sizeof(packet));
    desc->Packet(&info, sizeof info);
}

#endif

#ifdef ENABLE_VOICE_CHAT
void CHARACTER::VoiceChatSetConfig(uint8_t type, bool flag)
{
	int questFlag = GetQuestFlag("voice_chat.config");
	if (flag)
	{
		SET_BIT(questFlag, (1 << type));
	}
	else
	{
		REMOVE_BIT(questFlag, (1 << type));
	}

	SetQuestFlag("voice_chat.config", questFlag);
	ChatPacket(CHAT_TYPE_COMMAND, "voice_chat_config %d", questFlag);
}

bool CHARACTER::VoiceChatIsBlock(uint8_t type) const
{
	int flag = GetQuestFlag("voice_chat.config");
	return IS_SET(flag, (1 << type));
}

int CHARACTER::VoiceChatGetConfig() const
{
	return GetQuestFlag("voice_chat.config");
}
#endif

//#ifdef ENABLE_ITEMSHOP
#ifdef USE_ITEMSHOP_RENEWED
void CHARACTER::SetAccountMoney(long long lldCoins, long long lldJCoins, bool bPlus)
#else
void CHARACTER::SetAccountMoney(long long lldCoins, bool bPlus)
#endif
{
	const auto d = GetDesc();
	if (!d)
	{
		sys_err("no desc pointer.");
		return;
	}

	if (lldCoins < 0)
	{
		sys_err("invalid coins amount: %lld -> player (id: %u) (name: %s).", lldCoins, GetPlayerID(), GetName());
		return;
	}
#ifdef USE_ITEMSHOP_RENEWED
	else if (lldJCoins < 0)
	{
		sys_err("invalid jcoins amount: %lld -> player (id: %u) (name: %s).", lldJCoins, GetPlayerID(), GetName());
		return;
	}
#endif

	if (lldCoins == 0
#ifdef USE_ITEMSHOP_RENEWED
&& lldJCoins == 0
#endif
	)
	{
		return;
	}

	long long act_lldCoins;
#ifdef USE_ITEMSHOP_RENEWED
	long long act_lldJCoins;
	GetAccountMoney(act_lldCoins, act_lldJCoins);
#else
	GetAccountMoney(act_lldCoins);
#endif

	if (!bPlus)
	{
		if (act_lldCoins < lldCoins)
		{
			sys_err("actual dragon coins (%lld) is lower than the deducted amount (%lld) -> player (id: %u) (name: %s).", act_lldCoins, lldCoins, GetPlayerID(), GetName());
			return;
		}
#ifdef USE_ITEMSHOP_RENEWED
		else if (act_lldJCoins < lldJCoins)
		{
			sys_err("actual dragon jetons (%lld) is lower than the deducted amount (%lld) -> player (id: %u) (name: %s).", act_lldJCoins, lldJCoins, GetPlayerID(), GetName());
			return;
		}
#endif
	}

	char szQuery[256 + 1] = {};

#ifdef USE_ITEMSHOP_RENEWED
	if (lldCoins > 0 && lldJCoins > 0)
	{
		snprintf(szQuery, sizeof(szQuery), "UPDATE `account`.`account` SET `coins` = `coins` %s %lld, `jcoins` = `jcoins` %s %lld WHERE id = %u;", (bPlus) ? "+" : "-", lldCoins, (bPlus) ? "+" : "-", lldJCoins, d->GetAccountTable().id);
	}
	else if (lldCoins > 0)
	{
		snprintf(szQuery, sizeof(szQuery), "UPDATE `account`.`account` SET `coins` = `coins` %s %lld WHERE id = %u;", bPlus ? "+" : "-", lldCoins, d->GetAccountTable().id);
	}
	else if (lldJCoins > 0)
	{
		snprintf(szQuery, sizeof(szQuery), "UPDATE `account`.`account` SET `jcoins` = `jcoins` %s %lld WHERE id = %u;", bPlus ? "+" : "-", lldJCoins, d->GetAccountTable().id);
	}
#else
	if (lldCoins > 0)
	{
		snprintf(szQuery, sizeof(szQuery), "UPDATE `account`.`account` SET `coins` = `coins` %s %lld WHERE id = %u;", bPlus ? "+" : "-", lldCoins, d->GetAccountTable().id);
	}
#endif

	if (bPlus)
	{
		m_lldAccountCoins += lldCoins;
#ifdef USE_ITEMSHOP_RENEWED
		m_lldAccountJCoins += lldJCoins;
#endif
	}
	else
	{
		m_lldAccountCoins -= lldCoins;
#ifdef USE_ITEMSHOP_RENEWED
		m_lldAccountJCoins -= lldJCoins;
#endif
	}

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));

#ifdef USE_ITEMSHOP_RENEWED
	ChatPacket(CHAT_TYPE_COMMAND, "RefreshAccountMoney %lld %lld", m_lldAccountCoins, m_lldAccountJCoins);
#else
	ChatPacket(CHAT_TYPE_COMMAND, "RefreshAccountMoney %lld", m_lldAccountCoins);
#endif
}

#ifdef USE_ITEMSHOP_RENEWED
void CHARACTER::GetAccountMoney(long long& lldCoins, long long& lldJCoins, bool bReloadForced/* = false*/)
#else
void CHARACTER::GetAccountMoney(long long& lldCoins, bool bReloadForced/* = false*/)
#endif
{
	const auto d = GetDesc();
	if (!d)
	{
		lldCoins = 0;
#ifdef USE_ITEMSHOP_RENEWED
		lldJCoins = 0;
#endif
		return;
	}

	if (!m_bAccountMoneyLoaded || bReloadForced)
	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT `coins`, `jcoins` FROM `account`.`account` WHERE `id` = %u;", d->GetAccountTable().id));

		if (!pMsg->Get() || pMsg->Get()->uiNumRows == 0)
		{
			lldCoins = 0;
#ifdef USE_ITEMSHOP_RENEWED
			lldJCoins = 0;
#endif

			sys_err("no desc pointer.");
			return;
		}

		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

		str_to_number(m_lldAccountCoins, row[0]);
#ifdef USE_ITEMSHOP_RENEWED
		str_to_number(m_lldAccountJCoins, row[1]);
#endif

		if (!m_bAccountMoneyLoaded)
		{
			m_bAccountMoneyLoaded = true;
		}
	}

	lldCoins = m_lldAccountCoins;
#ifdef USE_ITEMSHOP_RENEWED
	lldJCoins = m_lldAccountJCoins;
#endif
}

void CHARACTER::RefreshAccountMoney()
{
	long long act_lldCoins;
#ifdef USE_ITEMSHOP_RENEWED
	long long act_lldJCoins;
	GetAccountMoney(act_lldCoins, act_lldJCoins, true);
#else
	GetAccountMoney(act_lldCoins, true);
#endif

#ifdef USE_ITEMSHOP_RENEWED
	ChatPacket(CHAT_TYPE_COMMAND, "RefreshAccountMoney %lld %lld", m_lldAccountCoins, m_lldAccountJCoins);
#else
	ChatPacket(CHAT_TYPE_COMMAND, "RefreshAccountMoney %lld", m_lldAccountCoins);
#endif
}
//#endif

#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
int CHARACTER::GetAutoHuntLastDay() const
{
	return m_playerData ? m_playerData->m_iAutoHuntLastDay : 0;
};

void CHARACTER::SetAutoHuntLastDay(const int val)
{
	if (m_playerData)
	{
		m_playerData->m_iAutoHuntLastDay = val;
	}
};

int CHARACTER::GetAutoHuntUsedTime() const
{
	return m_playerData ? m_playerData->m_iAutoHuntUsedTime : 0;
};

void CHARACTER::SetAutoHuntUsedTime(const int val, bool update/* = false*/)
{
	if (m_playerData)
	{
		m_playerData->m_iAutoHuntUsedTime = val;

		if (update)
		{
			if (val == AUTO_HUNT_DAILY_TIME_LIMIT_SEC)
			{
				ChatPacket(CHAT_TYPE_COMMAND, "autohunt_stat %d", val);
			}
			else
			{
				m_playerData->m_iAutoHuntLastUpdate += 1;

				if (m_playerData->m_iAutoHuntLastUpdate >= 8)
				{
					m_playerData->m_iAutoHuntLastUpdate = 0;
					ChatPacket(CHAT_TYPE_COMMAND, "autohunt_update %d", val);
				}
			}
		}
	}
};

void CHARACTER::AutoHunt_Initialize()
{
#ifdef USE_PREMIUM_AFFECT
	if (!FindAffect(AFFECT_PREMIUM_USER))
#else
	if (!FindAffect(AFFECT_AUTO_HUNT))
#endif
	{
		ChatPacket(CHAT_TYPE_COMMAND, "autohunt_stat %d", AUTO_HUNT_DAILY_TIME_LIMIT_SEC);
		return;
	}

#ifdef USE_DAY_CHANGE_MGR
	const int iToday = CDayChangeMgr::Instance().GetTodayNum();
#else
	const int iToday = GetTodayNum();
#endif
	const int iLastDay = GetAutoHuntLastDay();

	if (iLastDay == 0
		 || iToday != iLastDay)
	{
		SetAutoHuntLastDay(iToday);
		SetAutoHuntUsedTime(0);
	}

	ChatPacket(CHAT_TYPE_COMMAND, "autohunt_stat %d", GetAutoHuntUsedTime());
}
#endif
