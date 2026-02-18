#pragma once
#include "StdAfx.h"
#include "BattlePassUtils.hpp"

#ifdef ENABLE_BATTLEPASS
#include <ctime>

inline PyObject* battlepassMainClass{nullptr};
#define CallPyMethodBattlePass(str, ...)                                                                                     \
    if (battlepassMainClass)                                                                                        \
        PyCallClassMemberFunc(battlepassMainClass, str, __VA_ARGS__);                                               \
    else                                                                                                           \
        TraceError("BattlePassManager:: battlepassMainClass can is not valid %s", str);


class BattlePassManager : public CSingleton<BattlePassManager>
{
  public:
    void LoadBattlePass();

    BattlePassInfo& GetBattlePassInfo() { return m_battlepass_info; }
    std::unordered_map<uint32_t /*tier id*/, TierInfo>& GetBattlePassTierMap() { return m_battle_pass_tier_map; }
    std::unordered_map<uint32_t /*tier id*/, BattlePassMission>& GetBattlePassMissionMap() { return m_battle_pass_missions; }

  private:
    BattlePassInfo m_battlepass_info;
    std::unordered_map<uint32_t /*mission id*/, BattlePassMission> m_battle_pass_missions;
    std::unordered_map<uint32_t /*tier id*/, TierInfo> m_battle_pass_tier_map;

    public:
    /*
     *      *** Networking ***
     * Do not touch unless what you are doing from now on.
     */
      static bool RecvBattlePassPacket();
      static void UpdateCharacterData(BattlePassCharacterInfo data);
      static void UpdateMissionState(BattlePassCharacterMissionInfo data);
      static void UpdateTierState(BattlePassCharacterTierInfo data);
};

#endif