# coding: utf-8

import app
import chat
import player
import exchange
import net
import item
import wndMgr

import ui
import localeInfo
import mouseModule
import uiPickMoney
import constInfo
import uiToolTip

if app.ENABLE_NEW_EXCHANGE_WINDOW:
	import grp
	import time
	import uiCommon
	import playerSettingModule

###################################################################################################
## Exchange
class ExchangeDialog(ui.ScriptWindow):
	if app.ENABLE_NEW_EXCHANGE_WINDOW:
		FACE_IMAGE_DICT = {
							playerSettingModule.RACE_WARRIOR_M : "icon/face/warrior_m.tga",
							playerSettingModule.RACE_WARRIOR_W : "icon/face/warrior_w.tga",
							playerSettingModule.RACE_ASSASSIN_M : "icon/face/assassin_m.tga",
							playerSettingModule.RACE_ASSASSIN_W : "icon/face/assassin_w.tga",
							playerSettingModule.RACE_SURA_M : "icon/face/sura_m.tga",
							playerSettingModule.RACE_SURA_W : "icon/face/sura_w.tga",
							playerSettingModule.RACE_SHAMAN_M : "icon/face/shaman_m.tga",
							playerSettingModule.RACE_SHAMAN_W : "icon/face/shaman_w.tga",
		}

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.toolTipDesc = None
		self.dlgPickMoney = None

		if app.USE_CHEQUE_CURRENCY:
			self.dlgPickCheque = None

		self.TitleName = 0
		self.tooltipItem = 0
		self.xStart = 0
		self.yStart = 0

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.interface = 0
			self.wndInventory = 0
			self.lockedItems = {i:(-1,-1) for i in range(exchange.EXCHANGE_ITEM_MAX_NUM)}
			self.wndInventoryNew = 0
			self.lockedItemsSpecial = {i:(-1,-1,-1) for i in range(exchange.EXCHANGE_ITEM_MAX_NUM)}

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	if app.ENABLE_NEW_EXCHANGE_WINDOW:
		class Item(ui.ListBoxEx.Item):
			def __init__(self,parent, text, value=0):
				ui.ListBoxEx.Item.__init__(self)
				self.textBox=ui.TextLine()
				self.textBox.SetParent(self)
				self.textBox.SetText(text)
				self.textBox.Show()
				self.value = value

			def GetValue(self):
				return self.value

			def __del__(self):
				ui.ListBoxEx.Item.__del__(self)

	def OnOverIn(self, num):
		if not self.toolTipDesc:
			self.toolTipDesc = uiToolTip.ToolTip()

		self.toolTipDesc.ClearToolTip()

		if num == 0:
			self.toolTipDesc.AutoAppendTextLineSpecial(localeInfo.TOOLTIP_CURRENCY_YANG, 0xFFFFEB3B)
			self.toolTipDesc.ShowToolTip()
		elif app.USE_CHEQUE_CURRENCY and num == 1:
			self.toolTipDesc.AutoAppendTextLineSpecial(localeInfo.TOOLTIP_CURRENCY_WON, 0xFF2596BE)
			self.toolTipDesc.ShowToolTip()

	def OnOverOut(self):
		if self.toolTipDesc:
			self.toolTipDesc.HideToolTip()

	def LoadDialog(self):
		PythonScriptLoader = ui.PythonScriptLoader()
		if app.ENABLE_NEW_EXCHANGE_WINDOW:
			PythonScriptLoader.LoadScriptFile(self, "uiscript/exchangedialog_new.py")
		else:
			PythonScriptLoader.LoadScriptFile(self, "uiscript/exchangedialog.py")

		## Owner
		self.OwnerSlot = self.GetChild("Owner_Slot")
		self.OwnerSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectOwnerEmptySlot))
		self.OwnerSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectOwnerItemSlot))
		self.OwnerSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInOwnerItem))
		self.OwnerSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		if not app.ENABLE_NEW_EXCHANGE_WINDOW:
			self.OwnerAcceptLight = self.GetChild("Owner_Accept_Light")
			self.OwnerAcceptLight.Disable()

		self.ownerMoneyText = self.GetChild("owner_money_text_ui")
		self.ownerMoneyButton = self.GetChild("owner_money_slot_ui")
		self.ownerMoneyButton.SetEvent(ui.__mem_func__(self.OpenPickMoneyDialog), 0)

		self.ownerMoneyIcon = self.GetChild("owner_money_icon_ui")
		self.ownerMoneyIcon.SetMouseOverInEvent(ui.__mem_func__(self.OnOverIn), 0)
		self.ownerMoneyIcon.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverOut))
		self.ownerMoneyIcon.SetEvent(ui.__mem_func__(self.OpenPickMoneyDialog), 0)

		if app.USE_CHEQUE_CURRENCY:
			self.ownerChequeText = self.GetChild("owner_cheque_text_ui")

			self.ownerChequeButton = self.GetChild("owner_cheque_slot_ui")
			self.ownerChequeButton.SetEvent(ui.__mem_func__(self.OpenPickMoneyDialog), 1)

			self.ownerChequeIcon = self.GetChild("owner_cheque_icon_ui")
			self.ownerChequeIcon.SetMouseOverInEvent(ui.__mem_func__(self.OnOverIn), 1)
			self.ownerChequeIcon.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverOut))
			self.ownerChequeIcon.SetEvent(ui.__mem_func__(self.OpenPickMoneyDialog), 1)
		else:
			self.GetChild("owner_cheque_icon_ui").Hide()
			self.GetChild("owner_cheque_slot_ui").Hide()

		## Target
		self.TargetSlot = self.GetChild("Target_Slot")
		self.TargetSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectTargetItemSlot))
		self.TargetSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInTargetItem))
		self.TargetSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		if not app.ENABLE_NEW_EXCHANGE_WINDOW:
			self.TargetAcceptLight = self.GetChild("Target_Accept_Light")
			self.TargetAcceptLight.Disable()

		self.targetMoneyText = self.GetChild("target_money_text_ui")

		self.targetMoneyIcon = self.GetChild("target_money_icon_ui")
		self.targetMoneyIcon.SetMouseOverInEvent(ui.__mem_func__(self.OnOverIn), 0)
		self.targetMoneyIcon.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverOut))

		if app.USE_CHEQUE_CURRENCY:
			self.targetChequeText = self.GetChild("target_cheque_text_ui")

			self.targetChequeIcon = self.GetChild("target_cheque_icon_ui")
			self.targetChequeIcon.SetMouseOverInEvent(ui.__mem_func__(self.OnOverIn), 1)
			self.targetChequeIcon.SetMouseOverOutEvent(ui.__mem_func__(self.OnOverOut))
		else:
			self.GetChild("target_cheque_icon_ui").Hide()
			self.GetChild("target_cheque_slot_ui").Hide()

		## Button
		self.AcceptButton = self.GetChild("Owner_Accept_Button")
		self.AcceptButton.SetToggleDownEvent(ui.__mem_func__(self.AcceptExchange))

		if app.ENABLE_NEW_EXCHANGE_WINDOW:
			self.TargetAcceptButton = self.GetChild("Target_Accept_Button")

		self.TitleName = self.GetChild("TitleName")
		self.GetChild("TitleBar").SetCloseEvent(net.SendExchangeExitPacket)

		if app.ENABLE_NEW_EXCHANGE_WINDOW:
			self.FaceOwnerImage = self.GetChild("FaceOwner_Image")
			self.FaceTargetImage = self.GetChild("FaceTarget_Image")
			self.TargetName = self.GetChild("target_NameText")
			self.TargetLevel = self.GetChild("target_LvText")
			self.ExchangeLogs = self.GetChild("ExchangeLogs")
			self.LogsScrollBar = ui.ThinScrollBar()
			self.LogsScrollBar.SetParent(self.ExchangeLogs)
			self.LogsScrollBar.SetPosition(442 - 75, 17)
			self.LogsScrollBar.SetScrollBarSize(50)
			self.LogsScrollBar.Show()
			self.LogsDropList = ui.ListBoxEx()
			self.LogsDropList.SetParent(self.ExchangeLogs)
			self.LogsDropList.itemHeight = 12
			self.LogsDropList.itemStep = 13
			self.LogsDropList.SetPosition(35, 27)
			self.LogsDropList.SetSize(0, 45)
			self.LogsDropList.SetScrollBar(self.LogsScrollBar)
			self.LogsDropList.SetViewItemCount(2)
			self.LogsDropList.Show()
			self.LogsScrollBar.Show()
			self.listOwnerSlot = []
			self.listTargetSlot = []

	def Destroy(self):
		print "---------------------------------------------------------------------------- DESTROY EXCHANGE"

		self.OwnerSlot = None
		self.ownerMoneyText = None

		if not app.ENABLE_NEW_EXCHANGE_WINDOW:
			self.OwnerAcceptLight = 0

		if self.ownerMoneyButton:
			self.ownerMoneyButton.SetEvent(None)
			self.ownerMoneyButton = None

		if self.ownerMoneyIcon:
			self.ownerMoneyIcon.SetMouseOverInEvent(None)
			self.ownerMoneyIcon.SetMouseOverOutEvent(None)
			self.ownerMoneyIcon.SetEvent(None, "mouse_click")
			self.ownerMoneyIcon = None

		if app.USE_CHEQUE_CURRENCY:
			self.ownerChequeText = None

			if self.ownerChequeButton:
				self.ownerChequeButton.SetEvent(None)
				self.ownerChequeButton = None

			if self.ownerChequeIcon:
				self.ownerChequeIcon.SetMouseOverInEvent(None)
				self.ownerChequeIcon.SetMouseOverOutEvent(None)
				self.ownerChequeIcon.SetEvent(None, "mouse_click")
				self.ownerChequeIcon = None

		self.TargetSlot = None
		self.targetMoneyText = None

		if not app.ENABLE_NEW_EXCHANGE_WINDOW:
			self.TargetAcceptLight = None

		if self.targetMoneyIcon:
			self.targetMoneyIcon.SetMouseOverInEvent(None)
			self.targetMoneyIcon.SetMouseOverOutEvent(None)
			self.targetMoneyIcon = None

		if app.USE_CHEQUE_CURRENCY:
			self.targetChequeText = None

			if self.targetChequeIcon:
				self.targetChequeIcon.SetMouseOverInEvent(None)
				self.targetChequeIcon.SetMouseOverOutEvent(None)
				self.targetChequeIcon = None

		self.TitleName = 0
		self.AcceptButton = 0
		if app.ENABLE_NEW_EXCHANGE_WINDOW:
			self.TargetAcceptButton = 0
			self.FaceOwnerImage = None
			self.FaceTargetImage = None
			self.TargetName = None
			self.TargetLevel = None
			self.ExchangesLogsWindow = None
			self.LogsDropList.RemoveAllItems()
			self.LogsScrollBar = None
			self.LogsDropList = None

		self.tooltipItem = 0

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.interface = 0
			self.wndInventory = 0
			self.lockedItems = {i:(-1,-1) for i in range(exchange.EXCHANGE_ITEM_MAX_NUM)}
			self.wndInventoryNew = 0
			self.lockedItemsSpecial = {i:(-1,-1,-1) for i in range(exchange.EXCHANGE_ITEM_MAX_NUM)}

		if self.toolTipDesc:
			self.toolTipDesc.ClearToolTip()
			del self.toolTipDesc
			self.toolTipDesc = None

		if self.dlgPickMoney:
			self.dlgPickMoney.Destroy()
			del self.dlgPickMoney
			self.dlgPickMoney = None

		if app.USE_CHEQUE_CURRENCY and self.dlgPickCheque:
			self.dlgPickCheque.Destroy()
			del self.dlgPickCheque
			self.dlgPickCheque = None

		self.ClearDictionary()

	def OpenDialog(self):
		if app.ENABLE_LEVEL_IN_TRADE:
			self.TitleName.SetText(localeInfo.EXCHANGE_TITLE_LEVEL % (exchange.GetNameFromTarget(), exchange.GetLevelFromTarget()))
		else:
			self.TitleName.SetText(localeInfo.EXCHANGE_TITLE % (exchange.GetNameFromTarget()))
		self.AcceptButton.Enable()
		self.AcceptButton.SetUp()
		if app.ENABLE_NEW_EXCHANGE_WINDOW:
			self.TargetAcceptButton.Disable()
			self.TargetAcceptButton.SetUp()
			self.FaceOwnerImage.LoadImage(self.FACE_IMAGE_DICT[exchange.GetRaceFromSelf()])
			self.FaceTargetImage.LoadImage(self.FACE_IMAGE_DICT[exchange.GetRaceFromTarget()])
			self.TargetName.SetText(exchange.GetNameFromTarget())
			self.TargetLevel.SetText(localeInfo.NEW_EXCHANGE_LEVEL % (exchange.GetLevelFromTarget()))
			self.LogsDropList.RemoveAllItems()
			self.LogsDropList.AppendItem(self.Item(self, localeInfo.NEW_EXCHANGE_YOU_READY % (str(time.strftime("[%H:%M:%S]"))), 0))

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.interface.SetOnTopWindow(player.ON_TOP_WND_EXCHANGE)
			self.interface.RefreshMarkInventoryBag()

		self.Show()

		(self.xStart, self.yStart, z) = player.GetMainCharacterPosition()

	def CloseDialog(self):
		wndMgr.OnceIgnoreMouseLeftButtonUpEvent()

		self.OnOverOut()

		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()

		if self.dlgPickMoney and self.dlgPickMoney.IsShow():
			self.dlgPickMoney.Close()

		if app.USE_CHEQUE_CURRENCY and self.dlgPickCheque and self.dlgPickCheque.IsShow():
			self.dlgPickCheque.Close()

		if app.WJ_ENABLE_TRADABLE_ICON:
			for exchangePos, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
				if itemInvenPage == self.wndInventory.GetInventoryPageIndex():
					self.wndInventory.wndItem.SetCanMouseEventSlot(itemSlotPos)
					
			# for exchangePos, (itemInvenPage, itemSlotPos, slotType) in self.lockedItemsSpecial.items():
				# if itemInvenPage == self.wndInventoryNew.GetInventoryPageIndex():
					# self.wndInventoryNew.wndItem.SetCanMouseEventSlot(itemSlotPos)
			for exchangePos, (itemInvenPage, itemSlotPos, invenType) in self.lockedItemsSpecial.items():
		
				category = 0
				if invenType == player.UPGRADE_INVENTORY:
					category = 1
				elif invenType == player.BOOK_INVENTORY:
					category = 2
				elif invenType == player.STONE_INVENTORY:
					category = 3
				elif invenType == player.CHANGE_INVENTORY:
					category = 4
				elif invenType == player.COSTUME_INVENTORY:
					category = 5
				elif invenType == player.CHEST_INVENTORY:
					category = 6
					
				if itemInvenPage == self.wndInventoryNew.GetInventoryPageIndex() and self.wndInventoryNew.GetCategoryIndex() == category:
					self.wndInventoryNew.wndItem.SetCanMouseEventSlot(itemSlotPos)
				
			self.lockedItemsSpecial = {i:(-1,-1,-1) for i in range(exchange.EXCHANGE_ITEM_MAX_NUM)}
			self.lockedItems = {i:(-1,-1) for i in range(exchange.EXCHANGE_ITEM_MAX_NUM)}
			self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
			self.interface.RefreshMarkInventoryBag()

		self.Hide()

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def OnPickMoney(self, money):
		net.SendExchangeElkAddPacket(money)

	if app.USE_CHEQUE_CURRENCY:
		def OnPickCheque(self, cheque):
			net.SendExchangeChequeAddPacket(cheque)

	def OpenPickMoneyDialog(self, num):
		if num == 0:
			if app.USE_CHEQUE_CURRENCY and self.dlgPickCheque and self.dlgPickCheque.IsShow():
				self.dlgPickCheque.Close()

			if exchange.GetElkFromSelf() > 0:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.NEW_EXCHANGE_CANT_EDIT_MONEY if app.USE_CHEQUE_CURRENCY else localeInfo.EXCHANGE_CANT_EDIT_MONEY)
				return

			if not self.dlgPickMoney:
				self.dlgPickMoney = uiPickMoney.PickMoneyDialogNew(0)

			self.dlgPickMoney.SetAcceptEvent(ui.__mem_func__(self.OnPickMoney))
			self.dlgPickMoney.SetTitleName(localeInfo.TOOLTIP_CURRENCY_YANG)
			self.dlgPickMoney.Open(player.GetElk())
		elif app.USE_CHEQUE_CURRENCY and num == 1:
			if self.dlgPickMoney and self.dlgPickMoney.IsShow():
				self.dlgPickMoney.Close()

			if exchange.GetChequeFromSelf() > 0:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.NEW_EXCHANGE_CANT_EDIT_CHEQUE)
				return

			if not self.dlgPickCheque:
				self.dlgPickCheque = uiPickMoney.PickMoneyDialogNew(1)

			self.dlgPickCheque.SetAcceptEvent(ui.__mem_func__(self.OnPickCheque))
			self.dlgPickCheque.SetTitleName(localeInfo.TOOLTIP_CURRENCY_WON)
			self.dlgPickCheque.Open(player.GetCheque())

	def AcceptExchange(self):
		if app.ENABLE_NEW_EXCHANGE_WINDOW:
			atLeastOneItem = 0
			atLeastOneYang = 0
			for i in xrange(exchange.EXCHANGE_ITEM_MAX_NUM):
				itemCount = exchange.GetItemCountFromTarget(i)
				if itemCount >= 1:
					atLeastOneYang = 1
					break

			if exchange.GetElkFromTarget() >= 1:
				atLeastOneYang = 1
			elif app.USE_CHEQUE_CURRENCY and exchange.GetChequeFromTarget() >= 1:
				atLeastOneYang = 1

			if atLeastOneItem or atLeastOneYang:
				net.SendExchangeAcceptPacket()
				self.AcceptButton.Disable()
			else:
				atLeastOneItem = 0
				atLeastOneYang = 0
				for i in xrange(exchange.EXCHANGE_ITEM_MAX_NUM):
					itemCount = exchange.GetItemCountFromSelf(i)
					if itemCount >= 1:
						atLeastOneYang = 1
						break

				if exchange.GetElkFromSelf() >= 1:
					atLeastOneYang = 1
				elif app.USE_CHEQUE_CURRENCY and exchange.GetChequeFromSelf() >= 1:
					atLeastOneYang = 1

				if atLeastOneItem or atLeastOneYang:
					self.questionDialog = uiCommon.QuestionDialog2()
					self.questionDialog.SetText1(localeInfo.NEW_EXCHANGE_ALERT1)
					self.questionDialog.SetText2(localeInfo.NEW_EXCHANGE_ALERT2)
					self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.AcceptQuestion))
					self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
					self.questionDialog.Open()
				else:
					net.SendExchangeAcceptPacket()
					self.AcceptButton.Disable()
		else:
			net.SendExchangeAcceptPacket()
			self.AcceptButton.Disable()

	if app.ENABLE_NEW_EXCHANGE_WINDOW:
		def AcceptQuestion(self):
			net.SendExchangeAcceptPacket()
			self.AcceptButton.Disable()
			if self.questionDialog:
				self.questionDialog.Close()

			self.questionDialog = None

		def OnCloseQuestionDialog(self):
			if self.questionDialog:
				self.questionDialog.Close()

			self.questionDialog = None
			self.AcceptButton.Enable()
			self.AcceptButton.SetUp()

	def SelectOwnerEmptySlot(self, SlotIndex):
		if False == mouseModule.mouseController.isAttached():
			return

		if mouseModule.mouseController.IsAttachedMoney():
			net.SendExchangeElkAddPacket(mouseModule.mouseController.GetAttachedMoneyAmount())
		else:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			if (player.SLOT_TYPE_INVENTORY == attachedSlotType or player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedSlotType):
				attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
				SrcSlotNumber = mouseModule.mouseController.GetAttachedSlotNumber()
				DstSlotNumber = SlotIndex

				itemID = player.GetItemIndex(attachedInvenType, SrcSlotNumber)
				item.SelectItem(itemID)

				if item.IsAntiFlag(item.ANTIFLAG_GIVE):
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANNOT_GIVE)
					mouseModule.mouseController.DeattachObject()
					return

				net.SendExchangeItemAddPacket(attachedInvenType, SrcSlotNumber, DstSlotNumber)

			if app.ENABLE_SPECIAL_STORAGE:
				if player.SLOT_TYPE_UPGRADE_INVENTORY == attachedSlotType or\
					player.SLOT_TYPE_BOOK_INVENTORY == attachedSlotType or\
					player.SLOT_TYPE_CHANGE_INVENTORY == attachedSlotType or\
					player.SLOT_TYPE_COSTUME_INVENTORY == attachedSlotType or\
					player.SLOT_TYPE_STONE_INVENTORY == attachedSlotType or\
					player.SLOT_TYPE_CHEST_INVENTORY == attachedSlotType:
					attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
					SrcSlotNumber = mouseModule.mouseController.GetAttachedSlotNumber()
					DstSlotNumber = SlotIndex

					itemID = player.GetItemIndex(attachedSlotType, SrcSlotNumber)
					item.SelectItem(itemID)

					if item.IsAntiFlag(item.ANTIFLAG_GIVE):
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANNOT_GIVE)
						mouseModule.mouseController.DeattachObject()
						return

					net.SendExchangeItemAddPacket(attachedInvenType, SrcSlotNumber, DstSlotNumber)

		mouseModule.mouseController.DeattachObject()

	def SelectTargetItemSlot(self, SlotIndex):
		if app.IsCanOpenRender() and app.ENABLE_RENDER_TARGET:
			interface = constInfo.GetInterfaceInstance()
			if interface:
				interface.OpenRenderTargetWindow(0, exchange.GetItemVnumFromTarget(SlotIndex))
			return

	def SelectOwnerItemSlot(self, SlotIndex):

		if app.IsCanOpenRender() and app.ENABLE_RENDER_TARGET:
			interface = constInfo.GetInterfaceInstance()
			if interface:
				interface.OpenRenderTargetWindow(0, exchange.GetItemVnumFromSelf(SlotIndex))
			return

		if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():

			money = mouseModule.mouseController.GetAttachedItemCount()
			net.SendExchangeElkAddPacket(money)

	def RefreshOwnerSlot(self):
		for i in xrange(exchange.EXCHANGE_ITEM_MAX_NUM):
			itemIndex = exchange.GetItemVnumFromSelf(i)
			itemCount = exchange.GetItemCountFromSelf(i)
			if 1 == itemCount:
				itemCount = 0
			self.OwnerSlot.SetItemSlot(i, itemIndex, itemCount)
		self.OwnerSlot.RefreshSlot()

	def RefreshTargetSlot(self):
		for i in xrange(exchange.EXCHANGE_ITEM_MAX_NUM):
			itemIndex = exchange.GetItemVnumFromTarget(i)
			itemCount = exchange.GetItemCountFromTarget(i)
			if 1 == itemCount:
				itemCount = 0
			self.TargetSlot.SetItemSlot(i, itemIndex, itemCount)
		self.TargetSlot.RefreshSlot()

	def Refresh(self):

		self.RefreshOwnerSlot()
		self.RefreshTargetSlot()

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.RefreshLockedSlot()

		if app.USE_CHEQUE_CURRENCY:
			self.ownerChequeText.SetText(localeInfo.DottedNumber(exchange.GetChequeFromSelf()))
			self.ownerMoneyText.SetText(localeInfo.DottedNumber(exchange.GetElkFromSelf()))

			self.targetChequeText.SetText(localeInfo.DottedNumber(exchange.GetChequeFromTarget()))
			self.targetMoneyText.SetText(localeInfo.DottedNumber(exchange.GetElkFromTarget()))
		else:
			self.ownerMoneyText.SetText(localeInfo.NumberToMoneyString(exchange.GetElkFromSelf()))
			self.targetMoneyText.SetText(localeInfo.NumberToMoneyString(exchange.GetElkFromTarget()))

		if True == exchange.GetAcceptFromSelf():
			if not app.ENABLE_NEW_EXCHANGE_WINDOW:
				self.OwnerAcceptLight.Down()
			else:
				self.OwnerSlot.SetSlotBaseImage("d:/ymir work/ui/public/slot_base.sub", 0.3500, 0.8500, 0.3500, 1.0)
				self.LogsDropList.AppendItem(self.Item(self, localeInfo.NEW_EXCHANGE_YOU_ACCEPT % (str((time.strftime("[%H:%M:%S]")))), 0))
		else:
			self.AcceptButton.Enable()
			self.AcceptButton.SetUp()
			if not app.ENABLE_NEW_EXCHANGE_WINDOW:
				self.OwnerAcceptLight.SetUp()
			else:
				self.OwnerSlot.SetSlotBaseImage("d:/ymir work/ui/public/slot_base.sub", 1.0, 1.0, 1.0, 1.0)

		if True == exchange.GetAcceptFromTarget():
			if not app.ENABLE_NEW_EXCHANGE_WINDOW:
				self.TargetAcceptLight.Down()
			else:
				self.TargetAcceptButton.Down()
				self.TargetSlot.SetSlotBaseImage("d:/ymir work/ui/public/slot_base.sub", 0.3500, 0.8500, 0.3500, 1.0)
				self.LogsDropList.AppendItem(self.Item(self, localeInfo.NEW_EXCHANGE_ACCEPT % (str((time.strftime("[%H:%M:%S]"))), exchange.GetNameFromTarget()), 0))
		else:
			if not app.ENABLE_NEW_EXCHANGE_WINDOW:
				self.TargetAcceptLight.SetUp()
			else:
				if self.TargetAcceptButton.IsDown() == True:
					self.LogsDropList.AppendItem(self.Item(self, localeInfo.NEW_EXCHANGE_ABORT % (str((time.strftime("[%H:%M:%S]"))), exchange.GetNameFromTarget()), 0))

				self.TargetAcceptButton.SetUp()
				self.TargetSlot.SetSlotBaseImage("d:/ymir work/ui/public/slot_base.sub", 1.0, 1.0, 1.0, 1.0)

	def OverInOwnerItem(self, slotIndex):
		if 0 != self.tooltipItem:
			self.tooltipItem.SetExchangeOwnerItem(slotIndex)

	def OverInTargetItem(self, slotIndex):
		if 0 != self.tooltipItem:
			self.tooltipItem.SetExchangeTargetItem(slotIndex)

	def OverOutItem(self):
		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OnTop(self):
		if app.WJ_ENABLE_TRADABLE_ICON:
			if self.interface:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_EXCHANGE)
				self.interface.RefreshMarkInventoryBag()
		self.tooltipItem.SetTop()

	if app.WJ_ENABLE_TRADABLE_ICON:
		def CantTradableItem(self, destSlotIndex, srcSlotIndex):
			if True == exchange.GetAcceptFromTarget():
				return

			itemInvenPage = srcSlotIndex / player.INVENTORY_PAGE_SIZE
			localSlotPos = srcSlotIndex - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
			self.lockedItems[destSlotIndex] = (itemInvenPage, localSlotPos)

			if self.wndInventory.GetInventoryPageIndex() == itemInvenPage and self.IsShow():
				self.wndInventory.wndItem.SetCantMouseEventSlot(localSlotPos)

		def CantTradableItemFromStorage(self, destSlotIndex, srcSlotIndex, invenType):
			if True == exchange.GetAcceptFromTarget():
				return

			itemInvenPage = srcSlotIndex / player.SPECIAL_PAGE_SIZE
			localSlotPos = srcSlotIndex - (itemInvenPage * player.SPECIAL_PAGE_SIZE)
			self.lockedItemsSpecial[destSlotIndex] = (itemInvenPage, localSlotPos, invenType)
			
			category = 0
			if invenType == player.UPGRADE_INVENTORY:
				category = 1
			elif invenType == player.BOOK_INVENTORY:
				category = 2
			elif invenType == player.STONE_INVENTORY:
				category = 3
			elif invenType == player.CHANGE_INVENTORY:
				category = 4
			elif invenType == player.COSTUME_INVENTORY:
				category = 5
			elif invenType == player.CHEST_INVENTORY:
				category = 6


			if self.wndInventoryNew.GetInventoryPageIndex() == itemInvenPage and self.wndInventoryNew.GetCategoryIndex() == category and self.IsShow():
				self.wndInventoryNew.wndItem.SetCantMouseEventSlot(localSlotPos)


		def RefreshLockedSlot(self):
			if self.wndInventory:
				for exchangePos, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
					if self.wndInventory.GetInventoryPageIndex() == itemInvenPage:
						self.wndInventory.wndItem.SetCantMouseEventSlot(itemSlotPos)

				self.wndInventory.wndItem.RefreshSlot()
				
			if self.wndInventoryNew:
				for exchangePos, (itemInvenPage, itemSlotPos, invenType) in self.lockedItemsSpecial.items():
				
					category = 0
					if invenType == player.UPGRADE_INVENTORY:
						category = 1
					elif invenType == player.BOOK_INVENTORY:
						category = 2
					elif invenType == player.STONE_INVENTORY:
						category = 3
					elif invenType == player.CHANGE_INVENTORY:
						category = 4
					elif invenType == player.COSTUME_INVENTORY:
						category = 5
					elif invenType == player.CHEST_INVENTORY:
						category = 6
				
					if self.wndInventoryNew.GetInventoryPageIndex() == itemInvenPage and self.wndInventoryNew.GetCategoryIndex() == category:
						self.wndInventoryNew.wndItem.SetCantMouseEventSlot(itemSlotPos)

				self.wndInventoryNew.wndItem.RefreshSlot()

		def BindInterface(self, interface):
			self.interface = interface

		def SetInven(self, wndInventory):
			from _weakref import proxy
			self.wndInventory = proxy(wndInventory)

		def SetInvenStorage(self, wndInventoryNew):
			from _weakref import proxy
			self.wndInventoryNew = proxy(wndInventoryNew)

	def OnUpdate(self):
		if self.toolTipDesc and self.toolTipDesc.IsShow():
			(x, y) = wndMgr.GetMousePosition()
			self.toolTipDesc.SetToolTipPosition(x + 5, y - 25)

		USE_EXCHANGE_LIMIT_RANGE = 1000

		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xStart) > USE_EXCHANGE_LIMIT_RANGE or abs(y - self.yStart) > USE_EXCHANGE_LIMIT_RANGE:
			(self.xStart, self.yStart, z) = player.GetMainCharacterPosition()
			net.SendExchangeExitPacket()
