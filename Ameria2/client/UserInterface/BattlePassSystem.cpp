#include "StdAfx.h"

#ifdef ENABLE_BATTLEPASS
#include "BattlePassSystem.hpp"
#include "PythonNetworkStream.h"
#include <fstream>
#include <json.hpp>
#include <chrono>

void BattlePassManager::LoadBattlePass()
{
    m_battle_pass_missions.clear();
    m_battle_pass_tier_map.clear();

    using js = nlohmann::json;
    std::string file_name = "lib/nevoria/battlepass_config.json";

    std::ifstream ifs(file_name);
    if (!ifs.is_open())
    {
        TraceError("Check your lib/nevoria folder!, battlepass_config.json is not found!");
        return;
    }

    try
    {
        js jf = js::parse(ifs);

        if (jf.contains("general"))
        {
            js& general = jf["general"];

            if (general.contains("id") && general.contains("start_date") && general.contains("end_date"))
            {
                m_battlepass_info.id = general["id"];
                m_battlepass_info.start_date = general["start_date"];
                m_battlepass_info.end_date = general["end_date"];
            }
            else
            {
                TraceError("Error: Missing id or start_date or end_date in 'general' key in battlepass_config.json");
            }
        }
        if (jf.contains("missions"))
        {
            js& missions = jf["missions"];

            if (missions.is_array())
            {
                for (const auto& mission : missions)
                {
                    if (mission.contains("mission_index") && mission.contains("mission_type") &&
                        mission.contains("target_object") && mission.contains("target_count") &&
                        mission.contains("mission_exp_reward"))
                    {
                        const int32_t mission_type =
                            GetBattlePassMissionIndex(mission["mission_type"].get<std::string>());

                        if (mission_type == -1)
                        {
                            TraceError("Invalid Mission_Type %s", mission["mission_type"].get<std::string>().c_str());
                            continue;
                        }

                        uint16_t min_level = 1;
                        uint16_t max_level = 120;

                        if (mission.contains("min_level"))
                            min_level = mission["min_level"].get<uint16_t>();

                        if (mission.contains("max_level"))
                            max_level = mission["max_level"].get<uint16_t>();
                            
                        uint32_t mission_index = mission["mission_index"].get<uint32_t>();
                        uint32_t target_object = mission["target_object"].get<uint32_t>();
                        uint32_t target_count = mission["target_count"].get<uint32_t>();
                        uint32_t mission_exp_reward = mission["mission_exp_reward"].get<uint32_t>();

                        BattlePassMission mission_info(mission_type, target_object, target_count, mission_exp_reward,
                                                       min_level, max_level);

                        m_battle_pass_missions.emplace(mission_index, mission_info);
                    }
                    else
                    {
                        TraceError("Error: Missing required fields in one of the missions");
                    }
                }
            }
            else
            {
                TraceError("Error: 'missions' is not an array");
            }
        }
        else
        {
            TraceError("Error: 'missions' key not found in JSON");
        }

        if (jf.contains("tiers"))
        {
            js& tiers = jf["tiers"];

            if (tiers.is_array())
            {
                for (const auto& tier : tiers)
                {
                    if (tier.contains("tier") && tier.contains("needed_exp") && tier.contains("reward_vnum") &&
                        tier.contains("reward_count") && tier.contains("reward_vnum_premium") &&
                        tier.contains("reward_premium_count"))
                    {
                        uint8_t tier_index = tier["tier"].get<uint8_t>();
                        uint32_t needed_exp = tier["needed_exp"].get<uint32_t>();
                        uint32_t reward_vnum = tier["reward_vnum"].get<uint32_t>();
                        uint32_t reward_count = tier["reward_count"].get<uint32_t>();
                        uint32_t reward_vnum_premium = tier["reward_vnum_premium"].get<uint32_t>();
                        uint32_t reward_premium_count = tier["reward_premium_count"].get<uint32_t>();

                        TierInfo tier_info(needed_exp, reward_vnum, reward_count, reward_vnum_premium,
                                           reward_premium_count);

                        m_battle_pass_tier_map.emplace(tier_index, tier_info);
                    }
                    else
                    {
                        TraceError("Error: Missing required fields in one of the tiers");
                    }
                }
            }
            else
            {
                TraceError("Error: 'tiers' is not an array");
            }
        }
        else
        {
            TraceError("Error: 'tiers' key not found in JSON");
        }
    }

    catch (const nlohmann::json::exception& e)
    {
        TraceError("%s", e.what());
    }
}

PyObject* moduleBattlePassGetRemainingTime(PyObject* poSelf, PyObject* poArgs)
{
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    auto& battlePassInfo = BattlePassManager::Instance().GetBattlePassInfo();

    return Py_BuildValue("LLL", battlePassInfo.start_date, now_time_t, battlePassInfo.end_date);
}

PyObject* moduleBattlePassGetTierSize(PyObject* poSelf, PyObject* poArgs)
{
    return Py_BuildValue("i", BattlePassManager::Instance().GetBattlePassTierMap().size());
}

PyObject* moduleBattlePassGetTierObject(PyObject* poSelf, PyObject* poArgs)
{
    uint32_t tier_index{};

    if (!PyTuple_GetUnsignedInteger(poArgs, 0, &tier_index))
        return Py_BadArgument();

    const auto& tier_map = BattlePassManager::Instance().GetBattlePassTierMap();
    const auto& tier_object = tier_map.find(tier_index);
    if (tier_object == tier_map.end())
        return nullptr;

    return Py_BuildValue("iiii", tier_object->second.reward_vnum, tier_object->second.reward_count,
                         tier_object->second.reward_vnum_premium, tier_object->second.reward_premium_count);
}

PyObject* moduleBattlePassGetMissionObject(PyObject* poSelf, PyObject* poArgs)
{
    uint32_t mission_index{};

    if (!PyTuple_GetUnsignedInteger(poArgs, 0, &mission_index))
        return Py_BadArgument();

    const auto& mission_map = BattlePassManager::Instance().GetBattlePassMissionMap();
    const auto& mission_object = mission_map.find(mission_index);

    if (mission_object == mission_map.end())
        return nullptr;

    return Py_BuildValue("ikLkii", mission_object->second.mission_type, mission_object->second.target_object,
                         mission_object->second.target_count, mission_object->second.exp_reward,
                         mission_object->second.min_level, mission_object->second.max_level);
}

PyObject* moduleBattlePassGetMissionSize(PyObject* poSelf, PyObject* poArgs)
{
    return Py_BuildValue("i", BattlePassManager::Instance().GetBattlePassMissionMap().size());
}


PyObject* moduleBattlePassGetNextLevelExp(PyObject* poSelf, PyObject* poArgs)
{
    uint32_t tier_index{};

    if (!PyTuple_GetUnsignedInteger(poArgs, 0, &tier_index))
        return Py_BadArgument();

    const auto& tier_map = BattlePassManager::Instance().GetBattlePassTierMap();
    const auto& tier_object = tier_map.find(tier_index);
    if (tier_object == tier_map.end())
        return Py_BuildValue("i", 0);

    return Py_BuildValue("i", tier_object->second.needed_exp);
}

PyObject* moduleBattlePasRedeemMissionReward(PyObject* poSelf, PyObject* poArgs)
{
    uint32_t mission_index{};
    if (!PyTuple_GetUnsignedInteger(poArgs, 0, &mission_index))
        return Py_BadArgument();
    
    uint32_t is_premium_item{};
    if (!PyTuple_GetUnsignedInteger(poArgs, 1, &is_premium_item))
        return Py_BadArgument();

    char command[256 + 1];
    snprintf(command, sizeof command, "/battlepass_command %u %u %u", REDEEM_MISSION_REWARD,
             mission_index, is_premium_item);

    CPythonNetworkStream::Instance().SendChatPacket(command, CHAT_TYPE_COMMAND);
    return Py_BuildNone();
}

PyObject* moduleBattlePasRedeemMissionExp(PyObject* poSelf, PyObject* poArgs)
{
    uint32_t mission_index{};
    if (!PyTuple_GetUnsignedInteger(poArgs, 0, &mission_index))
        return Py_BadArgument();
    
    char command[256 + 1];
    snprintf(command, sizeof command, "/battlepass_command %u %u", REDEEM_MISSION_EXP,
             mission_index);

    CPythonNetworkStream::Instance().SendChatPacket(command, CHAT_TYPE_COMMAND);
    return Py_BuildNone();
}

PyObject* moduleBattlePasRequestCurrentProgress(PyObject* poSelf, PyObject* poArgs)
{
    char command[256 + 1];
    snprintf(command, sizeof command, "/battlepass_command %u", REQUEST_CURRENT_PROGRESS_DATA);
    CPythonNetworkStream::Instance().SendChatPacket(command, CHAT_TYPE_COMMAND);
    TraceError(command);
    return Py_BuildNone();
}

PyObject* moduleBattlePassRegisterClass(PyObject* poSelf, PyObject* poArgs)
{
    PyObject* addr{nullptr};

    if (!PyTuple_GetObject(poArgs, 0, &addr))
        return Py_BadArgument();

    battlepassMainClass = addr;
    return Py_BuildNone();
}

PyObject* moduleBattlePassUnregister(PyObject* poSelf, PyObject* poArgs)
{
    battlepassMainClass = nullptr;
    return Py_BuildNone();
}

void initBattlePass()
{
    static PyMethodDef s_methods[] = {
        {"GetRemainingTime", moduleBattlePassGetRemainingTime, METH_VARARGS},
        {"GetTierSize", moduleBattlePassGetTierSize,      METH_VARARGS},
        {"GetTierObject", moduleBattlePassGetTierObject,      METH_VARARGS},

        {"GetMissionSize",   moduleBattlePassGetMissionSize,   METH_VARARGS},
        {"GetMissionObject",   moduleBattlePassGetMissionObject, METH_VARARGS},
        {"GetNextLevelEXP",    moduleBattlePassGetNextLevelExp,  METH_VARARGS},

        {"RedeemMissionReward", moduleBattlePasRedeemMissionReward,    METH_VARARGS},
        {"RedeemMissionExp",    moduleBattlePasRedeemMissionExp,       METH_VARARGS},
        {"RequestCurrentProgress", moduleBattlePasRequestCurrentProgress, METH_VARARGS},

        {"RegisterClass",          moduleBattlePassRegisterClass,         METH_VARARGS},
        {"UnregisterClass",        moduleBattlePassUnregister,            METH_VARARGS},

        {nullptr,                    nullptr,                         NULL        },
    };

    PyObject* module = Py_InitModule("battlepass", s_methods);
        
    PyModule_AddIntConstant(module, "MISSION_NONE", MISSION_NONE);
    PyModule_AddIntConstant(module, "MISSION_REACH_LEVEL", MISSION_REACH_LEVEL);
    PyModule_AddIntConstant(module, "MISSION_COLLECT_GOLD", MISSION_COLLECT_GOLD);
    PyModule_AddIntConstant(module, "MISSION_COLLECT_EXP", MISSION_COLLECT_EXP);
    PyModule_AddIntConstant(module, "MISSION_KILL_PLAYER", MISSION_KILL_PLAYER);
    PyModule_AddIntConstant(module, "MISSION_KILL_MOB", MISSION_KILL_MOB);
    PyModule_AddIntConstant(module, "MISSION_CATCH_FISH", MISSION_CATCH_FISH);
    PyModule_AddIntConstant(module, "MISSION_MINE", MISSION_MINE);
    PyModule_AddIntConstant(module, "MISSION_SEND_SHOUT_MESSAGE", MISSION_SEND_SHOUT_MESSAGE);
    PyModule_AddIntConstant(module, "MISSION_UPGRADE_OBJECT", MISSION_UPGRADE_OBJECT);
    PyModule_AddIntConstant(module, "MISSION_USE_ITEM", MISSION_USE_ITEM);
    PyModule_AddIntConstant(module, "MISSION_BATTLEPASS_MAX", MISSION_BATTLEPASS_MAX);
};


/*
 *      *** Networking ***
 * Do not touch unless what you are doing from now on.
 */

bool BattlePassManager::RecvBattlePassPacket()
{
    packet_battlepass packet;
    if (!CPythonNetworkStream::Instance().Recv(sizeof(packet_battlepass), &packet))
        return false;

    switch (packet.subheader)
    {
        case SUBHEADER_SEND_CHARACTER_DATA:
        {
            /* Recv Packet and content size */
            packet_battlepass_info info;
            if (!CPythonNetworkStream::Instance().Recv(sizeof(info), &info))
                return false;

            /* Recv Character Info */
            BattlePassCharacterInfo chr_info;
            if (!CPythonNetworkStream::Instance().Recv(sizeof(chr_info), &chr_info))
                return false;

            UpdateCharacterData(chr_info);

            /* Recv Mission Info */
            while (info.mission_count > 0)
            {
                BattlePassCharacterMissionInfo mission;
                if (!CPythonNetworkStream::Instance().Recv(sizeof(mission), &mission))
                    return false;

                UpdateMissionState(mission);
                info.mission_count--;
            }

            /* Recv Tier Info */
            while (info.tier_count > 0)
            {
                BattlePassCharacterTierInfo tier;
                if (!CPythonNetworkStream::Instance().Recv(sizeof(tier), &tier))
                    return false;

                UpdateTierState(tier);
                info.tier_count--;
            }
            break;
        }
            
        case SUBHEADER_SEND_CHARACTER_PROGRESS_UPDATE:
        {
            BattlePassCharacterInfo info;
            if (!CPythonNetworkStream::Instance().Recv(sizeof(info), &info))
                return false;
            UpdateCharacterData(info);
            break;
        }

        case SUBHEADER_SEND_MISSION_PROGRESS_UPDATE:
        {
            BattlePassCharacterMissionInfo info;
            if (!CPythonNetworkStream::Instance().Recv(sizeof(info), &info))
                return false;
            UpdateMissionState(info);
            break;
        }
        
        case SUBHEADER_SEND_REWARD_UPDATE:
        {
            BattlePassCharacterTierInfo info;
            if (!CPythonNetworkStream::Instance().Recv(sizeof(info), &info))
                return false;
            UpdateTierState(info);
            break;
        }

        default:
            break;
    }
    return true;
}

void BattlePassManager::UpdateCharacterData(BattlePassCharacterInfo data)
{
    CallPyMethodBattlePass("UpdateCharacterData",
                 Py_BuildValue("(iii)", data.current_level, data.current_exp, data.premium_status));
}
void BattlePassManager::UpdateMissionState(BattlePassCharacterMissionInfo data)
{
    CallPyMethodBattlePass("UpdateMissionState",
                 Py_BuildValue("(iiii)", data.mission_index, data.progress, data.completed, data.collected));
}
void BattlePassManager::UpdateTierState(BattlePassCharacterTierInfo data) 
{
    CallPyMethodBattlePass("UpdateRewardState", Py_BuildValue("(iii)", data.mission_index, data.reward_collected,
                                                              data.reward_premium_collected));
}

#endif