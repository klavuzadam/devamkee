# coding: latin_1

import ui, constInfo, uiCommon, localeInfo, app, net, item, player, chat, skill

IMG_DIR = "d:/ymir work/ui/gemshop/"

class GemShop(ui.ScriptWindow):
    def __del__(self):
        ui.ScriptWindow.__del__(self)
        
    def Destroy(self):
        self.ClearDictionary()
        self.__data = {}
        
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.Destroy()
        self.LoadWindow()
        
    def OnPressEscapeKey(self):
        self.Close()
        return True
        
    def Open(self):
        self.Show()
        self.SetTop()

        loadStatus = self.__data["serverPacket"] if self.__data.has_key("serverPacket") else False
        if not loadStatus:
            self.__data["serverPacket"] = True
            net.SendChatPacket("/gem load")

    def Close(self):
        self.Hide()

    def LoadWindow(self):
        try:
            PythonScriptLoader = ui.PythonScriptLoader()
            PythonScriptLoader.LoadScriptFile(self, "UIScript/gemshopwindow.py")
        except:
            import exception
            exception.Abort("GemShop.LoadDialog.LoadObject")

        GetObject = self.GetChild
        GetObject("board").SetCloseEvent(ui.__mem_func__(self.Close))

        board = GetObject("bg_slots")

        itemSlot = ui.GridSlotWindow()
        itemSlot.SetParent(board)
        itemSlot.SetPosition(8, 7)
        itemSlot.ArrangeSlot(0, 5, 5, 32, 32, 13, 26)
        itemSlot.SAFE_SetButtonEvent("LEFT", "EXIST", self.__SelectItemSlot)
        itemSlot.SetOverInItemEvent(ui.__mem_func__(self.__OverInItem))
        itemSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OverOutItem))
        itemSlot.Show()
        self.ElementDictionary["itemSlot"] = itemSlot

        i = 0
        for j in xrange(5):
            for x in xrange(5):
                gayaIcon = ui.ImageBox()
                gayaIcon.SetParent(board)
                gayaIcon.SetPosition((8+(x*45))-2,(28+(j*58))+19)
                gayaIcon.LoadImage("d:/ymir work/ui/gemshop/gemshop_gemicon.sub")
                self.ElementDictionary["icon_{}".format(i)] = gayaIcon

                gayaPrice = ui.TextLine()
                gayaPrice.SetParent(board)
                gayaPrice.SetPosition((8+(x*45))+14,(28+(j*58))+18)
                self.ElementDictionary["price_{}".format(i)] = gayaPrice
                i+=1
        
        self.SetCenterPosition()
        self.Clear()
        self.Refresh()
        
    def Clear(self):
        itemSlot = self.GetChild("itemSlot")
        for j in xrange(25):
            self.GetChild("price_{}".format(j)).SetText("0")
            itemSlot.ClearSlot(j)
        self.__data["items"] = {}

    def Refresh(self):
        itemSlot = self.GetChild("itemSlot")
        for i in xrange(25):
            itemSlot.ClearSlot(i)

            itemData = self.__data["items"][i] if self.__data["items"].has_key(i) else None
            if itemData:
                itemSlot.SetItemSlot(i, itemData["itemVnum"], itemData["itemCount"])
                self.GetChild("price_{}".format(i)).SetText(str(itemData["itemPrice"]))
                self.GetChild("icon_{}".format(i)).Show()
                self.GetChild("price_{}".format(i)).Show()
        itemSlot.RefreshSlot()

    def EmptyFunc(self):
        pass

    def __PopupNotifyMessage(self, msg, func = 0):
        if not func:
            func = self.EmptyFunc
        game = constInfo.GetGameInstance()
        if game:
            game.stream.popupWindow.Close()
            game.stream.popupWindow.Open(msg, func, localeInfo.UI_OK)

    def __SelectItemSlot(self, slotPos):
        itemData = self.__data["items"][slotPos] if self.__data["items"].has_key(slotPos) else None
        if itemData:
            questionDialog = self.GetChild("questionDialog") if self.IsChild("questionDialog") else None
            if not questionDialog:
                questionDialog = uiCommon.QuestionDialog()
                questionDialog.SetCancelEvent(self.__CloseQuestionDialog)
                self.ElementDictionary["questionDialog"] = questionDialog

            item.SelectItem(itemData["itemVnum"])
            questionDialog.SetText(localeInfo.GAYA_BUY.format(item.GetItemName(), itemData["itemPrice"]))
            questionDialog.SetAcceptEvent(self.__Buy)
            questionDialog.buyPos = slotPos
            questionDialog.Open()

    def __OverOutItem(self):
        interface = constInfo.GetInterfaceInstance()
        if interface:
            if interface.tooltipItem:
                interface.tooltipItem.HideToolTip()

    def __OverInItem(self, slotIndex):
        itemData = self.__data["items"][slotIndex] if self.__data["items"].has_key(slotIndex) else None
        if not itemData:
            return

        interface = constInfo.GetInterfaceInstance()
        if interface:
            if interface.tooltipItem:
                interface.tooltipItem.SetItemToolTip(itemData["itemVnum"])

    def __Buy(self):
        questionDialog = self.GetChild("questionDialog") if self.IsChild("questionDialog") else None
        if questionDialog:
            questionDialog.Close()

            itemData = self.__data["items"][questionDialog.buyPos] if self.__data["items"].has_key(questionDialog.buyPos) else None
            if itemData:
                if itemData["itemPrice"] > player.GetGem():
                    chat.AppendChat(1, localeInfo.GAYA_NO_GEM)
                    return
                net.SendChatPacket("/gem buy {}".format(questionDialog.buyPos))

    def __CloseQuestionDialog(self):
        questionDialog = self.GetChild("questionDialog") if self.IsChild("questionDialog") else None
        if questionDialog:
            questionDialog.Close()

    def SetItemData(self, slotPos, itemVnum, itemCount, itemPrice, itemBuyed):
        self.__data["items"][int(slotPos)] = {
            "itemVnum":  int(itemVnum),
            "itemCount": int(itemCount),
            "itemPrice": int(itemPrice),
            "itemBuyed": int(itemBuyed),
        }
        
    def SetItemsWithString(self, cmdString):
        itemsList = cmdString.split("|")
        for itemData in itemsList:
            itemSplit = itemData.split("#")
            if len(itemSplit) != 5:
                continue
            self.SetItemData(*itemSplit)
        self.Refresh()
