# coding: latin_1

import player
import net
import app
import item
import chat
import ime
import exception
import ui
import uiCommon
#import uiPrivateShopBuilder
import uiPickEtc
import localeInfo
import constInfo
import mouseModule
import uiScriptLocale

class SpecialStorageWindow(ui.ScriptWindow):

	liHighlightedItems = []

	UPGRADE_TYPE = 0
	BOOK_TYPE = 1
	STONE_TYPE = 2
	COSTUME_TYPE = 3

	SLOT_WINDOW_TYPE = {
		UPGRADE_TYPE	:	{"window" : player.UPGRADE_INVENTORY, "slot" : player.SLOT_TYPE_UPGRADE_INVENTORY},
		BOOK_TYPE		:	{"window" : player.BOOK_INVENTORY, "slot" : player.SLOT_TYPE_BOOK_INVENTORY},
		STONE_TYPE		:	{"window" : player.STONE_INVENTORY, "slot" : player.SLOT_TYPE_STONE_INVENTORY},
		COSTUME_TYPE	:	{"window" : player.COSTUME_INVENTORY, "slot" : player.SLOT_TYPE_COSTUME_INVENTORY},
	}

	WINDOW_NAMES = {
		UPGRADE_TYPE	:	uiScriptLocale.STORAGE_1,
		BOOK_TYPE		:	uiScriptLocale.STORAGE_2,
		STONE_TYPE		:	uiScriptLocale.STORAGE_3
	}

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.questionDialog = None
		self.tooltipItem = None
		self.dlgSplitItems = None
		self.SLOT_SPLIT_ITEM = -1
		self.sellingSlotNumber = -1
		self.isLoaded = 0
		self.inventoryPageIndex = 0
		self.categoryPageIndex = 0
		self.interface = None
		self.pagecount = 4
		self.SetWindowName("SpecialStorageWindow")
		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		self.__LoadWindow()
		ui.ScriptWindow.Show(self)

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/SpecialStorageWindow.py")
		except:
			exception.Abort("SpecialStorageWindow.LoadWindow.LoadObject")

		try:
			wndItem = self.GetChild("ItemSlot")
			self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
			self.titleName = self.GetChild("TitleName")
			self.inventoryTab = []
			self.inventoryTab.append(self.GetChild("Inventory_Tab_01"))
			self.inventoryTab.append(self.GetChild("Inventory_Tab_02"))
			self.inventoryTab.append(self.GetChild("Inventory_Tab_03"))
			self.inventoryTab.append(self.GetChild("Inventory_Tab_04"))
			
			self.categoryTab = []
			self.categoryTab.append(self.GetChild("Category_Tab_01"))
			self.categoryTab.append(self.GetChild("Category_Tab_02"))
			self.categoryTab.append(self.GetChild("Category_Tab_03"))
			# self.separateButton = self.GetChild("SeparateButton")
		except:
			exception.Abort("SpecialStorageWindow.LoadWindow.BindObject")

		wndItem.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		wndItem.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		wndItem.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		wndItem.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		wndItem.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
		# self.separateButton.SetEvent(self.__CustomeEvent)

		self.inventoryTab[0].SetEvent(lambda arg=0: self.SetInventoryPage(arg))
		self.inventoryTab[1].SetEvent(lambda arg=1: self.SetInventoryPage(arg))
		self.inventoryTab[2].SetEvent(lambda arg=2: self.SetInventoryPage(arg))
		self.inventoryTab[3].SetEvent(lambda arg=3: self.SetInventoryPage(arg))
		self.inventoryTab[0].Down()

		self.categoryTab[0].SetEvent(lambda arg=0: self.SetCategoryPage(arg))
		self.categoryTab[1].SetEvent(lambda arg=1: self.SetCategoryPage(arg))
		self.categoryTab[2].SetEvent(lambda arg=2: self.SetCategoryPage(arg))
		self.categoryTab[0].Down()

		self.wndItem = wndItem

		self.wndPopupDialog = uiCommon.PopupDialog()

		self.dlgSplitItems = uiPickEtc.PickEtcDialog()
		self.dlgSplitItems.LoadDialog()
		self.dlgSplitItems.Hide()

		self.SetInventoryPage(0)
		self.SetCategoryPage(0)
		self.RefreshItemSlot()
		self.RefreshBagSlotWindow()

	def __CustomeEvent(self):
		self.question = uiCommon.QuestionDialog()
		self.question.SetCenterPosition()
		self.question.SetText(uiScriptLocale.SORT_QUESTION)
		self.question.SetAcceptEvent(ui.__mem_func__(self.ClickSort))
		self.question.SetCancelEvent(ui.__mem_func__(self.NoOpenInventory))
		self.question.SetAcceptText(uiScriptLocale.YES)
		self.question.SetCancelText(uiScriptLocale.NO)
		self.question.Open()

	def NoOpenInventory(self):
		self.question.Close()
		
	def ClickSort(self):
		net.SendChatPacket("/click_sort_special_storage")
		self.question.Close()

	def Destroy(self):
		self.ClearDictionary()
		self.tooltipItem = None
		self.wndItem = 0
		self.questionDialog = None
		self.dlgSplitItems.Destroy()
		self.dlgSplitItems = None
		self.inventoryTab = []
		self.categoryTab = []
		self.titleName = None

	def Close(self):
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()
		if self.dlgSplitItems:
			self.dlgSplitItems.Close()
		self.Hide()

	def SetInventoryPage(self, page):
		self.inventoryTab[self.inventoryPageIndex].SetUp()
		self.inventoryPageIndex = page
		self.inventoryTab[self.inventoryPageIndex].Down()

		self.RefreshBagSlotWindow()

	def SetCategoryPage(self, page):
		self.categoryTab[self.categoryPageIndex].SetUp()
		self.categoryPageIndex = page
		self.categoryTab[self.categoryPageIndex].Down()

		self.titleName.SetText(self.WINDOW_NAMES[self.categoryPageIndex])
		self.RefreshBagSlotWindow()

	def BindInterface(self,interface):
		self.interface = interface

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def RefreshItemSlot(self):
		self.RefreshBagSlotWindow()

	def RefreshStatus(self):
		self.RefreshItemSlot()

	def __InventoryLocalSlotPosToGlobalSlotPos(self, localSlotPos):
		return self.inventoryPageIndex * player.SPECIAL_PAGE_SIZE + localSlotPos

	def RefreshBagSlotWindow(self):
		getItemVNum=player.GetItemIndex
		getItemCount=player.GetItemCount
		setItemVnum=self.wndItem.SetItemSlot

		for i in xrange(player.SPECIAL_PAGE_SIZE):
			self.wndItem.EnableSlot(i)
			slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(i)

			itemCount = getItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotNumber)
			itemVnum = getItemVNum(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotNumber)

			if 0 == itemCount:
				self.wndItem.ClearSlot(i)
				continue
			elif 1 == itemCount:
				itemCount = 0

			setItemVnum(i, itemVnum, itemCount)

		self.wndItem.RefreshSlot()

	def ShowToolTip(self, slotIndex):
		if None != self.tooltipItem:
			self.tooltipItem.SetInventoryItem(slotIndex, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"])

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnTop(self):
		if None != self.tooltipItem:
			self.tooltipItem.SetTop()

	def OverOutItem(self):
		self.wndItem.SetUsableItem(False)
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OverInItem(self, overSlotPosN):
		overSlotPos = self.__InventoryLocalSlotPosToGlobalSlotPos(overSlotPosN)
		
		self.wndItem.SetUsableItem(False)
		self.ShowToolTip(overSlotPos)

	def OnPickItem(self, count):
		itemSlotIndex = self.dlgSplitItems.itemGlobalSlotIndex
		selectedItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)
		mouseModule.mouseController.AttachObject(self, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["slot"], itemSlotIndex, selectedItemVNum, count)

	def __GetCurrentItemGrid(self):
		itemGrid = [[False for cell in xrange(player.SPECIAL_PAGE_SIZE)] for page in xrange(self.pagecount)]

		for page in xrange(self.pagecount):
			for slot in xrange(player.SPECIAL_PAGE_SIZE):
				itemVnum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slot + page * player.SPECIAL_PAGE_SIZE)
				if itemVnum <> 0:
					item.SelectItem(itemVnum)
					(w, h) = item.GetItemSize()
					for i in xrange(h):
						itemGrid[page][slot + i * 5] = True

		return itemGrid

	def __FindEmptyCellForSize(self, itemGrid, size):
		for page in xrange(self.pagecount):
			for slot in xrange(player.SPECIAL_PAGE_SIZE):
				if itemGrid[page][slot] == False:
					possible = True
					for i in xrange(size):
						p = slot + 5 * i
						try:
							if itemGrid[page][p] == True:
								possible = False
								break
						except IndexError:
							possible = False
							break

					if possible:
						return slot + page * player.SPECIAL_PAGE_SIZE

		return -1

	def AttachItemFromSafebox(self, slotIndex, item_type, itemIndex):
		itemGrid = self.__GetCurrentItemGrid()

		emptySlotIndex = self.__FindEmptyCellForSize(itemGrid, item.GetItemSize()[1])
		if emptySlotIndex <> -1:
			net.SendSafeboxCheckoutPacket(slotIndex, item_type, emptySlotIndex)

		return True


	def SelectItemSlot(self, itemSlotIndex):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
			return

		itemSlotIndex = self.__InventoryLocalSlotPosToGlobalSlotPos(itemSlotIndex)

		if app.IsPressed(app.DIK_LCONTROL):
			if self.interface and self.interface.AttachInvenItemToOtherWindowSlot(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex):
				return
		elif app.IsCanOpenRender() and app.ENABLE_RENDER_TARGET:
			self.interface.OpenRenderTargetWindow(0, player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex))
			return

		selectedItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)
		itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)

		if mouseModule.mouseController.isAttached():
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemVID = mouseModule.mouseController.GetAttachedItemIndex()
			attachedItemCount  = mouseModule.mouseController.GetAttachedItemCount()

			if self.SLOT_SPLIT_ITEM == attachedSlotPos:
				mouseModule.mouseController.DeattachObject()
				return

			if self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["slot"] == attachedSlotType or player.SLOT_TYPE_INVENTORY == attachedSlotType:

				slotIndex = self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"]
				if player.SLOT_TYPE_INVENTORY == attachedSlotType:
					slotIndex = player.INVENTORY

				if attachedItemVID == selectedItemVNum:
					net.SendItemMovePacket(slotIndex, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex, attachedItemCount)
				else:
					net.SendItemUseToItemPacket(slotIndex, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)

			mouseModule.mouseController.DeattachObject()
		else:
			curCursorNum = app.GetCursor()

			if app.SELL == curCursorNum:
				self.__SellItem(itemSlotIndex)
			elif app.BUY == curCursorNum:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_BUY_INFO)
			elif app.IsPressed(app.DIK_LALT):
				link = player.GetItemLink(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)
				ime.PasteString(link)
			elif app.IsPressed(app.DIK_LSHIFT):
				if itemCount > 1:
					self.SLOT_SPLIT_ITEM = itemSlotIndex
					self.dlgSplitItems.SetTitleName(localeInfo.PICK_ITEM_TITLE)
					self.dlgSplitItems.SetAcceptEvent(ui.__mem_func__(self.OnPickItem))
					self.dlgSplitItems.Open(itemCount)
					self.dlgSplitItems.itemGlobalSlotIndex = itemSlotIndex
			else:
				mouseModule.mouseController.AttachObject(self, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["slot"], itemSlotIndex, selectedItemVNum, itemCount)
				self.wndItem.SetUseMode(False)

	def __SellItem(self, itemSlotPos):
		self.sellingSlotNumber = itemSlotPos
		itemIndex = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotPos)
		itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotPos)

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

		itemPrice = item.GetISellItemPrice()

		if item.Is1GoldItem():
			#itemPrice = itemCount / itemPrice / 5
			itemPrice = itemCount / itemPrice
		else:
			#itemPrice = itemPrice * itemCount / 5
			itemPrice = itemPrice * itemCount

		item.GetItemName(itemIndex)
		itemName = item.GetItemName()

		self.questionDialog = uiCommon.QuestionDialog()
		self.questionDialog.SetText(localeInfo.DO_YOU_SELL_ITEM(itemName, itemCount, itemPrice))
		self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.SellItem))
		self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		self.questionDialog.Open()
		self.questionDialog.count = itemCount

	def SellItem(self):
		net.SendShopSellPacketNew(self.sellingSlotNumber, self.questionDialog.count, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"])
		self.OnCloseQuestionDialog()

	def OnCloseQuestionDialog(self):
		if self.questionDialog:
			self.questionDialog.Close()

		self.questionDialog = None

	def __OnClosePopupDialog(self):
		self.pop = None

	def SelectEmptySlot(self, selectedSlotPos):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
			return

		selectedSlotPos = self.__InventoryLocalSlotPosToGlobalSlotPos(selectedSlotPos)
		if mouseModule.mouseController.isAttached():

			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()
			attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()

			attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)

			if player.SLOT_TYPE_PRIVATE_SHOP == attachedSlotType:
				mouseModule.mouseController.RunCallBack("INVENTORY")
			elif player.SLOT_TYPE_SHOP == attachedSlotType:
				net.SendShopBuyPacket(attachedSlotPos)
			elif player.SLOT_TYPE_SAFEBOX == attachedSlotType:
				net.SendSafeboxCheckoutPacket(attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos)
			elif player.SLOT_TYPE_MALL == attachedSlotType:
				net.SendMallCheckoutPacket(attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos)
			elif player.RESERVED_WINDOW != attachedInvenType:
				itemCount = player.GetItemCount(attachedInvenType, attachedSlotPos)
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()

				self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)

			mouseModule.mouseController.DeattachObject()

	def UseItemSlot(self, slotIndex):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS():
			return
		slotIndex = self.__InventoryLocalSlotPosToGlobalSlotPos(slotIndex)

		net.SendItemUsePacket(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)

		mouseModule.mouseController.DeattachObject()
		self.OverOutItem()

	def __SendMoveItemPacket(self, srcSlotWindow, srcSlotPos, dstSlotWindow, dstSlotPos, srcItemCount):
		#if uiPrivateShopBuilder.IsBuildingPrivateShop():
		#	chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
		#	return

		net.SendItemMovePacket(srcSlotWindow , srcSlotPos, dstSlotWindow, dstSlotPos, srcItemCount)