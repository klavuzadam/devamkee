#pragma once

#include "../eterLib/FuncObject.h"
#include "../eterlib/NetStream.h"
#include "../eterLib/NetPacketHeaderMap.h"

#include "InsultChecker.h"

#include "packet.h"
#ifdef ENABLE_SWITCHBOT
#include "PythonSwitchbot.h"
#endif

class CInstanceBase;
class CNetworkActorManager;
struct SNetworkActorData;
struct SNetworkUpdateActorData;

class CPythonNetworkStream : public CNetworkStream, public CSingleton<CPythonNetworkStream>
{
	public:
		enum
		{
			SERVER_COMMAND_LOG_OUT = 0,
			SERVER_COMMAND_RETURN_TO_SELECT_CHARACTER = 1,
			SERVER_COMMAND_QUIT = 2,

			MAX_ACCOUNT_PLAYER
		};

		enum
		{
			ERROR_NONE,
			ERROR_UNKNOWN,
			ERROR_CONNECT_MARK_SERVER,
			ERROR_LOAD_MARK,
			ERROR_MARK_WIDTH,
			ERROR_MARK_HEIGHT,

			// MARK_BUG_FIX
			ERROR_MARK_UPLOAD_NEED_RECONNECT,
			ERROR_MARK_CHECK_NEED_RECONNECT,
			// END_OF_MARK_BUG_FIX
		};

		enum
		{
			ACCOUNT_CHARACTER_SLOT_ID,
			ACCOUNT_CHARACTER_SLOT_NAME,
			ACCOUNT_CHARACTER_SLOT_RACE,
			ACCOUNT_CHARACTER_SLOT_LEVEL,
			ACCOUNT_CHARACTER_SLOT_STR,
			ACCOUNT_CHARACTER_SLOT_DEX,
			ACCOUNT_CHARACTER_SLOT_HTH,
			ACCOUNT_CHARACTER_SLOT_INT,
			ACCOUNT_CHARACTER_SLOT_PLAYTIME,
			ACCOUNT_CHARACTER_SLOT_FORM,
			ACCOUNT_CHARACTER_SLOT_ADDR,
			ACCOUNT_CHARACTER_SLOT_PORT,
			ACCOUNT_CHARACTER_SLOT_GUILD_ID,
			ACCOUNT_CHARACTER_SLOT_GUILD_NAME,
			ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG,
			ACCOUNT_CHARACTER_SLOT_HAIR,
			#ifdef ENABLE_SASH_SYSTEM
			ACCOUNT_CHARACTER_SLOT_SASH,
			#endif
#ifdef ENABLE_YOHARA_SYSTEM
			ACCOUNT_CHARACTER_SLOT_CONQUEROR_LEVEL,
			ACCOUNT_CHARACTER_SLOT_SUNGMA_ST,
			ACCOUNT_CHARACTER_SLOT_SUNGMA_HP,
			ACCOUNT_CHARACTER_SLOT_SUNGMA_MOVE,
			ACCOUNT_CHARACTER_SLOT_SUNGMA_IMMUNE,
#endif
#ifdef NEW_SELECT_CHARACTER
			ACCOUNT_CHARACTER_SLOT_LAST_PLAYTIME,
#endif
		};

		enum
		{
			PHASE_WINDOW_LOGO,
			PHASE_WINDOW_LOGIN,
			PHASE_WINDOW_SELECT,
			PHASE_WINDOW_CREATE,
			PHASE_WINDOW_LOAD,
			PHASE_WINDOW_GAME,
			PHASE_WINDOW_EMPIRE,
			PHASE_WINDOW_NUM,
		};

	public:
		CPythonNetworkStream();
		virtual ~CPythonNetworkStream();

		bool SendSpecial(int nLen, void * pvBuf);

		void StartGame();
		void Warp(long lGlobalX, long lGlobalY);

		void NotifyHack(const char* c_szMsg);
		void SetWaitFlag();

#if defined(ENABLE_DISCORD_RPC)
		void Discord_Start();
		void Discord_Close();
		void Discord_Update(const bool ingame);
#endif
		
		void SendEmoticon(UINT eEmoticon);

		void ExitApplication();
		void ExitGame();
		void LogOutGame();
		void AbsoluteExitGame();
		void AbsoluteExitApplication();

		void EnableChatInsultFilter(bool isEnable);
		bool IsChatInsultIn(const char* c_szMsg);
		bool IsInsultIn(const char* c_szMsg);

		DWORD GetGuildID();

		UINT UploadMark(const char* c_szImageFileName);
		UINT UploadSymbol(const char* c_szImageFileName);

		bool LoadInsultList(const char* c_szInsultListFileName);
		bool LoadConvertTable(DWORD dwEmpireID, const char* c_szFileName);

		UINT		GetAccountCharacterSlotDatau(UINT iSlot, UINT eType);
		const char* GetAccountCharacterSlotDataz(UINT iSlot, UINT eType);
		void		UpdateAccountCharacterSlotHair(UINT iHair);

		// SUPPORT_BGM
		const char*		GetFieldMusicFileName();
		float			GetFieldMusicVolume();
		// END_OF_SUPPORT_BGM

#ifdef NEW_SELECT_CHARACTER
		const char* GetLoginID();
#endif

		bool IsSelectedEmpire();

		void ToggleGameDebugInfo();

		void SetMarkServer(const char* c_szAddr, UINT uPort);
		void ConnectLoginServer(const char* c_szAddr, UINT uPort);
		void ConnectGameServer(UINT iChrSlot);

		void SetLoginInfo(const char* c_szID, const char* c_szPassword);
		void SetLoginKey(DWORD dwLoginKey);
		void ClearLoginInfo( void );

		void SetHandler(PyObject* poHandler);
		void SetPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd);
		void ClearPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd);
		void SetServerCommandParserWindow(PyObject* poPhaseWnd);

		bool SendSyncPositionElementPacket(DWORD dwVictimVID, DWORD dwVictimX, DWORD dwVictimY);

		bool SendAttackPacket(UINT uMotAttack, DWORD dwVIDVictim);
		bool SendCharacterStatePacket(const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg);
		bool SendUseSkillPacket(DWORD dwSkillIndex, DWORD dwTargetVID = 0
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, bool bFromAutoHunt = false
#endif
		);
		bool SendTargetPacket(DWORD dwVID);
		// OLDCODE:
		bool SendCharacterStartWalkingPacket(float fRotation, long lx, long ly);
		bool SendCharacterEndWalkingPacket(float fRotation, long lx, long ly);
		bool SendCharacterCheckWalkingPacket(float fRotation, long lx, long ly);

		bool SendCharacterPositionPacket(BYTE iPosition);

		bool SendItemUsePacket(TItemPos pos);
		bool SendItemUseToItemPacket(TItemPos source_pos, TItemPos target_pos);
		bool SendItemDropPacket(TItemPos pos, DWORD elk);
		bool SendItemDropPacketNew(TItemPos pos, DWORD elk, DWORD count);
		bool SendItemMovePacket(TItemPos pos, TItemPos change_pos, DWORD num);
		bool SendItemPickUpPacket(DWORD vid);

#ifdef ENABLE_SELL_ITEM
		bool SendItemSellPacket(TItemPos pos, DWORD elk);
#endif

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
		bool SendItemsPickUpPacket(std::vector<DWORD> itemVector);
#endif


		bool SendQuickSlotAddPacket(BYTE bPos, BYTE type, WORD wPos);
		bool SendQuickSlotDelPacket(BYTE wpos);
		bool SendQuickSlotMovePacket(BYTE wpos, BYTE change_pos);

		// PointReset 개 임시
		bool SendPointResetPacket();

#if defined(__BL_BATTLE_ROYALE__)
		bool SendBattleRoyaleApplication();
		bool SendBattleRoyaleApplicationCancel();
		bool SendBattleRoyaleExit();
		bool SendBattleRoyaleRanking();
		bool SendBattleRoyaleStart(bool bUseSpecialItem);
		bool SendBattleRoyaleClaimReward();
#endif

		// Shop
		bool SendShopEndPacket();
		bool SendShopBuyPacket(BYTE byCount);
		bool SendShopSellPacket(BYTE bySlot);

#ifdef USE_CAPTCHA_SYSTEM
        bool RecvCaptcha();
        bool SendCaptcha(uint16_t code);
        void RecvCloseClient();
#endif

#ifdef ENABLE_SPECIAL_STORAGE
		bool SendShopSellPacketNew(BYTE byInv, WORD wSlot, DWORD byCount);
#else
		bool SendShopSellPacketNew(WORD wSlot, DWORD byCount);
#endif
#ifdef ENABLE_EXCHANGE_LOG
		bool RecvExchangeLog();
#endif

		// Exchange
		bool SendExchangeStartPacket(DWORD vid);
		bool SendExchangeItemAddPacket(TItemPos ItemPos, BYTE byDisplayPos);
		bool SendExchangeElkAddPacket(const long long lldValue
#ifdef USE_CHEQUE_CURRENCY
, const BYTE bSubHeader
#endif
		);
		bool SendExchangeItemDelPacket(BYTE pos);
		bool SendExchangeAcceptPacket();
		bool SendExchangeExitPacket();

		// Quest
		bool SendScriptAnswerPacket(int iAnswer);
		bool SendScriptButtonPacket(unsigned int iIndex);
		bool SendAnswerMakeGuildPacket(const char * c_szName);
		bool SendQuestInputStringPacket(const char * c_szString);
		bool SendQuestInputStringLongPacket(const char * c_pszString);
		bool SendQuestConfirmPacket(BYTE byAnswer, DWORD dwPID);

#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
		bool RecvHitCountInfo();
#endif

		// Event
		bool SendOnClickPacket(DWORD vid);
#ifdef ENABLE_MINI_GAME_CATCH_KING
		bool SendMiniGameCatchKingStart(int ibetNumber);
		bool SendMiniGameCatchKingDeckCardClick();
		bool SendMiniGameCatchKingFieldCardClick(int icardNumber);
		bool SendMiniGameCatchKingReward();
		bool RecvMiniGameCatchKingPacket();
		bool SendOpenCatchKing();
#endif

#ifdef ENABLE_ENTITY_LOADING
		// Entity
		bool RecvEntity();
#endif

		// Fly
		bool SendFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget);
		bool SendAddFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget);
		bool SendShootPacket(UINT uSkill);

		// Command
		bool ClientCommand(const char * c_szCommand);
		void ServerCommand(char * c_szCommand);

		// Emoticon
		void RegisterEmoticonString(const char * pcEmoticonString);

		// Party
		bool SendPartyInvitePacket(DWORD dwVID);
		bool SendPartyInviteAnswerPacket(DWORD dwLeaderVID, BYTE byAccept);
		bool SendPartyRemovePacket(DWORD dwPID);
		bool SendPartySetStatePacket(DWORD dwVID, BYTE byState, BYTE byFlag);
		bool SendPartyUseSkillPacket(BYTE bySkillIndex, DWORD dwVID);
		bool SendPartyParameterPacket(BYTE byDistributeMode);

		// SafeBox
		bool SendSafeBoxMoneyPacket(BYTE byState, DWORD dwMoney);
		bool SendSafeBoxCheckinPacket(TItemPos InventoryPos, BYTE bySafeBoxPos);
		bool SendSafeBoxCheckoutPacket(BYTE bySafeBoxPos, TItemPos InventoryPos);
		bool SendSafeBoxItemMovePacket(BYTE bySourcePos, BYTE byTargetPos, DWORD byCount);

		// Mall
		bool SendMallCheckoutPacket(BYTE byMallPos, TItemPos InventoryPos);

		// Guild
		bool SendGuildAddMemberPacket(DWORD dwVID);
		bool SendGuildRemoveMemberPacket(DWORD dwPID);
		bool SendGuildChangeGradeNamePacket(BYTE byGradeNumber, const char * c_szName);
		bool SendGuildChangeGradeAuthorityPacket(BYTE byGradeNumber, BYTE byAuthority);
		bool SendGuildOfferPacket(DWORD dwExperience);
		bool SendGuildPostCommentPacket(const char * c_szMessage);
		bool SendGuildDeleteCommentPacket(DWORD dwIndex);
		bool SendGuildRefreshCommentsPacket(DWORD dwHighestIndex);
		bool SendGuildChangeMemberGradePacket(DWORD dwPID, BYTE byGrade);
		bool SendGuildUseSkillPacket(DWORD dwSkillID, DWORD dwTargetVID);
		bool SendGuildChangeMemberGeneralPacket(DWORD dwPID, BYTE byFlag);
		bool SendGuildInvitePacket(DWORD dwVID);
		bool SendGuildInviteAnswerPacket(DWORD dwGuildID, BYTE byAnswer);
		bool SendGuildChargeGSPPacket(DWORD dwMoney);
		bool SendGuildDepositMoneyPacket(DWORD dwMoney);
		bool SendGuildWithdrawMoneyPacket(DWORD dwMoney);
#ifdef NEW_PET_SYSTEM
		bool PetSetNamePacket(const char * petname);
#endif

#ifdef ENABLE_CALENDAR_SYSTEM
		bool SendCalendarSystemOpen();
		bool SendCalendarSystemClose();
		bool SendCalendarSystem(int day, int index_event, const char* hora_inicio, const char* hora_final);
#endif

		// Mall
		bool RecvMallOpenPacket();
		bool RecvMallItemSetPacket();
		bool RecvMallItemDelPacket();


#ifdef ENABLE_IKASHOP_RENEWAL
		bool RecvOfflineshopPacket();

		bool RecvOfflineshopShopOpen();
		bool RecvOfflineshopShopOpenOwner();
		bool RecvOfflineshopShopOpenOwnerNoShop();
		bool RecvOfflineshopShopExpiredGuesting();
		bool RecvOfflineshopShopRemoveItem(bool owner);

		bool RecvOfflineshopShopGuestEditItem();
		bool RecvOfflineshopShopOwnerEditItem();


		bool RecvOfflineshopShopFilterResult();
		bool RecvOfflineshopOfferList();
		bool RecvOfflineshopShopSafeboxRefresh();
#ifdef USE_IKASHOP_UI_RENEWED
		bool RecvOfflineshopShopSafeboxRefreshValutes();
#endif
		bool RecvOfflineshopShopSafeboxRemoveItem();
		bool RecvOfflineshopShopSafeboxAddItem();

		bool RecvOfflineshopAuctionList();
		bool RecvOfflineshopOpenMyAuction();
		bool RecvOfflineshopOpenMyAuctionNoAuction();
		bool RecvOfflineshopOpenAuction();

		bool RecvOfflineshopPopupMessage();

		bool RecvOfflineshopBoardCounters();
#ifdef EXTEND_IKASHOP_PRO
		bool RecvOfflineshopNotificationList();
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		bool RecvOfflineshopPriceAverageResponse();
#endif
#ifdef ENABLE_IKASHOP_ENTITIES
		bool RecvOfflineshopInsertEntity();
		bool RecvOfflineshopRemoveEntity();

		void SendIkarusShopOnClickShopEntity(DWORD dwPickedShopVID);
#if defined(ENABLE_IKASHOP_ENTITIES) && !defined(USE_IKASHOP_UI_RENEWED)
		void SendIkarusShopMoveShopEntity();
#endif
#ifdef USE_IKASHOP_UI_RENEWED
		void SendTeleportToShopEntity(DWORD dwPID);
#endif

		bool RecvOfflineshopSearchResultDelete();
#endif



		void SendIkarusShopCreateShop();
		void SendIkarusShopForceCloseShop();

		void SendIkarusShopRequestShopList();

		void SendIkarusShopOpenShop(DWORD dwOwnerID);
		void SendIkarusShopOpenShopOwner();

		void SendIkarusShopBuyItem(DWORD dwOwnerID, DWORD dwItemID, bool isSearch, long long TotalPriceSeen); //patch seen price check

		void SendIkarusShopAddItem(ikashop::TShopItemInfo& itemInfo);
		void SendIkarusShopRemoveItem(DWORD dwItemID);
		void SendOfflineShopEditItem(DWORD dwItemID, const ikashop::TPriceInfo& price);

		void SendIkarusShopFilterRequest(const ikashop::TFilterInfo& filter);
		void SendIkarusShopSearchFillRequest();

		void SendIkarusShopOfferCreate(const ikashop::TOfferInfo& offer);
		void SendIkarusShopOfferAccept(DWORD dwOfferID);
		void SendIkarusShopOfferCancel(DWORD dwOfferID, DWORD dwOwnerID);
		void SendIkarusShopOfferListRequest();

		void SendIkarusShopSafeboxOpen();
		void SendIkarusShopSafeboxGetItem(DWORD dwItemID);
		void SendIkarusShopSafeboxGetValutes();
		void SendIkarusShopSafeboxClose();

		//AUCTION
		void SendIkarusShopAuctionListRequest();
		void SendIkarusShopAuctionOpen(DWORD dwOwnerID);
		void SendIkarusShopAuctionAddOffer(DWORD dwOwnerID, const ikashop::TPriceInfo& price);
		void SendIkarusShopAuctionExitFrom(DWORD dwOwnerID);
		void SendIkarusShopAuctionCreate(const TItemPos& pos, const ikashop::TPriceInfo& price);
		void SendIkarusShopMyAuctionOpen();
		void SendIkarusShopMyAuctionClose();
		void SendIkarusShopMyAuctionCancel();

		void SendIkarusMyShopCloseBoard();
		void SendIkarusOffersListClose();
		void SendIkarusCloseShopGuestBoard();

#ifdef EXTEND_IKASHOP_PRO
		void SendIkarusNotificationListRequest();
		void SendIkarusNotificationListClose();
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		void SendPriceAverageRequest(DWORD vnum, DWORD count);
		void SendShopDecorationUsage(int index);
		void SendShopMoveItem(int srcPos, int destPos);
#endif
#endif

#ifdef __BUFFI_SUPPORT__
		bool RecvBuffiSkill();
#endif

		// Lover
		bool RecvLoverInfoPacket();
		bool RecvLovePointUpdatePacket();
#ifdef ENABLE_YOHARA_SYSTEM
		bool RecvSungmaAttrUpdatePacket();
#endif
#ifdef ENABLE_EVENT_MANAGER
		bool RecvEventManager();
#endif
		// Dig
		bool RecvDigMotionPacket();

		// Fishing
		bool SendFishingPacket(int iRotation);
		bool SendGiveItemPacket(DWORD dwTargetVID, TItemPos ItemPos, int iItemCount);
#if defined(ENABLE_FISHING_GAME)
		bool SendFishingGamePacket(BYTE bSubHeader, BYTE bGoals = 0, uint32_t fishing_key = 0);
#endif

		// Refine
		bool SendRefinePacket(WORD wPos, BYTE byType, BYTE bySpecialStorage
#ifdef ENABLE_PITTY_REFINE
, bool bUseSealOfGod
#endif
		);

		bool SendSelectItemPacket(DWORD dwItemPos);
#if defined(__BL_67_ATTR__)
		bool Send67AttrPacket(int iMaterialCount, int iSupportCount, int iSupportPos, int iItemPos);
		bool Send67AttrClosePacket();
		bool Recv67AttrOpenPacket();
#endif
		
		// Client Version
		bool SendClientVersionPacket();

		// CRC Report
		bool __SendCRCReportPacket();

		// 용홍석 강화
		bool SendDragonSoulRefinePacket(BYTE bRefineType, TItemPos* pos);

		// Handshake
		bool RecvHandshakePacket();
		bool RecvHandshakeOKPacket();

#ifdef _IMPROVED_PACKET_ENCRYPTION_
		bool RecvKeyAgreementPacket();
		bool RecvKeyAgreementCompletedPacket();

#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
		bool SendSwitchBotOpen();
		bool SendSwitchBotClose();
		bool SendSwitchBotChange(PyObject* attr);
		bool RecvSwitchBot();
#endif

#ifdef ENABLE_DUNGEON_INFO
		bool SendDungeonInfoOpen();
		bool SendDungeonInfoTeleport(int index_teleport);
		bool SendDungeonInfoRanking(int index_dungeon);
		bool SendDungeonInfoMision(int index);
#endif

#ifdef ENABLE_DROP_ITEM_WORLDARD
		bool SendDropItemOpen();
		bool SearchDropItem(int vnum_item);
		bool RecvDropItemPacket();
#endif
	#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
		bool CubeRenewalMakeItem(int index_item, int count_item);
		bool CubeRenewalClose();
		bool RecvCubeRenewalPacket();
#endif
#ifdef BATTLEPASS_WORLDARD
		bool RecvBattlePassPacket();
		bool BattlePassOpen();
		bool BattlePassReceiveReward(int index);
#endif
		// ETC
		DWORD GetMainActorVID();
		DWORD GetMainActorRace();
		DWORD GetMainActorEmpire();
		DWORD GetMainActorSkillGroup();
		void SetEmpireID(DWORD dwEmpireID);
		DWORD GetEmpireID();
		void __TEST_SetSkillGroupFake(int iIndex);
		#ifdef ENABLE_SASH_SYSTEM
		bool	SendSashClosePacket();
		bool	SendSashAddPacket(TItemPos tPos, BYTE bPos);
		bool	SendSashRemovePacket(BYTE bPos);
		bool	SendSashRefinePacket();
		#endif
	//////////////////////////////////////////////////////////////////////////
	// Phase 관련
	//////////////////////////////////////////////////////////////////////////
	public:
		void SetOffLinePhase();
		void SetHandShakePhase();
		void SetLoginPhase();
		void SetSelectPhase();
		void SetLoadingPhase();
		void SetGamePhase();
		void ClosePhase();

		// Login Phase
		bool SendLoginPacket(const char * c_szName, const char * c_szPassword);
		bool SendLoginPacketNew(const char * c_szName, const char * c_szPassword);
		bool SendChinaMatrixCardPacket(const char * c_szMatrixCardString);
		bool SendRunupMatrixAnswerPacket(const char * c_szMatrixCardString);
		bool SendNEWCIBNPasspodAnswerPacket(const char * answer);
		bool SendDirectEnterPacket(const char * c_szName, const char * c_szPassword, UINT uChrSlot);

		bool SendEnterGame();

		// Select Phase
		bool SendSelectEmpirePacket(DWORD dwEmpireID);
		bool SendSelectCharacterPacket(BYTE account_Index);
		bool SendChangeNamePacket(BYTE index, const char *name);
		bool SendCreateCharacterPacket(BYTE index, const char *name, BYTE job, BYTE shape, BYTE byStat1, BYTE byStat2, BYTE byStat3, BYTE byStat4);
		bool SendDestroyCharacterPacket(BYTE index, const char * szPrivateCode);
		bool SendChangeCharacterHairPacket(BYTE index, int hairIndex);
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		bool RequestFlag(const char* szName);
#endif
		// Main Game Phase
		bool SendC2CPacket(DWORD dwSize, void * pData);
		bool SendChatPacket(const char * c_szChat, BYTE byType = CHAT_TYPE_TALKING);
		bool SendWhisperPacket(const char * name, const char * c_szChat);
#if defined(WJ_COMBAT_ZONE)
		bool SendCombatZoneRequestActionPacket(int action, int value);
#endif

		bool SendMobileMessagePacket(const char * name, const char * c_szChat);
		bool SendMessengerAddByVIDPacket(DWORD vid);
		bool SendMessengerAddByNamePacket(const char * c_szName);
		bool SendMessengerRemovePacket(const char * c_szKey, const char * c_szName);
#ifdef ENABLE_DISCORD_STUFF
		std::string GetPhaseName() { return m_strPhase; }
#endif
#ifdef ENABLE_MESSENGER_BLOCK
		bool SendMessengerBlockAddByVIDPacket(DWORD vid);
		bool SendMessengerBlockRemoveByVIDPacket(DWORD vid);
		bool SendMessengerBlockAddByNamePacket(const char* c_szName);
		bool SendMessengerBlockRemovePacket(const char* c_szKey, const char* c_szName);
#endif
		bool SendHackNotification(const char* c_szMsg, const char* c_szInfo, const char* c_szHwid);

	protected:
        typedef struct SRecvChat
        {
            uint8_t     type;
            uint32_t    vid;
            char        buf[1024 + 1];
        } TRecvChat;

        std::queue<TRecvChat> m_ChatQueue;
        bool ProcessChat();

		bool OnProcess();
		void OffLinePhase();
		void HandShakePhase();
		void LoginPhase();
		void SelectPhase();
		void LoadingPhase();
		void GamePhase();

		bool __IsNotPing();

		void __DownloadMark();
		void __DownloadSymbol(const std::vector<DWORD> & c_rkVec_dwGuildID);

		void __PlayInventoryItemUseSound(TItemPos uSlotPos);
		void __PlayInventoryItemDropSound(TItemPos uSlotPos);
		//void __PlayShopItemDropSound(UINT uSlotPos);
		void __PlaySafeBoxItemDropSound(UINT uSlotPos);
		void __PlayMallItemDropSound(UINT uSlotPos);

//#common fix in most of packets
//#ifdef USE_CURRENCY_EXCHANGE
		bool __CanActMainInstance(bool bNoAct = false);
//#else
//		bool __CanActMainInstance();
//#endif

		enum REFRESH_WINDOW_TYPE
		{
			RefreshStatus = (1 << 0),
			RefreshAlignmentWindow = (1 << 1),
			RefreshCharacterWindow = (1 << 2),
			RefreshEquipmentWindow = (1 << 3),
			RefreshInventoryWindow = (1 << 4),
			RefreshExchangeWindow = (1 << 5),
			RefreshSkillWindow = (1 << 6),
			RefreshSafeboxWindow  = (1 << 7),
			RefreshMessengerWindow = (1 << 8),
			RefreshGuildWindowInfoPage = (1 << 9),
			RefreshGuildWindowBoardPage = (1 << 10),
			RefreshGuildWindowMemberPage = (1 << 11),
			RefreshGuildWindowMemberPageGradeComboBox = (1 << 12),
			RefreshGuildWindowSkillPage = (1 << 13),
			RefreshGuildWindowGradePage = (1 << 14),
			RefreshTargetBoard = (1 << 15),
			RefreshMallWindow = (1 << 16),
		};

		void __RefreshStatus();
		void __RefreshAlignmentWindow();
		void __RefreshCharacterWindow();
		void __RefreshEquipmentWindow();
		void __RefreshInventoryWindow();
		void __RefreshExchangeWindow();
		void __RefreshSkillWindow();
		void __RefreshSafeboxWindow();
		void __RefreshMessengerWindow();
		void __RefreshGuildWindowInfoPage();
		void __RefreshGuildWindowBoardPage();
		void __RefreshGuildWindowMemberPage();
		void __RefreshGuildWindowMemberPageGradeComboBox();
		void __RefreshGuildWindowSkillPage();
		void __RefreshGuildWindowGradePage();
		void __RefreshTargetBoardByVID(DWORD dwVID);
		void __RefreshTargetBoardByName(const char * c_szName);
		void __RefreshTargetBoard();
		void __RefreshMallWindow();
		void __RefreshAffectWindow();

		bool __SendHack(const char* c_szMsg);
#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_PRO
	public:
		void RefreshInventory() { __RefreshInventoryWindow(); }
#endif
#endif

	protected:
		bool RecvObserverAddPacket();
		bool RecvObserverRemovePacket();
		bool RecvObserverMovePacket();

		// Common
		bool RecvErrorPacket(int header);
		bool RecvPingPacket();
		bool RecvDefaultPacket(int header);
		bool RecvPhasePacket();

		// Login Phase
		bool __RecvLoginSuccessPacket3();
		bool __RecvLoginSuccessPacket4();
		bool __RecvLoginFailurePacket();
		bool __RecvEmpirePacket();
		bool __RecvChinaMatrixCardPacket();
		bool __RecvRunupMatrixQuizPacket();
		bool __RecvNEWCIBNPasspodRequestPacket();
		bool __RecvNEWCIBNPasspodFailurePacket();
		bool __RecvLoginKeyPacket();

		// Select Phase
		bool __RecvPlayerCreateSuccessPacket();
		bool __RecvPlayerCreateFailurePacket();
		bool __RecvPlayerDestroySuccessPacket();
		bool __RecvPlayerDestroyFailurePacket();
		bool __RecvPreserveItemPacket();
		bool __RecvPlayerPoints();
		bool __RecvChangeName();
#ifdef ENABLE_MANAGER_BANK_SYSTEM
		bool RecvAccountBankPacket();
#endif
#ifdef ENABLE_RANKING
		bool	RecvRankingTable();
#endif
#if defined(WJ_COMBAT_ZONE)
		bool __RecvCombatZoneRankingPacket();
		bool __RecvCombatZonePacket();
#endif
#if defined(ENABLE_VOICE_CHAT)
	bool RecvVoiceChatPacket();
#endif
		// Loading Phase
		bool RecvMainCharacter();
		bool RecvMainCharacter2_EMPIRE();
		bool RecvMainCharacter3_BGM();
		bool RecvMainCharacter4_BGM_VOL();

		void __SetFieldMusicFileName(const char* musicName);
		void __SetFieldMusicFileInfo(const char* musicName, float vol);
		// END_OF_SUPPORT_BGM

		bool RecvCheatBlacklist();

		// Main Game Phase
		bool RecvWarpPacket();
		bool RecvPVPPacket();
		bool RecvDuelStartPacket();
        bool RecvGlobalTimePacket();
		bool RecvCharacterAppendPacket();
		bool RecvCharacterAdditionalInfo();
		bool isAllowedNpc(DWORD race);
		bool RecvCharacterAppendPacketNew();
		bool RecvCharacterUpdatePacket();
		bool RecvCharacterUpdatePacketNew();
		bool RecvCharacterDeletePacket();
		bool RecvChatPacket();
		bool RecvOwnerShipPacket();
		bool RecvSyncPositionPacket();
		bool RecvWhisperPacket();
		bool RecvPointChange();					// Alarm to python
		bool RecvChangeSpeedPacket();
		bool RecvRankInfo();

#ifdef ENABLE_NINJEYE
		bool RecvNinjEyeRequest();
#endif

#ifdef ENABLE_SHOW_CHEST_DROP
		bool RecvChestDrop();
#endif
		bool RecvStunPacket();
		bool RecvDeadPacket();
		bool RecvCharacterMovePacket();

		bool RecvItemSetPacket();					// Alarm to python
		bool RecvItemSetPacket2();					// Alarm to python
		bool RecvItemUsePacket();					// Alarm to python
#ifdef USE_EXPANDED_ITEM_REFRESH
        bool RecvItemUpdatePacket(bool bJustCount = false);
#else
        bool RecvItemUpdatePacket();
#endif
		bool RecvItemGroundAddPacket();
		bool RecvItemGroundDelPacket();
		bool RecvItemOwnership();
#ifdef ENABLE_SUPPORT_SYSTEM
		bool RecvSupportUseSkill();
#endif
		bool RecvQuickSlotAddPacket();				// Alarm to python
		bool RecvQuickSlotDelPacket();				// Alarm to python
		bool RecvQuickSlotMovePacket();				// Alarm to python

		bool RecvCharacterPositionPacket();
		bool RecvMotionPacket();

		bool RecvShopPacket();

		bool RecvExchangePacket();

		// Quest
		bool RecvScriptPacket();
		bool RecvQuestInfoPacket();
		bool RecvQuestConfirmPacket();
		bool RecvRequestMakeGuild();

		// Skill
		bool RecvSkillLevel();
		bool RecvSkillLevelNew();
		bool RecvSkillCoolTimeEnd();
#ifdef ENABLE_NEW_DETAILS_GUI
		bool RecvKillLOG();
#endif

		// Target
		bool RecvTargetPacket();
		bool RecvViewEquipPacket();
		bool RecvDamageInfoPacket();
	#ifdef ENABLE_SEND_TARGET_INFO
		bool RecvTargetInfoPacket();

		public:
			bool SendTargetInfoLoadPacket(DWORD dwVID);


			void OnScriptEventStart(int iSkin, int iIndex);

		protected:
	#endif
		// Mount
		bool RecvMountPacket();

		// Fly
		bool RecvCreateFlyPacket();
		bool RecvFlyTargetingPacket();
		bool RecvAddFlyTargetingPacket();

		// Messenger
		bool RecvMessenger();

		// Guild
		bool RecvGuild();

		// Party
		bool RecvPartyInvite();
		bool RecvPartyAdd();
		bool RecvPartyUpdate();
		bool RecvPartyRemove();
		bool RecvPartyLink();
		bool RecvPartyUnlink();
		bool RecvPartyParameter();
#ifdef TOURNAMENT_PVP_SYSTEM
		bool RecvTournamentAdd();
#endif
		// SafeBox
		bool RecvSafeBoxSetPacket();
		bool RecvSafeBoxDelPacket();
		bool RecvSafeBoxWrongPasswordPacket();
		bool RecvSafeBoxSizePacket();
		bool RecvSafeBoxMoneyChangePacket();

		// Fishing
		bool RecvFishing();
#ifdef ENABLE_ITEMSHOP
		bool RecvItemShop();
#endif
		// Dungeon
		bool RecvDungeon();

		// Time
		bool RecvTimePacket();

		// WalkMode
		bool RecvWalkModePacket();

		// ChangeSkillGroup
		bool RecvChangeSkillGroupPacket();

		// Refine
		bool RecvRefineInformationPacket();

		// Use Potion
		bool RecvSpecialEffect();

		// 서버에서 지정한 이팩트 발동 패킷.
		bool RecvSpecificEffect();

		// 용혼석 관련
		bool RecvDragonSoulRefine();

		// MiniMap Info
#ifndef USE_MARK_ATLAS_BINARY
		bool RecvNPCList();
#endif
		bool RecvLandPacket();
		bool RecvTargetCreatePacket();
		bool RecvTargetCreatePacketNew();
		bool RecvTargetUpdatePacket();
		bool RecvTargetDeletePacket();

		// Affect
		bool RecvAffectAddPacket();
		bool RecvAffectRemovePacket();

		// Channel
		bool RecvChannelPacket();
#if defined(__BL_BATTLE_ROYALE__)
		bool RecvBattleRoyale();
#endif
		#ifdef ENABLE_SASH_SYSTEM
		bool	RecvSashPacket(bool bReturn = false);
		#endif

#ifdef ENABLE_KINGDOMS_WAR
		bool	RecvKingdomsWarPacket(bool bReturn = false);
#endif
		
#ifdef ENABLE_DUNGEON_INFO
		bool RecvDungeonInfoPacket();
#endif
		
#ifdef ENABLE_CALENDAR_SYSTEM
		bool RecvCalendarSystem();
		bool RecvCalendarSystemADM();
#endif
		// @fixme007
		bool RecvUnk213();

	protected:
		// 이모티콘
		bool ParseEmoticon(const char * pChatMsg, DWORD * pdwEmoticon);

		// 파이썬으로 보내는 콜들
		void OnConnectFailure();
		

		void OnRemoteDisconnect();
		void OnDisconnect();

		void SetGameOnline();
		void SetGameOffline();
		BOOL IsGameOnline();

	protected:
		bool CheckPacket(TPacketHeader * pRetHeader);

		void __InitializeGamePhase();
		void __InitializeMarkAuth();
		void __GlobalPositionToLocalPosition(long& rGlobalX, long& rGlobalY);
		void __LocalPositionToGlobalPosition(long& rLocalX, long& rLocalY);

		bool __IsPlayerAttacking();
		bool __IsEquipItemInSlot(TItemPos Cell);

		void __ShowMapName(long lLocalX, long lLocalY);

		void __LeaveOfflinePhase() {}
		void __LeaveHandshakePhase() {}
		void __LeaveLoginPhase() {}
		void __LeaveSelectPhase() {}
		void __LeaveLoadingPhase() {}
		void __LeaveGamePhase();

		void __ClearNetworkActorManager();

		void __ClearSelectCharacterData();

		// DELETEME
		//void __SendWarpPacket();

		void __ConvertEmpireText(DWORD dwEmpireID, char* szText);

		void __RecvCharacterAppendPacket(SNetworkActorData * pkNetActorData);
		void __RecvCharacterUpdatePacket(SNetworkUpdateActorData * pkNetUpdateActorData);

		void __FilterInsult(char* szLine, UINT uLineLen);

		void __SetGuildID(DWORD id);

	protected:
		TPacketGCHandshake m_HandshakeData;
		DWORD m_dwChangingPhaseTime;
		DWORD m_dwBindupRetryCount;
		DWORD m_dwMainActorVID;
		DWORD m_dwMainActorRace;
		DWORD m_dwMainActorEmpire;
		DWORD m_dwMainActorSkillGroup;
		BOOL m_isGameOnline;
		BOOL m_isStartGame;

		DWORD m_dwGuildID;
		DWORD m_dwEmpireID;

		struct SServerTimeSync
		{
			DWORD m_dwChangeServerTime;
			DWORD m_dwChangeClientTime;
		} m_kServerTimeSync;

		void __ServerTimeSync_Initialize();
		//DWORD m_dwBaseServerTime;
		//DWORD m_dwBaseClientTime;

		DWORD m_dwLastGamePingTime;

		std::string	m_stID;
		std::string	m_stPassword;
		std::string	m_strLastCommand;
		std::string	m_strPhase;
		DWORD m_dwLoginKey;
		BOOL m_isWaitLoginKey;

		std::string m_stMarkIP;

		CFuncObject<CPythonNetworkStream>	m_phaseProcessFunc;
		CFuncObject<CPythonNetworkStream>	m_phaseLeaveFunc;

		PyObject*							m_poHandler;
	public:
		PyObject*							m_apoPhaseWnd[PHASE_WINDOW_NUM];
	protected:
		PyObject*							m_poSerCommandParserWnd;

		TSimplePlayerInformation			m_akSimplePlayerInfo[PLAYER_PER_ACCOUNT4];
		DWORD								m_adwGuildID[PLAYER_PER_ACCOUNT4];
		std::string							m_astrGuildName[PLAYER_PER_ACCOUNT4];
		bool m_bSimplePlayerInfo;

		CRef<CNetworkActorManager>			m_rokNetActorMgr;

		bool m_isRefreshStatus;
		bool m_isRefreshCharacterWnd;
		bool m_isRefreshEquipmentWnd;
		bool m_isRefreshInventoryWnd;
		bool m_isRefreshExchangeWnd;
		bool m_isRefreshSkillWnd;
		bool m_isRefreshSafeboxWnd;
		bool m_isRefreshMallWnd;
		bool m_isRefreshMessengerWnd;
		bool m_isRefreshGuildWndInfoPage;
		bool m_isRefreshGuildWndBoardPage;
		bool m_isRefreshGuildWndMemberPage;
		bool m_isRefreshGuildWndMemberPageGradeComboBox;
		bool m_isRefreshGuildWndSkillPage;
		bool m_isRefreshGuildWndGradePage;
		bool m_isRefreshAffectWindow;

		// Emoticon
		std::vector<std::string> m_EmoticonStringVector;

		struct STextConvertTable
		{
			char acUpper[26];
			char acLower[26];
			BYTE aacHan[5000][2];
		} m_aTextConvTable[3];



		struct SMarkAuth
		{
			CNetworkAddress m_kNetAddr;
			DWORD m_dwHandle;
			DWORD m_dwRandomKey;
		} m_kMarkAuth;



		DWORD m_dwSelectedCharacterIndex;

		CInsultChecker m_kInsultChecker;

		bool m_isEnableChatInsultFilter;
		bool m_bComboSkillFlag;

		std::deque<std::string> m_kQue_stHack;

	private:
        struct SDirectEnterMode
        {
            bool m_isSet;
            bool m_isWarp;
            DWORD m_dwChrSlotIndex;
        } m_kDirectEnterMode;

        void __DirectEnterMode_Initialize();
        void __DirectEnterMode_Set(UINT uChrSlotIndex, bool bIsWarp = false);
        bool __DirectEnterMode_IsWarp();
        bool __DirectEnterMode_IsSet();

	public:
		bool GetDirectEnterMode_IsWarp() { return __DirectEnterMode_IsWarp(); }

		DWORD EXPORT_GetBettingGuildWarValue(const char* c_szValueName);

	private:
		struct SBettingGuildWar
		{
			DWORD m_dwBettingMoney;
			DWORD m_dwObserverCount;
		} m_kBettingGuildWar;

		CInstanceBase * m_pInstTarget;

		void __BettingGuildWar_Initialize();
		void __BettingGuildWar_SetObserverCount(UINT uObserverCount);
		void __BettingGuildWar_SetBettingMoney(UINT uBettingMoney);

#ifdef ENABLE_SWITCHBOT
	public:
		bool RecvSwitchbotPacket();

		bool SendSwitchbotStartPacket(BYTE slot, std::vector<CPythonSwitchbot::TSwitchbotAttributeAlternativeTable> alternatives);
		bool SendSwitchbotStopPacket(BYTE slot);
#endif

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	public:
		bool SendCShieldPacket(bool start, DWORD num);

	protected:
		bool RecvCShieldPacket();
#endif

#ifdef USE_OFFLINESHOP_SEARCH_V2
    protected:
        bool m_isRefreshShopSearchWnd;
        void __RefreshShopSearchWindow();

        bool RecvShopSearchResult();

    public:
#ifdef USE_PREMIUM_AFFECT
		void RefreshShopSearchWindow()
		{
			__RefreshShopSearchWindow();
		}
#endif

        void SendShopSearchOpen(const ikashop::TFilterInfo tmp);
        void SendShopSearchChangePage(const int32_t pageIdx);
        void SendShopSearchBuyItem(const std::vector<ikashop::TSubPacketCGShopBuyItem>& vecItems);
#endif
#ifdef ENABLE_AURA_SYSTEM
	protected:
		bool RecvAuraPacket();

	public:
		bool SendAuraRefineCheckIn(TItemPos InventoryCell, TItemPos AuraCell, BYTE byAuraRefineWindowType);
		bool SendAuraRefineCheckOut(TItemPos AuraCell, BYTE byAuraRefineWindowType);
		bool SendAuraRefineAccept(BYTE byAuraRefineWindowType);
		bool SendAuraRefineCancel();
#endif

#ifdef USE_CURRENCY_EXCHANGE
	public:
		bool SendCurrencyExchange(const long long exchangeAmount, const uint8_t exchangeType);
#endif

#ifdef USE_CRYSTAL_SYSTEM
	protected:
		bool RecvShardCraft();

	public:
		bool SendShardCraft(const uint8_t bSubHeader,
							int32_t iSourceSlot = -1,
							int32_t iTargetSlot = -1);
#endif
};
