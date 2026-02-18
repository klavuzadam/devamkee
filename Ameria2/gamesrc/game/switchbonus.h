#include <boost/unordered_map.hpp>
#include "../common/stl.h"
#include "../common/tables.h"

#ifdef ENABLE_SWITCHBOT_WORLDARD
class CSwitchBonus : public singleton<CSwitchBonus>
{

	enum
	{
		VNUM_CHANGE_ITEM = 71084,
		VNUM_TALISMAN_CHANGE_ITEM = 71302,
		COUNT_CHANGE_ITEM = 2000,	
	};

	public:
		CSwitchBonus();
		virtual ~CSwitchBonus();

		bool CheckElementsSB(LPCHARACTER ch);
		bool CanMoveIntoSB(LPITEM item);
		void OpenSB(LPCHARACTER ch);
		void ClearSB(LPCHARACTER ch);
		void CloseSB(LPCHARACTER ch);
		void SendSBItem(LPCHARACTER ch,LPITEM item);
		void ChangeSB(LPCHARACTER ch,const TPlayerItemAttribute* aAttr);
		void SendSBPackets(LPCHARACTER ch,BYTE subheader, int id_bonus = 0, int value_bonus_0 = 0, int value_bonus_1 = 0, int value_bonus_2 = 0, int value_bonus_3 = 0, int value_bonus_4 = 0);

};
#endif
