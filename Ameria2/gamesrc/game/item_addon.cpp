#include "stdafx.h"
#include "constants.h"
#include "utils.h"
#include "item.h"
#include "item_addon.h"
#ifdef ENABLE_REWARD_SYSTEM
#include "char_manager.h"
#include "char.h"
#endif

CItemAddonManager::CItemAddonManager()
{
}

CItemAddonManager::~CItemAddonManager()
{
}

#ifdef ENABLE_SWITCHBOT_WORLDARD
void CItemAddonManager::ApplyAddonTo(int iAddonType, LPITEM pItem, bool switchbot)
#else
void CItemAddonManager::ApplyAddonTo(int iAddonType, LPITEM pItem)
#endif
{
	if (!pItem)
	{
		sys_err("ITEM pointer null");
		return;
	}
#ifdef ENABLE_LOCK_ATTR
	if (pItem->GetAttrLockIndex(0) == 1 || pItem->GetAttrLockIndex(0) == 2 || pItem->GetAttrLockIndex(1) == 1 || pItem->GetAttrLockIndex(1) == 2)
		return;
#endif
	int iSkillBonus = MINMAX(-30, (int)(gauss_random(0, 5) + 0.5f), 30);
	int iNormalHitBonus = 0;
	if (abs(iSkillBonus) <= 20)
		iNormalHitBonus = -2 * iSkillBonus + abs(number(-8, 8) + number(-8, 8)) + number(1, 4);
	else
		iNormalHitBonus = -2 * iSkillBonus + number(1, 5);

	pItem->RemoveAttributeType(APPLY_SKILL_DAMAGE_BONUS);
	pItem->RemoveAttributeType(APPLY_NORMAL_HIT_DAMAGE_BONUS);
#ifdef ENABLE_SWITCHBOT_WORLDARD
	if (switchbot){
		pItem->AddAttributeSwitchBot(APPLY_NORMAL_HIT_DAMAGE_BONUS, iNormalHitBonus);
		pItem->AddAttributeSwitchBot(APPLY_SKILL_DAMAGE_BONUS, iSkillBonus);
	}else{
		pItem->AddAttribute(APPLY_NORMAL_HIT_DAMAGE_BONUS, iNormalHitBonus);
		pItem->AddAttribute(APPLY_SKILL_DAMAGE_BONUS, iSkillBonus);	
	}
#else
	pItem->AddAttribute(APPLY_NORMAL_HIT_DAMAGE_BONUS, iNormalHitBonus);
	pItem->AddAttribute(APPLY_SKILL_DAMAGE_BONUS, iSkillBonus);
#endif
#ifdef ENABLE_REWARD_SYSTEM
	if(iNormalHitBonus>=60 && pItem->GetLevelLimit() == 75)
	{
		LPCHARACTER ch = pItem->GetOwner();
		if(ch)
			CHARACTER_MANAGER::Instance().SetRewardData(REWARD_75WEAPONAVG,ch->GetName(), true);
	}

	if(iNormalHitBonus>=60 && pItem->GetLevelLimit() == 105)
	{
		LPCHARACTER ch = pItem->GetOwner();
		if(ch)
			CHARACTER_MANAGER::Instance().SetRewardData(REWARD_105WEAPONAVG,ch->GetName(), true);
	}
#endif
}
