# coding: latin_1

import ui
import dbg, localeInfo, event, uiScriptLocale
import app
import net as dr2
from uiUtils import Edit2 as Edit2

class PrivateShopSearch(ui.Window):

    ITEMS = {}
    LIST = []
    LIST2 = {}

    def __init__(self):
        ui.Window.__init__(self)
        self.LoadWindow()

    def __del__(self):
        ui.Window.__del__(self)

    def Close(self):
        self.board.Hide()

    def Open(self):
        self.board.Show()
        
    def Show(self):
        # ui.Window.Show(self)
        
        if self.board.IsShow():
            self.board.Hide()
        else:
            self.board.Show()

    def OnPressEscapeKey(self):
        self.board.Hide()
        return True
    
    def LoadWindow(self):
        self.selected = False
        self.text = ""
        self.board = ui.BoardWithTitleBar()
        self.board.SetSize(300, 280)
        self.board.SetCenterPosition()
        self.board.AddFlag("movable")
        self.board.AddFlag("float")
        self.board.SetTitleName(uiScriptLocale.SHOP_SEARCH_TITLE)
        self.board.SetCloseEvent(self.Close)
        self.board.Hide()
        
        self.thinboards, self.texts, self.buttons = {}, {}, {}
        
        # u = 0
        # for i in xrange(3):
            # self.thinboards[i] = ui.ThinBoard()
            # self.thinboards[i].SetParent(self.board)
            # self.thinboards[i].SetPosition(10,80+u)
            # self.thinboards[i].SetSize(170,30)
            # self.thinboards[i].Show()
            
            # self.texts[i] = ui.TextLine()
            # self.texts[i].SetParent(self.thinboards[i])
            # self.texts[i].SetPosition(10,12)
            # self.texts[i].Show()
            # u+=50
            
        for i in xrange(2):
            self.buttons[i] = ui.Button()
            self.buttons[i].SetParent(self.board)
            self.buttons[i].SetUpVisual("d:/ymir work/ui/public/large_button_01.sub")
            self.buttons[i].SetOverVisual("d:/ymir work/ui/public/large_button_02.sub")
            self.buttons[i].SetDownVisual("d:/ymir work/ui/public/large_button_03.sub")
            if i == 0:
                self.buttons[i].SetPosition(+107-(107/2)+15, self.board.GetHeight()-30)
            else:
                self.buttons[i].SetPosition(-107+(107/2)-15, self.board.GetHeight()-30)
            self.buttons[i].SetWindowHorizontalAlignCenter()
            self.buttons[i].SetText((uiScriptLocale.SHOP_SEARCH_SEND,uiScriptLocale.SHOP_SEARCH_RESET)[i])
            self.buttons[i].SetEvent(lambda x = i : self.event(x))
            self.buttons[i].Show()
    
        self.slot = ui.ImageBox()
        self.slot.SetParent(self.board)
        self.slot.SetPosition(0,50)
        self.slot.SetWindowHorizontalAlignCenter()
        self.slot.LoadImage("d:/ymir work/ui/public/parameter_slot_06.sub")
        self.slot.Show()
        
        self.name = ui.TextLine()
        self.name.SetParent(self.board)
        self.name.SetPosition(-107,30)
        self.name.SetWindowHorizontalAlignCenter()
        self.name.SetText(uiScriptLocale.SHOP_SEARCH_NAME)
        self.name.Show()
        
        self.input = ui.EditLine()
        self.input.SetParent(self.slot)
        self.input.SetPosition(5,5-2-2+1)
        self.input.SetSize(self.slot.GetWidth(),self.slot.GetHeight())
        self.input.SetMax(30)
        self.input.SetFontName("Tahoma:11")
        self.input.SetIMEFlag(0)
        self.input.SetText("")
        self.input.SetEscapeEvent(lambda : self.Show())
        self.input.Show()        
    
        self.base = ui.ThinBoard()
        self.base.SetParent(self.board)
        self.base.SetSize(self.slot.GetWidth(),143)
        self.base.SetPosition(0,50+30)
        self.base.SetWindowHorizontalAlignCenter()
        self.base.Show()
        
        self.fileListBox = ui.ListBoxEx()
        self.fileListBox.SetParent(self.base)
        self.fileListBox.SetPosition(10, 10)
        self.fileListBox.SetViewItemCount(6)
        self.fileListBox.SetSelectEvent(lambda x=0: self.SelectItem(x))
        self.fileListBox.Show()
        
        self.ScrollBar = ui.ScrollBar()
        self.ScrollBar.SetParent(self.board)
        self.ScrollBar.SetPosition(self.board.GetWidth()-32, 50+30)
        self.ScrollBar.SetScrollBarSize(143)
        self.ScrollBar.Show()
        self.fileListBox.SetScrollBar(self.ScrollBar)
        

        self.precio = ui.TextLine()
        self.precio.SetParent(self.board)
        self.precio.SetPosition(-107,self.board.GetHeight()-55)
        self.precio.SetWindowHorizontalAlignCenter()
        self.precio.SetText(uiScriptLocale.SHOP_SEARCH_PRICE)
        self.precio.Show()
        
        self.textedit=Edit2(self.board,"0",self.board.GetWidth()-155,self.board.GetHeight()-57,100,250,FALSE,4,30,True)
        self.textedit.CanEdit(False)
            
        self.icon = ui.ImageBox()
        self.icon.SetParent(self.board)
        self.icon.SetPosition(self.board.GetWidth()-155-20,self.board.GetHeight()-55)
        self.icon.LoadImage("d:/ymir work/ui/game/windows/money_icon.sub")
        self.icon.Show()
        
        self.LoadItems()
    
    def GetPrice(self):
        price = self.precio.GetText()
        b = price.replace(".","")
        return str(b)

    def GetNewPrice(self):
        if self.SelectSpace() == 1:
            return 10000
        return 0

    def SelectSpace(self):
        if self.selected == False:
            return 0
        else:
            return 1

    def SelectItem(self,x):
        self.selected = True
        lista_items = self.fileListBox.GetSelectedItem()
        item = lista_items.GetText()
        if self.SelectSpace() == 0:
            return
        else:
            self.text = item
            self.input.SetText(item)
            self.LIST2 = {}
            self.fileListBox.RemoveAllItems()
            

    def event(self, arg):
        if arg == 0:
            if len(self.input.GetText()) > 0:
                try:
                    item1 = str(self.ITEMS[self.input.GetText()])
                    if item1 == "" or item1 == "0":
                        item1 = "0"
                    dr2.SendChatPacket("/search_item_privateshop "+item1+" 0 0 "+self.GetPrice())
                except:
                    pass
        else:
            dr2.SendChatPacket("/delete_item_privateshop")
            self.input.SetText("")
            self.selected = False
    

    def LoadItems(self):
        self.ITEMS = {}
        self.LIST = []
        self.ITEMS[""] = 0
        self.fileListBox.RemoveAllItems()
        lines = open(app.GetLocalePath()+"/item_names.txt","r").readlines()
        split = app.GetLocalePath().split("/")[1]
        for line in lines:
            tokens = line.split("\t")
            if tokens[0] != "VNUM" and tokens[0] != "1" and tokens[0] != "2" and tokens[0] != "3":
                try:
                    vnum = int(tokens[0])
                    name = tokens[1].replace("\n","")
                    if not self.ITEMS.has_key(name):
                        self.LIST.append(name)
                        self.ITEMS[name] = vnum
                except:
                    pass


    def CheckItems(self):
        self.fileListBox.RemoveAllItems()
        for i in xrange(len(self.LIST)):
            if self.LIST[i].find(self.text) != -1:
                if not self.LIST2.has_key(self.LIST[i]):
                    self.LIST2[self.LIST[i]] = 1
                    self.fileListBox.AppendItem(Item(self.LIST[i]))
        

    def OnUpdate(self):
        if self.selected == False:
            self.textedit.SetText("0")

        if len(self.LIST2) >=6:
            self.ScrollBar.SetMiddleBarSize(float(6)/float(len(self.LIST2)))
            self.ScrollBar.Show()
        else:
            self.ScrollBar.Hide()
        if self.input.GetText() != self.text:
            self.selected = False
            self.text = self.input.GetText()
            self.LIST2 = {}
            if len(self.text) > 0:
                self.CheckItems()
            else:
                self.fileListBox.RemoveAllItems()

        if self.SelectSpace() == 1 and self.selected == True:
            self.textedit.SetText(localeInfo.NumberToMoneyString(self.GetNewPrice()).replace(" Yang", ""))

class Item(ui.ListBoxEx.Item):
    def __init__(self, fileName):
        ui.ListBoxEx.Item.__init__(self)
        self.canLoad=0
        self.text=fileName
        self.textLine=self.__CreateTextLine(fileName[:40])

    def __del__(self):
        ui.ListBoxEx.Item.__del__(self)

    def GetText(self):
        return self.text

    def SetSize(self, width, height):
        ui.ListBoxEx.Item.SetSize(self, 6*len(self.textLine.GetText()) + 4, height)

    def __CreateTextLine(self, fileName):
        textLine=ui.TextLine()
        textLine.SetParent(self)
        textLine.SetPosition(0, 0)
        textLine.SetText(fileName)
        textLine.Show()
        return textLine

# x=PrivateShopSearch().Show()