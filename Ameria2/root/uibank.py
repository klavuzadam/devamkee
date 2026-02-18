# coding: latin_1

import ui,net,localeinfo,constInfo,app,uiScriptLocale
from uiUtils import Edit2 as Edit2

already_opened = 0

class BankDialog(ui.ScriptWindow):
    
    def __init__(self):
        ui.ScriptWindow.__init__(self)
        self.text = None
        self.LoadDialog()
        
    def __del__(self):
        ui.ScriptWindow.__del__(self)
    
        
    def Show(self):
        if self.board.IsShow():
            self.board.Hide()
        else:
            self.board.Show()
            ui.ScriptWindow.Show(self)
        
    def LoadDialog(self):
        global already_opened
        if already_opened > 0:
            return
            
        net.SendChatPacket("/bank_gold")
        
        self.board = ui.BoardWithTitleBar()
        self.board.AddFlag("movable")
        self.board.AddFlag("animate")
        self.board.SetCenterPosition()
        self.board.SetSize(300,150)
        self.board.SetTitleName(uiScriptLocale.BANK_NAME)
        self.board.SetCloseEvent(self.Close)
        self.board.Hide()
        
        self.stext = {}
        for i in xrange(3):
            self.stext[i] = ui.MakeText(self.board,(uiScriptLocale.BANK_1,uiScriptLocale.BANK_2,uiScriptLocale.BANK_3)[i],0,15*i+35,None)
            self.stext[i].SetWindowHorizontalAlignCenter()
            self.stext[i].SetHorizontalAlignCenter()
    
        self.text=Edit2(self.board,localeinfo.NumberToMoneyString(int(constInfo.shop_money)),50,45+40,100,250,FALSE,6,12,True)
        self.text.CanEdit(False)
        
        self.text2=Edit2(self.board,"0",50,45+30+40,100,25,True,4,9)

        self.btn = ui.MakeButton(self.board,(self.text.GetWidth()*2)+10,45+30+40,"","d:/ymir work/ui/public/","middle_button_01.sub","middle_button_02.sub","middle_button_03.sub")
        self.btn.SetEvent(lambda : self.GetGold())
        self.btn.SetText(uiScriptLocale.BANK_GET)
        self.btn.Show()
        
        # self.onClickKeyDict = {}
        # self.onClickKeyDict[app.DIK_ESC] = lambda : self.Close()
        
        already_opened = 1


    def OnPressEscapeKey(self):
        self.Close()
        return True
        
    def Close(self):
        global already_opened
        already_opened = 0
        if self.board != None:
            self.board.Hide()
            self.Hide()
            
    def OnUpdate(self):
        if self.text != None:
            # import dbg
            # dbg.TraceError(str(constInfo.shop_money))
            self.text.SetText(localeinfo.NumberToMoneyString(int(constInfo.shop_money)))
            
    def GetGold(self):
        self.cant = self.text2.GetText()
        if str(self.cant) == "0":
            return
        net.SendChatPacket("/bank_get_gold "+str(self.cant))

    # def OnKeyUp(self, key):
        # try:
            # self.onClickKeyDict[key]()
        # except KeyError:
            # pass
        # except:
            # raise
        # return TRUE

