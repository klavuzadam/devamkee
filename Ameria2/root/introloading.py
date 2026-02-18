# coding: latin_1

import app
import chrmgr
import net
import player

import ui
import background
import playersettingmodule
import colorinfo
import constInfo
import localeInfo
import uiScriptLocale
import emotion
import wndMgr
import systemSetting

from _weakref import proxy

NAME_COLOR_DICT = {
	chrmgr.NAMECOLOR_PC : colorinfo.CHR_NAME_RGB_PC,
	chrmgr.NAMECOLOR_NPC : colorinfo.CHR_NAME_RGB_NPC,
	chrmgr.NAMECOLOR_MOB : colorinfo.CHR_NAME_RGB_MOB,
	chrmgr.NAMECOLOR_PVP : colorinfo.CHR_NAME_RGB_PVP,
	chrmgr.NAMECOLOR_PK : colorinfo.CHR_NAME_RGB_PK,
	chrmgr.NAMECOLOR_PARTY : colorinfo.CHR_NAME_RGB_PARTY,
	chrmgr.NAMECOLOR_WARP : colorinfo.CHR_NAME_RGB_WARP,
	chrmgr.NAMECOLOR_WAYPOINT : colorinfo.CHR_NAME_RGB_WAYPOINT,
	chrmgr.NAMECOLOR_EMPIRE_MOB : colorinfo.CHR_NAME_RGB_EMPIRE_MOB,
	chrmgr.NAMECOLOR_EMPIRE_NPC : colorinfo.CHR_NAME_RGB_EMPIRE_NPC,
	chrmgr.NAMECOLOR_EMPIRE_PC + 1 : colorinfo.CHR_NAME_RGB_EMPIRE_PC_A,
	chrmgr.NAMECOLOR_EMPIRE_PC + 2 : colorinfo.CHR_NAME_RGB_EMPIRE_PC_B,
	chrmgr.NAMECOLOR_EMPIRE_PC + 3 : colorinfo.CHR_NAME_RGB_EMPIRE_PC_C,
}

if app.ENABLE_STONEMINIMAP:
	NAME_COLOR_DICT[chrmgr.NAMECOLOR_METIN] = colorinfo.CHR_NAME_RGB_METIN

if app.NAMECOLOR_BOSS_CLIENT:
	NAME_COLOR_DICT[chrmgr.NAMECOLOR_BOSS] = colorinfo.CHR_NAME_RGB_BOSS

if app.TOURNAMENT_PVP_SYSTEM:
	NAME_COLOR_DICT[chrmgr.NAMECOLOR_TOURNAMENT_TEAM_A] = colorinfo.CHR_NAME_RGB_TEAM_A
	NAME_COLOR_DICT[chrmgr.NAMECOLOR_TOURNAMENT_TEAM_B] = colorinfo.CHR_NAME_RGB_TEAM_B

TITLE_COLOR_LIST = (
	colorinfo.TITLE_RGB_GOOD_4,
	colorinfo.TITLE_RGB_GOOD_3,
	colorinfo.TITLE_RGB_GOOD_2,
	colorinfo.TITLE_RGB_GOOD_1,
	colorinfo.TITLE_RGB_NORMAL,
	colorinfo.TITLE_RGB_EVIL_1,
	colorinfo.TITLE_RGB_EVIL_2,
	colorinfo.TITLE_RGB_EVIL_3,
	colorinfo.TITLE_RGB_EVIL_4,
)

def __main__():
	for nameIndex, nameColor in NAME_COLOR_DICT.items():
		chrmgr.RegisterNameColor(nameIndex, *nameColor)

	for titleIndex, titleColor in enumerate(TITLE_COLOR_LIST):
		chrmgr.RegisterTitleColor(titleIndex, *titleColor)

	for titleIndex, titleName in enumerate(localeInfo.TITLE_NAME_LIST):
		chrmgr.RegisterTitleName(titleIndex, titleName)

	emotion.RegisterEmotionIcons()

	dungeonMapNameList = ("metin2_map_spiderdungeon", "metin2_map_monkeydungeon", "metin2_map_monkeydungeon_02", "metin2_map_monkeydungeon_13", "metin2_map_deviltower1")
	for dungeonMapName in dungeonMapNameList:
		background.RegisterDungeonMapName(dungeonMapName)

	playersettingmodule.LoadGuildBuildingList(localeInfo.GUILD_BUILDING_LIST_TXT)

class LoadingWindow(ui.ScriptWindow):
	def __init__(self, stream, isWarp):
		ui.Window.__init__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_LOAD, self)
		self.stream = proxy(stream)

		self.isWarp = isWarp
		self.logoUI = None

	def __del__(self):
		net.SetPhaseWindow(net.PHASE_WINDOW_LOAD, 0)
		ui.Window.__del__(self)

	def Open(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/ldwindow.py")

			if self.isWarp:
				# self.GetChild("bg1_ui").Hide()
				# self.GetChild("bg2_ui").Show()
			# else:
				# self.GetChild("bg2_ui").Hide()
				# self.GetChild("bg1_ui").Show()

				self.logoUI = self.GetChild("logo_ui")
		except:
			import exception
			exception.Abort("LoadingWindow.Open - LoadScriptFile Error")

		# self.logoUI.ResetFrame()

		app.HideCursor()

		net.SendSelectCharacterPacket(self.stream.GetCharacterSlot())

		#app.SetFrameSkip(0)
		self.Show()

	def Close(self):
		#app.SetFrameSkip(1)

		self.Hide()
		self.ClearDictionary()

	def OnPressEscapeKey(self):
		return True

	def LoadData(self, x, y):
		playersettingmodule.RegisterSkill(net.GetMainActorRace(), net.GetMainActorSkillGroup(), net.GetMainActorEmpire())

		background.SetViewDistanceSet(background.DISTANCE0, 25600)
		background.SelectViewDistanceNum(background.DISTANCE0)

		app.SetGlobalCenterPosition(x, y)

		net.StartGame()
