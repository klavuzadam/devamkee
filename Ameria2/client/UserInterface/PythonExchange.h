#pragma once

#include "Packet.h"

/*
 *	교환 창 관련
 */
class CPythonExchange : public CSingleton<CPythonExchange>
{
	public:
		enum
		{
#ifdef ENABLE_NEW_EXCHANGE_WINDOW
			EXCHANGE_ITEM_MAX_NUM = 24,
#else
			EXCHANGE_ITEM_MAX_NUM = 12,
#endif
		};

		typedef struct trade
		{
			char					name[CHARACTER_NAME_MAX_LEN + 1];
#if defined(ENABLE_LEVEL_IN_TRADE) || defined(ENABLE_NEW_EXCHANGE_WINDOW)
			DWORD					level;
#endif

			DWORD					item_vnum[EXCHANGE_ITEM_MAX_NUM];
			DWORD					item_count[EXCHANGE_ITEM_MAX_NUM];
			DWORD					item_metin[EXCHANGE_ITEM_MAX_NUM][ITEM_SOCKET_SLOT_MAX_NUM];
			TPlayerItemAttribute	item_attr[EXCHANGE_ITEM_MAX_NUM][ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_YOHARA_SYSTEM
			TPlayerItemApplyRandom	item_sungma_attr[EXCHANGE_ITEM_MAX_NUM][APPLY_RANDOM_SLOT_MAX_NUM];
#endif

#ifdef ENABLE_NEW_NAME_ITEM
			std::string				item_name[EXCHANGE_ITEM_MAX_NUM];
#endif
			BYTE					accept;
			long long				elk;
#ifdef USE_CHEQUE_CURRENCY
			long long cheque;
#endif
#ifdef ENABLE_NEW_EXCHANGE_WINDOW
			DWORD	race;
#endif
		} TExchangeData;

	public:
		CPythonExchange();
		virtual ~CPythonExchange();

		void			Clear();

		void			Start();
		void			End();
		bool			isTrading();

		// Interface

		void			SetSelfName(const char *name);
		void			SetTargetName(const char *name);

		char			*GetNameFromSelf();
		char			*GetNameFromTarget();

#if defined(ENABLE_LEVEL_IN_TRADE) || defined(ENABLE_NEW_EXCHANGE_WINDOW)
		void			SetSelfLevel(DWORD level);
		void			SetTargetLevel(DWORD level);

		DWORD			GetLevelFromSelf();
		DWORD			GetLevelFromTarget();
#endif
#ifdef ENABLE_NEW_EXCHANGE_WINDOW
		void	SetSelfRace(DWORD race);
		void	SetTargetRace(DWORD race);
		DWORD	GetRaceFromSelf();
		DWORD	GetRaceFromTarget();
#endif

#ifdef ENABLE_YOHARA_SYSTEM
		void SetItemApplyRandomToTarget(int pos, int iattrpos, BYTE wType, short sValue);
		void SetItemApplyRandomToSelf(int pos, int iattrpos, BYTE wType, short sValue);

		void GetItemApplyRandomFromTarget(BYTE pos, int iAttrPos, BYTE* pwType, short* psValue) const;
		void GetItemApplyRandomFromSelf(BYTE pos, int iAttrPos, BYTE* pwType, short* psValue) const;
#endif
		
		void SetElkToTarget(const long long lldValue
#ifdef USE_CHEQUE_CURRENCY
, const BYTE bSubHeader
#endif
		);
		void SetElkToSelf(const long long lldValue
#ifdef USE_CHEQUE_CURRENCY
, const BYTE bSubHeader
#endif
		);

		long long GetElkFromTarget() const;
		long long GetElkFromSelf() const;

#ifdef USE_CHEQUE_CURRENCY
		long long GetChequeFromTarget() const;
		long long GetChequeFromSelf() const;
#endif

#ifdef ENABLE_NEW_NAME_ITEM
		void			SetItemNewName(int iPos, const char* name, bool bSelf);
		const char*		GetItemNewName(int iPos, bool bSelf);
#endif
		void			SetItemToTarget(DWORD pos, DWORD vnum, DWORD count);
		void			SetItemToSelf(DWORD pos, DWORD vnum, DWORD count);

		void			SetItemMetinSocketToTarget(int pos, int imetinpos, DWORD vnum);
		void			SetItemMetinSocketToSelf(int pos, int imetinpos, DWORD vnum);

		void			SetItemAttributeToTarget(int pos, int iattrpos, BYTE byType, short sValue);
		void			SetItemAttributeToSelf(int pos, int iattrpos, BYTE byType, short sValue);


		void			DelItemOfTarget(BYTE pos);
		void			DelItemOfSelf(BYTE pos);

		DWORD			GetItemVnumFromTarget(BYTE pos);
		DWORD			GetItemVnumFromSelf(BYTE pos);

		DWORD			GetItemCountFromTarget(BYTE pos);
		DWORD			GetItemCountFromSelf(BYTE pos);

		DWORD			GetItemMetinSocketFromTarget(BYTE pos, int iMetinSocketPos);
		DWORD			GetItemMetinSocketFromSelf(BYTE pos, int iMetinSocketPos);

		void			GetItemAttributeFromTarget(BYTE pos, int iAttrPos, BYTE * pbyType, short * psValue);
		void			GetItemAttributeFromSelf(BYTE pos, int iAttrPos, BYTE * pbyType, short * psValue);

		void			SetAcceptToTarget(BYTE Accept);
		void			SetAcceptToSelf(BYTE Accept);

		bool			GetAcceptFromTarget();
		bool			GetAcceptFromSelf();

		bool			GetElkMode();
		void			SetElkMode(bool value);

	protected:
		bool				m_isTrading;

		bool				m_elk_mode;   // 엘크를 클릭해서 교환했을때를 위한 변종임.
		TExchangeData		m_self;
		TExchangeData		m_victim;
};
