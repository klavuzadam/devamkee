
#include "stdafx.h"
#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "db.h"
#include "guild_manager.h"
#include "marriage.h"
#include "../common/CommonDefines.h"
#include "desc.h"
#include "RankPlayer.h"

/*
   Return Value
		0 : ¾Ë ¼ö ¾ø´Â ¿¡·¯ or Äõ¸® ¿¡·¯
		1 : µ¿ÀÏÇÑ Á¦±¹À¸·Î ¹Ù²Ù·Á°íÇÔ
		2 : ±æµå °¡ÀÔÇÑ Ä³¸¯ÅÍ°¡ ÀÖÀ½
		3 : °áÈ¥ÇÑ Ä³¸¯ÅÍ°¡ ÀÖÀ½

		999 : Á¦±¹ ÀÌµ¿ ¼º°ø
*/
int CHARACTER::ChangeEmpire(BYTE empire)
{
	if (GetEmpire() == empire)
		return 1;

	char szQuery[1024+1];
	DWORD dwAID;
	DWORD dwPID[4];
	memset(dwPID, 0, sizeof(dwPID));

	{
		// 1. ³» °èÁ¤ÀÇ ¸ðµç pid¸¦ ¾ò¾î ¿Â´Ù
#ifdef ENABLE_PLAYER_PER_ACCOUNT5
		snprintf(szQuery, sizeof(szQuery),
				"SELECT id, pid1, pid2, pid3, pid4, pid5 FROM player_index%s WHERE pid1=%u OR pid2=%u OR pid3=%u OR pid4=%u OR pid5=%u AND empire=%u",
				get_table_postfix(), GetPlayerID(), GetPlayerID(), GetPlayerID(), GetPlayerID(), GetPlayerID(), GetEmpire());
#else
		snprintf(szQuery, sizeof(szQuery),
				"SELECT id, pid1, pid2, pid3, pid4 FROM player_index%s WHERE pid1=%u OR pid2=%u OR pid3=%u OR pid4=%u AND empire=%u",
				get_table_postfix(), GetPlayerID(), GetPlayerID(), GetPlayerID(), GetPlayerID(), GetEmpire());
#endif
		std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery(szQuery));

		if (msg->Get()->uiNumRows == 0)
		{
			return 0;
		}

		MYSQL_ROW row = mysql_fetch_row(msg->Get()->pSQLResult);

		str_to_number(dwAID, row[0]);
		str_to_number(dwPID[0], row[1]);
		str_to_number(dwPID[1], row[2]);
		str_to_number(dwPID[2], row[3]);
		str_to_number(dwPID[3], row[4]);
	}

	const int loop = 4;

	{
		// 2. °¢ Ä³¸¯ÅÍÀÇ ±æµå Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
		//   ÇÑ Ä³¸¯ÅÍ¶óµµ ±æµå¿¡ °¡ÀÔ µÇ¾î ÀÖ´Ù¸é, Á¦±¹ ÀÌµ¿À» ÇÒ ¼ö ¾ø´Ù.
		DWORD dwGuildID[4];
		CGuild * pGuild[4];
		SQLMsg * pMsg = NULL;

		for (int i = 0; i < loop; ++i)
		{
			snprintf(szQuery, sizeof(szQuery), "SELECT guild_id FROM guild_member%s WHERE pid=%u", get_table_postfix(), dwPID[i]);

			pMsg = DBManager::instance().DirectQuery(szQuery);

			if (pMsg != NULL)
			{
				if (pMsg->Get()->uiNumRows > 0)
				{
					MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

					str_to_number(dwGuildID[i], row[0]);

					pGuild[i] = CGuildManager::instance().FindGuild(dwGuildID[i]);

					if (pGuild[i] != NULL)
					{
						M2_DELETE(pMsg);
						return 2;
					}
				}
				else
				{
					dwGuildID[i] = 0;
					pGuild[i] = NULL;
				}

				M2_DELETE(pMsg);
			}
		}
	}

	{
		// 3. °¢ Ä³¸¯ÅÍÀÇ °áÈ¥ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
		//   ÇÑ Ä³¸¯ÅÍ¶óµµ °áÈ¥ »óÅÂ¶ó¸é Á¦±¹ ÀÌµ¿À» ÇÒ ¼ö ¾ø´Ù.
		for (int i = 0; i < loop; ++i)
		{
			if (marriage::CManager::instance().IsEngagedOrMarried(dwPID[i]) == true)
				return 3;
		}
	}

	{
		// 4. dbÀÇ Á¦±¹ Á¤º¸¸¦ ¾÷µ¥ÀÌÆ® ÇÑ´Ù.
#ifdef ENABLE_PLAYER_PER_ACCOUNT5
		snprintf(szQuery, sizeof(szQuery), "UPDATE player_index%s SET empire=%u WHERE pid1=%u OR pid2=%u OR pid3=%u OR pid4=%u OR pid5=%u AND empire=%u",
				get_table_postfix(), empire, GetPlayerID(), GetPlayerID(), GetPlayerID(), GetPlayerID(), GetPlayerID(), GetEmpire());
#else
		snprintf(szQuery, sizeof(szQuery), "UPDATE player_index%s SET empire=%u WHERE pid1=%u OR pid2=%u OR pid3=%u OR pid4=%u AND empire=%u",
				get_table_postfix(), empire, GetPlayerID(), GetPlayerID(), GetPlayerID(), GetPlayerID(), GetEmpire());
#endif
		std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery(szQuery));

		if (msg->Get()->uiAffectedRows > 0)
		{
			SetChangeEmpireCount();
			RankPlayer::Instance().LoadInfoPlayer(GetPlayerID(), true);
			return 999;
		}
	}

	return 0;
}

int CHARACTER::GetChangeEmpireCount() const
{
	char szQuery[1024+1];
	DWORD dwAID = GetAID();

	if (dwAID == 0)
		return 0;

	snprintf(szQuery, sizeof(szQuery), "SELECT change_count FROM change_empire WHERE account_id = %u", dwAID);

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));

	if (pMsg->Get() && pMsg->Get()->uiNumRows > 0)
	{
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

		DWORD count = 0;
		str_to_number(count, row[0]);

		return count;
	}

	return 0;
}

void CHARACTER::SetChangeEmpireCount()
{
	char szQuery[1024+1];

	DWORD dwAID = GetAID();

	if (dwAID == 0) return;

	int count = GetChangeEmpireCount();

	if (count == 0)
	{
		count++;
		snprintf(szQuery, sizeof(szQuery), "INSERT INTO change_empire VALUES(%u, %d)", dwAID, count);
	}
	else
	{
		count++;
		snprintf(szQuery, sizeof(szQuery), "UPDATE change_empire SET change_count=%d WHERE account_id=%u", count, dwAID);
	}

	std::unique_ptr<SQLMsg> pmsg(DBManager::instance().DirectQuery(szQuery));
}

DWORD CHARACTER::GetAID() const
{
	return GetDesc() ? GetDesc()->GetAccountTable().id : 0;
}

