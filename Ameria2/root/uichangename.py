# coding: latin_1

import ui
import player
import net
import item
import constInfo
import chat
import localeInfo

class NameInputWindow(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.__LoadWindow()

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def Show(self):
        ui.ScriptWindow.Show(self)

    def Destroy(self):
        self.ClearDictionary()
        self.board = 0
        self.boardtitle = 0
        self.itemslot = 0
        self.input_string = 0
        self.accept_btn = 0
        self.slotIndex = -1
        self.itemVnum = 0
        self.type = -1

    def Close(self):
        self.Hide()
        self.input_string.SetText("")
        self.slotIndex = -1
        self.destslotIndex = -1
        self.itemVnum = 0
        self.type = -1

    def OnPressEscapeKey(self):
        self.Close()
        return True

    def __LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "uiscript/nameinputwindow.py")
        except:
            import exception
            exception.Abort("NameInputWindow.LoadWindow.LoadObject")
        try:
            self.board = self.GetChild("board")
            self.boardtitle = self.GetChild("Titlebar")
            self.boardtitlename = self.GetChild("TitleName")
            self.itemslot = self.GetChild("ItemSlot")
            self.input_string = self.GetChild("InputString")
            self.accept_btn = self.GetChild("acceptbtn")
            self.boardtitle.SetCloseEvent(ui.__mem_func__(self.Close))
            self.accept_btn.SetEvent(ui.__mem_func__(self.AcceptInput))
            self.itemslot.SetOverInItemEvent(ui.__mem_func__(self.__OverIn))
            self.itemslot.SetOverOutItemEvent(ui.__mem_func__(self.__OverOut))
            
        except:
            import exception
            exception.Abort("NameInputWindow.LoadWindow.BindObject")

    def __OverOut(self):
        interface = constInfo.GetInterfaceInstance()
        if not interface:
            return
        if None != interface.tooltipItem:
            interface.tooltipItem.HideToolTip()

    def __OverIn(self, index):
        interface = constInfo.GetInterfaceInstance()
        if not interface:
            return
        if None != interface.tooltipItem:
            interface.tooltipItem.SetInventoryItem(self.destslotIndex)

    def LoadItemData(self, srcItemPos, destItemPos, itemVnum, type = 0):
        self.type = type
        self.slotIndex = srcItemPos
        self.destslotIndex = destItemPos
        self.itemVnum = itemVnum

        if self.type == 2:
            self.GetChild("PetNamingTitle").SetText(localeInfo.NAME_PET_INPUT_TEXT)
        else:
            self.GetChild("PetNamingTitle").SetText(localeInfo.NAME_INPUT_TEXT)

        if self.itemVnum:
            itemName = player.GetItemNewName(self.destslotIndex)
            if itemName != "^":
                self.boardtitlename.SetText(itemName)
            else:
                item.SelectItem(self.itemVnum)
                self.boardtitlename.SetText(item.GetItemName())

        self.itemslot.SetItemSlot(0,self.itemVnum, 0)


    def AcceptInput(self):
        text = self.input_string.GetText()
        if text == "" or len(text) < 2:
            chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.INPUT_WINDOW_TEXT)
            return
        if player.GetElk() < 100000:
            player.AppendChat(chat.CHAT_TYPE_INFO, "687 %d"%100000)
            return
        if constInfo.getInjectCheck(self.input_string.GetText()):
            chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.INPUT_WINDOW_TEXT)
            return
        #text = self.input_string.GetText().replace(" ",";")
        if self.type == 2:
            net.SendChatPacket("/pet_egg %d %s"%(self.slotIndex,text))
        else:
            net.SendChatPacket("/change_name %d %d %s"%(self.slotIndex,self.destslotIndex,text))
        self.Close()
