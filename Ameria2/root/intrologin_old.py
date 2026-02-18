# coding: latin_1

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
import os
import sys
import introLoading

#SERVER_IP1 = "94.23.77.86"
SERVER_IP1 = "188.212.100.78"
SERVER_IP2 = "188.212.100.78"

CH1_PORT = 13002
CH2_PORT = 13102
CH3_PORT = 13202
CH4_PORT = 13302
PORT_AUTH = 39994

CH1_PORT2 = 13002
CH2_PORT2 = 13102
CH3_PORT2 = 13202
CH4_PORT2 = 13302
PORT_AUTH2 = 39994

NUME_SERVER = "Ameria"

URL_DICT = {
    "CREATE_ACCOUNT": "https://www.solana2.global/users/register",
    "LANGUAGE": "",
    "DISCORD": "https://discord.gg/solana2",
    "HOMEPAGE": "http://solana2.global/"
}

class LoginWindow(ui.ScriptWindow):
    def __init__(self, stream):
        ui.ScriptWindow.__init__(self)

        net.SetPhaseWindow(net.PHASE_WINDOW_LOGIN, self)
        net.SetAccountConnectorHandler(self)

        self.channelButtonList = []
        self.accountSlotList = []
        self.webButtonDict = {}
        self.languageButtonDict = {}

        self.stream = stream

    def __del__(self):
        ui.ScriptWindow.__del__(self)
        net.ClearPhaseWindow(net.PHASE_WINDOW_LOGIN, self)
        net.SetAccountConnectorHandler(0)

    def Open(self):
        self.loginFailureMsgDict = {
            "ALREADY": localeInfo.LOGIN_FAILURE_ALREAY,
            "NOID": localeInfo.LOGIN_FAILURE_NOT_EXIST_ID,
            "WRONGPWD": localeInfo.LOGIN_FAILURE_WRONG_PASSWORD,
            "FULL": localeInfo.LOGIN_FAILURE_TOO_MANY_USER,
            "SHUTDOWN": localeInfo.LOGIN_FAILURE_SHUTDOWN,
            "REPAIR": localeInfo.LOGIN_FAILURE_REPAIR_ID,
            "BLOCK": localeInfo.LOGIN_FAILURE_BLOCK_ID,
            "BESAMEKEY": localeInfo.LOGIN_FAILURE_BE_SAME_KEY,
            "NOTAVAIL": localeInfo.LOGIN_FAILURE_NOT_AVAIL,
            "NOBILL": localeInfo.LOGIN_FAILURE_NOBILL,
            "BLKLOGIN": localeInfo.LOGIN_FAILURE_BLOCK_LOGIN,
            "WEBBLK": localeInfo.LOGIN_FAILURE_WEB_BLOCK,
            "HWID": "HWIDHWIDHWIDHWID.",
            "HARDBAN": "Your computer is banned.",
            "BLOCK_ROUTHER": localeInfo.LOGIN_FAILURE_BLOCK_ROUTHER,
        }
        self.loginFailureFuncDict = {
            "QUIT": app.Exit,
        }

        self.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
        self.SetWindowName("LoginWindow")

        self.__LoadScript("UIScript/loginwindow.py")
        #self.__LoadLoginFile("logininfo.py")

        ime.AddExceptKey(91)
        ime.AddExceptKey(93)
        
        loadServerID, loadChannelID = self.__LoadChannelInfo()
        self.selectedChannel = loadChannelID
        self.selectedServer = loadServerID
        
        self.__OnClickServerButton(self.selectedServer)

        self.Show()
        app.ShowCursor()

    #def __LoadLoginFile(self, loginInfoFileName):
    #    try:
    #        loginInfo = {}
    #        execfile(loginInfoFileName, loginInfo)
    #    except IOError:
    #        print(\
    #            "ÀÚµ¿ ·Î±×ÀÎÀ» ÇÏ½Ã·Á¸é" + loginInfoFileName + "ÆÄÀÏÀ» ÀÛ¼ºÇØÁÖ¼¼¿ä\n"\
    #            "\n"\
    #            "³»¿ë:\n"\
    #            "================================================================\n"
    #        );

    def Close(self):
        if self.stream.popupWindow:
            self.stream.popupWindow.Close()

        self.ClearDictionary()
        self.Hide()
        self.KillFocus()

        self.channelButtonList = []
        self.accountSlotList = []
        self.webButtonDict = {}
        self.languageButtonDict = {}

        app.HideCursor()
        ime.ClearExceptKey()
        self.__SaveChannelInfo()

    def OnConnectFailure(self):
        self.PopupNotifyMessage(localeInfo.LOGIN_CONNECT_FAILURE, self.EmptyFunc)

    def OnHandShake(self):
        self.PopupDisplayMessage(localeInfo.LOGIN_CONNECT_SUCCESS)

    def OnLoginStart(self):
        self.PopupDisplayMessage(localeInfo.LOGIN_PROCESSING)

    def OnLoginFailure(self, error):
        try:
            loginFailureMsg = self.loginFailureMsgDict[error]
        except KeyError:
            loginFailureMsg = localeInfo.LOGIN_FAILURE_BLOCK_ROUTHER

        loginFailureFunc = self.loginFailureFuncDict.get(error, self.EmptyFunc)

        self.PopupNotifyMessage(loginFailureMsg, loginFailureFunc)

    def __LoadScript(self, fileName):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, fileName)
        except:
            import exception
            exception.Abort("LoginWindow.__LoadScript.LoadObject")

        try:
            self.idEditLine = self.GetChild("id")
            self.pwdEditLine = self.GetChild("pwd")
            self.loginButton = self.GetChild("login_button")
            # self.server1Button = self.GetChild("server1_button")
            # self.server2Button = self.GetChild("server2_button")
            self.languageBoard = self.GetChild("language_board")
            self.languageImage = self.GetChild("language_image")

            self.channelButtonList = [
                self.GetChild("channel_button_%02d" % (i + 1)) for i in xrange(4)
            ]

            for i in xrange(4):
                accountSlot = {
                    "value": self.GetChild("account_value_%02d" % (i + 1)),
                    "delete_button": self.GetChild("account_delete_button_%02d" % (i + 1)),
                    "load_button": self.GetChild("account_load_button_%02d" % (i + 1)),
                }

                accountSlot["delete_button"].SetEvent(ui.__mem_func__(self.__OnClickAccountDeleteButton), i)
                accountSlot["load_button"].SetEvent(ui.__mem_func__(self.__OnClickAccountLoadButton), i)

                self.accountSlotList.append(accountSlot)

            self.webButtonDict = {
                "CREATE_ACCOUNT": self.GetChild("create_account_button"),
                "LANGUAGE": self.GetChild("language_button"),
                "DISCORD": self.GetChild("discord_button"),
                "HOMEPAGE": self.GetChild("homepage_button"),
            }

            for key, button in self.webButtonDict.items():
                button.SetEvent(ui.__mem_func__(self.__OnClickWebButton), key)
        except:
            import exception
            exception.Abort("LoginWindow.__LoadScript.BindObject")

        for i, button in enumerate(self.channelButtonList):
            button.SetEvent(ui.__mem_func__(self.__OnClickChannelButton), i)

        self.loginButton.SetEvent(ui.__mem_func__(self.__OnClickLoginButton))
        # self.server1Button.SetEvent(ui.__mem_func__(self.__OnClickServerButton), 1)
        # self.server2Button.SetEvent(ui.__mem_func__(self.__OnClickServerButton), 2)

        self.idEditLine.SetReturnEvent(ui.__mem_func__(self.pwdEditLine.SetFocus))
        self.idEditLine.SetTabEvent(ui.__mem_func__(self.pwdEditLine.SetFocus))
        self.pwdEditLine.SetReturnEvent(ui.__mem_func__(self.__OnClickLoginButton))
        self.pwdEditLine.SetTabEvent(ui.__mem_func__(self.idEditLine.SetFocus))
        self.idEditLine.SetFocus()

        self.languageButtonDict = {
            # "uk": self.GetChild("language_uk_button"),
            # "us": self.GetChild("language_us_button"),
            # "fr": self.GetChild("language_fr_button"),
            "de": self.GetChild("language_de_button"),
            # "cn": self.GetChild("language_cn_button"),
            # "at": self.GetChild("language_at_button"),
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

                self.languageImage.LoadImage(button.GetUpVisualFileName())

        # self.languageBoard.Hide()

        return 1

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

    def __OnClickChannelButton(self, ch):
        map(lambda x: x.SetUp(), self.channelButtonList)

        self.channelButtonList[ch].Down()

        self.stream.SetConnectInfo(self.SERVER_IP, self.ChannelPort(ch, 0), self.SERVER_IP, self.ChannelPort("LOGIN"))
        net.SetMarkServer(self.SERVER_IP, self.ChannelPort("LOGO"))
        app.SetGuildMarkPath("10.tga")
        app.SetGuildSymbolPath("10")
        self.__RefreshSavedAccount()
        net.SetServerInfo(self.ChannelPort(ch, 2))
        self.selectedChannel = ch
        
        self.__SaveChannelInfo()

    def ChannelPort(self, ch, value=0):
        channel = {
            0: CH1_PORT,
            1: CH2_PORT,
            2: CH3_PORT,
            3: CH4_PORT, }

        if self.selectedServer == 2:
            channel = {
                0: CH1_PORT2,
                1: CH2_PORT2,
                2: CH3_PORT2,
                3: CH4_PORT2, }

        if ch == "LOGIN":
            if self.selectedServer == 2:
                return PORT_AUTH2

            return PORT_AUTH
        elif ch == "LOGO":
            return channel[0]
        elif value == 2:
            return NUME_SERVER + ", CH%s" % (ch + 1)
        else:
            return channel[ch]

    def Connect(self, id, pwd):
        constInfo.LastAccount = id.lower()

        self.stream.popupWindow.Close()
        self.stream.popupWindow.Open(localeInfo.LOGIN_CONNETING, self.EmptyFunc, localeInfo.UI_CANCEL)

        self.stream.SetLoginInfo(id, pwd)
        self.stream.Connect()

    def PopupDisplayMessage(self, msg):
        self.stream.popupWindow.Close()
        self.stream.popupWindow.Open(msg)

    def PopupNotifyMessage(self, msg, func=0):
        if not func:
            func = self.EmptyFunc

        # self.stream.popupWindow.Close()
        self.stream.popupWindow.Open(msg, func, localeInfo.UI_OK)

    def OnPressExitKey(self):
        if self.stream.popupWindow:
            self.stream.popupWindow.Close()
        self.stream.SetPhaseWindow(0)
        return True

    def EmptyFunc(self):
        pass

    def __OnClickLoginButton(self):
        id = self.idEditLine.GetText()
        pwd = self.pwdEditLine.GetText()

        if len(id) == 0:
            self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_ID, self.EmptyFunc)
            return

        if len(pwd) == 0:
            self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_PASSWORD, self.EmptyFunc)
            return

        self.Connect(id, pwd)

    def __OnClickServerButton(self, server):
        if server == 1:
            # self.server1Button.Down()
            # self.server2Button.SetUp()
            
            self.SERVER_IP = SERVER_IP1
        else:
            # self.server2Button.Down()
            # self.server1Button.SetUp()
            
            self.SERVER_IP = SERVER_IP2

        self.selectedServer = server
        
        self.__OnClickChannelButton(self.selectedChannel)

    def __RefreshSavedAccount(self):
        for i, accountSlot in enumerate(self.accountSlotList):
            login = constInfo.get_reg("login%d" % i, "")

            if login:
                accountSlot["value"].SetText("F{}. {}".format(i + 1, login))
                accountSlot["delete_button"].Show()
                accountSlot["load_button"].Show()
            else:
                accountSlot["value"].SetText(uiScriptLocale.LOGIN_INTERFACE_FREE_SPACE)
                accountSlot["delete_button"].Hide()
                accountSlot["load_button"].Show()

    def __OnClickAccountLoadButton(self, id, by_system=False):
        login = constInfo.get_reg("login%d" % id, "")
        pwd = constInfo.get_reg("password%d" % id, "")
        if login and pwd:
            self.Connect(login, pwd)
            return

        if by_system:
            return

        user = self.idEditLine.GetText()
        pwd = self.pwdEditLine.GetText()

        if user == "":
            self.PopupNotifyMessage(uiScriptLocale.LOGIN_INTERFACE_PASTE_ID)
            return

        if pwd == "":
            self.PopupNotifyMessage(uiScriptLocale.LOGIN_INTERFACE_PASTE_PW)
            return

        constInfo.set_reg("login%d" % id, user)
        constInfo.set_reg("password%d" % id, pwd)

        self.PopupNotifyMessage(uiScriptLocale.LOGIN_INTERFACE_SAVED)
        self.__RefreshSavedAccount()

    def __OnClickAccountDeleteButton(self, id):
        constInfo.set_reg("login%d" % id, "")
        constInfo.set_reg("password%d" % id, "")
        self.__RefreshSavedAccount()
        self.PopupNotifyMessage("Slot " + str(id + 1) + " " + uiScriptLocale.LOGIN_INTERFACE_DELETED)

    def __OnClickWebButton(self, key):
        if key == "LANGUAGE":
            if self.languageBoard.IsShow():
                self.languageBoard.Show()

            return

        url = URL_DICT.get(key, None)

        if url:
            os.system("start %s" % url)


    def OnKeyDown(self, key):
        if key in range(app.DIK_F1, app.DIK_F4):
            self.__OnClickAccountLoadButton(key - app.DIK_F1, True)
            return True
            
    def __SaveChannelInfo(self):
        with open("channel.inf", "w") as file:
            file.write("%d %d" % (self.selectedServer, self.selectedChannel))

    def __LoadChannelInfo(self):
        try:
            with open("channel.inf", "r+") as file:
                lines=file.readlines()

                
                if len(lines)>0:
                    tokens=lines[0].split()

                    selServerID=int(tokens[0])
                    selChannelID=int(tokens[1])

                    return selServerID, selChannelID
        except:
            self.selectedServer = 1
            self.selectedChannel = 0
            self.__SaveChannelInfo()

        return 1, 0
