import ui, chat, item, nonplayer, battlepass, localeInfo, player
from _weakref import proxy

IMAGE_PATH = "battlepass/"

PAGE_REWARDS, PAGE_MISSIONS = xrange(2)
REWARD_PER_PAGE = 4


def GetMissionText(mission_type, value, specific_target=0, min_level = 1, max_level = 120):
	value = localeInfo.NumberWithPoint(value)
	text = ""


	if (mission_type == battlepass.MISSION_NONE):
		text =  localeInfo.BATTLEPASS_INVALID_MISSION_TYPE
		
	elif (mission_type == battlepass.MISSION_REACH_LEVEL):
		text = localeInfo.BATTLEPASS_MISSION_REACH_LEVEL.format(value)

	elif (mission_type == battlepass.MISSION_COLLECT_GOLD):
		text = localeInfo.BATTLEPASS_COLLECT_GOLD.format(value)

	elif (mission_type == battlepass.MISSION_COLLECT_EXP):
		text = localeInfo.BATTLEPASS_COLLECT_EXP.format(value)

	elif (mission_type == battlepass.MISSION_KILL_PLAYER):
		text = localeInfo.BATTLEPASS_KILL_ANY_PLAYERS.format(value)

	elif (mission_type == battlepass.MISSION_KILL_MOB):
		if (specific_target):
			text = localeInfo.BATTLEPASS_KILL_X_FOR_X_TIMES.format(nonplayer.GetMonsterName(specific_target), value)
		else:
			text = localeInfo.BATTLEPASS_KILL_ANY_MONSTER_FOR_TIMES.format(value)

	elif (mission_type == battlepass.MISSION_CATCH_FISH):
		item.SelectItem(specific_target)
		fish_name = item.GetItemName()

		if (specific_target):
			text = localeInfo.BATTLEPASS_CATCH_FISH.format(fish_name, value)
		else:
			text = localeInfo.BATTLEPASS_CATCH_ANY_FISH.format(value)

	elif (mission_type == battlepass.MISSION_MINE):
		text = localeInfo.BATTLEPASS_MINE.format(value)

	elif (mission_type == battlepass.MISSION_SEND_SHOUT_MESSAGE):
		text = localeInfo.BATTLEPASS_SEND_SHOUT.format(value)

	elif (mission_type == battlepass.MISSION_UPGRADE_OBJECT):
		item.SelectItem(specific_target)
		item_name = item.GetItemName()

		if (specific_target):
			text = localeInfo.BATTLEPASS_UPGRADE_ITEM.format(item_name, value)
		else:
			text = localeInfo.BATTLEPASS_UPGRADE_ANY_ITEM.format(value)

	elif (mission_type == battlepass.MISSION_USE_ITEM):
		item.SelectItem(specific_target)
		item_name = item.GetItemName()

		if (specific_target):
			text = localeInfo.BATTLEPASS_USE_ITEM.format(item_name, value)
		else:
			text = localeInfo.BATTLEPASS_USE_ANY_ITEM.format(value)

	if min_level != 1 or max_level != 120:
		text += " Lv ({} - {})".format(min_level, max_level)
		
	return text

class BattlePassWindow(ui.BoardWithTitleBar):

	def __init__(self, tooltip_instance):
		battlepass.RegisterClass(self)

		super(BattlePassWindow, self).__init__()
		self.SetWindowProperties()
		self.SetBackgroundImage()
		self.SetNavigationObjects()

		self.SetDefaultVariables()
		self.SetPageContents()
		self.SetTooltipInstance(tooltip_instance)
		self.UpdateBattlePassRemainingTime()

		# First we are opening missions page to load missions
		self.ChangePage(PAGE_MISSIONS)

		# now we can go page rewards
		self.ChangePage(PAGE_REWARDS)

	def __del__(self):
		super(BattlePassWindow, self).__del__()
		battlepass.UnregisterClass()

	def Destroy(self):
		if self.mission_listbox:
			self.mission_listbox.Destroy()
			self.mission_listbox.Clear()
			del self.mission_listbox

	def Open(self):
		if not self.data_requested:
			battlepass.RequestCurrentProgress()
			self.data_requested = True

		super(BattlePassWindow, self).Show()

	def Close(self):
		super(BattlePassWindow, self).Hide()

	def SetWindowProperties(self):
		self.SetSize(510, 417)
		self.SetCenterPosition()
		self.SetTitleName("BattlePass")
		self.AddFlag("movable")

	def SetBackgroundImage(self):
		self.main_background_image = ui.ImageBox()
		self.main_background_image.SetParent(self)
		self.main_background_image.SetPosition(6, 32)
		self.main_background_image.LoadImage(IMAGE_PATH + "background_image.png")
		self.main_background_image.Show()

	def SetNavigationObjects(self):
	
		self.time_title_image = ui.ImageBox()
		self.time_title_image.SetParent(self.main_background_image)
		self.time_title_image.SetPosition(14, 15)
		self.time_title_image.LoadImage(IMAGE_PATH + "time_info_title.png")
		self.time_title_image.Show()

		self.time_title_clock_image = ui.ImageBox()
		self.time_title_clock_image.SetParent(self.time_title_image)
		self.time_title_clock_image.SetWindowVerticalAlignCenter()
		self.time_title_clock_image.SetWindowHorizontalAlignCenter()
		self.time_title_clock_image.SetPosition(-125, 0)
		self.time_title_clock_image.LoadImage(IMAGE_PATH + "clock_image.png")
		self.time_title_clock_image.Show()

		self.time_remaining = ui.TextLine()
		self.time_remaining.AddFlag("attach")
		self.time_remaining.AddFlag("not_pick")
		self.time_remaining.SetParent(self.time_title_image)
		self.time_remaining.SetText('Battlepass will end in 28 days 13 hours 12 minutes!')
		self.time_remaining.SetVerticalAlignCenter()
		self.time_remaining.SetHorizontalAlignCenter()
		self.time_remaining.SetWindowVerticalAlignCenter()
		self.time_remaining.SetWindowHorizontalAlignCenter()
		self.time_remaining.SetPosition(0, 0)
		self.time_remaining.Show()

		self.user_premium_status = ui.TextLine()
		self.user_premium_status.AddFlag("attach")
		self.user_premium_status.AddFlag("not_pick")
		self.user_premium_status.SetParent(self.time_title_image)
		self.user_premium_status.SetText("|cff808080[Free]|r")
		self.user_premium_status.SetVerticalAlignCenter()
		self.user_premium_status.SetHorizontalAlignRight()
		self.user_premium_status.SetWindowVerticalAlignCenter()
		self.user_premium_status.SetWindowHorizontalAlignRight()
		self.user_premium_status.SetPosition(25, 0)
		self.user_premium_status.Show()

		self.my_tier_level_backgound = ui.ImageBox()
		self.my_tier_level_backgound.SetParent(self.main_background_image)
		self.my_tier_level_backgound.SetPosition(52, 43)
		self.my_tier_level_backgound.LoadImage(IMAGE_PATH + "info_object.png")
		self.my_tier_level_backgound.Show()

		self.my_tier_level_text = ui.TextLine()
		self.my_tier_level_text.SetParent(self.my_tier_level_backgound)
		self.my_tier_level_text.SetPosition(0, 0)
		self.my_tier_level_text.SetVerticalAlignCenter()
		self.my_tier_level_text.SetHorizontalAlignCenter()
		self.my_tier_level_text.SetWindowVerticalAlignCenter()
		self.my_tier_level_text.SetWindowHorizontalAlignCenter()
		self.my_tier_level_text.SetOutline()
		self.my_tier_level_text.SetText("Tier Level: 5")
		self.my_tier_level_text.Show()

		self.my_tier_exp_backgound = ui.ImageBox()
		self.my_tier_exp_backgound.SetParent(self.main_background_image)
		self.my_tier_exp_backgound.SetPosition(self.my_tier_level_backgound.GetLocalPosition()[0] + self.my_tier_level_backgound.GetWidth() + 5, self.my_tier_level_backgound.GetLocalPosition()[1])
		self.my_tier_exp_backgound.LoadImage(IMAGE_PATH + "info_object.png")
		self.my_tier_exp_backgound.Show()

		self.my_tier_exp_text = ui.TextLine()
		self.my_tier_exp_text.SetParent(self.my_tier_exp_backgound)
		self.my_tier_exp_text.SetPosition(0, 0)
		self.my_tier_exp_text.SetVerticalAlignCenter()
		self.my_tier_exp_text.SetHorizontalAlignCenter()
		self.my_tier_exp_text.SetWindowVerticalAlignCenter()
		self.my_tier_exp_text.SetWindowHorizontalAlignCenter()
		self.my_tier_exp_text.SetOutline()
		self.my_tier_exp_text.SetText("Experience: 150 / 250")
		self.my_tier_exp_text.Show()

		self.show_missions_page_button = ui.Button()
		self.show_missions_page_button.SetParent(self.main_background_image)
		self.show_missions_page_button.SetUpVisual(IMAGE_PATH + "buttons/button_01.png")
		self.show_missions_page_button.SetOverVisual(IMAGE_PATH + "buttons/button_02.png")
		self.show_missions_page_button.SetDownVisual(IMAGE_PATH + "buttons/button_01.png")
		self.show_missions_page_button.SetText(localeInfo.BATTLEPASS_MISSIONS_BUTTON_TEXT)
		self.show_missions_page_button.SetPosition(self.my_tier_exp_backgound.GetLocalPosition()[0] + self.my_tier_exp_backgound.GetWidth() + 14, self.my_tier_exp_backgound.GetLocalPosition()[1] + 3)
		self.show_missions_page_button.SetEvent(ui.__mem_func__(self.ChangePage), PAGE_MISSIONS)
		self.show_missions_page_button.Show()

		self.show_rewards_page_button = ui.Button()
		self.show_rewards_page_button.SetParent(self.main_background_image)
		self.show_rewards_page_button.SetUpVisual(IMAGE_PATH + "buttons/button_01.png")
		self.show_rewards_page_button.SetOverVisual(IMAGE_PATH + "buttons/button_02.png")
		self.show_rewards_page_button.SetDownVisual(IMAGE_PATH + "buttons/button_01.png")
		self.show_rewards_page_button.SetText(localeInfo.BATTLEPASS_REWARDS_BUTTON_TEXT)
		self.show_rewards_page_button.SetPosition(self.my_tier_exp_backgound.GetLocalPosition()[0] + self.my_tier_exp_backgound.GetWidth() + 14, self.my_tier_exp_backgound.GetLocalPosition()[1] + 3)
		self.show_rewards_page_button.SetEvent(ui.__mem_func__(self.ChangePage), PAGE_REWARDS)
		self.show_rewards_page_button.Hide()

	def SetPageContents(self):
		self.page_general_rewards = ui.Window()
		self.page_general_rewards.SetParent(self)
		self.page_general_rewards.SetPosition(16, 107)
		self.page_general_rewards.SetSize(477, 290)
		self.page_general_rewards.Show()

		self.free_pass_image = ui.ImageBox()
		self.free_pass_image.SetParent(self.page_general_rewards)
		self.free_pass_image.SetPosition(14, 30)
		self.free_pass_image.LoadImage(IMAGE_PATH + "free_battlepass_title.png")
		self.free_pass_image.Show()

		self.premium_pass_image = ui.ImageBox()
		self.premium_pass_image.SetParent(self.page_general_rewards)
		self.premium_pass_image.SetPosition(14, self.free_pass_image.GetLocalPosition()[1] + self.free_pass_image.GetHeight() + 11)
		self.premium_pass_image.LoadImage(IMAGE_PATH + "premium_battlepass_title.png")
		self.premium_pass_image.Show()

		self.rewards_container = ui.Window()
		self.rewards_container.SetParent(self.page_general_rewards)
		self.rewards_container.SetPosition(51, 0)
		self.rewards_container.SetSize(425, self.page_general_rewards.GetHeight())
		self.rewards_container.Show()

		self.current_page_text = ui.TextLine()
		self.current_page_text.AddFlag("attach")
		self.current_page_text.AddFlag("not_pick")
		self.current_page_text.SetParent(self.page_general_rewards)
		self.current_page_text.SetFontName("Verdana:14")
		self.current_page_text.SetText(localeInfo.BATTLEPASS_PAGIONATION_DEFAULT)
		self.current_page_text.SetVerticalAlignBottom()
		self.current_page_text.SetHorizontalAlignCenter()
		self.current_page_text.SetWindowVerticalAlignBottom()
		self.current_page_text.SetWindowHorizontalAlignCenter()
		self.current_page_text.SetPosition(0, 10)
		self.current_page_text.Show()

		self.first_page_button = ui.Button()
		self.first_page_button.SetParent(self.rewards_container)
		self.first_page_button.SetPosition(-105,20)
		self.first_page_button.SetUpVisual(IMAGE_PATH + "buttons/arrow_first_page_0.tga")
		self.first_page_button.SetOverVisual(IMAGE_PATH + "buttons/arrow_first_page_1.tga")
		self.first_page_button.SetDownVisual(IMAGE_PATH + "buttons/arrow_first_page_2.tga")
		self.first_page_button.SetEvent(ui.__mem_func__(self.OnFirstRewardsPage))
		self.first_page_button.SetWindowVerticalAlignBottom()
		self.first_page_button.SetWindowHorizontalAlignCenter()
		self.first_page_button.Show()

		self.previous_page_button = ui.Button()
		self.previous_page_button.SetParent(self.rewards_container)
		self.previous_page_button.SetUpVisual(IMAGE_PATH + "buttons/arrow_prev_0.tga")
		self.previous_page_button.SetOverVisual(IMAGE_PATH + "buttons/arrow_prev_1.tga")
		self.previous_page_button.SetDownVisual(IMAGE_PATH + "buttons/arrow_prev_2.tga")
		self.previous_page_button.SetPosition(-85,20)
		self.previous_page_button.SetWindowVerticalAlignBottom()
		self.previous_page_button.SetWindowHorizontalAlignCenter()
		self.previous_page_button.SetEvent(ui.__mem_func__(self.OnPreviousRewardsPage))
		self.previous_page_button.Show()

		self.next_page_button = ui.Button()
		self.next_page_button.SetParent(self.rewards_container)
		self.next_page_button.SetUpVisual(IMAGE_PATH + "buttons/arrow_next_0.tga")
		self.next_page_button.SetOverVisual(IMAGE_PATH + "buttons/arrow_next_1.tga")
		self.next_page_button.SetDownVisual(IMAGE_PATH + "buttons/arrow_next_2.tga")
		self.next_page_button.SetPosition(40, 20)
		self.next_page_button.SetWindowVerticalAlignBottom()
		self.next_page_button.SetWindowHorizontalAlignCenter()
		self.next_page_button.SetEvent(ui.__mem_func__(self.OnNextRewardsPage))
		self.next_page_button.Show()

		self.last_page_button = ui.Button()
		self.last_page_button.SetParent(self.rewards_container)
		self.last_page_button.SetPosition(60, 20)
		self.last_page_button.SetUpVisual(IMAGE_PATH + "buttons/arrow_last_page_0.tga")
		self.last_page_button.SetOverVisual(IMAGE_PATH + "buttons/arrow_last_page_1.tga")
		self.last_page_button.SetDownVisual(IMAGE_PATH + "buttons/arrow_last_page_2.tga")
		self.last_page_button.SetWindowVerticalAlignBottom()
		self.last_page_button.SetWindowHorizontalAlignCenter()
		self.last_page_button.SetEvent(ui.__mem_func__(self.OnLastRewardsPage))
		self.last_page_button.Show()
		## Page Mission

		self.page_general_missions = ui.Window()
		self.page_general_missions.SetParent(self)
		self.page_general_missions.SetPosition(16, 107)
		self.page_general_missions.SetSize(477, 295)
		self.page_general_missions.Show()

		self.page_missions_title = ui.ImageBox()
		self.page_missions_title.SetParent(self.page_general_missions)
		self.page_missions_title.SetPosition(5,8)
		self.page_missions_title.LoadImage(IMAGE_PATH + "time_info_title.png")
		self.page_missions_title.Show()

		self.mission_title = ui.TextLine()
		self.mission_title.AddFlag("attach")
		self.mission_title.AddFlag("not_pick")
		self.mission_title.SetParent(self.page_missions_title)
		self.mission_title.SetText(localeInfo.BATTLEPASS_MISSION_TITLE_TEXT)
		self.mission_title.SetVerticalAlignCenter()
		self.mission_title.SetHorizontalAlignLeft()
		self.mission_title.SetWindowVerticalAlignCenter()
		self.mission_title.SetWindowHorizontalAlignLeft()
		self.mission_title.SetPosition(40, 0)
		self.mission_title.SetPackedFontColor(0xFFd9c997)
		self.mission_title.SetOutline()
		self.mission_title.Show()

		self.mission_progress = ui.TextLine()
		self.mission_progress.AddFlag("attach")
		self.mission_progress.AddFlag("not_pick")
		self.mission_progress.SetParent(self.page_missions_title)
		self.mission_progress.SetText(localeInfo.BATTLEPASS_MISSION_PROGRESS_TEXT)
		self.mission_progress.SetVerticalAlignCenter()
		self.mission_progress.SetHorizontalAlignCenter()
		self.mission_progress.SetWindowVerticalAlignCenter()
		self.mission_progress.SetWindowHorizontalAlignCenter()
		self.mission_progress.SetPosition(0, 0)
		self.mission_progress.SetPackedFontColor(0xFFd9c997)
		self.mission_progress.SetOutline()
		self.mission_progress.Show()

		self.mission_reward = ui.TextLine()
		self.mission_reward.AddFlag("attach")
		self.mission_reward.AddFlag("not_pick")
		self.mission_reward.SetParent(self.page_missions_title)
		self.mission_reward.SetText(localeInfo.BATTLEPASS_MISSION_REWARD_TEXT)
		self.mission_reward.SetVerticalAlignCenter()
		self.mission_reward.SetHorizontalAlignRight()
		self.mission_reward.SetWindowVerticalAlignCenter()
		self.mission_reward.SetWindowHorizontalAlignRight()
		self.mission_reward.SetPosition(40, 0)
		self.mission_reward.SetPackedFontColor(0xFFd9c997)
		self.mission_reward.SetOutline()
		self.mission_reward.Show()

		self.mission_container = ui.Window()
		self.mission_container.SetParent(self.page_general_missions)
		self.mission_container.SetPosition(0, 35)
		self.mission_container.SetSize(477, 255)
		self.mission_container.Show()

		self.mission_listbox = ItemListBox(self)
		self.mission_listbox.SetParent(self.page_general_missions)
		self.mission_listbox.SetSize(477, 255)
		self.mission_listbox.SetPosition(5, 35)
		self.mission_listbox.Show()

		self.mission_listbox_scrollbar = ui.SlimScrollBar()
		self.mission_listbox_scrollbar.SetParent(self.page_general_missions)
		self.mission_listbox_scrollbar.SetScrollBarSize(255)
		self.mission_listbox_scrollbar.SetPosition(468, 35)
		self.mission_listbox.SetScrollBar(self.mission_listbox_scrollbar)
		self.mission_listbox_scrollbar.Show()

		self.battle_pass_deactive_text = ui.TextLine()
		self.battle_pass_deactive_text.AddFlag("attach")
		self.battle_pass_deactive_text.AddFlag("not_pick")
		self.battle_pass_deactive_text.SetParent(self)
		self.battle_pass_deactive_text.SetFontName("Verdana:14")
		self.battle_pass_deactive_text.SetText(localeInfo.BATTLEPASS_NOT_ACTIVE_NOW_TEXT)
		self.battle_pass_deactive_text.SetVerticalAlignCenter()
		self.battle_pass_deactive_text.SetHorizontalAlignCenter()
		self.battle_pass_deactive_text.SetWindowVerticalAlignCenter()
		self.battle_pass_deactive_text.SetWindowHorizontalAlignCenter()
		self.battle_pass_deactive_text.SetPosition(0, 40)
		self.battle_pass_deactive_text.SetPackedFontColor(0xFFd9c997)
		self.battle_pass_deactive_text.SetOutline()
		self.battle_pass_deactive_text.Show()


	def SetRewardsPage(self, page):
		self.current_rewards_page = page
		self.current_page_text.SetText(localeInfo.BATTLEPASS_PAGIONATION_PAGE.format(self.current_rewards_page))
		self.ShowRewardsObjectsOnPage()

	def SetDefaultVariables(self):
		self.current_rewards_page = 1
		self.total_rewards_page_count = 0
		self.tooltip_item = None
		self.is_battle_pass_active = False
		self.premium_status = False
		self.my_tier_level = 0
		self.my_experience = 0
		self.reward_objects = []
		self.data_requested = False

	def SetTooltipInstance(self, instance):
		self.tooltip_item = instance

	def OnCloseWindow(self):
		self.HideItemTooltip()

	def HideItemTooltip(self):
		if self.tooltip_item:
			self.tooltip_item.ClearToolTip()
			self.tooltip_item.Hide()

	def ChangePage(self, page):
		if page == PAGE_MISSIONS:
			self.show_missions_page_button.Hide()
			self.show_rewards_page_button.Show()
			self.page_general_rewards.Hide()
			self.page_general_missions.Show()
			self.LoadMissionObjects()
		else:

			self.show_missions_page_button.Show()
			self.show_rewards_page_button.Hide()
			self.page_general_missions.Hide()
			self.page_general_rewards.Show()
			self.LoadRewardObjects()

			self.SetPremiumStatus(self.premium_status)
			self.SetTierButtonsStatus()

	def OnPressEscapeKey(self):
		self.OnCloseWindow()
		self.Close()
		return True

	def OnFirstRewardsPage(self):
		if self.current_rewards_page == 1:
			return
		self.SetRewardsPage(1)
	
	def OnNextRewardsPage(self):
		if self.current_rewards_page < self.total_rewards_page_count:
			self.SetRewardsPage(self.current_rewards_page + 1)

	def OnPreviousRewardsPage(self):
		if self.current_rewards_page > 1:
			self.SetRewardsPage(self.current_rewards_page - 1)

	def OnLastRewardsPage(self):
		if self.current_rewards_page == self.total_rewards_page_count:
			return
		self.SetRewardsPage(self.total_rewards_page_count)
  
	def LoadRewardObjects(self):
		if not self.is_battle_pass_active:
			return

		for object in self.reward_objects:
			object.Hide()

		if not len(self.reward_objects):
			self.item_count = battlepass.GetTierSize()
			self.total_rewards_page_count = (self.item_count + REWARD_PER_PAGE - 1) // REWARD_PER_PAGE

			x_positions = (15, 117, 217, 320)

			for index in xrange(self.item_count):
				(reward_vnum, reward_count, reward_premium_vnum, reward_premium_count) = battlepass.GetTierObject(index)
				obj = RewardObject(index + 1, {
				"free_item_vnum" : reward_vnum,
				"free_item_count" : reward_count,
				"premium_item_vnum" : reward_premium_vnum,       
				"premium_item_count" : reward_premium_count,       
			},
				self       
			)
				obj.SetParent(self.rewards_container)
				obj.SetPosition(x_positions[index % 4], 10)
				obj.Show()
				self.reward_objects.append(obj)

		self.ShowRewardsObjectsOnPage()

	def UnlockPremiumSlots(self):
		for object in self.reward_objects:
			object.UnlockPremiumSlot()

		self.user_premium_status.SetText("|cffFFD700[{}]|r".format(localeInfo.BATTLEPASS_PREMIUM_TEXT))

	def ShowRewardsObjectsOnPage(self):
		for obj in self.reward_objects:
			obj.Hide()

		total_pages = (len(self.reward_objects) + REWARD_PER_PAGE - 1) // REWARD_PER_PAGE

		if self.current_rewards_page > total_pages or self.current_rewards_page < 1:
			self.SetRewardsPage(1)
			self.current_rewards_page = 1

		start_index = (self.current_rewards_page - 1) * REWARD_PER_PAGE
		end_index = min(start_index + REWARD_PER_PAGE, len(self.reward_objects))

		for item in self.reward_objects[start_index:end_index]:
			item.Show()

	def SetBattlePassActive(self):
		self.is_battle_pass_active = True
		self.battle_pass_deactive_text.Hide()

	def SetTierLevel(self, new_tier_level):
		self.my_tier_level = new_tier_level
		self.my_tier_level_text.SetText(localeInfo.BATTLEPASS_TIER_LEVEL.format(new_tier_level))

	def SetPremiumStatus(self, is_premium):
		self.premium_status = is_premium

		if (is_premium):
			self.UnlockPremiumSlots()

	def SetTierButtonsStatus(self):
		if not len(self.reward_objects):
			return
		
		for index, item in enumerate(self.reward_objects):
			objects_tier_level = index + 1
			
			if (objects_tier_level <= self.my_tier_level):
				item.ShowRedeemButtons(self.premium_status)


	def SetExperience(self, new_experience):
		self.my_experience = new_experience

		next_level_experience = battlepass.GetNextLevelEXP(self.my_tier_level)

		self.my_tier_exp_text.SetText(localeInfo.BATTLEPASS_EXPERIENCE.format(new_experience, next_level_experience))


	def LoadMissionObjects(self):
		if not self.is_battle_pass_active:
			return

		if self.mission_listbox.GetItemCount():
			return

		self.mission_item_count = battlepass.GetMissionSize()


		for i in xrange(self.mission_item_count):
			(mission_type, target_object, target_count, exp_reward, min_level, max_level) = battlepass.GetMissionObject(i)

			self.mission_listbox.AppendItem({
				"mission_index": i,
				"title": GetMissionText(mission_type, target_count, target_object, min_level, max_level),
				"progress" : "0 / 0",
				"reward" : localeInfo.NumberWithPoint(exp_reward),
				})
			
		# items = self.mission_listbox.GetItems()

		# for index, item in enumerate(items):
		#     if player.GetStatus(player.LEVEL) > max_level:
		#         item.Hide()

	def UpdateBattlePassRemainingTime(self):

		(start_time, current_time, end_time) = battlepass.GetRemainingTime()

		start_now_diff = start_time - current_time
		end_now_diff = end_time - current_time
		if start_now_diff > 0:
			# Battlepass has not started yet
			self.time_remaining.SetText(localeInfo.BATTLEPASS_START_TIME.format(localeInfo.SecondToDHM(start_now_diff)))
		elif start_now_diff <= 0 and end_now_diff > 0:
			# Battlepass is currently active
			self.time_remaining.SetText(localeInfo.BATTLEPASS_END_TIME.format(localeInfo.SecondToDHM(end_now_diff)))
			self.SetBattlePassActive()
		else:
			# Battlepass has ended
			days_ago = abs(start_now_diff) // 86400  # Number of days since end
			self.time_remaining.SetText(localeInfo.BATTLEPASS_ACTIVE_TIME.format(days_ago))

	def OnUpdate(self):
		self.UpdateBattlePassRemainingTime()

	def UpdateCharacterData(self, current_level, current_exp, premium_status):
		self.SetTierLevel(current_level)
		self.SetExperience(current_exp)
		self.SetPremiumStatus(premium_status)
		self.SetTierButtonsStatus()



	def UpdateMissionState(self, mission_index, progress, completed, collected):
		if not self.mission_listbox.GetItemCount():
			return
		
		items = self.mission_listbox.GetItems()

		for index, item in enumerate(items):
			if index == mission_index:
				item.UpdateProgress(progress, completed, collected)

	def UpdateRewardState(self, mission_index, reward_collected, reward_premium_collected):
		if not len(self.reward_objects):
			return

		for index, item in enumerate(self.reward_objects):
			if index == mission_index:
				item.SetRewardRedeemed(reward_collected, reward_premium_collected)

	def OnRunMouseWheel(self, length):
		if self.page_general_rewards.IsShow() and self.page_general_rewards.IsInPosition():
			if length > 0:
				self.OnNextRewardsPage()
			else:
				self.OnPreviousRewardsPage()
			return True

		if self.page_general_missions.IsInPosition():
			if length > 0:
				self.mission_listbox_scrollbar.OnUp()
			else:
				self.mission_listbox_scrollbar.OnDown()
			return True
		return False

class RewardObject(ui.Window):

	def __init__(self, index, reward_info, main_class):
		super(RewardObject, self).__init__()

		self.main_class = main_class
		self.reward_index = index - 1

		self.tier_title_background_image = ui.ImageBox()
		self.tier_title_background_image.SetParent(self)
		self.tier_title_background_image.LoadImage(IMAGE_PATH + "tier_background_image.png")
		self.tier_title_background_image.Show()

		self.tier_title = ui.TextLine()
		self.tier_title.AddFlag("attach")
		self.tier_title.AddFlag("not_pick")
		self.tier_title.SetParent(self.tier_title_background_image)
		self.tier_title.SetText(localeInfo.BATTLEPASS_TIER_TEXT.format(index))
		self.tier_title.SetVerticalAlignCenter()
		self.tier_title.SetHorizontalAlignCenter()
		self.tier_title.SetWindowVerticalAlignCenter()
		self.tier_title.SetWindowHorizontalAlignCenter()
		self.tier_title.SetPosition(0, 0)
		self.tier_title.Show()

		self.reward_item_free_background = ui.ImageBox()
		self.reward_item_free_background.SetParent(self)
		self.reward_item_free_background.SetPosition(0, 25)
		self.reward_item_free_background.LoadImage(IMAGE_PATH + "item_container_free.png")
		self.reward_item_free_background.Show()

		item.SelectItem(reward_info["free_item_vnum"] )
		self.reward_item_free_icon = ui.ImageBox()
		self.reward_item_free_icon.SetParent(self.reward_item_free_background)
		self.reward_item_free_icon.SetWindowVerticalAlignCenter()
		self.reward_item_free_icon.SetWindowHorizontalAlignCenter()
		self.reward_item_free_icon.LoadImage(item.GetIconImageFileName())
		self.reward_item_free_icon.OnMouseOverIn = lambda item_vnum = reward_info["free_item_vnum"] : self.ShowTooltipItem(item_vnum)
		self.reward_item_free_icon.OnMouseOverOut = ui.__mem_func__(self.main_class.HideItemTooltip)
		self.reward_item_free_icon.Show()

		self.reward_item_free_count = ui.NumberLine()
		self.reward_item_free_count.SetParent(self.reward_item_free_icon)
		self.reward_item_free_count.SetPosition(28, 20)
		self.reward_item_free_count.SetHorizontalAlignRight()
		self.reward_item_free_count.SetNumber(str(reward_info["free_item_count"]))
		if (reward_info["free_item_count"] > 1):
			self.reward_item_free_count.Show()

		self.redeem_reward_button = ui.Button()
		self.redeem_reward_button.SetParent(self.reward_item_free_background)
		self.redeem_reward_button.SetUpVisual(IMAGE_PATH + "buttons/collect_01.png")
		self.redeem_reward_button.SetOverVisual(IMAGE_PATH + "buttons/collect_01.png")
		self.redeem_reward_button.SetDownVisual(IMAGE_PATH + "buttons/collect_02.png")
		self.redeem_reward_button.SetDisableVisual(IMAGE_PATH + "buttons/collect_02.png")
		self.redeem_reward_button.SetText(localeInfo.BATTLEPASS_REDEEM_BUTTON_TEXT)
		self.redeem_reward_button.SetWindowVerticalAlignBottom()
		self.redeem_reward_button.SetWindowHorizontalAlignCenter()
		self.redeem_reward_button.SetPosition(0, 22)
		self.redeem_reward_button.SetEvent(ui.__mem_func__(self.CollectReward))
		self.redeem_reward_button.Hide()

		self.reward_item_premium_background = ui.ImageBox()
		self.reward_item_premium_background.SetParent(self)
		self.reward_item_premium_background.SetPosition(0, 135)
		self.reward_item_premium_background.LoadImage(IMAGE_PATH + "item_container_premium.png")
		self.reward_item_premium_background.Show()
		
		item.SelectItem(reward_info["premium_item_vnum"] )
		self.reward_item_premium_icon = ui.ImageBox()
		self.reward_item_premium_icon.SetParent(self.reward_item_premium_background)
		self.reward_item_premium_icon.SetWindowVerticalAlignCenter()
		self.reward_item_premium_icon.SetWindowHorizontalAlignCenter()
		self.reward_item_premium_icon.LoadImage(item.GetIconImageFileName())
		self.reward_item_premium_icon.OnMouseOverIn = lambda item_vnum = reward_info["premium_item_vnum"] : self.ShowTooltipItem(item_vnum)
		self.reward_item_premium_icon.OnMouseOverOut = ui.__mem_func__(self.main_class.HideItemTooltip)
		self.reward_item_premium_icon.SetAlpha(0.5)
		self.reward_item_premium_icon.Show()

		self.reward_item_premium_count = ui.NumberLine()
		self.reward_item_premium_count.SetParent(self.reward_item_premium_icon)
		self.reward_item_premium_count.SetPosition(28, 20)
		self.reward_item_premium_count.SetHorizontalAlignRight()
		self.reward_item_premium_count.SetNumber(str(reward_info["premium_item_count"]))
		self.reward_item_premium_count.Show()
		if (reward_info["premium_item_count"] > 1):
			self.reward_item_premium_count.Show()

		self.reward_item_premium_locked_slot_image = ui.ImageBox()
		self.reward_item_premium_locked_slot_image.SetParent(self.reward_item_premium_background)
		self.reward_item_premium_locked_slot_image.SetWindowVerticalAlignBottom()
		self.reward_item_premium_locked_slot_image.SetWindowHorizontalAlignCenter()
		self.reward_item_premium_locked_slot_image.SetPosition(0, 35)
		self.reward_item_premium_locked_slot_image.LoadImage(IMAGE_PATH + "locked_slot_image.png")
		self.reward_item_premium_locked_slot_image.OnMouseLeftButtonDown = ui.__mem_func__(self.OnBuyPremium)
		self.reward_item_premium_locked_slot_image.Show()
		
		# self.unlock_reward_button = ui.Button()
		# self.unlock_reward_button.SetParent(self.reward_item_premium_background)
		# self.unlock_reward_button.SetUpVisual(IMAGE_PATH + "buttons/collect_01.png")
		# self.unlock_reward_button.SetOverVisual(IMAGE_PATH + "buttons/collect_01.png")
		# self.unlock_reward_button.SetDownVisual(IMAGE_PATH + "buttons/collect_02.png")
		# self.unlock_reward_button.SetDisableVisual(IMAGE_PATH + "buttons/collect_02.png")
		# self.unlock_reward_button.SetText("Unlock")
		# self.unlock_reward_button.SetWindowVerticalAlignBottom()
		# self.unlock_reward_button.SetWindowHorizontalAlignCenter()
		# self.unlock_reward_button.SetPosition(0, 22)
		# self.unlock_reward_button.SetEvent(ui.__mem_func__(self.OnBuyPremium))
		# self.unlock_reward_button.Show()

		self.redeem_reward_button_premium = ui.Button()
		self.redeem_reward_button_premium.SetParent(self.reward_item_premium_background)
		self.redeem_reward_button_premium.SetUpVisual(IMAGE_PATH + "buttons/collect_01.png")
		self.redeem_reward_button_premium.SetOverVisual(IMAGE_PATH + "buttons/collect_01.png")
		self.redeem_reward_button_premium.SetDownVisual(IMAGE_PATH + "buttons/collect_02.png")
		self.redeem_reward_button_premium.SetDisableVisual(IMAGE_PATH + "buttons/collect_02.png")
		self.redeem_reward_button_premium.SetText(localeInfo.BATTLEPASS_REDEEM_BUTTON_TEXT)
		self.redeem_reward_button_premium.SetWindowVerticalAlignBottom()
		self.redeem_reward_button_premium.SetWindowHorizontalAlignCenter()
		self.redeem_reward_button_premium.SetPosition(0, 22)
		self.redeem_reward_button_premium.SetEvent(ui.__mem_func__(self.CollectReward), True)
		self.redeem_reward_button_premium.Hide()


		self.SetSize(self.reward_item_free_background.GetWidth(), 250)

	def ShowTooltipItem(self, item_vnum):
		self.main_class.tooltip_item.ClearToolTip()
		self.main_class.tooltip_item.AddItemData(item_vnum, 0)

	def SetRewardRedeemed(self, reward_redemeed, premium_reward_redemeed):

		if reward_redemeed:
			self.redeem_reward_button.Down()
			self.redeem_reward_button.Disable()
			self.redeem_reward_button.SetText(localeInfo.BATTLEPASS_REDEEMED_BUTTON_TEXT)

		if premium_reward_redemeed:
			self.redeem_reward_button_premium.Down()
			self.redeem_reward_button_premium.Disable()
			self.redeem_reward_button_premium.SetText(localeInfo.BATTLEPASS_REDEEMED_BUTTON_TEXT)

	def ShowRedeemButtons(self, is_premium):
		self.redeem_reward_button.Show()
		if is_premium:
			self.redeem_reward_button_premium.Show()

	def CollectReward(self, is_premium = False):
		battlepass.RedeemMissionReward(self.reward_index, is_premium)

	def OnBuyPremium(self):
		return
		# self.main_class.UnlockPremiumSlots()

	def UnlockPremiumSlot(self):
		self.reward_item_premium_icon.SetAlpha(1.0)
		self.reward_item_premium_locked_slot_image.Hide()

	def __del__(self):
		super(RewardObject, self).__del__()

class ItemListBox(ui.Window):
	PADDING = 5

	def __init__(self, parentClass):
		ui.Window.__init__(self)
		self.parentClass = parentClass
		self.SetDefaultParameters()

	def __del__(self):
		self.Clear()
		ui.Window.__del__(self)

	def Destroy(self):
		self.Clear()

	def SetDefaultParameters(self):
		self.itemList = []
		self.scrollBar = None
		self.selectEvent = None

	def SetScrollBar(self, scrollBar):
		scrollBar.SetScrollEvent(ui.__mem_func__(self.__OnScroll))
		scrollBar.SetScrollStep(0.1)
		self.scrollBar = scrollBar

	def SetSelectEvent(self, event):
		self.selectEvent = event

	def __OnScroll(self):
		self.RefreshItemPosition(True)

	def GetTotalHeightItems(self):
		totalHeight = sum((item.GetHeight() + self.PADDING) for item in self.itemList)
		return totalHeight

	def GetItemCount(self):
		return len(self.itemList)

	def GetItems(self):
		return self.itemList

	def AppendItem(self, data):
		item = self.Item(data, self.parentClass)
		item.SetParent(self)
		item.Show()
		self.itemList.append(item)

		self.ResetScrollBarPos()
		self.ResizeScrollBar()
		self.RefreshItemPosition()

		if len(self.itemList) > 10:
			self.scrollBar.Show()
		else:
			self.scrollBar.Hide()

	def ResizeScrollBar(self):
		totalHeight = float(self.GetTotalHeightItems())
		if totalHeight > 0:
			scrollBarHeight = min(float(self.GetHeight() - 10) / totalHeight, 1.0)
		else:
			scrollBarHeight = 1.0

		self.scrollBar.SetMiddleBarSize(scrollBarHeight)
		
	def ResetScrollBarPos(self):
		self.scrollBar.SetPos(0)

	def RefreshItemPosition(self, bScroll=False):
		scrollPos = self.scrollBar.GetPos()
		totalHeight = self.GetTotalHeightItems() - self.GetHeight()

		yAccumulate = 0
		for item in self.itemList:
			if bScroll:
				setPos = yAccumulate - int(scrollPos * totalHeight)
				item.SetPosition(0, + setPos + self.PADDING)
			else:
				item.SetPosition(0, + yAccumulate + self.PADDING)

			yAccumulate += item.GetHeight() + self.PADDING

	def Clear(self):
		for item in self.itemList:
			item.SetDefaultParameters()
			item.Hide()
			del item

		self.itemList = []

	class Item(ui.Window):
		def __init__(self, data, parent):
			ui.Window.__init__(self)

			self.SetDefaultParameters()
			self.data = data
			self.mainParent = parent

			self.list_item_background_image = ui.ImageBox()
			self.list_item_background_image.SetParent(self)
			self.list_item_background_image.LoadImage(IMAGE_PATH + "mission_normal_background_image.png")
			self.list_item_background_image.Show()
			self.list_item_background_image.SetClippingMaskWindow(self.mainParent.mission_container)

			self.mission_name = ui.TextLine()
			self.mission_name.AddFlag("attach")
			self.mission_name.AddFlag("not_pick")
			self.mission_name.SetParent(self.list_item_background_image)
			self.mission_name.SetText(data["title"])
			self.mission_name.SetVerticalAlignCenter()
			self.mission_name.SetHorizontalAlignLeft()
			self.mission_name.SetWindowVerticalAlignCenter()
			self.mission_name.SetWindowHorizontalAlignLeft()
			self.mission_name.SetPosition(20, 0)
			self.mission_name.SetClippingMaskWindow(self.mainParent.mission_container)
			self.mission_name.SetPackedFontColor(0xFFd9c997)
			self.mission_name.SetOutline()
			self.mission_name.Show()
			
			self.mission_progress = ui.TextLine()
			self.mission_progress.AddFlag("attach")
			self.mission_progress.AddFlag("not_pick")
			self.mission_progress.SetParent(self.list_item_background_image)
			self.mission_progress.SetText(data["progress"])
			self.mission_progress.SetVerticalAlignCenter()
			self.mission_progress.SetHorizontalAlignCenter()
			self.mission_progress.SetWindowVerticalAlignCenter()
			self.mission_progress.SetWindowHorizontalAlignCenter()
			self.mission_progress.SetPosition(0, 0)
			self.mission_progress.SetPackedFontColor(0xFFd9c997)
			self.mission_progress.SetClippingMaskWindow(self.mainParent.mission_container)
			self.mission_progress.SetOutline()
			self.mission_progress.Show()

			self.mission_reward = ui.TextLine()
			self.mission_reward.AddFlag("attach")
			self.mission_reward.AddFlag("not_pick")
			self.mission_reward.SetParent(self.list_item_background_image)
			self.mission_reward.SetText(data["reward"])
			self.mission_reward.SetVerticalAlignCenter()
			self.mission_reward.SetHorizontalAlignRight()
			self.mission_reward.SetWindowVerticalAlignCenter()
			self.mission_reward.SetWindowHorizontalAlignRight()
			self.mission_reward.SetPosition(75, 0)
			self.mission_reward.SetPackedFontColor(0xFFd9c997)
			self.mission_reward.SetClippingMaskWindow(self.mainParent.mission_container)
			self.mission_reward.SetOutline()
			self.mission_reward.Show()

			self.collect_reward_button = ui.Button()
			self.collect_reward_button.SetParent(self.list_item_background_image)
			self.collect_reward_button.SetUpVisual(IMAGE_PATH + "buttons/collect_01.png")
			self.collect_reward_button.SetOverVisual(IMAGE_PATH + "buttons/collect_01.png")
			self.collect_reward_button.SetDownVisual(IMAGE_PATH + "buttons/collect_02.png")
			self.collect_reward_button.SetDisableVisual(IMAGE_PATH + "buttons/collect_02.png")
			self.collect_reward_button.SetText(localeInfo.BATTLEPASS_COLLECT_BUTTON_TEXT)
			self.collect_reward_button.SetWindowVerticalAlignCenter()
			self.collect_reward_button.SetWindowHorizontalAlignRight()
			self.collect_reward_button.SetPosition(60, 0)
			self.collect_reward_button.SetClippingMaskWindow(self.mainParent.mission_container)
			self.collect_reward_button.SetEvent(ui.__mem_func__(self.CollectItem))
			self.collect_reward_button.Show()
			self.SetSize(self.list_item_background_image.GetWidth(), self.list_item_background_image.GetHeight())

		def __del__(self):
			ui.Window.__del__(self)

		def SetDefaultParameters(self):
			self.itemInfo = None

		def CollectItem(self):
			battlepass.RedeemMissionExp(self.data["mission_index"])
			# self.MarkItemCollected()

		def MarkItemCollected(self):
			self.list_item_background_image.LoadImage(IMAGE_PATH + "mission_collected_background_image.png")
			self.collect_reward_button.Down()
			self.collect_reward_button.Disable()
			self.collect_reward_button.SetText(localeInfo.BATTLEPASS_COLLECTED_BUTTON_TEXT)

		def SetParent(self, parent):
			ui.Window.SetParent(self, parent)
			self.parent = proxy(parent)

		def Destroy(self):
			self.SetDefaultParameters()

		def UpdateProgress(self, progress, completed, collected):
			(mission_type, target_object, target_count, exp_reward, min_level, max_level) = battlepass.GetMissionObject(self.data["mission_index"])
			self.mission_progress.SetText("{} / {}".format(localeInfo.NumberWithPoint(progress), localeInfo.NumberWithPoint(target_count)))

			if progress > 0:
				self.list_item_background_image.LoadImage(IMAGE_PATH + "mission_progress_background_image.png")
				if completed:
					self.list_item_background_image.LoadImage(IMAGE_PATH + "mission_completed_background_image.png")

			if collected:
				self.MarkItemCollected()
