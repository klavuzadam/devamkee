# coding: latin_1

import generateinterface
import wndMgr
import uiToolTip
import item
import constInfo2
import localeInfo

class PetInventoryDialog(generateinterface.ScriptWindow):

    class TextToolTip(generateinterface.Window):
        def __init__(self):
            generateinterface.Window.__init__(self, "TOP_MOST")

            textLine = generateinterface.TextLine()
            textLine.SetParent(self)
            textLine.SetHorizontalAlignCenter()
            textLine.SetOutline()
            textLine.Show()
            self.textLine = textLine

        def __del__(self):
            generateinterface.Window.__del__(self)

        def SetText(self, text):
            self.textLine.SetText(text)

        def OnRender(self):
            (mouseX, mouseY) = wndMgr.GetMousePosition()
            self.textLine.SetPosition(mouseX, mouseY - 15)

    def __init__(self):
        generateinterface.ScriptWindow.__init__(self)
        
        self.SLOT_HEAD = 0
        self.SLOT_NECK = 1
        self.SLOT_FOOT = 2
        
        self.SLOT_ITEMS = [0, 0, 0]
        self.TOOLTIP_SHOWED = -1
        self.INCREASE_BONI_TYPE = 0
        self.ep = 0
        self.maxep = 0
        
        self.name = "Name"
        self.level = 1

        self.tooltipEXP = self.TextToolTip()
        self.tooltipEXP.Hide()
        
        self.__Load()

    def __del__(self):
        generateinterface.ScriptWindow.__del__(self)

    def Destroy(self):
        self.Hide()

    def __Load_LoadScript(self):
        try:
            pyScriptLoader = generateinterface.PythonScriptLoader()
            pyScriptLoader.LoadScriptFile(self, "UIscript/petinventoryboard.py")

        except:
            import exception
            exception.Abort("PetInventoryDialog.__Load_LoadScript")

    def __Load_BindObject(self):
        try:
            self.board = self.GetChild("board")
            self.title = self.GetChild("titlebar")
        except:
            import exception
            exception.Abort("PetInventoryDialog.__Load_BindObject")

        self.title.SetCloseEvent(self.Hide)

    def __Load_CreateObject(self):
        self.pet_name = generateinterface.TextLine()
        self.pet_name.SetParent(self.board)
        self.pet_name.SetPosition(self.GetWidth()/2, self.title.GetHeight() - 5)
        self.pet_name.SetHorizontalAlignCenter()
        self.pet_name.SetFontName("Tahoma:17")
        self.pet_name.SetText("Name (Lv. 1)")
        self.pet_name.Show()
        w, h = self.pet_name.GetTextSize()
        
        self.pet_image = generateinterface.ImageBox()
        self.pet_image.SetParent(self.board)
        self.pet_image.SetPosition(0, self.title.GetHeight() - 5 + h + 8)
        self.pet_image.SetWindowHorizontalAlignCenter()
        self.pet_image.Show()
        
        self.itemToolTip = uiToolTip.ItemToolTip()
        self.itemToolTip.HideToolTip()
        
        self.slots = []
        for i in xrange(0, 2+1):
            self.slots.append(generateinterface.ImageBox())
            self.slots[i].SetParent(self.board)
            self.slots[i].LoadImage("d:/ymir work/age_of_zaria/game/windows/metin_slot_gold.sub")
            self.slots[i].itemImage = generateinterface.ImageBox()
            self.slots[i].itemImage.SetParent(self.slots[i])
            self.slots[i].itemImage.SetPosition(0, 0)
            self.slots[i].itemImage.SetWindowHorizontalAlignCenter()
            self.slots[i].itemImage.SetWindowVerticalAlignCenter()
            self.slots[i].itemImage.SetMouseOverInEvent(self.ShowItemToolTip, i)
            self.slots[i].itemImage.SetMouseOverOutEvent(self.HideItemToolTip)
            self.slots[i].itemImage.SetMouseRightClickEvent(self.ClearItemSlot, i)
            self.slots[i].itemImage.SetMouseDoubleClickEvent(self.ClearItemSlot, i)
            self.slots[i].itemImage.Hide()
            self.slots[i].Show()

        self.skill_points = generateinterface.TextLine()
        self.skill_points.SetParent(self.board)
        self.skill_points.SetHorizontalAlignCenter()
        self.skill_points.SetWindowHorizontalAlignCenter()
        self.skill_points.SetFontName("Tahoma:13")
        self.skill_points.SetFontColor(0.8784, 0.5255, 0.0)
        self.skill_points.Hide()
        
        self.Boni = generateinterface.Window()
        self.Boni.SetParent(self.board)
        self.Boni.SetSize(230, 30*3)
        self.Boni.SetWindowHorizontalAlignCenter()
        self.Boni.Show()
        
        ##################################
        ## Attack-Damage Boni
        ##################################
        self.Boni.Attack = generateinterface.Window()
        self.Boni.Attack.SetParent(self.Boni)
        self.Boni.Attack.SetPosition(0, 30 * 0)
        self.Boni.Attack.SetSize(230, 30)
        self.Boni.Attack.Show()
        
        self.Boni.Attack.Text = generateinterface.TextLine()
        self.Boni.Attack.Text.SetParent(self.Boni.Attack)
        self.Boni.Attack.Text.SetPosition(30, 2)
        self.Boni.Attack.Text.SetDefaultFontName()
        self.Boni.Attack.Text.SetText("Angriffswert")
        self.Boni.Attack.Text.Show()
        
        self.Boni.Attack.ValueBox = generateinterface.ImageBox()
        self.Boni.Attack.ValueBox.SetParent(self.Boni.Attack)
        self.Boni.Attack.ValueBox.SetPosition(150, 0)
        self.Boni.Attack.ValueBox.LoadImage("d:/ymir work/age_of_zaria/public/Parameter_Slot_00.sub")
        self.Boni.Attack.ValueBox.Show()
        
        self.Boni.Attack.Value = generateinterface.TextLine()
        self.Boni.Attack.Value.SetParent(self.Boni.Attack.ValueBox)
        self.Boni.Attack.Value.SetPosition(0, 0)
        self.Boni.Attack.Value.SetWindowHorizontalAlignCenter()
        self.Boni.Attack.Value.SetHorizontalAlignCenter()
        self.Boni.Attack.Value.SetWindowVerticalAlignCenter()
        self.Boni.Attack.Value.SetVerticalAlignCenter()
        self.Boni.Attack.Value.SetText("0")
        self.Boni.Attack.Value.Show()
        
        self.Boni.Attack.IncreaseButton = generateinterface.Button()
        self.Boni.Attack.IncreaseButton.SetParent(self.Boni.Attack)
        self.Boni.Attack.IncreaseButton.SetPosition(150 + self.Boni.Attack.ValueBox.GetWidth() + 2, 3)
        self.Boni.Attack.IncreaseButton.SetUpVisual("d:/ymir work/age_of_zaria/game/windows/btn_plus_up.sub")
        self.Boni.Attack.IncreaseButton.SetOverVisual("d:/ymir work/age_of_zaria/game/windows/btn_plus_over.sub")
        self.Boni.Attack.IncreaseButton.SetDownVisual("d:/ymir work/age_of_zaria/game/windows/btn_plus_down.sub")
        self.Boni.Attack.IncreaseButton.SetEvent(self.IncreaseBoni, 1)
        self.Boni.Attack.IncreaseButton.Hide()
        
        ##################################
        ## Magic-Attack-Damage Boni
        ##################################
        self.Boni.MAttack = generateinterface.Window()
        self.Boni.MAttack.SetParent(self.Boni)
        self.Boni.MAttack.SetPosition(0, 30 * 1)
        self.Boni.MAttack.SetSize(230, 30)
        self.Boni.MAttack.Show()
        
        self.Boni.MAttack.Text = generateinterface.TextLine()
        self.Boni.MAttack.Text.SetParent(self.Boni.MAttack)
        self.Boni.MAttack.Text.SetPosition(30, 2)
        self.Boni.MAttack.Text.SetDefaultFontName()
        self.Boni.MAttack.Text.SetText("Magischer Angriffswert")
        self.Boni.MAttack.Text.Show()
        
        self.Boni.MAttack.ValueBox = generateinterface.ImageBox()
        self.Boni.MAttack.ValueBox.SetParent(self.Boni.MAttack)
        self.Boni.MAttack.ValueBox.SetPosition(150, 0)
        self.Boni.MAttack.ValueBox.LoadImage("d:/ymir work/age_of_zaria/public/Parameter_Slot_00.sub")
        self.Boni.MAttack.ValueBox.Show()
        
        self.Boni.MAttack.Value = generateinterface.TextLine()
        self.Boni.MAttack.Value.SetParent(self.Boni.MAttack.ValueBox)
        self.Boni.MAttack.Value.SetPosition(0, 0)
        self.Boni.MAttack.Value.SetWindowHorizontalAlignCenter()
        self.Boni.MAttack.Value.SetHorizontalAlignCenter()
        self.Boni.MAttack.Value.SetWindowVerticalAlignCenter()
        self.Boni.MAttack.Value.SetVerticalAlignCenter()
        self.Boni.MAttack.Value.SetText("0")
        self.Boni.MAttack.Value.Show()
        
        self.Boni.MAttack.IncreaseButton = generateinterface.Button()
        self.Boni.MAttack.IncreaseButton.SetParent(self.Boni.MAttack)
        self.Boni.MAttack.IncreaseButton.SetPosition(150 + self.Boni.MAttack.ValueBox.GetWidth() + 2, 3)
        self.Boni.MAttack.IncreaseButton.SetUpVisual("d:/ymir work/age_of_zaria/game/windows/btn_plus_up.sub")
        self.Boni.MAttack.IncreaseButton.SetOverVisual("d:/ymir work/age_of_zaria/game/windows/btn_plus_over.sub")
        self.Boni.MAttack.IncreaseButton.SetDownVisual("d:/ymir work/age_of_zaria/game/windows/btn_plus_down.sub")
        self.Boni.MAttack.IncreaseButton.SetEvent(self.IncreaseBoni, 2)
        self.Boni.MAttack.IncreaseButton.Hide()
        
        ##################################
        ## Magic-Attack-Damage Boni
        ##################################
        self.Boni.Armor = generateinterface.Window()
        self.Boni.Armor.SetParent(self.Boni)
        self.Boni.Armor.SetPosition(0, 30 * 2)
        self.Boni.Armor.SetSize(230, 30)
        self.Boni.Armor.Show()
        
        self.Boni.Armor.Text = generateinterface.TextLine()
        self.Boni.Armor.Text.SetParent(self.Boni.Armor)
        self.Boni.Armor.Text.SetPosition(30, 2)
        self.Boni.Armor.Text.SetDefaultFontName()
        self.Boni.Armor.Text.SetText("Verteidigung")
        self.Boni.Armor.Text.Show()
        
        self.Boni.Armor.ValueBox = generateinterface.ImageBox()
        self.Boni.Armor.ValueBox.SetParent(self.Boni.Armor)
        self.Boni.Armor.ValueBox.SetPosition(150, 0)
        self.Boni.Armor.ValueBox.LoadImage("d:/ymir work/age_of_zaria/public/Parameter_Slot_00.sub")
        self.Boni.Armor.ValueBox.Show()
        
        self.Boni.Armor.Value = generateinterface.TextLine()
        self.Boni.Armor.Value.SetParent(self.Boni.Armor.ValueBox)
        self.Boni.Armor.Value.SetPosition(0, 0)
        self.Boni.Armor.Value.SetWindowHorizontalAlignCenter()
        self.Boni.Armor.Value.SetHorizontalAlignCenter()
        self.Boni.Armor.Value.SetWindowVerticalAlignCenter()
        self.Boni.Armor.Value.SetVerticalAlignCenter()
        self.Boni.Armor.Value.SetText("0")
        self.Boni.Armor.Value.Show()
        
        self.Boni.Armor.IncreaseButton = generateinterface.Button()
        self.Boni.Armor.IncreaseButton.SetParent(self.Boni.Armor)
        self.Boni.Armor.IncreaseButton.SetPosition(150 + self.Boni.Armor.ValueBox.GetWidth() + 2, 3)
        self.Boni.Armor.IncreaseButton.SetUpVisual("d:/ymir work/age_of_zaria/game/windows/btn_plus_up.sub")
        self.Boni.Armor.IncreaseButton.SetOverVisual("d:/ymir work/age_of_zaria/game/windows/btn_plus_over.sub")
        self.Boni.Armor.IncreaseButton.SetDownVisual("d:/ymir work/age_of_zaria/game/windows/btn_plus_down.sub")
        self.Boni.Armor.IncreaseButton.SetEvent(self.IncreaseBoni, 3)
        self.Boni.Armor.IncreaseButton.Hide()
        
        ##################################
        ## EXP Window
        ##################################
        self.expWnd = generateinterface.ImageBox()
        self.expWnd.SetParent(self)
        self.expWnd.SetWindowHorizontalAlignCenter()
        self.expWnd.LoadImage("d:/ymir work/age_of_zaria/pets/pet_exp_gauge.tga")
        self.expWnd.Show()

        self.expWnd.Points = []
        for i in xrange(4):
            self.expWnd.Points.append(generateinterface.ExpandedImageBox())
            self.expWnd.Points[i].SetParent(self.expWnd)
            self.expWnd.Points[i].SetPosition(25+25*i, 9)
            self.expWnd.Points[i].LoadImage("d:/ymir work/age_of_zaria/game/taskbar/exp_gauge_point.sub")
            self.expWnd.Points[i].Show()

    def ClearItemSlot(self, itemSlot):
        self.CLEAR_ITEM_SLOT = itemSlot
        import event
        event.QuestButtonClick(constInfo2.PET_CLEAR_ITEM_SLOT_BUTTON_INDEX)

    def GetClearSlot(self):
        return self.CLEAR_ITEM_SLOT

    def IncreaseBoni(self, boniType):
        self.INCREASE_BONI_TYPE = boniType
        import event
        event.QuestButtonClick(constInfo2.PET_INCREASE_BONI_BUTTON_INDEX)

    def GetIncreaseBoni(self):
        return self.INCREASE_BONI_TYPE

    def ShowItemToolTip(self, slotId):
        if self.SLOT_ITEMS[slotId] > 0:
            self.TOOLTIP_SHOWED = slotId
            self.itemToolTip.ClearToolTip()
            self.itemToolTip.AddItemData(self.SLOT_ITEMS[slotId], [0, 0, 0])

    def HideItemToolTip(self):
        self.TOOLTIP_SHOWED = -1
        self.itemToolTip.HideToolTip()

    def Resize(self):
        if self.skillPoints > 0:
            w, h = self.pet_name.GetTextSize()
            for i in xrange(len(self.slots)):
                self.slots[i].SetPosition(self.board.GetWidth() / 2 - i * 32 - i * 20 + 32, self.title.GetHeight() - 5 + h + 8 + self.pet_image.GetHeight() + 8)
            self.skill_points.SetText(str(self.skillPoints) + " Punkt(e) der Fähigkeit")
            self.skill_points.SetPosition(0, self.title.GetHeight() - 5 + h + 8 + self.pet_image.GetHeight() + 8 + 32 + 15)
            self.skill_points.Show()
            w2, h2 = self.skill_points.GetTextSize()
            self.Boni.SetPosition(0, self.title.GetHeight() - 5 + h + 8 + self.pet_image.GetHeight() + 8 + 32 + 15 + h2 + 5)
        else:
            self.skill_points.Hide()
            w, h = self.pet_name.GetTextSize()
            for i in xrange(len(self.slots)):
                self.slots[i].SetPosition(self.board.GetWidth() / 2 - i * 32 - i * 20 + 32, self.title.GetHeight() - 5 + h + 8 + self.pet_image.GetHeight() + 8)
            self.Boni.SetPosition(0, self.title.GetHeight() - 5 + h + 8 + self.pet_image.GetHeight() + 8 + 32 + 15)
        self.expWnd.SetPosition(0, self.Boni.GetBottom())
        self.board.SetSize(self.board.GetWidth(), self.expWnd.GetBottom() + 3)
        self.SetSize(self.board.GetWidth(), self.expWnd.GetBottom() + 3)

    def __Load(self):
        self.__Load_LoadScript()
        self.__Load_BindObject()
        self.__Load_CreateObject()

    def Show(self):
        self.Resize()
        wndMgr.Show(self.hWnd)

    def SetPet(self, itemVnum):
        self.pet_image.LoadImage("d:/ymir work/age_of_zaria/pets/" + str(itemVnum) + ".tga")

    def SetHeadItem(self, itemVnum):
        self.SLOT_ITEMS[self.SLOT_HEAD] = int(itemVnum)
        item.SelectItem(int(itemVnum))
        self.slots[self.SLOT_HEAD].itemImage.LoadImage(item.GetIconImageFileName())
        self.slots[self.SLOT_HEAD].itemImage.Show()

    def ClearHeadItem(self):
        self.slots[self.SLOT_HEAD].itemImage.Hide()
        if self.TOOLTIP_SHOWED == self.SLOT_HEAD:
            self.HideItemToolTip()

    def SetNeckItem(self, itemVnum):
        self.SLOT_ITEMS[self.SLOT_NECK] = int(itemVnum)
        item.SelectItem(int(itemVnum))
        self.slots[self.SLOT_NECK].itemImage.LoadImage(item.GetIconImageFileName())
        self.slots[self.SLOT_NECK].itemImage.Show()

    def ClearNeckItem(self):
        self.slots[self.SLOT_NECK].itemImage.Hide()
        if self.TOOLTIP_SHOWED == self.SLOT_NECK:
            self.HideItemToolTip()

    def SetFootItem(self, itemVnum):
        self.SLOT_ITEMS[self.SLOT_FOOT] = int(itemVnum)
        item.SelectItem(int(itemVnum))
        self.slots[self.SLOT_FOOT].itemImage.LoadImage(item.GetIconImageFileName())
        self.slots[self.SLOT_FOOT].itemImage.Show()

    def ClearFootItem(self):
        self.slots[self.SLOT_FOOT].itemImage.Hide()
        if self.TOOLTIP_SHOWED == self.SLOT_FOOT:
            self.HideItemToolTip()

    def SetAttackValue(self, value):
        self.Boni.Attack.Value.SetText(str(value))

    def SetMagicAttackValue(self, value):
        self.Boni.MAttack.Value.SetText(str(value))

    def SetArmorValue(self, value):
        self.Boni.Armor.Value.SetText(str(value))

    def SetName(self, name):
        self.name = name
        self.pet_name.SetText(name + " (Lv. " + str(self.level) + ")")

    def SetLevel(self, level):
        self.level = int(level)
        self.pet_name.SetText(self.name + " (Lv. " + str(level) + ")")

    def SetExp(self, ep):
        self.ep = int(ep)

    def SetMaxExp(self, maxep):
        self.maxep = int(maxep)

    def UpdateExpBar(self):
        curPoint = max(self.ep, 0)
        maxPoint = max(self.maxep, 0)

        quarterPoint = maxPoint / 4
        FullCount = 0

        if 0 != quarterPoint:
            FullCount = min(4, curPoint / quarterPoint)

        for i in xrange(4):
            self.expWnd.Points[i].Hide()

        for i in xrange(FullCount):
            self.expWnd.Points[i].SetRenderingRect(0.0, 0.0, 0.0, 0.0)
            self.expWnd.Points[i].Show()

        if 0 != quarterPoint:
            if FullCount < 4:
                Percentage = float(curPoint % quarterPoint) / quarterPoint - 1.0
                self.expWnd.Points[FullCount].SetRenderingRect(0.0, Percentage, 0.0, 0.0)
                self.expWnd.Points[FullCount].Show()

        self.tooltipEXP.SetText("%s : %.2f%%" % (localeInfo.TASKBAR_EXP, float(curPoint) / max(1, float(maxPoint)) * 100))

    def SetSkillPoints(self, points):
        self.skillPoints = int(points)
        if self.skillPoints > 0:
            self.Boni.Attack.IncreaseButton.Show()
            self.Boni.MAttack.IncreaseButton.Show()
            self.Boni.Armor.IncreaseButton.Show()
        else:
            self.Boni.Attack.IncreaseButton.Hide()
            self.Boni.MAttack.IncreaseButton.Hide()
            self.Boni.Armor.IncreaseButton.Hide()
        self.Resize()

    def OnPressEscapeKey(self):
        self.Hide()
        return TRUE

    def Hide(self):
        try:
            self.tooltipEXP.Hide()
            self.HideItemToolTip()
        except:
            pass
        wndMgr.Hide(self.hWnd)

    def OnUpdate(self):
        if self.expWnd.IsIn() or self.expWnd.Points[0].IsIn() or self.expWnd.Points[1].IsIn() or self.expWnd.Points[2].IsIn() or self.expWnd.Points[3].IsIn():
            self.tooltipEXP.Show()
        else:
            self.tooltipEXP.Hide()
