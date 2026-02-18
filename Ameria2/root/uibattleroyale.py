import ui
import uiCommon
import uiScriptLocale
import localeInfo
import app
import battleRoyaleMgr
import chat
import event
import net
import player
import wndMgr

DEFAULT_DESC_Y = 7
VISIBLE_LINE_COUNT = 20

class BattleRoyaleDead(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.Initialize()
        self.LoadWindow()

    def __del__(self):
        ui.ScriptWindow.__del__(self)
        self.Initialize()

    def Initialize(self):
        self.dead_by_text = None
        self.personal_rank_text = None
        self.return_village_button = None

    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/BattleRoyale_DeadWindow.py")
        except:
            import exception
            exception.Abort("BattleRoyaleDead.LoadWindow.LoadScript")

        try:
            self.dead_by_text = self.GetChild("dead_by_text")
            self.personal_rank_text = self.GetChild("personal_rank_text")
            self.return_village_button = self.GetChild("return_village_button")

        except:
            import exception
            exception.Abort("BattleRoyaleDead.LoadWindow.BindObject")

        try:
            self.return_village_button.SetEvent(ui.__mem_func__(self.ReturnToTown))
       
        except:
            import exception
            exception.Abort("BattleRoyaleDead.LoadWindow.BindEvent")

    def Open(self):
        self.Show()

    def Close(self):
        self.Hide()

    def SetInfo(self, who_killed, personal_rank):
        self.dead_by_text.SetText(localeInfo.BATTLE_ROYALE_WHO_KILLED % who_killed)
        self.personal_rank_text.SetText(localeInfo.BATTLE_ROYALE_PERSONAL_RANK % personal_rank)

    def ReturnToTown(self):
        net.SendChatPacket("/restart_town")

class BattleRoyaleKillInfo(ui.ScriptWindow):
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.Initialize()
        self.LoadWindow()

    def __del__(self):
        ui.ScriptWindow.__del__(self)
        self.Initialize()

    def Initialize(self):
        self.alive_count_text = None
        self.kill_count_text = None

    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/BattleRoyale_KillInfoWindow.py")
        except:
            import exception
            exception.Abort("BattleRoyaleKillInfo.LoadWindow.LoadScript")

        try:
            self.alive_count_text = self.GetChild("alive_count_text")
            self.kill_count_text = self.GetChild("kill_count_text")

        except:
            import exception
            exception.Abort("BattleRoyaleKillInfo.LoadWindow.BindObject")
    
    def Open(self):
        self.Show()

    def Close(self):
        self.Hide()

    def SetInfo(self, alive_count, personal_kill_count):
        self.alive_count_text.SetText(localeInfo.BATTLE_ROYALE_ALIVE_COUNT % alive_count)
        self.kill_count_text.SetText(localeInfo.BATTLE_ROYALE_KILL_COUNT % personal_kill_count)

class BattleRoyaleWinner(BattleRoyaleDead):
    def __init__(self):
        BattleRoyaleDead.__init__(self)

    def __del__(self):
        BattleRoyaleDead.__del__(self)

    def SetInfo(self):
        self.dead_by_text.SetText(localeInfo.BATTLE_ROYALE_WINNER_1)
        self.personal_rank_text.SetText(localeInfo.BATTLE_ROYALE_WINNER_2)

class BattleRoyaleApplication(ui.ScriptWindow):
    MAIN_BUTTON_APPLICATION = 0
    MAIN_BUTTON_CANCEL = 1
    MAIN_BUTTON_EXIT = 2
    MAIN_BUTTON_STARTABLE = 3
    MAIN_BUTTON_REWARD = 4

    class DescriptionBox(ui.Window):
        def __init__(self):
            ui.Window.__init__(self)
            self.descIndex = -1

        def __del__(self):
            ui.Window.__del__(self)

        def SetIndex(self, index):
            self.descIndex = index

        def OnRender(self):
            event.RenderEventSet(self.descIndex)
    
    def __init__(self):
        ui.ScriptWindow.__init__(self)

        self.AddFlag("limit")

        self.Initialize()
        self.LoadWindow()

    def __del__(self):
        ui.ScriptWindow.__del__(self)

        self.Initialize()

    def Initialize(self):
        self.prev_button = None
        self.next_button = None
        self.ranking_button = None
        self.main_button = None

        self.descBoard = None
        self.descriptionBox = None
        self.descIndex = -1
        self.desc_y = DEFAULT_DESC_Y

        self.main_button_state = self.MAIN_BUTTON_APPLICATION
        self.question_dialog = None
    
    def LoadWindow(self):
        try:
            pyScrLoader = ui.PythonScriptLoader()
            pyScrLoader.LoadScriptFile(self, "UIScript/BattleRoyale_ApplicationWindow.py")
        except:
            import exception
            exception.Abort("BattleRoyaleApplication.LoadWindow.LoadScript")

        try:
            self.prev_button = self.GetChild("prev_button")
            self.next_button = self.GetChild("next_button")
            self.ranking_button = self.GetChild("ranking_button")
            self.main_button = self.GetChild("main_button")

            self.descBoard = self.GetChild("desc_board")
            self.descriptionBox = self.DescriptionBox()

            if localeInfo.IsARABIC():
                self.prev_button.LeftRightReverse()
                self.next_button.LeftRightReverse()

            self.ranking_button.Hide()

        except:
            import exception
            exception.Abort("BattleRoyaleApplication.LoadWindow.BindObject")

        try:
            self.GetChild("board").SetCloseEvent( ui.__mem_func__(self.Close) )
            self.prev_button.SetEvent(ui.__mem_func__(self.PrevDescriptionPage))
            self.next_button.SetEvent(ui.__mem_func__(self.NextDescriptionPage))
            self.ranking_button.SetEvent(ui.__mem_func__(self.ClickRankingButton))
            self.main_button.SetEvent(ui.__mem_func__(self.ClickMainButton))

        except:
            import exception
            exception.Abort("BattleRoyaleApplication.LoadWindow.BindEvent")

    def BattleRoyaleApplicated(self):
        chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.BATTLE_ROYALE_NOTIFY_APPLICATED)
        self.main_button_state = self.MAIN_BUTTON_CANCEL
        self.main_button.SetText(localeInfo.BATTLE_ROYALE_MAINBUTTON_CANCEL)

    def BattleRoyaleStartable(self):
        chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.BATTLE_ROYALE_NOTIFY_STARTABLE)
        self.main_button_state = self.MAIN_BUTTON_STARTABLE
        self.main_button.SetText(localeInfo.BATTLE_ROYALE_MAINBUTTON_STARTABLE)
    
    def BattleRoyaleNotApplicated(self):
        chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.BATTLE_ROYALE_NOTIFY_ENTRY_CLOSED)
        self.main_button_state = self.MAIN_BUTTON_APPLICATION
        self.main_button.SetText(localeInfo.BATTLE_ROYALE_MAINBUTTON_APPLICATION)

    def StartBattleRoyale(self):
        self.main_button_state = self.MAIN_BUTTON_EXIT
        self.main_button.SetText(localeInfo.BATTLE_ROYALE_MAINBUTTON_EXIT)
    
    def BattleRoyaleProcess(self, type, data):
        if type == player.BATTLE_ROYALE_GC_INSERT_APPLICANT:
            self.BattleRoyaleApplicated()
        elif type == player.BATTLE_ROYALE_GC_REMOVE_APPLICANT:
            self.main_button_state = self.MAIN_BUTTON_APPLICATION
            self.main_button.SetText(localeInfo.BATTLE_ROYALE_MAINBUTTON_APPLICATION)
        elif type == player.BATTLE_ROYALE_GC_STARTABLE:
            self.BattleRoyaleStartable()
        elif type == player.BATTLE_ROYALE_GC_ENTRY_CLOSED:
            self.BattleRoyaleNotApplicated()
        elif type == player.BATTLE_ROYALE_GC_START:
            self.StartBattleRoyale()
        elif type == player.BATTLE_ROYALE_GC_EXIT:
            self.main_button_state = self.MAIN_BUTTON_APPLICATION
            self.main_button.SetText(localeInfo.BATTLE_ROYALE_MAINBUTTON_APPLICATION)
        elif type == player.BATTLE_ROYALE_GC_HAS_REWARD:
            self.main_button_state = self.MAIN_BUTTON_REWARD
            self.main_button.SetText(localeInfo.BATTLE_ROYALE_MAINBUTTON_CLAIM)

    def ApplyBattleRoyale(self):
        net.SendBattleRoyaleApplication()
    
    def CancelApplicationBattleRoyale(self):
        net.SendBattleRoyaleApplicationCancel()
    
    def ExitBattleRoyale(self):
        net.SendBattleRoyaleExit()
    
    def OnStartBattleRoyale(self, useSpecialItem):
        net.SendBattleRoyaleStart(useSpecialItem)
        self.question_dialog.Close()
        self.question_dialog = None
    
    def BattleRoyaleStart(self):
        if not self.question_dialog:
            self.question_dialog = uiCommon.QuestionDialog()
            self.question_dialog.SetText(localeInfo.BATTLE_ROYALE_QUESTION_USE_INFINITE_STAMINA)
            self.question_dialog.SetAcceptEvent(lambda arg=True: self.OnStartBattleRoyale(arg))
            self.question_dialog.SetCancelEvent(lambda arg=False: self.OnStartBattleRoyale(arg))
        
        if player.GetItemCountByVnum(battleRoyaleMgr.ITEM_VNUM_BATTLE_ROYALE_INFINITE_STAMINA) > 0:
            self.question_dialog.Open()
        else:
            self.OnStartBattleRoyale(False)

    def BattleRoyaleClaimReward(self):
        net.SendBattleRoyaleClaimReward()

    def ClickMainButton(self):
        self.Close()

        if self.main_button_state == self.MAIN_BUTTON_APPLICATION:
            self.ApplyBattleRoyale()
        elif self.main_button_state == self.MAIN_BUTTON_CANCEL:
            self.CancelApplicationBattleRoyale()
        elif self.main_button_state == self.MAIN_BUTTON_EXIT:
            self.ExitBattleRoyale()
        elif self.main_button_state == self.MAIN_BUTTON_STARTABLE:
            self.BattleRoyaleStart()
        elif self.main_button_state == self.MAIN_BUTTON_REWARD:
            self.BattleRoyaleClaimReward()

    def ClickRankingButton(self):
        self.Close()
        net.SendBattleRoyaleRanking()

    def Open(self):
        if player.GetBattleRoyaleEnable() == False:
            return
        
        if player.IsBattleRoyalePlaying():
            return
        
        self.Show()
        
        event.ClearEventSet(self.descIndex)
        self.descIndex = event.RegisterEventSet( uiScriptLocale.BATTLE_ROYALE_EVENT_DESC )
        event.SetFontColor( self.descIndex, 0.7843, 0.7843, 0.7843 )
        event.SetVisibleLineCount( self.descIndex, VISIBLE_LINE_COUNT )
        total_line = event.GetTotalLineCount(self.descIndex)
        
        if localeInfo.IsARABIC():
            event.SetEventSetWidth(self.descIndex, self.descBoard.GetWidth() - 20)
            
        event.SetRestrictedCount(self.descIndex, 60)

        if VISIBLE_LINE_COUNT >= total_line :
            self.prev_button.Hide()
            self.next_button.Hide()
        else :
            self.prev_button.Show()
            self.next_button.Show()
        
        if self.descriptionBox:
            self.descriptionBox.Show()

    def Close(self):
        self.Hide()
        event.ClearEventSet(self.descIndex)
        self.descIndex = -1
        
        if self.descriptionBox:
            self.descriptionBox.Hide()

        self.desc_y = DEFAULT_DESC_Y

    def OnUpdate(self):
        (xposEventSet, yposEventSet) = self.descBoard.GetGlobalPosition()
        event.UpdateEventSet(self.descIndex, xposEventSet+7, -(yposEventSet+self.desc_y))
        self.descriptionBox.SetIndex(self.descIndex)

    def PrevDescriptionPage(self):

        line_height			= event.GetLineHeight(self.descIndex) + 4
        if localeInfo.IsARABIC():
            line_height = line_height - 4
            
        cur_start_line		= event.GetVisibleStartLine(self.descIndex)
        
        decrease_count = VISIBLE_LINE_COUNT
        
        if cur_start_line - decrease_count < 0:
            return;

        event.SetVisibleStartLine(self.descIndex, cur_start_line - decrease_count)
        self.desc_y += ( line_height * decrease_count )

    def NextDescriptionPage(self):

        line_height			= event.GetLineHeight(self.descIndex) + 4
        if localeInfo.IsARABIC():
            line_height = line_height - 4
            
        total_line_count	= event.GetProcessedLineCount(self.descIndex)
        cur_start_line		= event.GetVisibleStartLine(self.descIndex)
        
        increase_count = VISIBLE_LINE_COUNT
        
        if cur_start_line + increase_count >= total_line_count:
            increase_count = total_line_count - cur_start_line

        if increase_count < 0 or cur_start_line + increase_count >= total_line_count:
            return
        
        event.SetVisibleStartLine(self.descIndex, cur_start_line + increase_count)
        self.desc_y -= ( line_height * increase_count )

    def OnPressEscapeKey(self):
        self.Close()
        return True

class BattleRoyaleInterface:
    def __init__(self):
        self.dead_wnd = BattleRoyaleDead()
        self.kill_wnd = BattleRoyaleKillInfo()
        self.winner_wnd = BattleRoyaleWinner()
        self.electric_field_damage_on_screen = None

    def __del__(self):
        self.dead_wnd = None
        self.kill_wnd = None
        self.winner_wnd = None
        self.electric_field_damage_on_screen = None

    def BattleRoyaleProcess(self, type, data):
        if type == player.BATTLE_ROYALE_GC_OPEN_KILL_INFO:
            self.kill_wnd.Open()
        elif type == player.BATTLE_ROYALE_GC_KILL_INFO:
            self.kill_wnd.SetInfo(data[0], data[1])
        elif type == player.BATTLE_ROYALE_GC_DEAD:
            self.dead_wnd.SetInfo(data[0], data[1])
            self.dead_wnd.Open()
        elif type == player.BATTLE_ROYALE_GC_WINNER:
            self.winner_wnd.SetInfo()
            self.winner_wnd.Open()

    def CreateElectricFieldOnScreenImage(self, parent_window):
        self.electric_field_damage_on_screen = ui.AniImageBox()
        self.electric_field_damage_on_screen.SetParent(parent_window)
        self.electric_field_damage_on_screen.SetDelay(6)
        self.electric_field_damage_on_screen.AppendImage("d:/ymir work/ui/game/battleroyale/electric_field_damage_on_screen_01.sub")
        self.electric_field_damage_on_screen.AppendImage("d:/ymir work/ui/game/battleroyale/electric_field_damage_on_screen_02.sub")
        self.electric_field_damage_on_screen.AppendImage("d:/ymir work/ui/game/battleroyale/electric_field_damage_on_screen_03.sub")
        self.electric_field_damage_on_screen.AppendImage("d:/ymir work/ui/game/battleroyale/electric_field_damage_on_screen_04.sub")
        self.electric_field_damage_on_screen.AppendImage("d:/ymir work/ui/game/battleroyale/electric_field_damage_on_screen_04.sub")
        self.electric_field_damage_on_screen.AppendImage("d:/ymir work/ui/game/battleroyale/electric_field_damage_on_screen_03.sub")
        self.electric_field_damage_on_screen.AppendImage("d:/ymir work/ui/game/battleroyale/electric_field_damage_on_screen_02.sub")
        self.electric_field_damage_on_screen.AppendImage("d:/ymir work/ui/game/battleroyale/electric_field_damage_on_screen_01.sub")
        self.electric_field_damage_on_screen.SetCenterPosition(-170, -170)

    def OpenElectricFieldOnScreenImage(self, open, parent_window):
        if not self.electric_field_damage_on_screen:
            self.CreateElectricFieldOnScreenImage(parent_window)

        if open:
            self.electric_field_damage_on_screen.Show()
        else:
            self.electric_field_damage_on_screen.Hide()