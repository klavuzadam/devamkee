#pragma once

#include "Packet.h"

typedef enum
{
	SHOP_COIN_TYPE_GOLD, // DEFAULT VALUE
	SHOP_COIN_TYPE_SECONDARY_COIN,
#if defined(WJ_COMBAT_ZONE)
	SHOP_COIN_TYPE_COMBAT_ZONE,
#endif
} EShopCoinType;

class CPythonShop : public CSingleton<CPythonShop>
{
	public:
		CPythonShop(void);
		virtual ~CPythonShop(void);

		void Clear();

		void SetItemData(DWORD dwIndex, const TShopItemData & c_rShopItemData);
		BOOL GetItemData(DWORD dwIndex, const TShopItemData ** c_ppItemData);

		void SetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData & c_rShopItemData);
		BOOL GetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData ** c_ppItemData);

		void SetTabCount(BYTE bTabCount) { m_bTabCount = bTabCount; }
		BYTE GetTabCount() { return m_bTabCount; }

		void SetTabCoinType(BYTE tabIdx, BYTE coinType);
		BYTE GetTabCoinType(BYTE tabIdx);

		void SetTabName(BYTE tabIdx, const char* name);
		const char* GetTabName(BYTE tabIdx);


		//BOOL GetSlotItemID(DWORD dwSlotPos, DWORD* pdwItemID);

		void Open();
		void Close();
		BOOL IsOpen();

	protected:
		BOOL	CheckSlotIndex(DWORD dwIndex);

	protected:
		BOOL				m_isShoping;
		struct ShopTab
		{
			ShopTab()
			{
				coinType = SHOP_COIN_TYPE_GOLD;
			}
			BYTE				coinType;
			std::string			name;
			TShopItemData		items[SHOP_HOST_ITEM_MAX_NUM];
		};

		BYTE m_bTabCount;
		ShopTab m_aShoptabs[SHOP_TAB_COUNT_MAX];
};
