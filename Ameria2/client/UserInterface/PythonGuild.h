#pragma once

#include "Packet.h"

class CPythonGuild : public CSingleton<CPythonGuild>
{
	public:
		enum
		{
			GUILD_SKILL_MAX_NUM = 12,
			ENEMY_GUILD_SLOT_MAX_COUNT = 6,
		};

		enum EGuildBuildingToken : BYTE
		{
			GUIILD_BUILDING_TOKEN_VNUM,
			GUIILD_BUILDING_TOKEN_TYPE,
			GUIILD_BUILDING_TOKEN_NAME,
			GUIILD_BUILDING_TOKEN_LOCAL_NAME,
			GUIILD_BUILDING_NO_USE_TOKEN_SIZE_1,
			GUIILD_BUILDING_NO_USE_TOKEN_SIZE_2,
			GUIILD_BUILDING_NO_USE_TOKEN_SIZE_3,
			GUIILD_BUILDING_NO_USE_TOKEN_SIZE_4,
			GUIILD_BUILDING_TOKEN_X_ROT_LIMIT,
			GUIILD_BUILDING_TOKEN_Y_ROT_LIMIT,
			GUIILD_BUILDING_TOKEN_Z_ROT_LIMIT,
			GUIILD_BUILDING_TOKEN_PRICE,
			GUIILD_BUILDING_TOKEN_MATERIAL,
			GUIILD_BUILDING_TOKEN_NPC,
			GUIILD_BUILDING_TOKEN_GROUP,
			GUIILD_BUILDING_TOKEN_DEPEND_GROUP,
			GUIILD_BUILDING_TOKEN_ENABLE_FLAG,
#if defined(ENABLE_CONQUEROR_LEVEL)
			GUIILD_BUILDING_TOKEN_NEW_WORLD,
#endif
			GUIILD_BUILDING_LIMIT_TOKEN_COUNT,
		};

		enum EGuildBuildingMaterialItem : DWORD
		{
			MATERIAL_STONE_ID = 90010,
			MATERIAL_LOG_ID = 90011,
			MATERIAL_PLYWOOD_ID = 90012
		};

		enum EGuildBuildingMaterialIndex : BYTE
		{
			MATERIAL_STONE_INDEX,
			MATERIAL_LOG_INDEX,
			MATERIAL_PLYWOOD_INDEX
		};

		typedef struct SGulidInfo
		{
			DWORD dwGuildID;
			char szGuildName[GUILD_NAME_MAX_LEN+1];
			DWORD dwMasterPID;
			DWORD dwGuildLevel;
			DWORD dwCurrentExperience;
			DWORD dwCurrentMemberCount;
			DWORD dwMaxMemberCount;
			DWORD dwGuildMoney;
			BOOL bHasLand;
		} TGuildInfo;

		typedef struct SGuildGradeData
		{
			SGuildGradeData(){}
			SGuildGradeData(BYTE byAuthorityFlag_, const char * c_szName_) : byAuthorityFlag(byAuthorityFlag_), strName(c_szName_) {}
			BYTE byAuthorityFlag;
			std::string strName;
		} TGuildGradeData;
		typedef std::map<BYTE, TGuildGradeData> TGradeDataMap;

		typedef struct SGuildMemberData
		{
			DWORD dwPID;

			std::string strName;
			BYTE byGrade;
			BYTE byJob;
			BYTE byLevel;
			BYTE byGeneralFlag;
			DWORD dwOffer;
		} TGuildMemberData;
		typedef std::vector<TGuildMemberData> TGuildMemberDataVector;

		typedef struct SGuildBoardCommentData
		{
			DWORD dwCommentID;
			std::string strName;
			std::string strComment;
		} TGuildBoardCommentData;
		typedef std::vector<TGuildBoardCommentData> TGuildBoardCommentDataVector;

		typedef struct SGuildSkillData
		{
			BYTE bySkillPoint;
			BYTE bySkillLevel[GUILD_SKILL_MAX_NUM];
			WORD wGuildPoint;
			WORD wMaxGuildPoint;
		} TGuildSkillData;

		typedef std::map<DWORD, std::string> TGuildNameMap;

	public:
		CPythonGuild();
		virtual ~CPythonGuild();

		void Destroy();

		void EnableGuild();
		void SetGuildMoney(DWORD dwMoney);
		void SetGuildEXP(BYTE byLevel, DWORD dwEXP);
		void SetGradeData(BYTE byGradeNumber, TGuildGradeData & rGuildGradeData);
		void SetGradeName(BYTE byGradeNumber, const char * c_szName);
		void SetGradeAuthority(BYTE byGradeNumber, BYTE byAuthority);
		void ClearComment();
		void RegisterComment(DWORD dwCommentID, const char * c_szName, const char * c_szComment);
		void RegisterMember(TGuildMemberData & rGuildMemberData);
		void ChangeGuildMemberGrade(DWORD dwPID, BYTE byGrade);
		void ChangeGuildMemberGeneralFlag(DWORD dwPID, BYTE byFlag);
		void RemoveMember(DWORD dwPID);
		void RegisterGuildName(DWORD dwID, const char * c_szName);

		BOOL IsMainPlayer(DWORD dwPID);
		BOOL IsGuildEnable();
		TGuildInfo & GetGuildInfoRef();
		BOOL GetGradeDataPtr(DWORD dwGradeNumber, TGuildGradeData ** ppData);
		const TGuildBoardCommentDataVector & GetGuildBoardCommentVector();
		DWORD GetMemberCount();
		BOOL GetMemberDataPtr(DWORD dwIndex, TGuildMemberData ** ppData);
		BOOL GetMemberDataPtrByPID(DWORD dwPID, TGuildMemberData ** ppData);
		BOOL GetMemberDataPtrByName(const char * c_szName, TGuildMemberData ** ppData);
		DWORD GetGuildMemberLevelSummary();
		DWORD GetGuildMemberLevelAverage();
		DWORD GetGuildExperienceSummary();
		TGuildSkillData & GetGuildSkillDataRef();
		bool GetGuildName(DWORD dwID, std::string * pstrGuildName);
		DWORD GetGuildID();
		BOOL HasGuildLand();

		void StartGuildWar(DWORD dwEnemyGuildID);
		void EndGuildWar(DWORD dwEnemyGuildID);
		DWORD GetEnemyGuildID(DWORD dwIndex);
		BOOL IsDoingGuildWar();

	protected:
		void __CalculateLevelAverage();
		void __SortMember();
		BOOL __IsGradeData(BYTE byGradeNumber);

		void __Initialize();

	protected:
		TGuildInfo m_GuildInfo;
		TGradeDataMap m_GradeDataMap;
		TGuildMemberDataVector m_GuildMemberDataVector;
		TGuildBoardCommentDataVector m_GuildBoardCommentVector;
		TGuildSkillData m_GuildSkillData;
		TGuildNameMap m_GuildNameMap;
		DWORD m_adwEnemyGuildID[ENEMY_GUILD_SLOT_MAX_COUNT];

		DWORD m_dwMemberLevelSummary;
		DWORD m_dwMemberLevelAverage;
		DWORD m_dwMemberExperienceSummary;

		BOOL m_bGuildEnable;
};
