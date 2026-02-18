#author: dracaryS - 8.12.22

# static
import ui, uiToolTip, constInfo, localeInfo, math

#dynamic
import item, dbg, player, app, chat, net

IMG_DIR = "d:/ymir work/ui/game/skill_tree/"
ICON_IMG_DIR = "d:/ymir work/ui/game/skill_tree/icon/"
DIRECTION_IMG_DIR = "d:/ymir work/ui/game/skill_tree/direction/"

MIN_LEVEL = 90
REFINE_DECREASE_ITEM = 72347

def GetBonusName(affectType, affectValue):
	return uiToolTip.ItemToolTip.AFFECT_DICT[affectType](affectValue) if uiToolTip.ItemToolTip.AFFECT_DICT.has_key(affectType) else "UNKNOWN_TYPE[%d] %d" % (affectType, affectValue)

refillList = [[50255, 1000], [30618, 2],[91116, 15]]

refine_dict = {
	0 : {
		"items": [[31107, 2000], [91116, 10],[91117, 20], [30005, 1000], [70036, 10]],
		"yang": 100000000,
	},
	1 : {
		"items": [[31108, 2000], [91116, 15],[91117, 25], [30005, 2000], [70036, 15]],
		"yang": 250000000,
	},
	2 : {
		"items": [[31109, 2000], [30618, 2],[50255, 250], [30630, 20], [30621, 20]],
		"yang": 500000000,
	},
	3 : {
		"items": [[31114, 2000], [30618, 5],[50255, 500], [30620, 20], [30622, 20]],
		"yang": 750000000,
	},
	4 : {
		"items": [[31114, 2000], [30618, 7],[31257, 50], [31258, 50], [70428, 50]],
		"yang": 1000000000,
	},
}

data_dict = {
	0 : {
		"bonus": item.APPLY_MAX_HP,
		"value" : [500, 1000, 1500, 2000, 2500],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (340, 283),
		"icon_idx": "0",
		"treeConnect" : 0,
		"skillConnect" : -1,
		"serverRefineLevel": 0,
	},
	1 : {
		"bonus": item.APPLY_MAX_HP,
		"value" : [500, 1000, 1500, 2000, 2500],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (244, 309),
		"icon_idx": "0",
		"treeConnect" : 1,
		"skillConnect" : 0,
		"serverRefineLevel": 0,
	},
	2 : {
		"bonus": item.APPLY_ATTBONUS_WARRIOR,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (174, 309),
		"icon_idx": "2",
		"treeConnect" : -1,
		"skillConnect" : 1,
		"serverRefineLevel": 0,
	},
	3 : {
		"bonus": item.APPLY_RESIST_WARRIOR,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (204, 223),
		"icon_idx": "3",
		"treeConnect" : -1,
		"skillConnect" : 1,
		"serverRefineLevel": 0,
	},
	4 : {
		"bonus": item.APPLY_ATTBONUS_SURA,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (117, 265),
		"icon_idx": "4",
		"treeConnect" : -1,
		"skillConnect" : 1,
		"serverRefineLevel": 0,
	},
	5 : {
		"bonus": item.APPLY_RESIST_SURA,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (149, 179),
		"icon_idx": "5",
		"treeConnect" : -1,
		"skillConnect" : 1,
		"serverRefineLevel": 0,
	},
	6 : {
		"bonus": item.APPLY_ATTBONUS_ASSASSIN,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (62, 218),
		"icon_idx": "6",
		"treeConnect" : -1,
		"skillConnect" : 1,
		"serverRefineLevel": 0,
	},
	7 : {
		"bonus": item.APPLY_RESIST_ASSASSIN,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (94, 133),
		"icon_idx": "7",
		"treeConnect" : -1,
		"skillConnect" : 1,
		"serverRefineLevel": 0,
	},
	8 : {
		"bonus": item.APPLY_ATTBONUS_SHAMAN,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (7, 170),
		"icon_idx": "8",
		"treeConnect" : -1,
		"skillConnect" : 1,
		"serverRefineLevel": 0,
	},
	9 : {
		"bonus": item.APPLY_RESIST_SHAMAN,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (37, 88),
		"icon_idx": "9",
		"treeConnect" : -1,
		"skillConnect" : 1,
		"serverRefineLevel": 0,
	},
	10 : {
		"bonus": item.APPLY_GOLD_DOUBLE_BONUS,
		"value" : [2, 4, 8, 10, 15],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (433, 304),
		"icon_idx": "10",
		"treeConnect" : 2,
		"skillConnect" : 0,
		"serverRefineLevel": 0,
	},
	11 : {
		"bonus": item.APPLY_CON,
		"value" : [4, 6, 8, 10, 12],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (489, 276),
		"icon_idx": "11",
		"treeConnect" : 3,
		"skillConnect" : 10,
		"serverRefineLevel": 0,
	},
	12 : {
		"bonus": item.APPLY_DEX,
		"value" : [4, 6, 8, 10, 12],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (521, 339),
		"icon_idx": "12",
		"treeConnect" : -1,
		"skillConnect" : 11,
		"serverRefineLevel": 0,
	},
	13 : {
		"bonus": item.APPLY_STR,
		"value" : [4, 6, 8, 10, 12],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (553, 240),
		"icon_idx": "13",
		"treeConnect" : 4,
		"skillConnect" : 11,
		"serverRefineLevel": 0,
	},
	14 : {
		"bonus": item.APPLY_ITEM_DROP_BONUS,
		"value" : [2, 4, 8, 10, 15],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (614, 205),
		"icon_idx": "14",
		"treeConnect" : 5,
		"skillConnect" : 13,
		"serverRefineLevel": 0,
	},
	15 : {
		"bonus": item.APPLY_RESIST_ICE,#This should resistance for boss!
		"value" : [4, 7, 10, 12, 15],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (646, 289),
		"icon_idx": "15",
		"treeConnect" : -1,
		"skillConnect" : 14,
		"serverRefineLevel": 0,
	},
	16 : {
		"bonus": item.APPLY_INT,
		"value" : [4, 6, 8, 10, 12],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (460, 212),
		"icon_idx": "16",
		"treeConnect" : 6,
		"skillConnect" : 11,
		"serverRefineLevel": 0,
	},
	17 : {
		"bonus": item.APPLY_RESIST_ICE,#This should resistance for monster!
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (532, 164),
		"icon_idx": "17",
		"treeConnect" : 7,
		"skillConnect" : 16,
		"serverRefineLevel": 0,
	},
	18 : {
		"bonus": item.APPLY_RESIST_ICE,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (575, 132),
		"icon_idx": "18",
		"treeConnect" : -1,
		"skillConnect" : 17,
		"serverRefineLevel": 0,
	},
	19 : {
		"bonus": item.APPLY_ATTBONUS_DEVIL,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (340, 152),
		"icon_idx": "19",
		"treeConnect" : 8,
		"skillConnect" : 0,
		"serverRefineLevel": 0,
	},
	20 : {
		"bonus": item.APPLY_ATTBONUS_ORC,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (243, 179),
		"icon_idx": "20",
		"treeConnect" : -1,
		"skillConnect" : 19,
		"serverRefineLevel": 0,
	},
	21 : {
		"bonus": item.APPLY_ATTBONUS_UNDEAD,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (425, 168),
		"icon_idx": "21",
		"treeConnect" : 9,
		"skillConnect" : 19,
		"serverRefineLevel": 0,
	},
	22 : {
		"bonus": item.APPLY_ATTBONUS_MONSTER,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (483, 120),
		"icon_idx": "22",
		"treeConnect" : -1,
		"skillConnect" : 21,
		"serverRefineLevel": 0,
	},
	23 : {
		"bonus": item.APPLY_CRITICAL_PCT,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (406, 58),
		"icon_idx": "23",
		"treeConnect" : -1,
		"skillConnect" : 19,
		"serverRefineLevel": 0,
	},
	24 : {
		"bonus": item.APPLY_PENETRATE_PCT,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (340, 80),
		"icon_idx": "24",
		"treeConnect" : 10,
		"skillConnect" : 19,
		"serverRefineLevel": 0,
	},
	25 : {
		"bonus": item.APPLY_ATTBONUS_BOSS,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (232, 108),
		"icon_idx": "25",
		"treeConnect" : -1,
		"skillConnect" : 24,
		"serverRefineLevel": 0,
	},
	26 : {
		"bonus": item.APPLY_MOV_SPEED,
		"value" : [4, 8, 12, 16, 20],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (272, 50),
		"icon_idx": "26",
		"treeConnect" : 11,
		"skillConnect" : 24,
		"serverRefineLevel": 0,
	},
	27 : {
		"bonus": item.APPLY_ATTBONUS_STONE,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (171, 75),
		"icon_idx": "27",
		"treeConnect" : -1,
		"skillConnect" : 26,
		"serverRefineLevel": 0,
	},
	28 : {
		"bonus": item.APPLY_ATTBONUS_MILGYO,#zodiac monster!
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (338, 10),
		"icon_idx": "28",
		"treeConnect" : 12,
		"skillConnect" : 24,
		"serverRefineLevel": 0,
	},
	29 : {
		"bonus": item.APPLY_ATTBONUS_MILGYO,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (174, 10),
		"icon_idx": "29",
		"treeConnect" : 13,
		"skillConnect" : 28,
		"serverRefineLevel": 0,
	},
	30 : {
		"bonus": item.APPLY_RESIST_BOW,
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (85, 10),
		"icon_idx": "30",
		"treeConnect" : -1,
		"skillConnect" : 29,
		"serverRefineLevel": 0,
	},
	31 : {
		"bonus": item.APPLY_ATTBONUS_MILGYO,#snake monster!
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (477, 32),
		"icon_idx": "31",
		"treeConnect" : 14,
		"skillConnect" : 28,
		"serverRefineLevel": 0,
	},
	32 : {
		"bonus": item.APPLY_ATTBONUS_MILGYO,#for meteors!
		"value" : [2, 4, 6, 8, 10],
		"refineList" : [0, 1, 2, 3, 4],

		# dont touch!
		"position" : (538, 70),
		"icon_idx": "32",
		"treeConnect" : -1,
		"skillConnect" : 31,
		"serverRefineLevel": 0,
	},
}

class SkillTreeWindow(ui.BoardWithTitleBar):
	def __del__(self):
		ui.BoardWithTitleBar.__del__(self)
	def __init__(self):
		ui.BoardWithTitleBar.__init__(self)
		self.Destroy()
		self.__LoadWindow()
	def Destroy(self):
		self.__children = {}
	def __LoadWindow(self):
		self.AddFlag("attach")
		self.AddFlag("movable")
		self.AddFlag("float")
		self.SetCloseEvent(ui.__mem_func__(self.Close))
		self.SetTitleName("Skill Tree Window")

		bg = CreateWindow(ui.ImageBox(), self, (8, 30), IMG_DIR+"bg.tga")
		self.__children["bg"] = bg
		self.SetSize(8 + bg.GetWidth() + 8, 30 + bg.GetHeight() + 44)
		self.SetCenterPosition()

		global data_dict
		for idx, data in data_dict.items():
			data["serverRefineLevel"] = 0

			skillBtn = CreateWindow(ui.Button(), bg, data["position"])
			skillBtn.SetUpVisual(ICON_IMG_DIR+data["icon_idx"]+"_0.tga")
			skillBtn.SetOverVisual(ICON_IMG_DIR+data["icon_idx"]+"_1.tga")
			skillBtn.SetDownVisual(ICON_IMG_DIR+data["icon_idx"]+"_2.tga")
			skillBtn.SAFE_SetEvent(self.__ClickSkill, idx)
			skillBtn.SetShowToolTipEvent(ui.__mem_func__(self.OverInSkill), idx)
			skillBtn.SetHideToolTipEvent(ui.__mem_func__(self.OverOut))
			self.__children["skillBtn{}".format(idx)] = skillBtn

			skillEffect = CreateWindow(ui.AniImageBox(), skillBtn, (0,0))
			for j in xrange(12):
				skillEffect.AppendImage("d:/ymir work/ui/public/slotactiveeffect/slot1/%02d.sub"% j)
			skillEffect.AddFlag("not_pick")
			skillEffect.SetPosition(2, 1)
			skillEffect.Hide()
			self.__children["skillEffect{}".format(idx)] = skillEffect

		refillBtn = CreateWindow(ui.Button(), self, (8 + bg.GetWidth() - 200, 30 + bg.GetHeight() + 8))
		refillBtn.SetUpVisual("d:/ymir work/ui/public/large_button_01.sub")
		refillBtn.SetOverVisual("d:/ymir work/ui/public/large_button_02.sub")
		refillBtn.SetDownVisual("d:/ymir work/ui/public/large_button_03.sub")
		refillBtn.SetDisableVisual("d:/ymir work/ui/public/large_button_03.sub")
		refillBtn.SAFE_SetEvent(self.__ClickRefill)
		refillBtn.SetShowToolTipEvent(ui.__mem_func__(self.OverInSkill), -1)
		refillBtn.SetHideToolTipEvent(ui.__mem_func__(self.OverOut))
		refillBtn.SetText("Refill")
		
		self.__children["refillBtn"] = refillBtn

		self.__children["skillStatus"] = CreateWindow(ui.ImageBox(), self, (8 + 15, 30 + bg.GetHeight() + 6), IMG_DIR+"deactivated.tga")

		activeBtn = CreateWindow(ui.ToggleButton(), self, (8 + 15 + 40, 30 + bg.GetHeight() + 6))
		activeBtn.SetUpVisual("d:/ymir work/ui/public/xlarge_button_01.sub")
		activeBtn.SetOverVisual("d:/ymir work/ui/public/xlarge_button_02.sub")
		activeBtn.SetDownVisual("d:/ymir work/ui/public/xlarge_button_03.sub")
		activeBtn.SetDisableVisual("d:/ymir work/ui/public/xlarge_button_03.sub")
		activeBtn.SetToggleUpEvent(self.__ClickActivate)
		activeBtn.SetToggleDownEvent(self.__ClickActivate)
		activeBtn.SetShowToolTipEvent(ui.__mem_func__(self.OverInSkill), -2)
		activeBtn.SetHideToolTipEvent(ui.__mem_func__(self.OverOut))
		self.__children["activeBtn"] = activeBtn

		self.__children["timeText"] = CreateWindow(ui.TextLine(), self, (8 + 15 + 40 + activeBtn.GetWidth() + 10, 30 + bg.GetHeight() + 6 + 4), "0 Sec.", "", (-1, -1), "Tahoma:14")

		self.__children["timeDuration"] = -1
		self.__children["activeStatus"] = False

		self.Refresh()

	def OnUpdate(self):
		activeStatus = self.__children["activeStatus"] if self.__children.has_key("activeStatus") else False
		if activeStatus:
			timeDuration = self.__children["timeDuration"] if self.__children.has_key("timeDuration") else -1
			timeDuration-=app.GetGlobalTimeStamp()
			if timeDuration <= 0:
				self.__children["timeDuration"] = -1
				self.__children["activeStatus"] = False
				self.Refresh()
				return
			else:
				self.__children["timeText"].SetText(FormatTime(timeDuration))

	def Refresh(self):
		bg = self.__children["bg"] if self.__children.has_key("bg") else None
		if not bg:
			return

		global data_dict
		timeDuration = self.__children["timeDuration"] if self.__children.has_key("timeDuration") else -1
		if timeDuration <= 0:
			self.__children["timeText"].SetText(FormatTime(0))
		else:
			self.__children["timeText"].SetText(FormatTime(timeDuration))

		if self.NotHasAnySkill():
			self.__children["refillBtn"].Hide()
		else:
			self.__children["refillBtn"].Show()

		activeStatus = self.__children["activeStatus"]
		if activeStatus:
			self.__children["activeBtn"].Down()
		else:
			self.__children["activeBtn"].SetUp()

		self.__children["activeBtn"].SetText("Active" if not activeStatus else "Deactive")
		self.__children["skillStatus"].LoadImage(IMG_DIR+("activated.tga" if activeStatus else "deactivated.tga"))

		if player.GetStatus(player.LEVEL) < MIN_LEVEL:
			return

		(directionDict, plusBtnDict, levelTextDict)  = ({}, {}, {})

		for idx, data in data_dict.items():
			serverRefineLevel = data["serverRefineLevel"] if data.has_key("serverRefineLevel") else 0

			skillBtn = self.__children["skillBtn{}".format(idx)] if self.__children.has_key("skillBtn{}".format(idx)) else None
			if not skillBtn:
				continue
			if serverRefineLevel > 0:
				levelTextDict[idx] = CreateWindow(ui.TextLine(), skillBtn, (5, skillBtn.GetHeight()-13), str(serverRefineLevel))
				levelTextDict[idx].SetOutline()

				if serverRefineLevel == len(data["refineList"]):
					treeConnect = data["treeConnect"] if data.has_key("treeConnect") else -1
					if treeConnect != -1:
						treeImg = CreateWindow(ui.ImageBox(), bg, (0, 0), DIRECTION_IMG_DIR+str(treeConnect)+".tga")
						treeImg.AddFlag("not_pick")
						directionDict[treeConnect] = treeImg

				if activeStatus:
					self.__children["skillEffect{}".format(idx)].Show()
				else:
					self.__children["skillEffect{}".format(idx)].Hide()

			skillConnect = data["skillConnect"] if data.has_key("skillConnect") else -1
			if serverRefineLevel < len(data["refineList"]) and (skillConnect == -1 or data_dict[skillConnect]["serverRefineLevel"] == len(data_dict[skillConnect]["refineList"])):
				plusImg = CreateWindow(ui.ImageBox(), skillBtn, (skillBtn.GetWidth()-11, skillBtn.GetHeight()-11), IMG_DIR+"plus.tga")
				plusImg.AddFlag("not_pick")
				plusBtnDict[idx] = plusImg

		self.__children["dataList"] = [directionDict, plusBtnDict, levelTextDict]

	def OverInSkill(self, skillIdx):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			tooltipItem = interface.tooltipItem
			if tooltipItem:
				tooltipItem.ClearToolTip()
				if skillIdx == -2:
					return
				elif skillIdx == -1:
					global refillList
					for j in xrange(len(refillList)):
						item.SelectItem(refillList[j][0])
						imgFile = item.GetIconImageFileName().replace(".tga", "")
						imgFile = imgFile.replace("icon\\", "")
						tooltipItem.AutoAppendTextLine("|E{}|e".format(imgFile))
						tooltipItem.AppendSpace(8)
						item.SelectItem(refillList[j][0])
						tooltipItem.AutoAppendTextLine("x{} {}".format(refillList[j][1], item.GetItemName()))
						tooltipItem.AppendSpace(5)
						tooltipItem.ShowToolTip()
				else:
					global data_dict
					data = data_dict[skillIdx] if data_dict.has_key(skillIdx) else {}

					if data != {}:
						serverRefineLevel = data["serverRefineLevel"] if data.has_key("serverRefineLevel") else 0

						tooltipItem.AutoAppendTextLine("Lv.{}: {}".format(1 if serverRefineLevel == 0 else serverRefineLevel, GetBonusName(data["bonus"], data["value"][serverRefineLevel if serverRefineLevel == 0 else serverRefineLevel-1])),tooltipItem.NEGATIVE_COLOR if serverRefineLevel == 0 else tooltipItem.POSITIVE_COLOR)
						tooltipItem.AppendSpace(3)
						if serverRefineLevel >= 1 and serverRefineLevel <= 4:
							tooltipItem.AutoAppendTextLine("Lv.{}: {}".format(1 if serverRefineLevel == 0 else serverRefineLevel+1,GetBonusName(data["bonus"], data["value"][serverRefineLevel])),tooltipItem.NEGATIVE_COLOR)
							tooltipItem.AppendSpace(3)

						tooltipItem.AutoAppendTextLine("Level: {}/{}".format(serverRefineLevel, len(data["refineList"])))
						tooltipItem.AppendSpace(3)
						tooltipItem.ShowToolTip()


	def __ClickActivate(self):
		self.Refresh()

		lastClickActivate = self.__children["lastClickActivate"] if self.__children.has_key("lastClickActivate") else -1
		if lastClickActivate > app.GetGlobalTimeStamp():
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Please be slow.")
			return
		self.__children["lastClickActivate"] = app.GetGlobalTimeStamp() + 2

		timeDuration = self.__children["timeDuration"] if self.__children.has_key("timeDuration") else -1

		activeStatus = self.__children["activeStatus"] if self.__children.has_key("activeStatus") else False
		if not activeStatus:
			if timeDuration == -1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Can't activate without duration!")
				return
		else:
			timeDuration-=app.GetGlobalTimeStamp()
			if timeDuration <= 0:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Can't activate without duration!")
				return

		if self.NotHasAnySkill():
			chat.AppendChat(chat.CHAT_TYPE_INFO, "You not has any skill")
			return
		newStatus = not activeStatus
		net.SendChatPacket("/skill_tree status {}".format(1 if newStatus else 0))

		#self.__children["activeStatus"] = newStatus
		#if newStatus:
		#	self.__children["timeDuration"] = app.GetGlobalTimeStamp() + self.__children["timeDuration"]
		#else:
		#	self.__children["timeDuration"] =  self.__children["timeDuration"]-app.GetGlobalTimeStamp() if self.__children["timeDuration"] > app.GetGlobalTimeStamp() else self.__children["timeDuration"]
		#self.Refresh()

	def __ClickRefill(self):
		if self.NotHasAnySkill():
			chat.AppendChat(chat.CHAT_TYPE_INFO, "You not has any skill")
			return

		activeStatus = self.__children["activeStatus"] if self.__children.has_key("activeStatus") else False
		
		if activeStatus:
			chat.AppendChat(chat.CHAT_TYPE_INFO, "You can't do that when active!")
			return
		
		timeDuration = self.__children["timeDuration"] if self.__children.has_key("timeDuration") else -1
		if timeDuration > 0:
			chat.AppendChat(chat.CHAT_TYPE_INFO, "You already has time!")
			return

		net.SendChatPacket("/skill_tree refill")

	def __ClickSkill(self, skillIdx):
		global data_dict
		data = data_dict[skillIdx] if data_dict.has_key(skillIdx) else {}

		skillConnect = data["skillConnect"] if data.has_key("skillConnect") else -1
		activeStatus = self.__children["activeStatus"] if self.__children.has_key("activeStatus") else False

		if skillConnect != -1 and data_dict[skillConnect]["serverRefineLevel"] != len(data_dict[skillConnect]["refineList"]):
			return
		elif data["serverRefineLevel"] >= len(data["refineList"]):
			return
		elif activeStatus:
			chat.AppendChat(chat.CHAT_TYPE_INFO, "You can't do this when skill tree active!")
			return
		self.CloseRefineDialog()

		refineDialog = BuyQuestion(refine_dict[data["refineList"][data["serverRefineLevel"]]])
		refineDialog.SetAcceptEvent(self.__AccepRefine, skillIdx)
		refineDialog.SetCancelEvent(self.CloseRefineDialog)
		refineDialog.Show()
		refineDialog.SetTop()
		self.__children["refineDialog"] = refineDialog

	def __AccepRefine(self, skillIdx):
		global data_dict
		data = data_dict[skillIdx] if data_dict.has_key(skillIdx) else {}

		skillConnect = data["skillConnect"] if data.has_key("skillConnect") else -1
		if skillConnect != -1 and data_dict[skillConnect]["serverRefineLevel"] != len(data_dict[skillConnect]["refineList"]):
			return
		elif data["serverRefineLevel"] >= len(data["refineList"]):
			return
		
		if player.GetStatus(player.LEVEL) < MIN_LEVEL:
			chat.AppendChat(chat.CHAT_TYPE_INFO, "You need minimum {} level.".format(MIN_LEVEL))
			return

		net.SendChatPacket("/skill_tree refine {} {}".format(skillIdx, 1 if self.__children["refineDialog"].GetStatus() else 0))

		# [TEST CODE[VISUAL]]
		#data_dict[skillIdx]["serverRefineLevel"] += 1
		#self.Refresh()

		self.CloseRefineDialog()

	def SkillTreeSetTime(self, leftTime):
		self.__children["timeDuration"] = leftTime if leftTime > 0 else -1
		self.Refresh()

	def SkillTreeSetStatus(self, newStatus, leftTime):
		if self.__children["activeStatus"] == newStatus:
			return
		currentDuration = self.__children["timeDuration"] if leftTime == -1 else leftTime
		if newStatus:
			self.__children["timeDuration"] = app.GetGlobalTimeStamp() + currentDuration
		else:
			self.__children["timeDuration"] = currentDuration - app.GetGlobalTimeStamp()

		self.__children["activeStatus"] = newStatus

		self.Refresh()

	def SkillTreeSetLevel(self, skillIdx, skillLevel):
		global data_dict
		if data_dict.has_key(skillIdx):
			data_dict[skillIdx]["serverRefineLevel"] = skillLevel
			self.Refresh()
	def SkillTreeLoad(self, skillData, leftTime, isActive):
		global data_dict
		self.__children["timeDuration"] = leftTime if leftTime > 0 else -1
		if isActive:
			self.__children["timeDuration"] += app.GetGlobalTimeStamp()
		try:
			skillList = skillData.split("#")
			for skillSplit in skillList:
				skill = skillSplit.split("?")
				if len(skill) == 2:
					if data_dict.has_key(int(skill[0])):
						data_dict[int(skill[0])]["serverRefineLevel"] = int(skill[1])
		except:
			pass
		self.Refresh()

	def NotHasAnySkill(self):
		global data_dict
		for key, data in data_dict.items():
			if data["serverRefineLevel"] > 0:
				return False
		return True
	def CloseRefineDialog(self):
		if self.__children.has_key("refineDialog"):
			self.__children["refineDialog"].Destroy()
			self.__children["refineDialog"].Hide()
			del self.__children["refineDialog"]

	def OverOut(self):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			if interface.tooltipItem:
				interface.tooltipItem.HideToolTip()
	def Open(self):
		self.Show()
		self.SetTop()
	def Close(self):
		self.CloseRefineDialog()
		self.Hide()
	def OnPressEscapeKey(self):
		self.Close()
		return True

class BuyQuestion(ui.BoardWithTitleBar):
	def __del__(self):
		ui.BoardWithTitleBar.__del__(self)
	def Destroy(self):
		self.__children = {}	
	def __init__(self, refineData):
		ui.BoardWithTitleBar.__init__(self)
		self.__LoadWindow(refineData)

	def __LoadWindow(self, refineData):
		self.Destroy()
		self.AddFlag("movable")
		self.AddFlag("attach")
		self.AddFlag("float")
		self.SetCloseEvent(ui.__mem_func__(self.Close))
		self.SetTitleName("Refine")

		bgFirst = CreateWindow(Board(), self, (10, 33), "", "", (142, 54))
		self.__children["bgFirst"] = bgFirst

		itemShopItemSlot = CreateWindow(ui.ImageBox(), bgFirst, (10, 12), IMG_DIR+"slot.tga")
		self.__children["itemShopItemSlot"] = itemShopItemSlot

		global REFINE_DECREASE_ITEM
		item.SelectItem(REFINE_DECREASE_ITEM)
		itemShopItem = CreateWindow(ui.ImageBox(), itemShopItemSlot, (0, 0), item.GetIconImageFileName())
		itemShopItem.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem, REFINE_DECREASE_ITEM)
		itemShopItem.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
		itemShopItem.SetEvent(ui.__mem_func__(self.__ClickCheckBox),"mouse_click")
		self.__children["itemShopItem"] = itemShopItem

		itemName = CreateWindow(ui.TextLine(), bgFirst, (10+itemShopItemSlot.GetWidth()+7, 22), item.GetItemName())
		self.__children["itemName"] = itemName

		checkBoxImg = CreateWindow(ui.ImageBox(), itemShopItem, (20, 20), IMG_DIR+"deactive.tga")
		checkBoxImg.SetEvent(ui.__mem_func__(self.__ClickCheckBox),"mouse_click")
		self.__children["checkBoxImg"] = checkBoxImg

		self.__children["checkBoxStatus"] = False

		self.__children["refineData"] = refineData

		acceptBtn = CreateWindow(ui.Button(), self, (0, 0))
		acceptBtn.SetUpVisual("d:/ymir work/ui/public/middle_button_01.sub")
		acceptBtn.SetOverVisual("d:/ymir work/ui/public/middle_button_02.sub")
		acceptBtn.SetDownVisual("d:/ymir work/ui/public/middle_button_03.sub")
		acceptBtn.SetText("Refine")
		self.__children["acceptBtn"] = acceptBtn

		cancelBtn = CreateWindow(ui.Button(), self, (0, 0))
		cancelBtn.SetUpVisual("d:/ymir work/ui/public/middle_button_01.sub")
		cancelBtn.SetOverVisual("d:/ymir work/ui/public/middle_button_02.sub")
		cancelBtn.SetDownVisual("d:/ymir work/ui/public/middle_button_03.sub")
		cancelBtn.SetText("Cancel")
		self.__children["cancelBtn"] = cancelBtn

		infoBtn = CreateWindow(ui.Button(), self, (20, 10))
		infoBtn.SetUpVisual(IMG_DIR+"info_btn_0.tga")
		infoBtn.SetOverVisual(IMG_DIR+"info_btn_1.tga")
		infoBtn.SetDownVisual(IMG_DIR+"info_btn_1.tga")
		infoBtn.SetShowToolTipEvent(ui.__mem_func__(self.OverInItem), -1)
		infoBtn.SetHideToolTipEvent(ui.__mem_func__(self.OverOutItem))
		self.__children["infoBtn"] = infoBtn

		self.RefreshItems()

	def RefreshItems(self):
		bgItem = CreateWindow(Board(), self, (10, 33 + 54 + 2), "", "", (142, 54))
		self.__children["bgItem"] = bgItem

		maxWidth = 142
		if maxWidth < 20+32+7+self.__children["itemName"].GetTextSize()[0]:
			maxWidth = 20+32+7+self.__children["itemName"].GetTextSize()[0] + 8

		yPos = 12

		refineData = self.__children["refineData"]

		itemList = refineData["items"] if refineData.has_key("items") else []

		for j in xrange(len(itemList)):
			itemData = itemList[j]

			itemSlot = CreateWindow(ui.ImageBox(), bgItem, (10, yPos), IMG_DIR+"slot.tga")
			self.__children["itemSlot{}".format(j)] = itemSlot

			itemCount = itemData[1] if not self.GetStatus() else int(math.floor((float(itemData[1]) / 2.0) + 0.5))

			item.SelectItem(itemData[0])
			itemImg = CreateWindow(ui.ImageBox(), itemSlot, (0, 0), item.GetIconImageFileName())
			itemImg.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem, itemData[0])
			itemImg.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
			self.__children["itemImg{}".format(j)] = itemImg


			itemText = ("|cFF7BAA78" if player.GetItemCountByVnum(itemData[0]) >= itemCount else "|cFFE57875") + ("x{} {}".format(itemCount, item.GetItemName()))
	
			itemName = CreateWindow(ui.TextLine(), bgItem, (10+itemImg.GetWidth()+7, yPos + 10), itemText)
			if maxWidth < 10+itemImg.GetWidth()+7+itemName.GetTextSize()[0]:
				maxWidth = 10+itemImg.GetWidth()+7+itemName.GetTextSize()[0] + 8
			self.__children["itemName{}".format(j)] = itemName

			yPos+=37

		yPos+=12

		yangText = CreateWindow(ui.TextLine(), bgItem, (10, yPos - 6), "Price: {}".format(localeInfo.NumberToMoneyString(refineData["yang"] if refineData.has_key("yang") else 0)))
		if maxWidth < 10+yangText.GetTextSize()[0]:
			maxWidth = 10+yangText.GetTextSize()[0] + 8
		self.__children["yangText"] = yangText

		yPos+=20

		self.__children["bgFirst"].SetSize(maxWidth, self.__children["bgFirst"].GetHeight())
		bgItem.SetSize(maxWidth, yPos)
		maxWidth = 10+maxWidth+10
		self.SetSize(maxWidth, 33+54+2+yPos+36)
		

		self.__children["acceptBtn"].SetPosition((maxWidth/2)-65, 33+54+2+yPos+6)
		self.__children["cancelBtn"].SetPosition((maxWidth/2)+5, 33+54+2+yPos+6)
		self.SetCenterPosition()

	def GetStatus(self):
		return self.__children["checkBoxStatus"] if self.__children.has_key("checkBoxStatus") else False

	def __ClickCheckBox(self, emptyArg):
		checkBoxStatus = not self.__children["checkBoxStatus"]
		self.__children["checkBoxStatus"] = checkBoxStatus
		self.__children["checkBoxImg"].LoadImage(IMG_DIR+("active.tga" if checkBoxStatus else "deactive.tga"))
		self.RefreshItems()

	def SetAcceptEvent(self, func, skillIdx):
		self.__children["acceptBtn"].SAFE_SetEvent(func, skillIdx)

	def SetCancelEvent(self, func):
		self.__children["cancelBtn"].SAFE_SetEvent(func)

	def OverInItem(self, itemVnum):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			if interface.tooltipItem:
				if itemVnum == -1:
					interface.tooltipItem.ClearToolTip()
					interface.tooltipItem.AutoAppendTextLine("With Skill Tree Reducation you can reduce the upgrade costs by half. One Skill Tree Reduction item is for one upgraade. You have to tick the Item to use it.")
					interface.tooltipItem.ShowToolTip()
				else:
					interface.tooltipItem.SetItemToolTip(itemVnum)
	def OverOutItem(self):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			if interface.tooltipItem:
				interface.tooltipItem.HideToolTip()
	def Close(self):
		self.Hide()
	def OnPressEscapeKey(self):
		self.Close()
		return True

class Board(ui.Window):
	(CORNER_WIDTH, CORNER_HEIGHT, LINE_WIDTH, LINE_HEIGHT) = (7, 7, 7, 7)
	(LT, LB, RT, RB) = (0, 1, 2, 3)
	(L, R, T, B) = (0, 1, 2, 3)
	def __init__(self):
		ui.Window.__init__(self)
		self.MakeBoard()
		self.MakeBase()
	def MakeBoard(self):
		cornerPath = IMG_DIR+"board/corner_"
		linePath = IMG_DIR+"board/"
		CornerFileNames = [ cornerPath+dir+".tga" for dir in ("left_top", "left_bottom", "right_top", "right_bottom") ]
		LineFileNames = [ linePath+dir+".tga" for dir in ("left", "right", "top", "bottom") ]
		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ui.ExpandedImageBox()
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)
		self.Lines = []
		for fileName in LineFileNames:
			Line = ui.ExpandedImageBox()
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)
		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)
	def MakeBase(self):
		self.Base = ui.ExpandedImageBox()
		self.Base.AddFlag("not_pick")
		self.Base.LoadImage(IMG_DIR+"board/base.tga")
		self.Base.SetParent(self)
		self.Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Base.Show()
	def __del__(self):
		ui.Window.__del__(self)
	def SetSize(self, width, height):
		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		ui.Window.SetSize(self, width, height)
		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)
		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		if self.Base:
			self.Base.SetRenderingRect(0, 0, horizontalShowingPercentage, verticalShowingPercentage)

def CreateWindow(window, parent, windowPos, windowArgument = "", windowPositionRule = "", windowSize = (-1, -1), windowFontName = -1):
	window.SetParent(parent)
	window.SetPosition(*windowPos)
	if windowSize != (-1, -1):
		window.SetSize(*windowSize)
	if windowPositionRule:
		splitList = windowPositionRule.split(":")
		if len(splitList) == 2:
			(type, mode) = (splitList[0], splitList[1])
			if type == "all":
				window.SetHorizontalAlignCenter()
				window.SetVerticalAlignCenter()
				window.SetWindowHorizontalAlignCenter()
				window.SetWindowVerticalAlignCenter()
			elif type == "horizontal":
				if isinstance(window, ui.TextLine):
					if mode == "center":
						window.SetHorizontalAlignCenter()
					elif mode == "right":
						window.SetHorizontalAlignRight()
					elif mode == "left":
						window.SetHorizontalAlignLeft()
				else:
					if mode == "center":
						window.SetWindowHorizontalAlignCenter()
					elif mode == "right":
						window.SetWindowHorizontalAlignRight()
					elif mode == "left":
						window.SetWindowHorizontalAlignLeft()
			elif type == "vertical":
				if isinstance(window, ui.TextLine):
					if mode == "center":
						window.SetVerticalAlignCenter()
					elif mode == "top":
						window.SetVerticalAlignTop()
					elif mode == "bottom":
						window.SetVerticalAlignBottom()
				else:
					if mode == "top":
						window.SetWindowVerticalAlignTop()
					elif mode == "center":
						window.SetWindowVerticalAlignCenter()
					elif mode == "bottom":
						window.SetWindowVerticalAlignBottom()
	if windowArgument:
		if isinstance(window, ui.TextLine) or isinstance(window, ui.Button):
			if windowFontName != -1:
				window.SetFontName(windowFontName)
			window.SetText(windowArgument)
		elif isinstance(window, ui.NumberLine):
			window.SetNumber(windowArgument)
		elif isinstance(window, ui.ExpandedImageBox) or isinstance(window, ui.ImageBox):
			window.LoadImage(windowArgument if windowArgument.find("gr2") == -1 else "icon/item/27995.tga")
	window.Show()
	return window

def FormatTime(seconds):
	if seconds <= 0:
		return "0 Sec."
	m, s = divmod(seconds, 60)
	h, m = divmod(m, 60)
	d, h = divmod(h, 24)
	text = ""
	if d > 0:
		text+=str(d) + " Day "
	if h > 0:
		text+=str(h) + " Hour "
	if m > 0:
		text+=str(m) + " Minute "
	if s > 0:
		text+=str(s) + " Sec"
	text+="."
	return text
