#include "../common/length.h"

class CHARACTER;

enum ERankByValue
{
	RANK_BY_LEVEL,
	RANK_BY_YANG,
	RANK_BY_STONE,
	RANK_BY_BOSS,
	RANK_BY_MONSTER,
	RANK_BY_DUNGEON_COMPLETE,
	RANK_BY_FISH_CAUGHT,
	RANK_BY_EXTRACTION,
	RANK_BY_BONUS_CHANGED,
	RANK_BY_BOX_OPENED,
	RANK_BY_ITEM_IMPROVED,
	RANK_BY_DEATH,

	RANK_MAX,
};

enum EConfigRankPlayer
{
	MAX_LIMIT_GET_RANK_PLAYER = 50, // limit maximum load players from mysql
	SHOW_MAX_LIMIT = 10, // show maximum 20 players in top. (2 pages)
};

struct SInfoPlayer
{
	bool bCached;
	bool bLoaded;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	uint8_t bEmpire, bLevel;
	int64_t lYang;
	int iStone, iBoss, iMonster, iDungeon, iFish, iExtraction, iBonus, iBox, iImprove, iDeath;
	char szGuildName[GUILD_NAME_MAX_LEN + 1];

	SInfoPlayer()
	{
		bCached = false;
		bLoaded = false;
		snprintf(szName, sizeof(szName), "%s", "-");
		bEmpire = 0;
		bLevel = 0; lYang = 0;
		iStone = 0; iBoss = 0; iMonster = 0;
		iDungeon = 0; iFish = 0; iExtraction = 0;
		iBonus = 0; iBox = 0; iImprove = 0;
		iDeath = 0;
		snprintf(szGuildName, sizeof(szGuildName), "%s", "-");
	}
};

class RankPlayer : public singleton<RankPlayer>
{
	public:
		RankPlayer();
		~RankPlayer();

		void	Initialize();
		void	LoadPlayerValue(int iMode, const char* szNameTable, const char* szWhereExcept, const char* szSelect, const char* szNameColumn);
		void	LoadInfoPlayer(uint32_t dwPID, bool bReload = false);
		int		GetEmpire(uint32_t dwAcc) const;
		void	ReceiveP2P(int iMode, uint32_t dwPid, const char* szName, uint8_t bEmpire, int64_t lValue, const char* szGuildName);
		void	SendInfoPlayer(LPCHARACTER ch, int iMode, int64_t lValue, bool bCanP2P = true);
		void	RequestInfoRank(LPCHARACTER ch, int iMode, int iFromPos);
		
		int64_t GetProgressByPID(uint32_t dwPid, int iMode);
		bool 	LoadAccountStatus(uint32_t dwPlayerID, bool& isBlocked);

		void	SetCachedStatusPlayer(uint32_t dwPID, bool bStatus);
		bool	IsCachedPlayer(uint32_t dwPID);
		bool 	IsPlayerInGmList(const char* playerName);

	protected:	
		std::map<uint32_t, SInfoPlayer> m_info_player;
};
