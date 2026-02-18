# coding: latin_1

import ui
import uiScriptLocale
import wndMgr
import player
import miniMap
import localeInfo
import net
import app
import colorInfo
import constInfo
import background
import time
import interfacemodule

if app.ENABLE_IKASHOP_RENEWAL:
	import ikashop

def FormatTime(seconds):
	if seconds <= 0:
		return "0s"
	m, s = divmod(seconds, 60)
	h, m = divmod(m, 60)
	d, h = divmod(h, 24)
	text = ""
	if d > 0:
		text+= str(d)+"d "
	if h > 0:
		text+= str(h)+"h "
	if m > 0:
		text+= str(m)+"m "
	if s > 0:
		text+= str(s)+"s "
	return text

class MapTextToolTip(ui.Window):
	def __init__(self):
		ui.Window.__init__(self)

		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetHorizontalAlignCenter()
		textLine.SetOutline()
		textLine.SetHorizontalAlignRight()
		textLine.Show()
		self.textLine = textLine

	def __del__(self):
		ui.Window.__del__(self)

	def SetText(self, text):
		self.textLine.SetText(text)

	def SetTooltipPosition(self, PosX, PosY):
		if localeInfo.IsARABIC():
			w, h = self.textLine.GetTextSize()
			self.textLine.SetPosition(PosX - w - 5, PosY)
		else:
			self.textLine.SetPosition(PosX - 5, PosY)

	def SetTextColor(self, TextColor):
		self.textLine.SetPackedFontColor(TextColor)

	def GetTextSize(self):
		return self.textLine.GetTextSize()

class AtlasWindow(ui.ScriptWindow):

	class AtlasRenderer(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
			self.AddFlag("not_pick")

		def OnUpdate(self):
			miniMap.UpdateAtlas()

		def OnRender(self):
			(x, y) = self.GetGlobalPosition()
			fx = float(x)
			fy = float(y)
			miniMap.RenderAtlas(fx, fy)

		def HideAtlas(self):
			miniMap.HideAtlas()

		def ShowAtlas(self):
			miniMap.ShowAtlas()

	def __init__(self):
		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Hide()
		self.tooltipInfoEx = MapTextToolTip()
		self.tooltipInfoEx.Hide()
		self.infoGuildMark = ui.MarkBox()
		self.infoGuildMark.Hide()
		self.AtlasMainWindow = None
		self.mapName = ""
		self.board = 0

		ui.ScriptWindow.__init__(self)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def SetMapName(self, mapName):
		if 949==app.GetDefaultCodePage():
			try:
				self.board.SetTitleName(localeInfo.MINIMAP_ZONE_NAME_DICT[mapName])
			except:
				pass

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/AtlasWindow.py")
		except:
			import exception
			exception.Abort("AtlasWindow.LoadWindow.LoadScript")

		try:
			self.board = self.GetChild("board")

		except:
			import exception
			exception.Abort("AtlasWindow.LoadWindow.BindObject")

		self.AtlasMainWindow = self.AtlasRenderer()
		self.board.SetCloseEvent(self.Hide)
		self.AtlasMainWindow.SetParent(self.board)
		self.AtlasMainWindow.SetPosition(7, 30)
		self.tooltipInfo.SetParent(self.board)
		self.tooltipInfoEx.SetParent(self.board)
		self.infoGuildMark.SetParent(self.board)
		self.SetPosition(wndMgr.GetScreenWidth() - 136 - 256 - 10, 0)
		self.Hide()

		miniMap.RegisterAtlasWindow(self)

	def Destroy(self):
		miniMap.UnregisterAtlasWindow()
		self.ClearDictionary()
		self.AtlasMainWindow = None
		self.tooltipAtlasClose = 0
		self.tooltipInfo = None
		self.tooltipInfoEx = None
		self.infoGuildMark = None
		self.board = None

	def OnUpdate(self):

		if not self.tooltipInfo:
			return

		if not self.infoGuildMark:
			return

		self.infoGuildMark.Hide()
		self.tooltipInfo.Hide()
		self.tooltipInfoEx.Hide()

		if False == self.board.IsIn():
			return

		(mouseX, mouseY) = wndMgr.GetMousePosition()
		(bFind, sName, iPosX, iPosY, dwTextColor, dwGuildID, diRegenTime) = miniMap.GetAtlasInfo(mouseX, mouseY)

		if False == bFind:
			return
		
		(x, y) = self.GetGlobalPosition()

		if "empty_guild_area" == sName:
			sName = localeInfo.GUILD_EMPTY_AREA

		if diRegenTime != 0:# it is boss!
			self.tooltipInfo.SetText(localeInfo.MINIMAP_BOSS_TOOLTIP % FormatTime(diRegenTime))

			self.tooltipInfoEx.SetText("%s(%d, %d)" % (sName, iPosX, iPosY))
			self.tooltipInfoEx.SetTooltipPosition(mouseX - x, mouseY - y + 12)
			self.tooltipInfoEx.SetTextColor(dwTextColor)
			self.tooltipInfoEx.Show()
			self.tooltipInfoEx.SetTop()
		else:
			if localeInfo.IsARABIC() and sName[-1].isalnum():
				self.tooltipInfo.SetText("(%s)%d, %d" % (sName, iPosX, iPosY))
			else:
				self.tooltipInfo.SetText("%s(%d, %d)" % (sName, iPosX, iPosY))
			
			self.tooltipInfo.SetTextColor(dwTextColor)

		self.tooltipInfo.SetTooltipPosition(mouseX - x, mouseY - y)
		self.tooltipInfo.SetTextColor(dwTextColor)
		self.tooltipInfo.Show()
		self.tooltipInfo.SetTop()

		if 0 != dwGuildID:
			textWidth, textHeight = self.tooltipInfo.GetTextSize()
			self.infoGuildMark.SetIndex(dwGuildID)
			self.infoGuildMark.SetPosition(mouseX - x - textWidth - 18 - 5, mouseY - y)
			self.infoGuildMark.Show()

	def Hide(self):
		if self.AtlasMainWindow:
			self.AtlasMainWindow.HideAtlas()
			self.AtlasMainWindow.Hide()
		ui.ScriptWindow.Hide(self)

	def Show(self):
		if self.AtlasMainWindow:
			(bGet, iSizeX, iSizeY) = miniMap.GetAtlasSize()
			if bGet:
				self.SetSize(iSizeX + 15, iSizeY + 38)

				if localeInfo.IsARABIC():
					self.board.SetPosition(iSizeX+15, 0)

				self.board.SetSize(iSizeX + 15, iSizeY + 38)
				#self.AtlasMainWindow.SetSize(iSizeX, iSizeY)
				self.AtlasMainWindow.ShowAtlas()
				self.AtlasMainWindow.Show()
		ui.ScriptWindow.Show(self)

	def SetCenterPositionAdjust(self, x, y):
		self.SetPosition((wndMgr.GetScreenWidth() - self.GetWidth()) / 2 + x, (wndMgr.GetScreenHeight() - self.GetHeight()) / 2 + y)

	def OnPressEscapeKey(self):
		self.Hide()
		return True

if app.ENABLE_SUNG_MAHI_TOWER:
	class MovableText(ui.TextLine):
		def __init__(self):
			ui.TextLine.__init__(self)
			
			try:
				self.__Initialize()
				self.__LoadObjects()
			
			except:
				import exception
				exception.Abort("MovableText.__init__")
		
		def __del__(self):
			ui.TextLine.__del__(self)

		def __Initialize(self):
			self.posX = 0
			self.posY = 0
			
			self.defaultText = ""
			self.default_x_pos = 0
			self.default_y_pos = 0
			
			self.updateContor = 0
			self.defaultTextSize = 0
			
			self.hideParameter = 0
			
		def __LoadObjects(self):
			self.SetPosition(115, 57)
			self.SetText("")
			self.Show()
			
			self.defaultTextSize = self.GetTextSize()[0]
		
		def SetPos(self, x, y, hide_parameter):
			self.SetPosition(x, y)
			
			self.posX = x
			self.posY = y
			
			self.default_x_pos = x
			self.default_y_pos = y
			
			self.hideParameter = hide_parameter
			
		def SetTextInfo(self, text):
			self.SetText(text)
			self.defaultText = text
			self.defaultTextSize = self.GetTextSize()[0]
		
		def GetTextX(self):
			return self.posX
		
		def GetTextY(self):
			return self.posY
		
		def SetTextPos(self, value):
			self.posX = value
		
		def GetDefaultText(self):
			return self.defaultText
			
		def GetLevelDescriptionUpdate(self):
			return self.updateContor
			
		def SeUpdateContor(self, value):
			self.updateContor = value
			
		def GetDefaultTextSize(self):
			return self.defaultTextSize
			
		def UpdateRender(self):
			defaultText = self.GetDefaultText()
			
			if not self.IsShow():
				self.Show()
			
			if self.GetTextX() < (self.hideParameter - self.GetDefaultTextSize()):
				self.SetTextPos(self.default_x_pos)
				self.SeUpdateContor(0)
				
				self.SetText(self.GetDefaultText())
				self.Hide()
			
			if self.GetTextX() < self.hideParameter:
				if self.GetTextX() % 5 == 0:
					textSize = len(defaultText)
					self.SeUpdateContor(self.GetLevelDescriptionUpdate() + 1)
					
					self.SetTextPos(self.GetTextX() + (self.GetCharSize() - 1))
					self.SetText(defaultText[self.GetLevelDescriptionUpdate():textSize])
					
			self.SetPosition((self.GetTextX() - 1), self.GetTextY())
			self.SetTextPos(self.GetTextX() - 1)
			
	class SungMahiCover(ui.ImageBox):
		def __init__(self):
			ui.ImageBox.__init__(self)
			
			try:
				self.__Initialize()
				self.__LoadObjects()
			
			except:
				import exception
				exception.Abort("SungMahiCover.__init__")
		
		def __del__(self):
			ui.ImageBox.__del__(self)

		def __Initialize(self):
			self.levelDescription = None
			self.levelCurse = None
			
			self.minimapBG = None
			self.roomLevel = []
			
			self.levelInfo = None
			self.timeGauge = None
			self.timeGaugeText = None
			
			self.exitDlgBox = None
			self.dlgBoxText = None
			self.dlgBoxAcceptButton = None
			self.exitButton = None
			
			self.levelCurseBG = None
			self.levelCurseIcon = None
			
			self.textUpdateTime = 0
			self.gaugeUpTime = 0
			self.gaugeGlobalTime = 0
			
		def __LoadObjects(self):
			self.LoadImage("d:/ymir work/ui/game/sungmahee_tower/information_bg.sub")
			
			self.levelDescription = MovableText()
			self.levelDescription.SetParent(self)
			self.levelDescription.Hide()
			
			self.minimapBG = ui.ImageBox()
			self.minimapBG.SetParent(self)
			self.minimapBG.LoadImage("d:/ymir work/ui/game/sungmahee_tower/smhtower_bg.png")
			self.minimapBG.Show()
			
			roomLevelPos = [[17, 18], [47, 12], [88, 18]]
			for index in xrange(3):
				roomLevel = ui.ImageBox()
				roomLevel.SetParent(self)
				roomLevel.LoadImage("d:/ymir work/ui/game/sungmahee_tower/{}stage_clear_icon.sub".format(index + 1))
				roomLevel.SetPosition(roomLevelPos[index][0], roomLevelPos[index][1])
				roomLevel.Hide()
				
				self.roomLevel.append(roomLevel)
			
			self.levelInfo = ui.TextLine()
			self.levelInfo.SetParent(self)
			self.levelInfo.SetPosition(68, 36)
			self.levelInfo.SetHorizontalAlignCenter()
			self.levelInfo.SetPackedFontColor(0xffffaf00)
			self.levelInfo.SetText("L50")
			self.levelInfo.Show()
			
			self.timeGauge = ui.ExpandedImageBox()
			self.timeGauge.SetParent(self)
			self.timeGauge.SetPosition(16, 78)
			self.timeGauge.LoadImage("d:/ymir work/ui/game/sungmahee_tower/information_gauge.sub")
			self.timeGauge.SetPercentage(0, 0)
			self.timeGauge.Show()
			
			self.timeGaugeText = ui.TextLine()
			self.timeGaugeText.SetParent(self.timeGauge)
			self.timeGaugeText.SetPosition(53, 0)
			self.timeGaugeText.SetHorizontalAlignCenter()
			self.timeGaugeText.Show()
			
			self.exitDlgBox = ui.Board()
			self.exitDlgBox.SetSize(300, 100)
			self.exitDlgBox.SetPosition((wndMgr.GetScreenWidth() / 2) - 150, (wndMgr.GetScreenHeight() / 2) - 100)
			# self.exitDlgBox.SetCloseEvent(self.DenyExit)
			
			self.dlgBoxText = ui.TextLine()
			self.dlgBoxText.SetParent(self.exitDlgBox)
			self.dlgBoxText.SetPosition(150, 30)
			self.dlgBoxText.SetHorizontalAlignCenter()
			self.dlgBoxText.SetText(uiScriptLocale.SUNGMAHEE_TOWER_DUNGEON_EXIT_QUESTION)
			self.dlgBoxText.Show()
			
			self.dlgBoxDenyButton = ui.Button()
			self.dlgBoxDenyButton.SetParent(self.exitDlgBox)
			self.dlgBoxDenyButton.SetUpVisual("d:/ymir work/ui/public/middle_button_01.sub")
			self.dlgBoxDenyButton.SetOverVisual("d:/ymir work/ui/public/middle_button_02.sub")
			self.dlgBoxDenyButton.SetDownVisual("d:/ymir work/ui/public/middle_button_03.sub")
			self.dlgBoxDenyButton.SetPosition(155, 55)
			self.dlgBoxDenyButton.SetEvent(ui.__mem_func__(self.DenyExit))
			self.dlgBoxDenyButton.SetText(uiScriptLocale.NO)
			self.dlgBoxDenyButton.Show()
			
			self.dlgBoxAcceptButton = ui.Button()
			self.dlgBoxAcceptButton.SetParent(self.exitDlgBox)
			self.dlgBoxAcceptButton.SetUpVisual("d:/ymir work/ui/public/middle_button_01.sub")
			self.dlgBoxAcceptButton.SetOverVisual("d:/ymir work/ui/public/middle_button_02.sub")
			self.dlgBoxAcceptButton.SetDownVisual("d:/ymir work/ui/public/middle_button_03.sub")
			self.dlgBoxAcceptButton.SetPosition(75, 55)
			self.dlgBoxAcceptButton.SetEvent(ui.__mem_func__(self.AcceptExit))
			self.dlgBoxAcceptButton.SetText(uiScriptLocale.YES)
			self.dlgBoxAcceptButton.Show()
			
			self.exitDlgBox.Hide()
			
			self.exitButton = ui.Button()
			self.exitButton.SetParent(self)
			self.exitButton.SetUpVisual("d:/ymir work/ui/game/sungmahee_tower/exit_button_default.sub")
			self.exitButton.SetOverVisual("d:/ymir work/ui/game/sungmahee_tower/exit_button_over.sub")
			self.exitButton.SetDownVisual("d:/ymir work/ui/game/sungmahee_tower/exit_button_down.sub")
			self.exitButton.SetPosition(54, 98)
			self.exitButton.SetEvent(ui.__mem_func__(self.ShowExitDialog))
			self.exitButton.Show()
			
			self.levelCurseBG = ui.ImageBox()
			self.levelCurseBG.SetParent(self)
			self.levelCurseBG.LoadImage("d:/ymir work/ui/game/sungmahee_tower/buff_debuff.sub")
			self.levelCurseBG.SetPosition(15, 140)
			self.levelCurseBG.Hide()
			
			self.levelCurse = MovableText()
			self.levelCurse.SetParent(self)
			self.levelCurse.Hide()
			
			self.levelCurseIcon = ui.ExpandedImageBox()
			self.levelCurseIcon.SetParent(self)
			self.levelCurseIcon.LoadImage("d:/ymir work/ui/skill/common/affect/sungmahee_tower_debuff.sub")
			self.levelCurseIcon.SetScale(0.7, 0.7)
			self.levelCurseIcon.SetPosition(-7, 141)
			self.levelCurseIcon.Hide()
			
			self.Show()
			
		def Destroy(self):
			self.__Initialize()
			
		def SetLevelInfo(self, level):
			self.levelInfo.SetText(uiScriptLocale.SUNGMAHEE_TOWER_INFO_BOARD_FLOOR % int(level))

		def UpdateRoomTime(self, roomTime):
			roomTime = int(roomTime)
			
			self.timeGauge.SetPercentage(roomTime, roomTime)
			self.timeGaugeText.Show()
			
			self.gaugeUpTime = roomTime
			self.gaugeGlobalTime = app.GetGlobalTimeStamp()
			
		def SetRoomLevel(self, level):
			for roomIndex in self.roomLevel:
				roomIndex.Hide()
			
			for index in xrange(int(level)):
				self.roomLevel[index].Show()
			
			#since it's completing a stage, reset the gauge
			self.timeGauge.SetPercentage(0, 0)
			self.timeGaugeText.Hide()
			self.gaugeUpTime = 0
		
		def SetLevelDescriptionText(self, noticeText):
			self.levelDescription.SetPos(115, 57, 17)
			self.levelDescription.SetTextInfo(noticeText)
			self.levelDescription.Show()
			
		def SetLevelCurseText(self, noticeText, noticeType = 2):
			self.levelCurse.SetPos(134, 144, 5)
			self.levelCurse.SetTextInfo(noticeText)
			
			self.levelCurse.Show()
			self.levelCurseBG.Show()
			
			iconPath = (noticeType > 2 and "d:/ymir work/ui/skill/common/affect/sungmahee_tower_debuff.sub" or "d:/ymir work/ui/skill/common/affect/sungmahee_tower_buff.sub")
			self.levelCurseIcon.LoadImage(iconPath)
			self.levelCurseIcon.SetScale(0.7, 0.7)
			
			self.levelCurseIcon.Show()
		
		def SungMahiClearNotice(self, noticeType):
			if noticeType == 1:
				self.SetLevelDescriptionText("")
			else:
				self.SetLevelCurseText("")
				
				self.levelCurse.Hide()
				self.levelCurseBG.Hide()
				self.levelCurseIcon.Hide()
				
		def AcceptExit(self):
			net.SendChatPacket("/restart_here")
			self.DenyExit()
		
		def DenyExit(self):
			if self.exitDlgBox.IsShow():
				self.exitDlgBox.Hide()
		
		def ShowExitDialog(self):
			self.exitDlgBox.Show()
		
		def Update(self):
			curTime = app.GetTime()
			if (self.textUpdateTime - curTime) <= 1:
				self.textUpdateTime = (curTime + 1.01)
				
				actualTime = (app.GetGlobalTimeStamp() - self.gaugeGlobalTime)
				if actualTime <= self.gaugeUpTime:
					self.timeGauge.SetPercentage((self.gaugeUpTime - actualTime), self.gaugeUpTime)
					endTime = self.gaugeUpTime - actualTime
					
					self.timeGaugeText.SetText("{}".format(localeInfo.SecondToM(endTime)))
					if endTime < 60:
						self.timeGaugeText.SetText("< {}".format(localeInfo.SecondToM(endTime)))
					
				self.levelDescription.UpdateRender()
				self.levelCurse.UpdateRender()


def __RegisterMiniMapColor(type, rgb):
	miniMap.RegisterColor(type, rgb[0], rgb[1], rgb[2])

class MiniMap(ui.ScriptWindow):

	CANNOT_SEE_INFO_MAP_DICT = {
		# "metin2_map_monkeydungeon" : False,
		# "metin2_map_monkeydungeon_02" : False,
		# "metin2_map_monkeydungeon_03" : False,
	}

	MAPS_GLOBAL = {
		"metin2_map_a1",
		"metin2_map_c1",
		"metin2_map_b1",
		"metin2_map_battleroyale"
	}

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__Initialize()

		miniMap.Create()
		miniMap.SetScale(2.0)

		self.AtlasWindow = AtlasWindow()
		self.AtlasWindow.LoadWindow()
		self.AtlasWindow.Hide()

		if app.ENABLE_SUNG_MAHI_TOWER:
			self.sungMahiCover = SungMahiCover()
			self.sungMahiCover.SetParent(self)
			self.sungMahiCover.SetPosition(3, 0)
			self.sungMahiCover.Hide()

		self.interface = None

		self.tooltipMiniMapOpen = MapTextToolTip()
		self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP)
		#self.tooltipMiniMapOpen.Show()
		self.tooltipMiniMapClose = MapTextToolTip()
		self.tooltipMiniMapClose.SetText(localeInfo.UI_CLOSE)
		#self.tooltipMiniMapClose.Show()
		self.tooltipScaleUp = MapTextToolTip()
		self.tooltipScaleUp.SetText(localeInfo.MINIMAP_INC_SCALE)
		#self.tooltipScaleUp.Show()
		self.tooltipScaleDown = MapTextToolTip()
		self.tooltipScaleDown.SetText(localeInfo.MINIMAP_DEC_SCALE)
		#self.tooltipScaleDown.Show()
		self.tooltipAtlasOpen = MapTextToolTip()
		self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		#self.tooltipAtlasOpen.Show()

		self.tooltipDungeonInfo = MapTextToolTip()
		self.tooltipDungeonInfo.SetText(localeInfo.SYSTEM_DUNGEON_BUTTON)
		#self.tooltipDungeonInfo.Show()


		
		self.tooltipBattlePass = MapTextToolTip()
		self.tooltipBattlePass.SetText(localeInfo.BATTLEPASS_TITLE)
		#self.tooltipBattlePass.Show()

		if app.ENABLE_BIYOLOG:
			self.tooltipBiolog = MapTextToolTip()
			self.tooltipBiolog.SetText(localeInfo.BIO_TITLE)
			#self.tooltipBiolog.Show()

		self.tooltipwiki = MapTextToolTip()
		self.tooltipwiki.SetText(localeInfo.WIKI_TITLE)

		self.tooltiprank = MapTextToolTip()
		self.tooltiprank.SetText(localeInfo.RANK_TITLE)

		self.tooltipsearch = MapTextToolTip()
		self.tooltipsearch.SetText(localeInfo.SEARCH_TITLE)

		self.tooltipevent = MapTextToolTip()
		self.tooltipevent.SetText(localeInfo.EVENTBTNMAP)

		self.toolTipTotalWar = MapTextToolTip()
		self.toolTipTotalWar.SetText("Panel Total War")
		#self.toolTipTotalWar.Show()

		self.tooltipAtlasOpen = MapTextToolTip()
		self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		#self.tooltipAtlasOpen.Show()

		self.tooltipInfo = MapTextToolTip()
		#self.tooltipInfo.Show()

		if miniMap.IsAtlas():
			self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		else:
			self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_CAN_NOT_SHOW_AREAMAP)

		self.mapName = ""

		self.isLoaded = 0
		self.canSeeInfo = True

		# AUTOBAN
		self.imprisonmentDuration = 0
		self.imprisonmentEndTime = 0
		self.imprisonmentEndTimeText = ""
		# END_OF_AUTOBAN

	def __del__(self):
		miniMap.Destroy()
		ui.ScriptWindow.__del__(self)

	def BindInterface(self, interface):
		self.interface = interface

	def __Initialize(self):
		self.positionInfo = 0
		if app.__BL_BATTLE_ROYALE__:
			self.ClientTimer = 0
			self.ClientTimerText = 0
		self.observerCount = 0

		self.OpenWindow = 0
		self.CloseWindow = 0
		self.ScaleUpButton = 0
		self.ScaleDownButton = 0
		self.MiniMapHideButton = 0
		self.MiniMapShowButton = 0
		self.AtlasShowButton = 0
		self.tooltipMiniMapOpen = 0
		self.tooltipMiniMapClose = 0
		self.tooltipScaleUp = 0
		self.tooltipScaleDown = 0
		self.tooltipAtlasOpen = 0

		if (app.WJ_COMBAT_ZONE):
			self.btnCombatZone = 0

		self.tooltipDungeonInfo = 0
		self.toolTipTotalWar = 0
		self.tooltipBattlePass = None
		if app.ENABLE_BIYOLOG:
			self.tooltipBiolog = None

		self.tooltipwiki = 0
		self.tooltiprank = 0
		self.tooltipsearch = 0
		self.tooltipevent = 0

		self.tooltipInfo = None
		self.serverInfo = None
		# self.evenimente = None
		self.Hora = None
		if app.ENABLE_SUNG_MAHI_TOWER:
			self.sungMahiCover = None

	def SetMapName(self, mapName):
		self.mapName=mapName
		self.AtlasWindow.SetMapName(mapName)

		if app.TOURNAMENT_PVP_SYSTEM:
			if player.IsTournamentMap():
				self.canSeeInfo = FALSE
				self.HideMiniMap()
				ui.ScriptWindow.Hide(self)
				
		if self.CANNOT_SEE_INFO_MAP_DICT.has_key(mapName):
			self.canSeeInfo = False
			self.HideMiniMap()
			self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP_CANNOT_SEE)
		else:
			self.canSeeInfo = True
			self.ShowMiniMap()
			self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP)

	# AUTOBAN
	def SetImprisonmentDuration(self, duration):
		self.imprisonmentDuration = duration
		self.imprisonmentEndTime = app.GetGlobalTimeStamp() + duration

		self.__UpdateImprisonmentDurationText()

	def __UpdateImprisonmentDurationText(self):
		restTime = max(self.imprisonmentEndTime - app.GetGlobalTimeStamp(), 0)

		imprisonmentEndTimeText = localeInfo.SecondToDHM(restTime)
		if imprisonmentEndTimeText != self.imprisonmentEndTimeText:
			self.imprisonmentEndTimeText = imprisonmentEndTimeText
			self.serverInfo.SetText("%s: %s" % (uiScriptLocale.AUTOBAN_QUIZ_REST_TIME, self.imprisonmentEndTimeText))
	# END_OF_AUTOBAN


	def Show(self):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			if localeInfo.IsARABIC():
				pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "Minimap.py")
			else:
				pyScrLoader.LoadScriptFile(self, "UIScript/MiniMap.py")
		except:
			import exception
			exception.Abort("MiniMap.LoadWindow.LoadScript")

		try:
			self.OpenWindow = self.GetChild("OpenWindow")
			self.MiniMapWindow = self.GetChild("MiniMapWindow")
			self.ScaleUpButton = self.GetChild("ScaleUpButton")
			self.ScaleDownButton = self.GetChild("ScaleDownButton")
			self.MiniMapHideButton = self.GetChild("MiniMapHideButton")
			self.AtlasShowButton = self.GetChild("AtlasShowButton")
			self.CloseWindow = self.GetChild("CloseWindow")
			self.MiniMapShowButton = self.GetChild("MiniMapShowButton")
			#self.positionInfo = self.GetChild("PositionInfo")
			self.observerCount = self.GetChild("ObserverCount")
			self.serverInfo = self.GetChild("ServerInfo")
			if app.__BL_BATTLE_ROYALE__:
				self.ClientTimerText = self.GetChild("ClientTimerText")
			# if app.ENABLE_ZODIAC_MISSION:
				# self.bead = self.GetChild("bead")
				# self.beadInfo = self.GetChild("beadInfo")
			# self.evenimente = self.GetChild("ButonEveniment")
			if app.ENABLE_BIYOLOG:
				self.GetChild("bio").SetEvent(ui.__mem_func__(self.OpenBio))
			self.Hora = self.GetChild("Hora")

			if (app.WJ_COMBAT_ZONE):
				self.btnCombatZone = self.GetChild("BattleButton")

			if app.__DUNGEON_INFO__:
				self.btnDungeonSystem = self.GetChild("DungeonSystemButton")

			self.btnWiki = self.GetChild("WikiButton")

			self.btnRank = self.GetChild("OpenRewardWindow")

			self.btnSearch = self.GetChild("SearchButton")

			self.GetChild("battlepass").SetEvent(ui.__mem_func__(self.OpenBattlePass))

		except:
			import exception
			exception.Abort("MiniMap.LoadWindow.Bind")

		#if constInfo.MINIMAP_POSITIONINFO_ENABLE==0:
			#self.positionInfo.Hide()
		if app.__BL_BATTLE_ROYALE__:
			self.ClientTimerText.Hide()
		
		self.ScaleUpButton.SetEvent(ui.__mem_func__(self.ScaleUp))
		self.ScaleDownButton.SetEvent(ui.__mem_func__(self.ScaleDown))
		self.MiniMapHideButton.SetEvent(ui.__mem_func__(self.HideMiniMap))
		self.MiniMapShowButton.SetEvent(ui.__mem_func__(self.ShowMiniMap))
		if (app.WJ_COMBAT_ZONE):
			self.btnCombatZone.SetEvent(ui.__mem_func__(self.OpenCombatZoneWindow))
			self.btnCombatZone.Down()

		# self.evenimente.SetEvent(ui.__mem_func__(self.ShowEvenimente))

		if app.__DUNGEON_INFO__:
			self.btnDungeonSystem.SetEvent(ui.__mem_func__(self.OpenTableDungeonInfo))

		self.btnWiki.SetEvent(ui.__mem_func__(self.OpenWikiWindow))

		self.btnRank.SetEvent(ui.__mem_func__(self.OpenRankWindow))

		self.btnSearch.SetEvent(ui.__mem_func__(self.ShowShopSearch))

		if miniMap.IsAtlas():
			self.AtlasShowButton.SetEvent(ui.__mem_func__(self.ShowAtlas))

		(ButtonPosX, ButtonPosY) = self.MiniMapShowButton.GetGlobalPosition()
		self.tooltipMiniMapOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.MiniMapHideButton.GetGlobalPosition()
		self.tooltipMiniMapClose.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.ScaleUpButton.GetGlobalPosition()
		self.tooltipScaleUp.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.ScaleDownButton.GetGlobalPosition()
		self.tooltipScaleDown.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.AtlasShowButton.GetGlobalPosition()
		self.tooltipAtlasOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)


		(ButtonPosX, ButtonPosY) = self.btnDungeonSystem.GetGlobalPosition()
		self.tooltipDungeonInfo.SetTooltipPosition(ButtonPosX, ButtonPosY)
		
		(ButtonPosX, ButtonPosY) = self.GetChild("battlepass").GetGlobalPosition()
		self.tooltipBattlePass.SetTooltipPosition(ButtonPosX, ButtonPosY)

		if app.ENABLE_BIYOLOG:
			(ButtonPosX, ButtonPosY) = self.GetChild("bio").GetGlobalPosition()
			self.tooltipBiolog.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.btnWiki.GetGlobalPosition()
		self.tooltipwiki.SetTooltipPosition(ButtonPosX, ButtonPosY)
		
		(ButtonPosX, ButtonPosY) = self.btnRank.GetGlobalPosition()
		self.tooltiprank.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.btnSearch.GetGlobalPosition()
		self.tooltipsearch.SetTooltipPosition(ButtonPosX, ButtonPosY)

		# (ButtonPosX, ButtonPosY) = self.evenimente.GetGlobalPosition()
		# self.tooltipevent.SetTooltipPosition(ButtonPosX, ButtonPosY)

		self.ShowMiniMap()

		self.GetMapsGlobal()

	def OpenBattlePass(self):
		interface = constInfo.GetInterfaceInstance()
		if interface != None:
			interface.ToggleBattlePass()

	if app.ENABLE_BIYOLOG:
		def OpenBio(self):
			try:
				interface = constInfo.GetInterfaceInstance()
				if interface:
					interface.OpenBiologWindow()
			except:
				pass

	if app.ENABLE_IKASHOP_RENEWAL:
		def ShowShopSearch(self):
			if app.USE_OFFLINESHOP_SEARCH_V2:
				interfaceInst = constInfo.GetInterfaceInstance()
				if interfaceInst:
					interfaceInst.ToggleIkashopSearchShopBoard()
			else:
				board = ikashop.GetSearchShopBoard()
				if board:
					board.Toggle()

	def ShowEvenimente(self):
		try:
			interface = constInfo.GetInterfaceInstance()
			if interface:
				interface.OpenEventCalendar()
		except:
			pass

	def GetMapsGlobal(self):
		for x in self.MAPS_GLOBAL:
			if background.GetCurrentMapName() == x:
				text = net.GetServerInfo().split(",")
				self.serverInfo.SetText("{}, General".format(text[0]))
				break
			else:
				self.serverInfo.SetText(net.GetServerInfo())

	if app.__DUNGEON_INFO__:
		def OpenTableDungeonInfo(self):
			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				interface.OpenDungeonInfo()

	def OpenWikiWindow(self):
		interface = constInfo.GetInterfaceInstance()
		if interface != None:
			interface.OpenWikiWindow()

	def OpenRankWindow(self):
		interface = constInfo.GetInterfaceInstance()
		if interface != None:
			interface.ShowRankInfo()

	def Destroy(self):
		self.HideMiniMap()

		self.AtlasWindow.Destroy()
		self.AtlasWindow = None

		self.ClearDictionary()

		self.__Initialize()

	def UpdateObserverCount(self, observerCount):
		if observerCount>0:
			self.observerCount.Show()
		elif observerCount<=0:
			self.observerCount.Hide()

		self.observerCount.SetText(localeInfo.MINIMAP_OBSERVER_COUNT % observerCount)

	def OnUpdate(self):
		(x, y, z) = player.GetMainCharacterPosition()
		miniMap.Update(x, y)

		cur_time_stamp = app.GetGlobalTimeStamp()
		seconds = cur_time_stamp % 60
		minutes = (cur_time_stamp / 60) % 60
		hours = (cur_time_stamp / 60) / 60 % 24

		self.Hora.SetText("%02i:%02i:%02i - (%.0f, %.0f)" % (hours, minutes, seconds , x/100, y/100))

		if self.tooltipInfo:
			if True == self.MiniMapWindow.IsIn():
				(mouseX, mouseY) = wndMgr.GetMousePosition()
				(bFind, sName, iPosX, iPosY, dwTextColor) = miniMap.GetInfo(mouseX, mouseY)
				if bFind == 0:
					self.tooltipInfo.Hide()
				elif not self.canSeeInfo:
					self.tooltipInfo.SetText("%s(%s)" % (sName, localeInfo.UI_POS_UNKNOWN))
					self.tooltipInfo.SetTooltipPosition(mouseX - 5, mouseY)
					self.tooltipInfo.SetTextColor(dwTextColor)
					self.tooltipInfo.Show()
				else:
					if localeInfo.IsARABIC() and sName[-1].isalnum():
						self.tooltipInfo.SetText("(%s)%d, %d" % (sName, iPosX, iPosY))
					else:
						self.tooltipInfo.SetText("%s(%d, %d)" % (sName, iPosX, iPosY))
					self.tooltipInfo.SetTooltipPosition(mouseX - 5, mouseY)
					self.tooltipInfo.SetTextColor(dwTextColor)
					self.tooltipInfo.Show()
			else:
				self.tooltipInfo.Hide()

			# AUTOBAN
			if self.imprisonmentDuration:
				self.__UpdateImprisonmentDurationText()
			# END_OF_AUTOBAN

		if True == self.MiniMapShowButton.IsIn():
			self.tooltipMiniMapOpen.Show()
		else:
			self.tooltipMiniMapOpen.Hide()

		if True == self.MiniMapHideButton.IsIn():
			self.tooltipMiniMapClose.Show()
		else:
			self.tooltipMiniMapClose.Hide()

		if True == self.ScaleUpButton.IsIn():
			self.tooltipScaleUp.Show()
		else:
			self.tooltipScaleUp.Hide()

		if True == self.ScaleDownButton.IsIn():
			self.tooltipScaleDown.Show()
		else:
			self.tooltipScaleDown.Hide()

		if True == self.AtlasShowButton.IsIn():
			self.tooltipAtlasOpen.Show()
		else:
			self.tooltipAtlasOpen.Hide()

		if app.__BL_BATTLE_ROYALE__:
			leftTime = int(max(0, self.ClientTimer - app.GetTime()))
			leftMin = leftTime/60
			leftSecond = leftTime%60

			if self.ClientTimerText:
				self.ClientTimerText.SetFontColor(0.8549, 0.8549, 0.8549)

				if leftTime > 0:
					if leftTime <= 10:
						self.ClientTimerText.SetFontColor(1.0, 0.0, 0.0)

					self.ClientTimerText.SetText("%02d:%02d" % (leftMin, leftSecond))
					self.ClientTimerText.Show()
				else:
					self.ClientTimerText.Hide()

		if app.ENABLE_SUNG_MAHI_TOWER:
			if self.sungMahiCover.IsShow():
				self.sungMahiCover.Update()

		if True == self.btnDungeonSystem.IsIn():
			self.tooltipDungeonInfo.Show()
		else:
			self.tooltipDungeonInfo.Hide()


		if True == self.GetChild("battlepass").IsIn():
			self.tooltipBattlePass.Show()
		else:
			self.tooltipBattlePass.Hide()

		if app.ENABLE_BIYOLOG:
			if True == self.GetChild("bio").IsIn():
				self.tooltipBiolog.Show()
			else:
				self.tooltipBiolog.Hide()

		if True == self.btnWiki.IsIn():
			self.tooltipwiki.Show()
		else:
			self.tooltipwiki.Hide()

		if True == self.btnRank.IsIn():
			self.tooltiprank.Show()
		else:
			self.tooltiprank.Hide()

		if True == self.btnSearch.IsIn():
			self.tooltipsearch.Show()
		else:
			self.tooltipsearch.Hide()

		# if True == self.evenimente.IsIn():
			# self.tooltipevent.Show()
		# else:
			# self.tooltipevent.Hide()


		#if True == self.btnCombatZone.IsIn():
		#	self.toolTipTotalWar.Show()
		#else:
		#	self.toolTipTotalWar.Hide()

	def OnRender(self):
		(x, y) = self.GetGlobalPosition()
		fx = float(x)
		fy = float(y)
		miniMap.Render(fx + 4.0, fy + 5.0)

	def Close(self):
		self.HideMiniMap()

	def HideMiniMap(self):
		miniMap.Hide()
		self.OpenWindow.Hide()
		self.CloseWindow.Show()
		if app.ENABLE_BIYOLOG:
			self.GetChild("bio").Hide()
		if app.ENABLE_SUNG_MAHI_TOWER:
			if self.sungMahiCover:
				self.sungMahiCover.Hide()

		self.GetChild("battlepass").Hide()
		if app.ENABLE_SUNG_MAHI_TOWER:
			if self.sungMahiCover:
				self.sungMahiCover.Hide()

	if app.__BL_BATTLE_ROYALE__:
		def SetMinimapTimer(self, second):
			self.ClientTimer = app.GetTime() + second

	def ShowMiniMap(self):
		if not self.canSeeInfo:
			return

		miniMap.Show()
		self.OpenWindow.Show()
		self.CloseWindow.Hide()
		if app.ENABLE_BIYOLOG:
			self.GetChild("bio").Show()

		self.GetChild("battlepass").Show()
		if app.ENABLE_SUNG_MAHI_TOWER:
			if self.sungMahiCover and self.mapName == "metin2_map_smhdungeon_02":
				self.HideMiniMap()
				self.CloseWindow.Hide()
				self.sungMahiCover.Show()
				self.GetChild("battlepass").Hide()
				self.GetChild("bio").Hide()
				self.btnDungeonSystem.Hide()
				# self.evenimente.Hide()
				self.btnSearch.Hide()
				self.btnRank.Hide()
				self.btnWiki.Hide()

	if (app.WJ_COMBAT_ZONE):
		def OnAskCombatZoneQuestionDialog(self):
			import uiCommon
			self.combatZoneLeaveQuestionDialog = uiCommon.QuestionDialog2()
			self.combatZoneLeaveQuestionDialog.SetText1(uiScriptLocale.EXIT_BATTLE_FIELD_COLLECTED_POINTS % (player.GetCombatZonePoints()))
			self.combatZoneLeaveQuestionDialog.SetText2(uiScriptLocale.EXIT_BATTLE_FIELD)
			self.combatZoneLeaveQuestionDialog.SetWidth(320)
			self.combatZoneLeaveQuestionDialog.SetAcceptEvent(lambda arg = TRUE: self.OnToggleCombatZoneQuestionDialog(arg))
			self.combatZoneLeaveQuestionDialog.SetCancelEvent(lambda arg = FALSE: self.OnToggleCombatZoneQuestionDialog(arg))
			self.combatZoneLeaveQuestionDialog.Open()
			
		def OnToggleCombatZoneQuestionDialog(self, answer):
			if not self.combatZoneLeaveQuestionDialog:
				return

			self.combatZoneLeaveQuestionDialog.Close()
			self.combatZoneLeaveQuestionDialog = None

			if not answer:
				return

			net.SendCombatZoneRequestActionPacket(net.COMBAT_ZONE_ACTION_LEAVE, net.COMBAT_ZONE_EMPTY_DATA)
			return TRUE
	
		def OpenCombatZoneWindow(self):
			if player.IsCombatZoneMap():
				self.OnAskCombatZoneQuestionDialog()
			else:
				net.SendCombatZoneRequestActionPacket(net.COMBAT_ZONE_ACTION_OPEN_RANKING, net.COMBAT_ZONE_EMPTY_DATA)
				
	def isShowMiniMap(self):
		return miniMap.isShow()

	def ScaleUp(self):
		miniMap.ScaleUp()

	def ScaleDown(self):
		miniMap.ScaleDown()

	def ShowAtlas(self):
		if not miniMap.IsAtlas():
			return
		if not self.AtlasWindow.IsShow():
			self.AtlasWindow.Show()

	def ToggleAtlasWindow(self):
		if not miniMap.IsAtlas():
			return
		if self.AtlasWindow.IsShow():
			self.AtlasWindow.Hide()
		else:
			self.AtlasWindow.Show()

	def IsShowingAtlas(self):
		return self.AtlasWindow.IsShow()
