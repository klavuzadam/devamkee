# coding: latin_1

import ui, renderTarget, app, item, chr, playersettingmodule

IMG_DIR = "d:/ymir work/ui/game/battle_pass/"

class ShopRender(ui.Board):
    def __del__(self):
        ui.Board.__del__(self)
    def __init__(self):
        ui.Board.__init__(self)
        self.Destroy()
        self.LoadWindow()
    def Destroy(self):
        self.children = {}
        self.modelIndex = 0

    def LoadWindow(self):
        self.SetSize(339,455)
        self.SetCenterPosition()
        self.modelIndex = 999

        titleBar = ui.TitleBar()
        titleBar.SetParent(self)
        titleBar.SetPosition(10,10)
        titleBar.MakeTitleBar(self.GetWidth()-10-10, "red")
        titleBar.SetCloseEvent(self.Close)
        titleBar.Show()
        self.children["titleBar"] = titleBar

        render = ui.RenderTarget()
        render.SetParent(self)
        render.SetPosition(10,30)
        render.SetSize(titleBar.GetWidth(), self.GetHeight()-40)
        render.SetRenderTarget(self.modelIndex)
        renderTarget.SelectModel(self.modelIndex, 1)
        renderTarget.SetVisibility(self.modelIndex, True)
        renderTarget.SetBackground(self.modelIndex, IMG_DIR+"preview_back.tga")
        render.Show()
        self.children["render"] = render


    def Close(self):
        self.Hide()
        return True

    def CanEquipItem(self, raceIndex):
        ANTI_FLAG_DICT = {
            0 : item.ITEM_ANTIFLAG_WARRIOR,
            1 : item.ITEM_ANTIFLAG_ASSASSIN,
            2 : item.ITEM_ANTIFLAG_SURA,
            3 : item.ITEM_ANTIFLAG_SHAMAN,
        }
        job = chr.RaceToJob(raceIndex)
        sex = chr.RaceToSex(raceIndex)
        MALE = 1
        FEMALE = 0
        if item.IsAntiFlag(ANTI_FLAG_DICT[job]):
            return 1
        elif item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE) and sex == MALE:
            return 2
        elif item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE) and sex == FEMALE:
            return 2
        return 0

    def GetValidRace(self, raceIndex = 0):
        can_equip = self.CanEquipItem(raceIndex)
        race = raceIndex
        sex = chr.RaceToSex(race)
        MALE = 1
        FEMALE = 0
        if can_equip == 0:
            return race
        elif can_equip == 1:
            if item.GetItemType() == item.ITEM_TYPE_COSTUME and item.GetItemSubType() == item.COSTUME_TYPE_WEAPON:
                raceDict = {
                    0 :    [ playersettingmodule.RACE_WARRIOR_W, playersettingmodule.RACE_WARRIOR_M, ],
                    1 :    [ playersettingmodule.RACE_ASSASSIN_W, playersettingmodule.RACE_ASSASSIN_M ],
                    2 :    [ playersettingmodule.RACE_ASSASSIN_W, playersettingmodule.RACE_ASSASSIN_M ],
                    3 :    [ playersettingmodule.RACE_WARRIOR_W, playersettingmodule.RACE_WARRIOR_M, ],
                    4 :    [ playersettingmodule.RACE_SHAMAN_W, playersettingmodule.RACE_SHAMAN_M ],
                    5 :    [ playersettingmodule.RACE_SHAMAN_W, playersettingmodule.RACE_SHAMAN_M ],
                }
                item_type = item.GetValue(3)
                return raceDict[item_type][sex]
            else:
                raceDict = {
                    0 :    [ playersettingmodule.RACE_WARRIOR_W, playersettingmodule.RACE_WARRIOR_M ],
                    1 :    [ playersettingmodule.RACE_ASSASSIN_W, playersettingmodule.RACE_ASSASSIN_M ],
                    2 :    [ playersettingmodule.RACE_SURA_W, playersettingmodule.RACE_SURA_M ],
                    3 :    [ playersettingmodule.RACE_SHAMAN_W, playersettingmodule.RACE_SHAMAN_M ],
                }
                flags = []
                ANTI_FLAG_DICT = {
                    0 : item.ITEM_ANTIFLAG_WARRIOR,
                    1 : item.ITEM_ANTIFLAG_ASSASSIN,
                    2 : item.ITEM_ANTIFLAG_SURA,
                    3 : item.ITEM_ANTIFLAG_SHAMAN,
                }
                for i in xrange(len(ANTI_FLAG_DICT)):
                    if not item.IsAntiFlag(ANTI_FLAG_DICT[i]):
                        flags.append(i)
                if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
                    sex = FEMALE
                if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
                    sex = MALE
                return raceDict[flags[0]][sex] if len(flags) == 1 else 0
        elif can_equip == 2:
            return self.GetOtherSexRace(race)

    def SetItemToModelPreview(self, itemVnum):
        item.SelectItem(itemVnum)
        if item.GetItemType() == item.ITEM_TYPE_COSTUME and (item.GetItemSubType() == item.COSTUME_TYPE_MOUNT or (app.ENABLE_MOUNT_SKIN and itemSubType == item.COSTUME_TYPE_MOUNT_SKIN)):
            renderTarget.SelectModel(self.modelIndex, item.GetValue(0))
        else:
            raceIndex = self.GetValidRace(app.GetRandom(0,4))
            renderTarget.SelectModel(self.modelIndex, raceIndex)
            if item.GetItemType() == item.ITEM_TYPE_WEAPON:
                renderTarget.SetArmor(self.modelIndex,11299)
                renderTarget.SetWeapon(self.modelIndex,itemVnum)
            elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
                renderTarget.SetArmor(self.modelIndex,itemVnum)
            elif item.GetItemType() == item.ITEM_TYPE_COSTUME:
                if item.GetItemSubType() == item.COSTUME_TYPE_WEAPON:
                    renderTarget.SetArmor(self.modelIndex,11299)
                    renderTarget.SetWeapon(self.modelIndex,itemVnum)
                elif item.GetItemSubType() == item.COSTUME_TYPE_ARMOR:
                    renderTarget.SetArmor(self.modelIndex,itemVnum)
                elif item.GetItemSubType() == item.COSTUME_TYPE_HAIR:
                    renderTarget.SetArmor(self.modelIndex,11299)
                    renderTarget.SetHair(self.modelIndex,itemVnum)
        renderTarget.SetVisibility(self.modelIndex, True)

    def Open(self, itemVnum):
        self.SetItemToModelPreview(itemVnum)
        self.Show()
