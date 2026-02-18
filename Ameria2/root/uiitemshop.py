# coding: latin_1

import ui, uiToolTip, grp, item, event, time, constInfo, dbg, app, uiCommon, chat, math, localeInfo, player, chat

ENABLE_DRS_SHOP = 1
ENABLE_3RD_SHOP = 0
ENABLE_WHEEL = 0
ENABLE_HOME = 1
ITEM_MAX_COUNT    = 200

class ItemShopWindow(ui.ScriptWindow):

    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.isLoaded = FALSE
        self.__LoadWindow()
        
    def __LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/ItemShopWindow.py")
        except:
            import exception
            exception.Abort("ItemShopWindow.LoadWindow.LoadObject")
            
        self.pages = ["HOME", "ITEMSHOP", "DRS_SHOP", "3RD_SHOP", "WHEEL", "LOGS", "GM_ZONE"]
            
        self.categorys = {
            'itemshop' : [
                [1, "Special items"],
                [2, "Alchemy"],
                [3, "Costumes"],
                [4, "Hairs"],
                [5, "Mounts"],
                [6, "Sash"],
                [7, "Pets"],
            ],
            'drs_shop' : [
                [1, "Special items"],
            ],
            '3rd_shop' : [
                [1, "3rd Armas"],
                [2, "Armaduras"],
                [3, "BlaBla"],
            ]
        }
        
        self.shop_names = [localeInfo.ITEMSHOP_ISHOP_NAME, localeInfo.ITEMSHOP_DRSSHOP_NAME, localeInfo.ITEMSHOP_3RDSHOP_NAME]
        self.int_pos = [[206,38],[267,50],[319,83],[355,136],[375,199],[358,263],[321,317],[265,346],[206,359],[144,345],[91,314],[53,262],[40,199],[54,137],[92,88],[143,52]]
        self.wheelStartSpin = False
        self.wheelRandomNumber = 0
        self.wheelRotationSpeed = 5
        self.wheelRotationCurrent = 0
        self.wheelRotationLimit = 0
        self.wheelCooldown = None
        self.bannerChangeTime = 15 # time in seconds which the banner will change it automatically
        self.adminItemList = {}
        self.adminTmpAddVnum = 0
        self.adminCategory = 0
        self.curShop = 0
        self.isSearching = False
        self.searchList = []
        self.isGM = False
        
        self.bannerVar = {'fadeOut' : 0, 'currentTime' : 0, 'intervallEndTime' : 0, 'currentAlphaValue' : 0, 'currentImage' : 0, 'lastSwitch' : time.clock() + self.bannerChangeTime}
        self.bannerOptions = {'folder' : app.GetLocalePath() + '/ui/itemshop/', 'time' : 10, 'timeToFade' : 0.04, 'interval' : 0.05, 'banner_0' : 'banner_0', 'banner_1' : 'banner_1'}
        
        self.curPage = 'HOME'
        self.curAdminPage = 'NONE'
    
        self.__BindObjects()
        self.__BindEvents()
        
        for i in xrange(len(self.shop_names)):
            self.elements['admin']['shop_type'].InsertItem(i, self.shop_names[i])
            
    def __BindObjects(self):
        self.board = self.GetChild("board")
        
        self.arrows = {'home' : {'mostBought' : 1, 'hotOffers' : 1}, 'itemshop' : {'categorys' : 1, 'items' : 1}, 'drs_shop' : {'categorys' : 1, 'items' : 1}, '3rd_shop' : {'categorys' : 1, 'items' : 1}, 'logs' : 1}
        self.category = {'itemshop' : 0, 'drs_shop' : 0, '3rd_shop' : 0, 'admin' : 0}
        
        self.elements = {
            'money' : {'coins' : self.GetChild("CoinsValue"), 'drs' : self.GetChild("DrsValue"), '3rd_coin' : self.GetChild("3rd_Value"), 'update' : self.GetChild("UpdateCoinsButton")},
            'home' : {'mostBought' : [], 'hotOffers' : [], 'mostBoughtItems' : {'box_0' : ItemBox(), 'box_1' : ItemBox(), 'box_2' : ItemBox()}, 'hotOffersItems' : {'box_0' : ItemBox(), 'box_1' : ItemBox()}, 'buttons' : {'arrowLeft' : self.GetChild("HomeArrowLeft"), 'arrowRight' : self.GetChild("HomeArrowRight"), 'arrowUp' : self.GetChild("HomeArrowUp"), 'arrowDown' : self.GetChild("HomeArrowDown")}, 'banner' : {'banner_0' : self.GetChild("Banner1"), 'banner_1' : self.GetChild("Banner2"), 'change_0' : self.GetChild("ChangeBanner1"), 'change_1' : self.GetChild("ChangeBanner2")}},
            'itemshop' : {'buttons' : {'arrowUp' : self.GetChild("ItemShopArrowUp"), 'arrowDown' : self.GetChild("ItemShopArrowDown"), 'arrowLeft' : self.GetChild("ItemShopArrowLeft"), 'arrowRight' : self.GetChild("ItemShopArrowRight")}, 'items' : {'box_0' : ItemBox(), 'box_1' : ItemBox(), 'box_2' : ItemBox(), 'box_3' : ItemBox(), 'box_4' : ItemBox(), 'box_5' : ItemBox(), 'box_6' : ItemBox(), 'box_7' : ItemBox(), 'box_8' : ItemBox()}, 'textline' : {'page_nr' : self.GetChild("ItemShopPageNumber")}},
            'drs_shop' : {'buttons' : {'arrowUp' : self.GetChild("DrsShopArrowUp"), 'arrowDown' : self.GetChild("DrsShopArrowDown"), 'arrowLeft' : self.GetChild("DrsShopArrowLeft"), 'arrowRight' : self.GetChild("DrsShopArrowRight")}, 'items' : {'box_0' : ItemBox(), 'box_1' : ItemBox(), 'box_2' : ItemBox(), 'box_3' : ItemBox(), 'box_4' : ItemBox(), 'box_5' : ItemBox(), 'box_6' : ItemBox(), 'box_7' : ItemBox(), 'box_8' : ItemBox()}, 'textline' : {'page_nr' : self.GetChild("DrsShopPageNumber")}},
            '3rd_shop' : {'buttons' : {'arrowUp' : self.GetChild("3rdShopArrowUp"), 'arrowDown' : self.GetChild("3rdShopArrowDown"), 'arrowLeft' : self.GetChild("3rdShopArrowLeft"), 'arrowRight' : self.GetChild("3rdShopArrowRight")}, 'items' : {'box_0' : ItemBox(), 'box_1' : ItemBox(), 'box_2' : ItemBox(), 'box_3' : ItemBox(), 'box_4' : ItemBox(), 'box_5' : ItemBox(), 'box_6' : ItemBox(), 'box_7' : ItemBox(), 'box_8' : ItemBox()}, 'textline' : {'page_nr' : self.GetChild("3rdShopPageNumber")}},
            'wheel' : {'images' : {'bg' : self.GetChild("BgWheel"), 'rotation' : self.GetChild("RotationWheel")}, 'buttons' : {'spin' : self.GetChild("SpinButton")}, 'objects' : {}, 'items' : [], 'best_items' : []},
            'logs' : {'items' : {'box_0' : LogBox(), 'box_1' : LogBox(), 'box_2' : LogBox(), 'box_3' : LogBox(), 'box_4' : LogBox(), 'box_5' : LogBox(), 'box_6' : LogBox(), 'box_7' : LogBox(), 'box_8' : LogBox()}, 'textline' : {'page_nr' : self.GetChild("LogsPageNumber")}, 'buttons' : {'arrowLeft' : self.GetChild("LogsArrowLeft"), 'arrowRight' : self.GetChild("LogsArrowRight")}},
            'admin' : {
                'buttons' : {'addItemTime' : self.GetChild("AdminAddItemTime"), 'addNewItem' : self.GetChild("AdminAddNewItem"), 'return' : self.GetChild("ReturnButton")},
                'addItemTimeWindow' : self.GetChild("AddItemTimeWindow"),
                'addItemWindow' : self.GetChild("AddItemWindow"),
                'categoryListBox' : self.GetChild("CategorysListBox"),
                'categoryListBoxScroll' : self.GetChild("CategorysListBoxScroll"),
                'itemListBox' : self.GetChild("ItemsListBox"),
                'itemListBoxScroll' : self.GetChild("ItemsListBoxScroll"),
                'discountBox' : DiscountItemBox(),
                'categoryDiscountBox' : DiscountCategoryBox(),
                'bonusBoxes' : {'attrtype0' : self.GetChild("AddItemAttrtype0"), 'attrvalue0' : self.GetChild("AddItemAttrvalue0"), 'attrtype1' : self.GetChild("AddItemAttrtype1"), 'attrvalue1' : self.GetChild("AddItemAttrvalue1"), 'attrtype2' : self.GetChild("AddItemAttrtype2"), 'attrvalue2' : self.GetChild("AddItemAttrvalue2"), 'attrtype3' : self.GetChild("AddItemAttrtype3"), 'attrvalue3' : self.GetChild("AddItemAttrvalue3"), 'attrtype4' : self.GetChild("AddItemAttrtype4"), 'attrvalue4' : self.GetChild("AddItemAttrvalue4"), 'attrtype5' : self.GetChild("AddItemAttrtype5"), 'attrvalue5' : self.GetChild("AddItemAttrvalue5"), 'attrtype6' : self.GetChild("AddItemAttrtype6"), 'attrvalue6' : self.GetChild("AddItemAttrvalue6")},
                'socketsBoxes' : {'socket0' : self.GetChild("AddItemSocket0"), 'socket1' : self.GetChild("AddItemSocket1"), 'socket2' : self.GetChild("AddItemSocket2")},
                'applyAddItem' : self.GetChild("AddItemApply"), 'resetAddItem' : self.GetChild("AddItemReset"), 'iconAddItem' : self.GetChild("AddItemIcon"), 'vnumAddItem' : self.GetChild("AddItemVnum"), 'priceAddItem' : self.GetChild("AddItemPrice"),
            }
        }

        self.shopType = ui.ComboBox()
        self.shopType.SetParent(self.GetChild("AddItemNewBoard"))
        self.shopType.SetPosition(70, 2+28*4)
        self.shopType.SetSize(100, 20)
        self.shopType.SetCurrentItem(self.shop_names[0])
        self.shopType.SetEvent(self.SelectShop)
        self.shopType.Show()
        self.elements['admin'].update({'shop_type':self.shopType})
        
        self.shopTypeText = ui.TextLine()
        self.shopTypeText.SetParent(self.shopType)
        self.shopTypeText.SetPosition(-37, 0)
        self.shopTypeText.SetWindowVerticalAlignCenter()
        self.shopTypeText.SetVerticalAlignCenter()
        self.shopTypeText.SetText(localeInfo.ITEMSHOP_SHOP_TITLE)
        self.shopTypeText.SetOutline()
        self.shopTypeText.Show()
        
        self.categoryAdmin = ui.ComboBox()
        self.categoryAdmin.SetParent(self.GetChild("AddItemNewBoard"))
        self.categoryAdmin.SetPosition(70, 2+28*2)
        self.categoryAdmin.SetSize(80, 20)
        self.categoryAdmin.SetEvent(self.SelectCategoryAdmin)
        self.categoryAdmin.Show()
        
        self.page = []
        self.page.append(self.GetChild("HomeWindow"))
        self.page.append(self.GetChild("IShopWindow"))
        self.page.append(self.GetChild("DrsShopWindow"))
        self.page.append(self.GetChild("3rdShopWindow"))
        self.page.append(self.GetChild("WheelWindow"))
        self.page.append(self.GetChild("LogsWindow"))
        self.page.append(self.GetChild("GmWindow"))
        
        self.tab = []
        self.tab.append(self.GetChild("HomeTab"))
        self.tab.append(self.GetChild("IShopTab"))
        self.tab.append(self.GetChild("DrsShopTab"))
        self.tab.append(self.GetChild("3rdShopTab"))
        self.tab.append(self.GetChild("WheelTab"))
        self.tab.append(self.GetChild("LogsTab"))
        self.tab.append(self.GetChild("GmTab"))

        if ENABLE_DRS_SHOP and not ENABLE_3RD_SHOP:
            self.tab[3].Hide()
            self.tab[4].SetPosition(18+(100+8)*3, 40)
            self.tab[5].SetPosition(18+(100+8)*4, 40)
            self.tab[6].SetPosition(18+(100+8)*5, 40)
        elif not ENABLE_DRS_SHOP and ENABLE_3RD_SHOP:
            self.tab[2].Hide()
            self.tab[3].SetPosition(18+(100+8)*2, 40)
            self.tab[4].SetPosition(18+(100+8)*3, 40)
            self.tab[5].SetPosition(18+(100+8)*4, 40)
            self.tab[6].SetPosition(18+(100+8)*5, 40)
        self.tab[6].Hide()
        
        if not ENABLE_WHEEL:
            self.tab[5].SetPosition(18+(100+8)*3, 40)
            self.tab[4].Hide()
            
        if not ENABLE_HOME:
            self.tab[0].Hide()
        
        if not ENABLE_3RD_SHOP:
            self.GetChild("3rd_Slot").Hide()
        
        for i in xrange(16):
            self.elements['wheel']['objects']['tooltip_%d' % i] = self.CreateToolTip()
            self.elements['wheel']['objects']['int_%d' % i] = self.CreateImage(self.elements['wheel']['images']['rotation'], app.GetLocalePath() + "/ui/itemshop/int_icon.tga", self.int_pos[i][0], self.int_pos[i][1])
            self.elements['wheel']['objects']['icon_%d' % i] = self.CreateImage(self.elements['wheel']['objects']['int_%d' % i], None, 0, 0)
            try:
                self.elements['wheel']['objects']['icon_%d' % i].SetMouseOverInEvent(self.elements['wheel']['objects']['tooltip_%d' % i].ShowToolTip)
                self.elements['wheel']['objects']['icon_%d' % i].SetMouseOverOutEvent(self.elements['wheel']['objects']['tooltip_%d' % i].HideToolTip)
            except:
                self.elements['wheel']['objects']['icon_%d' % i].SAFE_SetStringEvent("MOUSE_OVER_IN", self.elements['wheel']['objects']['tooltip_%d' % i].ShowToolTip)
                self.elements['wheel']['objects']['icon_%d' % i].SAFE_SetStringEvent("MOUSE_OVER_OUT", self.elements['wheel']['objects']['tooltip_%d' % i].HideToolTip)
            self.elements['wheel']['objects']['icon_%d' % i].SetWindowHorizontalAlignCenter()
            self.elements['wheel']['objects']['icon_%d' % i].SetWindowVerticalAlignCenter()
            
        for i in xrange(6):
            self.elements['wheel']['objects']['bgbar_%d' % i] = self.CreateBar(self.GetChild("bgBestItems"), 0, 12+42+66*i, 0, 0)
            self.elements['wheel']['objects']['tooltipbests_%d' % i] = self.CreateToolTip()
            self.elements['wheel']['objects']['iconbests_%d' % i] = self.CreateImage(self.elements['wheel']['objects']['bgbar_%d' % i], None, 0, 0)
            try:
                self.elements['wheel']['objects']['iconbests_%d' % i].SetMouseOverInEvent(self.elements['wheel']['objects']['tooltipbests_%d' % i].ShowToolTip)
                self.elements['wheel']['objects']['iconbests_%d' % i].SetMouseOverOutEvent(self.elements['wheel']['objects']['tooltipbests_%d' % i].HideToolTip)
            except:
                self.elements['wheel']['objects']['iconbests_%d' % i].SAFE_SetStringEvent("MOUSE_OVER_IN", self.elements['wheel']['objects']['tooltipbests_%d' % i].ShowToolTip)
                self.elements['wheel']['objects']['iconbests_%d' % i].SAFE_SetStringEvent("MOUSE_OVER_OUT", self.elements['wheel']['objects']['tooltipbests_%d' % i].HideToolTip)
            self.elements['wheel']['objects']['iconbests_%d' % i].SetWindowVerticalAlignCenter()
            self.elements['wheel']['objects']['namebests_%d' % i] = self.CreateText(self.elements['wheel']['objects']['bgbar_%d' % i])
            self.elements['wheel']['objects']['namebests_%d' % i].SetPosition(50, 0)
            self.elements['wheel']['objects']['namebests_%d' % i].SetWindowVerticalAlignCenter()
            self.elements['wheel']['objects']['namebests_%d' % i].SetVerticalAlignCenter()
            
        self.adminAddNewItemToolTip = uiToolTip.ItemToolTip()
        self.adminAddNewItemToolTip.Hide()
        
    def __BindEvents(self):
        self.board.SetCloseEvent(ui.__mem_func__(self.Close))
        
        for i in xrange(len(self.tab)):
            self.tab[i].SetEvent(lambda tabName = self.pages[i] : self.SelectPage(tabName))
            
        self.elements['itemshop']['buttons']['arrowUp'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'ITEMSHOP_CATEGORYS_UP')
        self.elements['itemshop']['buttons']['arrowDown'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'ITEMSHOP_CATEGORYS_DOWN')
        self.elements['itemshop']['buttons']['arrowLeft'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'ITEMSHOP_ITEMS_LEFT')
        self.elements['itemshop']['buttons']['arrowRight'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'ITEMSHOP_ITEMS_RIGHT')
        self.elements['drs_shop']['buttons']['arrowUp'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'DRSSHOP_CATEGORYS_UP')
        self.elements['drs_shop']['buttons']['arrowDown'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'DRSSHOP_CATEGORYS_DOWN')
        self.elements['drs_shop']['buttons']['arrowLeft'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'DRSSHOP_ITEMS_LEFT')
        self.elements['drs_shop']['buttons']['arrowRight'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'DRSSHOP_ITEMS_RIGHT')
        self.elements['3rd_shop']['buttons']['arrowUp'].SetEvent(ui.__mem_func__(self.__OnClickArrow), '3RDSHOP_CATEGORYS_UP')
        self.elements['3rd_shop']['buttons']['arrowDown'].SetEvent(ui.__mem_func__(self.__OnClickArrow), '3RDSHOP_CATEGORYS_DOWN')
        self.elements['3rd_shop']['buttons']['arrowLeft'].SetEvent(ui.__mem_func__(self.__OnClickArrow), '3RDSHOP_ITEMS_LEFT')
        self.elements['3rd_shop']['buttons']['arrowRight'].SetEvent(ui.__mem_func__(self.__OnClickArrow), '3RDSHOP_ITEMS_RIGHT')
        self.elements['home']['buttons']['arrowRight'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'HOME_MOSTBOUGHT_RIGHT')
        self.elements['home']['buttons']['arrowLeft'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'HOME_MOSTBOUGHT_LEFT')
        self.elements['home']['buttons']['arrowDown'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'HOME_HOTOFFERS_DOWN')
        self.elements['home']['buttons']['arrowUp'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'HOME_HOTOFFERS_UP')
        self.elements['logs']['buttons']['arrowLeft'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'LOGS_LEFT')
        self.elements['logs']['buttons']['arrowRight'].SetEvent(ui.__mem_func__(self.__OnClickArrow), 'LOGS_RIGHT')
        
        self.elements['wheel']['buttons']['spin'].SetEvent(ui.__mem_func__(self.__OnClickRequestSpinWheel))
        self.elements['admin']['categoryListBox'].SetScrollBar(self.elements['admin']['categoryListBoxScroll'])
        self.elements['admin']['itemListBox'].SetScrollBar(self.elements['admin']['itemListBoxScroll'])
        
        self.elements['admin']['buttons']['addItemTime'].SetEvent(ui.__mem_func__(self.__OnClickAdminButton), 'ITEM_TIME')
        self.elements['admin']['buttons']['addNewItem'].SetEvent(ui.__mem_func__(self.__OnClickAdminButton), 'ADD_ITEM')
        
        self.elements['admin']['applyAddItem'].SetEvent(ui.__mem_func__(self.__OnClickAddNewItem))
        self.elements['admin']['resetAddItem'].SetEvent(ui.__mem_func__(self.__OnClickResetAddNewItem))
        self.GetChild("SearchButton").SetEvent(ui.__mem_func__(self.__OnClickSearchButton))
        self.GetChild("SearchValue").SetReturnEvent(ui.__mem_func__(self.__OnClickSearchButton))
        self.GetChild("SearchValue").SetEscapeEvent(ui.__mem_func__(self.Close))
        
        for i in xrange(2):
            self.elements['home']['banner']['change_%d' % i].SetEvent(ui.__mem_func__(self.SwitchBanner), i)
        
        self.elements['admin']['buttons']['return'].SetEvent(ui.__mem_func__(self.SelectPage), 'GM_ZONE')
        self.elements['admin']['discountBox'].Open(self.elements['admin']['addItemTimeWindow'], 320, 80)
        self.elements['admin']['categoryDiscountBox'].Open(self.elements['admin']['addItemTimeWindow'], 320, 80+180+15)
        
        self.elements['itemshop']['items']['box_0'].SetSettings(self.page[1], 160, 9)
        self.elements['itemshop']['items']['box_1'].SetSettings(self.page[1], 160+202+14, 9)
        self.elements['itemshop']['items']['box_2'].SetSettings(self.page[1], 160+(202+14)*2, 9)
        self.elements['itemshop']['items']['box_3'].SetSettings(self.page[1], 160, 9+(76+45+16)+10)
        self.elements['itemshop']['items']['box_4'].SetSettings(self.page[1], 160+202+14, 9+(76+45+16)+10)
        self.elements['itemshop']['items']['box_5'].SetSettings(self.page[1], 160+(202+14)*2, 9+(76+45+16)+10)
        self.elements['itemshop']['items']['box_6'].SetSettings(self.page[1], 160, 9+((76+45+16)+10)*2)
        self.elements['itemshop']['items']['box_7'].SetSettings(self.page[1], 160+202+14, 9+((76+45+16)+10)*2)
        self.elements['itemshop']['items']['box_8'].SetSettings(self.page[1], 160+(202+14)*2, 9+((76+45+16)+10)*2)
        
        self.elements['drs_shop']['items']['box_0'].SetSettings(self.page[2], 160, 9)
        self.elements['drs_shop']['items']['box_1'].SetSettings(self.page[2], 160+202+14, 9)
        self.elements['drs_shop']['items']['box_2'].SetSettings(self.page[2], 160+(202+14)*2, 9)
        self.elements['drs_shop']['items']['box_3'].SetSettings(self.page[2], 160, 9+(76+45+16)+10)
        self.elements['drs_shop']['items']['box_4'].SetSettings(self.page[2], 160+202+14, 9+(76+45+16)+10)
        self.elements['drs_shop']['items']['box_5'].SetSettings(self.page[2], 160+(202+14)*2, 9+(76+45+16)+10)
        self.elements['drs_shop']['items']['box_6'].SetSettings(self.page[2], 160, 9+((76+45+16)+10)*2)
        self.elements['drs_shop']['items']['box_7'].SetSettings(self.page[2], 160+202+14, 9+((76+45+16)+10)*2)
        self.elements['drs_shop']['items']['box_8'].SetSettings(self.page[2], 160+(202+14)*2, 9+((76+45+16)+10)*2)
        
        self.elements['3rd_shop']['items']['box_0'].SetSettings(self.page[3], 160, 9)
        self.elements['3rd_shop']['items']['box_1'].SetSettings(self.page[3], 160+202+14, 9)
        self.elements['3rd_shop']['items']['box_2'].SetSettings(self.page[3], 160+(202+14)*2, 9)
        self.elements['3rd_shop']['items']['box_3'].SetSettings(self.page[3], 160, 9+(76+45+16)+10)
        self.elements['3rd_shop']['items']['box_4'].SetSettings(self.page[3], 160+202+14, 9+(76+45+16)+10)
        self.elements['3rd_shop']['items']['box_5'].SetSettings(self.page[3], 160+(202+14)*2, 9+(76+45+16)+10)
        self.elements['3rd_shop']['items']['box_6'].SetSettings(self.page[3], 160, 9+((76+45+16)+10)*2)
        self.elements['3rd_shop']['items']['box_7'].SetSettings(self.page[3], 160+202+14, 9+((76+45+16)+10)*2)
        self.elements['3rd_shop']['items']['box_8'].SetSettings(self.page[3], 160+(202+14)*2, 9+((76+45+16)+10)*2)
    
        self.elements['home']['mostBoughtItems']['box_0'].SetSettings(self.page[0], 39+14+10, 324+14)
        self.elements['home']['mostBoughtItems']['box_1'].SetSettings(self.page[0], 39+14+10+202+40, 324+14)
        self.elements['home']['mostBoughtItems']['box_2'].SetSettings(self.page[0], 39+14+10+(202+40)*2, 324+14)
        
        self.elements['home']['hotOffersItems']['box_0'].SetSettings(self.page[0], 85+14+10+(202+40)*2, 33)
        self.elements['home']['hotOffersItems']['box_1'].SetSettings(self.page[0], 85+14+10+(202+40)*2, 33+117+14)
        
        self.elements['logs']['items']['box_0'].SetSettings(self.page[5], 25, 18)
        self.elements['logs']['items']['box_1'].SetSettings(self.page[5], 25+250+10, 18)
        self.elements['logs']['items']['box_2'].SetSettings(self.page[5], 25+(250+10)*2, 18)
        self.elements['logs']['items']['box_3'].SetSettings(self.page[5], 25, 18+120+18)
        self.elements['logs']['items']['box_4'].SetSettings(self.page[5], 25+250+10, 18+120+18)
        self.elements['logs']['items']['box_5'].SetSettings(self.page[5], 25+(250+10)*2, 18+120+18)
        self.elements['logs']['items']['box_6'].SetSettings(self.page[5], 25, 18+(120+18)*2)
        self.elements['logs']['items']['box_7'].SetSettings(self.page[5], 25+250+10, 18+(120+18)*2)
        self.elements['logs']['items']['box_8'].SetSettings(self.page[5], 25+(250+10)*2, 18+(120+18)*2)
        
        self.elements['admin']['vnumAddItem'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['priceAddItem'].SetFocus))
        self.elements['admin']['vnumAddItem'].SetTabEvent(ui.__mem_func__(self.elements['admin']['priceAddItem'].SetFocus))
        self.elements['admin']['vnumAddItem'].SetEscapeEvent(ui.__mem_func__(self.Close))
        self.elements['admin']['priceAddItem'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype0'].SetFocus))
        self.elements['admin']['priceAddItem'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype0'].SetFocus))
        self.elements['admin']['priceAddItem'].SetEscapeEvent(ui.__mem_func__(self.Close))
        self.elements['admin']['bonusBoxes']['attrtype0'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue0'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype0'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue0'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue0'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype1'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue0'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype1'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype1'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue1'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype1'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue1'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue1'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype2'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue1'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype2'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype2'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue2'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype2'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue2'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue2'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype3'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue2'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype3'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype3'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue3'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype3'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue3'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue3'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype4'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue3'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype4'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype4'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue4'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype4'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue4'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue4'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype5'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue4'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype5'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype5'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue5'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype5'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue5'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue5'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype6'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue5'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrtype6'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype6'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue6'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrtype6'].SetTabEvent(ui.__mem_func__(self.elements['admin']['bonusBoxes']['attrvalue6'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue6'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['socketsBoxes']['socket0'].SetFocus))
        self.elements['admin']['bonusBoxes']['attrvalue6'].SetTabEvent(ui.__mem_func__(self.elements['admin']['socketsBoxes']['socket0'].SetFocus))
        self.elements['admin']['socketsBoxes']['socket0'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['socketsBoxes']['socket1'].SetFocus))
        self.elements['admin']['socketsBoxes']['socket0'].SetTabEvent(ui.__mem_func__(self.elements['admin']['socketsBoxes']['socket1'].SetFocus))
        self.elements['admin']['socketsBoxes']['socket1'].SetReturnEvent(ui.__mem_func__(self.elements['admin']['socketsBoxes']['socket2'].SetFocus))
        self.elements['admin']['socketsBoxes']['socket1'].SetTabEvent(ui.__mem_func__(self.elements['admin']['socketsBoxes']['socket2'].SetFocus))
        self.elements['admin']['socketsBoxes']['socket2'].SetReturnEvent(ui.__mem_func__(self.__OnClickAddNewItem))
        self.elements['admin']['socketsBoxes']['socket2'].SetTabEvent(ui.__mem_func__(self.__OnClickAddNewItem))
        
        for i in xrange(7):
            self.elements['admin']['bonusBoxes']['attrtype%d' %i].SetEscapeEvent(ui.__mem_func__(self.Close))
            self.elements['admin']['bonusBoxes']['attrvalue%d' %i].SetEscapeEvent(ui.__mem_func__(self.Close))
        for i in xrange(3):
            self.elements['admin']['socketsBoxes']['socket%d' % i].SetEscapeEvent(ui.__mem_func__(self.Close))
        
        try:
            self.elements['admin']['iconAddItem'].SetMouseOverInEvent(self.adminAddNewItemToolTip.ShowToolTip)
            self.elements['admin']['iconAddItem'].SetMouseOverOutEvent(self.adminAddNewItemToolTip.HideToolTip)
        except:
            self.elements['admin']['iconAddItem'].SAFE_SetStringEvent("MOUSE_OVER_IN", self.adminAddNewItemToolTip.ShowToolTip)
            self.elements['admin']['iconAddItem'].SAFE_SetStringEvent("MOUSE_OVER_OUT", self.adminAddNewItemToolTip.HideToolTip)
        self.elements['money']['update'].SetEvent(ui.__mem_func__(self.__OnClickUpdateCoins))
    
        self.isLoaded = TRUE
        
    def CreateCategoryButton(self, parent, x, y, text, func, arg):
        button = ui.Button()
        button.SetParent(parent)
        button.SetUpVisual("d:/ymir work/ui/public/large_button_01.sub")
        button.SetOverVisual("d:/ymir work/ui/public/large_button_02.sub")
        button.SetDownVisual("d:/ymir work/ui/public/large_button_03.sub")
        button.SetText(text)
        button.SetEvent(ui.__mem_func__(func), arg)
        button.SetPosition(x, y)
        button.Show()
        return button
        
    def CreateToolTip(self):
        toolTip = uiToolTip.ItemToolTip()
        toolTip.HideToolTip()
        return toolTip
        
    def CreateImage(self, parent, src_img, x, y):
        img = ui.ExpandedImageBox()
        img.SetParent(parent)
        img.SetPosition(x, y)
        if src_img != None:
            img.LoadImage(src_img)
        img.Show()
        return img
        
    def CreateBar(self, parent, x, y, sizex, sizey):
        bar = ui.Bar()
        bar.SetParent(parent)
        bar.SetPosition(x, y)
        bar.SetSize(sizex, sizey)
        bar.SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 0.0))
        bar.Show()
        return bar
        
    def CreateText(self, parent):
        text = ui.TextLine()
        text.SetParent(parent)
        text.Show()
        return text
        
    def Set3rdCoins(self, coins):
        self.elements['money']['3rd_coin'].SetText(localeInfo.ITEMSHOP_3RDSHOP_COINS % coins)
        
    def RefreshIShopCategorys(self):
        try:
            for i in xrange(12):
                self.elements['itemshop']['buttons']['category_%d' % i].Hide()
        except:
            pass
        try:
            for i in xrange(min(12, len(self.categorys['itemshop']))):
                scrolledId = i + self.arrows['itemshop']['categorys']
                self.elements['itemshop']['buttons']['category_%d' % i] = self.CreateCategoryButton(self.page[1], 25, 35+35*i, self.categorys['itemshop'][scrolledId][1], self.__OnClickSelectCategoryIShop, self.categorys['itemshop'][scrolledId][0])
                self.elements['itemshop']['buttons']['category_%d' % i].Show()
        except:
            pass
            
        if (len(self.categorys['itemshop']) > 12):
            if (self.arrows['itemshop']['categorys'] <= 0):
                self.elements['itemshop']['buttons']['arrowDown'].Show()
                self.elements['itemshop']['buttons']['arrowUp'].Hide()
            elif (self.arrows['itemshop']['categorys']+12 < len(self.categorys['itemshop'])):
                self.elements['itemshop']['buttons']['arrowDown'].Show()
                self.elements['itemshop']['buttons']['arrowUp'].Show()
            elif (self.arrows['itemshop']['categorys']+12 >= len(self.categorys['itemshop'])):
                self.elements['itemshop']['buttons']['arrowDown'].Hide()
                self.elements['itemshop']['buttons']['arrowUp'].Show()
        else:
            self.elements['itemshop']['buttons']['arrowDown'].Hide()
            self.elements['itemshop']['buttons']['arrowUp'].Hide()
            
    def RefreshIShopItems(self):
        curPage = self.arrows['itemshop']['items']
        for i in xrange(9):
            self.elements['itemshop']['items']['box_%d' % i].Hide()

        try:
            if self.isSearching:
                for i in xrange(min(9, len(self.searchList) - curPage * 9 +9)):
                    curItem = self.searchList[i + (curPage - 1)*9]
                    self.elements['itemshop']['items']['box_%d' % i].SetContent(curItem[0], curItem[1], curItem[3], curItem[4], curItem[2])
                    self.elements['itemshop']['items']['box_%d' % i].SetPercent(curItem[7])
                    self.elements['itemshop']['items']['box_%d' % i].SetTime(curItem[5], curItem[6])
                    self.elements['itemshop']['items']['box_%d' % i].SetGM(self.isGM)
                    self.elements['itemshop']['items']['box_%d' % i].Show()
            else:
                for i in xrange(min(9, len(constInfo.ItemShop['ITEMS']['itemshop'][self.category['itemshop']]) - curPage * 9 +9)):
                    curItem = constInfo.ItemShop['ITEMS']['itemshop'][self.category['itemshop']][i + (curPage - 1)*9]
                    self.elements['itemshop']['items']['box_%d' % i].SetContent(curItem[0], curItem[1], curItem[3], curItem[4], curItem[2])
                    self.elements['itemshop']['items']['box_%d' % i].SetPercent(curItem[7])
                    self.elements['itemshop']['items']['box_%d' % i].SetTime(curItem[5], curItem[6])
                    self.elements['itemshop']['items']['box_%d' % i].SetGM(self.isGM)
                    self.elements['itemshop']['items']['box_%d' % i].Show()
        except:
            self.elements['itemshop']['buttons']['arrowRight'].Hide()
            self.elements['itemshop']['buttons']['arrowLeft'].Hide()
            self.GetChild("ItemShopPageNumberSlot").Hide()
            return
        
        if self.isSearching:
            maxPage = math.ceil(float(len(self.searchList))/float(9))
        else:
            maxPage = math.ceil(float(len(constInfo.ItemShop['ITEMS']['itemshop'][self.category['itemshop']]))/float(9))
        if maxPage >= 1:
            self.elements['itemshop']['textline']['page_nr'].SetText('%d/%d' % (curPage, maxPage))
            self.GetChild("ItemShopPageNumberSlot").Show()
        else:
            self.GetChild("ItemShopPageNumberSlot").Hide()
        
        if self.isSearching:
            if curPage * 9 >= len(self.searchList):
                self.elements['itemshop']['buttons']['arrowRight'].Hide()
            else:
                self.elements['itemshop']['buttons']['arrowRight'].Show()
        else:
            if curPage * 9 >= len(constInfo.ItemShop['ITEMS']['itemshop'][self.category['itemshop']]):
                self.elements['itemshop']['buttons']['arrowRight'].Hide()
            else:
                self.elements['itemshop']['buttons']['arrowRight'].Show()

        if curPage > 1:
            self.elements['itemshop']['buttons']['arrowLeft'].Show()
        else:
            self.elements['itemshop']['buttons']['arrowLeft'].Hide()
            
    def RefreshDrsShopCategorys(self):
        try:
            for i in xrange(12):
                self.elements['drs_shop']['buttons']['category_%d' % i].Hide()
        except:
            pass
        try:
            for i in xrange(min(12, len(self.categorys['drs_shop']))):
                scrolledId = i + self.arrows['drs_shop']['categorys']
                self.elements['drs_shop']['buttons']['category_%d' % i] = self.CreateCategoryButton(self.page[2], 25, 35+35*i, self.categorys['drs_shop'][scrolledId][1], self.__OnClickSelectCategoryDrsShop, self.categorys['drs_shop'][scrolledId][0])
                self.elements['drs_shop']['buttons']['category_%d' % i].Show()
        except:
            pass
            
        if (len(self.categorys['drs_shop']) > 12):
            if (self.arrows['drs_shop']['categorys'] <= 0):
                self.elements['drs_shop']['buttons']['arrowDown'].Show()
                self.elements['drs_shop']['buttons']['arrowUp'].Hide()
            elif (self.arrows['drs_shop']['categorys']+12 < len(self.categorys['drs_shop'])):
                self.elements['drs_shop']['buttons']['arrowDown'].Show()
                self.elements['drs_shop']['buttons']['arrowUp'].Show()
            elif (self.arrows['drs_shop']['categorys']+12 >= len(self.categorys['drs_shop'])):
                self.elements['drs_shop']['buttons']['arrowDown'].Hide()
                self.elements['drs_shop']['buttons']['arrowUp'].Show()
        else:
            self.elements['drs_shop']['buttons']['arrowDown'].Hide()
            self.elements['drs_shop']['buttons']['arrowUp'].Hide()
            
    def RefreshDrsShopItems(self):
        curPage = self.arrows['drs_shop']['items']
        for i in xrange(9):
            self.elements['drs_shop']['items']['box_%d' % i].Hide()

        try:
            if self.isSearching:
                for i in xrange(min(9, len(self.searchList) - curPage * 9 +9)):
                    curItem = self.searchList[i + (curPage - 1)*9]
                    self.elements['drs_shop']['items']['box_%d' % i].SetCoin(2)
                    self.elements['drs_shop']['items']['box_%d' % i].SetContent(curItem[0], curItem[1], curItem[3], curItem[4], curItem[2])
                    self.elements['drs_shop']['items']['box_%d' % i].SetPercent(curItem[7])
                    self.elements['drs_shop']['items']['box_%d' % i].SetTime(curItem[5], curItem[6])
                    self.elements['drs_shop']['items']['box_%d' % i].SetGM(self.isGM)
                    self.elements['drs_shop']['items']['box_%d' % i].Show()
            else:
                for i in xrange(min(9, len(constInfo.ItemShop['ITEMS']['drs_shop'][self.category['drs_shop']]) - curPage * 9 +9)):
                    curItem = constInfo.ItemShop['ITEMS']['drs_shop'][self.category['drs_shop']][i + (curPage - 1)*9]
                    self.elements['drs_shop']['items']['box_%d' % i].SetCoin(2)
                    self.elements['drs_shop']['items']['box_%d' % i].SetContent(curItem[0], curItem[1], curItem[3], curItem[4], curItem[2])
                    self.elements['drs_shop']['items']['box_%d' % i].SetPercent(curItem[7])
                    self.elements['drs_shop']['items']['box_%d' % i].SetTime(curItem[5], curItem[6])
                    self.elements['drs_shop']['items']['box_%d' % i].SetGM(self.isGM)
                    self.elements['drs_shop']['items']['box_%d' % i].Show()
        except:
            self.elements['drs_shop']['buttons']['arrowRight'].Hide()
            self.elements['drs_shop']['buttons']['arrowLeft'].Hide()
            self.GetChild("DrsShopPageNumberSlot").Hide()
            return
        
        if self.isSearching:
            maxPage = math.ceil(float(len(self.searchList))/float(9))
        else:
            maxPage = math.ceil(float(len(constInfo.ItemShop['ITEMS']['drs_shop'][self.category['drs_shop']]))/float(9))
        if maxPage >= 1:
            self.elements['drs_shop']['textline']['page_nr'].SetText('%d/%d' % (curPage, maxPage))
            self.GetChild("DrsShopPageNumberSlot").Show()
        else:
            self.GetChild("DrsShopPageNumberSlot").Hide()
        
        if self.isSearching:
            if curPage * 9 >= len(self.searchList):
                self.elements['drs_shop']['buttons']['arrowRight'].Hide()
            else:
                self.elements['drs_shop']['buttons']['arrowRight'].Show()
        else:
            if curPage * 9 >= len(constInfo.ItemShop['ITEMS']['drs_shop'][self.category['drs_shop']]):
                self.elements['drs_shop']['buttons']['arrowRight'].Hide()
            else:
                self.elements['drs_shop']['buttons']['arrowRight'].Show()

        if curPage > 1:
            self.elements['drs_shop']['buttons']['arrowLeft'].Show()
        else:
            self.elements['drs_shop']['buttons']['arrowLeft'].Hide()
            
    def Refresh3rdShopCategorys(self):
        try:
            for i in xrange(12):
                self.elements['3rd_shop']['buttons']['category_%d' % i].Hide()
        except:
            pass
        try:
            for i in xrange(min(12, len(self.categorys['3rd_shop']))):
                scrolledId = i + self.arrows['3rd_shop']['categorys']
                self.elements['3rd_shop']['buttons']['category_%d' % i] = self.CreateCategoryButton(self.page[3], 25, 35+35*i, self.categorys['3rd_shop'][scrolledId][1], self.__OnClickSelectCategory3rdShop, self.categorys['3rd_shop'][scrolledId][0])
                self.elements['3rd_shop']['buttons']['category_%d' % i].Show()
        except:
            pass
            
        if (len(self.categorys['3rd_shop']) > 12):
            if (self.arrows['3rd_shop']['categorys'] <= 0):
                self.elements['3rd_shop']['buttons']['arrowDown'].Show()
                self.elements['3rd_shop']['buttons']['arrowUp'].Hide()
            elif (self.arrows['3rd_shop']['categorys']+12 < len(self.categorys['3rd_shop'])):
                self.elements['3rd_shop']['buttons']['arrowDown'].Show()
                self.elements['3rd_shop']['buttons']['arrowUp'].Show()
            elif (self.arrows['3rd_shop']['categorys']+12 >= len(self.categorys['3rd_shop'])):
                self.elements['3rd_shop']['buttons']['arrowDown'].Hide()
                self.elements['3rd_shop']['buttons']['arrowUp'].Show()
        else:
            self.elements['3rd_shop']['buttons']['arrowDown'].Hide()
            self.elements['3rd_shop']['buttons']['arrowUp'].Hide()
            
    def Refresh3rdShopItems(self):
        curPage = self.arrows['3rd_shop']['items']
        for i in xrange(9):
            self.elements['3rd_shop']['items']['box_%d' % i].Hide()

        try:
            if self.isSearching:
                for i in xrange(min(9, len(self.searchList) - curPage * 9 +9)):
                    curItem = self.searchList[i + (curPage - 1)*9]
                    self.elements['3rd_shop']['items']['box_%d' % i].SetCoin(3)
                    self.elements['3rd_shop']['items']['box_%d' % i].SetContent(curItem[0], curItem[1], curItem[3], curItem[4], curItem[2])
                    self.elements['3rd_shop']['items']['box_%d' % i].SetPercent(curItem[7])
                    self.elements['3rd_shop']['items']['box_%d' % i].SetTime(curItem[5], curItem[6])
                    self.elements['3rd_shop']['items']['box_%d' % i].SetGM(self.isGM)
                    self.elements['3rd_shop']['items']['box_%d' % i].Show()
            else:
                for i in xrange(min(9, len(constInfo.ItemShop['ITEMS']['3rd_shop'][self.category['3rd_shop']]) - curPage * 9 +9)):
                    curItem = constInfo.ItemShop['ITEMS']['3rd_shop'][self.category['3rd_shop']][i + (curPage - 1)*9]
                    self.elements['3rd_shop']['items']['box_%d' % i].SetCoin(3)
                    self.elements['3rd_shop']['items']['box_%d' % i].SetContent(curItem[0], curItem[1], curItem[3], curItem[4], curItem[2])
                    self.elements['3rd_shop']['items']['box_%d' % i].SetPercent(curItem[7])
                    self.elements['3rd_shop']['items']['box_%d' % i].SetTime(curItem[5], curItem[6])
                    self.elements['3rd_shop']['items']['box_%d' % i].SetGM(self.isGM)
                    self.elements['3rd_shop']['items']['box_%d' % i].Show()
        except:
            self.elements['3rd_shop']['buttons']['arrowRight'].Hide()
            self.elements['3rd_shop']['buttons']['arrowLeft'].Hide()
            self.GetChild("3rdShopPageNumberSlot").Hide()
            return
        
        if self.isSearching:
            maxPage = math.ceil(float(len(self.searchList))/float(9))
        else:
            maxPage = math.ceil(float(len(constInfo.ItemShop['ITEMS']['3rd_shop'][self.category['3rd_shop']]))/float(9))
        if maxPage >= 1:
            self.elements['3rd_shop']['textline']['page_nr'].SetText('%d/%d' % (curPage, maxPage))
            self.GetChild("3rdShopPageNumberSlot").Show()
        else:
            self.GetChild("3rdShopPageNumberSlot").Hide()
        
        if self.isSearching:
            if curPage * 9 >= len(self.searchList):
                self.elements['3rd_shop']['buttons']['arrowRight'].Hide()
            else:
                self.elements['3rd_shop']['buttons']['arrowRight'].Show()
        else:
            if curPage * 9 >= len(constInfo.ItemShop['ITEMS']['3rd_shop'][self.category['3rd_shop']]):
                self.elements['3rd_shop']['buttons']['arrowRight'].Hide()
            else:
                self.elements['3rd_shop']['buttons']['arrowRight'].Show()

        if curPage > 1:
            self.elements['3rd_shop']['buttons']['arrowLeft'].Show()
        else:
            self.elements['3rd_shop']['buttons']['arrowLeft'].Hide()
            
    def RefreshIShopMostBought(self):
        curPage = self.arrows['home']['mostBought']
        for i in xrange(3):
            self.elements['home']['mostBoughtItems']['box_%d' % i].Hide()

        try:
            for i in xrange(min(3, len(constInfo.ItemShop['ITEMS']['mostBought']) - curPage * 3 +3)):
                curItem = constInfo.ItemShop['ITEMS']['mostBought'][i + (curPage - 1)*3]
                realSockets = self.GetRealSockets(curItem[1])
                self.elements['home']['mostBoughtItems']['box_%d' % i].SetContent(curItem[0], curItem[1], curItem[3], realSockets, curItem[2])
                self.elements['home']['mostBoughtItems']['box_%d' % i].SetPercent(curItem[7])
                self.elements['home']['mostBoughtItems']['box_%d' % i].SetTime(curItem[5], curItem[6])
                self.elements['home']['mostBoughtItems']['box_%d' % i].SetGM(self.isGM)
                self.elements['home']['mostBoughtItems']['box_%d' % i].Show()
        except:
            self.elements['home']['buttons']['arrowRight'].Hide()
            self.elements['home']['buttons']['arrowLeft'].Hide()
            return
            
        if curPage * 3 >= len(constInfo.ItemShop['ITEMS']['mostBought']):
            self.elements['home']['buttons']['arrowRight'].Hide()
        else:
            self.elements['home']['buttons']['arrowRight'].Show()

        if curPage > 1:
            self.elements['home']['buttons']['arrowLeft'].Show()
        else:
            self.elements['home']['buttons']['arrowLeft'].Hide()
            
    def RefreshIShopHotOffers(self):
        curPage = self.arrows['home']['hotOffers']
        for i in xrange(2):
            self.elements['home']['hotOffersItems']['box_%d' % i].Hide()

        try:
            for i in xrange(min(2, len(constInfo.ItemShop['ITEMS']['hotOffers']) - curPage * 2 +2)):
                curItem = constInfo.ItemShop['ITEMS']['hotOffers'][i + (curPage - 1)*2]
                self.elements['home']['hotOffersItems']['box_%d' % i].SetContent(curItem[0], curItem[1], curItem[3], curItem[4], curItem[2])
                self.elements['home']['hotOffersItems']['box_%d' % i].SetPercent(curItem[7])
                self.elements['home']['hotOffersItems']['box_%d' % i].SetTime(curItem[5], curItem[6])
                self.elements['home']['hotOffersItems']['box_%d' % i].SetGM(self.isGM)
                self.elements['home']['hotOffersItems']['box_%d' % i].Show()
        except:
            self.elements['home']['buttons']['arrowDown'].Hide()
            self.elements['home']['buttons']['arrowUp'].Hide()
            return
            
        if curPage * 2 >= len(constInfo.ItemShop['ITEMS']['hotOffers']):
            self.elements['home']['buttons']['arrowDown'].Hide()
        else:
            self.elements['home']['buttons']['arrowDown'].Show()

        if curPage > 1:
            self.elements['home']['buttons']['arrowUp'].Show()
        else:
            self.elements['home']['buttons']['arrowUp'].Hide()

    def RefreshWheelOfDestiny(self):
        if self.wheelStartSpin: return
        self.wheelRotationLimit = 0
        self.wheelRotationCurrent = 0
        for i in xrange(16):
            self.elements['wheel']['objects']['icon_%d' % i].Hide()
        for i in xrange(6):
            self.elements['wheel']['objects']['bgbar_%d' % i].Hide()
        self.elements['wheel']['images']['rotation'].SetRotation(0)
        
    def RefreshLogs(self):
        curPage = self.arrows['logs']
        for i in xrange(9):
            self.elements['logs']['items']['box_%d' % i].Hide()

        try:
            for i in xrange(min(9, len(constInfo.ItemShop['LOGS']) - curPage * 9 +9)):
                curItem = constInfo.ItemShop['LOGS'][i + (curPage - 1)*9]
                self.elements['logs']['items']['box_%d' % i].SetContent(curItem[0], curItem[1], curItem[2], curItem[4], curItem[5])
                self.elements['logs']['items']['box_%d' % i].SetDate(curItem[3].split('[_]')[0], curItem[3].split('[_]')[1])
                self.elements['logs']['items']['box_%d' % i].Show()
        except:
            self.elements['logs']['buttons']['arrowRight'].Hide()
            self.elements['logs']['buttons']['arrowLeft'].Hide()
            self.GetChild("LogsPageNumberSlot").Hide()
            return
            
        maxPage = math.ceil(float(len(constInfo.ItemShop['LOGS']))/float(9))
        if maxPage >= 1:
            self.elements['logs']['textline']['page_nr'].SetText('%d/%d' % (curPage, maxPage))
            self.GetChild("LogsPageNumberSlot").Show()
        else:
            self.GetChild("LogsPageNumberSlot").Hide()
        
        if curPage * 9 >= len(constInfo.ItemShop['LOGS']):
            self.elements['logs']['buttons']['arrowRight'].Hide()
        else:
            self.elements['logs']['buttons']['arrowRight'].Show()

        if curPage > 1:
            self.elements['logs']['buttons']['arrowLeft'].Show()
        else:
            self.elements['logs']['buttons']['arrowLeft'].Hide()
            
    def SelectShop(self, idx):
        self.elements['admin']['shop_type'].SetCurrentItem(self.shop_names[idx])
        self.curShop = idx
        self.elements['admin']['categoryListBox'].RemoveAllItems()
        self.RefreshAdminItems()
        self.elements['admin']['itemListBox'].RemoveAllItems()
        self.elements['admin']['itemListBoxScroll'].Hide()
        
        self.categoryAdmin.ClearItem()
        for cat in xrange(len(self.categorys[('itemshop', 'drs_shop', '3rd_shop')[idx]])):
            self.categoryAdmin.InsertItem(self.categorys[('itemshop', 'drs_shop', '3rd_shop')[idx]][cat][0], self.categorys[('itemshop', 'drs_shop', '3rd_shop')[idx]][cat][1])
        self.categoryAdmin.SelectItem(0)
        
    def GetRealSockets(self, vnum):
        for c in xrange(len(self.categorys['itemshop'])):
            items = constInfo.ItemShop['ITEMS']['itemshop'][self.categorys['itemshop'][c][0]]
            for i in xrange(len(items)):
                if items[i][1] == vnum:
                    return items[i][4]
        return (0, 0, 0)
        
    def SelectCategoryAdmin(self, idx):
        self.adminCategory = idx
        self.categoryAdmin.SetCurrentItem(self.categorys[('itemshop', 'drs_shop', '3rd_shop')[self.curShop]][self.adminCategory - 1][1])
        
    def RefreshAdminItems(self):
        for cat in self.categorys[('itemshop','drs_shop','3rd_shop')[self.curShop]]:
            self.adminItemList[cat[0]] = []
            self.elements['admin']['categoryListBox'].AppendItem(Item(cat[1]))
        if len(self.categorys[('itemshop','drs_shop','3rd_shop')[self.curShop]]) > 8:
            self.elements['admin']['categoryListBoxScroll'].SetMiddleBarSize(float(8)/float(len(self.adminItemList)))
            self.elements['admin']['categoryListBoxScroll'].Show()
        else:
            self.elements['admin']['categoryListBoxScroll'].Hide()
        self.elements['admin']['categoryListBox'].SetSelectEvent(self.RefreshAdminItemLists)

    def RefreshAdminItemLists(self, selectItem):
        self.elements['admin']['discountBox'].Hide()
        cat = self.categorys[('itemshop','drs_shop','3rd_shop')[self.curShop]][self.elements['admin']['categoryListBox'].GetItemIndex(selectItem)][0]
        self.elements['admin']['categoryDiscountBox'].SetCategoryID(cat)
        self.elements['admin']['categoryDiscountBox'].SetShop(self.curShop)
        self.elements['admin']['categoryDiscountBox'].SetPosition(320, 80)
        self.elements['admin']['categoryDiscountBox'].Show()
        self.category['admin'] = cat
        self.elements['admin']['itemListBox'].RemoveAllItems()
        try:
            for i in xrange(len(constInfo.ItemShop['ITEMS'][('itemshop','drs_shop','3rd_shop')[self.curShop]][cat])):
                item.SelectItem(constInfo.ItemShop['ITEMS'][('itemshop','drs_shop','3rd_shop')[self.curShop]][cat][i][1])
                self.elements['admin']['itemListBox'].AppendItem(Item(item.GetItemName()))
                self.adminItemList[cat].append(constInfo.ItemShop['ITEMS'][('itemshop','drs_shop','3rd_shop')[self.curShop]][cat][i])
            self.elements['admin']['itemListBox'].SetSelectEvent(self.SelectAdminDicountItem)
        except:
            pass
        if len(self.adminItemList[cat]) > 10:
            self.elements['admin']['itemListBoxScroll'].SetMiddleBarSize(float(10)/float(len(self.adminItemList[cat])))
            self.elements['admin']['itemListBoxScroll'].Show()
        else:
            self.elements['admin']['itemListBoxScroll'].Hide()
            
    def SelectAdminDicountItem(self, selectItem):
        itemInfo = constInfo.ItemShop['ITEMS'][('itemshop','drs_shop','3rd_shop')[self.curShop]][self.category['admin']][self.elements['admin']['itemListBox'].GetItemIndex(selectItem)]
        self.elements['admin']['discountBox'].SetShop(self.curShop)
        self.elements['admin']['discountBox'].SetContent(itemInfo[0], itemInfo[1], itemInfo[3], itemInfo[4])
        self.elements['admin']['discountBox'].Show()
        self.elements['admin']['categoryDiscountBox'].SetPosition(320, 80+180+15)
        
    def SwitchBanner(self, newBanner):
        self.bannerVar['lastSwitch'] = time.clock() + self.bannerOptions['time'] + self.bannerOptions['timeToFade']/self.bannerOptions['interval']
        self.elements['home']['banner']['banner_1'].LoadImage(self.bannerOptions['folder'] + self.bannerOptions['banner_%d' % self.bannerVar['currentImage']] + '.tga')
        self.elements['home']['banner']['banner_1'].Show()
        self.elements['home']['banner']['banner_0'].LoadImage(self.bannerOptions['folder'] + self.bannerOptions['banner_%d' % newBanner]+ '.tga')
        self.bannerVar['currentImage'] = newBanner
        self.SetAlpha(self.elements['home']['banner']['banner_1'], 1.0)
        self.bannerVar['fadeOut'] = 1
        self.bannerVar['intervallEndTime'] = self.bannerVar['currentTime'] + self.bannerOptions['timeToFade']
        if newBanner == 0:
            self.elements['home']['banner']['change_1'].SetUp()
            self.elements['home']['banner']['change_0'].Down()
        elif newBanner == 1:
            self.elements['home']['banner']['change_0'].SetUp()
            self.elements['home']['banner']['change_1'].Down()
            
    def SetAlpha(self, image, alpha):
        self.bannerVar['currentAlphaValue'] = alpha
        image.SetAlpha(alpha)
        
    def CreateWheelPrizesTable(self):
        self.elements['wheel']['items'] = []
        self.elements['wheel']['best_items'] = []
        for i in xrange(14):
            self.elements['wheel']['items'].append(constInfo.ItemShop['WOD'][0][app.GetRandom(0, len(constInfo.ItemShop['WOD'][0])-1)])
        for i in xrange(2):
            self.elements['wheel']['items'].append(constInfo.ItemShop['WOD'][1][app.GetRandom(0, len(constInfo.ItemShop['WOD'][1])-1)])
        for i in xrange(6):
            self.elements['wheel']['best_items'].append(constInfo.ItemShop['WOD'][1][app.GetRandom(0, len(constInfo.ItemShop['WOD'][1])-1)])

    def SelectPage(self, pageName):
        for tab in self.tab:
            tab.SetUp()
        for page in self.page:
            page.Hide()
        self.GetChild("SearchValue").KillFocus()
        
        Idx = 0
        if pageName == 'HOME':
            Idx = 0
            self.arrows['home']['mostBought'] = 1
            self.arrows['home']['hotOffers'] = 1
            self.RefreshIShopMostBought()
            self.RefreshIShopHotOffers()
            self.SwitchBanner(0)
        elif pageName == 'ITEMSHOP':
            Idx = 1
            self.category['itemshop'] = self.categorys['itemshop'][0][0]
            self.arrows['itemshop']['items'] = 1
            self.arrows['itemshop']['categorys'] = 0
            self.isSearching = False
            self.RefreshIShopCategorys()
            self.RefreshIShopItems()
        elif pageName == 'DRS_SHOP':
            Idx = 2
            self.category['drs_shop'] = self.categorys['drs_shop'][0][0]
            self.arrows['drs_shop']['items'] = 1
            self.arrows['drs_shop']['categorys'] = 0
            self.isSearching = False
            self.RefreshDrsShopCategorys()
            self.RefreshDrsShopItems()
        elif pageName == '3RD_SHOP':
            Idx = 3
            self.category['3rd_shop'] = self.categorys['3rd_shop'][0][0]
            self.arrows['3rd_shop']['items'] = 1
            self.arrows['3rd_shop']['categorys'] = 0
            self.isSearching = False
            self.Refresh3rdShopCategorys()
            self.Refresh3rdShopItems()
        elif pageName == 'WHEEL':
            Idx = 4
            self.RefreshWheelOfDestiny()
        elif pageName == 'LOGS':
            Idx = 5
            self.RefreshLogs()
        elif pageName == 'GM_ZONE':
            Idx = 6
    
        self.__OnClickAdminButton('NONE')
        self.curPage = pageName
        self.tab[Idx].Down()
        self.page[Idx].Show()
        
    def RefreshWindow(self):
        if self.curPage == 'GM_ZONE':
            self.__OnClickAdminButton(self.curAdminPage)
            return
        if self.curPage == 'ITEMSHOP':
            if self.isSearching:
                maxPage = math.ceil(float(len(self.searchList))/float(9))
            else:
                maxPage = math.ceil(float(len(constInfo.ItemShop['ITEMS']['itemshop'][self.category['itemshop']]))/float(9))
            if self.arrows['itemshop']['items'] > maxPage:
                self.arrows['itemshop']['items'] -= 1
            self.RefreshIShopItems()
            return
        elif self.curPage == 'DRS_SHOP':
            if self.isSearching:
                maxPage = math.ceil(float(len(self.searchList))/float(9))
            else:
                maxPage = math.ceil(float(len(constInfo.ItemShop['ITEMS']['drs_shop'][self.category['drs_shop']]))/float(9))
            if self.arrows['drs_shop']['items'] > maxPage:
                self.arrows['drs_shop']['items'] -= 1
            self.RefreshDrsShopItems()
            return
        elif self.curPage == '3RD_SHOP':
            if self.isSearching:
                maxPage = math.ceil(float(len(self.searchList))/float(9))
            else:
                maxPage = math.ceil(float(len(constInfo.ItemShop['ITEMS']['3rd_shop'][self.category['3rd_shop']]))/float(9))
            if self.arrows['3rd_shop']['items'] > maxPage:
                self.arrows['3rd_shop']['items'] -= 1
            self.Refresh3rdShopItems()
            return
        self.SelectPage(self.curPage)
        
    def __OnClickAdminButton(self, admin):
        self.page[6].Hide()
        self.elements['admin']['shop_type'].CloseListBox()
        if admin == 'ITEM_TIME':
            self.elements['admin']['shop_type'].SelectItem(0)
            self.elements['admin']['discountBox'].Hide()
            self.elements['admin']['categoryDiscountBox'].SetPosition(320, 80)
            self.elements['admin']['categoryDiscountBox'].Hide()
            self.elements['admin']['addItemTimeWindow'].Show()
            self.elements['admin']['buttons']['return'].Show()
            self.elements['admin']['shop_type'].SetParent(self.GetChild("AddItemTimeWindow"))
            self.elements['admin']['shop_type'].SetPosition(150, 16)
        elif admin == 'ADD_ITEM':
            self.__OnClickResetAddNewItem()
            self.elements['admin']['shop_type'].SetParent(self.GetChild("AddItemNewBoard"))
            self.elements['admin']['shop_type'].SetPosition(70, 2+28*4)
            self.elements['admin']['addItemWindow'].Show()
            self.elements['admin']['buttons']['return'].Show()
        elif admin == 'NONE':
            self.elements['admin']['addItemTimeWindow'].Hide()
            self.elements['admin']['addItemWindow'].Hide()
            self.elements['admin']['buttons']['return'].Hide()
        self.curAdminPage = admin
        
    def __OnClickArrow(self, arrow):
        if arrow == 'ITEMSHOP_CATEGORYS_UP':
            self.arrows['itemshop']['categorys'] -= 1
            self.RefreshIShopCategorys()
        elif arrow == 'ITEMSHOP_CATEGORYS_DOWN':
            self.arrows['itemshop']['categorys'] += 1
            self.RefreshIShopCategorys()
        elif arrow == 'ITEMSHOP_ITEMS_LEFT':
            self.arrows['itemshop']['items'] -= 1
            self.RefreshIShopItems()
        elif arrow == 'ITEMSHOP_ITEMS_RIGHT':
            self.arrows['itemshop']['items'] += 1
            self.RefreshIShopItems()
        elif arrow == 'DRSSHOP_CATEGORYS_UP':
            self.arrows['drs_shop']['categorys'] -= 1
            self.RefreshDrsShopCategorys()
        elif arrow == 'DRSSHOP_CATEGORYS_DOWN':
            self.arrows['drs_shop']['categorys'] += 1
            self.RefreshDrsShopCategorys()
        elif arrow == 'DRSSHOP_ITEMS_LEFT':
            self.arrows['drs_shop']['items'] -= 1
            self.RefreshDrsShopItems()
        elif arrow == 'DRSSHOP_ITEMS_RIGHT':
            self.arrows['drs_shop']['items'] += 1
            self.RefreshDrsShopItems()
        elif arrow == '3RDSHOP_CATEGORYS_UP':
            self.arrows['3rd_shop']['categorys'] -= 1
            self.Refresh3rdShopCategorys()
        elif arrow == '3RDSHOP_CATEGORYS_DOWN':
            self.arrows['3rd_shop']['categorys'] += 1
            self.Refresh3rdShopCategorys()
        elif arrow == '3RDSHOP_ITEMS_LEFT':
            self.arrows['3rd_shop']['items'] -= 1
            self.Refresh3rdShopItems()
        elif arrow == '3RDSHOP_ITEMS_RIGHT':
            self.arrows['3rd_shop']['items'] += 1
            self.Refresh3rdShopItems()
        elif arrow == 'HOME_MOSTBOUGHT_LEFT':
            self.arrows['home']['mostBought'] -= 1
            self.RefreshIShopMostBought()
        elif arrow == 'HOME_MOSTBOUGHT_RIGHT':
            self.arrows['home']['mostBought'] += 1
            self.RefreshIShopMostBought()
        elif arrow == 'HOME_HOTOFFERS_DOWN':
            self.arrows['home']['hotOffers'] += 1
            self.RefreshIShopHotOffers()
        elif arrow == 'HOME_HOTOFFERS_UP':
            self.arrows['home']['hotOffers'] -= 1
            self.RefreshIShopHotOffers()
        elif arrow == 'LOGS_LEFT':
            self.arrows['logs'] -= 1
            self.RefreshLogs()
        elif arrow == 'LOGS_RIGHT':
            self.arrows['logs'] += 1
            self.RefreshLogs()
            
    def __OnClickUpdateCoins(self):
        constInfo.ItemShop['QCMD'] = 'UPDATE_COINS#'
        event.QuestButtonClick(constInfo.ItemShop['QID'])
            
    def __OnClickAddNewItem(self):
        attributes,sockets = "",""
        for i in xrange(7):
            attributes = attributes+self.elements['admin']['bonusBoxes']['attrtype%d' % i].GetText()+'#'+self.elements['admin']['bonusBoxes']['attrvalue%d' % i].GetText()+'#'
        for i in xrange(3):
            sockets = sockets+self.elements['admin']['socketsBoxes']['socket%d' % i].GetText()+'#'
        if self.elements['admin']['vnumAddItem'].GetText() == '0' or self.elements['admin']['vnumAddItem'].GetText() == '' or self.elements['admin']['priceAddItem'].GetText() == '0' or self.elements['admin']['priceAddItem'].GetText() == '':
            return
        constInfo.ItemShop['QCMD'] = 'ADD_ITEM#%s#%s#%s#%s#%s#%d' % (self.adminCategory, self.elements['admin']['vnumAddItem'].GetText(), attributes[:-1], sockets[:-1], self.elements['admin']['priceAddItem'].GetText(), self.curShop)
        event.QuestButtonClick(constInfo.ItemShop['QID'])
        
    def __OnClickResetAddNewItem(self):
        for i in xrange(7):
            self.elements['admin']['bonusBoxes']['attrtype%d' % i].SetText('0')
            self.elements['admin']['bonusBoxes']['attrvalue%d' % i].SetText('0')
        for i in xrange(3):
            self.elements['admin']['socketsBoxes']['socket%d' % i].SetText('0')
        self.elements['admin']['vnumAddItem'].SetText('')
        self.elements['admin']['vnumAddItem'].SetFocus()
        self.elements['admin']['priceAddItem'].SetText('0')
        self.elements['admin']['iconAddItem'].Hide()
        self.elements['admin']['shop_type'].SelectItem(0)
        self.categoryAdmin.SelectItem(0)
        
    def __OnClickSearchButton(self):
        if self.curPage == 'HOME' or self.curPage == 'WHEEL' or self.curPage == 'LOGS' or self.curPage == 'GM_ZONE':
            self.SelectPage('ITEMSHOP')
        else:
            self.SelectPage(self.curPage)
        self.searchList = []
        shop = self.curPage.lower()
        item_name = self.GetChild("SearchValue").GetText()
        if item_name != "" and item_name != localeInfo.ITEMSHOP_SEARCH_TEXT:
            self.isSearching = True
            for cat in constInfo.ItemShop['ITEMS'][shop]:
                for i in xrange(len(constInfo.ItemShop['ITEMS'][shop][cat])):
                    item.SelectItem(constInfo.ItemShop['ITEMS'][shop][cat][i][1])
                    if (item_name in item.GetItemName()) or (item_name in item.GetItemName().lower()):
                        self.searchList.append(constInfo.ItemShop['ITEMS'][shop][cat][i])
            if self.curPage == 'ITEMSHOP':
                self.RefreshIShopItems()
            elif self.curPage == 'DRS_SHOP':
                self.RefreshDrsShopItems()
            elif self.curPage == '3RD_SHOP':
                self.Refresh3rdShopItems()
        
    def __OnClickSelectCategoryIShop(self, catIdx):
        self.isSearching = False
        self.category['itemshop'] = catIdx
        self.arrows['itemshop']['items'] = 1
        self.RefreshIShopItems()
    
    def __OnClickSelectCategoryDrsShop(self, catIdx):
        self.isSearching = False
        self.category['drs_shop'] = catIdx
        self.arrows['drs_shop']['items'] = 1
        self.RefreshDrsShopItems()
        
    def __OnClickSelectCategory3rdShop(self, catIdx):
        self.isSearching = False
        self.category['3rd_shop'] = catIdx
        self.arrows['3rd_shop']['items'] = 1
        self.Refresh3rdShopItems()
        
    def __OnClickRequestSpinWheel(self):
        if self.wheelStartSpin: return
        self.CreateWheelPrizesTable()
        constInfo.ItemShop['QCMD'] = 'REQUEST_SPIN_WHEEL#'
        event.QuestButtonClick(constInfo.ItemShop['QID'])
        
    def SpinWheel(self):
        if self.wheelStartSpin: return
        self.RefreshWheelOfDestiny()
        self.wheelStartSpin = True
        self.wheelRandomNumber = app.GetRandom(0, 15)
        self.wheelRotationLimit = self.wheelRandomNumber*22.5+5*360
        self.wheelRotationSpeed = 5
        
        for i in xrange(16):
            item.SelectItem(self.elements['wheel']['items'][i])
            self.elements['wheel']['objects']['icon_%d' % i].SetParent(self.elements['wheel']['objects']['int_%d' % i])
            self.elements['wheel']['objects']['icon_%d' % i].LoadImage(item.GetIconImageFileName())
            self.elements['wheel']['objects']['icon_%d' % i].Show()
            self.elements['wheel']['objects']['tooltip_%d' % i].ClearToolTip()
            if item.GetLimit(0)[0] == 7:
                self.elements['wheel']['objects']['tooltip_%d' % i].AddItemData(self.elements['wheel']['items'][i], [item.GetLimit(0)[1]+app.GetGlobalTimeStamp(),0,0])
            else:
                self.elements['wheel']['objects']['tooltip_%d' % i].AddItemData(self.elements['wheel']['items'][i], [0,0,0])    
            self.elements['wheel']['objects']['tooltip_%d' % i].HideToolTip()

        for i in xrange(6):
            item.SelectItem(self.elements['wheel']['best_items'][i])
            self.elements['wheel']['objects']['iconbests_%d' % i].LoadImage(item.GetIconImageFileName())
            self.elements['wheel']['objects']['namebests_%d' % i].SetText(item.GetItemName())
            self.elements['wheel']['objects']['tooltipbests_%d' % i].ClearToolTip()
            if item.GetLimit(0)[0] == 7:
                self.elements['wheel']['objects']['tooltipbests_%d' % i].AddItemData(self.elements['wheel']['best_items'][i], [item.GetLimit(0)[1]+app.GetGlobalTimeStamp(),0,0])
            else:
                self.elements['wheel']['objects']['tooltipbests_%d' % i].AddItemData(self.elements['wheel']['best_items'][i], [0,0,0])                
            self.elements['wheel']['objects']['tooltipbests_%d' % i].HideToolTip()
            self.elements['wheel']['objects']['bgbar_%d' % i].SetSize(3+50+self.elements['wheel']['objects']['namebests_%d' % i].GetTextSize()[0], 64)
            self.elements['wheel']['objects']['bgbar_%d' % i].SetPosition(0, 66*i)
            self.elements['wheel']['objects']['bgbar_%d' % i].SetWindowHorizontalAlignCenter()
            self.elements['wheel']['objects']['bgbar_%d' % i].Show()
        
    def SetRotationWheel(self):
        if self.wheelRotationLimit <= (360*4) and self.wheelRotationLimit > (3*360) and self.wheelRotationSpeed != 4:
            self.wheelRotationSpeed = 4
        elif self.wheelRotationLimit <= (360*3) and self.wheelRotationLimit > (2*360) and self.wheelRotationSpeed != 3:
            self.wheelRotationSpeed = 3
        elif self.wheelRotationLimit <= (360*2) and self.wheelRotationLimit > (200) and self.wheelRotationSpeed != 2:
            self.wheelRotationSpeed = 2
        elif self.wheelRotationLimit <= 200 and self.wheelRotationLimit > 0 and self.wheelRotationSpeed != 1:
            self.wheelRotationSpeed = 1
        elif self.wheelRotationLimit <= 0:
            constInfo.ItemShop['QCMD'] = 'REQUEST_PRIZE_WHEEL#%d' % self.elements['wheel']['items'][self.wheelRandomNumber]
            event.QuestButtonClick(constInfo.ItemShop['QID'])
            self.wheelStartSpin = False
            self.wheelCooldown = time.clock() + 5
            return
        self.wheelRotationLimit -= self.wheelRotationSpeed
        self.wheelRotationCurrent += self.wheelRotationSpeed
        self.elements['wheel']['images']['rotation'].SetRotation(self.wheelRotationCurrent)
        
    def __del__(self):
        ui.ScriptWindow.__del__(self)
        
    def EnableGMZone(self):
        self.tab[6].Show()
        self.isGM = True
        
    def Open(self, mode):
        if not self.isLoaded:
            self.__LoadWindow()
        
        self.isGM = False
        if mode == 1:
            self.EnableGMZone()
        self.SelectPage('HOME')
        self.SwitchBanner(0)
        self.SetCenterPosition()
        self.SetTop()
        self.Show()
        
    def Close(self):
        constInfo.ItemShop['QCMD'] = 'CLOSE_SHOP#'
        event.QuestButtonClick(constInfo.ItemShop['QID'])
        self.Hide()
        
    def Destroy(self):
        self.Close()
        self.ClearDictionary()
        
    def OnPressEscapeKey(self):
        self.Close()
        return TRUE
        
    def OnPressExitKey(self):
        self.Close()
        return TRUE
        
    def OnUpdate(self):        
        if self.wheelStartSpin:
            self.SetRotationWheel()
            
        if self.wheelCooldown:
            if self.wheelCooldown < time.clock():
                self.RefreshWheelOfDestiny()
                self.wheelCooldown = None
                
        if self.elements['admin']['vnumAddItem'].GetText() != '0' and self.elements['admin']['vnumAddItem'].GetText() != '':
            if self.adminTmpAddVnum != int(self.elements['admin']['vnumAddItem'].GetText()):
                try:
                    self.adminTmpAddVnum = int(self.elements['admin']['vnumAddItem'].GetText())
                    item.SelectItem(self.adminTmpAddVnum)
                    self.elements['admin']['iconAddItem'].LoadImage(item.GetIconImageFileName())
                    self.elements['admin']['iconAddItem'].Show()
                    self.adminAddNewItemToolTip.ClearToolTip()
                    self.adminAddNewItemToolTip.AddItemData(self.adminTmpAddVnum, [0, 0, 0])
                    self.adminAddNewItemToolTip.HideToolTip()
                except:
                    self.elements['admin']['iconAddItem'].Hide()
        
        self.elements['money']['coins'].SetText(localeInfo.NumberToMoneyString(constInfo.COINS_DRS[0]).replace('Yang', localeInfo.ITEMSHOP_ISHOP_COIN))
        self.elements['money']['drs'].SetText(localeInfo.NumberToMoneyString(constInfo.COINS_DRS[1]).replace('Yang', localeInfo.ITEMSHOP_DRSSHOP_COIN))
        
        if self.bannerVar['lastSwitch'] < time.clock():
            if self.bannerVar['currentImage'] == 0:
                self.SwitchBanner(1)
            else:
                self.SwitchBanner(0)
        
        if self.bannerVar['fadeOut'] == 1:
            self.bannerVar['currentTime'] = time.clock()
        
        if self.bannerVar['currentAlphaValue'] > 0.0:
            if self.bannerVar['currentTime'] >= self.bannerVar['intervallEndTime']:
                newAlphaValue = self.bannerVar['currentAlphaValue'] 
                newAlphaValue -= self.bannerOptions['interval']
                self.SetAlpha(self.elements['home']['banner']['banner_1'], newAlphaValue)
                self.bannerVar['intervallEndTime'] = self.bannerVar['currentTime'] + self.bannerOptions['timeToFade']
        else:
            self.bannerVar['fadeOut'] = 0
            self.elements['home']['banner']['banner_1'].Hide()

class ItemBox(ui.Window):

    def __init__(self):
        ui.Window.__init__(self)
        self.isLoaded = FALSE
        
        self.priceValues = [localeInfo.ITEMSHOP_ISHOP_COIN, localeInfo.ITEMSHOP_DRSSHOP_COIN, localeInfo.ITEMSHOP_3RDSHOP_COIN]
        self.priceValue = self.priceValues[0]
        self.time = None
        self.runOut = None
        self.price = None
        self.itemData = [0, [], []]
        self.lastTime = None
        self.id = -1
        
    def __LoadObjects(self):
        self.SetSize(192, 107+18)
        
        Board = ui.ResizableTextValue()
        Board.SetParent(self)
        Board.SetSize(192, 76+31)
        Board.SetPosition(0, 18)
        Board.SetLine('left')
        Board.SetLine('right')
        Board.SetLine('top')
        Board.SetLine('bottom')
        Board.Show()
        self.Board = Board
        
        TimerBoard = ui.ResizableTextValue()
        TimerBoard.SetParent(self)
        TimerBoard.SetSize(85, 18)
        TimerBoard.SetPosition(22, 0)
        TimerBoard.SetLine('left')
        TimerBoard.SetLine('right')
        TimerBoard.SetLine('top')
        TimerBoard.SetText('10h 10m 10s')
        TimerBoard.Hide()
        self.TimerBoard = TimerBoard
        
        PercentBoard = ui.ResizableTextValue()
        PercentBoard.SetParent(self)
        PercentBoard.SetSize(40, 18)
        PercentBoard.SetPosition(137, 0)
        PercentBoard.SetLine('left')
        PercentBoard.SetLine('right')
        PercentBoard.SetLine('top')
        PercentBoard.SetText('50%')
        PercentBoard.Hide()
        self.PercentBoard = PercentBoard
        
        ItemName = ui.ResizableTextValue()
        ItemName.SetParent(self.Board)
        ItemName.SetSize(150, 18)
        ItemName.SetPosition(41, 1)
        ItemName.SetLine('left')
        ItemName.SetLine('bottom')
        ItemName.Show()
        self.ItemName = ItemName
        
        ItemPrice = ui.ResizableTextValue()
        ItemPrice.SetParent(self.Board)
        ItemPrice.SetSize(85, 18)
        ItemPrice.SetPosition(106, 19+1)
        ItemPrice.SetLine('left')
        ItemPrice.SetLine('bottom')
        ItemPrice.Show()
        self.ItemPrice = ItemPrice
        
        ItemAmountSlot = ui.ResizableTextValue()
        ItemAmountSlot.SetParent(self.Board)
        ItemAmountSlot.SetSize(85, 18)
        ItemAmountSlot.SetPosition(106, 1+19*2)
        ItemAmountSlot.SetLine('left')
        ItemAmountSlot.SetLine('bottom')
        ItemAmountSlot.Show()
        self.ItemAmountSlot = ItemAmountSlot
        
        ItemAmountTxt = ui.TextLine()
        ItemAmountTxt.SetParent(self.ItemAmountSlot)
        ItemAmountTxt.SetPosition(11, 0)
        ItemAmountTxt.SetWindowHorizontalAlignCenter()
        ItemAmountTxt.SetHorizontalAlignCenter()
        ItemAmountTxt.SetWindowVerticalAlignCenter()
        ItemAmountTxt.SetVerticalAlignCenter()
        ItemAmountTxt.SetText(localeInfo.ITEMSHOP_ITEM_AMOUNT)
        ItemAmountTxt.SetOutline()
        ItemAmountTxt.Show()
        self.ItemAmountTxt = ItemAmountTxt
        
        ItemAmount = ui.EditLine()
        ItemAmount.SetParent(self.ItemAmountSlot)
        ItemAmount.SetSize(10, 18)
        ItemAmount.SetPosition((self.ItemAmountSlot.GetWidth()/2)-13, 3)
        ItemAmount.SetMax(3)
        ItemAmount.SetNumberMode()
        ItemAmount.SetText('1')
        ItemAmount.SetOutline()
        ItemAmount.Show()
        self.ItemAmount = ItemAmount
        
        BuyButton = ui.CoolButton()
        BuyButton.SetParent(self.Board)
        BuyButton.SetSize(75, 12*2)
        BuyButton.SetPosition(111, 17+19*3)
        BuyButton.SetText(localeInfo.ITEMSHOP_BUY_BUTTON)
        BuyButton.SetEvent(ui.__mem_func__(self.__OnClickBuyButton))
        BuyButton.Show()
        self.BuyButton = BuyButton
        
        DeleteButton = ui.CoolButton()
        DeleteButton.SetParent(self.Board)
        DeleteButton.SetSize(19, 19)
        DeleteButton.SetPosition(0, 0)
        DeleteButton.SetText('X')
        DeleteButton.SetToolTipText(localeInfo.ITEMSHOP_DELETE_ITEM_TOOLTIP_BUTTON)
        DeleteButton.BACKGROUND_COLOR = grp.GenerateColor(1.0, 0.2, 0.2, 0.4)
        DeleteButton.SetEvent(ui.__mem_func__(self.__OnClickDeleteButton))
        DeleteButton.Hide()
        self.DeleteButton = DeleteButton
        
        self.DeleteQuestion = uiCommon.QuestionDialog()
        self.DeleteQuestion.SetText(localeInfo.ITEMSHOP_DELETE_ITEM_QUESTION)
        self.DeleteQuestion.SetAcceptEvent(lambda arg = TRUE: self.AnswerDeleteItem(arg))
        self.DeleteQuestion.SetCancelEvent(lambda arg = FALSE: self.AnswerDeleteItem(arg))
        
        ChangePriceButton = ui.CoolButton()
        ChangePriceButton.SetParent(self.Board)
        ChangePriceButton.SetSize(19, 19)
        ChangePriceButton.SetPosition(87, 19)
        ChangePriceButton.SetText('!')
        ChangePriceButton.SetToolTipText(localeInfo.ITEMSHOP_CHANGE_PRICE_TOOLTIP_BUTTON)
        ChangePriceButton.BACKGROUND_COLOR = grp.GenerateColor(1.0, 0.2, 0.2, 0.4)
        ChangePriceButton.SetEvent(ui.__mem_func__(self.__OpenChangePriceWindow))
        ChangePriceButton.Hide()
        self.ChangePriceButton = ChangePriceButton
        
        ToolTip = uiToolTip.ItemToolTip()
        ToolTip.HideToolTip()
        self.ToolTip = ToolTip
        
        ItemIcon = ui.ExpandedImageBox()
        ItemIcon.SetParent(self.Board)
        try:
            ItemIcon.SetMouseOverInEvent(self.ToolTip.ShowToolTip)
            ItemIcon.SetMouseOverOutEvent(self.ToolTip.HideToolTip)
        except:
            ItemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN", self.ToolTip.ShowToolTip)
            ItemIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT", self.ToolTip.HideToolTip)
        ItemIcon.Show()
        self.ItemIcon = ItemIcon
        
        self.BuyQuestion = uiCommon.QuestionDialog()
        self.BuyQuestion.SetAcceptEvent(lambda arg = TRUE: self.AnswerBuyItem(arg))
        self.BuyQuestion.SetCancelEvent(lambda arg = FALSE: self.AnswerBuyItem(arg))
        
        ChangePriceWindow = ui.ResizableTextValue()
        ChangePriceWindow.SetParent(self.Board)
        ChangePriceWindow.SetSize(130, 75)
        ChangePriceWindow.SetWindowHorizontalAlignCenter()
        ChangePriceWindow.SetWindowVerticalAlignCenter()
        ChangePriceWindow.SetLine('left')
        ChangePriceWindow.SetLine('right')
        ChangePriceWindow.SetLine('top')
        ChangePriceWindow.SetLine('bottom')
        ChangePriceWindow.Hide()
        self.ChangePriceWindow = ChangePriceWindow
        
        ChangePriceTitle = ui.TextLine()
        ChangePriceTitle.SetParent(self.ChangePriceWindow)
        ChangePriceTitle.SetPosition(0, 4)
        ChangePriceTitle.SetWindowHorizontalAlignCenter()
        ChangePriceTitle.SetHorizontalAlignCenter()
        ChangePriceTitle.SetText(localeInfo.ITEMSHOP_NEW_PRICE_TITLE)
        ChangePriceTitle.Show()
        self.ChangePriceTitle = ChangePriceTitle
        
        self.ChangePriceSlot = ui.MakeSlotBar(self.ChangePriceWindow, 0, 4+20, 100, 18)
        self.ChangePriceSlot.SetWindowHorizontalAlignCenter()
        ChangePriceValue = ui.EditLine()
        ChangePriceValue.SetParent(self.ChangePriceSlot)
        ChangePriceValue.SetSize(100-3, 18-3)
        ChangePriceValue.SetPosition(3, 3)
        ChangePriceValue.SetMax(5)
        ChangePriceValue.SetNumberMode()
        ChangePriceValue.SetText('1')
        ChangePriceValue.SetOutline()
        ChangePriceValue.SetEscapeEvent(ui.__mem_func__(self.ChangePriceWindow.Hide))
        ChangePriceValue.SetReturnEvent(ui.__mem_func__(self.__OnClickChangePrice))
        ChangePriceValue.Show()
        self.ChangePriceValue = ChangePriceValue
        
        ChangePriceOK = ui.CoolButton()
        ChangePriceOK.SetParent(self.ChangePriceWindow)
        ChangePriceOK.SetSize(50, 18)
        ChangePriceOK.SetPosition(15, 4+20+25)
        ChangePriceOK.SetText(localeInfo.ITEMSHOP_ACCEPT_CHANGE_PRICE)
        ChangePriceOK.BACKGROUND_COLOR = grp.GenerateColor(0.2, 1.0, 0.2, 0.4)
        ChangePriceOK.SetEvent(ui.__mem_func__(self.__OnClickChangePrice))
        ChangePriceOK.Show()
        self.ChangePriceOK = ChangePriceOK
        
        ChangePriceCancel = ui.CoolButton()
        ChangePriceCancel.SetParent(self.ChangePriceWindow)
        ChangePriceCancel.SetSize(50, 18)
        ChangePriceCancel.SetPosition(15+50, 4+20+25)
        ChangePriceCancel.SetText(localeInfo.ITEMSHOP_CANCEL_CHANGE_PRICE)
        ChangePriceCancel.BACKGROUND_COLOR = grp.GenerateColor(1.0, 0.2, 0.2, 0.4)
        ChangePriceCancel.SetEvent(ui.__mem_func__(self.ChangePriceWindow.Hide))
        ChangePriceCancel.Show()
        self.ChangePriceCancel = ChangePriceCancel
        
        ChangePriceQuestion = uiCommon.QuestionDialog()
        ChangePriceQuestion.SetText(localeInfo.ITEMSHOP_CHANGE_PRICE_QUESTION)
        ChangePriceQuestion.SetAcceptEvent(lambda arg = TRUE : self.AnswerChangePrice(arg))
        ChangePriceQuestion.SetCancelEvent(lambda arg = TRUE : self.AnswerChangePrice(arg))
        self.ChangePriceQuestion = ChangePriceQuestion
        
        self.SetSize(self.Board.GetWidth(), self.Board.GetHeight()+(self.TimerBoard.GetHeight()-8))
        self.isLoaded = TRUE
        
    def __del__(self):
        ui.Window.__del__(self)
        
    def SetGM(self, isGM):
        if isGM:
            self.DeleteButton.Show()
            self.ChangePriceButton.Show()
        else:
            self.DeleteButton.Hide()
            self.ChangePriceButton.Hide()
        
    def SetCoin(self, coin):
        self.priceValue = self.priceValues[coin-1]
        
    def SetContent(self, id, vnum, attrs, sockets, price):
        item.SelectItem(vnum)
        self.id = id
        self.price = price
        
        if item.IsFlag(4) == 1:
            self.ItemAmountSlot.Show()
        else:
            self.ItemAmountSlot.Hide()
        self.ItemName.SetText(item.GetItemName())
        self.ItemIcon.LoadImage(item.GetIconImageFileName())
        self.ItemIcon.SetPosition(40, (49,35,28)[item.GetItemSize()[1]-1])
        self.ItemIcon.SetScale(1, (1,1,0.8)[item.GetItemSize()[1]-1])
        self.ItemPrice.SetText(localeInfo.ITEMSHOP_PRICE_TITLE_BOX % (price, self.priceValue))
        self.ItemAmount.SetText('1')
        self.ItemAmount.KillFocus()
        self.ItemAmount.CanEdit(TRUE)
        self.BuyButton.Enable()
        
        attrs += [(0,0)]*(player.ATTRIBUTE_SLOT_MAX_NUM-7)
        self.itemData = [vnum, attrs, sockets]
        
        self.ToolTip.ClearToolTip()
        if item.GetLimit(0)[0] == 7:
            self.ToolTip.AddItemData(vnum, [sockets[0]+app.GetGlobalTimeStamp(), sockets[1], sockets[2]], attrs)
        else:
            self.ToolTip.AddItemData(vnum, sockets, attrs)
        self.ToolTip.HideToolTip()
        
    def SetTime(self, time, runOut):
        if time <= 0:
            self.TimerBoard.Hide()
            self.time = None
            return
        else:
            self.TimerBoard.Show()
            self.time = time
            self.runOut = runOut
            self.lastTime = 0
            
    def SetPercent(self, percent):
        if percent <= 0:
            self.PercentBoard.Hide()
        else:
            self.PercentBoard.SetText('%d%%' % percent)
            self.PercentBoard.Show()
            self.ItemPrice.SetText(localeInfo.ITEMSHOP_PRICE_TITLE_BOX % (math.ceil(self.price-(self.price/100.00)*percent), self.priceValue))
            
    def __OnClickDeleteButton(self):
        self.DeleteQuestion.Open()
        
    def AnswerDeleteItem(self, arg):
        if arg:
            shop = ''
            if self.priceValue == self.priceValues[0]:
                shop = 'itemshop'
            elif self.priceValue == self.priceValues[1]:
                shop = 'drs_shop'
            elif self.priceValue == self.priceValues[2]:
                shop = '3rd_shop'
            constInfo.ItemShop['QCMD'] = 'DELETE_ITEM#%s#%d' % (shop, self.id)
            event.QuestButtonClick(constInfo.ItemShop['QID'])
        self.DeleteQuestion.Close()
        
    def __OpenChangePriceWindow(self):
        self.ChangePriceValue.SetText('1')
        self.ChangePriceValue.SetFocus()
        self.ChangePriceWindow.Show()
        
    def __OnClickChangePrice(self):
        if len(self.ChangePriceValue.GetText()) == 0:
            return
        if int(self.ChangePriceValue.GetText()) == 0:
            return
        self.ChangePriceQuestion.Open()
        
    def AnswerChangePrice(self, arg):
        if arg:
            shop = ''
            if self.priceValue == self.priceValues[0]:
                shop = 'itemshop'
            elif self.priceValue == self.priceValues[1]:
                shop = 'drs_shop'
            elif self.priceValue == self.priceValues[2]:
                shop = '3rd_shop'
            constInfo.ItemShop['QCMD'] = 'CHANGE_PRICE#%s#%d#%s' % (shop, self.id, self.ChangePriceValue.GetText())
            event.QuestButtonClick(constInfo.ItemShop['QID'])
            self.ChangePriceWindow.Hide()
        self.ChangePriceQuestion.Close()
            
    def __OnClickBuyButton(self):
        self.ItemAmount.KillFocus()
        amount = self.ItemAmount.GetText()
        price = int(amount) * int(self.ItemPrice.GetText().split(' ')[1])
        if self.priceValue not in self.priceValues:
            self.priceValue = self.priceValues[0]

        if amount == '1':
            self.BuyQuestion.SetText(localeInfo.ITEMSHOP_BUY_ITEM_WITHOUT_AMOUNT_QUESTION % (self.ItemName.GetText(), price, self.priceValue))
        else:
            self.BuyQuestion.SetText(localeInfo.ITEMSHOP_BUY_ITEM_WITH_AMOUNT_QUESTION % (amount, self.ItemName.GetText(), price, self.priceValue))
        self.BuyQuestion.Open()
        
    def AnswerBuyItem(self, arg):
        if arg:
            shop = ''
            if self.priceValue == self.priceValues[0]:
                shop = 'itemshop'
            elif self.priceValue == self.priceValues[1]:
                shop = 'drs_shop'
            elif self.priceValue == self.priceValues[2]:
                shop = '3rd_shop'
            constInfo.ItemShop['QCMD'] = 'BUY_ITEM#%s#%d#%s' % (shop, self.id, self.ItemAmount.GetText())
            event.QuestButtonClick(constInfo.ItemShop['QID'])
        self.BuyQuestion.Close()
    
    def OnUpdate(self):
        if not self.ItemAmount.IsFocus() and (self.ItemAmount.GetText() == '' or self.ItemAmount.GetText() == '0'):
            self.ItemAmount.SetText('1')
        if int(self.ItemAmount.GetText()) > ITEM_MAX_COUNT:
            self.ItemAmount.SetText('%d' % ITEM_MAX_COUNT)
        elif int(self.ItemAmount.GetText()) < 1:
            self.ItemAmount.SetText('1')

        if self.time:
            remaining = self.time - app.GetGlobalTimeStamp()
            if self.lastTime < time.clock():
                if remaining <= 0:
                    self.time = None
                    self.PercentBoard.Hide()
                    self.TimerBoard.Hide()
                    self.ItemPrice.SetText(localeInfo.ITEMSHOP_PRICE_TITLE_BOX % (self.price, self.priceValue))
                    return

                self.lastTime = time.clock() + 1
                hoursRemaining = int(remaining) / 3600
                minutesRemaining = int(remaining % 3600) / 60
                secondsRemaining = int(remaining % 60)
                self.TimerBoard.SetText(localeInfo.ITEMSHOP_HOURS_MINUTES_SECONDS % (hoursRemaining, minutesRemaining, secondsRemaining))
        
    def SetSettings(self, parent, x, y):
        if not self.isLoaded:
            self.__LoadObjects()
        
        self.SetParent(parent)
        self.SetPosition(x, y)
        self.SetTop()
        self.Show()

class LogBox(ui.Window):
    
    def __init__(self):
        ui.Window.__init__(self)
        self.isLoaded = FALSE
        
    def __LoadObjects(self):
        Board = ui.ResizableTextValue()
        Board.SetParent(self)
        Board.SetSize(230, 110)
        Board.SetLine('left')
        Board.SetLine('right')
        Board.SetLine('top')
        Board.SetLine('bottom')
        Board.Show()
        self.Board = Board
        
        ItemName = ui.ResizableTextValue()
        ItemName.SetParent(self.Board)
        ItemName.SetSize(182, 18)
        ItemName.SetPosition(47, 0)
        ItemName.SetLine('left')
        ItemName.SetLine('bottom')
        ItemName.SetLine('top')
        ItemName.Show()
        self.ItemName = ItemName
        
        ToolTip = uiToolTip.ItemToolTip()
        ToolTip.HideToolTip()
        self.ToolTip = ToolTip
        
        ItemIcon = ui.ExpandedImageBox()
        ItemIcon.SetParent(self.Board)
        try:
            ItemIcon.SetMouseOverInEvent(self.ToolTip.ShowToolTip)
            ItemIcon.SetMouseOverOutEvent(self.ToolTip.HideToolTip)
        except:
            ItemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN", self.ToolTip.ShowToolTip)
            ItemIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT", self.ToolTip.HideToolTip)
        ItemIcon.Show()
        self.ItemIcon = ItemIcon
        
        ItemPrice = ui.ResizableTextValue()
        ItemPrice.SetParent(self.Board)
        ItemPrice.SetSize(117, 18)
        ItemPrice.SetPosition(112, 19)
        ItemPrice.SetLine('left')
        ItemPrice.SetLine('bottom')
        ItemPrice.Show()
        self.ItemPrice = ItemPrice
        
        ItemDate = ui.ResizableTextValue()
        ItemDate.SetParent(self.Board)
        ItemDate.SetSize(117, 18)
        ItemDate.SetPosition(112, 19*2)
        ItemDate.SetLine('left')
        ItemDate.SetLine('bottom')
        ItemDate.Show()
        self.ItemDate = ItemDate
        
        ItemHour = ui.ResizableTextValue()
        ItemHour.SetParent(self.Board)
        ItemHour.SetSize(117, 18)
        ItemHour.SetPosition(112, 19*3)
        ItemHour.SetLine('left')
        ItemHour.SetLine('bottom')
        ItemHour.Show()
        self.ItemHour = ItemHour
        
        self.SetSize(self.Board.GetWidth(), self.Board.GetHeight())
        self.isLoaded = TRUE
        
    def SetContent(self, vnum, amount, price, attrs, sockets):
        item.SelectItem(vnum)
        
        if amount > 1:
            self.ItemName.SetText('%s %dx' % (item.GetItemName(), amount))
        else:
            self.ItemName.SetText(item.GetItemName())
        self.ItemIcon.LoadImage(item.GetIconImageFileName())
        self.ItemIcon.SetPosition(40, (51,37,30)[item.GetItemSize()[1]-1])
        self.ItemIcon.SetScale(1, (1,1,0.8)[item.GetItemSize()[1]-1])
        self.ItemPrice.SetText(localeInfo.ITEMSHOP_PRICE_TITLE_BOX % (price, localeInfo.ITEMSHOP_ISHOP_COIN))
        
        attrs += [(0,0)]*(player.ATTRIBUTE_SLOT_MAX_NUM-7)
        
        self.ToolTip.ClearToolTip()
        if item.GetLimit(0)[0] == 7:
            self.ToolTip.AddItemData(vnum, [sockets[0]+app.GetGlobalTimeStamp(), sockets[1], sockets[2]], attrs)
        else:
            self.ToolTip.AddItemData(vnum, sockets, attrs)
        self.ToolTip.HideToolTip()
        
    def SetDate(self, date, hour):
        self.ItemDate.SetText(localeInfo.ITEMSHOP_DATE % date)
        self.ItemHour.SetText(localeInfo.ITEMSHOP_HOUR % hour)
    
    def SetSettings(self, parent, x, y):
        if not self.isLoaded:
            self.__LoadObjects()
        
        self.SetParent(parent)
        self.SetPosition(x, y)
        self.SetTop()
        self.Show()
        
class DiscountItemBox(ui.ScriptWindow):

    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.isLoaded = False
        self.data = [0,0]
        self.curShop = ''
        
    def __LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/itemshop_discountitembox.py")
        except:
            import exception
            exception.Abort("DiscountItemBox.LoadWindow.LoadObject")
            
        try:
            self.iconItem = self.GetChild("ItemIcon")
            self.nameItem = self.GetChild("ItemName")
            self.percent = self.GetChild("Percent")
            self.hours = self.GetChild("Hours")
            self.minutes = self.GetChild("Minutes")
            self.seconds = self.GetChild("Seconds")
            self.applyButton = self.GetChild("ApplyButton")
        except:
            import exception
            exception.Abort("DiscountItemBox.LoadWindow.LoadObject")
            
        self.toolTip = uiToolTip.ItemToolTip()
        self.toolTip.HideToolTip()
        
        try:
            self.iconItem.SetMouseOverInEvent(self.toolTip.ShowToolTip)
            self.iconItem.SetMouseOverOutEvent(self.toolTip.HideToolTip)
        except:
            self.iconItem.SAFE_SetStringEvent("MOUSE_OVER_IN", self.toolTip.ShowToolTip)
            self.iconItem.SAFE_SetStringEvent("MOUSE_OVER_OUT", self.toolTip.HideToolTip)
            
        self.AcceptQuestion = uiCommon.QuestionDialog()
        self.AcceptQuestion.SetAcceptEvent(lambda arg = True: self.AnswerAcceptQuestion(arg))
        self.AcceptQuestion.SetCancelEvent(lambda arg = False: self.AnswerAcceptQuestion(arg))
        self.applyButton.SetEvent(ui.__mem_func__(self.__OnClickAcceptButton))
        
        self.percent.SetReturnEvent(ui.__mem_func__(self.hours.SetFocus))
        self.percent.SetTabEvent(ui.__mem_func__(self.hours.SetFocus))
        self.hours.SetReturnEvent(ui.__mem_func__(self.minutes.SetFocus))
        self.hours.SetTabEvent(ui.__mem_func__(self.minutes.SetFocus))
        self.minutes.SetReturnEvent(ui.__mem_func__(self.seconds.SetFocus))
        self.minutes.SetTabEvent(ui.__mem_func__(self.seconds.SetFocus))
        self.seconds.SetReturnEvent(ui.__mem_func__(self.__OnClickAcceptButton))
        self.seconds.SetTabEvent(ui.__mem_func__(self.percent.SetFocus))
        
        self.isLoaded = True
        
    def SetShop(self, shop):
        self.curShop = shop
        
    def SetContent(self, id, vnum, attrs, sockets):
        self.data = [id, vnum]
        item.SelectItem(vnum)
        
        self.percent.SetText('1')
        self.hours.SetText('1')
        self.minutes.SetText('1')
        self.seconds.SetText('1')
        self.iconItem.LoadImage(item.GetIconImageFileName())
        self.nameItem.SetText(item.GetItemName())
        attrs += [(0,0)]*(player.ATTRIBUTE_SLOT_MAX_NUM-7)
        self.toolTip.ClearToolTip()
        self.toolTip.AddItemData(vnum, sockets, attrs)
        self.toolTip.HideToolTip()
        
    def __OnClickAcceptButton(self):
        item.SelectItem(self.data[1])
        self.AcceptQuestion.SetText(localeInfo.ITEMSHOP_ITEM_DISCOUNT_QUESTION % (int(self.percent.GetText()), item.GetItemName(), int(self.hours.GetText()), int(self.minutes.GetText()), int(self.seconds.GetText())))
        self.AcceptQuestion.Open()
        
    def AnswerAcceptQuestion(self, arg):
        self.time = (int(self.hours.GetText())*3600)+(int(self.minutes.GetText())*60)+(int(self.seconds.GetText()))
        if arg:
            constInfo.ItemShop['QCMD'] = 'ADD_ITEM_TIME#%d#%d#%s#%d' % (self.data[0], self.time, self.percent.GetText(), self.curShop)
            event.QuestButtonClick(constInfo.ItemShop['QID'])
        self.AcceptQuestion.Close()
        
    def __del__(self):
        ui.ScriptWindow.__del__(self)
        
    def OnUpdate(self):
        if self.percent.IsFocus():
            if len(self.percent.GetText()) > 0:
                if int(self.percent.GetText()) > 100:
                    self.percent.SetText('100')
        
    def Open(self, parent, x, y):
        if not self.isLoaded:
            self.__LoadWindow()
            
        self.SetParent(parent)
        self.SetPosition(x, y)
        self.Show()
        
class DiscountCategoryBox(ui.ScriptWindow):
    
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.isLoaded = False
        self.category = 0
        self.curShop = ''
    
    def __LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/itemshop_discountcategorybox.py")
        except:
            import exception
            exception.Abort("DiscountCategoryBox.LoadWindow.LoadObject")
            
        try:
            self.percent = self.GetChild("Percent")
            self.hours = self.GetChild("Hours")
            self.minutes = self.GetChild("Minutes")
            self.seconds = self.GetChild("Seconds")
            self.applyButton = self.GetChild("ApplyButton")
        except:
            import exception
            exception.Abort("DiscountItemBox.LoadWindow.LoadObject")
            
        self.AcceptQuestion = uiCommon.QuestionDialog()
        self.AcceptQuestion.SetAcceptEvent(lambda arg = True: self.AnswerAcceptQuestion(arg))
        self.AcceptQuestion.SetCancelEvent(lambda arg = False: self.AnswerAcceptQuestion(arg))
        self.applyButton.SetEvent(ui.__mem_func__(self.__OnClickAcceptButton))
        
        self.percent.SetReturnEvent(ui.__mem_func__(self.hours.SetFocus))
        self.percent.SetTabEvent(ui.__mem_func__(self.hours.SetFocus))
        self.hours.SetReturnEvent(ui.__mem_func__(self.minutes.SetFocus))
        self.hours.SetTabEvent(ui.__mem_func__(self.minutes.SetFocus))
        self.minutes.SetReturnEvent(ui.__mem_func__(self.seconds.SetFocus))
        self.minutes.SetTabEvent(ui.__mem_func__(self.seconds.SetFocus))
        self.seconds.SetReturnEvent(ui.__mem_func__(self.__OnClickAcceptButton))
        self.seconds.SetTabEvent(ui.__mem_func__(self.percent.SetFocus))
        self.isLoaded = True
        
    def __del__(self):
        ui.ScriptWindow.__del__(self)
        
    def SetCategoryID(self, id):
        self.category = id
        
    def SetShop(self, shop):
        self.curShop = shop
        
    def __OnClickAcceptButton(self):
        self.AcceptQuestion.SetText(localeInfo.ITEMSHOP_CATEGORY_DISCOUNT_QUESTION % (int(self.percent.GetText()), int(self.hours.GetText()), int(self.minutes.GetText()), int(self.seconds.GetText())))
        self.AcceptQuestion.Open()
        
    def AnswerAcceptQuestion(self, arg):
        self.time = (int(self.hours.GetText())*3600)+(int(self.minutes.GetText())*60)+(int(self.seconds.GetText()))
        if arg:
            constInfo.ItemShop['QCMD'] = 'ADD_CATEGORY_TIME#%d#%d#%s#%d' % (self.category, self.time, self.percent.GetText(), self.curShop)
            event.QuestButtonClick(constInfo.ItemShop['QID'])
        self.AcceptQuestion.Close()
        
    def Open(self, parent, x, y):
        if not self.isLoaded:
            self.__LoadWindow()
            
        self.SetParent(parent)
        self.SetPosition(x, y)
        self.Show()

class Item(ui.ListBoxEx.Item):

    def __init__(self, fileName):
        ui.ListBoxEx.Item.__init__(self)
        self.canLoad = 0
        self.text = fileName
        if len(fileName) > 40:
            self.textLine = self.__CreateTextLine(fileName[:37]+"...")
        else:
            self.textLine = self.__CreateTextLine(fileName[:40])

    def __del__(self):
        ui.ListBoxEx.Item.__del__(self)

    def GetText(self):
        return self.text

    def SetSize(self, width, height):
        ui.ListBoxEx.Item.SetSize(self, 6*len(self.textLine.GetText()) + 4, height)

    def __CreateTextLine(self, fileName):
        textLine = ui.TextLine()
        textLine.SetParent(self)
        textLine.SetPosition(0, 0)
        textLine.SetText(fileName)
        textLine.Show()
        return textLine

## Oscar ~ Shang