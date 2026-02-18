import net
import wndMgr
import ui
import ime
import chat
import constInfo

class BanDialog(ui.ScriptWindow):
	# global gplayer_name
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.eventAccept = 0
		constInfo.IS_PERMA_BAN = 0
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/banpanel.py")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.LoadScript")

		try:
			self.board = self.GetChild("board")
			self.hourValueEditLine = self.GetChild("hour_value")
			self.minValueEditLine = self.GetChild("min_value")
			self.acceptButton = self.GetChild("accept_button")
			self.cancelButton = self.GetChild("cancel_button")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.BindObject")

		self.hourValueEditLine.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		self.hourValueEditLine.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.minValueEditLine.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		self.minValueEditLine.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.acceptButton.SetEvent(ui.__mem_func__(self.OnAccept))
		self.cancelButton.SetEvent(ui.__mem_func__(self.Close))
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))
		
		self.checkBox = ui.CheckBox()
		self.checkBox.SetParent(self.board)
		self.checkBox.SetPosition(15, 55)
		# self.checkBox.SetWindowHorizontalAlignCenter()
		self.checkBox.SetWindowVerticalAlignBottom()
		self.checkBox.SetEvent(ui.__mem_func__(self.CheckPerm), "ON_CHECK", True)
		self.checkBox.SetEvent(ui.__mem_func__(self.CheckPerm), "ON_UNCKECK", False)
		self.checkBox.SetCheckStatus(constInfo.IS_PERMA_BAN)
		self.checkBox.SetTextInfo("Permanent ban")
		self.checkBox.Show()
		
	def CheckPerm(self, checkType, autoFlag):
		constInfo.IS_PERMA_BAN = autoFlag

	def Destroy(self):
		self.ClearDictionary()
		self.eventAccept = 0
		self.hourValueEditLine = 0	
		self.minValueEditLine = 0	
		self.acceptButton = 0
		self.cancelButton = 0
		self.board = None

	def SetTitleName(self, text):
		self.board.SetTitleName(text)

	def SetAcceptEvent(self, event):
		self.eventAccept = event

	def Open(self, player_name):
		global gplayer_name
		gplayer_name = player_name
		self.SetCenterPosition()
		self.hourValueEditLine.SetFocus()

		ime.SetCursorPosition(1)
		self.Show()
		self.SetTop()

	def Close(self):
		self.hourValueEditLine.KillFocus()
		self.minValueEditLine.KillFocus()
		self.Hide()

	def OnAccept(self):
		global gplayer_name
		hour_string = self.hourValueEditLine.GetText()
		reason_string = self.minValueEditLine.GetText()
		trueReason = reason_string.replace(" ", "_")
		
		if constInfo.IS_PERMA_BAN == False:
			net.SendChatPacket("/ban %s %d %s" % (gplayer_name, int(hour_string), str(trueReason)))
		else:
			net.SendChatPacket("/ban %s 9999 %s" % (gplayer_name, str(trueReason)))

		self.Close()
