# coding: latin_1

import app
import localeInfo
import ui
import wndMgr
import uiToolTip
import grp
import net
from datetime import datetime as dt, timedelta

MONTH_NAMES = ["", localeInfo.JANUARY, localeInfo.FEBRUARY, localeInfo.MARCH, localeInfo.APRIL, localeInfo.MAY, localeInfo.JUNE, localeInfo.JULY, localeInfo.AUGUST, localeInfo.SEPTEMBER, localeInfo.OCTOBER, localeInfo.NOVEMBER, localeInfo.DECEMBER]
FONT_COLOR = grp.GenerateColor(1, 1, 0, 1.0)

def SecondToHHMMSS(seconds):
    seconds = seconds % (24 * 3600) 
    hour = seconds // 3600
    seconds %= 3600
    minutes = seconds // 60
    seconds %= 60
    return [hour, minutes, seconds]

def SHHMM(seconds):
    t = SecondToHHMMSS(seconds)

    hour = t[0]
    minutes = t[1]
    seconds = t[2]

    text = ""
    
    if hour >= 1:
        text += "%dh " % (hour)

    if minutes >= 1:
        text += "%dmin " % (minutes)

    if seconds >= 1:
        if hour >= 1 and minutes == 0:
            pass
        else:
            text += "%ds " % (seconds)

    return text

class EventNotificationItem(ui.ScriptWindow):
    def __init__(self, parent):
        ui.ScriptWindow.__init__(self)

        self.visibility = 0
        self.image = ""
        self.parent = parent
        self.startTimeStamp = 0
        self.endTimeStamp = 0
        self.lastUpdate = 0

        self.LoadWindow()


    def SetParentW(self, parent):
        self.parent = parent

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def Open(self):
        self.Show()

    def Close(self):
        self.Hide()

    def SetCurrentEvent(self, image, timeStamp, duration, vs = 1):
        self.image = image
        self.startTimeStamp = timeStamp
        self.endTimeStamp = timeStamp + duration
        self.SetVisibility(vs)
        self.lastUpdate = 0

    def SetVisibility(self, value):
        self.visibility = value
        
    def SetEvent(self, event):
        self.onEvent = event

    def __OnMouseLeftButtonDoubleClick(self):
        self.onEvent()

    def OnMouseLeftButtonDoubleClick(self):
        self.__OnMouseLeftButtonDoubleClick()

    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/EventNotificationItem.py")
        except:
            import exception
            exception.Abort("EventNotificationItem.LoadWindow.LoadObject")

        try:
            self.background = self.GetChild("Background")
            self.background.AddFlag("not_pick")
            self.background.SetEvent(self.__OnMouseLeftButtonDoubleClick)
            self.textLine = self.GetChild("TextLine")
            self.LeftTime = self.GetChild("LeftTime")
        except:
            import exception
            exception.Abort("EventNotificationItem.LoadWindow.BindObject")

    def OnUpdate(self):
        if not self.visibility:
            self.background.Hide()
            return

        if self.lastUpdate > app.GetGlobalTimeStamp():
            return
        self.lastUpdate = app.GetGlobalTimeStamp() + 1

        self.background.Show()
        IMG = "d:/ymir work/ui/calendar/events/%s" % self.image
        self.background.SetUpVisual(IMG)
        self.background.SetOverVisual(IMG)
        self.background.SetDownVisual(IMG)
        currentTimeStamp = app.GetGlobalTimeStamp()
        
        if currentTimeStamp < self.startTimeStamp:
            leftTime = self.startTimeStamp - currentTimeStamp
            text = localeInfo.EVENT_START_AT
        
        elif currentTimeStamp >= self.startTimeStamp and currentTimeStamp < self.endTimeStamp:
            leftTime = self.endTimeStamp - currentTimeStamp
            text = localeInfo.EVENT_END_AT

        else:
            self.parent.ForceUpdate()
            return

        self.textLine.SetText("%s" % (text))
        self.LeftTime.SetText("%s" % SHHMM(leftTime))

class EventCalendarItem(ui.ScriptWindow):
    def __init__(self, parent, index):
        ui.ScriptWindow.__init__(self)

        self.updated = 0
        self.index = index
        self.year = 0
        self.month = 0
        self.eventList = []
        self.imagesList = []
        self.lastUpdate = 0

        self.buttontooltip = None
        self.showButtonToolTip = False

        self.SetParent(parent)
        self.LoadWindow()

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def GetIndex(self):
        return self.index

    def ClearEvents(self):
        self.eventList = []
        self.imagesList = []
        self.updated = 0

    def AppendEvent(self, name,image,startAt, endAt,duration):
        self.eventList.append([name,image,startAt, endAt,duration])
        self.updated = 0

    def SetDate(self, year, month):
        self.year = year
        self.month = month

    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/EventCalendarItem.py")
        except:
            import exception
            exception.Abort("EventCalendarItem.LoadWindow.LoadObject")

        try:
            self.indexText = self.GetChild("IndexText")
            self.indexText.SetText(str(self.index))

            self.background = self.GetChild("Background")
            self.eventImage = self.GetChild("EventImage")
        except:
            import exception
            exception.Abort("EventCalendarItem.LoadWindow.BindObject")

        self.buttontooltip = uiToolTip.ToolTip()
        self.buttontooltip.ClearToolTip()

        self.eventImage.OnMouseOverIn = lambda : self.OverInToolTipButton()
        self.eventImage.OnMouseOverOut = lambda : self.OverOutToolTipButton()

    def Refresh(self):
        if self.updated:
            return

        self.updated = 1
        if self.eventList:
            self.background.LoadImage("d:/ymir work/ui/calendar/with_event.tga")
        else:
            self.background.LoadImage("d:/ymir work/ui/calendar/without_event.tga")
            self.eventImage.Hide()

    def Animation(self):
        if not self.eventList:
            return

        if not self.imagesList:
            for event in self.eventList:
                self.imagesList.append(event[1])

        currentTimeStamp = app.GetGlobalTimeStamp()
        if self.lastUpdate > currentTimeStamp:
            return

        self.lastUpdate = currentTimeStamp + 5

        image = self.imagesList.pop()
        self.eventImage.LoadImage("d:/ymir work/ui/calendar/events/%s" % (image))
        self.eventImage.Show()

    def OverInToolTipButton(self):
        pos_x, pos_y = wndMgr.GetMousePosition()
    
        self.buttontooltip.ClearToolTip()
        self.buttontooltip.SetThinBoardSize(11 + 200)
        self.buttontooltip.SetToolTipPosition(pos_x + 25, pos_y + (uiToolTip.ToolTip.TEXT_LINE_HEIGHT * 2))
        
        
        date = "%s-%s-%s" % (self.index, self.month, self.year)
        self.buttontooltip.AppendTextLine("Evento del dia %s" % date, 0xffffffff)
        self.buttontooltip.AppendSpace(5)

        for event in self.eventList:
            self.buttontooltip.AppendSpace(5)

            ## Event name
            self.buttontooltip.AppendTextLine("%s" % event[0], FONT_COLOR)

            ## Event time
            #tStart = dt.fromtimestamp(event[2])
            self.buttontooltip.AppendTextLine("Inicia a las: %s" % (event[2]), 0xffffffff)

            #tEnd = tStart + timedelta(seconds = event[3])
            self.buttontooltip.AppendTextLine("Termina a las: %s" % (event[3]), 0xffffffff)
            
            self.buttontooltip.AppendSpace(5)

            tDuration = localeInfo.EVENT_DURATION + SHHMM(event[4])
            self.buttontooltip.AppendTextLine(tDuration, 0xffffffff)
            
        self.buttontooltip.Show()
        self.showButtonToolTip = True
        
    def OverOutToolTipButton(self):
        self.buttontooltip.Hide()
        self.showButtonToolTip = False

    def ButtonToolTipProgress(self):
        if self.showButtonToolTip:
            pos_x, pos_y = wndMgr.GetMousePosition()
            self.buttontooltip.SetToolTipPosition(pos_x + 25, pos_y + (uiToolTip.ToolTip.TEXT_LINE_HEIGHT * 2) + self.buttontooltip.GetToolTopHeight())
    
    def OnUpdate(self):
        self.ButtonToolTipProgress()
        self.Animation()
        self.Refresh()

class EventCalendarWindow(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.currentTimeStamp = app.GetGlobalTimeStamp()
        self.LoadWindow()

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def Open(self, currentTimeStamp):
        self.currentTimeStamp = currentTimeStamp
        self.Refresh()
        self.Show()

    def Close(self):
        net.CalendarClose()
        self.Hide()

    def OnPressEscapeKey(self):
        self.Close()
        return True

    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/EventCalendarWindow.py")
        except:
            import exception
            exception.Abort("EventCalendarWindow.LoadWindow.LoadObject")

        try:
            self.titleBar = self.GetChild("TitleBar")
            self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

            self.titleName = self.GetChild("TitleName")
        except:
            import exception
            exception.Abort("CalendarWindow.LoadWindow.BindObject")

        self.calentarItems = []
        for row in range(4):
            for column in range(8):
                calentarItem = EventCalendarItem(self.GetChild("Background"), len(self.calentarItems) + 1)
                calentarItem.SetPosition(13 + (77 * column), 10 + (77 * row))

                self.calentarItems.append(calentarItem)

    def Refresh(self):
        dateTime = dt.fromtimestamp(self.currentTimeStamp)

        self.titleName.SetText(localeInfo.EVENT_CALENDAR_TITLE % MONTH_NAMES[dateTime.month])

        for item in self.calentarItems:
            if item.GetIndex() <= self.GetAmountDays(dateTime.year, dateTime.month):
                item.Show()
            else:
                item.Hide()

        for item in self.calentarItems:
            item.SetDate(dateTime.year, dateTime.month)

    def ClearEvents(self):
        for item in self.calentarItems:
            item.ClearEvents()

    def AppendEvent(self, day,name,image,startAt, endAt,duration):
        self.calentarItems[day - 1].AppendEvent(name.upper(), image, startAt, endAt, duration)

    def IsLeapYear(self, year):
        return (year % 4 == 0 and (year % 100 != 0 or year % 400 == 0))

    def GetAmountDays(self, year, month):
        if month != 2:
            return 31 if month % 2 == 0 else 30

        return 29 if IsLeapYear(year) else 28


"""
wndEventCalendar = EventCalendarWindow()

## START TEST
wndEventCalendar.ClearEvents()

wndEventCalendar.AppendEvent(1, "Evento Caja Luz de Luna 1", "alquimia.tga", 1633654036, 7200)
wndEventCalendar.AppendEvent(1, "Evento Caja Luz de Luna 2", "doble_jefes.tga", 1633654036, 7200)
wndEventCalendar.AppendEvent(1, "Evento Caja Luz de Luna 3", "doble_metin.tga", 1633654036, 7200)
wndEventCalendar.AppendEvent(1, "Evento Caja Luz de Luna 4", "drop.tga", 1633654036, 7200)
wndEventCalendar.AppendEvent(1, "Evento Caja Luz de Luna 5", "exp.tga", 1633654036, 7200)
wndEventCalendar.AppendEvent(1, "Evento Caja Luz de Luna 6", "luz_luna.tga", 1633654036, 7200)

wndEventCalendar.AppendEvent(2, "Evento Caja Luz de Luna 7", "exp.tga", 1633654036, 7200)
wndEventCalendar.AppendEvent(2, "Evento Caja Luz de Luna 8", "luz_luna.tga", 1633654036, 7200)
wndEventCalendar.AppendEvent(5, "Evento Caja Luz de Luna 9", "doble_metin.tga", 1633654036, 7200)
wndEventCalendar.AppendEvent(9, "Evento Caja Luz de Luna 10", "drop.tga", 1633654036, 7200)
wndEventCalendar.AppendEvent(9, "Evento Caja Luz de Luna 11", "exp.tga", 1633654036, 7200)
wndEventCalendar.AppendEvent(9, "Evento Caja Luz de Luna 12", "luz_luna.tga", 1633654036, 7200)

wndEventCalendar.AppendEvent(15, "Evento Caja Luz de Luna 13", "luz_luna.tga", 1633700874, 1633700874)
wndEventCalendar.AppendEvent(31, "Evento Caja Luz de Luna 13", "luz_luna.tga", 1633707874, 7200)
## END TEST

wndEventCalendar.Open(app.GetGlobalTimeStamp())
"""

class Example(ui.BoardWithTitleBar):
    def __init__(self):
        ui.BoardWithTitleBar.__init__(self)

        self.eventList = []
        self.tempEventList = []
        self.lastUpdate = 0

        self.LoadWindow()

    def __del__(self):
        ui.BoardWithTitleBar.__del__(self)

    def Close(self):
        self.notificationItem.Hide()
        self.Hide()

    def Open(self):
        self.notificationItem.Open()
        self.Show()

    def AppendEvent(self, image, timeStamp, duration):
        self.eventList.append([image, timeStamp, duration])

    def LoadWindow(self):
        self.SetTitleName("--")
        self.SetSize(300, 300)
        self.SetPosition(400, 400)
        self.SetCloseEvent(ui.__mem_func__(self.Close))
        #self.AddFlag("movable")


        self.notificationItem = EventNotificationItem(self)
        self.notificationItem.SetCurrentEvent("luz_luna.tga", 0, 0, 0)
        self.notificationItem.SetPosition(400, 400)
        self.notificationItem.SetEvent(self.OpenEventCalendar)
        self.notificationItem.Open()

    def OnUpdate(self):
        self.AnimationEvent()

    def ForceUpdate(self):
        self.lastUpdate = 0


    def ClearEvents(self):
        self.eventList = []
        self.tempEventList = []
        
    def OpenEventCalendar(self):
        self.SetTitleName("click!!!!")

    def AnimationEvent(self):
        currentTimeStamp = app.GetGlobalTimeStamp()
        if not self.eventList:
            self.notificationItem.SetVisibility(0)
            return

        if not self.tempEventList:
            for sevent in self.eventList:
                if sevent[1] >= currentTimeStamp:
                    self.tempEventList.append(sevent)
                elif currentTimeStamp >= sevent[1] and currentTimeStamp < sevent[1] + sevent[2]:
                    self.tempEventList.append(sevent)

        if not self.tempEventList:
            self.notificationItem.SetVisibility(0)
            return

        if self.lastUpdate > app.GetGlobalTimeStamp():
            return
        self.lastUpdate = app.GetGlobalTimeStamp() + 5

        currentEvent = self.tempEventList.pop()
        self.notificationItem.SetCurrentEvent(currentEvent[0], currentEvent[1], currentEvent[2])

"""
x = Example()
x.AppendEvent("luz_luna.tga", app.GetGlobalTimeStamp() + 2, 23)
x.AppendEvent("drop.tga", app.GetGlobalTimeStamp() + 2, 10)
#x.AppendEvent("exp.tga", app.GetGlobalTimeStamp() + 10, 5200)
x.Open()"""