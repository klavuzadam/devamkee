# coding: latin_1

import ui
import player
import mouseModule
import net
import app
import snd
import item
import player
import safebox
import chat
import grp
import uiScriptLocale
import uiRefine
import uiAttachMetin
import uiPickMoney
import uiCommon
#import uiPrivateShopBuilder # 개인상점 열동안 ItemMove 방지
import exchange
import localeInfo
import constInfo
import ime
import dbg
import wndMgr
import uiToolTip
import game
import uisplititem
from uiUtils import Edit2 as Edit2

if app.ENABLE_IKASHOP_RENEWAL:
	import ikashop

if app.BL_67_ATTR:
	import uiAttr67Add

ITEM_MALL_BUTTON_ENABLE = True

import exception


ITEM_FLAG_APPLICABLE = 1 << 14
if app.ENABLE_SASH_SYSTEM:
	import sash
	
ENABLE_SEARCH_FIELD = True # Enable system search item field

class InventoryWindowNew(ui.ScriptWindow):
	liHighlightedItems = []

	USE_TYPE_TUPLE = ("USE_SPECIAL", "USE_CLEAN_SOCKET", "USE_CHANGE_ATTRIBUTE", "USE_CHANGE_ATTRIBUTE2", "USE_ADD_ATTRIBUTE", "USE_ADD_ATTRIBUTE2", "USE_ADD_ACCESSORY_SOCKET", "USE_PUT_INTO_ACCESSORY_SOCKET", "USE_PUT_INTO_BELT_SOCKET", "USE_PUT_INTO_RING_SOCKET", "USE_CHANGE_COSTUME_ATTR", "USE_RESET_COSTUME_ATTR")
	USE_TYPE_TUPLE = tuple(list(USE_TYPE_TUPLE) + ["USE_ADD_BOOSTER_ATTR", "USE_CHANGE_BOOSTER_ATTR", "USE_ADD_BOOSTER_TIME"])

	UPGRADE_TYPE = 1
	BOOK_TYPE = 2
	STONE_TYPE = 3
	CHANGE_TYPE = 4
	COSTUME_TYPE = 5
	CHEST_TYPE = 6
	SAFEBOX_TYPE = 7
	MALL_TYPE = 8

	SLOT_WINDOW_TYPE = {
		UPGRADE_TYPE : {"name": uiScriptLocale.STORAGE_1, "window" : player.UPGRADE_INVENTORY, "slot" : player.SLOT_TYPE_UPGRADE_INVENTORY},
		BOOK_TYPE : {"name": uiScriptLocale.STORAGE_2, "window" : player.BOOK_INVENTORY, "slot" : player.SLOT_TYPE_BOOK_INVENTORY},
		STONE_TYPE : {"name": uiScriptLocale.STORAGE_3, "window" : player.STONE_INVENTORY, "slot" : player.SLOT_TYPE_STONE_INVENTORY},
		CHANGE_TYPE : {"name": uiScriptLocale.STORAGE_4, "window" : player.CHANGE_INVENTORY, "slot" : player.SLOT_TYPE_CHANGE_INVENTORY},
		COSTUME_TYPE : {"name": uiScriptLocale.STORAGE_5, "window" : player.COSTUME_INVENTORY, "slot" : player.SLOT_TYPE_COSTUME_INVENTORY},
		CHEST_TYPE : {"name": uiScriptLocale.STORAGE_6, "window" : player.CHEST_INVENTORY, "slot" : player.SLOT_TYPE_CHEST_INVENTORY},
		SAFEBOX_TYPE : {"name": uiScriptLocale.STORAGE_7, "window" : player.RESERVED_WINDOW, "slot" : player.SLOT_TYPE_SAFEBOX},
		MALL_TYPE : {"name": uiScriptLocale.STORAGE_8, "window" : player.RESERVED_WINDOW, "slot" : player.SLOT_TYPE_MALL},
	}

	questionDialog = None
	tooltipItem = None
	dlgPickMoney = None
	sellingSlotNumber = -1

	isLoaded = 0

	interface = None
	if app.WJ_ENABLE_TRADABLE_ICON:
		bindWnds = []
	
	wndPrivateShopSearch	= None
	wndWiki					= None

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.categoryPageIndex = 1
		self.inventoryPageIndex = 0
		self.ChangeEquipIndex = 0
		self.rotationImage = 0
		self.wndPrivateShopSearch	= None
		self.wndWiki					= None

		self.grid = ui.Grid(width = 5, height = (9*4))
		if ENABLE_SEARCH_FIELD:
			self.wndItemSRC = None
			self.wndNameSlot = None
			self.btnResetSearch = None
			self.wndName = None
		self.manageOpen = False

		if app.ENABLE_SASH_SYSTEM:
			self.listAttachedSashs = []
		if app.ENABLE_HIDE_COSTUME_SYSTEM:
			self.elemets_hide = []
		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def BindInterfaceClass(self, interface):
		self.interface = interface
		
	if app.WJ_ENABLE_TRADABLE_ICON:
		def BindWindow(self, wnd):
			self.bindWnds.append(wnd)

	def __LoadWindow(self):

		try:
			pyScrLoader = ui.PythonScriptLoader()

			pyScrLoader.LoadScriptFile(self, "UIScript/InventoryWindowNew.py")

		except:
			import exception
			exception.Abort("InventoryWindow.LoadWindow.LoadObject")

		try:
			self.wndItem = self.GetChild("ItemSlot")
			self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
			self.wndBoard = self.GetChild("board")
			self.board = self.GetChild("board")
			self.categoryBoard = self.GetChild("CategoryBoard")

			self.inventory_special = []
			for key, data in self.SLOT_WINDOW_TYPE.items():
				pointer = self.GetChild("Inventory_Special_%d"%key)
				pointer.SetEvent(lambda arg=key: self.SetCategoryPage(arg))
				pointer.SetToolTipText(self.SLOT_WINDOW_TYPE[key]["name"])
				self.inventory_special.append(pointer)

			# self.inventory_special[len(self.inventory_special)-1].Hide()#CLOSE MALL BTN

			# self.GetChild("SeparateButton").SetEvent(self.__CustomeEvent)

			self.inventoryTab = []
			for i in xrange(player.INVENTORY_PAGE_COUNT):
				self.inventoryTab.append(self.GetChild("Inventory_Tab_%02d" % (i+1)))
				self.inventoryTab[i].SetEvent(lambda arg=i: self.SetInventoryPage(arg))
			
			self.dlgPickMoney = uisplititem.PickItemDialog()
			self.dlgPickMoney.LoadDialog()
			self.dlgPickMoney.Hide()

			self.objectToMove = []
			self.objectToMove.append(self.wndItem)
			self.objectToMove.append(self.categoryBoard)

			self.objectToMove = self.objectToMove + self.inventoryTab

			self.manageInventoryBtn = self.GetChild("ManageInventory")
			self.manageOpen = False
		except:
			import exception
			exception.Abort("InventoryWindow.LoadWindow.BindObject")

		self.wndItem.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		self.wndItem.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		self.wndItem.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		self.wndItem.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
		self.wndItem.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.wndItem.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		self.lock, self.lock2, u, e = {}, {}, 0, 0
		for i in xrange(18):
			self.lock[i] = ui.Button()
			self.lock[i].SetParent(self.wndItem)
			if i >= 9:
				self.lock[i].SetPosition(0,0+e)
				e+=32
			else:
				self.lock[i].SetPosition(0,0+u)
			self.lock[i].SetUpVisual("d:/ymir work/drakon2/inventory/0.tga")
			self.lock[i].SetOverVisual("d:/ymir work/drakon2/inventory/0.tga")
			self.lock[i].SetDownVisual("d:/ymir work/drakon2/inventory/0.tga")
			self.lock[i].SetEvent(lambda x=i: self.ClickUnlockInventory(x))
			self.lock[i].Hide()
			u+=32

		if ENABLE_SEARCH_FIELD:
			self.wndItemSRC = ui.GridSlotWindow()
			self.wndItemSRC.SetParent(self)
			self.wndItemSRC.SetPosition(12, 35)
			self.wndItemSRC.ArrangeSlot(0, 5, 9, 32, 32, 0, 0)
			self.wndItemSRC.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)
			self.wndItemSRC.SetGridSpecial(5, 9)
			self.wndItemSRC.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
			self.wndItemSRC.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
			self.wndItemSRC.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
			self.wndItemSRC.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			self.wndItemSRC.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
			self.wndItemSRC.dictItems = {}
			self.wndItemSRC.Hide()

			self.wndButtonBase = ui.ImageBox()
			self.wndButtonBase.SetParent(self)
			self.wndButtonBase.LoadImage("d:/ymir work/ui/game/interfaces/special_storage/btn-base_storage.png")
			self.wndButtonBase.SetPosition(12, 30.5)
			self.wndButtonBase.Hide()

			self.wndNameSlot = ui.ImageBox()
			self.wndNameSlot.SetParent(self.wndButtonBase)
			self.wndNameSlot.LoadImage("d:/ymir work/ui/game/special_inventory/input.png")
			self.wndNameSlot.SetPosition(52, 4)
			self.wndNameSlot.Show()

			# self.btnResetSearch = ui.Button()
			# self.btnResetSearch.SetParent(self.wndButtonBase)
			# self.btnResetSearch.SetUpVisual("d:/ymir work/ui/game/search_inventory/x_norm.png")
			# self.btnResetSearch.SetOverVisual("d:/ymir work/ui/game/search_inventory/x_over.png")
			# self.btnResetSearch.SetDownVisual("d:/ymir work/ui/game/search_inventory/x_down.png")
			# self.btnResetSearch.SetPosition(136, 7)
			# self.btnResetSearch.SetToolTipText("Clear", 0, -23)
			# self.btnResetSearch.SetEvent(ui.__mem_func__(self.ResetSearchField))
			# self.btnResetSearch.Show()

			self.separateButton = ui.Button()
			self.separateButton.SetParent(self.wndButtonBase)
			self.separateButton.SetUpVisual("d:/ymir work/ui/game/interfaces/special_storage/btn-stack-1.png")
			self.separateButton.SetOverVisual("d:/ymir work/ui/game/interfaces/special_storage/btn-stack-2.png")
			self.separateButton.SetDownVisual("d:/ymir work/ui/game/interfaces/special_storage/btn-stack-3.png")
			self.separateButton.SetPosition(3, 4)
			self.separateButton.SetToolTipText("Stack", 0, -23)
			self.separateButton.SetEvent(ui.__mem_func__(self.__CustomeEvent))
			self.separateButton.Show()

			self.separate2Button = ui.Button()
			self.separate2Button.SetParent(self.wndButtonBase)
			self.separate2Button.SetUpVisual("d:/ymir work/ui/game/special_inventory/btn-sort-1.png")
			self.separate2Button.SetOverVisual("d:/ymir work/ui/game/special_inventory/btn-sort-2.png")
			self.separate2Button.SetDownVisual("d:/ymir work/ui/game/special_inventory/btn-sort-3.png")
			self.separate2Button.SetPosition(27, 4)
			self.separate2Button.SetToolTipText("Sort", 0, -23)
			self.separate2Button.SetEvent(ui.__mem_func__(self.__Custome2Event))
			self.separate2Button.Show()

			self.wndName = ui.EditLine()
			self.wndName.SetParent(self.wndNameSlot)
			self.wndName.SetPosition(6, 4) # 2, 2
			self.wndName.SetSize(self.wndNameSlot.GetWidth() - 2, self.wndNameSlot.GetHeight() - 2)
			self.wndName.SetMax(55)
			self.wndName.SetInfoMessage("Search..")
			self.wndName.OnIMEUpdate = ui.__mem_func__(self.OnValueUpdateSearch)
			self.wndName.SetEscapeEvent(ui.__mem_func__(self.Close))
			self.wndName.KillFocus()
			self.wndName.Show()
			
		self.objectToMove.append(self.wndItemSRC)
	
		if self.manageInventoryBtn:
			self.manageInventoryBtn.SetEvent(ui.__mem_func__(self.ManageInventory))

		self.ManageInventory()
		self.SetInventoryPage(0)
		self.SetCategoryPage(1)
		self.RefreshStatus()

	def GetPageSizeCategory(self):
		if self.GetIsCategorySpecial():
			return player.SPECIAL_PAGE_SIZE
		return player.INVENTORY_PAGE_SIZE

	def GetPageCountCategory(self):
		if self.GetIsCategorySpecial():
			return 4
		return player.INVENTORY_PAGE_COUNT

	def GetIsCategorySpecial(self):
		if self.categoryPageIndex <= 0:
			return False
		return True

	def __CustomeEvent(self):
		self.question = uiCommon.QuestionDialog()
		self.question.SetCenterPosition()
		self.question.SetText(uiScriptLocale.SORT_QUESTION)
		self.question.SetAcceptEvent(ui.__mem_func__(self.ClickSort))
		self.question.SetCancelEvent(ui.__mem_func__(self.NoOpenInventory))
		self.question.SetAcceptText(uiScriptLocale.YES)
		self.question.SetCancelText(uiScriptLocale.NO)
		self.question.Open()

	def __Custome2Event(self):
		self.question = uiCommon.QuestionDialog()
		self.question.SetCenterPosition()
		self.question.SetText("Sort?")
		self.question.SetAcceptEvent(ui.__mem_func__(self.ClickStack))
		self.question.SetCancelEvent(ui.__mem_func__(self.NoOpenInventory))
		self.question.SetAcceptText(uiScriptLocale.YES)
		self.question.SetCancelText(uiScriptLocale.NO)
		self.question.Open()

	def ClickSort(self):
		category = self.GetCategoryIndex()
		if category == 0:
			net.SendChatPacket("/click_sort_items")
		else:
			net.SendChatPacket("/click_sort_special_storage %d"%(category))
		self.question.Close()

	def ClickStack(self):
		category = self.GetCategoryIndex()
		if category == 0:
			net.SendChatPacket("/real_sort_items 0")
		else:
			net.SendChatPacket("/real_sort_items %d"%(category))
		self.question.Close()

	def ManageInventory(self):
		if not self.manageOpen:
			self.manageOpen = True
			# if self.inventoryType != 6 and self.inventoryType != 7: # IS_SAFEBOX | MALL
			self.ClearGrid()
			self.wndButtonBase.Show()
				# self.stackInventoryBtn.Show()
			# self.sortInventoryBtn.Show()
			self.SetSize(self.GetWidth(), self.GetHeight()+25)
			self.board.SetSize(self.board.GetWidth(), self.board.GetHeight()+25)
			for i in range(len(self.objectToMove)):
				self.objectToMove[i].SetPosition(self.objectToMove[i].GetLocalPosition()[0], self.objectToMove[i].GetLocalPosition()[1]+25)
		else:
			self.manageOpen = False
			# if self.inventoryType != 6 and self.inventoryType != 7: # IS_SAFEBOX | MALL
			self.wndButtonBase.Hide()
			self.wndName.SetText("")
			self.wndItemSRC.Hide()
			self.wndItem.Show()
			# self.sortInventoryBtn.Hide()
			# self.stackInventoryBtn.Hide()
			self.SetSize(self.GetWidth(), self.GetHeight()-25)
			self.board.SetSize(self.board.GetWidth(), self.board.GetHeight()-25)
			for i in range(len(self.objectToMove)):
				self.objectToMove[i].SetPosition(self.objectToMove[i].GetLocalPosition()[0], self.objectToMove[i].GetLocalPosition()[1]-25)

	if ENABLE_SEARCH_FIELD:
		def ClearGrid(self):
			self.wndItemSRC.ClearGrid()
			for x in xrange(5 * 9):
				self.wndItemSRC.ClearSlot(x)
				
			self.wndItemSRC.RefreshSlot()
		
		def ResetSearchField(self):
			self.wndName.SetText("")
			self.OnValueUpdateSearch()
			self.RefreshBagSlotWindow()
		
		def OnValueUpdateSearch(self):
			if self.IsShow() == False:
				return

			if self.inventoryPageIndex != 0 and self.wndName.GetText != "":
				chat.AppendChat(1, "You need to be on the first page to search something.")
				self.wndName.SetText("")
				return
				
			if self.wndName.IsFocus():
				ui.EditLine.OnIMEUpdate(self.wndName)
			
			text = self.wndName.GetText()
	
			self.ClearGrid()
			self.wndItemSRC.dictItems = {}

			if not text:
				self.wndItemSRC.Hide()
				self.wndItem.Show()
				return

			self.wndItemSRC.Show()
			self.wndItem.Hide()

			## Start searching the items try

			window_type = self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"]
			for i in xrange(player.INVENTORY_PAGE_COUNT * player.INVENTORY_PAGE_SIZE):
				itemCount = player.GetItemCount(window_type, i)
				
				if itemCount == 0:
					continue
				elif 1 == itemCount:
					itemCount = 0			
				
				itemVNum = player.GetItemIndex(window_type, i)
				item.SelectItem(itemVNum)

				stName = item.GetItemName()
				
				if itemVNum == 50300: # IS_SKILLBOOK
					stName = skill.GetSkillName(player.GetItemMetinSocket(window_type, i, 0))
				
				if len(stName) >= len(text) and stName[:len(text)].lower() == text.lower():
					_, size = item.GetItemSize()
					iPos = self.wndItemSRC.GetEmptyGrid(size)
					
					if iPos == -1:
						break
					
					self.wndItemSRC.PutItemGrid(iPos, size)
					self.wndItemSRC.SetItemSlot(iPos, itemVNum, itemCount)
					self.wndItemSRC.dictItems[iPos] = (window_type, i)
			
			self.wndItemSRC.RefreshSlot()

	def ClickUnlockInventory(self, index):
		if index > constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]:
			pass
		else:
			if constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())] <= 18:
				self.question = uiCommon.QuestionDialog()
				self.question.SetCenterPosition()
				self.question.SetText(uiScriptLocale.INVENTORY_UNLOCK % int((constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]*2)+2))
				self.question.SetAcceptEvent(ui.__mem_func__(self.OpenInventory))
				self.question.SetCancelEvent(ui.__mem_func__(self.NoOpenInventory))
				self.question.SetAcceptText(uiScriptLocale.YES)
				self.question.SetCancelText(uiScriptLocale.NO)
				self.question.Open()

	def OpenInventory(self):
		if constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())] <= 18:
			net.SendChatPacket("/unlock_inventory %d"%(self.GetCategoryIndex()))
		self.question.Close()

	def NoOpenInventory(self):
		self.question.Close()

	def Destroy(self):
		self.ClearDictionary()
		self.grid.reset()
		self.grid = None
		self.dlgPickMoney.Destroy()
		self.dlgPickMoney = 0
		self.tooltipItem = None
		self.wndItem = 0
		self.questionDialog = None
		self.dlgQuestion = None
		self.interface = None
		self.wndPrivateShopSearch	= None
		self.wndWiki				= None
		if ENABLE_SEARCH_FIELD:
			self.wndItemSRC = None
			self.wndNameSlot = None
			self.btnResetSearch = None
			self.wndName = None
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.bindWnds = []
		self.inventoryTab = []
		self.Hide()

	def Hide(self, force_hide = False):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS():
			self.OnCloseQuestionDialog()
			return
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()
		
		if self.dlgPickMoney:
			self.dlgPickMoney.Close()

		wndMgr.Hide(self.hWnd, force_hide)

	def Close(self):
		self.Hide()
		
		if ENABLE_SEARCH_FIELD:
			if self.wndName and self.wndName.IsFocus():
				self.wndName.KillFocus()
		
		if self.categoryPageIndex >= 7:
			self.SetCategoryPage(1)#auto move to book inv.

	def __InventoryLocalSlotPosToGlobalSlotPos(self, local):
		if self.GetIsCategorySpecial():
			return self.inventoryPageIndex * player.SPECIAL_PAGE_SIZE + local

		if player.IsEquipmentSlot(local) or player.IsCostumeSlot(local) or (app.ENABLE_NEW_EQUIPMENT_SYSTEM and player.IsBeltInventorySlot(local)):
			return local

		return self.inventoryPageIndex*player.INVENTORY_PAGE_SIZE + local
	
	def __InventoryLocalSlotPosToGlobalSlotPosWithPage(self, page, local):
		if self.GetIsCategorySpecial():
			return page * player.SPECIAL_PAGE_SIZE + local

		if player.IsEquipmentSlot(local) or player.IsCostumeSlot(local) or (app.ENABLE_NEW_EQUIPMENT_SYSTEM and player.IsBeltInventorySlot(local)):
			return local

		return page*player.INVENTORY_PAGE_SIZE + local

	def GetInventoryPageIndex(self):
		return self.inventoryPageIndex

	if app.WJ_ENABLE_TRADABLE_ICON:
		def RefreshMarkSlots(self, localIndex=None):
			if not self.interface:
				return
			if not self.wndItem:
				return
				
			# chat.AppendChat(1, "i'm in refreshmarkslots")
			
			onTopWnd = self.interface.GetOnTopWindow()
			if localIndex:
				slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(localIndex)
				# chat.AppendChat(1, "appended with localidx %d and slotnUmber %d" % (localIndex, slotNumber))
				if onTopWnd == player.ON_TOP_WND_NONE:
					self.wndItem.SetUsableSlotOnTopWnd(localIndex)
	
				elif onTopWnd == player.ON_TOP_WND_SHOP:
					if player.IsAntiFlagBySlot(slotNumber, item.ANTIFLAG_SELL):
						self.wndItem.SetUnusableSlotOnTopWnd(localIndex)
					else:
						self.wndItem.SetUsableSlotOnTopWnd(localIndex)
	
				elif onTopWnd == player.ON_TOP_WND_EXCHANGE:
					# chat.AppendChat(1, "appended exchange")
					if player.IsAntiFlagBySlot(slotNumber, item.ANTIFLAG_GIVE):
						self.wndItem.SetUnusableSlotOnTopWnd(localIndex)
					else:
						self.wndItem.SetUsableSlotOnTopWnd(localIndex)
	
				elif onTopWnd == player.ON_TOP_WND_PRIVATE_SHOP:
					if player.IsAntiFlagBySlot(slotNumber, item.ITEM_ANTIFLAG_MYSHOP):
						self.wndItem.SetUnusableSlotOnTopWnd(localIndex)
					else:
						self.wndItem.SetUsableSlotOnTopWnd(localIndex)
	
				elif onTopWnd == player.ON_TOP_WND_SAFEBOX:
					if player.IsAntiFlagBySlot(slotNumber, item.ANTIFLAG_SAFEBOX):
						self.wndItem.SetUnusableSlotOnTopWnd(localIndex)
					else:
						self.wndItem.SetUsableSlotOnTopWnd(localIndex)
	
				elif app.BL_67_ATTR and onTopWnd == player.ON_TOP_WND_ATTR_67:
					mark = (not self.interface.IsAttr67RegistItem() and not uiAttr67Add.Attr67AddWindow.CantAttachToAttrSlot(slotNumber, True)) or \
						(self.interface.IsAttr67RegistItem() and not self.interface.IsAttr67SupportItem() and not uiAttr67Add.Attr67AddWindow.IsSupportItem(slotNumber))
	
					if mark:
						self.wndItem.SetUnusableSlotOnTopWnd(localIndex)
					else:
						self.wndItem.SetUsableSlotOnTopWnd(localIndex)
	
				return
				
			for j in xrange(4):
				for i in xrange(self.GetPageSizeCategory()):
					slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPosWithPage(j,i)

			# for i in xrange(self.GetPageSizeCategory()):
				# slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(i)
		
					if onTopWnd == player.ON_TOP_WND_NONE:
						self.wndItem.SetUsableSlotOnTopWnd(i)
		
					elif onTopWnd == player.ON_TOP_WND_SHOP:
						if player.IsAntiFlagBySlot(slotNumber, item.ANTIFLAG_SELL):
							self.wndItem.SetUnusableSlotOnTopWnd(i)
						else:
							self.wndItem.SetUsableSlotOnTopWnd(i)
		
					elif onTopWnd == player.ON_TOP_WND_EXCHANGE:
						# chat.AppendChat(1, "appended exchange x2")
						if player.IsAntiFlagBySlot(slotNumber, item.ANTIFLAG_GIVE):
							self.wndItem.SetUnusableSlotOnTopWnd(i)
						else:
							self.wndItem.SetUsableSlotOnTopWnd(i)
		
					elif onTopWnd == player.ON_TOP_WND_PRIVATE_SHOP:
						if player.IsAntiFlagBySlot(slotNumber, item.ITEM_ANTIFLAG_MYSHOP):
							self.wndItem.SetUnusableSlotOnTopWnd(i)
						else:
							self.wndItem.SetUsableSlotOnTopWnd(i)
		
					elif onTopWnd == player.ON_TOP_WND_SAFEBOX:
						if player.IsAntiFlagBySlot(slotNumber, item.ANTIFLAG_SAFEBOX):
							self.wndItem.SetUnusableSlotOnTopWnd(i)
						else:
							self.wndItem.SetUsableSlotOnTopWnd(i)
		
					elif app.BL_67_ATTR and onTopWnd == player.ON_TOP_WND_ATTR_67:
						mark = (not self.interface.IsAttr67RegistItem() and not uiAttr67Add.Attr67AddWindow.CantAttachToAttrSlot(slotNumber, True)) or \
							(self.interface.IsAttr67RegistItem() and not self.interface.IsAttr67SupportItem() and not uiAttr67Add.Attr67AddWindow.IsSupportItem(slotNumber))
						
						if mark:
							self.wndItem.SetUnusableSlotOnTopWnd(i)
						else:
							self.wndItem.SetUsableSlotOnTopWnd(i)

	def GetCategoryIndex(self):
		return self.categoryPageIndex

	def SetCategoryPage(self,index):
		# old category close!
		if self.categoryPageIndex == 7:
			net.SendChatPacket("/safebox_close")
		elif self.categoryPageIndex == 8:
			net.SendChatPacket("/mall_close")

		self.categoryPageIndex = index

		for i in xrange(len(self.inventory_special)):
			self.inventory_special[i].SetUp()
		self.inventory_special[index-1].Down()

		if index >= 7:
			self.SetInventoryPage(0)

			for x in xrange(18):
				self.lock[x].Hide()
			self.ClearSlot()
			self.GetChild("loadingImage").Show()

			if index == 7:#safebox
				net.SendChatPacket("/safebox_password 000000")
			elif index == 8:#mall
				net.SendChatPacket("/mall_password 000000")
		else:
			self.wndItem.Show()
			self.GetChild("loadingImage").Hide()
			for i in xrange(len(self.inventoryTab)):
				self.inventoryTab[i].Show()

			for x in xrange(18):
				self.lock[x].SetUpVisual("d:/ymir work/drakon2/inventory/0.tga")
				self.lock[x].SetOverVisual("d:/ymir work/drakon2/inventory/0.tga")
				self.lock[x].SetDownVisual("d:/ymir work/drakon2/inventory/0.tga")
				self.lock[x].Show()
				# if self.categoryPageIndex == 6:
					# self.lock[x].Hide()

			self.RefreshItemSlot()

	def SetInventoryPage(self, page):
		self.inventoryPageIndex = page
		for i in xrange(len(self.inventoryTab)):
			self.inventoryTab[i].SetUp()
		self.inventoryTab[page].Down()

		if self.categoryPageIndex <= 6:
			self.RefreshBagSlotWindow()
		elif self.categoryPageIndex == 7:#safebox.
			self.RefreshSafebox()
		elif self.categoryPageIndex == 8:#mall.
			self.RefreshMall()

	def ShowWindow(self, size):
		if self.categoryPageIndex >= 7:

			pageCount = max(1, size / safebox.SAFEBOX_SLOT_Y_COUNT)
			pageCount = min(4, pageCount)
			
			self.pagecount = pageCount

			for i in xrange(len(self.inventoryTab)):
				self.inventoryTab[i].Hide()

			if pageCount > len(self.inventoryTab):
				pageCount = len(self.inventoryTab)
			
			self.pagecount = pageCount

			for j in xrange(pageCount):
				self.inventoryTab[j].Show()

			self.wndItem.Show()
			self.GetChild("loadingImage").Hide()
			self.RefreshStatus()

	def __LocalPosToGlobalPos(self, local):
		return self.inventoryPageIndex*safebox.SAFEBOX_PAGE_SIZE+local
	
	def __LocalPosToGlobalPosWithPage(self, index, local):
		return index*safebox.SAFEBOX_PAGE_SIZE+local

	def RefreshSafebox(self):
		# LoadGrid

		self.grid.reset()
		for j in xrange(4):
			for i in xrange(safebox.SAFEBOX_PAGE_SIZE):
				slotIndex = self.__LocalPosToGlobalPosWithPage(j,i)
				itemCount = safebox.GetItemCount(slotIndex)
				if itemCount == 0:
					continue
				itemVnum = safebox.GetItemID(slotIndex)
				if itemVnum == 0:
					continue
				item.SelectItem(itemVnum)
				(w, h) = item.GetItemSize()
				self.grid.put(slotIndex,w, h)

		for i in xrange(safebox.SAFEBOX_PAGE_SIZE):
			slotIndex = self.__LocalPosToGlobalPos(i)
			itemCount = safebox.GetItemCount(slotIndex)
			if itemCount <= 1:
				itemCount = 0
			self.wndItem.SetItemSlot(i, safebox.GetItemID(slotIndex), itemCount)
			self.wndItem.DeactivateSlotOld(i)
		self.wndItem.RefreshSlot()

	def CommandCloseSafebox(self):
		#if need something after close safebox!..
		pass

	def RefreshMall(self):
		for i in xrange(safebox.GetMallSize()):
			itemID = safebox.GetMallItemID(i)
			itemCount = safebox.GetMallItemCount(i)
			if itemCount <= 1:
				itemCount = 0
			self.wndItem.SetItemSlot(i, itemID, itemCount)
			self.wndItem.DeactivateSlotOld(i)
		self.wndItem.RefreshSlot()
	def CommandCloseMall(self):
		#if need something after close mall!..
		pass

	def ClearSlot(self):
		for i in xrange(self.GetPageSizeCategory()):
			self.wndItem.ClearSlot(i)
		self.wndItem.Hide()

	def RefreshBagSlotWindow(self):

		inven_type = self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"]

		self.grid.reset()
		for j in xrange(4):
			for i in xrange(self.GetPageSizeCategory()):
				slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPosWithPage(j,i)
				itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotNumber)
				if itemCount == 0:
					continue
				itemVnum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotNumber)
				if itemVnum == 0:
					continue
				item.SelectItem(itemVnum)
				(w, h) = item.GetItemSize()
				self.grid.put(slotNumber,w, h)

		for i in xrange(self.GetPageSizeCategory()):
			slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(i)
			itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotNumber)
			if 0 == itemCount:
				self.wndItem.ClearSlot(i)
				continue
			elif 1 == itemCount:
				itemCount = 0
			itemVnum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotNumber)
			self.wndItem.SetItemSlot(i, itemVnum, itemCount)

			if itemVnum >= 91170 and itemVnum <= 91172:
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]
				if metinSocket[0]-app.GetGlobalTimeStamp() < 0:
					self.wndItem.SetUnusableSlot(i)

			if itemVnum == 0 and [inven_type,i] in self.liHighlightedItems:
				self.liHighlightedItems.remove(slotNumber)
			
			if app.ENABLE_IKASHOP_RENEWAL:
				if app.EXTEND_IKASHOP_PRO:
					self.wndItem.EnableSlot(i)
					board = ikashop.GetBusinessBoard()
					if board and (board.IsShow() or board.IsCreatingAuction()):
						item.SelectItem(itemVnum)
						if item.IsAntiFlag(item.ANTIFLAG_GIVE) or item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
							self.wndItem.DisableSlot(i)

			if app.WJ_ENABLE_TRADABLE_ICON:
				self.RefreshMarkSlots(i)

		self.__RefreshHighlights()

		if app.WJ_ENABLE_TRADABLE_ICON:
			map(lambda wnd:wnd.RefreshLockedSlot(), self.bindWnds)
			
		self.wndItem.RefreshSlot()
		if ENABLE_SEARCH_FIELD:
			if self.wndName.GetText() != "":
				self.OnValueUpdateSearch()

	def HighlightSlot(self, inven_type, slot):
		if not [inven_type,slot] in self.liHighlightedItems:
			self.liHighlightedItems.append([inven_type,slot])
			#chat.AppendChat(1, "Go on slot %d" % slot)

	def __RefreshHighlights(self):
		inven_type = self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"]
		for i in xrange(player.INVENTORY_PAGE_SIZE):
			slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(i)
			if [inven_type,slotNumber] in self.liHighlightedItems:
				self.wndItem.ActivateSlotOld(i)
			else:
				self.wndItem.DeactivateSlotOld(i)

	def RefreshItemSlot(self):
		if self.categoryPageIndex <= 6:
			self.RefreshBagSlotWindow()
		elif self.categoryPageIndex == 7:#safebox.
			self.RefreshSafebox()
		elif self.categoryPageIndex == 8:#mall.
			self.RefreshMall()

	def RefreshStatus(self):
		if self.categoryPageIndex <= 6:
			self.RefreshBagSlotWindow()
		elif self.categoryPageIndex == 7:#safebox.
			self.RefreshSafebox()
		elif self.categoryPageIndex == 8:#mall.
			self.RefreshMall()

	def OnUpdate(self):
		if self.GetCategoryIndex() >= 7:
			image = self.GetChild("loadingImage")
			if image.IsShow():
				self.rotationImage += 5
				image.SetRotation(self.rotationImage)
		
		else:
			if constInfo.Inventory_Locked["Active"]:
				if constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())] >= 18:
					for i in xrange(18):
						self.lock[i].Hide()
						# if self.categoryPageIndex == 6:
							# self.lock[i].Hide()
				else:
					if self.inventoryPageIndex >= 2:
						self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetUpVisual("d:/ymir work/drakon2/inventory/1.tga")
						self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetOverVisual("d:/ymir work/drakon2/inventory/1.tga")
						self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetDownVisual("d:/ymir work/drakon2/inventory/1.tga")

						for i in xrange(9):
							self.lock[i].Show()
							if constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())] > i:
								self.lock[i].Hide()
							# if self.categoryPageIndex == 6:
								# self.lock[i].Hide()
					else:
						for i in xrange(9):
							self.lock[i].Hide()
						# if self.categoryPageIndex == 6:
							# self.lock[i].Hide()

					if self.inventoryPageIndex == 3:
						self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetUpVisual("d:/ymir work/drakon2/inventory/1.tga")
						self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetOverVisual("d:/ymir work/drakon2/inventory/1.tga")
						self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetDownVisual("d:/ymir work/drakon2/inventory/1.tga")

						for i in xrange(9,18):
							self.lock[i].Show()
							if constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())] > i:
								self.lock[i].Hide()
							# if self.categoryPageIndex == 6:
								# self.lock[i].Hide()

					else:
						for i in xrange(9,18):
							self.lock[i].Hide()
						# if self.categoryPageIndex == 6:
							# self.lock[i].Hide()

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def SellItem(self):
		if self.sellingSlotitemIndex == player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],self.sellingSlotNumber):
			if self.sellingSlotitemCount == player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],self.sellingSlotNumber):
				net.SendShopSellPacketNew(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], self.sellingSlotNumber, self.questionDialog.count)
				#snd.PlaySound("sound/ui/money.wav")
		self.OnCloseQuestionDialog()

	def OnDetachMetinFromItem(self):
		if None == self.questionDialog:
			return
		self.__SendUseItemToItemPacket(self.questionDialog.sourcePos, self.questionDialog.targetPos)
		self.OnCloseQuestionDialog()

	def OnCloseQuestionDialog(self):
		if not self.questionDialog:
			return

		self.questionDialog.Close()
		self.questionDialog = None
		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

	def __OpenQuestionDialog(self, srcItemPos, dstItemPos):
		self.dlgQuestion = uiCommon.QuestionDialog2()

		self.dlgQuestion.SetAcceptEvent(lambda arg=srcItemPos, arg1=dstItemPos : self.__Accept(arg,arg1))
		self.dlgQuestion.SetCancelEvent(ui.__mem_func__(self.__Cancel))

		self.dlgQuestion.SetText1(localeInfo.DRAGON_SOUL_UNEQUIP_WARNING1)
		self.dlgQuestion.SetText2(localeInfo.DRAGON_SOUL_UNEQUIP_WARNING2)

		self.dlgQuestion.Open()

	def __Accept(self,srcItemPos,dstItemSlotPos):
		(attachedInvenType,attachedSlotPos) = srcItemPos
		(attachedInvenType1, selectedSlotPos, attachedCount) = dstItemSlotPos
		self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, attachedInvenType1, selectedSlotPos, attachedCount)
		self.dlgQuestion.Close()

	def __Cancel(self):
		self.dlgQuestion.Close()


	def SelectEmptySlot(self, selectedSlotPos):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
			return

		if mouseModule.mouseController.isAttached():
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()
			attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()

			if self.categoryPageIndex == 7:#safebox.
				selectedSlotPos = self.__LocalPosToGlobalPos(selectedSlotPos)

				if player.SLOT_TYPE_SAFEBOX == attachedSlotType:
					net.SendSafeboxItemMovePacket(attachedSlotPos, selectedSlotPos, 0)
					#snd.PlaySound("sound/ui/drop.wav")
				else:
					attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
					if player.RESERVED_WINDOW == attachedInvenType:
						return
					if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
						net.SendSafeboxSaveMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
						#snd.PlaySound("sound/ui/money.wav")
					else:
						net.SendSafeboxCheckinPacket(attachedInvenType, attachedSlotPos, selectedSlotPos)
						#snd.PlaySound("sound/ui/drop.wav")
			elif self.categoryPageIndex == 7:#mall.
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MALL_CANNOT_INSERT)
			else:
				selectedSlotPos = self.__InventoryLocalSlotPosToGlobalSlotPos(selectedSlotPos)
				slot_type_list = [player.SLOT_TYPE_INVENTORY,player.SLOT_TYPE_UPGRADE_INVENTORY,player.SLOT_TYPE_BOOK_INVENTORY,player.SLOT_TYPE_STONE_INVENTORY,player.SLOT_TYPE_CHANGE_INVENTORY,player.SLOT_TYPE_COSTUME_INVENTORY,player.SLOT_TYPE_CHEST_INVENTORY]
				if attachedSlotType in slot_type_list:
					attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
					itemCount = player.GetItemCount(attachedInvenType,attachedSlotPos)
					attachedCount = mouseModule.mouseController.GetAttachedItemCount()
					if player.IsDSEquipmentSlot(attachedInvenType, attachedSlotPos):
						srcItemPos = (attachedInvenType, attachedSlotPos)
						dstItemPos = (self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)
						self.__OpenQuestionDialog(srcItemPos, dstItemPos)
					else:
						self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)
					if item.IsRefineScroll(attachedItemIndex):
						self.wndItem.SetUseMode(False)
				if player.SLOT_TYPE_CHANGE_EQUIP == attachedSlotType:
					attachedCount = mouseModule.mouseController.GetAttachedItemCount()
					net.SendItemMovePacket(player.CHANGE_EQUIP, attachedSlotPos, player.INVENTORY, selectedSlotPos, attachedCount)
				elif app.ENABLE_SWITCHBOT_WORLDARD and player.SLOT_TYPE_SWITCHBOT == attachedSlotType:
					attachedCount = mouseModule.mouseController.GetAttachedItemCount()
					net.SendItemMovePacket(player.SWITCHBOT, attachedSlotPos, player.INVENTORY, selectedSlotPos, attachedCount)
				elif player.SLOT_TYPE_PRIVATE_SHOP == attachedSlotType:
					mouseModule.mouseController.RunCallBack("INVENTORY")
				elif player.SLOT_TYPE_SHOP == attachedSlotType:
					net.SendShopBuyPacket(attachedSlotPos)
				elif player.RESERVED_WINDOW != attachedInvenType:
					if self.dlgPickMoney and self.dlgPickMoney.IsSplitAll():
						if self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.UPGRADE_INVENTORY:
							net.SendChatPacket("/split_storage_items %d %d %d a" % (attachedSlotPos, attachedCount, selectedSlotPos))
						elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.BOOK_INVENTORY:
							net.SendChatPacket("/split_storage_items %d %d %d b" % (attachedSlotPos, attachedCount, selectedSlotPos))
						elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.STONE_INVENTORY:
							net.SendChatPacket("/split_storage_items %d %d %d c" % (attachedSlotPos, attachedCount, selectedSlotPos))
						elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.CHANGE_INVENTORY:
							net.SendChatPacket("/split_storage_items %d %d %d d" % (attachedSlotPos, attachedCount, selectedSlotPos))
						elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.COSTUME_INVENTORY:
							net.SendChatPacket("/split_storage_items %d %d %d e" % (attachedSlotPos, attachedCount, selectedSlotPos))
						elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.CHEST_INVENTORY:
							net.SendChatPacket("/split_storage_items %d %d %d m" % (attachedSlotPos, attachedCount, selectedSlotPos))

						self.dlgPickMoney.SplitClear()

				elif player.SLOT_TYPE_SAFEBOX == attachedSlotType:
					if player.ITEM_MONEY == attachedItemIndex:
						net.SendSafeboxWithdrawMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
						#snd.PlaySound("sound/ui/money.wav")
					else:
						net.SendSafeboxCheckoutPacket(attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos)
				elif player.SLOT_TYPE_MALL == attachedSlotType:
					net.SendMallCheckoutPacket(attachedSlotPos, selectedSlotPos)
			mouseModule.mouseController.DeattachObject()

	def AttachItemFromInventoryToSpecial(self, slotWindow, slotIndex):
		attachedItemID = player.GetItemIndex(slotWindow, slotIndex)
		item.SelectItem(attachedItemID)
		if item.IsAntiFlag(item.ITEM_ANTIFLAG_SAFEBOX):
			return False
		(w, h) = item.GetItemSize()
		pos = self.grid.find_blank(w, h)
		if pos == -1:
			return False
		self.__SendMoveItemPacket(slotWindow, slotIndex, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], pos, player.GetItemCount(slotWindow, slotIndex))
		return True

	def AttachItemFromInventoryToSafebox(self, slotWindow, slotIndex):
		attachedItemID = player.GetItemIndex(slotWindow, slotIndex)
		item.SelectItem(attachedItemID)
		if item.IsAntiFlag(item.ITEM_ANTIFLAG_SAFEBOX):
			return False
		(w, h) = item.GetItemSize()
		pos = self.grid.find_blank(w, h)
		if pos == -1:
			return False
		net.SendSafeboxCheckinPacket(slotWindow, slotIndex, pos)
		return True

	def SelectItemSlot(self, itemSlotIndex):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
			return

		if self.categoryPageIndex == 7:#safebox.
		
			selectedSlotPos = self.__LocalPosToGlobalPos(itemSlotIndex)
			selectedItemID = safebox.GetItemID(selectedSlotPos)

			if app.IsPressed(app.DIK_LCONTROL):
				if self.interface:
					if self.interface.AttachItemFromSafebox(selectedSlotPos, player.INVENTORY, selectedItemID):
						return

			if mouseModule.mouseController.isAttached():
				attachedSlotType = mouseModule.mouseController.GetAttachedType()
				if player.SLOT_TYPE_INVENTORY == attachedSlotType:
					if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
						net.SendSafeboxSaveMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
						#snd.PlaySound("sound/ui/money.wav")
					else:
						attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
						#net.SendSafeboxCheckinPacket(attachedSlotPos, selectedSlotPos)
						#snd.PlaySound("sound/ui/drop.wav")
				mouseModule.mouseController.DeattachObject()
			else:
				curCursorNum = app.GetCursor()
				if app.SELL == curCursorNum:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SAFEBOX_SELL_DISABLE_SAFEITEM)
				elif app.BUY == curCursorNum:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_BUY_INFO)
				else:
					selectedItemID = safebox.GetItemID(selectedSlotPos)
					mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_SAFEBOX, selectedSlotPos, selectedItemID)
					#snd.PlaySound("sound/ui/pick.wav")
		
		elif self.categoryPageIndex == 8:#mall
			if mouseModule.mouseController.isAttached():
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MALL_CANNOT_INSERT)
				mouseModule.mouseController.DeattachObject()
			else:
				curCursorNum = app.GetCursor()
				selectedItemID = safebox.GetMallItemID(itemSlotIndex)
				mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_MALL, itemSlotIndex, selectedItemID)
				#snd.PlaySound("sound/ui/pick.wav")
		else:

			itemSlotIndex = self.__InventoryLocalSlotPosToGlobalSlotPos(itemSlotIndex)

			if ENABLE_SEARCH_FIELD:
				if self.wndName.GetText() and self.wndItemSRC.dictItems.has_key(itemSlotIndex):
					window, pos = self.wndItemSRC.dictItems[itemSlotIndex]
					itemSlotIndex = pos
					
					selectedItemVNum = player.GetItemIndex(window, itemSlotIndex)
					itemCount = player.GetItemCount(window, itemSlotIndex)


			# import dbg
			# dbg.TraceError("Heelo ")
			if app.IsPressed(app.DIK_LCONTROL):
				if self.interface and self.interface.AttachSpecialToInv(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex):
					return

			if app.IsPressed(app.DIK_LALT) and app.ENABLE_RENDER_TARGET:
				self.interface.OpenRenderTargetWindow(0, player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex))
				return

			if mouseModule.mouseController.isAttached():
				attachedSlotType = mouseModule.mouseController.GetAttachedType()
				attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
				attachedItemVID = mouseModule.mouseController.GetAttachedItemIndex()
				if app.ENABLE_SPECIAL_STORAGE:
					if player.SLOT_TYPE_INVENTORY == attachedSlotType or player.SLOT_TYPE_STONE_INVENTORY == attachedSlotType or player.SLOT_TYPE_CHANGE_INVENTORY == attachedSlotType or player.SLOT_TYPE_UPGRADE_INVENTORY == attachedSlotType or player.SLOT_TYPE_BOOK_INVENTORY == attachedSlotType or player.SLOT_TYPE_COSTUME_INVENTORY == attachedSlotType or player.SLOT_TYPE_CHEST_INVENTORY:
						self.__DropSrcItemToDestItemInInventory(attachedItemVID, attachedSlotPos, itemSlotIndex, attachedSlotType)
				else:
					if player.SLOT_TYPE_INVENTORY == attachedSlotType:
						self.__DropSrcItemToDestItemInInventory(attachedItemVID, attachedSlotPos, itemSlotIndex, attachedSlotType)
				mouseModule.mouseController.DeattachObject()
			else:
				curCursorNum = app.GetCursor()
				if app.SELL == curCursorNum:
					self.__SellItem(itemSlotIndex)
				elif app.IsPressed(app.DIK_LSHIFT) and app.ENABLE_RENDER_TARGET and self.categoryPageIndex == 5:
					self.interface.OpenRenderTargetWindow(0, player.GetItemIndex(itemSlotIndex))
				elif app.BUY == curCursorNum:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_BUY_INFO)
				elif app.IsPressed(app.DIK_LALT):
					link = player.GetItemLink(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)
					ime.PasteString(link)
				elif app.IsPressed(app.DIK_LSHIFT):
					itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotIndex)
					if itemCount > 1:
						self.dlgPickMoney.SetTitleName(localeInfo.PICK_ITEM_TITLE)
						self.dlgPickMoney.SetAcceptEvent(ui.__mem_func__(self.OnPickItem))
						self.dlgPickMoney.Open(itemCount)
						self.dlgPickMoney.itemGlobalSlotIndex = itemSlotIndex
				elif app.IsPressed(app.DIK_LCONTROL):
					itemIndex = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotIndex)
					if not self.GetIsCategorySpecial():
						if True == item.CanAddToQuickSlotItem(itemIndex):
							player.RequestAddToEmptyLocalQuickSlot(player.SLOT_TYPE_INVENTORY, itemSlotIndex)
						else:
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.QUICKSLOT_REGISTER_DISABLE_ITEM)
				else:
					selectedItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotIndex)
					itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotIndex)
					mouseModule.mouseController.AttachObject(self, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["slot"], itemSlotIndex, selectedItemVNum, itemCount)
					if self.__IsUsableItemToItem(selectedItemVNum, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex):
						self.wndItem.SetUseMode(True)
					else:
						self.wndItem.SetUseMode(False)
					#snd.PlaySound("sound/ui/pick.wav")
			if ENABLE_SEARCH_FIELD:
				if self.wndName.GetText() != "":
					self.OnValueUpdateSearch()

	def OnPickItem(self, count):
		itemSlotIndex = self.dlgPickMoney.itemGlobalSlotIndex
		selectedItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotIndex)
		mouseModule.mouseController.AttachObject(self, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["slot"], itemSlotIndex, selectedItemVNum, count)

	def __DropSrcItemToDestItemInInventory(self, srcItemVID, srcItemSlotPos, dstItemSlotPos,attachedSlotType):
		destItemVnum = player.GetItemIndex(dstItemSlotPos)
		item.SelectItem(destItemVnum)
		destItemType = item.GetItemType()
		destItemSubType = item.GetItemSubType()

		if app.ENABLE_SPECIAL_STORAGE:

			if srcItemSlotPos == dstItemSlotPos and (attachedSlotType != player.SLOT_TYPE_STONE_INVENTORY and attachedSlotType != player.SLOT_TYPE_CHANGE_INVENTORY and attachedSlotType != player.SLOT_TYPE_CHEST_INVENTORY):
				return

			if srcItemSlotPos == dstItemSlotPos and item.IsMetin(srcItemVID) and self.GetIsCategorySpecial() and attachedSlotType == player.SLOT_TYPE_STONE_INVENTORY:
				return

			if srcItemSlotPos == dstItemSlotPos and (item.GetUseType(srcItemVID) == "USE_CHANGE_ATTRIBUTE" or item.GetUseType(srcItemVID) == "USE_CHANGE_ATTRIBUTE2") and self.GetIsCategorySpecial() and attachedSlotType == player.SLOT_TYPE_CHANGE_INVENTORY:
				return

		else:
			if srcItemSlotPos == dstItemSlotPos:
				return
		
		if srcItemVID == 71051 or srcItemVID == 71052 or srcItemVID == 90000:
			self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos)

		if srcItemVID == 39022 or srcItemVID == 71032 or srcItemVID == 76009:
			constInfo.ADDPERCENT = 10

		if app.ENABLE_SOULBIND_SYSTEM and item.IsSealScroll(srcItemVID):
			self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos)

		elif item.IsRefineScroll(srcItemVID):
			return
			self.RefineItem(srcItemSlotPos, dstItemSlotPos)
			self.wndItem.SetUseMode(False)

		elif item.IsMetin(srcItemVID):
			self.AttachMetinToItem(srcItemSlotPos, dstItemSlotPos)

		elif destItemType == item.ITEM_TYPE_PET and destItemSubType == item.PET_LEVELABLE:
			if srcItemVID == 55008:
				self.interface.OpenChangeNameWindow(srcItemSlotPos,dstItemSlotPos,destItemVnum, 1)
			elif srcItemVID == 72325:
				self.DetachMetinFromItem(srcItemSlotPos, dstItemSlotPos)
			elif (srcItemVID == 55001 or srcItemVID == 55033 or (srcItemVID >= 55010 and srcItemVID <= 55031)):
				self.questionDialog = uiCommon.QuestionDialog()
				if 55001 == srcItemVID:
					self.questionDialog.SetText(localeInfo.PET_PROTEIN_QUESTION_TEXT)
				elif 55033 == srcItemVID:
					self.questionDialog.SetText(localeInfo.PET_REMOVE_QUESTION_TEXT)
				else:
					self.questionDialog.SetText(localeInfo.INVENTORY_REALLY_USE_ITEM % item.GetItemName())
				self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnAccept))
				self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnCancel))
				self.questionDialog.Open()
				self.questionDialog.srcItemSlotPos = srcItemSlotPos
				self.questionDialog.dstItemSlotPos = dstItemSlotPos
				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

		elif item.IsDetachScroll(srcItemVID):
			self.DetachMetinFromItem(srcItemSlotPos, dstItemSlotPos)

		elif item.IsKey(srcItemVID):
			self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos)

		elif (player.GetItemFlags(srcItemSlotPos) & ITEM_FLAG_APPLICABLE) == ITEM_FLAG_APPLICABLE:
			self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos)

		elif item.GetUseType(srcItemVID) in self.USE_TYPE_TUPLE:
			self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos, srcItemVID, attachedSlotType)

		else:
			if player.IsEquipmentSlot(dstItemSlotPos):
				if item.IsEquipmentVID(srcItemVID):
					self.__UseItem(srcItemSlotPos)
			else:
				net.SendItemMovePacket(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], srcItemSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], dstItemSlotPos, 0)

	def __SellItem(self, itemSlotPos):
		if not player.IsEquipmentSlot(itemSlotPos):
			self.sellingSlotNumber = itemSlotPos
			itemIndex = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotPos)
			itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotPos)


			self.sellingSlotitemIndex = itemIndex
			self.sellingSlotitemCount = itemCount

			item.SelectItem(itemIndex)
			## 안티 플레그 검사 빠져서 추가
			## 20140220
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

			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

	def __OnClosePopupDialog(self):
		self.pop = None

	def RefineItem(self, scrollSlotPos, targetSlotPos):
		scrollIndex = player.GetItemIndex(scrollSlotPos)
		targetIndex = player.GetItemIndex(targetSlotPos)

		if player.REFINE_OK != player.CanRefine(scrollIndex, targetSlotPos):
			return

		if app.ENABLE_REFINE_RENEWAL:
			constInfo.AUTO_REFINE_TYPE = 1
			constInfo.AUTO_REFINE_DATA["ITEM"][0] = scrollSlotPos
			constInfo.AUTO_REFINE_DATA["ITEM"][1] = targetSlotPos

		###########################################################
		self.__SendUseItemToItemPacket(scrollSlotPos, targetSlotPos)
		#net.SendItemUseToItemPacket(scrollSlotPos, targetSlotPos)
		return
		###########################################################

		###########################################################
		#net.SendRequestRefineInfoPacket(targetSlotPos)
		#return
		###########################################################

		result = player.CanRefine(scrollIndex, targetSlotPos)

		if player.REFINE_ALREADY_MAX_SOCKET_COUNT == result:
			#snd.PlaySound("sound/ui/jaeryun_fail.wav")
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_NO_MORE_SOCKET)

		elif player.REFINE_NEED_MORE_GOOD_SCROLL == result:
			#snd.PlaySound("sound/ui/jaeryun_fail.wav")
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_NEED_BETTER_SCROLL)

		elif player.REFINE_CANT_MAKE_SOCKET_ITEM == result:
			#snd.PlaySound("sound/ui/jaeryun_fail.wav")
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_SOCKET_DISABLE_ITEM)

		elif player.REFINE_NOT_NEXT_GRADE_ITEM == result:
			#snd.PlaySound("sound/ui/jaeryun_fail.wav")
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_UPGRADE_DISABLE_ITEM)

		elif player.REFINE_CANT_REFINE_METIN_TO_EQUIPMENT == result:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_EQUIP_ITEM)

		if player.REFINE_OK != result:
			return

		self.refineDialog.Open(scrollSlotPos, targetSlotPos)

	def DetachMetinFromItem(self, scrollSlotPos, targetSlotPos):
		scrollIndex = player.GetItemIndex(scrollSlotPos)
		targetIndex = player.GetItemIndex(targetSlotPos)
		if app.ENABLE_SASH_SYSTEM:
			if not player.CanDetach(scrollIndex, targetSlotPos):
				item.SelectItem(scrollIndex)
				if item.GetValue(0) == sash.CLEAN_ATTR_VALUE0:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SASH_FAILURE_CLEAN)
				else:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_METIN_INSEPARABLE_ITEM)
				
				return
		else:
			if not player.CanDetach(scrollIndex, targetSlotPos):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_METIN_INSEPARABLE_ITEM)
				return
		
		self.questionDialog = uiCommon.QuestionDialog()
		self.questionDialog.SetText(localeInfo.REFINE_DO_YOU_SEPARATE_METIN)
		if app.ENABLE_SASH_SYSTEM:
			item.SelectItem(targetIndex)
			if item.GetItemType() == item.ITEM_TYPE_COSTUME and item.GetItemSubType() == item.COSTUME_TYPE_SASH:
				item.SelectItem(scrollIndex)
				if item.GetValue(0) == sash.CLEAN_ATTR_VALUE0:
					self.questionDialog.SetText(localeInfo.SASH_DO_YOU_CLEAN)
		
		self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnDetachMetinFromItem))
		self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		self.questionDialog.Open()
		self.questionDialog.sourcePos = scrollSlotPos
		self.questionDialog.targetPos = targetSlotPos

	def AttachMetinToItem(self, metinSlotPos, targetSlotPos):
		return#special-inv bug

		if app.ENABLE_SPECIAL_STORAGE:
			metinIndex = player.GetItemIndex(player.STONE_INVENTORY, metinSlotPos)
		else:
			metinIndex = player.GetItemIndex(metinSlotPos)
		targetIndex = player.GetItemIndex(targetSlotPos)

		item.SelectItem(metinIndex)
		itemName = item.GetItemName()

		result = player.CanAttachMetin(metinIndex, targetSlotPos)

		if player.ATTACH_METIN_NOT_MATCHABLE_ITEM == result:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_CAN_NOT_ATTACH(itemName))

		if player.ATTACH_METIN_NO_MATCHABLE_SOCKET == result:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_NO_SOCKET(itemName))

		elif player.ATTACH_METIN_NOT_EXIST_GOLD_SOCKET == result:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_NO_GOLD_SOCKET(itemName))

		elif player.ATTACH_METIN_CANT_ATTACH_TO_EQUIPMENT == result:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_EQUIP_ITEM)

		if player.ATTACH_METIN_OK != result:
			return

		self.attachMetinDialog.Open(metinSlotPos, targetSlotPos)



	def OverOutItem(self):
		self.wndItem.SetUsableItem(False)
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OverInItem(self, overSlotPos):

		if self.categoryPageIndex == 7:#safebox.
			self.wndItem.SetUsableItem(False)
			slotIndex = self.__LocalPosToGlobalPos(overSlotPos)
			if self.tooltipItem:
				self.tooltipItem.SetSafeBoxItem(slotIndex)

		elif self.categoryPageIndex == 8:#mall.
			if self.tooltipItem:
				self.tooltipItem.SetMallItem(overSlotPos)
		else:
			overSlotPosGlobal = self.__InventoryLocalSlotPosToGlobalSlotPos(overSlotPos)
			self.wndItem.SetUsableItem(False)


			inven_type = self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"]
			if [inven_type, overSlotPosGlobal] in self.liHighlightedItems:
				self.liHighlightedItems.remove([inven_type, overSlotPosGlobal])
				self.wndItem.DeactivateSlotOld(overSlotPosGlobal)

			if mouseModule.mouseController.isAttached():
				attachedItemType = mouseModule.mouseController.GetAttachedType()
				if app.ENABLE_SPECIAL_STORAGE:
					if player.SLOT_TYPE_INVENTORY == attachedItemType or player.SLOT_TYPE_STONE_INVENTORY == attachedItemType or player.SLOT_TYPE_CHANGE_INVENTORY == attachedItemType or player.SLOT_TYPE_COSTUME_INVENTORY == attachedItemType or player.SLOT_TYPE_CHEST_INVENTORY:
						attachedSlotType = mouseModule.mouseController.GetAttachedType()
						attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
						attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()

						if self.__CanUseSrcItemToDstItem(attachedItemVNum, player.SlotTypeToInvenType(attachedSlotType), attachedSlotPos, overSlotPos):
							self.wndItem.SetUsableItem(True)
							self.wndItem.SetUseMode(True)
							self.ShowToolTip(overSlotPos)
							return
				else:
					if player.SLOT_TYPE_INVENTORY == attachedItemType:

						attachedSlotType = mouseModule.mouseController.GetAttachedType()
						attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
						attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()

						if self.__CanUseSrcItemToDstItem(attachedItemVNum, player.SlotTypeToInvenType(attachedSlotType), attachedSlotPos, overSlotPos):
							self.wndItem.SetUsableItem(True)
							self.ShowToolTip(overSlotPos)
							return

			self.ShowToolTip(overSlotPosGlobal)


	def __IsUsableItemToItem(self, srcItemVNum, srcSlotType, srcSlotPos):
		"다른 아이템에 사용할 수 있는 아이템인가?"

		if item.IsRefineScroll(srcItemVNum):
			return False
		elif app.ENABLE_SOULBIND_SYSTEM and (item.IsSealScroll(srcItemVNum) or item.IsUnSealScroll(srcItemVNum)):
			return True
		elif item.IsMetin(srcItemVNum):
			return True
		elif item.IsDetachScroll(srcItemVNum):
			return True
		if srcItemVNum >= 55701 and srcItemVNum <= 55704:
			return True
		if srcItemVNum == 71051 or srcItemVNum == 71052 or srcItemVNum == 90000:
			return True
		if srcItemVNum == 55001:
			return True
		elif item.IsKey(srcItemVNum):
			return True
		elif srcItemVNum >= 55701 and srcItemVNum <= 55715:
			return TRUE
		elif srcItemVNum == 55008:
			return TRUE
		elif srcItemVNum == 55001:
			return TRUE
		elif srcItemVNum == 55033:
			return TRUE
		elif srcItemVNum >= 55101 and srcItemVNum <= 55121:
			return TRUE
		elif srcItemVNum >= 55010 and srcItemVNum <= 55031:
			return TRUE
		elif (player.GetItemFlags(srcSlotType, srcSlotPos) & ITEM_FLAG_APPLICABLE) == ITEM_FLAG_APPLICABLE:
			return True
		else:
			if item.GetUseType(srcItemVNum) in self.USE_TYPE_TUPLE:
				return True

		return False

	def __CanUseSrcItemToDstItem(self, srcItemVNum, srcSlotType, srcSlotPos, dstSlotPos):
		item.SelectItem(player.GetItemIndex(dstSlotPos))
		destItemType = item.GetItemType()
		destItemSubType = item.GetItemSubType()

		if app.ENABLE_SPECIAL_STORAGE:
			#if srcSlotPos == dstSlotPos and not item.IsMetin(srcItemVNum):
			if srcSlotPos == dstSlotPos and (not item.IsMetin(srcItemVNum) and (item.GetUseType(srcItemVNum) != "USE_CHANGE_ATTRIBUTE" or item.GetUseType(srcItemVNum) != "USE_CHANGE_ATTRIBUTE2")):
				return False
			if item.IsMetin(srcItemVNum):
				return True
		else:
			if srcSlotPos == dstSlotPos:
				return False

		if srcItemVNum >= 55701 and  srcItemVNum <= 55704 and player.GetItemIndex(dstSlotPos) == 55002:
			return True		

		if srcItemVNum == 55001 and player.GetItemIndex(dstSlotPos) >= 55701 and player.GetItemIndex(dstSlotPos) <= 55704:
			return True
			
		if srcItemVNum == 71051 or srcItemVNum == 71052 or srcItemVNum == 90000:
			return True
			
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if item.IsRefineScroll(srcItemVNum):
			if player.REFINE_OK == player.CanRefine(srcItemVNum, dstSlotPos):
				return True

		elif destItemType == item.ITEM_TYPE_PET and destItemSubType == item.PET_LEVELABLE:
			if srcItemVNum == 55008:
				return TRUE
			elif srcItemVNum == 55001:
				return TRUE
			elif srcItemVNum == 55033:
				return TRUE
			elif srcItemVNum >= 55101 and srcItemVNum <= 55121:
				return TRUE
			elif srcItemVNum >= 55010 and srcItemVNum <= 55031:
				return TRUE

		elif item.IsMetin(srcItemVNum):
			if player.ATTACH_METIN_OK == player.CanAttachMetin(srcItemVNum, dstSlotPos):
				return True
		elif item.IsDetachScroll(srcItemVNum):
			if player.DETACH_METIN_OK == player.CanDetach(srcItemVNum, dstSlotPos):
				return True
		elif item.IsKey(srcItemVNum):
			if player.CanUnlock(srcItemVNum, dstSlotPos):
				return True
		elif app.ENABLE_SOULBIND_SYSTEM and item.IsSealScroll(srcItemVNum):
			if (item.IsSealScroll(srcItemVNum) and player.CanSealItem(dstSlotPos)) or (item.IsUnSealScroll(srcItemVNum) and player.CanUnSealItem(dstSlotPos)):
				return True
		elif (player.GetItemFlags(srcSlotType, srcSlotPos) & ITEM_FLAG_APPLICABLE) == ITEM_FLAG_APPLICABLE:
			return True
		elif srcItemVNum in (70071,70063,70064):
			return True
		else:
			useType=item.GetUseType(srcItemVNum)

			if "USE_CLEAN_SOCKET" == useType:
				if self.__CanCleanBrokenMetinStone(dstSlotPos):
					return True
			elif "USE_CHANGE_ATTRIBUTE" == useType:
				if self.__CanChangeItemAttrList(dstSlotPos,srcItemVNum):
					return True
			elif app.BL_67_ATTR and "USE_CHANGE_ATTRIBUTE2" == useType:
				if self.__CanChangeItemAttrList2(dstSlotPos):
					return True
			elif "USE_ADD_ATTRIBUTE" == useType:
				if self.__CanAddItemAttr(dstSlotPos):
					return True
			elif "USE_ADD_ATTRIBUTE2" == useType:
				if self.__CanAddItemAttr(dstSlotPos):
					return True
			elif "USE_ADD_ACCESSORY_SOCKET" == useType:
				if self.__CanAddAccessorySocket(dstSlotPos):
					return True
			elif "USE_PUT_INTO_ACCESSORY_SOCKET" == useType:
				if self.__CanPutAccessorySocket(dstSlotPos, srcItemVNum):
					return True;
			elif "USE_ADD_BOOSTER_ATTR" == useType:
				if self.__CanChangeBoosterAttr(dstSlotPos):
					return True
			elif "USE_CHANGE_BOOSTER_ATTR" == useType:
				if self.__CanChangeBoosterAttr(dstSlotPos):
					return True
			elif "USE_ADD_BOOSTER_TIME" == useType:
				return True
			elif "USE_PUT_INTO_BELT_SOCKET" == useType:
				dstItemVNum = player.GetItemIndex(dstSlotPos)
				print "USE_PUT_INTO_BELT_SOCKET", srcItemVNum, dstItemVNum

				item.SelectItem(dstItemVNum)

				if item.ITEM_TYPE_BELT == item.GetItemType():
					return True
			elif "USE_CHANGE_COSTUME_ATTR" == useType:
				if self.__CanChangeCostumeAttrList(dstSlotPos):
					return True
			elif "USE_RESET_COSTUME_ATTR" == useType:
				if self.__CanResetCostumeAttr(dstSlotPos):
					return True

		return False

	def __CanChangeBoosterAttr(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if not item.GetItemType() == item.ITEM_TYPE_BOOSTER:
			return False

		# if not item.GetItemSubType() in (item.BOOSTER_HEAD, item.BOOSTER_ARMOR, item.BOOSTER_WEAPON):
			# return False
			
		attrCount = 0
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemAttribute(dstSlotPos, i) != 0:
				attrCount += 1

		if attrCount<3:
			return True

		return False

	def __CanCleanBrokenMetinStone(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if item.ITEM_TYPE_WEAPON != item.GetItemType():
			return False

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemMetinSocket(dstSlotPos, i) == constInfo.ERROR_METIN_STONE:
				return True

		return False

	if app.BL_67_ATTR:
		def __CanChangeItemAttrList2(self, dstSlotPos):
			return uiAttr67Add.Attr67AddWindow.CantAttachToAttrSlot(dstSlotPos, False)

	def __CanChangeItemAttrList(self, dstSlotPos, srcItemVNum):
		dstItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if item.GetItemType() == item.ITEM_TYPE_DS and srcItemVNum != 71097:
			return False

		elif srcItemVNum == 71097 and item.GetItemType() != item.ITEM_TYPE_DS:
			return False

		elif not item.GetItemType() in (item.ITEM_TYPE_WEAPON, item.ITEM_TYPE_ARMOR, item.ITEM_TYPE_RINGS) and srcItemVNum != 71097:
			return False


		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemAttribute(dstSlotPos, i) != 0:
				return True

		return False

	def __CanChangeCostumeAttrList(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if item.GetItemType() != item.ITEM_TYPE_COSTUME:
			return False

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemAttribute(dstSlotPos, i) != 0:
				return True

		return False

	def __CanResetCostumeAttr(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if item.GetItemType() != item.ITEM_TYPE_COSTUME:
			return False

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemAttribute(dstSlotPos, i) != 0:
				return True

		return False

	def __CanPutAccessorySocket(self, dstSlotPos, mtrlVnum):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)


		if item.GetItemType() != item.ITEM_TYPE_ARMOR and item.GetItemType() != item.ITEM_TYPE_RINGS:
			return False

		if item.GetItemType() == item.ITEM_TYPE_ARMOR:
			if not item.GetItemSubType() in (item.ARMOR_WRIST, item.ARMOR_NECK, item.ARMOR_EAR):
				return False

		curCount = player.GetItemMetinSocket(dstSlotPos, 0)
		maxCount = player.GetItemMetinSocket(dstSlotPos, 1)

		type = item.GetItemSubType()

		if item.GetItemType() == item.ITEM_TYPE_RINGS:
			type = item.GetItemType()

		if mtrlVnum != constInfo.GET_ACCESSORY_MATERIAL_VNUM(dstItemVNum, type):
			return False

		if curCount>=maxCount:
			return False

		return True

	def __CanAddAccessorySocket(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if item.GetItemType() != item.ITEM_TYPE_ARMOR and item.GetItemType() != item.ITEM_TYPE_RINGS:
			return False

		if item.GetItemType() == item.ITEM_TYPE_ARMOR:
			if not item.GetItemSubType() in (item.ARMOR_WRIST, item.ARMOR_NECK, item.ARMOR_EAR):
				return False

		curCount = player.GetItemMetinSocket(dstSlotPos, 0)
		maxCount = player.GetItemMetinSocket(dstSlotPos, 1)

		ACCESSORY_SOCKET_MAX_SIZE = 3
		if maxCount >= ACCESSORY_SOCKET_MAX_SIZE:
			return False

		return True

	def __CanAddItemAttr(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if not item.GetItemType() in (item.ITEM_TYPE_WEAPON, item.ITEM_TYPE_ARMOR, item.ITEM_TYPE_RINGS):
			return False

		attrCount = 0
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemAttribute(dstSlotPos, i) != 0:
				attrCount += 1

		if attrCount<4:
			return True

		return False

	def ShowToolTip(self, slotIndex):
		if None != self.tooltipItem:
			if ENABLE_SEARCH_FIELD:
				if self.wndName.GetText() and self.wndItemSRC.dictItems.has_key(slotIndex):
					window, pos = self.wndItemSRC.dictItems[slotIndex]
					
					self.tooltipItem.SetInventoryItem(pos, window)
					if app.ENABLE_IKASHOP_RENEWAL:
						self.AppendIkashopSellToolTip(window, pos)
				else:
					self.tooltipItem.SetInventoryItem(slotIndex, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"])
					if app.ENABLE_IKASHOP_RENEWAL:
						self.AppendIkashopSellToolTip(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)
			else:
				self.tooltipItem.SetInventoryItem(slotIndex, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"])
				if app.ENABLE_IKASHOP_RENEWAL:
					self.AppendIkashopSellToolTip(player.INVENTORY, slotIndex)

	if app.ENABLE_IKASHOP_RENEWAL:
		def AppendIkashopSellToolTip(self, win, pos):
			board = ikashop.GetBusinessBoard()
			if board and (board.IsShow() or board.IsCreatingAuction()):
				item.SelectItem(player.GetItemIndex(win, pos))
				if not item.IsAntiFlag(item.ANTIFLAG_GIVE) and not item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
					self.tooltipItem.AppendTextLine(localeInfo.IKASHOP_BUSINESS_SELL_ITEM_TOOLTIP)


	def OnTop(self):
		if None != self.tooltipItem:
			self.tooltipItem.SetTop()

		if app.WJ_ENABLE_TRADABLE_ICON:
			map(lambda wnd:wnd.RefreshLockedSlot(), self.bindWnds)
			self.RefreshMarkSlots()

	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def GetExchangeEmptyItemPos(self, itemHeight):
		inventorySize = exchange.EXCHANGE_ITEM_MAX_NUM
		inventoryWidth = 6
		GetBlockedSlots = lambda slot, size: [slot+(round*inventoryWidth) for round in xrange(size)] 
		blockedSlots = [element for sublist in [GetBlockedSlots(slot, item.GetItemSize(item.SelectItem(exchange.GetItemVnumFromSelf(slot)))[1]) for slot in xrange(inventorySize) if exchange.GetItemVnumFromSelf(slot) != 0] for element in sublist] 
		freeSlots = [slot for slot in xrange(inventorySize) if not slot in blockedSlots and not True in [e in blockedSlots for e in [slot+(round*inventoryWidth) for round in xrange(itemHeight)]]] 
		return [freeSlots, -1][len(freeSlots) == 0]

	def UseItemSlot(self, slotIndex):
		curCursorNum = app.GetCursor()
		if app.SELL == curCursorNum:
			return

		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS():
			return

		slotIndex = self.__InventoryLocalSlotPosToGlobalSlotPos(slotIndex)
		ivenType = self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"]
		
		if ENABLE_SEARCH_FIELD:
			if self.wndName.GetText() and self.wndItemSRC.dictItems.has_key(slotIndex):
				window, pos = self.wndItemSRC.dictItems[slotIndex]
				slotIndex = pos
				
				ivenType = window

		if app.ENABLE_SASH_SYSTEM:
			if self.isShowSashWindow():
				sash.Add(ivenType, slotIndex, 255)
				return
		
		if app.ENABLE_IKASHOP_RENEWAL:
			board = ikashop.GetBusinessBoard()
			if board:
				item.SelectItem(player.GetItemIndex(ivenType, slotIndex))
				if not item.IsAntiFlag(item.ANTIFLAG_GIVE) and not item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
					if board.IsCreatingAuction():
						ikashop.AddItemOnAuction(ivenType, slotIndex)
						return
					elif board.IsShow():
						ikashop.AddItemOnShop(ivenType, slotIndex)
						return
			
			if app.EXTEND_IKASHOP_ULTIMATE:
				itemVnum = player.GetItemIndex(ivenType, slotIndex)
				if itemVnum == 52000:
					shopSkinBoard = ikashop.GetShopSkinBoard()
					if shopSkinBoard:
						shopSkinBoard.Open()
					return
				elif itemVnum == 51998:
					if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
						for i in xrange(min(player.GetItemCount(ivenType, slotIndex), 10)):
							self.__UseItem(slotIndex)
						self.OverOutItem()
						return
				
		if app.IsPressed(app.DIK_LSHIFT) and exchange.isTrading() and slotIndex < player.EQUIPMENT_SLOT_START:
			item.SelectItem(player.GetItemIndex(slotIndex))
			emptyExchangeSlots = self.GetExchangeEmptyItemPos(item.GetItemSize()[1])
				
			if emptyExchangeSlots == -1:
				return

			net.SendExchangeItemAddPacket(ivenType, slotIndex, emptyExchangeSlots[0])
			return

		self.__UseItem(slotIndex)
		mouseModule.mouseController.DeattachObject()
		self.OverOutItem()

	def __GetCurrentItemGrid(self):
		itemGrid = [[False for slot in xrange(self.GetPageSizeCategory())] for page in xrange(self.GetPageCountCategory())]

		for page in xrange(self.GetPageCountCategory()):
			for slot in xrange(self.GetPageSizeCategory()):
				itemVnum = player.GetItemIndex(slot + page * self.GetPageSizeCategory())
				if itemVnum <> 0:
					(w, h) = item.GetItemSize(item.SelectItem(itemVnum))
					for i in xrange(h):
						itemGrid[page][slot + i * 5] = True

		return itemGrid

	def __FindEmptyCellForSize(self, itemGrid, size):
		for page in xrange(self.GetPageCountCategory()):
			for slot in xrange(self.GetPageSizeCategory()):
				if itemGrid[page][slot] == False:
					possible = True
					for i in xrange(size):
						p = slot + (i * 5)

						try:
							if itemGrid[page][p] == True:
								possible = False
								break
						except IndexError:
							possible = False
							break

					if possible:
						return slot + page * self.GetPageSizeCategory()

		return -1

	def AttachItemFromSafebox(self, slotIndex, item_type, itemIndex):
		itemGrid = self.__GetCurrentItemGrid()

		if item.GetItemType(item.SelectItem(itemIndex)) == item.ITEM_TYPE_DS:
			return

		emptySlotIndex = self.__FindEmptyCellForSize(itemGrid, item.GetItemSize()[1])
		if emptySlotIndex <> -1:
			net.SendSafeboxCheckoutPacket(slotIndex, item_type, emptySlotIndex)

		return True

	def __UseItem(self, slotIndex):
		ItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],slotIndex)
		item.SelectItem(ItemVNum)

		if app.IsPressed(app.DIK_LALT) and app.IsPressed(app.DIK_LCONTROL):
			if self.wndWiki:
				if not self.wndWiki.IsShow():
					self.wndWiki.Open()
					
				self.wndWiki.SearchItem(ItemVNum)
				return
		elif app.IsPressed(app.DIK_LALT) and app.ENABLE_IKASHOP_RENEWAL:
			if app.USE_OFFLINESHOP_SEARCH_V2:
				if self.interface:
					self.interface.ShopSearchFastItem(ItemVNum, item.GetItemName())
			else:
				board = ikashop.GetSearchShopBoard()
				if board:
					board.FastSearchByItem(ItemVNum)

			return

		if item.IsFlag(item.ITEM_FLAG_CONFIRM_WHEN_USE):
			self.questionDialog = uiCommon.QuestionDialog()

			if app.USE_DROP_AFFECT_BONUSES and item.IsDropAffectBonus(ItemVNum):
				self.questionDialog.SetText(localeInfo.INVENTORY_REALLY_USE_ITEM_NAME % item.GetItemName())
			elif app.USE_PREMIUM_AFFECT and item.IsPremiumUser(ItemVNum):
				self.questionDialog.SetText(localeInfo.INVENTORY_REALLY_USE_ITEM_NAME % item.GetItemName())
			else:
				self.questionDialog.SetText(localeInfo.INVENTORY_REALLY_USE_ITEM)

			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnAccept))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnCancel))
			self.questionDialog.Open()
			self.questionDialog.slotIndex = slotIndex

			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

		if app.ENABLE_SELL_ITEM and app.IsPressed(app.DIK_LCONTROL) and self.IsSellItems(slotIndex):
				self.__SendSellItemPacket(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)
				return
		elif app.ENABLE_SHOW_CHEST_DROP and\
			(app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL))\
			and item.GetItemType() == item.ITEM_TYPE_GIFTBOX:
			if app.IsPressed(app.DIK_Z):
				net.SendChatPacket("/chest_drop %d %d %d"%(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex,player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)))
			elif self.interface:
				self.interface.OpenChestDropWindow(ItemVNum, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)
		else:
			if app.ENABLE_NEW_PET_SYSTEM and item.GetItemType() == item.ITEM_TYPE_PET and item.GetItemSubType() == item.PET_EGG:
				self.interface.OpenChangeNameWindow(slotIndex,slotIndex,ItemVNum, 2)
			else:
				self.__SendUseItemPacket(slotIndex)

	def BindPrivateShopSearchWindow(self, wndPrivateShopSearch):
		self.wndPrivateShopSearch = wndPrivateShopSearch
		
	def BindWikiWindow(self, wndWiki):
		self.wndWiki = wndWiki
		
	def __UseItemQuestionDialog_OnCancel(self):
		self.OnCloseQuestionDialog()

	def __UseItemQuestionDialog_OnAccept(self):
		self.__SendUseItemPacket(self.questionDialog.slotIndex)
		self.OnCloseQuestionDialog()

	def __SendUseItemToItemPacket(self, srcSlotPos, dstSlotPos, srcItemVID = -1, srcInvType = -1):
		#if srcItemVID != -1:
		#	if item.GetUseType(srcItemVID) == "USE_CHANGE_ATTRIBUTE" or item.GetUseType(srcItemVID) == "USE_CHANGE_ATTRIBUTE2":
		#		net.SendItemUseToItemPacket(player.CHANGE_INVENTORY, srcSlotPos, player.INVENTORY, dstSlotPos)
		#		return
		if srcInvType != -1:
			net.SendItemUseToItemPacket(player.SlotTypeToInvenType(srcInvType),srcSlotPos,self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],dstSlotPos)
		else:
			net.SendItemUseToItemPacket(srcSlotPos, dstSlotPos)

	def __SendUseItemPacket(self, slotPos):
		net.SendItemUsePacket(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],slotPos)

	def __SendMoveItemPacket(self, srcSlotWindow, srcSlotPos, dstSlotWindow, dstSlotPos, srcItemCount):
		net.SendItemMovePacket(srcSlotWindow , srcSlotPos, dstSlotWindow, dstSlotPos, srcItemCount)

	if app.ENABLE_SELL_ITEM:
		def IsSellItems(self, slotIndex):
			itemVnum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)
			item.SelectItem(itemVnum)
			itemPrice = item.GetISellItemPrice()
			
			# if item.GetItemType() == item.ITEM_TYPE_WEAPON or item.GetItemType() == item.ITEM_TYPE_ARMOR:
				# return True
				
			if itemPrice > 1:
				return True
				
			return False
			
		def __SendSellItemPacket(self, itemInvenType, itemVNum):
			net.SendItemSellPacket(itemInvenType, itemVNum)

	if app.ENABLE_SASH_SYSTEM:
		def SetSashWindow(self, wndSashCombine, wndSashAbsorption):
			self.wndSashCombine = wndSashCombine
			self.wndSashAbsorption = wndSashAbsorption

		def isShowSashWindow(self):
			if self.wndSashCombine:
				if self.wndSashCombine.IsShow():
					return 1

			if self.wndSashAbsorption:
				if self.wndSashAbsorption.IsShow():
					return 1
			
			return 0
