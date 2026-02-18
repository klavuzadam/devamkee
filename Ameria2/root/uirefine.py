# coding: latin_1

import app
import net
import player
import item
import ui
import uiToolTip
import mouseModule
import localeInfo
import uiCommon
import constInfo
import uiScriptLocale
import chr
import ikashop
import chat

if app.ENABLE_PITTY_REFINE:
	ITEM_SEAL_VNUM = 94910
	IS_AUTO_REFINE_SEAL = 0

if constInfo.SHOW_REFINE_ITEM_DESC == True:
	TOOLTIP_DATA = {
		"materials" : [],
		"slot_count" : 0
	}

def kformat(n):
	if n >= 1000000000:
		value = n / 1000000000.0
		suffix = 'B'
	elif n >= 1000000:
		value = n / 1000000.0
		suffix = 'M'
	elif n >= 1000:
		value = n / 1000.0
		suffix = 'k'
	else:
		return str(n)
	
	formatted_value = '{:.2f}'.format(value).rstrip('0').rstrip('.')
	return '{}{}'.format(formatted_value, suffix)

class RefineDialog(ui.ScriptWindow):

	makeSocketSuccessPercentage = ( 100, 33, 20, 15, 10, 5, 0 )
	upgradeStoneSuccessPercentage = ( 30, 29, 28, 27, 26, 25, 24, 23, 22 )
	upgradeArmorSuccessPercentage = ( 99, 66, 33, 33, 33, 33, 33, 33, 33 )
	upgradeAccessorySuccessPercentage = ( 99, 88, 77, 66, 33, 33, 33, 33, 33 )
	upgradeSuccessPercentage = ( 99, 66, 33, 33, 33, 33, 33, 33, 33 )

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadScript()

		self.scrollItemPos = 0
		self.targetItemPos = 0

	def __LoadScript(self):

		self.__LoadQuestionDialog()

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/refinedialog.py")

		except:
			import exception
			exception.Abort("RefineDialog.__LoadScript.LoadObject")

		try:
			self.board = self.GetChild("Board")
			self.titleBar = self.GetChild("TitleBar")
			self.successPercentage = self.GetChild("SuccessPercentage")
			self.GetChild("AcceptButton").SetEvent(self.OpenQuestionDialog)
			self.GetChild("CancelButton").SetEvent(self.Close)
		except:
			import exception
			exception.Abort("RefineDialog.__LoadScript.BindObject")

		## 936 : °³·® È®·ü Ç¥½Ã ¾ÈÇÔ
		##if 936 == app.GetDefaultCodePage():
		self.successPercentage.Show()

		toolTip = uiToolTip.ItemToolTip()
		toolTip.SetParent(self)
		toolTip.SetPosition(15, 38)
		toolTip.SetFollow(FALSE)
		toolTip.Show()
		self.toolTip = toolTip

		if constInfo.SHOW_REFINE_ITEM_DESC == True:
			self.tooltipItem = uiToolTip.ItemToolTip()
			self.tooltipItem.Hide()

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadQuestionDialog(self):
		self.dlgQuestion = ui.ScriptWindow()

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self.dlgQuestion, "uiscript/questiondialog2.py")
		except:
			import exception
			exception.Abort("RefineDialog.__LoadQuestionDialog.LoadScript")

		try:
			GetObject=self.dlgQuestion.GetChild
			GetObject("message1").SetText(localeInfo.REFINE_DESTROY_WARNING)
			GetObject("message2").SetText(localeInfo.REFINE_WARNING2)
			GetObject("accept").SetEvent(ui.__mem_func__(self.Accept))
			GetObject("cancel").SetEvent(ui.__mem_func__(self.dlgQuestion.Hide))
		except:
			import exception
			exception.Abort("SelectCharacterWindow.__LoadQuestionDialog.BindObject")

	def Destroy(self):
		self.ClearDictionary()
		self.board = 0
		self.successPercentage = 0
		self.titleBar = 0
		self.toolTip = 0
		self.dlgQuestion = 0

	def GetRefineSuccessPercentage(self, scrollSlotIndex, itemSlotIndex):

		if -1 != scrollSlotIndex:
			if player.IsRefineGradeScroll(scrollSlotIndex):
				curGrade = player.GetItemGrade(itemSlotIndex)
				itemIndex = player.GetItemIndex(itemSlotIndex)

				item.SelectItem(itemIndex)
				itemType = item.GetItemType()
				itemSubType = item.GetItemSubType()

				if item.ITEM_TYPE_METIN == itemType:

					if curGrade >= len(self.upgradeStoneSuccessPercentage):
						return 0
					return self.upgradeStoneSuccessPercentage[curGrade]

				elif item.ITEM_TYPE_ARMOR == itemType:

					if item.ARMOR_BODY == itemSubType:
						if curGrade >= len(self.upgradeArmorSuccessPercentage):
							return 0
						return self.upgradeArmorSuccessPercentage[curGrade]
					else:
						if curGrade >= len(self.upgradeAccessorySuccessPercentage):
							return 0
						return self.upgradeAccessorySuccessPercentage[curGrade]

				else:

					if curGrade >= len(self.upgradeSuccessPercentage):
						return 0
					return self.upgradeSuccessPercentage[curGrade]

		for i in xrange(player.METIN_SOCKET_MAX_NUM+1):
			if 0 == player.GetItemMetinSocket(itemSlotIndex, i):
				break

		return self.makeSocketSuccessPercentage[i]

	def Open(self, scrollItemPos, targetItemPos):
		self.scrollItemPos = scrollItemPos
		self.targetItemPos = targetItemPos

		percentage = self.GetRefineSuccessPercentage(scrollItemPos, targetItemPos)
		if 0 == percentage:
			return
		self.successPercentage.SetText(localeInfo.REFINE_SUCCESS_PROBALITY)

		itemIndex = player.GetItemIndex(targetItemPos)
		self.toolTip.ClearToolTip()

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(targetItemPos, i))
		self.toolTip.AddItemData(itemIndex, metinSlot)

		self.UpdateDialog()
		self.SetTop()
		self.Show()

	def UpdateDialog(self):
		newWidth = self.toolTip.GetWidth() + 30
		newHeight = self.toolTip.GetHeight() + 98
		self.board.SetSize(newWidth, newHeight)
		self.titleBar.SetWidth(newWidth-15)
		self.SetSize(newWidth, newHeight)

		(x, y) = self.GetLocalPosition()
		self.SetPosition(x, y)

	def OpenQuestionDialog(self):
		percentage = self.GetRefineSuccessPercentage(-1, self.targetItemPos)
		if 100 == percentage:
			self.Accept()
			return

		self.dlgQuestion.SetTop()
		self.dlgQuestion.Show()

	def Accept(self):
		net.SendItemUseToItemPacket(self.scrollItemPos, self.targetItemPos)
		self.Close()

	def Close(self):
		self.dlgQuestion.Hide()
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

class RefineDialogNew(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.isLoaded = False

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.wndInventory = None

	def __Initialize(self):
		self.dlgQuestion = None
		self.interface = None
		self.children = []
		self.vnum = 0
		self.targetItemPos = 0
		self.dialogHeight = 0
		self.cost = 0
		self.percentage = 0
		self.type = 0
		self.special_storage = 0

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.lockedItem = (-1,-1)

	def __LoadScript(self):

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/refinedialog.py")

		except:
			import exception
			exception.Abort("RefineDialog.__LoadScript.LoadObject")

		try:
			self.board = self.GetChild("Board")
			self.titleBar = self.GetChild("TitleBar")

			self.costText = self.GetChild("Cost")

			if app.ENABLE_PITTY_REFINE:
				self.GetChild("SuccessPercentage").Hide()
			else:
				self.probText = self.GetChild("SuccessPercentage")

			self.GetChild("AcceptButton").SetEvent(self.OpenQuestionDialog)
			self.GetChild("CancelButton").SetEvent(self.CancelRefine)
		except:
			import exception
			exception.Abort("RefineDialog.__LoadScript.BindObject")

		toolTip = uiToolTip.ItemToolTip()
		toolTip.SetParent(self)
		toolTip.SetFollow(False)
		toolTip.SetPosition(15, 38)
		toolTip.Show()
		self.toolTip = toolTip

		self.slotList = []
		for i in xrange(3):
			slot = self.__MakeSlot()
			slot.SetParent(toolTip)
			slot.SetWindowVerticalAlignCenter()
			self.slotList.append(slot)

		itemImage = self.__MakeItemImage()
		itemImage.SetParent(toolTip)
		itemImage.SetWindowVerticalAlignCenter()
		itemImage.SetPosition(-35, 0)
		self.itemImage = itemImage

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.CancelRefine))

		if constInfo.SHOW_REFINE_ITEM_DESC == True:
			self.tooltipItem = uiToolTip.ItemToolTip()
			self.tooltipItem.Hide()

		if app.ENABLE_REFINE_RENEWAL:
			self.checkBox = ui.CheckBox()
			self.checkBox.SetParent(self)
			self.checkBox.SetPosition(-3, 83+8)
			self.checkBox.SetWindowHorizontalAlignCenter()
			self.checkBox.SetWindowVerticalAlignBottom()
			self.checkBox.SetEvent(ui.__mem_func__(self.AutoRefine), "ON_CHECK", True)
			self.checkBox.SetEvent(ui.__mem_func__(self.AutoRefine), "ON_UNCKECK", False)
			self.checkBox.SetCheckStatus(constInfo.IS_AUTO_REFINE)
			self.checkBox.SetTextInfo(uiScriptLocale.UPGRADE)

			if not app.ENABLE_PITTY_REFINE:
				self.checkBox.Show()

		if app.ENABLE_PITTY_REFINE:
			self.MakePittyInfoWindow()

		self.isLoaded = True

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __MakeSlot(self):
		slot = ui.ImageBox()
		slot.LoadImage("d:/ymir work/ui/public/slot_base.sub")
		slot.Show()
		self.children.append(slot)
		return slot

	def __MakeItemImage(self):
		itemImage = ui.ImageBox()
		itemImage.Show()
		self.children.append(itemImage)
		return itemImage

	def __MakeThinBoard(self):
		thinBoard = ui.ThinBoard()
		thinBoard.SetParent(self)
		thinBoard.Show()
		self.children.append(thinBoard)
		return thinBoard

	def Destroy(self):
		self.dlgQuestion = None
		self.board = 0
		self.probText = 0
		self.costText = 0
		self.titleBar = 0
		self.toolTip = 0
		self.successPercentage = None
		self.interface = None
		self.special_storage = 0
		self.slotList = []
		self.children = []

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.wndInventory = None
			self.lockedItem = (-1, -1)

		self.ClearDictionary()

	if app.ENABLE_REFINE_RENEWAL:
		def __InitializeOpen(self):
			self.children = []
			self.vnum = 0
			self.targetItemPos = 0
			self.dialogHeight = 0
			self.cost = 0
			self.percentage = 0
			self.type = 0
			self.xRefineStart = 0
			self.yRefineStart = 0

	def Open(self, targetItemPos, nextGradeItemVnum, cost, prob, special_storage, type):

		if False == self.isLoaded:
			self.__LoadScript()

		if app.ENABLE_REFINE_RENEWAL:
			self.__InitializeOpen()
		else:
			self.__Initialize()

		self.targetItemPos = targetItemPos
		self.vnum = nextGradeItemVnum
		self.cost = cost
		self.percentage = prob
		self.type = type
		self.special_storage = int(special_storage)

		if app.ENABLE_PITTY_REFINE:
			if player.GetElk() >= self.cost:
				self.costText.SetPackedFontColor(0xFF35dDE3D)
			else:
				self.costText.SetPackedFontColor(0xFFFF0033)

			self.costText.SetText(kformat(self.cost))

			self.wndTextPitty.SetText(localeInfo.REFINE_SUCCESS_PROBALITY)
		else:
			self.costText.SetText(localeInfo.REFINE_COST % (self.cost))
			self.probText.SetText(localeInfo.REFINE_SUCCESS_PROBALITY)

		self.toolTip.ClearToolTip()

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.SetCantMouseEventSlot(targetItemPos)

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(targetItemPos, i))

		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(targetItemPos, i))
		self.toolTip.AddRefineItemData(nextGradeItemVnum, metinSlot, attrSlot)

		item.SelectItem(nextGradeItemVnum)
		self.itemImage.LoadImage(item.GetIconImageFileName())
		xSlotCount, ySlotCount = item.GetItemSize()
		for slot in self.slotList:
			slot.Hide()
		for i in xrange(min(3, ySlotCount)):
			self.slotList[i].SetPosition(-35, i*32 - (ySlotCount-1)*16)
			self.slotList[i].Show()

		if app.ENABLE_REFINE_RENEWAL:
			if constInfo.AUTO_REFINE_TYPE == 2 and chr.GetVirtualNumber(constInfo.AUTO_REFINE_DATA["NPC"][0]) == 20091:
				constInfo.IS_AUTO_REFINE = False
				self.checkBox.Hide()
			else:
				self.checkBox.Show()

		if app.ENABLE_PITTY_REFINE:
			self.boardPittyThin.Hide()
			self.boardPitty.SetSize(self.boardPitty.GetWidth(), 91)
			self.boardPittyBG.SetPosition(self.boardPittyBG.GetLocalPosition()[0], 0)

		self.dialogHeight = self.toolTip.GetHeight() + 46
		self.UpdateDialog()

		self.SetTop()
		self.Show()

	if app.ENABLE_PITTY_REFINE:
		def MakePittyInfoWindow(self):
			self.boardPitty = ui.Window()
			self.boardPitty.SetParent(self)
			self.boardPitty.SetSize(261, 91 + 44 + 8)
			self.boardPitty.Show()

			self.boardPittyThin = ui.ThinBoard()
			self.boardPittyThin.SetParent(self.boardPitty)
			self.boardPittyThin.SetSize(self.boardPitty.GetWidth() - 6, 44)
			self.boardPittyThin.SetPosition(3, 0)
			self.boardPittyThin.Show()

			self.boardPittyTitle = ui.TextLine()
			self.boardPittyTitle.SetParent(self.boardPittyThin)
			self.boardPittyTitle.SetWindowHorizontalAlignCenter()
			self.boardPittyTitle.SetHorizontalAlignCenter()
			self.boardPittyTitle.SetPosition(10, 8 + 1)
			self.boardPittyTitle.SetText("")
			self.boardPittyTitle.Show()

			self.btnMark = ui.MakeButton(self.boardPittyThin, 0, 0, "", "d:/ymir work/ui/pattern/", "q_mark_01.tga", "q_mark_02.tga", "q_mark_01.tga")
			self.btnMark.SetWindowHorizontalAlignCenter()
			self.btnMark.SetPosition(-40, 8)
			self.btnMark.SetEventOverIn(ui.__mem_func__(self.OverInMarkPitty))
			self.btnMark.SetEventOverOut(ui.__mem_func__(self.OverOutItemPitty))

			self.boardPittyGauge = ui.Gauge()
			self.boardPittyGauge.SetParent(self.boardPittyThin)
			self.boardPittyGauge.SLOT_HEIGHT = 12
			self.boardPittyGauge.MakeGauge(self.boardPittyThin.GetWidth() - 30, "blue")
			self.boardPittyGauge.SetPosition(15, 8 + 20)
			self.boardPittyGauge.Show()

			self.boardPittyBG = ui.ImageBox()
			self.boardPittyBG.SetParent(self.boardPitty)
			self.boardPittyBG.LoadImage("d:/ymir work/ui/refine_ui_new.png")
			self.boardPittyBG.SetPosition(0, 40 + 8)
			self.boardPittyBG.Show()

			self.imageSeal = ui.ImageBox()
			self.imageSeal.SetParent(self.boardPittyBG)
			self.imageSeal.SetPosition(6, 9)
			self.imageSeal.Show()

			if player.GetItemCountByVnum(ITEM_SEAL_VNUM) > 0:
				item.SelectItem(ITEM_SEAL_VNUM)
				self.imageSeal.LoadImage(item.GetIconImageFileName())
			else:
				self.imageSeal.Hide()

			self.wndTextPittyTitle = ui.MakeTextLine(self.boardPittyBG)
			self.wndTextPittyTitle.SetText(localeInfo.REFINE_INCRESE)
			self.wndTextPittyTitle.SetPosition(3, -36)

			self.checkBoxUseSeal = ui.CheckBox()
			self.checkBoxUseSeal.SetParent(self.boardPittyBG)
			self.checkBoxUseSeal.SetPosition(45, 20)
			self.checkBoxUseSeal.SetCheckStatus(0)
			self.checkBoxUseSeal.SetEvent(ui.__mem_func__(self.CheckBoxSeal), "ON_CHECK", True)
			self.checkBoxUseSeal.SetEvent(ui.__mem_func__(self.CheckBoxSeal), "ON_UNCKECK", False)
			self.checkBoxUseSeal.SetTextInfo(localeInfo.REFINE_INCRESE2)
			self.checkBoxUseSeal.Show()

			self.checkBoxUseSealAlways = ui.CheckBox()
			self.checkBoxUseSealAlways.SetParent(self.boardPittyBG)
			self.checkBoxUseSealAlways.SetPosition(45, 35)

			global IS_AUTO_REFINE_SEAL
			self.checkBoxUseSealAlways.SetCheckStatus(IS_AUTO_REFINE_SEAL)

			self.checkBoxUseSealAlways.SetEvent(ui.__mem_func__(self.CheckBoxSealAlways), "ON_CHECK", True)
			self.checkBoxUseSealAlways.SetEvent(ui.__mem_func__(self.CheckBoxSealAlways), "ON_UNCKECK", False)
			self.checkBoxUseSealAlways.SetTextInfo(localeInfo.REFINE_INCRESE3)
			self.checkBoxUseSealAlways.Show()

			self.costText.SetParent(self.boardPittyBG)
			self.costText.SetPosition(95, 19)

			if app.ENABLE_REFINE_RENEWAL:
				self.checkBox.SetParent(self.boardPittyBG)
				self.checkBox.SetPosition(0, 38)

			self.wndTextPitty = ui.MakeTextLine(self.boardPittyBG)
			self.wndTextPitty.SetText("100%")
			self.wndTextPitty.SetPosition(-42, 33)
			self.wndTextPitty.SetPackedFontColor(0xffF2E7C1)

		def CheckBoxSeal(self, checkType, autoFlag):
			self.checkBoxUseSealAlways.SetCheckStatus(0)

			global IS_AUTO_REFINE_SEAL
			IS_AUTO_REFINE_SEAL = False

			if autoFlag and player.GetItemCountByVnum(ITEM_SEAL_VNUM) == 0:
				self.checkBoxUseSeal.SetCheckStatus(0)
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_NO_SOULD_OF_GOD_ALERT)

		def CheckBoxSealAlways(self, checkType, autoFlag):
			self.checkBoxUseSeal.SetCheckStatus(0)

			global IS_AUTO_REFINE_SEAL
			IS_AUTO_REFINE_SEAL = autoFlag

			if autoFlag and player.GetItemCountByVnum(ITEM_SEAL_VNUM) == 0:
				self.checkBoxUseSealAlways.SetCheckStatus(0)
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_NO_SOULD_OF_GOD_ALERT)

		def SetPittyInfo(self, forVnum, pittyCurrent, pittyMax):
			if player.GetItemIndex(self.targetItemPos) != forVnum:
				self.boardPitty.Hide()
				self.UpdateDialog()
				return

			if pittyMax == 0:
				self.boardPittyThin.Hide()
				self.boardPitty.SetSize(self.boardPitty.GetWidth(), 91)
				self.boardPittyBG.SetPosition(self.boardPittyBG.GetLocalPosition()[0], 0)
			else:
				self.boardPittyTitle.SetText(localeInfo.PITTY_SYSTEM_NAME % (pittyCurrent, pittyMax))
				self.boardPittyGauge.SetPercentage(pittyCurrent, pittyMax)

				self.boardPitty.SetSize(261, 91 + 44 + 8)
				self.boardPittyBG.SetPosition(self.boardPittyBG.GetLocalPosition()[0], 40 + 8)
				self.boardPittyThin.Show()

			if pittyCurrent == pittyMax:
				self.wndTextPitty.SetText("100%")
				self.percentage = 100
			else:
				self.wndTextPitty.SetText(localeInfo.REFINE_SUCCESS_PROBALITY)

			self.boardPitty.Show()
			self.UpdateDialog()

		def OverInMarkPitty(self):
			interface = constInfo.GetInterfaceInstance()

			if interface.tooltipItem:
				interface.tooltipItem.ClearToolTip()
				interface.tooltipItem.SetCannotUseItemForceSetDisableColor(False)

				interface.tooltipItem.SetTitle("Pity System")
				interface.tooltipItem.AppendSpace(5)
				interface.tooltipItem.AppendDescription(localeInfo.REFINE_INFO_PITTY_DESC1, 26)
				interface.tooltipItem.AppendHorizontalLine()
				interface.tooltipItem.AppendDescription(localeInfo.REFINE_INFO_PITTY_DESC2, 26)
				interface.tooltipItem.ShowToolTip()

		def OverOutItemPitty(self):
			interface = constInfo.GetInterfaceInstance()
			if not interface:
				return

			if interface.tooltipItem:
				interface.tooltipItem.HideToolTip()

	def Close(self):
		self.dlgQuestion = None
		self.Hide()

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.lockedItem = (-1, -1)
			self.SetCanMouseEventSlot(self.targetItemPos)

		if constInfo.SHOW_REFINE_ITEM_DESC:
			TOOLTIP_DATA["materials"] = []

	if constInfo.SHOW_REFINE_ITEM_DESC == True:
		def __MakeItemSlot(self, slotIndex):
			slot = ui.SlotWindow()
			slot.SetParent(self)
			slot.SetSize(32, 32)
			slot.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)
			slot.AppendSlot(slotIndex, 0, 0, 32, 32)
			slot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			slot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
			slot.SetSelectItemSlotEvent(ui.__mem_func__(self.UseMaterialSearch))
			slot.RefreshSlot()
			slot.Show()
			self.children.append(slot)
			return slot

		def BindInterface(self, interface):
			self.interface = interface

		def UseMaterialSearch(self, slotIndex):
			if slotIndex > len(TOOLTIP_DATA['materials']):
				return

			if app.IsPressed(app.DIK_LALT) and app.ENABLE_IKASHOP_RENEWAL:
				item.SelectItem(TOOLTIP_DATA['materials'][slotIndex])
				if app.USE_OFFLINESHOP_SEARCH_V2:
					if self.interface:
						self.interface.ShopSearchFastItem(TOOLTIP_DATA['materials'][slotIndex], item.GetItemName())
				else:
					board = ikashop.GetSearchShopBoard()
					if board:
						board.FastSearchByItem(slotIndex)

				return

		def OverInItem(self, slotIndex):
			if slotIndex > len(TOOLTIP_DATA['materials']):
				return

			if self.tooltipItem:
				self.tooltipItem.ClearToolTip()
				self.tooltipItem.AddItemData(TOOLTIP_DATA['materials'][slotIndex], 0, 0, 0, 0, player.INVENTORY)
				self.tooltipItem.AppendSpace(7)
				self.tooltipItem.SearchShopKeyBind()
				self.tooltipItem.ShowToolTip()

		def OverOutItem(self):
			if self.tooltipItem:
				self.tooltipItem.HideToolTip()

		def AppendMaterial(self, vnum, count):
			if constInfo.SHOW_REFINE_ITEM_DESC == True:
				slotIndex = len(TOOLTIP_DATA['materials'])

				slot = self.__MakeItemSlot(slotIndex)
				slot.SetPosition(15, self.dialogHeight)
				slot.SetItemSlot(slotIndex, vnum, 0)#count)

				TOOLTIP_DATA['materials'].append(vnum)
			else:
				slot = self.__MakeSlot()
				slot.SetParent(self)
				slot.SetPosition(15, self.dialogHeight)

				itemImage = self.__MakeItemImage()
				itemImage.SetParent(slot)
				item.SelectItem(vnum)
				itemImage.LoadImage(item.GetIconImageFileName())

			thinBoard = self.__MakeThinBoard()
			thinBoard.SetPosition(50, self.dialogHeight)
			thinBoard.SetSize(191, 20)

			textLine = ui.TextLine()
			textLine.SetParent(thinBoard)
			textLine.SetFontName(localeInfo.UI_DEF_FONT)

			itemCount = player.GetItemCountByVnum(vnum)

			if itemCount >= count:
				textLine.SetPackedFontColor(0xFF35dDE3D)
			else:
				textLine.SetPackedFontColor(0xFFFF0033)

			textLine.SetText("%s x%d  |cFFffce00(%d)" % (item.GetItemName(), count, itemCount))

			textLine.SetOutline()
			textLine.SetFeather(False)
			textLine.SetWindowVerticalAlignCenter()
			textLine.SetVerticalAlignCenter()

			if localeInfo.IsARABIC():
				(x,y) = textLine.GetTextSize()
				textLine.SetPosition(x, 0)
			else:
				textLine.SetPosition(15, 0)

			textLine.Show()
			self.children.append(textLine)

			self.dialogHeight += 34
			self.UpdateDialog()

	def UpdateDialog(self):
		newWidth = self.toolTip.GetWidth() + 60
		newHeight = self.dialogHeight + 100+10

		## 936 : °³·® È®·ü Ç¥½Ã ¾ÈÇÔ
		##if 936 == app.GetDefaultCodePage():
		newHeight -= 8

		if localeInfo.IsARABIC():
			self.board.SetPosition( newWidth, 0 )

			(x, y) = self.titleBar.GetLocalPosition()
			self.titleBar.SetPosition( newWidth - 15, y )

		if app.ENABLE_PITTY_REFINE:
			if self.boardPitty.IsShow():
				self.boardPitty.SetPosition(6, newHeight - 87 - 6)

				newHeight += self.boardPitty.GetHeight() - 50
				if newWidth < self.boardPitty.GetWidth() + 13:
					newWidth = self.boardPitty.GetWidth() + 13

		self.board.SetSize(newWidth, newHeight)
		self.toolTip.SetPosition(15 + 35, 38)
		self.titleBar.SetWidth(newWidth-15)
		self.SetSize(newWidth, newHeight)

		(x, y) = self.GetLocalPosition()
		self.SetPosition(x, y)

	def OpenQuestionDialog(self):
		if 100 == self.percentage:
			self.Accept()
			return

		if 5 == self.type: ## ¹«½ÅÀÇ Ãàº¹¼­
			self.Accept()
			return

		dlgQuestion = uiCommon.QuestionDialog2()
		dlgQuestion.SetText2(localeInfo.REFINE_WARNING2)
		dlgQuestion.SetAcceptEvent(ui.__mem_func__(self.Accept))
		dlgQuestion.SetCancelEvent(ui.__mem_func__(dlgQuestion.Close))

		if 3 == self.type: ## ÇöÃ¶
			dlgQuestion.SetText1(localeInfo.REFINE_DESTROY_WARNING_WITH_BONUS_PERCENT_1)
			dlgQuestion.SetText2(localeInfo.REFINE_DESTROY_WARNING_WITH_BONUS_PERCENT_2)
		elif 2 == self.type: ## Ãàº¹¼­
			dlgQuestion.SetText1(localeInfo.REFINE_DOWN_GRADE_WARNING)
		else:
			dlgQuestion.SetText1(localeInfo.REFINE_DESTROY_WARNING)

		dlgQuestion.Open()
		self.dlgQuestion = dlgQuestion

	def Accept(self):
		if app.ENABLE_PITTY_REFINE:
			bUseSealOfGod = True if (self.checkBoxUseSeal.GetCheckStatus() or self.checkBoxUseSealAlways.GetCheckStatus()) else False
			net.SendRefinePacket(self.targetItemPos, self.type, self.special_storage, bUseSealOfGod)
		else:
			net.SendRefinePacket(self.targetItemPos, self.type, self.special_storage)

		if not app.ENABLE_REFINE_RENEWAL:
			self.Close()

	if app.ENABLE_REFINE_RENEWAL:
		def AutoRefine(self, checkType, autoFlag):
			constInfo.IS_AUTO_REFINE = autoFlag
		
		def CheckRefine(self, isFail):
			if constInfo.IS_AUTO_REFINE == True:
				if constInfo.AUTO_REFINE_TYPE == 1:
					if constInfo.AUTO_REFINE_DATA["ITEM"][0] != -1 and constInfo.AUTO_REFINE_DATA["ITEM"][1] != -1:
						scrollIndex = player.GetItemIndex(constInfo.AUTO_REFINE_DATA["ITEM"][0])
						itemIndex = player.GetItemIndex(constInfo.AUTO_REFINE_DATA["ITEM"][1])
						
						#chat.AppendChat(1,"test 2")
						# chat.AppendChat(chat.CHAT_TYPE_INFO, "%d %d" % (itemIndex, int(itemIndex %10)))
						if scrollIndex == 0 or (itemIndex % 10 == 8 and not isFail):
							#chat.AppendChat(1,"test 3")
							self.Close()
						else:
							net.SendItemUseToItemPacket(constInfo.AUTO_REFINE_DATA["ITEM"][0], constInfo.AUTO_REFINE_DATA["ITEM"][1])
				elif constInfo.AUTO_REFINE_TYPE == 2:
					npcData = constInfo.AUTO_REFINE_DATA["NPC"]					
					if npcData[0] != 0 and npcData[1] != -1 and npcData[2] != -1 and npcData[3] != 0:
						itemIndex = player.GetItemIndex(npcData[1], npcData[2])
						if (itemIndex % 10 == 8 and not isFail) or isFail:
							self.Close()
						else:
							net.SendGiveItemPacket(npcData[0], npcData[1], npcData[2], npcData[3])
				else:
					#chat.AppendChat(1,"test 4")
					self.Close()
			else:
				#chat.AppendChat(1,"test 5")
				self.Close()

	def CancelRefine(self):
		if app.ENABLE_PITTY_REFINE:
			net.SendRefinePacket(255, 255, 255, False)
		else:
			net.SendRefinePacket(255, 255, 255)

		self.Close()

		if app.ENABLE_REFINE_RENEWAL:
			constInfo.AUTO_REFINE_TYPE = 0
			constInfo.AUTO_REFINE_DATA = {
				"ITEM" : [-1, -1],
				"NPC" : [0, -1, -1, 0]
			}

		if constInfo.SHOW_REFINE_ITEM_DESC:
			TOOLTIP_DATA["materials"] = []

	if app.WJ_ENABLE_TRADABLE_ICON:
		def SetCanMouseEventSlot(self, slotIndex):
			try:
				itemInvenPage = slotIndex / player.INVENTORY_PAGE_SIZE
				localSlotPos = slotIndex - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
				self.lockedItem = (-1, -1)

				if itemInvenPage == self.wndInventory.GetInventoryPageIndex():
					self.wndInventory.wndItem.SetCanMouseEventSlot(localSlotPos)
			except Exception as e:
				pass

		def SetCantMouseEventSlot(self, slotIndex):
			itemInvenPage = slotIndex / player.INVENTORY_PAGE_SIZE
			localSlotPos = slotIndex - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
			self.lockedItem = (itemInvenPage, localSlotPos)

			if itemInvenPage == self.wndInventory.GetInventoryPageIndex():
				self.wndInventory.wndItem.SetCantMouseEventSlot(localSlotPos)

		def SetInven(self, wndInventory):
			from _weakref import proxy
			self.wndInventory = proxy(wndInventory)

		def RefreshLockedSlot(self):
			if self.wndInventory:
				itemInvenPage, itemSlotPos = self.lockedItem
				if self.wndInventory.GetInventoryPageIndex() == itemInvenPage:
					self.wndInventory.wndItem.SetCantMouseEventSlot(itemSlotPos)

				self.wndInventory.wndItem.RefreshSlot()

	def OnPressEscapeKey(self):
		self.CancelRefine()
		return True
