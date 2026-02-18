# coding: latin_1

import net
import app
import ui
import uiOption

import uiSystemOption
import uiGameOption
import uiScriptLocale
import networkModule
import constInfo
import localeInfo
import background
import chat
if app.ENABLE_NEW_GAMEOPTION:
	import uiGameOptionNew
if app.ENABLE_GAME_OPTION_ESCAPE:
	import uiToolTip, wndMgr

SYSTEM_MENU_FOR_PORTAL = False

###################################################################################################
## System
class SystemDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.tooltip = uiToolTip.ToolTip()
			
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		if app.ENABLE_GAME_OPTION_ESCAPE:
			del self.tooltip

	def __Initialize(self):
		self.eventOpenHelpWindow = None
		self.systemOptionDlg = None
		self.gameOptionDlg = None
		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.interface = None
			self.is_show_tooltip = False


	def LoadDialog(self):
		if SYSTEM_MENU_FOR_PORTAL:
			self.__LoadSystemMenu_ForPortal()
		else:
			self.__LoadSystemMenu_Default()

		if app.ENABLE_NEW_GAMEOPTION:
			# When load game loading gameoption for settings.
			if self.gameOptionDlg == None:
				self.gameOptionDlg = uiGameOptionNew.GameOptionWindow()

	def __LoadSystemMenu_Default(self):
		pyScrLoader = ui.PythonScriptLoader()
		if constInfo.IN_GAME_SHOP_ENABLE:
			pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "SystemDialog.py")
		else:
			pyScrLoader.LoadScriptFile(self, "uiscript/systemdialog.py")

		# self.GetChild("system_option_button").SAFE_SetEvent(self.__ClickSystemOptionButton)
		self.GetChild("game_option_button").SAFE_SetEvent(self.__ClickGameOptionButton)
		self.GetChild("change_button").SAFE_SetEvent(self.__ClickChangeCharacterButton)
		self.GetChild("logout_button").SAFE_SetEvent(self.__ClickLogOutButton)
		self.GetChild("exit_button").SAFE_SetEvent(self.__ClickExitButton)
		self.GetChild("help_button").SAFE_SetEvent(self.__ClickHelpButton)
		self.GetChild("cancel_button").SAFE_SetEvent(self.Close)

		if app.ENABLE_CHANNEL_CHANGE:
			self.GetChild("change_channel_button").SAFE_SetEvent(self.__ClickChangeChannel)

		if constInfo.IN_GAME_SHOP_ENABLE:
			self.GetChild("mall_button").SAFE_SetEvent(self.__ClickInGameShopButton)

		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.GetChild("escape_button").SAFE_SetEvent(self.__ClickEscapeButton)
			self.GetChild("escape_button").ShowToolTip = lambda arg = localeInfo.OPTION_ESCAPE_SYSTEM_BUTTON_TOOLTIP : self.OverInToolTipButton(arg)
			self.GetChild("escape_button").HideToolTip = lambda : self.OverOutToolTipButton()

	def __LoadSystemMenu_ForPortal(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/systemdialog_forportal.py")

		# self.GetChild("system_option_button").SAFE_SetEvent(self.__ClickSystemOptionButton)
		self.GetChild("game_option_button").SAFE_SetEvent(self.__ClickGameOptionButton)
		self.GetChild("change_button").SAFE_SetEvent(self.__ClickChangeCharacterButton)
		self.GetChild("exit_button").SAFE_SetEvent(self.__ClickExitButton)
		self.GetChild("help_button").SAFE_SetEvent(self.__ClickHelpButton)
		self.GetChild("cancel_button").SAFE_SetEvent(self.Close)

	if app.ENABLE_GAME_OPTION_ESCAPE:
		def BindInterface(self, interface):
			from _weakref import proxy
			self.interface = proxy(interface)

		def __ClickEscapeButton(self):
			self.Close()

			if self.interface and not self.interface.IsEscapeQuestionPopupOpen():
				self.interface.OpenEscapeQuestionPopup()

		def ToolTipProgress(self):
			if self.is_show_tooltip:
				pos_x, pos_y = wndMgr.GetMousePosition()
				self.tooltip.SetToolTipPosition(pos_x + 50, pos_y + 50)

		def OverInToolTipButton(self, arg):
			arglen = len(str(arg))
			pos_x, pos_y = wndMgr.GetMousePosition()

			self.tooltip.ClearToolTip()
			self.tooltip.SetThinBoardSize(11 * arglen)
			self.tooltip.SetToolTipPosition(pos_x + 50, pos_y + 50)
			self.tooltip.AppendTextLine(arg, 0xffffffff)
			self.tooltip.Show()
			self.is_show_tooltip = True

		def OverOutToolTipButton(self):
			self.tooltip.Hide()
			self.is_show_tooltip = False

		def OnUpdate(self):
			self.ToolTipProgress()

	def Destroy(self):
		self.ClearDictionary()

		if self.gameOptionDlg:
			self.gameOptionDlg.Destroy(True)

		if self.systemOptionDlg:
			self.systemOptionDlg.Destroy()

		self.__Initialize()

	if app.ENABLE_CHANNEL_CHANGE:
		def __ClickChangeChannel(self):
			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				interface.OpenChangeChannel()
			self.Close()

	def SetOpenHelpWindowEvent(self, event):
		self.eventOpenHelpWindow = event

	def OpenDialog(self):
		self.Show()

	def __ClickChangeCharacterButton(self):
		self.Close()

		net.ExitGame()

	def __OnClosePopupDialog(self):
		self.popup = None

	def __ClickLogOutButton(self):
		if SYSTEM_MENU_FOR_PORTAL:
			if app.loggined:
				self.Close()
				net.ExitApplication()
			else:
				self.Close()
				net.LogOutGame()
		else:
			self.Close()
			net.LogOutGame()


	def __ClickExitButton(self):
		self.Close()
		net.ExitApplication()

	def __ClickSystemOptionButton(self):
		self.Close()

		if not self.systemOptionDlg:
			self.systemOptionDlg = uiSystemOption.OptionDialog()

		self.systemOptionDlg.Show()

	def __ClickGameOptionButton(self):
		self.Close()

		if not self.gameOptionDlg:
			if app.ENABLE_NEW_GAMEOPTION:
				self.gameOptionDlg = uiGameOptionNew.GameOptionWindow()
			else:
				self.gameOptionDlg = uiGameOption.OptionDialog()

		self.gameOptionDlg.Show()


	def __ClickHelpButton(self):
		self.Close()

		if None != self.eventOpenHelpWindow:
			self.eventOpenHelpWindow()

	def __ClickInGameShopButton(self):
		self.Close()
		if app.ENABLE_ITEMSHOP:
			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				interface.OpenItemShopWindow()
		else:
			import event
			constInfo.ITEMSHOP["questCMD"] = 'LOAD#'+str(constInfo.ITEMSHOP['tableUpdate'])
			event.QuestButtonClick(int(constInfo.ITEMSHOP["qid"]))
		#net.SendChatPacket("/in_game_mall")

	def Close(self):
		self.Hide()
		return True

	def RefreshMobile(self):
		if self.gameOptionDlg:
			self.gameOptionDlg.RefreshMobile()
		#self.optionDialog.RefreshMobile()

	def OnMobileAuthority(self):
		if self.gameOptionDlg:
			self.gameOptionDlg.OnMobileAuthority()
		#self.optionDialog.OnMobileAuthority()

	def OnBlockMode(self, mode):
		uiGameOption.blockMode = mode
		if self.gameOptionDlg:
			self.gameOptionDlg.OnBlockMode(mode)
		#self.optionDialog.OnBlockMode(mode)

	def OnChangePKMode(self):
		if self.gameOptionDlg:
			self.gameOptionDlg.OnChangePKMode()
		#self.optionDialog.OnChangePKMode()
		
	if app.ENABLE_AUTOMATIC_PICK_UP_SYSTEM:
		def OnChangePickUPMode(self):
			if self.gameOptionDlg:
				self.gameOptionDlg.OnChangePickUPMode()

	def OnPressExitKey(self):
		self.Close()
		return True

	def OnPressEscapeKey(self):
		self.Close()
		return True

class MoveChannelDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.Destroy()
		self.__LoadDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.ParentBoard = 0
		self.ChildBoard = 0
		self.ChannelList = 0
		self.DlgWidht = 0
		self.channel = 0
		self.BlackBoardHeight = 0
		self.DlgHeight = 0
		self.AcceptBtn = 0
		self.CloseBtn = 0
		self.ChangeChannelNumber = 0

	def __LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/MoveChannelDialog.py")
		except:
			import exception
			exception.Abort("MoveChannelDialog.__LoadDialog")

		self.ParentBoard = self.GetChild("MoveChannelBoard")
		self.ChildBoard = self.GetChild("BlackBoard")
		self.GetChild("MoveChannelTitle").SetCloseEvent(ui.__mem_func__(self.Close))

		self.ChannelList = []
		cnt = 4

		self.DlgWidht = 190
		self.BlackBoardHeight = 23*cnt + 5*(cnt-1) + 13
		self.DlgHeight = self.BlackBoardHeight + 75

		self.AcceptBtn = ui.MakeButton(self.ParentBoard, 13, self.DlgHeight - 33, "", "d:/ymir work/ui/public/", "middle_button_01.sub", "middle_button_02.sub", "middle_button_03.sub")
		self.AcceptBtn.SetText( localeInfo.MOVE_CHANNEL_SELECT )
		self.AcceptBtn.SetEvent(ui.__mem_func__(self.AcceptButton))
		self.CloseBtn = ui.MakeButton(self.ParentBoard, self.DlgWidht - 73, self.DlgHeight - 33, "", "d:/ymir work/ui/public/", "middle_button_01.sub", "middle_button_02.sub", "middle_button_03.sub")
		self.CloseBtn.SetText( localeInfo.MOVE_CHANNEL_CANCEL )
		self.CloseBtn.SetEvent(ui.__mem_func__(self.Close))

		for i in xrange(cnt):
			btn = ui.MakeButton(self.ChildBoard, 8, 6 + i*28, "", "d:/ymir work/ui/game/myshop_deco/", "select_btn_01.sub", "select_btn_02.sub", "select_btn_03.sub")
			btn.SetText("CH %d"%(i+1))
			btn.SetEvent(ui.__mem_func__(self.__SelectChannel), i+1)
			self.ChannelList.append(btn)

		self.ParentBoard.SetSize(self.DlgWidht, self.DlgHeight)
		self.ChildBoard.SetSize(self.DlgWidht - 26, self.BlackBoardHeight)
		self.SetSize(self.DlgWidht, self.DlgHeight)

		self.UpdateRect()

	def __SelectChannel(self, idx):
		self.ChangeChannelNumber = idx

		if self.ChangeChannelNumber >= 8:
			return
		for btn in self.ChannelList:
			btn.SetUp()
			btn.Enable()

		self.ChannelList[idx-1].Down()
		self.ChannelList[idx-1].Disable()

	def AcceptButton(self):
		#if self.CheckIsBlockMap(): # already have in gamesrc.
		#	return

		if self.ChangeChannelNumber == self.StartChannelNumber:
			return

		net.SendChatPacket("/move_channel %d" % self.ChangeChannelNumber)
		self.__SaveChannelInfo()
		self.StartChannelNumber = self.ChangeChannelNumber
		self.Close()

	def Show(self):
		ui.ScriptWindow.Show(self)

		self.StartChannelNumber = self.GetChannelNumber()
		self.__SelectChannel(self.StartChannelNumber)

	def Close(self):
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True
	
	def GetChannelNumber(self):
		return self.channel

	def CheckIsBlockMap(self):
		blockmapList = ["metin2_map_a1", "metin2_map_deviltower1", "metin2_map_b1","metin2_map_c1","metin2_map_wedding_01","metin2_map_oxevent","metin2_map_t3","metin2_map_t4","metin2_map_sungzi","metin2_map_sungzi_flame_hill_01","metin2_map_sungzi_flame_hill_02","metin2_map_sungzi_flame_hill_03","metin2_map_sungzi_snow","metin2_map_sungzi_snow_pass01","metin2_map_sungzi_snow_pass02","metin2_map_sungzi_snow_pass03","metin2_map_sungzi_desert_01","metin2_map_sungzi_desert_hill_01","metin2_map_sungzi_desert_hill_02","metin2_map_sungzi_desert_hill_03","metin2_map_empirewar01","metin2_map_empirewar02","metin2_map_empirewar03","metin2_map_guild_02","metin2_map_guild_03","metin2_map_kingdoomswar","metin2_map_duel"]
		if background.GetCurrentMapName() in blockmapList:
			#chat.AppendChat(1, localeInfo.CHANNEL_CHANGE_BLOCK)
			return True
		return False

	def IsShowWindow(self):
		return self.IsShow()

	def __SaveChannelInfo(self):
		selServerID, selChannelID = self.__LoadChannelInfo()
		try:
			file = open("channel.inf", "w+")
			file.write("%d %d" % (loadServerID, self.ChangeChannelNumber - 1))
			file.close()
		except:
			return

	def __LoadChannelInfo(self):
		try:
			file = open("channel.inf")
			lines = file.readlines()
			file.close()
			if len(lines) > 0:
				tokens = lines[0].split()
				selServerID=int(tokens[0])
				selChannelID=int(tokens[1])

				return selServerID, selChannelID
		except:
			return 1, 0
			
		return 1, 0
	
if __name__ == "__main__":

	import app
	import wndMgr
	import systemSetting
	import mouseModule
	import grp
	import ui
	import chr
	import background
	import player

	#wndMgr.SetOutlineFlag(True)

	app.SetMouseHandler(mouseModule.mouseController)
	app.SetHairColorEnable(True)
	wndMgr.SetMouseHandler(mouseModule.mouseController)
	wndMgr.SetScreenSize(systemSetting.GetWidth(), systemSetting.GetHeight())
	app.Create("METIN2 CLOSED BETA", systemSetting.GetWidth(), systemSetting.GetHeight(), 1)
	mouseModule.mouseController.Create()


	wnd = SystemDialog()
	wnd.LoadDialog()
	wnd.Show()

	app.Loop()

