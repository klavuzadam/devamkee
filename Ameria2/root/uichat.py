# coding: latin_1

import ui
import grp
import chat
import wndMgr
import net
import app
import ime
import localeInfo
import colorInfo
import constInfo
import systemSetting
import player

from _weakref import proxy
if app.__CHAT_SETTINGS__:
	import uiCommon

ENABLE_CHAT_COMMAND = True
ENABLE_LAST_SENTENCE_STACK = True
ENABLE_INSULT_CHECK = True

if localeInfo.IsHONGKONG():
	ENABLE_LAST_SENTENCE_STACK = True

if localeInfo.IsEUROPE():
	ENABLE_CHAT_COMMAND = False

if localeInfo.IsCANADA():
	ENABLE_LAST_SENTENCE_STACK = False

chatInputSetList = []
def InsertChatInputSetWindow(wnd):
	global chatInputSetList
	chatInputSetList.append(wnd)
def RefreshChatMode():
	global chatInputSetList
	map(lambda wnd:wnd.OnRefreshChatMode(), chatInputSetList)
def DestroyChatInputSetWindow():
	global chatInputSetList
	chatInputSetList = []

## ChatModeButton
class ChatModeButton(ui.Window):

	OUTLINE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 1.0)
	OVER_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.3)
	BUTTON_STATE_UP = 0
	BUTTON_STATE_OVER = 1
	BUTTON_STATE_DOWN = 2

	def __init__(self):
		ui.Window.__init__(self)
		self.state = None
		self.buttonText = None
		self.event = None
		self.SetWindowName("ChatModeButton")

		net.EnableChatInsultFilter(ENABLE_INSULT_CHECK)

	def __del__(self):
		ui.Window.__del__(self)

	def SAFE_SetEvent(self, event):
		self.event=ui.__mem_func__(event)

	def SetText(self, text):
		if None == self.buttonText:
			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetWindowHorizontalAlignCenter()
			textLine.SetWindowVerticalAlignCenter()
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.SetPackedFontColor(self.OUTLINE_COLOR)
			textLine.Show()
			self.buttonText = textLine

		self.buttonText.SetText(text)

	def SetSize(self, width, height):
		self.width = width
		self.height = height
		ui.Window.SetSize(self, width, height)

	def OnMouseOverIn(self):
		self.state = self.BUTTON_STATE_OVER

	def OnMouseOverOut(self):
		self.state = self.BUTTON_STATE_UP

	def OnMouseLeftButtonDown(self):
		self.state = self.BUTTON_STATE_DOWN

	def OnMouseLeftButtonUp(self):
		self.state = self.BUTTON_STATE_UP
		if self.IsIn():
			self.state = self.BUTTON_STATE_OVER

		if None != self.event:
			self.event()

	def OnRender(self):

		(x, y) = self.GetGlobalPosition()

		grp.SetColor(self.OUTLINE_COLOR)
		grp.RenderRoundBox(x, y, self.width, self.height)

		if self.state >= self.BUTTON_STATE_OVER:
			grp.RenderRoundBox(x+1, y, self.width-2, self.height)
			grp.RenderRoundBox(x, y+1, self.width, self.height-2)

			if self.BUTTON_STATE_DOWN == self.state:
				grp.SetColor(self.OVER_COLOR)
				grp.RenderBar(x+1, y+1, self.width-2, self.height-2)

## ChatLine
class ChatLine(ui.EditLine):

	CHAT_MODE_NAME = {	chat.CHAT_TYPE_TALKING : localeInfo.CHAT_NORMAL,
						chat.CHAT_TYPE_PARTY : localeInfo.CHAT_PARTY,
						chat.CHAT_TYPE_GUILD : localeInfo.CHAT_GUILD,
						chat.CHAT_TYPE_SHOUT : localeInfo.CHAT_SHOUT, }

	def __init__(self):
		ui.EditLine.__init__(self)
		self.SetWindowName("Chat Line")
		self.lastShoutTime = 0
		self.eventEscape = lambda *arg: None
		self.eventReturn = lambda *arg: None
		self.eventTab = None
		self.chatMode = chat.CHAT_TYPE_TALKING
		self.bCodePage = True

		self.overTextLine = ui.TextLine()
		self.overTextLine.SetParent(self)
		self.overTextLine.SetPosition(-1, 0)
		self.overTextLine.SetFontColor(1.0, 1.0, 0.0)
		self.overTextLine.SetOutline()
		self.overTextLine.Hide()

		self.lastSentenceStack = []
		self.lastSentencePos = 0

	def SetChatMode(self, mode):
		self.chatMode = mode

	def GetChatMode(self):
		return self.chatMode

	def ChangeChatMode(self):
		if chat.CHAT_TYPE_TALKING == self.GetChatMode():
			self.SetChatMode(chat.CHAT_TYPE_PARTY)
			self.SetText("#")
			self.SetEndPosition()

		elif chat.CHAT_TYPE_PARTY == self.GetChatMode():
			self.SetChatMode(chat.CHAT_TYPE_GUILD)
			self.SetText("%")
			self.SetEndPosition()

		elif chat.CHAT_TYPE_GUILD == self.GetChatMode():
			self.SetChatMode(chat.CHAT_TYPE_SHOUT)
			self.SetText("!")
			self.SetEndPosition()

		elif chat.CHAT_TYPE_SHOUT == self.GetChatMode():
			self.SetChatMode(chat.CHAT_TYPE_TALKING)
			self.SetText("")

		self.__CheckChatMark()

	if app.LINK_IN_CHAT:
		def GetLink(self, text):
			link = ""
			start = text.find("http://")
			if start == -1:
				start = text.find("https://")
			if start == -1:
				return ""

			return text[start:len(text)].split(" ")[0]

	def GetCurrentChatModeName(self):
		try:
			return self.CHAT_MODE_NAME[self.chatMode]
		except:
			import exception
			exception.Abort("ChatLine.GetCurrentChatModeName")

	def SAFE_SetEscapeEvent(self, event):
		self.eventReturn = ui.__mem_func__(event)

	def SAFE_SetReturnEvent(self, event):
		self.eventEscape = ui.__mem_func__(event)

	def SAFE_SetTabEvent(self, event):
		self.eventTab = ui.__mem_func__(event)

	def SetTabEvent(self, event):
		self.eventTab = event

	def OpenChat(self):
		self.SetFocus()
		self.__ResetChat()

	def __ClearChat(self):
		self.SetText("")
		self.lastSentencePos = 0

	def __ResetChat(self):
		if chat.CHAT_TYPE_PARTY == self.GetChatMode():
			self.SetText("#")
			self.SetEndPosition()
		elif chat.CHAT_TYPE_GUILD == self.GetChatMode():
			self.SetText("%")
			self.SetEndPosition()
		elif chat.CHAT_TYPE_SHOUT == self.GetChatMode():
			self.SetText("!")
			self.SetEndPosition()
		else:
			self.__ClearChat()

		self.__CheckChatMark()


	def __SendChatPacket(self, text, type):
		#import blocklevel

		if net.IsChatInsultIn(text):
			chat.AppendChat(chat.CHAT_TYPE_INFO, locale.CHAT_INSULT_STRING)
		else:
			if app.LINK_IN_CHAT:
				link = self.GetLink(text)
				if link != "":
					import chr
					if not chr.IsGameMaster():
						text = text.replace(link, "|cFF00C0FC|h|Hweb:" + link.replace("://", "XxX") + "|h" + link + "|h|r")
					else:
						text = text.replace(link, "|cFF00C0FC|h|Hsysweb:" + link.replace("://", "XxX") + "|h" + link + "|h|r")
					
			net.SendChatPacket(text, type)

	def __SendPartyChatPacket(self, text):

		if 1 == len(text):
			self.RunCloseEvent()
			return

		self.__SendChatPacket(text[1:], chat.CHAT_TYPE_PARTY)
		self.__ResetChat()

	def __SendGuildChatPacket(self, text):

		if 1 == len(text):
			self.RunCloseEvent()
			return

		self.__SendChatPacket(text[1:], chat.CHAT_TYPE_GUILD)
		self.__ResetChat()

	def __SendShoutChatPacket(self, text):

		if 1 == len(text):
			self.RunCloseEvent()
			return

		if app.GetTime() < self.lastShoutTime + 3:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHAT_SHOUT_LIMIT)
			self.__ResetChat()
			return

		self.__SendChatPacket('|cFFFFA500|Hchat_mp:'+str(player.GetName())+'|h[MP]|h|r'+" " + ": "+text[1:], chat.CHAT_TYPE_SHOUT)

		#self.__SendChatPacket(text[1:], chat.CHAT_TYPE_SHOUT)
		self.__ResetChat()

		self.lastShoutTime = app.GetTime()

	def __SendTalkingChatPacket(self, text):
		self.__SendChatPacket(text, chat.CHAT_TYPE_TALKING)
		self.__ResetChat()

	def OnIMETab(self):
		#if None != self.eventTab:
		#	self.eventTab()
		#return True
		return False

	def OnIMEUpdate(self):
		ui.EditLine.OnIMEUpdate(self)
		self.__CheckChatMark()

	def __CheckChatMark(self):

		self.overTextLine.Hide()

		text = self.GetText()
		if len(text) > 0:
			if '#' == text[0]:
				self.overTextLine.SetText("#")
				self.overTextLine.Show()
			elif '%' == text[0]:
				self.overTextLine.SetText("%")
				self.overTextLine.Show()
			elif '!' == text[0]:
				self.overTextLine.SetText("!")
				self.overTextLine.Show()

	def OnIMEKeyDown(self, key):
		# LAST_SENTENCE_STACK
		if app.VK_UP == key:
			self.__PrevLastSentenceStack()
			return True

		if app.VK_DOWN == key:
			self.__NextLastSentenceStack()
			return True
		# END_OF_LAST_SENTENCE_STACK

		ui.EditLine.OnIMEKeyDown(self, key)

	# LAST_SENTENCE_STACK
	def __PrevLastSentenceStack(self):
		global ENABLE_LAST_SENTENCE_STACK
		if not ENABLE_LAST_SENTENCE_STACK:
			return

		if self.lastSentenceStack and self.lastSentencePos < len(self.lastSentenceStack):
			self.lastSentencePos += 1
			lastSentence = self.lastSentenceStack[-self.lastSentencePos]
			self.SetText(lastSentence)
			self.SetEndPosition()

	def __NextLastSentenceStack(self):
		global ENABLE_LAST_SENTENCE_STACK
		if not ENABLE_LAST_SENTENCE_STACK:
			return

		if self.lastSentenceStack and self.lastSentencePos > 1:
			self.lastSentencePos -= 1
			lastSentence = self.lastSentenceStack[-self.lastSentencePos]
			self.SetText(lastSentence)
			self.SetEndPosition()

	def __PushLastSentenceStack(self, text):
		global ENABLE_LAST_SENTENCE_STACK
		if not ENABLE_LAST_SENTENCE_STACK:
			return

		if len(text) <= 0:
			return

		LAST_SENTENCE_STACK_SIZE = 32
		if len(self.lastSentenceStack) > LAST_SENTENCE_STACK_SIZE:
			self.lastSentenceStack.pop(0)

		self.lastSentenceStack.append(text)
	# END_OF_LAST_SENTENCE_STACK

	if app.ENABLE_EMOTICONS:
		def OnEmotions(self, emoticon):
			self.OnSetFocus()
			emotion = "|cffffc700|Eemoji/%s|e|r"%emoticon
			ime.PasteString(emotion)

	def OnIMEReturn(self):
		text = self.GetText()
		textLen=len(text)

		# LAST_SENTENCE_STACK
		self.__PushLastSentenceStack(text)
		# END_OF_LAST_SENTENCE_STACK

		textSpaceCount=text.count(' ')

		if (textLen > 0) and (textLen != textSpaceCount):
			if '#' == text[0]:
				self.__SendPartyChatPacket(text)
			elif '%' == text[0]:
				self.__SendGuildChatPacket(text)
			elif '!' == text[0]:
				self.__SendShoutChatPacket(text)
			else:
				self.__SendTalkingChatPacket(text)
		else:
			self.__ClearChat()
			self.eventReturn()

		return True

	def OnPressEscapeKey(self):
		self.__ClearChat()
		self.eventEscape()
		return True

	def RunCloseEvent(self):
		self.eventEscape()

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
		else:
			ui.EditLine.OnMouseLeftButtonDown(self)

class ChatInputSet(ui.Window):

	CHAT_OUTLINE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 1.0)

	if app.ENABLE_EMOTICONS:
		def __init__(self, isLogWindow = False):
			ui.Window.__init__(self)
			self.SetWindowName("ChatInputSet")
			self.Emotions = []
			self.isLogWindow = isLogWindow
			if isLogWindow == False:
				self.SetSize(650, 80)

			InsertChatInputSetWindow(self)
			self.__Create()
	else:
		def __init__(self):
			ui.Window.__init__(self)
			self.SetWindowName("ChatInputSet")
			InsertChatInputSetWindow(self)
			self.__Create()

	def __del__(self):
		ui.Window.__del__(self)

	def __Create(self):
		chatModeButton = ChatModeButton()
		chatModeButton.SetParent(self)
		chatModeButton.SetSize(40, 17)
		chatModeButton.SetText(localeInfo.CHAT_NORMAL)
		chatModeButton.SetPosition(7, 2)
		chatModeButton.SAFE_SetEvent(self.OnChangeChatMode)
		self.chatModeButton = chatModeButton

		chatLine = ChatLine()
		chatLine.SetParent(self)
		chatLine.SetMax(512)
		chatLine.SetUserMax(60)
		chatLine.SetText("")
		chatLine.SAFE_SetTabEvent(self.OnChangeChatMode)
		chatLine.x = 0
		chatLine.y = 0
		chatLine.width = 0
		chatLine.height = 0
		self.chatLine = chatLine

		btnSend = ui.Button()
		btnSend.SetParent(self)
		btnSend.SetUpVisual("d:/ymir work/ui/game/taskbar/Send_Chat_Button_01.sub")
		btnSend.SetOverVisual("d:/ymir work/ui/game/taskbar/Send_Chat_Button_02.sub")
		btnSend.SetDownVisual("d:/ymir work/ui/game/taskbar/Send_Chat_Button_03.sub")
		btnSend.SetToolTipText(localeInfo.CHAT_SEND_CHAT)
		btnSend.SAFE_SetEvent(self.chatLine.OnIMEReturn)
		self.btnSend = btnSend
		
		if app.ENABLE_EMOTICONS:
			if self.isLogWindow == False:
				EmotionsPNG = ["angel","blushing","crying","devil","frowny","gasp","glasses","grinning","heart","kiss","like","penguin","poop","robot","shark","shocked","smiley","squint","sunglasses","tongue_out","unsure","upset","winking","troll_2","troll_3","troll_4","middle_finger","troll","considerate"]
				for x in xrange(len(EmotionsPNG)):
					FastEmotion = ui.Button()
					FastEmotion.SetParent(self)
					FastEmotion.SetUpVisual("icon/emoji/"+EmotionsPNG[x]+".tga")
					FastEmotion.SetOverVisual("icon/emoji/"+EmotionsPNG[x]+".tga")
					FastEmotion.SetDownVisual("icon/emoji/"+EmotionsPNG[x]+".tga")
					FastEmotion.SetSize(15,15)
					#FastEmotion.SAFE_SetEvent(self.chatLine.OnEmotions,EmotionsPNG[x])
					FastEmotion.SetEvent(ui.__mem_func__(self.chatLine.OnEmotions),EmotionsPNG[x])
					self.Emotions.append(FastEmotion)

	def Destroy(self):
		self.chatModeButton = None
		self.chatLine = None
		self.btnSend = None

	def Open(self):
		self.chatLine.Show()
		self.chatLine.SetPosition(57, 5)
		self.chatLine.SetFocus()
		self.chatLine.OpenChat()

		self.chatModeButton.SetPosition(7, 2)
		self.chatModeButton.Show()
		
		if app.ENABLE_EMOTICONS:
			for emoji in self.Emotions:
				emoji.Show()

		self.btnSend.Show()
		self.Show()

		self.RefreshPosition()
		return True

	def Close(self):
		self.chatLine.KillFocus()
		self.chatLine.Hide()
		self.chatModeButton.Hide()
		if app.ENABLE_EMOTICONS:
			for emoji in self.Emotions:
				emoji.Hide()
		self.btnSend.Hide()
		self.Hide()
		return True

	def SetEscapeEvent(self, event):
		self.chatLine.SetEscapeEvent(event)

	def SetReturnEvent(self, event):
		self.chatLine.SetReturnEvent(event)

	def OnChangeChatMode(self):
		RefreshChatMode()

	def OnRefreshChatMode(self):
		self.chatLine.ChangeChatMode()
		self.chatModeButton.SetText(self.chatLine.GetCurrentChatModeName())

	def SetChatFocus(self):
		self.chatLine.SetFocus()

	def KillChatFocus(self):
		self.chatLine.KillFocus()

	def SetChatMax(self, max):
		self.chatLine.SetUserMax(max)

	def RefreshPosition(self):
		if localeInfo.IsARABIC():
			self.chatLine.SetSize(self.GetWidth() - 93, 18)
		else:
			if app.ENABLE_EMOTICONS and self.isLogWindow == False:
				self.chatLine.SetSize(550 - 93, 15)
			else:
				self.chatLine.SetSize(self.GetWidth() - 93, 13)

		if app.ENABLE_EMOTICONS and self.isLogWindow == False:
			self.btnSend.SetPosition(550 - 25, 2)
			for x in xrange(len(self.Emotions)):
				self.Emotions[x].SetPosition((550-(20*x))+25, 23)
		else:
			self.btnSend.SetPosition(self.GetWidth() - 25, 2)

		(self.chatLine.x, self.chatLine.y, self.chatLine.width, self.chatLine.height) = self.chatLine.GetRect()

	def BindInterface(self, interface):
		self.chatLine.BindInterface(interface)

	def OnRender(self):
		(x, y, width, height) = self.chatLine.GetRect()
		ui.RenderRoundBox(x-4, y-3, width+7, height+4, self.CHAT_OUTLINE_COLOR)

## ChatWindow
class ChatWindow(ui.Window):

	BOARD_START_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.5)
	BOARD_END_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.5)
	BOARD_MIDDLE_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.5)
	CHAT_OUTLINE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 1.0)

	if app.ENABLE_EMOTICONS:
		EDIT_LINE_HEIGHT = 40
	else:
		EDIT_LINE_HEIGHT = 25
	CHAT_WINDOW_WIDTH = 600

	if app.__CHAT_SETTINGS__:
		EDIT_LINE_HIDE_HEIGHT = 20
		chatLoby = []
		ChatLog = {}
		chatIndex = 0
		CHAT_WINDOW_WIDTH+=30

	class ChatBackBoard(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
		def __del__(self):
			ui.Window.__del__(self)

	class ChatButton(ui.DragButton):

		def __init__(self):
			ui.DragButton.__init__(self)
			self.AddFlag("float")
			self.AddFlag("movable")
			self.AddFlag("animate")
			self.AddFlag("restrict_x")
			self.topFlag = False
			self.SetWindowName("ChatWindow:ChatButton")


		def __del__(self):
			ui.DragButton.__del__(self)

		def SetOwner(self, owner):
			self.owner = owner

		def OnMouseOverIn(self):
			app.SetCursor(app.VSIZE)

		def OnMouseOverOut(self):
			app.SetCursor(app.NORMAL)

		def OnTop(self):
			if True == self.topFlag:
				return

			self.topFlag = True
			self.owner.SetTop()
			self.topFlag = False

	def __init__(self):
		ui.Window.__init__(self)
		self.AddFlag("float")
		self.AddFlag("animate")

		self.SetWindowName("ChatWindow")
		self.__RegisterChatColorDict()

		self.boardState = chat.BOARD_STATE_VIEW
		self.chatID = chat.CreateChatSet(chat.CHAT_SET_CHAT_WINDOW)
		chat.SetBoardState(self.chatID, chat.BOARD_STATE_VIEW)

		if app.ENABLE_VOICE_CHAT:
			self.interface = None

		self.xBar = 0
		self.yBar = 0
		self.widthBar = 0
		self.heightBar = 0
		self.curHeightBar = 0
		self.visibleLineCount = 0
		self.scrollBarPos = 1.0
		self.scrollLock = False

		chatInputSet = ChatInputSet()
		chatInputSet.SetParent(self)
		chatInputSet.SetEscapeEvent(ui.__mem_func__(self.CloseChat))
		chatInputSet.SetReturnEvent(ui.__mem_func__(self.CloseChat))
		if app.ENABLE_EMOTICONS:
			chatInputSet.SetSize(650, 35)
		else:
			chatInputSet.SetSize(550, 25)
		self.chatInputSet = chatInputSet

		btnSendWhisper = ui.Button()
		btnSendWhisper.SetParent(self)
		btnSendWhisper.SetUpVisual("d:/ymir work/ui/game/taskbar/Send_Whisper_Button_01.sub")
		btnSendWhisper.SetOverVisual("d:/ymir work/ui/game/taskbar/Send_Whisper_Button_02.sub")
		btnSendWhisper.SetDownVisual("d:/ymir work/ui/game/taskbar/Send_Whisper_Button_03.sub")
		btnSendWhisper.SetToolTipText(localeInfo.CHAT_SEND_MEMO)
		btnSendWhisper.Hide()
		self.btnSendWhisper = btnSendWhisper

		btnChatLog = ui.Button()
		btnChatLog.SetParent(self)
		btnChatLog.SetUpVisual("d:/ymir work/ui/game/taskbar/Open_Chat_Log_Button_01.sub")
		btnChatLog.SetOverVisual("d:/ymir work/ui/game/taskbar/Open_Chat_Log_Button_02.sub")
		btnChatLog.SetDownVisual("d:/ymir work/ui/game/taskbar/Open_Chat_Log_Button_03.sub")
		btnChatLog.SetToolTipText(localeInfo.CHAT_LOG)
		btnChatLog.Hide()
		self.btnChatLog = btnChatLog
		
		btnSlowChat = ui.Button()
		btnSlowChat.SetParent(self)
		btnSlowChat.SetUpVisual("d:/ymir work/ui/game/taskbar/Open_Chat_Log_Button_01.sub")
		btnSlowChat.SetOverVisual("d:/ymir work/ui/game/taskbar/Open_Chat_Log_Button_02.sub")
		btnSlowChat.SetDownVisual("d:/ymir work/ui/game/taskbar/Open_Chat_Log_Button_03.sub")
		btnSlowChat.SetToolTipText("Slow chat")
		btnSlowChat.SetEvent(ui.__mem_func__(self.SlowChat))
		btnSlowChat.Hide()
		self.btnSlowChat = btnSlowChat

		btnChatSizing = self.ChatButton()
		btnChatSizing.SetOwner(self)
		btnChatSizing.SetMoveEvent(ui.__mem_func__(self.Refresh))
		btnChatSizing.Hide()
		self.btnChatSizing = btnChatSizing

		if app.ENABLE_VOICE_CHAT:
			voiceChatButton = ui.Button()
			voiceChatButton.SetParent(self.btnChatSizing)
			voiceChatButton.SetUpVisual("d:/ymir work/ui/voice_chat/btn_addtab01_default.tga")
			voiceChatButton.SetOverVisual("d:/ymir work/ui/voice_chat/btn_addtab01_over.tga")
			voiceChatButton.SetDownVisual("d:/ymir work/ui/voice_chat/btn_addtab01_down.tga")
			voiceChatButton.SAFE_SetEvent(self.__ToggleVoiceChat)
			voiceChatButton.SetToolTipText(localeInfo.CHAT_VOICE_CHAT)
			voiceChatButton.Hide()
			self.voiceChatButton = voiceChatButton

		if app.__CHAT_SETTINGS__:
			imgChatBarLeft = ui.ImageBox()
			imgChatBarLeft.SetParent(self.btnChatSizing)
			imgChatBarLeft.AddFlag("not_pick")
			imgChatBarLeft.LoadImage("d:/ymir work/ui/chat/chat_linebar_left.tga")
			imgChatBarLeft.Show()
			self.imgChatBarLeft = imgChatBarLeft

			imgChatBarRight = ui.ImageBox()
			imgChatBarRight.SetParent(self.btnChatSizing)
			imgChatBarRight.AddFlag("not_pick")
			imgChatBarRight.LoadImage("d:/ymir work/ui/chat/chat_linebar_right.tga")
			imgChatBarRight.Show()
			self.imgChatBarRight = imgChatBarRight

			imgChatBarMiddle = ui.ExpandedImageBox()
			imgChatBarMiddle.SetParent(self.btnChatSizing)
			imgChatBarMiddle.AddFlag("not_pick")
			imgChatBarMiddle.LoadImage("d:/ymir work/ui/chat/chatmenutab_line.tga")
			imgChatBarMiddle.Show()
			self.imgChatBarMiddle = imgChatBarMiddle

			btnChatLobyCreate = ui.Button()
			btnChatLobyCreate.SetParent(self.btnChatSizing)
			btnChatLobyCreate.SetUpVisual("d:/ymir work/ui/chat/btn_addtab01_default.tga")
			btnChatLobyCreate.SetOverVisual("d:/ymir work/ui/chat/btn_addtab01_over.tga")
			btnChatLobyCreate.SetDownVisual("d:/ymir work/ui/chat/btn_addtab01_down.tga")
			btnChatLobyCreate.SetEvent(ui.__mem_func__(self.CreateLobyButton))
			btnChatLobyCreate.Show()
			self.btnChatLobyCreate = btnChatLobyCreate

			btnChatSettingOption = ui.Button()
			btnChatSettingOption.SetParent(self.btnChatSizing)
			btnChatSettingOption.SetUpVisual("d:/ymir work/ui/chat/btn_option01_default.tga")
			btnChatSettingOption.SetOverVisual("d:/ymir work/ui/chat/btn_option01_over.tga")
			btnChatSettingOption.SetDownVisual("d:/ymir work/ui/chat/btn_option01_down.tga")
			btnChatSettingOption.SetToolTipText(localeInfo.CHATTING_SETTING_TITLE, 0, -23)
			btnChatSettingOption.SetEvent(ui.__mem_func__(self.__SettingOptionWndOpen))
			btnChatSettingOption.Show()
			self.btnChatSettingOption = btnChatSettingOption

			self.wndChatSettingOption = ChatSettingWindow()

			self.LoadChatLoby(True)
		else:
			#import dbg
			#dbg.TraceError("test")
			imgChatBarLeft = ui.ImageBox()
			imgChatBarLeft.SetParent(self.btnChatSizing)
			imgChatBarLeft.AddFlag("not_pick")
			imgChatBarLeft.LoadImage("d:/ymir work/ui/pattern/chat_bar_left.tga")
			imgChatBarLeft.Show()
			self.imgChatBarLeft = imgChatBarLeft
			imgChatBarRight = ui.ImageBox()
			imgChatBarRight.SetParent(self.btnChatSizing)
			imgChatBarRight.AddFlag("not_pick")
			imgChatBarRight.LoadImage("d:/ymir work/ui/pattern/chat_bar_right.tga")
			imgChatBarRight.Show()
			self.imgChatBarRight = imgChatBarRight
			imgChatBarMiddle = ui.ExpandedImageBox()
			imgChatBarMiddle.SetParent(self.btnChatSizing)
			imgChatBarMiddle.AddFlag("not_pick")
			imgChatBarMiddle.LoadImage("d:/ymir work/ui/pattern/chat_bar_middle.tga")
			imgChatBarMiddle.Show()
			self.imgChatBarMiddle = imgChatBarMiddle

		scrollBar = ui.ScrollBar()
		scrollBar.AddFlag("float")
		scrollBar.AddFlag("animate")
		scrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
		self.scrollBar = scrollBar

		self.Refresh()
		self.chatInputSet.RefreshPosition() # RTL �� ��ġ�� ����� �������� ��ġ ������ �ʿ��ϴ�

	def __del__(self):
		ui.Window.__del__(self)

	def __RegisterChatColorDict(self):
		CHAT_COLOR_DICT = {
			chat.CHAT_TYPE_TALKING : colorInfo.CHAT_RGB_TALK,
			chat.CHAT_TYPE_INFO : colorInfo.CHAT_RGB_INFO,
			chat.CHAT_TYPE_NOTICE : colorInfo.CHAT_RGB_NOTICE,
			chat.CHAT_TYPE_PARTY : colorInfo.CHAT_RGB_PARTY,
			chat.CHAT_TYPE_GUILD : colorInfo.CHAT_RGB_GUILD,
			chat.CHAT_TYPE_COMMAND : colorInfo.CHAT_RGB_COMMAND,
			chat.CHAT_TYPE_SHOUT : colorInfo.CHAT_RGB_SHOUT,
			chat.CHAT_TYPE_WHISPER : colorInfo.CHAT_RGB_WHISPER,
		}
		if app.__CHAT_SETTINGS__:
			CHAT_COLOR_DICT.update({
				chat.CHAT_TYPE_EXP_INFO : colorInfo.CHAT_RGB_INFO,
				chat.CHAT_TYPE_ITEM_INFO : colorInfo.CHAT_RGB_INFO,
				chat.CHAT_TYPE_MONEY_INFO : colorInfo.CHAT_RGB_INFO,
			})
		if app.ENABLE_DICE_SYSTEM:
			CHAT_COLOR_DICT.update({chat.CHAT_TYPE_DICE_INFO : colorInfo.CHAT_RGB_DICE_INFO,})

		for colorItem in CHAT_COLOR_DICT.items():
			type=colorItem[0]
			rgb=colorItem[1]
			chat.SetChatColor(type, rgb[0], rgb[1], rgb[2])

	def Destroy(self):
		self.chatInputSet.Destroy()
		self.chatInputSet = None

		if app.ENABLE_VOICE_CHAT:
			self.voiceChatButton = None

		if app.__CHAT_SETTINGS__:
			self.EDIT_LINE_HIDE_HEIGHT = 0
			self.chatLoby = []
			self.btnChatLobyCreate = None
			self.btnChatSettingOption = None
			for key, data in self.ChatLog.items():
				data.Close(True)
				data.Destroy()
			self.ChatLog ={}
			if self.wndChatSettingOption:
				self.wndChatSettingOption.Hide()
				self.wndChatSettingOption.Destroy()
				self.wndChatSettingOption=None
			constInfo.SaveChatData()

		self.btnSendWhisper = 0
		self.btnChatLog = 0
		self.btnSlowChat = 0
		self.btnChatSizing = 0

	################
	## Open & Close
	def OpenChat(self):
		if app.ENABLE_EMOTICONS:
			self.SetSize(self.CHAT_WINDOW_WIDTH, 40)
		else:
			self.SetSize(self.CHAT_WINDOW_WIDTH, 25)
		chat.SetBoardState(self.chatID, chat.BOARD_STATE_EDIT)
		self.boardState = chat.BOARD_STATE_EDIT

		(x, y, width, height) = self.GetRect()
		(btnX, btnY) = self.btnChatSizing.GetGlobalPosition()

		if localeInfo.IsARABIC():
			chat.SetPosition(self.chatID, x + width - 10, y)
		else:
			chat.SetPosition(self.chatID, x + 10, y)

		chat.SetHeight(self.chatID, y - btnY - self.EDIT_LINE_HEIGHT + 100)

		if self.IsShow():
			self.btnChatSizing.Show()

		self.Refresh()

		if app.__CHAT_SETTINGS__:
			self.btnSendWhisper.SetPosition(self.GetWidth() - 75, 2)
			self.btnChatLog.SetPosition(self.GetWidth() - 50, 2)
			self.btnSlowChat.SetPosition(self.GetWidth() - 25, 2)
		else:
			self.btnSendWhisper.SetPosition(self.GetWidth() - 50, 2)
			self.btnChatLog.SetPosition(self.GetWidth() - 25, 2)
			self.btnSlowChat.SetPosition(self.GetWidth(), 2)
		self.btnSendWhisper.Show()
		self.btnChatLog.Show()
		self.btnSlowChat.Show()
		if app.ENABLE_VOICE_CHAT:
			self.voiceChatButton.Show()

		self.chatInputSet.Open()
		self.chatInputSet.SetTop()
		self.SetTop()

	def CloseChat(self):
		chat.SetBoardState(self.chatID, chat.BOARD_STATE_VIEW)
		self.boardState = chat.BOARD_STATE_VIEW

		(x, y, width, height) = self.GetRect()

		if localeInfo.IsARABIC():
			chat.SetPosition(self.chatID, x + width - 10, y + self.EDIT_LINE_HEIGHT)
		else:
			chat.SetPosition(self.chatID, x + 10, y + self.EDIT_LINE_HEIGHT)

		self.SetSize(self.CHAT_WINDOW_WIDTH, 0)

		self.chatInputSet.Close()
		self.btnSendWhisper.Hide()
		self.btnChatLog.Hide()
		self.btnSlowChat.Hide()
		self.btnChatSizing.Hide()
		if app.ENABLE_VOICE_CHAT:
			self.voiceChatButton.Hide()

		self.Refresh()

	if app.__CHAT_SETTINGS__:
		def __ClickRadioButton(self, buttonList, buttonIndex):
			try:
				selButton = buttonList[buttonIndex]
			except IndexError:
				return
			for eachButton in buttonList:
				eachButton.SetUp()
			selButton.Down()
		
		def SelectLobyButtonWithKey(self, index = 0):
			for eachButton in self.chatLoby:
				if eachButton.key == index:
					self.SelectLobyButton(self.chatLoby.index(eachButton), index, True)
					return

		def SelectLobyButton(self, buttonIndex = 0, index = 0, isNotFast = False):
			self.wndChatSettingOption.CheckCacheDict(index)

			if app.IsPressed(app.DIK_LCONTROL) and isNotFast == False:
				if not self.ChatLog.has_key(index):
					newLog = ChatLogWindowNew()
					newLog.Open(index)
					self.ChatLog[index] = newLog

				if index != 0:
					self.RemoveChat(index)

					if index == self.chatIndex:
						self.chatIndex = 0
						self.__ClickRadioButton(self.chatLoby,0)
						self.UpdateChat()

				else:
					self.chatLoby[0].Down()

			else:
				
				self.__ClickRadioButton(self.chatLoby,buttonIndex)
				if index == self.chatIndex:
					return
				self.chatIndex = index
				self.UpdateChat()

		def CreateLobyButton(self):
			maxChat = len(self.ChatLog)+len(self.chatLoby)
			if maxChat >= 10:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHATTING_SETTING_ADD_MAX)
				return
			lasyKey = 0
			for key, data in constInfo.cacheChat.items():
				if len(data[0]) > 0:
					if data[0].isdigit():
						exKey = int(data[0])
						if exKey > lasyKey:
							lasyKey = exKey
					else:
						if key > lasyKey:
							lasyKey = key

			for key, data in self.ChatLog.items():
				if len(data.backupData[0]) > 0:
					if data.backupData[0].isdigit():
						exKey = int(data.backupData[0])
						if exKey > lasyKey:
							lasyKey = exKey
					else:
						if key > lasyKey:
							lasyKey = key

			constInfo.LoadChatNewEmpty(lasyKey+1)
			self.LoadChatLoby()

		def __SettingOptionWndOpen(self):
			if self.wndChatSettingOption:
				self.wndChatSettingOption.Open(self.chatIndex)

		def UpdateChat(self):
			if self.wndChatSettingOption == None:
				return

			CHAT_MODE_INDEX = [ chat.CHAT_TYPE_TALKING,chat.CHAT_TYPE_PARTY,chat.CHAT_TYPE_GUILD,chat.CHAT_TYPE_SHOUT,chat.CHAT_TYPE_INFO,chat.CHAT_TYPE_NOTICE]
			if app.__CHAT_SETTINGS__:
				CHAT_MODE_INDEX.append(chat.CHAT_TYPE_EXP_INFO)
				CHAT_MODE_INDEX.append(chat.CHAT_TYPE_ITEM_INFO)
				CHAT_MODE_INDEX.append(chat.CHAT_TYPE_MONEY_INFO)

			for i in CHAT_MODE_INDEX:
				chat.DisableChatMode(self.chatID, i)

			if self.wndChatSettingOption.cacheDict.has_key(self.chatIndex):
				dataChat = self.wndChatSettingOption.cacheDict[self.chatIndex]

				for key, data in constInfo.OPTION_CHECKBOX_MODE.items():
					if dataChat[key] > 0:
						for chatIndex in data[0]:
							chat.EnableChatMode(self.chatID, chatIndex)

				for j in range(1,12):
					chat.DisableCountryMode(self.chatID, j)

				if dataChat[constInfo.OPTION_CHECKBOX_FILTER] == 1:
					for country in range(constInfo.OPTION_CHECKBOX_FLAG_EN, constInfo.OPTION_MAX):
						if dataChat[country] > 0:
							chat.EnableCountryMode(self.chatID, dataChat[country])
				else:
					for j in range(1,12):
						chat.EnableCountryMode(self.chatID, j)

		def RemoveAllLoby(self):
			self.chatLoby = []

		def RemoveChat(self, chatIndex):
			del self.wndChatSettingOption.cacheDict[chatIndex]
			constInfo.RemoveChat(chatIndex)
			self.LoadChatLoby()

		def LoadChatLoby(self, isFirst = False):
			if isFirst:
				constInfo.LoadChatData()
			self.RemoveAllLoby()

			indexForX = 0

			for key, data in constInfo.cacheChat.items():
				btnChatTab = ui.ToggleButton()
				btnChatTab.SetParent(self.btnChatSizing)
				btnChatTab.SetUpVisual("d:/ymir work/ui/chat/chatmenutab_default.tga")
				btnChatTab.SetOverVisual("d:/ymir work/ui/chat/chatmenutab_default.tga")
				btnChatTab.SetDownVisual("d:/ymir work/ui/chat/chatmenutab_down.tga")
				if data[0] == "0":
					btnChatTab.SetToolTipText(localeInfo.CHATTING_SETTING_TALKING, 0, -23)
					btnChatTab.SetTextAddPos(localeInfo.CHATTING_SETTING_TALKING, -2)
				else:
					btnChatTab.SetToolTipText(data[0], 0, -23)
					btnChatTab.SetTextAddPos(data[0], -2)
				btnChatTab.SetPosition(4+(indexForX*57),0)
				btnChatTab.SetToggleUpEvent(self.SelectLobyButton, indexForX, key)
				btnChatTab.SetToggleDownEvent(self.SelectLobyButton, indexForX, key)
				btnChatTab.Show()
				btnChatTab.key = key
				self.chatLoby.append(btnChatTab)

				indexForX+=1

			if isFirst:
				self.SelectLobyButton(0,0)
			else:
				self.SelectLobyButtonWithKey(self.chatIndex)
				if self.wndChatSettingOption.IsShow():
					self.wndChatSettingOption.Open(self.chatIndex)

			self.__RefreshSizingBar()
			if self.wndChatSettingOption:
				if isFirst:
					self.wndChatSettingOption.CheckCacheDict(0)

				self.UpdateChat()

	def SetSendWhisperEvent(self, event):
		self.btnSendWhisper.SetEvent(event)

	def SetOpenChatLogEvent(self, event):
		self.btnChatLog.SetEvent(event)

	def IsEditMode(self):
		if chat.BOARD_STATE_EDIT == self.boardState:
			return True

		return False

	def __RefreshSizingBar(self):
		(x, y, width, height) = self.GetRect()
		gxChat, gyChat = self.btnChatSizing.GetGlobalPosition()
		self.btnChatSizing.SetPosition(x, gyChat)
		self.btnChatSizing.SetSize(width, 22)

		if app.ENABLE_VOICE_CHAT:
			self.voiceChatButton.SetPosition(width - 70, 3)#Mai muti tu daca nu iese bine

		if app.__CHAT_SETTINGS__:
			self.imgChatBarLeft.SetPosition(0, 17)
			self.imgChatBarRight.SetPosition(width - 57, 0)
			self.btnChatSettingOption.SetPosition(width - 27, 3)
			self.btnChatLobyCreate.SetPosition(width - 27 - 23, 3)
			xEx = len(self.chatLoby)*57
			self.imgChatBarMiddle.SetPosition(xEx, 0)
			self.imgChatBarMiddle.SetRenderingRect(0.0, 0.0, float(width - xEx) / 57.0 - 1.0, 0.0)
		else:
			self.imgChatBarLeft.SetPosition(0, 0)
			self.imgChatBarRight.SetPosition(width - 64, 0)
			self.imgChatBarMiddle.SetPosition(64, 0)
			self.imgChatBarMiddle.SetRenderingRect(0.0, 0.0, float(width - 128) / 64.0 - 1.0, 0.0)

	def SetPosition(self, x, y):
		ui.Window.SetPosition(self, x, y)
		self.__RefreshSizingBar()

	def SetSize(self, width, height):
		ui.Window.SetSize(self, width, height)
		self.__RefreshSizingBar()

	def SetHeight(self, height):
		gxChat, gyChat = self.btnChatSizing.GetGlobalPosition()
		self.btnChatSizing.SetPosition(gxChat, wndMgr.GetScreenHeight() - height)
		
	def SlowChat(self):
		# pass
		if constInfo.slowChat == 0:
			constInfo.slowChat = 1
			chat.SetDelayProcess(1)
			chat.AppendChat(1, "The chat has been slowed.")
		else:
			constInfo.slowChat = 0
			chat.SetDelayProcess(0)
			chat.AppendChat(1, "The chat has been un-slowed.")

	###########
	## Refresh
	def Refresh(self):
		if self.boardState == chat.BOARD_STATE_EDIT:
			self.RefreshBoardEditState()
		elif self.boardState == chat.BOARD_STATE_VIEW:
			self.RefreshBoardViewState()

	def RefreshBoardEditState(self):

		(x, y, width, height) = self.GetRect()
		(btnX, btnY) = self.btnChatSizing.GetGlobalPosition()

		self.xBar = x
		self.yBar = btnY
		self.widthBar = width
		self.heightBar = y - btnY + self.EDIT_LINE_HEIGHT
		self.curHeightBar = self.heightBar

		if localeInfo.IsARABIC():
			chat.SetPosition(self.chatID, x + width - 10, y)
		else:
			chat.SetPosition(self.chatID, x + 10, y)

		chat.SetHeight(self.chatID, y - btnY - self.EDIT_LINE_HEIGHT)
		chat.ArrangeShowingChat(self.chatID)

		if btnY > y:
			self.btnChatSizing.SetPosition(btnX, y)
			self.heightBar = self.EDIT_LINE_HEIGHT

	def RefreshBoardViewState(self):
		(x, y, width, height) = self.GetRect()
		(btnX, btnY) = self.btnChatSizing.GetGlobalPosition()
		textAreaHeight = self.visibleLineCount * chat.GetLineStep(self.chatID)

		if localeInfo.IsARABIC():
			chat.SetPosition(self.chatID, x + width - 10, y + self.EDIT_LINE_HEIGHT)
		else:
			chat.SetPosition(self.chatID, x + 10, y + self.EDIT_LINE_HEIGHT)

		chat.SetHeight(self.chatID, y - btnY - self.EDIT_LINE_HEIGHT + 100)

		if self.boardState == chat.BOARD_STATE_EDIT:
			textAreaHeight += 45
		elif self.visibleLineCount != 0:
			textAreaHeight += 10 + 10

		self.xBar = x
		self.yBar = y + self.EDIT_LINE_HEIGHT - textAreaHeight
		self.widthBar = width
		self.heightBar = textAreaHeight

		self.scrollBar.Hide()

	##########
	## Render
	def OnUpdate(self):
		if self.boardState == chat.BOARD_STATE_EDIT:
			chat.Update(self.chatID)
		elif self.boardState == chat.BOARD_STATE_VIEW:
			if systemSetting.IsViewChat():
				chat.Update(self.chatID)

	def OnRender(self):
		if chat.GetVisibleLineCount(self.chatID) != self.visibleLineCount:
			self.visibleLineCount = chat.GetVisibleLineCount(self.chatID)
			self.Refresh()

		if self.curHeightBar != self.heightBar:
			self.curHeightBar += (self.heightBar - self.curHeightBar) / 10

		if self.boardState == chat.BOARD_STATE_EDIT:
			grp.SetColor(self.BOARD_MIDDLE_COLOR)
			if app.__CHAT_SETTINGS__:
				grp.RenderBar(self.xBar, self.yBar + (self.heightBar - self.curHeightBar) + self.EDIT_LINE_HIDE_HEIGHT, self.widthBar, self.curHeightBar)
			else:
				grp.RenderBar(self.xBar, self.yBar + (self.heightBar - self.curHeightBar) + 10, self.widthBar, self.curHeightBar)
			chat.Render(self.chatID)
		elif self.boardState == chat.BOARD_STATE_VIEW:
			if systemSetting.IsViewChat():
				grp.RenderGradationBar(self.xBar, self.yBar + (self.heightBar - self.curHeightBar), self.widthBar, self.curHeightBar, self.BOARD_START_COLOR, self.BOARD_END_COLOR)
				chat.Render(self.chatID)

	##########
	## Event
	def OnTop(self):
		self.btnChatSizing.SetTop()
		self.scrollBar.SetTop()

	def OnScroll(self):
		if not self.scrollLock:
			self.scrollBarPos = self.scrollBar.GetPos()

		lineCount = chat.GetLineCount(self.chatID)
		visibleLineCount = chat.GetVisibleLineCount(self.chatID)
		endLine = visibleLineCount + int(float(lineCount - visibleLineCount) * self.scrollBarPos)

		chat.SetEndPos(self.chatID, self.scrollBarPos)

	def OnChangeChatMode(self):
		self.chatInputSet.OnChangeChatMode()

	def SetChatFocus(self):
		self.chatInputSet.SetChatFocus()

	if app.ENABLE_VOICE_CHAT:
		def __ToggleVoiceChat(self):
			if self.interface:
				self.interface.ToggleVoiceChatConfigWindow()

	def BindInterface(self, interface):
		self.chatInputSet.BindInterface(interface)
		if app.ENABLE_VOICE_CHAT:
			self.interface = proxy(interface)

	#if app.USE_CAPTCHA_SYSTEM:
	def hide_btnChatSizing(self):
		if self.btnChatSizing:
			if self.IsShow():
				self.btnChatSizing.Hide()

	def show_btnChatSizing(self):
		if self.btnChatSizing:
			if self.btnChatLog.IsShow():
				self.btnChatSizing.Show()

## ChatLogWindow
class ChatLogWindow(ui.Window):

	BLOCK_WIDTH = 32
	CHAT_MODE_NAME = [ localeInfo.CHAT_NORMAL, localeInfo.CHAT_PARTY, localeInfo.CHAT_GUILD, localeInfo.CHAT_SHOUT, localeInfo.CHAT_INFORMATION, localeInfo.CHAT_NOTICE, ]
	CHAT_MODE_INDEX = [ chat.CHAT_TYPE_TALKING,
						chat.CHAT_TYPE_PARTY,
						chat.CHAT_TYPE_GUILD,
						chat.CHAT_TYPE_SHOUT,
						chat.CHAT_TYPE_INFO,
						chat.CHAT_TYPE_NOTICE, ]

	if app.ENABLE_DICE_SYSTEM:
		CHAT_MODE_NAME.append(localeInfo.CHAT_DICE_INFO)
		CHAT_MODE_INDEX.append(chat.CHAT_TYPE_DICE_INFO)

	CHAT_LOG_WINDOW_MINIMUM_WIDTH = 450
	CHAT_LOG_WINDOW_MINIMUM_HEIGHT = 120

	class ResizeButton(ui.DragButton):

		def __init__(self):
			ui.DragButton.__init__(self)

		def __del__(self):
			ui.DragButton.__del__(self)

		def OnMouseOverIn(self):
			app.SetCursor(app.HVSIZE)

		def OnMouseOverOut(self):
			app.SetCursor(app.NORMAL)

	def __init__(self):

		self.allChatMode = True
		self.chatInputSet = None

		ui.Window.__init__(self)
		self.AddFlag("float")
		self.AddFlag("movable")
		self.AddFlag("animate")
		self.SetWindowName("ChatLogWindow")
		self.__CreateChatInputSet()
		self.__CreateWindow()
		self.__CreateButton()
		self.__CreateScrollBar()

		self.chatID = chat.CreateChatSet(chat.CHAT_SET_LOG_WINDOW)
		chat.SetBoardState(self.chatID, chat.BOARD_STATE_LOG)
		for i in self.CHAT_MODE_INDEX:
			chat.EnableChatMode(self.chatID, i)

		self.SetPosition(20, 20)
		self.SetSize(self.CHAT_LOG_WINDOW_MINIMUM_WIDTH, self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT)
		self.btnSizing.SetPosition(self.CHAT_LOG_WINDOW_MINIMUM_WIDTH-self.btnSizing.GetWidth(), self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT-self.btnSizing.GetHeight()+2)

		self.OnResize()

	def __CreateChatInputSet(self):
		if app.ENABLE_EMOTICONS:
			chatInputSet = ChatInputSet(True)
		else:
			chatInputSet = ChatInputSet()
		chatInputSet.SetParent(self)
		chatInputSet.SetEscapeEvent(ui.__mem_func__(self.Close))
		chatInputSet.SetWindowVerticalAlignBottom()
		chatInputSet.Open()
		self.chatInputSet = chatInputSet

	def __CreateWindow(self):
		imgLeft = ui.ImageBox()
		imgLeft.AddFlag("not_pick")
		imgLeft.SetParent(self)

		imgCenter = ui.ExpandedImageBox()
		imgCenter.AddFlag("not_pick")
		imgCenter.SetParent(self)

		imgRight = ui.ImageBox()
		imgRight.AddFlag("not_pick")
		imgRight.SetParent(self)

		if localeInfo.IsARABIC():
			imgLeft.LoadImage("locale/ae/ui/pattern/titlebar_left.tga")
			imgCenter.LoadImage("locale/ae/ui/pattern/titlebar_center.tga")
			imgRight.LoadImage("locale/ae/ui/pattern/titlebar_right.tga")
		else:
			imgLeft.LoadImage("d:/ymir work/ui/pattern/chatlogwindow_titlebar_left.tga")
			imgCenter.LoadImage("d:/ymir work/ui/pattern/chatlogwindow_titlebar_middle.tga")
			imgRight.LoadImage("d:/ymir work/ui/pattern/chatlogwindow_titlebar_right.tga")

		imgLeft.Show()
		imgCenter.Show()
		imgRight.Show()

		btnClose = ui.Button()
		btnClose.SetParent(self)
		btnClose.SetUpVisual("d:/ymir work/ui/public/close_button_01.sub")
		btnClose.SetOverVisual("d:/ymir work/ui/public/close_button_02.sub")
		btnClose.SetDownVisual("d:/ymir work/ui/public/close_button_03.sub")
		btnClose.SetToolTipText(localeInfo.UI_CLOSE, 0, -23)
		btnClose.SetEvent(ui.__mem_func__(self.Close))
		btnClose.Show()

		btnSizing = self.ResizeButton()
		btnSizing.SetParent(self)
		btnSizing.SetMoveEvent(ui.__mem_func__(self.OnResize))
		btnSizing.SetSize(16, 16)
		btnSizing.Show()

		titleName = ui.TextLine()
		titleName.SetParent(self)

		if localeInfo.IsARABIC():
			titleName.SetPosition(self.GetWidth()-20, 6)
		else:
			titleName.SetPosition(20, 6)

		titleName.SetText(localeInfo.CHAT_LOG_TITLE)
		titleName.Show()

		self.imgLeft = imgLeft
		self.imgCenter = imgCenter
		self.imgRight = imgRight
		self.btnClose = btnClose
		self.btnSizing = btnSizing
		self.titleName = titleName

	def __CreateButton(self):

		if localeInfo.IsARABIC():
			bx = 20
		else:
			bx = 13

		btnAll = ui.RadioButton()
		btnAll.SetParent(self)
		btnAll.SetPosition(bx, 24)
		btnAll.SetUpVisual("d:/ymir work/ui/public/xsmall_button_01.sub")
		btnAll.SetOverVisual("d:/ymir work/ui/public/xsmall_button_02.sub")
		btnAll.SetDownVisual("d:/ymir work/ui/public/xsmall_button_03.sub")
		btnAll.SetText(localeInfo.CHAT_ALL)
		btnAll.SetEvent(ui.__mem_func__(self.ToggleAllChatMode))
		btnAll.Down()
		btnAll.Show()
		self.btnAll = btnAll

		x = bx + 48
		i = 0
		self.modeButtonList = []
		for name in self.CHAT_MODE_NAME:
			btn = ui.ToggleButton()
			btn.SetParent(self)
			btn.SetPosition(x, 24)
			btn.SetUpVisual("d:/ymir work/ui/public/xsmall_button_01.sub")
			btn.SetOverVisual("d:/ymir work/ui/public/xsmall_button_02.sub")
			btn.SetDownVisual("d:/ymir work/ui/public/xsmall_button_03.sub")
			btn.SetText(name)
			btn.Show()

			mode = self.CHAT_MODE_INDEX[i]
			btn.SetToggleUpEvent(lambda arg=mode: self.ToggleChatMode(arg))
			btn.SetToggleDownEvent(lambda arg=mode: self.ToggleChatMode(arg))
			self.modeButtonList.append(btn)

			x += 48
			i += 1

	def __CreateScrollBar(self):
		scrollBar = ui.SmallThinScrollBar()
		scrollBar.SetParent(self)
		scrollBar.Show()
		scrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
		self.scrollBar = scrollBar
		self.scrollBarPos = 1.0

	def __del__(self):
		ui.Window.__del__(self)

	def Destroy(self):
		self.imgLeft = None
		self.imgCenter = None
		self.imgRight = None
		self.btnClose = None
		self.btnSizing = None
		self.modeButtonList = []
		self.scrollBar = None
		self.chatInputSet = None

	def ToggleAllChatMode(self):
		if self.allChatMode:
			return

		self.allChatMode = True

		for i in self.CHAT_MODE_INDEX:
			chat.EnableChatMode(self.chatID, i)
		for btn in self.modeButtonList:
			btn.SetUp()

	def ToggleChatMode(self, mode):
		if self.allChatMode:
			self.allChatMode = False
			for i in self.CHAT_MODE_INDEX:
				chat.DisableChatMode(self.chatID, i)
			chat.EnableChatMode(self.chatID, mode)
			if app.__CHAT_SETTINGS__:
				if mode == chat.CHAT_TYPE_INFO:
					chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_EXP_INFO)
					chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_ITEM_INFO)
					chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_MONEY_INFO)
			self.btnAll.SetUp()

		else:
			chat.ToggleChatMode(self.chatID, mode)

	def SetSize(self, width, height):
		self.imgCenter.SetRenderingRect(0.0, 0.0, float((width - self.BLOCK_WIDTH*2) - self.BLOCK_WIDTH) / self.BLOCK_WIDTH, 0.0)
		self.imgCenter.SetPosition(self.BLOCK_WIDTH, 0)
		self.imgRight.SetPosition(width - self.BLOCK_WIDTH, 0)

		if localeInfo.IsARABIC():
			self.titleName.SetPosition(self.GetWidth()-20, 3)
			self.btnClose.SetPosition(3, 3)
			self.scrollBar.SetPosition(1, 45)
		else:
			self.btnClose.SetPosition(width - self.btnClose.GetWidth() - 5, 5)
			self.scrollBar.SetPosition(width - 15, 45)

		self.scrollBar.SetScrollBarSize(height - 45 - 12)
		self.scrollBar.SetPos(self.scrollBarPos)
		ui.Window.SetSize(self, width, height)

	def Open(self):
		self.OnResize()
		self.chatInputSet.SetChatFocus()
		self.Show()

	def Close(self):
		if self.chatInputSet:
			self.chatInputSet.KillChatFocus()
		self.Hide()

	def OnResize(self):
		x, y = self.btnSizing.GetLocalPosition()
		width = self.btnSizing.GetWidth()
		height = self.btnSizing.GetHeight()

		if x < self.CHAT_LOG_WINDOW_MINIMUM_WIDTH - width:
			self.btnSizing.SetPosition(self.CHAT_LOG_WINDOW_MINIMUM_WIDTH - width, y)
			return
		if y < self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT - height:
			self.btnSizing.SetPosition(x, self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT - height)
			return

		self.scrollBar.LockScroll()
		self.SetSize(x + width, y + height)
		self.scrollBar.UnlockScroll()

		if localeInfo.IsARABIC():
			self.chatInputSet.SetPosition(20, 25)
		else:
			if app.ENABLE_EMOTICONS and self.chatInputSet.isLogWindow == False:
				self.chatInputSet.SetPosition(0,45)
			else:
				self.chatInputSet.SetPosition(0, 25)

		if app.ENABLE_EMOTICONS:
			if self.chatInputSet.isLogWindow == True:
				self.chatInputSet.SetSize(self.GetWidth() - 20, 20)
		else:
			self.chatInputSet.SetSize(self.GetWidth() - 20, 20)

		self.chatInputSet.RefreshPosition()

		if (self.GetWidth() / 8) < 74:
			self.chatInputSet.SetChatMax(self.GetWidth() / 8)
		else:
			self.chatInputSet.SetChatMax(60)

	def OnScroll(self):
		self.scrollBarPos = self.scrollBar.GetPos()

		lineCount = chat.GetLineCount(self.chatID)
		visibleLineCount = chat.GetVisibleLineCount(self.chatID)
		endLine = visibleLineCount + int(float(lineCount - visibleLineCount) * self.scrollBarPos)

		chat.SetEndPos(self.chatID, self.scrollBarPos)

	def OnRender(self):
		(x, y, width, height) = self.GetRect()

		if localeInfo.IsARABIC():
			grp.SetColor(0x77000000)
			grp.RenderBar(x+2, y+45, 13, height-45)

			grp.SetColor(0x77000000)
			grp.RenderBar(x, y, width, height)
			grp.SetColor(0x77000000)
			grp.RenderBox(x, y, width-2, height)
			grp.SetColor(0x77000000)
			grp.RenderBox(x+1, y+1, width-2, height)

			grp.SetColor(0xff989898)
			grp.RenderLine(x+width-13, y+height-1, 11, -11)
			grp.RenderLine(x+width-9, y+height-1, 7, -7)
			grp.RenderLine(x+width-5, y+height-1, 3, -3)
		else:
			grp.SetColor(0x77000000)
			grp.RenderBar(x+width-15, y+45, 13, height-45)

			grp.SetColor(0x77000000)
			grp.RenderBar(x, y, width, height)
			grp.SetColor(0x77000000)
			grp.RenderBox(x, y, width-2, height)
			grp.SetColor(0x77000000)
			grp.RenderBox(x+1, y+1, width-2, height)

			grp.SetColor(0xff989898)
			grp.RenderLine(x+width-13, y+height-1, 11, -11)
			grp.RenderLine(x+width-9, y+height-1, 7, -7)
			grp.RenderLine(x+width-5, y+height-1, 3, -3)

		#####

		chat.ArrangeShowingChat(self.chatID)

		if localeInfo.IsARABIC():
			chat.SetPosition(self.chatID, x + width - 10, y + height - 25)
		else:
			if app.ENABLE_EMOTICONS:
				chat.SetPosition(self.chatID, x + 10, y + height - 25)
			else:
				chat.SetPosition(self.chatID, x + 10, y + height - 45)
		if app.ENABLE_EMOTICONS:
			chat.SetHeight(self.chatID, height - 45 - 45)
		else:
			chat.SetHeight(self.chatID, height - 45 - 25)
		chat.Update(self.chatID)
		chat.Render(self.chatID)

	def OnPressEscapeKey(self):
		self.Close()
		return True

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

if app.__CHAT_SETTINGS__:
	CHECK_BOX_X_POS = 145

	class ChatSettingWindow(ui.ScriptWindow):
		def __del__(self):
			ui.ScriptWindow.__del__(self)
		def __init__(self):
			ui.ScriptWindow.__init__(self)
			self.Destroy()
			self.LoadWindow()

		def Destroy(self):
			self.questionDialog = None
			self.children = {}
			self.checkBoxSlotDict = {}
			self.cacheDict = {}
			self.chatIndex = 0

		def LoadWindow(self):
			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, "UIScript/ChatSettingWindow.py")


				self.GetChild("lang_setting").SetText(localeInfo.CHATTING_SETTING_LANGUAGE_CUR % app.GetLocalePath().split("/")[-1])

				self.GetChild("name_button").SAFE_SetEvent(self.SetNameQuestion)
				self.GetChild("reset_button").SAFE_SetEvent(self.ResetButton)
				self.GetChild("save_button").SAFE_SetEvent(self.Save)
				self.GetChild("cancel_button").SAFE_SetEvent(self.Close)

				self.GetChild("filter_button").SetToggleUpEvent(self.__FilterUp)
				self.GetChild("filter_button").SetToggleDownEvent(self.__FilterDown)

				self.GetChild("board").SetCloseEvent(self.Close)

				yPos = 64+18
				for key in range(constInfo.OPTION_CHECKBOX_TALKING, constInfo.OPTION_CHECKBOX_MONEY_INFO+1):
					event = lambda index = key : ui.__mem_func__(self.SetCurrentChatOption)(index)
					x = 145
					y = yPos+((key-1)*18)
					if key >= constInfo.OPTION_CHECKBOX_DICE:
						y+=28+21+18+18+18+28+3
					if key > constInfo.OPTION_CHECKBOX_DICE:
						y+=28+3
					self.checkBoxSlotDict[key] = ui.CheckBoxNew(self.GetChild("board"), x, y, event)

				(xPos,yPos) = self.GetChild("filter_button").GetLocalPosition()
				CHATTING_PATH = "d:/ymir work/ui/chat/"
				for j in xrange(3):
					realIndex = 2 - j

					langCombo = ui.ComboBoxImage(self.GetChild("board"),CHATTING_PATH + "chattingoption_dropdown_large_bg.sub",xPos,yPos+18+(18*realIndex))
					langCombo.Show()
					langCombo.SetEvent(lambda x, real = realIndex, point=proxy(self): point.__ClickLanguageBox(x, real))
					self.children["langCombo%d"%realIndex] = langCombo

					langComboButton = ui.Button()
					langComboButton.SetParent(langCombo)
					langComboButton.SetUpVisual(CHATTING_PATH + "chattingoption_dropdown_arrow_01.sub")
					langComboButton.SetOverVisual(CHATTING_PATH + "chattingoption_dropdown_arrow_02.sub")
					langComboButton.SetDownVisual(CHATTING_PATH + "chattingoption_dropdown_arrow_03.sub")
					langComboButton.SetPosition(langCombo.GetWidth()-langComboButton.GetWidth(),1)
					langComboButton.SAFE_SetEvent(langCombo.OnMouseLeftButtonUp)
					langComboButton.Show()
					self.children["langComboButton%d"%realIndex] = langComboButton

				listCombo = ui.ComboBoxImage(self.GetChild("board"),CHATTING_PATH + "chattingoption_dropdown_small_bg.sub",145,64)
				listCombo.Show()
				listCombo.SetEvent(lambda x, point=proxy(self): point.__ClickChat(x))
				self.children["listCombo"] = listCombo

				listComboButton = ui.Button()
				listComboButton.SetParent(listCombo)
				listComboButton.SetUpVisual(CHATTING_PATH + "chattingoption_dropdown_arrow_01.sub")
				listComboButton.SetOverVisual(CHATTING_PATH + "chattingoption_dropdown_arrow_02.sub")
				listComboButton.SetDownVisual(CHATTING_PATH + "chattingoption_dropdown_arrow_03.sub")
				listComboButton.SetPosition(listCombo.GetWidth()-listComboButton.GetWidth(),1)
				listComboButton.SAFE_SetEvent(listCombo.OnMouseLeftButtonUp)
				listComboButton.Show()
				self.children["listComboButton"] = listComboButton

			except:
				import exception
				exception.Abort("ChatSettingWindow.LoadWindow.LoadScript")

		def __FilterUp(self):
			if self.CheckCacheDict(self.chatIndex) == False:
				return
			self.cacheDict[self.chatIndex][constInfo.OPTION_CHECKBOX_FILTER] = 0
			self.SetLangFilter()

			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				interface.wndChat.UpdateChat()

		def __FilterDown(self):
			if self.CheckCacheDict(self.chatIndex) == False:
				return
			self.cacheDict[self.chatIndex][constInfo.OPTION_CHECKBOX_FILTER] = 1
			self.SetLangFilter()
			
			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				interface.wndChat.UpdateChat()
	
		def __ClickChat(self, chatIndex):
			if self.chatIndex == chatIndex:
				return
			self.Open(chatIndex)

		def __ClickLanguageBox(self, langIndex, realIndex):
			if self.CheckCacheDict(self.chatIndex) == False:
				return
			self.cacheDict[self.chatIndex][constInfo.OPTION_CHECKBOX_FLAG_EN+realIndex] = langIndex
			self.SetLangFilter()
			
			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				interface.wndChat.UpdateChat()

		def SetCurrentChatOption(self, index):
			if self.CheckCacheDict(self.chatIndex) == False:
				return
			value = 0
			if not self.checkBoxSlotDict[index].GetCheck():
				value = 1
			self.checkBoxSlotDict[index].SetCheck(value)
			self.cacheDict[self.chatIndex][index] = value

			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				interface.wndChat.UpdateChat()

		def CheckCacheDict(self, chatIndex):
			if not self.cacheDict.has_key(chatIndex):
				if constInfo.cacheChat.has_key(chatIndex):
					self.cacheDict[chatIndex] = constInfo.cacheChat[chatIndex]
				else:
					return False
			return True

		def Open(self, chatIndex = 0):
			if self.CheckCacheDict(chatIndex) == False:
				self.Open(0)
				return

			if chatIndex == -1:
				self.Open(self.chatIndex)
				return

			self.chatIndex = chatIndex
			self.SetList()
			self.SetCheckBox()
			self.SetLangFilter()
			self.Show()

			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				interface.wndChat.UpdateChat()

		def SetList(self):
			if self.CheckCacheDict(self.chatIndex) == False:
				return
			dataChat = constInfo.cacheChat[self.chatIndex]
			#dataChat = self.cacheDict[self.chatIndex]

			self.children["listCombo"].ClearItem()
			if dataChat[0] == "0" and self.chatIndex == 0:
				self.children["listCombo"].SetCurrentItem(localeInfo.CHATTING_SETTING_TALKING)
				self.GetChild("name_editline").SetText("")
				self.GetChild("name_editline").SetInfoMessage(localeInfo.CHATTING_SETTING_TALKING)
			else:
				self.children["listCombo"].SetCurrentItem(dataChat[0])
				self.GetChild("name_editline").SetText("")
				self.GetChild("name_editline").SetInfoMessage(dataChat[0])

			for index, data in constInfo.cacheChat.iteritems():
				if index == 0 and data[0] == "0":
					self.children["listCombo"].InsertItem(index, localeInfo.CHATTING_SETTING_TALKING)
				else:
					self.children["listCombo"].InsertItem(index, data[0])

		def SetCheckBox(self):
			if self.CheckCacheDict(self.chatIndex) == False:
				return
			dataChat = self.cacheDict[self.chatIndex]
			for j in range(constInfo.OPTION_CHECKBOX_TALKING, constInfo.OPTION_CHECKBOX_MONEY_INFO+1):
				self.checkBoxSlotDict[j].SetCheck(dataChat[j])

		def SetLangFilter(self):
			if self.CheckCacheDict(self.chatIndex) == False:
				return
			dataChat = self.cacheDict[self.chatIndex]

			if dataChat[constInfo.OPTION_CHECKBOX_FILTER]:
				self.GetChild("filter_button").SetText(localeInfo.CHATTING_SETTING_LANGUAGE_ON)
				self.GetChild("filter_button").Down()
			else:
				self.GetChild("filter_button").SetText(localeInfo.CHATTING_SETTING_LANGUAGE_OFF)
				self.GetChild("filter_button").SetUp()

			alreadyHaveLang = []
			langData = {0:"Not Selected", 1 : "En",2: "De",3: "Tr", 4: "Pt", 5: "Es", 6: "Fr", 7: "Ro", 8: "Pl", 9: "It", 10: "Cz", 11: "Hu", }

			for j in xrange(len(langData)-1):
				alreadyHaveLang.append(dataChat[j+constInfo.OPTION_CHECKBOX_FLAG_EN])

			for j in xrange(3):
				langIndex = dataChat[j+constInfo.OPTION_CHECKBOX_FLAG_EN]
				self.children["langCombo%d"%j].ClearItem()
				self.children["langCombo%d"%j].SetCurrentItem(langData[langIndex])
				for index, data in langData.iteritems():
					if index != 0:
						if index in alreadyHaveLang:
							continue
					self.children["langCombo%d"%j].InsertItem(index, data)


		def SetNameQuestion(self):
			if self.CheckCacheDict(self.chatIndex) == False:
				return

			if self.questionDialog != None:
				self.__QuestionPopupCancel()
			questionDialog = uiCommon.QuestionDialog()
			questionDialog.SetText(localeInfo.CHATTING_SETTING_CHANGE_TITLE_NAME)
			questionDialog.SetAcceptEvent(ui.__mem_func__(self.__QuestionSetNameAccept))
			questionDialog.SetCancelEvent(ui.__mem_func__(self.__QuestionPopupCancel))
			questionDialog.Open()
			self.questionDialog = questionDialog

		def __QuestionSetNameAccept(self):
			if self.chatIndex == 0:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHATTING_SETTING_CHANGE_TITLE_NOT)
				self.__QuestionPopupCancel()
				return

			text = self.GetChild("name_editline").GetText()

			self.GetChild("name_editline").SetText("General")
			(x_,y_) = self.GetChild("name_editline").GetTextSize()
			self.GetChild("name_editline").SetText(text)

			if self.GetChild("name_editline").GetTextSize()[0] > x_ or len(text) == 0 or text.isspace():
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHATTING_SETTING_CANT_CHANGE_TITLE)
				self.__QuestionPopupCancel()
				return

			self.cacheDict[self.chatIndex][0] = text
			constInfo.cacheChat[self.chatIndex][0] = text

			constInfo.SaveChatData()

			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				interface.wndChat.LoadChatLoby()

			self.__QuestionPopupCancel()

		def ResetButton(self):
			if self.CheckCacheDict(self.chatIndex) == False:
				return

			if self.questionDialog != None:
				self.__QuestionPopupCancel()
			questionDialog = uiCommon.QuestionDialog()
			questionDialog.SetText(localeInfo.CHATTING_SETTING_CLEAR_QUESTION)
			questionDialog.SetAcceptEvent(ui.__mem_func__(self.__QuestionClearAccept))
			questionDialog.SetCancelEvent(ui.__mem_func__(self.__QuestionPopupCancel))
			questionDialog.Open()
			self.questionDialog = questionDialog

		def __QuestionClearAccept(self):
			constInfo.LoadChatNewEmpty(self.chatIndex)
			self.cacheDict[self.chatIndex] = constInfo.cacheChat[self.chatIndex]
			self.Open(self.chatIndex)

			self.__QuestionPopupCancel()

		def __QuestionPopupCancel(self):
			if self.questionDialog != None:
				self.questionDialog.Close()
				self.questionDialog=None

		def Save(self):
			if self.questionDialog != None:
				self.__QuestionPopupCancel()
			questionDialog = uiCommon.QuestionDialog()
			name = constInfo.cacheChat[self.chatIndex][0]
			if self.chatIndex == 0 and name == "0":
				name = localeInfo.CHATTING_SETTING_TALKING
			questionDialog.SetText(localeInfo.CHATTING_SETTING_SAVE_QUESTION % name)
			questionDialog.SetAcceptEvent(ui.__mem_func__(self.__QuestionSaveAccept))
			questionDialog.SetCancelEvent(ui.__mem_func__(self.__QuestionPopupCancel))
			questionDialog.Open()
			self.questionDialog = questionDialog

		def __QuestionSaveAccept(self):
			constInfo.cacheChat[self.chatIndex] = self.cacheDict[self.chatIndex]
			constInfo.SaveChatData()
			self.__QuestionPopupCancel()

		def Close(self):
			self.Hide()

		def OnPressEscapeKey(self):
			self.Close()
			return True

	GLOBAL_CHAT_INDEX = 10

	class ChatLogWindowNew(ui.Window):
		CHAT_LOG_WINDOW_MINIMUM_WIDTH = 450
		CHAT_LOG_WINDOW_MINIMUM_HEIGHT = 120
		CHAT_POSITION = 25
		BLOCK_WIDTH = 32

		class ResizeButton(ui.DragButton):
			def __init__(self):
				ui.DragButton.__init__(self)
			def __del__(self):
				ui.DragButton.__del__(self)
			def OnMouseOverIn(self):
				app.SetCursor(app.HVSIZE)
			def OnMouseOverOut(self):
				app.SetCursor(app.NORMAL)

		def __init__(self):
			ui.Window.__init__(self)
			self.AddFlag("float")
			self.AddFlag("movable")
			self.AddFlag("animate")
			self.SetWindowName("ChatLogWindowNew")
			self.chatID = 0

			self.__CreateWindow()
			self.__CreateScrollBar()

			self.SetPosition(20, 20)
			self.SetSize(self.CHAT_LOG_WINDOW_MINIMUM_WIDTH, self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT)
			self.btnSizing.SetPosition(self.CHAT_LOG_WINDOW_MINIMUM_WIDTH-self.btnSizing.GetWidth(), self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT-self.btnSizing.GetHeight()+2)
			self.OnResize()

		def __del__(self):
			ui.Window.__del__(self)

		def Destroy(self):
			self.BLOCK_WIDTH = 0
			self.CHAT_MODE_INDEX =0
			self.CHAT_LOG_WINDOW_MINIMUM_WIDTH = 0
			self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT = 0
			self.allChatMode = 0
			self.chatID = 0
			self.imgLeft = 0
			self.imgCenter = 0
			self.imgRight = 0
			self.btnClose = 0
			self.btnSizing = 0
			self.titleName = 0
			self.btnAll = 0
			self.modeButtonList = 0
			self.scrollBar = 0
			self.scrollBarPos = 0
			self.Hide()

		def __CreateWindow(self):
			imgLeft = ui.ImageBox()
			imgLeft.AddFlag("not_pick")
			imgLeft.SetParent(self)				

			imgCenter = ui.ExpandedImageBox()
			imgCenter.AddFlag("not_pick")
			imgCenter.SetParent(self)

			imgRight = ui.ImageBox()
			imgRight.AddFlag("not_pick")
			imgRight.SetParent(self)
			imgLeft.LoadImage("d:/ymir work/ui/pattern/titlebar_left.tga")
			imgCenter.LoadImage("d:/ymir work/ui/pattern/titlebar_center.tga")
			imgRight.LoadImage("d:/ymir work/ui/pattern/titlebar_right.tga")

			imgLeft.Show()
			imgCenter.Show()
			imgRight.Show()

			btnClose = ui.Button()
			btnClose.SetParent(self)
			btnClose.SetUpVisual("d:/ymir work/ui/public/close_button_01.sub")
			btnClose.SetOverVisual("d:/ymir work/ui/public/close_button_02.sub")
			btnClose.SetDownVisual("d:/ymir work/ui/public/close_button_03.sub")
			btnClose.SetToolTipText(localeInfo.UI_CLOSE, 0, -20)
			btnClose.SetEvent(ui.__mem_func__(self.Close))
			btnClose.Show()

			btnAdd = ui.Button()
			btnAdd.SetParent(self)
			btnAdd.SetUpVisual("d:/ymir work/ui/chat/btn_hide01_default.tga")
			btnAdd.SetOverVisual("d:/ymir work/ui/chat/btn_hide01_over.tga")
			btnAdd.SetDownVisual("d:/ymir work/ui/chat/btn_hide01_down.tga")
			btnAdd.SetToolTipText(localeInfo.CHATTING_SETTING_DEL, 0, -40)
			btnAdd.SetEvent(ui.__mem_func__(self.Add))
			btnAdd.Show()

			btnSizing = self.ResizeButton()
			btnSizing.SetParent(self)
			btnSizing.SetMoveEvent(ui.__mem_func__(self.OnResize))
			btnSizing.SetSize(16, 16)
			btnSizing.Show()

			titleName = ui.TextLine()
			titleName.SetParent(self)
			titleName.SetPosition(20, 3)
			titleName.Show()

			self.imgLeft = imgLeft
			self.imgCenter = imgCenter
			self.imgRight = imgRight
			self.btnClose = btnClose
			self.btnSizing = btnSizing
			self.titleName = titleName
			self.btnAdd = btnAdd

			self.backupData = {}
			self.chatIndex = -1

		def __CreateScrollBar(self):
			scrollBar = ui.SmallThinScrollBar()
			scrollBar.SetParent(self)
			scrollBar.Show()
			scrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
			self.scrollBar = scrollBar
			self.scrollBarPos = 1.0

		def SetSize(self, width, height):
			self.imgCenter.SetRenderingRect(0.0, 0.0, float((width - self.BLOCK_WIDTH*2) - self.BLOCK_WIDTH) / self.BLOCK_WIDTH, 0.0)
			self.imgCenter.SetPosition(self.BLOCK_WIDTH, 0)
			self.imgRight.SetPosition(width - self.BLOCK_WIDTH, 0)
			self.btnClose.SetPosition(width - self.btnClose.GetWidth()-3, 3)
			self.btnAdd.SetPosition(width - 42, 0)
			self.scrollBar.SetPosition(width - 15, 22)
			self.scrollBar.SetScrollBarSize(height - 22 - 12)
			self.scrollBar.SetPos(self.scrollBarPos)
			ui.Window.SetSize(self, width, height)

		def UpdateChat(self):
			CHAT_MODE_INDEX = [ chat.CHAT_TYPE_TALKING,chat.CHAT_TYPE_PARTY,chat.CHAT_TYPE_GUILD,chat.CHAT_TYPE_SHOUT,chat.CHAT_TYPE_INFO,chat.CHAT_TYPE_NOTICE]
			CHAT_MODE_INDEX.append(chat.CHAT_TYPE_EXP_INFO)
			CHAT_MODE_INDEX.append(chat.CHAT_TYPE_ITEM_INFO)
			CHAT_MODE_INDEX.append(chat.CHAT_TYPE_MONEY_INFO)

			for i in CHAT_MODE_INDEX:
				chat.DisableChatMode(self.chatID, i)
			dataChat = self.backupData
			for key, data in constInfo.OPTION_CHECKBOX_MODE.items():
				if dataChat[key] > 0:
					for chatIndex in data[0]:
						chat.EnableChatMode(self.chatID, chatIndex)

			for j in range(1,5):
				chat.DisableCountryMode(self.chatID, j)

			if dataChat[constInfo.OPTION_CHECKBOX_FILTER] == 1:
				for country in range(constInfo.OPTION_CHECKBOX_FLAG_EN, constInfo.OPTION_MAX):
					if dataChat[country] > 0:
						chat.EnableCountryMode(self.chatID, dataChat[country])
			else:
				for j in range(1,5):
					chat.EnableCountryMode(self.chatID, j)

		def Open(self, chatIndex = -1):

			if constInfo.cacheChat.has_key(chatIndex):
				global GLOBAL_CHAT_INDEX
				GLOBAL_CHAT_INDEX+=1
				self.chatID = chat.CreateChatSet(GLOBAL_CHAT_INDEX)
				chat.SetBoardState(self.chatID, chat.BOARD_STATE_LOG)

				self.chatIndex = int(chatIndex)
				chatData = constInfo.cacheChat[chatIndex]
				self.backupData = chatData

				if chatData[0] == "0" and chatIndex == 0:
					self.titleName.SetText(localeInfo.CHATTING_SETTING_TALKING)
				else:
					self.titleName.SetText(chatData[0])
				self.UpdateChat()
			self.OnResize()
			self.Show()

		def AddReal(self):
			if self.chatIndex != 0:
				if len(constInfo.cacheChat) == 10:
					#chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHATTING_SETTING_ADD_MAX)
					return
				#lasyKey = 0
				#for key, data in constInfo.cacheChat.items():
				#	lasyKey = key
				#constInfo.cacheChat[lasyKey+1] = self.backupData
				constInfo.cacheChat[self.chatIndex] = self.backupData
			
		def Add(self):
			if self.chatIndex != 0:
				if len(constInfo.cacheChat) == 10:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHATTING_SETTING_ADD_MAX)
					return

				#lasyKey = 0
				#for key, data in constInfo.cacheChat.items():
				#	lasyKey = key

				#constInfo.cacheChat[lasyKey+1] = self.backupData
				constInfo.cacheChat[self.chatIndex] = self.backupData

				interface = constInfo.GetInterfaceInstance()
				if interface != None:
					if interface.wndChat:
						interface.wndChat.LoadChatLoby()

			#self.Destroy()
			self.Close()
			#interface = constInfo.GetInterfaceInstance()
			#if interface != None:
			#	if interface.wndChat:
			#		del interface.wndChat.ChatLog[self.chatIndex]


		def Close(self, isAuto = False):
			self.Hide()
			chat.DeleteChatSet(self.chatID)

			if isAuto == False:
				interface = constInfo.GetInterfaceInstance()
				if interface != None:
					if interface.wndChat:
						del interface.wndChat.ChatLog[self.chatIndex]

		def OnResize(self):
			x, y = self.btnSizing.GetLocalPosition()
			width = self.btnSizing.GetWidth()
			height = self.btnSizing.GetHeight()

			if x < self.CHAT_LOG_WINDOW_MINIMUM_WIDTH - width:
				self.btnSizing.SetPosition(self.CHAT_LOG_WINDOW_MINIMUM_WIDTH - width, y)
				return
			if y < self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT - height:
				self.btnSizing.SetPosition(x, self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT - height)
				return

			self.scrollBar.LockScroll()
			self.SetSize(x + width, y + height)
			self.scrollBar.UnlockScroll()

		def OnMouseWheel(self, nLen):
			if self.scrollBar:
				if nLen > 0:
					self.scrollBar.OnUp()
				else:
					self.scrollBar.OnDown()
				return True
			return False

		def OnScroll(self):
			self.scrollBarPos = self.scrollBar.GetPos()

			lineCount = chat.GetLineCount(self.chatID)
			visibleLineCount = chat.GetVisibleLineCount(self.chatID)
			endLine = visibleLineCount + int(float(lineCount - visibleLineCount) * self.scrollBarPos)

			chat.SetEndPos(self.chatID, self.scrollBarPos)

		def OnRender(self):
			(x, y, width, height) = self.GetRect()
			grp.SetColor(0x77000000)
			grp.RenderBar(x+width-15, y+22, 13, height-22)

			grp.SetColor(0x77000000)
			grp.RenderBar(x, y, width, height)
			grp.SetColor(0xff525552)
			grp.RenderBox(x, y, width-2, height)
			grp.SetColor(0xff000000)
			grp.RenderBox(x+1, y+1, width-2, height)

			grp.SetColor(0xff989898)
			grp.RenderLine(x+width-13, y+height-1, 11, -11)
			grp.RenderLine(x+width-9, y+height-1, 7, -7)
			grp.RenderLine(x+width-5, y+height-1, 3, -3)

			chat.ArrangeShowingChat(self.chatID)
			chat.SetPosition(self.chatID, x + 10, y + height - self.CHAT_POSITION)
			chat.SetHeight(self.chatID, height - 22 - self.CHAT_POSITION)
			chat.Update(self.chatID)
			chat.Render(self.chatID)

		def OnPressEscapeKey(self):
			self.Close()
			return TRUE

		def OnMouseLeftButtonDown(self):
			hyperlink = ui.GetHyperlink()
			if hyperlink:
				if app.IsPressed(app.DIK_LALT):
					link = chat.GetLinkFromHyperlink(hyperlink)
					ime.PasteString(link)
				else:
					interface = constInfo.GetInterfaceInstance()
					if interface != None:
						interface.MakeHyperlinkTooltip(hyperlink)
