# coding: utf-8

import app
import wndMgr

import ui
import ime
import localeInfo
import constInfo

class PickMoneyDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.unitValue = 1
		self.maxValue = 0
		self.eventAccept = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PickMoneyDialog.py")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.LoadScript")

		try:
			self.board = self.GetChild("board")
			self.maxValueTextLine = self.GetChild("max_value")
			self.pickValueEditLine = self.GetChild("money_value")
			self.acceptButton = self.GetChild("accept_button")
			self.cancelButton = self.GetChild("cancel_button")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.BindObject")

		self.pickValueEditLine.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		self.pickValueEditLine.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.acceptButton.SetEvent(ui.__mem_func__(self.OnAccept))
		self.cancelButton.SetEvent(ui.__mem_func__(self.Close))
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))

	def Destroy(self):
		self.ClearDictionary()
		self.eventAccept = 0
		self.maxValue = 0
		self.pickValueEditLine = 0
		self.acceptButton = 0
		self.cancelButton = 0
		self.board = None

	def SetTitleName(self, text):
		self.board.SetTitleName(text)

	def SetAcceptEvent(self, event):
		self.eventAccept = event

	def SetMax(self, max):
		self.pickValueEditLine.SetMax(max)

	def Open(self, maxValue, unitValue=1):

		if localeInfo.IsYMIR() or localeInfo.IsCHEONMA() or localeInfo.IsHONGKONG():
			unitValue = ""

		width = self.GetWidth()
		(mouseX, mouseY) = wndMgr.GetMousePosition()

		if mouseX + width/2 > wndMgr.GetScreenWidth():
			xPos = wndMgr.GetScreenWidth() - width
		elif mouseX - width/2 < 0:
			xPos = 0
		else:
			xPos = mouseX - width/2

		self.SetPosition(xPos, mouseY - self.GetHeight() - 20)

		if localeInfo.IsARABIC():
			self.maxValueTextLine.SetText("/" + str(maxValue))
		else:
			self.maxValueTextLine.SetText(" / " + str(maxValue))

		self.pickValueEditLine.SetText(str(unitValue))
		self.pickValueEditLine.SetFocus()

		ime.SetCursorPosition(1)

		self.unitValue = unitValue
		self.maxValue = maxValue
		self.Show()
		self.SetTop()

	def Close(self):
		self.pickValueEditLine.KillFocus()
		self.Hide()

	def OnAccept(self):
		text = self.pickValueEditLine.GetText()
		if text:
			text = constInfo.ConvertMoneyText(text)

		if len(text) > 0 and text.isdigit():

			money = long(text)

			money = min(money, self.maxValue)

			if money > 0:
				if self.eventAccept:
					self.eventAccept(money)

		self.Close()

class PickMoneyDialogNew(ui.ScriptWindow):
	def Close(self):
		if self.pickValueEditLine.IsFocus():
			self.pickValueEditLine.KillFocus()

		self.Hide()

	def Open(self, maxValue):
		width = self.GetWidth()
		(mouseX, mouseY) = wndMgr.GetMousePosition()

		if mouseX + width / 2 > wndMgr.GetScreenWidth():
			xPos = wndMgr.GetScreenWidth() - width
		elif mouseX - width / 2 < 0:
			xPos = 0
		else:
			xPos = mouseX - width / 2

		self.SetPosition(xPos, mouseY - self.GetHeight() - 20)

		if app.USE_CHEQUE_CURRENCY and self.currencyType == 1:
			self.maxValueTextLine.SetText(" / " + localeInfo.DottedNumber(maxValue) + " " + localeInfo.PICKUP_CURRENCY_WON)
		else:
			self.maxValueTextLine.SetText(" / " + localeInfo.DottedNumber(maxValue) + " " + localeInfo.PICKUP_CURRENCY_YANG)

		self.maxValue = maxValue

		self.pickValueEditLine.SetText("")

		if not self.pickValueEditLine.IsFocus():
			self.pickValueEditLine.SetFocus()

		ime.SetCursorPosition(-1)

		self.Show()
		self.SetTop()

	def OnAccept(self):
		text = self.pickValueEditLine.GetText()
		text = text.lower().replace("k", "000")

		inputValue = 0

		if len(text) > 0 and text.isdigit():
			inputValue = long(text)
			inputValue = min(inputValue, self.maxValue)

			if inputValue > 0:
				if self.eventAccept:
					self.eventAccept(inputValue)

		self.Close()

	def __OnValueUpdate(self):
		if self.pickValueEditLine.IsFocus():
			ui.EditLine.OnIMEUpdate(self.pickValueEditLine)
		else:
			pass

		text = self.pickValueEditLine.GetText()
		text = text.lower().replace("k", "000")

		inputValue = 0

		if text and text.isdigit():
			try:
				inputValue = long(text)

				if inputValue > self.maxValue:
					inputValue = self.maxValue
					self.pickValueEditLine.SetText(str(inputValue))
					ime.SetCursorPosition(-1)
			except ValueError:
				inputValue = 0

	def SetAcceptEvent(self, event):
		self.eventAccept = event

	def SetTitleName(self, text):
		self.board.SetTitleName(text)

	def Destroy(self):
		self.currencyType = 0
		self.maxValue = 0

		self.pickValueEditLine.SetReturnEvent(None)
		self.pickValueEditLine.SetEscapeEvent(None)
		self.pickValueEditLine.OnIMEUpdate = None
		self.pickValueEditLine = None

		self.acceptButton.SetEvent(None)
		self.acceptButton = None

		self.cancelButton.SetEvent(None)
		self.cancelButton = None

		self.eventAccept = None
		self.board = None

		self.ClearDictionary()

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()

			if app.USE_CHEQUE_CURRENCY and self.currencyType == 1:
				pyScrLoader.LoadScriptFile(self, "uiscript/pickchequedialog_new.py")
			else:
				pyScrLoader.LoadScriptFile(self, "uiscript/pickmoneydialog_new.py")
		except:
			import exception
			exception.Abort("PickMoneyDialogNew.LoadDialog.LoadScript")

		try:
			self.board = self.GetChild("board_ui")
			self.pickValueEditLine = self.GetChild("input_value_ui")
			self.maxValueTextLine = self.GetChild("max_value_text_ui")
			self.acceptButton = self.GetChild("accept_button_ui")
			self.cancelButton = self.GetChild("cancel_button_ui")
		except:
			import exception
			exception.Abort("PickMoneyDialogNew.LoadDialog.LoadObject")

		try:
			self.pickValueEditLine.SetReturnEvent(ui.__mem_func__(self.OnAccept))
			self.pickValueEditLine.SetEscapeEvent(ui.__mem_func__(self.Close))
			self.pickValueEditLine.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdate)

			self.acceptButton.SetEvent(ui.__mem_func__(self.OnAccept))
			self.cancelButton.SetEvent(ui.__mem_func__(self.Close))
			self.board.SetCloseEvent(ui.__mem_func__(self.Close))
		except:
			import exception
			exception.Abort("PickMoneyDialogNew.LoadDialog.BindObject")

	def __init__(self, currencyType):
		ui.ScriptWindow.__init__(self)

		self.currencyType = currencyType
		self.maxValue = 0
		self.eventAccept = None

		self.LoadDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def SetMax(self, max):
		self.pickValueEditLine.SetMax(max)
