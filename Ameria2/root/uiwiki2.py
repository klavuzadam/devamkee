# coding: latin_1

import ui, grp, app, wiki, item, nonplayer, renderTarget, constInfo, skill, WikiUI
import localeInfo
import chat
import dbg

# if false will be textline!
USE_ITEM_COUNT_NUMBER_LINE = False

# if this true item refine showing start index in +0 than +9 will be next item if have!
SHOW_NEXT_ITEM_REFINE = False

SHOW_ITEM_LOWER_TO_BIG = False
# category load speed
AUTOLOAD_SPEED = 0.010
AUTOLOAD_MONSTER_SPEED = 0.020

IMG_DIR = "d:/ymir work/ui/game/wiki/"
class EncyclopediaofGame(ui.BoardWithTitleBar):
	def __del__(self):
		ui.BoardWithTitleBar.__del__(self)

	def Destroy(self):
		if self.children.has_key("listBoxCube"):
			self.children["listBoxCube"].ClearItem()
			self.children["listBoxCube"]=None
			del self.children["listBoxCube"]

		if self.children.has_key("resultpageListbox"):
			self.children["resultpageListbox"].RemoveAllItems()
			self.children["resultpageListbox"]=None
			del self.children["resultpageListbox"]

		self.selectArg=""
		self.AIAppendAlgoritm = None

		if len(self.children) != 0:
			ui.BoardWithTitleBar.Destroy(self)
			constInfo.SetMainParent(None)
			constInfo.SetListBox(None)

		self.children = {}

	def __init__(self):
		ui.BoardWithTitleBar.__init__(self)
		self.SetWindowName("EncyclopediaofGame")
		self.AddFlag("movable")
		self.AddFlag("float")
		self.AddFlag("animate")
		self.children = {}

		self.Destroy()

		# Default variable
		self.children["characterIndex"]=0
		self.Initialize()

	def Initialize(self):
		self.SetSize(720, 480)
		self.SetTitleName(localeInfo.WIKI_TITLE)
		self.SetCenterPosition()

		self.LoadSearchInfos()
		self.LoadCategoryInfos()
		self.LoadBlock()
		self.LoadResultPage()

	def LoadResultPage(self):
		resultPageImage = ui.ImageBox()
		resultPageImage.SetParent(self)
		resultPageImage.LoadImage(IMG_DIR+"result_wall.tga")
		resultPageImage.SetPosition(150,104)
		resultPageImage.Show()
		self.children["resultPageImage"] = resultPageImage

		for j in xrange(4):
			characterBtn =  ui.RadioButton()
			characterBtn.SetParent(resultPageImage)
			characterBtn.SetEvent(ui.__mem_func__(self.__SelectCharacters),j)
			characterBtn.SetUpVisual(IMG_DIR+"character/%d_0.tga"%j)
			characterBtn.SetOverVisual(IMG_DIR+"character/%d_1.tga"%j)
			characterBtn.SetDownVisual(IMG_DIR+"character/%d_2.tga"%j)
			characterBtn.SetPosition(16+(132*j),11)
			characterBtn.Hide()
			self.children["job_%d_characterBtn"%j] = characterBtn

		resultpageListboxScrollbar = WikiUI.ScrollBarNew()
		resultpageListboxScrollbar.SetParent(self)
		self.children["resultpageListboxScrollbar"] = resultpageListboxScrollbar

		resultpageListbox = WikiUI.ListBox()
		resultpageListbox.SetParent(self)
		resultpageListbox.SetScrollBar(resultpageListboxScrollbar)
		self.children["resultpageListbox"] = resultpageListbox

		constInfo.SetMainParent(self)
		constInfo.SetListBox(resultpageListbox)
		
		resultpagebtn = ui.ExpandedImageBox()
		resultpagebtn.SetParent(self) # <board>
		resultpagebtn.SetPosition(150,32)
		resultpagebtn.SetEvent(ui.__mem_func__(self.LoadGuidePage),"mouse_click")
		resultpagebtn.Show()
		self.children["resultpagebtn"] = resultpagebtn

		self.LoadGuidePage()

	def LoadGuidePage(self, emptyArg = ""):
		self.__SelectType("System#0",False,False)

	def LoadBlock(self):
		blockImage = ui.ImageBox()
		blockImage.SetParent(self)
		blockImage.LoadImage(IMG_DIR+"block.tga")
		blockImage.SetPosition(140,32)
		blockImage.Show()
		self.children["blockImage"] = blockImage

	def LoadSearchInfos(self):
		self.children["selectedMob"] = 0
		self.children["selectedItem"] = 0

		searchSlot = ui.ImageBox()
		searchSlot.SetParent(self)
		searchSlot.LoadImage(IMG_DIR+"search_slot.tga")
		searchSlot.SetPosition(13,32)
		searchSlot.Show()
		self.children["searchSlot"] = searchSlot

		searchButton = ui.Button()
		searchButton.SetParent(searchSlot)
		searchButton.SetUpVisual(IMG_DIR+"button_0.tga")
		searchButton.SetOverVisual(IMG_DIR+"button_1.tga")
		searchButton.SetDownVisual(IMG_DIR+"button_2.tga")
		searchButton.SAFE_SetEvent(self.StartSearchItem)
		searchButton.SetPosition(91,3)
		searchButton.Show()
		self.children["searchButton"] = searchButton

		searchItemName = ui.EditLine()
		searchItemName.SetParent(searchSlot)
		searchItemName.SetPosition(2,5)
		searchItemName.SetSize(91,26)
		#searchItemName.SetFontName("Tahoma:11")
		searchItemName.SetInfoMessage(localeInfo.WIKI_ITEM_NAME)
		searchItemName.SetMax(30)
		searchItemName.isNeedEmpty = False
		searchItemName.OnPressEscapeKey = ui.__mem_func__(self.Close)
		searchItemName.SetOutline()
		searchItemName.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateItem)
		searchItemName.SetReturnEvent(ui.__mem_func__(self.StartSearchItem))
		searchItemName.Show()
		self.children["searchItemName"] = searchItemName

		searchClearBtn = ui.Button()
		searchClearBtn.SetParent(searchSlot)
		searchClearBtn.SetUpVisual(IMG_DIR+"clear_button_1.tga")
		searchClearBtn.SetOverVisual(IMG_DIR+"clear_button_2.tga")
		searchClearBtn.SetDownVisual(IMG_DIR+"clear_button_1.tga")
		searchClearBtn.SetPosition(75,5)
		searchClearBtn.SAFE_SetEvent(self.ClearEditlineItem)
		searchClearBtn.Hide()
		self.children["searchClearBtn"] = searchClearBtn

		mobSlot = ui.ImageBox()
		mobSlot.SetParent(self)
		mobSlot.LoadImage(IMG_DIR+"search_slot.tga")
		mobSlot.SetPosition(13,32+29)
		mobSlot.Show()
		self.children["mobSlot"] = mobSlot

		searchButtonMob = ui.Button()
		searchButtonMob.SetParent(mobSlot)
		searchButtonMob.SetUpVisual(IMG_DIR+"button_0.tga")
		searchButtonMob.SetOverVisual(IMG_DIR+"button_1.tga")
		searchButtonMob.SetDownVisual(IMG_DIR+"button_2.tga")
		searchButtonMob.SAFE_SetEvent(self.StartSearchMob)
		searchButtonMob.SetPosition(91,2)
		searchButtonMob.Show()
		self.children["searchButtonMob"] = searchButtonMob

		searchMobName = ui.EditLine()
		searchMobName.SetParent(mobSlot)
		searchMobName.SetPosition(2,5)
		searchMobName.SetSize(91,26)
		#searchMobName.SetFontName("Tahoma:11")
		searchMobName.SetInfoMessage(localeInfo.WIKI_MOB_NAME)
		searchMobName.isNeedEmpty = False
		searchMobName.SetMax(30)
		searchMobName.SetOutline()
		searchMobName.OnPressEscapeKey = ui.__mem_func__(self.Close)
		searchMobName.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMob)
		searchMobName.SetReturnEvent(ui.__mem_func__(self.StartSearchMob))
		searchMobName.Show()
		self.children["searchMobName"] = searchMobName

		searchClearBtnMob = ui.Button()
		searchClearBtnMob.SetParent(mobSlot)
		searchClearBtnMob.SetUpVisual(IMG_DIR+"clear_button_1.tga")
		searchClearBtnMob.SetOverVisual(IMG_DIR+"clear_button_2.tga")
		searchClearBtnMob.SetDownVisual(IMG_DIR+"clear_button_1.tga")
		searchClearBtnMob.SetPosition(75,5)
		searchClearBtnMob.SAFE_SetEvent(self.ClearEditlineMob)
		searchClearBtnMob.Hide()
		self.children["searchClearBtnMob"] = searchClearBtnMob

	def SetHistoryButtons(self):
		historyBack = self.children["historyBack"]
		historyNext = self.children["historyNext"]

		#currentSearch = self.children["currentSearch"]
		currentIndex = self.children["currentIndex"]
		historySearch = self.children["historySearch"]

		if len(historySearch) == 0:
			historyNext.SetUpVisual(IMG_DIR+"btn_arrow_right_normal.tga")
			historyNext.SetOverVisual(IMG_DIR+"btn_arrow_right_normal.tga")
			historyNext.SetDownVisual(IMG_DIR+"btn_arrow_right_normal.tga")
			historyBack.SetUpVisual(IMG_DIR+"btn_arrow_left_normal.tga")
			historyBack.SetOverVisual(IMG_DIR+"btn_arrow_left_normal.tga")
			historyBack.SetDownVisual(IMG_DIR+"btn_arrow_left_normal.tga")
			return

		#chat.AppendChat(1,"currentIndex %d len %d"%(currentIndex, len(historySearch)))
		if currentIndex > 0:
			historyBack.SetUpVisual(IMG_DIR+"btn_arrow_left_normal.tga")
			historyBack.SetOverVisual(IMG_DIR+"btn_arrow_left_hover.tga")
			historyBack.SetDownVisual(IMG_DIR+"btn_arrow_left_down.tga")
		else:
			historyBack.SetUpVisual(IMG_DIR+"btn_arrow_left_normal.tga")
			historyBack.SetOverVisual(IMG_DIR+"btn_arrow_left_normal.tga")
			historyBack.SetDownVisual(IMG_DIR+"btn_arrow_left_normal.tga")

		if currentIndex+1 >= len(historySearch):
			historyNext.SetUpVisual(IMG_DIR+"btn_arrow_right_normal.tga")
			historyNext.SetOverVisual(IMG_DIR+"btn_arrow_right_normal.tga")
			historyNext.SetDownVisual(IMG_DIR+"btn_arrow_right_normal.tga")
		else:
			historyNext.SetUpVisual(IMG_DIR+"btn_arrow_right_normal.tga")
			historyNext.SetOverVisual(IMG_DIR+"btn_arrow_right_hover.tga")
			historyNext.SetDownVisual(IMG_DIR+"btn_arrow_right_down.tga")

	def RunHistoryArgument(self, argument):
		if argument.find("NEW") != -1:
			argumentList = argument.split("#")
			self.ShowItemInfo(int(argumentList[1]), int(argumentList[2]), False)
		else:
			self.__SelectType(argument, False, False)

	def ClickBackHistory(self):
		currentIndex = self.children["currentIndex"]
		historySearch = self.children["historySearch"]
		if currentIndex-1 < 0:
			return
		currentIndex-=1
		self.children["currentIndex"]=currentIndex
		self.RunHistoryArgument(historySearch[currentIndex])
		self.SetHistoryButtons()

	def ClickNextHistory(self):
		currentIndex = self.children["currentIndex"]
		historySearch = self.children["historySearch"]
		if currentIndex+1 >= len(historySearch):
			return
		currentIndex+=1
		self.children["currentIndex"]=currentIndex
		self.RunHistoryArgument(historySearch[currentIndex])
		self.SetHistoryButtons()

	def get_length(self, x):
		return len(x[0])

	def UpdateItemsList(self):
		input_text_real = self.children["searchItemName"].GetText()
		input_len = len(input_text_real)
		if input_len == 0:
			self.ClearEditlineItem()
			return False
		if localeInfo.IsARABIC():
			input_text = input_text_real
		else:
			input_text = input_text_real.lower()
		self.children["searchClearBtn"].Show()
		items_list = item.GetItemsByName(str(input_text))
		itemList = []
		namesList = []
		for i, itemVnum in enumerate(items_list, start=1):
			(realVnum, isRefineItem) = WikiUI.getRealVnum(itemVnum)
			if isRefineItem:
				realVnum += wiki.GetRefineMaxLevel(realVnum)
				if itemVnum != realVnum:
					continue
			item.SelectItem(itemVnum)
			if localeInfo.IsARABIC():
				itemName = item.GetItemName()
			else:
				itemName = item.GetItemName().lower()
			if itemName.find("+") != -1:
				itemName = itemName[:itemName.find("+")]
			tempName = list(itemName)
			for i in xrange(input_len):
				tempName[i]=list(input_text_real)[i]
			itemName = ""
			for x in xrange(len(tempName)):
				itemName+=tempName[x]
			if itemName in namesList:
				continue
			namesList.append(itemName)
			itemList.append([itemName, realVnum])
		if len(itemList) > 0:
			if len(itemList) > 1:
				itemList = sorted(itemList, key=self.get_length,reverse=False)
			self.children["selectedItem"] = itemList[0][1]
			self.children["searchItemName"].SetInfoMessage(itemList[0][0])
		else:
			self.children["selectedItem"] = 0
			self.children["searchItemName"].SetInfoMessage("")
		return True

	def __OnValueUpdateItem(self):
		ui.EditLine.OnIMEUpdate(self.children["searchItemName"])
		if not self.UpdateItemsList():
			self.ClearEditlineItem()

		def OnKeyDown(self, key):
			if app.DIK_RETURN == key:
				if self.children["selectedItem"] > 1:
					self.StartSearchItem()
				else:
					self.StartSearchItem()
					return True
			return True

	def ClearEditlineItem(self):
		self.children["selectedItem"]=0
		self.children["searchItemName"].SetText("")
		self.children["searchItemName"].SetInfoMessage(localeInfo.WIKI_ITEM_NAME)
		self.children["searchClearBtn"].Hide()

	def StartSearchItem(self):
		if self.children["selectedItem"] != 0:
			self.ShowItemInfo(self.children["selectedItem"],0)

	def UpdateMobsList(self):
		input_text_real = self.children["searchMobName"].GetText()
		input_len = len(input_text_real)
		if input_len == 0:
			self.ClearEditlineMob()
			return False
		if localeInfo.IsARABIC():
			input_text = input_text_real
		else:
			input_text = input_text_real.lower()
		self.children["searchClearBtnMob"].Show()
		mobs_list = nonplayer.GetMobsByName(str(input_text))
		mobList = []
		namesList = []
		for i, mobVnum in enumerate(mobs_list, start=1):
			if localeInfo.IsARABIC():
				mob_name = nonplayer.GetMonsterName(mobVnum)
			else:
				mob_name = nonplayer.GetMonsterName(mobVnum).lower()
			tempName = list(mob_name)
			for i in xrange(input_len):
				tempName[i]=list(input_text_real)[i]
			mob_name = ""
			for x in xrange(len(tempName)):
				mob_name+=tempName[x]
			if mob_name in namesList:
				continue
			namesList.append(mob_name)
			mobList.append([mob_name, mobVnum])
		if len(mobList) > 0:
			if len(mobList) > 1:
				mobList = sorted(mobList, key=self.get_length,reverse=False)
			self.children["selectedMob"] = mobList[0][1]
			self.children["searchMobName"].SetInfoMessage(mobList[0][0])
		else:
			self.children["selectedMob"] = 0
			self.children["searchMobName"].SetInfoMessage("")
		return True

	def __OnValueUpdateMob(self):
		ui.EditLine.OnIMEUpdate(self.children["searchMobName"])
		if not self.UpdateMobsList():
			self.ClearEditlineMob()
		def OnKeyDown(self, key):
			if app.DIK_RETURN == key:
				if self.children["selectedMob"] > 1:
					self.StartSearchMob()
				else:
					self.StartSearchMob()
					return True
			return True

	def ClearEditlineMob(self):
		self.children["selectedMob"]=0
		self.children["searchMobName"].SetText("")
		self.children["searchMobName"].SetInfoMessage(localeInfo.WIKI_MOB_NAME)
		self.children["searchClearBtnMob"].Hide()

	def StartSearchMob(self):
		if self.children["selectedMob"] != 0:
			self.ShowItemInfo(self.children["selectedMob"],1)

	def LoadCategoryInfos(self):
		categoryText = ui.TextLine()
		categoryText.SetParent(self)
		categoryText.SetPosition(13,89)
		categoryText.SetText(localeInfo.WIKI_CATEGORY)
		categoryText.Show()
		self.children["categoryText"] = categoryText

		scrollBarListBoxCube = WikiUI.ScrollBarNew()
		scrollBarListBoxCube.SetParent(self)
		scrollBarListBoxCube.SetPosition(109+15,105)

		scrollBarListBoxCube.Show()
		self.children["scrollBarListBoxCube"] = scrollBarListBoxCube

		listBoxCube = WikiUI.CategoryList()
		listBoxCube.SetParent(self)
		listBoxCube.SetPosition(13,105)
		listBoxCube.SetSize(109, 355)
		listBoxCube.Show()
		listBoxCube.SetScrollBar(scrollBarListBoxCube)
		self.children["listBoxCube"] = listBoxCube
		scrollBarListBoxCube.SetScrollBarSize(listBoxCube.GetHeight()-5)

		self.children["historySearch"] = []
		self.children["currentIndex"] = 0

		historyBack = ui.Button()
		historyBack.SetParent(self)
		historyBack.SetUpVisual(IMG_DIR+"btn_arrow_left_normal.tga")
		historyBack.SetOverVisual(IMG_DIR+"btn_arrow_left_normal.tga")
		historyBack.SetDownVisual(IMG_DIR+"btn_arrow_left_normal.tga")
		historyBack.SetPosition(13,105+listBoxCube.GetHeight()-5)
		historyBack.SAFE_SetEvent(self.ClickBackHistory)
		historyBack.Show()
		self.children["historyBack"] = historyBack

		historyNext = ui.Button()
		historyNext.SetParent(self)
		historyNext.SetUpVisual(IMG_DIR+"btn_arrow_right_normal.tga")
		historyNext.SetOverVisual(IMG_DIR+"btn_arrow_right_normal.tga")
		historyNext.SetDownVisual(IMG_DIR+"btn_arrow_right_normal.tga")
		historyNext.SetPosition(13+historyBack.GetWidth()+2,105+listBoxCube.GetHeight()-5)
		historyNext.SAFE_SetEvent(self.ClickNextHistory)
		historyNext.Show()
		self.children["historyNext"] = historyNext

		listBoxCubeItems = [
		{
			'item' : self.CreateCategoryItem(localeInfo.WIKI_EQUIPMENT, lambda arg = ("Equipment#"): self.__EmptyFunction(arg)), 
			'children' : (
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_EQUIPMENT_WEAPONS, lambda arg = ("Equipment#0"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_EQUIPMENT_ARMOR, lambda arg = ("Equipment#1"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_EQUIPMENT_HELMET, lambda arg = ("Equipment#2"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_EQUIPMENT_SHIELD, lambda arg = ("Equipment#3"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_EQUIPMENT_EARRING, lambda arg = ("Equipment#4"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_EQUIPMENT_BRACELET, lambda arg = ("Equipment#5"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_EQUIPMENT_NECKLACE, lambda arg = ("Equipment#6"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_EQUIPMENT_SHOES, lambda arg = ("Equipment#7"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_EQUIPMENT_BELT, lambda arg = ("Equipment#8"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_EQUIPMENT_TALISMAN, lambda arg = ("Equipment#9"): self.__SelectType(arg)),},
			)
		},
		{
			'item' : self.CreateCategoryItem("Costume", lambda arg = ("Costume#"): self.__EmptyFunction(arg)), 
			'children' : (
				{'item' : self.CreateCategorySubItem("Weapon", lambda arg = ("Costume#0"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem("Armor", lambda arg = ("Costume#1"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem("Hair", lambda arg = ("Costume#2"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem("Sash", lambda arg = ("Costume#3"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem("Shining", lambda arg = ("Costume#4"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem("Pet", lambda arg = ("Costume#5"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem("Mount", lambda arg = ("Costume#6"): self.__SelectType(arg)),},
			)
		},
		{ 
			'item' : self.CreateCategoryItem(localeInfo.WIKI_CHESTS, lambda arg = ("Chests#"): self.__EmptyFunction(arg)), 
			'children' : (
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_CHESTS_BOSS, lambda arg = ("Chests#0"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_CHESTS_EVENT, lambda arg = ("Chests#1"): self.__SelectType(arg))},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_CHESTS_ALTERNATIVE, lambda arg = ("Chests#2"): self.__SelectType(arg))},
			)
		},
		{ 
			'item' : self.CreateCategoryItem(localeInfo.WIKI_BOSSES, lambda arg = ("Bosses#"): self.__EmptyFunction(arg)), 
			'children' : (
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_1_75, lambda arg = ("Bosses#0"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_76_100, lambda arg = ("Bosses#1"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_100, lambda arg = ("Bosses#2"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_BOSSES_EVENT, lambda arg = ("Bosses#3"): self.__SelectType(arg)),},
			)
		},
		{
			'item' : self.CreateCategoryItem(localeInfo.WIKI_MONSTER, lambda arg = ("Monster#"): self.__EmptyFunction(arg)), 
			'children' : (
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_1_75, lambda arg = ("Monster#0"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_76_100, lambda arg = ("Monster#1"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_100, lambda arg = ("Monster#2"): self.__SelectType(arg)),},
			)
		},
		{
			'item' : self.CreateCategoryItem(localeInfo.WIKI_METINSTONE, lambda arg = ("Metinstone#"): self.__EmptyFunction(arg)), 
			'children' : (
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_1_75, lambda arg = ("Metinstone#0"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_76_100, lambda arg = ("Metinstone#1"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_100, lambda arg = ("Metinstone#2"): self.__SelectType(arg)),},
			)
		},
		{
			'item' : self.CreateCategoryItem(localeInfo.WIKI_GUIDES, lambda arg = ("System#"): self.__EmptyFunction(arg)), 
			'children' : (
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_BASIC, lambda arg = ("System#0"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_ATTR_RARE, lambda arg = ("System#1"): self.__SelectType(arg)),},
				{'item' : self.CreateCategorySubItem(localeInfo.WIKI_COSTUME, lambda arg = ("System#2"): self.__SelectType(arg)),},
			)
		},
		]

		self.children["listBoxCube"].AppendItemList(listBoxCubeItems)

	def __EmptyFunction(self, arg):
		pass

	def LoadData(self, arg):
		self.__SelectType(arg)

	def __SelectCharacters(self, buttonIndex):
		for j in xrange(4):
			if self.children["characterIndex"] == buttonIndex:
				self.children["job_%d_characterBtn"%j].Down()
			else:
				self.children["job_%d_characterBtn"%j].SetUp()
		self.children["characterIndex"]=buttonIndex

		self.__SelectType(self.selectArg, True)

	def __ClickRadioButton(self, buttonList, buttonIndex):
		try:
			btn=buttonList[buttonIndex]
		except IndexError:
			return
		for eachButton in buttonList:
			eachButton.SetUp()
		btn.Down()

	def SetCharacterImagesStatus(self, flag):
		for j in xrange(4):
			if not flag:
				self.children["job_%d_characterBtn"%j].Hide()
			else:
				self.children["job_%d_characterBtn"%j].Show()
				if self.children["characterIndex"] == j:
					self.children["job_%d_characterBtn"%j].Down()
				else:
					self.children["job_%d_characterBtn"%j].SetUp()

	def ClearResultListbox(self, argList, isSpecial = False):
		self.AIAppendAlgoritm = None
		try:
			resultpageListbox = self.children["resultpageListbox"]
			resultpageListboxScrollbar = self.children["resultpageListboxScrollbar"]

			resultpageListbox.RemoveAllItems()
			resultpageListbox.Show()
			resultpageListboxScrollbar.Hide()

			if len(argList) == 0 or argList[0] == "":
				return
	
			imageFile = WikiUI.GetResultPageImage(argList)
			if imageFile:
				self.children["resultpagebtn"].LoadImage(imageFile)
				self.children["resultpagebtn"].Show()

			if argList[0] == "Equipment" and isSpecial == False:
				self.SetCharacterImagesStatus(True)
				resultpageListbox.SetPosition(155, 160)
				resultpageListbox.SetSize(540, 318)
				resultpageListboxScrollbar.SetPosition(699, 160)
				resultpageListboxScrollbar.SetScrollBarSize(resultpageListbox.GetHeight()-20)
			else:
				self.SetCharacterImagesStatus(False)
				resultpageListbox.SetPosition(158,109)
				resultpageListbox.SetSize(540, 360)
				resultpageListboxScrollbar.SetPosition(699, 109)
				resultpageListboxScrollbar.SetScrollBarSize(350)
		except:
			pass

	def __SelectType(self, arg, isCharacterBtn = False, isHistory = True):
		#try:
		if isCharacterBtn == False and self.selectArg == arg:
			return

		if isHistory:
			historySearch = self.children["historySearch"]
			historySearch.append(arg)
			self.children["currentIndex"] = len(historySearch)-1
			self.SetHistoryButtons()

		self.selectArg = arg
		argList = arg.split("#")
		self.ClearResultListbox(argList)

		AIAppendAlgoritm = WikiUI.AutoLoad()
		AIAppendAlgoritm.SetFlag("loadTime", AUTOLOAD_SPEED)

		if argList[0] == "Equipment":
			maxSize = wiki.GetCategorySize(self.children["characterIndex"],int(argList[1]))
			if maxSize == 0:
				return
			AIAppendAlgoritm.SetFlag("maxSize", maxSize-1)
			AIAppendAlgoritm.SetFlag("loadType","Equipment")
			AIAppendAlgoritm.SetFlag("characterIndex",self.children["characterIndex"])
			AIAppendAlgoritm.SetFlag("itemType",int(argList[1]))

		elif argList[0] == "Costume":
			maxSize = wiki.GetCostumeSize(int(argList[1]))
			if maxSize == 0:
				return
			AIAppendAlgoritm.SetFlag("maxSize", maxSize-1)
			AIAppendAlgoritm.SetFlag("loadType","Costume")
			AIAppendAlgoritm.SetFlag("itemType",int(argList[1]))

		elif argList[0] == "Chests":
			maxSize = wiki.GetChestSize(int(argList[1]))
			if maxSize == 0:
				return
			AIAppendAlgoritm.SetFlag("maxSize", maxSize-1)
			AIAppendAlgoritm.SetFlag("loadType","Chests")
			AIAppendAlgoritm.SetFlag("itemType",int(argList[1]))
		elif argList[0] == "Bosses":
			maxSize = wiki.GetBossSize(int(argList[1]))
			if maxSize == 0:
				return
			AIAppendAlgoritm.SetFlag("maxSize", maxSize-1)
			AIAppendAlgoritm.SetFlag("loadType","Bosses")
			AIAppendAlgoritm.SetFlag("itemType",int(argList[1]))
			AIAppendAlgoritm.SetFlag("loadTime", AUTOLOAD_MONSTER_SPEED)

		elif argList[0] == "Monster":
			maxSize = wiki.GetMonsterSize(int(argList[1]))
			if maxSize == 0:
				return
			AIAppendAlgoritm.SetFlag("maxSize", maxSize-1)
			AIAppendAlgoritm.SetFlag("loadType","Monster")
			AIAppendAlgoritm.SetFlag("itemType",int(argList[1]))
			AIAppendAlgoritm.SetFlag("loadTime", AUTOLOAD_MONSTER_SPEED)

		elif argList[0] == "Metinstone":
			maxSize = wiki.GetStoneSize(int(argList[1]))
			if maxSize == 0:
				return
			AIAppendAlgoritm.SetFlag("maxSize", maxSize-1)
			AIAppendAlgoritm.SetFlag("loadType","Metinstone")
			AIAppendAlgoritm.SetFlag("itemType",int(argList[1]))

		elif WikiUI.IsArticleCategory(argList):
			if len(argList) == 3:
				event_item = ArticleGUI(argList[1]+"#"+argList[2])
			else:
				event_item = ArticleGUI(int(argList[1]))
			self.children["resultpageListbox"].AppendItem(event_item)
			event_item.LoadItemInfos()
			self.children["resultpageListboxScrollbar"].Hide()
			return

		if AIAppendAlgoritm.GetFlag("loadTime") != 0:
			self.AIAppendAlgoritm = AIAppendAlgoritm
		#except:
		#	dbg.TraceError("Saving crash from __SelectType")


	def ShowItemInfo(self, vnum, arg, isHistory = True):
		#try:
		if isHistory:
			historySearch = self.children["historySearch"]
			historySearch.append("NEW#%d#%d"%(vnum, arg))
			self.children["currentIndex"] = len(historySearch)-1
			self.SetHistoryButtons()

		if arg == 3:
			self.__SelectType(vnum)

		elif arg == 0:
			self.selectArg = "Equipment#0"
			self.ClearResultListbox(self.selectArg.split("#"), True)
			self.children["resultpageListboxScrollbar"].Hide()
			(vnum, isRefineItem) = WikiUI.getRealVnum(vnum)
			if isRefineItem:
				vnum += wiki.GetRefineMaxLevel(vnum)
			resultpageListbox = self.children["resultpageListbox"]
			if isRefineItem:
				new_refine_table = EquipmentItem(99, vnum)
				new_refine_table.LoadItemInfos()
				resultpageListbox.AppendItem(new_refine_table)
			item.SelectItem(vnum)
			if item.GetItemType() == item.ITEM_TYPE_GIFTBOX:
				item_table = MonsterItemSpecial(vnum, 3)
				item_table.LoadItemInfos()
				resultpageListbox.AppendItem(item_table)
			
				monster_statics = MonsterStatics(vnum, 3)
				monster_statics.LoadItemInfos()
				resultpageListbox.AppendItem(monster_statics)
			else:
				item_table = MonsterItemSpecial(vnum, 1)
				item_table.LoadItemInfos()
				resultpageListbox.AppendItem(item_table)
			resultpageListbox.Render(0)
			self.children["resultpageListboxScrollbar"].Hide()
			self.selectArg=""

		elif arg == 1:
			self.selectArg = "Monster#0"
			self.ClearResultListbox(self.selectArg.split("#"), True)
			self.children["resultpageListboxScrollbar"].Show()
			resultpageListbox = self.children["resultpageListbox"]

			monster_table = MonsterItemSpecial(vnum, 0)
			monster_table.LoadItemInfos()
			resultpageListbox.AppendItem(monster_table)

			monster_statics = MonsterStatics(vnum, 0)
			monster_statics.LoadItemInfos()
			resultpageListbox.AppendItem(monster_statics)

			resultpageListbox.Render(0)

			self.selectArg=""
		#except:
		#	dbg.TraceError("Saving crash from ShowItemInfo")

	def OnUpdate(self):
		self.CheckLoadProcess()

	def SetTimeForHide(self, isOpen):
		__ai = self.AIAppendAlgoritm
		if __ai != None:
			if isOpen:
				__ai.SetFlag("nexTime",app.GetTime()+0.15)
			else:
				__ai.SetFlag("nexTime",app.GetTime()+999999)

	def CheckLoadProcess(self):
		#try:
		__ai = self.AIAppendAlgoritm
		if __ai != None:
			if __ai.GetFlag("nexTime") > app.GetTime():
				return
			__ai.SetFlag("nexTime", app.GetTime()+__ai.GetFlag("loadTime"))
			loadType = __ai.GetFlag("loadType")
			listIndex = __ai.GetFlag("maxSize")
			if loadType == "Equipment":
				equipItemPointer = EquipmentItem(listIndex, wiki.GetCategoryData(__ai.GetFlag("characterIndex"), __ai.GetFlag("itemType"), listIndex))
				equipItemPointer.LoadItemInfos()
				equipItemPointer.sortIndex = listIndex
				self.children["resultpageListbox"].AppendItem(equipItemPointer, False)

			elif loadType == "Costume":
				createNewWindow = True
				ListBoxItems = self.children["resultpageListbox"].itemList
				if len(ListBoxItems) > 0:
					lastItem = ListBoxItems[len(ListBoxItems)-1]
					if lastItem.CanAddNewItem():
						lastItem.LoadItemInfos(wiki.GetCostumeData(__ai.GetFlag("itemType"), listIndex))
						createNewWindow = False

				if createNewWindow:
					equipItemPointer = SpecialClass(listIndex, 0)
					equipItemPointer.LoadItemInfos(wiki.GetCostumeData(__ai.GetFlag("itemType"), listIndex))
					equipItemPointer.sortIndex = listIndex
					self.children["resultpageListbox"].AppendItem(equipItemPointer, False)

			elif loadType == "Chests":
				(itemVnum, bossVnum) = wiki.GetChestData(__ai.GetFlag("itemType"), listIndex)
				if itemVnum == 0:
					return
				equipItemPointer = ListBoxItemSpecial(listIndex, itemVnum, bossVnum, 0)
				equipItemPointer.LoadItemInfos()
				equipItemPointer.sortIndex = listIndex
				self.children["resultpageListbox"].AppendItem(equipItemPointer, False)

			elif loadType == "Monster":
				mobVnum = wiki.GetMonsterData(__ai.GetFlag("itemType"), listIndex)
				if mobVnum == 0:
					return
				equipItemPointer = ListBoxItemSpecial(listIndex, mobVnum, 0, 1)
				equipItemPointer.LoadItemInfos()
				equipItemPointer.sortIndex = listIndex
				self.children["resultpageListbox"].AppendItem(equipItemPointer, False)

			elif loadType == "Bosses":
				mobVnum = wiki.GetBossData(__ai.GetFlag("itemType"), listIndex)
				if mobVnum == 0:
					return
				equipItemPointer = ListBoxItemSpecial(listIndex, mobVnum, 0, 1)
				equipItemPointer.LoadItemInfos()
				equipItemPointer.sortIndex = listIndex
				self.children["resultpageListbox"].AppendItem(equipItemPointer, False)

			elif loadType == "Metinstone":
				mobVnum = wiki.GetStoneData(__ai.GetFlag("itemType"), listIndex)
				if mobVnum == 0:
					return
				equipItemPointer = ListBoxItemSpecial(listIndex, mobVnum, 0, 1)
				equipItemPointer.LoadItemInfos()
				equipItemPointer.sortIndex = listIndex
				self.children["resultpageListbox"].AppendItem(equipItemPointer, False)

			self.SetPositionToSort(self.children["resultpageListbox"])
			__ai.SetFlag("maxSize", listIndex-1)

			if listIndex-1 < 0:
				self.AIAppendAlgoritm = None
				self.children["resultpageListbox"].CalculateScroll()
		#except:
		#	dbg.TraceError("Saving crash from OnUpdate")

	def get_key(self, data):
		return data.sortIndex

	def SetPositionToSort(self, listBox):
		itemList = listBox.itemList
		if len(itemList) > 1:
			itemList = sorted(itemList, key=self.get_key,reverse=SHOW_ITEM_LOWER_TO_BIG)
		y = 0
		for child in itemList:
			child.Hide()
			child.SetPosition(0, y, True)
			child.OnRender()
			child.Show()
			y+=child.GetHeight()

	def CreateCategoryItem(self, text, event, offset = 0):
		listboxItem = WikiUI.CategoryItem(text)
		listboxItem.SetVisible(True)
		listboxItem.SetEvent(event)
		listboxItem.SetOffset(offset)
		return listboxItem

	def CreateCategorySubItem(self, text, event, offset = 0):
		listboxItem = WikiUI.CategorySubItem(text)
		listboxItem.SetEvent(event)
		listboxItem.SetOffset(offset)
		return listboxItem
	def Open(self):
		self.Show()
		self.SetTimeForHide(True)
	def Close(self):
		self.SetTimeForHide(False)
		self.children["searchMobName"].KillFocus()
		self.children["searchItemName"].KillFocus()
		self.Hide()
	def OnPressExitKey(self):
		self.Close()
		return TRUE
	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

class EquipmentItem(WikiUI.DefaultWikiImage):
	class RefineItem(WikiUI.DefaultWikiWindow):
		def __del__(self):
			WikiUI.DefaultWikiWindow.__del__(self)
		def __init__(self):
			WikiUI.DefaultWikiWindow.__init__(self)
		def LoadData(self, refine, itemVnum, refineCount, refineData):
			tooltipImage = ui.ImageBox()
			tooltipImage.SetParent(self)
			tooltipImage.SetPosition(0,0)
			tooltipImage.SetSize(41, 19)
			tooltipImage.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,(itemVnum-wiki.GetRefineMaxLevel(itemVnum))+refine)
			tooltipImage.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
			tooltipImage.Show()
			self.children.append(tooltipImage)

			step_refine = ui.TextLine()
			step_refine.SetParent(self)
			step_refine.SetText("+%d"%refine)
			step_refine.SetHorizontalAlignCenter()
			step_refine.SetPosition(21,5)
			step_refine.Show()
			self.children.append(step_refine)

			step_price = ui.TextLine()
			step_price.SetParent(self)
			if SHOW_NEXT_ITEM_REFINE:
				step_price.SetText(localeInfo.MoneyFormat(refineData["cost"]).replace(".000","k"))
			else:
				if refine:
					step_price.SetText(localeInfo.MoneyFormat(refineData["cost"]).replace(".000","k"))
				else:
					step_price.SetText("-")
			step_price.SetHorizontalAlignCenter()
			priceYPos = [140, 140, 140, 180, 230, 275]
			step_price.SetPosition(21, priceYPos[refineCount]-21)
			step_price.Show()
			self.children.append(step_price)
			for i in xrange(refineCount):
				refineItem = refineData["item_vnum_%d"%i]
				needInsertIcon = refineItem != 0
				if needInsertIcon == True and SHOW_NEXT_ITEM_REFINE == False and refine == 0:
					needInsertIcon = False
				if needInsertIcon:
					item.SelectItem(refineItem)
					refineItemIcon = ui.ExpandedImageBox()
					refineItemIcon.SetParent(self)
					refineItemIcon.LoadImage(item.GetIconImageFileName())
					refineItemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,refineItem)
					refineItemIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
					refineItemIcon.SetEvent(ui.__mem_func__(self.OnClickItem),"mouse_click",0,refineItem)
					refineItemIcon.SetPosition(5,20+5+(i*(32+5+10)))
					refineItemIcon.Show()
					self.children.append(refineItemIcon)
					if refineData["item_count_%d"%i] > 0:
						if USE_ITEM_COUNT_NUMBER_LINE:
							refineItemCount = ui.NumberLine()
							refineItemCount.SetParent(self)
							refineItemCount.SetNumber(str(refineData["item_count_%d"%i]))
							refineItemCount.SetPosition(5+28,20+5+(i*(32+5+10))+32)
						else:
							refineItemCount = ui.TextLine()
							refineItemCount.SetParent(self)
							refineItemCount.SetText(str(refineData["item_count_%d"%i]))
							refineItemCount.SetPosition(5+28,20+5+(i*(32+5+10))+20)
						refineItemCount.Show()
						self.children.append(refineItemCount)
				else:
					emptyRefine = ui.TextLine()
					emptyRefine.SetParent(self)
					emptyRefine.AddFlag("not_pick")
					emptyRefine.SetPosition(self.GetWidth()/2,21+18+(i*44))
					emptyRefine.SetText("-")
					emptyRefine.Show()
					self.children.append(emptyRefine)
	def __del__(self):
		WikiUI.DefaultWikiImage.__del__(self)
	def Destroy(self):
		self.refineItems = []
		self.listIndex = 0
		self.itemVnum = 0
		self.refineCount = 0
		WikiUI.DefaultWikiImage.Destroy(self)

	def __init__(self, listIndex, itemVnum):
		WikiUI.DefaultWikiImage.__init__(self)
		self.Destroy()
		self.listIndex = listIndex
		self.itemVnum = itemVnum
		self.refineCount = 2
		self.refineLevel = wiki.GetRefineMaxLevel(itemVnum)
		for j in xrange(self.refineLevel+1):
			if item.SelectItemWiki((self.itemVnum-self.refineLevel)+j) == 1:
				argv = wiki.GetRefineItems(item.GetRefineSet())
				if argv != 0:
					self.InsertRefine(*argv)
					continue
			self.InsertRefine(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
		self.LoadImage(IMG_DIR+"slot/slot_%d.tga"%self.refineCount)
		self.children.append(self)
		# add scrollbar extra height!
		if self.refineLevel >= 11:
			self.SetSize(self.GetWidth(),self.GetHeight()+20)

	def LoadItemInfos(self):
		if self.IsLoaded == False:
			self.IsLoaded=True
			item.SelectItem(self.itemVnum-self.refineLevel)
			(firstLevel, secondLevel) = (0, 0)

			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)

				if app.ENABLE_YOHARA_SYSTEM and limitType == item.LIMIT_CONQUEROR_LEVEL and limitValue > 0:
					firstLevel = 120 + limitValue
					break

				if item.LIMIT_LEVEL == limitType and limitValue > 0:
					firstLevel = limitValue
					break

			item.SelectItem(self.itemVnum)

			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)

				if app.ENABLE_YOHARA_SYSTEM and limitType == item.LIMIT_CONQUEROR_LEVEL and limitValue > 0:
					secondLevel = 120 + limitValue
					break

				if item.LIMIT_LEVEL == limitType and limitValue > 0:
					secondLevel = limitValue
					break

			itemName = ui.TextLine()
			itemName.SetParent(self)
			item_name = item.GetItemName()
			if item_name.find("+") != -1:
				item_name = item_name[:item_name.find("+")]
			itemName.SetText(item_name)
			itemName.SetPosition(5, 5)
			itemName.Show()
			self.children.append(itemName)

			itemLevel = ui.TextLine()
			itemLevel.SetParent(self)

			if secondLevel != firstLevel:
				if app.ENABLE_YOHARA_SYSTEM:
					if firstLevel > 120:
						itemLevel.SetText(localeInfo.WIKI_LEVEL_TEXT2_CHAMPION % (firstLevel - 120, secondLevel - 120, 0, self.refineLevel))
					else:
						itemLevel.SetText(localeInfo.WIKI_LEVEL_TEXT2 % (firstLevel, secondLevel, 0, self.refineLevel))
				else:
					itemLevel.SetText(localeInfo.WIKI_LEVEL_TEXT2 % (firstLevel, secondLevel, 0, self.refineLevel))
			else:
				if app.ENABLE_YOHARA_SYSTEM:
					if firstLevel > 120:
						itemLevel.SetText(localeInfo.WIKI_LEVEL_TEXT_CHAMPION % (firstLevel - 120, 0, self.refineLevel))
					else:
						itemLevel.SetText(localeInfo.WIKI_LEVEL_TEXT % (firstLevel, 0, self.refineLevel))
				else:
					itemLevel.SetText(localeInfo.WIKI_LEVEL_TEXT % (firstLevel, 0, self.refineLevel))

			itemLevel.SetPosition(355,5)
			itemLevel.Show()
			self.children.append(itemLevel)

			itemIcon = ui.ExpandedImageBox()
			itemIcon.SetParent(self)
			if item.GetIconImageFileName().find("gr2") == -1:
				itemIcon.LoadImage(item.GetIconImageFileName())
			else:
				itemIcon.LoadImage("icon/item/27995.tga")
			itemLevelCoordinates = [ [0,0],[0,0],[10,55],[10,70],[10,80],[10,115]]
			itemIcon.SetPosition(itemLevelCoordinates[self.refineCount][0],itemLevelCoordinates[self.refineCount][1])
			if self.listIndex == 99:
				itemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,self.itemVnum-self.refineLevel)
			else:
				itemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,self.itemVnum)
				itemIcon.SetEvent(ui.__mem_func__(self.OnClickItem),"mouse_click", 0)
			itemIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
			itemIcon.Show()
			self.children.append(itemIcon)

			upgradeItem = ui.TextLine()
			upgradeItem.SetParent(self)
			upgradeItem.SetText(localeInfo.WIKI_UPGRADE_COSTS)
			upgradeItem.SetPosition(50,24)
			upgradeItem.Show()
			self.children.append(upgradeItem)

			yangCost = ui.TextLine()
			yangCost.SetParent(self)
			yangCost.SetText(localeInfo.WIKI_YANG_COSTS)
			list = [140, 140, 140, 180, 230, 275]
			yangCost.SetPosition(58,list[self.refineCount])
			yangCost.Show()
			self.children.append(yangCost)

			listboxSizes = [ [0,0],[0,0],[411,136],[411,180],[411,228],[411,276]]

			refinedVnum = item.GetRefinedVnum()
			if refinedVnum != 0:
				item.SelectItem(refinedVnum)
				nextItemIcon = ui.ExpandedImageBox()
				nextItemIcon.SetParent(self)
				if item.GetIconImageFileName().find("gr2") == -1:
					nextItemIcon.LoadImage(item.GetIconImageFileName())
				else:
					nextItemIcon.LoadImage("icon/item/27995.tga")
				nextItemIcon.SetPosition(75,itemLevelCoordinates[self.refineCount][1])
				nextItemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,refinedVnum)
				nextItemIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
				nextItemIcon.SetEvent(ui.__mem_func__(self.OnClickItem),"mouse_click",refinedVnum)
				nextItemIcon.Show()
				self.children.append(nextItemIcon)

			Listbox = WikiUI.ListBoxEx(True)
			Listbox.SetParent(self)
			Listbox.SetSize(listboxSizes[self.refineCount][0],listboxSizes[self.refineCount][1])
			Listbox.SetPosition(130,20)
			Listbox.SetItemStep(41)
			Listbox.SetItemSize(41,listboxSizes[self.refineCount][1])
			Listbox.SetViewItemCount(10)
			Listbox.SetOnMouseWhell(False)

			if self.refineLevel >= 11:
				scrollheightpos = [ 156, 156, 156, 200, 255, 296]
				scrollbar = WikiUI.HorizontalScrollBarNew()
				scrollbar.SetParent(self)
				scrollbar.SetPosition(0, scrollheightpos[self.refineCount])
				scrollbar.SetScrollBarSize(540)
				#scrollbar.SetScrollBarSize(10.0/float(self.refineLevel))
				scrollbar.itsNeedDoubleRender = True
				scrollbar.Show()
				Listbox.SetScrollBar(scrollbar)
				self.children.append(scrollbar)

			for i in xrange(self.refineLevel+1):
				refine_data = self.RefineItem()
				Listbox.AppendItem(refine_data)
				if SHOW_NEXT_ITEM_REFINE:
					refine_data.LoadData(i, self.itemVnum, self.refineCount, self.refineItems[i])
				else:
					if i == 0:
						refine_data.LoadData(i, self.itemVnum, self.refineCount, self.refineItems[i])
					else:
						refine_data.LoadData(i, self.itemVnum, self.refineCount, self.refineItems[i-1])

			Listbox.SetBasePos(0)
			Listbox.Show()
			self.Listbox = Listbox
			self.OnRender()
			self.Show()

	def OnClickItem(self, arg, vnum = 0):
		self.OverOutItem()
		parent = constInfo.GetMainParent()
		if parent != None:
			if vnum != 0:
				parent.ShowItemInfo(vnum+wiki.GetRefineMaxLevel(vnum),0)
			else:
				parent.ShowItemInfo(self.itemVnum,0)
	def InsertRefine(self, id,item_vnum_0,item_count_0,item_vnum_1,item_count_1,item_vnum_2,item_count_2,item_vnum_3,item_count_3,item_vnum_4,item_count_4,cost,prob,refine_count):
		self.refineItems.append(\
			{
				"id" : int(id),\
				"item_vnum_0" : int(item_vnum_0),\
				"item_count_0" : int(item_count_0),\
				"item_vnum_1" : int(item_vnum_1),\
				"item_count_1" : int(item_count_1),\
				"item_vnum_2" : int(item_vnum_2),\
				"item_count_2" : int(item_count_2),\
				"item_vnum_3" : int(item_vnum_3),\
				"item_count_3" : int(item_count_3),\
				"item_vnum_4" : int(item_vnum_4),\
				"item_count_4" : int(item_count_4),\
				"cost" : long(cost),\
				"prob" : long(prob),\
				"refine_count" : int(refine_count),\
			},
		)
		if refine_count > self.refineCount:
			self.refineCount = refine_count

class MonsterItemSpecial(WikiUI.DefaultWikiImage):
	def __del__(self):
		WikiUI.DefaultWikiImage.__del__(self)
	def Destroy(self):
		self.vnum = 0
		renderTarget.SetVisibility(self.renderIndex, False)
		renderTarget.ResetModel(self.renderIndex)
		WikiUI.DefaultWikiImage.Destroy(self)
	def __init__(self, vnum, type):
		WikiUI.DefaultWikiImage.__init__(self)
		self.vnum = vnum
		self.isType = type
		self.LoadImage(IMG_DIR+"slot/special_slot.tga")
		self.children.append(self)
	def LoadItemInfos(self):
		if self.IsLoaded == False:
			self.IsLoaded=True
			#self.renderIndex = app.GetRandom(900, 1000)
			self.renderIndex = renderTarget.GetFreeIndex(1000, 1000000)
			Listbox = None
			if self.isType == 0 or self.isType == 3:
				if self.isType == 0:
					renterTarget = ui.RenderTarget()
					renterTarget.SetParent(self)
					renterTarget.SetPosition(1,1)
					renterTarget.SetSize(187,163)
					renterTarget.SetRenderTarget(self.renderIndex)
					renterTarget.Show()
					self.children.append(renterTarget)
					renderTarget.SelectModel(self.renderIndex, self.vnum)
					renderTarget.SetVisibility(self.renderIndex, True)
				elif self.isType == 3:
					item.SelectItem(self.vnum)
					itemIcon = ui.ExpandedImageBox()
					itemIcon.SetParent(self)
					if item.GetIconImageFileName().find("gr2") == -1:
						itemIcon.LoadImage(item.GetIconImageFileName())
					else:
						itemIcon.LoadImage("icon/item/27995.tga")
					itemIcon.SetPosition(70,45)
					itemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,self.vnum)
					itemIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
					itemIcon.Show()
					self.children.append(itemIcon)

				Listbox = WikiUI.ListBoxGrid()
				Listbox.SetParent(self)
				Listbox.SetPosition(190, 25)
				Listbox.SetSize(350, 138)
				Listbox.AddRenderEvent(self.OnRender)

				dropList = ui.TextLine()
				dropList.SetParent(self)
				if self.isType == 0:
					dropList.SetText(localeInfo.WIKI_DROPLIST_INFO%nonplayer.GetMonsterName(self.vnum))
				elif self.isType == 3:
					dropList.SetText(localeInfo.WIKI_CONTENT_INFO%item.GetItemName())
				dropList.SetPosition(300,6)
				dropList.Show()
				self.children.append(dropList)

				monsterInfo = ui.TextLine()
				monsterInfo.SetParent(self)
				if self.isType == 0:
					monsterInfo.SetText(localeInfo.WIKI_STATICS_INFO%nonplayer.GetMonsterName(self.vnum))
				elif self.isType == 3:
					monsterInfo.SetText(localeInfo.WIKI_AVAIBLE_AT)
				monsterInfo.SetHorizontalAlignCenter()
				monsterInfo.SetPosition(245,170)
				monsterInfo.Show()
				self.children.append(monsterInfo)

				if self.isType == 0:
					sizeFunc = wiki.GetMobInfoSize
					getFunc = wiki.GetMobInfoData
				elif self.isType == 3:
					sizeFunc = wiki.GetSpecialInfoSize
					getFunc = wiki.GetSpecialInfoData

				grid = ui.Grid(width = 11, height = 50)
				for j in xrange(sizeFunc(self.vnum)):
					(vnum, count) = getFunc(self.vnum,j)
					if vnum == 0:
						continue
					item.SelectItem(vnum)
					(width, height) = item.GetItemSize()
					if width == 0 or height == 0:
						continue

					item_new = ui.ExpandedImageBox()
					item_new.SetParent(Listbox)
					if item.GetIconImageFileName().find("gr2") == -1:
						item_new.LoadImage(item.GetIconImageFileName())
					else:
						item_new.LoadImage("icon/item/27995.tga")
					item_new.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,vnum)
					item_new.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
					item_new.SetEvent(ui.__mem_func__(self.OnClickItem),"mouse_click",0,vnum)
					pos = grid.find_blank(width, height)
					grid.put(pos, width, height)
					(x, y) = WikiUI.calculatePos(pos, 10)
					item_new.SetPosition(x, y, True)
					item_new.itsNeedDoubleRender = True
					item_new.Show()
					self.children.append(item_new)
					Listbox.AppendItem(item_new)

					if count>1:
						itemNumberline = ui.NumberLine()
						itemNumberline.SetParent(Listbox)
						itemNumberline.SetNumber(str(count))
						itemNumberline.SetPosition(x+15,y+item_new.GetHeight()-10,True)
						itemNumberline.itsNeedDoubleRender = True
						itemNumberline.Show()
						Listbox.AppendItem(itemNumberline)
						self.children.append(itemNumberline)

				if Listbox.isNeedScrollBar():
					scrollBar = WikiUI.ScrollBarNew()
					scrollBar.SetParent(Listbox)
					scrollBar.SetPosition(Listbox.GetWidth()-10, 0)
					scrollBar.SetScrollBarSize(137)
					scrollBar.Hide()
					self.children.append(scrollBar)
					Listbox.SetScrollBar(scrollBar)
			else:
				item.SelectItem(self.vnum)
				if WikiUI.IsCanModelPreview(self.vnum):
					renterTarget = ui.RenderTarget()
					renterTarget.SetParent(self)
					renterTarget.SetPosition(1,1)
					renterTarget.SetSize(187,163)
					renterTarget.SetRenderTarget(self.renderIndex)
					renterTarget.Show()
					self.children.append(renterTarget)
					WikiUI.SetItemToModelPreview(self.renderIndex, self.vnum)

				itemIcon = ui.ExpandedImageBox()
				itemIcon.SetParent(self)
				if item.GetIconImageFileName().find("gr2") == -1:
					itemIcon.LoadImage(item.GetIconImageFileName())
				else:
					itemIcon.LoadImage("icon/item/27995.tga")

				if WikiUI.IsCanModelPreview(self.vnum):
					itemIcon.SetPosition(renterTarget.GetWidth()-itemIcon.GetWidth()-3,3)
				else:
					itemIcon.SetPosition(70,45)

				itemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,self.vnum)
				itemIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
				itemIcon.Show()
				self.children.append(itemIcon)

				avaible = ui.TextLine()
				avaible.SetParent(self)
				avaible.SetText(localeInfo.WIKI_AVAIBLE_AT)
				avaible.SetPosition(350,6)
				avaible.Show()
				self.children.append(avaible)

				Listbox = WikiUI.ListBoxGrid()
				Listbox.SetParent(self)
				Listbox.SetPosition(190, 25)
				Listbox.SetSize(350, 128)
				Listbox.AddRenderEvent(self.OnRender)

				item.SelectItem(60001)
				(itemVnum, isRefineItem) = WikiUI.getRealVnum(self.vnum)
				(x , y, isHave, unknownItem) = (5, 5, False, item.GetItemName())
	
				chest_list = wiki.GetItemDropFromChest(itemVnum, isRefineItem)
				for i, chestVnum in enumerate(chest_list, start=0):
					if isHave == False:
						isHave=True
						self.AppendTextLine(Listbox, "|cFF0080FF"+localeInfo.WIKI_CHESTS+":", x, y)
						y+=14
					item.SelectItem(chestVnum)
					self.AppendTextLine(Listbox, "|Eemoji/e_wiki|e "+item.GetItemName() if unknownItem != item.GetItemName() else "|Eemoji/e_wiki|e "+localeInfo.WIKI_UNKOWN_ITEM%chestVnum,x,y)
					y+=14

				if len(Listbox.itemList) > 0:
					y+=28

				isHave = False
				mob_vnums = wiki.GetItemDropFromMonster(itemVnum, isRefineItem)
				for i, mobVnum in enumerate(mob_vnums, start=0):
					if isHave == False:
						isHave=True
						self.AppendTextLine(Listbox, "|cFF0080FF"+localeInfo.WIKI_MONSTER+":",x,y)
						y+=14
					level = nonplayer.GetMonsterLevel(mobVnum)
					self.AppendTextLine(Listbox, "|Eemoji/e_wiki|e "+localeInfo.WIKI_UNKOWN_MOB%mobVnum  if level <= 0 else "|Eemoji/e_wiki|e "+nonplayer.GetMonsterName(mobVnum)+ " - Lv."+str(level), x, y)
					y+=14

				if Listbox.isNeedScrollBar():
					scrollBar = WikiUI.ScrollBarNew()
					scrollBar.SetParent(Listbox)
					scrollBar.SetPosition(Listbox.GetWidth()-10, 0)
					scrollBar.SetScrollBarSize(137)
					self.children.append(scrollBar)
					Listbox.SetScrollBar(scrollBar)

			if Listbox != None:
				if len(Listbox.itemList) > 0:
					self.Listbox = Listbox
					Listbox.SetBasePos(0, False)
					Listbox.Show()
			self.OnRender()
			self.Show()

	def AppendTextLine(self, Listbox, text, x, y):
		textLine = ui.TextLine()
		textLine.SetParent(Listbox)
		textLine.itsNeedDoubleRender = True
		textLine.SetText(text)
		textLine.SetPosition(x, y, True)
		textLine.Show()
		self.children.append(textLine)
		Listbox.AppendItem(textLine)

class MonsterStatics(WikiUI.DefaultWikiImage):
	def __del__(self):
		WikiUI.DefaultWikiImage.__del__(self)
	def Destroy(self):
		self.mobVnum = 0
		WikiUI.DefaultWikiImage.Destroy(self)
	def __init__(self, mobVnum, isType):
		WikiUI.DefaultWikiImage.__init__(self)
		self.Destroy()
		self.mobVnum = mobVnum
		self.isType = isType
		self.LoadImage(IMG_DIR+"slot/big_empty.tga")
		self.children.append(self)

	def LoadItemInfos(self):
		if self.IsLoaded == False:
			self.IsLoaded=True

			ListBox = WikiUI.ListBoxGrid()
			ListBox.SetParent(self)
			ListBox.SetSize(self.GetWidth(), self.GetHeight()-10)
			ListBox.AddRenderEvent(self.OnRender)

			if self.isType == 3:
				item.SelectItem(60001)
				(itemVnum, isRefineItem) = WikiUI.getRealVnum(self.mobVnum)
				(x , y, isHave, unknownItem) = (5, 5, False, item.GetItemName())
	
				chest_list = wiki.GetItemDropFromChest(itemVnum, isRefineItem)
				for i, chestVnum in enumerate(chest_list, start=0):
					if isHave == False:
						isHave=True
						self.AppendTextLine(ListBox, "|cFF0080FF"+localeInfo.WIKI_CHESTS+":", x, y)
						y+=14
					item.SelectItem(chestVnum)
					self.AppendTextLine(ListBox, "|Eemoji/e_wiki|e "+item.GetItemName() if unknownItem != item.GetItemName() else "|Eemoji/e_wiki|e "+localeInfo.WIKI_UNKOWN_ITEM%chestVnum,x,y)
					y+=14

				if len(ListBox.itemList) > 0:
					y+=28

				isHave = False
				mob_vnums = wiki.GetItemDropFromMonster(itemVnum, isRefineItem)
				for i, mobVnum in enumerate(mob_vnums, start=0):
					if isHave == False:
						isHave=True
						self.AppendTextLine(ListBox, "|cFF0080FF"+localeInfo.WIKI_MONSTER+":",x,y)
						y+=14
					level = nonplayer.GetMonsterLevel(mobVnum)
					self.AppendTextLine(ListBox, "|Eemoji/e_wiki|e "+localeInfo.WIKI_UNKOWN_MOB%mobVnum  if level <= 0 else "|Eemoji/e_wiki|e "+nonplayer.GetMonsterName(mobVnum)+ " - Lv."+str(level), x, y)
					y+=14
			elif self.isType == 0:
				self.AppendTextLine(ListBox, "|Eemoji/e_wiki|e "+localeInfo.WIKI_LEVEL_MOB_TEXT%nonplayer.GetMonsterLevel(self.mobVnum), 5, 3)
				RACE_FLAG_TO_NAME = {
					1 << 0 : localeInfo.TARGET_INFO_RACE_ANIMAL,
					1 << 1 : localeInfo.TARGET_INFO_RACE_UNDEAD,
					1 << 2 : localeInfo.TARGET_INFO_RACE_DEVIL,
					1 << 3 : localeInfo.TARGET_INFO_RACE_HUMAN,
					1 << 4 : localeInfo.TARGET_INFO_RACE_ORC,
					1 << 5 : localeInfo.TARGET_INFO_RACE_MILGYO,
				}
				SUB_RACE_FLAG_TO_NAME = {
					1 << 11 : localeInfo.TARGET_INFO_RACE_ELEC,
					1 << 12 : localeInfo.TARGET_INFO_RACE_FIRE,
					1 << 13 : localeInfo.TARGET_INFO_RACE_ICE,
					1 << 14 : localeInfo.TARGET_INFO_RACE_WIND,
					1 << 15 : localeInfo.TARGET_INFO_RACE_EARTH,
					1 << 16 : localeInfo.TARGET_INFO_RACE_DARK,
					1 << 17 : localeInfo.TARGET_INFO_RACE_ZODIAC,
				}
				mainrace = ""
				subrace = ""
				dwRaceFlag = nonplayer.GetMonsterRaceFlag(self.mobVnum)
				for i in xrange(18):
					curFlag = 1 << i
					if constInfo.IS_SET(dwRaceFlag, curFlag):
						if RACE_FLAG_TO_NAME.has_key(curFlag):
							mainrace += RACE_FLAG_TO_NAME[curFlag] + ", "
						elif SUB_RACE_FLAG_TO_NAME.has_key(curFlag):
							subrace += SUB_RACE_FLAG_TO_NAME[curFlag] + ", "
				if nonplayer.IsMonsterStone(self.mobVnum):
					mainrace += localeInfo.TARGET_INFO_RACE_METIN + ", "
				if mainrace == "":
					mainrace = localeInfo.TARGET_INFO_NO_RACE
				else:
					mainrace = mainrace[:-2]
				if subrace == "":
					subrace = localeInfo.TARGET_INFO_NO_RACE
				else:
					subrace = subrace[:-2]
				self.AppendTextLine(ListBox, "|Eemoji/e_wiki|e "+localeInfo.WIKI_STATICS_INFO_TYPE%(mainrace,subrace), 5, 3*(7*1))
				(mindmg, maxdmg) = nonplayer.GetMonsterDamage(self.mobVnum)
				self.AppendTextLine(ListBox, "|Eemoji/e_wiki|e "+localeInfo.WIKI_STATICS_INFO_DMG%(mindmg,maxdmg,nonplayer.GetMonsterMaxHP(self.mobVnum)), 5, 3*(7*2))
				(minyang, maxyang) = nonplayer.GetMonsterPrice(self.mobVnum)
				exp = nonplayer.GetMonsterExp(self.mobVnum)
				self.AppendTextLine(ListBox, "|Eemoji/e_wiki|e "+localeInfo.WIKI_STATICS_INFO_YNG%(minyang,maxyang,exp), 5, 3*(7*3))
				self.AppendTextLine(ListBox, "|Eemoji/e_wiki|e "+localeInfo.WIKI_STATICS_INFO_DEFENSES, 5, 3*(7*4))
				resists = {
					nonplayer.MOB_RESIST_SWORD : localeInfo.TARGET_INFO_RESIST_SWORD,
					nonplayer.MOB_RESIST_TWOHAND : localeInfo.TARGET_INFO_RESIST_TWOHAND,
					nonplayer.MOB_RESIST_DAGGER : localeInfo.TARGET_INFO_RESIST_DAGGER,
					nonplayer.MOB_RESIST_BELL : localeInfo.TARGET_INFO_RESIST_BELL,
					nonplayer.MOB_RESIST_FAN : localeInfo.TARGET_INFO_RESIST_FAN,
					nonplayer.MOB_RESIST_BOW : localeInfo.TARGET_INFO_RESIST_BOW,
					nonplayer.MOB_RESIST_MAGIC : localeInfo.TARGET_INFO_RESIST_MAGIC,
				}

				c = 0
				for resist, label in resists.items():
					resistText = label % nonplayer.GetMonsterResist(self.mobVnum, resist)
					self.AppendTextLine(ListBox, "|Eemoji/e_wiki|e %s"%resistText, 5+8, 3*(7*(5+c)))
					c+=1

			if ListBox.isNeedScrollBar():
				scrollBar = WikiUI.ScrollBarNew()
				scrollBar.SetParent(ListBox)
				scrollBar.SetPosition(ListBox.GetWidth()-10,5)
				scrollBar.SetScrollBarSize(150)
				scrollBar.Show()
				ListBox.SetScrollBar(scrollBar)
				self.children.append(scrollBar)

			self.OnRender()
			ListBox.Show()
			self.Listbox = ListBox
			self.Show()
	
	def AppendTextLine(self, Listbox, text, x, y):
		textLine = ui.TextLine()
		textLine.SetParent(Listbox)
		textLine.itsNeedDoubleRender = True
		textLine.SetText(text)
		textLine.SetPosition(x, y, True)
		textLine.Show()
		self.children.append(textLine)
		Listbox.AppendItem(textLine)

class ListBoxItemSpecial(WikiUI.DefaultWikiImage):
	def __del__(self):
		WikiUI.DefaultWikiImage.__del__(self)
	def Destroy(self):
		self.renterTarget = None
		WikiUI.DefaultWikiImage.Destroy(self)
	def __init__(self, index, itemVnum, mobVnum, type):
		WikiUI.DefaultWikiImage.__init__(self)
		self.listIndex = index
		self.itemVnum = itemVnum
		self.mobVnum = mobVnum
		self.isType = type

		self.LoadImage(IMG_DIR+"slot/slot.tga")
		self.children.append(self)

	def LoadItemInfos(self):
		if self.IsLoaded == False:
			self.IsLoaded=True

			name = "-"
			if self.isType == 0:
				item.SelectItem(self.itemVnum)
				name = nonplayer.GetMonsterName(self.mobVnum)
			else:
				name = nonplayer.GetMonsterName(self.itemVnum)

			itemName = ui.TextLine()
			itemName.SetParent(self)
			if self.isType == 0:
				itemName.SetText(localeInfo.WIKI_CONTENT_INFO%item.GetItemName())
			else:
				itemName.SetText(localeInfo.WIKI_DROPLIST_INFO%name)
				itemName.SetText(itemName.GetText()+" - Level %d"%nonplayer.GetMonsterLevel(self.itemVnum))
				itemName.SetText(itemName.GetText()+" - Mob Vnum %d"%self.itemVnum) # test
			itemName.SetPosition(270, 5)
			itemName.SetHorizontalAlignCenter()
			itemName.Show()
			self.children.append(itemName)

			origin = ui.TextLine()
			origin.SetParent(self)
			origin.SetText(localeInfo.WIKI_ORIGIN)
			origin.SetPosition(480,5)
			origin.Show()
			self.children.append(origin)

			bossVnum = ui.TextLine()
			bossVnum.SetParent(self)
			bossVnum.SetHorizontalAlignCenter()
			bossVnum.SetPosition(490,45)
			if self.isType == 0 and self.mobVnum != 0:
				bossVnum.SetText(name)
			else:
				bossVnum.SetText("-")
			bossVnum.Show()
			self.children.append(bossVnum)

			if self.isType == 0:
				itemIcon = ui.ExpandedImageBox()
				itemIcon.SetParent(self)
				if item.GetIconImageFileName().find("gr2") == -1:
					itemIcon.LoadImage(item.GetIconImageFileName())
				else:
					itemIcon.LoadImage("icon/item/27995.tga")
				itemIcon.SetPosition(10,25)
				itemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,self.itemVnum)
				itemIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
				itemIcon.SetEvent(ui.__mem_func__(self.OnClickItem),"mouse_click",0,self.itemVnum)
				itemIcon.Show()
				self.children.append(itemIcon)
			else:
				renterTarget = ui.RenderTarget()
				renterTarget.SetParent(self)
				renterTarget.SetPosition(1,1)
				renterTarget.SetSize(47,87)
				renterTarget.SetRenderTarget(20+self.listIndex)
				renterTarget.Show()
				renterTarget.SetEvent(ui.__mem_func__(self.OnClickItem),"mouse_click",1,self.itemVnum)
				self.children.append(renterTarget)

				renderTarget.SelectModel(20+self.listIndex, self.itemVnum)
				renderTarget.SetVisibility(20+self.listIndex, True)

			whileSize = 0
			if self.isType == 0:
				whileSize = wiki.GetSpecialInfoSize(self.itemVnum)
			else:
				whileSize = wiki.GetMobInfoSize(self.itemVnum)

			if whileSize != 0:
				Listbox = WikiUI.ListBoxGrid()
				Listbox.SetParent(self)
				Listbox.SetPosition(48,22)
				Listbox.SetSize(403, 65)
				Listbox.AddRenderEvent(self.OnRender)
				Listbox.Show()

				gridCalculate = ui.Grid(width = 12, height = 50)

				screenSize = 0
				for j in xrange(whileSize):
					vnum, count = 0,0
					if self.isType == 0:
						(vnum,count) = wiki.GetSpecialInfoData(self.itemVnum,j)
					else:
						(vnum,count) = wiki.GetMobInfoData(self.itemVnum,j)
					if vnum == 0:
						continue
					item.SelectItem(vnum)
					(width,height) = item.GetItemSize()
					if width == 0 or height == 0:
						continue
					pos = gridCalculate.find_blank(width, height)
					gridCalculate.put(pos,width,height)
					(x, y) = WikiUI.calculatePos(pos, 11)

					item_new = ui.ExpandedImageBox()
					item_new.SetParent(Listbox)
					if item.GetIconImageFileName().find("gr2") == -1:
						item_new.LoadImage(item.GetIconImageFileName())
					else:
						item_new.LoadImage("icon/item/27995.tga")
					item_new.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,vnum)
					item_new.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
					item_new.SetEvent(ui.__mem_func__(self.OnClickItem),"mouse_click",0,vnum)
					item_new.SetPosition(x, y, True)
					if y+item_new.GetHeight() > screenSize:
						screenSize = y+item_new.GetHeight()
					item_new.itsNeedDoubleRender = True
					item_new.Show()
					Listbox.AppendItem(item_new)
					self.children.append(item_new)

					if count>1:
						if USE_ITEM_COUNT_NUMBER_LINE:
							itemNumberline = ui.NumberLine()
						else:
							itemNumberline = ui.TextLine()
						itemNumberline.SetParent(Listbox)
						if USE_ITEM_COUNT_NUMBER_LINE:
							itemNumberline.SetNumber(str(count))
							itemNumberline.SetPosition(x+15,y+item_new.GetHeight()-10,True)
						else:
							itemNumberline.SetText(str(count))
							itemNumberline.SetPosition(x+item_new.GetWidth()-5,y+item_new.GetHeight()-10,True)
						itemNumberline.itsNeedDoubleRender = True
						itemNumberline.Show()
						Listbox.AppendItem(itemNumberline)
						self.children.append(itemNumberline)

				if Listbox.isNeedScrollBar():
					scrollbar = WikiUI.ScrollBarNew()
					scrollbar.SetParent(self)
					scrollbar.SetPosition(443, 23)
					scrollbar.SetScrollBarSize(63)
					scrollbar.itsNeedDoubleRender = True
					scrollbar.Show()
					self.children.append(scrollbar)
					Listbox.SetScrollBar(scrollbar)

				self.Listbox = Listbox
			self.OnRender()
			self.Show()

class ArticleGUI(WikiUI.DefaultWikiImage):
	def __del__(self):
		WikiUI.DefaultWikiImage.__del__(self)
	def Destroy(self):
		self.index = 0
		self.scrollPos = 0.0
		WikiUI.DefaultWikiImage.Destroy(self)
	def __init__(self, index):
		WikiUI.DefaultWikiImage.__init__(self)
		if isinstance(index, str):
			if index.find("#") != -1:
				indexList = index.split("#")
				if len(indexList) == 2:
					self.index = int(indexList[0])
					self.scrollPos = float(indexList[1])
		else:
			self.index = index
			self.scrollPos = 0.0
		self.LoadImage(IMG_DIR+"slot/big2_empty.tga")
		self.children.append(self)
	def LoadItemInfos(self):
		if self.IsLoaded == False:
			self.IsLoaded = True
			Listbox = WikiUI.ListBoxGrid()
			Listbox.SetParent(self)
			Listbox.SetPosition(0,0)
			Listbox.SetSize(self.GetWidth()-15, self.GetHeight()-15)
			Listbox.AddRenderEvent(self.OnRender)
			self.ReadArticle(Listbox, self.index)
			self.CheckScrollBarNeed(Listbox)
			Listbox.Show()
			self.Listbox = Listbox
			self.OnRender()
			self.Show()
	def GetFileName(self, index):
		file_dict= {
			0 : "wiki/basic_guide.txt",
			1 : "wiki/bonus67.txt",
			2 : "wiki/costume.txt",
		}
		if file_dict.has_key(index):
			return app.GetLocalePath()+"/"+file_dict[index]
		return ""
	def ParseToken(self, data):
		data = data.replace(chr(10), "").replace(chr(13), "")
		if not (len(data) and data[0] == "["):
			return (False, {}, data)
		fnd = data.find("]")
		if fnd <= 0:
			return (False, {}, data)
		content = data[1:fnd]
		data = data[fnd+1:]
		content = content.split(";")
		container = {}
		for i in content:
			i = i.strip()
			splt = i.split("=")
			if len(splt) == 1:
				container[splt[0].lower().strip()] = True
			else:
				container[splt[0].lower().strip()] = splt[1].lower().strip()
		return (True, container, data)
	def GetColorFromString(self, strCol):
		retData = []
		dNum = 4
		hCol = long(strCol, 16)
		if hCol <= 0xFFFFFF:
			retData.append(1.0)
			dNum = 3
		for i in xrange(dNum):
			retData.append(float((hCol >> (8 * i)) & 0xFF) / 255.0)
		retData.reverse()
		return retData
	def DirectionEvent(self, emptyArg, type, index, pos):
		parent = constInfo.GetMainParent()
		if parent != None:
			if "item" == type:
				parent.ShowItemInfo(index,0)
			elif "mob" == type:
				parent.ShowItemInfo(index,1)
			elif "article" == type:
				parent.ShowItemInfo("system#"+str(index)+"#"+str(pos),3)
	def ReadArticle(self, Listbox, index):
		fileName = self.GetFileName(index)
		if fileName == "":
			return
		try:
			lines = open(fileName, "r").readlines()
		except:
			pass
		_y = 15
		for i in lines:
			(ret, tokenMap, i) = self.ParseToken(i)
			if ret:
				if tokenMap.has_key("banner_img"):
					mainParent = constInfo.GetMainParent()
					if mainParent != None:
						resultpagebtn = mainParent.children["resultpagebtn"]
						resultpagebtn.LoadImage(tokenMap["banner_img"])
						resultpagebtn.Show()
					tokenMap.pop("banner_img")

				if tokenMap.has_key("img"):
					cimg = ui.ExpandedImageBox()
					cimg.SetParent(Listbox)
					cimg.AddFlag("attach")
					cimg.AddFlag("not_pick")
					cimg.LoadImage(tokenMap["img"])
					cimg.Show()
					tokenMap.pop("img")
					x = 0
					if tokenMap.has_key("x"):
						x = int(tokenMap["x"])
						tokenMap.pop("x")
					y = 0
					if tokenMap.has_key("y"):
						y = int(tokenMap["y"])
						tokenMap.pop("y")
					if tokenMap.has_key("center_align"):
						cimg.SetPosition(Listbox.GetWidth() / 2 - cimg.GetWidth() / 2, y, True)
						tokenMap.pop("center_align")
					elif tokenMap.has_key("right_align"):
						cimg.SetPosition(Listbox.GetWidth() - cimg.GetWidth(), y, True)
						tokenMap.pop("right_align")
					else:
						cimg.SetPosition(x, y, True)
					self.children.append(cimg)
					Listbox.AppendItem(cimg)

				if tokenMap.has_key("item"):
					itemVnum = int(tokenMap["item"])
					tokenMap.pop("item")
					item.SelectItem(itemVnum)
					cimg = ui.ExpandedImageBox()
					cimg.SetParent(Listbox)
					if item.GetIconImageFileName().find("gr2") == -1:
						cimg.LoadImage(item.GetIconImageFileName())
					else:
						cimg.LoadImage("icon/item/27995.tga")
					cimg.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,itemVnum)
					cimg.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
					cimg.SetEvent(ui.__mem_func__(self.OnClickItem),"mouse_click",0,itemVnum)
					cimg.Show()
					x = 0
					if tokenMap.has_key("x"):
						x = int(tokenMap["x"])
						tokenMap.pop("x")
					y = 0
					if tokenMap.has_key("y"):
						y = int(tokenMap["y"])
						tokenMap.pop("y")
					if tokenMap.has_key("center_align"):
						cimg.SetPosition(Listbox.GetWidth() / 2 - cimg.GetWidth() / 2, y, True)
						tokenMap.pop("center_align")
					elif tokenMap.has_key("right_align"):
						cimg.SetPosition(Listbox.GetWidth() - cimg.GetWidth(), y, True)
						tokenMap.pop("right_align")
					else:
						cimg.SetPosition(x, y, True)
					self.children.append(cimg)
					Listbox.AppendItem(cimg)

				if tokenMap.has_key("link"):
					link = tokenMap["link"]
					tokenMap.pop("link")

					if tokenMap.has_key("linkindex"):
						linkindex = int(tokenMap["linkindex"])
						tokenMap.pop("linkindex")
					else:
						linkindex = 0

					if tokenMap.has_key("linkpos"):
						linkpos = float(tokenMap["linkpos"])
						tokenMap.pop("linkpos")
					else:
						linkpos = 0

					if tokenMap.has_key("linktext"):
						linkText = WikiUI.GetArgToString(tokenMap["linktext"])
						tokenMap.pop("linktext")
					else:
						linkText = ""

					tmp = WikiUI.TextlineLink()
					tmp.SetParent(Listbox)
					if tokenMap.has_key("font_size"):
						splt = localeInfo.UI_DEF_FONT.split(":")
						tmp.SetFontName(splt[0]+":"+tokenMap["font_size"])
						tokenMap.pop("font_size")
					else:
						tmp.SetFontName(localeInfo.UI_DEF_FONT)
					tmp.SetText(WikiUI.GetArgToString(linkText), 1.2)
					tmp.Show()
					tmp.SetSize(*tmp.GetTextSize())
					if tokenMap.has_key("color"):
						fontColor = self.GetColorFromString(tokenMap["color"])
						tmp.SetColor(grp.GenerateColor(fontColor[0], fontColor[1], fontColor[2], fontColor[3]), fontColor[0], fontColor[1], fontColor[2])
						tokenMap.pop("color")
					tmp.SetMouseLeftButtonDownEvent(ui.__mem_func__(self.DirectionEvent), "",link, linkindex, linkpos)
					tmp.linkIcon.SetMouseLeftButtonDownEvent(ui.__mem_func__(self.DirectionEvent),"", link, linkindex, linkpos)
					if tokenMap.has_key("y"):
						y = int(tokenMap["y"])
						tokenMap.pop("y")
					if tokenMap.has_key("center_align"):
						tmp.SetPosition(Listbox.GetWidth() / 2 - tmp.GetWidth() / 2, y, True)
						tokenMap.pop("center_align")
					elif tokenMap.has_key("right_align"):
						tmp.SetPosition(Listbox.GetWidth() - tmp.GetWidth(), y, True)
						tokenMap.pop("right_align")
					else:
						tmp.SetPosition(x, y, True)
					self.children.append(tmp)
					Listbox.AppendItem(tmp)

				if tokenMap.has_key("rendertarget"):
					mobVnum = int(tokenMap["rendertarget"])
					tokenMap.pop("rendertarget")

					(width, height) = (47, 87)

					if tokenMap.has_key("width"):
						width = int(tokenMap["width"])
						tokenMap.pop("width")
					
					if tokenMap.has_key("height"):
						height = int(tokenMap["height"])
						tokenMap.pop("height")

					#targetIndex = app.GetRandom(100,1000)
					targetIndex = renderTarget.GetFreeIndex(1000, 1000000)
					tmp = ui.RenderTarget()
					tmp.SetParent(Listbox)
					tmp.SetSize(width, height)
					tmp.SetRenderTarget(targetIndex)
					tmp.SetEvent(ui.__mem_func__(self.DirectionEvent),"mouse_click", "mob", mobVnum, 0)
					tmp.Show()

					renderTarget.SelectModel(targetIndex, mobVnum)
					renderTarget.SetVisibility(targetIndex, True)
					if tokenMap.has_key("y"):
						y = int(tokenMap["y"])
						tokenMap.pop("y")
					if tokenMap.has_key("center_align"):
						tmp.SetPosition(Listbox.GetWidth() / 2 - tmp.GetWidth() / 2, y, True)
						tokenMap.pop("center_align")
					elif tokenMap.has_key("right_align"):
						tmp.SetPosition(Listbox.GetWidth() - tmp.GetWidth(), y, True)
						tokenMap.pop("right_align")
					else:
						tmp.SetPosition(x, y, True)
					self.children.append(tmp)
					Listbox.AppendItem(tmp)

			if ret and not len(i):
				continue

			i = i.replace("*", "|Eemoji/e_wiki|e")
			tmp = ui.TextLine()
			tmp.SetParent(Listbox)
			if tokenMap.has_key("font_size"):
				splt = localeInfo.UI_DEF_FONT.split(":")
				tmp.SetFontName(splt[0]+":"+tokenMap["font_size"])
				tokenMap.pop("font_size")
			else:
				tmp.SetFontName(localeInfo.UI_DEF_FONT)
			tmp.SetText(WikiUI.GetArgToString(i))
			tmp.Show()
			tmp.SetSize(*tmp.GetTextSize())

			if tokenMap.has_key("color"):
				fontColor = self.GetColorFromString(tokenMap["color"])
				tmp.SetPackedFontColor(grp.GenerateColor(fontColor[0], fontColor[1], fontColor[2], fontColor[3]))
				tokenMap.pop("color")

			tmp.SetPosition(5, _y, True)
			_y+=tmp.GetHeight()

			if tokenMap.has_key("center_align"):
				tmp.SetPosition(Listbox.GetWidth() / 2 - tmp.GetWidth() / 2, tmp.GetLocalPosition()[1], True)
				tokenMap.pop("center_align")
			elif tokenMap.has_key("right_align"):
				tmp.SetPosition(Listbox.GetWidth() - tmp.GetWidth(), tmp.GetLocalPosition()[1], True)
				tokenMap.pop("right_align")
			elif tokenMap.has_key("x_padding"):
				tmp.SetPosition(int(tokenMap["x_padding"]), tmp.GetLocalPosition()[1], True)
				tokenMap.pop("x_padding")

			tmp.Show()
			self.children.append(tmp)
			Listbox.AppendItem(tmp)

	def CheckScrollBarNeed(self, Listbox):
		if Listbox.isNeedScrollBar():
			scrollBar = WikiUI.ScrollBarNew()
			scrollBar.SetParent(self)
			scrollBar.SetPosition(self.GetWidth()-15+8,2)
			scrollBar.SetScrollBarSize(337)
			scrollBar.Show()
			self.children.append(scrollBar)
			Listbox.SetScrollBar(scrollBar)
			scrollBar.SetPos(self.scrollPos)
			Listbox.OnScroll()

class SpecialClass(WikiUI.DefaultWikiWindow):
	def __del__(self):
		WikiUI.DefaultWikiWindow.__del__(self)
	def Destroy(self):
		self.vnumList = []
		WikiUI.DefaultWikiWindow.Destroy(self)
	def __init__(self, listIndex, isMonster):
		WikiUI.DefaultWikiWindow.__init__(self)
		self.Destroy()
		self.isType = isMonster
		self.listIndex = listIndex
		self.SetSize(540, 147)
	def CanAddNewItem(self):
		return len(self.vnumList) < 4

	def LoadItemInfos(self, data = -1):
		if data == -1:
			return
		xPos = len(self.vnumList) * (127+9)

		bg = ui.ExpandedImageBox()
		bg.SetParent(self)
		bg.LoadImage(IMG_DIR+"slot/special_single.tga")
		bg.SetPosition(xPos, 0)
		bg.Show()
		self.children.append(bg)

		item.SelectItem(data)

		itemIcon = ui.ExpandedImageBox()
		itemIcon.SetParent(bg)
		if item.GetIconImageFileName().find("gr2") == -1:
			itemIcon.LoadImage(item.GetIconImageFileName())
		else:
			itemIcon.LoadImage("icon/item/27995.tga")
		itemIcon.SetPosition((bg.GetWidth()/2)-(itemIcon.GetWidth()/2), ((bg.GetHeight()-20)/2)-(itemIcon.GetHeight()/2))
		itemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,data)
		itemIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
		itemIcon.SetEvent(ui.__mem_func__(self.OnClickItem),"mouse_click",0,data)
		itemIcon.Show()
		self.children.append(itemIcon)

		itemName = ui.TextLine()
		itemName.SetParent(bg)
		itemName.SetText(item.GetItemName())

		maxWidth = bg.GetWidth()-15
		newText = ""
		while True:
			if itemName.GetTextSize()[0] > maxWidth:
				newText = itemName.GetText()[:len(itemName.GetText())-2]+"..."
				itemName.SetText(itemName.GetText()[:len(itemName.GetText())-2])
				continue
			break
		if newText != "":
			itemName.SetText(newText)
		itemName.SetPosition(bg.GetWidth()/2, bg.GetHeight()-18)
		itemName.SetHorizontalAlignCenter()
		itemName.Show()
		self.children.append(itemName)

		self.vnumList.append(data)

		self.OnRender()
		self.Show()

	def OverOutItem(self):
		if self.renderIndex != 0:
			renderTarget.SetVisibility(self.renderIndex, False)
			renderTarget.ResetModel(self.renderIndex)
		WikiUI.DefaultWikiWindow.OverOutItem(self)

	def OverInItem(self, itemVnum):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			tooltipItem = interface.tooltipItem
			if tooltipItem:
				tooltipItem.ClearToolTip()

				self.renderIndex = renderTarget.GetFreeIndex(1000, 1000000)

				tooltipItem.toolTipWidth-= 35

				renterTarget = ui.RenderTarget()
				renterTarget.SetParent(tooltipItem)
				renterTarget.SetPosition(10, 5)
				renterTarget.SetSize(tooltipItem.toolTipWidth-20,150)
				renterTarget.SetRenderTarget(self.renderIndex)
				renterTarget.Show()
				tooltipItem.childrenList.append(renterTarget)
				tooltipItem.toolTipHeight+=150
				tooltipItem.ResizeToolTip()

				WikiUI.SetItemToModelPreview(self.renderIndex, itemVnum)
				tooltipItem.SetItemToolTipWiki(itemVnum)
