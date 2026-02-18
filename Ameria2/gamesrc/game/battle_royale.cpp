/*
* @author: Amun
* @discord: amun3808
* @github: https://github.com/amun3808
* @dev: https://metin2.dev/profile/28461-amun/
* @project: https://github.com/blackdragonx61/Metin2-Official-Battle-Royale
*/

#include "stdafx.h"
#include "battle_royale.h"
#include "char.h"
#include "char_manager.h"
#include "start_position.h"
#include "questmanager.h"
#include "p2p.h"
#include "config.h"
#include "buffer_manager.h"
#include "desc.h"
#include "sectree_manager.h"
#include "item.h"
#include "item_manager.h"
#include "mob_manager.h"
#include "desc_client.h"
#include <algorithm>

#if defined(__BL_BATTLE_ROYALE__)
#undef min
#undef max
// show the logs in syserr for debugging on windows
// #ifdef __WIN32__
#define BR_LOG(log_level, fmt, ...) _sys_err(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)
// #else 
// #define BR_LOG(log_level, fmt, ...) sys_log(log_level, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
// #endif

constexpr long START_X = 291600;
constexpr long START_Y = 397600;

static bool isPointInsideCircle(float x, float y, float r, float h, float k)
{
	float distance = (x - h) * (x - h) + (y - k) * (y - k);
	return distance <= r * r;
}

// TODO: AFFECT_BATTLE_ROYALE_INFINITE_STAMINA

BattleRoyale::BattleRoyale()
{
	Initialize();
}

void BattleRoyale::Initialize()
{
	m_State = BR_STATE_CLOSED;
	m_FieldState = BR_FIELD_STATE_NONE;
	m_ShrinkCount = 0;
	m_SpecialChestSpawnTime = 0;

	m_NextUpdate = 0;
	m_StartShrinkTime = 0;
	m_ShrinkDuration = 0;

	m_WhiteCirclePos = {};
	m_RedCirclePos = {};
	m_RedCirclePosCopy = {};

	m_Fighters.clear();
	m_PlayersInside.clear();
	m_KillsByPlayerID.clear();
}

void BattleRoyale::Destroy()
{
	BattleCore_Close();
}

bool BattleRoyale::IsEnabled() const
{
	return quest::CQuestManager::Instance().GetEventFlag("enable_battle_royale") > 0;
}

bool BattleRoyale::IsBattleChannel() const
{
	return g_bChannel == BR_BATTLE_CHANNEL;
}

bool BattleRoyale::IsFighter(uint32_t playerID) const
{
	return m_Fighters.find(playerID) != m_Fighters.end();
}

void BattleRoyale::PushNextUpdate(int seconds)
{
	if (seconds < 0)
		return;

	m_NextUpdate = thecore_pulse() + PASSES_PER_SEC(seconds);
	ForEachPlayerInside([&](LPCHARACTER ch) { ch->ChatPacket(CHAT_TYPE_COMMAND, "SetMinimapTimer %d", seconds); });
}

void BattleRoyale::NotifyClient(LPCHARACTER ch, uint8_t action)
{
	TPacketGCBattleRoyale packet{};
	packet.header = HEADER_GC_BATTLE_ROYALE;
	packet.subheader = action;
	packet.size = sizeof(packet);

	ch->GetDesc()->Packet(&packet, sizeof(packet));
}

void BattleRoyale::NotifyFighters(uint8_t action, bool checkMap)
{
	BR_LOG(0, "%s %u", __FUNCTION__, action);
	TPacketGCBattleRoyale packet{};
	packet.header = HEADER_GC_BATTLE_ROYALE;
	packet.subheader = action;
	packet.size = sizeof(packet);
	ForEachFighter([&](LPCHARACTER fighter) {
		if (checkMap && !fighter->IsBattleRoyale())
			return;
		fighter->GetDesc()->Packet(&packet, sizeof(packet));
		});
}

bool BattleRoyale::BattleCore_IsWinner(LPCHARACTER ch) const
{
	return ch && m_Fighters.size() == 1 && ch->GetPlayerID() == *m_Fighters.begin();
}

template<typename Func> void BattleRoyale::ForEachPlayerInside(Func f)
{
	auto& chrMgr = CHARACTER_MANAGER::Instance();
	for (auto it = m_PlayersInside.begin(); it != m_PlayersInside.end();)
	{
		if (LPCHARACTER player = chrMgr.FindByPID(*(it++)))
			f(player);
	}
}

template<typename Func> void BattleRoyale::ForEachFighter(Func f)
{
	auto& chrMgr = CHARACTER_MANAGER::Instance();
	for (auto it = m_Fighters.begin(); it != m_Fighters.end();)
	{
		if (LPCHARACTER fighter = chrMgr.FindByPID(*(it++)))
			f(fighter);
	}
}

#pragma region P2P_PACKETS

// GM can request open/close from anywhere
void BattleRoyale::RequestStateChange(LPCHARACTER ch, uint8_t state)
{
	if (!ch)
		return;

	if (m_State == state)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[BATTLE_ROYALE] The event is already in this state(%u)"), m_State);
		return;
	}

	// You can only open and close. The other states are decided by the system.
	if (state == BR_STATE_REGISTER)
	{
		if (!IsEnabled())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[BATTLE_ROYALE] The event flag is disabled. Please enable it."));
			return;
		}

		if (m_State != BR_STATE_CLOSED)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[BATTLE_ROYALE] Please close the event first."));
			return;
		}
	}
	else if (state != BR_STATE_CLOSED)
	{
		sys_err("[BATTLE_ROYALE] Unhandled packet state %u(current %u) requested by %s", state, m_State, ch->GetName());
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[BATTLE_ROYALE] Error: Unhandled state %u"), state);
		return;
	}

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[BATTLE_ROYALE] State change requested."));
	BR_LOG(0, "[BATTLE_ROYALE] Request state change by %s from %u to %u", ch->GetName(), m_State, state);

	if (IsBattleChannel())
	{
		BattleCore_ReceiveRequestStateChange(state, ch->GetPlayerID());
	}
	else
	{
		TPacketGGBattleRoyale pack{};
		pack.header = HEADER_GG_BATTLE_ROYALE;
		pack.subheader = BR_SUB_CHANGE_STATE;
		pack.state = state;
		pack.dwPID = ch->GetPlayerID();
		P2P_MANAGER::instance().Send(&pack, sizeof(pack));
	}
}

void BattleRoyale::BattleCore_ReceiveRequestStateChange(uint8_t state, uint32_t gmPlayerID)
{
	if (!IsBattleChannel())
		return;

	BR_LOG(0, "[BATTLE_ROYALE] Received request to change state by pid#%u. Current state %u, new state %u", gmPlayerID, m_State, state);

	if (state == BR_STATE_REGISTER && IsEnabled())
		BattleCore_OpenRegistrations();
	else if (state == BR_STATE_CLOSED)
		BattleCore_Close();
}

void BattleRoyale::SendActionRequest(LPCHARACTER ch, uint8_t action)
{
	if (!ch || ch->IsBattleRoyale() || HasRewards(ch))
		return;

	BR_LOG(0, "%s %u %u", __FUNCTION__, ch->GetPlayerID(), action);

	if (action == BR_SUB_REGISTER || action == BR_SUB_ENTER)
	{
		if (ch->GetParty())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Battle Royale] You can't do this in a party."));
			return;
		}
	}
	else if (action != BR_SUB_CANCEL_REGISTRATION)
	{
		sys_err("Unhandled action %u requested by %s", action, ch->GetName());
		return;
	}

	if (IsBattleChannel())
	{
		BattleCore_ReceiveActionRequest(ch->GetPlayerID(), action);
	}
	else
	{
		TPacketGGBattleRoyale pack{};
		pack.header = HEADER_GG_BATTLE_ROYALE;
		pack.subheader = action;
		pack.dwPID = ch->GetPlayerID();
		P2P_MANAGER::instance().Send(&pack, sizeof(pack));
	}
}

void BattleRoyale::BattleCore_ReceiveActionRequest(uint32_t playerID, uint8_t action)
{
	if (!IsBattleChannel())
		return;

	switch (action)
	{
	case BR_SUB_REGISTER:
	{
		if (m_State != BR_STATE_REGISTER && m_State != BR_STATE_ENTER)
		{
			BR_LOG(0, "%s pid %u BR_SUB_REGISTER_REPLY_FAIL", __FUNCTION__, playerID);
			BattleCore_SendActionRequestReply(playerID, BR_SUB_REGISTER_REPLY_FAIL);
			return;
		}

		if (m_Fighters.size() >= BR_MAX_PLAYER_COUNT)
		{
			BR_LOG(0, "%s full.", __FUNCTION__);
			BattleCore_SendActionRequestReply(playerID, BR_SUB_REGISTER_REPLY_FULL);
			return;
		}

		if (IsFighter(playerID))
		{
			BR_LOG(0, "%s pid %u is already registered.", __FUNCTION__, playerID);
			BattleCore_SendActionRequestReply(playerID, BR_SUB_REGISTER_REPLY_ALREADY);
			return;
		}

		m_Fighters.emplace(playerID);
		m_KillsByPlayerID.emplace(playerID, 0);

		BR_LOG(0, "%s registered pid %u", __FUNCTION__, playerID);
		BattleCore_SendActionRequestReply(playerID, BR_SUB_REGISTER_REPLY_SUCCESS);

		if (m_Fighters.size() >= BR_MIN_PLAYER_COUNT && m_State != BR_STATE_ENTER)
			BattleCore_OpenEntry();
	} break;

	case BR_SUB_ENTER:
	{
		if (m_State != BR_STATE_ENTER)
			return;

		if (!IsFighter(playerID))
		{
			if (m_Fighters.size() >= BR_MAX_PLAYER_COUNT)
			{
				BR_LOG(0, "%s full. BR_STATE_ENTER", __FUNCTION__);
				BattleCore_SendActionRequestReply(playerID, BR_SUB_REGISTER_REPLY_FULL);
				return;
			}

			m_Fighters.emplace(playerID);
			m_KillsByPlayerID.emplace(playerID, 0);
			BR_LOG(0, "%s accept warp pid %u wasn't registered. Registered now.", __FUNCTION__, playerID);
			BattleCore_SendActionRequestReply(playerID, BR_SUB_REGISTER_REPLY_SUCCESS);
		}

		BR_LOG(0, "%s accept warp pid %u .", __FUNCTION__, playerID);
		BattleCore_SendActionRequestReply(playerID, BR_SUB_ENTER_REPLY_SUCCESS);
	} break;

	case BR_SUB_CANCEL_REGISTRATION:
	{
		if (m_State != BR_STATE_REGISTER)
			return;

		if (!IsFighter(playerID))
		{
			BattleCore_SendActionRequestReply(playerID, BR_SUB_CANCEL_REGISTRATION_REPLY_NOT_REGISTERED);
			return;
		}

		m_Fighters.erase(playerID);
		m_KillsByPlayerID.erase(playerID);

		BR_LOG(0, "%s pid %u cancelled registration.", __FUNCTION__, playerID);
		BattleCore_SendActionRequestReply(playerID, BR_SUB_CANCEL_REGISTRATION_REPLY_SUCCESS);
	} break;
	}
}

void BattleRoyale::BattleCore_SendActionRequestReply(uint32_t playerID, uint8_t sub)
{
	if (!IsBattleChannel())
		return;

	ReceiveActionRequestReply(playerID, sub);

	BR_LOG(0, "%s %u %u", __FUNCTION__, playerID, sub);
	TPacketGGBattleRoyale pack{};
	pack.header = HEADER_GG_BATTLE_ROYALE;
	pack.subheader = sub;
	pack.state = m_State;
	pack.dwPID = playerID;
	P2P_MANAGER::instance().Send(&pack, sizeof(pack));
}

void BattleRoyale::ReceiveActionRequestReply(uint32_t playerID, uint8_t action)
{
	bool isRegister = BR_SUB_REGISTER_REPLY_SUCCESS == action;
	bool isCancel = BR_SUB_CANCEL_REGISTRATION_REPLY_SUCCESS == action;

	if (isCancel)
		m_Fighters.erase(playerID);
	else if (isRegister)
		m_Fighters.emplace(playerID);

	if ((isCancel || isRegister) && m_State == BR_STATE_REGISTER && m_Fighters.size() < BR_MIN_PLAYER_COUNT)
	{
		ForEachFighter([&](LPCHARACTER fighter) { fighter->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Battle Royale registration status: (%u/%u)"), m_Fighters.size(), BR_MIN_PLAYER_COUNT); });
	}

	LPCHARACTER ch = CHARACTER_MANAGER::Instance().FindByPID(playerID);
	if (!ch)
		return;

	BR_LOG(0, "%s %u %u", __FUNCTION__, playerID, action);

	switch (action)
	{
	case BR_SUB_REGISTER_REPLY_SUCCESS:
		NotifyClient(ch, BATTLE_ROYALE_GC_INSERT_APPLICANT);
		if (m_State == BR_STATE_ENTER)
			NotifyClient(ch, BATTLE_ROYALE_GC_STARTABLE);
		break;
	case BR_SUB_REGISTER_REPLY_FAIL:
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Battle royale] Applications are not available at this time."));
		break;
	case BR_SUB_REGISTER_REPLY_ALREADY:
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Battle royale] You are already registered."));
		break;
	case BR_SUB_REGISTER_REPLY_FULL:
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Battle royale] The battle royale is already full."));
		break;

	case BR_SUB_CANCEL_REGISTRATION_REPLY_SUCCESS:
		NotifyClient(ch, BATTLE_ROYALE_GC_REMOVE_APPLICANT);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Battle royale] Your registration has been cancelled."));
		break;

	case BR_SUB_ENTER_REPLY_SUCCESS:
		ch->WarpSet(START_X, START_Y, BattleRoyale::BR_MAP_INDEX);
		break;
	}
}

void BattleRoyale::BattleCore_SyncState()
{
	if (!IsBattleChannel())
		return;

	TPacketGGBattleRoyale pack{};
	pack.header = HEADER_GG_BATTLE_ROYALE;
	pack.subheader = BR_SUB_SYNC;
	pack.state = m_State;
	pack.dwPID = 0;
	P2P_MANAGER::instance().Send(&pack, sizeof(pack));
	BR_LOG(0, "%s", __FUNCTION__);

	NormalCore_ReceiveSync(m_State);
}

void BattleRoyale::NormalCore_ReceiveSync(uint8_t state)
{
	BR_LOG(0, "%s %u", __FUNCTION__, state);

	switch (state)
	{
	case BR_STATE_REGISTER:
		SendNotice(LC_TEXT("[Battle Royale] The registrations are now opened!"));
		break;
	case BR_STATE_CLOSED:
		NotifyFighters(BATTLE_ROYALE_GC_EXIT);
		break;
	case BR_STATE_ENTER:
		NotifyFighters(BATTLE_ROYALE_GC_STARTABLE);
		break;
	case BR_STATE_ENTER_CLOSED:
		NotifyFighters(BATTLE_ROYALE_GC_ENTRY_CLOSED); // Notify all fighters that didn't enter
		break;
	case BR_STATE_BATTLE:
		break;
	}

	if (!IsBattleChannel())
	{
		if (state != BR_STATE_ENTER) // We're still sending them messages in this state(or the following)
			Initialize(); // Wipe them.

		m_State = state;
	}
}

#pragma endregion P2P_PACKETS

bool BattleRoyale::BattleCore_CanBeginBattle() const
{
	return IsBattleChannel() && m_Fighters.size() >= BR_MIN_PLAYER_COUNT && m_PlayersInside.size() >= BR_MIN_PLAYER_COUNT;
}

void BattleRoyale::BattleCore_OpenRegistrations()
{
	if (!IsBattleChannel())
		return;

	if (m_State != BR_STATE_CLOSED)
	{
		sys_err("[BATTLE_ROYALE] Cannot change state to OPEN if it's not closed. Current state %u", m_State);
		return;
	}

	BR_LOG(0, "%s", __FUNCTION__);

	Initialize();
	m_State = BR_STATE_REGISTER;
	BattleCore_SyncState();
}

void BattleRoyale::BattleCore_OpenEntry()
{
	if (!IsBattleChannel())
		return;

	BR_LOG(0, "%s", __FUNCTION__);

	m_State = BR_STATE_ENTER;
	BattleCore_SyncState();
	PushNextUpdate(BR_TIME_TO_CLOSE_ENTRY);
}

void BattleRoyale::BattleCore_CloseEntry()
{
	if (!IsBattleChannel())
		return;

	BR_LOG(0, "%s", __FUNCTION__);

	m_State = BR_STATE_ENTER_CLOSED;
	BattleCore_SyncState();
	
	NotifyFighters(BATTLE_ROYALE_GC_ENTRY_CLOSED);

	char notice[256]{};
	snprintf(notice, sizeof(notice),
		LC_TEXT("[Battle Royale] The countdown has begun. The battle royale starts in %d seconds."), BR_TIME_TO_BATTLE);
	SendNoticeMap(notice, BR_MAP_INDEX, true);

	// Remove the fighters that registered and didn't enter
	// Remove the fighters that registered and didn't enter
	for (auto it = m_Fighters.begin(); it != m_Fighters.end();)
	{
		LPCHARACTER ch = CHARACTER_MANAGER::Instance().FindByPID(*it);
		if (ch && ch->IsBattleRoyale())
			it++;
		else
			it = m_Fighters.erase(it);
	}

	PushNextUpdate(BR_TIME_TO_BATTLE);
}

void BattleRoyale::BattleCore_OpenBattle()
{
	if (!IsBattleChannel())
		return;

	BR_LOG(0, "%s", __FUNCTION__);

	m_State = BR_STATE_BATTLE;
	BattleCore_SyncState();

	//NotifyFighters(BATTLE_ROYALE_GC_EXIT);
	NotifyFighters(BATTLE_ROYALE_GC_START);
	ForEachFighter([&](LPCHARACTER fighter) { fighter->AddAffect(AFFECT_BATTLE_ROYALE_MOVE_SPEED, POINT_MOV_SPEED, BR_EXTRA_MOV_SPEED, AFF_NONE, BR_EXTRA_MOV_SPEED_DURATION, 0, true); });
	BattleCore_SendCirclePacket(true);
	BattleCore_SendCirclePacket();
	BattleCore_SpawnChests();
	BattleCore_SpreadPlayers();
	BattleCore_SendOpenKillInfoPacket(true);
	BattleCore_SendSurvivorPacket();

	// Pick a random spawn time based on the shrink count
	m_SpecialChestSpawnTime = number(2, 5);
	BR_LOG(0, "%s special chest scheduled for %d", __FUNCTION__, m_SpecialChestSpawnTime);
}

struct FnBRCleanup
{
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->GetDesc())
			{
				// Doesn't matter where you warp them(it won't take them there), they'll all be sent home by PlayerLoad
				// I just need them to trigger the logout
				//ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()), EMPIRE_START_MAP(ch->GetEmpire()));
				ch->GoHome();
			}
			else if (ch->IsStone()) // We only spanwed stones so.. also easier to deal with than having to consider pets and shit
			{
				M2_DESTROY_CHARACTER(ch);
			}
		}
		else if (ent->IsType(ENTITY_ITEM)) // Wipe all items
		{
			LPITEM item = (LPITEM)ent;
			M2_DESTROY_ITEM(item);
		}
	}
};

void BattleRoyale::BattleCore_Close()
{
	BR_LOG(0, "%s Cleanup start", __FUNCTION__);
	FnBRCleanup cleanup{};
	SECTREE_MANAGER::Instance().for_each(BR_MAP_INDEX, cleanup);
	BR_LOG(0, "%s Cleanup finished", __FUNCTION__);

	if (m_State == BR_STATE_CLOSED)
		sys_err("[BATTLE_ROYALE] Already closed.");

	Initialize();
	m_FieldState = BR_FIELD_STATE_CLOSED; // Block picking the winner
	BattleCore_SyncState();
}

bool BattleRoyale::SetBattleRoyaleTableFromPlayerTable(LPCHARACTER ch, const TPlayerTable* t)
{
	if (!t || !IsBattleChannel())
		return false;

	if (!ch)
		return false;

	BR_LOG(0, "%s %u", __FUNCTION__, t->id);
	ch->SetSkipSave(true);

	constexpr uint8_t races[2] = { MAIN_RACE_ASSASSIN_W, MAIN_RACE_ASSASSIN_M };
	constexpr uint8_t defaultJob = JOB_ASSASSIN;
	TPlayerTable brTable{};

	brTable.id = t->id;
	brTable.level = 1;
	brTable.job = races[uint8_t(number(0, 100) > 50)];

	brTable.st = JobInitialPoints[defaultJob].st;
	brTable.dx = JobInitialPoints[defaultJob].dx;
	brTable.ht = JobInitialPoints[defaultJob].ht;
	brTable.iq = JobInitialPoints[defaultJob].iq;

	brTable.hp = JobInitialPoints[defaultJob].max_hp + brTable.ht * JobInitialPoints[defaultJob].hp_per_ht;
	brTable.sp = JobInitialPoints[defaultJob].max_sp + brTable.iq * JobInitialPoints[defaultJob].sp_per_iq;
	brTable.stamina = JobInitialPoints[defaultJob].max_stamina;

	brTable.x = t->x;
	brTable.y = t->y;
	brTable.z = t->z;
	brTable.lMapIndex = t->lMapIndex;

	strlcpy(brTable.name, t->name, sizeof(brTable.name));
	ch->SetPlayerProto(&brTable);

	return true;
}

void BattleRoyale::BattleCore_PushFieldState(uint8_t state, int secondsTillChange)
{
	m_FieldState = state;
	if (-1 != secondsTillChange)
		PushNextUpdate(secondsTillChange);
}

// Packets, creation, shrinkage
#pragma region FIELD_AND_SAFEZONE
void BattleRoyale::BattleCore_SendCirclePacket(bool isRed)
{
	BR_LOG(0, "%s isRed ? %u", __FUNCTION__, isRed);

	if (isRed)
	{
		CreateRedCircle();
	}
	else
	{
		CreateWhiteCircle();

		int timeToShrink = BR_TIME_TO_FIRST_SHRINK - m_ShrinkCount * BR_TIME_DECREASE_TO_NEXT_SHRINK;
		BattleCore_PushFieldState(BR_FIELD_STATE_SHRINK, timeToShrink);

		char notice[256]{};
		snprintf(notice, sizeof(notice), LC_TEXT("Combat zone has been updated. It will close in %d seconds."), timeToShrink);
		SendNoticeMap(notice, BR_MAP_INDEX, true);
	}

	auto& pos = isRed ? m_RedCirclePos : m_WhiteCirclePos;

	int x = (int)pos.x;
	int y = (int)pos.y;
	int radius = (int)pos.w;

	TPacketGCBattleRoyale p;
	p.header = HEADER_GC_BATTLE_ROYALE;
	p.subheader = isRed ? BATTLE_ROYALE_GC_RED_CIRCLE : BATTLE_ROYALE_GC_WHITE_CIRCLE;
	p.size = sizeof(p) + sizeof(x) + sizeof(y) + sizeof(radius);

	TEMP_BUFFER buf;
	buf.write(&p, sizeof(p));
	buf.write(&x, sizeof(x));
	buf.write(&y, sizeof(y));
	buf.write(&radius, sizeof(radius));
	ForEachPlayerInside([&](LPCHARACTER ch) { ch->GetDesc()->Packet(buf.read_peek(), buf.size()); });
}

// Red is the one that determines where the white circle can go.
void BattleRoyale::CreateRedCircle()
{
	if (m_RedCirclePos.w > 0)
		return;

	auto sectree = SECTREE_MANAGER::Instance().GetMap(BR_MAP_INDEX);
	if (!sectree)
		return;

	long mapWidth = sectree->m_setting.iWidth / 100;
	long mapHeight = sectree->m_setting.iHeight / 100;
	int redX = mapWidth / 2;
	int redY = mapHeight / 2;
	double redRadius = sqrt(redX * redX + redY * redY); // Set the radius as the diagonal of the map(to encompass all of it)

	m_RedCirclePos = D3DXVECTOR4{ static_cast<float>(redX), static_cast<float>(redY), 0.0f, static_cast<float>(redRadius) };
	BR_LOG(0, "%s %d %d %d", __FUNCTION__, redX, redY, (int)redRadius);
}

void BattleRoyale::CreateWhiteCircle()
{
	auto sectree = SECTREE_MANAGER::Instance().GetMap(BR_MAP_INDEX);
	if (!sectree)
		return;

	long width = sectree->m_setting.iWidth / 100;
	long height = sectree->m_setting.iHeight / 100;
	int radius = (int)m_RedCirclePos.w / 2;

	// Take 5% off the top because the line is thick af on client side and it may look like it's outside of the red circle.
	int maxSize = (int)m_RedCirclePos.w * 95 / 100 - radius;
	int whiteX = number((int)m_RedCirclePos.x - maxSize, (int)m_RedCirclePos.x + maxSize);
	int whiteY = number((int)m_RedCirclePos.y - maxSize, (int)m_RedCirclePos.y + maxSize);
	whiteX = MINMAX(radius, whiteX, width - radius);
	whiteY = MINMAX(radius, whiteY, height - radius);

	if (m_RedCirclePos.w <= 32)
		whiteX = whiteY = radius = 0;

	m_WhiteCirclePos = D3DXVECTOR4{ static_cast<float>(whiteX), static_cast<float>(whiteY), 0.0f, static_cast<float>(radius) };

	BR_LOG(0, "%s %d %d %d", __FUNCTION__, whiteX, whiteY, radius);
}

void BattleRoyale::BattleCore_StartShrinking()
{
	if (m_RedCirclePos.w == 0)  // we're done
	{
		BR_LOG(0, "Can't shrink anymore. White circle radius %d, red circle radius %d", (int)m_WhiteCirclePos.w, (int)m_RedCirclePos.w);
		BattleCore_PushFieldState(BR_FIELD_STATE_NONE);
		return;
	}

	BR_LOG(0, "%s", __FUNCTION__);

	m_ShrinkDuration = BR_INITIAL_SHRINK_DURATION - 9 * m_ShrinkCount;
	m_ShrinkDuration = std::max(m_ShrinkDuration, 5);//Only for windows, so that the counter doesn't go negative on test
	m_ShrinkCount++;

	TPacketGCBattleRoyale p;
	p.header = HEADER_GC_BATTLE_ROYALE;
	p.subheader = BATTLE_ROYALE_GC_SHRINK;
	p.size = sizeof(p) + sizeof(m_ShrinkDuration);

	TEMP_BUFFER buf;
	buf.write(&p, sizeof(p));
	buf.write(&m_ShrinkDuration, sizeof(m_ShrinkDuration));
	ForEachPlayerInside([&](LPCHARACTER ch) { ch->GetDesc()->Packet(buf.read_peek(), buf.size()); });

	m_StartShrinkTime = thecore_pulse() + PASSES_PER_SEC(1); // Let the client be 1s ahead
	m_RedCirclePosCopy = m_RedCirclePos;
	BattleCore_PushFieldState(BR_FIELD_STATE_SHRINKING, m_ShrinkDuration);

	char notice[256]{};
	snprintf(notice, sizeof(notice), LC_TEXT("The combat zone will close in %d seconds."), m_ShrinkDuration);
	SendNoticeMap(notice, BR_MAP_INDEX, true);
}

void BattleRoyale::BattleCore_Shrink()
{
	auto& from = m_RedCirclePosCopy;
	auto& to = m_WhiteCirclePos;

	float elapsed = static_cast<float>(thecore_pulse() - m_StartShrinkTime);
	float d = elapsed / (float)PASSES_PER_SEC(m_ShrinkDuration);

	float currentX = from.x - (from.x - to.x) * d;
	float currentY = from.y - (from.y - to.y) * d;
	float currentRadius = from.w - (from.w - to.w) * d;

	m_RedCirclePos = { currentX, currentY, 0.0f, currentRadius };
}

void BattleRoyale::BattleCore_StopShrinking()
{
	BR_LOG(0, "%s", __FUNCTION__);

	m_RedCirclePos = m_WhiteCirclePos;

	TPacketGCBattleRoyale p;
	p.header = HEADER_GC_BATTLE_ROYALE;
	p.subheader = BATTLE_ROYALE_GC_SHRINK_DONE;
	p.size = sizeof(p);
	ForEachPlayerInside([&](LPCHARACTER ch) { ch->GetDesc()->Packet(&p, sizeof(p)); });

	if (m_RedCirclePos.w == 0) // we're done, now we wait for the players to die
		return;

	int timeTillNextSafezone = 10;
	BattleCore_PushFieldState(BR_FIELD_STATE_WHITE_CIRCLE, timeTillNextSafezone);

	char notice[256]{};
	snprintf(notice, sizeof(notice),
		LC_TEXT("The magnetic field has stopped. The next combat zone will appear in %d seconds."), timeTillNextSafezone);
	SendNoticeMap(notice, BR_MAP_INDEX, true);
}

void BattleRoyale::BattleCore_ProcessFieldDamage()
{
	if (m_Fighters.size() <= 1)
		return;

	auto sectree = SECTREE_MANAGER::Instance().GetMap(BR_MAP_INDEX);
	if (!sectree)
		return;

	//BR_LOG(0, "%s", __FUNCTION__);

	long basex = sectree->m_setting.iBaseX;
	long basey = sectree->m_setting.iBaseY;

	auto& chrMgr = CHARACTER_MANAGER::Instance();
	for (auto it = m_Fighters.begin(); it != m_Fighters.end();)
	{
		if (LPCHARACTER fighter = chrMgr.FindByPID(*(it++)))
		{
			if (fighter->IsDead())
				continue;

			long localx = (fighter->GetX() - basex) / 100;
			long localy = (fighter->GetY() - basey) / 100;

			auto aff = fighter->FindAffect(AFFECT_BATTLE_ROYALE_FIELD_DAMAGE);
			if (m_Fighters.size() > 1 && !isPointInsideCircle((float)localx, (float)localy, m_RedCirclePos.w, m_RedCirclePos.x, m_RedCirclePos.y))
			{
				fighter->PointChange(POINT_HP, -BR_DAMAGE_OUTSIDE_OF_FIELD, false);

				TPacketGCDamageInfo damageInfo{};
				damageInfo.header = HEADER_GC_DAMAGE_INFO;
				damageInfo.dwVictimVID = (DWORD)fighter->GetVID();
				damageInfo.flag = DAMAGE_TYPE_NORMAL;
				damageInfo.damage = BR_DAMAGE_OUTSIDE_OF_FIELD;
				fighter->GetDesc()->Packet(&damageInfo, sizeof(TPacketGCDamageInfo));

				// Killed by electrical field
				if (fighter->GetHP() <= 0)
					fighter->Dead(0, true);
				else if (!aff)
					fighter->AddAffect(AFFECT_BATTLE_ROYALE_FIELD_DAMAGE, 0, 0, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false);
			}
			else if (aff)
			{
				fighter->RemoveAffect(aff);
			}
		}
	}
}
#pragma endregion FIELD_AND_SAFEZONE

// Send messages to the players about the current state of the battle royale.
void BattleRoyale::OnLogin(LPCHARACTER ch)
{
	if (!ch)
		return;

	NotifyClient(ch, BATTLE_ROYALE_GC_EXIT); // Force stop if it's still marked as "running" on client side
	if (ch->IsBattleRoyale())
	{
		uint32_t playerID = ch->GetPlayerID();
		BR_LOG(0, "%s %u", __FUNCTION__, playerID);

		if (!IsBattleChannel() || !IsFighter(playerID))
		{
			ch->GoHome();
			return;
		}

		ch->SetPKMode(PK_MODE_FREE);
		BattleCore_SendClientTimer(ch);

		if (std::find(m_PlayersInside.begin(), m_PlayersInside.end(), playerID) == m_PlayersInside.end())
			m_PlayersInside.emplace_back(playerID);

		// Show the number of players needed for the battle to start.
		if (m_State == BR_STATE_ENTER)
		{
			//NotifyFighters(BATTLE_ROYALE_GC_START, true); // Show the "Leave" button

			if (m_PlayersInside.size() < BR_MIN_PLAYER_COUNT)
			{
				ForEachPlayerInside([&](LPCHARACTER ch) { ch->ChatPacket(CHAT_TYPE_INFO,
					LC_TEXT("Battle Royale participants status: (%u/%u)"), m_PlayersInside.size(), BR_MIN_PLAYER_COUNT); });
			}
		}
	}
	else if (HasRewards(ch))
	{
		NotifyClient(ch, BATTLE_ROYALE_GC_HAS_REWARD);
	}
	else if (m_State == BR_STATE_ENTER)
	{
		NotifyClient(ch, BATTLE_ROYALE_GC_STARTABLE);
	}
	else
	{
		if (m_State == BR_STATE_REGISTER && !IsFighter(ch->GetPlayerID()))
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Battle Royale] The registrations are now opened! Use F11 Key to Register / Enter"));
	}
}

// The tables will never be saved, you can't do anything with the real character here.
void BattleRoyale::OnLogout(LPCHARACTER ch)
{
	if (!ch || !ch->IsBattleRoyale())
		return;

	uint32_t playerID = ch->GetPlayerID();
	auto search = std::find(m_PlayersInside.begin(), m_PlayersInside.end(), playerID);
	if (search != m_PlayersInside.end())
		m_PlayersInside.erase(search);

	BR_LOG(0, "%s %u", __FUNCTION__, playerID);
	OnDead(ch, nullptr, true);
}

// Attacking other players will trigger a debuff on attack(spd) and movement speed
// On wiki it says that it can stack for up to 5 times
bool BattleRoyale::OnAttack(LPCHARACTER attacker, LPCHARACTER victim) const
{
	if (!attacker || !victim || !attacker->IsBattleRoyale() || !victim->IsBattleRoyale()) // Make sure we're in BR
		return true;

	if (m_State != BR_STATE_BATTLE)
		return false;

	if (!attacker->IsPC()) // Mobs can't damage players in BR
		return false;

	if (victim->IsPC() && !victim->IsDead())
	{
		constexpr int affDuration = 5;
		int flag = 1;
		if (victim->IsAffectFlag(AFF_BATTLE_ROYALE_SLOW_1))
		{
			flag++;
			if (victim->IsAffectFlag(AFF_BATTLE_ROYALE_SLOW_2))
			{
				flag++;
				if (victim->IsAffectFlag(AFF_BATTLE_ROYALE_SLOW_3))
				{
					flag = 1; // We already have all of them, refresh the first
				}
			}
		}

		long val = BR_DEBUFF_VALUE * flag;
		victim->AddAffect(AFFECT_BATTLE_ROYALE_SLOW, POINT_MOV_SPEED, -val, AFF_BATTLE_ROYALE_SLOW_1 + flag - 1, affDuration, 0, false);
		victim->AddAffect(AFFECT_BATTLE_ROYALE_SLOW, POINT_ATT_SPEED, -val, AFF_BATTLE_ROYALE_SLOW_1 + flag - 1, affDuration, 0, false);
	}

	return true;
}

void BattleRoyale::OnDead(LPCHARACTER victim, LPCHARACTER killer, bool isLogout)
{
	if (!victim || !victim->IsBattleRoyale() || !IsBattleChannel() || (victim->IsPC() && !IsFighter(victim->GetPlayerID())))
		return;

	if (victim->IsPC())
	{
		BR_LOG(0, "%s pid %u isLogout %u", __FUNCTION__, victim->GetPlayerID(), isLogout);

		PIXEL_POSITION pixel = victim->GetXYZ();
		for (WORD i = 0; i < INVENTORY_AND_EQUIP_SLOT_MAX; i++)
		{
			LPITEM item = victim->GetInventoryItem(i);
			if (!item)
				continue;

			victim->SyncQuickslot(QUICKSLOT_TYPE_ITEM, i, WORD_MAX);
			item->RemoveFromCharacter();
			pixel.x += number(-7, 7) * 20;
			pixel.y += number(-7, 7) * 20;
			item->AddToGround(BR_MAP_INDEX, pixel);
			item->StartDestroyEvent();
		}

		if (killer && killer->IsPC())
			m_KillsByPlayerID[killer->GetPlayerID()]++;

		m_Fighters.erase(victim->GetPlayerID());
		victim->SetPolymorph(BR_FUNERARY_STONE_VNUM);
		BattleCore_SendSurvivorPacket();

		if (m_Fighters.size() <= 1)
			BattleCore_PickWinnerAndClose();
	}

	if (isLogout)
		return;

	char killerName[CHARACTER_NAME_MAX_LEN + 1]{};
	uint32_t killerPid = 0;

	if (killer && killer->IsPC())
	{
		killerPid = killer->GetPlayerID();
		strlcpy(killerName, killer->GetName(), sizeof(killerName));

		if (victim->IsPC())
		{
			// Killing other players gives you +1k HP
			if (killer->GetPoint(POINT_HP_RECOVERY) + killer->GetHP() < killer->GetMaxHP())
			{
				killer->PointChange(POINT_HP_RECOVERY, BR_HP_REGEN_WHEN_KILLING_PLAYER);
				killer->StartAffectEvent();
			}
		}
		// Killing stones and chests gives you +50 hp
		else if (killer->GetPoint(POINT_HP_RECOVERY) + killer->GetHP() < killer->GetMaxHP())
		{
			killer->PointChange(POINT_HP_RECOVERY, BR_HP_REGEN_WHEN_KILLING_STONE);
			killer->StartAffectEvent();
		}
	}
	else
	{
		auto mob = CMobManager::Instance().Get(BR_ELECTRICAL_FIELD_VNUM);
		if (!mob)
			return;

		killerPid = mob->m_table.dwVnum;
		strlcpy(killerName, mob->m_table.szLocaleName, sizeof(killerName));
	}

	if (!victim->IsPC())
		return;

	if (auto aff = victim->FindAffect(AFFECT_BATTLE_ROYALE_FIELD_DAMAGE))
		victim->RemoveAffect(aff);

	BR_LOG(0, "%s %s(%u) -> %s(%u)", __FUNCTION__, killerName, killerPid, victim->GetName(), victim->GetPlayerID());

	BattleCore_SendDeadPacket(victim, killerName, (uint8_t)m_Fighters.size() + 1);

	char notice[256]{};
	snprintf(notice, sizeof(notice), LC_TEXT("%s killed %s"), killerName, victim->GetName());
	SendNoticeMap(notice, BR_MAP_INDEX, true);
}

void BattleRoyale::BattleCore_SendDeadPacket(LPCHARACTER ch, const char* killerName, uint8_t rank)
{
	if (!IsBattleChannel() || !ch || !ch->GetDesc() || !ch->IsBattleRoyale())
		return;

	constexpr int charNameSize = CHARACTER_NAME_MAX_LEN + 1;

	TPacketGCBattleRoyale p{};
	p.header = HEADER_GC_BATTLE_ROYALE;
	p.subheader = BATTLE_ROYALE_GC_DEAD;
	p.size = sizeof(p) + sizeof(rank) + charNameSize;

	TEMP_BUFFER buf;
	buf.write(&p, sizeof(p));
	buf.write(&rank, sizeof(rank));
	buf.write(killerName, charNameSize);
	ch->GetDesc()->Packet(buf.read_peek(), buf.size());
}

void BattleRoyale::BattleCore_SendSurvivorPacket()
{
	if (!IsBattleChannel())
		return;

	auto& chrMgr = CHARACTER_MANAGER::Instance();
	uint32_t survivors = m_Fighters.size();
	uint32_t kills = 0;

	TPacketGCBattleRoyale p{};
	p.header = HEADER_GC_BATTLE_ROYALE;
	p.subheader = BATTLE_ROYALE_GC_KILL_INFO;
	p.size = sizeof(p) + sizeof(survivors) + sizeof(kills);

	for (const uint32_t playerID : m_PlayersInside)
	{
		LPCHARACTER ch = chrMgr.FindByPID(playerID);
		if (!ch)
			continue;

		kills = m_KillsByPlayerID[playerID];

		TEMP_BUFFER buf;
		buf.write(&p, sizeof(p));
		buf.write(&survivors, sizeof(survivors));
		buf.write(&kills, sizeof(kills));
		ch->GetDesc()->Packet(buf.read_peek(), buf.size());
	}
}

void BattleRoyale::BattleCore_SendOpenKillInfoPacket(bool open)
{
	if (!IsBattleChannel())
		return;

	auto& chrMgr = CHARACTER_MANAGER::Instance();
	TPacketGCBattleRoyale p{};
	p.header = HEADER_GC_BATTLE_ROYALE;
	p.subheader = BATTLE_ROYALE_GC_OPEN_KILL_INFO;
	p.size = sizeof(p) + sizeof(open);

	TEMP_BUFFER buf;
	buf.write(&p, sizeof(p));
	buf.write(&open, sizeof(open));
	ForEachPlayerInside([&](LPCHARACTER ch) { ch->GetDesc()->Packet(buf.read_peek(), buf.size()); });
}

void BattleRoyale::BattleCore_SendClientTimer(LPCHARACTER ch)
{
	if (!ch || !m_NextUpdate || !IsBattleChannel())
		return;

	int seconds = (m_NextUpdate - thecore_pulse()) / PASSES_PER_SEC(1);
	if (seconds >= 0)
		ch->ChatPacket(CHAT_TYPE_COMMAND, "SetMinimapTimer %d", seconds);
}

void BattleRoyale::BattleCore_SpawnChests()
{
	if (!IsBattleChannel())
		return;

	BR_LOG(0, "%s Start", __FUNCTION__);
	uint32_t t = get_dword_time();

	int countOfEach = 200;
	int spawned = 0;
	for (int i = 0; i < countOfEach; i++)
	{
		/*
			20710	Supply Chest (BR)
			20711	Equipment Chest(BR)
			20713	Equipment Chest(BR)
			20714	Special Supply Chest(BR)
		*/
		LPCHARACTER ch = CHARACTER_MANAGER::Instance().SpawnMobRandomPosition(20710, BR_MAP_INDEX);
		if (ch)
			spawned++;

		ch = CHARACTER_MANAGER::Instance().SpawnMobRandomPosition(20710, BR_MAP_INDEX);
		if (ch)
			spawned++;

		ch = CHARACTER_MANAGER::Instance().SpawnMobRandomPosition(20710, BR_MAP_INDEX);
		if (ch)
			spawned++;

		//ch = CHARACTER_MANAGER::Instance().SpawnMobRandomPosition(20714, BR_MAP_INDEX);
		//if (ch)
		//	spawned++;
	}

	uint32_t diff = get_dword_time() - t;
	BR_LOG(0, "%s Finished. Spawned %u mobs in %u ms", __FUNCTION__, spawned, diff);
}

void BattleRoyale::BattleCore_SpawnSpecialChest()
{
	if (!IsBattleChannel())
		return;

	BR_LOG(0, "%s shrink count %d", __FUNCTION__, m_ShrinkCount);
	auto& sectreeMgr = SECTREE_MANAGER::Instance();

	auto pkSectreeMap = sectreeMgr.GetMap(BR_MAP_INDEX);
	if (!pkSectreeMap)
		return;

	long basex = pkSectreeMap->m_setting.iBaseX + m_WhiteCirclePos.x * 100;
	long basey = pkSectreeMap->m_setting.iBaseY + m_WhiteCirclePos.y * 100;

	for (int i = 0; i < 2000; i++)
	{
		long x = number(-m_WhiteCirclePos.w, m_WhiteCirclePos.w) * 100 + basex;
		long y = number(-m_WhiteCirclePos.w, m_WhiteCirclePos.w) * 100 + basey;

		LPSECTREE tree = pkSectreeMap->Find(x, y);
		if (!tree)
			continue;

		DWORD dwAttr = tree->GetAttribute(x, y);
		if (IS_SET(dwAttr, ATTR_BLOCK | ATTR_OBJECT))
			continue;

		LPSECTREE sectree = sectreeMgr.Get(BR_MAP_INDEX, x, y);
		if (!sectree)
			continue;

		if (LPCHARACTER ch = CHARACTER_MANAGER::Instance().SpawnMob(20714, BR_MAP_INDEX, x, y, 0, true))
			SendNoticeMap(LC_TEXT("A special package of supplies has appeared somewhere."), BR_MAP_INDEX, true);

		return;
	}

	BR_LOG(0, "%s Failed to spawn special chest.", __FUNCTION__);
}

void BattleRoyale::BattleCore_SpreadPlayers()
{
	if (!IsBattleChannel())
		return;

	uint32_t tStart = get_dword_time();
	auto& chrMgr = CHARACTER_MANAGER::Instance();
	auto& sectreeMgr = SECTREE_MANAGER::Instance();

	auto pkSectreeMap = sectreeMgr.GetMap(BR_MAP_INDEX);
	if (!pkSectreeMap)
		return;

	long basex = pkSectreeMap->m_setting.iBaseX;
	long basey = pkSectreeMap->m_setting.iBaseY;

	// 10% away from min border
	long minX = (pkSectreeMap->m_setting.iWidth / 100) * 10 / 100;
	long minY = (pkSectreeMap->m_setting.iHeight / 100) * 10 / 100;

	// 10% away from max border
	long maxX = (pkSectreeMap->m_setting.iWidth / 100) * 90 / 100;
	long maxY = (pkSectreeMap->m_setting.iHeight / 100) * 90 / 100;

	for (const uint32_t fighterID : m_Fighters)
	{
		if (LPCHARACTER fighter = chrMgr.FindByPID(fighterID))
		{
			if (!fighter->IsBattleRoyale())
				continue;

			for (int i = 0; i < 2000; i++)
			{
				long x = number(minX, maxX) * 100 + basex;
				long y = number(minY, maxY) * 100 + basey;
				LPSECTREE tree = pkSectreeMap->Find(x, y);

				if (!tree)
					continue;

				uint32_t dwAttr = tree->GetAttribute(x, y);
				if (IS_SET(dwAttr, ATTR_BLOCK | ATTR_OBJECT))
					continue;

				if (LPSECTREE sectree = sectreeMgr.Get(BR_MAP_INDEX, x, y))
				{
					fighter->SetRotation((float)number(0, 360));
					fighter->Show(BR_MAP_INDEX, x, y, 0, true);
					fighter->Stop();
					//BR_LOG(0, "%s found position for player %u after %d trials", __FUNCTION__, playerID, i);
				}

				break;
			}
		}
	}

	uint32_t tEnd = get_dword_time() - tStart;
	BR_LOG(0, "%s spread %u players in %u ms", __FUNCTION__, m_Fighters.size(), tEnd);
}

void BattleRoyale::BattleCore_PickWinnerAndClose()
{
	if (m_FieldState == BR_FIELD_STATE_CLOSED || !IsBattleChannel())
		return;

	constexpr int secondsTillExit = 15;
	uint32_t playerId = 0;
	if (!m_Fighters.empty())
	{
		playerId = *m_Fighters.begin();

		TQuestTable tbl{};
		tbl.dwPID = playerId;
		tbl.lValue = 1;
		snprintf(tbl.szName, sizeof(tbl.szName), "%s", "battle_royale");
		snprintf(tbl.szState, sizeof(tbl.szState), "%s", "has_reward");
		db_clientdesc->DBPacket(HEADER_GD_QUEST_SAVE, 0, &tbl, sizeof(tbl));

		if (LPCHARACTER ch = CHARACTER_MANAGER::Instance().FindByPID(playerId))
		{
			ch->AddAffect(AFFECT_BATTLE_ROYALE_CROWN, 0, 0, AFF_BATTLE_ROYALE_CROWN, BR_CROWN_AFFECT_DURATION, 0, true);

			if (LPDESC d = ch->GetDesc())
			{
				TPacketGCBattleRoyale packet{};
				packet.header = HEADER_GC_BATTLE_ROYALE;
				packet.subheader = BATTLE_ROYALE_GC_WINNER;
				packet.size = sizeof(packet);

				d->Packet(&packet, sizeof(packet));
			}

			char notice[256]{};
			snprintf(notice, sizeof(notice),
				LC_TEXT("%s has won the battle royale! Everyone will be sent home in %d seconds!"), ch->GetName(), secondsTillExit);
			SendNoticeMap(notice, BR_MAP_INDEX, true);
		}
	}

	BR_LOG(0, "%s Applicants count %u. Winner %u", __FUNCTION__, m_Fighters.size(), playerId);
	BattleCore_PushFieldState(BR_FIELD_STATE_CLOSED, secondsTillExit);
}

bool BattleRoyale::HasRewards(LPCHARACTER ch) const
{
	return ch && ch->GetQuestFlag("battle_royale.has_reward") > 0;
}

void BattleRoyale::ClaimRewards(LPCHARACTER ch)
{
	if (ch)

	if (!ch || !ch->CanHandleItem() || ch->IsBattleRoyale() || !HasRewards(ch))
		return;

	BR_LOG(0, "%s %s", __FUNCTION__, ch->GetName());

	// Give rewards
	ch->AutoGiveItem(77916, 1);

	ch->SetQuestFlag("battle_royale.has_reward", 0);
	OnLogin(ch); // Change UI button state
}

void BattleRoyale::BattleCore_Leave(LPCHARACTER ch)
{
	if (!ch || !IsBattleChannel() || !ch->IsBattleRoyale())
		return;

	// Doesn't matter where you warp them(it won't take them there), they'll all be sent home by PlayerLoad
	// I just need them to trigger the logout
	//ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()), EMPIRE_START_MAP(ch->GetEmpire()));
	ch->GoHome();
}

void BattleRoyale::BattleCore_Update(int pulse)
{
	if (m_State == BR_STATE_CLOSED || !IsBattleChannel())
		return;

	if (m_State == BR_STATE_BATTLE)
	{
		if (pulse % PASSES_PER_SEC(1)) // Update each second
			return;

		if (m_FieldState == BR_FIELD_STATE_SHRINKING)
			BattleCore_Shrink();

		if (CanUpdate())
		{
			switch (m_FieldState)
			{
			case BR_FIELD_STATE_WHITE_CIRCLE:
				BattleCore_SendCirclePacket();
				if (m_SpecialChestSpawnTime == m_ShrinkCount)
					BattleCore_SpawnSpecialChest();
				break;
			case BR_FIELD_STATE_SHRINK:
				BattleCore_StartShrinking();
				break;
			case BR_FIELD_STATE_SHRINKING:
				BattleCore_StopShrinking();
				break;
			case BR_FIELD_STATE_CLOSED:
				BattleCore_Close();
				break;
			}
		}

		BattleCore_ProcessFieldDamage();
	}
	else if (CanUpdate())
	{
		if (!BattleCore_CanBeginBattle())
		{
			BR_LOG(0, "[BATTLE_ROYALE] Not enough fighters(%d) to start. Shutting down.", m_Fighters.size());
			BattleCore_Close();
		}
		else if (m_State == BR_STATE_ENTER)
			BattleCore_CloseEntry();
		else if (m_State == BR_STATE_ENTER_CLOSED)
			BattleCore_OpenBattle();
	}
}

#endif
