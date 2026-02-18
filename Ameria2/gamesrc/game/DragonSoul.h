#ifndef __INC_METIN_II_GAME_DRAGON_SOUL_H__
#define __INC_METIN_II_GAME_DRAGON_SOUL_H__

#include "../common/length.h"

class CHARACTER;
class CItem;

class DragonSoulTable;

class DSManager : public singleton<DSManager>
{
public:
	DSManager();
	~DSManager();
	bool	ReadDragonSoulTableFile(const char * c_pszFileName);

	void	GetDragonSoulInfo(DWORD dwVnum, OUT BYTE& bType, OUT BYTE& bGrade, OUT BYTE& bStep, OUT BYTE& bRefine) const;
	// fixme : titempos·Î
	WORD	GetBasePosition(const LPITEM pItem) const;
	bool	IsValidCellForThisItem(const LPITEM pItem, const TItemPos& Cell) const;
#ifdef ENABLE_SPECIAL_STORAGE
	bool	IsValidCellForThisItem2(const LPITEM pItem, const TItemPos& Cell) const;
#endif
	int		GetDuration(const LPITEM pItem) const;

	// ¿ëÈ¥¼®À» ¹Þ¾Æ¼­ Æ¯Á¤ ¿ë½ÉÀ» ÃßÃâÇÏ´Â ÇÔ¼ö
	bool	ExtractDragonHeart(LPCHARACTER ch, LPITEM pItem, LPITEM pExtractor = NULL);

	// Æ¯Á¤ ¿ëÈ¥¼®(pItem)À» ÀåºñÃ¢¿¡¼­ Á¦°ÅÇÒ ¶§¿¡ ¼º°ø ¿©ºÎ¸¦ °áÁ¤ÇÏ°í,
	// ½ÇÆÐ½Ã ºÎ»ê¹°À» ÁÖ´Â ÇÔ¼ö.(ºÎ»ê¹°Àº dragon_soul_table.txt¿¡ Á¤ÀÇ)
	// DestCell¿¡ invalidÇÑ °ªÀ» ³ÖÀ¸¸é ¼º°ø ½Ã, ¿ëÈ¥¼®À» ºó °ø°£¿¡ ÀÚµ¿ Ãß°¡.
	// ½ÇÆÐ ½Ã, ¿ëÈ¥¼®(pItem)Àº deleteµÊ.
	// ÃßÃâ¾ÆÀÌÅÛÀÌ ÀÖ´Ù¸é ÃßÃâ ¼º°ø È®·üÀÌ pExtractor->GetValue(0)%¸¸Å­ Áõ°¡ÇÔ.
	// ºÎ»ê¹°Àº ¾ðÁ¦³ª ÀÚµ¿ Ãß°¡.
	bool	PullOut(LPCHARACTER ch, TItemPos DestCell, IN OUT LPITEM& pItem, LPITEM pExtractor = NULL);

	// ¿ëÈ¥¼® ¾÷±×·¹ÀÌµå ÇÔ¼ö
	bool	DoRefineGrade(LPCHARACTER ch, TItemPos (&aItemPoses)[DRAGON_SOUL_REFINE_GRID_SIZE]);
	bool	DoRefineStep(LPCHARACTER ch, TItemPos (&aItemPoses)[DRAGON_SOUL_REFINE_GRID_SIZE]);
	bool	DoRefineStrength(LPCHARACTER ch, TItemPos (&aItemPoses)[DRAGON_SOUL_REFINE_GRID_SIZE]);

	bool	DragonSoulItemInitialize(LPITEM pItem);

	bool	IsTimeLeftDragonSoul(LPITEM pItem) const;
	int		LeftTime(LPITEM pItem) const;
	bool	ActivateDragonSoul(LPITEM pItem);
	bool	DeactivateDragonSoul(LPITEM pItem, bool bSkipRefreshOwnerActiveState = false);
	bool	IsActiveDragonSoul(LPITEM pItem) const;
private:
	void	SendRefineResultPacket(LPCHARACTER ch, BYTE bSubHeader, const TItemPos& pos);

	// Ä³¸¯ÅÍÀÇ ¿ëÈ¥¼® µ¦À» »ìÆìº¸°í, È°¼ºÈ­ µÈ ¿ëÈ¥¼®ÀÌ ¾ø´Ù¸é, Ä³¸¯ÅÍÀÇ ¿ëÈ¥¼® È°¼º »óÅÂ¸¦ off ½ÃÅ°´Â ÇÔ¼ö.
	void	RefreshDragonSoulState(LPCHARACTER ch);

	DWORD	MakeDragonSoulVnum(BYTE bType, BYTE grade, BYTE step, BYTE refine);
	
	bool	PutAttributes(LPITEM pDS);
	bool	RefreshItemAttributes(LPITEM pItem);
	
public:
	bool 	ChangeAttributes(LPCHARACTER ch, LPITEM pDS);
	

	DragonSoulTable*	m_pTable;
};

#endif
