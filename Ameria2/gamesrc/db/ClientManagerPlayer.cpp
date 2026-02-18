
#include "stdafx.h"

#include "ClientManager.h"

#include "Main.h"
#include "QID.h"
#include "ItemAwardManager.h"
#include "HB.h"
#include "Cache.h"
#ifdef __FAKE_PLAYER__
	#include "FakePlayerManager.h"
#endif
extern bool g_bHotBackup;

extern std::string g_stLocale;
extern int g_test_server;
extern int g_log;

//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!! IMPORTANT !!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// Check all SELECT syntax on item table before change this function!!!
//
bool CreateItemTableFromRes(MYSQL_RES * res, std::vector<TPlayerItem> * pVec, DWORD dwPID)
{
	if (!res)
	{
		pVec->clear();
		return true;
	}

	int rows;

	if ((rows = mysql_num_rows(res)) <= 0)	// µ¥ÀÌÅÍ ¾øÀ½
	{
		pVec->clear();
		return true;
	}

	pVec->resize(rows);

	for (int i = 0; i < rows; ++i)
	{
		MYSQL_ROW row = mysql_fetch_row(res);
		TPlayerItem & item = pVec->at(i);

		int cur = 0;

		// Check all SELECT syntax on item table before change this function!!!
		// Check all SELECT syntax on item table before change this function!!!
		// Check all SELECT syntax on item table before change this function!!!
		str_to_number(item.id, row[cur++]);
		str_to_number(item.window, row[cur++]);
		str_to_number(item.pos, row[cur++]);
		str_to_number(item.count, row[cur++]);
		str_to_number(item.vnum, row[cur++]);
		for(BYTE j=0;j<ITEM_SOCKET_MAX_NUM;++j)
			str_to_number(item.alSockets[j], row[cur++]);

		for (int j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; j++)
		{
			str_to_number(item.aAttr[j].bType, row[cur++]);
			str_to_number(item.aAttr[j].sValue, row[cur++]);
		}
#ifdef ENABLE_YOHARA_SYSTEM
		for (int j = 0; j < APPLY_RANDOM_SLOT_MAX_NUM; j++)
		{
			str_to_number(item.aApplyRandom[j].bType, row[cur++]);
			str_to_number(item.aApplyRandom[j].sValue, row[cur++]);
			str_to_number(item.aApplyRandom[j].row, row[cur++]);
		}
#endif
#ifdef ENABLE_NEW_NAME_ITEM
		strlcpy(item.name, row[cur++], sizeof(item.name));
#endif

		item.owner		= dwPID;
	}

	return true;
}

size_t CreatePlayerSaveQuery(char * pszQuery, size_t querySize, TPlayerTable * pkTab)
{
	size_t queryLen;

	queryLen = snprintf(pszQuery, querySize,
			"UPDATE player%s SET "
			"job = %d, "
			"voice = %d, "
			"dir = %d, "
			"x = %d, "
			"y = %d, "
			"z = %d, "
			"map_index = %d, "
			"exit_x = %ld, "
			"exit_y = %ld, "
			"exit_map_index = %ld, "
			"hp = %d, "
			"mp = %d, "
			"stamina = %d, "
			"random_hp = %d, "
			"random_sp = %d, "
			"playtime = %d, "
			"level = %d, "
			"level_step = %d, "
			"st = %d, "
			"ht = %d, "
			"dx = %d, "
			"iq = %d, "
			"gold = %lld, "
#ifdef USE_CHEQUE_CURRENCY
			"cheque = %u, "
#endif
			"exp = %u, "
			"stat_point = %d, "
			"skill_point = %d, "
			"sub_skill_point = %d, "
			"stat_reset_count = %d, "
			"ip = '%s', "
			"part_main = %d, "
			"part_hair = %d, "
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
			"pick_up_mode = %d, "
			"rarity_mode = %d, "
#endif

			#ifdef __SASH_SYSTEM__
			"part_sash = %d, "
			#endif
			"last_play = NOW(), "
			"skill_group = %d, "
			"alignment = %ld, "
#ifdef ENABLE_YOHARA_SYSTEM
			"conquerorlevel = %d, "
			"conqueror_level_step = %d, "
			"sungma_str = %d, "
			"sungma_hp = %d, "
			"sungma_move = %d, "
			"sungma_inmune = %d, "
			"conqueror_exp = %u, "
			"conqueror_point = %d, "
#endif

			"horse_level = %d, "
			"horse_riding = %d, "
			"horse_hp = %d, "
			"horse_hp_droptime = %u, "
			"horse_stamina = %d, "
			"horse_skill_point = %d, "
#ifdef UNLOCK_INVENTORY_WORLDARD
			"inventory_0 = %d, "
			"inventory_1 = %d, "
			"inventory_2 = %d, "
			"inventory_3 = %d, "
			"inventory_4 = %d, "
			"inventory_5 = %d, "
			"inventory_6 = %d, "
#endif
#ifdef BATTLEPASS_WORLDARD
			"battle_pass_end = FROM_UNIXTIME(%d), "
#endif
#ifdef ENABLE_ANTI_EXP
			"anti_exp = %d, "
#endif
#ifdef __GEM_SYSTEM__
			"gem = %d, "
#endif
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
			"auto_hunt_day = %d,"
			"auto_hunt_time = %d,"
#endif
#ifdef ENABLE_RANKING
			"r0 = %lld, "
			"r1 = %lld, "
			"r2 = %lld, "
			"r3 = %lld, "
			"r4 = %lld, "
			"r5 = %lld, "
			"r6 = %lld, "
			"r7 = %lld, "
			"r8 = %lld, "
			"r9 = %lld, "
			"r10 = %lld, "
			"r11 = %lld, "
			"r12 = %lld, "
			"r13 = %lld, "
			"r14 = %lld, "
			"r15 = %lld, "
			"r16 = %lld, "
			"r17 = %lld, "
			"r18 = %lld, "
			"r19 = %lld, "
#endif
			,
		GetTablePostfix(),
		pkTab->job,
		pkTab->voice,
		pkTab->dir,
		pkTab->x,
		pkTab->y,
		pkTab->z,
		pkTab->lMapIndex,
		pkTab->lExitX,
		pkTab->lExitY,
		pkTab->lExitMapIndex,
		pkTab->hp,
		pkTab->sp,
		pkTab->stamina,
		pkTab->sRandomHP,
		pkTab->sRandomSP,
		pkTab->playtime,
		pkTab->level,
		pkTab->level_step,
		pkTab->st,
		pkTab->ht,
		pkTab->dx,
		pkTab->iq,
		pkTab->gold,
#ifdef USE_CHEQUE_CURRENCY
		pkTab->cheque,
#endif
		pkTab->exp,
		pkTab->stat_point,
		pkTab->skill_point,
		pkTab->sub_skill_point,
		pkTab->stat_reset_count,
		pkTab->ip,
		pkTab->parts[PART_MAIN],
		pkTab->parts[PART_HAIR],
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
		pkTab->dwPickUPMode,
		pkTab->dwRarityMode,
#endif
		#ifdef __SASH_SYSTEM__
		pkTab->parts[PART_SASH],
		#endif
		pkTab->skill_group,
		pkTab->lAlignment,
#ifdef ENABLE_YOHARA_SYSTEM
		pkTab->conquerorlevel,
		pkTab->conqueror_level_step,
		pkTab->sungma_str,
		pkTab->sungma_hp,
		pkTab->sungma_move,
		pkTab->sungma_inmune,
		pkTab->conqueror_exp,
		pkTab->conqueror_point,
#endif
		pkTab->horse.bLevel,
		pkTab->horse.bRiding,
		pkTab->horse.sHealth,
		pkTab->horse.dwHorseHealthDropTime,
		pkTab->horse.sStamina,
		pkTab->horse_skill_point
#ifdef UNLOCK_INVENTORY_WORLDARD
		,pkTab->inventory_unlock[0]
		,pkTab->inventory_unlock[1]
		,pkTab->inventory_unlock[2]
		,pkTab->inventory_unlock[3]
		,pkTab->inventory_unlock[4]
		,pkTab->inventory_unlock[5]
		,pkTab->inventory_unlock[6]
#endif
#ifdef BATTLEPASS_WORLDARD
		,pkTab->dwBattlePassEndTime
#endif
#ifdef ENABLE_ANTI_EXP
		,pkTab->anti_exp
#endif

#ifdef __GEM_SYSTEM__
		,pkTab->gem
#endif
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
		, pkTab->auto_hunt_last_day
		, pkTab->auto_hunt_used_time
#endif
#ifdef ENABLE_RANKING
		, pkTab->lRankPoints[0],
		pkTab->lRankPoints[1],
		pkTab->lRankPoints[2],
		pkTab->lRankPoints[3],
		pkTab->lRankPoints[4],
		pkTab->lRankPoints[5],
		pkTab->lRankPoints[6],
		pkTab->lRankPoints[7],
		pkTab->lRankPoints[8],
		pkTab->lRankPoints[9],
		pkTab->lRankPoints[10],
		pkTab->lRankPoints[11],
		pkTab->lRankPoints[12],
		pkTab->lRankPoints[13],
		pkTab->lRankPoints[14],
		pkTab->lRankPoints[15],
		pkTab->lRankPoints[16],
		pkTab->lRankPoints[17],
		pkTab->lRankPoints[18],
		pkTab->lRankPoints[19]
#endif
		);

	//sys_err("save %d",pkTab->anti_exp);
	// Binary ·Î ¹Ù²Ù±â À§ÇÑ ÀÓ½Ã °ø°£
	static char text[8192 + 1];

	CDBManager::instance().EscapeString(text, pkTab->skills, sizeof(pkTab->skills));
	queryLen += snprintf(pszQuery + queryLen, querySize - queryLen, "skill_level = '%s', ", text);

	CDBManager::instance().EscapeString(text, pkTab->quickslot, sizeof(pkTab->quickslot));
	queryLen += snprintf(pszQuery + queryLen, querySize - queryLen, "quickslot = '%s' ", text);
#ifdef ENABLE_BATTLE_PASS
	CDBManager::instance().EscapeString(text, pkTab->battlePassData, sizeof(pkTab->battlePassData));
	queryLen += snprintf(pszQuery + queryLen, querySize - queryLen, ",battlepass = '%s' ", text);
#endif
#ifdef ENABLE_NEW_DETAILS_GUI
	CDBManager::instance().EscapeString(text, pkTab->kill_log, sizeof(pkTab->kill_log));
	queryLen += snprintf(pszQuery + queryLen, querySize - queryLen, ", kill_log = '%s' ", text);
#endif

	queryLen += snprintf(pszQuery + queryLen, querySize - queryLen, " WHERE id=%d", pkTab->id);
	return queryLen;
}

CPlayerTableCache * CClientManager::GetPlayerCache(DWORD id)
{
	TPlayerTableCacheMap::iterator it = m_map_playerCache.find(id);

	if (it == m_map_playerCache.end())
		return NULL;

	TPlayerTable* pTable = it->second->Get(false);
	pTable->logoff_interval = GetCurrentTime() - it->second->GetLastUpdateTime();
	return it->second;
}

void CClientManager::PutPlayerCache(TPlayerTable * pNew)
{
	CPlayerTableCache * c;

	c = GetPlayerCache(pNew->id);

	if (!c)
	{
		c = new CPlayerTableCache;
		m_map_playerCache.insert(TPlayerTableCacheMap::value_type(pNew->id, c));
	}

	if (g_bHotBackup)
		PlayerHB::instance().Put(pNew->id);

	c->Put(pNew);
}

/*
 * PLAYER LOAD
 */
void CClientManager::QUERY_PLAYER_LOAD(CPeer * peer, DWORD dwHandle, TPlayerLoadPacket * packet)
{
	CPlayerTableCache * c;
	TPlayerTable * pTab;

	//
	// ÇÑ °èÁ¤¿¡ ¼ÓÇÑ ¸ðµç Ä³¸¯ÅÍµé Ä³½¬Ã³¸®
	//
	CLoginData * pLoginData = GetLoginDataByAID(packet->account_id);

	if (pLoginData)
	{
		for (int n = 0; n < PLAYER_PER_ACCOUNT; ++n)
			if (pLoginData->GetAccountRef().players[n].dwID != 0)
				DeleteLogoutPlayer(pLoginData->GetAccountRef().players[n].dwID);
	}

	//----------------------------------------------------------------
	// 1. À¯ÀúÁ¤º¸°¡ DBCache ¿¡ Á¸Àç : DBCache¿¡¼­
	// 2. À¯ÀúÁ¤º¸°¡ DBCache ¿¡ ¾øÀ½ : DB¿¡¼­
	// ---------------------------------------------------------------

	//----------------------------------
	// 1. À¯ÀúÁ¤º¸°¡ DBCache ¿¡ Á¸Àç : DBCache¿¡¼­
	//----------------------------------
	if ((c = GetPlayerCache(packet->player_id)))
	{
		CLoginData * pkLD = GetLoginDataByAID(packet->account_id);

		if (!pkLD || pkLD->IsPlay())
		{
			sys_log(0, "PLAYER_LOAD_ERROR: LoginData %p IsPlay %d", pkLD, pkLD ? pkLD->IsPlay() : 0);
			peer->EncodeHeader(HEADER_DG_PLAYER_LOAD_FAILED, dwHandle, 0);
			return;
		}

		pTab = c->Get();

		pkLD->SetPlay(true);
		SendLoginToBilling(pkLD, true);
		std::memcpy(pTab->aiPremiumTimes, pkLD->GetPremiumPtr(), sizeof(pTab->aiPremiumTimes));

		peer->EncodeHeader(HEADER_DG_PLAYER_LOAD_SUCCESS, dwHandle, sizeof(TPlayerTable));
		peer->Encode(pTab, sizeof(TPlayerTable));

		if (packet->player_id != pkLD->GetLastPlayerID())
		{
			TPacketNeedLoginLogInfo logInfo;
			logInfo.dwPlayerID = packet->player_id;

			pkLD->SetLastPlayerID( packet->player_id );

			peer->EncodeHeader( HEADER_DG_NEED_LOGIN_LOG, dwHandle, sizeof(TPacketNeedLoginLogInfo) );
			peer->Encode( &logInfo, sizeof(TPacketNeedLoginLogInfo) );
		}

		char szQuery[QUERY_MAX_LEN] = { 0, };

		TItemCacheSet * pSet = GetItemCacheSet(pTab->id);

#ifdef USE_CHEQUE_CURRENCY
		sys_log(0, "[PLAYER_LOAD] ID %s pid %d gold %lld cheque %u", pTab->name, pTab->id, pTab->gold, pTab->cheque);
#else
		sys_log(0, "[PLAYER_LOAD] ID %s pid %d gold %lld", pTab->name, pTab->id, pTab->gold);
#endif

		//--------------------------------------------
		// ¾ÆÀÌÅÛ & AFFECT & QUEST ·Îµù :
		//--------------------------------------------
		// 1) ¾ÆÀÌÅÛÀÌ DBCache ¿¡ Á¸Àç : DBCache ¿¡¼­ °¡Á®¿È
		// 2) ¾ÆÀÌÅÛÀÌ DBCache ¿¡ ¾øÀ½ : DB ¿¡¼­ °¡Á®¿È

		/////////////////////////////////////////////
		// 1) ¾ÆÀÌÅÛÀÌ DBCache ¿¡ Á¸Àç : DBCache ¿¡¼­ °¡Á®¿È
		/////////////////////////////////////////////
		if (pSet)
		{
			static std::vector<TPlayerItem> s_items;
			s_items.resize(pSet->size());

			DWORD dwCount = 0;
			TItemCacheSet::iterator it = pSet->begin();

			while (it != pSet->end())
			{
				CItemCache * c = *it++;
				TPlayerItem * p = c->Get();

				if (p->vnum) // vnumÀÌ ¾øÀ¸¸é »èÁ¦µÈ ¾ÆÀÌÅÛÀÌ´Ù.
					std::memcpy(&s_items[dwCount++], p, sizeof(TPlayerItem));
			}

			if (g_test_server)
				sys_log(0, "ITEM_CACHE: HIT! %s count: %u", pTab->name, dwCount);

			peer->EncodeHeader(HEADER_DG_ITEM_LOAD, dwHandle, sizeof(DWORD) + sizeof(TPlayerItem) * dwCount);
			peer->EncodeDWORD(dwCount);

			if (dwCount)
				peer->Encode(&s_items[0], sizeof(TPlayerItem) * dwCount);

			// Quest
			snprintf(szQuery, sizeof(szQuery),
					"SELECT dwPID,szName,szState,lValue FROM quest%s WHERE dwPID=%d AND lValue<>0",
					GetTablePostfix(), pTab->id);

			CDBManager::instance().ReturnQuery(szQuery, QID_QUEST, peer->GetHandle(), new ClientHandleInfo(dwHandle,0,packet->account_id));

			// Affect
			snprintf(szQuery, sizeof(szQuery),
					"SELECT dwPID,bType,bApplyOn,lApplyValue,dwFlag,lDuration,lSPCost"
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
					",dwFlag2"
//#endif
					" FROM affect%s WHERE dwPID=%d",
					GetTablePostfix(), pTab->id);
			// @fixme402 ClientHandleInfo+pTab->id
			CDBManager::instance().ReturnQuery(szQuery, QID_AFFECT, peer->GetHandle(), new ClientHandleInfo(dwHandle, pTab->id));
		}

		/////////////////////////////////////////////
		// 2) ¾ÆÀÌÅÛÀÌ DBCache ¿¡ ¾øÀ½ : DB ¿¡¼­ °¡Á®¿È
		/////////////////////////////////////////////
		else
		{

			snprintf(szQuery, sizeof(szQuery),
				"SELECT id,window+0,pos,count,vnum"
				",grade_elements, attackelement1, attackelement2, attackelement3, elementstype, elementsvalue1, elementsvalue2, elementsvalue3,socket0,socket1,socket2,socket3,socket4,socket5,socket6,socket7,attrtype0,attrvalue0,attrtype1,attrvalue1,attrtype2,attrvalue2,attrtype3,attrvalue3,attrtype4,attrvalue4,attrtype5,attrvalue5,attrtype6,attrvalue6,attrtype7,attrvalue7,attrtype8,attrvalue8,attrtype9,attrvalue9,attrtype10,attrvalue10,attrtype11,attrvalue11,attrtype12,attrvalue12,attrtype13,attrvalue13,attrtype14,attrvalue14"
#ifdef ENABLE_YOHARA_SYSTEM
				", randomtype0, randomvalue0, randomrow0"
				", randomtype1, randomvalue1, randomrow1"
				", randomtype2, randomvalue2, randomrow2"
#endif
#ifdef ENABLE_NEW_NAME_ITEM
				",name "
#endif
				"FROM item%s WHERE owner_id=%d AND (window < %d or window = %d"
#ifdef ENABLE_SPECIAL_STORAGE
				" or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d"
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
				" or window = %d"
#endif
#ifdef CHANGE_EQUIP_WORLDARD
				" or window = %d"
#endif
				")"
				,GetTablePostfix(), pTab->id, SAFEBOX, DRAGON_SOUL_INVENTORY
#ifdef ENABLE_SPECIAL_STORAGE
				, UPGRADE_INVENTORY, BOOK_INVENTORY, STONE_INVENTORY, CHANGE_INVENTORY , COSTUME_INVENTORY, CHEST_INVENTORY
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
				,SWITCHBOT
#endif
#ifdef CHANGE_EQUIP_WORLDARD
				,CHANGE_EQUIP
#endif
				);

			CDBManager::instance().ReturnQuery(szQuery,
					QID_ITEM,
					peer->GetHandle(),
					new ClientHandleInfo(dwHandle, pTab->id));
			snprintf(szQuery, sizeof(szQuery),
					"SELECT dwPID, szName, szState, lValue FROM quest%s WHERE dwPID=%d",
					GetTablePostfix(), pTab->id);

			CDBManager::instance().ReturnQuery(szQuery,
					QID_QUEST,
					peer->GetHandle(),
					new ClientHandleInfo(dwHandle, pTab->id));
			snprintf(szQuery, sizeof(szQuery),
					"SELECT dwPID, bType, bApplyOn, lApplyValue, dwFlag, lDuration, lSPCost"
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
					", dwFlag2"
//#endif
					" FROM affect%s WHERE dwPID=%d",
					GetTablePostfix(), pTab->id);

			CDBManager::instance().ReturnQuery(szQuery,
					QID_AFFECT,
					peer->GetHandle(),
					new ClientHandleInfo(dwHandle, pTab->id));
		}
		//ljw
		//return;




	}
	//----------------------------------
	// 2. À¯ÀúÁ¤º¸°¡ DBCache ¿¡ ¾øÀ½ : DB¿¡¼­
	//----------------------------------
	else
	{
		sys_log(0, "[PLAYER_LOAD] Load from PlayerDB pid[%d]", packet->player_id);

		char queryStr[QUERY_MAX_LEN];

		//--------------------------------------------------------------
		// Ä³¸¯ÅÍ Á¤º¸ ¾ò¾î¿À±â : ¹«Á¶°Ç DB¿¡¼­
		//--------------------------------------------------------------
		snprintf(queryStr, sizeof(queryStr),
												"SELECT "
												"id, "
												"name, "
												"job, "
												"voice, "
												"dir, "
												"x, "
												"y, "
												"z, "
												"map_index, "
												"exit_x, "
												"exit_y, "
												"exit_map_index, "
												"hp, "
												"mp, "
												"stamina, "
												"random_hp, "
												"random_sp, "
												"playtime, "
												"gold, "
#ifdef USE_CHEQUE_CURRENCY
												"cheque, "
#endif
												"level, "
												"level_step, "
												"st, "
												"ht, "
												"dx, "
												"iq, "
												"exp, "
												"stat_point, "
												"skill_point, "
												"sub_skill_point, "
												"stat_reset_count, "
												"part_base, "
												"part_hair, "
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
												"pick_up_mode,"
												"rarity_mode,"
#endif
												#ifdef __SASH_SYSTEM__
												"part_sash, "
												#endif
												"skill_level, "
												"quickslot, "
												"skill_group, "
												"alignment, "
#ifdef ENABLE_YOHARA_SYSTEM
												"conquerorlevel, "
												"conqueror_level_step, "
												"sungma_str, "
												"sungma_hp, "
												"sungma_move, "
												"sungma_inmune, "
												"conqueror_exp, "
												"conqueror_point, "
#endif
												"mobile, "
												"horse_level, "
												"horse_riding, "
												"horse_hp, "
												"horse_hp_droptime, "
												"horse_stamina, "
												"UNIX_TIMESTAMP(NOW())-UNIX_TIMESTAMP(last_play), "
												"horse_skill_point"
#ifdef UNLOCK_INVENTORY_WORLDARD
												", inventory_0"
												", inventory_1"
												", inventory_2"
												", inventory_3"
												", inventory_4"
												", inventory_5"
												", inventory_6"
#endif			
#ifdef BATTLEPASS_WORLDARD
												", UNIX_TIMESTAMP(battle_pass_end) "
#endif
#ifdef ENABLE_ANTI_EXP
												", anti_exp "
#endif

#ifdef __GEM_SYSTEM__
												", gem "
#endif

#ifdef ENABLE_BATTLE_PASS
												", battlepass "
#endif
#ifdef ENABLE_NEW_DETAILS_GUI
												",kill_log "
#endif
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
												", auto_hunt_day "
												", auto_hunt_time "
#endif
#ifdef ENABLE_RANKING
												", r0, "
												"r1, "
												"r2, "
												"r3, "
												"r4, "
												"r5, "
												"r6, "
												"r7, "
												"r8, "
												"r9, "
												"r10, "
												"r11, "
												"r12, "
												"r13, "
												"r14, "
												"r15, "
												"r16, "
												"r17, "
												"r18, "
												"r19 "
#endif
												"FROM player%s WHERE id=%d"
												,
												GetTablePostfix(),
												packet->player_id
				);

		ClientHandleInfo * pkInfo = new ClientHandleInfo(dwHandle, packet->player_id);
		pkInfo->account_id = packet->account_id;
		CDBManager::instance().ReturnQuery(queryStr, QID_PLAYER, peer->GetHandle(), pkInfo);

		snprintf(queryStr, sizeof(queryStr),
			"SELECT id,window+0,pos,count,vnum"
			",socket0,socket1,socket2,socket3,socket4,socket5,socket6,socket7"
			",attrtype0,attrvalue0,attrtype1,attrvalue1,attrtype2,attrvalue2,attrtype3,attrvalue3,attrtype4,attrvalue4,attrtype5,attrvalue5,attrtype6,attrvalue6,attrtype7,attrvalue7,attrtype8,attrvalue8,attrtype9,attrvalue9,attrtype10,attrvalue10,attrtype11,attrvalue11,attrtype12,attrvalue12,attrtype13,attrvalue13,attrtype14,attrvalue14"
#ifdef ENABLE_YOHARA_SYSTEM
			", randomtype0, randomvalue0, randomrow0"
			", randomtype1, randomvalue1, randomrow1"
			", randomtype2, randomvalue2, randomrow2"
#endif
#ifdef ENABLE_NEW_NAME_ITEM
			", name "
#endif

			"FROM item%s WHERE owner_id=%d AND (window < %d or window = %d"
#ifdef ENABLE_SPECIAL_STORAGE
			" or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d"
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
			" or window = %d"
#endif
#ifdef CHANGE_EQUIP_WORLDARD
			" or window = %d"
#endif
			")"

			,GetTablePostfix(), packet->player_id, SAFEBOX, DRAGON_SOUL_INVENTORY
#ifdef ENABLE_SPECIAL_STORAGE
			, UPGRADE_INVENTORY, BOOK_INVENTORY, STONE_INVENTORY, CHANGE_INVENTORY , COSTUME_INVENTORY, CHEST_INVENTORY
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
			, SWITCHBOT
#endif
#ifdef CHANGE_EQUIP_WORLDARD
			, CHANGE_EQUIP
#endif
		);

		CDBManager::instance().ReturnQuery(queryStr, QID_ITEM, peer->GetHandle(), new ClientHandleInfo(dwHandle, packet->player_id));

		//--------------------------------------------------------------
		// QUEST °¡Á®¿À±â
		//--------------------------------------------------------------
		snprintf(queryStr, sizeof(queryStr),
				"SELECT dwPID,szName,szState,lValue FROM quest%s WHERE dwPID=%d",
				GetTablePostfix(), packet->player_id);
		CDBManager::instance().ReturnQuery(queryStr, QID_QUEST, peer->GetHandle(), new ClientHandleInfo(dwHandle, packet->player_id,packet->account_id));
		//µ¶ÀÏ ¼±¹° ±â´É¿¡¼­ item_awardÅ×ÀÌºí¿¡¼­ login Á¤º¸¸¦ ¾ò±âÀ§ÇØ account idµµ ³Ñ°ÜÁØ´Ù
		//--------------------------------------------------------------
		// AFFECT °¡Á®¿À±â
		//--------------------------------------------------------------
		snprintf(queryStr, sizeof(queryStr),
				"SELECT dwPID,bType,bApplyOn,lApplyValue,dwFlag,lDuration,lSPCost"
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
				",dwFlag2"
//#endif
				" FROM affect%s WHERE dwPID=%d",
				GetTablePostfix(), packet->player_id);
		CDBManager::instance().ReturnQuery(queryStr, QID_AFFECT, peer->GetHandle(), new ClientHandleInfo(dwHandle, packet->player_id));
	}
#ifdef ENABLE_IKASHOP_RENEWAL
	IkarusShopLoadShopSafebox(peer, packet->player_id);
#ifdef EXTEND_IKASHOP_PRO
	IkarusShopLoadNotifications(peer, packet->player_id);
#endif
#endif


}
void CClientManager::ItemAward(CPeer * peer,char* login)
{
	char login_t[LOGIN_MAX_LEN + 1] = "";
	strlcpy(login_t,login,LOGIN_MAX_LEN + 1);
	std::set<TItemAward *> * pSet = ItemAwardManager::instance().GetByLogin(login_t);
	if(pSet == NULL)
		return;
	auto it = pSet->begin();	//taken_timeÀÌ NULLÀÎ°Íµé ÀÐ¾î¿È
	while(it != pSet->end() )
	{
		TItemAward * pItemAward = *(it++);
		char* whyStr = pItemAward->szWhy;	//why ÄÝ·ë ÀÐ±â
		char cmdStr[100] = "";	//whyÄÝ·ë¿¡¼­ ÀÐÀº °ªÀ» ÀÓ½Ã ¹®ÀÚ¿­¿¡ º¹»çÇØµÒ
		strcpy(cmdStr,whyStr);	//¸í·É¾î ¾ò´Â °úÁ¤¿¡¼­ ÅäÅ«¾²¸é ¿øº»µµ ÅäÅ«È­ µÇ±â ¶§¹®
		char command[20] = "";
		// @fixme203 directly GetCommand instead of strcpy
		GetCommand(cmdStr, command);			// command ¾ò±â
		if( !(strcmp(command,"GIFT") ))	// command °¡ GIFTÀÌ¸é
		{
			TPacketItemAwardInfromer giftData;
			strcpy(giftData.login, pItemAward->szLogin);	//·Î±×ÀÎ ¾ÆÀÌµð º¹»ç
			strcpy(giftData.command, command);					//¸í·É¾î º¹»ç
			giftData.vnum = pItemAward->dwVnum;				//¾ÆÀÌÅÛ vnumµµ º¹»ç
			ForwardPacket(HEADER_DG_ITEMAWARD_INFORMER,&giftData,sizeof(TPacketItemAwardInfromer));
		}
	}
}
char* CClientManager::GetCommand(char* str, char* command) // @fixme203
{
	char* tok;

	if( str[0] == '[' )
	{
		tok = strtok(str,"]");
		strcat(command,&tok[1]);
	}

	return command;
}

bool CreatePlayerTableFromRes(MYSQL_RES * res, TPlayerTable * pkTab)
{
	if (mysql_num_rows(res) == 0)	// µ¥ÀÌÅÍ ¾øÀ½
		return false;

	memset(pkTab, 0, sizeof(TPlayerTable));

	MYSQL_ROW row = mysql_fetch_row(res);

	int	col = 0;

	// "id,name,job,voice,dir,x,y,z,map_index,exit_x,exit_y,exit_map_index,hp,mp,stamina,random_hp,random_sp,playtime,"
	// "gold,level,level_step,st,ht,dx,iq,exp,"
	// "stat_point,skill_point,sub_skill_point,stat_reset_count,part_base,part_hair,"
	// "skill_level,quickslot,skill_group,alignment,mobile,horse_level,horse_riding,horse_hp,horse_stamina FROM player%s WHERE id=%d",
	str_to_number(pkTab->id, row[col++]);
	strlcpy(pkTab->name, row[col++], sizeof(pkTab->name));
	str_to_number(pkTab->job, row[col++]);
	str_to_number(pkTab->voice, row[col++]);
	str_to_number(pkTab->dir, row[col++]);
	str_to_number(pkTab->x, row[col++]);
	str_to_number(pkTab->y, row[col++]);
	str_to_number(pkTab->z, row[col++]);
	str_to_number(pkTab->lMapIndex, row[col++]);
	str_to_number(pkTab->lExitX, row[col++]);
	str_to_number(pkTab->lExitY, row[col++]);
	str_to_number(pkTab->lExitMapIndex,  row[col++]);
	str_to_number(pkTab->hp, row[col++]);
	str_to_number(pkTab->sp, row[col++]);
	str_to_number(pkTab->stamina, row[col++]);
	str_to_number(pkTab->sRandomHP, row[col++]);
	str_to_number(pkTab->sRandomSP, row[col++]);
	str_to_number(pkTab->playtime, row[col++]);
	str_to_number(pkTab->gold, row[col++]);
#ifdef USE_CHEQUE_CURRENCY
	str_to_number(pkTab->cheque, row[col++]);
#endif
	str_to_number(pkTab->level, row[col++]);
	str_to_number(pkTab->level_step, row[col++]);
	str_to_number(pkTab->st, row[col++]);
	str_to_number(pkTab->ht, row[col++]);
	str_to_number(pkTab->dx, row[col++]);
	str_to_number(pkTab->iq, row[col++]);
	str_to_number(pkTab->exp, row[col++]);
	str_to_number(pkTab->stat_point, row[col++]);
	str_to_number(pkTab->skill_point, row[col++]);
	str_to_number(pkTab->sub_skill_point, row[col++]);
	str_to_number(pkTab->stat_reset_count, row[col++]);
	str_to_number(pkTab->part_base, row[col++]);
	str_to_number(pkTab->parts[PART_HAIR], row[col++]);
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	str_to_number(pkTab->dwPickUPMode, row[col++]);
	str_to_number(pkTab->dwRarityMode, row[col++]);
#endif
	#ifdef __SASH_SYSTEM__
	str_to_number(pkTab->parts[PART_SASH], row[col++]);
	#endif
	if (row[col])
		std::memcpy(pkTab->skills, row[col], sizeof(pkTab->skills));
	else
		memset(&pkTab->skills, 0, sizeof(pkTab->skills));

	col++;

	if (row[col])
		std::memcpy(pkTab->quickslot, row[col], sizeof(pkTab->quickslot));
	else
		memset(pkTab->quickslot, 0, sizeof(pkTab->quickslot));

	col++;

	str_to_number(pkTab->skill_group, row[col++]);
	str_to_number(pkTab->lAlignment, row[col++]);
#ifdef ENABLE_YOHARA_SYSTEM
	str_to_number(pkTab->conquerorlevel, row[col++]);
	str_to_number(pkTab->conqueror_level_step, row[col++]);	
	str_to_number(pkTab->sungma_str, row[col++]);	
	str_to_number(pkTab->sungma_hp, row[col++]);	
	str_to_number(pkTab->sungma_move, row[col++]);	
	str_to_number(pkTab->sungma_inmune, row[col++]);	
	str_to_number(pkTab->conqueror_exp, row[col++]);	
	str_to_number(pkTab->conqueror_point, row[col++]);
#endif
	if (row[col])
	{
		strlcpy(pkTab->szMobile, row[col], sizeof(pkTab->szMobile));
	}

	col++;

	str_to_number(pkTab->horse.bLevel, row[col++]);
	str_to_number(pkTab->horse.bRiding, row[col++]);
	str_to_number(pkTab->horse.sHealth, row[col++]);
	str_to_number(pkTab->horse.dwHorseHealthDropTime, row[col++]);
	str_to_number(pkTab->horse.sStamina, row[col++]);
	str_to_number(pkTab->logoff_interval, row[col++]);
	str_to_number(pkTab->horse_skill_point, row[col++]);
#ifdef UNLOCK_INVENTORY_WORLDARD
	str_to_number(pkTab->inventory_unlock[0], row[col++]);
	str_to_number(pkTab->inventory_unlock[1], row[col++]);
	str_to_number(pkTab->inventory_unlock[2], row[col++]);
	str_to_number(pkTab->inventory_unlock[3], row[col++]);
	str_to_number(pkTab->inventory_unlock[4], row[col++]);
	str_to_number(pkTab->inventory_unlock[5], row[col++]);
	str_to_number(pkTab->inventory_unlock[6], row[col++]);

#endif
#ifdef BATTLEPASS_WORLDARD
	str_to_number(pkTab->dwBattlePassEndTime, row[col++]);
#endif
#ifdef ENABLE_ANTI_EXP
	str_to_number(pkTab->anti_exp, row[col++]);
#endif
#ifdef __GEM_SYSTEM__
	str_to_number(pkTab->gem, row[col++]);
#endif
#ifdef ENABLE_BATTLE_PASS
	if (row[col])
		std::memcpy(pkTab->battlePassData, row[col], sizeof(pkTab->battlePassData));
	else
		memset(&pkTab->battlePassData, 0, sizeof(pkTab->battlePassData));
	
	col++;
#endif

#ifdef ENABLE_NEW_DETAILS_GUI
	if (row[col])
		std::memcpy(pkTab->kill_log, row[col], sizeof(pkTab->kill_log));
	else
		memset(pkTab->kill_log, 0, sizeof(pkTab->kill_log));
	col++;
#endif

#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
	str_to_number(pkTab->auto_hunt_last_day, row[col++]);
	str_to_number(pkTab->auto_hunt_used_time, row[col++]);
#endif

#ifdef ENABLE_RANKING
	for (int i = 0; i < RANKING_MAX_CATEGORIES; ++i)
		str_to_number(pkTab->lRankPoints[i], row[col++]);
#endif

	// reset sub_skill_point
	{
		pkTab->skills[123].bLevel = 0; // SKILL_CREATE

		if (pkTab->level > 9)
		{
			int max_point = pkTab->level - 9;

			int skill_point =
				MIN(20, pkTab->skills[121].bLevel) +	// SKILL_LEADERSHIP			Åë¼Ö·Â
				MIN(20, pkTab->skills[124].bLevel) +	// SKILL_MINING				Ã¤±¤
				MIN(10, pkTab->skills[131].bLevel) +	// SKILL_HORSE_SUMMON		¸»¼ÒÈ¯
				MIN(20, pkTab->skills[141].bLevel) +	// SKILL_ADD_HP				HPº¸°­
				MIN(20, pkTab->skills[142].bLevel);		// SKILL_RESIST_PENETRATE	°üÅëÀúÇ×

			pkTab->sub_skill_point = max_point - skill_point;
		}
		else
			pkTab->sub_skill_point = 0;
	}

	return true;
}

void CClientManager::RESULT_COMPOSITE_PLAYER(CPeer * peer, SQLMsg * pMsg, DWORD dwQID)
{
	CQueryInfo * qi = (CQueryInfo *) pMsg->pvUserData;
	std::unique_ptr<ClientHandleInfo> info((ClientHandleInfo *) qi->pvData);

	MYSQL_RES * pSQLResult = pMsg->Get()->pSQLResult;
	if (!pSQLResult)
	{
		sys_err("null MYSQL_RES QID %u", dwQID);
		return;
	}

	switch (dwQID)
	{
		case QID_PLAYER:
			sys_log(0, "QID_PLAYER %u %u", info->dwHandle, info->player_id);
			RESULT_PLAYER_LOAD(peer, pSQLResult, info.get());

			break;

		case QID_ITEM:
			sys_log(0, "QID_ITEM %u", info->dwHandle);
			RESULT_ITEM_LOAD(peer, pSQLResult, info->dwHandle, info->player_id);
			break;
        case QID_QUEST:
        {
            sys_log(0, "QID_QUEST %u", info->dwHandle);
            RESULT_QUEST_LOAD(peer, pSQLResult, info->dwHandle, info->player_id);

            ClientHandleInfo* temp1 = info.get();
            if (temp1 == NULL)
            {
                break;
            }

            CLoginData* pLoginData1 = GetLoginDataByAID(temp1->account_id);
            if (pLoginData1 == NULL)
            {
                break;
            }

            const char* login = pLoginData1->GetAccountRef().login;
            if (!login)
            {
                break;
            }

            sys_log(0, "info of pLoginData1 before call ItemAwardfunction %d", pLoginData1);
            ItemAward(peer, pLoginData1->GetAccountRef().login);
            break;
        }
		case QID_AFFECT:
			sys_log(0, "QID_AFFECT %u", info->dwHandle);
			// @fixme402 RESULT_AFFECT_LOAD+info->player_id
			RESULT_AFFECT_LOAD(peer, pSQLResult, info->dwHandle, info->player_id);
			break;
			/*
			   case QID_PLAYER_ITEM_QUEST_AFFECT:
			   sys_log(0, "QID_PLAYER_ITEM_QUEST_AFFECT %u", info->dwHandle);
			   RESULT_PLAYER_LOAD(peer, pSQLResult, info->dwHandle);

			   if (!pMsg->Next())
			   {
			   sys_err("RESULT_COMPOSITE_PLAYER: QID_PLAYER_ITEM_QUEST_AFFECT: ITEM FAILED");
			   return;
			   }

			   case QID_ITEM_QUEST_AFFECT:
			   sys_log(0, "QID_ITEM_QUEST_AFFECT %u", info->dwHandle);
			   RESULT_ITEM_LOAD(peer, pSQLResult, info->dwHandle, info->player_id);

			   if (!pMsg->Next())
			   {
			   sys_err("RESULT_COMPOSITE_PLAYER: QID_PLAYER_ITEM_QUEST_AFFECT: QUEST FAILED");
			   return;
			   }

			   case QID_QUEST_AFFECT:
			   sys_log(0, "QID_QUEST_AFFECT %u", info->dwHandle);
			   RESULT_QUEST_LOAD(peer, pSQLResult, info->dwHandle);

			   if (!pMsg->Next())
			   sys_err("RESULT_COMPOSITE_PLAYER: QID_PLAYER_ITEM_QUEST_AFFECT: AFFECT FAILED");
			   else
			   RESULT_AFFECT_LOAD(peer, pSQLResult, info->dwHandle);

			   break;
			   */
	}

}

void CClientManager::RESULT_PLAYER_LOAD(CPeer * peer, MYSQL_RES * pRes, ClientHandleInfo * pkInfo)
{
	TPlayerTable tab;

	if (!CreatePlayerTableFromRes(pRes, &tab))
	{
		peer->EncodeHeader(HEADER_DG_PLAYER_LOAD_FAILED, pkInfo->dwHandle, 0);
		return;
	}

	CLoginData * pkLD = GetLoginDataByAID(pkInfo->account_id);

	if (!pkLD || pkLD->IsPlay())
	{
		sys_log(0, "PLAYER_LOAD_ERROR: LoginData %p IsPlay %d", pkLD, pkLD ? pkLD->IsPlay() : 0);
		peer->EncodeHeader(HEADER_DG_PLAYER_LOAD_FAILED, pkInfo->dwHandle, 0);
		return;
	}

	pkLD->SetPlay(true);
	SendLoginToBilling(pkLD, true);
	std::memcpy(tab.aiPremiumTimes, pkLD->GetPremiumPtr(), sizeof(tab.aiPremiumTimes));

	peer->EncodeHeader(HEADER_DG_PLAYER_LOAD_SUCCESS, pkInfo->dwHandle, sizeof(TPlayerTable));
	peer->Encode(&tab, sizeof(TPlayerTable));

	if (tab.id != pkLD->GetLastPlayerID())
	{
		TPacketNeedLoginLogInfo logInfo;
		logInfo.dwPlayerID = tab.id;

		pkLD->SetLastPlayerID( tab.id );

		peer->EncodeHeader( HEADER_DG_NEED_LOGIN_LOG, pkInfo->dwHandle, sizeof(TPacketNeedLoginLogInfo) );
		peer->Encode( &logInfo, sizeof(TPacketNeedLoginLogInfo) );
	}
}

void CClientManager::RESULT_ITEM_LOAD(CPeer * peer, MYSQL_RES * pRes, DWORD dwHandle, DWORD dwPID)
{
	static std::vector<TPlayerItem> s_items;
	//DB¿¡¼­ ¾ÆÀÌÅÛ Á¤º¸¸¦ ÀÐ¾î¿Â´Ù.
	CreateItemTableFromRes(pRes, &s_items, dwPID);
	DWORD dwCount = s_items.size();

	peer->EncodeHeader(HEADER_DG_ITEM_LOAD, dwHandle, sizeof(DWORD) + sizeof(TPlayerItem) * dwCount);
	peer->EncodeDWORD(dwCount);

	//CacheSetÀ» ¸¸µç´Ù
	CreateItemCacheSet(dwPID);

	// ITEM_LOAD_LOG_ATTACH_PID
	sys_log(0, "ITEM_LOAD: count %u pid %u", dwCount, dwPID);
	// END_OF_ITEM_LOAD_LOG_ATTACH_PID

	if (dwCount)
	{
		peer->Encode(&s_items[0], sizeof(TPlayerItem) * dwCount);

		for (DWORD i = 0; i < dwCount; ++i)
			PutItemCache(&s_items[i], true); // ·ÎµåÇÑ °ÍÀº µû·Î ÀúÀåÇÒ ÇÊ¿ä ¾øÀ¸¹Ç·Î, ÀÎÀÚ bSkipQuery¿¡ true¸¦ ³Ö´Â´Ù.
	}
}

// @fixme402 (RESULT_AFFECT_LOAD +dwRealPID)
void CClientManager::RESULT_AFFECT_LOAD(CPeer * peer, MYSQL_RES * pRes, DWORD dwHandle, DWORD dwRealPID)
{
	int iNumRows;

	if ((iNumRows = mysql_num_rows(pRes)) == 0) // µ¥ÀÌÅÍ ¾øÀ½
	{
		// @fixme402 begin
		static DWORD dwPID;
		static DWORD dwCount = 0; //1;
		static TPacketAffectElement paeTable = {0};

		dwPID = dwRealPID;
		sys_log(0, "AFFECT_LOAD: count %u PID %u RealPID %u", dwCount, dwPID, dwRealPID);

		peer->EncodeHeader(HEADER_DG_AFFECT_LOAD, dwHandle, sizeof(DWORD) + sizeof(DWORD) + sizeof(TPacketAffectElement) * dwCount);
		peer->Encode(&dwPID, sizeof(DWORD));
		peer->Encode(&dwCount, sizeof(DWORD));
		peer->Encode(&paeTable, sizeof(TPacketAffectElement) * dwCount);
		// @fixme402 end
		return;
	}

	static std::vector<TPacketAffectElement> s_elements;
	s_elements.resize(iNumRows);

	DWORD dwPID = 0;

	MYSQL_ROW row;

	for (int i = 0; i < iNumRows; ++i)
	{
		TPacketAffectElement & r = s_elements[i];
		row = mysql_fetch_row(pRes);

		if (dwPID == 0)
			str_to_number(dwPID, row[0]);

		str_to_number(r.dwType, row[1]);
		str_to_number(r.bApplyOn, row[2]);
		str_to_number(r.lApplyValue, row[3]);
		str_to_number(r.dwFlag, row[4]);
		str_to_number(r.lDuration, row[5]);
		str_to_number(r.lSPCost, row[6]);
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
		str_to_number(r.dwFlag2, row[7]);
//#endif
	}

	sys_log(0, "AFFECT_LOAD: count %d PID %u", s_elements.size(), dwPID);

	DWORD dwCount = s_elements.size();

	peer->EncodeHeader(HEADER_DG_AFFECT_LOAD, dwHandle, sizeof(DWORD) + sizeof(DWORD) + sizeof(TPacketAffectElement) * dwCount);
	peer->Encode(&dwPID, sizeof(DWORD));
	peer->Encode(&dwCount, sizeof(DWORD));
	peer->Encode(&s_elements[0], sizeof(TPacketAffectElement) * dwCount);
}

void CClientManager::RESULT_QUEST_LOAD(CPeer * peer, MYSQL_RES * pRes, DWORD dwHandle, DWORD pid)
{
	int iNumRows;

	if ((iNumRows = mysql_num_rows(pRes)) == 0)
	{
		DWORD dwCount = 0;
		peer->EncodeHeader(HEADER_DG_QUEST_LOAD, dwHandle, sizeof(DWORD));
		peer->Encode(&dwCount, sizeof(DWORD));
		return;
	}

	static std::vector<TQuestTable> s_table;
	s_table.resize(iNumRows);

	MYSQL_ROW row;

	for (int i = 0; i < iNumRows; ++i)
	{
		TQuestTable & r = s_table[i];

		row = mysql_fetch_row(pRes);

		str_to_number(r.dwPID, row[0]);
		strlcpy(r.szName, row[1], sizeof(r.szName));
		strlcpy(r.szState, row[2], sizeof(r.szState));
		str_to_number(r.lValue, row[3]);
	}

	sys_log(0, "QUEST_LOAD: count %d PID %u", s_table.size(), s_table[0].dwPID);

	DWORD dwCount = s_table.size();

	peer->EncodeHeader(HEADER_DG_QUEST_LOAD, dwHandle, sizeof(DWORD) + sizeof(TQuestTable) * dwCount);
	peer->Encode(&dwCount, sizeof(DWORD));
	peer->Encode(&s_table[0], sizeof(TQuestTable) * dwCount);
}

/*
 * PLAYER SAVE
 */


void CClientManager::QUERY_PLAYER_SAVE(CPeer * peer, DWORD dwHandle, TPlayerTable * pkTab)
{
	if (g_test_server)
		sys_log(0, "PLAYER_SAVE: %s", pkTab->name);

	PutPlayerCache(pkTab);
}

typedef std::map<DWORD, time_t> time_by_id_map_t;
static time_by_id_map_t s_createTimeByAccountID;

/*
 * PLAYER CREATE
 */
void CClientManager::__QUERY_PLAYER_CREATE(CPeer *peer, DWORD dwHandle, TPlayerCreatePacket* packet)
{
	char	queryStr[QUERY_MAX_LEN];
	int		queryLen;
	int		player_id;

	// ÇÑ °èÁ¤¿¡ XÃÊ ³»·Î Ä³¸¯ÅÍ »ý¼ºÀ» ÇÒ ¼ö ¾ø´Ù.
	time_by_id_map_t::iterator it = s_createTimeByAccountID.find(packet->account_id);

	if (it != s_createTimeByAccountID.end())
	{
		time_t curtime = time(0);

		if (curtime - it->second < 30)
		{
			peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_FAILED, dwHandle, 0);
			return;
		}
	}

	queryLen = snprintf(queryStr, sizeof(queryStr),
			"SELECT pid%u FROM player_index%s WHERE id=%d", packet->account_index + 1, GetTablePostfix(), packet->account_id);

	std::unique_ptr<SQLMsg> pMsg0(CDBManager::instance().DirectQuery(queryStr));

	if (pMsg0->Get()->uiNumRows != 0)
	{
		if (!pMsg0->Get()->pSQLResult)
		{
			peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_FAILED, dwHandle, 0);
			return;
		}

		MYSQL_ROW row = mysql_fetch_row(pMsg0->Get()->pSQLResult);

		DWORD dwPID = 0; str_to_number(dwPID, row[0]);
		if (row[0] && dwPID > 0)
		{
			peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_ALREADY, dwHandle, 0);
			sys_log(0, "ALREADY EXIST AccountChrIdx %d ID %d", packet->account_index, dwPID);
			return;
		}
	}
	else
	{
		peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_FAILED, dwHandle, 0);
		return;
	}

	if (g_stLocale == "sjis")
		snprintf(queryStr, sizeof(queryStr),
			"SELECT COUNT(*) as count FROM player%s WHERE name='%s' collate sjis_japanese_ci",
			GetTablePostfix(), packet->player_table.name);
	else
	snprintf(queryStr, sizeof(queryStr),
			"SELECT COUNT(*) as count FROM player%s WHERE name='%s'", GetTablePostfix(), packet->player_table.name);

	std::unique_ptr<SQLMsg> pMsg1(CDBManager::instance().DirectQuery(queryStr));

	if (pMsg1->Get()->uiNumRows)
	{
		if (!pMsg1->Get()->pSQLResult)
		{
			peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_FAILED, dwHandle, 0);
			return;
		}

		MYSQL_ROW row = mysql_fetch_row(pMsg1->Get()->pSQLResult);

		if (*row[0] != '0')
		{
			sys_log(0, "ALREADY EXIST name %s, row[0] %s query %s", packet->player_table.name, row[0], queryStr);
			peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_ALREADY, dwHandle, 0);
			return;
		}
	}
	else
	{
		peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_FAILED, dwHandle, 0);
		return;
	}
#ifdef __FAKE_PLAYER__
	snprintf(queryStr, sizeof(queryStr), "SELECT COUNT(*) as count FROM fake_player WHERE name = '%s'", packet->player_table.name);
	std::unique_ptr<SQLMsg> pMsg4(CDBManager::instance().DirectQuery(queryStr));

	if (pMsg4->Get()->uiNumRows)
	{
		if (!pMsg4->Get()->pSQLResult)
		{
			peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_FAILED, dwHandle, 0);
			return;
		}

		MYSQL_ROW row = mysql_fetch_row(pMsg4->Get()->pSQLResult);
		if (*row[0] != '0')
		{
			peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_ALREADY, dwHandle, 0);
			return;
		}
	}
	else
	{
		peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_FAILED, dwHandle, 0);
		return;
	}
#endif	

	queryLen = snprintf(queryStr, sizeof(queryStr), 
													"INSERT INTO player%s ("
													"id, "
													"account_id, "
													"name, "
													"level, "
													"st, "
													"ht, "
													"dx, "
													"iq, "
													"job, "
													"voice, "
													"dir, "
													"x, "
													"y, "
													"z, "
													"hp, "
													"mp, "
													"random_hp, "
													"random_sp, "
													"stat_point, "
													"stamina, "
													"part_base, "
													"part_main, "
													"part_hair, "
													#ifdef __SASH_SYSTEM__
													"part_sash, "
													#endif
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
													"pick_up_mode, "
													"rarity_mode, "

#endif
													"gold, "
#ifdef USE_CHEQUE_CURRENCY
													"cheque, "
#endif
													"playtime, "
													"skill_level, "
													"quickslot "
													") VALUES("
													"0, "
													"%u, "
													"'%s', "
													"%u, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													"%d, "
													#ifdef __SASH_SYSTEM__
													"0, "
													#endif
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
													"%d, "
													"%d, "
#endif
													"%lld, "
#ifdef USE_CHEQUE_CURRENCY
													"%u, "
#endif
													"0, "
													,
													GetTablePostfix(),
													packet->account_id,
													packet->player_table.name,
													packet->player_table.level,
													packet->player_table.st,
													packet->player_table.ht,
													packet->player_table.dx,
													packet->player_table.iq,
													packet->player_table.job,
													packet->player_table.voice,
													packet->player_table.dir,
													packet->player_table.x,
													packet->player_table.y,
													packet->player_table.z,
													packet->player_table.hp,
													packet->player_table.sp,
													packet->player_table.sRandomHP,
													packet->player_table.sRandomSP,
													packet->player_table.stat_point,
													packet->player_table.stamina,
													packet->player_table.part_base,
													packet->player_table.part_base,
													packet->player_table.part_base,
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
													packet->player_table.dwPickUPMode,
													packet->player_table.dwRarityMode,
#endif
													packet->player_table.gold
#ifdef USE_CHEQUE_CURRENCY
													, packet->player_table.cheque
#endif
	);

	sys_log(0, "PlayerCreate accountid %d name %s level %d gold %lld, st %d ht %d job %d",
			packet->account_id,
			packet->player_table.name,
			packet->player_table.level,
			packet->player_table.gold,
			packet->player_table.st,
			packet->player_table.ht,
			packet->player_table.job);

	static char text[4096 + 1];

	CDBManager::instance().EscapeString(text, packet->player_table.skills, sizeof(packet->player_table.skills));
	queryLen += snprintf(queryStr + queryLen, sizeof(queryStr) - queryLen, "'%s', ", text);
	if (g_test_server)
		sys_log(0, "Create_Player queryLen[%d] TEXT[%s]", queryLen, text);


	CDBManager::instance().EscapeString(text, packet->player_table.quickslot, sizeof(packet->player_table.quickslot));
	queryLen += snprintf(queryStr + queryLen, sizeof(queryStr) - queryLen, "'%s')", text);

	std::unique_ptr<SQLMsg> pMsg2(CDBManager::instance().DirectQuery(queryStr));
	if (g_test_server)
		sys_log(0, "Create_Player queryLen[%d] TEXT[%s]", queryLen, text);

	if (pMsg2->Get()->uiAffectedRows <= 0)
	{
		peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_ALREADY, dwHandle, 0);
		sys_log(0, "ALREADY EXIST3 query: %s AffectedRows %lu", queryStr, pMsg2->Get()->uiAffectedRows);
		return;
	}

	player_id = pMsg2->Get()->uiInsertID;

	snprintf(queryStr, sizeof(queryStr), "UPDATE player_index%s SET pid%d=%d WHERE id=%d",
			GetTablePostfix(), packet->account_index + 1, player_id, packet->account_id);
	std::unique_ptr<SQLMsg> pMsg3(CDBManager::instance().DirectQuery(queryStr));

	if (pMsg3->Get()->uiAffectedRows <= 0)
	{
		sys_err("QUERY_ERROR: %s", queryStr);

		snprintf(queryStr, sizeof(queryStr), "DELETE FROM player%s WHERE id=%d", GetTablePostfix(), player_id);
		CDBManager::instance().DirectQuery(queryStr);

		peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_FAILED, dwHandle, 0);
		return;
	}

	TPacketDGCreateSuccess pack;
	memset(&pack, 0, sizeof(pack));

	pack.bAccountCharacterIndex = packet->account_index;

	pack.player.dwID			= player_id;
	strlcpy(pack.player.szName, packet->player_table.name, sizeof(pack.player.szName));
	pack.player.byJob			= packet->player_table.job;
	pack.player.byLevel			= 1;
	pack.player.dwPlayMinutes	= 0;
	pack.player.byST			= packet->player_table.st;
	pack.player.byHT			= packet->player_table.ht;
	pack.player.byDX 			= packet->player_table.dx;
	pack.player.byIQ			= packet->player_table.iq;
	pack.player.wMainPart		= packet->player_table.part_base;
	pack.player.x			= packet->player_table.x;
	pack.player.y			= packet->player_table.y;

	peer->EncodeHeader(HEADER_DG_PLAYER_CREATE_SUCCESS, dwHandle, sizeof(TPacketDGCreateSuccess));
	peer->Encode(&pack, sizeof(TPacketDGCreateSuccess));

	sys_log(0, "7 name %s job %d", pack.player.szName, pack.player.byJob);

	s_createTimeByAccountID[packet->account_id] = time(0);
}

/*
 * PLAYER DELETE
 */
void CClientManager::__QUERY_PLAYER_DELETE(CPeer* peer, DWORD dwHandle, TPlayerDeletePacket* packet)
{
	if (!packet->login[0] || !packet->player_id || packet->account_index >= PLAYER_PER_ACCOUNT)
		return;

	CLoginData * ld = GetLoginDataByLogin(packet->login);

	if (!ld)
	{
		peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_FAILED, dwHandle, 1);
		peer->EncodeBYTE(packet->account_index);
		return;
	}

	TAccountTable & r = ld->GetAccountRef();

	// block for japan
	if (g_stLocale != "sjis")
	{
		if (!IsChinaEventServer())
		{
			if (strlen(r.social_id) < 7 || strncmp(packet->private_code, r.social_id + strlen(r.social_id) - 7, 7))
			{
				sys_log(0, "PLAYER_DELETE FAILED len(%d)", strlen(r.social_id));
				peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_FAILED, dwHandle, 1);
				peer->EncodeBYTE(packet->account_index);
				return;
			}

			CPlayerTableCache * pkPlayerCache = GetPlayerCache(packet->player_id);
			if (pkPlayerCache)
			{
				TPlayerTable * pTab = pkPlayerCache->Get();

				if (pTab->level >= m_iPlayerDeleteLevelLimit)
				{
					sys_log(0, "PLAYER_DELETE FAILED LEVEL %u >= DELETE LIMIT %d", pTab->level, m_iPlayerDeleteLevelLimit);
					peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_FAILED, dwHandle, 1);
					peer->EncodeBYTE(packet->account_index);
					return;
				}

				if (pTab->level < m_iPlayerDeleteLevelLimitLower)
				{
					sys_log(0, "PLAYER_DELETE FAILED LEVEL %u < DELETE LIMIT %d", pTab->level, m_iPlayerDeleteLevelLimitLower);
					peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_FAILED, dwHandle, 1);
					peer->EncodeBYTE(packet->account_index);
					return;
				}
			}
		}
	}
#ifdef ENABLE_IKASHOP_RENEWAL
	if (IsUsingIkarusShopSystem(packet->player_id))
	{
		sys_log(0, "PLAYER_DELETE FAILED %u IS USING OFFLINESHOP SYSTEM", packet->player_id);
		peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_FAILED, dwHandle, 1);
		peer->EncodeBYTE(packet->account_index);
		return;
	}
#endif

	char szQuery[128];
	snprintf(szQuery, sizeof(szQuery), "SELECT p.id, p.level, p.name FROM player_index%s AS i, player%s AS p WHERE pid%u=%u AND pid%u=p.id",
			GetTablePostfix(), GetTablePostfix(), packet->account_index + 1, packet->player_id, packet->account_index + 1);

	ClientHandleInfo * pi = new ClientHandleInfo(dwHandle, packet->player_id);
	pi->account_index = packet->account_index;

	sys_log(0, "PLAYER_DELETE TRY: %s %d pid%d", packet->login, packet->player_id, packet->account_index + 1);
	CDBManager::instance().ReturnQuery(szQuery, QID_PLAYER_DELETE, peer->GetHandle(), pi);
}

void CClientManager::__QUERY_PLAYER_HAIR_CHANGE(CPeer* peer, DWORD dwHandle, TPlayerChangeHairPacket* packet)
{
	if (!packet->login[0] || packet->player_index >= PLAYER_PER_ACCOUNT)
		return;

	CLoginData * ld = GetLoginDataByLogin(packet->login);

	if (!ld)
	{
		return;
	}

	TAccountTable & r = ld->GetAccountRef();
	r.players[packet->player_index].wHairPart = packet->hairIndex;
	
	char szQuery[256 + 1];
	snprintf(szQuery, sizeof(szQuery), "UPDATE player%s SET part_hair=%d WHERE id=%d", 
	GetTablePostfix(), packet->hairIndex, r.players[packet->player_index].dwID);
	
	sys_log(0, "PLAYER_HAIR_CHANGE: %s %d pid%d hair %d", packet->login, 
				r.players[packet->player_index].dwID, packet->player_index + 1, packet->hairIndex);
	
	CDBManager::instance().AsyncQuery(szQuery);
	
	CPlayerTableCache* pPlayerCache = GetPlayerCache(r.players[packet->player_index].dwID);
	
	if (pPlayerCache)
	{
		TPlayerTable* pTable = pPlayerCache->Get();
		pTable->parts[PART_HAIR] = packet->hairIndex;
		PutPlayerCache(pTable);
	}
}

//
// @version	05/06/10 Bang2ni - ÇÃ·¹ÀÌ¾î »èÁ¦½Ã °¡°ÝÁ¤º¸ ¸®½ºÆ® »èÁ¦ Ãß°¡.
//
void CClientManager::__RESULT_PLAYER_DELETE(CPeer *peer, SQLMsg* msg)
{
	CQueryInfo * qi = (CQueryInfo *) msg->pvUserData;
	ClientHandleInfo * pi = (ClientHandleInfo *) qi->pvData;

	if (msg->Get() && msg->Get()->uiNumRows)
	{
		MYSQL_ROW row = mysql_fetch_row(msg->Get()->pSQLResult);

		DWORD dwPID = 0;
		str_to_number(dwPID, row[0]);

		int deletedLevelLimit = 0;
		str_to_number(deletedLevelLimit, row[1]);

		char szName[64];
		strlcpy(szName, row[2], sizeof(szName));

		if (deletedLevelLimit >= m_iPlayerDeleteLevelLimit && !IsChinaEventServer())
		{
			sys_log(0, "PLAYER_DELETE FAILED LEVEL %u >= DELETE LIMIT %d", deletedLevelLimit, m_iPlayerDeleteLevelLimit);
			peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_FAILED, pi->dwHandle, 1);
			peer->EncodeBYTE(pi->account_index);
			return;
		}

		if (deletedLevelLimit < m_iPlayerDeleteLevelLimitLower)
		{
			sys_log(0, "PLAYER_DELETE FAILED LEVEL %u < DELETE LIMIT %d", deletedLevelLimit, m_iPlayerDeleteLevelLimitLower);
			peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_FAILED, pi->dwHandle, 1);
			peer->EncodeBYTE(pi->account_index);
			return;
		}

		char queryStr[QUERY_MAX_LEN];

		snprintf(queryStr, sizeof(queryStr), "INSERT INTO player%s_deleted SELECT * FROM player%s WHERE id=%d",
				GetTablePostfix(), GetTablePostfix(), pi->player_id);
		std::unique_ptr<SQLMsg> pIns(CDBManager::instance().DirectQuery(queryStr));

		if (pIns->Get()->uiAffectedRows == 0 || pIns->Get()->uiAffectedRows == (uint32_t)-1)
		{
			sys_log(0, "PLAYER_DELETE FAILED %u CANNOT INSERT TO player%s_deleted", dwPID, GetTablePostfix());

			peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_FAILED, pi->dwHandle, 1);
			peer->EncodeBYTE(pi->account_index);
			return;
		}

		// »èÁ¦ ¼º°ø
		sys_log(0, "PLAYER_DELETE SUCCESS %u", dwPID);

		char account_index_string[16];

		snprintf(account_index_string, sizeof(account_index_string), "player_id%d", m_iPlayerIDStart + pi->account_index);

		// ÇÃ·¹ÀÌ¾î Å×ÀÌºíÀ» Ä³½¬¿¡¼­ »èÁ¦ÇÑ´Ù.
		CPlayerTableCache * pkPlayerCache = GetPlayerCache(pi->player_id);

		if (pkPlayerCache)
		{
			m_map_playerCache.erase(pi->player_id);
			delete pkPlayerCache;
		}

		// ¾ÆÀÌÅÛµéÀ» Ä³½¬¿¡¼­ »èÁ¦ÇÑ´Ù.
		TItemCacheSet * pSet = GetItemCacheSet(pi->player_id);

		if (pSet)
		{
			TItemCacheSet::iterator it = pSet->begin();

			while (it != pSet->end())
			{
				CItemCache * pkItemCache = *it++;
				DeleteItemCache(pkItemCache->Get()->id);
			}

			pSet->clear();
			delete pSet;

			m_map_pkItemCacheSetPtr.erase(pi->player_id);
		}

		snprintf(queryStr, sizeof(queryStr), "UPDATE player_index%s SET pid%u=0 WHERE pid%u=%d",
				GetTablePostfix(),
				pi->account_index + 1,
				pi->account_index + 1,
				pi->player_id);

		std::unique_ptr<SQLMsg> pMsg(CDBManager::instance().DirectQuery(queryStr));

		if (pMsg->Get()->uiAffectedRows == 0 || pMsg->Get()->uiAffectedRows == (uint32_t)-1)
		{
			sys_log(0, "PLAYER_DELETE FAIL WHEN UPDATE account table");
			peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_FAILED, pi->dwHandle, 1);
			peer->EncodeBYTE(pi->account_index);
			return;
		}

		snprintf(queryStr, sizeof(queryStr), "DELETE FROM player%s WHERE id=%d", GetTablePostfix(), pi->player_id);
		delete CDBManager::instance().DirectQuery(queryStr);

#ifdef ENABLE_SPECIAL_STORAGE
#ifdef ENABLE_SWITCHBOT_WORLDARD //Replace your old line with the whole ifdef/else statement
#ifdef CHANGE_EQUIP_WORLDARD
		snprintf(queryStr, sizeof(queryStr), "DELETE FROM item%s WHERE owner_id=%d AND (window < %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d)", 
		GetTablePostfix(), pi->player_id, SAFEBOX, DRAGON_SOUL_INVENTORY, UPGRADE_INVENTORY, BOOK_INVENTORY, STONE_INVENTORY, CHANGE_INVENTORY, COSTUME_INVENTORY, CHEST_INVENTORY, SWITCHBOT, CHANGE_EQUIP);
#else
		snprintf(queryStr, sizeof(queryStr), "DELETE FROM item%s WHERE owner_id=%d AND (window < %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d)", 
		GetTablePostfix(), pi->player_id, SAFEBOX, DRAGON_SOUL_INVENTORY, UPGRADE_INVENTORY, BOOK_INVENTORY, STONE_INVENTORY, CHANGE_INVENTORY, COSTUME_INVENTORY, CHEST_INVENTORY, SWITCHBOT);
#endif
#else
#ifdef CHANGE_EQUIP_WORLDARD
		snprintf(queryStr, sizeof(queryStr), "DELETE FROM item%s WHERE owner_id=%d AND (window < %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d)", 
		GetTablePostfix(), pi->player_id, SAFEBOX, DRAGON_SOUL_INVENTORY, UPGRADE_INVENTORY, BOOK_INVENTORY, STONE_INVENTORY, CHANGE_INVENTORY, COSTUME_INVENTORY, CHEST_INVENTORY, CHANGE_EQUIP);
#else
		snprintf(queryStr, sizeof(queryStr), "DELETE FROM item%s WHERE owner_id=%d AND (window < %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d or window = %d)", 
		GetTablePostfix(), pi->player_id, SAFEBOX, DRAGON_SOUL_INVENTORY, UPGRADE_INVENTORY, BOOK_INVENTORY, STONE_INVENTORY, COSTUME_INVENTORY, CHANGE_INVENTORY, CHEST_INVENTORY);
#endif
#endif
#else
#ifdef ENABLE_SWITCHBOT_WORLDARD //Replace your old line with the whole ifdef/else statement
#ifdef CHANGE_EQUIP_WORLDARD
		snprintf(queryStr, sizeof(queryStr), "DELETE FROM item%s WHERE owner_id=%d AND (window < %d or window = %d or window = %d or window = %d)", GetTablePostfix(), pi->player_id, SAFEBOX, DRAGON_SOUL_INVENTORY, SWITCHBOT, CHANGE_EQUIP);
#else
		snprintf(queryStr, sizeof(queryStr), "DELETE FROM item%s WHERE owner_id=%d AND (window < %d or window = %d or window = %d)", GetTablePostfix(), pi->player_id, SAFEBOX, DRAGON_SOUL_INVENTORY, SWITCHBOT);
#endif
#else
#ifdef CHANGE_EQUIP_WORLDARD
		snprintf(queryStr, sizeof(queryStr), "DELETE FROM item%s WHERE owner_id=%d AND (window < %d or window = %d or window = %d)", GetTablePostfix(), pi->player_id, SAFEBOX, DRAGON_SOUL_INVENTORY, CHANGE_EQUIP);

#else
		snprintf(queryStr, sizeof(queryStr), "DELETE FROM item%s WHERE owner_id=%d AND (window < %d or window = %d)", GetTablePostfix(), pi->player_id, SAFEBOX, DRAGON_SOUL_INVENTORY);
#endif
#endif
#endif

		delete CDBManager::instance().DirectQuery(queryStr);

		snprintf(queryStr, sizeof(queryStr), "DELETE FROM quest%s WHERE dwPID=%d", GetTablePostfix(), pi->player_id);
		CDBManager::instance().AsyncQuery(queryStr);

		snprintf(queryStr, sizeof(queryStr), "DELETE FROM affect%s WHERE dwPID=%d", GetTablePostfix(), pi->player_id);
		CDBManager::instance().AsyncQuery(queryStr);

		snprintf(queryStr, sizeof(queryStr), "DELETE FROM guild_member%s WHERE pid=%d", GetTablePostfix(), pi->player_id);
		CDBManager::instance().AsyncQuery(queryStr);


		snprintf(queryStr, sizeof(queryStr), "DELETE FROM messenger_list%s WHERE account='%s' OR companion='%s'", GetTablePostfix(), szName, szName);
		CDBManager::instance().AsyncQuery(queryStr);

		peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_SUCCESS, pi->dwHandle, 1);
		peer->EncodeBYTE(pi->account_index);
	}
	else
	{
		// »èÁ¦ ½ÇÆÐ
		sys_log(0, "PLAYER_DELETE FAIL NO ROW");
		peer->EncodeHeader(HEADER_DG_PLAYER_DELETE_FAILED, pi->dwHandle, 1);
		peer->EncodeBYTE(pi->account_index);
	}
}

void CClientManager::QUERY_ADD_AFFECT(CPeer * peer, TPacketGDAddAffect * p)
{
	char queryStr[QUERY_MAX_LEN];
	/*
	   snprintf(queryStr, sizeof(queryStr),
	   "INSERT INTO affect%s (dwPID, bType, bApplyOn, lApplyValue, dwFlag, lDuration, lSPCost) "
	   "VALUES(%u, %u, %u, %d, %u, %d, %d) "
	   "ON DUPLICATE KEY UPDATE lApplyValue=%d, dwFlag=%u, lDuration=%d, lSPCost=%d",
	   GetTablePostfix(),
	   p->dwPID,
	   p->elem.dwType,
	   p->elem.bApplyOn,
	   p->elem.lApplyValue,
	   p->elem.dwFlag,
	   p->elem.lDuration,
	   p->elem.lSPCost,
	   p->elem.lApplyValue,
	   p->elem.dwFlag,
	   p->elem.lDuration,
	   p->elem.lSPCost);
	   */
	snprintf(queryStr, sizeof(queryStr),
			"REPLACE INTO affect%s (dwPID, bType, bApplyOn, lApplyValue, dwFlag, lDuration, lSPCost"
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
			", dwFlag2"
//#endif
			") "
			"VALUES(%u, %u, %u, %ld, %u, %ld, %ld"
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
			", %u"
//#endif
			")",
			GetTablePostfix(),
			p->dwPID,
			p->elem.dwType,
			p->elem.bApplyOn,
			p->elem.lApplyValue,
			p->elem.dwFlag,
			p->elem.lDuration,
			p->elem.lSPCost
//#if defined(USE_BOOST_POTIONS) || defined(USE_CRYSTAL_SYSTEM)
			, p->elem.dwFlag2
//#endif
			);

	CDBManager::instance().AsyncQuery(queryStr);
}

void CClientManager::QUERY_REMOVE_AFFECT(CPeer * peer, TPacketGDRemoveAffect * p)
{
	char queryStr[QUERY_MAX_LEN];

	snprintf(queryStr, sizeof(queryStr),
			"DELETE FROM affect%s WHERE dwPID=%u AND bType=%u AND bApplyOn=%u",
			GetTablePostfix(), p->dwPID, p->dwType, p->bApplyOn);

	CDBManager::instance().AsyncQuery(queryStr);
}


void CClientManager::QUERY_HIGHSCORE_REGISTER(CPeer* peer, TPacketGDHighscore * data)
{
	char szQuery[128];
	snprintf(szQuery, sizeof(szQuery), "SELECT value FROM highscore%s WHERE board='%s' AND pid = %u", GetTablePostfix(), data->szBoard, data->dwPID);

	sys_log(0, "HEADER_GD_HIGHSCORE_REGISTER: PID %u", data->dwPID);

	ClientHandleInfo * pi = new ClientHandleInfo(0);
	strlcpy(pi->login, data->szBoard, sizeof(pi->login));
	pi->account_id = (DWORD)data->lValue;
	pi->player_id = data->dwPID;
	pi->account_index = (data->cDir > 0);

	CDBManager::instance().ReturnQuery(szQuery, QID_HIGHSCORE_REGISTER, peer->GetHandle(), pi);
}

void CClientManager::RESULT_HIGHSCORE_REGISTER(CPeer * pkPeer, SQLMsg * msg)
{
	CQueryInfo * qi = (CQueryInfo *) msg->pvUserData;
	ClientHandleInfo * pi = (ClientHandleInfo *) qi->pvData;
	//DWORD dwHandle = pi->dwHandle;

	char szBoard[21];
	strlcpy(szBoard, pi->login, sizeof(szBoard));
	int value = (int)pi->account_id;

	SQLResult * res = msg->Get();

	if (res->uiNumRows == 0)
	{
		// »õ·Î¿î ÇÏÀÌ½ºÄÚ¾î¸¦ »ðÀÔ
		char buf[256];
		snprintf(buf, sizeof(buf), "INSERT INTO highscore%s VALUES('%s', %u, %d)", GetTablePostfix(), szBoard, pi->player_id, value);
		CDBManager::instance().AsyncQuery(buf);
	}
	else
	{
		if (!res->pSQLResult)
		{
			delete pi;
			return;
		}

		MYSQL_ROW row = mysql_fetch_row(res->pSQLResult);
		if (row && row[0])
		{
			int current_value = 0; str_to_number(current_value, row[0]);
			if (((pi->account_index)&&(current_value >= value)) || ((!pi->account_index)&&(current_value <= value)))
			{
				value = current_value;
			}
			else
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "REPLACE INTO highscore%s VALUES('%s', %u, %d)", GetTablePostfix(), szBoard, pi->player_id, value);
				CDBManager::instance().AsyncQuery(buf);
			}
		}
		else
		{
			char buf[256];
			snprintf(buf, sizeof(buf), "INSERT INTO highscore%s VALUES('%s', %u, %d)", GetTablePostfix(), szBoard, pi->player_id, value);
			CDBManager::instance().AsyncQuery(buf);
		}
	}
	// TODO: ÀÌ°÷¿¡¼­ ÇÏÀÌ½ºÄÚ¾î°¡ ¾÷µ¥ÀÌÆ® µÇ¾ú´ÂÁö Ã¼Å©ÇÏ¿© °øÁö¸¦ »Ñ·Á¾ßÇÑ´Ù.
	delete pi;
}

void CClientManager::InsertLogoutPlayer(DWORD pid)
{
	TLogoutPlayerMap::iterator it = m_map_logout.find(pid);

	// Á¸ÀçÇÏÁö ¾ÊÀ»°æ¿ì Ãß°¡
	if (it != m_map_logout.end())
	{
		// Á¸ÀçÇÒ°æ¿ì ½Ã°£¸¸ °»½Å
		if (g_log)
			sys_log(0, "LOGOUT: Update player time pid(%d)", pid);

		it->second->time = time(0);
		return;
	}

	TLogoutPlayer * pLogout = new TLogoutPlayer;
	pLogout->pid = pid;
	pLogout->time = time(0);
	m_map_logout.insert(std::make_pair(pid, pLogout));

	if (g_log)
		sys_log(0, "LOGOUT: Insert player pid(%d)", pid);
}

void CClientManager::DeleteLogoutPlayer(DWORD pid)
{
	TLogoutPlayerMap::iterator it = m_map_logout.find(pid);

	if (it != m_map_logout.end())
	{
		delete it->second;
		m_map_logout.erase(it);
	}
}

extern int g_iLogoutSeconds;

void CClientManager::UpdateLogoutPlayer()
{
	time_t now = time(0);

	TLogoutPlayerMap::iterator it = m_map_logout.begin();

	while (it != m_map_logout.end())
	{
		TLogoutPlayer* pLogout = it->second;

		if (now - g_iLogoutSeconds > pLogout->time)
		{
			FlushItemCacheSet(pLogout->pid);
			FlushPlayerCacheSet(pLogout->pid);

			delete pLogout;
			m_map_logout.erase(it++);
		}
		else
			++it;
	}
}

void CClientManager::FlushPlayerCacheSet(DWORD pid)
{
	TPlayerTableCacheMap::iterator it = m_map_playerCache.find(pid);

	if (it != m_map_playerCache.end())
	{
		CPlayerTableCache * c = it->second;
		m_map_playerCache.erase(it);

		c->Flush();
		delete c;
	}
}

