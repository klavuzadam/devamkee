#pragma once

#ifdef __FAKE_PLAYER__
#include <memory>
#include "Peer.h"

class CFakePlayerManager : public singleton<CFakePlayerManager>
{
	typedef std::map<std::string, long> TFakePlayerMap;
	
	public:
		void 	Initialize();
		void 	GDFakePlayer(CPeer* peer, uint32_t dwHandle, const char* data);
		
		void 	FakePlayerLogin(CPeer* peer, uint32_t dwHandle, TFakePlayerLoginData* fakePlayerLoginData);
		void 	FakePlayerLogout(CPeer* peer, uint32_t dwHandle, TFakePlayerLogoutData* fakePlayerLogoutData);
		
	private:
		TFakePlayerMap	m_mapFakePlayer;
};
#endif