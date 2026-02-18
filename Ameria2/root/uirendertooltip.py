# coding: latin_1

import ui
import dbg
import app
import player
import wndMgr
import renderTarget
import grp
import localeInfo
import chr
import item
import WikiUI

class RenderTooltip(ui.ScriptWindow):
    def __init__(self, tooltip):
        ui.ScriptWindow.__init__(self)
        
        self.tooltip = tooltip
        self.RENDER_TARGET_INDEX = 47
        self.defFontName = localeInfo.UI_DEF_FONT
        self.ModelPreviewBoard = None
        self.ModelPreview = None
        self.ModelPreviewText = None


        self.ModelTypeShow = None
        self.ModelVnum = None
        self.ModelValue3 = None


    def __del__(self):
        ui.ScriptWindow.__del__(self)

    def ShowRenderTooltip(self):
        if self.ModelTypeShow:
            if self.ModelTypeShow == 1:
                self.SetRenderWeapon()
            elif self.ModelTypeShow == 2:
                self.SetRenderArmor()
            elif self.ModelTypeShow == 3:
                self.SetRenderHair()
            elif self.ModelTypeShow == 4:
                self.SetRenderAcce()
            elif self.ModelTypeShow == 5:
                self.SetRenderModel()
            elif self.ModelTypeShow == 6:
                self.SetRenderShiningWeapon()
            elif self.ModelTypeShow == 7:
                self.SetRenderShiningArmor()


    def CloseRenderTooltip(self):
        if self.ModelPreviewBoard:
            self.ModelPreviewBoard.Hide()
            self.ModelPreview.Hide()
            self.ModelPreviewText.Hide()

            self.ModelPreviewBoard = None
            self.ModelPreview = None
            self.ModelPreviewText = None

            
            renderTarget.SetVisibility(self.RENDER_TARGET_INDEX, False)

    def RenderClearDates(self):
        self.ModelTypeShow = None
        self.ModelVnum = None
        self.ModelValue3 = None

    def FuncModelPreview(self, model):
        self.RENDER_TARGET_INDEX = renderTarget.GetFreeIndex(500,1000)

        self.ModelPreviewBoard = ui.ThinBoard()
        self.ModelPreviewBoard.SetParent(self.tooltip)
        self.ModelPreviewBoard.SetSize(190+10, 210+30)
        self.ModelPreviewBoard.SetPosition(-202, 0)
        self.ModelPreviewBoard.Show()

        self.ModelPreview = ui.RenderTarget()
        self.ModelPreview.SetParent(self.ModelPreviewBoard)
        self.ModelPreview.SetSize(190, 210)
        self.ModelPreview.SetPosition(5, 22)
        self.ModelPreview.SetRenderTarget(self.RENDER_TARGET_INDEX)
        self.ModelPreview.Show()

        self.ModelPreviewText = ui.TextLine()
        self.ModelPreviewText.SetParent(self.ModelPreviewBoard)
        self.ModelPreviewText.SetFontName(self.defFontName)
        self.ModelPreviewText.SetPackedFontColor(grp.GenerateColor(0.8824, 0.9804, 0.8824, 1.0))
        self.ModelPreviewText.SetPosition(0, 5)
        self.ModelPreviewText.SetText("Visualizar")
        self.ModelPreviewText.SetOutline()
        self.ModelPreviewText.SetFeather(False)
        self.ModelPreviewText.SetWindowHorizontalAlignCenter()
        self.ModelPreviewText.SetHorizontalAlignCenter()
        self.ModelPreviewText.Show()

        renderTarget.SetBackground(self.RENDER_TARGET_INDEX, "d:/ymir work/ui/game/myshop_deco/model_view_bg.sub")
        renderTarget.SetVisibility(self.RENDER_TARGET_INDEX, True)
        renderTarget.SelectModel(self.RENDER_TARGET_INDEX, model)


    def SetRenderDates(self, vnum, type, value3):
        if self.ModelTypeShow == type and self.ModelVnum == vnum and self.ModelValue3 == value3:
            return

        self.ModelTypeShow = type
        self.ModelVnum = vnum
        self.ModelValue3 = value3

        #if app.IsPressed(app.DIK_LALT):
        #    self.ShowRenderTooltip()

    def SetRenderModel(self):
        self.FuncModelPreview(self.ModelVnum)

    def SetRenderWeapon(self):
        self.FuncModelPreview(self.ItemGetRace(self.ModelVnum))
        renderTarget.SetWeapon(self.RENDER_TARGET_INDEX, self.ModelVnum)
        self.SetRenderMotion(chr.MOTION_WAIT)
        self.SetZoom(1100)

    def SetRenderArmor(self):
        self.FuncModelPreview(self.ItemGetRace(self.ModelVnum))
        renderTarget.SetArmor(self.RENDER_TARGET_INDEX, self.ModelVnum)
        renderTarget.SetHair(self.RENDER_TARGET_INDEX, 0)
        self.SetRenderMotion(chr.MOTION_WAIT)
        self.SetZoom(1100)

    def SetRenderAcce(self):
        self.FuncModelPreview(self.ItemGetRace(self.ModelVnum))

        #self.ModelVnum -= 85000;
        
        renderTarget.SetAcce(self.RENDER_TARGET_INDEX, self.ModelVnum - 85000)
        self.SetRenderMotion(chr.MOTION_WAIT)
        self.SetZoom(1000)

    def SetRenderHair(self):
        self.FuncModelPreview(self.ItemGetRace(self.ModelVnum))
        renderTarget.SetHair(self.RENDER_TARGET_INDEX, self.ModelValue3)
        self.SetRenderMotion(chr.MOTION_WAIT)
        self.SetZoom(1100)

    def SetRenderShiningWeapon(self):
        self.FuncModelPreview(self.ItemGetRace(self.ModelVnum))
        WikiUI.SetItemToModelPreview(self.RENDER_TARGET_INDEX, self.ModelVnum)
        #self.FuncModelPreview(self.ItemGetRace(self.GetWeaponShining()))
        #renderTarget.SetWeapon(self.RENDER_TARGET_INDEX, self.GetWeaponShining())
        #renderTarget.SetShiningWeapon(self.RENDER_TARGET_INDEX, self.ModelVnum)
        #self.SetRenderMotion(chr.MOTION_WAIT)
        #self.SetZoom(1100)

    def GetWeaponShining(self):
        job = player.GetJob()
        list_items = [180, 1130, 190, 5120]
        return list_items[job]

    def SetRenderShiningArmor(self):
        self.FuncModelPreview(self.ItemGetRace(self.ModelVnum))
        WikiUI.SetItemToModelPreview(self.RENDER_TARGET_INDEX, self.ModelVnum)

        #renderTarget.SetShiningArmor(self.RENDER_TARGET_INDEX, self.ModelVnum)
        #self.SetRenderMotion(chr.MOTION_WAIT)
        #self.SetZoom(1100)

    def IsRenderTooltip(self):
        if self.ModelTypeShow != None:
            return True

        return False

    def SetZoom(self,value):
        return
        #renderTarget.SetZoom(self.RENDER_TARGET_INDEX, float(value))

    def SetPosition(self, value):
        return
        #renderTarget.SetPosition(self.RENDER_TARGET_INDEX, float(value))

    def SetRenderMotion(self, motion):
        #renderTarget.SetMotion(self.RENDER_TARGET_INDEX, motion)
        return

    def ItemGetRace(self,itemVnum):
        race = 0

        item.SelectItem(itemVnum)

        if item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN):
            race = 9
        elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN):
            race = 1
        elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN):
            race = 2
        elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA):
            race = 3

        sex = chr.RaceToSex(player.GetRace())
        MALE = 1
        FEMALE = 0

        if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE) and sex == MALE:
            race = player.GetRace() + 4

        if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE) and sex == FEMALE:
            race = player.GetRace()

        if race == 0:
            race = player.GetRace()

        if race == 9:
            race = 0

        return race

