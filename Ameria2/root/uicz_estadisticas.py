# coding: latin_1

import ui
import player
import net
import item
import localeInfo
import chat
import app
import uiScriptLocale

class CombatZoneEstadisticas(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.time = 0
        self.LoadWindow()

    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "uiscript/cz_estadisticas.py")
        except:
            import exception
            exception.Abort("CombatZoneEstadisticas.LoadDialog.LoadScript")

        try:
            GetObject=self.GetChild
            self.btnexit = GetObject("exit_button")
            self.puntos = GetObject("puntos")
            self.bonus = GetObject("bonus")
            self.tiempo = GetObject("tiempo")

        except:
            import exception
            exception.Abort("Discord.LoadDialog.BindObject")

        self.btnexit.SetEvent(ui.__mem_func__(self.__OnClickExit))

    def SetPoint(self):
        self.puntos.SetText("{}".format(player.GetCombatZonePoints()))

    def SetBonus(self,arg,type=0,value=0):
        if arg == "ADD":
            list_bonus = [
                [item.APPLY_ATTBONUS_HUMAN,"M.Humanos"],
                [item.APPLY_ATTBONUS_WARRIOR,"F.G"],
                [item.APPLY_ATTBONUS_ASSASSIN,"F.N"],
                [item.APPLY_ATTBONUS_SURA,"F.S"],
                [item.APPLY_ATTBONUS_SHAMAN,"F.C"],
                [item.APPLY_MAX_HP,"HP"],
                [item.APPLY_SKILL_DAMAGE_BONUS,"D. Hab"],
                [item.APPLY_NORMAL_HIT_DAMAGE_BONUS,"D. Med"]
            ]

            for x in list_bonus:
                if x[0] == int(type):
                    self.bonus.SetText("{} + {}".format(x[1],int(value)))
        else:
            self.bonus.SetText("None")

    def RefreshStatus(self):
        self.SetPoint()



    def SetTime(self,time):
        self.time =  int(time) + app.GetGlobalTimeStamp()

    def SetTimeConvert(self, time):
        time_collect = time - app.GetGlobalTimeStamp()

        if time_collect < 0:
            time_collect = 0

        self.tiempo.SetText(self.__FormatTime(time_collect))

    def __FormatTime(self, time):
        m, s = divmod(time, 60)
        h, m = divmod(m, 60)
        return "%1d:%02d:%02d" % (h, m, s)    

    def OnUpdate(self):
        self.SetTimeConvert(self.time)

    def __OnClickExit(self):
        if player.IsCombatZoneMap():
            self.OnAskCombatZoneQuestionDialog()


    def OnAskCombatZoneQuestionDialog(self):
        import uiCommon
        self.combatZoneLeaveQuestionDialog = uiCommon.QuestionDialog2()
        self.combatZoneLeaveQuestionDialog.SetText1(uiScriptLocale.EXIT_BATTLE_FIELD_COLLECTED_POINTS % (player.GetCombatZonePoints()))
        self.combatZoneLeaveQuestionDialog.SetText2(uiScriptLocale.EXIT_BATTLE_FIELD)
        self.combatZoneLeaveQuestionDialog.SetWidth(320)
        self.combatZoneLeaveQuestionDialog.SetAcceptEvent(lambda arg = TRUE: self.OnToggleCombatZoneQuestionDialog(arg))
        self.combatZoneLeaveQuestionDialog.SetCancelEvent(lambda arg = FALSE: self.OnToggleCombatZoneQuestionDialog(arg))
        self.combatZoneLeaveQuestionDialog.Open()
            
    def OnToggleCombatZoneQuestionDialog(self, answer):
        if not self.combatZoneLeaveQuestionDialog:
            return

        self.combatZoneLeaveQuestionDialog.Close()
        self.combatZoneLeaveQuestionDialog = None

        if not answer:
            return

        net.SendCombatZoneRequestActionPacket(net.COMBAT_ZONE_ACTION_LEAVE, net.COMBAT_ZONE_EMPTY_DATA)
        return TRUE

    def Close(self):
        self.Hide()

    #def OnPressEscapeKey(self):
    #    self.Hide()
    #    return True
