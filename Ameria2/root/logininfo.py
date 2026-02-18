# coding: latin_1

import app
import ui
import wndMgr

class TestWindow(ui.ScriptWindow):
    def __init__(self):
        ui.Window.__init__(self)
        self.wndLoader = None

    def __del__(self):
        ui.Window.__del__(self)
        if self.wndLoader:
            del self.wndLoader
        self.wndLoader = None

    def Open(self):
        wndLoader = ui.Bar()
        wndLoader.SetParent(self)
        wndLoader.SetColor(0x0A000000)
        wndLoader.SetPosition(0, 0)
        wndLoader.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
        wndLoader.Show()

        wndLoader.Animation = ui.AniImageBox()
        wndLoader.Animation.SetParent(wndLoader)
        for i in range(15):
            wndLoader.Animation.AppendImage("d:/ymir work/ui/loading_img/%d.png" % (i + 1))
        wndLoader.Animation.SetWindowHorizontalAlignCenter()
        wndLoader.Animation.SetWindowVerticalAlignCenter()
        wndLoader.Animation.SetDelay(2)
        wndLoader.Animation.Show()

        self.wndLoader = wndLoader
        self.Show()

    def Close(self):
        self.Hide()

q = TestWindow()
q.Open()
