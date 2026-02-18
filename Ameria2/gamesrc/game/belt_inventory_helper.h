#ifndef __HEADER_BELT_INVENTORY_HELPER__
#define __HEADER_BELT_INVENTORY_HELPER__

#include "char.h"
#include "item.h"

class CBeltInventoryHelper
{
public:
	typedef BYTE	TGradeUnit;

	static TGradeUnit GetBeltGradeByRefineLevel(DWORD level)
	{
		static TGradeUnit beltGradeByLevelTable[] =
		{
			0,			// º§Æ®+0
			1,			// +1
			1,			// +2
			2,			// +3
			2,			// +4,
			3,			// +5
			4,			// +6,
			5,			// +7,
			6,			// +8,
			7,			// +9
		};

		if (level >= _countof(beltGradeByLevelTable))
		{
			sys_err("CBeltInventoryHelper::GetBeltGradeByRefineLevel - Overflow level (%u)", level);
			return 0;
		}

		return beltGradeByLevelTable[level];
	}

	// ÇöÀç º§Æ® ·¹º§À» ±âÁØÀ¸·Î, ¾î¶² ¼¿µéÀ» ÀÌ¿ëÇÒ ¼ö ÀÖ´ÂÁö ¸®ÅÏ
	static const TGradeUnit* GetAvailableRuleTableByGrade()
	{
		/**
			º§Æ®´Â ÃÑ +0 ~ +9 ·¹º§À» °¡Áú ¼ö ÀÖÀ¸¸ç, ·¹º§¿¡ µû¶ó 7´Ü°è µî±ÞÀ¸·Î ±¸ºÐµÇ¾î ÀÎº¥Åä¸®°¡ È°¼º È­ µÈ´Ù.
			º§Æ® ·¹º§¿¡ µû¸¥ »ç¿ë °¡´ÉÇÑ ¼¿Àº ¾Æ·¡ ±×¸²°ú °°À½. ÇöÀç µî±Þ >= È°¼º°¡´É µî±ÞÀÌ¸é »ç¿ë °¡´É.
			(´Ü, ÇöÀç ·¹º§ÀÌ 0ÀÌ¸é ¹«Á¶°Ç »ç¿ë ºÒ°¡, °ýÈ£ ¾ÈÀÇ ¼ýÀÚ´Â µî±Þ)

				2(1)  4(2)  6(4)  8(6)
				5(3)  5(3)  6(4)  8(6)
				7(5)  7(5)  7(5)  8(6)
				9(7)  9(7)  9(7)  9(7)

			º§Æ® ÀÎº¥Åä¸®ÀÇ Å©±â´Â 4x4 (16Ä­)
		*/

		static TGradeUnit availableRuleByGrade[BELT_INVENTORY_SLOT_COUNT] = {
			1, 2, 4, 6,
			3, 3, 4, 6,
			5, 5, 5, 6,
			7, 7, 7, 7
		};

		return availableRuleByGrade;
	}

	static bool IsAvailableCell(WORD cell, int beltGrade /*int beltLevel*/)
	{
		// ±âÈ¹ ¶Ç ¹Ù²ñ.. ¾Æ³ö...
		//const TGradeUnit beltGrade = GetBeltGradeByRefineLevel(beltLevel);
		const TGradeUnit* ruleTable = GetAvailableRuleTableByGrade();

		return ruleTable[cell] <= beltGrade;
	}

	/// pcÀÇ º§Æ® ÀÎº¥Åä¸®¿¡ ¾ÆÀÌÅÛÀÌ ÇÏ³ª¶óµµ Á¸ÀçÇÏ´Â Áö °Ë»çÇÏ´Â ÇÔ¼ö.
	static bool IsExistItemInBeltInventory(LPCHARACTER pc)
	{
		for (WORD i = BELT_INVENTORY_SLOT_START; i < BELT_INVENTORY_SLOT_END; ++i)
		{
			LPITEM beltInventoryItem = pc->GetInventoryItem(i);

			if (NULL != beltInventoryItem)
				return true;
		}

		return false;
	}

	/// itemÀÌ º§Æ® ÀÎº¥Åä¸®¿¡ µé¾î°¥ ¼ö ÀÖ´Â Å¸ÀÔÀÎÁö °Ë»çÇÏ´Â ÇÔ¼ö. (ÀÌ ±ÔÄ¢Àº ±âÈ¹ÀÚ°¡ °áÁ¤ÇÔ)
	static bool CanMoveIntoBeltInventory(LPITEM item)
	{
		bool canMove = false;

		if (item->GetType() == ITEM_USE)
		{
			switch (item->GetSubType())
			{
			case USE_POTION:
			case USE_POTION_NODELAY:
			case USE_ABILITY_UP:
				canMove = true;
				break;
			}
		}

		return canMove;
	}

};

#endif //__HEADER_BELT_INVENTORY_HELPER__
