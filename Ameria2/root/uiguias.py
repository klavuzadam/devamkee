# coding: latin_1

import ui
#import uiWeb
import os

class Guias(ui.ScriptWindow):
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
            pyScrLoader.LoadScriptFile(self, "WorldArd/guias_windows.py")
        except:
            import exception
            exception.Abort("Guias.LoadDialog.LoadScript")

        try:
            GetObject=self.GetChild
            self.gameButton = GetObject("guias_button")

        except:
            import exception
            exception.Abort("Guias.LoadDialog.BindObject")

        self.gameButton.SetEvent(ui.__mem_func__(self.__OnClickGame))

    def __OnClickGame(self):
        #net.CalendarOpen()
        os.system("start " + "https://www.mancos.games/forums/forum/5-guias/")
        #os.popen("start %s" % (url))
        #self.webWnd.Open("https://Guiasapp.com/invite/E6ZDP3M","Guias")
        
    def Destroy(self):
        self.ClearDictionary()