#ifndef METIN_II_SERVER_GAME_DAY_CHANGE_MGR_HPP
#define METIN_II_SERVER_GAME_DAY_CHANGE_MGR_HPP

#if VSTD_HAS_PRAGMA_ONCE
#pragma once
#endif

#ifdef USE_DAY_CHANGE_MGR
class CDayChangeMgr : public singleton<CDayChangeMgr>
{
	public:
		CDayChangeMgr();
		virtual ~CDayChangeMgr();

	private:
		int m_iDayNum;

	public:
		void Setup(const int v)
		{
			m_iDayNum = v;
		}

		void OnChange(const int v);

		int GetTodayNum() const
		{
			return m_iDayNum;
		};
};
#endif // USE_DAY_CHANGE_MGR
#endif // METIN_II_SERVER_GAME_DAY_CHANGE_MGR_HPP
