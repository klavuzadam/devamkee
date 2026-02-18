# coding: latin_1

import ui
#import uiWeb
import os

class Discord(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)

        #self.webWnd = uiWeb.WebWindow()
        #self.webWnd.LoadWindow()
        #self.webWnd.Hide()

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "WorldArd/discord_windows.py")
        except:
            import exception
            exception.Abort("Discord.LoadDialog.LoadScript")

        try:
            GetObject=self.GetChild
            self.gameButton = GetObject("discord_button")

        except:
            import exception
            exception.Abort("Discord.LoadDialog.BindObject")

        self.gameButton.SetEvent(ui.__mem_func__(self.__OnClickGame))

    def __OnClickGame(self):
        #net.CalendarOpen()
        os.system("start " + "https://discordapp.com/invite/vUTqEV4")
        #os.popen("start %s" % (url))
        #self.webWnd.Open("https://discordapp.com/invite/E6ZDP3M","Discord")
        
    def Destroy(self):
        self.ClearDictionary()