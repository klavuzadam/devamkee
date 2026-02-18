# coding: latin_1

import ui
import constInfo
import player
import item
import nonplayer
import net
import uiCommon
import mouseModule
import chat
import grp
import app
import wndMgr
from uiToolTip import ItemToolTip
import localeInfo

def GetAffectString(affectType, affectValue):
	if 0 == affectType:
		return None
	try:
		return ItemToolTip.AFFECT_DICT[affectType](affectValue)
	except TypeError:
		return "UNKNOWN_VALUE[%s] %s" % (affectType, affectValue)
	except KeyError:
		return "UNKNOWN_TYPE[%s] %s" % (affectType, affectValue)



class PetSystemMain(ui.ScriptWindow):

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

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.petSlotIndex = -1
		self.feedWindow = None
		self.feedGrid = None
		self.feedButton = None
		self.questionDialog=None
		self.tooltipexp = self.TextToolTip()
		self.LoadWindow()

	def Destroy(self):
		self.petSlotIndex=0
		self.tooltipexp=None
		self.feedGrid = None
		self.feedButton = None
		self.feedWindow = None
		self.questionDialog = None
		self.ClearDictionary()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/PetInformationWindowNew.py")
			self.GetChild("CloseButton").SetEvent(ui.__mem_func__(self.Close))
			self.GetChild("UpBringing_Pet_Slot").SetOverInItemEvent(ui.__mem_func__(self.OverInPet))
			self.GetChild("UpBringing_Pet_Slot").SetOverOutItemEvent(ui.__mem_func__(self.OverOutPet))

			self.GetChild("UpBringing_Pet_EXP_Gauge_Board").SAFE_SetOverInEvent(self.OverInExp)
			self.GetChild("UpBringing_Pet_EXP_Gauge_Board").SAFE_SetOverOutEvent(self.OverOutExp)

			self.GetChild("PetSkillSlot").SetOverInItemEvent(ui.__mem_func__(self.OverInPetSkill))
			self.GetChild("PetSkillSlot").SetOverOutItemEvent(ui.__mem_func__(self.OverOutPet))
			self.GetChild("PetSkillSlot").SetSelectItemSlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
			#self.GetChild("PetSkillSlot").SAFE_SetButtonEvent("LEFT", "EXIST", self.OnSelectItemSlot)
			self.GetChild("PetSkillSlot").SAFE_SetButtonEvent("RIGHT", "EXIST", self.OnSelectItemSlot)
	
			self.GetChild("FeedEvolButton").SAFE_SetEvent(self.ClickEvolveGUI)
			self.GetChild("AgeWindow").Hide()
		except:
			import exception
			exception.Abort("PetInformationWindow.LoadWindow.LoadObject")

		# pet slot = UpBringing_Pet_Slot
		# pet name = PetName
		# pet mob name = PetMobName
		# pet level = LevelValue
		# pet evolution = AgeValue
		# pet life time = LifeTextValue
		# pet life gquge = LifeGauge

		# bonus_title_0 0-2
		# bonus_value_0 0-2

		# pet exp image UpBringing_Pet_EXPGauge_01 - UpBringing_Pet_EXPGauge_04

	def CreateFeedWindow(self):
		self.feedWindow = ui.BoardWithTitleBar()
		self.feedWindow.SetSize(130,170)
		self.feedWindow.Show()

		self.feedGrid = ui.GridSlotWindow()
		self.feedGrid.SetParent(self.feedWindow)
		self.feedGrid.SetPosition(17,30)
		self.feedGrid.ArrangeSlot(0,3,3,32,32,0,0)
		self.feedGrid.SetOverInItemEvent(ui.__mem_func__(self.OverInFeed))
		self.feedGrid.SetOverOutItemEvent(ui.__mem_func__(self.OverOutPet))
		wndMgr.SetSlotBaseImage(self.feedGrid.hWnd,  "d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)
		self.feedGrid.Show()

		self.feedButton = ui.Button()
		self.feedButton.SetParent(self.feedWindow)
		self.feedButton.SetPosition(50,135)
		self.feedButton.SetUpVisual("d:/ymir work/ui/public/acceptbutton00.sub")
		self.feedButton.SetOverVisual("d:/ymir work/ui/public/acceptbutton01.sub")
		self.feedButton.SetDownVisual("d:/ymir work/ui/public/acceptbutton02.sub")
		self.feedButton.SAFE_SetEvent(self.ClickEvolve)
		self.feedButton.Show()

		self.CheckEvolveFlash()
		self.CheckFeedWindow()

	def OverInFeed(self, index):
		if self.petSlotIndex == 0:
			self.OverOutPet()
			return
		interface = constInfo.GetInterfaceInstance()
		if not interface:
			return
		if None != interface.tooltipItem:
			evolve = player.GetItemMetinSocket(self.petSlotIndex, 3)
			if evolve > 2:
				return

			interface.tooltipItem.SetItemToolTip(constInfo.evoleItems[evolve][index][0])
	
	def CheckFeedWindow(self):
		if self.feedWindow == None:
			return False
		evolve = player.GetItemMetinSocket(self.petSlotIndex, 3)
		if self.petSlotIndex == -1 or evolve == 3:
			for j in xrange(9):
				self.feedGrid.ClearSlot(j)
			self.feedGrid.RefreshSlot()
			return False

		items = constInfo.evoleItems[evolve]
		for j in xrange(len(items)):
			self.feedGrid.SetItemSlot(j, items[j][0], items[j][1])
		self.feedGrid.RefreshSlot()
		return True

	def CheckEvolveFlash(self):
		if self.petSlotIndex == -1:
			self.GetChild("FeedEvolButton").Flash(False)
			if self.feedButton:
				self.feedButton.Flash(False)
			return False
		level = player.GetItemMetinSocket(self.petSlotIndex, 1)
		evolve = player.GetItemMetinSocket(self.petSlotIndex, 3)
		if evolve == 3:
			self.GetChild("FeedEvolButton").Flash(False)
			if self.feedButton:
				self.feedButton.Flash(False)
			return False
		byLimit = constInfo.petEvolutionLimits[evolve]
		if (level == byLimit):
			self.GetChild("FeedEvolButton").Flash(True)
			if self.feedButton:
				self.feedButton.Flash(True)
			return True
		if self.feedButton:
				self.feedButton.Flash(False)
		self.GetChild("FeedEvolButton").Flash(False)
		return False

	def OnMoveWindow(self, x, y):
		if self.feedWindow:
			if self.feedWindow.IsShow():
				self.feedWindow.SetPosition(x+self.GetWidth()-5, y+150)

	def ClickEvolveGUI(self):
		if self.feedWindow == None:
			self.CreateFeedWindow()
			(x,y) = self.GetGlobalPosition()
			self.OnMoveWindow(x,y)
			return

		if self.feedWindow.IsShow():
			self.feedWindow.Hide()
		else:
			self.feedWindow.Show()
			self.CheckFeedWindow()

		(x,y) = self.GetGlobalPosition()
		self.OnMoveWindow(x,y)

	def ClickEvolve(self):
		if self.petSlotIndex == -1:
			return
		petItemVnum = player.GetItemIndex(self.petSlotIndex)
		if petItemVnum == 0:
			return
		level = player.GetItemMetinSocket(self.petSlotIndex, 1)
		if level != 100 and level != 40 and level != 75:
			return
		net.SendChatPacket("/pet_evolve")

	def OverInExp(self):
		self.tooltipexp.Show()

	def OverOutExp(self):
		self.tooltipexp.Hide()

	def OverInPet(self, index):
		if self.petSlotIndex == 0:
			self.OverOutPet()
			return
		interface = constInfo.GetInterfaceInstance()
		if not interface:
			return
		if None != interface.tooltipItem:
			interface.tooltipItem.SetInventoryItem(self.petSlotIndex)

	def OverOutPet(self):
		interface = constInfo.GetInterfaceInstance()
		if not interface:
			return
		if None != interface.tooltipItem:
			interface.tooltipItem.HideToolTip()

	def ClearData(self):
		self.GetChild("UpBringing_Pet_Slot").ClearSlot(0)
		self.GetChild("PetName").SetText("")
		self.GetChild("PetMobName").SetText("")
		self.GetChild("LevelValue").SetText("")
		self.GetChild("AgeValue").SetText("")
		self.GetChild("LifeTextValue").SetText("")
		self.GetChild("LifeGauge").Hide()

		for j in xrange(3):
			self.GetChild("bonus_value_%d"%j).SetText("")
			self.GetChild("bonus_title_%d"%j).SetText("")

		for i in range(1,5):
			self.GetChild("UpBringing_Pet_EXPGauge_0%d"%i).Hide()

		for j in xrange(15):
			self.GetChild("PetSkillSlot").SetPetSkillSlot(j,99,0)
		self.GetChild("PetSkillSlot").RefreshSlot()

		self.petSlotIndex=-1
		self.tooltipexp.SetText("")

	def SetSlotIndex(self, petSlotIndex):
		self.petSlotIndex = int(petSlotIndex)

	def Open(self):
		if self.LoadDataFromPet() == True:
			self.UpdateExp()
			self.UpdateLevel()
			self.UpdateAge()
			self.UpdateTime()
			self.UpdateBonus()
			self.UpdateSkill()
			self.CheckEvolveFlash()
			self.CheckFeedWindow()
		if not self.IsShow():
			self.SetCenterPosition()
			self.SetTop()
		self.Show()

	def LoadDataFromPet(self):
		if self.petSlotIndex == -1:
			self.ClearData()
			return False

		petItemVnum = player.GetItemIndex(self.petSlotIndex)

		item.SelectItem(petItemVnum)

		self.GetChild("UpBringing_Pet_Slot").SetItemSlot(0,petItemVnum,0)

		if player.GetItemMetinSocket(self.petSlotIndex, 1) >= 80:
			self.GetChild("PetMobName").SetText(nonplayer.GetMonsterName(item.GetValue(0)))
		else:
			self.GetChild("PetMobName").SetText(nonplayer.GetMonsterName(item.GetValue(1)))

		itemName = player.GetItemNewName(self.petSlotIndex)
		if itemName != "^":
			self.GetChild("PetName").SetText(itemName)
		else:
			self.GetChild("PetName").SetText(item.GetItemName())
		return True

	def UpdateExp(self):
		level = player.GetItemMetinSocket(self.petSlotIndex, 1)
		curPoint = player.GetItemMetinSocket(self.petSlotIndex, 2)
		if level >= 120:
			maxPoint = 2500000000
		else:
			maxPoint = constInfo.exp_table[level]
		curPoint = min(curPoint, maxPoint)
		curPoint = max(curPoint, 0)
		maxPoint = max(maxPoint, 0)
		quarterPoint = maxPoint / 4
		FullCount = 0
		if 0 != quarterPoint:
			FullCount = min(4, curPoint / quarterPoint)
		for i in range(1,5):
			self.GetChild("UpBringing_Pet_EXPGauge_0%d"%i).Hide()
		for i in xrange(FullCount):
			self.GetChild("UpBringing_Pet_EXPGauge_0%d"%(i+1)).SetRenderingRect(0.0, 0.0, 0.0, 0.0)
			self.GetChild("UpBringing_Pet_EXPGauge_0%d"%(i+1)).Show()
		if 0 != quarterPoint:
			if FullCount < 4:
				Percentage = float(curPoint % quarterPoint) / quarterPoint - 1.0
				self.GetChild("UpBringing_Pet_EXPGauge_0%d"%(FullCount+1)).SetRenderingRect(0.0, Percentage, 0.0, 0.0)
				self.GetChild("UpBringing_Pet_EXPGauge_0%d"%(FullCount+1)).Show()
		self.tooltipexp.SetText("%s : %.2f%%" % (localeInfo.TASKBAR_EXP, float(curPoint) / max(1, float(maxPoint)) * 100))

	def UpdateLevel(self):
		self.GetChild("LevelValue").SetText(str(player.GetItemMetinSocket(self.petSlotIndex, 1)))

	def UpdateAge(self):
		age_text = [localeInfo.PET_GUI_YOUNG, localeInfo.PET_GUI_WILD, localeInfo.PET_GUI_BRAVE, localeInfo.PET_GUI_HERO]
		self.GetChild("AgeValue").SetText(age_text[player.GetItemMetinSocket(self.petSlotIndex, 3)])

	def UpdateTime(self):
		real_time = player.GetItemMetinSocket(self.petSlotIndex, 0)-app.GetGlobalTimeStamp()
		if real_time < 0:
			real_time = 0
		self.GetChild("LifeTextValue").SetText(localeInfo.SecondToDHM(real_time))
		if int(real_time) > 0:
			self.GetChild("LifeGauge").SetPercentage(int(real_time)*1.6, int(60*60*24*14)*6)
			self.GetChild("LifeGauge").Show()
		else:
			self.GetChild("LifeGauge").Hide()

	def UpdateBonus(self):

		for j in xrange(3):
			ptr = player.GetItemMetinSocket(self.petSlotIndex, 5+j)
			if ptr == 20:
				bonus_value = constInfo.pet_bonus_value[j]
			else:
				bonus_value = float(float(constInfo.pet_bonus_value[j])/constInfo.PET_BONUS_MAX_LEVEL) * ptr

			text = GetAffectString(constInfo.pet_bonus_types[j],int(bonus_value))
			text = text.replace(":","")
			index = text.find("+")
			if index == -1:
				index = text.find("%")
				if index == -1:
					index =0
			if not index  <= 0:
				self.GetChild("bonus_value_%d"%j).SetText("Lv %d  -  %s"%(ptr,text[index:]))
				self.GetChild("bonus_title_%d"%j).SetText(text[:index])
			else:
				self.GetChild("bonus_value_%d"%j).SetText("Lv %d  -  +%d"%(ptr,int(bonus_value)))
				self.GetChild("bonus_title_%d"%j).SetText(text)

		# SKILLS
	
	def UpdateSkill(self):
		for j in xrange(15):
			itemAttribute = player.GetItemAttribute(self.petSlotIndex,j)
			self.GetChild("PetSkillSlot").SetPetSkillSlot(j,itemAttribute[0],itemAttribute[1])
		self.GetChild("PetSkillSlot").RefreshSlot()

	def OnSelectItemSlot(self, selectedSlotPos):
		if self.petSlotIndex == -1:
			return

		itemAttribute = player.GetItemAttribute(self.petSlotIndex,selectedSlotPos)
		if itemAttribute[0] != 99:
			return

		self.questionDialog = uiCommon.QuestionDialog()
		self.questionDialog.SetText(localeInfo.PET_ADD_SLOT_QUESTION)
		self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.__UseAddSkillSlotOnAccept))
		self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__UseItemOnCancel))
		self.questionDialog.Open()
		self.questionDialog.dstItemSlotPos = selectedSlotPos
		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

	def __UseAddSkillSlotOnAccept(self):
		if player.GetItemMetinSocket(self.petSlotIndex, 1) < 100:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PET_DIALOG%115)
			self.__UseItemOnCancel()
			return
		net.SendChatPacket("/pet_add_slot %d"%self.questionDialog.dstItemSlotPos)
		self.__UseItemOnCancel()

	def OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if (isAttached):
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()
			if (player.SLOT_TYPE_INVENTORY != attachedSlotType):
				return

			attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
			itemVNum = player.GetItemIndex(attachedInvenType, attachedSlotPos)
			item.SelectItem(itemVNum)

			if item.GetItemType() == item.ITEM_TYPE_PET and item.GetItemSubType() == item.PET_SKILL_DEL_BOOK:
				itemAttribute = player.GetItemAttribute(self.petSlotIndex,selectedSlotPos)
				# if itemAttribute[0] >= 1 and itemAttribute[0] <= 23:
				self.questionDialog = uiCommon.QuestionDialog()
				self.questionDialog.SetText(localeInfo.PET_REMOVE_SLOT_QUESTION %constInfo.pet_skill_data[itemAttribute[0]][0])
				self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.__UseItemOnAccept))
				self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__UseItemOnCancel))
				self.questionDialog.Open()
				self.questionDialog.srcItemSlotPos = attachedSlotPos
				self.questionDialog.dstItemSlotPos = selectedSlotPos
				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

	def __UseItemOnCancel(self):
		if not self.questionDialog:
			return
		self.questionDialog.Close()
		self.questionDialog = None
		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

	def __UseItemOnAccept(self):
		net.SendItemUseToItemPacket(self.questionDialog.srcItemSlotPos,self.questionDialog.dstItemSlotPos)
		self.__UseItemOnCancel()

	def OverInPetSkill(self, index):
		itemAttribute = player.GetItemAttribute(self.petSlotIndex,index)

		if itemAttribute[0] == 99 or itemAttribute[0] == 0:
			self.OverOutPet()
			return

		interface = constInfo.GetInterfaceInstance()
		if not interface:
			return
		if None != interface.tooltipItem:
			pet_data = constInfo.pet_skill_data[itemAttribute[0]]
	
			interface.tooltipItem.ClearToolTip()
			interface.tooltipItem.AutoAppendTextLine(pet_data[0], grp.GenerateColor(0.9490, 0.9058, 0.7568, 1.0))
			interface.tooltipItem.AppendSpace(5)

			if isinstance(pet_data[1], list):
				for j in xrange(len(pet_data[1])):
					interface.tooltipItem.AppendSpace(5)
					if itemAttribute[1] == 20:
						bonus_value = pet_data[2]
					else:
						bonus_value = float(float(pet_data[2])/constInfo.PET_SKILL_MAX_LEVEL) * itemAttribute[1]
					text = GetAffectString(pet_data[1][j],int(bonus_value))
					interface.tooltipItem.AutoAppendTextLine(text)
			else:
				interface.tooltipItem.AppendSpace(5)
				if itemAttribute[1] == 20:
					bonus_value = pet_data[2]
				else:
					bonus_value = float(float(pet_data[2])/constInfo.PET_SKILL_MAX_LEVEL) * itemAttribute[1]
				text = GetAffectString(pet_data[1],int(bonus_value))
				interface.tooltipItem.AutoAppendTextLine(text)
			interface.tooltipItem.ResizeToolTip()
			interface.tooltipItem.ShowToolTip()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def Close(self):
		if self.feedWindow:
			if self.feedWindow.IsShow():
				self.feedWindow.Hide()
		self.Hide()
