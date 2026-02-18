# coding: latin_1

import ui 
# import wndMgr
import player
import net

class OptionDialog(ui.ScriptWindow):

    def __init__(self): 
        self.isLoaded = False
        self.channels = 4 #configure from here!
        self.ChannelButtonList = []
        ui.ScriptWindow.__init__(self) 
        self.__LoadWindow()

    def __del__(self): 
        ui.ScriptWindow.__del__(self) 

    def Destroy(self):
        self.ChannelButtonList = []
        self.isLoaded = False
        # self.cancelButton = None        
        self.board = None

    def Show(self):
        self.__LoadWindow()
        for btn in self.ChannelButtonList:
            btn.Show()

        ui.ScriptWindow.Show(self)

    def Close(self):
        self.Hide()

    def __LoadWindow(self):
        if self.isLoaded:
            return

        self.isLoaded = True

        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/SwitchChannel.py")
            # self.cancelButton = self.GetChild("cancel")
            # self.cancelButton.SAFE_SetEvent(self.OnPressEscapeKey)

            self.board = self.GetChild("board")
            self.board.SetCloseEvent(ui.__mem_func__(self.Close))
            self.board.SetSize(180,(self.channels * 40)+35)

            baseChannel = self.GetChild("channel")
            baseChannel.Hide()
            baseX, baseY = baseChannel.GetLocalPosition()

            for i in xrange(0, self.channels):
                # Create button
                btn = ui.Button()
                btn.SetParent(self.board)
                btn.SetPosition(15, 30*i + baseY)
                btn.SetUpVisual("flag/1.tga")
                btn.SetOverVisual("flag/2.tga")
                btn.SetDownVisual("flag/3.tga")
                btn.SetDisableVisual("flag/3.tga")
                btn.SetText("Channel "+str(i+1))
                btn.Show()
                if str(net.GetServerInfo()[-1:]) == str(i + 1):
                    btn.Disable()
                # Store & set event
                self.ChannelButtonList.append(btn)
                self.ChannelButtonList[i].SAFE_SetEvent(self.__Channel_switch, i+1)
            
            self.Show()
        except:
            import exception
            exception.Abort("SwitchChannel.__LoadWindow - error")

    def __Channel_switch(self, ch):
        net.SendChatPacket("/channel " + str(ch))
        net.SetServerInfo("Realm2" + " - Ch "+str(ch))

    def OnPressEscapeKey(self):
        self.Close()