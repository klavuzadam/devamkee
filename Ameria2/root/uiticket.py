# coding: latin_1

import ui, grp, app, chat, constInfo, event, localeInfo, player
from _weakref import proxy

CATEGORIES = [
    localeInfo.TICKET_CATEGORY_0,
    'Sugerencias',
    'Bugs',
    'Hacks',
    'So on',
]

class TicketWindow(ui.ScriptWindow):

    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.isLoaded = False
        
    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def __LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "uiscript/TicketWindow.py")
        except:
            import exception
            exception.Abort("TicketWindow.__LoadWindow.LoadObject")
        
        self.pages = ['MY_TICKETS', 'ADMINISTRATOR']
        self.priority_levels = [localeInfo.TICKET_PRIORITY_LOW, localeInfo.TICKET_PRIORITY_MEDIUM, localeInfo.TICKET_PRIORITY_HIGH]
        self.sort = [localeInfo.TICKET_DATE, localeInfo.TICKET_STATE, localeInfo.TICKET_PRIORITY]
        self.tmpID = ''
        self.myticketsPos = 0
        self.globalticketsPos = 0
        self.permisionsPos = 0
        self.priority = 0
        self.category = 0
        self.curPage = self.pages[0]
        self.curCategory = 0
        self.lastSort = 0
        self.isGM = False
        self.lastSaw = ''
        self.sortedList = []
        self.isSorting = False
        
        self.__BindObjects()
        self.__BindEvents()
        self.isLoaded = True
        
    def __BindObjects(self):
        self.board = self.GetChild('board')
        
        for i in xrange(2):
            self.GetChild('btnpage%d' % (i+1)).SetEvent(ui.__mem_func__(self.SelectPage), self.pages[i])
            
        self.elements = {
            'listboxes' : {'my_tickets' : self.GetChild('my_tickets_listbox'), 'admin' : self.GetChild('admin_listbox')},
            'scrollbars' : {'my_tickets' : self.GetChild('my_tickets_scrollbar'), 'admin' : self.GetChild('admin_scrollbar'), 'permisions' : self.GetChild('permisions_scrollbar')},
            'editlines' : {'create_ticket' : {'title' : self.GetChild('create_ticket_title_value'), 'msg' : self.GetChild('create_ticket_msg')}, 'search' : self.GetChild('search_value')},
            'comboboxes' : {'create_ticket' : 0, 'sort' : 0},
            'buttons' : {'create_ticket' : 0, 'add_member' : self.GetChild('permisions_add_member'), 'search' : self.GetChild('search_button')},
            'priorities' : {},
            'unique_ticket' : TicketManage(),
            'add_member' : AddMember(),
        }
        for i in xrange(7):
            self.elements.update({'permision_%d' % i: PermisionManage()})
        
        self.elements['comboboxes']['create_ticket'] = ui.ComboBox()        
        self.elements['comboboxes']['create_ticket'].SetParent(self.GetChild('create_ticket_bg'))
        self.elements['comboboxes']['create_ticket'].SetSize(55, 15)
        self.elements['comboboxes']['create_ticket'].SetPosition(78, 46)
        self.elements['comboboxes']['create_ticket'].SetWindowHorizontalAlignRight()
        self.elements['comboboxes']['create_ticket'].SetEvent(self.SelectPriority)
        self.elements['comboboxes']['create_ticket'].Show()
        
        self.elements['comboboxes']['create_ticket_category'] = ui.ComboBox()        
        self.elements['comboboxes']['create_ticket_category'].SetParent(self.GetChild('create_ticket_bg'))
        self.elements['comboboxes']['create_ticket_category'].SetSize(90, 15)
        self.elements['comboboxes']['create_ticket_category'].SetPosition(245, 46)
        self.elements['comboboxes']['create_ticket_category'].SetWindowHorizontalAlignRight()
        self.elements['comboboxes']['create_ticket_category'].SetEvent(self.SelectCategorySend)
        self.elements['comboboxes']['create_ticket_category'].Show()
        
        self.elements['comboboxes']['sort'] = ui.ComboBox()        
        self.elements['comboboxes']['sort'].SetParent(self.GetChild('board'))
        self.elements['comboboxes']['sort'].SetSize(65, 18)
        self.elements['comboboxes']['sort'].SetPosition(34+55, 35)
        self.elements['comboboxes']['sort'].SetWindowHorizontalAlignRight()
        self.elements['comboboxes']['sort'].SetEvent(self.SelectSort)
        self.elements['comboboxes']['sort'].Show()
        
        self.elements['comboboxes']['categories'] = ui.ComboBox()        
        self.elements['comboboxes']['categories'].SetParent(self.GetChild('board'))
        self.elements['comboboxes']['categories'].SetSize(100, 18)
        self.elements['comboboxes']['categories'].SetPosition(180+68+14, 35)
        self.elements['comboboxes']['categories'].SetWindowHorizontalAlignRight()
        self.elements['comboboxes']['categories'].SetEvent(self.SelectCategory)
        self.elements['comboboxes']['categories'].Show()
        
        self.elements['buttons']['create_ticket'] = ui.CoolButton()
        self.elements['buttons']['create_ticket'].SetParent(self.GetChild('create_ticket_bg_msg2'))
        self.elements['buttons']['create_ticket'].SetSize(95, 50)
        self.elements['buttons']['create_ticket'].SetPosition(102, 0)
        self.elements['buttons']['create_ticket'].SetWindowHorizontalAlignRight()
        self.elements['buttons']['create_ticket'].SetWindowVerticalAlignCenter()
        self.elements['buttons']['create_ticket'].SetEvent(ui.__mem_func__(self.SendTicket))
        self.elements['buttons']['create_ticket'].SetText(localeInfo.TICKET_SEND)
        self.elements['buttons']['create_ticket'].Show()
        
        idx = 0
        for prty in self.priority_levels:
            self.elements['comboboxes']['create_ticket'].InsertItem(idx, prty)
            idx += 1
            
        idx = 0
        for obj in self.sort:
            self.elements['comboboxes']['sort'].InsertItem(idx, obj)
            idx += 1
            
        idx = 0
        for cat in CATEGORIES:
            self.elements['comboboxes']['categories'].InsertItem(idx, cat)
            idx += 1
            
        for i in xrange(len(CATEGORIES)-1):
            self.elements['comboboxes']['create_ticket_category'].InsertItem(i, CATEGORIES[i+1])
        
    def __BindEvents(self):
        self.board.SetCloseEvent(ui.__mem_func__(self.Close))
        self.elements['comboboxes']['create_ticket'].SelectItem(0)
        self.elements['comboboxes']['create_ticket_category'].SelectItem(0)
        self.elements['editlines']['create_ticket']['title'].SetReturnEvent(ui.__mem_func__(self.elements['editlines']['create_ticket']['msg'].SetFocus))
        self.elements['editlines']['create_ticket']['title'].SetTabEvent(ui.__mem_func__(self.elements['editlines']['create_ticket']['msg'].SetFocus))
        self.elements['editlines']['create_ticket']['msg'].SetTabEvent(ui.__mem_func__(self.elements['editlines']['create_ticket']['title'].SetFocus))
        self.elements['editlines']['create_ticket']['title'].SetEscapeEvent(ui.__mem_func__(self.Close))
        self.elements['editlines']['create_ticket']['msg'].SetEscapeEvent(ui.__mem_func__(self.Close))
        self.elements['editlines']['search'].SetEscapeEvent(ui.__mem_func__(self.Close))
        self.elements['unique_ticket'].Open(self.GetChild('window3'), 1, 5)
        self.elements['unique_ticket'].SetCloseEvent(self.Close)
        self.elements['listboxes']['my_tickets'].SetSelectEvent(self.SelectMyTicket)
        self.elements['scrollbars']['my_tickets'].SetScrollEvent(self.__OnScrollMyTickets)
        self.elements['listboxes']['admin'].SetSelectEvent(self.SelectGlobalTicket)
        self.elements['scrollbars']['admin'].SetScrollEvent(self.__OnScrollGlobalTickets)
        self.elements['scrollbars']['permisions'].SetScrollEvent(self.__OnScrollPermisions)
        for i in xrange(7):
            self.elements['permision_%d' % i].Open(self.GetChild('permisions_bg'), 0, 10+20*i)
            self.elements['permision_%d' % i].SetWindowHorizontalAlignCenter()
        self.elements['buttons']['add_member'].SetEvent(ui.__mem_func__(self.__OnClickAddMember))
        self.elements['add_member'].Open(self.board, 0, -20)
        self.elements['add_member'].SetWindowHorizontalAlignCenter()
        self.elements['add_member'].SetWindowVerticalAlignCenter()
        self.elements['add_member'].Hide()
        self.elements['buttons']['search'].SetEvent(ui.__mem_func__(self.SelectFoundTicket))
        self.elements['comboboxes']['sort'].SelectItem(0)
        
    def Close(self):
        constInfo.Tickets['QCMD'] = 'CLOSE#'
        event.QuestButtonClick(constInfo.Tickets['QID'])
        self.Hide()
        
    def Destroy(self):
        self.Close()
        self.ClearDictionary()
        
    def OnPressEscapeKey(self):
        self.Close()
        return True
        
    def RequestOpen(self):
        constInfo.Tickets['QCMD'] = 'OPEN#'
        event.QuestButtonClick(constInfo.Tickets['QID'])
        
    def Open(self, mode):
        if not self.isLoaded:
            self.__LoadWindow()
            
        if mode == 1:
            self.SetGmMode()
        else:
            self.SetPlayerMode()
            
        self.RefreshMode()        
        self.SelectPage('MY_TICKETS')
        self.SetCenterPosition()
        self.SetTop()
        self.Show()
        
    def SetPlayerMode(self):
        self.isGM = False
        self.GetChild('btnpage2').Hide()
        
    def SetGmMode(self):
        self.isGM = True
        self.GetChild('btnpage2').Show()
        
    def RefreshMode(self):
        if self.IsMember():
            self.SetGmMode()
        else:
            self.SetPlayerMode()
        self.elements['unique_ticket'].SetGMLevel(self.isGM)
        
    def IsMember(self):
        for i in xrange(len(constInfo.Tickets['PERMISIONS'])):
            if constInfo.Tickets['PERMISIONS'][i][0] == player.GetName():
                return True
        return False
        
    def SelectPage(self, pageName):
        for i in xrange(2):
            self.GetChild('btnpage%d' % (i+1)).SetUp()
        for i in xrange(3):
            self.GetChild('window%d' % (i+1)).Hide()
        self.curPage = pageName
        self.elements['add_member'].Hide()
        self.isSorting = False
        idx = 1
        if pageName == 'MY_TICKETS':
            self.myticketsPos = 0
            self.SelectSort(0)
            self.elements['comboboxes']['sort'].SelectItem(self.lastSort)
            self.RefreshMyTickets()
            self.ResetTicket()
            self.elements['comboboxes']['categories'].Hide()
            idx = 1
        elif pageName == 'ADMINISTRATOR':
            self.globalticketsPos = 0
            self.elements['editlines']['search'].SetText('')
            self.elements['comboboxes']['categories'].Show()
            self.elements['comboboxes']['categories'].Enable()
            self.SelectCategory(self.curCategory)
            self.RefreshPermisions()
            idx = 2
        elif pageName == 'SHOW_TICKET':
            idx = 3
            self.GetChild('window%d' % idx).Show()
            return
        self.GetChild('btnpage%d' % idx).Down()
        self.GetChild('window%d' % idx).Show()
        
    def RefreshPage(self):
        self.RefreshMode()
        if self.curPage == 'SHOW_TICKET':
            self.RefreshSelectedTicket()
            return
        if self.curPage == 'ADMINISTRATOR' and not self.isGM:
            self.SelectPage('MY_TICKETS')
            return
        self.SelectPage(self.curPage)
        
    def RefreshMyTickets(self):
        self.elements['listboxes']['my_tickets'].RemoveAllItems()
        for i in xrange(min(len(constInfo.Tickets['MY_TICKETS']), 11)):
            if self.isSorting:
                curTicket = self.sortedList[self.myticketsPos + i]
            else:
                curTicket = constInfo.Tickets['MY_TICKETS'][self.myticketsPos + i]
            self.elements['listboxes']['my_tickets'].AppendItem(TicketInfo(curTicket[0], curTicket[1], curTicket[2], curTicket[3], curTicket[5]))
            
        if len(constInfo.Tickets['MY_TICKETS']) <= 11:
            self.elements['scrollbars']['my_tickets'].Hide()
        else:
            self.elements['scrollbars']['my_tickets'].SetMiddleBarSize(float(11)/float(len(constInfo.Tickets['MY_TICKETS'])))
            self.elements['scrollbars']['my_tickets'].Show()
            
    def __OnScrollMyTickets(self):
        myticketslen = len(constInfo.Tickets['MY_TICKETS'])
        self.myticketsPos = int(self.elements['scrollbars']['my_tickets'].GetPos() * (myticketslen-11))
        self.RefreshMyTickets()
    
    def __OnScrollGlobalTickets(self):
        globalticketslen = len(constInfo.Tickets['GLOBAL_TICKETS'])
        self.globalticketsPos = int(self.elements['scrollbars']['admin'].GetPos() * (globalticketslen-10))
        self.RefreshGlobalTickets()
        
    def __OnScrollPermisions(self):
        permisionslen = len(constInfo.Tickets['PERMISIONS'])
        self.permisionsPos = int(self.elements['scrollbars']['permisions'].GetPos() * (permisionslen-7))
        self.RefreshPermisions()
        
    def RefreshGlobalTickets(self):
        self.elements['listboxes']['admin'].RemoveAllItems()
        if self.isSorting:
            TicketList = self.sortedList
        else:
            TicketList = self.GetCategoryTickets(self.curCategory)
        for i in xrange(min(len(TicketList), 10)):
            curTicket = TicketList[self.globalticketsPos + i]
            self.elements['listboxes']['admin'].AppendItem(TicketInfo(curTicket[0], curTicket[1], curTicket[2], curTicket[3], curTicket[5]))
            
        if len(TicketList) <= 10:
            self.elements['scrollbars']['admin'].Hide()
        else:
            self.elements['scrollbars']['admin'].SetMiddleBarSize(float(10)/float(len(TicketList)))
            self.elements['scrollbars']['admin'].Show()
            
    def RefreshPermisions(self):
        for i in xrange(7):
            self.elements['permision_%d' % i].Hide()
            
        try:
            for i in xrange(min(len(constInfo.Tickets['PERMISIONS']), 7)):
                curPermision = constInfo.Tickets['PERMISIONS'][i + self.permisionsPos]
                self.elements['permision_%d' % i].SetPermisionName(curPermision[0])
                if curPermision[1] == 0:
                    self.elements['permision_%d' % i].AnswerCheckBox.SetCheck(FALSE)
                else:
                    self.elements['permision_%d' % i].AnswerCheckBox.SetCheck(TRUE)
                if curPermision[2] == 0:
                    self.elements['permision_%d' % i].DeleteCheckBox.SetCheck(FALSE)
                else:
                    self.elements['permision_%d' % i].DeleteCheckBox.SetCheck(TRUE)
                if curPermision[3] == 0:
                    self.elements['permision_%d' % i].AddCheckBox.SetCheck(FALSE)
                else:
                    self.elements['permision_%d' % i].AddCheckBox.SetCheck(TRUE)
                if i + self.permisionsPos == 0:
                    self.elements['permision_%d' % i].AddCheckBox.Disable()
                    self.elements['permision_%d' % i].DeleteCheckBox.Disable()
                    self.elements['permision_%d' % i].AnswerCheckBox.Disable()
                self.elements['permision_%d' % i].Show()
        except:
            self.elements['scrollbars']['permisions'].Hide()
            return
            
        if len(constInfo.Tickets['PERMISIONS']) <= 7:
            self.elements['scrollbars']['permisions'].Hide()
        else:
            self.elements['scrollbars']['permisions'].SetMiddleBarSize(float(7)/float(len(constInfo.Tickets['PERMISIONS'])))
            self.elements['scrollbars']['permisions'].Show()
        
    def SelectMyTicket(self, idx):
        if self.isSorting:
            curTicket = self.sortedList[self.elements['listboxes']['my_tickets'].GetItemIndex(idx) + self.myticketsPos]
        else:
            curTicket = constInfo.Tickets['MY_TICKETS'][self.elements['listboxes']['my_tickets'].GetItemIndex(idx) + self.myticketsPos]
        self.tmpID = curTicket[0]
        self.elements['unique_ticket'].SetContent(curTicket[0], curTicket[1], curTicket[2], curTicket[3], curTicket[4], curTicket[5], curTicket[6], curTicket[7])
        self.elements['comboboxes']['categories'].Show()
        self.elements['comboboxes']['categories'].Disable()
        self.elements['comboboxes']['categories'].SetCurrentItem(CATEGORIES[curTicket[8]])
        self.SelectPage('SHOW_TICKET')
        
    def SelectGlobalTicket(self, idx):
        if self.isSorting:
            curTicket = self.sortedList[self.elements['listboxes']['admin'].GetItemIndex(idx) + self.globalticketsPos]
        else:
            curTicket = self.GetCategoryTickets(self.curCategory)[self.elements['listboxes']['admin'].GetItemIndex(idx) + self.globalticketsPos]
        self.tmpID = curTicket[0]
        self.elements['unique_ticket'].SetContent(curTicket[0], curTicket[1], curTicket[2], curTicket[3], curTicket[4], curTicket[5], curTicket[6], curTicket[7])
        self.SelectPage('SHOW_TICKET')
        
    def SelectFoundTicket(self, tid = ''):
        id = self.elements['editlines']['search'].GetText() if tid == '' else tid
        selected = -1
        for i in xrange(len(constInfo.Tickets['GLOBAL_TICKETS'])):
            if constInfo.Tickets['GLOBAL_TICKETS'][i][0] == id:
                selected = i
                break
        if selected == -1:
            chat.AppendChat(chat.CHAT_TYPE_INFO, 'No se ha encontrado ningun ticket con la id %s.' % id)
            return
        curTicket = constInfo.Tickets['GLOBAL_TICKETS'][selected]
        self.tmpID = curTicket[0]
        self.elements['unique_ticket'].SetContent(curTicket[0], curTicket[1], curTicket[2], curTicket[3], curTicket[4], curTicket[5], curTicket[6], curTicket[7])
        self.elements['comboboxes']['categories'].Show()
        self.elements['comboboxes']['categories'].Disable()
        self.elements['comboboxes']['categories'].SetCurrentItem(CATEGORIES[curTicket[8]])
        self.SelectPage('SHOW_TICKET')
        
    def RefreshSelectedTicket(self):
        self.SelectFoundTicket(self.tmpID)
        
    def SelectPriority(self, priority):
        self.elements['comboboxes']['create_ticket'].SetCurrentItem(self.priority_levels[priority])
        self.priority = priority
        
    def SelectCategorySend(self, category):
        self.elements['comboboxes']['create_ticket_category'].SetCurrentItem(CATEGORIES[category+1])
        self.category = category+1
        
    def SendTicket(self):
        self.elements['editlines']['create_ticket']['title'].KillFocus()
        self.elements['editlines']['create_ticket']['msg'].KillFocus()
        
        title = self.elements['editlines']['create_ticket']['title'].GetText()
        msg = self.elements['editlines']['create_ticket']['msg'].GetText()
        priority = self.priority+1
        if msg == '' or title == '':
            chat.AppendChat(chat.CHAT_TYPE_INFO, '<Tikets> Debes rellenar los campos correctamente.')
            return

        constInfo.Tickets['QCMD'] = 'CREATE_TICKET#%s#%s#%d#%d' % (title.replace(' ', '[_]'), msg.replace(' ', '[_]'), priority, self.category)
        event.QuestButtonClick(constInfo.Tickets['QID'])
        
    def ResetTicket(self):
        self.elements['editlines']['create_ticket']['title'].SetText('')
        self.elements['editlines']['create_ticket']['msg'].SetText('')
        self.SelectPriority(0)
        self.elements['comboboxes']['create_ticket'].SelectItem(0)
        
    def __OnClickAddMember(self):
        self.elements['add_member'].ResetValues()
        self.elements['add_member'].Show()
    
    def SelectSort(self, idx):
        self.elements['comboboxes']['sort'].SetCurrentItem(self.sort[idx])
        self.isSorting = True
        self.lastSort = idx
        self.sortedList = []
        if self.curPage == 'MY_TICKETS':
            self.sortedList += constInfo.Tickets['MY_TICKETS']
            self.sortedList.sort(key=lambda x: x[(3,5,2)[idx]], reverse=(True,False,True)[idx])
            self.RefreshMyTickets()
        elif self.curPage == 'ADMINISTRATOR':
            self.sortedList += self.GetCategoryTickets(self.curCategory)
            self.sortedList.sort(key=lambda x: x[(3,5,2)[idx]], reverse=(True,False,True)[idx])
            self.RefreshGlobalTickets()
            
    def SelectCategory(self, cat_id):
        self.elements['comboboxes']['categories'].SetCurrentItem(CATEGORIES[cat_id])
        self.curCategory = cat_id
        self.SelectSort(self.lastSort)
        self.elements['comboboxes']['sort'].SelectItem(self.lastSort)
        
    def GetCategoryTickets(self, category):
        if category == 0:
            return constInfo.Tickets['GLOBAL_TICKETS']
        else:
            retList = []
            for i in xrange(len(constInfo.Tickets['GLOBAL_TICKETS'])):
                if constInfo.Tickets['GLOBAL_TICKETS'][i][8] == category:
                    retList.append(constInfo.Tickets['GLOBAL_TICKETS'][i])
            return retList
        
class TicketInfo(ui.ListBoxEx.Item):

    def __init__(self, tID, tTitle, priority, tDate, state):
        ui.ListBoxEx.Item.__init__(self)
        self.canLoad = 0
        if len(tTitle) > 40:
            self.textLine = self.__CreateTextLine(tID, tTitle[:37]+"...", priority, tDate, state)
        else:
            self.textLine = self.__CreateTextLine(tID, tTitle[:40], priority, tDate, state)

    def __del__(self):
        ui.ListBoxEx.Item.__del__(self)

    def SetSize(self, width, height):
        ui.ListBoxEx.Item.SetSize(self, 375+55, height)

    def __CreateTextLine(self, tID, tTitle, priority, tDate, state):        
        textID = ui.TextLine()
        textID.SetParent(self)
        textID.SetPosition(-186, 0)
        textID.SetWindowVerticalAlignCenter()
        textID.SetVerticalAlignCenter()
        textID.SetWindowHorizontalAlignCenter()
        textID.SetHorizontalAlignCenter()
        textID.SetText(tID)
        textID.Show()
        
        textTitle = ui.TextLine()
        textTitle.SetParent(self)
        textTitle.SetPosition(-63, 0)
        textTitle.SetWindowVerticalAlignCenter()
        textTitle.SetVerticalAlignCenter()
        textTitle.SetWindowHorizontalAlignCenter()
        textTitle.SetHorizontalAlignCenter()
        textTitle.SetText(tTitle)
        textTitle.Show()
        
        textPriority = ui.TextLine()
        textPriority.SetParent(self)
        textPriority.SetPosition(54, 0)
        textPriority.SetWindowVerticalAlignCenter()
        textPriority.SetVerticalAlignCenter()
        textPriority.SetWindowHorizontalAlignCenter()
        textPriority.SetHorizontalAlignCenter()
        textPriority.SetText((localeInfo.TICKET_PRIORITY_LOW,localeInfo.TICKET_PRIORITY_MEDIUM,localeInfo.TICKET_PRIORITY_HIGH)[priority-1])
        textPriority.SetPackedFontColor((grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0),0xffD87A00,0xffD80000)[priority-1])
        textPriority.Show()
        
        textDate = ui.TextLine()
        textDate.SetParent(self)
        textDate.SetPosition(124, 0)
        textDate.SetWindowVerticalAlignCenter()
        textDate.SetVerticalAlignCenter()
        textDate.SetWindowHorizontalAlignCenter()
        textDate.SetHorizontalAlignCenter()
        textDate.SetText(tDate)
        textDate.Show()
        
        imgState = ui.Button()
        imgState.SetParent(self)
        imgState.SetPosition(189, 0)
        imgState.SetWindowHorizontalAlignCenter()
        imgState.SetWindowVerticalAlignCenter()
        imgState.SetUpVisual("locale/en/ui/tickets/%s.tga" % ('onhold','closed','solved')[state-1])
        imgState.SetOverVisual("locale/en/ui/tickets/%s.tga" % ('onhold','closed','solved')[state-1])
        imgState.SetDownVisual("locale/en/ui/tickets/%s.tga" % ('onhold','closed','solved')[state-1])
        imgState.SetToolTipText((localeInfo.TICKET_STATE_ON_HOLD,localeInfo.TICKET_STATE_CLOSED,localeInfo.TICKET_STATE_SOLVED)[state-1])
        imgState.Show()
        
        return textID, textTitle, textPriority, textDate, imgState
        
class PermisionManage(ui.ScriptWindow):

    class CheckBox(ui.ImageBox):
        class MouseReflector(ui.Window):
            def __init__(self, parent):
                ui.Window.__init__(self)
                self.SetParent(parent)
                self.AddFlag("not_pick")
                self.width = self.height = 0
                self.isDown = False

            def Down(self):
                self.isDown = True

            def Up(self):
                self.isDown = False

            def OnRender(self):
                import grp
                if self.isDown:
                    grp.SetColor(ui.WHITE_COLOR)
                else:
                    grp.SetColor(ui.HALF_WHITE_COLOR)

                x, y = self.GetGlobalPosition()
                grp.RenderBar(x+2, y+2, self.GetWidth()-4, self.GetHeight()-4)
                
        def __init__(self, parent, x, y, event, filename = "d:/ymir work/ui/public/Parameter_Slot_01.sub"):
            ui.ImageBox.__init__(self)
            self.SetParent(parent)
            self.SetPosition(x, y)
            self.LoadImage(filename)

            self.mouseReflector = self.MouseReflector(self)
            self.mouseReflector.SetSize(self.GetWidth(), self.GetHeight())

            image = ui.MakeImageBox(self, "d:/ymir work/ui/public/check_image.sub", 0, 0)
            image.AddFlag("not_pick")
            image.SetWindowHorizontalAlignCenter()
            image.SetWindowVerticalAlignCenter()
            image.Hide()
            self.Enable = True
            self.Checked = False
            self.image = image
            self.event = event
            self.Show()

            self.mouseReflector.UpdateRect()

        def __del__(self):
            ui.ImageBox.__del__(self)

        def SetCheck(self, flag):
            if flag:
                self.image.Show()
                self.Checked = True
            else:
                self.image.Hide()
                self.Checked = False
            
        def GetCheck(self):
            return self.Checked

        def Disable(self):
            self.Enable = False

        def OnMouseOverIn(self):
            if not self.Enable:
                return
            self.mouseReflector.Show()

        def OnMouseOverOut(self):
            if not self.Enable:
                return
            self.mouseReflector.Hide()

        def OnMouseLeftButtonDown(self):
            if not self.Enable:
                return
            self.mouseReflector.Down()

        def OnMouseLeftButtonUp(self):
            if not self.Enable:
                return
            self.mouseReflector.Up()
            self.event()
            
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/Ticket_Permision.py")
        except:
            import exception
            exception.Abort("Ticket_Permision.LoadWindow.LoadObject")
            
        self.memberName = self.GetChild('name_value')
        self.deleteMember = self.GetChild('delete_member')
        
        event = lambda argSelf=proxy(self), argIndex=0, argPermision='answer': apply(argSelf.OnCheckPriority, (argIndex, argPermision))
        self.AnswerCheckBox = self.CheckBox(self, 140, 0, event)
        
        event = lambda argSelf=proxy(self), argIndex=1, argPermision='delete': apply(argSelf.OnCheckPriority, (argIndex, argPermision))
        self.DeleteCheckBox = self.CheckBox(self, 140+75, 0, event)
        
        event = lambda argSelf=proxy(self), argIndex=2, argPermision='add': apply(argSelf.OnCheckPriority, (argIndex, argPermision))
        self.AddCheckBox = self.CheckBox(self, 140+75*2, 0, event)
        
        self.deleteMember.SetEvent(ui.__mem_func__(self.DeleteMember))
    
    def __del__(self):
        ui.ScriptWindow.__del__(self)
        
    def OnCheckPriority(self, argIndex, argPermision):
        permision = 0
        if argPermision == 'answer':
            if self.AnswerCheckBox.GetCheck():
                self.AnswerCheckBox.SetCheck(False)
            else:
                self.AnswerCheckBox.SetCheck(True)
                permision = 1
        elif argPermision == 'delete':
            if self.DeleteCheckBox.GetCheck():
                self.DeleteCheckBox.SetCheck(False)
            else:
                self.DeleteCheckBox.SetCheck(True)
                permision = 1
        elif argPermision == 'add':
            if self.AddCheckBox.GetCheck():
                self.AddCheckBox.SetCheck(False)
            else:
                self.AddCheckBox.SetCheck(True)
                permision = 1
                
        constInfo.Tickets['QCMD'] = 'UPDATE_PERMISIONS#%s#%s#%d' % (self.memberName.GetText(), argPermision, permision)
        event.QuestButtonClick(constInfo.Tickets['QID'])
        
    def SetPermisionName(self, name):
        self.memberName.SetText(name)
        
    def DeleteMember(self):
        name = self.memberName.GetText()
        if name == '':
            return
        constInfo.Tickets['QCMD'] = 'DELETE_MEMBER#%s' % name
        event.QuestButtonClick(constInfo.Tickets['QID'])
        
    def Open(self, parent, x, y):
        self.SetParent(parent)
        self.SetPosition(x, y)
        self.Show()

class TicketManage(ui.ScriptWindow):

    class TicketManageTop(ui.ScriptWindow):
        def __init__(self):
            ui.ScriptWindow.__init__(self)
            
            try:
                pyScrLoader = ui.PythonScriptLoader()
                pyScrLoader.LoadScriptFile(self, "UIScript/Ticket_Manage_Top.py")
            except:
                import exception
                exception.Abort("Ticket_Manage_Top.LoadWindow.LoadObject")
                
            self.title = self.GetChild('title_value')
            self.stateBoard = self.GetChild('state_bar')
            self.state = self.GetChild('state_value')
            self.id = self.GetChild('ticket_id_value')
            self.priority = self.GetChild('ticket_priority_value')
            self.date = self.GetChild('ticket_date_value')
            self.answers = self.GetChild('ticket_answers_value')
            
            self.stateColours = [grp.GenerateColor(1.0, 0.6, 0.1, 0.4),grp.GenerateColor(1.0, 0.0, 0.0, 0.4),grp.GenerateColor(0.0, 1.0, 0.0, 0.4)]
        
        def __del__(self):
            ui.ScriptWindow.__del__(self)
            
        def SetContent(self, id, title, date, priority):
            self.id.SetText(id)
            self.title.SetText(title)
            self.priority.SetText((localeInfo.TICKET_PRIORITY_LOW,localeInfo.TICKET_PRIORITY_MEDIUM,localeInfo.TICKET_PRIORITY_HIGH)[priority-1])
            self.date.SetText(date)
            try:
                self.answers.SetText('%d' % len(constInfo.Tickets['ANSWERS'][id]))
            except:
                self.answers.SetText('0')
            
        def SetState(self, state):
            self.stateBoard.SetColor(self.stateColours[state-1])
            self.state.SetText((localeInfo.TICKET_STATE_ON_HOLD,localeInfo.TICKET_STATE_CLOSED,localeInfo.TICKET_STATE_SOLVED)[state-1])
            
    class TicketManageMiddle(ui.ScriptWindow):
        class Answer(ui.ScriptWindow):
            def __init__(self):
                ui.ScriptWindow.__init__(self)
                
                try:
                    pyScrLoader = ui.PythonScriptLoader()
                    pyScrLoader.LoadScriptFile(self, "UIScript/Ticket_Manage_Answer.py")
                except:
                    import exception
                    exception.Abort("Ticket_Manage_Answer.LoadWindow.LoadObject")
                    
                self.msg = self.GetChild('msg')
                self.msg.CanEdit(False)
            
            def __del__(self):
                ui.ScriptWindow.__del__(self)
                
            def SetContent(self, creator, date, hour, msg):
                self.GetChild('creator_date_value').SetText('por %s en %s %s' % (creator, date, hour))
                self.GetChild('h_line').SetSize(self.GetChild('creator_date_value').GetTextSize()[0]+12, 0)
                self.msg.SetText(msg)
        
        def __init__(self):
            ui.ScriptWindow.__init__(self)
            self.id = ''
            self.ticketsPos = 0
            
            try:
                pyScrLoader = ui.PythonScriptLoader()
                pyScrLoader.LoadScriptFile(self, "UIScript/Ticket_Manage_Middle.py")
            except:
                import exception
                exception.Abort("Ticket_Manage_Middle.LoadWindow.LoadObject")
        
            self.msg = self.GetChild('msg')
            self.msg.CanEdit(False)
            self.scrollbar = self.GetChild('scrollbar')
            self.scrollbar.SetScrollEvent(self.__OnScroll)
            self.answers = {'0' : self.Answer(), '1' : self.Answer()}
            
            for i in xrange(2):
                self.answers['%d' % i].SetParent(self)
                self.answers['%d' % i].SetPosition(-5, 80+78*i)
                self.answers['%d' % i].SetWindowHorizontalAlignCenter()
                self.answers['%d' % i].Hide()

        def __del__(self):
            ui.ScriptWindow.__del__(self)
            
        def SetContent(self, id, creator, date, hour, msg):
            self.id = id
            self.GetChild('creator_date_value').SetText('por %s en %s %s' % (creator, date, hour))
            self.GetChild('h_line').SetSize(self.GetChild('creator_date_value').GetTextSize()[0]+12, 0)
            self.msg.SetText(msg)
            self.ticketsPos = 0
            self.RefreshAnswers()
        
        def RefreshAnswers(self):
            for i in xrange(2):
                self.answers['%d' % i].Hide()

            try:
                for i in xrange(2):
                    curTicket = constInfo.Tickets['ANSWERS'][self.id][self.ticketsPos + i]
                    self.answers['%d' % i].SetContent(curTicket[0], curTicket[1], curTicket[2], curTicket[3])
                    self.answers['%d' % i].Show()
            except:
                self.scrollbar.Hide()
                return
                
            if len(constInfo.Tickets['ANSWERS'][self.id]) <= 2:
                self.scrollbar.Hide()
            else:
                self.scrollbar.SetMiddleBarSize(float(2)/float(len(constInfo.Tickets['ANSWERS'][self.id])))
                self.scrollbar.Show()
                
        def __OnScroll(self):
            ticketslen = len(constInfo.Tickets['ANSWERS'][self.id])
            self.ticketsPos = int(self.scrollbar.GetPos() * (ticketslen-2))
            self.RefreshAnswers()
            
    class TicketManageBottom(ui.ScriptWindow):
        def __init__(self):
            ui.ScriptWindow.__init__(self)
            self.id = ''
            self.state = 1
            
            try:
                pyScrLoader = ui.PythonScriptLoader()
                pyScrLoader.LoadScriptFile(self, "UIScript/Ticket_Manage_Bottom.py")
            except:
                import exception
                exception.Abort("Ticket_Manage_Bottom.LoadWindow.LoadObject")
                
            self.msg = self.GetChild('msg')
            self.send = self.GetChild('send_button')
                
            for i in xrange(3):
                self.GetChild('state_%d_button' % (i+1)).SetEvent(ui.__mem_func__(self.SetState), (i+1))
                self.GetChild('state_%d_button' % (i+1)).Hide()
                
            self.send.SetEvent(ui.__mem_func__(self.SendTicket))

        def __del__(self):
            ui.ScriptWindow.__del__(self)
            
        def SetContent(self, id, state):
            self.id = id
            self.SetState(state)
            
        def SetState(self, state):
            for i in xrange(3):
                self.GetChild('state_%d_button' % (i+1)).SetUp()
            self.state = state
            self.GetChild('state_%d_button' % state).Down()
            
        def SetGMAction(self, level):
            if level:
                for i in xrange(3):
                    self.GetChild('state_%d_button' % (i+1)).Show()
            else:
                for i in xrange(3):
                    self.GetChild('state_%d_button' % (i+1)).Hide()
                
        def SendTicket(self):
            constInfo.Tickets['QCMD'] = 'ANSWER_TICKET#%s#%d#%s' % (self.id, self.state, self.msg.GetText().replace(' ', '[_]'))
            event.QuestButtonClick(constInfo.Tickets['QID'])
            self.msg.SetText('')
            
        def SetCloseEvent(self, func):
            self.msg.SetEscapeEvent(func)
    
    def __init__(self):
        ui.Window.__init__(self)
        
        self.Top = self.TicketManageTop()
        self.Top.SetParent(self)
        self.Top.SetPosition(0, 0)
        self.Top.Show()
        
        self.Middle = self.TicketManageMiddle()
        self.Middle.SetParent(self)
        self.Middle.SetPosition(0, self.Top.GetHeight()+1)
        self.Middle.Show()
        
        self.Bottom = self.TicketManageBottom()
        self.Bottom.SetParent(self)
        self.Bottom.SetPosition(0, self.Top.GetHeight()+1+self.Middle.GetHeight()+1)
        self.Bottom.Show()
        
        self.SetSize(self.Top.GetWidth(), self.Top.GetHeight()+self.Middle.GetHeight()+self.Bottom.GetHeight()+2)
        
    def __del__(self):
        ui.Window.__del__(self)
        
    def SetCloseEvent(self, func):
        self.Bottom.SetCloseEvent(func)
        
    def SetContent(self, id, title, priority, date, hour, state, creator, msg):
        self.Top.SetContent(id, title, date, priority)
        self.Top.SetState(state)
        self.Middle.SetContent(id, creator, date, hour, msg)
        self.Bottom.SetContent(id, state)
        
    def SetGMLevel(self, level):
        self.Bottom.SetGMAction(level)
        
    def Open(self, parent, x, y):
        self.SetParent(parent)
        self.SetPosition(x, y)
        self.SetTop()
        self.Show()
        
class AddMember(ui.Window):

    def __init__(self):
        ui.Window.__init__(self)
        
        self.board = ui.BoardWithTitleBar()
        self.board.SetParent(self)
        self.board.SetSize(185, 100)
        self.board.SetPosition(0, 0)
        self.board.SetTitleName(localeInfo.TICKET_ADD_TITLE)
        self.board.SetCloseEvent(ui.__mem_func__(self.Hide))
        self.board.Show()
        
        self.nameSlot = ui.MakeSlotBar(self.board, 23, 40, 100, 18)
        self.nameSlot.SetWindowHorizontalAlignCenter()
        
        self.nameText = ui.TextLine()
        self.nameText.SetParent(self.nameSlot)
        self.nameText.SetPosition(105, 3)
        self.nameText.SetWindowHorizontalAlignRight()
        self.nameText.SetHorizontalAlignRight()
        self.nameText.SetText(localeInfo.TICKET_ADD_NAME)
        self.nameText.Show()
        
        self.nameValue = ui.EditLine()
        self.nameValue.SetParent(self.nameSlot)
        self.nameValue.SetSize(100, 18)
        self.nameValue.SetPosition(3, 3)
        self.nameValue.SetMax(16)
        self.nameValue.SetEscapeEvent(ui.__mem_func__(self.Hide))
        self.nameValue.Show()
        
        self.acceptButton = ui.MakeButton(self.board, 0, self.board.GetHeight() - 32, '', 'd:/ymir work/ui/public/', 'large_button_01.sub', 'large_button_02.sub', 'large_button_03.sub')
        self.acceptButton.SetWindowHorizontalAlignCenter()
        self.acceptButton.SetText(localeInfo.TICKET_ADD_ACCEPT)
        self.acceptButton.SetEvent(ui.__mem_func__(self.SendMember))
        
        self.SetSize(185, 100)
        
    def __del__(self):
        ui.Window.__del__(self)
        
    def ResetValues(self):
        self.nameValue.SetText('')
        self.nameValue.SetFocus()
        
    def SendMember(self):
        self.nameValue.KillFocus()
        name = self.nameValue.GetText()
        if name == '':
            return
        constInfo.Tickets['QCMD'] = 'ADD_MEMBER#%s' % name
        event.QuestButtonClick(constInfo.Tickets['QID'])
        self.Hide()
        
    def Open(self, parent, x, y):
        self.SetParent(parent)
        self.SetPosition(x, y)
        self.Show()
    
# Shang ~ Ticketsystem