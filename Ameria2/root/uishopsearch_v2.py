# coding: latin_1

import app
import dbg
import ime
import item
import ikashop
import player
import grp
import wndMgr

import ui
import chat
import uicommon
import constInfo
import uitooltip
import localeinfo

import math
from operator import truediv
from _weakref import proxy

YANG_PER_CHEQUE = 100000000

#1001 -> uncomment when price avg is fixed & inc the width in the window script.

class ShopSearch(ui.ScriptWindow):
	SEARCH_FOUND_COLOR = grp.GenerateColor(0.8549, 0.8549, 0.8549, 1.0)
	SEARCH_NOTFOUND_COLOR = grp.GenerateColor(1.0, 0.2, 0.2, 1.0)

	SCROLL_SPEED = 7.7
	SCROLL_SPEED_ITEM = 20

	IMG_PATH = "d:/ymir work/ui/game/v2_shopsearch/"
	CATEGORY_IMG_PATH = IMG_PATH + "category_ico/"
	SLOT_IMG_PATH = IMG_PATH + "slot/"

	ROOT_PATH = "d:/ymir work/ui/game/newsearch/"
	SLOT_IMG_PATH_NEW = "d:/ymir work/ui/game/newsearch/right_rectangle/lines/"
	MAX_ITEM_PER_SEARCH_PAGE = ikashop.OFFLINESHOP_MAX_ITEM_PER_SEARCH_PAGE

	DIFFUSE_COLOR_IN = (200.0 / 255.0, 138.0 / 255.0, 109.0 / 255.0, 0.8)

	def GetFilterMinPrice(self):
		if self.priceMinInput.GetText() == "":
			return 0

		try:
			r = long(self.priceMinInput.GetText())
			return r
		except:
			return 0

	def GetFilterMaxPrice(self):
		if self.priceMaxInput.GetText() == "":
			return 0

		try:
			r = long(self.priceMaxInput.GetText())
			return r
		except:
			return 0

	def GetFilterMinLevel(self):
		if self.levelMinInput.GetText() == "":
			return 0

		try:
			r = int(self.levelMinInput.GetText())
			return r
		except:
			return 0

	def GetFilterMaxLevel(self):
		if self.levelMaxInput.GetText() == "":
			return 0

		try:
			r = int(self.levelMaxInput.GetText())
			return r
		except:
			return 0

	def GetFilterAntiFlag(self):
		r = 0

		if self.filterMaleBtn.IsDown():
			r |= item.ITEM_ANTIFLAG_MALE

		if self.filterFemaleBtn.IsDown():
			r |= item.ITEM_ANTIFLAG_FEMALE

		if self.filterWarriorBtn.IsDown():
			r |= item.ITEM_ANTIFLAG_WARRIOR

		if self.filterAssassinBtn.IsDown():
			r |= item.ITEM_ANTIFLAG_ASSASSIN

		if self.filterSuraBtn.IsDown():
			r |= item.ITEM_ANTIFLAG_SURA

		if self.filterShamanBtn.IsDown():
			r |= item.ITEM_ANTIFLAG_SHAMAN

		return r

	def OnOverIn(self, vnum, sockets, attrs):
		if not self.itemToolTip:
			self.itemToolTip = uitooltip.ItemToolTip()
			self.itemToolTip.SetShopSearch()

		self.itemToolTip.ClearToolTip()
		self.itemToolTip.AddItemData(vnum, sockets, attrs)
		self.itemToolTip.ShowToolTip()

	def OnOverInIco(self, arg, name):
		if not self.toolTip:
			self.toolTip = uitooltip.ToolTip()

		if arg == 5:
			text = localeinfo.V2_SHOP_SEARCH_SEND_PM % (name)
		elif arg == 6:
			text = localeinfo.V2_SHOP_SEARCH_OPEN_SHOP % (name)
		else:
			text = localeinfo.V2_SHOP_SEARCH_LOOK_SEARCH % (name)

		self.toolTip.ClearToolTip()
		self.toolTip.AutoAppendNewTextLine(text)
		self.toolTip.ShowToolTip()

	def OnOverOut(self):
		if self.itemToolTip:
			self.itemToolTip.HideToolTip()

		if self.toolTip:
			self.toolTip.HideToolTip()

	def OpenWhisper(self, name):
		if name == player.GetName():
			return

		self.interfaceInst.OpenWhisperDialogWithoutTarget()
		self.interfaceInst.RegisterTemporaryWhisperDialog(name)

	def OnSearchByOwner(self, name, pid):
		dif = constInfo.changeShopSearchTime - app.GetTime()
		if dif > 0:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
			return
		chat.AppendChat(1, "not filtered?")

		constInfo.changeShopSearchTime = app.GetTime() + 1

		if not self.isPlayerMatch:
			self.isPlayerMatch = True
			self.matchSearchImg.LoadImage(ShopSearch.IMG_PATH + "filter/exact_search_checked.png")
			self.nameInput.SetOverlayText(localeinfo.V2_SHOP_SEARCH_OVERLAY_PLAYER)

		self.ownerPid = pid
		chat.AppendChat(1, "OwnerSearch by pid ?? %d " % pid)
		self.nameInput.SetText("")
		self.nameInputHint.SetText("")

		self.nameInput.SetText(name)
		ime.SetCursorPosition(-1)

		self.__ResetFilter(False, True)

		if self.nameInput and self.nameInput.IsFocus():
			self.nameInput.KillFocus()

		ikashop.SendShopSearchOpen(0 if self.itemCatIdx == -1 else self.itemCatIdx,\
									self.itemSubCatIdx,\
									self.nameInput.GetText(),\
									self.isPerfectMatch,\
									self.resVnum,\
									self.isPlayerMatch,\
									self.ownerPid,\
									self.GetFilterAntiFlag(),\
									self.GetFilterMinPrice(),\
									self.GetFilterMaxPrice(),\
									self.GetFilterMinLevel(),\
									self.GetFilterMaxLevel())

	class BuyDialog(ui.ScriptWindow):
		def Refresh(self, elementInst):
			if player.GetElk() < elementInst.itemPriceYang:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_SHOP_SEARCH_NOT_ENOUGH_YANG)
				return False

			self.elementInst = elementInst

			for k, v in self.slotImgDict.items():
				if k == self.elementInst.itemSize:
					v.Show()
				else:
					v.Hide()

			self.itemIco.UnloadImage()
			self.itemIco.LoadImage(self.elementInst.itemIcoPath)

			self.priceTxt.SetText(localeinfo.V2_SHOP_SEARCH_BUY_QUANTITY1 % (localeinfo.DottedNumber(self.elementInst.itemPriceYang)))

			self.quantityTxt1.SetText(localeinfo.V2_SHOP_SEARCH_BUY_QUANTITY2 % (self.elementInst.itemCount))
			self.quantityTxt2.SetText(localeinfo.V2_SHOP_SEARCH_BUY_QUANTITY3 % (localeinfo.DottedNumber(self.elementInst.itemPriceYang / self.elementInst.itemCount)))

			#1001
			#self.avgTxt.SetText(localeinfo.V2_SHOP_SEARCH_BUY_QUANTITY4 % (localeinfo.DottedNumber(self.elementInst.itemPriceAvg)))
			return True

		def Hide(self):
			wndMgr.Hide(self.hWnd)

		def Close(self):
			self.Hide()

		def Show(self):
			wndMgr.Show(self.hWnd)

		def Open(self):
			self.SetTop()
			self.SetCenterPosition()
			self.Show()

		def OnPressEscapeKey(self):
			self.Close()
			return True

		def OnOverElement(self, isIn):
			if self.elementInst:
				self.elementInst.OnOverElement(isIn, 3)

		def OnBuy(self):
			buyList = []
			tmp = (self.elementInst.ownerID, self.elementInst.itemID, self.elementInst.itemPriceYang + (self.elementInst.itemPriceCheque * YANG_PER_CHEQUE),)
			buyList.append(tmp)
			ikashop.SendShopSearchBuyItem(tuple(buyList))
			self.Close()

		def __Load(self):
			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, "uiscript/shopsearchbuywindow_v2.py")

				self.board = self.GetChild("board_ui")

				tmp = self.GetChild("item_bg_ui")
				tmp.SetDiffuseColor(ShopSearch.DIFFUSE_COLOR_IN[0], ShopSearch.DIFFUSE_COLOR_IN[1], ShopSearch.DIFFUSE_COLOR_IN[2], ShopSearch.DIFFUSE_COLOR_IN[3])
				self.itemBG = tmp

				self.slotImgDict = {}
				for i in range(1, 4):
					tmp = self.GetChild("item_slot%d_ui" % (i))
					tmp.SetDiffuseColor(1.0, 1.0, 1.0, 0.6)
					self.slotImgDict[i] = tmp

				tmp = self.GetChild("item_ico_ui")
				tmp.SetMouseOverInEvent(ui.__mem_func__(self.OnOverElement), True)
				tmp.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverElement), False)
				self.itemIco = tmp

				self.priceTxt = self.GetChild("msg2_buy_ui")
				self.quantityTxt1 = self.GetChild("msg3_buy_ui")
				self.quantityTxt2 = self.GetChild("msg4_buy_ui")

				self.avgTxt = self.GetChild("msg5_buy_ui")
				self.avgTxt.Hide()#1001 -> just remove

				tmp = self.GetChild("buy_button_ui")
				tmp.SetEvent(ui.__mem_func__(self.OnBuy))
				self.buyBtn = tmp

				tmp = self.GetChild("close_button_ui")
				tmp.SetEvent(ui.__mem_func__(self.Close))
				self.closeBtn = tmp
			except Exception as e:
				import dbg
				dbg.TraceError("BuyDialog.__Load: {}".format(e))
				app.Abort()

		def __init__(self):
			self.itemIco = None

			ui.ScriptWindow.__init__(self)
			self.__Load()

		def Destroy(self):
			if self.itemIco:
				self.itemIco.SetMouseOverInEvent(None)
				self.itemIco.SetMouseOverOutEvent(None)
				self.itemIco = None

			self.slotImgDict = {}

			self.priceTxt = None
			self.quantityTxt1 = None
			self.quantityTxt2 = None
			self.avgTxt = None
			self.itemBG = None

			self.elementInst = None

			self.board = None

			self.ClearDictionary()

		def __del__(self):
			ui.ScriptWindow.__del__(self)

	def OnCloseBuyDlg(self):
		if self.dlgBuy:
			self.dlgBuy.Close()

	def OnCloseQuestionDlg(self):
		if self.dlgQuestion:
			self.dlgQuestion.Close()

	def BuyItem(self, elementInst):
		if not self.dlgBuy:
			self.dlgBuy = ShopSearch.BuyDialog()

		self.OnCloseQuestionDlg()

		if self.dlgBuy.Refresh(elementInst):
			self.dlgBuy.Open()

	def __BuySelected(self):
		selectedLen = len(self.selectedDict)

		if selectedLen < 0:
			return

		buyList = []
		for v in self.selectedDict.values():
			tmp = (v[0], v[1], v[2],)
			buyList.append(tmp)

		ikashop.SendShopSearchBuyItem(tuple(buyList))
		self.OnCloseQuestionDlg()

	def OnOpenQuestionDlg(self, *args):
		if not self.dlgQuestion:
			tmp = uicommon.QuestionDialog()
			tmp.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDlg))
			self.dlgQuestion = tmp

		self.OnCloseBuyDlg()

		if args[0] == 0:
			self.dlgQuestion.SetText(localeinfo.V2_SHOP_SEARCH_SELECTED_ALERT1)
			self.dlgQuestion.acceptButton.SetEvent(ui.__mem_func__(self.__GotoPage), args[1], True)
		elif args[0] == 1:
			self.dlgQuestion.SetText(localeinfo.V2_SHOP_SEARCH_SELECTED_ALERT1)
			self.dlgQuestion.acceptButton.SetEvent(ui.__mem_func__(self.__NextPage), True)
		elif args[0] == 2:
			self.dlgQuestion.SetText(localeinfo.V2_SHOP_SEARCH_SELECTED_ALERT1)
			self.dlgQuestion.acceptButton.SetEvent(ui.__mem_func__(self.__PrevPage), True)
		elif args[0] == 3:
			self.dlgQuestion.SetText(localeinfo.V2_SHOP_SEARCH_SELECTED_ALERT1)
			self.dlgQuestion.acceptButton.SetEvent(ui.__mem_func__(self.__LastPage), True)
		elif args[0] == 4:
			self.dlgQuestion.SetText(localeinfo.V2_SHOP_SEARCH_SELECTED_ALERT1)
			self.dlgQuestion.acceptButton.SetEvent(ui.__mem_func__(self.__FirstPage), True)
		elif args[0] == 5:
			self.dlgQuestion.SetText(localeinfo.V2_SHOP_SEARCH_SELECTED_ALERT2)
			self.dlgQuestion.acceptButton.SetEvent(ui.__mem_func__(self.__BuySelected))
		else:
			return

		self.dlgQuestion.Open()

	def __OnOpenQuestionDlg(self):
		self.OnOpenQuestionDlg(5)

	def OnChangeSelected(self, elementInst):
		self.OnCloseBuyDlg()
		self.OnCloseQuestionDlg()

		if elementInst.isSelected:
			totPrice = elementInst.itemPriceYang + (elementInst.itemPriceCheque * YANG_PER_CHEQUE)
			for v in self.selectedDict.values():
				totPrice += v[2]

			if player.GetElk() < totPrice:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_SHOP_SEARCH_NOT_ENOUGH_YANG_TO_ADD)
				return False

		if self.selectedDict.has_key(elementInst.index):
			del self.selectedDict[elementInst.index]

		if elementInst.isSelected:
			self.selectedDict[elementInst.index] = (elementInst.ownerID, elementInst.itemID, elementInst.itemPriceYang + (elementInst.itemPriceCheque * YANG_PER_CHEQUE))

		if len(self.selectedDict) > 0:
			self.buyAllBtn.Show()
		else:
			self.buyAllBtn.Hide()

		return True

	class ElementItem(ui.ImageBox):
		def OnOverElement(self, isIn, isNum):
			if not self.isSelected:
				if isIn:
					self.LoadImage(ShopSearch.SLOT_IMG_PATH_NEW + str(self.itemSize) + "_down" + ".png")
				else:
					self.LoadImage(ShopSearch.SLOT_IMG_PATH_NEW + str(self.itemSize) + ".png")

			if isNum == 2 or isNum == 3:
				if isIn:
					if self.itemSockets == 0:
						self.itemSockets = [ikashop.GetShopSearchItemSocket(self.index, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]

					if self.itemAttrs == 0:
						self.itemAttrs = [ikashop.GetShopSearchItemAttr(self.index, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]

					self.shopSearchInst.OnOverIn(self.itemVnum, self.itemSockets, self.itemAttrs)
				else:
					self.shopSearchInst.OnOverOut()
			elif isNum == 5:
				if isIn:
					self.pmImg.SetDiffuseColor(ShopSearch.DIFFUSE_COLOR_IN[0], ShopSearch.DIFFUSE_COLOR_IN[1], ShopSearch.DIFFUSE_COLOR_IN[2], ShopSearch.DIFFUSE_COLOR_IN[3])
				else:
					self.pmImg.SetDiffuseColor(self.diffuseColorIco[0], self.diffuseColorIco[1], self.diffuseColorIco[2], self.diffuseColorIco[3])
			elif isNum == 6:
				if isIn:
					self.shopImg.SetDiffuseColor(ShopSearch.DIFFUSE_COLOR_IN[0], ShopSearch.DIFFUSE_COLOR_IN[1], ShopSearch.DIFFUSE_COLOR_IN[2], ShopSearch.DIFFUSE_COLOR_IN[3])
				else:
					self.shopImg.SetDiffuseColor(self.diffuseColorIco[0], self.diffuseColorIco[1], self.diffuseColorIco[2], self.diffuseColorIco[3])
			elif isNum == 7:
				if isIn:
					self.searchImg.SetDiffuseColor(ShopSearch.DIFFUSE_COLOR_IN[0], ShopSearch.DIFFUSE_COLOR_IN[1], ShopSearch.DIFFUSE_COLOR_IN[2], ShopSearch.DIFFUSE_COLOR_IN[3])
				else:
					self.searchImg.SetDiffuseColor(self.diffuseColorIco[0], self.diffuseColorIco[1], self.diffuseColorIco[2], self.diffuseColorIco[3])

			if isNum >= 5 and isNum <= 7:
				if isIn:
					if len(self.sellerName) == 0:
						(self.sellerName, self.sellerID, self.isAuction) = ikashop.GetShopSearchSellerDetails(self.index)

					if self.sellerID != 0:
						self.shopSearchInst.OnOverInIco(isNum, self.sellerName)
				else:
					self.shopSearchInst.OnOverOut()

		def __OpenShop(self):
			if self.sellerID == 0:
				(self.sellerName, self.sellerID, self.isAuction) = ikashop.GetShopSearchSellerDetails(self.index)

			if self.sellerID != 0:
				if self.isAuction:
					ikashop.SendAuctionOpenAuction(self.sellerID)
				else:
					ikashop.SendOpenShop(self.sellerID)

		def __OpenWhisper(self):
			if self.sellerID == 0:
				(self.sellerName, self.sellerID, self.isAuction) = ikashop.GetShopSearchSellerDetails(self.index)

			if self.sellerID != 0:
				self.shopSearchInst.OpenWhisper(self.sellerName)

		def __SearchByOwner(self):
			if self.sellerID == 0:
				(self.sellerName, self.sellerID, self.isAuction) = ikashop.GetShopSearchSellerDetails(self.index)
				
			if self.sellerID != 0:
				self.shopSearchInst.OnSearchByOwner(self.sellerName, self.sellerID)

		def __ChangeSelected(self):
			self.multiBuy.UnloadImage()

			if self.isSelected:
				self.isSelected = False
				self.multiBuy.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png")
				self.shopSearchInst.OnChangeSelected(self)
			else:
				self.isSelected = True
				if self.shopSearchInst.OnChangeSelected(self):
					self.multiBuy.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/check.png")
				else:
					self.isSelected = False
					self.multiBuy.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png")

			self.SetDiffuseColor(ShopSearch.DIFFUSE_COLOR_IN[0], ShopSearch.DIFFUSE_COLOR_IN[1], ShopSearch.DIFFUSE_COLOR_IN[2], ShopSearch.DIFFUSE_COLOR_IN[3])

		def __ResetSelected(self):
			self.isSelected = False

			self.multiBuy.UnloadImage()
			self.multiBuy.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png")

		def __BuyItem(self):
			if self.shopSearchInst.selectedDict.has_key(self.index):
				self.__ChangeSelected()
			else:
				self.shopSearchInst.BuyItem(self)

		def __init__(self, shopSearchInst):
			ui.ImageBox.__init__(self)
			self.SetParent(shopSearchInst.itemsContainer)

			self.shopSearchInst = proxy(shopSearchInst)

			self.index = 0

			self.ownerID = 0
			self.itemID = 0

			self.itemVnum = -1
			self.itemSize = 0
			self.itemIcoPath = ""
			self.itemCount = -1
			self.itemPriceYang = 0
			self.itemPriceCheque = 0
			self.itemPriceAvg = 0

			self.itemSockets = 0
			self.itemAttrs = 0

			self.sellerName = ""
			self.sellerID = 0
			self.isAuction = False

			self.slotImgsDict = {}
			self.itemImgsDict = {}

			self.isSelected = False
			tmp = ui.ImageBox()
			tmp.SetParent(self)
			tmp.LoadImage(ShopSearch.IMG_PATH + "filter/exact_search_unchecked.png")
			tmp.SetPosition(8, 0)
			tmp.SetWindowVerticalAlignCenter()
			tmp.Show()
			tmp.SetMouseOverInEvent(ui.__mem_func__(self.OnOverElement), True, 1)
			tmp.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverElement), False, 1)
			tmp.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__ChangeSelected))
			self.multiBuy = tmp

			x = 30
			w = 0

			for i in xrange(3):
				tmp = ui.ImageBox()
				tmp.SetParent(self)
				tmp.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/slot_" + str(i + 1) + ".png")
				tmp.SetPosition(x, 0)
				tmp.SetWindowVerticalAlignCenter()
				tmp.Show()

				if w == 0:
					w = tmp.GetWidth()

				self.slotImgsDict[i + 1] = tmp

				tmp = ui.ImageBox()
				tmp.SetParent(self.slotImgsDict[i + 1])
				tmp.SetPosition(0, 0)
				tmp.SetWindowVerticalAlignCenter()
				tmp.SetWindowHorizontalAlignCenter()

				tmp.SetMouseOverInEvent(ui.__mem_func__(self.OnOverElement), True, 3)
				tmp.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverElement), False, 3)
				tmp.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__BuyItem))

				self.itemImgsDict[i + 1] = tmp

			tmp = ui.TextLine()
			tmp.SetParent(self)
			tmp.SetPosition(x + w + 4, -2)
			tmp.SetVerticalAlignCenter()
			tmp.SetWindowVerticalAlignCenter()
			tmp.Show()
			self.itemName = tmp

			tmp = ui.ImageBox()
			tmp.SetParent(self)
			tmp.LoadImage(ShopSearch.IMG_PATH + "money.png")
			tmp.SetPosition(230, 0)
			tmp.SetWindowVerticalAlignCenter()
			tmp.Show()
			tmp.SetMouseOverInEvent(ui.__mem_func__(self.OnOverElement), True, 4)
			tmp.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverElement), False, 4)
			tmp.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__BuyItem))
			self.yangImg = tmp

			tmp = ui.TextLine()
			tmp.SetParent(self)
			tmp.SetPosition(self.yangImg.GetLocalPosition()[0] + self.yangImg.GetWidth() + 4, -2)
			tmp.SetFontName("Verdana:14b")
			tmp.SetVerticalAlignCenter()
			tmp.SetWindowVerticalAlignCenter()
			tmp.Show()
			self.itemYang = tmp

			self.diffuseColorIco = (1.0, 1.0, 1.0, 0.8)

			tmp = ui.ImageBox()
			tmp.SetParent(self)
			tmp.LoadImage(ShopSearch.IMG_PATH + "pm.png")
			tmp.SetPosition(470, 0)
			tmp.SetWindowVerticalAlignCenter()
			tmp.Show()
			tmp.SetDiffuseColor(self.diffuseColorIco[0], self.diffuseColorIco[1], self.diffuseColorIco[2], self.diffuseColorIco[3])
			tmp.SetMouseOverInEvent(ui.__mem_func__(self.OnOverElement), True, 5)
			tmp.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverElement), False, 5)
			tmp.SetOnMouseLeftButtonUpEvent(self.__OpenWhisper)
			self.pmImg = tmp

			tmp = ui.ImageBox()
			tmp.SetParent(self)
			tmp.LoadImage(ShopSearch.IMG_PATH + "store.png")
			tmp.SetPosition(490, 0)
			tmp.SetWindowVerticalAlignCenter()
			tmp.Show()
			tmp.SetDiffuseColor(self.diffuseColorIco[0], self.diffuseColorIco[1], self.diffuseColorIco[2], self.diffuseColorIco[3])
			tmp.SetMouseOverInEvent(ui.__mem_func__(self.OnOverElement), True, 6)
			tmp.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverElement), False, 6)
			tmp.SetOnMouseLeftButtonUpEvent(self.__OpenShop)
			self.shopImg = tmp

			tmp = ui.ImageBox()
			tmp.SetParent(self)
			tmp.LoadImage(ShopSearch.IMG_PATH + "search.png")
			tmp.SetPosition(510, 0)
			tmp.SetWindowVerticalAlignCenter()
			tmp.Show()
			tmp.SetDiffuseColor(self.diffuseColorIco[0], self.diffuseColorIco[1], self.diffuseColorIco[2], self.diffuseColorIco[3])
			tmp.SetMouseOverInEvent(ui.__mem_func__(self.OnOverElement), True, 7)
			tmp.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverElement), False, 7)
			tmp.SetOnMouseLeftButtonUpEvent(self.__SearchByOwner)
			self.searchImg = tmp

			self.lastPath = ""

			self.diffuseColor = (1.0, 1.0, 1.0, 1.0)

			self.SetMouseOverInEvent(ui.__mem_func__(self.OnOverElement), True, 1)
			self.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverElement), False, 1)
			self.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__BuyItem))

		def __del__(self):
			self.SetMouseOverInEvent(None)
			self.SetMouseOverOutEvent(None)
			self.SetOnMouseLeftButtonUpEvent(None)

			if self.yangImg:
				self.yangImg.SetMouseOverInEvent(None)
				self.yangImg.SetMouseOverOutEvent(None)
				self.yangImg.SetOnMouseLeftButtonUpEvent(None)

				del self.yangImg

			if self.pmImg:
				self.pmImg.SetMouseOverInEvent(None)
				self.pmImg.SetMouseOverOutEvent(None)
				self.pmImg.SetOnMouseLeftButtonUpEvent(None)

				del self.pmImg

			if self.shopImg:
				self.shopImg.SetMouseOverInEvent(None)
				self.shopImg.SetMouseOverOutEvent(None)
				self.shopImg.SetOnMouseLeftButtonUpEvent(None)

				del self.shopImg

			if self.searchImg:
				self.searchImg.SetMouseOverInEvent(None)
				self.searchImg.SetMouseOverOutEvent(None)
				self.searchImg.SetOnMouseLeftButtonUpEvent(None)

				del self.searchImg

			if self.itemName:
				del self.itemName
				self.itemName = None

			for v in self.itemImgsDict.values():
				v.SetMouseOverInEvent(None)
				v.SetMouseOverOutEvent(None)
				v.SetOnMouseLeftButtonUpEvent(None)

				del v

			self.itemImgsDict = {}

			for v in self.slotImgsDict.values():
				del v

			self.slotImgsDict = {}

			self.shopSearchInst = None

			ui.ImageBox.__del__(self)

		def Build(self, ownerID, itemID, itemVnum, itemCount, j, a):
			self.__ResetSelected()

			self.ownerID = ownerID
			self.itemID = itemID

			self.index = j

			self.itemSockets = 0
			self.itemAttrs = 0

			item.SelectItem(itemVnum)
			_, self.itemSize = item.GetItemSize()
			self.itemIcoPath = item.GetIconImageFileName()
			name = item.GetItemName()

			if app.ENABLE_CHEQUE_SYSTEM:
				(self.itemPriceYang, self.itemPriceCheque, self.itemPriceAvg) = ikashop.GetShopSearchItemPrice(j)
			else:
				(self.itemPriceYang, self.itemPriceAvg) = ikashop.GetShopSearchItemPrice(j)

			self.sellerName = ""
			self.sellerID = 0
			self.isAuction = False

			newPath = ShopSearch.SLOT_IMG_PATH_NEW + str(self.itemSize) + ".png"
			if self.lastPath != newPath:
				self.lastPath = newPath

				self.UnloadImage()
				self.LoadImage(ShopSearch.SLOT_IMG_PATH_NEW + str(self.itemSize) + ".png")

			for k, v in self.slotImgsDict.items():
				if k == self.itemSize:
					self.itemImgsDict[k].UnloadImage()
					self.itemImgsDict[k].LoadImage(self.itemIcoPath)

					if itemCount == 1:
						self.itemName.SetText(name)
					else:
						self.itemName.SetText("x" + str(itemCount) + " " + name)

					v.Show()
					self.itemImgsDict[k].Show()
				else:
					if v.IsShow():
						v.Hide()

					if self.itemImgsDict[k].IsShow():
						self.itemImgsDict[k].Hide()

			self.itemYang.SetText(localeinfo.DottedNumber(self.itemPriceYang))

			# if a == 1:
				# self.diffuseColor = (1.0, 1.0, 1.0, 0.6)
			# else:
				# self.diffuseColor = (1.0, 1.0, 1.0, 1.0)

			self.SetDiffuseColor(self.diffuseColor[0], self.diffuseColor[1], self.diffuseColor[2], self.diffuseColor[3])

			self.itemVnum = itemVnum
			self.itemCount = itemCount

	def Hide(self):
		if self.nameInput and self.nameInput.IsFocus():
			self.nameInput.KillFocus()

		if self.priceMinInput and self.priceMinInput.IsFocus():
			self.priceMinInput.KillFocus()

		if self.priceMaxInput and self.priceMaxInput.IsFocus():
			self.priceMaxInput.KillFocus()

		if self.levelMinInput and self.levelMinInput.IsFocus():
			self.levelMinInput.KillFocus()

		if self.levelMaxInput and self.levelMaxInput.IsFocus():
			self.levelMaxInput.KillFocus()

		self.OnOverOut()
		self.OnCloseBuyDlg()
		self.OnCloseQuestionDlg()

		wndMgr.Hide(self.hWnd)

	def __Close(self):
		self.Hide()

	def Show(self):
		wndMgr.Show(self.hWnd)

	def __GotoPage(self, page, ignoreDictLen = False):
		if self.underUpdate:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_SOON)
			return

		if page < 1 or page > self.currentPageNum:
			return

		if not ignoreDictLen and len(self.selectedDict) > 0:
			self.OnOpenQuestionDlg(0, page)
		else:
			self.OnCloseQuestionDlg()

			self.emptyContentText.Hide()
			self.loadingContentAni.Show()

			ikashop.SendShopSearchChangePage(page)

	def __NextPage(self, ignoreDictLen = False):
		if self.underUpdate:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_SOON)
			return

		page = self.pageNum + 1
		if page > self.currentPageNum or page < 1:
			return

		if not ignoreDictLen and len(self.selectedDict) > 0:
			self.OnOpenQuestionDlg(1)
		else:
			self.OnCloseQuestionDlg()

			self.emptyContentText.Hide()
			self.loadingContentAni.Show()

			ikashop.SendShopSearchChangePage(page)

	def __PrevPage(self, ignoreDictLen = False):
		if self.underUpdate:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_SOON)
			return

		page = self.pageNum - 1
		if page > self.currentPageNum or page < 1:
			return

		if not ignoreDictLen and len(self.selectedDict) > 0:
			self.OnOpenQuestionDlg(2)
		else:
			self.OnCloseQuestionDlg()

			self.emptyContentText.Hide()
			self.loadingContentAni.Show()

			ikashop.SendShopSearchChangePage(page)

	def __LastPage(self, ignoreDictLen = False):
		if self.underUpdate:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_SOON)
			return

		if not ignoreDictLen and len(self.selectedDict) > 0:
			self.OnOpenQuestionDlg(3)
		else:
			self.OnCloseQuestionDlg()

			self.emptyContentText.Hide()
			self.loadingContentAni.Show()

			ikashop.SendShopSearchChangePage(self.currentPageNum)

	def __FirstPage(self, ignoreDictLen = False):
		if self.underUpdate:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_SOON)
			return

		if not ignoreDictLen and len(self.selectedDict) > 0:
			self.OnOpenQuestionDlg(4)
		else:
			self.OnCloseQuestionDlg()

			self.emptyContentText.Hide()
			self.loadingContentAni.Show()

			ikashop.SendShopSearchChangePage(1)

	def RefreshPages(self):
		self.currentPaginationPage = int(math.ceil(float(self.pageNum) / 5.0))
		self.shownPages = min(self.currentPageNum - (5 * (self.currentPaginationPage - 1)), 5)

		for i in xrange(len(self.pageButtonsList)):
			currentPage = (i + ((self.currentPaginationPage - 1) * 5) + 1)
			self.pageButtonsList[i].SetUp()
			self.pageButtonsList[i].SetText("%d" % currentPage)
			self.pageButtonsList[i].SetEvent(ui.__mem_func__(self.__GotoPage), currentPage)

		map(ui.Button.Hide, self.pageButtonsList)
		map(ui.Button.Enable, self.pageButtonsList)

		for i in xrange(self.shownPages):
			self.pageButtonsList[i].Show()

		self.pageButtonsList[(self.pageNum - ((self.currentPaginationPage - 1) * 5)) - 1].Down()
		self.pageButtonsList[(self.pageNum - ((self.currentPaginationPage - 1) * 5)) - 1].Disable()

		if self.currentPageNum <= 1:
			if self.currentPageNum == 1:
				self.pageButtonsList[0].Hide()

			self.firstPageButton.Hide()
			self.prevPageButton.Hide()
			self.nextPageButton.Hide()
			self.lastPageButton.Hide()
		else:
			x, y = self.content.GetLocalPosition()

			w = self.content.GetWidth()
			h = self.content.GetHeight()

			s = self.shownPages + 4
			t = (s * self.btnPageWidth) + ((s - 1) * 4)

			f = x + (w / 2)

			self.btnBackGround.SetSize(t, self.btnPageHeight)
			self.btnBackGround.SetPosition(f - (t / 2) + 55, y + h + 3)

			x, y = self.btnBackGround.GetLocalPosition()

			self.firstPageButton.SetPosition(x, y)
			x += self.btnPageWidth + 4

			self.prevPageButton.SetPosition(x, y)
			x += self.btnPageWidth + 4

			for i in xrange(self.shownPages):
				self.pageButtonsList[i].SetPosition(x, y)
				x += self.btnPageWidth + 4

			self.nextPageButton.SetPosition(x, y)
			x += self.btnPageWidth + 4

			self.lastPageButton.SetPosition(x, y)

			self.firstPageButton.Show()
			self.prevPageButton.Show()
			self.nextPageButton.Show()
			self.lastPageButton.Show()

	def RefreshMe(self):
		constInfo.changeShopSearchTime = app.GetTime() + 1

		self.OnCloseBuyDlg()
		self.OnCloseQuestionDlg()

		self.underUpdate = True

		self.selectedDict = {}
		self.buyAllBtn.Hide()

		self.loadingContentAni.Hide()

		self.itemDataList[:] = []
		self.itemCount = ikashop.GetShopSearchItemCount()
		(self.pageNum, self.currentPageNum) = ikashop.GetShopSearchPageData()

		if self.itemCount == 0:
			self.emptyContentText.Show()
		else:
			self.emptyContentText.Hide()

		self.scrollContentWindow.Hide()

		w = 0
		h = 0

		a = 0
		j = 0

		for i in self.itemsList:
			if j < self.itemCount:
				(ownerID, itemID, itemVnum, itemCount) = ikashop.GetShopSearchItemData(j)
				if itemVnum == 0:
					dbg.TraceError("ShopSearch.RefreshMe:: vnum == 0 (pos: %d)" % (j))
					continue
				elif itemCount == 0:
					dbg.TraceError("ShopSearch.RefreshMe:: count == 0 (pos: %d)" % (j))
					continue

				i.Build(ownerID, itemID, itemVnum, itemCount, j, a)

				i.SetPosition(0, h)
				i.Show()

				if w == 0:
					w = i.GetWidth()

				h += i.GetHeight() + 3
			else:
				i.Hide()

			if a == 0:
				a = 1
			else:
				a = 0

			j += 1

		self.itemsContainer.SetSize(w, h)
		self.scrollContentWindow.Show()

		self.itemsContainer.SetPosition(0, 0)

		if self.scrollBarItem:
			self.scrollBarItem.SetPos(0)

		self.ChangeScrollbarItem()

		self.RefreshPages()

		self.underUpdate = False

	def __ChangeExactSearch(self):
		self.exactSearchImg.UnloadImage()

		if self.isPerfectMatch:
			self.isPerfectMatch = False
			self.exactSearchImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png")
		else:
			self.isPerfectMatch = True
			self.exactSearchImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/check.png")

	def __ResetExactSearch(self):
		if not self.isPerfectMatch:
			return

		self.exactSearchImg.UnloadImage()
		self.isPerfectMatch = False
		self.exactSearchImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png")

	def __ChangeExactMatch(self):
		self.matchSearchImg.UnloadImage()

		if self.isPlayerMatch:
			self.isPlayerMatch = False
			self.matchSearchImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png")
			self.nameInput.SetOverlayText(localeinfo.V2_SHOP_SEARCH_OVERLAY_ITEM)
		else:
			self.isPlayerMatch = True
			self.matchSearchImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/check.png")
			self.nameInput.SetOverlayText(localeinfo.V2_SHOP_SEARCH_OVERLAY_PLAYER)

		self.nameInput.SetText("")
		self.nameInputHint.SetText("")

	def __ResetExactMatch(self):
		if not self.isPlayerMatch:
			return

		self.matchSearchImg.UnloadImage()
		self.isPlayerMatch = False
		self.matchSearchImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png")

	if app.USE_PREMIUM_AFFECT:
		def __ChangeAscendingSearch(self):
			if not self.isAscendingOrder and player.GetStatus(player.POINT_PREMIUM_USER) < 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_SHOP_PRICE_ASCENDING_NO_PREMIUM)
				return

			self.ascendingOrderImg.UnloadImage()

			if self.isAscendingOrder:
				self.isAscendingOrder = False
				self.ascendingOrderImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png")
			else:
				self.isAscendingOrder = True
				self.ascendingOrderImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/check.png")

			if not ikashop.ChangeAscendingOrder(self.isAscendingOrder):
				self.__ResetAscendingSearch(False)

		def __ResetAscendingSearch(self, bRefresh):
			if not self.isAscendingOrder:
				return

			self.ascendingOrderImg.UnloadImage()
			self.isAscendingOrder = False
			self.ascendingOrderImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png")

			if bRefresh:
				ikashop.ChangeAscendingOrder(self.isAscendingOrder)

		def RefreshAscending(self):
			if self.IsShow() and self.isAscendingOrder:
				if player.GetStatus(player.POINT_PREMIUM_USER) < 1:
					self.__ResetAscendingSearch(True)

	def __SearchFiltered(self):
		dif = constInfo.changeShopSearchTime - app.GetTime()
		if dif > 0:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
			return

		constInfo.changeShopSearchTime = app.GetTime() + 1
		# chat.AppendChat(1, "Filtered with ownerpid %d" % self.ownerPid)
		ikashop.SendShopSearchOpen(0 if self.itemCatIdx == -1 else self.itemCatIdx,\
									self.itemSubCatIdx,\
									self.nameInput.GetText(),\
									self.isPerfectMatch,\
									self.resVnum,\
									self.isPlayerMatch,\
									self.ownerPid,\
									self.GetFilterAntiFlag(),\
									self.GetFilterMinPrice(),\
									self.GetFilterMaxPrice(),\
									self.GetFilterMinLevel(),\
									self.GetFilterMaxLevel())

	def SearchEmeraldus(self, ownerName):
		# dif = constInfo.changeShopSearchTime - app.GetTime()
		# if dif > 0:
			# chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
			# return

		constInfo.changeShopSearchTime = app.GetTime() + 1
		# chat.AppendChat(1, "SearchEmeraldus %d" % self.ownerPid)
		ikashop.SendShopSearchOpen(0 if self.itemCatIdx == -1 else self.itemCatIdx,\
									self.itemSubCatIdx,\
									ownerName,\
									self.isPerfectMatch,\
									self.resVnum,\
									True,\
									0,\
									self.GetFilterAntiFlag(),\
									self.GetFilterMinPrice(),\
									self.GetFilterMaxPrice(),\
									self.GetFilterMinLevel(),\
									self.GetFilterMaxLevel())

	def __ResetInput(self, fromOpen = False):
		if not fromOpen:
			dif = constInfo.changeShopSearchTime - app.GetTime()
			if dif > 0:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
				return

		self.ownerPid = 0
		self.resName = ""
		self.resVnum = -1

		self.nameInput.SetOverlayText(localeinfo.V2_SHOP_SEARCH_OVERLAY_ITEM)
		self.nameInput.SetText("")
		self.nameInputHint.SetText("")

		self.__ResetExactSearch()
		self.__ResetExactMatch()

		if app.USE_PREMIUM_AFFECT:
			self.__ResetAscendingSearch(True)

		if not fromOpen:
			constInfo.changeShopSearchTime = app.GetTime() + 1

			ikashop.SendShopSearchOpen(0 if self.itemCatIdx == -1 else self.itemCatIdx,\
										self.itemSubCatIdx,\
										self.nameInput.GetText(),\
										self.isPerfectMatch,\
										self.resVnum,\
										self.isPlayerMatch,\
										self.ownerPid,\
										self.GetFilterAntiFlag(),\
										self.GetFilterMinPrice(),\
										self.GetFilterMaxPrice(),\
										self.GetFilterMinLevel(),\
										self.GetFilterMaxLevel())

	def __Open(self):
		dif = constInfo.changeShopSearchTime - app.GetTime()
		if dif > 0:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
			return

		self.__ResetInput(True)
		self.__ResetFilter(True)

		for item in self.categoryContainer.GetItems():
			item.SetOpenCloseEventStatus(False)
			item.Close()
			item.SetOpenCloseEventStatus(True)

		self.categoryContainer.RecalculateHeight()
		self.UpdateScrollbar()

		self.categoryIdx = -1
		self.itemCatIdx = -1
		self.itemSubCatIdx = -1

		self.categoryContainer.GetElementByIndex(0).Open()

		self.SetCenterPosition()
		self.Show()
		
	def OpenNoSearch(self):
		# dif = constInfo.changeShopSearchTime - app.GetTime()
		# if dif > 0:
			# chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
			# return

		self.__ResetInput(True)
		self.__ResetFilter(True)

		for item in self.categoryContainer.GetItems():
			item.SetOpenCloseEventStatus(False)
			item.Close()
			item.SetOpenCloseEventStatus(True)

		self.categoryContainer.RecalculateHeight()
		self.UpdateScrollbar()

		self.categoryIdx = -1
		self.itemCatIdx = -1
		self.itemSubCatIdx = -1

		self.categoryContainer.GetElementByIndex(0).Open()

		self.SetCenterPosition()
		self.Show()

	def Toggle(self, isFromWhisper = False):
		if isFromWhisper:
			self.OpenNoSearch()
			return
			
		self.__Close() if self.IsShow() else self.__Open()

	def OnSearchFastItem(self, itemVnum, itemName):
		dif = constInfo.changeShopSearchTime - app.GetTime()
		if dif > 0:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
			return

		constInfo.changeShopSearchTime = app.GetTime() + 1

		if self.isPlayerMatch:
			self.__ChangeExactMatch()

		if not self.isPerfectMatch:
			self.__ChangeExactSearch()

		self.nameInput.SetText("")
		self.nameInputHint.SetText("")

		self.nameInput.SetText(itemName)
		ime.SetCursorPosition(-1)

		self.__ResetFilter(True)

		if self.nameInput and self.nameInput.IsFocus():
			self.nameInput.KillFocus()

		self.resVnum = itemVnum

		for item in self.categoryContainer.GetItems():
			item.SetOpenCloseEventStatus(False)
			item.Close()
			item.SetOpenCloseEventStatus(True)

		self.categoryContainer.RecalculateHeight()
		self.UpdateScrollbar()

		self.categoryIdx = -1
		self.itemCatIdx = -1
		self.itemSubCatIdx = -1

		self.categoryContainer.GetElementByIndex(0).Open()

		self.SetCenterPosition()
		self.Show()
		
	def OnSearchByOwnerName(self, itemVnum, itemName):
		dif = constInfo.changeShopSearchTime - app.GetTime()
		if dif > 0:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
			return

		constInfo.changeShopSearchTime = app.GetTime() + 1
		self.isPlayerMatch = True

		# if self.isPlayerMatch:
		# self.__ChangeExactMatch()

		# if not self.isPerfectMatch:
			# self.__ChangeExactSearch()

		self.nameInput.SetText("")
		self.nameInputHint.SetText("")

		self.nameInput.SetText(itemName)
		ime.SetCursorPosition(-1)

		self.__ResetFilter(True)

		if self.nameInput and self.nameInput.IsFocus():
			self.nameInput.KillFocus()

		self.resVnum = itemVnum

		for item in self.categoryContainer.GetItems():
			item.SetOpenCloseEventStatus(False)
			item.Close()
			item.SetOpenCloseEventStatus(True)

		self.categoryContainer.RecalculateHeight()
		self.UpdateScrollbar()

		self.categoryIdx = -1
		self.itemCatIdx = -1
		self.itemSubCatIdx = -1

		self.categoryContainer.GetElementByIndex(0).Open()

		self.SetCenterPosition()
		self.Show()

	def OnPressEscapeKey(self):
		self.__Close()
		return True

	def OnToggleWindowToggle(self, idx):
		if idx == self.categoryIdx and idx != 0 and not (idx >= 5 and idx <= 10):
			self.categoryIdx = -1
			self.itemCatIdx = -1
			self.itemSubCatIdx = -1

			for item in self.categoryContainer.GetItems():
				item.SetOpenCloseEventStatus(False)
				item.Close()
				item.SetOpenCloseEventStatus(True)

			self.categoryContainer.RecalculateHeight()
			self.UpdateScrollbar()
			return

		self.categoryIdx = idx

		for item in self.categoryContainer.GetItems():
			item.SetOpenCloseEventStatus(False)
			item.Close()
			item.SetOpenCloseEventStatus(True)

		self.categoryContainer.GetElementByIndex(idx).Open()
		self.categoryContainer.RecalculateHeight()
		self.UpdateScrollbar()

	def OnToggleWindowClose(self, idx):
		pass

	def OnToggleWindowOpen(self, idx):
		self.itemCatIdx = -1
		self.itemSubCatIdx = -1

		for v in self.subCategoriesDict.values():
			for l in v:
				l[1].SetUp()

		if idx == 0:
			constInfo.changeShopSearchTime = app.GetTime() + 1

			self.emptyContentText.Hide()
			self.loadingContentAni.Show()

			ikashop.SendShopSearchOpen(0 if self.itemCatIdx == -1 else self.itemCatIdx,\
										self.itemSubCatIdx,\
										self.nameInput.GetText(),\
										self.isPerfectMatch,\
										self.resVnum,\
										self.isPlayerMatch,\
										self.ownerPid,\
										self.GetFilterAntiFlag(),\
										self.GetFilterMinPrice(),\
										self.GetFilterMaxPrice(),\
										self.GetFilterMinLevel(),\
										self.GetFilterMaxLevel())
		elif idx >= 5 and idx <= 10:
			constInfo.changeShopSearchTime = app.GetTime() + 1

			self.emptyContentText.Hide()
			self.loadingContentAni.Show()

			ikashop.SendShopSearchOpen(idx if self.itemCatIdx == -1 else self.itemCatIdx,\
										self.itemSubCatIdx,\
										self.nameInput.GetText(),\
										self.isPerfectMatch,\
										self.resVnum,\
										self.isPlayerMatch,\
										self.ownerPid,\
										self.GetFilterAntiFlag(),\
										self.GetFilterMinPrice(),\
										self.GetFilterMaxPrice(),\
										self.GetFilterMinLevel(),\
										self.GetFilterMaxLevel())

	def OnChooseCategory(self, j, k, idx):
		dif = constInfo.changeShopSearchTime - app.GetTime()
		if dif > 0:
			if self.subCategoriesDict.has_key(k):
				for l in self.subCategoriesDict[k]:
					if l[0] == j:
						l[1].SetUp()

			chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
			return

		self.itemCatIdx = idx[0]
		self.itemSubCatIdx = idx[1]

		if self.subCategoriesDict.has_key(k):
			for l in self.subCategoriesDict[k]:
				if l[0] == j:
					l[1].Down()
				else:
					l[1].SetUp()

		constInfo.changeShopSearchTime = app.GetTime() + 1

		self.emptyContentText.Hide()
		self.loadingContentAni.Show()

		ikashop.SendShopSearchOpen(0 if self.itemCatIdx == -1 else self.itemCatIdx,\
									self.itemSubCatIdx,\
									self.nameInput.GetText(),\
									self.isPerfectMatch,\
									self.resVnum,\
									self.isPlayerMatch,\
									self.ownerPid,\
									self.GetFilterAntiFlag(),\
									self.GetFilterMinPrice(),\
									self.GetFilterMaxPrice(),\
									self.GetFilterMinLevel(),\
									self.GetFilterMaxLevel())

	def OnScrollBar(self, fScale):
		curr = min(0, max(math.ceil((self.categoryContainer.GetHeight() - self.scrollCatWindow.GetHeight()) * fScale * -1.0), -self.categoryContainer.GetHeight() + self.scrollCatWindow.GetHeight()))
		self.categoryContainer.SetPosition(0, curr)

	def ChangeScrollbar(self):
		if not self.scrollBar:
			return

		if self.categoryContainer.GetHeight() <= self.scrollCatWindow.GetHeight():
			self.scrollBar.Hide()
		else:
			self.scrollBar.SetScale(self.scrollCatWindow.GetHeight(), self.categoryContainer.GetHeight())
			self.scrollBar.SetPosScale(truediv(abs(self.categoryContainer.GetLocalPosition()[1]), (self.categoryContainer.GetHeight() - self.scrollCatWindow.GetHeight())))
			self.scrollBar.Show()

	def UpdateScrollbar(self, val = 0):
		curr = min(0, max(self.categoryContainer.GetLocalPosition()[1] + val, -self.categoryContainer.GetHeight() + self.scrollCatWindow.GetHeight()))
		self.categoryContainer.SetPosition(0, curr)

		self.ChangeScrollbar()

	def OnScrollbarItem(self, fScale):
		curr = min(0, max(math.ceil((self.itemsContainer.GetHeight() - self.scrollContentWindow.GetHeight()) * fScale * -1.0), -self.itemsContainer.GetHeight() + self.scrollContentWindow.GetHeight()))
		self.itemsContainer.SetPosition(0, curr)

	def ChangeScrollbarItem(self):
		if not self.scrollBarItem:
			return

		if self.itemsContainer.GetHeight() <= self.scrollContentWindow.GetHeight():
			self.scrollBarItem.Hide()
			return
		else:
			if (self.itemsContainer.GetHeight() - self.scrollContentWindow.GetHeight()) <= 2:
				self.scrollBarItem.Hide()
				return

		self.scrollBarItem.SetScale(self.scrollContentWindow.GetHeight(), self.itemsContainer.GetHeight())
		self.scrollBarItem.SetPosScale(truediv(abs(self.itemsContainer.GetLocalPosition()[1]), (self.itemsContainer.GetHeight() - self.scrollContentWindow.GetHeight())))
		self.scrollBarItem.Show()

	def UpdateScrollbarItem(self, val = 0):
		curr = min(0, max(self.itemsContainer.GetLocalPosition()[1] + val, -self.itemsContainer.GetHeight() + self.scrollContentWindow.GetHeight()))
		self.itemsContainer.SetPosition(0, curr)

		self.ChangeScrollbarItem()

	def OnRunMouseWheel(self, length):
		if self.menu.IsInPosition():
			if self.scrollBar and self.scrollBar.IsShow():
				self.UpdateScrollbar(int((length * 0.01) * self.SCROLL_SPEED))
				return True

		if self.content.IsInPosition():
			if self.scrollBarItem and self.scrollBarItem.IsShow():
				self.UpdateScrollbarItem(int((length * 0.01) * self.SCROLL_SPEED_ITEM))
				return True

		return False

	def OnPressNameInputEscapeKey(self):
		if not self.nameInput.IsShowCursor() or self.nameInput.GetText() == "":
			self.OnPressEscapeKey()
		else:
			self.nameInput.SetText("")
			self.nameInputHint.SetText("")

	def OnUpdateNameInput(self):
		if self.isPlayerMatch:
			self.nameInput.SetPackedFontColor(ShopSearch.SEARCH_FOUND_COLOR)
			return

		if not self.nameInputHint:
			return

		self.nameInputHint.SetText("")
		self.nameInput.SetPackedFontColor(ShopSearch.SEARCH_FOUND_COLOR)

		searchHint = self.nameInput.GetText()

		(self.resName, self.resVnum) = item.GetItemDataByNamePart(searchHint)
		if self.resVnum == -1:
			self.nameInputHint.SetText("")
			self.nameInput.SetPackedFontColor(ShopSearch.SEARCH_NOTFOUND_COLOR)
		else:
			self.nameInputHint.SetText(searchHint + " " + self.resName[len(searchHint):])

	def OnAutoCompleteNameInput(self):
		if not self.isPlayerMatch and len(self.resName) > 0:
			self.nameInput.SetText(self.resName)
			self.OnUpdateNameInput()

		self.nameInput.SetEndPosition()

	def __CloseFilterWindow(self):
		self.scrollFilterWindow.Hide()

	def __OpenFilterWindow(self):
		self.scrollFilterWindow.Show()

	def __OnToggleFilter(self):
		self.filterImg.UnloadImage()

		if self.filterStatus:
			self.filterStatus = False
			self.filterImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/filterbtn.png")

			if self.scrollBarWasShown:
				self.scrollBar.Show()

			self.scrollCatWindow.Show()

			self.__CloseFilterWindow()
		else:
			self.scrollBarWasShown = self.scrollBar.IsShow()

			self.scrollBar.Hide()
			self.scrollCatWindow.Hide()

			self.__OpenFilterWindow()

			self.filterStatus = True
			self.filterImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/filterbtn_down.png")

	def OnFilterToggleButtonDown(self, btn):
		btn.SetUp()

	def OnFilterToggleButtonSetUp(self, btn):
		btn.Down()

	def __ResetFilter(self, fromOpening = False, fromByName = False):
		if not fromOpening and not fromByName:
			dif = constInfo.changeShopSearchTime - app.GetTime()
			if dif > 0:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeinfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
				return

			constInfo.changeShopSearchTime = app.GetTime() + 1

		if not fromByName and fromOpening and self.filterStatus:
			self.scrollBarWasShown = False
			self.__OnToggleFilter()

		self.OnFilterToggleButtonDown(self.filterWarriorBtn)
		self.OnFilterToggleButtonDown(self.filterAssassinBtn)
		self.OnFilterToggleButtonDown(self.filterSuraBtn)
		self.OnFilterToggleButtonDown(self.filterShamanBtn)

		self.OnFilterToggleButtonDown(self.filterMaleBtn)
		self.OnFilterToggleButtonDown(self.filterFemaleBtn)

		self.priceMinInput.SetText("")
		self.priceMaxInput.SetText("")

		self.levelMinInput.SetText("")
		self.levelMaxInput.SetText("")

		if not fromByName and not fromOpening:
			ikashop.SendShopSearchOpen(0 if self.itemCatIdx == -1 else self.itemCatIdx,\
										self.itemSubCatIdx,\
										self.nameInput.GetText(),\
										self.isPerfectMatch,\
										self.resVnum,\
										self.isPlayerMatch,\
										self.ownerPid,\
										self.GetFilterAntiFlag(),\
										self.GetFilterMinPrice(),\
										self.GetFilterMaxPrice(),\
										self.GetFilterMinLevel(),\
										self.GetFilterMaxLevel())

	# def __CloseFilter(self):
		# if self.filterStatus:
			# self.filterImg.UnloadImage()

			# self.filterStatus = False
			# self.filterImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/filterbtn.png")

			# if self.scrollBarWasShown:
				# self.scrollBar.Show()

			# self.scrollCatWindow.Show()

			# self.__CloseFilterWindow()

	def OnTabPriceInput(self):
		if self.priceMinInput.IsFocus():
			self.priceMinInput.KillFocus()
			self.priceMaxInput.SetFocus()
		elif self.priceMaxInput.IsFocus():
			self.priceMaxInput.KillFocus()
			self.priceMinInput.SetFocus()
		else:
			return

		ime.SetCursorPosition(-1)

	def OnTabLevelInput(self):
		if self.levelMinInput.IsFocus():
			self.levelMinInput.KillFocus()
			self.levelMaxInput.SetFocus()
		elif self.levelMaxInput.IsFocus():
			self.levelMaxInput.KillFocus()
			self.levelMinInput.SetFocus()
		else:
			return

		ime.SetCursorPosition(-1)

	def __Load(self):
		try:
			self.underUpdate = False

			self.itemCatIdx = -1
			self.itemSubCatIdx = -1
			self.categoryIdx = -1

			self.itemDataList = []
			self.itemCount = 0

			self.pageNum = 0
			self.currentPageNum = 0

			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/shopsearchwindow_v2.py")

			self.board = self.GetChild("board_ui")
			self.board.SetCloseEvent(ui.__mem_func__(self.__Close))

			self.menu = self.GetChild("menu_ui")

			self.categoriesList = []

			# 0: all
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_ALL, ShopSearch.CATEGORY_IMG_PATH + "all.png"], []])

			# 1: weapons
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_WEAPONS, ShopSearch.CATEGORY_IMG_PATH + "weapons.png"], [\
				[localeinfo.V2_SHOP_SEARCH_SWORD, (item.ITEM_TYPE_WEAPON, item.WEAPON_SWORD)],
				[localeinfo.V2_SHOP_SEARCH_TWOSWORD, (item.ITEM_TYPE_WEAPON, item.WEAPON_TWO_HANDED)],
				[localeinfo.V2_SHOP_SEARCH_DAGGER, (item.ITEM_TYPE_WEAPON, item.WEAPON_DAGGER)],
				[localeinfo.V2_SHOP_SEARCH_BOW, (item.ITEM_TYPE_WEAPON, item.WEAPON_BOW)],
				[localeinfo.V2_SHOP_SEARCH_BELL, (item.ITEM_TYPE_WEAPON, item.WEAPON_BELL)],
				[localeinfo.V2_SHOP_SEARCH_FAN, (item.ITEM_TYPE_WEAPON, item.WEAPON_FAN)],
				[localeinfo.V2_SHOP_SEARCH_ARROW, (item.ITEM_TYPE_WEAPON, item.WEAPON_ARROW)],
				[localeinfo.V2_SHOP_SEARCH_QUIVER, (item.ITEM_TYPE_WEAPON, item.WEAPON_QUIVER)],
			]])

			# 2: equipment
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_EQUIPMENTS, ShopSearch.CATEGORY_IMG_PATH + "equipments.png"], [\
				[localeinfo.V2_SHOP_SEARCH_BODY, (item.ITEM_TYPE_ARMOR, item.ARMOR_BODY)],
				[localeinfo.V2_SHOP_SEARCH_HEAD, (item.ITEM_TYPE_ARMOR, item.ARMOR_HEAD)],
				[localeinfo.V2_SHOP_SEARCH_SHIELD, (item.ITEM_TYPE_ARMOR, item.ARMOR_SHIELD)],
				[localeinfo.V2_SHOP_SEARCH_WRIST, (item.ITEM_TYPE_ARMOR, item.ARMOR_WRIST)],
				[localeinfo.V2_SHOP_SEARCH_FOOTS, (item.ITEM_TYPE_ARMOR, item.ARMOR_FOOTS)],
				[localeinfo.V2_SHOP_SEARCH_NECK, (item.ITEM_TYPE_ARMOR, item.ARMOR_NECK)],
				[localeinfo.V2_SHOP_SEARCH_EAR, (item.ITEM_TYPE_ARMOR, item.ARMOR_EAR)],
			]])

			if app.ENABLE_SASH_SYSTEM:
				self.categoriesList[2][1].append([localeinfo.V2_SHOP_SEARCH_CATEGORY_SHOULDER_SASH, (item.ITEM_TYPE_COSTUME, item.COSTUME_TYPE_SASH)])

			# if app.ITEM_TALISMAN_EQUIPMENT:#todo
				# self.categoriesList[2][1].append([localeinfo.V2_SHOP_SEARCH_PENDANT, (item.ITEM_TYPE_ARMOR, item.ARMOR_PENDANT)])

			if app.ENABLE_YOHARA_SYSTEM:
				self.categoriesList[2][1].append([localeinfo.V2_SHOP_SEARCH_GLOVE, (item.ITEM_TYPE_ARMOR, item.ARMOR_GLOVE)])

			# 3: ds
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_DS, ShopSearch.CATEGORY_IMG_PATH + "ds.png"], [\
				[localeinfo.V2_SHOP_SEARCH_DS_WHITE, (item.ITEM_TYPE_DS, item.DS_WHITE)],
				[localeinfo.V2_SHOP_SEARCH_DS_RED, (item.ITEM_TYPE_DS, item.DS_RED)],
				[localeinfo.V2_SHOP_SEARCH_DS_GREEN, (item.ITEM_TYPE_DS, item.DS_GREEN)],
				[localeinfo.V2_SHOP_SEARCH_DS_BLUE, (item.ITEM_TYPE_DS, item.DS_BLUE)],
				[localeinfo.V2_SHOP_SEARCH_DS_YELLOW, (item.ITEM_TYPE_DS, item.DS_YELLOW)],
				[localeinfo.V2_SHOP_SEARCH_DS_BLACK, (item.ITEM_TYPE_DS, item.DS_BLACK)],
			]])

			# 4: costumes
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_COSTUMES, ShopSearch.CATEGORY_IMG_PATH + "costumes.png"], [\
				[localeinfo.V2_SHOP_SEARCH_COSTUME_BODY, (item.ITEM_TYPE_COSTUME, item.COSTUME_TYPE_BODY)],
				[localeinfo.V2_SHOP_SEARCH_COSTUME_HAIR, (item.ITEM_TYPE_COSTUME, item.COSTUME_TYPE_HAIR)],
			]])

			if app.ENABLE_WEAPON_COSTUME_SYSTEM:
				self.categoriesList[4][1].append([localeinfo.V2_SHOP_SEARCH_COSTUME_WEAPON, (item.ITEM_TYPE_COSTUME, item.COSTUME_TYPE_WEAPON)])

			if app.ENABLE_SASH_COSTUME_SYSTEM:
				self.categoriesList[4][1].append([localeinfo.V2_SHOP_SEARCH_COSTUME_SASH, (item.ITEM_TYPE_COSTUME, item.COSTUME_TYPE_SKIN_SASH)])

			if app.ENABLE_NEW_PET_SYSTEM:
				self.categoriesList[4][1].append([localeinfo.V2_SHOP_SEARCH_CATEGORY_PETS, (item.ITEM_TYPE_COSTUME, item.COSTUME_TYPE_PET)])

			if app.ENABLE_MOUNT_SKIN:
				self.categoriesList[4][1].append([localeinfo.V2_SHOP_SEARCH_CATEGORY_MOUNTS, (item.ITEM_TYPE_COSTUME, item.COSTUME_TYPE_MOUNT_SKIN)])

			# 5: mounts
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_MOUNTS, ShopSearch.CATEGORY_IMG_PATH + "mounts.png"], []])

			# 6: pets
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_PETS, ShopSearch.CATEGORY_IMG_PATH + "pets.png"], []])

			# 7: companions
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_COMPANIONS, ShopSearch.CATEGORY_IMG_PATH + "companions.png"], []])

			# 8: chests
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_CHESTS, ShopSearch.CATEGORY_IMG_PATH + "chests.png"], []])

			# 9: upgrades
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_UPGRADES, ShopSearch.CATEGORY_IMG_PATH + "upgrades.png"], []])

			# 10: books
			self.categoriesList.append([[localeinfo.V2_SHOP_SEARCH_CATEGORY_BOOKS, ShopSearch.CATEGORY_IMG_PATH + "books.png"], []])

			y = 66

			tmp = ui.Window()
			tmp.SetParent(self.menu)
			tmp.AddFlag("attach")
			tmp.AddFlag("not_pick")
			tmp.SetSize(self.menu.GetWidth() - 5, self.menu.GetHeight() - 5)
			tmp.SetPosition(0, y - 65)
			tmp.SetWindowHorizontalAlignCenter()
			tmp.SetWindowVerticalAlignTop()
			tmp.Show()
			self.scrollCatWindow = tmp

			tmp = ui.ThinBoardCircle()
			tmp.SetParent(self.menu)
			tmp.AddFlag("attach")
			tmp.AddFlag("not_pick")
			tmp.SetSize(self.menu.GetWidth() - 10, self.menu.GetHeight() - 66 - 20 - 28)
			tmp.SetPosition(0, y - 60)
			tmp.SetWindowHorizontalAlignCenter()
			tmp.SetWindowVerticalAlignTop()
			tmp.Hide()
			self.scrollFilterWindow = tmp

			tmp = ui.AutoGrowingVerticalContainerEx()
			tmp.SetParent(self.scrollCatWindow)
			tmp.SetPosition(0, 0)
			tmp.SetWidth(169)
			self.categoryContainer = tmp

			self.subCategoriesDict = {}

			for i, c in enumerate(self.categoriesList):
				tmp = ui.ToggleAbleTitledWindow(ShopSearch.ROOT_PATH + "left_rectangle/category_on.png", ShopSearch.ROOT_PATH + "left_rectangle/category_off.png", c[0][1])
				tmp.SetParent(self.categoryContainer)
				tmp.SetWidth(169)
				tmp.SetPosition(0, 0)
				tmp.SetTitle(c[0][0])
				tmp.SetOnToggleEvent(ui.__mem_func__(self.OnToggleWindowToggle), i)
				tmp.SetCloseEvent(ui.__mem_func__(self.OnToggleWindowClose), i)
				tmp.SetOpenEvent(ui.__mem_func__(self.OnToggleWindowOpen), i)

				if i != 0:
					if not (i >= 5 and i <= 10):
						tmp.SetDelay(False)

				if len(c[1]) > 0:
					tmp.SetToggleHideFilename(ShopSearch.IMG_PATH + "arrow_up.png")
					tmp.SetToggleShowFilename(ShopSearch.IMG_PATH + "arrow_down.png")

				tmp.Close()
				tmp.Show()

				self.categoryContainer.AppendItem(tmp)
				self.subCategoriesDict[i] = []

			for idx, cat in enumerate(self.categoriesList):
				j = 0
				for info in cat[1]:
					tmp = ui.MakeRadioButton(self.categoryContainer, 0, 0, info[0], "", ShopSearch.IMG_PATH, "subcategory_off.png", "subcategory_off.png", "subcategory_on.png")
					tmp.SetEvent(ui.__mem_func__(self.OnChooseCategory), j, idx, info[1])
					self.categoryContainer.GetElementByIndex(idx).AppendToToggleContent(tmp)

					self.subCategoriesDict[idx].append([j, tmp],)
					j += 1

			self.categoryContainer.Show()
			self.categoryContainer.RecalculateHeight()

			tmp = ui.ScrollBarToggle()
			tmp.SetParent(self.board)
			tmp.SetPosition(self.scrollCatWindow.GetLocalPosition()[0] + self.scrollCatWindow.GetWidth() + 5 + 11, self.menu.GetLocalPosition()[1] + y - 63)
			tmp.SetSize(7, self.scrollCatWindow.GetHeight())
			tmp.SetScrollEvent(self.OnScrollBar)
			tmp.SetScrollSpeed(ShopSearch.SCROLL_SPEED)
			self.scrollBar = tmp

			if app.USE_CLIP_MASK:
				self.menu.SetClippingMaskWindow(self.scrollCatWindow)

			self.loadingContentAni = self.GetChild("loading_content_ui")
			self.loadingContentAni.Hide()

			self.emptyContentText = self.GetChild("empty_content_ui")
			self.emptyContentText.Hide()

			self.content = self.GetChild("content_ui")

			tmp = ui.Window()
			tmp.SetParent(self.content)
			tmp.AddFlag("attach")
			tmp.AddFlag("not_pick")
			tmp.SetSize(self.content.GetWidth() - 6, self.content.GetHeight() - 6)
			tmp.SetPosition(0, 3)
			tmp.SetWindowHorizontalAlignCenter()
			tmp.SetWindowVerticalAlignTop()
			tmp.Show()
			self.scrollContentWindow = tmp

			tmp = ui.Window()
			tmp.SetParent(self.scrollContentWindow)
			tmp.SetPosition(0, 0)
			tmp.Show()
			self.itemsContainer = tmp

			self.itemsList = []
			for i in xrange(ShopSearch.MAX_ITEM_PER_SEARCH_PAGE):
				tmp = ShopSearch.ElementItem(self)
				self.itemsList.append(tmp)

			tmp = ui.ScrollBarToggle()
			tmp.SetParent(self.board)
			tmp.SetPosition(self.content.GetLocalPosition()[0] + self.content.GetWidth() - 9, self.content.GetLocalPosition()[1] + 2)
			tmp.SetSize(7, self.scrollContentWindow.GetHeight() + 2)
			tmp.SetScrollEvent(self.OnScrollbarItem)
			tmp.SetScrollSpeed(ShopSearch.SCROLL_SPEED_ITEM)
			self.scrollBarItem = tmp

			if app.USE_CLIP_MASK:
				self.content.SetClippingMaskWindow(self.scrollContentWindow)

			self.pageButtonsList = []

			for i in range(1, 6):
				tmp = self.GetChild("page%d_button_ui" % (i))

				if i == 1:
					tmp.Show()
					tmp.Down()
					tmp.Disable()
				else:
					tmp.Hide()

				self.pageButtonsList.append(tmp)

			tmp = self.GetChild("next_button_ui")
			tmp.SetEvent(ui.__mem_func__(self.__NextPage))
			self.nextPageButton = tmp

			tmp = self.GetChild("prev_button_ui")
			tmp.SetEvent(ui.__mem_func__(self.__PrevPage))
			self.prevPageButton = tmp

			tmp = self.GetChild("last_next_button_ui")
			tmp.SetEvent(ui.__mem_func__(self.__LastPage))
			self.lastPageButton = tmp

			tmp = self.GetChild("first_prev_button_ui")
			tmp.SetEvent(ui.__mem_func__(self.__FirstPage))
			self.firstPageButton = tmp

			self.btnPageWidth = self.nextPageButton.GetWidth()
			self.btnPageHeight = self.nextPageButton.GetHeight()

			tmp = ui.Window()
			tmp.SetParent(self)
			tmp.SetSize(0, 0)
			tmp.Hide()
			self.btnBackGround = tmp

			(x, y) = 10, 34
			self.nameInputBG = ui.MakeImageBox(self, ShopSearch.ROOT_PATH + "right_rectangle/searchrect.png", x + 1, y - 1)

			tmp = ui.EditLine()
			tmp.SetParent(self.nameInputBG)
			tmp.SetMax(24 * 2)
			tmp.SetSize(self.nameInputBG.GetWidth() - 50, self.nameInputBG.GetHeight())
			tmp.SetLimitWidth(tmp.GetWidth() - 6)
			tmp.SetPosition(6, 6)
			tmp.SetOutline()
			tmp.SetEscapeEvent(ui.__mem_func__(self.OnPressNameInputEscapeKey))
			tmp.SetUpdateEvent(ui.__mem_func__(self.OnUpdateNameInput))
			tmp.SetTabEvent(ui.__mem_func__(self.OnAutoCompleteNameInput))
			tmp.SetReturnEvent(ui.__mem_func__(self.__SearchFiltered))
			tmp.Show()
			self.nameInput = tmp

			tmp = ui.TextLine()
			tmp.SetParent(self.nameInput)
			tmp.SetPackedFontColor(ui.WHITE_COLOR)
			tmp.Show()
			self.nameInputHint = tmp

			(x, y) = self.nameInputBG.GetLocalPosition()
			tmp = ui.MakeButton(self, x + 165, y + 2, localeinfo.V2_SHOP_INPUT_SEARCH, ShopSearch.ROOT_PATH + "right_rectangle/", "searchbtn.png", "searchbtn_over.png", "searchbtn_down.png")
			tmp.SetEvent(ui.__mem_func__(self.__SearchFiltered))
			self.btnSearch = tmp

			(x, y) = self.nameInputBG.GetLocalPosition()
			tmp = ui.MakeButton(self, x + self.nameInputBG.GetWidth() - 20, y + self.btnSearch.GetHeight() + 12, localeinfo.V2_SHOP_INPUT_RESET, ShopSearch.ROOT_PATH + "right_rectangle/", "resetbtn.png", "resetbtn_over.png", "resetbtn_down.png")
			tmp.SetEvent(ui.__mem_func__(self.__ResetInput))
			self.resetSearch = tmp

			self.isPerfectMatch = False
			# (x, y) = self.nameInputBG.GetLocalPosition()
			tmp = ui.MakeImageBox(self, ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png", 20, 65 + 3)
			tmp.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__ChangeExactSearch))
			self.exactSearchImg = tmp

			# (x, y) = self.exactSearchImg.GetLocalPosition()
			tmp = ui.TextLine()
			tmp.SetParent(self)
			tmp.SetText(localeinfo.V2_SHOP_EXACT_MATCH)
			tmp.SetPosition(40, 65 + 3)
			tmp.Show()
			self.exactSearchText = tmp

			self.ownerPid = 0
			self.resName = ""
			self.resVnum = -1

			self.isPlayerMatch = False
			# (x, y) = self.nameInputBG.GetLocalPosition()
			tmp = ui.MakeImageBox(self, ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png", 120, 45 + 6 + self.exactSearchImg.GetHeight() + 2)
			tmp.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__ChangeExactMatch))
			self.matchSearchImg = tmp

			# (x, y) = self.exactSearchImg.GetLocalPosition()
			tmp = ui.TextLine()
			tmp.SetParent(self)
			tmp.SetText(localeinfo.V2_SHOP_PLAYER_NAME)
			tmp.SetPosition(140, 45 + 6 + self.exactSearchImg.GetHeight() + 2)
			tmp.Show()
			self.matchSearchText = tmp

			#app.USE_PREMIUM_AFFECT
			self.isAscendingOrder = False

			if app.USE_PREMIUM_AFFECT:
				tmp = ui.MakeImageBox(self, ShopSearch.ROOT_PATH + "right_rectangle/uncheck.png", 20, 65 + 3 + 29)
				tmp.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__ChangeAscendingSearch))
				self.ascendingOrderImg = tmp

				tmp = ui.TextLine()
				tmp.SetParent(self)
				tmp.SetText(localeinfo.V2_SHOP_PRICE_ASCENDING)
				tmp.SetPosition(40, 65 + 3 + 29)
				tmp.Show()
				self.ascendingOrderText = tmp

			self.scrollBarWasShown = False
			self.filterStatus = False

			self.filterImg = ui.ImageBox()
			self.filterImg.LoadImage(ShopSearch.ROOT_PATH + "right_rectangle/filterbtn.png")

			# (x, y) = self.matchSearchImg.GetLocalPosition()
			tmp = ui.Window()
			tmp.SetParent(self)
			tmp.SetPosition(199, 12 + self.matchSearchImg.GetHeight() + 8)
			tmp.SetSize(self.filterImg.GetWidth(), self.filterImg.GetHeight())
			tmp.Show()
			self.filterBG = tmp

			self.filterImg.SetParent(self.filterBG)
			self.filterImg.SetPosition(0, 0)
			self.filterImg.Show()
			self.filterImg.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__OnToggleFilter))

			tmp = ui.TextLine()
			tmp.SetParent(self)
			tmp.SetPosition(135 * 2, -108 * 2)
			tmp.SetFontName("Verdana:14b")
			tmp.SetVerticalAlignCenter()
			tmp.SetWindowVerticalAlignCenter()
			tmp.SetText("Item")
			tmp.Show()
			self.itemText = tmp

			tmp = ui.TextLine()
			tmp.SetParent(self)
			tmp.SetPosition(245 * 2, -108 * 2)
			tmp.SetFontName("Verdana:14b")
			tmp.SetVerticalAlignCenter()
			tmp.SetWindowVerticalAlignCenter()
			tmp.SetText("Price")
			tmp.Show()
			self.priceText = tmp

			tmp = ui.TextLine()
			tmp.SetParent(self)
			tmp.SetPosition(340 * 2, -108 * 2)
			tmp.SetFontName("Verdana:14b")
			tmp.SetVerticalAlignCenter()
			tmp.SetWindowVerticalAlignCenter()
			tmp.SetText("Information")
			tmp.Show()
			self.informationText = tmp

			# tmp = ui.TextLine()
			# tmp.SetParent(self.filterBG)
			# tmp.SetText(localeinfo.V2_SHOP_FILTER)
			# tmp.SetPosition(0, 0)
			# tmp.SetVerticalAlignCenter()
			# tmp.SetWindowVerticalAlignCenter()
			# tmp.SetHorizontalAlignCenter()
			# tmp.SetWindowHorizontalAlignCenter()
			# tmp.Show()
			# self.filterText = tmp

			# (x, y) = self.filterBG.GetLocalPosition()
			# tmp = ui.MakeButton(self, x + self.filterBG.GetWidth() + 2, y, localeinfo.V2_SHOP_FILTER_CLOSE, ShopSearch.IMG_PATH + "filter/", "btn_close_normal.png", "btn_close_hover.png", "btn_close_down.png")
			# tmp.SetEvent(ui.__mem_func__(self.__CloseFilter))
			# self.filterCloseBtn = tmp

			(x, y) = self.content.GetLocalPosition()
			tmp = self.GetChild("buy_all_ui")
			tmp.SetPosition(x, y + self.content.GetHeight() + 5)
			tmp.SetEvent(ui.__mem_func__(self.__OnOpenQuestionDlg))
			self.buyAllBtn = tmp

			y = 18
			self.levelInputTitleImg = ui.MakeImageBox(self.scrollFilterWindow, ShopSearch.IMG_PATH + "filter/input_title_bar.png", 0, y)
			self.levelInputTitleImg.SetWindowHorizontalAlignCenter()
			self.levelInputTitleImg.SetDiffuseColor(ShopSearch.DIFFUSE_COLOR_IN[0], ShopSearch.DIFFUSE_COLOR_IN[1], ShopSearch.DIFFUSE_COLOR_IN[2], ShopSearch.DIFFUSE_COLOR_IN[3])

			tmp = ui.TextLine()
			tmp.SetParent(self.levelInputTitleImg)
			tmp.SetText(localeinfo.V2_SHOP_SEARCH_PRICE_RANGE)
			tmp.SetVerticalAlignCenter()
			tmp.SetWindowVerticalAlignCenter()
			tmp.SetHorizontalAlignCenter()
			tmp.SetWindowHorizontalAlignCenter()
			tmp.SetPosition(0, -7)
			tmp.Show()
			self.levelInputTitleText = tmp

			y += 10
			self.minPriceInputBG = ui.MakeImageBox(self.scrollFilterWindow, ShopSearch.IMG_PATH + "filter/bg_input_large.png", 0, y)
			self.minPriceInputBG.SetWindowHorizontalAlignCenter()

			tmp = ui.EditLine()
			tmp.SetParent(self.minPriceInputBG)
			tmp.SetMax(18)
			tmp.SetNumberMode()
			tmp.SetSize(self.minPriceInputBG.GetWidth() - 4, self.minPriceInputBG.GetHeight())
			tmp.SetLimitWidth(tmp.GetWidth() - 6)
			tmp.SetPosition(6, 4)
			tmp.SetOutline()
			tmp.SetEscapeEvent(ui.__mem_func__(self.OnPressEscapeKey))
			tmp.SetTabEvent(ui.__mem_func__(self.OnTabPriceInput))
			tmp.SetReturnEvent(ui.__mem_func__(self.__SearchFiltered))
			tmp.Show()
			self.priceMinInput = tmp

			y += self.minPriceInputBG.GetHeight() + 4
			self.maxPriceInputBG = ui.MakeImageBox(self.scrollFilterWindow, ShopSearch.IMG_PATH + "filter/bg_input_large.png", 0, y)
			self.maxPriceInputBG.SetWindowHorizontalAlignCenter()

			tmp = ui.EditLine()
			tmp.SetParent(self.maxPriceInputBG)
			tmp.SetMax(18)
			tmp.SetNumberMode()
			tmp.SetSize(self.maxPriceInputBG.GetWidth() - 4, self.maxPriceInputBG.GetHeight())
			tmp.SetLimitWidth(tmp.GetWidth() - 6)
			tmp.SetPosition(6, 4)
			tmp.SetOutline()
			tmp.SetEscapeEvent(ui.__mem_func__(self.OnPressEscapeKey))
			tmp.SetTabEvent(ui.__mem_func__(self.OnTabPriceInput))
			tmp.SetReturnEvent(ui.__mem_func__(self.__SearchFiltered))
			tmp.Show()
			self.priceMaxInput = tmp

			y += self.maxPriceInputBG.GetHeight() + 20 + 6
			self.priceInputTitleImg = ui.MakeImageBox(self.scrollFilterWindow, ShopSearch.IMG_PATH + "filter/input_title_bar.png", 0, y)
			self.priceInputTitleImg.SetWindowHorizontalAlignCenter()
			self.priceInputTitleImg.SetDiffuseColor(ShopSearch.DIFFUSE_COLOR_IN[0], ShopSearch.DIFFUSE_COLOR_IN[1], ShopSearch.DIFFUSE_COLOR_IN[2], ShopSearch.DIFFUSE_COLOR_IN[3])

			tmp = ui.TextLine()
			tmp.SetParent(self.priceInputTitleImg)
			tmp.SetText(localeinfo.V2_SHOP_SEARCH_LEVEL_RANGE)
			tmp.SetVerticalAlignCenter()
			tmp.SetWindowVerticalAlignCenter()
			tmp.SetHorizontalAlignCenter()
			tmp.SetWindowHorizontalAlignCenter()
			tmp.SetPosition(0, -7)
			tmp.Show()
			self.priceInputTitleText = tmp

			y += 10
			self.minLevelInputBG = ui.MakeImageBox(self.scrollFilterWindow, ShopSearch.IMG_PATH + "filter/bg_input_small.png", 30, y)
			tmp = ui.EditLine()
			tmp.SetParent(self.minLevelInputBG)
			tmp.SetMax(3)
			tmp.SetNumberMode()
			tmp.SetSize(self.minLevelInputBG.GetWidth() - 4, self.minLevelInputBG.GetHeight())
			tmp.SetLimitWidth(tmp.GetWidth() - 6)
			tmp.SetPosition(6, 4)
			tmp.SetOutline()
			tmp.SetEscapeEvent(ui.__mem_func__(self.OnPressEscapeKey))
			tmp.SetTabEvent(ui.__mem_func__(self.OnTabLevelInput))
			tmp.SetReturnEvent(ui.__mem_func__(self.__SearchFiltered))
			tmp.Show()
			self.levelMinInput = tmp

			self.maxLevelInputBG = ui.MakeImageBox(self.scrollFilterWindow, ShopSearch.IMG_PATH + "filter/bg_input_small.png", 100, y)
			tmp = ui.EditLine()
			tmp.SetParent(self.maxLevelInputBG)
			tmp.SetMax(3)
			tmp.SetNumberMode()
			tmp.SetSize(self.maxLevelInputBG.GetWidth() - 4, self.maxLevelInputBG.GetHeight())
			tmp.SetLimitWidth(tmp.GetWidth() - 6)
			tmp.SetPosition(6, 4)
			tmp.SetOutline()
			tmp.SetEscapeEvent(ui.__mem_func__(self.OnPressEscapeKey))
			tmp.SetTabEvent(ui.__mem_func__(self.OnTabLevelInput))
			tmp.SetReturnEvent(ui.__mem_func__(self.__SearchFiltered))
			tmp.Show()
			self.levelMaxInput = tmp

			y += 42 + 6

			self.otherInputTitleImg = ui.MakeImageBox(self.scrollFilterWindow, ShopSearch.IMG_PATH + "filter/input_title_bar.png", 0, y)
			self.otherInputTitleImg.SetWindowHorizontalAlignCenter()
			self.otherInputTitleImg.SetDiffuseColor(ShopSearch.DIFFUSE_COLOR_IN[0], ShopSearch.DIFFUSE_COLOR_IN[1], ShopSearch.DIFFUSE_COLOR_IN[2], ShopSearch.DIFFUSE_COLOR_IN[3])

			tmp = ui.TextLine()
			tmp.SetParent(self.otherInputTitleImg)
			tmp.SetText(localeinfo.V2_SHOP_SEARCH_OTHERS)
			tmp.SetVerticalAlignCenter()
			tmp.SetWindowVerticalAlignCenter()
			tmp.SetHorizontalAlignCenter()
			tmp.SetWindowHorizontalAlignCenter()
			tmp.SetPosition(0, -7)
			tmp.Show()
			self.otherInputTitleText = tmp

			y += 10
			self.filterWarriorBtn = ui.MakeToggleButton(self.scrollFilterWindow, 18, y, "", "", ShopSearch.ROOT_PATH + "left_rectangle/filter_window/race_select/", "warrior_norm.png", "warrior_down.png", "warrior_over.png")
			self.filterWarriorBtn.SetToggleUpEvent(ui.__mem_func__(self.OnFilterToggleButtonDown), self.filterWarriorBtn)
			self.filterWarriorBtn.SetToggleDownEvent(ui.__mem_func__(self.OnFilterToggleButtonSetUp), self.filterWarriorBtn)

			w = self.filterWarriorBtn.GetWidth()
			h = self.filterWarriorBtn.GetHeight()

			self.filterAssassinBtn = ui.MakeToggleButton(self.scrollFilterWindow, 18 + w + 2, y, "", "", ShopSearch.ROOT_PATH + "left_rectangle/filter_window/race_select/", "assassin_norm.png", "assassin_down.png", "assassin_over.png")
			self.filterAssassinBtn.SetToggleUpEvent(ui.__mem_func__(self.OnFilterToggleButtonDown), self.filterAssassinBtn)
			self.filterAssassinBtn.SetToggleDownEvent(ui.__mem_func__(self.OnFilterToggleButtonSetUp), self.filterAssassinBtn)

			self.filterSuraBtn = ui.MakeToggleButton(self.scrollFilterWindow, 18 + (w * 2) + (2 * 2), y, "", "", ShopSearch.ROOT_PATH + "left_rectangle/filter_window/race_select/", "sura_norm.png", "sura_down.png", "sura_over.png")
			self.filterSuraBtn.SetToggleUpEvent(ui.__mem_func__(self.OnFilterToggleButtonDown), self.filterSuraBtn)
			self.filterSuraBtn.SetToggleDownEvent(ui.__mem_func__(self.OnFilterToggleButtonSetUp), self.filterSuraBtn)

			self.filterShamanBtn = ui.MakeToggleButton(self.scrollFilterWindow, 18 + (w * 3) + (2 * 3), y, "", "", ShopSearch.ROOT_PATH + "left_rectangle/filter_window/race_select/", "shaman_norm.png", "shaman_down.png", "shaman_over.png")
			self.filterShamanBtn.SetToggleUpEvent(ui.__mem_func__(self.OnFilterToggleButtonDown), self.filterShamanBtn)
			self.filterShamanBtn.SetToggleDownEvent(ui.__mem_func__(self.OnFilterToggleButtonSetUp), self.filterShamanBtn)

			self.filterMaleBtn = ui.MakeToggleButton(self.scrollFilterWindow, 0, 0, "", localeinfo.V2_SHOP_SEARCH_TOOLTIP_MALE, ShopSearch.ROOT_PATH + "left_rectangle/filter_window/gender_select/", "man_norm.png", "man_over.png", "man_down.png")
			self.filterMaleBtn.SetWindowHorizontalAlignCenter()
			self.filterMaleBtn.SetPosition(-14, y + h + 4)
			self.filterMaleBtn.SetToggleUpEvent(ui.__mem_func__(self.OnFilterToggleButtonDown), self.filterMaleBtn)
			self.filterMaleBtn.SetToggleDownEvent(ui.__mem_func__(self.OnFilterToggleButtonSetUp), self.filterMaleBtn)

			self.filterFemaleBtn = ui.MakeToggleButton(self.scrollFilterWindow, 0, 0, "", localeinfo.V2_SHOP_SEARCH_TOOLTIP_FEMALE, ShopSearch.ROOT_PATH + "left_rectangle/filter_window/gender_select/", "woman_norm.png", "woman_over.png", "woman_down.png")
			self.filterFemaleBtn.SetWindowHorizontalAlignCenter()
			self.filterFemaleBtn.SetPosition(14, y + h + 4)
			self.filterFemaleBtn.SetToggleUpEvent(ui.__mem_func__(self.OnFilterToggleButtonDown), self.filterFemaleBtn)
			self.filterFemaleBtn.SetToggleDownEvent(ui.__mem_func__(self.OnFilterToggleButtonSetUp), self.filterFemaleBtn)

			tmp = ui.MakeButton(self.scrollFilterWindow, 0, self.scrollFilterWindow.GetHeight() - 60, "", ShopSearch.IMG_PATH + "filter/", "btn_filter_normal.png", "btn_filter_hover.png", "btn_filter_down.png")
			tmp.SetText(localeinfo.V2_SHOP_INPUT_SEARCH)
			tmp.SetWindowHorizontalAlignCenter()
			tmp.SetEvent(ui.__mem_func__(self.__SearchFiltered))
			self.filterSearchBtn = tmp

			tmp = ui.MakeButton(self.scrollFilterWindow, 0, self.scrollFilterWindow.GetHeight() - 32, "", ShopSearch.IMG_PATH + "filter/", "btn_filter_normal.png", "btn_filter_hover.png", "btn_filter_down.png")
			tmp.SetText(localeinfo.V2_SHOP_INPUT_RESET)
			tmp.SetWindowHorizontalAlignCenter()
			tmp.SetEvent(ui.__mem_func__(self.__ResetFilter))
			self.filterResetBtn = tmp
		except Exception as e:
			import dbg
			dbg.TraceError("ShopSearch.__Load: {}".format(e))
			app.Abort()

	def __init__(self, interfaceInst):
		self.interfaceInst = interfaceInst

		self.nameInput = None
		self.nameInputHint = None

		self.priceMinInput = None
		self.priceMaxInput = None
		self.levelMinInput = None
		self.levelMaxInput = None

		self.dlgBuy = None
		self.dlgQuestion = None
		self.itemToolTip = None
		self.toolTip = None

		self.selectedDict = {}

		ui.ScriptWindow.__init__(self)
		self.__Load()

	def Destroy(self):
		constInfo.changeShopSearchTime = 0

		self.interfaceInst = None

		if self.priceMinInput:
			if self.priceMinInput.IsFocus():
				self.priceMinInput.KillFocus()

			self.priceMinInput.SetEscapeEvent(None)
			self.priceMinInput.SetTabEvent(None)
			self.priceMinInput.SetReturnEvent(None)
			del self.priceMinInput
			self.priceMinInput = None

		if self.priceMaxInput:
			if self.priceMaxInput.IsFocus():
				self.priceMaxInput.KillFocus()

			self.priceMaxInput.SetEscapeEvent(None)
			self.priceMaxInput.SetTabEvent(None)
			self.priceMaxInput.SetReturnEvent(None)
			del self.priceMaxInput
			self.priceMaxInput = None

		if self.levelMinInput:
			if self.levelMinInput.IsFocus():
				self.levelMinInput.KillFocus()

			self.levelMinInput.SetEscapeEvent(None)
			self.levelMinInput.SetTabEvent(None)
			self.levelMinInput.SetReturnEvent(None)
			del self.levelMinInput
			self.levelMinInput = None

		if self.levelMaxInput:
			if self.levelMaxInput.IsFocus():
				self.levelMaxInput.KillFocus()

			self.levelMaxInput.SetEscapeEvent(None)
			self.levelMaxInput.SetTabEvent(None)
			self.levelMaxInput.SetReturnEvent(None)
			del self.levelMaxInput
			self.levelMaxInput = None

		if self.otherInputTitleText:
			del self.otherInputTitleText
			self.otherInputTitleText = None

		if self.otherInputTitleImg:
			del self.otherInputTitleImg
			self.otherInputTitleImg = None

		if self.levelInputTitleText:
			del self.levelInputTitleText
			self.levelInputTitleText = None

		if self.levelInputTitleImg:
			del self.levelInputTitleImg
			self.levelInputTitleImg = None

		if self.priceInputTitleText:
			del self.priceInputTitleText
			self.priceInputTitleText = None

		if self.priceInputTitleImg:
			del self.priceInputTitleImg
			self.priceInputTitleImg = None

		if self.filterSearchBtn:
			self.filterSearchBtn.SetEvent(None)
			del self.filterSearchBtn
			self.filterSearchBtn = None

		if self.filterResetBtn:
			self.filterResetBtn.SetEvent(None)
			del self.filterResetBtn
			self.filterResetBtn = None

		if self.filterAssassinBtn:
			self.filterAssassinBtn.SetToggleUpEvent(None)
			self.filterAssassinBtn.SetToggleDownEvent(None)
			del self.filterAssassinBtn
			self.filterAssassinBtn = None

		if self.filterSuraBtn:
			self.filterSuraBtn.SetToggleUpEvent(None)
			self.filterSuraBtn.SetToggleDownEvent(None)
			del self.filterSuraBtn
			self.filterSuraBtn = None

		if self.filterShamanBtn:
			self.filterShamanBtn.SetToggleUpEvent(None)
			self.filterShamanBtn.SetToggleDownEvent(None)
			del self.filterShamanBtn
			self.filterShamanBtn = None

		if self.filterMaleBtn:
			self.filterMaleBtn.SetToggleUpEvent(None)
			self.filterMaleBtn.SetToggleDownEvent(None)
			del self.filterMaleBtn
			self.filterMaleBtn = None

		if self.filterFemaleBtn:
			self.filterFemaleBtn.SetToggleUpEvent(None)
			self.filterFemaleBtn.SetToggleDownEvent(None)
			del self.filterFemaleBtn
			self.filterFemaleBtn = None

		if self.buyAllBtn:
			self.buyAllBtn.SetEvent(None)
			self.buyAllBtn = None

		# if self.filterCloseBtn:
			# self.filterCloseBtn.SetEvent(None)
			# del self.filterCloseBtn
			# self.filterCloseBtn = None

		# if self.filterText:
			# del self.filterText
			# self.filterText = None

		if self.filterImg:
			self.filterImg.SetOnMouseLeftButtonUpEvent(None)
			del self.filterImg
			self.filterImg = None

		if self.filterBG:
			del self.filterBG
			self.filterBG = None

		if self.matchSearchImg:
			self.matchSearchImg.SetOnMouseLeftButtonUpEvent(None)
			del self.matchSearchImg
			self.matchSearchImg = None

		if self.matchSearchText:
			del self.matchSearchText
			self.matchSearchText = None

		if self.exactSearchText:
			del self.exactSearchText
			self.exactSearchText = None

		if self.exactSearchImg:
			self.exactSearchImg.SetOnMouseLeftButtonUpEvent(None)
			del self.exactSearchImg
			self.exactSearchImg = None

		if app.USE_PREMIUM_AFFECT:
			if self.ascendingOrderText:
				del self.ascendingOrderText
				self.ascendingOrderText = None

			if self.ascendingOrderImg:
				self.ascendingOrderImg.SetOnMouseLeftButtonUpEvent(None)
				del self.ascendingOrderImg
				self.ascendingOrderImg = None

		if self.btnSearch:
			self.btnSearch.SetEvent(None)
			del self.btnSearch
			self.btnSearch = None

		if self.resetSearch:
			self.resetSearch.SetEvent(None)
			del self.resetSearch
			self.resetSearch = None

		if self.nameInputHint:
			del self.nameInputHint
			self.nameInputHint = None

		if self.nameInput:
			if self.nameInput.IsFocus():
				self.nameInput.KillFocus()

			self.nameInput.SetEscapeEvent(None)
			self.nameInput.SetUpdateEvent(None)
			self.nameInput.SetTabEvent(None)
			self.nameInput.SetReturnEvent(None)
			del self.nameInput
			self.nameInput = None

		if self.nameInputBG:
			del self.nameInputBG
			self.nameInputBG = None

		if self.dlgBuy:
			self.dlgBuy.Destroy()
			del self.dlgBuy
			self.dlgBuy = None

		if self.dlgQuestion:
			del self.dlgQuestion
			self.dlgQuestion = None

		self.emptyContentText = None
		self.loadingContentAni = None

		for i in self.itemsList:
			del i

		self.itemsList = []

		if self.itemsContainer:
			del self.itemsContainer
			self.itemsContainer = None

		if self.scrollContentWindow:
			del self.scrollContentWindow
			self.scrollContentWindow = None

		if self.scrollBar:
			del self.scrollBar
			self.scrollBar = None

		if self.scrollBarItem:
			del self.scrollBarItem
			self.scrollBarItem = None

		if self.categoryContainer:
			del self.categoryContainer
			self.categoryContainer = None

		for k, v in self.subCategoriesDict.items():
			for l in v:
				l[1].SetEvent(None)
				del l[1]

		self.subCategoriesDict = {}

		if self.scrollCatWindow:
			del self.scrollCatWindow
			self.scrollCatWindow = None

		if self.scrollFilterWindow:
			del self.scrollFilterWindow
			self.scrollFilterWindow = None

		self.categoriesList = []

		for v in self.pageButtonsList:
			v.SetEvent(None)

		self.pageButtonsList = []

		if self.firstPageButton:
			self.firstPageButton.SetEvent(None)
			self.firstPageButton = None

		if self.prevPageButton:
			self.prevPageButton.SetEvent(None)
			self.prevPageButton = None

		if self.nextPageButton:
			self.nextPageButton.SetEvent(None)
			self.nextPageButton = None

		if self.lastPageButton:
			self.lastPageButton.SetEvent(None)
			self.lastPageButton = None

		if self.btnBackGround:
			del self.btnBackGround
			self.btnBackGround = None

		if self.itemToolTip:
			del self.itemToolTip
			self.itemToolTip = None

		if self.toolTip:
			del self.toolTip
			self.toolTip = None

		self.menu = None
		self.content = None

		if self.board:
			self.board.SetCloseEvent(None)
			self.board = None

		self.ClearDictionary()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
