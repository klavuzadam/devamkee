# coding: latin_1

import ui
import net
import chat
import player
import app
import localeInfo
import constInfo
import ime
import chr
import messenger

class WhisperButton(ui.Button):
	def __init__(self):
		ui.Button.__init__(self, "TOP_MOST")

	def __del__(self):
		ui.Button.__del__(self)

	def SetToolTipText(self, text, x=0, y = 32):
		ui.Button.SetToolTipText(self, text, x, y)
		self.ToolTipText.Show()

	def SetToolTipTextWithColor(self, text, color, x=0, y = 32):
		ui.Button.SetToolTipText(self, text, x, y)
		self.ToolTipText.SetPackedFontColor(color)
		self.ToolTipText.Show()

	def ShowToolTip(self):
		if 0 != self.ToolTipText:
			self.ToolTipText.Show()

	def HideToolTip(self):
		if 0 != self.ToolTipText:
			self.ToolTipText.Show()

class WhisperDialog(ui.ScriptWindow):

	class TextRenderer(ui.Window):
		def SetTargetName(self, targetName):
			self.targetName = targetName

		def OnRender(self):
			(x, y) = self.GetGlobalPosition()
			chat.RenderWhisper(self.targetName, x, y)

	class ResizeButton(ui.DragButton):

		def __init__(self):
			ui.DragButton.__init__(self)

		def __del__(self):
			ui.DragButton.__del__(self)

		def OnMouseOverIn(self):
			app.SetCursor(app.HVSIZE)

		def OnMouseOverOut(self):
			app.SetCursor(app.NORMAL)

	def __init__(self, eventMinimize, eventClose):
		print "NEW WHISPER DIALOG  ----------------------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)
		self.targetName = ""
		self.eventMinimize = eventMinimize
		self.eventClose = eventClose
		self.eventAcceptTarget = None
		if app.ENABLE_MULTILANGUAGE_SYSTEM:
			self.languageID = 0
			self.empireID = 0
	def __del__(self):
		print "---------------------------------------------------------------------------- DELETE WHISPER DIALOG"
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/WhisperDialog.py")
		except:
			import exception
			exception.Abort("WhisperDialog.LoadDialog.LoadScript")

		try:
			GetObject=self.GetChild
			self.titleName = GetObject("titlename")
			self.titleNameEdit = GetObject("titlename_edit")
			self.closeButton = GetObject("closebutton")
			self.scrollBar = GetObject("scrollbar")
			self.chatLine = GetObject("chatline")
			self.minimizeButton = GetObject("minimizebutton")
			self.ignoreButton = GetObject("ignorebutton")
			self.reportViolentWhisperButton = GetObject("reportviolentwhisperbutton")
			self.acceptButton = GetObject("acceptbutton")
			self.sendButton = GetObject("sendbutton")
			self.board = GetObject("board")
			self.editBar = GetObject("editbar")
			self.gamemasterMark = GetObject("gamemastermark")
			if app.ENABLE_MULTILANGUAGE_SYSTEM:
				self.languageFlag = GetObject("language_flag")
				self.empireFlag = GetObject("empire_flag")
		except:
			import exception
			exception.Abort("DialogWindow.LoadDialog.BindObject")

		self.gamemasterMark.Hide()
		self.titleName.SetText("")
		self.titleNameEdit.SetText("")
		self.minimizeButton.SetEvent(ui.__mem_func__(self.Minimize))
		self.closeButton.SetEvent(ui.__mem_func__(self.Close))
		self.scrollBar.SetPos(1.0)
		self.scrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
		self.chatLine.SetReturnEvent(ui.__mem_func__(self.SendWhisper))
		self.chatLine.SetEscapeEvent(ui.__mem_func__(self.Minimize))
		self.chatLine.SetMultiLine()
		self.sendButton.SetEvent(ui.__mem_func__(self.SendWhisper))
		self.titleNameEdit.SetReturnEvent(ui.__mem_func__(self.AcceptTarget))
		self.titleNameEdit.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.ignoreButton.SetToggleDownEvent(ui.__mem_func__(self.IgnoreTarget))
		self.ignoreButton.SetToggleUpEvent(ui.__mem_func__(self.IgnoreTarget))
		self.reportViolentWhisperButton.SetEvent(ui.__mem_func__(self.ReportViolentWhisper))
		self.acceptButton.SetEvent(ui.__mem_func__(self.AcceptTarget))
		if app.ENABLE_MULTILANGUAGE_SYSTEM:
			self.languageFlag.SetPosition(150, 9)
			self.empireFlag.SetPosition(163, 12)
			
		self.textRenderer = self.TextRenderer()
		self.textRenderer.SetParent(self)
		self.textRenderer.SetPosition(20, 28)
		self.textRenderer.SetTargetName("")
		self.textRenderer.Show()

		self.resizeButton = self.ResizeButton()
		self.resizeButton.SetParent(self)
		self.resizeButton.SetSize(20, 20)
		self.resizeButton.SetPosition(280, 180)
		self.resizeButton.SetMoveEvent(ui.__mem_func__(self.ResizeWhisperDialog))
		self.resizeButton.Show()
		
		self.blockButton = ui.MakeButton(self.board, 270 - 41 - 3, 9, "Add Friend", "d:/ymir work/ui/game/whisper/", "add_norm.png", "add_hover.png", "add_selct.png")
		self.unblockButton = ui.MakeButton(self.board, 270 - 41 - 22, 9, "Block", "d:/ymir work/ui/game/whisper/", "block_norm.png", "block_hover.png", "block_select.png")
		self.searchButton = ui.MakeButton(self.board, 270 - 41 - 44, 9, "Search shop", "d:/ymir work/ui/game/whisper/", "tp_norm.png", "tp_hover.png", "tp_select.png")
		self.blockButton.SetEvent(ui.__mem_func__(self.AddFriend))
		self.unblockButton.SetEvent(ui.__mem_func__(self.BlockTarget))
		self.searchButton.SetEvent(ui.__mem_func__(self.SearchByName))

		self.ResizeWhisperDialog()

	def Destroy(self):

		self.eventMinimize = None
		self.eventClose = None
		self.eventAcceptTarget = None

		self.ClearDictionary()
		self.scrollBar.Destroy()
		self.titleName = None
		self.titleNameEdit = None
		self.closeButton = None
		self.scrollBar = None
		self.chatLine = None
		self.sendButton = None
		self.ignoreButton = None
		self.reportViolentWhisperButton = None
		self.acceptButton = None
		self.minimizeButton = None
		self.textRenderer = None
		self.board = None
		self.editBar = None
		self.resizeButton = None
		if app.ENABLE_MULTILANGUAGE_SYSTEM:
			self.languageFlag = None
			self.empireFlag = None
			self.languageID = 0
			self.empireID = 0
			
	def ResizeWhisperDialog(self):
		(xPos, yPos) = self.resizeButton.GetLocalPosition()
		if xPos < 280:
			self.resizeButton.SetPosition(280, yPos)
			return
		if yPos < 150:
			self.resizeButton.SetPosition(xPos, 150)
			return
		self.SetWhisperDialogSize(xPos + 20, yPos + 20)
		
	def SearchByName(self):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			interface.OnSearchByOwnerName(0, self.targetName)
			
	def AddFriend(self):
		if not messenger.IsFriendByName(self.targetName):
			net.SendMessengerAddByNamePacket(self.targetName)
		else:
			chat.AppendChat(1, "%s is already your friend." % self.targetName)
			return

	def BlockTarget(self):
		if self.targetName:
			if messenger.IsBlockFriendByName(self.targetName):
				net.SendMessengerBlockRemovePacket(self.targetName, self.targetName)
			elif not messenger.IsBlockFriendByName(self.targetName):
				net.SendMessengerBlockAddByNamePacket(self.targetName)

	def SetWhisperDialogSize(self, width, height):
		try:

			max = int((width-90)/6) * 3 - 6

			self.board.SetSize(width, height)
			self.scrollBar.SetPosition(width-25, 35)
			self.scrollBar.SetScrollBarSize(height-100)
			self.scrollBar.SetPos(1.0)
			self.editBar.SetSize(width-18, 50)
			self.chatLine.SetSize(width-90, 40)
			self.chatLine.SetLimitWidth(width-90)
			self.SetSize(width, height)

			if 0 != self.targetName:
				chat.SetWhisperBoxSize(self.targetName, width - 50, height - 90)

			if localeInfo.IsARABIC():
				self.textRenderer.SetPosition(width-20, 28)
				self.scrollBar.SetPosition(width-25+self.scrollBar.GetWidth(), 35)
				self.editBar.SetPosition(10 + self.editBar.GetWidth(), height-60)
				self.sendButton.SetPosition(width - 80 + self.sendButton.GetWidth(), 10)
				self.minimizeButton.SetPosition(width-42 + self.minimizeButton.GetWidth(), 12)
				self.closeButton.SetPosition(width-24+self.closeButton.GetWidth(), 12)
				self.chatLine.SetPosition(5 + self.chatLine.GetWidth(), 5)
				self.board.SetPosition(self.board.GetWidth(), 0)
			else:
				self.textRenderer.SetPosition(20, 28)
				self.scrollBar.SetPosition(width-25, 35)
				self.editBar.SetPosition(10, height-60)
				self.sendButton.SetPosition(width-80, 10)
				self.minimizeButton.SetPosition(width-42, 12)
				self.closeButton.SetPosition(width-24, 12)
				
			self.blockButton.SetPosition(width-41 - 24, 9)
			self.unblockButton.SetPosition(width-41 - 24 - 24, 9)
			self.searchButton.SetPosition(width-41 - 24 - 24 - 24, 9)

			self.SetChatLineMax(max)

		except:
			import exception
			exception.Abort("WhisperDialog.SetWhisperDialogSize.BindObject")

	def SetChatLineMax(self, max):
		self.chatLine.SetMax(max)

		from grpText import GetSplitingTextLine

		text = self.chatLine.GetText()
		if text:
			self.chatLine.SetText(GetSplitingTextLine(text, max, 0))
	if app.ENABLE_MULTILANGUAGE_SYSTEM:
		def RequestFlag(self, targetName):
			if targetName:
				import player
				player.RequestFlag(targetName)

		def SetFlag(self, languageID, empireID):
			self.languageID = int(languageID)
			self.empireID = int(empireID)

			if self.languageID == 0:
				self.languageFlag.Hide()
			else:
				langlist = ["en","de","tr","pt","es","fr","ro","pl","it","cz","hu"]
				if self.languageID > 11 or  self.languageID == 0:
					self.languageID = 1

				self.languageFlag.LoadImage("d:/ymir work/ui/language_flag/%s.tga" % langlist[self.languageID-1])
				self.languageFlag.SetScale(2.1, 1.7)
				self.languageFlag.Show()

			# if self.empireID == 0:
			self.empireFlag.Hide()
			# else:
				# self.empireFlag.LoadImage("d:/ymir work/ui/flag_empire/empire_%d_256.dds" % self.empireID)
				# self.empireFlag.Show()

			
	def OpenWithTarget(self, targetName):
		if app.ENABLE_BADGE_NOTIFICATION_MANAGER:
			chat.CreateWhisper(targetName, self.chatLine.hWnd)
		else:
			chat.CreateWhisper(targetName)
		if app.ENABLE_MULTILANGUAGE_SYSTEM:
			self.RequestFlag(targetName)
		chat.SetWhisperBoxSize(targetName, self.GetWidth() - 60, self.GetHeight() - 90)
		self.chatLine.SetFocus()
		self.titleName.SetText(targetName)
		self.targetName = targetName
		self.textRenderer.SetTargetName(targetName)
		self.titleNameEdit.Hide()
		self.ignoreButton.Hide()
		if app.IsDevStage():
			self.reportViolentWhisperButton.Show()
		else:
			self.reportViolentWhisperButton.Hide()
		self.acceptButton.Hide()
		self.gamemasterMark.Hide()
		self.minimizeButton.Show()
		if app.ENABLE_MULTILANGUAGE_SYSTEM:
			self.languageFlag.Hide()
			self.empireFlag.Hide()
	def OpenWithoutTarget(self, event):
		self.eventAcceptTarget = event
		self.titleName.SetText("")
		self.titleNameEdit.SetText("")
		self.titleNameEdit.SetFocus()
		self.targetName = 0
		self.titleNameEdit.Show()
		self.ignoreButton.Hide()
		self.reportViolentWhisperButton.Hide()
		self.acceptButton.Show()
		self.minimizeButton.Hide()
		self.gamemasterMark.Hide()
		if app.ENABLE_MULTILANGUAGE_SYSTEM:
			self.languageFlag.Hide()
			self.empireFlag.Hide()
	def SetGameMasterLook(self):
		self.gamemasterMark.Show()
		self.reportViolentWhisperButton.Hide()

	def Minimize(self):
		self.titleNameEdit.KillFocus()
		self.chatLine.KillFocus()
		self.Hide()

		if None != self.eventMinimize:
			if app.ENABLE_MULTILANGUAGE_SYSTEM:
				self.eventMinimize(self.targetName, self.languageID, self.empireID)
			else:
				self.eventMinimize(self.targetName)

	def Close(self):
		chat.ClearWhisper(self.targetName)
		self.titleNameEdit.KillFocus()
		self.chatLine.KillFocus()
		self.Hide()

		if None != self.eventClose:
			self.eventClose(self.targetName)

	def ReportViolentWhisper(self):
		net.SendChatPacket("/reportviolentwhisper " + self.targetName)

	def IgnoreTarget(self):
		net.SendChatPacket("/ignore " + self.targetName)

	def AcceptTarget(self):
		name = self.titleNameEdit.GetText()
		if len(name) <= 0:
			self.Close()
			return

		if None != self.eventAcceptTarget:
			self.titleNameEdit.KillFocus()
			if app.ENABLE_MULTILANGUAGE_SYSTEM:
				self.RequestFlag(name)
			self.eventAcceptTarget(name)

	def OnScroll(self):
		chat.SetWhisperPosition(self.targetName, self.scrollBar.GetPos())

	def SendWhisper(self):
		import time

		text = self.chatLine.GetText()
		textLength = len(text)

		if textLength > 0:
			if app.LINK_IN_CHAT:
				link = self.GetLink(text)
				if link != "":
					import chr
					if not chr.IsGameMaster():
						text = text.replace(link, "|cFF00C0FC|h|Hweb:" + link.replace("://", "XxX") + "|h" + link + "|h|r")
					else:
						text = text.replace(link, "|cFF00C0FC|h|Hsysweb:" + link.replace("://", "XxX") + "|h" + link + "|h|r")

			if net.IsInsultIn(text):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHAT_INSULT_STRING)
				return

			net.SendWhisperPacket(self.targetName, text)
			self.chatLine.SetText("")

			chat.AppendWhisper(chat.WHISPER_TYPE_CHAT, self.targetName, player.GetName() + " : " + text)

	def OnTop(self):
		self.chatLine.SetFocus()
		if app.ENABLE_BADGE_NOTIFICATION_MANAGER:
			if self.targetName:
				chat.SeenWhisper(self.targetName)
			
	def BindInterface(self, interface):
		self.interface = interface

	def OnMouseLeftButtonDown(self):
		hyperlink = ui.GetHyperlink()
		if hyperlink:
			if app.IsPressed(app.DIK_LALT):
				link = chat.GetLinkFromHyperlink(hyperlink)
				ime.PasteString(link)
			else:
				self.interface.MakeHyperlinkTooltip(hyperlink)

	if app.LINK_IN_CHAT:
		def GetLink(self, text):
			link = ""
			start = text.find("http://")
			if start == -1:
				start = text.find("https://")
			if start == -1:
				return ""

			return text[start:len(text)].split(" ")[0]

if "__main__" == __name__:
	import uiTest

	class TestApp(uiTest.App):
		def OnInit(self):
			wnd = WhisperDialog(self.OnMax, self.OnMin)
			wnd.LoadDialog()
			wnd.OpenWithoutTarget(self.OnNew)
			wnd.SetPosition(0, 0)
			wnd.Show()

			self.wnd = wnd

		def OnMax(self):
			pass

		def OnMin(self):
			pass

		def OnNew(self):
			pass

	TestApp().MainLoop()
