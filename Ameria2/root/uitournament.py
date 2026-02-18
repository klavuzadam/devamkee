# coding: latin_1

import chat
import grp
import net
import app
import wndMgr
import uiCommon
import uiGuild
import uiToolTip
import ui
import constInfo
import localeInfo

def CalculateTimeLeft(iTime):
    A, B = divmod(iTime, 60)
    C, A = divmod(A, 60)
    return "%02d:%02d" % (A, B)    

class Window(ui.Window):
    def __init__(self):
        ui.Window.__init__(self)
        self.timeElapsed = 0
        self.Initialize()

    def __del__(self):
        ui.Window.__del__(self)

    def Initialize(self):
        self.background = ui.ExpandedImageBox()
        self.background.SetParent(self)
        self.background.LoadImage("d:/ymir work/ui/tournament/background.tga")
        self.background.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2, - 5)
        self.background.SetScale(0.9, 0.9)
        self.background.Show()

        self.online_A = ui.TextLine()
        self.online_A.SetParent(self)
        self.online_A.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 240, 27)
        self.online_A.SetFontName("Roman:20")
        self.online_A.SetPackedFontColor(0xff948b7d)
        self.online_A.SetHorizontalAlignLeft()
        self.online_A.Show()
        
        self.online_B = ui.TextLine()
        self.online_B.SetParent(self)
        self.online_B.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 500, 27)
        self.online_B.SetFontName("Roman:20")
        self.online_B.SetPackedFontColor(0xff948b7d)
        self.online_B.SetHorizontalAlignLeft()
        self.online_B.Show()
        
        self.membersDeadA = ui.TextLine()
        self.membersDeadA.SetParent(self)
        self.membersDeadA.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 180, 22)
        self.membersDeadA.SetFontName("Roman:20")
        self.membersDeadA.SetPackedFontColor(0xff948b7d)
        self.membersDeadA.SetHorizontalAlignLeft()
        self.membersDeadA.Show()
        
        self.membersDeadB = ui.TextLine()
        self.membersDeadB.SetParent(self)
        self.membersDeadB.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 565, 22)
        self.membersDeadB.SetFontName("Roman:20")
        self.membersDeadB.SetPackedFontColor(0xff948b7d)
        self.membersDeadB.SetHorizontalAlignLeft()
        self.membersDeadB.Show()
        
        self.memberLives = ui.TextLine()
        self.memberLives.SetParent(self)
        self.memberLives.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 374, 80)
        self.memberLives.SetFontName("Roman:20")
        self.memberLives.SetPackedFontColor(0xffff4719)
        self.memberLives.SetHorizontalAlignLeft()
        self.memberLives.Show()
        
        self.clock = ui.TextLine()
        self.clock.SetParent(self)
        self.clock.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 350, 22)
        self.clock.SetFontName("Roman:25")
        self.clock.SetPackedFontColor(0xff948b7d)
        self.clock.SetHorizontalAlignLeft()
        self.clock.Show()
        
        self.Hide()
        
    def Append(self, tokens):
        if constInfo.TOURNAMENT_WINDOW_IS_SHOWED < 1:
            self.timeElapsed = int(tokens[0]) + app.GetGlobalTimeStamp()

        self.online_A.SetText(tokens[1])
        self.online_B.SetText(tokens[2])        
        self.membersDeadA.SetText(tokens[3])
        self.membersDeadB.SetText(tokens[4])
        self.memberLives.SetText(tokens[5])
        constInfo.TOURNAMENT_WINDOW_IS_SHOWED = 1
        self.Show()

    def OnUpdate(self):
        iTimeRemaining = self.timeElapsed - app.GetGlobalTimeStamp()
        if (iTimeRemaining > 0):
            self.clock.SetText(("%s" % (CalculateTimeLeft(iTimeRemaining))))

    def Destroy(self):
        self.ClearDictionary()