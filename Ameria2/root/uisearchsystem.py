# coding: latin_1

import ui
import net
import wndMgr
import item
import chat
import uiToolTip
import ime
import grp
import localeInfo

T_W = 10
T_W = 10
SKILLS=[1, 2, 3, 4, 5, 16, 17, 18, 19, 20, 122, 123, 121, 124, 125, 129, 130, 131, 137, 138, 139, 140, 31, 32, 33, 34, 35, 46, 47, 48, 49, 50, 61, 62, 63, 64, 65, 66, 76, 77, 78, 79, 80, 81,91, 92, 93, 94, 95, 96, 106, 107, 108, 109, 110, 111]
SKILL_BOOKS=  []
ITEM_LIST=item.GetNames()
SKILL_BOOK_NAMES={
    50300:localeInfo.TOOLTIP_SKILLBOOK_NAME,
    70037:localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME,
    # 70055:localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME,
}

class UiSearchSystem(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.UI={}
        self.search_vnum=0
        self.search_book=0
        self.curPage = 1
        self.MaxPage = T_W
        self.show_all = False
        self.toolTipItems = None
        self.items=[]
        self.attr = []
        self.gold = []
        self.id = []
        self.lst2=[]

    def __del__(self):
        ui.ScriptWindow.__del__(self)
        self.board.Hide()

    def BindToolTipItem(self,tooltip):
        self.toolTipItems = tooltip

    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "d:/ymir work/privateshopsearchdialog.py")
        except:
            import exception
            exception.Abort("UiSearchSystemElement.LoadWindow")

        try:
            self.board = self.GetChild("board")
            self.item_name = self.GetChild("ItemNameValue")
            self.item_gold = self.GetChild("ItemGoldValue")
            self.search = self.GetChild("SearchButton")
            self.prev_button = self.GetChild("prev_button")
            self.last_prev_button = self.GetChild("last_prev_button")
            self.next_button = self.GetChild("next_button")
            self.last_next_button = self.GetChild("last_next_button")
            self.acept_button = self.GetChild("AceptButton")
            self.all_items = self.GetChild("AllFunctionItems")
            self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))

        except:
            import exception
            exception.Abort("UiSearchSystemElement.LoadWindow")

        self.prev_button.SetEvent(self.__OnClickArrow, 'MOSTBOUGHT_RIGHT')
        self.last_prev_button.SetEvent(self.__OnClickArrow, 'MOSTBOUGHT_RIGHT_LAST')
        self.next_button.SetEvent(self.__OnClickArrow, 'MOSTBOUGHT_LEFT')
        self.last_next_button.SetEvent(self.__OnClickArrow, 'MOSTBOUGHT_LEFT_LAST')
        self.search.SetEvent(self.__OnSearch)
        self.acept_button.SetEvent(self.__AceptButton)
        self.all_items.SetEvent(self.__AllItemsButton)

        self.UI["list_names"] = DropDown(self)
        self.UI["list_names"].OnChange=self.OnChange
        self.UI["list_names"].SetPosition(15,60)
        self.UI["list_names"].Hide()

        self.UI["nameEdit"]=self.item_name
        self.UI["nameEdit"].OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdate)
        self.UI["nameEdit"].SetReturnEvent(ui.__mem_func__(self.__OnHideList))
        self.UI["nameEdit"].SetEscapeEvent(ui.__mem_func__(self.__OnHideList))

        self.item_gold.SetEscapeEvent(ui.__mem_func__(self.Close))

        self.Box = UiSearchSystemBox()
        self.Box.Open(self.board,137,62)
        
        self.Box.Hide()

        self.prev_button.Hide()
        self.last_prev_button.Hide()
        self.next_button.Hide()
        self.last_next_button.Hide()

    def Destroy(self):
        self.ClearDictionary()
        self.items = []
        self.attr = []
        self.gold = []
        self.lst2=[]
        self.Close()
        self.__del__()

    def Information(self, id, vnum, gold, attr1, val1, attr2, val2, attr3, val3, attr4, val4, attr5, val5, attr6, val6, attr7, val7):
        self.id.append(id)
        self.items.append(vnum)
        self.gold.append(gold)
        self.attr.append([attr1, val1, attr2, val2, attr3, val3, attr4, val4, attr5, val5, attr6, val6, attr7, val7])


    def __OnValueUpdate(self):
        ui.EditLine.OnIMEUpdate(self.UI["nameEdit"])
        val=self.UI["nameEdit"].GetText()
        if len(val)>0:
            self.UI["list_names"].Clear()
            f=0
            n=[]
            for it in ITEM_LIST:
                vnum,name=it["vnum"],it["name"]
                if f==10:
                    break
                if vnum in SKILL_BOOK_NAMES.keys():
                    continue
                if len(name)>=len(val) and name[:len(val)].lower() == val.lower():
                    self.UI["list_names"].AppendItem(name,vnum)
                    f+=1
            if f==0:
                for book in SKILL_BOOKS:
                    if f==10:
                        break
                    vnum,name,skill=book["vnum"],book["name"],book["skill"]
                    if len(name)>=len(val) and name[:len(val)].lower() == val.lower():
                        self.UI["list_names"].AppendItem(name,vnum,skill)
                        f+=1
            
            
            if f>0:
                if self.UI["list_names"].dropped==0:
                    self.UI["list_names"].Clear()
                    self.UI["list_names"].ExpandMe()
                self.UI["list_names"].Show()
                return

        self.__OnHideList()

    def __OnHideList(self):
        self.UI["list_names"].dropped=0
        self.UI["list_names"].Clear()
        self.UI["list_names"].Hide()

        #if not self.UI["list_names"].IsShow():
            #self.Close()


    def OnChange(self):
        self.search_vnum=self.UI["list_names"].DropList.GetSelectedItem().value
        self.search_book=self.UI["list_names"].DropList.GetSelectedItem().skill
        name=""
        if not self.search_book:
            for it in ITEM_LIST:
                if int(it["vnum"]) == self.search_vnum:
                    name=it["name"]
                    break
        else:
            for book in SKILL_BOOKS:
                if int(book["vnum"]) == self.search_vnum and int(book["skill"]) == self.search_book:
                    name=book["name"]
                    break
                    
        self.UI["nameEdit"].SetText(str(name))
        self.UI["list_names"].Clear()
        self.UI["list_names"].Hide()
        ime.SetCursorPosition(len(name)+1)

    def Page(self):
        self.Box.Hide()        
        self.Box.ClearList()
        self.Box.ClearSelect()

        self.Box.BindToolTipItem(self.toolTipItems)

        if self.show_all == False:
            for key in self.items:
                if key not in self.lst2:
                    self.lst2.append(key)

            for a in xrange(min(self.MaxPage, len(self.lst2) - self.curPage * self.MaxPage +self.MaxPage)):
                curId = self.id[a + (self.curPage - 1)*self.MaxPage]
                curVnum = self.lst2[a + (self.curPage - 1)*self.MaxPage]
                curGold = self.gold[a + (self.curPage - 1)*self.MaxPage]
                curGold2 = self.item_gold.GetText()
                curCount = self.Count(curVnum, self.items)
                curAttr = self.attr[a + (self.curPage - 1)*self.MaxPage]
                self.Box.SetContent(curId,curVnum,a,curCount,curGold,curGold2,curAttr,self.show_all)
                self.Box.Show()
        else:
            for a in xrange(min(self.MaxPage, len(self.items) - self.curPage * self.MaxPage +self.MaxPage)):
                curId = self.id[a + (self.curPage - 1)*self.MaxPage]
                curVnum = self.items[a + (self.curPage - 1)*self.MaxPage]
                curGold = self.gold[a + (self.curPage - 1)*self.MaxPage]
                curGold2 = self.item_gold.GetText()
                curCount = 1
                curAttr = self.attr[a + (self.curPage - 1)*self.MaxPage]
                self.Box.SetContent(curId,curVnum,a,curCount,curGold,curGold2,curAttr,self.show_all)
                self.Box.Show()

        if self.show_all == False:
            if self.curPage *self.MaxPage >= len(self.lst2):
                self.next_button.Hide()
                self.last_next_button.Hide()
            else:
                self.next_button.Show()
                self.last_next_button.Show()

            if self.curPage > 1:
                self.prev_button.Show()
                self.last_prev_button.Show()
            else:
                self.prev_button.Hide()
                self.last_prev_button.Hide()
        else:
            if self.curPage *self.MaxPage >= len(self.items):
                self.next_button.Hide()
                self.last_next_button.Hide()
            else:
                self.next_button.Show()
                self.last_next_button.Show()

            if self.curPage > 1:
                self.prev_button.Show()
                self.last_prev_button.Show()
            else:
                self.prev_button.Hide()
                self.last_prev_button.Hide()


    def __OnClickArrow(self, arrow):
        if self.show_all == False:
            Left_Last = self.WFunction(float(len(self.lst2))/float(self.MaxPage))
        else:
            Left_Last = self.WFunction(float(len(self.items))/float(self.MaxPage))

        if arrow == 'MOSTBOUGHT_LEFT':
            self.curPage += 1
            self.Page()
        elif arrow == 'MOSTBOUGHT_LEFT_LAST':
            self.curPage = Left_Last
            self.Page()
        elif arrow == 'MOSTBOUGHT_RIGHT':
            self.curPage -= 1
            self.Page()
        elif arrow == 'MOSTBOUGHT_RIGHT_LAST':
            self.curPage = 1
            self.Page()

    def WFunction(self, num):
        if (num + 1) != int(num+1):
            return int(num+1)
        else:
            return int(num)

    def Count(self, elemento, lista):
        veces = 0
        for i in lista:
            if elemento == i:
                veces += 1
        return veces

    def __OnSearch(self):

        item_name = self.item_name.GetText()
        item_gold = self.item_gold.GetText()
        if item_gold == "":
            chat.AppendChat(1, "Introdu preþul !")
            return

        self.items=[]
        self.attr = []
        self.gold = []
        self.id = []
        self.lst2=[]
        self.curPage = 1

        #net.SendPrivateSearch(str(item_name),int(item_gold))
        net.SendChatPacket("/search_item %d %s" % (self.search_vnum,item_gold))

    def __AceptButton(self):
        self.Box.ClickEvent()

    def __AllItemsButton(self):
        if self.show_all == False:
            self.show_all = True
        else:
            self.show_all = False

        self.Page()

    def Close(self):
        self.Box.Hide()        
        self.Box.ClearList()
        self.Box.ClearSelect()
        self.Hide()
    
    def OnPressEscapeKey(self):
        self.Close()
        return True

    def OnPressExitKey(self):
        self.Close()
        return True

class UiSearchSystemBox(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.isLoaded = FALSE
        self.toolTipItems = {}
        for i in xrange(0,T_W):
            self.toolTipItems[i] = None
        self.vnum = 0
        self.index = 0
        self.gold = 0
        self.item_bg = {}
        self.item_name = {}
        self.item_count = {}
        self.list_items = []
        self.list_attr = []
        self.list_id = []
        self.list_gold = []
        self.index_select = None
        self.index_over = 0

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def BindToolTipItem(self,tooltip):
        for i in xrange(0,T_W):
            self.toolTipItems[i] = tooltip

    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/privateshopsearchdialogitems.py")
        except:
            import exception
            exception.Abort("UiSearchSystemElement.LoadWindow")

        try:
            for i in xrange(0,T_W):
                self.item_bg[i] = self.GetChild("PrivateSearchBoxItemsBG%s"%i)
                self.item_name[i] = self.GetChild("PrivateSearchBoxItemsName%s"%i)
                self.item_count[i] = self.GetChild("PrivateSearchBoxItemsCount%s"%i)
                self.item_bg[i].SetEvent(self.SelectItem,i)
                self.item_bg[i].Hide()
        except:
            import exception
            exception.Abort("UiSearchSystemElement.LoadWindow")

        self.isLoaded = TRUE

    def Open(self, parent, x, y):
        if FALSE == self.isLoaded:
            self.LoadWindow()

        self.SetParent(parent)
        self.SetPosition(x,y)
        self.Show()

    def SetContent(self, id, vnum, index, count, gold, gold2, attr, show_attr):
        self.vnum = int(vnum)
        self.index = index
        self.gold = int(gold2)
        self.all_items_attr = show_attr

        item.SelectItem(int(vnum))

        self.item_name[index].SetText(item.GetItemName())
        self.item_count[index].SetText(str(count))
        self.item_bg[index].Show()
        self.list_items.append(vnum)
        self.list_attr.append(attr)
        self.list_gold.append(gold)
        self.list_id.append(id)

    def SelectItem(self,index):
        for i in xrange(0,T_W):
            self.item_bg[i].SetUp()
        self.item_bg[index].Down()
        self.index_select = index

    def ClearSelect(self):
        self.index_select = None

    def ClickEvent(self):
        if self.index_select == None:
            chat.AppendChat(1, "Selectaþi o opþiune !")
            return
        
        net.SendChatPacket("/search_shop %s %s %s %d" % (self.list_id[self.index_select], self.list_items[self.index_select], self.gold, self.all_items_attr))

    def ClearList(self):
        for i in xrange(0,T_W):
            self.item_bg[i].Hide()
            self.item_bg[i].SetUp()
            self.list_items = []
            self.list_attr = []
            self.list_id = []

    def ShowItemTool(self,index):
        if self.toolTipItems[index]:
            self.toolTipItems[index].ClearToolTip()
            if self.all_items_attr == False:
                self.toolTipItems[index].AddSearchItemData(int(self.list_items[self.index_over]),-1)
            else:
                self.toolTipItems[index].AddSearchItemData(int(self.list_items[self.index_over]),int(self.list_gold[self.index_over]),
                    [(int(self.list_attr[self.index_over][0]),int(self.list_attr[self.index_over][1])),
                        (int(self.list_attr[self.index_over][2]),int(self.list_attr[self.index_over][3])),
                        (int(self.list_attr[self.index_over][4]),int(self.list_attr[self.index_over][5])),
                        (int(self.list_attr[self.index_over][6]),int(self.list_attr[self.index_over][7])),
                        (int(self.list_attr[self.index_over][8]),int(self.list_attr[self.index_over][9])),
                        (int(self.list_attr[self.index_over][10]),int(self.list_attr[self.index_over][11])),
                        (int(self.list_attr[self.index_over][12]),int(self.list_attr[self.index_over][13]))])

    def HideItemTool(self,index):
        if self.toolTipItems[index]:
            self.toolTipItems[index].Hide()

    def OnUpdate(self):
        for i in xrange(0,T_W):
            if self.item_bg[i].IsIn():
                self.index_over = i
                self.ShowItemTool(i)
            else:
                self.HideItemTool(i)
                
class DropDown(ui.Window):
    dropped  = 0
    dropstat = 0
    width = 0
    height = 0
    maxh = 30
    OnChange = None
    class Item(ui.Window):
        TEMPORARY_PLACE = 0
        width = 0
        height = 0
        def __init__(self,parent, text,value=0,skill=0):
            ui.Window.__init__(self)
            self.textBox=ui.MakeTextLine(self)
            self.textBox.SetText(text)
            self.value = int(value)
            self.skill = int(skill)

        def __del__(self):
            ui.Window.__del__(self)

        def SetParent(self, parent):
            ui.Window.SetParent(self, parent)
            self.parent=parent

        def OnMouseLeftButtonDown(self):
            self.parent.SelectItem(self)

        def SetSize(self,w,h):
            w = w-110
            ui.Window.SetSize(self,w,h)
            self.width = w
            self.height = h
        def OnUpdate(self):    
            if self.IsIn():
                self.isOver = True
            else:
                self.isOver = False
        def OnRender(self):
            xRender, yRender = self.GetGlobalPosition()
            yRender -= self.TEMPORARY_PLACE
            widthRender = self.width
            heightRender = self.height + self.TEMPORARY_PLACE*2
            grp.SetColor(ui.BACKGROUND_COLOR)
            grp.RenderBar(xRender, yRender, widthRender, heightRender)
            grp.SetColor(ui.DARK_COLOR)
            grp.RenderLine(xRender, yRender, widthRender, 0)
            grp.RenderLine(xRender, yRender, 0, heightRender)
            grp.SetColor(ui.BRIGHT_COLOR)
            grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
            grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

            if self.isOver:
                grp.SetColor(ui.HALF_WHITE_COLOR)
                grp.RenderBar(xRender + 2, yRender + 3, self.width - 3, heightRender - 5)

    
    def __init__(self,parent):
        ui.Window.__init__(self,"TOP_MOST")
        self.down = 1
        self.parent=parent
    
        
        self.DropList = ui.ListBoxEx()
        self.DropList.SetParent(self)
        self.DropList.itemHeight = 20
        self.DropList.itemWidth = 220
        self.DropList.itemStep = 18
        self.DropList.SetPosition(0,0)
        self.DropList.SetSize(200,2) 
        self.DropList.SetSelectEvent(self.SetTitle)
        self.DropList.SetViewItemCount(0)
        self.DropList.Show()
        self.selected = self.DropList.GetSelectedItem()
        
        self.SetSize(220-110,95)
    
    def __del__(self): 
        ui.Window.__del__(self)
        
    def AppendItem(self,text,value=0,skill=0):  
        self.DropList.AppendItem(self.Item(self,text,value,skill))
    
    def OnPressEscapeKey(self):        
        self.Hide()
        self.Clear()
                
    def SetTitle(self,item):
        self.dropped = 0
        self.selected = item
        if self.OnChange:
            self.OnChange()
        self.Clear()        
        
    def SetSize(self,w,h):
        ui.Window.SetSize(self,w,h+10)
        self.width = w
        self.height = h
        self.DropList.SetSize(w,h)

    def Clear(self):
        for x in self.DropList.itemList:
            x.Hide()
        self.DropList.RemoveAllItems()

    def ExpandMe(self):
        if self.dropped == 1:
            self.dropped = 0
        else:
            self.dropped = 1
            
    def OnUpdate(self):
        (w,h) = self.parent.GetLocalPosition()
        self.maxh =self.DropList.itemStep*len(self.DropList.itemList)
        self.SetPosition(w+15,h+75)
        if self.dropped == 0 or not self.parent.IsShow() or len(self.DropList.itemList)==0:
            self.SetSize(self.GetWidth(),0)
            self.DropList.SetViewItemCount(0)
            self.Hide()
        elif self.dropped == 1:
            self.Show()
            self.SetTop()
            height = self.maxh+5 if int(self.maxh/self.DropList.itemStep) <2 else self.maxh
            self.SetSize(self.GetWidth(),height)
            self.DropList.SetViewItemCount(self.maxh/self.DropList.itemStep)