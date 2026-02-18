#include "stdafx.h"
#include "constants.h"
#include "log.h"
#include "item.h"
#include "char.h"
#include "desc.h"
#include "item_manager.h"
#ifdef ENABLE_NEWSTUFF
#include "config.h"
#endif

#ifndef ENABLE_SWITCHBOT
const int MAX_NORM_ATTR_NUM = ITEM_MANAGER::MAX_NORM_ATTR_NUM;
const int MAX_RARE_ATTR_NUM = ITEM_MANAGER::MAX_RARE_ATTR_NUM;
#endif

int CItem::GetAttributeSetIndex()
{
#ifdef USE_CRYSTAL_SYSTEM
	if (GetType() == ITEM_CRYSTAL)
	{
		return -1;
	}
#endif

	if (GetType() == ITEM_WEAPON)
	{
		if (GetSubType() == WEAPON_ARROW)
			return -1;
		if (GetSubType() == WEAPON_QUIVER)
			return -1;
		return ATTRIBUTE_SET_WEAPON;
	}
#ifdef __AURA_SYSTEM__
	if (GetType() == ITEM_COSTUME && GetSubType() == COSTUME_AURA)
		return -1;
#endif

	if (GetType() == ITEM_BOOSTER)
	{
		return ATTRIBUTE_SET_BOOSTER;
	}

	if (GetType() == ITEM_ARMOR)
	{
		switch (GetSubType())
		{
			case ARMOR_BODY:
				return ATTRIBUTE_SET_BODY;

			case ARMOR_WRIST:
				return ATTRIBUTE_SET_WRIST;

			case ARMOR_FOOTS:
				return ATTRIBUTE_SET_FOOTS;

			case ARMOR_NECK:
				return ATTRIBUTE_SET_NECK;

			case ARMOR_HEAD:
				return ATTRIBUTE_SET_HEAD;

			case ARMOR_SHIELD:
				return ATTRIBUTE_SET_SHIELD;

			case ARMOR_EAR:
				return ATTRIBUTE_SET_EAR;

#ifdef ENABLE_YOHARA_SYSTEM
			case ARMOR_GLOVE:
				return ATTRIBUTE_SET_GLOVE;
#endif

			case ARMOR_PENDANT_SOUL:
				return ATTRIBUTE_SET_PENDANT_SOUL;

			
		}
	}

	else if(GetType() == ITEM_NEW_RINGS)
	{
		return ATTRIBUTE_SET_NEW_RINGS;
	}

	else if (GetType() == ITEM_COSTUME)
	{
		switch (GetSubType())
		{
			case COSTUME_BODY: // ÄÚ½ºÃõ °©¿ÊÀº ÀÏ¹Ý °©¿Ê°ú µ¿ÀÏÇÑ Attribute SetÀ» ÀÌ¿ëÇÏ¿© ·£´ý¼Ó¼º ºÙÀ½ (ARMOR_BODY == COSTUME_BODY)
#ifdef ADD_NEW_BONUS_INTO_COSTUME
				return ATTRIBUTE_SET_COSTUME_BODY;
#else
				return ATTRIBUTE_SET_BODY;
#endif

			case COSTUME_HAIR: // ÄÚ½ºÃõ Çì¾î´Â ÀÏ¹Ý Åõ±¸ ¾ÆÀÌÅÛ°ú µ¿ÀÏÇÑ Attribute SetÀ» ÀÌ¿ëÇÏ¿© ·£´ý¼Ó¼º ºÙÀ½ (ARMOR_HEAD == COSTUME_HAIR)
#ifdef ADD_NEW_BONUS_INTO_COSTUME
				return ATTRIBUTE_SET_COSTUME_HEAD;
#else
				return ATTRIBUTE_SET_HEAD;
#endif
				

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
			case COSTUME_MOUNT:
				break;
#endif

#ifdef ENABLE_MOUNT_SKIN
			case COSTUME_MOUNT_SKIN:
				break;
#endif

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
			case COSTUME_WEAPON:
	#ifdef ADD_NEW_BONUS_INTO_COSTUME
				return ATTRIBUTE_SET_COSTUME_WEAPON;
	#else
				return ATTRIBUTE_SET_WEAPON;
	#endif
#endif

		}
	}

	return -1;
}

bool CItem::HasAttr(BYTE bApply)
{
	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
		if (m_pProto->aApplies[i].bType == bApply)
			return true;

	for (int i = 0; i < MAX_NORM_ATTR_NUM; ++i)
		if (GetAttributeType(i) == bApply)
			return true;

	return false;
}

bool CItem::HasRareAttr(BYTE bApply)
{
	for (int i = 0; i < MAX_RARE_ATTR_NUM; ++i)
		if (GetAttributeType(i + 5) == bApply)
			return true;

	return false;
}

void CItem::AddAttribute(BYTE bApply, short sValue)
{
	if (HasAttr(bApply))
		return;

	int i = GetAttributeCount();

	if (i >= MAX_NORM_ATTR_NUM)
		sys_err("item attribute overflow!");
	else
	{
		if (sValue)
			SetAttribute(i, bApply, sValue);
	}
}

void CItem::AddAttr(BYTE bApply, BYTE bLevel)
{
	if (HasAttr(bApply))
		return;

	if (bLevel <= 0)
		return;
#ifdef ENABLE_LOCK_ATTR
	int i = GetEmptyAttrIndex();
#else
	int i = GetAttributeCount();
#endif
	if (i == MAX_NORM_ATTR_NUM)
		sys_err("item attribute overflow!");
	else
	{
		const TItemAttrTable & r = GetType() == ITEM_NEW_RINGS ? g_map_itemRingsAttr[bApply] : g_map_itemAttr[bApply];

		long lVal = r.lValues[MIN(4, bLevel - 1)];

		if (lVal)
			SetAttribute(i, bApply, lVal);
	}
}

void CItem::PutAttributeWithLevel(BYTE bLevel)
{
	int iAttributeSet = GetAttributeSetIndex();
	if (iAttributeSet < 0)
		return;

	if (bLevel > ITEM_ATTRIBUTE_MAX_LEVEL)
		return;

	std::vector<int> avail;

	int total = 0;

	// ºÙÀÏ ¼ö ÀÖ´Â ¼Ó¼º ¹è¿­À» ±¸Ãà
	for (int i = 0; i < MAX_APPLY_NUM; ++i)
	{

		const TItemAttrTable & r = GetType() == ITEM_NEW_RINGS ?  g_map_itemRingsAttr[i] : g_map_itemAttr[i];

		if (r.bMaxLevelBySet[iAttributeSet] && !HasAttr(i))
		{
			avail.push_back(i);
			total += r.dwProb;
		}
	}

	// ±¸ÃàµÈ ¹è¿­·Î È®·ü °è»êÀ» ÅëÇØ ºÙÀÏ ¼Ó¼º ¼±Á¤
	unsigned int prob = number(1, total);
	int attr_idx = APPLY_NONE;

	for (DWORD i = 0; i < avail.size(); ++i)
	{		

		const TItemAttrTable & r = GetType() == ITEM_NEW_RINGS ?  g_map_itemRingsAttr[avail[i]] : g_map_itemAttr[avail[i]];


		if (prob <= r.dwProb)
		{
			attr_idx = avail[i];
			break;
		}

		prob -= r.dwProb;
	}

	if (!attr_idx)
	{
		sys_err("Cannot put item attribute %d %d", iAttributeSet, bLevel);
		return;
	}

	const TItemAttrTable & r = GetType() == ITEM_NEW_RINGS ?  g_map_itemRingsAttr[attr_idx] : g_map_itemAttr[attr_idx];

	// Á¾·ùº° ¼Ó¼º ·¹º§ ÃÖ´ë°ª Á¦ÇÑ
	if (bLevel > r.bMaxLevelBySet[iAttributeSet])
		bLevel = r.bMaxLevelBySet[iAttributeSet];

	AddAttr(attr_idx, bLevel);
}

void CItem::PutAttribute(const int * aiAttrPercentTable)
{
	int iAttrLevelPercent = number(1, 100);
	int i;

	for (i = 0; i < ITEM_ATTRIBUTE_MAX_LEVEL; ++i)
	{
		if (iAttrLevelPercent <= aiAttrPercentTable[i])
			break;

		iAttrLevelPercent -= aiAttrPercentTable[i];
	}

	PutAttributeWithLevel(i + 1);
}

void CItem::ChangeAttribute(const int* aiChangeProb)
{
	int iAttributeCount = GetAttributeCount();
	if (iAttributeCount == 0)
		return;

#ifdef ENABLE_LOCK_ATTR
	std::map<BYTE, std::pair<BYTE, long>> m_stackBonus;
	for (BYTE j = 0; j < 2; ++j)
	{
		const BYTE lockAttrIndex = GetAttrLockIndex(j);
		if (lockAttrIndex != 0)
		{
			if (IsAddonItem() && lockAttrIndex <= 2)
			{
				m_stackBonus.emplace(0, std::make_pair(GetAttributeType(0), GetAttributeValue(0)));
				m_stackBonus.emplace(1, std::make_pair(GetAttributeType(1), GetAttributeValue(1)));
			}
			else
				m_stackBonus.emplace(lockAttrIndex-1, std::make_pair(GetAttributeType(lockAttrIndex-1), GetAttributeValue(lockAttrIndex-1)));
		}
	}
#endif

	ClearAttribute();

// #ifdef ENABLE_MULTI_CHEST
	SetUpdateStatus(true);
// #endif

	TItemTable const* pProto = GetProto();
	if (pProto && pProto->sAddonType)
		ApplyAddon(pProto->sAddonType);

#ifdef ENABLE_LOCK_ATTR
	for (itertype(m_stackBonus) it = m_stackBonus.begin(); it != m_stackBonus.end(); ++it)
		SetForceAttribute(it->first, it->second.first, it->second.second);
#endif

	static const int tmpChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
	{
		0, 10, 40, 35, 15,
	};

	for (int i = GetAttributeCount(); i < iAttributeCount; ++i)
	{
		if (aiChangeProb == NULL)
		{
			PutAttribute(tmpChangeProb);
		}
		else
		{
			PutAttribute(aiChangeProb);
		}
	}

// #ifdef ENABLE_MULTI_CHEST
	SetUpdateStatus(false);
	UpdatePacket();
// #endif

	if (IsPendantSoul() && m_pOwner)
		m_pOwner->ComputePoints();
}

void CItem::ChangeAttributeSpecial(const int* aiChangeProb)
{
	int iAttributeCount = GetAttributeCount();
	if (iAttributeCount == 0)
		return;

	std::map<BYTE, std::pair<BYTE, long>> m_stackBonus;
	for (BYTE j = 0; j < 2; ++j)
	{
		// const BYTE lockAttrIndex = 1;
		// if (lockAttrIndex != 0)
		// {
		if (IsAddonItem())
		{
			m_stackBonus.emplace(0, std::make_pair(GetAttributeType(0), GetAttributeValue(0)));
			m_stackBonus.emplace(1, std::make_pair(GetAttributeType(1), GetAttributeValue(1)));
		}
		// else
			// m_stackBonus.emplace(lockAttrIndex-1, std::make_pair(GetAttributeType(lockAttrIndex-1), GetAttributeValue(lockAttrIndex-1)));
		// }
	}

	ClearAttribute();

// #ifdef ENABLE_MULTI_CHEST
	SetUpdateStatus(true);
// #endif

	TItemTable const* pProto = GetProto();
	if (pProto && pProto->sAddonType)
		ApplyAddon(pProto->sAddonType);

	for (itertype(m_stackBonus) it = m_stackBonus.begin(); it != m_stackBonus.end(); ++it)
		SetForceAttribute(it->first, it->second.first, it->second.second);

	static const int tmpChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
	{
		0, 10, 40, 35, 15,
	};

	for (int i = GetAttributeCount(); i < iAttributeCount; ++i)
	{
		if (aiChangeProb == NULL)
		{
			PutAttribute(tmpChangeProb);
		}
		else
		{
			PutAttribute(aiChangeProb);
		}
	}

// #ifdef ENABLE_MULTI_CHEST
	SetUpdateStatus(false);
	UpdatePacket();
// #endif

	if (IsPendantSoul() && m_pOwner)
		m_pOwner->ComputePoints();
}

void CItem::AddAttribute()
{
	static const int aiItemAddAttributePercent[ITEM_ATTRIBUTE_MAX_LEVEL] =
	{
		40, 50, 10, 0, 0
	};

	if (GetAttributeCount() < MAX_NORM_ATTR_NUM)
		PutAttribute(aiItemAddAttributePercent);

	if (IsPendantSoul() && m_pOwner)
		m_pOwner->ComputePoints();
}

void CItem::ClearAttribute()
{
	for (int i = 0; i < MAX_NORM_ATTR_NUM; ++i)
	{
		m_aAttr[i].bType = 0;
		m_aAttr[i].sValue = 0;
	}
}

void CItem::ClearAttributeDragonSoul()
{

	for (int i = 3; i < ITEM_ATTRIBUTE_MAX_NUM; i++)
	{
		m_aAttr[i].bType = 0;
		m_aAttr[i].sValue = 0;
	}
}

int CItem::GetAttributeCount()
{
#ifdef ENABLE_LOCK_ATTR
	int total = 0;
	for (int i = 0; i < MAX_NORM_ATTR_NUM; ++i)
	{
		if (GetAttributeType(i) != 0)
			total += 1;
	}
	return total;
#else
	int i;
	for (i = 0; i < MAX_NORM_ATTR_NUM; ++i)
	{
		if (GetAttributeType(i) == 0)
			break;
	}
	return i;
#endif
}

int CItem::FindAttribute(BYTE bType)
{
	for (int i = 0; i < MAX_NORM_ATTR_NUM; ++i)
	{
		if (GetAttributeType(i) == bType)
			return i;
	}

	return -1;
}

bool CItem::RemoveAttributeAt(int index)
{
	if (GetAttributeCount() <= index)
		return false;

	for (int i = index; i < MAX_NORM_ATTR_NUM - 1; ++i)
	{
		SetAttribute(i, GetAttributeType(i + 1), GetAttributeValue(i + 1));
	}

	SetAttribute(MAX_NORM_ATTR_NUM - 1, APPLY_NONE, 0);
	return true;
}

bool CItem::RemoveAttributeType(BYTE bType)
{
	int index = FindAttribute(bType);
	return index != -1 && RemoveAttributeType(index);
}

void CItem::SetAttributes(const TPlayerItemAttribute* c_pAttribute)
{
	std::memcpy(m_aAttr, c_pAttribute, sizeof(m_aAttr));
	Save();
}

void CItem::SetAttribute(int i, BYTE bType, short sValue)
{
	assert(i < MAX_NORM_ATTR_NUM);

	m_aAttr[i].bType = bType;
	m_aAttr[i].sValue = sValue;
	UpdatePacket();
	Save();

	if (bType)
	{
		const char * pszIP = NULL;

		if (GetOwner() && GetOwner()->GetDesc())
			pszIP = GetOwner()->GetDesc()->GetHostName();

		LOG_LEVEL_CHECK(LOG_LEVEL_MAX, LogManager::instance().ItemLog(i, bType, sValue, GetID(), "SET_ATTR", "", pszIP ? pszIP : "", GetOriginalVnum()));
	}
}

void CItem::SetForceAttribute(int i, BYTE bType, short sValue)
{
	assert(i < ITEM_ATTRIBUTE_MAX_NUM);

	m_aAttr[i].bType = bType;
	m_aAttr[i].sValue = sValue;
	UpdatePacket();
	Save();

	if (bType)
	{
		const char * pszIP = NULL;

		if (GetOwner() && GetOwner()->GetDesc())
			pszIP = GetOwner()->GetDesc()->GetHostName();

		LOG_LEVEL_CHECK(LOG_LEVEL_MAX, LogManager::instance().ItemLog(i, bType, sValue, GetID(), "SET_FORCE_ATTR", "", pszIP ? pszIP : "", GetOriginalVnum()));
	}
}


void CItem::CopyAttributeTo(LPITEM pItem)
{
	pItem->SetAttributes(m_aAttr);
}

int CItem::GetRareAttrCount()
{
	int ret = 0;

	for (DWORD dwIdx = ITEM_ATTRIBUTE_RARE_START; dwIdx < ITEM_ATTRIBUTE_RARE_END; dwIdx++)
	{
		if (m_aAttr[dwIdx].bType != 0)
			ret++;
	}

	return ret;
}

bool CItem::ChangeRareAttribute()
{
	if (GetRareAttrCount() == 0)
		return false;

	int cnt = GetRareAttrCount();

	for (int i = 0; i < cnt; ++i)
	{
		m_aAttr[i + ITEM_ATTRIBUTE_RARE_START].bType = 0;
		m_aAttr[i + ITEM_ATTRIBUTE_RARE_START].sValue = 0;
	}

	if (GetOwner() && GetOwner()->GetDesc())
		LOG_LEVEL_CHECK(LOG_LEVEL_MAX, LogManager::instance().ItemLog(GetOwner(), this, "SET_RARE_CHANGE", ""))
	else
		LOG_LEVEL_CHECK(LOG_LEVEL_MAX, LogManager::instance().ItemLog(0, 0, 0, GetID(), "SET_RARE_CHANGE", "", "", GetOriginalVnum()))

	for (int i = 0; i < cnt; ++i)
	{
		AddRareAttribute();
	}

	return true;
}

bool CItem::ChangeRareAttribute2()
{
	if (GetRareAttrCount() == 0)
		return false;

	int cnt = GetRareAttrCount();

	for (int i = 0; i < cnt; ++i)
	{
		m_aAttr[i + ITEM_ATTRIBUTE_RARE_START].bType = 0;
		m_aAttr[i + ITEM_ATTRIBUTE_RARE_START].sValue = 0;
	}

	for (int i = 0; i < cnt; ++i)
	{
		AddRareAttribute2();
	}

	return true;
}

bool CItem::AddRareAttribute()
{
	int count = GetRareAttrCount();

	if (count >= ITEM_ATTRIBUTE_RARE_NUM)
		return false;

	int pos = count + ITEM_ATTRIBUTE_RARE_START;
	TPlayerItemAttribute & attr = m_aAttr[pos];

	int nAttrSet = GetAttributeSetIndex();
	std::vector<int> avail;

	for (int i = 0; i < MAX_APPLY_NUM; ++i)
	{

		const TItemAttrTable & r = GetType() == ITEM_NEW_RINGS ?  g_map_itemRingsRare[i] : g_map_itemRare[i];
		if (r.dwApplyIndex != 0 && r.bMaxLevelBySet[nAttrSet] > 0 && HasRareAttr(i) != true)
		{
			avail.push_back(i);
		}
	}

	if(avail.size() <= 1)
		return false;

	const TItemAttrTable & r = GetType() == ITEM_NEW_RINGS ?  g_map_itemRingsRare[avail[number(0, avail.size() - 1)]] : g_map_itemRare[avail[number(0, avail.size() - 1)]];
	int nAttrLevel = 5;

	if (nAttrLevel > r.bMaxLevelBySet[nAttrSet])
		nAttrLevel = r.bMaxLevelBySet[nAttrSet];

	attr.bType = r.dwApplyIndex;
	attr.sValue = r.lValues[nAttrLevel - 1];

	UpdatePacket();

	Save();

	const char * pszIP = NULL;

	if (GetOwner() && GetOwner()->GetDesc())
		pszIP = GetOwner()->GetDesc()->GetHostName();

	LOG_LEVEL_CHECK(LOG_LEVEL_MAX, LogManager::instance().ItemLog(pos, attr.bType, attr.sValue, GetID(), "SET_RARE", "", pszIP ? pszIP : "", GetOriginalVnum()));
	return true;
}

void CItem::AddRareAttribute2(const int * aiAttrPercentTable)
{
	static const int aiItemAddAttributePercent[ITEM_ATTRIBUTE_MAX_LEVEL] =
	{
		40, 50, 10, 0, 0
	};
	if (aiAttrPercentTable == NULL)
		aiAttrPercentTable = aiItemAddAttributePercent;

	if (GetRareAttrCount() < MAX_RARE_ATTR_NUM)
		PutRareAttribute(aiAttrPercentTable);
}

void CItem::PutRareAttribute(const int * aiAttrPercentTable)
{
	int iAttrLevelPercent = number(1, 100);
	int i;

	for (i = 0; i < ITEM_ATTRIBUTE_MAX_LEVEL; ++i)
	{
		if (iAttrLevelPercent <= aiAttrPercentTable[i])
			break;

		iAttrLevelPercent -= aiAttrPercentTable[i];
	}

	PutRareAttributeWithLevel(i + 1);
}

void CItem::PutRareAttributeWithLevel(BYTE bLevel)
{
	int iAttributeSet = GetAttributeSetIndex();
	if (iAttributeSet < 0)
		return;

	if (bLevel > ITEM_ATTRIBUTE_MAX_LEVEL)
		return;

	std::vector<int> avail;

	int total = 0;

	// ºÙÀÏ ¼ö ÀÖ´Â ¼Ó¼º ¹è¿­À» ±¸Ãà
	for (int i = 0; i < MAX_APPLY_NUM; ++i)
	{
		const TItemAttrTable & r = GetType() == ITEM_NEW_RINGS ?  g_map_itemRingsRare[i] : g_map_itemRare[i];

		if (r.bMaxLevelBySet[iAttributeSet] && !HasRareAttr(i))
		{
			avail.push_back(i);
			total += r.dwProb;
		}
	}

	// ±¸ÃàµÈ ¹è¿­·Î È®·ü °è»êÀ» ÅëÇØ ºÙÀÏ ¼Ó¼º ¼±Á¤
	unsigned int prob = number(1, total);
	int attr_idx = APPLY_NONE;

	for (DWORD i = 0; i < avail.size(); ++i)
	{
		const TItemAttrTable & r = GetType() == ITEM_NEW_RINGS ?  g_map_itemRingsRare[avail[i]] : g_map_itemRare[avail[i]];

		if (prob <= r.dwProb)
		{
			attr_idx = avail[i];
			break;
		}

		prob -= r.dwProb;
	}

	if (!attr_idx)
	{
		sys_err("Cannot put item rare attribute %d %d", iAttributeSet, bLevel);
		return;
	}

	const TItemAttrTable & r = GetType() == ITEM_NEW_RINGS ?  g_map_itemRingsRare[attr_idx] : g_map_itemRare[attr_idx];

	// Á¾·ùº° ¼Ó¼º ·¹º§ ÃÖ´ë°ª Á¦ÇÑ
	if (bLevel > r.bMaxLevelBySet[iAttributeSet])
		bLevel = r.bMaxLevelBySet[iAttributeSet];

	AddRareAttr(attr_idx, bLevel);
}

void CItem::AddRareAttr(BYTE bApply, BYTE bLevel)
{
	if (HasRareAttr(bApply))
		return;

	if (bLevel <= 0)
		return;

	int i = ITEM_ATTRIBUTE_RARE_START + GetRareAttrCount();

	if (i == ITEM_ATTRIBUTE_RARE_END)
		sys_err("item rare attribute overflow!");
	else
	{
		const TItemAttrTable & r = GetType() == ITEM_NEW_RINGS ?  g_map_itemRingsRare[bApply] : g_map_itemRare[bApply];

		long lVal = r.lValues[MIN(4, bLevel - 1)];

		if (lVal)
			SetForceAttribute(i, bApply, lVal);
	}
}

void CItem::AddAttrCostumeNew(BYTE bApply, BYTE bLevel)
{
	if (HasAttr(bApply))
		return;

	if (bLevel <= 0)
		return;

	int i = GetAttributeCount();

	if (i == MAX_NORM_ATTR_NUM)
		sys_err("item attribute overflow!");
	else
	{

		TItemAttrTable & r = g_map_itemCostume[bApply];
		
		long lVal = r.lValues[MIN(4, bLevel - 1)];

		if (lVal)
			SetAttribute(i, bApply, lVal);
	}
}

void CItem::PutAttributeWithLevelCostume(BYTE bLevel)
{
	int iAttributeSet = GetAttributeSetIndex();

	if (iAttributeSet < 0)
		return;

	if (bLevel > ITEM_ATTRIBUTE_MAX_LEVEL)
		return;

	std::vector<int> avail;

	int total = 0;

	// ºÙÀÏ ¼ö ÀÖ´Â ¼Ó¼º ¹è¿­À» ±¸Ãà
	for (int i = 0; i < MAX_APPLY_NUM; ++i)
	{
		TItemAttrTable & r = g_map_itemCostume[i];
		if (r.bMaxLevelBySet[iAttributeSet] && !HasAttr(i))
		{
			avail.push_back(i);
			total += r.dwProb;
		}
	}

	// ±¸ÃàµÈ ¹è¿­·Î È®·ü °è»êÀ» ÅëÇØ ºÙÀÏ ¼Ó¼º ¼±Á¤
	unsigned int prob = number(1, total);
	int attr_idx = APPLY_NONE;

	for (DWORD i = 0; i < avail.size(); ++i)
	{

		TItemAttrTable & r = g_map_itemCostume[avail[i]];

		if (prob <= r.dwProb)
		{
			attr_idx = avail[i];
			break;
		}

		prob -= r.dwProb;
	}

	if (!attr_idx)
	{
		sys_err("Cannot put item attribute %d %d", iAttributeSet, bLevel);
		return;
	}

	TItemAttrTable & r = g_map_itemCostume[attr_idx];
	// Á¾·ùº° ¼Ó¼º ·¹º§ ÃÖ´ë°ª Á¦ÇÑ
	if (bLevel > r.bMaxLevelBySet[iAttributeSet])
		bLevel = r.bMaxLevelBySet[iAttributeSet];

	AddAttrCostumeNew(attr_idx, bLevel);
}

void CItem::PutAttributeCostume(const int * aiAttrPercentTable)
{
	int iAttrLevelPercent = number(1, 100);
	int i;

	for (i = 0; i < ITEM_ATTRIBUTE_MAX_LEVEL; ++i)
	{
		if (iAttrLevelPercent <= aiAttrPercentTable[i])
			break;

		iAttrLevelPercent -= aiAttrPercentTable[i];
	}

	PutAttributeWithLevelCostume(i + 1);
}

void CItem::ChangeAttrCostume()
{
	int iAttributeCount = GetAttributeCount();
	ClearAttribute();
	if (iAttributeCount == 0)
		return;

	TItemTable const * pProto = GetProto();

	if (pProto && pProto->sAddonType)
	{
		ApplyAddon(pProto->sAddonType);
	}

	static const int tmpChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
	{
		0, 10, 40, 35, 15,
	};

	for (int i = GetAttributeCount(); i < iAttributeCount; ++i)
	{

		PutAttributeCostume(tmpChangeProb);
	}
}

void CItem::AddAttrCostume(){
	static const int aiItemAddAttributePercent[ITEM_ATTRIBUTE_MAX_LEVEL] = 
	{
		40, 50, 10, 0, 0
	};

	if (GetAttributeCount() < MAX_NORM_ATTR_NUM)
		PutAttributeCostume(aiItemAddAttributePercent);
}

void CItem::AddAttrCostumeHairNew(BYTE bApply, BYTE bLevel)
{
	if (HasAttr(bApply))
		return;

	if (bLevel <= 0)
		return;

	int i = GetAttributeCount();

	if (i == MAX_NORM_ATTR_NUM)
		sys_err("item attribute overflow!");
	else
	{

		TItemAttrTable & r = g_map_itemCostumeHair[bApply];
		
		long lVal = r.lValues[MIN(4, bLevel - 1)];

		if (lVal)
			SetAttribute(i, bApply, lVal);
	}
}

void CItem::PutAttributeWithLevelCostumeHair(BYTE bLevel)
{
	int iAttributeSet = GetAttributeSetIndex();

	if (iAttributeSet < 0)
		return;

	if (bLevel > ITEM_ATTRIBUTE_MAX_LEVEL)
		return;

	std::vector<int> avail;

	int total = 0;

	// ºÙÀÏ ¼ö ÀÖ´Â ¼Ó¼º ¹è¿­À» ±¸Ãà
	for (int i = 0; i < MAX_APPLY_NUM; ++i)
	{
		TItemAttrTable & r = g_map_itemCostumeHair[i];
		if (r.bMaxLevelBySet[iAttributeSet] && !HasAttr(i))
		{
			avail.push_back(i);
			total += r.dwProb;
		}
	}

	// ±¸ÃàµÈ ¹è¿­·Î È®·ü °è»êÀ» ÅëÇØ ºÙÀÏ ¼Ó¼º ¼±Á¤
	unsigned int prob = number(1, total);
	int attr_idx = APPLY_NONE;

	for (DWORD i = 0; i < avail.size(); ++i)
	{

		TItemAttrTable & r = g_map_itemCostumeHair[avail[i]];

		if (prob <= r.dwProb)
		{
			attr_idx = avail[i];
			break;
		}

		prob -= r.dwProb;
	}

	if (!attr_idx)
	{
		sys_err("Cannot put item attribute %d %d", iAttributeSet, bLevel);
		return;
	}

	TItemAttrTable & r = g_map_itemCostumeHair[attr_idx];
	// Á¾·ùº° ¼Ó¼º ·¹º§ ÃÖ´ë°ª Á¦ÇÑ
	if (bLevel > r.bMaxLevelBySet[iAttributeSet])
		bLevel = r.bMaxLevelBySet[iAttributeSet];

	AddAttrCostumeHairNew(attr_idx, bLevel);
}

void CItem::PutAttributeCostumeHair(const int * aiAttrPercentTable)
{
	int iAttrLevelPercent = number(1, 100);
	int i;

	for (i = 0; i < ITEM_ATTRIBUTE_MAX_LEVEL; ++i)
	{
		if (iAttrLevelPercent <= aiAttrPercentTable[i])
			break;

		iAttrLevelPercent -= aiAttrPercentTable[i];
	}

	PutAttributeWithLevelCostumeHair(i + 1);
}

void CItem::ChangeAttrCostumeHair()
{
	int iAttributeCount = GetAttributeCount();
	ClearAttribute();
	if (iAttributeCount == 0)
		return;

	TItemTable const * pProto = GetProto();

	if (pProto && pProto->sAddonType)
	{
		ApplyAddon(pProto->sAddonType);
	}

	static const int tmpChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
	{
		0, 10, 40, 35, 15,
	};

	for (int i = GetAttributeCount(); i < iAttributeCount; ++i)
	{

		PutAttributeCostumeHair(tmpChangeProb);
	}
}

void CItem::AddAttrCostumeHair(){
	static const int aiItemAddAttributePercent[ITEM_ATTRIBUTE_MAX_LEVEL] = 
	{
		40, 50, 10, 0, 0
	};

	if (GetAttributeCount() < MAX_NORM_ATTR_NUM)
		PutAttributeCostumeHair(aiItemAddAttributePercent);
}
#ifdef ENABLE_LOCK_ATTR
int CItem::GetEmptyAttrIndex()
{
	for(int i=0;i<MAX_NORM_ATTR_NUM;++i)
	{
		if(GetAttributeType(i) == APPLY_NONE)
			return i;
	}
	return MAX_NORM_ATTR_NUM;
}
BYTE CItem::GetAttrLockIndex(BYTE index)
{
	return GetAttributeValue(LOCK_ATTR_DATA_INDEX+index);
}
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD

int CItem::ChangeAttributeSwtichBot(int count,const TPlayerItemAttribute* aAttr, bool* check_add)
{
	static const int tmpChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
	{
		0, 10, 40, 35, 15,
	};

	int max_count = 0;

	for (int a = 0; a < count; ++a)
	{
		int iAttributeCount = GetAttributeCount();
		ClearAttribute();
		if (iAttributeCount == 0)
			return 0;

		TItemTable const * pProto = GetProto();

		if (pProto && pProto->sAddonType)
		{
			ApplyAddon(pProto->sAddonType,true);
		}


		for (int b = GetAttributeCount(); b < iAttributeCount; ++b)
		{
			int iAttrLevelPercent = number(1, 100);
			int c;

			for (c = 0; c < ITEM_ATTRIBUTE_MAX_LEVEL; ++c)
			{
				if (iAttrLevelPercent <= tmpChangeProb[c])
					break;

				iAttrLevelPercent -= tmpChangeProb[c];
			}

			int iAttributeSet = GetAttributeSetIndex();

			if (iAttributeSet < 0)
				return 0;

			BYTE bLevel = c + 1;
			if (bLevel > ITEM_ATTRIBUTE_MAX_LEVEL)
				return 0;

			std::vector<int> avail;

			int total = 0;

			for (int d = 0; d < MAX_APPLY_NUM; ++d)
			{
				const TItemAttrTable & r = g_map_itemAttr[d];

				if (r.bMaxLevelBySet[iAttributeSet] && !HasAttr(d))
				{
					avail.push_back(d);
					total += r.dwProb;
				}
			}

			unsigned int prob = number(1, total);
			int attr_idx = APPLY_NONE;

			for (DWORD e = 0; e < avail.size(); ++e)
			{
				const TItemAttrTable & r = g_map_itemAttr[avail[e]];

				if (prob <= r.dwProb)
				{
					attr_idx = avail[e];
					break;
				}

				prob -= r.dwProb;
			}

			if (!attr_idx)
			{
				sys_err("Cannot put item attribute %d %d", iAttributeSet, bLevel);
				return 0;
			}

			const TItemAttrTable & r = g_map_itemAttr[attr_idx];

			if (bLevel > r.bMaxLevelBySet[iAttributeSet])
				bLevel = r.bMaxLevelBySet[iAttributeSet];

			if (HasAttr(attr_idx))
				return 0;

			if (bLevel <= 0)
				return 0;

			int i = GetAttributeCount();

			if (i == MAX_NORM_ATTR_NUM)
				sys_err("item attribute overflow!");
			else
			{
				const TItemAttrTable & r = g_map_itemAttr[attr_idx];
				long lVal = r.lValues[MIN(4, bLevel - 1)];

				if (lVal)
					SetAttributeSwtichBot(i, attr_idx, lVal);
			}
		}

		max_count++;

		if (CheckBonusSwitchBot(aAttr))
		{
			*check_add = true;
			return max_count;
		}
	}
	*check_add = false;
	return max_count;
}

int CItem::GetCountSelect(const TPlayerItemAttribute* aAttr)
{
	int count = 0;
	for (int i = 0; i < MAX_NORM_ATTR_NUM; ++i)
	{
		if(aAttr[i].bType != 0)
		{
			count ++;
		}
	}

	return count;
}

bool CItem::CheckBonusSwitchBot(const TPlayerItemAttribute* aAttr)
{	

	int iAttributeCount = GetAttributeCount();

	int max_count_select = GetCountSelect(aAttr);

	int count_total = 0;

	for (int i = 0; i < iAttributeCount; ++i)
	{
		for (int a = 0; a < MAX_NORM_ATTR_NUM; ++a)
		{
			if(aAttr[a].bType != 0)
			{
				if(aAttr[a].bType == GetAttributeType(i) && GetAttributeValue(i) >= aAttr[a].sValue)
				{
					count_total ++;
				}
			}
		}
		
	}

	if (count_total == max_count_select){
		return true;
	}

	return false;

}

void CItem::SetAttributeSwtichBot(int i, BYTE bType, short sValue)
{
	assert(i < MAX_NORM_ATTR_NUM);

	m_aAttr[i].bType = bType;
	m_aAttr[i].sValue = sValue;
}


void CItem::AddAttributeSwitchBot(BYTE bApply, short sValue)
{

	if (HasAttr(bApply))
		return;

	int i = GetAttributeCount();

	if (i >= MAX_NORM_ATTR_NUM)
		sys_err("item attribute overflow!");
	else
	{
		if (sValue)
			SetAttributeSwtichBot(i, bApply, sValue);
	}
}

#endif

#ifdef ENABLE_YOHARA_SYSTEM
// Random Bonus
// Defaultfunctions
int CItem::GetRandomAttrCount()
{
	int ret = 0;

	for (int dwIdx = 0; dwIdx < APPLY_RANDOM_SLOT_MAX_NUM; dwIdx++)
		if (m_aApplyRandom[dwIdx].bType != 0)
			ret++;

	return ret;
}

void CItem::ClearRandomAttribute()
{
	for (int i = 0; i < APPLY_RANDOM_SLOT_MAX_NUM; ++i) {
		m_aApplyRandom[i].bType = 0;
		m_aApplyRandom[i].sValue = 0;
		m_aApplyRandom[i].row = 0;
	}
}

void CItem::SetRandomAttrs(const TPlayerItemApplyRandom* c_pAttribute)
{
	std::memcpy(m_aApplyRandom, c_pAttribute, sizeof(m_aApplyRandom));
	Save();
}

void CItem::CopyApplyRandomTo(LPITEM pItem)
{
	pItem->SetRandomAttrs(m_aApplyRandom);
}





////////////////////////////////////////////////////////////////////
// Refine
void CItem::SetItemApplyRandom(int i, BYTE wType, long sValue, BYTE bRow)	//@fixme532
{
	if (wType >= MAX_APPLY_NUM)
		return;

	assert(i < APPLY_RANDOM_SLOT_MAX_NUM);

	m_aApplyRandom[i].bType = wType;
	m_aApplyRandom[i].sValue = sValue;
	m_aApplyRandom[i].row = bRow;
	UpdatePacket();
	Save();

	if (wType)
	{
		const char* pszIP = nullptr;

		if (GetOwner() && GetOwner()->GetDesc())
			pszIP = GetOwner()->GetDesc()->GetHostName();

		LOG_LEVEL_CHECK(LOG_LEVEL_MAX, LogManager::Instance().ItemLog(i, wType, sValue, GetID(), "SET_SUNGMA_ATTR", "", pszIP ? pszIP : "", GetOriginalVnum()));
	}
}

void CItem::PutRandomAttributeWithLevel(BYTE bLevel)	//@fixme532
{
	int iAttributeSet = GetAttributeSetIndex();
	if (iAttributeSet < 0)
		return;

	if (bLevel > APPLY_RANDOM_SLOT_MAX_NUM)
		return;

	std::vector<int> avail;

	int total = 0;

	for (int i = 0; i < MAX_APPLY_NUM; ++i)
	{
		const TItemAttrTable& r = g_map_itemAttr[i];

		if (r.bMaxLevelBySet[iAttributeSet] && !HasRandomAttr(i))
		{
			avail.emplace_back(i);
			total += r.dwProb;
		}
	}

	total = (total == 0 ? 1 : total);
	unsigned int prob = number(1, total);
	int attr_idx = APPLY_NONE;

	for (int i = 0; i < avail.size(); ++i)
	{
		const TItemAttrTable& r = g_map_itemAttr[avail[i]];

		if (prob <= r.dwProb)
		{
			attr_idx = avail[i];
			break;
		}

		prob -= r.dwProb;
	}

	if (!attr_idx)
	{
		sys_err("Cannot put item random attribute %d %d", iAttributeSet, bLevel);
		return;
	}

	const TItemAttrTable& r = g_map_itemAttr[attr_idx];

	if (bLevel > r.bMaxLevelBySet[iAttributeSet])
		bLevel = r.bMaxLevelBySet[iAttributeSet];

	AddRandomAttr(attr_idx, bLevel);
}

void CItem::AddRandomAttr(BYTE wApply, BYTE bLevel)	//@fixme532
{
	if (HasRandomAttr(wApply))
		return;

	if (bLevel <= 0)
		return;

	int i = GetRandomAttrCount();

	if (i == APPLY_RANDOM_SLOT_MAX_NUM)
		sys_err("item random attribute overflow!");
	else
	{
		const TItemAttrTable& r = g_map_itemAttr[wApply];
		long lVal = r.lValues[MIN(4, bLevel - 1)];

		if (lVal)
			SetItemApplyRandom(i, wApply, lVal, 0);
	}
}

bool CItem::HasRandomAttr(BYTE wApply)	//@fixme532
{
	for (int i = 0; i < APPLY_RANDOM_SLOT_MAX_NUM; ++i)
		if (GetApplyRandomType(i) == wApply)
			return true;

	return false;
}
#endif
