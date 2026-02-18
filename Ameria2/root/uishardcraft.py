# coding: utf-8

import net
import player
import wndMgr
import item

import ui
import mouseModule
import constInfo
import uiToolTip
import localeInfo

LIMIT_RANGE = 1000

class CraftWindow(ui.ScriptWindow):
	def Add(self, sourceSlot, targetSlot):
		itemVnum = player.GetItemIndex(sourceSlot)
		if not itemVnum:
			self.Close()
			return False
		elif not constInfo.SHARD_CRAFT_SLOTS_DICT.has_key(targetSlot):
			self.Close()
			return False

		item.SelectItem(itemVnum)

		craftPrice = item.GetValue(item.CRYSTAL_ADD_BONUS_UPGRADE_PRICE)
		self.resultItemVnum = item.GetValue(item.CRYSTAL_ADD_BONUS_UPGRADE_RESULT_VNUM) if targetSlot == player.SHARD_CRAFT_SLOT_MATERIAL else 0

		constInfo.SHARD_CRAFT_SLOTS_DICT[targetSlot] = sourceSlot

		itemCount = player.GetItemCount(sourceSlot)

		if itemCount == 1:
			itemCount = 0

		self.slotsUI.SetItemSlot(targetSlot, itemVnum, itemCount)

		if targetSlot == player.SHARD_CRAFT_SLOT_MAIN:
			self.textPrice.SetText(localeInfo.UI_SHARD_CRAFT_PRICE % (localeInfo.NumberToMoneyString(craftPrice)))
		elif targetSlot == player.SHARD_CRAFT_SLOT_MATERIAL and self.resultItemVnum > 0:
			self.slotsUI.SetItemSlot(player.SHARD_CRAFT_SLOT_RESULT, self.resultItemVnum, 0)

		return True

	def InvAdd(self, sourceSlot):
		if not self.IsShow():
			return False

		for k, v in constInfo.SHARD_CRAFT_SLOTS_DICT.items():
			if v == -1:
				net.SendShardCraft(net.SUBHEADER_SHARD_CRAFT_ADD,\
									sourceSlot,\
									k)

		return True

	def Out(self, targetSlot):
		if not constInfo.SHARD_CRAFT_SLOTS_DICT.has_key(targetSlot):
			return

		constInfo.SHARD_CRAFT_SLOTS_DICT[targetSlot] = -1
		self.slotsUI.ClearSlot(targetSlot)
		self.slotsUI.ClearSlot(player.SHARD_CRAFT_SLOT_RESULT)

	def OutAll(self):
		self.Clear()

	def Show(self):
		self.SetCenterPosition()
		self.SetTop()

		wndMgr.Show(self.hWnd)

	def Clear(self):
		self.resultItemVnum = -1

		self.textPrice.SetText(localeInfo.UI_SHARD_CRAFT_PRICE % (localeInfo.NumberToMoneyString(0)))

		for k, v in constInfo.SHARD_CRAFT_SLOTS_DICT.items():
			constInfo.SHARD_CRAFT_SLOTS_DICT[k] = -1

		for i in xrange(self.slotsUI.GetSlotCount()):
			self.slotsUI.ClearSlot(i)

	def ShowRecv(self):
		self.Clear()

		(self.iPosX, self.iPosY, z) = player.GetMainCharacterPosition()

		self.Show()

	def Hide(self):
		self.OverOutSlot()

		wndMgr.Hide(self.hWnd)

	def HideRecv(self):
		self.Clear()

		self.Hide()

	def Close(self):
		net.SendShardCraft(net.SUBHEADER_SHARD_CRAFT_CLOSE)

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def Craft(self):
		net.SendShardCraft(net.SUBHEADER_SHARD_CRAFT_REFINE)

	def OnUpdate(self):
		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.iPosX) > LIMIT_RANGE or abs(y - self.iPosY) > LIMIT_RANGE:
			(self.iPosX, self.iPosY, z) = player.GetMainCharacterPosition()
			self.Close()

	def SelectEmptySlot(self, selectedSlotPos):
		if not mouseModule.mouseController.isAttached():
			return

		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)

		mouseModule.mouseController.DeattachObject()

		net.SendShardCraft(net.SUBHEADER_SHARD_CRAFT_ADD,\
							attachedSlotPos,\
							selectedSlotPos)

	def SelectItemSlot(self, selectedSlotPos):
		if mouseModule.mouseController.isAttached():
			mouseModule.mouseController.DeattachObject()

		if not constInfo.SHARD_CRAFT_SLOTS_DICT.has_key(selectedSlotPos):
			return

		net.SendShardCraft(net.SUBHEADER_SHARD_CRAFT_OUT,\
							selectedSlotPos)

	def OverInSlot(self, selectedSlotPos):
		if not constInfo.SHARD_CRAFT_SLOTS_DICT.has_key(selectedSlotPos) and\
			selectedSlotPos != player.SHARD_CRAFT_SLOT_RESULT:
			return

		if selectedSlotPos == player.SHARD_CRAFT_SLOT_RESULT and\
			self.resultItemVnum == -1:
			return

		if not self.toolTipItem:
			self.toolTipItem = uiToolTip.ItemToolTip()

		if selectedSlotPos == player.SHARD_CRAFT_SLOT_RESULT:
			self.toolTipItem.SetItemToolTip(self.resultItemVnum)
		elif constInfo.SHARD_CRAFT_SLOTS_DICT[selectedSlotPos] != -1:
			self.toolTipItem.SetInventoryItem(constInfo.SHARD_CRAFT_SLOTS_DICT[selectedSlotPos])

	def OverOutSlot(self):
		if self.toolTipItem:
			self.toolTipItem.HideToolTip()

	def __Build(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/shard_craftwindow.py")
		except:
			import exception
			exception.Abort("Shard_CraftWindow.Script.Load -> 1")

		try:
			self.titleBar = self.GetChild("titlebar_ui")

			self.slotsUI = self.GetChild("slot_ui_materials")

			self.craftButton = self.GetChild("button_ui_craft")
			self.closeButton = self.GetChild("button_ui_close")

			self.textPrice = self.GetChild("text_ui_price")
		except:
			import exception
			exception.Abort("Shard_CraftWindow.Script.Load -> 2")

		try:
			self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

			self.slotsUI.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))

			self.slotsUI.SetUnselectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
			self.slotsUI.SetUseSlotEvent(ui.__mem_func__(self.SelectItemSlot))

			self.slotsUI.SetOverInItemEvent(ui.__mem_func__(self.OverInSlot))
			self.slotsUI.SetOverOutItemEvent(ui.__mem_func__(self.OverOutSlot))

			self.craftButton.SetEvent(ui.__mem_func__(self.Craft))
			self.closeButton.SetEvent(ui.__mem_func__(self.Close))
		except:
			import exception
			exception.Abort("Shard_CraftWindow.Script.Bind")

	def __init__(self):
		self.toolTipItem = None

		ui.ScriptWindow.__init__(self)

		self.iPosX = -1
		self.iPosY = -1

		self.resultItemVnum = -1

		self.__Build()

	def Destroy(self):
		self.Clear()

		if self.toolTipItem:
			self.toolTipItem.ClearToolTip()
			del self.toolTipItem
			self.toolTipItem = None

		self.titleBar.SetCloseEvent(None)
		self.titleBar = None

		self.slotsUI.SetSelectEmptySlotEvent(None)

		self.slotsUI.SetUnselectItemSlotEvent(None)
		self.slotsUI.SetUseSlotEvent(None)

		self.slotsUI.SetOverInItemEvent(None)
		self.slotsUI.SetOverOutItemEvent(None)

		self.slotsUI = None

		self.craftButton.SetEvent(None)
		self.craftButton = None

		self.closeButton.SetEvent(None)
		self.closeButton = None

		self.textPrice = None

		self.ClearDictionary()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
