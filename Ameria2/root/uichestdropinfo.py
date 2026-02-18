# coding: latin_1

#blackdragonx61 (28.11.21)
import ui
import item
import uiToolTip
import player
import net

class ChestDropInfoWindow(ui.ScriptWindow):
    DROP_SLOT_SIZE = 5 * 8
    ITEM = 0
    COUNT = 1

    def __init__(self) :
        ui.ScriptWindow.__init__(self)
        self.isLoaded = False
        self.ItemVnum = -1
        self.PageCount = 0
        self.CurrentPage = 0
        self.DropDict = dict()
        self.MainItemSlot = None
        self.DropItemSlot = None
        self.prevButton = None
        self.nextButton = None
        self.currentPageText = None
        self.tooltipitem = None
        self.itemCell = [-1, -1]
        self.itemCount = 0

    def __del__(self) :
        ui.ScriptWindow.__del__(self)
        self.DropDict = None
        self.MainItemSlot = None
        self.DropItemSlot = None
        self.prevButton = None
        self.nextButton = None
        self.currentPageText = None
        self.tooltipitem = None
        self.itemCell = 0
        self.itemCount = 0
        
    def __LoadWindow(self):
        if self.isLoaded:
            return

        self.isLoaded = True

        # script
        try:
            self.__LoadScript("UIScript/ChestDropInfoWindow.py")
        except:
            import exception
            exception.Abort("ChestDropInfoWindow.__LoadWindow.__LoadScript")

        # object
        try:
            self.__BindObject()
        except:
            import exception
            exception.Abort("ChestDropInfoWindow.__LoadWindow.__BindObject")

        # event
        try:
            self.__BindEvent()
        except:
            import exception
            exception.Abort("ChestDropInfoWindow.__LoadWindow.__BindEvent")

    def __LoadScript(self, fileName):
        pyScrLoader = ui.PythonScriptLoader()
        pyScrLoader.LoadScriptFile(self, fileName)

    def __BindObject(self):
        self.MainItemSlot        = self.GetChild("main_item_slot")
        self.DropItemSlot        = self.GetChild("drop_item_slot")
        self.prevButton            = self.GetChild("prev_button")
        self.nextButton            = self.GetChild("next_button")
        self.currentPageText    = self.GetChild("CurrentPage")

    def __BindEvent(self):
        self.GetChild("board").SetCloseEvent(ui.__mem_func__(self.Close))
        
        self.MainItemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInMainItemSlot))
        self.MainItemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
        
        self.GetChild("open_btn").SetEvent(ui.__mem_func__(self.ClickOpenMultiChest))
        
        self.GetChild("count_controller").SetEvent(ui.__mem_func__(self.OnChangeCount))

        self.DropItemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInDropItemSlot))
        self.DropItemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

        self.prevButton.SetEvent(ui.__mem_func__(self.SetPage), -1)
        self.nextButton.SetEvent(ui.__mem_func__(self.SetPage), +1)
    
    def UpdateItems(self):
        for i in range(ChestDropInfoWindow.DROP_SLOT_SIZE):
            self.DropItemSlot.ClearSlot(i)

        if self.CurrentPage in self.DropDict:
            for pos in self.DropDict[self.CurrentPage]:
                data = self.DropDict[self.CurrentPage][pos]
                self.DropItemSlot.SetItemSlot(pos, data[ChestDropInfoWindow.ITEM], data[ChestDropInfoWindow.COUNT])
        
        self.DropItemSlot.RefreshSlot()

    def ClickOpenMultiChest(self):
        pos = self.GetChild("count_controller").GetSliderPos()*float(self.itemCount)
        value = 1.0/float(self.itemCount)
        realpos = (value*pos)*float(self.itemCount)
        net.SendChatPacket("/chest_drop %d %d %d"%(self.itemCell[0], self.itemCell[1], int(realpos)))
        self.Close()

    def OnChangeCount(self):
        pos = self.GetChild("count_controller").GetSliderPos()*float(self.itemCount)
        value =1.0/float(self.itemCount)
        realpos = (value*pos)*float(self.itemCount)
        self.GetChild("countSliderText").SetText("%d/%d"%(int(realpos),self.itemCount))

    def UpdateCountSlider(self, itemWindow, itemCell):
        if itemCell == -1:
            return
        self.itemCell = [itemWindow, itemCell]
        self.itemCount = player.GetItemCount(*self.itemCell)
        if self.itemCount > 1000:
            self.itemCount = 1000

        slider = self.GetChild("count_controller")
        sliderText = self.GetChild("countSliderText")

        # if self.itemCount <= 1:
            # slider.Hide()
            # sliderText.Hide()
            # self.GetChild("open_btn").Hide()
            # return
        # else:
            # self.GetChild("open_btn").Show()
            # slider.Show()
            # sliderText.Show()

        value =1.0/float(self.itemCount)
        pos = value*1.0
        slider.SetSliderPos(pos)
        sliderText.SetText("1/%d"%self.itemCount)

    def SetTooltipItem(self, tooltipItem):
        self.tooltipitem = tooltipItem
    def SetPage(self, page):
        nextpage = page + self.CurrentPage
        if 0 <= nextpage <= self.PageCount:
            self.CurrentPage = nextpage
            self.currentPageText.SetText(str(self.CurrentPage + 1))
            self.UpdateItems()
    
    def SetUp(self, itemVnum):
        self.ItemVnum = itemVnum
        self.MainItemSlot.SetItemSlot(0, self.ItemVnum, 0)
        self.MainItemSlot.RefreshSlot()

        (self.PageCount, DropList) = item.GetDropInfo(self.ItemVnum)

        self.DropDict.clear()
        for i in range(self.PageCount + 1):
            self.DropDict[i] = dict()

        for page, pos, vnum, count in DropList:
            self.DropDict[page][pos] = (vnum, count)

        self.CurrentPage = 0
        self.SetPage(0)
    
    def Open(self, itemVnum, itemWindow, itemCell):
        #if self.IsShow():
        #    return

        self.__LoadWindow()
        self.SetUp(itemVnum)
        self.UpdateCountSlider(itemWindow, itemCell)
        self.SetCenterPosition()
        self.SetTop()
        self.Show()
    
    def OverInMainItemSlot(self, slotIndex):
        if self.tooltipitem:
            self.tooltipitem.SetItemToolTip(self.ItemVnum)

    def OverInDropItemSlot(self, slotIndex):
        if self.tooltipitem:
            if self.CurrentPage in self.DropDict:
                if slotIndex in self.DropDict[self.CurrentPage]:
                    data = self.DropDict[self.CurrentPage][slotIndex]
                    self.tooltipitem.SetItemToolTip(data[ChestDropInfoWindow.ITEM])

    def OverOutItem(self):
        if self.tooltipitem:
            self.tooltipitem.HideToolTip()
            self.tooltipitem.ClearToolTip()
    
    def Close(self):
        self.OverOutItem()
        self.Hide()

    def OnPressEscapeKey(self):
        self.Close()
        return True