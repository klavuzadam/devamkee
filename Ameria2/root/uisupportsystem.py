# coding: latin_1

import os
import ui
import player
import mouseModule
import net
import app
import snd
import item
import player
import chat
import grp
import uiScriptLocale
import localeInfo
import constInfo
import ime
import wndMgr
import uiToolTip
import dbg
import uiCommon

def checkdiv(n):
    x = str(n/10.0)
    if len(x) > 3:
        return str(x)[0:3]
    return str(x)

def pointop(n):
    t = int(n)
    if t / 10 < 1:
        return "0."+n
    else:        
        return n[0:len(n)-1]+"."+n[len(n)-1:]
        
class SupportMainGui(ui.ScriptWindow):

    ITEMS_VNUMS = [0,0,0,0,0,0,0]
    ITEMS_TIME = [0,0,0,0,0,0,0]
    ITEMS_PERMANENTES = [0,0,0]
    SLOT_ITEM_INV = -1

    def __init__(self, vnum = 0):
        ui.ScriptWindow.__init__(self)
        self.vnum = vnum
        self.tooltipItem = uiToolTip.ItemToolTip()
        self.tooltipItem.Hide()
        self.itemDropQuestionDialog = None
        self.__LoadWindow()

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def Show(self):
        ui.ScriptWindow.Show(self)

    def Close(self):
        self.Hide()
        constInfo.SUPPORTGUI = 0
        
    def OnPressEscapeKey(self):
        self.Close()
        return TRUE    

    def __LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "uiscript/supportinformationwindow.py")
        except:
            import exception
            exception.Abort("supportinformationwindow.LoadWindow.LoadObject")
            
        try:
            self.board = self.GetChild("board")
            self.boardclose = self.GetChild("CloseButton")
        
            self.boardclose.SetEvent(ui.__mem_func__(self.Close))
            self.combSlot = self.GetChild("CombSlot")
            self.combSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))
            self.combSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
            self.combSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
            self.combSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

            self.SetDefaultInfo()
        except:
            import exception
            exception.Abort("PetInformationWindow.LoadWindow.BindObject")

    def SetDefaultInfo(self):
        for i in xrange(7):
            self.combSlot.ClearSlot(i)

    def SetArmor(self, vnum, time, permanente):
        if int(vnum) == 11899:
            return

        constInfo.SUPPORT_INFO_ITEMS["item_1"] = int(vnum)

        self.combSlot.SetItemSlot(1, int(vnum), 1)
        self.ITEMS_VNUMS[0] = int(vnum)
        self.ITEMS_TIME[0] = int(time) + app.GetGlobalTimeStamp()
        self.ITEMS_PERMANENTES[0] = int(permanente)

    def SetHair(self, vnum, time, permanente):

        constInfo.SUPPORT_INFO_ITEMS["item_2"] = int(vnum)

        self.combSlot.SetItemSlot(2, int(vnum), 1)
        self.ITEMS_VNUMS[1] = int(vnum)
        self.ITEMS_TIME[1] = int(time) + app.GetGlobalTimeStamp()
        self.ITEMS_PERMANENTES[1] = int(permanente)

    def SetWeapon(self, vnum, time, permanente):
        if int(vnum) == 7159:
            return

        constInfo.SUPPORT_INFO_ITEMS["item_3"] = int(vnum)

        self.combSlot.SetItemSlot(3, int(vnum), 1)
        self.ITEMS_VNUMS[2] = int(vnum)
        self.ITEMS_TIME[2] = int(time) + app.GetGlobalTimeStamp()
        self.ITEMS_PERMANENTES[2] = int(permanente)

    def SetSkill(self, slot, vnum, time):
        constInfo.SUPPORT_INFO_ITEMS["item_%d"%int(slot)] = int(vnum)

        if int(vnum) >= 55502 and int(vnum) <= 55510:
            self.combSlot.SetItemSlot(int(slot), int(vnum), 0)
            self.ITEMS_VNUMS[int(slot)] = int(vnum)
            self.ITEMS_TIME[int(slot)] = int(time) + app.GetGlobalTimeStamp()


    def SupportClear(self,index):
        
        self.combSlot.SetItemSlot(index, 0, 0)
        if index >= 4 and index <= 6:
            self.ITEMS_VNUMS[index] = 0
            self.ITEMS_TIME[index] = 0

            constInfo.SUPPORT_INFO_ITEMS["item_%d"%(int(index))] = 0
        else:
            self.ITEMS_VNUMS[index-1] = 0
            self.ITEMS_TIME[index-1] = 0
            self.ITEMS_PERMANENTES[index-1] = 0
            constInfo.SUPPORT_INFO_ITEMS["item_%d"%(int(index))] = 0

        self.combSlot.RefreshSlot()


    def OverInItem(self, slot):
        permanente = 0
        if slot <= 3:
            slot = slot -1
            permanente = self.ITEMS_PERMANENTES[slot]
            
        if self.ITEMS_VNUMS[slot] <= 0:
            return

        self.tooltipItem.SetItemBuff(self.ITEMS_VNUMS[slot],self.ITEMS_TIME[slot],self.ITEMS_PERMANENTES[slot])
        
    def OverOutItem(self):
        if self.tooltipItem:
            self.tooltipItem.HideToolTip()
            
    def __OnSelectEmptySlot(self, selectedSlotPos):
        isAttached = mouseModule.mouseController.isAttached()
        if isAttached:
            if constInfo.SUPPORT_SHOW == 0:
                mouseModule.mouseController.DeattachObject()
                return
            attachedSlotType = mouseModule.mouseController.GetAttachedType()
            attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
            targetIndex = player.GetItemIndex(attachedSlotPos)
            if attachedSlotType != player.SLOT_TYPE_INVENTORY:
                return
            mouseModule.mouseController.DeattachObject()
            if targetIndex <= 0:
                return
            item.SelectItem(targetIndex)
            itemType = item.GetItemType()
            itemSubType = item.GetItemSubType()
            itemVnum = player.GetItemIndex(attachedSlotPos)

            if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
                chat.AppendChat(chat.CHAT_TYPE_INFO, "<Sistem> You cant add a item for male character.")
                return
                
            # dbg.LogBox(str(selectedSlotPos)+"_"+str(itemVnum))
            if selectedSlotPos == 1:
                net.SendChatPacket("/support_system %d %d" %(attachedSlotPos, selectedSlotPos))
            elif selectedSlotPos == 2:
                net.SendChatPacket("/support_system %d %d" % (attachedSlotPos, selectedSlotPos))
            elif selectedSlotPos == 3:
                if not item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN):
                    net.SendChatPacket("/support_system %d %d" % (attachedSlotPos, selectedSlotPos))
            elif selectedSlotPos >= 4 and selectedSlotPos <= 6:
                if itemVnum >= 55502 and itemVnum <= 55504 and selectedSlotPos != 4:
                    chat.AppendChat(chat.CHAT_TYPE_INFO, "<Sistem> This item must be equiped in the first slot.")
                    return
                if itemVnum >= 55505 and itemVnum <= 55507 and selectedSlotPos != 5:
                    chat.AppendChat(chat.CHAT_TYPE_INFO, "<Sistem> This item must be equiped in the second slot.")
                    return
                if itemVnum >= 55508 and itemVnum <= 55510 and selectedSlotPos != 6:
                    chat.AppendChat(chat.CHAT_TYPE_INFO, "<Sistem> This item must be equiped in the third slot.")
                    return
                net.SendChatPacket("/support_system %d %d" % (attachedSlotPos, selectedSlotPos))

            if item.COSTUME_TYPE_BODY == itemSubType:
                selectedSlotPos = 1

            if item.COSTUME_TYPE_HAIR == itemSubType:
                selectedSlotPos = 2

            if item.COSTUME_TYPE_WEAPON == itemSubType:
                selectedSlotPos = 3

            #self.SLOT_ITEM_INV=attachedSlotPos

            #self.combSlot.SetItemSlot(selectedSlotPos, player.GetItemIndex(attachedSlotPos), player.GetItemCount(attachedSlotPos))
            #self.ITEMS_VNUMS[selectedSlotPos] = itemVnum

    def SelectItemSlot(self, selectedSlotPos):
        if constInfo.SUPPORT_SHOW == 0:
            return
            
        self.itemDropQuestionDialog = uiCommon.QuestionDialog()
        self.itemDropQuestionDialog.SetText("Desea Retirar el Objeto?")
        self.itemDropQuestionDialog.SetAcceptEvent(lambda arg=True, slot=selectedSlotPos: self.RequestDropItem(arg, slot))
        self.itemDropQuestionDialog.SetCancelEvent(lambda arg=False, slot=selectedSlotPos: self.RequestDropItem(arg, slot))
        self.itemDropQuestionDialog.Open()

    def RequestDropItem(self, arg, selectedSlotPos):
        if not self.itemDropQuestionDialog:
            return

        if arg:
            if selectedSlotPos == 1:
                net.SendChatPacket("/support_system_clear 1")
            elif selectedSlotPos == 2:
                net.SendChatPacket("/support_system_clear 2")
            elif selectedSlotPos == 3:
                net.SendChatPacket("/support_system_clear 3")
            elif selectedSlotPos >= 4 and selectedSlotPos <= 6:
                net.SendChatPacket("/support_system_clear %d"%selectedSlotPos)
                
        self.itemDropQuestionDialog.Close()
        self.itemDropQuestionDialog = None