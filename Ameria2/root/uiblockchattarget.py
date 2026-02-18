import net
import wndMgr
import ui
import ime
import chat

class ChatBlockDialog(ui.ScriptWindow):
	# global gplayer_name
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.eventAccept = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/blockchatTarget.py")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.LoadScript")

		try:
			self.board = self.GetChild("board")
			self.hourValueEditLine = self.GetChild("hour_value")
			self.minValueEditLine = self.GetChild("min_value")
			self.secValueEditLine = self.GetChild("sec_value")
			self.acceptButton = self.GetChild("accept_button")
			self.cancelButton = self.GetChild("cancel_button")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.BindObject")

		self.hourValueEditLine.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		self.hourValueEditLine.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.minValueEditLine.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		self.minValueEditLine.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.secValueEditLine.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		self.secValueEditLine.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.acceptButton.SetEvent(ui.__mem_func__(self.OnAccept))
		self.cancelButton.SetEvent(ui.__mem_func__(self.Close))
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))

	def Destroy(self):
		self.ClearDictionary()
		self.eventAccept = 0
		self.hourValueEditLine = 0	
		self.minValueEditLine = 0	
		self.secValueEditLine = 0			
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
		self.secValueEditLine.KillFocus()
		self.Hide()

	def OnAccept(self):
		global gplayer_name
		hour_string = self.hourValueEditLine.GetText()
		min_string = self.minValueEditLine.GetText()
		sec_string = self.secValueEditLine.GetText()
		
		if len(hour_string) > 0 and hour_string.isdigit():
			if len(min_string) > 0 and min_string.isdigit():
				if len(sec_string) > 0 and sec_string.isdigit():
					h = int(hour_string)
					m = int(min_string)
					s = int(sec_string)
					ges = h*60*60 + m*60 + s
					net.SendChatPacket("/block_chat " + gplayer_name + " " + str(ges))
		self.Close()
