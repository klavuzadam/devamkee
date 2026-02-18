# coding: latin_1

import ui
import uiScriptLocale
import app
import net
import dbg
import snd
import player
import mouseModule
import wndMgr
import skill
import playerSettingModule
import quest
import localeInfo
import uiToolTip
import constInfo
import emotion
import chr
import chat
	
if app.ENABLE_NEW_DETAILS_GUI:
	import uiCharacterDetails

PARTY_AFFECT_ATTACKER			= 1
PARTY_AFFECT_TANKER				= 2
PARTY_AFFECT_BUFFER				= 3
PARTY_AFFECT_SKILL_MASTER		= 4
PARTY_AFFECT_BERSERKER			= 5
PARTY_AFFECT_DEFENDER			= 6

SHOW_ONLY_ACTIVE_SKILL = False
SHOW_LIMIT_SUPPORT_SKILL_LIST = []
HIDE_SUPPORT_SKILL_POINT = False

#if app.ENABLE_YOHARA_SYSTEM:
HIDE_SUPPORT_SKILL_POINT = True
SHOW_LIMIT_SUPPORT_SKILL_LIST = [121, 122, 123, 124, 126, 127, 129, 128, 131, 137, 138, 139, 140, 132, 133, 134, 246]
#else:
#	if localeInfo.IsYMIR():
#		SHOW_LIMIT_SUPPORT_SKILL_LIST = [121, 122, 123, 124, 126, 127, 129, 128, 131, 137, 138, 139, 140,141,142]
#		[121, 122, 123, 124, 126, 127, 129, 128, 131, 137, 138, 139, 140, 0, 0, 0, 0]
#		if not localeInfo.IsCHEONMA():
#			HIDE_SUPPORT_SKILL_POINT = True
#			SHOW_LIMIT_SUPPORT_SKILL_LIST = [121, 122, 123, 124, 126, 127, 129, 128, 131, 137, 138, 139, 140,141,142]
#	elif localeInfo.IsJAPAN() or   (localeInfo.IsEUROPE() and app.GetLocalePath() != "locale/ca") and (localeInfo.IsEUROPE() and app.GetLocalePath() != "locale/br"):
#		HIDE_SUPPORT_SKILL_POINT = True	
#		SHOW_LIMIT_SUPPORT_SKILL_LIST = [121, 122, 123, 124, 126, 127, 129, 128, 131, 137, 138, 139, 140]
#
#	else:
#		HIDE_SUPPORT_SKILL_POINT = True

if app.USE_PASSIVE_ABILITY_TAMER:
	for i in range(skill.PASSIVE_ABILITY_VNUM_TAMER_1, skill.PASSIVE_ABILITY_VNUM_TAMER_MAX):
		SHOW_LIMIT_SUPPORT_SKILL_LIST.extend([i,])

FACE_IMAGE_DICT = {
	playerSettingModule.RACE_WARRIOR_M	: "icon/face/warrior_m.tga",
	playerSettingModule.RACE_WARRIOR_W	: "icon/face/warrior_w.tga",
	playerSettingModule.RACE_ASSASSIN_M	: "icon/face/assassin_m.tga",
	playerSettingModule.RACE_ASSASSIN_W	: "icon/face/assassin_w.tga",
	playerSettingModule.RACE_SURA_M		: "icon/face/sura_m.tga",
	playerSettingModule.RACE_SURA_W		: "icon/face/sura_w.tga",
	playerSettingModule.RACE_SHAMAN_M	: "icon/face/shaman_m.tga",
	playerSettingModule.RACE_SHAMAN_W	: "icon/face/shaman_w.tga",
}
if app.ENABLE_WOLFMAN_CHARACTER:
	FACE_IMAGE_DICT.update({playerSettingModule.RACE_WOLFMAN_M  : "icon/face/wolfman_m.tga",})

def unsigned32(n):
	return n & 0xFFFFFFFFL

if app.ENABLE_QUEST_RENEWAL:
	from _weakref import proxy
	import grp

	QUEST_LABEL_TAB_COLOR_IMG_DICT = {
		quest.QUEST_TYPE_MAIN : "d:/ymir work/ui/quest_re/tabcolor_1_main.tga",
		quest.QUEST_TYPE_SUB : "d:/ymir work/ui/quest_re/tabcolor_2_sub.tga",
		quest.QUEST_TYPE_LEVELUP : "d:/ymir work/ui/quest_re/tabcolor_3_levelup.tga",
		quest.QUEST_TYPE_EVENT : "d:/ymir work/ui/quest_re/tabcolor_4_event.tga",
		quest.QUEST_TYPE_COLLECTION : "d:/ymir work/ui/quest_re/tabcolor_5_collection.tga",
		quest.QUEST_TYPE_SYSTEM : "d:/ymir work/ui/quest_re/tabcolor_6_system.tga",
		quest.QUEST_TYPE_SCROLL : "d:/ymir work/ui/quest_re/tabcolor_7_scroll.tga",
		quest.QUEST_TYPE_DAILY : "d:/ymir work/ui/quest_re/tabcolor_8_daily.tga"
	}

	QUEST_LABEL_NAME_DICT = {
		quest.QUEST_TYPE_MAIN : uiScriptLocale.QUEST_UI_TEXT_MAIN,
		quest.QUEST_TYPE_SUB : uiScriptLocale.QUEST_UI_TEXT_SUB,
		quest.QUEST_TYPE_LEVELUP : uiScriptLocale.QUEST_UI_TEXT_LEVELUP,
		quest.QUEST_TYPE_EVENT : uiScriptLocale.QUEST_UI_TEXT_EVENT,
		quest.QUEST_TYPE_COLLECTION : uiScriptLocale.QUEST_UI_TEXT_COLLECTION,
		quest.QUEST_TYPE_SYSTEM : uiScriptLocale.QUEST_UI_TEXT_SYSTEM,
		quest.QUEST_TYPE_SCROLL : uiScriptLocale.QUEST_UI_TEXT_SCROLL,
		quest.QUEST_TYPE_DAILY : uiScriptLocale.QUEST_UI_TEXT_DAILY
	}

	class QuestDescObject(ui.Window):
		WIDTH = 222
		HEIGHT = 15

		DESC_TEXT_MAX_WIDTH = 130 #202

		def __init__(self, parent, quest_index, desc_data = None):
			ui.Window.__init__(self)

			self.parent = parent
			self.quest_index = quest_index
			self.desc_data = desc_data

			self.quest_text = None
			self.quest_string_type = quest.QUEST_STRING_TYPE_NORMAL

			self.SetParent(parent)

			# Contents window.
			contents_window = ui.Window()
			contents_window.SetParent(self)
			contents_window.SetSize(self.WIDTH, self.HEIGHT)
			contents_window.SetPosition(15, 0)
			contents_window.Show()
			self.contents_window = contents_window

			# Contents text line.
			contents_textline = ui.TextLine()
			contents_textline.SetParent(self.contents_window)
			contents_textline.SetText(desc_data)
			contents_textline.SetPosition(0, 0)
			contents_textline.Show()
			self.contents_textline = contents_textline

			ui.Window.Show(self)

		def __del__(self):
			ui.Window.__del__(self)

			self.parent = None
			self.quest_index = None
			self.desc_data = None

			self.quest_text = None
			self.quest_string_type = None

		def SetQuestText(self, text):
			if app.GetTextWidth(text) > self.DESC_TEXT_MAX_WIDTH:
				string = text[:self.DESC_TEXT_MAX_WIDTH] + "..."
			else:
				string = text

			self.quest_text = text
			self.contents_textline.SetText(string)

		def SetQuestStringType(self, type):
			self.quest_string_type = type

		def __UpdateQuestClock(self):
			(last_name, last_time) = quest.GetQuestLastTime(self.quest_index)

			clock_text = localeInfo.QUEST_UNLIMITED_TIME
			if len(last_name) > 0:
				if last_time <= 0:
					clock_text = localeInfo.QUEST_TIMEOVER
				else:
					second = int(last_time % 60)
					minute = int((last_time / 60) % 60)
					hour = int((last_time / 60) / 60) % 24

					clock_text = last_name + " : "

					if hour > 0:
						clock_text += str(hour) + localeInfo.QUEST_HOUR
						clock_text += " "

					if minute > 0:
						clock_text += str(minute) + localeInfo.QUEST_MIN

					if second > 0 and minute < 1:
						clock_text += str(second) + localeInfo.QUEST_SEC

			self.contents_textline.SetText(clock_text)

		def OnUpdate(self):
			if self.quest_string_type == quest.QUEST_STRING_TYPE_CLOCK:
				self.__UpdateQuestClock()

	class QuestObject(ui.Window):
		LINE_Y = 3

		def __init__(self, parent, quest_index, is_confirmed):
			ui.Window.__init__(self)

			self.parent = parent
			self.quest_index = quest_index
			self.is_confirmed = is_confirmed

			self.desc_list = []

			self.SetParent(self.parent)
			self.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__ClickQuest))

			# Quest line image.
			line = ui.ImageBox()
			line.LoadImage("d:/ymir work/ui/quest_re/quest_list_line_01.tga")
			line.SetParent(self)
			line.Hide()
			self.line = line

			# New quest icon. (green dot)
			new_image = ui.ImageBox()
			new_image.SetParent(self)
			new_image.AddFlag("not_pick")
			new_image.SetPosition(3, 4)
			new_image.LoadImage("d:/ymir work/ui/quest_re/quest_new.tga")
			if not self.is_confirmed:
				new_image.Show()
			else:
				new_image.Hide()
			self.new_image = new_image

			ui.Window.Hide(self)

		def __del__(self):
			ui.Window.__del__(self)

			self.parent = None
			self.quest_index = 0
			self.is_confirmed = False

			self.desc_list = []

		def AddLine(self):
			if not self.desc_list:
				return

			x, y = self.desc_list[-1].GetLocalPosition()

			if self.line:
				self.line.SetPosition(4, y + 15 + self.LINE_Y)
				self.line.Show()

		def RemoveLine(self):
			self.line.Hide()

		def ReplaceDesc(self, desc_list):
			self.desc_list = []

			for i, desc_data in enumerate(desc_list):

				desc_object = QuestDescObject(self, self.quest_index)
				desc_object.SetQuestStringType(i)

				if i == quest.QUEST_STRING_TYPE_CLOCK:
					(clock_last_name, clock_last_time) = desc_data

					clock_text = localeInfo.QUEST_UNLIMITED_TIME
					if len(clock_last_name) > 0:
						if clock_last_time <= 0:
							clock_text = localeInfo.QUEST_TIMEOVER
						else:
							quest_last_minute = clock_last_time / 60
							quest_last_sec = clock_last_time % 60

							clock_text = clock_last_name + " : "

							if quest_last_minute > 0:
								clock_text += str(quest_last_minute) + localeInfo.QUEST_MIN
								if quest_last_sec > 0:
									clock_text += " "

							if quest_last_sec > 0:
								clock_text += str(quest_last_sec) + localeInfo.QUEST_SEC

					desc_object.SetQuestText(clock_text)
					self.desc_list.append(desc_object)

				elif i == quest.QUEST_STRING_TYPE_COUNT:
					(quest_counter_name, quest_counter_value) = desc_data

					if len(quest_counter_name) > 0:
						counter_text = ("%s : %d" % (quest_counter_name, quest_counter_value))

						desc_object.SetQuestText(counter_text)
						self.desc_list.append(desc_object)

				else:
					quest_name = desc_data

					desc_object.SetQuestText(quest_name)
					self.desc_list.append(desc_object)

		def SetPositionIndex(self, pos):
			self.quest_index = pos

		def SetConfirmed(self, is_confirmed):
			self.is_confirmed = is_confirmed
			if not self.is_confirmed:
				self.new_image.Show()
			else:
				self.new_image.Hide()

		def IsConfirmed(self):
			return self.is_confirmed

		def Arrange(self):
			y = 0
			for desc_obj in self.desc_list:
				desc_obj.SetPosition(0, y)
				y += 15 # Add space between textlines.

		def __ClickQuest(self):
			import event
			event.QuestButtonClick(-2147483648 + self.quest_index)

			# Remove new image from quest object.
			self.is_confirmed = True
			self.new_image.Hide()

			if self.parent:
				self.parent.CheckNewImage()

		def GetDescCount(self):
			return len(self.desc_list)

	class QuestCategory(ui.Window):
		BOARD_WIDTH = 222
		BOARD_HEIGHT = 340

		TAB_WIDTH = 222
		TAB_HEIGHT = 22

		LINE_GAP = 10

		def __init__(self, clipping_mask_window, parent, quest_type):
			self.clipping_mask_window = clipping_mask_window
			self.parent = parent
			self.quest_type = quest_type

			# Category position and height.
			self.x = 0
			self.y = 0
			self.height = 0

			# Dictionary that contains all the quests of this category.
			self.quest_dict = {}

			# Check if the category is on or off.
			self.is_on = False

			ui.Window.__init__(self)

			self.SetParent(self.clipping_mask_window)
			self.SetSize(self.BOARD_WIDTH, 23) # hotfix height clickable
			self.AddFlag("not_pick")
			#self.SetColor(grp.GenerateColor(1.0, 1.0, 0.0, 0.2))

			# Name window that contains all the children.
			name_window = ui.Window()
			name_window.SetParent(self)
			name_window.SetPosition(0, 0)
			name_window.SetSize(self.TAB_WIDTH, self.TAB_HEIGHT)
			name_window.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__ClickLabel))
			name_window.Show()
			self.name_window = name_window

			# Image of the label.
			label_image = ui.ImageBox()
			label_image.SetParent(self.name_window)
			label_image.AddFlag("not_pick")
			label_image.SetPosition(0, 0)
			label_image.SetSize(10, 10)
			label_image.LoadImage("d:/ymir work/ui/quest_re/quest_tab_01.tga")
			label_image.Show()
			self.label_image = label_image

			# Opened image icon.
			opened_image = ui.ImageBox()
			opened_image.SetParent(self.name_window)
			opened_image.AddFlag("not_pick")
			opened_image.SetPosition(4, 2)
			opened_image.LoadImage("d:/ymir work/ui/quest_re/quest_up.tga")
			opened_image.Hide()
			self.opened_image = opened_image

			# Closed image icon.
			closed_image = ui.ImageBox()
			closed_image.SetParent(self.name_window)
			closed_image.AddFlag("not_pick")
			closed_image.SetPosition(4, 2)
			closed_image.LoadImage("d:/ymir work/ui/quest_re/quest_down.tga")
			closed_image.Show()
			self.closed_image = closed_image

			# Quest exist image icon.
			quest_exist_image = ui.ImageBox()
			quest_exist_image.SetParent(self.name_window)
			quest_exist_image.AddFlag("not_pick")
			quest_exist_image.SetPosition(188, 12)
			if quest_type in QUEST_LABEL_TAB_COLOR_IMG_DICT:
				quest_exist_image.LoadImage(QUEST_LABEL_TAB_COLOR_IMG_DICT[quest_type])
				quest_exist_image.Show()
			else:
				quest_exist_image.Hide()
			self.quest_exist_image = quest_exist_image

			# Label name.
			name_textline = ui.TextLine()
			name_textline.SetParent(self.name_window)
			name_textline.SetPosition(24, 2)
			if quest_type in parent.quest_label_name_dict:
				name_textline.SetText(parent.quest_label_name_dict[quest_type])
			else:
				name_textline.SetText("")
			name_textline.SetPackedFontColor(0xffCEC6B5)
			name_textline.Show()
			self.name_textline = name_textline

			# New quest icon (green dot)
			new_image = ui.ImageBox()
			new_image.SetParent(self.name_window)
			new_image.AddFlag("not_pick")
			new_image.LoadImage("d:/ymir work/ui/quest_re/quest_new.tga")
			if quest_type in parent.quest_label_name_dict:
				new_image.SetPosition(app.GetTextWidth(parent.quest_label_name_dict[quest_type]) + 30, 5)
				new_image.Show()
			else:
				new_image.SetPosition(0, 0)
				new_image.Hide()
			self.new_image = new_image

			self.__HideQuestExistImage()
			self.__HideNewImage()

			ui.Window.Show(self)

		def __del__(self):
			ui.Window.__del__(self)

			self.clipping_mask_window = None
			self.parent = None
			self.quest_type = 0

			self.x = 0
			self.y = 0
			self.height = 0

			self.quest_dict = {}
			self.is_on = False

			self.name_window = None
			self.label_image = None
			self.opened_image = None
			self.closed_image = None
			self.quest_exist_image = None
			self.name_textline = None
			self.new_image = None

		def __ShowClosedImg(self):
			self.opened_image.Hide()
			self.closed_image.Show()

		def __ShowOpendImg(self):
			self.closed_image.Hide()
			self.opened_image.Show()

		def __ShowQuestExistImage(self):
			self.quest_exist_image.Show()

		def __HideQuestExistImage(self):
			self.quest_exist_image.Hide()

		def __ShowNewImage(self):
			self.new_image.Show()

		def __HideNewImage(self):
			self.new_image.Hide()

		def OpenCategory(self):
			self.__ClickLabel()

		def CloseCategory(self):
			self.is_on = False
			for key, item in self.quest_dict.items():
				item.Hide()

			self.__ShowClosedImg()
			self.__Arrange()

		# When clicking on the tab label.
		def __ClickLabel(self):
			# Prevent trying to open an empty category.
			if not self.quest_dict:
				self.is_on = False
				self.__ShowClosedImg()
				return

			if self.is_on:
				self.__ShowClosedImg()
				self.is_on = False
			else:
				self.__ShowOpendImg()
				self.is_on = True

			# Show all the quests from the category.
			for key, item in self.quest_dict.items():
				item.Show() if self.is_on else item.Hide()

			self.__Arrange()

		# Check if there are any confirmed quests in the category.
		def CheckNewImage(self):
			for key, item in self.quest_dict.items():
				if not item.IsConfirmed():
					self.__ShowNewImage()
					break

			self.__HideNewImage()

		# Replace quests in the category.
		def ReplaceQuest(self, quest_index, is_confirmed, desc_data_list):
			if not quest_index in self.quest_dict:
				quest_obj = QuestObject(self, quest_index, is_confirmed)
			else:
				quest_obj = self.quest_dict[quest_index]

			#quest_obj.SetPositionIndex(quest_pos)
			#quest_obj.SetConfirmed(is_confirmed)
			quest_obj.ReplaceDesc(desc_data_list)

			quest_obj.SetClippingMaskWindow(self.clipping_mask_window)

			if self.is_on:
				quest_obj.Show()

			self.quest_dict.update({ quest_index : quest_obj })
			if self.quest_dict:
				self.__ShowQuestExistImage()

			if not is_confirmed:
				self.__ShowNewImage()
			else:
				self.__HideNewImage()

			self.__Arrange()

		# Delete quests in the category.
		def DeleteQuest(self, quest_index):
			if quest_index in self.quest_dict:
				self.quest_dict[quest_index].Hide()
				del self.quest_dict[quest_index]

			# Check if there are any quests in the dictionary.
			if not self.quest_dict:
				self.is_on = False

				self.__ShowClosedImg()

				self.__HideNewImage()
				self.__HideQuestExistImage()

			self.__Arrange()

		# Method of arranging all the quests in the category.
		def __Arrange(self):
			y = self.TAB_HEIGHT # Initial vertical position.
			for index, (key, item) in enumerate(self.quest_dict.items()):
				quest_desc_count = item.GetDescCount() # Quest description count.
				quest_desc_height = quest_desc_count * 15 # Quest description height.
				quest_desc_height += self.LINE_GAP # Add and extra gap for the line separator.

				item.SetSize(self.TAB_WIDTH, quest_desc_height)
				item.SetPosition(0, y)
				item.Arrange() # Arrange the quest description text lines.

				# Add line at the penultimate quest object.
				if index < len(self.quest_dict) - 1:
					item.AddLine()
				else:
					item.RemoveLine()

				# Increase vertical position by quest description height.
				y += quest_desc_height

			self.SetSize(self.BOARD_WIDTH, y)

			# Call the parent method for arranging the category positions.
			if self.parent:
				self.parent.ChildHeightChanged(self.quest_type)

		# Unused.
		def __height_resize_post_process(self, original_function):
			pass

		def IsOn(self):
			return self.is_on

		# Returns the total height of the quest object based on
		# each quest description.
		def GetQuestObjectHeight(self):
			height = 0
			for index, (key, item) in enumerate(self.quest_dict.items()):
				height += item.GetDescCount() * 15 # Quest description height.
				height += self.LINE_GAP # Add and extra gap for the line separator.
			return height

		# Updates the vertical position for the scroll.
		def UpdateYPosition(self, pos):
			self.SetPosition(self.x, self.y + self.height - pos)

		def UpdatePosition(self, x, y):
			self.x = x; self.y = y
			self.SetPosition(self.x, self.y)

		def SetHeight(self, height):
			self.height = height
			self.SetPosition(self.x, self.y + self.height)

		def GetPosition(self):
			return self.x, self.y

		def GetHeight(self):
			return self.height

	class QuestCategoryGroup(ui.Window):
		LABEL_HEIGHT = 22
		SCROLL_STEP = 18
		BOARD_HEIGHT = 340

		# The constructor method that initializes everything.
		def __init__(self, clipping_mask_window, scroll_object):
			ui.Window.__init__(self)

			self.clipping_mask_window = proxy(clipping_mask_window)

			self.scroll = scroll_object
			self.scroll.SetScrollEvent(ui.__mem_func__(self.__ScrollEvent))
			self.scroll.SetScrollStep(self.SCROLL_STEP)

			self.diff_height = 0

			self.quest_label_name_dict = {
				quest.QUEST_TYPE_MAIN : uiScriptLocale.QUEST_UI_TEXT_MAIN,
				quest.QUEST_TYPE_SUB : uiScriptLocale.QUEST_UI_TEXT_SUB,
				quest.QUEST_TYPE_LEVELUP : uiScriptLocale.QUEST_UI_TEXT_LEVELUP,
				quest.QUEST_TYPE_EVENT : uiScriptLocale.QUEST_UI_TEXT_EVENT,
				quest.QUEST_TYPE_COLLECTION : uiScriptLocale.QUEST_UI_TEXT_COLLECTION,
				quest.QUEST_TYPE_SYSTEM : uiScriptLocale.QUEST_UI_TEXT_SYSTEM,
				quest.QUEST_TYPE_SCROLL : uiScriptLocale.QUEST_UI_TEXT_SCROLL,
				quest.QUEST_TYPE_DAILY : uiScriptLocale.QUEST_UI_TEXT_DAILY
			}

			# Dictionary that contains all the categories.
			self.quest_category_dict = {}
			for quest_type in self.quest_label_name_dict.keys():
				self.quest_category_dict[quest_type] = QuestCategory(self.clipping_mask_window, self, quest_type)
				self.quest_category_dict[quest_type].SetClippingMaskWindow(self.clipping_mask_window)

			# Arrange all the categories initial position.
			self.__Arrange()

			# Refresh the scroll for its initial position.
			self.__RefreshScroll()

		# The destructor method which is called as soon as
		# all references of the object are deleted.
		def __del__(self):
			ui.Window.__del__(self)

			self.clipping_mask_window = None
			self.scroll = None
			self.diff_height = 0

			self.quest_category_dict = {}

		# This will replace (update) any quest in a category.
		def ReplaceQuest(self, quest_type, quest_index, is_confirmed, desc_data_list):
			if quest_type in self.quest_category_dict:
				self.quest_category_dict[quest_type].ReplaceQuest(quest_index, is_confirmed, desc_data_list)

		# Delete and hide the quest from the category.
		def DeleteQuest(self, quest_type, quest_index):
			if quest_type in self.quest_category_dict:
				self.quest_category_dict[quest_type].DeleteQuest(quest_index)

		# This method is called from the QuestCategory class
		# which arranges all the category positions.
		def ChildHeightChanged(self, quest_type):
			self.__ArrangeAfter(quest_type)
			self.__AdjustScrollPos(quest_type)

		# Scroll event in which while scrolling, the categories
		# move vertical along with its children.
		def __ScrollEvent(self):
			pos = self.scroll.GetPos() * self.diff_height
			for key, item in self.quest_category_dict.items():
				item.UpdateYPosition(pos)

		# Refresh the scroll based on the category height.
		def __RefreshScroll(self):
			self.diff_height = 0

			label_height = 0; height = 0
			for key, item in self.quest_category_dict.items():
				label_height += self.LABEL_HEIGHT - 2
				is_on = item.IsOn()
				height += item.GetQuestObjectHeight() if is_on else 0

			total_height = height + label_height + 12
			if total_height >= self.BOARD_HEIGHT:
				self.diff_height = height - label_height

				step_size = float(self.SCROLL_STEP) / self.diff_height
				self.scroll.SetScrollStep(step_size)
				self.scroll.Show()
			else:
				self.scroll.Hide()

		# Unused.
		def __AdjustScrollPos(self, quest_type):
			pass

		# Arranges the categories on startup in a closed state.
		def __Arrange(self):
			for key, item in self.quest_category_dict.iteritems():
				item.UpdatePosition(0, self.LABEL_HEIGHT * key)

		# This arrangement is made when any category is clicked or
		# when any other event is made in the quest object board,
		# like updating objects or removing them.
		def __ArrangeAfter(self, quest_type):
			pos = self.scroll.GetPos() * self.diff_height
			for key, item in self.quest_category_dict.items():
				last_key = key -1; last_item = None; height = 0

				if last_key in self.quest_category_dict:
					last_item = self.quest_category_dict[key - 1]
					is_on = last_item.IsOn()
					height = last_item.GetQuestObjectHeight() if is_on else 0

				# Set the height of the category from its base y position
				# from the first arrangement "__Arrange", following up
				# its height adjustment from its neighbor categories.
				item.SetHeight(height + last_item.GetHeight() if last_item else 0)

				# Update y position for the scroll step when scrolling.
				item.UpdateYPosition(pos) 

			# Refreshing the scroll will properly set the scroll step
			# size for all categories opened or closed on the board.
			# If there is an empty space at the window then there will
			# be no need for a scroll bar.
			self.__RefreshScroll()

			# Using the scroll event will snap the last category to
			# the bottom of the board preventing empty spaces at the
			# end of the window.
			self.__ScrollEvent()

		# Method used for opening a category. (Add-on)
		def OpenCategory(self, quest_type, close_all = False):
			if close_all:
				for key, item in self.quest_category_dict.items():
					item.CloseCategory()

			if quest_type in self.quest_category_dict:
				self.quest_category_dict[quest_type].OpenCategory()

		# Method used for closing a category. (Add-on)
		def CloseCategory(self, quest_type):
			for key, item in self.quest_category_dict.items():
				item.CloseCategory()

class CharacterWindow(ui.ScriptWindow):

	# if app.ENABLE_YOHARA_SYSTEM:
		# ACTIVE_PAGE_SLOT_COUNT = 9
		# SUPPORT_PAGE_SLOT_COUNT = 18
	# else:
	ACTIVE_PAGE_SLOT_COUNT = 8	
	SUPPORT_PAGE_SLOT_COUNT = 18

	PAGE_SLOT_COUNT = 12
	PAGE_HORSE = 2

	if app.__BUFFI_SUPPORT__:
		PAGE_BUFFI = 3

	if app.ENABLE_NEW_DETAILS_GUI:
		chDetailsWnd = None
		isOpenedDetailsWnd = False

	SKILL_GROUP_NAME_DICT = {
		playerSettingModule.JOB_WARRIOR	: { 1 : localeInfo.SKILL_GROUP_WARRIOR_1,	2 : localeInfo.SKILL_GROUP_WARRIOR_2, },
		playerSettingModule.JOB_ASSASSIN	: { 1 : localeInfo.SKILL_GROUP_ASSASSIN_1,	2 : localeInfo.SKILL_GROUP_ASSASSIN_2, },
		playerSettingModule.JOB_SURA		: { 1 : localeInfo.SKILL_GROUP_SURA_1,		2 : localeInfo.SKILL_GROUP_SURA_2, },
		playerSettingModule.JOB_SHAMAN		: { 1 : localeInfo.SKILL_GROUP_SHAMAN_1,	2 : localeInfo.SKILL_GROUP_SHAMAN_2, },
	}
	if app.ENABLE_WOLFMAN_CHARACTER:
		SKILL_GROUP_NAME_DICT.update({playerSettingModule.JOB_WOLFMAN		: { 1 : localeInfo.JOB_WOLFMAN1,	2 : localeInfo.JOB_WOLFMAN2, },})

	STAT_DESCRIPTION =	{
		"HTH" : localeInfo.STAT_TOOLTIP_CON,
		"INT" : localeInfo.STAT_TOOLTIP_INT,
		"STR" : localeInfo.STAT_TOOLTIP_STR,
		"DEX" : localeInfo.STAT_TOOLTIP_DEX,
	}

	if app.ENABLE_YOHARA_SYSTEM:
		TAB_DESCRIPTION = {
			"STATUS" : localeInfo.STAT_TOOLTIP_TAB_CHARACTER,
			"SKILL" : localeInfo.STAT_TOOLTIP_TAB_SKILL,
			"EMOTICON" : localeInfo.STAT_TOOLTIP_TAB_EMOTICON,
			"QUEST" : localeInfo.STAT_TOOLTIP_TAB_QUEST,
		}

		SUNGMA_STAT_DESCRIPTION = {
			"SUNGMA_STR" : localeInfo.STAT_TOOLTIP_SSTR,
			"SUNGMA_HP" : localeInfo.STAT_TOOLTIP_SHP,
			"SUNGMA_MOVE" : localeInfo.STAT_TOOLTIP_SMOVE,
			"SUNGMA_IMMUNE" : localeInfo.STAT_TOOLTIP_SIMMUNE,
		}

	STAT_MINUS_DESCRIPTION = localeInfo.STAT_MINUS_DESCRIPTION

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.state = "STATUS"
		if app.ENABLE_YOHARA_SYSTEM:
			self.stateInfo = "BASE"
			self.buttontooltip = None
			self.ShowButtonToolTip = False

		self.isLoaded = 0

		self.toolTipSkill = 0

		self.__Initialize()
		self.__LoadWindow()

		self.statusPlusCommandDict={
			"HTH" : "/s_val_w ht ",
			"INT" : "/s_val_w iq ",
			"STR" : "/s_val_w st ",
			"DEX" : "/s_val_w dx ",
		}
		
		if app.DISABLE_OLD_MINUS:
			self.statusMinusCommandDict={
				"HTH-" : "/stat_val- ht",
				"INT-" : "/stat_val- iq",
				"STR-" : "/stat_val- st",
				"DEX-" : "/stat_val- dx",
			}
		else:
			self.statusMinusCommandDict={
				"HTH-" : "/stat- ht",
				"INT-" : "/stat- iq",
				"STR-" : "/stat- st",
				"DEX-" : "/stat- dx",
			}

		if app.ENABLE_YOHARA_SYSTEM:
			self.sungmaStatusPlusCommandDict={
				"SUNGMA_STR" : "/stat sstr",
				"SUNGMA_HP" : "/stat shp",
				"SUNGMA_MOVE" : "/stat smove",
				"SUNGMA_IMMUNE" : "/stat simmune",
			}

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __Initialize(self):
		self.refreshToolTip = 0
		self.curSelectedSkillGroup = 0
		self.canUseHorseSkill = -1

		if app.ENABLE_NEW_DETAILS_GUI:
			self.ExpandBtn = None
			self.MinimizeBtn = None
			self.chDetailsWnd = None
			self.isOpenedDetailsWnd = False

		self.toolTip = None
		self.toolTipJob = None
		self.toolTipAlignment = None
		self.toolTipSkill = None

		self.faceImage = None
		self.statusPlusLabel = None
		self.statusPlusValue = None
		self.activeSlot = None
		self.tabDict = None
		self.tabButtonDict = None
		self.pageDict = None
		self.titleBarDict = None
		self.statusPlusButtonDict = None
		self.statusMinusButtonDict = None

		if app.ENABLE_YOHARA_SYSTEM:
			self.tabInfoButtonDict = None
			self.sungmaStatusPlusButtonDict = None
			self.relictrefine = None

		self.skillPageDict = None
		if app.ENABLE_QUEST_RENEWAL:
			self.questScrollBar = None
			self.questPageBoardWnd = None
			self.questCategoryGroup = None
		else:
			self.questShowingStartIndex = 0
			self.questScrollBar = None
			self.questSlot = None
			self.questNameList = None
			self.questLastTimeList = None
			self.questLastCountList = None
		self.skillGroupButton = ()

		self.activeSlot = None
		self.activeSkillPointValue = None
		self.supportSkillPointValue = None
		self.skillGroupButton1 = None
		self.skillGroupButton2 = None
		self.skillGroupButton3 = None
		self.activeSkillGroupName = None

		self.guildNameSlot = None
		self.guildNameValue = None
		self.characterNameSlot = None
		self.characterNameValue = None

		self.emotionToolTip = None
		self.soloEmotionSlot = None
		self.dualEmotionSlot = None

	def Show(self):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)
		if app.ENABLE_NEW_DETAILS_GUI:
			self.__InitCharacterDetailsUIButton()
			if self.chDetailsWnd and self.isOpenedDetailsWnd:
				self.chDetailsWnd.Show()
				self.SetTop()

	def __LoadScript(self, fileName):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, fileName)

	def __BindObject(self):
		self.toolTip = uiToolTip.ToolTip()
		self.toolTipJob = uiToolTip.ToolTip()
		self.toolTipAlignment = uiToolTip.ToolTip(130)

		self.faceImage = self.GetChild("Face_Image")

		faceSlot=self.GetChild("Face_Slot")
		if 949 == app.GetDefaultCodePage():
			faceSlot.SAFE_SetStringEvent("MOUSE_OVER_IN", self.__ShowJobToolTip)
			faceSlot.SAFE_SetStringEvent("MOUSE_OVER_OUT", self.__HideJobToolTip)

		self.boardLeader = self.GetChild("leader_board")
		self.boardLeader.Hide()
		
		list_leader = {
			"LeaderAttacker" : PARTY_AFFECT_ATTACKER, 
			"LeaderBerserker" : PARTY_AFFECT_BERSERKER, 
			"LeaderTanker" : PARTY_AFFECT_TANKER, 
			"LeaderDefender" : PARTY_AFFECT_DEFENDER, 
			"LeaderBuffer" : PARTY_AFFECT_BUFFER, 
			"LeaderSkillMaster" : PARTY_AFFECT_SKILL_MASTER
		}

		for key in list_leader:
			self.GetChild(key).SetEvent(ui.__mem_func__(self.SelectLeaderBonus), list_leader[key])
			
			self.GetChild(key).SetEventOverIn(ui.__mem_func__(self.OverInLeaderBonus), list_leader[key])
			self.GetChild(key).SetEventOverOut(ui.__mem_func__(self.OverOutLeaderBonus), list_leader[key])


		self.statusPlusLabel = self.GetChild("Status_Plus_Label")
		self.statusPlusValue = self.GetChild("Status_Plus_Value")

		self.characterNameSlot = self.GetChild("Character_Name_Slot")
		self.characterNameValue = self.GetChild("Character_Name")
		self.guildNameSlot = self.GetChild("Guild_Name_Slot")
		self.guildNameValue = self.GetChild("Guild_Name")
		self.characterNameSlot.SAFE_SetStringEvent("MOUSE_OVER_IN", self.__ShowAlignmentToolTip)
		self.characterNameSlot.SAFE_SetStringEvent("MOUSE_OVER_OUT", self.__HideAlignmentToolTip)
		
		self.fastReadButton = self.GetChild("FastReadButton")
		self.fastReadButton.SetEvent(ui.__mem_func__(self.OpenFastRead))
		self.fastReadButton.FlashEx()

		self.activeSlot = self.GetChild("Skill_Active_Slot")
		self.activeSkillPointValue = self.GetChild("Active_Skill_Point_Value")
		self.supportSkillPointValue = self.GetChild("Support_Skill_Point_Value")
		self.skillGroupButton1 = self.GetChild("Skill_Group_Button_1")
		self.skillGroupButton2 = self.GetChild("Skill_Group_Button_2")
		self.skillGroupButton3 = self.GetChild("Skill_Group_Button_3")
		self.activeSkillGroupName = self.GetChild("Active_Skill_Group_Name")

		self.tabDict = {
			"STATUS"	: self.GetChild("Tab_01"),
			"SKILL"		: self.GetChild("Tab_02"),
			"EMOTICON"	: self.GetChild("Tab_03"),
			"QUEST"		: self.GetChild("Tab_04"),
		}

		self.tabButtonDict = {
			"STATUS"	: self.GetChild("Tab_Button_01"),
			"SKILL"		: self.GetChild("Tab_Button_02"),
			"EMOTICON"	: self.GetChild("Tab_Button_03"),
			"QUEST"		: self.GetChild("Tab_Button_04")
		}

		self.pageDict = {
			"STATUS"	: self.GetChild("Character_Page"),
			"SKILL"		: self.GetChild("Skill_Page"),
			"EMOTICON"	: self.GetChild("Emoticon_Page"),
			"QUEST"		: self.GetChild("Quest_Page")
		}

		self.titleBarDict = {
			"STATUS"	: self.GetChild("Character_TitleBar"),
			"SKILL"		: self.GetChild("Skill_TitleBar"),
			"EMOTICON"	: self.GetChild("Emoticon_TitleBar"),
			"QUEST"		: self.GetChild("Quest_TitleBar")
		}

		self.statusPlusButtonDict = {
			"HTH"		: self.GetChild("HTH_Plus"),
			"INT"		: self.GetChild("INT_Plus"),
			"STR"		: self.GetChild("STR_Plus"),
			"DEX"		: self.GetChild("DEX_Plus"),
		}

		self.statusMinusButtonDict = {
			"HTH-"		: self.GetChild("HTH_Minus"),
			"INT-"		: self.GetChild("INT_Minus"),
			"STR-"		: self.GetChild("STR_Minus"),
			"DEX-"		: self.GetChild("DEX_Minus"),
		}

		self.skillPageDict = {
			"ACTIVE" : self.GetChild("Skill_Active_Slot"),
			"SUPPORT" : self.GetChild("Skill_ETC_Slot"),
			"HORSE" : self.GetChild("Skill_Active_Slot"),
		}

		self.skillPageStatDict = {
			"SUPPORT"	: player.SKILL_SUPPORT,
			"ACTIVE"	: player.SKILL_ACTIVE,
			"HORSE"		: player.SKILL_HORSE,
		}

		self.skillGroupButton = (
			self.GetChild("Skill_Group_Button_1"),
			self.GetChild("Skill_Group_Button_2"),
			self.GetChild("Skill_Group_Button_3"),
		)

		if app.ENABLE_YOHARA_SYSTEM:
			self.tabInfoButtonDict = {
				"BASE" : self.GetChild("change_base_button"),
				"SUNGMA" : self.GetChild("change_conqueror_button"),
			}

			self.InfoPageDict = {
				"BASE" : self.GetChild("base_info"),
				"SUNGMA" : self.GetChild("sungma_info"),
			}

			self.sungmaStatusPlusButtonDict = {
				"SUNGMA_STR" : self.GetChild("sungma_str_plus"),
				"SUNGMA_HP" : self.GetChild("sungma_hp_plus"),
				"SUNGMA_MOVE" : self.GetChild("sungma_move_plus"),
				"SUNGMA_IMMUNE" : self.GetChild("sungma_immune_plus"),
			}

			self.relictrefine = self.GetChild("passive_expanded_btn")
			# self.GetChild("change_conqueror_button").Hide()
			self.GetChild("passive_expanded_btn").Hide()
			# Character:
			# Level
			self.GetChild("Lv_ToolTip").SetShowToolTipEvent(ui.__mem_func__(self.OverInToolTipButton), localeInfo.STAT_TOOLTIP_LEVEL)
			self.GetChild("Lv_ToolTip").SetHideToolTipEvent(ui.__mem_func__(self.OverOutToolTipButton))

			# Exp
			self.GetChild("Exp_ToolTip").SetShowToolTipEvent(ui.__mem_func__(self.OverInToolTipButton), localeInfo.STAT_TOOLTIP_EXP)
			self.GetChild("Exp_ToolTip").SetHideToolTipEvent(ui.__mem_func__(self.OverOutToolTipButton))

			# Base level
			self.GetChild("change_base_button").SetShowToolTipEvent(ui.__mem_func__(self.OverInToolTipButton), localeInfo.STAT_TOOLTIP_BASE_LEVEL)
			self.GetChild("change_base_button").SetHideToolTipEvent(ui.__mem_func__(self.OverOutToolTipButton))

			# Conqueror level
			self.GetChild("change_conqueror_button").SetShowToolTipEvent(ui.__mem_func__(self.OverInToolTipButton), localeInfo.STAT_TOOLTIP_CONQUEROR_LEVEL)
			self.GetChild("change_conqueror_button").SetHideToolTipEvent(ui.__mem_func__(self.OverOutToolTipButton))

			# Passive button
			self.GetChild("passive_expanded_btn").SetShowToolTipEvent(ui.__mem_func__(self.OverInToolTipButton), localeInfo.STAT_TOOLTIP_PASSIVE)
			self.GetChild("passive_expanded_btn").SetHideToolTipEvent(ui.__mem_func__(self.OverOutToolTipButton))

			# Char info status img
			self.GetChild("Char_Info_Status_img").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_STAT)
			self.GetChild("Char_Info_Status_img").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			# Status plus label
			self.GetChild("Status_Plus_Label").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_POINT)
			self.GetChild("Status_Plus_Label").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			# Left side
			# Up
			# Base
			self.GetChild("HTH_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_IMG_CON)
			self.GetChild("HTH_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("INT_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_IMG_INT)
			self.GetChild("INT_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("STR_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_IMG_STR)
			self.GetChild("STR_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("DEX_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_IMG_DEX)
			self.GetChild("DEX_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			# Sungma
			self.GetChild("SUNGMA_STR_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_SUNGMA_STR)
			self.GetChild("SUNGMA_STR_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("SUNGMA_HP_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_SUNGMA_HP)
			self.GetChild("SUNGMA_HP_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("SUNGMA_MOVE_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_SUNGMA_MOVE)
			self.GetChild("SUNGMA_MOVE_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("SUNGMA_IMMUNE_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_SUNGMA_IMMUNE)
			self.GetChild("SUNGMA_IMMUNE_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			# Down
			self.GetChild("MSPD_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_MOVE_SPEED)
			self.GetChild("MSPD_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("ASPD_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_ATT_SPEED)
			self.GetChild("ASPD_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("CSPD_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_CAST_SPEED)
			self.GetChild("CSPD_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			# Right side
			# Up
			self.GetChild("HEL_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_HP)
			self.GetChild("HEL_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("SP_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_SP)
			self.GetChild("SP_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("ATT_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_ATT)
			self.GetChild("ATT_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("DEF_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_DEF)
			self.GetChild("DEF_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			# Down
			self.GetChild("MATT_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_MAG_ATT)
			self.GetChild("MATT_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("MDEF_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_MAG_DEF)
			self.GetChild("MDEF_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("ER_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_DODGE_PER)
			self.GetChild("ER_IMG").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			# Skill:
			self.GetChild("Active_Skill_Point_Label").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_POINT)
			self.GetChild("Active_Skill_Point_Label").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("Support_Skill_ToolTip").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_SUPPORT_SKILL)
			self.GetChild("Support_Skill_ToolTip").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			# Emoticon:
			self.GetChild("Action_Bar_Img").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_ACTION)
			self.GetChild("Action_Bar_Img").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("Reaction_Bar_Img").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_REACTION)
			self.GetChild("Reaction_Bar_Img").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

			self.GetChild("Special_Action_Bar_Img").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", localeInfo.STAT_TOOLTIP_SPECIAL_ACTION)
			self.GetChild("Special_Action_Bar_Img").SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", "")

		global SHOW_ONLY_ACTIVE_SKILL
		global HIDE_SUPPORT_SKILL_POINT
		if SHOW_ONLY_ACTIVE_SKILL or HIDE_SUPPORT_SKILL_POINT:
			self.GetChild("Support_Skill_Point_Label").Hide()

		self.soloEmotionSlot = self.GetChild("SoloEmotionSlot")
		self.dualEmotionSlot = self.GetChild("DualEmotionSlot")
		self.__SetEmotionSlot()

		if app.ENABLE_QUEST_RENEWAL:
			self.questScrollBar = self.GetChild("Quest_ScrollBar")
			self.questPageBoardWnd = self.GetChild("quest_object_board_window")
			self.questCategoryGroup = QuestCategoryGroup(self.questPageBoardWnd, self.questScrollBar)
		else:
			self.questShowingStartIndex = 0
			self.questScrollBar = self.GetChild("Quest_ScrollBar")
			self.questScrollBar.SetScrollEvent(ui.__mem_func__(self.OnQuestScroll))
			self.questSlot = self.GetChild("Quest_Slot")
			for i in xrange(quest.QUEST_MAX_NUM):
				self.questSlot.HideSlotBaseImage(i)
				self.questSlot.SetCoverButton(i,\
					"d:/ymir work/ui/game/quest/slot_button_01.sub",\
					"d:/ymir work/ui/game/quest/slot_button_02.sub",\
					"d:/ymir work/ui/game/quest/slot_button_03.sub",\
					"d:/ymir work/ui/game/quest/slot_button_03.sub", True)

			self.questNameList = []
			self.questLastTimeList = []
			self.questLastCountList = []
			for i in xrange(quest.QUEST_MAX_NUM):
				self.questNameList.append(self.GetChild("Quest_Name_0" + str(i)))
				self.questLastTimeList.append(self.GetChild("Quest_LastTime_0" + str(i)))
				self.questLastCountList.append(self.GetChild("Quest_LastCount_0" + str(i)))
		
		if app.ENABLE_NEW_DETAILS_GUI:
			MainBoard = self.GetChild("board")
			self.ExpandBtn = ui.MakeButton(MainBoard, 240, 120, "", "d:/ymir work/ui/game/belt_inventory/", "btn_minimize_normal.tga", "btn_minimize_over.tga", "btn_minimize_down.tga")
			self.ExpandBtn.SetEvent(ui.__mem_func__(self.__ClickExpandButton))
			self.MinimizeBtn = ui.MakeButton(MainBoard, 240, 120, "", "d:/ymir work/ui/game/belt_inventory/", "btn_expand_normal.tga", "btn_expand_over.tga", "btn_expand_down.tga")
			self.MinimizeBtn.SetEvent(ui.__mem_func__(self.__ClickMinimizeButton))
		
	def OpenFastRead(self):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			interface.OpenAutoSkillReader()
	
	if app.ENABLE_YOHARA_SYSTEM:
		def OverInToolTipButton(self, arg):
			arglen = len(str(arg))
			pos_x, pos_y = wndMgr.GetMousePosition()

			self.buttontooltip.ClearToolTip()
			self.buttontooltip.SetThinBoardSize(11 * arglen)
			self.buttontooltip.SetToolTipPosition(pos_x + 50, pos_y + 50)
			self.buttontooltip.AppendTextLine(arg)
			self.buttontooltip.Show()
			self.ShowButtonToolTip = True

		def OverOutToolTipButton(self):
			self.buttontooltip.Hide()
			self.ShowButtonToolTip = False

		def ButtonToolTipProgress(self) :
			if self.buttontooltip and self.ShowButtonToolTip:
				pos_x, pos_y = wndMgr.GetMousePosition()
				self.buttontooltip.SetToolTipPosition(pos_x + 50, pos_y + 50)

		def EventProgress(self, event_type, arg) :
			#print "EventProcess %s, %s" %(event_type, slot)
			if "mouse_click" == event_type :
				self.OverInToolTipButton(arg)
			elif "mouse_over_in" == event_type :
				self.OverInToolTipButton(arg)
			elif "mouse_over_out" == event_type :
				self.OverOutToolTipButton()
			else :
				print " uiCharacter.py ::EventProgress : FALSE"

	if app.ENABLE_NEW_DETAILS_GUI:
		def __InitCharacterDetailsUIButton(self):
			self.ExpandBtn.Show()
			self.MinimizeBtn.Hide()
		def __ClickExpandButton(self):
			if not self.chDetailsWnd:
				self.chDetailsWnd = uiCharacterDetails.CharacterDetailsUI()
			self.chDetailsWnd.Show()
			self.SetTop()
			self.ExpandBtn.Hide()
			self.MinimizeBtn.Show()
		def __ClickMinimizeButton(self):
			self.chDetailsWnd.Hide()
			self.MinimizeBtn.Hide()
			self.ExpandBtn.Show()
		def OnMoveWindow(self, x, y):
			if self.chDetailsWnd:
				self.chDetailsWnd.AdjustPosition(x, y)

	def __SetSkillSlotEvent(self):
		for skillPageValue in self.skillPageDict.itervalues():
			skillPageValue.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
			skillPageValue.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectSkill))
			skillPageValue.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
			skillPageValue.SetUnselectItemSlotEvent(ui.__mem_func__(self.ClickSkillSlot))
			skillPageValue.SetUseSlotEvent(ui.__mem_func__(self.ClickSkillSlot))
			skillPageValue.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			skillPageValue.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
			skillPageValue.SetPressedSlotButtonEvent(ui.__mem_func__(self.OnPressedSlotButton))
			skillPageValue.AppendSlotButton("d:/ymir work/ui/game/windows/btn_plus_up.sub",\
											"d:/ymir work/ui/game/windows/btn_plus_over.sub",\
											"d:/ymir work/ui/game/windows/btn_plus_down.sub")

	def __SetEmotionSlot(self):

		self.emotionToolTip = uiToolTip.ToolTip()

		for slot in (self.soloEmotionSlot, self.dualEmotionSlot):
			slot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
			slot.SetSelectItemSlotEvent(ui.__mem_func__(self.__SelectEmotion))
			slot.SetUnselectItemSlotEvent(ui.__mem_func__(self.__ClickEmotionSlot))
			slot.SetUseSlotEvent(ui.__mem_func__(self.__ClickEmotionSlot))
			slot.SetOverInItemEvent(ui.__mem_func__(self.__OverInEmotion))
			slot.SetOverOutItemEvent(ui.__mem_func__(self.__OverOutEmotion))
			slot.AppendSlotButton("d:/ymir work/ui/game/windows/btn_plus_up.sub",\
											"d:/ymir work/ui/game/windows/btn_plus_over.sub",\
											"d:/ymir work/ui/game/windows/btn_plus_down.sub")

		for slotIdx, datadict in emotion.EMOTION_DICT.items():
			emotionIdx = slotIdx

			slot = self.soloEmotionSlot
			if slotIdx > 50:
				slot = self.dualEmotionSlot

			slot.SetEmotionSlot(slotIdx, emotionIdx)
			slot.SetCoverButton(slotIdx)

	def __SelectEmotion(self, slotIndex):
		if not slotIndex in emotion.EMOTION_DICT:
			return

		if app.IsPressed(app.DIK_LCONTROL):
			player.RequestAddToEmptyLocalQuickSlot(player.SLOT_TYPE_EMOTION, slotIndex)
			return

		mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_EMOTION, slotIndex, slotIndex)

	def __ClickEmotionSlot(self, slotIndex):
		print "click emotion"
		if not slotIndex in emotion.EMOTION_DICT:
			return

		print "check acting"
		if player.IsActingEmotion():
			return

		command = emotion.EMOTION_DICT[slotIndex]["command"]
		print "command", command

		if slotIndex > 50:
			vid = player.GetTargetVID()

			if 0 == vid or vid == player.GetMainCharacterIndex() or chr.IsNPC(vid) or chr.IsEnemy(vid):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EMOTION_CHOOSE_ONE)
				return

			command += " " + chr.GetNameByVID(vid)

		print "send_command", command
		net.SendChatPacket(command)

	def ActEmotion(self, emotionIndex):
		self.__ClickEmotionSlot(emotionIndex)

	def __OverInEmotion(self, slotIndex):
		if self.emotionToolTip:

			if not slotIndex in emotion.EMOTION_DICT:
				return

			self.emotionToolTip.ClearToolTip()
			self.emotionToolTip.SetTitle(emotion.EMOTION_DICT[slotIndex]["name"])
			self.emotionToolTip.AlignHorizonalCenter()
			self.emotionToolTip.ShowToolTip()

	def __OverOutEmotion(self):
		if self.emotionToolTip:
			self.emotionToolTip.HideToolTip()

	def __BindEvent(self):
		for i in xrange(len(self.skillGroupButton)):
			self.skillGroupButton[i].SetEvent(lambda arg=i: self.__SelectSkillGroup(arg))

		if not app.ENABLE_QUEST_RENEWAL:
			self.RefreshQuest()
		self.__HideJobToolTip()

		for (tabKey, tabButton) in self.tabButtonDict.items():
			tabButton.SetEvent(ui.__mem_func__(self.__OnClickTabButton), tabKey)
			if app.ENABLE_YOHARA_SYSTEM:
				tabButton.SetShowToolTipEvent(ui.__mem_func__(self.__OverInTabButton), tabKey)
				tabButton.SetHideToolTipEvent(ui.__mem_func__(self.OverOutToolTipButton), tabKey)

		if app.ENABLE_YOHARA_SYSTEM:
			for (infoTabKey, infoTabButton) in self.tabInfoButtonDict.items():
				infoTabButton.SetEvent(ui.__mem_func__(self.__OnClickTabInfoButton), infoTabKey)

			for (sungmaStatusPlusKey, sungmaStatusPlusButton) in self.sungmaStatusPlusButtonDict.items():
				sungmaStatusPlusButton.SAFE_SetEvent(self.__OnClickSungmaStatusPlusButton, sungmaStatusPlusKey)
				sungmaStatusPlusButton.SetShowToolTipEvent(ui.__mem_func__(self.__OverInSungmaStatButton), sungmaStatusPlusKey)
				sungmaStatusPlusButton.SetHideToolTipEvent(ui.__mem_func__(self.OverOutToolTipButton), sungmaStatusPlusKey)

		for (statusPlusKey, statusPlusButton) in self.statusPlusButtonDict.items():
			statusPlusButton.SAFE_SetEvent(self.__OnClickStatusPlusButton, statusPlusKey)
			statusPlusButton.ShowToolTip = lambda arg=statusPlusKey: self.__OverInStatButton(arg)
			statusPlusButton.HideToolTip = lambda arg=statusPlusKey: self.__OverOutStatButton()

		for (statusMinusKey, statusMinusButton) in self.statusMinusButtonDict.items():
			statusMinusButton.SAFE_SetEvent(self.__OnClickStatusMinusButton, statusMinusKey)
			statusMinusButton.ShowToolTip = lambda arg=statusMinusKey: self.__OverInStatMinusButton(arg)
			statusMinusButton.HideToolTip = lambda arg=statusMinusKey: self.__OverOutStatMinusButton()

		for titleBarValue in self.titleBarDict.itervalues():
			titleBarValue.SetCloseEvent(ui.__mem_func__(self.Close))

		if not app.ENABLE_QUEST_RENEWAL:
			self.questSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.__SelectQuest))
			self.questSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__SelectQuest))

		if app.ENABLE_YOHARA_SYSTEM:
			self.relictrefine.SetEvent(ui.__mem_func__(self.__OpenRelictWindow))

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			self.__LoadScript("UIScript/CharacterWindow.py")

			self.__BindObject()
			self.__BindEvent()
		except:
			import exception
			exception.Abort("CharacterWindow.__LoadWindow")

		#self.tabButtonDict["EMOTICON"].Disable()
		self.SetState("STATUS")
		if app.ENABLE_YOHARA_SYSTEM:
			self.SetInfoState("BASE")
			self.buttontooltip = uiToolTip.ToolTip()
			self.buttontooltip.ClearToolTip()

	def Destroy(self):
		self.ClearDictionary()
		
		if app.ENABLE_NEW_DETAILS_GUI:
			if self.chDetailsWnd:
				self.chDetailsWnd.Destroy()
				self.chDetailsWnd=None

		self.__Initialize()


	def Close(self):
		if None != self.toolTipSkill:
			self.toolTipSkill.Hide()
		
		if app.ENABLE_NEW_DETAILS_GUI:
			if self.chDetailsWnd:
				self.isOpenedDetailsWnd = self.chDetailsWnd.IsShow()
				self.chDetailsWnd.Close()

		if app.ENABLE_YOHARA_SYSTEM:
			if self.buttontooltip:
				self.buttontooltip.Hide()
				self.ShowButtonToolTip = False

		self.Hide()

	def SetSkillToolTip(self, toolTipSkill):
		self.toolTipSkill = toolTipSkill

	def __OnClickStatusPlusButton(self, statusKey):
		cmd = self.statusPlusCommandDict[statusKey]

		if app.IsPressed(app.DIK_LCONTROL):
			cmd = cmd + "10"
		else:
			cmd = cmd + "1"
			
		net.SendChatPacket(cmd)

	def __OnClickStatusMinusButton(self, statusKey):
		try:
			statusMinusCommand=self.statusMinusCommandDict[statusKey]
			if app.DISABLE_OLD_MINUS:
				if app.IsPressed(app.DIK_LCONTROL):
					statusMinusCommand += " 10"
				else:
					statusMinusCommand += " 1"
				
			net.SendChatPacket(statusMinusCommand)
		except KeyError, msg:
			dbg.TraceError("CharacterWindow.__OnClickStatusMinusButton KeyError: %s", msg)


	def __OnClickTabButton(self, stateKey):
		self.SetState(stateKey)

	def SetState(self, stateKey):
		self.state = stateKey

		for (tabKey, tabButton) in self.tabButtonDict.items():
			if stateKey!=tabKey:
				tabButton.SetUp()

		for tabValue in self.tabDict.itervalues():
			tabValue.Hide()

		for pageValue in self.pageDict.itervalues():
			pageValue.Hide()

		for titleBarValue in self.titleBarDict.itervalues():
			titleBarValue.Hide()

		self.titleBarDict[stateKey].Show()
		self.tabDict[stateKey].Show()
		self.pageDict[stateKey].Show()

	def GetState(self):
		return self.state

	if app.ENABLE_YOHARA_SYSTEM:
		def __OnClickSungmaStatusPlusButton(self, sungmaStatusKey):
			try:
				sungmaStatusPlusCommand=self.sungmaStatusPlusCommandDict[sungmaStatusKey]
				net.SendChatPacket(sungmaStatusPlusCommand)
			except KeyError, msg:
				dbg.TraceError("CharacterWindow.__OnClickSungmaStatusPlusButton KeyError: %s", msg)

		def __OnClickTabInfoButton(self, infoStateKey):
			self.SetInfoState(infoStateKey)

		def SetInfoState(self, infoStateKey):
			self.infoState = infoStateKey

			for (infoTabKey, infoTabButton) in self.tabInfoButtonDict.items():
				if infoStateKey!=infoTabKey:
					infoTabButton.SetUp()
				else:
					infoTabButton.Down()

			for pageValue in self.InfoPageDict.itervalues():
				pageValue.Hide()

			self.InfoPageDict[infoStateKey].Show() ## Slots info
			self.RefreshStatus()

	def __GetTotalAtkText(self):
		minAtk=player.GetStatus(player.ATT_MIN)
		maxAtk=player.GetStatus(player.ATT_MAX)
		atkBonus=player.GetStatus(player.ATT_BONUS)
		attackerBonus=player.GetStatus(player.ATTACKER_BONUS)

		if minAtk==maxAtk:
			return "%d" % (minAtk+atkBonus+attackerBonus)
		else:
			return "%d-%d" % (minAtk+atkBonus+attackerBonus, maxAtk+atkBonus+attackerBonus)

	def __GetTotalMagAtkText(self):
		minMagAtk=player.GetStatus(player.MAG_ATT)+player.GetStatus(player.MIN_MAGIC_WEP)
		maxMagAtk=player.GetStatus(player.MAG_ATT)+player.GetStatus(player.MAX_MAGIC_WEP)

		if minMagAtk==maxMagAtk:
			return "%d" % (minMagAtk)
		else:
			return "%d-%d" % (minMagAtk, maxMagAtk)

	def __GetTotalDefText(self):
		defValue=player.GetStatus(player.DEF_GRADE)
		if constInfo.ADD_DEF_BONUS_ENABLE:
			defValue+=player.GetStatus(player.DEF_BONUS)
		return "%d" % (defValue)

	def RefreshStatus(self):
		if self.isLoaded==0:
			return

		try:
			if app.ENABLE_YOHARA_SYSTEM and self.infoState != "BASE":
				self.GetChild("Level_Value").SetText(str(player.GetStatus(player.CONQUEROR_LEVEL)))
				self.GetChild("Exp_Value").SetText(str(unsigned32(player.GetConquerorEXP())))
				self.GetChild("RestExp_Value").SetText(str(unsigned32(player.GetStatus(player.CONQUEROR_NEXT_EXP)) - unsigned32(player.GetStatus(player.CONQUEROR_EXP))))
			else:
				self.GetChild("Level_Value").SetText(str(player.GetStatus(player.LEVEL)))
				self.GetChild("Exp_Value").SetText(str(unsigned32(player.GetEXP())))
				self.GetChild("RestExp_Value").SetText(str(unsigned32(player.GetStatus(player.NEXT_EXP)) - unsigned32(player.GetStatus(player.EXP))))

			self.GetChild("HP_Value").SetText(str(player.GetStatus(player.HP)) + '/' + str(player.GetStatus(player.MAX_HP)))
			self.GetChild("SP_Value").SetText(str(player.GetStatus(player.SP)) + '/' + str(player.GetStatus(player.MAX_SP)))

			self.GetChild("STR_Value").SetText(str(player.GetStatus(player.ST)))
			self.GetChild("DEX_Value").SetText(str(player.GetStatus(player.DX)))
			self.GetChild("HTH_Value").SetText(str(player.GetStatus(player.HT)))
			self.GetChild("INT_Value").SetText(str(player.GetStatus(player.IQ)))

			self.GetChild("ATT_Value").SetText(self.__GetTotalAtkText())
			self.GetChild("DEF_Value").SetText(self.__GetTotalDefText())

			if app.ENABLE_YOHARA_SYSTEM:
				self.GetChild("sungma_str_value").SetText(str(player.GetStatus(player.SUNGMA_STR)))
				self.GetChild("sungma_hp_value").SetText(str(player.GetStatus(player.SUNGMA_HP)))
				self.GetChild("sungma_move_value").SetText(str(player.GetStatus(player.SUNGMA_MOVE)))
				self.GetChild("sungma_immune_value").SetText(str(player.GetStatus(player.SUNGMA_INMUNE)))

			self.GetChild("MATT_Value").SetText(self.__GetTotalMagAtkText())
			#self.GetChild("MATT_Value").SetText(str(player.GetStatus(player.MAG_ATT)))

			self.GetChild("MDEF_Value").SetText(str(player.GetStatus(player.MAG_DEF)))
			self.GetChild("ASPD_Value").SetText(str(player.GetStatus(player.ATT_SPEED)))
			self.GetChild("MSPD_Value").SetText(str(player.GetStatus(player.MOVING_SPEED)))
			self.GetChild("CSPD_Value").SetText(str(player.GetStatus(player.CASTING_SPEED)))
			self.GetChild("ER_Value").SetText(str(player.GetStatus(player.EVADE_RATE)))
		except:
			#import exception
			#exception.Abort("CharacterWindow.RefreshStatus.BindObject")
			## °ÔÀÓÀÌ Æ¨°Ü ¹ö¸²
			pass

		self.__RefreshStatusPlusButtonList()
		self.__RefreshStatusMinusButtonList()
		self.RefreshAlignment()

		if self.refreshToolTip:
			self.refreshToolTip()
		
		if app.ENABLE_NEW_DETAILS_GUI:
			if self.chDetailsWnd:
				if self.chDetailsWnd.IsShow():
					self.chDetailsWnd.Refresh()

	def __RefreshStatusPlusButtonList(self):
		if self.isLoaded==0:
			return

		if app.ENABLE_YOHARA_SYSTEM and self.infoState != "BASE":
			statusPlusPoint=player.GetStatus(player.CONQUEROR_POINT)
		else:
			statusPlusPoint=player.GetStatus(player.STAT)

		if statusPlusPoint>0:
			self.statusPlusValue.SetText(str(statusPlusPoint))
			self.statusPlusLabel.Show()
			if app.ENABLE_YOHARA_SYSTEM and self.infoState != "BASE":
				self.ShowSungmaStatusPlusButtonList()
			else:
				self.ShowStatusPlusButtonList()
		else:
			self.statusPlusValue.SetText(str(0))
			self.statusPlusLabel.Hide()
			if app.ENABLE_YOHARA_SYSTEM and self.infoState != "BASE":
				self.HideSungmaStatusPlusButtonList()
			else:
				self.HideStatusPlusButtonList()

	def __RefreshStatusMinusButtonList(self):
		if self.isLoaded==0:
			return

		if app.DISABLE_OLD_MINUS:
			self.__ShowStatusMinusButtonList()
		else:
			statusMinusPoint=self.__GetStatMinusPoint()
			if statusMinusPoint>0:
				self.__ShowStatusMinusButtonList()
			else:
				self.__HideStatusMinusButtonList()

	def RefreshAlignment(self):
		point, grade = player.GetAlignmentData()

		import colorInfo
		COLOR_DICT = {	0 : colorInfo.TITLE_RGB_GOOD_4,
						1 : colorInfo.TITLE_RGB_GOOD_3,
						2 : colorInfo.TITLE_RGB_GOOD_2,
						3 : colorInfo.TITLE_RGB_GOOD_1,
						4 : colorInfo.TITLE_RGB_NORMAL,
						5 : colorInfo.TITLE_RGB_EVIL_1,
						6 : colorInfo.TITLE_RGB_EVIL_2,
						7 : colorInfo.TITLE_RGB_EVIL_3,
						8 : colorInfo.TITLE_RGB_EVIL_4, }
		colorList = COLOR_DICT.get(grade, colorInfo.TITLE_RGB_NORMAL)
		gradeColor = ui.GenerateColor(colorList[0], colorList[1], colorList[2])

		self.toolTipAlignment.ClearToolTip()
		self.toolTipAlignment.AutoAppendTextLine(localeInfo.TITLE_NAME_LIST[grade], gradeColor)
		self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGNMENT_NAME + str(point))
		self.toolTipAlignment.AlignHorizonalCenter()

	def __ShowStatusMinusButtonList(self):
		for (stateMinusKey, statusMinusButton) in self.statusMinusButtonDict.items():
			statusMinusButton.Show()

	def __HideStatusMinusButtonList(self):
		for (stateMinusKey, statusMinusButton) in self.statusMinusButtonDict.items():
			statusMinusButton.Hide()

	def ShowStatusPlusButtonList(self):
		for (statePlusKey, statusPlusButton) in self.statusPlusButtonDict.items():
			statusPlusButton.Show()

	def HideStatusPlusButtonList(self):
		for (statePlusKey, statusPlusButton) in self.statusPlusButtonDict.items():
			statusPlusButton.Hide()

	if app.ENABLE_YOHARA_SYSTEM:
		def ShowSungmaStatusPlusButtonList(self):
			for (sungmaStatePlusKey, sungmaStatusPlusButton) in self.sungmaStatusPlusButtonDict.items():
				sungmaStatusPlusButton.Show()

		def HideSungmaStatusPlusButtonList(self):
			for (sungmaStatePlusKey, sungmaStatusPlusButton) in self.sungmaStatusPlusButtonDict.items():
				sungmaStatusPlusButton.Hide()

		#if app.ENABLE_PASSIVE_SYSTEM:
		def __OpenRelictWindow(self):
			pass

	def SelectSkill(self, skillSlotIndex):
		if app.__BUFFI_SUPPORT__ and self.PAGE_BUFFI == self.curSelectedSkillGroup:
			return

		mouseController = mouseModule.mouseController

		if False == mouseController.isAttached():

			srcSlotIndex = self.__RealSkillSlotToSourceSlot(skillSlotIndex)
			selectedSkillIndex = player.GetSkillIndex(srcSlotIndex)
			selectedSkillGrade = player.GetSkillGrade(srcSlotIndex)

			if selectedSkillIndex == 121:
				if selectedSkillGrade >= 3:
					if self.boardLeader.IsShow():
						self.boardLeader.Hide()
						self.skillPageDict["SUPPORT"].DeactivateSlot(skillSlotIndex)
					else:
						self.boardLeader.Show()
						self.skillPageDict["SUPPORT"].ActivateSlot(skillSlotIndex, (238.00 / 255.0), (11.00 / 255.0), (11.00 / 255.0), 1.0)
					return
				else:
					return

			if skill.CanUseSkill(selectedSkillIndex):

				if app.IsPressed(app.DIK_LCONTROL):

					player.RequestAddToEmptyLocalQuickSlot(player.SLOT_TYPE_SKILL, srcSlotIndex)
					return

				mouseController.AttachObject(self, player.SLOT_TYPE_SKILL, srcSlotIndex, selectedSkillIndex)

		else:

			mouseController.DeattachObject()

	def SelectEmptySlot(self, SlotIndex):
		mouseModule.mouseController.DeattachObject()

	## ToolTip
	def OverInItem(self, slotNumber):
		if mouseModule.mouseController.isAttached():
			return

		if 0 == self.toolTipSkill:
			return

		srcSlotIndex = self.__RealSkillSlotToSourceSlot(slotNumber)
		skillIndex = player.GetSkillIndex(srcSlotIndex)
		skillLevel = player.GetSkillLevel(srcSlotIndex)
		skillGrade = player.GetSkillGrade(srcSlotIndex)
		skillType = skill.GetSkillType(skillIndex)

		## ACTIVE
		if skill.SKILL_TYPE_ACTIVE == skillType:
			overInSkillGrade = self.__GetSkillGradeFromSlot(slotNumber)

			if overInSkillGrade == skill.SKILL_GRADE_COUNT-1 and skillGrade == skill.SKILL_GRADE_COUNT:
				self.toolTipSkill.SetSkillNew(srcSlotIndex, skillIndex, skillGrade, skillLevel)
			elif overInSkillGrade == skillGrade:
				self.toolTipSkill.SetSkillNew(srcSlotIndex, skillIndex, overInSkillGrade, skillLevel)
			else:
				self.toolTipSkill.SetSkillOnlyName(srcSlotIndex, skillIndex, overInSkillGrade)

		else:
			self.toolTipSkill.SetSkillNew(srcSlotIndex, skillIndex, skillGrade, skillLevel)

	def OverOutItem(self):
		if 0 != self.toolTipSkill:
			self.toolTipSkill.HideToolTip()

	## Quest
	if not app.ENABLE_QUEST_RENEWAL:
		## Quest
		def __SelectQuest(self, slotIndex):
			questIndex = quest.GetQuestIndex(self.questShowingStartIndex + slotIndex)

			import event
			event.QuestButtonClick(-2147483648 + questIndex)

	if app.ENABLE_QUEST_RENEWAL:
		def RefreshQuest(self, quest_type, quest_index):
			if not self.isLoaded or not self.questCategoryGroup:
				return

			try:
				(quest_type, is_confirmed, quest_name, quest_icon, quest_counter_name, quest_counter_value) = quest.GetQuestData(quest_index)
				(last_clock_name, last_clock_time) = quest.GetQuestLastTime(quest_index)

				if self.questCategoryGroup:
					self.questCategoryGroup.ReplaceQuest(quest_type, quest_index, is_confirmed, [quest_name, [last_clock_name, last_clock_time], [quest_counter_name, quest_counter_value]])
			except TypeError:
				return

		def DeleteQuest(self, quest_type, quest_index):
			self.questCategoryGroup.DeleteQuest(quest_type, quest_index)

		def OpenQuestCategory(self, quest_type, close_all):
			self.questCategoryGroup.OpenCategory(quest_type, close_all)
	else:
		def RefreshQuest(self):
			if self.isLoaded == 0:
				return

			self.OnQuestScroll()

			questCount = quest.GetQuestCount()
			questRange = range(quest.QUEST_MAX_NUM)

			if questCount > quest.QUEST_MAX_NUM:
				self.questScrollBar.Show()
			else:
				self.questScrollBar.Hide()

			for i in questRange[:questCount]:
				(questName, questIcon, questCounterName, questCounterValue) = quest.GetQuestData(self.questShowingStartIndex + i)

				#if questName[0] == "*":
					#questName = questName[1:]

				self.questNameList[i].SetText(questName)
				self.questNameList[i].Show()
				self.questLastCountList[i].Show()
				self.questLastTimeList[i].Show()

				if len(questCounterName) > 0:
					self.questLastCountList[i].SetText("%s : %d" % (questCounterName, questCounterValue))
				else:
					self.questLastCountList[i].SetText("")

				## Icon
				self.questSlot.SetSlot(i, i, 1, 1, questIcon)

			for i in questRange[questCount:]:
				self.questNameList[i].Hide()
				self.questLastTimeList[i].Hide()
				self.questLastCountList[i].Hide()
				self.questSlot.ClearSlot(i)
				self.questSlot.HideSlotBaseImage(i)

			self.__UpdateQuestClock()

		def __UpdateQuestClock(self):
			if "QUEST" == self.state:
				# QUEST_LIMIT_COUNT_BUG_FIX
				for i in xrange(min(quest.GetQuestCount(), quest.QUEST_MAX_NUM)):
				# END_OF_QUEST_LIMIT_COUNT_BUG_FIX
					(lastName, lastTime) = quest.GetQuestLastTime(i + self.questShowingStartIndex)

					clockText = localeInfo.QUEST_UNLIMITED_TIME
					if len(lastName) > 0:

						if lastTime <= 0:
							clockText = localeInfo.QUEST_TIMEOVER
						else:
							questLastMinute = lastTime / 60
							questLastSecond = lastTime % 60

							clockText = lastName + " : "

							if questLastMinute > 0:
								clockText += str(questLastMinute) + localeInfo.QUEST_MIN
								if questLastSecond > 0:
									clockText += " "

							if questLastSecond > 0:
								clockText += str(questLastSecond) + localeInfo.QUEST_SEC

					self.questLastTimeList[i].SetText(clockText)

	def __GetStatMinusPoint(self):
		POINT_STAT_RESET_COUNT = 112
		return player.GetStatus(POINT_STAT_RESET_COUNT)

	def __OverInStatMinusButton(self, stat):
		try:
			if app.DISABLE_OLD_MINUS:
				self.__ShowStatToolTip(self.STAT_MINUS_DESCRIPTION[stat], localeInfo.EMOJI_CHARACTER_STATS_DELETE, True)
			else:
				self.__ShowStatToolTip(self.STAT_MINUS_DESCRIPTION[stat] % self.__GetStatMinusPoint())
		except KeyError:
			pass

		self.refreshToolTip = lambda arg=stat: self.__OverInStatMinusButton(arg)

	def __OverOutStatMinusButton(self):
		self.__HideStatToolTip()
		self.refreshToolTip = 0

	def __OverInStatButton(self, stat):	
		try:
			self.__ShowStatToolTip(self.STAT_DESCRIPTION[stat], localeInfo.EMOJI_CHARACTER_STATS_ADD, True)

		except KeyError:
			pass

	def __OverOutStatButton(self):
		self.__HideStatToolTip()

	if app.ENABLE_YOHARA_SYSTEM:
		def __OverInSungmaStatButton(self, sungmaStat):
			try:
				self.OverInToolTipButton(self.SUNGMA_STAT_DESCRIPTION[sungmaStat])
			except KeyError:
				pass

	def __ShowStatToolTip(self, statDesc, statDesc2 = False, arg2 = False):
		self.toolTip.ClearToolTip()
		self.toolTip.AppendTextLine(statDesc)
		
		if arg2 == True:
			self.toolTip.AppendTextLine(statDesc2)
			
		self.toolTip.Show()

	def __HideStatToolTip(self):
		self.toolTip.Hide()

	if app.ENABLE_YOHARA_SYSTEM:
		def __OverInTabButton(self, tab):
			try:
				self.OverInToolTipButton(self.TAB_DESCRIPTION[tab])
			except KeyError:
				pass

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnUpdate(self):
		if not app.ENABLE_QUEST_RENEWAL:
			self.__UpdateQuestClock()
		if app.ENABLE_YOHARA_SYSTEM:
			self.ButtonToolTipProgress()

	def RefreshNormalSkillCoolTime(self, skillPage, slotCount):
		startSlotIndex = skillPage.GetStartIndex()
		
		getSkillType=skill.GetSkillType
		getSkillIndex=player.GetSkillIndex
		getSkillGrade=player.GetSkillGrade
		getSkillLevel=player.GetSkillLevel
		
		for i in xrange(slotCount+1):
			slotIndex = i + startSlotIndex
			skillIndex = getSkillIndex(slotIndex)

			if 0 == skillIndex:
				continue

			skillGrade = getSkillGrade(slotIndex)
			skillLevel = getSkillLevel(slotIndex)
			skillType = getSkillType(skillIndex)
			
			slot_index_old = None
			for j in xrange(skill.SKILL_GRADE_COUNT):
				realSlotIndex = self.__GetRealSkillSlot(j, slotIndex)
				if(skillGrade != j):
					slot_index_old = realSlotIndex
				else:
					if slot_index_old:
						skillPage.UpdateSlotCoolTime(skillType, realSlotIndex, slot_index_old)
					break

	## Skill Process
	def __RefreshSkillPage(self, name, slotCount):
		global SHOW_LIMIT_SUPPORT_SKILL_LIST

		skillPage = self.skillPageDict[name]

		startSlotIndex = skillPage.GetStartIndex()
		if "ACTIVE" == name:
			if self.PAGE_HORSE == self.curSelectedSkillGroup:
				startSlotIndex += slotCount
				for x in xrange(1,7):
					self.GetChild("buffi_slot_hide_%d" % x).Hide()
				self.GetChild("buffi_skill_page").Hide()
			elif app.__BUFFI_SUPPORT__ and self.PAGE_BUFFI == self.curSelectedSkillGroup:
				startSlotIndex = slotCount + slotCount
				self.GetChild("buffi_skill_page").Show()

		getSkillType=skill.GetSkillType
		getSkillIndex=player.GetSkillIndex
		getSkillGrade=player.GetSkillGrade
		getSkillLevel=player.GetSkillLevel
		getSkillMaxLevel = skill.GetSkillMaxLevel
		getSkillLevelUpPoint=skill.GetSkillLevelUpPoint

		for i in xrange(slotCount):
			slotIndex = i
			skillIndex = getSkillIndex(slotIndex + startSlotIndex)
			for j in xrange(skill.SKILL_GRADE_COUNT):
				skillPage.ClearSlot(self.__GetRealSkillSlot(j, i))

			if not skillIndex:
				continue

			skillType = getSkillType(skillIndex)
			skillGrade = getSkillGrade(slotIndex + startSlotIndex)
			skillLevel = getSkillLevel(slotIndex + startSlotIndex)
			skillType = getSkillType(skillIndex)

			if skillIndex == player.SKILL_INDEX_RIDING:
				if skillGrade == 1:
					skillLevel += 19
				elif skillGrade == 2:
					skillLevel += 29
				elif skillGrade == 3:
					skillLevel = 40

				skGr = max(skillLevel - 1, 0)
				skillPage.SetSkillSlotNew(slotIndex + startSlotIndex, skillIndex, skGr, skillLevel)
				skillPage.SetSlotCount(slotIndex + startSlotIndex, skillLevel)

			elif skillType == skill.SKILL_TYPE_ACTIVE:

				for j in xrange(skill.SKILL_GRADE_COUNT):
					realSlotIndex = self.__GetRealSkillSlot(j, slotIndex)
					skillPage.SetSkillSlotNew(realSlotIndex, skillIndex, j, skillLevel)
					skillPage.SetCoverButton(realSlotIndex)

					if (skillGrade == skill.SKILL_GRADE_COUNT) and j == (skill.SKILL_GRADE_COUNT-1):
						skillPage.SetSlotCountNew(realSlotIndex, skillGrade, skillLevel)
					elif (not self.__CanUseSkillNow()) or (skillGrade != j):
						skillPage.SetSlotCount(realSlotIndex, 0)
						skillPage.DisableCoverButton(realSlotIndex)
						skillPage.DeactivateSlot(realSlotIndex)
					else:
						skillPage.SetSlotCountNew(realSlotIndex, skillGrade, skillLevel)

					if player.IsSkillActive(slotIndex + startSlotIndex) and (skillGrade == j): # hotfix
						skillPage.ActivateSlot(realSlotIndex)
					# elif not player.IsSkillActive(slotIndex + startSlotIndex):
						# self.RefreshNormalSkillCoolTime(skillPage, slotCount)
			else:
				if not SHOW_LIMIT_SUPPORT_SKILL_LIST or skillIndex in SHOW_LIMIT_SUPPORT_SKILL_LIST:
					realSlotIndex = self.__GetETCSkillRealSlotIndex(slotIndex + startSlotIndex)
					skillPage.SetSkillSlot(realSlotIndex, skillIndex, skillLevel)
					skillPage.SetSlotCountNew(realSlotIndex, skillGrade, skillLevel)

					if skill.CanUseSkill(skillIndex):
						skillPage.SetCoverButton(realSlotIndex)
						
			if skillIndex == 166 and skillLevel < 1:
				self.GetChild("buffi_slot_hide_1").Show()
				self.GetChild("buffi_slot_hide_2").Show()
				self.GetChild("buffi_slot_hide_3").Show()

			if skillIndex == 167 and skillLevel < 1:
				self.GetChild("buffi_slot_hide_4").Show()
				self.GetChild("buffi_slot_hide_5").Show()
				self.GetChild("buffi_slot_hide_6").Show()
				

			skillPage.RefreshSlot()


	def RefreshSkill(self):

		if self.isLoaded==0:
			return

		if self.__IsChangedHorseRidingSkillLevel():
			self.RefreshCharacter()
			return
			
		self.GetChild("buffi_skill_page").Hide()
		
		for muiezenaris in xrange(1,7):
			self.GetChild("buffi_slot_hide_%d" % muiezenaris).Hide()

		global SHOW_ONLY_ACTIVE_SKILL
		if SHOW_ONLY_ACTIVE_SKILL:
			self.__RefreshSkillPage("ACTIVE", self.ACTIVE_PAGE_SLOT_COUNT)
		else:
			self.__RefreshSkillPage("ACTIVE", self.ACTIVE_PAGE_SLOT_COUNT)
			self.__RefreshSkillPage("SUPPORT", self.SUPPORT_PAGE_SLOT_COUNT)

		self.RefreshSkillPlusButtonList()

	def CanShowPlusButton(self, skillIndex, skillLevel, curStatPoint):

		## ½ºÅ³ÀÌ ÀÖÀ¸¸é
		if 0 == skillIndex:
			return False

		## ·¹º§¾÷ Á¶°ÇÀ» ¸¸Á·ÇÑ´Ù¸é
		if not skill.CanLevelUpSkill(skillIndex, skillLevel):
			return False

		return True

	def __RefreshSkillPlusButton(self, name):
		global HIDE_SUPPORT_SKILL_POINT
		if HIDE_SUPPORT_SKILL_POINT and "SUPPORT" == name:
			return

		slotWindow = self.skillPageDict[name]
		slotWindow.HideAllSlotButton()

		slotStatType = self.skillPageStatDict[name]
		if 0 == slotStatType:
			return

		statPoint = player.GetStatus(slotStatType)
		startSlotIndex = slotWindow.GetStartIndex()
		if "HORSE" == name:
			startSlotIndex += self.ACTIVE_PAGE_SLOT_COUNT

		if statPoint > 0:
			for i in xrange(self.PAGE_SLOT_COUNT):
				slotIndex = i + startSlotIndex
				skillIndex = player.GetSkillIndex(slotIndex)
				skillGrade = player.GetSkillGrade(slotIndex)
				skillLevel = player.GetSkillLevel(slotIndex)

				if skillIndex == 0:
					continue
				if skillGrade != 0:
					continue

				if name == "HORSE":
					if player.GetStatus(player.LEVEL) >= skill.GetSkillLevelLimit(skillIndex):
						if skillLevel < 20:
							slotWindow.ShowSlotButton(self.__GetETCSkillRealSlotIndex(slotIndex))

				else:
					if "SUPPORT" == name:
						if not SHOW_LIMIT_SUPPORT_SKILL_LIST or skillIndex in SHOW_LIMIT_SUPPORT_SKILL_LIST:
							if self.CanShowPlusButton(skillIndex, skillLevel, statPoint):
								slotWindow.ShowSlotButton(slotIndex)
					else:
						if self.CanShowPlusButton(skillIndex, skillLevel, statPoint):
							slotWindow.ShowSlotButton(slotIndex)


	def RefreshSkillPlusButtonList(self):

		if self.isLoaded==0:
			return

		self.RefreshSkillPlusPointLabel()

		if not self.__CanUseSkillNow():
			return

		try:
			if self.PAGE_HORSE == self.curSelectedSkillGroup or self.PAGE_BUFFI == self.curSelectedSkillGroup:
				self.__RefreshSkillPlusButton("HORSE")
			else:
				self.__RefreshSkillPlusButton("ACTIVE")

			self.__RefreshSkillPlusButton("SUPPORT")

		except:
			import exception
			exception.Abort("CharacterWindow.RefreshSkillPlusButtonList.BindObject")

	def RefreshSkillPlusPointLabel(self):
		if self.isLoaded==0:
			return

		if self.PAGE_HORSE == self.curSelectedSkillGroup:
			activeStatPoint = player.GetStatus(player.SKILL_HORSE)
			self.activeSkillPointValue.SetText(str(activeStatPoint))
		elif app.__BUFFI_SUPPORT__ and self.PAGE_BUFFI == self.curSelectedSkillGroup:
			self.activeSkillPointValue.SetText("0")
		else:
			activeStatPoint = player.GetStatus(player.SKILL_ACTIVE)
			self.activeSkillPointValue.SetText(str(activeStatPoint))

		supportStatPoint = max(0, player.GetStatus(player.SKILL_SUPPORT))
		self.supportSkillPointValue.SetText(str(supportStatPoint))

	## Skill Level Up Button
	def OnPressedSlotButton(self, slotNumber):
		srcSlotIndex = self.__RealSkillSlotToSourceSlot(slotNumber)

		skillIndex = player.GetSkillIndex(srcSlotIndex)
		curLevel = player.GetSkillLevel(srcSlotIndex)
		maxLevel = skill.GetSkillMaxLevel(skillIndex)

		net.SendChatPacket("/skillup " + str(skillIndex))

	## Use Skill
	def ClickSkillSlot(self, slotIndex):
		if app.__BUFFI_SUPPORT__ and self.PAGE_BUFFI == self.curSelectedSkillGroup:
			return

		srcSlotIndex = self.__RealSkillSlotToSourceSlot(slotIndex)
		skillIndex = player.GetSkillIndex(srcSlotIndex)
		skillType = skill.GetSkillType(skillIndex)

		if not self.__CanUseSkillNow():
			if skill.SKILL_TYPE_ACTIVE == skillType:
				return

		for slotWindow in self.skillPageDict.values():
			if slotWindow.HasSlot(slotIndex):
				if skill.CanUseSkill(skillIndex):
					player.ClickSkillSlot(srcSlotIndex)
					return

		mouseModule.mouseController.DeattachObject()

	def OnUseSkill(self, slotIndex, coolTime):			
		if app.__BUFFI_SUPPORT__ and self.PAGE_BUFFI == self.curSelectedSkillGroup:
			return
		skillIndex = player.GetSkillIndex(slotIndex)
		skillType = skill.GetSkillType(skillIndex)

		fakeSlotIndex = slotIndex
		## ACTIVE
		if skill.SKILL_TYPE_ACTIVE == skillType:
			skillGrade = player.GetSkillGrade(slotIndex)
			slotIndex = self.__GetRealSkillSlot(skillGrade, slotIndex)
		else:
			slotIndex = self.__GetETCSkillRealSlotIndex(slotIndex)

		for slotWindow in self.skillPageDict.values(): 
			if fakeSlotIndex >= 11 and fakeSlotIndex <= 15:
				if self.PAGE_HORSE != self.curSelectedSkillGroup:
					return False

			if slotWindow.HasSlot(slotIndex):
				slotWindow.SetSlotCoolTime(slotIndex, coolTime)
				#slotWindow.StoreSlotCoolTime(skillType, slotIndex, coolTime)
				#self.__RestoreSlotCoolTime(slotWindow)
				return

	def __RestoreSlotCoolTime(self, skillPage):
		restoreType = skill.SKILL_TYPE_NONE
		if self.PAGE_HORSE == self.curSelectedSkillGroup:
			restoreType = skill.SKILL_TYPE_HORSE
		else:
			restoreType = skill.SKILL_TYPE_ACTIVE
		
		skillPage.RestoreSlotCoolTime(restoreType)

	def OnActivateSkill(self, slotIndex):
		if app.__BUFFI_SUPPORT__ and self.PAGE_BUFFI == self.curSelectedSkillGroup:
			return

		skillGrade = player.GetSkillGrade(slotIndex)
		slotIndex = self.__GetRealSkillSlot(skillGrade, slotIndex)
		for slotWindow in self.skillPageDict.values():
			if slotWindow.HasSlot(slotIndex):
				if app.ENABLE_SASH_SYSTEM:
					slotWindow.ActivateSlotOld(slotIndex)
				else:
					slotWindow.ActivateSlot(slotIndex)
				
				return

	def OnDeactivateSkill(self, slotIndex):
		skillGrade = player.GetSkillGrade(slotIndex)
		slotIndex = self.__GetRealSkillSlot(skillGrade, slotIndex)
		for slotWindow in self.skillPageDict.values():
			if slotWindow.HasSlot(slotIndex):
				if app.ENABLE_SASH_SYSTEM:
					slotWindow.DeactivateSlotOld(slotIndex)
				else:
					slotWindow.DeactivateSlot(slotIndex)
				
				return

	def __ShowJobToolTip(self):
		self.toolTipJob.ShowToolTip()

	def __HideJobToolTip(self):
		self.toolTipJob.HideToolTip()

	def __SetJobText(self, mainJob, subJob):
		if player.GetStatus(player.LEVEL)<5:
			subJob=0

	def __ShowAlignmentToolTip(self):
		self.toolTipAlignment.ShowToolTip()

	def __HideAlignmentToolTip(self):
		self.toolTipAlignment.HideToolTip()

	def RefreshCharacter(self):
		if self.isLoaded==0:
			return

		## Name
		try:
			characterName = player.GetName()
			guildName = player.GetGuildName()
			self.characterNameValue.SetText(characterName)
			self.guildNameValue.SetText(guildName)
			if not guildName:
				if localeInfo.IsARABIC():
					self.characterNameSlot.SetPosition(190, 34)
				else:
					self.characterNameSlot.SetPosition(109, 34)

				self.guildNameSlot.Hide()
			else:
				if localeInfo.IsJAPAN():
					self.characterNameSlot.SetPosition(143, 34)
				else:
					self.characterNameSlot.SetPosition(153, 34)
				self.guildNameSlot.Show()
		except:
			import exception
			exception.Abort("CharacterWindow.RefreshCharacter.BindObject")

		race = net.GetMainActorRace()
		group = net.GetMainActorSkillGroup()
		empire = net.GetMainActorEmpire()

		## Job Text
		job = chr.RaceToJob(race)
		self.__SetJobText(job, group)

		## FaceImage
		try:
			faceImageName = FACE_IMAGE_DICT[race]

			try:
				self.faceImage.LoadImage(faceImageName)
			except:
				print "CharacterWindow.RefreshCharacter(race=%d, faceImageName=%s)" % (race, faceImageName)
				self.faceImage.Hide()

		except KeyError:
			self.faceImage.Hide()

		## GroupName
		self.__SetSkillGroupName(race, group)

		## Skill
		if 0 == group:
			self.__SelectSkillGroup(0)
		else:
			self.__SetSkillSlotData(race, group, empire)

			if app.__BUFFI_SUPPORT__:
				self.__SelectSkillGroup(0)
			elif self.__CanUseHorseSkill():
				self.__SelectSkillGroup(0)

	def OverInLeaderBonus(self, index):
		k = constInfo.LEADERSHIP_POWER
		Bonus = 0
		
		if index == PARTY_AFFECT_ATTACKER:
			Bonus = int(10 + 60 * k)
		elif index == PARTY_AFFECT_TANKER:
			Bonus = int(50 + 1450 * k)
		elif index == PARTY_AFFECT_BUFFER:
			Bonus = int(5 + 45 * k)
		elif index == PARTY_AFFECT_SKILL_MASTER:
			Bonus = int(25 + 600 * k)
		elif index == PARTY_AFFECT_BERSERKER:
			Bonus = int(1 + 5 * k)
		elif index == PARTY_AFFECT_DEFENDER:
			Bonus = int(5 + 30 * k)
		
		AFFECT_STRING_DICT = {
			PARTY_AFFECT_ATTACKER : localeInfo.PARTY_BONUS_ATTACKER,
			PARTY_AFFECT_TANKER : localeInfo.PARTY_BONUS_TANKER,
			PARTY_AFFECT_BUFFER : localeInfo.PARTY_BONUS_BUFFER,
			PARTY_AFFECT_SKILL_MASTER : localeInfo.PARTY_BONUS_SKILL_MASTER,
			PARTY_AFFECT_BERSERKER : localeInfo.PARTY_BONUS_BERSERKER,
			PARTY_AFFECT_DEFENDER : localeInfo.PARTY_BONUS_DEFENDER,
		}
		
		AFFECT_TITLE_DICT = {
			PARTY_AFFECT_ATTACKER : "Attacker",
			PARTY_AFFECT_TANKER : "Tanker",
			PARTY_AFFECT_BUFFER : "Buffer",
			PARTY_AFFECT_SKILL_MASTER : "Skill Master",
			PARTY_AFFECT_BERSERKER : "Berserker",
			PARTY_AFFECT_DEFENDER : "Defender",
		}
		
		if not AFFECT_STRING_DICT.has_key(index):
			return
		
		self.toolTip.ClearToolTip()
		self.toolTip.SetTitle(AFFECT_TITLE_DICT[index])
		self.toolTip.AppendTextLine("Grants: " + AFFECT_STRING_DICT[index](Bonus))
		self.toolTip.Show()
		
	def OverOutLeaderBonus(self, index):
		self.toolTip.Hide()
	
	def SelectLeaderBonus(self, idx):
		net.SendChatPacket("/leadership_bonus %d" % (idx))
		self.boardLeader.Hide()

	def __SetSkillGroupName(self, race, group):

		job = chr.RaceToJob(race)

		if not self.SKILL_GROUP_NAME_DICT.has_key(job):
			return

		nameList = self.SKILL_GROUP_NAME_DICT[job]

		if 0 == group:
			self.skillGroupButton1.SetText(nameList[1])
			self.skillGroupButton2.SetText(nameList[2])
			self.skillGroupButton1.Show()
			self.skillGroupButton2.Show()
			self.activeSkillGroupName.Hide()

			if app.__BUFFI_SUPPORT__:
				self.skillGroupButton3.Hide()
		else:
			if self.__CanUseHorseSkill():
				self.activeSkillGroupName.Hide()
				self.skillGroupButton1.SetText(nameList.get(group, "Noname"))
				self.skillGroupButton2.SetText(localeInfo.SKILL_GROUP_HORSE)
				self.skillGroupButton1.Show()

				self.skillGroupButton2.Show()
				if app.__BUFFI_SUPPORT__:
					self.skillGroupButton2.SetPosition(50, 2)

				if app.__BUFFI_SUPPORT__:
					self.skillGroupButton3.SetPosition(95, 2)
					self.skillGroupButton3.Show()
			else:
				self.activeSkillGroupName.SetText(nameList.get(group, "Noname"))

				if app.__BUFFI_SUPPORT__:
					self.activeSkillGroupName.Hide()
				else:
					self.activeSkillGroupName.Show()

				if app.__BUFFI_SUPPORT__:
					self.skillGroupButton1.SetText(nameList.get(group, "Noname"))
					self.skillGroupButton1.Show()
				else:
					self.skillGroupButton1.Hide()

				self.skillGroupButton2.Hide()

				if app.__BUFFI_SUPPORT__:
					self.skillGroupButton3.SetPosition(50, 2)
					self.skillGroupButton3.Show()

	def __SetSkillSlotData(self, race, group, empire=0):
		playerSettingModule.RegisterSkill(race, group, empire)

		## Event
		self.__SetSkillSlotEvent()

		## Refresh
		self.RefreshSkill()

	def __SelectSkillGroup(self, index):
		group = net.GetMainActorSkillGroup()

		for btn in self.skillGroupButton:
			btn.SetUp()
		self.skillGroupButton[index].Down()

		if group == 0:
			if 0 == index:
				index = net.GetMainActorSkillGroup()
			elif 1 == index:
				index = net.GetMainActorSkillGroup()+1
		
		if group != 0:
			if 0 == index:
				if group == 1:
					index = net.GetMainActorSkillGroup()-1
				elif group == 2:
					index = net.GetMainActorSkillGroup()-1
			elif 1 == index:
				index = self.PAGE_HORSE
			elif 2 == index:
				index = self.PAGE_BUFFI

		self.curSelectedSkillGroup = index
		
		if self.PAGE_BUFFI == self.curSelectedSkillGroup:
			self.fastReadButton.Hide()
			self.GetChild("FastReaderTitle").Hide()
		else:
			self.fastReadButton.Show()
			self.GetChild("FastReaderTitle").Show()

		self.__SetSkillSlotData(net.GetMainActorRace(), index+1, net.GetMainActorEmpire())

	def __CanUseSkillNow(self):
		if 0 == net.GetMainActorSkillGroup():
			return False

		return True

	def __CanUseHorseSkill(self):

		slotIndex = player.GetSkillSlotIndex(player.SKILL_INDEX_RIDING)

		if not slotIndex:
			return False

		grade = player.GetSkillGrade(slotIndex)
		level = player.GetSkillLevel(slotIndex)
		if level < 0:
			level *= -1
		if grade >= 1 and level >= 1:
			return True

		return False

	def __IsChangedHorseRidingSkillLevel(self):
		ret = False

		if -1 == self.canUseHorseSkill:
			self.canUseHorseSkill = self.__CanUseHorseSkill()

		if self.canUseHorseSkill != self.__CanUseHorseSkill():
			ret = True

		self.canUseHorseSkill = self.__CanUseHorseSkill()
		return ret

	def __GetRealSkillSlot(self, skillGrade, skillSlot):
		return skillSlot + min(skill.SKILL_GRADE_COUNT-1, skillGrade)*skill.SKILL_GRADE_STEP_COUNT

	def __GetETCSkillRealSlotIndex(self, skillSlot):
		if skillSlot > 100:
			return skillSlot
		return skillSlot % self.ACTIVE_PAGE_SLOT_COUNT

	def __RealSkillSlotToSourceSlot(self, realSkillSlot):
		if realSkillSlot > 100:
			return realSkillSlot
		if self.PAGE_HORSE == self.curSelectedSkillGroup:
			return realSkillSlot + self.ACTIVE_PAGE_SLOT_COUNT
		# if app.__BUFFI_SUPPORT__ and self.PAGE_BUFFI == self.curSelectedSkillGroup:
			# return (realSkillSlot + self.ACTIVE_PAGE_SLOT_COUNT + self.ACTIVE_PAGE_SLOT_COUNT - 1) % skill.SKILL_GRADE_STEP_COUNT
		elif self.PAGE_BUFFI == self.curSelectedSkillGroup:
			if realSkillSlot == 1 or realSkillSlot == 21 or realSkillSlot == 41:
				return 17
			elif realSkillSlot == 2 or realSkillSlot == 22 or realSkillSlot == 42:
				return 18
			elif realSkillSlot == 3 or realSkillSlot == 23 or realSkillSlot == 43:
				return 19
			elif realSkillSlot == 4 or realSkillSlot == 24 or realSkillSlot == 44:
				return 20
		return realSkillSlot % skill.SKILL_GRADE_STEP_COUNT

	def __GetSkillGradeFromSlot(self, skillSlot):
		return int(skillSlot / skill.SKILL_GRADE_STEP_COUNT)

	def SelectSkillGroup(self, index):
		self.__SelectSkillGroup(index)

	if not app.ENABLE_QUEST_RENEWAL:
		def OnQuestScroll(self):
			questCount = quest.GetQuestCount()
			scrollLineCount = max(0, questCount - quest.QUEST_MAX_NUM)
			startIndex = int(scrollLineCount * self.questScrollBar.GetPos())

			if startIndex != self.questShowingStartIndex:
				self.questShowingStartIndex = startIndex
				self.RefreshQuest()
