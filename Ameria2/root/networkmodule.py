# coding: latin_1

###################################################################################################
# Network

import app
import chr
import dbg
import net
import snd

import chr
import chrmgr
import background
import player
import playerSettingModule

import ui
import uiPhaseCurtain

import localeInfo
import constInfo

class PopupDialog(ui.ScriptWindow):

    def __init__(self):
        print "NEW POPUP DIALOG ----------------------------------------------------------------------------"
        ui.ScriptWindow.__init__(self)
        self.CloseEvent = 0

    def __del__(self):
        print "---------------------------------------------------------------------------- DELETE POPUP DIALOG "
        ui.ScriptWindow.__del__(self)

    def LoadDialog(self):
        PythonScriptLoader = ui.PythonScriptLoader()
        PythonScriptLoader.LoadScriptFile(self, "UIScript/PopupDialog.py")

    def Open(self, Message, event = 0, ButtonName = localeInfo.UI_CANCEL):

        if True == self.IsShow():
            self.Close()

        self.Lock()
        self.SetTop()
        self.CloseEvent = event

        AcceptButton = self.GetChild("accept")
        AcceptButton.SetText(ButtonName)
        AcceptButton.SetEvent(ui.__mem_func__(self.Close))

        self.GetChild("message").SetText(Message)
        self.Show()

    def Close(self):

        if False == self.IsShow():
            self.CloseEvent = 0
            return

        self.Unlock()
        self.Hide()

        if 0 != self.CloseEvent:
            self.CloseEvent()
            self.CloseEvent = 0

    def Destroy(self):
        self.Close()
        self.ClearDictionary()

    def OnPressEscapeKey(self):
        self.Close()
        return True

    def OnIMEReturn(self):
        self.Close()
        return True

##
## Main Stream
##
class MainStream(object):
    isChrData=0

    def __init__(self):
        print "NEWMAIN STREAM ----------------------------------------------------------------------------"
        net.SetHandler(self)
        net.SetTCPRecvBufferSize(128*1024)
        if app.ENABLE_VOICE_CHAT:
            net.SetTCPSendBufferSize(1024 * 12) # 12kb
        else:
            net.SetTCPSendBufferSize(4096)
        net.SetUDPRecvBufferSize(4096)

        self.id=""
        self.pwd=""
        self.addr=""
        self.port=0
        self.account_addr=0
        self.account_port=0
        self.slot=0
        self.isAutoSelect=0
        self.isAutoLogin=0

        self.curtain = 0
        self.curPhaseWindow = 0
        self.newPhaseWindow = 0

    def __del__(self):
        print "---------------------------------------------------------------------------- DELETE MAIN STREAM "

    def Destroy(self):
        if self.curPhaseWindow:
            self.curPhaseWindow.Close()
            self.curPhaseWindow = 0

        if self.newPhaseWindow:
            self.newPhaseWindow.Close()
            self.newPhaseWindow = 0

        self.popupWindow.Destroy()
        self.popupWindow = 0

        if app.NEW_SELECT_CHARACTER:
            self.curtain = 0

    def Create(self):
        self.CreatePopupDialog()

        if app.NEW_SELECT_CHARACTER:
            self.curtain = uiPhaseCurtain.PhaseCurtain()

    def SetPhaseWindow(self, newPhaseWindow):
        if app.NEW_SELECT_CHARACTER and constInfo.PHASE_WINDOW:
            if self.newPhaseWindow:
                self.__ChangePhaseWindow()

            self.newPhaseWindow=newPhaseWindow

            if self.curPhaseWindow:
                self.curtain.FadeOut(self.__ChangePhaseWindow)
            else:
                self.__ChangePhaseWindow()
        else:
            self.newPhaseWindow = newPhaseWindow
            self.__ChangePhaseWindow()

    def __ChangePhaseWindow(self):
        oldPhaseWindow=self.curPhaseWindow
        newPhaseWindow=self.newPhaseWindow
        self.curPhaseWindow=0
        self.newPhaseWindow=0

        if oldPhaseWindow:
            oldPhaseWindow.Close()

        if newPhaseWindow:
            newPhaseWindow.Open()

        self.curPhaseWindow=newPhaseWindow

        if app.NEW_SELECT_CHARACTER and constInfo.PHASE_WINDOW:
            if self.curPhaseWindow:
                self.curtain.FadeIn()
                if app.NEW_SELECT_CHARACTER:
                    self.curPhaseWindow.SetFocus()
            else:
                app.Exit()
        else:
            if not self.curPhaseWindow:
                app.Exit()

    def CreatePopupDialog(self):
        self.popupWindow = PopupDialog()
        self.popupWindow.LoadDialog()
        self.popupWindow.SetCenterPosition()
        self.popupWindow.Hide()


    ## SelectPhase
    ##########################################################################################
    def SetLogoPhase(self):
        constInfo.PHASE_WINDOW = True
        net.Disconnect()

        import introLogo
        self.SetPhaseWindow(introLogo.LogoWindow(self))

    def SetLoginPhase(self):
        constInfo.PHASE_WINDOW = True
        if app.ENABLE_DISCORD_STUFF:
            app.SetTitle(localeInfo.APP_TITLE, localeInfo.APP_MINI_TITLE)
        net.Disconnect()

        import introLogin
        self.SetPhaseWindow(introLogin.LoginWindow(self))

    if app.NEW_SELECT_CHARACTER:
        def SameLogin_SetLoginPhase(self):
            constInfo.PHASE_WINDOW = True
            net.Disconnect()

            import introLogin
            introInst = introLogin.LoginWindow(self)
            self.SetPhaseWindow(introInst)
            introInst.SameLogin_OpenUI()

    def SetSelectEmpirePhase(self):
        constInfo.PHASE_WINDOW = True
        try:
            import introEmpire
            self.SetPhaseWindow(introEmpire.SelectEmpireWindow(self))
        except:
            import exception
            exception.Abort("networkModule.SetSelectEmpirePhase")

    def SetReselectEmpirePhase(self):
        constInfo.PHASE_WINDOW = True
        try:
            import introEmpire
            self.SetPhaseWindow(introEmpire.ReselectEmpireWindow(self))
        except:
            import exception
            exception.Abort("networkModule.SetReselectEmpirePhase")

    def SetSelectCharacterPhase(self):
        constInfo.PHASE_WINDOW = True
        if app.ENABLE_DISCORD_STUFF:
            app.SetTitle(localeInfo.APP_TITLE, localeInfo.APP_MINI_TITLE)
        try:
            localeInfo.LoadLocaleData()
            self.popupWindow.Close()
            if app.NEW_SELECT_CHARACTER:
                import New_introSelect
                self.SetPhaseWindow(New_introSelect.SelectCharacterWindow(self))
            else:
                import introSelect
                self.SetPhaseWindow(introSelect.SelectCharacterWindow(self))
        except:
            import exception
            exception.Abort("networkModule.SetSelectCharacterPhase")

    def SetCreateCharacterPhase(self):
        constInfo.PHASE_WINDOW = True
        try:
            import introCreate
            self.SetPhaseWindow(introCreate.CreateCharacterWindow(self))
        except:
            import exception
            exception.Abort("networkModule.SetCreateCharacterPhase")

    def SetGamePhase(self):
        constInfo.PHASE_WINDOW = True
        try:
            import game
            self.popupWindow.Close()
            self.SetPhaseWindow(game.GameWindow(self))
        except:
            raise
            import exception
            exception.Abort("networkModule.SetGamePhase")

    ################################
    # Functions used in python

    ## Login
    def Connect(self):
        if constInfo.KEEP_ACCOUNT_CONNETION_ENABLE:
            net.ConnectToAccountServer(self.addr, self.port, self.account_addr, self.account_port)
        else:
            net.ConnectTCP(self.addr, self.port)

        #net.ConnectUDP(IP, Port)

    def SetConnectInfo(self, addr, port, account_addr=0, account_port=0):
        self.addr = addr
        self.port = port
        self.account_addr = account_addr
        self.account_port = account_port

    def GetConnectAddr(self):
        return self.addr

    def SetLoginInfo(self, id, pwd):
        self.id = id
        self.pwd = pwd
        net.SetLoginInfo(id, pwd)

    def CancelEnterGame(self):
        pass

    ## Select
    def SetCharacterSlot(self, slot):
        self.slot=slot

    def GetCharacterSlot(self):
        return self.slot

    ## Empty
    def EmptyFunction(self):
        pass

    def SetLoadingPhase(self, isWarp):
        try:
            import introLoading
            phaseWindow = introLoading.LoadingWindow(self, isWarp)

            if isWarp:
                oldPhaseWindow = self.curPhaseWindow
                self.curPhaseWindow = phaseWindow

                if oldPhaseWindow:
                    oldPhaseWindow.Close()

                if self.curPhaseWindow:
                    self.curPhaseWindow.Open()
                else:
                    app.Exit()
            else:
                self.SetPhaseWindow(phaseWindow)
        except:
            import exception
            exception.Abort("networkModule.SetLoadingPhase")
