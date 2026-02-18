# coding: latin_1

import time

import app
import chat
import net
import grp

import ui
import uiCommon
import localeInfo

class CaptchaDialog(ui.ScriptWindow):
    interfaceInst = None

    CAPTCHA_TIME = 0.0
    captchaImg = None

    def __init__(self, interfaceInst):
        ui.ScriptWindow.__init__(self, "TOP_MOST")

        self.isLoaded = False
        self.captchaTime = None
        self.popUp = None

        from _weakref import proxy
        self.interfaceInst = proxy(interfaceInst)

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def LoadDialog(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "uiscript/captchadialog.py")
        except:
            import exception
            exception.Abort("CaptchaDialog.LoadDialog.LoadScript")

        try:
            self.thinboard = self.GetChild("thin_board")
            self.board = self.GetChild("board")
            self.titleBar = self.GetChild("title_bar")
            self.titleName = self.GetChild("title_name")

            self.captchaTimeName = self.GetChild("title_name")
            self.captchaValue = self.GetChild("captcha_value")

            self.acceptButton = self.GetChild("accept_button")
            self.cancelButton = self.GetChild("cancel_button")
        except:
            import exception
            exception.Abort("CaptchaDialog.LoadDialog.BindObject")

        self.titleBar.SetCloseEvent(ui.__mem_func__(self.__OnCancelButton))
        self.acceptButton.SetEvent(ui.__mem_func__(self.__OnAcceptButton))
        self.cancelButton.SetEvent(ui.__mem_func__(self.__OnCancelButton))

        self.popUp = uiCommon.PopupDialog(1)

        self.startTime = float(app.GetTime())
        if self.captchaTime is None:
            self.captchaTime = app.GetGlobalTimeStamp() + int(self.CAPTCHA_TIME)

        self.captchaValue.SetFocus()
        self.isLoaded = True

    def Destroy(self):
        self.Close()

        self.interfaceInst = None

        if self.captchaImg:
            del self.captchaImg
            self.captchaImg = None

        if self.popUp:
            self.popUp.Close()
            self.popUp.Destroy()
            del self.popUp
            self.popUp = None

        self.isLoaded = False
        self.captchaTime = None

        self.ClearDictionary()

    def Open(self, code, limit):
        if code == 0 and limit == 0:
            if self.interfaceInst:
                self.interfaceInst.ShowDefaultWindows()

            self.captchaValue.KillFocus()
            self.Close()
        elif code == 0 and limit == 1:
            if self.popUp:
                self.popUp.Close()

            self.captchaValue.SetText("")
            self.popUp.Open()
            self.popUp.SetTop()
            self.popUp.SetText(localeInfo.CAPTCHA_WRONG)
            return 0
        else:
            self.CAPTCHA_TIME = limit

            if not self.isLoaded:
                self.LoadDialog()

            if self.captchaImg:
                del self.captchaImg
                self.captchaImg = None

            self.captchaImg = ui.ExpandedImageBox()
            self.captchaImg.SetParent(self.GetChild("captcha_bg_ui"))
            self.captchaImg.LoadImage("UserData/c.bmp")
            self.captchaImg.SetPosition(1, 1)
            self.captchaImg.Show()

            import os
            os.remove("UserData/c.bmp")

            self.SetCenterPosition()
            self.SetTop()
            self.Show()

    def Close(self):
        if self.captchaImg:
            del self.captchaImg
            self.captchaImg = None

        if self.popUp:
            self.popUp.Close()

        self.captchaTime = None
        self.isLoaded = False
        self.Hide()

    def CheckCaptcha(self):
        if self.popUp:
            self.popUp.Close()

        if len(self.captchaValue.GetText()) <= 0:
            self.popUp.Open()
            self.popUp.SetText(localeInfo.CAPTCHA_EMPTY)
            return 0

        return 1

    def __OnAcceptButton(self):
        if self.CheckCaptcha() == 1:
            net.ajf35iuhfs55539141(int(self.captchaValue.GetText()))
            self.captchaValue.SetText("")

    def __OnCancelButton(self):
        if len(self.captchaValue.GetText()) > 0:
            self.captchaValue.SetText("")
        else:
            self.CheckCaptcha()

    def OnUpdate(self):
        if self.IsShow():
            if self.captchaTime is None:
                self.captchaTime = app.GetGlobalTimeStamp() + int(self.CAPTCHA_TIME)

            leftTime = max(0, self.captchaTime - app.GetGlobalTimeStamp())
            leftMin, leftSec = divmod(leftTime, 30)

            if leftTime > 0:
                if leftSec <= 5:
                    self.captchaTimeName.SetFontColor((255.00 / 255), (0.00 / 255), (0.00 / 255))

                self.captchaTimeName.SetText("%02d:%02d" % (leftMin, leftSec))

                if self.interfaceInst:
                    self.interfaceInst.HideAllWindows()
            else:
                leftTime = None
                self.captchaTimeName.SetText("00:00")
