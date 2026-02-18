#ifndef __HEADER_VNUM_HELPER__
#define __HEADER_VNUM_HELPER__

#if VSTD_HAS_PRAGMA_ONCE
#pragma once
#endif

#include "CommonDefines.h"
#ifdef USE_BL_67_ATTR_RENEWED
#include "length.h"
#include "item_length.h"
#endif

/**
	ÀÌ¹Ì Á¸ÀçÇÏ°Å³ª ¾ÕÀ¸·Î Ãß°¡µÉ ¾ÆÀÌÅÛ, ¸÷ µîÀ» ¼Ò½º¿¡¼­ ½Äº°ÇÒ ¶§ ÇöÀç´Â ¸ðµÎ
	½Äº°ÀÚ(¼ýÀÚ=VNum)¸¦ ÇÏµåÄÚµùÇÏ´Â ¹æ½ÄÀ¸·Î µÇ¾îÀÖ¾î¼­ °¡µ¶¼ºÀÌ ¸Å¿ì ¶³¾îÁö´Âµ¥

	¾ÕÀ¸·Î´Â ¼Ò½º¸¸ ºÁµµ ¾î¶² ¾ÆÀÌÅÛ(È¤Àº ¸÷)ÀÎÁö ¾Ë ¼ö ÀÖ°Ô ÇÏÀÚ´Â ½ÂÃ¶´ÔÀÇ Á¦¾ÈÀ¸·Î Ãß°¡.

		* ÀÌ ÆÄÀÏÀº º¯°æÀÌ ÀæÀ»°ÍÀ¸·Î ¿¹»óµÇ´Âµ¥ PCH¿¡ ³ÖÀ¸¸é ¹Ù²ð ¶§¸¶´Ù ÀüÃ¼ ÄÄÆÄÀÏ ÇØ¾ßÇÏ´Ï
		ÀÏ´ÜÀº ÇÊ¿äÇÑ cppÆÄÀÏ¿¡¼­ include ÇØ¼­ ¾²µµ·Ï ÇßÀ½.

		* cpp¿¡¼­ ±¸ÇöÇÏ¸é ÄÄÆÄÀÏ ~ ¸µÅ©ÇØ¾ßÇÏ´Ï ±×³É common¿¡ Çì´õ¸¸ ³Ö¾úÀ½. (game, dbÇÁ·ÎÁ§Æ® µÑ ´Ù »ç¿ë ¿¹Á¤)

	@date	2011. 8. 29.
*/


class CItemVnumHelper
{
public:
	static const bool IsExtraBook(const DWORD dwVnum)
	{
		switch (dwVnum)
		{
			case 50301:
			case 50302:
			case 50303:
#ifdef USE_PASSIVE_ABILITY_TAMER
			case 50940:
			case 50941:
			case 50942:
			case 50950:
			case 50951:
			case 50952:
			case 50960:
			case 50961:
			case 50962:
			case 50970:
			case 50971:
			case 50972:
#endif
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	/// µ¶ÀÏ DVD¿ë ºÒ»çÁ¶ ¼ÒÈ¯±Ç
	static	const bool	IsPhoenix(DWORD vnum)				{ return 53001 == vnum; }		// NOTE: ºÒ»çÁ¶ ¼ÒÈ¯ ¾ÆÀÌÅÛÀº 53001 ÀÌÁö¸¸ mob-vnumÀº 34001 ÀÔ´Ï´Ù.

	/// ¶ó¸¶´Ü ÀÌº¥Æ® ÃÊ½Â´ÞÀÇ ¹ÝÁö (¿ø·¡´Â ¶ó¸¶´Ü ÀÌº¥Æ®¿ë Æ¯¼ö ¾ÆÀÌÅÛÀÌ¾úÀ¸³ª ¾ÕÀ¸·Î ¿©·¯ ¹æÇâÀ¸·Î ÀçÈ°¿ëÇØ¼­ °è¼Ó ¾´´Ù°í ÇÔ)
	static	const bool	IsRamadanMoonRing(DWORD vnum)		{ return 71135 == vnum; }

	/// ÇÒ·ÎÀ© »çÅÁ (½ºÆåÀº ÃÊ½Â´ÞÀÇ ¹ÝÁö¿Í µ¿ÀÏ)
	static	const bool	IsHalloweenCandy(DWORD vnum)		{ return 71136 == vnum; }

	/// Å©¸®½º¸¶½º Çàº¹ÀÇ ¹ÝÁö
	static	const bool	IsHappinessRing(DWORD vnum)		{ return 71143 == vnum; }

	/// ¹ß·»Å¸ÀÎ »ç¶ûÀÇ ÆÒ´øÆ®
	static	const bool	IsLovePendant(DWORD vnum)		{ return 71145 == vnum; }

#ifdef __BL_67_ATTR__
#ifdef USE_BL_67_ATTR_RENEWED
	static DWORD Get67MaterialVnum(const BYTE bType, const BYTE bSubType)
	{
		if (bType == ITEM_WEAPON)
		{
			switch (bSubType)
			{
				case WEAPON_SWORD:
				case WEAPON_DAGGER:
				case WEAPON_BOW:
				case WEAPON_TWO_HANDED:
				case WEAPON_BELL:
				case WEAPON_FAN:
#ifdef ENABLE_WOLFMAN_CHARACTER
				case WEAPON_CLAW:
#endif
				{
					return 39071;
				}
				default:
				{
					break;
				}
			}
		}
		else if (bType == ITEM_ARMOR)
		{
			switch (bSubType)
			{
				case ARMOR_BODY:
				{
					return 39072;
				}
				case ARMOR_HEAD:
				{
					return 39074;
				}
				case ARMOR_SHIELD:
				{
					return 39075;
				}
				case ARMOR_FOOTS:
				{
					return 39076;
				}
#ifdef ENABLE_YOHARA_SYSTEM
				case ARMOR_GLOVE:
				{
					return 39081;
				}
#endif
				case ARMOR_WRIST:
				case ARMOR_NECK:
				case ARMOR_EAR:
				{
					return 39073;
				}
				default:
				{
					break;
				}
			}
		}

		return 0;
	}
#else
	static DWORD Get67MaterialVnum(const int iLevelLimit
#ifdef ENABLE_YOHARA_SYSTEM
, const int iConquerorLevel
#endif
	)
	{
#ifdef ENABLE_YOHARA_SYSTEM
		if (iConquerorLevel > 0)
		{
			return 39081;
		}
#endif

		if (iLevelLimit >= 0 && iLevelLimit <= 45)
			return 39070;

		if (iLevelLimit >= 46 && iLevelLimit <= 75)
			return 39071;

		if (iLevelLimit >= 76 && iLevelLimit <= 105)
			return 39072;

		if (iLevelLimit >= 106 && iLevelLimit <= 120)
			return 39073;

		return 0;
	}
#endif
};
#endif

class CMobVnumHelper
{
public:
	/// µ¶ÀÏ DVD¿ë ºÒ»çÁ¶ ¸÷ ¹øÈ£
	static	bool	IsPhoenix(DWORD vnum)				{ return 34001 == vnum; }
	static	bool	IsIcePhoenix(DWORD vnum)				{ return 34003 == vnum; }
	/// PetSystemÀÌ °ü¸®ÇÏ´Â ÆêÀÎ°¡?
	static	bool	IsPetUsingPetSystem(DWORD vnum)	{ return (IsPhoenix(vnum) || IsReindeerYoung(vnum)) || IsIcePhoenix(vnum); }

	/// 2011³â Å©¸®½º¸¶½º ÀÌº¥Æ®¿ë Æê (¾Æ±â ¼ø·Ï)
	static	bool	IsReindeerYoung(DWORD vnum)	{ return 34002 == vnum; }

	/// ¶ó¸¶´Ü ÀÌº¥Æ® º¸»ó¿ë Èæ¸¶(20119) .. ÇÒ·ÎÀ© ÀÌº¥Æ®¿ë ¶ó¸¶´Ü Èæ¸¶ Å¬·Ð(½ºÆåÀº °°À½, 20219)
	static	bool	IsRamadanBlackHorse(DWORD vnum)		{ return 20119 == vnum || 20219 == vnum || 22022 == vnum; }
};

class CVnumHelper
{
};


#endif	//__HEADER_VNUM_HELPER__
