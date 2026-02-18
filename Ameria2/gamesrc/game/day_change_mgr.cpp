#include "stdafx.h"

#ifdef USE_AUTOMATIC_HUNTING_RENEWED
#include "day_change_mgr.hpp"
#include "desc_manager.h"
#include "desc.h"
#include "char.h"
#endif

#ifdef USE_DAY_CHANGE_MGR
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
struct FUpdateAutoHuntUsedTime
{
	int m_iDayNum;

	FUpdateAutoHuntUsedTime(int iDayNum) : m_iDayNum(iDayNum)
	{
		
	}

	void operator() (LPDESC d)
	{
		LPCHARACTER ch = d->GetCharacter();
		if (!ch)
		{
			return;
		}

#ifdef USE_PREMIUM_AFFECT
		if (!ch->FindAffect(AFFECT_PREMIUM_USER))
#else
		if (!ch->FindAffect(AFFECT_AUTO_HUNT))
#endif
		{
			return;
		}

		const int iLastDay = ch->GetAutoHuntLastDay();

		if (iLastDay == 0
			 || m_iDayNum != iLastDay)
		{
			ch->SetAutoHuntLastDay(m_iDayNum);
			ch->SetAutoHuntUsedTime(0);
		}

		ch->ChatPacket(CHAT_TYPE_COMMAND, "autohunt_stat %d", ch->GetAutoHuntUsedTime());
		ch->Save();
	}
};
#endif

void CDayChangeMgr::OnChange(const int v)
{
	if (v == m_iDayNum)
	{
		return;
	}

	m_iDayNum = v;

#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_RENEWED)
	const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::Instance().GetClientSet();
	std::for_each(c_ref_set.begin(), c_ref_set.end(), FUpdateAutoHuntUsedTime(m_iDayNum));
#endif
}

CDayChangeMgr::CDayChangeMgr() : m_iDayNum(0)
{
	
}

CDayChangeMgr::~CDayChangeMgr()
{
	
}
#endif // USE_DAY_CHANGE_MGR
