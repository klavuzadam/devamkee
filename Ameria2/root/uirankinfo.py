import ui
import app
import item
import net
import grp 
import wndMgr
import localeInfo
import player
import mouseModule
import chat
import ui
import item
from _weakref import proxy

DESIGN_PATH = "d:/ymir work/ui/game/rank_new/"

ROOT_PATH = "d:/ymir work/ui/game/ranking/"

EMPIRE_FLAGS = ["locale/ui/flag/shinsoo.tga", # 1
				"locale/ui/flag/chunjo.tga", # 2
				"locale/ui/flag/jinno.tga"]

JOBS = ["Warrior", # Warrior
		"Assasin", # Assasin
		"Sura", # Sura
		"Shaman", # Shaman
		"Wolfman" # Wolfman
]

TEXT_CATEGORY = [localeInfo.RANKING_TEXT_1, 
				localeInfo.RANKING_TEXT_2, 
				localeInfo.RANKING_TEXT_3,
				localeInfo.RANKING_TEXT_4,
				localeInfo.RANKING_TEXT_5,
				localeInfo.RANKING_TEXT_6,
				localeInfo.RANKING_TEXT_7,
				localeInfo.RANKING_TEXT_8,
				localeInfo.RANKING_TEXT_9,
				localeInfo.RANKING_TEXT_10,
				localeInfo.RANKING_TEXT_11,
				localeInfo.RANKING_TEXT_12,
]

MAX_CATEGORY_NUM = len(TEXT_CATEGORY)

class ListBox(ui.Window):
	class NewItem(ui.Window):
		def __init__(self, index):
			ui.Window.__init__(self)
			self.background = None
			self.bSelected = False
			self.OnResetThings()
			self.Index = index

			self.background = ui.ExpandedImageBox()
			self.background.SetParent(self)
			self.background.SetPosition(0, 0)
			self.background.LoadImage("d:/ymir work/ui/game/ranking/left_side/button_category_norm.png")
			self.background.OnMouseLeftButtonDown = ui.__mem_func__(self.OnMouseLeftButtonDownImage)
			self.background.OnMouseOverIn = ui.__mem_func__(self.OnMouseOverImage)
			self.background.OnMouseOverOut = ui.__mem_func__(self.OnMouseOverOutImage)
			self.background.Show()
			
			self.categoryImage = ui.ExpandedImageBox()
			self.categoryImage.SetParent(self)
			self.categoryImage.AddFlag("not_pick")
			self.categoryImage.LoadImage("d:/ymir work/ui/game/ranking/left_side/category_icons/rank_%d.png" % index)
			self.categoryImage.SetPosition(5, 5)
			self.categoryImage.Show()			

			
			self.SetSize(self.background.GetWidth(), self.background.GetHeight())	
			self.textCat = ui.TextLine()
			self.textCat.SetParent(self.background)
			self.textCat.SetPosition(45, 11)
			text = TEXT_CATEGORY[index]
			text1 = text.find(' ')
			self.textCat.SetText(text)
			self.textCat.Show()

		def OnMouseLeftButtonDownImage(self):
			if self.bSelected:
				return
			self.background.LoadImage("d:/ymir work/ui/game/ranking/left_side/button_category_selected.png")

			self.bSelected = True
			if self.clickEvent:
				self.clickEvent(self.Index)

		def OnMouseOverImage(self):
			if self.bSelected:
				return
			self.background.LoadImage("d:/ymir work/ui/game/ranking/left_side/button_category_hover.png")

		def OnMouseOverOutImage(self):
			if self.bSelected:
				return
			self.background.LoadImage("d:/ymir work/ui/game/ranking/left_side/button_category_norm.png")
		
		def __del__(self):
			ui.Window.__del__(self)
			self.OnResetThings()

		def OnResetThings(self):
			self.IsSelected = False
			self.bIsBlocked = False
			self.vnum = 0
			self.xBase = 0
			self.yBase = 0

			self.clickEvent = None
			if self.background != None:
				self.background.Hide()
			self.background = None
			self.categoryImage = None		

		def SetParent(self, parent):
			ui.Window.SetParent(self, parent)
			self.parent = proxy(parent)

		def SetBasePosition(self, x, y):
			self.xBase = x
			self.yBase = y
			
		def GetBasePosition(self):
			return (self.xBase, self.yBase)
			
		def SetClickEvent(self, event):
			self.clickEvent = event

		def OnRender(self):
			xList, yList = self.parent.GetGlobalPosition()
			widthList, heightList = self.parent.GetWidth(), self.parent.GetHeight()	

			images = [self.background, self.categoryImage]
			for img in images:
				if img:
					img.SetClipRect(xList, yList, xList + widthList, yList + heightList)
					# img.Hide()
		
			textList = [self.textCat]
			for text in textList:
				if text:
					xText, yText = text.GetGlobalPosition()

					if yText < yList or yText + text.GetTextSize()[1] > yList + heightList:
						text.Hide()
					else:
						text.Show()
						

	def __init__(self):
		ui.Window.__init__(self)
		self.OnResetThings()

	def __del__(self):
		ui.Window.__del__(self)
		self.OnResetThings()
		
	def Destroy(self):
		self.OnResetThings()
		
	def OnResetThings(self):
		self.SetFuncDown = None
		self.SelectIndexFunc = None
		self.itemList = []
		self.scrollBar = None

		self.selectEvent = None
		self.selectedItemVnum = 0
	
	def SetEventSelect(self, event):
		self.SelectIndexFunc = event
	
	def SetParent(self, parent):
		ui.Window.SetParent(self, parent)
		
		self.SetPosition(5, 5)
		self.SetSize(parent.GetWidth() - 10, parent.GetHeight() - 10)
		
	def SetScrollBar(self, scrollBar):
		scrollBar.SetScrollEvent(ui.__mem_func__(self.__OnScroll))
		scrollBar.SetScrollStep(0.2)
		self.scrollBar = scrollBar

	def SetSelectEvent(self, event):
		self.selectEvent = event
		
	def __OnScroll(self):
		self.RefreshItemPosition(True)
			
	def GetTotalHeightItems(self):
		totalHeight = 0
		
		if self.itemList:
			for itemH in self.itemList:
				totalHeight += itemH.GetHeight() + 2
			
		return totalHeight

	def GetItemCount(self):
		return len(self.itemList)

	def AppendItem(self, index):
		item = self.NewItem(index)
		item.SetParent(self)
		
		if len(self.itemList) == 0:
			item.SetBasePosition(0, 0)
		else:
			x, y = self.itemList[-1].GetLocalPosition()
			y += 2
			item.SetBasePosition(0, y + self.itemList[-1].GetHeight())

		item.SetClickEvent(ui.__mem_func__(self.SelectItem))
		item.Show()
		self.itemList.append(item)

		self.ResetPosScrollBar()
		self.ResizeScrollBar()	
		self.RefreshItemPosition()

	def SelectItem(self, index):
		for item in self.itemList:
			if item.Index == index:
				continue
			
			item.bSelected = False
			item.OnMouseOverOutImage()
			
		if self.SelectIndexFunc:
			self.SelectIndexFunc(index)

	def SelectItemForce(self, index):
		for item in self.itemList:
			if item.Index == index:
				item.background.LoadImage("d:/ymir work/ui/game/ranking/left_side/button_category_selected.png")

				item.bSelected = True
				continue
			
			item.bSelected = False
			item.OnMouseOverOutImage()

	def ResizeScrollBar(self):
		totalHeight = float(self.GetTotalHeightItems())
		if totalHeight:
			scrollBarHeight = min(float(self.GetHeight() - 10) / totalHeight, 1.0)
		else:
			scrollBarHeight = 1.0
			
		self.scrollBar.SetMiddleBarSize(scrollBarHeight)
	
	def ResetPosScrollBar(self):
		self.scrollBar.SetPos(0)

	def RefreshItemPosition(self, bScroll = False):		
		scrollPos = self.scrollBar.GetPos()
		totalHeight = self.GetTotalHeightItems() - self.GetHeight()

		idx, CurIdx, yAccumulate = 0, 0, 0
		for item in self.itemList[idx:]:
			if bScroll:
				setPos = yAccumulate - int(scrollPos * totalHeight)
				item.SetPosition(0, setPos)
			else:
				item.SetPosition(0, yAccumulate)
				
			item.SetBasePosition(0, yAccumulate)

			CurIdx += 1
			yAccumulate += item.GetHeight() + 2

	def Clear(self):
		range = len(self.itemList)
		
		if range > 0:
			for item in self.itemList:
				item.OnResetThings()
				item.Hide()
				del item
		
		self.itemList = []

class RankInfo(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.dicInfoRank = {}

		for x in xrange(MAX_CATEGORY_NUM):
			self.dicInfoRank[x] = {}

		self.Size = {}
		self.bLoaded = False
		self.bCurPage = -1
		self.MyPos = 0
		self.LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.bTab = {}
		self.bInfoPos = {}
		self.bInfoName = {}
		self.bInfoValue = {}
		self.bInfoEmpire = {}
		self.bInfoLevel = {}
		self.bInfoGuildName = {}

		self.dicInfoRank = {}

		self.ListBoxItem.Clear()
	
	def Show(self):
		self.LoadWindow()
		self.SetCenterPosition()
		self.DoChange(0, True) # Select Automatic
		self.SetTop() # Set Top
		ui.ScriptWindow.Show(self)

	def LoadWindow(self):
		if self.bLoaded == True:
			return
			
		self.bLoaded = True
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/rank_info.py")
		except:
			import exception
			exception.Abort("rank_info.LoadWindow.LoadObject")

		try:
			self.titleBar = self.GetChild("TitleBar")
			self.board = self.GetChild("board")
		except:
			import exception
			exception.Abort("rank_info.__LoadWindow.BindObject")
		
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.ScrollBar = ui.NewScrollBar()
		self.ScrollBar.SetParent(self.board)
		self.ScrollBar.SetScrollBarSize(355)
		self.ScrollBar.SetPosition(162, 75)
		self.ScrollBar.Show()		

		self.ListBoxItem = ListBox()
		self.ListBoxItem.SetParent(self.board)
		self.ListBoxItem.SetScrollBar(self.ScrollBar)
		self.ListBoxItem.SetSize(170, 352)
		self.ListBoxItem.SetPosition(10, 75)
		self.ListBoxItem.SetEventSelect(ui.__mem_func__(self.DoChange))
		self.ListBoxItem.Show()		
		
		self.CreateButtonCategory()

		self.wndAnimMask = ui.Bar()
		self.wndAnimMask.SetParent(self)
		self.wndAnimMask.SetPosition(180, 90)
		self.wndAnimMask.SetSize(385, 310)
		self.wndAnimMask.SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 0.6))
		self.wndAnimMask.Hide()
		
		self.wndAnim = ui.AniImageBox()
		self.wndAnim.SetDelay(2)
		self.wndAnim.SetParent(self.wndAnimMask)
		self.wndAnim.SetEndFrameEvent(ui.__mem_func__(self.SearchRank))
		self.wndAnim.SetWindowHorizontalAlignCenter()	
		self.wndAnim.SetWindowVerticalAlignCenter()	
		self.wndAnim.Hide()

		for i in xrange(38):
			self.wndAnim.AppendImage("d:/ymir work/ui/anim_new/frame_delay_%d.png" % (i))

		self.bHeader = ui.ExpandedImageBox()
		self.bHeader.SetParent(self)
		self.bHeader.AddFlag("not_pick")
		self.bHeader.LoadImage("d:/ymir work/ui/game/ranking/banner.png")
		self.bHeader.SetPosition(8, 32)
		self.bHeader.Show()

		self.bInfoHeader = ui.ExpandedImageBox()
		self.bInfoHeader.SetParent(self)
		self.bInfoHeader.AddFlag("not_pick")
		self.bInfoHeader.LoadImage("d:/ymir work/ui/game/ranking/table/header.png")
		self.bInfoHeader.SetPosition(179, 78)
		self.bInfoHeader.Show()

		self.wndTextName = ui.TextLine()
		self.wndTextName.SetParent(self.bInfoHeader)
		self.wndTextName.SetPosition(-110, 4)
		self.wndTextName.SetWindowHorizontalAlignCenter()
		self.wndTextName.SetHorizontalAlignCenter()
		self.wndTextName.SetText(localeInfo.RANKING_TAB_NAME)
		self.wndTextName.Show()

		self.wndTextEmpire = ui.TextLine()
		self.wndTextEmpire.SetParent(self.bInfoHeader)
		self.wndTextEmpire.SetPosition(-38, 4)
		self.wndTextEmpire.SetWindowHorizontalAlignCenter()
		self.wndTextEmpire.SetHorizontalAlignCenter()
		self.wndTextEmpire.SetText(localeInfo.RANKING_TAB_EMPIRE)
		self.wndTextEmpire.Show()

		self.wndTextCount = ui.TextLine()
		self.wndTextCount.SetParent(self.bInfoHeader)
		self.wndTextCount.SetPosition(153, 4)
		self.wndTextCount.SetWindowHorizontalAlignCenter()
		self.wndTextCount.SetHorizontalAlignCenter()
		self.wndTextCount.SetText(localeInfo.RANKING_TAB_COUNT)
		self.wndTextCount.Show()

		self.wndTextLevel = ui.TextLine()
		self.wndTextLevel.SetParent(self.bInfoHeader)
		self.wndTextLevel.SetPosition(16, 4)
		self.wndTextLevel.SetWindowHorizontalAlignCenter()
		self.wndTextLevel.SetHorizontalAlignCenter()
		self.wndTextLevel.SetText(localeInfo.RANKING_TAB_LEVEL)
		self.wndTextLevel.Show()

		self.wndTextGuildName = ui.TextLine()
		self.wndTextGuildName.SetParent(self.bInfoHeader)
		self.wndTextGuildName.SetPosition(80, 4)
		self.wndTextGuildName.SetWindowHorizontalAlignCenter()
		self.wndTextGuildName.SetHorizontalAlignCenter()
		self.wndTextGuildName.SetText(localeInfo.RANKING_TAB_GUILDNAME)
		self.wndTextGuildName.Show()

		self.bInfoMyHeader = ui.ExpandedImageBox()
		self.bInfoMyHeader.SetParent(self)
		self.bInfoMyHeader.AddFlag("not_pick")
		self.bInfoMyHeader.LoadImage("d:/ymir work/ui/game/ranking/table/rank_normal.png")
		self.bInfoMyHeader.SetPosition(179, 406)
		self.bInfoMyHeader.Show()

		self.bInfoMyPos = ui.TextLine()
		self.bInfoMyPos.SetParent(self)
		self.bInfoMyPos.SetPosition(194, 410)

		self.bInfoMyName = ui.TextLine()
		self.bInfoMyName.SetParent(self)
		self.bInfoMyName.SetPosition(-30, 410)
		self.bInfoMyName.SetWindowHorizontalAlignCenter()
		self.bInfoMyName.SetHorizontalAlignCenter()

		self.bInfoMyEmpire = ui.ImageBox()
		self.bInfoMyEmpire.SetParent(self)
		self.bInfoMyEmpire.AddFlag("not_pick")
		self.bInfoMyEmpire.SetPosition(325, 412)

		self.bInfoMyLevel = ui.TextLine()
		self.bInfoMyLevel.SetParent(self)
		self.bInfoMyLevel.SetPosition(100, 410)
		self.bInfoMyLevel.SetWindowHorizontalAlignCenter()
		self.bInfoMyLevel.SetHorizontalAlignCenter()

		self.bInfoMyGuildName = ui.TextLine()
		self.bInfoMyGuildName.SetParent(self)
		self.bInfoMyGuildName.SetPosition(160, 410)
		self.bInfoMyGuildName.SetWindowHorizontalAlignCenter()
		self.bInfoMyGuildName.SetHorizontalAlignCenter()

		self.bInfoMyValue = ui.TextLine()
		self.bInfoMyValue.SetParent(self)
		self.bInfoMyValue.SetPosition(235, 410)
		self.bInfoMyValue.SetWindowHorizontalAlignCenter()
		self.bInfoMyValue.SetHorizontalAlignCenter()

		self.bInfoMyName.SetText("[SA]JIGSAW")
		self.bInfoMyValue.SetText("20.000")
		self.bInfoMyPos.SetText("1")
		self.bInfoMyLevel.SetText("0")
		self.bInfoMyGuildName.SetText("NoName")

		self.bInfoMyEmpire.LoadImage(EMPIRE_FLAGS[1])
		self.bInfoMyName.Show()
		self.bInfoMyPos.Show()
		self.bInfoMyEmpire.Show()
		self.bInfoMyValue.Show()
		self.bInfoMyLevel.Show()
		self.bInfoMyGuildName.Show()

	def OnRunMouseWheel(self, nLen):
		if nLen > 0:
			self.ScrollBar.OnUp()
		else:
			self.ScrollBar.OnDown()

	def CreateButtonCategory(self):
		for x in xrange(MAX_CATEGORY_NUM):
			self.ListBoxItem.AppendItem(x)

	def DoChange(self, arg, bSelectFromList = False):
		if self.bCurPage == arg:
			return
			
		self.bCurPage = arg
	
		self.bTab = {}
		self.bInfoPos = {}
		self.bInfoName = {}
		self.bInfoValue = {}
		self.bInfoEmpire = {}
		self.bInfoLevel = {}
		self.bInfoGuildName = {}

		self.wndAnim.SetPosition(-30, -45)
		self.wndAnim.ResetFrame()
		self.wndAnimMask.Show()
		self.wndAnim.Show()

		self.bInfoMyPos.Hide()
		self.bInfoMyName.Hide()
		self.bInfoMyEmpire.Hide()
		self.bInfoMyValue.Hide()
		self.bInfoMyLevel.Hide()
		self.bInfoMyGuildName.Hide()

		if bSelectFromList: # This it to force select a index, like reset "selected item"
			self.ListBoxItem.SelectItemForce(arg)

	def SearchRank(self):
		self.wndAnimMask.Hide()
		self.wndAnim.Hide()
		net.SendChatPacket("/req_info_rank %d 0" % (self.bCurPage))
		self.bCurPage = self.bCurPage

	def AppendInfo(self, mode, MyPos, pos, name, value, empire, level, guildname):
		if app.ENABLE_YOHARA_SYSTEM:
			vis_level = level

			if vis_level > 120:
				vis_level -= 120
				levelColor = 0xff7ad3ff
			else:
				levelColor = 0xffffff7a

		if MyPos != -1:
			self.bInfoMyPos.SetText(str(pos))
			self.bInfoMyPos.SetOutline()
			self.bInfoMyPos.Show()

			self.bInfoMyName.SetText(name)
			self.bInfoMyName.SetOutline()
			self.bInfoMyName.Show()

			self.bInfoMyEmpire.LoadImage(EMPIRE_FLAGS[empire - 1])
			self.bInfoMyEmpire.Show()

			if mode == 0:
				self.bInfoMyValue.SetText("-")
			else:
				self.bInfoMyValue.SetText(localeInfo.NumberToMoneyString2(int(value)))

			self.bInfoMyValue.SetOutline()
			self.bInfoMyValue.Show()

			if app.ENABLE_YOHARA_SYSTEM:
				self.bInfoMyLevel.SetPackedFontColor(levelColor)

			self.bInfoMyLevel.SetText(str(vis_level))

			self.bInfoMyLevel.SetOutline()
			self.bInfoMyLevel.Show()

			self.bInfoMyGuildName.SetText(guildname)
			self.bInfoMyGuildName.SetOutline()
			self.bInfoMyGuildName.Show()
		else:
			self.dicInfoRank[mode][pos] = {"Name": name, "Value": value, "Empire": empire, "Level" : level, "GuildName" : guildname}
			self.Refresh(pos)

	def Refresh(self, size):
		self.bTab = {}
		self.bInfoPlayerHeader = {}
		self.bInfoPos = {}
		self.bInfoName = {}
		self.bInfoValue = {}
		self.bInfoEmpire = {}
		self.bInfoLevel = {}
		self.bInfoGuildName = {}

		y_pos = [110, 113+ (26 *1), 116+ (26 *2), 119+ (26 *3), 122+ (26 *4), 125+ (26 *5), 128+ (26 *6), 131+ (26 *7), 134+ (26 *8), 137+ (26 *9)] # Position
		colors = ["|cffffff00", "|cff888888", "|cFFA52A2A" ]

		for x in xrange(1, size):
			if x > len(y_pos):
				break
		
			self.bInfoPlayerHeader[x] = ui.ExpandedImageBox()
			self.bInfoPlayerHeader[x].SetParent(self)
			self.bInfoPlayerHeader[x].AddFlag("not_pick")
			self.bInfoPlayerHeader[x].LoadImage("d:/ymir work/ui/game/ranking/table/rank_normal.png")
			self.bInfoPlayerHeader[x].SetPosition(179, y_pos[x-1])
			self.bInfoPlayerHeader[x].Show()

			self.bInfoPos[x] = ui.TextLine()
			self.bInfoPos[x].SetParent(self)
			self.bInfoPos[x].SetPosition(193, y_pos[x-1] + 5)
			self.bInfoPos[x].SetText(str(x))
			self.bInfoPos[x].SetOutline()
			self.bInfoPos[x].Show()

			self.bInfoName[x] = ui.TextLine()
			self.bInfoName[x].SetParent(self)
			self.bInfoName[x].SetPosition(-33, y_pos[x-1] + 5)
			self.bInfoName[x].SetWindowHorizontalAlignCenter()
			self.bInfoName[x].SetOutline()
			self.bInfoName[x].SetHorizontalAlignCenter()

			if x <= 3:
				self.bInfoName[x].SetText(colors[x-1] + self.dicInfoRank[self.bCurPage][x]["Name"])
			else:
				self.bInfoName[x].SetText(self.dicInfoRank[self.bCurPage][x]["Name"])

			self.bInfoName[x].Show()

			self.bInfoEmpire[x] = ui.ImageBox()
			self.bInfoEmpire[x].SetParent(self)
			self.bInfoEmpire[x].AddFlag("not_pick")
			self.bInfoEmpire[x].SetPosition(325, y_pos[x-1] + 6)
			self.bInfoEmpire[x].LoadImage(EMPIRE_FLAGS[self.dicInfoRank[self.bCurPage][x]["Empire"] - 1])
			self.bInfoEmpire[x].Show()

			self.bInfoValue[x] = ui.TextLine()
			self.bInfoValue[x].SetParent(self)
			self.bInfoValue[x].SetPosition(233, y_pos[x-1] + 5)
			self.bInfoValue[x].SetWindowHorizontalAlignCenter()	
			self.bInfoValue[x].SetHorizontalAlignCenter()

			if self.bCurPage == 0:
				if x <= 3:
					self.bInfoValue[x].SetText(colors[x-1] + "-")
				else:
					self.bInfoValue[x].SetText("-")
			else:
				price = localeInfo.NumberToMoneyString2(self.dicInfoRank[self.bCurPage][x]["Value"])
				price = self.dicInfoRank[self.bCurPage][x]["Value"]

				if x <= 3:
					self.bInfoValue[x].SetText(colors[x-1] + localeInfo.DottedNumber(str(price)))
				else:
					self.bInfoValue[x].SetText(localeInfo.DottedNumber(str(price)))

			self.bInfoValue[x].SetOutline()
			self.bInfoValue[x].Show()

			self.bInfoLevel[x] = ui.TextLine()
			self.bInfoLevel[x].SetParent(self)
			self.bInfoLevel[x].SetPosition(100, y_pos[x-1] + 5)
			self.bInfoLevel[x].SetWindowHorizontalAlignCenter()
			self.bInfoLevel[x].SetHorizontalAlignCenter()

			if app.ENABLE_YOHARA_SYSTEM:
				#if x <= 3:
				#	self.bInfoLevel[x].SetText(colors[x-1] + str(self.dicInfoRank[self.bCurPage][x]["Level"]))
				#else:
				level = self.dicInfoRank[self.bCurPage][x]["Level"]

				if level > 120:
					level -= 120
					levelColor = 0xff7ad3ff
				else:
					levelColor = 0xffffff7a

				self.bInfoLevel[x].SetPackedFontColor(levelColor)
				self.bInfoLevel[x].SetText(str(level))
			else:
				self.bInfoLevel[x].SetText(str(self.dicInfoRank[self.bCurPage][x]["Level"]))

			self.bInfoLevel[x].SetOutline()
			self.bInfoLevel[x].Show()

			self.bInfoGuildName[x] = ui.TextLine()
			self.bInfoGuildName[x].SetParent(self)
			self.bInfoGuildName[x].SetPosition(160, y_pos[x - 1] + 5)
			self.bInfoGuildName[x].SetWindowHorizontalAlignCenter()
			self.bInfoGuildName[x].SetHorizontalAlignCenter()

			#if x <= 3:
			#	self.bInfoGuildName[x].SetText(colors[x-1] + self.dicInfoRank[self.bCurPage][x]["GuildName"])
			#else:
			self.bInfoGuildName[x].SetText(self.dicInfoRank[self.bCurPage][x]["GuildName"])

			self.bInfoGuildName[x].SetOutline()
			self.bInfoGuildName[x].Show()

	def Close(self):
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True
