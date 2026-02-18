# coding: latin_1

import ui
import player
import mouseModule
import net
import app
import snd
import item
import player
import chat
import grp
import uiScriptLocale
import uiRefine
import uiAttachMetin
import uiPickMoney
import uiCommon
import uiPrivateShopBuilder # ¡Æ©©?I¡íoA¢§ ?¡©¥ì??? ItemMove ??Ao
import localeInfo
import constInfo
import ime
import dbg
import wndMgr
import uiToolTip
import game
import exchange
import systemSetting
import uisplititem
import safebox
if app.ENABLE_IKASHOP_RENEWAL:
	import ikashop
from uiUtils import Edit2 as Edit2

if app.BL_67_ATTR:
	import uiAttr67Add

if app.__BL_BATTLE_ROYALE__:
	import background
	import battleRoyaleMgr

ITEM_MALL_BUTTON_ENABLE = True
ENABLE_DEBUG_SLOTS = False

import exception


ITEM_FLAG_APPLICABLE = 1 << 14

if app.ENABLE_SASH_SYSTEM:
	import sash

ENABLE_SEARCH_FIELD = True # Enable system search item field

AUTO_NAME_TOOL_TIP_DICT = {
	1 : uiScriptLocale.TEXT_TALISMAN_1,
	2 : uiScriptLocale.TEXT_TALISMAN_2,
	3 : uiScriptLocale.TEXT_TALISMAN_3,
	4 : "[ENTER]",
	5 : uiScriptLocale.TEXT_TALISMAN_4,
	6 : uiScriptLocale.TEXT_TALISMAN_5,
	7 : uiScriptLocale.TEXT_TALISMAN_6,
	8 : uiScriptLocale.TEXT_TALISMAN_7,
}

class BeltInventoryWindow(ui.ScriptWindow):
	def __init__(self, wndInventory):
		if not app.ENABLE_NEW_EQUIPMENT_SYSTEM:
			exception.Abort("What do you do?")
			return
		if not wndInventory:
			exception.Abort("wndInventory parameter must be set to InventoryWindow")
			return
		ui.ScriptWindow.__init__(self)

		self.isLoaded = 0
		self.wndInventory = wndInventory;

		self.wndBeltInventoryLayer = None
		self.wndBeltInventorySlot = None
		self.expandBtn = None
		self.minBtn = None

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self, openBeltSlot = False):
		self.__LoadWindow()
		self.RefreshSlot()
		ui.ScriptWindow.Show(self)
		if openBeltSlot:
			self.OpenInventory()
		else:
			self.CloseInventory()

	def Close(self):
		self.Hide()

	def IsOpeningInventory(self):
		return self.wndBeltInventoryLayer.IsShow()

	def OpenInventory(self):
		self.wndBeltInventoryLayer.Show()
		self.expandBtn.Hide()

		if localeInfo.IsARABIC() == 0:
			self.AdjustPositionAndSize()

	def CloseInventory(self):
		self.wndBeltInventoryLayer.Hide()
		self.expandBtn.Show()

		if localeInfo.IsARABIC() == 0:
			self.AdjustPositionAndSize()

	def GetBasePosition(self):
		x, y = self.wndInventory.GetGlobalPosition()
		return x - 148, y + 241	

	def AdjustPositionAndSize(self):
		bx, by = self.GetBasePosition()

		if self.IsOpeningInventory():
			self.SetPosition(bx, by)
			self.SetSize(self.ORIGINAL_WIDTH, self.GetHeight())

		else:
			self.SetPosition(bx + 138, by);
			self.SetSize(10, self.GetHeight())

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/BeltInventoryWindow.py")
		except:
			import exception
			exception.Abort("CostumeWindow.LoadWindow.LoadObject")

		try:
			self.ORIGINAL_WIDTH = self.GetWidth()
			wndBeltInventorySlot = self.GetChild("BeltInventorySlot")
			self.wndBeltInventoryLayer = self.GetChild("BeltInventoryLayer")
			self.expandBtn = self.GetChild("ExpandBtn")
			self.minBtn = self.GetChild("MinimizeBtn")

			self.expandBtn.SetEvent(ui.__mem_func__(self.OpenInventory))
			self.minBtn.SetEvent(ui.__mem_func__(self.CloseInventory))

			if localeInfo.IsARABIC() :
				self.expandBtn.SetPosition(self.expandBtn.GetWidth() - 2, 15)
				self.wndBeltInventoryLayer.SetPosition(self.wndBeltInventoryLayer.GetWidth() - 5, 0)
				self.minBtn.SetPosition(self.minBtn.GetWidth() + 3, 15)

			for i in xrange(item.BELT_INVENTORY_SLOT_COUNT):
				slotNumber = item.BELT_INVENTORY_SLOT_START + i
				wndBeltInventorySlot.SetCoverButton(slotNumber,	"d:/ymir work/ui/game/quest/slot_button_01.sub",\
												"d:/ymir work/ui/game/quest/slot_button_01.sub",\
												"d:/ymir work/ui/game/quest/slot_button_01.sub",\
												"d:/ymir work/ui/game/belt_inventory/slot_disabled.tga", False, False)

		except:
			import exception
			exception.Abort("CostumeWindow.LoadWindow.BindObject")

		## Equipment
		wndBeltInventorySlot.SetOverInItemEvent(ui.__mem_func__(self.wndInventory.OverInItem))
		wndBeltInventorySlot.SetOverOutItemEvent(ui.__mem_func__(self.wndInventory.OverOutItem))
		wndBeltInventorySlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.wndInventory.UseItemSlotNew))
		wndBeltInventorySlot.SetUseSlotEvent(ui.__mem_func__(self.wndInventory.UseItemSlotNew))
		wndBeltInventorySlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.wndInventory.SelectEmptySlot))
		wndBeltInventorySlot.SetSelectItemSlotEvent(ui.__mem_func__(self.wndInventory.SelectItemSlot))

		self.wndBeltInventorySlot = wndBeltInventorySlot

	def ReturnSlots(self):
		getItemVNum=player.GetItemIndex
		for i in xrange(player.NEW_EQUIPMENT_SLOT_COUNT):
			slotNumber = player.NEW_EQUIPMENT_SLOT_START + i
			vnum = getItemVNum(slotNumber)

			if vnum != 0:
				item.SelectItem(vnum)
				if item.GetItemType() == item.ITEM_TYPE_BELT and item.GetItemType() != item.ITEM_TYPE_COSTUME:
					return item.GetValue(0), vnum

		return 0, 0

	def ReturnCountSlot(self):
		value0, vnum = self.ReturnSlots()
		list = {
			7: 16,
			6: 12,
			5: 9,
			4: 6,
			3: 4,
			2: 2,
			1: 1,
			0:0,
		}
		if vnum != 0:
			if list.has_key(value0):
				# dbg.TraceError("VNUM: "+str(vnum)+" Value0: "+str(value0)+ " Slots desbloquear: "+str(list[value0]))
				return list[value0]
		return 0

	def RefreshSlot(self):
		getItemVNum=player.GetItemIndex
		# slots = self.ReturnCountSlot()

		table = [
			1,2,4,6,
			3,3,4,6,
			5,5,5,6,
			7,7,7,7,
		]
		table2 = []
		
		value0, vnum = self.ReturnSlots()
		for i in xrange(len(table)):
			if value0 >= table[i]:
				table2.append(1)
			else:
				table2.append(0)

		for i in xrange(item.BELT_INVENTORY_SLOT_COUNT):
			slotNumber = item.BELT_INVENTORY_SLOT_START + i
			self.wndBeltInventorySlot.SetItemSlot(slotNumber, getItemVNum(slotNumber), player.GetItemCount(slotNumber))
			self.wndBeltInventorySlot.SetAlwaysRenderCoverButton(slotNumber, True)

			if table2[i] == 1:
				self.wndBeltInventorySlot.EnableCoverButton(slotNumber)
			else:
				self.wndBeltInventorySlot.DisableCoverButton(slotNumber)

		self.wndBeltInventorySlot.RefreshSlot()

if app.ENABLE_IKASHOP_RENEWAL and app.EXTEND_IKASHOP_ULTIMATE and app.USE_IKASHOP_UI_RENEWED:
	class IkarusShopCounter(ui.ExpandedImageBox):
		def Load(self):
			self.LoadImage("ikashop/lite/business/counter.png")

			self.counter = ui.TextLine()
			self.counter.SetParent(self)
			self.counter.SetOutline(1)
			self.counter.SetPackedFontColor(0xFFFFFFFF)
			self.counter.SetPosition(self.GetWidth() / 2, self.GetHeight() / 2 - 1)
			self.counter.SetHorizontalAlignCenter()
			self.counter.SetVerticalAlignCenter()
			self.counter.Show()

		def Destroy(self):
			del self.counter
			self.counter = None

			self.UnloadImage()

		def SetCounter(self, count):
			if count == 0:
				self.Hide()
			else:
				self.counter.SetText(str(count) if count <= 9 else "9+")
				self.Show()

class InventoryWindow(ui.ScriptWindow):
	SLOTNAMES = {
					187 : "test",
					188 : "test",
					199 : "test",
					200 : "test",
					207 : "test",
					202 : "test",
					204 : "test",
					206 : "test",
					201 : "test",
					208 : "test",
					209 : "test",
	}

	liHighlightedItems = []

	USE_TYPE_TUPLE = ("USE_SPECIAL", "USE_CLEAN_SOCKET", "USE_CHANGE_ATTRIBUTE", "USE_CHANGE_ATTRIBUTE2", "USE_ADD_ATTRIBUTE", "USE_ADD_ATTRIBUTE2", "USE_ADD_ACCESSORY_SOCKET", "USE_PUT_INTO_ACCESSORY_SOCKET", "USE_PUT_INTO_BELT_SOCKET", "USE_PUT_INTO_RING_SOCKET", "USE_CHANGE_COSTUME_ATTR", "USE_RESET_COSTUME_ATTR", "USE_PUT_INTO_AURA_SOCKET")
	USE_TYPE_TUPLE = tuple(list(USE_TYPE_TUPLE) + ["USE_ADD_BOOSTER_ATTR", "USE_CHANGE_BOOSTER_ATTR", "USE_ADD_BOOSTER_TIME"])

	if app.USE_CRYSTAL_SYSTEM:
		USE_TYPE_TUPLE += ("USE_ADD_ATTR_CRYSTAL", "USE_CHANGE_ATTR_CRYSTAL", "USE_RECHARGE_CRYSTAL")

	NORMAL_TYPE = 0
	UPGRADE_TYPE = 1
	BOOK_TYPE = 2
	STONE_TYPE = 3
	CHANGE_TYPE = 4
	COSTUME_TYPE = 5
	

	SLOT_WINDOW_TYPE = {
		NORMAL_TYPE		:	{"name": uiScriptLocale.STORAGE_0, "window" : player.INVENTORY,		"slot" : player.SLOT_TYPE_INVENTORY},
		UPGRADE_TYPE	:	{"name": uiScriptLocale.STORAGE_1, "window" : player.UPGRADE_INVENTORY, "slot" : player.SLOT_TYPE_UPGRADE_INVENTORY},
		BOOK_TYPE		:	{"name": uiScriptLocale.STORAGE_2, "window" : player.BOOK_INVENTORY,	"slot" : player.SLOT_TYPE_BOOK_INVENTORY},
		STONE_TYPE		:	{"name": uiScriptLocale.STORAGE_3, "window" : player.STONE_INVENTORY,	"slot" : player.SLOT_TYPE_STONE_INVENTORY},
		CHANGE_TYPE		:	{"name": uiScriptLocale.STORAGE_4, "window" : player.CHANGE_INVENTORY,	"slot" : player.SLOT_TYPE_CHANGE_INVENTORY},
		COSTUME_TYPE		:	{"name": uiScriptLocale.STORAGE_5, "window" : player.COSTUME_INVENTORY,	"slot" : player.SLOT_TYPE_COSTUME_INVENTORY},
	}

	wndBraveCape = None
	questionDialog = None
	tooltipItem = None
	toolTipInfoNew = None
	dlgPickMoney = None
	sellingSlotNumber = -1

	isLoaded = 0

	isOpenedCostumeWindowWhenClosingInventory = 0
	isOpenedBeltWindowWhenClosingInventory = 0

	interface = None
	if app.WJ_ENABLE_TRADABLE_ICON:
		bindWnds = []

	wndPrivateShopSearch	= None
	wndWiki					= None

	def BindExpandedMoneyTaskBar(self, MoneyTaskBar):
		from _weakref import proxy
		self.wndExpandedMoneyTaskBar = proxy(MoneyTaskBar)

	def __init__(self):
		self.toolTipInfoNew = None
		self.wndExpandedMoneyTaskBar = None

		ui.ScriptWindow.__init__(self)

		self.isOpenedBeltWindowWhenClosingInventory = 0		# ?I???a¢¬¢ç ¢¥Y?¡í ¢Ò¡× ?¡×?¢ç ?I???a¢¬¢ç¡Æ¢§ ?¡©¡¤A?O?u¢¥AAo ?¨Ï?I-_-; ©©¡¿???O ¢´¢¬¢´¥ì
		if app.ENABLE_HIDE_COSTUME_SYSTEM:
			self.elemets_world = {}
		self.wndItem = None
		self.categoryPageIndex = 0
		self.inventoryPageIndex = 0
		self.ChangeEquipIndex = 0
		self.grid = ui.Grid(width = 5, height = (9*4))
		if app.ENABLE_SASH_SYSTEM:
			self.listAttachedSashs = []
		if app.ENABLE_HIDE_COSTUME_SYSTEM:
			self.elemets_hide = []
		self.wndPrivateShopSearch = None
		self.wndWiki = None
		self.buttonTab = []

		if ENABLE_SEARCH_FIELD:
			self.wndItemSRC = None
			self.wndNameSlot = None
			self.btnResetSearch = None
			self.wndName = None
			self.manageOpen = False

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

		self.manageOpen = False

	def Show(self):
		if self.wndExpandedMoneyTaskBar and not self.wndExpandedMoneyTaskBar.IsShow():
			self.wndExpandedMoneyTaskBar.Show()

		ui.ScriptWindow.Show(self)
		# if self.isOpenedCostumeWindowWhenClosingInventory and self.wndCostume:
			# self.wndCostume.Show()

		#if self.wndBelt:
			#self.wndBelt.Show(self.isOpenedBeltWindowWhenClosingInventory)

	def BindInterfaceClass(self, interface):
		self.interface = interface

	if app.WJ_ENABLE_TRADABLE_ICON:
		def BindWindow(self, wnd):
			self.bindWnds.append(wnd)

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			state = systemSetting.GetInventoryChange()
			if app.ENABLE_EXTEND_INVEN_SYSTEM:
				if state == 0:
					pyScrLoader.LoadScriptFile(self, "UIScript/InventoryWindowEx0.py")
				elif state == 1:
					pyScrLoader.LoadScriptFile(self, "UIScript/InventoryWindowEx1.py")
				elif state == 2:
					pyScrLoader.LoadScriptFile(self, "UIScript/InventoryWindowEx2.py")
			else:
				if ITEM_MALL_BUTTON_ENABLE:
					pyScrLoader.LoadScriptFile(self, "UIScript/InventoryWindow.py")
				else:
					pyScrLoader.LoadScriptFile(self, "UIScript/InventoryWindow.py")
		except:
			import exception
			exception.Abort("InventoryWindow.LoadWindow.LoadObject")

		try:
			wndItem = self.GetChild("ItemSlot")

			mask1 = self.GetChild("Equipment_Base_Tab_0")
			mask2 = self.GetChild("Equipment_Base_Tab_1")
			mask3 = self.GetChild("Equipment_Base_Tab_2")
			mask4 = self.GetChild("Equipment_Base_Tab_3")
			mask5 = self.GetChild("Equipment_Base_Tab_4")

			self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
			self.wndBoard = self.GetChild("board")

			self.toolTip = uiToolTip.ToolTip()
			self.toolTip.ClearToolTip()

			self.GetChild("Money").Hide()
			self.GetChild("Money_Slot").Hide()
			self.GetChild("Money_Icon").Hide()

			# self.separateButton = None

			self.inventoryTab = []
			for i in xrange(player.INVENTORY_PAGE_COUNT):
				self.inventoryTab.append(self.GetChild("Inventory_Tab_%02d" % (i+1)))

			self.equipmentTab = []
			slotIdxList = []

			for j in xrange(5):
				self.GetChild("Equipment_Tab_"+str(j)).SAFE_SetEvent(self.SetEquipmentPage, j)
				wndEquip = self.GetChild("EquipmentSlot_"+str(j))
				slotIdxList.append(wndEquip.GetSlotIndexList())
				wndEquip.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
				wndEquip.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
				wndEquip.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
				wndEquip.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
				wndEquip.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
				wndEquip.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

			self.slotIdxList = slotIdxList

			for j in xrange(5):
				equipment_tab = self.GetChild("Equipment_Tab_" + str(j))
				self.equipmentTab.append(equipment_tab)

			self.objectToMove = []
			self.objectToMove.append(wndItem)
			self.objectToMove.append(mask1)
			self.objectToMove.append(mask2)
			self.objectToMove.append(mask3)
			self.objectToMove.append(mask4)
			self.objectToMove.append(mask5)
			
			self.objectToMove = self.objectToMove + self.inventoryTab + self.equipmentTab

			self.manageInventoryBtn = self.GetChild("ManageInventory")
			
			self.manageOpen = False

			self.offlineShopButton = self.GetChild2("OfflineShopButton")

			if app.ENABLE_IKASHOP_RENEWAL and app.EXTEND_IKASHOP_ULTIMATE and app.USE_IKASHOP_UI_RENEWED:
				self.notificationCounter = IkarusShopCounter()
				self.notificationCounter.Load()
				self.notificationCounter.SetParent(self.offlineShopButton)
				self.notificationCounter.SetPosition(12, 9)
				self.notificationCounter.SetCounter(0)

			# self.NewInvButton = self.GetChild2("NewInvButton")
			# self.SwitchButton = self.GetChild2("SwitchButton")
			self.DSSButton = self.GetChild2("DSSButton")#alchimie
			self.MallButton = self.GetChild2("MallButton")

			if app.__RENEWAL_BRAVE_CAPE__:
				disbandBtn = ui.Button()
				disbandBtn.SetParent(self)
				disbandBtn.SetPosition(22, 220)
				disbandBtn.SetUpVisual("d:/ymir work/ui/game/belt_inventory/btn_expand_normal.tga")
				disbandBtn.SetOverVisual("d:/ymir work/ui/game/belt_inventory/btn_expand_over.tga")
				disbandBtn.SetDownVisual("d:/ymir work/ui/game/belt_inventory/btn_expand_down.tga")
				disbandBtn.SAFE_SetEvent(self.ClickBraveCape)
				disbandBtn.Show()
				self.disbandBtn = disbandBtn

				self.wndBraveCape = BraveCapeWindow()
				self.OnMoveWindow(*self.GetGlobalPosition())

		except:
			import exception
			exception.Abort("InventoryWindow.LoadWindow.BindObject")

		## Item
		wndItem.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		wndItem.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		wndItem.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		wndItem.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		## Equipment
		wndEquip.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		wndEquip.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		wndEquip.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndEquip.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndEquip.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		wndEquip.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		if app.ENABLE_HIDE_COSTUME_SYSTEM:
			self.elemets_world = {}
			self.elemets_world["hide_button_3"] = ui.Button()
			self.elemets_world["hide_button_3"].SetParent(self.GetChild("Equipment_Base_Tab_0"))
			self.elemets_world["hide_button_3"].SetPosition(100, 5)
			self.elemets_world["hide_button_3"].SetUpVisual("Modulo/HideCostume/button_show_1.tga")
			self.elemets_world["hide_button_3"].SetOverVisual("Modulo/HideCostume/button_show_0.tga")
			self.elemets_world["hide_button_3"].SetDownVisual("Modulo/HideCostume/button_show_1.tga")
			self.elemets_world["hide_button_3"].SetEvent(self.FuncHide, 3)
			self.elemets_world["hide_button_3"].Show()

			self.elemets_world["hide_button_5"] = ui.Button()
			self.elemets_world["hide_button_5"].SetParent(self.GetChild("EquipmentSlot_0"))
			self.elemets_world["hide_button_5"].SetPosition(137, 2)
			self.elemets_world["hide_button_5"].SetUpVisual("Modulo/HideCostume/button_show_1.tga")
			self.elemets_world["hide_button_5"].SetOverVisual("Modulo/HideCostume/button_show_0.tga")
			self.elemets_world["hide_button_5"].SetDownVisual("Modulo/HideCostume/button_show_1.tga")
			self.elemets_world["hide_button_5"].SetEvent(self.FuncHide,4)
			self.elemets_world["hide_button_5"].Hide()

		## PickMoneyDialog
		dlgPickMoney = uisplititem.PickItemDialog()
		dlgPickMoney.LoadDialog()
		dlgPickMoney.Hide()

		## RefineDialog
		self.refineDialog = uiRefine.RefineDialog()
		self.refineDialog.Hide()

		## AttachMetinDialog
		if app.WJ_ENABLE_TRADABLE_ICON:  
			self.attachMetinDialog = uiAttachMetin.AttachMetinDialog(self)
			self.BindWindow(self.attachMetinDialog)
		else:
			self.attachMetinDialog = uiAttachMetin.AttachMetinDialog()
		self.attachMetinDialog.Hide()

		for i in xrange(player.INVENTORY_PAGE_COUNT):
			self.inventoryTab[i].SetEvent(lambda arg=i: self.SetInventoryPage(arg))
		self.inventoryTab[0].Down()

		self.wndItem = wndItem
		self.wndEquip = wndEquip

		self.dlgPickMoney = dlgPickMoney

		
		self.GetChild("Pendant_Wind_Glow").Hide()
		self.GetChild("Pendant_Thunder_Glow").Hide()
		self.GetChild("Pendant_Dark_Glow").Hide()
		self.GetChild("Pendant_Earth_Glow").Hide()
		self.GetChild("Pendant_Fire_Glow").Hide()
		self.GetChild("Pendant_Ice_Glow").Hide()

		self.GetChild("PendantSetBonusIcon").OnMouseOverIn = ui.__mem_func__(self.OnOverTalismanBonus)
		self.GetChild("PendantSetBonusIcon").OnMouseOverOut = ui.__mem_func__(self.OnOverOutTalisman)
		self.GetChild("PendantSetBonusIcon").Hide()
		self.GetChild("PendantSetBonusWindow").Hide()

		self.mask1 = mask1
		self.mask2 = mask2
		self.mask3 = mask3
		self.mask4 = mask4
		self.mask5 = mask5

		if self.DSSButton:
			self.DSSButton.SetEvent(ui.__mem_func__(self.ClickDSSButton))

		if self.MallButton:
			self.MallButton.SetEvent(ui.__mem_func__(self.ClickOpenNewStorage))

		if self.offlineShopButton:
			self.offlineShopButton.SetEvent(ui.__mem_func__(self.ClickOfflineShopButton))

		self.toolTipInfoNew = uiToolTip.ToolTip()
		self.toolTipInfoNew.ClearToolTip()

		## inventory locked
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
			self.wndItemSRC.SetParent(self.GetChild("board"))
			self.wndItemSRC.SetPosition(8, 246)
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
			self.wndButtonBase.SetParent(self.GetChild("board"))
			self.wndButtonBase.LoadImage("d:/ymir work/ui/game/interfaces/special_storage/btn-base_storage.png")
			self.wndButtonBase.SetPosition(5, 30.5)
			self.wndButtonBase.Hide()

			self.wndNameSlot = ui.ImageBox()
			self.wndNameSlot.SetParent(self.wndButtonBase)
			self.wndNameSlot.LoadImage("d:/ymir work/ui/game/interfaces/special_storage/input.png")
			self.wndNameSlot.SetPosition(52, 4)
			self.wndNameSlot.Show()

			# self.btnResetSearch = ui.Button()
			# self.btnResetSearch.SetParent(self.wndButtonBase)
			# self.btnResetSearch.SetUpVisual("d:/ymir work/ui/game/search_inventory/x_norm.png")
			# self.btnResetSearch.SetOverVisual("d:/ymir work/ui/game/search_inventory/x_over.png")
			# self.btnResetSearch.SetDownVisual("d:/ymir work/ui/game/search_inventory/x_down.png")
			# self.btnResetSearch.SetPosition(134, 7)
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

		if app.__BL_BATTLE_ROYALE__ and background.IsBattleRoyaleMap():
			self.__CreateBattleRoyaleExtendInvenButton()

		self.ManageInventory()

		self.pendantPageIdx = 0
		self.pendantPageOpened = 0
		self.equipmentTypeIndex = 0
		self.pendantQuestionDialog = None
		self.GetChild("pendant_page_0").SAFE_SetEvent(self.__ClickPendantPage, 0)
		self.GetChild("pendant_page_1").SAFE_SetEvent(self.__ClickPendantPage, 1)
		self.SetEquipmentPage(0)
		self.SetInventoryPage(0)
		self.SetEquipmentPage(0)
		self.RefreshItemSlot()

	def ManageInventory(self):
		if not self.manageOpen:
			self.manageOpen = True
			self.ClearGrid()
			self.wndButtonBase.Show()
			self.SetSize(self.GetWidth(), self.GetHeight()+25)
			self.wndBoard.SetSize(self.wndBoard.GetWidth(), self.wndBoard.GetHeight()+25)
			# self.SetPosition(self.GetPosition()[0], self.GetPosition()[1]-25)
			for i in range(len(self.objectToMove)):
				self.objectToMove[i].SetPosition(self.objectToMove[i].GetLocalPosition()[0], self.objectToMove[i].GetLocalPosition()[1]+25)
		else:
			self.manageOpen = False
			self.wndButtonBase.Hide()
			self.wndName.SetText("")
			self.wndItemSRC.Hide()
			self.wndItem.Show()
			self.SetSize(self.GetWidth(), self.GetHeight()-25)
			self.wndBoard.SetSize(self.wndBoard.GetWidth(), self.wndBoard.GetHeight()-25)
			# self.SetPosition(self.GetPosition()[0], self.GetPosition()[1]+25)
			for i in range(len(self.objectToMove)):
				self.objectToMove[i].SetPosition(self.objectToMove[i].GetLocalPosition()[0], self.objectToMove[i].GetLocalPosition()[1]-25)

	def OnOverTalismanBonus(self):
		self.toolTipInfoNew.SetTitle("Talisman")

		for desc in AUTO_NAME_TOOL_TIP_DICT.itervalues():
			if desc == "[ENTER]":
				self.toolTipInfoNew.AppendHorizontalLine()
			else:
				self.toolTipInfoNew.AutoAppendTextLine(desc)
				self.toolTipInfoNew.AppendSpace(1)

		self.toolTipInfoNew.TextAlignHorizonalCenter()
		self.toolTipInfoNew.SetTop()
		self.toolTipInfoNew.Show()
		
	def OnOverOutTalisman(self):
		self.toolTipInfoNew.ClearToolTip()
		self.toolTipInfoNew.Hide()

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

			window_type = player.INVENTORY
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


	if app.ENABLE_HIDE_COSTUME_SYSTEM:
		def FuncHide(self,index):
			net.SendChatPacket("/costume_hide %d" % index)

		def costume_hide_load2(self):
			self.elemets_hide = self.get_costume_hide_list()
			
			for i in xrange(self.GetSlotCount()):
				# if not i == 3:
					# continue
				
				if len(self.elemets_hide) > 0:
					self.elemets_world["hide_button_%d"%self.elemets_hide[i][0]].SetUpVisual("Modulo/HideCostume/button_%s_0.tga"%self.ButtonInfoHide(self.elemets_hide[i][1]))
					self.elemets_world["hide_button_%d"%self.elemets_hide[i][0]].SetOverVisual("Modulo/HideCostume/button_%s_1.tga"%self.ButtonInfoHide(self.elemets_hide[i][1]))
					self.elemets_world["hide_button_%d"%self.elemets_hide[i][0]].SetDownVisual("Modulo/HideCostume/button_%s_0.tga"%self.ButtonInfoHide(self.elemets_hide[i][1]))
				self.elemets_world["hide_button_%d"%i].Show()

		def ButtonInfoHide(self,index):
			if index == 0:
				return "show"
			return "hide"

		def GetSlotCount(self):
			return len(self.elemets_world["position"]) if self.elemets_world.has_key("position") else 0

			slot_total = 2
			if app.ENABLE_HIDE_COSTUME_SYSTEM_ACCE:
				slot_total += 1
			if app.ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME:
				slot_total += 1
			slot_total += 4
			return slot_total

	def ClickOpenNewStorage(self):
		interface = constInfo.GetInterfaceInstance()
		if interface == None:
			return
		interface.ToggleInventoryNewWindow()

	def ClickOfflineShopButton(self):
		board = ikashop.GetBusinessBoard()
		if board:
			board.Toggle()

	def ClickSwitchbotButton(self):
		net.SBOpen()

	def SelectEmptySlotNew(self, selectedSlotPos):
		#if uiPrivateShopBuilder.IsBuildingPrivateShop():
		#	chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
		#	return
		if app.ENABLE_AURA_SYSTEM:
			if player.IsAuraRefineWindowOpen():
				return

		isAttached = mouseModule.mouseController.isAttached()
		index_change_equip = self.GetChangeEquipIndex()

		selectedSlotPos = player.CHANGE_EQUIP_SLOT_COUNT/player.CHANGE_EQUIP_PAGE_EXTRA*(index_change_equip-1) + (selectedSlotPos-player.CHANGE_EQUIP_SLOT_START)

		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()
			itemIndex = player.GetItemIndex(attachedSlotPos)
			itemCount = player.GetItemCount(attachedSlotPos)

			item.SelectItem(itemIndex)
			itemType = item.GetItemType()

			#if item.ITEM_TYPE_WEAPON == itemType or item.ITEM_TYPE_ARMOR == itemType :
			if player.SLOT_TYPE_INVENTORY == attachedSlotType:
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				net.SendItemMovePacket(player.INVENTORY, attachedSlotPos, player.CHANGE_EQUIP, selectedSlotPos, attachedCount)

	def SelectItemSlotNew(self, selectedSlotPos):

		#if uiPrivateShopBuilder.IsBuildingPrivateShop():
		#	chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
		#	return

		isAttached = mouseModule.mouseController.isAttached()
		index_change_equip = self.GetChangeEquipIndex()
		selectedSlotPos = player.CHANGE_EQUIP_SLOT_COUNT/player.CHANGE_EQUIP_PAGE_EXTRA*(index_change_equip-1) + (selectedSlotPos-player.CHANGE_EQUIP_SLOT_START)
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()
			itemIndex = player.GetItemIndex(attachedSlotPos)
			itemCount = player.GetItemCount(attachedSlotPos)

			item.SelectItem(itemIndex)
			itemType = item.GetItemType()

			#if item.ITEM_TYPE_WEAPON == itemType or item.ITEM_TYPE_ARMOR == itemType :
			if player.SLOT_TYPE_INVENTORY == attachedSlotType:
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				net.SendItemMovePacket(player.INVENTORY, attachedSlotPos, player.CHANGE_EQUIP, selectedSlotPos, attachedCount)

		else:

			itemVnum = player.GetItemIndex(player.CHANGE_EQUIP, selectedSlotPos)
			itemCount = player.GetItemCount(player.CHANGE_EQUIP, selectedSlotPos)
			mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_CHANGE_EQUIP, selectedSlotPos, itemVnum, itemCount)

	def OverInItemNew(self,index):
		index_change_equip = self.GetChangeEquipIndex()
		index = player.CHANGE_EQUIP_SLOT_COUNT/player.CHANGE_EQUIP_PAGE_EXTRA*(index_change_equip-1) + (index-player.CHANGE_EQUIP_SLOT_START)

		if None != self.tooltipItem:
			self.tooltipItem.ClearToolTip()
			self.tooltipItem.SetInventoryItem(index,player.CHANGE_EQUIP)

	def OverOutItemNew(self):
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def GetCategoryIndex(self):
		return self.categoryPageIndex

	def SetCategoryPage(self,index):
		self.categoryPageIndex = index
		for x in xrange(18):
			self.lock[x].SetUpVisual("d:/ymir work/drakon2/inventory/0.tga")
			self.lock[x].SetOverVisual("d:/ymir work/drakon2/inventory/0.tga")
			self.lock[x].SetDownVisual("d:/ymir work/drakon2/inventory/0.tga")
			self.lock[x].Show()

		self.RefreshItemSlot()

	def GetPageSizeCategory(self):
		if self.GetIsCategorySpecial():
			return player.SPECIAL_PAGE_SIZE
		return player.INVENTORY_PAGE_SIZE

	def GetPageCountCategory(self):
		if self.GetIsCategorySpecial():
			return 4
		return player.INVENTORY_PAGE_COUNT

	def GetIsCategorySpecial(self):
		#if self.GetUseItemSlotNew():
			#return False

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
	def ClickStack(self):
		category = self.GetCategoryIndex()
		if category == 0:
			net.SendChatPacket("/real_sort_items 0")
		else:
			net.SendChatPacket("/real_sort_items %d"%(category))
		self.question.Close()


	def ClickSort(self):
		#
		category = self.GetCategoryIndex()
		if category == 0:
			net.SendChatPacket("/click_sort_items")
		else:
			net.SendChatPacket("/click_sort_special_storage %d"%(category))
		self.question.Close()

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
		if self.wndExpandedMoneyTaskBar:
			self.wndExpandedMoneyTaskBar = None

		self.ClearDictionary()
		
		self.grid.reset()
		self.grid=None

		self.dlgPickMoney.Destroy()
		self.dlgPickMoney = 0
		if ENABLE_SEARCH_FIELD:
			self.wndItemSRC = None
			self.wndNameSlot = None
			self.btnResetSearch = None
			self.wndName = None

		self.refineDialog.Destroy()
		self.refineDialog = 0
		if app.ENABLE_HIDE_COSTUME_SYSTEM:
			self.elemets_world = {}
			self.elemets_hide = {}
		self.attachMetinDialog.Destroy()
		self.attachMetinDialog = 0
		self.tooltipItem = None
		self.wndItem = 0
		self.wndEquip = 0
		self.dlgPickMoney = 0
		# self.wndCostume = 0
		# self.wndRings = 0
		self.questionDialog = None
		self.dlgQuestion = None
		self.ChangeEquipButton = None
		self.DSSButton = None

		if app.ENABLE_IKASHOP_RENEWAL and app.EXTEND_IKASHOP_ULTIMATE and app.USE_IKASHOP_UI_RENEWED:
			self.notificationCounter.Destroy()
			del self.notificationCounter

			self.notificationCounter = None

		self.offlineShopButton.SetEvent(None)
		self.offlineShopButton = None

		self.MallButton = None
		self.interface = None
		if app.__BL_BATTLE_ROYALE__:
			self.BattleRoyaleExInvenButton = []
		self.wndPrivateShopSearch	= None
		self.wndWiki				= None
		# if app.ENABLE_DSS_ACTIVE_EFFECT_BUTTON:
		self.DSSButtonEffect = None

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.bindWnds = []

		# if self.wndCostume:
			# self.wndCostume.Destroy()
			# self.wndCostume = 0

		if app.__RENEWAL_BRAVE_CAPE__:
			if self.wndBraveCape:
				self.wndBraveCape.Destroy()
				self.wndBraveCape = None

		#if self.wndBelt:
			#self.wndBelt.Destroy()
			#self.wndBelt = None

		self.inventoryTab = []
		self.equipmentTab = []

		self.Hide()

	def Hide(self, force_hide = False):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS():
			self.OnCloseQuestionDialog()
			return
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

		# if self.wndCostume:
			# self.isOpenedCostumeWindowWhenClosingInventory = self.wndCostume.IsShow()			# ?I???a¢¬¢ç ?¢¨?? ¢¥Y?u ¢Ò¡× AU¢©????? ?¡©¡¤A ?O?u¢¥A¡Æ¢§?
			# self.wndCostume.Close()

		if self.dlgPickMoney:
			self.dlgPickMoney.Close()

		if app.__RENEWAL_BRAVE_CAPE__:
			if self.wndBraveCape:
				self.wndBraveCape.Close()

		wndMgr.Hide(self.hWnd, force_hide)

	def Close(self):
		if ENABLE_SEARCH_FIELD:
			if self.wndName and self.wndName.IsFocus():
				self.wndName.KillFocus()

		self.Hide()

	def SetInventoryPage(self, page):
		self.inventoryPageIndex = page
		for i in xrange(player.INVENTORY_PAGE_COUNT):
			if i!=page:
				self.inventoryTab[i].SetUp()
		self.RefreshBagSlotWindow()

	def __CheckAllPendant(self):
		for slotIdx in range(231, 237):
			itemIdx = player.GetItemIndex(slotIdx)
			if not itemIdx:
				return False
			item.SelectItem(itemIdx)
			if item.GetItemName().find("+25") == -1:
				return False
		return True
	def __AcceptPendantPage(self, pageIdx):
		net.SendChatPacket("/pendant set %d" % pageIdx)
		self.pendantQuestionDialog.Hide()
		self.pendantQuestionDialog.Destroy()
		self.pendantQuestionDialog = None
	def __ClickPendantPage(self, pageIdx):
		self.SetEquipmentPage(self.equipmentTypeIndex)
		if not self.pendantPageOpened:
			if not self.pendantQuestionDialog:
				self.pendantQuestionDialog = uiCommon.ItemQuestDialog()
			self.pendantQuestionDialog.SetTitleName("Pendant")
			self.pendantQuestionDialog.Open([[50255, 2000]])
			self.pendantQuestionDialog.SelectEvent(self.__AcceptPendantPage, pageIdx)
			return
		net.SendChatPacket("/pendant set %d" % pageIdx)

	def SetPendantPageIdx(self, pageIdx, secondOpened):
		self.pendantPageIdx = pageIdx
		self.pendantPageOpened = secondOpened
		self.SetEquipmentPage(self.equipmentTypeIndex)

	def SetEquipmentPage(self, pageIdx):
		self.equipmentTypeIndex = pageIdx
		net.SendChatPacket("/buffi_costume_page {}".format(0))
		for j in xrange(5):
			if j == pageIdx:
				self.GetChild("Equipment_Tab_"+str(j)).Down()
				self.GetChild("Equipment_Base_Tab_"+str(j)).Show()
			else:
				self.GetChild("Equipment_Tab_"+str(j)).SetUp()
				self.GetChild("Equipment_Base_Tab_"+str(j)).Hide()


		if pageIdx == 1:
			wndEquip = self.GetChild("EquipmentSlot_"+str(pageIdx))
			wndEquip.ClearAllSlot() # index / x / y / width / height
			wndEquip.AppendSlot(item.COSTUME_SLOT_START+0, 58, 73, 32, 64)
			wndEquip.AppendSlot(item.COSTUME_SLOT_START+1, 58, 32, 32, 32)
			wndEquip.AppendSlot(204, 9, 14, 32, 96) 
			wndEquip.AppendSlot(207, 107, 55, 32, 32)
			wndEquip.AppendSlot(211, 107, 96, 32, 32)
			# wndEquip.AppendSlot(item.SHINING_SLOT_START+1, 107, 134, 32, 32)

			self.elemets_hide = self.get_costume_hide_list()
			self.ButtonsHideCostume()
			self.costume_hide_load()

			self.slotIdxList[1] = wndEquip.GetSlotIndexList()
		if pageIdx == 2:
			wndEquip = self.GetChild("EquipmentSlot_"+str(pageIdx))
			wndEquip.ClearAllSlot() # index / x / y / width / height
			wndEquip.AppendSlot(239, 9, 18, 32, 32)
			wndEquip.AppendSlot(240, 59, 18, 32, 32)
			wndEquip.AppendSlot(241, 111, 18, 32, 32)
			wndEquip.AppendSlot(226, 9, 76, 32, 32)
			wndEquip.AppendSlot(227, 59, 76, 32, 32)
			wndEquip.AppendSlot(228, 111, 76, 32, 32)
			wndEquip.AppendSlot(237, 32, 131, 32, 32)
			wndEquip.AppendSlot(238, 82, 131, 32, 32)

			self.slotIdxList[2] = wndEquip.GetSlotIndexList()

		if pageIdx == 3:
			net.SendChatPacket("/buffi_costume_page {}".format(1))
			wndEquip = self.GetChild("EquipmentSlot_"+str(pageIdx))
			wndEquip.ClearAllSlot() # index / x / y / width / height
			wndEquip.AppendSlot(216, 62, 54, 32, 64)
			wndEquip.AppendSlot(217, 62, 15, 32, 32)
			wndEquip.AppendSlot(218, 11, 15, 32, 96)
			wndEquip.AppendSlot(219, 116, 13, 32, 32)
			wndEquip.AppendSlot(item.SLOT_MOUNT_SKIN, 110, 141, 32, 32)
			wndEquip.AppendSlot(item.EQUIPMENT_OLDPET, 15, 141, 32, 32)
			wndEquip.AppendSlot(item.COSTUME_SLOT_START+2, 60, 141, 32, 32)

			self.slotIdxList[3] = wndEquip.GetSlotIndexList()

		if pageIdx == 4:
			self.GetChild("pendant_page_"+str(self.pendantPageIdx)).Down()
			self.GetChild("pendant_page_"+str(0 if self.pendantPageIdx else 1)).SetUp()

			wndEquip = self.GetChild("EquipmentSlot_"+str(pageIdx))
			wndEquip.ClearAllSlot()
			wndEquip.AppendSlot(232, 60, 10, 32, 32)
			wndEquip.AppendSlot(231, 111, 46, 32, 32)
			wndEquip.AppendSlot(236, 111, 107, 32, 32)
			wndEquip.AppendSlot(235, 60, 144, 32, 32)
			wndEquip.AppendSlot(233, 10, 107, 32, 32)
			wndEquip.AppendSlot(234, 10, 46, 32, 32)
			wndEquip.AppendSlot(242 if self.pendantPageIdx == 0 else 243, 61, 70, 32, 32)

			self.slotIdxList[4] = wndEquip.GetSlotIndexList()

		self.RefreshEquipSlotWindow()

	def RefreshEquipSlotWindow(self):
		wndEquip = self.GetChild("EquipmentSlot_"+str(self.equipmentTypeIndex))
		slotIdxList = self.slotIdxList[self.equipmentTypeIndex]
		for slotNumber in slotIdxList:
			wndEquip.SetItemSlot(slotNumber, player.GetItemIndex(slotNumber), 0)
			if slotNumber== 242 or slotNumber== 243:
				if self.__CheckAllPendant():
					self.GetChild("Pendant_Wind_Glow").Show()
					self.GetChild("Pendant_Earth_Glow").Show()
					self.GetChild("Pendant_Dark_Glow").Show()
					self.GetChild("Pendant_Thunder_Glow").Show()
					self.GetChild("Pendant_Fire_Glow").Show()
					self.GetChild("Pendant_Ice_Glow").Show()
					wndEquip.ActivateSlot(slotNumber)
				else:
					self.GetChild("Pendant_Wind_Glow").Hide()
					self.GetChild("Pendant_Earth_Glow").Hide()
					self.GetChild("Pendant_Dark_Glow").Hide()
					self.GetChild("Pendant_Thunder_Glow").Hide()
					self.GetChild("Pendant_Fire_Glow").Hide()
					self.GetChild("Pendant_Ice_Glow").Hide()
					wndEquip.DeactivateSlot(slotNumber)
		wndEquip.RefreshSlot()

	def RefreshItemSlot(self):
		self.RefreshBagSlotWindow()
		self.RefreshEquipSlotWindow()

	def OverInToolTip(self, arg):
		arglen = len(str(arg))
		pos_x, pos_y = wndMgr.GetMousePosition()
		
		self.toolTip.ClearToolTip()
		self.toolTip.SetThinBoardSize(11 * arglen)
		self.toolTip.SetToolTipPosition(pos_x + 5, pos_y - 5)
		self.toolTip.AppendTextLine(arg, 0xffffff00)
		self.toolTip.Show()

	def OverOutToolTip(self):
		self.toolTip.Hide()

	def GetChangeEquipIndex(self):
		return self.ChangeEquipIndex

	# DSSButton
	def ClickDSSButton(self):
		print "click_dss_button"
		self.interface.ToggleDragonSoulWindow()

	def UseDSSButtonEffect(self, enable):
		if self.DSSButton:
			DSS_BUTTON_1 = "d:/ymir work/ui/dragonsoul/32132113.tga"
			DSS_BUTTON_2 = "d:/ymir work/ui/dragonsoul/32132113.tga"#test
			DSS_BUTTON_3 = "d:/ymir work/ui/dragonsoul/32132113.tga"

			DSSButtonEffect = ui.SlotWindow()
			DSSButtonEffect.AddFlag("attach")
			DSSButtonEffect.SetParent(self.DSSButton)
			DSSButtonEffect.SetPosition(3.2, 0)

			DSSButtonEffect.AppendSlot(0, 0, 0, 32, 32)
			DSSButtonEffect.SetRenderSlot(0)
			DSSButtonEffect.RefreshSlot()

			if enable == True:
				DSSButtonEffect.ActivateSlot(0)

				self.DSSButton.SetUpVisual("d:/ymir work/ui/dragonsoul/32132113.tga")
				self.DSSButton.SetOverVisual("d:/ymir work/ui/dragonsoul/32132113.tga")
				self.DSSButton.SetDownVisual("d:/ymir work/ui/dragonsoul/32132113.tga")

				DSSButtonEffect.Show()
			else:
				DSSButtonEffect.DeactivateSlot(0)

				self.DSSButton.SetUpVisual("d:/ymir work/ui/dragonsoul/dss_inventory_button_01.tga")
				self.DSSButton.SetOverVisual("d:/ymir work/ui/dragonsoul/dss_inventory_button_02.tga")
				self.DSSButton.SetDownVisual("d:/ymir work/ui/dragonsoul/dss_inventory_button_03.tga")

				DSSButtonEffect.Hide()
			self.DSSButtonEffect = DSSButtonEffect

	if app.__RENEWAL_BRAVE_CAPE__:
		def ClickBraveCape(self):
			if self.wndBraveCape:
				if self.wndBraveCape.IsShow():
					self.wndBraveCape.Close()
				else:
					self.wndBraveCape.Open()

	def OpenPickMoneyDialog(self):

		if mouseModule.mouseController.isAttached():

			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			if player.SLOT_TYPE_SAFEBOX == mouseModule.mouseController.GetAttachedType():

				if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
					net.SendSafeboxWithdrawMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
					snd.PlaySound("sound/ui/money.wav")

			mouseModule.mouseController.DeattachObject()

		else:
			curMoney = player.GetElk()

			if curMoney <= 0:
				return

			self.dlgPickMoney.SetTitleName(localeInfo.PICK_MONEY_TITLE)
			self.dlgPickMoney.SetAcceptEvent(ui.__mem_func__(self.OnPickMoney))
			self.dlgPickMoney.Open(curMoney)
			self.dlgPickMoney.SetMax(9) # ?I???a¢¬¢ç 990000 A|C? ?o¡¾¡¿ ?oA¢´

	def OnPickMoney(self, money):
		mouseModule.mouseController.AttachMoney(self, player.SLOT_TYPE_INVENTORY, money)

	def OnPickItem(self, count):
		itemSlotIndex = self.dlgPickMoney.itemGlobalSlotIndex
		selectedItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotIndex)
		mouseModule.mouseController.AttachObject(self, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["slot"], itemSlotIndex, selectedItemVNum, count)

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

			onTopWnd = self.interface.GetOnTopWindow()
			if localIndex:
				slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(localIndex)
				
				if onTopWnd == player.ON_TOP_WND_NONE:
					self.wndItem.SetUsableSlotOnTopWnd(localIndex)
					
				elif onTopWnd == player.ON_TOP_WND_SHOP:
					if player.IsAntiFlagBySlot(slotNumber, item.ANTIFLAG_SELL):
						self.wndItem.SetUnusableSlotOnTopWnd(localIndex)
					else:
						self.wndItem.SetUsableSlotOnTopWnd(localIndex)

				elif onTopWnd == player.ON_TOP_WND_EXCHANGE:
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

			for i in xrange(player.INVENTORY_PAGE_SIZE):
				slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(i)

				if onTopWnd == player.ON_TOP_WND_NONE:
					self.wndItem.SetUsableSlotOnTopWnd(i)

				elif onTopWnd == player.ON_TOP_WND_SHOP:
					if player.IsAntiFlagBySlot(slotNumber, item.ANTIFLAG_SELL):
						self.wndItem.SetUnusableSlotOnTopWnd(i)
					else:
						self.wndItem.SetUsableSlotOnTopWnd(i)

				elif onTopWnd == player.ON_TOP_WND_EXCHANGE:
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

	def RefreshBagSlotWindow(self):
		### Sometimes this function is called
		### when self.wndItem == None so better
		### return when that occurs.
		if not self.wndItem:
			return

		getItemVNum=player.GetItemIndex
		getItemCount=player.GetItemCount
		setItemVNum=self.wndItem.SetItemSlot
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

			itemCount = getItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotNumber)
			if 0 == itemCount:
				self.wndItem.ClearSlot(i)
				continue
			elif 1 == itemCount:
				itemCount = 0

			itemVnum = getItemVNum(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotNumber)

			setItemVNum(i, itemVnum, itemCount)
			self.wndItem.DeactivateSlot(i)
			
			if app.ENABLE_IKASHOP_RENEWAL:
				if app.EXTEND_IKASHOP_PRO:
					self.wndItem.EnableSlot(i)
					board = ikashop.GetBusinessBoard()
					if board and (board.IsShow() or board.IsCreatingAuction()):
						item.SelectItem(itemVnum)
						if item.IsAntiFlag(item.ANTIFLAG_GIVE) or item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
							self.wndItem.DisableSlot(i)
			
			if not self.GetIsCategorySpecial():
				if itemVnum == 0 and slotNumber in self.liHighlightedItems:
					self.liHighlightedItems.remove(slotNumber)

			wasActivated = False

			## ?U¥ì??¡Æ?? (HP: #72723 ~ #72726, SP: #72727 ~ #72730) ???o?©©¢¬¢ç - ???????I¥ì?¥ì¥ì ¢©¢©¡¤O?¢§ ?¡Æ???¡©/???¡Æ???¡© C?¢©?¢¬| ?¡×C? ???¡À?O - [hyo]
			if constInfo.IS_AUTO_POTION(itemVnum):
				# metinSocket - [0] : ?¡Æ???¡© ?¨Ï?I, [1] : ¡íc?eC? ?c, [2] : ?O¢¥e ?e¡¤¢ç
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]
				isActivated = 0 != metinSocket[0]

				if isActivated:
					wasActivated = True

					self.wndItem.ActivateSlot(i, 171.0 / 255.0, 85.0 / 255.0, 0.0 / 255.0, 1.0)

					potionType = 0;
					if constInfo.IS_AUTO_POTION_HP(itemVnum):
						potionType = player.AUTO_POTION_TYPE_HP
					elif constInfo.IS_AUTO_POTION_SP(itemVnum):
						potionType = player.AUTO_POTION_TYPE_SP
					
					usedAmount = int(metinSocket[1])
					totalAmount = int(metinSocket[2])
					player.SetAutoPotionInfo(potionType, isActivated, (totalAmount - usedAmount), totalAmount, self.__InventoryLocalSlotPosToGlobalSlotPos(i))
				else:
					self.wndItem.DeactivateSlot(i)
			else:
				if app.USE_CRYSTAL_SYSTEM:
					for v in constInfo.SHARD_CRAFT_SLOTS_DICT.values():
						if v == slotNumber:
							wasActivated = True
							self.wndItem.ActivateSlotOld(i)

				if wasActivated == False:
					self.wndItem.DeactivateSlot(i)

			if app.ENABLE_SASH_SYSTEM and not wasActivated:
				slotNumberChecked = 0
				self.wndItem.DeactivateSlotOld(i)

				for j in xrange(sash.WINDOW_MAX_MATERIALS):
					(isHere, iCell) = sash.GetAttachedItem(j)
					if isHere:
						if iCell == slotNumber:
							wasActivated = True

							self.wndItem.ActivateSlotOld(i)
							if not slotNumber in self.listAttachedSashs:
								self.listAttachedSashs.append(slotNumber)

							slotNumberChecked = 1
					else:
						if slotNumber in self.listAttachedSashs and not slotNumberChecked:
							self.wndItem.DeactivateSlotOld(i)
							self.listAttachedSashs.remove(slotNumber)

			if itemVnum >= 49330 and itemVnum <= 49332 or itemVnum >= 49336 and itemVnum <= 49338 or itemVnum >= 49342 and itemVnum <= 49344:
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]
				if metinSocket[0]-app.GetGlobalTimeStamp() < 0:
					self.wndItem.SetUnusableSlot(i)

			if itemVnum == 72501 and not wasActivated:
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]
				isActivated = metinSocket[0]
				if isActivated == 1:
					wasActivated = True

					self.wndItem.ActivateSlot(i, 171.0 / 255.0, 85.0 / 255.0, 0.0 / 255.0, 1.0)
				else:
					self.wndItem.DeactivateSlot(i)

			if app.NEW_PET_SYSTEM and not wasActivated:
				if self.tooltipItem:
					pets = self.tooltipItem.GetPetList()
					for date in pets:
						if itemVnum == date[0]:
							metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]
							isActivated = metinSocket[2]
							if isActivated:
								wasActivated = True

								self.wndItem.ActivateSlotOld(i)
							else:
								self.wndItem.DeactivateSlotOld(i)

				if not wasActivated:
					mount = self.tooltipItem.GetMountList()
					for date in mount:
						if itemVnum == date[0]:
							metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]
							isActivated = metinSocket[2]
							if isActivated:
								wasActivated = True

								self.wndItem.ActivateSlotOld(i)
							else:
								self.wndItem.DeactivateSlotOld(i)

			if app.USE_CRYSTAL_SYSTEM or app.__BUFFI_SUPPORT__ or app.USE_BOOST_POTIONS and not wasActivated:
				item.SelectItem(itemVnum)
				itemType = item.GetItemType()
				itemSubType = item.GetItemSubType()

				if app.USE_CRYSTAL_SYSTEM and itemType == item.ITEM_TYPE_CRYSTAL:
					if player.GetItemMetinSocket(slotNumber, item.CRYSTAL_STATUS_SOCKET_SLOT) != 0:
						wasActivated = True

						self.wndItem.ActivateSlot(i, 171.0 / 255.0, 85.0 / 255.0, 0.0 / 255.0, 1.0)
					else:
						self.wndItem.DeactivateSlot(i)

			if app.__BUFFI_SUPPORT__ and not wasActivated:
				if itemType == item.ITEM_TYPE_BUFFI and itemSubType == item.BUFFI_SCROLL:
					if player.GetItemMetinSocket(slotNumber, 1) == 1:
						wasActivated = True

						self.wndItem.ActivateSlot(i, 153, 77, 0)
					else:
						self.wndItem.DeactivateSlot(i)

			if app.USE_BOOST_POTIONS and not wasActivated and itemType == item.ITEM_TYPE_USE and itemSubType == item.USE_SPECIAL_BOOST:
				if player.GetItemMetinSocket(slotNumber, item.BOOST_POTION_STATUS) != 0:
					wasActivated = True

					self.wndItem.ActivateSlot(i, 171.0 / 255.0, 85.0 / 255.0, 0.0 / 255.0, 1.0)
				else:
					self.wndItem.DeactivateSlot(i)

			if app.WJ_ENABLE_TRADABLE_ICON:
				self.RefreshMarkSlots(i)

		self.__RefreshHighlights()
		self.wndItem.RefreshSlot()

		if app.__BL_BATTLE_ROYALE__ and background.IsBattleRoyaleMap():
			self.__RefreshBattleRoyaleExinvenCoverSlot()
			self.__RefreshBattleRoyaleExinvenSlotState()

		if app.WJ_ENABLE_TRADABLE_ICON:
			map(lambda wnd:wnd.RefreshLockedSlot(), self.bindWnds)
		if ENABLE_SEARCH_FIELD:
			if self.wndName.GetText() != "":
				self.OnValueUpdateSearch()

		if self.interface:
			if self.interface.wndInventoryNew.wndName.GetText() != "":
				self.interface.wndInventoryNew.OnValueUpdateSearch()

	def HighlightSlot(self, slot):
		if not slot in self.liHighlightedItems:
			self.liHighlightedItems.append(slot)
	
	def __RefreshHighlights(self):
		for i in xrange(player.INVENTORY_PAGE_SIZE):
			slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(i)
			if slotNumber in self.liHighlightedItems:
				self.wndItem.ActivateSlotOld(i)

	if app.ENABLE_HIDE_COSTUME_SYSTEM:
		def ButtonsHideCostume(self):
			self.elemets_world["position"] = [
				[70,28],		# 0 hide costum
				[80 - 5,5 - 20],		 # 1 Hair Costum
				[27- 4,10- 40],		  # 2 Costum Arma
				[200,28]	  # 3
			]

			for i in xrange(self.GetSlotCount()):
				if i == 3:
					continue
				self.elemets_world["hide_button_%d"%i] = ui.Button()
				self.elemets_world["hide_button_%d"%i].SetParent(self.GetChild("EquipmentSlot_1"))
				self.elemets_world["hide_button_%d"%i].SetPosition(self.elemets_world["position"][i][0]+12,self.elemets_world["position"][i][1]+37)
				self.elemets_world["hide_button_%d"%i].SetUpVisual("Modulo/HideCostume/button_show_0.tga")
				self.elemets_world["hide_button_%d"%i].SetOverVisual("Modulo/HideCostume/button_show_1.tga")
				self.elemets_world["hide_button_%d"%i].SetDownVisual("Modulo/HideCostume/button_show_0.tga")
				self.elemets_world["hide_button_%d"%i].SetEvent(self.FuncHide,i)
				self.elemets_world["hide_button_%d"%i].Hide()

		def FuncHide(self,index):
			net.SendChatPacket("/costume_hide %d" %index)

		def costume_hide_load(self):
			for i in xrange(self.GetSlotCount()):
				if i == 3:
					continue
				
				if len(self.elemets_hide) > 0:
					self.elemets_world["hide_button_%d"%self.elemets_hide[i][0]].SetUpVisual("Modulo/HideCostume/button_%s_0.tga"%self.ButtonInfoHide(self.elemets_hide[i][1]))
					self.elemets_world["hide_button_%d"%self.elemets_hide[i][0]].SetOverVisual("Modulo/HideCostume/button_%s_1.tga"%self.ButtonInfoHide(self.elemets_hide[i][1]))
					self.elemets_world["hide_button_%d"%self.elemets_hide[i][0]].SetDownVisual("Modulo/HideCostume/button_%s_0.tga"%self.ButtonInfoHide(self.elemets_hide[i][1]))
				self.elemets_world["hide_button_%d"%i].Show()
			self.elemets_world["hide_button_5"].Show()

		def costume_hide_clear(self):
			self.elemets_hide = []

		def costume_hide_list(self,slot,index):
			self.elemets_hide.append([int(slot),int(index)])

		def get_costume_hide_list(self):
			return self.elemets_hide

	def OnUpdate(self):
		if constInfo.Inventory_Locked["Active"]:
			if constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())] >= 18:
				for i in xrange(18):
					
					self.lock[i].Hide()
			else:
				if self.inventoryPageIndex >= 2:
					self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetUpVisual("d:/ymir work/drakon2/inventory/1.tga")
					self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetOverVisual("d:/ymir work/drakon2/inventory/1.tga")
					self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetDownVisual("d:/ymir work/drakon2/inventory/1.tga")

					for i in xrange(9):
						self.lock[i].Show()
						if constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())] > i:
							self.lock[i].Hide()
				else:
					for i in xrange(9):
						self.lock[i].Hide()
						
				if self.inventoryPageIndex == 3:
					self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetUpVisual("d:/ymir work/drakon2/inventory/1.tga")
					self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetOverVisual("d:/ymir work/drakon2/inventory/1.tga")
					self.lock[constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())]].SetDownVisual("d:/ymir work/drakon2/inventory/1.tga")

					for i in xrange(9,18):
						self.lock[i].Show()
						if constInfo.Inventory_Locked["Keys_Can_Unlock_%d"%(self.GetCategoryIndex())] > i:
							self.lock[i].Hide()
				else:
					for i in xrange(9,18):
						self.lock[i].Hide()

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def SellItem(self):
		if self.sellingSlotitemIndex == player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],self.sellingSlotNumber):
			if self.sellingSlotitemCount == player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],self.sellingSlotNumber):
				## ?e???¢ç¥ì¥ì ??¢¬¢ç¡ÆO C?¢¥A ¡¾a¢¥E ?©¬¡Æ¢§C?¢¬e?¡© ?I?U type ?©¬¡Æ¢§
				net.SendShopSellPacketNew(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], self.sellingSlotNumber, self.questionDialog.count)
				snd.PlaySound("sound/ui/money.wav")
		self.OnCloseQuestionDialog()

	def OnDetachMetinFromItem(self):
		if None == self.questionDialog:
			return

		#net.SendItemUseToItemPacket(self.questionDialog.sourcePos, self.questionDialog.targetPos)
		self.__SendUseItemToItemPacket(self.questionDialog.sourceInv, self.questionDialog.sourcePos, self.questionDialog.targetPos)
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

	## Slot Event
	def SelectEmptySlot(self, selectedSlotPos):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
			return

		if app.ENABLE_AURA_SYSTEM:
			if player.IsAuraRefineWindowOpen():
				return

		selectedSlotPos = self.__InventoryLocalSlotPosToGlobalSlotPos(selectedSlotPos)

		if mouseModule.mouseController.isAttached():

			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()
			attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()

			slot_type_list = [player.SLOT_TYPE_INVENTORY,player.SLOT_TYPE_UPGRADE_INVENTORY,player.SLOT_TYPE_BOOK_INVENTORY,player.SLOT_TYPE_STONE_INVENTORY,player.SLOT_TYPE_CHANGE_INVENTORY,player.SLOT_TYPE_COSTUME_INVENTORY,player.SLOT_TYPE_CHEST_INVENTORY]

			if attachedSlotType in slot_type_list:
				attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)

				itemCount = player.GetItemCount(attachedInvenType,attachedSlotPos)
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				#self.__SendMoveItemPacket(attachedSlotPos, selectedSlotPos, attachedCount)

				
				#if attachedInvenType == player.DRAGON_SOUL_INVENTORY:
					

				if player.IsDSEquipmentSlot(attachedInvenType, attachedSlotPos):
					srcItemPos = (attachedInvenType, attachedSlotPos)
					dstItemPos = (self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)
					self.__OpenQuestionDialog(srcItemPos, dstItemPos)
				elif self.dlgPickMoney and self.dlgPickMoney.IsSplitAll() and player.SLOT_TYPE_INVENTORY == attachedSlotType:
					net.SendChatPacket("/split_items %d %d %d" % (attachedSlotPos, attachedCount, selectedSlotPos))
					self.dlgPickMoney.SplitClear()
				else:
					self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)


			if player.SLOT_TYPE_INVENTORY == attachedSlotType:
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				# if self.dlgPickMoney and self.dlgPickMoney.IsSplitAll():
					# net.SendChatPacket("/split_items %d %d %d" % (attachedSlotPos, attachedCount, selectedSlotPos))
					# self.dlgPickMoney.SplitClear()
				# else:
					# self.__SendMoveItemPacket(attachedSlotPos, selectedSlotPos, attachedCount)


				if item.IsRefineScroll(attachedItemIndex):
					self.wndItem.SetUseMode(False)

			if player.SLOT_TYPE_CHANGE_EQUIP == attachedSlotType:
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				net.SendItemMovePacket(player.CHANGE_EQUIP, attachedSlotPos, player.INVENTORY, selectedSlotPos, attachedCount)

			elif player.SLOT_TYPE_SWITCHBOT == attachedSlotType:
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				net.SendItemMovePacket(player.SWITCHBOT, attachedSlotPos, player.INVENTORY, selectedSlotPos, attachedCount)

			elif player.SLOT_TYPE_PRIVATE_SHOP == attachedSlotType:
				mouseModule.mouseController.RunCallBack("INVENTORY")

			elif player.SLOT_TYPE_SHOP == attachedSlotType:
				net.SendShopBuyPacket(attachedSlotPos)

			elif player.SLOT_TYPE_SAFEBOX == attachedSlotType:

				if player.ITEM_MONEY == attachedItemIndex:
					net.SendSafeboxWithdrawMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
					snd.PlaySound("sound/ui/money.wav")

				else:
					net.SendSafeboxCheckoutPacket(attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos)

			elif player.SLOT_TYPE_MALL == attachedSlotType:
				net.SendMallCheckoutPacket(attachedSlotPos, selectedSlotPos)
			elif app.ENABLE_AURA_SYSTEM and player.SLOT_TYPE_AURA == attachedSlotType:
				net.SendAuraRefineCheckOut(attachedSlotPos, player.GetAuraRefineWindowType())

			mouseModule.mouseController.DeattachObject()

	def SelectItemSlot(self, itemSlotIndex):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
			return

		itemSlotIndex = self.__InventoryLocalSlotPosToGlobalSlotPos(itemSlotIndex)
		
		if ENABLE_SEARCH_FIELD:
			if self.wndName.GetText() and self.wndItemSRC.dictItems.has_key(itemSlotIndex):
				window, pos = self.wndItemSRC.dictItems[itemSlotIndex]
				itemSlotIndex = pos
				
				selectedItemVNum = player.GetItemIndex(window, itemSlotIndex)
				itemCount = player.GetItemCount(window, itemSlotIndex)

		if app.IsPressed(app.DIK_LCONTROL):
			if self.interface and player.IsEquipmentSlot(itemSlotIndex) == 0 and self.interface.AttachInvenItemToOtherWindowSlot(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex):
				return


		if mouseModule.mouseController.isAttached():
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemVID = mouseModule.mouseController.GetAttachedItemIndex()
			attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)


			if app.ENABLE_SPECIAL_STORAGE:
				if player.SLOT_TYPE_INVENTORY == attachedSlotType or player.SLOT_TYPE_STONE_INVENTORY == attachedSlotType or player.SLOT_TYPE_CHANGE_INVENTORY == attachedSlotType or player.SLOT_TYPE_UPGRADE_INVENTORY == attachedSlotType or player.SLOT_TYPE_BOOK_INVENTORY == attachedSlotType or player.SLOT_TYPE_COSTUME_INVENTORY == attachedSlotType or player.SLOT_TYPE_CHEST_INVENTORY == attachedSlotType:
					self.__DropSrcItemToDestItemInInventory(attachedItemVID, attachedInvenType, attachedSlotPos, itemSlotIndex, attachedSlotType)
			else:
				if player.SLOT_TYPE_INVENTORY == attachedSlotType:
					self.__DropSrcItemToDestItemInInventory(attachedItemVID, attachedInvenType, attachedSlotPos, itemSlotIndex, attachedSlotType)

			mouseModule.mouseController.DeattachObject()

		else:

			curCursorNum = app.GetCursor()
			if app.SELL == curCursorNum:
				self.__SellItem(itemSlotIndex)
			
			elif app.IsCanOpenRender() and app.ENABLE_RENDER_TARGET:
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
				#else:
					#selectedItemVNum = player.GetItemIndex(itemSlotIndex)
					#mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_INVENTORY, itemSlotIndex, selectedItemVNum)

			elif app.IsPressed(app.DIK_LCONTROL):
				itemIndex = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotIndex)

				if not self.interface.wndInventoryNew.GetCategoryIndex() == 7:
					if True == item.CanAddToQuickSlotItem(itemIndex):
						player.RequestAddToEmptyLocalQuickSlot(player.SLOT_TYPE_INVENTORY, itemSlotIndex)
					else:
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.QUICKSLOT_REGISTER_DISABLE_ITEM)

			else:
				selectedItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotIndex)
				itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotIndex)
				mouseModule.mouseController.AttachObject(self, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["slot"], itemSlotIndex, selectedItemVNum, itemCount)

				if self.__IsUsableItemToItem(selectedItemVNum, itemSlotIndex):
					self.wndItem.SetUseMode(True)
				else:
					self.wndItem.SetUseMode(False)

				snd.PlaySound("sound/ui/pick.wav")
				
		if ENABLE_SEARCH_FIELD:
			if self.wndName.GetText() != "":
				self.OnValueUpdateSearch()

		if self.interface.wndInventoryNew.wndName.GetText() != "":
			self.interface.wndInventoryNew.OnValueUpdateSearch()

	def __DropSrcItemToDestItemInInventory(self, srcItemVID, srcItemInv, srcItemSlotPos, dstItemSlotPos, attachedSlotType):
		destItemVnum = player.GetItemIndex(dstItemSlotPos)
		item.SelectItem(destItemVnum)
		destItemType = item.GetItemType()
		destItemSubType = item.GetItemSubType()
		if app.ENABLE_AURA_SYSTEM and player.IsAuraRefineWindowOpen():
			return

		if app.ENABLE_SPECIAL_STORAGE:

			if srcItemSlotPos == dstItemSlotPos and (attachedSlotType != player.SLOT_TYPE_STONE_INVENTORY and attachedSlotType != player.SLOT_TYPE_CHANGE_INVENTORY and attachedSlotType != player.SLOT_TYPE_CHEST_INVENTORY):
				return

			if srcItemSlotPos == dstItemSlotPos and item.IsMetin(srcItemVID) and self.GetIsCategorySpecial() and attachedSlotType == player.SLOT_TYPE_STONE_INVENTORY:
				return

			if srcItemSlotPos == dstItemSlotPos and item.GetUseType(srcItemVID) == "USE_CHANGE_ATTRIBUTE" and self.GetIsCategorySpecial() and attachedSlotType == player.SLOT_TYPE_CHANGE_INVENTORY:
				return

		else:
			if srcItemSlotPos == dstItemSlotPos:
				return
				
		if constInfo.IS_COSTUME_TIME_EXTENDER(player.GetItemIndex(srcItemSlotPos)):
			item.SelectItem(player.GetItemIndex(dstItemSlotPos))
			# if item.GetItemType() == item.ITEM_TYPE_COSTUME and (item.GetItemSubType() == item.COSTUME_TYPE_BODY or item.GetItemSubType() == item.COSTUME_TYPE_HAIR or item.GetItemSubType() == item.COSTUME_TYPE_WEAPON):
			self.__SendUseItemToItemPacket(srcItemInv, srcItemSlotPos, dstItemSlotPos)

		# cyh itemseal 2013 11 08
		if app.ENABLE_SOULBIND_SYSTEM and item.IsSealScroll(srcItemVID):
			self.__SendUseItemToItemPacket(srcItemInv, srcItemSlotPos, dstItemSlotPos)
		elif srcItemVID == 71068:
			self.__SendUseItemToItemPacket(srcItemInv, srcItemSlotPos, dstItemSlotPos)

		elif item.IsRefineScroll(srcItemVID):
			self.RefineItem(srcItemInv, srcItemSlotPos, dstItemSlotPos)
			self.wndItem.SetUseMode(False)

		elif item.IsMetin(srcItemVID):
			self.AttachMetinToItem(srcItemInv, srcItemSlotPos, dstItemSlotPos)

		
		elif destItemType == item.ITEM_TYPE_PET and destItemSubType == item.PET_LEVELABLE:
			if srcItemVID == 55008:
				self.interface.OpenChangeNameWindow(srcItemSlotPos,dstItemSlotPos,destItemVnum, 1)
			elif srcItemVID == 72325:
				self.DetachMetinFromItem(srcItemInv,srcItemSlotPos, dstItemSlotPos)
			elif (srcItemVID == 55001 or srcItemVID == 55033 or (srcItemVID >= 55010 and srcItemVID <= 55031)):
				self.questionDialog = uiCommon.QuestionDialog()
				if 55001 == srcItemVID:
					self.questionDialog.SetText(localeInfo.PET_PROTEIN_QUESTION_TEXT)
				elif 55033 == srcItemVID:
					self.questionDialog.SetText(localeInfo.PET_REMOVE_QUESTION_TEXT)
				else:
					self.questionDialog.SetText(localeInfo.INVENTORY_REALLY_USE_ITEM % item.GetItemName())
				self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.__UseItemToItemQuestionDialog_OnAccept))
				self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnCancel))
				self.questionDialog.Open()
				self.questionDialog.srcItemSlotPos = srcItemSlotPos
				self.questionDialog.dstItemSlotPos = dstItemSlotPos
				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

		elif item.IsDetachScroll(srcItemVID):
			self.DetachMetinFromItem(srcItemInv,srcItemSlotPos, dstItemSlotPos)

		elif item.IsKey(srcItemVID):
			self.__SendUseItemToItemPacket(srcItemInv, srcItemSlotPos, dstItemSlotPos)

		elif (player.GetItemFlags(srcItemSlotPos) & ITEM_FLAG_APPLICABLE) == ITEM_FLAG_APPLICABLE:
			self.__SendUseItemToItemPacket(srcItemInv, srcItemSlotPos, dstItemSlotPos)

		elif item.GetUseType(srcItemVID) in self.USE_TYPE_TUPLE:
			self.__SendUseItemToItemPacket(srcItemInv, srcItemSlotPos, dstItemSlotPos, srcItemVID)
		else:
			#snd.PlaySound("sound/ui/drop.wav")

			## ??¥ì?¢©??¢­ ¡Æ¡À?? ??A? ¢©¢©¡¤O?? ¡Æ??? ???????¡í ¡íc?eC??¡© ??A? ¢©??¢­¢¥? - [levites]
			if player.IsEquipmentSlot(dstItemSlotPos):

				## ¥ìe¡Æi ?O¢¥A ???????? ??????¢Ò¡×¢¬¢¬
				if item.IsEquipmentVID(srcItemVID):
					self.__UseItem(srcItemSlotPos)

			else:

				#itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],attachedSlotPos)
				#attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				#self.__SendMoveItemPacket(attachedSlotPos, selectedSlotPos, attachedCount)

				#attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)

				#self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)


			#	attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
				
				#self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, 0)

				net.SendItemMovePacket(srcItemInv, srcItemSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], dstItemSlotPos, 0)

				#self.__SendMoveItemPacket(srcItemSlotPos, dstItemSlotPos, 0)
				#return
				#net.SendItemMovePacket(srcItemSlotPos, dstItemSlotPos, 0)

	def __SellItem(self, itemSlotPos):
		if not player.IsEquipmentSlot(itemSlotPos):
			self.sellingSlotNumber = itemSlotPos
			itemIndex = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotPos)
			itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotPos)


			self.sellingSlotitemIndex = itemIndex
			self.sellingSlotitemCount = itemCount

			item.SelectItem(itemIndex)
			## ???? C?¡¤?¡¾¡¿ ¡ÆE¡íc ?uA¢ç?¡© ?©¬¡Æ¢§
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

	def RefineItem(self, scrollInv, scrollSlotPos, targetSlotPos):

		scrollIndex = player.GetItemIndex(scrollSlotPos)
		targetIndex = player.GetItemIndex(targetSlotPos)

		if player.REFINE_OK != player.CanRefine(scrollIndex, targetSlotPos):
			return

		if app.ENABLE_REFINE_RENEWAL:
			constInfo.AUTO_REFINE_TYPE = 1
			constInfo.AUTO_REFINE_DATA["ITEM"][0] = scrollSlotPos
			constInfo.AUTO_REFINE_DATA["ITEM"][1] = targetSlotPos

		###########################################################
		self.__SendUseItemToItemPacket(scrollInv, scrollSlotPos, targetSlotPos)
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

	def DetachMetinFromItem(self, scroolInv, scrollSlotPos, targetSlotPos):
		scrollIndex = player.GetItemIndex(scroolInv, scrollSlotPos)
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
		self.questionDialog.sourceInv = scroolInv
		self.questionDialog.sourcePos = scrollSlotPos
		self.questionDialog.targetPos = targetSlotPos

	def AttachMetinToItem(self, metinInv, metinSlotPos, targetSlotPos):
		metinIndex = player.GetItemIndex(metinInv, metinSlotPos)
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

		self.attachMetinDialog.Open(metinInv, metinSlotPos, targetSlotPos)

	def OverOutEmptySlot(self):
		self.wndItem.SetUsableItem(False)
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OverInEmptySlot(self, overSlotPos):
		if self.tooltipItem != None:
			try:
				slotName = self.SLOTNAMES[overSlotPos]
				self.tooltipItem.ClearToolTip()
				self.tooltipItem.AppendTextLine(slotName, grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0))
				self.tooltipItem.ShowToolTip()
			except:
				self.OverOutItem()

	def OverOutItem(self):
		if self.wndItem:
			self.wndItem.SetUsableItem(False)
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OverInItem(self, overSlotPos):
		overSlotPosGlobal = self.__InventoryLocalSlotPosToGlobalSlotPos(overSlotPos)
		if self.wndItem:
			self.wndItem.SetUsableItem(False)

		if not self.GetIsCategorySpecial():
			if overSlotPosGlobal in self.liHighlightedItems:
				self.liHighlightedItems.remove(overSlotPosGlobal)
				self.wndItem.DeactivateSlotOld(overSlotPos)
				
		if mouseModule.mouseController.isAttached():
			attachedItemType = mouseModule.mouseController.GetAttachedType()
			if app.ENABLE_SPECIAL_STORAGE:
				if player.SLOT_TYPE_INVENTORY == attachedItemType or player.SLOT_TYPE_STONE_INVENTORY == attachedItemType or player.SLOT_TYPE_CHANGE_INVENTORY == attachedItemType or player.SLOT_TYPE_CHEST_INVENTORY == attachedItemType:
					attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
					attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()

					if self.__CanUseSrcItemToDstItem(attachedItemVNum, attachedSlotPos, overSlotPos):
						self.wndItem.SetUsableItem(True)
						self.wndItem.SetUseMode(True)
						#self.ShowToolTip(overSlotPos)
						self.ShowToolTip(overSlotPosGlobal)
						return
			else:
				if player.SLOT_TYPE_INVENTORY == attachedItemType:

					attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
					attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()

					if self.__CanUseSrcItemToDstItem(attachedItemVNum, attachedSlotPos, overSlotPos):
						self.wndItem.SetUsableItem(True)
						self.ShowToolTip(overSlotPosGlobal)
						#self.ShowToolTip(overSlotPos)
						return

		self.ShowToolTip(overSlotPosGlobal)

	def __IsUsableItemToItem(self, srcItemVNum, srcSlotPos):
		if item.IsRefineScroll(srcItemVNum):
			return True
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
		if constInfo.IS_COSTUME_TIME_EXTENDER(srcItemVNum):
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
		elif (player.GetItemFlags(srcSlotPos) & ITEM_FLAG_APPLICABLE) == ITEM_FLAG_APPLICABLE:
			return True
		else:
			if item.GetUseType(srcItemVNum) in self.USE_TYPE_TUPLE:
				return True

		return False

	def __CanUseSrcItemToDstItem(self, srcItemVNum, srcSlotPos, dstSlotPos):
		itemIndex = player.GetItemIndex(dstSlotPos)
		item.SelectItem(itemIndex)
		destItemType = item.GetItemType()
		destItemSubType = item.GetItemSubType()

		if app.ENABLE_SPECIAL_STORAGE:
			#if srcSlotPos == dstSlotPos and not item.IsMetin(srcItemVNum):
			if srcSlotPos == dstSlotPos and (not item.IsMetin(srcItemVNum) and item.GetUseType(srcItemVNum) != "USE_CHANGE_ATTRIBUTE"):
				return False
			if item.IsMetin(srcItemVNum):
				return True
		else:
			if srcSlotPos == dstSlotPos:
				return False

		if constInfo.IS_COSTUME_TIME_EXTENDER(srcItemVNum):
			item.SelectItem(player.GetItemIndex(dstSlotPos))
			if item.GetItemType() == item.ITEM_TYPE_COSTUME and (item.GetItemSubType() == item.COSTUME_TYPE_BODY or item.GetItemSubType() == item.COSTUME_TYPE_HAIR or item.GetItemSubType() == item.COSTUME_TYPE_WEAPON):
				return True		

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
		elif (player.GetItemFlags(srcSlotPos) & ITEM_FLAG_APPLICABLE) == ITEM_FLAG_APPLICABLE:
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
				if self.__CanAddItemAttr(dstSlotPos, srcItemVNum):
					return True
			elif "USE_ADD_ATTRIBUTE2" == useType:
				if self.__CanAddItemAttr(dstSlotPos, srcItemVNum):
					return True
			elif "USE_ADD_ACCESSORY_SOCKET" == useType:
				if self.__CanAddAccessorySocket(dstSlotPos):
					return True
			elif "USE_PUT_INTO_ACCESSORY_SOCKET" == useType:
				if self.__CanPutAccessorySocket(dstSlotPos, srcItemVNum):
					return True;
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
			elif "USE_ADD_BOOSTER_ATTR" == useType:
				if self.__CanChangeBoosterAttr(dstSlotPos):
					return True
			elif "USE_CHANGE_BOOSTER_ATTR" == useType:
				if self.__CanChangeBoosterAttr(dstSlotPos):
					return True
			elif "USE_ADD_BOOSTER_TIME" == useType:
				return True
			elif "USE_PUT_INTO_AURA_SOCKET" == useType:
				dstItemVnum = player.GetItemIndex(dstSlotPos)
				item.SelectItem(dstItemVnum)
				if item.ITEM_TYPE_COSTUME == item.GetItemType() and item.COSTUME_TYPE_AURA == item.GetItemSubType():
					if player.GetItemMetinSocket(dstSlotPos, player.ITEM_SOCKET_AURA_BOOST) == 0:
						return True
			elif app.USE_CRYSTAL_SYSTEM and useType == "USE_ADD_ATTR_CRYSTAL":
				if self.__CanAddCrystalAttr(dstSlotPos, srcItemVNum):
					return True
			elif app.USE_CRYSTAL_SYSTEM and useType == "USE_CHANGE_ATTR_CRYSTAL":
				if self.__CanChangeCrystalAttr(dstSlotPos, srcItemVNum):
					return True
			elif app.USE_CRYSTAL_SYSTEM and useType == "USE_RECHARGE_CRYSTAL":
				if self.__CanRechargeCrystal(dstSlotPos):
					return True

		return False

	def __CanCleanBrokenMetinStone(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if item.ITEM_TYPE_WEAPON != item.GetItemType():
			return False

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemMetinSocket(dstSlotPos, i) == constInfo.ERROR_METIN_STONE:
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

	if app.BL_67_ATTR:
		def __CanChangeItemAttrList2(self, dstSlotPos):
			return uiAttr67Add.Attr67AddWindow.CantAttachToAttrSlot(dstSlotPos, False)

	def __CanChangeItemAttrList(self, dstSlotPos, srcItemVNum):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
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

	def __CanAddItemAttr(self, dstSlotPos, srcSlotPos):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		srcItemVNum = player.GetItemIndex(srcSlotPos)

		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if not item.GetItemType() in (item.ITEM_TYPE_WEAPON, item.ITEM_TYPE_ARMOR, item.ITEM_TYPE_RINGS):
			return False

		attrCount = 0
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemAttribute(dstSlotPos, i) != 0:
				attrCount += 1			

		if srcItemVNum == 71068:
			return True
		
		return False

	if app.USE_CRYSTAL_SYSTEM:
		def __CanAddCrystalAttr(self, dstSlotPos, srcSlotPos):
			dstItemVNum = player.GetItemIndex(dstSlotPos)
			if dstItemVNum == 0:
				return False

			item.SelectItem(dstItemVNum)

			if item.GetItemType() != item.ITEM_TYPE_CRYSTAL:
				return False

			attrCount = 0
			for i in xrange(item.ATTRIBUTE_NORM_NUM):
				if player.GetItemAttribute(dstSlotPos, i) != 0:
					attrCount += 1

			return True if attrCount == item.ATTRIBUTE_NORM_NUM else False

		def __CanChangeCrystalAttr(self, dstSlotPos, srcItemVNum):
			dstItemVNum = player.GetItemIndex(dstSlotPos)
			if dstItemVNum == 0:
				return False

			item.SelectItem(dstItemVNum)

			if item.GetItemType() != item.ITEM_TYPE_CRYSTAL:
				return False

			attrCount = 0
			for i in xrange(item.ATTRIBUTE_NORM_NUM):
				if player.GetItemAttribute(dstSlotPos, i) != 0:
					attrCount += 1

			return False if attrCount == 0 else True

		def __CanRechargeCrystal(self, dstSlotPos):
			dstItemVNum = player.GetItemIndex(dstSlotPos)
			if dstItemVNum == 0:
				return False

			item.SelectItem(dstItemVNum)

			if item.GetItemType() != item.ITEM_TYPE_CRYSTAL:
				return False

			return True

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
						self.AppendIkashopSellToolTip(player.INVENTORY, slotIndex)
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

	def UseItemSlotNew(self,slotIndex,index_default = 0):
		#self.SetInventoryPage(index_default)
		#self.inventoryTab[index_default].Down()
		self.UseItemSlot(slotIndex)
		
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
		if ENABLE_SEARCH_FIELD:
			if self.wndName.GetText() and self.wndItemSRC.dictItems.has_key(slotIndex):
				window, pos = self.wndItemSRC.dictItems[slotIndex]
				slotIndex = pos
				
				# ivenType = window
		
		if app.ENABLE_IKASHOP_RENEWAL:
			board = ikashop.GetBusinessBoard()
			if board:
				item.SelectItem(player.GetItemIndex(player.INVENTORY, slotIndex))
				if not item.IsAntiFlag(item.ANTIFLAG_GIVE) and not item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
					if board.IsCreatingAuction():
						ikashop.AddItemOnAuction(player.INVENTORY, slotIndex)
						return
					elif board.IsShow():
						ikashop.AddItemOnShop(player.INVENTORY, slotIndex)
						return
			
			if app.EXTEND_IKASHOP_ULTIMATE:
				itemVnum = player.GetItemIndex(player.INVENTORY, slotIndex)
				if itemVnum == 52000:
					shopSkinBoard = ikashop.GetShopSkinBoard()
					if shopSkinBoard:
						shopSkinBoard.Open()
					return
				elif itemVnum == 51998:
					if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
						for i in xrange(min(player.GetItemCount(player.INVENTORY, slotIndex), 10)):
							self.__UseItem(slotIndex)
						self.OverOutItem()
						return

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			if self.wndDragonSoulRefine.IsShow():
				self.wndDragonSoulRefine.AutoSetItem((player.INVENTORY, slotIndex), 1)
				return

		if app.ENABLE_SASH_SYSTEM:
			if self.isShowSashWindow():
				sash.Add(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex, 255)
				return

		if app.USE_CRYSTAL_SYSTEM and\
			self.interface and\
			self.interface.InvAddShardCraft(slotIndex):
			return

		if app.IsPressed(app.DIK_LCONTROL) and self.interface.wndInventoryNew.IsShow() and slotIndex < player.EQUIPMENT_SLOT_START and not exchange.isTrading():
			self.AttachItemFromInventory(player.INVENTORY, slotIndex)
			return

		if app.IsPressed(app.DIK_LSHIFT) and exchange.isTrading() and slotIndex < player.EQUIPMENT_SLOT_START:
			item.SelectItem(player.GetItemIndex(slotIndex))
			emptyExchangeSlots = self.GetExchangeEmptyItemPos(item.GetItemSize()[1])
				
			if emptyExchangeSlots == -1:
				return

			net.SendExchangeItemAddPacket(player.INVENTORY, slotIndex, emptyExchangeSlots[0])
			return
			
		self.__UseItem(slotIndex)
		mouseModule.mouseController.DeattachObject()
		self.OverOutItem()
		
	def __GetCurrentItemGrid(self):
		itemGrid = [[False for cell in xrange(safebox.SAFEBOX_PAGE_SIZE)] for page in xrange(4)]

		for page in xrange(4):
			for slot in xrange(safebox.SAFEBOX_PAGE_SIZE):
				itemVnum = safebox.GetItemID(slot + page * safebox.SAFEBOX_PAGE_SIZE)
				if itemVnum <> 0:
					item.SelectItem(itemVnum)
					(w, h) = item.GetItemSize()
					for i in xrange(h):
						itemGrid[page][slot + i * safebox.SAFEBOX_SLOT_X_COUNT] = True

		return itemGrid

	def __FindEmptyCellForSize(self, itemGrid, size):
		for page in xrange(4):
			for slot in xrange(safebox.SAFEBOX_PAGE_SIZE):
				if itemGrid[page][slot] == False:
					possible = True
					for i in xrange(size):
						p = slot + safebox.SAFEBOX_SLOT_X_COUNT * i
						try:
							if itemGrid[page][p] == True:
								possible = False
								break
						except IndexError:
							possible = False
							break

					if possible:
						return slot + page * safebox.SAFEBOX_PAGE_SIZE

		return -1

	def AttachItemFromInventory(self, slotWindow, slotIndex):
		attachedItemID = player.GetItemIndex(slotWindow, slotIndex)
		item.SelectItem(attachedItemID)
		if item.IsAntiFlag(item.ITEM_ANTIFLAG_SAFEBOX):
			return False

		itemGrid = self.__GetCurrentItemGrid()

		item.SelectItem(attachedItemID)
		emptySafeboxSlot = self.__FindEmptyCellForSize(itemGrid, item.GetItemSize()[1])
		if emptySafeboxSlot == -1 or item.GetItemType() == item.ITEM_TYPE_ELK:
			return False

		net.SendSafeboxCheckinPacket(slotWindow, slotIndex, emptySafeboxSlot)
		return True

	def AttachSpecialToInv(self, window, index):
		itemVnum = player.GetItemIndex(window, index)
		item.SelectItem(itemVnum)
		(w, h) = item.GetItemSize()
		pos = self.grid.find_blank(w, h)
		if pos == -1:
			return False
		self.__SendMoveItemPacket(window, index, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], pos, player.GetItemCount(window, index))
		return True

	def AttachItemFromSafebox(self, slotIndex, item_type, itemIndex):
		item.SelectItem(itemIndex)
		if item.GetItemType() == item.ITEM_TYPE_DS:
			return
		(w, h) = item.GetItemSize()
		pos = self.grid.find_blank(w, h)
		if pos == -1:
			return False
		net.SendSafeboxCheckoutPacket(slotIndex, item_type, pos)
		return True

	def __UseItem(self, slotIndex):
		ItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],slotIndex)
		item.SelectItem(ItemVNum)

		if app.ENABLE_AURA_SYSTEM:
			if player.IsAuraRefineWindowOpen():
				self.__UseItemAura(slotIndex)
				return

		if app.IsPressed(app.DIK_LALT) and app.IsPressed(app.DIK_LCONTROL):
			if self.wndWiki:
				if not self.wndWiki.IsShow():
					self.wndWiki.Open()
					
				self.wndWiki.SearchItem(ItemVNum)
				return
				
		elif app.ENABLE_SELL_ITEM and app.IsPressed(app.DIK_LCONTROL):
			if self.IsSellItems(slotIndex):
				self.__SendSellItemPacket(slotIndex)
				return
				
		elif app.ENABLE_SHOW_CHEST_DROP and item.GetItemType() == item.ITEM_TYPE_GIFTBOX and app.IsPressed(app.DIK_LCONTROL):
			if self.interface:
				self.interface.OpenChestDropWindow(ItemVNum, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)
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
				self.questionDialog.SetText(localeInfo.INVENTORY_REALLY_USE_ITEM % item.GetItemName())

			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnAccept))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnCancel))
			self.questionDialog.Open()
			self.questionDialog.slotIndex = slotIndex

			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
		elif app.ENABLE_SHOW_CHEST_DROP and\
			(app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL))\
			and item.GetItemType() == item.ITEM_TYPE_GIFTBOX:
			if app.IsPressed(app.DIK_Z):
				net.SendChatPacket("/chest_drop %d %d %d" % (self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex, player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)))
			elif self.interface:
				self.interface.OpenChestDropWindow(ItemVNum, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)
		else:
			if app.ENABLE_NEW_PET_SYSTEM and item.GetItemType() == item.ITEM_TYPE_PET and item.GetItemSubType() == item.PET_EGG:
				self.interface.OpenChangeNameWindow(slotIndex,slotIndex,ItemVNum, 2)
			else:
				self.__SendUseItemPacket(slotIndex)
			#net.SendItemUsePacket(slotIndex)

	def __UseItemQuestionDialog_OnCancel(self):
		self.OnCloseQuestionDialog()

	def __UseItemQuestionDialog_OnAccept(self):
		self.__SendUseItemPacket(self.questionDialog.slotIndex)
		self.OnCloseQuestionDialog()

	def __UseItemToItemQuestionDialog_OnAccept(self):
		self.__SendUseItemToItemPacket(player.INVENTORY, self.questionDialog.srcItemSlotPos, self.questionDialog.dstItemSlotPos)
		self.OnCloseQuestionDialog()

	def __SendUseItemToItemPacket(self, srcInv, srcSlotPos, dstSlotPos, srcItemVID = -1):		
		net.SendItemUseToItemPacket(srcInv, srcSlotPos, player.INVENTORY, dstSlotPos)

	def __SendUseItemPacket(self, slotPos):
		net.SendItemUsePacket(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],slotPos)

	def __SendMoveItemPacket(self, srcSlotWindow, srcSlotPos, dstSlotWindow, dstSlotPos, srcItemCount):
		net.SendItemMovePacket(srcSlotWindow , srcSlotPos, dstSlotWindow, dstSlotPos, srcItemCount)

	if app.ENABLE_SELL_ITEM:
		def IsSellItems(self, slotIndex):
			itemVnum = player.GetItemIndex(slotIndex)
			item.SelectItem(itemVnum)
			itemPrice = item.GetISellItemPrice()

			if itemPrice > 1:
				return True
				
			return False
			
		def __SendSellItemPacket(self, itemVNum):
			if uiPrivateShopBuilder.IsBuildingPrivateShop():
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_PRIVATE_SHOP)
				return
				
			net.SendItemSellPacket(itemVNum)

	def SetDragonSoulRefineWindow(self, wndDragonSoulRefine):
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoulRefine = wndDragonSoulRefine

	def ActivateAllAffectItems(self):
		
		activeList = list()
		notActiveList = list()
		for slotNumber in xrange((player.INVENTORY_PAGE_SIZE*player.INVENTORY_PAGE_COUNT)):
			itemVnum = player.GetItemIndex(slotNumber)
			isActivated = (player.GetItemMetinSocket(slotNumber, item.BOOST_POTION_STATUS) != 0) if not constInfo.IsEnergyCrystalItem(itemVnum) else 0 != player.GetItemMetinSocket(slotNumber, 0)

			if not constInfo.IsConsumableItem(itemVnum):
				continue

			if isActivated:
				activeList.append(slotNumber)
			else:
				notActiveList.append(slotNumber)

		if len(activeList) >= len(notActiveList):
			for i in activeList:
				self.__UseItem(i)
		else:
			if not activeList:
				for i in notActiveList:
					self.__UseItem(i)
			else:
				if notActiveList:
					for i in notActiveList:
						self.__UseItem(i)
				else:
					for i in activeList:
						self.__UseItem(i)


	def OnMoveWindow(self, x, y):
		if app.__RENEWAL_BRAVE_CAPE__:
			if self.wndBraveCape:
				self.wndBraveCape.AdjustPosition(x, y)

	if app.__BL_BATTLE_ROYALE__:
		def __CreateBattleRoyaleExtendInvenButton(self):
			parent = self.GetChild("board")
		
			self.BattleRoyaleExInvenButton = []
			start_x			= 8
			start_y			= 246 + 25
			img_height		= 32
			count			= 9
			EX_INVEN_IMAGE	= "d:/ymir work/ui/game/windows/ex_inven_cover_button_close.sub"
		
			for button_index in range(count * player.INVENTORY_PAGE_COUNT):
			
				ex_inven_button = ui.Button()
				
				increase_y	= img_height * (button_index % count)
				
				ex_inven_button.SetParent(parent)
				ex_inven_button.SetPosition(start_x, start_y + increase_y )
				ex_inven_button.SetUpVisual(EX_INVEN_IMAGE)
				ex_inven_button.SetOverVisual(EX_INVEN_IMAGE)
				ex_inven_button.SetDownVisual(EX_INVEN_IMAGE)
				ex_inven_button.SetDisableVisual(EX_INVEN_IMAGE)
				ex_inven_button.Hide()
				self.BattleRoyaleExInvenButton.append(ex_inven_button)
			
			for i in range(1, len(self.inventoryTab)):
				self.inventoryTab[i].Hide()

		def __HideAllBattleRoyaleExtendInvenButton(self):
			
			for index in range( len(self.BattleRoyaleExInvenButton) ):
				self.BattleRoyaleExInvenButton[index].Hide()
				
		def __ShowBattleRoyaleExtendInvenButton(self, cur_stage):
			count = 9
			min_range = self.inventoryPageIndex * count
			max_range = min_range + count
			
			for button_index in range(min_range, max_range):
				if button_index < cur_stage:
					self.BattleRoyaleExInvenButton[button_index].Hide()
				else:
					self.BattleRoyaleExInvenButton[button_index].Show()
		
		def __RefreshBattleRoyaleExinvenCoverSlot(self):
			self.__HideAllBattleRoyaleExtendInvenButton()
			self.__ShowBattleRoyaleExtendInvenButton(battleRoyaleMgr.BATTLE_ROYALE_INVENTORY_EXTEND_BUTTON_MIN_RANGE)
			
		def __RefreshBattleRoyaleExinvenSlotState(self):
			inventory_max = battleRoyaleMgr.BATTLE_ROYALE_INVENTORY_EXTEND_BUTTON_MIN_RANGE * 5

			for i in xrange(player.INVENTORY_PAGE_SIZE):
			
				slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(i)
				
				if slotNumber >= inventory_max:
					self.wndItem.LockSlot(i)
				else:
					self.wndItem.UnlockSlot(i)

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
			
	def BindPrivateShopSearchWindow(self, wndPrivateShopSearch):
		self.wndPrivateShopSearch = wndPrivateShopSearch
		
	def BindWikiWindow(self, wndWiki):
		self.wndWiki = wndWiki
		
	def __GetCurrentItemGrid(self):
		itemGrid = [[False for slot in xrange(player.INVENTORY_PAGE_SIZE)] for page in xrange(player.INVENTORY_PAGE_COUNT)]

		for page in xrange(player.INVENTORY_PAGE_COUNT):
			for slot in xrange(player.INVENTORY_PAGE_SIZE):
				itemVnum = player.GetItemIndex(slot + page * player.INVENTORY_PAGE_SIZE)
				if itemVnum <> 0:
					(w, h) = item.GetItemSize(item.SelectItem(itemVnum))
					for i in xrange(h):
						itemGrid[page][slot + i * 5] = True

		return itemGrid

	def __FindEmptyCellForSize(self, itemGrid, size):
		for page in xrange(player.INVENTORY_PAGE_COUNT):
			for slot in xrange(player.INVENTORY_PAGE_SIZE):
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
						return slot + page * player.INVENTORY_PAGE_SIZE

		return -1

	def AttachItemFromSwitchbot(self, slotIndex, itemIndex):
		itemGrid = self.__GetCurrentItemGrid()
		item.SelectItem(itemIndex)
		
		if item.GetItemType() == item.ITEM_TYPE_DS:
			return
			
		emptySlotIndex = self.__FindEmptyCellForSize(itemGrid, item.GetItemSize()[1])
		
		if emptySlotIndex <> -1:
			net.SendItemMovePacket(player.SWITCHBOT, slotIndex, player.INVENTORY, emptySlotIndex, 1)
			
		return True

	if app.ENABLE_AURA_SYSTEM:
		def __UseItemAuraQuestionDialog_OnAccept(self):
			self.questionDialog.Close()
			net.SendAuraRefineCheckIn(*(self.questionDialog.srcItem + self.questionDialog.dstItem + (player.GetAuraRefineWindowType(),)))
			self.questionDialog.srcItem = (0, 0)
			self.questionDialog.dstItem = (0, 0)

		def __UseItemAuraQuestionDialog_Close(self):
			self.questionDialog.Close()

			self.questionDialog.srcItem = (0, 0)
			self.questionDialog.dstItem = (0, 0)

		def __UseItemAura(self, slotIndex):
			AuraSlot = player.FineMoveAuraItemSlot()
			UsingAuraSlot = player.FindActivatedAuraSlot(player.INVENTORY, slotIndex)
			AuraVnum = player.GetItemIndex(slotIndex)
			item.SelectItem(AuraVnum)
			if player.GetAuraCurrentItemSlotCount() >= player.AURA_SLOT_MAX <= UsingAuraSlot:
				return

			if player.IsEquipmentSlot(slotIndex):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_IMPOSSIBLE_EQUIPITEM)
				return

			if player.GetAuraRefineWindowType() == player.AURA_WINDOW_TYPE_ABSORB:
				isAbsorbItem = False
				if item.GetItemType() == item.ITEM_TYPE_COSTUME:
					if item.GetItemSubType() == item.COSTUME_TYPE_AURA:
						if player.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM) == 0:
							if UsingAuraSlot == player.AURA_SLOT_MAX:
								if AuraSlot != player.AURA_SLOT_MAIN:
									return

								net.SendAuraRefineCheckIn(player.INVENTORY, slotIndex, player.AURA_REFINE, AuraSlot, player.GetAuraRefineWindowType())

						else:
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_IMPOSSIBLE_ABSORBITEM)
							return

					else:
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_IMPOSSIBLE_ABSORBITEM)
						return

				elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
					if item.GetItemSubType() in [item.ARMOR_SHIELD, item.ARMOR_WRIST, item.ARMOR_NECK, item.ARMOR_EAR]:
						if player.FindUsingAuraSlot(player.AURA_SLOT_MAIN) == player.NPOS():
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_POSSIBLE_REGISTER_AURAITEM)
							return

						isAbsorbItem = True
					else:
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_IMPOSSIBLE_ABSORBITEM)
						return

				else:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_IMPOSSIBLE_ABSORBITEM)
					return

				if isAbsorbItem:
					if UsingAuraSlot == player.AURA_SLOT_MAX:
						if AuraSlot != player.AURA_SLOT_SUB:
							if player.FindUsingAuraSlot(player.AURA_SLOT_SUB) == player.NPOS():
								AuraSlot = player.AURA_SLOT_SUB
							else:
								return

						self.questionDialog = uiCommon.QuestionDialog()
						self.questionDialog.SetText(localeInfo.AURA_NOTICE_DEL_ABSORDITEM)
						self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.__UseItemAuraQuestionDialog_OnAccept))
						self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__UseItemAuraQuestionDialog_Close))
						self.questionDialog.srcItem = (player.INVENTORY, slotIndex)
						self.questionDialog.dstItem = (player.AURA_REFINE, AuraSlot)
						self.questionDialog.Open()

			elif player.GetAuraRefineWindowType() == player.AURA_WINDOW_TYPE_GROWTH:
				if UsingAuraSlot == player.AURA_SLOT_MAX:
					if AuraSlot == player.AURA_SLOT_MAIN:
						if item.GetItemType() == item.ITEM_TYPE_COSTUME:
							if item.GetItemSubType() == item.COSTUME_TYPE_AURA:
								socketLevelValue = player.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
								curLevel = (socketLevelValue / 100000) - 1000
								curExp = socketLevelValue % 100000;
								if curLevel >= player.AURA_MAX_LEVEL:
									chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_MAX_LEVEL)
									return

								if curExp >= player.GetAuraRefineInfo(curLevel, player.AURA_REFINE_INFO_NEED_EXP):
									chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_IMPOSSIBLE_GROWTHITEM)
									return

								net.SendAuraRefineCheckIn(player.INVENTORY, slotIndex, player.AURA_REFINE, AuraSlot, player.GetAuraRefineWindowType())

							else:
								chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_POSSIBLE_AURAITEM)
								return

						else:
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_POSSIBLE_AURAITEM)
							return

					elif AuraSlot == player.AURA_SLOT_SUB:
						if player.FindUsingAuraSlot(player.AURA_SLOT_MAIN) != player.NPOS():
							if item.GetItemType() == item.ITEM_TYPE_RESOURCE:
								if item.GetItemSubType() == item.RESOURCE_AURA:
									if UsingAuraSlot == player.AURA_SLOT_MAX:
										if AuraSlot != player.AURA_SLOT_SUB:
											return

										net.SendAuraRefineCheckIn(player.INVENTORY, slotIndex, player.AURA_REFINE, AuraSlot, player.GetAuraRefineWindowType())
								else:
									chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_POSSIBLE_AURARESOURCE)
									return

							else:
								chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_POSSIBLE_AURARESOURCE)
								return

						else:
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_POSSIBLE_REGISTER_AURAITEM)
							return

			elif player.GetAuraRefineWindowType() == player.AURA_WINDOW_TYPE_EVOLVE:
				if UsingAuraSlot == player.AURA_SLOT_MAX:
					if AuraSlot == player.AURA_SLOT_MAIN:
						if item.GetItemType() == item.ITEM_TYPE_COSTUME:
							if item.GetItemSubType() == item.COSTUME_TYPE_AURA:
								socketLevelValue = player.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
								curLevel = (socketLevelValue / 100000) - 1000
								curExp = socketLevelValue % 100000;
								if curLevel >= player.AURA_MAX_LEVEL:
									chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_MAX_LEVEL)
									return

								if curLevel != player.GetAuraRefineInfo(curLevel, player.AURA_REFINE_INFO_LEVEL_MAX) or curExp < player.GetAuraRefineInfo(curLevel, player.AURA_REFINE_INFO_NEED_EXP):
									chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_IMPOSSIBLE_EVOLUTION_ITEM)
									return

								if player.FindUsingAuraSlot(AuraSlot) != player.NPOS():
									return

								net.SendAuraRefineCheckIn(player.INVENTORY, slotIndex, player.AURA_REFINE, AuraSlot, player.GetAuraRefineWindowType())

							else:
								chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_POSSIBLE_AURAITEM)
								return

						else:
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_POSSIBLE_AURAITEM)
							return

					elif AuraSlot == player.AURA_SLOT_SUB:
						Cell = player.FindUsingAuraSlot(player.AURA_SLOT_MAIN)
						if Cell == player.NPOS():
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_POSSIBLE_REGISTER_AURAITEM)
							return

						socketLevelValue = player.GetItemMetinSocket(*(Cell + (player.ITEM_SOCKET_AURA_CURRENT_LEVEL,)))
						curLevel = (socketLevelValue / 100000) - 1000
						curExp = socketLevelValue % 100000;
						if curLevel >= player.AURA_MAX_LEVEL:
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_MAX_LEVEL)
							return

						if curExp < player.GetAuraRefineInfo(curLevel, player.AURA_REFINE_INFO_NEED_EXP):
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_IMPOSSIBLE_EVOLUTION_ITEM)
							return

						if AuraVnum != player.GetAuraRefineInfo(curLevel, player.AURA_REFINE_INFO_MATERIAL_VNUM):
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_IMPOSSIBLE_EVOLUTION_ITEM)
							return

						if player.GetItemCount(slotIndex) < player.GetAuraRefineInfo(curLevel, player.AURA_REFINE_INFO_MATERIAL_COUNT):
							chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.AURA_IMPOSSIBLE_EVOLUTION_ITEMCOUNT)
							return

						if UsingAuraSlot == player.AURA_SLOT_MAX:
							if AuraSlot != player.AURA_SLOT_MAX:
								if player.FindUsingAuraSlot(AuraSlot) != player.NPOS():
									return

							net.SendAuraRefineCheckIn(player.INVENTORY, slotIndex, player.AURA_REFINE, AuraSlot, player.GetAuraRefineWindowType())

	if app.ENABLE_IKASHOP_RENEWAL and app.EXTEND_IKASHOP_ULTIMATE and app.USE_IKASHOP_UI_RENEWED:
		def SetShopNotificationCounter(self, count):
			if self.notificationCounter:
				self.notificationCounter.SetCounter(count)

if app.__RENEWAL_BRAVE_CAPE__:
	import os
	class BraveCapeWindow(ui.Board):
		__children={}
		class SliderBarNew(ui.Window):
			def __init__(self):
				ui.Window.__init__(self)
				self.curPos = 1.0
				self.pageSize = 1.0
				self.eventChange = None
				self.__Load()

			def __Load(self):
				IMG_DIR = "d:/ymir work/ui/game/bravery_cape/"

				img = ui.ImageBox()
				img.SetParent(self)
				img.LoadImage(IMG_DIR+"slider_bg.tga")
				img.Show()
				self.backGroundImage = img

				self.SetSize(self.backGroundImage.GetWidth(), self.backGroundImage.GetHeight())

				cursor = ui.DragButton()
				cursor.AddFlag("movable")
				cursor.AddFlag("restrict_y")
				cursor.SetParent(self)
				cursor.SetMoveEvent(ui.__mem_func__(self.__OnMove))
				cursor.SetUpVisual(IMG_DIR+"drag.tga")
				cursor.SetOverVisual(IMG_DIR+"drag.tga")
				cursor.SetDownVisual(IMG_DIR+"drag.tga")
				cursor.Show()
				self.cursor = cursor

				##
				self.cursor.SetRestrictMovementArea(0, 0, self.backGroundImage.GetWidth(), 0)
				self.pageSize = self.backGroundImage.GetWidth() - self.cursor.GetWidth()

			def __OnMove(self):
				(xLocal, yLocal) = self.cursor.GetLocalPosition()
				self.curPos = float(xLocal) / float(self.pageSize)
				if self.eventChange:
					self.eventChange()
			def SetSliderPos(self, pos):
				self.curPos = pos
				self.cursor.SetPosition(int(self.pageSize * pos), 0)
			def GetSliderPos(self):
				return self.curPos
			def SetEvent(self, event):
				self.eventChange = event
			def Enable(self):
				self.cursor.Show()
			def Disable(self):
				self.cursor.Hide()

		def Destroy(self):
			self.SaveData()
			self.__children={}
		def CreateWindow(self, classPtr, parent, pos):
			window = classPtr
			window.SetParent(parent)
			window.SetPosition(*pos)
			window.Show()
			return window
		def __OverOutItem(self):
			interface = constInfo.GetInterfaceInstance()
			if interface:
				if interface.tooltipItem:
					interface.tooltipItem.HideToolTip()
		def __OverInItem(self, itemIdx):
			interface = constInfo.GetInterfaceInstance()
			if interface:
				if interface.tooltipItem:
					interface.tooltipItem.SetItemToolTip(itemIdx)
		def __init__(self):
			ui.Board.__init__(self)
			self.SetSize(140, 130 + 44)
			self.AddFlag("attach")
			self.AddFlag("float")

			self.__children["firstOpened"] = app.GetGlobalTimeStamp() + 5

			IMG_DIR = "d:/ymir work/ui/game/bravery_cape/"

			BRAVE_CAPE_ITEM_IDX = 20574

			item.SelectItem(BRAVE_CAPE_ITEM_IDX)

			bgImg = self.CreateWindow(ui.ImageBox(), self, (5, 6))
			bgImg.LoadImage(IMG_DIR+"bg.tga")
			self.__children["bgImg"] = bgImg

			timeTextVisual = self.CreateWindow(ui.TextLine(), bgImg, (13, 60))
			timeTextVisual.SetText("Second")
			self.__children["timeTextVisual"] = timeTextVisual

			timeSlider = self.CreateWindow(self.SliderBarNew(), bgImg, (13, 73 + 5))
			timeSlider.SetEvent(ui.__mem_func__(self.OnChangeTimeSlider))
			self.__children["timeSlider"] = timeSlider

			timeBg = self.CreateWindow(ui.ImageBox(), bgImg, (77, 64))
			timeBg.LoadImage(IMG_DIR+"input_output.tga")
			self.__children["timeBg"] = timeBg

			timeText = self.CreateWindow(ui.TextLine(), timeBg, (0, 0))
			timeText.SetAllAlign()
			timeText.SetText("0")
			self.__children["timeText"] = timeText

			rangeTextVisual = self.CreateWindow(ui.TextLine(), bgImg, (13, 73 + 22 + 17 + 5 - 18))
			rangeTextVisual.SetText("Range")
			self.__children["rangeTextVisual"] = rangeTextVisual

			rangeSlider = self.CreateWindow(self.SliderBarNew(), bgImg, (13, 73 + 22 + 17 + 5))
			rangeSlider.SetEvent(ui.__mem_func__(self.OnChangeRangeSlider))
			self.__children["rangeSlider"] = rangeSlider

			rangeBg = self.CreateWindow(ui.ImageBox(), bgImg, (77, 95 + 8))
			rangeBg.LoadImage(IMG_DIR+"input_output.tga")
			self.__children["rangeBg"] = rangeBg

			rangeText = self.CreateWindow(ui.TextLine(), rangeBg, (0, 0))
			rangeText.SetAllAlign()
			rangeText.SetText("0")
			self.__children["rangeText"] = rangeText

			itemIcon = self.CreateWindow(ui.ImageBox(), bgImg, (50, 13))
			itemIcon.LoadImage(item.GetIconImageFileName())
			itemIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.__OverOutItem)
			itemIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.__OverInItem, BRAVE_CAPE_ITEM_IDX)
			self.__children["itemIcon"] = itemIcon

			startBtn = self.CreateWindow(ui.Button(), bgImg, (6, 95 + 39))
			startBtn.SetUpVisual(IMG_DIR+"start_btn_0.tga")
			startBtn.SetOverVisual(IMG_DIR+"start_btn_1.tga")
			startBtn.SetDownVisual(IMG_DIR+"start_btn_2.tga")
			startBtn.SetDisableVisual(IMG_DIR+"start_btn_2.tga")
			startBtn.SAFE_SetEvent(self.__ClickStatusBtn, "ACTIVE")
			startBtn.SetText("Start")
			self.__children["startBtn"] = startBtn

			stopBtn = self.CreateWindow(ui.Button(), bgImg, (66, 95 + 39))
			stopBtn.SetUpVisual(IMG_DIR+"start_btn_0.tga")
			stopBtn.SetOverVisual(IMG_DIR+"start_btn_1.tga")
			stopBtn.SetDownVisual(IMG_DIR+"start_btn_2.tga")
			stopBtn.SetDisableVisual(IMG_DIR+"start_btn_2.tga")
			stopBtn.SAFE_SetEvent(self.__ClickStatusBtn, "DEACTIVE")
			stopBtn.SetText("Stop")
			self.__children["stopBtn"] = stopBtn

			expandBtn = self.CreateWindow(ui.Button(), self, (0, 10))
			expandBtn.SetUpVisual("d:/ymir work/ui/game/belt_inventory/btn_minimize_normal.tga")
			expandBtn.SetOverVisual("d:/ymir work/ui/game/belt_inventory/btn_minimize_over.tga")
			expandBtn.SetDownVisual("d:/ymir work/ui/game/belt_inventory/btn_minimize_down.tga")
			expandBtn.SAFE_SetEvent(self.Close)
			self.__children["expandBtn"] = expandBtn

			self.__children["second"] = 0
			self.__children["range"] = 0
			self.__children["status"] = False
			self.Refresh()

		def Refresh(self):
			(second, range, posTime, posSlider) = (self.__children["second"], self.__children["range"], 0.0, 0.0)
			if second > 5800:
				second = 5800
			if range > 8000:
				range = 8000

			self.__children["timeText"].SetText(str((second/100)+2))
			self.__children["rangeText"].SetText(str(range+1000))

			self.__children["timeSlider"].SetSliderPos((1.0/5800.0)*second)
			self.__children["rangeSlider"].SetSliderPos((1.0/4000.0)*range)

			self.__children["second"] = second
			self.__children["range"] = range

		def Open(self):
			interface = constInfo.GetInterfaceInstance()
			if interface:
				if interface.wndInventory:
					interface.wndInventory.disbandBtn.Hide()
			self.Show()

		def Close(self):
			interface = constInfo.GetInterfaceInstance()
			if interface:
				if interface.wndInventory:
					interface.wndInventory.disbandBtn.Show()
			self.Hide()

		def __ClickStatusBtn(self, type):
			if type == "ACTIVE":
				net.SendChatPacket("/brave_cape active {} {}".format((self.__children["second"]/100)+2, self.__children["range"] + 1000))
			elif type == "DEACTIVE":
				net.SendChatPacket("/brave_cape deactive")

		def AdjustPosition(self, x, y):
			self.SetPosition(x + 10 + 30 - self.GetWidth(), y + 220)
		def OnChangeRangeSlider(self):
			val = int(((1.0/4000.0)*(self.__children["rangeSlider"].GetSliderPos()*14000))*4000)
			self.__children["range"] = val
			self.Refresh()
		def OnChangeTimeSlider(self):
			val = int(((1.0/5800.0)*(self.__children["timeSlider"].GetSliderPos()*5800))*5800)
			self.__children["second"] = val
			self.Refresh()
		def SetStatus(self, status):
			self.__children["status"] = True if int(status) == 1 else False
			if self.__children["status"]:
				self.__children["startBtn"].Disable()
				self.__children["stopBtn"].Enable()
			else:
				self.__children["stopBtn"].Disable()
				self.__children["startBtn"].Enable()
			self.Refresh()
		def SaveData(self):
			try:
				file = open("lib/{}_brave_cape".format(player.GetName()), "w+")
				file.write("{}#{}#{}\n".format(1 if (self.__children["status"] if self.__children.has_key("status") else False) == True else 0, self.__children["second"], self.__children["range"]))
				file.close()
			except:
				pass
		def LoadData(self):
			try:
				splitList = open("lib/{}_brave_cape".format(player.GetName()), "r").readlines()[0].split("#")
				self.__children["status"] = True if int(splitList[0]) == 1 else False
				self.__children["second"] = int(splitList[1])
				self.__children["range"] = int(splitList[2])
				self.Refresh()
				if self.__children["status"]:
					self.__ClickStatusBtn("ACTIVE")
				os.remove("lib/{}_brave_cape".format(player.GetName()))
			except:
				pass
