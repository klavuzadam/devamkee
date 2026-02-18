# coding: latin_1

import ui
import snd
import systemSetting
import net
import chat
import app
import localeInfo
import constInfo
import chrmgr
import player
#import uiPrivateShopBuilder # ±èÁØÈ£
import interfaceModule # ±èÁØÈ£
import background

from collections import OrderedDict

blockMode = 0
viewChatMode = 0

MOBILE = False

if localeInfo.IsYMIR():
	MOBILE = True


class OptionDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()
		self.RefreshViewChat()
		self.RefreshAlwaysShowName()
		self.RefreshShowDamage()
		self.RefreshShowSalesText()
		self.RefreshOutline()
		if app.ENABLE_AUTOMATIC_PICK_UP_SYSTEM:
			self.__RefreshPickUP()

		if app.WJ_SHOW_MOB_INFO:
			self.RefreshShowMobInfo()

		self.RefreshShadow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE GAME OPTION DIALOG"

	def __Initialize(self):
		self.titleBar = 0
		self.nameColorModeButtonList = []
		self.viewTargetBoardButtonList = []
		self.pvpModeButtonDict = {}
		self.blockButtonList = []

		if app.ENABLE_HIDE_OBJECTS:
			self.VisibleButtonList = []
			self.mylist = [background.PART_TREE, background.PART_CLOUD]

		self.viewChatButtonList = []
		self.alwaysShowNameButtonList = []
		self.showDamageButtonList = []
		self.showsalesTextButtonList = []
		self.showSnowButtonList = []
		self.showOutlineButtonList = []
		if app.ENABLE_AUTOMATIC_PICK_UP_SYSTEM:
			self.mainPickUPButtonList = []

		if app.WJ_SHOW_MOB_INFO:
			self.showMobInfoButtonList = []

	def Destroy(self):
		self.ClearDictionary()

		self.__Initialize()
		print " -------------------------------------- DESTROY GAME OPTION DIALOG"

	def __Load_LoadScript(self, fileName):
		try:
			pyScriptLoader = ui.PythonScriptLoader()
			pyScriptLoader.LoadScriptFile(self, fileName)
		except:
			import exception
			exception.Abort("OptionDialog.__Load_LoadScript")

	def __Load_BindObject(self):
		try:
			GetObject = self.GetChild
			self.titleBar = GetObject("titlebar")
			self.nameColorModeButtonList.append(GetObject("name_color_normal"))
			self.nameColorModeButtonList.append(GetObject("name_color_empire"))
			self.viewTargetBoardButtonList.append(GetObject("target_board_no_view"))
			self.viewTargetBoardButtonList.append(GetObject("target_board_view"))
			self.pvpModeButtonDict[player.PK_MODE_PEACE] = GetObject("pvp_peace")
			self.pvpModeButtonDict[player.PK_MODE_REVENGE] = GetObject("pvp_revenge")
			self.pvpModeButtonDict[player.PK_MODE_GUILD] = GetObject("pvp_guild")
			self.pvpModeButtonDict[player.PK_MODE_FREE] = GetObject("pvp_free")
			self.blockButtonList.append(GetObject("block_exchange_button"))
			self.blockButtonList.append(GetObject("block_party_button"))
			self.blockButtonList.append(GetObject("block_guild_button"))
			self.blockButtonList.append(GetObject("block_whisper_button"))
			self.blockButtonList.append(GetObject("block_friend_button"))
			self.blockButtonList.append(GetObject("block_party_request_button"))
			self.viewChatButtonList.append(GetObject("view_chat_on_button"))
			self.viewChatButtonList.append(GetObject("view_chat_off_button"))
			self.alwaysShowNameButtonList.append(GetObject("always_show_name_on_button"))
			self.alwaysShowNameButtonList.append(GetObject("always_show_name_off_button"))
			self.showDamageButtonList.append(GetObject("show_damage_on_button"))
			self.showDamageButtonList.append(GetObject("show_damage_off_button"))
			self.showsalesTextButtonList.append(GetObject("salestext_on_button"))
			self.showsalesTextButtonList.append(GetObject("salestext_off_button"))
			self.showOutlineButtonList.append(GetObject("outline_on_button"))
			self.showOutlineButtonList.append(GetObject("outline_off_button"))
			if app.ENABLE_AUTOMATIC_PICK_UP_SYSTEM:
				self.mainPickUPButtonList.append(GetObject("pick_up_button_activate"))
				self.mainPickUPButtonList.append(GetObject("pick_up_button_deactivate"))
				self.mainPickUPButtonList.append(GetObject("pick_up_weapons"))
				self.mainPickUPButtonList.append(GetObject("pick_up_armors"))
				self.mainPickUPButtonList.append(GetObject("pick_up_accessories"))
				self.mainPickUPButtonList.append(GetObject("pick_up_stones"))
				self.mainPickUPButtonList.append(GetObject("pick_up_books"))
				self.mainPickUPButtonList.append(GetObject("pick_up_blacksmith"))
				self.mainPickUPButtonList.append(GetObject("pick_up_gold"))

			if app.ENABLE_HIDE_OBJECTS:
				for bl in xrange(len(self.mylist)):
					self.VisibleButtonList.append(GetObject("vsble" + str(bl)))

			self.salestext_range = GetObject("salestext_range")
			self.salestext_range.Hide()
			self.ctrlShopNamesRange = GetObject("salestext_range_controller")
			self.ctrlShopNamesRange.Hide()
			if app.WJ_SHOW_MOB_INFO:
				self.showMobInfoButtonList.append(GetObject("show_mob_level_button"))
				self.showMobInfoButtonList.append(GetObject("show_mob_AI_flag_button"))

			if systemSetting.GetDamageScale() < 0.15:
				systemSetting.SetDamageScale(0.15)
			ctrlDamageScale = GetObject("damage_scale_controller")
			ctrlDamageScale.SetSliderPos(systemSetting.GetDamageScale() * (1.0 / 2.5))
			ctrlDamageScale.SetEvent(ui.__mem_func__(self.OnChangeDamageScale))
			self.ctrlDamageScale = ctrlDamageScale

			stoneScale = GetObject("stone_scale_controller")
			stoneScale.SetSliderPos(float(systemSetting.GetStoneScale()) / 2.0)
			stoneScale.SetEvent(ui.__mem_func__(self.OnChangeStoneScale))
			self.stoneScale = stoneScale

			global MOBILE
			if MOBILE:
				self.inputMobileButton = GetObject("input_mobile_button")
				self.deleteMobileButton = GetObject("delete_mobile_button")

		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		global MOBILE
		if MOBILE:
			self.__Load_LoadScript("uiscript/gameoptiondialog_formobile.py")
		else:
			self.__Load_LoadScript("uiscript/gameoptiondialog.py")

		self.__Load_BindObject()

		self.SetCenterPosition()

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.nameColorModeButtonList[0].SAFE_SetEvent(self.__OnClickNameColorModeNormalButton)
		self.nameColorModeButtonList[1].SAFE_SetEvent(self.__OnClickNameColorModeEmpireButton)

		self.viewTargetBoardButtonList[0].SAFE_SetEvent(self.__OnClickTargetBoardViewButton)
		self.viewTargetBoardButtonList[1].SAFE_SetEvent(self.__OnClickTargetBoardNoViewButton)

		self.pvpModeButtonDict[player.PK_MODE_PEACE].SAFE_SetEvent(self.__OnClickPvPModePeaceButton)
		self.pvpModeButtonDict[player.PK_MODE_REVENGE].SAFE_SetEvent(self.__OnClickPvPModeRevengeButton)
		self.pvpModeButtonDict[player.PK_MODE_GUILD].SAFE_SetEvent(self.__OnClickPvPModeGuildButton)
		self.pvpModeButtonDict[player.PK_MODE_FREE].SAFE_SetEvent(self.__OnClickPvPModeFreeButton)

		self.blockButtonList[0].SetToggleUpEvent(self.__OnClickBlockExchangeButton)
		self.blockButtonList[1].SetToggleUpEvent(self.__OnClickBlockPartyButton)
		self.blockButtonList[2].SetToggleUpEvent(self.__OnClickBlockGuildButton)
		self.blockButtonList[3].SetToggleUpEvent(self.__OnClickBlockWhisperButton)
		self.blockButtonList[4].SetToggleUpEvent(self.__OnClickBlockFriendButton)
		self.blockButtonList[5].SetToggleUpEvent(self.__OnClickBlockPartyRequest)

		self.blockButtonList[0].SetToggleDownEvent(self.__OnClickBlockExchangeButton)
		self.blockButtonList[1].SetToggleDownEvent(self.__OnClickBlockPartyButton)
		self.blockButtonList[2].SetToggleDownEvent(self.__OnClickBlockGuildButton)
		self.blockButtonList[3].SetToggleDownEvent(self.__OnClickBlockWhisperButton)
		self.blockButtonList[4].SetToggleDownEvent(self.__OnClickBlockFriendButton)
		self.blockButtonList[5].SetToggleDownEvent(self.__OnClickBlockPartyRequest)

		self.viewChatButtonList[0].SAFE_SetEvent(self.__OnClickViewChatOnButton)
		self.viewChatButtonList[1].SAFE_SetEvent(self.__OnClickViewChatOffButton)

		self.alwaysShowNameButtonList[0].SAFE_SetEvent(self.__OnClickAlwaysShowNameOnButton)
		self.alwaysShowNameButtonList[1].SAFE_SetEvent(self.__OnClickAlwaysShowNameOffButton)

		self.showDamageButtonList[0].SAFE_SetEvent(self.__OnClickShowDamageOnButton)
		self.showDamageButtonList[1].SAFE_SetEvent(self.__OnClickShowDamageOffButton)

		self.showsalesTextButtonList[0].SAFE_SetEvent(self.__OnClickSalesTextOnButton)
		self.showsalesTextButtonList[1].SAFE_SetEvent(self.__OnClickSalesTextOffButton)

		self.showOutlineButtonList[0].SAFE_SetEvent(self.__OnClickShowOutlineButton)
		self.showOutlineButtonList[1].SAFE_SetEvent(self.__OnClickShowOutlineButton)

		if app.ENABLE_AUTOMATIC_PICK_UP_SYSTEM:
			self.mainPickUPButtonList[0].SetToggleUpEvent(self.__OnClicActivatePickUpButton)
			self.mainPickUPButtonList[0].SetToggleDownEvent(self.__OnClicActivatePickUpButton)
			self.mainPickUPButtonList[1].SetToggleUpEvent(self.__OnClicDeactivatePickUpButton)
			self.mainPickUPButtonList[1].SetToggleDownEvent(self.__OnClicDeactivatePickUpButton)
			self.mainPickUPButtonList[2].SetToggleUpEvent(self.__OnClickWeaponPickUpButton)
			self.mainPickUPButtonList[2].SetToggleDownEvent(self.__OnClickWeaponPickUpButton)
			self.mainPickUPButtonList[3].SetToggleUpEvent(self.__OnClickArmorPickUpButton)
			self.mainPickUPButtonList[3].SetToggleDownEvent(self.__OnClickArmorPickUpButton)
			self.mainPickUPButtonList[4].SetToggleUpEvent(self.__OnClickAccessoryPickUpButton)
			self.mainPickUPButtonList[4].SetToggleDownEvent(self.__OnClickAccessoryPickUpButton)
			self.mainPickUPButtonList[5].SetToggleUpEvent(self.__OnClickStonePickUpButton)
			self.mainPickUPButtonList[5].SetToggleDownEvent(self.__OnClickStonePickUpButton)
			self.mainPickUPButtonList[6].SetToggleUpEvent(self.__OnClickBookPickUpButton)
			self.mainPickUPButtonList[6].SetToggleDownEvent(self.__OnClickBookPickUpButton)
			self.mainPickUPButtonList[7].SetToggleUpEvent(self.__OnClickBlacksmithPickUpButton)
			self.mainPickUPButtonList[7].SetToggleDownEvent(self.__OnClickBlacksmithPickUpButton)
			self.mainPickUPButtonList[8].SetToggleUpEvent(self.__OnClickGoldPickUpButton)
			self.mainPickUPButtonList[8].SetToggleDownEvent(self.__OnClickGoldPickUpButton)

		if app.ENABLE_HIDE_OBJECTS:
			for bl in xrange(len(self.mylist)):
				self.VisibleButtonList[bl].SetToggleUpEvent(lambda arg=bl: self.SetVisibleFunc(arg))
				self.VisibleButtonList[bl].SetToggleDownEvent(lambda arg=bl: self.SetVisibleFunc(arg))

		if app.WJ_SHOW_MOB_INFO:
			self.showMobInfoButtonList[0].SetToggleUpEvent(self.__OnClickShowMobLevelButton)
			self.showMobInfoButtonList[0].SetToggleDownEvent(self.__OnClickShowMobLevelButton)
			self.showMobInfoButtonList[1].SetToggleUpEvent(self.__OnClickShowMobAIFlagButton)
			self.showMobInfoButtonList[1].SetToggleDownEvent(self.__OnClickShowMobAIFlagButton)


		self.__ClickRadioButton(self.nameColorModeButtonList, constInfo.GET_CHRNAME_COLOR_INDEX())
		self.__ClickRadioButton(self.viewTargetBoardButtonList, constInfo.GET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD())
		self.__SetPeacePKMode()
		
		if app.ENABLE_IKASHOP_RENEWAL:
			if app.EXTEND_IKASHOP_PRO:
				self.shopRangeViewHeader = ui.TextLine()
				self.shopRangeViewHeader.SetParent(self)
				self.shopRangeViewHeader.SetPosition(30, self.GetHeight() - 10 - 10)
				self.shopRangeViewHeader.SetText(localeInfo.IKASHOP_PRO_SHOP_RANGE_VIEW_HEADER)
				self.shopRangeViewHeader.Show()

				distance = systemSetting.GetShopRangeView()
				pct = (distance - 600.0) / 4400.0
				self.shopFloatBar = ui.SliderBar()
				self.shopFloatBar.SetParent(self)
				self.shopFloatBar.SetPosition(130-39, self.GetHeight()-6 - 10)
				self.shopFloatBar.SetEvent(self._OnSlideShopRangeView)
				self.shopFloatBar.SetSliderPos(pct)
				self.shopFloatBar.Show()

				self.SetSize(self.GetWidth(), self.GetHeight() + 10)
				board = self.GetChild("board")
				board.SetSize(board.GetWidth(), board.GetHeight() + 10)

		#global MOBILE
		if MOBILE:
			self.inputMobileButton.SetEvent(ui.__mem_func__(self.__OnChangeMobilePhoneNumber))
			self.deleteMobileButton.SetEvent(ui.__mem_func__(self.__OnDeleteMobilePhoneNumber))

	if app.ENABLE_IKASHOP_RENEWAL:
		if app.EXTEND_IKASHOP_PRO:
			def _OnSlideShopRangeView(self):
				pos = self.shopFloatBar.GetSliderPos()
				distance = pos * 4400.0 + 600
				systemSetting.SetShopRangeView(distance)

	if app.ENABLE_HIDE_OBJECTS:
		def SetVisibleFunc(self, index):
			background.SetVisiblePart(self.mylist[index], not background.IsVisiblePart(self.mylist[index]))
			if self.mylist[index] == background.PART_SHOP:
				# if not systemSetting.IsShowSalesText():
					# self.__OnClickSalesTextOnButton()
				# else:
					# self.__OnClickSalesTextOffButton()
				if not systemSetting.IsShowSalesText():
					self.__OnClickSalesTextOffButton()
				else:
					self.__OnClickSalesTextOnButton()
			for bl in xrange(len(self.mylist)):
				if background.IsVisiblePart(self.mylist[bl]):
					self.VisibleButtonList[bl].SetUp()
				else:
					self.VisibleButtonList[bl].Down()

	def __ClickRadioButton(self, buttonList, buttonIndex):
		try:
			selButton=buttonList[buttonIndex]
		except IndexError:
			return

		for eachButton in buttonList:
			eachButton.SetUp()

		selButton.Down()

	def __SetNameColorMode(self, index):
		constInfo.SET_CHRNAME_COLOR_INDEX(index)
		self.__ClickRadioButton(self.nameColorModeButtonList, index)

	def __SetTargetBoardViewMode(self, flag):
		constInfo.SET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD(flag)
		self.__ClickRadioButton(self.viewTargetBoardButtonList, flag)

	def __OnClickNameColorModeNormalButton(self):
		self.__SetNameColorMode(0)

	def __OnClickNameColorModeEmpireButton(self):
		self.__SetNameColorMode(1)
		
	def __OnClickShowOutlineButton(self):
		systemSetting.SetShowOutlineFlag(not systemSetting.IsShowOutline())
		self.RefreshOutline()

	def RefreshOutline(self):
		if systemSetting.IsShowOutline():
			self.showOutlineButtonList[0].Down()
			self.showOutlineButtonList[1].SetUp()
		else:
			self.showOutlineButtonList[0].SetUp()
			self.showOutlineButtonList[1].Down()

	def __OnClickTargetBoardViewButton(self):
		self.__SetTargetBoardViewMode(0)

	def __OnClickTargetBoardNoViewButton(self):
		self.__SetTargetBoardViewMode(1)

	def __OnClickCameraModeShortButton(self):
		self.__SetCameraMode(0)

	def __OnClickCameraModeLongButton(self):
		self.__SetCameraMode(1)

	def __OnClickFogModeLevel0Button(self):
		self.__SetFogLevel(0)

	def __OnClickFogModeLevel1Button(self):
		self.__SetFogLevel(1)

	def __OnClickFogModeLevel2Button(self):
		self.__SetFogLevel(2)

	def __OnClickBlockExchangeButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_EXCHANGE))
	def __OnClickBlockPartyButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_PARTY))
	def __OnClickBlockGuildButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_GUILD))
	def __OnClickBlockWhisperButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_WHISPER))
	def __OnClickBlockFriendButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_FRIEND))
	def __OnClickBlockPartyRequest(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_PARTY_REQUEST))

	def __OnClickViewChatOnButton(self):
		global viewChatMode
		viewChatMode = 1
		systemSetting.SetViewChatFlag(viewChatMode)
		self.RefreshViewChat()
	def __OnClickViewChatOffButton(self):
		global viewChatMode
		viewChatMode = 0
		systemSetting.SetViewChatFlag(viewChatMode)
		self.RefreshViewChat()

	def __OnClickAlwaysShowNameOnButton(self):
		systemSetting.SetAlwaysShowNameFlag(True)
		self.RefreshAlwaysShowName()

	def __OnClickAlwaysShowNameOffButton(self):
		systemSetting.SetAlwaysShowNameFlag(False)
		self.RefreshAlwaysShowName()

	def __OnClickShowDamageOnButton(self):
		systemSetting.SetShowDamageFlag(True)
		self.RefreshShowDamage()

	def __OnClickShowDamageOffButton(self):
		systemSetting.SetShowDamageFlag(False)
		self.RefreshShowDamage()

	def __OnClickSalesTextOnButton(self):
		systemSetting.SetShowSalesTextFlag(True)
		self.RefreshShowSalesText()
		uiOfflineShopBuilder.UpdateADBoard()

	if app.ENABLE_AUTOMATIC_PICK_UP_SYSTEM:
		def __OnClicActivatePickUpButton(self):
			self.__RefreshPickUP()
			net.SendChatPacket("/setpickupmode " + str(constInfo.PICKUPMODE ^ player.AUTOMATIC_PICK_UP_ACTIVATE))

		def __OnClicDeactivatePickUpButton(self):
			self.__RefreshPickUP()
			net.SendChatPacket("/setpickupmode " + str(constInfo.PICKUPMODE ^ player.AUTOMATIC_PICK_UP_DEACTIVATE))

		def __OnClickWeaponPickUpButton(self):
			self.__RefreshPickUP()
			net.SendChatPacket("/setpickupmode " + str(constInfo.PICKUPMODE ^ player.AUTOMATIC_PICK_UP_WEAPON))

		def __OnClickArmorPickUpButton(self):
			self.__RefreshPickUP()
			net.SendChatPacket("/setpickupmode " + str(constInfo.PICKUPMODE ^ player.AUTOMATIC_PICK_UP_ARMOR))

		def __OnClickAccessoryPickUpButton(self):
			self.__RefreshPickUP()
			net.SendChatPacket("/setpickupmode " + str(constInfo.PICKUPMODE ^ player.AUTOMATIC_PICK_UP_ACCESSORY))

		def __OnClickStonePickUpButton(self):
			self.__RefreshPickUP()
			net.SendChatPacket("/setpickupmode " + str(constInfo.PICKUPMODE ^ player.AUTOMATIC_PICK_UP_STONE))

		def __OnClickBookPickUpButton(self):
			self.__RefreshPickUP()
			net.SendChatPacket("/setpickupmode " + str(constInfo.PICKUPMODE ^ player.AUTOMATIC_PICK_UP_BOOK))

		def __OnClickBlacksmithPickUpButton(self):
			self.__RefreshPickUP()
			net.SendChatPacket("/setpickupmode " + str(constInfo.PICKUPMODE ^ player.AUTOMATIC_PICK_UP_BLACKSMITH))

		def __OnClickGoldPickUpButton(self):
			self.__RefreshPickUP()
			net.SendChatPacket("/setpickupmode " + str(constInfo.PICKUPMODE ^ player.AUTOMATIC_PICK_UP_GOLD))

	def __OnClickSalesTextOffButton(self):
		systemSetting.SetShowSalesTextFlag(False)
		self.RefreshShowSalesText()

	if app.WJ_SHOW_MOB_INFO:
		def __OnClickShowMobLevelButton(self):
			systemSetting.SetShowMobLevel(not systemSetting.IsShowMobLevel())
			self.RefreshShowMobInfo()
		def __OnClickShowMobAIFlagButton(self):
			systemSetting.SetShowMobAIFlag(not systemSetting.IsShowMobAIFlag())
			self.RefreshShowMobInfo()

	def __CheckPvPProtectedLevelPlayer(self):
		if player.GetStatus(player.LEVEL)<constInfo.PVPMODE_PROTECTED_LEVEL:
			self.__SetPeacePKMode()
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_PROTECT % (constInfo.PVPMODE_PROTECTED_LEVEL))
			return 1

		return 0

	def __SetPKMode(self, mode):
		for btn in self.pvpModeButtonDict.values():
			btn.SetUp()
		if self.pvpModeButtonDict.has_key(mode):
			self.pvpModeButtonDict[mode].Down()

	def __SetPeacePKMode(self):
		self.__SetPKMode(player.PK_MODE_PEACE)

	def __RefreshPVPButtonList(self):
		self.__SetPKMode(player.GetPKMode())

	def __OnClickPvPModePeaceButton(self):
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 0", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def __OnClickPvPModeRevengeButton(self):
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 1", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def __OnClickPvPModeFreeButton(self):
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 2", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def __OnClickPvPModeGuildButton(self):
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if 0 == player.GetGuildID():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_CANNOT_SET_GUILD_MODE)
			return

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 4", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def OnChangePKMode(self):
		self.__RefreshPVPButtonList()

	def __OnChangeMobilePhoneNumber(self):
		global MOBILE
		if not MOBILE:
			return

		import uiCommon
		inputDialog = uiCommon.InputDialog()
		inputDialog.SetTitle(localeInfo.MESSENGER_INPUT_MOBILE_PHONE_NUMBER_TITLE)
		inputDialog.SetMaxLength(13)
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OnInputMobilePhoneNumber))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseInputDialog))
		inputDialog.Open()
		self.inputDialog = inputDialog

	def __OnDeleteMobilePhoneNumber(self):
		global MOBILE
		if not MOBILE:
			return

		import uiCommon
		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.MESSENGER_DO_YOU_DELETE_PHONE_NUMBER)
		questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnDeleteMobile))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog

	def OnInputMobilePhoneNumber(self):
		global MOBILE
		if not MOBILE:
			return

		text = self.inputDialog.GetText()

		if not text:
			return

		text.replace('-', '')
		net.SendChatPacket("/mobile " + text)
		self.OnCloseInputDialog()
		return True

	def OnInputMobileAuthorityCode(self):
		global MOBILE
		if not MOBILE:
			return

		text = self.inputDialog.GetText()
		net.SendChatPacket("/mobile_auth " + text)
		self.OnCloseInputDialog()
		return True

	def OnDeleteMobile(self):
		global MOBILE
		if not MOBILE:
			return

		net.SendChatPacket("/mobile")
		self.OnCloseQuestionDialog()
		return True

	def OnCloseInputDialog(self):
		self.inputDialog.Close()
		self.inputDialog = None
		return True

	def OnCloseQuestionDialog(self):
		self.questionDialog.Close()
		self.questionDialog = None
		return True

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def RefreshMobile(self):
		global MOBILE
		if not MOBILE:
			return

		if player.HasMobilePhoneNumber():
			self.inputMobileButton.Hide()
			self.deleteMobileButton.Show()
		else:
			self.inputMobileButton.Show()
			self.deleteMobileButton.Hide()

	def OnMobileAuthority(self):
		global MOBILE
		if not MOBILE:
			return

		import uiCommon
		inputDialog = uiCommon.InputDialogWithDescription()
		inputDialog.SetTitle(localeInfo.MESSENGER_INPUT_MOBILE_AUTHORITY_TITLE)
		inputDialog.SetDescription(localeInfo.MESSENGER_INPUT_MOBILE_AUTHORITY_DESCRIPTION)
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OnInputMobileAuthorityCode))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseInputDialog))
		inputDialog.SetMaxLength(4)
		inputDialog.SetBoardWidth(310)
		inputDialog.Open()
		self.inputDialog = inputDialog

	if app.ENABLE_AUTOMATIC_PICK_UP_SYSTEM:
		def OnChangePickUPMode(self):
			self.__RefreshPickUP()

		def __RefreshPickUP(self):
			for i in xrange(len(self.mainPickUPButtonList)):
				if 0 != (constInfo.PICKUPMODE & (1 << i)):
					self.mainPickUPButtonList[i].Down()
				else:
					self.mainPickUPButtonList[i].SetUp()
					
	def RefreshBlock(self):
		global blockMode
		for i in xrange(len(self.blockButtonList)):
			if 0 != (blockMode & (1 << i)):
				self.blockButtonList[i].Down()
			else:
				self.blockButtonList[i].SetUp()

	def RefreshViewChat(self):
		if systemSetting.IsViewChat():
			self.viewChatButtonList[0].Down()
			self.viewChatButtonList[1].SetUp()
		else:
			self.viewChatButtonList[0].SetUp()
			self.viewChatButtonList[1].Down()

	def RefreshAlwaysShowName(self):
		if systemSetting.IsAlwaysShowName():
			self.alwaysShowNameButtonList[0].Down()
			self.alwaysShowNameButtonList[1].SetUp()
		else:
			self.alwaysShowNameButtonList[0].SetUp()
			self.alwaysShowNameButtonList[1].Down()

	def RefreshShowDamage(self):
		if systemSetting.IsShowDamage():
			self.showDamageButtonList[0].Down()
			self.showDamageButtonList[1].SetUp()
		else:
			self.showDamageButtonList[0].SetUp()
			self.showDamageButtonList[1].Down()

	def RefreshShowSalesText(self):
		if systemSetting.IsShowSalesText():
			self.showsalesTextButtonList[0].Down()
			self.showsalesTextButtonList[1].SetUp()
		else:
			self.showsalesTextButtonList[0].SetUp()
			self.showsalesTextButtonList[1].Down()

	if app.WJ_SHOW_MOB_INFO:
		def RefreshShowMobInfo(self):
			if systemSetting.IsShowMobLevel():
				self.showMobInfoButtonList[0].Down()
			else:
				self.showMobInfoButtonList[0].SetUp()
			if systemSetting.IsShowMobAIFlag():
				self.showMobInfoButtonList[1].Down()
			else:
				self.showMobInfoButtonList[1].SetUp()

	def RefreshShadow(self):
		systemSetting.SetShadowLevel(int(5))

	def OnBlockMode(self, mode):
		global blockMode
		blockMode = mode
		self.RefreshBlock()

	def Show(self):
		self.RefreshMobile()
		self.RefreshBlock()
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()
