# coding: latin_1

import dbg
import player
import item
import grp
import wndMgr
import skill
import shop
import exchange
import grpText
import safebox
import localeInfo
import app
import background
import nonplayer
import chr
import uiScriptLocale
import ui
import mouseModule
import constInfo
import pack
import extern_wa_shopitem

if app.ENABLE_RENDER_TARGET:
	import uiRenderTarget

WARP_SCROLLS = [22011, 22000, 22010]
if app.ENABLE_SASH_SYSTEM:
	import sash

DESC_DEFAULT_MAX_COLS = 26
DESC_WESTERN_MAX_COLS = 35
DESC_WESTERN_MAX_WIDTH = 220

if app.USE_PASSIVE_ABILITY_TAMER:
	PASSIVE_ABILITY_VNUM_LIST = []

	for i in range(skill.PASSIVE_ABILITY_VNUM_TAMER_1, skill.PASSIVE_ABILITY_VNUM_TAMER_MAX):
		PASSIVE_ABILITY_VNUM_LIST.extend([i,])

if app.USE_CRYSTAL_SYSTEM:
	CRYSTAL_BONUS_DESC_TYPE = {\
								item.CRYSTAL_BONUS_TYPE_PVM : localeInfo.CRYSTAL_BONUS_DESC_TYPE_PVM_TOOLTIP,
								item.CRYSTAL_BONUS_TYPE_PVP : localeInfo.CRYSTAL_BONUS_DESC_TYPE_PVP_TOOLTIP
	}

	CRYSTAL_BONUS_TYPE = {\
								item.CRYSTAL_BONUS_TYPE_PVM : localeInfo.CRYSTAL_BONUS_TYPE_PVM_TOOLTIP,
								item.CRYSTAL_BONUS_TYPE_PVP : localeInfo.CRYSTAL_BONUS_TYPE_PVP_TOOLTIP
	}

def chop(n):
	return round(n - 0.5, 1)

def pointop(n):
	t = int(n)
	if t / 10 < 1:
		return "0."+n
	else:		
		return n[0:len(n)-1]+"."+n[len(n)-1:]

def SplitDescription(desc, limit):
	total_tokens = desc.split()
	line_tokens = []
	line_len = 0
	lines = []
	for token in total_tokens:
		if "@" in token:
			sep_pos = token.find("@")
			line_tokens.append(token[:sep_pos])

			lines.append(" ".join(line_tokens))
			line_len = len(token) - (sep_pos + 1)
			line_tokens = [token[sep_pos+1:]]
		else:
			line_len += len(token)
			if len(line_tokens) + line_len > limit:
				lines.append(" ".join(line_tokens))
				line_len = len(token)
				line_tokens = [token]
			else:
				line_tokens.append(token)

	if line_tokens:
		lines.append(" ".join(line_tokens))

	return lines

def GET_AFFECT_STRING(bType, lValue):
	if bType == 0:
		return "UNKNOWN_TYPE[%s] %s" % (str(bType), str(lValue))

	try:
		affectString = ItemToolTip.AFFECT_DICT[bType]

		if type(affectString) != str:
			return affectString(lValue)

		if affectString.find("%d") != -1:
			return affectString % lValue
		else:
			return affectString
	except KeyError:
		return "UNKNOWN_TYPE[%d] %d" % (str(bType), str(lValue))

###################################################################################################
## ToolTip
##
##   NOTE : ÇöÀç´Â Item°ú SkillÀ» »ó¼ÓÀ¸·Î Æ¯È­ ½ÃÄÑµÎ¾úÀ½
##		  ÇÏÁö¸¸ ±×´ÙÁö ÀÇ¹Ì°¡ ¾ø¾î º¸ÀÓ
##
class ToolTip(ui.ThinBoard):

	TOOL_TIP_WIDTH = 190
	TOOL_TIP_HEIGHT = 10

	TEXT_LINE_HEIGHT = 17

	TITLE_COLOR = grp.GenerateColor(0.9490, 0.9058, 0.7568, 1.0)
	SPECIAL_TITLE_COLOR = grp.GenerateColor(1.0, 0.7843, 0.0, 1.0)
	NORMAL_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)
	FONT_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)
	PRICE_COLOR = 0xffFFB96D

	HIGH_PRICE_COLOR = SPECIAL_TITLE_COLOR
	MIDDLE_PRICE_COLOR = grp.GenerateColor(0.85, 0.85, 0.85, 1.0)
	LOW_PRICE_COLOR = grp.GenerateColor(0.7, 0.7, 0.7, 1.0)

	ENABLE_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)
	DISABLE_COLOR = grp.GenerateColor(0.9, 0.4745, 0.4627, 1.0)

	NEGATIVE_COLOR = grp.GenerateColor(0.9, 0.4745, 0.4627, 1.0)
	POSITIVE_COLOR = grp.GenerateColor(0.5411, 0.7254, 0.5568, 1.0)
	SPECIAL_POSITIVE_COLOR = grp.GenerateColor(0.6911, 0.8754, 0.7068, 1.0)
	SPECIAL_POSITIVE_COLOR2 = grp.GenerateColor(0.8824, 0.9804, 0.8824, 1.0)

	if app.ENABLE_BUY_WITH_ITEM:
		SHOP_ITEM_COLOR = 0xff15d131

	if app.BL_67_ATTR:
		ATTR_6TH_7TH_COLOR = -102

	CONDITION_COLOR = 0xffBEB47D
	CAN_LEVEL_UP_COLOR = 0xff8EC292
	CANNOT_LEVEL_UP_COLOR = DISABLE_COLOR
	NEED_SKILL_POINT_COLOR = 0xff9A9CDB

	if app.ENABLE_NEW_NAME_ITEM:
		TYRANIS_TOOLTIP_COLOR = 0xff5FFFF3
		CHANGELOOK_ITEMNAME_COLOR = 0xffBCE55C
		RENDER_TARGET = 0xff90ee90

	if app.ENABLE_YOHARA_SYSTEM:
		APPLY_RANDOM_TEXT_COLOR = 0xff7AF6D4

	#if app.ENABLE_YOHARA_SYSTEM:
	NORMAL_CONQUEROR_LEVEL = 0xFF2596BE

	def __init__(self, width = TOOL_TIP_WIDTH, isPickable=False):
		ui.ThinBoard.__init__(self, "TOP_MOST")

		if isPickable:
			pass
		else:
			self.AddFlag("not_pick")

		self.AddFlag("float")
		self.AddFlag("animate")

		self.followFlag = True
		self.toolTipWidth = width
		self.IsFromInventory = False

		if app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS:
			self.isAffect = False

		self.xPos = -1
		self.yPos = -1

		self.defFontName = localeInfo.UI_DEF_FONT
		self.ClearToolTip()

		if app.ENABLE_ACCE_ABSORBED_ITEM_PREVIEW:
			self.is_pickable = isPickable
			self.acce_preview_tooltip = None
			self.acce_preview = False

	def __del__(self):
		ui.ThinBoard.__del__(self)
		if app.ENABLE_ACCE_ABSORBED_ITEM_PREVIEW:
			del self.acce_preview_tooltip

	def AutoAppendNewTextLineResize(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()
 
		(textWidth, textHeight) = textLine.GetTextSize()
		textWidth += 30
		textHeight += 10
		if self.toolTipWidth < textWidth:
			self.toolTipWidth = textWidth
 
		if centerAlign:
			textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
			textLine.SetHorizontalAlignCenter()
		else:
			textLine.SetPosition(10, self.toolTipHeight)
 
		self.childrenList.append(textLine)
 
		self.toolTipHeight += self.TEXT_LINE_HEIGHT
		self.AlignHorizonalCenter()
		return textLine

	def AppendTextLineMultiBuy(self, text, color = FONT_COLOR, centerAlign = True, height = 0):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()

		if centerAlign:
			if height != 0:
				textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight + (height / 2))
			else:
				textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
			textLine.SetHorizontalAlignCenter()
		else:
			textLine.SetPosition(10, self.toolTipHeight)

		self.childrenList.append(textLine)
		
		if height == 0:
			height = self.TEXT_LINE_HEIGHT

		self.toolTipHeight += height
		self.ResizeToolTip()
		return textLine

	def ClearToolTip(self):
		self.toolTipHeight = 12
		self.childrenList = []

	def SetFollow(self, flag):
		self.followFlag = flag

	def SetDefaultFontName(self, fontName):
		self.defFontName = fontName

	def AppendSpace(self, size):
		self.toolTipHeight += size
		self.ResizeToolTip()

	def AppendHorizontalLine(self):

		for i in xrange(2):
			horizontalLine = ui.Line()
			horizontalLine.SetParent(self)
			horizontalLine.SetPosition(0, self.toolTipHeight + 3 + i)
			horizontalLine.SetWindowHorizontalAlignCenter()
			horizontalLine.SetSize(150, 0)
			horizontalLine.Show()

			if 0 == i:
				horizontalLine.SetColor(0xff555555)
			else:
				horizontalLine.SetColor(0xff000000)

			self.childrenList.append(horizontalLine)

		self.toolTipHeight += 11
		self.ResizeToolTip()
		
	def TextAlignHorizonalCenter(self):
		for child in self.childrenList:
			(x, y) = child.GetLocalPosition()
			try:
				if child.GetText() != "":
					child.SetPosition(self.toolTipWidth / 2, y)
			except:
				pass

		self.ResizeToolTip()

	def AlignHorizonalCenter(self):
		for child in self.childrenList:
			(x, y)=child.GetLocalPosition()
			child.SetPosition(self.toolTipWidth/2, y)
		self.ResizeToolTip()

	if app.ENABLE_ZODIAC_MISSION or app.NEW_SELECT_CHARACTER:
		def SetThinBoardSize(self, width, height = 12):
			self.toolTipWidth = width 
			self.toolTipHeight = height

	def AutoAppendNewTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(FALSE)
		textLine.Show()
		textLine.SetPosition(15, self.toolTipHeight)
		
		self.childrenList.append(textLine)
		(textWidth, textHeight) = textLine.GetTextSize()
		textWidth += 30
		textHeight += 10
		if self.toolTipWidth < textWidth:
			self.toolTipWidth = textWidth
		
		self.toolTipHeight += textHeight
		self.ResizeToolTipText(textWidth, self.toolTipHeight)
		return textLine
	
	def AutoAppendTextLineSpecial(self, text, color = FONT_COLOR, centerAlign = TRUE):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(FALSE)
		textLine.Show()

		self.childrenList.append(textLine)
		(textWidth, textHeight)=textLine.GetTextSize()
		textWidth += 40
		textHeight += 5

		if self.toolTipWidth < textWidth:
			self.toolTipWidth = textWidth
		if centerAlign:
			textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
			textLine.SetHorizontalAlignCenter()
		else:
			textLine.SetPosition(10, self.toolTipHeight)
		self.toolTipHeight += textHeight
		self.ResizeToolTip()
		return textLine

	def AutoAppendTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()

		if centerAlign:
			textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
			textLine.SetHorizontalAlignCenter()

		else:
			textLine.SetPosition(10, self.toolTipHeight)

		self.childrenList.append(textLine)

		(textWidth, textHeight)=textLine.GetTextSize()

		textWidth += 40
		textHeight += 5

		if self.toolTipWidth < textWidth:
			self.toolTipWidth = textWidth

		self.toolTipHeight += textHeight

		return textLine

	def SetThinBoardSize(self, width, height = 12) :
		self.toolTipWidth = width 
		self.toolTipHeight = height

	def GetToolTopHeight(self):
		return self.toolTipHeight

	def AppendTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()

		if centerAlign:
			textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
			textLine.SetHorizontalAlignCenter()

		else:
			textLine.SetPosition(10, self.toolTipHeight)

		self.childrenList.append(textLine)

		self.toolTipHeight += self.TEXT_LINE_HEIGHT
		self.ResizeToolTip()

		return textLine

	def AppendDescription(self, desc, limit, color = FONT_COLOR):
		if localeInfo.IsEUROPE():
			self.__AppendDescription_WesternLanguage(desc, color)
		else:
			self.__AppendDescription_EasternLanguage(desc, limit, color)

	def __AppendDescription_EasternLanguage(self, description, characterLimitation, color=FONT_COLOR):
		length = len(description)
		if 0 == length:
			return

		lineCount = grpText.GetSplitingTextLineCount(description, characterLimitation)
		for i in xrange(lineCount):
			if 0 == i:
				self.AppendSpace(5)
			self.AppendTextLine(grpText.GetSplitingTextLine(description, characterLimitation, i), color)

	def __AppendDescription_WesternLanguage(self, desc, color=FONT_COLOR):
		lines = SplitDescription(desc, DESC_WESTERN_MAX_COLS)
		if not lines:
			return

		self.AppendSpace(5)
		for line in lines:
			self.AppendTextLine(line, color)


	def ResizeToolTip(self):
		self.SetSize(self.toolTipWidth, self.TOOL_TIP_HEIGHT + self.toolTipHeight)
		
	def ResizeToolTipText(self, x, y):
		self.SetSize(x, y)
		
	def SetTitle(self, name):
		self.AppendTextLine(name, self.TITLE_COLOR)

	#app.ENABLE_YOHARA_SYSTEM: isYohara
	def GetLimitTextLineColor(self, curValue, limitValue, isYohara = False):
		if curValue < limitValue:
			return self.DISABLE_COLOR

		return self.NORMAL_CONQUEROR_LEVEL if isYohara else self.ENABLE_COLOR

	def GetChangeTextLineColor(self, value, isSpecial=False):
		if value > 0:
			if isSpecial:
				return self.SPECIAL_POSITIVE_COLOR
			else:
				return self.POSITIVE_COLOR

		if 0 == value:
			return self.NORMAL_COLOR

		return self.NEGATIVE_COLOR

	def SetToolTipPosition(self, x = -1, y = -1):
		self.xPos = x
		self.yPos = y

	def ShowToolTip(self):
		self.SetTop()
		self.Show()

		self.OnUpdate()

	def HideToolTip(self):
		self.IsFromInventory = False

		if app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS:
			self.isAffect = False

		if app.ENABLE_ACCE_ABSORBED_ITEM_PREVIEW:
			if self.acce_preview_tooltip:
				self.acce_preview_tooltip.Hide()
				del self.acce_preview_tooltip
				self.acce_preview_tooltip = None
				self.is_pickable = False

		self.Hide()

	def OnUpdate(self):

		if not self.followFlag:
			return

		x = 0
		y = 0
		width = self.GetWidth()
		height = self.toolTipHeight

		if -1 == self.xPos and -1 == self.yPos:

			(mouseX, mouseY) = wndMgr.GetMousePosition()

			if mouseY < wndMgr.GetScreenHeight() - 300:
				y = mouseY + 40
			else:
				y = mouseY - height - 30

			x = mouseX - width/2

		else:

			x = self.xPos - width/2
			y = self.yPos - height

		x = max(x, 0)
		y = max(y, 0)
		x = min(x + width/2, wndMgr.GetScreenWidth() - width/2) - width/2
		y = min(y + self.GetHeight(), wndMgr.GetScreenHeight()) - self.GetHeight()

		parentWindow = self.GetParentProxy()
		if parentWindow:
			(gx, gy) = parentWindow.GetGlobalPosition()
			x -= gx
			y -= gy

		self.SetPosition(x, y)

		if app.ENABLE_ACCE_ABSORBED_ITEM_PREVIEW:
			if app.IsPressed(app.DIK_LSHIFT):
				if self.acce_preview_tooltip:
					width = 0
					height = 0
					if x < self.acce_preview_tooltip.GetWidth():
						width = self.GetWidth()
					else:
						width = -self.acce_preview_tooltip.GetWidth()

					_height = wndMgr.GetScreenHeight() - self.acce_preview_tooltip.GetHeight()
					if y > _height:
						height = _height - y
					elif y < 0:
						height = 0 - y

					self.acce_preview_tooltip.SetPosition(x + width, y + height)
					self.acce_preview_tooltip.Show()
			else:
				if self.acce_preview_tooltip:
					self.acce_preview_tooltip.Hide()


class ItemToolTip(ToolTip):

	if app.ENABLE_SEND_TARGET_INFO:
		isStone = False
		isBook = False
		isBook2 = False
		
	CHARACTER_NAMES = (
		localeInfo.TOOLTIP_WARRIOR,
		localeInfo.TOOLTIP_ASSASSIN,
		localeInfo.TOOLTIP_SURA,
		localeInfo.TOOLTIP_SHAMAN
	)
	if app.ENABLE_WOLFMAN_CHARACTER:
		CHARACTER_NAMES += (
			localeInfo.TOOLTIP_WOLFMAN,
		)

	CHARACTER_COUNT = len(CHARACTER_NAMES)
	WEAR_NAMES = (
		localeInfo.TOOLTIP_ARMOR,
		localeInfo.TOOLTIP_HELMET,
		localeInfo.TOOLTIP_SHOES,
		localeInfo.TOOLTIP_WRISTLET,
		localeInfo.TOOLTIP_WEAPON,
		localeInfo.TOOLTIP_NECK,
		localeInfo.TOOLTIP_EAR,
		localeInfo.TOOLTIP_UNIQUE,
		localeInfo.TOOLTIP_SHIELD,
		localeInfo.TOOLTIP_ARROW
	)
	WEAR_COUNT = len(WEAR_NAMES)

	AFFECT_DICT = {
		item.APPLY_MAX_HP : localeInfo.TOOLTIP_MAX_HP,
		item.APPLY_MAX_SP : localeInfo.TOOLTIP_MAX_SP,
		item.APPLY_CON : localeInfo.TOOLTIP_CON,
		item.APPLY_INT : localeInfo.TOOLTIP_INT,
		item.APPLY_STR : localeInfo.TOOLTIP_STR,
		item.APPLY_DEX : localeInfo.TOOLTIP_DEX,
		item.APPLY_ATT_SPEED : localeInfo.TOOLTIP_ATT_SPEED,
		item.APPLY_MOV_SPEED : localeInfo.TOOLTIP_MOV_SPEED,
		item.APPLY_CAST_SPEED : localeInfo.TOOLTIP_CAST_SPEED,
		item.APPLY_HP_REGEN : localeInfo.TOOLTIP_HP_REGEN,
		item.APPLY_SP_REGEN : localeInfo.TOOLTIP_SP_REGEN,
		item.APPLY_POISON_PCT : localeInfo.TOOLTIP_APPLY_POISON_PCT,
		item.APPLY_STUN_PCT : localeInfo.TOOLTIP_APPLY_STUN_PCT,
		item.APPLY_SLOW_PCT : localeInfo.TOOLTIP_APPLY_SLOW_PCT,
		item.APPLY_CRITICAL_PCT : localeInfo.TOOLTIP_APPLY_CRITICAL_PCT,
		item.APPLY_PENETRATE_PCT : localeInfo.TOOLTIP_APPLY_PENETRATE_PCT,

		item.APPLY_ATTBONUS_WARRIOR : localeInfo.TOOLTIP_APPLY_ATTBONUS_WARRIOR,
		item.APPLY_ATTBONUS_ASSASSIN : localeInfo.TOOLTIP_APPLY_ATTBONUS_ASSASSIN,
		item.APPLY_ATTBONUS_SURA : localeInfo.TOOLTIP_APPLY_ATTBONUS_SURA,
		item.APPLY_ATTBONUS_SHAMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_SHAMAN,
		item.APPLY_ATTBONUS_MONSTER : localeInfo.TOOLTIP_APPLY_ATTBONUS_MONSTER,

		item.APPLY_ATTBONUS_HUMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_HUMAN,
		item.APPLY_ATTBONUS_ANIMAL : localeInfo.TOOLTIP_APPLY_ATTBONUS_ANIMAL,
		item.APPLY_ATTBONUS_ORC : localeInfo.TOOLTIP_APPLY_ATTBONUS_ORC,
		item.APPLY_ATTBONUS_MILGYO : localeInfo.TOOLTIP_APPLY_ATTBONUS_MILGYO,
		item.APPLY_ATTBONUS_UNDEAD : localeInfo.TOOLTIP_APPLY_ATTBONUS_UNDEAD,
		item.APPLY_ATTBONUS_DEVIL : localeInfo.TOOLTIP_APPLY_ATTBONUS_DEVIL,
		item.APPLY_STEAL_HP : localeInfo.TOOLTIP_APPLY_STEAL_HP,
		item.APPLY_STEAL_SP : localeInfo.TOOLTIP_APPLY_STEAL_SP,
		item.APPLY_MANA_BURN_PCT : localeInfo.TOOLTIP_APPLY_MANA_BURN_PCT,
		item.APPLY_DAMAGE_SP_RECOVER : localeInfo.TOOLTIP_APPLY_DAMAGE_SP_RECOVER,
		item.APPLY_BLOCK : localeInfo.TOOLTIP_APPLY_BLOCK,
		item.APPLY_DODGE : localeInfo.TOOLTIP_APPLY_DODGE,
		item.APPLY_RESIST_SWORD : localeInfo.TOOLTIP_APPLY_RESIST_SWORD,
		item.APPLY_RESIST_TWOHAND : localeInfo.TOOLTIP_APPLY_RESIST_TWOHAND,
		item.APPLY_RESIST_DAGGER : localeInfo.TOOLTIP_APPLY_RESIST_DAGGER,
		item.APPLY_RESIST_BELL : localeInfo.TOOLTIP_APPLY_RESIST_BELL,
		item.APPLY_RESIST_FAN : localeInfo.TOOLTIP_APPLY_RESIST_FAN,
		item.APPLY_RESIST_BOW : localeInfo.TOOLTIP_RESIST_BOW,
		item.APPLY_RESIST_FIRE : localeInfo.TOOLTIP_RESIST_FIRE,
		item.APPLY_RESIST_ELEC : localeInfo.TOOLTIP_RESIST_ELEC,
		item.APPLY_RESIST_MAGIC : localeInfo.TOOLTIP_RESIST_MAGIC,
		item.APPLY_RESIST_WIND : localeInfo.TOOLTIP_APPLY_RESIST_WIND,
		item.APPLY_REFLECT_MELEE : localeInfo.TOOLTIP_APPLY_REFLECT_MELEE,
		item.APPLY_REFLECT_CURSE : localeInfo.TOOLTIP_APPLY_REFLECT_CURSE,
		item.APPLY_POISON_REDUCE : localeInfo.TOOLTIP_APPLY_POISON_REDUCE,
		item.APPLY_KILL_SP_RECOVER : localeInfo.TOOLTIP_APPLY_KILL_SP_RECOVER,
		item.APPLY_EXP_DOUBLE_BONUS : localeInfo.TOOLTIP_APPLY_EXP_DOUBLE_BONUS,
		item.APPLY_GOLD_DOUBLE_BONUS : localeInfo.TOOLTIP_APPLY_GOLD_DOUBLE_BONUS,
		item.APPLY_ITEM_DROP_BONUS : localeInfo.TOOLTIP_APPLY_ITEM_DROP_BONUS,
		item.APPLY_POTION_BONUS : localeInfo.TOOLTIP_APPLY_POTION_BONUS,
		item.APPLY_KILL_HP_RECOVER : localeInfo.TOOLTIP_APPLY_KILL_HP_RECOVER,
		item.APPLY_IMMUNE_STUN : localeInfo.TOOLTIP_APPLY_IMMUNE_STUN,
		item.APPLY_IMMUNE_SLOW : localeInfo.TOOLTIP_APPLY_IMMUNE_SLOW,
		item.APPLY_IMMUNE_FALL : localeInfo.TOOLTIP_APPLY_IMMUNE_FALL,
		item.APPLY_BOW_DISTANCE : localeInfo.TOOLTIP_BOW_DISTANCE,
		item.APPLY_DEF_GRADE_BONUS : localeInfo.TOOLTIP_DEF_GRADE,
		item.APPLY_ATT_GRADE_BONUS : localeInfo.TOOLTIP_ATT_GRADE,
		item.APPLY_MAGIC_ATT_GRADE : localeInfo.TOOLTIP_MAGIC_ATT_GRADE,
		item.APPLY_MAGIC_DEF_GRADE : localeInfo.TOOLTIP_MAGIC_DEF_GRADE,
		item.APPLY_MAX_STAMINA : localeInfo.TOOLTIP_MAX_STAMINA,
		item.APPLY_MALL_ATTBONUS : localeInfo.TOOLTIP_MALL_ATTBONUS,
		item.APPLY_MALL_DEFBONUS : localeInfo.TOOLTIP_MALL_DEFBONUS,
		item.APPLY_MALL_EXPBONUS : localeInfo.TOOLTIP_MALL_EXPBONUS,
		item.APPLY_MALL_ITEMBONUS : localeInfo.TOOLTIP_MALL_ITEMBONUS,
		item.APPLY_MALL_GOLDBONUS : localeInfo.TOOLTIP_MALL_GOLDBONUS,
		item.APPLY_SKILL_DAMAGE_BONUS : localeInfo.TOOLTIP_SKILL_DAMAGE_BONUS,
		item.APPLY_NORMAL_HIT_DAMAGE_BONUS : localeInfo.TOOLTIP_NORMAL_HIT_DAMAGE_BONUS,
		item.APPLY_SKILL_DEFEND_BONUS : localeInfo.TOOLTIP_SKILL_DEFEND_BONUS,
		item.APPLY_NORMAL_HIT_DEFEND_BONUS : localeInfo.TOOLTIP_NORMAL_HIT_DEFEND_BONUS,
		item.APPLY_PC_BANG_EXP_BONUS : localeInfo.TOOLTIP_MALL_EXPBONUS_P_STATIC,
		item.APPLY_PC_BANG_DROP_BONUS : localeInfo.TOOLTIP_MALL_ITEMBONUS_P_STATIC,
		item.APPLY_RESIST_WARRIOR : localeInfo.TOOLTIP_APPLY_RESIST_WARRIOR,
		item.APPLY_RESIST_ASSASSIN : localeInfo.TOOLTIP_APPLY_RESIST_ASSASSIN,
		item.APPLY_RESIST_SURA : localeInfo.TOOLTIP_APPLY_RESIST_SURA,
		item.APPLY_RESIST_SHAMAN : localeInfo.TOOLTIP_APPLY_RESIST_SHAMAN,
		item.APPLY_MAX_HP_PCT : localeInfo.TOOLTIP_APPLY_MAX_HP_PCT,
		item.APPLY_MAX_SP_PCT : localeInfo.TOOLTIP_APPLY_MAX_SP_PCT,
		item.APPLY_ENERGY : localeInfo.TOOLTIP_ENERGY,
		item.APPLY_COSTUME_ATTR_BONUS : localeInfo.TOOLTIP_COSTUME_ATTR_BONUS,

		item.APPLY_MAGIC_ATTBONUS_PER : localeInfo.TOOLTIP_MAGIC_ATTBONUS_PER,
		item.APPLY_MELEE_MAGIC_ATTBONUS_PER : localeInfo.TOOLTIP_MELEE_MAGIC_ATTBONUS_PER,
		item.APPLY_RESIST_ICE : localeInfo.TOOLTIP_RESIST_ICE,
		item.APPLY_RESIST_EARTH : localeInfo.TOOLTIP_RESIST_EARTH,
		item.APPLY_RESIST_DARK : localeInfo.TOOLTIP_RESIST_DARK,
		item.APPLY_ANTI_CRITICAL_PCT : localeInfo.TOOLTIP_ANTI_CRITICAL_PCT,
		item.APPLY_ANTI_PENETRATE_PCT : localeInfo.TOOLTIP_ANTI_PENETRATE_PCT,

		item.APPLY_ATTBONUS_ELEC : localeInfo.TOOLTIP_APPLY_ATTBONUS_ELEC,
		item.APPLY_ATTBONUS_FIRE : localeInfo.TOOLTIP_APPLY_ATTBONUS_FIRE,
		item.APPLY_ATTBONUS_ICE : localeInfo.TOOLTIP_APPLY_ATTBONUS_ICE,
		item.APPLY_ATTBONUS_WIND : localeInfo.TOOLTIP_APPLY_ATTBONUS_WIND,
		item.APPLY_ATTBONUS_EARTH : localeInfo.TOOLTIP_APPLY_ATTBONUS_EARTH,
		item.APPLY_ATTBONUS_DARK : localeInfo.TOOLTIP_APPLY_ATTBONUS_DARK,

	}
	if app.ENABLE_YOHARA_SYSTEM:
		AFFECT_DICT.update({
			item.APPLY_SUNGMA_STR : localeInfo.TOOLTIP_SUNGMA_STR,
			item.APPLY_SUNGMA_HP : localeInfo.TOOLTIP_SUNGMA_HP,
			item.APPLY_SUNGMA_MOVE : localeInfo.TOOLTIP_SUNGMA_MOVE,
			item.APPLY_SUNGMA_IMMUNE : localeInfo.TOOLTIP_SUNGMA_IMMUNE,
		})

	if app.ENABLE_MAGIC_REDUCTION_SYSTEM:
		AFFECT_DICT.update({
			item.APPLY_RESIST_MAGIC_REDUCTION : localeInfo.TOOLTIP_RESIST_MAGIC_REDUCTION,
		})

	AFFECT_DICT.update({
		item.APPLY_RESIST_HUMAN : localeInfo.TOOLTIP_RESIST_HUMAN,
		})
		
	AFFECT_DICT.update({
		item.APPLY_ENCHANT_ELEC : localeInfo.TOOLTIP_APPLY_ENCHANT_ELECT,
		item.APPLY_ENCHANT_FIRE : localeInfo.TOOLTIP_APPLY_ENCHANT_FIRE,
		item.APPLY_ENCHANT_ICE : localeInfo.TOOLTIP_APPLY_ENCHANT_ICE,
		item.APPLY_ENCHANT_WIND : localeInfo.TOOLTIP_APPLY_ENCHANT_WIND,
		item.APPLY_ENCHANT_EARTH : localeInfo.TOOLTIP_APPLY_ENCHANT_EARTH,
		item.APPLY_ENCHANT_DARK : localeInfo.TOOLTIP_APPLY_ENCHANT_DARK,
		
		item.APPLY_ATTBONUS_SWORD : localeInfo.TOOLTIP_APPLY_ATTBONUS_SWORD,
		item.APPLY_ATTBONUS_TWOHAND: localeInfo.TOOLTIP_APPLY_ATTBONUS_TWOHAND,
		item.APPLY_ATTBONUS_DAGGER : localeInfo.TOOLTIP_APPLY_ATTBONUS_DAGGER,
		item.APPLY_ATTBONUS_BELL : localeInfo.TOOLTIP_APPLY_ATTBONUS_BELL,
		item.APPLY_ATTBONUS_FAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_FAN,
		item.APPLY_ATTBONUS_BOW : localeInfo.TOOLTIP_APPLY_ATTBONUS_BOW,
		item.APPLY_ATTBONUS_ZODIAC : localeInfo.TOOLTIP_APPLY_ATTBONUS_ZODIAC,
		item.APPLY_ATTBONUS_DESERT : localeInfo.TOOLTIP_APPLY_ATTBONUS_DESERT,
		item.APPLY_ATTBONUS_INSECT : localeInfo.TOOLTIP_APPLY_ATTBONUS_INSECT,
		#item.APPLY_ATTBONUS_CLAW : localeInfo.TOOLTIP_APPLY_ATTBONUS_CLAW,	
	})
	
	if app.NEW_BONUS:
		AFFECT_DICT.update({
			item.APPLY_ATTBONUS_STONE : localeInfo.TOOLTIP_APPLY_ATT_STONE,
			item.APPLY_ATTBONUS_BOSS : localeInfo.TOOLTIP_APPLY_ATT_BOSS,
		})

	if app.ENABLE_YOHARA_SYSTEM:
		AFFECT_DICT[item.APPLY_HIT_PCT] = localeInfo.TOOLTIP_HIT_PCT
		AFFECT_DICT[item.APPLY_RESIST_MOUNT_FALL] = localeInfo.TOOLTIP_APPLY_RESIST_MOUNT_FALL
		AFFECT_DICT[item.APPLY_MONSTER_DEFEND_BONUS] = localeInfo.TOOLTIP_APPLY_MONSTER_DEFEND_BONUS

	if app.USE_DROP_AFFECT_BONUSES:
		AFFECT_DICT[item.APPLY_DOUBLE_DROP_METIN] = localeInfo.TOOLTIP_APPLY_DOUBLE_DROP_METIN
		AFFECT_DICT[item.APPLY_DOUBLE_DROP_BOSS] = localeInfo.TOOLTIP_APPLY_DOUBLE_DROP_BOSS

	ATTRIBUTE_NEED_WIDTH = {
		23 : 230,
		24 : 230,
		25 : 230,
		26 : 220,
		27 : 210,

		35 : 210,
		36 : 210,
		37 : 210,
		38 : 210,
		39 : 210,
		40 : 210,
		41 : 210,

		42 : 220,
		43 : 230,
		45 : 230,
	}

	ANTI_FLAG_DICT = {
		0 : item.ITEM_ANTIFLAG_WARRIOR,
		1 : item.ITEM_ANTIFLAG_ASSASSIN,
		2 : item.ITEM_ANTIFLAG_SURA,
		3 : item.ITEM_ANTIFLAG_SHAMAN,
	}
	if app.ENABLE_WOLFMAN_CHARACTER:
		ANTI_FLAG_DICT.update({
			4 : item.ITEM_ANTIFLAG_WOLFMAN,
		})

	ITEM_ANTIFLAG_DICT = {
		"|Eemoji/anti_drop|e",
		"|Eemoji/anti_safebox|e",
		"|Eemoji/anti_sell|e",
		"|Eemoji/anti_shop|e",
	}

	ANTI_FLAG_COUNT = len(ITEM_ANTIFLAG_DICT)

	FONT_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)

	if app.USE_OFFLINESHOP_SEARCH_V2:
		def SetShopSearch(self):
			self.show_render = False
			self.isShopSearch = True

	def __init__(self, *args, **kwargs):
		ToolTip.__init__(self, *args, **kwargs)
		self.itemVnum = 0
		self.isShopItem = False

		# ¾ÆÀÌÅÛ ÅøÆÁÀ» Ç¥½ÃÇÒ ¶§ ÇöÀç Ä³¸¯ÅÍ°¡ Âø¿ëÇÒ ¼ö ¾ø´Â ¾ÆÀÌÅÛÀÌ¶ó¸é °­Á¦·Î Disable Color·Î ¼³Á¤ (ÀÌ¹Ì ±×·¸°Ô ÀÛµ¿ÇÏ°í ÀÖÀ¸³ª ²¨¾ß ÇÒ ÇÊ¿ä°¡ ÀÖ¾î¼­)
		self.bCannotUseItemForceSetDisableColor = True

		self.interface = None
		self.IsFromInventory = False

		#self.show_render = True
		self.show_render = False

		self.emoji_safebox = 0
		self.GetItemElementGrade = 0

		if app.USE_OFFLINESHOP_SEARCH_V2:
			self.isShopSearch = False

	def __del__(self):
		ToolTip.__del__(self)

	def BindInterface(self,interface):
		self.interface = interface

	def ShowRender(self,value):
		#self.show_render = value
		self.show_render = False

	def ShowSafeboxEmoji(self,value):
		self.emoji_safebox = value

	def SetCannotUseItemForceSetDisableColor(self, enable):
		self.bCannotUseItemForceSetDisableColor = enable

	if app.ENABLE_DS_SET_RENEWAL:
		def AppendDSTextLine(self, text, color = FONT_COLOR, centerAlign = True, first = False):
			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetFontName(self.defFontName)
			textLine.SetPackedFontColor(color)
			textLine.SetText(text)
			textLine.SetOutline()
			textLine.SetFeather(False)
			textLine.Show()
			if first:
				self.toolTipWidth += self.toolTipWidth
			
			if centerAlign:
				textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
				textLine.SetHorizontalAlignCenter()
			else:
				textLine.SetPosition(10, self.toolTipHeight)
			
			self.childrenList.append(textLine)
			self.toolTipHeight += self.TEXT_LINE_HEIGHT
			
			self.ResizeToolTip()
			return textLine

		def AppendDSSetInfo(self):
			self.AppendDSTextLine(localeInfo.DS_SET_INFO, self.FONT_COLOR, True, True)
			self.AppendDSTextLine(self.__GetAffectString(item.APPLY_ATTBONUS_STONE, 8))
			self.AppendDSTextLine(self.__GetAffectString(item.APPLY_ATTBONUS_MONSTER, 8))
			self.AppendDSTextLine(self.__GetAffectString(item.APPLY_ATTBONUS_HUMAN, 8))

	def CanEquip(self):
		if app.ENABLE_ACCE_ABSORBED_ITEM_PREVIEW:
			if self.acce_preview:
				return True

		if not item.IsEquipmentVID(self.itemVnum):
			return True

		race = player.GetRace()
		job = chr.RaceToJob(race)
		if not self.ANTI_FLAG_DICT.has_key(job):
			return False

		if item.IsAntiFlag(self.ANTI_FLAG_DICT[job]):
			return False

		sex = chr.RaceToSex(race)

		MALE = 1
		FEMALE = 0

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE) and sex == MALE:
			return False

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE) and sex == FEMALE:
			return False

		for i in xrange(item.LIMIT_MAX_NUM):
			(limitType, limitValue) = item.GetLimit(i)

			if app.ENABLE_YOHARA_SYSTEM and limitType == item.LIMIT_CONQUEROR_LEVEL:
				if limitValue > player.GetStatus(player.CONQUEROR_LEVEL):
					return False

			if item.LIMIT_LEVEL == limitType:
				if player.GetStatus(player.LEVEL) < limitValue:
					return False
			"""
			elif item.LIMIT_STR == limitType:
				if player.GetStatus(player.ST) < limitValue:
					return False
			elif item.LIMIT_DEX == limitType:
				if player.GetStatus(player.DX) < limitValue:
					return False
			elif item.LIMIT_INT == limitType:
				if player.GetStatus(player.IQ) < limitValue:
					return False
			elif item.LIMIT_CON == limitType:
				if player.GetStatus(player.HT) < limitValue:
					return False
			"""

		return True

	def AppendTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		if not self.CanEquip() and self.bCannotUseItemForceSetDisableColor:
			color = self.DISABLE_COLOR

		return ToolTip.AppendTextLine(self, text, color, centerAlign)

	def AppendTextLineBuyItem(self, text, color = FONT_COLOR, centerAlign = True, height = 0):
		if not self.CanEquip() and self.bCannotUseItemForceSetDisableColor:
			color = self.DISABLE_COLOR

		self.ResizeToolTip()
		return ToolTip.AppendTextLineMultiBuy(self, text, color, centerAlign, height)

	def ClearToolTip(self):
		self.isShopItem = False
		self.toolTipWidth = self.TOOL_TIP_WIDTH
		ToolTip.ClearToolTip(self)

	#if app.USE_BOOST_POTIONS or app.USE_CRYSTAL_SYSTEM @isAffect
	def SetInventoryItem(self, slotIndex, window_type = player.INVENTORY, isAffect = False):
		itemVnum = player.GetItemIndex(window_type, slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()
		
		if (app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS) and isAffect:
			self.isAffect = True
		else:
			self.IsFromInventory = True

		if shop.IsOpen():
			if (app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS) and isAffect:
				pass
			else:
				item.SelectItem(itemVnum)
				self.AppendSellingPrice(player.GetISellItemPrice(window_type, slotIndex))

		metinSlot = [player.GetItemMetinSocket(window_type, slotIndex, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
		attrSlot = [player.GetItemAttribute(window_type, slotIndex, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]

		if app.ENABLE_YOHARA_SYSTEM:
			apply_random_list = [player.GetItemApplyRandom(window_type, slotIndex, i) for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM)]

		if (app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS) and isAffect:
			pass
		else:
			self.ShowSafeboxEmoji(1)

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, window_type, slotIndex, 0, 0, apply_random_list, "^", 1)

		if app.ENABLE_IKASHOP_RENEWAL:
			if app.EXTEND_IKASHOP_ULTIMATE:
				if itemVnum == 51998:
					self.AppendTextLine(localeInfo.IKASHOP_ULTIMATE_SHOP_PRIVATE_KEY_MULTIUSE_TOOLTIP)

	if (app.WJ_COMBAT_ZONE):
		def SetShopItemByCombatZoneCoin(self, slotIndex):
			itemVnum = shop.GetItemID(slotIndex)
			if 0 == itemVnum:
				return

			price = shop.GetItemPrice(slotIndex)
			self.ClearToolTip()
			self.isShopItem = TRUE

			metinSlot = []
			for i in xrange(player.METIN_SOCKET_MAX_NUM):
				metinSlot.append(shop.GetItemMetinSocket(slotIndex, i))
			attrSlot = []
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				attrSlot.append(shop.GetItemAttribute(slotIndex, i))

			self.AddItemData(itemVnum, metinSlot, attrSlot)

			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_BUYPRICE  % (localeInfo.NumberToCombatZoneCoinString(price)), self.HIGH_PRICE_COLOR)

	def SetShopItem(self, slotIndex):
		itemVnum = shop.GetItemID(slotIndex)
		if 0 == itemVnum:
			return

		price = shop.GetItemPrice(slotIndex)
		self.ClearToolTip()
		self.isShopItem = True

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(shop.GetItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(shop.GetItemAttribute(slotIndex, i))

		if app.ENABLE_YOHARA_SYSTEM:
			applyRandomList = []
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				applyRandomList.append(shop.GetItemApplyRandom(slotIndex, i))

		if app.ENABLE_ITEMSHOP:
			item.SelectItem(itemVnum)
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)
				if item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
					metinSlot[2] = limitValue
				elif item.LIMIT_REAL_TIME == limitType or item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
					#metinSlot[2] = app.GetGlobalTimeStamp()+limitValue
					metinSlot[2] = limitValue

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, player.INVENTORY,-1, 0, 0, applyRandomList)

		if app.ENABLE_BUY_WITH_ITEM:
			self.AppendSpace(5)
			self.AppendTextLineBuyItem(localeInfo.ITEM_PRICE_TITLE, self.GetPriceColor(price))
			priced = False
			if price > 0:
				self.AppendPrice(price)
				priced = True

			for i in xrange(shop.MAX_SHOP_PRICES):
				(vnum, count) = shop.GetBuyWithItem(slotIndex, i)
				if vnum > 0 and count > 0:
					if not priced:
						priced = True
					
					self.AppendPrice_WithItem(vnum, count)
			
			if not priced:
				self.AppendTextLineBuyItem(localeInfo.ITEM_IS_FREE, self.GetPriceColor(price))
		else:
			self.AppendPrice(price)

		if app.ENABLE_SEND_TARGET_INFO:
			def SetItemToolTipStone(self, itemVnum):
				self.itemVnum = itemVnum
				item.SelectItem(itemVnum)
				itemType = item.GetItemType()

				itemDesc = item.GetItemDescription()
				itemSummary = item.GetItemSummary()
				attrSlot = 0
				self.__AdjustMaxWidth(attrSlot, itemDesc)
				itemName = item.GetItemName()
				realName = itemName[:itemName.find("+")]
				self.SetTitle(realName + " +0 - +4")

				## Description ###
				self.AppendDescription(itemDesc, 26)
				self.AppendDescription(itemSummary, 26, self.CONDITION_COLOR)

				if item.ITEM_TYPE_METIN == itemType:
					self.AppendMetinInformation()
					self.AppendMetinWearInformation()

				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
						self.AppendRealTimeStartFirstUseLastTime(item, metinSlot, i)

					elif item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
						self.AppendTimerBasedOnWearLastTime(metinSlot)

				self.ShowToolTip()

	def SetShopItemBySecondaryCoin(self, slotIndex):
		itemVnum = shop.GetItemID(slotIndex)
		if 0 == itemVnum:
			return

		price = shop.GetItemPrice(slotIndex)
		self.ClearToolTip()
		self.isShopItem = True

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(shop.GetItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(shop.GetItemAttribute(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot)

		self.AppendPriceBySecondaryCoin(price)

	if app.ENABLE_BUY_WITH_ITEM:
		def AppendTextLine_WithItem(self, text, img, color = FONT_COLOR, itemsize = 32):
			ayraC = text.split("|")
			if (len(ayraC) < 2):
				return

			priceText = ui.TextLine()
			priceText.SetParent(self)
			priceText.SetPackedFontColor(color)
			priceText.Show()
			priceText.SetText("%s" % ayraC[0])
			self.childrenList.append(priceText)
			
			tw, th = priceText.GetTextSize() 
			
			image = ui.ImageBox()
			image.SetParent(self)
			image.Show()
			image.LoadImage(img)
			self.childrenList.append(image)
			
			priceItemText = ui.TextLine()
			priceItemText.SetParent(self)
			priceItemText.SetPackedFontColor(color)
			priceItemText.Show()
			priceItemText.SetText(ayraC[1])
			self.childrenList.append(priceItemText)
			
			tiw, tih = priceItemText.GetTextSize() 
			
			topPos = self.toolTipHeight + 12
			leftPos = self.toolTipWidth/2 - (tw + tiw + 32) / 2
			priceText.SetPosition(leftPos, topPos)
			image.SetPosition(leftPos + tw, topPos - 12)
			priceItemText.SetPosition(leftPos + tw + 32, topPos)
			self.toolTipHeight += itemsize
			self.ResizeToolTip()

		def AppendPrice_WithItem(self, vnum, price):
			item.SelectItem(vnum)
			icon = item.GetIconImageFileName()
			(w, h) = item.GetItemSize()
			base_text = localeInfo.NumberToWithItemString(price, item.GetItemName())
			item_count = player.GetItemCountByVnum(vnum)
			base_color = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)  # Light gray color for base text

			self.AppendTextLine_WithItem(base_text + " (" + str(item_count) + ") ", icon, base_color, 32 * h)

	def SetExchangeOwnerItem(self, slotIndex):
		itemVnum = exchange.GetItemVnumFromSelf(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(exchange.GetItemMetinSocketFromSelf(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(exchange.GetItemAttributeFromSelf(slotIndex, i))

		if app.ENABLE_YOHARA_SYSTEM:
			applyRandomList = []
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				applyRandomList.append(exchange.GetItemApplyRandomFromSelf(slotIndex, i))

		if app.ENABLE_NEW_NAME_ITEM:
			newname = exchange.GetItemNewName(slotIndex,True)
			if not newname:
				newname = "^"

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, player.INVENTORY, -1, 0, 0, applyRandomList, newname)

	def SetExchangeTargetItem(self, slotIndex):
		itemVnum = exchange.GetItemVnumFromTarget(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(exchange.GetItemMetinSocketFromTarget(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(exchange.GetItemAttributeFromTarget(slotIndex, i))

		if app.ENABLE_YOHARA_SYSTEM:
			applyRandomList = []
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				applyRandomList.append(exchange.GetItemApplyRandomFromTarget(slotIndex, i))

		if app.ENABLE_NEW_NAME_ITEM:
			newname = exchange.GetItemNewName(slotIndex,False)
			if not newname:
				newname = "^"

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, player.INVENTORY, -1, 0, 0, applyRandomList, newname)

	def SetEditPrivateShopItem(self, invenType, invenPos, price):
		itemVnum = player.GetItemIndex(invenType, invenPos)
		if 0 == itemVnum:
			return

		item.SelectItem(itemVnum)
		self.ClearToolTip()
		self.AppendSellingPrice(price)

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(invenPos, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(invenPos, i))

		if app.ENABLE_YOHARA_SYSTEM:
			applyRandomList = []
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				applyRandomList.append(shop.GetItemApplyRandom(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, player.INVENTORY, -1, 0, 0, applyRandomList, newname)

	def SetPrivateShopBuilderItem(self, invenType, invenPos, privateShopSlotIndex):
		itemVnum = player.GetItemIndex(invenType, invenPos)
		if 0 == itemVnum:
			return

		item.SelectItem(itemVnum)
		self.ClearToolTip()
		self.AppendSellingPrice(shop.GetPrivateShopItemPrice(invenType, invenPos))

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(invenPos, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(invenPos, i))

		if app.ENABLE_YOHARA_SYSTEM:
			applyRandomList = []
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				applyRandomList.append(shop.GetItemApplyRandom(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, player.INVENTORY, -1, 0, 0, applyRandomList)

	def SetSafeBoxItem(self, slotIndex):
		itemVnum = safebox.GetItemID(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(safebox.GetItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(safebox.GetItemAttribute(slotIndex, i))

		if app.ENABLE_YOHARA_SYSTEM:
			apply_random_list = []
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				apply_random_list.append(safebox.GetItemApplyRandom(slotIndex, i))

		self.ShowSafeboxEmoji(2)

		if app.ENABLE_NEW_NAME_ITEM:
			newname = safebox.GetItemNewName(slotIndex)
			if not newname:
				newname = "^"

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, safebox.GetItemFlags(slotIndex), -1, 0, 0, apply_random_list, newname)

	def SetMallItem(self, slotIndex):
		itemVnum = safebox.GetMallItemID(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(safebox.GetMallItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(safebox.GetMallItemAttribute(slotIndex, i))
		
		if app.ENABLE_ITEMSHOP:
			item.SelectItem(itemVnum)
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)
				if item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
					metinSlot[0] = limitValue
				elif item.LIMIT_REAL_TIME == limitType or item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
					metinSlot[0] = app.GetGlobalTimeStamp()+limitValue

		self.AddItemData(itemVnum, metinSlot, attrSlot)

	if app.ENABLE_WIKI:
		def SetItemToolTipWiki(self, itemVnum):
			self.itemVnum = itemVnum
			item.SelectItem(itemVnum)
			metinSlot = [0 for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot = [(0,0) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)
				if item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
					metinSlot[0] = limitValue
				elif item.LIMIT_REAL_TIME == limitType or item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
					metinSlot[0] = app.GetGlobalTimeStamp()+limitValue
			self.SetTitle(item.GetItemName())
			self.AppendDescription(item.GetItemDescription(), 26)
			self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)
			self.AppendWearableInformation()
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)
				if item.LIMIT_REAL_TIME == limitType:
					if metinSlot[0] > 0:
						self.AppendMallItemLastTime(metinSlot[0])
					break
			self.ShowToolTip()

	def SetItemToolTip(self, itemVnum):
		self.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(0)
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append((0, 0))

		if app.ENABLE_YOHARA_SYSTEM:
			applyRandomList = []
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				applyRandomList.append((0, 0))

		if app.ENABLE_ITEMSHOP:
			item.SelectItem(itemVnum)
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)
				if item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
					metinSlot[0] = limitValue
				elif item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
					metinSlot[0] = limitValue
				elif item.LIMIT_REAL_TIME == limitType:
					metinSlot[0] = app.GetGlobalTimeStamp()+limitValue
		
		
		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, player.INVENTORY, -1, 0, 0, applyRandomList)

	def SetItemBuff(self, itemVnum, time, permanente):
		self.ClearToolTip()

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(0)

		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append((0, 0))

		self.AddItemData(itemVnum,metinSlot,attrSlot,0, 0, player.INVENTORY, -1, 1)

		if permanente == 0:
			self.AppendMallItemLastTime(time)
		else:
			self.AppendSpace(5)
			self.AppendTextLine("Permanente")

	def __AppendAttackSpeedInfo(self, item):
		atkSpd = item.GetValue(0)

		if atkSpd < 80:
			stSpd = localeInfo.TOOLTIP_ITEM_VERY_FAST
		elif atkSpd <= 95:
			stSpd = localeInfo.TOOLTIP_ITEM_FAST
		elif atkSpd <= 105:
			stSpd = localeInfo.TOOLTIP_ITEM_NORMAL
		elif atkSpd <= 120:
			stSpd = localeInfo.TOOLTIP_ITEM_SLOW
		else:
			stSpd = localeInfo.TOOLTIP_ITEM_VERY_SLOW

		self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_SPEED % stSpd, self.NORMAL_COLOR)

	def __AppendAttackGradeInfo(self):
		atkGrade = item.GetValue(1)
		self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_GRADE % atkGrade, self.GetChangeTextLineColor(atkGrade))
	
	if app.ENABLE_SASH_SYSTEM:
		def CalcSashValue(self, value, abs):

			if not value:
				return 0
			
			valueCalc = round(value * abs) / 100
			valueCalc += 0.5 #changed the -0.5 which + 0.5 (rounding bug)
			valueCalc = int(valueCalc) +1 if valueCalc > 0 else int(valueCalc)
			value = 1 if (valueCalc <= 0 and value > 0) else valueCalc
			return value

	def __AppendAttackPowerInfo(self, itemAbsChance = 0):
		minPower = item.GetValue(3)
		maxPower = item.GetValue(4)
		addPower = item.GetValue(5)
		
		if app.ENABLE_SASH_SYSTEM:
			if itemAbsChance:
				minPower = self.CalcSashValue(minPower, itemAbsChance)
				maxPower = self.CalcSashValue(maxPower, itemAbsChance)
				addPower = self.CalcSashValue(addPower, itemAbsChance)
		
		if maxPower > minPower:
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_POWER % (minPower+addPower, maxPower+addPower), self.POSITIVE_COLOR)
		else:
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_POWER_ONE_ARG % (minPower+addPower), self.POSITIVE_COLOR)

	def __AppendMagicAttackInfo(self, itemAbsChance = 0):
		minMagicAttackPower = item.GetValue(1)
		maxMagicAttackPower = item.GetValue(2)
		addPower = item.GetValue(5)
		
		if app.ENABLE_SASH_SYSTEM:
			if itemAbsChance:
				minMagicAttackPower = self.CalcSashValue(minMagicAttackPower, itemAbsChance)
				maxMagicAttackPower = self.CalcSashValue(maxMagicAttackPower, itemAbsChance)
				addPower = self.CalcSashValue(addPower, itemAbsChance)
		
		if minMagicAttackPower > 0 or maxMagicAttackPower > 0:
			if maxMagicAttackPower > minMagicAttackPower:
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_ATT_POWER % (minMagicAttackPower + addPower, maxMagicAttackPower + addPower), self.POSITIVE_COLOR)
			else:
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_ATT_POWER_ONE_ARG % (minMagicAttackPower + addPower), self.POSITIVE_COLOR)

	def __AppendMagicDefenceInfo(self, itemAbsChance = 0):
		magicDefencePower = item.GetValue(0)
		
		if app.ENABLE_SASH_SYSTEM:
			if itemAbsChance:
				magicDefencePower = self.CalcSashValue(magicDefencePower, itemAbsChance)
		
		if magicDefencePower > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_DEF_POWER % magicDefencePower, self.GetChangeTextLineColor(magicDefencePower))

	def __AppendBonusElementSash(self, itemAbsChance = 0):
		if self.GetItemElementGrade >= 0:
			if self.GetAddElementSpellOpen and self.GetFuncElementSpell:
				self.GetItemElementValue += 1
				self.AppendTextLine(self.AFFECT_DICT_ELEMENT[self.GetItemElementType][1]%(self.GetItemElementValue,self.GetItemElementValue+7), self.AFFECT_DICT_ELEMENT[self.GetItemElementType][2])
			else:
				if self.GetItemElementGrade > 0:
					if item.GetItemType() == item.ITEM_TYPE_COSTUME and item.GetItemSubType() == item.COSTUME_TYPE_SASH and itemAbsChance:
						value = self.CalcSashValue(self.GetItemElementValue, itemAbsChance)

						self.AppendTextLine(self.AFFECT_DICT[self.GetItemElementType](value), self.AFFECT_DICT_ELEMENT[self.GetItemElementType][2])

	if app.ENABLE_SPECIAL_COSTUME_ATTR:
		def __AppendAttributeInformation(self, attrSlot, itemAbsChance = 0, itemVnum = 0):
			if 0 != attrSlot:
				if app.ENABLE_LOCK_ATTR:
					lockIndex=[]
					isAddonItem = False
					for j in xrange(5):
						if attrSlot[j][0] == item.APPLY_NORMAL_HIT_DAMAGE_BONUS or attrSlot[j][0] == item.APPLY_SKILL_DAMAGE_BONUS:
							isAddonItem = True
							break
					for j in xrange(2):
						if attrSlot[7+j][1] != 0:
							if isAddonItem:
								if attrSlot[7+j][1] == 1 or attrSlot[7+j][1] == 2:
									lockIndex.append(0)
									lockIndex.append(1)
									continue
							lockIndex.append(attrSlot[7+j][1]-1)

				needInfo = False
				attrLimit = -1
				if itemVnum != 0:
					item.SelectItem(itemVnum)
					__CanAttrNewAttr = {
						item.ITEM_TYPE_COSTUME: {
							item.COSTUME_TYPE_BODY : item.SPECIAL_ATTR_COSTUME_BODY_LIMIT,
							item.COSTUME_TYPE_HAIR : item.SPECIAL_ATTR_COSTUME_HAIR_LIMIT,
							item.COSTUME_TYPE_WEAPON : item.SPECIAL_ATTR_COSTUME_WEAPON_LIMIT,
							item.COSTUME_TYPE_MOUNT_SKIN : item.SPECIAL_ATTR_COSTUME_MOUNT_SKIN_LIMIT,
							item.COSTUME_TYPE_SKIN_SASH : item.SPECIAL_ATTR_COSTUME_SASH_SKIN_LIMIT,
						},
						item.ITEM_TYPE_SHINING : {
							item.SHINING_WEAPON : item.SPECIAL_ATTR_COSTUME_SHINING_LIMIT,
							item.SHINING_ARMOR : item.SPECIAL_ATTR_COSTUME_SHINING_LIMIT,
							item.SHINING_SPECIAL : item.SPECIAL_ATTR_COSTUME_SHINING_LIMIT,
						},
					}
					if __CanAttrNewAttr.has_key(item.GetItemType()):
						if __CanAttrNewAttr[item.GetItemType()].has_key(item.GetItemSubType()):
							needInfo = True
							attrLimit = __CanAttrNewAttr[item.GetItemType()][item.GetItemSubType()]
							
				self.forbiddenItems = [40457, 40458, 40459, 40460, 40461, 40462, 41324, 41325, 45160, 45161, 40576, 40577, 40578, 40579, 49042, 49037, 49038, 49039, 49040, 49041, 49290, 49291, 49292, 49293, 49294, 49295, 49296, 49297, 49298, 49299]
				
				for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
					type = attrSlot[i][0]
					value = attrSlot[i][1]
					if 0 == value and not itemVnum in self.forbiddenItems:
						if needInfo == True and i <= attrLimit-1:
							self.AppendTextLine(localeInfo.NOBONUS, self.NORMAL_COLOR)
						if app.ENABLE_LOCK_ATTR:
							if type == 0 or 0 == value:
								continue
						continue

					affectString = self.__GetAffectString(type, value)
					if app.ENABLE_SASH_SYSTEM:
						if item.GetItemType() == item.ITEM_TYPE_COSTUME and item.GetItemSubType() == item.COSTUME_TYPE_SASH and itemAbsChance:
							value = self.CalcSashValue(value, itemAbsChance)
							affectString = self.__GetAffectString(type, value)

					if affectString:
						affectColor = self.__GetAttributeColor(i, value)
						if app.ENABLE_LOCK_ATTR:
							if i in lockIndex:
								self.AppendTextLine("<<"+affectString+">>", self.SPECIAL_TITLE_COLOR)
								continue
						self.AppendTextLine(affectString, affectColor)
	else:
		def __AppendAttributeInformation(self, attrSlot, itemAbsChance = 0):
			if 0 != attrSlot:
				for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
					type = attrSlot[i][0]
					value = attrSlot[i][1]
					if 0 == value:
						continue
					affectString = self.__GetAffectString(type, value)
					if app.ENABLE_SASH_SYSTEM:
						if item.GetItemType() == item.ITEM_TYPE_COSTUME and item.GetItemSubType() == item.COSTUME_TYPE_SASH and itemAbsChance:
							value = self.CalcSashValue(value, itemAbsChance)
							affectString = self.__GetAffectString(type, value)
					
					if affectString:
						affectColor = self.__GetAttributeColor(i, value)
						self.AppendTextLine(affectString, affectColor)
	
	if app.ENABLE_IKASHOP_RENEWAL:
		def AppendAttributes(self, attrs):
			attrs = list(attrs)
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM - len(attrs)):
				attrs.append((0,0))
			self.__AppendAttributeInformation(attrs)

	if app.ENABLE_YOHARA_SYSTEM:
		def __AppendDefaultItemApplyInformation(self, apply_random_list):
			if 0 != apply_random_list:
				for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
					type = apply_random_list[i][0]
					value = apply_random_list[i][1]

					if 0 == value:
						continue

					affectString = self.__GetAffectString(type, value)
					if affectString:
						affectColor = self.APPLY_RANDOM_TEXT_COLOR
						self.AppendTextLine(affectString, affectColor)

	def __GetAttributeColor(self, index, value):
		if value > 0:
			if index >= player.ATTRIBUTE_SLOT_RARE_START and index < player.ATTRIBUTE_SLOT_RARE_END:
				return self.ATTR_6TH_7TH_COLOR
			else:
				return self.SPECIAL_POSITIVE_COLOR
		elif value == 0:
			return self.NORMAL_COLOR
		else:
			return self.NEGATIVE_COLOR

	if app.BL_67_ATTR:
		def AppendAttribute6th7thPossibility(self, attrSlot):
			if attrSlot == 0:
				return

			count = 0
			for i in range(player.ATTRIBUTE_SLOT_MAX_NUM):
				type = attrSlot[i][0]
				value = attrSlot[i][1]

				if 0 == type or 0 == value:
					continue

				count += 1

			if (5 <= count <= 6):
				self.AppendTextLine(localeInfo.ATTR_6TH_7TH_POSSIBILITY, self.ATTR_6TH_7TH_COLOR)

	def __IsPolymorphItem(self, itemVnum):
		if itemVnum >= 70103 and itemVnum <= 70106:
			return 1
		return 0

	def __SetPolymorphItemTitle(self, monsterVnum):
		if localeInfo.IsVIETNAM():
			itemName =item.GetItemName()
			itemName+=" "
			itemName+=nonplayer.GetMonsterName(monsterVnum)
		else:
			itemName =nonplayer.GetMonsterName(monsterVnum)
			itemName+=" "
			itemName+=item.GetItemName()
		self.SetTitle(itemName)

	if app.ENABLE_NEW_NAME_ITEM:
		def __SetItemTitle(self, itemVnum, metinSlot, attrSlot, window_type = player.INVENTORY, slotIndex = -1, newname = "^"):
			if self.__IsPolymorphItem(itemVnum):
				self.__SetPolymorphItemTitle(metinSlot[0])
			else:
				if newname == "^" and slotIndex >= 0:
					if window_type == player.INVENTORY:
						newname = player.GetItemNewName(window_type, slotIndex)
					elif window_type == player.SAFEBOX:
						newname = safebox.GetItemNewName(slotIndex)
					elif window_type == player.MALL:
						newname = safebox.GetItemNewName(slotIndex)
				if self.__IsAttr(attrSlot):
					self.__SetSpecialItemTitle(newname)
					return
				self.__SetNormalItemTitle(newname)
		def __SetNormalItemTitle(self, newname):
			if app.ENABLE_SEND_TARGET_INFO:
				if self.isStone:
					itemName = item.GetItemName()
					realName = itemName[:itemName.find("+")]
					self.SetTitle(realName + " +0 - +4")
				else:
					if newname != "^":
						self.SetTitle(newname)
					else:
						self.SetTitle(item.GetItemName())
			else:
				self.SetTitle(item.GetItemName())
		def __SetSpecialItemTitle(self, newname):
			if newname != "^":
				self.AppendTextLine(newname, self.SPECIAL_TITLE_COLOR)
			else:
				self.AppendTextLine(item.GetItemName(), self.SPECIAL_TITLE_COLOR)
	else:
		def __SetNormalItemTitle(self):
			if app.ENABLE_SEND_TARGET_INFO:
				if self.isStone:
					itemName = item.GetItemName()
					realName = itemName[:itemName.find("+")]
					self.SetTitle(realName + " +0/+4")
				else:
					self.SetTitle(item.GetItemName())
			else:
				self.SetTitle(item.GetItemName())

		def __SetSpecialItemTitle(self):
			self.AppendTextLine(item.GetItemName(), self.SPECIAL_TITLE_COLOR)


		def __SetItemTitle(self, itemVnum, metinSlot, attrSlot):
			if localeInfo.IsCANADA():
				if 72726 == itemVnum or 72730 == itemVnum:
					self.AppendTextLine(item.GetItemName(), grp.GenerateColor(1.0, 0.7843, 0.0, 1.0))
					return

			if self.__IsPolymorphItem(itemVnum):
				self.__SetPolymorphItemTitle(metinSlot[0])
			else:
				if self.__IsAttr(attrSlot):
					self.__SetSpecialItemTitle()
					return

				self.__SetNormalItemTitle()

	if app.ENABLE_RENDER_TARGET:
		def __AppendRenderTarget(self, itemVnum):
			if uiRenderTarget.IsCanShowItems(itemVnum):
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_RENDER_TARGET, self.POSITIVE_COLOR)

	def __IsAttr(self, attrSlot):
		if not attrSlot:
			return False

		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			type = attrSlot[i][0]
			if 0 != type:
				return True

		return False

	def AddRefineItemData(self, itemVnum, metinSlot, attrSlot = 0):
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlotData=metinSlot[i]
			if self.GetMetinItemIndex(metinSlotData) == constInfo.ERROR_METIN_STONE:
				metinSlot[i]=player.METIN_SOCKET_TYPE_SILVER

		self.show_render = False

		self.AddItemData(itemVnum, metinSlot, attrSlot)

	def AddItemData_Offline(self, itemVnum, itemDesc, itemSummary, metinSlot, attrSlot):
		self.__AdjustMaxWidth(attrSlot, itemDesc)
		self.__SetItemTitle(itemVnum, metinSlot, attrSlot)

		if self.__IsHair(itemVnum):
			self.__AppendHairIcon(itemVnum)

		### Description ###
		self.AppendDescription(itemDesc, 26)
		self.AppendDescription(itemSummary, 26, self.CONDITION_COLOR)

	def check_sigillo(self, item_vnum):
		for x in range(55701,55705):
			if x == item_vnum:
				return TRUE	
		if item_vnum == 55801:
			return TRUE
		return FALSE

	def AddSearchItemData(self, itemVnum, gold, attrSlot = 0):
		self.__AppendSearchIcon(itemVnum)

		self.show_render = False
		
		self.AddItemData(itemVnum,0,attrSlot,0,0,0)

		if gold != -1:
			self.AppendTextLine(localeInfo.PRICE01+localeInfo.NumberToMoneyString(gold), self.GetPriceColor(gold))

	def IsCanSeeRender(self, itemType, itemSubType):
		if item.ITEM_TYPE_COSTUME == itemType:
			if item.COSTUME_TYPE_HAIR == itemSubType\
				or item.COSTUME_TYPE_BODY == itemSubType\
				or item.COSTUME_TYPE_MOUNT == itemSubType\
				or item.COSTUME_TYPE_PET == itemSubType\
				or item.COSTUME_TYPE_SASH == itemSubType\
				or item.COSTUME_TYPE_WEAPON == itemSubType:
					return True
		elif item.ITEM_TYPE_ARMOR == itemType and itemSubType == item.ARMOR_BODY:
			return True
		elif item.ITEM_TYPE_WEAPON == itemType and itemSubType != item.WEAPON_ARROW:
			return True
		elif item.ITEM_TYPE_PET == itemType and itemSubType == item.PET_LEVELABLE:
			return True
		elif item.ITEM_TYPE_SHINING == itemType and (itemSubType == item.SHINING_WEAPON or itemSubType == item.SHINING_ARMOR):
			return True
		return False

	## apply_random_list -> ENABLE_YOHARA_SYSTEM
	def AddItemData(self, itemVnum, metinSlot, attrSlot = 0, flags = 0, unbindTime = 0, window_type = player.INVENTORY, slotIndex = -1, buff  = 0, search = 0, apply_random_list = 0, newname = "^", window_open = 0):
		if itemVnum == 55501:
			self.toolTipWidth = 250
			self.ResizeToolTip()
	
		self.itemVnum = itemVnum
		item.SelectItem(itemVnum)
		itemType = item.GetItemType()
		itemSubType = item.GetItemSubType()

		if 50026 == itemVnum:
			if 0 != metinSlot:
				name = item.GetItemName()
				if metinSlot[0] > 0:
					name += " "
					name += localeInfo.NumberToMoneyString(metinSlot[0])
				self.SetTitle(name)
				self.__AppendSealInformation(window_type, slotIndex) ## cyh itemseal 2013 11 11
				self.ShowToolTip()
			return

		### Skill Book ###
		if app.ENABLE_SEND_TARGET_INFO:
			if 50300 == itemVnum and not self.isBook:
				if 0 != metinSlot and not self.isBook:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILLBOOK_NAME, 1)
					self.ShowToolTip()
				elif self.isBook:
					self.SetTitle(item.GetItemName())
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
				
				if self.emoji_safebox > 0 and self.interface.SafeboxIsShow():
					self.AppendSpace(5)
					self.AppendTextLine("|Eemoji/key_ctrl|e + |Eemoji/key_lclick|e - Inventar")
				return
			elif 70037 == itemVnum :
				if 0 != metinSlot and not self.isBook2:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME, 0)
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
				elif self.isBook2:
					self.SetTitle(item.GetItemName())
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
				
				if self.emoji_safebox > 0 and self.interface.SafeboxIsShow():
					self.AppendSpace(5)
					self.AppendTextLine("|Eemoji/key_ctrl|e + |Eemoji/key_lclick|e - Inventar")
					
				return
			elif 70055 == itemVnum:
				if 0 != metinSlot:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME, 0)
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
				
				if self.emoji_safebox > 0 and self.interface.SafeboxIsShow():
					self.AppendSpace(5)
					self.AppendTextLine("|Eemoji/key_ctrl|e + |Eemoji/key_lclick|e - Inventar")
				
				return
		else:
			if 50300 == itemVnum:
				if 0 != metinSlot:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILLBOOK_NAME, 1)
					self.ShowToolTip()
				return
			elif 70037 == itemVnum:
				if 0 != metinSlot:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME, 0)
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
				return
			elif 70055 == itemVnum:
				if 0 != metinSlot:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME, 0)
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
				return
		###########################################################################################


		itemDesc = item.GetItemDescription()
		itemSummary = item.GetItemSummary()

		isCostumeItem = 0
		isCostumeHair = 0
		isCostumeBody = 0
		isCostumeSkinSash = 0
		isCostumePet = 0
		if app.ENABLE_MOUNT_COSTUME_SYSTEM:
			isCostumeMount = 0
		if app.ENABLE_SASH_SYSTEM:
			isCostumeSash = 0
		if app.ENABLE_WEAPON_COSTUME_SYSTEM:
			isCostumeWeapon = 0
		if app.ENABLE_AURA_SYSTEM:
			isCostumeAura = 0

		if app.ENABLE_COSTUME_SYSTEM:
			if item.ITEM_TYPE_COSTUME == itemType:
				isCostumeItem = 1
				isCostumeHair = item.COSTUME_TYPE_HAIR == itemSubType
				isCostumeBody = item.COSTUME_TYPE_BODY == itemSubType
				if app.ENABLE_MOUNT_COSTUME_SYSTEM:
					isCostumeMount = item.COSTUME_TYPE_MOUNT == itemSubType
				
				if app.ENABLE_MOUNT_SKIN and isCostumeMount == False:
					isCostumeMount = item.COSTUME_TYPE_MOUNT_SKIN == itemSubType

				isCostumePet = item.COSTUME_TYPE_PET == itemSubType
				if app.ENABLE_SASH_SYSTEM:
					isCostumeSash = itemSubType == item.COSTUME_TYPE_SASH
				if app.ENABLE_WEAPON_COSTUME_SYSTEM:
					isCostumeWeapon = item.COSTUME_TYPE_WEAPON == itemSubType
				isCostumeSkinSash = itemSubType == item.COSTUME_TYPE_SKIN_SASH

				if app.ENABLE_AURA_SYSTEM:
					isCostumeAura = item.COSTUME_TYPE_AURA == itemSubType

				#dbg.TraceError("IS_COSTUME_ITEM! body(%d) hair(%d)" % (isCostumeBody, isCostumeHair))

		self.__AdjustMaxWidth(attrSlot, itemDesc)
		if app.ENABLE_NEW_NAME_ITEM:
			self.__SetItemTitle(itemVnum, metinSlot, attrSlot, window_type, slotIndex, newname)
		else:
			self.__SetItemTitle(itemVnum, metinSlot, attrSlot)

		if player.IsGameMaster():
			self.AppendSpace(5)
			self.AppendTextLine("%d - %d/%d"%(itemVnum, itemType, itemSubType),self.SPECIAL_TITLE_COLOR)
			self.AppendSpace(5)

		#if self.show_render == True:
		#	self.interface.CloseRenderTooltip()
		#	self.interface.RenderClearDates()

		if self.show_render == False or self.IsCanSeeRender(itemType, itemSubType) == False:
			interface = constInfo.GetInterfaceInstance()
			if interface != None:
				interface.CloseRenderTooltip()
				interface.RenderClearDates()

		### Hair Preview Image ###
		if self.__IsHair(itemVnum):
			self.__AppendHairIcon(itemVnum)
		elif app.__BL_HYPERLINK_ITEM_ICON__ and isinstance(self, HyperlinkItemToolTip):
			self.__AppendItemIcon(itemVnum)
		### Description ###
		self.AppendDescription(itemDesc, 26)
		self.AppendDescription(itemSummary, 26, self.CONDITION_COLOR)

		if app.__BUFFI_SUPPORT__:
			if itemType == item.ITEM_TYPE_BUFFI:
				if item.BUFFI_SCROLL == itemSubType:
					self.__AdjustMaxWidth(attrSlot, localeInfo.BUFFI_TOOLTIP_INFO)
					self.AppendSpace(5)
					self.AppendTextLine("|Eitem/61030|e", 0xFFFF5C5C)
					self.AppendSpace(5)
					self.AppendDescription(localeInfo.BUFFI_TOOLTIP_INFO, 26)
					self.AppendTextLine(localeInfo.BUFFI_TOOLTIP_INFO2, 0xFFFF5C5C)
					self.AppendSpace(5)
					self.AppendTextLine("["+self.__GetAffectString(item.APPLY_INT, 206).replace("+", "")+"]", 0xFFBEB47D)
					for i in xrange(item.LIMIT_MAX_NUM):
						(limitType, limitValue) = item.GetLimit(i)
						if item.LIMIT_REAL_TIME == limitType:
							self.AppendMallItemLastTime(metinSlot[0])
							break

		if app.TOURNAMENT_PVP_SYSTEM:
			if itemVnum in (84201, 84202):

				self.DESCRIPTION_VNUMS = {
					84201: localeInfo.TOURNAMENT_ITEM_BOX_REWARD,
					84202: localeInfo.TOURNAMENT_ITEM_HEART,
				}

				self.AppendDescription(self.DESCRIPTION_VNUMS[itemVnum], None, self.HIGH_PRICE_COLOR)

		# if self.check_sigillo(itemVnum) or itemVnum == 55002:
			# if attrSlot[0][1] != 0:
				# self.AppendSpace(5)
				# self.AppendTextLine("Livello: "+str(metinSlot[1]), self.NORMAL_COLOR)
				# self.AppendTextLine("Hp: +"+pointop(str(attrSlot[0][1]))+"%", self.SPECIAL_POSITIVE_COLOR)
				# self.AppendTextLine("Dif: +"+pointop(str(attrSlot[1][1]))+"%", self.SPECIAL_POSITIVE_COLOR)
				# self.AppendTextLine("Mp: +"+pointop(str(attrSlot[2][1]))+"%", self.SPECIAL_POSITIVE_COLOR)
				# self.AppendSpace(5)
				# if itemVnum != 55002:
					# days = (int(attrSlot[3][1])/60)/24
					# hours = (int(attrSlot[3][1]) - (days*60*24)) / 60
					# mins = int(attrSlot[3][1]) - (days*60*24) - (hours*60)
					# self.AppendTextLine("Durata: %d giorni %d Ore %d Minuti" % (days, hours, mins), self.SPECIAL_POSITIVE_COLOR)

		if app.ENABLE_NEW_PET_SYSTEM:
			if item.ITEM_TYPE_PET == itemType:
				if itemSubType == item.PET_LEVELABLE:
					level = metinSlot[1]
					if level > 0:
						curPoint = metinSlot[2]
						if level >= 120:
							maxPoint = 2500000000
						else:
							maxPoint = constInfo.exp_table[level]
						curPoint = min(curPoint, maxPoint)
						curPoint = max(curPoint, 0)
						maxPoint = max(maxPoint, 0)

						self.AppendTextLine("Lv. %d"%level,self.SPECIAL_POSITIVE_COLOR)
						age_text = [localeInfo.PET_GUI_YOUNG, localeInfo.PET_GUI_WILD, localeInfo.PET_GUI_BRAVE, localeInfo.PET_GUI_HERO]
						self.AppendTextLine("%s : %s"%(localeInfo.PET_GUI_AGE,age_text[metinSlot[3]]),self.SPECIAL_POSITIVE_COLOR)
						self.AppendTextLine("%s : %.2f%%" % (localeInfo.TASKBAR_EXP, float(curPoint) / max(1, float(maxPoint)) * 100),self.SPECIAL_POSITIVE_COLOR)

						self.AppendSpace(5)

						#pet_bonus_types=[item.APPLY_MAX_HP,item.APPLY_ATTBONUS_MONSTER,item.APPLY_CRITICAL_PCT]
						#pet_bonus_value=[4000,20,10]
						for j in xrange(3):
							ptr = metinSlot[5+j]
							if ptr == 20:
								bonus_value = constInfo.pet_bonus_value[j]
							else:
								bonus_value = float(float(constInfo.pet_bonus_value[j])/constInfo.PET_BONUS_MAX_LEVEL) * ptr

							text = self.__GetAffectString(constInfo.pet_bonus_types[j],int(bonus_value))
							if text != None:
								text = text.replace(":","")
								index = text.find("+")
								if index == -1:
									index = text.find("%")
									if index == -1:
										index =0
								if not index  <= 0:
									new_text = text[:index]
									new_text += " (Lv %d  -  %s)"%(ptr,text[index:])
									self.AppendTextLine(new_text,self.TYRANIS_TOOLTIP_COLOR)
								else:
									new_text = text
									new_text += " (Lv %d  -  +%d)"%(ptr,int(bonus_value))
									self.AppendTextLine(new_text,self.TYRANIS_TOOLTIP_COLOR)

						self.AppendSpace(5)
						for j in xrange(len(attrSlot)):
							if attrSlot[j][0] == 0 or attrSlot[j][0] == 99:
								continue
							self.AppendTextLine(constInfo.pet_skill_data[attrSlot[j][0]][0]+"(Lv"+str(attrSlot[j][1])+")",self.SPECIAL_TITLE_COLOR)

						self.AppendSpace(5)
						self.AppendTextLine("|Eemoji/time_icon|e " + localeInfo.LEFT_TIME+ ": "+localeInfo.SecondToDHM(metinSlot[0]-app.GetGlobalTimeStamp()),self.SPECIAL_POSITIVE_COLOR)

						if self.show_render == True:
							item.SelectItem(itemVnum)
							if metinSlot[3] >= 3:
								self.interface.SetRenderDates(item.GetValue(1),5)
							else:
								self.interface.SetRenderDates(item.GetValue(0),5)

						## if have transmutation !!!!!!!!!!!!!!!!!
						#self.AppendSpace(5)
						#self.AppendTransmutationEx(window_type, slotIndex, transmutation) #if have transmutation...

		if item.ITEM_TYPE_SHINING == itemType:
			if self.show_render == True:
				if itemSubType == item.SHINING_WEAPON:
					self.interface.SetRenderDates(itemVnum,6)
				elif itemSubType == item.SHINING_ARMOR:
					self.interface.SetRenderDates(itemVnum,7)

			self.__AppendAffectInformation()
			if app.ENABLE_SPECIAL_COSTUME_ATTR:
				self.__AppendAttributeInformation(attrSlot, 0, itemVnum)
			else:
				self.__AppendAttributeInformation(attrSlot)

			bHasRealtimeFlag = 0
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)
				if item.LIMIT_REAL_TIME == limitType:
					bHasRealtimeFlag = 1
			if bHasRealtimeFlag == 1:
				self.AppendMallItemLastTime(metinSlot[0])

		elif item.ITEM_TYPE_RINGS == itemType:
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)
			if app.BL_67_ATTR:
				self.AppendAttribute6th7thPossibility(attrSlot)
			self.__AppendAccessoryMetinSlotInfo(metinSlot, constInfo.GET_ACCESSORY_MATERIAL_VNUM(itemVnum, itemType))

			self.AppendWearableInformation()

		elif item.ITEM_TYPE_SPECIAL_RING == itemType:
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)
			if app.BL_67_ATTR:
				self.AppendAttribute6th7thPossibility(attrSlot)
			self.__AppendAccessoryMetinSlotInfo(metinSlot, constInfo.GET_ACCESSORY_MATERIAL_VNUM(itemVnum, itemType))

			self.AppendWearableInformation()
			bHasRealtimeFlag = 0
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)
				if item.LIMIT_REAL_TIME == limitType:
					bHasRealtimeFlag = 1
			if bHasRealtimeFlag == 1:
				self.AppendMallItemLastTime(metinSlot[0])

		elif item.ITEM_TYPE_BOOSTER == itemType:
			self.__AppendLimitInformation()

			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)
			self.AppendSpace(5)
			time = metinSlot[0]

			if metinSlot[0]-app.GetGlobalTimeStamp() < 0:
				self.AppendTextLine("Expired",self.SPECIAL_POSITIVE_COLOR)
			else:
				self.AppendTextLine("|Eemoji/time_icon|e " + localeInfo.LEFT_TIME+ ": "+localeInfo.SecondToDHM(metinSlot[0]-app.GetGlobalTimeStamp()), self.SPECIAL_POSITIVE_COLOR)

		### Weapon ###
		elif item.ITEM_TYPE_WEAPON == itemType:
			self.__AppendLimitInformation()

			if self.show_render == True:
				arrow_render = False
				if itemSubType == item.WEAPON_ARROW:
					arrow_render = True

				if arrow_render == False:
					interface = constInfo.GetInterfaceInstance()
					if interface != None:
						interface.SetRenderDates(itemVnum,1)


			self.AppendSpace(5)

			if item.WEAPON_FAN == itemSubType:
				self.__AppendMagicAttackInfo()
				self.__AppendAttackPowerInfo()
			else:
				self.__AppendAttackPowerInfo()
				self.__AppendMagicAttackInfo()

			self.__AppendAffectInformation()

			if app.ENABLE_YOHARA_SYSTEM:
				self.__AppendDefaultItemApplyInformation(apply_random_list)
			self.__AppendAttributeInformation(attrSlot)

			if app.BL_67_ATTR:
				self.AppendAttribute6th7thPossibility(attrSlot)

			self.AppendWearableInformation()

			if itemSubType != item.WEAPON_QUIVER:
				self.__AppendMetinSlotInfo(metinSlot)
			else:
				bHasRealtimeFlag = 0
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)
					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1
					
				if bHasRealtimeFlag == 1:
					self.AppendMallItemLastTime(metinSlot[0])

			#self.__AppendMetinSlotInfo(metinSlot)

		### Armor ###
		elif item.ITEM_TYPE_ARMOR == itemType:
			self.__AppendLimitInformation()

			## ¹æ¾î·Â
			defGrade = item.GetValue(1)
			defBonus = item.GetValue(5)*2 ## ¹æ¾î·Â Ç¥½Ã Àß¸ø µÇ´Â ¹®Á¦¸¦ ¼öÁ¤
			if defGrade > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade+defBonus), self.GetChangeTextLineColor(defGrade))

			self.__AppendMagicDefenceInfo()
			self.__AppendAffectInformation()
			if app.ENABLE_YOHARA_SYSTEM:
				self.__AppendDefaultItemApplyInformation(apply_random_list)
			self.__AppendAttributeInformation(attrSlot)

			if app.BL_67_ATTR:
				self.AppendAttribute6th7thPossibility(attrSlot)

			self.AppendWearableInformation()
			if search == 0:
				if itemSubType in (item.ARMOR_WRIST, item.ARMOR_NECK, item.ARMOR_EAR):				
					self.__AppendAccessoryMetinSlotInfo(metinSlot, constInfo.GET_ACCESSORY_MATERIAL_VNUM(itemVnum, itemSubType))
				else:
					self.__AppendMetinSlotInfo(metinSlot)

			if self.show_render == True:
				if itemSubType == item.ARMOR_BODY:
					self.interface.SetRenderDates(itemVnum,2)


		### Ring Slot Item (Not UNIQUE) ###
		elif item.ITEM_TYPE_RING == itemType:
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)

		elif item.ITEM_TYPE_NEWRING == itemType:
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)

			bHasRealtimeFlag = 0
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)

				if item.LIMIT_REAL_TIME == limitType:
					bHasRealtimeFlag = 1

			if 1 == bHasRealtimeFlag:
				self.AppendMallItemLastTime(metinSlot[0])
			else:
				if 0 != metinSlot:
					time = metinSlot[player.METIN_SOCKET_MAX_NUM-1]

					if 1 == item.GetValue(2):
						self.AppendMallItemLastTime(time)


		### Belt Item ###
		elif item.ITEM_TYPE_BELT == itemType:
			
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			if app.ENABLE_YOHARA_SYSTEM:
				self.__AppendDefaultItemApplyInformation(apply_random_list)
			self.__AppendAttributeInformation(attrSlot)
			if search == 0:
				self.__AppendAccessoryMetinSlotInfo(metinSlot, constInfo.GET_BELT_MATERIAL_VNUM(itemVnum))

		## ÄÚ½ºÃõ ¾ÆÀÌÅÛ ##
		elif 0 != isCostumeItem:
			self.__AppendLimitInformation()

			if app.ENABLE_SASH_SYSTEM:
				if isCostumeSash:
					## ABSORPTION RATE
					absChance = int(metinSlot[sash.ABSORPTION_SOCKET])
					self.AppendTextLine(localeInfo.SASH_ABSORB_CHANCE % (absChance), self.CONDITION_COLOR)
					## END ABSOPRTION RATE
					if self.show_render == True:
						self.interface.SetRenderDates(itemVnum,4)

					itemAbsorbedVnum = int(metinSlot[sash.ABSORBED_SOCKET])
					if itemAbsorbedVnum:
						## ATTACK / DEFENCE
						item.SelectItem(itemAbsorbedVnum)
						if item.GetItemType() == item.ITEM_TYPE_WEAPON:
							if item.GetItemSubType() == item.WEAPON_FAN:
								self.__AppendMagicAttackInfo(metinSlot[sash.ABSORPTION_SOCKET])
								item.SelectItem(itemAbsorbedVnum)
								self.__AppendAttackPowerInfo(metinSlot[sash.ABSORPTION_SOCKET])
							else:
								self.__AppendAttackPowerInfo(metinSlot[sash.ABSORPTION_SOCKET])
								item.SelectItem(itemAbsorbedVnum)
								self.__AppendMagicAttackInfo(metinSlot[sash.ABSORPTION_SOCKET])
						elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
							defGrade = item.GetValue(1)
							defBonus = item.GetValue(5) * 2
							defGrade = self.CalcSashValue(defGrade, metinSlot[sash.ABSORPTION_SOCKET])
							defBonus = self.CalcSashValue(defBonus, metinSlot[sash.ABSORPTION_SOCKET])
							
							if defGrade > 0:
								self.AppendSpace(5)
								self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))
							
							item.SelectItem(itemAbsorbedVnum)
							self.__AppendMagicDefenceInfo(metinSlot[sash.ABSORPTION_SOCKET])
						## END ATTACK / DEFENCE
						
						## EFFECT
						item.SelectItem(itemAbsorbedVnum)
						for i in xrange(item.ITEM_APPLY_MAX_NUM):
							(affectType, affectValue) = item.GetAffect(i)
							affectValue = self.CalcSashValue(affectValue, metinSlot[sash.ABSORPTION_SOCKET])
							affectString = self.__GetAffectString(affectType, affectValue)
							if affectString and affectValue > 0:
								self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
							
							item.SelectItem(itemAbsorbedVnum)
						# END EFFECT
						
						item.SelectItem(itemVnum)

						## ATTR
						self.__AppendAttributeInformation(attrSlot, metinSlot[sash.ABSORPTION_SOCKET])
						# END ATTR

						if app.BL_67_ATTR:
							self.AppendAttribute6th7thPossibility(attrSlot)


						

					else:
						# ATTR
						self.__AppendAttributeInformation(attrSlot)
						if app.BL_67_ATTR:
							self.AppendAttribute6th7thPossibility(attrSlot)
						# END ATTR
				else:
					if app.ENABLE_AURA_SYSTEM and isCostumeAura:
						self.__AppendAffectInformationAura(window_type, slotIndex, metinSlot)
						self.__AppendAuraItemAffectInformation(itemVnum, window_type, slotIndex, metinSlot)
						self.__AppendAttributeInformationAura(window_type, slotIndex, attrSlot, metinSlot)
						self.__AppendAuraBoostMetinSlotInfo(itemVnum, window_type, slotIndex, metinSlot)
					else:
						# self.__AppendAffectInformation()
						# self.__AppendAttributeInformation(attrSlot)

						self.__AppendAffectInformation()

						if app.ENABLE_SPECIAL_COSTUME_ATTR:
							self.__AppendAttributeInformation(attrSlot, 0, itemVnum)
						else:
							self.__AppendAttributeInformation(attrSlot)

						if self.show_render == True:
							if isCostumeHair != 0:
								self.interface.SetRenderDates(itemVnum, 3 ,item.GetValue(3))

							if isCostumeBody != 0:
								self.interface.SetRenderDates(itemVnum,2)

							if isCostumeWeapon != 0:
								self.interface.SetRenderDates(itemVnum,1)

							if isCostumeSkinSash != 0:
								self.interface.SetRenderDates(itemVnum,4)

							if isCostumePet != 0 or isCostumeMount != 0:
								item.SelectItem(itemVnum)
								self.interface.SetRenderDates(item.GetValue(0),5)

			else:
				self.__AppendAffectInformation()
				self.__AppendAttributeInformation(attrSlot)
			
			if self.itemVnum in (45160, 45161, 45148, 45147, 40457, 40460, 40458, 40461, 40462, 40459, 40101, 40104, 40103, 40105, 40106, 41324, 41325, 41311, 41312, 86062, 49120, 49127, 87013, 49130, 49131, 49132, 49133, 49134, 49135, 40424, 40425, 40426, 40427, 40428, 40429, 49138, 49139, 45305, 45306, 49136, 49137, 34042, 34043, 49313, 41003, 41004, 11971, 11972, 11973, 11974, 49049, 49050, 49286, 49287, 49060, 49061, 49073, 49073, 49086, 49087, 49106, 49107, 49116, 49117, 49126, 49128, 49051, 49052, 49288, 49289, 49062, 49063, 49075, 49075, 49088, 49089, 49108, 49109, 49118, 49119, 49128, 49129, 49043, 49044, 49045, 49046, 49047, 49048, 49280, 49281, 49282, 49283, 49284, 49285, 49054, 49055, 49056, 49057, 49058, 49059, 49067, 49068, 49069, 49070, 49071, 49071, 49080, 49081, 49082, 49083, 49084, 49085, 49100, 49101, 49102, 49103, 49104, 49105, 49110, 49111, 49112, 49113, 49114, 49115, 49121, 49122, 49123, 49124, 49125, 86065, 86063, 86067, 86004, 86048, 86035, 86036, 86037, 71247, 71246, 71245, 71235, 71226, 71227, 71228, 71229, 71230): #costume bonus
				self.AppendTextLine("[Costum Bonus Set]", self.SPECIAL_POSITIVE_COLOR)
			self.AppendWearableInformation()
			bHasRealtimeFlag = 0
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)
				if item.LIMIT_REAL_TIME == limitType:
					bHasRealtimeFlag = 1
			
			if bHasRealtimeFlag == 1 and buff == 0:
				self.AppendMallItemLastTime(metinSlot[0])
		## Rod ##
		elif item.ITEM_TYPE_ROD == itemType:

			if 0 != metinSlot:
				curLevel = item.GetValue(0) / 10
				curEXP = metinSlot[0]
				maxEXP = item.GetValue(2)
				self.__AppendLimitInformation()
				self.__AppendRodInformation(curLevel, curEXP, maxEXP)

		## Pick ##
		elif item.ITEM_TYPE_PICK == itemType:

			if 0 != metinSlot:
				curLevel = item.GetValue(0) / 10
				curEXP = metinSlot[0]
				maxEXP = item.GetValue(2)
				self.__AppendLimitInformation()
				self.__AppendPickInformation(curLevel, curEXP, maxEXP)

		## Lottery ##
		elif item.ITEM_TYPE_LOTTERY == itemType:
			if 0 != metinSlot:

				ticketNumber = int(metinSlot[0])
				stepNumber = int(metinSlot[1])

				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_LOTTERY_STEP_NUMBER % (stepNumber), self.NORMAL_COLOR)
				self.AppendTextLine(localeInfo.TOOLTIP_LOTTO_NUMBER % (ticketNumber), self.NORMAL_COLOR);

		### Metin ###
		elif item.ITEM_TYPE_METIN == itemType:
			self.AppendMetinInformation()
			self.AppendMetinWearInformation()

		### Fish ###
		elif item.ITEM_TYPE_FISH == itemType:
			if 0 != metinSlot:
				self.__AppendFishInfo(metinSlot[0])

		## item.ITEM_TYPE_BLEND
		elif item.ITEM_TYPE_BLEND == itemType:
			self.__AppendLimitInformation()

			if metinSlot:
				affectType = metinSlot[0]
				affectValue = metinSlot[1]
				time = metinSlot[2]

				self.AppendSpace(5)
				affectText = self.__GetAffectString(affectType, affectValue)

				self.AppendTextLine(affectText, self.NORMAL_COLOR)

				if time > 0:
					minute = (time / 60)
					second = (time % 60)
					timeString = localeInfo.TOOLTIP_POTION_TIME

					if minute > 0:
						timeString += str(minute) + localeInfo.TOOLTIP_POTION_MIN
					if second > 0:
						timeString += " " + str(second) + localeInfo.TOOLTIP_POTION_SEC

					self.AppendTextLine(timeString)
				else:
					self.AppendTextLine(localeInfo.BLEND_POTION_NO_TIME)
			else:
				self.AppendTextLine(localeInfo.BLEND_POTION_NO_TIME)

		elif item.ITEM_TYPE_UNIQUE == itemType:
			if app.ENABLE_MULTI_FARM_BLOCK:
				if itemVnum>=55610 and itemVnum<=55615:
					self.AppendTextLine("Adding Time: (%s)"% localeInfo.SecondToDHM(item.GetValue(0)),self.SPECIAL_TITLE_COLOR)
					self.AppendTextLine("Increase Count: (+%d)" % item.GetValue(1), self.SPECIAL_TITLE_COLOR)
			if 0 != metinSlot:
				bHasRealtimeFlag = 0

				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1

				if buff == 0:
					if 1 == bHasRealtimeFlag:
						self.AppendMallItemLastTime(metinSlot[0])
					else:
						time = metinSlot[player.METIN_SOCKET_MAX_NUM-1]

						if 1 == item.GetValue(2): ## ½Ç½Ã°£ ÀÌ¿ë Flag / ÀåÂø ¾ÈÇØµµ ÁØ´Ù
							self.AppendMallItemLastTime(time)
						else:
							self.AppendUniqueItemLastTime(time)

			if 60302 == itemVnum or 59101 == itemVnum or 31330 == itemVnum or 31331 == itemVnum or 31332 == itemVnum or 31333 == itemVnum or 59102 == itemVnum or 60301 == itemVnum or 60300 == itemVnum or 71145 == itemVnum or 71146 == itemVnum or 71147 == itemVnum or 71169 == itemVnum or 40724 == itemVnum or 40725 == itemVnum or 40726 == itemVnum or 40727 == itemVnum or 71173 == itemVnum:
				self.__AppendAffectInformation()

		### Use ###
		elif item.ITEM_TYPE_USE == itemType:
			self.__AppendLimitInformation()

			if app.ENABLE_LOCK_ATTR:
				if itemVnum == 50348 or itemVnum == 50349:
					attrIndex = 0
					try:
						attrIndex = int(metinSlot[0])
					except:
						attrIndex = 0
					self.AppendSpace(5)
					if 0 != attrIndex:
						self.AppendTextLine(localeInfo.LOCK_BONUS %attrIndex, self.SPECIAL_TITLE_COLOR)
					else:
						self.AppendTextLine(localeInfo.LOCK_BONUS2, self.SPECIAL_TITLE_COLOR)

			if app.ENABLE_COPY_ATTR_ITEM:
				if itemVnum == 77927:
					hasAttr = False
					for attr in attrSlot:
						if attr[0] > 0:
							hasAttr = True
							break
					if hasAttr:
						itemNames = {
							item.ITEM_TYPE_WEAPON : ["Weapon", [localeInfo.COPY_ITEM3,localeInfo.COPY_ITEM4,localeInfo.COPY_ITEM5,localeInfo.COPY_ITEM6,localeInfo.COPY_ITEM7,localeInfo.COPY_ITEM8]],
							item.ITEM_TYPE_ARMOR : ["Armor", [localeInfo.COPY_ITEM9,localeInfo.COPY_ITEM10,localeInfo.COPY_ITEM11,localeInfo.COPY_ITEM12,localeInfo.COPY_ITEM13,localeInfo.COPY_ITEM14,localeInfo.COPY_ITEM15]],
							item.ITEM_TYPE_COSTUME : ["Costume", [localeInfo.COPY_ITEM16,localeInfo.COPY_ITEM17,localeInfo.COPY_ITEM18,localeInfo.COPY_ITEM19,localeInfo.COPY_ITEM20,localeInfo.COPY_ITEM21,localeInfo.COPY_ITEM22,localeInfo.COPY_ITEM23,localeInfo.COPY_ITEM24]],
						}

						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.COPY_ITEM)
						self.AppendSpace(5)
						self.__AppendAttributeInformation(attrSlot)
						self.AppendSpace(5)
						
						try:
							self.AppendTextLine("[{}:{}]".format(itemNames[int(metinSlot[0])][0], itemNames[int(metinSlot[0])][1][int(metinSlot[1])]))
							self.AppendSpace(5)
						except:
							pass
						self.AppendTextLine(localeInfo.COPY_ITEM1)
					else:
						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.COPY_ITEM2)

			if app.ENABLE_SPECIAL_COSTUME_ATTR:
				if itemVnum >= 53998 and itemVnum <= 54500:
					if item.GetValue(0) != 0:
						self.AppendSpace(5)
						affectString = self.__GetAffectString(item.GetValue(0), item.GetValue(1))
						if affectString:
							affectColor = self.__GetAttributeColor(0, item.GetValue(1))
							self.AppendTextLine(affectString, affectColor)

						if player.IsGameMaster():
							bonusValueFlag = item.GetValue(2)
							bonusFlag = ["Body","Hair","Weapon","MountSkin","SashSkin","Effect"]
							flagText = ""
							for j in xrange(len(bonusFlag)):
								if constInfo.IS_SET(bonusValueFlag, 1 << j+1) or bonusValueFlag == 0:
									flagText+=", "+bonusFlag[j]
							self.AppendSpace(5)
							self.AppendTextLine("Flags: ["+flagText[2:]+"]", self.SPECIAL_TITLE_COLOR)

			if app.USE_CRYSTAL_SYSTEM and itemSubType == item.USE_RECHARGE_CRYSTAL:
				leftSec = metinSlot[item.CRYSTAL_LEFT_TIME_SOCKET_SLOT] if metinSlot[item.CRYSTAL_LEFT_TIME_SOCKET_SLOT] > 0 else item.GetValue(item.CRYSTAL_ADD_TIME_SLOT_VALUE_SLOT)
				if leftSec > 0:
					self.AppendSpace(5)
					self.AppendDescription(localeInfo.TOOLTIP_CRYSTAL_ADDTIME_LEFT % (localeInfo.SecondToDHM(leftSec)), 26, self.CONDITION_COLOR)
			elif item.USE_POTION == itemSubType or item.USE_POTION_NODELAY == itemSubType:
				self.__AppendPotionInformation()
			elif item.USE_ABILITY_UP == itemSubType:
				self.__AppendAbilityPotionInformation()

			if (app.WJ_COMBAT_ZONE):
				if itemVnum in [50287, 50288, 50290]:
					if 0 != metinSlot:
						useCount = int(metinSlot[0])

						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.TOOLTIP_REST_USABLE_COUNT % ((3 - useCount)), self.CONDITION_COLOR)
			## ¿µ¼® °¨Áö±â
			if 27989 == itemVnum or 76006 == itemVnum:
				if 0 != metinSlot:
					useCount = int(metinSlot[0])

					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_REST_USABLE_COUNT % (6 - useCount), self.NORMAL_COLOR)

			## ÀÌº¥Æ® °¨Áö±â
			elif 50004 == itemVnum:
				if 0 != metinSlot:
					useCount = int(metinSlot[0])

					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_REST_USABLE_COUNT % (10 - useCount), self.NORMAL_COLOR)

			## ÀÚµ¿¹°¾à
			elif constInfo.IS_AUTO_POTION(itemVnum):
				if 0 != metinSlot:
					## 0: È°¼ºÈ­, 1: »ç¿ë·®, 2: ÃÑ·®
					isActivated = int(metinSlot[0])
					usedAmount = float(metinSlot[1])
					totalAmount = float(metinSlot[2])

					if 0 == totalAmount:
						totalAmount = 1

					self.AppendSpace(5)

					if 0 != isActivated:
						self.AppendTextLine("(%s)" % (localeInfo.TOOLTIP_AUTO_POTION_USING), self.SPECIAL_POSITIVE_COLOR)
						self.AppendSpace(5)

					try:
						n = (100.0 - ((usedAmount / totalAmount) * 100.0))
						self.AppendTextLine(localeInfo.TOOLTIP_AUTO_POTION_REST % float(n), self.POSITIVE_COLOR)
					except:
						pass

			## ±ÍÈ¯ ±â¾ïºÎ
			elif itemVnum in WARP_SCROLLS:
				if 0 != metinSlot:
					xPos = int(metinSlot[0])
					yPos = int(metinSlot[1])

					if xPos != 0 and yPos != 0:
						(mapName, xBase, yBase) = background.GlobalPositionToMapInfo(xPos, yPos)

						localeMapName=localeInfo.MINIMAP_ZONE_NAME_DICT.get(mapName, "")

						self.AppendSpace(5)

						if localeMapName!="":
							self.AppendTextLine(localeInfo.TOOLTIP_MEMORIZED_POSITION % (localeMapName, int(xPos-xBase)/100, int(yPos-yBase)/100), self.NORMAL_COLOR)
						else:
							self.AppendTextLine(localeInfo.TOOLTIP_MEMORIZED_POSITION_ERROR % (int(xPos)/100, int(yPos)/100), self.NORMAL_COLOR)
							dbg.TraceError("NOT_EXIST_IN_MINIMAP_ZONE_NAME_DICT: %s" % mapName)

			#####
			if item.USE_SPECIAL == itemSubType:
				if app.USE_DROP_AFFECT_BONUSES and item.IsDropAffectBonus(itemVnum):
					self.__AppendAffectInformation()
					self.AppendTextLine(localeInfo.TOOLTIP_APPLY_DOUBLE_DROP_FOR % (localeInfo.SecondToDHM(item.GetValue(item.DOUBLE_DROP_BONUSES_TIME_VALUE_SLOT))), self.NORMAL_COLOR)
				else:
					bHasRealtimeFlag = 0
					for i in xrange(item.LIMIT_MAX_NUM):
						(limitType, limitValue) = item.GetLimit(i)
						if item.LIMIT_REAL_TIME == limitType:
							bHasRealtimeFlag = 1

					if buff == 0:
						## ÀÖ´Ù¸é °ü·Ã Á¤º¸¸¦ Ç¥½ÃÇÔ. ex) ³²Àº ½Ã°£ : 6ÀÏ 6½Ã°£ 58ºÐ
						if 1 == bHasRealtimeFlag:
							self.AppendMallItemLastTime(metinSlot[0])
						else:
							# ... ÀÌ°Å... ¼­¹ö¿¡´Â ÀÌ·± ½Ã°£ Ã¼Å© ¾ÈµÇ¾î ÀÖ´Âµ¥...
							# ¿Ö ÀÌ·±°Ô ÀÖ´ÂÁö ¾ËÁö´Â ¸øÇÏ³ª ±×³É µÎÀÚ...
							if 0 != metinSlot:
								time = metinSlot[player.METIN_SOCKET_MAX_NUM-1]

								## ½Ç½Ã°£ ÀÌ¿ë Flag
								if 1 == item.GetValue(2):
									self.AppendMallItemLastTime(time)
			elif item.USE_TIME_CHARGE_PER == itemSubType:
				bHasRealtimeFlag = 0
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1
				if metinSlot[2]:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_PER(metinSlot[2]))
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_PER(item.GetValue(0)))

				## ÀÖ´Ù¸é °ü·Ã Á¤º¸¸¦ Ç¥½ÃÇÔ. ex) ³²Àº ½Ã°£ : 6ÀÏ 6½Ã°£ 58ºÐ
				if 1 == bHasRealtimeFlag:
					self.AppendMallItemLastTime(metinSlot[0])

			elif item.USE_TIME_CHARGE_FIX == itemSubType:
				bHasRealtimeFlag = 0
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1
				if metinSlot[2]:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_FIX(metinSlot[2]))
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_FIX(item.GetValue(0)))

				## ÀÖ´Ù¸é °ü·Ã Á¤º¸¸¦ Ç¥½ÃÇÔ. ex) ³²Àº ½Ã°£ : 6ÀÏ 6½Ã°£ 58ºÐ
				if 1 == bHasRealtimeFlag:
					self.AppendMallItemLastTime(metinSlot[0])
			elif app.USE_BOOST_POTIONS and itemSubType == item.USE_SPECIAL_BOOST:
				self.__AppendAffectInformation()

				if metinSlot and self.IsFromInventory:
					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_USE_SPECIAL_BOOST_STATUS_ON if metinSlot[item.BOOST_POTION_STATUS] != 0 else localeInfo.TOOLTIP_USE_SPECIAL_BOOST_STATUS_OFF)

				IsTimerBasedOnWear = 0

				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)
					if limitType == item.LIMIT_TIMER_BASED_ON_WEAR and limitValue != 0:
						IsTimerBasedOnWear = limitValue
						break

				if IsTimerBasedOnWear != 0:
					if metinSlot and metinSlot[0]:
						self.AppendTimerBasedOnWearLastTime(metinSlot)
					else:
						self.AppendMallItemLastTime(app.GetGlobalTimeStamp() + IsTimerBasedOnWear)
				else:
					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_USE_SPECIAL_BOOST_PERMANENT)
			elif app.USE_CRYSTAL_SYSTEM and itemSubType == item.USE_ADD_ATTR_CRYSTAL:
				if self.__AppendAffectInformation():
					bonusType = item.GetValue(item.CRYSTAL_BONUS_TYPE_VALUE_SLOT)

					if CRYSTAL_BONUS_DESC_TYPE.has_key(bonusType):
						self.AppendSpace(5)
						self.AppendDescription(CRYSTAL_BONUS_DESC_TYPE[bonusType], 26, self.CONDITION_COLOR)
		elif app.USE_CRYSTAL_SYSTEM and itemType == item.ITEM_TYPE_CRYSTAL:
			self.__AppendAttributeInformation(attrSlot)

			if metinSlot:
				bnsType = metinSlot[item.CRYSTAL_BONUS_TYPE_SOCKET_SLOT]

				if CRYSTAL_BONUS_TYPE.has_key(bnsType):
					self.AppendSpace(5)
					self.AppendDescription(CRYSTAL_BONUS_TYPE[bnsType], 26, self.CONDITION_COLOR)
				else:
					self.AppendSpace(5)
					self.AppendDescription(localeInfo.CRYSTAL_BONUS_TYPE_TOOLTIP, 26, self.CONDITION_COLOR)

				if self.IsFromInventory:
					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_CRYSTAL_STATUS_ON if metinSlot[item.CRYSTAL_STATUS_SOCKET_SLOT] != 0 else localeInfo.TOOLTIP_CRYSTAL_STATUS_OFF)

				IsTimerBasedOnWear = 0

				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)
					if limitType == item.LIMIT_TIMER_BASED_ON_WEAR and limitValue != 0:
						IsTimerBasedOnWear = limitValue
						break

				if IsTimerBasedOnWear != 0:
					if metinSlot:
						self.AppendTimerBasedOnWearLastTime(metinSlot, True)
					else:
						self.AppendMallItemLastTime(app.GetGlobalTimeStamp() + IsTimerBasedOnWear)
		elif item.ITEM_TYPE_QUEST == itemType:
			if itemVnum >= 53008 and itemVnum <= 53013 or itemVnum == 53001 or itemVnum == 53003 or itemVnum == 53017 or itemVnum == 53002 or itemVnum == 53007 or itemVnum == 53224 or itemVnum == 53225 or itemVnum == 53226 or itemVnum == 53227 or itemVnum == 53228 or itemVnum == 53229 or itemVnum == 53218 or itemVnum == 53219 or itemVnum == 53220 or itemVnum == 53221 or itemVnum == 53006 or itemVnum == 53030 or itemVnum == 53031 or itemVnum == 53032 or itemVnum == 53033 or itemVnum == 53034 or itemVnum == 53035 or itemVnum == 53036 or itemVnum == 53037 or itemVnum == 53038 or itemVnum == 53039 or itemVnum == 53040 or itemVnum == 53041 or itemVnum == 53042 or itemVnum == 53043 or itemVnum == 53044 or itemVnum == 53045 or itemVnum == 53046 or itemVnum == 53047 or itemVnum == 53048 or itemVnum == 53049 or itemVnum == 53050 or itemVnum == 53051 or itemVnum == 53052 or itemVnum == 53053 or itemVnum == 53054 or itemVnum == 53055 or itemVnum == 53056 or itemVnum == 53057 or itemVnum == 53058 or itemVnum == 53058 or itemVnum == 53060 or itemVnum == 53061 or itemVnum == 53062 or itemVnum == 53063 or itemVnum == 53064 or itemVnum == 53260 or itemVnum == 53261:
				self.__AppendAffectInformation()
				
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)
				if search == 0 or buff == 0:
					if item.LIMIT_REAL_TIME == limitType:
						self.AppendMallItemLastTime(metinSlot[0])

			if itemVnum >= 71124 and itemVnum <= 71128 or itemVnum >= 71131 and itemVnum <= 71134 or itemVnum >= 71171 and itemVnum <= 71172 or itemVnum >= 71176 and itemVnum <= 71177 or itemVnum >= 71166 and itemVnum <= 71192 or itemVnum >= 71193 or itemVnum >= 71114 or itemVnum >= 71115 or itemVnum >= 71116 or itemVnum >= 71117 or itemVnum >= 71222 or itemVnum >= 71223 or itemVnum >= 71224 or itemVnum >= 71225 or itemVnum >= 71226 or itemVnum >= 71227 or itemVnum >= 71228 or itemVnum >= 71229 or itemVnum >= 71231 or itemVnum >= 71232 or itemVnum >= 71233 or itemVnum >= 71234 or itemVnum >= 71235 or itemVnum >= 71236 or itemVnum >= 71242 or itemVnum >= 71243 or itemVnum >= 71244 or itemVnum >= 71245 or itemVnum >= 71246 or itemVnum >= 71247 or itemVnum >= 71248 or itemVnum >= 71249 or itemVnum >= 71250 or itemVnum >= 71251 or itemVnum >= 71252:
				self.__AppendAffectInformation()
			
		
		elif item.ITEM_TYPE_GIFTBOX == itemType and app.ENABLE_SHOW_CHEST_DROP:
			self.AppendChestDropInfo(itemVnum, player.GetItemCount(window_type, slotIndex))

		elif item.ITEM_TYPE_DS == itemType:
			self.AppendTextLine(self.__DragonSoulInfoString(itemVnum))
			self.__AppendAttributeInformation(attrSlot)
		else:
			self.__AppendLimitInformation()

		if (app.USE_CRYSTAL_SYSTEM and itemType == item.ITEM_TYPE_CRYSTAL) or\
			(app.USE_BOOST_POTIONS and itemType == item.ITEM_TYPE_USE and itemSubType == item.USE_SPECIAL_BOOST):
			pass
		else:
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)
				#dbg.TraceError("LimitType : %d, limitValue : %d" % (limitType, limitValue))

				if item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
					self.AppendRealTimeStartFirstUseLastTime(item, metinSlot, i)
					#dbg.TraceError("2) REAL_TIME_START_FIRST_USE flag On ")

				elif item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
					self.AppendTimerBasedOnWearLastTime(metinSlot)
					#dbg.TraceError("1) REAL_TIME flag On ")

		if (app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS) and self.isAffect:
			pass
		else:
			itemPrice = item.GetISellItemPrice()
			itemCount = player.GetItemCount(window_type, slotIndex)
			itemPriceCount = itemPrice * itemCount

			if window_open != 0:
				if app.ENABLE_SELL_ITEM:
					if self.IsSellItems(itemVnum, itemPrice):
						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.EMOJI_SELL_ITEMS, self.CAN_LEVEL_UP_COLOR)
						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.SELL_PRICE_INFO, self.SPECIAL_TITLE_COLOR)
						
						if itemCount > 1:
							self.AppendTextLine(localeInfo.SELL_PRICE_INFO_PCS % localeInfo.NumberToMoneyString(itemPrice), self.SPECIAL_TITLE_COLOR)
							self.AppendTextLine(localeInfo.SELL_PRICE_INFO_ALL % localeInfo.NumberToMoneyString(itemPriceCount), self.SPECIAL_TITLE_COLOR)
						else:
							self.AppendTextLine(localeInfo.NumberToMoneyString(itemPrice), self.SPECIAL_TITLE_COLOR)

		self.__AppendSealInformation(window_type, slotIndex)

		if (app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS) and self.isAffect:
			pass
		elif app.USE_OFFLINESHOP_SEARCH_V2 and self.isShopSearch:
			pass
		else:
			interface = constInfo.GetInterfaceInstance()
			if interface:
				if self.show_render == True:
					if interface.IsRenderTooltip():
						self.AppendSpace(5)
						self.AppendTextLine("|Eemoji/key_alt|e - Preview", self.POSITIVE_COLOR)
				# 1 = inventory
				if self.emoji_safebox == 1 and interface.SafeboxIsShow():
					self.AppendSpace(5)
					self.AppendTextLine("|Eemoji/key_ctrl|e + |Eemoji/key_lclick|e - Move item", self.POSITIVE_COLOR)
				# 2 = safebox
				if self.emoji_safebox == 2 and interface.SafeboxIsShow():
					self.AppendSpace(5)
					self.AppendTextLine("|Eemoji/key_ctrl|e + |Eemoji/key_lclick|e - Inventar", self.POSITIVE_COLOR)

		if (app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS) and self.isAffect:
			pass
		elif app.USE_OFFLINESHOP_SEARCH_V2 and self.isShopSearch:
			pass
		else:
			self.ShowSafeboxEmoji(0)

		#self.show_render = True
		if app.ENABLE_MINI_GAME_CATCH_KING:
			if self.itemVnum in [79603, 79604]:
				if 0 != metinSlot[0]:
					self.AppendMallItemLastTime(metinSlot[0])

		if (app.USE_CRYSTAL_SYSTEM or app.USE_BOOST_POTIONS) and self.isAffect:
			pass
		elif app.USE_OFFLINESHOP_SEARCH_V2 and self.isShopSearch:
			pass
		elif self.IsFromInventory:
			if window_type == player.INVENTORY or\
				window_type == player.UPGRADE_INVENTORY or\
				window_type == player.BOOK_INVENTORY or\
				window_type == player.STONE_INVENTORY or\
				window_type == player.CHANGE_INVENTORY or\
				window_type == player.COSTUME_INVENTORY or\
				window_type == player.CHEST_INVENTORY:

				if exchange.isTrading():
					self.AppendSpace(5)
					self.AppendTextLine("|Eemoji/key_shift|e + |Eemoji/key_rclick|e - "+localeInfo.ADD_EXCHANGE, self.POSITIVE_COLOR)

				self.AppendSpace(5)
				self.AppendTextLine("|Eemoji/key_alt|e + |Eemoji/key_rclick|e - "+localeInfo.SEARCH_ITEM, self.POSITIVE_COLOR)
				self.AppendSpace(5)
				self.AppendTextLine("|Eemoji/key_ctrl|e + |Eemoji/key_alt|e|e|e +|Eemoji/key_rclick|e - Wikipedia", self.POSITIVE_COLOR)

			if app.ENABLE_RENDER_TARGET:
				self.__AppendRenderTarget(itemVnum)

		if app.ENABLE_ACCE_ABSORBED_ITEM_PREVIEW:
			if itemType == item.ITEM_TYPE_COSTUME and item.COSTUME_TYPE_SASH == itemSubType and not self.is_pickable:
				# absorbed_item_vnum = metinSlot[1]
				absorbed_item_vnum = int(metinSlot[sash.ABSORBED_SOCKET]) # use on top if not this works
				if absorbed_item_vnum != 0:
					self.AppendSpace(5)
					self.SetAbsorbedItemPreview(absorbed_item_vnum, attrSlot)
					self.AppendTextLine(localeInfo.ITEM_TOOLTIP_PREVIEW_ABSORBED_ITEM, self.POSITIVE_COLOR)


		self.AppendAntiFlagInformation()
		self.ShowToolTip()

	def FuncListBlend(self):
		info = [
		#Vnum - IdBonus -ValuesBonus
		[51003,1,5,2],
		[51004,1,5,2],
		[51005,1,5,2],
		[51006,1,5,3],
		[51007,1,5,3],

		]

		return info

	def SearchShopKeyBind(self):
		self.AppendTextLine("|Eemoji/key_alt|e + |Eemoji/key_lclick|e - " + localeInfo.SEARCH_ITEM, self.POSITIVE_COLOR)

	if app.ENABLE_SHOW_CHEST_DROP:
		def AppendChestDropInfo(self, itemVnum, itemCount = 0):
			if itemCount <= 1:
				self.AppendSpace(5)
				self.AppendTextLine("|Eemoji/key_ctrl|e + |Eemoji/key_rclick|e - "+localeInfo.CHEST_TOOLTIP_TEXT, self.POSITIVE_COLOR)
				return
			self.AppendSpace(5)
			self.AppendTextLine("|Eemoji/key_ctrl|e + |Eemoji/key_rclick|e - "+localeInfo.CHEST_TOOLTIP_TEXT, self.POSITIVE_COLOR)
			self.AppendSpace(5)
			self.AppendTextLine("|Eemoji/key_ctrl|e + |Eemoji/key_z|e + |Eemoji/key_rclick|e - "+localeInfo.CHEST_TOOLTIP_TEXT_OPEN, self.POSITIVE_COLOR)

	def FuncBlendGet(self,vnum):
		list_get = self.FuncListBlend()

		for i in xrange(0,len(list_get)):
			if vnum == list_get[i][0]:
				return True

		return False

	def FuncBlendGetIDBonus(self,vnum):
		list_get = self.FuncListBlend()

		for i in xrange(0,len(list_get)):
			if vnum == list_get[i][1]:
				return list_get[i][1]

	def FuncBlendGetValueBonus(self,vnum):
		list_get = self.FuncListBlend()

		for i in xrange(0,len(list_get)):
			if vnum == list_get[i][2]:
				return list_get[i][2]

	def FuncBlendGetTime(self,vnum):
		list_get = self.FuncListBlend()

		for i in xrange(0,len(list_get)):
			if vnum == list_get[i][3]:
				return list_get[i][3]

	def AppendTextLineAbsorb(self, text, color = FONT_COLOR, centerAlign = True):
		return ToolTip.AppendTextLine(self, text, color, centerAlign)

	if app.ENABLE_AURA_SYSTEM:
		def SetAuraWindowItem(self, slotIndex):
			itemVnum = player.GetAuraItemID(slotIndex)
			if 0 == itemVnum: return

			self.ClearToolTip()
			if shop.IsOpen() and not shop.IsPrivateShop():
				self.AppendSellingPrice(item.GetISellItemPrice(item.SelectItem(itemVnum)))

			metinSlot = [player.GetAuraItemMetinSocket(slotIndex, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot = [player.GetAuraItemAttribute(slotIndex, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]

			self.AddItemData(itemVnum, metinSlot, attrSlot, player.AURA_REFINE, slotIndex)

		def __AppendAffectInformationAura(self, windowType, slotIndex, metinSlot):
			socketLevelValue = 0
			socketBoostValue = 0
			if windowType == player.INVENTORY:
				socketLevelValue = player.GetItemMetinSocket(windowType, slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
				if metinSlot[player.ITEM_SOCKET_AURA_CURRENT_LEVEL] != 0 and socketLevelValue == 0:
					socketLevelValue = metinSlot[player.ITEM_SOCKET_AURA_CURRENT_LEVEL]
				socketBoostValue = player.GetItemMetinSocket(windowType, slotIndex, player.ITEM_SOCKET_AURA_BOOST)
				if metinSlot[player.ITEM_SOCKET_AURA_BOOST] != 0 and socketBoostValue == 0:
					socketBoostValue = metinSlot[player.ITEM_SOCKET_AURA_BOOST]
			elif windowType == player.AURA_REFINE:
				socketLevelValue = player.GetAuraItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
				socketBoostValue = player.GetAuraItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_BOOST)
			elif windowType == player.SAFEBOX:
				socketLevelValue = safebox.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
				socketBoostValue = safebox.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_BOOST)

			if socketLevelValue == 0:
				return

			curLevel = (socketLevelValue / 100000) - 1000
			curStep = player.GetAuraRefineInfo(curLevel, player.AURA_REFINE_INFO_STEP)
			if curStep < item.AURA_GRADE_RADIANT:
				curExpPer = 100.0 * (socketLevelValue % 100000) / player.GetAuraRefineInfo(curLevel, player.AURA_REFINE_INFO_NEED_EXP)

			self.AppendTextLine(localeInfo.AURA_LEVEL_STEP % (curLevel, curStep), self.CONDITION_COLOR)
			if curStep < item.AURA_GRADE_RADIANT:
				self.AppendTextLine(localeInfo.AURA_TOOLTIP_EXP % (curExpPer), self.CONDITION_COLOR)

			boostPercent = 0
			if socketBoostValue != 0:
				curBoostIndex = socketBoostValue / 100000000
				boostItemVnum = curBoostIndex + item.AURA_BOOST_ITEM_VNUM_BASE
				if boostItemVnum:
					item.SelectItem(boostItemVnum)
					boostPercent = item.GetValue(player.ITEM_VALUE_AURA_BOOST_PERCENT)

			if boostPercent > 0:
				self.AppendTextLine(localeInfo.AURA_DRAIN_BOOST_PER % (1.0 * curLevel / 10, boostPercent), self.CONDITION_COLOR)
			else:
				self.AppendTextLine(localeInfo.AURA_DRAIN_PER % (1.0 * curLevel / 10), self.CONDITION_COLOR)

		def __AppendAuraItemAffectInformation(self, oriItemVnum, windowType, slotIndex, metinSlot):
			socketLevelValue = 0
			socketBoostValue = 0
			if windowType == player.INVENTORY:
				socketLevelValue = player.GetItemMetinSocket(windowType, slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
				if metinSlot[player.ITEM_SOCKET_AURA_CURRENT_LEVEL] != 0 and socketLevelValue == 0:
					socketLevelValue = metinSlot[player.ITEM_SOCKET_AURA_CURRENT_LEVEL]
				socketBoostValue = player.GetItemMetinSocket(windowType, slotIndex, player.ITEM_SOCKET_AURA_BOOST)
				if metinSlot[player.ITEM_SOCKET_AURA_BOOST] != 0 and socketBoostValue == 0:
					socketBoostValue = metinSlot[player.ITEM_SOCKET_AURA_BOOST]
			elif windowType == player.AURA_REFINE:
				socketLevelValue = player.GetAuraItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
				socketBoostValue = player.GetAuraItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_BOOST)
			elif windowType == player.SAFEBOX:
				socketLevelValue = safebox.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
				socketBoostValue = safebox.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_BOOST)

			if socketLevelValue == 0:
				return

			curLevel = (socketLevelValue / 100000) - 1000
			boostPercent = 0.0
			if socketBoostValue != 0:
				curBoostIndex = socketBoostValue / 100000000
				boostItemVnum = curBoostIndex + item.AURA_BOOST_ITEM_VNUM_BASE
				if boostItemVnum:
					item.SelectItem(boostItemVnum)
					boostPercent = item.GetValue(player.ITEM_VALUE_AURA_BOOST_PERCENT) / 100.0

			drainlate = curLevel / 10. / 100. + boostPercent

			socketInDrainItemVnum = 0
			if windowType == player.INVENTORY:
				socketInDrainItemVnum = player.GetItemMetinSocket(windowType, slotIndex, player.ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM)
				if not metinSlot[player.ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM] == 0 and socketInDrainItemVnum == 0:
					socketInDrainItemVnum = metinSlot[player.ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM]
			elif windowType == player.AURA_REFINE:
				socketInDrainItemVnum = player.GetAuraItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM)
			elif windowType == player.SAFEBOX:
				socketInDrainItemVnum = safebox.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM)

			if socketInDrainItemVnum == 0:
				return

			item.SelectItem(socketInDrainItemVnum)
			itemType = item.GetItemType()
			if itemType == item.ITEM_TYPE_ARMOR:
				defBonus = item.GetValue(5)*2
				if item.GetValue(1) >= 1:
					defGrade = max(((item.GetValue(1) + defBonus) * drainlate) , 1)
					if defGrade > 0:
						self.AppendSpace(5)
						self.AppendTextLineAbsorb(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade), self.GetChangeTextLineColor(defGrade))

			for i in xrange(item.ITEM_APPLY_MAX_NUM):
				(affectType, affectValue) = item.GetAffect(i)
				if affectValue > 0:
					affectValue = max((affectValue * drainlate), 1)
					affectString = self.__GetAffectString(affectType, affectValue)
					if affectString:
						self.AppendTextLineAbsorb(affectString, self.GetChangeTextLineColor(affectValue))

			item.SelectItem(oriItemVnum)

		def __AppendAttributeInformationAura(self, windowType, slotIndex, attrSlot, metinSlot):
			if 0 != attrSlot:
				socketLevelValue = 0
				socketBoostValue = 0
				if windowType == player.INVENTORY:
					socketLevelValue = player.GetItemMetinSocket(windowType, slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
					socketBoostValue = player.GetItemMetinSocket(windowType, slotIndex, player.ITEM_SOCKET_AURA_BOOST)
					if metinSlot[player.ITEM_SOCKET_AURA_CURRENT_LEVEL] != 0 and socketLevelValue == 0:
						socketLevelValue = metinSlot[player.ITEM_SOCKET_AURA_CURRENT_LEVEL]
					if metinSlot[player.ITEM_SOCKET_AURA_BOOST] != 0 and socketBoostValue == 0:
						socketBoostValue = metinSlot[player.ITEM_SOCKET_AURA_BOOST]
				elif windowType == player.AURA_REFINE:
					socketLevelValue = player.GetAuraItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
					socketBoostValue = player.GetAuraItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_BOOST)
				elif windowType == player.SAFEBOX:
					socketLevelValue = safebox.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_CURRENT_LEVEL)
					socketBoostValue = safebox.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_BOOST)

				if socketLevelValue == 0:
					return

				curLevel = (socketLevelValue / 100000) - 1000
				boostPercent = 0.0
				if socketBoostValue != 0:
					curBoostIndex = socketBoostValue / 100000000
					boostItemVnum = curBoostIndex + item.AURA_BOOST_ITEM_VNUM_BASE
					if boostItemVnum:
						item.SelectItem(boostItemVnum)
						boostPercent = item.GetValue(player.ITEM_VALUE_AURA_BOOST_PERCENT) / 100.0

				drainlate = curLevel / 10. / 100. + boostPercent

				for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
					type = attrSlot[i][0]
					value = attrSlot[i][1]
					if 0 >= value:
						continue

					value = max(((value) * drainlate) , 1)
					affectString = self.__GetAffectString(type, value)

					if affectString:
						affectColor = self.__GetAttributeColor(i, value)
						self.AppendTextLine(affectString, affectColor)

		def __AppendAuraBoostMetinSlotInfo(self, oriItemVnum, windowType, slotIndex, metinSlot):
			socketBoostValue = 0
			if windowType == player.INVENTORY:
				socketBoostValue = player.GetItemMetinSocket(windowType, slotIndex, player.ITEM_SOCKET_AURA_BOOST)
				if metinSlot[player.ITEM_SOCKET_AURA_BOOST] != 0 and socketBoostValue == 0:
					socketBoostValue = metinSlot[player.ITEM_SOCKET_AURA_BOOST]
			elif windowType == player.AURA_REFINE:
				socketBoostValue = player.GetAuraItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_BOOST)
			elif windowType == player.SAFEBOX:
				socketBoostValue = safebox.GetItemMetinSocket(slotIndex, player.ITEM_SOCKET_AURA_BOOST)

			if socketBoostValue == 0:
				return

			curBoostIndex = socketBoostValue / 100000000

			socketImageDict = {
				player.METIN_SOCKET_TYPE_SILVER : "d:/ymir work/ui/game/windows/metin_slot_silver.sub",
				player.METIN_SOCKET_TYPE_GOLD : "d:/ymir work/ui/game/windows/metin_slot_gold.sub",
			}
			socketType = player.METIN_SOCKET_TYPE_NONE
			if item.ITEM_AURA_BOOST_ERASER < curBoostIndex < item.ITEM_AURA_BOOST_ULTIMATE:
				socketType = player.METIN_SOCKET_TYPE_SILVER
			elif curBoostIndex == item.ITEM_AURA_BOOST_ULTIMATE:
				socketType = player.METIN_SOCKET_TYPE_GOLD

			if player.METIN_SOCKET_TYPE_NONE == socketType:
				return

			boostRemainTime = socketBoostValue % 100000000
			boostItemVnum = curBoostIndex + item.AURA_BOOST_ITEM_VNUM_BASE

			self.AppendSpace(5)

			slotImage = ui.ImageBox()
			slotImage.SetParent(self)
			slotImage.LoadImage(socketImageDict[socketType])
			slotImage.Show()
			self.childrenList.append(slotImage)

			## Name
			nameTextLine = ui.TextLine()
			nameTextLine.SetParent(self)
			nameTextLine.SetFontName(self.defFontName)
			nameTextLine.SetPackedFontColor(self.NORMAL_COLOR)
			nameTextLine.SetOutline()
			nameTextLine.SetFeather()
			nameTextLine.Show()
			self.childrenList.append(nameTextLine)

			if localeInfo.IsARABIC():
				slotImage.SetPosition(self.toolTipWidth - slotImage.GetWidth() - 9, self.toolTipHeight-1)
				nameTextLine.SetPosition(self.toolTipWidth - 50, self.toolTipHeight + 2)
			else:
				slotImage.SetPosition(9, self.toolTipHeight-1)
				nameTextLine.SetPosition(50, self.toolTipHeight + 2)

			boostItemImage = ui.ImageBox()
			boostItemImage.SetParent(self)
			boostItemImage.Show()
			self.childrenList.append(boostItemImage)

			if boostItemVnum:
				item.SelectItem(boostItemVnum)
				try:
					boostItemImage.LoadImage(item.GetIconImageFileName())
				except:
					dbg.TraceError("ItemToolTip.__AppendAuraBoostMetinSlotInfo() - Failed to find image file %d:%s" % (boostItemVnum, item.GetIconImageFileName()))

				nameTextLine.SetText(item.GetItemName())

				boostDrainTextLine = ui.TextLine()
				boostDrainTextLine.SetParent(self)
				boostDrainTextLine.SetFontName(self.defFontName)
				boostDrainTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
				boostDrainTextLine.SetOutline()
				boostDrainTextLine.SetFeather()
				boostDrainTextLine.SetText(localeInfo.AURA_BOOST_DRAIN_PER % (item.GetValue(player.ITEM_VALUE_AURA_BOOST_PERCENT)))
				boostDrainTextLine.Show()
				self.childrenList.append(boostDrainTextLine)

				if localeInfo.IsARABIC():
					boostItemImage.SetPosition(self.toolTipWidth - boostItemImage.GetWidth() - 10, self.toolTipHeight)
					boostDrainTextLine.SetPosition(self.toolTipWidth - 50, self.toolTipHeight + 16 + 2)
				else:
					boostItemImage.SetPosition(10, self.toolTipHeight)
					boostDrainTextLine.SetPosition(50, self.toolTipHeight + 16 + 2)

				if 1 == item.GetValue(player.ITEM_VALUE_AURA_BOOST_UNLIMITED):
					boostRemainTime = 0

				if 0 != boostRemainTime:
					timeText = (localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(boostRemainTime))

					timeTextLine = ui.TextLine()
					timeTextLine.SetParent(self)
					timeTextLine.SetFontName(self.defFontName)
					timeTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
					timeTextLine.SetPosition(50, self.toolTipHeight + 16 + 2 + 16 + 2)
					timeTextLine.SetOutline()
					timeTextLine.SetFeather()
					timeTextLine.Show()
					timeTextLine.SetText(timeText)
					self.childrenList.append(timeTextLine)
					self.toolTipHeight += 16 + 2

			self.toolTipHeight += 35
			self.ResizeToolTip()


	if app.ENABLE_SELL_ITEM:
		def IsSellItems(self, itemVnum, itemPrice):
			item.SelectItem(itemVnum)
			
			# if item.GetItemType() == item.ITEM_TYPE_WEAPON or item.GetItemType() == item.ITEM_TYPE_ARMOR:
				# return True
				
			if itemPrice > 1:
				return True
				
			return False

	def __DragonSoulInfoString (self, dwVnum):
		step = (dwVnum / 100) % 10
		refine = (dwVnum / 10) % 10
		if 0 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL1 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 1 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL2 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 2 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL3 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 3 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL4 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 4 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL5 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		else:
			return ""


	## Çì¾îÀÎ°¡?
	def __IsHair(self, itemVnum):
		return (self.__IsOldHair(itemVnum) or
			self.__IsNewHair(itemVnum) or
			self.__IsNewHair2(itemVnum) or
			self.__IsNewHair3(itemVnum) or
			self.__IsCostumeHair(itemVnum)
			)

	def __IsOldHair(self, itemVnum):
		return itemVnum > 73000 and itemVnum < 74000

	def __IsNewHair(self, itemVnum):
		return itemVnum > 74000 and itemVnum < 75000

	def __IsNewHair2(self, itemVnum):
		return itemVnum > 75000 and itemVnum < 76000

	def __IsNewHair3(self, itemVnum):
		return ((74012 < itemVnum and itemVnum < 74022) or
			(74262 < itemVnum and itemVnum < 74272) or
			(74512 < itemVnum and itemVnum < 74522) or
			(74544 < itemVnum and itemVnum < 74560) or
			(74762 < itemVnum and itemVnum < 74772) or
			(45000 < itemVnum and itemVnum < 47000))

	def __IsCostumeHair(self, itemVnum):
		return app.ENABLE_COSTUME_SYSTEM and self.__IsNewHair3(itemVnum - 100000)

	def __AppendSearchIcon(self, itemVnum):
		item.SelectItem(itemVnum)

		itemImage = ui.ImageBox()
		itemImage.SetParent(self)
		itemImage.Show()

		itemImage.LoadImage(item.GetIconImageFileName())

		itemImage.SetPosition(itemImage.GetWidth()/2+60, self.toolTipHeight)
		self.toolTipHeight += itemImage.GetHeight()
		self.childrenList.append(itemImage)
		self.ResizeToolTip()

	def __AppendHairIcon(self, itemVnum):
		if itemVnum >=46001 and itemVnum <= 46014:
			return

		imageFolder = ""
		if self.__IsOldHair(itemVnum):
			imageFolder = "d:/ymir work/item/quest/"+str(itemVnum)+".tga"
		elif self.__IsNewHair3(itemVnum):
			imageFolder = "icon/hair/%d.sub" % (itemVnum)
		elif self.__IsNewHair(itemVnum): # ±âÁ¸ Çì¾î ¹øÈ£¸¦ ¿¬°á½ÃÄÑ¼­ »ç¿ëÇÑ´Ù. »õ·Î¿î ¾ÆÀÌÅÛÀº 1000¸¸Å­ ¹øÈ£°¡ ´Ã¾ú´Ù.
			imageFolder = "d:/ymir work/item/quest/"+str(itemVnum-1000)+".tga"
		elif self.__IsNewHair2(itemVnum):
			imageFolder = "icon/hair/%d.sub" % (itemVnum)
		elif self.__IsCostumeHair(itemVnum):
			imageFolder = "icon/hair/%d.sub" % (itemVnum - 100000)

		if not pack.Exist(imageFolder) or imageFolder == "":
			return

		itemImage = ui.ImageBox()
		itemImage.SetParent(self)
		itemImage.LoadImage(imageFolder)
		itemImage.Show()

		itemImage.SetPosition(itemImage.GetWidth()/2, self.toolTipHeight)
		self.toolTipHeight += itemImage.GetHeight()
		#self.toolTipWidth += itemImage.GetWidth()/2
		self.childrenList.append(itemImage)
		self.ResizeToolTip()

	if app.__BL_HYPERLINK_ITEM_ICON__:
		def __AppendItemIcon(self, itemVnum):
			itemImage = ui.ImageBox()
			itemImage.SetParent(self)
			item.SelectItem(itemVnum)
			itemImage.LoadImage(item.GetIconImageFileName())
			itemImage.SetPosition(self.toolTipWidth / 2 - itemImage.GetWidth() / 2, self.toolTipHeight)
			itemImage.Show()

			self.toolTipHeight += itemImage.GetHeight()
			self.childrenList.append(itemImage)
			self.ResizeToolTip()
	## »çÀÌÁî°¡ Å« Description ÀÏ °æ¿ì ÅøÆÁ »çÀÌÁî¸¦ Á¶Á¤ÇÑ´Ù
	def __AdjustMaxWidth(self, attrSlot, desc):
		newToolTipWidth = self.toolTipWidth
		newToolTipWidth = max(self.__AdjustAttrMaxWidth(attrSlot), newToolTipWidth)
		newToolTipWidth = max(self.__AdjustDescMaxWidth(desc), newToolTipWidth)
		if newToolTipWidth > self.toolTipWidth:
			self.toolTipWidth = newToolTipWidth
			self.ResizeToolTip()

	def __AdjustAttrMaxWidth(self, attrSlot):
		if 0 == attrSlot:
			return self.toolTipWidth

		maxWidth = self.toolTipWidth
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			type = attrSlot[i][0]
			value = attrSlot[i][1]
			if self.ATTRIBUTE_NEED_WIDTH.has_key(type):
				if value > 0:
					maxWidth = max(self.ATTRIBUTE_NEED_WIDTH[type], maxWidth)

					# ATTR_CHANGE_TOOLTIP_WIDTH
					#self.toolTipWidth = max(self.ATTRIBUTE_NEED_WIDTH[type], self.toolTipWidth)
					#self.ResizeToolTip()
					# END_OF_ATTR_CHANGE_TOOLTIP_WIDTH

		return maxWidth

	def __AdjustDescMaxWidth(self, desc):
		if len(desc) < DESC_DEFAULT_MAX_COLS:
			return self.toolTipWidth

		return DESC_WESTERN_MAX_WIDTH

	def __SetSkillBookToolTip(self, skillIndex, bookName, skillGrade):
		skillName = skill.GetSkillName(skillIndex)

		if not skillName:
			return

		if localeInfo.IsVIETNAM():
			itemName = bookName + " " + skillName
		else:
			itemName = skillName + " " + bookName
		self.SetTitle(itemName)

	def __AppendPickInformation(self, curLevel, curEXP, maxEXP):
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_PICK_LEVEL % (curLevel), self.NORMAL_COLOR)
		self.AppendTextLine(localeInfo.TOOLTIP_PICK_EXP % (curEXP, maxEXP), self.NORMAL_COLOR)

		if curEXP == maxEXP:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_PICK_UPGRADE1, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_PICK_UPGRADE2, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_PICK_UPGRADE3, self.NORMAL_COLOR)


	def __AppendRodInformation(self, curLevel, curEXP, maxEXP):
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_LEVEL % (curLevel), self.NORMAL_COLOR)
		self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_EXP % (curEXP, maxEXP), self.NORMAL_COLOR)

		if curEXP == maxEXP:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_UPGRADE1, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_UPGRADE2, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_UPGRADE3, self.NORMAL_COLOR)

	def __AppendLimitInformation(self):
		appendSpace = False

		for i in xrange(item.LIMIT_MAX_NUM):
			(limitType, limitValue) = item.GetLimit(i)

			if limitValue > 0:
				if appendSpace == False:
					self.AppendSpace(5)
					appendSpace = True
			else:
				continue

			if app.ENABLE_YOHARA_SYSTEM and limitType == item.LIMIT_CONQUEROR_LEVEL:
				color = self.GetLimitTextLineColor(player.GetStatus(player.CONQUEROR_LEVEL), limitValue, True)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_CONQUEROR_LEVEL % (limitValue), color)
				break

			if item.LIMIT_LEVEL == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.LEVEL), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_LEVEL % (limitValue), color)

	## cyh itemseal 2013 11 11
	def __AppendSealInformation(self, window_type, slotIndex):
		if not app.ENABLE_SOULBIND_SYSTEM:
			return

		itemSealDate = player.GetItemSealDate(window_type, slotIndex)
		if itemSealDate == item.GetDefaultSealDate():
			return

		if itemSealDate == item.GetUnlimitedSealDate():
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_SEALED, self.NEGATIVE_COLOR)

		elif itemSealDate > 0:
			self.AppendSpace(5)
			hours, minutes = player.GetItemUnSealLeftTime(window_type, slotIndex)
			self.AppendTextLine(localeInfo.TOOLTIP_UNSEAL_LEFT_TIME % (hours, minutes), self.NEGATIVE_COLOR)

	def __GetAffectString(self, affectType, affectValue):
		if 0 == affectType:
			return None

		if 0 == affectValue:
			return None

		if affectType == item.APPLY_RANDOM:
			return None

		try:
			return self.AFFECT_DICT[affectType](affectValue)
		except TypeError:
			return "UNKNOWN_VALUE[%s] %s" % (affectType, affectValue)
		except KeyError:
			return "UNKNOWN_TYPE[%s] %s" % (affectType, affectValue)


	def __AppendAffectInformation(self):
		#if app.USE_CRYSTAL_SYSTEM:
		bRet = False

		for i in xrange(item.ITEM_APPLY_MAX_NUM):
			(affectType, affectValue) = item.GetAffect(i)
			affectString = self.__GetAffectString(affectType, affectValue)
			if affectString:
				self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))

				if not bRet:
					bRet = True

		return bRet

	def AppendWearableInformation(self):

		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_ITEM_WEARABLE_JOB, self.NORMAL_COLOR)

		flagList = (
			not item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR),
			not item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN),
			not item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA),
			not item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN))
		if app.ENABLE_WOLFMAN_CHARACTER:
			flagList += (not item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN),)
		characterNames = ""
		for i in xrange(self.CHARACTER_COUNT):

			name = self.CHARACTER_NAMES[i]
			flag = flagList[i]

			if flag:
				characterNames += " "
				characterNames += name

		textLine = self.AppendTextLine(characterNames, self.NORMAL_COLOR, True)
		textLine.SetFeather()

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
			textLine = self.AppendTextLine(localeInfo.FOR_FEMALE, self.NORMAL_COLOR, True)
			textLine.SetFeather()

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
			textLine = self.AppendTextLine(localeInfo.FOR_MALE, self.NORMAL_COLOR, True)
			textLine.SetFeather()

	def AppendAntiFlagInformation(self):
		antiFlagDict = {
			localeInfo.TOOLTIP_ANTIDROP: item.IsAntiFlag(item.ITEM_ANTIFLAG_DROP),
			localeInfo.TOOLTIP_ANTISELL: item.IsAntiFlag(item.ITEM_ANTIFLAG_SELL),
			localeInfo.TOOLTIP_ANTISHOP: item.IsAntiFlag(item.ITEM_ANTIFLAG_MYSHOP),
			localeInfo.TOOLTIP_ANTISAFEBOX: item.IsAntiFlag(item.ITEM_ANTIFLAG_SAFEBOX),
		}
		
		antiFlagNames = [name for name, flag in antiFlagDict.iteritems() if flag]
		if antiFlagNames:
			self.AppendSpace(5)
			textLine = self.AppendTextLine('{} {}'.format(', '.join(antiFlagNames), ""), self.NORMAL_COLOR)
			textLine.SetFeather()

	def __AppendPotionInformation(self):
		self.AppendSpace(5)

		healHP = item.GetValue(0)
		healSP = item.GetValue(1)
		healStatus = item.GetValue(2)
		healPercentageHP = item.GetValue(3)
		healPercentageSP = item.GetValue(4)

		if healHP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_HP_POINT % healHP, self.GetChangeTextLineColor(healHP))
		if healSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_SP_POINT % healSP, self.GetChangeTextLineColor(healSP))
		if healStatus != 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_CURE)
		if healPercentageHP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_HP_PERCENT % healPercentageHP, self.GetChangeTextLineColor(healPercentageHP))
		if healPercentageSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_SP_PERCENT % healPercentageSP, self.GetChangeTextLineColor(healPercentageSP))

	def __AppendAbilityPotionInformation(self):

		self.AppendSpace(5)

		abilityType = item.GetValue(0)
		time = item.GetValue(1)
		point = item.GetValue(2)

		if abilityType == item.APPLY_ATT_SPEED:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_ATTACK_SPEED % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_MOV_SPEED:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_MOVING_SPEED % point, self.GetChangeTextLineColor(point))

		if time > 0:
			minute = (time / 60)
			second = (time % 60)
			timeString = localeInfo.TOOLTIP_POTION_TIME

			if minute > 0:
				timeString += str(minute) + localeInfo.TOOLTIP_POTION_MIN
			if second > 0:
				timeString += " " + str(second) + localeInfo.TOOLTIP_POTION_SEC

			self.AppendTextLine(timeString)

	def GetPriceColor(self, price):
		if price>=constInfo.HIGH_PRICE:
			return self.HIGH_PRICE_COLOR
		if price>=constInfo.MIDDLE_PRICE:
			return self.MIDDLE_PRICE_COLOR
		else:
			return self.LOW_PRICE_COLOR

	def AppendPrice(self, price):
		if not app.ENABLE_BUY_WITH_ITEM:
			self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_BUYPRICE  % (localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))

	def AppendPriceBySecondaryCoin(self, price):
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_BUYPRICE  % (localeInfo.NumberToSecondaryCoinString(price)), self.GetPriceColor(price))

	def AppendSellingPrice(self, price):
		if item.IsAntiFlag(item.ITEM_ANTIFLAG_SELL):
			self.AppendTextLine(localeInfo.TOOLTIP_ANTI_SELL, self.DISABLE_COLOR)
			self.AppendSpace(5)
		else:
			self.AppendTextLine(localeInfo.TOOLTIP_SELLPRICE % (localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))
			self.AppendSpace(5)

	def AppendMetinInformation(self):
		if constInfo.ENABLE_FULLSTONE_DETAILS:
			for i in xrange(item.ITEM_APPLY_MAX_NUM):
				(affectType, affectValue) = item.GetAffect(i)
				affectString = self.__GetAffectString(affectType, affectValue)
				if affectString:
					self.AppendSpace(5)
					self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))

	def AppendMetinWearInformation(self):

		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_SOCKET_REFINABLE_ITEM, self.NORMAL_COLOR)

		flagList = (item.IsWearableFlag(item.WEARABLE_BODY),
					item.IsWearableFlag(item.WEARABLE_HEAD),
					item.IsWearableFlag(item.WEARABLE_FOOTS),
					item.IsWearableFlag(item.WEARABLE_WRIST),
					item.IsWearableFlag(item.WEARABLE_WEAPON),
					item.IsWearableFlag(item.WEARABLE_NECK),
					item.IsWearableFlag(item.WEARABLE_EAR),
					item.IsWearableFlag(item.WEARABLE_UNIQUE),
					item.IsWearableFlag(item.WEARABLE_SHIELD),
					item.IsWearableFlag(item.WEARABLE_ARROW))

		wearNames = ""
		for i in xrange(self.WEAR_COUNT):

			name = self.WEAR_NAMES[i]
			flag = flagList[i]

			if flag:
				wearNames += "  "
				wearNames += name

		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
		textLine.SetHorizontalAlignCenter()
		textLine.SetPackedFontColor(self.NORMAL_COLOR)
		textLine.SetText(wearNames)
		textLine.Show()
		self.childrenList.append(textLine)

		self.toolTipHeight += self.TEXT_LINE_HEIGHT
		self.ResizeToolTip()

	def GetMetinSocketType(self, number):
		if player.METIN_SOCKET_TYPE_NONE == number:
			return player.METIN_SOCKET_TYPE_NONE
		elif player.METIN_SOCKET_TYPE_SILVER == number:
			return player.METIN_SOCKET_TYPE_SILVER
		elif player.METIN_SOCKET_TYPE_GOLD == number:
			return player.METIN_SOCKET_TYPE_GOLD
		else:
			item.SelectItem(number)
			if item.METIN_NORMAL == item.GetItemSubType():
				return player.METIN_SOCKET_TYPE_SILVER
			elif item.METIN_GOLD == item.GetItemSubType():
				return player.METIN_SOCKET_TYPE_GOLD
			elif "USE_PUT_INTO_ACCESSORY_SOCKET" == item.GetUseType(number):
				return player.METIN_SOCKET_TYPE_SILVER
			elif "USE_PUT_INTO_RING_SOCKET" == item.GetUseType(number):
				return player.METIN_SOCKET_TYPE_SILVER
			elif "USE_PUT_INTO_BELT_SOCKET" == item.GetUseType(number):
				return player.METIN_SOCKET_TYPE_SILVER

		return player.METIN_SOCKET_TYPE_NONE

	def GetMetinItemIndex(self, number):
		if player.METIN_SOCKET_TYPE_SILVER == number:
			return 0
		if player.METIN_SOCKET_TYPE_GOLD == number:
			return 0

		return number

	def __AppendAccessoryMetinSlotInfo(self, metinSlot, mtrlVnum):
		ACCESSORY_SOCKET_MAX_SIZE = 3

		cur=min(metinSlot[0], ACCESSORY_SOCKET_MAX_SIZE)
		end=min(metinSlot[1], ACCESSORY_SOCKET_MAX_SIZE)

		affectType1, affectValue1 = item.GetAffect(0)
		affectList1=[0, max(1, affectValue1*10/100), max(2, affectValue1*20/100), max(3, affectValue1*40/100)]

		affectType2, affectValue2 = item.GetAffect(1)
		affectList2=[0, max(1, affectValue2*10/100), max(2, affectValue2*20/100), max(3, affectValue2*40/100)]

		affectType3, affectValue3 = item.GetAffect(2)
		affectList3=[0, max(1, affectValue3*10/100), max(2, affectValue3*20/100), max(3, affectValue3*40/100)]

		mtrlPos=0
		mtrlList=[mtrlVnum]*cur+[player.METIN_SOCKET_TYPE_SILVER]*(end-cur)
		for mtrl in mtrlList:
			affectString1 = self.__GetAffectString(affectType1, affectList1[mtrlPos+1]-affectList1[mtrlPos])
			affectString2 = self.__GetAffectString(affectType2, affectList2[mtrlPos+1]-affectList2[mtrlPos])
			affectString3 = self.__GetAffectString(affectType3, affectList3[mtrlPos+1]-affectList3[mtrlPos])

			leftTime = 0
			if cur == mtrlPos+1:
				leftTime=metinSlot[2]

			self.__AppendMetinSlotInfo_AppendMetinSocketData(mtrlPos, mtrl, affectString1, affectString2, affectString3, leftTime)
			mtrlPos+=1

	def __AppendMetinSlotInfo(self, metinSlot):
		if self.__AppendMetinSlotInfo_IsEmptySlotList(metinSlot):
			return

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			self.__AppendMetinSlotInfo_AppendMetinSocketData(i, metinSlot[i])

	def __AppendMetinSlotInfo_IsEmptySlotList(self, metinSlot):
		if 0 == metinSlot:
			return 1

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlotData=metinSlot[i]
			if 0 != self.GetMetinSocketType(metinSlotData):
				if 0 != self.GetMetinItemIndex(metinSlotData):
					return 0
		return 1

	def __AppendMetinSlotInfo_AppendMetinSocketData(self, index, metinSlotData, custumAffectString="", custumAffectString2="", custumAffectString3="", leftTime=0):

		slotType = self.GetMetinSocketType(metinSlotData)
		itemIndex = self.GetMetinItemIndex(metinSlotData)

		if 0 == slotType:
			return

		self.AppendSpace(5)

		slotImage = ui.ImageBox()
		slotImage.SetParent(self)
		slotImage.Show()

		## Name
		nameTextLine = ui.TextLine()
		nameTextLine.SetParent(self)
		nameTextLine.SetFontName(self.defFontName)
		nameTextLine.SetPackedFontColor(self.NORMAL_COLOR)
		nameTextLine.SetOutline()
		nameTextLine.SetFeather()
		nameTextLine.Show()

		self.childrenList.append(nameTextLine)

		if player.METIN_SOCKET_TYPE_SILVER == slotType:
			slotImage.LoadImage("d:/ymir work/ui/game/windows/metin_slot_silver.sub")
		elif player.METIN_SOCKET_TYPE_GOLD == slotType:
			slotImage.LoadImage("d:/ymir work/ui/game/windows/metin_slot_gold.sub")

		self.childrenList.append(slotImage)

		if localeInfo.IsARABIC():
			slotImage.SetPosition(self.toolTipWidth - slotImage.GetWidth() - 9, self.toolTipHeight-1)
			nameTextLine.SetPosition(self.toolTipWidth - 50, self.toolTipHeight + 2)
		else:
			slotImage.SetPosition(9, self.toolTipHeight-1)
			nameTextLine.SetPosition(50, self.toolTipHeight + 2)

		metinImage = ui.ImageBox()
		metinImage.SetParent(self)
		metinImage.Show()
		self.childrenList.append(metinImage)

		if itemIndex:

			item.SelectItem(itemIndex)

			## Image
			try:
				metinImage.LoadImage(item.GetIconImageFileName())
			except:
				dbg.TraceError("ItemToolTip.__AppendMetinSocketData() - Failed to find image file %d:%s" %
					(itemIndex, item.GetIconImageFileName())
				)

			nameTextLine.SetText(item.GetItemName())

			## Affect
			affectTextLine = ui.TextLine()
			affectTextLine.SetParent(self)
			affectTextLine.SetFontName(self.defFontName)
			affectTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
			affectTextLine.SetOutline()
			affectTextLine.SetFeather()
			affectTextLine.Show()

			if localeInfo.IsARABIC():
				metinImage.SetPosition(self.toolTipWidth - metinImage.GetWidth() - 10, self.toolTipHeight)
				affectTextLine.SetPosition(self.toolTipWidth - 50, self.toolTipHeight + 16 + 2)
			else:
				metinImage.SetPosition(10, self.toolTipHeight)
				affectTextLine.SetPosition(50, self.toolTipHeight + 16 + 2)

			if custumAffectString:
				affectTextLine.SetText(custumAffectString)
			elif itemIndex!=constInfo.ERROR_METIN_STONE:
				affectType, affectValue = item.GetAffect(0)
				affectString = self.__GetAffectString(affectType, affectValue)
				if affectString:
					affectTextLine.SetText(affectString)
			else:
				affectTextLine.SetText(localeInfo.TOOLTIP_APPLY_NOAFFECT)

			self.childrenList.append(affectTextLine)

			if constInfo.ENABLE_FULLSTONE_DETAILS and (not custumAffectString2) and (itemIndex!=constInfo.ERROR_METIN_STONE):
				custumAffectString2 = self.__GetAffectString(*item.GetAffect(1))

			if custumAffectString2:
				affectTextLine = ui.TextLine()
				affectTextLine.SetParent(self)
				affectTextLine.SetFontName(self.defFontName)
				affectTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
				affectTextLine.SetPosition(50, self.toolTipHeight + 16 + 2 + 16 + 2)
				affectTextLine.SetOutline()
				affectTextLine.SetFeather()
				affectTextLine.Show()
				affectTextLine.SetText(custumAffectString2)
				self.childrenList.append(affectTextLine)
				self.toolTipHeight += 16 + 2

			if constInfo.ENABLE_FULLSTONE_DETAILS and (not custumAffectString3) and (itemIndex!=constInfo.ERROR_METIN_STONE):
				custumAffectString3 = self.__GetAffectString(*item.GetAffect(2))

			if custumAffectString3:
				affectTextLine = ui.TextLine()
				affectTextLine.SetParent(self)
				affectTextLine.SetFontName(self.defFontName)
				affectTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
				affectTextLine.SetPosition(50, self.toolTipHeight + 16 + 2 + 16 + 2)
				affectTextLine.SetOutline()
				affectTextLine.SetFeather()
				affectTextLine.Show()
				affectTextLine.SetText(custumAffectString3)
				self.childrenList.append(affectTextLine)
				self.toolTipHeight += 16 + 2

			if 0 != leftTime:
				timeText = ("|Eemoji/time_icon|e " + localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(leftTime))

				timeTextLine = ui.TextLine()
				timeTextLine.SetParent(self)
				timeTextLine.SetFontName(self.defFontName)
				timeTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
				timeTextLine.SetPosition(50, self.toolTipHeight + 16 + 2 + 16 + 2)
				timeTextLine.SetOutline()
				timeTextLine.SetFeather()
				timeTextLine.Show()
				timeTextLine.SetText(timeText)
				self.childrenList.append(timeTextLine)
				self.toolTipHeight += 16 + 2

		else:
			nameTextLine.SetText(localeInfo.TOOLTIP_SOCKET_EMPTY)

		self.toolTipHeight += 35
		self.ResizeToolTip()

	def __AppendFishInfo(self, size):
		try:
			size = int(size)
			if size > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_FISH_LEN % (float(size) / 100.0), self.NORMAL_COLOR)
		except:
			pass

	def AppendUniqueItemLastTime(self, restMin):
		restSecond = restMin*60
		self.AppendSpace(5)
		self.AppendTextLine("|Eemoji/time_icon|e " + localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(restSecond), self.NORMAL_COLOR)

	def AppendMallItemLastTime(self, endTime):
		leftSec = max(0, endTime - app.GetGlobalTimeStamp())
		self.AppendSpace(5)
		self.AppendTextLine("|Eemoji/time_icon|e " + localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(leftSec), self.NORMAL_COLOR)

	# if app.USE_CRYSTAL_SYSTEM: @isCrystal
	def AppendTimerBasedOnWearLastTime(self, metinSlot, isCrystal = False):
		if app.USE_CRYSTAL_SYSTEM and isCrystal == True:
			endTime = app.GetGlobalTimeStamp() + metinSlot[0]
			self.AppendMallItemLastTime(endTime)
		elif metinSlot[0] == 0:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.CANNOT_USE, self.DISABLE_COLOR)
		else:
			endTime = app.GetGlobalTimeStamp() + metinSlot[0]
			self.AppendMallItemLastTime(endTime)

	def AppendRealTimeStartFirstUseLastTime(self, item, metinSlot, limitIndex):
		useCount = metinSlot[1]
		endTime = metinSlot[0]

		# ÇÑ ¹øÀÌ¶óµµ »ç¿ëÇß´Ù¸é Socket0¿¡ Á¾·á ½Ã°£(2012³â 3¿ù 1ÀÏ 13½Ã 01ºÐ °°Àº..) ÀÌ ¹ÚÇôÀÖÀ½.
		# »ç¿ëÇÏÁö ¾Ê¾Ò´Ù¸é Socket0¿¡ ÀÌ¿ë°¡´É½Ã°£(ÀÌ¸¦Å×¸é 600 °°Àº °ª. ÃÊ´ÜÀ§)ÀÌ µé¾îÀÖÀ» ¼ö ÀÖ°í, 0ÀÌ¶ó¸é Limit Value¿¡ ÀÖ´Â ÀÌ¿ë°¡´É½Ã°£À» »ç¿ëÇÑ´Ù.
		if 0 == useCount:
			if 0 == endTime:
				(limitType, limitValue) = item.GetLimit(limitIndex)
				endTime = limitValue

			endTime += app.GetGlobalTimeStamp()

		self.AppendMallItemLastTime(endTime)
	if app.ENABLE_SASH_SYSTEM:
		def SetSashResultItem(self, slotIndex, window_type = player.INVENTORY):
			(itemVnum, MinAbs, MaxAbs) = sash.GetResultItem()
			if not itemVnum:
				return
			
			self.ClearToolTip()
			
			metinSlot = [player.GetItemMetinSocket(window_type, slotIndex, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot = [player.GetItemAttribute(window_type, slotIndex, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			
			item.SelectItem(itemVnum)
			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()
			if itemType != item.ITEM_TYPE_COSTUME and itemSubType != item.COSTUME_TYPE_SASH:
				return
			
			absChance = MaxAbs
			itemDesc = item.GetItemDescription()
			self.__AdjustMaxWidth(attrSlot, itemDesc)
			self.__SetItemTitle(itemVnum, metinSlot, attrSlot)
			self.AppendDescription(itemDesc, 26)
			self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
			self.__AppendLimitInformation()
			
			## ABSORPTION RATE
			if MinAbs == MaxAbs:
				self.AppendTextLine(localeInfo.SASH_ABSORB_CHANCE % (MinAbs), self.CONDITION_COLOR)
			else:
				self.AppendTextLine(localeInfo.SASH_ABSORB_CHANCE2 % (MinAbs, MaxAbs), self.CONDITION_COLOR)
			## END ABSOPRTION RATE
			
			itemAbsorbedVnum = int(metinSlot[sash.ABSORBED_SOCKET])
			if itemAbsorbedVnum:
				## ATTACK / DEFENCE
				item.SelectItem(itemAbsorbedVnum)
				if item.GetItemType() == item.ITEM_TYPE_WEAPON:
					if item.GetItemSubType() == item.WEAPON_FAN:
						self.__AppendMagicAttackInfo(absChance)
						item.SelectItem(itemAbsorbedVnum)
						self.__AppendAttackPowerInfo(absChance)
					else:
						self.__AppendAttackPowerInfo(absChance)
						item.SelectItem(itemAbsorbedVnum)
						self.__AppendMagicAttackInfo(absChance)
				elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
					defGrade = item.GetValue(1)
					defBonus = item.GetValue(5) * 2
					defGrade = self.CalcSashValue(defGrade, absChance)
					defBonus = self.CalcSashValue(defBonus, absChance)
					
					if defGrade > 0:
						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))
					
					item.SelectItem(itemAbsorbedVnum)
					self.__AppendMagicDefenceInfo(absChance)
				## END ATTACK / DEFENCE
				
				## EFFECT
				item.SelectItem(itemAbsorbedVnum)
				for i in xrange(item.ITEM_APPLY_MAX_NUM):
					(affectType, affectValue) = item.GetAffect(i)
					affectValue = self.CalcSashValue(affectValue, absChance)
					affectString = self.__GetAffectString(affectType, affectValue)
					if affectString and affectValue > 0:
						self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
					
					item.SelectItem(itemAbsorbedVnum)
				# END EFFECT
				
			item.SelectItem(itemVnum)
			## ATTR
			self.__AppendAttributeInformation(attrSlot, MaxAbs)
			# END ATTR
			
			self.AppendWearableInformation()
			self.ShowToolTip()

		def SetSashResultAbsItem(self, slotIndex1, slotIndex2, window_type = player.INVENTORY):
			itemVnumSash = player.GetItemIndex(window_type, slotIndex1)
			itemVnumTarget = player.GetItemIndex(window_type, slotIndex2)
			if not itemVnumSash or not itemVnumTarget:
				return
			
			self.ClearToolTip()
			
			item.SelectItem(itemVnumSash)
			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()
			if itemType != item.ITEM_TYPE_COSTUME and itemSubType != item.COSTUME_TYPE_SASH:
				return
			
			metinSlot = [player.GetItemMetinSocket(window_type, slotIndex1, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot = [player.GetItemAttribute(window_type, slotIndex2, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			
			itemDesc = item.GetItemDescription()
			self.__AdjustMaxWidth(attrSlot, itemDesc)
			self.__SetItemTitle(itemVnumSash, metinSlot, attrSlot)
			self.AppendDescription(itemDesc, 26)
			self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
			item.SelectItem(itemVnumSash)
			self.__AppendLimitInformation()
			
			## ABSORPTION RATE
			self.AppendTextLine(localeInfo.SASH_ABSORB_CHANCE % (metinSlot[sash.ABSORPTION_SOCKET]), self.CONDITION_COLOR)
			## END ABSOPRTION RATE
			
			## ATTACK / DEFENCE
			itemAbsorbedVnum = itemVnumTarget
			item.SelectItem(itemAbsorbedVnum)
			if item.GetItemType() == item.ITEM_TYPE_WEAPON:
				if item.GetItemSubType() == item.WEAPON_FAN:
					self.__AppendMagicAttackInfo(metinSlot[sash.ABSORPTION_SOCKET])
					item.SelectItem(itemAbsorbedVnum)
					self.__AppendAttackPowerInfo(metinSlot[sash.ABSORPTION_SOCKET])
				else:
					self.__AppendAttackPowerInfo(metinSlot[sash.ABSORPTION_SOCKET])
					item.SelectItem(itemAbsorbedVnum)
					self.__AppendMagicAttackInfo(metinSlot[sash.ABSORPTION_SOCKET])
			elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
				defGrade = item.GetValue(1)
				defBonus = item.GetValue(5) * 2
				defGrade = self.CalcSashValue(defGrade, metinSlot[sash.ABSORPTION_SOCKET])
				defBonus = self.CalcSashValue(defBonus, metinSlot[sash.ABSORPTION_SOCKET])
				
				if defGrade > 0:
					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))
				
				item.SelectItem(itemAbsorbedVnum)
				self.__AppendMagicDefenceInfo(metinSlot[sash.ABSORPTION_SOCKET])
			## END ATTACK / DEFENCE
			
			## EFFECT
			item.SelectItem(itemAbsorbedVnum)
			for i in xrange(item.ITEM_APPLY_MAX_NUM):
				(affectType, affectValue) = item.GetAffect(i)
				affectValue = self.CalcSashValue(affectValue, metinSlot[sash.ABSORPTION_SOCKET])
				affectString = self.__GetAffectString(affectType, affectValue)
				if affectString and affectValue > 0:
					self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
				
				item.SelectItem(itemAbsorbedVnum)
			## END EFFECT
			
			## ATTR
			item.SelectItem(itemAbsorbedVnum)
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				type = attrSlot[i][0]
				value = attrSlot[i][1]
				if not value:
					continue
				
				value = self.CalcSashValue(value, metinSlot[sash.ABSORPTION_SOCKET])
				affectString = self.__GetAffectString(type, value)
				if affectString and value > 0:
					affectColor = self.__GetAttributeColor(i, value)
					self.AppendTextLine(affectString, affectColor)
				
				item.SelectItem(itemAbsorbedVnum)
			## END ATTR
			
			## WEARABLE
			item.SelectItem(itemVnumSash)
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_WEARABLE_JOB, self.NORMAL_COLOR)
			
			item.SelectItem(itemVnumSash)
			flagList = (
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR),
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN),
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA),
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN)
			)
			if app.ENABLE_WOLFMAN_CHARACTER:
				flagList += (not item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN),)

			characterNames = ""
			for i in xrange(self.CHARACTER_COUNT):
				name = self.CHARACTER_NAMES[i]
				flag = flagList[i]
				if flag:
					characterNames += " "
					characterNames += name
			
			textLine = self.AppendTextLine(characterNames, self.NORMAL_COLOR, True)
			textLine.SetFeather()
			
			item.SelectItem(itemVnumSash)
			if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
				textLine = self.AppendTextLine(localeInfo.FOR_FEMALE, self.NORMAL_COLOR, True)
				textLine.SetFeather()
			
			if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
				textLine = self.AppendTextLine(localeInfo.FOR_MALE, self.NORMAL_COLOR, True)
				textLine.SetFeather()
			## END WEARABLE
			
			self.ShowToolTip()

	if app.ENABLE_ACCE_ABSORBED_ITEM_PREVIEW:
		def SetAbsorbedItemPreview(self, itemVnum, attrSlot):
			item.SelectItem(itemVnum)

			metinSlot = []
			for i in xrange(player.METIN_SOCKET_MAX_NUM):
				metinSlot.append(0)

			self.acce_preview_tooltip = ItemToolTip()
			self.acce_preview_tooltip.AutoAppendTextLine(localeInfo.ITEM_TOOLTIP_PREVIEW_TITLE, 0xffADFF2F)
			self.acce_preview_tooltip.AppendSpace(5)
			self.acce_preview_tooltip.acce_preview = True
			self.acce_preview_tooltip.AddItemData(itemVnum, metinSlot, attrSlot)
			self.acce_preview_tooltip.ResizeToolTip()
			self.acce_preview_tooltip.Hide()


class HyperlinkItemToolTip(ItemToolTip):
	def __init__(self):
		ItemToolTip.__init__(self, isPickable=True)


	def SetHyperlinkItem(self, tokens):
		minTokenCount = 3

		head, vnum, flag = tokens[:minTokenCount]
		itemVnum = int(vnum, 16)

		metinSlot = [int(metin, 16) for metin in tokens[minTokenCount:minTokenCount+player.METIN_SOCKET_MAX_NUM]]

		minTokenCount+=player.METIN_SOCKET_MAX_NUM

		attrSlot = []
		rests = tokens[minTokenCount:minTokenCount+(player.ATTRIBUTE_SLOT_MAX_NUM*2)]
		if rests:
			rests.reverse()
			while rests:
				key = int(rests.pop(), 16)
				if rests:
					val = int(rests.pop())
					attrSlot.append((key, val))
				else:
					attrSlot.append((0, 0))

		minTokenCount+=(player.ATTRIBUTE_SLOT_MAX_NUM*2)

		self.ShowRender(False)
		self.ClearToolTip()
		self.AddItemData(itemVnum, metinSlot, attrSlot)

		ItemToolTip.OnUpdate(self)

	def OnUpdate(self):
		pass

	def OnMouseLeftButtonDown(self):
		self.Hide()

class SkillToolTip(ToolTip):

	POINT_NAME_DICT = {
		player.LEVEL : localeInfo.SKILL_TOOLTIP_LEVEL,
		player.IQ : localeInfo.SKILL_TOOLTIP_INT,
	}

	SKILL_TOOL_TIP_WIDTH = 200
	PARTY_SKILL_TOOL_TIP_WIDTH = 340

	PARTY_SKILL_EXPERIENCE_AFFECT_LIST = (	( 2, 2,  10,),
											( 8, 3,  20,),
											(14, 4,  30,),
											(22, 5,  45,),
											(28, 6,  60,),
											(34, 7,  80,),
											(38, 8, 100,), )

	PARTY_SKILL_PLUS_GRADE_AFFECT_LIST = (	( 4, 2, 1, 0,),
											(10, 3, 2, 0,),
											(16, 4, 2, 1,),
											(24, 5, 2, 2,), )

	PARTY_SKILL_ATTACKER_AFFECT_LIST = (	( 36, 3, ),
											( 26, 1, ),
											( 32, 2, ), )

	SKILL_GRADE_NAME = {	player.SKILL_GRADE_MASTER : localeInfo.SKILL_GRADE_NAME_MASTER,
							player.SKILL_GRADE_GRAND_MASTER : localeInfo.SKILL_GRADE_NAME_GRAND_MASTER,
							player.SKILL_GRADE_PERFECT_MASTER : localeInfo.SKILL_GRADE_NAME_PERFECT_MASTER, }

	AFFECT_NAME_DICT =	{
							"HP" : localeInfo.TOOLTIP_SKILL_AFFECT_ATT_POWER,
							"ATT_GRADE" : localeInfo.TOOLTIP_SKILL_AFFECT_ATT_GRADE,
							"DEF_GRADE" : localeInfo.TOOLTIP_SKILL_AFFECT_DEF_GRADE,
							"ATT_SPEED" : localeInfo.TOOLTIP_SKILL_AFFECT_ATT_SPEED,
							"MOV_SPEED" : localeInfo.TOOLTIP_SKILL_AFFECT_MOV_SPEED,
							"DODGE" : localeInfo.TOOLTIP_SKILL_AFFECT_DODGE,
							"RESIST_NORMAL" : localeInfo.TOOLTIP_SKILL_AFFECT_RESIST_NORMAL,
							"REFLECT_MELEE" : localeInfo.TOOLTIP_SKILL_AFFECT_REFLECT_MELEE,
						}
	AFFECT_APPEND_TEXT_DICT =	{
									"DODGE" : "%",
									"RESIST_NORMAL" : "%",
									"REFLECT_MELEE" : "%",
								}

	def __init__(self):
		ToolTip.__init__(self, self.SKILL_TOOL_TIP_WIDTH)
	def __del__(self):
		ToolTip.__del__(self)
		
	def SetSkillEx(self, skillIndex, skillLevel = -1):
		if 0 == skillIndex:
			return
		slotIndex = player.GetSkillSlotIndex(skillIndex)
		skillCurrentPercentage = player.GetSkillCurrentEfficientPercentage(slotIndex)
		try:
			for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
				self.AppendTextLine(skill.GetSkillAffectDescription(skillIndex, i, skillCurrentPercentage), self.ENABLE_COLOR)
		except:
			pass
			
		self.ResizeToolTip()
		
	def SetSkillTest(self, skillIndex, skillLevel = -1):
		if 0 == skillIndex:
			return
		slotIndex = player.GetSkillSlotIndex(skillIndex)
		skillCurrentPercentage = player.GetSkillCurrentEfficientPercentage(slotIndex)
		try:
			for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
				self.AppendTextLine(skill.GetAffectDescriptionWithPower(skillIndex, i, skillLevel), self.ENABLE_COLOR)
		except:
			pass
			
		self.ResizeToolTip()

	def SetSkill(self, skillIndex, skillLevel = -1):

		if 0 == skillIndex:
			return

		if skill.SKILL_TYPE_GUILD == skill.GetSkillType(skillIndex):

			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			self.AppendDefaultData(skillIndex)
			self.AppendSkillConditionData(skillIndex)
			self.AppendGuildSkillData(skillIndex, skillLevel)

		else:

			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			slotIndex = player.GetSkillSlotIndex(skillIndex)
			skillGrade = player.GetSkillGrade(slotIndex)
			skillLevel = player.GetSkillLevel(slotIndex)
			skillCurrentPercentage = player.GetSkillCurrentEfficientPercentage(slotIndex)
			skillNextPercentage = player.GetSkillNextEfficientPercentage(slotIndex)

			self.AppendDefaultData(skillIndex)
			self.AppendSkillConditionData(skillIndex)
			self.AppendSkillDataNew(slotIndex, skillIndex, skillGrade, skillLevel, skillCurrentPercentage, skillNextPercentage)
			self.AppendSkillRequirement(skillIndex, skillLevel)

		self.ShowToolTip()

	def SetSkillNew(self, slotIndex, skillIndex, skillGrade, skillLevel):
		if skillIndex == 0:
			return

		if app.USE_PASSIVE_ABILITY_TAMER:
			global PASSIVE_ABILITY_VNUM_LIST
			if skillIndex in PASSIVE_ABILITY_VNUM_LIST:
				if skillGrade == 1:
					skillLevel += 19
				elif skillGrade == 2:
					skillLevel += 29
				elif skillGrade == 3:
					skillLevel = 40

				self.ClearToolTip()
				self.__SetSkillTitle(skillIndex, skillGrade)

				descList = [\
								localeInfo.PASSIVE_ABILITY_VNUM_TAMER_1,
								localeInfo.PASSIVE_ABILITY_VNUM_TAMER_2,
								localeInfo.PASSIVE_ABILITY_VNUM_TAMER_3,
								localeInfo.PASSIVE_ABILITY_VNUM_TAMER_4
				]

				valList = [\
							( 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 ),
							( 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 ),
							( 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 ),
							( 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 )
				]

				self.AppendDescription(skill.GetSkillDescription(skillIndex), 25)

				if skillLevel >= 0 and skillLevel < 40:
					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel), self.NORMAL_COLOR)
					self.AppendTextLine(descList[skillIndex - skill.PASSIVE_ABILITY_VNUM_TAMER_1] % (valList[skillIndex - skill.PASSIVE_ABILITY_VNUM_TAMER_1][skillLevel]), self.POSITIVE_COLOR)

					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_NEXT_LEVEL % (skillLevel+1), self.NEGATIVE_COLOR)
					self.AppendTextLine(descList[skillIndex - skill.PASSIVE_ABILITY_VNUM_TAMER_1] % (valList[skillIndex - skill.PASSIVE_ABILITY_VNUM_TAMER_1][skillLevel + 1]), self.NEGATIVE_COLOR)
				elif skillGrade == 3 and skillLevel >= 40:
					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_MASTER % (skillLevel), self.NORMAL_COLOR)
					self.AppendTextLine(descList[skillIndex - skill.PASSIVE_ABILITY_VNUM_TAMER_1] % (valList[skillIndex - skill.PASSIVE_ABILITY_VNUM_TAMER_1][skillLevel]), self.POSITIVE_COLOR)

				self.ShowToolTip()
				return

		if player.SKILL_INDEX_TONGSOL == skillIndex:

			slotIndex = player.GetSkillSlotIndex(skillIndex)
			skillLevel = player.GetSkillLevel(slotIndex)

			self.AppendDefaultData(skillIndex)
			self.AppendPartySkillData(skillGrade, skillLevel)

		elif player.SKILL_INDEX_RIDING == skillIndex:

			slotIndex = player.GetSkillSlotIndex(skillIndex)
			self.AppendSupportSkillDefaultData(skillIndex, skillGrade, skillLevel, 30)

		elif player.SKILL_INDEX_SUMMON == skillIndex:

			maxLevel = 10

			self.ClearToolTip()
			self.__SetSkillTitle(skillIndex, skillGrade)

			## Description
			description = skill.GetSkillDescription(skillIndex)
			self.AppendDescription(description, 25)

			if skillLevel == 10:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_MASTER % (skillLevel), self.NORMAL_COLOR)
				self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (skillLevel*10), self.NORMAL_COLOR)

			else:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel), self.NORMAL_COLOR)
				self.__AppendSummonDescription(skillLevel, self.NORMAL_COLOR)

				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel+1), self.NEGATIVE_COLOR)
				self.__AppendSummonDescription(skillLevel+1, self.NEGATIVE_COLOR)

		elif skill.SKILL_TYPE_GUILD == skill.GetSkillType(skillIndex):

			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			self.AppendDefaultData(skillIndex)
			self.AppendSkillConditionData(skillIndex)
			self.AppendGuildSkillData(skillIndex, skillLevel)

		else:

			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			slotIndex = player.GetSkillSlotIndex(skillIndex)

			skillCurrentPercentage = player.GetSkillCurrentEfficientPercentage(slotIndex)
			skillNextPercentage = player.GetSkillNextEfficientPercentage(slotIndex)

			self.AppendDefaultData(skillIndex, skillGrade)
			self.AppendSkillConditionData(skillIndex)
			self.AppendSkillDataNew(slotIndex, skillIndex, skillGrade, skillLevel, skillCurrentPercentage, skillNextPercentage)
			self.AppendSkillRequirement(skillIndex, skillLevel)

		self.ShowToolTip()

	def __SetSkillTitle(self, skillIndex, skillGrade):
		self.SetTitle(skill.GetSkillName(skillIndex, skillGrade))
		self.__AppendSkillGradeName(skillIndex, skillGrade)

	def __AppendSkillGradeName(self, skillIndex, skillGrade):
		if self.SKILL_GRADE_NAME.has_key(skillGrade):
			self.AppendSpace(5)
			self.AppendTextLine(self.SKILL_GRADE_NAME[skillGrade] % (skill.GetSkillName(skillIndex, 0)), self.CAN_LEVEL_UP_COLOR)

	def SetSkillOnlyName(self, slotIndex, skillIndex, skillGrade):
		if 0 == skillIndex:
			return

		slotIndex = player.GetSkillSlotIndex(skillIndex)

		self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
		self.ResizeToolTip()

		self.ClearToolTip()
		self.__SetSkillTitle(skillIndex, skillGrade)
		self.AppendDefaultData(skillIndex, skillGrade)
		self.AppendSkillConditionData(skillIndex)
		self.ShowToolTip()

	def AppendDefaultData(self, skillIndex, skillGrade = 0):
		self.ClearToolTip()
		self.__SetSkillTitle(skillIndex, skillGrade)

		## Level Limit
		levelLimit = skill.GetSkillLevelLimit(skillIndex)
		if levelLimit > 0:

			color = self.NORMAL_COLOR
			if player.GetStatus(player.LEVEL) < levelLimit:
				color = self.NEGATIVE_COLOR

			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_LEVEL % (levelLimit), color)

		## Description
		description = skill.GetSkillDescription(skillIndex)
		self.AppendDescription(description, 25)

	def AppendSupportSkillDefaultData(self, skillIndex, skillGrade, skillLevel, maxLevel):
		self.ClearToolTip()
		self.__SetSkillTitle(skillIndex, skillGrade)

		## Description
		description = skill.GetSkillDescription(skillIndex)
		self.AppendDescription(description, 25)

		if 1 == skillGrade:
			skillLevel += 19
		elif 2 == skillGrade:
			skillLevel += 29
		elif 3 == skillGrade:
			skillLevel = 40

		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_WITH_MAX % (skillLevel, maxLevel), self.NORMAL_COLOR)

	def AppendSkillConditionData(self, skillIndex):
		conditionDataCount = skill.GetSkillConditionDescriptionCount(skillIndex)
		if conditionDataCount > 0:
			self.AppendSpace(5)
			for i in xrange(conditionDataCount):
				self.AppendTextLine(skill.GetSkillConditionDescription(skillIndex, i), self.CONDITION_COLOR)

	def AppendGuildSkillData(self, skillIndex, skillLevel):
		skillMaxLevel = 7
		skillCurrentPercentage = float(skillLevel) / float(skillMaxLevel)
		skillNextPercentage = float(skillLevel+1) / float(skillMaxLevel)
		## Current Level
		if skillLevel > 0:
			if self.HasSkillLevelDescription(skillIndex, skillLevel):
				self.AppendSpace(5)
				if skillLevel == skillMaxLevel:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_MASTER % (skillLevel), self.NORMAL_COLOR)
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel), self.NORMAL_COLOR)

				#####

				for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
					self.AppendTextLine(skill.GetSkillAffectDescription(skillIndex, i, skillCurrentPercentage), self.ENABLE_COLOR)

				## Cooltime
				coolTime = skill.GetSkillCoolTime(skillIndex, skillCurrentPercentage)
				if coolTime > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_COOL_TIME + str(coolTime), self.ENABLE_COLOR)

				## SP
				needGSP = skill.GetSkillNeedSP(skillIndex, skillCurrentPercentage)
				if needGSP > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_NEED_GSP % (needGSP), self.ENABLE_COLOR)

		## Next Level
		if skillLevel < skillMaxLevel:
			if self.HasSkillLevelDescription(skillIndex, skillLevel+1):
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_NEXT_SKILL_LEVEL_1 % (skillLevel+1, skillMaxLevel), self.DISABLE_COLOR)

				#####

				for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
					self.AppendTextLine(skill.GetSkillAffectDescription(skillIndex, i, skillNextPercentage), self.DISABLE_COLOR)

				## Cooltime
				coolTime = skill.GetSkillCoolTime(skillIndex, skillNextPercentage)
				if coolTime > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_COOL_TIME + str(coolTime), self.DISABLE_COLOR)

				## SP
				needGSP = skill.GetSkillNeedSP(skillIndex, skillNextPercentage)
				if needGSP > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_NEED_GSP % (needGSP), self.DISABLE_COLOR)

	def AppendSkillDataNew(self, slotIndex, skillIndex, skillGrade, skillLevel, skillCurrentPercentage, skillNextPercentage):

		self.skillMaxLevelStartDict = { 0 : 17, 1 : 7, 2 : 10, }
		self.skillMaxLevelEndDict = { 0 : 20, 1 : 10, 2 : 10, }

		skillLevelUpPoint = 1
		realSkillGrade = player.GetSkillGrade(slotIndex)
		skillMaxLevelStart = self.skillMaxLevelStartDict.get(realSkillGrade, 15)
		skillMaxLevelEnd = self.skillMaxLevelEndDict.get(realSkillGrade, 20)

		## Current Level
		if skillLevel > 0:
			if self.HasSkillLevelDescription(skillIndex, skillLevel):
				self.AppendSpace(5)
				if skillGrade == skill.SKILL_GRADE_COUNT:
					pass
				elif skillLevel == skillMaxLevelEnd:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_MASTER % (skillLevel), self.NORMAL_COLOR)
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel), self.NORMAL_COLOR)
				self.AppendSkillLevelDescriptionNew(skillIndex, skillCurrentPercentage, self.ENABLE_COLOR)

		## Next Level
		if skillGrade != skill.SKILL_GRADE_COUNT:
			if skillLevel < skillMaxLevelEnd:
				if self.HasSkillLevelDescription(skillIndex, skillLevel+skillLevelUpPoint):
					self.AppendSpace(5)
					## HPº¸°­, °üÅëÈ¸ÇÇ º¸Á¶½ºÅ³ÀÇ °æ¿ì
					if skillIndex == 141 or skillIndex == 142:
						self.AppendTextLine(localeInfo.TOOLTIP_NEXT_SKILL_LEVEL_3 % (skillLevel+1), self.DISABLE_COLOR)
					else:
						self.AppendTextLine(localeInfo.TOOLTIP_NEXT_SKILL_LEVEL_1 % (skillLevel+1, skillMaxLevelEnd), self.DISABLE_COLOR)
					self.AppendSkillLevelDescriptionNew(skillIndex, skillNextPercentage, self.DISABLE_COLOR)

	def AppendSkillLevelDescriptionNew(self, skillIndex, skillPercentage, color):

		affectDataCount = skill.GetNewAffectDataCount(skillIndex)
		if affectDataCount > 0:
			for i in xrange(affectDataCount):
				type, minValue, maxValue = skill.GetNewAffectData(skillIndex, i, skillPercentage)

				if not self.AFFECT_NAME_DICT.has_key(type):
					continue

				minValue = int(minValue)
				maxValue = int(maxValue)
				affectText = self.AFFECT_NAME_DICT[type]

				if "HP" == type:
					if minValue < 0 and maxValue < 0:
						minValue *= -1
						maxValue *= -1

					else:
						affectText = localeInfo.TOOLTIP_SKILL_AFFECT_HEAL

				affectText += str(minValue)
				if minValue != maxValue:
					affectText += " - " + str(maxValue)
				affectText += self.AFFECT_APPEND_TEXT_DICT.get(type, "")

				#import debugInfo
				#if debugInfo.IsDebugMode():
				#	affectText = "!!" + affectText

				self.AppendTextLine(affectText, color)

		else:
			for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
				self.AppendTextLine(skill.GetSkillAffectDescription(skillIndex, i, skillPercentage), color)

		## Duration
		duration = skill.GetDuration(skillIndex, skillPercentage)
		if duration > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_SKILL_DURATION % (duration), color)

		## Cooltime
		coolTime = skill.GetSkillCoolTime(skillIndex, skillPercentage)
		if coolTime > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_SKILL_COOL_TIME + str(coolTime), color)

		## SP
		needSP = skill.GetSkillNeedSP(skillIndex, skillPercentage)
		if needSP != 0:
			continuationSP = skill.GetSkillContinuationSP(skillIndex, skillPercentage)

			if skill.IsUseHPSkill(skillIndex):
				self.AppendNeedHP(needSP, continuationSP, color)
			else:
				self.AppendNeedSP(needSP, continuationSP, color)

	def AppendSkillRequirement(self, skillIndex, skillLevel):

		skillMaxLevel = skill.GetSkillMaxLevel(skillIndex)

		if skillLevel >= skillMaxLevel:
			return

		isAppendHorizontalLine = False

		## Requirement
		if skill.IsSkillRequirement(skillIndex):

			if not isAppendHorizontalLine:
				isAppendHorizontalLine = True
				self.AppendHorizontalLine()

			requireSkillName, requireSkillLevel = skill.GetSkillRequirementData(skillIndex)

			color = self.CANNOT_LEVEL_UP_COLOR
			if skill.CheckRequirementSueccess(skillIndex):
				color = self.CAN_LEVEL_UP_COLOR
			self.AppendTextLine(localeInfo.TOOLTIP_REQUIREMENT_SKILL_LEVEL % (requireSkillName, requireSkillLevel), color)

		## Require Stat
		requireStatCount = skill.GetSkillRequireStatCount(skillIndex)
		if requireStatCount > 0:

			for i in xrange(requireStatCount):
				type, level = skill.GetSkillRequireStatData(skillIndex, i)
				if self.POINT_NAME_DICT.has_key(type):

					if not isAppendHorizontalLine:
						isAppendHorizontalLine = True
						self.AppendHorizontalLine()

					name = self.POINT_NAME_DICT[type]
					color = self.CANNOT_LEVEL_UP_COLOR
					if player.GetStatus(type) >= level:
						color = self.CAN_LEVEL_UP_COLOR
					self.AppendTextLine(localeInfo.TOOLTIP_REQUIREMENT_STAT_LEVEL % (name, level), color)

	def HasSkillLevelDescription(self, skillIndex, skillLevel):
		if skill.GetSkillAffectDescriptionCount(skillIndex) > 0:
			return True
		if skill.GetSkillCoolTime(skillIndex, skillLevel) > 0:
			return True
		if skill.GetSkillNeedSP(skillIndex, skillLevel) > 0:
			return True

		return False

	def AppendMasterAffectDescription(self, index, desc, color):
		self.AppendTextLine(desc, color)

	def AppendNextAffectDescription(self, index, desc):
		self.AppendTextLine(desc, self.DISABLE_COLOR)

	def AppendNeedHP(self, needSP, continuationSP, color):

		self.AppendTextLine(localeInfo.TOOLTIP_NEED_HP % (needSP), color)

		if continuationSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_HP_PER_SEC % (continuationSP), color)

	def AppendNeedSP(self, needSP, continuationSP, color):

		if -1 == needSP:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_ALL_SP, color)

		else:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_SP % (needSP), color)

		if continuationSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_SP_PER_SEC % (continuationSP), color)

	def AppendPartySkillData(self, skillGrade, skillLevel):
		def fix001(vl):
			return vl.replace("%,0f", "%.0f")

		if 1 == skillGrade:
			skillLevel += 19
		elif 2 == skillGrade:
			skillLevel += 29
		elif 3 == skillGrade:
			skillLevel =  40

		if skillLevel <= 0:
			return

		skillIndex = player.SKILL_INDEX_TONGSOL
		slotIndex = player.GetSkillSlotIndex(skillIndex)
		skillPower = player.GetSkillCurrentEfficientPercentage(slotIndex)
		if localeInfo.IsBRAZIL():
			k = skillPower
		else:
			k = player.GetSkillLevel(skillIndex) / 100.0
		self.AppendSpace(5)
		self.AutoAppendTextLine(localeInfo.TOOLTIP_PARTY_SKILL_LEVEL % skillLevel, self.NORMAL_COLOR)

		if skillLevel>=10:
			self.AutoAppendTextLine(fix001(localeInfo.PARTY_SKILL_ATTACKER) % chop( 10 + 60 * k ))

		if skillLevel>=20:
			self.AutoAppendTextLine(fix001(localeInfo.PARTY_SKILL_BERSERKER) % chop(1 + 5 * k))
			self.AutoAppendTextLine(fix001(localeInfo.PARTY_SKILL_TANKER) % chop(50 + 1450 * k))

		if skillLevel>=25:
			self.AutoAppendTextLine(fix001(localeInfo.PARTY_SKILL_BUFFER) % chop(5 + 45 * k ))

		if skillLevel>=35:
			self.AutoAppendTextLine(fix001(localeInfo.PARTY_SKILL_SKILL_MASTER) % chop(25 + 600 * k ))

		if skillLevel>=40:
			self.AutoAppendTextLine(fix001(localeInfo.PARTY_SKILL_DEFENDER) % chop( 5 + 30 * k ))

		self.AlignHorizonalCenter()

	def __AppendSummonDescription(self, skillLevel, color):
		if skillLevel > 1:
			self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (skillLevel * 10), color)
		elif 1 == skillLevel:
			self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (15), color)
		elif 0 == skillLevel:
			self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (10), color)


if __name__ == "__main__":
	import app
	import wndMgr
	import systemSetting
	import mouseModule
	import grp
	import ui

	#wndMgr.SetOutlineFlag(True)

	app.SetMouseHandler(mouseModule.mouseController)
	app.SetHairColorEnable(True)
	wndMgr.SetMouseHandler(mouseModule.mouseController)
	wndMgr.SetScreenSize(systemSetting.GetWidth(), systemSetting.GetHeight())
	app.Create("METIN2 CLOSED BETA", systemSetting.GetWidth(), systemSetting.GetHeight(), 1)
	mouseModule.mouseController.Create()

	toolTip = ItemToolTip()
	toolTip.ClearToolTip()
	#toolTip.AppendTextLine("Test")
	desc = "Item descriptions:|increase of width of display to 35 digits per row AND installation of function that the displayed words are not broken up in two parts, but instead if one word is too long to be displayed in this row, this word will start in the next row."
	summ = ""

	toolTip.AddItemData_Offline(10, desc, summ, 0, 0)
	toolTip.Show()

	app.Loop()
