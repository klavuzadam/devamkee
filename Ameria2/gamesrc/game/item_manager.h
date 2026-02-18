#ifndef __INC_ITEM_MANAGER__
#define __INC_ITEM_MANAGER__

#ifdef M2_USE_POOL
#include "pool.h"
#endif

// special_item_group.txt¿¡¼­ Á¤ÀÇÇÏ´Â ¼Ó¼º ±×·ì
// type attr·Î ¼±¾ðÇÒ ¼ö ÀÖ´Ù.
// ÀÌ ¼Ó¼º ±×·ìÀ» ÀÌ¿ëÇÒ ¼ö ÀÖ´Â °ÍÀº special_item_group.txt¿¡¼­ Special typeÀ¸·Î Á¤ÀÇµÈ ±×·ì¿¡ ¼ÓÇÑ UNIQUE ITEMÀÌ´Ù.
class CSpecialAttrGroup
{
public:
	CSpecialAttrGroup(DWORD vnum)
		: m_dwVnum(vnum)
	{}
	struct CSpecialAttrInfo
	{
		CSpecialAttrInfo (DWORD _apply_type, DWORD _apply_value)
			: apply_type(_apply_type), apply_value(_apply_value)
		{}
		DWORD apply_type;
		DWORD apply_value;

	};
	DWORD m_dwVnum;
	std::string	m_stEffectFileName;
	std::vector<CSpecialAttrInfo> m_vecAttrs;
};

class CSpecialItemGroup
{
	public:
		enum EGiveType
		{
			NONE,
			GOLD,
			EXP,
			MOB,
			SLOW,
			DRAIN_HP,
			POISON,
			MOB_GROUP,
#ifdef ENABLE_WOLFMAN_CHARACTER
			BLEEDING,
#endif
		};

		// QUEST Å¸ÀÔÀº Äù½ºÆ® ½ºÅ©¸³Æ®¿¡¼­ vnum.sig_use¸¦ »ç¿ëÇÒ ¼ö ÀÖ´Â ±×·ìÀÌ´Ù.
		//		´Ü, ÀÌ ±×·ì¿¡ µé¾î°¡±â À§ÇØ¼­´Â ITEM ÀÚÃ¼ÀÇ TYPEÀÌ QUEST¿©¾ß ÇÑ´Ù.
		// SPECIAL Å¸ÀÔÀº idx, item_vnum, attr_vnumÀ» ÀÔ·ÂÇÑ´Ù. attr_vnumÀº À§¿¡ CSpecialAttrGroupÀÇ VnumÀÌ´Ù.
		//		ÀÌ ±×·ì¿¡ µé¾îÀÖ´Â ¾ÆÀÌÅÛÀº °°ÀÌ Âø¿ëÇÒ ¼ö ¾ø´Ù.
		enum ESIGType { NORMAL, PCT, QUEST, SPECIAL };

		struct CSpecialItemInfo
		{
			DWORD vnum;
			int count;
			int rare;

			CSpecialItemInfo(DWORD _vnum, int _count, int _rare)
				: vnum(_vnum), count(_count), rare(_rare)
				{}
		};

		CSpecialItemGroup(DWORD vnum, BYTE type=0)
			: m_dwVnum(vnum), m_bType(type)
			{}

		void AddItem(DWORD vnum, int count, int prob, int rare)
		{
			if (!prob)
				return;
			if (!m_vecProbs.empty())
				prob += m_vecProbs.back();
			m_vecProbs.push_back(prob);
			m_vecItems.push_back(CSpecialItemInfo(vnum, count, rare));
		}

		bool IsEmpty() const
		{
			return m_vecProbs.empty();
		}

		// Type Multi, Áï m_bType == PCT ÀÎ °æ¿ì,
		// È®·üÀ» ´õÇØ°¡Áö ¾Ê°í, µ¶¸³ÀûÀ¸·Î °è»êÇÏ¿© ¾ÆÀÌÅÛÀ» »ý¼ºÇÑ´Ù.
		// µû¶ó¼­ ¿©·¯ °³ÀÇ ¾ÆÀÌÅÛÀÌ »ý¼ºµÉ ¼ö ÀÖ´Ù.
		// by rtsummit
		int GetMultiIndex(std::vector <int> &idx_vec) const
		{
			idx_vec.clear();
			if (m_bType == PCT)
			{
				int count = 0;
				if (number(1,100) <= m_vecProbs[0])
				{
					idx_vec.push_back(0);
					count++;
				}
				for (uint32_t i = 1; i < m_vecProbs.size(); i++)
				{
					if (number(1,100) <= m_vecProbs[i] - m_vecProbs[i-1])
					{
						idx_vec.push_back(i);
						count++;
					}
				}
				return count;
			}
			else
			{
				idx_vec.push_back(GetOneIndex());
				return 1;
			}
		}

		int GetOneIndex() const
		{
			int n = number(1, m_vecProbs.back());
			itertype(m_vecProbs) it = lower_bound(m_vecProbs.begin(), m_vecProbs.end(), n);
			return std::distance(m_vecProbs.begin(), it);
		}

		int GetVnum(int idx) const
		{
			return m_vecItems[idx].vnum;
		}

		int GetCount(int idx) const
		{
			return m_vecItems[idx].count;
		}

		int GetRarePct(int idx) const
		{
			return m_vecItems[idx].rare;
		}

		bool Contains(DWORD dwVnum) const
		{
			for (DWORD i = 0; i < m_vecItems.size(); i++)
			{
				if (m_vecItems[i].vnum == dwVnum)
					return true;
			}
			return false;
		}

		// GroupÀÇ TypeÀÌ SpecialÀÎ °æ¿ì¿¡
		// dwVnum¿¡ ¸ÅÄªµÇ´Â AttrVnumÀ» returnÇØÁØ´Ù.
		DWORD GetAttrVnum(DWORD dwVnum) const
		{
			if (CSpecialItemGroup::SPECIAL != m_bType)
				return 0;
			for (itertype(m_vecItems) it = m_vecItems.begin(); it != m_vecItems.end(); it++)
			{
				if (it->vnum == dwVnum)
				{
					return it->count;
				}
			}
			return 0;
		}

		// GroupÀÇ Size¸¦ returnÇØÁØ´Ù.
		int GetGroupSize() const
		{
			return m_vecProbs.size();
		}

		DWORD m_dwVnum;
		BYTE	m_bType;
		std::vector<int> m_vecProbs;
		std::vector<CSpecialItemInfo> m_vecItems; // vnum, count
};

class CMobItemGroup
{
	public:
		struct SMobItemGroupInfo
		{
			DWORD dwItemVnum;
			int iCount;
			int iRarePct;

			SMobItemGroupInfo(DWORD dwItemVnum, int iCount, int iRarePct)
				: dwItemVnum(dwItemVnum),
			iCount(iCount),
			iRarePct(iRarePct)
			{
			}
		};

		CMobItemGroup(DWORD dwMobVnum, int iKillDrop, const std::string& r_stName)
			:
			m_dwMobVnum(dwMobVnum),
		m_iKillDrop(iKillDrop),
		m_stName(r_stName)
		{
		}

#ifdef ENABLE_DROP_ITEM_WORLDARD
		const std::vector<SMobItemGroupInfo> & GetVector()
		{
			return m_vecItems;
		}
#endif

		int GetKillPerDrop() const
		{
			return m_iKillDrop;
		}

		void AddItem(DWORD dwItemVnum, int iCount, int iPartPct, int iRarePct)
		{
			if (!m_vecProbs.empty())
				iPartPct += m_vecProbs.back();
			m_vecProbs.push_back(iPartPct);
			m_vecItems.push_back(SMobItemGroupInfo(dwItemVnum, iCount, iRarePct));
		}

		// MOB_DROP_ITEM_BUG_FIX
		bool IsEmpty() const
		{
			return m_vecProbs.empty();
		}

		int GetOneIndex() const
		{
			int n = number(1, m_vecProbs.back());
			itertype(m_vecProbs) it = lower_bound(m_vecProbs.begin(), m_vecProbs.end(), n);
			return std::distance(m_vecProbs.begin(), it);
		}
		// END_OF_MOB_DROP_ITEM_BUG_FIX

		const SMobItemGroupInfo& GetOne() const
		{
			return m_vecItems[GetOneIndex()];
		}

	private:
		DWORD m_dwMobVnum;
		int m_iKillDrop;
		std::string m_stName;
		std::vector<int> m_vecProbs;
		std::vector<SMobItemGroupInfo> m_vecItems;
};

class CDropItemGroup
{
	struct SDropItemGroupInfo
	{
		DWORD	dwVnum;
		DWORD	dwPct;
		int	iCount;

		SDropItemGroupInfo(DWORD dwVnum, DWORD dwPct, int iCount)
			: dwVnum(dwVnum), dwPct(dwPct), iCount(iCount)
			{}
	};

	public:
	CDropItemGroup(DWORD dwVnum, DWORD dwMobVnum, const std::string& r_stName)
		:
		m_dwVnum(dwVnum),
	m_dwMobVnum(dwMobVnum),
	m_stName(r_stName)
	{
	}

	const std::vector<SDropItemGroupInfo> & GetVector()
	{
		return m_vec_items;
	}

	void AddItem(DWORD dwItemVnum, DWORD dwPct, int iCount)
	{
		m_vec_items.push_back(SDropItemGroupInfo(dwItemVnum, dwPct, iCount));
	}

	private:
	DWORD m_dwVnum;
	DWORD m_dwMobVnum;
	std::string m_stName;
	std::vector<SDropItemGroupInfo> m_vec_items;
};

#ifdef NEW_COMMON_DROP
class CCommonDropNew
{
public:

	bool CanDropThisMap(DWORD mapIndex)
	{
		if (m_vecMapIndex.size())
		{
			auto it = std::find(m_vecMapIndex.begin(), m_vecMapIndex.end(), mapIndex);
			if (it == m_vecMapIndex.end())
				return false;

		}
		return true;
	}
	void AddItem(DWORD dwItemVnum, DWORD iCount, DWORD iPartPct)
	{
		m_vecProbs.push_back(iPartPct);
		m_vecItems.emplace_back(dwItemVnum, iCount);
	}

	bool IsEmpty() const{return m_vecProbs.empty();}

	int	minLevel;
	int	maxLevel;
	std::vector<DWORD> m_vecMapIndex;
	std::vector<DWORD> m_vecProbs;
	std::vector<std::pair<DWORD,DWORD>> m_vecItems;
};
#endif

class CLevelItemGroup
{
	struct SLevelItemGroupInfo
	{
		DWORD dwVNum;
		DWORD dwPct;
		int iCount;

		SLevelItemGroupInfo(DWORD dwVnum, DWORD dwPct, int iCount)
			: dwVNum(dwVnum), dwPct(dwPct), iCount(iCount)
		{ }
	};

	private :
		DWORD m_dwLevelLimit;
		std::string m_stName;
		std::vector<SLevelItemGroupInfo> m_vec_items;

	public :
		CLevelItemGroup(DWORD dwLevelLimit)
			: m_dwLevelLimit(dwLevelLimit)
		{}

		DWORD GetLevelLimit() { return m_dwLevelLimit; }

		void AddItem(DWORD dwItemVnum, DWORD dwPct, int iCount)
		{
			m_vec_items.push_back(SLevelItemGroupInfo(dwItemVnum, dwPct, iCount));
		}

		const std::vector<SLevelItemGroupInfo> & GetVector()
		{
			return m_vec_items;
		}
};

class CBuyerThiefGlovesItemGroup
{
	struct SThiefGroupInfo
	{
		DWORD	dwVnum;
		DWORD	dwPct;
		int	iCount;

		SThiefGroupInfo(DWORD dwVnum, DWORD dwPct, int iCount)
			: dwVnum(dwVnum), dwPct(dwPct), iCount(iCount)
			{}
	};

	public:
	CBuyerThiefGlovesItemGroup(DWORD dwVnum, DWORD dwMobVnum, const std::string& r_stName)
		:
		m_dwVnum(dwVnum),
	m_dwMobVnum(dwMobVnum),
	m_stName(r_stName)
	{
	}

	const std::vector<SThiefGroupInfo> & GetVector()
	{
		return m_vec_items;
	}

	void AddItem(DWORD dwItemVnum, DWORD dwPct, int iCount)
	{
		m_vec_items.push_back(SThiefGroupInfo(dwItemVnum, dwPct, iCount));
	}

	private:
	DWORD m_dwVnum;
	DWORD m_dwMobVnum;
	std::string m_stName;
	std::vector<SThiefGroupInfo> m_vec_items;
};

class ITEM;

class ITEM_MANAGER : public singleton<ITEM_MANAGER>
{
	public:
		ITEM_MANAGER();
		virtual ~ITEM_MANAGER();

		bool                    Initialize(TItemTable * table, int size);
		void			Destroy();
		void			Update();	// ¸Å ·çÇÁ¸¶´Ù ºÎ¸¥´Ù.
		void			GracefulShutdown();

		DWORD			GetNewID();
		bool			SetMaxItemID(TItemIDRangeTable range); // ÃÖ´ë °íÀ¯ ¾ÆÀÌµð¸¦ ÁöÁ¤
		bool			SetMaxSpareItemID(TItemIDRangeTable range);

		// DelayedSave: ¾î¶°ÇÑ ·çÆ¾ ³»¿¡¼­ ÀúÀåÀ» ÇØ¾ß ÇÒ ÁþÀ» ¸¹ÀÌ ÇÏ¸é ÀúÀå
		// Äõ¸®°¡ ³Ê¹« ¸¹¾ÆÁö¹Ç·Î "ÀúÀåÀ» ÇÑ´Ù" ¶ó°í Ç¥½Ã¸¸ ÇØµÎ°í Àá±ñ
		// (¿¹: 1 frame) ÈÄ¿¡ ÀúÀå½ÃÅ²´Ù.
		void			DelayedSave(LPITEM item);
		void			FlushDelayedSave(LPITEM item); // Delayed ¸®½ºÆ®¿¡ ÀÖ´Ù¸é Áö¿ì°í ÀúÀåÇÑ´Ù. ²÷±è Ã³¸®½Ã »ç¿ë µÊ.
		void			SaveSingleItem(LPITEM item);

#ifdef __SOULBINDING_SYSTEM__
		LPITEM	CreateItem(DWORD vnum, DWORD count = 1, DWORD dwID = 0, bool bTryMagic = false, int iRarePct = -1, bool bSkipSave = false, long lBind = 0);
#else
		LPITEM	CreateItem(DWORD vnum, DWORD count = 1, DWORD dwID = 0, bool bTryMagic = false, int iRarePct = -1, bool bSkipSave = false);
#endif
#ifndef DEBUG_ALLOC
		void DestroyItem(LPITEM item);
#else
		void DestroyItem(LPITEM item, const char* file, size_t line);
#endif
		void			RemoveItem(LPITEM item, const char * c_pszReason=NULL); // »ç¿ëÀÚ·Î ºÎÅÍ ¾ÆÀÌÅÛÀ» Á¦°Å

		LPITEM			Find(DWORD id);
		LPITEM                  FindByVID(DWORD vid);
		TItemTable *            GetTable(DWORD vnum);
		bool			GetVnum(const char * c_pszName, DWORD & r_dwVnum);
		bool			GetVnumByOriginalName(const char * c_pszName, DWORD & r_dwVnum);

		bool			GetDropPct(LPCHARACTER pkChr, LPCHARACTER pkKiller, OUT int& iDeltaPercent, OUT int& iRandRange);
		bool			CreateDropItem(LPCHARACTER pkChr, LPCHARACTER pkKiller, std::vector<LPITEM> & vec_item);
#ifdef __SEND_TARGET_INFO__
		bool			CreateDropItemVector(LPCHARACTER pkChr, LPCHARACTER pkKiller, std::vector<LPITEM> & vec_item);
#endif
		bool			ReadCommonDropItemFile(const char * c_pszFileName);
		bool			ReadEtcDropItemFile(const char * c_pszFileName);
		bool			ReadDropItemGroup(const char * c_pszFileName);
		bool			ReadMonsterDropItemGroup(const char * c_pszFileName);
		bool			ReadSpecialDropItemFile(const char * c_pszFileName);
#ifdef NEW_COMMON_DROP
		bool			ReadCommonDropNewItemFile(const char* c_pszFileName);
#endif
		// convert name -> vnum special_item_group.txt
		bool			ConvSpecialDropItemFile();
		// convert name -> vnum special_item_group.txt

		DWORD			GetRefineFromVnum(DWORD dwVnum);

		static void		CopyAllAttrTo(LPITEM pkOldItem, LPITEM pkNewItem);		// pkNewItemÀ¸·Î ¸ðµç ¼Ó¼º°ú ¼ÒÄÏ °ªµéÀ» ¸ñ»çÇÏ´Â ÇÔ¼ö.

		const CSpecialItemGroup* GetSpecialItemGroup(DWORD dwVnum);
		const CSpecialAttrGroup* GetSpecialAttrGroup(DWORD dwVnum);

		const std::vector<TItemTable> & GetTable() { return m_vec_prototype; }

		// CHECK_UNIQUE_GROUP
		int			GetSpecialGroupFromItem(DWORD dwVnum) const { itertype(m_ItemToSpecialGroup) it = m_ItemToSpecialGroup.find(dwVnum); return (it == m_ItemToSpecialGroup.end()) ? 0 : it->second; }
		// END_OF_CHECK_UNIQUE_GROUP

#ifdef ENABLE_DROP_ITEM_WORLDARD
		std::vector<int> GetMobVnumsItemGroup(){return m_map_pkItemMobVnumGroup; }
		bool GetLevelItemGroup(int dwMobVnumS, DWORD dwVnumS);
		std::map<DWORD, CLevelItemGroup*> & GetLevelItemGroup(){return m_map_pkLevelItemGroup; }
		std::map<DWORD, CMobItemGroup*> & GetMobItemGroup(){return  m_map_pkMobItemGroup; }
		std::map<DWORD, CDropItemGroup*> & GetDropItemGroup(){return  m_map_pkDropItemGroup; }
#endif
#ifdef NEW_COMMON_DROP
		std::vector<CCommonDropNew*> m_map_pkCommonNewItemGroup;
#endif
	protected:
		int                     RealNumber(DWORD vnum);
		void			CreateQuestDropItem(LPCHARACTER pkChr, LPCHARACTER pkKiller, std::vector<LPITEM> & vec_item, int iDeltaPercent, int iRandRange);

	protected:
		typedef std::map<DWORD, LPITEM> ITEM_VID_MAP;

		std::vector<TItemTable>		m_vec_prototype;
		std::vector<TItemTable*> m_vec_item_vnum_range_info;
		std::map<DWORD, DWORD>		m_map_ItemRefineFrom;
		int				m_iTopOfTable;

		ITEM_VID_MAP			m_VIDMap;			///< m_dwVIDCount ÀÇ °ª´ÜÀ§·Î ¾ÆÀÌÅÛÀ» ÀúÀåÇÑ´Ù.
		DWORD				m_dwVIDCount;			///< ÀÌ³à¼® VID°¡ ¾Æ´Ï¶ó ±×³É ÇÁ·Î¼¼½º ´ÜÀ§ À¯´ÏÅ© ¹øÈ£´Ù.
		DWORD				m_dwCurrentID;
		TItemIDRangeTable	m_ItemIDRange;
		TItemIDRangeTable	m_ItemIDSpareRange;

		TR1_NS::unordered_set<LPITEM> m_set_pkItemForDelayedSave;
		std::map<DWORD, LPITEM>		m_map_pkItemByID;
		std::map<DWORD, DWORD>		m_map_dwEtcItemDropProb;
		std::map<DWORD, CDropItemGroup*> m_map_pkDropItemGroup;
		std::map<DWORD, CSpecialItemGroup*> m_map_pkSpecialItemGroup;
		std::map<DWORD, CSpecialItemGroup*> m_map_pkQuestItemGroup;
		std::map<DWORD, CSpecialAttrGroup*> m_map_pkSpecialAttrGroup;
		std::map<DWORD, CMobItemGroup*> m_map_pkMobItemGroup;
		std::map<DWORD, CLevelItemGroup*> m_map_pkLevelItemGroup;
		std::map<DWORD, CBuyerThiefGlovesItemGroup*> m_map_pkGloveItemGroup;

#ifdef ENABLE_DROP_ITEM_WORLDARD
		std::vector<int> m_map_pkItemMobVnumGroup;
#endif
		
		// CHECK_UNIQUE_GROUP
		std::map<DWORD, int>		m_ItemToSpecialGroup;
		// END_OF_CHECK_UNIQUE_GROUP

	private:
		// µ¶ÀÏ¿¡¼­ ±âÁ¸ Ä³½Ã ¾ÆÀÌÅÛ°ú °°Áö¸¸, ±³È¯ °¡´ÉÇÑ Ä³½Ã ¾ÆÀÌÅÛÀ» ¸¸µç´Ù°í ÇÏ¿©,
		// ¿À¸®Áö³Î ¾ÆÀÌÅÛ¿¡ ±³È¯ ±ÝÁö ÇÃ·¡±×¸¸ »èÁ¦ÇÑ »õ·Î¿î ¾ÆÀÌÅÛµéÀ» ¸¸µé¾î,
		// »õ·Î¿î ¾ÆÀÌÅÛ ´ë¿ªÀ» ÇÒ´çÇÏ¿´´Ù.
		// ¹®Á¦´Â »õ·Î¿î ¾ÆÀÌÅÛµµ ¿À¸®Áö³Î ¾ÆÀÌÅÛ°ú °°Àº È¿°ú¸¦ ³»¾ßÇÏ´Âµ¥,
		// ¼­¹ö°Ç, Å¬¶ó°Ç, vnum ±â¹ÝÀ¸·Î µÇ¾îÀÖ¾î
		// »õ·Î¿î vnumÀ» ÁË´Ù ¼­¹ö¿¡ »õ·Î ´Ù ¹Ú¾Æ¾ßÇÏ´Â ¾ÈÅ¸±î¿î »óÈ²¿¡ ¸Â´ê¾Ò´Ù.
		// ±×·¡¼­ »õ vnumÀÇ ¾ÆÀÌÅÛÀÌ¸é, ¼­¹ö¿¡¼­ µ¹¾Æ°¥ ¶§´Â ¿À¸®Áö³Î ¾ÆÀÌÅÛ vnumÀ¸·Î ¹Ù²ã¼­ µ¹°í ÇÏ°í,
		// ÀúÀåÇÒ ¶§¿¡ º»·¡ vnumÀ¸·Î ¹Ù²ãÁÖµµ·Ï ÇÑ´Ù.
		// ÀÌ¸¦ À§ÇØ ¿À¸®Áö³Î vnum°ú »õ·Î¿î vnumÀ» ¿¬°á½ÃÄÑÁÖ´Â ¸ÊÀ» ¸¸µê.
		typedef std::map <DWORD, DWORD> TMapDW2DW;
		TMapDW2DW	m_map_new_to_ori;

	public:
		DWORD	GetMaskVnum(DWORD dwVnum);
		std::map<DWORD, TItemTable>  m_map_vid;
		std::map<DWORD, TItemTable>&  GetVIDMap() { return m_map_vid; }
		std::vector<TItemTable>& GetVecProto() { return m_vec_prototype; }
#ifndef ENABLE_SWITCHBOT
		const static int MAX_NORM_ATTR_NUM = 5;
		const static int MAX_RARE_ATTR_NUM = 2;
#endif
		bool ReadItemVnumMaskTable(const char * c_pszFileName);
	private:
#ifdef M2_USE_POOL
		ObjectPool<CItem> pool_;
#endif
};

#ifndef DEBUG_ALLOC
#define M2_DESTROY_ITEM(ptr) ITEM_MANAGER::instance().DestroyItem(ptr)
#else
#define M2_DESTROY_ITEM(ptr) ITEM_MANAGER::instance().DestroyItem(ptr, __FILE__, __LINE__)
#endif

#endif
