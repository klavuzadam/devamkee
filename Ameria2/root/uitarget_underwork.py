# coding: latin_1

import app
import ui
import player
import net
import wndMgr
import messenger
import guild
import chr
import nonplayer
import localeInfo
import constInfo
import uiToolTip
import item
import chat
import colorInfo

import background
import uiBlockChatTarget
import uiBanPanel
import mouseModule
import chrmgr

if app.ENABLE_VIEW_ELEMENT:
	ELEMENT_IMAGE_DIC = {1: "elect", 2: "fire", 3: "ice", 4: "wind", 5: "earth", 6 : "dark"}

if app.ENABLE_SEND_TARGET_INFO:
	def HAS_FLAG(value, flag):
		return (value & flag) == flag

	BOOK_CHESTS_VNUMLIST = [\
								39048,
								39050,
								39052,
								39054
	]

	BOOK_CHESTS_VNUMLAST = 0

	BOOK_CHESTS_SHOWONE = True

class TargetBoardGM(ui.ThinBoard):

	BUTTON_NAME_LIST = (
		"Kick",
		"ChatBlock",
		"Map1",
		"Kill",
		"Ban",
	)


	def __init__(self):
		ui.ThinBoard.__init__(self)

		self.buttonDict = {}
		self.showingButtonList = []

		for buttonName in self.BUTTON_NAME_LIST:
			button = ui.Button()
			button.SetParent(self)
			button.SetUpVisual("d:/ymir work/ui/public/small_thin_button_01.sub")
			button.SetOverVisual("d:/ymir work/ui/public/small_thin_button_02.sub")
			button.SetDownVisual("d:/ymir work/ui/public/small_thin_button_03.sub")
			button.SetText(buttonName)
			button.SetWindowHorizontalAlignCenter()
			button.Hide()

			self.buttonDict[buttonName] = button
			self.showingButtonList.append(button)

		self.buttonDict["Kick"].SetEvent(ui.__mem_func__(self.OnKick))
		self.buttonDict["ChatBlock"].SetEvent(ui.__mem_func__(self.OnChatBlock))
		self.buttonDict["Map1"].SetEvent(ui.__mem_func__(self.OnSendCity))
		self.buttonDict["Kill"].SetEvent(ui.__mem_func__(self.OnKill))
		self.buttonDict["Ban"].SetEvent(ui.__mem_func__(self.OnBan))

		self.isShowButton = False
		self.vid = 0
		
		dlgChatBlock = uiBlockChatTarget.ChatBlockDialog()
		dlgChatBlock.LoadDialog()
		dlgChatBlock.SetTitleName("ChatBlock")
		dlgChatBlock.Hide()
		self.dlgChatBlock = dlgChatBlock
				
		dlgBlockAccount = uiBanPanel.BanDialog()
		dlgBlockAccount.LoadDialog()
		dlgBlockAccount.SetTitleName("Ban")
		dlgBlockAccount.Hide()
		self.dlgBlockAccount = dlgBlockAccount
		
		self.__Initialize()
		self.ResetTargetBoard()
		
		self.HideCorners(ui.ThinBoard.LT)
		self.HideCorners(ui.ThinBoard.RT)
		self.HideLine(ui.ThinBoard.T)

	def __del__(self):
		ui.ThinBoard.__del__(self)

	def __Initialize(self):
		self.vid = 0
		self.isShowButton = False

	def Destroy(self):
		for btn in self.showingButtonList:
			btn.SetEvent(None)
			del btn

		self.showingButtonList = []

		self.dlgChatBlock.Destroy()
		self.dlgChatBlock = 0
		self.dlgBlockAccount.Destroy()
		self.dlgBlockAccount = 0

		self.__Initialize()
	
	def Close(self):
		self.__Initialize()
		self.Hide()

	def Open(self, vid):
		if vid and not player.IsMainCharacterIndex(vid):
			self.SetTargetVID(vid)
			self.RefreshButton()
			self.Show()
		else:
			self.HideAllButton()
			self.Hide()
		
	def RefreshButton(self):
		self.HideAllButton()
		self.ShowDefaultButton()
		self.__ArrangeButtonPosition()
		
	def __ArrangeButtonPosition(self):
		showingButtonCount = len(self.showingButtonList)
		pos = -(showingButtonCount / 2) * 68
		if 0 == showingButtonCount % 2:
			pos += 34

		for button in self.showingButtonList:
			button.SetPosition(pos, 20)
			pos += 68

		self.SetSize(showingButtonCount * 75, 50)
		self.SetPosition(wndMgr.GetScreenWidth()/2 + 30 - self.GetWidth()/2, 120)
		
	def SetTargetVID(self, vid):
		self.vid = vid
		
	def ResetTargetBoard(self):
		for btn in self.buttonDict.values():
			btn.Hide()

		self.__Initialize()

	def ShowDefaultButton(self):
		self.isShowButton = True

		self.showingButtonList.append(self.buttonDict["Kick"])
		self.showingButtonList.append(self.buttonDict["ChatBlock"])
		self.showingButtonList.append(self.buttonDict["Map1"])
		self.showingButtonList.append(self.buttonDict["Kill"])
		self.showingButtonList.append(self.buttonDict["Ban"])

		for button in self.showingButtonList:
			button.Show()

	def HideAllButton(self):
		self.isShowButton = False

		for button in self.showingButtonList:
			button.Hide()

		self.showingButtonList = []

	def IsShowButton(self):
		return self.isShowButton

	def OnKick(self):
		net.SendChatPacket("/dc " + str(chr.GetNameByVID(self.vid)))
		
	def OnKill(self):
		net.SendChatPacket("/kill " + str(chr.GetNameByVID(self.vid)))

	def OnChatBlock(self):
		self.dlgChatBlock.SetTitleName("ChatBlock: " + str(chr.GetNameByVID(self.vid)))
		self.dlgChatBlock.Open(str(chr.GetNameByVID(self.vid)))
		
	def OnBan(self):
		self.dlgBlockAccount.SetTitleName("Ban player: " + str(chr.GetNameByVID(self.vid)))
		self.dlgBlockAccount.Open(str(chr.GetNameByVID(self.vid)))
		
	def OnSendCity(self):
		net.SendChatPacket("/warp_to_town " + str(chr.GetNameByVID(self.vid)))

CLASS_THINBOARD = ui.ThinBoard

if app.ENABLE_NEW_TARGET_UI:
	CLASS_THINBOARD = ui.ThinBoardTarget

class TargetBoard(CLASS_THINBOARD):
	if app.ENABLE_SEND_TARGET_INFO:
		class InfoBoard(ui.ThinBoard):
			class ItemListBoxItem(ui.ListBoxExNew.Item):
				def __init__(self, width):
					ui.ListBoxExNew.Item.__init__(self)

					image = ui.ExpandedImageBox()
					image.SetParent(self)
					image.Show()
					self.image = image

					nameLine = ui.TextLine()
					nameLine.SetParent(self)
					nameLine.SetPosition(32 + 5, 0)
					nameLine.Show()
					self.nameLine = nameLine

					self.SetSize(width, 32 + 5)

				def LoadImage(self, image, name = None):
					self.image.LoadImage(image)
					self.SetSize(self.GetWidth(), self.image.GetHeight() + 5 * (self.image.GetHeight() / 32))
					if name != None:
						self.SetText(name)

				def SetText(self, text):
					self.nameLine.SetText(text)

				def RefreshHeight(self):
					ui.ListBoxExNew.Item.RefreshHeight(self)
					self.image.SetRenderingRect(0.0, 0.0 - float(self.removeTop) / float(self.GetHeight()), 0.0, 0.0 - float(self.removeBottom) / float(self.GetHeight()))
					self.image.SetPosition(0, - self.removeTop)

			MAX_ITEM_COUNT = 5

			EXP_BASE_LVDELTA = [
				1,  #  -15 0
				5,  #  -14 1
				10, #  -13 2
				20, #  -12 3
				30, #  -11 4
				50, #  -10 5
				70, #  -9  6
				80, #  -8  7
				85, #  -7  8
				90, #  -6  9
				92, #  -5  10
				94, #  -4  11
				96, #  -3  12
				98, #  -2  13
				100,	#  -1  14
				100,	#  0   15
				105,	#  1   16
				110,	#  2   17
				115,	#  3   18
				120,	#  4   19
				125,	#  5   20
				130,	#  6   21
				135,	#  7   22
				140,	#  8   23
				145,	#  9   24
				150,	#  10  25
				155,	#  11  26
				160,	#  12  27
				165,	#  13  28
				170,	#  14  29
				180,	#  15  30
			]

			RACE_FLAG_TO_NAME = {
				1 << 0 : localeInfo.TARGET_INFO_RACE_ANIMAL,
				1 << 1 : localeInfo.TARGET_INFO_RACE_UNDEAD,
				1 << 2 : localeInfo.TARGET_INFO_RACE_DEVIL,
				1 << 3 : localeInfo.TARGET_INFO_RACE_HUMAN,
				1 << 4 : localeInfo.TARGET_INFO_RACE_ORC,
				1 << 5 : localeInfo.TARGET_INFO_RACE_MILGYO,
				# 1 << 6 : localeInfo.TARGET_INFO_NEWRACE_INSECT,
				# 1 << 7 : localeInfo.TARGET_INFO_NEWRACE_FIRE,
				# 1 << 8 : localeInfo.TARGET_INFO_NEWRACE_ICE,
				# 1 << 9 : localeInfo.TARGET_INFO_NEWRACE_DESERT,
				# 1 << 10 : localeInfo.TARGET_INFO_NEWRACE_TREE,
			}

			SUB_RACE_FLAG_TO_NAME = {
				1 << 11 : localeInfo.TARGET_INFO_RACE_ELEC,
				1 << 12 : localeInfo.TARGET_INFO_RACE_FIRE,
				1 << 13 : localeInfo.TARGET_INFO_RACE_ICE,
				1 << 14 : localeInfo.TARGET_INFO_RACE_WIND,
				1 << 15 : localeInfo.TARGET_INFO_RACE_EARTH,
				1 << 16 : localeInfo.TARGET_INFO_RACE_DARK,
			}

			STONE_START_VNUM = 28030
			STONE_LAST_VNUM = 28045

			BOARD_WIDTH = 250

			def __init__(self):
				ui.ThinBoard.__init__(self)

				self.HideCorners(self.LT)
				self.HideCorners(self.RT)
				self.HideLine(self.T)

				self.race = 0
				self.hasItems = False

				self.itemTooltip = uiToolTip.ItemToolTip()
				self.itemTooltip.HideToolTip()

				self.stoneImg = None
				self.stoneVnum = None
				self.lastStoneVnum = 0

				self.chestBookList = []

				self.nextStoneIconChange = 0
				self.children = []

				self.scrollBar = None

				self.SetSize(self.BOARD_WIDTH, 0)

			def __del__(self):
				ui.ThinBoard.__del__(self)

			def __UpdatePosition(self, targetBoard):
				self.SetPosition(targetBoard.GetLeft() + (targetBoard.GetWidth() - self.GetWidth()) / 2, targetBoard.GetBottom() - 17)

			def Open(self, targetBoard, race):
				self.__LoadInformation(race)

				self.SetSize(self.BOARD_WIDTH, self.yPos + 10)
				self.__UpdatePosition(targetBoard)

				self.Show()

			def Refresh(self):
				self.__LoadInformation(self.race)
				self.SetSize(self.BOARD_WIDTH, self.yPos + 10)

			def Close(self):
				self.itemTooltip.HideToolTip()
				self.Hide()

			def __LoadInformation(self, race):
				self.yPos = 7

				for child in self.children:
					if isinstance(child, ui.ListBoxExNew):
						if child.scrollBar:
							child.scrollBar.Hide()

					child.Hide()
					child.Destroy()

				self.children = []
				self.race = race
				self.stoneImg = None
				self.stoneVnum = None

				self.chestBookList = []

				self.__LoadInformation_Default(race)
				self.__LoadInformation_Race(race)
				self.__LoadInformation_Resists(race)
				self.__LoadInformation_Drops(race)

			def __LoadInformation_Default_GetHitRate(self, race):
				attacker_dx = nonplayer.GetMonsterDX(race)
				attacker_level = nonplayer.GetMonsterLevel(race)

				self_dx = player.GetStatus(player.DX)
				self_level = player.GetStatus(player.LEVEL)

				iARSrc = min(90, (attacker_dx * 4 + attacker_level * 2) / 6)
				iERSrc = min(90, (self_dx * 4 + self_level * 2) / 6)

				fAR = (float(iARSrc) + 210.0) / 300.0
				fER = (float(iERSrc) * 2 + 5) / (float(iERSrc) + 95) * 3.0 / 10.0

				return fAR - fER
				
			def __LoadInformation_Resists(self, race):
				self.AppendSeperator()
				self.AppendTextLine(localeInfo.TARGET_INFO_RESISTS)
				self.AppendTextLine(localeInfo.TARGET_INFO_RESISTS_LINE0 % (nonplayer.GetResist(race, nonplayer.MOB_RESIST_SWORD), nonplayer.GetResist(race, nonplayer.MOB_RESIST_TWOHAND), nonplayer.GetResist(race, nonplayer.MOB_RESIST_BELL)))
				self.AppendTextLine(localeInfo.TARGET_INFO_RESISTS_LINE1 % (nonplayer.GetResist(race, nonplayer.MOB_RESIST_DAGGER), nonplayer.GetResist(race, nonplayer.MOB_RESIST_FAN), nonplayer.GetResist(race, nonplayer.MOB_RESIST_BOW)))

			def __LoadInformation_Default(self, race):
				self.AppendSeperator()
				self.AppendTextLine(localeInfo.TARGET_INFO_MAX_HP % localeInfo.DottedNumber(nonplayer.GetMonsterMaxHP(race)))

				# calc att damage
				monsterLevel = nonplayer.GetMonsterLevel(race)
				fHitRate = self.__LoadInformation_Default_GetHitRate(race)
				iDamMin, iDamMax = nonplayer.GetMonsterDamage(race)
				iDamMin = int((iDamMin + nonplayer.GetMonsterST(race)) * 2 * fHitRate) + monsterLevel * 2
				iDamMax = int((iDamMax + nonplayer.GetMonsterST(race)) * 2 * fHitRate) + monsterLevel * 2
				iDef = player.GetStatus(player.DEF_GRADE) * (100 + player.GetStatus(player.DEF_BONUS)) / 100
				fDamMulti = nonplayer.GetMonsterDamageMultiply(race)
				iDamMin = int(max(0, iDamMin - iDef) * fDamMulti)
				iDamMax = int(max(0, iDamMax - iDef) * fDamMulti)
				if iDamMin < 1:
					iDamMin = 1
				if iDamMax < 5:
					iDamMax = 5
				self.AppendTextLine(localeInfo.TARGET_INFO_DAMAGE % (str(iDamMin), str(iDamMax)))

				idx = min(len(self.EXP_BASE_LVDELTA) - 1, max(0, (monsterLevel + 15) - player.GetStatus(player.LEVEL)))
				iExp = nonplayer.GetMonsterExp(race) * self.EXP_BASE_LVDELTA[idx] / 100
				self.AppendTextLine(localeInfo.TARGET_INFO_EXP % localeInfo.DottedNumber(iExp))
				
				self.AppendTextLine(localeInfo.TARGET_INFO_GOLD_MIN_MAX % (localeInfo.DottedNumber(nonplayer.GetMobGoldMin(race)), localeInfo.DottedNumber(nonplayer.GetMobGoldMax(race))))
				self.AppendTextLine(localeInfo.TARGET_INFO_REGEN_INFO % (nonplayer.GetMobRegenPercent(race), nonplayer.GetMobRegenCycle(race)))

			def __LoadInformation_Race(self, race):
				dwRaceFlag = nonplayer.GetMonsterRaceFlag(race)
				self.AppendSeperator()

				mainrace = ""
				subrace = ""
				for i in xrange(17):
					curFlag = 1 << i
					if HAS_FLAG(dwRaceFlag, curFlag):
						if self.RACE_FLAG_TO_NAME.has_key(curFlag):
							mainrace += self.RACE_FLAG_TO_NAME[curFlag] + ", "
						elif self.SUB_RACE_FLAG_TO_NAME.has_key(curFlag):
							subrace += self.SUB_RACE_FLAG_TO_NAME[curFlag] + ", "
				if nonplayer.IsMonsterStone(race):
					mainrace += localeInfo.TARGET_INFO_RACE_METIN + ", "
				if mainrace == "":
					mainrace = localeInfo.TARGET_INFO_NO_RACE
				else:
					mainrace = mainrace[:-2]
				if subrace == "":
					subrace = localeInfo.TARGET_INFO_NO_RACE
				else:
					subrace = subrace[:-2]

				self.AppendTextLine(localeInfo.TARGET_INFO_MAINRACE % mainrace)
				self.AppendTextLine(localeInfo.TARGET_INFO_SUBRACE % subrace)

			def __LoadInformation_Drops(self, race):
				self.AppendSeperator()

				if race in constInfo.MONSTER_INFO_DATA:
					if len(constInfo.MONSTER_INFO_DATA[race]["items"]) == 0:
						self.AppendTextLine(localeInfo.TARGET_INFO_NO_ITEM_TEXT)
					else:
						itemListBox = ui.ListBoxExNew(32 + 5, self.MAX_ITEM_COUNT)
						itemListBox.SetSize(self.GetWidth() - 15 * 2 - ui.ScrollBar.SCROLLBAR_WIDTH, (32 + 5) * self.MAX_ITEM_COUNT)
						height = 0

						j = 0
						for curItem in constInfo.MONSTER_INFO_DATA[race]["items"]:
							if curItem.has_key("vnum_list"):
								height += self.AppendItem(itemListBox, curItem["vnum_list"], curItem["count"])
							else:
								if curItem["vnum"] in BOOK_CHESTS_VNUMLIST and BOOK_CHESTS_SHOWONE and len(self.chestBookList) == 1:
									j += 1
									continue

								height += self.AppendItem(itemListBox, curItem["vnum"], curItem["count"])

						if height < itemListBox.GetHeight():
							itemListBox.SetSize(itemListBox.GetWidth(), height)

						self.AppendWindow(itemListBox, 15)
						itemListBox.SetBasePos(0)

						realCount = len(constInfo.MONSTER_INFO_DATA[race]["items"]) - j

						if realCount > itemListBox.GetViewItemCount():
							itemScrollBar = ui.ScrollBar()
							itemScrollBar.SetParent(self)
							itemScrollBar.SetPosition(itemListBox.GetRight(), itemListBox.GetTop())
							itemScrollBar.SetScrollBarSize(32 * self.MAX_ITEM_COUNT + 5 * (self.MAX_ITEM_COUNT - 1))
							itemScrollBar.SetMiddleBarSize(float(self.MAX_ITEM_COUNT) / float(realCount))
#							itemScrollBar.SetScrollStep(float(self.MAX_ITEM_COUNT) / float(realCount))
							itemScrollBar.Show()
							itemListBox.SetScrollBar(itemScrollBar)

							self.scrollBar = itemScrollBar
				else:
					self.AppendTextLine(localeInfo.TARGET_INFO_NO_ITEM_TEXT)

			def OnRunMouseWheel(self, nLen):
				if self.scrollBar:
					if nLen > 0:
						self.scrollBar.OnUp()
					else:
						self.scrollBar.OnDown()

			def AppendTextLine(self, text):
				textLine = ui.TextLine()
				textLine.SetParent(self)
				textLine.SetWindowHorizontalAlignCenter()
				textLine.SetHorizontalAlignCenter()
				textLine.SetText(text)
				textLine.SetPosition(0, self.yPos)
				textLine.Show()

				self.children.append(textLine)
				self.yPos += 17

			def AppendSeperator(self):
				img = ui.ImageBox()
				img.LoadImage("d:/ymir work/ui/seperator.tga")
				self.AppendWindow(img)
				img.SetPosition(img.GetLeft(), img.GetTop() - 15)
				self.yPos -= 15

			def AppendItem(self, listBox, vnums, count):
				if type(vnums) == int:
					vnum = vnums
				else:
					vnum = vnums[0]

				item.SelectItem(vnum)
				itemName = item.GetItemName()

				if type(vnums) != int and len(vnums) > 1:
					vnums = sorted(vnums)
					realName = itemName[:itemName.find("+")]
					if item.GetItemType() == item.ITEM_TYPE_METIN:
						realName = localeInfo.TARGET_INFO_STONE_NAME
						itemName = realName + "+0 - +4"
					else:
						itemName = realName + "+" + str(vnums[0] % 10) + " - +" + str(vnums[len(vnums) - 1] % 10)
					vnum = vnums[len(vnums) - 1]

				myItem = self.ItemListBoxItem(listBox.GetWidth())
				myItem.LoadImage(item.GetIconImageFileName())
				if count <= 1:
					myItem.SetText(itemName)
				else:
					myItem.SetText("%dx %s" % (count, itemName))
				myItem.SAFE_SetOverInEvent(self.OnShowItemTooltip, vnum)
				myItem.SAFE_SetOverOutEvent(self.OnHideItemTooltip)
				listBox.AppendItem(myItem)

				if item.GetItemType() == item.ITEM_TYPE_METIN:
					self.stoneImg = myItem
					self.stoneVnum = vnums
					self.lastStoneVnum = self.STONE_LAST_VNUM + vnums[len(vnums) - 1] % 1000 / 100 * 100

				if vnum in BOOK_CHESTS_VNUMLIST:
					global BOOK_CHESTS_VNUMLAST
					self.chestBookList.append([BOOK_CHESTS_VNUMLIST[0], BOOK_CHESTS_VNUMLAST, 0, myItem])

					if BOOK_CHESTS_VNUMLAST == 3:
						BOOK_CHESTS_VNUMLAST = 0
					else:
						BOOK_CHESTS_VNUMLAST += 1

				return myItem.GetHeight()

			def OnShowItemTooltip(self, vnum):
				if not vnum:
					return

				if vnum in BOOK_CHESTS_VNUMLIST:
					for book in self.chestBookList:
						if book[3].IsIn():
							self.itemTooltip.SetItemToolTip(BOOK_CHESTS_VNUMLIST[book[1]])
							break

					self.itemTooltip.isStone = False
					self.itemTooltip.isBook = False
					self.itemTooltip.isBook2 = False
					self.itemTooltip.ischestBookList = True
				else:
					item.SelectItem(vnum)

					if item.GetItemType() == item.ITEM_TYPE_METIN:
						self.itemTooltip.isStone = True
						self.itemTooltip.isBook = False
						self.itemTooltip.isBook2 = False
						self.itemTooltip.ischestBookList = False
						self.itemTooltip.SetItemToolTip(self.lastStoneVnum)
					else:
						self.itemTooltip.isStone = False
						self.itemTooltip.isBook = True
						self.itemTooltip.isBook2 = True
						self.itemTooltip.ischestBookList = False
						self.itemTooltip.SetItemToolTip(vnum)

			def OnHideItemTooltip(self):
				self.itemTooltip.HideToolTip()

			def AppendWindow(self, wnd, x = 0, width = 0, height = 0):
				if width == 0:
					width = wnd.GetWidth()
				if height == 0:
					height = wnd.GetHeight()

				wnd.SetParent(self)
				if x == 0:
					wnd.SetPosition((self.GetWidth() - width) / 2, self.yPos)
				else:
					wnd.SetPosition(x, self.yPos)
				wnd.Show()

				self.children.append(wnd)
				self.yPos += height + 5

			def OnUpdate(self):
				if self.stoneImg != None and self.stoneVnum != None and app.GetTime() >= self.nextStoneIconChange:
					nextImg = self.lastStoneVnum + 1
					if nextImg % 100 > self.STONE_LAST_VNUM % 100:
						nextImg -= (self.STONE_LAST_VNUM - self.STONE_START_VNUM) + 1
					self.lastStoneVnum = nextImg
					self.nextStoneIconChange = app.GetTime() + 2.5

					item.SelectItem(nextImg)
					itemName = item.GetItemName()
					realName = itemName[:itemName.find("+")]
					realName = realName + " +0 - +4"
					self.stoneImg.LoadImage(item.GetIconImageFileName(), realName)

					if self.itemTooltip.IsShow() and self.itemTooltip.isStone:
						self.itemTooltip.SetItemToolTip(nextImg)

				j = 0

				for book in self.chestBookList:
					j += 1
					if app.GetTime() >= book[2]:
						if book[1] == 3:
							book[1] = 0
						else:
							book[1] += 1

						book[2] = app.GetTime() + 2

						item.SelectItem(BOOK_CHESTS_VNUMLIST[book[1]])

						book[3].LoadImage(item.GetIconImageFileName(), item.GetItemName())

						if self.itemTooltip.IsShow() and book[3].IsIn():
							self.itemTooltip.SetItemToolTip(BOOK_CHESTS_VNUMLIST[book[1]])

						break

	if app.ENABLE_NEW_TARGET_UI:
		class TextToolTip(ui.Window):
			def __init__(self):
				ui.Window.__init__(self, "TOP_MOST")

				textLine = ui.TextLine()
				textLine.SetParent(self)
				textLine.SetHorizontalAlignCenter()
				textLine.SetOutline()
				textLine.Show()
				self.textLine = textLine

			def __del__(self):
				ui.Window.__del__(self)

			def SetText(self, text):
				self.textLine.SetText(text)

			def OnRender(self):
				(mouseX, mouseY) = wndMgr.GetMousePosition()
				self.textLine.SetPosition(mouseX, mouseY - 15)

	BUTTON_NAME_LIST = (
		localeInfo.TARGET_BUTTON_WHISPER,
		localeInfo.TARGET_BUTTON_EXCHANGE,
		localeInfo.TARGET_BUTTON_FIGHT,
		localeInfo.TARGET_BUTTON_ACCEPT_FIGHT,
		localeInfo.TARGET_BUTTON_AVENGE,
		localeInfo.TARGET_BUTTON_FRIEND,
		localeInfo.TARGET_BUTTON_INVITE_PARTY,
		localeInfo.TARGET_BUTTON_LEAVE_PARTY,
		localeInfo.TARGET_BUTTON_EXCLUDE,
		localeInfo.TARGET_BUTTON_INVITE_GUILD,
		localeInfo.TARGET_BUTTON_DISMOUNT,
		localeInfo.TARGET_BUTTON_EXIT_OBSERVER,
		localeInfo.TARGET_BUTTON_VIEW_EQUIPMENT,
		localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY,
		localeInfo.TARGET_BUTTON_BUILDING_DESTROY,
		localeInfo.TARGET_BUTTON_EMOTION_ALLOW,
		"VOTE_BLOCK_CHAT",
	)

	BUTTON_NAME_LANG = {
		"en" : {
			0: "Whispering",1: "Trading",2: "Duel",3: "Agree",4: "Revenge",5: "Friend",6: "Group Invitation",7: "Leave Group",8: "Exclude",9: "Guild Invitation",10: "Dismount",11: "End Observer Mode",12: "Equipment",13: "Join Group",14: "Destroy",15: "Emotions",16: "VOTE_BLOCK_CHAT",
		},
		"es" : {
			0: "Conversacion",1: "Comercio",2: "Duelo",3: "Aceptar",4: "Vengar",5: "Amigo",6: "Invitar al Grupo",7: "Salir del Grupo",8: "Excluir",9: "Invitar al Gremio",10: "Desmontar",11: "Finalizar modo observador",12: "Equipamiento",13: "Entrar en grupo",14: "Destruir",15: "Emociones",16: "VOTE_BLOCK_CHAT",
		},
		"pt" : {
			0: "Whispering",1: "Trading",2: "Duel",3: "Agree",4: "Revenge",5: "Friend",6: "Group Invitation",7: "Leave Group",8: "Exclude",9: "Guild Invitation",10: "Dismount",11: "End Observer Mode",12: "Equipment",13: "Join Group",14: "Destroy",15: "Emotions",16: "VOTE_BLOCK_CHAT",
		},
		"ro" : {
			0: "Soapta",1: "Negociaza",2: "Duel",3: "Aproba",4: "Razbunare",5: "Prieten",6: "Invitatia grupei",7: "Paraseste grupa",8: "Exclude",9: "Invitatia breslei",10: "Descaleca",11: "End Observer Mode",12: "Equipment",13: "Alatura-te grupei",14: "Distruge",15: "Emotii",16: "VOTE_BLOCK_CHAT",
		},
	}

	BUTTON_NAME_ID = {
		"localeInfo.TARGET_BUTTON_WHISPER":0,
		"localeInfo.TARGET_BUTTON_EXCHANGE":1,
		"localeInfo.TARGET_BUTTON_FIGHT":2,
		"localeInfo.TARGET_BUTTON_ACCEPT_FIGHT":3,
		"localeInfo.TARGET_BUTTON_AVENGE":4,
		"localeInfo.TARGET_BUTTON_FRIEND":5,
		"localeInfo.TARGET_BUTTON_INVITE_PARTY":6,
		"localeInfo.TARGET_BUTTON_LEAVE_PARTY":7,
		"localeInfo.TARGET_BUTTON_EXCLUDE":8,
		"localeInfo.TARGET_BUTTON_INVITE_GUILD":9,
		"localeInfo.TARGET_BUTTON_DISMOUNT":10,
		"localeInfo.TARGET_BUTTON_EXIT_OBSERVER":11,
		"localeInfo.TARGET_BUTTON_VIEW_EQUIPMENT":12,
		"localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY":13,
		"localeInfo.TARGET_BUTTON_BUILDING_DESTROY":14,
		"localeInfo.TARGET_BUTTON_EMOTION_ALLOW":15,
		"VOTE_BLOCK_CHAT":16,
	}

	GRADE_NAME =	{
						nonplayer.PAWN : localeInfo.TARGET_LEVEL_PAWN,
						nonplayer.S_PAWN : localeInfo.TARGET_LEVEL_S_PAWN,
						nonplayer.KNIGHT : localeInfo.TARGET_LEVEL_KNIGHT,
						nonplayer.S_KNIGHT : localeInfo.TARGET_LEVEL_S_KNIGHT,
						nonplayer.BOSS : localeInfo.TARGET_LEVEL_BOSS,
						nonplayer.KING : localeInfo.TARGET_LEVEL_KING,
					}
	EXCHANGE_LIMIT_RANGE = 3000

	def __init__(self):
		CLASS_THINBOARD.__init__(self)
		self.canOpenGM = False

		name = ui.TextLine()
		name.SetParent(self)
		name.SetDefaultFontName()
		name.SetOutline()
		name.Show()

		nameBoard = ui.ExpandedImageBox()
		nameBoard.SetParent(self)
		nameBoard.LoadImage("d:/ymir work/ui/game/target/target_hp_bar_background.png")
		nameBoard.SetScale(2.2, 1.0)
		nameBoard.SetPosition(-2, -20)
		nameBoard.Show()
		self.nameBoard = nameBoard

		hpboard = ui.ExpandedImageBox()
		hpboard.SetParent(self)
		hpboard.LoadImage("d:/ymir work/ui/game/target/target_hp_bar_background2.png")
		hpboard.SetScale(1.3, 1.0)
		hpboard.SetPosition(30, 43)
		hpboard.Show()
		self.hpboard = hpboard

		if app.__AUTO_QUQUE_ATTACK__:
			autoFarmText = ui.TextLine()
			autoFarmText.SetParent(self)
			autoFarmText.SetOutline()
			# autoFarmText.SetHorizontalAlignCenter()
			#autoFarmText.SetText("|Eemoji/key_shift|e + |Eemoji/key_rclick|e Target Metin Queue")
			autoFarmText.Hide()
			self.autoFarmText = autoFarmText

			autoFarmBtn = ui.Button()
			autoFarmBtn.SetParent(self)
			autoFarmBtn.SetUpVisual("d:/ymir work/ui/pattern/q_mark_01.tga")
			autoFarmBtn.SetOverVisual("d:/ymir work/ui/pattern/q_mark_02.tga")
			autoFarmBtn.SetDownVisual("d:/ymir work/ui/pattern/q_mark_01.tga")
			autoFarmBtn.SetEvent(ui.__mem_func__(self.__AutoFarmInfo))
			autoFarmBtn.Hide()
			self.autoFarmBtn = autoFarmBtn

			self.autoFarmInfo = None

		if app.ENABLE_ACCUMULATE_DAMAGE_DISPLAY:
			nameText = ui.TextLine()
			nameText.SetParent(self.nameBoard)
			nameText.SetPosition(32, 3)
			nameText.SetDefaultFontName()
			nameText.SetOutline()
			self.nameText = nameText
			nameText.Show()

		if app.ENABLE_VIEW_TARGET_DECIMAL_HP:#hp
			hpDecimal = ui.TextLine()
			hpDecimal.SetParent(self.hpboard)
			hpDecimal.SetDefaultFontName()
			hpDecimal.SetPosition(15, 3)
			hpDecimal.SetOutline()
			hpDecimal.Hide()
		
		closeButton = ui.Button()
		closeButton.SetParent(self)
		closeButton.SetUpVisual("d:/ymir work/ui/public/close_button_01.sub")
		closeButton.SetOverVisual("d:/ymir work/ui/public/close_button_02.sub")
		closeButton.SetDownVisual("d:/ymir work/ui/public/close_button_03.sub")
		closeButton.SetPosition(30, 13)

		if localeInfo.IsARABIC():
			closeButton.SetWindowHorizontalAlignLeft()
		else:
			closeButton.SetWindowHorizontalAlignRight()

		hpText = ui.TextLine()
		hpText.SetParent(hpDecimal)
		hpText.SetHorizontalAlignCenter()
		hpText.SetWindowHorizontalAlignCenter()
		hpText.SetPosition(80, 0)
		hpText.SetOutline()
		hpText.Hide()

		hpDecimal.hpText = hpText

		self.TargetBoardGM = TargetBoardGM()
		self.TargetBoardGM.Hide()

		if app.ENABLE_SEND_TARGET_INFO:
			infoButton = ui.Button()
			infoButton.SetParent(self)
			infoButton.SetUpVisual("flag/q_mark_01.tga")
			infoButton.SetOverVisual("flag/q_mark_01.tga")
			infoButton.SetDownVisual("flag/q_mark_02.tga")
			infoButton.SetEvent(ui.__mem_func__(self.OnPressedInfoButton))
			infoButton.Hide()
			infoBoard = self.InfoBoard()
			infoBoard.Hide()
			infoButton.showWnd = infoBoard
		closeButton.SetEvent(ui.__mem_func__(self.OnPressedCloseButton))
		closeButton.Show()

		if app.ENABLE_HIDE_COSTUME_SYSTEM:
			import uiToolTip
			self.toolTip = uiToolTip.ToolTip(160)
			self.toolTip.HideToolTip()
			
			HideCostume = ui.ImageBox()
			HideCostume.SetParent(self)
			HideCostume.SetPosition(225, 14)
			HideCostume.SetWindowHorizontalAlignRight()
			HideCostume.LoadImage("Modulo/HideCostume/button_show_0.tga")
			HideCostume.OnMouseOverIn = ui.__mem_func__(self.OnMouseOverInHideCostume)
			HideCostume.OnMouseOverOut = ui.__mem_func__(self.OnMouseOverOutHideCostume)
			HideCostume.Hide()

		self.buttonDict = {}
		self.showingButtonList = []

		a = 0
		for buttonName in self.BUTTON_NAME_LIST:
			button = ui.Button()
			button.SetParent(self)

			if localeInfo.IsARABIC():
				button.SetUpVisual("d:/ymir work/ui/public/Small_Button_01.sub")
				button.SetOverVisual("d:/ymir work/ui/public/Small_Button_02.sub")
				button.SetDownVisual("d:/ymir work/ui/public/Small_Button_03.sub")
			else:
				button.SetUpVisual("d:/ymir work/ui/public/small_thin_button_01.sub")
				button.SetOverVisual("d:/ymir work/ui/public/small_thin_button_02.sub")
				button.SetDownVisual("d:/ymir work/ui/public/small_thin_button_03.sub")

			button.SetWindowHorizontalAlignCenter()
			try:
				button.SetText(self.BUTTON_NAME_LANG[app.GetLocaleName()][a])
			except:
				button.SetText(self.BUTTON_NAME_LANG["en"][a])
			button.Hide()
			self.buttonDict[a] = button
			self.showingButtonList.append(button)
			a+=1

		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_WHISPER"]].SetEvent(ui.__mem_func__(self.OnWhisper))
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_EXCHANGE"]].SetEvent(ui.__mem_func__(self.OnExchange))
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FIGHT"]].SetEvent(ui.__mem_func__(self.OnPVP))
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_ACCEPT_FIGHT"]].SetEvent(ui.__mem_func__(self.OnPVP))
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_AVENGE"]].SetEvent(ui.__mem_func__(self.OnPVP))
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FRIEND"]].SetEvent(ui.__mem_func__(self.OnAppendToMessenger))
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FRIEND"]].SetEvent(ui.__mem_func__(self.OnAppendToMessenger))
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_INVITE_PARTY"]].SetEvent(ui.__mem_func__(self.OnPartyInvite))
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_LEAVE_PARTY"]].SetEvent(ui.__mem_func__(self.OnPartyExit))
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_EXCLUDE"]].SetEvent(ui.__mem_func__(self.OnPartyRemove))

		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_INVITE_GUILD"]].SAFE_SetEvent(self.__OnGuildAddMember)
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_DISMOUNT"]].SAFE_SetEvent(self.__OnDismount)
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_EXIT_OBSERVER"]].SAFE_SetEvent(self.__OnExitObserver)
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_VIEW_EQUIPMENT"]].SAFE_SetEvent(self.__OnViewEquipment)
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY"]].SAFE_SetEvent(self.__OnRequestParty)
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_BUILDING_DESTROY"]].SAFE_SetEvent(self.__OnDestroyBuilding)
		self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_EMOTION_ALLOW"]].SAFE_SetEvent(self.__OnEmotionAllow)

		self.buttonDict[self.BUTTON_NAME_ID["VOTE_BLOCK_CHAT"]].SetEvent(ui.__mem_func__(self.__OnVoteBlockChat))

		if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
			self.hpDecimal = hpDecimal
		if app.ENABLE_SEND_TARGET_INFO:
			self.infoButton = infoButton
		if app.ENABLE_SEND_TARGET_INFO:
			self.vnum = 0
		self.closeButton = closeButton

		if app.ENABLE_HIDE_COSTUME_SYSTEM:
			self.HideCostume = HideCostume

		if app.ENABLE_NEW_TARGET_UI:
			self.tooltipHP = self.TextToolTip()

			self.cirleBGImg = ui.ExpandedImageBox()
			self.cirleBGImg.LoadImage("d:/ymir work/ui/game/target/circle_bg.tga")
			self.cirleBGImg.SetScale(1.1, 1.1)
			self.cirleBGImg.SetParent(self)
			self.cirleBGImg.SetWindowVerticalAlignCenter()
			self.cirleBGImg.SetPosition(-60, 0)

			self.cirleHPImg = ui.ExpandedImageBox()
			self.cirleHPImg.LoadImage("d:/ymir work/ui/game/target/circle_red.tga")
			self.cirleHPImg.SetScale(1.1, 1.1)
			self.cirleHPImg.SetParent(self.cirleBGImg)
			self.cirleHPImg.SetPosition(7 + 1, 6 + 1)
			self.cirleHPImg.Show()

			self.cirleGaguageImg = ui.ExpandedImageBox()
			self.cirleGaguageImg.LoadImage("d:/ymir work/ui/game/target/gauge_center.tga")
			self.cirleGaguageImg.SetScale(1.1, 1.1)
			self.cirleGaguageImg.SetParent(self.cirleHPImg)
			self.cirleGaguageImg.SetPosition(9 + 1, 9 + 1)
			self.cirleGaguageImg.Show()

			self.cirleRaceImg = ui.ExpandedImageBox()
			self.cirleRaceImg.LoadImage("d:/ymir work/ui/game/target/stone_face.tga")
			self.cirleRaceImg.SetScale(1.1, 1.1)
			self.cirleRaceImg.SetParent(self.cirleGaguageImg)
			self.cirleRaceImg.SetPosition(1 + 1, 0 + 1)
			self.cirleRaceImg.Show()

		self.nameString = 0
		self.vid = 0
		self.eventWhisper = None
		self.isShowButton = False
		if app.ENABLE_VIEW_ELEMENT:
			self.elementImage = None
		self.__Initialize()
		self.ResetTargetBoard()

	def __del__(self):
		CLASS_THINBOARD.__del__(self)

		print "===================================================== DESTROYED TARGET BOARD"

	def __Initialize(self):
		self.nameString = ""
		self.vid = 0
		if app.ENABLE_SEND_TARGET_INFO:
			self.vnum = 0
		self.isShowButton = False

	def Destroy(self):
		self.showingButtonList = []

		for btn in self.buttonDict.values():
			btn.SetEvent(None)
			del btn

		self.buttonDict = {}

		if app.ENABLE_SEND_TARGET_INFO:
			self.infoButton = None
		self.eventWhisper = None
		self.closeButton = None

		if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
			self.hpDecimal = None
		if app.ENABLE_HIDE_COSTUME_SYSTEM:
			self.HideCostume = None
		if app.ENABLE_ACCUMULATE_DAMAGE_DISPLAY:
			self.nameText = None
		if self.TargetBoardGM:
			self.TargetBoardGM.Hide()
			self.TargetBoardGM.Destroy()
			self.TargetBoardGM = None

		self.__Initialize()

		if app.ENABLE_NEW_TARGET_UI:
			self.cirleBGImg = None
			self.cirleHPImg = None
			self.cirleGaguageImg = None
			self.cirleRaceImg = None
			self.tooltipHP = None

		if app.ENABLE_VIEW_ELEMENT:
			self.elementImage = None

	if app.ENABLE_SEND_TARGET_INFO:
		def RefreshMonsterInfoBoard(self):
			if not self.infoButton.showWnd.IsShow():
				return
			self.infoButton.showWnd.Refresh()

		def OnPressedInfoButton(self):
			vid = player.GetTargetVID()
			raceNum = nonplayer.GetRaceNumByVID(vid)

			if constInfo.MONSTER_INFO_DATA.has_key(raceNum):
				self.RefreshMonsterInfoBoard()
			else:
				net.SendTargetInfoLoad(vid)

			if self.infoButton.showWnd.IsShow():
				self.infoButton.showWnd.Close()
			elif self.vnum != 0:
				self.infoButton.showWnd.Open(self, self.vnum)

	def OnPressedCloseButton(self):
		player.ClearTarget()
		self.Close()

	def Close(self):
		self.TargetBoardGM.Close()
		self.__Initialize()
		if app.ENABLE_SEND_TARGET_INFO:
			self.infoButton.showWnd.Close()
		self.Hide()
		if app.__AUTO_QUQUE_ATTACK__:
			if self.autoFarmInfo:
				self.autoFarmInfo.Close()
				self.autoFarmInfo=None

	def Open(self, vid, name):
		if vid:
			if not constInfo.GET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD():
				if not player.IsSameEmpire(vid):
					self.Hide()
					return

			if vid != self.GetTargetVID():
				self.ResetTargetBoard()
				self.SetTargetVID(vid)
				self.SetTargetName(name)

			if player.IsMainCharacterIndex(vid):
				self.__ShowMainCharacterMenu()
			elif chr.INSTANCE_TYPE_BUILDING == chr.GetInstanceType(self.vid):
				self.Hide()
			else:
				self.RefreshButton()
				self.Show()
			
			# if chr.IsGameMaster(player.GetMainCharacterIndex()):
			# self.TargetBoardGM.Open(vid)

			if player.GetName() != name:
				self.HideCostume.Show()
				self.UpdatePosition()
			else:
				self.SetSizeNew(65, 1)
				self.UpdatePosition()
		else:
			self.TargetBoardGM.Hide()
			self.HideAllButton()
			self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_WHISPER"])
			self.__ShowButton(self.BUTTON_NAME_ID["VOTE_BLOCK_CHAT"])
			self.__ArrangeButtonPosition()
			self.SetTargetName(name)
			self.Show()

	def Refresh(self):
		if self.IsShow():
			if self.IsShowButton():
				self.RefreshButton()

	def RefreshByVID(self, vid):
		if vid == self.GetTargetVID():
			self.Refresh()

	def RefreshByName(self, name):
		if name == self.GetTargetName():
			self.Refresh()

	def __ShowMainCharacterMenu(self):
		canShow=0

		self.HideAllButton()

		if player.IsMountingHorse():
			self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_DISMOUNT"])
			canShow=1

		if player.IsObserverMode():
			self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_EXIT_OBSERVER"])
			canShow=1

		if canShow:
			self.__ArrangeButtonPosition()
			self.Show()
		else:
			self.Hide()

	def __ShowNameOnlyMenu(self):
		self.HideAllButton()

	def SetWhisperEvent(self, event):
		self.eventWhisper = event

	def UpdatePosition(self):
		if app.TOURNAMENT_PVP_SYSTEM:
			if player.IsTournamentMap():
				self.SetPosition(wndMgr.GetScreenWidth()/2 - self.GetWidth()/2, 114)
			else:
				if app.ENABLE_NEW_TARGET_UI:
					self.SetPosition(wndMgr.GetScreenWidth()/2 - self.GetWidth()/2 + 25, 40)
				else:
					self.SetPosition(wndMgr.GetScreenWidth()/2 - self.GetWidth()/2, 10)
		else:
			if app.ENABLE_NEW_TARGET_UI:#modificare
				self.SetPosition(wndMgr.GetScreenWidth()/2 - self.GetWidth()/2 + 25, 40)
			else:
				self.SetPosition(wndMgr.GetScreenWidth()/2 - self.GetWidth()/2, 10)

	if app.__AUTO_QUQUE_ATTACK__:
		def __AutoFarmInfo(self):
			if not self.autoFarmInfo:
				self.autoFarmInfo = AutoFarmInfoWindow()
			if self.autoFarmInfo.IsShow():
				self.autoFarmInfo.Close()
			else:
				self.autoFarmInfo.Open()
				(x, y) = self.autoFarmBtn.GetGlobalPosition()
				self.autoFarmInfo.SetPosition((x / 2) + 180, y + 30)

	def ResetTargetBoard(self):

		for btn in self.buttonDict.values():
			btn.Hide()

		self.__Initialize()

		self.TargetBoardGM.Hide()
		self.hpDecimal.hpText.Hide()

		if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
			self.hpDecimal.Hide()
		if app.ENABLE_VIEW_ELEMENT and self.elementImage:
			self.elementImage = None
		if app.ENABLE_HIDE_COSTUME_SYSTEM:
			self.HideCostume.Hide()

		if app.ENABLE_SEND_TARGET_INFO:
			self.infoButton.Hide()
			self.infoButton.showWnd.Close()

		self.SetSizeNew(65, 1)

	def SetTargetVID(self, vid):
		self.vid = vid
		if app.ENABLE_SEND_TARGET_INFO:
			self.vnum = 0

	if app.ENABLE_VIEW_ELEMENT:
		def SetElementImage(self,elementId):
			try:
				if elementId > 0 and elementId in ELEMENT_IMAGE_DIC.keys():
					self.elementImage = ui.ImageBox()
					self.elementImage.SetParent(self.name)
					self.elementImage.SetPosition(-143,-12)
					self.elementImage.LoadImage("d:/ymir work/ui/game/12zi/element/%s.sub" % (ELEMENT_IMAGE_DIC[elementId]))
					self.elementImage.Show()
			except:
				pass
				
	def SetEnemyVID(self, vid):
		self.SetTargetVID(vid)
		if app.ENABLE_SEND_TARGET_INFO:
			vnum = nonplayer.GetRaceNumByVID(vid)
		name = chr.GetNameByVID(vid)
		level = nonplayer.GetLevelByVID(vid)
		grade = nonplayer.GetGradeByVID(vid)

		nameFront = ""
		#if -1 != level:
		if chr.GetInstanceType(self.vid) == chr.INSTANCE_TYPE_ENEMY or chr.GetInstanceType(self.vid) == chr.INSTANCE_TYPE_STONE:
			nameFront += "Lv." + str(level) + " "
		if self.GRADE_NAME.has_key(grade):
			nameFront += "(" + self.GRADE_NAME[grade] + ") "
			
		if vnum in [ 0, 1, 2, 3, 4, 5, 6, 7, 8 ]:
			self.canOpenGM = True
		else:
			self.canOpenGM = False


		self.SetTargetName(nameFront + name)

		if app.ENABLE_ACCUMULATE_DAMAGE_DISPLAY:
			accumulate_count = player.GetAccumulateDamageByVID(vid)
			# self.nameText.SetText( localeInfo.TARGET_TOOLTIP_ATTACK_COUNT % accumulate_count )
			self.nameText.Show()

		if app.ENABLE_SEND_TARGET_INFO:
			(textWidth, textHeight) = self.nameText.GetTextSize()

			self.infoButton.SetPosition(textWidth + 45, 12)
			self.infoButton.SetWindowHorizontalAlignLeft()

			self.vnum = vnum
			if chr.GetInstanceType(self.vid) == chr.INSTANCE_TYPE_PLAYER:
				self.infoButton.Hide()
			else:
				self.infoButton.Show()

	def GetTargetVID(self):
		return self.vid

	def GetTargetName(self):
		return self.nameString

	def SetTargetName(self, name):
		self.nameString = name
		self.nameText.SetText(name)

	# if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
	def SetHP(self, hpPercentage, iMinHP, iMaxHP):
		if app.ENABLE_VIEW_TARGET_PLAYER_HP:
			if self.showingButtonList:
				c = 0
				for i in self.showingButtonList:
					if i.IsShow():
						c += 1

				showingButtonCount = c
			else:
				showingButtonCount = 0

			self.SetSizeNew(65, showingButtonCount)
		else:
			self.SetSizeNew(65, 7)

		self.UpdatePosition()

		if chr.IsGameMaster(player.GetMainCharacterIndex()) and chr.GetInstanceType(self.vid) == chr.INSTANCE_TYPE_PLAYER:
			self.TargetBoardGM.Open(self.vid)

		self.hpDecimal.hpText.SetText("(" + str(hpPercentage) + "%" + ")")
		self.hpDecimal.hpText.Show()

		# if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
		iMinHPText = '.'.join([i - 3 < 0 and str(iMinHP)[:i] or str(iMinHP)[i-3:i] for i in range(len(str(iMinHP)) % 3, len(str(iMinHP))+1, 3) if i])
		iMaxHPText = '.'.join([i - 3 < 0 and str(iMaxHP)[:i] or str(iMaxHP)[i-3:i] for i in range(len(str(iMaxHP)) % 3, len(str(iMaxHP))+1, 3) if i])
		self.hpDecimal.SetText(str(iMinHPText) + "/" + str(iMaxHPText))
		(textWidth, textHeight) = self.hpDecimal.GetTextSize()

		if localeInfo.IsARABIC():
			self.hpDecimal.SetPosition(120 / 2 + textWidth / 2, -13)
		else:
			self.hpDecimal.hpText.SetPosition(textWidth + 20, 0)

		self.hpDecimal.Show()

		self.cirleBGImg.Show()
		self.cirleHPImg.DisplayProcent(hpPercentage)
		self.cirleGaguageImg.Show()
		self.hpboard.Show()
		race = nonplayer.GetRaceNumByVID(self.vid)

		self.cirleRaceImg.UnloadImage()

		if race == 0 or race == 1 or race == 2 or race == 3 or race == 4 or race == 5 or race == 6 or race == 7 or race == 8:
			self.cirleRaceImg.LoadImage("d:/ymir work/ui/game/target/race/circle_%d.tga" % race)

			self.hpboard.UnloadImage()
			self.hpboard.LoadImage("d:/ymir work/ui/game/target/target_hp_bar_background.png")
			self.hpboard.SetScale(1.1, 1.1)
		elif race >= 8001 and race <= 8062:
			self.cirleRaceImg.LoadImage("d:/ymir work/ui/game/target/stone_face.tga")
		else:
			self.cirleRaceImg.LoadImage("d:/ymir work/ui/game/target/monster_face.tga")

		self.cirleRaceImg.SetScale(1.1, 1.1)

	if app.ENABLE_HIDE_COSTUME_SYSTEM:
		def SetCostumeHide(self, costumeHide):
			c_h=costumeHide.split("|")
			try:
				armor = c_h[0]
				hair = c_h[1]

				if app.ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME:
					weapon = c_h[2]

				if app.ENABLE_HIDE_COSTUME_SYSTEM_ACCE:
					acce = c_h[3]


				self.toolTip.ClearToolTip()
				self.toolTip.SetTitle("Hide Costume Info")
				self.toolTip.AppendTextLine("Armor: %s"%(self.GetInfoHide(armor)))
				self.toolTip.AppendTextLine("Hair: %s"%(self.GetInfoHide(hair)))
				if app.ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME:
					self.toolTip.AppendTextLine("Weapon: %s"%(self.GetInfoHide(weapon)))

				if app.ENABLE_HIDE_COSTUME_SYSTEM_ACCE:
					self.toolTip.AppendTextLine("Sash: %s"%(self.GetInfoHide(acce)))
			except Exception as msg:
				import dbg
				dbg.TraceError(str(msg))

		def GetInfoHide(self,index):
			if int(index) == 0:
				return "|cFF1EBD11|hShow"
			return "|cFFDB1A1A|hHide"

		def OnMouseOverInHideCostume(self):
			self.toolTip.ShowToolTip()

		def OnMouseOverOutHideCostume(self):
			self.toolTip.HideToolTip()

	def ShowDefaultButton(self):
		self.isShowButton = True

		self.showingButtonList.append(self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_WHISPER"]])
		self.showingButtonList.append(self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_EXCHANGE"]])
		self.showingButtonList.append(self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FIGHT"]])
		self.showingButtonList.append(self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_EMOTION_ALLOW"]])

		for button in self.showingButtonList:
			button.Show()

	def HideAllButton(self):
		self.isShowButton = False

		for button in self.showingButtonList:
			button.Hide()

		self.showingButtonList = []

	if app.ENABLE_ACCUMULATE_DAMAGE_DISPLAY:
		def RefreshAccumulateCount(self, vid):
			accumulate_count = player.GetAccumulateDamageByVID(vid)
			if accumulate_count:
				# self.nameText.SetText( localeInfo.TARGET_TOOLTIP_ATTACK_COUNT % accumulate_count )
				self.nameText.Show()
			else:
				self.nameText.Show()

	def __ShowButton(self, name):

		if not self.buttonDict.has_key(name):
			return

		self.buttonDict[name].Show()
		self.showingButtonList.append(self.buttonDict[name])

	def __HideButton(self, name):

		if not self.buttonDict.has_key(name):
			return

		button = self.buttonDict[name]
		button.Hide()

		for btnInList in self.showingButtonList:
			if btnInList == button:
				self.showingButtonList.remove(button)
				break

	def OnWhisper(self):
		if None != self.eventWhisper:
			self.eventWhisper(self.nameString)

	def OnExchange(self):
		net.SendExchangeStartPacket(self.vid)

	def OnPVP(self):
		net.SendChatPacket("/pvp %d" % (self.vid))

	def OnAppendToMessenger(self):
		net.SendMessengerAddByVIDPacket(self.vid)

	def OnPartyInvite(self):
		net.SendPartyInvitePacket(self.vid)

	def OnPartyExit(self):
		net.SendPartyExitPacket()

	def OnPartyRemove(self):
		net.SendPartyRemovePacketVID(self.vid)

	def __OnGuildAddMember(self):
		net.SendGuildAddMemberPacket(self.vid)

	def __OnDismount(self):
		net.SendChatPacket("/unmount")

	def __OnExitObserver(self):
		net.SendChatPacket("/observer_exit")

	def __OnViewEquipment(self):
		net.SendChatPacket("/view_equip " + str(self.vid))

	def __OnRequestParty(self):
		net.SendChatPacket("/party_request " + str(self.vid))

	def __OnDestroyBuilding(self):
		net.SendChatPacket("/build d %d" % (self.vid))

	def __OnEmotionAllow(self):
		net.SendChatPacket("/emotion_allow %d" % (self.vid))

	def __OnVoteBlockChat(self):
		cmd = "/vote_block_chat %s" % (self.nameString)
		net.SendChatPacket(cmd)

	def OnPressEscapeKey(self):
		self.OnPressedCloseButton()
		return True

	def IsShowButton(self):
		return self.isShowButton

	def RefreshButton(self):
		self.HideAllButton()

		if chr.INSTANCE_TYPE_BUILDING == chr.GetInstanceType(self.vid):
			#self.__ShowButton(localeInfo.TARGET_BUTTON_BUILDING_DESTROY)
			#self.__ArrangeButtonPosition()
			return

		if player.IsPVPInstance(self.vid) or player.IsObserverMode():
			self.SetSizeNew(65, 1)
			self.UpdatePosition()
			# END_OF_PVP_INFO_SIZE_BUG_FIX
			return

		self.ShowDefaultButton()

		if guild.MainPlayerHasAuthority(guild.AUTH_ADD_MEMBER):
			if not guild.IsMemberByName(self.nameString):
				if 0 == chr.GetGuildID(self.vid):
					self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_INVITE_GUILD"])

		if not messenger.IsFriendByName(self.nameString):
			self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FRIEND"])

		if player.IsPartyMember(self.vid):

			self.__HideButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FIGHT"])

			if player.IsPartyLeader(self.vid):
				self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_LEAVE_PARTY"])
			elif player.IsPartyLeader(player.GetMainCharacterIndex()):
				self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_EXCLUDE"])

		else:
			if player.IsPartyMember(player.GetMainCharacterIndex()):
				if player.IsPartyLeader(player.GetMainCharacterIndex()):
					self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_INVITE_PARTY"])
			else:
				if chr.IsPartyMember(self.vid):
					self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY"])
				else:
					self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_INVITE_PARTY"])

			if player.IsRevengeInstance(self.vid):
				self.__HideButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FIGHT"])
				self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_AVENGE"])
			elif player.IsChallengeInstance(self.vid):
				self.__HideButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FIGHT"])
				self.__ShowButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_ACCEPT_FIGHT"])
			elif player.IsCantFightInstance(self.vid):
				self.__HideButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FIGHT"])

			if not player.IsSameEmpire(self.vid):
				self.__HideButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_INVITE_PARTY"])
				self.__HideButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FRIEND"])
				self.__HideButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_FIGHT"])

		distance = player.GetCharacterDistance(self.vid)
		if distance > self.EXCHANGE_LIMIT_RANGE:
			self.__HideButton(self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_EXCHANGE"])
			self.__ArrangeButtonPosition()

		self.__ArrangeButtonPosition()

	def SetSizeNew(self, height, showingButtonCount):
		self.SetSize(300 + showingButtonCount * 30, height)

		race = 101

		if self.vid > 0:
			race = nonplayer.GetRaceNumByVID(self.vid)

		# if self.name:
			# if race < 10:
				# self.nameBoard.Hide()
			# else:
				# self.nameBoard.Show()

	def __ArrangeButtonPosition(self):
		showingButtonCount = len(self.showingButtonList)

		pos = -(showingButtonCount / 2) * 68
		if 0 == showingButtonCount % 2:
			pos += 34

		for button in self.showingButtonList:
			button.SetPosition(pos, 33)
			pos += 68

		self.SetSizeNew(65, showingButtonCount)
		self.UpdatePosition()

	def IsTargetPoisoned( self  ):
		return chrmgr.HasAffectByVID( self.GetTargetVID( ), chr.AFFECT_POISON ) != 0

	def OnUpdate(self):
		if self.isShowButton:
			exchangeButton = self.buttonDict[self.BUTTON_NAME_ID["localeInfo.TARGET_BUTTON_EXCHANGE"]]
			distance = player.GetCharacterDistance(self.vid)

			if distance < 0:
				return

			if exchangeButton.IsShow():
				if distance > self.EXCHANGE_LIMIT_RANGE:
					self.RefreshButton()
			else:
				if distance < self.EXCHANGE_LIMIT_RANGE:
					self.RefreshButton()

		if app.ENABLE_NEW_TARGET_UI:
			if self.cirleBGImg.IsShow():
				self.cirleHPImg.UnloadImage()

				if self.IsTargetPoisoned():
					self.cirleHPImg.LoadImage("d:/ymir work/ui/game/target/circle_green.tga")
				else:
					self.cirleHPImg.LoadImage("d:/ymir work/ui/game/target/circle_red.tga")

				self.cirleHPImg.SetScale(1.1, 1.1)

				radius = 39
				mouse = mouseModule.mouseController
				x, y = self.cirleBGImg.GetGlobalPosition()
				posX = mouse.x - (x + radius)
				posY = mouse.y - (y + radius)

				if pow(posX, 2) + pow(posY, 2) <= pow(radius, 2):
					self.tooltipHP.Show()
				else:
					self.tooltipHP.Hide()

if app.__AUTO_QUQUE_ATTACK__:
	class AutoFarmInfoWindow(ui.ThinBoard):
		def __init__(self):
			ui.ThinBoard.__init__(self)
			self.__LoadWindow()

		def __LoadWindow(self):
			self.SetSize(356, 166)
			self.AddFlag("float")
			self.AddFlag("not_pick")

			title = ui.TextLine()
			title.SetParent(self)
			title.AddFlag("not_pick")
			title.SetHorizontalAlignCenter()
			title.SetPackedFontColor(0xFFFBC401)
			title.SetOutline()
			title.SetText("Target Metin Queue")
			title.SetPosition(self.GetWidth() / 2, 15)
			title.Show()
			self.title = title
			
			firstText = "Pressing |cFF74FF20Shift + Right Click|r on Metinstones your character will target#"\
			"a MetinStone and after you destroyed it, it will move to |cFF74FF20another one|r."

			firstDescription = ui.MultiTextLineNew()
			firstDescription.SetParent(self)
			firstDescription.AddFlag("not_pick")
			firstDescription.SetTextRange(13)
			firstDescription.SetTextType("horizontal#center")
			firstDescription.SetPosition(self.GetWidth() / 2, 37)
			firstDescription.SetOutline(1)
			firstDescription.SetText(firstText)
			firstDescription.Show()
			self.firstDescription = firstDescription

			secondText = "Max. 3 queued Metinstones (without Premium Status)#"\
			"Max. 5 queued Metinstones (with Premium status)"

			secondDescription = ui.MultiTextLineNew()
			secondDescription.SetParent(self)
			secondDescription.AddFlag("not_pick")
			secondDescription.SetTextRange(13)
			secondDescription.SetTextType("horizontal#center")
			secondDescription.SetPosition(self.GetWidth() / 2, 73)
			secondDescription.SetOutline(1)
			secondDescription.SetPackedFontColor(0xFFFFD071)
			secondDescription.SetText(secondText)
			secondDescription.Show()
			self.secondDescription = secondDescription


			item.SelectItem(40001)
			itemIcon = ui.ImageBox()
			itemIcon.SetParent(self)
			itemIcon.AddFlag("not_pick")
			itemIcon.LoadImage(item.GetIconImageFileName())
			itemIcon.SetPosition((self.GetWidth() / 2) - 16, 105)
			itemIcon.Show()
			self.itemIcon = itemIcon

			itemName = ui.TextLine()
			itemName.SetParent(self)
			itemName.AddFlag("not_pick")
			itemName.SetHorizontalAlignCenter()
			itemName.SetPackedFontColor(0xFFFBC401)
			itemName.SetOutline()
			itemName.SetText("Premium Status")
			itemName.SetPosition(self.GetWidth() / 2, 105 + 32 + 5)
			itemName.Show()
			self.itemName = itemName

			self.SetCenterPosition()

		def Open(self):
			self.Show()
			self.SetTop()
		def Close(self):
			self.Hide()
		def OnPressEscapeKey(self):
			self.Close()
			return True
