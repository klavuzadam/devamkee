#ifdef __FAKE_PLAYER__
#include <memory>
#include "../common/tables.h"
#include "../common/length.h"

enum EFakePlayer
{
	FAKE_PLAYER_LIVE_TIME 	= 10 * 60 * 1000,
	FAKE_PLAYER_MOV_SPEED  	= 200,
};

class CFakePlayer 
{		
	public:
		CFakePlayer(TFakePlayerLoginData* fakePlayerLoginData);
		~CFakePlayer(); 

		void 			Login(TFakePlayerLoginData* fakePlayerLoginData);
		bool 			LoadFakePlayer();
		void 			LoadFakePlayerItem(const char* szName);
		LPCHARACTER		GetFakePlayer() { return m_pFakePlayer; }
		
	private:
		LPCHARACTER		m_pFakePlayer;
};

class CFakePlayerManager: public singleton<CFakePlayerManager>
{		
	typedef std::map<std::string, std::unique_ptr<CFakePlayer>> TFakePlayerMap;
	
	public:		
		void 			Initialize();
		void 			Destroy();
		
		void 			DGFakePlayer(uint32_t dwHandle, const char* c_pData);
		
		void 			FakePlayerLogin(LPCHARACTER ch, TFakePlayerLoginData* fakePlayerLoginData);
		void 			FakePlayerLogout(const char* szName, LPCHARACTER ch = nullptr);

		void			SpawnFakePlayer(TFakePlayerLoginData* fakePlayerLoginData);
		void 			DestroyFakePlayer(TFakePlayerLogoutData* fakePlayerLogoutData);
		
		LPCHARACTER		FindFakePlayer(std::string stName);

	protected:
		uint32_t		m_dwLastSummonTime;
		TFakePlayerMap	m_mapFakePlayer;
};
#endif
