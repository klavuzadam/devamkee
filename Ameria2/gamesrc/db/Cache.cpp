
#include "stdafx.h"
#include "Cache.h"

#include "QID.h"
#include "ClientManager.h"
#include "Main.h"

extern CPacketInfo g_item_info;
extern int g_iPlayerCacheFlushSeconds;
extern int g_iItemCacheFlushSeconds;
extern int g_test_server;

extern int g_item_count;
const int auctionMinFlushSec = 1800;

CItemCache::CItemCache()
{
	m_expireTime = MIN(1800, g_iItemCacheFlushSeconds);
}

CItemCache::~CItemCache()
{
}

// ÀÌ°Å ÀÌ»óÇÑµ¥...
// Delete¸¦ ÇßÀ¸¸é, Cacheµµ ÇØÁ¦ÇØ¾ß ÇÏ´Â°Í ¾Æ´Ñ°¡???
// ±Ùµ¥ Cache¸¦ ÇØÁ¦ÇÏ´Â ºÎºÐÀÌ ¾ø¾î.
// ¸ø Ã£Àº °Ç°¡?
// ÀÌ·¸°Ô ÇØ³õÀ¸¸é, °è¼Ó ½Ã°£ÀÌ µÉ ¶§¸¶´Ù ¾ÆÀÌÅÛÀ» °è¼Ó Áö¿ö...
// ÀÌ¹Ì »ç¶óÁø ¾ÆÀÌÅÛÀÎµ¥... È®ÀÎ»ç»ì??????
// fixme
// by rtsummit
void CItemCache::Delete()
{
	if (m_data.vnum == 0)
		return;

	//char szQuery[QUERY_MAX_LEN];
	//szQuery[QUERY_MAX_LEN] = '\0';
	if (g_test_server)
		sys_log(0, "ItemCache::Delete : DELETE %u", m_data.id);

	m_data.vnum = 0;
	m_bNeedQuery = true;
	m_lastUpdateTime = time(0);
	OnFlush();

	//m_bNeedQuery = false;
	//m_lastUpdateTime = time(0) - m_expireTime; // ¹Ù·Î Å¸ÀÓ¾Æ¿ô µÇµµ·Ï ÇÏÀÚ.
}

void CItemCache::OnFlush()
{
	if (m_data.vnum == 0) // vnumÀÌ 0ÀÌ¸é »èÁ¦ÇÏ¶ó°í Ç¥½ÃµÈ °ÍÀÌ´Ù.
	{
		char szQuery[QUERY_MAX_LEN];
		snprintf(szQuery, sizeof(szQuery), "DELETE FROM item%s WHERE id=%u", GetTablePostfix(), m_data.id);
		CDBManager::instance().ReturnQuery(szQuery, QID_ITEM_DESTROY, 0, NULL);

		if (g_test_server)
			sys_log(0, "ItemCache::Flush : DELETE %u %s", m_data.id, szQuery);
	}
	else
	{
		long alSockets[ITEM_SOCKET_MAX_NUM];
		bool isSocket = false;

		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
		bool isAttr = false;

#ifdef ENABLE_YOHARA_SYSTEM
		TPlayerItemApplyRandom	aApplyRandom[APPLY_RANDOM_SLOT_MAX_NUM];
		bool isRandomAttr = false;
#endif

		memset(&alSockets, 0, sizeof(long) * ITEM_SOCKET_MAX_NUM);
		memset(&aAttr, 0, sizeof(TPlayerItemAttribute) * ITEM_ATTRIBUTE_MAX_NUM);
#ifdef ENABLE_YOHARA_SYSTEM
		memset(&aApplyRandom, 0, sizeof(TPlayerItemAttribute) * APPLY_RANDOM_SLOT_MAX_NUM);
#endif

		TPlayerItem * p = &m_data;

		if (memcmp(alSockets, p->alSockets, sizeof(long) * ITEM_SOCKET_MAX_NUM))
			isSocket = true;

		if (memcmp(aAttr, p->aAttr, sizeof(TPlayerItemAttribute) * ITEM_ATTRIBUTE_MAX_NUM))
			isAttr = true;

#ifdef ENABLE_YOHARA_SYSTEM
		if (memcmp(aApplyRandom, p->aApplyRandom, sizeof(TPlayerItemAttribute) * APPLY_RANDOM_SLOT_MAX_NUM))
			isRandomAttr = true;
#endif

		char szColumns[QUERY_MAX_LEN];
		char szValues[QUERY_MAX_LEN];
		char szUpdate[QUERY_MAX_LEN];

		int iLen = snprintf(szColumns, sizeof(szColumns), "id, owner_id, window, pos, count, vnum");

		int iValueLen = snprintf(szValues, sizeof(szValues), "%u, %u, %d, %d, %u, %u",
				p->id, p->owner, p->window, p->pos, p->count, p->vnum);

		int iUpdateLen = snprintf(szUpdate, sizeof(szUpdate), "owner_id=%u, window=%d, pos=%d, count=%u, vnum=%u",
				p->owner, p->window, p->pos, p->count, p->vnum);

#ifdef ENABLE_NEW_NAME_ITEM
		iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", name");
		iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,", '%s'", p->name);
		iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,", name='%s'", p->name);
#endif

		if (isSocket)
		{
			for(BYTE j=0;j<ITEM_SOCKET_MAX_NUM;++j)
			{
				iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", socket%d",j);
				iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,", %lu", p->alSockets[j]);
				iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,", socket%d=%lu", j, p->alSockets[j]);
			}
		}

		if (isAttr)
		{
			for(BYTE j=0;j<ITEM_ATTRIBUTE_MAX_NUM;++j)
			{
				iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen,", attrtype%d, attrvalue%d",j,j);
				iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,", %d, %d",p->aAttr[j].bType, p->aAttr[j].sValue);
				iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,", attrtype%d=%d, attrvalue%d=%d",j, p->aAttr[j].bType, j, p->aAttr[j].sValue);
			}
		}

#ifdef ENABLE_YOHARA_SYSTEM
		if (isRandomAttr)
		{
			for (BYTE j = 0; j < APPLY_RANDOM_SLOT_MAX_NUM; ++j)
			{
				iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", randomtype%d, randomvalue%d, randomrow%d", j, j, j);
				iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen, ", %d, %d, %d", p->aApplyRandom[j].bType, p->aApplyRandom[j].sValue, p->aApplyRandom[j].row);
				iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen, ", randomtype%d=%d, randomvalue%d=%d, randomrow%d=%d", j, p->aApplyRandom[j].bType, j, p->aApplyRandom[j].sValue, j, p->aApplyRandom[j].row);
			}
		}
#endif

		char szItemQuery[QUERY_MAX_LEN + QUERY_MAX_LEN];
		snprintf(szItemQuery, sizeof(szItemQuery), "REPLACE INTO item%s (%s) VALUES(%s)", GetTablePostfix(), szColumns, szValues);

		if (g_test_server)
			sys_log(0, "ItemCache::Flush :REPLACE  (%s)", szItemQuery);

		CDBManager::instance().ReturnQuery(szItemQuery, QID_ITEM_SAVE, 0, NULL);

		//g_item_info.Add(p->vnum);
		++g_item_count;
	}

	m_bNeedQuery = false;
}

//
// CPlayerTableCache
//
CPlayerTableCache::CPlayerTableCache()
{
	m_expireTime = MIN(1800, g_iPlayerCacheFlushSeconds);
}

CPlayerTableCache::~CPlayerTableCache()
{
}

void CPlayerTableCache::OnFlush()
{
	if (g_test_server)
		sys_log(0, "PlayerTableCache::Flush : %s", m_data.name);

	char szQuery[QUERY_MAX_LEN];
	CreatePlayerSaveQuery(szQuery, sizeof(szQuery), &m_data);
	CDBManager::instance().ReturnQuery(szQuery, QID_PLAYER_SAVE, 0, NULL);
}
