
#ifndef METIN2_TABLE_BY_SKILL_
#define METIN2_TABLE_BY_SKILL_

class CTableBySkill : public singleton<CTableBySkill>
{
	public:
		CTableBySkill()
			: m_aiSkillDamageByLevel(NULL)
		{
			//½ºÅ³ ·¹º§´ç Ãß°¡µ¥¹ÌÁö ÃÊ±âÈ­
			for ( int job = 0; job < JOB_MAX_NUM * 2; ++job )
				m_aiSkillPowerByLevelFromType[job] = NULL;
		}

		~CTableBySkill()
		{
			DeleteAll();
		}

		//Å×ÀÌºí ¼¼ÆÃ Ã¼Å©
		bool 	Check() const;

		//»èÁ¦
		void 	DeleteAll();

		//½ºÅ³·¹º§´ÜÀ§ ½ºÅ³ÆÄ¿ö Å×ÀÌºí
		int 	GetSkillPowerByLevelFromType( int job, int skillgroup, int skilllevel, bool bMob ) const;
		void 	SetSkillPowerByLevelFromType( int idx, const int* aTable );
		void	DeleteSkillPowerByLevelFromType( int idx );

		//·¹º§´ç Ãß°¡ ½ºÅ³µ¥¹ÌÁö  Å×ÀÌºí
		int 	GetSkillDamageByLevel( int Level ) const;
		void	SetSkillDamageByLevelTable( const int* aTable );
		void 	DeleteSkillDamageByLevelTable();

	private:
		int * m_aiSkillPowerByLevelFromType[JOB_MAX_NUM*2];
		int * m_aiSkillDamageByLevel;
};

#endif /* METIN2_TABLE_BY_SKILL_ */

