#pragma once
// #define ENABLE_NEW_MOB_PROTO_STRUCT_20141125	// bleeding resistance 2014/11/25
// #define ENABLE_NEW_MOB_PROTO_STRUCT_20151020	// claw resistance 2015/10/20

/*
 *	NPC 데이터 프로토 타잎을 관리 한다.
 */

#ifdef ENABLE_NPC_WEAR_ITEM
enum
{
	NPC_WEAR_RACE,
	NPC_WEAR_WEAPON,
	NPC_WEAR_ARMOR,
	NPC_WEAR_HAIR,
	NPC_WEAR_ACCE,
	NPC_WEAR_MOUNT,
	NPC_WEAR_SHINING_WEAPON1,
	NPC_WEAR_SHINING_WEAPON2,
	NPC_WEAR_SHINING_WEAPON3,
	NPC_WEAR_SHINING_ARMOR1,
	NPC_WEAR_SHINING_ARMOR2,
	NPC_WEAR_SHINING_ARMOR3,
	NPC_WEAR_SHINING_SPECIAL,
	NPC_WEAR_MAX,
};
#endif

class CPythonNonPlayer : public CSingleton<CPythonNonPlayer>
{
	public:
		enum  EClickEvent
		{
			ON_CLICK_EVENT_NONE		= 0,
			ON_CLICK_EVENT_BATTLE	= 1,
			ON_CLICK_EVENT_SHOP		= 2,
			ON_CLICK_EVENT_TALK		= 3,
			ON_CLICK_EVENT_VEHICLE	= 4,

			ON_CLICK_EVENT_MAX_NUM,
		};

#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
		enum EAIFlags
		{
			AIFLAG_AGGRESSIVE	= (1 << 0),
			AIFLAG_NOMOVE	= (1 << 1),
			AIFLAG_COWARD	= (1 << 2),
			AIFLAG_NOATTACKSHINSU	= (1 << 3),
			AIFLAG_NOATTACKJINNO	= (1 << 4),
			AIFLAG_NOATTACKCHUNJO	= (1 << 5),
			AIFLAG_ATTACKMOB = (1 << 6 ),
			AIFLAG_BERSERK	= (1 << 7),
			AIFLAG_STONESKIN	= (1 << 8),
			AIFLAG_GODSPEED	= (1 << 9),
			AIFLAG_DEATHBLOW	= (1 << 10),
			AIFLAG_REVIVE		= (1 << 11),
		};
#endif

		enum EMobEnchants
		{
			MOB_ENCHANT_CURSE,
			MOB_ENCHANT_SLOW,
			MOB_ENCHANT_POISON,
			MOB_ENCHANT_STUN,
			MOB_ENCHANT_CRITICAL,
			MOB_ENCHANT_PENETRATE,
			MOB_ENCHANTS_MAX_NUM
		};
		enum EMobResists
		{
			MOB_RESIST_SWORD,
			MOB_RESIST_TWOHAND,
			MOB_RESIST_DAGGER,
			MOB_RESIST_BELL,
			MOB_RESIST_FAN,
			MOB_RESIST_BOW,
			MOB_RESIST_FIRE,
			MOB_RESIST_ELECT,
			MOB_RESIST_MAGIC,
			MOB_RESIST_WIND,
			MOB_RESIST_POISON,
			MOB_RESISTS_MAX_NUM
		};

		enum EMobRank
		{
			MOB_RANK_PAWN = 0,
			MOB_RANK_S_PAWN = 1,
			MOB_RANK_KNIGHT = 2,
			MOB_RANK_S_KNIGHT = 3,
			MOB_RANK_BOSS = 4,
			MOB_RANK_KING = 5,
		};
		enum EMobMaxNum
		{
			MOB_ATTRIBUTE_MAX_NUM = 12,
			MOB_SKILL_MAX_NUM = 5,
		};

#pragma pack(push)
#pragma pack(1)
		typedef struct SMobSkillLevel
		{
			DWORD       dwVnum;
			BYTE        bLevel;
		} TMobSkillLevel;

		typedef struct SMobTable_r235
		{
			enum EMobMaxNum
			{
				MOB_ATTRIBUTE_MAX_NUM = 12,
				MOB_SKILL_MAX_NUM = 1,//r1
			};

			DWORD       dwVnum;
			char        szName[CHARACTER_NAME_MAX_LEN + 1];
			char        szLocaleName[CHARACTER_NAME_MAX_LEN + 1];

			BYTE        bType;                  // Monster, NPC
			BYTE        bRank;                  // PAWN, KNIGHT, KING
			BYTE        bBattleType;            // MELEE, etc..
			BYTE        bLevel;                 // Level
			BYTE        bSize;

			DWORD       dwGoldMin;
			DWORD       dwGoldMax;
			DWORD       dwExp;
			DWORD       dwMaxHP;
			BYTE        bRegenCycle;
			BYTE        bRegenPercent;
			WORD        wDef;

			DWORD       dwAIFlag;
			DWORD       dwRaceFlag;
			DWORD       dwImmuneFlag;

			BYTE        bStr, bDex, bCon, bInt;
			DWORD       dwDamageRange[2];

			short       sAttackSpeed;
			short       sMovingSpeed;
			BYTE        bAggresiveHPPct;
			WORD        wAggressiveSight;
			WORD        wAttackRange;

			char        cEnchants[MOB_ENCHANTS_MAX_NUM];
			char        cResists[MOB_RESISTS_MAX_NUM];

			DWORD       dwResurrectionVnum;
			DWORD       dwDropItemVnum;

			BYTE        bMountCapacity;
			BYTE        bOnClickType;

			BYTE        bEmpire;
			char        szFolder[64 + 1];
			float       fDamMultiply;
			DWORD       dwSummonVnum;
			DWORD       dwDrainSP;
			DWORD		dwMonsterColor;
		    DWORD       dwPolymorphItemVnum;

			TMobSkillLevel	Skills[SMobTable_r235::MOB_SKILL_MAX_NUM];

		    BYTE		bBerserkPoint;
			BYTE		bStoneSkinPoint;
			BYTE		bGodSpeedPoint;
			BYTE		bDeathBlowPoint;
			BYTE		bRevivePoint;
		} TMobTable_r235;

		typedef struct SMobTable
		{
			DWORD			dwVnum;

			char			szName[CHARACTER_NAME_MAX_LEN + 1];
			char			szLocaleName[CHARACTER_NAME_MAX_LEN + 1];

			BYTE			bType;
			BYTE			bRank;
			BYTE			bBattleType;
			BYTE			bLevel;
			BYTE			bSize;

			DWORD			dwGoldMin;
			DWORD			dwGoldMax;
			DWORD			dwExp;
#ifdef ENABLE_YOHARA_SYSTEM
			DWORD			dwSungMaExp;
#endif
			DWORD			dwMaxHP;
			BYTE			bRegenCycle;
			BYTE			bRegenPercent;
			WORD			wDef;

			DWORD			dwAIFlag;
			DWORD			dwRaceFlag;
			DWORD			dwImmuneFlag;

			BYTE			bStr, bDex, bCon, bInt;
			DWORD			dwDamageRange[2];

			short			sAttackSpeed;
			short			sMovingSpeed;
			BYTE			bAggresiveHPPct;
			WORD			wAggressiveSight;
			WORD			wAttackRange;

			char			cEnchants[MOB_ENCHANTS_MAX_NUM];
			char			cResists[MOB_RESISTS_MAX_NUM];

			DWORD			dwResurrectionVnum;
			DWORD			dwDropItemVnum;

			BYTE			bMountCapacity;
			BYTE			bOnClickType;

			BYTE			bEmpire;
			char			szFolder[64 + 1];

			float			fDamMultiply;
			DWORD			dwSummonVnum;
			DWORD			dwDrainSP;
			DWORD			dwMobColor;
			DWORD			dwPolymorphItemVnum;

			TMobSkillLevel	Skills[MOB_SKILL_MAX_NUM];

			BYTE			bBerserkPoint;
			BYTE			bStoneSkinPoint;
			BYTE			bGodSpeedPoint;
			BYTE			bDeathBlowPoint;
			BYTE			bRevivePoint;
		} TMobTable;

#pragma pack(pop)

		typedef std::list<TMobTable *> TMobTableList;
		typedef std::map<DWORD, TMobTable *> TNonPlayerDataMap;


#ifdef ENABLE_MOB_SCALE_SYSTEM
		typedef struct SMobScaleTable
		{
			DWORD	dwMobNum;
			float	fx;
			float	fy;
			float	fz;
		} TMobScaleTable;
		
		enum EMobScale
		{
			MOBSCALETABLE_MOB_NUM,
			MOBSCALETABLE_X,
			MOBSCALETABLE_Y,
			MOBSCALETABLE_Z,
			MOBSCALETABLE_MAX_NUM,
		};		
#endif

#ifdef ENABLE_MOB_SCALE_SYSTEM
		typedef std::map<DWORD, TMobScaleTable *> TNonMobScaleDataMap;
#endif


	public:
		CPythonNonPlayer(void);
		virtual ~CPythonNonPlayer(void);

		void Clear();
		void Destroy();

		bool				LoadNonPlayerData(const char * c_szFileName);

#ifdef ENABLE_MOB_SCALE_SYSTEM
		bool				LoadMobScale(const char * c_szFileName);
#endif
#ifdef ENABLE_MOB_SCALE_SYSTEM
		const TMobScaleTable *	GetScaleTable(DWORD dwVnum);
#endif

		const TMobTable *	GetTable(DWORD dwVnum);
		bool				GetName(DWORD dwVnum, const char ** c_pszName);
		bool				GetInstanceType(DWORD dwVnum, BYTE* pbType);
		BYTE				GetEventType(DWORD dwVnum);
		BYTE				GetEventTypeByVID(DWORD dwVID);
		DWORD				GetMonsterColor(DWORD dwVnum);
		const char*			GetMonsterName(DWORD dwVnum);
		DWORD				GetMonsterType(DWORD dwVnum);
		DWORD				GetMonsterRank(DWORD dwVnum);
		#ifdef ENABLE_SEND_TARGET_INFO
		// TARGET_INFO
		DWORD				GetMonsterMaxHP(DWORD dwVnum);
		DWORD				GetMonsterRaceFlag(DWORD dwVnum);
		DWORD				GetMonsterLevel(DWORD dwVnum);
		DWORD				GetMonsterDamage1(DWORD dwVnum);
		DWORD				GetMonsterDamage2(DWORD dwVnum);
		DWORD				GetMonsterExp(DWORD dwVnum);
		float				GetMonsterDamageMultiply(DWORD dwVnum);
		DWORD				GetMonsterST(DWORD dwVnum);
		DWORD				GetMonsterDX(DWORD dwVnum);
		bool				IsMonsterStone(DWORD dwVnum);
		#endif
		
#ifdef ENABLE_WIKI
		DWORD				GetMonsterPrice1(DWORD dwVnum);
		DWORD				GetMonsterPrice2(DWORD dwVnum);
		TNonPlayerDataMap	GetMonsterData() {return m_NonPlayerDataMap;}
		char				GetMonsterResist(DWORD dwVnum, BYTE byResist);

#endif

	BYTE				GetMobRegenCycle(DWORD dwVnum);
	BYTE				GetMobRegenPercent(DWORD dwVnum);
	DWORD				GetMobGoldMin(DWORD dwVnum);
	DWORD				GetMobGoldMax(DWORD dwVnum);
	DWORD				GetMobResist(DWORD dwVnum, BYTE bResistNum);
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL) && !defined(ENABLE_SEND_TARGET_INFO)
		DWORD				GetMonsterLevel(DWORD dwVnum);
#endif

#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
		bool				IsAggressive(DWORD dwVnum);
#endif
#ifdef ENABLE_MOB_SCALE_SYSTEM
		bool				GetScale(DWORD dwVnum, float & fx, float & fy, float & fz);
#endif
		// Function for outer
		void				GetMatchableMobList(int iLevel, int iInterval, TMobTableList * pMobTableList);

	protected:
		TNonPlayerDataMap	m_NonPlayerDataMap;
#ifdef ENABLE_MOB_SCALE_SYSTEM
		TNonMobScaleDataMap	m_NonMobScaleDataMap;
#endif

#ifdef ENABLE_NPC_WEAR_ITEM
public:
	bool		LoadNPCWear(const char* szNPCWearFile);
	int			GetNPCWear(const int raceIndex, const BYTE wearIndex);
protected:
	std::map<std::string, std::map<DWORD, std::vector<int>>> m_mapnpcWear;
#endif

};