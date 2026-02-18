#ifndef PY_PLAYER_SETTINGS_MODULE_HPP
#define PY_PLAYER_SETTINGS_MODULE_HPP

#if VSTD_HAS_PRAGMA_ONCE
#pragma once
#endif

#include <cstdint>
#include <initializer_list>
#include <array>
#include <PythonPlayerSettingsModuleLib.h>

#include "InstanceBase.h"
#include "PythonItem.h"

static const auto EMOTICON_PATH = "d:/ymir work/effect/etc/emoticon/";
static const auto NPC_LIST_FILE_NAME = "npclist.txt";

enum
{
    HORSE_SKILL_WILDATTACK = CRaceMotionData::NAME_SKILL + 121,
    HORSE_SKILL_CHARGE = CRaceMotionData::NAME_SKILL + 122,
    HORSE_SKILL_SPLASH = CRaceMotionData::NAME_SKILL + 123,

    GUILD_SKILL_DRAGONBLOOD = CRaceMotionData::NAME_SKILL + 101,
    GUILD_SKILL_DRAGONBLESS = CRaceMotionData::NAME_SKILL + 102,
    GUILD_SKILL_BLESSARMOR = CRaceMotionData::NAME_SKILL + 103,
    GUILD_SKILL_SPPEDUP = CRaceMotionData::NAME_SKILL + 104,
    GUILD_SKILL_DRAGONWRATH = CRaceMotionData::NAME_SKILL + 105,
    GUILD_SKILL_MAGICUP = CRaceMotionData::NAME_SKILL + 106,

    COMBO_TYPE_1 = 0,
    COMBO_TYPE_2 = 1,
    COMBO_TYPE_3 = 2,

    COMBO_INDEX_1 = 0,
    COMBO_INDEX_2 = 1,
    COMBO_INDEX_3 = 2,
    COMBO_INDEX_4 = 3,
    COMBO_INDEX_5 = 4,
    COMBO_INDEX_6 = 5,
};

static const std::vector<uint8_t> m_vecPassiveGuildSkillIndex = { 151, };

static const std::vector<uint8_t> m_vecActiveGuildSkillIndex = { 152, 153, 154, 155, 156, 157, };

static const std::vector<TSkillIndex> m_vecSkillIndex =
{
    {
        NRaceData::JOB_WARRIOR,
        {
            { 1, { 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 137, 0, 138, 0, 139, 0, } },
            { 2, { 16, 17, 18, 19, 20, 21, 0, 0, 0, 0, 137, 0, 138, 0, 139, 0, } },
        },
        { 0, 0, 121, 134, 133, 129, 130, 131, 123, 124, 122, 132, 246,
#ifdef USE_PASSIVE_ABILITY_TAMER
PASSIVE_ABILITY_VNUM_TAMER_1, PASSIVE_ABILITY_VNUM_TAMER_2, PASSIVE_ABILITY_VNUM_TAMER_3, PASSIVE_ABILITY_VNUM_TAMER_4
#else
0, 0, 0, 0
#endif
, 0 }
    },
    {
        NRaceData::JOB_ASSASSIN,
        {
            { 1, { 31, 32, 33, 34, 35, 36, 0, 0, 0, 0, 137, 0, 138, 0, 139, 0, 140 } },
            { 2, { 46, 47, 48, 49, 50, 51, 0, 0, 0, 0, 137, 0, 138, 0, 139, 0, 140 } },
        },
        { 0, 0, 121, 134, 133, 129, 130, 131, 123, 124, 122, 132, 246,
#ifdef USE_PASSIVE_ABILITY_TAMER
PASSIVE_ABILITY_VNUM_TAMER_1, PASSIVE_ABILITY_VNUM_TAMER_2, PASSIVE_ABILITY_VNUM_TAMER_3, PASSIVE_ABILITY_VNUM_TAMER_4
#else
0, 0, 0, 0
#endif
, 0 }
    },
    {
        NRaceData::JOB_SURA,
        {
            { 1, { 61, 62, 63, 64, 65, 66, 0, 0, 0, 0, 137, 0, 138, 0, 139, 0 } },
            { 2, { 76, 77, 78, 79, 80, 81, 0, 0, 0, 0, 137, 0, 138, 0, 139, 0 } },
        },
        { 0, 0, 121, 134, 133, 129, 130, 131, 123, 124, 122, 132, 246,
#ifdef USE_PASSIVE_ABILITY_TAMER
PASSIVE_ABILITY_VNUM_TAMER_1, PASSIVE_ABILITY_VNUM_TAMER_2, PASSIVE_ABILITY_VNUM_TAMER_3, PASSIVE_ABILITY_VNUM_TAMER_4
#else
0, 0, 0, 0
#endif
, 0 }
    },
    {
        NRaceData::JOB_SHAMAN,
        {
            { 1, { 91, 92, 93, 94, 95, 96, 0, 0, 0, 0, 137, 0, 138, 0, 139, 0 } },
            { 2, { 106, 107, 108, 109, 110, 111, 0, 0, 0, 0, 137, 0, 138, 0, 139, 0 } },
        },
        { 0, 0, 121, 134, 133, 129, 130, 131, 123, 124, 122, 132, 246,
#ifdef USE_PASSIVE_ABILITY_TAMER
PASSIVE_ABILITY_VNUM_TAMER_1, PASSIVE_ABILITY_VNUM_TAMER_2, PASSIVE_ABILITY_VNUM_TAMER_3, PASSIVE_ABILITY_VNUM_TAMER_4
#else
0, 0, 0, 0
#endif
, 0 }
    },
};

static auto string_path(const std::initializer_list<std::string> & args) -> std::string
{
    std::string stPathName;
    for (const auto& it : args)
    {
        stPathName.append(it);
    }

    return stPathName;
}

static const std::vector<THorse> m_vecHorse =
{
    { NRaceData::JOB_WARRIOR, { CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD } },
    { NRaceData::JOB_ASSASSIN, { CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD } },
    { NRaceData::JOB_SURA, { CRaceMotionData::MODE_HORSE_ONEHAND_SWORD } },
    { NRaceData::JOB_SHAMAN, { CRaceMotionData::MODE_HORSE_FAN, CRaceMotionData::MODE_HORSE_BELL } },
};

static const std::vector<TSkillData> m_vecSkillFileName =
{
    { NRaceData::JOB_WARRIOR, { "samyeon", "palbang", "jeongwi", "geomgyeong", "tanhwan", "gihyeol", "gigongcham", "gyeoksan", "daejin", "cheongeun", "geompung", "noegeom" }, },
    { NRaceData::JOB_ASSASSIN, { "amseup", "gungsin", "charyun", "eunhyeong", "sangong", "seomjeon", "yeonsa", "gwangyeok", "hwajo", "gyeonggong", "dokgigung", "seomgwang" }, },
    { NRaceData::JOB_SHAMAN,
        { "bipabu", "yongpa", "paeryong", "hosin", "boho", "gicheon", "noejeon", "byeorak", "pokroe", "jeongeop", "kwaesok", "jeungryeok" },
        { "bipabu", "yongpa", "paeryong", "hosin_target", "boho_target", "gicheon_target", "noejeon", "byeorak", "pokroe", "jeongeop_target", "kwaesok_target", "jeungryeok_target" }
    },
    { NRaceData::JOB_SURA, { "swaeryeong", "yonggwon", "gwigeom", "gongpo", "jumagap", "pabeop", "maryeong", "hwayeom", "muyeong", "heuksin", "tusok", "mahwan" }, },
};

static const std::vector<TCacheEffect> m_vecEffects =
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // Cache
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    { CInstanceBase::EFFECT_BLOCK, "", "d:/ymir work/effect/etc/", true },
    { CInstanceBase::EFFECT_DODGE, "", "d:/ymir work/effect/etc/", true },
    { CInstanceBase::EFFECT_SUCCESS, "", "d:/ymir work/effect/success.mse", true },
    { CInstanceBase::EFFECT_FAIL, "", "d:/ymir work/effect/fail.mse", true },
#ifdef VERSION_162_ENABLED
    { CInstanceBase::EFFECT_HEALER, "", "d:/ymir work/effect/monster2/healer/healer_effect.mse", true },
#endif
*/

    { CInstanceBase::EFFECT_DUST, "", "d:/ymir work/effect/etc/dust/dust.mse", true },
    { CInstanceBase::EFFECT_HORSE_DUST, "", "d:/ymir work/effect/etc/dust/running_dust.mse", true },
    { CInstanceBase::EFFECT_HIT, "", "d:/ymir work/effect/hit/blow_1/blow_1_low.mse", true },
    { CInstanceBase::EFFECT_HPUP_RED, "", "d:/ymir work/effect/etc/recuperation/drugup_red.mse", true },
    { CInstanceBase::EFFECT_SPUP_BLUE, "", "d:/ymir work/effect/etc/recuperation/drugup_blue.mse", true },
    { CInstanceBase::EFFECT_SPEEDUP_GREEN, "", "d:/ymir work/effect/etc/recuperation/drugup_green.mse", true },
    { CInstanceBase::EFFECT_DXUP_PURPLE, "", "d:/ymir work/effect/etc/recuperation/drugup_purple.mse", true },
    { CInstanceBase::EFFECT_AUTO_HPUP, "", "d:/ymir work/effect/etc/recuperation/autodrugup_red.mse", true },
    { CInstanceBase::EFFECT_AUTO_SPUP, "", "d:/ymir work/effect/etc/recuperation/autodrugup_blue.mse", true },
    { CInstanceBase::EFFECT_RAMADAN_RING_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item1.mse", true },
    { CInstanceBase::EFFECT_HALLOWEEN_CANDY_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item2.mse", true },
    { CInstanceBase::EFFECT_HAPPINESS_RING_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item3.mse", true },
    { CInstanceBase::EFFECT_LOVE_PENDANT_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item4.mse", true },
//#ifdef ENABLE_VOICE_CHAT
    { CInstanceBase::EFFECT_VOICE_CHAT, "", "d:/ymir work/effect/voice_chat/voice.mse", true },
//#endif
    { CInstanceBase::EFFECT_PENETRATE, "Bip01", "d:/ymir work/effect/hit/gwantong.mse", true },
    { CInstanceBase::EFFECT_FIRECRACKER, "", "d:/ymir work/effect/etc/firecracker/newyear_firecracker.mse", true },
    { CInstanceBase::EFFECT_SPIN_TOP, "", "d:/ymir work/effect/etc/firecracker/paing_i.mse", true },
    { CInstanceBase::EFFECT_SELECT, "", "d:/ymir work/effect/etc/click/click_select.mse", true },
    { CInstanceBase::EFFECT_TARGET, "", "d:/ymir work/effect/etc/click/click_glow_select.mse", true },
    { CInstanceBase::EFFECT_STUN, "Bip01 Head", "d:/ymir work/effect/etc/stun/stun.mse", true },
    { CInstanceBase::EFFECT_CRITICAL, "Bip01 R Hand", "d:/ymir work/effect/hit/critical.mse", true },
    { CInstanceBase::EFFECT_DAMAGE_TARGET, "", "d:/ymir work/effect/affect/damagevalue/target.mse", true },
    { CInstanceBase::EFFECT_DAMAGE_NOT_TARGET, "", "d:/ymir work/effect/affect/damagevalue/nontarget.mse", true },
    { CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE, "", "d:/ymir work/effect/affect/damagevalue/damage.mse", true },
    { CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE2, "", "d:/ymir work/effect/affect/damagevalue/damage_1.mse", true },
    { CInstanceBase::EFFECT_DAMAGE_POISON, "", "d:/ymir work/effect/affect/damagevalue/poison.mse", true },
    { CInstanceBase::EFFECT_DAMAGE_MISS, "", "d:/ymir work/effect/affect/damagevalue/miss.mse", true },
    { CInstanceBase::EFFECT_DAMAGE_TARGETMISS, "", "d:/ymir work/effect/affect/damagevalue/target_miss.mse", true },
    { CInstanceBase::EFFECT_DAMAGE_CRITICAL, "", "d:/ymir work/effect/affect/damagevalue/critical.mse", true },
    { CInstanceBase::EFFECT_DAMAGE_PENETRATE, "", "d:/ymir work/effect/hit/gwantong.mse", true },
    { CInstanceBase::EFFECT_LEVELUP_ON_14_FOR_GERMANY, "", "season1/effect/paymessage_warning.mse", true },
    { CInstanceBase::EFFECT_LEVELUP_UNDER_15_FOR_GERMANY, "", "season1/effect/paymessage_decide.mse", true },
    { CInstanceBase::EFFECT_PERCENT_DAMAGE1, "", "d:/ymir work/effect/hit/percent_damage1.mse", true },
    { CInstanceBase::EFFECT_PERCENT_DAMAGE2, "", "d:/ymir work/effect/hit/percent_damage2.mse", true },
    { CInstanceBase::EFFECT_PERCENT_DAMAGE3, "", "d:/ymir work/effect/hit/percent_damage3.mse", true },
#ifdef ENABLE_SASH_SYSTEM
    { CInstanceBase::EFFECT_SASH_SUCCEDED, "", "d:/ymir work/effect/etc/buff/buff_item6.mse", true },
    { CInstanceBase::EFFECT_SASH_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item7.mse", true },
#endif
#ifdef WJ_COMBAT_ZONE
    { CInstanceBase::EFFECT_COMBAT_ZONE_POTION, "", "d:/ymir work/effect/etc/buff/buff_item12.mse", true },
#endif

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // No cache
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUILDING_CONSTRUCTION_SMALL, "", "d:/ymir work/guild/effect/10_construction.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUILDING_CONSTRUCTION_LARGE, "", "d:/ymir work/guild/effect/20_construction.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUILDING_UPGRADE, "", "d:/ymir work/guild/effect/20_upgrade.mse", false },
#ifdef __BUFFI_SUPPORT__
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUFFI_HOSIN, "", "d:/ymir work/pc/shaman/effect/3hosin_loop.mse", false},
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUFFI_GICHEON, "Bip01 R Hand", "d:/ymir work/pc/shaman/effect/6gicheon_hand.mse", false},
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUFFI_KWAESOK, "", "d:/ymir work/pc/shaman/effect/10kwaesok_loop.mse", false},
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUFFI_JEUNGRYEO, "Bip01 L Hand", "d:/ymir work/pc/shaman/effect/jeungryeok_hand.mse", false},
#endif
*/
#ifdef __BUFFI_SUPPORT__
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUFFI_HOSIN, "", "d:/ymir work/pc/shaman/effect/3hosin_loop.mse", false},
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUFFI_GICHEON, "Bip01 R Hand", "d:/ymir work/pc/shaman/effect/6gicheon_hand.mse", false},
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUFFI_KWAESOK, "", "d:/ymir work/pc/shaman/effect/10kwaesok_loop.mse", false},
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BUFFI_JEUNGRYEO, "Bip01 L Hand", "d:/ymir work/pc/shaman/effect/jeungryeok_hand.mse", false},
#endif
    { CInstanceBase::EFFECT_SPAWN_APPEAR, "Bip01", "d:/ymir work/effect/etc/appear_die/monster_appear.mse", false },
    { CInstanceBase::EFFECT_SPAWN_DISAPPEAR, "Bip01", "d:/ymir work/effect/etc/appear_die/monster_die.mse", false },
    { CInstanceBase::EFFECT_FLAME_ATTACK, "equip_right_hand", "d:/ymir work/effect/hit/blow_flame/flame_3_weapon.mse", false },
    { CInstanceBase::EFFECT_FLAME_HIT, "", "d:/ymir work/effect/hit/blow_flame/flame_3_blow.mse", false },
    { CInstanceBase::EFFECT_FLAME_ATTACH, "", "d:/ymir work/effect/hit/blow_flame/flame_3_body.mse", false },
    { CInstanceBase::EFFECT_ELECTRIC_ATTACK, "equip_right", "d:/ymir work/effect/hit/blow_electric/light_1_weapon.mse", false },
    { CInstanceBase::EFFECT_ELECTRIC_HIT, "", "d:/ymir work/effect/hit/blow_electric/light_1_blow.mse", false },
    { CInstanceBase::EFFECT_ELECTRIC_ATTACH, "", "d:/ymir work/effect/hit/blow_electric/light_1_body.mse", false },
    { CInstanceBase::EFFECT_LEVELUP, "", "d:/ymir work/effect/etc/levelup_1/level_up.mse", false },
    { CInstanceBase::EFFECT_SKILLUP, "", "d:/ymir work/effect/etc/skillup/skillup_1.mse", false },
    { CInstanceBase::EFFECT_BOSS, "", "d:/ymir work/effect/boss.mse", false },

    { CInstanceBase::EFFECT_EMPIRE + 1, "Bip01", "d:/ymir work/effect/etc/empire/empire_A.mse", false },
    { CInstanceBase::EFFECT_EMPIRE + 2, "Bip01", "d:/ymir work/effect/etc/empire/empire_B.mse", false },
    { CInstanceBase::EFFECT_EMPIRE + 3, "Bip01", "d:/ymir work/effect/etc/empire/empire_C.mse", false },

#ifdef ENABLE_QUEEN_NETHIS
    { CInstanceBase::EFFECT_SNAKE_REGEN, "", "d:/ymir work/effect/monster2/snake_circle_snake.mse", false },
#endif

    { CInstanceBase::EFFECT_METIN_QUEUE, "", "d:/ymir work/effect/ivan/metin_loop_effect/queue_effect.mse", false },

    { CInstanceBase::EFFECT_WEAPON + 1, "equip_right_hand", "d:/ymir work/pc/warrior/effect/geom_sword_loop.mse", false },
    { CInstanceBase::EFFECT_WEAPON + 2, "equip_right_hand", "d:/ymir work/pc/warrior/effect/geom_spear_loop.mse", false },

    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_YMIR, "Bip01", "locale/de/effect/gm.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_POISON, "Bip01", "d:/ymir work/effect/hit/blow_poison/poison_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_SLOW, "", "d:/ymir work/effect/affect/slow.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_STUN, "Bip01 Head", "d:/ymir work/effect/etc/stun/stun_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_DUNGEON_READY, "", "d:/ymir work/effect/etc/ready/ready.mse", false },

    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_CHEONGEUN, "", "d:/ymir work/pc/warrior/effect/gyeokgongjang_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_GYEONGGONG, "", "d:/ymir work/pc/assassin/effect/gyeonggong_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_GWIGEOM, "Bip01 R Finger2", "d:/ymir work/pc/sura/effect/gwigeom_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_GONGPO, "", "d:/ymir work/pc/sura/effect/fear_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_JUMAGAP, "", "d:/ymir work/pc/sura/effect/jumagap_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_HOSIN, "", "d:/ymir work/pc/shaman/effect/3hosin_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BOHO, "", "d:/ymir work/pc/shaman/effect/boho_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_KWAESOK, "", "d:/ymir work/pc/shaman/effect/10kwaesok_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_HEUKSIN, "", "d:/ymir work/pc/sura/effect/heuksin_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_MUYEONG, "", "d:/ymir work/pc/sura/effect/muyeong_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_FIRE, "Bip01", "d:/ymir work/effect/hit/hwayeom_loop_1.mse", false },
    
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_GICHEON, "Bip01 R Hand", "d:/ymir work/pc/shaman/effect/6gicheon_hand.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_JEUNGRYEOK, "Bip01 L Hand", "d:/ymir work/pc/shaman/effect/jeungryeok_hand.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_PABEOP, "Bip01 Head", "d:/ymir work/pc/sura/effect/pabeop_loop.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_FALLEN_CHEONGEUN, "", "d:/ymir work/pc/warrior/effect/gyeokgongjang_loop.mse", false },

    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_WAR_FLAG1, "", "d:/ymir work/effect/etc/guild_war_flag/flag_red.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_WAR_FLAG2, "", "d:/ymir work/effect/etc/guild_war_flag/flag_blue.mse", false },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_WAR_FLAG3, "", "d:/ymir work/effect/etc/guild_war_flag/flag_yellow.mse", false },

    { CInstanceBase::EFFECT_AFFECT + 45, "", "d:/ymir work/pc/shaman/effect/chunwoon_4_target.mse", false },
    { CInstanceBase::EFFECT_AFFECT + 46, "Bip01", "d:/ymir work/pc/shaman/effect/chunwoon_moojuk.mse", false },

    // SWORD
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SWORD_REFINED7, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_7.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SWORD_REFINED8, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_8.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SWORD_REFINED9, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_9.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SWORD_REFINED10, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_7th.mse", true },

    // BOW
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BOW_REFINED7, "PART_WEAPON_LEFT", "d:/ymir work/pc/common/effect/sword/sword_7_b.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BOW_REFINED8, "PART_WEAPON_LEFT", "d:/ymir work/pc/common/effect/sword/sword_8_b.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BOW_REFINED9, "PART_WEAPON_LEFT", "d:/ymir work/pc/common/effect/sword/sword_9_b.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BOW_REFINED10, "PART_WEAPON_LEFT", "d:/ymir work/pc/common/effect/sword/sword_7th_b.mse", true },

    // FAN
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_FANBELL_REFINED7, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_7_f.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_FANBELL_REFINED8, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_8_f.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_FANBELL_REFINED9, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_9_f.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_FANBELL_REFINED10, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_7th_f.mse", true },

    // DEGER RIGHT
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SMALLSWORD_REFINED7, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_7_s.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SMALLSWORD_REFINED8, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_8_s.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SMALLSWORD_REFINED9, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_9_s.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SMALLSWORD_REFINED10, "PART_WEAPON", "d:/ymir work/pc/common/effect/sword/sword_7th_s.mse", true },

    // DEGER LEFT
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SMALLSWORD_REFINED7_LEFT, "PART_WEAPON_LEFT", "d:/ymir work/pc/common/effect/sword/sword_7_s.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SMALLSWORD_REFINED8_LEFT, "PART_WEAPON_LEFT", "d:/ymir work/pc/common/effect/sword/sword_8_s.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SMALLSWORD_REFINED9_LEFT, "PART_WEAPON_LEFT", "d:/ymir work/pc/common/effect/sword/sword_9_s.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_SMALLSWORD_REFINED10_LEFT, "PART_WEAPON_LEFT", "d:/ymir work/pc/common/effect/sword/sword_7th_s.mse", true },

    // BODY
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BODYARMOR_REFINED7, "Bip01", "d:/ymir work/pc/common/effect/armor/armor_7.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BODYARMOR_REFINED8, "Bip01", "d:/ymir work/pc/common/effect/armor/armor_8.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BODYARMOR_REFINED9, "Bip01", "d:/ymir work/pc/common/effect/armor/armor_9.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BODYARMOR_REFINED10, "Bip01", "d:/ymir work/pc/common/effect/armor/armor_7th_01.mse", true },

    // BODY SPECIAL
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BODYARMOR_SPECIAL, "Bip01", "d:/ymir work/pc/common/effect/armor/armor-4-2-1.mse", true },
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BODYARMOR_SPECIAL2, "Bip01", "d:/ymir work/pc/common/effect/armor/armor-4-2-2.mse", true },

#ifdef ENABLE_LVL115_ARMOR_EFFECT
    { CInstanceBase::EFFECT_REFINED + CInstanceBase::EFFECT_BODYARMOR_SPECIAL3, "Bip01", "d:/ymir work/pc/common/effect/armor/armor-5-1.mse", true },
#endif

#ifdef ENABLE_SASH_SYSTEM
    { CInstanceBase::EFFECT_REFINED + 22, "Bip01", "d:/ymir work/pc/common/effect/armor/acc_01.mse", true },
#endif

    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_HOSIN_PERFECT,"", "d:/ymir work/pc/shaman/effect/3hosin_loop_perfect.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BOHO_PERFECT,"", "d:/ymir work/pc/shaman/effect/boho_loop_perfect.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_KWAESOK_PERFECT,"", "d:/ymir work/pc/shaman/effect/10kwaesok_loop_perfect.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_GICHEON_PERFECT,"Bip01 R Hand", "d:/ymir work/pc/shaman/effect/6gicheon_hand_perfect.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_JEUNGRYEOK_PERFECT,"Bip01 L Hand", "d:/ymir work/pc/shaman/effect/jeungryeok_hand_perfect.mse", true },

#ifdef ENABLE_AUTOMATIC_HUNTING_SYSTEM
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_AUTO, "Bip01", "d:/ymir work/effect/autohunt/autohunt.mse", true },
#endif

    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BATTLE_ROYALE_SLOW_1, "", "d:/ymir work/effect/battleroyale/spiderline_01.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BATTLE_ROYALE_SLOW_2, "", "d:/ymir work/effect/battleroyale/spiderline_02.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_BATTLE_ROYALE_SLOW_3, "", "d:/ymir work/effect/battleroyale/spiderline_03.mse", true },


#ifdef ENABLE_MELEY_LAIR_DUNGEON
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_STATUE1, "", "d:/ymir work/effect/monster2/redd_moojuk.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_STATUE2, "", "d:/ymir work/effect/monster2/redd_moojuk.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_STATUE3, "", "d:/ymir work/effect/monster2/redd_moojuk_blue.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_STATUE4, "", "d:/ymir work/effect/monster2/redd_moojuk_green.mse", true },
#endif

#ifdef ENABLE_ZODIAC_MISSION
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE, "", "d:/ymir work/effect/monster2/12_shelter_in_01.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_BUYUK, "", "d:/ymir work/effect/monster2/12_shelter_in_02.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_ORTA, "", "d:/ymir work/effect/monster2/12_shelter_in_03.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_KUCUK, "", "d:/ymir work/effect/monster2/12_shelter_in_04.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_SKILL_SAFE_ZONE, "", "d:/ymir work/effect/monster2/12_shelter_in_05.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_SKILL_SAFE_ZONE_BUYUK, "", "d:/ymir work/effect/monster2/12_shelter_in_06.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_SKILL_SAFE_ZONE_ORTA, "", "d:/ymir work/effect/monster2/12_shelter_in_07.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_SKILL_SAFE_ZONE_KUCUK, "", "d:/ymir work/effect/monster2/12_shelter_in_08.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_METEOR, "", "d:/ymir work/effect/monster2/12_tiger_s3_drop.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_BEAD_RAIN, "", "d:/ymir work/effect/monster2/12_dra_s2_drop.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_FALL_ROCK, "", "d:/ymir work/effect/monster2/12_mon_s3_drop.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_ARROW_RAIN, "", "d:/ymir work/effect/monster2/12_sna_s3_drop.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_HORSE_DROP, "", "d:/ymir work/effect/monster2/12_hor_s3_drop.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_EGG_DROP, "", "d:/ymir work/effect/monster2/12_chi_s3_drop.mse", true },
    { CInstanceBase::EFFECT_AFFECT + CInstanceBase::EFFECT_DEAPO_BOOM, "", "d:/ymir work/effect/monster2/daepo_na_02_boom.mse", true },
#endif

#ifdef ENABLE_SHINING_SYSTEM
    //Efectos Armas una mano
    { CInstanceBase::EFFECT_SHINING_WEAPON + 0, "PART_WEAPON", "d:/ymir work/effect/ridack_goldblack/ridack_sword.mse", true },

//    { CInstanceBase::EFFECT_SHINING_SPECIAL + 0, "Bip01", "d:/ymir work/effect/shiningsystem/boss.mse", true },
#endif
};

static const std::vector<TEmotion> m_vecEmotions = {
    { CRaceMotionData::NAME_CLAP, "clap.msa" },
    { CRaceMotionData::NAME_CHEERS_1, "cheers_1.msa" },
    { CRaceMotionData::NAME_CHEERS_2, "cheers_2.msa" },
    { CRaceMotionData::NAME_DANCE_1, "dance_1.msa" },
    { CRaceMotionData::NAME_DANCE_2, "dance_2.msa" },
    { CRaceMotionData::NAME_DANCE_3, "dance_3.msa" },
    { CRaceMotionData::NAME_DANCE_4, "dance_4.msa" },
    { CRaceMotionData::NAME_DANCE_5, "dance_5.msa" },
    { CRaceMotionData::NAME_DANCE_6, "dance_6.msa" },
    { CRaceMotionData::NAME_CONGRATULATION, "congratulation.msa" },
    { CRaceMotionData::NAME_FORGIVE, "forgive.msa" },
    { CRaceMotionData::NAME_ANGRY, "angry.msa" },
    { CRaceMotionData::NAME_ATTRACTIVE, "attractive.msa" },
    { CRaceMotionData::NAME_SAD, "sad.msa" },
    { CRaceMotionData::NAME_SHY, "shy.msa" },
    { CRaceMotionData::NAME_CHEERUP, "cheerup.msa" },
    { CRaceMotionData::NAME_BANTER, "banter.msa" },
    { CRaceMotionData::NAME_JOY, "joy.msa" },
    { CRaceMotionData::NAME_FRENCH_KISS_WITH_WARRIOR, "french_kiss_with_warrior.msa" },
    { CRaceMotionData::NAME_FRENCH_KISS_WITH_ASSASSIN, "french_kiss_with_assassin.msa" },
    { CRaceMotionData::NAME_FRENCH_KISS_WITH_SURA, "french_kiss_with_sura.msa" },
    { CRaceMotionData::NAME_FRENCH_KISS_WITH_SHAMAN, "french_kiss_with_shaman.msa" },
    { CRaceMotionData::NAME_KISS_WITH_WARRIOR, "kiss_with_warrior.msa" },
    { CRaceMotionData::NAME_KISS_WITH_ASSASSIN, "kiss_with_assassin.msa" },
    { CRaceMotionData::NAME_KISS_WITH_SURA, "kiss_with_sura.msa" },
    { CRaceMotionData::NAME_KISS_WITH_SHAMAN, "kiss_with_shaman.msa" },
    { CRaceMotionData::NAME_SLAP_HIT_WITH_WARRIOR, "slap_hit.msa" },
    { CRaceMotionData::NAME_SLAP_HIT_WITH_ASSASSIN, "slap_hit.msa" },
    { CRaceMotionData::NAME_SLAP_HIT_WITH_SURA, "slap_hit.msa" },
    { CRaceMotionData::NAME_SLAP_HIT_WITH_SHAMAN, "slap_hit.msa" },
    { CRaceMotionData::NAME_SLAP_HURT_WITH_WARRIOR, "slap_hurt.msa" },
    { CRaceMotionData::NAME_SLAP_HURT_WITH_ASSASSIN, "slap_hurt.msa" },
    { CRaceMotionData::NAME_SLAP_HURT_WITH_SURA, "slap_hurt.msa" },
    { CRaceMotionData::NAME_SLAP_HURT_WITH_SHAMAN, "slap_hurt.msa" },
};

static const std::vector<TEmoticon> m_vecEmoticons =
{
    { CInstanceBase::EFFECT_EMOTICON + 0, "", string_path({ EMOTICON_PATH, "sweat.mse" }), "(황당)" },
    { CInstanceBase::EFFECT_EMOTICON + 1, "", string_path({ EMOTICON_PATH, "money.mse" }), "(돈)" },
    { CInstanceBase::EFFECT_EMOTICON + 2, "", string_path({ EMOTICON_PATH, "happy.mse" }), "(기쁨)" },
    { CInstanceBase::EFFECT_EMOTICON + 3, "", string_path({ EMOTICON_PATH, "love_s.mse" }), "(좋아)" },
    { CInstanceBase::EFFECT_EMOTICON + 4, "", string_path({ EMOTICON_PATH, "love_l.mse" }), "(사랑)" },
    { CInstanceBase::EFFECT_EMOTICON + 5, "", string_path({ EMOTICON_PATH, "angry.mse" }), "(분노)" },
    { CInstanceBase::EFFECT_EMOTICON + 6, "", string_path({ EMOTICON_PATH, "aha.mse" }), "(아하)" },
    { CInstanceBase::EFFECT_EMOTICON + 7, "", string_path({ EMOTICON_PATH, "gloom.mse" }), "(우울)" },
    { CInstanceBase::EFFECT_EMOTICON + 8, "", string_path({ EMOTICON_PATH, "sorry.mse" }), "(죄송)" },
    { CInstanceBase::EFFECT_EMOTICON + 9, "", string_path({ EMOTICON_PATH, "!_mix_back.mse" }), "(!)" },
    { CInstanceBase::EFFECT_EMOTICON + 10, "", string_path({ EMOTICON_PATH, "question.mse" }), "(?)" },
    { CInstanceBase::EFFECT_EMOTICON + 11, "", string_path({ EMOTICON_PATH, "fish.mse" }), "(fish)" },
};

static const std::vector<TRaceInfo> m_vecRaceData =
{
    { "warrior_m.msm", "d:/ymir work/pc/warrior/" },
    { "assassin_w.msm", "d:/ymir work/pc/assassin/" },
    { "sura_m.msm", "d:/ymir work/pc/sura/" },
    { "shaman_w.msm", "d:/ymir work/pc/shaman/" },

    { "warrior_w.msm", "d:/ymir work/pc2/warrior/" },
    { "assassin_m.msm", "d:/ymir work/pc2/assassin/" },
    { "sura_w.msm", "d:/ymir work/pc2/sura/" },
    { "shaman_m.msm", "d:/ymir work/pc2/shaman/" },
};

static const std::vector<TSoundFileName> m_vecUseSoundFileName =
{
    { CPythonItem::USESOUND_DEFAULT, "sound/ui/drop.wav" },
    { CPythonItem::USESOUND_ACCESSORY, "sound/ui/equip_ring_amulet.wav" },
    { CPythonItem::USESOUND_ARMOR, "sound/ui/equip_metal_armor.wav" },
    { CPythonItem::USESOUND_BOW, "sound/ui/equip_bow.wav" },
    { CPythonItem::USESOUND_WEAPON, "sound/ui/equip_metal_weapon.wav" },
    { CPythonItem::USESOUND_POTION, "sound/ui/eat_potion.wav" },
    { CPythonItem::USESOUND_PORTAL, "sound/ui/potal_scroll.wav" },
};

static const std::vector<TSoundFileName> m_vecDropSoundFileName =
{
    { CPythonItem::DROPSOUND_DEFAULT, "sound/ui/drop.wav" },
    { CPythonItem::DROPSOUND_ACCESSORY, "sound/ui/equip_ring_amulet.wav" },
    { CPythonItem::DROPSOUND_ARMOR, "sound/ui/equip_metal_armor.wav" },
    { CPythonItem::DROPSOUND_BOW, "sound/ui/equip_bow.wav" },
    { CPythonItem::DROPSOUND_WEAPON, "sound/ui/equip_metal_weapon.wav" },
};

static const std::vector<TFlyEffects> m_vecFlyEffects =
{
    { CFlyingManager::FLY_EXP, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_yellow_small2.msf" },
    { CFlyingManager::FLY_HP_MEDIUM, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_red_small.msf" },
    { CFlyingManager::FLY_HP_BIG, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_red_big.msf" },
    { CFlyingManager::FLY_SP_SMALL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_blue_warrior_small.msf" },
    { CFlyingManager::FLY_SP_MEDIUM, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_blue_small.msf" },
    { CFlyingManager::FLY_SP_BIG, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_blue_big.msf" },
    { CFlyingManager::FLY_FIREWORK1, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_1.msf" },
    { CFlyingManager::FLY_FIREWORK2, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_2.msf" },
    { CFlyingManager::FLY_FIREWORK3, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_3.msf" },
    { CFlyingManager::FLY_FIREWORK4, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_4.msf" },
    { CFlyingManager::FLY_FIREWORK5, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_5.msf" },
    { CFlyingManager::FLY_FIREWORK6, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_6.msf" },
    { CFlyingManager::FLY_FIREWORK_XMAS, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_xmas.msf" },
    { CFlyingManager::FLY_CHAIN_LIGHTNING, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/pc/shaman/effect/pokroe.msf" },
    { CFlyingManager::FLY_HP_SMALL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_red_smallest.msf" },
    { CFlyingManager::FLY_SKILL_MUYEONG, CFlyingManager::INDEX_FLY_TYPE_AUTO_FIRE, "d:/ymir work/pc/sura/effect/muyeong_fly.msf" },
#ifdef ENABLE_QUIVER_SYSTEM
    { CFlyingManager::FLY_QUIVER_ATTACK_NORMAL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/pc/assassin/effect/arrow_02.msf" },
#endif
#ifdef ENABLE_YOHARA_SYSTEM
    { CFlyingManager::FLY_CONQUEROR_EXP, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_orange_small.msf" },
#endif
};

class CPythonPlayerSettingsModule : public CSingleton<CPythonPlayerSettingsModule>
{
    public:
        CPythonPlayerSettingsModule();
        virtual ~CPythonPlayerSettingsModule();

        static auto Load() -> void;
        static auto IsVisibleInstance(uint32_t dwVID, uint8_t bType, uint32_t dwVnum, uint32_t dwDistance) -> bool;

    protected:
        static auto __RegisterCacheMotionData(CRaceData* pRaceData, const uint16_t wMotionMode, const uint16_t wMotionIndex, const char* c_szFileName, const uint8_t byPercentage = 100) -> void;
        static auto __SetIntroMotions(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
        static auto __SetGeneralMotions(CRaceData* pRaceData, const uint16_t wMotionMode, const std::string& c_rstrFolderName) -> void;

        static auto __LoadFishing(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
        static auto __LoadCombo(CRaceData* pRaceData, const uint16_t wMotionMode) -> void;
        static auto __LoadWeaponMotion(CRaceData* pRaceData, WORD wMotionMode, const std::string & c_rstrFolderName) -> void;
        static auto __LoadHorse(CRaceData * pRaceData, const std::string & c_rstrFolderName) -> void;
        static auto __LoadSkill(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
        static auto __LoadGuildSkill(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
        static auto __RegisterSharedEmotionAnis(CRaceData* pRaceData, uint16_t wMotionMode, const std::string& c_rstrFolderName) -> void;
        static auto __LoadEmotions(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
        static auto __LoadGameWarriorEx(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
        static auto __LoadGameAssassinEx(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
        static auto __LoadGameSuraEx(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
        static auto __LoadGameShamanEx(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
        static auto __LoadGameRace(CRaceData * pRaceData, const std::string & c_rstrFolderName) -> void;
        static auto __LoadGameNPC() -> void;
};
#endif // PY_PLAYER_SETTINGS_MODULE_HPP
