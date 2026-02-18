# coding: latin_1

# */*import ui
# import player
# import mouseModule
# import net
# import app
# import chat
# import item
# import grp
# import localeInfo
# import uiToolTip
# import chat

# class DopadorManual(ui.ScriptWindow):

    # POSITIVE_COLOR = grp.GenerateColor(0.5411, 0.7254, 0.5568, 1.0)
    # NEGATIVE_COLOR = grp.GenerateColor(0.9, 0.4745, 0.4627, 1.0)
    # VNUM_CHANGE    = 71084

    # def __init__(self):
        # import exception
        # ui.ScriptWindow.__init__(self)

        # self.get_affect = uiToolTip.ItemToolTip().AFFECT_DICT
        # self.pos_item = -1
        # self.pos_cambio = -1
        # self.__LoadWindow()

    # def __del__(self):
        # ui.ScriptWindow.__del__(self)

    # def __LoadWindow(self):
        # try:            
            # pyScrLoader = ui.PythonScriptLoader()
            # pyScrLoader.LoadScriptFile(self, "uiscript/switcher.py")
        # except:
            # import exception
            # exception.Abort("DopadorManual.LoadWindow.LoadObject")

        # self.bonus_name = [self.GetChild2("bonus_0"), self.GetChild2("bonus_1"), self.GetChild2("bonus_2"), self.GetChild2("bonus_3"), self.GetChild2("bonus_4"), ]
        # self.switchslot = self.GetChild("switchslot")
        # self.TitleBar = self.GetChild("TitleBar")
        # self.cambiar_bonus = self.GetChild("cambiar_bonus")

        # self.cambiar_bonus.SetEvent(ui.__mem_func__(self.CambiarBonus))
        # self.switchslot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))
        # self.switchslot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSlot))
        # self.TitleBar.SetCloseEvent(ui.__mem_func__(self.Close))

    # def __OnSelectItemSlot(self, selectedSlotPos):
        # isAttached = mouseModule.mouseController.isAttached()
        # if isAttached:
            # attachedSlotType = mouseModule.mouseController.GetAttachedType()
            # attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
            # mouseModule.mouseController.DeattachObject()
            # itemIndex = player.GetItemIndex(attachedSlotPos)
            # itemCount = player.GetItemCount(attachedSlotPos)
            # item.SelectItem(itemIndex)
            # itemType = item.GetItemType()

            # if item.ITEM_TYPE_WEAPON == itemType or item.ITEM_TYPE_ARMOR == itemType or item.ITEM_TYPE_RINGS == itemType:
                # self.pos_item = attachedSlotPos
                # self.switchslot.SetItemSlot(selectedSlotPos, itemIndex, 0)        
                # self.LoadBonus(attachedSlotPos)                
        # else:
            
            # self.ClearSlot()

    # def __OnSelectEmptySlot(self, selectedSlotPos):
        # isAttached = mouseModule.mouseController.isAttached()
        # if isAttached:
            # attachedSlotType = mouseModule.mouseController.GetAttachedType()
            # attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
            # mouseModule.mouseController.DeattachObject()
            # itemIndex = player.GetItemIndex(attachedSlotPos)
            # itemCount = player.GetItemCount(attachedSlotPos)
            # item.SelectItem(itemIndex)
            # itemType = item.GetItemType()
            # if item.ITEM_TYPE_WEAPON == itemType or item.ITEM_TYPE_ARMOR == itemType or item.ITEM_TYPE_RINGS == itemType:
                # self.pos_item = attachedSlotPos
                # self.switchslot.SetItemSlot(selectedSlotPos, itemIndex, 0)
                # self.LoadBonus(attachedSlotPos)                


    # def LoadBonus(self,slot):
        # attrSlot = [player.GetItemAttribute(slot, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]    
        # for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM-2):
            # type = attrSlot[i][0]
            # value = attrSlot[i][1]
            # if type != 0:
                # self.bonus_name[i].SetText("{}".format(self.get_affect[type](int(value))))

    # def CambiarBonus(self):
        # if self.pos_item < 0:
            # return

        # if player.GetItemIndex(self.pos_item) <= 0:
            # self.ClearSlot()
            # return

        # if self.pos_cambio < 0:
            # if not self.CheckCambiosSlot():
                # return
        # else:
            # if player.GetItemIndex(player.CHANGE_INVENTORY,self.pos_cambio) != self.VNUM_CHANGE:
                # if not self.CheckCambiosSlot():
                    # self.pos_cambio = -1
                    # return

        # net.SendItemUseToItemPacket(player.CHANGE_INVENTORY, self.pos_cambio, player.INVENTORY, self.pos_item)

    # def CheckCambiosSlot(self):
        # index = -1
        # for i in xrange(player.SPECIAL_PAGE_SIZE*4):
            # if player.GetItemIndex(player.CHANGE_INVENTORY,i) == self.VNUM_CHANGE:
                # self.pos_cambio = i
                # return True
        # return False

    # def Refresh(self,dstSlotPos,srcSlotPos):
        # attrSlot = [player.GetItemAttribute(dstSlotPos, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]    
        # for i in xrange(5):        
            # self.bonus_name[i].SetText("")
            
        # for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM-2):
            # type = attrSlot[i][0]
            # value = attrSlot[i][1]
            # if type != 0:
                # if int(value) < 0:                
                    # self.bonus_name[i].SetPackedFontColor(self.NEGATIVE_COLOR)
                # else:
                    # self.bonus_name[i].SetPackedFontColor(self.POSITIVE_COLOR)
                # self.bonus_name[i].SetText("{}".format(self.get_affect[type](int(value))))

    # def GetPosItemChange(self):
        # return self.pos_item

    # def OnUpdate(self):
        # self.Refresh(self.pos_item,self.pos_cambio)

    # def ClearSlot(self):
        # self.pos_item = -1
        # self.switchslot.SetItemSlot(0, 0, 0)
        # for i in xrange(5):        
            # self.bonus_name[i].SetText("None")

    # def Close(self):
        # self.ClearSlot()
        # self.Hide()
        
    # def OnPressEscapeKey(self):
        # self.Close()
        # return TRUE


# class DopadorMenu(ui.BoardWithTitleBar):
    # def __init__(self):
        # ui.BoardWithTitleBar.__init__(self)
        # self.interface = None
        # self.__LoadWindow()

    # def __del__(self):
        # ui.BoardWithTitleBar.__del__(self)

    # def __LoadWindow(self):
        # self.buttons = [["Dopador Manual"],["Dopador Automatico"]]

        # self.board = ui.BoardWithTitleBar()
        # self.AddFlag("movable")
        # self.AddFlag("animate")
        # self.SetCenterPosition()
        # self.SetTitleName("Dopador Menu")
        # self.SetSize(180,(len(self.buttons) * 40)+30)
        # self.SetCloseEvent(ui.__mem_func__(self.Close))
        # self.Hide()

        # self.btn = {}
        # for i in xrange(len(self.buttons)):
            # self.btn[i] = ui.Button()
            # self.btn[i].SetParent(self)
            # self.btn[i].SetPosition(15, 30*i + 40)
            # self.btn[i].SetUpVisual("flag/1.tga")
            # self.btn[i].SetOverVisual("flag/2.tga")
            # self.btn[i].SetDownVisual("flag/3.tga")
            # self.btn[i].SetText(self.buttons[i][0])
            # self.btn[i].SetEvent(self.OpenFunction, i)
            # self.btn[i].Show()

    # def BindInterface(self,interface):
        # self.interface = interface

    # def OpenFunction(self, arg):
        # if arg == 0:
            # if self.interface:
                # self.interface.ToggleSwitchBotManual()
        # elif arg == 1:
            # if self.interface:
                # self.interface.ToggleSwitchbotWindow()

        # self.Close()

    # def Open(self):
        # if self.IsShow():
            # self.Close()
        # else:
            # self.Show()

    # def IsShowNew(self):
        # if self.IsShow():
            # return True

        # return False

    # def Close(self):
        # self.Hide()
        
    # def OnPressEscapeKey(self):
        # self.Close()
        # return True
