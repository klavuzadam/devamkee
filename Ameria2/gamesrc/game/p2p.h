
#ifndef P2P_MANAGER_H_
#define P2P_MANAGER_H_

#include <boost/unordered_map.hpp>

#include "input.h"
#include "../common/stl.h"

typedef struct _CCI
{
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	dwPID;
	BYTE	bEmpire;
	long	lMapIndex;
	BYTE	bChannel;
	LPDESC	pkDesc;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	BYTE	lang;
#endif
} CCI;

class P2P_MANAGER : public singleton<P2P_MANAGER>
{
	public:
		P2P_MANAGER();
		~P2P_MANAGER();

		// ¾Æ·¡ Register* Unregister* pairµéÀº ³»ºÎÀûÀ¸·Î »ç½Ç °°Àº ·çÆ¾À» »ç¿ëÇÑ´Ù.
		// ´ÜÁö ¸í½ÃÀûÀ¸·Î Ç¥½ÃÇÏ±â À§ÇÑ °Í
		void			RegisterAcceptor(LPDESC d);
		void			UnregisterAcceptor(LPDESC d);

		void			RegisterConnector(LPDESC d);
		void			UnregisterConnector(LPDESC d);

		void			EraseUserByDesc(LPDESC d);	// ÇØ´ç desc¿¡ ÀÖ´Â À¯ÀúµéÀ» Áö¿î´Ù.

		void			FlushOutput();

		void			Boot(LPDESC d);	// p2p Ã³¸®¿¡ ÇÊ¿äÇÑ Á¤º¸¸¦ º¸³»ÁØ´Ù. (Àü Ä³¸¯ÅÍÀÇ ·Î±×ÀÎ Á¤º¸ µî)

		void			Send(const void * c_pvData, int iSize, LPDESC except = NULL);

		void			Login(LPDESC d, const TPacketGGLogin * p);
		#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		void			LoginEx(LPDESC d, const TPacketGGLoginEx * p);
		#endif
		void			Logout(const char * c_pszName);

		CCI *			Find(const char * c_pszName);
		CCI *			FindByPID(DWORD pid);

		int				GetCount();
		int				GetEmpireUserCount(int idx);
		int				GetDescCount();
		void			GetP2PHostNames(std::string& hostNames);

	private:
		void			Logout(CCI * pkCCI);

		CInputProcessor *	m_pkInputProcessor;
		int			m_iHandleCount;

		typedef boost::unordered_map<std::string, CCI *, stringhash> TCCIMap;
		typedef boost::unordered_map<DWORD, CCI*> TPIDCCIMap;

		TR1_NS::unordered_set<LPDESC> m_set_pkPeers;
		TCCIMap			m_map_pkCCI;
		TPIDCCIMap		m_map_dwPID_pkCCI;
		int			m_aiEmpireUserCount[EMPIRE_MAX_NUM];
};

#endif /* P2P_MANAGER_H_ */

