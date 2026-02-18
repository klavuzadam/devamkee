/*
* @author: Amun
* @discord: amun3808
* @github: https://github.com/amun3808
* @dev: https://metin2.dev/profile/28461-amun/
* @project: https://github.com/blackdragonx61/Metin2-Official-Battle-Royale
*/

#pragma once

#include "../common/d3dtype.h"
#include "../common/tables.h"
#include "config.h"
#include <vector>

#if defined(__BL_BATTLE_ROYALE__)

class BattleRoyale : public singleton<BattleRoyale>
{
public:
	struct SMember
	{
		uint32_t playerID;
	};

	enum Settings
	{
		BR_BATTLE_CHANNEL = 99,
		BR_MAP_INDEX = 404,

		BR_MIN_PLAYER_COUNT = 2, // official = 2
		BR_MAX_PLAYER_COUNT = 500, // official = 100

		BR_ELECTRICAL_FIELD_VNUM = 20712,
		BR_FUNERARY_STONE_VNUM = 20352,

		BR_INVENTORY_SLOTS = 3 * 5, // official = 3 rows

		BR_MULTIPLIER = 10, // This is just for tests. Official is multiplier of 10
		BR_TIME_TO_CLOSE_ENTRY = 300, // official = 2 minutes and 30 seconds
		BR_TIME_TO_BATTLE = 60, // official = 60 sec
		BR_TIME_TO_FIRST_SHRINK = 20 * BR_MULTIPLIER, // official = 200 sec
		BR_TIME_DECREASE_TO_NEXT_SHRINK = 3 * BR_MULTIPLIER, // official = 30 sec
		BR_INITIAL_SHRINK_DURATION = 6 * BR_MULTIPLIER, // official = 60 sec

		BR_EXTRA_MOV_SPEED = 150, // official = 50
		BR_EXTRA_MOV_SPEED_DURATION = 3600, // official = +50% mov speed for 3 minutes
		BR_DEBUFF_VALUE = 5, // official

		BR_DAMAGE_OUTSIDE_OF_FIELD = 50, // official = 50

		BR_CROWN_AFFECT_DURATION = 3600 * 3, // official = 3 hours
		BR_HP_REGEN_WHEN_KILLING_STONE = 50, // official
		BR_HP_REGEN_WHEN_KILLING_PLAYER = 1000, // official
	};

	enum EBattleRoyaleSubheaders : uint8_t
	{
		BR_SUB_CHANGE_STATE,

		BR_SUB_REGISTER,
		BR_SUB_REGISTER_REPLY_FAIL,
		BR_SUB_REGISTER_REPLY_SUCCESS,
		BR_SUB_REGISTER_REPLY_ALREADY,
		BR_SUB_REGISTER_REPLY_FULL,

		BR_SUB_CANCEL_REGISTRATION,
		BR_SUB_CANCEL_REGISTRATION_REPLY_SUCCESS,
		BR_SUB_CANCEL_REGISTRATION_REPLY_NOT_REGISTERED,

		BR_SUB_ENTER,
		BR_SUB_ENTER_REPLY_SUCCESS,

		BR_SUB_SYNC,
	};

	enum EBattleRoyaleStates : uint8_t
	{
		BR_STATE_CLOSED,
		BR_STATE_REGISTER,
		BR_STATE_ENTER,
		BR_STATE_ENTER_CLOSED,
		BR_STATE_BATTLE,
	};

	enum EBattleRoyaleSubStates : uint8_t
	{
		BR_FIELD_STATE_NONE,
		BR_FIELD_STATE_WHITE_CIRCLE,
		BR_FIELD_STATE_SHRINK,
		BR_FIELD_STATE_SHRINKING,
		BR_FIELD_STATE_CLOSED,
	};

	enum
	{
		BATTLE_ROYALE_GC_INSERT_APPLICANT, // Current fighter
		BATTLE_ROYALE_GC_REMOVE_APPLICANT, // Current fighter
		BATTLE_ROYALE_GC_STARTABLE, // All fighters + normal core login
		BATTLE_ROYALE_GC_ENTRY_CLOSED, // Fighters that registered and didn't enter
		BATTLE_ROYALE_GC_START, // All fighters
		BATTLE_ROYALE_GC_EXIT, // BR login
		BATTLE_ROYALE_GC_OPEN_KILL_INFO, // Everyone in BR
		BATTLE_ROYALE_GC_KILL_INFO, // Everyone in BR
		BATTLE_ROYALE_GC_DEAD, // Current fighter
		BATTLE_ROYALE_GC_WHITE_CIRCLE, // Everyone in BR
		BATTLE_ROYALE_GC_RED_CIRCLE, // Everyone in BR
		BATTLE_ROYALE_GC_SHRINK, // Everyone in BR
		BATTLE_ROYALE_GC_SHRINK_DONE, // Everyone in BR
		BATTLE_ROYALE_GC_WINNER, // Current fighter
		BATTLE_ROYALE_GC_HAS_REWARD, // Change button to "Claim"
	};

public:
	BattleRoyale();
	~BattleRoyale() = default;

	void Initialize();
	void Destroy();

	bool IsEnabled() const;
	bool IsBattleChannel() const;
	bool BattleCore_IsWinner(LPCHARACTER ch) const;

	void RequestStateChange(LPCHARACTER ch, uint8_t state);
	void BattleCore_ReceiveRequestStateChange(uint8_t state, uint32_t gmPlayerID);

	void SendActionRequest(LPCHARACTER ch, uint8_t action);
	void BattleCore_ReceiveActionRequest(uint32_t playerID, uint8_t action);
	void ReceiveActionRequestReply(uint32_t playerID, uint8_t status);
	void NormalCore_ReceiveSync(uint8_t state);

	bool SetBattleRoyaleTableFromPlayerTable(LPCHARACTER ch, const TPlayerTable* t);

	void OnLogin(LPCHARACTER ch);
	void OnLogout(LPCHARACTER ch);
	bool OnAttack(LPCHARACTER attacker, LPCHARACTER victim) const;
	void OnDead(LPCHARACTER victim, LPCHARACTER killer = nullptr, bool isLogout = false);

	void BattleCore_Leave(LPCHARACTER ch);
	void BattleCore_Update(int pulse);

	void ClaimRewards(LPCHARACTER ch);

	bool IsFighter(uint32_t playerID) const;
private:
	void NotifyClient(LPCHARACTER ch, uint8_t action);
	void NotifyFighters(uint8_t action, bool checkMap = false);

	bool BattleCore_CanBeginBattle() const;
	void BattleCore_SendDeadPacket(LPCHARACTER ch, const char* killerName, uint8_t rank);
	void BattleCore_SendSurvivorPacket();
	void BattleCore_SendOpenKillInfoPacket(bool open);

	void BattleCore_SendActionRequestReply(uint32_t playerID, uint8_t sub);
	void BattleCore_OpenRegistrations();
	void BattleCore_OpenEntry();
	void BattleCore_CloseEntry();
	void BattleCore_OpenBattle();
	void BattleCore_Close();
	void BattleCore_SyncState();

	void CreateRedCircle();
	void CreateWhiteCircle();
	void BattleCore_StartShrinking();
	void BattleCore_Shrink();
	void BattleCore_StopShrinking();

	void BattleCore_ProcessFieldDamage();

	void BattleCore_SpawnChests();
	void BattleCore_SpawnSpecialChest();

	void BattleCore_SpreadPlayers();

	void BattleCore_PickWinnerAndClose();

	bool HasRewards(LPCHARACTER ch) const;

	void PushNextUpdate(int seconds);
	void BattleCore_SendClientTimer(LPCHARACTER ch);
	bool CanUpdate() const { return m_NextUpdate && thecore_pulse() >= m_NextUpdate; }
	void BattleCore_PushFieldState(uint8_t state, int secondsTillChange = -1);
	void BattleCore_SendCirclePacket(bool isRed = false);

	template<typename Func> void ForEachPlayerInside(Func f);
	template<typename Func> void ForEachFighter(Func f);
private:
	uint8_t m_State;
	uint8_t m_FieldState;

	int m_NextUpdate;
	int m_StartShrinkTime;
	int m_ShrinkDuration;

	uint8_t m_ShrinkCount;
	uint8_t m_SpecialChestSpawnTime;

	D3DXVECTOR4 m_WhiteCirclePos;
	D3DXVECTOR4 m_RedCirclePos;
	D3DXVECTOR4 m_RedCirclePosCopy;

	std::vector<uint32_t> m_PlayersInside;
	std::set<uint32_t> m_Fighters;
	std::unordered_map<uint32_t, uint32_t> m_KillsByPlayerID;
};

#endif
