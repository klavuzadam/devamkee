# coding: latin_1

import constInfo
import chat
import time
import item
#import uiWeb
import uiToolTip
import app
#import urllib
import localeInfo
import ui
import math
## add "editline.CanEdit()" in the ui.py

IS_BUY = FALSE ## Do not change
IMG_ITEMSHOP = "Itemshop/"

#########################################################
## NEW
#########################################################
class ScrollBar(ui.Window):

    SCROLLBAR_WIDTH = 17
    SCROLLBAR_MIDDLE_HEIGHT = 9
    SCROLLBAR_BUTTON_WIDTH = 17
    SCROLLBAR_BUTTON_HEIGHT = 17
    MIDDLE_BAR_POS = 5
    MIDDLE_BAR_UPPER_PLACE = -15
    MIDDLE_BAR_DOWNER_PLACE = -17
    TEMP_SPACE = MIDDLE_BAR_UPPER_PLACE + MIDDLE_BAR_DOWNER_PLACE

    class MiddleBar(ui.DragButton):
        def __init__(self):
            ui.DragButton.__init__(self)
            self.AddFlag("movable")
            self.AddFlag("animate")
            #self.AddFlag("restrict_x")

        def MakeImage(self):
            top = ui.ImageBox()
            top.SetParent(self)
            top.LoadImage("d:/ymir work/ui/itemshop/scrollbar_top.tga")
            top.SetPosition(0, 0)
            top.AddFlag("not_pick")
            top.Show()
            bottom = ui.ImageBox()
            bottom.SetParent(self)
            bottom.LoadImage("d:/ymir work/ui/itemshop/scrollbar_bottom.tga")
            bottom.AddFlag("not_pick")
            bottom.Show()

            middle = ui.ExpandedImageBox()
            middle.SetParent(self)
            middle.LoadImage("d:/ymir work/ui/itemshop/scrollbar_middle.tga")
            middle.SetPosition(0, 4)
            middle.AddFlag("not_pick")
            middle.Show()

            self.top = top
            self.bottom = bottom
            self.middle = middle

        def SetSize(self, height):
            height = max(12, height)
            ui.DragButton.SetSize(self, 10, height)
            self.bottom.SetPosition(0, height-4)

            height -= 4*3
            self.middle.SetRenderingRect(0, 0, 0, float(height)/4.0)

    def __init__(self):
        ui.Window.__init__(self)

        self.pageSize = 1
        self.curPos = 0.0
        self.eventScroll = lambda *arg: None
        self.lockFlag = False
        self.scrollStep = 0.20


        self.CreateScrollBar()

    def SetDown(self, n):

        self.MIDDLE_BAR_DOWNER_PLACE = n
        self.TEMP_SPACE = self.MIDDLE_BAR_UPPER_PLACE + self.MIDDLE_BAR_DOWNER_PLACE

    def __del__(self):
        ui.Window.__del__(self)

    def CreateScrollBar(self):
        barSlot = ui.Bar3D()
        barSlot.SetParent(self)
        barSlot.AddFlag("not_pick")
        barSlot.Hide()

        background = ui.ImageBox()
        background.SetParent(self)
        background.SetPosition(0, 0)
        background.LoadImage("d:/ymir work/ui/itemshop/scroll_bar.tga")
        #background.SetScale(1, 0.97)
        background.Show()
        self.background = background

        middleBar = self.MiddleBar()
        middleBar.SetParent(self)
        middleBar.SetMoveEvent(ui.__mem_func__(self.OnMove))
        middleBar.Show()
        middleBar.MakeImage()
        middleBar.SetSize(12)

        upButton = ui.Button()
        upButton.SetParent(self)
        upButton.SetEvent(ui.__mem_func__(self.OnUp))
        upButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_up_button_01.sub")
        upButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_up_button_02.sub")
        upButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_up_button_03.sub")
        upButton.Hide()

        downButton = ui.Button()
        downButton.SetParent(self)
        downButton.SetEvent(ui.__mem_func__(self.OnDown))
        downButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_down_button_01.sub")
        downButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_down_button_02.sub")
        downButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_down_button_03.sub")
        downButton.Hide()

        self.upButton = upButton
        self.downButton = downButton
        self.middleBar = middleBar
        self.barSlot = barSlot

        self.SCROLLBAR_WIDTH = self.upButton.GetWidth()
        self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()
        self.SCROLLBAR_BUTTON_WIDTH = self.upButton.GetWidth()
        self.SCROLLBAR_BUTTON_HEIGHT = self.upButton.GetHeight()


        if app.ENABLE_MOUSEWHEEL_EVENT:
            self.upButton.SetMouseWheelScrollEvent(self.OnMouseWheelScroll_ScrollBar)
            self.downButton.SetMouseWheelScrollEvent(self.OnMouseWheelScroll_ScrollBar)
            self.middleBar.SetMouseWheelScrollEvent(self.OnMouseWheelScroll_ScrollBar)
            self.barSlot.SetMouseWheelScrollEvent(self.OnMouseWheelScroll_ScrollBar)
            self.SetMouseWheelScrollEvent(self.OnMouseWheelScroll_ScrollBar)

    if app.ENABLE_MOUSEWHEEL_EVENT:
        def OnMouseWheelScroll_ScrollBar(self,mode):
            #chat.AppendChat(chat.CHAT_TYPE_INFO, "--")
            eventDct = { "UP" : lambda : self.SetPos(self.curPos - (self.scrollStep/4)) , "DOWN" : lambda: self.SetPos(self.curPos + (self.scrollStep/4)) }
            
            if mode in eventDct:
                eventDct[mode]()
            

    def Destroy(self):
        self.middleBar = None
        self.upButton = None
        self.downButton = None
        self.eventScroll = lambda *arg: None

    def SetScrollEvent(self, event):
        self.eventScroll = event

    def SetMiddleBarSize(self, pageScale):
        realHeight = self.GetHeight() - self.SCROLLBAR_BUTTON_HEIGHT*2
        self.SCROLLBAR_MIDDLE_HEIGHT = int(pageScale * float(realHeight))
        self.middleBar.SetSize(self.SCROLLBAR_MIDDLE_HEIGHT)
        self.pageSize = (self.GetHeight() - self.SCROLLBAR_BUTTON_HEIGHT*2) - self.SCROLLBAR_MIDDLE_HEIGHT - (self.TEMP_SPACE)

    def SetScrollBarSize(self, height):
        self.pageSize = (height - self.SCROLLBAR_BUTTON_HEIGHT*2) - self.SCROLLBAR_MIDDLE_HEIGHT - (self.TEMP_SPACE)
        self.SetSize(self.SCROLLBAR_WIDTH, height)
        self.upButton.SetPosition(0, 0)
        self.downButton.SetPosition(0, height - self.SCROLLBAR_BUTTON_HEIGHT)
        self.middleBar.SetRestrictMovementArea(self.MIDDLE_BAR_POS, self.SCROLLBAR_BUTTON_HEIGHT + self.MIDDLE_BAR_UPPER_PLACE, self.MIDDLE_BAR_POS+2, height - self.SCROLLBAR_BUTTON_HEIGHT*2 - self.TEMP_SPACE)
        self.middleBar.SetPosition(0, 0)

        self.UpdateBarSlot()

    def UpdateBarSlot(self):
        self.barSlot.SetPosition(0, self.SCROLLBAR_BUTTON_HEIGHT)
        self.barSlot.SetSize(self.GetWidth() - 2, self.GetHeight() - self.SCROLLBAR_BUTTON_HEIGHT*2 - 2)

    def GetPos(self):
        return self.curPos

    def SetPos(self, pos):
        pos = max(0.0, pos)
        pos = min(1.0, pos)

        newPos = float(self.pageSize) * pos
        self.middleBar.SetPosition(self.MIDDLE_BAR_POS, int(newPos) + self.SCROLLBAR_BUTTON_HEIGHT + self.MIDDLE_BAR_UPPER_PLACE)
        self.OnMove()

    def SetScrollStep(self, step):
        self.scrollStep = step

    def GetScrollStep(self):
        return self.scrollStep

    def OnUp(self):
        self.SetPos(self.curPos-self.scrollStep)

    def OnDown(self):
        self.SetPos(self.curPos+self.scrollStep)

    def OnMove(self):

        if self.lockFlag:
            return

        if 0 == self.pageSize:
            return

        (xLocal, yLocal) = self.middleBar.GetLocalPosition()
        self.curPos = float(yLocal - self.SCROLLBAR_BUTTON_HEIGHT - self.MIDDLE_BAR_UPPER_PLACE) / float(self.pageSize)

        self.eventScroll()

    def OnMouseLeftButtonDown(self):
        (xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
        pickedPos = yMouseLocalPosition - self.SCROLLBAR_BUTTON_HEIGHT - self.SCROLLBAR_MIDDLE_HEIGHT/2
        newPos = float(pickedPos) / float(self.pageSize)
        self.SetPos(newPos)

    def LockScroll(self):
        self.lockFlag = True

    def UnlockScroll(self):
        self.lockFlag = False

class ItemShopListBox(ui.Window):
    class Item(ui.ScriptWindow):
        def __init__(self):
            ui.ScriptWindow.__init__(self)
            self.parent = None

        def __del__(self):
            ui.ScriptWindow.__del__(self)

        def SetParent(self, parent):
            ui.ScriptWindow.SetParent(self, parent)
            self.parent = ui.proxy(parent)

        def SetData(self, data):
            pass

    def __init__(self, instance):
        ui.Window.__init__(self)
        self.viewColumn = 0
        self.viewRow    = 0
        self.curRow        = 0
        self.curColumn    = 0
        self.itemWidth    = 0
        self.itemHeight = 0
        self.instance    = instance
        self.instances    = []
        self.data        = {}
        self.basePos    = 0
        self.scrollBar    = None
        self.itemtoolTip = None

    def __del__(self):
        ui.Window.__del__(self)

    def SetParent(self, parent):
        ui.Window.SetParent(self, parent)
        self.parent = ui.proxy(parent)

    def SetItemSize(self, itemWidth, itemHeight):
        self.itemWidth    = itemWidth
        self.itemHeight = itemHeight

    def SetScrollBar(self, scrollBar):
        self.scrollBar = scrollBar
        self.scrollBar.SetScrollEvent(ui.__mem_func__(self.__OnScroll))

    def __OnScroll(self):
        pos = self.scrollBar.GetPos()
        scrollLen = (len(self.data)) - self.viewRow
        self.SetBasePos(math.floor(pos * scrollLen))

    def SetViewItemCount(self, column, row, itemtoolTip, event, event2):
        self.viewColumn = column
        self.viewRow    = row
        self.itemtoolTip = itemtoolTip

        self.instances = [[self.instance() for x in xrange(0, column)] for y in xrange(0, row)]
        for itemList in self.instances:
            for item in itemList:
                item.SetMouseWheelScrollEvent2(event2)
                item.SetEscapeEvent(event)
                item.SetItemTooltip(self.itemtoolTip)
                item.SetParent(self)

    def RemoveAllItems(self):
        self.data        = {}
        self.curRow        = 0
        self.curColumn    = 0
        self.scrollBar.SetPos(0)
        self.SetBasePos(0)

    def AppendData(self, data):
        if self.curColumn == self.viewColumn:
            self.curColumn = 0
            self.curRow += 1

        if not self.curRow in self.data:
            self.data.update({self.curRow: []})

        self.data[self.curRow].append(data)
        self.curColumn += 1

    def SetBasePos(self, basePos):
        self.basePos = basePos
        for itemList in self.instances:
            self.__HideItems(itemList)

        pos = self.basePos
        skipCount = self.basePos
        curRow = 0

        for key, data in self.data.items():
            if skipCount > 0:
                skipCount -= 1
                continue

            if curRow == self.viewRow:
                break

            x = 0
            y = curRow * self.itemHeight
            curData = 0

            for item in self.instances[curRow]:
                if curData < len(data):
                    item.SetData(data[curData])
                    item.SetPosition(x, y)
                    item.Show()
                    x+=self.itemWidth
                    curData+= 1
                else:
                    item.Hide()

            pos += 1
            curRow += 1

        if len(self.data) > self.viewRow:
            self.scrollBar.SetMiddleBarSize(float(self.viewRow) / float(len(self.data)))
            self.scrollBar.Show()
        else:
            self.scrollBar.Hide()

    def __HideItems(self, itemList):
        for item in itemList:
            item.Hide()

class ItemShopItem(ItemShopListBox.Item):
    def __init__(self):
        ItemShopListBox.Item.__init__(self)
        self.__LoadObject()
        
    def __del__(self):
        ItemShopListBox.Item.__del__(self)

    def SetSize(self, width, height):
        ItemShopListBox.Item.SetSize(self, width, height)

    def __LoadObject(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/itemshop_v2_itembox.py")
        except:
            import exception
            exception.Abort("ItemShopItem.Item.__init__.LoadObject")

        try: 
            self.itemBox = self.GetChild('Background')
            self.timeBox = self.GetChild('TimeBox')
            self.countdown = self.GetChild('tx_countdown')
            self.percentBox = self.GetChild('PercentBox')
            self.tx_percent = self.GetChild('tx_percent')
            self.btn_buy = self.GetChild('btn_buy')
            self.itemName = self.GetChild('tx_itemName')
            self.itemPrice = self.GetChild('tx_itemPrice')
            self.amount = self.GetChild('ed_amount')
            self.amountBox = self.GetChild('sb_amount')
            self.icon = self.GetChild('icon_item')
            self.icon_price = self.GetChild("icon_price")


            self.itemBuyQuestionDialog = ItemBuyDialog()
            
        except:
            import exception
            exception.Abort('test.__LoadScript.BindObject')

        try: 
            self.btn_buy.SetEvent(self.__OnClickBuy)
            self.amount.SetNumberMode()
            self.itemBuyQuestionDialog.SetAcceptEvent(lambda arg=TRUE: self.AnswerBuyItem(arg))
            self.itemBuyQuestionDialog.SetCancelEvent(lambda arg=FALSE: self.AnswerBuyItem(arg))


            self.amount.SetEscapeEvent(ui.__mem_func__(self.__OnEscapeEvent))

            self.icon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.Icon_MouseOverIn)
            self.icon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.Icon_MouseOverOut)
        except:
            import exception
            exception.Abort('test.__LoadScript.BindEvent')

    def SetMouseWheelScrollEvent2(self, event):
        self.itemBox.SetMouseWheelScrollEvent(event)
        self.timeBox.SetMouseWheelScrollEvent(event)
        self.countdown.SetMouseWheelScrollEvent(event)
        self.percentBox.SetMouseWheelScrollEvent(event)
        self.tx_percent.SetMouseWheelScrollEvent(event)
        self.btn_buy.SetMouseWheelScrollEvent(event)
        self.itemName.SetMouseWheelScrollEvent(event)
        self.itemPrice.SetMouseWheelScrollEvent(event)
        self.amount.SetMouseWheelScrollEvent(event)
        self.amountBox.SetMouseWheelScrollEvent(event)
        self.icon.SetMouseWheelScrollEvent(event)
        self.icon_price.SetMouseWheelScrollEvent(event)


    def SetData(self, curItem):
        self.SetContent(curItem[0], curItem[1], curItem[2], curItem[3]) ## type, itemvnum, itemdetails, price
        self.SetPercent(curItem[4]) ## percent
        self.SetTime(curItem[5], curItem[6]) ## time , runOut

    def SetEscapeEvent(self, event):
        self.escapeEvent = event            
            
    def __OnEscapeEvent(self):
        if self.escapeEvent:
            self.escapeEvent()

    ## Set the content of this box
    def SetContent(self, type, id, itemData, price):
        
        self.type = type
        self.itemData = itemData
        self.id = id ## id from mysql, used to identify the item from the table 
        self.price = price
        self.itemPrice.SetText('%d ' % (price))
        self.icon_price.LoadImage(IMG_ITEMSHOP+"%s.tga"%(['icon_coin','icon_vcoin','icon_coin'])[type])
        self.amount.SetText('1')

        item.SelectItem(itemData[0])
        if item.IsFlag(4) == 1:
            self.amountBox.Show()
        else:
            self.amountBox.Hide()

        self.itemName.SetText(item.GetItemName())

        ## Load the image and scale the item if the slotsize is bigger than 1
        self.icon.LoadImage(str(item.GetIconImageFileName()))
        #self.icon.SetScale(1, ([1, 0.6, 0.4])[item.GetItemSize()[1]-1])
        
        self.btn_buy.Enable()
        self.amount.KillFocus()
        self.amount.CanEdit(TRUE)
        self.timeBox.Hide()
        self.percentBox.Hide()

    def SetTime(self, time, runOut):
        if time == 0:
            self.timeBox.Hide()
            self.time = None
            return
        else:
            self.timeBox.Show()
            self.time = time
            self.runOut = runOut
            self.lastTime = 0

    def SetPercent(self, percent):
        if percent == 0:
            self.percentBox.Hide()
            return
        else:
            self.percentBox.LoadImage(IMG_ITEMSHOP+"descuento.tga")
            self.percentBox.Show()
            
            self.itemPrice.SetText('%d ' % (self.price-(self.price/100.00)*percent))
            self.icon_price.LoadImage(IMG_ITEMSHOP+"%s.tga"%(['icon_coin','icon_vcoin','icon_coin'])[self.type])
            self.tx_percent.SetText(str(percent) + '%')
        
    ## Set the parent to attach and the coordinates
    def SetItemTooltip(self, itemtoolTip):
        self.toolTip = itemtoolTip

    def Icon_MouseOverIn(self):
        
        self.toolTip.ClearToolTip()
        item.SelectItem(self.itemData[0])

        ## if item is real time (limit type) then calculate the time
        if item.GetLimit(0)[0] == 7:
            self.toolTip.AddItemData(self.itemData[0], [self.itemData[1] + app.GetGlobalTimeStamp(),self.itemData[2],self.itemData[3],0,0,0], [(self.itemData[4],self.itemData[5]),(self.itemData[6],self.itemData[7]),(self.itemData[8],self.itemData[9]),(self.itemData[10],self.itemData[11]),(self.itemData[12],self.itemData[13]),(self.itemData[14],self.itemData[15]),(self.itemData[16],self.itemData[17]),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0)])
        else:
            self.toolTip.AddItemData(self.itemData[0], [self.itemData[1],self.itemData[2],self.itemData[3],0,0,0],    [(self.itemData[4],self.itemData[5]),(self.itemData[6],self.itemData[7]),(self.itemData[8],self.itemData[9]),(self.itemData[10],self.itemData[11]),(self.itemData[12],self.itemData[13]),(self.itemData[14],self.itemData[15]),(self.itemData[16],self.itemData[17]),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0)])

    def Icon_MouseOverOut(self):
        self.toolTip.Hide()

    def OnUpdate(self):
        amount = self.amount.GetText()
        if amount:
            ## if the first character of amount is a 0 or its empty and not focused, then set the amount to 1
            if (amount != "" and amount[0] == '0') or (not self.amount.IsFocus() and amount == ""):
                self.amount.SetText('1')
            if int(amount) > 200:
                self.amount.SetText('200')

        ## if time is set then calculate the time until end
        if self.time:
            remaining = self.time - app.GetGlobalTimeStamp()
            if self.lastTime < time.clock():
                if remaining <= 0:
                    if self.runOut == 1:
                        self.time = None
                        self.countdown.SetText('Abgelaufen')
                        self.btn_buy.Disable()
                        self.amount.CanEdit(FALSE) ## new function in ui
                    else:
                        self.timeBox.Hide()
                        self.percentBox.Hide()
                        self.itemPrice.SetText('%d %s' % (self.price, (['Coins','Tokens','L-Coins'])[self.type]))
                    return

                self.lastTime = time.clock() + 1
                hoursRemaining = int(remaining) / 3600
                minutesRemaining = int(remaining % 3600) / 60
                secondsRemaining = int(remaining % 60)
                self.countdown.SetText('%dh %dm %ds' % (hoursRemaining, minutesRemaining, secondsRemaining))

    def __OnClickBuy(self):
        if self.amount.GetText() == '':
            self.amount.SetText('1')

        self.amount.KillFocus()
        amount = self.amount.GetText()
        price = int(amount) * int(self.itemPrice.GetText().split(' ')[0])

        if amount == '1':
            self.itemBuyQuestionDialog.SetText("Do you want buy %s for %d %s?" % (self.itemName.GetText(), price, (['Coins','Tokens','L-Coins'])[self.type]))
        else:
            self.itemBuyQuestionDialog.SetText("Do you want buy %sx %s for %d %s?" % (amount, self.itemName.GetText(), price, (['Coins','Tokens','L-Coins'])[self.type]))
        self.itemBuyQuestionDialog.Open()

    def AnswerBuyItem(self, arg):
        self.itemBuyQuestionDialog.Close()
        if arg == 1:
            # self.SendChat('BUY ID: %d AMOUNT: %s' % (self.id, self.amount.GetText()))
            import event
            constInfo.ITEMSHOP["questCMD"] = 'BUY#%d#%s' % (self.id, self.amount.GetText())
            event.QuestButtonClick(int(constInfo.ITEMSHOP["qid"]))
            ## Send buy item : [type, id, amount]
        self.amount.SetText('1')
        
    def SendChat(self, text):
        chat.AppendChat(chat.CHAT_TYPE_INFO, '<Shop>: '+str(text))


#########################################################
## NEW
#########################################################

class DropdownTree(ui.Window):
    class Item(ui.Window):
        def __init__(self):
            ui.Window.__init__(self)
            self.id = -1
            self.parentId = -1
            self.offset = 0
            self.visible = False
            self.expanded = False
            self.event = None
            self.onCollapseEvent = None
            self.onExpandEvent = None
            self.isSubTree = False

        def SetSubTree(self, arg):
            self.isSubTree = arg

        def IsSubTree(self):
            return self.isSubTree
            
        def __del__(self):
            ui.Window.__del__(self)

        def SetParent(self, parent):
            ui.Window.SetParent(self, parent)
            self.parent=ui.proxy(parent)

        def SetSize(self, width, height):
            ui.Window.SetSize(self, width, height)

        def GetId(self):
            return self.id

        def SetId(self, id):
            self.id = id

        def GetParentId(self):
            return self.parentId

        def SetParentId(self, parentId):
            self.parentId = parentId
            
        def IsParent(self):
            return self.parentId == -1

        def SetVisible(self, visible):
            self.visible = visible
            
        def IsVisible(self):
            return self.visible
            
        def IsExpanded(self):
            return self.expanded
            
        def Expand(self):
            self.expanded = True
            if self.onExpandEvent:
                self.onExpandEvent()
            
        def Collapse(self):
            self.expanded = False
            if self.onCollapseEvent:
                self.onCollapseEvent()

        def SetOnExpandEvent(self, event):
            self.onExpandEvent = event

        def SetOnCollapseEvent(self, event):
            self.onCollapseEvent = event

        def SetOffset(self, offset):
            self.offset = offset

        def GetOffset(self):
            return self.offset

        def SetEvent(self, event):
            self.event = event

        def OnSelect(self):
            if self.event:
                self.event()
            self.parent.SelectItem(self)

        def OnMouseLeftButtonDown(self):
            self.OnSelect()

    def __init__(self):
        ui.Window.__init__(self)

        self.__curItemId=0
        self.viewItemCount=10
        self.viewItemCount2=0
        self.basePos=0
        self.itemHeight=16
        self.itemStep=20
        self.itemStep2=20
        self.selItem=0
        self.itemList=[]
        self.onSelectItemEvent = lambda *arg: None

        if localeInfo.IsARABIC():
            self.itemWidth=130
        else:
            self.itemWidth=100

        self.scrollBar=None
        self.__UpdateSize()
    
    def __del__(self):
        ui.Window.__del__(self)

    def __UpdateSize(self):
        height=self.itemStep*self.__GetViewItemCount()

        self.SetSize(self.itemWidth, height)

    def IsEmpty(self):
        if len(self.itemList)==0:
            return 1
        return 0

    def SetItemStep(self, itemStep, itemStep2):
        self.itemStep=itemStep
        self.itemStep2=itemStep2
        self.__UpdateSize()

    def SetItemSize(self, itemWidth, itemHeight):
        self.itemWidth=itemWidth
        self.itemHeight=itemHeight
        self.__UpdateSize()
    
    def SetViewItemCount(self, viewItemCount):
        self.viewItemCount=viewItemCount
    
    def SetSelectEvent(self, event):
        self.onSelectItemEvent = event

    def SetBasePos(self, basePos):
        for oldItem in self.itemList:
            oldItem.Hide()

        self.basePos=basePos

        skipCount = basePos
        pos = basePos

        prevX = 0
        prevY = 0

        
        for lItem in self.itemList:
            if not lItem.IsVisible():
                continue
            
            if skipCount > 0:
                skipCount -= 1
                continue

            if prevY >= 410:
                continue

            if pos >= (self.basePos+self.viewItemCount+(self.viewItemCount2)):
                break
            (x, y) = self.GetItemViewCoord(pos, lItem)

            #lItem.SetPosition(x+lItem.GetOffset(), y)
            prevX = x+lItem.GetOffset()
            lItem.SetPosition(prevX, prevY)
            if lItem.IsSubTree():
                prevY += 17
            else:
                prevY += 37
            lItem.Show()
            pos+=1

        self.UpdateScrollbar()

    def GetItemIndex(self, argItem):
        return self.itemList.index(argItem)

    def GetSelectedItem(self):
        return self.selItem

    def SelectIndex(self, index):
        if index >= len(self.itemList) or index < 0:
            self.selItem = None
            return

        try:
            self.selItem=self.itemList[index]
        except:
            pass

    def ClearItem(self):
        self.selItem=None
        for lItem in self.itemList:
            lItem.Hide()

        self.itemList=[]
        self.__curItemId = 0

        if self.scrollBar:
            self.scrollBar.SetPos(0)
        self.SetBasePos(0)

    def SelectItem(self, selItem):
        self.selItem = selItem
        if selItem.IsExpanded():
            self.CloseTree(selItem, self.itemList)
        else:
            self.OpenTree(selItem, self.itemList)
        self.SetBasePos(self.basePos)

    def __AppendItem(self, newItem, parentId):
        curItemId = self.__curItemId
        self.__curItemId += 1
        
        newItem.SetParent(self)
        newItem.SetParentId(parentId)
        newItem.SetSize(self.itemWidth, self.itemHeight)
        newItem.SetId(curItemId)

        pos = self.__GetItemCount()
        self.itemList.append(newItem)

        if newItem.IsVisible() and self.__IsInViewRange(pos):
            (x, y)=self.GetItemViewCoord(pos, newItem)
            newItem.SetPosition(x, y)
            newItem.Show()
        else:
            newItem.Hide()

        self.UpdateScrollbar()

        return curItemId

    def AppendItemList(self, dict):
        self.__AppendItemList(-1, dict)
    
    def __AppendItemList(self, parentId, dict):
        for lItem in dict:
            if 'item' in lItem:
                id = self.__AppendItem(lItem['item'], parentId)
                if 'children' in lItem:
                    self.__AppendItemList(id, lItem['children'])
                
    def SetScrollBar(self, scrollBar):
        scrollBar.SetScrollEvent(ui.__mem_func__(self.__OnScroll))
        self.scrollBar=scrollBar

    def __OnScroll(self):
        self.SetBasePos(int(self.scrollBar.GetPos()*self.__GetScrollLen()))

    def __GetScrollLen(self):
        scrollLen=self.__GetItemCount()-self.__GetViewItemCount()
        if scrollLen<0:
            return 0

        return scrollLen

    def __GetViewItemCount(self):
        return self.viewItemCount

    def __GetItemCount(self):
        return sum(1 for lItem in self.itemList if lItem.IsVisible())

    def __GetItemCount2(self, t):
        n = 1
        for lItem in self.itemList:
            if not lItem.IsVisible():
                continue

            if t == 1 and lItem.IsSubTree():
                n+=1

            if t == 2 and not lItem.IsSubTree():
                n+=1

        return n

    def GetItemViewCoord(self, pos, item, add = 0):
        itemWidth = item.GetWidth()
        itemStep = self.itemStep
        if item.IsSubTree():
            itemStep = self.itemStep2
        if localeInfo.IsARABIC():
            return (self.GetWidth()-itemWidth-10, (pos-self.basePos)*itemStep)
        else:
            return (0, ((pos-self.basePos)*itemStep) + add)

    def __IsInViewRange(self, pos):
        if pos<self.basePos:
            return 0
        if pos>=self.basePos+self.viewItemCount:
            return 0
        return 1
    
    def UpdateScrollbar(self):
        if self.__GetViewItemCount() < self.__GetItemCount():
            self.scrollBar.SetMiddleBarSize(float(self.__GetViewItemCount())/self.__GetItemCount())
            self.scrollBar.Show()
        else:
            self.scrollBar.Hide()

    def CloseTree(self, curItem, list):
        curItem.Collapse()
        n = 0
        
        for listboxItem in list:
            if listboxItem.GetParentId() == curItem.GetId():
                listboxItem.SetVisible(False)
                n+=1
                self.CloseTree(listboxItem, list)

        self.viewItemCount2 -= n
        self.scrollBar.SetPos(self.scrollBar.GetPos())
        
        
    def OpenTree(self, curItem, list):
        n = 0
        curItem.Expand()
        for listboxItem in list:
            if listboxItem.GetParentId() == curItem.GetId():
                listboxItem.SetVisible(True)
                n+=1
        self.viewItemCount2 += n
        #self.scrollBar.SetPos(self.scrollBar.GetPos())

class CategoryMenuTab1(DropdownTree.Item):
    def __init__(self, text, image, amount, kImg):
        DropdownTree.Item.__init__(self)
        kImg = int(kImg)

        dImg = {
            1: "d:/ymir work/ui/itemshop/big_button_%d.tga",
            2: "d:/ymir work/ui/itemshop/blue_button_%d.tga",
            3: "d:/ymir work/ui/itemshop/gold_button_%d.tga",
            4: "d:/ymir work/ui/itemshop/green_button_%d.tga",
            5: "d:/ymir work/ui/itemshop/red_button_%d.tga",
        }

        if not kImg in dImg:
            kImg = 1

        #if amount > 0:
        #    self.background = ui.ImageBox()
        #    self.background.SetParent(self)
        #    self.background.AddFlag("not_pick")
        #    self.background.SetPosition(0, 0)
        #    self.background.LoadImage(dImg[kImg] % 1)
        #    self.background.Show()
        #else:
        self.background = ui.Button()
        self.background.SetEvent(self.OnSelect)
        self.background.SetParent(self)
        self.background.SetUpVisual(dImg[kImg] % 1)
        self.background.SetOverVisual(dImg[kImg] % 2)
        self.background.SetDownVisual(dImg[kImg] % 3)
        self.background.Show()

        self.iconA = ui.ImageBox()
        self.iconA.SetParent(self)
        self.iconA.AddFlag("not_pick")
        self.iconA.SetPosition(self.background.GetWidth() - 15, 13)
        self.iconA.LoadImage("d:/ymir work/ui/itemshop/flecha_abajo.tga")
        self.amount = amount
        if self.amount == 0:
            self.iconA.Hide()
        else:
            self.iconA.Show()

        self.icon = ui.ImageBox()
        self.icon.SetParent(self)
        self.icon.AddFlag("not_pick")
        self.icon.SetPosition(5, 7)
        self.icon.LoadImage("d:/ymir work/ui/itemshop/%s" % image)
        self.icon.Show()

        self.textLine = ui.TextLine()
        self.textLine.SetParent(self)
        self.textLine.SetHorizontalAlignLeft()
        self.textLine.SetPosition(35, 0)
        self.textLine.SetWindowVerticalAlignCenter()
        self.textLine.SetVerticalAlignCenter()
        self.textLine.SetText(text)
        self.textLine.Show()

        self.SetOnExpandEvent(self.ExpandEvent)
        self.SetOnCollapseEvent(self.CollapseEvent)

    def __del__(self):
        DropdownTree.Item.__del__(self)

    def SetMouseWheelScrollEvent2(self, event):
        self.SetMouseWheelScrollEvent(event)
        self.background.SetMouseWheelScrollEvent(event)
        self.icon.SetMouseWheelScrollEvent(event)

    def SetSize(self, width, height):
        DropdownTree.Item.SetSize(self, width-self.GetOffset(), height)

    def CollapseEvent(self):    
        self.iconA.LoadImage("d:/ymir work/ui/itemshop/flecha_abajo.tga")

    def ExpandEvent(self):
        self.iconA.LoadImage("d:/ymir work/ui/itemshop/flecha_arriba.tga")

class CategoryMenuTab2(DropdownTree.Item):
    def __init__(self, text):
        DropdownTree.Item.__init__(self)
        self.background = ui.Button()
        self.background.SetParent(self)
        #self.background.AddFlag("not_pick")
        self.background.SetUpVisual("d:/ymir work/ui/itemshop/mini_button_1.tga")
        self.background.SetOverVisual("d:/ymir work/ui/itemshop/mini_button_2.tga")
        self.background.SetDownVisual("d:/ymir work/ui/itemshop/mini_button_3.tga")
        #self.background.SetText(text)
        self.background.SetEvent(self.OnSelect)
        self.background.Show()

        self.textLine = ui.TextLine()
        self.textLine.SetParent(self.background)
        self.textLine.SetHorizontalAlignLeft()
        self.textLine.SetPosition(16, 0)
        #self.textLine.SetWindowVerticalAlignCenter()
        #self.textLine.SetVerticalAlignCenter()
        self.textLine.SetText(text)
        self.textLine.Show()



        self.SetOnExpandEvent(self.ExpandEvent)
        self.SetOnCollapseEvent(self.CollapseEvent)

    def __del__(self):
        DropdownTree.Item.__del__(self)

    def SetMouseWheelScrollEvent2(self, event):
        self.SetMouseWheelScrollEvent(event)
        self.background.SetMouseWheelScrollEvent(event)

    def SetSize(self, width, height):
        DropdownTree.Item.SetSize(self, width-self.GetOffset(), height)

    def CollapseEvent(self):    
        pass

    def ExpandEvent(self):
        pass

class Itemshop(ui.ScriptWindow):
    ###########
    ##Options##
    ###########

    ## Banner options
    bannerOptions = {
            'folder' : 'Itemshop/', ## folderpath to the banner images
            'time' : 5, ## time in seconds to change the banner automatically
            'timeToFade' : 0.04,
            'interval' : 0.05, 
            'banner_0' : 'banner',
            'banner_1' : 'banner',
        }

    ## Buy coins; voting options
    link = {\
        'buyCoins' : "http://goo.gl/forms/gwopLqxyfa",
        # 'vote' : "http://homepage.ephyra2.info/?s=vote4coins_system",
    }

    ## Item/voteshop category options
    categorys = {
            'itemshop' : [
                ['Objetos especiales',1],
                ['Alquimia',2],
                ['Monturas',3],
                ["Estolas",4],
                ["Piedras+5",5],
                ["Mascotas",6],
                ["Atuendos(M)",7],
                ["Atuendos(F)",8],
                ["Skins de armas",9],
                ["Peinados(M)",10],
                ["Peinados(F)",11],
                ["Efectos Armas",12],
                ["Efectos Armaduras",13],
                ["Atuendos PERM.",14],
                ["ATUENDOS UNICOS",15],
                ["Skins de Estolas",16],
                ["ATUENDOS DE VERANO",17],

                ],
            'voteshop' : [
                ['Objetos especiales',1],

                ],
            'achievementshop' : [
                ['Weapons',1],
                ['Armors',2],
                ['Accessories',3],
                ['Special Items',4],
                ['Yang',5],
                ['Objects Pokemons',6],
                ['Costumes',7],
                ["New Stones",8],
                ['Objects Dungeons',9],
                ['New Hairstyles',10],
                ['Skins Weapons',11],
                ['Lvls UP',12],
                ],
        }

    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.isLoaded = FALSE
        
        self.itemtoolTip = None

    def BindToolTipItem(self, itemtoolTip):
        self.itemtoolTip = itemtoolTip
        
    #def OnRunMouseWheel(self, nLen):
    #    if self.itemShopListBox.IsIn():
    #        chat.AppendChat(chat.CHAT_TYPE_INFO, "22")

    def __LoadScript(self):
        try:
            self.__LoadVariables()
        except:
            import exception
            exception.Abort('test.__LoadScript.LoadVariables')
        ## Load script
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, 'uiscript/itemshop_v2.py')
        except:
            import exception
            exception.Abort('test.__LoadScript.LoadObject')

        ## Load gui
        try: 
            self.BindObjects()
        except:
            import exception
            exception.Abort('test.__LoadScript.BindObjects')
            
        ## Load events
        try: 
            self.BindEvents()
        except:
            import exception
            exception.Abort('test.__LoadScript.BindEvents')
            
        self.isLoaded = TRUE

    ## Load variables (DONT CHANGE)
    def __LoadVariables(self):
        self.bannerVar = {
                'fadeOut' : 0,
                'currentTime' : 0 ,
                'intervallEndTime' : 0,
                'currentAlphaValue' : 0,
                'currentImage' : 0,
                'lastSwitch' : time.clock() + self.bannerOptions['time'],
            }

        self.page = {\
                        'curPage' : 'STARTPAGE',
            }
        self.arrows = {\
                        'startpage' : {
                                'mostBought' : 1,
                                'hotOffers' : 1,
                            },
                        'itemshop' : {
                                'arrowOnSale' : 1,
                                'arrowCategory' : 0,
                            },
                        'voteshop' : {
                                'arrowOnSale' : 1,
                                'arrowCategory' : 0,
                            },
                        'achievementshop' : {
                                'arrowOnSale' : 1,
                                'arrowCategory' : 0,
                            },
        }

        self.category = {
                    'itemshop' : 0,
                    'voteshop' : 0,
                    'achievementshop' : 0,
                }
        
    ## Bind the objects in __LoadScript
    def BindObjects(self):
        self.board = self.GetChild('Board')
        self.elements = {
            'titlebar' : self.GetChild('titlebar'),
            'pages' : {
                    'startpage' : self.GetChild('Startpage'),
                    'itemshop' : self.GetChild('Itemshop'),
                    'voteshop' : self.GetChild('Voteshop'),
                    'achievementshop' : self.GetChild('Achievementshop'),
                },
            'windows' : {
                    'startpage': {
                        'banner' : self.GetChild('Banner'),
                        'mostBought'  : self.GetChild('MostBought'),
                        },

                    ## Itemshop start ##

                    ## Itemshop end ##
                },
            'buttons' : {

                    'startpage': {
                        'base_inicio': self.GetChild('base_inicio'),
                        'base_categorias' : self.GetChild('base_categorias'),
                        'startpage' : self.GetChild('btn_startpage'),
                        'itemshop' : self.GetChild('btn_itemshop'),
                        'voteshop' : self.GetChild('btn_voteshop'),
                        'achievementshop' : self.GetChild('btn_achievementshop'),
                        'banner_0' : self.GetChild('btn_banner_0'),
                        'banner_1' : self.GetChild('btn_banner_1'),
                        'buy_coins' : self.GetChild('btn_buy_coins'),
                        # 'vote' : self.GetChild('btn_vote'),
                        'mostBought_left' : self.GetChild('btn_mostBought_left'),
                        'mostBought_right' : self.GetChild('btn_mostBought_right'),
                        'hotOffers_up' : self.GetChild('btn_hotOffers_up'),
                        'hotOffers_down' : self.GetChild('btn_hotOffers_down'),
                    },

                    'itemshop' : {
                        'onSale_left' : self.GetChild('btn_IsOnSale_left'),
                        'onSale_right' : self.GetChild('btn_IsOnSale_right'),
                        'onSale_up' : self.GetChild('btn_IsCategory_up'),
                        'onSale_down' : self.GetChild('btn_IsCategory_down'),
                    },

                    'voteshop' : {
                        'onSale_left' : self.GetChild('btn_VsOnSale_left'),
                        'onSale_right' : self.GetChild('btn_VsOnSale_right'),
                        'onSale_up' : self.GetChild('btn_VsCategory_up'),
                        'onSale_down' : self.GetChild('btn_VsCategory_down'),
                    },
                    
                    'achievementshop' : {
                        'onSale_left' : self.GetChild('btn_AsOnSale_left'),
                        'onSale_right' : self.GetChild('btn_AsOnSale_right'),
                        'onSale_up' : self.GetChild('btn_AsCategory_up'),
                        'onSale_down' : self.GetChild('btn_AsCategory_down'),
                    },
                },

            'textline' : {
                    'menue' : {
                        'itemshop_coins' : self.GetChild('tx_i_coins'),
                        'voteshop_coins' : self.GetChild('tx_v_coins'),
                        'achievementshop_coins' : self.GetChild('tx_a_coins'),
                    },
                    'itemshop': {
                        'page_nr' : self.GetChild('tx_IsOnSale_pageNr'),
                    },
                    'voteshop': {
                        'page_nr' : self.GetChild('tx_VsOnSale_pageNr'),
                    },
                    'achievementshop': {
                        'page_nr' : self.GetChild('tx_AsOnSale_pageNr'),
                    },
                },
                
            'images' : {
                    'startpage': {
                        'banner' : self.GetChild('image_banner'),
                        'fade_banner' : self.GetChild('image_fade_banner'),
                    },
                },
            'itemBoxes' : {
                    'startpage' : {
                        'mostBought': {
                            'box_00' : ItemBox(),
                            'box_01' : ItemBox(),
                            'box_02' : ItemBox(),
                        },
                        'hotOffers': {
                            'box_00' : ItemBox(),
                            'box_01' : ItemBox(),
                        },
                    },
                    'itemshop' : {
                        'box_00' : ItemBox(),
                        'box_01' : ItemBox(),
                        'box_02' : ItemBox(),
                        'box_03' : ItemBox(),
                        'box_04' : ItemBox(),
                        'box_05' : ItemBox(),
                        'box_06' : ItemBox(),
                        'box_07' : ItemBox(),
                        'box_08' : ItemBox(),
                        'box_09' : ItemBox(),
                        'box_010' : ItemBox(),
                        'box_011' : ItemBox(),
                    },
                    'voteshop' : {
                        'box_00' : ItemBox(),
                        'box_01' : ItemBox(),
                        'box_02' : ItemBox(),
                        'box_03' : ItemBox(),
                        'box_04' : ItemBox(),
                        'box_05' : ItemBox(),
                        'box_06' : ItemBox(),
                        'box_07' : ItemBox(),
                        'box_08' : ItemBox(),
                        'box_09' : ItemBox(),
                        'box_010' : ItemBox(),
                        'box_011' : ItemBox(),
                    },
                    'achievementshop' : {
                        'box_00' : ItemBox(),
                        'box_01' : ItemBox(),
                        'box_02' : ItemBox(),
                        'box_03' : ItemBox(),
                        'box_04' : ItemBox(),
                        'box_05' : ItemBox(),
                        'box_06' : ItemBox(),
                        'box_07' : ItemBox(),
                        'box_08' : ItemBox(),
                        'box_09' : ItemBox(),
                        'box_010' : ItemBox(),
                        'box_011' : ItemBox(),
                    },
                },
        }


        self.image_lcoins = self.GetChild('sb_a_coins')
        self.image_lcoins.Hide()

        ##Itemshop category buttons

        # for i in xrange(min(10, len(self.categorys['itemshop']))):
            # self.elements['buttons']['itemshop']['category_%d' % i] = self.CreateCategoryButton(self.elements['pages']['itemshop'], 44, 47+30*i,self.categorys['itemshop'][i][0], self.__OnClickItemshopCategory, self.categorys['itemshop'][i][1])
        
        # for i in xrange(min(10, len(self.categorys['voteshop']))):
            # self.elements['buttons']['voteshop']['category_%d' % i] = self.CreateCategoryButton(self.elements['pages']['voteshop'], 44, 47+30*i,self.categorys['voteshop'][i][0], self.__OnClickVoteshopCategory, self.categorys['voteshop'][i][1])
        
        self.ItemshopCategoryRefresh()
        self.VoteshopCategoryRefresh()
        self.AchievementshopCategoryRefresh()
        
        #self.webWnd = uiWeb.WebWindow()
        #self.webWnd.LoadWindow()
        #self.webWnd.Hide()

        self.PopUp = PricePopUp()
        self.PopUp.BindToolTipItem(self.itemtoolTip)

        ITEM_WITH = 132 + 3
        ITEM_HEIGHT = 140 + 3
        X = 180
        Y = 0

        self.scrollBarItemShopListBox = ScrollBar()
        self.scrollBarItemShopListBox.SetParent(self.elements['pages']['itemshop'])
        self.scrollBarItemShopListBox.SetPosition(ITEM_WITH * 4 + 5 + X, Y)
        self.scrollBarItemShopListBox.SetScrollBarSize(ITEM_HEIGHT    * 3 + 30)
        self.scrollBarItemShopListBox.Show()

        self.itemShopListBox = ItemShopListBox(ItemShopItem)
        self.itemShopListBox.SetParent(self.elements['pages']['itemshop'])
        self.itemShopListBox.SetSize(ITEM_WITH * 4, ITEM_HEIGHT * 3)
        self.itemShopListBox.SetPosition(X, 15 + Y)
        self.itemShopListBox.SetViewItemCount(4, 3, self.itemtoolTip, self.Close, self.scrollBarItemShopListBox.OnMouseWheelScroll_ScrollBar)
        self.itemShopListBox.SetItemSize(ITEM_WITH, ITEM_HEIGHT)
        self.itemShopListBox.SetScrollBar(self.scrollBarItemShopListBox)
        self.itemShopListBox.SetMouseWheelScrollEvent(self.scrollBarItemShopListBox.OnMouseWheelScroll_ScrollBar)
        self.itemShopListBox.Show()

    def SendSystemChat(self, text):
        chat.AppendChat(chat.CHAT_TYPE_INFO, "<System>: "+str(text))


    ## Bind events to the objects in __LoadScript
    def BindEvents(self):
        self.elements['titlebar'].SetCloseEvent(ui.__mem_func__(self.__OnClickClose))
        # self.elements['buttons']['menue']['question'].SetEvent(self.__OnClickQuestion)
        self.elements['buttons']['startpage']['startpage'].SetEvent(self.ChangePage, 'STARTPAGE')
        self.elements['buttons']['startpage']['itemshop'].SetEvent(self.ChangePage, 'ITEMSHOP')
        self.elements['buttons']['startpage']['voteshop'].SetEvent(self.ChangePage, 'VOTESHOP')
        self.elements['buttons']['startpage']['achievementshop'].SetEvent(self.ChangePage, 'ACHIEVEMENTSHOP')
        self.elements['buttons']['startpage']['buy_coins'].SetEvent(self.__OnClickBuyCoins)
        self.elements['buttons']['startpage']['buy_coins'].Hide()
        # self.elements['buttons']['startpage']['vote'].SetEvent(self.__OnClickVote)
        
        self.elements['buttons']['startpage']['achievementshop'].Hide()
        
        ## Change Banner
        self.elements['buttons']['startpage']['banner_0'].SetToggleDownEvent(lambda arg = 0: self.__OnClickBannerBtn(arg))
        self.elements['buttons']['startpage']['banner_0'].SetToggleUpEvent(lambda arg = 0: self.__OnClickBannerBtn(arg))
        self.elements['buttons']['startpage']['banner_1'].SetToggleDownEvent(lambda arg = 1: self.__OnClickBannerBtn(arg))
        self.elements['buttons']['startpage']['banner_1'].SetToggleUpEvent(lambda arg = 1: self.__OnClickBannerBtn(arg))

        ## Item boxes most bought
        self.elements['itemBoxes']['startpage']['mostBought']['box_00'].Open(self.elements['pages']['startpage'],91+8-36,389-100+8,self.itemtoolTip)
        self.elements['itemBoxes']['startpage']['mostBought']['box_01'].Open(self.elements['pages']['startpage'],(91+8-36)+137,389-100+8,self.itemtoolTip)
        self.elements['itemBoxes']['startpage']['mostBought']['box_02'].Open(self.elements['pages']['startpage'],(91+8-36)+(137*2),389-100+8,self.itemtoolTip)

        ## Item boxes hot offers
        self.elements['itemBoxes']['startpage']['hotOffers']['box_00'].Open(self.elements['pages']['startpage'],540,389-100+8,self.itemtoolTip)
        self.elements['itemBoxes']['startpage']['hotOffers']['box_01'].Open(self.elements['pages']['startpage'],603,307-100,self.itemtoolTip)

        ## Item boxes itemshop
        self.elements['itemBoxes']['itemshop']['box_00'].Open(self.elements['pages']['itemshop'],175,106-100,self.itemtoolTip)
        self.elements['itemBoxes']['itemshop']['box_01'].Open(self.elements['pages']['itemshop'],172+142,106-100,self.itemtoolTip)
        self.elements['itemBoxes']['itemshop']['box_02'].Open(self.elements['pages']['itemshop'],172+142+140,106-100,self.itemtoolTip)
        self.elements['itemBoxes']['itemshop']['box_03'].Open(self.elements['pages']['itemshop'],172+142+(140*2),106-100,self.itemtoolTip)

        self.elements['itemBoxes']['itemshop']['box_04'].Open(self.elements['pages']['itemshop'],175,215-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['itemshop']['box_05'].Open(self.elements['pages']['itemshop'],172+142,216-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['itemshop']['box_06'].Open(self.elements['pages']['itemshop'],172+142+140,216-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['itemshop']['box_07'].Open(self.elements['pages']['itemshop'],172+142+(140*2),216-100+32,self.itemtoolTip)

        self.elements['itemBoxes']['itemshop']['box_08'].Open(self.elements['pages']['itemshop'],175,356-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['itemshop']['box_09'].Open(self.elements['pages']['itemshop'],172+142,356-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['itemshop']['box_010'].Open(self.elements['pages']['itemshop'],172+142+140,356-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['itemshop']['box_011'].Open(self.elements['pages']['itemshop'],172+142+(140*2),356-100+32,self.itemtoolTip)


        

        
        ## Item boxes voteshop
        self.elements['itemBoxes']['voteshop']['box_00'].Open(self.elements['pages']['voteshop'],175,106-100,self.itemtoolTip)
        self.elements['itemBoxes']['voteshop']['box_01'].Open(self.elements['pages']['voteshop'],172+142,106-100,self.itemtoolTip)
        self.elements['itemBoxes']['voteshop']['box_02'].Open(self.elements['pages']['voteshop'],172+142+140,106-100,self.itemtoolTip)
        self.elements['itemBoxes']['voteshop']['box_03'].Open(self.elements['pages']['voteshop'],172+142+(140*2),106-100,self.itemtoolTip)

        self.elements['itemBoxes']['voteshop']['box_04'].Open(self.elements['pages']['voteshop'],175,215-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['voteshop']['box_05'].Open(self.elements['pages']['voteshop'],172+142,216-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['voteshop']['box_06'].Open(self.elements['pages']['voteshop'],172+142+140,216-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['voteshop']['box_07'].Open(self.elements['pages']['voteshop'],172+142+(140*2),216-100+32,self.itemtoolTip)

        self.elements['itemBoxes']['voteshop']['box_08'].Open(self.elements['pages']['voteshop'],175,356-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['voteshop']['box_09'].Open(self.elements['pages']['voteshop'],172+142,356-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['voteshop']['box_010'].Open(self.elements['pages']['voteshop'],172+142+140,356-100+32,self.itemtoolTip)
        self.elements['itemBoxes']['voteshop']['box_011'].Open(self.elements['pages']['voteshop'],172+142+(140*2),356-100+32,self.itemtoolTip)
        



        ## Item boxes achievementshop
        self.elements['itemBoxes']['achievementshop']['box_00'].Open(self.elements['pages']['achievementshop'],259,126-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_01'].Open(self.elements['pages']['achievementshop'],405,126-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_02'].Open(self.elements['pages']['achievementshop'],551,126-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_03'].Open(self.elements['pages']['achievementshop'],259,216-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_04'].Open(self.elements['pages']['achievementshop'],405,216-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_05'].Open(self.elements['pages']['achievementshop'],551,216-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_06'].Open(self.elements['pages']['achievementshop'],259,306-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_07'].Open(self.elements['pages']['achievementshop'],405,306-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_08'].Open(self.elements['pages']['achievementshop'],551,306-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_09'].Open(self.elements['pages']['achievementshop'],259,396-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_010'].Open(self.elements['pages']['achievementshop'],405,396-100,self.itemtoolTip)
        self.elements['itemBoxes']['achievementshop']['box_011'].Open(self.elements['pages']['achievementshop'],551,396-100,self.itemtoolTip)

        ## Arrows Startpage
        self.elements['buttons']['startpage']['mostBought_left'].SetEvent(self.__OnClickArrow, 'MOSTBOUGHT_LEFT')
        self.elements['buttons']['startpage']['mostBought_right'].SetEvent(self.__OnClickArrow, 'MOSTBOUGHT_RIGHT')
        self.elements['buttons']['startpage']['hotOffers_up'].SetEvent(self.__OnClickArrow, 'HOTOFFERS_UP')
        self.elements['buttons']['startpage']['hotOffers_down'].SetEvent(self.__OnClickArrow, 'HOTOFFERS_DOWN')

        ## Arrows Itemshop
        self.elements['buttons']['itemshop']['onSale_left'].SetEvent(self.__OnClickArrow, 'ITEMSHOP_ONSALE_LEFT')
        self.elements['buttons']['itemshop']['onSale_right'].SetEvent(self.__OnClickArrow, 'ITEMSHOP_ONSALE_RIGHT')
        self.elements['buttons']['itemshop']['onSale_up'].SetEvent(self.__OnClickArrow, 'ITEMSHOP_ONSALE_UP')
        self.elements['buttons']['itemshop']['onSale_down'].SetEvent(self.__OnClickArrow, 'ITEMSHOP_ONSALE_DOWN')
        
        ## Arrows Voteshop
        self.GetChild("btn_voteshop").Hide()
        self.GetChild("sb_v_coins_text").Hide()
        self.GetChild("sb_v_coins").Hide()
        self.elements['buttons']['voteshop']['onSale_left'].SetEvent(self.__OnClickArrow, 'VOTESHOP_ONSALE_LEFT')
        self.elements['buttons']['voteshop']['onSale_right'].SetEvent(self.__OnClickArrow, 'VOTESHOP_ONSALE_RIGHT')
        self.elements['buttons']['voteshop']['onSale_up'].SetEvent(self.__OnClickArrow, 'VOTESHOP_ONSALE_UP')
        self.elements['buttons']['voteshop']['onSale_down'].SetEvent(self.__OnClickArrow, 'VOTESHOP_ONSALE_DOWN')
        
        ## Arrows Achievementshop
        self.elements['buttons']['achievementshop']['onSale_left'].SetEvent(self.__OnClickArrow, 'ACHIEVEMENTSHOP_ONSALE_LEFT')
        self.elements['buttons']['achievementshop']['onSale_right'].SetEvent(self.__OnClickArrow, 'ACHIEVEMENTSHOP_ONSALE_RIGHT')
        self.elements['buttons']['achievementshop']['onSale_up'].SetEvent(self.__OnClickArrow, 'ACHIEVEMENTSHOP_ONSALE_UP')
        self.elements['buttons']['achievementshop']['onSale_down'].SetEvent(self.__OnClickArrow, 'ACHIEVEMENTSHOP_ONSALE_DOWN')
        
        ## Textlines
        self.elements['textline']['itemshop']['page_nr'].SetParent(self.elements['pages']['itemshop'])
        self.elements['textline']['voteshop']['page_nr'].SetParent(self.elements['pages']['voteshop'])
        self.elements['textline']['achievementshop']['page_nr'].SetParent(self.elements['pages']['achievementshop'])

        for x in xrange(0,12):
            self.elements['itemBoxes']['itemshop']['box_0%d'%(x)].SetEscapeEvent(ui.__mem_func__(self.Close))
            self.elements['itemBoxes']['voteshop']['box_0%d'%(x)].SetEscapeEvent(ui.__mem_func__(self.Close))
            self.elements['itemBoxes']['achievementshop']['box_0%d'%(x)].SetEscapeEvent(ui.__mem_func__(self.Close))

        for x in xrange(0,3):
            self.elements['itemBoxes']['startpage']['mostBought']['box_0%d'%(x)].SetEscapeEvent(ui.__mem_func__(self.Close))

        for x in xrange(0,2):
            self.elements['itemBoxes']['startpage']['hotOffers']['box_0%d'%(x)].SetEscapeEvent(ui.__mem_func__(self.Close))


    def __del__(self):
        ui.ScriptWindow.__del__(self)
        
    ## Hide the Gui
    def Destroy(self):
        self.Hide()
        
    ## Open the gui
    def Open(self, isCoins, vsCoins, asCoins, banner_0, banner_1):
        if FALSE == self.isLoaded:
            self.__LoadScript()
        
        self.bannerOptions['banner_0'] = banner_0
        self.bannerOptions['banner_1'] = banner_1
        self.SetItemshopCoins(isCoins)
        self.SetVoteshopCoins(vsCoins)
        self.SetAchievementshopCoins(asCoins)
        self.page['curPage'] = 'STARTPAGE'
        self.ChangePage('STARTPAGE')
        self.ChangeBannerButton(0)
        self.SwitchBanner(0)
        self.SetTop()
        self.Show()
        
    ## Close the gui
    def Close(self):
        self.Hide()
        
    ## Hide other pages and load the new Page
    def ChangePage(self, page):
        self.page['curPage'] = page

        self.elements['buttons']['startpage']['base_categorias'].Hide()
        self.elements['buttons']['startpage']['base_inicio'].Hide()
        #self.GetChild("back_img").Hide()

        if page == 'STARTPAGE':
            self.arrows['startpage']['mostBought'] = 1
            self.arrows['startpage']['hotOffers'] = 1
            
            self.HotOffersItemsRefresh()
            self.MostBoughtItemsRefresh()

            self.elements['pages']['itemshop'].Hide()
            self.elements['pages']['voteshop'].Hide()
            self.elements['pages']['achievementshop'].Hide()
            self.elements['pages']['startpage'].Show()

            self.elements['buttons']['startpage']['base_inicio'].Show()
            
        elif page == 'ITEMSHOP':
            self.arrows['itemshop']['arrowOnSale'] = 1
            self.category['itemshop'] = self.categorys['itemshop'][0][1]
            self.arrows['itemshop']['arrowCategory'] = 0
            
            self.ItemshopCategoryRefresh()
            self.ItemshopItemsRefresh()

            self.elements['pages']['startpage'].Hide()
            self.elements['pages']['voteshop'].Hide()
            self.elements['pages']['achievementshop'].Hide()
            self.elements['pages']['itemshop'].Show()

            self.elements['buttons']['startpage']['base_categorias'].Show()
            #self.GetChild("back_img").Show()

        elif page == 'VOTESHOP':
            self.arrows['voteshop']['arrowOnSale'] = 1
            self.category['voteshop'] = self.categorys['voteshop'][0][1]
            self.arrows['voteshop']['arrowCategory'] = 0
            
            self.VoteshopCategoryRefresh()
            self.VoteshopItemsRefresh()

            self.elements['pages']['startpage'].Hide()
            self.elements['pages']['itemshop'].Hide()
            self.elements['pages']['achievementshop'].Hide()
            self.elements['pages']['voteshop'].Hide()

            self.elements['buttons']['startpage']['base_categorias'].Hide()
        
        elif page == 'ACHIEVEMENTSHOP':
            self.arrows['achievementshop']['arrowOnSale'] = 1
            self.category['achievementshop'] = self.categorys['achievementshop'][0][1]
            self.arrows['achievementshop']['arrowCategory'] = 0
            
            self.AchievementshopCategoryRefresh()
            self.AchievementshopItemsRefresh()

            self.elements['pages']['startpage'].Hide()
            self.elements['pages']['itemshop'].Hide()
            self.elements['pages']['voteshop'].Hide()
            self.elements['pages']['achievementshop'].Show()

    def ChangeBannerButton(self, enable):
        if enable == 0:
            self.elements['buttons']['startpage']['banner_1'].SetUp()
            self.elements['buttons']['startpage']['banner_0'].Down()
        elif enable == 1:
            self.elements['buttons']['startpage']['banner_0'].SetUp()
            self.elements['buttons']['startpage']['banner_1'].Down()

    #########################
    ## REFRESH ITEMS START ##
    #########################

    def HotOffersItemsRefresh(self):
        curPage = self.arrows['startpage']['hotOffers']
        ## Hide Itemboxes
        for i in xrange(2):
            self.elements['itemBoxes']['startpage']['hotOffers']['box_0%d' % i].Hide()

        ## Load hotOffers first itemboxes
        for i in xrange(min(1, len(constInfo.ITEMSHOP['items']['startpage']['hotOffers']) - curPage * 1 +1)):
            curItem = constInfo.ITEMSHOP['items']['startpage']['hotOffers'][i + (curPage - 1)*1]
            self.elements['itemBoxes']['startpage']['hotOffers']['box_0%d' % i].SetContent(curItem[0], curItem[1], curItem[2], curItem[3]) ## type, itemvnum, itemdetails, price
            self.elements['itemBoxes']['startpage']['hotOffers']['box_0%d' % i].SetPercent(curItem[4]) ## percent
            self.elements['itemBoxes']['startpage']['hotOffers']['box_0%d' % i].SetTime(curItem[5], curItem[6]) ## time , runOut
            self.elements['itemBoxes']['startpage']['hotOffers']['box_0%d' % i].Show()
        
        if curPage * 1 >= len(constInfo.ITEMSHOP['items']['startpage']['hotOffers']):
            self.elements['buttons']['startpage']['hotOffers_down'].Hide()
        else:
            self.elements['buttons']['startpage']['hotOffers_down'].Show()

        if curPage > 1:
            self.elements['buttons']['startpage']['hotOffers_up'].Show()
        else:
            self.elements['buttons']['startpage']['hotOffers_up'].Hide()

    def MostBoughtItemsRefresh(self):
        curPage = self.arrows['startpage']['mostBought']
        ## Hide Itemboxes
        for i in xrange(3):
            self.elements['itemBoxes']['startpage']['mostBought']['box_0%d' % i].Hide()

        ## Load mostBought first itemboxes
        for i in xrange(min(3, len(constInfo.ITEMSHOP['items']['startpage']['mostBought']) - curPage * 3 +3)):
            curItem = constInfo.ITEMSHOP['items']['startpage']['mostBought'][i + (curPage - 1)*3]
            self.elements['itemBoxes']['startpage']['mostBought']['box_0%d' % i].SetContent(curItem[0], curItem[1], curItem[2], curItem[3]) ## type, itemvnum, itemdetails, price
            self.elements['itemBoxes']['startpage']['mostBought']['box_0%d' % i].SetPercent(curItem[4]) ## percent
            self.elements['itemBoxes']['startpage']['mostBought']['box_0%d' % i].SetTime(curItem[5], curItem[6]) ## time , runOut
            self.elements['itemBoxes']['startpage']['mostBought']['box_0%d' % i].Show()
        
        if curPage * 3 >= len(constInfo.ITEMSHOP['items']['startpage']['mostBought']):
            self.elements['buttons']['startpage']['mostBought_right'].Hide()
        else:
            self.elements['buttons']['startpage']['mostBought_right'].Show()

        if curPage > 1:
            self.elements['buttons']['startpage']['mostBought_left'].Show()
        else:
            self.elements['buttons']['startpage']['mostBought_left'].Hide()

    def ItemshopItemsRefresh(self):
        curPage = self.arrows['itemshop']['arrowOnSale']
        ## Hide Itemboxes
        for i in xrange(12):
            self.elements['itemBoxes']['itemshop']['box_0%d' % i].Hide()

        ## Load mostBought first itemboxes
        try:
            self.itemShopItemList = []
            self.itemShopListBox.RemoveAllItems()
            
            for i in range(0, len(constInfo.ITEMSHOP['items']['itemshop'][self.category['itemshop']])):
                self.itemShopListBox.AppendData(constInfo.ITEMSHOP['items']['itemshop'][self.category['itemshop']][i])

            self.itemShopListBox.SetBasePos(0)
            #########################################################
            ## NEW
            #########################################################

        except:
            self.SendChat('Here, no articles were found.')
            self.SendChat('Error persists, contact a staff member.')
            self.elements['buttons']['itemshop']['onSale_right'].Hide()
            self.elements['buttons']['itemshop']['onSale_left'].Hide()
            self.elements['textline']['itemshop']['page_nr'].Hide()
            return
            
        maxPage = self.RoundUp(float(len(constInfo.ITEMSHOP['items']['itemshop'][self.category['itemshop']]))/float(12))
        if maxPage > 1:
            self.elements['textline']['itemshop']['page_nr'].SetText(str(curPage) + ' / ' + str(maxPage))
            self.elements['textline']['itemshop']['page_nr'].Show()
        else:
            self.elements['textline']['itemshop']['page_nr'].Hide()
        
        if curPage * 12 >= len(constInfo.ITEMSHOP['items']['itemshop'][self.category['itemshop']]):
            self.elements['buttons']['itemshop']['onSale_right'].Hide()
        else:
            self.elements['buttons']['itemshop']['onSale_right'].Show()

        if curPage > 1:
            self.elements['buttons']['itemshop']['onSale_left'].Show()
        else:
            self.elements['buttons']['itemshop']['onSale_left'].Hide()

        self.elements['textline']['itemshop']['page_nr'].Hide()
        self.elements['buttons']['itemshop']['onSale_left'].Hide()
        self.elements['buttons']['itemshop']['onSale_right'].Hide()
        self.GetChild("tx_IsOnSale_pageNr_img").Hide()

    def VoteshopItemsRefresh(self):
        curPage = self.arrows['voteshop']['arrowOnSale']
        ## Hide Itemboxes 
        for i in xrange(12):
            self.elements['itemBoxes']['voteshop']['box_0%d' % i].Hide()

        ## Load mostBought first itemboxes
        try:
            for i in xrange(min(12, len(constInfo.ITEMSHOP['items']['voteshop'][self.category['voteshop']]) - curPage * 12 +12)):
                curItem = constInfo.ITEMSHOP['items']['voteshop'][self.category['voteshop']][i + (curPage - 1)*12]
                self.elements['itemBoxes']['voteshop']['box_0%d' % i].SetContent(curItem[0], curItem[1], curItem[2], curItem[3]) ## type, itemvnum, itemdetails, price
                self.elements['itemBoxes']['voteshop']['box_0%d' % i].SetPercent(curItem[4]) ## percent
                self.elements['itemBoxes']['voteshop']['box_0%d' % i].SetTime(curItem[5], curItem[6]) ## time , runOut
                self.elements['itemBoxes']['voteshop']['box_0%d' % i].Show()
        except:
            self.SendChat('Here, no articles were found.')
            self.SendChat('Error persists, contact a staff member.')
            self.elements['buttons']['voteshop']['onSale_right'].Hide()
            self.elements['buttons']['voteshop']['onSale_left'].Hide()
            self.elements['textline']['voteshop']['page_nr'].Hide()
            return
        
        maxPage = self.RoundUp(float(len(constInfo.ITEMSHOP['items']['voteshop'][self.category['voteshop']]))/float(12))
        if maxPage > 1:
            self.elements['textline']['voteshop']['page_nr'].SetText(str(curPage) + ' / ' + str(maxPage))
            self.elements['textline']['voteshop']['page_nr'].Show()
        else:
            self.elements['textline']['voteshop']['page_nr'].Hide()
            
        if curPage * 12 >= len(constInfo.ITEMSHOP['items']['voteshop'][self.category['voteshop']]):
            self.elements['buttons']['voteshop']['onSale_right'].Hide()
        else:
            self.elements['buttons']['voteshop']['onSale_right'].Show()

        if curPage > 1:
            self.elements['buttons']['voteshop']['onSale_left'].Show()
        else:
            self.elements['buttons']['voteshop']['onSale_left'].Hide()
            
    def AchievementshopItemsRefresh(self):
        curPage = self.arrows['achievementshop']['arrowOnSale']
        # self.SendSystemChat('curPage: '+str(curPage))
        ## Hide Itemboxes 
        for i in xrange(12):
            self.elements['itemBoxes']['achievementshop']['box_0%d' % i].Hide()

        ## Load mostBought first itemboxes
        try:
            for i in xrange(min(12, len(constInfo.ITEMSHOP['items']['achievementshop'][self.category['achievementshop']]) - curPage * 12 +12)):
                curItem = constInfo.ITEMSHOP['items']['achievementshop'][self.category['achievementshop']][i + (curPage - 1)*12]
                # self.SendSystemChat('curItem: '+ str(i)+ ' ' +str(curItem))
                self.elements['itemBoxes']['achievementshop']['box_0%d' % i].SetContent(curItem[0], curItem[1], curItem[2], curItem[3]) ## type, itemvnum, itemdetails, price
                self.elements['itemBoxes']['achievementshop']['box_0%d' % i].SetPercent(curItem[4]) ## percent
                self.elements['itemBoxes']['achievementshop']['box_0%d' % i].SetTime(curItem[5], curItem[6]) ## time , runOut
                self.elements['itemBoxes']['achievementshop']['box_0%d' % i].Show()
                # self.SendSystemChat('curItem: added' + str(i))
        except:
            self.SendChat('Here, no articles were found.')
            self.SendChat('Error persists, contact a staff member.')
            self.elements['buttons']['achievementshop']['onSale_right'].Hide()
            self.elements['buttons']['achievementshop']['onSale_left'].Hide()
            self.elements['textline']['achievementshop']['page_nr'].Hide()
            return
        
        maxPage = self.RoundUp(float(len(constInfo.ITEMSHOP['items']['achievementshop'][self.category['achievementshop']]))/float(12))
        if maxPage > 1:
            self.elements['textline']['achievementshop']['page_nr'].SetText(str(curPage) + ' / ' + str(maxPage))
            self.elements['textline']['achievementshop']['page_nr'].Show()
        else:
            self.elements['textline']['achievementshop']['page_nr'].Hide()
            
        if curPage * 12 >= len(constInfo.ITEMSHOP['items']['achievementshop'][self.category['achievementshop']]):
            self.elements['buttons']['achievementshop']['onSale_right'].Hide()
        else:
            self.elements['buttons']['achievementshop']['onSale_right'].Show()

        if curPage > 1:
            self.elements['buttons']['achievementshop']['onSale_left'].Show()
        else:
            self.elements['buttons']['achievementshop']['onSale_left'].Hide()
    
    def ItemshopCategoryRefresh(self):
        #try:
        #    for i in xrange(17):
        #        self.elements['buttons']['itemshop']['category_%d' % i].Hide()
        #except:
        #    pass
        #try:
        #    for i in xrange(min(17, len(self.categorys['itemshop']))):
        #        scrolledId = i + self.arrows['itemshop']['arrowCategory']
        #        self.elements['buttons']['itemshop']['category_%d' % i] = self.CreateCategoryButton(self.elements['pages']['itemshop'], 17, 2+25*i,self.categorys['itemshop'][scrolledId][0], self.__OnClickItemshopCategory, self.categorys['itemshop'][scrolledId][1])
        #        self.elements['buttons']['itemshop']['category_%d' % i].Show()
        #except:
        #    pass
            
        #if (len(self.categorys['itemshop']) > 17):
            #if (self.arrows['itemshop']['arrowCategory'] <= 0):
                #self.elements['buttons']['itemshop']['onSale_down'].Show()
                #self.elements['buttons']['itemshop']['onSale_up'].Hide()
            #elif (self.arrows['itemshop']['arrowCategory']+17 < len(self.categorys['itemshop'])):
                #self.elements['buttons']['itemshop']['onSale_down'].Show()
                #self.elements['buttons']['itemshop']['onSale_up'].Show()
            #elif (self.arrows['itemshop']['arrowCategory']+17 >= len(self.categorys['itemshop'])):
                #self.elements['buttons']['itemshop']['onSale_down'].Hide()
                #self.elements['buttons']['itemshop']['onSale_up'].Show()
        #else:
        
        

        self.elements['buttons']['itemshop']['onSale_down'].Hide()
        self.elements['buttons']['itemshop']['onSale_up'].Hide()
        
        self.scrollBarListBoxCategory = ScrollBar()
        self.scrollBarListBoxCategory.SetDown(-40)
        self.scrollBarListBoxCategory.SetParent(self.elements['pages']['itemshop'])
        self.scrollBarListBoxCategory.SetPosition(151+18, 0)
        self.scrollBarListBoxCategory.SetScrollBarSize(135    * 3 + 30)
        self.scrollBarListBoxCategory.SetFocus()
        self.scrollBarListBoxCategory.Show()

        self.listBoxCategory = DropdownTree()
        self.listBoxCategory.SetParent(self.elements['pages']['itemshop'])
        self.listBoxCategory.SetPosition(15, 10)
        self.listBoxCategory.SetSize(150, 500)
        self.listBoxCategory.SetItemSize(150, 35)
        self.listBoxCategory.SetViewItemCount(12)
        self.listBoxCategory.SetItemStep(37, 20)
        self.listBoxCategory.SetScrollBar(self.scrollBarListBoxCategory)
        self.listBoxCategory.SetMouseWheelScrollEvent(self.scrollBarListBoxCategory.OnMouseWheelScroll_ScrollBar)
        self.listBoxCategory.Show()
        
        # self.__OnClickItemshopCategory, self.categorys['itemshop'][scrolledId][1]
        
        listBoxCategoriItems = []

        for category in constInfo.ITEMSHOP['category']:
            listSubCategories = list(())
            if category[0] in constInfo.ITEMSHOP['subCategories']:
                for subCategory in constInfo.ITEMSHOP['subCategories'][category[0]]:
                    onEvent = lambda arg = subCategory[0] : self.__OnClickItemshopCategory2(arg)
                    listSubCategories.append({'item' : self.CreateCategoryMenuTab2Item(subCategory[1], onEvent)})

            onEvent = lambda arg = category[0] : self.__OnClickItemshopCategory2(arg)
            if len(listSubCategories) > 0:
                onEvent = None

            listBoxCategoriItems.append({
                    'item' : self.CreateCategoryMenuTab1Item(category[1], category[2], len(listSubCategories), onEvent, category[3]),
                    'children': tuple(listSubCategories),
                })

        self.listBoxCategory.AppendItemList(listBoxCategoriItems)
        self.listBoxCategory.SetBasePos(0)

    def __OnClickItemshopCategory2(self, arg):
        self.__OnClickItemshopCategory(arg)
        #self.GetChild("TitleName").SetText(str(arg))
        
    def CreateCategoryMenuTab1Item(self, text, image, amount,event, kImg, offset = 0):
        listboxItem = CategoryMenuTab1(text, image, amount, kImg)
        listboxItem.SetVisible(True)
        listboxItem.SetOffset(offset)
        listboxItem.SetEvent(event)
        listboxItem.SetMouseWheelScrollEvent2(self.scrollBarListBoxCategory.OnMouseWheelScroll_ScrollBar)
        return listboxItem

    def CreateCategoryMenuTab2Item(self, text, event, offset = 2):
        listboxItem = CategoryMenuTab2(text)
        listboxItem.SetSubTree(True)
        listboxItem.SetOffset(offset)
        listboxItem.SetEvent(event)
        listboxItem.SetMouseWheelScrollEvent2(self.scrollBarListBoxCategory.OnMouseWheelScroll_ScrollBar)
        return listboxItem
        
    def VoteshopCategoryRefresh(self):
        try:
            for i in xrange(17):
                self.elements['buttons']['voteshop']['category_%d' % i].Hide()
        except:
            pass
        try:
            for i in xrange(min(17, len(self.categorys['voteshop']))):
                scrolledId = i + self.arrows['voteshop']['arrowCategory']
                self.elements['buttons']['voteshop']['category_%d' % i] = self.CreateCategoryButton(self.elements['pages']['voteshop'], 17, 2+25*i,self.categorys['voteshop'][scrolledId][0], self.__OnClickVoteshopCategory, self.categorys['voteshop'][scrolledId][1])
                self.elements['buttons']['voteshop']['category_%d' % i].Show()
        except:
            pass
            
        #if (len(self.categorys['voteshop']) > 10):
            #if (self.arrows['voteshop']['arrowCategory'] <= 0):
                #self.elements['buttons']['voteshop']['onSale_down'].Show()
                #self.elements['buttons']['voteshop']['onSale_up'].Hide()
            #elif (self.arrows['voteshop']['arrowCategory']+10 < len(self.categorys['voteshop'])):
                #self.elements['buttons']['voteshop']['onSale_down'].Show()
                #self.elements['buttons']['voteshop']['onSale_up'].Show()
            #elif (self.arrows['voteshop']['arrowCategory']+10 >= len(self.categorys['voteshop'])):
                #self.elements['buttons']['voteshop']['onSale_down'].Hide()
                #self.elements['buttons']['voteshop']['onSale_up'].Show()
        #else:
        self.elements['buttons']['voteshop']['onSale_down'].Hide()
        self.elements['buttons']['voteshop']['onSale_up'].Hide()
            
    def AchievementshopCategoryRefresh(self):
        try:
            for i in xrange(10):
                self.elements['buttons']['achievementshop']['category_%d' % i].Hide()
        except:
            pass
        try:
            for i in xrange(min(10, len(self.categorys['achievementshop']))):
                scrolledId = i + self.arrows['achievementshop']['arrowCategory']
                self.elements['buttons']['achievementshop']['category_%d' % i] = self.CreateCategoryButton(self.elements['pages']['achievementshop'], 44, 47+30*i,self.categorys['achievementshop'][scrolledId][0], self.__OnClickAchievementshopCategory, self.categorys['achievementshop'][scrolledId][1])
                self.elements['buttons']['achievementshop']['category_%d' % i].Show()
        except:
            pass
            
        if (len(self.categorys['achievementshop']) > 10):
            if (self.arrows['achievementshop']['arrowCategory'] <= 0):
                self.elements['buttons']['achievementshop']['onSale_down'].Show()
                self.elements['buttons']['achievementshop']['onSale_up'].Hide()
            elif (self.arrows['achievementshop']['arrowCategory']+10 < len(self.categorys['achievementshop'])):
                self.elements['buttons']['achievementshop']['onSale_down'].Show()
                self.elements['buttons']['achievementshop']['onSale_up'].Show()
            elif (self.arrows['achievementshop']['arrowCategory']+10 >= len(self.categorys['achievementshop'])):
                self.elements['buttons']['achievementshop']['onSale_down'].Hide()
                self.elements['buttons']['achievementshop']['onSale_up'].Show()
        else:
            self.elements['buttons']['achievementshop']['onSale_down'].Hide()
            self.elements['buttons']['achievementshop']['onSale_up'].Hide()
        
    #######################
    ## REFRESH ITEMS END ##
    #######################

    #########################
    ## OnClick Events START #
    #########################

    ## Arrows (startpage [mostBought, hotOffers], itemshop, voteshop)
    def __OnClickArrow(self, arrow):
        if arrow == 'MOSTBOUGHT_LEFT':
            self.arrows['startpage']['mostBought'] -= 1
            self.MostBoughtItemsRefresh()
        elif arrow == 'MOSTBOUGHT_RIGHT':
            self.arrows['startpage']['mostBought'] += 1
            self.MostBoughtItemsRefresh()
        elif arrow == 'HOTOFFERS_UP':
            self.arrows['startpage']['hotOffers'] -= 1
            self.HotOffersItemsRefresh()
        elif arrow == 'HOTOFFERS_DOWN':
            self.arrows['startpage']['hotOffers'] += 1
            self.HotOffersItemsRefresh()
        elif arrow == 'ITEMSHOP_ONSALE_LEFT':
            self.arrows['itemshop']['arrowOnSale'] -= 1
            self.ItemshopItemsRefresh()
        elif arrow == 'ITEMSHOP_ONSALE_RIGHT':
            self.arrows['itemshop']['arrowOnSale'] += 1
            self.ItemshopItemsRefresh()
        elif arrow == 'VOTESHOP_ONSALE_LEFT':
            self.arrows['voteshop']['arrowOnSale'] -= 1
            self.VoteshopItemsRefresh()
        elif arrow == 'VOTESHOP_ONSALE_RIGHT':
            self.arrows['voteshop']['arrowOnSale'] += 1
            self.VoteshopItemsRefresh()
        elif arrow == 'VOTESHOP_ONSALE_UP':
            self.arrows['voteshop']['arrowCategory'] -= 1
            self.VoteshopCategoryRefresh()
        elif arrow == 'VOTESHOP_ONSALE_DOWN':
            self.arrows['voteshop']['arrowCategory'] += 1
            self.VoteshopCategoryRefresh()
        elif arrow == 'ITEMSHOP_ONSALE_UP':
            self.arrows['itemshop']['arrowCategory'] -= 1
            self.ItemshopCategoryRefresh()
        elif arrow == 'ITEMSHOP_ONSALE_DOWN':
            self.arrows['itemshop']['arrowCategory'] += 1
            self.ItemshopCategoryRefresh()
        elif arrow == 'ACHIEVEMENTSHOP_ONSALE_LEFT':
            self.arrows['achievementshop']['arrowOnSale'] -= 1
            self.AchievementshopItemsRefresh()
        elif arrow == 'ACHIEVEMENTSHOP_ONSALE_RIGHT':
            self.arrows['achievementshop']['arrowOnSale'] += 1
            self.AchievementshopItemsRefresh()
        elif arrow == 'ACHIEVEMENTSHOP_ONSALE_UP':
            self.arrows['achievementshop']['arrowCategory'] -= 1
            self.AchievementshopItemsRefresh()
        elif arrow == 'ACHIEVEMENTSHOP_ONSALE_DOWN':
            self.arrows['achievementshop']['arrowCategory'] += 1
            self.AchievementshopItemsRefresh()

        #self.GetChild("Menue").Hide()
        

    ## Category Buttons

    def __OnClickItemshopCategory(self, arg):
        self.category['itemshop'] = arg
        self.arrows['itemshop']['arrowOnSale'] = 1
        self.ItemshopItemsRefresh()

    def __OnClickVoteshopCategory(self, arg):
        self.category['voteshop'] = arg
        self.arrows['voteshop']['arrowOnSale'] = 1
        self.VoteshopItemsRefresh()
        
    def __OnClickAchievementshopCategory(self, arg):
        self.category['achievementshop'] = arg
        self.arrows['achievementshop']['arrowOnSale'] = 1
        self.AchievementshopItemsRefresh()

    ## Show the current market price of coins
    def __OnClickQuestion(self):
        if self.PopUp.IsShow():
            self.PopUp.Close()
        else:
            self.PopUp.Open()

    ## Close gui
    def __OnClickClose(self):
        self.Close()

    ## Switch the banner
    def __OnClickBannerBtn(self, arg):
        self.SwitchBanner(arg)

    ## Open link to buy coins
    def __OnClickBuyCoins(self):
        pass
        #self.webWnd.Open(self.link['buyCoins'])

    ## Open link to vote
    # def __OnClickVote(self):
        # self.webWnd.Open(self.link['vote'])

    ########################
    ## OnClick Events END ##
    ########################

    ## Other functions [ConvertNumberToCoins, SetVoteshopCoins, SetItemshopCoins, CreateCategoryButton, SendChat, OnUpdate(BANNER,), SwitchBanner, SetAlpha]

    def ConvertNumberToCoins(self, coins, text):
        if coins <= 0 :
            return("0 %s" % text)

        return("%s" % ('.'.join([ i-3<0 and str(coins)[:i] or str(coins)[i-3:i] for i in range(len(str(coins))%3, len(str(coins))+1, 3) if i ])))

    def SetItemshopCoins(self, coins):
        self.elements['textline']['menue']['itemshop_coins'].SetText('%s' % self.ConvertNumberToCoins(coins, 'Coins'))

    def SetVoteshopCoins(self, coins):
        self.elements['textline']['menue']['voteshop_coins'].SetText('%s' % self.ConvertNumberToCoins(coins, 'Tokens'))
        
    def SetAchievementshopCoins(self, coins):
        self.elements['textline']['menue']['achievementshop_coins'].SetText('%s' % self.ConvertNumberToCoins(coins, 'L-Coins'))

    def CreateCategoryButton(self, parent, x, y, text, func, arg):
        button = ui.Button()
        button.SetParent(parent)
        button.SetUpVisual(IMG_ITEMSHOP+"boton_cat_1.tga")
        button.SetOverVisual(IMG_ITEMSHOP+"boton_cat_2.tga")
        button.SetDownVisual(IMG_ITEMSHOP+"boton_cat_3.tga")
        button.SetText(text)
        button.SetEvent(ui.__mem_func__(func), arg)
        button.SetPosition(x, y)
        button.Show()
        return button


    def SendChat(self, text):
        chat.AppendChat(chat.CHAT_TYPE_INFO, '<Shop>: '+str(text))

    def OnUpdate(self):
        ## Banner UPDATE START

        if self.bannerVar['lastSwitch'] < time.clock():
            if self.bannerVar['currentImage'] == 1:
                self.SwitchBanner(0)
            else:
                self.SwitchBanner(1)

        ## if image fade out activated, start fading out
        if self.bannerVar['fadeOut'] == 1:
            ## get current time
            self.bannerVar['currentTime'] = time.clock()

            ## if alpha value is bigger than zero, then check if it's time to change the alpha value - interval;
            ## else deactivate fade out and hide the fade_banner and change the banner button 

            if self.bannerVar['currentAlphaValue'] > 0.0:
                if self.bannerVar['currentTime'] >= self.bannerVar['intervallEndTime']:
                    newAlphaValue = self.bannerVar['currentAlphaValue'] 
                    newAlphaValue -= self.bannerOptions['interval']
                    self.SetAlpha(self.elements['images']['startpage']['fade_banner'], newAlphaValue)
                    self.bannerVar['intervallEndTime'] = self.bannerVar['currentTime'] + self.bannerOptions['timeToFade']
            else:
                self.bannerVar['fadeOut'] = 0
                self.elements['images']['startpage']['fade_banner'].Hide()
        ## Banner UPDATE END ## 
        
    ## switch the banner with a fade out
    def SwitchBanner(self,newBanner):
        self.bannerVar['lastSwitch'] = time.clock() + self.bannerOptions['time'] + self.bannerOptions['timeToFade']/self.bannerOptions['interval']
        self.elements['images']['startpage']['fade_banner'].LoadImage(self.bannerOptions['folder'] + self.bannerOptions['banner_%d' % self.bannerVar['currentImage']] + '.tga')
        self.elements['images']['startpage']['fade_banner'].Show()
        self.elements['images']['startpage']['banner'].LoadImage(self.bannerOptions['folder'] + self.bannerOptions['banner_%d' % newBanner]+ '.tga')
        self.bannerVar['currentImage'] = newBanner
        self.SetAlpha(self.elements['images']['startpage']['fade_banner'], 1.0)
        self.bannerVar['fadeOut'] = 1
        self.bannerVar['intervallEndTime'] = self.bannerVar['currentTime'] + self.bannerOptions['timeToFade']
        self.ChangeBannerButton(newBanner)

    ## set the alpha value of an image 'transparent'
    def SetAlpha(self, image, alpha):
        self.bannerVar['currentAlphaValue'] = alpha
        image.SetAlpha(alpha)
        
    def RoundUp(self, num):
        if (num + 1) != int(num+1):
            return int(num+1)
        else:
            return int(num)

        
    ## Other functions end

    def OnPressEscapeKey(self):
        self.Close()
        return TRUE

    def OnPressExitKey(self):
        self.Close()
        return TRUE

class ItemBox(ui.ScriptWindow):

    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.isLoaded = FALSE
        self.time = None
        self.runOut = None
        self.price = None
        self.itemData = []
        self.lastTime = None
        self.itemtoolTip = None
        
    def __LoadScript(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, 'uiscript/itemshop_v2_itembox.py')
        except:
            import exception
            exception.Abort('test.__LoadScript.LoadObject')

        try: 
            self.itemBox = self.GetChild('Background')
            self.timeBox = self.GetChild('TimeBox')
            self.countdown = self.GetChild('tx_countdown')
            self.percentBox = self.GetChild('PercentBox')
            self.tx_percent = self.GetChild('tx_percent')
            self.btn_buy = self.GetChild('btn_buy')
            self.itemName = self.GetChild('tx_itemName')
            self.itemPrice = self.GetChild('tx_itemPrice')
            self.amount = self.GetChild('ed_amount')
            self.amountBox = self.GetChild('sb_amount')
            self.icon = self.GetChild('icon_item')
            self.icon_price = self.GetChild("icon_price")


            self.itemBuyQuestionDialog = ItemBuyDialog()
            
        except:
            import exception
            exception.Abort('test.__LoadScript.BindObject')

        try: 
            self.btn_buy.SetEvent(self.__OnClickBuy)
            self.amount.SetNumberMode()
            self.itemBuyQuestionDialog.SetAcceptEvent(lambda arg=TRUE: self.AnswerBuyItem(arg))
            self.itemBuyQuestionDialog.SetCancelEvent(lambda arg=FALSE: self.AnswerBuyItem(arg))


            self.amount.SetEscapeEvent(ui.__mem_func__(self.__OnEscapeEvent))

            self.icon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.Icon_MouseOverIn)
            self.icon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.Icon_MouseOverOut)
        except:
            import exception
            exception.Abort('test.__LoadScript.BindEvent')

        self.isLoaded = TRUE

    def __del__(self):
        ui.ScriptWindow.__del__(self)
        
    def Destroy(self):
        self.Hide()

    def SetEscapeEvent(self, event):
        self.escapeEvent = event            
            
    def __OnEscapeEvent(self):
        if self.escapeEvent:
            self.escapeEvent()

    ## Set the content of this box
    def SetContent(self, type, id, itemData, price):
        self.type = type
        self.itemData = itemData
        self.id = id ## id from mysql, used to identify the item from the table 
        self.price = price
        self.itemPrice.SetText('%d ' % (price))
        self.icon_price.LoadImage(IMG_ITEMSHOP+"%s.tga"%(['icon_coin','icon_vcoin','icon_coin'])[type])
        self.amount.SetText('1')

        item.SelectItem(itemData[0])
        if item.IsFlag(4) == 1:
            self.amountBox.Show()
        else:
            self.amountBox.Hide()

        self.itemName.SetText(item.GetItemName())

        ## Load the image and scale the item if the slotsize is bigger than 1
        self.icon.LoadImage(str(item.GetIconImageFileName()))
        #self.icon.SetScale(1, ([1, 0.6, 0.4])[item.GetItemSize()[1]-1])
        
        self.btn_buy.Enable()
        self.amount.KillFocus()
        self.amount.CanEdit(TRUE)
        self.timeBox.Hide()
        self.percentBox.Hide()

    def SetTime(self, time, runOut):
        if time == 0:
            self.timeBox.Hide()
            self.time = None
            return
        else:
            self.timeBox.Show()
            self.time = time
            self.runOut = runOut
            self.lastTime = 0

    def SetPercent(self, percent):
        if percent == 0:
            self.percentBox.Hide()
            return
        else:
            self.percentBox.LoadImage(IMG_ITEMSHOP+"descuento.tga")
            self.percentBox.Show()
            
            self.itemPrice.SetText('%d ' % (self.price-(self.price/100.00)*percent))
            self.icon_price.LoadImage(IMG_ITEMSHOP+"%s.tga"%(['icon_coin','icon_vcoin','icon_coin'])[self.type])
            self.tx_percent.SetText(str(percent) + '%')
        
    ## Set the parent to attach and the coordinates
    def Open(self,parent,x,y,itemtoolTip):

        self.toolTip = itemtoolTip

        if FALSE == self.isLoaded:
            self.__LoadScript()

        
        self.SetParent(parent)
        self.SetPosition(x,y)
        self.amount.SetText('1')
        self.btn_buy.Disable()
        self.amountBox.Hide()
        self.amount.CanEdit(FALSE)
        
        self.Show()

    def Icon_MouseOverIn(self):
        self.toolTip.ClearToolTip()
        item.SelectItem(self.itemData[0])

        ## if item is real time (limit type) then calculate the time
        if item.GetLimit(0)[0] == 7:
            self.toolTip.AddItemData(self.itemData[0], [self.itemData[1] + app.GetGlobalTimeStamp(),self.itemData[2],self.itemData[3],0,0,0], [(self.itemData[4],self.itemData[5]),(self.itemData[6],self.itemData[7]),(self.itemData[8],self.itemData[9]),(self.itemData[10],self.itemData[11]),(self.itemData[12],self.itemData[13]),(self.itemData[14],self.itemData[15]),(self.itemData[16],self.itemData[17]),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0)])
        else:
            self.toolTip.AddItemData(self.itemData[0], [self.itemData[1],self.itemData[2],self.itemData[3],0,0,0],    [(self.itemData[4],self.itemData[5]),(self.itemData[6],self.itemData[7]),(self.itemData[8],self.itemData[9]),(self.itemData[10],self.itemData[11]),(self.itemData[12],self.itemData[13]),(self.itemData[14],self.itemData[15]),(self.itemData[16],self.itemData[17]),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0),(0,0)])

    def Icon_MouseOverOut(self):
        self.toolTip.Hide()

    def OnUpdate(self):
        amount = self.amount.GetText()
        if amount:
            ## if the first character of amount is a 0 or its empty and not focused, then set the amount to 1
            if (amount != "" and amount[0] == '0') or (not self.amount.IsFocus() and amount == ""):
                self.amount.SetText('1')
            if int(amount) > 200:
                self.amount.SetText('200')

        ## if time is set then calculate the time until end
        if self.time:
            remaining = self.time - app.GetGlobalTimeStamp()
            if self.lastTime < time.clock():
                if remaining <= 0:
                    if self.runOut == 1:
                        self.time = None
                        self.countdown.SetText('Abgelaufen')
                        self.btn_buy.Disable()
                        self.amount.CanEdit(FALSE) ## new function in ui
                    else:
                        self.timeBox.Hide()
                        self.percentBox.Hide()
                        self.itemPrice.SetText('%d %s' % (self.price, (['Coins','Tokens','L-Coins'])[self.type]))
                    return

                self.lastTime = time.clock() + 1
                hoursRemaining = int(remaining) / 3600
                minutesRemaining = int(remaining % 3600) / 60
                secondsRemaining = int(remaining % 60)
                self.countdown.SetText('%dh %dm %ds' % (hoursRemaining, minutesRemaining, secondsRemaining))

    def __OnClickBuy(self):
        if self.amount.GetText() == '':
            self.amount.SetText('1')

        self.amount.KillFocus()
        amount = self.amount.GetText()
        price = int(amount) * int(self.itemPrice.GetText().split(' ')[0])

        if amount == '1':
            self.itemBuyQuestionDialog.SetText("Do you want buy %s for %d %s?" % (self.itemName.GetText(), price, (['Coins','Tokens','L-Coins'])[self.type]))
        else:
            self.itemBuyQuestionDialog.SetText("Do you want buy %sx %s for %d %s?" % (amount, self.itemName.GetText(), price, (['Coins','Tokens','L-Coins'])[self.type]))
        self.itemBuyQuestionDialog.Open()

    def AnswerBuyItem(self, arg):
        self.itemBuyQuestionDialog.Close()
        if arg == 1:
            # self.SendChat('BUY ID: %d AMOUNT: %s' % (self.id, self.amount.GetText()))
            import event
            constInfo.ITEMSHOP["questCMD"] = 'BUY#%d#%s' % (self.id, self.amount.GetText())
            event.QuestButtonClick(int(constInfo.ITEMSHOP["qid"]))
            ## Send buy item : [type, id, amount]
        self.amount.SetText('1')
        
    def SendChat(self, text):
        chat.AppendChat(chat.CHAT_TYPE_INFO, '<Shop>: '+str(text))



class ItemBuyDialog(ui.ScriptWindow):

    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.__CreateDialog()

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def __CreateDialog(self):
        pyScrLoader = ui.PythonScriptLoader()
        pyScrLoader.LoadScriptFile(self, "uiscript/questiondialog.py")

        self.board = self.GetChild("board")
        self.textLine = self.GetChild("message")
        self.acceptButton = self.GetChild("accept")
        self.cancelButton = self.GetChild("cancel")

    def Open(self):
        global IS_BUY
        if IS_BUY == TRUE:
            return
        IS_BUY = TRUE
        self.SetCenterPosition()
        self.SetTop()
        self.Show()
        

    def Close(self):
        global IS_BUY
        IS_BUY = FALSE
        self.Hide()

    def SetWidth(self, width):
        height = self.GetHeight()
        self.SetSize(width, height)
        self.board.SetSize(width, height)
        self.SetCenterPosition()
        self.UpdateRect()

    def SAFE_SetAcceptEvent(self, event):
        self.acceptButton.SAFE_SetEvent(event)

    def SAFE_SetCancelEvent(self, event):
        self.cancelButton.SAFE_SetEvent(event)

    def SetAcceptEvent(self, event):
        self.acceptButton.SetEvent(event)

    def SetCancelEvent(self, event):
        self.cancelButton.SetEvent(event)

    def SetText(self, text):
        self.textLine.SetText(text)

    def SetAcceptText(self, text):
        self.acceptButton.SetText(text)

    def SetCancelText(self, text):
        self.cancelButton.SetText(text)

    def OnPressEscapeKey(self):
        self.Close()
        return TRUE





class PricePopUp(ui.ScriptWindow):

    POPUPTXTLINK = "http://client.connect.ephyra2.info/shopprices.txt"

    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.isLoaded = FALSE
        self.itemtoolTip = None

    def BindToolTipItem(self, itemtoolTip):
        self.itemtoolTip = itemtoolTip

    def __LoadScript(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "uiscript/itemshop_popup.py")
        except:
            import exception
            exception.Abort("test.__LoadScript.LoadObject")

        try: 
            self.board = self.GetChild("ItemshopPopupBoard")
            self.closeBtn = self.GetChild("CloseButton")
            self.itemShopText = {
                        "text_0" : self.GetChild("label_text_0"),
                        "text_1" : self.GetChild("label_text_1"),
                        "text_2" : self.GetChild("label_text_2"),
                        "text_3" : self.GetChild("label_text_3"),
                        }
            self.voteShopText = self.GetChild("label_text_4")
            self.slot = self.GetChild("ItemSlotPreview")

        except:
            import exception
            exception.Abort("test.__LoadScript.BindObject")
            
        self.closeBtn.SetEvent(ui.__mem_func__(self.Close))
        self.toolTip = self.itemtoolTip
        self.slot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
        self.slot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

        self.isLoaded = TRUE

    def __del__(self):
        ui.ScriptWindow.__del__(self)
        
    def Destroy(self):
        self.Close()
        
    def Open(self):
        if FALSE == self.isLoaded:
            self.__LoadScript()
        self.SetTop()
        self.Show()
        self.BuildPage()

    def OverOutItem(self):
        self.toolTip.Hide()

    def OverInItem(self):
        self.toolTip.Show()
        self.toolTip.SetItemToolTip(self.vnum)

    def BuildPage(self):
        try:
            #respons = str(urllib.urlopen(self.POPUPTXTLINK).read()) 
            self.txt = eval(respons)
            self.vnum = None
            count = None
            for i in range(4):
                if self.txt[i].find("[ITEMSHOP_TEXT_"+str(i)+"]") != -1:
                    startIndex = self.txt[i].find("[ITEMSHOP_TEXT_"+str(i)+"]")+17
                    endIndex = self.txt[i].find("[/ITEMSHOP_TEXT_"+str(i)+"]")
                    self.itemShopText['text_'+str(i)].SetText(str((self.txt[i])[startIndex:endIndex]))
            if self.txt[4].find("[VOTESHOP_TEXT_0]") != -1:
                startIndex = self.txt[4].find("[VOTESHOP_TEXT_0]")+17
                endIndex = self.txt[4].find("[/VOTESHOP_TEXT_0]")
                self.voteShopText.SetText(str((self.txt[4])[startIndex:endIndex]))
            if self.txt[5].find("[VOTESHOP_ITEM_VNUM]") != -1:
                startIndex = self.txt[5].find("[VOTESHOP_ITEM_VNUM]")+20
                endIndex = self.txt[5].find("[/VOTESHOP_ITEM_VNUM]")
                self.vnum = int((self.txt[5])[startIndex:endIndex])

            if self.txt[6].find("[VOTESHOP_ITEM_COUNT]") != -1:
                startIndex = self.txt[6].find("[VOTESHOP_ITEM_COUNT]")+21
                endIndex = self.txt[6].find("[/VOTESHOP_ITEM_COUNT]")
                count = int((self.txt[6])[startIndex:endIndex])

            if self.vnum:
                self.slot.Show()
                self.slot.SetItemSlot(0, self.vnum, count)
                self.slot.RefreshSlot()
            else:
                self.slot.Hide()
        except:
            self.Hide()
    
    def SendSystemChat(self, text):
        chat.AppendChat(chat.CHAT_TYPE_INFO, "<System>: "+str(text))
        
    def Close(self):
        self.Hide()

    def OnPressEscapeKey(self):
        self.Close()
        return TRUE

    def OnPressExitKey(self):
        self.Close()
        return TRUE
        
class LoadingBar(ui.ScriptWindow):

    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.isLoaded = FALSE
        

    def __LoadScript(self):
        ## Load gui
        try: 
            self.BindObjects()
        except:
            import exception
            exception.Abort('test.__LoadScript.BindObjects')
            
        ## Load events
        try: 
            self.BindEvents()
        except:
            import exception
            exception.Abort('test.__LoadScript.BindEvents')
            
        self.isLoaded = TRUE
        
    ## Bind the objects in __LoadScript
    def BindObjects(self):
        self.board = ui.ThinBoard()
        self.board.SetSize(260,30)
        self.board.SetCenterPosition()
        self.board.Show()

        self.progressBarActualFile = ui.AniImageBox()
        self.progressBarActualFile.SetParent(self.board)
        self.progressBarActualFile.AppendImage("locale/es/ui/itemshop/loadingBar.tga")
        self.progressBarActualFile.SetPosition(5, 8)
        self.progressBarActualFile.SetDelay(90)
        self.progressBarActualFile.SetPercentage(0, 100)
        self.progressBarActualFile.Show()

        self.lb_0 = ui.TextLine()
        self.lb_0.SetParent(self.board)
        self.lb_0.SetPosition(10,9)
        self.lb_0.SetText(localeInfo.ITEMSHOP_LOAD)
        self.lb_0.Show()

        self.lb_1 = ui.TextLine()
        self.lb_1.SetParent(self.board)
        self.lb_1.SetPosition(225,9)
        self.lb_1.SetText('0%')
        self.lb_1.Show()

    ## Bind events to the objects in __LoadScript
    def BindEvents(self):
        pass
        #self.elements['buttons']['menue']['close'].SetEvent(self.__OnClickClose)

    def __del__(self):
        ui.ScriptWindow.__del__(self)
        
    ## Hide the Gui
    def Destroy(self):
        self.Hide()

    def SetPercent(self, percent):
        if percent >= 100:
            self.board.Hide()
        else:
            self.board.SetTop()
            self.board.Show()
            self.progressBarActualFile.SetPercentage(percent, 100)
            self.lb_1.SetText(str(percent) +'%')
        
    ## Open the gui
    def Open(self):
        if FALSE == self.isLoaded:
            self.__LoadScript()
        self.SetTop()
        self.Show()

    ## Close the gui
    def Close(self):
        self.Hide()

"""
constInfo.ITEMSHOP['category'] = []
constInfo.ITEMSHOP['category'].append([1, "Objetos especiales", "cupones.tga", 1])
constInfo.ITEMSHOP['category'].append([2, "Alquimia", "cupones.tga", 2])

constInfo.ITEMSHOP['subCategories'] = {}
constInfo.ITEMSHOP['subCategories'].update({1: []})
constInfo.ITEMSHOP['subCategories'][1].append([1, "Prueba sub"])
constInfo.ITEMSHOP['subCategories'].update({2: []})


x = Itemshop()
x.Open( 0, 0, 0, "", "")
"""