#include "stdafx.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "utils.h"
#include "config.h"
#include "constants.h"
#include "auto_event_manager.h"
#include "questmanager.h"
#include "cmd.h"
#include "desc.h"
#include "char.h"
#include "char_manager.h"
#include "locale_service.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "db.h"
#include "p2p.h"
#include <random>

static LPEVENT running_event = NULL;

EVENTINFO(EventsManagerInfoData)
{
	CEventsManager *pEvents;

	EventsManagerInfoData()
	: pEvents(0)
	{
	}
};

EVENTFUNC(automatic_event_timer)
{
	if (event == NULL)
		return 0;

	if (event->info == NULL)
		return 0;

	EventsManagerInfoData* info = dynamic_cast<EventsManagerInfoData*>(event->info);

	if (info == NULL)
		return 0;
	
	CEventsManager* pInstance = info->pEvents;

	if (pInstance == NULL)
		return 0;
	
	CEventsManager::instance().PrepareChecker();
	return PASSES_PER_SEC(1);
}

void CEventsManager::PrepareChecker()
{
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);

	int day = vKey.tm_wday;
	int hour = vKey.tm_hour;
	int minute = vKey.tm_min;
	int second = vKey.tm_sec;

	CEventsManager::instance().Check(day, hour, minute, second);
}

void CEventsManager::Check(int day, int hour, int minute, int second)
{
	if (g_bChannel == 99)
	{
		if (second == 1 && (minute == 59))
		{
			uint32_t count_limit = 700;
			uint32_t bannedCount = 0;
			
			std::unique_ptr<SQLMsg> pmsg(DBManager::Instance().DirectQuery("SELECT name, login, hwid FROM log.hacker_detections GROUP BY name HAVING COUNT(*) > %d LIMIT 30;", count_limit));
			SQLResult* resSelect = pmsg->Get();
			MYSQL_ROW row;

			std::map<std::string, std::pair<std::string, std::string>> bannedAccounts = {};

			while ((row = mysql_fetch_row(resSelect->pSQLResult)))
			{
				std::string name = row[0];
				std::string login = row[1];
				std::string hwid = row[2];
				bannedAccounts[name] = std::pair(login, hwid);
			}

			for (auto& it : bannedAccounts)
			{
				bannedCount++;
				
				char banAccount[512];
				
#ifdef __BANS_HWID_TOO__
				sprintf(banAccount, 
						"UPDATE account.account AS a \
						 JOIN ( \
							 SELECT hwid_routher FROM account.account WHERE login = '%s' \
						 ) AS b ON a.hwid_routher = b.hwid_routher \
						 SET a.status = 'BLOCK'",
						it.second.first.c_str());
#else

				sprintf(banAccount, 
						"UPDATE account.account \
						 SET status = 'BLOCK' \
						 WHERE login = '%s'",
						it.second.first.c_str());
#endif
				DBManager::Instance().Query(banAccount);

				
				char bannedShoutNew[256];
				sprintf(bannedShoutNew, "[HackShield]Banned a player that used cheats!");
				BroadcastNotice(bannedShoutNew, false);

#ifdef __BANS_HWID_TOO__
				char insertQuery[512];
				sprintf(insertQuery, 
						"INSERT INTO account.hwid_routher (hwid, reason, date) \
						 SELECT hwid_routher, 'AUTO_BANNED', NOW() \
						 FROM account.account \
						 WHERE LENGTH(hwid_routher) > 4 AND hwid_routher = ( \
							 SELECT hwid_routher FROM account.account WHERE login = '%s')",
						it.second.first.c_str());

				DBManager::Instance().Query(insertQuery);
#endif

				LPDESC desc = DESC_MANAGER::Instance().FindByCharacterName(it.first.c_str());

				if (desc)
					desc->SetPhase(PHASE_CLOSE);
				else
				{
					TPacketGGDisconnectName p;
					p.bHeader = HEADER_GG_DISCONNECT_NAME;
					snprintf(p.szName, sizeof(p.szName), "%s", it.first.c_str());
					P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGDisconnectName));
				}
				
				char banQuery[256];
				sprintf(banQuery, "INSERT INTO `log`.`hacker_bans` (`dateTime`, `playerName`, `accountName`, `playerHWID`, `detectionCount`) SELECT NOW(), '%s', '%s', '%s', COUNT(*) AS detectionCount FROM log.hacker_detections WHERE `name` = '%s';", it.first.c_str(), it.second.first.c_str(), it.second.second.c_str(), it.first.c_str());
				DBManager::Instance().Query(banQuery);

				char deleteQuery[256];
				sprintf(deleteQuery, "DELETE FROM `log`.`hacker_detections` WHERE name = '%s';", it.first.c_str());
				DBManager::Instance().Query(deleteQuery);
			}
			char bannedShout[256];
			sprintf(bannedShout, "[HackShield] %d players were banned, play fair and don't use cheats!", bannedCount);
			BroadcastNotice(bannedShout, false);

			bannedAccounts.clear();
		}
	}
}

bool CEventsManager::Initialize()
{
	if (running_event != NULL)
	{
		event_cancel(&running_event);
		running_event = NULL;
	}

	EventsManagerInfoData* info = AllocEventInfo<EventsManagerInfoData>();
	info->pEvents = this;

	running_event = event_create(automatic_event_timer, info, PASSES_PER_SEC(30));
	return true;
}

void CEventsManager::Destroy()
{
	if (running_event != NULL)
	{
		event_cancel(&running_event);
		running_event = NULL;
	}
}
