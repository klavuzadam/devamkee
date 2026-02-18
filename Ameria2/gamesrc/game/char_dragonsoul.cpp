#include "stdafx.h"
#include "char.h"
#include "item.h"
#include "desc.h"
#include "DragonSoul.h"
#include "log.h"

// ¿ëÈ¥¼® ÃÊ±âÈ­
// ¿ëÈ¥¼® on/off´Â Affect·Î ÀúÀåµÇ±â ¶§¹®¿¡,
// ¿ëÈ¥¼® Affect°¡ ÀÖ´Ù¸é µ¦¿¡ ÀÖ´Â ¿ëÈ¥¼®À» activateÇØ¾ßÇÑ´Ù.
// ¶ÇÇÑ ¿ëÈ¥¼® »ç¿ë ÀÚ°ÝÀº QuestFlag·Î ÀúÀåÇØ ³õ¾Ò±â ¶§¹®¿¡,
// Äù½ºÆ® Flag¿¡¼­ ¿ëÈ¥¼® »ç¿ë ÀÚ°ÝÀ» ÀÐ¾î¿Â´Ù.

// Ä³¸¯ÅÍÀÇ affect, quest°¡ load µÇ±â Àü¿¡ DragonSoul_Initialize¸¦ È£ÃâÇÏ¸é ¾ÈµÈ´Ù.
// affect°¡ °¡Àå ¸¶Áö¸·¿¡ ·ÎµåµÇ¾î LoadAffect¿¡¼­ È£ÃâÇÔ.
void CHARACTER::DragonSoul_Initialize()
{
	for (WORD i = INVENTORY_MAX_NUM + WEAR_MAX_NUM; i < DRAGON_SOUL_EQUIP_SLOT_END; i++)
	{
		LPITEM pItem = GetItem(TItemPos(INVENTORY, i));
		if (NULL != pItem)
			pItem->SetSocket(ITEM_SOCKET_DRAGON_SOUL_ACTIVE_IDX, 0);
	}

	if (FindAffect(AFFECT_DRAGON_SOUL_DECK_0))
	{
		DragonSoul_ActivateDeck(DRAGON_SOUL_DECK_0);
	}
	else if (FindAffect(AFFECT_DRAGON_SOUL_DECK_1))
	{
		DragonSoul_ActivateDeck(DRAGON_SOUL_DECK_1);
	}
}

int CHARACTER::DragonSoul_GetActiveDeck() const
{
	return m_pointsInstant.iDragonSoulActiveDeck;
}

bool CHARACTER::DragonSoul_IsDeckActivated() const
{
	return m_pointsInstant.iDragonSoulActiveDeck >= 0;
}

bool CHARACTER::DragonSoul_IsQualified() const
{
	return FindAffect(AFFECT_DRAGON_SOUL_QUALIFIED) != NULL;
}

void CHARACTER::DragonSoul_GiveQualification()
{
	if(NULL == FindAffect(AFFECT_DRAGON_SOUL_QUALIFIED))
	{
		LogManager::instance().CharLog(this, 0, "DS_QUALIFIED", "");
	}
	AddAffect(AFFECT_DRAGON_SOUL_QUALIFIED, APPLY_NONE, 0, AFF_NONE, INFINITE_AFFECT_DURATION, 0, false, false);
	//SetQuestFlag("dragon_soul.is_qualified", 1);
	//// ÀÚ°ÝÀÖ´Ù¸é POINT_DRAGON_SOUL_IS_QUALIFIED´Â ¹«Á¶°Ç 1
	//PointChange(POINT_DRAGON_SOUL_IS_QUALIFIED, 1 - GetPoint(POINT_DRAGON_SOUL_IS_QUALIFIED));
}

bool CHARACTER::DragonSoul_ActivateDeck(int deck_idx)
{
	if (deck_idx < DRAGON_SOUL_DECK_0 || deck_idx >= DRAGON_SOUL_DECK_MAX_NUM)
	{
		return false;
	}

	if (DragonSoul_GetActiveDeck() == deck_idx)
		return true;

	DragonSoul_DeactivateAll();

	if (!DragonSoul_IsQualified())
	{
		ChatPacket(CHAT_TYPE_INFO, "105");
		return false;
	}

	AddAffect(AFFECT_DRAGON_SOUL_DECK_0 + deck_idx, APPLY_NONE, 0, 0, INFINITE_AFFECT_DURATION, 0, false);

	m_pointsInstant.iDragonSoulActiveDeck = deck_idx;

#ifdef ENABLE_DS_SET
	std::vector<int> gradeList, stepList, strengthList;
	bool bExpired = false;
	for (int i = 0; i < DS_SLOT_MAX; i++) {
		gradeList.push_back(0);
		stepList.push_back(0);
		strengthList.push_back(0);
	}

	int j = 0;
#endif

	for (int i = DRAGON_SOUL_EQUIP_SLOT_START + DS_SLOT_MAX * deck_idx; i < DRAGON_SOUL_EQUIP_SLOT_START + DS_SLOT_MAX * (deck_idx + 1); i++)
	{
		LPITEM pItem = GetInventoryItem(i);
		if (NULL != pItem) {
			DSManager::instance().ActivateDragonSoul(pItem);
#ifdef ENABLE_DS_SET
			if (!DSManager::instance().IsTimeLeftDragonSoul(pItem))
				bExpired = true;

			gradeList[j] = (pItem->GetVnum() / 1000) % 10;
			stepList[j] = (pItem->GetVnum() / 100) % 10;
			strengthList[j] = (pItem->GetVnum() / 10) % 10;
			j += 1;
#endif
		}
	}

#ifdef ENABLE_DS_SET
	if ((!bExpired) && (gradeList[0] ==
#ifdef ENABLE_DS_GRADE_MYTH
		DRAGON_SOUL_GRADE_MYTH
#else
		DRAGON_SOUL_GRADE_LEGENDARY
#endif
		) && (stepList[0] == DRAGON_SOUL_STEP_HIGHEST) && (strengthList[0] == 6)) {
		if ((std::equal(gradeList.begin() + 1, gradeList.end(), gradeList.begin())) && (std::equal(stepList.begin() + 1, stepList.end(), stepList.begin())) && (std::equal(strengthList.begin() + 1, strengthList.end(), strengthList.begin()))) {

			AddAffect(AFFECT_DS_SET, POINT_NONE, 1, 0, INFINITE_AFFECT_DURATION, 0, false);
			AddAffect(AFFECT_DS_BNS1, POINT_MAX_HP, 1000, 0, INFINITE_AFFECT_DURATION, 0, false);
			AddAffect(AFFECT_DS_BNS2, POINT_ATTBONUS_STONE, 8, 0, INFINITE_AFFECT_DURATION, 0, false);
			AddAffect(AFFECT_DS_BNS3, POINT_ATTBONUS_BOSS, 8, 0, INFINITE_AFFECT_DURATION, 0, false);
		}
		else {
			AddAffect(AFFECT_DS_SET, POINT_NONE, 0, 0, INFINITE_AFFECT_DURATION, 0, false);
			RemoveAffect(AFFECT_DS_BNS1);
			RemoveAffect(AFFECT_DS_BNS2);
			RemoveAffect(AFFECT_DS_BNS3);
		}
	}
	else {
		AddAffect(AFFECT_DS_SET, POINT_NONE, 0, 0, INFINITE_AFFECT_DURATION, 0, false);
		RemoveAffect(AFFECT_DS_BNS1);
		RemoveAffect(AFFECT_DS_BNS2);
		RemoveAffect(AFFECT_DS_BNS3);
	}
#endif

	return true;
}

void CHARACTER::DragonSoul_DeactivateAll()
{
	for (WORD i = DRAGON_SOUL_EQUIP_SLOT_START; i < DRAGON_SOUL_EQUIP_SLOT_END; i++)
	{
		DSManager::instance().DeactivateDragonSoul(GetInventoryItem(i), true);
	}
	m_pointsInstant.iDragonSoulActiveDeck = -1;
	RemoveAffect(AFFECT_DRAGON_SOUL_DECK_0);
	RemoveAffect(AFFECT_DRAGON_SOUL_DECK_1);
#ifdef ENABLE_DS_SET
	RemoveAffect(AFFECT_DS_SET);
	RemoveAffect(AFFECT_DS_BNS1);
	RemoveAffect(AFFECT_DS_BNS2);
	RemoveAffect(AFFECT_DS_BNS3);
#endif
}

void CHARACTER::DragonSoul_CleanUp()
{
	for (WORD i = DRAGON_SOUL_EQUIP_SLOT_START; i < DRAGON_SOUL_EQUIP_SLOT_END; i++)
	{
		DSManager::instance().DeactivateDragonSoul(GetInventoryItem(i), true);
	}
}

bool CHARACTER::DragonSoul_RefineWindow_Open(LPENTITY pEntity)
{
	if (m_playerData && !m_playerData->m_pDragonSoulRefineWindowOpener)
	{
		m_playerData->m_pDragonSoulRefineWindowOpener = pEntity;
	}

	LPDESC d = GetDesc();
	if (!d)
	{
		sys_err("User(%s)'s DESC is NULL POINT.", GetName());
		return false;
	}

	TPacketGCDragonSoulRefine p;
	p.header = HEADER_GC_DRAGON_SOUL_REFINE;
	p.bSubType = DS_SUB_HEADER_OPEN;

	d->Packet(&p, sizeof(p));
	return true;
}

bool CHARACTER::DragonSoul_RefineWindow_Close()
{
	if (m_playerData)
	{
		m_playerData->m_pDragonSoulRefineWindowOpener = NULL;
	}

	return true;
}

bool CHARACTER::DragonSoul_RefineWindow_CanRefine()
{
	return (m_playerData && m_playerData->m_pDragonSoulRefineWindowOpener) ? true : false;
}
