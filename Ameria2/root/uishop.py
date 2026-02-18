# coding: latin_1

import net
import player
import item
import snd
import shop
import net
import wndMgr
import app
import chat
import chr
import ui
import uiCommon
import mouseModule
import localeInfo
import constInfo
import dbg
import extern_wa_shopitem

import uiShopRender

###################################################################################################
## Shop
class ShopDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.tooltipItem = 0
		self.xShopStart = 0
		self.yShopStart = 0
		self.questionDialog = None
		self.popup = None
		self.vid = 0
		self.itemBuyQuestionDialog = None
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.interface = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)
	
	def __GetRealIndex(self, i):
		return self.tabIdx * shop.SHOP_SLOT_COUNT + i
	
	def Refresh(self):
		getItemID = shop.GetItemID
		getItemCount = shop.GetItemCount
		setItemID = self.itemSlotWindow.SetItemSlot
		for i in xrange(shop.SHOP_SLOT_COUNT):
			idx = self.__GetRealIndex(i)
			itemCount = getItemCount(idx)
			if itemCount <= 1:
				itemCount = 0
			setItemID(i, getItemID(idx), itemCount)

		wndMgr.RefreshSlot(self.itemSlotWindow.GetWindowHandle())

	def SetItemData(self, pos, itemID, itemCount, itemPrice):
		shop.SetItemData(pos, itemID, itemCount, itemPrice)

	def LoadDialog(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/shopdialog.py")
		except:
			import exception
			exception.Abort("ShopDialog.LoadDialog.LoadObject")

		smallTab1 = None
		smallTab2 = None
		smallTab3 = None
		middleTab1 = None
		middleTab2 = None
		
		try:
			GetObject = self.GetChild
			self.itemSlotWindow = GetObject("ItemSlot")
			self.btnBuy = GetObject("BuyButton")
			self.btnSell = GetObject("SellButton")
			self.btnClose = GetObject("CloseButton")
			self.titleBar = GetObject("TitleBar")
			middleTab1 = GetObject("MiddleTab1")
			middleTab2 = GetObject("MiddleTab2")
			smallTab1 = GetObject("SmallTab1")
			smallTab2 = GetObject("SmallTab2")
			smallTab3 = GetObject("SmallTab3")
			
			if (app.WJ_COMBAT_ZONE):
				self.boardBattleShop = GetObject("BattleShopSubBoard")
				self.curPoints = GetObject("BattleShopSubInfo1")
				self.curLimitPoints = GetObject("BattleShopSubInfo2")
			else:
				GetObject("BattleShopSubBoard").Hide()
		except:
			import exception
			exception.Abort("ShopDialog.LoadDialog.BindObject")

		self.itemSlotWindow.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		self.itemSlotWindow.SAFE_SetButtonEvent("LEFT", "EMPTY", self.SelectEmptySlot)
		self.itemSlotWindow.SAFE_SetButtonEvent("LEFT", "EXIST", self.SelectItemSlot)
		self.itemSlotWindow.SAFE_SetButtonEvent("RIGHT", "EXIST", self.UnselectItemSlot)

		self.itemSlotWindow.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.itemSlotWindow.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		self.btnBuy.SetToggleUpEvent(ui.__mem_func__(self.CancelShopping))
		self.btnBuy.SetToggleDownEvent(ui.__mem_func__(self.OnBuy))

		self.btnSell.SetToggleUpEvent(ui.__mem_func__(self.CancelShopping))
		self.btnSell.SetToggleDownEvent(ui.__mem_func__(self.OnSell))

		self.shopRender = uiShopRender.ShopRender()

		self.btnClose.SetEvent(ui.__mem_func__(self.AskClosePrivateShop))

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.smallRadioButtonGroup = ui.RadioButtonGroup.Create([[smallTab1, lambda : self.OnClickTabButton(0), None], [smallTab2, lambda : self.OnClickTabButton(1), None], [smallTab3, lambda : self.OnClickTabButton(2), None]])
		self.middleRadioButtonGroup = ui.RadioButtonGroup.Create([[middleTab1, lambda : self.OnClickTabButton(0), None], [middleTab2, lambda : self.OnClickTabButton(1), None]])

		self.__HideMiddleTabs()
		self.__HideSmallTabs()
		
		self.tabIdx = 0
		self.coinType = shop.SHOP_COIN_TYPE_GOLD
		
		self.Refresh()
	
	def __ShowBuySellButton(self):
		self.btnBuy.Show()
		self.btnSell.Show()
		
	def __ShowMiddleTabs(self):
		self.middleRadioButtonGroup.Show()
	
	def __ShowSmallTabs(self):
		self.smallRadioButtonGroup.Show()
	
	def __HideBuySellButton(self):
		self.btnBuy.Hide()
		self.btnSell.Hide()
	
	def __HideMiddleTabs(self):
		self.middleRadioButtonGroup.Hide()
	
	def __HideSmallTabs(self):
		self.smallRadioButtonGroup.Hide()
		
	def __SetTabNames(self):
		if shop.GetTabCount() == 2:
			self.middleRadioButtonGroup.SetText(0, shop.GetTabName(0))
			self.middleRadioButtonGroup.SetText(1, shop.GetTabName(1))
		elif shop.GetTabCount() == 3:
			self.smallRadioButtonGroup.SetText(0, shop.GetTabName(0))
			self.smallRadioButtonGroup.SetText(1, shop.GetTabName(1))
			self.smallRadioButtonGroup.SetText(2, shop.GetTabName(2))

	def Destroy(self):
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.Close(True)
			self.interface = None
		else:
			self.Close()

		self.ClearDictionary()

		self.tooltipItem = 0
		self.itemSlotWindow = 0
		self.btnBuy = 0
		self.vid = 0
		self.btnSell = 0
		self.btnClose = 0
		self.titleBar = 0
		if (app.WJ_COMBAT_ZONE):
			self.boardBattleShop = 0
			self.curPoints = 0
			self.curLimitPoints = 0
		self.questionDialog = None
		self.popup = None
		
	if (app.WJ_COMBAT_ZONE):
		def SetCombatZoneBoard(self, flag):
			if flag:
				self.boardBattleShop.Show()
			else:
				self.boardBattleShop.Hide()
	
		def SetCombatZonePoints(self, points):
			self.curPoints.SetText(localeInfo.COMBAT_ZONE_CURRENT_REAL_POINTS % points)
			
		def SetLimitCombatZonePoints(self, curLimit, maxLimit):
			self.curLimitPoints.SetText(localeInfo.COMBAT_ZONE_LIMIT_SHOP_POINTS % (curLimit, maxLimit))

	if (app.WJ_COMBAT_ZONE):
		def Open(self, vid, points, curLimit, maxLimit):
			self.vid=int(vid)

			self.btnBuy.Show()
			self.btnSell.Show()
			self.btnClose.Hide()
			shop.Open()
			self.tabIdx = 0
			self.Refresh()
			self.SetTop()

			if (app.WJ_COMBAT_ZONE):
				if shop.SHOP_COIN_TYPE_COMBAT_ZONE == shop.GetTabCoinType(self.tabIdx):
					self.SetCombatZonePoints(points)
					self.SetLimitCombatZonePoints(curLimit, maxLimit)
					self.SetCombatZoneBoard(True)
				else:
					self.SetCombatZoneBoard(False)

			self.Show()

			(self.xShopStart, self.yShopStart, z) = player.GetMainCharacterPosition()

			if app.WJ_ENABLE_TRADABLE_ICON:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_SHOP)
				self.interface.RefreshMarkInventoryBag()

			if vid == 99:
				self.shopRender.Open(0)
				(x,y) = self.GetGlobalPosition()
				self.shopRender.SetPosition(x-self.shopRender.GetWidth()-5,y)

	else:
		def Open(self, vid):
			isPrivateShop = FALSE
			isMainPlayerPrivateShop = FALSE
			if chr.IsNPC(vid):
				isPrivateShop = FALSE
			else:
				isPrivateShop = TRUE
			if player.IsMainCharacterIndex(vid):
				isMainPlayerPrivateShop = TRUE
				self.btnBuy.Hide()
				self.btnSell.Hide()
				self.btnClose.Show()
			else:
				isMainPlayerPrivateShop = FALSE
				self.btnBuy.Show()
				self.btnSell.Show()
				self.btnClose.Hide()
			shop.Open(isPrivateShop, isMainPlayerPrivateShop)
			self.tabIdx = 0
			if isPrivateShop:
				self.__HideMiddleTabs()
				self.__HideSmallTabs()
			else:
				if shop.GetTabCount() == 1:
					self.__ShowBuySellButton()
					self.__HideMiddleTabs()
					self.__HideSmallTabs()
				elif shop.GetTabCount() == 2:
					self.__HideBuySellButton()
					self.__ShowMiddleTabs()
					self.__HideSmallTabs()
					self.__SetTabNames()
					self.middleRadioButtonGroup.OnClick(0)
				elif shop.GetTabCount() == 3:
					self.__HideBuySellButton()
					self.__HideMiddleTabs()
					self.__ShowSmallTabs()
					self.__SetTabNames()
					self.middleRadioButtonGroup.OnClick(1)
			self.Refresh()
			self.SetTop()
			self.Show()

			(self.xShopStart, self.yShopStart, z) = player.GetMainCharacterPosition()

			if app.WJ_ENABLE_TRADABLE_ICON:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_SHOP)
				self.interface.RefreshMarkInventoryBag()

			if vid == 99:
				self.shopRender.Open(0)
				(x,y) = self.GetGlobalPosition()
				self.shopRender.SetPosition(x-self.shopRender.GetWidth()-5,y)


	if app.WJ_ENABLE_TRADABLE_ICON:
		def Close(self, isDestroy=False):
			self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
			if not isDestroy:
				self.interface.RefreshMarkInventoryBag()

			if self.itemBuyQuestionDialog:
				self.itemBuyQuestionDialog.Close()
				self.itemBuyQuestionDialog = None
				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

			self.OnCloseQuestionDialog()
			shop.Close()
			net.SendShopEndPacket()
			self.CancelShopping()
			self.tooltipItem.HideToolTip()
			# if app.ENABLE_BATTLE_PASS:
			self.shopRender.Close()
			self.Hide()
			
	else:
		def Close(self):
			if self.itemBuyQuestionDialog:
				self.itemBuyQuestionDialog.Close()
				self.itemBuyQuestionDialog = None
				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

			self.OnCloseQuestionDialog()
			shop.Close()
			net.SendShopEndPacket()
			self.CancelShopping()
			self.tooltipItem.HideToolTip()
			# if app.ENABLE_BATTLE_PASS:
			self.shopRender.Close()
			self.Hide()

	def GetIndexFromSlotPos(self, slotPos):
		return self.tabIdx * shop.SHOP_SLOT_COUNT + slotPos
		
	def OnClickTabButton(self, idx):
		self.tabIdx = idx
		self.Refresh()
		
	def AskClosePrivateShop(self):
		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.PRIVATE_SHOP_CLOSE_QUESTION)
		questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnClosePrivateShop))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog

		return TRUE

	def OnClosePrivateShop(self):
		net.SendChatPacket("/close_shop "+str(self.vid))
		self.OnCloseQuestionDialog()
		return True

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

	def OnPressExitKey(self):
		self.Close()
		return TRUE

	def OnBuy(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_BUY_INFO)
		app.SetCursor(app.BUY)
		self.btnSell.SetUp()

	def OnSell(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_SELL_INFO)
		app.SetCursor(app.SELL)
		self.btnBuy.SetUp()

	def CancelShopping(self):
		self.btnBuy.SetUp()
		self.btnSell.SetUp()
		app.SetCursor(app.NORMAL)

	def __OnClosePopupDialog(self):
		self.pop = None

	def SellAttachedItem(self):
		if (app.WJ_COMBAT_ZONE):
			if shop.SHOP_COIN_TYPE_COMBAT_ZONE == shop.GetTabCoinType(self.tabIdx):
				mouseModule.mouseController.DeattachObject()
				return

		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedCount = mouseModule.mouseController.GetAttachedItemCount()
		if app.ENABLE_SPECIAL_STORAGE:
			if player.SLOT_TYPE_INVENTORY == attachedSlotType or\
				player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedSlotType or\
				player.SLOT_TYPE_UPGRADE_INVENTORY == attachedSlotType or\
				player.SLOT_TYPE_BOOK_INVENTORY == attachedSlotType or\
				player.SLOT_TYPE_CHANGE_INVENTORY == attachedSlotType or\
				player.SLOT_TYPE_COSTUME_INVENTORY == attachedSlotType or\
				player.SLOT_TYPE_STONE_INVENTORY == attachedSlotType or\
				player.SLOT_TYPE_CHEST_INVENTORY == attachedSlotType:
				
				attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
	
				itemIndex = player.GetItemIndex(attachedInvenType, attachedSlotPos)
				attachedCount = player.GetItemCount(attachedInvenType, attachedSlotPos)
				if itemIndex <= 0:
					return
				item.SelectItem(itemIndex)
	
				if item.IsAntiFlag(item.ANTIFLAG_SELL):
					popup = uiCommon.PopupDialog()
					popup.SetText(localeInfo.SHOP_CANNOT_SELL_ITEM)
					popup.SetAcceptEvent(self.__OnClosePopupDialog)
					popup.Open()
					self.popup = popup
					return
	
				itemtype = player.SlotTypeToInvenType(attachedSlotType)
	
				if player.IsValuableItem(itemtype, attachedSlotPos):
	
					itemPrice = item.GetISellItemPrice()
	
					if item.Is1GoldItem():
						#itemPrice = attachedCount / itemPrice / 5
						itemPrice = attachedCount / itemPrice
					else:
						#itemPrice = itemPrice * max(1, attachedCount) / 5
						itemPrice = itemPrice * max(1, attachedCount)
	
					itemName = item.GetItemName()
	
					questionDialog = uiCommon.QuestionDialog()
					questionDialog.SetText(localeInfo.DO_YOU_SELL_ITEM(itemName, attachedCount, itemPrice))
	
					questionDialog.SetAcceptEvent(lambda arg5 = attachedInvenType,arg1=attachedSlotPos, arg2=attachedCount: self.OnSellItem(arg5,arg1, arg2))
					questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
					questionDialog.Open()
					self.questionDialog = questionDialog
	
					#constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
	
				else:
					self.OnSellItem(attachedInvenType, attachedSlotPos, attachedCount)
			else:
				snd.PlaySound("sound/ui/loginfail.wav")
		else:
			if player.SLOT_TYPE_INVENTORY == attachedSlotType or player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedSlotType:
	
				itemIndex = player.GetItemIndex(attachedSlotPos)
				item.SelectItem(itemIndex)
				
				if item.IsAntiFlag(item.ANTIFLAG_SELL):
					popup = uiCommon.PopupDialog()
					popup.SetText(localeInfo.SHOP_CANNOT_SELL_ITEM)
					popup.SetAcceptEvent(self.__OnClosePopupDialog)
					popup.Open()
					self.popup = popup
					return
	
				itemtype = player.INVENTORY
	
				if player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedSlotType:
					itemtype = player.DRAGON_SOUL_INVENTORY

				if player.IsValuableItem(itemtype, attachedSlotPos):
	
					itemPrice = item.GetISellItemPrice()
	
					if item.Is1GoldItem():
						#itemPrice = attachedCount / itemPrice / 5
						itemPrice = attachedCount / itemPrice
					else:
						#itemPrice = itemPrice * max(1, attachedCount) / 5
						itemPrice = itemPrice * max(1, attachedCount)
	
					itemName = item.GetItemName()
	
					questionDialog = uiCommon.QuestionDialog()
					questionDialog.SetText(localeInfo.DO_YOU_SELL_ITEM(itemName, attachedCount, itemPrice))
	
					questionDialog.SetAcceptEvent(lambda arg1=attachedSlotPos, arg2=attachedCount, arg3 = itemtype: self.OnSellItem(arg1, arg2, arg3))
					questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
					questionDialog.Open()
					self.questionDialog = questionDialog
	
					#constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
	
				else:
					self.OnSellItem(attachedSlotPos, attachedCount, itemtype)
			else:
				snd.PlaySound("sound/ui/loginfail.wav")

		mouseModule.mouseController.DeattachObject()

	if app.ENABLE_SPECIAL_STORAGE:
		def OnSellItem(self, slotInv, slotPos, count):
			net.SendShopSellPacketNew(slotInv,slotPos, count)
			snd.PlaySound("sound/ui/money.wav")
			self.OnCloseQuestionDialog()
	else:
		def OnSellItem(self, slotPos, count):
			net.SendShopSellPacketNew(slotPos, count)
			snd.PlaySound("sound/ui/money.wav")
			self.OnCloseQuestionDialog()

	def OnCloseQuestionDialog(self):
		if self.questionDialog:
			self.questionDialog.Close()

		self.questionDialog = None

	def SelectEmptySlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			self.SellAttachedItem()

	def UnselectItemSlot(self, selectedSlotPos):
		if (app.WJ_COMBAT_ZONE):
			if shop.SHOP_COIN_TYPE_COMBAT_ZONE == shop.GetTabCoinType(self.tabIdx):
				self.AskBuyItem(selectedSlotPos)
			else:
				#if self.ISSHOPOFFLINE:
				#self.AskBuyItem(self.__GetRealIndex(selectedSlotPos)) #ask to buy in offshop
				#else:
				net.SendShopBuyPacket(self.__GetRealIndex(selectedSlotPos))		
		else:
			#if self.ISSHOPOFFLINE:
				# self.AskBuyItem(self.__GetRealIndex(selectedSlotPos)) #ask to buy in offshop
		#	else:
			net.SendShopBuyPacket(self.__GetRealIndex(selectedSlotPos))				

	def SelectItemSlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		selectedSlotPos = self.__GetRealIndex(selectedSlotPos)
		if isAttached:
			self.SellAttachedItem()

		else:
			#if TRUE == shop.IsMainPlayerPrivateShop():
			#	return

			curCursorNum = app.GetCursor()
			if app.BUY == curCursorNum:
				#self.AskBuyItem(selectedSlotPos)
				net.SendShopBuyPacket(selectedSlotPos)
			elif app.IsCanOpenRender() and app.ENABLE_RENDER_TARGET:
				interface = constInfo.GetInterfaceInstance()
				if interface:
					interface.OpenRenderTargetWindow(0, shop.GetItemID(selectedSlotPos))
				return
			elif app.SELL == curCursorNum:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_SELL_INFO)

			else:
				selectedItemID = shop.GetItemID(selectedSlotPos)
				itemCount = shop.GetItemCount(selectedSlotPos)

				type = player.SLOT_TYPE_SHOP

				mouseModule.mouseController.AttachObject(self, type, selectedSlotPos, selectedItemID, itemCount)
				mouseModule.mouseController.SetCallBack("INVENTORY", ui.__mem_func__(self.DropToInventory))
				snd.PlaySound("sound/ui/pick.wav")

	def DropToInventory(self):
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		self.AskBuyItem(attachedSlotPos)
	
	def AskBuyItem(self, slotPos):
		slotPos = self.__GetRealIndex(slotPos)
		
		itemIndex = shop.GetItemID(slotPos)
		itemPrice = shop.GetItemPrice(slotPos)
		itemCount = shop.GetItemCount(slotPos)

		item.SelectItem(itemIndex)
		itemName = item.GetItemName()

		itemBuyQuestionDialog = uiCommon.QuestionDialog()
		if app.ENABLE_BUY_WITH_ITEM:
			itemBuyQuestionDialog.SetText(localeInfo.DO_YOU_BUY_ITEM_NORMAL % (itemCount, itemName))
		else:
			itemBuyQuestionDialog.SetText(localeInfo.DO_YOU_BUY_ITEM(itemName, itemCount, localeInfo.NumberToMoneyString(itemPrice)))
		
		itemBuyQuestionDialog.SetAcceptEvent(lambda arg=TRUE: self.AnswerBuyItem(arg))
		itemBuyQuestionDialog.SetCancelEvent(lambda arg=FALSE: self.AnswerBuyItem(arg))
		itemBuyQuestionDialog.Open()
		itemBuyQuestionDialog.pos = slotPos
		self.itemBuyQuestionDialog = itemBuyQuestionDialog

	def AnswerBuyItem(self, flag):
		if flag:
			pos = self.itemBuyQuestionDialog.pos
			net.SendShopBuyPacket(pos)

		self.itemBuyQuestionDialog.Close()
		self.itemBuyQuestionDialog = None

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	if (app.WJ_COMBAT_ZONE):
		def OverInItem(self, slotIndex):
			slotIndex = self.__GetRealIndex(slotIndex)
			if mouseModule.mouseController.isAttached():
				return
			if 0 != self.tooltipItem:
				if shop.SHOP_COIN_TYPE_GOLD == shop.GetTabCoinType(self.tabIdx):
					self.tooltipItem.SetShopItem(slotIndex)
				elif shop.SHOP_COIN_TYPE_COMBAT_ZONE == shop.GetTabCoinType(self.tabIdx):
					self.tooltipItem.SetShopItemByCombatZoneCoin(slotIndex)
				else:
					self.tooltipItem.SetShopItemBySecondaryCoin(slotIndex)
				

				if self.shopRender.IsShow():
					self.shopRender.Open(shop.GetItemID(slotIndex))
	else:
		def OverInItem(self, slotIndex):
			slotIndex = self.__GetRealIndex(slotIndex)
			if mouseModule.mouseController.isAttached():
				return
			if 0 != self.tooltipItem:
				if shop.SHOP_COIN_TYPE_GOLD == shop.GetTabCoinType(self.tabIdx):
					self.tooltipItem.SetShopItem(slotIndex)
				else:
					self.tooltipItem.SetShopItemBySecondaryCoin(slotIndex)
				
				if self.shopRender.IsShow():
					self.shopRender.Open(shop.GetItemID(slotIndex))
					
	def OverOutItem(self):
		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	if app.WJ_ENABLE_TRADABLE_ICON:
		def BindInterface(self, interface):
			self.interface = interface

		def OnTop(self):
			self.interface.SetOnTopWindow(player.ON_TOP_WND_SHOP)
			self.interface.RefreshMarkInventoryBag()


	def OnMoveWindow(self, x, y):
		self.shopRender.SetPosition(x-self.shopRender.GetWidth()-5,y)

	def OnUpdate(self):
		USE_SHOP_LIMIT_RANGE = 1000

		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xShopStart) > USE_SHOP_LIMIT_RANGE or abs(y - self.yShopStart) > USE_SHOP_LIMIT_RANGE:
			self.Close()
