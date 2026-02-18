
#define OXEVENT_MAP_INDEX 113

struct tag_Quiz
{
	char level;
	char Quiz[256];
	bool answer;
};

enum OXEventStatus
{
	OXEVENT_FINISH = 0, // OXÀÌº¥Æ®°¡ ¿ÏÀüÈ÷ ³¡³­ »óÅÂ
	OXEVENT_OPEN = 1,	// OXÀÌº¥Æ®°¡ ½ÃÀÛµÊ. À»µÎÁö(20012)¸¦ ÅëÇØ¼­ ÀÔÀå°¡´É
	OXEVENT_CLOSE = 2,	// OXÀÌº¥Æ®ÀÇ Âü°¡°¡ ³¡³². À»µÎÁö(20012)¸¦ ÅëÇÑ ÀÔÀåÀÌ Â÷´ÜµÊ
	OXEVENT_QUIZ = 3,	// ÄûÁî¸¦ ÃâÁ¦ÇÔ.

	OXEVENT_ERR = 0xff
};
#ifdef ENABLE_NEW_OX_FEATURES
struct SEventItem
{
	DWORD dwVnum;
	DWORD dwCount;
};
#endif
class COXEventManager : public singleton<COXEventManager>
{
	private :
		std::map<DWORD, DWORD> m_map_char;
		std::map<DWORD, DWORD> m_map_attender;
		std::map<DWORD, DWORD> m_map_miss;

		std::vector<std::vector<tag_Quiz> > m_vec_quiz;

		LPEVENT m_timedEvent;

	protected :
		bool CheckAnswer();

		bool EnterAudience(LPCHARACTER pChar);
		bool EnterAttender(LPCHARACTER pChar);

	public :
		bool Initialize();
		void Destroy();

		OXEventStatus GetStatus();
		void SetStatus(OXEventStatus status);

		bool LoadQuizScript(const char* szFileName);

		bool Enter(LPCHARACTER pChar);

		bool CloseEvent();

		void ClearQuiz();
		bool AddQuiz(unsigned char level, const char* pszQuestion, bool answer);
		bool ShowQuizList(LPCHARACTER pChar);

		bool Quiz(unsigned char level, int timelimit);
		bool GiveItemToAttender(DWORD dwItemVnum, DWORD count);

		bool CheckAnswer(bool answer);
		void WarpToAudience();

		bool LogWinner();

		DWORD GetAttenderCount() { return m_map_attender.size(); }
};

