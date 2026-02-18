# coding: latin_1

##
## Interface
##
import constInfo
import systemSetting
import wndMgr
import chat
import app
import player
import uiTaskBar
import uiCharacter
import uiInventory
import uiDragonSoul
import uiChat
import uiMessenger
import guild
import net
import item
import uiCards
import introLoading
import uiVote4Buff
if app.ENABLE_SUNG_MAHI_TOWER:
	import uiSungMahi
if app.ENABLE_SWITCHBOT:
	import uiSwitchbot
if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
	import uiAuto
if app.ENABLE_AURA_SYSTEM:
	import uiAura

if app.__SPIN_WHEEL__:
	import uiSpinWheel

if app.ENABLE_QUEST_RENEWAL:
	import quest

if app.__ROULETTE__:
	import uiRoulette

if app.__BL_BATTLE_ROYALE__:
	import uiBattleRoyale

if app.ENABLE_FISHING_GAME:
	import uiFishingGame

if app.AUTO_SHOUT:
	import uishout

if app.__AUTO_HUNT__:
	import uiAutoHunt

if app.RENEWAL_MISSION_BOOKS:
	import uiMission

if app.ENABLE_TRACK_WINDOW:
	import uiTrack

if app.ENABLE_ZODIAC_MISSION:
	import ui12zi

if app.ENABLE_SWITCHBOT_WORLDARD:
	import uiswtichbot
	
if app.ENABLE_GAME_OPTION_ESCAPE:
	import uiEscapePopup

if app.ENABLE_RENDER_TARGET:
	import uiRenderTarget
if app.ENABLE_EXCHANGE_LOG:
	import uiExchangeLog
if app.ENABLE_EVENT_MANAGER:
	import uiEventCalendarNew
if app.ENABLE_AUTO_SELECT_SKILL:
	import uiAutoSkill
if app.ENABLE_WIKI:
	import uiWiki
import ui
import uiHelp
import uiWhisper
import uiPointReset
import uiShop
import uiExchange
import uiSystem
import uiRestart
import uiToolTip
import uiMiniMap
import uiParty
import uiSafebox
import uiGuild
import uiQuest
import uiRankInfo
import uiCommon
import uiRefine
import uiEquipmentDialog
import uiGameButton
import uiTip
import uiCube
import miniMap
import uiNameReinforce

# ACCESSORY_REFINE_ADD_METIN_STONE
import uiselectitem
# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
import uiScriptLocale
if app.ENABLE_BIYOLOG:
	import uiBiyolog

if app.ENABLE_NEW_PET_SYSTEM:
	import uiPetSystemNew
	import uiChangeName

import event
import localeInfo
# import uiItemShop
import uiTicket
import uiitemshop_v2
import dbg
if app.ENABLE_FISH_GAME:
	import uiFishGame

if app.ENABLE_ITEMSHOP:
	import uiItemShopNew

if app.BL_67_ATTR:
	import uiAttr67Add

if app.ENABLE_REWARD_SYSTEM:
	import uiReward

if app.ENABLE_KINGDOMS_WAR:
	import uikingdomswar

import uiSpecialInventory

if app.dracaryS_DUNGEON_LIB:
	import uiDungeonTimer

IsQBHide = 0
if app.ENABLE_SASH_SYSTEM:
	import uisash
if app.ENABLE_MANAGER_BANK_SYSTEM:
	import uibankamanger
if app.ENABLE_SPECIAL_STORAGE:
	import uiSpecialStorage
if app.ENABLE_SHOW_CHEST_DROP:
	import uiChestDropInfo
if app.ENABLE_MINI_GAME_CATCH_KING:
	import uiMiniGameCatchKing

import uicalendar
#import uiEventCalendar
if app.LINK_IN_CHAT:
	import os

if app.ENABLE_DUNGEON_INFO:
	import uiDungeonInfo
	import uitabledungeon

if app.__DUNGEON_INFO__:
	import uiDungeonTrack
#import uidiscord
import uiguias
import grp
if app.ENABLE_RANKING:
	import uiranking
if app.ENABLE_GEM_SYSTEM:
	import uiGem
import uicz_estadisticas
import uiswitch
import uitablabonus
if app.__AUTO_SKILL_READER__:
	import uiAutoSkillReader
	
if app.ENABLE_IKASHOP_RENEWAL:
	import uiikashop

if app.USE_CAPTCHA_SYSTEM:
	import uicaptcha
if app.ENABLE_BATTLEPASS:
	import uibattlepass
if app.ENABLE_VOICE_CHAT:
	import uiVoiceChatConfig

class Interface(object):
	CHARACTER_STATUS_TAB = 1
	CHARACTER_SKILL_TAB = 2
	wndParty = None
	if app.USE_CAPTCHA_SYSTEM:
		def GetHideUIMode(self):
			return self.IsHideUIMode

		def RecvCaptcha(self, code, limit):
			if self.wndCaptcha:
				self.wndCaptcha.Open(code, limit)

	if app.USE_CAPTCHA_SYSTEM or app.USE_OFFLINESHOP_SEARCH_V2:
		def SetAffectShower(self, affectShowerInst):
			from _weakref import proxy
			self.uiAffectShower = proxy(affectShowerInst)

		if app.USE_OFFLINESHOP_SEARCH_V2:
			def RefreshShopSearch(self):
				if self.wndIkashopSearchShopV2:
					self.wndIkashopSearchShopV2.RefreshMe()

			if app.USE_PREMIUM_AFFECT:
				def RefreshShopSearchAscending(self):
					if self.wndIkashopSearchShopV2:
						self.wndIkashopSearchShopV2.RefreshAscending()

	def __init__(self):
		systemSetting.SetInterfaceHandler(self)

		if app.USE_CRYSTAL_SYSTEM:
			self.wndShardCraftWnd = None

		if app.__SPIN_WHEEL__:
			self.wndSpinWheel = None
		if app.__ROULETTE__:
			self.wndRoulette = None
		if app.__AUTO_HUNT__:
			self.wndAutoHunt = None

		if app.__AUTO_SKILL_READER__:
			self.wndAutoSkillReader = None

		if app.__DUNGEON_INFO__:
			self.wndDungeonTrack = None
			self.wndDungeonManager = None
		if app.ENABLE_GEM_SYSTEM:
			self.wndGemShop = None
		if app.ENABLE_TRACK_WINDOW:
			self.wndTrackWindow = None
		if app.ENABLE_EXCHANGE_LOG:
			self.wndExchangeLog = None
		if app.ENABLE_RENDER_TARGET:
			self.wndRenderTarget = None
		if app.RENEWAL_MISSION_BOOKS:
			self.wndBookMission = None
		self.windowOpenPosition = 0
		if app.ENABLE_FISHING_GAME:
			self.wndFishingGame = None

		if app.ENABLE_FISH_GAME:
			self.wndFishGame=None
		self.dlgWhisperWithoutTarget = None
		self.inputDialog = None
		self.tipBoard = None
		if app.ENABLE_REWARD_SYSTEM:
			self.wndReward = None
		self.bigBoard = None
		self.wndGiftBox = None
		self.wndRankInfo = None
		self.wndReinforceName = None

		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			self.wndAutoWindow = None
		if app.ENABLE_RANKING:
			self.wndRanking = None
		if app.ENABLE_ZODIAC_MISSION:
			self.wnd12ziReward = None
			self.wnd12ziTimer = None
		if app.ENABLE_WIKI:
			self.wndWiki = None
		if app.ENABLE_SHOW_CHEST_DROP:
			self.wndChestDropInfo = None
		if app.ENABLE_AUTO_SELECT_SKILL:
			self.wndAutoSkill = None
		if app.ENABLE_EVENT_MANAGER:
			self.wndEventManager = None
			self.wndEventIcon = None
		if app.ENABLE_BIYOLOG:
			self.wndBio = None
		if app.ENABLE_DUNGEON_INFO:
			self.wndDungeonInfo = None
		self.wndVote4Buff = None

		self.questButtonList = []

		if app.dracaryS_DUNGEON_LIB:
			self.missionBoard = None
			self.wndDungeonTimer=None
		
		if app.ENABLE_ITEMSHOP:
			self.wndItemShop=None

		# ITEM_MALL
		#self.mallPageDlg = None
		# END_OF_ITEM_MALL

		if app.ENABLE_DROP_ITEM_WORLDARD:
			self.wndDropItem = None

		self.wndTableBonus = None

		if app.ENABLE_NEW_PET_SYSTEM:
			self.wndPet=None
			self.change_window=None

		self.wndBattlePassButton = None

		if app.ENABLE_CHANNEL_CHANGE:
			self.wndChangeChannel = None

		if app.ENABLE_MINI_GAME_CATCH_KING:
			self.wndCatchKingGame = None
			self.wndCatchKingIcon = None

		if app.ENABLE_CUBE_RENEWAL_WORLDARD:
			self.wndCubeRenewal = None
		#self.wndWeb = None
		self.wndTaskBar = None
		self.wndCharacter = None
		self.managerAccountBank = None
		self.wndInventory = None
		self.wndInventoryNew = None
		self.wndExpandedTaskBar = None
		self.wndDragonSoul = None
		self.wndDragonSoulRefine = None
		self.wndChat = None
		self.wndMessenger = None
		self.wndMiniMap = None
		self.wndGuild = None
		self.wndGuildBuilding = None
		if app.ENABLE_VOICE_CHAT:
			self.wndVoiceChatConfig = None
			self.wndVoiceChatOverlay = None
		if app.__BL_BATTLE_ROYALE__:
			self.wndBattleRoyaleApplication = None
			self.wndBattleRoyaleInterface = None
		if app.ENABLE_SUNG_MAHI_TOWER:
			self.wndSungMahiWindow = None
		if app.ENABLE_SWITCHBOT:
			self.wndSwitchbot = None
		if app.ENABLE_BATTLEPASS:
			self.wndBattlePass = None

		if app.USE_CAPTCHA_SYSTEM:
			self.wndCaptcha = None
			self.uiAffectShower = None

		if app.ENABLE_SPECIAL_STORAGE:
			self.wndSpecialStorage = None
		if app.LINK_IN_CHAT:
			self.OpenLinkQuestionDialog = None

		self.wndAlmacenMenu = None

		self.listGMName = {}
		self.wndQuestWindow = {}
		self.wndQuestWindowNewKey = 0
		self.guildScoreBoardDict = {}
		self.equipmentDialogDict = {}

		if app.ENABLE_IKASHOP_RENEWAL:
			self.wndIkashopBusiness = None
			self.wndIkashopSearchShop = None

			if app.USE_OFFLINESHOP_SEARCH_V2:
				self.wndIkashopSearchShopV2 = None

			if app.EXTEND_IKASHOP_ULTIMATE:
				self.wndIkashopShopSkin = None

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.onTopWindow = player.ON_TOP_WND_NONE

		if app.USE_CAPTCHA_SYSTEM:
			self.IsHideUIMode = False
			
		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.wndEscapePopup = None

		self.wndExpandedMoneyTaskBar = None

		event.SetInterfaceWindow(self)

	def __del__(self):
		systemSetting.DestroyInterfaceHandler()
		event.SetInterfaceWindow(None)

	if app.AUTO_SHOUT:
		def __MakeShoutWindow(self):
			self.wndShout = uishout.ShoutManager()
			self.wndShout.LoadWindow()
			self.wndShout.Hide()
	
	def OpenCombatZoneWindow(self):
		if player.IsCombatZoneMap():
			self.OnAskCombatZoneQuestionDialog()
		else:
			net.SendCombatZoneRequestActionPacket(net.COMBAT_ZONE_ACTION_OPEN_RANKING, net.COMBAT_ZONE_EMPTY_DATA)

	def __OnClickGiftButton(self):
		if self.wndGameButton:
			if not self.wndGiftBox.IsShow():
				self.wndGiftBox.Open()
			else:
				self.wndGiftBox.Close()

	def ClearGift(self):
		if self.wndGameButton:
			self.wndGameButton.HideGiftButton()
		if self.wndGiftBox:
			self.wndGiftBox.Clear()
			self.wndGiftBox.Refresh()
	################################
	## Make Windows & Dialogs
	if app.ENABLE_SWITCHBOT_WORLDARD:
		def __MakeSwitchBot(self):
			self.wndSwitchBot = uiswtichbot.UiSwitchBot()
			self.wndSwitchBot.Hide()

	def __MakeUICurtain(self):
		wndUICurtain = ui.Bar("TOP_MOST")
		wndUICurtain.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
		wndUICurtain.SetColor(0x77000000)
		wndUICurtain.Hide()
		self.wndUICurtain = wndUICurtain

	def __MakeMessengerWindow(self):
		self.wndMessenger = uiMessenger.MessengerWindow()

		from _weakref import proxy
		self.wndMessenger.SetWhisperButtonEvent(lambda n,i=proxy(self):i.OpenWhisperDialog(n))
		self.wndMessenger.SetGuildButtonEvent(ui.__mem_func__(self.ToggleGuildWindow))

	def __MakeGuildWindow(self):
		self.wndGuild = uiGuild.GuildWindow()

	def __MakeChatWindow(self):

		wndChat = uiChat.ChatWindow()

		wndChat.SetSize(wndChat.CHAT_WINDOW_WIDTH, 0)
		wndChat.SetPosition(wndMgr.GetScreenWidth()/2 - wndChat.CHAT_WINDOW_WIDTH/2, wndMgr.GetScreenHeight() - wndChat.EDIT_LINE_HEIGHT - 37)
		wndChat.SetHeight(200)
		wndChat.Refresh()
		wndChat.Show()

		self.wndChat = wndChat
		self.wndChat.BindInterface(self)
		self.wndChat.SetSendWhisperEvent(ui.__mem_func__(self.OpenWhisperDialogWithoutTarget))
		self.wndChat.SetOpenChatLogEvent(ui.__mem_func__(self.ToggleChatLogWindow))

	def ToggleExpandedMoneyButton(self):
		if self.wndExpandedMoneyTaskBar:
			if self.wndExpandedMoneyTaskBar.IsShow():
				self.wndExpandedMoneyTaskBar.Hide()
			else:
				self.wndExpandedMoneyTaskBar.Show()

	def __MakeTaskBar(self):
		import uiGift
		wndGiftBox=uiGift.GiftDialog()
		wndGiftBox.Hide()
		self.wndGiftBox=wndGiftBox

		wndTaskBar = uiTaskBar.TaskBar()
		wndTaskBar.LoadWindow()
		self.wndTaskBar = wndTaskBar
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_CHARACTER, ui.__mem_func__(self.ToggleCharacterWindowStatusPage))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_INVENTORY, ui.__mem_func__(self.ToggleInventoryWindow))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_MESSENGER, ui.__mem_func__(self.ToggleMessenger))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_SYSTEM, ui.__mem_func__(self.ToggleSystemDialog))
		#if app.ENABLE_EVENT_MANAGER:
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_CALENDAR, ui.__mem_func__(self.OpenEventCalendar))
		#else:
		#self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_CALENDAR, ui.__mem_func__(self.CalendarDialog))
		if uiTaskBar.TaskBar.IS_EXPANDED:
			self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_EXPAND, ui.__mem_func__(self.ToggleExpandedButton))
			self.wndExpandedTaskBar = uiTaskBar.ExpandedTaskBar()
			self.wndExpandedTaskBar.LoadWindow()
			self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskBar.ExpandedTaskBar.BUTTON_DRAGON_SOUL, ui.__mem_func__(self.ToggleDragonSoulWindow))

			if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
				self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskBar.ExpandedTaskBar.BUTTON_AUTO_WINDOW, ui.__mem_func__(self.ToggleAutoWindow))

			self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskBar.ExpandedTaskBar.BUTTON_SWITCHBOT, ui.__mem_func__(self.OpenSwitch))
			self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskBar.ExpandedTaskBar.BUTTON_PET, ui.__mem_func__(self.OpenPetWindow))
			self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskBar.ExpandedTaskBar.BUTTON_SHOPSEARCH, ui.__mem_func__(self.ToggleIkashopSearchShopBoard))
			self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskBar.ExpandedTaskBar.BUTTON_WIKI, ui.__mem_func__(self.OpenWikiWindow))
			# self.wndExpandedTaskBar.Show()
			# self.wndExpandedTaskBar.SetTop()

		else:
			self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_CHAT, ui.__mem_func__(self.ToggleChat))

		self.wndExpandedMoneyTaskBar = uiTaskBar.ExpandedMoneyTaskBar()
		self.wndExpandedMoneyTaskBar.LoadWindow()

		if self.wndInventory and self.wndExpandedMoneyTaskBar:
			self.wndInventory.BindExpandedMoneyTaskBar(self.wndExpandedMoneyTaskBar)

		if self.wndTaskBar:
			self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_EXPAND_MONEY, ui.__mem_func__(self.ToggleExpandedMoneyButton))

	def __MakeParty(self):
		wndParty = uiParty.PartyWindow()
		wndParty.Hide()
		self.wndParty = wndParty

	def CalendarDialog(self):
		pass
		if self.wndCalendar.IsShow():
			self.wndCalendar.Close()
		else:
			net.CalendarOpen()

	def __MakeGameButtonWindow(self):
		wndGameButton = uiGameButton.GameButtonWindow()
		wndGameButton.SetTop()
		wndGameButton.Show()
		wndGameButton.SetButtonEvent("STATUS", ui.__mem_func__(self.__OnClickStatusPlusButton))
		wndGameButton.SetButtonEvent("SKILL", ui.__mem_func__(self.__OnClickSkillPlusButton))
		wndGameButton.SetButtonEvent("QUEST", ui.__mem_func__(self.__OnClickQuestButton))
		wndGameButton.SetButtonEvent("HELP", ui.__mem_func__(self.__OnClickHelpButton))
		wndGameButton.SetButtonEvent("BUILD", ui.__mem_func__(self.__OnClickBuildButton))
		wndGameButton.SetButtonEvent("GIFT", ui.__mem_func__(self.__OnClickGiftButton))
		wndGameButton.SetButtonEvent("COMBAT_ZONE", ui.__mem_func__(self.OpenCombatZoneWindow))
		wndGameButton.SetButtonEvent("BOSS_EVENT", ui.__mem_func__(self.OpenBOSS_EVENT))
		wndGameButton.SetButtonEvent("METIN_EVENT", ui.__mem_func__(self.OpenMETIN_EVENT))

		self.wndGameButton = wndGameButton

		if app.ENABLE_CHANNEL_CHANGE:
			self.wndChangeChannel = uiSystem.MoveChannelDialog()
			self.wndChangeChannel.Hide()

	def OpenBOSS_EVENT(self):
		import event
		event.QuestButtonClick(constInfo.BOSS_EVENT)

	def CheckBossOpenOrNot(self):
		if constInfo.BOSS_EVENT_STATUS == 0:
			self.wndGameButton.HideBossButton()
		else:
			self.wndGameButton.ShowBossButton()

	def OpenMETIN_EVENT(self):
		import event
		event.QuestButtonClick(constInfo.METIN_EVENT)

	def CheckMetinOpenOrNot(self):
		if constInfo.METIN_EVENT_STATUS == 0:
			self.wndGameButton.HideMetinButton()
		else:
			self.wndGameButton.ShowMetinButton()

	def CheckCombatOpenOrNot(self):
		if constInfo.COMBAT_ZONE == 0:
			self.wndGameButton.HideCombatButton()
		else:
			self.wndGameButton.ShowCombatButton()

	def __IsChatOpen(self):
		return True

	if app.ENABLE_REFINE_RENEWAL:
		def CheckRefineDialog(self, isFail):
			self.dlgRefineNew.CheckRefine(isFail)

	def __MakeWindows(self):
		#self.wndWarpShower = introLoading.WarpingWindow()
		if app.ENABLE_FISHING_GAME:
			self.wndFishingGame = uiFishingGame.FishGame()
		if app.ENABLE_AURA_SYSTEM:
			wndAura = uiAura.AuraWindow()
			self.wndAura = wndAura

		wndCharacter = uiCharacter.CharacterWindow()
		wndInventory = uiInventory.InventoryWindow()
		wndInventoryNew = uiSpecialInventory.InventoryWindowNew()
		wndInventory.SetTop()
		wndInventoryNew.SetTop()
		wndInventory.BindInterfaceClass(self)
		wndInventoryNew.BindInterfaceClass(self)
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			wndDragonSoul = uiDragonSoul.DragonSoulWindow()
			wndDragonSoul.BindInterfaceClass(self)
			wndDragonSoulRefine = uiDragonSoul.DragonSoulRefineWindow()
		else:
			wndDragonSoul = None
			wndDragonSoulRefine = None


		wndMiniMap = uiMiniMap.MiniMap()
		wndSafebox = uiSafebox.SafeboxWindow()
		#if app.WJ_ENABLE_TRADABLE_ICON:
		wndSafebox.BindInterface(self)
		# ITEM_MALL
		wndMall = uiSafebox.MallWindow()
		self.wndMall = wndMall
		# END_OF_ITEM_MALL

		self.wndAlmacenMenu = uiSafebox.AlmacenMenu()
		self.wndAlmacenMenu.Hide()

		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			self.wndAutoWindow = uiAuto.AutoWindow()

		wndChatLog = uiChat.ChatLogWindow()
		wndChatLog.BindInterface(self)

		self.wndCharacter = wndCharacter
		self.wndInventory = wndInventory
		self.wndInventoryNew = wndInventoryNew
		self.wndDragonSoul = wndDragonSoul
		self.wndDragonSoulRefine = wndDragonSoulRefine
		self.wndMiniMap = wndMiniMap
		self.wndMiniMap.BindInterface(self)
		self.wndSafebox = wndSafebox
		self.wndChatLog = wndChatLog
		if app.ENABLE_SUNG_MAHI_TOWER:
			self.wndSungMahiWindow = uiSungMahi.SungMahiWindow()
		if app.__BL_BATTLE_ROYALE__:
			self.wndBattleRoyaleApplication = uiBattleRoyale.BattleRoyaleApplication()
			self.wndBattleRoyaleInterface = uiBattleRoyale.BattleRoyaleInterface()

		if app.USE_CAPTCHA_SYSTEM:
			self.wndCaptcha = uicaptcha.CaptchaDialog(self)

		if app.ENABLE_SHOW_CHEST_DROP:
			self.wndChestDropInfo = uiChestDropInfo.ChestDropInfoWindow()

		if app.ENABLE_RANKING:
			self.wndRanking = uiranking.RankingWindow()

		if app.ENABLE_KINGDOMS_WAR:
			self.wndKingdomsWar = uikingdomswar.Window()

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)
			self.wndDragonSoulRefine.SetInventoryWindows(self.wndInventory, self.wndDragonSoul)
			self.wndInventory.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)
		
		if app.ENABLE_SWITCHBOT:
			self.wndSwitchbot = uiSwitchbot.SwitchbotWindow()
		
		if app.ENABLE_IKASHOP_RENEWAL:
			self.wndIkashopBusiness = uiikashop.IkarusShopBusinessBoard()

			self.wndIkashopSearchShop = uiikashop.IkarusSearchShopBoard()
			if app.EXTEND_IKASHOP_PRO:
				from _weakref import proxy
				self.wndIkashopSearchShop.SetOpenWhisper(lambda name, _s = proxy(self) : _s.OpenWhisperDialog(name))

				if not app.USE_OFFLINESHOP_SEARCH_V2:
					self.uiAffectShower.SetSearchShopBoard(self.wndIkashopSearchShop)

			if app.EXTEND_IKASHOP_ULTIMATE:
				self.wndIkashopShopSkin = uiikashop.IkarusShopSkinBoard()
		
		if app.ENABLE_MINI_GAME_CATCH_KING:
			self.wndCatchKingGame = uiMiniGameCatchKing.MiniGameCatchKing()
			
		self.wndRankInfo = uiRankInfo.RankInfo()
		self.wndRankInfo.Hide()
		self.wndReinforceName = uiNameReinforce.NameReinforceWindow()
		self.wndReinforceName.Hide()


		if app.BL_67_ATTR:
			self.wndAttr67Add = uiAttr67Add.Attr67AddWindow()
			if app.WJ_ENABLE_TRADABLE_ICON:
				self.wndAttr67Add.BindInterface(self)
				self.wndAttr67Add.SetInven(self.wndInventory)
				# self.wndAttr67Add.SetInvenStorage(self.wndInventoryNew)
				self.wndInventory.BindWindow(self.wndAttr67Add)
				self.wndInventoryNew.BindWindow(self.wndAttr67Add)

		if app.ENABLE_SPECIAL_STORAGE:
			self.wndSpecialStorage = uiSpecialStorage.SpecialStorageWindow()
			self.wndSpecialStorage.BindInterface(self)
		else:
			self.wndSpecialStorage = None

		self.wndVote4Buff = uiVote4Buff.VoteWindow()
		self.wndVote4Buff.Hide()

		self.wndczestadisticas = uicz_estadisticas.CombatZoneEstadisticas()
		self.wndczestadisticas.Hide()
	
	def __MakeDialogs(self):
		self.dlgExchange = uiExchange.ExchangeDialog()
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.dlgExchange.BindInterface(self)
			self.dlgExchange.SetInven(self.wndInventory)
			self.dlgExchange.SetInvenStorage(self.wndInventoryNew)
			self.wndInventory.BindWindow(self.dlgExchange)
			self.wndInventoryNew.BindWindow(self.dlgExchange)
		self.dlgExchange.LoadDialog()
		self.dlgExchange.SetCenterPosition()
		self.dlgExchange.Hide()
		self.dlgPointReset = uiPointReset.PointResetDialog()
		self.dlgPointReset.LoadDialog()
		self.dlgPointReset.Hide()

		if app.ENABLE_MANAGER_BANK_SYSTEM:
			self.managerAccountBank = uibankamanger.BankGui()
			self.managerAccountBank.Hide()
		
		self.dlgShop = uiShop.ShopDialog()
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.dlgShop.BindInterface(self)
		self.dlgShop.LoadDialog()
		self.dlgShop.Hide()
		
		self.wndTicket = uiTicket.TicketWindow()
		self.wndTicket.Hide()

		# self.wndItemShop = uiItemShop.ItemShopWindow()
		# self.wndItemShop.Hide()
		
		self.dlgRestart = uiRestart.RestartDialog()
		self.dlgRestart.LoadDialog()
		self.dlgRestart.Hide()

		self.dlgSystem = uiSystem.SystemDialog()
		self.dlgSystem.LoadDialog()
		self.dlgSystem.SetOpenHelpWindowEvent(ui.__mem_func__(self.OpenHelpWindow))
		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.dlgSystem.BindInterface(self)

		self.dlgSystem.Hide()

		self.dlgPassword = uiSafebox.PasswordDialog()
		self.dlgPassword.Hide()

		self.hyperlinkItemTooltip = uiToolTip.HyperlinkItemToolTip()
		self.hyperlinkItemTooltip.Hide()

		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.BindInterface(self)
		self.tooltipItem.Hide()

		if app.ENABLE_SHOW_CHEST_DROP:
			self.wndChestDropInfo.SetTooltipItem(self.tooltipItem)

		self.Itemshop_v2 = uiitemshop_v2.Itemshop()
		self.Itemshop_v2.BindToolTipItem(self.tooltipItem)

		self.tooltipSkill = uiToolTip.SkillToolTip()
		self.tooltipSkill.Hide()

		if app.ENABLE_WHISPER_CLOSE_SHORTCUT:
			self.whisper_tooltip = uiToolTip.ToolTip()
			self.whisper_tooltip.AutoAppendTextLine(localeInfo.WHISPER_TOOLTIP_CLOSE_SHORTCUT)
			self.whisper_tooltip.AlignHorizonalCenter()
			self.whisper_tooltip.Hide()

		self.dlgRefineNew = uiRefine.RefineDialogNew()
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.dlgRefineNew.SetInven(self.wndInventory)
			# self.dlgRefineNew.SetInvenStorage(self.wndInventoryNew)
			self.wndInventory.BindWindow(self.dlgRefineNew)
			self.wndInventoryNew.BindWindow(self.dlgRefineNew)

		if constInfo.SHOW_REFINE_ITEM_DESC:
			self.dlgRefineNew.BindInterface(self)

		self.dlgRefineNew.Hide()
		#self.wndDiscord = uidiscord.Discord()
		#self.wndDiscord.LoadWindow()
		#self.wndDiscord.Show()

		#self.wndGuias = uiguias.Guias()
		#self.wndGuias.LoadWindow()
		#self.wndGuias.Show()
		
		self.__MakeWiki()

	def __MakeTableBonus(self):
		self.wndTableBonus = uitablabonus.TableBonusWindows()
		self.wndTableBonus.LoadWindow()
		self.wndTableBonus.Hide()


	def __MakeHelpWindow(self):
		self.wndHelp = uiHelp.HelpWindow()
		self.wndHelp.LoadDialog()
		self.wndHelp.SetCloseEvent(ui.__mem_func__(self.CloseHelpWindow))
		self.wndHelp.Hide()

	def __MakeTipBoard(self):
		# if app.__RENEWAL_NOTICE__:
			# self.tipBoard = uiTip.TipBoardNew()
		# else:
		self.tipBoard = uiTip.TipBoard()
		self.tipBoard.Hide()

		self.bigBoard = uiTip.BigBoard()
		self.bigBoard.Hide()
		
		if app.dracaryS_DUNGEON_LIB:
			self.missionBoard = uiTip.MissionBoard()
			self.missionBoard.Hide()

	def __MakeWebWindow(self):
		pass
		#if constInfo.IN_GAME_SHOP_ENABLE:
		#	import uiWeb
		#	self.wndWeb = uiWeb.WebWindow()
		#	self.wndWeb.LoadWindow()
		#	self.wndWeb.Hide()

	def __MakeCubeWindow(self):
		self.wndCube = uiCube.CubeWindow()
		self.wndCube.LoadWindow()
		self.wndCube.Hide()

	if app.ENABLE_SASH_SYSTEM:
		def __MakeSashWindow(self):
			self.wndSashCombine = uisash.CombineWindow()
			self.wndSashCombine.LoadWindow()
			self.wndSashCombine.Hide()
			
			self.wndSashAbsorption = uisash.AbsorbWindow()
			self.wndSashAbsorption.LoadWindow()
			self.wndSashAbsorption.Hide()
			
			if self.wndInventory:
				self.wndInventory.SetSashWindow(self.wndSashCombine, self.wndSashAbsorption)
				self.wndInventoryNew.SetSashWindow(self.wndSashCombine, self.wndSashAbsorption)
				
	def __MakeCubeResultWindow(self):
		self.wndCubeResult = uiCube.CubeResultWindow()
		self.wndCubeResult.LoadWindow()
		self.wndCubeResult.Hide()
	def __MakeCardsInfoWindow(self):
		self.wndCardsInfo = uiCards.CardsInfoWindow()
		self.wndCardsInfo.LoadWindow()
		self.wndCardsInfo.Hide()
		
	def __MakeCardsWindow(self):
		self.wndCards = uiCards.CardsWindow()
		self.wndCards.LoadWindow()
		self.wndCards.Hide()
		
	def __MakeCardsIconWindow(self):
		self.wndCardsIcon = uiCards.IngameWindow()
		self.wndCardsIcon.LoadWindow()
		self.wndCardsIcon.Hide()

	if app.ENABLE_MINI_GAME_CATCH_KING:
		def __MakeCatchKingIconWindow(self):
			self.wndCatchKingIcon = uiMiniGameCatchKing.IngameWindow()
			self.wndCatchKingIcon.LoadWindow()
			self.wndCatchKingIcon.Hide()

	# ACCESSORY_REFINE_ADD_METIN_STONE
	def __MakeItemSelectWindow(self):
		self.wndItemSelect = uiselectitem.SelectItemWindow()
		self.wndItemSelect.Hide()
	# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

	#def __MakeCalendar(self):
		#self.wndCalendarADM = uicalendar.UiCalendarAdm()
		#self.wndCalendarADM.LoadWindow()
		#self.wndCalendarADM.Hide()

		#self.wndCalendar = uicalendar.UiCalendar()
		#self.wndCalendar.LoadWindow()
		#self.wndCalendar.()
		#self.wndCalendar.Close()
		
		#self.wndCalendar = uiEventCalendar.EventCalendarWindow()
		#self.wndCalendar.ClearEvents()
		#self.wndCalendar.Close()

		#self.wndCalendarIcon = uicalendar.CalendarIngameWindow()
		#self.wndCalendarIcon.LoadWindow()
		#self.wndCalendarIcon.Show()

	if app.ENABLE_DUNGEON_INFO:
		def __MakeTableDungeonInfo(self):
			self.wndTableDungeonInfo = uitabledungeon.TableDungeonWindows()
			self.wndTableDungeonInfo.Hide()


	if app.ENABLE_DROP_ITEM_WORLDARD:
		def __MakeDropItem(self):
			import uidropitem
			self.wndDropItem = uidropitem.DropItemWindows()
			self.wndDropItem.Hide()
		def OpenDropItem(self):
			if self.wndDropItem == None:
				self.__MakeDropItem()
			if self.wndDropItem.IsShow():
				self.wndDropItem.Close()
			else:
				self.wndDropItem.Show()

	if app.ENABLE_CUBE_RENEWAL_WORLDARD:
		def __MakeCubeRenewal(self):
			import uicuberenewal
			self.wndCubeRenewal = uicuberenewal.CubeRenewalWindows()
			self.wndCubeRenewal.BindToolTipItem(self.tooltipItem)
			self.wndCubeRenewal.BindInterface(self)
			self.wndCubeRenewal.Hide()


		def __MakeBattlePass(self):
			return

	def __MakeRenderTooltip(self):
		import uirendertooltip
		self.rendertooltip = uirendertooltip.RenderTooltip(self.tooltipItem)
		self.rendertooltip.CloseRenderTooltip()

	def MakeInterface(self):
		self.__MakeMessengerWindow()
		self.__MakeGuildWindow()
		self.__MakeChatWindow()
		self.__MakeParty()
		self.__MakeWindows()
		self.__MakeDialogs()

		self.__MakeUICurtain()
		self.__MakeTaskBar()
		self.__MakeGameButtonWindow()
		self.__MakeHelpWindow()
		self.__MakeTipBoard()
		self.__MakeWebWindow()

		self.__MakeTableBonus()
		self.__MakeBattlePass()

		if app.ENABLE_CUBE_RENEWAL_WORLDARD:
			self.__MakeCubeRenewal()

		self.__MakeCubeWindow()
		if app.ENABLE_VOICE_CHAT:
			self.wndVoiceChatConfig = uiVoiceChatConfig.VoiceChatConfig()
			self.wndVoiceChatOverlay = uiVoiceChatConfig.SpeakerQueue()
		if app.ENABLE_SASH_SYSTEM:
			self.__MakeSashWindow()
		self.__MakeCubeResultWindow()
		if app.AUTO_SHOUT:
			self.__MakeShoutWindow()
		self.__MakeCardsInfoWindow()
		self.__MakeCardsWindow()
		self.__MakeCardsIconWindow()
		if app.ENABLE_MINI_GAME_CATCH_KING:
			self.__MakeCatchKingIconWindow()
		#self.__MakeCalendar()

		if app.ENABLE_DROP_ITEM_WORLDARD:
			self.__MakeDropItem()

		if app.ENABLE_DUNGEON_INFO:
			self.__MakeTableDungeonInfo()

		self.__MakeRenderTooltip()
		if app.ENABLE_SWITCHBOT_WORLDARD:
			self.__MakeSwitchBot()
		# ACCESSORY_REFINE_ADD_METIN_STONE
		self.__MakeItemSelectWindow()
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

		self.questButtonList = []
		if app.ENABLE_QUEST_RENEWAL:
			self.__MakeQuestButton()
		self.whisperButtonList = []
		self.whisperDialogDict = {}
		
		if app.ENABLE_ZODIAC_MISSION:
			self.__Make12ziTimerWindow()
			self.__Make12ziRewardWindow()
		if app.ENABLE_BATTLEPASS:
			self.CreateBattlePassWindow()
		self.wndInventory.SetItemToolTip(self.tooltipItem)
		self.wndInventoryNew.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_AURA_SYSTEM:
			self.wndAura.SetItemToolTip(self.tooltipItem)

		if app.ENABLE_SASH_SYSTEM:
			self.wndSashAbsorption.SetItemToolTip(self.tooltipItem)
			self.wndSashCombine.SetItemToolTip(self.tooltipItem)
			
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetItemToolTip(self.tooltipItem)
			self.wndDragonSoulRefine.SetItemToolTip(self.tooltipItem)
		self.wndSafebox.SetItemToolTip(self.tooltipItem)
		self.wndCube.SetItemToolTip(self.tooltipItem)
		self.wndCubeResult.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_SPECIAL_STORAGE:
			self.wndSpecialStorage.SetItemToolTip(self.tooltipItem)
		# ITEM_MALL
		self.wndMall.SetItemToolTip(self.tooltipItem)
		# END_OF_ITEM_MALL
		if app.ENABLE_SUNG_MAHI_TOWER:
			self.wndSungMahiWindow.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_SWITCHBOT:
			self.wndSwitchbot.SetItemToolTip(self.tooltipItem)

		self.wndCharacter.SetSkillToolTip(self.tooltipSkill)
		self.wndTaskBar.SetItemToolTip(self.tooltipItem)
		self.wndTaskBar.SetSkillToolTip(self.tooltipSkill)
		self.wndGuild.SetSkillToolTip(self.tooltipSkill)
		# ACCESSORY_REFINE_ADD_METIN_STONE
		self.wndItemSelect.SetItemToolTip(self.tooltipItem)
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

		self.dlgShop.SetItemToolTip(self.tooltipItem)
		self.dlgExchange.SetItemToolTip(self.tooltipItem)

		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			self.wndAutoWindow.SetSkillToolTip(self.tooltipSkill)
			self.wndAutoWindow.SetItemToolTip(self.tooltipItem)

		self.__InitWhisper()
		self.DRAGON_SOUL_IS_QUALIFIED = False

	if app.LINK_IN_CHAT:
		def AnswerOpenLink(self, answer):
			if not self.OpenLinkQuestionDialog:
				return

			self.OpenLinkQuestionDialog.Close()
			self.OpenLinkQuestionDialog = None

			if not answer:
				return

			link = constInfo.link
			
	def ShowRankInfo(self):
		if self.wndRankInfo.IsShow():
			self.wndRankInfo.Close()
		else:
			self.wndRankInfo.Show()
			
	def AppendInfoRankGlobal(self, mode, my_pos, pos, name, value, empire, level, guildname):
		if self.wndRankInfo:
			self.wndRankInfo.AppendInfo(mode, my_pos, pos, name, value, empire, level, guildname)

	def MakeHyperlinkTooltip(self, hyperlink):
		tokens = hyperlink.split(":")
		if tokens and len(tokens):
			type = tokens[0]
			if "item" == type:
				self.hyperlinkItemTooltip.SetHyperlinkItem(tokens)
			elif "msg" == type and str(tokens[1]) != player.GetMainCharacterName():
				self.OpenWhisperDialog(str(tokens[1]))				
			elif app.LINK_IN_CHAT and "web" == type and tokens[1].startswith("httpXxX") or "web" == type and tokens[1].startswith("httpsXxX"):
					os.system("start " + tokens[1].replace("XxX", "://").replace("&","^&"))
			elif app.LINK_IN_CHAT and "sysweb" == type:
				os.system("start " + tokens[1].replace("XxX", "://"))
			elif "chat_mp" == type:	
				self.OpenWhisperDialog(str(tokens[1]))
			elif "wiki" == type:
				self.OpenWikiWindow()
				self.wndWiki.OpenRules()

	## Make Windows & Dialogs
	################################
	def OpenGift(self):
		if self.wndGameButton:
			self.wndGameButton.ShowGiftButton()
	
	def Close(self):
		if app.USE_CRYSTAL_SYSTEM and self.wndShardCraftWnd:
			self.wndShardCraftWnd.Destroy()
			del self.wndShardCraftWnd
			self.wndShardCraftWnd = None

		if app.USE_CAPTCHA_SYSTEM and self.wndCaptcha:
			self.wndCaptcha.Destroy()
			del self.wndCaptcha
			self.wndCaptcha = None

		if app.ENABLE_AURA_SYSTEM:
			if self.wndAura:
				self.wndAura.Destroy()
			del self.wndAura

		if self.wndVote4Buff:
			self.wndVote4Buff.Hide()
			self.wndVote4Buff.Destroy()

		if self.wndRankInfo:
			self.wndRankInfo.Hide()
			self.wndRankInfo.Destroy()
			self.wndRankInfo = None
			
		if self.wndReinforceName:
			self.wndReinforceName.Hide()
			self.wndReinforceName.Destroy()

		if app.ENABLE_FISHING_GAME:
			if self.wndFishingGame:
				self.wndFishingGame.Destroy()

		if app.__SPIN_WHEEL__:
			if self.wndSpinWheel:
				self.wndSpinWheel.Close()
				self.wndSpinWheel.Destroy()
				self.wndSpinWheel = None
		if app.__ROULETTE__:
			if self.wndRoulette:
				self.wndRoulette.Close()
				self.wndRoulette.Destroy()
				self.wndRoulette = None
		if app.__AUTO_HUNT__:
			if self.wndAutoHunt:
				self.wndAutoHunt.Close()
				self.wndAutoHunt.Destroy()
				self.wndAutoHunt = None

		if app.__AUTO_SKILL_READER__ and self.wndAutoSkillReader:
			self.wndAutoSkillReader.Hide()
			self.wndAutoSkillReader.Destroy()
			self.wndAutoSkillReader = None

		if app.__DUNGEON_INFO__:
			if self.wndDungeonTrack:
				self.wndDungeonTrack.Close()
				self.wndDungeonTrack.Destroy()
				self.wndDungeonTrack = None
			if self.wndDungeonManager:
				self.wndDungeonManager.Destroy()
				self.wndDungeonManager = None
		if app.RENEWAL_MISSION_BOOKS:
			if self.wndBookMission:
				self.wndBookMission.Close()
				self.wndBookMission.Destroy()
				self.wndBookMission = None
		if app.ENABLE_GEM_SYSTEM:
			if self.wndGemShop:
				self.wndGemShop.Close()
				self.wndGemShop.Destroy()
				self.wndGemShop=None
		if app.ENABLE_EXCHANGE_LOG:
			if self.wndExchangeLog:
				self.wndExchangeLog.Close()
				self.wndExchangeLog.Destroy()
				self.wndExchangeLog = None
		if app.ENABLE_FISH_GAME:
			if self.wndFishGame:
				self.wndFishGame.Hide(True)
				self.wndFishGame.Destroy()
				self.wndFishGame=None
		if app.ENABLE_TRACK_WINDOW:
			if self.wndTrackWindow:
				self.wndTrackWindow.Close()
				self.wndTrackWindow.Destroy()
				self.wndTrackWindow=None
		if app.ENABLE_RENDER_TARGET:
			if self.wndRenderTarget:
				self.wndRenderTarget.Close()
				self.wndRenderTarget.Destroy()
				self.wndRenderTarget=None
		if app.ENABLE_REWARD_SYSTEM:
			if self.wndReward:
				self.wndReward.Hide(True)
				self.wndReward.Destroy()
				self.wndReward = None
		if self.dlgWhisperWithoutTarget:
			self.dlgWhisperWithoutTarget.Hide(True)
			self.dlgWhisperWithoutTarget.Destroy()
			del self.dlgWhisperWithoutTarget

		if app.ENABLE_ZODIAC_MISSION:
			if self.wnd12ziTimer:
				self.wnd12ziTimer.Hide(True)
				self.wnd12ziTimer=None
			if self.wnd12ziReward:
				self.wnd12ziReward.Hide(True)
				self.wnd12ziReward=None

		if uiQuest.QuestDialog.__dict__.has_key("QuestCurtain"):
			uiQuest.QuestDialog.QuestCurtain.Close()

		if self.wndQuestWindow:
			for key, eachQuestWindow in self.wndQuestWindow.items():
				eachQuestWindow.nextCurtainMode = -1
				eachQuestWindow.Hide()
				eachQuestWindow.CloseSelf()
				eachQuestWindow = None
		self.wndQuestWindow = {}
		
		if app.ENABLE_WIKI:
			if self.wndWiki:
				self.wndWiki.Close()
				self.wndWiki.Destroy()
				self.wndWiki=None

		if app.ENABLE_AUTO_SELECT_SKILL:
			if self.wndAutoSkill:
				self.wndAutoSkill.Hide(True)
				self.wndAutoSkill.Destroy()
				self.wndAutoSkill=None

		if app.ENABLE_EVENT_MANAGER:
			if self.wndEventManager:
				self.wndEventManager.Hide(True)
				self.wndEventManager.Destroy()
				self.wndEventManager = None
			if self.wndEventIcon:
				self.wndEventIcon.Hide(True)
				self.wndEventIcon.Destroy()
				self.wndEventIcon = None
	
		if app.ENABLE_ITEMSHOP:
			if self.wndItemShop:
				self.wndItemShop.Hide(True)
				self.wndItemShop.Destroy()
				self.wndItemShop = None
	
		if app.ENABLE_DUNGEON_INFO:
			if self.wndDungeonInfo:
				self.wndDungeonInfo.Close()
				self.wndDungeonInfo.Destroy()
				self.wndDungeonInfo = None

		if app.ENABLE_MINI_GAME_CATCH_KING:
			if self.wndCatchKingGame:
				self.wndCatchKingGame.Destroy()
				del self.wndCatchKingGame
			if self.wndCatchKingIcon:
				self.wndCatchKingIcon.Destroy()
				del self.wndCatchKingIcon

		if self.wndGiftBox:
			self.wndGiftBox.Clear()
			self.wndGiftBox.Hide(True)
			self.wndGiftBox.Destroy()
		del self.wndGiftBox
		

		if app.ENABLE_CHANNEL_CHANGE:
			if self.wndChangeChannel:
				self.wndChangeChannel.Hide(True)
				self.wndChangeChannel.Destroy()
				self.wndChangeChannel = None

		if app.ENABLE_NEW_PET_SYSTEM:
			if self.wndPet:
				self.wndPet.Close()
				self.wndPet.Destroy()
				del self.wndPet
				self.wndPet = None

			if self.change_window:
				self.change_window.Hide(True)
				self.change_window.Destroy()
				del self.change_window
				self.change_window = None

		#if self.wndDiscord:
			#self.wndDiscord.Destroy()

		#if self.wndGuias:
			#self.wndGuias.Destroy()

		if self.wndChat:
			self.wndChat.Hide(True)
			self.wndChat.Destroy()

		if self.wndTaskBar:
			self.wndTaskBar.Hide(True)
			self.wndTaskBar.Destroy()

		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Hide(True)
			self.wndExpandedTaskBar.Destroy()

		if self.wndCharacter:
			self.wndCharacter.Hide(True)
			self.wndCharacter.Destroy()

		if app.BL_67_ATTR:
			if self.wndAttr67Add:
				del self.wndAttr67Add

		if self.wndInventory:
			self.wndInventory.Hide(True)
			self.wndInventory.Destroy()
		if self.wndInventoryNew:
			self.wndInventoryNew.Hide(True)
			self.wndInventoryNew.Destroy()

		if self.wndDragonSoul:
			self.wndDragonSoul.Hide(True)
			self.wndDragonSoul.Destroy()

		if self.wndDragonSoulRefine:
			self.wndDragonSoulRefine.Hide(True)
			self.wndDragonSoulRefine.Destroy()

		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			if self.wndAutoWindow:
				self.wndAutoWindow.Destroy()

		if app.ENABLE_SPECIAL_STORAGE:
			if self.wndSpecialStorage:
				self.wndSpecialStorage.Hide()
				self.wndSpecialStorage.Destroy()
		if self.dlgExchange:
			self.dlgExchange.Hide(True)
			self.dlgExchange.Destroy()

		if self.dlgPointReset:
			self.dlgPointReset.Hide(True)
			self.dlgPointReset.Destroy()

		if self.dlgShop:
			self.dlgShop.Hide(True)
			self.dlgShop.Destroy()

		if self.wndTicket:
			self.wndTicket.Hide(True)
			self.wndTicket.Destroy()
			
		# if self.wndItemShop:
			# self.wndItemShop.Destroy()
			
		if self.dlgRestart:
			self.dlgRestart.Hide(True)
			self.dlgRestart.Destroy()

		if self.dlgSystem:
			self.dlgSystem.Hide(True)
			self.dlgSystem.Destroy()

		if self.dlgPassword:
			self.dlgPassword.Hide(True)
			self.dlgPassword.Destroy()

		if self.wndMiniMap:
			self.wndMiniMap.Hide(True)
			self.wndMiniMap.Destroy()

		if self.wndSafebox:
			self.wndSafebox.Hide(True)
			self.wndSafebox.Destroy()

		#if self.wndWeb:
		#	self.wndWeb.Hide(True)
		#	self.wndWeb.Destroy()
		#	self.wndWeb = None

		if self.wndMall:
			self.wndMall.Hide(True)
			self.wndMall.Destroy()

		if self.Itemshop_v2:
			self.Itemshop_v2.Hide(True)
			self.Itemshop_v2.Destroy()
			
		if app.ENABLE_MANAGER_BANK_SYSTEM:
			if self.managerAccountBank:
				self.managerAccountBank.Hide(True)
				self.managerAccountBank.Destroy()
			
		if self.wndParty:
			self.wndParty.Destroy()


		if self.wndczestadisticas:
			self.wndczestadisticas.Close()
			self.wndczestadisticas.Destroy()

		if self.wndAlmacenMenu:
			self.wndAlmacenMenu.Close()
			self.wndAlmacenMenu.Destroy()

		if self.wndHelp:
			self.wndHelp.Hide(True)
			self.wndHelp.Destroy()
		if self.wndCardsInfo:
			self.wndCardsInfo.Destroy()

		if self.wndCards:
			self.wndCards.Hide(True)
			self.wndCards.Destroy()

		if self.wndCardsIcon:
			self.wndCardsIcon.Hide(True)
			self.wndCardsIcon.Destroy()
		if self.wndCube:
			self.wndCube.Hide(True)
			self.wndCube.Destroy()
		
		if app.ENABLE_IKASHOP_RENEWAL:
			if self.wndIkashopBusiness:
				self.wndIkashopBusiness.Destroy()
				self.wndIkashopBusiness = None

			if self.wndIkashopSearchShop:
				self.wndIkashopSearchShop.Destroy()
				self.wndIkashopSearchShop = None

			if app.USE_OFFLINESHOP_SEARCH_V2 and self.wndIkashopSearchShopV2:
				self.wndIkashopSearchShopV2.Destroy()
				del self.wndIkashopSearchShopV2
				self.wndIkashopSearchShopV2 = None

			if app.EXTEND_IKASHOP_ULTIMATE:
				if self.wndIkashopShopSkin:
					self.wndIkashopShopSkin.Destroy()
					self.wndIkashopShopSkin = None

		if self.wndCubeResult:
			self.wndCubeResult.Hide(True)
			self.wndCubeResult.Destroy()


		if app.ENABLE_DUNGEON_INFO:
			if self.wndTableDungeonInfo:
				self.wndTableDungeonInfo.Hide(True)
				self.wndTableDungeonInfo.Close()
				self.wndTableDungeonInfo.Destroy()

		if self.rendertooltip:
			self.rendertooltip.Hide(True)
			self.rendertooltip.Destroy()

		if self.wndMessenger:
			self.wndMessenger.Hide(True)
			self.wndMessenger.Destroy()

		if self.wndGuild:
			self.wndGuild.Hide(True)
			self.wndGuild.Destroy()

		if app.AUTO_SHOUT:
			if self.wndShout:
				self.wndShout.Destroy()

		#if self.wndCalendarIcon:
			#self.wndCalendarIcon.Show()

		#if self.wndCalendarADM:
			#self.wndCalendarADM.Destroy()

		#if self.wndCalendar:
		#	self.wndCalendar.Hide()
		#	self.wndCalendar.Destroy()

		if self.dlgRefineNew:
			self.dlgRefineNew.Hide(True)
			self.dlgRefineNew.Close()
			self.dlgRefineNew.Destroy()

		if self.wndGuildBuilding:
			self.wndGuildBuilding.Hide(True)
			self.wndGuildBuilding.Destroy()

		if app.ENABLE_VOICE_CHAT:
			if self.wndVoiceChatConfig:
				self.wndVoiceChatConfig.Hide()
				self.wndVoiceChatConfig.Destroy()
				del self.wndVoiceChatConfig

			if self.wndVoiceChatOverlay:
				self.wndVoiceChatOverlay.Hide()
				self.wndVoiceChatOverlay.Destroy()
				del self.wndVoiceChatOverlay

		if self.wndGameButton:
			self.wndGameButton.Hide(True)
			self.wndGameButton.Destroy()
		
		if app.ENABLE_SHOW_CHEST_DROP:
			if self.wndChestDropInfo:
				del self.wndChestDropInfo

		# ITEM_MALL
		#if self.mallPageDlg:
		#	self.mallPageDlg.Hide(True)
		#	self.mallPageDlg.Destroy()

		# END_OF_ITEM_MALL
		# ACCESSORY_REFINE_ADD_METIN_STONE
		if self.wndItemSelect:
			self.wndItemSelect.Hide(True)
			self.wndItemSelect.Destroy()
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
		if app.ENABLE_SUNG_MAHI_TOWER:
			if self.wndSungMahiWindow:
				self.wndSungMahiWindow.Destroy()

		if app.ENABLE_DROP_ITEM_WORLDARD:
			if self.wndDropItem:
				self.wndDropItem.Hide(True)
				self.wndDropItem.Close()
				self.wndDropItem.Destroy()

		if app.ENABLE_BATTLEPASS:
			if self.wndBattlePass:
				self.wndBattlePass.Hide()
				del self.wndBattlePass

		if app.ENABLE_CUBE_RENEWAL_WORLDARD:
			if self.wndCubeRenewal:
				self.wndCubeRenewal.Hide(True)
				self.wndCubeRenewal.Close()
				self.wndCubeRenewal.Destroy()

		if self.wndBattlePassButton:
			self.wndBattlePassButton.Hide(True)
			self.wndBattlePassButton.Destroy()
			#self.wndBattlePassButton.Close()

		if self.wndTableBonus:
			self.wndTableBonus.Hide(True)
			self.wndTableBonus.Close()
			self.wndTableBonus.Destroy()

		if app.ENABLE_BIYOLOG:
			if self.wndBio:
				self.wndBio.Hide(True)
				self.wndBio.Destroy()
				del self.wndBio

		if app.ENABLE_SWITCHBOT_WORLDARD:
			if self.wndSwitchBot:
				self.wndSwitchBot.Hide(True)
				self.wndSwitchBot.Close()
				self.wndSwitchBot.Destroy()
				del self.wndSwitchBot
				
		if app.ENABLE_GAME_OPTION_ESCAPE:
			if self.wndEscapePopup:
				self.wndEscapePopup.Destroy()
				del self.wndEscapePopup

		if self.wndExpandedMoneyTaskBar:
			self.wndExpandedMoneyTaskBar.Destroy()
			del self.wndExpandedMoneyTaskBar
			self.wndExpandedMoneyTaskBar = None

		if app.ENABLE_SWITCHBOT:
			if self.wndSwitchbot:
				self.wndSwitchbot.Destroy()
				self.wndSwitchbot.Hide()

		self.wndChatLog.Destroy()

		for btn in self.questButtonList:
			btn.SetEvent(0)
			btn.Hide()
		for btn in self.whisperButtonList:
			btn.SetEvent(0)
		for dlg in self.whisperDialogDict.itervalues():
			dlg.Hide()
			dlg.Destroy()
		for brd in self.guildScoreBoardDict.itervalues():
			brd.Hide()
			brd.Destroy()
		for dlg in self.equipmentDialogDict.itervalues():
			dlg.Hide()
			dlg.Destroy()


		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			if self.wndAutoWindow:
				del self.wndAutoWindow

		del self.wndczestadisticas


		del self.wndAlmacenMenu

		# ITEM_MALL
		#del self.mallPageDlg
		# END_OF_ITEM_MALL
		
		if app.ENABLE_DROP_ITEM_WORLDARD:
			del self.wndDropItem
		if app.ENABLE_FISHING_GAME:
			if self.wndFishingGame:
				del self.wndFishingGame

		del self.Itemshop_v2
		if app.ENABLE_MANAGER_BANK_SYSTEM:
			del self.managerAccountBank
		del self.wndGuild
		del self.wndMessenger
		del self.wndUICurtain
		del self.wndChat
		del self.wndReinforceName

		del self.wndBattlePassButton
		del self.wndVote4Buff

		if app.ENABLE_CUBE_RENEWAL_WORLDARD:
			del self.wndCubeRenewal

		del self.wndTableBonus
		del self.wndTaskBar
		if self.wndExpandedTaskBar:
			del self.wndExpandedTaskBar
		del self.wndCharacter
		del self.wndInventory
		del self.wndInventoryNew
		if self.wndDragonSoul:
			del self.wndDragonSoul
		if self.wndDragonSoulRefine:
			del self.wndDragonSoulRefine
		if app.ENABLE_SPECIAL_STORAGE:
			if self.wndSpecialStorage:
				del self.wndSpecialStorage
		del self.dlgExchange
		del self.dlgPointReset
		del self.dlgShop
		del self.dlgRestart
		del self.dlgSystem
		del self.dlgPassword
		del self.hyperlinkItemTooltip
		del self.tooltipItem
		del self.tooltipSkill
		del self.wndMiniMap
		del self.wndSafebox
		del self.wndMall
		del self.wndParty
		del self.wndHelp
		del self.wndCardsInfo
		del self.wndCards
		del self.wndCardsIcon
		del self.wndCube
		del self.wndCubeResult
		del self.inputDialog
		del self.wndChatLog
		del self.dlgRefineNew
		del self.wndGuildBuilding
		del self.wndGameButton
		del self.tipBoard
		del self.bigBoard
		if app.dracaryS_DUNGEON_LIB:
			del self.missionBoard
			if self.wndDungeonTimer:
				self.wndDungeonTimer.Hide()
				self.wndDungeonTimer=0
		del self.wndItemSelect
		if app.ENABLE_SWITCHBOT:
			del self.wndSwitchbot	
		if app.__BL_BATTLE_ROYALE__:
			if self.wndBattleRoyaleApplication:
				del self.wndBattleRoyaleApplication
			
			if self.wndBattleRoyaleInterface:
				del self.wndBattleRoyaleInterface
		if app.AUTO_SHOUT:
			del self.wndShout
		del self.wndTicket
		if app.ENABLE_RANKING:
			if self.wndRanking:
				self.wndRanking.Destroy()
				del self.wndRanking
		#del self.wndDiscord
		#del self.wndGuias
		if app.ENABLE_SUNG_MAHI_TOWER:
			del self.wndSungMahiWindow

		if app.ENABLE_DUNGEON_INFO:
			del self.wndTableDungeonInfo


		#del self.wndCalendarIcon
		#del self.wndCalendarADM
		#del self.wndCalendar

		del self.rendertooltip

		# del self.wndItemShop
		self.questButtonList = []
		self.whisperButtonList = []
		self.whisperDialogDict = {}

		self.guildScoreBoardDict = {}
		self.equipmentDialogDict = {}

		if app.ENABLE_KINGDOMS_WAR:
			if self.wndKingdomsWar:
				self.wndKingdomsWar.Hide()
				self.wndKingdomsWar.Destroy()
			
			del self.wndKingdomsWar

		event.SetInterfaceWindow(None)
		uiChat.DestroyChatInputSetWindow()
	def OpenVoteWindow(self):
		if self.wndVote4Buff.IsShow():
			self.wndVote4Buff.Close()
		else:
			self.wndVote4Buff.Show()

	## Skill
	def OnUseSkill(self, slotIndex, coolTime):
		self.wndCharacter.OnUseSkill(slotIndex, coolTime)
		self.wndTaskBar.OnUseSkill(slotIndex, coolTime)
		self.wndGuild.OnUseSkill(slotIndex, coolTime)
		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			self.wndAutoWindow.OnUseSkill()
			
	if app.ENABLE_FISHING_GAME:
		def OpenFishingGameWindow(self, level, fishing_key):
			if self.wndFishingGame:
				self.wndFishingGame.Open(level, fishing_key)

		def CloseFishingGameWindow(self):
			if self.wndFishingGame:
				self.wndFishingGame.Close()

		def IsFishGameOpen(self):
			if self.wndFishingGame and self.wndFishingGame.IsShow():
				return True
			return False

	def OnActivateSkill(self, slotIndex):
		self.wndCharacter.OnActivateSkill(slotIndex)
		self.wndTaskBar.OnActivateSkill(slotIndex)
		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			self.wndAutoWindow.OnActivateSkill()

	def OnDeactivateSkill(self, slotIndex):
		self.wndCharacter.OnDeactivateSkill(slotIndex)
		self.wndTaskBar.OnDeactivateSkill(slotIndex)
		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			self.wndAutoWindow.OnDeactivateSkill(slotIndex)

	def OnChangeCurrentSkill(self, skillSlotNumber):
		self.wndTaskBar.OnChangeCurrentSkill(skillSlotNumber)

	def SelectMouseButtonEvent(self, dir, event):
		self.wndTaskBar.SelectMouseButtonEvent(dir, event)
		
	def ShowNameReinforce(self, arg):
		if self.wndReinforceName.IsShow():
			self.wndReinforceName.Hide()
		else:
			self.wndReinforceName.OpenHatchWindow(arg)

	if app.BL_67_ATTR:
		def OpenAttr67AddDlg(self):
			if self.wndAttr67Add:
				self.wndAttr67Add.Show()

		if app.WJ_ENABLE_TRADABLE_ICON:
			def IsAttr67RegistItem(self):
				return self.wndAttr67Add and self.wndAttr67Add.RegistSlotIndex != -1

			def IsAttr67SupportItem(self):
				return self.wndAttr67Add and self.wndAttr67Add.SupportSlotIndex != -1

	## Refresh
	def RefreshAlignment(self):
		self.wndCharacter.RefreshAlignment()

	def RefreshStatus(self):
		self.wndTaskBar.RefreshStatus()
		self.wndCharacter.RefreshStatus()

		self.wndInventoryNew.RefreshStatus()

		if self.wndExpandedMoneyTaskBar:
			self.wndExpandedMoneyTaskBar.RefreshStatus()

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.RefreshStatus()

		if self.wndTableBonus:
			self.wndTableBonus.RefreshBonus()

		#if self.wndczestadisticas:
		#	self.wndczestadisticas.RefreshStatus()

		if app.__AUTO_SKILL_READER__ and self.wndAutoSkillReader and self.wndAutoSkillReader.IsShow():
			self.wndAutoSkillReader.Refresh()

	if app.ENABLE_HIDE_COSTUME_SYSTEM:
		def costume_hide_clear(self):
			self.wndInventory.costume_hide_clear()
		def costume_hide_list(self,slot,index):
			self.wndInventory.costume_hide_list(slot,index)
		def costume_hide_load(self):
			self.wndInventory.costume_hide_load()
			
	def RefreshStamina(self):
		self.wndTaskBar.RefreshStamina()

	def RefreshSkill(self):
		self.wndCharacter.RefreshSkill()
		self.wndTaskBar.RefreshSkill()

	def RefreshInventory(self):
		self.wndTaskBar.RefreshQuickSlot()
		self.wndInventory.RefreshItemSlot()
		self.wndInventoryNew.RefreshItemSlot()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.RefreshItemSlot()
		if app.ENABLE_SPECIAL_STORAGE:
			self.wndSpecialStorage.RefreshItemSlot()
		if app.ENABLE_SWITCHBOT_WORLDARD:
			if self.wndSwitchBot:
				self.wndSwitchBot.RefreshItemSlot()
		if app.ENABLE_AURA_SYSTEM:
			if player.IsAuraRefineWindowOpen():
				if self.wndAura and self.wndAura.IsShow():
					self.wndAura.RefreshAuraWindow()

		if app.__AUTO_SKILL_READER__ and self.wndAutoSkillReader and self.wndAutoSkillReader.IsShow():
			self.wndAutoSkillReader.Refresh()

	def RefreshCharacter(self): ## Character ÆäÀÌÁöÀÇ ¾ó±¼, Inventory ÆäÀÌÁöÀÇ Àü½Å ±×¸² µîÀÇ Refresh
		self.wndCharacter.RefreshCharacter()
		self.wndTaskBar.RefreshQuickSlot()

	if not app.ENABLE_QUEST_RENEWAL:
		def RefreshQuest(self):
			self.wndCharacter.RefreshQuest()

	def RefreshSafebox(self):
		#self.wndSafebox.RefreshSafebox()
		self.wndInventoryNew.RefreshSafebox()

	# ITEM_MALL
	def RefreshMall(self):
		#self.wndSafebox.RefreshMall()
		self.wndInventoryNew.RefreshMall()

	def OpenItemMall(self):
		pass
#		if not self.mallPageDlg:
#			self.mallPageDlg = uiShop.MallPageDialog()
#
#		self.mallPageDlg.Open()
	# END_OF_ITEM_MALL

	def RefreshMessenger(self):
		self.wndMessenger.RefreshMessenger()

	def RefreshGuildInfoPage(self):
		self.wndGuild.RefreshGuildInfoPage()

	def RefreshGuildBoardPage(self):
		self.wndGuild.RefreshGuildBoardPage()

	def RefreshGuildMemberPage(self):
		self.wndGuild.RefreshGuildMemberPage()

	def RefreshGuildMemberPageGradeComboBox(self):
		self.wndGuild.RefreshGuildMemberPageGradeComboBox()

	def RefreshGuildSkillPage(self):
		self.wndGuild.RefreshGuildSkillPage()

	def RefreshGuildGradePage(self):
		self.wndGuild.RefreshGuildGradePage()

	def DeleteGuild(self):
		self.wndMessenger.ClearGuildMember()
		self.wndGuild.DeleteGuild()

	def RefreshMobile(self):
		self.dlgSystem.RefreshMobile()

	def OnMobileAuthority(self):
		self.dlgSystem.OnMobileAuthority()

	def OnBlockMode(self, mode):
		self.dlgSystem.OnBlockMode(mode)

	## Calling Functions
	# PointReset
	def OpenPointResetDialog(self):
		self.dlgPointReset.Show()
		self.dlgPointReset.SetTop()

	def ClosePointResetDialog(self):
		self.dlgPointReset.Close()

	# Shop
	if (app.WJ_COMBAT_ZONE):
		def OpenShopDialog(self, vid, points, curLimit, maxLimit):
			self.wndInventory.Show()
			self.wndInventory.SetTop()
			self.dlgShop.Open(vid, points, curLimit, maxLimit)
			self.dlgShop.SetTop()
	else:
		def OpenShopDialog(self, vid):
			self.wndInventory.Show()
			self.wndInventory.SetTop()
			self.dlgShop.Open(vid)
			self.dlgShop.SetTop()

	def CloseShopDialog(self):
		self.dlgShop.Close()

	def RefreshShopDialog(self):
		self.dlgShop.Refresh()

	## Quest
	def OpenCharacterWindowQuestPage(self):
		self.wndCharacter.Show()
		self.wndCharacter.SetState("QUEST")

	def OpenQuestWindow(self, skin, idx):

		wnds = ()
		q = uiQuest.QuestDialog(skin, idx)
		q.SetWindowName("QuestWindow" + str(idx))
		q.Show()
		if skin:
			q.Lock()
			wnds = self.__HideWindows()
			q.AddOnDoneEvent(lambda tmp_self, args=wnds: self.__ShowWindows(args))
			q.AddOnCloseEvent(q.Unlock)
		q.AddOnCloseEvent(lambda key = self.wndQuestWindowNewKey:ui.__mem_func__(self.RemoveQuestDialog)(key))
		self.wndQuestWindow[self.wndQuestWindowNewKey] = q
		self.wndQuestWindowNewKey = self.wndQuestWindowNewKey + 1

		# END_OF_UNKNOWN_UPDATE

	def RemoveQuestDialog(self, key):
		del self.wndQuestWindow[key]

	## Exchange
	def StartExchange(self):
		self.dlgExchange.OpenDialog()
		self.dlgExchange.Refresh()

	def EndExchange(self):
		self.dlgExchange.CloseDialog()

	def RefreshExchange(self):
		self.dlgExchange.Refresh()

	if app.WJ_ENABLE_TRADABLE_ICON:
		def CantTradableItemExchange(self, dstSlotIndex, srcSlotIndex):
			self.dlgExchange.CantTradableItem(dstSlotIndex, srcSlotIndex)
		def CantTradableItemExchangeStorage(self, dstSlotIndex, srcSlotIndex, invenType):
			self.dlgExchange.CantTradableItemFromStorage(dstSlotIndex, srcSlotIndex, invenType)


	def BINARY_CALENDAR_OPEN_ADM(self):
		return
		#self.wndCalendarADM.Show()

	def BINARY_CALENDAR_LOAD_EVENTS_ADM(self,index_event,name_event,descrip_event):
		return
		#self.wndCalendarADM.RegistroEventos(index_event,name_event)

	def BINARY_CALENDAR_CLEAR_EVENTS_ADM(self):
		return
		#self.wndCalendarADM.ClearRegistro()

	def BINARY_CALENDAR_LOADING_EVENTS_ADM(self):
		return
		#self.wndCalendarADM.LoadEvents()


	def BINARY_CALENDAR_OPEN(self, currentTimeStamp):
		pass
		#self.wndCalendar.Open(currentTimeStamp)

	def BINARY_CALENDAR_LOAD_EVENTS(self,day,name,image,startAt, endAt,duration):
		pass
		#wndEventCalendar.AppendEvent(1, "Evento Caja Luz de Luna 1", "alquimia.tga", 1633654036, 7200)
		#self.wndCalendar.AppendEvent(day,name,image,startAt, endAt,duration)

	def BINARY_CALENDAR_CLEAR_EVENTS(self):
		pass
		#self.wndCalendar.ClearEvents()

	def BINARY_CALENDAR_LOADING_EVENTS(self):
		pass
		#self.wndCalendar.LoadingEventos()

	## Party
	def AddPartyMember(self, pid, name):
		self.wndParty.AddPartyMember(pid, name)

		self.__ArrangeQuestButton()

	def UpdatePartyMemberInfo(self, pid):
		self.wndParty.UpdatePartyMemberInfo(pid)

	def RemovePartyMember(self, pid):
		self.wndParty.RemovePartyMember(pid)

		##!! 20061026.levites.Äù½ºÆ®_À§Ä¡_º¸Á¤
		self.__ArrangeQuestButton()

	def LinkPartyMember(self, pid, vid):
		self.wndParty.LinkPartyMember(pid, vid)

	def UnlinkPartyMember(self, pid):
		self.wndParty.UnlinkPartyMember(pid)

	def UnlinkAllPartyMember(self):
		self.wndParty.UnlinkAllPartyMember()

	def ExitParty(self):
		self.wndParty.ExitParty()

		##!! 20061026.levites.Äù½ºÆ®_À§Ä¡_º¸Á¤
		self.__ArrangeQuestButton()

	def PartyHealReady(self):
		self.wndParty.PartyHealReady()

	def ChangePartyParameter(self, distributionMode):
		self.wndParty.ChangePartyParameter(distributionMode)

	## Safebox
	def AskSafeboxPassword(self):
		if self.wndSafebox.IsShow():
			return

		# SAFEBOX_PASSWORD
		self.dlgPassword.SetTitle(localeInfo.PASSWORD_TITLE)
		self.dlgPassword.SetSendMessage("/safebox_password ")
		# END_OF_SAFEBOX_PASSWORD

		self.dlgPassword.ShowDialog()

	def OpenSafeboxWindow(self, size):
		self.dlgPassword.CloseDialog()
		#self.wndSafebox.ShowWindow(size)
		self.wndInventoryNew.ShowWindow(size)

	def RefreshSafeboxMoney(self):
		return
		self.wndSafebox.RefreshSafeboxMoney()

	def CommandCloseSafebox(self):
		self.wndInventoryNew.CommandCloseSafebox()
		#self.wndSafebox.CommandCloseSafebox()

	# ITEM_MALL
	def AskMallPassword(self):
		if self.wndMall.IsShow():
			return
		self.dlgPassword.SetTitle(localeInfo.MALL_PASSWORD_TITLE)
		self.dlgPassword.SetSendMessage("/mall_password ")
		self.dlgPassword.ShowDialog()

	def OpenMallWindow(self, size):
		self.dlgPassword.CloseDialog()
		#self.wndMall.ShowWindow(size)
		self.wndInventoryNew.ShowWindow(size)

	def CommandCloseMall(self):
		#self.wndMall.CommandCloseMall()
		self.wndInventoryNew.CommandCloseMall()
	# END_OF_ITEM_MALL

	## Guild
	def OnStartGuildWar(self, guildSelf, guildOpp):
		self.wndGuild.OnStartGuildWar(guildSelf, guildOpp)

		guildWarScoreBoard = uiGuild.GuildWarScoreBoard()
		guildWarScoreBoard.Open(guildSelf, guildOpp)
		guildWarScoreBoard.Show()
		self.guildScoreBoardDict[uiGuild.GetGVGKey(guildSelf, guildOpp)] = guildWarScoreBoard

	def OnEndGuildWar(self, guildSelf, guildOpp):
		self.wndGuild.OnEndGuildWar(guildSelf, guildOpp)

		key = uiGuild.GetGVGKey(guildSelf, guildOpp)

		if not self.guildScoreBoardDict.has_key(key):
			return

		self.guildScoreBoardDict[key].Hide()
		self.guildScoreBoardDict[key].Destroy()
		del self.guildScoreBoardDict[key]

	# GUILDWAR_MEMBER_COUNT
	def UpdateMemberCount(self, gulidID1, memberCount1, guildID2, memberCount2):
		key = uiGuild.GetGVGKey(gulidID1, guildID2)

		if not self.guildScoreBoardDict.has_key(key):
			return

		self.guildScoreBoardDict[key].UpdateMemberCount(gulidID1, memberCount1, guildID2, memberCount2)
	# END_OF_GUILDWAR_MEMBER_COUNT

	def OnRecvGuildWarPoint(self, gainGuildID, opponentGuildID, point):
		key = uiGuild.GetGVGKey(gainGuildID, opponentGuildID)
		if not self.guildScoreBoardDict.has_key(key):
			return

		guildBoard = self.guildScoreBoardDict[key]
		guildBoard.SetScore(gainGuildID, opponentGuildID, point)

	## PK Mode
	def OnChangePKMode(self):
		self.wndCharacter.RefreshAlignment()
		self.dlgSystem.OnChangePKMode()

	if app.ENABLE_AUTOMATIC_PICK_UP_SYSTEM:
		def OnChangePickUPMode(self):
			self.dlgSystem.OnChangePickUPMode()

	## Refine
	def OpenRefineDialog(self, targetItemPos, nextGradeItemVnum, cost, prob, special_storage, type):
		self.dlgRefineNew.Open(targetItemPos, nextGradeItemVnum, cost, prob, special_storage, type)

	def AppendMaterialToRefineDialog(self, vnum, count):
		self.dlgRefineNew.AppendMaterial(vnum, count)

	## Show & Hide
	def ShowDefaultWindows(self):
		self.wndTaskBar.Show()

		if self.wndExpandedMoneyTaskBar:
			self.wndExpandedMoneyTaskBar.Show()

		self.wndMiniMap.Show()
		self.wndMiniMap.ShowMiniMap()

		if app.USE_CAPTCHA_SYSTEM:
			if self.wndChat:
				self.wndChat.Show()

			if self.wndGameButton:
				self.wndGameButton.Show()

			if self.uiAffectShower:
				self.uiAffectShower.Show()

			self.IsHideUIMode = False

	def ShowAllWindows(self):
		self.wndTaskBar.Show()

		if self.wndExpandedMoneyTaskBar:
			self.wndExpandedMoneyTaskBar.Show()

		self.wndCharacter.Show()
		self.wndInventory.Show()

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.Show()
			self.wndDragonSoulRefine.Show()

		self.wndChat.Show()

		if app.ENABLE_RANKING:
			if self.wndRanking:
				self.wndRanking.Open()

		self.wndMiniMap.Show()

		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Show()
			self.wndExpandedTaskBar.SetTop()

		if app.ENABLE_ZODIAC_MISSION:
			if self.wnd12ziTimer:
				self.wnd12ziTimer.Show()
			if self.wnd12ziReward:
				self.wnd12ziReward.Show()

	def HideAllWindows(self):
		if app.USE_CAPTCHA_SYSTEM and self.IsHideUIMode == True:
			return

		if app.ENABLE_SWITCHBOT:
			if self.wndSwitchbot:
				self.wndSwitchbot.Hide()

		if self.wndTaskBar:
			self.wndTaskBar.Hide()

		if self.wndExpandedMoneyTaskBar:
			self.wndExpandedMoneyTaskBar.Hide()

		if app.ENABLE_AURA_SYSTEM:
			if self.wndAura:
				self.wndAura.Hide()

		if self.wndAlmacenMenu:
			self.wndAlmacenMenu.Hide()

		if self.wndczestadisticas:
			self.wndczestadisticas.Hide()

		if self.wndCharacter:
			self.wndCharacter.Close()

		if self.wndInventory:
			self.wndInventory.Hide(True)
		if self.wndInventoryNew:
			self.wndInventoryNew.Hide(True)

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.Hide()
			self.wndDragonSoulRefine.Hide()

		if self.wndChat:
			self.wndChat.hide_btnChatSizing()
			self.wndChat.Hide()

		if app.ENABLE_ZODIAC_MISSION:
			if self.wnd12ziTimer:
				self.wnd12ziTimer.Hide()
			if self.wnd12ziReward:
				self.wnd12ziReward.Hide()

		if self.wndMiniMap:
			self.wndMiniMap.Hide()

		if self.wndMessenger:
			self.wndMessenger.Hide()

		if self.wndGuild:
			self.wndGuild.Hide()

		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Hide()
			
		if app.ENABLE_SUNG_MAHI_TOWER:
			if self.wndSungMahiWindow:
				self.wndSungMahiWindow.Hide()

		if app.ENABLE_BATTLEPASS:
			if self.wndBattlePass:
				self.wndBattlePass.Hide()

		if app.ENABLE_RANKING:
			if self.wndRanking:
				self.wndRanking.Close()

		if app.ENABLE_SHOW_CHEST_DROP:
			if self.wndChestDropInfo:
				self.wndChestDropInfo.Hide()

		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			if self.wndAutoWindow:
				self.wndAutoWindow.Hide()

		if app.USE_CAPTCHA_SYSTEM:
			if self.uiAffectShower:
				self.uiAffectShower.Hide()

			self.IsHideUIMode = True

	def ShowMouseImage(self):
		self.wndTaskBar.ShowMouseImage()

	def HideMouseImage(self):
		self.wndTaskBar.HideMouseImage()

	def ToggleChat(self):
		if True == self.wndChat.IsEditMode():
			self.wndChat.CloseChat()
		else:
			# À¥ÆäÀÌÁö°¡ ¿­·ÈÀ»¶§´Â Ã¤ÆÃ ÀÔ·ÂÀÌ ¾ÈµÊ
			#if self.wndWeb and self.wndWeb.IsShow():
			#	pass
			#else:
			self.wndChat.OpenChat()

	def IsOpenChat(self):
		return self.wndChat.IsEditMode()

	def SetChatFocus(self):
		self.wndChat.SetChatFocus()

	if app.RENEWAL_DEAD_PACKET:
		def OpenRestartDialog(self, d_time):
			self.dlgRestart.OpenDialog(d_time)
			self.dlgRestart.SetTop()
	else:
		def OpenRestartDialog(self):
			self.dlgRestart.OpenDialog()
			self.dlgRestart.SetTop()

	def CloseRestartDialog(self):
		self.dlgRestart.Close()
		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			if self.wndAutoWindow.IsShow():
				self.wndAutoWindow.Close()
				
	if app.ENABLE_SUNG_MAHI_TOWER:
		def SetSungMahiRestartDialog(self):
			self.dlgRestart.SetSungMahiRestartDialog()

	def ToggleSystemDialog(self):
		if False == self.dlgSystem.IsShow():
			self.dlgSystem.OpenDialog()
			self.dlgSystem.SetTop()
		else:
			self.dlgSystem.Close()

	def OpenSystemDialog(self):
		self.dlgSystem.OpenDialog()
		self.dlgSystem.SetTop()

	def ToggleMessenger(self):
		if self.wndMessenger.IsShow():
			self.wndMessenger.Hide()
		else:
			self.wndMessenger.SetTop()
			self.wndMessenger.Show()

	def ToggleMiniMap(self):
		if app.IsPressed(app.DIK_LSHIFT) or app.IsPressed(app.DIK_RSHIFT):
			if False == self.wndMiniMap.isShowMiniMap():
				self.wndMiniMap.ShowMiniMap()
				self.wndMiniMap.SetTop()
			else:
				self.wndMiniMap.HideMiniMap()

		else:
			self.wndMiniMap.ToggleAtlasWindow()

	def PressMKey(self):
		if app.IsPressed(app.DIK_LALT) or app.IsPressed(app.DIK_RALT):
			self.ToggleMessenger()

		else:
			self.ToggleMiniMap()

	def SetMapName(self, mapName):
		self.wndMiniMap.SetMapName(mapName)

	def MiniMapScaleUp(self):
		self.wndMiniMap.ScaleUp()

	def MiniMapScaleDown(self):
		self.wndMiniMap.ScaleDown()

	def ToggleCharacterWindow(self, state):
		if False == player.IsObserverMode():
			if False == self.wndCharacter.IsShow():
				self.OpenCharacterWindowWithState(state)
			else:
				if state == self.wndCharacter.GetState():
					self.wndCharacter.OverOutItem()
					self.wndCharacter.Close()
				else:
					self.wndCharacter.SetState(state)

	def OpenCharacterWindowWithState(self, state):
		if False == player.IsObserverMode():
			self.wndCharacter.SetState(state)
			self.wndCharacter.Show()
			self.wndCharacter.SetTop()

	def ToggleCharacterWindowStatusPage(self):
		self.ToggleCharacterWindow("STATUS")

	def ToggleInventoryWindow(self):
		if False == player.IsObserverMode():
			if False == self.wndInventory.IsShow():
				self.wndInventory.Show()
				self.wndInventory.SetTop()
			else:
				self.wndInventory.OverOutItem()
				self.wndInventory.Close()

	def ToggleInventoryNewWindow(self):
		if False == player.IsObserverMode():
			if False == self.wndInventoryNew.IsShow():
				self.wndInventoryNew.Show()
				self.wndInventoryNew.SetTop()
			else:
				self.wndInventoryNew.OverOutItem()
				self.wndInventoryNew.Close()

	def ToggleExpandedButton(self):
		if False == player.IsObserverMode():
			if False == self.wndExpandedTaskBar.IsShow():
				self.wndExpandedTaskBar.Show()
				self.wndExpandedTaskBar.SetTop()
			else:
				self.wndExpandedTaskBar.Close()

	def DragonSoulActivate(self, deck):
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.ActivateDragonSoulByExtern(deck)

	def DragonSoulDeactivate(self):
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.DeactivateDragonSoul()

	def Highligt_Item(self, inven_type, inven_pos):
		if player.DRAGON_SOUL_INVENTORY == inven_type:
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				self.wndDragonSoul.HighlightSlot(inven_pos)
		elif player.SLOT_TYPE_INVENTORY == inven_type:
			self.wndInventory.HighlightSlot(inven_pos)
		else:
			self.wndInventoryNew.HighlightSlot(inven_type, inven_pos)

	def DragonSoulGiveQuilification(self):
		self.DRAGON_SOUL_IS_QUALIFIED = True
		self.wndExpandedTaskBar.SetToolTipText(uiTaskBar.ExpandedTaskBar.BUTTON_DRAGON_SOUL, uiScriptLocale.TASKBAR_DRAGON_SOUL)

	def ToggleDragonSoulWindow(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoul.IsShow():
					self.wndDragonSoul.Show()
				else:
					self.wndDragonSoul.Close()

	def ToggleDragonSoulWindowWithNoInfo(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoul.IsShow():
					if self.DRAGON_SOUL_IS_QUALIFIED:
						self.wndDragonSoul.Show()
				else:
					self.wndDragonSoul.Close()
	if app.ENABLE_SPECIAL_STORAGE:
		def ToggleSpecialStorageWindow(self):
			return
			if False == player.IsObserverMode():
				if False == self.wndSpecialStorage.IsShow():
					self.wndSpecialStorage.Show()
				else:
					self.wndSpecialStorage.Close()
	def FailDragonSoulRefine(self, reason, inven_type, inven_pos):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.RefineFail(reason, inven_type, inven_pos)

	def SucceedDragonSoulRefine(self, inven_type, inven_pos):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.RefineSucceed(inven_type, inven_pos)

	def OpenDragonSoulRefineWindow(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.Show()
					if None != self.wndDragonSoul:
						if False == self.wndDragonSoul.IsShow():
							self.wndDragonSoul.Show()

	def CloseDragonSoulRefineWindow(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.Close()

	def ToggleGuildWindow(self):
		if not self.wndGuild.IsShow():
			if self.wndGuild.CanOpen():
				self.wndGuild.Open()
			else:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GUILD_YOU_DO_NOT_JOIN)
		else:
			self.wndGuild.OverOutItem()
			self.wndGuild.Hide()

	if app.ENABLE_SWITCHBOT:
		def ToggleSwitchbotWindow(self):
			if self.wndSwitchbot.IsShow():
				self.wndSwitchbot.Close()
			else:
				self.wndSwitchbot.Open()
				
		def RefreshSwitchbotWindow(self):
			if self.wndSwitchbot and self.wndSwitchbot.IsShow():
				self.wndSwitchbot.RefreshSwitchbotWindow()

		def RefreshSwitchbotItem(self, slot):
			if self.wndSwitchbot and self.wndSwitchbot.IsShow():
				self.wndSwitchbot.RefreshSwitchbotItem(slot)

	def ToggleChatLogWindow(self):
		if self.wndChatLog.IsShow():
			self.wndChatLog.Hide()
		else:
			self.wndChatLog.Show()

	if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
		def OnGameOver(self):
			if self.wndAutoWindow:
				self.wndAutoWindow.OnGameOver()

		def ToggleAutoWindow(self):
			if False == player.IsObserverMode():
				if not self.wndAutoWindow.IsShow():
					self.wndAutoWindow.Show()
				else:
					self.wndAutoWindow.Close()

		def SetAutoCooltime(self, slotindex, cooltime):
			self.wndAutoWindow.SetAutoCooltime(slotindex, cooltime)

		def GetAutoStartonoff(self):
			return self.wndAutoWindow.GetAutoStartonoff()

		def RefreshAutoSkillSlot(self):
			if self.wndAutoWindow:
				self.wndAutoWindow.RefreshAutoSkillSlot()

		def RefreshAutoPotionSlot(self):
			if self.wndAutoWindow:
				self.wndAutoWindow.RefreshAutoPotionSlot()

		def AutoOff(self):
			if self.wndAutoWindow:
				self.wndAutoWindow.Close()

			if self.wndExpandedTaskBar:
				self.wndExpandedTaskBar.EnableAutoButton(False)

		def AutoOn(self):
			if self.wndExpandedTaskBar:
				self.wndExpandedTaskBar.EnableAutoButton(True)

		if app.USE_AUTOMATIC_HUNTING_RENEWED:
			def AutoHuntStat(self, leftTime):
				if self.wndAutoWindow:
					self.wndAutoWindow.Stat(leftTime)

			def AutoHuntUpdate(self, leftTime):
				if self.wndAutoWindow:
					self.wndAutoWindow.UpdateTime(leftTime)

	def ToggleAlmacenMenu(self):
		if self.wndAlmacenMenu:
			self.wndAlmacenMenu.Open()


	def OpenCZEstadisticas(self):
		if self.wndczestadisticas:
			self.wndczestadisticas.Show()

	def SetBonusCombatZone(self,arg,type=0,value=0):
		if self.wndczestadisticas:
			self.wndczestadisticas.SetBonus(arg,type,value)

	def SetTimeCombatZone(self,time):
		if self.wndczestadisticas:
			self.wndczestadisticas.SetTime(time)

	def CloseCZEstadisticas(self):
		if self.wndczestadisticas:
			self.wndczestadisticas.Hide()
			
	def CheckGameButton(self):
		if self.wndGameButton:
			self.wndGameButton.CheckGameButton()
			
	if app.ENABLE_SUNG_MAHI_TOWER:
		def ToggleSungMahiWindow(self):
			if self.wndSungMahiWindow.IsShow():
				self.wndSungMahiWindow.Close()
			else:
				self.wndSungMahiWindow.Open()
				
		def UpdateSungMahiNotice(self, noticeType, noticeText):
			if self.wndMiniMap.sungMahiCover.IsShow():
				if noticeType == 1:
					self.wndMiniMap.sungMahiCover.SetLevelDescriptionText(noticeText)
				else:
					self.wndMiniMap.sungMahiCover.SetLevelCurseText(noticeText, noticeType)
			
		def SungMahiClearNotice(self, noticeType):
			if self.wndMiniMap.sungMahiCover.IsShow():
				self.wndMiniMap.sungMahiCover.SungMahiClearNotice(noticeType)
			
		def UpdateRoomLevel(self, roomLevel):
			if self.wndMiniMap.sungMahiCover.IsShow():
				self.wndMiniMap.sungMahiCover.SetRoomLevel(roomLevel)
			
		def UpdateTowerLevel(self, towerLevel):
			if self.wndMiniMap.sungMahiCover.IsShow():
				self.wndMiniMap.sungMahiCover.SetLevelInfo(towerLevel)
			
		def UpdateRoomTime(self, roomTime):
			if self.wndMiniMap.sungMahiCover.IsShow():
				self.wndMiniMap.sungMahiCover.UpdateRoomTime(roomTime)

	def __OnClickStatusPlusButton(self):
		self.ToggleCharacterWindow("STATUS")

	def __OnClickSkillPlusButton(self):
		self.ToggleCharacterWindow("SKILL")

	def __OnClickQuestButton(self):
		self.ToggleCharacterWindow("QUEST")

	def __OnClickHelpButton(self):
		if not app.NEW_SELECT_CHARACTER:
			player.SetPlayTime(1)
		self.CheckGameButton()
		self.OpenHelpWindow()

	def __OnClickBuildButton(self):
		self.BUILD_OpenWindow()

	def OpenHelpWindow(self):
		self.wndUICurtain.Show()
		self.wndHelp.Open()

	def CloseHelpWindow(self):
		self.wndUICurtain.Hide()
		self.wndHelp.Close()

	def OpenWebWindow(self, url):
		pass
		#self.wndWeb.Open(url)

		# À¥ÆäÀÌÁö¸¦ ¿­¸é Ã¤ÆÃÀ» ´Ý´Â´Ù
		#self.wndChat.CloseChat()

	if app.ENABLE_DUNGEON_INFO:
		def DUNGEON_INFO_CHECK_SHOW(self):
			if self.wndTableDungeonInfo:
				if self.wndTableDungeonInfo.IsShow():
					self.wndTableDungeonInfo.Close()
				else:
					import dungeon_info
					dungeon_info.Open()
		def BINARY_TABLE_DUNGEON_INFO_OPEN(self):
			if self.wndTableDungeonInfo:
				self.wndTableDungeonInfo.LoadDateInfo()
				self.wndTableDungeonInfo.Show()

		def BINARY_TABLE_DUNGEON_RANKING_LOAD(self):
			if self.wndTableDungeonInfo:
				self.wndTableDungeonInfo.LoadRanking()

		def BINARY_TABLE_DUNGEON_MISION_LOAD(self):
			if self.wndTableDungeonInfo:
				self.wndTableDungeonInfo.LoadMision()
	
	def CloseRenderTooltip(self):
		self.rendertooltip.CloseRenderTooltip()

	def ShowRenderTooltip(self):
		self.rendertooltip.ShowRenderTooltip()

	def RenderClearDates(self):
		self.rendertooltip.RenderClearDates()

	def SetRenderDates(self, vnum, type, value3 = None):
		self.rendertooltip.SetRenderDates(vnum, type, value3)

	def SetRenderWeapon(self, itemVnum):
		self.rendertooltip.SetRenderWeapon(itemVnum)

	def SetRenderArmor(self, itemVnum):
		self.rendertooltip.SetRenderArmor(itemVnum)

	def SetRenderAcce(self,itemVnum):
		self.rendertooltip.SetRenderAcce(itemVnum)

	def SetRenderHair(self,itemVnum,value3):
		self.rendertooltip.SetRenderHair(itemVnum,value3)

	def SetRenderArmor(self,itemVnum):
		self.rendertooltip.SetRenderArmor(itemVnum)

	def IsRenderTooltip(self):
		if self.rendertooltip.IsRenderTooltip():
			return True
		return False

	# show GIFT
	def ShowGift(self):
		self.wndTaskBar.ShowGift()

	if app.ENABLE_DROP_ITEM_WORLDARD:
		def BINARY_DROP_ITEM_OPEN(self):
			self.wndDropItem.Show()
	
	if app.ENABLE_CUBE_RENEWAL_WORLDARD:
		def BINARY_CUBE_RENEWAL_OPEN(self):
			self.wndCubeRenewal.Show()

	def OpenTableBonus(self):
		if self.wndTableBonus:
			if self.wndTableBonus.IsShow():
				self.wndTableBonus.Hide()
			else:
				self.wndTableBonus.Show()

	def CloseWbWindow(self):
		pass
		#self.wndWeb.Close()

	def OpenCardsInfoWindow(self):
		self.wndCardsInfo.Open()
		
	def OpenCardsWindow(self, safemode):
		self.wndCards.Open(safemode)
		
	def UpdateCardsInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points):
		self.wndCards.UpdateCardsInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points)
		
	def UpdateCardsFieldInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
		self.wndCards.UpdateCardsFieldInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)
		
	def CardsPutReward(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
		self.wndCards.CardsPutReward(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)
		
	def CardsShowIcon(self):
		self.wndCardsIcon.Show()
	if app.ENABLE_SASH_SYSTEM:
		def ActSash(self, iAct, bWindow):
			if iAct == 1:
				if bWindow == True:
					if not self.wndSashCombine.IsOpened():
						self.wndSashCombine.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				else:
					if not self.wndSashAbsorption.IsOpened():
						self.wndSashAbsorption.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				
				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 2:
				if bWindow == True:
					if self.wndSashCombine.IsOpened():
						self.wndSashCombine.Close()
				else:
					if self.wndSashAbsorption.IsOpened():
						self.wndSashAbsorption.Close()
				
				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 3 or iAct == 4:
				if bWindow == True:
					if self.wndSashCombine.IsOpened():
						self.wndSashCombine.Refresh(iAct)
				else:
					if self.wndSashAbsorption.IsOpened():
						self.wndSashAbsorption.Refresh(iAct)
				
				self.wndInventory.RefreshBagSlotWindow()

	def OpenCubeWindow(self):
		self.wndCube.Open()

		if False == self.wndInventory.IsShow():
			self.wndInventory.Show()

	def UpdateCubeInfo(self, gold, itemVnum, count):
		self.wndCube.UpdateInfo(gold, itemVnum, count)

	def CloseCubeWindow(self):
		self.wndCube.Close()

	def FailedCubeWork(self):
		self.wndCube.Refresh()

	def SucceedCubeWork(self, itemVnum, count):
		self.wndCube.Clear()

		print "Å¥ºê Á¦ÀÛ ¼º°ø! [%d:%d]" % (itemVnum, count)

		if 0: # °á°ú ¸Þ½ÃÁö Ãâ·ÂÀº »ý·« ÇÑ´Ù
			self.wndCubeResult.SetPosition(*self.wndCube.GetGlobalPosition())
			self.wndCubeResult.SetCubeResultItem(itemVnum, count)
			self.wndCubeResult.Open()
			self.wndCubeResult.SetTop()
	
	if app.ENABLE_IKASHOP_RENEWAL:
		def ToggleIkashopBusinessBoard(self):
			if app.ENABLE_AURA_SYSTEM:
				if self.wndAura and self.wndAura.IsShow():
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_OPEN_OTHER_WINDOW)
					return

			self.wndIkashopBusiness.Toggle()

		def ToggleIkashopSearchShopBoard(self, fromWhisper = False):
			if app.ENABLE_AURA_SYSTEM:
				if self.wndAura and self.wndAura.IsShow():
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_OPEN_OTHER_WINDOW)
					return

			if app.USE_OFFLINESHOP_SEARCH_V2:
				if not self.wndIkashopSearchShopV2:
					import uishopsearch_v2
					self.wndIkashopSearchShopV2 = uishopsearch_v2.ShopSearch(self)
					self.uiAffectShower.SetSearchShopBoard(self.wndIkashopSearchShopV2)

				self.wndIkashopSearchShopV2.Toggle(fromWhisper)
			else:
				self.wndIkashopSearchShop.Toggle()
		
		if app.USE_OFFLINESHOP_SEARCH_V2:
			def ShopSearchByOwnerName(self, ownerName):
				if not self.wndIkashopSearchShopV2:
					import uishopsearch_v2
					self.wndIkashopSearchShopV2 = uishopsearch_v2.ShopSearch(self)

				self.wndIkashopSearchShopV2.SearchEmeraldus(ownerName)

			def ShopSearchFastItem(self, itemVnum, itemName):
				if not self.wndIkashopSearchShopV2:
					import uishopsearch_v2
					self.wndIkashopSearchShopV2 = uishopsearch_v2.ShopSearch(self)
					self.uiAffectShower.SetSearchShopBoard(self.wndIkashopSearchShopV2)

				self.wndIkashopSearchShopV2.OnSearchFastItem(itemVnum, itemName)
				
			def OnSearchByOwnerName(self, itemVnum, itemName):
				if not self.wndIkashopSearchShopV2:
					import uishopsearch_v2
					self.wndIkashopSearchShopV2 = uishopsearch_v2.ShopSearch(self)
					self.uiAffectShower.SetSearchShopBoard(self.wndIkashopSearchShopV2)

				self.wndIkashopSearchShopV2.OnSearchByOwnerName(itemVnum, itemName)
				
	if app.ENABLE_AURA_SYSTEM:
		def AuraWindowOpen(self, type):
			if self.wndAura.IsShow():
				return

			# if self.inputDialog.IsShow():# or shop.GetNameDialogOpen():
				# chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.ACCE_NOT_OPEN_PRIVATE_SHOP)
				# return

			if self.dlgRefineNew and self.dlgRefineNew.IsShow():
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.ACCE_NOT_OPEN_REFINE)
				return

			self.wndAura.Open(type)

			if not self.wndInventory.IsShow():
				self.wndInventory.Show()

		def AuraWindowClose(self):
			if not self.wndAura.IsShow():
				return

			self.wndAura.CloseWindow()

	def __HideWindows(self):
		hideWindows = self.wndTaskBar,\
						self.wndCharacter,\
						self.wndMiniMap,\
						self.wndGuild,\
						self.wndMessenger,\
						self.wndChat,\
						self.wndParty,\
						self.wndGameButton,

		if app.USE_CRYSTAL_SYSTEM and self.wndShardCraftWnd:
			hideWindows += self.wndShardCraftWnd,

		if app.ENABLE_ZODIAC_MISSION:
			if self.wnd12ziTimer:
				hideWindows += self.wnd12ziTimer,
			if self.wnd12ziReward:
				hideWindows += self.wnd12ziReward,

		if self.wndExpandedTaskBar:
			hideWindows += self.wndExpandedTaskBar,
		
		if app.ENABLE_IKASHOP_RENEWAL:
			hideWindows += self.wndIkashopBusiness, self.wndIkashopSearchShop

			if app.USE_OFFLINESHOP_SEARCH_V2:
				if self.wndIkashopSearchShopV2:
					hideWindows += self.wndIkashopSearchShopV2,

			if app.EXTEND_IKASHOP_ULTIMATE:
				hideWindows += self.wndIkashopShopSkin,

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			hideWindows += self.wndDragonSoul,\
						self.wndDragonSoulRefine,

		if app.ENABLE_RANKING:
			if self.wndRanking:
				hideWindows += self.wndRanking,

		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM:
			if self.wndAutoWindow:
				hideWindows += self.wndAutoWindow,

		if self.wndExpandedMoneyTaskBar:
			hideWindows += self.wndExpandedMoneyTaskBar,

		if app.ENABLE_SWITCHBOT and self.wndSwitchbot:
			hideWindows += self.wndSwitchbot,

		if app.ENABLE_BATTLEPASS:
			hideWindows += self.wndBattlePass,

		if app.__AUTO_SKILL_READER__ and self.wndAutoSkillReader and self.wndAutoSkillReader.IsShow():
			hideWindows += self.wndAutoSkillReader,

		hideWindows = filter(lambda x:x.IsShow(), hideWindows)
		map(lambda x:x.Hide(), hideWindows)
		import sys

		if self.wndInventory:
			self.wndInventory.Hide(True)

		if self.wndAlmacenMenu.IsShowNew():
			self.wndAlmacenMenu.Open()

		self.HideAllQuestButton()
		self.HideAllWhisperButton()

		if self.wndChat.IsEditMode():
			self.wndChat.CloseChat()

		return hideWindows

	def __ShowWindows(self, wnds):
		import sys
		map(lambda x:x.Show(), wnds)
		global IsQBHide
		if not IsQBHide:
			self.ShowAllQuestButton()
		else:
			self.HideAllQuestButton()

		self.ShowAllWhisperButton()

		if self.wndChat:
			if self.wndChat.IsShow():
				self.wndChat.show_btnChatSizing()

	def BINARY_OpenAtlasWindow(self):
		if self.wndMiniMap:
			self.wndMiniMap.ShowAtlas()

	def BINARY_SetObserverMode(self, flag):
		self.wndGameButton.SetObserverMode(flag)

	# ACCESSORY_REFINE_ADD_METIN_STONE
	def BINARY_OpenSelectItemWindow(self):
		self.wndItemSelect.Open()
	# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE


	#####################################################################################
	### Equipment ###

	def OpenEquipmentDialog(self, vid):
		dlg = uiEquipmentDialog.EquipmentDialog()
		dlg.SetItemToolTip(self.tooltipItem)
		dlg.SetCloseEvent(ui.__mem_func__(self.CloseEquipmentDialog))
		dlg.Open(vid)

		self.equipmentDialogDict[vid] = dlg

	def SetEquipmentDialogItem(self, vid, slotIndex, vnum, count):
		if not vid in self.equipmentDialogDict:
			return
		self.equipmentDialogDict[vid].SetEquipmentDialogItem(slotIndex, vnum, count)

	def SetEquipmentDialogSocket(self, vid, slotIndex, socketIndex, value):
		if not vid in self.equipmentDialogDict:
			return
		self.equipmentDialogDict[vid].SetEquipmentDialogSocket(slotIndex, socketIndex, value)

	def SetEquipmentDialogAttr(self, vid, slotIndex, attrIndex, type, value):
		if not vid in self.equipmentDialogDict:
			return
		self.equipmentDialogDict[vid].SetEquipmentDialogAttr(slotIndex, attrIndex, type, value)

	def CloseEquipmentDialog(self, vid):
		if not vid in self.equipmentDialogDict:
			return
		del self.equipmentDialogDict[vid]

	#####################################################################################

	#####################################################################################
	### Quest ###
	if app.ENABLE_QUEST_RENEWAL:
		def RefreshQuest(self, quest_type, quest_index):
			self.wndCharacter.RefreshQuest(quest_type, quest_index)

			# Refresh quest button.
			self.__RefreshQuestButton()

		def DeleteQuest(self, quest_type, quest_index):
			self.wndCharacter.DeleteQuest(quest_type, quest_index)

		# Unused.
		def ShowQuestButton(self):
			pass

		def __OnClickQuestLetterButton(self, btn):
			self.OpenCharacterWindowWithState("QUEST")

			if btn.type == quest.QUEST_TYPE_EVENT:
				self.wndCharacter.OpenQuestCategory(quest.QUEST_TYPE_EVENT, True)

			if constInfo.ENABLE_QUEST_LETTER_CATEGORY:
				self.wndCharacter.OpenQuestCategory(btn.type, True)

			self.HideAllQuestButton()
			global IsQBHide
			IsQBHide = 1

		def __MakeQuestButton(self):
			if constInfo.ENABLE_QUEST_LETTER_CATEGORY:
				for quest_type in range(quest.QUEST_TYPE_MAX):
					if quest_type == quest.QUEST_TYPE_UNEXPOSED:
						continue

					btn = uiWhisper.WhisperButton()
					if quest_type == quest.QUEST_TYPE_MAIN:
						btn.SetUpVisual("d:/ymir work/ui/game/quest/scroll_close_red.tga")
						btn.SetOverVisual("d:/ymir work/ui/game/quest/scroll_open_red.tga")
						btn.SetDownVisual("d:/ymir work/ui/game/quest/scroll_close_red.tga")
					elif quest_type == quest.QUEST_TYPE_SUB:
						btn.SetUpVisual("d:/ymir work/ui/game/quest/scroll_close_orange.tga")
						btn.SetOverVisual("d:/ymir work/ui/game/quest/scroll_open_orange.tga")
						btn.SetDownVisual("d:/ymir work/ui/game/quest/scroll_close_orange.tga")
					elif quest_type == quest.QUEST_TYPE_LEVELUP:
						btn.SetUpVisual("d:/ymir work/ui/game/quest/scroll_close_purple.tga")
						btn.SetOverVisual("d:/ymir work/ui/game/quest/scroll_open_purple.tga")
						btn.SetDownVisual("d:/ymir work/ui/game/quest/scroll_close_purple.tga")
					elif quest_type == quest.QUEST_TYPE_EVENT:
						btn.SetUpVisual("d:/ymir work/ui/game/quest/scroll_close_sky_blue.tga")
						btn.SetOverVisual("d:/ymir work/ui/game/quest/scroll_open_sky_blue.tga")
						btn.SetDownVisual("d:/ymir work/ui/game/quest/scroll_close_sky_blue.tga")
					elif quest_type == quest.QUEST_TYPE_COLLECTION:
						btn.SetUpVisual("d:/ymir work/ui/game/quest/scroll_close_pink.tga")
						btn.SetOverVisual("d:/ymir work/ui/game/quest/scroll_open_pink.tga")
						btn.SetDownVisual("d:/ymir work/ui/game/quest/scroll_close_pink.tga")
					elif quest_type == quest.QUEST_TYPE_SYSTEM:
						btn.SetUpVisual("d:/ymir work/ui/game/quest/scroll_close_green.tga")
						btn.SetOverVisual("d:/ymir work/ui/game/quest/scroll_open_green.tga")
						btn.SetDownVisual("d:/ymir work/ui/game/quest/scroll_close_green.tga")
					elif quest_type == quest.QUEST_TYPE_SCROLL:
						btn.SetUpVisual("d:/ymir work/ui/game/quest/scroll_close_cyan.tga")
						btn.SetOverVisual("d:/ymir work/ui/game/quest/scroll_open_cyan.tga")
						btn.SetDownVisual("d:/ymir work/ui/game/quest/scroll_close_cyan.tga")
					elif quest_type == quest.QUEST_TYPE_DAILY:
						btn.SetUpVisual("d:/ymir work/ui/game/quest/scroll_close_blue.tga")
						btn.SetOverVisual("d:/ymir work/ui/game/quest/scroll_open_blue.tga")
						btn.SetDownVisual("d:/ymir work/ui/game/quest/scroll_close_blue.tga")
					else:
						btn.SetUpVisual("icon/item/scroll_close.tga")
						btn.SetOverVisual("icon/item/scroll_open.tga")
						btn.SetDownVisual("icon/item/scroll_close.tga")

					btn.SetEvent(ui.__mem_func__(self.__OnClickQuestLetterButton), btn)
					btn.Hide()
					btn.type = quest_type
					btn.count = 0

					btn.counter_img = ui.ImageBox()
					btn.counter_img.SetParent(btn)
					btn.counter_img.AddFlag("not_pick")
					btn.counter_img.SetPosition(0, 0)
					btn.counter_img.LoadImage("d:/ymir work/ui/game/quest/counter_background.tga")
					btn.counter_img.Show()

					btn.counter = ui.TextLine()
					btn.counter.SetParent(btn.counter_img)
					btn.counter.SetText("99")
					btn.counter.SetPosition(-1, -1)
					btn.counter.SetWindowHorizontalAlignCenter()
					btn.counter.SetHorizontalAlignCenter()
					btn.counter.SetPackedFontColor(0xffffffff)
					btn.counter.Show()

					self.questButtonList.append(btn)
			else:
				btn = uiWhisper.WhisperButton()
				btn.SetUpVisual(localeInfo.GetLetterCloseImageName())
				btn.SetOverVisual(localeInfo.GetLetterOpenImageName())
				btn.SetDownVisual(localeInfo.GetLetterOpenImageName())
				btn.SetEvent(ui.__mem_func__(self.__OnClickQuestLetterButton), btn)
				btn.Hide()
				btn.type = quest.QUEST_TYPE_MAX
				btn.count = 0
				self.questButtonList.append(btn)

				# Used for event quests in order to highlight it.
				btn = uiWhisper.WhisperButton()
				btn.SetUpVisual(localeInfo.GetBlueLetterImageName())
				btn.SetOverVisual(localeInfo.GetBlueLetterOpenImageName())
				btn.SetDownVisual(localeInfo.GetBlueLetterOpenImageName())
				btn.SetEvent(ui.__mem_func__(self.__OnClickQuestLetterButton), btn)
				btn.Hide()
				btn.type = quest.QUEST_TYPE_EVENT
				btn.count = 0
				self.questButtonList.append(btn)

			self.__ArrangeQuestButton()

		def __ArrangeQuestButton(self):
			screen_width = wndMgr.GetScreenWidth()
			screen_height = wndMgr.GetScreenHeight()

			x = 100 + 30 if self.wndParty and self.wndParty.IsShow() else 20
			if localeInfo.IsARABIC():
				x += 15

			y = 170 * screen_height / 600
			y_count = (screen_height - 330) / 63

			count = 0
			for btn in self.questButtonList:
				if btn.count != 0:
					if constInfo.ENABLE_QUEST_LETTER_CATEGORY:
						btn.SetToolTipText(uiCharacter.QUEST_LABEL_NAME_DICT[btn.type], -20, 35)
						btn.counter.SetText("%d" % btn.count)
						if localeInfo.IsARABIC():
							btn.ToolTipText.SetHorizontalAlignRight()
						else:
							btn.ToolTipText.SetHorizontalAlignLeft()
					else:
						btn.SetToolTipText(str(btn.count), 0, 35)
					btn.SetPosition(x + (int(count / y_count) * 100), y + (count % y_count * 63))
					count += 1

				global IsQBHide
				if IsQBHide:
					btn.Hide()
				else:
					if btn.count > 0:
						btn.Show()

		def __RefreshQuestButton(self):
			for btn in self.questButtonList:
				btn.count = quest.GetQuestButtonNoticeCount(btn.type)
			self.__ArrangeQuestButton()
	else:
		def BINARY_ClearQuest(self, index):
			btn = self.__FindQuestButton(index)
			if 0 != btn:
				self.__DestroyQuestButton(btn)

		def RecvQuest(self, index, name):
			# QUEST_LETTER_IMAGE
			self.BINARY_RecvQuest(index, name, "file", localeInfo.GetLetterImageName())
			# END_OF_QUEST_LETTER_IMAGE

		def BINARY_RecvQuest(self, index, name, iconType, iconName):
			btn = self.__FindQuestButton(index)
			if 0 != btn:
				self.__DestroyQuestButton(btn)

			btn = uiWhisper.WhisperButton()

			# QUEST_LETTER_IMAGE
			##!! 20061026.levites.퀘스트_이미지_교체
			import item

			if "item" == iconType:
				item.SelectItem(int(iconName))
				buttonImageFileName = item.GetIconImageFileName()
			elif "blue_quest" == iconType or name[0] == "*":
				buttonImageFileName = localeInfo.GetBlueLetterImageName()
			else:
				buttonImageFileName = iconName

			if localeInfo.IsEUROPE():
				if name[0] == "*":
					btn.SetUpVisual(localeInfo.GetBlueLetterImageName())
					btn.SetOverVisual(localeInfo.GetBlueLetterOpenImageName())
					btn.SetDownVisual(localeInfo.GetBlueLetterCloseImageName())
					name = name[1:]
				else:
					if "highlight" == iconType:
						btn.SetUpVisual("locale/ymir_ui/highlighted_quest.tga")
						btn.SetOverVisual("locale/ymir_ui/highlighted_quest_r.tga")
						btn.SetDownVisual("locale/ymir_ui/highlighted_quest_r.tga")
					else:
						btn.SetUpVisual(localeInfo.GetLetterCloseImageName())
						btn.SetOverVisual(localeInfo.GetLetterOpenImageName())
						btn.SetDownVisual(localeInfo.GetLetterOpenImageName())
			else:
				btn.SetUpVisual(buttonImageFileName)
				btn.SetOverVisual(buttonImageFileName)
				btn.SetDownVisual(buttonImageFileName)
				btn.Flash()
			# END_OF_QUEST_LETTER_IMAGE

			if localeInfo.IsARABIC():
				btn.SetToolTipText(name, -20, 35)
				btn.ToolTipText.SetHorizontalAlignRight()
			else:
				btn.SetToolTipText(name, -20, 35)
				btn.ToolTipText.SetHorizontalAlignLeft()

			btn.SetEvent(ui.__mem_func__(self.__StartQuest), btn)
			btn.Show()

			btn.index = index
			btn.name = name

			self.questButtonList.insert(0, btn)
			self.__ArrangeQuestButton()

			#chat.AppendChat(chat.CHAT_TYPE_NOTICE, localeInfo.QUEST_APPEND)

		def __ArrangeQuestButton(self):
			screenWidth = wndMgr.GetScreenWidth()
			screenHeight = wndMgr.GetScreenHeight()

			##!! 20061026.levites.퀘스트_위치_보정
			if self.wndParty.IsShow():
				xPos = 100 + 30
			else:
				xPos = 20

			if localeInfo.IsARABIC():
				xPos = xPos + 15

			yPos = 170 * screenHeight / 600
			yCount = (screenHeight - 330) / 63

			count = 0
			for btn in self.questButtonList:
				btn.SetPosition(xPos + (int(count / yCount) * 100), yPos + (count % yCount * 63))
				count += 1

				global IsQBHide
				if IsQBHide:
					btn.Hide()
				else:
					btn.Show()

		def __StartQuest(self, btn):
			event.QuestButtonClick(btn.index)
			self.__DestroyQuestButton(btn)

		def __FindQuestButton(self, index):
			for btn in self.questButtonList:
				if btn.index == index:
					return btn

			return 0

		def __DestroyQuestButton(self, btn):
			btn.SetEvent(0)
			self.questButtonList.remove(btn)
			self.__ArrangeQuestButton()

	def HideAllQuestButton(self):
		for btn in self.questButtonList:
			btn.Hide()

	def ShowAllQuestButton(self):
		if app.ENABLE_KINGDOMS_WAR:
			if self.wndKingdomsWar.IsShow():
				return

		if app.ENABLE_QUEST_RENEWAL:
			for btn in self.questButtonList:
				if btn.count > 0:
					btn.Show()
		else:
			for btn in self.questButtonList:
				btn.Show()
	#####################################################################################

	#####################################################################################
	### Whisper ###

	def __InitWhisper(self):
		chat.InitWhisper(self)

	## Ã¤ÆÃÃ¢ÀÇ "¸Þ½ÃÁö º¸³»±â"¸¦ ´­·¶À»¶§ ÀÌ¸§ ¾ø´Â ´ëÈ­Ã¢À» ¿©´Â ÇÔ¼ö
	## ÀÌ¸§ÀÌ ¾ø±â ¶§¹®¿¡ ±âÁ¸ÀÇ WhisperDialogDict ¿Í º°µµ·Î °ü¸®µÈ´Ù.
	def OpenWhisperDialogWithoutTarget(self):
		if not self.dlgWhisperWithoutTarget:
			dlgWhisper = uiWhisper.WhisperDialog(self.MinimizeWhisperDialog, self.CloseWhisperDialog)
			dlgWhisper.BindInterface(self)
			dlgWhisper.LoadDialog()
			dlgWhisper.OpenWithoutTarget(self.RegisterTemporaryWhisperDialog)
			dlgWhisper.SetPosition(self.windowOpenPosition*30,self.windowOpenPosition*30)
			dlgWhisper.Show()
			self.dlgWhisperWithoutTarget = dlgWhisper

			self.windowOpenPosition = (self.windowOpenPosition+1) % 5

		else:
			self.dlgWhisperWithoutTarget.SetTop()
			self.dlgWhisperWithoutTarget.OpenWithoutTarget(self.RegisterTemporaryWhisperDialog)

	## ÀÌ¸§ ¾ø´Â ´ëÈ­Ã¢¿¡¼­ ÀÌ¸§À» °áÁ¤ÇßÀ»¶§ WhisperDialogDict¿¡ Ã¢À» ³Ö¾îÁÖ´Â ÇÔ¼ö
	if app.ENABLE_MULTILANGUAGE_SYSTEM:
		def RegisterTemporaryWhisperDialog(self, name):
			if not self.dlgWhisperWithoutTarget:
				return

			btn = self.__FindWhisperButton(name)
			if 0 != btn:
				self.__DestroyWhisperButton(btn)

			elif self.whisperDialogDict.has_key(name.lower()):
				oldDialog = self.whisperDialogDict[name.lower()]
				oldDialog.Hide()
				oldDialog.Destroy()
				del self.whisperDialogDict[name.lower()]

			self.whisperDialogDict[name.lower()] = self.dlgWhisperWithoutTarget
			self.dlgWhisperWithoutTarget.OpenWithTarget(name)
			self.dlgWhisperWithoutTarget = None
			self.__CheckGameMaster(name)
	else:
		def RegisterTemporaryWhisperDialog(self, name):
			if not self.dlgWhisperWithoutTarget:
				return

			btn = self.__FindWhisperButton(name)
			if 0 != btn:
				self.__DestroyWhisperButton(btn)

			elif self.whisperDialogDict.has_key(name):
				oldDialog = self.whisperDialogDict[name]
				oldDialog.Hide()
				oldDialog.Destroy()
				del self.whisperDialogDict[name]

			self.whisperDialogDict[name] = self.dlgWhisperWithoutTarget
			self.dlgWhisperWithoutTarget.OpenWithTarget(name)
			self.dlgWhisperWithoutTarget = None
			self.__CheckGameMaster(name)

	## Ä³¸¯ÅÍ ¸Þ´ºÀÇ 1:1 ´ëÈ­ ÇÏ±â¸¦ ´­·¶À»¶§ ÀÌ¸§À» °¡Áö°í ¹Ù·Î Ã¢À» ¿©´Â ÇÔ¼ö
	if app.ENABLE_MULTILANGUAGE_SYSTEM:
		def OpenWhisperDialog(self, name, language = 0, empire = 0):
			if not self.whisperDialogDict.has_key(name.lower()):
				dlg = self.__MakeWhisperDialog(name)
				dlg.OpenWithTarget(name)
				language = int(language)
				empire = int(empire)
				if language != 0 and empire != 0:
					dlg.SetFlag(language, empire)
				dlg.chatLine.SetFocus()
				dlg.Show()

				self.__CheckGameMaster(name)
				btn = self.__FindWhisperButton(name)
				if 0 != btn:
					dlg.SetFlag(btn.languageID, btn.empireID)
					self.__DestroyWhisperButton(btn)
	else:
		def OpenWhisperDialog(self, name):
				if not self.whisperDialogDict.has_key(name):
					dlg = self.__MakeWhisperDialog(name)
					dlg.OpenWithTarget(name)
					dlg.chatLine.SetFocus()
					dlg.Show()

					self.__CheckGameMaster(name)
					btn = self.__FindWhisperButton(name)
					if 0 != btn:
						self.__DestroyWhisperButton(btn)

	## ´Ù¸¥ Ä³¸¯ÅÍ·ÎºÎÅÍ ¸Þ¼¼Áö¸¦ ¹Þ¾ÒÀ»¶§ ÀÏ´Ü ¹öÆ°¸¸ ¶ç¿ö µÎ´Â ÇÔ¼ö
	if app.ENABLE_MULTILANGUAGE_SYSTEM:
		def RecvWhisper(self, name, language = 0, empire = 0):
			if not self.whisperDialogDict.has_key(name.lower()):
				btn = self.__FindWhisperButton(name)

				if 0 == btn:
					btn = self.__MakeWhisperButton(name, int(language), int(empire))
					btn.Flash()
					# if app.ENABLE_WHISPER_FLASHING:
						# app.FlashApplication()
					chat.AppendChat(chat.CHAT_TYPE_NOTICE, localeInfo.RECEIVE_MESSAGE % (name))
				else:
					language = int(language)
					empire = int(empire)
					if language != 0 and empire != 0:
						btn.languageID = language
						btn.empireID = empire
					btn.Flash()
					# if app.ENABLE_WHISPER_FLASHING:
						# app.FlashApplication()
			elif self.IsGameMasterName(name.lower()):
				dlg = self.whisperDialogDict[name.lower()]
				dlg.SetGameMasterLook()

				if language != "" and empire != "":
					dlg.SetFlag(language, empire)
	else:
		def RecvWhisper(self, name):
			if not self.whisperDialogDict.has_key(name):
				btn = self.__FindWhisperButton(name)
				if 0 == btn:
					btn = self.__MakeWhisperButton(name)
					btn.Flash()
					# if app.ENABLE_WHISPER_FLASHING:
						# app.FlashApplication()
					chat.AppendChat(chat.CHAT_TYPE_NOTICE, localeInfo.RECEIVE_MESSAGE % (name))
				else:
					btn.Flash()
					# if app.ENABLE_WHISPER_FLASHING:
						# app.FlashApplication()
			elif self.IsGameMasterName(name):
				dlg = self.whisperDialogDict[name]
				dlg.SetGameMasterLook()

	def MakeWhisperButton(self, name):
		self.__MakeWhisperButton(name)

	if app.ENABLE_MULTILANGUAGE_SYSTEM:
		def SetInterfaceFlag(self, name, language, empire):
			if self.whisperDialogDict.has_key(name.lower()):
				self.whisperDialogDict[name.lower()].SetFlag(language, empire)
			else:
				btn = self.__FindWhisperButton(name)
				if btn != 0:
					btn.languageID = language
					btn.empireID = empire

	## ¹öÆ°À» ´­·¶À»¶§ Ã¢À» ¿©´Â ÇÔ¼ö
	if app.ENABLE_MULTILANGUAGE_SYSTEM:
		def ShowWhisperDialog(self, btn):
			if app.ENABLE_WHISPER_CLOSE_SHORTCUT:
				if app.IsPressed(app.DIK_LCONTROL):
					chat.ClearWhisper(btn.name)
					self.__DestroyWhisperButton(btn)
					self.HideWhisperTooltip()
					return		
			try:
				self.__MakeWhisperDialog(btn.name)
				dlgWhisper = self.whisperDialogDict[btn.name.lower()]
				dlgWhisper.OpenWithTarget(btn.name)
				if btn.languageID != 0 and btn.empireID != 0:
					dlgWhisper.SetFlag(btn.languageID, btn.empireID)
				dlgWhisper.Show()
				self.__CheckGameMaster(btn.name)
			except:
				import dbg
				dbg.TraceError("interface.ShowWhisperDialog - Failed to find key")
			self.__DestroyWhisperButton(btn)
			if app.ENABLE_WHISPER_CLOSE_SHORTCUT:
				self.HideWhisperTooltip()
	else:
		def ShowWhisperDialog(self, btn):
			if app.ENABLE_WHISPER_CLOSE_SHORTCUT:
				if app.IsPressed(app.DIK_LCONTROL):
					chat.ClearWhisper(btn.name)
					self.__DestroyWhisperButton(btn)
					self.HideWhisperTooltip()
					return		
			try:
				self.__MakeWhisperDialog(btn.name)
				dlgWhisper = self.whisperDialogDict[btn.name]
				dlgWhisper.OpenWithTarget(btn.name)
				dlgWhisper.Show()
				self.__CheckGameMaster(btn.name)
			except:
				import dbg
				dbg.TraceError("interface.ShowWhisperDialog - Failed to find key")
			self.__DestroyWhisperButton(btn)
			if app.ENABLE_WHISPER_CLOSE_SHORTCUT:
				self.HideWhisperTooltip()
		## ¹öÆ° ÃÊ±âÈ­
		#self.__DestroyWhisperButton(btn)

	## WhisperDialog Ã¢¿¡¼­ ÃÖ¼ÒÈ­ ¸í·ÉÀ» ¼öÇàÇßÀ»¶§ È£ÃâµÇ´Â ÇÔ¼ö
	## Ã¢À» ÃÖ¼ÒÈ­ ÇÕ´Ï´Ù.
	if app.ENABLE_MULTILANGUAGE_SYSTEM:
		def MinimizeWhisperDialog(self, name, languageID, empireID):
			if 0 != name:
				self.__MakeWhisperButton(name, languageID, empireID)
			self.CloseWhisperDialog(name)
	else:
		def MinimizeWhisperDialog(self, name):
			if 0 != name:
				self.__MakeWhisperButton(name)
			self.CloseWhisperDialog(name)

	## WhisperDialog Ã¢¿¡¼­ ´Ý±â ¸í·ÉÀ» ¼öÇàÇßÀ»¶§ È£ÃâµÇ´Â ÇÔ¼ö
	## Ã¢À» Áö¿ó´Ï´Ù.
	def CloseWhisperDialog(self, name):

		if 0 == name:

			if self.dlgWhisperWithoutTarget:
				self.dlgWhisperWithoutTarget.Hide()
				self.dlgWhisperWithoutTarget.Destroy()
				self.dlgWhisperWithoutTarget = None

			return

		try:
			if app.ENABLE_MULTILANGUAGE_SYSTEM:
				dlgWhisper = self.whisperDialogDict[name.lower()]
				dlgWhisper.Hide()
				dlgWhisper.Destroy()
				del self.whisperDialogDict[name.lower()]
			else:
				dlgWhisper = self.whisperDialogDict[name]
				dlgWhisper.Hide()
				dlgWhisper.Destroy()
				del self.whisperDialogDict[name]
		except:
			import dbg
			# dbg.TraceError("interface.CloseWhisperDialog - Failed to find key")
			# dbg.TraceError("name: "+str(self.whisperDialogDict[name]))

	## ¹öÆ°ÀÇ °³¼ö°¡ ¹Ù²î¾úÀ»¶§ ¹öÆ°À» ÀçÁ¤·Ä ÇÏ´Â ÇÔ¼ö
	def __ArrangeWhisperButton(self):

		screenWidth = wndMgr.GetScreenWidth()
		screenHeight = wndMgr.GetScreenHeight()

		xPos = screenWidth - 70
		yPos = 170 * screenHeight / 600
		yCount = (screenHeight - 330) / 63
		#yCount = (screenHeight - 285) / 63

		count = 0
		for button in self.whisperButtonList:

			button.SetPosition(xPos + (int(count/yCount) * -50), yPos + (count%yCount * 63))
			count += 1

	## ÀÌ¸§À¸·Î Whisper ¹öÆ°À» Ã£¾Æ ¸®ÅÏÇØ ÁÖ´Â ÇÔ¼ö
	## ¹öÆ°Àº µñ¼Å³Ê¸®·Î ÇÏÁö ¾Ê´Â °ÍÀº Á¤·Ä µÇ¾î ¹ö·Á ¼ø¼­°¡ À¯Áö µÇÁö ¾ÊÀ¸¸ç
	## ÀÌ·Î ÀÎÇØ ToolTipµéÀÌ ´Ù¸¥ ¹öÆ°µé¿¡ ÀÇÇØ °¡·ÁÁö±â ¶§¹®ÀÌ´Ù.
	if app.ENABLE_MULTILANGUAGE_SYSTEM:
		def __FindWhisperButton(self, name):
			for button in self.whisperButtonList:
				if button.name.lower() == name.lower():
					return button
			return 0
	else:
		def __FindWhisperButton(self, name):
			for button in self.whisperButtonList:
				if button.name == name:
					return button
			return 0

	## Ã¢À» ¸¸µì´Ï´Ù.
	def __MakeWhisperDialog(self, name):
		dlgWhisper = uiWhisper.WhisperDialog(self.MinimizeWhisperDialog, self.CloseWhisperDialog)
		dlgWhisper.BindInterface(self)
		dlgWhisper.LoadDialog()
		dlgWhisper.SetPosition(self.windowOpenPosition*30,self.windowOpenPosition*30)
		
		if app.ENABLE_MULTILANGUAGE_SYSTEM:
			self.whisperDialogDict[name.lower()] = dlgWhisper
		else:
			self.whisperDialogDict[name] = dlgWhisper
		self.windowOpenPosition = (self.windowOpenPosition+1) % 5
		return dlgWhisper

	if app.ENABLE_WHISPER_CLOSE_SHORTCUT:
		def ShowWhisperToolTip(self):
			if self.whisper_tooltip:
				self.whisper_tooltip.SetTop()
				self.whisper_tooltip.Show()

		def HideWhisperTooltip(self):
			if self.whisper_tooltip:
				self.whisper_tooltip.Hide()
	## ¹öÆ°À» ¸¸µì´Ï´Ù.
	if app.ENABLE_MULTILANGUAGE_SYSTEM:
		def __MakeWhisperButton(self, name, languageID = 0, empireID = 0):
			whisperButton = uiWhisper.WhisperButton()
			if name == "[README]":
				whisperButton.SetUpVisual("d:/ymir work/ui/game/whisper/wikipedia.tga")
				whisperButton.SetOverVisual("d:/ymir work/ui/game/whisper/wikipedia_over.tga")
				whisperButton.SetDownVisual("d:/ymir work/ui/game/whisper/wikipedia.tga")
				whisperButton.SetToolTipTextWithColor(name, 0xffe7a83b)
			else:
				whisperButton.SetUpVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
				whisperButton.SetOverVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
				whisperButton.SetDownVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
			if self.IsGameMasterName(name):
				whisperButton.SetToolTipTextWithColor(name, 0xffffa200)
			else:
				whisperButton.SetToolTipText(name)
			whisperButton.ToolTipText.SetHorizontalAlignCenter()
			whisperButton.SetEvent(ui.__mem_func__(self.ShowWhisperDialog), whisperButton)
			if app.ENABLE_WHISPER_CLOSE_SHORTCUT:
				whisperButton.OnMouseOverIn = ui.__mem_func__(self.ShowWhisperToolTip)
				whisperButton.OnMouseOverOut = ui.__mem_func__(self.HideWhisperTooltip)
			whisperButton.Show()
			whisperButton.name = name
			whisperButton.languageID = int(languageID)
			whisperButton.empireID = int(empireID)
			self.whisperButtonList.insert(0, whisperButton)
			self.__ArrangeWhisperButton()
			return whisperButton
	else:
		def __MakeWhisperButton(self, name):
			whisperButton = uiWhisper.WhisperButton()
			whisperButton.SetUpVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
			whisperButton.SetOverVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
			whisperButton.SetDownVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
			if self.IsGameMasterName(name):
				whisperButton.SetToolTipTextWithColor(name, 0xffffa200)
			else:
				whisperButton.SetToolTipText(name)
			whisperButton.ToolTipText.SetHorizontalAlignCenter()
			whisperButton.SetEvent(ui.__mem_func__(self.ShowWhisperDialog), whisperButton)
			whisperButton.Show()
			whisperButton.name = name
			self.whisperButtonList.insert(0, whisperButton)
			self.__ArrangeWhisperButton()
			return whisperButton

	def __DestroyWhisperButton(self, button):
		button.SetEvent(0)
		self.whisperButtonList.remove(button)
		self.__ArrangeWhisperButton()
		if app.ENABLE_WHISPER_CLOSE_SHORTCUT:
			self.HideWhisperTooltip()

	def HideAllWhisperButton(self):
		for btn in self.whisperButtonList:
			btn.Hide()

	def ShowAllWhisperButton(self):
		for btn in self.whisperButtonList:
			btn.Show()

	if app.ENABLE_MULTILANGUAGE_SYSTEM:
		def __CheckGameMaster(self, name):
			if not self.listGMName.has_key(name.lower()):
				return
			if self.whisperDialogDict.has_key(name.lower()):
				dlg = self.whisperDialogDict[name.lower()]
				dlg.SetGameMasterLook()
	else:
		def __CheckGameMaster(self, name):
			if not self.listGMName.has_key(name):
				return
			if self.whisperDialogDict.has_key(name):
				dlg = self.whisperDialogDict[name]
				dlg.SetGameMasterLook()

	def RegisterGameMasterName(self, name):
		if self.listGMName.has_key(name):
			return
		self.listGMName[name] = "GM"

	def IsGameMasterName(self, name):
		if self.listGMName.has_key(name):
			return True
		else:
			return False

	#####################################################################################

	#####################################################################################
	### Guild Building ###

	def BUILD_OpenWindow(self):
		self.wndGuildBuilding = uiGuild.BuildGuildBuildingWindow()
		self.wndGuildBuilding.Open()
		self.wndGuildBuilding.wnds = self.__HideWindows()
		self.wndGuildBuilding.SetCloseEvent(ui.__mem_func__(self.BUILD_CloseWindow))

	def BUILD_CloseWindow(self):
		self.__ShowWindows(self.wndGuildBuilding.wnds)
		self.wndGuildBuilding = None

	def BUILD_OnUpdate(self):
		if not self.wndGuildBuilding:
			return

		if self.wndGuildBuilding.IsPositioningMode():
			import background
			x, y, z = background.GetPickingPoint()
			self.wndGuildBuilding.SetBuildingPosition(x, y, z)

	def BUILD_OnMouseLeftButtonDown(self):
		if not self.wndGuildBuilding:
			return

		# GUILD_BUILDING
		if self.wndGuildBuilding.IsPositioningMode():
			self.wndGuildBuilding.SettleCurrentPosition()
			return True
		elif self.wndGuildBuilding.IsPreviewMode():
			pass
		else:
			return True
		# END_OF_GUILD_BUILDING
		return False

	def BUILD_OnMouseLeftButtonUp(self):
		if not self.wndGuildBuilding:
			return

		if not self.wndGuildBuilding.IsPreviewMode():
			return True

		return False

	def BULID_EnterGuildArea(self, areaID):
		# GUILD_BUILDING
		mainCharacterName = player.GetMainCharacterName()
		masterName = guild.GetGuildMasterName()

		if mainCharacterName != masterName:
			return

		if areaID != player.GetGuildID():
			return
		# END_OF_GUILD_BUILDING

		self.wndGameButton.ShowBuildButton()

	def BULID_ExitGuildArea(self, areaID):
		self.wndGameButton.HideBuildButton()

	#####################################################################################

	def IsEditLineFocus(self):
		if self.ChatWindow.chatLine.IsFocus():
			return 1

		if self.ChatWindow.chatToLine.IsFocus():
			return 1

		return 0

	if app.AUTO_SHOUT:
		def OpenShoutWindow(self):
			if self.wndShout.IsShow():
				self.wndShout.Hide()
			else:
				self.wndShout.Open()
	def GetInventoryPageIndex(self):
		if self.wndInventory:
			return self.wndInventory.GetInventoryPageIndex()
		else:
			return -1

	def AttachItemFromSafebox(self, slotIndex, item_type, itemIndex):
		if self.wndInventory and self.wndInventory.IsShow():
			self.wndInventory.AttachItemFromSafebox(slotIndex, item_type, itemIndex)
		return True

	def AttachItemSpecialFromSafebox(self, slotIndex, item_type, itemIndex):
		if self.wndSpecialStorage and self.wndSpecialStorage.IsShow():
			self.wndSpecialStorage.AttachItemFromSafebox(slotIndex, item_type, itemIndex)

		return True

	def AttachSpecialToInv(self, slotWindow, slotIndex):
		if self.wndInventory and self.wndInventory.IsShow():
			self.wndInventory.AttachSpecialToInv(slotWindow, slotIndex)
		return True

	def AttachInvenItemToOtherWindowSlot(self, slotWindow, slotIndex):
		if self.wndInventoryNew and self.wndInventoryNew.IsShow():
			if self.wndInventoryNew.categoryPageIndex == 7:
				return self.wndInventoryNew.AttachItemFromInventoryToSafebox(slotWindow, slotIndex)
			elif self.wndInventoryNew.categoryPageIndex <= 5:
				return self.wndInventoryNew.AttachItemFromInventoryToSpecial(slotWindow, slotIndex)
		#if self.wndSafebox and self.wndSafebox.IsShow():
		#	return self.wndSafebox.AttachItemFromInventory(slotWindow, slotIndex)

		return False


	def SafeboxIsShow(self):
		if self.wndInventoryNew and self.wndInventory:
			if self.wndInventoryNew.IsShow() and self.wndInventory.IsShow():
				return True

		if self.wndSafebox and self.wndInventory:
			if self.wndSafebox.IsShow() and self.wndInventory.IsShow():
				return True

		return False

	if app.WJ_ENABLE_TRADABLE_ICON:
		def SetOnTopWindow(self, onTopWnd):
			self.onTopWindow = onTopWnd

		def GetOnTopWindow(self):
			return self.onTopWindow

		def RefreshMarkInventoryBag(self):
			self.wndInventory.RefreshMarkSlots()
			self.wndInventoryNew.RefreshMarkSlots()

	if app.ENABLE_KINGDOMS_WAR:
		def ActKingdomsWar(self, act, score1, score2, score3, limitKills, deads, limiDeads, timeRemained):
			if self.wndKingdomsWar:
				if act == 1:
					self.wndKingdomsWar.ShowWindow(score1, score2, score3, limitKills, deads, limiDeads, timeRemained)
					global IsQBHide
					IsQBHide = 1
					for btn in self.questButtonList:
						btn.Hide()
				elif act == 2:
					self.wndKingdomsWar.RefreshScore(score1, score2, score3, limitKills)
				elif act == 3:
					self.wndKingdomsWar.RefreshDeads(deads, limiDeads)

	def EmptyFunction(self):
		pass
		
	# if app.ENABLE_DSS_ACTIVE_EFFECT_BUTTON:
		# def UseDSSButtonEffect(self, enable):
			# if self.wndInventory:
				# self.wndInventory.UseDSSButtonEffect(enable)

	if app.ENABLE_NEW_PET_SYSTEM:
		def MakeChangeNameWindow(self):
			self.change_window = uiChangeName.NameInputWindow()

		def OpenChangeNameWindow(self, srcitemPos, destitemPos, itemVnum, type = False):
			if self.change_window == None:
				self.MakeChangeNameWindow()
			self.change_window.LoadItemData(srcitemPos,destitemPos, itemVnum, type)
			self.change_window.Show()
			self.change_window.SetTop()
		def MakePetWindow(self):
			self.wndPet = uiPetSystemNew.PetSystemMain()
			self.wndPet.Hide()

		def ClearPetData(self):
			if self.wndPet == None:
				return
			self.wndPet.ClearData()

		def SetPetSlotIndex(self, index):
			if self.wndPet == None:
				self.MakePetWindow()
			self.wndPet.SetSlotIndex(index)

			if self.wndPet.IsShow():
				self.wndPet.Open()

		def PetUpdate(self, index):
			if self.wndPet == None:
				return
			if index == 0:
				self.wndPet.UpdateTime()
			elif index == 1:
				self.wndPet.UpdateLevel()
				self.wndPet.CheckEvolveFlash()
				self.wndPet.CheckFeedWindow()
			elif index == 2:
				self.wndPet.UpdateExp()
			elif index == 3:
				self.wndPet.UpdateAge()
				self.wndPet.CheckEvolveFlash()
				self.wndPet.CheckFeedWindow()
				self.wndPet.UpdateSkill()
			elif index >= 5 and index <= 7:
				self.wndPet.UpdateBonus()
			elif ((index >= 100 and index <= 114) or (index >= 150 and index <= 164)):
				self.wndPet.UpdateSkill()

		def OpenPetWindow(self):
			if self.wndPet == None:
				self.MakePetWindow()
			if self.wndPet.IsShow():
				self.wndPet.Hide()
			else:
				self.wndPet.Open()

	if app.ENABLE_BIYOLOG:
		def MakeBioWindow(self):
			if self.wndBio == None:
				self.wndBio = uiBiyolog.BiologWindow()
				self.wndBio.LoadEmpty()
				return True
			return False
		def OpenBiologWindow(self):
			self.MakeBioWindow()
			if self.wndBio.IsShow():
				self.wndBio.Close()
			else:
				self.wndBio.Open()
		def SetBioData(self, level, count, time):
			self.MakeBioWindow()
			self.wndBio.LoadData(int(level), int(count), int(time))
		def SetBioStone(self, level):
			self.MakeBioWindow()
			self.wndBio.LoadStone(int(level))
		def SetBioGift(self, level):
			self.MakeBioWindow()
			self.wndBio.LoadGift(int(level))
		def SetBioEmpty(self):
			if self.MakeBioWindow() == False:
				self.wndBio.LoadEmpty()

	if app.ENABLE_CHANNEL_CHANGE:
		def OpenChangeChannel(self):
			if self.wndChangeChannel.IsShow():
				self.wndChangeChannel.Hide()
			else:
				self.wndChangeChannel.Show()

	if app.dracaryS_DUNGEON_LIB:
		def MakeDungeonTimerWindow(self):
			self.wndDungeonTimer = uiDungeonTimer.Cooldown()
			self.wndDungeonTimer.Hide()

	if app.ENABLE_DUNGEON_INFO:
		def MakeDungeonInfo(self):
			if self.wndDungeonInfo == None:
				self.wndDungeonInfo = uiDungeonInfo.DungeonInfo()
		def OpenDungeonInfo(self):
			self.MakeDungeonInfo()
			if self.wndDungeonInfo.IsShow():
				self.wndDungeonInfo.Close()
			else:
				self.wndDungeonInfo.Open()

	if app.ENABLE_ITEMSHOP:
		def MakeItemShopWindow(self):
			if not self.wndItemShop:
				self.wndItemShop = uiItemShopNew.ItemShopWindow()

		def OpenItemShopWindow(self):
			if not self.wndItemShop:
				self.wndItemShop = uiItemShopNew.ItemShopWindow()

			if self.wndItemShop.IsShow():
				self.wndItemShop.Close()
			else:
				self.wndItemShop.Open()

		def OpenItemShopMainWindow(self):
			self.MakeItemShopWindow()
			self.wndItemShop.Open()
			self.wndItemShop.LoadFirstOpening()
		def ItemShopHideLoading(self):
			self.MakeItemShopWindow()
			self.wndItemShop.Open()
			self.wndItemShop.CloseLoading()
		def ItemShopPurchasesWindow(self):
			self.MakeItemShopWindow()
			self.wndItemShop.Open()
			self.wndItemShop.OpenPurchasesWindow()
		def ItemShopUpdateItem(self, itemID, itemMaxSellingCount):
			self.MakeItemShopWindow()
			self.wndItemShop.UpdateItem(itemID, itemMaxSellingCount)

		#USE_ITEMSHOP_RENEWED: @lldJCoins
		def ItemShopSetDragonCoin(self, lldCoins, lldJCoins = 0):
			if not self.wndItemShop:
				self.wndItemShop = uiItemShopNew.ItemShopWindow()

			self.wndItemShop.SetDragonCoin(lldCoins)

			if app.USE_ITEMSHOP_RENEWED:
				self.wndItemShop.SetDragonJeton(lldJCoins)

		def SetWheelItemData(self, cmd):
			self.MakeItemShopWindow()
			self.wndItemShop.SetWheelItemData(str(cmd))
		def OnSetWhell(self, giftIndex):
			self.MakeItemShopWindow()
			self.wndItemShop.OnSetWhell(int(giftIndex))
		def GetWheelGiftData(self, itemVnum, itemCount):
			self.MakeItemShopWindow()
			self.wndItemShop.GetWheelGiftData(int(itemVnum), int(itemCount))
	
	if app.ENABLE_EVENT_MANAGER:
		def MakeEventIcon(self):
			if self.wndEventIcon == None:
				self.wndEventIcon = uiEventCalendarNew.MovableImage()
				self.wndEventIcon.Show()
		def MakeEventCalendar(self):
			if self.wndEventManager == None:
				self.wndEventManager = uiEventCalendarNew.EventCalendarWindow()
		def OpenEventCalendar(self):
			self.MakeEventCalendar()
			if self.wndEventManager.IsShow():
				self.wndEventManager.Close()
			else:
				self.wndEventManager.Open()
		def RefreshEventStatus(self, eventID, eventStatus, eventendTime, eventEndTimeText):
			if eventendTime != 0:
				eventendTime += app.GetGlobalTimeStamp()
			uiEventCalendarNew.SetEventStatus(eventID, eventStatus, eventendTime, eventEndTimeText)
			self.RefreshEventManager()
		def ClearEventManager(self):
			uiEventCalendarNew.server_event_data={}
		def RefreshEventManager(self):
			if self.wndEventManager:
				self.wndEventManager.Refresh()
			if self.wndEventIcon:
				self.wndEventIcon.Refresh()
		def AppendEvent(self, dayIndex, eventID, eventIndex, startTime, endTime, empireFlag, channelFlag, value0, value1, value2, value3, startRealTime, endRealTime, isAlreadyStart):
			self.MakeEventCalendar()
			self.MakeEventIcon()
			#import dbg
			#dbg.TraceError("startTime: %d endTime: %d"%(startRealTime, endRealTime))
			if startRealTime != 0:
				startRealTime += app.GetGlobalTimeStamp()
			if endRealTime != 0:
				endRealTime += app.GetGlobalTimeStamp()
			uiEventCalendarNew.SetServerData(dayIndex, eventID, eventIndex, startTime, endTime, empireFlag, channelFlag, value0, value1, value2, value3, startRealTime, endRealTime, isAlreadyStart)


	if app.ENABLE_AUTO_SELECT_SKILL:
		def MakeAutoSkillWindow(self):
			if self.wndAutoSkill == None:
				self.wndAutoSkill = uiAutoSkill.AutoSkillSelect()
		def OpenAutoSkillWindow(self):
			self.MakeAutoSkillWindow()
			self.wndAutoSkill.Show()
	
	if app.ENABLE_SHOW_CHEST_DROP:
		def OpenChestDropWindow(self, itemVnum, itemWindow, itemCell):
			if self.wndChestDropInfo:
				self.wndChestDropInfo.Open(itemVnum, itemWindow, itemCell)

	if app.ENABLE_WIKI:
		def OpenWikiWindow(self):
			if self.wndWiki.IsShow():
				self.wndWiki.Close()
			else:
				self.wndWiki.Open()
		def __MakeWiki(self):
			if self.wndWiki == None:
				self.wndWiki = uiWiki.EncyclopediaofGame()
				self.wndInventory.BindWikiWindow(self.wndWiki)
				self.wndInventoryNew.BindWikiWindow(self.wndWiki)

	if app.ENABLE_RENDER_TARGET:
		def MakeRenderTargetWindow(self):
			if self.wndRenderTarget == None:
				self.wndRenderTarget = uiRenderTarget.RenderTargetWindow()
		def OpenRenderTargetWindow(self, renderType = 0, renderVnum = 11299):
			self.MakeRenderTargetWindow()
			self.wndRenderTarget.Open(renderType, renderVnum)

	if app.ENABLE_DISCORD_STUFF:
		def OpenWhisperWithMessage(self, name):
			if not self.whisperDialogDict.has_key(name):
				btn = self.__FindWhisperButton(name)
				if 0 != btn:
					self.ShowWhisperDialog(btn)
		def CheckWhisperIsOpen(self, name):
			if self.whisperDialogDict:
				if self.whisperDialogDict.has_key(name):
					return self.whisperDialogDict[name].IsShow()
			return False

	if app.ENABLE_ZODIAC_MISSION:
		def __Make12ziRewardWindow(self):
			self.wnd12ziReward = ui12zi.Reward12ziWindow()
			self.wnd12ziReward.SetItemToolTip(self.tooltipItem)
			self.wnd12ziReward.Hide()
		def __Make12ziTimerWindow(self):
			self.wnd12ziTimer = ui12zi.FloorLimitTimeWindow()
			self.wnd12ziTimer.Hide()

	if app.ENABLE_TRACK_WINDOW:
		def TrackWindowCheckPacket(self):
			self.wndTrackWindow.CheckPacket()
		def MakeTrackWindow(self):
			if self.wndTrackWindow == None:
				self.wndTrackWindow = uiTrack.TrackWindow()
		def OpenTrackWindow(self):
			self.MakeTrackWindow()
			if self.wndTrackWindow.IsShow():
				self.wndTrackWindow.Close()
			else:
				self.wndTrackWindow.Open()
		def TrackDungeonInfo(self, cmdData):
			self.MakeTrackWindow()
			self.wndTrackWindow.TrackDungeonInfo(cmdData)
		def TrackBossInfo(self, bossID, bossLeftTime, bossChannel):
			self.MakeTrackWindow()
			self.wndTrackWindow.TrackBossInfo(int(bossID), int(bossLeftTime), int(bossChannel))

	if app.ENABLE_FISH_GAME:
		def MakeFishGameWindow(self):
			if self.wndFishGame == None:
				self.wndFishGame = uiFishGame.FishGameWindow()
		def OpenFishGameWindow(self, gameKey):
			self.MakeFishGameWindow()
			self.wndFishGame.Open(int(gameKey))
		def SetFishGameGoal(self, goalCount):
			self.MakeFishGameWindow()
			self.wndFishGame.SetFishScore(int(goalCount))
		def CloseFishGame(self):
			self.MakeFishGameWindow()
			self.wndFishGame.Hide()

	if app.ENABLE_EXCHANGE_LOG:
		def MakeExchangeLogWindow(self):
			if self.wndExchangeLog == None:
				self.wndExchangeLog = uiExchangeLog.ExchangeLog()
		def OpenExchangeLog(self):
			self.MakeExchangeLogWindow()
			if self.wndExchangeLog.IsShow():
				self.wndExchangeLog.Close()
			else:
				self.wndExchangeLog.Open()
		def ExchangeLogClear(self, playerCode):
			if self.wndExchangeLog:
				self.wndExchangeLog.Clear(playerCode)
		def ExchangeLogRefresh(self, isLogItemRefresh):
			if self.wndExchangeLog:
				if isLogItemRefresh:
					self.wndExchangeLog.RefreshItems(isLogItemRefresh)
				else:
					self.wndExchangeLog.Refresh()
		def ExchangeLogAppend(self, logID, ownerName, ownerGold, ownerIP, targetName, targetGold, targetIP, date):
			if self.wndExchangeLog:
				self.wndExchangeLog.ExchangeLogAppend(logID, ownerName, ownerGold, ownerIP, targetName, targetGold, targetIP, date)
		def ExchangeLogItemAppend(self, logID, itemPos, itemVnum, itemCount, metinSlot, attrType, attrValue, isOwnerItem):
			if self.wndExchangeLog:
				self.wndExchangeLog.ExchangeLogItemAppend(logID, itemPos, itemVnum, itemCount, metinSlot, attrType, attrValue, isOwnerItem)

	if app.ENABLE_GEM_SYSTEM:
		def MakeGemShopWindow(self):
			if self.wndGemShop == None:
				self.wndGemShop = uiGem.GemShop()
		def OpenGemShop(self):
			self.MakeGemShopWindow()
			if self.wndGemShop.IsShow():
				self.wndGemShop.Close()
			else:
				self.wndGemShop.Open()
		def GemUpdateSlotCount(self, slotCount):
			pass
		def GemClear(self):
			if self.wndGemShop:
				self.wndGemShop.Clear()
		def GemSetRefreshLeftTime(self, leftTime):
			pass
		def GemSetItemsWithString(self, cmdData):
			if self.wndGemShop:
				self.wndGemShop.SetItemsWithString(cmdData)
		def GemSetBuyedSlot(self, slotIndex, buyedStatus):
			pass

	if app.ENABLE_MINI_GAME_CATCH_KING:
		def MiniGameCatchKingEventStart(self, bigScore):
			if self.wndCatchKingGame:
				self.wndCatchKingGame.GameStart(bigScore)

		def MiniGameCatchKingSetHandCard(self, cardNumber):
			if self.wndCatchKingGame:
				self.wndCatchKingGame.CatchKingSetHandCard(cardNumber)

		def MiniGameCatchKingResultField(self, score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear):
			if self.wndCatchKingGame:
				self.wndCatchKingGame.CatchKingResultField(score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear)

		def MiniGameCatchKingSetEndCard(self, cardPos, cardNumber):
			if self.wndCatchKingGame:
				self.wndCatchKingGame.CatchKingSetEndCard(cardPos, cardNumber)

		def MiniGameCatchKingReward(self, rewardCode):
			if self.wndCatchKingGame:
				self.wndCatchKingGame.CatchKingReward(rewardCode)

		def ClickCatchKingEventButton(self):
			if self.wndCatchKingGame:
				self.wndCatchKingGame.Open()

		def MiniGameCatchKing(self, value):
			import dbg
			if int(value) == 0:
				self.wndCatchKingIcon.Hide()
			else:
				self.wndCatchKingIcon.Show()

	if app.RENEWAL_MISSION_BOOKS:
		def MakeBookMission(self):
			if self.wndBookMission == None:
				self.wndBookMission = uiMission.MissionWindow()
		def OpenBookMission(self):
			self.MakeBookMission()
			if self.wndBookMission.IsShow():
				self.wndBookMission.Close()
			else:
				self.wndBookMission.Open()
		def ClearBookMission(self):
			self.MakeBookMission()
			self.wndBookMission.Clear()
		def UpdateMissionInfo(self, cmd):
			self.wndBookMission.UpdateMissionInfo(cmd)
		def UpdateMissionValue(self, missionID, value):
			self.wndBookMission.UpdateMissionValue(int(missionID), int(value))
		def RewardMissionData(self, missionID, rewardStatus):
			self.wndBookMission.RewardMissionData(int(missionID), int(rewardStatus))
		def RemoveMissionData(self, missionID):
			self.wndBookMission.RemoveMissionData(int(missionID))
		def UpdateMissionEndTime(self, missionID, endTime):
			self.wndBookMission.UpdateMissionEndTime(int(missionID), str(endTime))

	if app.ENABLE_RANKING:
		def OpenRanking(self):
			if self.wndRanking:
				self.wndRanking.Open()

		def RankingClearData(self):
			if self.wndRanking:
				self.wndRanking.ClearData()

		def RankingAddRank(self, position, level, points, name, realPosition):
			if self.wndRanking:
				self.wndRanking.AddRank(position, name, points, level, realPosition)

		def RankingRefresh(self):
			if self.wndRanking:
				self.wndRanking.RefreshList()
				self.wndRanking.OnScroll()

	if app.ENABLE_DS_SET_RENEWAL:
		def SetDSSet(self, idx):
			if self.wndDragonSoul:
				self.wndDragonSoul.SetDSSet(idx)

	if app.ENABLE_REWARD_SYSTEM:
		def MakeRewardWindow(self):
			if self.wndReward == None:
				self.wndReward = uiReward.RewardWindow()
		def SetRewardPlayers(self, data):
			self.MakeRewardWindow()
			self.wndReward.SetRewardPlayers(data)
		def OpenRewardWindow(self):
			self.MakeRewardWindow()
			if self.wndReward.IsShow():
				self.wndReward.Close()
			else:
				self.wndReward.Open()

	if app.ENABLE_SWITCHBOT_WORLDARD:
		def BINARY_SWITCHBOT_OPEN(self):
			self.wndSwitchBot.Show()
			
		def BINARY_SWITCHBOT_CLEAR_INFO(self):
			self.wndSwitchBot.func_clear_bonus_item()

		def BINARY_SWITCHBOT_INFO_BONUS(self,id_bonus,bonus_value_0,bonus_value_1,bonus_value_2,bonus_value_3,bonus_value_4):
			self.wndSwitchBot.func_set_bonus_items(id_bonus,bonus_value_0,bonus_value_1,bonus_value_2,bonus_value_3,bonus_value_4)

		def BINARY_SWITCHBOT_INFO_EXTRA(self):
			self.wndSwitchBot.func_add_mh()

	def OpenSwitch(self):
		self.ToggleSwitchbotWindow()

	if app.__DUNGEON_INFO__:
		def DungeonInfoQuestIdx(self, questIdx):
			constInfo.dungeonInfoQuestIdx = int(questIdx)
		def DungeonInfoQuestCMD(self):
			net.SendQuestInputStringPacket(constInfo.dungeonInfoCMD)
			constInfo.dungeonInfoCMD = ""
		def OpenDungeonInfo(self):
			if self.wndDungeonTrack != None:
				if self.wndDungeonTrack.IsShow():
					self.wndDungeonTrack.Close()
				else:
					self.wndDungeonTrack.Open()
		def DungeonInfoCooldown(self, clean, data):
			dungeonData = uiDungeonTrack.dungeonData
			if int(clean):
				for mobIdx, dungeonInfo in dungeonData.items():
					dungeonInfo["server_cooldown"] = 0
					dungeonInfo["server_completed"] = 0
					dungeonInfo["server_fastest"] = 0
					dungeonInfo["server_damage"] = 0
					dungeonInfo["whisper_info"] = False
			if data != "" and data != "-":
				dungeonList = data[:len(data)-1].split("#")
				for dungeonInfo in dungeonList:
					dungeonSplitData = dungeonInfo.split("|")
					if len(dungeonSplitData) == 5:
						bossIdx = int(dungeonSplitData[0])
						if dungeonData.has_key(bossIdx):
							if dungeonData[bossIdx]["server_cooldown"] != app.GetGlobalTimeStamp() + int(dungeonSplitData[1]):
								dungeonData[bossIdx]["whisper_info"] = False
							dungeonData[bossIdx]["server_cooldown"] = 0 if int(dungeonSplitData[1]) <= 0 else app.GetGlobalTimeStamp() + int(dungeonSplitData[1])
							dungeonData[bossIdx]["server_completed"] = int(dungeonSplitData[2])
							dungeonData[bossIdx]["server_fastest"] = int(dungeonSplitData[3])
							dungeonData[bossIdx]["server_damage"] = int(dungeonSplitData[4])
			if self.wndDungeonManager == None:
				self.wndDungeonManager = uiDungeonTrack.DungeonManager()
			if self.wndDungeonTrack == None:
				self.wndDungeonTrack = uiDungeonTrack.DungeonTrack()
			self.wndDungeonTrack.Refresh()
		def LoadServerData(self, mobIdx, rankIdx, cmd):
			if self.wndDungeonTrack != None:
				self.wndDungeonTrack.LoadServerData(int(mobIdx), int(rankIdx), cmd)

	if app.__AUTO_SKILL_READER__:
		def OpenAutoSkillReader(self):
			if not self.wndAutoSkillReader:
				self.wndAutoSkillReader = uiAutoSkillReader.Window()

			if self.wndAutoSkillReader.IsShow():
				self.wndAutoSkillReader.Hide()
			else:
				self.wndAutoSkillReader.Show()

		def AutoSkillStatus(self, status):
			if self.wndAutoSkillReader:
				self.wndAutoSkillReader.ServerSetStatus(status)

	if app.__AUTO_HUNT__:
		def AutoHuntStatus(self, status):
			if self.wndAutoHunt:
				self.wndAutoHunt.SetStatus(True if int(status) else False)
		def OpenAutoHunt(self):
			if self.wndAutoHunt == None:
				self.wndAutoHunt = uiAutoHunt.Window()
			if self.wndAutoHunt.IsShow():
				self.wndAutoHunt.Close()
			else:
				self.wndAutoHunt.Open()

	if app.__SPIN_WHEEL__:
		def SetSpinReward(self, selectedItemIdx, selectedItemCount, cmdData):
			if self.wndSpinWheel:
				self.wndSpinWheel.SetSpinReward(selectedItemIdx, selectedItemCount, cmdData)
		def SetSpinWheel(self, count, show):
			if self.wndSpinWheel == None:
				self.wndSpinWheel = uiSpinWheel.Window()
			self.wndSpinWheel.SetSpinData(int(count))
			if int(show):
				self.wndSpinWheel.Open()
			else:
				self.wndSpinWheel.Close()

	if app.__ROULETTE__:
		def OpenRoulette(self, gameKey = -1, itemData = "", selectedIdx = app.GetRandom(0, 19)):
			if self.wndRoulette == None:
				self.wndRoulette = uiRoulette.Window()

			if gameKey == -1:
				if self.wndRoulette.IsShow():
					self.wndRoulette.Close()
				else:
					self.wndRoulette.Open()
			else:
				self.wndRoulette.Start(int(gameKey), itemData, int(selectedIdx))

	def OpenWarpShowerWindow(self):
		if self.wndMiniMap and self.wndMiniMap.IsShowingAtlas():
			self.wndMiniMap.ToggleAtlasWindow()

		self.HideAllQuestButton()
		self.HideAllWhisperButton()

		self.HideAllWindows()
		if self.wndInventory:
			self.wndInventory.Hide(True)

		# self.wndWarpShower.Open(False)

	def CloseWarpShowerWindow(self):
		pass
		# if self.wndWarpShower:
			# self.wndWarpShower.Close()

	def GetInventoryPageIndex(self):
		if self.wndInventory:
			return self.wndInventory.GetInventoryPageIndex()
		else:
			return -1

	def AttachItemFromSwitchbot(self, slotIndex, itemIndex):
		if self.wndInventory and self.wndInventory.IsShow():
			self.wndInventory.AttachItemFromSwitchbot(slotIndex, itemIndex)

		return True

	if app.ENABLE_GAME_OPTION_ESCAPE:
		def IsEscapeQuestionPopupOpen(self):
			return self.wndEscapePopup and self.wndEscapePopup.IsEscapeQuestionPopupOpen()

		def OpenEscapeQuestionPopup(self):
			if not self.wndEscapePopup:
				self.wndEscapePopup = uiEscapePopup.EscapeManager()
			self.wndEscapePopup.OpenEscapePopup()

	def UseDSSButtonEffect(self, enable):
		if self.wndInventory:
			self.wndInventory.UseDSSButtonEffect(enable)

	if app.ENABLE_BATTLEPASS:
		def CreateBattlePassWindow(self):
			self.wndBattlePass = uibattlepass.BattlePassWindow(self.tooltipItem)
			self.wndBattlePass.Hide()

		def ToggleBattlePass(self):
			if not self.wndBattlePass:
				self.CreateWikipediaWindow()

			if self.wndBattlePass.IsShow():
				self.wndBattlePass.Close()
			else:
				self.wndBattlePass.Open()

	if app.__BL_BATTLE_ROYALE__:
		def ClickBattleRoyaleEventButton(self):
			if self.wndBattleRoyaleApplication:
				self.wndBattleRoyaleApplication.Open()

		def OpenElectricFieldOnScreenImage(self, open, parent_window):
			if self.wndBattleRoyaleInterface:
				self.wndBattleRoyaleInterface.OpenElectricFieldOnScreenImage(open, parent_window)
		
		def BattleRoyaleProcess(self, category, type, data = []):
			if category == player.BATTLE_ROYALE_GC_PROCESS_CATEGORY_MINI_GAME:
				if self.wndBattleRoyaleApplication:
					self.wndBattleRoyaleApplication.BattleRoyaleProcess(type, data)
			elif category == player.BATTLE_ROYALE_GC_PROCESS_CATEGORY_BATTLE_ROYALE_INTERFACE:
				if self.wndBattleRoyaleInterface:
					self.wndBattleRoyaleInterface.BattleRoyaleProcess(type, data)

		def SetMinimapTimer(self, second):
			if self.wndMiniMap:
				self.wndMiniMap.SetMinimapTimer(second)

	def SetPendantPageIdx(self, pageIdx, secondOpened):
		self.wndInventory.SetPendantPageIdx(int(pageIdx), int(secondOpened))

	if app.ENABLE_VOICE_CHAT:
		def ToggleVoiceChatConfigWindow(self):
			if self.wndVoiceChatConfig:
				if self.wndVoiceChatConfig.IsShow():
					self.wndVoiceChatConfig.Hide()
				else:
					self.wndVoiceChatConfig.Show()
					self.wndVoiceChatConfig.SetTop()

	if app.USE_CRYSTAL_SYSTEM:
		def OpenShardCraft(self):
			if not self.wndShardCraftWnd:
				import uishardcraft
				self.wndShardCraftWnd = uishardcraft.CraftWindow()

			self.wndShardCraftWnd.ShowRecv()

		def CloseShardCraft(self):
			if self.wndShardCraftWnd:
				self.wndShardCraftWnd.HideRecv()

				if self.wndInventory and\
					self.wndInventory.IsShow():
					self.wndInventory.RefreshItemSlot()

		def AddShardCraft(self, sourceSlot, targetSlot):
			if self.wndShardCraftWnd:
				if self.wndShardCraftWnd.Add(sourceSlot, targetSlot) == True:
					if self.wndInventory and\
						self.wndInventory.IsShow():
						self.wndInventory.RefreshItemSlot()

					return True

			return False

		def InvAddShardCraft(self, sourceSlot):
			if self.wndShardCraftWnd:
				return self.wndShardCraftWnd.InvAdd(sourceSlot)

			return False

		def OutShardCraft(self, targetSlot):
			if self.wndShardCraftWnd:
				self.wndShardCraftWnd.Out(targetSlot)

				if self.wndInventory and\
					self.wndInventory.IsShow():
					self.wndInventory.RefreshItemSlot()

		def OutAllShardCraft(self):
			if self.wndShardCraftWnd:
				self.wndShardCraftWnd.OutAll()

				if self.wndInventory and\
					self.wndInventory.IsShow():
					self.wndInventory.RefreshItemSlot()
