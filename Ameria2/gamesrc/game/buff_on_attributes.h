#ifndef __METIN2_BUFF_ON_ATTRIBUTES_H
#define __METIN2_BUFF_ON_ATTRIBUTES_H

class CHARACTER;

class CBuffOnAttributes
{
public:
	CBuffOnAttributes(LPCHARACTER pOwner, BYTE m_point_type, std::vector <WORD>* vec_buff_targets);
	~CBuffOnAttributes();

	// ÀåÂø Áß ÀÌ¸é¼­, m_p_vec_buff_wear_targets¿¡ ÇØ´çÇÏ´Â ¾ÆÀÌÅÛÀÎ °æ¿ì, ÇØ´ç ¾ÆÀÌÅÛÀ¸·Î ÀÎÇØ ºÙÀº È¿°ú¸¦ Á¦°Å.
	void RemoveBuffFromItem(LPITEM pItem);
	// m_p_vec_buff_wear_targets¿¡ ÇØ´çÇÏ´Â ¾ÆÀÌÅÛÀÎ °æ¿ì, ÇØ´ç ¾ÆÀÌÅÛÀÇ attribute¿¡ ´ëÇÑ È¿°ú Ãß°¡.
	void AddBuffFromItem(LPITEM pItem);
	// m_bBuffValue¸¦ ¹Ù²Ù°í, ¹öÇÁÀÇ °ªµµ ¹Ù²Þ.
	void ChangeBuffValue(BYTE bNewValue);

	// m_p_vec_buff_wear_targets¿¡ ÇØ´çÇÏ´Â ¸ðµç ¾ÆÀÌÅÛÀÇ attribute¸¦ typeº°·Î ÇÕ»êÇÏ°í,
	// ±× attributeµéÀÇ (m_bBuffValue)% ¸¸Å­À» ¹öÇÁ·Î ÁÜ.
	bool On(BYTE bValue);
	// ¹öÇÁ Á¦°Å ÈÄ, ÃÊ±âÈ­
	void Off();

	void Initialize();
private:
	LPCHARACTER m_pBuffOwner;
	BYTE m_bPointType;
	BYTE m_bBuffValue;
	std::vector <WORD>* m_p_vec_buff_wear_targets;

	// apply_type, apply_value Æä¾îÀÇ ¸Ê
	typedef std::map <BYTE, int> TMapAttr;
	// m_p_vec_buff_wear_targets¿¡ ÇØ´çÇÏ´Â ¸ðµç ¾ÆÀÌÅÛÀÇ attribute¸¦ typeº°·Î ÇÕ»êÇÏ¿© °¡Áö°í ÀÖÀ½.
	TMapAttr m_map_additional_attrs;

};

#endif
