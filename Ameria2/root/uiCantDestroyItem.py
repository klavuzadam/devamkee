# coding: latin_1

import ui
import app
import localeInfo

class CantDestroyItemDialog(ui.Board):
    def __init__(self):
        ui.Board.__init__(self)
        self.__LoadWindow()
        self.timeStamp = 0
        self.end = False

    def __del__(self):
        ui.Board.__del__(self)

    def Open(self):
        self.timeStamp = app.GetTime() + 2 # 5s
        self.Show()

    def Close(self):
        if not self.end:
            return

        self.end = False
        self.Hide()

    def __LoadWindow(self):
        self.SetSize(280, 90)
        self.SetCenterPosition()
        self.AddFlag("float")
        self.AddFlag("animate")
        #self.SetCloseEvent(ui.__mem_func__(self.Close))
        self.Hide()

        self.textline = ui.TextLine()
        self.textline.SetParent(self)
        self.textline.SetPosition(280 / 2, 25)
        self.textline.SetHorizontalAlignCenter()
        self.textline.SetText("This item cannot be destroyed.")
        self.textline.Show()

        self.btn = ui.Button()
        self.btn.SetParent(self)
        self.btn.SetPosition(50, 45)
        self.btn.SetUpVisual("d:/ymir work/ui/public/xlarge_button_01.sub")
        self.btn.SetOverVisual("d:/ymir work/ui/public/xlarge_button_02.sub")
        self.btn.SetDownVisual("d:/ymir work/ui/public/xlarge_button_03.sub")
        self.btn.SetText("")
        self.btn.SetEvent(ui.__mem_func__(self.Close))
        self.btn.Show()

        self.leftTime = ui.TextLine()
        self.leftTime.SetParent(self.btn)
        self.leftTime.SetPosition(90, 5)
        self.leftTime.SetHorizontalAlignCenter()
        self.leftTime.SetText(localeInfo.UI_OK)
        self.leftTime.Show()

    def OnUpdate(self):
        endtime = self.timeStamp - app.GetTime()
        if self.end:
            return


        if endtime >= 0:
            self.leftTime.SetText("%s (%.1f)" % (localeInfo.UI_OK, endtime))
            self.btn.SetUpVisual("d:/ymir work/ui/public/xlarge_button_03.sub")
        else:
            self.leftTime.SetText(localeInfo.UI_OK)
            self.btn.SetUpVisual("d:/ymir work/ui/public/xlarge_button_01.sub")
            self.end = True


        """if endtime > 0:
            self.btn.SetText("Bien ({0:.1f})".format(endtime))
            self.btn.SetUpVisual("d:/ymir work/ui/public/xlarge_button_03.sub")
            self.btn.Disable()
        else:
            self.btn.SetText("Bien")
            self.btn.SetUpVisual("d:/ymir work/ui/public/xlarge_button_01.sub")
            self.btn.Enable()"""

# x = CantDestroyItemDialog()
# x.Open()
