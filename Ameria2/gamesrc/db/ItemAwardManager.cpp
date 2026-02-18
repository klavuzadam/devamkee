#include "stdafx.h"
#include "QID.h"
#include "DBManager.h"
#include "ItemAwardManager.h"
#include "Peer.h"

#include "ClientManager.h"



DWORD g_dwLastCachedItemAwardID = 0;
ItemAwardManager::ItemAwardManager()
{
}

ItemAwardManager::~ItemAwardManager()
{
}

void ItemAwardManager::RequestLoad()
{
	char szQuery[QUERY_MAX_LEN];
	snprintf(szQuery, sizeof(szQuery), "SELECT id,login,vnum,count,socket0,socket1,socket2,mall,why FROM item_award WHERE taken_time IS NULL and id > %d", g_dwLastCachedItemAwardID);
	CDBManager::instance().ReturnQuery(szQuery, QID_ITEM_AWARD_LOAD, 0, NULL);
}

void ItemAwardManager::Load(SQLMsg * pMsg)
{
	MYSQL_RES * pRes = pMsg->Get()->pSQLResult;

	for (uint32_t i = 0; i < pMsg->Get()->uiNumRows; ++i)
	{
		MYSQL_ROW row = mysql_fetch_row(pRes);
		int col = 0;

		DWORD dwID = 0;
		str_to_number(dwID, row[col++]);

		if (m_map_award.find(dwID) != m_map_award.end())
			continue;

		TItemAward * kData = new TItemAward;
		memset(kData, 0, sizeof(TItemAward));

		kData->dwID	= dwID;
		trim_and_lower(row[col++], kData->szLogin, sizeof(kData->szLogin));
		str_to_number(kData->dwVnum, row[col++]);
		str_to_number(kData->dwCount, row[col++]);
		str_to_number(kData->dwSocket0, row[col++]);
		str_to_number(kData->dwSocket1, row[col++]);
		str_to_number(kData->dwSocket2, row[col++]);
		str_to_number(kData->bMall, row[col++]);

		if (row[col])
		{
			strlcpy(kData->szWhy, row[col], sizeof(kData->szWhy));
			//°ÔÀÓ Áß¿¡ whyÄÝ·ë¿¡ º¯µ¿ÀÌ »ý±â¸é
			char* whyStr = kData->szWhy;	//why ÄÝ·ë ÀÐ±â
			char cmdStr[100] = "";	//whyÄÝ·ë¿¡¼­ ÀÐÀº °ªÀ» ÀÓ½Ã ¹®ÀÚ¿­¿¡ º¹»çÇØµÒ
			strcpy(cmdStr,whyStr);	//¸í·É¾î ¾ò´Â °úÁ¤¿¡¼­ ÅäÅ«¾²¸é ¿øº»µµ ÅäÅ«È­ µÇ±â ¶§¹®
			char command[20] = "";
			// @fixme203 directly GetCommand instead of strcpy
			CClientManager::instance().GetCommand(cmdStr, command);				// command ¾ò±â
			//sys_err("%d,  %s",pItemAward->dwID,command);
			if( !(strcmp(command,"GIFT") ))	// command °¡ GIFTÀÌ¸é
			{
				TPacketItemAwardInfromer giftData;
				strcpy(giftData.login, kData->szLogin);	//·Î±×ÀÎ ¾ÆÀÌµð º¹»ç
				strcpy(giftData.command, command);					//¸í·É¾î º¹»ç
				giftData.vnum = kData->dwVnum;				//¾ÆÀÌÅÛ vnumµµ º¹»ç
				CClientManager::instance().ForwardPacket(HEADER_DG_ITEMAWARD_INFORMER,&giftData,sizeof(TPacketItemAwardInfromer));
			}
		}

		m_map_award.insert(std::make_pair(dwID, kData));

		printf("ITEM_AWARD load id %u bMall %d \n", kData->dwID, kData->bMall);
		sys_log(0, "ITEM_AWARD: load id %lu login %s vnum %lu count %u socket %lu", kData->dwID, kData->szLogin, kData->dwVnum, kData->dwCount, kData->dwSocket0);
		std::set<TItemAward *> & kSet = m_map_kSetAwardByLogin[kData->szLogin];
		kSet.insert(kData);

		if (dwID > g_dwLastCachedItemAwardID)
			g_dwLastCachedItemAwardID = dwID;
	}
}

std::set<TItemAward *> * ItemAwardManager::GetByLogin(const char * c_pszLogin)
{
	itertype(m_map_kSetAwardByLogin) it = m_map_kSetAwardByLogin.find(c_pszLogin);

	if (it == m_map_kSetAwardByLogin.end())
		return NULL;

	return &it->second;
}

void ItemAwardManager::Taken(DWORD dwAwardID, DWORD dwItemID)
{
	itertype(m_map_award) it = m_map_award.find(dwAwardID);

	if (it == m_map_award.end())
	{
		sys_log(0, "ITEM_AWARD: Taken ID not exist %lu", dwAwardID);
		return;
	}

	TItemAward * k = it->second;
	k->bTaken = true;

	//
	// Update taken_time in database to prevent not to give him again.
	//
	char szQuery[QUERY_MAX_LEN];

	snprintf(szQuery, sizeof(szQuery),
			"UPDATE item_award SET taken_time=NOW(),item_id=%u WHERE id=%u AND taken_time IS NULL",
			dwItemID, dwAwardID);

	CDBManager::instance().ReturnQuery(szQuery, QID_ITEM_AWARD_TAKEN, 0, NULL);
}

std::map<DWORD, TItemAward *>& ItemAwardManager::GetMapAward()
{
	return m_map_award;
}

std::map<std::string, std::set<TItemAward *> >& ItemAwardManager::GetMapkSetAwardByLogin()
{
	return m_map_kSetAwardByLogin;
}
