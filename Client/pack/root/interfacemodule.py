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
import ikashop
import net
if app.ENABLE_CONQUEROR_UI:
	import uicharacternew as uiCharacter
else:
	import uiCharacter
import uiInventory
import uiDragonSoul
import uiChat
import uiMessenger
import guild

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
import uiCommon
import uiRefine
import uiEquipmentDialog
import uiGameButton
import uiTip
import uiCube
import miniMap
# ACCESSORY_REFINE_ADD_METIN_STONE
import uiSelectItem
# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
import uiScriptLocale
import uiMaintenance
import uiBusyAction
import uiHorseInventory
import uiReport

import event
import localeInfo
import captcha

if app.ENABLE_ACCE_COSTUME_SYSTEM:
	import uiacce

if app.ENABLE_MOVE_CHANNEL:
	import uiMoveChannel

if app.ENABLE_WON_EXCHANGE_WINDOW:
	import uiWonExchange

import uiPrivateShopBuilder
import offlineShopBuilder
import offlineShopManage
import offlineShopGuest
import offlineShopHistory
import offlineShopSearch
import uiItemShop

import uiItemExchange
import uiPlayerStat
import flamewindPath

import eventManager
import uiFishing
import uiCraft
import uiAttributeList
import uiSpecialShop
import uiReputation
import uiPotionRecharge

import uiCaptcha

IsQBHide = 0
class Interface(object):
	CHARACTER_STATUS_TAB = 1
	CHARACTER_SKILL_TAB = 2

	def __init__(self):
		systemSetting.SetInterfaceHandler(self)
		self.windowOpenPosition = 0
		self.dlgWhisperWithoutTarget = None
		self.inputDialog = None
		self.tipBoard = None
		self.bigBoard = None
		self.fancyBoard = None

		# ITEM_MALL
		self.mallPageDlg = None
		# END_OF_ITEM_MALL

		self.wndWeb = None
		self.wndTaskBar = None
		self.wndCharacter = None
		self.wndInventory = None
		self.wndItemShop = None
		self.wndExpandedTaskBar = None
		self.wndDragonSoul = None
		self.wndDragonSoulRefine = None
		self.wndChat = None
		self.wndMessenger = None
		self.wndMiniMap = None
		self.wndGuild = None
		self.wndGuildBuilding = None

		self.wndPopupDialog = None

		self.listGMName = {}
		self.wndQuestWindow = {}
		self.wndQuestWindowNewKey = 0
		self.privateShopAdvertisementBoardDict = {"player":{}, "offline":{}}
		self.reputationBarDict = {}
		self.guildScoreBoardDict = {}
		self.equipmentDialogDict = {}
		if app.ENABLE_MOVE_CHANNEL:
			self.wndMoveChannel = None
		if app.ENABLE_WON_EXCHANGE_WINDOW:
			self.wndWonExchange = None
		event.SetInterfaceWindow(self)

		self.interfaceWindowList = {}

		self.RegisterEvents()
		self.popupManager = uiCommon.PopupManager()

	def __del__(self):
		systemSetting.DestroyInterfaceHandler()
		event.SetInterfaceWindow(None)

	def RegisterEvents(self):
		eventMgr = eventManager.EventManager()
		eventMgr.add_observer(eventManager.OPEN_WHISPER_EVENT, self.OpenWhisperDialog)
		eventMgr.add_observer(eventManager.EVENT_MARK_SHOP_VIEWED, self.MarkPrivateShopAsViewed)
		eventMgr.add_observer(eventManager.EVENT_MARK_SHOP_CURRENT, self.MarkPrivateShopAsCurrent)
		eventMgr.add_observer(uiShop.EVENT_CLICK_PRIVATE_SHOP, self.ClickPrivateShop)
		eventMgr.add_observer(uiMessenger.EVENT_UPDATE_BLOCK_STATE, self.__UpdateUserBlockState)

	################################
	## Make Windows & Dialogs
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

	def __MakeTaskBar(self):
		wndTaskBar = uiTaskBar.TaskBar()
		wndTaskBar.LoadWindow()
		self.wndTaskBar = wndTaskBar
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_CHARACTER, ui.__mem_func__(self.ToggleCharacterWindowStatusPage))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_INVENTORY, ui.__mem_func__(self.ToggleInventoryWindow))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_MESSENGER, ui.__mem_func__(self.ToggleMessenger))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_SYSTEM, ui.__mem_func__(self.ToggleSystemDialog))
		# if uiTaskBar.TaskBar.IS_EXPANDED:
		# 	self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_EXPAND, ui.__mem_func__(self.ToggleExpandedButton))
		# 	self.wndExpandedTaskBar = uiTaskBar.ExpandedTaskBar()
		# 	self.wndExpandedTaskBar.LoadWindow()
		# 	self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskBar.ExpandedTaskBar.BUTTON_DRAGON_SOUL, ui.__mem_func__(self.ToggleDragonSoulWindow))
		#
		# else:
		# 	self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_CHAT, ui.__mem_func__(self.ToggleChat))

		self.wndEnergyBar = None
		import app
		if app.ENABLE_ENERGY_SYSTEM:
			wndEnergyBar = uiTaskBar.EnergyBar()
			wndEnergyBar.LoadWindow()
			self.wndEnergyBar = wndEnergyBar

	def __MakeParty(self):
		wndParty = uiParty.PartyWindow()
		wndParty.Hide()
		self.wndParty = wndParty

	def __MakeGameButtonWindow(self):
		wndGameButton = uiGameButton.GameButtonWindow()
		wndGameButton.SetTop()
		wndGameButton.Show()
		wndGameButton.SetButtonEvent("STATUS", ui.__mem_func__(self.__OnClickStatusPlusButton))
		wndGameButton.SetButtonEvent("SKILL", ui.__mem_func__(self.__OnClickSkillPlusButton))
		wndGameButton.SetButtonEvent("QUEST", ui.__mem_func__(self.__OnClickQuestButton))
		wndGameButton.SetButtonEvent("HELP", ui.__mem_func__(self.__OnClickHelpButton))
		wndGameButton.SetButtonEvent("BUILD", ui.__mem_func__(self.__OnClickBuildButton))

		self.wndGameButton = wndGameButton

	def __IsChatOpen(self):
		return True

	def __MakeWindows(self):
		wndCharacter = uiCharacter.CharacterWindow()
		wndInventory = uiInventory.InventoryWindow()
		wndInventory.BindInterfaceClass(self)
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			wndDragonSoul = uiDragonSoul.DragonSoulWindow()
			wndDragonSoulRefine = uiDragonSoul.DragonSoulRefineWindow()
		else:
			wndDragonSoul = None
			wndDragonSoulRefine = None

		wndMiniMap = uiMiniMap.MiniMap()
		wndSafebox = uiSafebox.SafeboxWindow()

		# ITEM_MALL
		wndMall = uiSafebox.MallWindow()
		self.wndMall = wndMall
		# END_OF_ITEM_MALL

		wndChatLog = uiChat.ChatLogWindow()
		wndChatLog.BindInterface(self)

		self.wndCharacter = wndCharacter
		self.wndInventory = wndInventory
		self.wndDragonSoul = wndDragonSoul
		self.wndDragonSoulRefine = wndDragonSoulRefine
		self.wndMiniMap = wndMiniMap
		self.wndSafebox = wndSafebox
		self.wndChatLog = wndChatLog

		self.wndItemShop = uiItemShop.ItemShopWindow()
		self.wndItemShop.Hide()

		self.wndCrafting = uiCraft.CraftingWindow()
		self.wndCrafting.Hide()

		self.wndHorseInventory = uiHorseInventory.HorseInventoryWindow()
		self.wndHorseInventory.Hide()
		self.wndInventory.SetHorseInventory(self.wndHorseInventory)

		self.wndAttributeList = uiAttributeList.AttributeListWindow()
		self.wndAttributeList.Hide()

		self.wndSpecialShop = uiSpecialShop.SpecialShopWindow()
		self.wndSpecialShop.Hide()

		self.wndPlayerStat = uiPlayerStat.PlayerStatsWindow()
		self.wndPlayerStat.Hide()

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)
			self.wndDragonSoulRefine.SetInventoryWindows(self.wndInventory, self.wndDragonSoul)
			self.wndInventory.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)

		if app.ENABLE_MOVE_CHANNEL:
			self.wndMoveChannel = uiMoveChannel.MoveChannelWindow()

		if app.ENABLE_WON_EXCHANGE_WINDOW:
			self.wndWonExchange = uiWonExchange.WonExchangeWindow()
			self.wndWonExchange.BindInterface(self)

	def __MakeDialogs(self):
		self.tooltip = uiToolTip.ToolTip()
		self.tooltip.Hide()

		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()

		self.tooltipSkill = uiToolTip.SkillToolTip()
		self.tooltipSkill.Hide()

		self.dlgExchange = uiExchange.ExchangeDialog()
		self.dlgExchange.LoadDialog()
		self.dlgExchange.SetCenterPosition()
		self.dlgExchange.Hide()

		self.dlgPointReset = uiPointReset.PointResetDialog()
		self.dlgPointReset.LoadDialog()
		self.dlgPointReset.Hide()

		self.dlgShop = uiShop.ShopDialog()
		self.dlgShop.LoadDialog()
		self.dlgShop.Hide()

		self.dlgRestart = uiRestart.RestartDialog()
		self.dlgRestart.LoadDialog()
		self.dlgRestart.Hide()

		self.dlgSystem = uiSystem.SystemDialog()
		self.dlgSystem.LoadDialog()
		self.dlgSystem.SetToolTip(self.tooltip)
		self.dlgSystem.SetOpenHelpWindowEvent(ui.__mem_func__(self.OpenHelpWindow))
		self.dlgSystem.BindInterface(self)
		self.dlgSystem.Hide()

		self.dlgPassword = uiSafebox.PasswordDialog()
		self.dlgPassword.Hide()

		self.itemExchangeDialog = uiItemExchange.ExchangeItemDialog()
		self.itemExchangeDialog.SetItemToolTip(self.tooltipItem)
		self.itemExchangeDialog.Hide()

		self.offlineShopBuilder = offlineShopBuilder.OfflineShopBuilder()
		self.offlineShopBuilder.Hide()

		self.offlineShopManage = offlineShopManage.OfflineShopManage()
		self.offlineShopManage.Hide()

		self.offlineShopGuest = offlineShopGuest.OfflineShopGuest()
		self.offlineShopGuest.Hide()

		self.offlineShopHistory = offlineShopHistory.OfflineShopHistory()
		self.offlineShopHistory.Hide()

		self.offlineShopSearch = offlineShopSearch.ShopSearchWindow()
		self.offlineShopSearch.SetToolTip(self.tooltipItem)
		self.offlineShopSearch.Hide()

		self.hyperlinkItemTooltip = uiToolTip.HyperlinkItemToolTip()
		self.hyperlinkItemTooltip.Hide()

		self.dlgRefineNew = uiRefine.RefineDialogNew()
		self.dlgRefineNew.Hide()

		self.fishingGameDialog = uiFishing.FishingGameDialog()
		self.fishingGameDialog.Hide()

		self.potionRechargeDialog = uiPotionRecharge.PotionRechargeDialog()
		self.potionRechargeDialog.SetItemToolTip(self.tooltipItem)
		self.potionRechargeDialog.Hide()

		self.busyActionDialog = uiBusyAction.BusyActionDialog()
		self.busyActionDialog.Hide()

		self.maintenanceDialog = uiMaintenance.MaintenanceDialog()
		self.maintenanceDialog.Hide()

		self.reportPlayerDialog = uiReport.ReportPlayerDialog()
		self.reportPlayerDialog.Hide()

		self.captchaDialog = uiCaptcha.CaptchaDialog()
		self.captchaDialog.Hide()

		self.gameMasterTargetDialog = None

	def __MakeHelpWindow(self):
		self.wndHelp = uiHelp.HelpWindow()
		self.wndHelp.LoadDialog()
		self.wndHelp.SetCloseEvent(ui.__mem_func__(self.CloseHelpWindow))
		self.wndHelp.Hide()

	def __MakeTipBoard(self):
		self.tipBoard = uiTip.TipBoard()
		self.tipBoard.Hide()

		self.bigBoard = uiTip.BigBoard()
		self.bigBoard.Hide()

		self.fancyBoard = uiTip.FancyBoard()
		self.fancyBoard.Hide()

	def __MakeWebWindow(self):
		if constInfo.IN_GAME_SHOP_ENABLE:
			import uiWeb
			self.wndWeb = uiWeb.WebWindow()
			self.wndWeb.LoadWindow()
			self.wndWeb.Hide()

	def __MakeCubeWindow(self):
		self.wndCube = uiCube.CubeWindow()
		self.wndCube.LoadWindow()
		self.wndCube.Hide()

	def __MakeCubeResultWindow(self):
		self.wndCubeResult = uiCube.CubeResultWindow()
		self.wndCubeResult.LoadWindow()
		self.wndCubeResult.Hide()

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def __MakeAcceWindow(self):
			self.wndAcceCombine = uiacce.CombineWindow()
			self.wndAcceCombine.LoadWindow()
			self.wndAcceCombine.Hide()

			self.wndAcceAbsorption = uiacce.AbsorbWindow()
			self.wndAcceAbsorption.LoadWindow()
			self.wndAcceAbsorption.Hide()

			if self.wndInventory:
				self.wndInventory.SetAcceWindow(self.wndAcceCombine, self.wndAcceAbsorption)

	# ACCESSORY_REFINE_ADD_METIN_STONE
	def __MakeItemSelectWindow(self):
		self.wndItemSelect = uiSelectItem.SelectItemWindow()
		self.wndItemSelect.Hide()
	# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

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
		self.__MakeCubeWindow()
		self.__MakeCubeResultWindow()
		if app.ENABLE_ACCE_COSTUME_SYSTEM:
			self.__MakeAcceWindow()

		# ACCESSORY_REFINE_ADD_METIN_STONE
		self.__MakeItemSelectWindow()
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

		#gamemater
		self.gameMaster_CaptchaDialogs = {}

		self.questButtonList = []
		self.whisperButtonList = []
		self.whisperDialogDict = {}
		self.privateShopAdvertisementBoardDict = {"player":{}, "offline":{}}
		self.reputationBarDict = {}

		self.wndInventory.SetItemToolTip(self.tooltipItem)
		self.wndItemShop.SetItemToolTip(self.tooltipItem)
		self.wndItemShop.SetToolTip(self.tooltip)

		self.dlgRefineNew.SetItemToolTip(self.tooltipItem)

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetItemToolTip(self.tooltipItem)
			self.wndDragonSoulRefine.SetItemToolTip(self.tooltipItem)
		self.wndSafebox.SetItemToolTip(self.tooltipItem)
		self.wndCube.SetItemToolTip(self.tooltipItem)
		self.wndCubeResult.SetItemToolTip(self.tooltipItem)

		if app.ENABLE_ACCE_COSTUME_SYSTEM:
			self.wndAcceCombine.SetItemToolTip(self.tooltipItem)
			self.wndAcceAbsorption.SetItemToolTip(self.tooltipItem)

		# ITEM_MALL
		self.wndMall.SetItemToolTip(self.tooltipItem)
		# END_OF_ITEM_MALL

		self.wndCharacter.SetSkillToolTip(self.tooltipSkill)
		self.wndTaskBar.SetItemToolTip(self.tooltipItem)
		self.wndTaskBar.SetSkillToolTip(self.tooltipSkill)
		self.wndGuild.SetSkillToolTip(self.tooltipSkill)

		# ACCESSORY_REFINE_ADD_METIN_STONE
		self.wndItemSelect.SetItemToolTip(self.tooltipItem)
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

		self.dlgShop.SetItemToolTip(self.tooltipItem)
		self.dlgExchange.SetItemToolTip(self.tooltipItem)
		self.offlineShopBuilder.SetToolTip(self.tooltip, self.tooltipItem)
		self.offlineShopManage.SetToolTip(self.tooltip, self.tooltipItem)
		self.offlineShopGuest.SetToolTip(self.tooltip, self.tooltipItem)
		self.wndHorseInventory.SetToolTip(self.tooltip)
		self.wndCrafting.SetItemToolTip(self.tooltipItem)

		self.__InitWhisper()
		self.DRAGON_SOUL_IS_QUALIFIED = True if app.ENABLE_NO_DSS_QUALIFICATION else False

		self.__InitializeWindows()

	def __InitializeWindows(self):
		pass

	def _AppendInterfaceWindow(self, name, window):
		self.interfaceWindowList[name] = window

	def _GetInterfaceWindow(self, name):
		return self.interfaceWindowList.get(name, None)

	GetInterfaceWindow = _GetInterfaceWindow

	def MakeHyperlinkTooltip(self, hyperlink):
		tokens = hyperlink.split(":")
		if tokens and len(tokens):
			type = tokens[0]
			if "item" == type:
				self.hyperlinkItemTooltip.SetHyperlinkItem(tokens)
			elif "msg" == type:
				data = tokens[1].split(",")
				name = data[0]
				empire = int(data[1])
				self.OpenWhisperDialog(name, empire)

	## Make Windows & Dialogs
	################################

	def Close(self):
		if self.popupManager:
			self.popupManager.Destroy()
			del self.popupManager

		if self.dlgWhisperWithoutTarget:
			self.dlgWhisperWithoutTarget.Destroy()
			self.dlgWhisperWithoutTarget = None

		player_name = player.GetMainCharacterName()
		if constInfo.WHISPER_DICT.has_key(player_name):
			for target_name, whisper_data in constInfo.WHISPER_DICT[player_name].items():
				if whisper_data["dialog"]:
					whisper_data["dialog"].Destroy()
					whisper_data["dialog"] = None

		if uiQuest.QuestDialog.__dict__.has_key("QuestCurtain"):
			uiQuest.QuestDialog.QuestCurtain.Close()
			del uiQuest.QuestDialog.QuestCurtain #@fixme016 it's recreated only if it's deleted from scope

		if self.wndQuestWindow:
			for key, eachQuestWindow in self.wndQuestWindow.items():
				eachQuestWindow.nextCurtainMode = -1
				eachQuestWindow.CloseSelf()
				eachQuestWindow = None
		self.wndQuestWindow = {}

		chat.DestroyWhisperMap()

		if self.wndChat:
			self.wndChat.Destroy()

		if self.wndTaskBar:
			self.wndTaskBar.Destroy()

		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Destroy()

		if self.wndEnergyBar:
			self.wndEnergyBar.Destroy()

		if self.wndCharacter:
			self.wndCharacter.Destroy()

		if self.wndInventory:
			self.wndInventory.Destroy()

		if self.wndItemShop:
			self.wndItemShop.Destroy()

		if self.wndCrafting:
			self.wndCrafting.Destroy()

		if self.wndHorseInventory:
			self.wndHorseInventory.Destroy()

		if self.wndSpecialShop:
			self.wndSpecialShop.Destroy()

		if self.wndAttributeList:
			self.wndAttributeList.Destroy()

		if self.wndPlayerStat:
			self.wndPlayerStat.Destroy()

		if self.wndDragonSoul:
			self.wndDragonSoul.Destroy()

		if self.wndDragonSoulRefine:
			self.wndDragonSoulRefine.Destroy()

		if self.dlgExchange:
			self.dlgExchange.Destroy()

		if self.dlgPointReset:
			self.dlgPointReset.Destroy()

		if self.dlgShop:
			self.dlgShop.Destroy()

		if self.dlgRestart:
			self.dlgRestart.Destroy()

		if self.dlgSystem:
			self.dlgSystem.Destroy()

		if self.dlgPassword:
			self.dlgPassword.Destroy()

		if self.wndMiniMap:
			self.wndMiniMap.Destroy()

		if self.wndSafebox:
			self.wndSafebox.Destroy()

		if self.wndWeb:
			self.wndWeb.Destroy()
			self.wndWeb = None

		if self.wndMall:
			self.wndMall.Destroy()

		if self.wndParty:
			self.wndParty.Destroy()

		if self.wndHelp:
			self.wndHelp.Destroy()

		if self.wndCube:
			self.wndCube.Destroy()

		if app.ENABLE_ACCE_COSTUME_SYSTEM and self.wndAcceCombine:
			self.wndAcceCombine.Destroy()

		if app.ENABLE_ACCE_COSTUME_SYSTEM and self.wndAcceAbsorption:
			self.wndAcceAbsorption.Destroy()

		if app.ENABLE_MOVE_CHANNEL and self.wndMoveChannel:
			self.wndMoveChannel.Destroy()
			self.wndMoveChannel = None

		if app.ENABLE_WON_EXCHANGE_WINDOW:
			self.wndWonExchange.Destroy()
			self.wndWonExchange = None

		if self.wndCubeResult:
			self.wndCubeResult.Destroy()

		if self.wndMessenger:
			self.wndMessenger.Destroy()

		if self.wndGuild:
			self.wndGuild.Destroy()

		if self.offlineShopBuilder:
			self.offlineShopBuilder.Destroy()

		if self.offlineShopManage:
			self.offlineShopManage.Destroy()

		if self.offlineShopGuest:
			self.offlineShopGuest.Destroy()

		if self.offlineShopHistory:
			self.offlineShopHistory.Destroy()

		if self.offlineShopSearch:
			self.offlineShopSearch.Destroy()

		if self.dlgRefineNew:
			self.dlgRefineNew.Destroy()

		if self.wndGuildBuilding:
			self.wndGuildBuilding.Destroy()

		if self.wndGameButton:
			self.wndGameButton.Destroy()

		if self.fishingGameDialog:
			self.fishingGameDialog.Destroy()
			self.fishingGameDialog.Hide()

		if self.potionRechargeDialog:
			self.potionRechargeDialog.Destroy()
			self.potionRechargeDialog.Hide()

		if self.busyActionDialog:
			self.busyActionDialog.Destroy()
			self.busyActionDialog.Hide()

		if self.maintenanceDialog:
			self.maintenanceDialog.Destroy()
			self.maintenanceDialog.Hide()

		if self.reportPlayerDialog:
			self.reportPlayerDialog.Destroy()
			self.reportPlayerDialog.Hide()

		if self.itemExchangeDialog:
			self.itemExchangeDialog.Destroy()
			self.itemExchangeDialog.Hide()

		if self.captchaDialog:
			self.captchaDialog.Destroy()
			self.captchaDialog.Hide()

		if self.gameMasterTargetDialog:
			self.gameMasterTargetDialog.Destroy()
			self.gameMasterTargetDialog.Hide()

		if self.gameMaster_CaptchaDialogs:
			for dialog in self.gameMaster_CaptchaDialogs.values():
				if dialog:
					dialog.Destroy()
					dialog.Hide()

		# ITEM_MALL
		if self.mallPageDlg:
			self.mallPageDlg.Destroy()
		# END_OF_ITEM_MALL

		# ACCESSORY_REFINE_ADD_METIN_STONE
		if self.wndItemSelect:
			self.wndItemSelect.Destroy()
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

		self.wndChatLog.Destroy()
		for btn in self.questButtonList:
			btn.SetEvent(0)
		for btn in self.whisperButtonList:
			btn.SetEvent(0)
		for dlg in self.whisperDialogDict.itervalues():
			dlg.Destroy()
		for brd in self.guildScoreBoardDict.itervalues():
			brd.Destroy()
		for dlg in self.equipmentDialogDict.itervalues():
			dlg.Destroy()

		# ITEM_MALL
		del self.mallPageDlg
		# END_OF_ITEM_MALL

		del self.wndGuild
		del self.wndMessenger
		del self.wndUICurtain
		del self.wndChat
		del self.wndTaskBar
		if self.wndExpandedTaskBar:
			del self.wndExpandedTaskBar
		del self.wndEnergyBar
		del self.wndCharacter
		del self.wndInventory
		del self.wndItemShop
		del self.wndCrafting
		del self.wndHorseInventory
		del self.wndSpecialShop
		del self.wndAttributeList
		del self.wndPlayerStat
		if self.wndDragonSoul:
			del self.wndDragonSoul
		if self.wndDragonSoulRefine:
			del self.wndDragonSoulRefine
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
		del self.wndCube
		del self.wndCubeResult
		del self.offlineShopBuilder
		del self.offlineShopManage
		del self.offlineShopGuest
		del self.offlineShopHistory
		del self.offlineShopSearch
		del self.inputDialog
		del self.wndChatLog
		del self.dlgRefineNew
		del self.wndGuildBuilding
		del self.wndGameButton
		del self.tipBoard
		del self.bigBoard
		del self.fancyBoard
		del self.wndItemSelect
		del self.fishingGameDialog
		del self.potionRechargeDialog
		del self.busyActionDialog
		del self.maintenanceDialog
		del self.reportPlayerDialog
		del self.itemExchangeDialog
		del self.captchaDialog
		del self.gameMasterTargetDialog

		if app.ENABLE_ACCE_COSTUME_SYSTEM:
			del self.wndAcceCombine
			del self.wndAcceAbsorption

		self.questButtonList = []
		self.whisperButtonList = []
		self.whisperDialogDict = {}
		self.privateShopAdvertisementBoardDict ={"player":{}, "offline":{}}
		self.reputationBarDict = {}
		self.guildScoreBoardDict = {}
		self.equipmentDialogDict = {}
		self.gameMaster_CaptchaDialogs = {}

		map(lambda wnd : wnd[1].Destroy(), self.interfaceWindowList.iteritems())
		self.interfaceWindowList = {}

		captcha.ClearAllCaptchaImage()
		uiChat.DestroyChatInputSetWindow()

	## Skill
	def OnUseSkill(self, slotIndex, coolTime):
		self.wndCharacter.OnUseSkill(slotIndex, coolTime)
		self.wndTaskBar.OnUseSkill(slotIndex, coolTime)
		self.wndGuild.OnUseSkill(slotIndex, coolTime)

	def OnActivateSkill(self, slotIndex):
		self.wndCharacter.OnActivateSkill(slotIndex)
		self.wndTaskBar.OnActivateSkill(slotIndex)

	def OnDeactivateSkill(self, slotIndex):
		self.wndCharacter.OnDeactivateSkill(slotIndex)
		self.wndTaskBar.OnDeactivateSkill(slotIndex)

	def OnChangeCurrentSkill(self, skillSlotNumber):
		self.wndTaskBar.OnChangeCurrentSkill(skillSlotNumber)

	def SelectMouseButtonEvent(self, dir, event):
		self.wndTaskBar.SelectMouseButtonEvent(dir, event)

	## Refresh
	def RefreshAlignment(self):
		self.wndCharacter.RefreshAlignment()

	def RefreshStatus(self):
		self.wndTaskBar.RefreshStatus()
		self.wndCharacter.RefreshStatus()
		self.wndInventory.RefreshGold()
		self.wndAttributeList.RefreshStatus()
		if self.wndEnergyBar:
			self.wndEnergyBar.RefreshStatus()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.RefreshStatus()

	def RefreshStamina(self):
		self.wndTaskBar.RefreshStamina()

	def RefreshSkill(self):
		self.wndCharacter.RefreshSkill()
		self.wndTaskBar.RefreshSkill()

	def RefreshInventory(self):
		self.wndTaskBar.RefreshQuickSlot()
		if self.wndInventory:
			self.wndInventory.RefreshItemSlot()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			if self.wndDragonSoul and self.wndDragonSoul.IsShow():
				self.wndDragonSoul.RefreshItemSlot()

	def RefreshCharacter(self):
		self.wndCharacter.RefreshCharacter()
		self.wndTaskBar.RefreshQuickSlot()

	def RefreshQuest(self):
		self.wndCharacter.RefreshQuest()

	def RefreshSafebox(self):
		self.wndSafebox.RefreshSafebox()

	# ITEM_MALL
	def RefreshMall(self):
		self.wndMall.RefreshMall()

	def OpenItemMall(self):
		if not self.mallPageDlg:
			self.mallPageDlg = uiShop.MallPageDialog()

		self.mallPageDlg.Open()
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
	def OpenShopDialog(self, vid):
		self.wndInventory.Show()
		self.wndInventory.SetTop()
		self.dlgShop.Open(vid)
		self.dlgShop.SetTop()

		eventManager.EventManager().send_event(eventManager.EVENT_MARK_SHOP_CURRENT, vid, False)

	def CloseShopDialog(self):
		self.dlgShop.Close()
		self.offlineShopGuest.CloseNormal()

	def RefreshShopDialog(self):
		self.dlgShop.Refresh()
		self.offlineShopGuest.RefreshNormal()

	## Quest
	def OpenCharacterWindowQuestPage(self):
		self.wndCharacter.Show()
		self.wndCharacter.SetState("QUEST")

	def OpenQuestWindow(self, skin, idx):

		wnds = ()

		q = uiQuest.QuestDialog(skin, idx)
		q.SetToolTip(self.tooltipItem)
		q.SetWindowName("QuestWindow" + str(idx))
		q.Show()
		if skin:
			q.Lock()
			wnds = self.__HideWindows()

			# UNKNOWN_UPDATE
			q.AddOnDoneEvent(lambda tmp_self, args=wnds: self.__ShowWindows(args))
			# END_OF_UNKNOWN_UPDATE

		if skin:
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

	## Party
	def AddPartyMember(self, pid, name):
		self.wndParty.AddPartyMember(pid, name)

		self.__ArrangeQuestButton()

	def UpdatePartyMemberInfo(self, pid):
		self.wndParty.UpdatePartyMemberInfo(pid)

	def RemovePartyMember(self, pid):
		self.wndParty.RemovePartyMember(pid)

		self.__ArrangeQuestButton()

	def LinkPartyMember(self, pid, vid):
		self.wndParty.LinkPartyMember(pid, vid)

	def UnlinkPartyMember(self, pid):
		self.wndParty.UnlinkPartyMember(pid)

	def UnlinkAllPartyMember(self):
		self.wndParty.UnlinkAllPartyMember()

	def ExitParty(self):
		self.wndParty.ExitParty()

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
		self.wndSafebox.ShowWindow(size)

	def RefreshSafeboxMoney(self):
		self.wndSafebox.RefreshSafeboxMoney()

	def CommandCloseSafebox(self):
		self.wndSafebox.CommandCloseSafebox()

	# ITEM_MALL
	def AskMallPassword(self):
		if self.wndMall.IsShow():
			return
		self.dlgPassword.SetTitle(localeInfo.MALL_PASSWORD_TITLE)
		self.dlgPassword.SetSendMessage("/mall_password ")
		self.dlgPassword.ShowDialog()

	def OpenMallWindow(self, size):
		self.dlgPassword.CloseDialog()
		self.wndMall.ShowWindow(size)

	def CommandCloseMall(self):
		self.wndMall.CommandCloseMall()
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

	## Refine
	def OpenRefineDialog(self, targetItemPos, nextGradeItemVnum, cost, prob, type):
		self.dlgRefineNew.Open(targetItemPos, nextGradeItemVnum, cost, prob, type)

	def AppendMaterialToRefineDialog(self, vnum, count):
		self.dlgRefineNew.AppendMaterial(vnum, count)

	## Show & Hide
	def ShowDefaultWindows(self):
		self.wndTaskBar.Show()
		self.wndMiniMap.Show()
		self.wndMiniMap.ShowMiniMap()
		if self.wndEnergyBar:
			self.wndEnergyBar.Show()

	def ShowAllWindows(self):
		self.wndTaskBar.Show()
		self.wndCharacter.Show()
		self.wndInventory.Show()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.Show()
			self.wndDragonSoulRefine.Show()
		self.wndChat.Show()
		self.wndMiniMap.Show()
		if self.wndEnergyBar:
			self.wndEnergyBar.Show()
		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Show()
			self.wndExpandedTaskBar.SetTop()

	def HideAllWindows(self):
		if self.wndTaskBar:
			self.wndTaskBar.Hide()

		if self.wndEnergyBar:
			self.wndEnergyBar.Hide()

		if self.wndCharacter:
			self.wndCharacter.Hide()

		if self.wndInventory:
			self.wndInventory.Hide()

		if self.wndItemShop:
			self.wndItemShop.Hide()

		if self.wndCrafting:
			self.wndCrafting.Hide()

		if self.wndSpecialShop:
			self.wndSpecialShop.Hide()

		if self.wndAttributeList:
			self.wndAttributeList.Hide()

		if self.wndPlayerStat:
			self.wndPlayerStat.Hide()

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.Hide()
			self.wndDragonSoulRefine.Hide()

		if self.wndChat:
			self.wndChat.Hide()

		if self.wndMiniMap:
			self.wndMiniMap.Hide()

		if self.wndMessenger:
			self.wndMessenger.Hide()

		if self.wndGuild:
			self.wndGuild.Hide()

		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Hide()

		if app.ENABLE_MOVE_CHANNEL and self.wndMoveChannel:
			self.wndMoveChannel.Hide()

		if app.ENABLE_WON_EXCHANGE_WINDOW:
			self.wndWonExchange.Hide()

		map(lambda wnd : wnd[1].Close(), self.interfaceWindowList.iteritems())

	def ShowMouseImage(self):
		self.wndTaskBar.ShowMouseImage()

	def HideMouseImage(self):
		self.wndTaskBar.HideMouseImage()

	def ToggleChat(self):
		if True == self.wndChat.IsEditMode():
			self.wndChat.CloseChat()
		else:
			if self.wndWeb and self.wndWeb.IsShow():
				pass
			else:
				self.wndChat.OpenChat()

	def IsOpenChat(self):
		return self.wndChat.IsEditMode()

	def SetChatFocus(self):
		self.wndChat.SetChatFocus()

	def OpenRestartDialog(self):
		self.dlgRestart.OpenDialog()
		self.dlgRestart.SetTop()

	def CloseRestartDialog(self):
		self.dlgRestart.Close()

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

	def ToggleTaskbarVisibility(self):
		if self.wndTaskBar.IsShow():
			self.wndTaskBar.Hide()
		else:
			self.wndTaskBar.Show()

	def ToggleMinimapVisibility(self):
		if self.wndMiniMap.IsShow():
			self.wndMiniMap.Hide()
		else:
			self.wndMiniMap.Show()

	def ToggleGameButtonVisibility(self):
		if self.wndGameButton.IsShow():
			self.wndGameButton.Hide()
		else:
			self.wndGameButton.Show()

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
					self.wndCharacter.Hide()
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

		elif app.ENABLE_HIGHLIGHT_NEW_ITEM and player.SLOT_TYPE_INVENTORY == inven_type:
			self.wndInventory.HighlightSlot(inven_pos)


	def DragonSoulGiveQuilification(self):
		self.DRAGON_SOUL_IS_QUALIFIED = True
		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.SetToolTipText(uiTaskBar.ExpandedTaskBar.BUTTON_DRAGON_SOUL, uiScriptLocale.TASKBAR_DRAGON_SOUL)

	def ToggleDragonSoulWindow(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoul.IsShow():
					if self.DRAGON_SOUL_IS_QUALIFIED:
						self.wndDragonSoul.Show()
					else:
						try:
							self.wndPopupDialog.SetText(localeInfo.DRAGON_SOUL_UNQUALIFIED)
							self.wndPopupDialog.Open()
						except:
							self.wndPopupDialog = uiCommon.PopupDialog()
							self.wndPopupDialog.SetText(localeInfo.DRAGON_SOUL_UNQUALIFIED)
							self.wndPopupDialog.Open()
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

	def ToggleChatLogWindow(self):
		if self.wndChatLog.IsShow():
			self.wndChatLog.Hide()
		else:
			self.wndChatLog.Show()

	def CheckGameButton(self):
		if self.wndGameButton:
			self.wndGameButton.CheckGameButton()

	def __OnClickStatusPlusButton(self):
		self.ToggleCharacterWindow("STATUS")

	def __OnClickSkillPlusButton(self):
		self.ToggleCharacterWindow("SKILL")

	def __OnClickQuestButton(self):
		self.ToggleCharacterWindow("QUEST")

	def __OnClickHelpButton(self):
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
		self.wndWeb.Open(url)

		self.wndChat.CloseChat()

	# show GIFT
	def ShowGift(self):
		self.wndTaskBar.ShowGift()

	def CloseWbWindow(self):
		self.wndWeb.Close()

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

		if 0:
			self.wndCubeResult.SetPosition(*self.wndCube.GetGlobalPosition())
			self.wndCubeResult.SetCubeResultItem(itemVnum, count)
			self.wndCubeResult.Open()
			self.wndCubeResult.SetTop()

	if app.ENABLE_MOVE_CHANNEL:
		def ToggleMoveChannelWindow(self):
			if not player.IsObserverMode():
				if not self.wndMoveChannel.IsShow():
					self.wndMoveChannel.Open()
				else:
					self.wndMoveChannel.Hide()

	if app.ENABLE_WON_EXCHANGE_WINDOW:
		def ToggleWonExchangeWindow(self):
			if player.IsObserverMode():
				return

			if not self.wndWonExchange.IsShow():
				self.wndWonExchange.Open()
				self.wndWonExchange.SetTop()
			else:
				self.wndWonExchange.Close()

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def ActAcce(self, iAct, bWindow):
			board = (self.wndAcceAbsorption,self.wndAcceCombine)[int(bWindow)]
			if iAct == 1:
				self.ActAcceOpen(board)
			elif iAct == 2:
				self.ActAcceClose(board)
			elif iAct == 3 or iAct == 4:
				self.ActAcceRefresh(board, iAct)

		def ActAcceOpen(self,board):
			if not board.IsOpened():
				board.Open()
			if not self.wndInventory.IsShow():
				self.wndInventory.Show()
			self.wndInventory.RefreshBagSlotWindow()

		def ActAcceClose(self,board):
			if board.IsOpened():
				board.Close()
			self.wndInventory.RefreshBagSlotWindow()

		def ActAcceRefresh(self,board,iAct):
			if board.IsOpened():
				board.Refresh(iAct)
			self.wndInventory.RefreshBagSlotWindow()

	def __HideWindows(self):
		hideWindows = self.wndTaskBar,\
						self.wndCharacter,\
						self.wndInventory,\
						self.wndMiniMap,\
						self.wndGuild,\
						self.wndMessenger,\
						self.wndChat,\
						self.wndParty,\
						self.wndGameButton, \
					  	self.wndItemShop

		if self.wndEnergyBar:
			hideWindows += self.wndEnergyBar,

		if self.wndExpandedTaskBar:
			hideWindows += self.wndExpandedTaskBar,

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			hideWindows += self.wndDragonSoul,\
						self.wndDragonSoulRefine,

		if app.ENABLE_MOVE_CHANNEL and self.wndMoveChannel:
			hideWindows += self.wndMoveChannel,
		
		for window in self.interfaceWindowList.values():
			hideWindows += window,

		hideWindows = filter(lambda x: x != None and x.IsShow(), hideWindows)
		map(lambda x: x.Hide(), hideWindows)

		self.HideAllQuestButton()
		self.HideAllWhisperButton()

		if self.wndChat.IsEditMode():
			self.wndChat.CloseChat()

		return hideWindows

	def __ShowWindows(self, wnds):
		map(lambda x:x.Show(), wnds)
		global IsQBHide
		if not IsQBHide:
			self.ShowAllQuestButton()
		else:
			self.HideAllQuestButton()

		self.ShowAllWhisperButton()

	def BINARY_OpenAtlasWindow(self):
		if self.wndMiniMap:
			self.wndMiniMap.ShowAtlas()

	def BINARY_SetObserverMode(self, flag):
		self.wndGameButton.SetObserverMode(flag)

	# ACCESSORY_REFINE_ADD_METIN_STONE
	def BINARY_OpenSelectItemWindow(self):
		self.wndItemSelect.Open()
	# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

	def AppearReputationBar(self, vid, fraction):
		if self.reputationBarDict.has_key(vid):
			self.reputationBarDict[vid].Open(vid, fraction)
			return

		self.reputationBarDict[vid] = uiReputation.ReputationBar()
		self.reputationBarDict[vid].Open(vid, fraction)

	def DisappearReputationBar(self, vid):
		if self.reputationBarDict.has_key(vid):
			self.reputationBarDict[vid].Hide()

	#####################################################################################
	### Private Shop ###

	def OpenPrivateShopInputNameDialog(self):
		#if player.IsInSafeArea():
		#	chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CANNOT_OPEN_PRIVATE_SHOP_IN_SAFE_AREA)
		#	return

		inputDialog = uiCommon.InputDialog()
		inputDialog.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_NAME_DIALOG_TITLE)
		inputDialog.SetMaxLength(32)
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OpenPrivateShopBuilder))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.ClosePrivateShopInputNameDialog))
		inputDialog.Open()
		self.inputDialog = inputDialog

	def ClosePrivateShopInputNameDialog(self):
		self.inputDialog = None
		return True

	def OpenPrivateShopBuilder(self):

		if not self.inputDialog:
			return True

		if not len(self.inputDialog.GetText()):
			return True

		self.offlineShopBuilder.Open(self.inputDialog.GetText())
		return True

	def OpenPrivateShopManage(self):
		self.offlineShopManage.Toggle()
		return True

	def ClickPrivateShop(self, vid, is_offline):
		if self.offlineShopGuest.IsShowNormal():
			self.offlineShopGuest.Close()
			eventManager.EventManager().send_delayed_event(uiShop.EVENT_CLICK_PRIVATE_SHOP, 0.15, vid, is_offline)
			return
		elif self.offlineShopGuest.IsShow():
			self.offlineShopGuest.Close()

		if is_offline:
			ikashop.SendOnClickPacket(vid)
		else:
			net.SendOnClickPacket(vid)

	def MarkPrivateShopAsViewed(self, vid, is_offline=False):
		shop_type = "offline" if is_offline else "player"
		if self.privateShopAdvertisementBoardDict[shop_type].has_key(vid):
			self.privateShopAdvertisementBoardDict[shop_type][vid].MarkAsViewed()

	def MarkPrivateShopAsCurrent(self, vid, is_offline=False):
		shop_type = "offline" if is_offline else "player"
		if self.privateShopAdvertisementBoardDict[shop_type].has_key(vid):
			self.privateShopAdvertisementBoardDict[shop_type][vid].MarkAsCurrent()

	def AppearPrivateShop(self, vid, text, is_offline_shop=False):

		board = uiPrivateShopBuilder.PrivateShopAdvertisementBoard()
		board.Open(vid, text, is_offline_shop)

		shop_type = "offline" if is_offline_shop else "player"
		self.privateShopAdvertisementBoardDict[shop_type][vid] = board

	def DisappearPrivateShop(self, vid, is_offline_shop=False):
		shop_type = "offline" if is_offline_shop else "player"
		if not self.privateShopAdvertisementBoardDict[shop_type].has_key(vid):
			return
		del self.privateShopAdvertisementBoardDict[shop_type][vid]

		uiPrivateShopBuilder.DeleteADBoard(vid, is_offline_shop)

	def ToggleOfflineShopVisibility(self, vid, state):
		if not self.privateShopAdvertisementBoardDict["offline"].has_key(vid):
			return

		self.privateShopAdvertisementBoardDict["offline"][vid].SetVisible(state)

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
		import item
		if "item"==iconType:
			item.SelectItem(int(iconName))
			buttonImageFileName=item.GetIconImageFileName()
		else:
			buttonImageFileName=iconName

		if iconName and (iconType not in ("item", "file")):  # type "ex" implied
			btn.SetUpVisual("d:/ymir work/ui/game/quest/questicon/%s.tga" % (iconName.replace("open", "close")))
			btn.SetOverVisual("d:/ymir work/ui/game/quest/questicon/%s.tga" % (iconName))
			btn.SetDownVisual("d:/ymir work/ui/game/quest/questicon/%s.tga" % (iconName))
		else:
			btn.SetUpVisual("d:/ymir work/ui/game/quest/questicon/scroll_close.tga")
			btn.SetDownVisual("d:/ymir work/ui/game/quest/questicon/scroll_open.tga")
			btn.SetOverVisual("d:/ymir work/ui/game/quest/questicon/scroll_open.tga")
		# END_OF_QUEST_LETTER_IMAGE

		btn.SetToolTipText(name, -20, 35)
		btn.ToolTipText.SetHorizontalAlignLeft()

		listOfTypes = iconType.split(",")
		if "blink" in listOfTypes:
			btn.Flash()

		listOfColors = {
			"golden":	0xFFffa200,
			"green":	0xFF00e600,
			"blue":		0xFF0099ff,
			"purple":	0xFFcc33ff,

			"fucsia":	0xFFcc0099,
			"aqua":		0xFF00ffff,
		}
		for k,v in listOfColors.iteritems():
			if k in listOfTypes:
				btn.ToolTipText.SetPackedFontColor(v)

		btn.SetEvent(ui.__mem_func__(self.__StartQuest), btn)
		btn.Show()

		btn.index = index
		btn.name = name

		self.questButtonList.insert(0, btn)
		self.__ArrangeQuestButton()

	def __ArrangeQuestButton(self):

		screenWidth = wndMgr.GetScreenWidth()
		screenHeight = wndMgr.GetScreenHeight()

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

			btn.SetPosition(xPos + (int(count/yCount) * 100), yPos + (count%yCount * 63))
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
		for btn in self.questButtonList:
			btn.Show()
	#####################################################################################

	#####################################################################################
	### Whisper ###

	def __UpdateUserBlockState(self, name, isBlock):
		whisper = self.__GetWhisper(name)
		if not whisper or not whisper["dialog"]:
			return

		whisper["dialog"].RefreshIgnoreButton(isBlock)

	def __InitWhisper(self):
		#chat.InitWhisper(self)

		player_name = player.GetMainCharacterName()
		if constInfo.WHISPER_DICT.has_key(player_name):
			for target_name, whisper_data in constInfo.WHISPER_DICT[player_name].items():
				if self.__FindWhisperButton(target_name) == 0:
					self.__MakeWhisperButton(target_name, whisper_data["mode"])

					for msg_data in whisper_data["history"]:
						mode = msg_data[0]
						msg = msg_data[1]
						chat.AppendWhisper(mode, target_name, msg)

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

	def RegisterTemporaryWhisperDialog(self, name):
		if not self.dlgWhisperWithoutTarget:
			return

		btn = self.__FindWhisperButton(name)
		if 0 != btn:
			self.__DestroyWhisperButton(btn)

		self.__DestroyWhisper(name)
		self.__AddNewWhisper(name, self.dlgWhisperWithoutTarget)
		self.dlgWhisperWithoutTarget.OpenWithTarget(name)
		self.dlgWhisperWithoutTarget = None
		self.__CheckGameMaster(name)

	def OpenWhisperDialog(self, name, targetEmpire=0):
		whisper = self.__GetWhisper(name)
		if not whisper or not whisper["dialog"]:
			dlg = self.__MakeWhisperDialog(name)
			dlg.OpenWithTarget(name, targetEmpire)
			dlg.chatLine.SetFocus()
			dlg.Show()

			self.__CheckGameMaster(name)
			btn = self.__FindWhisperButton(name)
			if 0 != btn:
				self.__DestroyWhisperButton(btn)

	def PushWhisperMessageStack(self, name, mode, text):
		if len(text) <= 0:
			return

		data = self.__GetWhisper(name)
		if not data:
			return

		sentence_stack = data["history"]

		LAST_SENTENCE_STACK_SIZE = 12
		if len(sentence_stack) > LAST_SENTENCE_STACK_SIZE:
			sentence_stack.pop(0)

		sentence_stack.append([mode, text])
		constInfo.WHISPER_DICT[player.GetMainCharacterName()][name] = data

	def RecvWhisper(self, name, mode=chat.WHISPER_TYPE_NORMAL):
		whisper = self.__GetWhisper(name)
		if whisper and whisper["dialog"]:
			if self.IsGameMasterName(name):
				whisper["dialog"].SetGameMasterLook()
		else:
			btn = self.__FindWhisperButton(name)
			if 0 == btn:
				btn = self.__MakeWhisperButton(name, mode)
				chat.AppendChat(chat.CHAT_TYPE_NOTICE, localeInfo.RECEIVE_MESSAGE % (name))

			btn.Flash()

	def ShowWhisperDialog(self, btn):
		try:
			dlgWhisper = self.__MakeWhisperDialog(btn.name)
			dlgWhisper.OpenWithTarget(btn.name)
			dlgWhisper.Show()
			self.__CheckGameMaster(btn.name)
		except:
			import dbg
			dbg.TraceError("interface.ShowWhisperDialog - Failed to find key")

		self.__DestroyWhisperButton(btn)

	def MinimizeWhisperDialog(self, name):
		if 0 != name:
			whisper = self.__GetWhisper(name)
			mode = chat.WHISPER_TYPE_NORMAL
			if whisper:
				mode = whisper["mode"]
			self.__MakeWhisperButton(name, mode)

		return self.CloseWhisperDialog(name, False)

	def CloseWhisperDialog(self, name, destroy_data=True):
		if 0 == name:
			if self.dlgWhisperWithoutTarget:
				self.dlgWhisperWithoutTarget.Destroy()
				self.dlgWhisperWithoutTarget = None
			return True

		try:
			self.__DestroyWhisper(name, destroy_data)
			return True
		except:
			import dbg
			dbg.TraceError("interface.CloseWhisperDialog - Failed to find key")

		return False

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

	def __FindWhisperButton(self, name):
		for button in self.whisperButtonList:
			if button.name == name:
				return button

		return 0

	def __GetWhisper(self, target_name):
		player_name = player.GetMainCharacterName()
		if not constInfo.WHISPER_DICT.has_key(player_name):
			return None

		if not constInfo.WHISPER_DICT[player_name].has_key(target_name):
			return None

		return constInfo.WHISPER_DICT[player_name][target_name]

	def __DestroyWhisper(self, target_name, destroy_data=True):
		whisper = self.__GetWhisper(target_name)
		if whisper and whisper["dialog"]:
			player_name = player.GetMainCharacterName()
			constInfo.WHISPER_DICT[player_name][target_name]["dialog"].Destroy()
			if destroy_data:
				constInfo.WHISPER_DICT[player_name].pop(target_name)

	def __AddNewWhisper(self, target_name, dialog, mode=0):
		player_name = player.GetMainCharacterName()
		if not constInfo.WHISPER_DICT.has_key(player_name):
			constInfo.WHISPER_DICT[player_name] = {}

		data = constInfo.WHISPER_DICT[player_name]
		if not data.has_key(target_name):
			data[target_name] = {
				"dialog": dialog,
				"message_stack": [],
				"history": [],
				"mode": mode,
			}
		else:
			data[target_name]["dialog"] = dialog

		constInfo.WHISPER_DICT[player_name] = data

	def __MakeWhisperDialog(self, name):
		dlgWhisper = uiWhisper.WhisperDialog(self.MinimizeWhisperDialog, self.CloseWhisperDialog)
		dlgWhisper.BindInterface(self)
		dlgWhisper.LoadDialog()
		dlgWhisper.SetPosition(self.windowOpenPosition * 30, self.windowOpenPosition * 30)
		self.__AddNewWhisper(name, dlgWhisper)

		self.windowOpenPosition = (self.windowOpenPosition + 1) % 5

		return dlgWhisper

	def __MakeWhisperButton(self, name, mode=chat.WHISPER_TYPE_NORMAL):
		whisperButton = uiWhisper.WhisperButton()

		img_by_type_dict = {
			chat.WHISPER_TYPE_NORMAL: (None, "d:/ymir work/ui/game/windows/btn_mail_up.sub"),
			chat.WHISPER_TYPE_GM: (0xffffa200, flamewindPath.GetPublic("whisper_gm")),
			11: (0xffffa200, flamewindPath.GetPublic("whisper_gm")),
			12: (0xffffa200, flamewindPath.GetPublic("whisper_gm")),
			13: (0xffffa200, flamewindPath.GetPublic("whisper_gm")),
			14: (0xffffa200, flamewindPath.GetPublic("whisper_gm")),
			15: (0xffffa200, flamewindPath.GetPublic("whisper_gm")),
			chat.WHISPER_TYPE_MYSHOP: (0xffffc2d2, flamewindPath.GetPublic("whisper_shop")),
		}

		if not img_by_type_dict.has_key(mode):
			mode = chat.WHISPER_TYPE_NORMAL
		(color, imgPath) = img_by_type_dict[mode]
		whisperButton.SetUpVisual(imgPath)
		whisperButton.SetOverVisual(imgPath)
		whisperButton.SetDownVisual(imgPath)
		if color:
			whisperButton.SetToolTipTextWithColor(name, color)
		else:
			whisperButton.SetToolTipText(name)

		self.__AddNewWhisper(name, None, mode)

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

	def HideAllWhisperButton(self):
		for btn in self.whisperButtonList:
			btn.Hide()

	def ShowAllWhisperButton(self):
		for btn in self.whisperButtonList:
			btn.Show()

	def __CheckGameMaster(self, name):
		if not self.listGMName.has_key(name):
			return
		whisper = self.__GetWhisper(name)
		if whisper:
			whisper["dialog"].SetGameMasterLook()

	def RegisterGameMasterName(self, name):
		if self.listGMName.has_key(name):
			return

		print "RegisterGameMasterName", name
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

	def EmptyFunction(self):
		pass

	## GAME MASTER CAPTCHA ##
	def GetGameMasterCaptchaDialog(self, pid):
		if self.gameMaster_CaptchaDialogs.has_key(pid):
			return self.gameMaster_CaptchaDialogs[pid]
		return None

	def CreateGameMasterCaptchaDialog(self, pid):
		if self.gameMaster_CaptchaDialogs.has_key(pid):
			oldDlg = self.gameMaster_CaptchaDialogs[pid]
			if oldDlg:
				oldDlg.Destroy()
				oldDlg.Hide()

		dlg = uiCaptcha.GameMasterCaptchaDialog()
		self.gameMaster_CaptchaDialogs[pid] = dlg
		return dlg

