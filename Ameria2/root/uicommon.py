# coding: latin_1

import ui
import localeInfo
import app
import ime
import uiScriptLocale
import uiToolTip
import constInfo

import net, player, item
class DSAssistmentWindow(ui.ScriptWindow):
	WIDTH_WINDOW = 300
	HEIGHT_WINDOW = 290
	
	## CONFIG
	SEC_ROUTINE_TIME = 0.05
	CORE_VNUMS = [50255, 51569, 51549]
	## CONFIG
	
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		
		self.lastTimeRoutine = 0
		self.__LoadDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		self.AddFlag("movable")

		self.Board = ui.BoardWithTitleBar()
		self.Board.SetParent(self)
		self.Board.SetSize(self.WIDTH_WINDOW, self.HEIGHT_WINDOW)
		self.Board.SetTitleName("Dragon Soul Assistment")
		self.Board.SetCloseEvent(ui.__mem_func__(self.Close))
		self.Board.Show()
		
		self.SetSize(self.Board.GetWidth(), self.Board.GetHeight())
		
		self.MakeAutoOpenCoreWindow()
		self.MakeAutoRefineWindow()

	def MakeAutoOpenCoreWindow(self):
		self.HorizontalBarhorizontalBar2 = ui.HorizontalBar()
		self.HorizontalBarhorizontalBar2.Create(self.WIDTH_WINDOW - 20)
		self.HorizontalBarhorizontalBar2.SetParent(self.Board)
		self.HorizontalBarhorizontalBar2.SetPosition(10, 35)
		self.HorizontalBarhorizontalBar2.SetWidth(self.WIDTH_WINDOW - 20)
		self.HorizontalBarhorizontalBar2.Show()
		
		self.HorizontalBarhorizontalBar2.SetText("Auto Open Core")

		self.ThinBoard2 = ui.ThinBoardCircle()
		self.ThinBoard2.SetParent(self)
		self.ThinBoard2.SetSize(self.WIDTH_WINDOW - 19, 85)
		self.ThinBoard2.SetPosition(10, 35 + 19)
		import grp
		self.ThinBoard2.Base.SetColor(grp.GenerateColor(255.0, 255.0, 255.0, 0.5))
		self.ThinBoard2.Show()

		self.checkBoxCoreRough = ui.CheckBox()
		self.checkBoxCoreRough.SetParent(self.ThinBoard2)
		self.checkBoxCoreRough.SetPosition(6, 9)
		self.checkBoxCoreRough.SetCheckStatus(False)
		item.SelectItem(self.CORE_VNUMS[0])
		self.checkBoxCoreRough.SetTextInfo(item.GetItemName() + "|Eitem/51501|e")
		self.checkBoxCoreRough.Show()

		self.checkBoxCoreNoble = ui.CheckBox()
		self.checkBoxCoreNoble.SetParent(self.ThinBoard2)
		self.checkBoxCoreNoble.SetPosition(6, 35)
		self.checkBoxCoreNoble.SetCheckStatus(False)
		item.SelectItem(self.CORE_VNUMS[1])
		self.checkBoxCoreNoble.SetTextInfo(item.GetItemName() + "|Eitem/51569|e")
		self.checkBoxCoreNoble.Show()

		self.checkBoxCoreCut = ui.CheckBox()
		self.checkBoxCoreCut.SetParent(self.ThinBoard2)
		self.checkBoxCoreCut.SetPosition(6, 60)
		self.checkBoxCoreCut.SetCheckStatus(False)
		item.SelectItem(self.CORE_VNUMS[2])
		self.checkBoxCoreCut.SetTextInfo(item.GetItemName() + "|Eitem/51549|e")
		self.checkBoxCoreCut.Show()

	def MakeAutoRefineWindow(self):
		self.HorizontalBarhorizontalBar = ui.HorizontalBar()
		self.HorizontalBarhorizontalBar.Create(self.WIDTH_WINDOW - 20)
		self.HorizontalBarhorizontalBar.SetParent(self.Board)
		self.HorizontalBarhorizontalBar.SetPosition(10, 145)
		self.HorizontalBarhorizontalBar.SetWidth(self.WIDTH_WINDOW - 20)
		self.HorizontalBarhorizontalBar.Show()
		
		self.HorizontalBarhorizontalBar.SetText("Auto Refine Options")

		self.ThinBoard = ui.ThinBoardCircle()
		self.ThinBoard.SetParent(self)
		self.ThinBoard.SetSize(self.WIDTH_WINDOW - 19, 115)
		self.ThinBoard.SetPosition(10, 145 + 19)
		import grp
		self.ThinBoard.Base.SetColor(grp.GenerateColor(255.0, 255.0, 255.0, 0.5))
		self.ThinBoard.Show()

		self.checkBoxDiamond = ui.CheckBox()
		self.checkBoxDiamond.SetParent(self.ThinBoard)
		self.checkBoxDiamond.SetPosition(6, 9)
		self.checkBoxDiamond.SetCheckStatus(False)
		self.checkBoxDiamond.SetTextInfo("Diamond" + "|Eitem/110000|e")
		self.checkBoxDiamond.Show()
	
		self.checkBoxRuby = ui.CheckBox()
		self.checkBoxRuby.SetParent(self.ThinBoard)
		self.checkBoxRuby.SetPosition(110, 9)
		self.checkBoxRuby.SetCheckStatus(False)
		self.checkBoxRuby.SetTextInfo("Ruby" + "|Eitem/120000|e")
		self.checkBoxRuby.Show()

		self.checkBoxJade = ui.CheckBox()
		self.checkBoxJade.SetParent(self.ThinBoard)
		self.checkBoxJade.SetPosition(200, 9)
		self.checkBoxJade.SetCheckStatus(False)
		self.checkBoxJade.SetTextInfo("Jade" + "|Eitem/130000|e")
		self.checkBoxJade.Show()

		# nextLine
		self.checkBoxSapphire = ui.CheckBox()
		self.checkBoxSapphire.SetParent(self.ThinBoard)
		self.checkBoxSapphire.SetPosition(6, 38)
		self.checkBoxSapphire.SetCheckStatus(False)
		self.checkBoxSapphire.SetTextInfo("Sapphire"+ "|Eitem/140000|e")
		self.checkBoxSapphire.Show()

		self.checkBoxGarnet = ui.CheckBox()
		self.checkBoxGarnet.SetParent(self.ThinBoard)
		self.checkBoxGarnet.SetPosition(110, 38)
		self.checkBoxGarnet.SetCheckStatus(False)
		self.checkBoxGarnet.SetTextInfo("Garnet"+ "|Eitem/150000|e")
		self.checkBoxGarnet.Show()

		self.checkBoxOnyx = ui.CheckBox()
		self.checkBoxOnyx.SetParent(self.ThinBoard)
		self.checkBoxOnyx.SetPosition(200, 38)
		self.checkBoxOnyx.SetCheckStatus(False)
		self.checkBoxOnyx.SetTextInfo("Onyx"+ "|Eitem/160000|e")
		self.checkBoxOnyx.Show()

		self.CheckBoxAll = ui.CheckBox()
		self.CheckBoxAll.SetParent(self.ThinBoard)
		self.CheckBoxAll.SetPosition(6, 67)
		self.CheckBoxAll.SetCheckStatus(False)
		self.CheckBoxAll.SetTextInfo("Refine All")
		self.CheckBoxAll.SetEvent(ui.__mem_func__(self.OnRefineAll), "ON_CHECK", True)
		self.CheckBoxAll.SetEvent(ui.__mem_func__(self.OnRefineAll), "ON_UNCKECK", False)
		self.CheckBoxAll.Show()

		self.gradeSlot = ui.ComboBox()
		self.gradeSlot.SetParent(self.ThinBoard)
		self.gradeSlot.SetPosition(0, 90)
		self.gradeSlot.SetSize(150, 18)
		self.gradeSlot.SetWindowHorizontalAlignCenter()
		self.gradeSlot.SetCurrentItem("Mythic")
		self.gradeSlot.InsertItem(0, "Rough")
		self.gradeSlot.InsertItem(1, "Cut")
		self.gradeSlot.InsertItem(2, "Rare")
		self.gradeSlot.InsertItem(3, "Antique")
		self.gradeSlot.InsertItem(4, "Legendary")
		self.gradeSlot.InsertItem(5, "Mythic")
		self.gradeSlot.gradeSelected = 5
		
		self.gradeSlot.SetEvent(ui.__mem_func__(self.OnGradeSlot))
		self.gradeSlot.Show()
	
	def OnRefineAll(self, checkType, state):
		btns = [self.checkBoxDiamond,self.checkBoxRuby,self.checkBoxJade,self.checkBoxSapphire,self.checkBoxGarnet,self.checkBoxOnyx]
	
		for btn in btns:
			if state:
				btn.SetCheckStatus(True)
			else:
				btn.SetCheckStatus(False)
	
	def OnGradeSlot(self, w):
		self.gradeSlot.gradeSelected = w
		self.gradeSlot.SetCurrentItem(self.gradeSlot.listBox.textDict[w])

	def Show(self):
		self.SetCenterPosition()
		self.SetTop()
		ui.ScriptWindow.Show(self)
	
	def Close(self):
		self.Hide()

	def Destroy(self):
		self.Close()
		self.ClearDictionary()
	
	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnIMEReturn(self):
		self.Close()
		return True
	
	def GetEmptySizeInventory(self):
		cells = 0
		for i in xrange(180):
			itemVnum = player.GetItemIndex(i)
			if itemVnum == 0:
				cells += 1
				
		return cells
	
	def IsRefineItem(self, cell, grade, myDict):
		if item.GetItemType() != item.ITEM_TYPE_DS:
			return myDict
		
		CHECKBOX = {
			0 : self.checkBoxDiamond,
			1 : self.checkBoxRuby,
			2 : self.checkBoxJade,
			3 : self.checkBoxSapphire,
			4 : self.checkBoxGarnet,
			5 : self.checkBoxOnyx,
		}
		
		subType = item.GetItemSubType()
		if not CHECKBOX.has_key(subType):
			return myDict
		
		if not CHECKBOX[subType].GetCheckStatus():
			return myDict

		if not myDict.has_key(subType):
			myDict[subType] = {}

		if not myDict[subType].has_key(grade):
			myDict[subType][grade] = []
	
		myDict[subType][grade].append(cell)
		return myDict
	
	def SendUseItemRefine(self, forSubType, grade, myDict):
		interface = constInfo.GetInterfaceInstance()
		if not interface:
			return
		if not interface.wndDragonSoulRefine:
			return
		if not interface.wndDragonSoulRefine.IsShow():
			return
			
		for cell in myDict[forSubType][grade]:
			interface.wndDragonSoulRefine.AutoSetItem((player.DRAGON_SOUL_INVENTORY, cell), 1)

		interface.wndDragonSoulRefine.PressDoRefineButton()
		
	def CheckAutoOpenCore(self):
		cores = { self.CORE_VNUMS[0] : self.checkBoxCoreRough, self.CORE_VNUMS[1] : self.checkBoxCoreNoble, self.CORE_VNUMS[2] : self.checkBoxCoreCut}
		
		dictOpenSlots = {}

		for i in xrange(6*6*32):
			itemVnum = player.GetItemIndex(player.DRAGON_SOUL_INVENTORY, i)
			if itemVnum == 0:
				continue
			
			item.SelectItem(itemVnum)
			if item.GetItemType() != item.ITEM_TYPE_DS:
				continue
			
			grade = itemVnum % 10000 / 1000
			if grade > self.gradeSlot.gradeSelected:
				continue
			
			subType = item.GetItemSubType()
			dictOpenSlots = self.IsRefineItem(i, grade, dictOpenSlots)
			
			if dictOpenSlots.has_key(subType):
				if dictOpenSlots[subType].has_key(grade):
					if len(dictOpenSlots[subType][grade]) >= 2:
						self.SendUseItemRefine(subType, grade, dictOpenSlots)
						break

		emptyCells = self.GetEmptySizeInventory()
		if emptyCells <= 3:
			return

		for i in xrange(180):
			itemVnum = player.GetItemIndex(i)
			if itemVnum == 0:
				continue
			
			for vnumCore, btn in cores.items():
				if btn.GetCheckStatus() and itemVnum == vnumCore:
					net.SendItemUsePacket(i)
	
	def OnRender(self):
		if self.lastTimeRoutine >= app.GetTime():
			return
	
		self.lastTimeRoutine = app.GetTime() + self.SEC_ROUTINE_TIME
	
		self.CheckAutoOpenCore()

class PopupDialog(ui.ScriptWindow):
	#if app.USE_CAPTCHA_SYSTEM:
	def __init__(self, top = 0):
		if top == 1:
			ui.ScriptWindow.__init__(self, "TOP_MOST")
		else:
			ui.ScriptWindow.__init__(self)

		if app.ENABLE_IKASHOP_RENEWAL:
			self.autocloseTime = None
		self.__LoadDialog()
		self.acceptEvent = lambda *arg: None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/PopupDialog.py")

			self.board = self.GetChild("board")
			self.message = self.GetChild("message")
			self.accceptButton = self.GetChild("accept")
			self.accceptButton.SetEvent(ui.__mem_func__(self.Close))

		except:
			import exception
			exception.Abort("PopupDialog.LoadDialog.BindObject")

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		# self.accceptButton.SetFocus() #ADDED THIS LINE
		self.Show()

	def Close(self):
		self.Hide()
		self.acceptEvent()

	def Destroy(self):
		self.Close()
		self.ClearDictionary()

	def SetWidth(self, width):
		height = self.GetHeight()
		self.SetSize(width, height)
		self.board.SetSize(width, height)
		self.SetCenterPosition()
		self.UpdateRect()

	def SetText(self, text):
		self.message.SetText(text)

	def SetAcceptEvent(self, event):
		self.acceptEvent = event
		# self.accceptButton.OnIMEReturn = event #ADDED THIS LINE

	def SetButtonName(self, name):
		self.accceptButton.SetText(name)

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnIMEReturn(self):
		self.Close()
		return True
	if app.ENABLE_IKASHOP_RENEWAL:
		def SetAutoClose(self, seconds):
			self.autocloseTime = app.GetTime() + seconds

		def OnUpdate(self):
			if self.autocloseTime is not None:
				remainTime = max(0, self.autocloseTime - app.GetTime())
				if remainTime == 0:
					self.autocloseTime = None
					self.accceptButton.SetText(uiScriptLocale.OK)
					self.Close()
					return
				self.accceptButton.SetText(uiScriptLocale.OK + " ({:0.1f} s)".format(remainTime))



class InputDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/inputdialog.py")

		getObject = self.GetChild
		self.board = getObject("Board")
		self.acceptButton = getObject("AcceptButton")
		self.cancelButton = getObject("CancelButton")
		self.inputSlot = getObject("InputSlot")
		self.inputValue = getObject("InputValue")

	def Open(self):
		self.inputValue.SetFocus()
		self.SetCenterPosition()
		self.SetTop()
		# self.acceptButton.SetFocus() #ADDED THIS LINE
		self.Show()

	def Close(self):
		self.ClearDictionary()
		self.board = None
		self.acceptButton = None
		self.cancelButton = None
		self.inputSlot = None
		self.inputValue = None
		self.Hide()

	def SetTitle(self, name):
		self.board.SetTitleName(name)

	def SetNumberMode(self):
		self.inputValue.SetNumberMode()

	def SetSecretMode(self):
		self.inputValue.SetSecret()

	def SetFocus(self):
		self.inputValue.SetFocus()

	def SetMaxLength(self, length):
		width = length * 6 + 10
		self.SetBoardWidth(max(width + 50, 160))
		self.SetSlotWidth(width)
		self.inputValue.SetMax(length)

	def SetSlotWidth(self, width):
		self.inputSlot.SetSize(width, self.inputSlot.GetHeight())
		self.inputValue.SetSize(width, self.inputValue.GetHeight())
		if self.IsRTL():
			self.inputValue.SetPosition(self.inputValue.GetWidth(), 0)

	def SetBoardWidth(self, width):
		self.SetSize(max(width + 50, 160), self.GetHeight())
		self.board.SetSize(max(width + 50, 160), self.GetHeight())
		if self.IsRTL():
			self.board.SetPosition(self.board.GetWidth(), 0)
		self.UpdateRect()

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		self.inputValue.OnIMEReturn = event
		# self.acceptButton.OnIMEReturn = event #ADDED THIS LINE

	def SetCancelEvent(self, event):
		self.board.SetCloseEvent(event)
		self.cancelButton.SetEvent(event)
		self.inputValue.OnPressEscapeKey = event

	def GetText(self):
		return self.inputValue.GetText()

class InputDialogWithDescription(InputDialog):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()

	def __del__(self):
		InputDialog.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		if localeInfo.IsARABIC() :
			pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "inputdialogwithdescription.py")
		else:
			pyScrLoader.LoadScriptFile(self, "uiscript/inputdialogwithdescription.py")

		try:
			getObject = self.GetChild
			self.board = getObject("Board")
			self.acceptButton = getObject("AcceptButton")
			self.cancelButton = getObject("CancelButton")
			self.inputSlot = getObject("InputSlot")
			self.inputValue = getObject("InputValue")
			self.description = getObject("Description")

		except:
			import exception
			exception.Abort("InputDialogWithDescription.LoadBoardDialog.BindObject")

	def SetDescription(self, text):
		self.description.SetText(text)

class InputDialogWithDescription2(InputDialog):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()

	def __del__(self):
		InputDialog.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/inputdialogwithdescription2.py")

		try:
			getObject = self.GetChild
			self.board = getObject("Board")
			self.acceptButton = getObject("AcceptButton")
			self.cancelButton = getObject("CancelButton")
			self.inputSlot = getObject("InputSlot")
			self.inputValue = getObject("InputValue")
			self.description1 = getObject("Description1")
			self.description2 = getObject("Description2")

		except:
			import exception
			exception.Abort("InputDialogWithDescription.LoadBoardDialog.BindObject")

	def SetDescription1(self, text):
		self.description1.SetText(text)

	def SetDescription2(self, text):
		self.description2.SetText(text)

class QuestionDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__CreateDialog()
		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.accept_event_func = None
			self.cancel_event_func = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.accept_event_func = None
			self.cancel_event_func = None

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/questiondialog.py")

		self.board = self.GetChild("board")
		self.textLine = self.GetChild("message")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		# self.acceptButton.SetFocus() #ADDED THIS LINE
		self.Show()

	def Close(self):
		self.Hide()

	def SetWidth(self, width):
		height = self.GetHeight()
		self.SetSize(width, height)
		self.board.SetSize(width, height)
		self.SetCenterPosition()
		self.UpdateRect()

	def SAFE_SetAcceptEvent(self, event):
		self.acceptButton.SAFE_SetEvent(event)
		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.accept_event_func = ui.__mem_func__(event)

	def SAFE_SetCancelEvent(self, event):
		self.cancelButton.SAFE_SetEvent(event)
		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.cancel_event_func = ui.__mem_func__(event)

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		# self.acceptButton.OnIMEReturn = event #ADDED THIS LINE
		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.accept_event_func = event

	def SetCancelEvent(self, event):
		self.cancelButton.SetEvent(event)
		if app.ENABLE_GAME_OPTION_ESCAPE:
			self.cancel_event_func = event

	def SetText(self, text):
		self.textLine.SetText(text)

	def SetAcceptText(self, text):
		self.acceptButton.SetText(text)

	def SetCancelText(self, text):
		self.cancelButton.SetText(text)

	def OnPressEscapeKey(self):
		if app.ENABLE_GAME_OPTION_ESCAPE:
			if self.cancel_event_func:
				apply(self.cancel_event_func)
		self.Close()
		return True

class QuestionDialogItem(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.tooltipItem = uiToolTip.ItemToolTip()
		self.slot = None
		self.__CreateDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/questiondialogitem.py")

		self.board = self.GetChild("board")
		self.textLine = self.GetChild("message")
		self.acceptButton = self.GetChild("accept")
		self.destroyButton = self.GetChild("destroy")
		self.cancelButton = self.GetChild("cancel")
		
		self.ItemSlot = self.GetChild("ItemSlot")
		self.ItemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.ItemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		# self.acceptButton.SetFocus() #ADDED THIS LINE
		self.Show()
		height = self.GetHeight()
		width = self.GetWidth()
		self.SetSize(width, height+(32*3)+20)
		self.board.SetSize(width, height+(32*3)+20)
		
		self.acceptButton.SetPosition(-60,63 +(32*3)+10)
		self.destroyButton.SetPosition(0,63 +(32*3)+10)
		self.cancelButton.SetPosition(60,63 +(32*3)+10)
		
	def Close(self):
		self.Hide()

	def SetWidth(self, width):
		height = self.GetHeight()
		self.SetSize(width, height)
		self.board.SetSize(width, height)
		self.SetCenterPosition()
		self.UpdateRect()
		
	def SetItem(self, vnum, attachedItemSlotPos, count):
		self.ItemSlot.SetItemSlot(0,vnum, count)
		self.slot = attachedItemSlotPos

	def OverOutItem(self):
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OverInItem(self, slotIndex=0):
		if None != self.tooltipItem:
			self.tooltipItem.SetInventoryItem(self.slot)

	def SAFE_SetAcceptEvent(self, event):
		self.acceptButton.SAFE_SetEvent(event)

	def SAFE_SetCancelEvent(self, event):
		self.cancelButton.SAFE_SetEvent(event)

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		# self.acceptButton.OnIMEReturn = event #ADDED THIS LINE

	def SetDestroyEvent(self, event):
		self.destroyButton.SetEvent(event)

	def SetCancelEvent(self, event):
		self.cancelButton.SetEvent(event)

	def SetText(self, text):
		self.textLine.SetText(text)

	def SetAcceptText(self, text):
		self.acceptButton.SetText(text)

	def SetCancelText(self, text):
		self.cancelButton.SetText(text)

	def OnPressEscapeKey(self):
		self.Close()
		self.cancelButton.CallEvent()
		return True

class QuestionDialog2(QuestionDialog):

	def __init__(self):
		QuestionDialog.__init__(self)
		self.__CreateDialog()

	def __del__(self):
		QuestionDialog.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/questiondialog2.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def SetText1(self, text):
		self.textLine1.SetText(text)

	def SetText2(self, text):
		self.textLine2.SetText(text)

class QuestionDialogWithTimeLimit(QuestionDialog2):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()
		self.endTime = 0

	def __del__(self):
		QuestionDialog2.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/questiondialog2.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def Open(self, msg, timeout):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

		self.SetText1(msg)
		self.endTime = app.GetTime() + timeout

	def OnUpdate(self):
		leftTime = max(0, self.endTime - app.GetTime())
		self.SetText2(localeInfo.UI_LEFT_TIME % (leftTime))

class MoneyInputDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.moneyHeaderText = localeInfo.MONEY_INPUT_DIALOG_SELLPRICE
		self.__CreateDialog()
		self.SetMaxLength(13)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/moneyinputdialog.py")

		getObject = self.GetChild
		self.board = self.GetChild("board")
		self.acceptButton = getObject("AcceptButton")
		self.cancelButton = getObject("CancelButton")
		self.inputValue = getObject("InputValue")
		#self.inputValue.SetNumberMode()
		self.inputValue.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdate)
		self.moneyText = getObject("MoneyValue")
		if app.ENABLE_IKASHOP_RENEWAL:
			self.inputValue.numberMode = False
			if app.EXTEND_IKASHOP_ULTIMATE:
				self.priceAverage = None

	if app.ENABLE_IKASHOP_RENEWAL:
		if app.EXTEND_IKASHOP_ULTIMATE:
			def SetPriceAverage(self, price):
				if not self.priceAverage:
					self.priceAverage = ui.TextLine()
					self.priceAverage.SetParent(self)
					self.priceAverage.SetPosition(self.GetWidth()/2, 30)
					self.priceAverage.SetHorizontalAlignCenter()
					self.priceAverage.Show()

					for child in self.board.Children:
						cx, cy = child.GetLocalPosition()
						child.SetPosition(cx, cy+15)
					self.SetSize(self.GetWidth(), self.GetHeight() + 15)
					self.board.SetSize(self.board.GetWidth(), self.board.GetHeight() + 15)

				if price == -1:
					self.priceAverage.SetText(localeInfo.IKASHOP_ULTIMATE_PRICE_AVERAGE_REQUESTING)
				elif price == 0:
					self.priceAverage.SetText(localeInfo.IKASHOP_ULTIMATE_PRICE_AVERAGE_NOT_AVAILABLE)
				else:
					self.priceAverage.SetText(localeInfo.IKASHOP_ULTIMATE_PRICE_AVERAGE_VALUE.format(localeInfo.NumberToMoneyString(price)))


	def Open(self):
		self.inputValue.SetText("")
		self.inputValue.SetFocus()
		self.__OnValueUpdate()
		self.SetCenterPosition()
		self.SetTop()
		# self.acceptButton.SetFocus() #ADDED THIS LINE
		self.Show()

	def Close(self):
		self.ClearDictionary()
		self.board = None
		self.acceptButton = None
		self.cancelButton = None
		self.inputValue = None
		if app.ENABLE_IKASHOP_RENEWAL:
			if app.EXTEND_IKASHOP_ULTIMATE:
				if self.priceAverage:
					self.priceAverage.Destroy()
				self.priceAverage = None
		self.Hide()

	def SetTitle(self, name):
		self.board.SetTitleName(name)

	def SetFocus(self):
		self.inputValue.SetFocus()

	def SetMaxLength(self, length):
		length = min(13, length)

		self.inputValue.SetMax(length)
		self.inputValue.SetUserMax(length)

	def SetMoneyHeaderText(self, text):
		self.moneyHeaderText = text

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		self.inputValue.OnIMEReturn = event
		# self.acceptButton.OnIMEReturn = event #ADDED THIS LINE

	def SetCancelEvent(self, event):
		self.board.SetCloseEvent(event)
		self.cancelButton.SetEvent(event)
		self.inputValue.OnPressEscapeKey = event

	def SetValue(self, value):
		if app.ENABLE_IKASHOP_RENEWAL:
			if value == "":
				self.inputValue.SetText("")
				self.__OnValueUpdate()
				ime.SetCursorPosition(0)
				return
		value=str(value)
		self.inputValue.SetText(value)
		self.__OnValueUpdate()
		ime.SetCursorPosition(len(value))		

	if app.ENABLE_IKASHOP_RENEWAL:
		def SoftClose(self):
			self.Hide()
			for child in vars(self).values():
				if isinstance(child, ui.EditLine):
					if child.IsFocus():
						child.KillFocus()

	def GetText(self):
		return self.inputValue.GetText()

	def __OnValueUpdate(self):
		ui.EditLine.OnIMEUpdate(self.inputValue)

		text = self.inputValue.GetText()
		if app.ENABLE_IKASHOP_RENEWAL:
			text = text.lower().replace("k", "000")
		if text:
			text = constInfo.ConvertMoneyText(text)

		for i in xrange(len(text)):
			if not text[i].isdigit():
				text=text[0:i]+text[i+1:]
				self.inputValue.SetText(text)
		self.moneyText.SetText(self.moneyHeaderText + localeInfo.NumberToMoneyString(text))

if app.USE_CURRENCY_EXCHANGE:
	class CurrencyExchangeDialog(ui.ScriptWindow):
		def Open(self, exchangeType, x, y):
			if exchangeType == 0:
				self.board.SetTitleName(uiScriptLocale.CURRENCY_WON_BUY)
			elif exchangeType == 1:
				self.board.SetTitleName(uiScriptLocale.CURRENCY_WON_SELL)
			else:
				return False

			self.exchangeType = exchangeType

			self.inputValue.SetText("")
			self.inputValue.SetFocus()
			ime.SetCursorPosition(-1)

			self.SetPosition(x, y - self.GetHeight())

			self.SetTop()
			self.Show()
			return True

		def Close(self):
			if self.inputValue.IsFocus():
				self.inputValue.KillFocus()

			self.exchangeType = -1

			self.Hide()

		def SetAcceptEvent(self, event):
			self.acceptButton.SetEvent(event)
			self.inputValue.OnIMEReturn = event

		def SetCancelEvent(self, event):
			self.board.SetCloseEvent(event)
			self.cancelButton.SetEvent(event)
			self.inputValue.OnPressEscapeKey = event

		def __OnValueUpdate(self):
			ui.EditLine.OnIMEUpdate(self.inputValue)

		def GetExchangeAmount(self):
			exchangeAmount = 0

			try:
				exchangeAmount = long(self.inputValue.GetText())
				return exchangeAmount
			except:
				return 0

		def __Load(self):
			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, "uiscript/currency_exchange_dialog.py")

				self.board = self.GetChild("board_ui")

				self.inputSlot = self.GetChild("input_slot_ui")
				self.inputValue = self.GetChild("input_value_ui")

				self.acceptButton = self.GetChild("accept_button_ui")
				self.cancelButton = self.GetChild("cancel_button_ui")
			except:
				import exception
				exception.Abort("CurrencyExchangeDialog.Load")

			try:
				self.inputValue.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdate)
			except:
				import exception
				exception.Abort("CurrencyExchangeDialog.Bind")

			self.exchangeType = -1

		def __init__(self):
			ui.ScriptWindow.__init__(self)
			self.__Load()

		def Destroy(self):
			self.Close()

			self.inputValue.OnIMEUpdate = None
			self.inputValue.OnIMEReturn = None
			self.inputValue.OnPressEscapeKey = None

			self.acceptButton.SetEvent(None)
			self.acceptButton = None

			self.cancelButton.SetEvent(None)
			self.cancelButton = None

			self.board = None

			self.ClearDictionary()

		def __del__(self):
			ui.ScriptWindow.__del__(self)

if app.ENABLE_IKASHOP_RENEWAL and app.USE_IKASHOP_UI_RENEWED:
	class PriceInputDialog(ui.ScriptWindow):
		def ShowCheque(self):
			pass

		def HideCheque(self):
			pass

		def SetCheque(self, price):
			pass

		def GetCheque(self):
			return "0"

		def SetValue(self, price):
			pass

		def GetText(self):
			return self.inputValue.GetText()

		if app.EXTEND_IKASHOP_ULTIMATE:
			def SetPriceAverage(self, price):
				if price == -1:
					self.itemPrice = 0
					self.avgPriceButton.Hide()
					self.avgPriceDesc.Hide()

					self.itemAvgPrice.SetText(localeInfo.PRICEINPUT_DLG_AVERAGE_REQUESTING)
				elif price == 0:
					self.itemPrice = 0
					self.avgPriceButton.Hide()
					self.avgPriceDesc.Hide()

					self.itemAvgPrice.SetText(localeInfo.PRICEINPUT_DLG_AVERAGE_NOT_AVAILABLE)
				else:
					self.itemPrice = price

					self.avgPriceButton.Show()
					self.avgPriceDesc.Show()

					self.itemAvgPrice.SetText(localeInfo.PRICEINPUT_DLG_AVERAGE_VALUE.format(localeInfo.NumberWithPoint(price)))

		def SetAcceptEvent(self, event):
			self.acceptButton.SetEvent(event)
			self.inputValue.OnIMEReturn = event

		def SetCancelEvent(self, event):
			self.cancelButton.SetEvent(event)
			self.inputValue.OnPressEscapeKey = event

		def OnOverIn(self):
			if not self.toolTipItem:
				self.toolTipItem = uiToolTip.ItemToolTip()

			self.toolTipItem.ClearToolTip()
			self.toolTipItem.SetInventoryItem(self.itemPos, self.itemWindow)

		def OnOverOut(self):
			if self.toolTipItem:
				self.toolTipItem.HideToolTip()

		def Close(self):
			if self.inputValue.IsFocus():
				self.inputValue.KillFocus()

			self.OnOverOut()
			self.Hide()

		def Open(self, itemWindow, itemPos):
			itemVnum = player.GetItemIndex(itemWindow, itemPos)
			if not itemVnum:
				return

			itemCount = player.GetItemCount(itemWindow, itemPos)
			if not itemCount:
				return

			self.itemWindow = itemWindow
			self.itemPos = itemPos

			self.itemVnum = itemVnum
			self.itemCount = itemCount

			item.SelectItem(itemVnum)

			if itemCount > 1:
				self.itemNameUI.SetText(item.GetItemName() + " (x" + str(itemCount) + ") ")
				self.msgQst.SetText(localeInfo.PRICEINPUT_DLG_DESC2)
			else:
				self.itemNameUI.SetText(item.GetItemName())
				self.msgQst.SetText(localeInfo.PRICEINPUT_DLG_DESC1)

			self.itemIconUI.UnloadImage()
			self.itemIconUI.LoadImage(item.GetIconImageFileName())

			self.finalPrice.SetText(localeInfo.PRICEINPUT_DLG_AVERAGE_VALUE.format(localeInfo.NumberWithPoint(0)))
			self.inputValue.SetText("")
			self.inputValue.SetFocus()
			ime.SetCursorPosition(-1)

			self.avgPriceButton.SetUp()

			self.SetCenterPosition()
			self.SetTop()
			self.Show()

		def OnPressEscapeKey(self):
			self.Close()
			return True

		def OnValueUpdate(self):
			ui.EditLine.OnIMEUpdate(self.inputValue)

			text = self.inputValue.GetText()
			if len(text) == 0:
				self.finalPrice.SetText(localeInfo.PRICEINPUT_DLG_AVERAGE_VALUE.format(localeInfo.NumberWithPoint(0)))
				return

			val = 0
			if text.isdigit():
				try:
					val = min(999999999999, long(text))
				except:
					self.finalPrice.SetText(localeInfo.PRICEINPUT_DLG_AVERAGE_VALUE.format(localeInfo.NumberWithPoint(0)))
					self.inputValue.SetText("")
					ime.SetCursorPosition(-1)
					self.OnValueUpdate()
					return
			else:
				origText = text
				newText = ""
				k = 0

				for l in text:
					if l.isdigit():
						newText += str(l)
						continue

					if l == "k" or l == "K":
						k += 1
						if k <= 4:
							newText += str(l)

						continue

				if newText != origText:
					self.inputValue.SetText(newText)
					ime.SetCursorPosition(-1)
					self.OnValueUpdate()
					return
				else:
					text = text.replace("k", "000")

				try:
					val = min(999999999999, long(text))
				except:
					self.finalPrice.SetText(localeInfo.PRICEINPUT_DLG_AVERAGE_VALUE.format(localeInfo.NumberWithPoint(0)))
					self.inputValue.SetText("")
					ime.SetCursorPosition(-1)
					self.OnValueUpdate()
					return

			self.finalPrice.SetText(localeInfo.PRICEINPUT_DLG_AVERAGE_VALUE.format(localeInfo.NumberWithPoint(val)))

		def OnPressAvgBtn(self, isUp):
			if isUp == False:
				self.inputValue.SetText(str(self.itemPrice))

				if not self.inputValue.IsFocus():
					self.inputValue.SetFocus()

				ime.SetCursorPosition(-1)
				self.OnValueUpdate()

				self.avgPriceButton.Down()
			else:
				self.inputValue.SetText("")

				if not self.inputValue.IsFocus():
					self.inputValue.SetFocus()

				ime.SetCursorPosition(-1)
				self.OnValueUpdate()

				self.avgPriceButton.SetUp()

		def __LoadDialog(self):
			self.itemWindow = -1
			self.itemPos = -1

			self.itemVnum = 0
			self.itemCount = 0

			self.itemPrice = -1

			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, "uiscript/priceinput_dlg.py")

				self.itemNameUI = self.GetChild("item_name_ui")

				self.toolTipItem = None

				self.itemIconUI = self.GetChild("item_icon_ui")
				self.itemIconUI.SetMouseOverInEvent(ui.__mem_func__(self.OnOverIn))
				self.itemIconUI.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverOut))

				self.itemAvgPrice = self.GetChild("average_price_text_ui")

				self.msgQst = self.GetChild("desc_text_ui")

				self.acceptButton = self.GetChild("accept_button_ui")
				self.cancelButton = self.GetChild("cancel_button_ui")

				self.finalPrice = self.GetChild("final_price_value_ui")
				self.finalPrice.SetText(localeInfo.PRICEINPUT_DLG_AVERAGE_VALUE.format(localeInfo.NumberWithPoint(0)))

				self.inputValue = self.GetChild("input_value_ui")
				self.inputValue.SetText("")
				self.inputValue.OnIMEUpdate = ui.__mem_func__(self.OnValueUpdate)
				self.inputValue.OnPressEscapeKey = ui.__mem_func__(self.OnPressEscapeKey)

				self.avgPriceDesc = self.GetChild("bg_avg_desc_ui")

				self.avgPriceButton = self.GetChild("bg_avg_button_ui")
				self.avgPriceButton.SetToggleUpEvent(ui.__mem_func__(self.OnPressAvgBtn), True)
				self.avgPriceButton.SetToggleDownEvent(ui.__mem_func__(self.OnPressAvgBtn), False)
				self.avgPriceButton.Hide()
			except Exception as eMsg:
				import dbg
				dbg.TraceError(str(eMsg))

		def __init__(self):
			ui.ScriptWindow.__init__(self)
			self.__LoadDialog()

		def Destroy(self):
			if self.toolTipItem:
				self.toolTipItem.ClearToolTip()
				del self.toolTipItem
				self.toolTipItem = None

			self.avgPriceDesc = None

			self.avgPriceButton.SetToggleUpEvent(None)
			self.avgPriceButton.SetToggleDownEvent(None)

			self.avgPriceButton = None

			if self.inputValue.IsFocus():
				self.inputValue.KillFocus()

			self.SetAcceptEvent(None)
			self.SetCancelEvent(None)

			self.acceptButton = None
			self.cancelButton = None

			self.inputValue = None

			self.finalPrice = None

			self.itemIconUI.SetMouseOverInEvent(None)
			self.itemIconUI.SetMouseOverOutEvent(None)
			self.itemIconUI = None

			self.itemNameUI = None

			self.itemAvgPrice = None

			self.msgQst = None

			self.itemWindow = -1
			self.itemPos = -1

			self.itemVnum = 0
			self.itemCount = 0

			self.itemPrice = -1

		def __del__(self):
			ui.ScriptWindow.__del__(self)

import item
class ItemQuestDialog(ui.BoardWithTitleBar):
	def __init__(self):
		ui.BoardWithTitleBar.__init__(self)
		self.AddFlag("attach")
		self.AddFlag("float")
		self.AddFlag("movable")
	def Destroy(self):
		self.__children ={}
	def Open(self, itemList):
		self.Destroy()

		window = ui.ThinBoardCircle()
		window.SetParent(self)
		window.SetPosition(10, 35)
		window.Show()
		self.__children["window"] = window

		yPos = 0

		windowWidth = 180

		for itemData in itemList:
			item.SelectItem(itemData[0])

			yPos += 8

			itemImage = ui.ImageBox()
			itemImage.SetParent(window)
			itemImage.SetPosition(10, yPos)
			itemImage.LoadImage(item.GetIconImageFileName())
			itemImage.Show()
			self.__children["icon%d"%yPos] = itemImage

			itemName = ui.TextLine()
			itemName.SetParent(window)
			itemName.SetPosition(10 + 32 + 5, yPos + 8)
			itemName.SetText("x"+str(itemData[1])+" "+item.GetItemName())
			itemName.Show()
			self.__children["name%d"%yPos] = itemName

			if 10 + 32 + 5 + itemName.GetTextSize()[0] + 10 > windowWidth:
				windowWidth = 10 + 32 + 5 + itemName.GetTextSize()[0] + 10

			yPos += itemImage.GetHeight()

		window.SetSize(windowWidth, yPos + 10)
		self.SetSize(10 + window.GetWidth() + 10, 35 + window.GetHeight() + 15)

		acceptBtn = ui.Button()
		acceptBtn.SetParent(self)
		acceptBtn.SetUpVisual("d:/ymir work/ui/public/middle_button_01.sub")
		acceptBtn.SetOverVisual("d:/ymir work/ui/public/middle_button_02.sub")
		acceptBtn.SetDownVisual("d:/ymir work/ui/public/middle_button_03.sub")
		acceptBtn.SetText(localeInfo.YES)
		acceptBtn.Show()
		acceptBtn.SetPosition((self.GetWidth() / 2 ) - (acceptBtn.GetWidth() + 10), window.GetLocalPosition()[1]+ window.GetHeight() + 10)
		self.__children["acceptBtn"] = acceptBtn

		cancelBtn = ui.Button()
		cancelBtn.SetParent(self)
		cancelBtn.SetUpVisual("d:/ymir work/ui/public/middle_button_01.sub")
		cancelBtn.SetOverVisual("d:/ymir work/ui/public/middle_button_02.sub")
		cancelBtn.SetDownVisual("d:/ymir work/ui/public/middle_button_03.sub")
		cancelBtn.SetText(localeInfo.DUNGEONINFO_NO)
		cancelBtn.SetEvent(self.Hide)
		cancelBtn.Show()
		cancelBtn.SetPosition((self.GetWidth() / 2 ) + 10, window.GetLocalPosition()[1]+ window.GetHeight() + 10)
		self.__children["cancelBtn"] = cancelBtn
		self.SetSize(self.GetWidth(), self.GetHeight() + 30)
		self.SetCenterPosition()
		self.Show()
		self.SetTop()
	def SelectEvent(self, func, *arg):
		self.__children["acceptBtn"].SAFE_SetEvent(func, arg)
	def OnPressEscapeKey(self):
		self.Hide()
		self.Destroy()
		return True
