# coding: latin_1

import dbg
import app
import net
import localeInfo
import ui
import uiScriptLocale, player
if app.ENABLE_SUNG_MAHI_TOWER:
	import wndMgr

###################################################################################################
## Restart
class RestartDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/restartdialog.py")
		except Exception, msg:
			import sys
			(type, msg, tb)=sys.exc_info()
			dbg.TraceError("RestartDialog.LoadDialog - %s:%s" % (type, msg))
			app.Abort()
			return 0

		try:
			self.restartHereButton=self.GetChild("restart_here_button")
			self.restartTownButton=self.GetChild("restart_town_button")

			if app.RENEWAL_DEAD_PACKET:
				self.reviveTimeStamp = [0 for i in xrange(player.REVIVE_TYPE_MAX)]
				self.reviveTimeTexts = map(lambda i:self.GetChild("T{:02d}".format(i)), xrange(player.REVIVE_TYPE_MAX))

			if (app.WJ_COMBAT_ZONE):
				self.board = self.GetChild("board")
		except:
			import sys
			(type, msg, tb)=sys.exc_info()
			dbg.TraceError("RestartDialog.LoadDialog - %s:%s" % (type, msg))
			app.Abort()
			return 0
		self.restartHereButton.SetEvent(ui.__mem_func__(self.RestartHere))
		self.restartTownButton.SetEvent(ui.__mem_func__(self.RestartTown))
		
		if app.ENABLE_SUNG_MAHI_TOWER:
			self.isSungMahiMap = False
			
			self.dlgBox = ui.Board()
			self.dlgBox.SetSize(300, 100)
			self.dlgBox.SetPosition((wndMgr.GetScreenWidth() / 2) - 150, (wndMgr.GetScreenHeight() / 2) - 100)
			
			self.dlgBoxText = ui.TextLine()
			self.dlgBoxText.SetParent(self.dlgBox)
			self.dlgBoxText.SetPosition(40, 35)
			self.dlgBoxText.SetText("O infrangere amara! Calatoria ta se termina aici.")
			self.dlgBoxText.Show()
			
			self.dlgBoxButton = ui.Button()
			self.dlgBoxButton.SetParent(self.dlgBox)
			self.dlgBoxButton.SetUpVisual("d:/ymir work/ui/public/small_button_01.sub")
			self.dlgBoxButton.SetOverVisual("d:/ymir work/ui/public/small_button_02.sub")
			self.dlgBoxButton.SetDownVisual("d:/ymir work/ui/public/small_button_03.sub")
			self.dlgBoxButton.SetPosition(125, 60)
			self.dlgBoxButton.SetEvent(ui.__mem_func__(self.RestartHere))
			self.dlgBoxButton.SetText("OK")
			self.dlgBoxButton.Show()
			
			self.dlgBox.Hide()

		if (app.WJ_COMBAT_ZONE):
			restartCombatZoneButton = ui.Button()
			restartCombatZoneButton.SetParent(self.board)
			restartCombatZoneButton.SetPosition(10, 77)
			restartCombatZoneButton.SetUpVisual("d:/ymir work/ui/public/XLarge_Button_01.sub")
			restartCombatZoneButton.SetOverVisual("d:/ymir work/ui/public/XLarge_Button_02.sub")
			restartCombatZoneButton.SetDownVisual("d:/ymir work/ui/public/XLarge_Button_03.sub")
			restartCombatZoneButton.SetText(uiScriptLocale.RESTART_IMMEDIATE)
			restartCombatZoneButton.SAFE_SetEvent(self.RestartCombatZone)
			restartCombatZoneButton.Hide()
			self.restartCombatZoneButton = restartCombatZoneButton	
		return 1

	def Destroy(self):
		self.restartHereButton=0
		self.restartTownButton=0
		if (app.WJ_COMBAT_ZONE):
			self.restartCombatZoneButton = 0
			self.board = 0

		if app.RENEWAL_DEAD_PACKET:
			del self.reviveTimeStamp[:]
			del self.reviveTimeTexts[:]

		self.ClearDictionary()
			
	if app.ENABLE_SUNG_MAHI_TOWER:
		def SetSungMahiRestartDialog(self):
			self.isSungMahiMap = True

	if app.RENEWAL_DEAD_PACKET:
		def OpenDialog(self, times):
			if app.ENABLE_SUNG_MAHI_TOWER:
				if self.isSungMahiMap:
					self.dlgBox.Show()
					return
			import chat
			chat.AppendChat(1,"{}".format(times))

			for x in xrange(player.REVIVE_TYPE_MAX):
				self.reviveTimeStamp[x] = app.GetTime() + times[x]
				self.reviveTimeTexts[x].Show()
			self.restartHereButton.Disable()
			self.restartTownButton.Disable()
			self.Show()

		def OnUpdate(self):
			for x in xrange(player.REVIVE_TYPE_MAX): 
				endtime = self.reviveTimeStamp[x] - app.GetTime()
				if endtime <= .1:
					self.reviveTimeTexts[x].Hide()
					if x == player.REVIVE_TYPE_HERE:
						self.restartHereButton.Enable()
					if x == player.REVIVE_TYPE_TOWN:
						self.restartTownButton.Enable()

				if x == player.REVIVE_TYPE_AUTO_TOWN:
					self.reviveTimeTexts[x].SetText(localeInfo.REVIVE_AUTO_TOWN_MESSAGE.format(localeInfo.SecondToMS(endtime)))
				else:
					self.reviveTimeTexts[x].SetText("{0:.1f}".format(endtime))
	else:
		def OpenDialog(self):
			if (app.WJ_COMBAT_ZONE):
				self.CheckWindowStyle()
			self.Show()


	def Close(self):
		self.Hide()
		
		if app.ENABLE_SUNG_MAHI_TOWER:
			if self.dlgBox.IsShow():
				self.dlgBox.Hide()
		
		return True

	def RestartHere(self):
		net.SendChatPacket("/restart_here")

	def RestartTown(self):
		net.SendChatPacket("/restart_town")
	if (app.WJ_COMBAT_ZONE):
		def RestartCombatZone(self):
			net.SendChatPacket("/restart_combat_zone")

		def CheckWindowStyle(self):

			def CheckCombatZoneButton():
				if player.IsCombatZoneMap():
					self.restartCombatZoneButton.Show()
				else:
					self.restartCombatZoneButton.Hide()
				
			def GetSizeByMapLocation():
				return [[200, 88], [200, 113]][player.IsCombatZoneMap()]
		
			xSize, ySize = GetSizeByMapLocation()
			self.board.SetSize(xSize, ySize)
			self.SetSize(xSize, ySize)
			CheckCombatZoneButton()
	def OnPressExitKey(self):
		return True

	def OnPressEscapeKey(self):
		return True
