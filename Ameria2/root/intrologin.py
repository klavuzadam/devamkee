import dbg
import app
import net
import ui
import ime
import snd
import wndMgr
import musicInfo
import serverInfo
import systemSetting
import ServerStateChecker
import localeInfo
import constInfo
import uiCommon
import time
import serverCommandParser
import ime
import uiScriptLocale
import config
import os
import _winreg

app.SetGuildMarkPath("mark")
app.SetGuildMarkPath("10.tga")

SPEED_SERVER_IP = "188.212.100.78" #SVTEST
LIVE_SERVER_IP = "188.212.100.78" #TEST IP
# SPEED_SERVER_IP = "94.23.77.86" #live
# LIVE_SERVER_IP = "94.23.77.86" #live
SERVER_IP = SPEED_SERVER_IP if constInfo.SPEED_SERVER else LIVE_SERVER_IP

def GetAuthIP():
	if constInfo.SPEED_SERVER == 1:
		#dbg.TraceError("Using SPEED_SERVER_IP: {}".format(SPEED_SERVER_IP))
		return SPEED_SERVER_IP
	elif constInfo.SPEED_SERVER == 0:
		#dbg.TraceError("Using LIVE_SERVER_IP: {}".format(LIVE_SERVER_IP))
		return LIVE_SERVER_IP

def get_server_ip():
	return SERVER_IP

SERVER_IP = GetAuthIP()
SERVER_INFO = {
	"data" : {
		"channel1"	: [SERVER_IP, 13002],
		# "channel1"	: [SERVER_IP, 13000], #local
		"channel2"	: [SERVER_IP, 13102],
		"channel3"	: [SERVER_IP, 13202],
		"channel4"	: [SERVER_IP, 13302],
		"channel5"	: [SERVER_IP, 13402],
		"auth"		: [SERVER_IP, 39994],#live
		# "auth"		: [SERVER_IP, 11000],#local
		"mark"		: [SERVER_IP, 13002],
	},

	"name" : "Ameria",
}


REG_PATH = r"SOFTWARE\ameriaprj"
ACCOUNTS_NUMBER = 12
ACCOUNTS = [[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED],[uiScriptLocale.ACCOUNT_SAVED, uiScriptLocale.ACCOUNT_SAVED]]

def set_reg(name, value):
	try:
		_winreg.CreateKey(_winreg.HKEY_CURRENT_USER, REG_PATH)
		registry_key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, REG_PATH, 0, _winreg.KEY_WRITE)
		_winreg.SetValueEx(registry_key, name, 0, _winreg.REG_SZ, value)
		_winreg.CloseKey(registry_key)
		return True
	except WindowsError:
		return False

def get_reg(name):
	try:
		registry_key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, REG_PATH, 0, _winreg.KEY_READ)
		value, regtype = _winreg.QueryValueEx(registry_key, name)
		_winreg.CloseKey(registry_key)
		return str(value)
	except WindowsError:
		return None	

def LoadAccounts():
	for i in xrange(ACCOUNTS_NUMBER):
		if get_reg("acc_%d" % i):
			ACCOUNTS[i][0] = get_reg("acc_%d" % i).split("|")[0]
			ACCOUNTS[i][1] = get_reg("acc_%d" % i).split("|")[1]
			
def SaveAccounts():	
	for i in xrange(ACCOUNTS_NUMBER):
		set_reg("acc_%d" % i, "%s|%s" % (ACCOUNTS[i][0], ACCOUNTS[i][1]))															

LOGIN_DELAY_SEC = 0.0

def IsLoginDelay():
	global LOGIN_DELAY_SEC
	if LOGIN_DELAY_SEC > 0.0:
		return True
	else:
		return False

def GetLoginDelay():
	global LOGIN_DELAY_SEC
	return LOGIN_DELAY_SEC

class ConnectingDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()
		self.eventTimeOver = lambda *arg: None
		self.eventExit = lambda *arg: None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/ConnectingDialog.py")

			self.board = self.GetChild("board")
			self.message = self.GetChild("message")
			self.countdownMessage = self.GetChild("countdown_message")

		except:
			import exception
			exception.Abort("ConnectingDialog.LoadDialog.BindObject")

	def Open(self, waitTime):
		curTime = time.clock()
		self.endTime = curTime + waitTime

		self.Lock()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()		

	def Close(self):
		self.Unlock()
		self.Hide()

	def Destroy(self):
		self.Hide()
		self.ClearDictionary()

	def SetText(self, text):
		self.message.SetText(text)

	def SetCountDownMessage(self, waitTime):
		self.countdownMessage.SetText("%.0f%s" % (waitTime, localeInfo.SECOND))

	def SAFE_SetTimeOverEvent(self, event):
		self.eventTimeOver = ui.__mem_func__(event)

	def SAFE_SetExitEvent(self, event):
		self.eventExit = ui.__mem_func__(event)

	def OnUpdate(self):
		lastTime = max(0, self.endTime - time.clock())
		if 0 == lastTime:
			self.Close()
			self.eventTimeOver()
		else:
			self.SetCountDownMessage(self.endTime - time.clock())

	def OnPressExitKey(self):
		return True

class LoginWindow(ui.ScriptWindow):

	IS_TEST = net.IsTest()

	def __init__(self, stream):
		ui.ScriptWindow.__init__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_LOGIN, self)
		net.SetAccountConnectorHandler(self)

		self.lastLoginTime = 0
		self.inputDialog = None
		self.languageButtonDict = {}
		self.connectingDialog = None
		self.stream = stream
		self.isNowCountDown = False

	def __del__(self):
		net.ClearPhaseWindow(net.PHASE_WINDOW_LOGIN, self)
		net.SetAccountConnectorHandler(0)
		ui.ScriptWindow.__del__(self)

	def Open(self):
		ServerStateChecker.Create(self)


		self.loginFailureMsgDict={
			#"DEFAULT" : localeInfo.LOGIN_FAILURE_UNKNOWN,

			"ALREADY"	: localeInfo.LOGIN_FAILURE_ALREAY,
			"NOID"		: localeInfo.LOGIN_FAILURE_NOT_EXIST_ID,
			"WRONGPWD"	: localeInfo.LOGIN_FAILURE_WRONG_PASSWORD,
			"FULL"		: localeInfo.LOGIN_FAILURE_TOO_MANY_USER,
			"SHUTDOWN"	: localeInfo.LOGIN_FAILURE_SHUTDOWN,
			"REPAIR"	: localeInfo.LOGIN_FAILURE_REPAIR_ID,
			"BLOCK"		: localeInfo.LOGIN_FAILURE_BLOCK_ID,
			"BESAMEKE"	: localeInfo.LOGIN_FAILURE_BE_SAME_KEY,
			"NOTAVAIL"	: localeInfo.LOGIN_FAILURE_NOT_AVAIL,
			"NOBILL"	: localeInfo.LOGIN_FAILURE_NOBILL,
			"BLKLOGIN"	: localeInfo.LOGIN_FAILURE_BLOCK_LOGIN,
			"WEBBLK"	: localeInfo.LOGIN_FAILURE_WEB_BLOCK,
			"BADSCLID"	: localeInfo.LOGIN_FAILURE_WRONG_SOCIALID,
			"AGELIMIT"	: localeInfo.LOGIN_FAILURE_SHUTDOWN_TIME,
			"WRONG_HW"	: localeInfo.LOGIN_FAILURE_WRONG_HWID,
			"VERSION"	: "Versiune incorecta. Ruleaza patcher-ul!",
		}

		self.loginFailureFuncDict = {
			"WRONGPWD"	: self.__DisconnectAndInputPassword,
			"QUIT"		: app.Exit,
		}

		self.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
		self.SetWindowName("LoginWindow")

		if not self.__LoadScript("uiscript/LoginWindow.py"):
			dbg.TraceError("LoginWindow.Open - __LoadScript Error")
			return

		if musicInfo.loginMusic != "":
			snd.SetMusicVolume(systemSetting.GetMusicVolume())
			snd.FadeInMusic("BGM/"+musicInfo.loginMusic)

		snd.SetSoundVolume(systemSetting.GetSoundVolume())

		# pevent key "[" "]"
		ime.AddExceptKey(91)
		ime.AddExceptKey(93)

		self.SetChannel(0)
		
		self.Show()
		app.ShowCursor()

	def Close(self):
		if self.connectingDialog:
			self.connectingDialog.Close()
		self.connectingDialog = None

		ServerStateChecker.Initialize(self)

		if musicInfo.loginMusic != "" and musicInfo.selectMusic != "":
			snd.FadeOutMusic("BGM/"+musicInfo.loginMusic)

		self.idEditLine.SetTabEvent(0)
		self.idEditLine.SetReturnEvent(0)
		self.pwdEditLine.SetReturnEvent(0)
		self.pwdEditLine.SetTabEvent(0)

		self.connectBoard = None
		self.loginBoard = None
		self.idEditLine = None
		self.pwdEditLine = None
		self.inputDialog = None
		self.connectingDialog = None
		self.languageButtonDict = {}

		self.KillFocus()
		self.Hide()

		self.stream.popupWindow.Close()
		self.loginFailureFuncDict = {}

		ime.ClearExceptKey()

		app.HideCursor()

	def __ExitGame(self):
		app.Exit()

	def SetIDEditLineFocus(self):
		if self.idEditLine != None:
			self.idEditLine.SetFocus()

	def SetPasswordEditLineFocus(self):
		if localeInfo.IsEUROPE():
			if self.idEditLine != None:
				self.idEditLine.SetText("")
				self.idEditLine.SetFocus()

			if self.pwdEditLine != None:
				self.pwdEditLine.SetText("")
		else:
			if self.pwdEditLine != None:
				self.pwdEditLine.SetFocus()								

	def OnEndCountDown(self):
		self.isNowCountDown = FALSE
		self.timeOutMsg = FALSE
		self.OnConnectFailure()

	def OnConnectFailure(self):

		if self.isNowCountDown:
			return

		snd.PlaySound("sound/ui/loginfail.wav")

		if self.connectingDialog:
			self.connectingDialog.Close()
		self.connectingDialog = None

		self.PopupNotifyMessage(localeInfo.LOGIN_CONNECT_FAILURE, self.SetPasswordEditLineFocus)

	def OnHandShake(self):
		if not IsLoginDelay():
			snd.PlaySound("sound/ui/loginok.wav")
			self.PopupDisplayMessage(localeInfo.LOGIN_CONNECT_SUCCESS)

	def OnLoginStart(self):
		if not IsLoginDelay():
			self.PopupDisplayMessage(localeInfo.LOGIN_PROCESSING)

	def OnLoginFailure(self, error):
		if self.connectingDialog:
			self.connectingDialog.Close()

		self.connectingDialog = None

		if not self.loginFailureFuncDict:
			return

		try:
			loginFailureMsg = self.loginFailureMsgDict[error]
		except KeyError:
			loginFailureMsg = localeInfo.LOGIN_FAILURE_UNKNOWN  + error

		loginFailureFunc=self.loginFailureFuncDict.get(error, self.SetPasswordEditLineFocus)
		self.PopupNotifyMessage(loginFailureMsg, loginFailureFunc)

		snd.PlaySound("sound/ui/loginfail.wav")

	def __DisconnectAndInputID(self):
		if self.connectingDialog:
			self.connectingDialog.Close()
		self.connectingDialog = None

		self.SetIDEditLineFocus()
		net.Disconnect()

	def __DisconnectAndInputPassword(self):
		if self.connectingDialog:
			self.connectingDialog.Close()
		self.connectingDialog = None

		self.SetPasswordEditLineFocus()
		net.Disconnect()

	def __LoadScript(self, fileName):
		import dbg
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, fileName)
		except:
			import exception
			exception.Abort("LoginWindow.__LoadScript.LoadObject")
		try:
			GetObject=self.GetChild
			self.loginBoard				= GetObject("LoginBoard")
			self.idEditLine				= GetObject("ID_EditLine")
			self.pwdEditLine			= GetObject("Password_EditLine")
			self.loginButton			= GetObject("LoginButton")
			self.loginExitButton		= GetObject("LoginExitButton")

			self.forgotButton			= GetObject("ForgotLink")
			self.registerButton			= GetObject("RegisterLink")

			self.accountNameDict = []
			self.accountSlotDict = []
			self.accountDeleteDict = []
			self.accountSaveDict = []
			for i in xrange(1,13):
				self.accountNameDict.append(self.GetChild("SlotLine%s_Text" % i))
				self.accountSlotDict.append(self.GetChild("SlotLine%s_Button" % i))
				self.accountDeleteDict.append(self.GetChild("SlotLine%s_Button_Delete" % i))
				self.accountSaveDict.append(self.GetChild("SlotLine%s_Button_Save" % i))
				
			self.channelTab = []
			self.channelTab.append(self.GetChild("Ch1Button"))
			self.channelTab.append(self.GetChild("Ch2Button"))
			self.channelTab.append(self.GetChild("Ch3Button"))
			self.channelTab.append(self.GetChild("Ch4Button"))
			self.channelTab.append(self.GetChild("Ch5Button"))
			# self.channelTab.append(self.GetChild("Ch6Button"))

			self.channelTab[0].SetEvent(lambda arg=0: self.SetChannel(arg))
			self.channelTab[1].SetEvent(lambda arg=1: self.SetChannel(arg))
			self.channelTab[2].SetEvent(lambda arg=2: self.SetChannel(arg))
			self.channelTab[3].SetEvent(lambda arg=3: self.SetChannel(arg))
			self.channelTab[4].SetEvent(lambda arg=4: self.SetChannel(arg))
			# self.channelTab[5].SetEvent(lambda arg=5: self.SetChannel(arg))
			self.channelIndex = 0
			self.channelTab[0].Down()
			
			for i in xrange(len(self.accountSlotDict)):
				self.accountSlotDict[i].SetEvent(ui.__mem_func__(self.ConnectOnAccount), i)
				
				self.accountDeleteDict[i].Hide()
				self.accountDeleteDict[i].SetEvent(ui.__mem_func__(self.DeleteAccount), i)
				
				self.accountSaveDict[i].Hide()
				self.accountSaveDict[i].SetEvent(ui.__mem_func__(self.SaveAccountSlot), i)
				
			# self.GetChild("AccountBoard").Show()
			
		except:
			import exception
			exception.Abort("LoginWindow.__LoadScript.BindObject")
			
		try:
			self.LoadAccounts()
		except:
			import exception
			exception.Abort("Cannot load account list")

		self.loginButton.SetEvent(ui.__mem_func__(self.__OnClickLoginButton))
		self.loginExitButton.SetEvent(ui.__mem_func__(self.__OnClickExitButton))
		
		self.forgotButton.SetEvent(ui.__mem_func__(self.OpenLink), 5)
		self.registerButton.SetEvent(ui.__mem_func__(self.OpenLink), 6)
		
		self.idEditLine.SetReturnEvent(ui.__mem_func__(self.pwdEditLine.SetFocus))
		self.idEditLine.SetTabEvent(ui.__mem_func__(self.pwdEditLine.SetFocus))

		self.pwdEditLine.SetReturnEvent(ui.__mem_func__(self.__OnClickLoginButton))
		self.pwdEditLine.SetTabEvent(ui.__mem_func__(self.idEditLine.SetFocus))

		self.languageButtonDict = {
			"de": self.GetChild("language_de_button"),
			"it": self.GetChild("language_it_button"),
			"en": self.GetChild("language_eu_button"),
			"tr": self.GetChild("language_tr_button"),
			"ro": self.GetChild("language_ro_button"),
			"pt": self.GetChild("language_pt_button"),
			"pl": self.GetChild("language_pl_button"),
			"es": self.GetChild("language_es_button"),
		}

		locale_name = app.GetLocaleName()
		for key, button in self.languageButtonDict.items():
			button.SetEvent(ui.__mem_func__(self.__QuestionChangeLanguage), key)
			if locale_name == key:
				button.Down()
				button.Disable()

		return 1

	def OpenLink(self, index):
		pass

	def __UpdateServerSettings(self):
		self.stream.SetConnectInfo(GetAuthIP(), config.SERVER_SETTINGS["CH1"], GetAuthIP(), config.SERVER_SETTINGS["AUTH"])
		net.SetMarkServer(GetAuthIP(), config.SERVER_SETTINGS["MARK"])
		app.SetGuildMarkPath("10.tga")
		app.SetGuildSymbolPath("10")
		net.SetServerInfo("%s, CH1" % config.SERVER_SETTINGS["SERVER_NAME"])
		self.__RequestServerStateList()

	def SaveAccounts(self):
		global ACCOUNTS
		id = self.idEditLine.GetText()
		pwd = self.pwdEditLine.GetText()
		
		for i in xrange(len(ACCOUNTS)):
			if ACCOUNTS[i][0] == uiScriptLocale.ACCOUNT_SAVED and id != "" and pwd != "":
				ACCOUNTS[i][0] = id
				ACCOUNTS[i][1] = pwd
				SaveAccounts()
				self.LoadAccounts()
				break
			elif ACCOUNTS[i][0] == id:
				self.PopupDisplayMessage("Account already saved.")
				break
				
	def DeleteAccount(self, nAccount):
		if str(ACCOUNTS[nAccount][0]) != uiScriptLocale.ACCOUNT_SAVED:
			ACCOUNTS[nAccount][0] = uiScriptLocale.ACCOUNT_SAVED
			ACCOUNTS[nAccount][1] = uiScriptLocale.ACCOUNT_SAVED
			SaveAccounts()
			
		for i in xrange(len(self.accountDeleteDict)):
			self.accountDeleteDict[i].Hide()
			
		self.accountSaveDict[nAccount].Show()

		self.LoadAccounts()
		
	def SaveAccountSlot(self, slot):
		id = self.idEditLine.GetText()
		pwd = self.pwdEditLine.GetText()
		
		if ACCOUNTS[slot][0] == uiScriptLocale.ACCOUNT_SAVED and id != "" and pwd != "":
			ACCOUNTS[slot][0] = id
			ACCOUNTS[slot][1] = pwd
			SaveAccounts()
			self.LoadAccounts()

	def LoadAccounts(self):
		global ACCOUNTS
		LoadAccounts()	
		for i in xrange(len(self.accountNameDict)):
			self.accountNameDict[i].SetText(str(ACCOUNTS[i][0]))
			
			if ACCOUNTS[i][0] != uiScriptLocale.ACCOUNT_SAVED:
				self.accountDeleteDict[i].Show()
				self.accountSaveDict[i].Hide()

			if ACCOUNTS[i][0] == uiScriptLocale.ACCOUNT_SAVED:
				self.accountSaveDict[i].Show()
			
	def ConnectOnAccount(self, nAccount):
		id = self.idEditLine.GetText()
		pwd = self.pwdEditLine.GetText()
		
		if str(ACCOUNTS[nAccount][0]) == uiScriptLocale.ACCOUNT_SAVED:
			self.PopupDisplayMessage("Save your account first.")
			return
			
		# Removed saving account when pressing slot account since it has its own button now..
		# if str(ACCOUNTS[nAccount][0]) == uiScriptLocale.ACCOUNT_SAVED and id != "" and pwd != "":
			# self.SaveAccounts()
		
		if str(ACCOUNTS[nAccount][0]) != uiScriptLocale.ACCOUNT_SAVED:
			self.idEditLine.SetText(str(ACCOUNTS[nAccount][0]))
			self.pwdEditLine.SetText(str(ACCOUNTS[nAccount][1]))
			self.Connect(str(ACCOUNTS[nAccount][0]), str(ACCOUNTS[nAccount][1]))

	def OnKeyDown(self, key):
		if self.loginBoard.IsShow():
			try:
				if key == app.DIK_F1:
					if str(ACCOUNTS[0][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[0][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[0][1]))
						self.Connect(str(ACCOUNTS[0][0]), str(ACCOUNTS[0][1]))
				if key == app.DIK_F2:
					if str(ACCOUNTS[1][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[1][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[1][1]))
						self.Connect(str(ACCOUNTS[1][0]), str(ACCOUNTS[1][1]))
				if key == app.DIK_F3:
					if str(ACCOUNTS[2][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[2][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[2][1]))
						self.Connect(str(ACCOUNTS[2][0]), str(ACCOUNTS[2][1]))
				if key == app.DIK_F4:
					if str(ACCOUNTS[3][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[3][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[3][1]))
						self.Connect(str(ACCOUNTS[3][0]), str(ACCOUNTS[3][1]))
				if key == app.DIK_F5:
					if str(ACCOUNTS[4][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[4][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[4][1]))
						self.Connect(str(ACCOUNTS[4][0]), str(ACCOUNTS[4][1]))
				if key == app.DIK_F6:
					if str(ACCOUNTS[5][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[5][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[5][1]))
						self.Connect(str(ACCOUNTS[5][0]), str(ACCOUNTS[5][1]))
				if key == app.DIK_F7:
					if str(ACCOUNTS[6][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[6][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[6][1]))
						self.Connect(str(ACCOUNTS[6][0]), str(ACCOUNTS[6][1]))

				if key == app.DIK_F8:
					if str(ACCOUNTS[7][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[7][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[7][1]))
						self.Connect(str(ACCOUNTS[7][0]), str(ACCOUNTS[7][1]))

				if key == app.DIK_F9:
					if str(ACCOUNTS[8][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[8][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[8][1]))
						self.Connect(str(ACCOUNTS[8][0]), str(ACCOUNTS[8][1]))

				if key == app.DIK_F10:
					if str(ACCOUNTS[9][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[9][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[9][1]))
						self.Connect(str(ACCOUNTS[9][0]), str(ACCOUNTS[9][1]))

				if key == app.DIK_F11:
					if str(ACCOUNTS[10][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[10][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[10][1]))
						self.Connect(str(ACCOUNTS[10][0]), str(ACCOUNTS[10][1]))

				if key == app.DIK_F12:
					if str(ACCOUNTS[11][0]) != uiScriptLocale.ACCOUNT_SAVED:
						self.idEditLine.SetText(str(ACCOUNTS[11][0]))
						self.pwdEditLine.SetText(str(ACCOUNTS[11][1]))
						self.Connect(str(ACCOUNTS[11][0]), str(ACCOUNTS[11][1]))

			except KeyError:
				pass
			except:
				raise

			return True
			
	def __RequestServerStateList(self):
		ServerStateChecker.Initialize();
		for id, channelDataDict in config.CHANNELS_DICT.items():
			key=channelDataDict["key"]
			ip=config.SERVER_SETTINGS["IP"]
			port=channelDataDict["port"]
			ServerStateChecker.AddChannel(key, ip, port)

		ServerStateChecker.Request()
	def GetIndexByName(self, lang):
		langCode = {
			"en": 0,
			"ro": 1,
			"de": 2,
			"it": 3,
			"es": 4,
			"pt": 5,
			"tr": 6,
		}

		try:
			return langCode[lang]
		except:
			return 0

	def __QuestionChangeLanguage(self, lang):
		if app.GetLocaleName() == lang:
			return

		import uiCommon
		self.questionDialog = uiCommon.QuestionDialog()
		self.questionDialog.SetText("Do you want to change the language ?")
		self.questionDialog.SetAcceptEvent(lambda arg=lang: self.OnClickLanguageButton(arg))
		self.questionDialog.SetCancelEvent(lambda arg=lang: self.__QuestionChangeLanguageCancel(arg))
		self.questionDialog.Open()

		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

	def __QuestionChangeLanguageCancel(self, lang):
		self.OnCloseQuestionDialog()

	def OnClickLanguageButton(self, lang):
		if app.GetLocaleName() != lang:
			with open("locale.cfg", "w+") as localeConfig:
				localeConfig.write("10022 %d %s" % (self.GetCodePage(lang), str(lang)))

			app.SetDefaultCodePage(self.GetCodePage(lang))
			app.ForceSetLocale(lang, "locale/%s" % (lang))

			self.OnCloseQuestionDialog()
			app.ShellExecute("Ameria.exe")
			app.Exit()

	def OnCloseQuestionDialog(self):
		if not self.questionDialog:
			return

		self.questionDialog.Close()
		self.questionDialog = None
		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

	def GetCodePage(self, lang):
		codePageDict = {
			"en": 1252,
			"ro": 1250,
			"de": 1252,
			"it": 1252,
			"es": 1252,
			"pt": 1252,
			"tr": 1254,
		}

		try:
			return codePageDict[lang]
		except:
			return 1252

	def SetChannel(self, ch):
		if self.channelTab[ch].IsShow() == False: # @ Fix Mouse Wheel
			return
	
		self.channelTab[self.channelIndex].SetUp()
		self.channelIndex = ch
		self.channelTab[self.channelIndex].Down()
			
		self.stream.SetConnectInfo(GetAuthIP(), config.SERVER_SETTINGS["CH%d" % (ch+1)], GetAuthIP(), config.SERVER_SETTINGS["AUTH"])
		net.SetMarkServer(GetAuthIP(), config.SERVER_SETTINGS["MARK"])
		app.SetGuildMarkPath("10.tga")
		app.SetGuildSymbolPath("10")
		net.SetServerInfo("%s, %s" % (config.SERVER_SETTINGS["SERVER_NAME"], "CH%d" % (ch+1)))
		self.__RequestServerStateList()

	def Connect(self, id, pwd):
		net.ACC_ID = id
		net.ACC_PWD = pwd

		if constInfo.SEQUENCE_PACKET_ENABLE:
			net.SetPacketSequenceMode()

		if IsLoginDelay():
			loginDelay = GetLoginDelay()
			self.connectingDialog = ConnectingDialog()
			self.connectingDialog.Open(loginDelay)
			self.connectingDialog.SAFE_SetTimeOverEvent(self.OnEndCountDown)
			self.connectingDialog.SAFE_SetExitEvent(self.OnPressExitKey)
			self.isNowCountDown = True
		else:
			self.stream.popupWindow.Close()
			self.stream.popupWindow.Open(localeInfo.LOGIN_CONNETING, self.SetPasswordEditLineFocus, localeInfo.UI_CANCEL)

		self.stream.SetLoginInfo(id, pwd)
		self.stream.Connect()

	def __OnClickExitButton(self):
		self.stream.SetPhaseWindow(0)
	
	def PopupDisplayMessage(self, msg):
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg)

	def PopupNotifyMessage(self, msg, func=0):
		if not func:
			func=self.EmptyFunc

		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, func, localeInfo.UI_OK)

	def __OnCloseInputDialog(self):
		if self.inputDialog:
			self.inputDialog.Close()
		self.inputDialog = None
		return True

	def OnPressExitKey(self):
		self.stream.popupWindow.Close()
		self.stream.SetPhaseWindow(0)
		return True

	def OnUpdate(self):
		ServerStateChecker.Update()

	def EmptyFunc(self):
		pass

	def __OnClickLoginButton(self):
		id = self.idEditLine.GetText()
		pwd = self.pwdEditLine.GetText()		

		if len(id)==0:
			self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_ID, self.SetIDEditLineFocus)
			return

		if len(pwd)==0:
			self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_PASSWORD, self.SetPasswordEditLineFocus)
			return

		self.Connect(id, pwd)
	
	def SameLogin_OpenUI(self):
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(localeInfo.LOGIN_FAILURE_SAMELOGIN, 0, localeInfo.UI_OK)
