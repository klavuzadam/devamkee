# coding: latin_1

import ui
import localeInfo
import uiScriptLocale
import chr
import item
import app
import skill
import player
import uitooltip
import collections
import math
import chat
import constInfo

from collections import OrderedDict

AFF_LEADERSHIP = 901
AFF_SWITCHBOT_PREMIUM = 605

if app.ENABLE_VOTE4BUFF:
	voteBuffData = [0,0]

import uiCommon, constInfo, net
from _weakref import proxy
overInImage = None
affectDict={}

def TypeToAffect(affect):
	_dict = {
		chr.AFFECT_JEONGWI : 3,
		chr.AFFECT_GEOMGYEONG : 4,
		chr.AFFECT_CHEONGEUN : 19,
		chr.AFFECT_FALLEN_CHEONGEUN : 19,
		chr.AFFECT_GYEONGGONG : 49,
		chr.AFFECT_EUNHYEONG : 34,
		chr.AFFECT_GWIGEOM : 63,
		chr.AFFECT_GONGPO : 64,
		chr.AFFECT_JUMAGAP : 65,
		chr.AFFECT_HOSIN : 94,
		chr.AFFECT_HOSIN_PERFECT : 94,
		chr.AFFECT_BOHO : 95,
		chr.AFFECT_BOHO_PERFECT : 95,
		chr.AFFECT_KWAESOK : 110,
		chr.AFFECT_KWAESOK_PERFECT : 110,
		chr.AFFECT_HEUKSIN : 79,
		chr.AFFECT_MUYEONG : 78,
		chr.AFFECT_GICHEON : 96,
		chr.AFFECT_GICHEON_PERFECT : 96,
		chr.AFFECT_JEUNGRYEOK : 111,
		chr.AFFECT_JEUNGRYEOK_PERFECT : 111,
		chr.AFFECT_PABEOP : 66,
	}

	_dict[chr.AFFECT_BUFFI_HOSIN] = 164
	_dict[chr.AFFECT_BUFFI_GICHEON] = 165
	_dict[chr.AFFECT_BUFFI_KWAESOK] = 166
	_dict[chr.AFFECT_BUFFI_JEUNGRYEO] = 167

	return _dict[affect] if _dict.has_key(affect) else affect

if app.ENABLE_MULTI_FARM_BLOCK:
	import uiCommon
	import net
	class MultiFarmImage(ui.ExpandedImageBox):
		def __del__(self):
			ui.ExpandedImageBox.__del__(self)
		def Destroy(self):
			self.multiFarmStatus = 0
			self.players = []
		def __init__(self):
			ui.ExpandedImageBox.__init__(self)
			self.Destroy()
		def SetMultiFarmInfo(self, multiFarmStatus):
			self.multiFarmStatus = multiFarmStatus
			if multiFarmStatus:
				self.LoadImage("d:/ymir work/ui/game/farm/can_farm.tga")
			else:
				self.LoadImage("d:/ymir work/ui/game/farm/cant_farm.tga")
		def OnMouseOverIn(self):
			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				if interface.tooltipItem:
					interface.tooltipItem.ClearToolTip()
					interface.tooltipItem.AutoAppendTextLineSpecial("|cffF1E6C0"+localeInfo.MULTI_FARM_TITLE)
					interface.tooltipItem.AppendSpace(5)
					interface.tooltipItem.AutoAppendTextLineSpecial(localeInfo.MULTI_FARM_TEXT)
					interface.tooltipItem.AppendSpace(5)
					for name in self.players:
						interface.tooltipItem.AutoAppendTextLineSpecial("|cffF1E6C0"+name)
					interface.tooltipItem.ShowToolTip()
		def OnMouseOverOut(self):
			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				if interface.tooltipItem:
					interface.tooltipItem.HideToolTip()

if app.ENABLE_YOHARA_SYSTEM:
	#Sungma
	class SungMaAffectImage(ui.ExpandedImageBox):		
		FILE_PATH_SUNGMA = "d:/ymir work/ui/skill/common/affect/sungma_buff.sub"

		def __init__(self, str, hp, move, inmune):
			ui.ExpandedImageBox.__init__(self)
			
			self.str = str
			self.hp = hp
			self.move = move
			self.inmune = inmune
			
			self.toolTip = uitooltip.ToolTip(300)
			self.toolTip.HideToolTip()

		def __del__(self):
			ui.ExpandedImageBox.__del__(self)

		def SetSungmaValue(self, str, hp, move, inmune):
			self.str = str
			self.hp = hp
			self.move = move
			self.inmune = inmune
			
			self.__Refresh()
		
		def __Refresh(self):
			self.str = self.str
			self.hp = self.hp
			self.move = self.move
			self.inmune = self.inmune
			
			fileName = self.FILE_PATH_SUNGMA
			
			try:
				self.LoadImage(fileName)
			except:
				import dbg
				dbg.TraceError("SungmaPointImage.(STR=%d, HP %d, MOVE %d, INMUNE %d) - LoadError %s" % (self.str, self.hp, self.move, self.inmune, fileName))

			self.SetScale(0.7, 0.7)

			self.toolTip.ClearToolTip()
			self.toolTip.SetTitle(localeInfo.TOOLTIP_AFFECT_SUNGMA_DESC)
			self.toolTip.AppendTextLine(localeInfo.TOOLTIP_AFFECT_SUNGMA_STR % (self.str))
			self.toolTip.AppendTextLine(localeInfo.TOOLTIP_AFFECT_SUNGMA_HP % (self.hp))
			self.toolTip.AppendTextLine(localeInfo.TOOLTIP_AFFECT_SUNGMA_MOVE % (self.move))
			self.toolTip.AppendTextLine(localeInfo.TOOLTIP_AFFECT_SUNGMA_IMMUNE % (self.inmune))
			self.toolTip.ResizeToolTip()
			
		def OnMouseOverIn(self):
			self.toolTip.ShowToolTip()

		def OnMouseOverOut(self):
			self.toolTip.HideToolTip()

class PremiumImage(ui.ExpandedImageBox):

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)
		self.eventToolTip = None
		self.isIn = False

		self.status = 0

	def __del__(self):
		ui.ExpandedImageBox.__del__(self)
		self.eventToolTip = None
		self.isIn = False

		self.status = 0

	def RefreshPremium(self):
		if self.status == 1:
			self.LoadImage("d:/ymir work/ui/game/premium/affect_on.png")
		else:
			self.LoadImage("d:/ymir work/ui/game/premium/affect_off.png")

		self.SetScale(0.7, 0.7)
		
	def GetAffectString(self, affectType, affectValue):
		return uitooltip.ItemToolTip.AFFECT_DICT[affectType](affectValue)

	def OnMouseOverIn(self):
		interface = constInfo.GetInterfaceInstance()
		if interface != None:
			if interface.tooltipItem:
				interface.tooltipItem.ClearToolTip()
				
				interface.tooltipItem.AutoAppendTextLine("Premium", ui.GenerateColor(255, 153, 51))

				if self.status > 0:
					interface.tooltipItem.AutoAppendTextLineSpecial(uiScriptLocale.SUB_TEXT_1.format(localeInfo.SecondToDHMSNew(constInfo.PremiumTime - app.GetGlobalTimeStamp())), ui.GenerateColor(0, 255, 0))
				else:
					interface.tooltipItem.AutoAppendTextLineSpecial(uiScriptLocale.SUB_TEXT_2, ui.GenerateColor(255, 0, 0))

				interface.tooltipItem.AutoAppendTextLine(uiScriptLocale.SUB_TEXT_3)
				interface.tooltipItem.AutoAppendTextLine(uiScriptLocale.SUB_TEXT_4, ui.GenerateColor(255, 153, 51))
				interface.tooltipItem.AutoAppendTextLine(uiScriptLocale.SUB_TEXT_5, ui.GenerateColor(255, 153, 51))
				interface.tooltipItem.AutoAppendTextLine(uiScriptLocale.SUB_TEXT_6, ui.GenerateColor(255, 153, 51))
				interface.tooltipItem.AutoAppendTextLine(uiScriptLocale.SUB_TEXT_7, ui.GenerateColor(255, 153, 51))
				interface.tooltipItem.AutoAppendTextLine(uiScriptLocale.SUB_TEXT_8, ui.GenerateColor(255, 153, 51))
				interface.tooltipItem.ResizeToolTip()

				interface.tooltipItem.ShowToolTip()

	def OnMouseOverOut(self):
		interface = constInfo.GetInterfaceInstance()
		if interface != None:
			if interface.tooltipItem:
				interface.tooltipItem.HideToolTip()

	def Refresh2(self):
		if (self.status != constInfo.IsPremiumActive):
			self.status = constInfo.IsPremiumActive
			self.RefreshPremium()
			
class LovePointImage(ui.ExpandedImageBox):

	FILE_PATH = "d:/ymir work/ui/pattern/LovePoint/"
	FILE_DICT = {
		0 : FILE_PATH + "01.dds",
		1 : FILE_PATH + "02.dds",
		2 : FILE_PATH + "02.dds",
		3 : FILE_PATH + "03.dds",
		4 : FILE_PATH + "04.dds",
		5 : FILE_PATH + "05.dds",
	}

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		self.loverName = ""
		self.lovePoint = 0

	def __del__(self):
		ui.ExpandedImageBox.__del__(self)

	def SetLoverInfo(self, name, lovePoint):
		self.loverName = name
		self.lovePoint = lovePoint
		self.__Refresh()

	def OnUpdateLovePoint(self, lovePoint):
		self.lovePoint = lovePoint
		self.__Refresh()

	def __Refresh(self):
		self.lovePoint = max(0, self.lovePoint)
		self.lovePoint = min(100, self.lovePoint)

		if 0 == self.lovePoint:
			loveGrade = 0
		else:
			loveGrade = self.lovePoint / 25 + 1
		fileName = self.FILE_DICT.get(loveGrade, self.FILE_PATH+"00.dds")

		try:
			self.LoadImage(fileName)
			self.SetScale(0.7, 0.7)
		except:
			import dbg
			dbg.TraceError("LovePointImage.SetLoverInfo(lovePoint=%d) - LoadError %s" % (self.lovePoint, fileName))
	def OnMouseOverIn(self):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			tooltipItem = interface.tooltipItem
			if tooltipItem:
				tooltipItem.ClearToolTip()
				tooltipItem.AutoAppendNewTextLineResize(self.loverName, 0xffE9E7D2)
				tooltipItem.AppendSpace(5)
				tooltipItem.AutoAppendNewTextLineResize(localeInfo.AFF_LOVE_POINT % self.lovePoint)
				tooltipItem.ShowToolTip()
	def OnMouseOverOut(self):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			if interface.tooltipItem:
				interface.tooltipItem.HideToolTip()



# class HorseImage(ui.ExpandedImageBox):

	# FILE_PATH = "d:/ymir work/ui/pattern/HorseState/"

	# FILE_DICT = {
		# 00 : FILE_PATH+"00.dds",
		# 01 : FILE_PATH+"00.dds",
		# 02 : FILE_PATH+"00.dds",
		# 03 : FILE_PATH+"00.dds",
		# 10 : FILE_PATH+"10.dds",
		# 11 : FILE_PATH+"11.dds",
		# 12 : FILE_PATH+"12.dds",
		# 13 : FILE_PATH+"13.dds",
		# 20 : FILE_PATH+"20.dds",
		# 21 : FILE_PATH+"21.dds",
		# 22 : FILE_PATH+"22.dds",
		# 23 : FILE_PATH+"23.dds",
		# 30 : FILE_PATH+"30.dds",
		# 31 : FILE_PATH+"31.dds",
		# 32 : FILE_PATH+"32.dds",
		# 33 : FILE_PATH+"33.dds",
	# }

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)
		self.toolTip = uitooltip.ToolTip(100)
		self.toolTip.HideToolTip()

	def __GetHorseGrade(self, level):
		if 0 == level:
			return 0

		return (level-1)/10 + 1

	def SetState(self, level, health, battery):
		self.toolTip.ClearToolTip()

		if level>0:

			try:
				grade = self.__GetHorseGrade(level)
				self.__AppendText(localeInfo.LEVEL_LIST[grade])
			except IndexError:
				return

			try:
				healthName=localeInfo.HEALTH_LIST[health]
				if len(healthName)>0:
					self.__AppendText(healthName)
			except IndexError:
				return

			if health>0:
				if battery==0:
					self.__AppendText(localeInfo.NEEFD_REST)

			try:
				fileName=self.FILE_DICT[health*10+battery]
			except KeyError:
				pass

			try:
				self.LoadImage(fileName)
			except:
				pass

		self.SetScale(0.7, 0.7)

	def __AppendText(self, text):

		self.toolTip.AppendTextLine(text)
		self.toolTip.ResizeToolTip()

		#x=self.GetWidth()/2
		#textLine = ui.TextLine()
		#textLine.SetParent(self)
		#textLine.SetSize(0, 0)
		#textLine.SetOutline()
		#textLine.Hide()
		#textLine.SetPosition(x, 40+len(self.textLineList)*16)
		#textLine.SetText(text)
		#self.textLineList.append(textLine)

	def OnMouseOverIn(self):
		#for textLine in self.textLineList:
		#	textLine.Show()

		self.toolTip.ShowToolTip()

	def OnMouseOverOut(self):
		#for textLine in self.textLineList:
		#	textLine.Hide()

		self.toolTip.HideToolTip()

class MountImage(ui.ExpandedImageBox):


	AFFECT_DICT = uitooltip.ItemToolTip.AFFECT_DICT

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		self.toolTip = uitooltip.ToolTip(100)
		self.toolTip.HideToolTip()

	def SetState(self, vnum):
		self.toolTip.ClearToolTip()
		
		item.SelectItem(vnum)
		for i in xrange(item.ITEM_APPLY_MAX_NUM):
			(affectType, affectValue) = item.GetAffect(i)
			if affectType != 0:
				affectString = self.AFFECT_DICT[affectType](affectValue)
				if affectString:
					self.__AppendText(affectString)
		self.LoadImage("d:/ymir work/ui/pattern/HorseState/33.dds")


		self.SetScale(0.7, 0.7)

	def __AppendText(self, text):
		self.toolTip.AppendTextLine(text)
		self.toolTip.ResizeToolTip()

	def OnMouseOverIn(self):
		# SET-ANIM-SCALE (Third Argument "True" its "IsScaleTemporary = True")
		if self.xScaleImage != 0 and self.yScaleImage != 0:
			self.SetScale(self.xScaleImage + 0.15, self.yScaleImage + 0.15, True)
		# SET-ANIM-SCALE
		self.toolTip.ShowToolTip()

	def OnMouseOverOut(self):
		# SET-ANIM-SCALE (Third Argument "Non-Present" its "IsScaleTemporary = False")
		if self.xScaleImage != 0 and self.yScaleImage != 0:
			self.SetScale(self.xScaleImage, self.yScaleImage)
		# SET-ANIM-SCALE
		self.toolTip.HideToolTip()


class AffectImage(ui.ExpandedImageBox):
	if app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS:
		def SetCell(self, cell):
			self.cell = cell

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		self.toolTipText = None
		self.toolTip = None
		self.isSkillAffect = False
		self.description = None
		self.endTime = 0
		self.realDuration = 0
		self.affect = None
		self.isClocked = True
		self.itemImage = None
		if app.ENABLE_AFFECT_BUFF_REMOVE:
			self.buffQuestionDialog = None
			self.skillIndex = None
			self.SetEvent(ui.__mem_func__(self.OnBuffQuestionDialog), "mouse_click")
		self.bStepImage = -1

		if app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS:
			self.cell = -1

	def SetAffect(self, affect):
		self.affect = affect

	def GetAffect(self):
		return self.affect

	def SetItemImage(self, fileName):
		if not self.itemImage:
			self.itemImage = ui.ExpandedImageBox()
			self.itemImage.SetParent(self)
			self.itemImage.AddFlag("not_pick")
			self.itemImage.SetWindowHorizontalAlignCenter()
			self.itemImage.SetWindowVerticalAlignCenter()

		if (app.USE_BOOST_POTIONS or app.USE_CRYSTAL_SYSTEM) and self.cell != -1:
			itemVnum = player.GetItemIndex(self.cell)
			if not itemVnum:
				self.itemImage.LoadImage(fileName)
			else:
				item.SelectItem(itemVnum)
				self.itemImage.LoadImage(item.GetIconImageFileName())
		else:
			self.itemImage.LoadImage(fileName)

		self.itemImage.SetScale(0.65, 0.65)
		self.itemImage.SetPosition(0, -1)
		self.itemImage.Show()

	def SetDescription(self, description):
		self.description = description

	def SetDuration(self, duration):
		self.endTime = 0
		if duration > 0:
			self.endTime = app.GetGlobalTimeStamp() + duration
			
		self.realDuration = duration

	def SetSkillAffectFlag(self, flag):
		self.isSkillAffect = flag
		
	if app.ENABLE_AFFECT_BUFF_REMOVE:
		def SetSkillIndex(self, skillIndex):
			self.skillIndex = skillIndex
			
	def IsSkillAffect(self):
		return self.isSkillAffect

	if app.ENABLE_AFFECT_BUFF_REMOVE:
		def OnBuffQuestionDialog(self):
			skillIndex = self.skillIndex
			if not skillIndex or skillIndex == 66:
				return
			self.buffQuestionDialog = uiCommon.QuestionDialog()
			self.buffQuestionDialog.SetWidth(350)
			self.buffQuestionDialog.SetText(localeInfo.BUFF_AFFECT_REMOVE_QUESTION % (skill.GetSkillName(skillIndex)))
			self.buffQuestionDialog.SetAcceptEvent(lambda arg = skillIndex: self.OnCloseBuffQuestionDialog(arg))
			self.buffQuestionDialog.SetCancelEvent(lambda arg = 0: self.OnCloseBuffQuestionDialog(arg))
			self.buffQuestionDialog.Open()
			
		def OnCloseBuffQuestionDialog(self, answer):
			if not self.buffQuestionDialog:
				return

			self.buffQuestionDialog.Close()
			self.buffQuestionDialog = None

			if not answer:
				return

			net.SendChatPacket("/remove_buff %d" % answer)
			return TRUE
			
	if app.ENABLE_COSTUME_SYSTEM:
		def FormatTime(self, seconds):
			if seconds <= 0:
				return "0s"
			m, s = divmod(seconds, 60)
			h, m = divmod(m, 60)
			d, h = divmod(h, 24)
			timeText = ""
			if d > 0:
				timeText += "{}d".format(d)
				timeText += " "
			if h > 0:
				timeText += "{}h".format(h)
				timeText += " "
			if m > 0:
				timeText += "{}m".format(m)
				timeText += " "
			if s > 0:
				timeText += "{}s".format(s)
			return timeText

		def AddAffect(self, pointIdx, pointVal):
			if not [pointIdx, pointVal] in self.affectList:
				self.affectList.append([pointIdx, pointVal])
		def RemoveAffect(self, pointIdx, pointVal):
			if [pointIdx, pointVal] in self.affectList:
				del self.affectList[self.affectList.index([pointIdx, pointVal])]

	def OnMouseOverIn(self):
		global overInImage
		overInImage = None

		# SET-ANIM-SCALE (Third Argument "True" its "IsScaleTemporary = True")
		if self.xScaleImage != 0 and self.yScaleImage != 0:
			self.SetScale(self.xScaleImage + 0.15, self.yScaleImage + 0.15, True)
		if self.itemImage:
			if self.itemImage.xScaleImage != 0 and self.itemImage.yScaleImage != 0:
				self.itemImage.SetScale(self.itemImage.xScaleImage + 0.15, self.itemImage.yScaleImage + 0.15, True)
		# SET-ANIM-SCALE

		if (app.USE_CRYSTAL_SYSTEM and self.affect == chr.AFFECT_CRYSTAL) or\
			(app.USE_BOOST_POTIONS and constInfo.IS_BOOST_POTION_AFFECT(self.affect)):
			if not self.toolTip:
				interfaceInst = constInfo.GetInterfaceInstance()
				if not interfaceInst:
					return

				self.toolTip = interfaceInst.tooltipItem

			if self.toolTip:
				self.toolTip.SetInventoryItem(self.cell, player.INVENTORY, True)
				overInImage = proxy(self)

			return

		if self.toolTip:
			self.toolTip.ShowToolTip()
			overInImage = proxy(self)
			return

		if app.USE_PREMIUM_AFFECT and self.affect == chr.AFFECT_PREMIUM_USER:
			interfaceInst = constInfo.GetInterfaceInstance()
			if not interfaceInst:
				return

			toolTip = interfaceInst.tooltipItem
			toolTip.ClearToolTip()

			toolTip.SetTitle(localeInfo.TOOLTIP_AFFECT_PREMIUM_USER_TITLE)
			toolTip.AppendSpace(5)

			toolTip.AppendDescription(localeInfo.TOOLTIP_AFFECT_PREMIUM_USER_DESC, 26, toolTip.CONDITION_COLOR)

			if self.endTime > 0:
				toolTip.AppendSpace(5)
				toolTip.AutoAppendTextLine("%s: %s" % (localeInfo.LEFT_TIME, self.FormatTime(self.endTime - app.GetGlobalTimeStamp())), 0xffC5C7C4)
				toolTip.AppendSpace(5)

			toolTip.ShowToolTip()

			overInImage = proxy(self)
			return

		if app.ENABLE_VOTE4BUFF:
			if self.GetAffect() == chr.NEW_AFFECT_VOTE4BUFF:
				global voteBuffData
				interface = constInfo.GetInterfaceInstance()
				if interface:
					if interface.tooltipItem:
						interface.tooltipItem.ClearToolTip()
						interface.tooltipItem.AutoAppendNewTextLineResize(self.description)
						interface.tooltipItem.AppendSpace(5)
						interface.tooltipItem.AutoAppendNewTextLineResize(uitooltip.GET_AFFECT_STRING(item.GetApplyTypeByPointType(voteBuffData[0]), voteBuffData[1]))
						interface.tooltipItem.AppendSpace(5)
						if self.endTime > 0:
							time = self.endTime - app.GetGlobalTimeStamp()
							day = int(int((time / 60) / 60) / 24)
							if day > 30:
								leftTime = uiScriptLocale.FOREVER
							else:
								leftTime = localeInfo.SecondToDHM(self.endTime - app.GetGlobalTimeStamp())
							interface.tooltipItem.AutoAppendNewTextLineResize(" (%s : %s)" % (localeInfo.LEFT_TIME, leftTime))
						interface.tooltipItem.ShowToolTip()
				return

		if app.ENABLE_COSTUME_SYSTEM:
			interface = constInfo.GetInterfaceInstance()
			if interface:
				tooltip = interface.tooltipSkill if self.IsSkillAffect() else interface.tooltipItem

				if tooltip:
					tooltip.ClearToolTip()

					global affectDict
					overInImage = proxy(self)
					affect = TypeToAffect(self.GetAffect()) if self.IsSkillAffect() else self.GetAffect()

					if affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY or affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY:
						potionType = player.AUTO_POTION_TYPE_HP if affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY else player.AUTO_POTION_TYPE_SP
						(isActivated, currentAmount, totalAmount, slotIndex) = player.GetAutoPotionInfo(potionType)
						amountPercent = 0.0
						try:
							amountPercent = float(currentAmount) / float(totalAmount) * 100.0
						except:
							amountPercent = 100.0
						tooltip.AutoAppendTextLine(localeInfo.TOOLTIP_AUTO_POTION_REST % amountPercent, 0xffC5C7C4)
						tooltip.AppendSpace(5)
					else:
						affectData = affectDict[affect] if affectDict.has_key(affect) else {}
						affectList = affectData["affect"] if affectData.has_key("affect") else []
						maxDuration = affectData["duration"] if affectData.has_key("duration") else 0
						
						if self.description:
							tooltip.AutoAppendTextLine(self.description, 0xffE9E7D2)
							tooltip.AppendSpace(5)
						__blockedAffects = [22, 23, 24, 29, 30]
						beforeT = self.GetAffect()

						# chat.AppendChat(1, "Hovering over affect %d" % beforeT)
						if self.IsSkillAffect() and not beforeT in __blockedAffects:
							try:
								tooltip.SetSkillEx(affect)
							except:
								for data in affectList:
									if data[0] > 0:
										tooltip.AutoAppendNewTextLineResize(uitooltip.GET_AFFECT_STRING(item.GetApplyTypeByPointType(data[0]), data[1]), 0xff95A693)
						else:
							try:
								for data in affectList:
									# chat.AppendChat(1 , "Data 0 is %d and data1 is %d" % (data[0], data[1]))
									if data[0] == 99:
										tooltip.SetSkillTest(affect, data[1])
									elif data[0] > 0:
										tooltip.AutoAppendNewTextLineResize(uitooltip.GET_AFFECT_STRING(item.GetApplyTypeByPointType(data[0]), data[1]), 0xff95A693)
							except:
								pass

						if self.realDuration > 10000000:
							tooltip.AppendSpace(5)
							tooltip.AutoAppendTextLine("%s: Permanent" % (localeInfo.LEFT_TIME), 0xffC5C7C4)
							tooltip.AppendSpace(5)
							tooltip.AlignHorizonalCenter()
							tooltip.ShowToolTip()
							return

						forbiddenAffects =[94, 95, 110, 96, 111]
						if self.endTime > 0:
							tooltip.AppendSpace(5)
							tooltip.AutoAppendTextLine("%s: %s" % (localeInfo.LEFT_TIME, self.FormatTime(self.endTime - app.GetGlobalTimeStamp())), 0xffC5C7C4)
						elif maxDuration > 0:
							tooltip.AppendSpace(5)
							tooltip.AutoAppendTextLine("%s: %s" % (localeInfo.LEFT_TIME, self.FormatTime(maxDuration - app.GetGlobalTimeStamp())), 0xffC5C7C4)
						tooltip.AppendSpace(5)
						tooltip.AlignHorizonalCenter()
					tooltip.ShowToolTip()
		else:
			if self.toolTipText:
				self.toolTipText.Show()

	def OnMouseOverOut(self):
		global overInImage
		overInImage = None

		# SET-ANIM-SCALE (Third Argument "Non-Present" its "IsScaleTemporary = False")
		if self.xScaleImage != 0 and self.yScaleImage != 0:
			self.SetScale(self.xScaleImage, self.yScaleImage)
		if self.itemImage:
			if self.itemImage.xScaleImage != 0 and self.itemImage.yScaleImage != 14:
				self.itemImage.SetScale(self.itemImage.xScaleImage, self.itemImage.yScaleImage)
		# SET-ANIM-SCALE

		if self.toolTip:
			self.toolTip.HideToolTip()
			return

		if app.ENABLE_VOTE4BUFF:
			if self.GetAffect() == chr.NEW_AFFECT_VOTE4BUFF:
				interface = constInfo.GetInterfaceInstance()
				if interface:
					if interface.tooltipItem:
						interface.tooltipItem.HideToolTip()
				return
		if app.ENABLE_COSTUME_SYSTEM:
			interface = constInfo.GetInterfaceInstance()
			if interface:
				if interface.tooltipItem:
					interface.tooltipItem.HideToolTip()
				if interface.tooltipSkill:
					interface.tooltipSkill.HideToolTip()
		else:
			if self.toolTipText:
				self.toolTipText.Hide()

class AffectShower(ui.Window):
	if app.USE_BOOST_POTIONS:
		BOOST_POTION_IDX_START = 65535

	MALL_DESC_IDX_START = 1000
	IMAGE_STEP = 25
	IMAGE_HEIGHT = 26

	COUNT_LINE = 14

	AFFECT_MAX_NUM = 32


	INFINITE_AFFECT_DURATION = 0x1FFFFFFF

	AFFECT_DATA_DICT =	{
			# chr.AFFECT_POISON : (localeInfo.SKILL_TOXICDIE, "d:/ymir work/ui/skill/common/affect/poison.sub"),
			# chr.AFFECT_SLOW : (localeInfo.SKILL_SLOW, "d:/ymir work/ui/skill/common/affect/slow.sub"),
			chr.AFFECT_STUN : (localeInfo.SKILL_STUN, "d:/ymir work/ui/skill/common/affect/stun.sub"),

			chr.AFFECT_ATT_SPEED_POTION : (localeInfo.SKILL_INC_ATKSPD, "flag/27102.tga"),
			chr.AFFECT_MOV_SPEED_POTION : (localeInfo.SKILL_INC_MOVSPD, "flag/27105.tga"),
			chr.AFFECT_FISH_MIND : (localeInfo.SKILL_FISHMIND, "d:/ymir work/ui/skill/common/affect/fishmind.sub"),

			chr.AFFECT_JEONGWI : (localeInfo.SKILL_JEONGWI, "d:/ymir work/ui/skill/warrior/jeongwi_03.sub",),
			chr.AFFECT_GEOMGYEONG : (localeInfo.SKILL_GEOMGYEONG, "d:/ymir work/ui/skill/warrior/geomgyeong_03.sub",),
			chr.AFFECT_CHEONGEUN : (localeInfo.SKILL_CHEONGEUN, "d:/ymir work/ui/skill/warrior/cheongeun_03.sub",),
			chr.AFFECT_GYEONGGONG : (localeInfo.SKILL_GYEONGGONG, "d:/ymir work/ui/skill/assassin/gyeonggong_03.sub",),
			chr.AFFECT_EUNHYEONG : (localeInfo.SKILL_EUNHYEONG, "d:/ymir work/ui/skill/assassin/eunhyeong_03.sub",),
			chr.AFFECT_GWIGEOM : (localeInfo.SKILL_GWIGEOM, "d:/ymir work/ui/skill/sura/gwigeom_03.sub",),
			chr.AFFECT_GONGPO : (localeInfo.SKILL_GONGPO, "d:/ymir work/ui/skill/sura/gongpo_03.sub",),
			chr.AFFECT_JUMAGAP : (localeInfo.SKILL_JUMAGAP, "d:/ymir work/ui/skill/sura/jumagap_03.sub"),
			chr.AFFECT_HOSIN : (localeInfo.SKILL_HOSIN, "d:/ymir work/ui/skill/shaman/hosin_03.sub",),
			
			chr.AFFECT_HOSIN_PERFECT : (localeInfo.SKILL_HOSIN, "d:/ymir work/ui/skill/shaman/hosin_03.sub",),
		
			chr.AFFECT_BOHO : (localeInfo.SKILL_BOHO, "d:/ymir work/ui/skill/shaman/boho_03.sub",),
			chr.AFFECT_BOHO_PERFECT : (localeInfo.SKILL_BOHO, "d:/ymir work/ui/skill/shaman/boho_03.sub",),
		
			chr.AFFECT_KWAESOK : (localeInfo.SKILL_KWAESOK, "d:/ymir work/ui/skill/shaman/kwaesok_03.sub",),
			chr.AFFECT_KWAESOK_PERFECT : (localeInfo.SKILL_KWAESOK, "d:/ymir work/ui/skill/shaman/kwaesok_03.sub",),
		
			chr.AFFECT_HEUKSIN : (localeInfo.SKILL_HEUKSIN, "d:/ymir work/ui/skill/sura/heuksin_03.sub",),
			chr.AFFECT_MUYEONG : (localeInfo.SKILL_MUYEONG, "d:/ymir work/ui/skill/sura/muyeong_03.sub",),
			chr.AFFECT_GICHEON : (localeInfo.SKILL_GICHEON, "d:/ymir work/ui/skill/shaman/gicheon_03.sub",),
			chr.AFFECT_GICHEON_PERFECT : (localeInfo.SKILL_GICHEON, "d:/ymir work/ui/skill/shaman/gicheon_03.sub",),
		
			
			chr.AFFECT_JEUNGRYEOK : (localeInfo.SKILL_JEUNGRYEOK, "d:/ymir work/ui/skill/shaman/jeungryeok_03.sub",),
			chr.AFFECT_JEUNGRYEOK_PERFECT : (localeInfo.SKILL_JEUNGRYEOK, "d:/ymir work/ui/skill/shaman/jeungryeok_03.sub",),
			chr.AFFECT_PABEOP : (localeInfo.SKILL_PABEOP, "d:/ymir work/ui/skill/sura/pabeop_03.sub",),
			chr.AFFECT_FALLEN_CHEONGEUN : (localeInfo.SKILL_CHEONGEUN, "d:/ymir work/ui/skill/warrior/cheongeun_03.sub",),
			28 : (localeInfo.SKILL_FIRE, "d:/ymir work/ui/skill/sura/hwayeom_03.sub",),
			chr.AFFECT_CHINA_FIREWORK : (localeInfo.SKILL_POWERFUL_STRIKE, "d:/ymir work/ui/skill/common/affect/powerfulstrike.sub",),

			#64 - END
			chr.NEW_AFFECT_EXP_BONUS : (localeInfo.TOOLTIP_MALL_EXPBONUS_STATIC, "d:/ymir work/ui/skill/common/affect/exp_bonus.sub",),

			# chr.NEW_AFFECT_ITEM_BONUS : (localeInfo.TOOLTIP_MALL_ITEMBONUS_STATIC, "d:/ymir work/ui/skill/common/affect/item_bonus.sub",),
			# chr.NEW_AFFECT_SAFEBOX : (localeInfo.TOOLTIP_MALL_SAFEBOX, "d:/ymir work/ui/skill/common/affect/safebox.sub",),
			# chr.NEW_AFFECT_AUTOLOOT : (localeInfo.TOOLTIP_MALL_AUTOLOOT, "d:/ymir work/ui/skill/common/affect/autoloot.sub",),
			# chr.NEW_AFFECT_FISH_MIND : (localeInfo.TOOLTIP_MALL_FISH_MIND, "d:/ymir work/ui/skill/common/affect/fishmind.sub",),
			# chr.NEW_AFFECT_MARRIAGE_FAST : (localeInfo.TOOLTIP_MALL_MARRIAGE_FAST, "d:/ymir work/ui/skill/common/affect/marriage_fast.sub",),
			# chr.NEW_AFFECT_GOLD_BONUS : (localeInfo.TOOLTIP_MALL_GOLDBONUS_STATIC, "d:/ymir work/ui/skill/common/affect/gold_bonus.sub",),

			chr.NEW_AFFECT_NO_DEATH_PENALTY : (localeInfo.TOOLTIP_APPLY_NO_DEATH_PENALTY, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			chr.NEW_AFFECT_SKILL_BOOK_BONUS : (localeInfo.TOOLTIP_APPLY_SKILL_BOOK_BONUS, "icon/item/71094.tga"),
			chr.NEW_AFFECT_SKILL_BOOK_NO_DELAY : (localeInfo.TOOLTIP_APPLY_SKILL_BOOK_NO_DELAY, "icon/item/71001.tga"),

			# ÀÚµ¿¹°¾à hp, sp
			chr.NEW_AFFECT_AUTO_HP_RECOVERY : (localeInfo.TOOLTIP_AUTO_POTION_REST, "icon/item/72725.tga"),
			chr.NEW_AFFECT_AUTO_SP_RECOVERY : (localeInfo.TOOLTIP_AUTO_POTION_REST, "icon/item/72729.tga"),
			#chr.NEW_AFFECT_AUTO_HP_RECOVERY : (localeInfo.TOOLTIP_AUTO_POTION_REST, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			#chr.NEW_AFFECT_AUTO_SP_RECOVERY : (localeInfo.TOOLTIP_AUTO_POTION_REST, "d:/ymir work/ui/skill/common/affect/gold_bonus.sub"),

			MALL_DESC_IDX_START+player.POINT_MALL_ATTBONUS : (localeInfo.TOOLTIP_MALL_ATTBONUS_STATIC, "d:/ymir work/ui/skill/common/affect/att_bonus.sub",),
			MALL_DESC_IDX_START+player.POINT_MALL_DEFBONUS : (localeInfo.TOOLTIP_MALL_DEFBONUS_STATIC, "d:/ymir work/ui/skill/common/affect/def_bonus.sub",),
			MALL_DESC_IDX_START+player.POINT_MALL_EXPBONUS : (localeInfo.TOOLTIP_MALL_EXPBONUS, "icon/item/71153.tga",),
			MALL_DESC_IDX_START+player.POINT_MALL_ITEMBONUS : (localeInfo.TOOLTIP_MALL_ITEMBONUS, "d:/ymir work/ui/skill/common/affect/item_bonus.sub",),
			MALL_DESC_IDX_START+player.POINT_MALL_GOLDBONUS : (localeInfo.TOOLTIP_MALL_GOLDBONUS, "d:/ymir work/ui/skill/common/affect/gold_bonus.sub",),
			MALL_DESC_IDX_START+player.POINT_CRITICAL_PCT : (localeInfo.TOOLTIP_APPLY_CRITICAL_PCT,"d:/ymir work/ui/skill/common/affect/critical.sub"),
			MALL_DESC_IDX_START+player.POINT_PENETRATE_PCT : (localeInfo.TOOLTIP_APPLY_PENETRATE_PCT, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			MALL_DESC_IDX_START+player.POINT_MAX_HP_PCT : (localeInfo.TOOLTIP_MAX_HP_PCT, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			MALL_DESC_IDX_START+player.POINT_MAX_SP_PCT : (localeInfo.TOOLTIP_MAX_SP_PCT, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),

			MALL_DESC_IDX_START+player.POINT_PC_BANG_EXP_BONUS : (localeInfo.TOOLTIP_MALL_EXPBONUS_P_STATIC, "d:/ymir work/ui/skill/common/affect/EXP_Bonus_p_on.sub",),
			MALL_DESC_IDX_START+player.POINT_PC_BANG_DROP_BONUS: (localeInfo.TOOLTIP_MALL_ITEMBONUS_P_STATIC, "d:/ymir work/ui/skill/common/affect/Item_Bonus_p_on.sub",),
	
			AFF_LEADERSHIP: ("Leadership", "d:/ymir work/ui/skill/common/affect/leadership.png",),
			AFF_SWITCHBOT_PREMIUM: ("Switchbot Premium", "d:/ymir work/ui/skill/common/affect/leadership.png",),

	}

	if app.ENABLE_VOTE4BUFF:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_VOTE4BUFF] =  (localeInfo.NEW_AFFECT_VOTE4BUFF, "icon/affectshower/vote4buff.tga")

	if app.ENABLE_MULTI_FARM_BLOCK:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_MULTI_FARM] =  (localeInfo.MULTI_FARM_PREMIUM_EFFECT, "d:/ymir work/ui/game/farm/can_farm_premimum.tga")
	if app.ENABLE_SUNG_MAHI_TOWER:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_SUNG_MAHI_CURSE] = (localeInfo.SUNG_MAHI_CURSE_TOOLTIP, "d:/ymir work/ui/skill/common/affect/sungmahee_tower_debuff.sub",)

	AFFECT_DATA_DICT[chr.NEW_AFFECT_THIEF] =  (localeInfo.AFFECT_THIEF, "icon/item/70043.tga")
	AFFECT_DATA_DICT[chr.NEW_AFFECT_EXP] =  (localeInfo.AFFECT_EXP, "icon/item/70005.tga")

	if app.USE_BOOST_POTIONS:
		AFFECT_DATA_DICT[chr.AFFECT_BOOST_POTION1] = (localeInfo.TOOLTIP_AFFECT_BOOST_POTION, "icon/blend/50821.tga")
		AFFECT_DATA_DICT[chr.AFFECT_BOOST_POTION2] = (localeInfo.TOOLTIP_AFFECT_BOOST_POTION, "icon/blend/50821.tga")
		AFFECT_DATA_DICT[chr.AFFECT_BOOST_POTION3] = (localeInfo.TOOLTIP_AFFECT_BOOST_POTION, "icon/blend/50821.tga")

	if app.ENABLE_DRAGON_SOUL_SYSTEM:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_DRAGON_SOUL_DECK1] = (localeInfo.TOOLTIP_DRAGON_SOUL_DECK1, "d:/ymir work/ui/dragonsoul/buff_ds_sky1.tga")
		AFFECT_DATA_DICT[chr.NEW_AFFECT_DRAGON_SOUL_DECK2] = (localeInfo.TOOLTIP_DRAGON_SOUL_DECK2, "d:/ymir work/ui/dragonsoul/buff_ds_land1.tga")
	
	if app.ENABLE_WOLFMAN_CHARACTER:
		AFFECT_DATA_DICT[chr.AFFECT_BLEEDING] = (localeInfo.SKILL_BLEEDING, "d:/ymir work/ui/skill/common/affect/poison.sub")
		AFFECT_DATA_DICT[chr.AFFECT_RED_POSSESSION] = ("Red Possession", "d:/ymir work/ui/skill/wolfman/red_possession_03.sub")
		AFFECT_DATA_DICT[chr.AFFECT_BLUE_POSSESSION] = ("Blue Possession", "d:/ymir work/ui/skill/wolfman/blue_possession_03.sub")
	if (app.WJ_COMBAT_ZONE):
		AFFECT_DATA_DICT[chr.NEW_AFFECT_COMBAT_ZONE_POTION] = (localeInfo.COMBAT_ZONE_TOOLTIP_BATTLE_POTION, "icon/item/27125.tga")

	if app.__BL_BATTLE_ROYALE__:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_BATTLE_ROYALE_SLOW] = (localeInfo.TOOLTIP_BATTLE_ROYALE_AFFECT_SLOW, "d:/ymir work/ui/skill/common/affect/slow.sub")
		AFFECT_DATA_DICT[chr.NEW_AFFECT_BATTLE_ROYALE_MOVE_SPEED] = (localeInfo.TOOLTIP_BATTLE_ROYALE_AFFECT_MOVE_SPEED, "d:/ymir work/ui/skill/common/affect/Increase_Move_Speed.sub")
		AFFECT_DATA_DICT[chr.NEW_AFFECT_BATTLE_ROYALE_INFINITE_STAMINA] = (localeInfo.TOOLTIP_BATTLE_ROYALE_AFFECT_INFINITE_STAMINA, "d:/ymir work/ui/skill/common/affect/exp_bonus.sub")

	if app.ENABLE_AUTOMATIC_PICK_UP_SYSTEM:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_AUTO_PICK_UP] = (localeInfo.NEW_AFFECT_AUTO_PICK_UP, "icon/item/70002.tga")

	if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM and\
		not app.USE_PREMIUM_AFFECT:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_AUTO_HUNT] = (localeInfo.TOOLTIP_AUTO_SYSTEM_PRIMIUM, "icon/item/otomatik_av.png")

	if app.USE_CRYSTAL_SYSTEM:
		AFFECT_DATA_DICT[chr.AFFECT_CRYSTAL] = (localeInfo.TOOLTIP_AFFECT_CRYSTAL, "icon/item/51010.tga")

	if app.USE_PASSIVE_ABILITY_TAMER:
		AFFECT_DATA_DICT[chr.AFFECT_SKILL_TAMER_BOOK_BONUS] = (localeInfo.TOOLTIP_AFFECT_SKILL_TAMER_BOOK_BONUS, "icon/item/tamer/50974.tga")
		AFFECT_DATA_DICT[chr.AFFECT_SKILL_TAMER_NO_BOOK_DELAY] = (localeInfo.TOOLTIP_AFFECT_SKILL_TAMER_NO_BOOK_DELAY, "icon/item/tamer/50973.tga")

	if app.USE_DROP_AFFECT_BONUSES:
		AFFECT_DATA_DICT[chr.AFFECT_DROP_GREEN] = (localeInfo.TOOLTIP_AFFECT_DROP_GREEN, "icon/item/doubledrop/64050.tga")
		AFFECT_DATA_DICT[chr.AFFECT_DROP_BLUE] = (localeInfo.TOOLTIP_AFFECT_DROP_BLUE, "icon/item/doubledrop/64051.tga")

	if app.USE_PREMIUM_AFFECT:
		AFFECT_DATA_DICT[chr.AFFECT_PREMIUM_USER] =  (localeInfo.TOOLTIP_AFFECT_PREMIUM_USER, "icon/item/premium/64052.tga")

	AFFECT_PET_DATA_DICT ={
		5401 : ("Rezistenza (Guerriero)", "d:/ymir work/ui/skill/pet/jijoong.sub"),
		5402 : ("Rezistenza (Sura)", "d:/ymir work/ui/skill/pet/jijoong.sub"),
		5403 : ("Rezistenza (Ninja)", "d:/ymir work/ui/skill/pet/jijoong.sub"),
		5404 : ("Rezistenza (Samano)", "d:/ymir work/ui/skill/pet/jijoong.sub"),
		5405 : ("Rezistenza (Lycan)", "d:/ymir work/ui/skill/pet/jijoong.sub"),
		5406 : ("Berserker", "d:/ymir work/ui/skill/pet/pacheon.sub"),
		5407 : ("Anti-Magia", "d:/ymir work/ui/skill/pet/cheonryeong.sub"),
		5408 : ("Accelerazione", "d:/ymir work/ui/skill/pet/banya.sub"),
		5409 : ("Perforazione", "d:/ymir work/ui/skill/pet/choehoenbimu.sub"),
		5410 : ("Rinnovo", "d:/ymir work/ui/skill/pet/heal.sub"),
		5411 : ("Vampirismo", "d:/ymir work/ui/skill/pet/stealhp.sub"),
		5412 : ("Fantasmi", "d:/ymir work/ui/skill/pet/stealmp.sub"),
		5413 : ("Intoppo", "d:/ymir work/ui/skill/pet/block.sub"),
		5414 : ("Specchio", "d:/ymir work/ui/skill/pet/reflect_melee.sub"),
		5415 : ("Drop Yang", "d:/ymir work/ui/skill/pet/gold_drop.sub"),
		5416 : ("Portata", "d:/ymir work/ui/skill/pet/bow_distance.sub"),
		5417 : ("Invincibilta", "d:/ymir work/ui/skill/pet/invincibility.sub"),
		5418 : ("Guarigione", "d:/ymir work/ui/skill/pet/removal.sub"),
	}
	
	if app.__RENEWAL_BRAVE_CAPE__:
		AFFECT_DATA_DICT[chr.AFFECT_NAME_PET] = (localeInfo.TOOLTIP_APPLY_ATT_STONE, "icon/item/94144.png")
		AFFECT_DATA_DICT[chr.AFFECT_NAME_MOUNT] = (localeInfo.TOOLTIP_APPLY_ATTBONUS_MONSTER, "icon/item/94145.png")
		AFFECT_DATA_DICT[chr.AFFECT_NAME_BUFFI] = (localeInfo.TOOLTIP_APPLY_ATT_BOSS, "icon/item/61030.tga")

	# if app.__BUFFI_SUPPORT__:
		# AFFECT_DATA_DICT[chr.AFFECT_BUFFI_HOSIN] = ("Buffi "+localeInfo.SKILL_HOSIN, "d:/ymir work/ui/skill/shaman/hosin" + END_STRING + ".sub", 2, 0)
		# AFFECT_DATA_DICT[chr.AFFECT_BUFFI_GICHEON] = ("Buffi "+localeInfo.SKILL_GICHEON, "d:/ymir work/ui/skill/shaman/gicheon" + END_STRING + ".sub", 2, 0)
		# AFFECT_DATA_DICT[chr.AFFECT_BUFFI_KWAESOK] = ("Buffi "+localeInfo.SKILL_KWAESOK, "d:/ymir work/ui/skill/shaman/kwaesok" + END_STRING + ".sub", 2, 0)
		# AFFECT_DATA_DICT[chr.AFFECT_BUFFI_JEUNGRYEO] = ("Buffi "+localeInfo.SKILL_JEUNGRYEOK, "d:/ymir work/ui/skill/shaman/jeungryeok" + END_STRING + ".sub", 2, 0)
	AFFECT_DATA_DICT[chr.NEW_AFFECT_BRAVE_CAPE] = (localeInfo.NEW_AFFECT_BRAVE_CAPE, "icon/item/70038_1.tga")

	if app.__BUFFI_SUPPORT__:
		AFFECT_DATA_DICT[chr.AFFECT_BUFFI_HOSIN] = ("Buffi "+localeInfo.SKILL_HOSIN, "d:/ymir work/ui/skill/shaman/hosin_03.sub")
		AFFECT_DATA_DICT[chr.AFFECT_BUFFI_GICHEON] = ("Buffi "+localeInfo.SKILL_GICHEON, "d:/ymir work/ui/skill/shaman/gicheon_03.sub")
		AFFECT_DATA_DICT[chr.AFFECT_BUFFI_KWAESOK] = ("Buffi "+localeInfo.SKILL_KWAESOK, "d:/ymir work/ui/skill/shaman/kwaesok_03.sub")
		AFFECT_DATA_DICT[chr.AFFECT_BUFFI_JEUNGRYEO] = ("Buffi "+localeInfo.SKILL_JEUNGRYEOK, "d:/ymir work/ui/skill/shaman/jeungryeok_03.sub")
	if app.VOTE_BUFF_RENEWAL:
		AFFECT_DATA_DICT[chr.AFFECT_VOTEFORBONUS] =  (localeInfo.NEW_AFFECT_VOTE4BUFF, "icon/affectshower/vote4buff.tga")

	def __del__(self):
		ui.Window.__del__(self)

	def Destroy(self):
		self.ClearAllAffects()
		self.serverPlayTime=0
		self.clientPlayTime=0

		self.lastUpdateTime=0
		if app.ENABLE_COSTUME_SYSTEM:
			global affectDict, overInImage
			affectDict={}
			overInImage=None
		# self.horseImage=None
		self.mountImage=None
		self.lovePointImage=None
		if app.ENABLE_YOHARA_SYSTEM:
			self.SungmaImage = None

		if app.ENABLE_MULTI_FARM_BLOCK:
			self.multiFarmBlockDialog=None
			self.farmStatusImage=None
	def __init__(self):
		ui.Window.__init__(self)
		self.affectImageDict = {}

		self.Destroy()
		self.SetPosition(10, 10)
		self.Show()
		if app.__NEW_SET_BONUS__:
			self.setBonusImage=NewSetBonus(self)
		self.SetLeaderShipStatus(False)

	if app.__NEW_SET_BONUS__:
		def CheckSetBonus(self):
			if self.setBonusImage.IsCanShow():
				self.setBonusImage.Show()
			else:
				self.setBonusImage.Hide()

	def ClearAllAffects(self):
		# self.horseImage=None
		self.mountImage=None
		self.lovePointImage=None
		if app.__NEW_SET_BONUS__:
			self.setBonusImage=None
		if app.ENABLE_MULTI_FARM_BLOCK:
			self.multiFarmBlockDialog=None
			self.farmStatusImage=None
		if app.ENABLE_YOHARA_SYSTEM:
			self.SungmaImage = None

		self.affectImageDict={}
		self.__ArrangeImageList()

	def ClearAffects(self):
		self.living_affectImageDict=OrderedDict()
		for key, image in self.affectImageDict.items():
			if not image.IsSkillAffect():
				self.living_affectImageDict[key] = image
		self.affectImageDict = self.living_affectImageDict
		self.__ArrangeImageList()
		
	def SetLeaderShipStatus(self, status, pointIdx = 0, value = 0):
		if not self.affectImageDict.has_key(AFF_LEADERSHIP):
			self.BINARY_NEW_AddAffect(AFF_LEADERSHIP, 0, 0, 0)
		
		ACTIVE_IMG = "d:/ymir work/ui/skill/common/affect/leadership.png"
		DEACTIVE_IMG = "d:/ymir work/ui/skill/common/affect/leadership.png"

		if self.affectImageDict[AFF_LEADERSHIP].toolTip == None:
			self.affectImageDict[AFF_LEADERSHIP].toolTip = uitooltip.ToolTip()
			self.affectImageDict[AFF_LEADERSHIP].toolTip.HideToolTip()
		
		toolTip = self.affectImageDict[AFF_LEADERSHIP].toolTip
		
		toolTip.ClearToolTip()
		toolTip.SetTitle(localeInfo.LEADERSHIP_TITLE)
		toolTip.AppendDescription(localeInfo.LEADERSHIP_DESCRIPTION, 26)
		toolTip.AppendSpace(5)
		toolTip.AppendTextLine("--------------------------")
		toolTip.AppendTextLine(localeInfo.LEADER_BONUS_RECIVED)
		
		if status == 1:
			AFFECT_STRING_DICT = {
				91 : localeInfo.PARTY_BONUS_ATTACKER,
				92 : localeInfo.PARTY_BONUS_TANKER,
				103 : localeInfo.PARTY_BONUS_BUFFER,
				104 : localeInfo.PARTY_BONUS_SKILL_MASTER,
				110 : localeInfo.PARTY_BONUS_BERSERKER,
				111 : localeInfo.PARTY_BONUS_DEFENDER,
			}
			
			self.affectImageDict[AFF_LEADERSHIP].SetItemImage(ACTIVE_IMG)
			# self.affectImageDict[AFF_LEADERSHIP].SetScale(0.7, 0.7)
			
			if AFFECT_STRING_DICT.has_key(pointIdx):
				toolTip.AppendTextLine(AFFECT_STRING_DICT[pointIdx](value))
			
			toolTip.AppendSpace(5)
			toolTip.AppendTextLine("--------------------------")
			toolTip.AppendTextLine("Active")

		elif status == 0:
			self.affectImageDict[AFF_LEADERSHIP].SetItemImage(DEACTIVE_IMG)
			# self.affectImageDict[AFF_LEADERSHIP].SetScale(0.7, 0.7)
			
			toolTip.AppendSpace(5)
			toolTip.AppendTextLine("--------------------------")
			toolTip.AppendTextLine("Deactive")
	
		
	if app.ENABLE_IKASHOP_RENEWAL:
		if app.EXTEND_IKASHOP_ULTIMATE:
			def SetSearchShopBoard(self, board):
				pass

	#if app.USE_BOOST_POTIONS or app.USE_CRYSTAL_SYSTEM @cell
	def BINARY_NEW_AddAffect(self, type, pointIdx, value, duration, cell = -1):
		affect = 0

		if type == chr.NEW_AFFECT_MALL:
			affect = self.MALL_DESC_IDX_START + pointIdx
		elif app.USE_BOOST_POTIONS and constInfo.IS_BOOST_POTION_AFFECT(type):
			affect = self.BOOST_POTION_IDX_START + (type * 4) + pointIdx
		elif app.ENABLE_AUTOMATIC_HUNTING_SYSTEM and\
			app.USE_PREMIUM_AFFECT and\
			type == chr.NEW_AFFECT_AUTO_USE:
			return
		else:
			affect = type

		if app.ENABLE_AUTOMATIC_HUNTING_SYSTEM and\
			not app.USE_PREMIUM_AFFECT:
			if type == chr.NEW_AFFECT_AUTO_HUNT:
				import chrmgr
				if not chrmgr.GetAutoOnOff():
					return

		if duration > 0 and affect == chr.AFFECT_VOTEFORBONUS:
			constInfo.AFFECT_V4B = app.GetGlobalTimeStamp() + duration

		if app.ENABLE_VOTE4BUFF:
			if type == chr.NEW_AFFECT_VOTE4BUFF:
				global voteBuffData
				voteBuffData = [pointIdx, value]

		if app.ENABLE_COSTUME_SYSTEM:
			if (app.USE_CRYSTAL_SYSTEM and type == chr.AFFECT_CRYSTAL) or\
				(app.USE_BOOST_POTIONS and constInfo.IS_BOOST_POTION_AFFECT(type)) or\
				(app.USE_PREMIUM_AFFECT and type == chr.AFFECT_PREMIUM_USER):
				pass
			else:
				global affectDict
				affectNew = self.AffectToRealIndex(affect)
				affectData = {}
				affectData = affectDict[affectNew] if affectDict.has_key(affectNew) else {}
				affectList = []
				affectList = affectData["affect"] if affectData.has_key("affect") else []

				maxDuration = 0
				maxDuration = affectData["duration"] if affectData.has_key("duration") else 0

				if not [pointIdx, value] in affectList:
					affectList.append([pointIdx, value])

				if duration+app.GetGlobalTimeStamp() > maxDuration:
					affectData["duration"] = duration+app.GetGlobalTimeStamp()

				affectData["affect"] = affectList
				affectDict[affectNew] = affectData

		if affect == chr.NEW_AFFECT_NO_DEATH_PENALTY or affect == chr.NEW_AFFECT_SKILL_BOOK_BONUS or affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY or affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY or affect == chr.NEW_AFFECT_SKILL_BOOK_NO_DELAY:
			duration = 0
		elif app.USE_PASSIVE_ABILITY_TAMER and (affect == chr.AFFECT_SKILL_TAMER_BOOK_BONUS or affect == chr.AFFECT_SKILL_TAMER_NO_BOOK_DELAY):
			duration = 0

		if not self.AFFECT_DATA_DICT.has_key(affect):
			if app.USE_BOOST_POTIONS and constInfo.IS_BOOST_POTION_AFFECT(type):
				pass
			else:
				return

		if app.USE_BOOST_POTIONS and constInfo.IS_BOOST_POTION_AFFECT(type):
			affectData = self.AFFECT_DATA_DICT[type]
		else:
			affectData = self.AFFECT_DATA_DICT[affect]

		if self.affectImageDict.has_key(affect):
			return

		description = affectData[0]
		filename = affectData[1]

		if pointIdx == player.POINT_MALL_ITEMBONUS or pointIdx == player.POINT_MALL_GOLDBONUS:
			value = 1 + float(value) / 100.0
		try:
			description = description(float(value))
		except:
			try:
				description = description % float(value)
			except:
				pass

		image = AffectImage()

		if (app.USE_CRYSTAL_SYSTEM and type == chr.AFFECT_CRYSTAL) or\
			(app.USE_BOOST_POTIONS and constInfo.IS_BOOST_POTION_AFFECT(type)):
			image.SetCell(cell)

		image.SetParent(self)
		image.LoadImage("icon/blend/50821.tga")
		image.SetItemImage(filename)
		image.SetDescription(description)
		image.SetDuration(duration)

		if app.USE_BOOST_POTIONS and constInfo.IS_BOOST_POTION_AFFECT(type):
			image.SetAffect(type)
		else:
			image.SetAffect(affect)

		if duration > 0 and affect == chr.AFFECT_VOTEFORBONUS:
			constInfo.AFFECT_V4B = app.GetGlobalTimeStamp() + duration

		isDSAffect = (affect == chr.NEW_AFFECT_DRAGON_SOUL_DECK1 or affect == chr.NEW_AFFECT_DRAGON_SOUL_DECK2)
		scaleDS = 0.7
		scaleScadere = (float(image.GetWidth()) / float(30) * 1.0) - 1.0
		scaleNormal = 0.8
		if image.GetWidth() >= 32:
			scaleNormal = scaleNormal - scaleScadere

		image.SetScale(scaleDS if isDSAffect else scaleNormal, scaleDS if isDSAffect else scaleNormal)
		image.Show()

		self.affectImageDict[affect] = image
		# self.__ArrangeImageList()

	def BINARY_NEW_RemoveAffect(self, type, pointIdx):
		if type == chr.NEW_AFFECT_MALL:
			affect = self.MALL_DESC_IDX_START + pointIdx
		elif app.USE_BOOST_POTIONS and constInfo.IS_BOOST_POTION_AFFECT(type):
			affect = self.BOOST_POTION_IDX_START + (type * 4) + pointIdx
		else:
			affect = type

		if app.ENABLE_COSTUME_SYSTEM:
			if (app.USE_CRYSTAL_SYSTEM and type == chr.AFFECT_CRYSTAL) or\
				(app.USE_BOOST_POTIONS and constInfo.IS_BOOST_POTION_AFFECT(type)):
				pass
			else:
				global affectDict
				affectNew = self.AffectToRealIndex(affect)
				affectData = affectDict[affectNew] if affectDict.has_key(affectNew) else {}
				affectList = affectData["affect"] if affectData.has_key("affect") else []
				removeIndex = -1
				for data in affectList:
					if data[0] == pointIdx:
						removeIndex = affectList.index(data)
						break
				if removeIndex != -1:
					del affectList[removeIndex]
					affectData["affect"] = affectList
					affectDict[affectNew] = affectData

		if affect == AFF_LEADERSHIP:
			self.SetLeaderShipStatus(False)
			return

		self.__RemoveAffect(affect)
		#self.__ArrangeImageList()

	def BINARY_NEW_RefreshAffect(self):
		self.__ArrangeImageList()

	def SetAffect(self, affect):
		self.__AppendAffect(affect)
		self.__ArrangeImageList()

	def ResetAffect(self, affect):
		self.__RemoveAffect(affect)
		self.__ArrangeImageList()

	def SetLoverInfo(self, name, lovePoint):
		image = LovePointImage()
		image.SetParent(self)
		image.SetLoverInfo(name, lovePoint)
		self.lovePointImage = image
		self.__ArrangeImageList()

	def ShowLoverState(self):
		if self.lovePointImage:
			self.lovePointImage.Show()
			self.__ArrangeImageList()

	def HideLoverState(self):
		if self.lovePointImage:
			self.lovePointImage.Hide()
			self.__ArrangeImageList()

	def ClearLoverState(self):
		self.lovePointImage = None
		self.__ArrangeImageList()

	def OnUpdateLovePoint(self, lovePoint):
		if self.lovePointImage:
			self.lovePointImage.OnUpdateLovePoint(lovePoint)

	# def SetHorseState(self, level, health, battery):
		# if level==0:
			# self.horseImage=None
			# self.__ArrangeImageList()
		# else:
			# image = HorseImage()
			# image.SetParent(self)
			# image.SetState(level, health, battery)
			# image.Show()

			# self.horseImage=image
			# self.__ArrangeImageList()

	if app.ENABLE_YOHARA_SYSTEM:
		def SetSungMaAffectImage(self, str, hp, move, inmune):
			image = SungMaAffectImage(str, hp, move, inmune)
			image.SetParent(self)
			image.SetSungmaValue(str, hp, move, inmune)
			image.Show()
			
			self.SungmaImage=image
			self.__ArrangeImageList()

	def SetPlayTime(self, playTime):
		self.serverPlayTime = playTime
		self.clientPlayTime = app.GetTime()

	def __AppendAffect(self, affect):
		if self.affectImageDict.has_key(affect):
			return

		try:
			affectData = self.AFFECT_DATA_DICT[affect]
		except KeyError:
			return

		name = affectData[0]
		filename = affectData[1]

		skillIndex = player.AffectIndexToSkillIndex(affect)
		if 0 != skillIndex:
			name = skill.GetSkillName(skillIndex)

		image = AffectImage()
		image.SetParent(self)
		image.LoadImage("icon/blend/50821.tga")
		image.SetItemImage(filename)
		
		# chat.AppendChat(1, "Affect is %d" % affect)
		
		image.SetSkillAffectFlag(True)

		if app.ENABLE_AFFECT_BUFF_REMOVE:
			image.SetSkillIndex(skillIndex)
				
		try:
			image.LoadImage("icon/blend/50821.tga")
			image.SetItemImage(filename)
		except:
			pass

		if app.ENABLE_COSTUME_SYSTEM:
			image.SetAffect(affect)
			image.SetDescription(name)
				
		if image.IsSkillAffect():
			image.SetScale(0.7, 0.7)
			
		image.Show()
		self.affectImageDict[affect] = image

	def __RemoveAffect(self, affect):
		if not self.affectImageDict.has_key(affect):
			return

		self.affectImageDict[affect].itemImage = None
		del self.affectImageDict[affect]

		self.__ArrangeImageList()

	def __ArrangeImageList(self):
		width = len(self.affectImageDict) * self.IMAGE_STEP

		if app.ENABLE_YOHARA_SYSTEM:
			if self.SungmaImage:
				width+=self.IMAGE_STEP
				
		if self.lovePointImage:
			width += self.IMAGE_STEP

		if app.ENABLE_MULTI_FARM_BLOCK:
			if self.farmStatusImage:
				width+=self.IMAGE_STEP

		if app.__NEW_SET_BONUS__:
			if self.setBonusImage:
				width+=self.IMAGE_STEP

		# if self.horseImage:
			# width += self.IMAGE_STEP


		self.SetSize(width, 26)

		xPos = 0
		yPos = 0
		AffectCount = 0
		
		if app.ENABLE_MULTI_FARM_BLOCK:
			if self.farmStatusImage:
				if self.farmStatusImage.IsShow():
					self.farmStatusImage.SetPosition(xPos, 0)
					xPos += self.IMAGE_STEP
					AffectCount += 1

		if app.ENABLE_YOHARA_SYSTEM:
			if self.SungmaImage:
				if self.SungmaImage.IsShow():
					self.SungmaImage.SetPosition(xPos, 0)
					xPos += self.IMAGE_STEP
					AffectCount += 1

		if self.lovePointImage:
			if self.lovePointImage.IsShow():
				self.lovePointImage.SetPosition(xPos, 0)
				xPos += self.IMAGE_STEP
				# if constInfo.SHOW_AFFECT_TOOLTIP:
				AffectCount += 1

		if self.setBonusImage:
			if self.setBonusImage.IsShow():
				self.setBonusImage.SetPosition(xPos, 0)
				self.setBonusImage.SetScale(0.7, 0.7)
				xPos += self.IMAGE_STEP
				# if constInfo.SHOW_AFFECT_TOOLTIP:
				AffectCount += 1

		# if self.horseImage:
			# self.horseImage.SetPosition(xPos, 0)
			# xPos += self.IMAGE_STEP
			# if constInfo.SHOW_AFFECT_TOOLTIP:
				# AffectCount += 1

		for image in self.affectImageDict.values():
			image.SetPosition(xPos, yPos)
			AffectCount += 1
			xPos += self.IMAGE_STEP

			if AffectCount % 11 == 0:
				yPos += self.IMAGE_STEP
				xPos = 0

				self.SetSize(12 * self.IMAGE_STEP, yPos + self.IMAGE_STEP)

	if app.ENABLE_MULTI_FARM_BLOCK:
		def __OnClickMultiFarm(self, emptyArg):
			multiFarmBlockDialog = uiCommon.QuestionDialog()
			if self.farmStatusImage.multiFarmStatus:
				multiFarmBlockDialog.SetText(localeInfo.MULTI_FARM_DEACTIVE_TEXT)
			else:
				multiFarmBlockDialog.SetText(localeInfo.MULTI_FARM_ACTIVE_TEXT)
			multiFarmBlockDialog.SetAcceptEvent(lambda arg = True: self.OnCloseMultiFarm(arg))
			multiFarmBlockDialog.SetCancelEvent(lambda arg = False: self.OnCloseMultiFarm(arg))
			multiFarmBlockDialog.Open()
			self.multiFarmBlockDialog = multiFarmBlockDialog
		def OnCloseMultiFarm(self, answer):
			if not self.multiFarmBlockDialog:
				return
			if answer:
				net.SendChatPacket("/multi_farm")
			self.multiFarmBlockDialog.Close()
			self.multiFarmBlockDialog = None
		def SetMultiFarmPlayer(self, playerName):
			if self.farmStatusImage:
				self.farmStatusImage.players.append(playerName)
		def SetMultiFarmInfo(self, farmStatus):
			image = MultiFarmImage()
			image.SetParent(self)
			image.SetMultiFarmInfo(farmStatus)
			image.SetEvent(ui.__mem_func__(self.__OnClickMultiFarm),"mouse_click")
			image.Show()
			self.farmStatusImage = image
			self.__ArrangeImageList()


	def OnUpdate(self):
		try:

			if app.GetGlobalTime() - self.lastUpdateTime > 500:
				self.lastUpdateTime = app.GetGlobalTime()
				global overInImage
				if overInImage:
					overInImage.OnMouseOverIn()
		except Exception, e:
			return
			
	if app.ENABLE_COSTUME_SYSTEM:
		def AffectToRealIndex(self, aff):
			aaffDict = {
				209:chr.AFFECT_POISON,
				211:chr.AFFECT_SLOW,
				210:chr.AFFECT_STUN,
				201:chr.AFFECT_ATT_SPEED_POTION,
				200:chr.AFFECT_MOV_SPEED_POTION,
				208:chr.AFFECT_FISH_MIND,
			}
			if aff in aaffDict:
				return aaffDict[aff]
			return aff
		def CheckRemoveAffect(self, type):
			if type == chr.NEW_AFFECT_POLYMORPH:
				return True
			return True if TypeToAffect(type) != type else False

if app.__NEW_SET_BONUS__:
	import player, item
	class NewSetBonus(ui.ExpandedImageBox):
		def __init__(self, parent):
			ui.ExpandedImageBox.__init__(self)
			self.SetParent(parent)
			self.LoadImage("icon/affectshower/set_bonus.tga")
			self.SetScale(0.7, 0.7)

			self.bonusSetDataDict = {
				"SetBonus" : {
					"slots" : {
						item.COSTUME_SLOT_BODY : [41324, 41325, 41311, 41312, 49049, 49050, 49286, 49287, 49060, 49061, 49073, 49073, 49086, 49087, 49106, 49107, 49116, 49117, 49126, 49128, 41003, 41004, 11971, 11972, 11973, 11974, 49136, 49137, 34042, 34043, 49127],
						item.COSTUME_SLOT_HAIR : [45160, 45161, 45148, 45147, 49051, 49052, 49288, 49289, 49062, 49063, 49075, 49075, 49088, 49089, 49108, 49109, 49118, 49119, 49128, 49129, 49138, 49139, 45305, 45306],
						204 : [40457, 40460, 40458, 40461, 40462, 40459, 40101, 40104, 40103, 40105, 40106, 49130, 49131, 49132, 49133, 49134, 49135, 40424, 40425, 40426, 40427, 40428, 40429, 49043, 49044, 49045, 49046, 49047, 49048, 49280, 49281, 49282, 49283, 49284, 49285, 49054, 49055, 49056, 49057, 49058, 49059, 49067, 49068, 49069, 49070, 49071, 49071, 49080, 49081, 49082, 49083, 49084, 49085, 49100, 49101, 49102, 49103, 49104, 49105, 49110, 49111, 49112, 49113, 49114, 49115, 49121, 49122, 49123, 49124, 49125, 49120],
						207 : [86065, 86063, 86067, 86004, 86048, 86035, 86036, 86037, 86062, 87013],
						item.SLOT_MOUNT_SKIN : [71247, 71246, 71245, 71235, 71226, 71227, 71228, 71229, 71230, 49313],
					},
					"partCountToBonusCount" : {
						0 : 0,
						1 : 1,
						2 : 1,
						3 : 2,
						4 : 2,
						5 : 3,
					},
					"bonus" : {
						1 : [item.APPLY_MAX_HP, 1000],
						2 : [item.APPLY_ATTBONUS_MONSTER, 8],
						3 : [item.APPLY_ATTBONUS_STONE, 8],
					},
				},
				# "SetBonus - 2" : {
					# "slots" : {
						# item.EQUIPMENT_WEAPON : 3159,
						# item.EQUIPMENT_BODY : 11299,
						# item.EQUIPMENT_HEAD : 12249,
						# item.EQUIPMENT_SHIELD : 13049,
						# item.EQUIPMENT_WRIST : 14109,
						# item.EQUIPMENT_SHOES : 15189,
						# item.EQUIPMENT_NECK : 16109,
						# item.EQUIPMENT_EAR : 17109,
						
					# },
					# "partCountToBonusCount" : {
						# 0 : 0,
						# 1 : 0,
						# 2 : 1,
						# 3 : 2,
						# 4 : 3,
						# 5 : 3,
						# 6 : 4,
						# 7 : 4,
						# 8 : 5,
					# },
					# "bonus" : {
						# 1 : [item.APPLY_MAX_HP, 3000],
						# 2 : [item.APPLY_ATTBONUS_MONSTER, 20],
						# 3 : [item.APPLY_ATTBONUS_STONE, 20],
						# 4 : [item.APPLY_ATTBONUS_BOSS, 20],
						# 5 : [item.APPLY_ATTBONUS_HUMAN, 20],
					# },
				# },
			}

		def IsCanShow(self):
			# bonusSetDataDict = self.bonusSetDataDict
			# for setBonusName, setBonusData in bonusSetDataDict.items():
				# for slotIdx, itemIdx in setBonusData["slots"].items():
					# if player.GetItemIndex(slotIdx) == itemIdx:
						# return True
			return True
			
		def OnMouseOverIn(self):
			interface = constInfo.GetInterfaceInstance()
			if interface:
				tooltipItem = interface.tooltipItem
				if tooltipItem:
					tooltipItem.ClearToolTip()

					bonusSetDataDict = self.bonusSetDataDict

					slotToItemName = {
						item.COSTUME_SLOT_BODY: "Costume Body",
						item.COSTUME_SLOT_HAIR: "Costume Hair",
						204: "Costume Weapon",
						207: "Costume Sash",
						item.SLOT_MOUNT_SKIN: "Costume Mount",
					}

					if not bonusSetDataDict:
						tooltipItem.AutoAppendTextLineSpecial("No set bonus config in the game.", 0xFFE57875)
						tooltipItem.ShowToolTip()
						return

					tooltipItem.AutoAppendTextLineSpecial("Set Bonus", 0xFFF1E6C0)
					tooltipItem.AppendSpace(5)

					x = 0
					for setBonusName, setBonusData in bonusSetDataDict.items():
						partCount = 0
						partNames = []

						for slotIdx, itemIdxList in setBonusData["slots"].items():
							if player.GetItemIndex(slotIdx) in itemIdxList:
								partCount += 1
								partNames.append(slotToItemName.get(slotIdx, "Unknown"))

						if not partNames:
							continue 

						if x != 0:
							tooltipItem.AppendSpace(30)

						tooltipItem.AutoAppendTextLineSpecial(setBonusName, 0xFFF1E6C0, False)
						tooltipItem.AppendSpace(5)

						if partCount == 0:
							tooltipItem.AutoAppendTextLine("Not wearing any outfit.", 0xFFE57875, False)
						else:
							for name in partNames:
								tooltipItem.AutoAppendTextLineSpecial(name, 0xFF89B88D, False)

						tooltipItem.AppendSpace(5)

						tooltipItem.AutoAppendTextLineSpecial("Bonus", 0xFFF1E6C0, False)
						tooltipItem.AppendSpace(5)

						bonusCount = setBonusData["partCountToBonusCount"].get(partCount, 0)
						if bonusCount:
							for j in range(1, 6):
								if not bonusCount:
									break
								bonusData = setBonusData["bonus"].get(j)
								if bonusData:
									tooltipItem.AutoAppendTextLineSpecial(tooltipItem.AFFECT_DICT[bonusData[0]](bonusData[1]), 0xFFB0DFB4, False)
								bonusCount -= 1
						else:
							tooltipItem.AutoAppendTextLineSpecial("No active bonus.", 0xFFE57875, False)

						x += 1

					tooltipItem.ShowToolTip()


		def OnMouseOverOut(self):
			interface = constInfo.GetInterfaceInstance()
			if interface and interface.tooltipItem:
				interface.tooltipItem.HideToolTip()
