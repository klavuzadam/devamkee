# coding: latin_1

import ui
import wndMgr
import localeInfo
import item
import player
import chat

RUTA_IMGS = "tabla_bonus/"

class TableBonusWindows(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.Max_Count = 14
        self.ScollPos = 0
        self.Max_Count_View = 14
        self.InfoList = []
        self.INFO_TEXT    = 0
        self.INFO_FILTRE = 1
        self.INFO_VALUE    = 2
        self.object_bonus = {}
        self.object_bonus["filtre_list"] = []
        self.filtre_select = 0

        self.BONUS_PVM = 1
        self.BONUS_PVP = 2
        self.BONUS_PVM_PVP = 3

    
    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UiScript/tablebonus.py")
        except:
            import exception
            exception.Abort("TableBonusWindows.LoadWindow.LoadObject")

        self.ImgBase = self.GetChild("ImgBase")
        self.ScrollBar = self.GetChild("ScrollBar")
        self.TitleBar = self.GetChild("TitleBar")

        self.object_bonus["filtre_bonus"] = []

        for x in xrange(1,4):
            self.object_bonus["filtre_bonus"].append(self.GetChild("filtro_%d"%(x)))

        count = 0
        for filtre in self.object_bonus["filtre_bonus"]:
            filtre.SetEvent(ui.__mem_func__(self.SetButtonFiltre),count)
            count+=1

        for i in xrange(self.Max_Count):
            self.object_bonus["slot_bonus%d"%i] = ui.ImageBox()
            self.object_bonus["slot_bonus%d"%i].SetParent(self.ImgBase)
            self.object_bonus["slot_bonus%d"%i].LoadImage(RUTA_IMGS+"slot.tga")
            self.object_bonus["slot_bonus%d"%i].SetPosition(3,(21*i+36)-11)
            self.object_bonus["slot_bonus%d"%i].Hide()

            self.object_bonus["name_bonus%d"%i] = ui.TextLine()
            self.object_bonus["name_bonus%d"%i].SetParent(self.object_bonus["slot_bonus%d"%i])
            self.object_bonus["name_bonus%d"%i].SetText("")
            self.object_bonus["name_bonus%d"%i].SetPosition(9,2)
            self.object_bonus["name_bonus%d"%i].Hide()

            self.object_bonus["value_bonus_%d"%i] = ui.TextLine()
            self.object_bonus["value_bonus_%d"%i].SetParent(self.object_bonus["slot_bonus%d"%i])
            self.object_bonus["value_bonus_%d"%i].SetHorizontalAlignCenter()
            self.object_bonus["value_bonus_%d"%i].SetText("")
            self.object_bonus["value_bonus_%d"%i].SetPosition(217,2)
            self.object_bonus["value_bonus_%d"%i].Hide()

        self.TitleBar.SetCloseEvent(ui.__mem_func__(self.Close))
        self.ScrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
        self.SetButtonFiltre(0)

    def RefreshBonus(self):
        self.RefreshLabel()

    def SetButtonFiltre(self,buttonIndex):
        self.filtre_select = buttonIndex
        
        self.LoadListBonus()
        self.ScrollBar.SetPos(0.0)
        self.__ClickRadioButton(buttonIndex)
        self.__ClearFiltre()
        self.RefreshLabel()

    def __ClickRadioButton(self,buttonIndex):
        selButton=self.object_bonus["filtre_bonus"][buttonIndex]

        for filtre in self.object_bonus["filtre_bonus"]:
            filtre.SetUp()

        selButton.Down()    

    def OnScroll(self):
        self.RefreshLabel()

    def RefreshLabel(self):
        self.ScollPos = int(self.ScrollBar.GetPos() * self.Diff)

        if self.ScollPos < 0:
            return

        for i in xrange(self.Max_Count):
            idx = i + self.ScollPos

            if idx < len(self.FiltreList()):
                info = self.FiltreList()
                text = info[idx][self.INFO_TEXT]
                type = info[idx][self.INFO_VALUE]
                value  = player.GetStatus(type)
                self.__LabelLine(i, text, value)

            else:
                self.__EmptyLine(i)


    def __ClearFiltre(self):
        for i in xrange(self.Max_Count):
            self.object_bonus["value_bonus_%d"%i].Hide()
            self.object_bonus["slot_bonus%d"%i].Hide()
            self.object_bonus["name_bonus%d"%i].Hide()

    def __LabelLine(self, idx, text, value):
        if(idx < self.Max_Count):
            self.object_bonus["name_bonus%d"%idx].SetText(text)
            self.object_bonus["value_bonus_%d"%idx].SetText(str(value))

            self.object_bonus["value_bonus_%d"%idx].Show()
            self.object_bonus["name_bonus%d"%idx].Show()
            self.object_bonus["slot_bonus%d"%idx].Show()

    def __EmptyLine(self, idx):
        if(idx < self.Max_Count):
            self.object_bonus["value_bonus_%d"%idx].Hide()
            self.object_bonus["slot_bonus%d"%idx].Hide()
            self.object_bonus["name_bonus%d"%idx].Hide()

    def LoadListBonus(self):
        self.InfoList = []
        self.object_bonus["filtre_list"] = []

        self.InfoList.append([localeInfo.DETAILS_13, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_DEF_GRADE_BONUS)])
        self.InfoList.append([localeInfo.DETAILS_14, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_NORMAL_HIT_DAMAGE_BONUS)])
        self.InfoList.append([localeInfo.DETAILS_15, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_NORMAL_HIT_DEFEND_BONUS)])
        self.InfoList.append([localeInfo.DETAILS_72, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_MALL_DEFBONUS)])
        self.InfoList.append([localeInfo.DETAILS_68, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_MAX_HP_PCT)])
        self.InfoList.append([localeInfo.DETAILS_69, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_MAX_SP_PCT)])
        self.InfoList.append([localeInfo.DETAILS_20, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_CRITICAL_PCT)])
        self.InfoList.append([localeInfo.DETAILS_21, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_PENETRATE_PCT)])
        self.InfoList.append([localeInfo.DETAILS_54, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_SLOW_PCT)])
        self.InfoList.append([localeInfo.DETAILS_55, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_POISON_PCT)])
        self.InfoList.append([localeInfo.DETAILS_53, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_STUN_PCT)])
        self.InfoList.append([localeInfo.DETAILS_89, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_MOV_SPEED)])
        self.InfoList.append([localeInfo.DETAILS_88, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_CAST_SPEED)])
        self.InfoList.append([localeInfo.DETAILS_87, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_ATT_SPEED)])
        self.InfoList.append([localeInfo.DETAILS_85, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_MAX_HP)])
        self.InfoList.append([localeInfo.DETAILS_86, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_MAX_SP)])
        self.InfoList.append([localeInfo.DETAILS_65, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_REFLECT_MELEE)])
        self.InfoList.append([localeInfo.DETAILS_82, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_CON)])
        self.InfoList.append([localeInfo.DETAILS_83, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_INT)])
        self.InfoList.append([localeInfo.DETAILS_81, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_STR)])
        self.InfoList.append([localeInfo.DETAILS_84, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_DEX)])
        self.InfoList.append([localeInfo.DETAILS_63, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_BLOCK)])
        self.InfoList.append([localeInfo.DETAILS_64, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_DODGE)])
        self.InfoList.append([localeInfo.DETAILS_56, self.BONUS_PVM_PVP, item.GetPointTypeByApplyType(item.APPLY_POISON_REDUCE)])
        self.InfoList.append([localeInfo.DETAILS_5, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_MONSTER)])
        self.InfoList.append([localeInfo.DETAILS_4, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_UNDEAD)])
        self.InfoList.append([localeInfo.DETAILS_3, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_ORC)])
        self.InfoList.append([localeInfo.DETAILS_7, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_ANIMAL)])
        self.InfoList.append([localeInfo.DETAILS_8, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_MILGYO)])
        self.InfoList.append([localeInfo.DETAILS_9, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_DEVIL)])
        self.InfoList.append([localeInfo.DETAILS_12, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_ATT_GRADE_BONUS)])
        self.InfoList.append([localeInfo.DETAILS_61, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_HP_REGEN)])
        self.InfoList.append([localeInfo.DETAILS_62, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_SP_REGEN)])
        self.InfoList.append([localeInfo.DETAILS_59, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_STEAL_HP)])
        self.InfoList.append([localeInfo.DETAILS_60, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_STEAL_SP)])
        self.InfoList.append([localeInfo.DETAILS_66, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_KILL_HP_RECOVER)])
        self.InfoList.append([localeInfo.DETAILS_67, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_KILL_SP_RECOVER)])
        self.InfoList.append([localeInfo.DETAILS_79, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_GOLD_DOUBLE_BONUS)])
        self.InfoList.append([localeInfo.DETAILS_75, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_MALL_GOLDBONUS)])
        self.InfoList.append([localeInfo.DETAILS_80, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_ITEM_DROP_BONUS)])
        self.InfoList.append([localeInfo.DETAILS_74, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_MALL_ITEMBONUS)])
        self.InfoList.append([localeInfo.DETAILS_78, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_EXP_DOUBLE_BONUS)])
        self.InfoList.append([localeInfo.DETAILS_73, self.BONUS_PVM, item.GetPointTypeByApplyType(item.APPLY_MALL_EXPBONUS)])
        self.InfoList.append([localeInfo.DETAILS_1, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_HUMAN)])
        self.InfoList.append([localeInfo.DETAILS_2, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_HUMAN)])
        self.InfoList.append([localeInfo.DETAILS_36, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_WARRIOR)])
        self.InfoList.append([localeInfo.DETAILS_41, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_WARRIOR)])
        self.InfoList.append([localeInfo.DETAILS_37, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_ASSASSIN)])
        self.InfoList.append([localeInfo.DETAILS_42, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_ASSASSIN)])
        self.InfoList.append([localeInfo.DETAILS_38, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_SURA)])
        self.InfoList.append([localeInfo.DETAILS_43, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_SURA)])
        self.InfoList.append([localeInfo.DETAILS_39, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_ATTBONUS_SHAMAN)])
        self.InfoList.append([localeInfo.DETAILS_44, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_SHAMAN)])
        self.InfoList.append([localeInfo.DETAILS_46, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_SWORD)])
        self.InfoList.append([localeInfo.DETAILS_47, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_TWOHAND)])
        self.InfoList.append([localeInfo.DETAILS_48, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_DAGGER)])
        self.InfoList.append([localeInfo.DETAILS_52, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_BOW)])
        self.InfoList.append([localeInfo.DETAILS_50, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_BELL)])
        self.InfoList.append([localeInfo.DETAILS_51, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_FAN)])
        self.InfoList.append([localeInfo.DETAILS_76, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_RESIST_MAGIC)])
        self.InfoList.append([localeInfo.DETAILS_16, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_SKILL_DAMAGE_BONUS)])
        self.InfoList.append([localeInfo.DETAILS_17, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_SKILL_DEFEND_BONUS)])
        self.InfoList.append([localeInfo.DETAILS_22, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_ANTI_CRITICAL_PCT)])
        self.InfoList.append([localeInfo.DETAILS_23, self.BONUS_PVP, item.GetPointTypeByApplyType(item.APPLY_ANTI_PENETRATE_PCT)])

        if self.filtre_select > 0:
            for filtre in self.InfoList:
                if filtre[self.INFO_FILTRE] == self.filtre_select or filtre[self.INFO_FILTRE] == self.BONUS_PVM_PVP:
                    self.object_bonus["filtre_list"].append(filtre)


        self.Diff = len(self.FiltreList()) - (self.Max_Count_View)
        if self.Diff > 0:
            stepSize = 1.0 / self.Diff
            self.ScrollBar.SetScrollStep( stepSize )
        self.ScollPos = 0

    def OnRunMouseWheel(self, nLen):
        if nLen > 0:
            self.ScrollBar.OnUp()
        else:
            self.ScrollBar.OnDown()

    def FiltreList(self):
        if self.filtre_select > 0:
            return self.object_bonus["filtre_list"]
        return self.InfoList

    def OnPressEscapeKey(self):
        self.Close()
        return True

    def Close(self):
        wndMgr.Hide(self.hWnd)
