#pragma once

#include "ItemData.h"
#if defined(ENABLE_SHOW_CHEST_DROP)
#include <unordered_map>
#endif


class CItemManager : public CSingleton<CItemManager>
{
	public:
		enum EItemDescCol
		{
			ITEMDESC_COL_VNUM,
			ITEMDESC_COL_NAME,
			ITEMDESC_COL_DESC,
			ITEMDESC_COL_SUMM,
			ITEMDESC_COL_NUM,
		};
		#ifdef ENABLE_SASH_SYSTEM
		enum EItemScaleColumn
		{
			ITEMSCALE_VNUM,
			ITEMSCALE_JOB,
			ITEMSCALE_SEX,
			ITEMSCALE_SCALE_X,
			ITEMSCALE_SCALE_Y,
			ITEMSCALE_SCALE_Z,
			ITEMSCALE_POSITION_X,
			ITEMSCALE_POSITION_Y,
			ITEMSCALE_POSITION_Z,
			ITEMSCALE_NUM,
		};
		#endif

	public:
		typedef std::map<DWORD, CItemData*> TItemMap;
		typedef std::map<std::string, CItemData*> TItemNameMap;

	public:
		CItemManager();
		virtual ~CItemManager();

		void			Destroy();

		BOOL			SelectItemData(DWORD dwIndex);
		CItemData *		GetSelectedItemDataPointer();

		BOOL			GetItemDataPointer(DWORD dwItemID, CItemData ** ppItemData);

#if defined(ENABLE_DROP_ITEM_WORLDARD) || defined(INSIDE_RENDER)
		TItemMap		GetItems() const { return m_ItemMap; }
#endif
		
		/////
		bool			LoadItemDesc(const char* c_szFileName);
		bool			LoadItemList(const char* c_szFileName);
		bool			LoadItemTable(const char* c_szFileName);
		CItemData *		MakeItemData(DWORD dwIndex);
#ifdef ENABLE_IKASHOP_RENEWAL
		void			GetItemsNameMap(std::map<DWORD, std::string>& inMap);
#endif
		#ifdef ENABLE_SASH_SYSTEM
		bool	LoadItemScale(const char* c_szFileName);
		#endif
#ifdef ENABLE_SHINING_TABLE_SYSTEM

private:
	std::unordered_map<uint32_t, std::string> m_shiningItemMap;
public:
		bool			LoadShiningTable(const char* c_szFileName);
		std::optional<std::string>		GetItemShiningPath(uint32_t vnum) const
		{
			auto it = m_shiningItemMap.find(vnum);

			if (it == m_shiningItemMap.end())
				return std::nullopt;

			return it->second;
		}
#endif

#if defined(ENABLE_SHOW_CHEST_DROP)
		struct SDropItemInfo 
		{
			DWORD	dwDropVnum;
			int		iCount;
		};

		using							TChestDropItemInfoVec = std::vector<SDropItemInfo>;
		using							TChestDropItemInfoMap = std::unordered_map<DWORD, TChestDropItemInfoVec>;

		bool							LoadChestDropInfo(const char* c_szFileName);
		const TChestDropItemInfoVec* 	GetItemDropInfoVec(const DWORD dwVnum) const;
		void 							ClearChest(DWORD chestIndex);
		void 							LoadChestDropFromServer(DWORD chestIndex, TChestDropItemInfoVec& drop);
#endif

#ifdef USE_OFFLINESHOP_SEARCH_V2
        bool GetNameByHint(const std::string& stHint, std::string& recvHint, uint32_t& recvVnum);
#endif

	protected:
		TItemMap m_ItemMap;
		std::vector<CItemData*>  m_vec_ItemRange;
		CItemData * m_pSelectedItemData;
#if defined(ENABLE_SHOW_CHEST_DROP)
		TChestDropItemInfoMap m_ItemDropInfoMap;
#endif
#ifdef ENABLE_AURA_SYSTEM
	public:
		enum EAuraScaleCol
		{
			AURA_SCALE_COL_VNUM,
			AURA_SCALE_COL_JOB,
			AURA_SCALE_COL_SEX,
			AURA_SCALE_COL_MESH_SCALE_X,
			AURA_SCALE_COL_MESH_SCALE_Y,
			AURA_SCALE_COL_MESH_SCALE_Z,
			AURA_SCALE_COL_PARTICLE_SCALE,
			AURA_SCALE_COL_NUM,
		};

		bool	LoadAuraScale(const char* c_szFileName);
#endif

};
