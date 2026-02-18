# coding: latin_1

###################################################################
# title_name        : Manager Bank System
# date_created        : 2016.09.21
# filename            : uibankmanager.py
# author            : VegaS
# version            : 0.0.1
#
import ui
import localeInfo
import managerBankLogs
import uiToolTip
import chat
import grp
import net
import introLogin
import constInfo
import app
import wndMgr
import uiCommon
import colorInfo

_mColor = 0xfff8d090
_mType = 0

_mbscpy = {}
_mstrcpy = {"date" : {}, "time" : {}, "action" : {}, "ip" : {}, "target" : {}, "amount" : {}}
_snprintf = {0: "", 1:  ""}
_pushback = ["/bank_open %s %s", "/bank_create %s %s", "/bank_send %s %s %s %s", "/bank_changepw %s %s %s %s", "/bank_add ", "/bank_withdraw "]

class BankWindowSelect(ui.BoardWithTitleBar):
    def __init__(self):
        ui.BoardWithTitleBar.__init__(self)
        self.SetSize(250, 75)
        self.SetTitleName(localeInfo.BANK_CHOOSE_BOARD_TITLE)
        self.SetCloseEvent(ui.__mem_func__(self.Close))
        self.SetCenterPosition()
        self.AddFlag('movable')
        self.SetTop()

        self.dict = []
        #self.Append(localeInfo.BANK_CHOOSE_CREATE, (lambda item = 1: self.Global(item)))
        self.Append(localeInfo.BANK_CHOOSE_LOGIN, (lambda item = 2: self.Global(item)))
        
    def Append(self, text, event):
        h = len(self.dict) * 26
        btn = ui.Button()
        btn.SetParent(self)
        btn.SetPosition(33, 36 + h)
        btn.SetUpVisual("d:/ymir work/ui/public/XLarge_Button_01.sub")
        btn.SetOverVisual("d:/ymir work/ui/public/XLarge_Button_02.sub")
        btn.SetDownVisual("d:/ymir work/ui/public/XLarge_Button_03.sub")
        btn.SetText(text)
        btn.SetEvent(event)
        btn.Show()
        self.dict.append(btn)

    def __del__(self):
        ui.BoardWithTitleBar.__del__(self)

    def Destroy(self):
        self.Hide()
        for obj in self.dict:
            obj.Hide()
            obj = None
        del self.dict[:]
        self.dict = None


    def OnPressEscapeKey(self):
        self.Close()
        return True
        
    def Close(self):
        self.Hide()

    def Global(self, c):
        global _mType
        if c == 1:
            _mType = 1
        elif c == 2:
            _mType = 2
        elif c == 3:
            _mType = 3
        elif c == 4:
            _mType = 4
        self.Hide()
        self.Append = LoginAccountBank()
        self.Append.LoadDialog()
        self.Append.Load("open")

class LoginAccountBank(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.UI = {}
        self.LoadDialog()

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def LoadDialog(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "uiscript/LoginAccountBank.py")
        except:
            import exception
            exception.Abort("LoginAccountBank.LoadDialog.LoadObject")
        try:
            self.GetChild("accept_button").SetEvent(lambda arg = "accept": self.Load(arg))
            self.GetChild("cancel_button").SetEvent(lambda arg = "cancel": self.Load(arg))
            self.GetChild("titlebar").SetCloseEvent(lambda arg = "cancel": self.Load(arg))

            self.UI["curLine1"] = self.GetChild("currentLine1_value")
            self.UI["curLine2"] = self.GetChild("currentLine2_value")
            self.UI["curText1"] = self.GetChild("currentLine1_name")
            self.UI["curText2"] = self.GetChild("currentLine2_name")
            self.UI["titleName"] = self.GetChild("TitleName")
            self.UI["btnRandom"] = self.GetChild("password_random")
            self.UI["btnRandom"].SetEvent(self.Random)
            self.UI["btnRandom"].Hide()
        except:
            import exception
            exception.Abort("LoginAccountBank.LoadDialog.BindObject")

        self.UI["curLine1"].SetTabEvent(lambda arg = 1: self.OnNextFocus(arg))
        self.UI["curLine2"].SetTabEvent(lambda arg = 2: self.OnNextFocus(arg))
        self.UI["curLine1"].SetReturnEvent(lambda arg = 1: self.OnNextFocus(arg))
        self.UI["curLine2"].SetReturnEvent(lambda arg = 2: self.OnNextFocus(arg))
        global _mType
        if _mType == 1:
            self.UI["titleName"].SetText(localeInfo.BANK_CREATE_TITLE)
            self.UI["curText1"].SetText(localeInfo.BANK_LINE_ACCOUNT)
            self.UI["curText2"].SetText(localeInfo.BANK_LINE_PASSWORD)
            self.UI["btnRandom"].Show()
            self.SetMaxLength(12, 4)
        if _mType == 2:
            self.UI["titleName"].SetText(localeInfo.BANK_LOGIN_TITLE)
            self.UI["curText1"].SetText(localeInfo.BANK_LINE_ACCOUNT)
            self.UI["curText2"].SetText(localeInfo.BANK_LINE_PASSWORD)
            self.SetMaxLength(12, 4)
            self.SetSecretMode(1)
        if _mType == 3:
            self.UI["titleName"].SetText(localeInfo.BANK_SEND_TITLE)
            self.UI["curText1"].SetText(localeInfo.BANK_LINE_ACCOUNT)
            self.UI["curText2"].SetText(localeInfo.BANK_LINE_AMOUNT)
            self.SetMaxLength(12, 10)
        if _mType == 4:
            self.UI["titleName"].SetText(localeInfo.BANK_CHANGEPW_TITLE)
            self.UI["curText1"].SetText(localeInfo.BANK_LINE_PASSWORD)
            self.UI["curText2"].SetText(localeInfo.BANK_LINE_REPASSWORD)
            self.SetMaxLength(4, 4)
            self.SetSecretMode(2)

    def Random(self):
        rand = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        max_len = 4
        res = ""
        for repeat in xrange(max_len):
            res = res + rand[app.GetRandom(-len(rand), len(rand))]
        self.UI["curLine2"].SetText(str(res))

    def OnNextFocus(self, arg):
        if 1 == arg:
            self.UI["curLine1"].KillFocus()
            self.UI["curLine2"].SetFocus()
        else:
            self.UI["curLine2"].KillFocus()
            self.UI["curLine1"].SetFocus()
            
    def SetSecretMode(self, mode):
        if mode == 1:
            self.UI["curLine2"].SetSecret()
        else:
            self.UI["curLine1"].SetSecret()
            self.UI["curLine2"].SetSecret()    

    def SetMaxLength(self, k, v):
        self.UI["curLine1"].SetMax(k)
        self.UI["curLine2"].SetMax(v)

    def Destroy(self):
        self.ClearDictionary()
        self.UI["curLine1"] = None
        self.UI["curLine2"] = None

    def End(self):
        self.Hide()

    def Login(self):
        _mbscpy["login"] = (self.UI["curLine1"].GetText(), self.UI["curLine2"].GetText())
        net.SendChatPacket(_pushback[0] % _mbscpy["login"])

        _snprintf.update({0 : str(self.UI["curLine1"].GetText())})
        _snprintf.update({1 : str(self.UI["curLine2"].GetText())})
        self.Hide()

    def Load(self, str):
        ret = ["open", "cancel", "accept", 1, 1*2, 1*3, 1*4]

        _mbscpy["create"] = (self.UI["curLine1"].GetText(), self.UI["curLine2"].GetText())
        _mbscpy["send"] = (_snprintf[0], _snprintf[1], self.UI["curLine1"].GetText(), self.UI["curLine2"].GetText())
        _mbscpy["changepassword"] = (_snprintf[0], _snprintf[1], self.UI["curLine1"].GetText(), self.UI["curLine2"].GetText())

        global _mType
        if str == ret[0]:
            self.UI["curLine1"].SetFocus()
            self.SetCenterPosition()
            self.SetTop()
            self.Show()
        elif str == ret[1]:
            self.Hide()
        elif str == ret[2]:
            if _mType == ret[3]:
                net.SendChatPacket(_pushback[1] % _mbscpy["create"])
            elif _mType == ret[4]:
                self.connect = introLogin.ConnectingDialog()
                self.connect.Open(3.0)
                self.connect.SetText(localeInfo.BANK_CONECTING_DIALOG)
                self.connect.SAFE_SetTimeOverEvent(self.Login)
                self.connect.SAFE_SetExitEvent(self.End)
            elif _mType == ret[5]:
                net.SendChatPacket(_pushback[2] % _mbscpy["send"])
            elif _mType == ret[6]:
                net.SendChatPacket(_pushback[3] % _mbscpy["changepassword"])
            self.Hide()

class BankGui(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.Input = {}

        PATH = "d:/ymir work/ui/public/bank/"
        self.str = {
            'btnAdd' : {'default_image': PATH+"btn_add_01.tga",    'over_image': PATH+"btn_add_02.tga", 'down_image': PATH+"btn_add_03.tga"},
            'btnWithdraw' : {'default_image': PATH+"btn_withdraw_01.tga", 'over_image': PATH+"btn_withdraw_02.tga", 'down_image': PATH+"btn_withdraw_03.tga"},
            'btnSend' : {'default_image': PATH+"btn_send01.tga", 'over_image': PATH+"btn_send02.tga", 'down_image': PATH+"btn_send03.tga"},
            'btnChangePassword' : {'default_image': PATH+"btn_change_01.tga", 'over_image': PATH+"btn_change_02.tga", 'down_image': PATH+"btn_change_03.tga"},
            'btnPrev' : {'default_image': PATH+"prev_btn_01.tga", 'over_image': PATH+"prev_btn_02.tga", 'down_image': PATH+"prev_btn_01.tga"},
            'btnNext' : {'default_image': PATH+"next_btn_01.tga", 'over_image': PATH+"next_btn_02.tga", 'down_image': PATH+"next_btn_01.tga"},
            'backgroundLogs' : PATH+"bg_logs.tga",
            'backgroundMoney' : PATH+"bg_money.tga",
        }
        self.UI = {}
        self.Text = {
            "logsDate" : {},
            "logsTime" : {},
            "logsAction" : {},
            "logsIP" : {},
            "logsTarget" : {},
            "logsAmount" : {},
        }
        self.pos = [[[12, 468]],[[502, 468]],[[-30, 5]],[[410, 474]],[[65, 8]],[[220, 8]]]

        self.page = 1
        self.descBtn = uiToolTip.ToolTip()
        self.linkBtn = BankWindowSelect()
        self.BuildWindow()

    def MakeText(self, parent, textlineText, x, y, color):
        textline = ui.TextLine()
        if parent != None:
            textline.SetParent(parent)
        textline.SetPosition(x, y)
        if color != None:
            textline.SetPackedFontColor(color)
        textline.SetText(textlineText)
        textline.Show()
        return textline

    def MakeLine(self, parent, x, y, sizeX, sizeY, color):
        line = ui.Line()
        if parent != None:
            line.SetParent(parent)
        line.SetPosition(x, y)
        line.SetSize(sizeX, sizeY)
        if color != None:
            line.SetColor(color)
        line.Show()
        return line

    def MakeImage(self, parent, x, y, img):
        image = ui.ExpandedImageBox()
        if parent != None:
            image.SetParent(parent)
        image.SetPosition(x, y)
        image.LoadImage(img)
        image.Show()
        return image

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def BuildWindow(self):
        self.UI["Board"] = ui.BoardWithTitleBar()
        self.UI["Board"].SetSize(575, 512)
        self.UI["Board"].SetCenterPosition()
        self.UI["Board"].AddFlag('movable')
        self.UI["Board"].AddFlag('float')
        self.UI["Board"].SetTitleName(localeInfo.BANK_TITLE_NAME)
        self.UI["Board"].SetCloseEvent(self.Close)

        self.UI["bar_line"] = ui.Bar()
        self.UI["bar_line"].SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 0.4))
        self.UI["bar_line"].SetParent(self.UI["Board"])
        self.UI["bar_line"].SetPosition(8, 32)
        self.UI["bar_line"].SetSize(555, 472)
        self.UI["bar_line"].Show()

        self.UI["right_line"] = self.MakeLine(self.UI["bar_line"], 555, 0, 0, 470, 0xffAAA6A1)
        self.UI["bottom_line"] = self.MakeLine(self.UI["bar_line"], 0, 470-1, 555, 0, 0xffAAA6A1)
        self.UI["left_line"] = self.MakeLine(self.UI["bar_line"], 0, 0, 0, 470-1, 0xffAAA6A1)
        self.UI["top_line"] = self.MakeLine(self.UI["bar_line"], 0, 0, 555, 0, 0xffAAA6A1)
        self.UI["end_line"] = self.MakeLine(self.UI["bar_line"], 0, 433-1, 555, 0, 0xffAAA6A1)

        self.UI["BackgroundLogs"] = self.MakeImage(self.UI["Board"] , 12, 36, self.str["backgroundLogs"])
        self.UI["BackgroundAmount"] = self.MakeImage(self.UI["Board"] , 145, 470, self.str["backgroundMoney"])

        self.UI["tmpDate"] = self.MakeText(self.UI["BackgroundLogs"], localeInfo.BANK_TITLE_LOGS_DATE, 20, 5, _mColor)
        self.UI["tmpTime"] = self.MakeText(self.UI["BackgroundLogs"], localeInfo.BANK_TITLE_LOGS_HOUR, 75, 5, _mColor)
        self.UI["tmpAction"] = self.MakeText(self.UI["BackgroundLogs"], localeInfo.BANK_TITLE_LOGS_ACTION, 130, 5, _mColor)
        self.UI["tmpIP"] = self.MakeText(self.UI["BackgroundLogs"], localeInfo.BANK_TITLE_LOGS_IP, 220, 5, _mColor)
        self.UI["tmpTarget"] = self.MakeText(self.UI["BackgroundLogs"], localeInfo.BANK_TITLE_LOGS_TARGET, 360, 5, _mColor)
        self.UI["tmpAmount"] = self.MakeText(self.UI["BackgroundLogs"], localeInfo.BANK_TITLE_LOGS_AMOUNT, 450, 5, _mColor)
        self.UI["tCurMoney"] = self.MakeText(self.UI["BackgroundAmount"], '', 194, 7, _mColor)
        self.UI["tPage"] = self.MakeText(self.UI["Board"], '', 465, 10, 0xfff8d090)
        self.UI["tmpTarget"].SetHorizontalAlignRight()
        self.UI["tCurMoney"].SetHorizontalAlignRight()

        L = 0
        yPos = 1 + 23
        while L < 25:
            self.Text["logsDate"][L] = self.MakeText(self.UI["BackgroundLogs"], '', 11, yPos, None)
            self.Text["logsTime"][L] = self.MakeText(self.UI["BackgroundLogs"], '', 70, yPos, None)
            self.Text["logsAction"][L] = self.MakeText(self.UI["BackgroundLogs"], '', 124, yPos, None)
            self.Text["logsIP"][L] = self.MakeText(self.UI["BackgroundLogs"], '', 197, yPos, None)
            self.Text["logsTarget"][L] = self.MakeText(self.UI["BackgroundLogs"], '', 265, yPos, None)
            self.Text["logsAmount"][L] = self.MakeText(self.UI["BackgroundLogs"], '', 130, yPos, None)
            self.Text["logsTarget"][L].SetWindowHorizontalAlignRight()
            self.Text["logsAmount"][L].SetWindowHorizontalAlignRight()
            L = L + 1
            yPos = yPos + 16

        self.UI["btnAdd"] = ui.MakeButton(self.UI["Board"], self.pos[0][0][0], self.pos[0][0][1], None, "", self.str["btnAdd"]["default_image"], self.str["btnAdd"]["over_image"], self.str["btnAdd"]["down_image"])
        self.UI["btnWithdraw"] = ui.MakeButton(self.UI["Board"], self.pos[1][0][0], self.pos[1][0][1], None, "", self.str["btnWithdraw"]["default_image"], self.str["btnWithdraw"]["over_image"], self.str["btnWithdraw"]["down_image"])
        self.UI["btnSend"] = ui.MakeButton(self.UI["BackgroundAmount"], self.pos[2][0][0], self.pos[2][0][1], None, "", self.str["btnSend"]["default_image"], self.str["btnSend"]["over_image"], self.str["btnSend"]["down_image"])
        self.UI["btnChangePassword"] = ui.MakeButton(self.UI["Board"], self.pos[3][0][0], self.pos[3][0][1], None, "", self.str["btnChangePassword"]["default_image"], self.str["btnChangePassword"]["over_image"], self.str["btnChangePassword"]["down_image"])
        self.UI["btnPrev"] = ui.MakeButton(self.UI["BackgroundAmount"], self.pos[4][0][0], self.pos[4][0][1], None, "", self.str["btnPrev"]["default_image"], self.str["btnPrev"]["over_image"], self.str["btnPrev"]["down_image"])
        self.UI["btnNext"] = ui.MakeButton(self.UI["BackgroundAmount"], self.pos[5][0][0], self.pos[2][0][1], None, "", self.str["btnNext"]["default_image"], self.str["btnNext"]["over_image"], self.str["btnNext"]["down_image"])

        self.UI["btnAdd"].SetEvent(lambda item=1: self.AppendInput(item))
        self.UI["btnWithdraw"].SetEvent(lambda item=2: self.AppendInput(item))
        self.UI["btnSend"].SetEvent(lambda item=3: self.linkBtn.Global(item))
        self.UI["btnChangePassword"].SetEvent(lambda item=4: self.linkBtn.Global(item))
        self.UI["btnPrev"].SetEvent(lambda item="back": self.AppendPage(item))
        self.UI["btnNext"].SetEvent(lambda item="next": self.AppendPage(item))

    def AppendLogs(self, a, b):
        self.Text["logsDate"][a].SetText(str(_mstrcpy["date"][b]))
        self.Text["logsTime"][a].SetText(str(_mstrcpy["time"][b]))
        self.Text["logsAction"][a].SetText(str(_mstrcpy["action"][b]))
        self.Text["logsIP"][a].SetText(str(_mstrcpy["ip"][b]))
        self.Text["logsTarget"][a].SetText(str(_mstrcpy["target"][b]))
        
        getString = "53 %s" % localeInfo.MONETARY_UNIT0
        self.Text["logsAmount"][a].SetText(str(_mstrcpy["amount"][b]).replace(getString, "************"))

    def SetPage(self, page):
        MAX_PAGE_LOGS = 40
        self.page = page
        array = [25,50,75,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975]
        for i in xrange(0, 25):
            for j in xrange(1, MAX_PAGE_LOGS+1):
                if page == 1:
                    self.AppendLogs(i,i)
                else:
                    if page == j:
                        self.AppendLogs(i,array[j-2]+i)
        self.UI["tPage"].SetText(localeInfo.BANK_CURRENT_PAGE % (page, MAX_PAGE_LOGS))
        
    def AppendPage(self, step):
        MAX_PAGE_LOGS = 40
        if step == "next":
            if self.page >= MAX_PAGE_LOGS:
                return
            self.page += 1
            self.SetPage(self.page)
        elif step == "back":
            if self.page <= 1:
                return
            self.page -= 1
            self.SetPage(self.page)

    def AppendInput(self, c):
        if c == 1:
            self.Input["add"] = uiCommon.InputDialogWithDescription()
            self.Input["add"].SetTitle(localeInfo.BANK_ADD_TITLE)
            self.Input["add"].SetDescription(localeInfo.BANK_TYPE_AMOUNT)
            self.Input["add"].SetAcceptEvent((lambda k = 0: self.Requested(int(k))))
            self.Input["add"].SetCancelEvent((lambda v = 0: self.RejectDialog(int(v))))
            self.Input["add"].SetMaxLength(11)
            self.Input["add"].SetBoardWidth(210)
            self.Input["add"].SetNumberMode()
            self.Input["add"].Open()
        elif c == 2:
            self.Input["withdraw"] = uiCommon.InputDialogWithDescription()
            self.Input["withdraw"].SetTitle(localeInfo.BANK_WITHDRAW_TITLE)
            self.Input["withdraw"].SetDescription(localeInfo.BANK_TYPE_AMOUNT)
            self.Input["withdraw"].SetAcceptEvent((lambda k = 1: self.Requested(int(k))))
            self.Input["withdraw"].SetCancelEvent((lambda v = 1: self.RejectDialog(int(v))))
            self.Input["withdraw"].SetMaxLength(11)
            self.Input["withdraw"].SetBoardWidth(210)
            self.Input["withdraw"].SetNumberMode()
            self.Input["withdraw"].Open()

    def Requested(self, mode):
        if mode == 0:
            _mbscpy["add"] = _snprintf[0] + " " + _snprintf[1] + " " + self.Input["add"].GetText()    
            net.SendChatPacket(_pushback[4] + _mbscpy["add"])
        elif mode == 1:
            _mbscpy["withdraw"] = _snprintf[0] + " " + _snprintf[1] + " " + self.Input["withdraw"].GetText()
            net.SendChatPacket(_pushback[5] + _mbscpy["withdraw"])
        self.RejectDialog(mode)

    def RejectDialog(self, mode):
        if mode == 0:
            self.Input["add"].Close()
        elif mode == 1:
            self.Input["withdraw"].Close()

    def Loading(self, c, value):
        self.loginFailure = uiCommon.PopupDialog()
        self.loginFailure.SetWidth(150)
        if c == "login":
            self.linkBtn.Show()
        elif c == "reload":
            self.UI["tCurMoney"].SetText(localeInfo.NumberToMoneyString(value))
        elif c == "open":
            self.OpenWindow()
        elif c == "close":
            self.Close()
        elif c == "wrong_id":
            self.loginFailure.SetText(localeInfo.BANK_WRONG_ID)
            self.loginFailure.Show()
        elif c == "wrong_password":
            self.loginFailure.SetText(localeInfo.BANK_WRONG_PW)
            self.loginFailure.Show()

    def BINARY_Account_Bank_Logs(self):
        for row in xrange(1000):
            res = managerBankLogs.Append(row)
            if int(res[0]) == 999: 
                for rmf in [_mstrcpy["date"],_mstrcpy["time"],_mstrcpy["action"],_mstrcpy["ip"],_mstrcpy["target"],_mstrcpy["amount"]]:
                    rmf.update({row : ""})
            else:
                c = res[2].split(' ')
                r = c[0].split('-')
                r = r[2] + "." + r[1] + "." + r[0]
                _mstrcpy["date"].update({row : r})
                t = c[1]
                _mstrcpy["time"].update({row : t})
                if res[0] == 1:
                    _mstrcpy["action"].update({row : "|cFFffe0e0" + localeInfo.BANK_APPEND_LOG_ADD})
                elif res[0] == 2:
                    _mstrcpy["action"].update({row : "|cFFfe6eff" + localeInfo.BANK_APPEND_LOG_WITHDRAW})
                elif res[0] == 3:
                    _mstrcpy["action"].update({row : "|cFF00ecff" + localeInfo.BANK_APPEND_LOG_SEND})
                elif res[0] == 4:
                    _mstrcpy["action"].update({row : "|cFFc9ff00" + localeInfo.BANK_APPEND_LOG_RECEIVED})
                elif res[0] == 5:
                    _mstrcpy["action"].update({row : "|cFFffa700" + localeInfo.BANK_APPEND_LOG_CHANGE_PW})
                _mstrcpy["ip"].update({row : str(res[3])})    
                _mstrcpy["target"].update({row : str(res[4])})    
                _mstrcpy["amount"].update({row : localeInfo.NumberToMoneyString(res[1])})
        self.SetPage(1)    

    def OnUpdate(self):
        if self.UI["btnAdd"].IsIn():
            self.descBtn.ClearToolTip()
            self.descBtn.AutoAppendTextLine(localeInfo.BANK_TOOLTIP_BTN_ADD, 0xfff8d090)
            self.descBtn.AlignHorizonalCenter()
            self.descBtn.ShowToolTip()

        elif self.UI["btnWithdraw"].IsIn():
            self.descBtn.ClearToolTip()
            self.descBtn.AutoAppendTextLine(localeInfo.BANK_TOOLTIP_BTN_WITHDRAW, 0xfff8d090)
            self.descBtn.AlignHorizonalCenter()
            self.descBtn.ShowToolTip()

        elif self.UI["btnSend"].IsIn():
            self.descBtn.ClearToolTip()
            self.descBtn.AutoAppendTextLine(localeInfo.BANK_TOOLTIP_BTN_SEND, 0xfff8d090)
            self.descBtn.AlignHorizonalCenter()
            self.descBtn.ShowToolTip()
            
        elif self.UI["btnChangePassword"].IsIn():
            self.descBtn.ClearToolTip()
            self.descBtn.AutoAppendTextLine(localeInfo.BANK_TOOLTIP_BTN_CHANGE, 0xfff8d090)
            self.descBtn.AlignHorizonalCenter()
            self.descBtn.ShowToolTip()
        else:
            self.descBtn.HideToolTip()

    def Show(self):
        self.UI["Board"].Show()

    def OpenWindow(self):
        self.UI["Board"].Show()

    def OnPressEscapeKey(self):
        self.Close()
        return True

    def Close(self):
        self.UI["Board"].Hide()