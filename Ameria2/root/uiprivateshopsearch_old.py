# coding: latin_1

import ui, mouseModule, localeInfo, math, uiCommon, constInfo
import app, player, net, chat, skill, nonplayer, item, privateShopSearch, background, grp
from _weakref import proxy

HEIGHT_STEP = 20
IMG_DIR = "d:/ymir work/ui/game/privatesearch/"
class PrivateShopSearchDialog(ui.ScriptWindow):

    class SearchResultItem(ui.ExpandedImageBox):
        def __init__(self, parent, index, size):
            ui.ExpandedImageBox.__init__(self)
            self.parent = proxy(parent)
            self.isLoad = True
            self.isSelected = False
            self.index = index
            self.size = size
            
            self.itemID = 0
            self.item_name = ""
            self.itemVnum = 0
            self.shopVid = 0
            self.shopItemPos = 0
            self.metinSlot = []
            self.attrSlot = []
            self.renderImages = []
            self.renderText = []
            self.effectID = 0
            if app.WJ_CHANGELOOK_SYSTEM:
                self.transmutation = 0
            self.LoadImage(IMG_DIR + "slot_%d_0.tga"%int(self.size))
            self.SAFE_SetStringEvent("MOUSE_OVER_IN",self.__OverInImage)
            self.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.__OverOutImage)
            self.SetEvent(ui.__mem_func__(self.__OnSelect),"mouse_click")
            self.SetPosition(0,7)
            self.renderImages.append(self)
            self.InitItem()

        def __del__(self):
            ui.ExpandedImageBox.__del__(self)

        def Destroy(self):
            self.item_icon =0
            self.item_name_text =0
            self.seller_name =0
            self.item_count =0
            self.money_icon =0
            self.price_text =0
            self.wiki_button =0
            self.shop_button =0
            self.whisper_button =0
            self.parent =0
            self.isLoad = 0
            self.isSelected = 0
            self.index = 0
            self.size = 0
            self.itemID = 0
            self.item_name = ""
            self.itemVnum = 0
            self.shopVid = 0
            self.shopItemPos = 0
            self.metinSlot = []
            self.attrSlot = []
            self.renderImages = []
            self.renderText = []
            self.effectID = 0
            if app.WJ_CHANGELOOK_SYSTEM:
                self.transmutation = 0
                self.transmutation_icon = 0
        def InitItem(self):
            startX = 13

            self.item_icon = ui.ExpandedImageBox()
            self.item_icon.SetParent(self)
            self.item_icon.SetPosition(startX+2, 10)
            self.item_icon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.__OverInItem)
            self.item_icon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.__OverOutItem)
            self.item_icon.SetEvent(ui.__mem_func__(self.__OnSelect),"mouse_click")
            self.item_icon.Show()
            self.renderImages.append(self.item_icon)

            y = (32*self.size)/2

            self.item_name_text = ui.TextLine()
            self.item_name_text.SetParent(self)
            self.item_name_text.SetPosition(130, y)
            self.item_name_text.SetHorizontalAlignCenter()
            self.item_name_text.SetFontName("Tahoma:15")
            self.item_name_text.SetText(item.GetItemName())
            self.item_name_text.Show()
            self.renderText.append(self.item_name_text)


            self.seller_name = ui.TextLine()
            self.seller_name.SetParent(self)
            self.seller_name.SetPosition(130+125, y)
            self.seller_name.SetHorizontalAlignCenter()
            self.seller_name.SetFontName("Tahoma:15")
            self.seller_name.Show()
            self.renderText.append(self.seller_name)

            self.item_count = ui.TextLine()
            self.item_count.SetParent(self)
            self.item_count.SetPosition(210+110, y)
            self.item_count.SetHorizontalAlignCenter()
            self.item_count.SetFontName("Tahoma:15")
            self.item_count.Show()
            self.renderText.append(self.item_count)

            self.money_icon = ui.ExpandedImageBox()
            self.money_icon.SetParent(self)
            self.money_icon.LoadImage(IMG_DIR+"money.tga")
            self.money_icon.SetPosition(330+40, y)
            self.money_icon.Show()
            self.renderImages.append(self.money_icon)

            self.price_text = ui.TextLine()
            self.price_text.SetParent(self)
            self.price_text.SetPosition(360+40, y)
            self.price_text.SetHorizontalAlignLeft()
            #self.price_text.SetFontName("Tahoma:15")
            self.price_text.Show()
            self.renderText.append(self.price_text)

            self.wiki_button = ui.ExpandedImageBox()
            self.wiki_button.SetParent(self)
            self.wiki_button.LoadImage(IMG_DIR+"wiki_0.tga")
            self.wiki_button.SetPosition(470+40, y)
            self.wiki_button.SAFE_SetStringEvent("MOUSE_OVER_IN",self.wikiIn)
            self.wiki_button.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.wikiOut)
            self.wiki_button.SetEvent(ui.__mem_func__(self.wikiClick),"mouse_click")
            self.wiki_button.Show()
            self.renderImages.append(self.wiki_button)

            self.shop_button = ui.ExpandedImageBox()
            self.shop_button.SetParent(self)
            self.shop_button.LoadImage(IMG_DIR+"home_0.tga")
            self.shop_button.SetPosition(485+40, y)
            self.shop_button.SAFE_SetStringEvent("MOUSE_OVER_IN",self.shopIn)
            self.shop_button.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.shopOut)
            self.shop_button.SetEvent(ui.__mem_func__(self.shopClick),"mouse_click")
            self.shop_button.Show()
            self.renderImages.append(self.shop_button)

            self.whisper_button = ui.ExpandedImageBox()
            self.whisper_button.SetParent(self)
            self.whisper_button.LoadImage(IMG_DIR+"whisper_1.tga")
            self.whisper_button.SetPosition(500+40, y)
            self.whisper_button.SAFE_SetStringEvent("MOUSE_OVER_IN",self.whisperIn)
            self.whisper_button.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.whisperOut)
            self.whisper_button.SetEvent(ui.__mem_func__(self.whisperClick),"mouse_click")
            self.whisper_button.Show()
            self.renderImages.append(self.whisper_button)

            self.Show()

        def wikiIn(self):
            self.wiki_button.LoadImage(IMG_DIR+"wiki_1.tga")
        def wikiOut(self):
            self.wiki_button.LoadImage(IMG_DIR+"wiki_0.tga")
        def wikiClick(self):
            pass

        def shopIn(self):
            self.shop_button.LoadImage(IMG_DIR+"home_1.tga")
        def shopOut(self):
            self.shop_button.LoadImage(IMG_DIR+"home_0.tga")
        def shopClick(self):
            net.SendOpenWithVID(self.shopVid)

        def whisperOut(self):
            self.whisper_button.LoadImage(IMG_DIR+ "whisper_1.tga")
        def whisperIn(self):
            self.whisper_button.LoadImage(IMG_DIR+ "whisper_2.tga")
        def whisperClick(self,arg):
            interface = constInfo.GetInterfaceInstance()
            if not interface:
                return
            interface.OpenWhisperDialog(self.seller_name.GetText())

        def __OverOutImage(self):
            self.LoadImage(IMG_DIR + "slot_%d_0.tga"%self.size)
            self.OnRender()

        def __OverInImage(self):
            self.LoadImage(IMG_DIR + "slot_%d_1.tga"%self.size)
            self.OnRender()

        def __OverOutItem(self):
            interface = constInfo.GetInterfaceInstance()
            if interface:
                if None != interface.tooltipItem:
                    interface.tooltipItem.HideToolTip()

        def __OverInItem(self):
            interface = constInfo.GetInterfaceInstance()
            if not interface:
                return
            if None == interface.tooltipItem:
                return
            interface.tooltipItem.ClearToolTip()
            interface.tooltipItem.isOfflineShopItem = True
            if app.WJ_CHANGELOOK_SYSTEM:
                interface.tooltipItem.AddItemData(self.itemVnum, self.metinSlot, self.attrSlot, 0, 0, 0,player.INVENTORY, -1,-1, self.transmutation)
            else:
                interface.tooltipItem.AddItemData(self.itemVnum, self.metinSlot, self.attrSlot)
            interface.tooltipItem.AppendPrice(long(self.price_text.GetText().replace(".","")))
            interface.tooltipItem.ShowToolTip()

        def SetItemName(self, name):
            self.item_name=name
            self.item_name_text.SetText(name)

        def SetCount(self, item_count):
            self.item_count.SetText(str(item_count))

        def SetPrice(self, price):
            self.price_text.SetText(constInfo.NumberToString(long(price)))

        def SetSeller(self, seller_name):
            self.seller_name.SetText(seller_name)
        
        def SetMetinSlot(self, metinSlot):
            self.metinSlot = metinSlot

        def SetAttrSlot(self, attrSLot):
            self.attrSlot = attrSLot
            
        def SetShopVid(self, vid):
            self.shopVid = vid

        def GetShopVid(self):
            return self.shopVid

        def SetItemVnum(self, vnum):
            self.itemVnum = vnum
            if vnum == 50300:
                self.item_icon.LoadImageInstance(skill.GetIconInstanceNew(self.metinSlot[0],0))
            else:
                self.item_icon.LoadImage(item.GetIconImageFileName())

        def GetShopItemPos(self):
            return self.shopItemPos

        def SetShopItemPos(self, itemPos):
            self.shopItemPos = itemPos

        if app.WJ_CHANGELOOK_SYSTEM:
            def SetTransmutation(self, transmutation):
                self.transmutation = transmutation
                if self.transmutation > 0:
                    self.transmutation_icon = ui.ExpandedImageBox()
                    self.transmutation_icon.SetParent(self)
                    self.transmutation_icon.SetPosition(13+4, 10)
                    self.transmutation_icon.LoadImage("d:/ymir work/ui/transmutation.tga")
                    self.transmutation_icon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.__OverInItem)
                    self.transmutation_icon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.__OverOutItem)
                    self.transmutation_icon.SetEvent(ui.__mem_func__(self.__OnSelect),"mouse_click")
                    self.transmutation_icon.Show()
                    self.renderImages.append(self.transmutation_icon)

        def __OnSelect(self,arg):
            interface = constInfo.GetInterfaceInstance()
            if not interface:
                return
            if interface.wndPrivateShopSearch==None:
                return
            interface.wndPrivateShopSearch.OnSearchResultItemSelect(self.index,self.item_name,self.item_count.GetText(),self.price_text.GetText(),self.GetShopVid(),self.GetShopItemPos(), self.itemVnum)
        
        def OnRender(self):
            if self.parent:
                (pgx,pgy) = self.parent.GetGlobalPosition()

                for item in self.renderText:
                    (igx,igy) = item.GetGlobalPosition()
                    if igy < pgy:
                        item.Hide()
                    elif igy > (pgy+self.parent.GetHeight()-60):
                        item.Hide()
                    else:
                        item.Show()

                for item in self.renderImages:
                    (igx,igy) = item.GetGlobalPosition()
                    item.SetRenderingRect(0,0,0,0)
                    if igy < pgy:
                        item.SetRenderingRect(0,ui.calculateRect(item.GetHeight()-abs(igy-pgy),item.GetHeight()),0,0)
                    elif igy+item.GetHeight() > (pgy+self.parent.GetHeight()-45):
                        calculate = (pgy+self.parent.GetHeight()-45) - (igy+item.GetHeight())
                        if calculate == 0:
                            return
                        item.SetRenderingRect(0.0,0.0,0.0,ui.calculateRect(item.GetHeight()-abs(calculate),item.GetHeight()))

    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.selectedItemIndex = -1
        self.board = None
        self.itemNameSearch = None
        self.currentItemCat = item.ITEM_TYPE_WEAPON
        self.currentSubItemCat = item.WEAPON_SWORD
        self.currentJob = 0
        self.itemDataList = []
        self.currentPage = 1
        self.pageCount = 1
        self.perPage = 15
        self.itemCount = 0
        self.sexValue = 0
        self.characterValue = 0
        self.bonusValue = 0
        self.alchemyValue = 0
        self.refineValue = 0
        self.minlevelValue = 0
        self.maxlevelValue = 120
        self.minRefineValue=0
        self.maxRefineValue=200
        self.minSkillValue=0
        self.maxSkillValue=40
        self.minAvgValue=0
        self.maxAvgValue=80
        self.minAbsValue=0
        self.maxAbsValue=40
        self.selectedItem = 0
        self.isFirstOpen = False
        self.skillList = []
        
        self.LoadWindow()

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def Destroy(self):
        self.ClearDictionary()

        self.selectedItemIndex = 0
        self.currentItemCat = 0
        self.currentSubItemCat =0
        self.currentJob = 0
        self.itemDataList = 0
        self.currentPage = 0
        self.pageCount = 0
        self.perPage = 0
        self.itemCount = 0
        self.sexValue = 0
        self.characterValue = 0
        self.bonusValue = 0
        self.refineValue = 0
        self.alchemyValue = 0
        self.minlevelValue = 0
        self.maxlevelValue = 0
        self.minRefineValue=0
        self.maxRefineValue=0
        self.minSkillValue=0
        self.maxSkillValue=0
        self.minAvgValue=0
        self.maxAvgValue=0
        self.minAbsValue=0
        self.maxAbsValue=0
        self.selectedItem = 0
        self.skillList = 0
        self.SearchTypeValue = 0
        self.SearchSubTypeValue = 0
        self.nextButton = 0
        self.lastButton = 0
        self.prevButton = 0
        self.firstButton = 0
        self.countDirecton = 0
        self.refineItems = 0
        self.alchemyItems = 0
        self.bonusItems = 0
        self.sexItems = 0
        self.characterItems = 0
        self.countDirectionValue = 0
        self.currentPaginationPage = 0
        self.paginationPageCount = 0
        self.shownPages = 0
        self.isFirstOpen = False

        self.pageButtons = 0

        self.itemNameSearchText = 0
        self.itemNameSearch.Destroy()
        self.itemNameSearch = 0

        self.scroll = 0
        self.ListBox.RemoveAllItems()
        self.ListBox = 0

        self.searchClearBtn = 0
        self.filterButton = 0

        self.searchBuyItem.Destroy()
        self.searchBuyItem = 0

        self.scrollBarListBoxCube=0
        self.listBoxButtons.ClearItem()
        self.listBoxButtons=0

        self.characterChoose.Destroy()
        self.characterChoose = 0
        self.sexChoose.Destroy()
        self.sexChoose = 0
        self.bonusChoose.Destroy()
        self.bonusChoose = 0
        self.alchemyChoose.Destroy()
        self.alchemyChoose = 0

        self.minAvg.Destroy()
        self.minAvg = 0
        self.maxAvg.Destroy()
        self.maxAvg = 0
        self.minSkill.Destroy()
        self.minSkill = 0
        self.maxSkill.Destroy()
        self.maxSkill = 0
        self.minAbs.Destroy()
        self.minAbs = 0
        self.maxAbs.Destroy()
        self.maxAbs = 0
        self.minLevel.Destroy()
        self.minLevel = 0
        self.maxLevel.Destroy()
        self.maxLevel = 0
        self.minRefine.Destroy()
        self.minRefine = 0
        self.maxRefine.Destroy()
        self.maxRefine = 0

        self.board.Destroy()
        self.board = 0

        self.filterTable.Destroy()
        self.filterTable = 0

        


    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/OfflineShop/PrivateShopSearchDialog.py")
        except:
            import exception
            exception.Abort("PrivateShopSearchDialog.LoadDialog.LoadScript")

        try:
            GetObject=self.GetChild
            self.board = GetObject("board")
            self.itemNameSearch = GetObject("ItemNameValue")
            self.itemNameSearchText = GetObject("itemNameSearchText")
            self.itemNameSearchText.SetFontColor(205,205,205)
            self.ListBox = GetObject("ListBox")

            self.pageButtons = []

            self.SearchTypeValue = 0
            self.SearchSubTypeValue = 0

            self.searchClearBtn = GetObject("Clearbutton")
            self.filterButton = GetObject("FilterButton")
            self.filterTable = GetObject("new_table")


            self.minAvg = GetObject("editline2")
            self.maxAvg = GetObject("editline3")

            self.minSkill = GetObject("editline4")
            self.maxSkill = GetObject("editline5")
            
            self.minAbs = GetObject("editline6")
            self.maxAbs = GetObject("editline7")
            
            self.minLevel = GetObject("editline8")
            self.maxLevel = GetObject("editline9")

            self.minRefine = GetObject("editline10")
            self.maxRefine = GetObject("editline11")

            for l in xrange(5):
                self.pageButtons.append(GetObject("page%d_button"%(l+1)))

            self.pageButtons[0].Show()
            self.pageButtons[1].Hide()
            self.pageButtons[2].Hide()
            self.pageButtons[3].Hide()
            self.pageButtons[4].Hide()
            self.pageButtons[0].Down()
            self.pageButtons[0].Disable()

            self.nextButton = GetObject("next_button")
            self.lastButton = GetObject("last_next_button")
            self.prevButton = GetObject("prev_button")
            self.firstButton = GetObject("first_prev_button")
            
            self.countDirecton = GetObject("reverse_count")

            self.nextButton.SetEvent(ui.__mem_func__(self.NextPage))
            self.prevButton.SetEvent(ui.__mem_func__(self.PrevPage))
            self.firstButton.SetEvent(ui.__mem_func__(self.FirstPage))
            self.lastButton.SetEvent(ui.__mem_func__(self.LastPage))
            self.searchClearBtn.SetEvent(ui.__mem_func__(self.ClearText))
            self.board.SetCloseEvent(ui.__mem_func__(self.__OnCloseButtonClick))

        except:
            import exception
            exception.Abort("PrivateShopSearchDialog.LoadDialog.BindObject")

        self.scroll = ui.ScrollBarNew()
        self.scroll.SetParent(self)
        self.scroll.SetPosition(810-25,55)
        self.scroll.SetScrollBarSize(self.GetHeight()-90)
        self.scroll.Show()

        self.ListBox.SetScrollBar(self.scroll)
        #self.ListBox.SetItemStep(20)
        #self.ListBox.SetItemSize(580, 20)

        self.searchBuyItem = uiCommon.QuestionDialog2()

        self.searchClearBtn.Hide()

        self.minAbs.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.minAbs.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMinAbs)
        self.minAbs.SetInfoMessage("Min Abs")

        self.maxAbs.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.maxAbs.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMaxAbs)
        self.maxAbs.SetInfoMessage("Max Abs")

        self.minLevel.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.minLevel.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMinLevel)
        self.minLevel.SetInfoMessage("Min Level")

        self.maxLevel.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.maxLevel.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMaxLevel)
        self.maxLevel.SetInfoMessage("Max Level")

        self.minAvg.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.minAvg.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMinAvg)
        self.minAvg.SetInfoMessage("Min Average")

        self.maxAvg.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.maxAvg.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMaxAvg)
        self.maxAvg.SetInfoMessage("Max Average")

        self.minSkill.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.minSkill.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMinSkill)
        self.minSkill.SetInfoMessage("Min Skill")

        self.maxSkill.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.maxSkill.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMaxSkill)
        self.maxSkill.SetInfoMessage("Max Skill")

        self.minRefine.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.minRefine.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMinRefine)
        self.minRefine.SetInfoMessage("Min Refine")

        self.maxRefine.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.maxRefine.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMaxRefine)
        self.maxRefine.SetInfoMessage("Max Refine")

        self.itemNameSearch.OnPressEscapeKey = ui.__mem_func__(self.Close)
        self.itemNameSearch.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdate)
        self.itemNameSearch.SetReturnEvent(ui.__mem_func__(self.StartSearchbyVnum))
        self.itemNameSearch.SetInfoMessage(localeInfo.PRIVATESHOPSEARCH_ITEMNAME)

        self.countDirecton.SetEvent(ui.__mem_func__(self.__ClickDirection),"mouse_click",1)
        self.countDirectionValue = 0

        self.alchemyItems = {
            0: localeInfo.SEARCH_DEFAULT,
            1: localeInfo.ALCHEMY_CLEAR,
            2: localeInfo.ALCHEMY_FLAWLESS,
            3: localeInfo.ALCHEMY_BRILLIANT,
            4: localeInfo.ALCHEMY_EXCELLENT,
            5: localeInfo.ALCHEMY_MTSY,
        }

        self.bonusItems = {
            0: localeInfo.SEARCH_DEFAULT,
            1: localeInfo.SEARCH_NOBONUS,
            2: localeInfo.SEARCH_BONUS,
        }

        self.sexItems = {
            0: localeInfo.SEARCH_DEFAULT,
            1: localeInfo.CHARACTER_CREATE_MALE,
            2: localeInfo.CHARACTER_CREATE_FEMALE,
        }

        self.characterItems = {
            0: localeInfo.SEARCH_DEFAULT,
            1: localeInfo.CHARACTER_WARRIOR,
            2: localeInfo.CHARACTER_ASSASSIN,
            3: localeInfo.CHARACTER_SHAMAN,
            4: localeInfo.CHARACTER_SURA,

        }

        self.characterChoose = ui.ComboBoxImage(self, IMG_DIR + "class_image2.tga",10,45+(35*8))
        self.characterChoose.SetCurrentItem(self.characterItems.itervalues().next())
        self.characterChoose.SetParent(self.filterTable)
        self.characterChoose.SetWindowName("characterChoose")
        for index, data in self.characterItems.iteritems():
            self.characterChoose.InsertItem(index, data)
        self.characterChoose.SetEvent(lambda subItemNumber, argSelf=proxy(self): argSelf.__ClickCharacterButton(subItemNumber))

        self.sexChoose = ui.ComboBoxImage(self, IMG_DIR + "class_image2.tga",10,45+(35*7))
        self.sexChoose.SetCurrentItem(self.sexItems.itervalues().next())
        self.sexChoose.SetParent(self.filterTable)
        self.sexChoose.SetWindowName("sexChoose")
        for index, data in self.sexItems.iteritems():
            self.sexChoose.InsertItem(index, data)
        self.sexChoose.SetEvent(lambda subItemNumber, argSelf=proxy(self): argSelf.__ClickSexButton(subItemNumber))

        self.bonusChoose = ui.ComboBoxImage(self, IMG_DIR + "class_image2.tga",10,45+(35*6))
        self.bonusChoose.SetCurrentItem(self.bonusItems.itervalues().next())
        self.bonusChoose.SetParent(self.filterTable)
        self.bonusChoose.SetWindowName("bonusChoose")
        for index, data in self.bonusItems.iteritems():
            self.bonusChoose.InsertItem(index, data)
        self.bonusChoose.SetEvent(lambda subItemNumber, argSelf=proxy(self): argSelf.__ClickBonusButton(subItemNumber))

        self.alchemyChoose = ui.ComboBoxImage(self, IMG_DIR + "class_image2.tga",10,45+(35*5))
        self.alchemyChoose.SetCurrentItem(self.alchemyItems.itervalues().next())
        self.alchemyChoose.SetWindowName("alchemyChoose")
        self.alchemyChoose.SetParent(self.filterTable)
        for index, data in self.alchemyItems.iteritems():
            self.alchemyChoose.InsertItem(index, data)
        self.alchemyChoose.SetEvent(lambda subItemNumber, argSelf=proxy(self): argSelf.__ClickAlchemyButton(subItemNumber))

        self.filterButton.SetEvent(ui.__mem_func__(self.__ClickFilterButton))

        self.alchemyChoose.Hide()
        self.filterTable.Hide()
        self.bonusChoose.Hide()
        self.sexChoose.Hide()
        self.characterChoose.Hide()

        self.listBoxButtons = ui.DropdownTree()
        self.listBoxButtons.SetParent(self)
        self.listBoxButtons.SetPosition(13,60)
        self.listBoxButtons.SetSize(185, 490)
        self.listBoxButtons.SetItemSize(185, 29)
        self.listBoxButtons.SetViewItemCount(16)
        self.listBoxButtons.isShopSearch=1
        self.listBoxButtons.Show()

        self.scrollBarListBoxCube = ui.ScrollBarNew()
        self.scrollBarListBoxCube.SetParent(self)
        self.scrollBarListBoxCube.SetPosition(13+185,60)
        self.scrollBarListBoxCube.SetScrollBarSize(470)
        self.scrollBarListBoxCube.Show()

        self.listBoxButtons.SetScrollBar(self.scrollBarListBoxCube)

        newself = proxy(self)
        listBoxCubeItems = [
            {
                'item' : self.CreateTypeButtonMenuTab1Item(localeInfo.PRIVATESHOPSEARCH_WEAPON, 0, lambda arg = (0): newself.__EmptyFunction(arg)), 
            },
            {
                'item' : self.CreateTypeButtonMenuTab1Item(localeInfo.PRIVATESHOPSEARCH_EQUIPMENT, 1, lambda arg = (1): newself.__EmptyFunction(arg)), 
            },
            
            {
                'item' : self.CreateTypeButtonMenuTab1Item(localeInfo.PRIVATESHOPSEARCH_COSTUMES, 2, lambda arg = (2): newself.__EmptyFunction(arg)), 
            },
        
            {
                'item' : self.CreateTypeButtonMenuTab1Item(localeInfo.PRIVATESHOPSEARCH_DRAGONSOULALCHEMY, 3, lambda arg = (3): newself.__EmptyFunction(arg)), 
            },
        
            {
                'item' : self.CreateTypeButtonMenuTab1Item(localeInfo.PRIVATESHOPSEARCH_OTHERITEMS, 4, lambda arg = (4): newself.__EmptyFunction(arg)), 
            },
        
            {
                'item' : self.CreateTypeButtonMenuTab1Item(localeInfo.PRIVATESHOPSEARCH_BOOKS, 5, lambda arg = (5): newself.__EmptyFunction(arg)), 
            },
        
            {
                'item' : self.CreateTypeButtonMenuTab1Item(localeInfo.PRIVATESHOPSEARCH_PETS, 6, lambda arg = (6): newself.__EmptyFunction(arg)), 
            },
        
        ]
        
        self.listBoxButtons.AppendItemList(listBoxCubeItems)
        self.LoadSkillNames()


    def __EmptyFunction(self, arg):
        self.SetSearchInfo(arg,0)
        self.StartSearchbyType()

    def __SelectType(self, arg):
        type,subtype = int(arg[0]),int(arg[2])
        self.SetSearchInfo(type,subtype)
        self.StartSearchbySubType()

    def __ClickDirection(self, t,arg):
        if arg == 1:
            if self.countDirectionValue == 0:
                self.countDirectionValue =1
                self.countDirecton.SetUpVisual(IMG_DIR+"up_1.tga")
                self.countDirecton.SetOverVisual(IMG_DIR+"up_0.tga")
                self.countDirecton.SetDownVisual(IMG_DIR+"up_1.tga")
            else:
                self.countDirectionValue =0
                self.countDirecton.SetUpVisual(IMG_DIR+"down_1.tga")
                self.countDirecton.SetOverVisual(IMG_DIR+"down_0.tga")
                self.countDirecton.SetDownVisual(IMG_DIR+"down_1.tga")
            self.RefreshList()

    def CreateTypeButtonMenuTab1Item(self, text, type, event, offset = 0):
        listboxItem = CubeMenuTab1(text,type)
        listboxItem.SetVisible(True)
        listboxItem.SetEvent(event)
        listboxItem.SetOffset(offset)
        return listboxItem

    def CreateLastListItem(self, text, event, offset = 0):
        listboxItem = ui.LastListItem(text)
        listboxItem.SetEvent(event)
        listboxItem.SetOffset(offset)
        return listboxItem

    def __ClickCharacterButton(self,p):
        self.characterValue = p
        self.characterChoose.SetCurrentItem(self.characterItems[p])
        self.characterChoose.CloseListBox()
    
    def __ClickSexButton(self,p):
        self.sexValue = p
        self.sexChoose.SetCurrentItem(self.sexItems[p])
        self.sexChoose.CloseListBox()

    def __ClickBonusButton(self,p):
        self.bonusValue = p
        self.bonusChoose.SetCurrentItem(self.bonusItems[p])
        self.bonusChoose.CloseListBox()

    def __ClickAlchemyButton(self,p):
        self.alchemyValue = p
        self.alchemyChoose.SetCurrentItem(self.alchemyItems[p])
        self.alchemyChoose.CloseListBox()

    def __ClickFilterButton(self):
        if self.filterTable.IsShow():
            self.alchemyChoose.Hide()
            self.filterTable.Hide()
            self.bonusChoose.Hide()
            self.sexChoose.Hide()
            self.characterChoose.Hide()
        else:
            self.alchemyChoose.Show()
            self.filterTable.Show()
            self.bonusChoose.Show()
            self.sexChoose.Show()
            self.characterChoose.Show()

    def CloseFiltre(self):
        self.alchemyChoose.Hide()
        self.filterTable.Hide()
        self.bonusChoose.Hide()
        self.sexChoose.Hide()
        self.characterChoose.Hide()

    def __OnValueUpdateMinAbs(self):
        ui.EditLine.OnIMEUpdate(self.minAbs)
        try:
            self.minAbsValue = int(self.minAbs.GetText())
        except:
            self.minAbsValue = 0

    def __OnValueUpdateMaxAbs(self):
        ui.EditLine.OnIMEUpdate(self.maxAbs)
        try:
            self.maxAbsValue = int(self.maxAbs.GetText())
        except:
            self.maxAbsValue = 30


    def __OnValueUpdateMinLevel(self):
        ui.EditLine.OnIMEUpdate(self.minLevel)
        try:
            self.minlevelValue = int(self.minLevel.GetText())
        except:
            self.minlevelValue = 0

    def __OnValueUpdateMaxLevel(self):
        ui.EditLine.OnIMEUpdate(self.maxLevel)
        try:
            self.maxlevelValue = int(self.maxLevel.GetText())
        except:
            self.maxlevelValue = 120
    
    def __OnValueUpdateMinRefine(self):
        ui.EditLine.OnIMEUpdate(self.minRefine)
        try:
            self.minRefineValue = int(self.minRefine.GetText())
        except:
            self.minRefineValue = 0

    def __OnValueUpdateMaxRefine(self):
        ui.EditLine.OnIMEUpdate(self.maxRefine)
        try:
            self.maxRefineValue = int(self.maxRefine.GetText())
        except:
            self.maxRefineValue = 200

    def __OnValueUpdateMinSkill(self):
        ui.EditLine.OnIMEUpdate(self.minSkill)
        try:
            self.minSkillValue = int(self.minSkill.GetText())
        except:
            self.minSkillValue = 0

    def __OnValueUpdateMaxSkill(self):
        ui.EditLine.OnIMEUpdate(self.maxSkill)
        try:
            self.maxSkillValue = int(self.maxSkill.GetText())
        except:
            self.maxSkillValue = 40

    def __OnValueUpdateMaxAvg(self):
        ui.EditLine.OnIMEUpdate(self.maxAvg)
        try:
            self.maxAvgValue = int(self.maxAvg.GetText())
        except:
            self.maxAvgValue = 80

    def __OnValueUpdateMinAvg(self):
        ui.EditLine.OnIMEUpdate(self.minAvg)
        try:
            self.minAvgValue = int(self.minAvg.GetText())
        except:
            self.minAvgValue = 0

#############################################################
    def __OnValueUpdate(self):
        ui.EditLine.OnIMEUpdate(self.itemNameSearch)
        self.UpdateItemsList()
        if len(self.itemNameSearch.GetText()) > 0:
            self.searchClearBtn.Show()

        def OnKeyDown(self, key):
            if app.DIK_RETURN == key:
                if self.selectedItem > 1:
                    self.StartSearchbyVnum()
                else:
                    #self.StartSearchbyVnum()
                    return True
            return True

    def ClearText(self):
        self.itemNameSearch.SetText("")
        self.itemNameSearchText.SetText("")
        #self.itemNameSearchText.SetFeather(True)
        self.searchClearBtn.Hide()
        self.selectedItem = 0

    def LoadSkillNames(self):
        if len(self.skillList) == 0:
            item.SelectItem(50300)
            text = item.GetItemName()
            for j in xrange(160):
                skill_name = skill.GetSkillName(j)
                if skill_name != "NoneSkill":
                    if skill.SKILL_TYPE_ACTIVE == skill.GetSkillType(j):
                        skill_list = []
                        skill_list.append(j)
                        name = skill_name+" "+text
                        skill_list.append(name.lower())
                        self.skillList.append(skill_list)

    def GetSkillIndex(self, string):
        for skill in self.skillList:
            if skill[1] == string:
                return skill[0]
        return 0

    def getSkillNames(self, string):
        list = []
        for skill in self.skillList:
            if skill[1].find(string) != -1:
                list.append(skill[1])
        return list

    def UpdateItemsList(self):
        text = self.itemNameSearch.GetText()
        text_len = len(self.itemNameSearch.GetText())

        if text_len > 0:
            text_lower = self.itemNameSearch.GetText().lower()

            self.searchClearBtn.Show()

            items_list = item.GetItemsByName(str(text_lower))
            for i, items in enumerate(items_list, start=1):
                item.SelectItem(items)
                if text_len > len(item.GetItemName()):
                    self.itemNameSearchText.SetText("")
                    #self.itemNameSearch.SetInfoMessage("")
                    continue

                pos = item.GetItemName().find("+")
                if pos != -1:
                    text = item.GetItemName()[:pos]
                    if text.lower() == text_lower:
                        self.selectedItem=items
                    self.itemNameSearchText.SetText(text.lower())
                    #self.itemNameSearch.SetInfoMessage(text.lower())
                else:
                    if item.GetItemName().lower() == text_lower:
                        self.selectedItem=items
                    self.itemNameSearchText.SetText(item.GetItemName().lower())
                    #self.itemNameSearch.SetInfoMessage(item.GetItemName().lower())

            skill_list = self.getSkillNames(str(text_lower))
            for skill in skill_list:
                if text_len > len(skill):
                    self.itemNameSearchText.SetText("")
                    #self.itemNameSearch.SetInfoMessage("")
                    continue
                if skill == text_lower:
                    self.selectedItem=50300
                self.itemNameSearchText.SetText(skill)
                #self.itemNameSearch.SetInfoMessage(skill)

            if len(self.itemNameSearchText.GetText().lower()) == text_len:
                self.itemNameSearchText.SetText("")
                #self.itemNameSearch.SetInfoMessage("")
        else:
            self.selectedItem=0
            self.itemNameSearch.SetText("")
            #self.itemNameSearch.SetInfoMessage("")
            self.itemNameSearchText.SetText("")
            self.searchClearBtn.Hide()

    def Open(self, type):
        self.RefreshMe()
        self.Show()
        self.SetCenterPosition()
        self.SetTop()
        
        if self.isFirstOpen == False:
            self.isFirstOpen = True
            self.__EmptyFunction(-1)

    def RefreshMe(self):
        background.DeletePrivateShopPos()
        self.itemDataList[:] = []
        self.ListBox.RemoveAllItems()
        self.itemCount = privateShopSearch.GetItemCount()
        for x in xrange(privateShopSearch.GetItemCount()):
            vnum = privateShopSearch.GetSearchItemVnum(x)
            count = int(privateShopSearch.GetSearchItemCount(x))
            price = long(privateShopSearch.GetSearchItemPrice(x))
            vid = privateShopSearch.GetSearchItemShopVID(x)
            seller = privateShopSearch.GetSearchItemSeller(x)
            itemPos = privateShopSearch.GetSearchItemPos(x)
            if app.WJ_CHANGELOOK_SYSTEM:
                transmutation = privateShopSearch.GetSearchItemTransmutation(x)
            metinSlot = []
            for i in xrange(player.METIN_SOCKET_MAX_NUM):
                metinSlot.append(privateShopSearch.GetSearchItemMetinSocket(x, i))
            attrSlot = []
            for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
                attrSlot.append(privateShopSearch.GetSearchItemAttribute(x, i))

            if app.WJ_CHANGELOOK_SYSTEM:
                self.itemDataList.append((vnum, count, price, seller, metinSlot, attrSlot, itemPos, vid, transmutation))
            else:
                self.itemDataList.append((vnum, count, price, seller, metinSlot, attrSlot, itemPos, vid))

        self.pageCount = int(math.ceil(float(self.itemCount) / float(self.perPage)))
        self.currentPaginationPage = 1
        self.paginationPageCount = int(math.ceil(float(self.pageCount) / 5.0 ))
        self.FirstPage()

    def get_count(self,ptr):
        return ptr[1]

    def RefreshList(self):
        background.DeletePrivateShopPos()
        self.selectedItemIndex = -1
        self.RefreshPaginationButtons()
        self.ListBox.RemoveAllItems()
        start = (self.currentPage - 1) * self.perPage
        end = ((self.currentPage - 1) * self.perPage) + self.perPage

        currentPageDict = self.itemDataList[start:end]

        if self.countDirectionValue == 1:
            currentPageDict =  sorted(currentPageDict, key=self.get_count,reverse=True)

        basePos = 2
        pos = 0
        for x, data in enumerate(currentPageDict):
            vnum = data[0]
            count = int(data[1])
            price = long(data[2])
            seller = data[3]
            metinSlot = data[4]
            attrSlot = data[5]
            itemPos = data[6]
            vid = data[7]
            if app.WJ_CHANGELOOK_SYSTEM:
                transmutation = data[8]
            item.SelectItem(vnum)

            (width, height) = item.GetItemSize()
            size = 1
            if height == 64:
                size = 2
            if height == 96:
                size = 3
            resultItem = PrivateShopSearchDialog.SearchResultItem(self.ListBox,x,height)

            resultItem.SetParent(self.ListBox)
            resultItem.SetPosition(10, basePos,True)
            resultItem.SetMetinSlot(metinSlot)
            resultItem.SetAttrSlot(attrSlot)
            resultItem.SetItemVnum(vnum)

            if 50300 == vnum or vnum == 70037:
                skillIndex = metinSlot[0]
                skillName = skill.GetSkillName(skillIndex)
                itemName = skillName + " " + item.GetItemName()
                resultItem.SetItemName(itemName)
            elif 70104 == vnum:
                mobvnum = metinSlot[0]
                itemName = nonplayer.GetMonsterName(mobvnum)
                itemName += " "
                itemName += item.GetItemName()
                resultItem.SetItemName(itemName)
            else:
                resultItem.SetItemName(item.GetItemName())

            resultItem.SetSeller(seller)
            resultItem.SetCount(int(count))
            resultItem.SetPrice(long(price))
            resultItem.SetShopItemPos(itemPos)
            resultItem.SetShopVid(vid)
            if app.WJ_CHANGELOOK_SYSTEM:
                resultItem.SetTransmutation(int(transmutation))
            resultItem.Show()
            self.ListBox.AppendItem(resultItem)

            items = self.ListBox.GetItems()
            if items[len(items)-1] != None:
                (x,y) = items[len(items)-1].GetLocalPosition()
                size = 50
                if items[len(items)-1].size == 2:
                    size = 82
                elif items[len(items)-1].size == 3:
                    size = 114
                basePos=y+size+3
        self.ListBox.Render(0)

    def RefreshPaginationButtons(self):
        self.currentPaginationPage = int(math.ceil(float(self.currentPage) / 5.0 ))
        self.shownPages = min(self.pageCount - (5 * (self.currentPaginationPage - 1)), 5)

        for x in xrange(5):
            currentPage = (x + ((self.currentPaginationPage-1) * 5) + 1)
            self.pageButtons[x].SetUp()
            self.pageButtons[x].SetText("%d" % currentPage)
            self.pageButtons[x].SetEvent(lambda arg=currentPage: self.GotoPage(arg))

        map(ui.Button.Hide, self.pageButtons)
        map(ui.Button.Enable, self.pageButtons)
        for x in xrange(self.shownPages):
            self.pageButtons[x].Show()
        self.pageButtons[(self.currentPage - ((self.currentPaginationPage - 1) * 5)) - 1].Down()
        self.pageButtons[(self.currentPage - ((self.currentPaginationPage - 1) * 5)) - 1].Disable()

    def GotoPage(self, page):
        self.currentPage = page
        self.RefreshList()

    def FirstPage(self):
        self.currentPage = 1
        self.RefreshList()

    def LastPage(self):
        self.currentPage = self.pageCount
        self.RefreshList()

    def NextPage(self):
        if self.currentPage < self.pageCount:
            self.currentPage += 1

        self.RefreshList()

    def PrevPage(self):
        if self.currentPage > 1:
            self.currentPage -= 1
        self.RefreshList()

    def SendTypeandValuePacket(self, arg):
        privateShopSearch.ClearSearchItems()
        background.DeletePrivateShopPos()
        self.RefreshMe()

        if arg == 0 or arg == 1:
            net.SendPrivateShopSearchInfo(int(self.SearchTypeValue),int(self.SearchSubTypeValue),int(self.minAvgValue),int(self.maxAvgValue),int(self.minSkillValue),int(self.maxSkillValue),int(self.minAbsValue),int(self.maxAbsValue),int(self.minlevelValue),int(self.maxlevelValue),int(self.minRefineValue),int(self.maxRefineValue),int(self.bonusValue),int(self.alchemyValue),int(self.sexValue),int(self.characterValue),0,0)
        else:
            if self.selectedItem == 0:
                return
            
            value = 0
            if self.selectedItem == 50300:
                value = self.GetSkillIndex(self.itemNameSearch.GetText().lower())
            else:
                item.SelectItem(self.selectedItem)
                pos = item.GetItemName().find("+")
                if pos != -1:
                    namenew= item.GetItemName()[pos+1:]
                    if len(namenew) != 0:
                        value = int(namenew)
            net.SendPrivateShopSearchInfo(int(self.SearchTypeValue),int(self.SearchSubTypeValue),int(self.minAvgValue),int(self.maxAvgValue),int(self.minSkillValue),int(self.maxSkillValue),int(self.minAbsValue),int(self.maxAbsValue),int(self.minlevelValue),int(self.maxlevelValue),int(self.minRefineValue),int(self.maxRefineValue),int(self.bonusValue),int(self.alchemyValue),int(self.sexValue),int(self.characterValue),value,self.selectedItem)

    def StartSearchbyType(self):
        self.SendTypeandValuePacket(1)

    def StartSearchbySubType(self):
        self.SendTypeandValuePacket(0)

    def StartSearchbyVnum(self):
        self.SendTypeandValuePacket(2)

    def BuySelectedItem(self):
        if self.selectedItemIndex == -1:
            return
        shopVid = self.ListBox[self.selectedItemIndex].GetShopVid()
        shopItemPos = self.ListBox[self.selectedItemIndex].GetShopItemPos()
        net.SendOfflineShopBuyPacket(shopVid, shopItemPos)

    def SetSearchInfo(self,p,i):
        self.SearchTypeValue = p
        self.SearchSubTypeValue = i

    def OnSearchResultItemSelect(self, index,item_name,item_count,item_price,shopVid,shopItemPos, itemIndex):
        background.DeletePrivateShopPos()
        self.selectedItemIndex = index
        background.CreatePrivateShopPos(shopVid)

        text = localeInfo.DO_YOU_BUY_ITEM(item_name,item_count,item_price)

        self.searchBuyItem.SetText1(text)
        self.searchBuyItem.SetText2(localeInfo.OFFLINESHOP_BUY_DIALOG)
        self.searchBuyItem.SetAcceptText(localeInfo.UI_ACCEPT)
        self.searchBuyItem.SetCancelText(localeInfo.UI_DENY)
        self.searchBuyItem.SetAcceptEvent(lambda arg=1: self.__AnswerBuyItem(arg))
        self.searchBuyItem.SetCancelEvent(lambda arg=0: self.__AnswerBuyItem(arg))
        #self.searchBuyItem.SetWidth(len(text)*5)
        self.searchBuyItem.shopVid = shopVid
        self.searchBuyItem.shopItemPos = shopItemPos
        self.searchBuyItem.Open()

    def __AnswerBuyItem(self,p):
        self.searchBuyItem.Close()
        if p:
            net.SendOfflineShopBuyPacket(self.searchBuyItem.shopVid, self.searchBuyItem.shopItemPos)
        else:
            pass

    def Close(self):
        background.DeletePrivateShopPos()
        self.Hide()
        return True

    def OnKeyDown(self, key):
        if key == 28:
            self.StartSearchbyVnum()

    def Clear(self):
        self.Refresh()

    def Refresh(self):
        pass

    def __OnCloseButtonClick(self):
        self.Hide()

    def OnPressEscapeKey(self):
        self.Close()

#class Item(ui.ListBoxEx.Item):
#    def __init__(self, text, idx):
#        ui.ListBoxEx.Item.__init__(self)
#        self.canLoad=0
#        self.text=text
#        self.id=idx
#        self.textLine=self.__CreateTextLine(text[:50])
#        ui.ListBoxEx.Item.SetPosition(self, 30, 0)
#        ui.ListBoxEx.Item.SetSize(self,150,20)
#    def __del__(self):
#        ui.ListBoxEx.Item.__del__(self)
#    def Destroy(self):
#        self.canLoad=0
#        self.text=0
#        self.id=0
#        self.textLine=0
#        ui.ListBoxEx.Item.Destroy(self)
#    def GetText(self):
#        return self.text
#    def GetIDX(self):
#        return int(self.id)
#    def SetSize(self, width, height):
#        ui.ListBoxEx.Item.SetSize(self, width, height)
#    def __CreateTextLine(self, text):
#        textLine=ui.TextLine()
#        textLine.SetParent(self)
#        textLine.SetHorizontalAlignCenter()
#        textLine.SetPosition(77, 0)
#        textLine.SetText(text)
#        textLine.Show()
#        return textLine

class CubeMenuTab1(ui.DropdownTree.Item):
    def __init__(self, text, type):
        ui.DropdownTree.Item.__init__(self)
        self.overLine = False
        self.type = type

        self.iconBackgroundImage = ui.ImageBox()
        self.iconBackgroundImage.SetParent(self)
        self.iconBackgroundImage.AddFlag("not_pick")
        self.iconBackgroundImage.SetPosition(0,0)
        self.iconBackgroundImage.LoadImage(IMG_DIR +"button_01.tga")
        self.iconBackgroundImage.Show()

        self.iconButtonType = ui.ImageBox()
        self.iconButtonType.SetParent(self.iconBackgroundImage)
        self.iconButtonType.AddFlag("not_pick")
        self.iconButtonType.SetPosition(10,5)
        self.iconButtonType.SetSize(0,0)
        self.iconButtonType.LoadImage(IMG_DIR +"icon_%d.tga"%self.type)
        self.iconButtonType.Show()

        self.textLine=ui.TextLine()
        self.textLine.SetParent(self.iconBackgroundImage)
        self.textLine.SetPosition(35,5)
        self.textLine.SetWindowHorizontalAlignLeft()
        self.textLine.SetFontName("Tahoma:15")
        self.textLine.SetText(text)
        self.textLine.Show()

        self.SetOnExpandEvent(self.ExpandEvent)
        self.SetOnCollapseEvent(self.CollapseEvent)

    def __del__(self):
        ui.DropdownTree.Item.__del__(self)

    def Destroy(self):
        ui.DropdownTree.Item.Destroy(self)
        self.overLine = 0
        self.type = 0
        self.iconButtonType =0
        self.textLine=0
        self.iconBackgroundImage = 0

    def SetSize(self, width, height):
        ui.DropdownTree.Item.SetSize(self, width-self.GetOffset(), height)

    def CollapseEvent(self):
        self.iconBackgroundImage.LoadImage(IMG_DIR +"button_01.tga")
        self.iconBackgroundImage.Show()

    def ExpandEvent(self):
        self.iconBackgroundImage.LoadImage(IMG_DIR +"button_02.tga")
        self.iconBackgroundImage.Show()

    def GetText(self):
        return self.textLine.GetText()

    
