# coding: latin_1

import ui
import uiScriptLocale
import net
import time
import item
import snd
import localeinfo
import app
import chat
import mouseModule
import constInfo
import dbg
import os
import grp
import uiCommon
import uiToolTip

RUTA_IMG = "rework_offlineshop/"

class Edit(ui.EditLine):
    def __init__(self,main = "",ml = 99):
        ui.EditLine.__init__(self)
        self.SetText(main)
        self.main = main
        self.SetMax(ml)
        self.SetUserMax(ml)
    def GetText(self):
        res = ui.EditLine.GetText(self)
        if res == "":
            return "0"
        else:
            return res
            
    def __del__(self):
        ui.EditLine.__del__(self)
    def OnSetFocus(self):
        ui.EditLine.OnSetFocus(self)
        if ui.EditLine.GetText(self) == self.main:
            self.SetText("")
    def OnKillFocus(self):
        ui.EditLine.OnKillFocus(self)
        if ui.EditLine.GetText(self) == "":
            self.SetText(self.main)    
class Edit2(ui.EditLine):
    def __init__(self,parent,text,x,y,width,height,number=FALSE,slot=2,max = 12,right=False):
        ui.EditLine.__init__(self)
        self.imageSlot=ui.MakeImageBox(parent, RUTA_IMG+"slot_offline.tga", x,y)
        self.SetText(text)
        self.main = text

        self.SetMax(max)
        self.SetUserMax(max)
        self.SetParent(self.imageSlot)
        if number:
            self.SetNumberMode()
        
        self.SetSize(width,height)
        self.SetPosition(5,2)
        self.Show()
        if right:
            self.SetHorizontalAlignRight()
            self.SetWindowHorizontalAlignRight()
            self.SetPosition(5,2)
            
    def SetTexxxt(self, text):
        self.SetText(text)

    def GetText(self):
        res = ui.EditLine.GetText(self)
        if res == "":
            return ""
        else:
            return res
            
    def __del__(self):
        ui.EditLine.__del__(self)
        
    def OnSetFocus(self):
        ui.EditLine.OnSetFocus(self)
        if ui.EditLine.GetText(self) == self.main:
            self.SetText("")
            
    def SetEventForEdit(self, event):
        self.SetEscapeEvent(event)

    def OnKillFocus(self):
        ui.EditLine.OnKillFocus(self)
        if ui.EditLine.GetText(self) == "":
            self.SetText(self.main)
            
    # def OnUpdate(self):
        # self.SetHorizontalAlignRight()
        # self.SetHorizontalAlignCenter()

